# Test status

This file enumerates every chapter\_N test from
nlsandler/writing-a-c-compiler-tests that doesn't pass under c6502
today, with the reason. Every other test passes.

The harnesses (`tests/test_chapter_<N>.py`) carry these as frozen
sets — `_INCOMPATIBLE_VALID` (skipped), `_EXPECTED_FAILURES_CODEGEN`
(must currently fail), and `_NOT_REJECTED_TODAY` (must currently
not be rejected). Each pinned entry is asserted at its current
behavior, so a regression OR a fix flips the test in either
direction and prompts a drop from the list.

Categories below:

- **Permanently incompatible** — c6502's narrow integer / 256-byte-frame
  / 1-byte-int model fundamentally can't compile these. No fix
  planned. Skipped at harness time.
- **Pinned: feature gaps** — features c6502 plans to add (`switch`,
  proper FP type-checks, etc.). Pinned at current accept/reject
  behavior.
- **Pinned: real bugs** — the compiler crashes or wrongly accepts
  programs the spec rejects. Pinned at current behavior.

Multi-TU `libraries/` subdirs (and platform-specific `.s` files)
aren't listed: they're skipped at import time, not at harness time.

---

## Permanently incompatible

### Integer literal beyond 16-bit Long range

c6502's `long` is 2 bytes (max 65535). The upstream tests assume an
8-byte `long`, so any literal whose only fitting type would be
`long long` / `unsigned long long` is rejected at parse time.

- **chapter\_2:**
  - `valid/bitwise_int_min.c` — uses INT\_MAX (2147483647)
  - `valid/negate_int_max.c` — same
- **chapter\_5:**
  - `valid/allocate_temps_and_vars.c`
  - `valid/extra_credit/compound_bitwise_shiftr.c`
- **chapter\_8:**
  - `valid/empty_loop_body.c`
  - `valid/for_absent_post.c`
- **chapter\_9:**
  - `valid/stack_arguments/test_for_memory_leaks.c`
- **chapter\_11:**
  26 files exercising 8-byte `long` semantics:
  - `valid/explicit_casts/truncate.c`
  - `valid/extra_credit/bitshift.c`
  - `valid/extra_credit/bitwise_long_op.c`
  - `valid/extra_credit/compound_assign_to_int.c`
  - `valid/extra_credit/compound_assign_to_long.c`
  - `valid/extra_credit/compound_bitshift.c`
  - `valid/extra_credit/compound_bitwise.c`
  - `valid/extra_credit/increment_long.c`
  - `valid/implicit_casts/common_type.c`
  - `valid/implicit_casts/convert_by_assignment.c`
  - `valid/implicit_casts/convert_function_arguments.c`
  - `valid/implicit_casts/convert_static_initializer.c`
  - `valid/implicit_casts/long_constants.c`
  - `valid/long_expressions/arithmetic_ops.c`
  - `valid/long_expressions/assign.c`
  - `valid/long_expressions/comparisons.c`
  - `valid/long_expressions/large_constants.c`
  - `valid/long_expressions/logical.c`
  - `valid/long_expressions/long_and_int_locals.c`
  - `valid/long_expressions/long_args.c`
  - `valid/long_expressions/multi_op.c`
  - `valid/long_expressions/return_long.c`
  - `valid/long_expressions/rewrite_large_multiply_regression.c`
  - `valid/long_expressions/simple.c`
  - `valid/long_expressions/static_long.c`
  - `valid/long_expressions/type_specifiers.c`
  - `valid/extra_credit/switch_int.c` — case constants beyond 16-bit
  - `valid/extra_credit/switch_long.c` — same
