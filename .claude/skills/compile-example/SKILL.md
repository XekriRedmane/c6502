---
name: compile-example
description: Compiles examples/<name>.c with -O into examples/<name>.asm
  and checks the optimized build against the unoptimized one in the
  simulator. Triggers: "compile example", "compile <name>",
  /compile-example.
---

# Compile Example

Takes a filename argument naming a source file in `examples/` and runs
it through `compile.py -S -O`, writing the output to
the matching `.asm` file in `examples/`. `-O` runs the optimizer,
including the unrolling of `[[c6502::unroll]]` loops and the default
zp_abi convention.

## Input

The argument is the example's base name or filename. Accept any of:

- `floor_enemy_draw`
- `floor_enemy_draw.c`
- `examples/floor_enemy_draw.c`

Normalize to `examples/<base>.c` for input and `examples/<base>.asm`
for output. If the input `.c` doesn't exist, report it and stop.

## Command

From the repository root:

```sh
uv run python compile.py examples/<base>.c -S -O -o examples/<base>.asm
```

The `-o` path must end in `.s` or `.asm` (compile.py enforces this).

## After compiling

- If compile.py exits non-zero, the build hit a **compiler bug** —
  the source is presumed correct (it's a checked-in example), so the
  failure is in a c6502 pass. See "On compiler-bug failure" below.
- On success, report the output path and the new file's size in one
  line, then proceed to "Sim differential check" below.
- Do NOT commit any result — leave that to the user.

## Sim differential check

After a successful compile, ALWAYS verify that the unoptimized and
optimized pipelines produce identical observable state for this
example. This surfaces optimizer bugs that gold-file diffs (purely
structural) miss.

The mechanism is the matching `tests/test_<base>_sim.py` harness —
it inlines the example source with a `main()` driver that exercises
the function under a battery of scenarios and records observable
state into a `result_log` array, then asserts (a) unopt matches
hand-computed expected, (b) opt matches expected, and (c) opt
matches unopt byte-for-byte.

Run:

```sh
uv run python -m unittest tests.test_<base>_sim
```

- If it doesn't exist: write one before running. Model it after
  `tests/test_floor_enemy_advance_sim.py` — see step 6 of "On
  compiler-bug failure" below for the exact shape (inline source +
  `main()` driver + flat `result_log` + three tests: unopt vs.
  hand-computed expected, opt vs. expected, opt vs. unopt byte-
  for-byte). Then run it. Do not silently proceed to "On success"
  without the comparison; the comparison is the whole point.
- If it fails on the `optimize=True` test OR the
  `opt-matches-unopt` test: the optimizer mis-compiled this
  example. Treat as a compiler bug — loop into "On compiler-bug
  failure" starting at step 1 (the divergence IS the failure
  signal; the test output names the diverging bytes / cycles).
- If it fails on the `optimize=False` test only: the unoptimized
  pipeline regressed, or the hand-computed expected state is
  stale. Investigate which.
- If all three tests pass: proceed to "Measure code size and
  cycles" below.

## Measure code size and cycles

After the sim differential test passes, run both pipelines once
more to surface the user-code byte count, the whole-program cycle
total, AND the per-function cycle breakdown — the function under
test is typically a small fraction of the sim total (the
`record()` snapshot helper and the stubbed callees dominate), so
the whole-program number dilutes the optimization-relevant delta.
`Simulation.run_bucketed` attributes each step's cycles to the
function whose entry-point address is the largest not exceeding
the current PC; an `<other>` bucket collects boot-stub and
runtime-hook cycles.

From the project root:

```sh
uv run python -c "
from tests.test_<base>_sim import _PROGRAM
from sim.harness import build_sim
for opt in (False, True):
    sim = build_sim(_PROGRAM, optimize=opt)
    res, by_fn = sim.run_bucketed(max_cycles=5_000_000)
    assert not res.timed_out, f'timed out (optimize={opt})'
    label = 'opt  ' if opt else 'unopt'
    print(f'{label}: code={sim.code_end - sim.origin} bytes, '
          f'cycles={res.cycles}')
    for name, cyc in sorted(by_fn.items(), key=lambda kv: -kv[1]):
        if cyc == 0:
            continue
        pct = 100 * cyc / res.cycles
        print(f'    {name:30s} {cyc:6d}  ({pct:5.1f}%)')
"
```

If either pipeline times out at 5,000,000 cycles, the example's
`main()` driver is doing far more work than expected — raise the
cap or shrink the driver; do not silently swallow the timeout.

