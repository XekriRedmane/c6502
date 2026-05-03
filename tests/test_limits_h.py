"""Tests for the bundled `<limits.h>`.

The header lives at `include/limits.h` next to `compile.py`;
`preprocessor.py` adds that directory to pcpp's search path after
any user `-I` flags so `#include <limits.h>` resolves out-of-the-
box. Tests:

  * Each macro expands to the documented value (asm-sim run of a
    small program returning each limit in the appropriate width).
  * SHRT_* aliases match INT_* exactly.
  * The header is idempotent (multiple `#include`s don't error).
  * A user `-I` path can shadow the bundled header.
"""
from __future__ import annotations

import os
import shutil
import tempfile
import unittest

from sim.harness import run_c_program


_HEADER = "#include <limits.h>\n"


@unittest.skipUnless(shutil.which("pcpp") or True, "pcpp lives in-process")
class TestLimitsH(unittest.TestCase):
    def _run_int_signed(self, body: str) -> int:
        src = _HEADER + f"int main(void) {{ {body} }}"
        return run_c_program(src).return_int_signed()

    def _run_int_unsigned(self, body: str) -> int:
        src = _HEADER + f"unsigned int main(void) {{ {body} }}"
        return run_c_program(src).return_int()

    def _run_long(self, body: str) -> int:
        src = _HEADER + f"long main(void) {{ {body} }}"
        return run_c_program(src).return_long_signed()

    def _run_ulong(self, body: str) -> int:
        src = _HEADER + f"unsigned long main(void) {{ {body} }}"
        return run_c_program(src).return_long()

    def _run_char(self, body: str) -> int:
        src = _HEADER + f"int main(void) {{ {body} }}"
        return run_c_program(src).return_int_signed()

    def test_char_bit(self) -> None:
        self.assertEqual(self._run_char("return CHAR_BIT;"), 8)

    def test_schar_min(self) -> None:
        self.assertEqual(self._run_char("return SCHAR_MIN;"), -128)

    def test_schar_max(self) -> None:
        self.assertEqual(self._run_char("return SCHAR_MAX;"), 127)

    def test_uchar_max(self) -> None:
        self.assertEqual(self._run_char("return UCHAR_MAX;"), 255)

    def test_char_min_is_unsigned(self) -> None:
        # plain char is unsigned in c6502
        self.assertEqual(self._run_char("return CHAR_MIN;"), 0)

    def test_char_max_is_uchar_max(self) -> None:
        self.assertEqual(self._run_char("return CHAR_MAX;"), 255)

    def test_mb_len_max(self) -> None:
        self.assertEqual(self._run_char("return MB_LEN_MAX;"), 1)

    def test_int_max(self) -> None:
        self.assertEqual(self._run_int_signed("return INT_MAX;"), 32767)

    def test_int_min(self) -> None:
        self.assertEqual(self._run_int_signed("return INT_MIN;"), -32768)

    def test_uint_max(self) -> None:
        self.assertEqual(self._run_int_unsigned("return UINT_MAX;"), 65535)

    def test_shrt_aliases_int(self) -> None:
        # SHRT_MIN / SHRT_MAX / USHRT_MAX alias INT_*.
        self.assertEqual(self._run_int_signed("return SHRT_MAX;"), 32767)
        self.assertEqual(self._run_int_signed("return SHRT_MIN;"), -32768)
        self.assertEqual(self._run_int_unsigned("return USHRT_MAX;"), 65535)

    def test_long_max(self) -> None:
        self.assertEqual(self._run_long("return LONG_MAX;"), 0x7FFFFFFF)

    def test_long_min(self) -> None:
        self.assertEqual(self._run_long("return LONG_MIN;"), -0x80000000)

    def test_ulong_max(self) -> None:
        self.assertEqual(self._run_ulong("return ULONG_MAX;"), 0xFFFFFFFF)

    def test_idempotent_double_include(self) -> None:
        # Including twice must not cause a redefinition error — pcpp's
        # auto-pragma-once heuristic + the header's own ifndef guard
        # both protect this.
        src = (
            "#include <limits.h>\n"
            "#include <limits.h>\n"
            "int main(void) { return INT_MAX; }\n"
        )
        self.assertEqual(run_c_program(src).return_int_signed(), 32767)

    def test_user_include_path_shadows_bundled(self) -> None:
        # If the user supplies `-I <dir>` and `<dir>/limits.h` exists,
        # it shadows the bundled header. Verifies precedence: user
        # `-I` paths are added BEFORE the bundled directory in pcpp's
        # search list.
        from preprocessor import preprocess
        with tempfile.TemporaryDirectory() as tmp:
            with open(os.path.join(tmp, "limits.h"), "w") as f:
                f.write("#define INT_MAX 99\n")
            src = "#include <limits.h>\nint x = INT_MAX;\n"
            out = preprocess(src, ["-I", tmp])
        self.assertIn("int x = 99", out)


if __name__ == "__main__":
    unittest.main()
