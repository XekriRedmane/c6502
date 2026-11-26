"""`fuzz.tac_passes`, which runs the TAC interpreter after each pass of
the TAC optimizer to name the pass that changes a program's result.

The tests of the interpreter on optimized TAC and on the SSA form
between the optimizer's passes, which called tac_sim.py and the TAC
optimizer directly, are doctest cases of the C++ port now
(cpp/tests/test_tac_sim_optimized.cpp, docs/cpp_port.md, step 8, group
G9).
"""
from __future__ import annotations

import os
import shutil
import unittest

from fuzz.tac_passes import first_changing_pass


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestTacPasses(unittest.TestCase):

    _SRC = """
        unsigned char g = 10;
        unsigned char h(void) { g = 77; return 1; }
        int main(void) {
            int t = g + 1;
            unsigned char k = h();
            unsigned char x = (unsigned char)t;
            return x + k;
        }
    """

    def test_no_pass_changes_a_correct_program(self) -> None:
        self.assertIsNone(first_changing_pass(self._SRC))

    def test_variable_without_a_value_before_a_loop(self) -> None:
        # SSA gives `c` a Phi at the loop's header whose argument from
        # the entry is `c` itself, which has no value there, and SSA
        # destruction copies it. Neither may raise, since nothing reads
        # the value (tests/chapter_8/valid/continue.c).
        src = """
            int main(void) {
                int c;
                for (int i = 0; i < 4; i++) c = i;
                return c;
            }
        """
        self.assertIsNone(first_changing_pass(src))

    def test_object_another_unit_defines(self) -> None:
        # DirectMemberAccess stores to `ext.a[1]` by name, and `ext`
        # has no StaticVariable here, since another unit defines it
        # (tests/chapter_18/valid/parameters/pass_args_on_page_boundary.c).
        src = """
            struct s { char a[3]; };
            extern struct s ext;
            int main(void) { ext.a[1] = 5; return ext.a[1]; }
        """
        self.assertIsNone(first_changing_pass(src))

    @unittest.skipIf(
        os.environ.get("C6502_CPP_BACKEND") == "1",
        "breaks a Python pass, which the C++ port doesn't run "
        "(cpp/README.md, \"The TAC interpreter\")",
    )
    def test_names_the_pass(self) -> None:
        # With narrow_widened_arith's check that a global is unchanged
        # since the extend read it turned off, it reads g after h()
        # changed it.
        import passes.optimization.narrow_widened_arith as n
        saved = n.var_unchanged
        n.var_unchanged = lambda *a, **k: True
        try:
            res = first_changing_pass(self._SRC)
        finally:
            n.var_unchanged = saved
        self.assertEqual(res, ("main", "NarrowWidenedArith", 12, 79))


if __name__ == "__main__":
    unittest.main()
