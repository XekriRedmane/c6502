"""Tests for the `bool` type (C23 §6.2.5, C99 §6.2.5.2).

tests/test_c23_bool.py covers what C23 added: the keywords `bool`,
`true`, and `false`, the warning for `_Bool`, C23's `<stdbool.h>`, and
`true` and `false` in `#if`.

Coverage:

  * The parser reads `bool` (or `_Bool`) as a type of its own, which no
    other type specifier can join.
  * Converting any scalar to `bool` gives 0 if the value compares
    equal to 0 and 1 otherwise (§6.3.1.2): integers of every width,
    pointers, floating values (with -0.0 equal to 0), constants, and
    static initializers. A pointer converts implicitly, as
    §6.5.16.1.1 allows only for `bool`.
  * `++` always stores 1 and `--` toggles (§6.5.2.4.2), and compound
    assignment converts its result.
  * A value already 0 or 1 (a comparison, `!`, a `bool`) converts
    without another comparison, and a conversion that does need one
    is `LDA; BEQ; LDA #1`.
  * c99_to_tac leaves no `bool` in the symbol table or the struct
    layouts for the back end.
  * `return_bool_in_carry` accepts `bool`, as the return type and as
    a carry struct's flag.
"""
from __future__ import annotations

import shutil
import unittest

import c99_ast
import tac_ast
from c99_to_tac import translate_program
from parser import ParserError, parse
from passes.identifier_resolution import resolve_program
from passes.type_checking import TypeCheckError, check_program
from preprocessor import preprocess
from sim.harness import build_sim
from tests.test_sim_chapters import _run_program as _run_tac
from tests.no_inlining import no_inlining


def _check(src: str):
    return check_program(resolve_program(parse(preprocess(src))))


def _codegen(src: str, optimize: bool = True) -> str:
    from compile import _run_stage
    return _run_stage("codegen", preprocess(src), optimize=optimize)


def _function_text(asm: str, name: str) -> str:
    return asm.split(f"\n{name}:")[1].split("RTS")[0]


class TestParse(unittest.TestCase):
    def test_bool_type(self) -> None:
        fn = parse("bool f(bool b) { return b; }").declaration[0] \
            .function_decl
        self.assertEqual(
            fn.data_type,
            c99_ast.FunType(params=[c99_ast.Bool()], ret=c99_ast.Bool()),
        )

    def test_rejects_other_specifiers(self) -> None:
        for src in (
            "unsigned bool b;", "bool int b;", "long bool b;",
            "bool bool b;", "_Bool int b;", "bool _Bool b;",
        ):
            with self.subTest(src=src):
                with self.assertRaises(ParserError):
                    parse(src)

    def test_alternative_spelling_is_the_same_type(self) -> None:
        prog = parse("_Bool a; bool b;")
        self.assertEqual(
            [d.var_decl.data_type for d in prog.declaration],
            [c99_ast.Bool(), c99_ast.Bool()],
        )


class TestTypeCheck(unittest.TestCase):
    def _static_value(self, src: str, name: str) -> int:
        _, symbols, _ = _check(src)
        return symbols[name].attrs.initial_value.value

    def test_static_initializers_convert(self) -> None:
        for init, expected in (
            ("0", 0), ("1", 1), ("256", 1), ("-1", 1), ("0x10000L", 1),
            ("0.5", 1), ("0.0", 0), ("-0.0", 0), ("0.25f", 1),
            ("&g", 1), ("(unsigned char)256", 0), ("2 > 1", 1),
            ("(bool)2 + 1", 1),
        ):
            with self.subTest(init=init):
                self.assertEqual(
                    self._static_value(f"int g; bool b = {init};", "b"),
                    expected,
                )

    def test_sizeof_and_promotion(self) -> None:
        _, symbols, _ = _check(
            "bool b; unsigned long n = sizeof(bool) + sizeof(b + b);"
        )
        # `b + b` promotes to int, 2 bytes.
        self.assertEqual(symbols["n"].attrs.initial_value.value, 3)

    def test_pointer_converts_implicitly_only_to_bool(self) -> None:
        _check("int g; bool f(int *p) { bool b = p; return p; }")
        with self.assertRaises(TypeCheckError):
            _check("int f(int *p) { unsigned char c = p; return c; }")

    def test_carry_return_accepts_bool(self) -> None:
        _check(
            "[[c6502::return_bool_in_carry]] bool f(int x)"
            " { return x; }"
            "struct r { unsigned char v; bool ok; };"
            "[[c6502::return_bool_in_carry]] struct r g(void)"
            " { return (struct r){ 1, 1 }; }"
        )


