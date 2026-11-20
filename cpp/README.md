# The C++ port of c6502

This directory holds the port of the compiler from Python to C++.
`docs/cpp_port.md` records the decisions and the order of the stages.
Each stage moves to C++ only when it produces the same output as the
Python code it replaces, byte for byte, and the Python compiler stays
the reference until the port is done.

So far the port has the build, the IR classes generated from the four
`.asdl` files, and the dump format that carries an IR value between
the two compilers.

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
