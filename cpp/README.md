# The C++ port of c6502

This directory holds the port of the compiler from Python to C++.
`docs/cpp_port.md` records the decisions and the order of the stages.
Each stage moves to C++ only when it produces the same output as the
Python code it replaces, byte for byte, and the Python compiler stays
the reference until the port is done.

So far the port has the build, the IR classes generated from the four
`.asdl` files, the dump format that carries an IR value between the
two compilers, the helpers that the asm passes share, and all of code
generation from the type-checked c99 program to the `-S` text:
`c99_to_tac`, the inliner and the other passes over the whole TAC
program, the TAC optimizer, the zero-page plan, the ABI, the
allocators, `tac_to_asm`, the asm optimizer, every asm pass of the back
end, and emission, each of which matches the Python code on every input
of the corpus it was checked on. With `C6502_CPP_BACKEND=1`, compile.py
hands the source text of a C input to C++, which runs the front end and
code generation, and takes the `-S` text back ("The stage's driver",
"The back end through c6502cpp", and "Emission", below). Of the front end, the pieces its passes share, the
preprocessor, the parser, the unroller, `identifier_resolution`,
string lifting, `label_resolution`, `loop_labeling`, and the type
checker are ported ("The front end's calls").

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
  below and the code generation that compile.py runs through it.

Warnings are on (`/W4`, `-Wall -Wextra`) and aren't errors yet.

## Layout

```
cpp/
  CMakeLists.txt
  generated/          asdl.py --cpp output, checked in, never edited
    asm_ast.h/.cpp    one header and one source per .asdl module
  src/
    asdl/runtime.*    Int, Box, isa/as, the dump format's Reader/Writer,
                      py_repr
    errors.h          CompileError, InternalError
    json.*            the reader of the capture harness's JSON
    capture.*         the reader of capture files and their contexts
    helper_checks.*   the C++ side of tools/capture_helpers.py
    backend.*         the `c6502cpp backend` command's request and response
    asm_emit.*        asm_emit.py, the asm2 program as dasm's text
    pyrepr.*          Python's repr and formatting, for messages and text
    ordered_map.h     OrderedMap, a map in the order of insertion, as a dict
    py_int_set.h      PyIntSet, a set of ints that iterates as CPython's does
    tagged_json.*     the codecs of the compile's tables (step 4)
    stage_checks.*    check-captures' checks of the stage calls
    stage_helper_checks.cpp   the code generation helpers' records
    tac_helper_checks.cpp     the TAC optimizer's helpers' records
    passes/           one .cpp/.h per Python pass module, mirroring passes/
      peephole_fixedpoint.*   compile.py's _peephole_fixedpoint
      optimized_asm.* the region of compile.py's optimized_asm, and the
                      unoptimized pipeline's fixed point and long branches
      codegen.*       the region through emission, the end of `_codegen`
      optimized_front.*   `_optimized_front` after the TAC optimizer
      codegen_stage.* the stage from the c99 program to the text, which
                      the backend command runs
      translate_for_codegen.*   compile.py's `translate_for_codegen`
                      and `_optimized_tac`
      return_in_zero_page.*   the `F@ret` objects of `static` pointer
                      results (step 5, group H)
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
      optimization_asm/optimizer.*, apply_coloring.*, const_static_fold.*,
                      dead_static.*, optimizer_stage.*   the asm
                      optimizer's driver (step 4, group F1) and its runners
      function_local_sizing.*   the sizes of the pools the driver fills
      local_slot_names.*  the names of a pool's bytes, which
                      apply_coloring and compute_local_slot_names share
      registry.*      the ported passes, by name, for check-captures
      stage_registry.*  the stage calls' signatures and runners (step 4)
      stage_registry_abi.*  the runners of group A's stage calls
      stage_registry_sharing.*  the runners of group B's stage calls
      address_taken_zp.*, local_slot_names.*, lower_data_load_address.*,
                      replace_pseudoregisters.*, zp_slot_naming.*   the
                      end of the stage (step 4, part F2)
      stage_registry_frames.*  the runners of part F2's stage calls
      optimization/   pool, register_allocation, the interference graph, var_visit
      optimization/framework.*, value_range.*, truncate_extend_fold.*,
                      tac_stage.*   the TAC optimizer's run loops, helpers,
                      passes, and the signatures of step 5's calls
      optimization/driver.*, optimizer.*, ssa_construction.*,
                      ssa_destruction.*, loop_rotate.*, noreturn_call.*
                      the TAC optimizer's driver, its table of passes,
                      SSA, and the passes before SSA (step 5, group B)
      optimization/extend_add_chain.*, recognize_indexed_load.*,
                      recognize_indexed_store.*, recognize_indirect_indexed.*,
                      direct_member_access.*, static_indexed_access.*,
                      phi_offset.*, sink_increment.*, sink_and_past_branch.*
                      the addressing and sinking passes (step 5, group F)
      optimization/cmp_zero_jump_fold.*, and_zero_jump_fold.*,
                      lnot_jump_fold.*, nonnegative_compare.*,
                      fold_producer_jumps.*, narrow_widened_arith.*,
                      narrow_byte_range.*, narrow_byte_bitwise.*
                      the jump folds and the narrowing (step 5, group D)
      optimization/constant_folding.*, phi_constant_fold.*,
                      static_const_fold.*, distribute_constant.*,
                      strength_reduction.*, reassoc_const.*, reassoc_offset.*
                      the folding passes of step 5 (group C)
      optimization/copy_propagation.*, dead_store_elimination.*,
                      unreachable_code_elimination.*, dead_loop_elimination.*,
                      jump_thread.*, cse.*, licm.*, copy_folding.*,
                      read_stored_global.*   the dataflow passes of step 5
                      (group E)
      optimization/compare_subtract.*, divmod_hoist.*, count_down.*,
                      step_on_edge.*, three_way_compare.*,
                      short_circuit_jump_fold.*, known_compare_thread.*
                      the passes after the fixed point and after SSA
                      (step 5, group G)
      optimization/inline_functions.*, row_address_tables.*,
                      dispatch_pointer_array.*, split_tables.*
                      the passes over the whole TAC program (step 5,
                      group H)
      front_stage.*   the signatures and runners of step 6's calls
      type_checking_types.*   the type queries of type_checking.py
                      that the front end's passes share (step 6)
      loop_labeling.* loop_labeling.py (step 6)
      type_checker.h  the TypeChecker class and the free functions of
                      type_checking.py, which the four parts share
      type_checker.cpp, type_checker_init.cpp, type_checker_decls.cpp,
                      type_checker_exp.cpp   type_checking.py's parts
                      T-A to T-D (step 6, group T)
      constant_expression.*, designators.*, stdbit_functions.*   the
                      modules the type checker reads (group T)
      identifier_resolution.*, label_resolution.*   the name passes
                      (step 6, group N)
      optimization_ast/unroll.*   the unroller of `[[c6502::unroll]]`
                      (step 6, group U)
      preprocess_stage.*  the runner of the step 6 call `preprocess`
      significant_bytes.*, in_place_member_update.*, pointer_restage.*,
                      shift_cost.*   the modules tac_to_asm reads
      capture_args.*  the decoders of a captured call's keyword arguments
    tac_to_asm.*      tac_to_asm.py's translate_program (step 4, group C)
    tac_to_asm_translator.h, tac_to_asm_calls.cpp, tac_to_asm_arith.cpp,
      tac_to_asm_compare.cpp   the Translator and the parts of the lowering
    tac_to_asm_stage.*  the runner of the stage call `tac_to_asm`
    c99_to_tac.*      c99_to_tac.py's translate_program (step 5, group A)
    c99_to_tac_translator.h, c99_to_tac_exp.cpp, c99_to_tac_lvalues.cpp,
      c99_to_tac_conversions.cpp   the Translator and the parts of the lowering
    c99_to_tac_stage.*  the runner of the step 5 call `c99_to_tac`
    fp_arith.*        fp_arith.py, the IEEE 754 patterns and their arithmetic
    builtin_functions.*   builtin_functions.py: the builtins' names, the
                      packed BCD arithmetic, and the <stdbit.h> operations
    bit_precise.*     the parts of bit_precise.py code generation,
                      c99_to_tac, and pp_numbers read
    pp_numbers.*      pp_numbers.py, the spelling of numbers (step 6)
    lark_table.h      the tables of c99.lark, which cpp/generated/c99_lalr.cpp
                      holds (tools/lalr_table.py writes it)
    py_regex.*        Python 3.11's `re`, its parser, compiler, and matcher,
                      for c99.lark's terminals and `--passthru-includes`
    py_regex_cases.*  the check of a record of `re` (tools/py_regex_records.py)
    lark_lexer.*      Lark's basic lexer and its UnexpectedCharacters
    lark_parser.*     Lark's LALR ParserState as a recognizer, and its
                      UnexpectedToken
    lexer.*           lexer.py: `check_number` and `tokenize`
    parser_context.*  the scopes, the typedef-aware TokenStream, and the
                      rewording of syntax errors, of parser.py (step 6, R1)
    parser_token_checks.*   the checks of recorded token streams
    parser.h          `parse`, parser.py's entry point (step 6, R2 and R3)
    parser_descent.*  the recursive descent's class, its start rule, its
                      tokens, and the guard of its actions
    parser_folds.cpp, parser_walk.h   the parser's folds through the type
                      checker, and its walks over the AST
    parser_declarations.*, parser_declarators.cpp, parser_specifiers.cpp
                      the declarations, declarators, specifiers, and
                      attributes (part R2)
    parser_statements.cpp, parser_expressions.cpp   the statements,
                      expressions, constants, and initializers (part R3)
    diagnostics.*     diagnostics.py, the warnings (step 6)
    front_helper_checks.cpp   the c99 type queries' records (step 6)
    preprocessor.*    preprocessor.py, c6502's configuration of pcpp and
                      its C23 hooks (step 6, group P)
    pcpp_preprocessor.*   pcpp 1.30's Preprocessor: lines, macros,
                      directives, includes, and `write`
    pcpp_lexer.*      pcpp's PLY lexer, its rules tested in PLY's order
    pcpp_evaluator.*, pcpp_evaluator_tables.h   pcpp's `#if` evaluator
                      and PLY's LALR loop, with the tables of the live
                      parser (tools/pcpp_evaluator_tables.py)
    pcpp_token.h      pcpp's token, with Python's reference semantics
    pcpp_text.*, pcpp_unicode.h   Python's `str`, `os.path`, and text
                      files as pcpp uses them, with the Unicode classes of
                      `\w`, `\d`, and the spaces and line breaks, and
                      `re`'s simple case mappings
                      (tools/pcpp_unicode_tables.py)
    preprocessor_cases.*  the check of a recorded call of `preprocess`
    sim/assembler.*   instruction_size, from sim/assembler.py
    main.cpp          the c6502cpp CLI
  tests/              doctest unit tests, test_<module>.cpp
    asm_text.h        a test's program and expected instructions as dump text
    call_case.h       a recorded call of a function of emission, for the
                      tests recorded from the Python unit tests
    tac_to_asm_case.h a recorded call of tac_to_asm from the Python unit tests
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

### The asm optimizer's driver

Group F1 of step 4 ported the program level of the asm optimizer,
`optimization_asm/optimizer.py`, as `optimize_program` and
`optimize_function` (`_optimize_function`) in
`src/passes/optimization_asm/optimizer.*`, with the passes it calls
around the round trip: `apply_coloring`, `fold_const_statics`
(`const_static_fold.*`), and `apply_dead_static_elimination`
(`dead_static.*`). `compute_local_bytes` and
`compute_address_taken_bytes`, which compile.py calls on the
optimizer's first result to size the pools, are in
`src/passes/function_local_sizing.*`. `optimize_function` runs group D's
round trip and group E's allocation in Python's order. The stage's
driver calls `optimize_program` with an `AsmOptimizerOptions`, which
holds Python's keyword arguments, and gets the program and the
colorings back in an `OptimizedAsmProgram`.

