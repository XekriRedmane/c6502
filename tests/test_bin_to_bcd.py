"""Tests for `__c6502_bin_to_bcd` (builtin_functions.py,
docs/builtins.md) and the runtime helper behind it, `bin16_to_bcd`
(rtlib/helpers.py).

The helper's 6502 code is run on py65 for every 16-bit input and
compared with its Python hook in the simulator, and every input is run
through the builtin in both pipelines too. The optimizer is checked
for the constants it folds, on the TAC that `compile.py --tac` prints.

The tests that called the Python functions directly are doctest cases of
the C++ port now, each named for the test it came from
(docs/cpp_port.md, step 8, group G6): the front end's type and errors
in cpp/tests/test_bin_to_bcd_front_end.cpp, the helper's size and HARGS
footprint in test_runtime_layout.cpp, and the IR check's decimal-mode
rule in test_bin_to_bcd_ir_check.cpp.
"""
from __future__ import annotations

import shutil
import unittest

import builtin_functions as bf
import tac_ast
from preprocessor import preprocess
from sim.harness import build_sim
from tests.test_bcd_builtins import _codegen
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

    def test_cycles(self) -> None:
        # docs/builtins.md gives the cycles, and the helper's size, which
        # is a doctest case of the C++ port. The loop takes the same
        # time for every input: 712 cycles with the JSR and the RTS,
        # and 15 more when the loop's BNE crosses a page, as it does
        # where the simulator puts the helper.
        from sim import runtime as rt
        run = _helper_runner()
        addr = rt.build_runtime().symbols["bin16_to_bcd"]
        # The loop starts 19 bytes in, and its BNE ends 44 bytes in.
        crosses = (addr + 19) >> 8 != (addr + 44) >> 8
        self.assertEqual({run(v)[2] for v in (0, 1, 9999, 65535)},
                         {712 + 15 * crosses})


def _tac(src: str, optimize: bool) -> tac_ast.Program:
    """The TAC that `compile.py --tac` prints for `src`, read back into
    `tac_ast` objects. Its `pretty` text is the constructors' calls with
    their fields as keywords, so Python can evaluate it."""
    from compile import _run_stage
    return eval(_run_stage("tac", preprocess(src), optimize=optimize),
                vars(tac_ast))


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
