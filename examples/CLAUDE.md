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
advancement, hit-entity refresh. Many use `__attribute__((zp_abi))`
on the leaf helpers to exercise the call-graph-disjoint ZP allocator.

Examples translated from the Zork I (r15) interpreter, as
reverse-engineered in https://github.com/XekriRedmane/zorki-r15-reveng,
are named `zork_<routine>`. Each one quotes the original 6502 code in
its header comment. `zork_dump_buffer.c` calls the Monitor's `COUT`
and `COUT1` through `<apple2/monitor_io.h>`, and
`tests/test_zork_dump_buffer_sim.py` runs it against stand-ins at the
ROM's addresses. `zork_print_string_literal.c` is print_string_literal
and the 18 routines it reaches. It's too long to quote them, so its
header gives their line ranges in Zork's `main.asm` instead.
`tests/test_zork_print_string_literal_sim.py` runs it on a small story
file and, when the environment variable `ZORK_MAIN_BIN` names Zork's
`main.bin`, compares its output and final state with Zork's own code.

`zork/instr_save/` is a multi-unit example: Zork's `save` instruction
and the routines it reaches, in three C files and a shared header,
linked by a linker script into the addresses the original occupies.
Its README describes it and compares it with Zork's code, and
`tests/test_zork_instr_save.py` checks its outputs (each unit's and the
linked program's, which `tests/test_example_outputs.py` doesn't cover)
and runs it, and Zork's own code when `ZORK_MAIN_BIN` is set, against
stand-ins for DOS, the Monitor, and the rest of Zork.

## Adding a new example

The `compile-example` skill (project-local) wraps the canonical
compile command. Type `/compile-example <name>` to compile
`examples/<name>.c` with `--optimize` and overwrite
`examples/<name>.asm`. Run the test snapshot afterward
(`uv run python -m unittest tests.test_example_outputs`) to confirm
the new pair is wired up.
