"""Helpers for tests that compile units to objects and link them.

`compile_object` runs `compile.py -c -O` on a source string, and `link`
runs `compile.py` on objects to write an executable, with the runtime
helpers' addresses given (`sim.linked.defsym_args`) so that the
simulator can run the result. `linked_text` renders an executable as
text that tests can search: its `objdump.py -D` listing, which has an
`EQU` line for every absolute symbol (the slots, the zero-page objects,
the runtime's block), followed by its link metadata.
"""
from __future__ import annotations

import io
from pathlib import Path
from unittest.mock import patch

from compile import main as compile_main


def compile_object(source: str, out_path: Path, *, optimize: bool = True,
                   extra: list[str] = ()) -> int:
    """Compiles C `source` from stdin to the object `out_path`. Returns
    the exit code."""
    argv = ["compile.py", "-", "-c", *(["-O"] if optimize else []),
            "-o", str(out_path), *extra]
    with patch("sys.stdin", io.StringIO(source)):
        return compile_main(argv)


def link(paths, out_path: Path, *, script: Path | str | None = None,
         extra: list[str] = ()) -> int:
    """Links `paths` into the executable `out_path`. Returns the exit
    code."""
    from sim.linked import defsym_args
    argv = ["compile.py", *map(str, paths), "-o", str(out_path),
            *defsym_args(), *extra]
    if script is not None:
        argv[1:1] = ["-T", str(script)]
    return compile_main(argv)


def link_capture_stderr(paths, out_path: Path, **kw) -> tuple[int, str]:
    """`link`, with what it prints to stderr."""
    with patch("sys.stderr", io.StringIO()) as err:
        rc = link(paths, out_path, **kw)
        return rc, err.getvalue()


class LinkedText(str):
    """`linked_text`'s text, with the executable's bytes in `exe`, for
    `sim.linked.build_linked_sim`."""
    exe: bytes = b""


def linked_text(exe_path: Path | str) -> LinkedText:
    """The executable at `exe_path` as searchable text: its full
    disassembly (with `EQU`s for its absolute symbols) and then its
    link metadata. Both come from `c6502cpp objdump`, the C++ port of
    objdump.py, which needs the C++ build (cpp/README.md)."""
    disassembly = _cpp_objdump("-D", "--no-show-raw-insn", "--no-addresses",
                               str(exe_path))
    meta = _cpp_objdump("--link-metadata", str(exe_path))
    if meta == "(no link metadata)\n":
        meta = ""
    text = LinkedText(disassembly + "\n" + meta)
    text.exe = Path(exe_path).read_bytes()
    return text


def _cpp_objdump(*args: str) -> str:
    """What `c6502cpp objdump` prints for the command line `args`."""
    import contextlib
    import cpp_backend
    out = io.StringIO()
    with contextlib.redirect_stdout(out):
        assert cpp_backend.run_tool("objdump", ["objdump.py", *args]) == 0
    return out.getvalue()


def listing(exe_path: Path | str) -> str:
    """What `objdump.py -D --no-show-raw-insn --no-addresses` prints
    for the executable at `exe_path`, which the multi-unit examples
    check in as the linked program."""
    import contextlib
    import objdump
    out = io.StringIO()
    with contextlib.redirect_stdout(out):
        assert objdump.main([
            "objdump.py", "-D", "--no-show-raw-insn", "--no-addresses",
            str(exe_path),
        ]) == 0
    return out.getvalue()


def symbols(exe_path: Path | str) -> dict[str, int]:
    """The executable's symbols by name, a global one where a name is
    both."""
    from binutils import elf as E
    out: dict[str, int] = {}
    for s in E.read_file(exe_path).symbols:
        if s.defined and s.type != E.STT_FILE:
            if s.is_global or s.name not in out:
                out[s.name] = s.value
    return out
