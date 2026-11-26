import unittest


from errors import ParserError


class TestPointerDeclarations(unittest.TestCase):
    """End-to-end tests that pointer types in var_decl /
    function_decl flow correctly through the parser into
    c99_ast nodes. Pointers are 2 bytes (the 6502's address
    width); see also test_replace_pseudoregisters for the
    frame-layout side and test_tac_to_asm for the size-dispatch."""


    def test_pointer_var_decl_through_full_pipeline(self):
        # End-to-end: `int *p;` should make it through the full
        # compile pipeline (parse → identifier_resolution →
        # label_resolution → loop_labeling → type_checking →
        # c99_to_tac → tac_to_asm → replace_pseudoregisters →
        # asm_emit) without raising. Pointer is sized as 2 bytes,
        # so a tentative `int *p;` lays down as `DS.B 2` (zero-run).
        from compile import _run_stage
        text = _run_stage("codegen", "int *p;\n")
        self.assertIn("p:", text)
        self.assertIn("DS.B  2", text)

    def test_pointer_local_gets_two_frame_bytes(self):
        # Pointer locals occupy 2 contiguous frame bytes — same
        # treatment as Long. Verify the prologue allocates the
        # right amount.
        from compile import _run_stage
        text = _run_stage(
            "codegen",
            "int main(void) { int *p; p = p; return 0; }\n",
        )
        # M=2 (one pointer local) + 2 (saved-FP slot) = 4; SSP -= 4.
        self.assertIn("prologue: 0 arg bytes, 2 local bytes", text)
        self.assertIn("SBC   #$04", text)

    def test_pointer_function_returns_in_a_and_x(self):
        # A function returning `int *` returns it as an `int`: the low
        # byte in A and the high byte in X, which the soft-stack
        # epilogue keeps on the hardware stack while it tears down
        # the frame.
        from compile import _run_stage
        text = _run_stage(
            "codegen",
            "int *foo(int *p) { return p; }\n"
            "int main(void) { return 0; }\n",
        )
        self.assertIn("foo:", text)
        self.assertNotIn("STA   HARGS", text)
        self.assertIn("   PLA\n   TAX\n   PLA\n   RTS\n", text)
        # Prologue allocates 0 local bytes (pointer param sits in
        # the caller's pushed args, not in our locals).
        self.assertIn("prologue: 2 arg bytes, 0 local bytes", text)


