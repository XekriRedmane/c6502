"""Tests for `__c6502_bin_to_bcd` (builtin_functions.py,
docs/builtins.md) and the runtime helper behind it, `bin16_to_bcd`
(rtlib/helpers.py).

The helper's 6502 code is run on py65 for every 16-bit input and
compared with its Python hook in the simulator, and every input is run
through the builtin in both pipelines too. The front end is checked
for the type it gives and the errors it reports, the optimizer for
the constants it folds, and the IR check for the decimal-mode rule
that the helper follows in place of the one for compiled code.
"""
from __future__ import annotations

import shutil
import unittest

import asm2_ast
import asm_ast
import builtin_functions as bf
import tac_ast
from passes import ir_check
from passes.identifier_resolution import IdentifierResolutionError
from passes.type_checking import TypeCheckError
from preprocessor import preprocess
from sim.harness import build_sim
from tests.test_bcd_builtins import _builtin_calls, _check, _codegen, _tac
from tests.test_sim_chapters import _run_program as _run_tac

_HAVE_PCPP = shutil.which("pcpp") is not None


class TestModel(unittest.TestCase):
    def test_every_value(self) -> None:
        for v in range(1 << 16):
            self.assertEqual(bf.bin_to_bcd(v), int(f"{v:05d}", 16))

    def test_takes_the_low_16_bits(self) -> None:
        self.assertEqual(bf.bin_to_bcd(-1), 0x65535)
        self.assertEqual(bf.bin_to_bcd(70000), 0x04464)


def _helper_runner():
    """A py65 MPU with the 6502 helpers at $E000 and `JSR bin16_to_bcd;
    BRK` at $0200, and a function that runs the helper on one input and
    returns HARGS+0..6, the processor status, and the cycles the call
    took with its `JSR` and `RTS`."""
    from py65.devices.mpu6502 import MPU
    from sim import runtime as rt
    runtime = rt.build_runtime()
    mpu = MPU()
    base = runtime.helper_origin
    mpu.memory[base:base + len(runtime.helper_image)] = runtime.helper_image
    addr = runtime.symbols["bin16_to_bcd"]
    mpu.memory[0x200:0x204] = [0x20, addr & 0xFF, addr >> 8, 0x00]
    h = rt.HARGS

    def run(v: int) -> tuple[list[int], int, int]:
        mpu.memory[h:h + 7] = [v & 0xFF, v >> 8, 0x5A, 0xA5, 0x3C, 0xC3, 0x99]
        mpu.pc = 0x200
        mpu.sp = 0xFF
        mpu.p = mpu.UNUSED
        start = mpu.processorCycles
        while mpu.pc != 0x203:
            mpu.step()
        return (list(mpu.memory[h:h + 7]), mpu.p,
                mpu.processorCycles - start)
    return run


def _hook_bytes(v: int) -> list[int]:
    from sim import runtime as rt
    mem = bytearray(0x100)
    mem[rt.HARGS:rt.HARGS + 2] = bytes([v & 0xFF, v >> 8])
    rt._bin16_to_bcd_hook()(mem)
    return list(mem[rt.HARGS:rt.HARGS + 7])


class TestHelper(unittest.TestCase):
    """The 6502 code against the hook on every input, in four shards
    so that parallel workers can share them."""

    def _shard(self, k: int) -> None:
        run = _helper_runner()
        bad = []
        for v in range(k << 14, (k + 1) << 14):
            got, p, _ = run(v)
            if got != _hook_bytes(v) or p & 0x08:
                bad.append((v, got, p))
        self.assertEqual(bad[:5], [])

    def test_inputs_0000_3FFF(self) -> None:
        self._shard(0)

    def test_inputs_4000_7FFF(self) -> None:
        self._shard(1)

    def test_inputs_8000_BFFF(self) -> None:
        self._shard(2)

    def test_inputs_C000_FFFF(self) -> None:
        self._shard(3)

    def test_size_and_cycles(self) -> None:
        # docs/builtins.md gives these. The loop takes the same time
        # for every input: 712 cycles with the JSR and the RTS, and 15
        # more when the loop's BNE crosses a page, as it does where
        # the simulator puts the helper.
        from passes.runtime_library import helper_sizes
        from sim import runtime as rt
        self.assertEqual(helper_sizes()["bin16_to_bcd"], 46)
        run = _helper_runner()
        addr = rt.build_runtime().symbols["bin16_to_bcd"]
        # The loop starts 19 bytes in, and its BNE ends 44 bytes in.
        crosses = (addr + 19) >> 8 != (addr + 44) >> 8
        self.assertEqual({run(v)[2] for v in (0, 1, 9999, 65535)},
                         {712 + 15 * crosses})

    def test_footprint(self) -> None:
        from passes.runtime_layout import helper_footprints
        self.assertEqual(helper_footprints()["bin16_to_bcd"], 7)


