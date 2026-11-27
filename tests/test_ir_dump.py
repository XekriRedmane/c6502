"""Tests for `ir_dump`, the dump format of the four IRs. Its agreement
with the C++ port's reader and writer on the dumps of tests/ir_dump/ is
checked by cpp/tests/test_ir_dump_files.cpp."""
from __future__ import annotations

import os
import unittest
from pathlib import Path

import asm_ast
import c99_ast
import tac_ast
from ir_dump import DumpError, dump, load


_ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
_SAMPLES = Path(__file__).resolve().parent / "ir_dump"


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


def _sample_irs() -> dict[str, str]:
    """Returns the dump of each of the four IRs of
    examples/paint_hud_strip_p1.c compiled with -O, by IR module, as
    tests/ir_dump/ holds them. The Python compiler wrote them on 25 Nov
    2026 (the c99 program after the type checker, the TAC from
    `translate_for_codegen`, and the asm and asm2 programs that
    `lower_to_asm2` takes and gives), so the test reads the IRs of a real
    program without a compiler."""
    return {
        module: (_SAMPLES / f"paint_hud_strip_p1.{module}.txt").read_bytes()
        .decode("utf-8")
        for module in ("c99_ast", "tac_ast", "asm_ast", "asm2_ast")
    }


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
    """Reads and dumps again the four IRs of a compiled example."""

    @classmethod
    def setUpClass(cls):
        cls.texts = _sample_irs()

    def test_python_round_trip(self):
        for module, text in self.texts.items():
            with self.subTest(module=module):
                prog = load(text, module)
                self.assertEqual(dump(prog), text)
                self.assertEqual(load(dump(prog), module), prog)


if __name__ == "__main__":
    unittest.main()
