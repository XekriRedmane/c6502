# The C++ port of c6502

This directory holds the port of the compiler from Python to C++.
`docs/cpp_port.md` records the decisions and the order of the stages.
Each stage moves to C++ only when it produces the same output as the
Python code it replaces, byte for byte, and the Python compiler stays
the reference until the port is done.

So far the port has the build, the IR classes generated from the four
`.asdl` files, the dump format that carries an IR value between the
two compilers, the helpers that the asm passes share, every asm pass
of the back end after prologue synthesis, and the work from there to
the `-S` text (`asm_to_asm2`, `asm_emit`, and the steps between them
that read the asm program), each of which matches the Python code on
every input of the corpus it was checked on. With
`C6502_CPP_BACKEND=1`, compile.py's `-S` text comes from C++ ("The back
end through c6502cpp" and "Emission", below).

## Building

The build needs CMake 3.20 or newer and a C++20 compiler. On Windows
it uses the MSVC 2022 Build Tools through CMake's Visual Studio
generator, which finds the Build Tools without `vcvars64.bat`. On this
machine CMake 4.3.1 came with STM32CubeCLT and is on the PATH, and the
Build Tools also install a CMake under
`Common7\IDE\CommonExtensions\Microsoft\CMake`. `uv tool install cmake`
or `winget install Kitware.CMake` installs one elsewhere.

```sh
# Windows (Git Bash or cmd), from the repository's root
cmake -S cpp -B cpp/build -G "Visual Studio 17 2022" -A x64
cmake --build cpp/build --config Release --parallel
ctest --test-dir cpp/build -C Release --output-on-failure

# Linux, with GCC or Clang
cmake -S cpp -B cpp/build -DCMAKE_BUILD_TYPE=Release
cmake --build cpp/build --parallel
ctest --test-dir cpp/build --output-on-failure
```

The executables go in `cpp/build/bin` with either generator, and a
Debug build of the Visual Studio generator puts them in
`cpp/build/bin/debug`. `cpp/build/` is ignored by git. The tests in
`tests/` that use the CLI look for it in `cpp/build/bin`, or at the
path in `C6502_CPP`, and are skipped when it isn't built.

On 19 Nov 2026 the build was clean with MSVC 19.36 (Build Tools 14.36),
and with GCC 12.2 and Clang 14.0.6 on Debian 12, the distribution the
repository's Docker image is based on. The Linux builds ran in WSL,
since the Docker daemon wasn't running, and the Docker image has no
CMake, so it would need `apt-get install cmake` first.

The build makes three targets.

- `c6502`, a static library with all of the compiler's code (`src/`
  and `generated/`). A `.cpp` file added under `src/`, `src/asdl/`,
  `src/passes/`, `src/passes/optimization/`,
  `src/passes/optimization_asm/`, or `src/sim/` is picked up the next
  time CMake runs.
- `c6502_tests`, the unit tests in `tests/`, on doctest.
- `c6502cpp`, the CLI (`src/main.cpp`), which for now holds the checks
  below and the back end that compile.py runs through it.

Warnings are on (`/W4`, `-Wall -Wextra`) and aren't errors yet.

## Layout

```
cpp/
  CMakeLists.txt
  generated/          asdl.py --cpp output, checked in, never edited
    asm_ast.h/.cpp    one header and one source per .asdl module
  src/
    asdl/runtime.*    Int, Box, isa/as, the dump format's Reader/Writer
    errors.h          CompileError, InternalError
    json.*            the reader of the capture harness's JSON
    capture.*         the reader of capture files and their contexts
    helper_checks.*   the C++ side of tools/capture_helpers.py
    backend.*         the `c6502cpp backend` command's request and response
    asm_emit.*        asm_emit.py, the asm2 program as dasm's text
    pyrepr.*          Python's repr and formatting, for messages and text
    ordered_map.h     OrderedMap, a map in the order of insertion, as a dict
    tagged_json.*     the codecs of the compile's tables (step 4)
    stage_checks.*    check-captures' checks of the stage calls
    stage_helper_checks.cpp   the code generation helpers' records
    passes/           one .cpp/.h per Python pass module, mirroring passes/
      peephole_fixedpoint.*   compile.py's _peephole_fixedpoint
      optimized_asm.* the region of compile.py's optimized_asm, and the
                      unoptimized pipeline's fixed point and long branches
      codegen.*       the region through emission, the end of `_codegen`
      asm_to_asm2.*   the lowering to the asm2 IR
      emission_args.* the decoders of the arguments of the work after the
                      region, for the backend command and the registry
      asm_context.*   AsmContext, the compile's state the asm passes read
      asm_operands.h  for_each_operand, a walk over an instruction's operands
      optimization_asm/cfg.*   the asm CFG and its dominators
      optimization_asm/owned_cfg.*   the asm CFG with blocks that own
                      their instructions
      optimization_asm/ssa_*, parallel_copies.*, copy_propagation.*, ...
                      the asm SSA round trip (step 4, group D)
      optimization_asm/liveness.*, interference.*, coalescing.*,
                      hwreg_eligibility.*, regalloc.*, all_operands.*,
                      regalloc_stage.*   the register allocation (step 4,
                      group E), and its codecs and runners
      registry.*      the ported passes, by name, for check-captures
      stage_registry.*  the stage calls' signatures and runners (step 4)
      optimization/   pool, register_allocation, the interference graph, var_visit
      capture_args.*  the decoders of a captured call's keyword arguments
    sim/assembler.*   instruction_size, from sim/assembler.py
    main.cpp          the c6502cpp CLI
  tests/              doctest unit tests, test_<module>.cpp
    asm_text.h        a test's program and expected instructions as dump text
    call_case.h       a recorded call of a function of emission, for the
                      tests recorded from the Python unit tests
  third_party/doctest doctest.h 2.4.12 and its MIT license
```

doctest is the test framework because it is one header of about 320 KB
with no dependencies, which compiles quickly and gave no warnings at
`/W4` or `-Wall -Wextra` with any of the three compilers. Catch2
version 3's amalgamated distribution is a header and a source file
several times that size (a figure from memory, not measured here), and
compile time matters for a suite that will grow to cover every pass.

## The IR classes

`uv run python asdl.py --cpp <name>.asdl cpp/generated` writes
`<name>_ast.h` and `<name>_ast.cpp`, in namespace `c6502::<name>_ast`.
`tests/test_asdl_cpp.py` checks that the checked-in files are what the
generator writes, so an `.asdl` change needs this run for all four
modules.

