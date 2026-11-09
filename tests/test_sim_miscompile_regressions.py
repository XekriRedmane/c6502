"""End-to-end regressions for miscompiles found by random-program
testing in Nov 2026.

Each program runs three ways: through the TAC simulator, through the
asm simulator without `--optimize` (the same pass list as
`compile.py --codegen`, peephole fixedpoint included), and through
the asm simulator with `--optimize`. Every run must return the value
C99 gives under c6502's widths (int 16 bits, long 32, long long 64,
plain char unsigned).

The comment on each case names the defect it pins down."""

from __future__ import annotations

import shutil
import unittest

from sim.harness import build_sim
from tests.test_sim_chapters import _run_program as _run_tac


# (name, C source, expected int return of main)
_CASES: list[tuple[str, str, int]] = [
    # flags_dead_at treated a label as the end of flag liveness, so
    # dead_a_arith dropped the signed compare's `EOR #$80` overflow
    # correction just before `.cmp_novf:`.
    ("signed_compare_overflow_if", """
        int main(void) { int a = -30000; int b = 30000;
                         if (a < b) return 1; return 0; }
    """, 1),
    ("signed_compare_overflow_value", """
        int lt(int a, int b) { return a < b; }
        int main(void) { return lt(-30000, 30000) + lt(-32768, 113) * 2; }
    """, 3),
    ("signed_compare_overflow_long", """
        long m = -2147483647L - 1;
        int main(void) { return (0L < m) + (m < 5L) * 2; }
    """, 2),
    # redundant_load kept "Z reflects Frame(k)" across a store into a
    # soft-stack slot, whose `LDY #off` overwrites the flags, and so
    # dropped the LDA that the sign-extension BMI reads.
    ("signed_char_sign_extension", """
        int main(void) { signed char c = -5; int r = c;
                         return (c < 0) + (r == -5) * 2; }
    """, 3),
    # dec_peephole folded the low byte of `x - 1` into DEC while the
    # high byte's `SBC #0` still read the carry.
    ("sub_one_high_byte_borrow", """
        int main(void) {
            int a[4] = {1, 2, 3, 4}; int i = 0;
            a[i++] += 10; a[i++] *= 3; a[--i] <<= 1;
            return a[0] + a[1] + i + a[2] - 1;
        }
    """, 26),
    # copy_folding fused `Copy(t, g); Copy(g, h)` into `Copy(t, h)`
    # because g had one direct use, losing the store to a global.
    ("global_store_kept", """
        int g;
        int rd(void) { return g; }
        int main(void) { int h; g = 7; h = g; return h + rd(); }
    """, 14),
    # ... and to an address-taken struct read through a pointer.
    ("address_taken_store_kept", """
        struct P { int x; char c; };
        struct P mk(int x) { struct P p; p.x = x; p.c = 2; return p; }
        int main(void) { struct P a = mk(30); struct P b = a;
                         b.x = a.x; return b.x; }
    """, 30),
    # Same-width signed/unsigned casts were no-ops in c99_to_tac, so
    # `/`, `%`, `>>`, and compares took the operand's old signedness.
    ("unsigned_cast_shift", """
        int main(void) { int s = -32768; return ((unsigned)s >> 15) == 1; }
    """, 1),
    ("unsigned_cast_through_variable", """
        int main(void) { int s = -2; unsigned u = (unsigned)s;
                         return (u / 2u) == 32767u; }
    """, 1),
    ("signed_cast_shift", """
        int main(void) { unsigned u = 65535u; return ((int)u) >> 4; }
    """, -1),
    ("implicit_conversion_modulo", """
        int main(void) { int x = -1; return (int)(x % 256u); }
    """, 255),
    ("long_cast_compare", """
        int main(void) { signed char z = 0;
                         return ((long)2147483648UL) > z; }
    """, 0),
    # Compound assignment had its own copy of the cast lowering, with
    # the same signedness erasure and no char widening for FP.
    ("compound_unsigned_divide", """
        int main(void) { int i = -1; i /= 2u; return i; }
    """, 32767),
    ("compound_uchar_times_double", """
        int main(void) { unsigned char uc = 2; uc *= 3.0; return uc; }
    """, 6),
    # redundant_store dropped `LDA v3; STA tmp` when tmp already held
    # v3, although the following SBC read the loaded A.
    ("redundant_store_live_load", """
        unsigned char v3 = 249;
        int main(void) { return (v3 & 1u) > (unsigned char)v3; }
    """, 0),
    # Case values were masked to 8 bits for int and 16 for long.
    ("case_label_wide", """
        int f(int x) { switch (x) { case 44: return 1; case 300: return 2;
                                    default: return 0; } }
        int g(long x) { switch (x) { case 70000L: return 4;
                                     default: return 0; } }
        int main(void) { return f(300) + g(70000L); }
    """, 6),
    # Case labels and static initializers rejected operators.
    ("case_label_expressions", """
        int f(int x) {
            switch (x) {
            case -1000: return 1;
            case 2 * 5: return 10;
            case (1 << 8) + 44: return 100;
            case sizeof(long): return 7;
            default: return 1000;
            }
        }
        int main(void) { return f(-1000) + f(10) + f(300) + f(44); }
    """, 1111),
    ("static_initializer_expressions", """
        int a = -5;
        long b = 1L << 20;
        unsigned c = ~0u;
        char d = -1;
        int f[3] = {-1, 2 * 3, -4};
        int g = sizeof(long) + (3 > 2 ? 10 : 20);
        int main(void) {
            static int s = -300;
            return (a == -5) + (b == 1048576L) * 2 + (c == 65535u) * 4
                + (d == 255) * 8 + (f[0] + f[1] + f[2] == 1) * 16
                + (g == 14) * 32 + (s == -300) * 64;
        }
    """, 127),
    # Function names didn't decay to function pointers, and the sim
    # runtime had no `icall` trampoline.
    ("function_designator_decay", """
        int add(int a, int b) { return a + b; }
        int mul(int a, int b) { return a * b; }
        int apply(int (*f)(int, int), int a, int b) { return f(a, b); }
        int (*g)(int, int) = add;
        int main(void) {
            int (*h)(int, int) = mul;
            return apply(add, 3, 4) - apply(h, 1, 2) + g(1, 2) + (h == mul);
        }
    """, 9),
    # The parser gave a function returning a function pointer the
    # returned type's parameter list, and prototypes with several
    # unnamed parameters were rejected as duplicates.
    ("function_returning_function_pointer", """
        int add(int, int);
        int mul(int, int);
        int add(int a, int b) { return a + b; }
        int mul(int a, int b) { return a * b; }
        int (*pick(int which))(int, int) { return which ? mul : add; }
        int main(void) { int (*k)(int, int) = pick(1); return k(2, 3); }
    """, 6),
    # x_save_slot_load rewrote `LDA slot` to `TXA` after X had been
    # overwritten, by a call returning in X or by an LDX of another
    # value. Both use `register` parameters under --optimize.
    ("register_param_across_call", """
        int f3(void) { return 0; }
        int f0(register unsigned char p0, register unsigned char p1) {
            f3();
            return p1 << 7;
        }
        int main(void) { return f0(2, 249); }
    """, 31872),
    ("register_params_both_used", """
        static unsigned char hud_buf[40];
        unsigned char paint(register unsigned char x_pixel,
                            register unsigned char color) {
            register unsigned char tmp = color;
            hud_buf[x_pixel] = tmp;
            return tmp;
        }
        int main(void) {
            paint(3, 0x7F);
            paint(7, 0x40);
            return hud_buf[3] + hud_buf[7];
        }
    """, 191),
    # A zp_abi function kept its parameters in the $80-$BF window
    # across a call to a soft-stack (here recursive) function, which
    # uses that window as caller-saved scratch.
    ("zp_abi_caller_of_recursive", """
        int rec(int d) { if (d <= 0) return 0; return rec(d - 1) + 1; }
        int f0(int p0) { int l = rec(3); return p0 + l; }
        int main(void) { return f0(4); }
    """, 7),
    # The asm interference graph skipped values live across a call
    # whose only uses were in a block not yet visited, so they got
    # caller-saved bytes the callee overwrote.
    ("value_live_across_call_into_later_block", """
        int g0 = 96;
        unsigned f2(int *pp) { return 0; }
        signed char f1(void) {
            int box2 = 0; unsigned (*fp)(int *) = f2; fp(&box2); return 0;
        }
        unsigned char f0(unsigned p1) {
            unsigned l1 = 1000 ^ (p1 >> (p1 & 7));
            l1 ^= f1(); g0 += l1; return 0;
        }
        int main(void) { f0(105); return g0; }
    """, 1084),
    # fold_truncate_extend read a global at the Truncate instead of
    # at the extend, after the global had changed.
    ("truncate_of_extended_global", """
        signed char g = 10;
        int f(signed char x) { return x; }
        int main(void) { long l2 = (int)g; g += 1; return f((signed char)l2); }
    """, 10),
    # narrow_widened_arith narrowed `Truncate(wide XOR)` and deleted
    # the wide XOR, which the Add still read.
    ("narrowed_binary_with_other_reader", """
        long g1 = 39;
        int f4(void) { return 0; }
        void f3(void) {
            unsigned long l0 = 3;
            l0 ^= f4();
            l0 += (int)l0;
            g1 += l0;
        }
        int main(void) { f3(); return (int)g1; }
    """, 45),
    # Array sizes had to be literals, so `buf[N * 2]` was rejected.
    ("array_size_expression", """
        int g[sizeof(long) * 2];
        int main(void) { int a[2 + 1]; a[2] = 5;
                         return a[2] + (int)sizeof(a) + (int)sizeof(g); }
    """, 27),
]


def _s16(v: int) -> int:
    v &= 0xFFFF
    return v - 0x10000 if v & 0x8000 else v


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestMiscompileRegressions(unittest.TestCase):

    def test_tac_sim(self) -> None:
        for name, src, expected in _CASES:
            with self.subTest(case=name):
                self.assertEqual(_s16(_run_tac(src)), expected)

    def test_asm_sim(self) -> None:
        for name, src, expected in _CASES:
            for optimize in (False, True):
                with self.subTest(case=name, optimize=optimize):
                    result = build_sim(src, optimize=optimize).run()
                    self.assertFalse(result.timed_out)
                    self.assertEqual(result.return_int_signed(), expected)


if __name__ == "__main__":
    unittest.main()
