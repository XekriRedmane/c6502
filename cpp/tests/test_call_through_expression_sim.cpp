// Calls through any expression that gives a function or a function
// pointer (C99 §6.5.2.2), `pick()(3)`, `t[i](3)`, `s.g(3)`, `p->g(3)`,
// and `(*pp)(3)`, run in the simulator in both pipelines, from
// tests/test_call_through_expression.py.
#include <string>

#include "doctest.h"
#include "sim_case.h"

namespace c6502::suite {

namespace {

void check_runs(const std::string& src, std::int64_t expected) {
  for (const bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    CHECK(build_sim(src, optimize).run().return_int() == expected);
  }
}

}  // namespace

TEST_CASE("call_through_expression sim: the arguments are converted") {
  // The `long` argument converts to the `unsigned char` parameter.
  check_runs(R"c(
            unsigned char low(unsigned char x) { return x; }
            unsigned char (*t[1])(unsigned char) = { low };
            int main(void) { long v = 0x1234; return t[0](v); }
        )c", 0x34);
}

TEST_CASE("call_through_expression sim: every shape of callee") {
  check_runs(R"c(
        struct S { int (*g)(int); };
        int dbl(int x) { return 2 * x; }
        int inc(int x) { return x + 1; }
        int (*t[2])(int) = { dbl, inc };
        int (*pick(int i))(int) { return t[i]; }
        struct S s = { inc };
        int (*fp)(int) = dbl;
        int main(void) {
            struct S *p = &s;
            int (**pp)(int) = &fp;
            int r = 0;
            r += pick(0)(3);                          /* 6 */
            r += (pick(1))(3);                        /* 4 */
            r += t[0](5);                             /* 10 */
            r += t[1](5) * 100;                       /* 600 */
            r += s.g(7);                              /* 8 */
            r += p->g(9);                             /* 10 */
            r += (*pp)(1);                            /* 2 */
            r += (**fp)(2) + (*dbl)(3) + (&inc)(4);   /* 4 + 6 + 5 */
            return r;
        }
    )c", 655);
}

TEST_CASE("call_through_expression sim: a dispatch table in a loop") {
  // A const table indexed by a variable, the usual dispatch shape.
  // ((0 + 1) * 2 + 2) * 2 = 8, and (8 + 3) * 2 = 22.
  check_runs(R"c(
            int add1(int x) { return x + 1; }
            int add2(int x) { return x + 2; }
            int add3(int x) { return x + 3; }
            static int (*const ops[3])(int) = { add1, add2, add3 };
            int main(void) {
                int v = 0;
                for (unsigned char i = 0; i < 3; i++) v = ops[i](v) * 2;
                return v;
            }
        )c", 22);
}

TEST_CASE("call_through_expression sim: a struct return and a void call") {
  check_runs(R"c(
            struct P { int a, b; };
            struct P mk(int x) { struct P p = { x, x + 1 }; return p; }
            int g;
            void set(int x) { g = x; }
            struct V {
                struct P (*make)(int);
                void (*store)(int);
            } v = { mk, set };
            int main(void) {
                struct V *pv = &v;
                pv->store(40);
                return pv->make(1).b + g;
            }
        )c", 42);
}

TEST_CASE("call_through_expression sim: the callee and the arguments are each evaluated once") {
  // The callee and the argument are unsequenced, so `a` is id(0),
  // id(10), or the same through `neg`. c6502 evaluates the callee first,
  // so `n` is 1 when `n * 10` runs, and a = id(10) = 10.
  check_runs(R"c(
            int n;
            int id(int x) { return x; }
            int neg(int x) { return -x; }
            int (*t[2])(int) = { id, neg };
            int (*next(void))(int) { return t[n++]; }
            int main(void) {
                int a = next()(n * 10);    /* id or neg of 0 or 10 */
                int b = next()(5);         /* neg(5) */
                return a * 100 + b + n;    /* n is 2 */
            }
        )c", 10 * 100 - 5 + 2);
}

}  // namespace c6502::suite