The names follow the Python modules. Each constructor is a struct with
the constructor's name and its fields as members, in the `.asdl`'s
order (`asm_ast::Mov{src, dst, is_volatile}`). A sum type is
`Type_<name>`, a `std::variant` of its constructors' structs
(`asm_ast::Type_operand`), and a product type is a struct `Type_<name>`
(`c99_ast::Type_var_decl`). A field whose name is a C++ keyword gets a
trailing underscore (`c99_ast::Constant::const_`). An optional field is
a `std::optional` and a sequence a `std::vector`. Every struct is an
aggregate, so a value is built with designated initializers, and a
field left out takes its default, as a keyword argument left out does
in Python:

```cpp
asm_ast::Mov{.src = asm_ast::Data{.name = "x"}, .dst = asm_ast::Reg{.reg = asm_ast::A{}}}
```

A sum type is a variant for the speed of dispatch and comparison. The
value is held inline, so a `std::vector<Type_instruction>` is one block
of memory with no pointer to follow per instruction, and finding the
constructor reads the variant's index. `kind(v)` returns that index as
the enum `Kind_<type>`, whose enumerators are the constructors' names,
for a `switch`. A base class with a kind field and a pointer per node
would cost an allocation per instruction and a pointer chase per test.
`==` on a variant compares the alternative and then the value, which
gives the Python dataclasses' `__eq__` (the same class and equal
fields), and every struct's `operator==` compares all of its fields.

A field whose type is in a cycle with its struct's type (an `exp` in
`c99_ast::Binary`, the `ret` of `tac_ast::FunType`) is an
`asdl::Box<T>`, a pointer with value semantics. Copying a box copies
the value and `==` compares values. A box takes a `T` or any
constructor of `T`, so `ptr.referenced_type = c99_ast::Char{}` works.
`asdl.py` finds the cycles from the strongly connected components of
the fields' types, so the asm IR, which has none, has no boxes.

An ASDL `int` is `asdl::Int`, a signed 128-bit integer (`__int128` on
GCC and Clang, `std::_Signed128` on MSVC). Python ints have no bound,
and the IR holds both values of 64 unsigned bits (a `double`'s bit
pattern in `DoubleInit`, `ConstULongLong`) and negative ones. MSVC's
type converts to the built-in integers only explicitly, so ported code
writes `static_cast<int>(op.offset)`.

A string (`identifier` or `string`) is a `std::string` in UTF-8. A
Python `str` that holds bytes, such as `StringInit.str`, holds them as
the code points 0 to 255, which take two bytes each in UTF-8 from 0x80
up, so code that needs the bytes decodes the string.

## The dump format

`ir_dump.py` describes the format and is its Python side. The C++ side
is the generated `read` and `write` functions with `asdl::Reader` and
`asdl::Writer`, through `asdl::dump(v)` and `asdl::load<T>(text)`. A
dump names the constructor of every value and gives the fields in
order, with each top-level item and each instruction on a line of its
own:

```
Program([
  Function("f" true [] [
    Mov(Imm(1) Reg(A) false)
    Return(true false false)
  ])
])
```

`c6502cpp roundtrip <module> [<file>]` reads a dump and writes it again,
and `tests/test_ir_dump.py` checks that it gives the Python dump's
bytes for the four IRs of a compiled example.

## The captured calls

`tools/capture_passes.py` runs the Python compiler over the corpus
and records each call of the chosen asm passes, with its arguments,
its input program, and its output, under `captures/` (ignored by git).
Its docstring describes the corpus and the file format.
`c6502cpp check-captures <path>...` reads the files under each path.
It checks that each file's dumps read and write back as the same bytes
in C++, and for each pass in `src/passes/registry.cpp` it runs the C++
pass on the recorded input and compares the dump of the result with
the recorded output.

```sh
uv run python tools/capture_passes.py --passes split_mem_to_mem -j 16
uv run python tools/check_captures.py split_mem_to_mem
```

The whole corpus took 73 seconds with 16 processes and gave 7535 calls
of `split_mem_to_mem`, 7049 of them distinct, in 215 MB. Of its 3576
compiles, 1306 fail, which are the chapters' programs that c6502 must
reject or can't compile and 14 fuzz programs whose unoptimized frames
are over the 253-byte limit. A capture of every pass takes much more
space. The examples and the games alone gave 21545 distinct calls of
the 73 passes in about 1 GB, so a capture is best limited to the passes
being ported.

## The compile's context

The Python asm passes read three pieces of state that the driver sets
for a compile in module globals: the runtime's zero-page layout
(`runtime_layout.current()`), the user globals that
`[[c6502::zeropage]]` puts in zero page (`asm_aliasing`'s
`_USER_ZP_GLOBALS` and `_USER_ZP_BYTES`, set by
`set_user_zp_globals`), and the names `may_alias` takes to be out of
reach of a pointer while a pass runs (`asm_aliasing.pointer_proof`).
In C++ they are one value, `passes::AsmContext`
(`src/passes/asm_context.h`), which every asm pass takes as `const
AsmContext& ctx` after the program and passes on to each helper that
reads it.

- `ctx.layout` is a `RuntimeLayout`, whose defaults are the classic
  layout, and `ctx.layout.addr("DPTR")` gives a runtime symbol's
  address as `current().addrs().get(name)` does.
- `ctx.user_zp` holds the zero-page globals by name and the bytes they
  occupy. `ctx.is_user_zp_global(name)`, `ctx.user_zp_address(name)`,
  and `ctx.is_user_zp_byte(addr)` stand for `is_user_zp_global`, the
  lookup in `_zp_data_alias`, and `is_user_zp_byte`.
- `ctx.pointer_proof` is null outside a pass that installs one. A pass
  that runs `with pointer_proof(unreachable_by_pointer(prog)):` makes
  `const AsmContext inner = ctx.with_pointer_proof(names);` and gives
  `inner` to the helpers it calls inside the block.

The tables are held through `shared_ptr<const ...>`, so a copy of a
context costs two reference counts and the tables are never changed
after they are built. A keyword argument that a Python pass is given,
such as `zp_slot_symbols`, stays a parameter of the C++ pass, as a
`const SymbolAddrs*` that may be null where Python's default is
`None`.

A capture of version 2 (`tools/capture_passes.py`) records the
context of each call on its `context` line, and `capture::context(call)`
returns it, which `check-captures` passes to the registered pass.

## The compile's tables

