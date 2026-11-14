# examples/CLAUDE.md

Real-world C sources used to exercise the compiler end-to-end. Each
`<name>.c` has a matching `<name>.asm` checked in — the asm file is
the expected output of

```sh
uv run python compile.py examples/<name>.c --codegen --optimize \
    -o examples/<name>.asm
```

The matched `.c` / `.asm` pairs serve two purposes:

1. **Snapshot tests** — `tests/test_example_outputs.py` compiles each
   `.c` and diffs the result against the checked-in `.asm`, so any
   codegen drift is caught at test time.
2. **Hand-readable reference** — the `.asm` files are short enough
   (typically <500 lines) that a reader can read them end-to-end and
   see what `--optimize` produces on representative game code
   (loop unrolling for `#pragma c6502 loop unroll(enable)` regions
   is folded into `--optimize` — there is no separate `--unroll`
   flag).

The example corpus is biased toward Apple II / game-engine workloads:
HUD strip painting, sprite blitting, audio tone generation, enemy AI
advancement, hit-entity refresh. Many use `[[c6502::zp_abi]]`
on the leaf helpers to exercise the call-graph-disjoint ZP allocator.

Examples translated from the Zork I (r15) interpreter, as
reverse-engineered in https://github.com/XekriRedmane/zorki-r15-reveng,
are named `zork_<routine>`. Each one quotes the original 6502 code in
its header comment. `zork_dump_buffer.c` calls the Monitor's `COUT`
and `COUT1` through `<apple2/monitor_io.h>`, and
`tests/test_zork_dump_buffer_sim.py` runs it against stand-ins at the
ROM's addresses.

`zork/instr_save/` is a multi-unit example: Zork's `save` instruction
and the routines it reaches, in three C files and a shared header,
linked by a linker script into the addresses the original occupies.
Its README describes it and compares it with Zork's code, and
`tests/test_zork_instr_save.py` checks its outputs (each unit's and the
linked program's, which `tests/test_example_outputs.py` doesn't cover)
and runs it, and Zork's own code when `ZORK_MAIN_BIN` is set, against
stand-ins for DOS, the Monitor, and the rest of Zork.

`zork/print_zstring/` is another: Zork's z-string printer and
print_string_literal, with the page cache, the line buffer, and the
disk routines they reach, in four C files and a shared header. Its
linker script puts each file at the address of the original routines,
and print_string_literal, in a section of its own, among Zork's
instruction handlers. Its README compares it with Zork's code, and
`tests/test_zork_print_zstring.py` checks its outputs and runs it on
a small story file, with a transcript, and with a failing disk read,
comparing it with Zork's own code when `ZORK_MAIN_BIN` is set.

## Translating Zork's code

Zork's program counters are 24-bit byte addresses, and the multi-unit
examples declare them as `uint24_t`, a `struct { uint16_t lo; uint8_t
hi; }` in each `zork.h`, rather than as the separate bytes
`output/main.asm` names. Keep that in mind when translating more of
Zork, and declare these the same way in a new example, so that units
agree and code that reads them stays plain C:

- `Z_PC` (`$8A`) keeps Zork's byte order, low, high, top, so its
  addresses are Zork's. `Z_PC` / `Z_PC+1` are the low and high bytes
  of `Z_PC.lo`, and `Z_PC+2` is `Z_PC.hi`. A save file holds these
  three bytes (`instr_save`, `instr_restore`), unchanged.
- `Z_PC2` (`$91`) is in a different order from Zork's. Zork keeps it
  high byte first: `Z_PC2_H` at `$91`, `Z_PC2_HH` at `$92`, and
  `Z_PC2_L` at `$93`. The examples keep it low byte first like
  `Z_PC`: low at `$91`, high at `$92`, top at `$93`. So:
  - `Z_PC2_L` is `(uint8_t)Z_PC2.lo`,
  - `Z_PC2_H` is `(uint8_t)(Z_PC2.lo >> 8)`,
  - `Z_PC2_HH` is `Z_PC2.hi`,
  - the story page, which Zork copies out as the word at `Z_PC2_H`
    (`MOVW Z_PC2_H, SCRATCH2`), is `(Z_PC2.lo >> 8) | ((uint16_t)
    Z_PC2.hi << 8)`, and
  - the three `MOVB`s between `Z_PC` and `Z_PC2` are one assignment,
    `Z_PC2 = Z_PC;`.

  Zork's other code that touches `Z_PC2` (`load_address` at line
  2428, which `print_addr`, `print_paddr`, and the object printer go
  through) does so by name, a byte at a time, so it works with this
  order once its three `EQU`s are changed, but Zork's binary as it is
  doesn't: its `load_address` would write the bytes where the C
  doesn't look for them. `Z_PC2` isn't in a save file.
  `tests/test_zork_print_zstring.py` compares `Z_PC2` with Zork's as a
  number, reading each program's bytes in its own order.
- `TMP_Z_PC` (`$F0`) is copied from `Z_PC` in the same order (`MOVW
  Z_PC, TMP_Z_PC; MOVB Z_PC+2, TMP_Z_PC+2`), so it would be a
  `uint24_t` with Zork's addresses. The examples only reserve it.

Zork's `INY; STY Z_PC2_L; BEQ` to step the low byte and carry into
the rest is `Z_PC2.lo++; if ((uint8_t)Z_PC2.lo == 0) { ...; if
(Z_PC2.lo == 0) Z_PC2.hi++; }`, which compiles to `INC Z_PC2; BNE`
(see docs/optimizations/in_place_member_update.md and
inc_low_byte_test.md).

## Adding a new example

The `compile-example` skill (project-local) wraps the canonical
compile command. Type `/compile-example <name>` to compile
`examples/<name>.c` with `--optimize` and overwrite
`examples/<name>.asm`. Run the test snapshot afterward
(`uv run python -m unittest tests.test_example_outputs`) to confirm
the new pair is wired up.
