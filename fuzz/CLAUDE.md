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
uv run python -m fuzz.stmts --start 0 --count 300   # control flow, storage
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

The fuzz drivers count a program that only an asm path rejects with
a known backend limit (an unoptimized frame over the 253-byte limit,
about 1 in 100 `exprs` programs; `runner.is_backend_limit`) as
"rejected" rather than wrong, and don't save it. Any other asm-path
error is a failure. A
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

## `stmts.py`

Random statement bodies for a few helper functions and `main`:
`if` / `else`, `for` / `while` / `do` loops nested up to three deep,
`switch` with fall-through and `default`, `break`, `continue`, early
`return`, and the assignment operators (`++` / `--` both ways and the
compound assignments) as statements and with their value used. The
objects are locals (some passed to a helper by pointer), block
statics, globals, zero-page globals, `*gp` through a global pointer,
array elements, and struct members, across all eight integer types.
`mix()` folds values into a checksum that main returns.

The generator builds a tree, renders it, and runs the same tree in a
Python interpreter under C's rules, so the program carries `// expect:
N` and every path, the TAC interpreter included, is checked against
it. Undefined behavior is ruled out by construction (one modification
per expression, on an object nothing else in the statement names;
loop counters the body never writes) or, for signed overflow and bad
shifts, by the interpreter raising `_UB`, in which case the seed
draws another program. A function is capped at `_STMT_BUDGET`
statements so the unoptimized frame stays under 253 bytes.

With the INC / DEC fix of `361c2e7` undone, it finds that bug in 3
of the first 150 seeds. In its first 3,000 seeds it found three
more:

- the INC peephole minting `.inc_done@0` twice in one function on
  separate fixed-point rounds (seed 987, rejected by the assembler);
- move coalescing putting the shift temp of `p *= 5` in `p`'s slot
  while `p` was still to be read (seed 1921, a wrong value with
  --optimize);
- `redundant_load` taking an empty `z_reflects` to mean "Z reflects
  A", after a soft-stack store's `LDY #off` had set Z (seed 2391, a
  wrong value without --optimize).

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