The stages before `apply_licm` (step 4, docs/cpp_port.md) read values
that aren't IR: the symbol table with each symbol's attributes, the
type table of struct and union layouts, the calling conventions
(`abi`), the shares and bindings of `slot_sharing`, the pools, the
colorings of the register allocation, the frames
`replace_pseudoregisters` computes, and the zero-page plan. Each is a
C++ type written by hand after the Python dataclass, with the same
fields in the same order, in the header named for the Python module
that defines it.

| Python | C++ | Header |
|---|---|---|
| `SymbolTable`, `Symbol`, `LocalAttr`, `StaticAttr`, `FunAttr`, the initial values | `SymbolTable`, `Symbol`, `IdAttr`, ..., `ValueTree` | `src/passes/type_checking.h` |
| `TypeTable`, `StructLayout`, `MemberInfo` | the same names | `src/passes/type_checking.h` |
| `ZpLayout`, `SoftStackLayout`, `abi` | `ZpLayout`, `SoftStackLayout`, `ParamLayout`, `Abi` | `src/passes/abi_selection.h` |
| `SlotShare`, the bindings | `SlotShare`, `Bindings` | `src/passes/slot_sharing.h` |
| `Pool`, a `range` | `Pool`, `Range` | `src/passes/optimization/pool.h` |
| `Coloring` | `Coloring` | `src/passes/optimization/register_allocation.h` |
| `FrameDims` | `FrameDims` | `src/passes/replace_pseudoregisters.h` |
| `ZpGlobals`, `ZpObjectMeta` | the same names | `src/passes/zp_globals.h`, `zp_link_metadata.h` |

Emission reads the same types: `place_zp_globals` and `link_records`
take the `ZpGlobals`, `link_records` returns `ZpObjectMeta`s, and the
link metadata's shares are `SlotShare`s.

A Python dict that a pass walks is an `OrderedMap` (`src/ordered_map.h`),
which iterates in the order of insertion as a dict does: the symbol
table, the type table, the ABI, the colorings' `assignments`, and every
map from functions to their values. A set is a `std::set`, since no
pass's output may depend on a Python set's order. A type C++ has no
field for is held with its Python meaning: a c99 type is the generated
`c99_ast::Type_data_type`, an initial value's float is the bits of the
double, and a tuple is a `std::vector`.

These values reach C++ as the capture harness's tagged JSON
(`tools/capture_passes.encode`, which gained `{"float": bits}` for
them). `src/tagged_json.h` has a codec for each form: a struct with a
`type`, a `decode` from a `json::Value` that throws `DecodeError` for a
class or a field the C++ type doesn't hold, and an `encode` that writes
the text Python's `json.dumps(encode(v))` writes, byte for byte. The
codecs of containers take their elements' codecs, so a
`dict[str, list[int]]` is `DictOf<Str, ListOf<IntC>>`, and `SetOf`
writes its elements sorted by their JSON text, as the harness does
(`10` before `9`). A later step that runs the stage in C++ from
compile.py, as `c6502cpp backend` runs the region, can send the tables
in the same form.

### The stage calls

`tools/capture_passes.py --passes stage` records the calls of
`STAGE_PASSES`, the stage's entry points, with their arguments, their
results, and the arguments they changed in place (the module docstring
describes the records). `src/passes/stage_registry.cpp` gives each call
a signature, the codec of each positional and keyword argument and of
the result, and a runner when the call is ported. For each recorded
call `c6502cpp check-captures` reads every value into its C++ type and
writes it back, and requires the text of the record: the input's dump,
the `args` line, the `mutated` line, and the result. For a ported call
it runs the C++ function and compares its result and the arguments it
changed with the record. It reports `<name>: N calls, N round-trip`,
and `M match, 0 differ` after it for a ported one.

```sh
uv run python tools/capture_passes.py --passes stage -j 14
cpp/build/bin/c6502cpp check-captures captures
```

On 20 Nov 2026 the capture of the whole corpus took about 125 seconds
with 14 processes and wrote 1.5 GB, most of it the symbol tables, one
per call. Every value of the 30,273 distinct calls of the 24 entries
made the trip, from 215 calls of `slot_aliases` to 3,798 of
`address_taken_slot_symbols`, and `synthesize_prologue` matched on all
937 of its distinct calls.

### The register allocation

The register allocation of the asm optimizer's `_optimize_function` is
ported, each Python module in a file of its own under
`src/passes/optimization_asm/`: `liveness` (`compute_liveness`, the
`Liveness` snapshot with `live_after` and `live_before`, and
`liveness::defs_in` and `uses_in`), `interference`
(`build_interference`), `coalescing` (`coalesce_moves` and
`CoalesceResult`), `hwreg_eligibility` (`scan_function`,
`HwRegEligibility`, and `hwreg_eligibility::ssa_base`), and `regalloc`
(`color_graph`). The `InterferenceGraph` is in
`src/passes/optimization/interference.*`, and `register_allocation`'s
`_blocked_bytes` and `_find_fit` are
`register_allocation::blocked_bytes` and `find_fit` beside `Coloring`.
`optimizer._all_operands_in`, which regalloc reads, is
`optimizer::all_operands_in` in `all_operands.h`.

A `Liveness` keeps the CFG of step 2, whose blocks are index ranges
into the function, so the function has to outlive it. The graph holds
its nodes and its adjacency in maps sorted by name, and a
`CoalesceResult` its `byte_offset`, where Python's dicts follow the
order in which a walk over Python sets added to them. Nothing reads
that order. The packed BCD reads, writes, and early writes come from
`decimal_arith`'s `early_write_pairs`, `pseudo_read_ptrs`, and
`pseudo_write_ptrs`, the last two `pseudo_reads` and `pseudo_writes` as
pointers into the instruction, which the liveness reads at every
instruction.

