# examples/CLAUDE.md

Real-world C sources used to exercise the compiler end-to-end. Each
`<name>.c` has a matching `<name>.asm` checked in — the asm file is
the expected output of

```sh
uv run python compile.py examples/<name>.c --codegen --optimize --unroll \
    -o examples/<name>.asm
```

The matched `.c` / `.asm` pairs serve two purposes:

1. **Snapshot tests** — `tests/test_example_outputs.py` compiles each
   `.c` and diffs the result against the checked-in `.asm`, so any
   codegen drift is caught at test time.
2. **Hand-readable reference** — the `.asm` files are short enough
   (typically <500 lines) that a reader can read them end-to-end and
   see what `--optimize --unroll` produces on representative game
   code.

The example corpus is biased toward Apple II / game-engine workloads:
HUD strip painting, sprite blitting, audio tone generation, enemy AI
advancement, hit-entity refresh. Many use `__attribute__((zp_abi))`
on the leaf helpers to exercise the call-graph-disjoint ZP allocator.

## Adding a new example

The `compile-example` skill (project-local) wraps the canonical
compile command. Type `/compile-example <name>` to compile
`examples/<name>.c` with `--optimize --unroll` and overwrite
`examples/<name>.asm`. Run the test snapshot afterward
(`uv run python -m unittest tests.test_example_outputs`) to confirm
the new pair is wired up.