**Reading the breakdown:** the function under test is typically
the one whose name matches `<base>` (the example's filename).
Subtract its cycle count between unopt and opt for the honest
optimization-win number. The whole-program cycle delta will
usually be smaller because the test instrumentation (`record`,
stubbed callees, `main`) is constant across iterations.

Carry the byte counts, whole-program cycles, AND the
function-under-test slice into the success report.

## On compiler-bug failure

Don't silently overwrite a prior good `.asm` with nothing — leave the
old file in place. Then:

1. **Diagnose.** Rerun the command with `C6502_CHECK_IR=1`. A failed
   IR check is reported as `after <pass>, function '<name>': ...`
   (cpp/src/passes/ir_check.cpp), which names the pass. An
   `AssemblerError: unsupported Mov` from `sim/assembler.py` appears
   only in the sim differential test, and means a pass produced an
   atom the simulator's assembler can't encode. Find the pass that
   produced the bad shape by grepping cpp/src/passes/ for the
   constructor that built it (e.g. `IndexedData` for an
   `IndexedData` operand). Read that
   pass's eligibility filter; the bug is almost always a missing
   case in the filter.

2. **Reduce.** If the failure repros on the full example, also try
   to find a small `.c` that triggers it (a few lines of the
   example often suffice). The small repro becomes the test case.

3. **Write a failing test first.** Add a doctest case to the pass's
   `cpp/tests/test_<pass>.cpp` (e.g.
   `cpp/tests/test_direct_index_load.cpp` for the
   `direct_index_load` peephole) that builds the asm shape and
   checks that the pass *doesn't* produce the bad atom (or that the
   eligibility filter rejects the input). A new `.cpp` file needs
   `cmake -S cpp -B cpp/build` again before it is compiled. Confirm
   the test fails before the fix by putting the fix in and taking it
   out of the source only, never with `git stash`. If the offending
   shape is only producible end-to-end, fall back to a test in
   `tests/test_<pass>.py` that runs `_run_source("codegen", src,
   optimize=True)` on the small repro.

4. **Fix the pass.** Tighten the eligibility filter (or whatever
   the root cause is). Avoid working around the bug downstream;
   fix it where the bad shape originates.

5. **Re-run the unit test.** Rebuild (`cmake --build cpp/build
   --config Release --parallel`, with Visual Studio's cmake on the
   PATH), run `cpp/build/bin/c6502_tests -tc="<pass>*"`, and check
   that the case count includes the new case. It must now pass. Then
   run `uv run python -m unittest tests.test_<pass>` to catch
   regressions.

6. **Write a sim differential test for the example.** Add
   `tests/test_<example>_sim.py` modeled after
   `tests/test_floor_enemy_advance_sim.py`:
   - Inline the example source plus a `main()` that drives the
     function under a battery of scenarios and records observable
     state into a flat `result_log` array.
   - Compute the expected post-call state by hand for each
     scenario (this is the ground truth — the optimized output is
     not).
   - Three tests: unoptimized matches expected, optimized matches
     expected, optimized matches unoptimized byte-for-byte.
   - Use `sim.harness.build_sim(_PROGRAM, optimize=...)` to drive
     each variant; cap `max_cycles` generously and assert no
     timeout.
   - Decorate each class with `@needs_c6502cpp` from
     `tests/cpp_build.py`, as the model does.

7. **Run the sim differential test.** Both pipelines must produce
   identical observable state. If the optimized variant diverges
   from the unoptimized variant, the optimizer has another bug —
   loop back to step 1.

8. **Re-run the checks** to confirm no regressions:
   `C6502_CHECK_IR=1 uv run pytest -n auto --dist worksteal -q`,
   `ctest --test-dir cpp/build -C Release --output-on-failure`,
   `cpp/build/bin/c6502cpp check-golden -j 12` (and `--update`, after
   reading each changed job, when the output changed on purpose),
   `cpp/build/bin/c6502cpp check-frozen`, and `uv run python -m
   unittest tests.test_example_outputs`. Then re-run the original
   `compile.py` command from "Command" above; it should now
   succeed. golden/O.txt has a job for each example, so a new
   example needs one added, modelled on its neighbours, and
   `check-golden --update`.

9. Proceed to "On success".

## On success

- Report the output path and the new `.asm` text-file size.
- Report that the sim differential test passed (opt and unopt
  match).
- Report the assembled user-code byte count, whole-program cycle
  count, AND the function-under-test cycle slice for each
  pipeline. Example shape:

  ```
  unopt: 412 bytes, 18234 cycles  (fn: 3920 cycles)
  opt  : 287 bytes, 11502 cycles  (fn: 1640 cycles)
                                   fn delta: -2280 cycles (-58.2%)
  ```

  The function-under-test slice is the bucket named after the
  example (e.g. `floor_enemy_draw` for `examples/floor_enemy_draw.c`).
  The fn delta is the honest optimization-win number — the
  whole-program delta is diluted by the test instrumentation
  (`record`, stubbed callees) which is constant across iterations.

- Do NOT commit. Leave the `.asm`, any new test, and any pass fix
  staged in the working tree for the user to review.
