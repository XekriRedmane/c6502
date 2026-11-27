"""End-to-end tests for the multi-TU linker (`compile.py` given
objects).

Strategy: compile each input .c file separately to an object with
`-c -O`, link the objects into an executable, and simulate it.

Coverage:
  - A cycle whose every call saves the hardware stack runs.
  - Slots go below the pool when it's full.
  - Code run through a pointer parameter stays in each function whose
    slots the link moves apart.
  - Two units' `static` functions of the same name are renamed.
  - The halves of the tables of row addresses that several units
    make with the same bytes are one copy in the program, and units
    read through the copy another unit made.

The tests that read only the linked program's text or the link's
errors are doctest cases in cpp/tests/test_linker_source.cpp.
"""
from __future__ import annotations

import tempfile
import unittest
from pathlib import Path

from tests.link_helpers import (
    compile_object as _compile_to_obj, link as _link,
    link_capture_stderr as _link_capture_stderr, linked_text,
)
from tests.cpp_build import needs_c6502cpp


def _equs(text: str) -> dict[str, int]:
    """The `name EQU $addr` bindings in a linked file."""
    out = {}
    for line in text.splitlines():
        parts = line.split("\t")
        if len(parts) == 3 and parts[1] == "EQU":
            out[parts[0]] = int(parts[2].lstrip("$"), 16)
    return out


@needs_c6502cpp
class TestParameterNames(unittest.TestCase):
    """A zp_abi parameter's slot is named after the parameter, so a
    declaration whose names differ from the definition's spells the
    same byte differently, and an unnamed one spells it by position.
    Each `put` here takes a leading `unsigned int`, which arrives in A
    and X, so that the parameters after it arrive in their slots."""

    _DEF = (
        "unsigned char put(unsigned int w, unsigned char c, int n) "
        "{ return w + c + n; }"
    )

    def test_unnamed_prototype_then_definition_runs(self) -> None:
        from sim.harness import build_sim
        src = ("unsigned char put(unsigned int, unsigned char, int);\n"
               "int main(void) { return put(2, 4, 300); }\n" + self._DEF)
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                self.assertEqual(
                    build_sim(src, optimize=optimize).run().return_int(),
                    (2 + 4 + 300) & 0xFF,
                )


class TestRecursionWithSaves(unittest.TestCase):
    """A cycle whose every call is marked
    `[[c6502::save_hardware_stack]]` links and runs, as Zork's
    print_zstring does."""

    _SAVED = """
    unsigned char depth;
    unsigned char out;
    void walk(unsigned char n) {
        unsigned char here = n;
        if (n == 0) return;
        [[c6502::save_hardware_stack]] walk(n - 1);
        out = out * 3 + here;
    }
    """

    def _link_one(self, src: str) -> tuple[int, str, str]:
        with tempfile.TemporaryDirectory() as tmp:
            tmp = Path(tmp)
            p = tmp / "a.o"
            out = tmp / "linked"
            self.assertEqual(_compile_to_obj(src, p), 0)
            rc, err = _link_capture_stderr([p], out)
            text = linked_text(out) if rc == 0 else ""
        return rc, err, text

    def test_saved_cycle_runs(self):
        from sim.linked import build_linked_sim
        rc, err, text = self._link_one(
            self._SAVED + "int main(void) { walk(3); return out; }\n",
        )
        self.assertEqual(rc, 0, err)
        # out = ((0 * 3 + 1) * 3 + 2) * 3 + 3
        self.assertEqual(build_linked_sim(text.exe).run().return_int(), 18)

if __name__ == "__main__":
    unittest.main()


