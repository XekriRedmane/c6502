# The C++ port of c6502

This directory holds the port of the compiler from Python to C++.
`docs/cpp_port.md` records the decisions and the order of the stages.
Each stage moves to C++ only when it produces the same output as the
Python code it replaces, byte for byte, and the Python compiler stays
the reference until the port is done.

So far the port has the build, the IR classes generated from the four
`.asdl` files, the dump format that carries an IR value between the
two compilers, and one asm pass, `split_mem_to_mem`, which matches the
Python pass on every call the corpus makes.

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
  and `generated/`). A `.cpp` file added under `src/`, `src/asdl/`, or
  `src/passes/` is picked up the next time CMake runs.
- `c6502_tests`, the unit tests in `tests/`, on doctest.
- `c6502cpp`, the CLI (`src/main.cpp`), which for now holds the checks
  below.

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
    capture.*         the reader of capture files
    passes/           one .cpp/.h per Python pass module, mirroring passes/
      registry.*      the ported passes, by name, for check-captures
    main.cpp          the c6502cpp CLI
  tests/              doctest unit tests, test_<module>.cpp
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
  `asm_ast::Program apply_x(asm_ast::Program prog, ...)`, and rewrites
  a function in place only when it changes something, so a caller that
  moves the program in copies nothing. Keyword arguments become
  parameters in the same order, with the Python defaults.
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
  `tools/capture_passes.py`'s `PASSES`, with a function that decodes
  the call's tagged-JSON arguments and returns the result's dump. The
  Python tests of the pass (`tests/test_<name>.py`) are ported to
  `tests/test_<name>.cpp`. A pass is done when `tools/check_captures.py
  <name>` reports no difference on a capture of the whole corpus.

## Status

- 19 Nov 2026: the groundwork landed. The CMake build with MSVC, GCC,
  and Clang; doctest; the C++ IR generated from all four `.asdl`
  files; the dump format on both sides, with the Python round trip and
  the Python-to-C++-to-Python byte check on a compiled example's four
  IRs; the capture harness for the passes of `optimized_asm` from
  `apply_licm` on; and `split_mem_to_mem`, which matches on all 7049
  distinct calls of the corpus.
