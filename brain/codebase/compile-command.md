# c6502cpp compile and compile.py's shim

- Python 3.11 on Windows reads and writes a pipe in cp1252 with CR LF. `c6502cpp compile` run by itself writes text mode (CR LF) UTF-8 and reads stdin as UTF-8, so it gives compile.py's bytes for ASCII text only.
- Each module compile.py imports at start adds to every compile's time, about 30 ms for dataclasses and 12 ms for shutil. The shim took 92 ms for `-S` of a one-line program on 24 Nov 2026 (docs/cpp_port.md, G2's Status line). Import lazily.

Related: [[codebase/checks-after-python]], [[codebase/git-bash-on-windows]]
