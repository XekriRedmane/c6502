# docs/CLAUDE.md

Design notes, walkthroughs, and reference material for c6502. These
are written for humans (and Claude) reading about the compiler — not
parsed by any tool.

## Long-form docs

- `architecture.md` — how the compiler is put together, for people
  working on it and for Claude: the pipeline as a Mermaid diagram
  with a table of stages and modules, the four IRs and the asm
  operands, the names each stage makes up, the lowerings to
  recognize in the output, the zero-page map, return values, each
  runtime helper's `HARGS` layout, the output format, and the link
  steps as a diagram. The root README links it from its build
  section, and the root CLAUDE.md as the summary of what the code
  does.
- `attributes.md` — user guide to c6502's C23 attributes
  (`[[c6502::zp_abi]]`, `return_bool_in_carry`, `zeropage`, `address`,
  `section`, ...), where they go, the standard attributes it accepts,
  the statement attributes (`unroll` and `keep` before a `for`, and
  `save_hardware_stack`, which replaced the `#pragma c6502`
  directives, with the cautions about the hardware stack's depth that
  come with the last),
  and to declaring and calling ROM routines, with Apple II Monitor
  examples, the errors the compiler reports, and current limits.
- `builtins.md` — user guide to the builtins, `__c6502_bcd_add` and
  `__c6502_bcd_sub`: what packed BCD is, the operand and result
  types, wrap-around and borrow, what digits above 9 produce (py65's
  model of the NMOS 6502), constant folding, the code they compile
  to, the guarantee that the decimal flag is set only across the
  chain, the caution about interrupt handlers on an NMOS 6502, a
  score as the example, the errors, and the limits. Then
  `__c6502_bin_to_bcd`: its types, the runtime helper `bin16_to_bcd`
  (the doubling algorithm, its `HARGS` layout, bytes, and cycles),
  constant folding, printing a number as the example, and the
  errors.
- `stdbit.md` — user guide to `<stdbit.h>`: the forms of one type
  and the type-generic forms, the types they take and return, each
  function's result with the values for 0 and all ones, constant
  folding, the loops they compile to with a listing and a table of
  sizes, the functions a program reaches by address (defined in each
  unit, with internal linkage), the header's other names, the errors,
  and the limits.
- `linker_scripts.md` — user guide to linking with `compile.py -T`:
  sections (the defaults and `[[c6502::section("name")]]`), the GNU
  ld script subset, the runtime library and where its helpers go,
  the section symbols, the startup copy and clear, and how the
  linker computes the layout and writes the executable.
- `cpp_port.md` — the port of the compiler to C++, decided on 19 Nov
  2026: the decisions (stage by stage with the frozen Python compiler
  as the byte-identical reference, the scope, which since 21 Nov 2026
  takes in the simulator, the tests, and the fuzzers, the hand-written
  parser, the build), how each stage is checked, the order of the
  stages, the surveys and groups of steps 4 to 7, and their status.
- `calling_conventions.md` — user guide to c6502's two calling
  conventions. The soft stack: SSP and FP, a call traced through the
  caller's argument push, the prologue, the frame layout, and the
  epilogue. zp_abi: per-function zero-page parameter and local bytes,
  why call-graph siblings may share them, `register` parameters,
  which functions qualify, mixing the two, and linking. Also return
  values, costs, and limits.
- `optimizations/` — one document per optimization, each with what
  it does, a before-and-after example captured from a compile, when it
  applies, and what it leaves alone. `optimizations/README.md` opens
  with how `-O` is organized (SSA at two levels, diagrams of the
  stages and the two asm optimizer runs, one value followed from a C
  variable to a zero-page byte, and how the fixed points repeat), then
  indexes them alphabetically and in pipeline order, and
  cpp/tests/test_optimization_docs.cpp fails when that order no longer
  matches the order `c6502cpp pass-order` gives for the optimizer's
  drivers, or when a document is missing from the index. A new pass
  needs a document, both index entries, and its module in the test's
  `kModules`.
- `leaf_zp_abi.md` — design for `[[c6502::zp_abi]]` and the
  call-graph-disjoint ZP allocator: leaf functions with bare-RTS
  prologue / epilogue when arg bytes fit in a private ZP slice.
- `sim_findings.md` — bugs and gaps found by running the 6502
  simulator across the chapter corpus. Each one is cross-referenced
  from `sim_skips()` in cpp/tests/sim_chapters.cpp.

## Reference grammars

`docs/*_grammar.txt` are reference documentation for the spec
grammars that the C parser implements. The parser's LALR tables are
in cpp/generated/c99_lalr.cpp, which is now the grammar's source, and
its descent is in cpp/src/parser*.cpp. No tool parses the grammar
files. They exist so the parser's choices (operator precedence,
dangling-else resolution, cast-vs-paren disambiguation) can be
cross-checked against the spec without leaving the repo.

- `chars_grammar.txt` — C99 §6.4.4.4 character-constant grammar.
- `floats_grammar.txt` — C99 §6.4.4.2 floating-constant grammar,
  with C23's digit separators.
- `ints_grammar.txt` — C99 §6.4.4.1 integer-constant grammar, with
  C23's binary constants, digit separators, and `wb` suffix.
- `strings_grammar.txt` — C99 §6.4.5 string-literal grammar.

## Drafts

`drafts/` holds in-progress blog-post material and session logs.
Branched into `drafts/log/` (per-session retrospectives, lessons
learned) and `drafts/r6502/` (article-style writeups for an outside
audience).