The harness records the five entry points under `compute_liveness`,
`build_interference`, `coalesce_moves`, `hwreg_eligibility`, and
`color_graph`, each a call that `_optimize_function` makes on one
function in SSA form, whose dump the record holds as its input
(`stage_checks.cpp` reads a `Function(` dump as a top-level item).
`_CAPTURE_FORMS` in `tools/capture_passes.py` gives three classes
fields of their own: a `Liveness` is recorded as the shape of its CFG
(each block's id, number of instructions, and edges) and its sets by
block, without the blocks' copies of the instructions or the cache of
per-instruction sets, and a graph and a `CoalesceResult` with their
dicts sorted. A runner that takes a `Liveness` checks that the
recorded shape is the CFG `build_cfg` gives the function and takes
the sets from the record. `src/passes/optimization_asm/regalloc_stage.*`
holds the codecs and the runners.

On 20 Nov 2026 the capture of the whole corpus took 113 seconds with 16
processes and wrote 241 MB, and every distinct call matched:
3,591 of `compute_liveness`, 3,668 of `build_interference`, 3,591 of
`coalesce_moves`, 3,601 of `hwreg_eligibility`, and 6,446 of
`color_graph`. The corpus reaches some paths rarely (2 colorings hold a
packed BCD chain, 8 a `register` local, and 13 a spill, and none raises
`RegallocError`), so `tests/test_regalloc.cpp` ports the direct calls of
the Python tests of coalescing and of the eligibility and checks the
coloring of those paths against what the Python functions gave.

## The shared helpers

These Python modules, which the asm passes from `apply_licm` on
import, are ported with only the parts those passes use.

| Python | C++ | Namespace |
|---|---|---|
| `passes/asm_liveness.py` | `src/passes/asm_liveness.*` | `passes` |
| `passes/asm_aliasing.py` | `src/passes/asm_aliasing.*` | `passes` |
| `passes/decimal_arith.py` (`reads`, `writes`, `operands`, `writes_y`, `map_operands`) | `src/passes/decimal_arith.*` | `passes::decimal_arith` |
| `passes/optimization_asm/cfg.py` | `src/passes/optimization_asm/cfg.*` | `passes` |
| `redundant_load._build_cfg_tolerant` | `src/passes/redundant_load.*` | `passes` |
| `passes/asm_dead_store.py`'s helpers (`mem_dead_after`, `_reads`, `_successors`, ...) | `src/passes/asm_dead_store.*` | `passes::asm_dead_store` |
| `inc_peephole.is_x_indexed`, `_is_inc_eligible_operand`, `_operands_equal` | `src/passes/inc_peephole.*` | `passes`, `passes::inc_peephole` |
| `dead_a_arith._flags_reflect_src_reg`, `_writes_reg_setting_n_z` | `src/passes/dead_a_arith.*` | `passes::dead_a_arith` |
| `passes/call_edges.py` | `src/passes/call_edges.*` | `passes` |
| `sim.assembler.instruction_size` | `src/sim/assembler.*` | `sim` |
| `long_branches._invert` | `src/passes/long_branches.*` (`invert`) | `passes::long_branches` |

The code generation passes of step 4 share these, which are ported
with the parts they use.

| Python | C++ | Namespace |
|---|---|---|
| `replace_pseudoregisters.sizeof`, `size_of_name` | `src/passes/replace_pseudoregisters.*` (`sizeof_type`, since `sizeof` is a keyword) | `passes` |
| `zp_slot_naming.register_param_local`, `source_spelling`, `parse_pseudo_name`, `param_slot_symbols` | `src/passes/zp_slot_naming.*` | `passes` |
| `optimization/var_visit.py` (`vals_in`, `uses_in`, `defs_in`, `count_uses`) | `src/passes/optimization/var_visit.*` | `passes` |
| `slot_sharing.reach_sets`, `coexist`, `share_conflicts`, `SlotShare`'s methods | `src/passes/slot_sharing.*` | `passes` |
| `optimization/pool.py`, `RuntimeLayout.bytes` | `src/passes/optimization/pool.*`, `src/passes/asm_context.*` | `passes` |

`uses_in` and `defs_in` return the names as `std::string_view`s into
the instruction, since a `tac_ast::Var` is only its name, and
`share_conflicts` returns the indices of the shares where Python
returns the shares.

A public Python name keeps its name in `c6502::passes`. A name that
was private to its module (a leading underscore) and that other
modules import goes in a namespace named for the module, without the
underscore, and so do `decimal_arith`'s, which Python always writes
with the module's name in front (`decimal_arith.reads`). The
exception is `asm_aliasing._is_zp_symbol` and `_instruction_operands`,
which became `passes::is_zp_symbol` and `instruction_operands`.

Some of the Python helpers keep caches that C++ does without.

- **The label map.** Every walk of `asm_liveness` and `asm_dead_store`
  finds jump targets through a map of label indices, and Python keeps
  the maps of the last eight lists it was given. In C++ each walk has
  an overload that takes a `LabelMap`, and one that builds it. A pass
  that asks many questions of one list builds `LabelMap labels(instrs)`
  once, and builds it again after it changes the list.
- **The names of each function.** `unreachable_by_pointer` keeps the
  names each function's instructions call, take the address of, and
  read, under the ids of the instructions. C++ computes them each time
  (`function_names`). A pass that calls it often on mostly unchanged
  programs may want a cache keyed by the function's index and a
  generation count, since C++ has no object identity to key on.

The walks take the instructions as `passes::Instrs`, a
`std::span<const asm_ast::Type_instruction>`, so a pass can pass a
function's vector or a part of one. A `Cfg` keeps index ranges into
the function's instructions where the Python CFG copies them into its
blocks, so the function has to outlive the CFG and stay unchanged
while it is used. `cfg.instructions(id)` gives a block's span. The
synthetic `Label; Return` blocks that `build_cfg_tolerant` adds for
targets outside the function live in `cfg.extra`. SSA construction and
destruction, which rewrite blocks in place, use `OwnedCfg`, whose
blocks own their instructions ("The asm SSA round trip").

`sim/assembler.py` stays in Python with the simulator, but
`instruction_size` is the compiler's, since six passes size code with
it. It throws `InternalError` with Python's message (`AssemblerError:
...`) for an instruction the assembler can't encode.

## Checking the helpers

Helpers aren't passes, so they are checked on the inputs the passes
see. `tools/capture_helpers.py` compiles a corpus (by default the
examples, the games, and the fuzz seeds, in both modes), wraps every
pass of the capture harness's table, and evaluates each helper on each
function the passes are given, in the context of the call: at every
instruction index for the walks, on every instruction for the
predicates, on every operand the function contains, and on every pair
of operands for `may_alias` and `_operands_equal`. Its docstring
describes the records and `function_values` the encoding of each
value. `c6502cpp check-helpers <path>...` computes the same values in
C++ (`src/helper_checks.cpp`) and compares them with the records,
reporting the number of values compared for each helper.

```sh
uv run python tools/capture_helpers.py -j 15
cpp/build/bin/c6502cpp check-helpers captures/helpers
```

On 19 Nov 2026 the capture of the default corpus (712 compiles, 14 of
which fail as they do for `capture_passes.py`) took about 15 minutes
with 15 processes and wrote 48786 function records, 4064 program
records, and 1827 TAC records in 2.7 GB. `check-helpers` took 9.5
minutes in one process and compared about 2.1 billion values with no
difference. Most of them are the matrices over pairs of operands: 255
million values each for the three `may_alias` contexts and for
`_operands_equal`, and 200 million each for the three forms of `_reads`
and for `_writes_same`. Every walk and every predicate of
`asm_liveness` compared 7.6 million values, `mem_dead_after` 5.2
million in each context, `unreachable_by_pointer` 80167 names, and
`call_edges` 46308 instructions. 23 of the functions hold a
`DecimalArith`, and 39611 records have user zero-page globals in their
context (Lode Runner's).

A helper that a pass reads should be added to both sides when it is
ported, so the check covers it.

The code generation helpers have records of their own, which
`capture_helpers.py --stage-only` writes without the others. A `tables`
record holds the TAC program, the tables, and the shares each call of
`find_shares` sees, and gives each helper's values over every symbol,
member, TAC instruction, function, pair of functions, and share
(`stage_values`). A `pseudos` record holds the pseudo names of each
program `replace_pseudoregisters` is given, with the names of the
colorings, and gives `parse_pseudo_name` of each. `check-helpers`
computes them in `src/stage_helper_checks.cpp`. On 20 Nov 2026 the
whole corpus gave 1,110 `tables` records and 1,812 `pseudos` records in
62 MB, with no difference among about 1.04 million values: 190,396 each
for `size_of_name` with and without the type table and for
`source_spelling`, 133,093 for `parse_pseudo_name`, 69,990 each for
`vals_in`, `uses_in`, and `defs_in`, 33,493 for `coexist`, and from
1,117 to 41,173 for the others.

## Porting a pass

These conventions apply to every pass ported after
`split_mem_to_mem`, which is the model.

- **Files and names.** The Python module `passes/<name>.py` becomes
  `src/passes/<name>.h` and `.cpp`, and `passes/<dir>/<name>.py`
  becomes `src/passes/<dir>/<name>.*` (add the directory to
  `CMakeLists.txt`'s glob). Code is in namespace `c6502::passes`, and
  each function keeps its Python name in snake_case (`apply_<name>`,
  `rewrite_function`), with private helpers in an anonymous namespace
  and a leading underscore dropped. Types are CamelCase, constants
  `kName`. An IR module is aliased as `namespace ir = ::c6502::asm_ast;`.
- **Signatures.** A pass takes the program by value and returns it,
  `asm_ast::Program apply_x(asm_ast::Program prog, const AsmContext&
  ctx, ...)`, and rewrites a function in place only when it changes
  something, so a caller that moves the program in copies nothing. A
  pass that reads none of the context, such as `split_mem_to_mem`,
  leaves the parameter out. Keyword arguments become parameters in the
  same order, with the Python defaults.
- **Comments.** The module's docstring, rewritten in the project's
  prose rules (full sentences, no "X, not Y"), opens the `.cpp` as a
  block comment, and keeps every reason and every example of a bug the
  pass once had. The header gets a short comment on the entry point.
  Each function's docstring becomes the comment above it, opening with
  a third-person verb, and each comment inside a body stays next to
  the code it explains.
- **isinstance.** `isinstance(x, C)` is `asdl::as<ir::C>(x)`, which
  returns a pointer or null, or `asdl::isa<ir::C>(x)` when only the
  answer is needed, and `isinstance(x, (C, D))` is
  `asdl::isa<ir::C, ir::D>(x)`. A chain of `isinstance` tests on one
  value with more than three branches is a `switch (ir::kind(x))`.
  `==` between IR values compares as Python's does.
- **Dispatch on several constructors with a switch over `kind`.** On
  20 Nov 2026 the Release build of MSVC 19.36 ran a helper of
  `asm_remat` written as `std::visit` with a generic lambda, which set
  a captured `bool` from the `dst` of the constructors an
  `if constexpr` chain named, as if it returned false for a `Sub` into
  A, and the pass matched Python on 5479 of 6783 calls. GCC 12.2 ran
  the same source correctly, and so did MSVC once the helper was a
  `switch (ir::kind(instr))` with a `std::get` in each case. The
  passes ported that day dispatch that way. The `std::visit` version
  matched on every call in MSVC's Debug build, and in a Clang build
  with `-fsanitize=address,undefined`, which reported nothing, so the
  fault is in MSVC's optimizer and not in undefined behavior of the
  pass. Building it with a second copy of the helper beside it, to
  compare the two, made the difference disappear.
- **Python's arithmetic.** C++'s `/` and `%` round toward zero, so
  Python's `//` and `%` on values that can be negative are
  `asdl::floor_div` and `asdl::floor_mod`. A Python dict iterates in
  insertion order, so a map that a pass iterates is a vector of pairs
  or an insertion-ordered map, never `std::unordered_map`. A Python
  set's iteration order depends on hashing, and a pass whose output
  depends on one is a bug to fix in Python first.
- **Identity.** The C++ IR has value semantics, so a Python test of
  `is` between IR nodes, or a cache keyed by `id()`, needs another key,
  such as the instruction's index.
- **Errors.** A Python exception that `compile._source_errors` lists
  (a problem with the program) is thrown as
  `c6502::CompileError("<Python type>", "<the same message>")`, and an
  `AssertionError` or `IRCheckError` as `c6502::InternalError`
  (`src/errors.h`). The messages are the Python ones, word for word,
  since the tests compare them.
- **Registration and checks.** Each ported pass gets an entry in
  `src/passes/registry.cpp` under its name in
  `tools/capture_passes.py`'s `PASSES`, with a function that takes the
  call's context, decodes its tagged-JSON arguments, and returns the
  result's dump. The
  Python tests of the pass (`tests/test_<name>.py`) are ported to
  `tests/test_<name>.cpp`, where `tests/asm_text.h` builds a test's
  program from the dump format and compares the instructions as dump
  text, so a failure shows both listings. A keyword argument such as
  `zp_slot_symbols` is decoded with `src/passes/capture_args.h`. A pass
  is done when `tools/check_captures.py <name>` reports no difference
  on a capture of the whole corpus.
- **The stage calls.** A call of step 4 that takes the compile's tables
  is registered by giving its entry in `src/passes/stage_registry.cpp`
  a `run`, which decodes the arguments with the codecs of
  `src/tagged_json.h` and returns the result's dump or tagged JSON and,
  for a call that changes an argument in place, the `mutated` line it
  would record. `synthesize_prologue` is the model.

## The control-flow and whole-program passes

The branch and jump peepholes of the fixed point (`dead_label_drop`,
`unreachable_drop`, `branch_invert`, `branch_to_next_drop`,
`branch_through_jump`, `branch_thread`, `branch_value_thread`,
`tail_call`, `tail_merge`, `cross_jump`) and the passes after it
(`apply_slot_coalescing`, `apply_ret_slot_coalescing`,
`apply_jump_tables`, `apply_copy_through_index`,
`expand_long_branches`, `apply_short_jumps`, `apply_shared_tails`,
`apply_outline_runs`) each have a file of their own under
`src/passes/`. Their registry entries are in
`src/passes/registry_control_flow.*`, and their tests in
`tests/test_control_flow_passes.cpp`, with `tests/asm_text.h`, which
builds a test's program from the dump format. These pieces are shared
among them.

- `next_label_number` (`src/passes/label_numbers.*`) gives the number
  after the largest of a label family such as `.thread@N` in the
  program, which `branch_thread`, `branch_value_thread`, `cross_jump`,
  and `jump_table` start their counters from, as the Python passes do
  with a regular expression.
- `for_each_operand_mut` (`src/passes/operand_rewrite.h`) visits every
  operand of an instruction so that it can be changed in place, which
  is what the generic `_rename` of `slot_coalescing` and
  `ret_slot_coalescing` does over a dataclass's fields.
- `long_branches::invert` gives a branch's opposite condition, as
  Python's `_invert` does.
- `slot_coalescing::excluded`, `successors`, and `copy_sources` are the
  helpers `ret_slot_coalescing` imports.

`apply_copy_through_index` is in `src/passes/copy_through_index.*`,
since the other pass of `via_a_store_fold.py` belongs to the sweep and
was ported apart from it.

The Python passes that key by `repr` (`tail_merge`'s tails,
`shared_tails`' shapes and normal forms, `outline_runs`' runs) key by
the dump in C++, which is equal for two values exactly when the
`repr`s are. `shared_tails` and `outline_runs` add each function they
make to the symbol table Python passes them. The C++ passes report
each one as a `NewFunction`, the new name and the function whose
symbol it copies, for the caller to do the same.
`apply_slot_coalescing` returns the program and the slots each
function now writes, as Python's tuple does, and its registry entry
writes the pair as the tagged JSON of the capture (`json_quote` writes
a string as Python's `json.dumps` does). `branch_value_thread` keeps
what it knows of a byte in the order it learned it, where Python keeps
a set: two different equalities for one byte can only be known on a
path that no run takes, and the corpus has no call where the order
changes the result.

## The back end through c6502cpp

`passes::peephole_fixedpoint` (`src/passes/peephole_fixedpoint.*`) is
compile.py's `_peephole_fixedpoint`: `peephole_sweep` lists the passes
of the sweep in its order, each with its keyword arguments bound, and
`run_to_fixed_point` repeats the sweep until a sweep leaves the program
equal to what it started with, giving up with Python's message after
16 sweeps. The Python memo (`passes/peephole_memo.py`) gives the
results a run without it gives, so the C++ fixed point has none.
`c6502cpp sweep-passes` lists the sweep, and `tests/test_cpp_backend.py`
checks the list against `tools/capture_passes.SWEEP_PASSES`, which
`tests/test_capture_passes.py` checks against compile.py.

`passes::optimized_region` (`src/passes/optimized_asm.*`) runs the
passes of `optimized_asm` from `apply_licm` through
`apply_outline_runs`, with Python's arguments, on the program after
`synthesize_prologue`. Its `RegionArgs` are the values compile.py
computes for those passes: `all_slot_symbols`, `zp.addrs`, `noreturn`,
what `apply_slot_coalescing` reads of `abi` and `shares`, the set
`_ret_coalescing_eligible` returns, and the zero-page names and slot
addresses of the last two passes. It returns the program, the slots
`apply_slot_coalescing` made each function write, and the functions
`apply_shared_tails` and `apply_outline_runs` made, each with the
function whose symbol it copies. `passes::unoptimized_region` is the
unoptimized pipeline's fixed point and long branches.

`c6502cpp backend` (`src/backend.*`) reads a request on stdin: the mode
(`optimized` or `unoptimized`), the compile's context as the capture
harness's `context` line gives it, the arguments as an object in the
harness's tagged JSON, and the program's dump. It writes the slots, the
functions made, and the result's dump. The arguments are decoded with
the same `capture_args` functions as the captured calls. A bug the
passes report (`InternalError`) exits with status 3.

With `C6502_CPP_BACKEND=1`, compile.py's `_codegen` hands the region
and everything after it to `c6502cpp backend` through `cpp_backend.py`,
one process per compile, and the `-S` text comes back ("Emission",
below). The simulator harness calls `optimized_asm` and
`compile.unoptimized_tail`, which hand only the region to C++ and go
on with the result in Python, since the harness assembles the asm
program. `cpp_backend.py` gives each function made a copy of its
owner's symbol with internal linkage, as the Python passes do, and
raises an exit status of 3 as an `AssertionError` with the C++
message. Under `C6502_CHECK_IR=1`, compile.py checks the result once,
since `ir_check` isn't ported and the checks after each sweep happened
in Python. compile.py reads the variable at each compile and, without
it, imports none of this.

`tools/compare_backends.py` compiles the corpus of the capture harness
with `-S` and `-S -O`, once with the variable and once without, and
requires the same `-S` text, exit status, messages, and exceptions.

## Emission

`passes::lower_to_asm2` (`src/passes/asm_to_asm2.*`) and
`asm_emit::emit_program` (`src/asm_emit.*`) are `asm_to_asm2` and
`asm_emit`, and `passes/y_peephole.py`, which `emit_program` runs on its
lines, is `passes::apply_y_peephole`. The work between the region and
emission is in a file per Python module: `place_zp_globals` and
`link_records` in `src/passes/zp_globals.*`, with `hargs_need` and
`check_hargs` of passes/runtime_layout.py; `check_shares` in
`src/passes/slot_sharing.*`; `prune_unused_locals` in
`src/passes/prune_unused_slots.*`; `items` as `section_items` in
`src/passes/sections.*`; and the records of the link metadata with
`format_metadata` in `src/passes/zp_link_metadata.*`.
`passes::optimized_text` and `unoptimized_text` (`src/passes/codegen.*`)
run the region and all of these in compile.py's order and return the
text, which `c6502cpp backend` gives in its `optimized-text` and
`unoptimized-text` modes (`src/backend.h`).

The boundary between the two compilers follows what each function
reads.

- **In C++**: everything that reads the asm program after the region.
  `link_records`, `place_zp_globals`, `check_shares`,
  `prune_unused_locals`, `hargs_need`, and `section_items` read the
  finished program, and `asm_to_asm2` and `asm_emit` turn it into the
  text, so running them in Python would mean reading the program back.
  `format_metadata` is in C++ too, since three of its kinds of records
  (the items, the zero-page objects, and the HARGS need) and the
  `changes` of each `def` (`compile._with_coalesced`, which adds the
  slots `slot_coalescing` made each function write) come from the
  region and the finished program.
- **In Python, computed before the call**: `build_metadata` reads the
  TAC and the ABI layouts, and `changed_param_slots`,
  `_carry_functions`, and `preferred_symbols` read the TAC, the symbol
  table, and the bindings of `find_shares`. Their results go in the
  request, as the `LinkMetadata` without its items, zero-page records,
  and HARGS need. `section_items` reads only three things of a
  top-level name's symbol, its section, whether its type is `const`,
  and whether it is mergeable, which `sections.section_info` gives for
  each top-level name of the program sent. A function that
  `shared_tails` or `outline_runs` makes takes its owner's, as its
  Python symbol is a copy of the owner's. The HARGS footprints of the
  runtime helpers (`runtime_layout.helper_footprints`) are measured
  from rtlib/helpers.py, which stays in Python with the simulator, so
  they go in the request too. The `ZpGlobals` of the unit, which
  `plan_zero_page` computes from the TAC, go as they are.

Python reads the response's text and nothing else. Under
`C6502_CHECK_IR=1` the response also holds the finished asm program and
the asm2 program, which Python checks with `ir_check.check_asm` and
`check_asm2`, since `ir_check` isn't ported. A problem with the program
that a function of emission reports (a `FrameSizeError`, a
`RuntimeLayoutError`, or the `ValueError` of an operand the 6502 can't
encode) is a `CompileError` with its Python type, which `c6502cpp
backend` writes on stderr with exit status 4, and `cpp_backend.py`
raises the Python exception of that type with the message.

The simulator harness assembles the asm program in Python, so it still
calls `optimized_asm` and `unoptimized_tail`, whose region alone runs
in C++ with the variable set (the `optimized` and `unoptimized` modes).

The messages of the ported functions quote IR values and strings as
Python's `repr` does (`src/pyrepr.*`), and the hexadecimal and signed
forms of the text are Python's format specifications, so the text and
every message are the Python ones. A `StringInit` holds its bytes as
code points, which `pyrepr::code_points` decodes from the UTF-8.

The capture harness records the calls of these functions under their
names in compile.py (`link_records`, `place_zp_globals`,
`check_shares`, `prune_unused_locals`, `lower_to_asm2`, `hargs_need`,
`section_items`, `format_metadata`, and `emit_program`). A capture
whose first argument isn't an IR value (`format_metadata`'s
`LinkMetadata`) holds its tagged JSON under `input-json`, and a
registry entry for a function whose input isn't an asm program sets
`run_text` and the module of its dumps (`input_module`,
`output_module`). `section_items` is recorded with what
`section_info` gives in place of the symbol table, since a table can
hold floats, which the C++ JSON reader takes no fractions of, and
`hargs_need` and `place_zp_globals` with the footprints as a keyword
argument. The unit tests in `tests/test_asm_emit.cpp`,
`test_asm_to_asm2.cpp`, `test_y_peephole.cpp`, and
`test_emission_work.cpp` are the calls that the Python unit tests make
of these functions, with the results and the exceptions they gave,
recorded by wrapping the Python functions while those tests ran.

## The asm SSA round trip

The passes of `_optimize_function` around the register allocation are
each in a file under `src/passes/optimization_asm/`, named for the
Python module: `to_ssa` (`ssa_construction.*`), `copy_propagate`,
`backward_copy_propagate`, `byte_dce`, `absorb_zero_load`
(`or_zero_absorb.*`), and `from_ssa` (`ssa_destruction.*`). Each takes
one asm function and the static names (`StaticNames`, the set codec's
type) where the Python one takes `statics`. These pieces are shared
with the other groups of step 4.

- `order_parallel_copies` (`parallel_copies.h`) is
  `ssa_destruction._order_parallel_copies`, which `tac_to_asm` calls
  for the arguments of a call that go in the callee's slots. The
  Python list of one int that counts the temporaries is an `int&`.
- `OwnedCfg` (`owned_cfg.h`) is the asm CFG with a vector of
  instructions per block, which SSA construction and destruction edit
  in place. Its `graph` is a `Cfg` whose instruction ranges are unused,
  so the dominator functions of `cfg.h` take it unchanged.
- `excluded_pseudo_names` (`ssa_construction.h`) gives the names every
  pass of the round trip leaves unversioned.
- `decimal_arith.h` has `pseudo_reads`, `pseudo_writes`, `map_reads`,
  `map_writes`, `later_sources`, and `early_write_pairs`.
- `ssa_operands.h` has the two walks over an instruction's operands
  that the passes share, all of them and those read.

The Python passes keep pairs of a name and a byte in sets, which C++
keeps in ordered sets, since their order changes no result. SSA
construction places a `Phi` for each pair in the order of the pair's
first definition, which is the order of the Python dict, and so the
order of the `Phi`s in a block and of the moves `from_ssa` writes.

`tools/capture_passes.py` records the seven calls as stage calls under
these names, with each function's dump as the input (`stage_checks`
reads a dump that starts with `Function(` as a top-level item), and
`order_parallel_copies` with its list of moves as tagged JSON and the
counter on the `mutated` line when a cycle took a temporary. `from_ssa`
renames the predecessor labels of its input's `Phi`s in place, which
its caller never reads again, so the harness gives it a copy
(`_COPIES_INPUT`). The tests in `tests/test_asm_ssa_round_trip.cpp`
are the calls that the Python unit tests of the six modules make, and
the calls on a function with a `DecimalArith` that
`tests/test_bcd_builtins.py`'s compiles make, recorded with the
Python results through `tests/stage_case.h`.

