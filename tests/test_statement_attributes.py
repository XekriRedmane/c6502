"""c6502's statement attributes (C23 §6.8): `[[c6502::unroll]]` and
`[[c6502::keep]]` before a `for` loop, and
`[[c6502::save_hardware_stack]]` before an expression statement, a
`return`, or a declaration in a block. Covers where the grammar takes
attributes before a statement, the errors for each attribute where it
doesn't apply, and the error for the `#pragma c6502` lines they
replaced. tests/test_unroll.py and tests/test_recursion_saves.py test
what `unroll` and `save_hardware_stack` do."""

from __future__ import annotations

import logging
import tempfile
import unittest
from pathlib import Path

from lark import Lark, logger as lark_logger

import c99_ast
from compile import _run_stage
from parser import ParserError, parse
from preprocessor import PreprocessorError, preprocess


def _statements(src: str) -> list[c99_ast.Type_statement]:
    """The statements of the last function in `src`, in order."""
    fn = parse(src).declaration[-1].function_decl
    return [bi.statement for bi in fn.body.block_item
            if isinstance(bi, c99_ast.S)]


def _parse_error(src: str) -> str:
    """The message of the ParserError that parsing `src` raises."""
    try:
        parse(preprocess(src))
    except ParserError as e:
        return str(e)
    raise AssertionError(f"no ParserError for {src!r}")


class TestGrammar(unittest.TestCase):

    def test_no_new_conflicts(self) -> None:
        # Lark resolves a shift/reduce conflict as a shift and logs it
        # at DEBUG. The grammar has had these four: the dangling
        # `else`, and `void` before `)` in three states. Attributes
        # before a statement add none.
        messages: list[str] = []

        class Collect(logging.Handler):
            def emit(self, record):
                messages.append(record.getMessage())

        handler = Collect()
        level = lark_logger.level
        lark_logger.addHandler(handler)
        lark_logger.setLevel(logging.DEBUG)
        try:
            Lark.open(
                str(Path(__file__).resolve().parent.parent / "c99.lark"),
                parser="lalr",
                start=["start", "lex_only", "declarator", "type_name"],
            )
        finally:
            lark_logger.removeHandler(handler)
            lark_logger.setLevel(level)
        conflicts = sorted(
            m for m in messages if "Reduce/Reduce" in m
            or m.startswith("Shift/Reduce")
        )
        self.assertEqual(conflicts, sorted(
            ["Shift/Reduce conflict for terminal ELSE: (resolving as shift)"]
            + ["Shift/Reduce conflict for terminal RPAREN: "
               "(resolving as shift)"] * 3
        ))

    def test_declarations_in_a_block_keep_their_attributes(self) -> None:
        prog = parse(
            "typedef int T;\n"
            "int f(void) {\n"
            "    [[c6502::zeropage]] static unsigned char z;\n"
            "    [[maybe_unused]] T y = 1;\n"
            "    [[gnu::cold]] T * p = &y;\n"
            "    return z + *p;\n"
            "}\n"
        )
        items = prog.declaration[-1].function_decl.body.block_item
        z, y, p = (bi.declaration.var_decl for bi in items[:3])
        self.assertEqual(z.placement, c99_ast.ZeroPage(address=None))
        self.assertEqual(y.data_type, c99_ast.Int())
        self.assertEqual(p.data_type, c99_ast.Pointer(c99_ast.Int()))

    def test_attribute_declaration_is_not_a_statement(self) -> None:
        # `[[fallthrough]];` stays an attribute declaration, so it
        # leaves no null statement behind.
        stmts = _statements(
            "int f(int x) { switch (x) { case 1: x++; [[fallthrough]]; "
            "case 2: x++; } return x; }"
        )
        case1 = stmts[0].body.block.block_item
        self.assertEqual(len(case1), 2)

    def test_other_prefixes_are_ignored_on_statements(self) -> None:
        stmts = _statements("int f(int x) { [[gnu::hot]] x++; return x; }")
        self.assertIsInstance(stmts[0], c99_ast.Expression)

    def test_maybe_unused_on_a_label(self) -> None:
        stmts = _statements(
            "int f(int x) { [[maybe_unused]] L: x++; return x; }"
        )
        self.assertIsInstance(stmts[0], c99_ast.LabeledStmt)

    def test_after_a_label(self) -> None:
        stmts = _statements(
            "int f(int x) { L: [[c6502::keep]] for (;x;) x--; return x; }"
        )
        self.assertEqual(stmts[0].statement.loop_attribute, "keep")