@needs_c6502cpp
class TestLowZeroPageFallback(unittest.TestCase):
    """When the pool, $80-$FF, has no room left, slots go in the free
    bytes below it, around the runtime's block, before they spill out
    of zero page. `add3`'s leading `base` arrives in A and X, so that
    its other three parameters arrive in slots."""

    _A = (
        "#include <stdint.h>\n"
        "extern uint8_t busy [[c6502::zeropage(0x80)]][124];\n"
        "uint16_t add3(uint16_t base, uint16_t a, uint16_t b, uint16_t c);\n"
        "int main(void) { return add3(4, 1000, 200, 30) == 1234; }\n"
    )
    _B = (
        "#include <stdint.h>\n"
        "uint16_t add3(uint16_t base, uint16_t a, uint16_t b, uint16_t c) {\n"
        "    return base + a + b + c;\n"
        "}\n"
    )

    def test_parameters_go_below_the_pool(self) -> None:
        from passes.runtime_layout import CLASSIC
        with tempfile.TemporaryDirectory() as tmp:
            a, b = Path(tmp) / "a.o", Path(tmp) / "b.o"
            self.assertEqual(_compile_to_obj(self._A, a), 0)
            self.assertEqual(_compile_to_obj(self._B, b), 0)
            out = Path(tmp) / "out"
            self.assertEqual(_link([a, b], out), 0)
            text = linked_text(out)
        equs = _equs(text)
        params = {k: v for k, v in equs.items()
                  if k.startswith("__zpabi_add3_")}
        self.assertEqual(len(params), 6, params)
        runtime = CLASSIC.bytes()
        for name, addr in params.items():
            self.assertLess(addr, 0x80, name)
            self.assertNotIn(addr, runtime, name)
        from sim.linked import build_linked_sim
        sim = build_linked_sim(text.exe)
        result = sim.run()
        self.assertEqual(result.return_int(), 1)


@needs_c6502cpp
class TestPointerInSharedSlotBytes(unittest.TestCase):
    """`f1` and `f2` read through their pointer parameters alike. In
    their unit, neither calls the other, so their parameter slots share
    `$80`-`$81`, and the reads are the same instructions, `LDA ($80),Y`.
    In the program, `f2` is called from `g`, whose parameter takes
    `$80`, so the linker moves `f2`'s slots and the two pointers no
    longer share bytes. Code that both functions run through the
    pointer has to stay in each of them, where `asm_emit` names the
    pointer by that function's own slot. Moved into a function of its
    own by `shared_tails` or `outline_runs`, it was named by `f1`'s slot
    alone, and `f2` read through `f1`'s bytes.

    Each function here takes a byte in X, `[[c6502::in_x]]`, so that
    the parameters after it arrive in their slots."""

    _MAIN = (
        "#include <stdint.h>\n"
        "extern uint8_t out[8];\n"
        "void f1(uint8_t m [[c6502::in_x]], uint8_t *p);\n"
        "void f2(uint8_t m [[c6502::in_x]], uint8_t *p);\n"
        "uint8_t A[3] = {1, 2, 3};\n"
        "uint8_t B[3] = {10, 20, 30};\n"
        "void g(uint8_t m [[c6502::in_x]], uint8_t k) {\n"
        "    f2(m, k ? B : A);\n"
        "}\n"
        "int main(void) {\n"
        "    f1(3, A);\n"
        "    g(4, 1);\n"
        "    return out[0] + out[1] + out[2] + out[3] + out[7];\n"
        "}\n"
    )

    @staticmethod
    def _readers(mark2: int) -> str:
        return (
            "#include <stdint.h>\n"
            "uint8_t out[8];\n"
            + "".join(
                f"void f{n}(uint8_t m [[c6502::in_x]], uint8_t *p) {{\n"
                "    out[3] = m;\n"
                "    out[0] = p[0];\n"
                "    out[1] = p[1];\n"
                "    out[2] = p[2];\n"
                f"    out[7] = {mark};\n"
                "}\n"
                for n, mark in ((1, 1), (2, mark2))
            )
        )

    def _run(self, readers: str) -> tuple[int, str]:
        from sim.linked import build_linked_sim
        with tempfile.TemporaryDirectory() as tmp:
            tmp = Path(tmp)
            a, b = tmp / "readers.o", tmp / "main.o"
            self.assertEqual(_compile_to_obj(readers, a), 0)
            self.assertEqual(_compile_to_obj(self._MAIN, b), 0)
            out = tmp / "linked"
            self.assertEqual(_link([a, b], out), 0)
            text = linked_text(out)
        equs = _equs(text)
        self.assertNotEqual(equs["__zpabi_f1__p_0"], equs["__zpabi_f2__p_0"])
        return build_linked_sim(text.exe).run().return_int(), text

    def test_run_through_pointer_stays_in_each_function(self):
        # The functions differ only in their last store, so the reads
        # would be a run that `outline_runs` moves.
        result, text = self._run(self._readers(2))
        self.assertEqual(result, 10 + 20 + 30 + 4 + 2)
        self.assertNotIn("@run", text)

    def test_tail_through_pointer_stays_in_each_function(self):
        # The functions are the same, so the reads would be in the tail
        # that `shared_tails` shares. The tail starts after them.
        result, text = self._run(self._readers(1))
        self.assertEqual(result, 10 + 20 + 30 + 4 + 1)
        self.assertIn("@tail", text)


