"""Tests of the C++ port's object-file tools (step 7, docs/cpp_port.md):
the opcode table, and nm.py and objdump.py with `C6502_CPP_BACKEND=1`,
which runs `c6502cpp nm` and `c6502cpp objdump`.
tools/capture_binutils.py records the Python tools over the corpus and
`c6502cpp check-binutils` compares the C++ ones with the records, and
these check what the records leave out: the terminal widths argparse
wraps the usage and help to, and the bytes each tool writes as a
process."""
from __future__ import annotations

import os
import subprocess
import sys
import tempfile
import unittest
from pathlib import Path

from tests.test_ir_dump import cpp_cli

_ROOT = Path(__file__).resolve().parent.parent

# Command lines of nm.py, with `{f}` for an object and `{d}` for a
# damaged copy of it.
_COMMAND_LINES = (
    ["-h"], [], ["-z", "{f}"], ["--de", "{f}"], ["-ax", "{f}"],
    ["{f}", "-a", "{f}"], ["-gun", "{f}", "{f}"], ["missing.o"],
    ["-A", "{d}", "{f}"], ["-a", "-n", "-S", "{f}"], ["--", "-h"],
    ["--print-file-name", "-r", "{f}", "{d}"], ["-p=1", "{f}"],
    ["./{d}", ".//missing.o"],
)


# Command lines of objdump.py, with `{e}` for an executable too.
_OBJDUMP_COMMAND_LINES = (
    ["--help"], [], ["{f}"], ["-q", "{f}"], ["-j"], ["-h", "--help"],
    ["-x", "{f}"], ["-d", "{f}"], ["-D", "{e}"], ["-s", "{e}"],
    ["-dj", ".text", "{f}"], ["-x", "missing.o", "{d}", "{e}"],
    ["-h", "-t", "-r", "{e}"], ["--link", "{f}"], ["--no", "-d", "{f}"],
    ["-D", "--no-show-raw-insn", "--no-addresses", "{e}"],
    ["-r", "-j", ".text", "--section=.data", "-s", "{f}", "{e}"],
    ["-f", "--", "-d"], ["-f", "./{d}", ".//missing.o"],
)

# A program whose executable has code, a table in `.rodata`, and an
# object in `.bss`.
_PROGRAM = """\
static const unsigned char t[5] = {1, 2, 3, 4, 5};
unsigned char buf[4];
int main(void) {
    unsigned char s = 0;
    for (unsigned char i = 0; i < 5; i++) s += t[i];
    buf[1] = s;
    return s;
}
"""


def _run(argv: list[str], *, cpp: bool, columns: int, cwd: str,
         script: bool, tool: str = "nm") -> subprocess.CompletedProcess:
    """Runs nm.py or objdump.py (`tool`) as a process (`script`), or
    `c6502cpp nm` or `c6502cpp objdump` itself, with the C++ port on or
    off and the terminal `columns` wide."""
    env = dict(os.environ)
    env.pop("C6502_CPP_BACKEND", None)
    env["COLUMNS"] = str(columns)
    if cpp:
        env["C6502_CPP_BACKEND"] = "1"
    if script:
        cmd = [sys.executable, str(_ROOT / f"{tool}.py"), *argv]
    else:
        cmd = [str(cpp_cli()), tool, *argv]
    return subprocess.run(cmd, cwd=cwd, env=env, capture_output=True)


@unittest.skipUnless(cpp_cli() is not None, "c6502cpp isn't built")
class TestOpcodes(unittest.TestCase):
    def test_table(self) -> None:
        from binutils.opcodes import OPCODES
        done = subprocess.run([str(cpp_cli()), "opcodes"], check=True,
                              capture_output=True, text=True)
        expected = [f"{m} {mode} 0x{op:02X}"
                    for (m, mode), op in OPCODES.items()]
        self.assertEqual(done.stdout.splitlines(), expected)