## Status

- 19 Nov 2026: the groundwork landed. The CMake build with MSVC, GCC,
  and Clang; doctest; the C++ IR generated from all four `.asdl`
  files; the dump format on both sides, with the Python round trip and
  the Python-to-C++-to-Python byte check on a compiled example's four
  IRs; the capture harness for the passes of `optimized_asm` from
  `apply_licm` on; and `split_mem_to_mem`, which matches on all 7049
  distinct calls of the corpus.
- 19 Nov 2026: the helpers that the asm passes share, which are
  `asm_liveness`, `asm_aliasing`, the parts of `decimal_arith` the asm
  passes use, the asm CFG and `redundant_load`'s tolerant builder,
  the helpers other passes import from `asm_dead_store`,
  `inc_peephole`, and `dead_a_arith`, `call_edges`, and
  `sim.assembler.instruction_size`. The compile's state that Python
  keeps in module globals is `AsmContext`, and pass captures record it
  (version 2). `tools/capture_helpers.py` and `c6502cpp
  check-helpers` compare every helper on the corpus, about 2.1 billion
  values with no difference.
- 20 Nov 2026: the twelve passes of the loops and the index
  registers: `apply_licm`, `promote_loop_carried_to_a`,
  `loop_counter_to_x`, `counter_to_index_reg`, `x_save_slot_load`,
  `dual_index_promotion`, `loop_invariant_a`, `rotate_loop_test`, and
  the peephole fixed point's `element_in_place`, `loop_constant_a`,
  `dec_register_peephole`, and `index_inc_cmp_y`. Each matches the
  Python pass on every distinct call of the corpus, from 926 calls of
  most of the pipeline's passes to 7051 of `element_in_place`.
  `rotate_loop_test` reads `long_branches._invert` as
  `passes::long_branches::invert`.
