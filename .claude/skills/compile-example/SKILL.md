---
name: compile-example
description: Compiles examples/<name>.c with -O, compares the text with
  golden/O/examples/<name>.s, and checks the optimized build against
  the unoptimized one in the simulator. Triggers: "compile example",
  "compile <name>", /compile-example.
---

# Compile Example

Takes a filename argument naming a source file in `examples/` and runs
it through `c6502cpp compile -S -O`, writing the output to a file
under `fuzz_out/`, which git ignores, and compares it with the
example's golden copy, `golden/O/examples/<base>.s`. `-O` runs the
optimizer, including the unrolling of `[[c6502::unroll]]` loops and
the default zp_abi convention. The examples' own `.asm` files were
deleted in step 9 of the port, since each was byte for byte its
golden copy.

## Input

The argument is the example's base name or filename. Accept any of:

- `floor_enemy_draw`
- `floor_enemy_draw.c`
- `examples/floor_enemy_draw.c`

Normalize to `examples/<base>.c` for input and `fuzz_out/<base>.s`
for output. If the input `.c` doesn't exist, report it and stop.

## Command

From the repository root, with the compiler built (cpp/README.md,
"Building"):

```sh
cpp/build/bin/c6502cpp compile examples/<base>.c -S -O -o fuzz_out/<base>.s
diff golden/O/examples/<base>.s fuzz_out/<base>.s
```

The `-o` path must end in `.s` or `.asm` (`c6502cpp compile` enforces
this). A file named with `-o` gets LF line endings, as the golden
copies have, so `diff` shows only changes to the text.

## After compiling

- If `c6502cpp compile` exits non-zero, the build hit a **compiler
  bug** — the source is presumed correct (it's a checked-in example),
  so the failure is in a c6502 pass. See "On compiler-bug failure"
  below.
- On success, report the output path, the new file's size, and
  whether it differs from the golden copy, in one line, then proceed
  to "Sim differential check" below. A difference is a change to the
  compiler's output, which `cpp/build/bin/c6502cpp check-golden -j 12`
  reports too.
- Do NOT commit any result — leave that to the user.

## Sim differential check

After a successful compile, ALWAYS verify that the unoptimized and
optimized pipelines produce identical observable state for this
example. This surfaces optimizer bugs that gold-file diffs (purely
structural) miss.

The mechanism is the matching `cpp/tests/test_<base>_sim.cpp` test —
it inlines the example source with a `main()` driver that exercises
the function under a battery of scenarios and records observable
state into a `result_log` array, then asserts (a) unopt matches
hand-computed expected, (b) opt matches expected, and (c) opt
matches unopt byte-for-byte.

Run:

```sh
cpp/build/bin/c6502_tests -tc="<base>_sim*"
```

- If it doesn't exist: write one before running. Model it after
  `cpp/tests/test_floor_enemy_advance_sim.cpp` — see step 6 of "On
  compiler-bug failure" below for the exact shape (inline source +
  `main()` driver + flat `result_log` + three tests: unopt vs.
  hand-computed expected, opt vs. expected, opt vs. unopt byte-
  for-byte). Then run it. Do not silently proceed to "On success"
  without the comparison; the comparison is the whole point.
- If it fails on the optimized test OR the opt-matches-unopt test:
  the optimizer mis-compiled this example. Treat as a compiler bug —
  loop into "On compiler-bug failure" starting at step 1 (the
  divergence IS the failure signal; the test output names the
  diverging bytes / cycles).
- If it fails on the unoptimized test only: the unoptimized
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
The breakdown attributes each step's cycles to the function whose
entry-point address is the largest not exceeding the current PC,
and an `<other>` bucket collects the boot stub's, the runtime
helpers', and the hooks' cycles.

