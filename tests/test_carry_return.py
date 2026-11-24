"""Tests for `[[c6502::return_bool_in_carry]]`.

A function with the attribute returns `e != 0` in the carry flag.
The callee ends in `CMP #1` (or `SEC` / `CLC` once the value is
known), and the caller reads C with `BCC` / `BCS`, or captures it
with `LDA #0; ROL A`. The peepholes are in `passes/carry_return.py`.

The tests that called the Python functions directly are doctest cases of
the C++ port now, each named for the test it came from
(docs/cpp_port.md, step 8, group G6).
"""

from __future__ import annotations

import shutil
import unittest

from passes.type_checking import TypeCheckError
from sim.harness import build_sim

_CARRY = "[[c6502::return_bool_in_carry]] "
_BOOL = "#include <stdbool.h>\n"


def _codegen(src, optimize=True):
    from compile import _run_stage
    from preprocessor import preprocess
    return _run_stage("codegen", preprocess(src), optimize=optimize)


def _function_text(asm, name):
    return asm.split(f"\n{name}:")[1].split("SUBROUTINE")[1].split(
        "\n\n", 2)[1]


class TestFrontEnd(unittest.TestCase):

    def _tac(self, src):
        from compile import _run_stage
        return _run_stage("tac", src)


    def test_rejects_non_bool_return(self):
        for ret in ("int", "unsigned char", "char"):
            with self.subTest(ret):
                with self.assertRaisesRegex(TypeCheckError, "return type"):
                    self._tac(_CARRY + f"{ret} f(void) {{ return 1; }}")

    def test_declarations_must_agree(self):
        with self.assertRaisesRegex(TypeCheckError, "must agree"):
            self._tac("bool f(void);\n"
                      + _CARRY + "bool f(void) { return 1; }")

    def test_rejects_address(self):
        with self.assertRaisesRegex(TypeCheckError, "address"):
            self._tac(_CARRY + "bool f(void) { return 1; }\n"
                      "bool (*p)(void) = f;")


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestSim(unittest.TestCase):
    """Each program runs in both pipelines. Without -O every
    callee has a frame, so its return goes through the `PHP` / `PLP`
    epilogue."""

    def _check(self, src, expected):
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                got = build_sim(src, optimize=optimize).run()
                self.assertEqual(got.return_int(), expected)

    def test_branch_and_value_uses(self):
        src = _BOOL + _CARRY + (
            "bool is_small(unsigned char x) { return x < 10; }\n"
            + _CARRY + "bool always(void) { return 1; }\n"
            "unsigned char g;\n"
            "int main(void) {\n"
            "    unsigned char n = 0;\n"
            "    if (is_small(3)) n += 1;\n"
            "    if (!is_small(30)) n += 2;\n"
            "    if (always()) n += 4;\n"
            "    g = is_small(5) + is_small(50) * 2;\n"
            "    return n * 10 + g;\n"
            "}\n"
        )
        self._check(src, 71)

    def test_search_loop(self):
        # The shape the attribute is for: `return true` from inside
        # a loop, `return false` after it.
        src = _BOOL + (
            "unsigned char tab[5] = {4, 9, 16, 25, 36};\n"
            + _CARRY + "bool contains(unsigned char k) {\n"
            "    for (unsigned char i = 0; i < 5; i++)\n"
            "        if (tab[i] == k) return 1;\n"
            "    return 0;\n"
            "}\n"
            "int main(void) {\n"
            "    int n = 0;\n"
            "    for (unsigned char k = 0; k < 40; k++)\n"
            "        if (contains(k)) n += k;\n"
            "    return n;\n"
            "}\n"
        )
        self._check(src, 4 + 9 + 16 + 25 + 36)

    def test_nonzero_value_is_true(self):
        # `return x & 4;` returns `(x & 4) != 0`, so a caller sees 1.
        src = _BOOL + _CARRY + (
            "bool bit2(unsigned int x) { return x & 4; }\n"
            + _CARRY + "bool nonnull(int *p) { return p; }\n"
            "int v;\n"
            "int main(void) {\n"
            "    return bit2(4) + bit2(3) * 10 + bit2(0x104) * 100\n"
            "        + nonnull(&v) * 1000 + nonnull(0) * 10000;\n"
            "}\n"
        )
        self._check(src, 1 + 100 + 1000)

    def test_recursive_soft_stack(self):
        # Recursion keeps `is_even` on the soft stack even with
        # -O, so its return goes through the frame teardown.
        src = _BOOL + _CARRY + (
            "bool is_even(unsigned char n) {\n"
            "    if (n == 0) return 1;\n"
            "    return !is_even(n - 1);\n"
            "}\n"
            "int main(void) {\n"
            "    int r = 0;\n"
            "    for (unsigned char i = 0; i < 9; i++)\n"
            "        r = r * 2 + is_even(i);\n"
            "    return r;\n"
            "}\n"
        )
        self._check(src, 0b101010101)

    def test_carry_to_carry(self):
        src = _BOOL + _CARRY + (
            "bool odd(unsigned char x) { return x & 1; }\n"
            + _CARRY + "bool odd_too(unsigned char x) { return odd(x); }\n"
            + _CARRY + "bool neither(unsigned char x) "
            "{ return !odd(x) && !odd(x >> 1); }\n"
            "int main(void) {\n"
            "    int r = 0;\n"
            "    for (unsigned char i = 0; i < 8; i++)\n"
            "        r = r * 4 + odd_too(i) * 2 + neither(i);\n"
            "    return r;\n"
            "}\n"
        )
        expected = 0
        for i in range(8):
            expected = expected * 4 + (i & 1) * 2 + (i & 3 == 0)
        self._check(src, expected & 0xFFFF)

    def test_loop_condition(self):
        src = _BOOL + _CARRY + (
            "bool done(unsigned char i) { return i * 3 > 20; }\n"
            "int main(void) {\n"
            "    unsigned char i = 0;\n"
            "    while (!done(i)) i++;\n"
            "    return i;\n"
            "}\n"
        )
        self._check(src, 7)


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestCodeShape(unittest.TestCase):

    def test_constant_returns_are_sec_clc(self):
        asm = _codegen(_BOOL + _CARRY + (
            "bool yes(void) { return 1; }\n"
            + _CARRY + "bool no(void) { return 0; }\n"
            "int main(void) { return yes() + no(); }\n"
        ))
        self.assertEqual(_function_text(asm, "yes").split(), ["SEC", "RTS"])
        self.assertEqual(_function_text(asm, "no").split(), ["CLC", "RTS"])

    def test_caller_branches_on_carry(self):
        asm = _codegen(_BOOL + (
            "unsigned char g;\n"
            + _CARRY + "bool f(unsigned char x) { return x == 3; }\n"
            "int main(void) { if (f(g)) g = 7; return g; }\n"
        ))
        main = asm.split("\nmain:")[1]
        self.assertRegex(main, r"JSR   f\n   BC[CS]   ")
        self.assertNotIn("ROL", main)

    def test_soft_stack_epilogue_keeps_carry(self):
        asm = _codegen(_BOOL + _CARRY + (
            "bool f(unsigned char n) { if (n == 0) return 1; "
            "return f(n - 1); }\n"
            "int main(void) { return f(3); }\n"
        ))
        f = asm.split("\nf:")[1].split("\nmain:")[0]
        self.assertIn("PHP", f)
        self.assertIn("PLP", f)