- 20 Nov 2026: the control-flow and whole-program passes: the ten
  branch and jump peepholes of the fixed point from `dead_label_drop`
  to `cross_jump`, slot coalescing for parameters and for `F@ret`, jump
  tables, copies through an index register, long branches, short
  jumps, shared tails, and outlined runs. Each matches the Python pass
  on every distinct call of the corpus.
- 20 Nov 2026: the dataflow passes of the peephole fixed point,
  `redundant_load`, `memory_value_propagation`, `remat`,
  `asm_dead_store`, `redundant_store`, and `mem_const_prop`, each in
  `src/passes/<module>.*`. Each matches the Python pass on every
  distinct call of the corpus. Their tests (`tests/test_<module>.cpp`)
  are the calls that the Python unit tests make, recorded with the
  Python pass's results and checked through `tests/pass_case.h`.
- 20 Nov 2026: the arithmetic, carry, and load/store peepholes of the
  fixed point (group C of step 2b), 36 entries of the registry:
  `inc_peephole`, `inc_low_byte_test`, `carry_inc`, `dec_peephole`,
  `sub1_test_zero`, `carry_halve`, `carry_in_add`,
  `carry_recompute_drop`, `carry_known`, the three passes of
  `carry_return`, `const_arith_fold`, `label_offset_fold`,
  `shift_in_a`, `dead_a_arith`, `redundant_load_after_rmw`,
  `dec_inc_branch_fold`, `cmp_sbc_fusion`, `adc_commute` (both
  stages), `transfer_pm1_store`, `round_trip_load_drop`,
  `self_store_drop`, `pointer_self_store_drop`, `and_sign_bit_branch`,
  `cmp_through_tmp_fold`, `via_a_store_fold`, `direct_index_load`,
  `cpx_cpy`, `dead_pha_pla`, `volatile_void_read_cmp`,
  `dec_branch_indirect_y_fold`, `transfer_past_branch`,
  `constant_store_order`, `join_store`, and `redundant_compare`. Each
  matches the Python pass on every distinct call of the corpus, from
  1180 calls of `carry_known` to 7052 of `carry_halve`.
  `src/passes/capture_args.*` decodes the keyword arguments of a
  captured call, and the unit tests write their programs in the dump
  format with `tests/asm_text.h`.