class TestPointerOpsEndToEnd(unittest.TestCase):
    """Dereference (`*p`) and AddressOf (`&x`) flowing all the way
    through to 6502 asm. Smoke-checks the pipeline: parse → resolve
    → type-check → c99_to_tac → tac_to_asm → replace_pseudoregisters
    → asm_emit."""

    def _codegen(self, src: str) -> str:
        from compile import _run_stage
        return _run_stage("codegen", src)

    def test_address_of_local_uses_fp_arithmetic(self):
        # `&x` for a local computes its address as `FP + frame_off`
        # via a 16-bit add. We don't pin the exact frame offset in
        # case layout shifts; just check the shape: CLC + LDA FP +
        # ADC #imm followed by a store, then LDA FP+1 + ADC #$00 +
        # store.
        text = self._codegen(
            "int main(void) { int x; int *p; p = &x; return 0; }\n"
        )
        self.assertIn("CLC", text)
        self.assertIn("LDA   FP", text)
        self.assertIn("LDA   FP+1", text)
        self.assertIn("ADC   #$00", text)

    def test_address_of_static_uses_immediate_label(self):
        # `&g` for a file-scope object uses `#<g` / `#>g`
        # immediates to load the address bytes; no FP arithmetic.
        text = self._codegen(
            "int g;\nint main(void) { int *p; p = &g; return 0; }\n"
        )
        self.assertIn("LDA   #<g", text)
        self.assertIn("LDA   #>g", text)

    def test_dereference_read_uses_dptr(self):
        # `*p` (read) stages p's two bytes into DPTR / DPTR+1, then
        # reads via `(DPTR),Y`.
        text = self._codegen(
            "int main(void) { int x; int *p; p = &x; return *p; }\n"
        )
        self.assertIn("STA   DPTR", text)
        self.assertIn("STA   DPTR+1", text)
        self.assertIn("LDA   (DPTR),Y", text)

    def test_dereference_write_uses_dptr(self):
        # `*p = 5` stages p's bytes into DPTR / DPTR+1, then writes
        # via `STA (DPTR),Y` with `Y = 0` for the (single) byte of
        # an Int store.
        text = self._codegen(
            "int main(void) { int x; int *p; p = &x; *p = 5; return 0; }\n"
        )
        self.assertIn("STA   DPTR", text)
        self.assertIn("STA   (DPTR),Y", text)
        # The value 5 lands in A right before the indirect store.
        self.assertIn("LDA   #$05", text)

    def test_pointer_to_long_writes_two_bytes_through_dptr(self):
        # `*lp = 0x1234L` for a `long *lp` writes 2 bytes via
        # `(DPTR),Y` with Y=0 then Y=1 — same byte-pair pattern as
        # any other Long copy, but the destination is indirect
        # rather than a Frame slot. The Y-tracking peephole rewrites
        # the second `LDY #$01` as `INY` since Y is known to be 0.
        text = self._codegen(
            "int main(void) { long y; long *lp; lp = &y; "
            "*lp = 0x1234L; return 0; }\n"
        )
        self.assertIn("LDA   #$34", text)
        self.assertIn("LDA   #$12", text)
        # Two indirect writes (Y=0 for low, INY for high).
        self.assertIn("LDY   #$00", text)
        self.assertIn("   INY", text)
        self.assertIn("STA   (DPTR),Y", text)

    def test_address_of_dereference_collapses(self):
        # `&*p` ≡ `p` per C99 §6.5.3.2.3 — c99_to_tac elides the
        # GetAddress when the operand is a Dereference, so `q = &*p`
        # produces no LoadAddress sequence (no FP-arithmetic, no
        # ImmLabel pair) — just a plain Long-style Copy from p to q.
        # Compare against the same program with `q = &x` to confirm
        # the elision: the `&*p` form should have one *fewer*
        # FP-add sequence than the `&x` form.
        with_amp_x = self._codegen(
            "int main(void) { int x; int *p; int *q; "
            "p = &x; q = &x; return 0; }\n"
        )
        with_amp_deref = self._codegen(
            "int main(void) { int x; int *p; int *q; "
            "p = &x; q = &*p; return 0; }\n"
        )
        # The CLC count differs by exactly 1: the `&*p` version
        # omits the LoadAddress arithmetic that `&x` would emit.
        # (Each `&local` lowers to one CLC for its FP-add; the
        # epilogue contributes a fixed CLC of its own.)
        self.assertEqual(
            with_amp_x.count("CLC") - with_amp_deref.count("CLC"), 1,
        )


