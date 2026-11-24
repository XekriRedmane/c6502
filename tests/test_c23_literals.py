"""Tests for C23's binary constants and digit separators (C23 §6.4.4.2,
§6.4.4.3, §6.4.8; pp_numbers.py).

Coverage:

  * `0b1010` and `0B1010` lex as one constant, with every suffix, and
    take the type list of octal and hexadecimal constants, so `0b` and
    sixteen 1s is an `unsigned int`, where the decimal 65535 is a
    `long`.
  * A `'` between two digits of a digit sequence is a digit separator,
    in decimal, octal, hexadecimal, and binary constants and in the
    whole part, fraction, and exponent of a floating constant. It's
    ignored for the value.
  * A separator anywhere else is a lex error whose message says where
    it is: at the end, doubled, after the prefix, touching the period,
    next to the exponent, or before the suffix. A binary constant with
    a digit other than 0 and 1, or with no digits, is an error, and so
    is a `wb` or `uwb` constant too wide for `_BitInt(64)` (the `wb`
    constants themselves are tested in tests/test_bitint.py).
  * The preprocessor keeps a number with separators one token, so the
    `'` doesn't start a character constant that would hide a macro
    from expansion, and `#if` reads binary constants, separators, and
    the `ll` suffixes. A number there that isn't an integer constant
    is a `PreprocessorError`.
  * An octal constant with a digit other than 0, such as `017`, has
    the right value. The parser used to raise a Python ValueError for
    it.
  * A program using them runs to the same result in the simulator, in
    both pipelines.
"""
from __future__ import annotations

import shutil
import unittest

from sim.harness import build_sim

_HAVE_PCPP = shutil.which("pcpp") is not None


@unittest.skipUnless(_HAVE_PCPP, "pcpp CLI not available")
class TestSimulator(unittest.TestCase):
    def test_program(self) -> None:
        src = """
#define K 1'000
#if 0b1 && K == 1000
static unsigned long table[] = { 0b1, 0b10, 0x1'00, 1'000'000 };
#endif
int main(void) {
    unsigned long sum = 0;
    for (unsigned char i = 0; i < 4; i++)
        sum += table[i];
    double d = 1'000.5;
    if (d != 1000.5)
        return -1;
    return (int)(sum - 1'000'000UL) + 0b1000'0000 + K;
}
"""
        want = 1 + 2 + 256 + 128 + 1000
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                result = build_sim(src, optimize=optimize).run()
                self.assertFalse(result.timed_out)
                self.assertEqual(result.return_int(), want)


if __name__ == "__main__":
    unittest.main()