class TestFrontEnd(unittest.TestCase):
    def test_result_type(self) -> None:
        for decl in ("uint8_t a;", "int16_t a;", "uint32_t a;", "bool a;",
                     "char a;", "_BitInt(12) a;", "enum { A } a;"):
            with self.subTest(decl=decl):
                src = (
                    "#include <stdint.h>\n" + decl
                    + " unsigned long g(void)"
                    " { return __c6502_bin_to_bcd(a); }"
                )
                prog, _s, _t = _check(src)
                [call] = _builtin_calls(prog)
                self.assertEqual(type(call.data_type).__name__, "ULong")
                self.assertEqual(
                    type(call.args[0].data_type).__name__, "UInt",
                )

    def test_type_errors(self) -> None:
        cases = [
            ("double d;", "__c6502_bin_to_bcd(d)", "integer type"),
            ("uint8_t *p;", "__c6502_bin_to_bcd(p)", "integer type"),
            ("uint8_t t[2];", "__c6502_bin_to_bcd(t)", "integer type"),
            ("struct s { int x; } v;", "__c6502_bin_to_bcd(v)",
             "integer type"),
            ("", "__c6502_bin_to_bcd()", "takes 1 arguments"),
            ("", "__c6502_bin_to_bcd(1, 2)", "takes 1 arguments"),
        ]
        for decls, call, message in cases:
            with self.subTest(call=call):
                src = (
                    "#include <stdint.h>\n" + decls
                    + f" void f(void) {{ {call}; }}"
                )
                with self.assertRaises(TypeCheckError) as cm:
                    _check(src)
                self.assertIn(message, str(cm.exception))

    def test_names_are_reserved(self) -> None:
        cases = [
            ("unsigned long __c6502_bin_to_bcd(unsigned x);",
             "can't be declared"),
            ("void f(void) { int __c6502_bin_to_bcd = 1; }",
             "can't be declared"),
            ("void *p = __c6502_bin_to_bcd;", "can only be called"),
        ]
        for src, message in cases:
            with self.subTest(src=src):
                with self.assertRaises(IdentifierResolutionError) as cm:
                    _check(src)
                self.assertIn(message, str(cm.exception))

    def test_constant_argument_makes_a_constant_expression(self) -> None:
        src = (
            "#include <stdint.h>\n"
            "static uint32_t s = __c6502_bin_to_bcd(12345);"
            "uint32_t t = __c6502_bin_to_bcd(-1);"
            "uint32_t u = __c6502_bin_to_bcd(70000L);"
            "int a[__c6502_bin_to_bcd(10)];"
            "int f(uint8_t k) { switch (k) {"
            " case __c6502_bin_to_bcd(42): return 1; }"
            " return sizeof a; }"
        )
        _prog, symbols, _types = _check(src)
        self.assertEqual(symbols["s"].attrs.initial_value.value, 0x12345)
        self.assertEqual(symbols["t"].attrs.initial_value.value, 0x65535)
        self.assertEqual(symbols["u"].attrs.initial_value.value, 0x04464)
        self.assertEqual(symbols["a"].type.size, 0x10)


def _unaries(prog: tac_ast.Program, fn: str) -> list:
    for tl in prog.top_level:
        if isinstance(tl, tac_ast.Function) and tl.name == fn:
            return [i for i in tl.instructions if isinstance(i, tac_ast.Unary)]
    raise KeyError(fn)


_SUM = """
#include <stdint.h>
uint16_t v[4] = {0, 9, 65535, 31415};
uint32_t f(uint16_t x) { return __c6502_bin_to_bcd(x); }
int main(void) {
    uint32_t s = 0;
    for (uint8_t i = 0; i < 4; i++) s ^= f(v[i]) + i;
    return (int)(s ^ (s >> 16));
}
"""


def _sum_expected() -> int:
    s = 0
    for i, x in enumerate((0, 9, 65535, 31415)):
        s ^= (bf.bin_to_bcd(x) + i) & 0xFFFFFFFF
    return (s ^ (s >> 16)) & 0xFFFF


@unittest.skipUnless(_HAVE_PCPP, "pcpp CLI not available")
class TestTac(unittest.TestCase):
    def test_lowers_to_a_unary(self) -> None:
        src = (
            "#include <stdint.h>\nuint16_t a;"
            "uint32_t f(void) { return __c6502_bin_to_bcd(a); }"
        )
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                ops = [type(u.op) for u in _unaries(_tac(src, optimize), "f")]
                self.assertEqual(ops, [tac_ast.BinToBcd])
                self.assertIn("JSR   bin16_to_bcd", _codegen(src, optimize))

    def test_constants_fold_in_both_pipelines(self) -> None:
        src = (
            "#include <stdint.h>\n"
            "uint32_t f(void) { return __c6502_bin_to_bcd(54321); }"
        )
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                self.assertEqual(_unaries(_tac(src, optimize), "f"), [])
                asm = _codegen(src, optimize)
                self.assertNotIn("bin16_to_bcd", asm)
                self.assertIn("#$21", asm)

    def test_propagated_constant_folds(self) -> None:
        src = (
            "#include <stdint.h>\n"
            "uint32_t f(void) { uint16_t x = 999; x++;"
            " return __c6502_bin_to_bcd(x); }"
        )
        self.assertEqual(_unaries(_tac(src, optimize=True), "f"), [])
        self.assertNotIn("bin16_to_bcd", _codegen(src, optimize=True))

    def test_unused_result_goes(self) -> None:
        src = (
            "#include <stdint.h>\nuint16_t a;"
            "void f(void) { (void)__c6502_bin_to_bcd(a); }"
        )
        self.assertEqual(_unaries(_tac(src, optimize=True), "f"), [])

    def test_tac_sim(self) -> None:
        self.assertEqual(_run_tac(preprocess(_SUM)) & 0xFFFF, _sum_expected())