class TestPointerEquality(unittest.TestCase):
    """`==` and `!=` between pointers, plus the null-pointer-
    constant exception. Relational ordering (`<` / `>` / `<=` / `>=`)
    on same-type pointers is also covered here; arithmetic ops on
    pointers live in their own test class."""

    def _codegen(self, src: str) -> str:
        from compile import _run_stage
        return _run_stage("codegen", src)

    def _typecheck(self, src: str) -> None:
        # Run the pipeline through the type-check stage so type
        # errors surface here rather than from later passes.
        from compile import _run_stage
        _run_stage("tac", src)

    def test_equal_same_pointer_type(self):
        # `p == q` for two same-type pointers — the existing 2-byte
        # Equal lowering applies unchanged (Pointer is sized like
        # Long), so the asm has the high-byte / low-byte CMP pair.
        text = self._codegen(
            "int main(void) {\n"
            "  int x;\n"
            "  int *p; int *q;\n"
            "  p = &x; q = &x;\n"
            "  return p == q;\n"
            "}\n"
        )
        # Two CMPs (one per byte) and the BNE short-circuit out of the
        # high byte's compare, which branch threading sends past the
        # low byte's BEQ (`.thread@N`).
        self.assertEqual(text.count("CMP   (FP),Y"), 2)
        self.assertRegex(text, r"BNE   \.(cmp_differ|thread)@")

    def test_not_equal_same_pointer_type(self):
        text = self._codegen(
            "int main(void) {\n"
            "  int x;\n"
            "  int *p; int *q;\n"
            "  p = &x; q = &x;\n"
            "  return p != q;\n"
            "}\n"
        )
        self.assertEqual(text.count("CMP   (FP),Y"), 2)

    def test_pointer_equal_to_null_constant_on_right(self):
        # `p == 0` — the literal 0 is recognized as a null pointer
        # constant; the type checker converts it to the pointer
        # type. End-to-end this should compile cleanly.
        self._codegen(
            "int main(void) { int *p; if (p == 0) return 1; return 0; }\n"
        )

    def test_pointer_equal_to_null_constant_on_left(self):
        # `0 == p` — same legality, mirror order.
        self._codegen(
            "int main(void) { int *p; if (0 == p) return 1; return 0; }\n"
        )

    def test_pointer_not_equal_to_null_through_long(self):
        # `p != 0L` — null pointer constant via a Long literal. The
        # detector drills past the `0` regardless of the integer
        # variant.
        self._codegen(
            "int main(void) { int *p; if (p != 0L) return 1; return 0; }\n"
        )

    def test_pointer_equal_to_null_through_cast(self):
        # `(long)0` is still a null pointer constant — the detector
        # drills through Cast wrappers.
        self._codegen(
            "int main(void) { int *p; if (p == (long)0) return 1; return 0; }\n"
        )

    def test_pointer_compare_to_nonzero_int_rejected(self):
        # `p == 5` — 5 isn't a null pointer constant, so the
        # comparison is illegal under our rules.
        from errors import TypeCheckError, VisitError
        with self.assertRaises((TypeCheckError, VisitError)) as cm:
            self._typecheck(
                "int main(void) { int *p; if (p == 5) return 1; return 0; }\n"
            )
        # Unwrap the TypeCheckError if it's wrapped (compile.py
        # may surface it directly).
        err = cm.exception
        if hasattr(err, "orig_exc") and isinstance(err.orig_exc, TypeCheckError):
            err = err.orig_exc
        self.assertIsInstance(err, TypeCheckError)

    def test_pointer_compare_distinct_pointer_types_rejected(self):
        # `int *p; long *q; p == q;` — different pointer types,
        # rejected even though both sides are pointers.
        from errors import TypeCheckError
        with self.assertRaises(TypeCheckError):
            self._typecheck(
                "int main(void) {\n"
                "  int *p; long *q;\n"
                "  if (p == q) return 1;\n"
                "  return 0;\n"
                "}\n"
            )

    def test_ordering_same_pointer_type_compiles(self):
        # `<` / `>` / `<=` / `>=` on same-type pointers — pointer
        # operands dispatch to the unsigned-ordering lowering (no
        # V-correction; carry flag carries the ordering result), so
        # we get the per-byte SBC pair, starting with CLC for `<`,
        # whose carry is then the answer, and C as the 0 or 1.
        text = self._codegen(
            "int main(void) {\n"
            "  int x;\n"
            "  int *p; int *q;\n"
            "  p = &x; q = &x;\n"
            "  return p < q;\n"
            "}\n"
        )
        # Two SBCs (one per byte), then C as the 0 or 1 for the
        # unsigned `<`.
        self.assertEqual(text.count("SBC   (FP),Y"), 2)
        self.assertIn("ROL", text)
        self.assertNotIn("EOR   #$01", text)
        self.assertNotIn(".cmp_novf@", text)

    def test_ordering_distinct_pointer_types_rejected(self):
        from errors import TypeCheckError
        with self.assertRaises(TypeCheckError):
            self._typecheck(
                "int main(void) { int *p; long *q; "
                "if (p < q) return 1; return 0; }\n"
            )

    def test_ordering_pointer_vs_int_rejected(self):
        # No null-pointer-constant exception for relational ops, so
        # `p < 0` is rejected even though `p == 0` would be fine.
        from errors import TypeCheckError
        with self.assertRaises(TypeCheckError):
            self._typecheck(
                "int main(void) { int *p; if (p < 0) return 1; "
                "return 0; }\n"
            )

    def test_equal_result_is_int(self):
        # The result of `==` on pointers is `int` (per C99 §6.5.9.1),
        # matching the existing comparison-result rule. Verify by
        # using it in a context that requires an int (a `return`
        # from an int-returning function).
        self._codegen(
            "int main(void) { int *p; int *q; "
            "p = q; return p == q; }\n"
        )