class TestErasure(unittest.TestCase):
    def test_no_bool_reaches_the_back_end(self) -> None:
        src = (
            "struct s { bool flag; bool *p; };"
            "bool g[3]; struct s gs;"
            "bool f(bool (*fp)(bool), bool b) { bool t = !b;"
            " return fp(t); }"
        )
        prog, symbols, types = _check(src)
        translate_program(prog, symbols, types)

        def has_bool(t) -> bool:
            return "Bool()" in repr(t)

        for name, sym in symbols.items():
            self.assertFalse(has_bool(sym.type), name)
        for tag, layout in types.items():
            for m in layout.members:
                self.assertFalse(has_bool(m.type), (tag, m.name))
        for d in prog.declaration:
            decl = getattr(d, "function_decl", None) or getattr(
                d, "var_decl", None,
            )
            if decl is not None:
                self.assertFalse(has_bool(decl.data_type), decl.name)


def _tac_instrs(src: str, fn: str) -> list:
    prog, symbols, types = _check(src)
    tac = translate_program(prog, symbols, types)
    for tl in tac.top_level:
        if isinstance(tl, tac_ast.Function) and tl.name == fn:
            return tl.instructions
    raise AssertionError(fn)


@no_inlining()
class TestLowering(unittest.TestCase):
    def test_zero_or_one_values_skip_the_comparison(self) -> None:
        for body in (
            "return a < b;", "return !a;", "return a && b;",
            "return c;", "return (a < b) & a;", "return a < b ? 1 : 0;",
            "return (int)c;",
        ):
            with self.subTest(body=body):
                instrs = _tac_instrs(
                    f"bool f(int a, int b, bool c) {{ {body} }}", "f",
                )
                self.assertFalse(any(
                    isinstance(i, tac_ast.Binary)
                    and isinstance(i.op, tac_ast.NotEqual)
                    for i in instrs
                ), instrs)

    def test_other_values_compare_with_zero(self) -> None:
        instrs = _tac_instrs("bool f(int a) { return a & 2; }", "f")
        self.assertTrue(any(
            isinstance(i, tac_ast.Binary)
            and isinstance(i.op, tac_ast.NotEqual)
            for i in instrs
        ))

    @unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
    def test_conversion_is_a_zero_test(self) -> None:
        # Static, so that they return in A: main keeps their results
        # as values (passes/carry_return_choice.py). `x` arrives in A
        # (and X), so f tests A with `ORA #$00`, and g stores the low
        # byte and ORs the high byte into it.
        asm = _codegen(
            "static bool f(unsigned char x) { return x; }"
            "static bool g(int x) { return x; }"
            "int main(void) { return f(3) + g(256); }"
        )
        self.assertEqual(_function_text(asm, "f").split(), [
            "SUBROUTINE",
            "ORA", "#$00",
            "BEQ", ".cmp_end@0",
            "LDA", "#$01",
            ".cmp_end@0:",
        ])
        self.assertEqual(_function_text(asm, "g").split(), [
            "SUBROUTINE",
            "STA", "__local_g__x_0",
            "TXA",
            "ORA", "__local_g__x_0",
            "BEQ", ".cmp_end@1",
            "LDA", "#$01",
            ".cmp_end@1:",
        ])