`apply_coloring` names the bytes of a private pool with
`local_slot_names` (`src/passes/local_slot_names.*`), which
`zp_slot_naming.compute_local_slot_names` reads too. Part F2 wrote it,
and both parts carry the same file.

Python keys a few dicts in an order that comes from iterating a set of
names: the `register` locals pinned to Y, and the use counts of the
eligibility projected onto the representatives of the coalescing.
Nothing reads their order, since the eligibility scan and the
allocator only look names up, so the C++ driver builds the first in the
symbol table's order and the second sorted by name. The order of `Coloring.assignments` is read (the first name
from the source at an address names its slot), and the driver adds
the coalesced names to it in the order of the coalescing's
`representative`, as Python does.

`tools/capture_passes.py` records `optimize_program` as `asm_optimizer`,
and four calls inside the driver as `optimize_function`,
`apply_coloring`, `fold_const_statics`, and `dead_static_elimination`,
where `optimize_program` reaches each through its module.
`src/passes/optimization_asm/optimizer_stage.*` holds the runners. On
20 Nov 2026 the capture of the whole corpus took 117 seconds with 10
processes, and every distinct call matched: 2,231 of `asm_optimizer`,
7,157 of `optimize_function`, 7,016 of `apply_coloring`, 1,110 of
`fold_const_statics`, 1,771 of `dead_static_elimination`, 937 of
`compute_local_bytes`, and 1,110 of `compute_address_taken_bytes`.
A line trace of the five Python modules found the lines that no call
of the corpus reaches, among them the explicit test of X before a
branch, most of the rules of `fold_const_statics`, and the store that
`apply_coloring` can't encode. `tests/test_asm_optimizer_calls.cpp`
holds 47 calls that reach them, recorded with their results through
`tests/stage_case.h`. They are calls that four tests of
tests/test_const_static_fold.py and tests/test_byte_arith_sim.py make
through compiles, the direct calls of tests/test_dead_static.py,
test_function_local_sizing.py, and test_hwreg_pinning.py, and calls on
inputs written for the lines no test reaches, two of which raise. The lines left are a few branches
of dispatch and the paths for a symbol table without `_table`, which
C++ has no form for.

### The ABI and the call graph

Group A of step 4 ported `select_abi`, `fixed_address_layouts`, and
`pinned_slot_symbols` (`src/passes/abi_selection.*`), the carry-return
choices (`src/passes/carry_return_choice.*`), the pool eligibility of
`zp_local_allocation` (`pool_eligible`, `recursive_pool_functions`,
`zp_local_allocation::determine_eligibility`), all of `recursion_saves`
(`call_graph`, `cycles`, `check_marked_calls`, `hw_stack_cycles`,
`plan_saves`, `recursion_saves::live_out` and `defs_in`), and the TAC
CFG. Their runners are in `src/passes/stage_registry_abi.*`, with the
codecs of a `SavePlan` and a `CFG`. Besides `pinned_slot_symbols`,
`STAGE_PASSES` records the calls that compile.py doesn't make itself
where their callers reach them. Those are `cycles` and
`hw_stack_cycles` in `zp_local_allocation`, `check_marked_calls` and
`plan_saves` in `recursion_saves`, `live_out` where `find_shares` calls
it, and `build_cfg` where `_live_out` calls it. A record whose input is a TAC `Function` holds its dump under
`input`, which `check-captures` reads as a `Type_top_level`.

Python builds each call graph as a dict from a set of names, whose
order follows the hashes of strings and changes from run to run, and
nothing reads that order, so `CallGraph` is a sorted `std::map`. The
functions that return a dict in an order that reaches their callers
(`select_abi`'s ABI, `cycles`, `pinned_slot_symbols`, the save plan's
maps) return an `OrderedMap` in Python's order. An `AbiSelectionError`
and a `RecursionSaveError` are `CompileError`s with Python's messages.

The capture of the whole corpus on 20 Nov 2026 gave from 16 distinct
calls of `plan_saves` and 58 of `pinned_slot_symbols` to 3,394 of
`build_cfg`, and every one matched. Few of them raise or give a
non-empty result, so `tests/test_abi_stage_calls.cpp` holds the calls
that the Python unit tests make, recorded with their results and
exceptions, and `tests/test_tac_cfg_cases.cpp` the CFG's values on each
function that tests/test_cfg.py builds a CFG of. The capture helpers'
`tables` records give the CFG's analyses on every function that
`find_shares` sees (`_cfg_values`).

### Code generation: tac_to_asm

Group C of step 4 ported `tac_to_asm.translate_program`, which lowers a
TAC program to the asm IR, as `tac_to_asm::translate_program`
(`src/tac_to_asm.h`), the header that the stage's driver includes. It
takes the symbol table as a pointer that may be null and adds the
`<param>.reg` locals to it, and the type table, the `AsmContext`, and
an `Options` with Python's keyword arguments. The Python class
`Translator` is a struct in `src/tac_to_asm_translator.h`, with the
lowering split over four files in the parts of docs/cpp_port.md:
`src/tac_to_asm.cpp` (the driver, the queries every part reads, the
returns and copies, and the free helpers), `tac_to_asm_calls.cpp`
(calls, the ABI, and memory), `tac_to_asm_arith.cpp` (arithmetic and
the inline shifts, divisions, and products), and
`tac_to_asm_compare.cpp` (casts, unary operators, conditional jumps,
and comparisons). The struct declares the lowerings one part calls in
another, and each part keeps its other helpers as free functions. The
modules it reads are ported beside it: `passes/significant_bytes.*`,
`passes/in_place_member_update.*`, `passes/pointer_restage.*`,
`passes/shift_cost.*`, `split_tables::plane_init`,
`compare_subtract::compared_difference`, and the parts of
`bit_precise.py` it uses (`src/bit_precise.*`).

The labels and the cycle-break temporaries are numbered by two counters
over the whole program, so the order of the lowering is part of its
output. A lowering that makes two labels makes each in a statement of
its own, since C++ leaves the order of a call's arguments open, and the
`tac_defs` of a function hold pointers into its instructions where
Python keys them by the identity of each instruction. The runner
(`src/tac_to_asm_stage.*`) gives the `mutated` line with the symbol
table after the call when the lowering added a local.

`c6502cpp check-captures --got <dir>` writes the C++ output of each
stage call that differs from its record to a file named for the record,
for comparing the two. The capture of the whole corpus on 20 Nov 2026
gave 2,256 distinct calls of `tac_to_asm` in both modes, and every one
matched with MSVC and with GCC. `tests/test_tac_to_asm_calls.cpp` holds
four calls that the Python unit tests make, recorded with their results
through `tests/tac_to_asm_case.h`. A line trace of the lowering over
the corpus and over the 1,291 unit tests of the 40 test modules that
compile through it found 27 lines that only the unit tests reach, and
those four calls reach all of them.
### Sharing and the zero-page allocators

Group B of step 4 ported the rest of `slot_sharing` that reads the TAC
(`find_shares`, `pinned_param_copies`, `slot_aliases`,
`changed_param_slots`, `preferred_symbols`), `allocate_zp_slots`
(`src/passes/zp_slot_allocation.*`), `allocate_function_locals` and
`build_local_slot_symbols` (beside the pool eligibility in
`src/passes/zp_local_allocation.*`), `zp_globals`' plan
(`allocate_zp_globals`, `plan_zero_page`, and `access_weights`, in
`src/passes/zp_globals_plan.cpp`), `runtime_layout.choose_layout`
(`src/passes/runtime_layout.*`), and `zp_link_metadata.build_metadata`.
The runners are in `src/passes/stage_registry_sharing.*`, with the codec
of a `LinkMetadata`. `STAGE_PASSES` records `access_weights` where
`plan_zero_page` calls it, and the three calls of the link metadata
and emission that compile.py makes after code generation,
`changed_param_slots`, `preferred_symbols`, and `build_metadata`.

`find_shares` keys its bindings by `id()` and `allocate_zp_slots` its
shares, and the C++ functions key them by their index in the list
Python keeps them in. `allocate_zp_slots` and `allocate_function_locals`
take their keyword arguments as one `ZpAllocationOptions`, with
Python's defaults. `promotion_order` sorts by Python's float key
`-w / size`, which both languages compute as the same double, since
the weights and sizes are far below 2**53.

`plan_zero_page` sizes HARGS, when the runtime's block has to move, by
translating the TAC with `tac_to_asm` and measuring the result. The C++
function takes that measurement as a `HargsNeed`, a function its caller
gives, which group F connects to group C's port of `tac_to_asm`. The
registry's runner gives the HARGS size of the recorded result, which is
the measurement or 1 when the program uses none, and `choose_layout`
makes the same layout of either. `build_local_slot_symbols` takes the
slot names (`slot_names_by_fn`), since naming them from the colorings
is `compute_local_slot_names`, which group F ports. `choose_layout` and
`allocate_zp_globals` take a function as an argument, which the
harness can't record, so they are checked through `plan_zero_page`,
their only caller in a compile, and through the unit tests' calls.

On 20 Nov 2026 the capture of the eleven calls over the whole corpus
took 123 seconds with 10 processes and wrote 271 MB, and every distinct
call matched: 2,292 of `plan_zero_page`, 1,145 of `access_weights`,
1,110 each of `find_shares` and `pinned_param_copies`, 952 of
`allocate_zp_slots`, 964 of `allocate_function_locals`, 778 of
`local_slot_symbols`, 215 of `slot_aliases`, 1,107 of
`changed_param_slots`, 216 of `preferred_symbols`, and 949 of
`build_metadata`. Of those, 74 calls of `plan_zero_page` move the
runtime's block and 252 promote a global, 20 calls of `find_shares`
find a share, and no call of `pinned_param_copies` finds a parameter to
copy, and none raises. So `tests/test_sharing_stage_calls.cpp` holds
546 calls that the Python unit tests make, recorded with their results
and exceptions (`tests/sharing_case.h`), among them 12 of
`choose_layout` and 74 of `allocate_zp_globals`, and
`tests/test_zp_allocator_errors.cpp` the allocators' errors that no
test reaches.

### The end of the stage

Part F2 of step 4 ported the work between the asm optimizer's last run
and `synthesize_prologue`. `compute_address_taken_assignments` and
`address_taken_zp::slot_symbols` (`src/passes/address_taken_zp.*`) put
the address-taken locals of a function with a private pool in the pool
bytes its coloring leaves free and name them. `local_slot_names`
(`src/passes/local_slot_names.*`, a file of its own because the asm
optimizer's `apply_coloring` calls it too) names one pool's bytes, and
`compute_local_slot_names` (`src/passes/zp_slot_naming.*`) every
pool's. `replace_program` and `replace_program_bare_exit`
(`src/passes/replace_pseudoregisters.*`) replace the pseudos and lay
out the frames, the second returning the `FrameDims` by function that
`synthesize_program` reads, and `lower_data_load_address`
(`src/passes/lower_data_load_address.*`) splits each `LoadAddress` of a
`Data` source. The runners are in `src/passes/stage_registry_frames.*`.

The stage's driver calls them in compile.py's order:
`compute_address_taken_assignments` on the pools of the functions off
every cycle, `slot_symbols` for each function, `replace_program_bare_exit`
with a `BareExitTables` of the ABI, the pools, and those two maps, then
`local_slots`, which is `compute_local_slot_names` followed by
`build_local_slot_symbols` and gives both the names and the `EQU`
table, and last `lower_data_load_address` with the table merged with
the zp_abi slots and each pool's bytes zipped with its names. The
unoptimized pipeline calls `replace_program` after `tac_to_asm`. A
pseudo of none of the four kinds, and a `Phi`, throw `InternalError`
with Python's message, since Python raises `ValueError` and
`TypeError`, which aren't errors of the program.

On 20 Nov 2026 the capture of the seven calls over the whole corpus
took 104 seconds with 12 processes and wrote 637 MB, and every distinct
call matched in the builds of MSVC 19.36 and GCC 12.2: 1,110 of `address_taken_assignments`, 3,798 of
`address_taken_slot_symbols`, 1,110 of
`replace_pseudoregisters_bare_exit`, 855 of `local_slot_names`, 778 of
`local_slot_symbols`, 938 of `lower_data_load_address`, and 1,146 of
`replace_pseudoregisters`. No function of the corpus has two
address-taken locals with one source name, so
`tests/test_frames_stage_calls.cpp` holds the 66 distinct calls that
the Python unit tests of the four modules make, recorded as stage calls
with Python's results (the per-function calls as the program-level
call on a program of the one function), and three cases written by
hand for the two errors and for the fallbacks of `local_slot_names`.