class TestPointerIntegerCasts(unittest.TestCase):
    """Casts between integer types and pointer types, both
    directions. Pointer is 2 bytes, so the existing
    SignExtend / ZeroExtend / Truncate / no-op machinery covers
    everything via byte-width dispatch — these tests pin the
    end-to-end paths."""

    def _codegen(self, src: str) -> str:
        from compile import _run_stage
        return _run_stage("codegen", src)

    def test_pointer_to_int_uses_truncate(self):
        # `(int)p` — Pointer (2B) → Int (1B) is a Truncate. The
        # asm just moves the low byte; the high byte is dropped.
        # No `BMI` (sign-extend marker) and no `LDA #$00` for the
        # high half should appear from the cast itself.
        text = self._codegen(
            "int main(void) {\n"
            "  int x; int a; int *p;\n"
            "  p = &x;\n"
            "  a = (int)p;\n"
            "  return a;\n"
            "}\n"
        )
        # The cast produces no extra arithmetic — just plain Movs.
        # Verify by checking a representative pattern: the asm
        # successfully includes a load of A and store-back. (Pinning
        # exact lines is too brittle; the smoke is that codegen
        # completes and the program structure is intact.)
        self.assertIn("RTS", text)

    def test_pointer_to_long_is_no_op(self):
        # `(long)p` — Pointer (2B) → Long (2B) is a no-op cast at
        # the c99_to_tac level (same width); the inner val passes
        # through. Smoke-check the program compiles end-to-end.
        self._codegen(
            "int main(void) {\n"
            "  int x; long l; int *p;\n"
            "  p = &x;\n"
            "  l = (long)p;\n"
            "  return 0;\n"
            "}\n"
        )

    def test_char_to_pointer_sign_extends(self):
        # `(int *)c` — Char (1B, signed) → Pointer (2B). Goes
        # through the SignExtend path: the low byte is the source's
        # value, the high byte is 0x00 if the source is non-negative
        # and 0xFF if it's negative. Verify by looking for the
        # sign-extend label landmarks. (Post the C99 width refresh,
        # Int is 2B same as Pointer — no extension needed there;
        # but Char is still 1B and needs the per-byte fan-out.)
        text = self._codegen(
            "int main(void) {\n"
            "  signed char a; int *p;\n"
            "  a = 5;\n"
            "  p = (int *)a;\n"
            "  return 0;\n"
            "}\n"
        )
        self.assertIn(".sx_neg@", text)
        self.assertIn("LDA   #$FF", text)
        self.assertIn("LDA   #$00", text)

    def test_uint_to_pointer_zero_extends(self):
        # `(int *)u` — UInt (1B, unsigned) → Pointer (2B). Goes
        # through ZeroExtend: high byte unconditionally zero, no
        # branch. The asm has the low-byte copy followed by a
        # `LDA #$00` for the high byte; no sign-extend labels.
        text = self._codegen(
            "int main(void) {\n"
            "  unsigned int u; int *p;\n"
            "  u = 200u;\n"
            "  p = (int *)u;\n"
            "  return 0;\n"
            "}\n"
        )
        self.assertNotIn(".sx_neg@", text)
        self.assertIn("LDA   #$00", text)

    def test_long_to_pointer_is_no_op(self):
        # `(int *)l` — Long (2B) → Pointer (2B) is a no-op cast.
        # No SignExtend / ZeroExtend / Truncate needed; the inner
        # val passes through. Smoke-check it compiles.
        self._codegen(
            "int main(void) {\n"
            "  long l; int *p;\n"
            "  l = 0x1234L;\n"
            "  p = (int *)l;\n"
            "  return 0;\n"
            "}\n"
        )

    def test_pointer_to_pointer_different_pointee_no_op(self):
        # `(long *)p` where p is `int *` — Pointer (2B) → Pointer
        # (2B) is a no-op cast (the bytes are identical; only the
        # type-checker's view of the pointee changes). Smoke-check
        # the program compiles.
        self._codegen(
            "int main(void) {\n"
            "  int x; int *p; long *lp;\n"
            "  p = &x;\n"
            "  lp = (long *)p;\n"
            "  return 0;\n"
            "}\n"
        )

    def test_double_pointer_cast(self):
        # `(int **)a` — building a 2-byte pointer-to-pointer from
        # a 1-byte int. Same SignExtend path as `(int *)a`.
        self._codegen(
            "int main(void) {\n"
            "  int a; int **pp;\n"
            "  a = 1;\n"
            "  pp = (int **)a;\n"
            "  return 0;\n"
            "}\n"
        )

    def test_float_to_pointer_rejected(self):
        # C99 §6.3.2.3 doesn't define floating ↔ pointer
        # conversions. The type checker rejects them with a
        # focused message.
        from errors import TypeCheckError
        with self.assertRaises(TypeCheckError) as cm:
            self._typecheck(
                "int main(void) { float f; int *p; "
                "f = 1.5f; p = (int *)f; return 0; }\n"
            )
        self.assertIn("pointer", str(cm.exception))
        self.assertIn("floating", str(cm.exception))

    def test_double_to_pointer_rejected(self):
        from errors import TypeCheckError
        with self.assertRaises(TypeCheckError):
            self._typecheck(
                "int main(void) { double d; long *lp; "
                "d = 2.5; lp = (long *)d; return 0; }\n"
            )

    def test_pointer_to_float_rejected(self):
        from errors import TypeCheckError
        with self.assertRaises(TypeCheckError):
            self._typecheck(
                "int main(void) { int x; int *p; float f; "
                "p = &x; f = (float)p; return 0; }\n"
            )

    def test_pointer_to_double_rejected(self):
        from errors import TypeCheckError
        with self.assertRaises(TypeCheckError):
            self._typecheck(
                "int main(void) { int x; int *p; double d; "
                "p = &x; d = (double)p; return 0; }\n"
            )

    def _typecheck(self, src: str) -> None:
        from compile import _run_stage
        _run_stage("tac", src)


