# fuzz/CLAUDE.md

Random-program testing. Each tool generates C programs, runs them
through every c6502 execution path, and reports any path that gives
a wrong value. In Nov 2026 these found about twenty miscompiles that
the chapter corpus and the per-pass tests had missed. Run them after
changing a pass, the ABI selection, or the register allocator.

Run everything from the repo root, as modules:

```sh
uv run python -m fuzz.exprs --start 0 --count 300 -j 12   # in 12 processes
uv run python -m fuzz.exprs --start 0 --count 300   # expression semantics
uv run python -m fuzz.calls --start 0 --count 300   # calling conventions
uv run python -m fuzz.stmts --start 0 --count 300   # control flow, storage
uv run python -m fuzz.yarpgen --start 0 --count 100 # YARPGen's programs
uv run python -m fuzz.runner prog.c dir/             # check given programs
uv run python -m fuzz.reduce fuzz_out/calls/calls534.c
uv run python -m fuzz.tac_passes fuzz_out/stmts/stmts1549.c  # guilty TAC pass
```

Failing programs land in `fuzz_out/<tool>/` (ignored by git). A seed
always generates the same program, so `--start 534 --count 1`
reproduces seed 534.

`-j N` runs the seeds in N processes (`fuzz_main`, a `spawn` pool;
each worker imports `runner.py`, which turns the IR checks on), with
the output in seed order. `-j 12` on a 12-thread machine is about 5
times faster than one process.

## IR checks

`runner.py` turns on `passes/ir_check.py`, which checks the IR after
every TAC optimizer pass, at the asm stage boundaries, and after each
peephole sweep. A pass that leaves the IR inconsistent, such as one
that drops the definition of a temporary something still reads, then
fails right there with `IRCheckError: after <pass>, function '<f>':
...`, even when the program's result would have come out right. The
error is an `ERR` result like any other, so the program is saved.

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
calls on purpose. About 30% of the functions carry
`__attribute__((return_bool_in_carry))`, and half the calls to them
are `if` conditions, so the carry-flag return is checked both as a
value and as a branch. Those choices come from a second random
stream (`crng`), so adding them left the rest of each older seed's
program as it was.

In half the seeds that have `rec`, it keeps a 1-byte and a 4-byte
local live across its recursive call under `#pragma c6502 call
save(hardware_stack)`, so it gets a private pool and saves them on
the hardware stack (`passes/recursion_saves.py`). That choice has its
own random stream too.

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
About a quarter of the scalar globals, statics, and locals are
declared `volatile`, chosen with a second RNG so that the qualifier
doesn't change the rest of a seed's program. An object whose address
is taken (a `*gp` target, or a local of a type some helper takes by
pointer) stays plain. `mix()` folds values into a checksum that main
returns.

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

Once it declared objects `volatile`, 43 of seeds 4400-4999 came out
wrong in every path, the TAC interpreter included: `_emit_cast`
tested a `volatile signed char` as written, didn't match `SChar`,
and zero-extended it, which `const signed char` also hit (seed 4984).
`reduce.py` didn't help there, because it kept the stale `// expect`
line while reducing away the computation, so the cause was found by
removing `volatile` from one declaration at a time.

## `yarpgen.py`

Programs from YARPGen (https://github.com/intel/yarpgen), Intel's
generator of UB-free random C, which is still maintained. Its tests
are loop nests over arrays and scalars of every integer type, with
the values of every expression checked against undefined behavior by
YARPGen's own model of C. Build it once with `uv run python -m
fuzz.build_yarpgen`, which clones the pinned commit into
`fuzz_out/yarpgen-src`, applies `fuzz/yarpgen.patch`, and compiles it
with Zig's clang and CMake from PyPI through `uvx`, so nothing has to
be installed.

The patch makes the model's `int` 16 bits (`-DYARPGEN_INT_BITS=16`),
so a program YARPGen accepts is well-defined for c6502. Without it,
shifts by 16 to 31 and `int` overflow it had ruled out at 32 bits
would be undefined here. The patch also writes `min` / `max` as C99
macros instead of GNU statement expressions, and adds
`--max-loop-iters` and `--max-stmts`. YARPGen's own unit tests still
assume a 32-bit `int`, so the build script builds only the
generator.

`merge` turns a test's `init.h`, `func.c`, and `driver.c` into one
program. `main` returns the checksum folded to 15 bits instead of
printing it, and `size_t` is `unsigned int`. The
TAC result is the reference. A program takes about 6 seconds, and
most of them need more than 253 bytes of frame without --optimize,
so the runner counts them as rejected and compares only the
optimized path against TAC.

## `reduce.py`

Line-based delta reduction, then replacing `((a) op (b))` with one
operand, keeping a change when the program still fails the same way
(same wrong paths, each path still compiling or erroring as before).
`--tac-opt` reduces against "optimized TAC simulates differently from
unoptimized TAC" instead, which is faster and isolates TAC-optimizer
bugs. Reduced programs often have functions with no `return`, which
c6502 defines as returning 0.

## `tac_passes.py`

Simulates the whole program with the TAC interpreter after every TAC
optimizer pass (through `PhaseDriver.observer`) and prints the first
pass, and the function, after which `main` returns something other
than it did on unoptimized TAC. `tac_sim.py` runs the optimizer's own
instructions and the SSA form between passes, so this works on any
program, without reducing it first. If no TAC pass changes the
result but the `--optimize` path is still wrong, the bug is in
`tac_to_asm` or an asm pass. Then turn off the asm passes in
`compile` one at a time (replace `compile.apply_<name>` with an
identity) until the result comes out right, which is how
`loop_counter_to_x` was found for `fuzz.stmts` seed 1549.
