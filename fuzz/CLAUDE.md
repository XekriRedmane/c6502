# fuzz/CLAUDE.md

Random-program testing. Each generator writes C programs, runs them
through every c6502 execution path, and reports any path that gives
a wrong value. In Nov 2026 these found about twenty miscompiles that
the chapter corpus and the per-pass tests had missed. Run them after
changing a pass, the ABI selection, or the register allocator.

The fuzzers are C++, in cpp/src/fuzz/, and run as `c6502cpp fuzz`
(cpp/src/fuzz/command.h). Group G6 of step 9 of the port
(docs/cpp_port.md) moved them from the Python modules of this
directory, which wrote the same program for each seed and the same
result for each path. This directory holds what isn't code:
`build_yarpgen.sh`, which builds YARPGen, and `yarpgen.patch`, which
it applies.

Run everything from the repo root:

```sh
cpp/build/bin/c6502cpp fuzz exprs --start 0 --count 300 -j 12   # in 12 processes
cpp/build/bin/c6502cpp fuzz exprs --start 0 --count 300   # expression semantics
cpp/build/bin/c6502cpp fuzz calls --start 0 --count 300   # calling conventions
cpp/build/bin/c6502cpp fuzz stmts --start 0 --count 300   # control flow, storage
cpp/build/bin/c6502cpp fuzz yarpgen --start 0 --count 100 # YARPGen's programs
cpp/build/bin/c6502cpp fuzz run prog.c dir/               # check given programs
cpp/build/bin/c6502cpp fuzz reduce fuzz_out/calls/calls534.c
cpp/build/bin/c6502cpp fuzz tac-passes fuzz_out/stmts/stmts1549.c  # guilty TAC pass
cpp/build/bin/c6502cpp fuzz programs stmts --start 0 --count 10 --out dir  # just write them
```

Failing programs land in `fuzz_out/<generator>/` (ignored by git). A
seed always generates the same program, so `--start 534 --count 1`
reproduces seed 534. The generators draw from cpp/src/py_random.h,
a copy of CPython's `random`, so a seed names the program the Python
generators wrote for it, and the seeds that the tests, brain/, and
docs/ cite still reproduce their bugs. golden/fuzz/ holds seeds 0 to
99 of `exprs`, `calls`, and `stmts`, which `check-golden` compiles and
cpp/tests/test_fuzz_generators.cpp checks the generators against.

`-j N` runs the seeds in N processes, one seed to a process (`c6502cpp
fuzz worker`, which writes a record of its seed for the parent), with
the output in seed order. The seeds don't run on threads, since the
compiler keeps some of its state in globals: the switch of the IR
checks (`ir_check::enabled`), the TAC optimizer's observer
(`framework::set_observer`), and the caches of `ir_check`'s CFG shapes
and of `fnmatch`'s compiled patterns. `--verbose` writes a line for
each program that passed too. 300 `stmts` programs take about 39
seconds in one process and 8 with `-j 12`, and 100 `yarpgen` programs
15 seconds with `-j 12`.

## IR checks

The runner turns on the IR checks (cpp/src/passes/ir_check.h), which
check the IR after every TAC optimizer pass, at the asm stage
boundaries, and after each peephole sweep. A pass that leaves the IR
inconsistent, such as one that drops the definition of a temporary
something still reads, then fails right there with `AssertionError:
after <pass>, function '<f>': ...`, even when the program's result
would have come out right. The error is an `ERR` result like any
other, so the program is saved.

## Execution paths (cpp/src/fuzz/runner.h)

Every program runs three ways, and main()'s return is compared:

- `tac`: the TAC interpreter on unoptimized TAC.
- `unopt`: the simulator on the program as `-S` compiles it (soft
  stack everywhere).
- `opt`: the simulator on the program as `-S -O` compiles it (zp_abi,
  private pools, `register` placement).

A result is an int, or a string starting with `ERR` or `TIMEOUT`.
An error names the Python class the error stands for and gives the
first 120 characters of its message, as the Python runner wrote it.
`disagreements` names the wrong paths. When a program states its
correct result (`exprs` does, and a hand-written file can with a
`// expect: N` line), every path is checked against it. Otherwise the
TAC result is the reference and only `unopt` and `opt` are checked.

The fuzz command counts a program that only an asm path rejects with
a known backend limit (an unoptimized frame over the 253-byte limit,
about 1 in 12 `exprs` programs; `is_backend_limit`) as "rejected"
rather than wrong, and doesn't save it. Any other asm-path error is a
failure. A front-end rejection is saved as a failure, because the
generators only produce valid C.