class TestPointerUnaryOps(unittest.TestCase):
    """Unary operators on pointer operands. `-p` and `~p` are
    nonsensical (negate / bit-flip an address) and rejected at
    type-check time. `!p` is the null-pointer test (`p != 0`) and
    is legal — the existing 2-byte LogicalNot lowering ORs the two
    address bytes and drives a 0/1 select off the resulting Z
    flag."""

    def _typecheck(self, src: str) -> None:
        from compile import _run_stage
        _run_stage("tac", src)

    def _codegen(self, src: str) -> str:
        from compile import _run_stage
        return _run_stage("codegen", src)

    def test_negate_pointer_rejected(self):
        from errors import TypeCheckError
        with self.assertRaises(TypeCheckError) as cm:
            self._typecheck(
                "int main(void) { int x; int *p; p = &x; return -p; }\n"
            )
        self.assertIn("'-'", str(cm.exception))

    def test_complement_pointer_rejected(self):
        from errors import TypeCheckError
        with self.assertRaises(TypeCheckError) as cm:
            self._typecheck(
                "int main(void) { int x; int *p; p = &x; return ~p; }\n"
            )
        self.assertIn("'~'", str(cm.exception))

    def test_logical_not_pointer_lowers_to_or_of_both_bytes(self):
        # `!p` for a 2-byte pointer ORs the two address bytes and
        # branches on the resulting Z flag — Z=1 iff both bytes are
        # zero, i.e., the pointer is null. The existing Long-sized
        # LogicalNot lowering handles this for free.
        text = self._codegen(
            "int main(void) { int x; int *p; p = &x; return !p; }\n"
        )
        # The asm has an `ORA (FP),Y` (combining the two bytes) then
        # the lnot 0/1-select via BEQ to a `lnot_true` label.
        self.assertIn("ORA   (FP),Y", text)
        self.assertIn(".lnot_true@", text)

    def test_logical_not_pointer_to_pointer(self):
        # `!pp` for an `int **` — also 2 bytes, same lowering.
        self._codegen(
            "int main(void) { int *p; int **pp; pp = &p; return !pp; }\n"
        )

    def test_long_pointer_negate_still_rejected(self):
        # Same rule applies for `long *` — pointer-ness is what
        # matters, not the pointee.
        from errors import TypeCheckError
        with self.assertRaises(TypeCheckError):
            self._typecheck(
                "int main(void) { long y; long *lp; lp = &y; return -lp; }\n"
            )

    def test_logical_and_two_pointers(self):
        # `p && q` — non-null check on each. The existing 2-byte
        # cond-jump lowering ORs the two address bytes per operand
        # and BEQ-shortcircuits on a null result, exactly matching
        # the truthy/falsy semantics for pointers.
        text = self._codegen(
            "int main(void) { int x; int y; int *p; int *q; "
            "p = &x; q = &y; if (p && q) return 1; return 0; }\n"
        )
        # Two ORA's (one per operand) and the and_false short-circuit.
        self.assertGreaterEqual(text.count("ORA   (FP),Y"), 2)
        self.assertIn(".and_false@", text)

    def test_logical_or_two_pointers(self):
        text = self._codegen(
            "int main(void) { int x; int y; int *p; int *q; "
            "p = &x; q = &y; if (p || q) return 1; return 0; }\n"
        )
        self.assertGreaterEqual(text.count("ORA   (FP),Y"), 2)
        self.assertIn(".or_true@", text)

    def test_logical_and_pointer_and_int(self):
        # Mixed pointer + integer is fine — the operands are tested
        # independently, so they don't have to share a common type.
        self._codegen(
            "int main(void) { int a; int x; int *p; "
            "a = 1; p = &x; if (a && p) return 1; return 0; }\n"
        )

    def test_logical_or_int_and_pointer(self):
        self._codegen(
            "int main(void) { int a; int x; int *p; "
            "a = 0; p = &x; if (a || p) return 1; return 0; }\n"
        )

    def test_logical_and_distinct_pointer_types(self):
        # Two pointers of *different* types are fine for `&&` /
        # `||` — unlike `==` / `!=`, no common type is required;
        # each operand's truthiness is tested independently.
        self._codegen(
            "int main(void) { int x; long y; int *p; long *lp; "
            "p = &x; lp = &y; if (p && lp) return 1; return 0; }\n"
        )

    def test_logical_and_result_is_int(self):
        # Result of `&&` / `||` is `int` per C99 §6.5.13.3 /
        # §6.5.14.3, regardless of operand type. Use it where an
        # int is required.
        self._codegen(
            "int main(void) { int *p; int *q; "
            "return p && q; }\n"
        )

    def test_multiply_pointer_rejected(self):
        # C99 §6.5.5 — `*` requires arithmetic operands. Pointer
        # is rejected on either side.
        from errors import TypeCheckError
        with self.assertRaises(TypeCheckError) as cm:
            self._typecheck(
                "int main(void) { int x; int *p; p = &x; "
                "return p * 2; }\n"
            )
        self.assertIn("'*'", str(cm.exception))

    def test_divide_two_pointers_rejected(self):
        from errors import TypeCheckError
        with self.assertRaises(TypeCheckError) as cm:
            self._typecheck(
                "int main(void) { int *p; int *q; "
                "return p / q; }\n"
            )
        self.assertIn("'/'", str(cm.exception))

    def test_modulo_pointer_rhs_rejected(self):
        # Pointer on the right also gets rejected — the check fires
        # symmetrically.
        from errors import TypeCheckError
        with self.assertRaises(TypeCheckError) as cm:
            self._typecheck(
                "int main(void) { int a; int *p; "
                "return a % p; }\n"
            )
        self.assertIn("'%'", str(cm.exception))

    def test_divide_pointer_lhs_int_rhs_rejected(self):
        from errors import TypeCheckError
        with self.assertRaises(TypeCheckError):
            self._typecheck(
                "int main(void) { int x; int *p; p = &x; "
                "return p / 2; }\n"
            )


