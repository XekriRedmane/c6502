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
        # absolute addressing against that mangled name. The zero
        # init lays down as `DS.B 1` (zero-run) rather than
        # `DC.B $00`.
        rc, out, _ = self._run(
            ["compile.py", "-", "--codegen"],
            stdin="int main(void) { static int x; return x; }",
        )
        self.assertEqual(rc, 0)
        self.assertIn("DS.B  1", out)
        # The mangled label appears (the exact name is brittle to
        # the unique-counter so we just check for the prefix).
        self.assertIn("@0.x:", out)
        self.assertIn("LDA   @0.x", out)

    def test_codegen_file_scope_tentative_definition(self):
        # `int x;` at file scope is a tentative definition; type-
        # checking resolves it to a zeroed StaticVariable, and
        # c99_to_tac emits the zero as a 1-byte `ZeroInit`.
        rc, out, _ = self._run(
            ["compile.py", "-", "--codegen"],
            stdin="int x; int main(void) { return x; }",
        )
        self.assertEqual(rc, 0)
        self.assertIn("x:", out)
        self.assertIn("DS.B  1", out)
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

    def test_codegen_static_multi_dim_array_init(self):
        # `static int nested[3][2] = {{1,2},{3,4},{5,6}};` lays
        # the bytes down in source order: 1, 2, 3, 4, 5, 6 — six
        # consecutive DC.B directives under the variable's label.
        rc, out, _ = self._run(
            ["compile.py", "-", "--codegen"],
            stdin=(
                "int main(void) { "
                "static int nested[3][2] = {{1,2},{3,4},{5,6}}; "
                "return nested[1][1]; }"
            ),
        )
        self.assertEqual(rc, 0)
        # Find the variable's section by the mangled label.
        idx = out.index("@0.nested:")
        section = out[idx:].splitlines()
        # The first 7 lines are the label and 6 DC.Bs (other code
        # may follow after).
        self.assertEqual(section[0], "@0.nested:")
        self.assertEqual(section[1], "   DC.B  $01")
        self.assertEqual(section[2], "   DC.B  $02")
        self.assertEqual(section[3], "   DC.B  $03")
        self.assertEqual(section[4], "   DC.B  $04")
        self.assertEqual(section[5], "   DC.B  $05")
        self.assertEqual(section[6], "   DC.B  $06")

    def test_codegen_static_array_partial_init_zero_pads(self):
        # `static int a[5] = {1, 2};` zero-pads the trailing slots.
        # The 3 trailing IntInit(0)s coalesce into one `DS.B 3`.
        rc, out, _ = self._run(
            ["compile.py", "-", "--codegen"],
            stdin=(
                "int main(void) { "
                "static int a[5] = {1, 2}; return a[2]; }"
            ),
        )
        self.assertEqual(rc, 0)
        idx = out.index("@0.a:")
        section = out[idx:].splitlines()
        self.assertEqual(section[1:4], [
            "   DC.B  $01",
            "   DC.B  $02",
            "   DS.B  3",
        ])

    def test_codegen_static_multi_dim_array_zero_holes(self):
        # `static long a[3][2] = {{100}, {200, 300}};` — a[0][1]
        # is missing (one Long zero = 2 bytes), and the entire a[2]
        # row is missing (two Longs = 4 bytes). The two zero gaps
        # are independent (they bracket the {200,300} row), so they
        # emit as two separate `DS.B`s.
        rc, out, _ = self._run(
            ["compile.py", "-", "--codegen"],
            stdin=(
                "int main(void) { "
                "static long a[3][2] = {{100}, {200, 300}}; "
                "return 0; }"
            ),
        )
        self.assertEqual(rc, 0)
        idx = out.index("@0.a:")
        section = out[idx:].splitlines()
        self.assertEqual(section[1:6], [
            "   DC.W  $0064",   # a[0][0] = 100
            "   DS.B  2",       # a[0][1] = 0  (zero pad)
            "   DC.W  $00C8",   # a[1][0] = 200
            "   DC.W  $012C",   # a[1][1] = 300
            "   DS.B  4",       # a[2][0] / a[2][1] = 0,0  (zero pad)
        ])

    def test_codegen_file_scope_long_array_init(self):
        # File-scope `long a[3] = {1, 2, 3};` — each element is a
        # 2-byte LongInit, so we get three DC.W directives.
        rc, out, _ = self._run(
            ["compile.py", "-", "--codegen"],
            stdin=(
                "long a[3] = {1, 2, 3}; "
                "int main(void) { return (int)a[0]; }"
            ),
        )
        self.assertEqual(rc, 0)
        idx = out.index("a:")
        section = out[idx:].splitlines()
        self.assertEqual(section[1:4], [
            "   DC.W  $0001",
            "   DC.W  $0002",
            "   DC.W  $0003",
        ])

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


