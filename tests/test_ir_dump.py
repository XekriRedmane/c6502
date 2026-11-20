"""Tests for `ir_dump`, the dump format of the four IRs, and for its
agreement with the C++ port's reader and writer (cpp/)."""
from __future__ import annotations

import os
import subprocess
import unittest

import asm_ast
import c99_ast
import tac_ast
from ir_dump import DumpError, dump, load


_ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
_EXAMPLE = os.path.join(_ROOT, "examples", "paint_hud_strip_p1.c")


def cpp_cli() -> str | None:
    """Returns the path of the C++ port's CLI if it has been built
    (cpp/README.md), or None. `C6502_CPP` names another one."""
    env = os.environ.get("C6502_CPP")
    if env:
        return env
    for name in ("c6502cpp.exe", "c6502cpp"):
        path = os.path.join(_ROOT, "cpp", "build", "bin", name)
        if os.path.exists(path):
            return path
    return None


A = asm_ast.Reg(reg=asm_ast.A())


def _sample_asm() -> asm_ast.Program:
    return asm_ast.Program(top_level=[
        asm_ast.Function(
            name="f", is_global=True, params=["@0.a", 'odd "name"\\'],
            instructions=[
                asm_ast.Label(name=".f@start"),
                asm_ast.Mov(src=asm_ast.Imm(value=-5), dst=A, is_volatile=False),
                asm_ast.Mov(src=asm_ast.IndirectY(),
                            dst=asm_ast.Data(name="x", offset=1),
                            is_volatile=True),
                asm_ast.Call(name="g", reg_args=["A", "X"], preserves=[]),
                asm_ast.Ret(arg_bytes=0, local_bytes=2, save_a=True,
                            save_x=False, callee_saved_addrs=[0xC0, 0xC1],
                            save_c=False),
            ],
        ),
        asm_ast.StaticVariable(name="s", is_global=False, init=[
            asm_ast.StringInit(str="a\n\xe9\u20ac", bytes=6),
            asm_ast.DoubleInit(bits=2**64 - 1),
            asm_ast.ZeroInit(bytes=3),
        ]),
        asm_ast.Function(name="empty", is_global=False, params=[],
                         instructions=[]),
    ])


# The text `_sample_asm` dumps as. cpp/tests/test_ir.cpp builds the
# same program and checks that the C++ writer gives the same text.
_SAMPLE_TEXT = (
    'Program([\n'
    '  Function("f" true ["@0.a" "odd \\"name\\"\\\\"] [\n'
    '    Label(".f@start")\n'
    '    Mov(Imm(-5) Reg(A) false)\n'
    '    Mov(IndirectY Data("x" 1) true)\n'
    '    Call("g" ["A" "X"] [])\n'
    '    Ret(0 2 true false [192 193] false)\n'
    '  ])\n'
    '  StaticVariable("s" false [StringInit("a\\x0a\\xe9\\u{20ac}" 6) '
    'DoubleInit(18446744073709551615) ZeroInit(3)])\n'
    '  Function("empty" false [] [])\n'
    '])\n'
)


def _compiled_irs():
    """Returns the c99, TAC, asm, and asm2 programs of an example,
    compiled with -O."""
    import compile as compile_module
    from preprocessor import preprocess
    from passes.type_checking import check_program
    from passes.zp_globals import plan_zero_page, zero_page_in_effect

    with open(_EXAMPLE) as f:
        source = preprocess(f.read())
    resolved = compile_module._resolved(source, optimize=True)
    prog, symbols, types = check_program(resolved)
    tac = compile_module.translate_for_codegen(prog, symbols, types, True)
    seen = {}
    real_lower = compile_module.lower_to_asm2

    def lower(asm):
        seen["asm"] = asm
        seen["asm2"] = real_lower(asm)
        return seen["asm2"]

    compile_module.lower_to_asm2 = lower
    try:
        zp = plan_zero_page(tac, symbols, types, promote=True)
        with zero_page_in_effect(zp):
            compile_module._codegen(prog, symbols, types, True, zp, tac)
    finally:
        compile_module.lower_to_asm2 = real_lower
    return {"c99_ast": prog, "tac_ast": tac, "asm_ast": seen["asm"],
            "asm2_ast": seen["asm2"]}