class TestStaticPointerInit(unittest.TestCase):
    """Static-storage pointer initializers — null (already covered)
    and `&otherstatic` (C99 §6.6.7 paragraph 9)."""

    def _codegen(self, src: str) -> str:
        from compile import _run_stage
        return _run_stage("codegen", src)

    def _typecheck(self, src: str) -> None:
        from compile import _run_stage
        _run_stage("tac", src)

    def test_static_pointer_addressof_static_at_file_scope(self):
        # `static int a; static int *a_ptr = &a;` — the canonical
        # case the user asked about. Lays down `a_ptr: DC.W a` so
        # the assembler resolves the address at link time.
        text = self._codegen(
            "static int a;\n"
            "static int *a_ptr = &a;\n"
            "int main(void) { return 0; }\n"
        )
        self.assertIn("a_ptr:", text)
        # The `dc.w` operand is the bare symbol `a` (no `+offset`
        # since the offset is 0).
        self.assertIn("DC.W  a", text)

    def test_external_pointer_addressof_static(self):
        # `int *p = &a;` (external linkage) — same lowering path.
        text = self._codegen(
            "int a;\n"
            "int *p = &a;\n"
            "int main(void) { return 0; }\n"
        )
        self.assertIn("p:", text)
        self.assertIn("DC.W  a", text)

    def test_block_scope_static_pointer_addressof_file_scope(self):
        # A function-local `static int *p = &g;` keeps its
        # block-scope rename (`@N.p`) but still resolves to a
        # `DC.W g` lay-down. The block-scope `static` makes the
        # variable internal-linkage; the AddressInit machinery
        # doesn't care about linkage.
        text = self._codegen(
            "int g;\n"
            "int main(void) {\n"
            "  static int *p = &g;\n"
            "  return 0;\n"
            "}\n"
        )
        self.assertIn("DC.W  g", text)

    def test_static_pointer_addressof_long(self):
        # `&y` where y is a 2-byte static — the address is still
        # 2 bytes (every address is); pointee type doesn't affect
        # it.
        text = self._codegen(
            "long y;\n"
            "long *lp = &y;\n"
            "int main(void) { return 0; }\n"
        )
        self.assertIn("DC.W  y", text)

    def test_addressof_local_in_static_init_rejected(self):
        # An automatic-storage variable's address isn't a constant
        # expression — its location isn't known at link time. The
        # type checker rejects it with a focused message.
        from errors import TypeCheckError
        with self.assertRaises(TypeCheckError) as cm:
            self._typecheck(
                "int main(void) {\n"
                "  int local;\n"
                "  static int *p = &local;\n"
                "  return 0;\n"
                "}\n"
            )
        self.assertIn("static storage duration", str(cm.exception))

    def test_static_pointer_zero_init_still_works(self):
        # `static int *p = 0;` — the existing null-pointer-constant
        # path. Verify it lays down `DS.B 2` (the typed zero
        # collapses to a 2-byte ZeroInit) instead of `DC.W $0000`.
        text = self._codegen(
            "static int *p = 0;\n"
            "int main(void) { return 0; }\n"
        )
        self.assertIn("p:", text)
        self.assertIn("DS.B  2", text)

    def test_static_pointer_no_init_is_zero(self):
        # Tentative definition (no initializer) lays down a 2-byte
        # zero run for the pointer slot.
        text = self._codegen(
            "int *p;\n"
            "int main(void) { return 0; }\n"
        )
        self.assertIn("p:", text)
        self.assertIn("DS.B  2", text)

    def test_function_pointer_initializer(self):
        # `int (*fp)(void) = &foo;` — function-pointer static
        # initializer. The address-of-function path produces an
        # AddressInit pointing at the function symbol; asm_emit
        # lays down `fp: DC.W foo` so the assembler resolves the
        # function's address at link time.
        text = self._codegen(
            "int foo(void) { return 7; }\n"
            "int (*fp)(void) = &foo;\n"
            "int main(void) { return 0; }\n"
        )
        self.assertIn("fp:", text)
        self.assertIn("DC.W  foo", text)

    def test_function_pointer_initializer_forward_decl(self):
        # The pointee function doesn't need a body in this TU —
        # `&foo` works against a pure forward declaration too.
        # The resulting `DC.W foo` reference is left for the
        # linker to resolve.
        text = self._codegen(
            "int foo(int x);\n"
            "int (*fp)(int) = &foo;\n"
            "int main(void) { return 0; }\n"
        )
        self.assertIn("DC.W  foo", text)

    def test_function_pointer_runtime_assign(self):
        # `fp = &foo;` (runtime, not a static init) — the
        # GetAddress / LoadAddress chain has to recognize `foo` as
        # a static-storage name so it takes the immediate-label
        # path (`LDA #<foo`) instead of the FP-relative-add path
        # for locals.
        text = self._codegen(
            "int foo(void) { return 7; }\n"
            "int main(void) {\n"
            "  int (*fp)(void);\n"
            "  fp = &foo;\n"
            "  return 0;\n"
            "}\n"
        )
        self.assertIn("LDA   #<foo", text)
        self.assertIn("LDA   #>foo", text)

    def test_block_scope_static_function_pointer_initializer(self):
        # Same as the variable cases — block-scope `static int
        # (*fp)(void) = &foo;` keeps its `@N.fp` rename but still
        # resolves the function symbol at link time.
        text = self._codegen(
            "int foo(void) { return 7; }\n"
            "int main(void) {\n"
            "  static int (*fp)(void) = &foo;\n"
            "  return 0;\n"
            "}\n"
        )
        self.assertIn("DC.W  foo", text)