## `exprs`

Eight random expressions over six variables of random integer types,
each compared inside the program against a value computed in C++
under C99's rules with c6502's widths, in `asdl::Int` (128 bits),
which holds every value Python's unbounded integers held. main()
returns a bitmask of wrong expressions, so the expected result is 0.
The generator throws away expressions with undefined behavior (signed
overflow, division by zero, bad shift counts, left shift of a
negative value). The six variables are globals in half the seeds and
`check()` parameters in the other half, which covers static
initializers and zp_abi parameters.

It catches front-end and codegen bugs in conversions, promotions,
signedness, comparisons, and multi-byte arithmetic. It does not
exercise control flow or calls beyond the one call to `check`.

Half the seeds add three of C23's bit-precise types, `_BitInt(N)` or
`unsigned _BitInt(N)` of random widths from 1 to 64, to the types the
variables, casts, and constants draw from.
cpp/src/fuzz/bit_precise_types.h has their rules (no promotion, rank
by width below a standard type of the same width, and the usual
arithmetic conversions), which `stmts` uses too. Whether a seed has
them, and which, come from a random stream of their own
(`choose_bit_precise`), so the seeds without them generate the
programs they did before. Those seeds' unoptimized frames are larger,
since restoring a bit-precise value's padding takes temporaries. Of
seeds 0 to 199, 15 of the 93 programs with bit-precise types passed
the 253 bytes and were counted as rejected, and none of the 107
without them did.

## `calls`

Three to six functions in a DAG, with a recursive function and a
function called through a pointer in some seeds (both stay on the
soft stack), `register` parameters and locals on the functions that
will get zp_abi, pointer parameters to callers' locals, calls inside
loops, and globals updated across calls. Locals stay live across the
calls on purpose. About 30% of the functions return `bool`, and half
the calls to them are `if` conditions, so the carry-flag return is
checked both as a value and as a branch. Half of those functions
carry `[[c6502::return_bool_in_carry]]`, and the rest return in the
carry flag under `-O` when they're zp_abi, by the compiler's
choice (cpp/src/passes/carry_return_choice.h). Those choices come from
random streams of their own (`crng`, and `arng` for the attribute),
so adding them left the rest of each older seed's program as it was.

In half the seeds that have `rec`, it keeps a 1-byte and a 4-byte
local live across its recursive call, marked
`[[c6502::save_hardware_stack]]`, so it gets a private pool and saves them on
the hardware stack (cpp/src/passes/recursion_saves.h). That choice has its
own random stream too.

Half the seeds add two bit-precise types, `_BitInt(N)` or `unsigned
_BitInt(N)`, to the types of the globals, parameters, returns, locals,
and casts, from a stream of their own like the choices above. The TAC
interpreter is the reference, so the generator has no model of
their rules. A 3-byte return comes back in `HARGS`, as a 4-byte one
does, and a bit-precise parameter of 1 or 2 bytes can be a `register`
parameter.

The generator predicts which functions end up on the soft stack (any
function that reaches `rec` or the pointer call) and only puts
`register` on the others, because `register` needs zp_abi and the
compiler rejects it otherwise. If the ABI eligibility rules in
cpp/src/passes/abi_selection.cpp change, update `soft_stack_functions`
in cpp/src/fuzz/calls.cpp to match, or the fuzzer reports rejections as
failures.

## `stmts`

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

Half the seeds add two bit-precise types, `_BitInt(N)` or `unsigned
_BitInt(N)`, weighted like `long`, with the rules of
cpp/src/fuzz/bit_precise_types.h, chosen by `choose_bit_precise`.

The generator builds a tree, renders it, and runs the same tree in its
own interpreter under C's rules, so the program carries `// expect:
N` and every path, the TAC interpreter included, is checked against
it. Undefined behavior is ruled out by construction (one modification
per expression, on an object nothing else in the statement names;
loop counters the body never writes) or, for signed overflow and bad
shifts, by the interpreter stopping, in which case the seed draws
another program. A function is capped at
`kStmtsPerFnUnderFrameLimit` statements so the unoptimized frame stays
under 253 bytes.

With the INC / DEC fix of `361c2e7` undone, it finds that bug in 3
of the first 150 seeds. In its first 3,000 seeds it found three
more:

- the INC peephole minting `.inc_done@0` twice in one function on
  separate fixed-point rounds (seed 987, rejected by the assembler);
- move coalescing putting the shift temp of `p *= 5` in `p`'s slot
  while `p` was still to be read (seed 1921, a wrong value with
  -O);
- `redundant_load` taking an empty `z_reflects` to mean "Z reflects
  A", after a soft-stack store's `LDY #off` had set Z (seed 2391, a
  wrong value without -O).

