// The `-S -O` text of tests/test_element_rmw.py's program, whose
// read-modify-writes of an element indexed by X are `INC t,X` and the
// rest. The runs and the simulator's assembler test stay in Python.
#include <stdexcept>
#include <string>
#include <vector>

#include "compile_case.h"
#include "doctest.h"

namespace {

namespace suite = c6502::suite;

// Returns `text[text.index(from):text.index(to)]`.
std::string between(const std::string& text, const std::string& from, const std::string& to) {
  const std::size_t start = text.find(from);
  const std::size_t end = text.find(to);
  if (start == std::string::npos || end == std::string::npos) {
    throw std::runtime_error("substring not found");
  }
  return end > start ? text.substr(start, end - start) : std::string();
}

}  // namespace

TEST_CASE("element_rmw: the optimized code uses the indexed forms") {
  const std::string text = suite::codegen(R"(
#include <stdint.h>
uint8_t a[16];
uint8_t out[24];
[[c6502::zeropage]] uint8_t flag;
void dec_unused(uint8_t i) { --a[i]; }
uint8_t dec_used(uint8_t i) { return --a[i]; }
void inc_unused(uint8_t i) { a[i]++; }
uint8_t inc_used(uint8_t i) { return ++a[i]; }
uint8_t post_dec(uint8_t i) { return a[i]--; }
uint8_t post_inc(uint8_t i) { return a[i]++; }
void shl(uint8_t i) { a[i] <<= 1; }
void shr(uint8_t i) { a[i] >>= 1; }
uint8_t shl_used(uint8_t i) { return a[i] <<= 1; }
uint8_t dec_branch(uint8_t i) { if (--a[i] == 0) return 7; return 9; }
uint8_t inc_branch(uint8_t i) { if (++a[i] == 0) return 7; return 9; }
uint8_t timer(uint8_t i) {
    if (a[i] == 0)
        return 1;
    uint8_t t = --a[i];
    if (t == 0) {
        flag = 6;
        return 2;
    }
    flag = 5;
    if (t == 20 || t == 10)
        return t;
    return 3;
}
void timers(uint8_t n) {
    for (uint8_t i = n; (int8_t)i >= 0; i--) {
        if (a[i] == 0)
            continue;
        uint8_t t = --a[i];
        if (t == 0) {
            flag = 1;
            continue;
        }
        flag = 2;
        if (t == 20 || t == 10)
            out[16 + i] = t;
    }
}
int main(void) {
    uint8_t k = 0;
    a[0] = 0;    dec_unused(0);
    a[1] = 1;    out[k++] = dec_used(1);
    a[2] = 255;  inc_unused(2);
    a[3] = 0x7F; out[k++] = inc_used(3);
    a[4] = 0;    out[k++] = post_dec(4);
    a[5] = 255;  out[k++] = post_inc(5);
    a[6] = 0x81; shl(6);
    a[7] = 0x81; shr(7);
    a[8] = 0xC0; out[k++] = shl_used(8);
    a[9] = 1;    out[k++] = dec_branch(9);
    out[k++] = dec_branch(9);
    a[10] = 255; out[k++] = inc_branch(10);
    out[k++] = inc_branch(10);
    a[11] = 0;   out[k++] = timer(11);
    a[12] = 1;   out[k++] = timer(12);
    a[13] = 21;  out[k++] = timer(13);
    a[14] = 11;  out[k++] = timer(14);
    a[15] = 6;   out[k++] = timer(15);
    out[k++] = flag;
    a[0] = 0; a[1] = 1; a[2] = 21; a[3] = 11; a[4] = 200;
    timers(4);
    out[k++] = flag;
    return k;
}
)", true);
  const std::vector<std::string> lines = suite::lines(text);
  for (const std::string op : {"DEC", "INC", "ASL", "LSR"}) {
    INFO(op);
    const std::string want = "   " + op + "   a,X";
    bool found = false;
    for (const std::string& line : lines) found = found || line == want;
    CHECK(found);
  }
  const std::string body = between(text, "timers:", "main:");
  CHECK_FALSE(suite::contains(body, "STA   a,X"));
  CHECK(suite::contains(body, "DEC   a,X"));
}