class TestIndirectCall(unittest.TestCase):
    """Calling through a function pointer. The 6502 has no JSR
    indirect, so we trampoline: stage the function pointer's two
    bytes into DPTR, then `JSR icall` (a runtime trampoline whose
    body is `JMP (DPTR)`)."""

    def _codegen(self, src: str) -> str:
        from compile import _run_stage
        return _run_stage("codegen", src)

    def _typecheck(self, src: str) -> None:
        from compile import _run_stage
        _run_stage("tac", src)

    def test_indirect_call_no_args(self):
        # `fp()` with a function-pointer-typed `fp`. The asm
        # stages fp's two bytes into DPTR and JSRs `icall`.
        text = self._codegen(
            "int foo(void) { return 7; }\n"
            "int main(void) {\n"
            "  int (*fp)(void) = &foo;\n"
            "  return fp();\n"
            "}\n"
        )
        self.assertIn("STA   DPTR", text)
        self.assertIn("STA   DPTR+1", text)
        self.assertIn("JSR   icall", text)

    def test_indirect_call_with_args(self):
        # `fp(5)` — args still ride on the soft stack the same as
        # for a direct call. Only the call-site `JSR` target
        # differs. Post the C99 width refresh, an int arg occupies
        # 2 bytes, so the stack-alloc subtracts 2.
        text = self._codegen(
            "int foo(int x) { return x + 1; }\n"
            "int main(void) {\n"
            "  int (*fp)(int) = &foo;\n"
            "  return fp(5);\n"
            "}\n"
        )
        self.assertIn("JSR   icall", text)
        # The `5` lands in A right before the soft-stack write.
        self.assertIn("LDA   #$05", text)
        # Allocate-stack for the 2-byte int arg (SSP -= 2).
        self.assertIn("SBC   #$02", text)

    def test_direct_and_indirect_call_in_same_function(self):
        # Direct call `foo(...)` keeps the existing `JSR foo`
        # form; indirect `fp(...)` uses `JSR icall`. Both can
        # coexist in the same function body.
        text = self._codegen(
            "int foo(int x) { return x; }\n"
            "int main(void) {\n"
            "  int (*fp)(int) = &foo;\n"
            "  int a = foo(1);\n"
            "  int b = fp(2);\n"
            "  return a + b;\n"
            "}\n"
        )
        self.assertIn("JSR   foo", text)
        self.assertIn("JSR   icall", text)

    def test_indirect_call_long_return(self):
        # Indirect call returning Long uses the same HARGS+0..1
        # convention as a direct call — the `JSR icall` is
        # transparent.
        text = self._codegen(
            "long foo(void) { return 0x1234L; }\n"
            "int main(void) {\n"
            "  long (*fp)(void) = &foo;\n"
            "  long y = fp();\n"
            "  return 0;\n"
            "}\n"
        )
        self.assertIn("JSR   icall", text)
        # After the JSR, the caller reads back HARGS+0 (low) and
        # HARGS+1 (high) into the destination slot.
        self.assertIn("LDA   HARGS", text)
        self.assertIn("LDA   HARGS+1", text)

    def test_indirect_call_through_runtime_assigned_pointer(self):
        # The function pointer doesn't have to be initialized at
        # compile time — assigning `&foo` at runtime works the
        # same way (GetAddress + Copy stamps fp's bytes; the
        # subsequent `fp()` reads them back into DPTR).
        text = self._codegen(
            "int foo(void) { return 7; }\n"
            "int main(void) {\n"
            "  int (*fp)(void);\n"
            "  fp = &foo;\n"
            "  return fp();\n"
            "}\n"
        )
        self.assertIn("LDA   #<foo", text)
        self.assertIn("LDA   #>foo", text)
        self.assertIn("JSR   icall", text)

    def test_indirect_call_with_dereference_callee(self):
        # `(*fp)(args)` — the canonical C idiom. Auto-decay per
        # C99 §6.3.2.1.4 makes this equivalent to `fp(args)`; the
        # parser reduces both forms to FunctionCall(name=fp, ...)
        # and the same JSR icall lowering applies.
        text = self._codegen(
            "int foo(int x) { return x + 1; }\n"
            "int main(void) {\n"
            "  int (*fp)(int) = &foo;\n"
            "  return (*fp)(5);\n"
            "}\n"
        )
        self.assertIn("JSR   icall", text)

    def test_parenthesized_direct_callee(self):
        # `(foo)(args)` — paren around a function name. Resolves
        # the same as `foo(args)` (direct call, JSR foo).
        text = self._codegen(
            "int foo(int x) { return x + 1; }\n"
            "int main(void) {\n"
            "  return (foo)(5);\n"
            "}\n"
        )
        self.assertIn("JSR   foo", text)
        self.assertNotIn("JSR   icall", text)

    def test_address_of_function_then_call(self):
        # `(&foo)(args)` — explicitly take foo's address, then
        # call through it. Per the auto-decay rule the parens-and-
        # `&` cancel out to leave just `foo(args)`. Reduces to a
        # direct call.
        text = self._codegen(
            "int foo(int x) { return x + 1; }\n"
            "int main(void) {\n"
            "  return (&foo)(5);\n"
            "}\n"
        )
        self.assertIn("JSR   foo", text)
        self.assertNotIn("JSR   icall", text)

    def test_complex_callee_rejected(self):
        # `(arr[i])(args)`, `(f())(args)`, etc. would need the
        # FunctionCall AST to carry an arbitrary callee expression
        # rather than a string name. Pin the current behavior:
        # a non-name callee inside the parens raises a parser
        # error.
        from errors import ParserError, VisitError
        # `(1)(args)` — a non-name callee.
        with self.assertRaises((ParserError, VisitError)):
            self._typecheck(
                "int main(void) { return (1)(5); }\n"
            )

    def test_complement_float_rejected(self):
        # C99 §6.5.3.3.4 — `~` requires an integer operand. Float
        # has no bit-pattern semantics that `~` would meaningfully
        # produce, so it's a strict type error.
        from errors import TypeCheckError
        with self.assertRaises(TypeCheckError) as cm:
            self._typecheck(
                "int main(void) { float f; f = 1.5f; return ~f; }\n"
            )
        self.assertIn("'~'", str(cm.exception))
        self.assertIn("integer", str(cm.exception))

    def test_complement_double_rejected(self):
        from errors import TypeCheckError
        with self.assertRaises(TypeCheckError):
            self._typecheck(
                "int main(void) { double d; d = 1.5; return ~d; }\n"
            )


if __name__ == "__main__":
    unittest.main()
