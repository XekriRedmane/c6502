"""Tests of the C++ port's object-file tools (step 7, docs/cpp_port.md).
tools/capture_binutils.py records the Python tools over the corpus and
`c6502cpp check-binutils` compares the C++ ones with the records, and
these check what the records leave out, such as the opcode table."""
from __future__ import annotations

import subprocess
import unittest
from pathlib import Path

from tests.test_ir_dump import cpp_cli

_ROOT = Path(__file__).resolve().parent.parent


@unittest.skipUnless(cpp_cli() is not None, "c6502cpp isn't built")
class TestOpcodes(unittest.TestCase):
    def test_table(self) -> None:
        from binutils.opcodes import OPCODES
        done = subprocess.run([str(cpp_cli()), "opcodes"], check=True,
                              capture_output=True, text=True)
        expected = [f"{m} {mode} 0x{op:02X}"
                    for (m, mode), op in OPCODES.items()]
        self.assertEqual(done.stdout.splitlines(), expected)


if __name__ == "__main__":
    unittest.main()