Add this case to the end of `cpp/tests/test_<base>_sim.cpp`, inside
its namespace, with `kProgram` replaced by the name of the test's
program string, and `#include <cstdio>`, `<iterator>`, and `<map>` at
the top if the file lacks them. Rebuild `c6502_tests` (step 5 of "On
compiler-bug failure" gives the command), run it with
`cpp/build/bin/c6502_tests -tc="<base>_sim: measure"`, and then take
the case out again, since it checks nothing.

```cpp
TEST_CASE("<base>_sim: measure") {
  for (const bool opt : {false, true}) {
    sim::Simulation s = build_sim(kProgram, opt);
    std::map<std::int64_t, std::string> entries;
    for (const auto& name : s.function_names) entries[s.symbol(name)] = name;
    std::map<std::string, std::uint64_t> by_fn;
    std::uint64_t before = s.cpu.cycles;
    while (s.cpu.cycles < 5'000'000) {
      const std::int64_t pc = s.cpu.pc;
      if (!s.step()) break;
      const auto next = entries.upper_bound(pc);
      const std::string name =
          pc < s.origin || pc >= s.code_end || next == entries.begin() ? "<other>" : std::prev(next)->second;
      by_fn[name] += s.cpu.cycles - before;
      before = s.cpu.cycles;
    }
    CHECK_MESSAGE(s.cpu.cycles < 5'000'000, "timed out, optimize=" << opt);
    std::printf("%s: code=%lld bytes, cycles=%llu\n", opt ? "opt  " : "unopt",
                static_cast<long long>(s.code_end - s.origin), static_cast<unsigned long long>(s.cpu.cycles));
    for (const auto& [fn, cycles] : by_fn) {
      std::printf("    %-30s %6llu  (%5.1f%%)\n", fn.c_str(), static_cast<unsigned long long>(cycles),
                  100.0 * static_cast<double>(cycles) / static_cast<double>(s.cpu.cycles));
    }
  }
}
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

Leave `golden/` as it is. Then:

1. **Diagnose.** Rerun the command with `C6502_CHECK_IR=1`. A failed
   IR check is reported as `after <pass>, function '<name>': ...`
   (cpp/src/passes/ir_check.cpp), which names the pass. An
   `AssemblerError` from the simulator's assembler
   (cpp/src/sim/assembler.h), such as one for an unsupported `Mov`,
   appears only in the sim differential test, and means a pass
   produced an atom the simulator's assembler can't encode. Find the
   pass that produced the bad shape by grepping cpp/src/passes/ for
   the constructor that built it (e.g. `IndexedData` for an
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
   shape is only producible end-to-end, fall back to a case in
   `cpp/tests/test_<pass>.cpp` that compiles the small repro with
   `suite::codegen(src, true)` (cpp/tests/compile_case.h).

4. **Fix the pass.** Tighten the eligibility filter (or whatever
   the root cause is). Avoid working around the bug downstream;
   fix it where the bad shape originates.

5. **Re-run the unit test.** Rebuild (`cmake --build cpp/build
   --config Release --parallel`, with Visual Studio's cmake on the
   PATH), run `cpp/build/bin/c6502_tests -tc="<pass>*"`, and check
   that the case count includes the new case. It must now pass.

6. **Write a sim differential test for the example.** Add
   `cpp/tests/test_<example>_sim.cpp` modeled after
   `cpp/tests/test_floor_enemy_advance_sim.cpp`:
   - Inline the example source plus a `main()` that drives the
     function under a battery of scenarios and records observable
     state into a flat `result_log` array.
   - Compute the expected post-call state by hand for each
     scenario (this is the ground truth — the optimized output is
     not).
   - Three tests: unoptimized matches expected, optimized matches
     expected, optimized matches unoptimized byte-for-byte.
   - Use `suite::build_sim(program, optimize)` (cpp/tests/sim_case.h)
     to drive each variant; cap `max_cycles` generously and check
     that the run doesn't time out.

7. **Run the sim differential test.** Both pipelines must produce
   identical observable state. If the optimized variant diverges
   from the unoptimized variant, the optimizer has another bug —
   loop back to step 1.

8. **Re-run the checks** to confirm no regressions:
   `cpp/build/bin/c6502_tests`,
   `ctest --test-dir cpp/build -C Release --output-on-failure`,
   `cpp/build/bin/c6502cpp check-golden -j 12` (and `--update`, after
   reading each changed job, when the output changed on purpose),
   and `cpp/build/bin/c6502cpp check-frozen`. Then re-run the
   original `c6502cpp compile` command from "Command" above; it
   should now succeed. golden/O.txt has a job for each example, so a
   new example needs one added, modelled on its neighbours, and
   `check-golden -j 12 --update`.

9. Proceed to "On success".

## On success

- Report the output path, its size, and whether it differs from
  `golden/O/examples/<base>.s`.
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

- Do NOT commit. Leave any new test, any pass fix, and any golden
  files that `check-golden --update` rewrote in the working tree for
  the user to review.
