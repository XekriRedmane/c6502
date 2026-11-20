# The C++ port of c6502

This directory holds the port of the compiler from Python to C++.
`docs/cpp_port.md` records the decisions and the order of the stages.
Each stage moves to C++ only when it produces the same output as the
Python code it replaces, byte for byte, and the Python compiler stays
the reference until the port is done.

So far the port has the build, its test runner, and the support code
the generated IR classes will use.

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
`cpp/build/bin/debug`. `cpp/build/` is ignored by git.

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
- `c6502cpp`, the CLI (`src/main.cpp`), which has no commands yet.

Warnings are on (`/W4`, `-Wall -Wextra`) and aren't errors yet.

## Layout

```
cpp/
  CMakeLists.txt
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
