"""Tests for the code that reaches struct members through pointers.

Coverage:

  * `cpp/src/passes/pointer_restage.h`: which wide stores through a pointer
    read the pointer again for each byte (`scalar_at`,
    `restage_store`), and that `P->g = v` and `r->g` then go through
    `(P),Y` and `(r),Y` for both bytes with no copy into DPTR, while
    `N->next = q` on a `struct node *N` keeps the copy.
  * `cpp/src/passes/pointer_self_store.h`: a store through a pointer of the
    byte read from the same place goes, and a slot holding such a
    byte is read through the pointer again where that removes the
    copy. Each rewrite is checked on asm IR, with the facts that stop
    it.
  * `c99_to_tac._calls_first`: `E->level |= h(x)` calls `h` first.
  * `cpp/src/passes/optimization/phi_offset.h`: `c ? &E->a : &E->b` chooses
    the offset in Y.
  * `cpp/src/passes/optimization/distribute_constant.h`: `&p->options[n -
    1]` multiplies the byte `n`.
  * Each program runs in the simulator in both pipelines, with edge
    cases, including a node that holds its own address.

The tests that called the Python functions directly are doctest cases of
the C++ port now, each named for the test it came from
(docs/cpp_port.md, step 8, group G6).
"""
from __future__ import annotations

import io
import unittest
from unittest.mock import patch

from compile import main as compile_main
from sim.harness import build_sim
from tests.cpp_build import needs_c6502cpp


def _asm(src: str) -> str:
    """`compile.py - -S -O` of `src`."""
    out = io.StringIO()
    with patch("sys.stdin", io.StringIO(src)), patch("sys.stdout", out), \
            patch("sys.stderr", io.StringIO()):
        rc = compile_main(["compile.py", "-", "-S", "-O", "-o", "-"])
    assert rc == 0
    return out.getvalue()


def _function(text: str, name: str) -> str:
    """The lines of function `name` in the asm `text`."""
    lines = text.splitlines()
    start = lines.index(f"{name}:")
    end = start + 1
    while end < len(lines) and not (
            lines[end] and not lines[end][0].isspace()
            and lines[end].endswith(":") and not lines[end].startswith(".")):
        end += 1
    return "\n".join(lines[start:end])


_STRUCT = """
#include <stdint.h>
struct s { uint8_t a[13]; uint16_t g; uint8_t *buf; };
struct node { struct node *next; uint16_t v; };
extern struct s *P [[c6502::zeropage(0xF4)]];
extern struct node *N [[c6502::zeropage(0xF6)]];
"""


@needs_c6502cpp
class TestShapes(unittest.TestCase):
    def test_member_store_through_zero_page_pointer(self) -> None:
        text = _function(_asm(_STRUCT + "void f(uint16_t v) { P->g = v; }"),
                         "f")
        self.assertNotIn("DPTR", text)
        self.assertEqual(text.count("STA   (P),Y"), 2)

    def test_member_through_pointer_parameter(self) -> None:
        text = _asm(_STRUCT + """
            void put(struct s *r) { r->g = 0x1234; }
            uint16_t get(struct s *r) { return r->g; }""")
        self.assertNotIn("DPTR", _function(text, "put"))
        self.assertNotIn("DPTR", _function(text, "get"))

    def test_self_referential_member_keeps_the_copy(self) -> None:
        text = _function(
            _asm(_STRUCT + "void f(struct node *q) { N->next = q; }"), "f")
        self.assertIn("STA   (DPTR),Y", text)

    def test_pointer_member_add(self) -> None:
        text = _function(_asm(_STRUCT + "void f(void) { P->buf += 0x100; }"),
                         "f")
        self.assertEqual(text.count("LDA   (P),Y"), 1)
        self.assertEqual(text.count("STA   (P),Y"), 1)
        self.assertIn("LDY   #$10", text)

    def test_compound_assignment_calls_first(self) -> None:
        text = _function(_asm("""
            #include <stdint.h>
            struct mob { uint8_t pad[11]; uint8_t level; };
            extern struct mob *E [[c6502::zeropage(0xF8)]];
            uint8_t h(uint8_t x);
            void f(uint8_t x) { E->level |= h(x); }"""), "f")
        self.assertEqual(
            [line.split()[0] for line in text.splitlines()[2:] if line],
            ["JSR", "LDY", "ORA", "STA", "RTS"])

    def test_choice_of_member_is_an_offset(self) -> None:
        text = _function(_asm("""
            #include <stdint.h>
            struct mob { uint8_t pad[7]; uint8_t sword, armor, dagger; };
            extern struct mob *E [[c6502::zeropage(0xF8)]];
            void f(uint8_t n, uint8_t v) {
                uint8_t *w = n < 3 ? &E->sword : &E->dagger;
                *w = v;
            }"""), "f")
        self.assertIn("LDY   #$07", text)
        self.assertIn("LDY   #$09", text)
        self.assertIn("STA   (E),Y", text)
        self.assertNotIn("ADC", text)

    def test_index_minus_one_multiplies_the_byte(self) -> None:
        text = _function(_asm("""
            #include <stdint.h>
            struct o { uint8_t b[9]; };
            struct o *option(struct o *p, uint8_t n) { return &p[n - 1]; }
            """), "option")
        self.assertNotIn("SBC   #$01", text)
        self.assertIn("SBC   #$09", text)

    def test_member_compare_reads_through_the_pointer(self) -> None:
        text = _function(_asm("""
            #include <stdint.h>
            struct mob { uint8_t b[13]; uint16_t gold; };
            extern struct mob *E [[c6502::zeropage(0xF8)]];
            extern uint16_t W [[c6502::address(0x5A02)]];
            uint8_t c(void) { return W <= E->gold; }"""), "c")
        self.assertNotIn("STA", text)
        self.assertEqual(text.count("LDA   (E),Y"), 2)