## The TAC optimizer's calls

Step 5 (docs/cpp_port.md) ports `c99_to_tac`, the inliner, and the TAC
optimizer. `tools/capture_passes.py --passes tac` records each call
that the optimizer's driver (`PhaseDriver.apply`) makes on one
function, as `tac_<name>` for each pass and `tac_to_ssa` and
`tac_from_ssa` for SSA construction and destruction, and `--passes
tac-stage` records the program-level calls of its `TAC_STAGE_PASSES`,
`c99_to_tac` through `split_tables`, with `tac_optimize_function` for
each function the optimizer is run on. Their signatures are in
`src/passes/optimization/tac_stage.cpp`, which `stage_signatures`
appends to those of step 4, so `check-captures` reads them as it reads
the stage calls. A signature's `output_module` names the IR of a
result the record holds as a dump, a TAC program or function here.

A call of the driver is recorded with the function as its input and
two keyword arguments, `symbols` and `ssa_dsts`. The symbol table the
record holds is a part of the compile's: the harness hands the pass a
view of the table that passes each read and write on and notes the
names, and the record holds each symbol the call read before it wrote
it, in the table's order. A pass that walks every entry
(`fold_static_const_reads`) gets the whole table. The `mutated` line
holds the symbols read or added after the call, and the SSA names,
when either changed, and the C++ runner (`run_tac_pass` in
tac_stage.cpp) writes the same line, with the symbols a pass adds at
the end of the table as Python adds them. `c99_to_tac` changes the
c99 program it is given in place, and its `mutated` line holds the
program after the call under `input`, which `stage_checks.cpp` reads
with the c99 codec.

The passes of step 5 take the function and a
`passes::framework::PassContext`, the symbol table and the SSA names,
either of which may be null, where Python's may be None.
`src/passes/optimization/framework.h` has the run loops of the
framework's base classes (`run_window`, `run_rules`, `run_def_use`,
`run_sink`, `run_operand_rewrite`), each of which takes the pass's
matching and rewrite as one function that returns none where the
Python pattern doesn't match or the rewrite declines, since the
pattern DSL of framework/patterns.py isn't ported.
`fold_truncate_extend` and `fold_extend_extend`
(`src/passes/optimization/truncate_extend_fold.*`) are the model of a
pass on `run_def_use`.

### Addressing and sinking (group F)

The passes of group F of step 5 each have a header named for their
Python module under `src/passes/optimization/`, and each takes the
function and the `PassContext`, as the driver calls it:
`recognize_indexed_load`, `recognize_indexed_store`, and
`recognize_indirect_indexed`, which share the walk of
`extend_add_chain.h`; `direct_member_access`; `static_indexed_access`;
`phi_offset`; `sink_increments` (`sink_increment.h`); and
`sink_and_past_branch`. Python's `DefUsePass` keeps the use counts of
`prepare_extra` in `env.extra`, which the C++ `DefUseEnv` doesn't have,
so each recognizer computes `count_uses` before the walk and reads it
with `use_count` (`extend_add_chain.h`).

These are the pieces that other groups read.

- `extend_add_chain.h`: `is_1_byte_var` and `is_1_byte_val`, which
  constant folding's `distribute_constant` reads, the walk
  `recognize_extend_add_chain`, `UseCounts` and `use_count`, and
  `all_dsts`.
- `narrow_widened_arith.h`: `passes::var_unchanged` and, in
  `passes::narrow_widened_arith`, `register_temp`, `narrow_constant`,
  `byte_width`, and `is_unsigned`, the helpers of
  narrow_widened_arith.py that `recognize_indirect_indexed` and
  `sink_increment` read. The module's pass is in the same files
  (group D). `var_unchanged` reads an entry of `env.instructions` that an
  earlier rewrite made a bare list as an instruction that does nothing.
- `split_tables.h`: `width`, Python's `split_tables._width`.

`recognize_indirect_indexed` finds the index of the access it rewrites
by identity in Python (`recognize_indirect_indexed.py:359` and
`:487`), which gives the first position of an object that sits at two.
The C++ pass takes the index at which `run_def_use` calls the rewrite,
which it counts in the rewrite, and every distinct call of the corpus
matched. The temporaries that pass and `static_indexed_access` add
(`<addr>.base`, `<ptr>.y<k>`, `<dst>.base<n>`) are registered before
the rewrite is known to go ahead, as in Python, so a rewrite that
declines or conflicts still leaves its symbol.

On 21 Nov 2026 each pass matched the Python pass on every distinct
call of the corpus: 9,273 calls each of `tac_recognize_indexed_load`
and `tac_recognize_indexed_store`, 3,942 each of
`tac_recognize_indirect_indexed`, `tac_static_indexed_access`, and
`tac_phi_offset`, 9,275 of `tac_direct_member_access`, 9,267 of
`tac_sink_increments`, and 9,243 of `tac_sink_and_past_branch`. Few of
them change the function (2 of the indexed load's, 4 of the store's, 1
of `phi_offset`'s, and 1 of `sink_and_past_branch`'s), so
`tests/test_<module>.cpp` holds the calls that the Python unit tests
of the eight modules make, recorded with the capture harness's wrappers
on the driver's passes and on the entry points the tests call. Of their
1,336 distinct calls, 1,334 matched, and the other two give a symbol a
`StaticAttr` with no initial value, which the codec can't read, and
match with a `NoInitializer` in its place. The tests hold the 230
that changed the function or the symbols, or that a test of synthetic
TAC made of an entry point, and the corpus calls that changed the
function for the four passes above.
`tests/test_extend_add_chain.cpp` holds cases of the walk's gates
written by hand, with Python's results.

The helpers that the TAC passes share have records of their own, which
`tools/capture_helpers.py --tac-only` makes at each call of a pass of
the driver. A `tacopt` record holds the function, the symbols of its
operands, and the SSA names, and gives `value_range.max_value` of each
operand, `int_width`, `type_max`, and `is_unsigned_type` of each
symbol's type, and `rewrite_uses_in` of each instruction with two
mappers (`tac_opt_values`). `check-helpers` computes them in
`src/tac_helper_checks.cpp`.

### Dataflow over the CFG (group E)

The passes of group E of step 5 each have a header named for their
Python module under `src/passes/optimization/` and take the function
and the `PassContext`, as the driver calls them: `copy_propagate`
(`copy_propagation.h`), `eliminate_dead_stores`
(`dead_store_elimination.h`), `eliminate_unreachable_code`,
`eliminate_dead_loops`, `thread_constant_jumps` (`jump_thread.h`),
`eliminate_common_subexpressions` (`cse.h`), `hoist_loop_invariants`
(`licm.h`), `fold_copies_in_fixedpoint` and
`fold_copies_post_destruction` (`copy_folding.h`), and
`read_stored_globals` (`read_stored_global.h`). Dead-loop elimination
finds a loop marked `[[c6502::keep]]` by `c99_to_tac::kKeepLoopMark`
in one of its labels.

Unreachable-code elimination, dead-loop elimination, and jump threading
build new Phis and instructions in Python, since a Python block holds
the instructions of the function the pass was given, and a change in
place changed the function the driver compares the result with. The
blocks of the C++ CFG own copies of their instructions, so the C++
passes change them in place.

Copy propagation rewrites the operands of the instructions Python's
`_rewrite` has a case for, which leaves a `GetAddress`, an
`IndexedConstLoad`, and the two indirect-indexed accesses alone, so it
has a switch of its own where `rewrite_uses_in` would rewrite those
too. Common-subexpression elimination keys its tables by strings made
of the dump of each operand and of the destination's type, where
Python keys them by tuples of reprs. Two dumps are equal exactly when
the values are, and the two operands of a commutative operator go in
the order of their strings, which pairs the same instructions as
Python's order of the reprs. Python's `copy_folding.fold_copies`, which
the driver doesn't call, is the window pass without a symbol table, so
the calls its unit tests make are recorded as calls of
`tac_fold_copies_in_fixedpoint` with none. Jump threading would give a
Phi an argument from a block without a label only where Python would
make a `PhiArg` with a `pred_label` of None, which the pass's test that
the target's Phis name the join rules out, and it throws
`InternalError` there.

On 21 Nov 2026 each pass matched the Python pass on every distinct call
of the corpus: 11,828 calls of `tac_copy_propagate`, 11,825 of
`tac_eliminate_dead_stores`, 12,829 each of
`tac_eliminate_unreachable_code`, `tac_eliminate_dead_loops`, and
`tac_thread_constant_jumps`, 11,830 of
`tac_eliminate_common_subexpressions`, 11,825 of
`tac_hoist_loop_invariants`, 9,573 of `tac_fold_copies_in_fixedpoint`,
3,948 of `tac_fold_copies_post_destruction`, and 9,564 of
`tac_read_stored_globals`. Few of those calls change the function for
some of the passes: 32 of dead-loop elimination's, 100 of the hoist's,
136 of `read_stored_globals`', and 362 of jump threading's.
`tests/test_<module>.cpp` holds 199 recorded calls of the Python unit
tests of the nine modules: every call of an entry point on a test's
synthetic TAC, and every call through the driver that changed the
function. The tests of dead-loop elimination, the hoist, and
`read_stored_globals` also hold the eight smallest corpus calls of each
that change the function. Every distinct call the unit tests make
matched, 534 in all, except three of `read_stored_globals` that give a
symbol a `StaticAttr` with no initial value, which the codec can't
read, and which match with a `NoInitializer` in its place.
### Jump folds and narrowing (group D)

The rule set `fold_producer_jumps` of Python's optimizer.py is
`passes::fold_producer_jumps` (`src/passes/optimization/fold_producer_jumps.h`),
which runs the seven rules of cmp_zero_jump_fold.py,
and_zero_jump_fold.py, lnot_jump_fold.py, and nonnegative_compare.py in
the driver's order on the framework's `run_rules`. Each rule is a
function in the header named for its module, which tests its window and
its guards and returns the replacement or none, and
`run_jump_fold_rules` runs any of them in a given order, as the Python
unit tests run rule sets of one rule. The narrowing passes are
`apply_narrow_widened_arith` (`narrow_widened_arith.h`, beside group
F's helpers, since a function can't share the name of that namespace),
`narrow_byte_range`, and `narrow_byte_bitwise`.

`passes::extend_source_unchanged` (`cmp_zero_jump_fold.h`) is the test
that an extend's source holds the same value at a later instruction,
which `and_zero_jump_fold`, `narrow_byte_range`, and
`narrow_byte_bitwise` read. It takes the extend's source and dst in
place of the instruction, and has an overload for a `DefUsePass`'s view
of the instructions, where an entry an earlier rewrite made a list
fails the test, as Python's type test of a list does. A position of -1,
where Python found none, passes only an SSA-renamed source.