@needs_c6502cpp
class TestStaticFunctionsInTwoUnits(unittest.TestCase):
    """Two units each define a `static` function `f`, and a `static`
    function `pick` that returns a pointer, which `-O` returns in a
    zero-page object of its own, `pick@ret`. Each unit spells its `f`'s
    slots `__zpabi_f__x` and `__local_f__*`, so the linker has to rename
    the slots with the function. Otherwise the merged metadata held
    two definitions of `f`, and the link failed. `main`'s `f` calls `a`, which calls the other
    unit's `f`, so the two functions' parameters must have different
    bytes, since the first `f` reads its `x` after the call. Each `f`
    takes `d` in A, `[[c6502::in_a]]`, so that `x` arrives in its
    slot."""

    _A = (
        "#include <stdint.h>\n"
        "static uint8_t T[2] = {40, 50};\n"
        "static uint8_t *pick(uint8_t k) { return &T[k & 1]; }\n"
        "static uint8_t f(uint8_t d [[c6502::in_a]], uint8_t x) {\n"
        "    uint8_t y = x + d;\n"
        "    return x + y + *pick(x);\n"
        "}\n"
        "uint8_t a(uint8_t x) { return f(1, x) + f(1, x + 1); }\n"
    )
    _B = (
        "#include <stdint.h>\n"
        "uint8_t a(uint8_t x);\n"
        "static uint8_t T[2] = {1, 2};\n"
        "static uint8_t *pick(uint8_t k) { return &T[k & 1]; }\n"
        "static uint8_t f(uint8_t d [[c6502::in_a]], uint8_t x) {\n"
        "    uint8_t y = a(x) + d;\n"
        "    return x + y + *pick(x);\n"
        "}\n"
        "int main(void) { return f(0, 5); }\n"
    )

    def test_links_and_runs(self) -> None:
        from sim.linked import build_linked_sim
        with tempfile.TemporaryDirectory() as tmp:
            tmp = Path(tmp)
            a, b, out = tmp / "a.o", tmp / "b.o", tmp / "linked"
            self.assertEqual(_compile_to_obj(self._A, a), 0)
            self.assertEqual(_compile_to_obj(self._B, b), 0)
            rc, err = _link_capture_stderr([a, b], out)
            self.assertEqual(rc, 0, err)
            text = linked_text(out)
        equs = _equs(text)
        # Unit 0's `f` is called from `a`, which unit 1's `f` calls, so
        # the allocators placed them as caller and callee.
        self.assertNotEqual(equs["__zpabi_f__x.0"], equs["__zpabi_f__x.1"])
        self.assertNotIn("__zpabi_f__x", equs)
        self.assertIn("def f.0 ", text)
        self.assertIn("def f.1 ", text)
        self.assertIn("call a -> f.0", text)
        self.assertIn("call f.1 -> a", text)
        # a(5) = f.0(5) + f.0(6) = (5 + 6 + 50) + (6 + 7 + 40) = 114, and
        # f.1(5) = 5 + 114 + 2.
        self.assertEqual(build_linked_sim(text.exe).run().return_int(), 121)

    def test_static_beside_external_function(self) -> None:
        # Unit 1's `f` has external linkage, so it keeps its name and
        # its slots, and only unit 0's is renamed.
        b = (
            "#include <stdint.h>\n"
            "uint8_t a(uint8_t x);\n"
            "uint8_t f(uint8_t d [[c6502::in_a]], uint8_t x) {\n"
            "    uint8_t y = a(x) + d;\n"
            "    return x + y;\n"
            "}\n"
            "int main(void) { return f(0, 5); }\n"
        )
        from sim.linked import build_linked_sim
        with tempfile.TemporaryDirectory() as tmp:
            tmp = Path(tmp)
            pa, pb, out = tmp / "a.o", tmp / "b.o", tmp / "linked"
            self.assertEqual(_compile_to_obj(self._A, pa), 0)
            self.assertEqual(_compile_to_obj(b, pb), 0)
            rc, err = _link_capture_stderr([pa, pb], out)
            self.assertEqual(rc, 0, err)
            text = linked_text(out)
        equs = _equs(text)
        self.assertNotEqual(equs["__zpabi_f__x.0"], equs["__zpabi_f__x"])
        self.assertEqual(build_linked_sim(text.exe).run().return_int(), 119)


