# cpp/src/rtlib/CLAUDE.md

The runtime library: the 6502 helpers that compiled code calls for
operations it doesn't do inline, such as `udivmod16` for a 16-bit
division by a variable, or a multiply of two 16-bit values. Every
linked program takes them from here, and the simulator runs the same
code.

- `helpers.h` / `helpers.cpp` — the helpers as `asm_ast::Function`s,
  built by functions that share builders such as `negate_n_inplace`
  and `copy_n_via_a` (`all_helper_functions`, in the library's order).
  Each reads its operands at the low offsets of `HARGS` and leaves its
  results at higher offsets, as `tac_to_asm` passes them; the comment
  on each builder and docs/architecture.md give each layout. The
  linker emits the ones a program calls with `asm_emit`, assembles
  them into an object named `runtime.o`, and links that in
  (cpp/src/passes/runtime_library.h), so a program links with no
  `--defsym`. `passes::helper_footprints` (cpp/src/passes/zp_globals.h)
  measures each one's HARGS footprint on them, counting the helpers it
  calls, and `check_acyclic`, which `all_helper_functions` runs, refuses
  helpers that call each other in a cycle, where that count wouldn't
  end.
- `bin16_to_bcd`, the helper behind `__c6502_bin_to_bcd`, converts a
  16-bit value to five packed BCD digits by doubling in decimal mode.
  It is the one helper that sets the decimal flag (`SED` and `CLD`,
  the asm atoms `SetDecimal` and `ClearDecimal`, which only helpers
  write), so it's named in `decimal_mode_helpers`, and the IR checks
  (cpp/src/passes/ir_check.h) check that it clears the flag before
  its `RTS` in place of the rule for compiled code.

The helpers were Python, in rtlib/helpers.py, until step 9 of the port
(docs/cpp_port.md). cpp/tests/frozen/rtlib_helpers.txt is their dump
from then, and test_rtlib_helpers.cpp checks that the builders give it
and that the footprints are Python's, so a change to a helper fails
that test until the capture is written again with `c6502cpp
helper-dump`, after the change has been checked in the simulator.

The simulator's runtime (cpp/src/sim/runtime.h) assembles every
helper at `$E000` with `sim::assemble`, lays `icall` (`JMP (DPTR)`)
after them, and runs a helper with no 6502 code (the 64-bit and
floating-point ones) as a hook at a trap address after that. Its
Python copy, sim/runtime.py, reads the helpers through
`cpp_backend.helper_program` (`c6502cpp helper-dump`) for the games and
the fuzzers' runner. cpp/tests/test_runtime_helpers_sim.cpp tests the
helpers through the harness, cpp/tests/test_runtime_library_sim.cpp
through linked C, and cpp/tests/test_bin_to_bcd_sim.cpp runs
`bin16_to_bcd` on the C++ CPU for all 65536 inputs, with its cycles.

`docs/linker_scripts.md` says where a linker script puts the helpers.