class TestFormat(unittest.TestCase):
    def test_sample_text(self):
        self.assertEqual(dump(_sample_asm()), _SAMPLE_TEXT)

    def test_round_trip(self):
        prog = _sample_asm()
        back = load(dump(prog), asm_ast)
        self.assertEqual(back, prog)
        self.assertEqual(dump(back), dump(prog))

    def test_any_value_names_its_type(self):
        self.assertEqual(dump(A), "Reg(A)\n")
        self.assertEqual(load("Reg(A)", "asm_ast"), A)
        member = c99_ast.Type_member_decl(name="m", data_type=c99_ast.UChar())
        self.assertEqual(dump(member), 'member_decl("m" UChar)\n')
        self.assertEqual(load(dump(member), c99_ast), member)

    def test_none_and_optional(self):
        self.assertEqual(dump(tac_ast.Ret(val=None)), "Ret(None)\n")
        self.assertEqual(load("Ret(None)", tac_ast), tac_ast.Ret(val=None))
        ret = tac_ast.Ret(val=tac_ast.Var(name="None"))
        self.assertEqual(load(dump(ret), tac_ast), ret)

    def test_strings_with_any_character(self):
        s = "".join(chr(c) for c in range(0x180)) + "\U0001f600\ud800"
        v = asm_ast.Label(name=s)
        self.assertEqual(load(dump(v), asm_ast), v)

    def test_large_and_negative_ints(self):
        for n in (0, -1, 2**63, -(2**63), 2**64 - 1, 10**30):
            v = tac_ast.Constant(const=tac_ast.ConstULongLong(value=n))
            self.assertEqual(load(dump(v), tac_ast), v)

    def test_bool_and_int_fields(self):
        # Python takes True and 1 as equal, so either may stand in for
        # the other, and the value read back is equal.
        v = asm_ast.Imm(value=True)
        self.assertEqual(dump(v), "Imm(1)\n")
        self.assertEqual(load(dump(v), asm_ast), v)
        m = asm_ast.Mov(src=A, dst=A, is_volatile=0)
        self.assertEqual(dump(m), "Mov(Reg(A) Reg(A) false)\n")

    def test_rejects_values_it_cant_hold(self):
        with self.assertRaises(DumpError):
            dump(asm_ast.Mov(src=None, dst=A, is_volatile=False))
        with self.assertRaises(DumpError):
            dump(asm_ast.Call(name="f", reg_args=("A",), preserves=[]))
        with self.assertRaises(DumpError):
            dump(asm_ast.Reg(reg=asm_ast.Imm(value=1)))
        with self.assertRaises(DumpError):
            dump(asm_ast.Mov(src=A, dst=A, is_volatile=2))

    def test_rejects_bad_text(self):
        for text in ("Data(\"x\")", "Nope(1)", "Imm(1) Imm(2)", "Imm(x)",
                     "Label(\"\\q\")", "Reg(Imm(1))", "Imm(1"):
            with self.subTest(text=text), self.assertRaises(DumpError):
                load(text, asm_ast)

    def test_whitespace_between_tokens(self):
        self.assertEqual(load('  Data( "x"\n -3 )  ', asm_ast),
                         asm_ast.Data(name="x", offset=-3))


class TestRealPrograms(unittest.TestCase):
    """Dumps and reads back the four IRs of a compiled example, and,
    when the C++ CLI is built, has it read and dump each one again."""

    @classmethod
    def setUpClass(cls):
        cls.irs = _compiled_irs()

    def test_python_round_trip(self):
        for module, prog in self.irs.items():
            with self.subTest(module=module):
                text = dump(prog)
                back = load(text, module)
                self.assertEqual(back, prog)
                self.assertEqual(dump(back), text)

    @unittest.skipUnless(cpp_cli(), "the C++ CLI isn't built (cpp/README.md)")
    def test_cpp_reads_and_writes_the_same_bytes(self):
        texts = {m: dump(p) for m, p in self.irs.items()}
        texts["asm_ast sample"] = _SAMPLE_TEXT
        for key, text in texts.items():
            module = key.split()[0]
            with self.subTest(module=key):
                out = subprocess.run(
                    [cpp_cli(), "roundtrip", module],
                    input=text.encode("utf-8"), capture_output=True,
                    check=True,
                ).stdout.decode("utf-8")
                self.assertEqual(out, text)


if __name__ == "__main__":
    unittest.main()