# The arrays the row-table tests read. M and B are 0x200 bytes apart, so
# the low bytes of their row addresses are the same.
_ROWS = (
    "#include <stdint.h>\n"
    "extern uint8_t M [[c6502::address(0x6000)]][16][28];\n"
    "extern uint8_t B [[c6502::address(0x6200)]][16][28];\n"
)


@needs_c6502cpp
class TestMergedRowTables(unittest.TestCase):
    """`-O` puts the halves of a 2-D array's table of row addresses in
    objects of their own (`A@lo`, `A@hi`), in each unit that indexes the
    array, and the linker keeps one copy of the halves with the same
    bytes (`linker._merge_identical`). One access to a 16-row array of
    28-byte rows saves more than its 34-byte table, so each function
    below makes a table."""

    def _link(self, units: list[str]) -> tuple[str, dict[str, int]]:
        """Links `units` and returns the linked text and the size of
        each table half in the executable, by name."""
        from binutils import elf as E
        with tempfile.TemporaryDirectory() as tmp:
            tmp = Path(tmp)
            objects = []
            for k, src in enumerate(units):
                obj = tmp / f"u{k}.o"
                self.assertEqual(_compile_to_obj(src, obj), 0)
                objects.append(obj)
            out = tmp / "linked"
            rc, err = _link_capture_stderr(objects, out)
            self.assertEqual(rc, 0, err)
            text = linked_text(out)
            halves = {
                s.name: s.size for s in E.read_file(out).symbols
                if s.type == E.STT_OBJECT and ("@lo" in s.name or "@hi" in s.name)
            }
        return text, halves

    def _run(self, text) -> int:
        from sim.linked import build_linked_sim
        return build_linked_sim(text.exe).run().return_int()

    # main fills M and B, so that M[r][c] is r * 16 + c and B[r][c] is
    # r + c, and returns the sum of what `reads` reads.
    @staticmethod
    def _main(reads: str) -> str:
        return (
            "int main(void) {\n"
            "    for (uint8_t r = 0; r < 16; r++)\n"
            "        for (uint8_t c = 0; c < 28; c++) {\n"
            "            M[r][c] = (uint8_t)(r * 16 + c);\n"
            "            B[r][c] = (uint8_t)(r + c);\n"
            "        }\n"
            f"    return {reads};\n"
            "}\n"
        )

    def test_two_units_share_a_table(self) -> None:
        both = "uint8_t both{k}(uint8_t r, uint8_t c) {{ return M[r][c] + B[r][c]; }}\n"
        u0 = _ROWS + both.format(k=0) + "uint8_t both1(uint8_t r, uint8_t c);\n" \
            + self._main("both0(3, 4) * 10 + both1(15, 27)")
        u1 = _ROWS + both.format(k=1)
        text, halves = self._link([u0, u1])
        # Each unit made B@lo, B@hi, and M@hi, and one copy of each is
        # left, unit 0's.
        self.assertEqual(halves, {"B@lo.0": 17, "B@hi.0": 17, "M@hi.0": 17})
        self.assertIn("M@hi.0,X", text)
        # M[15][27] is (240 + 27) & 0xFF.
        self.assertEqual(self._run(text), (52 + 7) * 10 + (11 + 42))

    def test_three_units_with_different_halves(self) -> None:
        # Unit 1 indexes only M and makes M@lo and M@hi, unit 2 only B.
        # M@lo has the same bytes as unit 0's B@lo, so the program keeps
        # unit 0's three halves, and units 1 and 2 read through them.
        u0 = _ROWS + (
            "uint8_t m1(uint8_t r, uint8_t c);\n"
            "uint8_t b2(uint8_t r, uint8_t c);\n"
            "uint8_t both(uint8_t r, uint8_t c) { return M[r][c] + B[r][c]; }\n"
        ) + self._main("both(1, 2) + m1(14, 5) * 2 + b2(15, 27) * 3")
        u1 = _ROWS + "uint8_t m1(uint8_t r, uint8_t c) { return M[r + 1][c]; }\n"
        u2 = _ROWS + "uint8_t b2(uint8_t r, uint8_t c) { return B[r][c]; }\n"
        text, halves = self._link([u0, u1, u2])
        self.assertEqual(halves, {"B@lo.0": 17, "B@hi.0": 17, "M@hi.0": 17})
        self.assertIn("M@hi.0+1,X", text)
        self.assertEqual(
            self._run(text), (18 + 3) + ((15 * 16 + 5) & 0xFF) * 2 + 42 * 3,
        )

    def test_tables_that_differ_stay_apart(self) -> None:
        # T's table has 11 row addresses, M's 17, so they share no half
        # although T's rows are M's first rows. Each unit's static S is an
        # array of its own, so their tables, whose bytes are relocations
        # against S.0 and S.1, stay apart too. A global G is one array,
        # so both units' tables of it are one.
        s = (
            "static uint8_t S[20][14];\n"
            "uint8_t s{k}(uint8_t r, uint8_t c) {{\n"
            "    S[r][c] = {k} + 1;\n"
            "    return S[r][c] + S[r - 1][c] + G[r][c] + G[r - 1][c];\n"
            "}}\n"
        )
        u0 = _ROWS + "uint8_t G[20][14];\n" \
            + s.format(k=0) + (
                "uint8_t t1(uint8_t r, uint8_t c);\n"
                "uint8_t s1(uint8_t r, uint8_t c);\n"
                "uint8_t m(uint8_t r, uint8_t c) { return M[r][c]; }\n"
            ) + self._main(
                "(G[19][13] = 7, m(2, 3) + t1(9, 27) * 2"
                " + s0(19, 13) * 3 + s1(19, 13) * 5)"
            )
        # T is the first 10 rows of M's memory. A unit can't declare both.
        u1 = (
            "#include <stdint.h>\n"
            "extern uint8_t T [[c6502::address(0x6000)]][10][28];\n"
            "extern uint8_t G[20][14];\n"
        ) + s.format(k=1) \
            + "uint8_t t1(uint8_t r, uint8_t c) { return T[r][c]; }\n"
        text, halves = self._link([u0, u1])
        self.assertEqual(halves, {
            "B@lo": 17, "B@hi": 17, "M@hi": 17, "T@lo": 11, "T@hi": 11,
            "S@lo.0": 21, "S@hi.0": 21, "S@lo.1": 21, "S@hi.1": 21,
            "G@lo.0": 21, "G@hi.0": 21,
        })
        # m(2, 3) = 35, t1(9, 27) = (144 + 27) & 0xFF, s0 = 1 + 0 + 7,
        # s1 = 2 + 0 + 7.
        self.assertEqual(self._run(text), 35 + 171 * 2 + 8 * 3 + 9 * 5)
