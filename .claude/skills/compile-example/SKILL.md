---
name: compile-example
description: Use when user says "compile example", "compile <name>",
  or invokes /compile-example with a filename — compiles a C source
  file from the examples/ directory with full optimization (--optimize
  --unroll) and writes the resulting 6502 assembly to examples/<name>.asm
---

# Compile Example

Takes a filename argument naming a source file in `examples/` and runs
it through `compile.py --codegen --optimize --unroll`, writing the
output to the matching `.asm` file in `examples/`.

## Input

The argument is the example's base name or filename. Accept any of:

- `floor_enemy_draw`
- `floor_enemy_draw.c`
- `examples/floor_enemy_draw.c`

Normalize to `examples/<base>.c` for input and `examples/<base>.asm`
for output. If the input `.c` doesn't exist, report it and stop.

## Command

From the project root (`/project/c6502`):

```sh
uv run python compile.py examples/<base>.c --codegen --optimize --unroll -o examples/<base>.asm
```

The `-o` path must end in `.asm` (compile.py enforces this).

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

1. **Diagnose.** The traceback's deepest c6502 frame names the
   failing pass. Common shapes: `AssemblerError: unsupported Mov:
   <src> -> <dst>` from `sim/assembler.py` means a peephole or
   lowering produced a non-encodable atom; an `AssertionError` in
   an asm pass usually means a precondition (Pseudos resolved,
   widths matched, etc.) was violated. Find the pass that produced
   the bad shape by grepping for the constructor that built it
   (e.g. `IndexedData(` for an `IndexedData` operand). Read that
   pass's eligibility filter; the bug is almost always a missing
   case in the filter.

2. **Reduce.** If the failure repros on the full example, also try
   to find a small `.c` that triggers it (a few lines of the
   example often suffice). The small repro becomes the test case.

3. **Write a failing test first.** Pick the matching tests/ file
   for the pass (e.g. `tests/test_direct_index_load.py` for the
   `direct_index_load` peephole) and add a unit test that builds
   the asm shape and asserts the pass *doesn't* produce the bad
   atom (or asserts the eligibility filter rejects the input).
   Confirm the test fails before the fix. If the offending shape
   is only producible end-to-end, fall back to an integration
   test that runs `compile.py --codegen --optimize` on the small
   repro.

4. **Fix the pass.** Tighten the eligibility filter (or whatever
   the root cause is). Avoid working around the bug downstream;
   fix it where the bad shape originates.

5. **Re-run the unit test.** It must now pass. Also run the
   broader test suite for that pass module (`uv run python -m
   unittest tests.test_<pass>`) to catch regressions.

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

7. **Run the sim differential test.** Both pipelines must produce
   identical observable state. If the optimized variant diverges
   from the unoptimized variant, the optimizer has another bug —
   loop back to step 1.

8. **Re-run the full chapter test suite** (`uv run python -m
   unittest`) to confirm no regressions. Then re-run the original
   `compile.py` command from "Command" above; it should now
   succeed.

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
