"""End-to-end regressions for miscompiles found by random-program
testing in Nov 2026.

Each program runs three ways: through the TAC simulator, through the
asm simulator without `-O` (the same pass list as
`compile.py -S`, peephole fixedpoint included), and through
the asm simulator with `-O`. Every run must return the value
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
    # from_ssa placed the copies for a conditional jump's taken edge
    # after the jump, on the fall-through, when the fall-through edge
    # had copies too: the first edge's copies went after the jump,
    # and the second edge took the last of them for the block's
    # terminator. Found in Zork's get_next_prop.
    ("ssa_copies_on_both_edges_of_a_jump", """
        unsigned char tab[8] = { 9, 7, 5, 3, 1, 0, 0, 0 };
        unsigned char want;
        unsigned char start(void) { return 1; }
        unsigned char next(unsigned char y) { return y + 1; }
        unsigned char g(void) {
            unsigned char y = start();
            if (want != 0) {
                for (;;) {
                    unsigned char num = tab[y];
                    if (num == want) { y = next(y); break; }
                    if (num < want) return 0;
                    y = next(y);
                }
            }
            return tab[y];
        }
        int main(void) {
            unsigned char a, b;
            want = 0; a = g();
            want = 5; b = g();
            return a * 10 + b;
        }
    """, 73),
    # cmp_through_tmp_fold took the store `g3 = p` to the zero-page
    # global `g3` for the temporary of the add after it, since the
    # add was the only read of `g3` in `f`, and dropped the store.
    # `main` reads `g3` afterwards. Found by fuzz.stmts, seed 1809.
    ("store_to_a_global_kept_for_its_readers", """
        unsigned int acc;
        unsigned char g3 [[c6502::zeropage]] = 2;
        unsigned long z4 = 40;
        unsigned long *gp = &z4;
        void mix(unsigned long v) { acc = acc * 3 + (unsigned int)v; }
        void f(void) {
            unsigned char w = 0;
            unsigned long p;
            while (w++ < 3) {
                p = --(*gp);
                g3 = p;
                mix(w + g3);
            }
        }
        int main(void) { f(); mix(g3); return (int)(acc & 0x7FFF); }
    """, 1597),
    # rotate_loop_test entered a rotated loop at its body when the
    # test was known to pass the first time, skipping the test's
    # `LDA i; CMP #1; BCS exit`. carry_known had dropped the body's
    # CLC before `gs5.m0++` because the test's fall-through left C
    # clear, so the first pass added C from before the loop. Found by
    # fuzz.stmts, seed 1262.
    ("rotated_loop_entered_without_its_test", """
        unsigned int acc;
        void mix(unsigned long v) {
            acc = (unsigned int)((acc << 1) | (acc >> 15))
                ^ (unsigned int)v ^ (unsigned int)(v >> 16);
        }
        unsigned int g2 = 0;
        unsigned int g3 = 256;
        unsigned int ga4[4] = {65535, 55822, 2, 255};
        struct S_gs5 { unsigned char pad; unsigned int m0; unsigned int m1; };
        struct S_gs5 gs5 = {0, 63853, 0};
        unsigned int *gp6 = &g2;
        unsigned int *gp7 = &g3;
        void fin(void) {
            mix(g3); mix(ga4[0]); mix(ga4[1]); mix(ga4[2]); mix(ga4[3]);
            mix(gs5.m0); mix(gs5.m1);
        }
        signed char f15(signed char p16, signed char p17,
                        unsigned int p18, signed char *pp19) {
            for (unsigned char i23 = 0; i23 < 1; i23++) {
                (*gp7) = (gs5.m0++);
                (--ga4[0]);
            }
            if ((128UL | (p18++))) {
            }
            if ((p17 || p18)) {
            }
            return 0;
        }
        int main(void) {
            signed char l24 = 0;
            for (unsigned char i27 = 0; i27 < 4; i27++) {
                mix(f15((ga4[(i27 & 3)] + i27), (*gp6), (i27 - i27), &l24));
            }
            (ga4[1] ^= 129);
            fin();
            return (int)(acc & 0x7FFF);
        }
    """, 31411),
    # narrow_widened_arith narrowed a right shift of any extended
    # value to a shift of the extend's low byte, so
    # `(uint8_t)((uint32_t)w >> 5)` lost bits 8 to 12 of `w`. Only an
    # extended byte narrows. The 1-byte compares check that a `>` or
    # `<=` with its constant moved to the right, as `>= k + 1` or `<
    # k + 1`, keeps each boundary and leaves the type's largest value
    # alone.
    ("right_shift_and_compare_constants", """
        unsigned int r;
        void acc(unsigned char v) { r = r * 3 + v; }
        unsigned int ws[3] = { 0x1FE0, 0xABCD, 0x00FF };
        unsigned char bs[5] = { 0xE7, 0x5A, 0x5B, 0xFF, 0x00 };
        signed char cs[5] = { -100, 100, -1, 127, -128 };
        int main(void) {
            for (unsigned char i = 0; i != 3; i++)
                acc((unsigned char)((unsigned long)ws[i] >> 5));
            for (unsigned char i = 0; i != 5; i++) {
                unsigned char b = bs[i];
                acc(b >> 5);
                acc(b > 0x5A);
                acc(b <= 0x5A);
                acc(0x41 < b);
            }
            for (unsigned char i = 0; i != 5; i++) {
                signed char c = cs[i];
                acc((unsigned char)(c >> 1));
                acc((unsigned char)(c >> 3));
                acc(c > 100);
                acc(c <= -2);
            }
            return (int)(r & 0x7FFF);
        }
    """, 8423),
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
    # dead_reg_entry_stub matched `And` / `Or` with `src1` / `src2`
    # fields they don't have, so a slot read only by ORA looked
    # unread and the `STA` that saves the A-passed byte was dropped.
    ("register_param_read_by_ora", """
        unsigned int g1 = 3;
        signed char f3(register int p0) { return (-g1) | p0; }
        int main(void) { return f3(246) + 1000; }
    """, 999),
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
    # value. Both use `register` parameters under -O.
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
    # from_ssa put the copies for every Phi successor of a block in
    # front of its terminator. A do/while's bottom test feeds Phis at
    # both the loop header and the exit, so `i = i + 1` for the
    # header ran before `c = i` for the exit.
    ("do_while_exit_phi_after_header_phi", """
        int main(void) {
            int i = 0; int c = 0;
            do { c = i; i = i + 1; } while (i <= 10);
            return c;
        }
    """, 10),
    # A `register` parameter in A was overwritten by the soft-stack
    # prologue when the function also needed a frame.
    ("register_param_with_frame", """
        char sum_n(register char n) {
            register char acc = 0;
            if (n > 0) do {
                acc = (char)(acc + n);
                n = (char)(n - 1);
            } while (n > 0);
            return acc;
        }
        int main(void) { return sum_n(5); }
    """, 15),
    # Array sizes had to be literals, so `buf[N * 2]` was rejected.
    ("array_size_expression", """
        int g[sizeof(long) * 2];
        int main(void) { int a[2 + 1]; a[2] = 5;
                         return a[2] + (int)sizeof(a) + (int)sizeof(g); }
    """, 27),
    # adc_commute kept the spilled `v` in A across `other = 7;`,
    # which emits as `LDA #7; STA other`, so the add used 7.
    ("adc_commute_across_constant_store", """
        unsigned char total = 10, other, s = 2;
        unsigned char sample(void) { return s; }
        void accumulate(void) {
            unsigned char v = sample();
            other = 7;
            total += v;
        }
        int main(void) { accumulate(); return total * 100 + other; }
    """, 1207),
    # and_sign_bit_branch dropped the AND when A was dead on the
    # fall-through, though the branch target returned A.
    ("and_sign_bit_branch_target_reads_a", """
        unsigned char flags = 5, hits;
        unsigned char top_bit(void) {
            unsigned char t = flags & 0x80;
            if (t) { hits = 1; return 7; }
            return t;
        }
        int main(void) { return top_bit(); }
    """, 0),
    # dec_inc_branch_fold dropped the `LDA timer` after `DEC timer`
    # for the same reason.
    ("dec_inc_branch_fold_target_reads_a", """
        unsigned char timer = 5, hits;
        unsigned char tick(void) {
            unsigned char t = --timer;
            if (t == 0) { hits = 1; return 7; }
            return t;
        }
        int main(void) { return tick(); }
    """, 4),
    # asm_licm moved `g = 5` out of the loop although `*p = 7` writes
    # g through a pointer on every iteration.
    ("licm_store_through_pointer", """
        unsigned char g, out;
        void f(unsigned char *p, unsigned char n) {
            unsigned char i = 0;
            do { g = 5; out = g; *p = 7; i++; } while (i < n);
        }
        int main(void) { f(&g, 2); return out; }
    """, 5),
    # A function whose address is taken only in a static initializer,
    # or only in its own body, got zp_abi, and the call through the
    # pointer passed its arguments on the soft stack.
    ("zp_abi_address_in_static_initializer", """
        unsigned char add3(unsigned char a, unsigned char b,
                           unsigned char c) { return a + b + c; }
        unsigned char (*fp)(unsigned char, unsigned char, unsigned char)
            = add3;
        int main(void) { return fp(1, 2, 3); }
    """, 6),
    ("zp_abi_address_taken_in_own_body", """
        unsigned char (*fp)(unsigned char, unsigned char, unsigned char);
        unsigned char add3(unsigned char a, unsigned char b,
                           unsigned char c) {
            fp = add3;
            return a + b + c;
        }
        int main(void) { add3(0, 0, 0); return fp(1, 2, 3); }
    """, 6),
    # hwreg_eligibility put the high byte of `return e.n` in X,
    # loaded straight from `e`. `e` has its address taken and main
    # makes an indirect call, so `e` lives in main's soft-stack
    # frame, and there's no LDX (FP),Y: the assembler rejected
    # `Mov(Frame, Reg(X))`.
    ("x_load_from_address_taken_frame_local", """
        struct ev { int n; };
        void inc(struct ev *e) { e->n++; }
        int main(void) { struct ev e; void (*c)(struct ev *) = inc;
                         e.n = 1; c(&e); return e.n; }
    """, 2),
    # narrow_widened_arith read a global at the Truncate instead of
    # at the extend, after a call had changed it.
    ("narrowed_read_of_global_after_call", """
        unsigned char g = 10;
        unsigned char h(void) { g = 77; return 1; }
        int main(void) {
            int t = g + 1;
            unsigned char k = h();
            unsigned char x = (unsigned char)t;
            return x + k;
        }
    """, 12),
    # narrow_widened_arith truncated a sign-extended constant as a
    # wide operand and still dropped the extend.
    ("narrowed_extended_constant", """
        int check(int v3) { return (signed char)(((signed char)44) | v3); }
        int main(void) { return check(0); }
    """, 44),
    # narrow_widened_arith registered the low halves of `l16` as
    # `unsigned int` temporaries while it refused to narrow `(unsigned
    # int)(l16 | l16)`, which two Truncates read, and reused the names
    # in the byte OR into `g4` after the other Truncate was gone. The
    # OR's high byte went into `g1`, the byte after `g4`. Found by
    # fuzz.stmts seed 4009.
    ("narrowed_temporary_of_refused_rewrite", """
        unsigned char g4 [[c6502::zeropage]] = 2;
        unsigned char g1 [[c6502::zeropage]] = 7;
        unsigned long long l16 = 0x1236;
        int main(void) {
            g4 = (unsigned int)(l16 | l16);
            return g1 + g4;
        }
    """, 61),
    # loop_counter_to_x took `z4`'s byte 1 for a loop counter: `--z4`
    # in the loop stored it once (the "init") and `z4++` after the loop
    # incremented it (the "step"). A global has a value at entry, so
    # the counter in X held nothing anyone loaded. Found by fuzz.stmts
    # seed 1549.
    ("global_byte_as_loop_counter", """
        unsigned long z4 [[c6502::zeropage]] = 2;
        int f9(void) {
            unsigned char l14 = 6;
            unsigned char w16;
            switch (((int)(l14 - z4)) & 7) {
            case 4:
                w16 = 0;
                while (w16++ < 3) {
                    (--z4);
                }
            case 6:
                return (z4++);
            }
            return 0;
        }
        int main(void) {
            int a = f9();
            int b = f9();
            return a * 100 + b * 10 + (int)z4;
        }
    """, -99),
    # The regalloc pinned `i_0` in Y across a body that reads and
    # writes `arr_6[i_0]` through `(DPTR),Y`, whose `LDY` overwrote
    # it, so the outer loop's second pass stored to the wrong byte. An
    # operand lowered with `LDY #k` now counts as a write of Y. Found by
    # YARPGen.
    ("loop_counter_in_y_across_indirect_access", """
        int arr_0[6];
        signed char arr_4[6];
        long long int arr_6[6];
        int main(void) {
            for (int i = 0; i < 6; i++) { arr_0[i] = 18911; arr_4[i] = -98; }
            for (unsigned char i_0 = 0; i_0 < 6; i_0 += 4)
                for (int i_1 = 3; i_1 < 5; i_1 += 3) {
                    arr_4[i_0] = 5;
                    arr_6[i_0] |= (long long int)(arr_0[i_1 - 1] != arr_0[i_1 + 1]);
                }
            return arr_4[0] * 10 + arr_4[4];
        }
    """, 55),
    # Constant folding gave `k != 0`, a conversion to `bool`, an
    # `int` result, which the 1-byte store then wrote as 2 bytes,
    # clearing `a[1]`. Found by YARPGen.
    ("bool_of_constant_into_array", """
        bool a[3];
        int main(void) {
            a[1] = 1;
            a[0] = (bool)(59071U * 65430U);
            return a[0] * 4 + a[1] * 2 + a[2];
        }
    """, 6),
    # A second narrowing dropped the first one's operand Truncate along
    # with the narrow Binary it replaced.
    ("narrowed_twice", """
        int check(unsigned int v2, unsigned long v4) {
            return (unsigned char)((unsigned int)(v4 * v2));
        }
        int main(void) { return check(65535u, 15UL); }
    """, 241),
    # redundant_load added the shifted byte to the bytes Z reflected
    # after `LSR pattern`, keeping `moves` among them, and dropped the
    # `LDA moves` before the `BEQ`, which then tested the shifted
    # pattern. With PAT = 1, f was never called.
    ("bit_tested_before_a_shift", """
        #include <stdint.h>
        #include <stdbool.h>
        uint8_t PAT [[c6502::zeropage]];
        uint8_t calls;
        void f(void) { calls++; }
        void g(void) {
            uint8_t pattern = PAT;
            do {
                bool moves = pattern & 1;
                pattern >>= 1;
                if (moves) f();
            } while (pattern != 0);
        }
        int main(void) {
            PAT = 1; g();
            PAT = 0x81; g();
            PAT = 6; g();
            return calls;
        }
    """, 5),
    # rotate_loop_test entered the rotated loop at its body because
    # the test, `DEC i; LDA i; CMP #$FF; BEQ exit`, passes on entry,
    # and so skipped the DEC. The body ran 32 times, the first with
    # i = 0x1F.
    ("post_decrement_in_for_test", """
        #include <stdint.h>
        uint8_t T[32];
        uint8_t first;
        uint8_t calls;
        void g(uint8_t v) { if (calls++ == 0) first = v; }
        int main(void) {
            for (uint8_t k = 0; k < 32; k++) T[k] = k;
            for (uint8_t i = 0x1F; i-- != 0;)
                g(T[i]);
            return calls * 100 + first;
        }
    """, 3130),
    # loop_rotate (TAC) skips the first test of a rotated countdown
    # loop, and with it the test's `n++`.
    ("side_effect_in_countdown_test", """
        int n;
        int main(void) {
            int c = 0;
            for (int i = 3; (n++, i) >= 0; i--) c++;
            return n * 10 + c;
        }
    """, 54),
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


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestExternFunctionAddress(unittest.TestCase):
    """`fp = f;` for an `extern` f stored the address of a soft-stack
    slot, because GetAddress lowered every non-static name as a
    local, and f kept zp_abi although a call through `fp` passes its
    arguments on the soft stack. Nothing defines f here, so this
    checks the assembly."""

    _SRC = (
        "unsigned char add3(unsigned char a, unsigned char b,\n"
        "                   unsigned char c);\n"
        "unsigned char (*fp)(unsigned char, unsigned char,\n"
        "                    unsigned char);\n"
        "unsigned char main(void) {\n"
        "    fp = add3;\n"
        "    return add3(1, 2, 3) + fp(1, 2, 3);\n"
        "}\n"
    )

    def test_address_is_the_label(self) -> None:
        from compile import _run_stage
        from preprocessor import preprocess
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                asm = _run_stage(
                    "codegen", preprocess(self._SRC), optimize=optimize,
                )
                self.assertIn("LDA   #<add3", asm)
                self.assertIn("LDA   #>add3", asm)
                self.assertNotIn("__zpabi_add3", asm)


if __name__ == "__main__":
    unittest.main()
