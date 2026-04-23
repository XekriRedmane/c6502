import io
import shutil
import tempfile
import unittest
from pathlib import Path
from unittest.mock import patch

from compile import main


@unittest.skipUnless(shutil.which("pcpp"), "pcpp not available on PATH")
class TestCompileDriver(unittest.TestCase):
    SOURCE = "int main(void) { return 42; }"

    def _run(self, argv: list[str], stdin: str = "") -> tuple[int, str, str]:
        with patch("sys.stdin", io.StringIO(stdin)), \
             patch("sys.stdout", new_callable=io.StringIO) as out, \
             patch("sys.stderr", new_callable=io.StringIO) as err:
            rc = main(argv)
        return rc, out.getvalue(), err.getvalue()

    def test_lex_stdin_prints_tokens(self):
        rc, out, _ = self._run(["compile.py", "-", "--lex"], stdin=self.SOURCE)
        self.assertEqual(rc, 0)
        lines = out.splitlines()
        self.assertEqual(lines[0].split("\t"), ["1:1", "keyword", "int"])
        # Last token is `}`.
        self.assertEqual(lines[-1].split("\t")[1:], ["symbol", "}"])

    def test_parse_stdin_prints_ast(self):
        rc, out, _ = self._run(["compile.py", "-", "--parse"], stdin=self.SOURCE)
        self.assertEqual(rc, 0)
        self.assertIn("Program(", out)
        self.assertIn("Function(", out)
        self.assertIn("Constant(", out)
        self.assertIn("value=42", out)

    def test_tac_stdin_prints_tac_ast(self):
        rc, out, _ = self._run(
            ["compile.py", "-", "--tac"],
            stdin="int main(void) { return -42; }",
        )
        self.assertEqual(rc, 0)
        self.assertIn("Unary(", out)
        self.assertIn("Negate(", out)
        self.assertIn("'%0'", out)
        self.assertIn("Ret(", out)

    def test_codegen_stdin_prints_asm(self):
        rc, out, _ = self._run(["compile.py", "-", "--codegen"], stdin=self.SOURCE)
        self.assertEqual(rc, 0)
        self.assertIn("main:", out)
        self.assertIn("SUBROUTINE", out)
        self.assertIn("LDA   #$2A", out)
        self.assertIn("RTS", out)

    def test_pcpp_strips_comments(self):
        rc, out, _ = self._run(
            ["compile.py", "-", "--codegen"],
            stdin="/* hello */ int main(void) { return 1; } // bye",
        )
        self.assertEqual(rc, 0)
        self.assertIn("LDA   #$01", out)

    def test_output_file_for_codegen(self):
        with tempfile.TemporaryDirectory() as tmp:
            out_path = Path(tmp) / "hello.asm"
            rc, _, _ = self._run(
                ["compile.py", "-", "--codegen", "-o", str(out_path)],
                stdin="int main(void) { return 7; }",
            )
            self.assertEqual(rc, 0)
            self.assertIn("LDA   #$07", out_path.read_text())

    def test_codegen_output_must_end_in_asm(self):
        rc, _, err = self._run(
            ["compile.py", "-", "--codegen", "-o", "out.txt"],
            stdin=self.SOURCE,
        )
        self.assertEqual(rc, 2)
        self.assertIn(".asm suffix", err)

    def test_non_codegen_outputs_can_have_any_suffix(self):
        with tempfile.TemporaryDirectory() as tmp:
            out_path = Path(tmp) / "tokens.txt"
            rc, _, _ = self._run(
                ["compile.py", "-", "--lex", "-o", str(out_path)],
                stdin=self.SOURCE,
            )
            self.assertEqual(rc, 0)
            self.assertIn("keyword\tint", out_path.read_text())

    def test_stages_are_mutually_exclusive(self):
        with self.assertRaises(SystemExit):
            self._run(["compile.py", "-", "--lex", "--parse"], stdin=self.SOURCE)

    def test_stage_is_required(self):
        with self.assertRaises(SystemExit):
            self._run(["compile.py", "-"], stdin=self.SOURCE)

    def test_input_from_file(self):
        with tempfile.TemporaryDirectory() as tmp:
            in_path = Path(tmp) / "hello.c"
            in_path.write_text(self.SOURCE)
            rc, out, _ = self._run(
                ["compile.py", str(in_path), "--codegen"],
            )
            self.assertEqual(rc, 0)
            self.assertIn("LDA   #$2A", out)


if __name__ == "__main__":
    unittest.main()
