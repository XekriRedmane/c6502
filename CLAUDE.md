# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when
working with code in this repository.

## Project overview

c6502 is a C99 compiler written in Python that targets the MOS 6502.
Dependencies are managed with `uv`; `pyproject.toml` is the source of
truth and `uv.lock` the resolved set. `requirements.txt` is a hand-
maintained `pip`-compatible fallback and may lag.

[docs/architecture.md](docs/architecture.md) summarizes what the code
does: the pipeline as a diagram, the four IRs and their operands, the
names each stage makes up, the lowerings to recognize in the output,
the zero-page map, the helpers' `HARGS` layouts, and the link steps.
Read it first for orientation. This file and the subdirectories'
CLAUDE.md files have the per-module detail.

## Subdirectories

Each subdirectory has its own CLAUDE.md with the details for the
files it contains. Read them as you navigate into them; this root
file holds the project overview and notes about root-level files
(parser, lexer, codegen, ASDL definitions, compile.py CLI).

- [passes/CLAUDE.md](passes/CLAUDE.md) — middle-end passes
  (identifier / label / loop / type-checking; replace_pseudoregisters;
  asm_to_asm2; peephole catalog; call-graph-disjoint ZP allocation).
- [passes/optimization/CLAUDE.md](passes/optimization/CLAUDE.md) —
  TAC-level fixed-point optimizer (SSA, constant folding, copy
  folding, IndexedStore recognizer, dead-loop elimination, …).
- [passes/optimization_asm/CLAUDE.md](passes/optimization_asm/CLAUDE.md)
  — asm-level SSA round-trip with byte-granular regalloc, move
  coalescing, const-static fold.
- [passes/optimization_ast/CLAUDE.md](passes/optimization_ast/CLAUDE.md)
  — AST-level unroller (`[[c6502::unroll]]`).
- [tests/CLAUDE.md](tests/CLAUDE.md) — test organization, chapter
  harnesses, where to look for status.
- [binutils/CLAUDE.md](binutils/CLAUDE.md) — the object-file tools:
  ELF objects and executables, the assembler `compile.py -c` runs,
  relocation and relaxation, and the disassembler behind `nm.py` and
  `objdump.py`.
- [rtlib/CLAUDE.md](rtlib/CLAUDE.md) — the runtime library, the
  6502 helpers that the linker adds to a program that calls them and
  that the simulator runs.
- [sim/CLAUDE.md](sim/CLAUDE.md) — in-process 6502 simulator
  (assembler, runtime stub, Python-implemented helpers, and the
  loader for linked executables).
- [docs/CLAUDE.md](docs/CLAUDE.md) — design notes, walkthroughs,
  reference grammars, drafts.
- [examples/CLAUDE.md](examples/CLAUDE.md) — real-world C sources
  with checked-in expected `.asm` outputs.
- [include/CLAUDE.md](include/CLAUDE.md) — c6502 flavors of
  `<stdint.h>` and `<limits.h>`.
- [fuzz/CLAUDE.md](fuzz/CLAUDE.md) — random-program fuzzers
  (expression semantics, calling conventions, statements and storage
  classes, and YARPGen's programs, built with a 16-bit `int` by
  `fuzz/build_yarpgen.py`), a runner that checks every execution path
  with the IR checks on, and a reducer for failing programs.
- [cpp/README.md](cpp/README.md) — the C++ port
  ([docs/cpp_port.md](docs/cpp_port.md) has the plan): its build, the
  IR classes `asdl.py --cpp` generates, the dump format's C++ side, the
  check of each ported pass against the calls `tools/capture_passes.py`
  records, and the conventions for porting a pass.

## Pipeline at a glance

```
C source
  → preprocess (pcpp)
  → lex + parse (lark, LALR)            → c99_ast
  → identifier_resolution
  → string_lifting
  → label_resolution
  → loop_labeling
  → type_checking                       → c99_ast (+ SymbolTable, TypeTable)
  → (-O: unroll_program — [[c6502::unroll]] for loops)
  → c99_to_tac                          → tac_ast
  → (-O: inline_functions — calls of small static functions
     replaced by copies of their bodies)
  → (-O: return_pointers_in_zero_page — a static's pointer result in F@ret)
  → (-O: apply_row_address_tables — 2-D rows from a table of row addresses)
  → (-O: TAC fixed-point opts)
  → (-O: dispatch_const_pointer_arrays, apply_split_tables — tables of
     2-byte elements as low bytes, then high bytes)
  → (-O: select_abi + allocate_zp_slots)
  → tac_to_asm                          → asm_ast
  → (-O: asm-SSA round-trip + byte-granular regalloc)
  → replace_pseudoregisters             (Pseudo → Frame / ZP / Data)
  → (-O: synthesize_prologue, loop_counter_to_x, peephole fixedpoint)
  → expand_long_branches
  → asm_to_asm2                         → asm2_ast (1:1 with 6502 opcodes)
  → asm_emit                            → dasm-syntax 6502 assembly text
  → (-c: binutils.assembler)            → relocatable ELF object
  → (link: passes.linker)               → ELF executable
```

Four ASDL-defined IRs (`c99.asdl`, `tac.asdl`, `asm.asdl`,
`asm2.asdl`) shape the data passed between passes. `tac_ast` carries
width AND signedness on integer types; `asm_ast` is byte-typed (one
IR atom per 6502 opcode, with `Pseudo` operands that
`replace_pseudoregisters` resolves); `asm2_ast` is `asm_ast` with the
three compound nodes (`FunctionPrologue` / `AllocateStack` / `Ret`)
already expanded into atoms.

Two key runtime conventions:

- **Soft data stack** at `SSP`/`FP` (zero-page pointers, both grow
  downward); the 6502 hardware stack at `$0100-$01FF` is reserved for
  JSR return addresses and short-lived PHA/PLA. Reserved ZP in the
  classic layout: `$00-$01` SSP, `$02-$03` FP, `$04-$23` HARGS
  (runtime-helper exchange block), `$24-$25` DPTR (caller-saved
  indirect-pointer scratch). The output binds all four with `EQU`
  lines. A fixed zero-page object in `$00-$25` (`address(addr)` or
  `zeropage(addr)`, e.g. from `#include <apple2/monitor_io.h>`) moves
  the block below `$80`, with HARGS sized to the program
  (`passes/runtime_layout.py`, whose `current()` the asm passes read
  for runtime addresses). Full layout in "Function stack frame"
  below; user guide in
  [docs/calling_conventions.md](docs/calling_conventions.md).
  The bytes left below `$80` are for user globals marked
  `[[c6502::zeropage]]`, and under `-O` for busy scalar
  globals the compiler promotes there itself; `zeropage(addr)` may also pin one in
  `$80-$FF`, which removes those bytes from the compiler's pools. Their initial values are stored by a generated `__zp_init`,
  which the boot code calls before `main` (see `passes/zp_globals.py`).
  `[[c6502::address(addr)]]` binds a function (a ROM routine)
  or an `extern` object (an I/O register) to a fixed address. The
  name only gets an `EQU`, and it can't be defined. Below `$100` the
  object is an `extern` zero-page object. Such a function's calling
  convention comes from its declaration alone in both pipelines
  (`abi_selection.fixed_address_layouts`). `preserves_x` /
  `preserves_y` on such a routine put a `preserves` list on each asm
  `Call`, and `asm_liveness.call_preserves` lets the passes keep a
  value in X or Y across it. User guide:
  [docs/attributes.md](docs/attributes.md).
- **Builtins** (`builtin_functions.py`). `__c6502_bcd_add(a, b)` and
  `__c6502_bcd_sub(a, b)` do packed BCD arithmetic. The parser and
  identifier resolution leave a call of one as a c99 `FunctionCall`
  with its own name (`is_builtin`), and reject a declaration of the
  name or a use of it as a value.
  `type_checking._check_bcd_call` types it (unsigned operands, the
  result the higher-rank operand type, no integer promotion), and
  `c99_to_tac._translate_builtin` lowers it to TAC
  `Binary(BcdAdd | BcdSubtract)`, folding constant operands there and
  in `constant_folding._fold_bcd`. `tac_to_asm` lowers that to one
  asm `DecimalArith(subtract, left*, right*, dst*)`, which stays one
  instruction through every asm pass (`passes/decimal_arith.py` gives
  its reads and writes) and which `asm_to_asm2.expand_decimal_arith`
  spells out as `SED; CLC; LDA; ADC; STA; ...; CLD`, with the asm2
  atoms `SetDecimal` / `ClearDecimal`. `ir_check.check_asm2` checks
  that nothing else runs with the decimal flag set. The arithmetic
  (`adc_byte` / `sbc_byte`) copies py65's decimal mode, for digits
  that aren't 0-9 too. `__c6502_bin_to_bcd(x)` converts `x`, as an
  `unsigned int`, to its five decimal digits in packed BCD in an
  `unsigned long` (`type_checking._check_bin_to_bcd_call`, any integer
  argument). A constant argument folds (`builtin_functions.bin_to_bcd`)
  in the type checker, `c99_to_tac`, `constant_expression`, and
  `constant_folding._fold_unary`, and any other call is TAC
  `Unary(BinToBcd)`, which `tac_to_asm` lowers to a call of the
  runtime helper `bin16_to_bcd` (input `HARGS+0..1`, result
  `HARGS+2..4`). That helper doubles a BCD number in decimal mode
  with the asm atoms `SetDecimal` / `ClearDecimal`, which only the
  helpers write (`check_asm` rejects them elsewhere), and
  `check_asm2` checks that it clears the flag before its `RTS`
  (`rtlib.helpers.DECIMAL_MODE_HELPERS`). User guide:
  [docs/builtins.md](docs/builtins.md).
  The other 84 builtins are `<stdbit.h>`'s (C23 §7.18):
  `__c6502_stdc_<op>` for each of the 14 operations in its
  type-generic form and `__c6502_stdc_<op>_uc` / `_us` / `_ui` /
  `_ul` / `_ull` for the forms of one type (`STDBIT_BUILTINS`), which
  `include/stdbit.h` names `stdc_<op>...` with macros.
  `type_checking._check_stdbit_call` types a form with a suffix as a
  call of a function with that parameter, and the type-generic form
  by its argument, which must be an unsigned integer type other than
  `bool`, with no integer promotion. `bit_floor` / `bit_ceil` return
  the value type, `has_single_bit` `bool`, the rest `unsigned int`.
  A constant argument folds (`builtin_functions.stdbit`) in the type
  checker, `c99_to_tac`, and `constant_expression`, so a call is an
  integer constant expression. `c99_to_tac._translate_stdbit` writes
  any other call as a TAC loop that shifts a copy of the value until
  it's zero and counts in a byte. A unit that takes the address of a
  `stdc_<op>_uc` etc., or calls it with the macro suppressed, gets a
  definition of it with internal linkage
  (`passes/stdbit_functions.py`, run at the start of
  `check_program`), and the header's declarations it doesn't refer
  to are dropped. User guide: [docs/stdbit.md](docs/stdbit.md).
