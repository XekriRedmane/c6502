// The tests of tests/test_sim_miscompile_regressions.py that run the
// TAC interpreter or read the `-S` text. The simulator's runs are in
// test_sim_miscompile_regressions_sim.cpp.
#include <string>
#include <vector>

#include "compile_case.h"
#include "doctest.h"

namespace {

namespace suite = c6502::suite;

struct Program {
  const char* name;
  std::string source;
  long long expected;
};

// Returns the low 16 bits of `main`'s value on the TAC, as an `int`.
long long tac_int(const std::string& source) {
  const auto value = suite::run_program(source);
  REQUIRE(value.has_value());
  long long v = static_cast<long long>(*value & 0xFFFF);
  if (v & 0x8000) v -= 0x10000;
  return v;
}

}  // namespace

TEST_CASE("sim_miscompile_regressions: the programs return their values on the TAC interpreter") {
  const std::vector<Program> programs = {
      {"ssa_copies_on_both_edges_of_a_jump", R"(
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
    )", 73},
      {"store_to_a_global_kept_for_its_readers", R"(
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
    )", 1597},
      {"rotated_loop_entered_without_its_test", R"(
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
    )", 31411},
      {"right_shift_and_compare_constants", R"(
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
    )", 8423},
      {"signed_compare_overflow_if", R"(
        int main(void) { int a = -30000; int b = 30000;
                         if (a < b) return 1; return 0; }
    )", 1},
      {"signed_compare_overflow_value", R"(
        int lt(int a, int b) { return a < b; }
        int main(void) { return lt(-30000, 30000) + lt(-32768, 113) * 2; }
    )", 3},
      {"signed_compare_overflow_long", R"(
        long m = -2147483647L - 1;
        int main(void) { return (0L < m) + (m < 5L) * 2; }
    )", 2},
      {"signed_char_sign_extension", R"(
        int main(void) { signed char c = -5; int r = c;
                         return (c < 0) + (r == -5) * 2; }
    )", 3},
      {"sub_one_high_byte_borrow", R"(
        int main(void) {
            int a[4] = {1, 2, 3, 4}; int i = 0;
            a[i++] += 10; a[i++] *= 3; a[--i] <<= 1;
            return a[0] + a[1] + i + a[2] - 1;
        }
    )", 26},
      {"global_store_kept", R"(
        int g;
        int rd(void) { return g; }
        int main(void) { int h; g = 7; h = g; return h + rd(); }
    )", 14},
      {"address_taken_store_kept", R"(
        struct P { int x; char c; };
        struct P mk(int x) { struct P p; p.x = x; p.c = 2; return p; }
        int main(void) { struct P a = mk(30); struct P b = a;
                         b.x = a.x; return b.x; }
    )", 30},
      {"unsigned_cast_shift", R"(
        int main(void) { int s = -32768; return ((unsigned)s >> 15) == 1; }
    )", 1},
      {"unsigned_cast_through_variable", R"(
        int main(void) { int s = -2; unsigned u = (unsigned)s;
                         return (u / 2u) == 32767u; }
    )", 1},
      {"signed_cast_shift", R"(
        int main(void) { unsigned u = 65535u; return ((int)u) >> 4; }
    )", -1},
      {"implicit_conversion_modulo", R"(
        int main(void) { int x = -1; return (int)(x % 256u); }
    )", 255},
      {"long_cast_compare", R"(
        int main(void) { signed char z = 0;
                         return ((long)2147483648UL) > z; }
    )", 0},
      {"compound_unsigned_divide", R"(
        int main(void) { int i = -1; i /= 2u; return i; }
    )", 32767},
      {"compound_uchar_times_double", R"(
        int main(void) { unsigned char uc = 2; uc *= 3.0; return uc; }
    )", 6},
      {"redundant_store_live_load", R"(
        unsigned char v3 = 249;
        int main(void) { return (v3 & 1u) > (unsigned char)v3; }
    )", 0},
      {"case_label_wide", R"(
        int f(int x) { switch (x) { case 44: return 1; case 300: return 2;
                                    default: return 0; } }
        int g(long x) { switch (x) { case 70000L: return 4;
                                     default: return 0; } }
        int main(void) { return f(300) + g(70000L); }
    )", 6},
      {"case_label_expressions", R"(
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
    )", 1111},
      {"static_initializer_expressions", R"(
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
    )", 127},
      {"function_designator_decay", R"(
        int add(int a, int b) { return a + b; }
        int mul(int a, int b) { return a * b; }
        int apply(int (*f)(int, int), int a, int b) { return f(a, b); }
        int (*g)(int, int) = add;
        int main(void) {
            int (*h)(int, int) = mul;
            return apply(add, 3, 4) - apply(h, 1, 2) + g(1, 2) + (h == mul);
        }
    )", 9},
      {"register_param_read_by_ora", R"(
        unsigned int g1 = 3;
        signed char f3(register int p0) { return (-g1) | p0; }
        int main(void) { return f3(246) + 1000; }
    )", 999},
      {"function_returning_function_pointer", R"(
        int add(int, int);
        int mul(int, int);
        int add(int a, int b) { return a + b; }
        int mul(int a, int b) { return a * b; }
        int (*pick(int which))(int, int) { return which ? mul : add; }
        int main(void) { int (*k)(int, int) = pick(1); return k(2, 3); }
    )", 6},
      {"register_param_across_call", R"(
        int f3(void) { return 0; }
        int f0(register unsigned char p0, register unsigned char p1) {
            f3();
            return p1 << 7;
        }
        int main(void) { return f0(2, 249); }
    )", 31872},
      {"register_params_both_used", R"(
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
    )", 191},
      {"zp_abi_caller_of_recursive", R"(
        int rec(int d) { if (d <= 0) return 0; return rec(d - 1) + 1; }
        int f0(int p0) { int l = rec(3); return p0 + l; }
        int main(void) { return f0(4); }
    )", 7},
      {"value_live_across_call_into_later_block", R"(
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
    )", 1084},
      {"truncate_of_extended_global", R"(
        signed char g = 10;
        int f(signed char x) { return x; }
        int main(void) { long l2 = (int)g; g += 1; return f((signed char)l2); }
    )", 10},
      {"narrowed_binary_with_other_reader", R"(
        long g1 = 39;
        int f4(void) { return 0; }
        void f3(void) {
            unsigned long l0 = 3;
            l0 ^= f4();
            l0 += (int)l0;
            g1 += l0;
        }
        int main(void) { f3(); return (int)g1; }
    )", 45},
      {"do_while_exit_phi_after_header_phi", R"(
        int main(void) {
            int i = 0; int c = 0;
            do { c = i; i = i + 1; } while (i <= 10);
            return c;
        }
    )", 10},
      {"register_param_with_frame", R"(
        char sum_n(register char n) {
            register char acc = 0;
            if (n > 0) do {
                acc = (char)(acc + n);
                n = (char)(n - 1);
            } while (n > 0);
            return acc;
        }
        int main(void) { return sum_n(5); }
    )", 15},
      {"array_size_expression", R"(
        int g[sizeof(long) * 2];
        int main(void) { int a[2 + 1]; a[2] = 5;
                         return a[2] + (int)sizeof(a) + (int)sizeof(g); }
    )", 27},
      {"adc_commute_across_constant_store", R"(
        unsigned char total = 10, other, s = 2;
        unsigned char sample(void) { return s; }
        void accumulate(void) {
            unsigned char v = sample();
            other = 7;
            total += v;
        }
        int main(void) { accumulate(); return total * 100 + other; }
    )", 1207},
      {"and_sign_bit_branch_target_reads_a", R"(
        unsigned char flags = 5, hits;
        unsigned char top_bit(void) {
            unsigned char t = flags & 0x80;
            if (t) { hits = 1; return 7; }
            return t;
        }
        int main(void) { return top_bit(); }
    )", 0},
      {"dec_inc_branch_fold_target_reads_a", R"(
        unsigned char timer = 5, hits;
        unsigned char tick(void) {
            unsigned char t = --timer;
            if (t == 0) { hits = 1; return 7; }
            return t;
        }
        int main(void) { return tick(); }
    )", 4},
      {"licm_store_through_pointer", R"(
        unsigned char g, out;
        void f(unsigned char *p, unsigned char n) {
            unsigned char i = 0;
            do { g = 5; out = g; *p = 7; i++; } while (i < n);
        }
        int main(void) { f(&g, 2); return out; }
    )", 5},
      {"zp_abi_address_in_static_initializer", R"(
        unsigned char add3(unsigned char a, unsigned char b,
                           unsigned char c) { return a + b + c; }
        unsigned char (*fp)(unsigned char, unsigned char, unsigned char)
            = add3;
        int main(void) { return fp(1, 2, 3); }
    )", 6},
      {"zp_abi_address_taken_in_own_body", R"(
        unsigned char (*fp)(unsigned char, unsigned char, unsigned char);
        unsigned char add3(unsigned char a, unsigned char b,
                           unsigned char c) {
            fp = add3;
            return a + b + c;
        }
        int main(void) { add3(0, 0, 0); return fp(1, 2, 3); }
    )", 6},
      {"x_load_from_address_taken_frame_local", R"(
        struct ev { int n; };
        void inc(struct ev *e) { e->n++; }
        int main(void) { struct ev e; void (*c)(struct ev *) = inc;
                         e.n = 1; c(&e); return e.n; }
    )", 2},
      {"narrowed_read_of_global_after_call", R"(
        unsigned char g = 10;
        unsigned char h(void) { g = 77; return 1; }
        int main(void) {
            int t = g + 1;
            unsigned char k = h();
            unsigned char x = (unsigned char)t;
            return x + k;
        }
    )", 12},
      {"narrowed_extended_constant", R"(
        int check(int v3) { return (signed char)(((signed char)44) | v3); }
        int main(void) { return check(0); }
    )", 44},
      {"narrowed_temporary_of_refused_rewrite", R"(
        unsigned char g4 [[c6502::zeropage]] = 2;
        unsigned char g1 [[c6502::zeropage]] = 7;
        unsigned long long l16 = 0x1236;
        int main(void) {
            g4 = (unsigned int)(l16 | l16);
            return g1 + g4;
        }
    )", 61},
      {"global_byte_as_loop_counter", R"(
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
    )", -99},
      {"loop_counter_in_y_across_indirect_access", R"(
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
    )", 55},
      {"bool_of_constant_into_array", R"(
        bool a[3];
        int main(void) {
            a[1] = 1;
            a[0] = (bool)(59071U * 65430U);
            return a[0] * 4 + a[1] * 2 + a[2];
        }
    )", 6},
      {"narrowed_twice", R"(
        int check(unsigned int v2, unsigned long v4) {
            return (unsigned char)((unsigned int)(v4 * v2));
        }
        int main(void) { return check(65535u, 15UL); }
    )", 241},
      {"bit_tested_before_a_shift", R"(
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
    )", 5},
      {"post_decrement_in_for_test", R"(
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
    )", 3130},
      {"side_effect_in_countdown_test", R"(
        int n;
        int main(void) {
            int c = 0;
            for (int i = 3; (n++, i) >= 0; i--) c++;
            return n * 10 + c;
        }
    )", 54},
  };
  for (const Program& p : programs) {
    INFO(p.name);
    CHECK(tac_int(p.source) == p.expected);
  }
}

TEST_CASE("sim_miscompile_regressions: the address of an extern function is its label") {
  // `fp = f;` for an `extern` f stored the address of a soft-stack
  // slot, because GetAddress lowered every non-static name as a local,
  // and f kept zp_abi although a call through `fp` passes its
  // arguments on the soft stack.
  const std::string source =
      R"(unsigned char add3(unsigned char a, unsigned char b,
                   unsigned char c);
unsigned char (*fp)(unsigned char, unsigned char,
                    unsigned char);
unsigned char main(void) {
    fp = add3;
    return add3(1, 2, 3) + fp(1, 2, 3);
}
)";
  for (const bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    const std::string text = suite::codegen(source, optimize);
    CHECK(suite::contains(text, "LDA   #<add3"));
    CHECK(suite::contains(text, "LDA   #>add3"));
    CHECK_FALSE(suite::contains(text, "__zpabi_add3"));
  }
}