class TestVoid(unittest.TestCase):
    """Void return type, void expressions, void *. Covers the four
    contexts where a void expression is permitted (expression
    statement, both branches of `?:`, the operand of `(void)`, and
    `for`-init / `for`-continuation), plus the implicit conversions
    between `void *` and any other pointer type."""

    def _run(self, argv: list[str], stdin: str = "") -> tuple[int, str, str]:
        with patch("sys.stdin", io.StringIO(stdin)), \
             patch("sys.stdout", new_callable=io.StringIO) as out, \
             patch("sys.stderr", new_callable=io.StringIO) as err:
            rc = main(argv)
        return rc, out.getvalue(), err.getvalue()

    def _codegen(self, src: str) -> str:
        rc, out, err = self._run(["compile.py", "-", "--codegen"], stdin=src)
        self.assertEqual(rc, 0, msg=err)
        return out

    def _expect_failure(self, src: str) -> str:
        # `main()` lets pipeline exceptions (ParserError,
        # TypeCheckError, ...) propagate; the chapter harnesses use
        # the same idiom. Catch broadly so the test can assert on the
        # message text regardless of which pass raised.
        try:
            self._run(["compile.py", "-", "--codegen"], stdin=src)
        except Exception as e:  # noqa: BLE001
            return str(e)
        self.fail(f"expected compilation to fail for: {src!r}")

    def test_codegen_void_function_empty_body(self):
        # `void f(void) { }` — falling off the end is legal per C99
        # §6.9.1.12. The TAC translator appends a Ret(val=None); the
        # asm epilogue collapses to a bare RTS (zero arg/local bytes,
        # save_a=False).
        out = self._codegen("void f(void) { }")
        self.assertIn("f:", out)
        self.assertIn("SUBROUTINE", out)
        self.assertIn("RTS", out)
        # No PHA/PLA — save_a=False on void Ret, and no value is
        # being staged into A.
        self.assertNotIn("PHA", out)
        self.assertNotIn("PLA", out)

    def test_codegen_void_function_explicit_return(self):
        # `return;` lowers identically to falling off the end.
        out = self._codegen("void f(void) { return; }")
        self.assertIn("f:", out)
        self.assertIn("RTS", out)

    def test_codegen_call_void_function_discards_result(self):
        # The caller emits `JSR f` with no return-value capture
        # afterwards (no LDA from A or HARGS into a frame slot).
        out = self._codegen(
            "void f(void) { } "
            "int main(void) { f(); return 0; }",
        )
        self.assertIn("JSR   f", out)
        # Right after the JSR f, the next non-blank line should be
        # the `LDA #$00` that stages the return-0 — i.e. NOT a
        # capture sequence.
        lines = [
            ln.strip() for ln in out.splitlines()
            if ln.strip() and not ln.strip().startswith(";")
        ]
        idx = lines.index("JSR   f")
        self.assertEqual(lines[idx + 1], "LDA   #$00")

    def test_codegen_cast_to_void_evaluates_for_side_effects(self):
        # `(void)(1+2)` — the binary still evaluates (LDA #$01; CLC;
        # ADC #$02), but its result is dropped.
        out = self._codegen("int main(void) { (void)(1+2); return 0; }")
        self.assertIn("LDA   #$01", out)
        self.assertIn("ADC   #$02", out)

    def test_codegen_void_conditional_no_branch_copies(self):
        # `flag ? f() : g()` where both branches are void: each
        # branch is just a JSR; no Copy-to-dst sequence.
        out = self._codegen(
            "void f(void); void g(void); "
            "int main(int flag) { flag ? f() : g(); return 0; }",
        )
        self.assertIn("JSR   f", out)
        self.assertIn("JSR   g", out)
        self.assertIn(".cond_else@", out)
        self.assertIn(".cond_end@", out)

    def test_codegen_void_pointer_implicit_conversion(self):
        # `void *p = ip;` and `ip = p;` are no-op casts at the byte
        # level (both 2 bytes). The generated asm is just a 2-byte
        # Copy in each direction.
        out = self._codegen(
            "int main(void) { "
            "void *p; int *ip; ip = (int*)0; p = ip; ip = p; "
            "return 0; }",
        )
        # No JSR to any helper — purely Copy / SignExtend at the
        # frame.
        self.assertNotIn("JSR   mul", out)

    def test_codegen_null_pointer_constant_assignable_to_void_pointer(self):
        # `void *p = 0;` — the integer 0 is a null pointer constant
        # (C99 §6.3.2.3.3); it should sign-extend to a 2-byte zero
        # address.
        out = self._codegen(
            "int main(void) { void *p = 0; return p == 0; }",
        )
        # Equality compare against the 2-byte zero — the LDA #$00 is
        # the rhs's zero byte and CMP / EOR forms drive the equality.
        self.assertIn("LDA   #$00", out)

    def test_codegen_void_in_for_init_and_post(self):
        # The for-init and for-continuation slots are expression-or-
        # empty; void calls work in both.
        out = self._codegen(
            "void f(void); void g(void); "
            "int main(void) { for (f(); 0; g()) {} return 0; }",
        )
        self.assertIn("JSR   f", out)
        self.assertIn("JSR   g", out)

    def test_reject_return_value_from_void_function(self):
        err = self._expect_failure("void f(void) { return 1; }")
        self.assertIn("void", err)

    def test_reject_bare_return_from_non_void_function(self):
        err = self._expect_failure("int f(void) { return; }")
        self.assertIn("return", err)

    def test_reject_void_variable(self):
        err = self._expect_failure(
            "int main(void) { void x; return 0; }",
        )
        self.assertIn("Void", err)

    def test_reject_void_parameter(self):
        # `int f(void x)` — `void` as a NAMED parameter (vs. the
        # `int f(void)` empty-params form, which is fine).
        err = self._expect_failure("int f(void x) { return 0; }")
        self.assertIn("void", err.lower())

    def test_reject_void_arithmetic(self):
        err = self._expect_failure(
            "void f(void); int main(void) { f() + 1; return 0; }",
        )
        self.assertIn("Void", err)

    def test_reject_void_as_function_argument(self):
        err = self._expect_failure(
            "void f(void); void g(int); "
            "int main(void) { g(f()); return 0; }",
        )
        self.assertIn("void", err.lower())

    def test_reject_void_pointer_arithmetic(self):
        # `void *p; p + 1;` — sizeof(void) is undefined.
        err = self._expect_failure(
            "int main(void) { void *p; p + 1; return 0; }",
        )
        self.assertIn("void", err.lower())

    def test_reject_mismatched_pointer_assignment_still_fails(self):
        # Adding void* shouldn't loosen the matching-pointee rule
        # for non-void pointer assignments.
        err = self._expect_failure(
            "int main(void) { int *ip; long *lp; ip = lp; return 0; }",
        )
        self.assertIn("pointer", err.lower())


