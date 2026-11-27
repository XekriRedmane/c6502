// Members of static objects computed in their own bytes
// (src/passes/in_place_member_update.h), run in both pipelines, from
// tests/test_in_place_member_update.py. Each program returns a value
// that a wrong rewrite changes.
#include <string>

#include "doctest.h"
#include "sim_case.h"

namespace c6502::suite {

namespace {

// Runs the declarations of `_PC` followed by `body` in both pipelines
// and checks what `main` returns.
void check(const char* body, std::int64_t expected) {
  const std::string src = std::string(R"c(
#include <stdint.h>
typedef struct { uint16_t lo; uint8_t hi; } uint24_t;
uint24_t P;
uint16_t W;
uint8_t flag;
uint16_t t[4];
)c") + body;
  for (const bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    CHECK(build_sim(src, optimize).run().return_int() == expected);
  }
}

}  // namespace

TEST_CASE("in_place_member_update sim: an increment carries into the high byte") {
  check("int main(void) { P.lo = 0x00FF; P.lo++; return P.lo; }", 0x0100);
}

TEST_CASE("in_place_member_update sim: a test of the low byte after an increment") {
  check(R"c(uint8_t step(uint16_t v) { P.lo = v; flag = 0; P.lo++;
    if ((uint8_t)P.lo == 0) { flag = 1;
        if (P.lo == 0) P.hi++; }
    return flag; }
int main(void) { P.hi = 7; int n = 0;
    n += step(0x00FF); n += 2 * step(0x1234);
    n += 4 * step(0xFFFF); n += 8 * step(0x0000);
    return n * 16 + P.hi; })c", (1 + 4) * 16 + 8);
}

TEST_CASE("in_place_member_update sim: a loaded value read after the store") {
  // `old` is read after the store, so the member can't stand for it.
  check(R"c(uint16_t bump(void) { uint16_t old = P.lo; P.lo = old + 1; return old; }
int main(void) { P.lo = 41; uint16_t a = bump(); return a * 100 + P.lo; })c", 41 * 100 + 42);
}

TEST_CASE("in_place_member_update sim: a store through a pointer between the load and the add") {
  check(R"c(uint16_t *q;
void bump(void) { uint16_t v = P.lo; *q = 500; P.lo = v + 1; }
int main(void) { q = &P.lo; P.lo = 10; bump(); return P.lo; })c", 11);
}

TEST_CASE("in_place_member_update sim: a value that reads the member it replaces") {
  // Writing the low byte first would change the high byte's source.
  check(R"c(void swap(void) { P.lo = (P.lo >> 8) | (P.lo << 8); }
int main(void) { P.lo = 0x1234; swap(); return P.lo; })c", 0x3412);
}

TEST_CASE("in_place_member_update sim: a value that reads another member") {
  check(R"c(void f(void) { P.lo = P.hi + 0x0102; }
int main(void) { P.hi = 3; f(); return P.lo; })c", 0x0105);
}

TEST_CASE("in_place_member_update sim: a subtract borrows") {
  check("int main(void) { t[1] = 0x0100; t[1] -= 1; return t[1]; }", 0x00FF);
}

}  // namespace c6502::suite