# A struct of a value and a `bool`, which a `return_bool_in_carry`
# function returns with the value in A (and X) and the flag in C.
_ZC = "struct zc { unsigned char v; bool end; };\n"
_WV = "struct wv { int v; bool end; };\n"


def _tac_main(src):
    """main()'s result on unoptimized TAC. `tac_sim` doesn't run the
    instructions the TAC optimizer introduces, such as
    `IndirectIndexedStore`."""
    import sim.harness as H
    from compile import _resolved
    from tac_sim import Simulator
    ast, syms, types = H.type_check_program(
        _resolved(H.preprocess(src)),
    )
    tac = H.translate_to_tac(ast, syms, types)
    return Simulator(tac, syms, types).call("main", [])


class TestStructFrontEnd(unittest.TestCase):

    def _tac(self, src):
        from compile import _run_stage
        return _run_stage("tac", src)

    def test_accepts_value_and_flag(self):
        for value in ("unsigned char", "signed char", "char", "int",
                      "unsigned int"):
            with self.subTest(value):
                self._tac(
                    f"struct s {{ {value} v; bool end; }};\n"
                    + _CARRY + "struct s f(void) { struct s r; r.v = 1; "
                    "r.end = 0; return r; }"
                )

    def test_rejects_other_shapes(self):
        shapes = {
            "three members": "unsigned char v; bool end; "
                             "unsigned char x;",
            "one member": "unsigned char v;",
            "pointer value": "unsigned char *v; bool end;",
            "long value": "long v; bool end;",
            "int flag": "unsigned char v; int end;",
            "unsigned char flag": "unsigned char v; unsigned char end;",
            "char flag": "unsigned char v; char end;",
            "flag first": "bool end; int v;",
        }
        for label, members in shapes.items():
            with self.subTest(label):
                with self.assertRaisesRegex(
                        TypeCheckError, "exactly two members"):
                    self._tac(
                        f"struct s {{ {members} }};\n"
                        + _CARRY + "struct s f(void);"
                    )

    def test_rejects_union(self):
        with self.assertRaisesRegex(TypeCheckError, "exactly two members"):
            self._tac("union u { unsigned char v; bool end; };\n"
                      + _CARRY + "union u f(void);")

    def test_no_hidden_pointer(self):
        tac = self._tac(_ZC + _CARRY + (
            "struct zc f(void) { struct zc r; r.v = 3; r.end = 1; "
            "return r; }\n"
            "int main(void) { struct zc c = f(); return c.v; }\n"
        ))
        self.assertNotIn(".sret", tac)


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestStructSim(unittest.TestCase):
    """Each program runs on both TACs and through both pipelines."""

    def _check(self, src, expected):
        self.assertEqual(_tac_main(src), expected)
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                got = build_sim(src, optimize=optimize).run()
                self.assertEqual(got.return_int(), expected)

    def test_byte_value(self):
        # The shape of Zork's get_next_zchar: a value, and a flag that
        # marks the end of the data.
        src = _ZC + (
            "unsigned char data[6] = {5, 7, 9, 0xFF, 3, 4};\n"
            "unsigned char pos;\n"
            + _CARRY + "struct zc next(void) {\n"
            "    struct zc r;\n"
            "    r.v = data[pos++];\n"
            "    r.end = r.v == 0xFF;\n"
            "    return r;\n"
            "}\n"
            "int main(void) {\n"
            "    int sum = 0;\n"
            "    for (;;) {\n"
            "        struct zc c = next();\n"
            "        if (c.end) break;\n"
            "        sum += c.v;\n"
            "    }\n"
            "    return sum * 10 + pos;\n"
            "}\n"
        )
        self._check(src, 21 * 10 + 4)

    def test_int_value(self):
        src = _WV + _CARRY + (
            "struct wv triple(int x) {\n"
            "    struct wv r;\n"
            "    r.v = x * 3;\n"
            "    r.end = x < 0;\n"
            "    return r;\n"
            "}\n"
            "int main(void) {\n"
            "    struct wv a = triple(300);\n"
            "    struct wv b = triple(-7);\n"
            "    return a.v + a.end * 10000 + b.v * 2 + b.end * 20000;\n"
            "}\n"
        )
        self._check(src, 900 + 20000 - 42)

    def test_flag_is_zero_or_one(self):
        # The caller rebuilds the flag from C, so any nonzero flag in
        # the callee arrives as 1.
        src = _ZC + _CARRY + (
            "struct zc f(unsigned char x) {\n"
            "    struct zc r;\n"
            "    r.v = x;\n"
            "    r.end = x & 6;\n"
            "    return r;\n"
            "}\n"
            "int main(void) {\n"
            "    struct zc a = f(4);\n"
            "    struct zc b = f(9);\n"
            "    return a.end * 100 + b.end * 10 + a.v + b.v;\n"
            "}\n"
        )
        self._check(src, 100 + 13)

    def test_recursive_soft_stack(self):
        # Recursion keeps `count` on the soft stack, so A, X, and C all
        # cross its frame teardown.
        src = _WV + _CARRY + (
            "struct wv count(unsigned char n) {\n"
            "    struct wv r;\n"
            "    if (n == 0) { r.v = 1000; r.end = 1; return r; }\n"
            "    r = count(n - 1);\n"
            "    r.v = r.v + n;\n"
            "    r.end = !r.end;\n"
            "    return r;\n"
            "}\n"
            "int main(void) {\n"
            "    struct wv a = count(4);\n"
            "    struct wv b = count(5);\n"
            "    return a.v + a.end * 10000 + b.end * 20000;\n"
            "}\n"
        )
        self._check(src, 1010 + 10000)

    def test_returned_call(self):
        # `return f();` passes the callee's A and C straight through.
        src = _ZC + _CARRY + (
            "struct zc inner(unsigned char x) {\n"
            "    struct zc r; r.v = x + 1; r.end = x > 5; return r;\n"
            "}\n"
            + _CARRY + "struct zc outer(unsigned char x) "
            "{ return inner(x * 2); }\n"
            "int main(void) {\n"
            "    struct zc a = outer(2);\n"
            "    struct zc b = outer(3);\n"
            "    return a.v * 100 + a.end * 10 + b.end + b.v * 1000;\n"
            "}\n"
        )
        self._check(src, 500 + 0 + 1 + 7000)

    def test_value_used_before_the_flag(self):
        # Zork's `PAGE_TABLE_INDEX2 = found.slot; if (found.not_found)`.
        src = _ZC + (
            "unsigned char data[6] = {5, 7, 9, 0xFF, 3, 4};\n"
            "unsigned char pos, last;\n"
            + _CARRY + "struct zc next(void) {\n"
            "    struct zc r;\n"
            "    r.v = data[pos++];\n"
            "    r.end = r.v == 0xFF;\n"
            "    return r;\n"
            "}\n"
            "int main(void) {\n"
            "    int sum = 0;\n"
            "    for (;;) {\n"
            "        struct zc c = next();\n"
            "        last = c.v;\n"
            "        if (c.end) break;\n"
            "        sum += c.v;\n"
            "    }\n"
            "    return sum * 10 + pos + last;\n"
            "}\n"
        )
        self._check(src, 21 * 10 + 4 + 0xFF)

    def test_fixed_address_routine(self):
        # A routine at $D000 that returns 42 in A with C set.
        src = _ZC + (
            "[[c6502::zp_abi, c6502::return_bool_in_carry, c6502::"
            "address(0xD000)]]\n"
            "struct zc rom(void);\n"
            "int main(void) { struct zc c = rom(); "
            "return c.v * 2 + c.end; }\n"
        )
        lda_sec_rts = bytes([0xA9, 42, 0x38, 0x60])
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                sim = build_sim(src, optimize=optimize)
                sim.mpu.memory[0xD000:0xD004] = lda_sec_rts
                self.assertEqual(sim.run().return_int(), 85)