_RUN = """
#include <stdint.h>
struct s { uint8_t a[13]; uint16_t g; uint8_t *buf; };
struct node { struct node *next; uint16_t v; };
struct mob { uint8_t pad[7]; uint8_t sword, armor, dagger, level; uint16_t gold; };
struct s obj;
struct mob m;
struct s *P [[c6502::zeropage(0xF4)]];
struct node *N [[c6502::zeropage(0xF6)]];
struct mob *E [[c6502::zeropage(0xF8)]];
uint16_t W;
uint8_t calls;
void put(uint16_t v) { P->g = v; }
void put2(struct s *r, uint16_t v) { r->g = v; }
uint16_t get(struct s *r) { return r->g; }
void step(void) { P->buf += 0x100; }
uint8_t h(uint8_t x) { calls++; return x + 1; }
void choose(uint8_t n, uint8_t v) {
    uint8_t *w = n < 3 ? &E->sword : &E->dagger;
    *w = v;
}
uint8_t le(void) { return W <= E->gold; }
struct mob m2;
void choose_if(uint8_t n, uint8_t v) {
    uint8_t *w = &E->sword;
    if (n > 5 && E->level == 3) w = &E->dagger;
    *w = v;
}
void choose_moved(uint8_t n, uint8_t v) {
    uint8_t *w = &E->sword;
    if (n > 5) { E = &m2; w = &E->dagger; }
    *w = v;
}
struct o { uint8_t b[9]; };
struct o *option(struct o *p, uint8_t n) { return &p[n - 1]; }
long scaled(uint8_t n) { return ((long)n - 5) * 100000L; }
uint16_t times37(uint8_t n) { return (n + 3) * 37; }
"""


@needs_c6502cpp
class TestRuns(unittest.TestCase):
    def _check(self, body: str, expected: int) -> None:
        src = _RUN + "int main(void) {" + body + "}"
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                r = build_sim(src, optimize=optimize).run(max_cycles=2_000_000)
                self.assertFalse(r.timed_out)
                self.assertEqual(r.return_int(), expected)

    def test_wide_member_stores_and_loads(self) -> None:
        self._check("""
            P = &obj;
            put(0x1234);
            put2(&obj, obj.g + 0x0101);
            obj.buf = (uint8_t *)0x20FF;
            step();
            return get(&obj) == 0x1335 && obj.buf == (uint8_t *)0x21FF
                && obj.a[12] == 0;""", 1)

    def test_node_holding_its_own_address(self) -> None:
        self._check("""
            N = (struct node *)&N;
            N->next = (struct node *)0x1234;
            return (uint16_t)N;""", 0x1234)

    def test_compound_assignment_with_a_call(self) -> None:
        self._check("""
            E = &m;
            m.level = 0x40;
            E->level |= h(2);
            E->level -= h(0);
            return m.level * 16 + calls;""", (0x43 - 1) * 16 + 2)

    def test_choice_of_member(self) -> None:
        self._check("""
            E = &m;
            choose(2, 11);
            choose(3, 22);
            choose(255, 33);
            return m.sword * 256 + m.dagger + m.armor;""", 11 * 256 + 33)

    def test_choice_of_member_after_a_test(self) -> None:
        self._check("""
            E = &m;
            m.level = 3;
            choose_if(5, 11);
            choose_if(6, 22);
            m.level = 4;
            choose_if(7, 33);
            return m.sword * 256 + m.dagger;""", 33 * 256 + 22)

    def test_choice_of_member_after_the_pointer_moves(self) -> None:
        self._check("""
            E = &m;
            choose_moved(5, 11);
            choose_moved(6, 22);
            return m.sword * 256 + m2.dagger + m.dagger;""", 11 * 256 + 22)

    def test_member_compare(self) -> None:
        self._check("""
            E = &m;
            m.gold = 0x1200;
            W = 0x11FF; uint8_t a = le();
            W = 0x1200; uint8_t b = le();
            W = 0x1201; uint8_t c = le();
            W = 0x1300; uint8_t d = le();
            return a + 2 * b + 4 * c + 8 * d;""", 3)

    def test_index_minus_one(self) -> None:
        for n, expected in ((0, -9), (1, 0), (30, 261), (255, 2286)):
            with self.subTest(n=n):
                self._check(f"""
                    struct o *base = (struct o *)0x4000;
                    return (uint16_t)option(base, {n}) - 0x4000;""",
                    expected & 0xFFFF)

    def test_other_products(self) -> None:
        self._check("return scaled(0) == -500000L && scaled(255) == "
                    "25000000L && times37(0) == 111 && times37(255) == "
                    "(uint16_t)(258 * 37);", 1)


if __name__ == "__main__":
    unittest.main()