Python finds an instruction's position by identity in four places
(`cmp_zero_jump_fold.py`'s compare and `JumpIfCmp`,
`narrow_widened_arith._truncate_index`, and the use positions of
`narrow_byte_range` and `narrow_byte_bitwise`), which gives the first
position of an object that sits at two. The C++ rules take the window's
position, and the passes on `run_def_use` count the calls of the
rewrite, and every distinct call of the corpus matched.

On 21 Nov 2026 each matched the Python pass on every distinct call of
the corpus: 12,839 calls of `tac_fold_producer_jumps`, of which 1,937
changed the function, 9,243 of `tac_narrow_widened_arith` (1,246
changed), and 9,045 each of `tac_narrow_byte_range` (984) and
`tac_narrow_byte_bitwise` (289). The tests
`tests/test_<module>_calls.cpp` hold the calls of the Python unit tests
of the seven modules and of test_flag_compares.py,
test_truncate_extend_fold.py, and test_bool.py that changed their
function, and every call those tests make of a pass on its own, 179 in
all, and `tests/test_jump_fold_rules_calls.cpp` the 56 calls of rule
sets of one rule, checked with `tests/tac_rules_case.h`. A static that a
synthetic test gives no initial value, which the codec can't read, has
a `NoInitializer` in its place.
### After the fixed point and after SSA (group G)

The passes of group G of step 5 run once each after the fixed point
(`hoist_subtract_to_compare`, `hoist_divmod_pairs`,
`count_loops_down`), once each after SSA destruction
(`move_steps_onto_edges`, `split_three_way_compares`), or in the fixed
point after destruction (`fold_short_circuit_jumps`,
`thread_known_compares`). Each has a header named for its Python
module under `src/passes/optimization/` and takes the function and the
`PassContext`, as the driver calls it. `compare_subtract.h` holds the
pass beside `compare_subtract::compared_difference`, which
`tac_to_asm` reads. `step_on_edge.h` also has the form of Python's
`move_steps_onto_edges` that takes the SSA names alone, and
`short_circuit_jump_fold.h` has `fold_short_circuit_jump` and
`merge_equality_jumps`, which the driver's entry for
`fold_short_circuit_jump` runs one after the other, as Python's
`FoldShortCircuitJump.run` does.

