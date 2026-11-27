# Test status

This file lists every program of the chapter\_N corpora from
nlsandler/writing-a-c-compiler-tests that c6502 doesn't handle as its
bucket says, with the reason. Every other program of a `valid/` bucket
compiles, and every other program of an `invalid_*` bucket is rejected
with a one-line error.

golden/unopt.txt and golden/O.txt hold each program's exit status and
message, so `c6502cpp check-golden` reports a change in any of them,
and a change made on purpose is taken with `check-golden --update`.
Until 26 Nov 2026 the Python harnesses `tests/test_chapter_<N>.py` held
the lists below as frozen sets and checked each program's error class
too. Step 9 of the port to C++ dropped them, since the golden files
cover what they checked apart from the class (docs/cpp_port.md, "The
tests golden/ covers").

The `libraries/` programs are halves of programs of two units, which
the harnesses skipped. golden/ compiles each half alone, and the ones
listed below fail for the reasons given.

## Locally adapted tests

Many upstream files were locally rewritten to fit c6502's narrow
integer and 253-byte frame model. The adaptations substitute c6502's
wider types for upstream's:

- upstream `int`     (4 B) → c6502 `long`     (4 B)
- upstream `long`    (8 B) → c6502 `long long` (8 B)
- upstream `unsigned int`  → c6502 `unsigned long`
- upstream `unsigned long` → c6502 `unsigned long long`

Literal magnitudes scale accordingly. Some files were also
restructured to split a large frame across helper functions, since
c6502's soft-stack frame is capped at 253 bytes by the `LDY #offset`
of each access.

chapter_19's programs are run in the TAC interpreter, and
golden/tacsim.txt holds `main`'s value for each. 132 of them equal
upstream's tests/chapter_19/expected_results.json, 20 equal the values
c6502's narrower types give (`_C6502_OVERRIDES` of the dropped
harness), and 15 call into libc (`putchar`, `copysign`,
`double_isnan`) or `exit_wrapper` of `helper_libs/exit.c`. Fourteen of
those stop at the call, and
`whole_pipeline/all_types/fold_cast_to_double.c` returns 4 before it
reaches its call of `copysign`.

---

## Frame larger than 253 bytes (`valid`, rejected)

`replace_pseudoregisters` lays out a frame larger than `LDY`
immediate addressing can reach, and the compile stops with "its locals
need N bytes on the soft stack, but a frame holds at most 253". The
literals were already adapted to c6502's integer widths, so splitting
`main` across helper functions is the only change left.

- chapter_15: `extra_credit/compound_bitwise_subscript.c`
- chapter_16: `char_constants/char_constant_operations.c`,
  `chars/convert_by_assignment.c`, `chars/explicit_casts.c`
- chapter_17: `void_pointer/conversion_by_assignment.c`
- chapter_18: `extra_credit/other_features/compound_assign_struct_members.c`,
  `extra_credit/other_features/incr_struct_members.c`,
  `no_structure_parameters/scalar_member_access/arrow.c`,
  `no_structure_parameters/scalar_member_access/nested_struct.c`,
  `no_structure_parameters/size_and_offset_calculations/member_offsets.c`

chapter_16's `extra_credit/compound_assign_chars.c` was listed with
them and compiles now.

## An array too large for the address space (chapter_17 `valid`, rejected)

- `sizeof/sizeof_derived_types.c` takes `sizeof(int[4294967297L][100000000])`,
  and the parser rejects the size with "array size must be an integer
  constant". The harness gave the reason as a literal that doesn't fit
  c6502's widest type, from before `long long` was 8 bytes, and the
  reason now wasn't traced.

## Valid in C23 (chapter_18 `invalid_parse`, compiled)

C23 allows the empty initializer `{}` (§6.7.11), which upstream's C17
corpus rejects.

- `empty_initializer_list.c`
- `extra_credit/union_empty_initializer.c`

## The halves of two-unit programs (chapter_18 `libraries/`, rejected alone)

- Frames larger than 253 bytes: `extra_credit/libraries/param_passing_client.c`,
  `extra_credit/libraries/union_retvals.c`,
  `extra_credit/libraries/union_retvals_client.c`,
  `no_structure_parameters/libraries/initializers/nested_auto_struct_initializers.c`,
  `no_structure_parameters/libraries/initializers/nested_static_struct_initializers.c`,
  `parameters/libraries/struct_sizes_client.c`,
  `params_and_returns/libraries/access_retval_members_client.c`,
  `params_and_returns/libraries/return_calling_conventions_client.c`,
  `params_and_returns/libraries/retval_struct_sizes_client.c`
- A frame offset past 255: `parameters/libraries/struct_sizes.c`
- A struct member initialized by an expression, which needs brace
  elision: `extra_credit/libraries/classify_unions_client.c`,
  `no_structure_parameters/libraries/initializers/nested_auto_struct_initializers_client.c`,
  `parameters/libraries/param_calling_conventions_client.c`
