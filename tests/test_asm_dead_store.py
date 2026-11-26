"""Tests for `passes.asm_dead_store.apply_asm_dead_store`.

The pass drops or morphs Mov-into-memory atoms whose written byte
isn't observed by any subsequent instruction reachable in the CFG.
The CFG walk treats `Call` / `FunctionPrologue` / `AllocateStack`
as opaque (they may read any memory), but `LoadAddress` is modeled
precisely: it writes 2 bytes to `dst`, reads `FP` / `FP+1` if `src`
is `Frame`, and reads nothing if `src` is `Data` (link-time
immediates). Without precise modeling, any `STA DPTR` whose forward
walk happens to pass through a downstream `LoadAddress` (very
common when the next pointer-write reloads the indirect base via
`&static`) is conservatively kept LIVE, leaving the previous DPTR
stage as dead code.

The tests that called the Python functions directly are doctest cases of
the C++ port now, each named for the test it came from
(docs/cpp_port.md, step 8, group G6).
"""

from __future__ import annotations

import unittest
from tests import cpp_build


class TestPointerInNumberedZeroPage(unittest.TestCase):
    """A function without a private pool keeps a pointer in numbered
    zero-page bytes, `ZP(addr)`. A load through it reads both bytes,
    though the bytes the load reaches can't be in the pool. The
    stores of a pointer an indirect call returned were dropped, and
    `pick()[0]` read through whatever the bytes held."""


    def test_runs(self):
        if not cpp_build.BUILT:
            self.skipTest(cpp_build.REASON)
        from sim.harness import build_sim
        src = """
            unsigned char table[8] = {10, 20, 30, 40, 50, 60, 70, 80};
            static unsigned char *last(void) { return &table[7]; }
            static unsigned char *(*pick)(void) = last;
            int main(void) { unsigned char *c = pick(); return *c; }
        """
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                self.assertEqual(
                    build_sim(src, optimize=optimize).run().return_int(), 80,
                )


if __name__ == "__main__":
    unittest.main()


class TestTailCallKeepsAddressTakenLocals(unittest.TestCase):
    """A `JMP` to another function (a tail call) leaves the
    function's own `__local_*` bytes dead, except one whose address
    the function takes: the callee may have been given it. Before
    this, `return sum(a, 3);` with a local array `a` lost the stores
    that fill `a`, and `sum` read whatever was there."""


    def test_runs(self):
        if not cpp_build.BUILT:
            self.skipTest(cpp_build.REASON)
        from sim.harness import build_sim
        src = (
            "unsigned char sum(unsigned char *a, unsigned char n) {\n"
            "    unsigned char s = 0; while (n--) s += a[n]; return s; }\n"
            "unsigned char wrap(void) {\n"
            "    unsigned char a[3]; a[0] = 4; a[1] = 5; a[2] = 6;\n"
            "    return sum(a, 3); }\n"
            "int main(void) { return wrap(); }\n"
        )
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                self.assertEqual(
                    build_sim(src, optimize=optimize).run().return_int(), 15,
                )