class TestLoopAttributes(unittest.TestCase):

    def test_keep_sets_loop_attribute(self) -> None:
        stmts = _statements(
            "void f(void) { [[c6502::keep]] "
            "for (unsigned char y = 0xB4; --y;) { } }"
        )
        self.assertEqual(stmts[0].loop_attribute, "keep")

    def test_keep_keeps_a_delay_loop(self) -> None:
        for attribute, kept in (("", False), ("[[c6502::keep]] ", True)):
            with self.subTest(attribute=attribute):
                tac = _run_stage("tac", preprocess(
                    "void f(void) { " + attribute
                    + "for (unsigned char y = 0xB4; --y;) { } }"
                ), optimize=True)
                self.assertEqual("_keep_start" in tac, kept)
                self.assertEqual("Jump(" in tac, kept)

    def test_errors(self) -> None:
        for src, needle in (
            ("void f(void) { [[c6502::keep]] while (1) ; }",
             "`c6502::keep` goes before a `for` statement; saw it on a "
             "`while` statement"),
            ("void f(void) { [[c6502::unroll]] do ; while (0); }",
             "saw it on a `do` statement"),
            ("void f(void) { [[c6502::unroll]] { } }",
             "saw it on a block"),
            ("void f(void) { [[c6502::keep, c6502::unroll]] for (;;) ; }",
             "both given on one `for` statement"),
            ("void f(void) { [[c6502::keep]] [[c6502::keep]] for (;;) ; }",
             "`c6502::keep` is given twice"),
            ("void f(void) { [[c6502::unroll(4)]] for (;;) ; }",
             "doesn't take an argument"),
            ("void f(void) { [[c6502::keep]]; }",
             "it doesn't apply to an attribute declaration"),
            ("void f(void) { [[c6502::keep]] L: for (;;) ; }",
             "Put it after the label"),
            ("void f(int x) { switch (x) { [[c6502::unroll]] case 1: "
             "for (;;) ; } }",
             "Put it after the label"),
            ("[[c6502::unroll]] void f(void);",
             "it doesn't apply to function declaration 'f'"),
            ("[[c6502::keep]] void f(void) { }",
             "it doesn't apply to function definition 'f'"),
            ("[[c6502::keep]] unsigned char n;",
             "it doesn't apply to object declaration `n`"),
            ("void f(void) { [[c6502::keep]] unsigned char n; }",
             "it doesn't apply to object declaration `n`"),
            ("void f(unsigned char n [[c6502::unroll]]);",
             "`c6502::unroll` goes before a `for` statement"),
            ("void f(void) { [[c6502::zp_abi]] for (;;) ; }",
             "`c6502::zp_abi` doesn't apply to a statement"),
            ("void f(void) { [[deprecated]] for (;;) ; }",
             "the standard attribute `deprecated` can't apply to a "
             "statement"),
            ("void f(void) { [[unroll]] for (;;) ; }",
             "needs c6502's prefix"),
        ):
            with self.subTest(src):
                self.assertIn(needle, _parse_error(src))


