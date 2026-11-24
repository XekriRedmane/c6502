# c6502cpp compile and compile.py's shim

- Python 3.11 on Windows reads and writes a pipe in cp1252 with CR LF. `c6502cpp compile` run by itself writes text mode (CR LF) UTF-8 and reads stdin as UTF-8, so it gives compile.py's bytes for ASCII text only.
- The shim starts in about 92 ms for `-S` of a one-line program (24 Nov 2026), and docs/cpp_port.md's Status line for G2 breaks that time down. Each import before the early exit adds to it, about 30 ms for dataclasses and 12 ms for shutil. `TestCommandLine.test_compile_imports_only_the_shim` in tests/test_cpp_backend.py fails when the shim imports a module of the repository other than cpp_backend, or imports argparse or shutil.
- The Python compiler and the `C6502_CPP_BACKEND` switch are a temporary adapter, which [[principles/migrate-callers-then-delete-legacy-apis]] allows when it is time-boxed. Group R of step 8 in docs/cpp_port.md removes both.

Related: [[codebase/checks-after-python]]
