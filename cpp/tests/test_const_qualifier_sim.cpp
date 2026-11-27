// Programs that use the `const` and `volatile` qualifiers (C99 §6.7.3),
// run in the simulator, from tests/test_const_qualifier.py.
#include <cstdint>
#include <string>

#include "doctest.h"
#include "sim_case.h"

namespace c6502::suite {

namespace {

// Checks that `main` returns `want`, read as signed, in both pipelines.
void check_returns(const std::string& src, std::int64_t want) {
  for (const bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    CHECK(build_sim(src, optimize).run().return_int_signed() == want);
  }
}

}  // namespace

// `(int *)p` where `p` is a `const int *` discards the qualifier, and the
// result is modifiable. C99 makes a write through it undefined when the
// object itself is const, but only modifying a const lvalue as written
// is rejected.
TEST_CASE("const_qualifier sim: a write through a pointer cast from a pointer to const") {
  CHECK(run_c_program(R"c(int main(void) {
    int x = 5;
    const int *p = &x;
    *(int *)p = 10;
    return x;
}
)c").return_int_signed() == 10);
}

TEST_CASE("const_qualifier sim: a cast of a const value") {
  // The cast strips the qualifier from the rvalue.
  CHECK(run_c_program(R"c(const int x = 5;
int main(void) { return (int)x + 1; }
)c").return_int_signed() == 6);
}

// Arithmetic works on the unqualified types (C99 §6.3.2.1p2).
TEST_CASE("const_qualifier sim: the sum of two const ints") {
  CHECK(run_c_program(R"c(const int a = 3;
const int b = 4;
int main(void) { return a + b; }
)c").return_int_signed() == 7);
}

TEST_CASE("const_qualifier sim: a const int plus an int") {
  CHECK(run_c_program(R"c(const int a = 100;
int main(void) { int b = 23; return a + b; }
)c").return_int_signed() == 123);
}

TEST_CASE("const_qualifier sim: a const lookup table") {
  CHECK(run_c_program(R"c(const int squares[5] = {0, 1, 4, 9, 16};
int main(void) { return squares[3]; }
)c").return_int_signed() == 9);
}

TEST_CASE("const_qualifier sim: a const parameter") {
  CHECK(run_c_program(R"c(int square(const int x) { return x * x; }
int main(void) { return square(7); }
)c").return_int_signed() == 49);
}

TEST_CASE("const_qualifier sim: a pointer-to-const parameter") {
  CHECK(run_c_program(R"c(int sum3(const int *p) { return p[0] + p[1] + p[2]; }
int main(void) {
    int arr[3] = {10, 20, 30};
    return sum3(arr);
}
)c").return_int_signed() == 60);
}

TEST_CASE("const_qualifier sim: arithmetic on a const pointer static writes both address bytes") {
  // A `T * const` static is the base of a 16-bit pointer + integer add.
  // tac_to_asm once read the static's type without stripping `Const`,
  // took it for 1 byte, and added only the low byte, leaving the
  // address's high byte unset. Index 300 needs both bytes, so the store
  // lands at $4000 + 300 only when the high byte is written.
  const char* src = R"c(static unsigned char * const buf = (unsigned char * const)0x4000;
int main(void) {
    buf[300] = 0x42;
    return 0;
}
)c";
  for (const bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    const sim::SimResult r = build_sim(src, optimize).run();
    CHECK(r.memory[0x4000 + 300] == 0x42);
  }
}

TEST_CASE("const_qualifier sim: a volatile object can be modified") {
  // Only `const` rejects writes, and the program reads back the stored
  // value.
  CHECK(run_c_program(R"c(int main(void) {
    volatile int x = 5;
    x = 10;
    return x;
}
)c").return_int_signed() == 10);
}

// Each of these programs reaches a struct, an array, or a pointer through
// a `const` or `volatile` qualifier. Each was rejected or crashed the
// compiler before the type checker and `c99_to_tac` looked through the
// qualifiers.
TEST_CASE("const_qualifier sim: an array member through a pointer to a const struct") {
  check_returns(R"c(struct s { unsigned char a[3]; };
static unsigned char f(const struct s *p) { return p->a[1]; }
int main(void) { struct s v = {{4, 5, 6}}; return f(&v); }
)c", 5);
}

TEST_CASE("const_qualifier sim: -> through a const pointer") {
  check_returns(R"c(struct s { unsigned char a; unsigned char b[3]; };
static void f(struct s *q) {
    struct s *const p = q; p->a = 5; p->b[1] = 7;
}
int main(void) {
    struct s v = {0, {0, 0, 0}}; f(&v); return v.a * 10 + v.b[1];
}
)c", 57);
}

TEST_CASE("const_qualifier sim: a member of a const struct") {
  check_returns(R"c(struct s { unsigned char x; unsigned char a; };
static unsigned char f(const struct s *p) { return p->a; }
int main(void) { struct s v = {1, 9}; return f(&v); }
)c", 9);
}

TEST_CASE("const_qualifier sim: comparisons of pointers that differ in their qualifiers") {
  check_returns(R"c(unsigned char buf[4];
static int count(const unsigned char *p) {
    int n = 0; while (p != buf + 4) { n++; p++; } return n;
}
static int before(const unsigned char *p) { return p < buf + 4; }
static const unsigned char *pick(int c, const unsigned char *p) {
    return c ? p : buf;
}
int main(void) {
    return count(buf + 1) * 100 + before(buf + 2) * 10
        + (pick(0, buf + 3) == buf);
}
)c", 311);
}

TEST_CASE("const_qualifier sim: address constants into arrays and members") {
  check_returns(R"c(const unsigned char A[3][4] = {{0, 1, 2, 3}, {10, 11, 12, 13}, {20, 21, 22, 23}};
struct m { unsigned char x; unsigned char m[4]; };
struct m S = {1, {30, 31, 32, 33}};
static const unsigned char *const t[] = { A[2], &A[1][3], A[1] + 2, &S.m[1], S.m + 3 };
int main(void) {
    return t[0][1] + t[1][0] + *t[2] + *t[3] + *t[4];
}
)c", 21 + 13 + 12 + 31 + 33);
}

TEST_CASE("const_qualifier sim: const struct objects") {
  check_returns(R"c(struct s { unsigned char a; int b; };
const struct s ca[2] = {{1, 200}, {3, 400}};
int main(void) {
    const struct s la = {7, 1000};
    struct s c = ca[1];
    return ca[0].b + la.b + c.b + la.a;
}
)c", 200 + 1000 + 400 + 7);
}

TEST_CASE("const_qualifier sim: a const struct whose tag is declared in a block") {
  check_returns(R"c(int main(void) {
    struct t { int a; int b; };
    const struct t x = {300, 4};
    const struct t *p = &x;
    return p->a + x.b;
}
)c", 304);
}

TEST_CASE("const_qualifier sim: a volatile pointer steps by the element's size") {
  check_returns(R"c(int arr[4] = {10, 20, 30, 40};
int main(void) { int *volatile p = arr; p++; p += 2; return *p; }
)c", 40);
}

}  // namespace c6502::suite