- **zp_abi + call-graph-disjoint private pools** put params AND
  body locals in ZP for eligible leaf / non-recursive /
  non-indirect-calling functions; ineligible functions fall back
  to the default caller/callee partition (`$80..$BF` / `$C0..$FF`).
  Eligible functions emit as bare body + RTS. Under `-O`
  every function defaults to zp_abi when eligible — the explicit
  `[[c6502::zp_abi]]` annotation is no longer required but
  remains supported as a strict-mode opt-in: an annotated
  function hard-errors on ineligibility (recursion, address
  taken, etc.) instead of silently falling back. A call to a
  `[[noreturn]]` function without arguments isn't a call-graph
  edge (`passes/call_edges.py`), so a game's `JMP RESET_GAME` from
  deep in its subroutines doesn't make a cycle. Details in
  [passes/CLAUDE.md](passes/CLAUDE.md) under "Call-graph-disjoint
  ZP allocation".
- **`[[c6502::return_bool_in_carry]]`** on a function
  returning `bool` (never `unsigned char` or `char`) makes
  it return `e != 0` in the carry flag. The callee ends in `CMP #1` (`SEC` / `CLC` for a
  constant, `PHP` / `PLP` around a frame teardown), and the caller
  branches with `BCC` / `BCS` or captures C with `LDA #0; ROL A`.
  The `save_c` flag on `Ret` / `Return` marks C live at the return.
  It's the only way a flag value crosses a call. Every declaration
  must carry it, and its address can't be taken. See
  `passes/carry_return.py`. The function may instead return a
  struct of exactly two members, a 1- or 2-byte integer value and
  then a `bool` (`type_checking.carry_struct_value_bytes`). The value
  comes back in A (and X) and the flag in C, with no sret pointer,
  so the function stays zp_abi-eligible. The callee does `LDA flag;
  CMP #1; [LDX hi;] LDA lo`, and the caller stores A (and X) and then
  `LDA #0; ROL A` into the flag byte. Under `-O`, a `static`
  `bool` function whose address isn't taken, and whose every call
  branches on the result or returns it from another such function,
  gets the convention without the annotation, and so does every
  zp_abi `bool` function with external linkage, which every unit
  decides from the declaration alone (`passes/carry_return_choice.py`;
  `FunAttr.returns_bool` records the `bool` before `c99_to_tac` erases
  it). The link metadata marks each such function `carry=true`, and
  the linker rejects units that disagree.
