# The checks after Python

- `c6502cpp check-golden -j 12` checks the C++ compiler against golden/ (about a minute with MSVC, 40 s with GCC in WSL), and `c6502cpp check-frozen` against cpp/tests/frozen (28 s on Windows, 4 s on Linux, where writing the unpacked records is cheaper). cpp/README.md, "The checks after Python", has the formats and how to accept a change with `check-golden --update`.
- The golden files name the repository's files relative to the root, and both sides take `<root>/` out of every message. A command's argument counts as such a path only when it starts with `examples/`, `tests/`, or `golden/`, so a new job that names another directory of the root needs that list (`ROOT_DIRS` in tools/write_golden.py, `kRootDirs` in src/golden.cpp) to grow.
- `tools/write_golden.py --modes ""` writes only the `links` and `cli` manifests, in a few seconds.
- A fix made in both compilers changes what Python writes, so the golden files are written again from Python for the modes it touches, and check-golden then shows whether C++ agrees. The linker's address-space check of 24 Nov 2026 took `--modes c,cO` (which writes `links` and `cli` too) in 105 s with `-j 12`. Read each changed job in the diff before committing. That one turned up two corpus programs with more than 64K of `.bss`.
- Capture binutils records without `C6502_CPP_BACKEND`. Since step 7 the switch sends the assembler and the linker to C++, so a run with it records only `elf_read`, `parse_metadata`, nm, objdump, and the disassembler.
- Reading a 1.5 GB capture one file at a time in one process took over 40 minutes. A process per function, with the blobs' sizes listed once, takes under a minute for the binutils records and about 3 minutes for the TAC calls.

Related: [[codebase/compile-command]], [[codebase/tac-interpreter]], [[codebase/recording-test-calls]]
