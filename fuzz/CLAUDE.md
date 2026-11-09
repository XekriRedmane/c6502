# fuzz/CLAUDE.md

Random-program testing. Each tool generates C programs, runs them
through every c6502 execution path, and reports any path that gives
a wrong value. In Nov 2026 these found about twenty miscompiles that
the chapter corpus and the per-pass tests had missed. Run them after
changing a pass, the ABI selection, or the register allocator.

Run everything from the repo root, as modules:

```sh
uv run python -m fuzz.exprs --start 0 --count 300   # expression semantics
uv run python -m fuzz.calls --start 0 --count 300   # calling conventions
uv run python -m fuzz.runner prog.c dir/             # check given programs
uv run python -m fuzz.reduce fuzz_out/calls/calls534.c
```

Failing programs land in `fuzz_out/<tool>/` (ignored by git). A seed
always generates the same program, so `--start 534 --count 1`
reproduces seed 534.

## Execution paths (`runner.py`)

Every program runs three ways, and main()'s return is compared:

- `tac`: the TAC interpreter on unoptimized TAC.
- `unopt`: the asm simulator on `compile.py --codegen` output
  (soft stack everywhere).
- `opt`: the asm simulator on `compile.py --codegen --optimize`
  output (zp_abi, private pools, `register` placement).

A result is an int, or a string starting with `ERR` or `TIMEOUT`.
`disagreements` names the wrong paths. When a program states its
correct result (`exprs` does, and a hand-written file can with a
`// expect: N` line), every path is checked against it. Otherwise the
TAC result is the reference and only `unopt` and `opt` are checked.

The fuzz drivers count a program that only an asm path rejects (an
unoptimized frame over the 253-byte limit, about 1 in 100 `exprs`
programs) as "rejected" rather than wrong, and don't save it. A
front-end rejection is saved as a failure, because the generators
only produce valid C.

## `exprs.py`

Eight random expressions over six variables of random integer types,
each compared inside the program against a value computed in Python
under C99's rules with c6502's widths. main() returns a bitmask of
wrong expressions, so the expected result is 0. The generator throws
away expressions with undefined behavior (signed overflow, division
by zero, bad shift counts, left shift of a negative value). The six
variables are globals in half the seeds and `check()` parameters in
the other half, which covers static initializers and zp_abi
parameters.

It catches front-end and codegen bugs in conversions, promotions,
signedness, comparisons, and multi-byte arithmetic. It does not
exercise control flow or calls beyond the one call to `check`.

## `calls.py`

Three to six functions in a DAG, with a recursive function and a
function called through a pointer in some seeds (both stay on the
soft stack), `register` parameters and locals on the functions that
will get zp_abi, pointer parameters to callers' locals, calls inside
loops, and globals updated across calls. Locals stay live across the
calls on purpose.

The generator predicts which functions end up on the soft stack (any
function that reaches `rec` or the pointer call) and only puts
`register` on the others, because `register` needs zp_abi and the
compiler rejects it otherwise. If the ABI eligibility rules in
`passes/abi_selection.py` change, update `soft` in `make_program` to
match, or the fuzzer reports rejections as failures.

## `reduce.py`

Line-based delta reduction, then replacing `((a) op (b))` with one
operand, keeping a change when the program still fails the same way
(same wrong paths, each path still compiling or erroring as before).
`--tac-opt` reduces against "optimized TAC simulates differently from
unoptimized TAC" instead, which is faster and isolates TAC-optimizer
bugs. Reduced programs often have functions with no `return`, which
c6502 defines as returning 0.

After reducing, finding the guilty pass usually takes one of two
steps. For an asm-level bug, simulate the program after each pass of
`compile._peephole_fixedpoint` (the session that wrote these tools
wrapped every `apply_*` in `compile` to snapshot its output). For a
TAC-level bug, rerun with each pass removed from
`passes.optimization.optimizer._DRIVER.fixedpoint` in turn.