class TestIrCheck(unittest.TestCase):
    def setUp(self) -> None:
        self._was = ir_check.enabled()
        ir_check.enable()

    def tearDown(self) -> None:
        ir_check.enable(self._was)

    def test_helpers_pass(self) -> None:
        from passes.asm_to_asm2 import translate_program
        from rtlib.helpers import all_helper_functions
        translate_program(asm_ast.Program(top_level=all_helper_functions()))

    def test_helper_without_cld_fails(self) -> None:
        from passes.asm_to_asm2 import translate_program
        from rtlib.helpers import bin16_to_bcd_function
        fn = bin16_to_bcd_function()
        fn.instructions = [
            i for i in fn.instructions
            if not isinstance(i, asm_ast.ClearDecimal)
        ]
        with self.assertRaises(ir_check.IRCheckError) as cm:
            translate_program(asm_ast.Program(top_level=[fn]))
        self.assertIn("without a CLD", str(cm.exception))

    def test_other_functions_keep_the_chain_rule(self) -> None:
        fn = asm2_ast.Function(
            name="f", is_global=True, params=[],
            instructions=[asm2_ast.SetDecimal(), asm2_ast.ClearDecimal(),
                          asm2_ast.Return()],
        )
        with self.assertRaises(ir_check.IRCheckError):
            ir_check.check_asm2(asm2_ast.Program(top_level=[fn]), after="t")

    def test_compiled_code_may_not_set_the_flag(self) -> None:
        fn = asm_ast.Function(
            name="f", is_global=True, params=[],
            instructions=[asm_ast.SetDecimal(), asm_ast.ClearDecimal()],
        )
        with self.assertRaises(ir_check.IRCheckError) as cm:
            ir_check.check_asm(asm_ast.Program(top_level=[fn]), after="t")
        self.assertIn("outside a runtime helper", str(cm.exception))


_BATCH = 8192


def _batch_source(start: int) -> str:
    """A program that stores the three BCD bytes of each value from
    `start` to `start + _BATCH - 1` in `out`."""
    return f"""
#include <stdint.h>
uint8_t out[{3 * _BATCH}];
int main(void) {{
    uint8_t *p = out;
    uint16_t v = {start};
    do {{
        uint32_t b = __c6502_bin_to_bcd(v);
        p[0] = (uint8_t)b;
        p[1] = (uint8_t)(b >> 8);
        p[2] = (uint8_t)(b >> 16);
        if ((uint8_t)(b >> 24) != 0) return 1;
        p += 3;
        v++;
    }} while (v != (uint16_t){start + _BATCH});
    return 0;
}}
"""


@unittest.skipUnless(_HAVE_PCPP, "pcpp CLI not available")
class TestSimulator(unittest.TestCase):
    """Every 16-bit value through the builtin, in batches, in both
    pipelines."""

    def _batch(self, k: int, optimize: bool) -> None:
        start = k * _BATCH
        sim = build_sim(_batch_source(start), optimize=optimize)
        result = sim.run(max_cycles=40_000_000)
        self.assertFalse(result.timed_out)
        self.assertFalse(sim.mpu.p & sim.mpu.DECIMAL)
        self.assertEqual(result.return_int(), 0)
        base = sim.symbols["out"]
        got = bytes(result.memory[base:base + 3 * _BATCH])
        want = b"".join(
            bf.bin_to_bcd(v).to_bytes(3, "little")
            for v in range(start, start + _BATCH)
        )
        if got != want:
            k = next(i for i in range(len(got)) if got[i] != want[i]) // 3
            self.fail(f"first difference at {start + k}")

    def test_sum(self) -> None:
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                result = build_sim(_SUM, optimize=optimize).run()
                self.assertEqual(result.return_int() & 0xFFFF,
                                 _sum_expected())


for _k in range((1 << 16) // _BATCH):
    for _opt in (False, True):
        def _test(self, k=_k, opt=_opt) -> None:
            self._batch(k, opt)
        _name = f"test_batch_{_k}_{'O' if _opt else 'unopt'}"
        _test.__name__ = _name
        setattr(TestSimulator, _name, _test)


if __name__ == "__main__":
    unittest.main()