class TestSaveHardwareStackPlacement(unittest.TestCase):
    """What tests/test_recursion_saves.py doesn't cover: declarations
    outside a block and in other places a declaration's attributes go."""

    def test_errors(self) -> None:
        for src, needle in (
            ("int g(int);\n[[c6502::save_hardware_stack]] int r = g(1);",
             "saw it on a declaration at file scope"),
            ("[[c6502::save_hardware_stack]] int g(int);",
             "applies to the calls in a declaration's initializers, and "
             "`g` declares a function"),
            ("[[c6502::save_hardware_stack]] int g(void) { return 0; }",
             "`c6502::save_hardware_stack` goes before an expression "
             "statement, a `return` statement, or a declaration inside "
             "a function; it doesn't apply to function definition 'g'"),
            ("struct s { [[c6502::save_hardware_stack]] int m; };",
             "it doesn't apply to"),
            ("int f(int x) { [[c6502::save_hardware_stack]] "
             "do f(x); while (0); return 0; }",
             "saw it on a `do` statement"),
        ):
            with self.subTest(src):
                self.assertIn(needle, _parse_error(src))

    def test_every_declarator_of_a_declaration(self) -> None:
        prog = parse(
            "int g(int);\nint f(int x) {\n"
            "    [[c6502::save_hardware_stack]] int a = g(1), b = g(2);\n"
            "    return a + b;\n}\n"
        )
        items = prog.declaration[-1].function_decl.body.block_item
        for bi in items[:2]:
            self.assertTrue(bi.declaration.var_decl.init.hw_stack_save)


class TestOldPragmas(unittest.TestCase):

    def test_each_names_its_replacement(self) -> None:
        for pragma, needle in (
            ("#pragma c6502 loop unroll(enable)",
             "write the statement attribute `[[c6502::unroll]]`"),
            ("#pragma c6502 loop unroll(disable)",
             "it never did anything, so delete the line"),
            ("#pragma c6502 loop keep",
             "write the statement attribute `[[c6502::keep]]`"),
            ("#pragma c6502 call save(hardware_stack)",
             "write the statement attribute "
             "`[[c6502::save_hardware_stack]]`"),
            ("#  pragma   c6502   loop  unroll ( enable )",
             "`[[c6502::unroll]]`"),
        ):
            with self.subTest(pragma):
                with self.assertRaises(PreprocessorError) as cm:
                    preprocess("int x;\n" + pragma + "\nint y;\n")
                message = str(cm.exception)
                self.assertIn("line 2: c6502 no longer takes", message)
                self.assertIn(needle, message)

    def test_line_is_the_source_line(self) -> None:
        # pcpp's output has the header's lines before the pragma, so
        # the line counted in the output would be far past 3.
        with self.assertRaises(PreprocessorError) as cm:
            preprocess(
                "#include <stdint.h>\n\n#pragma c6502 loop keep\nint x;\n",
            )
        self.assertTrue(
            str(cm.exception).startswith("line 3: "), str(cm.exception),
        )

    def test_line_in_a_header_names_the_header(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            header = Path(tmp) / "loops.h"
            header.write_text("int y;\n#pragma c6502 loop keep\n")
            main = Path(tmp) / "main.c"
            main.write_text(
                "#include <stdint.h>\n#include \"loops.h\"\nint x;\n",
            )
            with self.assertRaises(PreprocessorError) as cm:
                preprocess(main.read_text(), filename=str(main))
        message = str(cm.exception)
        self.assertTrue(message.startswith("line 2 of "), message)
        self.assertIn("loops.h: c6502 no longer takes", message)

    def test_pragma_in_a_skipped_group(self) -> None:
        out = preprocess(
            "#if 0\n#pragma c6502 loop keep\n#endif\nint x;\n",
        )
        self.assertNotIn("pragma", out)

    def test_unknown_c6502_pragma(self) -> None:
        with self.assertRaises(PreprocessorError) as cm:
            preprocess("#pragma c6502 wat\nint x;\n")
        message = str(cm.exception)
        self.assertIn("line 1", message)
        self.assertIn("`#pragma c6502 wat` is an error", message)
        self.assertIn("[[c6502::save_hardware_stack]]", message)

    def test_other_pragmas_are_dropped(self) -> None:
        # C99 §6.10.6 lets an implementation ignore a pragma it doesn't
        # recognize. The line stays, blank, so later line numbers hold.
        for src in ("#pragma GCC diagnostic ignored \"-Wfoo\"\nint x;\n",
                    "#pragma c6502x\nint x;\n",
                    "#pragma once\nint x;\n"):
            with self.subTest(src):
                out = preprocess(src)
                self.assertNotIn("pragma", out)
                self.assertEqual(out.count("\n"), src.count("\n"))


if __name__ == "__main__":
    unittest.main()
