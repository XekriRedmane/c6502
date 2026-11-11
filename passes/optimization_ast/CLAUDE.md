# passes/optimization_ast/CLAUDE.md

AST-level optimization. The only consumer today is `unroll_program`,
which runs as part of the `--optimize` pipeline; the directory exists
so the same shape (`passes/optimization_*/`) generalizes when more
AST-level optimizations land.

## Module roster

- `unroll.py` — `unroll_program`. Active under `--optimize`.

## `unroll_program`

Runs after parsing and before identifier resolution. Every for-loop
carrying `#pragma c6502 loop unroll(enable)` (with the canonical
`init=const; cond=var<const; step=var=var+const` shape and a compile-
time-known iteration count) is fully unrolled in place. The trip
count follows C. The induction variable wraps at its own width and
signedness, and the condition compares in the usual-arithmetic-
conversion type. A marked loop that never ends (the variable repeats
a value, as `uint8_t i <= 255` does) or doesn't match the shape is a
one-line `UnrollError` compile error.

Unrolling before name resolution means each unrolled body gets fresh
per-iteration identifier renames; unrolling-then-typechecking
sidesteps having to fix up SymbolTable entries.
