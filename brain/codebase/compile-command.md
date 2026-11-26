# c6502cpp compile and compile.py's shim

- Python 3.11 on Windows reads and writes a pipe in cp1252 with CR LF. `c6502cpp compile` run by itself writes text mode (CR LF) UTF-8 and reads stdin as UTF-8, so it gives compile.py's bytes for ASCII text only.
- The shim starts in about 92 ms for `-S` of a one-line program (24 Nov 2026), and docs/cpp_port.md's Status line for G2 breaks that time down. Each module the shim imports adds to it, about 30 ms for dataclasses and 12 ms for shutil, so compile.py run as a script imports no module of the repository but cpp_backend.py, which imports errors.py only for an error it raises.
- compile.py, nm.py, and objdump.py stay as shims until step 9, since the suite, the simulator harness, and the fuzzers call their functions in-process. Group R of step 8 removed the Python compiler and the `C6502_CPP_BACKEND` switch, the temporary adapter that [[principles/migrate-callers-then-delete-legacy-apis]] allows when it is time-boxed.

Related: [[codebase/checks-after-python]]
