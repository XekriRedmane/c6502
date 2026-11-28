# c6502cpp compile

- `c6502cpp compile` takes compile.py's command line, and since group R of step 9 (27 Nov 2026) its usage and messages name the program `c6502` (`usage: c6502 ...`, `c6502: link error: ...`). `c6502cpp nm` and `c6502cpp objdump` name `c6502-nm` and `c6502-objdump`. The executable is still `c6502cpp`.
- The command writes its stdout and stderr in text mode, so on Windows `-o -` gives CR LF, while `-o out.s` or `-o out.asm` writes LF bytes. Write an example's text to a file with `-o` before comparing it with golden/O/.
- Python 3.11 on Windows read and wrote a pipe in cp1252 with CR LF, and the command reads stdin and writes as UTF-8, so its bytes were compile.py's for ASCII text only.

Related: [[codebase/checks-after-python]], [[codebase/git-bash-on-windows]]