# (name, source, expected return of main)
_PROGRAMS: list[tuple[str, str, int]] = [
    ("int_conversion",
     "int main(void) { int x = 512; bool b = x; return b; }", 1),
    ("long_conversion",
     "int main(void) { long x = 0x10000L; bool b = x; long z = 0;"
     " bool c = z; return b * 2 + c; }", 2),
    ("char_conversion",
     "int main(void) { unsigned char x = 0; signed char y = -1;"
     " bool a = x, b = y; return a * 2 + b; }", 1),
    ("pointer_conversion",
     "int g; int main(void) { int *p = &g, *q = 0; bool a = p, b = q;"
     " return a * 2 + b; }", 2),
    ("floating_conversion",
     "double d = -0.0; double e = 1e-30; float f = 0.0f;"
     "int main(void) { bool a = d, b = e, c = f;"
     " return a * 4 + b * 2 + c; }", 2),
    ("explicit_cast",
     "int main(void) { int x = 2; return (bool)x + (bool)0 + (bool)3; }",
     2),
    ("parameter_and_return",
     "bool id(bool b) { return b; }"
     "int main(void) { return id(256) + id(0) * 2 + id(7) * 4; }", 5),
    ("increment_saturates",
     "int main(void) { bool b = 0; b++; ++b; int old = b++;"
     " return b * 2 + old; }", 3),
    ("decrement_toggles",
     "int main(void) { bool b = 1; int r = 0; b--; r += b; b--;"
     " r += b * 2; r += (--b) * 4; return r; }", 2),
    ("compound_assignment",
     "int main(void) { bool b = 0; b += 2; int r = b; b -= 1; r += b * 2;"
     " b |= 4; r += b * 4; b *= 0; r += b * 8; return r; }", 5),
    ("through_pointer_and_array",
     "bool flags[3];"
     "void set(bool *p, int v) { *p = v; }"
     "int main(void) { set(&flags[0], 10); set(&flags[1], 0);"
     " flags[2] = 300; flags[2]++;"
     " return flags[0] + flags[1] * 2 + flags[2] * 4; }", 5),
    ("struct_member",
     "struct s { int v; bool ok; };"
     "int main(void) { struct s x; x.ok = 0x100; x.v = 3;"
     " struct s *p = &x; p->ok += 1; return x.v * 10 + p->ok; }", 31),
    ("arithmetic_promotes",
     "int main(void) { bool a = 1, b = 1; return a + b + (a << 3); }", 10),
    ("condition",
     "int main(void) { bool b = 5; int r = 0; if (b) r = 1;"
     " while (b) { r += 2; b = 0; } return r; }", 3),
    ("stdbool",
     "#include <stdbool.h>\n"
     "bool odd(int x) { return x & 1; }"
     "int main(void) { bool t = true, f = false;"
     " return t + f * 2 + odd(3) * 4 + odd(4) * 8; }", 5),
    ("carry_return",
     "#include <stdbool.h>\n"
     "[[c6502::return_bool_in_carry]] bool nz(int x) { return x; }"
     "int main(void) { int n = 0; if (nz(512)) n++; if (nz(0)) n += 2;"
     " bool b = nz(-1); return n + b * 4; }", 5),
    ("static_initializers",
     "int g; bool a = 256, b = &g, c = 0.0, d = -0.0, e = 0.5;"
     "int main(void) { static bool s = 7;"
     " return a + b * 2 + c * 4 + d * 8 + e * 16 + s * 32; }", 51),
]


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestPrograms(unittest.TestCase):
    def test_tac_sim(self) -> None:
        for name, src, expected in _PROGRAMS:
            with self.subTest(case=name):
                value = _run_tac(preprocess(src)) & 0xFFFF
                if value & 0x8000:
                    value -= 0x10000
                self.assertEqual(value, expected)

    def test_asm_sim(self) -> None:
        for name, src, expected in _PROGRAMS:
            for optimize in (False, True):
                with self.subTest(case=name, optimize=optimize):
                    result = build_sim(src, optimize=optimize).run(
                        max_cycles=2_000_000,
                    )
                    self.assertFalse(result.timed_out)
                    self.assertEqual(result.return_int_signed(), expected)


if __name__ == "__main__":
    unittest.main()
