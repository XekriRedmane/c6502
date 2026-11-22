"""Tests of the C++ port's object-file tools (step 7, docs/cpp_port.md):
the opcode table, and nm.py with `C6502_CPP_BACKEND=1`, which runs
`c6502cpp nm`. tools/capture_binutils.py records the Python tools over
the corpus and `c6502cpp check-binutils` compares the C++ ones with the
records, and these check what the records leave out: the terminal
widths argparse wraps the usage and help to, and the bytes nm.py writes
as a process."""
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


def _run(argv: list[str], *, cpp: bool, columns: int, cwd: str,
         script: bool) -> subprocess.CompletedProcess:
    """Runs nm.py as a process (`script`), or `c6502cpp nm` itself, with
    the C++ port on or off and the terminal `columns` wide."""
    env = dict(os.environ)
    env.pop("C6502_CPP_BACKEND", None)
    env["COLUMNS"] = str(columns)
    if cpp:
        env["C6502_CPP_BACKEND"] = "1"
    if script:
        cmd = [sys.executable, str(_ROOT / "nm.py"), *argv]
    else:
        cmd = [str(cpp_cli()), "nm", *argv]
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


if __name__ == "__main__":
    unittest.main()