Once it declared objects `volatile`, 43 of seeds 4400-4999 came out
wrong in every path, the TAC interpreter included: `_emit_cast`
tested a `volatile signed char` as written, didn't match `SChar`,
and zero-extended it, which `const signed char` also hit (seed 4984).
The reducer didn't help there, because it kept the stale `// expect`
line while reducing away the computation, so the cause was found by
removing `volatile` from one declaration at a time.

## `yarpgen`

Programs from YARPGen (https://github.com/intel/yarpgen), Intel's
generator of UB-free random C, which is still maintained. Its tests
are loop nests over arrays and scalars of every integer type, with
the values of every expression checked against undefined behavior by
YARPGen's own model of C. Build it once with `bash
fuzz/build_yarpgen.sh`, which clones the pinned commit into
`fuzz_out/yarpgen-src`, applies `fuzz/yarpgen.patch`, downloads Zig
into `fuzz_out`, and compiles YARPGen with Zig's clang, so nothing
but CMake and Ninja has to be installed. The comment at the top of
the script says why it uses Zig's clang and not MSVC: YARPGen's
programs depend on the standard library's distributions, and only a
build against libc++ writes the programs the earlier builds wrote.

The patch makes the model's `int` 16 bits (`-DYARPGEN_INT_BITS=16`),
so a program YARPGen accepts is well-defined for c6502. Without it,
shifts by 16 to 31 and `int` overflow it had ruled out at 32 bits
would be undefined here. The patch also writes `min` / `max` as C99
macros instead of GNU statement expressions, and adds
`--max-loop-iters` and `--max-stmts`. YARPGen's own unit tests still
assume a 32-bit `int`, so the build script builds only the
generator.

`merge` (cpp/src/fuzz/yarpgen.h) turns a test's `init.h`, `func.c`,
and `driver.c` into one program. `main` returns the checksum folded to
15 bits instead of printing it, and `size_t` is `unsigned int`.
YARPGen's C output spells the boolean type `_Bool`, which draws C23's
deprecation warning, but the runner collects no warnings, so they
don't count against a program. The TAC result is the reference. Most
of the programs need more than 253 bytes of frame without -O, so the
runner counts them as rejected and compares only the optimized path
against TAC.

## `reduce` (cpp/src/fuzz/reduce.h)

Line-based delta reduction, then replacing `((a) op (b))` with one
operand, keeping a change when the program still fails the same way
(same wrong paths, each path still compiling or erroring as before).
`--tac-opt` reduces against "optimized TAC simulates differently from
unoptimized TAC" instead, which is faster and isolates TAC-optimizer
bugs. The result is written next to the input as `<name>.min.c`.

A candidate whose run on the TAC interpreter reaches the end of a
function that returns a value, other than `main`, is never kept
(`falls_off_end`, which appends a call of an undefined function to
each such body and runs the program). Using such a function's value
is undefined (C99 6.9.1p12), so every result is allowed, and before
this check the reduction of `calls` seed 335 emptied three functions
whose results `f0` still used, which left a disagreement that meant
nothing. The reducer doesn't rule out the other kinds of undefined
behavior an operand replacement can bring in, such as a shift by too
many places or signed overflow, so a reduced program has to be
checked for them by hand.

## `tac-passes`

Simulates the whole program with the TAC interpreter after every TAC
optimizer pass (the observer of the TAC optimizer's driver,
cpp/src/passes/optimization/driver.h, through
`tac_sim_modes::first_changing_pass`) and prints the first pass, and
the function, after which `main` returns something other than it did
on unoptimized TAC. The IR checks are on when `C6502_CHECK_IR=1` is
set. The interpreter (cpp/src/tac_sim.h) runs the optimizer's own
instructions and the SSA form between passes, so this works on any
program, without reducing it first. If no TAC pass changes the
result but the `-O` path is still wrong, the bug is in
`tac_to_asm` or an asm pass. Then turn off the asm passes one at a
time, by taking the call of each out of
cpp/src/passes/peephole_fixedpoint.cpp or
cpp/src/passes/optimized_asm.cpp in a build of your own, until the
result comes out right, which is how `loop_counter_to_x` was found
for `stmts` seed 1549.