class TestSizeof(unittest.TestCase):
    """sizeof operator. c6502 uses a 1-byte-int / 2-byte-long /
    4-byte-long-long storage model, so the byte counts here differ
    from the LP64 sizes the standard's tests assume. Tests check
    that sizeof produces the right c6502-specific value, that it's
    a compile-time constant of type unsigned long (ConstLong in
    TAC), that the operand is NOT evaluated, and that arrays don't
    decay as the operand of sizeof."""

    def _run(self, argv: list[str], stdin: str = "") -> tuple[int, str, str]:
        with patch("sys.stdin", io.StringIO(stdin)), \
             patch("sys.stdout", new_callable=io.StringIO) as out, \
             patch("sys.stderr", new_callable=io.StringIO) as err:
            rc = main(argv)
        return rc, out.getvalue(), err.getvalue()

    def _tac(self, src: str) -> str:
        rc, out, err = self._run(["compile.py", "-", "--tac"], stdin=src)
        self.assertEqual(rc, 0, msg=err)
        return out

    def _codegen(self, src: str) -> str:
        rc, out, err = self._run(["compile.py", "-", "--codegen"], stdin=src)
        self.assertEqual(rc, 0, msg=err)
        return out

    def _expect_failure(self, src: str) -> str:
        try:
            self._run(["compile.py", "-", "--codegen"], stdin=src)
        except Exception as e:  # noqa: BLE001
            return str(e)
        self.fail(f"expected compilation to fail for: {src!r}")

    def test_sizeof_scalar_types(self):
        # c6502's storage model: int 1, long 2, long long 4, char 1,
        # float 4, double 8, pointer 2.
        out = self._tac(
            "int main(void) { "
            "long sizes = sizeof(int) + sizeof(long) + sizeof(long long) "
            "+ sizeof(char) + sizeof(float) + sizeof(double) "
            "+ sizeof(int *); "
            "return 0; }",
        )
        # Each sizeof folds to a ConstLong literal (sizeof's result
        # is unsigned long → TAC ConstLong). Ensure all the expected
        # constants show up — the runtime additions chain through.
        for size in (1, 2, 4, 1, 4, 8, 2):
            self.assertIn(f"value={size}", out)

    def test_sizeof_array_does_not_decay(self):
        # `sizeof a` for `int a[10]` is 10 (10 elements × 1B/int),
        # NOT sizeof(int *) = 2. The decay-to-pointer rule is
        # explicitly suppressed here per C99 §6.3.2.1.3.
        out = self._tac(
            "int main(void) { int a[10]; return (int)sizeof a; }",
        )
        # Not the pointer width 2 — the array width 10.
        self.assertIn("value=10", out)

    def test_sizeof_string_literal_does_not_decay(self):
        # `sizeof "abc"` is 4 (3 chars + null), not 2 (sizeof char *).
        out = self._tac(
            'int main(void) { return (int)sizeof "abc"; }',
        )
        self.assertIn("value=4", out)

    def test_sizeof_multi_dim_array(self):
        # `sizeof a` for `long a[3][5]` is 3 * 5 * 2 = 30.
        out = self._tac(
            "int main(void) { long a[3][5]; return (int)sizeof a; }",
        )
        self.assertIn("value=30", out)

    def test_sizeof_does_not_evaluate_operand(self):
        # `sizeof (i++)` must NOT emit any inc instructions for `i`.
        # The TAC for the function body should contain the sizeof
        # constant but no Binary(Add) and no Copy back into i.
        out = self._tac(
            "int main(void) { int i = 0; long s = sizeof (i++); return i; }",
        )
        # The sizeof value (1) shows up as a ConstLong.
        self.assertIn("value=1", out)
        # The operand `i++` would lower to a Binary(Add) into a
        # temp + Copy back to i — neither should appear in the TAC.
        self.assertNotIn("Add(", out)

    def test_sizeof_does_not_evaluate_function_call(self):
        # `sizeof foo()` must NOT emit a JSR to foo. Only the result
        # type matters.
        out = self._codegen(
            "int foo(void); "
            "int main(void) { long s = sizeof foo(); return 0; }",
        )
        self.assertNotIn("JSR   foo", out)

    def test_sizeof_returns_unsigned_long(self):
        # Result type of sizeof is ULong. Assigning to a Long
        # variable is a same-width conversion (no Truncate or
        # SignExtend). Assigning to an Int truncates.
        out = self._tac(
            "int main(void) { "
            "long l = sizeof(int); int i = sizeof(int); "
            "return 0; }",
        )
        # Long ← ULong is a same-width pointer-style copy: just a
        # Copy. Int ← ULong narrows: a Truncate.
        self.assertIn("Truncate(", out)

    def test_sizeof_of_sizeof_is_two(self):
        # sizeof's result has type unsigned long (size 2 in c6502).
        out = self._tac(
            "int main(void) { return (int)sizeof sizeof(int); }",
        )
        self.assertIn("value=2", out)

    def test_sizeof_pointer_type_form(self):
        out = self._tac(
            "int main(void) { return (int)sizeof(int (*)[100]); }",
        )
        # Pointer is 2 bytes regardless of its pointee.
        self.assertIn("value=2", out)

    def test_sizeof_nested_array_type(self):
        # sizeof(char[3][6][17][9]) = 3 * 6 * 17 * 9 * 1 = 2754 bytes.
        out = self._tac(
            "int main(void) { return (int)sizeof(char[3][6][17][9]); }",
        )
        self.assertIn("value=2754", out)

    def test_reject_sizeof_void_type(self):
        err = self._expect_failure(
            "int main(void) { return sizeof(void); }",
        )
        self.assertIn("void", err.lower())

    def test_reject_sizeof_void_expression(self):
        err = self._expect_failure(
            "void f(void); int main(void) { return sizeof f(); }",
        )
        self.assertIn("void", err.lower())

    def test_reject_sizeof_function_type(self):
        # `(int(int))` parses as a type-name `int(int)` which is a
        # function type — sizeof(function-type) is illegal.
        err = self._expect_failure(
            "int main(void) { return sizeof(int(int)); }",
        )
        self.assertIn("function", err.lower())

    def test_sizeof_in_case_label(self):
        # sizeof folds at compile time so it's a valid §6.6.6 integer
        # constant expression — usable as a case label.
        out = self._codegen(
            "int main(int x) { "
            "switch (x) { "
            "case sizeof(char): return 1; "
            "case sizeof(long): return 2; "
            "case sizeof(int[5]): return 5; "
            "default: return 0; "
            "} }",
        )
        # Three case-dispatch comparisons: against 1, 2, 5
        # respectively. The constants get coerced to the switch's
        # promoted control type (Int — `x` is int) modulo width.
        self.assertIn("CMP   #$01", out)
        self.assertIn("CMP   #$02", out)
        self.assertIn("CMP   #$05", out)

    def test_sizeof_exp_in_case_label(self):
        # sizeof e form also works — the type checker populates the
        # inner expression's data_type so the const evaluator can
        # fold it.
        out = self._codegen(
            "int main(int x) { "
            "long y; long long z; "
            "switch (x) { "
            "case sizeof y: return 2; "
            "case sizeof z: return 4; "
            "default: return 0; "
            "} }",
        )
        # sizeof(long) = 2, sizeof(long long) = 4.
        self.assertIn("CMP   #$02", out)
        self.assertIn("CMP   #$04", out)

    def test_sizeof_case_label_duplicate_detection(self):
        # `sizeof y` and `sizeof(y+y)` both have type Long → both
        # equal 2 → duplicate case value, rejected.
        err = self._expect_failure(
            "int main(int x) { "
            "long y; "
            "switch (x) { "
            "case sizeof y: return 1; "
            "case sizeof(y+y): return 2; "
            "default: return 0; "
            "} }",
        )
        self.assertIn("duplicate", err.lower())


if __name__ == "__main__":
    unittest.main()