class TestStructCodeShape(unittest.TestCase):

    def test_callee_sets_c_then_loads_value(self):
        asm = _codegen(_WV + "struct wv g;\n" + _CARRY + (
            "struct wv f(void) { return g; }\n"
            "int main(void) { struct wv r = f(); return r.v; }\n"
        ))
        self.assertEqual(_function_text(asm, "f").split(), [
            "LDA", "g+2", "CMP", "#$01", "LDX", "g+1", "LDA", "g", "RTS",
        ])

    def test_value_used_before_the_flag(self):
        # `out = c.v;` before `if (c.end)` used to store the captured
        # flag and load it back; now the caller branches on C.
        asm = _codegen(_ZC + "unsigned char out;\n" + _CARRY + (
            "struct zc f(void);\n"
            "int main(void) {\n"
            "    struct zc c = f();\n"
            "    out = c.v;\n"
            "    if (c.end) return 1;\n"
            "    return 2;\n"
            "}\n"
        ))
        main = _function_text(asm, "main")
        self.assertNotIn("ROL", main)
        self.assertRegex(main, r"JSR\s+f\s+STA\s+out\s+BC[CS]")

    def test_zp_abi_eligible(self):
        # No hidden pointer, so `f` gets zp_abi under -O and
        # the call passes nothing on the soft stack.
        asm = _codegen(_ZC + _CARRY + (
            "struct zc f(unsigned char x) { struct zc r; r.v = x; "
            "r.end = x == 3; return r; }\n"
            "int main(void) { struct zc c = f(3); return c.v + c.end; }\n"
        ))
        self.assertIn("__zpabi_f__x", asm)
        self.assertNotIn("SSP", _function_text(asm, "main"))


if __name__ == "__main__":
    unittest.main()