- 20 Nov 2026: the driver of the back end's region,
  `passes::peephole_fixedpoint` and `passes::optimized_region`, the
  `c6502cpp backend` and `sweep-passes` commands, and
  `C6502_CPP_BACKEND=1` in compile.py ("The back end through
  c6502cpp"). `tools/compare_backends.py` found the same output for
  all 3,576 compiles of the corpus with and without the variable, and
  the suite and the four fuzzers pass with it set. The build is clean
  with MSVC 19.36, GCC 12.2, and Clang 14.0.6.
- 20 Nov 2026: emission ("Emission"): `lower_to_asm2`, `asm_emit`
  with its Y peephole, and the work between the region and emission
  that reads the asm program, run with the region by
  `passes::optimized_text` and `unoptimized_text`. With
  `C6502_CPP_BACKEND=1` the `-S` text comes from `c6502cpp backend`.
  Each function matches the Python one on every distinct call of the
  corpus, 11,580 calls in all, from 855 of `format_metadata` to 2,058
  of `place_zp_globals`, and `tools/compare_backends.py` found the same
  output for all 3,576 compiles. The build is clean with MSVC 19.36,
  GCC 12.2, and Clang 14.0.6.
- 20 Nov 2026: the groundwork of step 4. The compile's tables are C++
  types with codecs for the capture harness's tagged JSON ("The
  compile's tables"), the harness records the 24 entry points of the
  stage, and every value of their 30,273 distinct calls on the corpus
  reads into C++ and writes back unchanged. The helpers the stage's
  groups share are ported and match on about 1.04 million values, and
  `synthesize_prologue` matches on all 937 of its distinct calls.
- 20 Nov 2026: the asm SSA round trip (group D of step 4): `to_ssa`,
  `copy_propagate`, `backward_copy_propagate`, `byte_dce`,
  `absorb_zero_load`, `from_ssa`, and `order_parallel_copies`, with the
  asm CFG with owned blocks and `decimal_arith`'s Pseudo helpers ("The
  asm SSA round trip"). Each matches the Python function on every
  distinct call of the corpus, from 3,670 calls of `to_ssa` to 8,499 of
  `order_parallel_copies`.
  A bug found on the way was fixed in both compilers. When a block ended
  in a branch to the label right after it, and that label's block had
  `Phi`s, `from_ssa` gave both of each `Phi`'s arguments from it to the
  split of the taken edge, so the path that fell through skipped the
  moves. No program of the corpus has that shape.
- 20 Nov 2026: the register allocation of the asm optimizer (group E of
  step 4, "The register allocation"): liveness, interference,
  coalescing, the eligibility for X and Y, and the coloring, with
  `InterferenceGraph`, `blocked_bytes`, `find_fit`, and
  `all_operands_in`. Each entry point matches the Python function on
  every distinct call of the corpus, 20,897 calls in all.