- **`[[c6502::save_hardware_stack]]`** before an expression
  statement, a `return`, or a block-scope declaration marks the calls
  in it (`hw_stack_save` on the c99 and TAC `FunctionCall`, set by the
  parser's `_mark_hw_stack_save`). Each must be a direct call that can
  recurse. Under `-O`, a call-graph cycle whose every
  internal call is marked gets private pools, and each marked call
  saves the caller's live values with `PHA` / `PLA` instead of the
  function saving callee-saved bytes at entry and exit. The user
  vouches for the hardware stack's depth. See
  `passes/recursion_saves.py`; user guide, with the cautions, in
  [docs/attributes.md](docs/attributes.md#save_hardware_stack).
- **Arguments in A and X by default.** Under `-O`, a zp_abi function
  whose declaration names no register (`register`, `in_*`, or
  `zeropage(addr)` on a parameter) takes its leading integer and
  pointer parameters in A and then X, as many whole ones as fit, and
  the rest in slots (`abi_selection._default_param_registers`).
  `main` keeps its slots. Every unit decides it from the declaration,
  and the link metadata's `param_regs` records it.
- **The `register` keyword** requests 6502-register placement,
  refining the zp_abi calling convention. Placement is POSITIONAL:
  the first register arg-byte goes in **A**, the second in **X**
  (so `foo(register uint8_t a, register uint8_t b)` puts `a` in A
  and `b` in X; `foo(register uint16_t a)` spreads `a`'s low byte
  to A and high byte to X). `[[c6502::in_a]]` / `in_x` / `in_y`
  after a parameter names its register instead, and a pair (`in_xy`,
  low byte first) covers a 2-byte one, so Y is available to parameters
  that ask for it; the positional rule skips registers the named ones
  take. Such a parameter is a register parameter with or without
  `register`, and may be any integer or pointer with one byte per
  register (`type_checking._check_named_register_param`), so DOS's
  RWTS is `bool RWTS(struct iob *iob [[c6502::in_ya]])`.
  `param_registers` on the c99 function_decl holds "" / "auto" / the
  letters. A `register` local uses **Y** only — exactly one byte. **A
  positional `register` parameter or local can't be a pointer.**
  Return values are type-driven
  (no keyword): any return of ≤2 bytes, a pointer included, comes
  back low byte in A, high byte in X; wider returns ride HARGS.
  Requires zp_abi eligibility (no IndirectCall, no recursion, address
  not taken, params fit) — a `register` param/local on an ineligible
  function is a hard error. `&x` on a `register` object is a hard
  error per C99 §6.5.3.2.1. Locals are HARD pins (failure to honor →
  compilation error); parameters are SOFT: the callee copies each
  one into a local at entry (`tac_to_asm._register_param_locals`),
  which the regalloc keeps in the register when it can and in the
  function's pool otherwise, so a register parameter has no slot.
  A parameter with `[[c6502::zeropage(addr)]]` instead has its
  slot at that address, on any zp_abi function. A function defined
  here reads it there, or copies it into a local at entry when
  something it calls might store an argument at those bytes
  (`slot_sharing.pinned_param_copies`).
  See `passes/abi_selection.py` (positional assignment, type-driven
  return) + `passes/optimization_asm/{hwreg_eligibility,regalloc}.py`
  (Y-only local pins, `optimizer._entry_register_limits`) for the
  implementation. A 2-byte register return from a *soft-stack*
  function is preserved across the teardown (the `save_x` flag on the
  asm `Ret`/`Return` atoms); `loop_counter_to_x` refuses to promote a
  counter to X across a `Call` that returns a live value in X (the
  high byte would race the counter restore).

## Common commands

```sh
uv sync                                         # create/update the project venv
uv run python -m unittest                       # run all tests
C6502_CHECK_IR=1 uv run python -m unittest      # with IR checks after every pass
C6502_CHECK_IR=1 uv run pytest -n auto --dist worksteal -q   # the same, in parallel
uv run python -m unittest tests.test_asm_emit   # run one module
uv run python -m unittest tests.test_chapter_1.TestChapter1Valid    # run one test

uv run python compile.py <source.c> -S          # C → 6502 asm in <source>.s
uv run python compile.py <source.c> -S -o out.asm   # to a file (.s or .asm)
uv run python compile.py <source.c> -S -o -     # to stdout
uv run python compile.py - --tac < source.c     # read stdin, stop after TAC
uv run python compile.py - -S -O < src.c        # with the optimizer pipeline
uv run python compile.py -O -c a.c b.c          # objects a.o and b.o
uv run python compile.py a.o b.o -o prog        # link them (ELF executable)
uv run python nm.py a.o                         # list an object's symbols
uv run python objdump.py -d a.o                 # disassemble (dasm syntax)

# The C++ port (cpp/README.md). On Linux, leave out -G, -A, and
# --config, and add -DCMAKE_BUILD_TYPE=Release to the first command.
cmake -S cpp -B cpp/build -G "Visual Studio 17 2022" -A x64
cmake --build cpp/build --config Release --parallel
ctest --test-dir cpp/build -C Release --output-on-failure
uv run python tools/capture_passes.py --passes split_mem_to_mem -j 16
uv run python tools/check_captures.py split_mem_to_mem   # C++ vs the calls
C6502_CPP_BACKEND=1 uv run python compile.py <source.c> -S -O   # all in C++
uv run python tools/compare_backends.py -j 12   # both back ends on the corpus
cpp/build/bin/c6502cpp check-golden -j 12      # C++ against golden/, no Python
cpp/build/bin/c6502cpp check-frozen            # C++ against cpp/tests/frozen
uv run python tools/write_golden.py -j 12      # golden/ from the Python compiler
```

With `C6502_CPP_BACKEND=1`, the C++ port compiles each C input of
`-S`, `-c`, and a link from its source text to the `-S` text, and Python
runs no part of the front end, and the C++ port assembles `-c`'s
objects and links, so no Python assembles or links either. For `-S`,
`compile._compile_c` hands one call of
`c6502cpp backend` the text as compile.py read it, the preprocessor's
arguments, the file name (None for stdin), and the directory of
include/, and the C++ side runs `passes::front_end`
(cpp/src/passes/front_end.h: `preprocess`, `parse`, under `-O`
`unroll_program`, `identifier_resolution`, `string_lifting`,
`label_resolution`, `loop_labeling`, and `type_checking`) and then
`passes::codegen` (cpp/src/passes/codegen_stage.h):
`translate_for_codegen` (`c99_to_tac`, and under `-O` the inliner and
`return_in_zero_page`), under `-O` the TAC passes of `_optimized_tac`
(the tables of row addresses, the TAC optimizer, the dispatch of
const-pointer arrays, and the split tables), `plan_zero_page` on the TAC
from before those passes, the rest of `_optimized_front` (`select_abi`
through `synthesize_prologue`, with the pool retries) or, without `-O`,
`fixed_address_layouts`, `tac_to_asm`, and `replace_pseudoregisters`,
then the back end's region, the work after it, the link metadata, and
`asm_emit`. The runtime helpers' HARGS footprints come from the copy
of rtlib/helpers.py compiled into c6502cpp (cpp/generated/rtlib_helpers.cpp,
written by tools/rtlib_table.py). The request also holds a flag that is
false when `compile.inline_functions` has been patched
(tests/no_inlining.py), so that such a test's `static` functions stay
functions in C++ too (`compile._inlining`). The response holds what the
preprocessor wrote to stderr, which `cpp_backend.py` writes to
`sys.stderr`, and its `_Bool` warnings, which it issues again through
`diagnostics.warn`, so `_print_warnings` prints them as before, and a
problem with the program comes back as the Python exception of its
class and message (`cpp_backend._exception`, a subclass of the same name
for Lark's syntax errors), so `main` reports it as Python's front end
does. `_run_stage("codegen", text)`, which tests call with the
preprocessor's output, sends the text with `preprocess` off. Python
reads no program back. Under `C6502_CHECK_IR=1` (or after
`ir_check.enable()`), the request's `check_ir` turns on the C++ port of
`ir_check` (cpp/src/passes/ir_check.h), which checks the IR at every
place the Python pipeline checks it, and a failed check comes back as
an AssertionError with `IRCheckError`'s message. The simulator harness
(`sim.harness.compile_to_asm`) sends the source text with no
preprocessor arguments and takes back the finished asm program in the
dump format, with its slot symbols, since it assembles the program
itself, and it returns those two alone. `--lex` goes to C++ too, in the
backend's mode `lex` (`cpp_backend.lex_source`), which preprocesses the
source and prints its tokens, and `_run_stage("lex", text)` sends the
text with `preprocess` off. `--parse`, `--resolve`, and `--tac` go to
C++ in the modes `parse`, `resolve`, and `tac` (`cpp_backend.stage_source`,
src/passes/text_stages.h), which run the front end to where
`_run_stage` stops and print the tree with the `pretty` that `asdl.py
--cpp` generates for each IR, the text of pretty.py. Run as a script with the variable set,
compile.py stops before its pipeline's imports for every stage
(`_cpp_runs_everything`), so no stage imports the parser,
Lark, pcpp, or the modules of passes/, with `C6502_CHECK_IR=1` too.

`-c` and a link go to C++ in one request for the whole run
(`compile._cpp_build`, `cpp_backend.build`, and the modes `objects` and
`link` of cpp/src/backend.h). Python reads the inputs as `_object`
does, each C source's text, each `.asm` file's text, and each object's
bytes, up to the first it can't read (or, for `-c`, the first object),
and sends them in order with the preprocessor's arguments, and for a
link with the script's path, the origin, the `--defsym`s, and the
output's path. The C++ side compiles and assembles each C input,
assembles each `.asm` file, and reads each object, in order and in the
same process, stopping at the first error, then returns each object of
`-c`, or links the objects and writes the executable itself. Python goes
through the response input by input as its own path does: it writes
what each C input's preprocessor wrote to stderr and prints its
warnings, raises the input's error as its Python exception
(`AssemblerError`, `ElfError`, and the front end's classes) for `main`
to report with `_report`, writes each object, and only then reports the
input it stopped at. A link's error keeps its class (`LinkError`,
`ZpSlotAllocationError`, `ZpLocalAllocationError`; a `ScriptError` and
a `LayoutError` reach compile.py as `LinkError`s, as in Python), and
compile.py reports it as `compile.py: link error: <message>`. The
`struct.error` that `elf.write` raises for an addend or a size outside
its field comes back as `struct.error` (which `cpp_backend._error_type`
tells apart from `re.error`), and compile.py opens the output file
before raising it, as Python does. An executable that can't be written
gives the OSError of its errno. On these paths compile.py, run as a
script, imports none of binutils/, `passes.linker` and its imports, the
type checker, numpy, or rtlib (tests/test_cpp_backend.py,
`TestObjectsAndLinks`).

The variable is read at each compile, and without
it compile.py imports nothing new. `tools/compare_backends.py` compiles
the whole corpus both ways with `-S`, `-S -O`, `-c`, and `-c -O`, links
each object alone with and without a script, links the three games as
tools/capture_binutils.py does and from their sources, and links
damaged objects and bad command lines, and requires the same output,
errors, warnings, and files (cpp/README.md, "The back end through
c6502cpp" and "Emission"). With the variable set, nm.py's and
objdump.py's `main` run the C++ tools, `c6502cpp nm` and `c6502cpp
objdump`, with their command lines (`cpp_backend.run_tool`), and write
their output to Python's streams.

`compile.py`, `nm.py`, and `objdump.py` are the CLIs; every other
module is library-only. Flags `compile.py` doesn't recognize are
forwarded to the preprocessor (pcpp), so `-D`, `-U`, `-I`,
`--passthru-*`, `--line-directive` etc. work the same as the `pcpp`
CLI. pcpp's own `-o` is not forwarded.

Stage-selection flags (mutually exclusive, at most one):
`--lex`, `--parse`, `--resolve`, `--tac`, `-S` (assembly), `-c`
(a relocatable ELF object per input, `<stem>.o`). With none, the
inputs (C, `.asm`, or `.o`) are linked into an ELF executable
(`a.out` unless `-o`). `--resolve` runs the three name-resolution
passes (identifier resolution, label resolution, loop labeling) in
that order.

Modifier flag (orthogonal to the stage flags; applies to `--tac`,
`-S`, `-c`, and C inputs to a link):

- `-O` runs the optimizer pipeline:

  0. AST-level unroll (`passes.optimization_ast.unroll.unroll_
     program`) — fully unrolls every for-loop marked
     `[[c6502::unroll]]`. Runs after parsing and
     before identifier resolution. See
     [passes/optimization_ast/CLAUDE.md](passes/optimization_ast/CLAUDE.md).
  0a. Inlining (`passes.optimization.inline_functions`), in
     `compile.translate_for_codegen` right after `c99_to_tac`. The
     calls of a `static` function whose address isn't taken, that
     isn't recursive, and that has no attribute setting its
     convention or placement are replaced by renamed copies of its
     body when a byte estimate says the copies take no more than the
     calls and the function (`_Inliner._pays`, with constants fitted
     to the examples), and a function with no call left is dropped.
     See [docs/optimizations/inline_functions.md](docs/optimizations/inline_functions.md).
  0b. Row-address tables (`passes.optimization.row_address_tables`) —
     `A[i]` on a static 2-D array whose rows aren't a power of two
     bytes long reads the row's address from a table of row addresses
     split into low and high bytes, when the unit saves bytes by it.
     After the TAC optimizer, `apply_split_tables` stores a `static
     const` table of 2-byte elements read by a byte index the same
     way.
  1. TAC SSA construction (`passes.optimization.ssa_construction`).
  2. One-shot scalar const-static read fold — replaces `Var(static
     const scalar)` USE positions with `Constant(value)`.
  3. TAC fixed-point loop: constant folding (incl. const-array-
     subscript fold), `(x - c) * k` on a widened byte as `x * k - c *
     k` (`distribute_constant`), strength reduction, comparison-against-zero /
     jump fold, AND-zero-jump fold, jump threading (an edge that
     brings a tested join a constant goes where the test sends it,
     `jump_thread.py`), unreachable-code elimination,
     common-subexpression elimination, loop-invariant extends,
     copy propagation, dead-store elimination, copy folding, Add-
     with-Constant and offset-constant reassociation, loop rotation,
     sink-increment, IndexedLoad / IndexedStore / IndirectIndexed
     recognizers, and the narrowing of promoted bytes' arithmetic
     (`narrow_widened_arith`, `narrow_byte_range` for `/`, `%`, `>>`,
     and a 16-bit `*` of operands `value_range` bounds by 255,
     `narrow_byte_bitwise`). After it, a join of `p + k1` and `p + k2`
     read through once becomes a choice of `k1` or `k2` added to `p`
     (`phi_offset`), which the recognizers make `(p),Y`.
  4. TAC SSA destruction + post-destruction copy folding.
  5. Asm-level const-static fold (drops scalar `static T const`
     storage, replacing references with immediates).
  6. Asm-level SSA round-trip: move coalescing, forward + backward
     copy propagation, byte-granular DCE, byte-granular regalloc
     drawing from per-function private pools (when eligible) or the
     default caller/callee partition.
  7. `replace_pseudoregisters_bare_exit` resolves Pseudos.
  8. Late prologue / epilogue synthesis (collapses to bare body +
     RTS when no save / spill is needed).
  9. `loop_counter_to_x` (X-pivot promotion for hot loop counters),
     then `counter_to_index_reg` (a pure count-down counter into X
     or Y).
  10. Peephole fixed-point loop — see the peephole catalog in
      [passes/CLAUDE.md](passes/CLAUDE.md).
  11. `loop_invariant_a` (a loop-invariant compare key kept in A),
      `apply_jump_tables` (a switch's compare chain dispatched through
      a table of case addresses, the `JumpTable` asm instruction),
      `apply_copy_through_index` (a byte copied through X or Y while
      A is busy), `expand_long_branches`, `apply_short_jumps` (jumps
      to tail-call stubs go to the function, and `LDA #k; JMP` becomes
      a branch in reach), `asm_to_asm2`, `emit_program`.

  Also enables the default zp_abi calling convention: every function
  that is eligible (no IndirectCall, not in a call-graph cycle,
  address not taken, params fit) gets zp_abi without needing the
  explicit `[[c6502::zp_abi]]` annotation. Ineligible
  functions silently fall back to soft-stack. Annotated functions
  keep today's strict contract: ineligibility is a hard error.
  The INC / DEC peepholes run in the unoptimized pipeline too
  (their win is addressing-mode-aware, not regalloc-dependent).

Objects and linking:

- `compile.py -c` writes the text `-S` would and assembles it
  (`binutils/assembler.py`) into an ELF object (`binutils/elf.py`):
  one section per function and object, symbols, `R_MOS_*`
  relocations for everything the linker places, the link metadata
  block in `.c6502.link`, and the text itself in `.c6502.asm`. See
  [binutils/CLAUDE.md](binutils/CLAUDE.md).
- `compile.py a.o b.o ... -o prog` links (`passes/linker.py`). It
  merges every unit's link metadata (`passes.zp_link_metadata`) and
  globally re-allocates the `__zpabi_*` and `__local_*` ZP slots, the
  zero-page objects, and the runtime's block over the merged call
  graph. Then it fixes each instruction's addressing mode to the
  program's addresses: an absolute operand the program puts in zero
  page is shortened (`binutils.ld.relax`), and a unit that reached a
  name in zero page that the program puts above $FF is assembled
  again from its `.c6502.asm` (`linker._reassemble`). It lays the
  sections out, applies the relocations, and writes an ELF
  executable. With `-T script.ld` (a GNU ld subset,
  `passes/link_script.py`) the script places each item's section
  (`passes/sections.py`, `[[c6502::section("name")]]`;
  `passes/link_layout.py`); without one, `.text`, `.rodata`, `.data`,
  and `.bss` follow each other from `$0800` (`-Ttext=ADDR` moves
  it). A name no object defines and the linker doesn't place is an
  error unless `--defsym NAME=VALUE` gives it. User guide:
  [docs/linker_scripts.md](docs/linker_scripts.md).
- The runtime helpers a program calls (`mul16`, `sdivmod16`, `asl8`,
  and the rest with 6502 code in `rtlib/helpers.py`) come from
  the runtime library (`passes/runtime_library.py`), as routines come
  from libgcc. The linker takes each helper that some object names,
  that no object defines, and that no `--defsym` gives, and the
  helpers those call, and assembles them at link time into one more
  object, `runtime.o`, with a section in `.text` for each. Without a
  script they follow the program's code in `.text`, and a script
  places them as `runtime.o(.text)` or with a pattern such as
  `*(.text)`. They read and write the program's HARGS, and the
  object's metadata gives their HARGS footprint, which the program's
  block counts. The linker's `fall_through` can put a helper right
  after a function that ends with a `JMP` to it.
- `sim/linked.py` runs an executable in the simulator. A program
  linked without `--defsym` runs the helpers it holds. One linked with
  the simulator's helper addresses (`sim.linked.defsym_args`, as
  `tests/link_helpers.link` does) calls the simulator's copies at
  $E000 and its Python hooks, which also cover the helpers with no
  6502 code.

For the full optimizer walkthrough, see
[passes/optimization/CLAUDE.md](passes/optimization/CLAUDE.md) and
`docs/leaf_zp_abi.md`. Each optimization has
its own document in [docs/optimizations/](docs/optimizations/README.md),
whose README describes how `-O` is organized, with diagrams, and
lists the optimizations in pipeline order. A pass that's added or
moved needs that list updated, and `tests/test_optimization_docs.py`
fails until it is.

## Regenerating AST modules

Each `*_ast.py` module is generated from its matching `*.asdl` by
`asdl.py`. After editing an ASDL file, regenerate:

```sh
uv run python asdl.py c99.asdl c99_ast.py
uv run python asdl.py tac.asdl tac_ast.py
uv run python asdl.py asm.asdl asm_ast.py
uv run python asdl.py asm2.asdl asm2_ast.py
```

The C++ port's IR classes (cpp/generated/, checked in) come from the
same files, and `tests/test_asdl_cpp.py` fails until they are
regenerated too:

```sh
uv run python asdl.py --cpp c99.asdl cpp/generated
uv run python asdl.py --cpp tac.asdl cpp/generated
uv run python asdl.py --cpp asm.asdl cpp/generated
uv run python asdl.py --cpp asm2.asdl cpp/generated
```

The generator emits one `@dataclass` per type. Sum-type bases are
named `Type_<name>` (to avoid colliding with Python builtins like
`int`); constructor classes keep their ASDL names. Fields become
`int`, `str`, `list[...]`, or `T | None` depending on the primitive /
`*` / `?` markers.

## Root-level pipeline stages

`compile.py -S` chains eleven passes. The middle-end passes
(2–6, 9–10) live in `passes/`; their walkthroughs are in
[passes/CLAUDE.md](passes/CLAUDE.md). The four stages rooted in this
directory:

- **Pass 1: `parser.parse`** (`parser.py`, `c99.lark`, `lexer.py`,
  `preprocessor.py`) — C source → `c99_ast`. See "Parser" below.
- **Pass 7: `c99_to_tac.translate_program`** (`c99_to_tac.py`) —
  `(c99_ast, SymbolTable)` → `tac_ast`. See "c99_to_tac" below.
- **Pass 8: `tac_to_asm.translate_program`** (`tac_to_asm.py`) —
  `tac_ast` → `asm_ast`. See "tac_to_asm" below.
- **Pass 11: `asm_emit.emit_program`** (`asm_emit.py`) — `asm2_ast`
  → 6502 assembly text. See "asm_emit" below.

Other root-level files:

- `nm.py` — lists the symbols of objects and executables, as GNU nm
  does, through the C++ port under `C6502_CPP_BACKEND=1`.
- `objdump.py` — shows the headers, symbols, relocations, link
  metadata, and disassembly (in dasm's syntax) of objects and
  executables, as GNU objdump does.
- `lark_cache.py` — `open_lalr`, which builds the Lark LALR parser
  of `c99.lark` that `parser.py` runs, and the one whose lexer
  `lexer.tokenize` runs, once, and saves the tables in
  `__pycache__/c99-<options>-<key>.lark.pickle`. Every later process
  loads them from there instead of computing them again (about 0.6 s
  each). The key hashes the grammar's text, the options, Lark's
  version, and Python's, so a changed grammar is never read from a
  stale file. The file is written under a temporary name and renamed,
  so pytest-xdist and fuzzer workers that start together never read
  half of one, and the parser is built in memory when the file can't
  be read or written. `lexer.py` builds its parser on the first call
  of `tokenize`.
- `asdl.py` — code generator for `*_ast.py` from `*.asdl`, and with
  `--cpp` for the C++ port's IR classes in cpp/generated/.
- `ir_dump.py` — the dump format, a canonical text form of any IR
  value (`dump`, `load`) that the C++ port reads and writes too. Its
  docstring gives the grammar.
- `tools/capture_passes.py` — records the calls of chosen asm passes
  over the corpus, for checking their C++ ports, and
  `tools/check_captures.py` runs the check.
- `tools/capture_binutils.py` — records the calls of the assembler,
  the ELF reader, the linker, the disassembler, nm.py, and objdump.py
  over the corpus and the Python tests, for `c6502cpp check-binutils`
  (cpp/README.md, "The object-file tools"), and
  `tools/binutils_errors.py` lists the errors a capture holds.
- `cpp_backend.py` — the Python side of `C6502_CPP_BACKEND=1`, which
  runs the whole compile of a C source, from the preprocessor through
  emission, in `c6502cpp backend` (`compile_source`), or through the
  finished asm program for the simulator harness.
  `tools/compare_backends.py` compiles the corpus with and without it
  and compares the outputs.
- `c99.asdl` / `tac.asdl` / `asm.asdl` / `asm2.asdl` — the four ASDL
  IR definitions.
- `c99_ast.py` / `tac_ast.py` / `asm_ast.py` / `asm2_ast.py` —
  generated `@dataclass` modules.
- `tac_sim.py` — TAC-level interpreter, used by tests to validate
  expected behavior at the TAC stage.
- `fp_arith.py` — IEEE 754 Float / Double arithmetic in Python; used
  by `tac_sim.py` and `sim/runtime.py`'s FP traps.
- `pretty.py` — pretty-printer for ASTs (used in unit-test
  diagnostics).
- `compile.py` — the CLI driver.
- `main.py` — empty `hello world` stub; not part of the compiler.
- `diagnostics.py` — warnings: `warn(where, message)` records one in
  the innermost open `collecting()` block and drops it outside one,
  so only a caller that opens such a block receives them. `compile._compile_c`
  collects each C input's and prints them to stderr as `<file>:
  warning: line N: <message>`, before any error.
- `bit_precise.py` — C23's bit-precise types: `BITINT_MAXWIDTH`, the
  storage size and type of each, the TAC constant classes of the
  3-, 5-, 6-, and 7-byte integers, the width of a `wb` constant, and
  wrapping at N bits. Its docstring describes the representation.
- `builtin_functions.py` — the builtins' names and argument counts
  (`BUILTINS`, `is_builtin`), the packed BCD arithmetic
  (`adc_byte`, `sbc_byte`, `bcd_add`, `bcd_sub`) that constant
  folding, `tac_sim`, and the type checker's constant evaluator
  share, and the `<stdbit.h>` builtins (`STDBIT_BUILTINS`,
  `STDBIT_FUNCTIONS`) with their model, `stdbit`.
- `pp_numbers.py` — the spelling of numeric constants: C23's
  pp-number (`PP_NUMBER_RE`, `pp_number_at`), binary constants, digit
  separators, and suffixes (`parse_integer`, `check_floating`), with
  the messages for a misplaced separator, a bad digit, and a `wb`
  suffix. The lexer, the parser, and the preprocessor's `#if` share
  it.
- `knowledge_base.md` — engineering-knowledge running log.

## Parser

`parser.py` implements C source → `c99_ast`. Lark/LALR grammar lives
in `c99.lark`. The top-level production is `declaration*`: a
translation unit is a list of `var_decl` / `function_decl` forms.
The AST stores them as `Program(declaration*)` where each declaration
is `VarDecl(var_decl)` or `FunctionDecl(function_decl)`; a function
*definition* is the FunctionDecl variant with `body=Block(...)`,
while a forward declaration has `body=None`. Both `var_decl` and
`function_decl` carry a required `data_type` (one of `Int()`,
`Long()`, `UInt()`, `ULong()`, `Float()`, `Double()`, or — for
functions — `FunType(params, ret)`) and an optional `storage_class`
(`Static()` / `Extern()` / None). The parser builds the function's
`FunType` from the per-param `type_specifier+` runs and the return-
type specifiers. A `<param_list>` is `void` (empty params) or comma-
separated `<type_specifier>+ IDENT` pairs. Parameter *names* live on
the function_decl's `params` array; their *types* live in parallel on
`data_type.params`.

### Type vocabulary

Twelve integer types, C23's bit-precise integer types, two floating
types, `nullptr_t`, plus pointers and function types. Widths follow C99's minimum ranges per §5.2.4.2.1 — Int is 16
bits, Long is 32 bits, LongLong is 64 bits.

Integers:

- `Int()` is 2-byte signed (-32768..32767).
- `Long()` is 4-byte signed (-2^31..2^31-1).
- `LongLong()` is 8-byte signed (-2^63..2^63-1).
- `UInt()` is 2-byte unsigned (0..65535).
- `Short()` is `short` and `UShort()` is `unsigned short`, as wide
  as `Int()` and `UInt()` (C99 allows that) but distinct types.
  `short` promotes to `int` and `unsigned short` to `unsigned int`,
  since `int` can't hold 65535. Nothing after the type checker sees
  them: `c99_to_tac` starts by replacing them with `Int()` and
  `UInt()` everywhere (`_erase_short`).
- `ULong()` is 4-byte unsigned (0..2^32-1).
- `ULongLong()` is 8-byte unsigned (0..2^64-1).
- `Char()` is 1-byte unsigned (0..255; plain `char` is unsigned in
  c6502 per C99 §6.2.5.15's implementation-defined choice — same
  byte semantics as `unsigned char`).
- `SChar()` is `signed char` (1-byte signed, -128..127).
- `UChar()` is `unsigned char` (1-byte unsigned, 0..255).
- `Bool()` is `bool` (1 byte holding 0 or 1), a keyword since C23
  (§6.4.1). So are its constants `true` and `false`, which the
  parser makes `ConstBool(1)` and `ConstBool(0)` and which have
  type `bool`, so `sizeof(true)` is 1 and `true + true` is the
  `int` 2. `_Bool` is the same type, and the preprocessor warns at
  each use of it (`preprocessor.ALT_BOOL_WARNING`), since C23
  makes that spelling obsolescent. `<stdbool.h>` defines only
  `__bool_true_false_are_defined`. In an `#if`, `true` is 1 and
  `false` 0 (C23 §6.10.2p13). The warnings go through
  `diagnostics.py`: `diagnostics.warn` records one, a
  `diagnostics.collecting()` block collects them, and compile.py
  prints each C input's to stderr as `<file>: warning: line N:
  <message>` without failing the compile.

An enumerated type is `Int()` (C99 §6.7.2.2.4 lets it be compatible
with `int`), and an enumeration constant is an `int` constant.
With C23's fixed underlying type, `enum e : T`, the enumerated type
and its constants are T. Neither reaches the AST; see "Enumerations"
under Parser.

Char/SChar/UChar/Bool are distinct types from Int/UInt — C99
§6.3.1.1.2 integer-promotes them all to `int` (because Int's 16-bit
range covers both signed and unsigned char), so arithmetic always
happens at Int width or wider.

Converting a scalar to `bool` gives 0 if it compares equal to 0 and
1 otherwise (C99 §6.3.1.2). The type checker applies that to static
initializers. For everything else, c99_to_tac lowers each conversion
to `x != 0`, skipping it for a value that is already 0 or 1 (a
comparison, `!`, `&&`, `||`, a `bool`), and makes `++` store 1 and
`--` store `x == 0`. Then it replaces `Bool` with `UChar` in the
symbol table, the struct layouts, and the top-level declaration
types, so no pass after it sees `Bool`. It marks each such object's
`LocalAttr` / `StaticAttr` `was_bool`, which lets `!` of it be `^ 1`
(passes/optimization/lnot_jump_fold.py). tac_to_asm lowers a `!= 0`
with a 1-byte result as `LDA x; ORA ...; BEQ end; LDA #1`.

`BitInt(width, is_signed)` is C23's `_BitInt(N)` (`is_signed`, for
2 <= N <= 64) or `unsigned _BitInt(N)` (1 <= N <= 64), a bit-precise
integer type (§6.2.5p5, §6.7.3.1). `BITINT_MAXWIDTH` is 64, the least
C23 allows, since the runtime's helpers stop at 8 bytes; `<limits.h>`
defines it and `bit_precise.BITINT_MAXWIDTH` holds it. The parser's
`bitint_specifier` folds N as it folds an array size, and
`_bit_precise_type` takes at most one `signed` or `unsigned` beside
it and checks N. These rules come from the type checker:

- No integer promotion (§6.3.1.1p2): `_BitInt(5) + _BitInt(5)` is a
  `_BitInt(5)`, and so are `-x`, `~x`, `x++`, and `x << n`.
- Rank (§6.3.1.1p1, `type_checking._rank`): by width, with a standard
  type above a bit-precise one of the same width. So `_BitInt(16) +
  int` is an `int`, `unsigned _BitInt(16) + int` an `unsigned int`,
  and `_BitInt(24) + int` a `_BitInt(24)`
  (`_common_bit_precise_type`, the usual arithmetic conversions of
  §6.3.1.8).
- `sizeof` is ceil(N/8) (`bit_precise.storage_bytes`), so `unsigned
  _BitInt(24)` is 3 bytes.
- A constant expression computes at N bits (`_int_bits`,
  `constant_expression._width_bits`), with an unsigned type wrapping
  and signed overflow rejected as for the other types. A static
  initializer of a bit-precise type converts to N bits
  (`_coerce_init_value`).
- The type-generic `<stdbit.h>` functions take an `unsigned
  _BitInt(N)` of 8, 16, 32, or 64 bits (§7.18.3p2); the packed BCD
  builtins take none.

A value takes ceil(N/8) bytes, low byte first, and the padding bits
above bit N-1 hold the sign extension of a signed value and zeros for
an unsigned one, so the bytes are the value as an integer of that
width. `c99_to_tac._restore_padding` restores that after each
operation that can change it (`+`, `-`, `*`, `<<`, unary `-`, `~` of
an unsigned value, a signed `/`), and `_emit_bit_precise_cast` after
a conversion into the type from one with values it can't hold: an
`&` with the mask of N bits, and for a signed type `^` and `-` of the
sign bit after it. Neither step is emitted for a type whose N is a
multiple of 8.
Signed overflow wraps (it's undefined, and a conversion out of range
is implementation-defined). Conversions to and from `float` and
`double` extend to (or come from) the next of 2, 4, and 8 bytes. At
the end, `_erase_bit_precise` replaces each bit-precise type in the
symbol table, the layouts, and the declarations with its storage
type (`bit_precise.storage_type`): the standard integer type for 1,
2, 4, and 8 bytes and `BitInt(8 * bytes, is_signed)` for 3, 5, 6, and
7. The back end handles those as whole bytes, with the TAC constants
`ConstInt24` to `ConstUInt56` (`bit_precise.ODD_CONST_CLASSES`), and
`tac_to_asm._odd_width_helper_op` passes a multiply, divide,
remainder, or shift by a variable of one to the helper of the next of
4 and 8 bytes, each operand extended in `HARGS` (zero-filled for `*`
and `<<`, by its signedness for the others). See `bit_precise.py`.

Floating:

- `Float()` is IEEE 754 single (4 bytes).
- `Double()` is IEEE 754 double (8 bytes).
- `long double` (16-byte IEEE 754 quad / extended) isn't modelled —
  the parser rejects it.

`Pointer(referenced_type)` is a 2-byte address (the 6502's address
width); declared with `*` in the declarator, e.g. `int *p;`. Pointer
is its own TAC variant (`tac_ast.Pointer`) because 2-byte pointers
no longer match any integer width — Int is now 2 bytes too, but
conceptually distinct, and the symbol table preserves the Pointer
type for the rare codegen sites that inspect it (signedness checks
for unsigned ordering, pointer-arithmetic scaling).

`NullptrT()` is C23's `nullptr_t` (§7.21.2), a scalar type whose one
value is `nullptr`, 2 bytes with a null `void *`'s representation.
The keyword `nullptr` (C23 §6.4.1) is `ConstNullptr()`, a null
pointer constant of that type, and `<stddef.h>` declares the type as
C23's does, `typedef typeof_unqual(nullptr) nullptr_t;`.
The type checker applies C23's rules. A `nullptr_t` converts
implicitly to any pointer type and to `bool` (§6.3.2.4, §6.5.17.2),
and by cast to those and to `void`. Only a null pointer constant or
another `nullptr_t` converts to it (`_is_c23_null_pointer_constant`).
`==` and `!=` take two `nullptr_t`s, a `nullptr_t` and a null pointer
constant, or a pointer and a `nullptr_t` (§6.5.10p2,
`_check_nullptr_binary`). Every other binary operator, unary `-`,
`+`, `~`, `++`, and `--` reject it. `?:` takes it with a pointer or
another `nullptr_t`, but not with `0` (§6.5.16p3, footnote 109). It
isn't an integer constant (§6.6p8), so `case nullptr:` and `int
a[nullptr]` are errors, but `static int *p = nullptr;` is a static
initializer. `c99_to_tac` starts by replacing `NullptrT` with `void
*` and each `nullptr` with `(void *)0` (`_erase_nullptr`), so the TAC
has the constant 0 and no pass after the type checker sees either.

### Constants

The lexer splits integer literals into four terminals by suffix
(`INTEGER_CONSTANT` for no suffix, `LONG_INTEGER` for `L`/`LL`,
`UINT_INTEGER` for `U`-only, `ULONG_INTEGER` for `U+L` in any order
— `LL` shares a terminal with `L`, and `ULL` with `UL`; the parser's
`has_ll` flag then routes `LL`/`ULL` cases into separate candidate-
list rows). Floating literals split into three (`DOUBLE_CONSTANT` for
no suffix, `FLOAT_CONSTANT` for `f`/`F`, `LONG_DOUBLE_CONSTANT` for
`l`/`L`). The parser's `_const_for_token` then maps each integer
token + base (decimal vs. hex/octal) to a c99 const variant per the
C99 §6.4.4.1 paragraph 5 type-list rule (first type whose range fits
the value):

- unsuffixed decimal:    int → long → long long
- unsuffixed hex/octal:  int → unsigned int → long → unsigned long →
                          long long → unsigned long long
- `L` decimal:           long → long long
- `L` hex/octal:         long → unsigned long → long long → unsigned
                          long long
- `LL` decimal:          long long
- `LL` hex/octal:        long long → unsigned long long
- `U`:                   unsigned int → unsigned long → unsigned long
                          long
- `UL` (any letter order): unsigned long → unsigned long long
- `ULL` (any letter order): unsigned long long

Picking the matching const variant from those lists. A literal whose
value exceeds `unsigned long long` (the widest type c6502 models) is
rejected with "doesn't fit any supported type".

C23's `wb` / `WB` suffix, with `u` or `U` before or after it or
neither (§6.4.4.2p1), lexes as `BITINT_INTEGER` or `UBITINT_INTEGER`
and makes a `ConstBitInt(value, width, is_signed)`: `wb` the
`_BitInt(N)` of the least N > 1 that holds the value and a sign bit,
`uwb` the `unsigned _BitInt(N)` of the least N > 0 that holds it, in
any base (§6.4.4.2p6; `bit_precise.constant_width`). So `3wb` is a
`_BitInt(3)`, `3uwb` an `unsigned _BitInt(2)`, and `0uwb` an
`unsigned _BitInt(1)`. A constant whose value takes more than 64 bits has no
type, which `pp_numbers.bit_precise_width` reports. In `#if` a `wb`
constant is signed and a `uwb` one unsigned, since every type there
acts as `intmax_t` or `uintmax_t` (§6.10.2p13).

Floating literals
follow C99 §6.4.4.2 — the suffix uniquely determines the type, no
value-fitting rule:

- unsuffixed: `ConstDouble`
- `f` / `F`:  `ConstFloat`
- `l` / `L`:  rejected ("long double not supported")

Hex floating literals (`0x1.0p3`) lex but the parser rejects them —
Python's `float()` doesn't parse the C hex-float syntax, so wiring
up support would mean writing the conversion by hand; deferred until
something in the corpus needs them.

C23 §6.4.4.2 adds binary constants and digit separators, which the
terminals in `c99.lark` take. A binary constant (`0b1010`, `0B1010`)
is in `_BARE_INT` before the octal form, so it carries every integer
suffix and lexes as one of the four integer terminals, and the parser
types it by the hex/octal list (`base == "hex_oct"`), so
`0b1111111111111111` is an `unsigned int` where `65535` is a `long`.
A digit separator is a `'` between two digits of one digit sequence
(`1'000'000`, `0xFF'FF`, `0b1010'1010`, `0'17`, `1'000.5e1'0`). The
terminals take one only there, and `_parse_integer_token`
(through `pp_numbers.parse_integer`) and `_const_for_token` drop
them for the value. `lexer.check_number`, which `tokenize` and the
parser's `_TypedefLexer` both call on every numeric token, reads the
pp-number at the token (C23 §6.4.8) and raises `LexError` for a
separator anywhere else (last, doubled, after `0x` / `0b`, touching
the period or the exponent, before the suffix), for a binary
constant with a digit other than 0 and 1 or no digits, and for a
`wb` / `uwb` suffix, whose `_BitInt` c6502 doesn't have. A number
with none of those is left to the terminals as before, so `08` still
lexes as `0` and `8`. `pp_numbers.IntegerConstant.length` is "wb"
for the bit-precise suffix, which a `_BitInt` would type.
`_parse_integer_token` reads each base itself, which also fixed
octal constants with a nonzero digit (`017`), which used to raise
Python's ValueError.

`Constant(const)` wraps the resulting `Type_const`.
`_split_specifiers` validates the run of specifier tokens (`int`,
`long`, `signed`, `unsigned`, `float`, `double`, `static`, `extern`)
and splits it into the `(data_type, storage_class)` pair.
`_resolve_data_type` decodes the C99 §6.7.2 combinations c6502
supports (`int` / `signed [int]` → Int; `unsigned [int]` → UInt;
`long [int]` / `signed long [int]` → Long; `unsigned long [int]` →
ULong; `long long [int]` / `signed long long [int]` → LongLong;
`unsigned long long [int]` → ULongLong; `short [int]` / `signed short
[int]` → Short; `unsigned short [int]` → UShort; `char` → Char;
`signed char` → SChar; `unsigned char` → UChar; `bool` or `_Bool` → Bool; `float` → Float;
`double` → Double), rejecting multiple type specifiers, multiple
storage classes, missing type, three or more `long`s, `short` twice or
with `long`, `bool` combined with any other type specifier, `char`
combined with `int` / `long` / `short`, `long double`, `signed unsigned`, and any
FP/integer specifier mix (`int float`, `unsigned double`, etc.).

### Statement and expression grammar

See `c99.lark` for the full grammar — it's the authoritative,
compact description of what parses. A few non-obvious AST-shape
notes that aren't visible from the grammar alone:

- Forward function declarations (`int foo(int x);`) parse as
  `var_decl` (the trailing `;` matches that rule); the transformer
  rewraps as `Type_function_decl` with `body=None` when the
  declarator composes to a FunType.
- Compound assignments build a `CompoundAssignment(op, lval, rval,
  intermediate_type?, data_type?)` node, NOT a parse-time desugar
  to `Assignment(lval, Binary(OP, lval, rval))`. The explicit node
  lets c99_to_tac evaluate the lval's address ONCE for side-effect-
  ful lvals like `arr[i++] += 1`. The type checker stamps
  `intermediate_type` (binop working type) and `data_type` (lval
  type, the result type). When `rval` calls a function and `lval`
  neither calls nor writes anything and isn't volatile, c99_to_tac
  evaluates `rval` first (`_calls_first`, which C's unsequenced
  operands allow), so `E->level |= h(x)` keeps nothing across the
  call.
- Prefix `++a`/`--a` → `Prefix(incdec_op, exp)`; postfix `a++`/`a--`
  → `Postfix(incdec_op, exp)`. Separate nodes (not desugars) for
  the same address-once reason and because prefix returns the new
  value, postfix the old.
- A call is a postfix operator on any expression. `FunctionCall(name,
  args)` calls a name, a function or a function pointer (`f(x)`,
  `fp(x)`). Any other callee (`t[i](x)`, `p->g(x)`, `pick()(x)`,
  `(*fp)(x)`) is an `IndirectCall(callee, args)`. The type checker
  strips the `*` and `&` that don't change which function it is,
  leaving a `Var` when a name is left, which c99_to_tac calls as it
  would by name; otherwise c99_to_tac evaluates the callee and emits
  a TAC `IndirectCall` through it.
- Loop / switch / case / default / labeled nodes carry an
  `identifier label` field the parser leaves empty; loop_labeling /
  label_resolution fill it in later. `SwitchStmt` also has `cases`
  (list of `(value, label)` pairs), `default_label`, and a
  `promoted_type` filled by the type checker.
- An initializer in braces is an `InitList` whatever the declared
  type, and C23's empty initializer `{}` (§6.7.11) is one with no
  items. An item may be a `Designation(designators, init)`, from
  `[k] = v`, `.m = v`, or a chain such as `.a[2].b = v` (C23
  §6.7.11p1). The parser folds each array designator's index as it
  folds an array size, so an enumeration constant or a `constexpr`
  object works there, and rejects a negative one.
  `passes/designators.py` rewrites a list with designations into one
  item per subobject in order, with `{}` for each subobject it leaves
  out, before any other initializer processing. string_lifting does
  that for the whole program, and the type checker does it again for
  the lists the parser folds. A union's list then holds one item, and
  `InitList.union_member` names the member it initializes (None for
  the first). A static union whose member isn't the first has a
  `UnionValue` value tree, and both a static and an automatic one
  have zero in the bytes past the member. A later initializer of a
  subobject overrides an earlier one (§6.7.11p19), a designation into
  a subaggregate overrides only the part it names, and an array of
  unknown size takes its size from the largest index (§6.7.11p22).
  An overridden initializer isn't evaluated, and one with an
  assignment, `++`, `--`, or a call is an error. A positional item
  after a chain of designators would need brace elision, which c6502
  doesn't have, so it's an error that says to brace the
  subaggregate. A designator that doesn't fit the type, an index past
  the end, and a designator in a scalar's braces are errors too. The type checker gives a scalar's braces their meaning
  (`_unbrace_scalar_init`): `{e}` and `{e,}` are `e`, `{}` is a zero
  of the scalar's type (`_empty_init_zero`), and two items or a
  second pair of braces are errors (§6.7.11p12). This applies to a
  member or element too, as in `{ {}, {5} }`. An array of unknown
  size can't take `{}` (§6.7.11p4, `_size_from_initializer`). A
  static object's value tree from `{}` is all zero, so it goes in
  `.bss`. For an automatic aggregate, `c99_to_tac._init_leaves`
  lists the stores the initializer makes and the zero bytes it
  leaves: the subobjects the list doesn't reach, the explicit zero
  constants, and a default-initialized union's bytes past its first
  member, which §6.7.11p11 zeroes. Adjacent zero bytes are one run,
  which `_emit_zero_bytes` clears with constant stores of 8, 4, 2,
  or 1 bytes when it's shorter than `_CLEAR_LOOP_BYTES` (9), and
  otherwise with a loop that counts a byte index down (`LDA #0`
  before it and `DEY; STA v,Y; BNE` in it under `-O`, after
  `passes/loop_constant_a.py`). The comment at
  `_CLEAR_LOOP_BYTES` has the measurements.
- A compound literal `(T){...}` (C99 §6.5.2.5) is a
  `CompoundLiteral(name, target_type, init)` node, parsed as a
  postfix expression (`{` after `(T)` tells it from a cast). The
  braces may be empty, as in `(struct s){}`. Its `init` has a
  var_decl's shape (InitList, a String for a char array, or a
  scalar's one expression, with a scalar's `{}` or longer list left
  an InitList for the type checker), identifier_resolution names
  its object `@N.compound_literal`, and the type checker checks it as
  that declaration: automatic inside a function, static at file
  scope, an unsized array sized from the initializer. c99_to_tac
  initializes the object where evaluation reaches the literal and
  then treats it as a `Var`. C23 lets storage-class specifiers go
  between the `(` and the type (§6.5.3.6), the grammar's
  `compound_literal_storage`. `_literal_storage` turns them into the
  node's `storage_class` (`Static()` or `Register()`) and
  `is_constexpr`, and rejects the others, which C23 leaves undefined,
  and `thread_local`, since c6502 has no threads (`_NO_THREADS`, which
  `_split_specifiers` gives a declaration too). The type checker
  checks the literal as a declaration with those, and rejects
  `register` at file scope and `&` of a `register` literal. A
  `constexpr` literal is a compound literal constant (§6.6p6), whose
  value `TypeChecker._named_constant` gives by its name, as for a
  named constant. For the folds in the parser, `_named_literals`
  names each literal in the copy, since identifier_resolution hasn't
  run yet.
- Case-label expressions go through a `constant_exp` wrapper around
  `conditional_exp` — the wrapper is a hook for the §6.6 validator
  in `passes.constant_expression`.
- Array declarators (`int a[10];`) and function-pointer declarators
  (`int (*fp)(int);`) parse but `_apply_direct_declarator` raises
  NotImplementedError on the array suffix — c99_ast has no Array
  variant yet.
- The assignment LHS is loosened from C99's `unary-expression` to
  `conditional_exp`, so `1+2=3+4` parses — identifier resolution
  rejects non-lvalue forms.
- LALR(1) shift-reduce resolutions worth knowing: dangling-else
  binds to the nearest `if`; `IDENTIFIER COLON` at statement-start
  picks the labeled-statement branch; `(` followed by a type-
  specifier token picks the cast branch.
- A syntax error that `parse` passes on from Lark lists the terminals
  it could take after "Expected one of" in sorted order
  (`_sort_expected`). Lark gives them in an order that follows its
  parse table's, which differed between two runs of the same program.

### Typedef names

`typedef` (C99 §6.7.7) never reaches the AST. The parser puts each
typedef name's type in place of its uses, so every later pass sees
ordinary types, and a typedef declaration leaves nothing behind but
any struct body it defines (plus a forward `struct foo;` for a tag
it introduces into its scope, so identifier resolution introduces
the same tag there).

Because `T * x;` is a declaration when T names a type and a
multiplication otherwise, the parser tracks scopes while it parses
(`_ParseContext` in `parser.py`):

- The AST builder runs inside the LALR parse (`Lark(...,
  transformer=_BUILDER)`), as each rule reduces, rather than on the
  finished tree. `init_declarator` declares its name in the current
  scope as soon as the declarator ends: a typedef name with its
  type, or an ordinary name that hides one.
- `_TypedefLexer` pushes a scope at each block `{` and each `for`,
  pops it at the `}` (the `for` scope pops in `for_stmt`, which then
  retypes the token after the loop, since the lexer runs one token
  ahead of the reductions). It retypes an IDENTIFIER to TYPEDEF_NAME
  when the name is a typedef in scope AND the parse table can shift
  a TYPEDEF_NAME at that point. That second test is what makes `int
  T;`, `p->T`, `struct T`, and a parameter named T ordinary
  identifiers.
- The grammar admits a typedef name only as the head of a specifier
  list (`decl_specs`, `_specqual_list`), never after a type
  specifier, so after `int` the parser can't shift one.

`struct { ... }` with no tag gets a made-up tag
(`__anonymous_struct_<N>`). A typedef whose struct tag an inner
declaration hides at the point of use is rejected, since the
substituted `struct foo` would name the inner one. `<stdint.h>`
uses real typedefs. `bool`, `true`, and `false` are keywords (C23
§6.4.1), so `<stdbool.h>` defines only `__bool_true_false_are_defined`.
`<stddef.h>` declares `nullptr_t` as `typeof_unqual(nullptr)`.

### auto

C23's `auto` (§6.7.2, §6.7.10) is in the grammar's `specifier` rule.
With a type specifier it's C99's automatic storage class:
`_split_specifiers` leaves `storage_class` None, and `var_decl`
rejects it at file scope (`_has_auto`) and with another storage
class. With no type specifier, `_split_specifiers` returns an
`_Inferred` holding the qualifiers, and `init_declarator` infers the
type before it declares the name (`_inferred_type`): the
initializer's type from `_expression_type`, as for `typeof`, without
its qualifiers, with an array or a function as a pointer, and with
the declaration's qualifiers (and `const` for `constexpr`). The
declarator must be a plain name, with an initializer that is an
expression and doesn't use the name. `init_declarator` returns the
type as the fifth item of its tuple, and `var_decl` takes it in place
of composing the declarator, so no `auto` reaches the AST. A
function, a parameter, a typedef, and more than one declarator in an
inferring declaration are rejected.

### Enumerations

`enum` (C99 §6.7.2.2), like `typedef`, never reaches the AST. The
parser's `_ParseContext` declares each enumeration constant in the
current scope, with its value, when its `enumerator_list` rule
reduces, which is before the next enumerator's value is parsed, so
`enum { A, B = A + 1 }` works. The `identifier` rule puts
`Constant(ConstInt(value))` in place of each use, so an array size or
a case label can use one, and an inner declaration of the same name
hides it, since every ordinary declaration goes in the parser's
scopes. A value is folded like an array size (`_fold_integer_constant`)
and must fit in an `int`.

The specifier (`_EnumSpecifier`) resolves to `Int()`, or to the fixed
underlying type. No c6502 attribute applies to an enum
(`_no_enum_attributes`), and `[[c6502::packed]]`, which made an enum
the smallest of `UChar()`, `SChar()`, and `Int()` that held its values,
was removed on 19 Nov 2026 in favor of the fixed type. Using it is an
error whose message points to `enum e : uint8_t`. `_TagHead` follows the tokens from `enum`,
`struct`, or `union` to the body's `{`, so the lexer doesn't open a
scope there, and it follows an enum's `: T` too, which is why the
lexer retypes a typedef name before `_TagHead` sees the token. Enum
tags live in `_Scope.enum_tags` as `_EnumTag`s (the type, whether a
`:` fixed it, whether the list has been parsed), beside the
struct/union tags, and `enum tag` must name an enum declared in an
enclosing scope. A declaration that only defines an enum leaves
nothing in the AST.

C23's fixed underlying type (§6.7.3.3), `enum e : uint8_t { A, B }`,
makes the enum that type and each constant a constant of that type.
The grammar's `enum_head` (`enum`, attributes, tag, and `:
T`) reduces when the parser sees the `{`, before the first
enumerator. Its transformer pushes the type on
`_ParseContext.enum_types`, which `enumerator_list` checks each value
against, and declares a typed enum's tag right away, since the type
is complete from the end of its `: T`, so `enum e : long { A =
sizeof(enum e) }` works. An enum defined inside an enumerator's value
pushes its own entry, so `enum E : char { A = sizeof(enum F : long {
X }) }` checks `X` against `long` and `A` against `char`.
`enum_type_specifier` resolves T, which must be an integer type
other than a bit-precise one or an enumerated one: a typedef name for
an enum is marked `_Binding.enumerated`, and `enum_type_tag` takes
`struct s`, `union u`, and `enum e` after the `:` only to report them.
The qualifiers are dropped. A value, given or one more than the one
before, that T can't hold is an error.

The `identifier` rule puts `_enum_constant(value, T)` in place of a
use of a typed enum's constant: a `Constant` of T's own variant for
`int`, `unsigned int`, `long`, `unsigned long`, `long long`,
`unsigned long long`, `unsigned char` (`ConstUChar`), and `bool`
(`ConstBool`), and for `char`, `signed char`, `short`, and `unsigned
short`, which have none, a `Cast` to T of a `ConstInt` (`ConstUInt`
for `unsigned short`). The type checker and the constant evaluator
then give it T, so `sizeof A` is `sizeof(T)` and `A + A` follows T's
promotions: with `enum F : uint8_t { M = 200 }`, `M + M` is the
`int` 400.

`enum e : T;` alone is `enum_tag_decl`, a `declaration` of its own,
since C23 allows that form nowhere else. An `enum_specifier` with a
`:` always has its list, which keeps the `:` list from running into
the specifiers after an enum specifier, and the list after the `:` is always followed by `{` or
`;`, so the grammar has no new conflict. Any other use of `enum e :
T` without a list, such as `enum e : int x;`, `(enum e : int)x`, or
`enum e : int;` in a struct or a `for`, is a syntax error that
`_enum_type_without_list` rewords. `_declare_enum_tag` takes a tag
declared again in one scope only with the same type
(`enum e : int; ... enum e : int { A };`, or `enum e : int;` after
the definition), and rejects a definition without the type after a
declaration with one, a declaration with one after a definition
without, and two definitions.

### Attributes

Attributes are C23's (§6.7.13), and c6502's own carry its prefix:
`[[c6502::zp_abi]]`, `uint8_t n [[c6502::in_x]]`. GCC's
`__attribute__((...))` is gone; `parse` turns the syntax error a source
using it gets into a `ParserError` that shows the C23 form. The lexer
makes `[[` (with optional space between) one `ATTR_OPEN` token, since
C23 allows two `[` in a row nowhere else, so `x [[...]]` and `x[3]`
part on one token of lookahead; `]]` stays two tokens, and `::` is
`COLONCOLON`.

`attribute_specifier_seq` goes before a declaration, function
definition, parameter, or member declaration; after the name in a
`direct_declarator`; after `struct`, `union`, or `enum`; after an
enumerator's name; and alone before `;` (`attribute_declaration`,
for `[[fallthrough]];`). Its transformer builds an `_Attributes` with
two `(name, arg)` lists: c6502's (prefix dropped, `__x__` spellings
normalized) and the standard ones. Any other prefix's attributes, and
unknown unprefixed names, are dropped; a c6502 name without the prefix
or an unknown `c6502::` name is an error. Each declaration merges its
leading attributes with those after its name (`_merge_attributes`,
`_declarator_attributes`), and `_c6502_attributes(attrs, kind, where)`
checks the standard ones against the kind of entity
(`_STANDARD_ATTRIBUTES`), rejects a statement attribute on anything
but a statement, and hands the c6502 ones to the per-kind
helpers: `_extract_function_attributes`, `_extract_object_placement`,
`_extract_section`, and `_register_marker` (parameters), and
`_no_enum_attributes` rejects any on an enum. Of the standard ones only `noreturn` / `_Noreturn`
is kept, as `function_decl.noreturn` (`_is_noreturn`), which the type
checker ORs into `FunAttr.noreturn` and every pass that rebuilds a
`function_decl` copies. Attributes after a parameter list, `*`, `]`,
or the type specifiers (which C23 applies to a type) aren't in the
grammar.

Statement attributes (C23 §6.8) go before an unlabeled statement:
`?statement` has an `attribute_specifier_seq unlabeled_stmt ->
attributed_stmt` alternative, and `_apply_statement_attributes` checks
and applies them (`_STATEMENT_ATTRIBUTES` lists them).
`[[c6502::unroll]]` and `[[c6502::keep]]` before a `for` set
`ForStmt.loop_attribute` to "unroll" (the unroller's mark) or "keep"
(`c99_to_tac.KEEP_LOOP_MARK`, which dead-loop elimination reads).
`[[c6502::save_hardware_stack]]` before an expression statement or a
`return` marks its calls. Before a block-scope declaration it goes
with the declaration's leading attributes, and c6502 reads it as
applying to the calls in the initializers: `var_decl` takes it out
(`_take_hw_stack_save`) and returns a `_HwStackSaveDecls` list, which
`decl_item` marks and `start` and `for_init` reject. Attributes before
`;` stay an attribute declaration, so the null statement takes none,
and attributes before a label belong to the label
(`attributed_labeled_stmt` and the `case` / `default` forms), where
only `maybe_unused` applies. The `#pragma c6502` lines these replaced
are a `PreprocessorError` that names the attribute
(`preprocessor._reject_c6502_pragmas`); other `#pragma` lines become
blank lines.

### constexpr

C23's `constexpr` (§6.7.2) is a storage-class specifier, the
`CONSTEXPR` keyword in the grammar's `specifier` rule. Its rules come
from the parser and the type checker:

- `_split_specifiers` lets it come alone or with `static` or
  `register` and leaves it out of its result. `var_decl` tests for it
  with `_has_constexpr`, sets `var_decl.is_constexpr`, requires an
  initializer, and makes the declarator's type `const`
  (`_constexpr_type`), so `constexpr int *p` is `int *const p`. A
  function, a parameter, a typedef, and a declaration without a
  declarator reject it.
- `TypeChecker._begin_constexpr`, before the usual checks, rejects a
  volatile type or member and checks each scalar in the initializer
  on a copy (`_check_constexpr_init`): an integer object takes an
  integer constant expression and a real floating one an integer or
  floating constant, and the value must survive the conversion to the
  object's type (§6.7.2p5-6). A string's characters must fit a
  `signed char` element. `_end_constexpr`, after them, takes the
  object's value tree (its `Initial` value, or `_const_init_aggregate`
  of the converted initializer for an automatic one), requires every
  pointer in it to be null, and records it in `_constexpr`.
- At file scope a `constexpr` object becomes `static` (internal
  linkage, §6.2.2p3), and it must be the only declaration of its name.

The name of a `constexpr` object, and a `.` member of one at any
depth, is a named constant (§6.6p7), and a `constexpr` compound
literal is a compound literal constant (§6.6p6).
`constant_expression.named_constant` returns its type and value from
the innermost resolver installed with `named_constants`, which for
the whole program is `TypeChecker._named_constant`, installed by
`check_program`. The evaluator takes an
integer one as an operand, and a floating one (or a floating
constant) under a cast to an integer type (§6.6p8,
`_floating_operand`). `_const_init_value` and `_const_init_aggregate`
take any named constant, so a structure constant can initialize a
static struct. A union constant's members other than the first have
no value (§6.6p16). An integer named constant that is 0 is a null
pointer constant (`_is_null_pointer_constant`). Subscripting a
`constexpr` array isn't a constant expression, as C23 specifies.

The parser folds array sizes, enumeration values, and `_BitInt`
widths before the type checker runs, so `init_declarator` folds a
`constexpr` scalar's or structure's initializer when it can
(`_fold_named_constant`) and puts `(type, value)` on its `_Binding`.
`_fold_integer_constant` gives its type checker those of the names in
the expression (`_checker_with_named_constants`), and, when one is a
structure, the struct bodies parsed so far (`_ParseContext.struct_bodies`,
the latest body of each tag).

A static initializer folds `+`, `-`, `*`, and `/` of `float` and
`double` constants with `fp_arith` (`_FP_FOLDS`), so that `constexpr
double third = 1.0 / 3.0;` compiles.

### Lexer & preprocessor

The lexer (`lexer.py`) treats comments as lex errors — it assumes a
preprocessor has already stripped them. `preprocessor.preprocess`
wraps `pcpp` (installed as a uv tool, used via its Python API, no
shelling out). Malformed numeric tokens (`0x` with no digits, `3e`
with no exponent body) raise `LexError` rather than being split.

Two pcpp hooks in `CompilePreprocessor` follow C23. Its
`on_unknown_macro_in_expr` gives `true` the value 1 and `false` the
value 0 in an `#if` (C23 §6.10.2p13; pcpp gives every other name left
there 0), even under `--passthru-unknown-exprs`. Its `token` override
warns at each `_Bool` the preprocessor writes out, with the line it's
on (`_where`), which for a `_Bool` a macro expands to is the line of
the macro's use. A program that names something `bool`, `true`,
`false`, or `nullptr` gets a syntax error, which
`parser._c23_keyword_as_name` rewords to say that C23 made the name a
keyword. In an `#if`, `nullptr` is 0, as pcpp gives every other name
left there.

pcpp's own number rule is digits and a suffix, so `1'000` would be
`1` and a character constant that could swallow the rest of the
line. `CompilePreprocessor` runs a PLY lexer built from pcpp's rules
with C23's pp-number (`pp_numbers.PP_NUMBER_RE`) in place of the
integer rule (`_PP_LEXER`, cloned per preprocessor), so a number with
separators, a binary constant, and a floating constant are each one
`CPP_INTEGER` token. In an `#if`, `_NumberEvaluator` rewrites each
number token to its decimal value (with `U` when unsigned, or above
`INTMAX_MAX` in a non-decimal base) before pcpp's evaluator reads it,
and an invalid one, a floating constant, or a `wb` suffix raises
`PreprocessorError` from `evalexpr`.

`docs/*_grammar.txt` files are reference documentation for the spec
grammars that `c99.lark` implements — they aren't parsed by any tool.

## c99_to_tac

`c99_to_tac.translate_program` — `(c99_ast, SymbolTable)` →
`tac_ast`. See the module docstring at the top of `c99_to_tac.py`
for the program shape, the two-pass build (AST walk for function
definitions; symbol-table sweep for static-storage initializers),
the per-construct C99→TAC mapping table, the short-circuit
lowerings, and the cross-cutting invariants downstream passes depend
on (typed temporaries registered in the symbol table with the
surrounding expression's `data_type`; const variants that carry
width AND signedness; `@N.orig` parameter naming preserved from
identifier_resolution; implicit `Ret(0)` appended at fall-off;
pointer-arithmetic byte-scaling via `translate_pointer_arithmetic`).

## tac_to_asm

`tac_to_asm.translate_program` — `tac_ast` → `asm_ast`.
Under `-O`, a load or store of 2 bytes or more through a zero-page
pointer at a constant offset copies the pointer into DPTR again before
each byte (`_restage`, with the rules and the effective-type argument
for stores in `passes/pointer_restage.py`), so
`memory_value_propagation` sends every byte through `(p),Y`. See the
module docstring at the top of `tac_to_asm.py` for the per-construct
mapping, the runtime helper layout table (HARGS slots per helper),
the inline comparison / LogicalNot lowerings, the cast lowering
specifics, and the caller-/callee-side calling convention. Two
cross-cutting facts other docs / readers need at orientation level:

- **The asm IR is strictly 1:1 with 6502 opcodes** (the documented
  exceptions are `Ret`, `FunctionPrologue`, and `AllocateStack`,
  which `asm_to_asm2` expands into atoms before emit). No width
  tagging — the 6502 is an 8-bit machine, every operand is one
  byte. That makes `tac_to_asm` the single home of all multi-byte
  lowering: for each TAC instruction whose operands are wider than
  1 byte, the translator emits a sequence of byte-level asm atoms,
  typically one pass per byte with the 6502's carry flag threading
  naturally between them. (FP arithmetic isn't lowered inline; it
  dispatches to runtime helpers via the HARGS block.)
- **Inline arithmetic**: a 1- or 2-byte shift by a variable is a
  loop of one-bit shifts with the count in X (`_shift_loop`), an
  unsigned byte divided by a constant a loop of subtractions
  (`_inline_byte_divide`, one loop for a `/` and a `%` of the same
  operands, and one helper call for such a pair of another width),
  and a 16-bit product of two bytes an 8-by-8 shift-and-add loop
  (`_inline_byte_product`). Each has a document in
  docs/optimizations/ (`shift_loop.md`, `byte_divide.md`,
  `byte_product.md`).
- **HARGS** is the runtime-helper exchange block — 32 bytes of
  zero-page at `$04`–`$23` in the classic layout.
  [docs/architecture.md](docs/architecture.md) gives each helper's
  layout. Caller writes inputs at fixed offsets,
  JSRs the helper, reads outputs from later offsets. The same block
  doubles as the wide-return slot for 4-byte (`long`, `float`) and
  8-byte (`long long`, `double`) return values (`HARGS+8..11` and
  `HARGS+16..23`). HARGS is
  caller-saved, so any HARGS-returned value MUST be captured
  immediately after the JSR.

## asm_emit

`asm_emit.emit_program` — `asm2_ast` → 6502 assembly text. See the
module docstring at the top of `asm_emit.py` for the formatting
rules, the per-atom emit table (Mov / Add / Sub / Compare / ASL /
LSR / ROL / ROR / Inc / Dec / Push / Pop / Xor / And / Or / Call /
Jump / Branch / Label / BitTest etc.), the Data / ZP operand
addressing modes, the self-Mov peephole, and the StaticVariable
init-variant directives (DC.B / DC.W / DC.L / two DC.Ls). One
cross-cutting fact worth keeping at orientation level: every
asm2_ast node maps to exactly one 6502 instruction (addressing-mode
setup like the `LDY` for indirect-Y counts as part of the opcode);
the compound nodes from `asm_ast` (`Ret`, `FunctionPrologue`,
`AllocateStack`) and any leftover `Pseudo` operands are
already-resolved by `asm_to_asm2` and `replace_pseudoregisters`
before emit runs, and emit raises if one slips through.

## Function stack frame (soft stack)

Arguments and locals live on a **soft data stack** in main RAM,
separate from the 6502's hardware stack at `$0100`–`$01FF` (which is
reserved for return addresses and short-lived `PHA`/`PHP`). This
dodges the 256-byte page-1 limit and keeps return addresses out of
the way during frame teardown.

Reserved zero-page: `$00`/`$01` = `SSP` (soft stack pointer, low/
high), `$02`/`$03` = `FP` (frame pointer), `$04`–`$23` = `HARGS`
(32-byte runtime-helper exchange block — see "The helpers' exchange
block" in docs/architecture.md for each helper's slots; the block
is sized for the largest helpers, `udivmod64` / `sdivmod64`, with
16 bytes of inputs + 16 of output), `$24`/`$25` = `DPTR`. `SSP` and `FP` both point at the **next-free
byte** and grow downward. SSP/FP access is always indirect-indexed:
`LDY #off; LDA (SSP),Y` or `LDA (FP),Y`, so `Y` is scratch for any
soft-stack access. HARGS bytes are accessed absolutely (`LDA
HARGS+k` / `STA HARGS+k`); dasm picks zero-page addressing
automatically because the symbol resolves into page 0.

Inside a function `SSP` is unstable (any intra-function push shifts
it). So every function captures `FP` once in its prelude and
addresses args/locals via `FP` — codegen emits `Frame(off)` for those
and the emitter lowers to `LDY #off; LDA (FP),Y`. For `N` arg-bytes
and `M` local-bytes:

- Caller subtracts `N` from `SSP`, writes args at `SSP+1…SSP+N`,
  `JSR`s.
- Callee prelude (skipped when `N+M == 0`): subtract `M+2` from
  `SSP` (locals + saved-FP slot), write caller `FP` into
  `SSP+M+1`/`SSP+M+2`, then `FP = SSP`. Smallest valid `FP` offset
  is `1` (same convention as `SSP`).
- Callee epilogue: `PHA` return value, `SSP = FP + M + N + 2` in
  one 16-bit add, reload caller `FP` via `(FP),Y` (with low byte
  routed through `X` so we don't corrupt the indirect base between
  the two reads), `PLA`, `RTS`.
- When `N+M == 0` the prelude emits nothing and the epilogue
  collapses to `RTS`.

Arg `j` is at offset `M + 3 + j` (not `M + 1 + j`) because the
saved-FP slot sits between locals and args.
[docs/calling_conventions.md](docs/calling_conventions.md) has a frame
diagram and an annotated prologue and epilogue.

## Status

For what works and what's missing, see the README's `## Status`
section and `tests/STATUS.md` (chapter-by-chapter pass/fail). The chapter test
harnesses under `tests/test_chapter_<N>.py` are the authoritative
list of what compiles and runs.

This section captures only the **gaps and known imprecisions** that
an unwary contributor would otherwise discover by surprise.

### Not yet in the repo (programs assemble but won't link)

The runtime library (`passes/runtime_library.py`) has the integer
helpers with 6502 code, `mul8/16/32`, `udivmod8/16/32`,
`sdivmod8/16/32`, `asl8/16/32`, `asr8/16/32`, and `lsr8/16/32`, and
the linker adds the ones a program calls. These aren't in the repo:

- Startup code: setting `SSP`, and a reset vector. The simulator's
  boot stub does both.
- The 64-bit integer helpers: `mul64`, `udivmod64`, `sdivmod64`,
  `asl64`, `asr64`, `lsr64`.
- FP conversion helpers: 26 functions covering
  `{i,u,l,ul,ll,ull}{2f,2d}` and `{f,d}2{i,u,l,ul,ll,ull}` plus `f2d`
  / `d2f`.
- FP arithmetic helpers: `fadd` / `fsub` / `fmul` / `fdiv` and the
  `d`-variants. The ordering helpers `flt` / `fle` / `dlt` / `dle`
  exist as Python hooks in the sim but not yet as 6502 routines.
- `icall` trampoline (`JMP (DPTR)`), which the simulator lays down
  itself. The linker rejects a function with an indirect call
  anyway.

A program that calls one of these assembles but links only when
`--defsym` gives the helper's address, as `sim/linked.py`'s
`defsym_args` does with the simulator's, and the link error names
the helper. Python-implemented hooks in `sim/` cover all of these so
simulation-based tests still pass.

### Type-system limitations

- FP arithmetic (`+`/`-`/`*`/`/` on `float` / `double`) compiles to
  calls of `fadd` / `dmul` / etc., and unary `-` flips the sign bit
  inline. No helper exists as 6502 code; the sim runs them, the
  conversions, and the comparisons `flt` / `fle` / `dlt` / `dle`
  through Python hooks. `==` / `!=` compare bit patterns, so
  `0.0 == -0.0` is false.
- `long double` rejected at parse time (no 16-byte IEEE 754).
- Hex floating literals (`0x1.0p3`) lex but the parser rejects them
  — `float()` doesn't parse C hex-float syntax and the conversion
  isn't wired up.
- Some C99 init-list shapes rejected: scalar init for an array,
  too many initializers, the C99 subaggregate flat form (`int
  a[2][3] = {1,2,3,4,5,6};`), and a member or element of struct type
  initialized by an expression. Brace elision is missing with
  designators too, so a positional item after `.a.x = 1` and a scalar
  for an aggregate (`[0] = 1` for an element of type `int[2]`) are
  rejected, as is a designator that overrides part of a subobject an
  expression or a string literal initializes. An overridden
  initializer with side effects is rejected, and a read of a
  volatile object in an overridden one isn't evaluated.
- Variable-length arrays aren't supported. An array size must be
  an integer constant expression, which the parser folds through
  the type checker and `passes.constant_expression` (the same folder
  case labels and static initializers use), or come from the
  initializer (`int t[] = {1, 2};`, C99 §6.7.8.22; the type checker's
  `_size_from_initializer`). A declaration with a size completes an
  earlier `extern int t[];` (`_composite_array`).

### Codegen imprecisions

- The 8-bit signed `>>` helper (`asr8`) is mostly a placeholder —
  `signed char >>` integer-promotes to int before the shift, so
  `asr16` does the real work; `asr8` exists for completeness.
- Rvalue struct expressions used as lvalues (`f().m = …`, `(c?a:b).m
  = …`) are rejected: the sret slot has temporary lifetime, so
  assigning through it would be a memory-safety hole.

### Where to look for more

- `tests/STATUS.md` — chapter_18 file-by-file status.
- `tests/test_sim_differential.py` — opt-vs-unopt sim differential
  across the full chapter corpus; the `_OPT_DIVERGES` dict at the
  top is the live list of optimizer bugs.
- `tests/test_sim_asm_optimized.py` — chapter_1..12 corpus run
  through `-O` with end-to-end return-value assertions.

## Brain

The `brain/` directory is an Obsidian vault, persistent memory across
sessions (from brainmaxxing, https://github.com/poteto/brainmaxxing).

- **Read first.** Read brain files relevant to your task before acting.
- **Write** after mistakes, corrections, or notable codebase learnings.
- **Structure:** One topic per file. Directories with `[[wikilink]]` indexes, no inlined content.
- **Maintain:** Delete outdated notes and stale artifacts.