- **chapter\_12:**
  25 files with `unsigned int` literals beyond 16-bit ULong:
  - `valid/explicit_casts/chained_casts.c`
  - `valid/explicit_casts/extension.c`
  - `valid/explicit_casts/round_trip_casts.c`
  - `valid/explicit_casts/same_size_conversion.c`
  - `valid/explicit_casts/truncate.c`
  - `valid/extra_credit/bitwise_unsigned_ops.c`
  - `valid/extra_credit/bitwise_unsigned_shift.c`
  - `valid/extra_credit/compound_assign_uint.c`
  - `valid/extra_credit/compound_bitshift.c`
  - `valid/extra_credit/compound_bitwise.c`
  - `valid/extra_credit/postfix_precedence.c`
  - `valid/extra_credit/unsigned_incr_decr.c`
  - `valid/implicit_casts/common_type.c`
  - `valid/implicit_casts/convert_by_assignment.c`
  - `valid/implicit_casts/promote_constants.c`
  - `valid/implicit_casts/static_initializers.c`
  - `valid/type_specifiers/unsigned_type_specifiers.c`
  - `valid/unsigned_expressions/arithmetic_ops.c`
  - `valid/unsigned_expressions/arithmetic_wraparound.c`
  - `valid/unsigned_expressions/comparisons.c`
  - `valid/unsigned_expressions/locals.c`
  - `valid/unsigned_expressions/logical.c`
  - `valid/unsigned_expressions/simple.c`
  - `valid/unsigned_expressions/static_variables.c`
  - `valid/extra_credit/switch_uint.c` — case constants beyond 16-bit
- **chapter\_13:**
  9 files mixing FP with 8-byte int literals:
  - `valid/explicit_casts/double_to_signed.c`
  - `valid/explicit_casts/double_to_unsigned.c`
  - `valid/explicit_casts/signed_to_double.c`
  - `valid/explicit_casts/unsigned_to_double.c`
  - `valid/extra_credit/compound_assign_implicit_cast.c`
  - `valid/floating_expressions/logical.c`
  - `valid/implicit_casts/common_type.c`
  - `valid/implicit_casts/convert_for_assignment.c`
  - `valid/implicit_casts/static_initializers.c`
- **chapter\_14:**
  9 files mixing pointers with 8-byte int literals:
  - `valid/dereference/read_through_pointers.c`
  - `valid/dereference/static_var_indirection.c`
  - `valid/dereference/update_through_pointers.c`
  - `valid/extra_credit/bitshift_dereferenced_ptrs.c`
  - `valid/extra_credit/bitwise_ops_with_dereferenced_ptrs.c`
  - `valid/extra_credit/compound_assign_conversion.c`
  - `valid/extra_credit/compound_bitwise_dereferenced_ptrs.c`
  - `valid/extra_credit/incr_and_decr_through_pointer.c`
  - `valid/extra_credit/switch_dereferenced_pointer.c` — case
    constants beyond 16-bit
- **chapter\_15:**
  - `casts/implicit_and_explicit_conversions.c`
  - `declarators/big_array.c` — array dim 4294967297L
  - `extra_credit/bitwise_subscript.c`
  - `extra_credit/compound_assign_to_subscripted_val.c`
  - `extra_credit/compound_bitwise_subscript.c`
  - `extra_credit/compound_pointer_assignment.c`
  - `initialization/automatic.c`
  - `initialization/automatic_nested.c`
  - `initialization/static.c`

### Frame beyond 253 bytes

c6502's soft-stack frame addresses pseudos as `LDY #off` against
`(FP),Y`, which caps a function's frame size at 256 bytes (the test
infrastructure caps at 253 to leave headroom).

- **chapter\_13:**
  - `valid/function_calls/double_and_int_params_recursive.c`
- **chapter\_15:**
  - `extra_credit/incr_and_decr_nested_pointers.c`
  - `subscripting/addition_subscript_equivalence.c` — array of 3000+ bytes

### 1-byte int can't hold static initializer

c6502's `int` is 1 byte (max 255). The upstream tests sometimes
declare `unsigned int x = N;` with `N >> 255`.

- **chapter\_12:**
  - `valid/explicit_casts/rewrite_movz_regression.c` — `unsigned glob = 5000u;`

---

## Pinned: feature gaps

### Lexer accepts malformed FP exponent

The C standard treats `1.0e10.0` as a single preprocessing-number
that can't be converted to a constant. c6502's lexer has no
preprocessing-number concept and tokenises it as two CONSTANTs
(`1.0e10` and `.0`).

- **chapter\_13** invalid_lex:
  - `malformed_exponent.c`

---

## Pinned: real bugs

### chapter\_15 feature gaps the harness pins

- `extra_credit/incr_decr_subscripted_vals.c` — postfix `++`/`--`
  on a `Subscript` lvalue not wired through.
- `subscripting/simple_subscripts.c` — reverse subscript `i[arr]`
  (Int as the array side of `Subscript`) isn't accepted; also
  exercises FP comparisons that aren't lowered yet.