Several of the passes take a count or an index once and then change the
function, and the ports keep that. `move_steps_onto_edges` counts the
uses, the definitions, and the jumps to each label, and finds each
label's position, before its walk, and reads those after it has
removed a step. `hoist_subtract_to_compare` moves every subtract it can
and only then gives the loops' exits their `<a>.exit` values, from a
copy of each subtract as it was moved. `split_three_way_compares`
leaves in the symbol table the temporaries it made for the first
operand when the second operand has no bytes to compare and the pair
is left alone. None of the seven passes finds an instruction by
identity. The label counters follow Python: `@scfold@N` starts at 0 in
each call and skips the labels the function has, and `@cthread@N`
starts one past the largest one in the function.
`thread_known_compares` keeps each Var's facts as one value it equals
and a set of values it doesn't, where Python keeps a set of `("eq",
k)` and `("ne", k)` pairs, since a Var never has two facts of
equality.

On 21 Nov 2026 each pass matched the Python pass on every distinct call
of the corpus: 3,942 calls each of `tac_hoist_subtract_to_compare`,
`tac_hoist_divmod_pairs`, `tac_count_loops_down`,
`tac_move_steps_onto_edges`, and `tac_split_three_way_compares`, 4,031
of `tac_fold_short_circuit_jump`, and 4,017 of
`tac_thread_known_compares`, in the builds of MSVC 19.36 and GCC 12.2.
Few of those calls change the function (5, 2, 32, 6, 1, 14, and 75), so
`tests/test_<module>.cpp` holds the calls of the pass that the Python
unit tests make, recorded with the capture harness's wrappers on the
driver and on the entry points the tests call, and the corpus calls
that change the function (for the two passes of the last fixed point,
those with an input of under 3,000 characters). Every distinct call of the unit
tests matched: 49, 54, 56, 54, 17, 65, and 63 of them. Three tests of
`divmod_hoist` give a symbol a `StaticAttr` with no initial value,
which the codec can't read, and those calls match with a
`NoInitializer` in its place, which the pass doesn't read.

### The program level (group H)

The passes of group H of step 5 run once over the whole program, and
each has a header named for its Python module and takes the program by
value with the tables it reads: `inline_functions`
(`src/passes/optimization/inline_functions.h`), `return_pointers_in_zero_page`
(`src/passes/return_in_zero_page.h`), `apply_row_address_tables`
(`row_address_tables.h`), `dispatch_const_pointer_arrays`
(`dispatch_pointer_array.h`), and `apply_split_tables`, which joins
`plane_init` and `width` in `split_tables.h`. The inliner reads three
helpers of `abi_selection` that its header now gives,
`function_decls_by_name`, `has_register_local`, and
`has_register_request`.

The inliner's estimate of cost runs `optimize_function` on a copy of
the callee with a copy of the symbol table. Python's copy of the table
is shallow, so its `Symbol` objects are the compile's, and a pass that
changed a symbol in place during an estimate would change the
compile's table. No pass of the TAC optimizer does, which is why a copy
of the symbols as values gives Python's results. The inliner takes its
next `@N` and `%N` from the largest in the whole table, and it numbers
its copies' labels `.ik_` in the order it copies them, so the order in
which it takes the functions (leaves first, by name within a round) is
part of its output.

`passes::translate_for_codegen` and `optimized_tac`
(`src/passes/translate_for_codegen.*`) are compile.py's functions of
those names, and `passes::codegen` (`src/passes/codegen_stage.*`) runs
them before the stage ("The stage's driver").

On 21 Nov 2026 each pass matched the Python function on every distinct
call of the corpus: 1,150 calls of `inline_functions`, 1,146 each of
`return_pointers_in_zero_page` and `row_address_tables`, and 1,110 each
of `dispatch_const_pointer_arrays` and `split_tables`, in the builds of
MSVC 19.36 and GCC 12.2. Few of those calls change the program (26, 7,
7, 1, and 3), and most of those are the games' units, of 220 KB to 1
MB each, so `tests/test_<module>.cpp` holds the calls that the Python
unit tests of the five modules make, recorded with the capture
harness's wrapper on each function while those tests ran, and the
corpus calls under 50 KB that change the program. Every distinct call
of the unit tests matched, 159 in all, except two of
`row_address_tables` made by tests that replace its estimate
(`_saving`) with one that makes every table pay, which the C++ pass
doesn't have. The tests hold 142 calls.

## The front end's calls

Step 6 (docs/cpp_port.md) ports the preprocessor, the parser, and the
passes on the c99 program through the type checker.
`tools/capture_passes.py --passes front` records the calls of its
`FRONT_PASSES`, from `preprocess` to `type_checking`, and of its
`FRONT_HELPERS`, the functions of pp_numbers.py, and
`src/passes/front_stage.cpp` gives each a signature, which
`stage_signatures` appends to those of steps 4 and 5. The source text
that `preprocess` and `parse` take is the record's `input-json` line, a
JSON string, and a c99 program is a dump, with "" for an unnamed
parameter, which the C++ IR holds in place of Python's None.

A call of the front end that raised is recorded with an `output-error`
section, `{"type": ..., "message": ...}` with the exception's class and
`str(e)`, which `capture::Call` reads as `output_is_error`. A ported
runner throws `CompileError` with the Python class as its
`python_type()` and the same message, and `check-captures` writes the
same JSON for it (`stage_checks::error_text`) and compares the two. A
call that issued warnings has a `warnings` line, the JSON list of
`[where, message]` pairs, which a runner returns in
`StageOutput::warnings`. The type checker stamps the program it is
given, and its record's `mutated` line holds the program after the call
under `input`, as `c99_to_tac`'s does. `check-captures` reads and writes
back every value of every record, the errors and the warnings included.

These pieces of the front end are ported.

- `pp_numbers` (`src/pp_numbers.*`, namespace `pp_numbers`):
  `pp_number_at`, `is_floating`, `parse_integer`, `check_floating`,
  `bit_precise_width`, and `check`, which throw `NumberError`, a
  `CompileError` of type "NumberError", with Python's messages. Python
  keeps the value of a constant as an int of any size, so
  `IntegerConstant` holds its exact `bit_length` in `bits` and the value
  when that is at most 127, which every caller checks first, since a
  constant of more than 64 bits has no type. The module tests `in` on
  one-character strings that may be empty, and `py_in` keeps Python's
  meaning, in which the empty string is in every string. The corpus
  spells few numbers that the lexer checks, so `tests/test_pp_numbers.cpp`
  holds Python's results on spellings that reach every branch, written
  by a script from the Python module.
- `diagnostics` (`src/diagnostics.*`): `CompileWarning`, `warn`, and
  `Collecting`, the RAII form of `collecting()`, whose innermost block
  is kept per thread as Python keeps it in a context variable.
- Python's `repr` of every IR value. `asdl.py --cpp` writes a `repr`
  for each struct and sum type beside the dump format's functions, the
  dataclass's `ClassName(field=value, ...)` with the fields in the
  `.asdl`'s order, and `asdl::py_repr(v)` returns it. Strings go through
  `pyrepr::string`, which now writes U+0080 to U+00A0 and U+00AD as
  `\x80` and so on, as Python does for a `str` of bytes.
- The type queries of type_checking.py (`src/passes/type_checking_types.*`,
  namespace `passes::type_checking`): `strip_quals`, which returns a
  reference into the type, the predicates, `display_name`, `c_type`,
  `int_width`, `int_bits`, `is_signed`, `rank`, `promote_integer`,
  `coerce_int_to_type`, `common_type`, and `common_bit_precise_type`. A
  query that raises `TypeError` or `KeyError` in Python, for a type it
  doesn't apply to, throws `InternalError`.
- `loop_labeling` (`src/passes/loop_labeling.*`), `label_program`,
  which labels the program it takes by value, where Python builds a new
  one, and throws `CompileError` of type "LoopLabelingError".
  `tests/test_loop_labeling.cpp` holds the 38 distinct programs that the
  Python unit tests label, recorded with Python's results, a call of
  `label_function` on the legacy `Function` as the call of
  `label_program` on the program of the `function_decl` it builds.
- `identifier_resolution` (`src/passes/identifier_resolution.*`),
  `resolve_identifiers`, which resolves the program it takes by value
  and clears the `data_type` of each expression, which the nodes that
  Python builds anew leave None. Its `@N.` names come from one counter
  in the order of Python's walk, and that order includes the order in
  which Python evaluates the keyword arguments of each node it builds:
  a compound literal is named before its type's tags are resolved, a
  declaration before its type and its initializer, and a call's
  arguments are resolved before its name, so an undeclared name among
  the arguments is the one reported. The scopes are hash maps, since
  Python only looks names up in its dicts, and the tag scopes are a
  stack whose bottom is the file scope, where Python compares the
  innermost tag scope with the file scope's dict by identity. It throws
  `CompileError` of type "IdentifierResolutionError", and a message
  about an operand that isn't an lvalue shows the operand's repr.
- `label_resolution` (`src/passes/label_resolution.*`),
  `resolve_labels`, which throws "LabelResolutionError".
- `string_lifting` (`src/passes/string_lifting.*`), `lift_strings`,
  which lifts the strings of the program it takes by value. It numbers
  the `str@N` statics in the order of Python's walk, which visits each
  node's fields in the order of its constructor's keyword arguments, so
  a do-while's body comes before its condition. It rewrites a braced
  list with designators through `designators::normalize_initializer`,
  with the member lists of the struct and union bodies it has passed,
  which throws "TypeCheckError", and counts a string's length in code
  points, as `pyrepr::code_points` gives them from the UTF-8 the C++
  string holds.
- The unroller (`src/passes/optimization_ast/unroll.*`),
  `unroll_program`, which throws "UnrollError". Python keeps the values
  of the file-scope constant arrays and their element types in two
  module globals, which it sets for the length of a call and restores
  after, and here they are members of an `Unroller` made for each call.
  A tree of an array's values shares its padding, as Python's `[t] * n`
  shares the element. Python's `_walk` and `_substitute` visit every
  field of every dataclass, and the C++ `Walker` visits the statements,
  declarations, for inits, and expressions in the same order, which
  decides the error reported for a body with several faults, and skips
  the types, which hold no expression.
- The preprocessor (`src/preprocessor.*` with `src/pcpp_*`, namespaces
  `preprocessor` and `pcpp`), described below under "The preprocessor".

The type queries and the repr are checked on what the type checker
returns. `tools/capture_helpers.py --front-only` writes a `c99types`
record at each call of the type checker, with the dumps of every type of
the program and its symbol table (and a set of bit-precise types and
qualified pointers, which the corpus has few of), of its top-level
declarations, and the names of its symbol table, and `front_values`
gives each query's value on each type, on each pair of the promoted
arithmetic types for `common_type`, and on 28 values for
`coerce_int_to_type`. `check-helpers` computes them in
`src/front_helper_checks.cpp`.

```sh
uv run python tools/capture_passes.py --passes front -j 12
uv run python tools/capture_helpers.py --front-only --corpus examples,games,chapters,fuzz -j 12
cpp/build/bin/c6502cpp check-captures captures
cpp/build/bin/c6502cpp check-helpers captures/helpers
```

- The type checker (group T), described below in "The type checker".

On 21 Nov 2026 every value of the 20,177 distinct records of the front
end's calls on the corpus made the trip, and `check-captures` took 4
seconds over them. `pp_number_at`, `pp_parse_integer`, and
`pp_is_floating` matched on all 5,137, 2,117, and 1 of their calls, and
`loop_labeling` on all 1,477 of its calls, 10 of which raise. The 1,154
`c99types` records gave 2.3 million values with no difference, 1.77
million of them `common_type`.

### The type checker

`passes::type_checking::check_program` (`src/passes/type_checker.h`)
is the port of type_checking.py's `check_program`. It stamps the c99
program it is given in place, as Python's does, and returns the symbol
table and the type table. The `TypeChecker` class is declared once in
type_checker.h, and its methods are defined in the files of the
survey's four parts: `type_checker.cpp` (the free functions that every
part reads, with `sizeof_type`, `check_well_formed_type`, the null
pointer constants, `convert_to`, and `decay_if_array`),
`type_checker_init.cpp` (static initializers and `constexpr`),
`type_checker_decls.cpp` (`check_program`, declarations, layouts,
functions, calls, and statements), and `type_checker_exp.cpp`
(`check_exp`). A method that Python gives an expression it changes
takes a reference to the expression's slot, and a function that
returns a new expression wrapping its argument, such as `convert_to`,
takes the expression by value, so a caller writes `e =
convert_to(std::move(e), t)`. `TypeCheckError` and
`constant_expression::ConstantExpressionError` are `CompileError`s of
those Python types, so a `catch` names the class as Python's `except`
does.

`constant_expression.h` has the evaluator and the validator, and
`NamedConstants`, the RAII form of Python's `with
named_constants(resolver):`, whose stack is kept per thread.
`designators.h` has `normalize_initializer`, which string_lifting
calls, and `stdbit_functions.h` has `add_definitions`. The parser's
folds (parser.py:2422-2663) make a checker of their own, fill its
public `symbols`, `types` (through `check_struct_decl`), and
`constexpr_values`, install its `named_constant` with
`NamedConstants`, and call `check_exp` or `check_block_var`.

Two things the Python objects carry don't fit the C++ IR. Python's
checker gives a `String` it checks a `data_type` attribute of
`char[N+1]`, which the dataclass takes though it has no such field, so
`check_exp` computes that type where Python reads it back, and so does
the evaluator for `sizeof "abc"`. And loop_labeling puts one expression
object in both a `SwitchStmt`'s `cases` and the body's `CaseStmt`, so
Python's check of the case value stamps both, while the dump holds two
copies, so `check_switch` copies each stamped value into the body's
`CaseStmt` with the same label.

On 21 Nov 2026 the port matched Python on all 1,467 distinct calls of
`type_checking` that the corpus makes in both modes, 313 of which
raise, with the builds of MSVC and GCC. The Python suite, run with
`check_program` wrapped by `capture_passes._wrap_front` in a pytest
plugin, made 3,204 distinct calls, and all of them matched too.
`tests/test_type_checking_calls.cpp` holds the 151 of them that reach
the 615 lines of type_checking.py, constant_expression.py,
designators.py, and stdbit_functions.py that the corpus doesn't reach,
found with a line trace, and the smallest call for each of the 149
error messages the corpus never gives, 223 calls in all.
`tests/test_designators.cpp` holds the unit tests' 314 distinct calls
of `normalize_initializer`, and `tests/test_constant_expression.cpp`
their 40 calls of the evaluator and the validator.
The same day `unroll_program` matched on all 1,562 distinct calls of
the corpus, `identifier_resolution` on all 1,566, 79 of which raise,
and `label_resolution` on all 1,487, 10 of which raise, in the builds of
MSVC 19.36 and GCC 12.2. The corpus rejects few programs, and none in
the unroller, so the calls that the Python suite makes were recorded
too, by a run of the whole suite with each pass wrapped by the
harness's `_wrap_front`, and every one of them matched: 2,157 distinct
calls of the unroller (30 raise), 4,236 of identifier resolution (128
raise), and 3,483 of label resolution (19 raise).
`tests/test_unroll.cpp`, `tests/test_identifier_resolution.cpp`, and
`tests/test_label_resolution.cpp` hold the distinct calls of each
pass's own Python test module whose records are under 6,000
characters, with Python's results, and from the rest of the suite one
call for each other message the pass raised, which is every message
the suite reaches: 55, 144, and 31 cases.

On 21 Nov 2026 string lifting matched Python on all 1,487 distinct calls
of the corpus in both modes, none of which raise, and on all 3,495
distinct calls the Python suite makes, recorded by a run of the suite
with `StringLifter.lift_program` wrapped by `_wrap_front`, 25 of which
raise TypeCheckError from `normalize_initializer`. A line trace of the
Python pass found two lines that only the suite's calls reach, the
`{"abc"}` of a character array and a call through an expression.
`tests/test_string_lifting.cpp` holds the 9 distinct calls of
tests/test_string_lifting.py, the smallest call that reaches each of
those lines, and one call for each of the 21 messages the suite's calls
raise, 32 cases.

### The parser's lexer and recognizer (group R1)

The C++ parser is a recursive descent (docs/cpp_port.md, "The plan for
the parser"), and group R1 ported what it stands on: Lark's lexer and
LALR table, parser.py's `_TypedefLexer` and scopes, and the syntax
errors. Parts R2 and R3 write the descent on top of these headers.

- `lark_table.h` declares the tables, which `cpp/generated/c99_lalr.cpp`
  holds. `tools/lalr_table.py` writes that file from the parser Lark
  builds of c99.lark with parser.py's four start rules: 976 states,
  55,242 actions and gotos over 112 terminals and 100 nonterminals, the
  715 rules with their origins, expansions, and aliases, and the 64
  terminals of the lexer in the order Lark tries them, with their
  patterns and the keywords. Lark numbers states by iterating sets of
  strings, so the script numbers them canonically, by a breadth-first
  walk from the start states, and two processes with different hash
  seeds write the same file. `tests/test_lalr_table.py` regenerates it
  and fails when it differs, so after editing c99.lark, run `uv run
  python tools/lalr_table.py`.
- `py_regex.h` matches the terminals' patterns with the semantics of
  Python's `re`, in which an alternation takes the first alternative
  that matches, so a match needn't be the longest. It parses the
  patterns the table holds, so a change to a pattern in c99.lark reaches
  the C++ lexer by regenerating the table. `tests/test_py_regex.cpp`
  holds Python's match on each of the 15 patterns for 700 spellings.
  The preprocessor's `--passthru-includes` uses the same module, which
  "The preprocessor" describes.
- `lark_lexer.h` is Lark's `BasicLexer`, `Token`, and
  `UnexpectedCharacters`, over the source as code points, since Lark's
  lines, columns, and context count code points. `lexer.h` is lexer.py:
  `check_number`, which the parser's lexer runs on each number, and
  `tokenize`, which `--lex` prints.
- `lark_parser.h` is the `Recognizer`, Lark's `ParserState.feed_token`
  without the value stack, which makes Lark's reductions, merged
  lookaheads included, and throws `UnexpectedToken` with Lark's
  `expected` and `accepts`, sorted as `_sort_expected` sorts them.
  `accepts` holds `$END` when the input could end there, since Lark's
  interactive parser feeds `$END` as the end of the input.
  `can_shift` is parser.py's `_can_shift`, and `reduce_through` makes
  a lookahead's reductions up to a given rule.
- `parser_context.h` has `Binding`, `Scope`, `TagHead`, and
  `ParseContext` with its lookups and `declare`, and `TokenStream`,
  which lexes, retypes an identifier to `TYPEDEF_NAME` where the
  recognizer can shift one, opens and closes the scopes of braces and
  `for`, and feeds each token it takes to the recognizer. `take` feeds
  the lookahead and lexes the next token at once, which keeps Lark's
  order between lexing and the actions of constructs that end at the
  token taken. The descent calls `end_for_scope` where parser.py's
  `for_stmt` runs. A scope is a `shared_ptr`, compared by identity.
  `raise_syntax_error` throws what `parse` raises for an
  `UnexpectedToken` or `UnexpectedCharacters`: `_c23_keyword_as_name`'s
  and `_enum_type_without_list`'s ParserErrors, the error itself, or
  the note about `__attribute__`.

The descent has no trees yet, so the pieces are checked on token
streams. `tools/capture_parser_tokens.py` records, for each source the
parser is given, the tokens Lark's parser was fed, the number of scopes
open after each, and how the parse ended, with the positions of the
identifiers the parser's scopes named as typedefs, which the record
gives the C++ side in place of the declarations that R2's actions will
make. As a pytest plugin it records the Python tests' calls of `parse`
and `tokenize` too.

```sh
uv run python tools/capture_passes.py --passes parse -j 12
uv run python tools/capture_parser_tokens.py captures
C6502_TOKEN_CAPTURES=captures PYTHONPATH=tools uv run pytest -p capture_parser_tokens -n 12 -q
cpp/build/bin/c6502cpp check-captures captures/parse_tokens captures/tokenize
```

On 21 Nov 2026 the 1,788 distinct sources of the corpus and the 4,526
more of the Python tests gave 6,314 `parse_tokens` records, of which
every one matched: each token's type, text, line, column, and scope
depth, and the class and message of each error, of which 429 records
end in a syntax error and 8 in a LexError of a number, with 277
distinct messages. The 452 distinct calls of `tokenize` matched too, 47
distinct LexErrors among them. `tests/test_parser_syntax_errors.cpp`
holds, for each distinct message, the shortest source that gives it.
A parse whose error came from an action of the parser, which 316
records hold, is checked up to the tokens shifted before it.

### The parser's descent (groups R2 and R3)

`parser::parse` (`parser.h`) is the port of parser.py's `parse`. It
builds the c99 program with a recursive descent written by hand, the
class `Descent` of `parser_descent.h`, which has one function per
construct of c99.lark and runs the construct's action, the code of the
`_ASTBuilder` method of the same name, when it has taken the
construct's last token and before it takes the next one. The tokens
come from R1's `TokenStream`, which feeds each one to the recognizer
before the descent sees it and lexes the token after it at once, and
that gives three things in Python's order.

- A syntax error is reported at Lark's token with Lark's message, since
  the recognizer runs Lark's table on every token. The descent reports
  none of its own: where it meets a token it has no use for, it takes
  it (`unexpected`), and the recognizer throws. A descent that strays
  from the grammar on a program Lark accepts throws InternalError.
- An action runs after the token that follows its construct is lexed
  and before that token is shifted, which is when Lark reduces the
  rule. So the scopes the lexer opens and closes, the names that
  `init_declarator` and the other actions declare, and the typedef
  names the lexer retypes follow each other as they do in Python.
- An action that throws runs inside `action(rule, ...)`, which makes
  the lookahead's reductions on a copy of the recognizer. Lark makes
  them when it is fed the lookahead, and where the lookahead turns out
  to be a syntax error before the reduction of `rule`, Lark never calls
  the action, and the syntax error is thrown in place of the action's.

The grammar's ambiguities that LALR settles by its lookahead are
settled by the lookahead here too. A block item is a declaration when
it starts with a declaration specifier, or with attributes and then a
specifier or a `;`. A statement that starts with an identifier takes
it and looks for a `:`, and hands the identifier to the expression
parser as its first operand (`pending_primary`) when there is none.
After `(`, a type name begins a cast or a compound literal, and a `{`
after `( type )` makes it a literal, after `sizeof` too. `(void)` is a
parameter list with no parameters, `else` goes with the nearest `if`,
and `enum e : T;` is an `enum_tag_decl` only at the start of a
declaration. The end of a `for` statement calls `end_for_scope`
where parser.py's `for_stmt` pops the loop's scope.

The parser's folds (`fold_integer_constant`, `expression_type`, and
`fold_named_constant`, in `parser_folds.cpp`) make a `TypeChecker` of
their own for each array size, enumeration value, `_BitInt` width,
designator index, `constexpr` initializer, and operand of `typeof` and
`auto`, with the named constants and struct bodies seen so far, as
parser.py does. `parser_walk.h` has the walks over the AST that Python
makes with `dataclasses.fields`, for `_names_in`, `_named_literals`,
`_has_literal`, and `_mark_hw_stack_save`. The declarations' internal
types, the specifier lists, the declarator trees, and the parameter
lists, are in `parser_declarations.h`, and the stack of declaration
specifiers is in `ParseContext`.

On 21 Nov 2026 `parse` matched parser.py on all 1,788 distinct calls
of the corpus, 221 of which raise, and on all 5,163 distinct calls that
the Python suite makes, recorded by a pytest plugin that wraps `parse`
with `capture_passes._wrap_front`, 693 of which raise, those that fail
in an action included. That is 6,253 distinct sources, 751 of which
raise, each compared by the program's dump or the error's class and
message, in the builds of MSVC 19.36 and GCC 12.2.
`tests/test_parser_errors.cpp` holds the shortest source for each of
the 506 distinct messages, and `tests/test_parser_declarations.cpp` and
`tests/test_parser_statements.cpp` hold cases of each part recorded
from Python.
### The preprocessor

`preprocessor::preprocess(source, argv, filename, host, include_dir)`
is preprocessor.py's `preprocess`: pcpp 1.30 with preprocessor.py's
hooks, its flags read as argparse reads them, and `_strip_pragmas` after
it. It returns the text and what Python writes to stderr (pcpp's
`<file>:<line> error:` lines, `#error` and `#warning`, the NOTEs about
unknown flags, and the `--time` report), issues the `_Bool` warnings
through `diagnostics::warn`, and throws a `CompileError` of the Python
type for a `PreprocessorError` or any exception that escapes pcpp, such
as the IndexError of `#define` alone or the RecursionError of a macro
that expands inside its own argument. A `pcpp::Host` gives it what
Python reads from the machine: the rules of `os.path` (ntpath or
posixpath) and the current directory, the encoding Python opens a
header with, and the files. `pcpp::native_host()` is this machine's,
and the tests' host holds only the files a record lists.

The port follows the plan in docs/cpp_port.md, part by part.

- **The lexer** (`pcpp_lexer.*`) tests pcpp's rules in the order of
  PLY's master regular expression, which decides the token where
  several rules match, and reproduces the consequences: `^=` is two
  tokens, a lone `=` at the start of an input is a CPP_XOREQUAL, an
  unclosed quote or comment is punctuation, and a character no rule
  takes is a `Char` token whose type is the character.
- **The tokens** (`pcpp_token.h`) are shared pointers, since pcpp
  copies a token with `copy.copy`, which shares its `expanded_from`
  list, and changes tokens in place where several lists hold them.
- **The text** (`pcpp_text.*`): `str.splitlines`, `rstrip`, `strip`,
  `split`, and `repr` with Python's Unicode classes, the functions of
  ntpath and posixpath, and the decoding of a header with cp1252,
  UTF-8, Latin-1, or ASCII and universal newlines, with Python's
  `UnicodeDecodeError` messages.
- **pcpp** (`pcpp_preprocessor.*`): `group_lines`, the macros (define,
  undef, the prescan, the collection of arguments, `macro_expand_args`,
  `expand_macros`), `evalexpr`, `parsegen` with every directive and the
  include-guard heuristic, `include` with its search and pcpp's
  rewriting of paths, and `write`. pcpp's `parsegen` is a generator that
  `write` drains, and the C++ hands each token to `write`'s consumer
  where Python yields it, so a `_Bool` before an error is warned about
  only when Python warns about it.
- **The evaluator** (`pcpp_evaluator.*`) runs PLY's parse loop on the
  tables of the live parser, with Python's arithmetic on `Value`.
- **preprocessor.py** (`preprocessor.*`): `CompilePreprocessor`'s hooks,
  `_NumberEvaluator`, `_check_pragma`, `_strip_pragmas`, and
  `parse_args`.

`--debug` creates pcpp's log and writes nothing to it, which can't
matter to a compile. pcpp stops a macro that expands inside its own
argument when Python's stack runs out, which depends on how deep the
caller's stack is, so the C++ stops it at 300 nested expansions
(`kMaxExpansionDepth`).

pcpp compiles the pattern of `--passthru-includes` with
`re.compile(pattern)` and matches it with `.match` against the text of
an `#include` line's tokens, and py_regex does both with Python 3.11's
syntax and semantics. Its parser is a port of `re/_parser.py`, so an
invalid pattern gets Python's exception, with its class and message,
such as `re.error` with "missing ), unterminated subpattern at position
0", "nothing to repeat", "bad escape \q", "unterminated character
set", or "bad character range b-a". The compiler follows
`re/_compiler.py`, which picks the ASCII or Unicode forms of `\w`, `\b`,
and the rest, the multiline forms of `^` and `$`, and the folding of
case under `(?i)`, which reads Python's simple case mappings and
`re._casefix._EXTRA_CASES` from pcpp_unicode.h and keeps `_sre`'s
quirks for characters outside the Basic Multilingual Plane. The matcher
follows `_sre`'s order of backtracking, including the rule that ends a
repeat when an iteration matches the empty string. compile.py doesn't
catch `re.error`, so the class reaches `cpp_backend.py` as "error",
which it raises as `re.error`.

py_regex throws `Unsupported` for what it doesn't take, and the
preprocessor turns that into a NotImplementedError, which compile.py
reports as an error that names the construct. Those are
backreferences, conditionals, `\N{name}`, group names with non-ASCII
characters, a class for which Python prints a FutureWarning on stderr
(`[[`, or `--`, `&&`, `~~`, or `||` inside a class), and a possessive
repeat of anything but one character or class. Python 3.11.3's
POSSESSIVE_REPEAT doesn't restore its position after an iteration past
the minimum fails, so the rest of the pattern goes on from wherever the
failed iteration's last step left it, and `(x|(?<=a))++` matches the
empty string at the start of "x".

`tools/py_regex_records.py` writes records of Python's `re` on
generated patterns and subjects, and `c6502cpp check-regex` compares
py_regex with them, counting apart the patterns it reports as
unsupported. On 21 Nov 2026 the 27,177 records of seeds 1 and 2 gave no
difference, 4,013 of them Python's errors and 320 of them unsupported,
over 1,039,520 matches. `tests/test_py_regex_records.cpp` holds 589
of those records and 52 recorded calls of `preprocess` with
`--passthru-includes`, for headers that an `#include` opens and ones it
skips as already seen.

```sh
uv run python tools/py_regex_records.py regex.jsonl --count 5000
cpp/build/bin/c6502cpp check-regex regex.jsonl
```

pcpp's evaluator expands the escapes of a character constant with
Python's `unicode-escape` codec, which reads Python's `\N{name}` too, so
pcpp gave `'\N{LATIN SMALL LETTER A}'` the value 97 in an `#if`. C has no
such escape (C23 §6.4.4.4), and c6502's lexer rejects that constant
outside an `#if` as it rejects `'\q'`, so preprocessor.py rewrites each
character constant before pcpp's evaluator reads it
(`_without_named_escapes`). The rewritten constant holds the expansion of
the other escapes with each backslash doubled, so pcpp reads `\N` as two
characters and reports the TypeError of `ord()` for the constant, as it
does for `'\q'`. The C++ rewrites the constant the same way in
`call_evaluator`, with `pcpp::expand_escapes_without_names`, and the
evaluator's own `\N{name}` branch, which no compile reaches, still throws
`InternalError`. `tests/test_preprocessor_named_escapes.cpp` holds 34
recorded calls with such constants.

`preprocess` is checked three ways. Its captures (`--passes preprocess`)
hold the source, the arguments, the file name, the text or the error,
the warnings, and a `stderr` line with what the call wrote to stderr,
and the runner reads the included files from their paths, so
`check-captures` runs from the repository's root. The Python unit
tests' calls were recorded by a pytest plugin with every file each call
opened, and `c6502cpp check-preprocess <dir>` checks such records, one
JSON object per `.json` file (src/preprocessor_cases.h). A selection of
them, with calls on generated inputs that reach what the corpus and the
tests leave out, is in `tests/test_preprocessor_calls.cpp`, and
`tests/test_pcpp_lexer.cpp` and `tests/test_pcpp_text.cpp` compare the
lexer and the text functions with Python's.

```sh
uv run python tools/capture_passes.py --passes preprocess -j 16
cpp/build/bin/c6502cpp check-captures captures/preprocess
cpp/build/bin/c6502cpp check-preprocess <directory of records>
```

On 21 Nov 2026 the C++ matched on all 2,088 distinct `preprocess` calls
of the corpus, all 4,981 distinct calls of the Python unit tests, and
2,205 calls on generated inputs, errors, warnings, and stderr included.

## The TAC optimizer's driver

`passes::framework::PhaseDriver` (`src/passes/optimization/driver.h`)
is the port of `PhaseDriver.apply`. Its six lists hold `NamedPass`
entries, each a pass's `name` and a pointer to its port, and it runs
them as Python does, with `to_ssa` and `from_ssa` between them and the
two fixed points swept until a sweep leaves the function equal to what
it started with, or 256 times. `ctx.ssa_dsts` stays set after SSA
destruction, as Python's does, so the passes after it see the SSA
names. The C++ driver doesn't check the TAC after each pass under
`C6502_CHECK_IR=1` and has no `observer`, which only fuzz/tac_passes.py
sets.

`passes::tac_driver()` (`src/passes/optimization/optimizer.cpp`) is
Python's `_DRIVER`, with every pass in its list and its order. A pass
whose group hasn't landed has a null pointer there, and the group that
ports it sets the pointer, which is one line of the table and an
`#include`. The driver throws `InternalError` at a null entry, and
`tac_driver().complete()` reports whether the table is full. Until it
was, the runners of `tac_optimize_function` and `optimize_tac` in
tac_stage.cpp were null, so `check-captures` read those records
without running them, and with every entry filled the runners call
`optimize_function` and `optimize_program`. On 21 Nov 2026 the whole
driver matched Python's on all 4,054 distinct calls of
`tac_optimize_function` of the corpus, the inliner's estimates of cost
among them, and on all 1,146 of `optimize_tac`. `tests/test_tac_driver.cpp`
checks the table's names against the signatures of tac_stage.cpp,
which tests/test_capture_passes.py checks against Python's lists, and
runs a driver of the passes ported so far on three functions, with the
results of Python's `PhaseDriver` of the same lists.

## c99_to_tac

Group A of step 5 ported `c99_to_tac.translate_program` as
`c99_to_tac::translate_program` (`src/c99_to_tac.h`), which takes the
c99 program, the symbol table, and the type table (null where Python's
is None) and changes all three in place, as Python's does. The Python
class `Translator` is a struct in `src/c99_to_tac_translator.h`, with
the lowering split over four files in the parts of docs/cpp_port.md:
`src/c99_to_tac.cpp` (the free functions of the module, the queries
every part reads, the driver, the builtins, and the declarations, part
A1), `c99_to_tac_exp.cpp` (statements, calls, expressions, and pointer
arithmetic), `c99_to_tac_lvalues.cpp` (initializers, lvalues, and
subscripts), and `c99_to_tac_conversions.cpp` (increments, compound
assignments, casts, the bit-precise padding, and the short-circuit
operators). The public header gives other groups `tac_const_for`
(Python's `_tac_const_for`, which `static_const_fold` and constant
folding call), `sizeof_c99` (`_sizeof`, for `row_address_tables`),
`byte_width_of` (`_byte_width_of`, for the inliner), and
`kKeepLoopMark`.

Python walks every field of every c99 node to erase `short` and
`nullptr_t`, and `ProgramWalk` in `c99_to_tac.cpp` visits the same
fields. Python changes shared type objects in place, where C++ has
values, but every copy of a type gets the same replacement, so the
result is the same. A temporary is added to the symbol table, an
`OrderedMap` whose entries move when it grows, so the lowering never
holds a reference to a symbol across the creation of a temporary.
Python's `_fp_to_integer` converts a double to the whole integer it
holds, which can have 1,024 bits, and reduces it modulo 2^N, and the C++
port reduces fp_arith's `TruncatedInt` with `wrapped`.

It reads group C's ports of `fp_arith` (`src/fp_arith.*`) and
`builtin_functions` (`src/builtin_functions.*`), and `bit_precise`
gained `value_range`, `has_padding`, and `storage_type` for it.

The runner (`src/c99_to_tac_stage.*`) gives the `mutated` line with the
program after the call under `input` when the program changed and with
each table that changed by its position. On 21 Nov 2026 the capture of
the whole corpus in both modes gave 1,154 distinct calls, 199 of which
changed the program, and every one matched with MSVC and with GCC. A
line trace of c99_to_tac.py, builtin_functions.py, fp_arith.py, and
bit_precise.py over the corpus and over the 2,829 distinct calls that
the unit tests make found 253 lines that only the unit tests reach, most
of them the <stdbit.h> loops and the folding of the builtins, which no
program of the corpus uses. `tests/test_c99_to_tac_calls.cpp` holds 50
of those calls, recorded with their results through
`tests/stage_case.h`, which reach all 253, and all 2,825 of the unit
tests' calls that return matched too. `tests/test_c99_to_tac_helpers.cpp`
checks the additions to `bit_precise` and the fold of a cast of a
floating constant on values the Python functions give.

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
| `optimization/cfg.py`, the TAC CFG | `src/passes/optimization/cfg.*` | `passes::tac_cfg` |
| `recursion_saves._live_out`, `defs_in` | `src/passes/recursion_saves.*` (`live_out`, `defs_in`) | `passes::recursion_saves` |

The TAC optimizer's passes of step 5 share these.

| Python | C++ | Namespace |
|---|---|---|
| `optimization/var_visit.py`'s `rewrite_uses_in` | `src/passes/optimization/var_visit.*` | `passes` |
| `optimization/value_range.py` | `src/passes/optimization/value_range.*` | `passes::value_range` |
| `optimization/framework/` (the run loops, `PassContext`, `DefUseEnv`, `RuleEnv`, `single_use`) | `src/passes/optimization/framework.*` | `passes::framework` |
| `fp_arith.py` | `src/fp_arith.*` | `fp_arith` |
| `builtin_functions.py` | `src/builtin_functions.*` | `builtin_functions` |
| `constant_folding._fold`, `_is_integer_const` | `src/passes/optimization/constant_folding.*` (`fold`, `is_integer_const`) | `passes::constant_folding` |
| `strength_reduction._scalar_int_size`, `_zero_for_c99_type`, `_two_power_terms` | `src/passes/optimization/strength_reduction.*` | `passes::strength_reduction` |
| `c99_to_tac._tac_const_for` | `src/c99_to_tac.*` (`tac_const_for`) | `c99_to_tac` |

`constant_folding::fold` returns a `FoldResult`, whose outcome is
`kUnchanged` where Python returns the instruction itself, `kDropped`
where it returns None (a jump never taken), and `kReplaced` with the
folded instruction. `fp_arith` takes and returns the bit patterns as
`asdl::Int`s. It converts an integer or a decimal string to a `float`
through a `double`, as numpy does, so the result is rounded twice, and
`single_bits_to_int` and `double_bits_to_int` return the integer part
as a `TruncatedInt`, since a double's can take 1,024 bits. A caller
reduces it modulo 2^N with `wrapped`, or takes it whole with `exact`
when it fits in an `asdl::Int`, as c99_to_tac does for a conversion
to `bool`. `c99_to_tac::tac_const_for` is the piece of c99_to_tac.py
that the folding passes read, which group A's port of the module
("c99_to_tac") keeps with the same signature. `distribute_constant` reads group F's `is_1_byte_val`.

`rewrite_uses_in` takes a mapper that returns none to leave an operand,
where Python's returns the same object, and returns none when nothing
changed, where Python returns the same instruction.

`uses_in` and `defs_in` return the names as `std::string_view`s into
the instruction, since a `tac_ast::Var` is only its name, and
`share_conflicts` returns the indices of the shares where Python
returns the shares.

The TAC CFG is in namespace `passes::tac_cfg`, since the asm CFG of
`src/passes/optimization_asm/cfg.h` has the same names. Its blocks own
copies of their instructions, as the Python blocks hold lists of their
own, so a pass can edit them and flatten the graph with
`cfg_to_function`. Python keeps a dominance frontier and a loop body in
a `set` of block ids, and SSA construction and the loop passes walk
those sets, so their order reaches the output. The order of a set of
ints doesn't change from run to run, since an int's hash is its value,
but it is the order of the slots of CPython's hash table.
`src/py_int_set.h` keeps the same table, with the probing and resizing
of CPython 3.11's `setobject.c`, and iterates as CPython does. It gave
CPython 3.11.3's order for 200,000 random sequences of additions.
`recursion_saves::defs_in` differs from var_visit's `defs_in` only for
an `IndirectIndexedLoad`, whose destination it counts.

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

## The stage's driver

`passes::optimized_front` (`src/passes/optimized_front.*`) is compile.py's
`_optimized_front` after the TAC passes of `_optimized_tac`: `select_abi`,
the two carry-return choices, `recursive_pool_functions` and
`pool_eligible`, `find_shares`, `allocate_zp_slots`,
`pinned_param_copies`, `slot_aliases`, `tac_to_asm`, the asm optimizer's
preliminary run, the sizing of the pools, the final run with the pools
grown up to three times (`_POOL_RETRIES`), the address-taken slots,
`replace_pseudoregisters_bare_exit`, the names and `EQU` table of the
pools' bytes, `lower_data_load_address`, and `synthesize_prologue`, each
with the arguments compile.py gives it, in compile.py's order, since
several change the symbol table that later ones read. It returns what
`_OptimizedFront` holds besides the TAC.

`passes::codegen` (`src/passes/codegen_stage.*`) runs the whole of
code generation from the type-checked c99 program:
`translate_for_codegen`, the names of static storage that `_codegen`
takes at that point, and under `-O` a copy of the TAC for
`plan_zero_page`, `optimized_tac`, and `optimized_stage`, and without
it `unoptimized_stage`. It keeps the TAC it lowered for the IR checks.

`passes::optimized_stage` and `unoptimized_stage` run the stage from
the TAC. Each starts with
`plan_zero_page`, whose HARGS measurement lowers the unoptimized TAC
with `tac_to_asm` in a default `AsmContext`, since Python measures it
before `zero_page_in_effect` sets the plan. The plan's layout and
objects then make the `AsmContext` of the rest of the compile. Under
`-O` the stage runs `optimized_front` on the optimized TAC, builds the
link metadata's records of the TAC and the tables, and runs
`optimized_text` (or `optimized_program` for the simulator harness).
Without it the stage runs `fixed_address_layouts`, `tac_to_asm` with that
ABI, `replace_program`, and `unoptimized_text` (or the unoptimized
region and `place_zp_globals`). Each returns the changes to the symbol
table that Python enters in its own: the `<param>.reg` locals
`tac_to_asm` added, the functions the carry-return choices gave the
carry-flag return, and the functions the region made.

Python plans the zero page before the TAC optimizer, and the driver
plans it after, with the symbol table the optimizer left. The
optimizer adds locals, the halves of the tables of row addresses, and
temporaries, and gives a split table a type of bytes. `allocate_zp_globals`
places none of those and promotes none (it promotes scalars with
weight in the unoptimized TAC), and the names it had keep their order,
so the plan comes out the same, which `tools/compare_backends.py`
confirmed on the corpus. The names of static storage that
`replace_pseudoregisters` and the asm optimizer take as `extra_statics`
are those before the optimizer added the tables, as in `_codegen`, so
Python sends them.

A declaration may leave a parameter unnamed, which the Python c99
program holds as None in `function_decl.params`, and the dump format
has no form for None in an `identifier*`. `cpp_backend.py` sends such a
program with the empty string there, which no C identifier spells, and
`select_abi` and `fixed_address_layouts` spell that parameter's slot by
its position, as Python does. The capture harness couldn't record
those calls, since it dumps the program too, so the first run of the
suite with the stage in C++ found them (tests/test_linker.py's unnamed
parameters), and `tests/test_backend.cpp` holds such a request.

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

`passes::front_end` (`src/passes/front_end.*`) is compile.py's front
end, `_compile_c` from the source text through `type_check_program`, as
one function: `preprocessor::preprocess` with the arguments, the file
name, and the directory of c6502's headers, `parser::parse`, under `-O`
`unroll_program`, `resolve_identifiers`, `lift_strings`,
`resolve_labels`, `label_program`, and `check_program`, which returns
the program and its two tables. The preprocessor's warnings go to the
caller's `diagnostics::Collecting` block, and what it writes to stderr
is kept when a later pass throws.

`c6502cpp backend` (`src/backend.*`) reads a request on stdin, runs
`passes::front_end` on its source text and `passes::codegen` on the
result ("The stage's driver", above), and writes the response.
`src/backend.h` describes the request and the response. Every response
begins with what the preprocessor wrote to stderr and the warnings, and
a problem with the program (`CompileError`) ends the response with an
`error` line, the Python class and the message, so that the warnings and
stderr of a rejected program come back too. A bug the passes report
(`InternalError`) exits with status 3.

With `C6502_CPP_BACKEND=1`, compile.py's `_compile_c` hands the source
text of each C input of `-S`, `-c`, and a link to `c6502cpp backend`
through `cpp_backend.compile_source`, one process per compile, with the
preprocessor's arguments and the file name (None for stdin), and the
`-S` text comes back. The process inherits Python's current directory,
so the paths in the preprocessor's messages are the same. `--lex`,
`--parse`, `--resolve`, and `--tac` stay in Python.
`_run_stage("codegen", text)`, which the tests call with the
preprocessor's output, sends the text with `preprocess` off. The
request says whether `-O`
inlines, which is false when a test has patched
`compile.inline_functions` (tests/no_inlining.py, `compile._inlining`).
The simulator harness sends the source text too and takes back the
finished asm program, which it assembles. The program and its tables
stay in C++, since Python reads nothing of them afterward.
`cpp_backend.py` writes the preprocessor's stderr to `sys.stderr`,
issues the warnings again through `diagnostics.warn`, and raises an
`error` line as the Python exception of that class with that message
(`_exception`, which makes a subclass of the same name for a class such
as Lark's `UnexpectedToken` whose constructor takes other arguments, and
which turns argparse's exit into `SystemExit(2)`). An exit status of 3
is raised as an `AssertionError` with the C++ message. Under
`C6502_CHECK_IR=1` it checks the TAC that code generation lowered, the
finished asm program, and the asm2 program of a text once each, since
`ir_check` isn't ported. compile.py reads the variable at each compile
and, without it, imports none of this. Run as a script with the
variable set and `-S`, `-c`, or a link, compile.py stops before it
imports its pipeline (`_cpp_runs_everything`), and the runtime helpers'
footprints come from `passes.runtime_layout`, which reads
`TypeCheckError` from `passes/type_check_error.py`, so neither `-S` nor
`-c` imports the parser, Lark, pcpp, or a pass of the front end, unless
`C6502_CHECK_IR=1` loads `ir_check`, whose helpers import the type
checker.

`tools/compare_backends.py` compiles the corpus of the capture harness
with `-S` and `-S -O`, once with the variable and once without, and
requires the same `-S` text, exit status, messages, warnings, and
exceptions.

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
text, and `optimized_program` runs the region and the steps of
`optimized_asm` after it (`link_records`, `place_zp_globals`, and
`check_shares`) for the simulator harness.

Since the stage's driver runs code generation in C++ too, everything
these read comes from C++ except the HARGS footprints of the runtime
helpers (`runtime_layout.helper_footprints`), which are measured from
rtlib/helpers.py, which stays in Python with the simulator, and go in
the request. The link metadata's records that read the TAC, the ABI,
and the symbol table (`build_metadata`, `changed_param_slots`, the
carry functions, `preferred_symbols`, and the `ext` records of the
unoptimized pipeline) are computed in the driver after the stage, as
compile.py computed them after `_optimized_front`, and so is what
`sections.items` reads of each top-level name's symbol
(`section_info` in `src/passes/codegen_stage.cpp`). A function that
`shared_tails` or `outline_runs` makes takes its owner's section, as
its Python symbol is a copy of the owner's.

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
- 20 Nov 2026: group A of step 4, the ABI and the call graph ("The ABI
  and the call graph"): `select_abi`, `fixed_address_layouts`,
  `pinned_slot_symbols`, the carry-return choices, the pool eligibility,
  `recursion_saves`, and the TAC CFG, each matching the Python function
  on every distinct call of the corpus, with `PyIntSet` for the sets of
  block ids whose order the TAC passes read.
- 20 Nov 2026: group C of step 4, `tac_to_asm` ("Code generation:
  tac_to_asm"), with `significant_bytes`, `in_place_member_update`,
  `pointer_restage`, `shift_cost`, and the pieces of `bit_precise`,
  `split_tables`, and `compare_subtract` it reads. It matches the Python
  function on all 2,256 distinct calls of the corpus in both modes,
  with MSVC 19.36 and GCC 12.2.
- 20 Nov 2026: group B of step 4, sharing and the zero-page allocators
  ("Sharing and the zero-page allocators"): `find_shares` and the rest
  of `slot_sharing` that reads the TAC, `allocate_zp_slots`,
  `allocate_function_locals`, `build_local_slot_symbols`,
  `zp_globals`' plan, `choose_layout`, and `build_metadata`, each
  matching the Python function on every distinct call of the corpus,
  10,838 calls in all, and on 551 calls that the unit tests make or
  that reach the allocators' errors.
- 20 Nov 2026: part F2 of step 4, the end of the stage ("The end of the
  stage"): `compute_address_taken_assignments`, `slot_symbols`,
  `local_slot_names`, `compute_local_slot_names`, both forms of
  `replace_pseudoregisters`, and `lower_data_load_address`, each
  matching the Python function on every distinct call of the corpus,
  9,735 calls in all, on the 66 distinct calls of the unit tests, and on
  three cases written by hand.
- 20 Nov 2026: the asm optimizer's driver (group F1 of step 4, "The asm
  optimizer's driver"): `optimize_program`, `optimize_function`,
  `apply_coloring`, `fold_const_statics`, the elimination of dead
  statics, and the sizing of the pools, each matching the Python
  function on every distinct call of the corpus, 21,332 calls in all.
- 20 Nov 2026: the stage's driver (the end of step 4, "The stage's
  driver"): `optimized_front` and the stage from the TAC to the text,
  `optimized_stage` and `unoptimized_stage`, which `c6502cpp backend`'s
  stage modes run, so that `C6502_CPP_BACKEND=1` hands C++ the TAC.
  `tools/compare_backends.py` found the same output for all 3,576
  compiles, and the C++ tests pass in the builds of MSVC 19.36 and GCC
  12.2.
- 21 Nov 2026: the groundwork of step 5 ("The TAC optimizer's calls"):
  the captures of every call of the TAC optimizer's driver and of the
  program-level calls of step 5, with their signatures in
  `src/passes/optimization/tac_stage.cpp`, which read all 355,075
  distinct records of the corpus back; `rewrite_uses_in`,
  `value_range`, and the framework's run loops, which match on 26,982
  helper records; and `fold_truncate_extend` and `fold_extend_extend`,
  which match on every distinct call of the corpus. The build is clean
  with MSVC 19.36 and GCC 12.2.
- 21 Nov 2026: group B of step 5, the TAC optimizer's driver ("The TAC
  optimizer's driver"): `PhaseDriver` with its two fixed points and
  their cap, the table of `_DRIVER`'s passes, `optimize_function`,
  `optimize_program`, `to_ssa`, `from_ssa`, and the two passes before
  SSA, `rotate_signed_countdown_loops` and `noreturn_calls`. Each pass
  matches the Python function on every distinct call of the corpus,
  3,992 of `tac_to_ssa`, 3,746 of `tac_from_ssa`, 3,986 of
  `tac_rotate_signed_countdown_loops`, and 3,987 of
  `tac_noreturn_calls`, and on the 31 distinct calls of the Python unit
  tests (`tests/test_tac_ssa_calls.cpp`). The build is clean with MSVC
  19.36 and GCC 12.2.
- 21 Nov 2026: group F of step 5 ("Addressing and sinking"): the three
  addressing-mode recognizers and their shared walk,
  `direct_member_access`, `static_indexed_access`, `phi_offset`,
  `sink_increments`, and `sink_and_past_branch`, each matching the
  Python pass on every distinct call of the corpus, 58,157 calls in
  all, and on the calls of the Python unit tests, in the builds of
  MSVC 19.36 and GCC 12.2.
- 21 Nov 2026: group C of step 5, folding: `constant_fold` (with the
  loads from constant arrays and from tables split into byte planes),
  `fold_into_constant_phi`, `fold_static_const_reads`,
  `distribute_constant`, `reduce_strength`, `reassoc_constants`, and
  `reassoc_offset_constant`, with `fp_arith` and `builtin_functions`.
  Each pass matches the Python pass on every distinct call of the
  corpus, and on the 273 calls the Python unit tests make that the
  tests `test_<module>_calls.cpp` hold. Their entries in the driver's
  table, and group F's, are filled.
- 21 Nov 2026: group A of step 5, `c99_to_tac` ("c99_to_tac"), with
  the parts of `bit_precise` it reads. It matches the Python function
  on all 1,154 distinct calls of the corpus, the in-place changes to
  the program and the tables included, and on the 2,825 distinct calls
  of the unit tests that return, with MSVC 19.36 and GCC 12.2.
- 21 Nov 2026: group E of step 5, dataflow over the CFG ("Dataflow over
  the CFG (group E)"): `copy_propagate`, `eliminate_dead_stores`,
  `eliminate_unreachable_code`, `eliminate_dead_loops`,
  `thread_constant_jumps`, `eliminate_common_subexpressions`,
  `hoist_loop_invariants`, both phases of copy folding, and
  `read_stored_globals`. Each matches the Python pass on every distinct
  call of the corpus, 108,880 calls in all, and on the calls of the
  Python unit tests, in the builds of MSVC 19.36 and GCC 12.2. Their
  entries in the driver's table are filled.
- 21 Nov 2026: group D of step 5, the jump folds and the narrowing
  ("Jump folds and narrowing (group D)"): the seven rules of
  `fold_producer_jumps` from `cmp_zero_jump_fold`,
  `and_zero_jump_fold`, `lnot_jump_fold`, and `nonnegative_compare`,
  `narrow_widened_arith`, `narrow_byte_range`, and
  `narrow_byte_bitwise`, with `extend_source_unchanged`. Each pass
  matches the Python pass on every distinct call of the corpus, 40,172
  calls in all, and on the 235 calls of the Python unit tests that the
  tests hold, in the builds of MSVC 19.36 and GCC 12.2. Their entries in
  the driver's table are filled.
- 21 Nov 2026: group G of step 5 ("After the fixed point and after
  SSA"): `hoist_subtract_to_compare`, `hoist_divmod_pairs`,
  `count_loops_down`, `move_steps_onto_edges`,
  `split_three_way_compares`, `fold_short_circuit_jump` with
  `merge_equality_jumps`, and `thread_known_compares`, each matching the
  Python pass on every distinct call of the corpus, 27,758 calls in
  all, and on the 358 distinct calls of the Python unit tests, in the
  builds of MSVC 19.36 and GCC 12.2. Their entries in the driver's
  table are filled.
- 21 Nov 2026: group H of step 5, the program level ("The program
  level (group H)"): `inline_functions`, `return_pointers_in_zero_page`,
  `apply_row_address_tables`, `dispatch_const_pointer_arrays`, and
  `apply_split_tables`, each matching the Python function on every
  distinct call of the corpus, 5,662 calls in all, and on the calls of
  the Python unit tests, in the builds of MSVC 19.36 and GCC 12.2. The
  whole TAC optimizer's driver matches on every call of the corpus.
  `passes::codegen` runs everything after the type checker, and with
  `C6502_CPP_BACKEND=1` compile.py hands C++ the type-checked c99
  program. `tools/compare_backends.py` found the same output for all
  3,576 compiles.
- 21 Nov 2026: the groundwork of step 6 ("The front end's calls"): the
  captures of the front end, with their errors and warnings, whose
  20,177 distinct records of the corpus all make the trip; pp_numbers,
  diagnostics, Python's repr of the IR values, and the type checker's
  type queries, each matching the Python code on the corpus; and
  `loop_labeling`, which matches on all 1,477 distinct calls of the
  corpus and on the Python unit tests' programs, in the builds of MSVC
  19.36 and GCC 12.2.
- 21 Nov 2026: groups N and U of step 6, `identifier_resolution`,
  `label_resolution`, and the unroller, each matching the Python pass
  on every distinct call of the corpus and of the Python suite, errors
  included ("The front end's calls").
- 21 Nov 2026: group R1 of step 6, the parser's lexer and recognizer
  ("The parser's lexer and recognizer"): the LALR table and the lexer's
  terminals generated from c99.lark by Lark itself, with a test that
  regenerates them; Lark's basic lexer, with Python's `re` for the
  terminals' patterns; lexer.py; the recognizer with Lark's syntax
  errors; and parser.py's scopes, typedef-aware lexer, and rewordings.
  Every one of the 6,314 distinct token streams of the corpus and the
  Python tests matched, and so did all 452 calls of `tokenize`.