@unittest.skipUnless(cpp_cli() is not None, "c6502cpp isn't built")
class TestNm(unittest.TestCase):
    def setUp(self) -> None:
        from binutils import elf as E
        from binutils.assembler import assemble
        self.tmp = tempfile.TemporaryDirectory()
        text = (_ROOT / "examples" / "apply_bobble.asm").read_text()
        data = E.write(assemble(text, "apply_bobble.asm"))
        Path(self.tmp.name, "f.o").write_bytes(data)
        Path(self.tmp.name, "d.o").write_bytes(data[:len(data) // 2])

    def tearDown(self) -> None:
        self.tmp.cleanup()

    def _argv(self, cmd: list[str]) -> list[str]:
        return [a.replace("{f}", "f.o").replace("{d}", "d.o") for a in cmd]

    def test_nm_py_with_the_cpp_port(self) -> None:
        # nm.py writes the same bytes, messages, and exit status, at
        # widths that wrap the usage and help differently.
        for columns in (20, 41, 80, 132):
            for cmd in _COMMAND_LINES:
                argv = self._argv(cmd)
                with self.subTest(columns=columns, argv=argv):
                    py = _run(argv, cpp=False, columns=columns,
                              cwd=self.tmp.name, script=True)
                    cpp = _run(argv, cpp=True, columns=columns,
                               cwd=self.tmp.name, script=True)
                    self.assertEqual(cpp.stdout, py.stdout)
                    self.assertEqual(cpp.stderr, py.stderr)
                    self.assertEqual(cpp.returncode, py.returncode)

    def test_cpp_command(self) -> None:
        # `c6502cpp nm` itself gives nm.py's output, with LF line ends
        # on stderr, which cpp_backend.run_tool writes through Python's
        # text stream.
        for cmd in _COMMAND_LINES:
            argv = self._argv(cmd)
            with self.subTest(argv=argv):
                py = _run(argv, cpp=False, columns=80, cwd=self.tmp.name,
                          script=True)
                cpp = _run(argv, cpp=False, columns=80, cwd=self.tmp.name,
                           script=False)
                self.assertEqual(cpp.stdout, py.stdout)
                self.assertEqual(cpp.stderr, py.stderr.replace(b"\r\n", b"\n"))
                self.assertEqual(cpp.returncode, py.returncode)

    def test_nm_main_in_process(self) -> None:
        # A test that calls `nm.main` with stdout redirected gets the
        # C++ port's listing in its stream.
        import contextlib
        import io
        import nm
        here = os.getcwd()
        os.chdir(self.tmp.name)
        old = os.environ.get("C6502_CPP_BACKEND")
        try:
            outs = []
            for value in (None, "1"):
                if value is None:
                    os.environ.pop("C6502_CPP_BACKEND", None)
                else:
                    os.environ["C6502_CPP_BACKEND"] = value
                out, err = io.StringIO(), io.StringIO()
                with contextlib.redirect_stdout(out), \
                        contextlib.redirect_stderr(err):
                    rc = nm.main(["nm.py", "-a", "f.o", "d.o"])
                outs.append((rc, out.getvalue(), err.getvalue()))
            self.assertEqual(outs[1], outs[0])
            self.assertEqual(outs[0][0], 1)
        finally:
            os.chdir(here)
            if old is None:
                os.environ.pop("C6502_CPP_BACKEND", None)
            else:
                os.environ["C6502_CPP_BACKEND"] = old


@unittest.skipUnless(cpp_cli() is not None, "c6502cpp isn't built")
class TestObjdump(unittest.TestCase):
    @classmethod
    def setUpClass(cls) -> None:
        from binutils import elf as E
        from binutils.assembler import assemble
        cls.tmp = tempfile.TemporaryDirectory()
        tmp = Path(cls.tmp.name)
        text = (_ROOT / "examples" / "apply_bobble.asm").read_text()
        data = E.write(assemble(text, "apply_bobble.asm"))
        (tmp / "f.o").write_bytes(data)
        (tmp / "d.o").write_bytes(data[:len(data) // 2])
        (tmp / "x.c").write_text(_PROGRAM)
        env = dict(os.environ)
        env.pop("C6502_CPP_BACKEND", None)
        subprocess.run(
            [sys.executable, str(_ROOT / "compile.py"), "-O", "x.c", "-o",
             "x.exe"], cwd=tmp, env=env, check=True, capture_output=True,
        )

    @classmethod
    def tearDownClass(cls) -> None:
        cls.tmp.cleanup()

    def _argv(self, cmd: list[str]) -> list[str]:
        return [a.replace("{f}", "f.o").replace("{d}", "d.o")
                .replace("{e}", "x.exe") for a in cmd]

    def test_objdump_py_with_the_cpp_port(self) -> None:
        # objdump.py writes the same bytes, messages, and exit status, at
        # widths that wrap the usage and help differently.
        for columns in (20, 41, 80, 132):
            for cmd in _OBJDUMP_COMMAND_LINES:
                argv = self._argv(cmd)
                with self.subTest(columns=columns, argv=argv):
                    py = _run(argv, cpp=False, columns=columns,
                              cwd=self.tmp.name, script=True, tool="objdump")
                    cpp = _run(argv, cpp=True, columns=columns,
                               cwd=self.tmp.name, script=True, tool="objdump")
                    self.assertEqual(cpp.stdout, py.stdout)
                    self.assertEqual(cpp.stderr, py.stderr)
                    self.assertEqual(cpp.returncode, py.returncode)

    def test_cpp_command(self) -> None:
        # `c6502cpp objdump` itself gives objdump.py's output, with LF
        # line ends on stderr.
        for cmd in _OBJDUMP_COMMAND_LINES:
            argv = self._argv(cmd)
            with self.subTest(argv=argv):
                py = _run(argv, cpp=False, columns=80, cwd=self.tmp.name,
                          script=True, tool="objdump")
                cpp = _run(argv, cpp=False, columns=80, cwd=self.tmp.name,
                           script=False, tool="objdump")
                self.assertEqual(cpp.stdout, py.stdout)
                self.assertEqual(cpp.stderr, py.stderr.replace(b"\r\n", b"\n"))
                self.assertEqual(cpp.returncode, py.returncode)

    def test_objdump_main_in_process(self) -> None:
        # A caller of `objdump.main` gets the C++ port's listing in its
        # redirected stream, and the return of 2 for a command line that
        # asks for nothing, where argparse's errors raise SystemExit.
        import contextlib
        import io
        import objdump
        here = os.getcwd()
        os.chdir(self.tmp.name)
        old = os.environ.get("C6502_CPP_BACKEND")
        try:
            for argv in (["objdump.py", "-x", "-D", "x.exe", "d.o"],
                         ["objdump.py", "f.o"], ["objdump.py", "-z", "f.o"]):
                outs = []
                for value in (None, "1"):
                    if value is None:
                        os.environ.pop("C6502_CPP_BACKEND", None)
                    else:
                        os.environ["C6502_CPP_BACKEND"] = value
                    out, err = io.StringIO(), io.StringIO()
                    with contextlib.redirect_stdout(out), \
                            contextlib.redirect_stderr(err):
                        try:
                            rc = objdump.main(argv)
                        except SystemExit as e:
                            rc = ("exit", e.code)
                    outs.append((rc, out.getvalue(), err.getvalue()))
                with self.subTest(argv=argv):
                    self.assertEqual(outs[1], outs[0])
            self.assertEqual(outs[0][0], ("exit", 2))
        finally:
            os.chdir(here)
            if old is None:
                os.environ.pop("C6502_CPP_BACKEND", None)
            else:
                os.environ["C6502_CPP_BACKEND"] = old
def _assemble_request(text: str, filename: str, link_symbols, keep_source: bool) -> bytes:
    """The request `c6502cpp assemble` reads (src/binutils/assemble_command.h)."""
    import json
    args = json.dumps({
        "filename": filename, "keep_source": keep_source,
        "link_symbols": None if link_symbols is None
        else [[k, v] for k, v in link_symbols.items()],
    })
    body = text.encode("utf-8")
    return (f"c6502-assemble 1\nargs {args}\ntext {len(body)}\n".encode("utf-8")
            + body)


@unittest.skipUnless(cpp_cli() is not None, "c6502cpp isn't built")
class TestAssemble(unittest.TestCase):
    """`c6502cpp assemble`, which group F connects to compile.py, gives
    the bytes of Python's object, or Python's error."""

    def _cpp(self, text: str, filename: str, link_symbols=None,
             keep_source: bool = True) -> bytes:
        done = subprocess.run(
            [str(cpp_cli()), "assemble"], check=True, capture_output=True,
            input=_assemble_request(text, filename, link_symbols, keep_source))
        return done.stdout

    def test_objects(self) -> None:
        from binutils import elf as E
        from binutils.assembler import assemble
        text = (_ROOT / "examples" / "apply_bobble.asm").read_text(encoding="utf-8")
        for links, keep in ((None, True), ({"DPTR": None, "HARGS": 0x40}, False)):
            with self.subTest(links=links, keep=keep):
                data = E.write(assemble(text, "apply_bobble.asm", links, keep))
                self.assertEqual(self._cpp(text, "apply_bobble.asm", links, keep),
                                 f"object {len(data)}\n".encode() + data)

    def test_error(self) -> None:
        import json
        from binutils.assembler import AssemblerError, assemble
        text = "f:\n BNE 5\n"
        with self.assertRaises(AssemblerError) as raised:
            assemble(text, "t.s")
        expected = {"type": "AssemblerError", "message": str(raised.exception)}
        out = self._cpp(text, "t.s").decode("utf-8")
        self.assertTrue(out.startswith("error "))
        self.assertEqual(json.loads(out[len("error "):]), expected)


if __name__ == "__main__":
    unittest.main()
