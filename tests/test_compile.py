import io
import tempfile
import unittest
from pathlib import Path
from unittest.mock import patch

from compile import main


class TestCompileDriver(unittest.TestCase):
    SOURCE = "int main(void) { return 42; }"

    def _run(self, argv: list[str], stdin: str = "") -> tuple[int, str, str]:
        with patch("sys.stdin", io.StringIO(stdin)), \
             patch("sys.stdout", new_callable=io.StringIO) as out, \
             patch("sys.stderr", new_callable=io.StringIO) as err:
            rc = main(argv)
        return rc, out.getvalue(), err.getvalue()

    def test_lex_stdin_prints_tokens(self):
        rc, out, _ = self._run(["compile.py", "-", "--lex"], stdin=self.SOURCE)
        self.assertEqual(rc, 0)
        lines = out.splitlines()
        self.assertEqual(lines[0].split("\t"), ["1:1", "keyword", "int"])
        # Last token is `}`.
        self.assertEqual(lines[-1].split("\t")[1:], ["symbol", "}"])

    def test_parse_stdin_prints_ast(self):
        rc, out, _ = self._run(["compile.py", "-", "--parse"], stdin=self.SOURCE)
        self.assertEqual(rc, 0)
        self.assertIn("Program(", out)
        # Top level is `declaration=[FunctionDecl(function_decl=...)]`
        # since the grammar collapsed function definitions into the
        # generic declaration shape.
        self.assertIn("FunctionDecl(", out)
        # Constants are wrapped as `Constant(const=ConstInt(int=N))`
        # / `ConstLong(int=N)` per the new c99 AST.
        self.assertIn("Constant(", out)
        self.assertIn("ConstInt(", out)
        self.assertIn("int=42", out)

    def test_tac_stdin_prints_tac_ast(self):
        rc, out, _ = self._run(
            ["compile.py", "-", "--tac"],
            stdin="int main(void) { return -42; }",
        )
        self.assertEqual(rc, 0)
        self.assertIn("Unary(", out)
        self.assertIn("Negate(", out)
        self.assertIn("'%0'", out)
        self.assertIn("Ret(", out)

    def test_codegen_stdin_prints_asm(self):
        rc, out, _ = self._run(["compile.py", "-", "--codegen"], stdin=self.SOURCE)
        self.assertEqual(rc, 0)
        self.assertIn("main:", out)
        self.assertIn("SUBROUTINE", out)
        self.assertIn("LDA   #$2A", out)
        self.assertIn("RTS", out)

    def test_codegen_if_else(self):
        # `if (a) return 1; else return 2;` should produce a BEQ to
        # an `.if_else@*` local label and a JMP to an `.if_end@*`
        # local label (leading dot is dasm's local-label marker; `@`
        # marks it as translator-minted, never collidable with a
        # user-written name).
        rc, out, _ = self._run(
            ["compile.py", "-", "--codegen"],
            stdin="int main(void) { int a = 0; "
                  "if (a) return 1; else return 2; }",
        )
        self.assertEqual(rc, 0)
        self.assertIn("BEQ   .if_else@", out)
        self.assertIn("JMP   .if_end@", out)
        self.assertIn(".if_else@", out)
        self.assertIn(".if_end@", out)

    def test_codegen_block_shadowing(self):
        # `int a = 1; { int a = 2; } return a;` — the inner block
        # shadows the outer `a`. Variable resolution gives them
        # distinct unique names; codegen lays them out in distinct
        # frame slots, so both immediate writes (LDA #$01 and
        # LDA #$02) appear in the asm.
        rc, out, _ = self._run(
            ["compile.py", "-", "--codegen"],
            stdin="int main(void) { int a = 1; { int a = 2; } return a; }",
        )
        self.assertEqual(rc, 0)
        self.assertIn("LDA   #$01", out)
        self.assertIn("LDA   #$02", out)

    def test_codegen_goto_and_label(self):
        # `goto foo; foo: return 0;` should emit a JMP to the
        # function-prefixed label and the matching label definition.
        rc, out, _ = self._run(
            ["compile.py", "-", "--codegen"],
            stdin="int main(void) { goto foo; foo: return 0; }",
        )
        self.assertEqual(rc, 0)
        self.assertIn("JMP   .main@foo", out)
        self.assertIn(".main@foo:", out)

    def test_codegen_postfix_increment(self):
        # `a++` generates: load a into A, store into the saved-old
        # frame slot, then ADC #$01 against a, store back. We just
        # check both the ADC #$01 (the +=1 step) and that the asm
        # compiled cleanly.
        rc, out, _ = self._run(
            ["compile.py", "-", "--codegen"],
            stdin="int main(void) { int a = 0; a++; return a; }",
        )
        self.assertEqual(rc, 0)
        self.assertIn("ADC   #$01", out)

    def test_codegen_prefix_decrement(self):
        # Prefix `--a` desugars to `a = a - 1`, which lowers to SBC
        # #$01 against a's frame slot.
        rc, out, _ = self._run(
            ["compile.py", "-", "--codegen"],
            stdin="int main(void) { int a = 5; --a; return a; }",
        )
        self.assertEqual(rc, 0)
        self.assertIn("SBC   #$01", out)

    def test_codegen_compound_assignment(self):
        # `a += 3` desugars at parse time to `a = a + 3`, which lowers
        # to Binary(Add) + Copy in TAC and then to load-A from the
        # frame slot, ADC the constant, store back. We don't run the
        # result here (no runtime header yet); we just check the
        # immediate ADC and the initializer LDA both made it through.
        rc, out, _ = self._run(
            ["compile.py", "-", "--codegen"],
            stdin="int main(void) { int a = 5; a += 3; return a; }",
        )
        self.assertEqual(rc, 0)
        self.assertIn("LDA   #$05", out)
        self.assertIn("ADC   #$03", out)

    def test_codegen_file_scope_static_variable(self):
        # `static int g = 7;` at file scope → INTERNAL linkage,
        # Initial(7). The asm emits the variable as a labeled DC.B,
        # and references inside main use absolute addressing (LDA g).
        rc, out, _ = self._run(
            ["compile.py", "-", "--codegen"],
            stdin="static int g = 7; int main(void) { return g; }",
        )
        self.assertEqual(rc, 0)
        self.assertIn("g:", out)
        self.assertIn("DC.B  $07", out)
        self.assertIn("LDA   g", out)

    def test_codegen_block_scope_static_zero_initialized(self):
        # `static int x;` at block scope: NONE linkage but static
        # storage duration. Renamed by identifier_resolution to a
        # unique `@N.x`, default-zero-initialized, lowered to a
        # StaticVariable at top level. References inside main use
        # absolute addressing against that mangled name.
        rc, out, _ = self._run(
            ["compile.py", "-", "--codegen"],
            stdin="int main(void) { static int x; return x; }",
        )
        self.assertEqual(rc, 0)
        self.assertIn("DC.B  $00", out)
        # The mangled label appears (the exact name is brittle to
        # the unique-counter so we just check for the prefix).
        self.assertIn("@0.x:", out)
        self.assertIn("LDA   @0.x", out)

    def test_codegen_file_scope_tentative_definition(self):
        # `int x;` at file scope is a tentative definition; type-
        # checking resolves it to Initial(0) at end-of-TU and
        # c99_to_tac emits a StaticVariable with init=0.
        rc, out, _ = self._run(
            ["compile.py", "-", "--codegen"],
            stdin="int x; int main(void) { return x; }",
        )
        self.assertEqual(rc, 0)
        self.assertIn("x:", out)
        self.assertIn("DC.B  $00", out)
        self.assertIn("LDA   x", out)

    def test_codegen_extern_no_initializer_emits_nothing(self):
        # `extern int x;` with no initializer → NoInitializer; the
        # symbol is referenced (LDA x) but no DC.B is emitted —
        # the definition lives in another TU.
        rc, out, _ = self._run(
            ["compile.py", "-", "--codegen"],
            stdin="extern int x; int main(void) { return x; }",
        )
        self.assertEqual(rc, 0)
        self.assertIn("LDA   x", out)
        self.assertNotIn("x:\n   DC.B", out)

    def test_pcpp_strips_comments(self):
        rc, out, _ = self._run(
            ["compile.py", "-", "--codegen"],
            stdin="/* hello */ int main(void) { return 1; } // bye",
        )
        self.assertEqual(rc, 0)
        self.assertIn("LDA   #$01", out)

    def test_output_file_for_codegen(self):
        with tempfile.TemporaryDirectory() as tmp:
            out_path = Path(tmp) / "hello.asm"
            rc, _, _ = self._run(
                ["compile.py", "-", "--codegen", "-o", str(out_path)],
                stdin="int main(void) { return 7; }",
            )
            self.assertEqual(rc, 0)
            self.assertIn("LDA   #$07", out_path.read_text())

    def test_codegen_output_must_end_in_asm(self):
        rc, _, err = self._run(
            ["compile.py", "-", "--codegen", "-o", "out.txt"],
            stdin=self.SOURCE,
        )
        self.assertEqual(rc, 2)
        self.assertIn(".asm suffix", err)

    def test_non_codegen_outputs_can_have_any_suffix(self):
        with tempfile.TemporaryDirectory() as tmp:
            out_path = Path(tmp) / "tokens.txt"
            rc, _, _ = self._run(
                ["compile.py", "-", "--lex", "-o", str(out_path)],
                stdin=self.SOURCE,
            )
            self.assertEqual(rc, 0)
            self.assertIn("keyword\tint", out_path.read_text())

    def test_stages_are_mutually_exclusive(self):
        with self.assertRaises(SystemExit):
            self._run(["compile.py", "-", "--lex", "--parse"], stdin=self.SOURCE)

    def test_stage_is_required(self):
        with self.assertRaises(SystemExit):
            self._run(["compile.py", "-"], stdin=self.SOURCE)

    def test_input_from_file(self):
        with tempfile.TemporaryDirectory() as tmp:
            in_path = Path(tmp) / "hello.c"
            in_path.write_text(self.SOURCE)
            rc, out, _ = self._run(
                ["compile.py", str(in_path), "--codegen"],
            )
            self.assertEqual(rc, 0)
            self.assertIn("LDA   #$2A", out)

    def test_dash_d_macro_is_forwarded_to_preprocessor(self):
        rc, out, _ = self._run(
            ["compile.py", "-", "--codegen", "-D", "MAX=42"],
            stdin="int main(void) { return MAX; }",
        )
        self.assertEqual(rc, 0)
        self.assertIn("LDA   #$2A", out)

    def test_dash_d_without_value_defaults_to_one(self):
        rc, out, _ = self._run(
            ["compile.py", "-", "--codegen", "-D", "FOO"],
            stdin="int main(void) { return FOO; }",
        )
        self.assertEqual(rc, 0)
        self.assertIn("LDA   #$01", out)

    def test_pcpp_version_macro_is_predefined(self):
        # __PCPP_VERSION__ holds pcpp's version string ("1.30"), which
        # the parser would reject as a numeric expression. So we only
        # check that the macro is defined, via #ifdef.
        rc, out, _ = self._run(
            ["compile.py", "-", "--codegen"],
            stdin=("#ifdef __PCPP_VERSION__\n"
                   "int main(void) { return 99; }\n"
                   "#else\n"
                   "int main(void) { return 0; }\n"
                   "#endif\n"),
        )
        self.assertEqual(rc, 0)
        self.assertIn("LDA   #$63", out)

    def test_unknown_pcpp_flag_is_ignored(self):
        rc, _, err = self._run(
            ["compile.py", "-", "--codegen", "--no-such-pcpp-flag"],
            stdin=self.SOURCE,
        )
        self.assertEqual(rc, 0)
        self.assertIn("--no-such-pcpp-flag", err)
        self.assertIn("not known", err)


if __name__ == "__main__":
    unittest.main()
