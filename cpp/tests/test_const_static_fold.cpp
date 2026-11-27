// The tests of tests/test_const_static_fold.py that read the `-S` text:
// under `-O`, a `static` const scalar with a constant initializer whose
// address isn't taken is read as immediates of its bytes, and its
// storage goes. The simulator's runs are in
// test_const_static_fold_sim.cpp.
#include <stdexcept>
#include <string>

#include "compile_case.h"
#include "doctest.h"

namespace {

namespace suite = c6502::suite;

constexpr const char* kPut =
    "static unsigned char * const buf = (unsigned char * const)0x4123;\n"
    "void put(unsigned char x) { *buf = x; }\n"
    "int main(void) { put(0x42); return 0; }\n";

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

TEST_CASE("const_static_fold: a const pointer static folds to immediates of its address") {
  const std::string text = suite::codegen(kPut, true);
  CHECK_FALSE(suite::contains(text, "buf:"));
  CHECK_FALSE(suite::contains(text, "LDA   buf\n"));
  CHECK_FALSE(suite::contains(text, "LDA   buf+1"));
  CHECK(suite::contains(text, "#$23"));
  CHECK(suite::contains(text, "#$41"));
}

TEST_CASE("const_static_fold: without -O the static keeps its storage") {
  const std::string text = suite::codegen(kPut, false);
  CHECK(suite::contains(text, "buf:"));
  CHECK(suite::contains(text, "LDA   buf"));
}

TEST_CASE("const_static_fold: a static whose address is taken keeps its storage") {
  // Its address is needed at link time.
  const std::string text = suite::codegen(
      "static unsigned char * const buf = (unsigned char * const)0x4000;\n"
      "unsigned char ** ptr_holder;\n"
      "int main(void) { ptr_holder = &buf; return 0; }\n",
      true);
  CHECK(suite::contains(text, "buf:"));
}

TEST_CASE("const_static_fold: an array isn't folded") {
  const std::string text = suite::codegen(
      "static const unsigned char tbl[4] = {1, 2, 3, 4};\n"
      "int main(int i) { return tbl[i]; }\n",
      true);
  CHECK(suite::contains(text, "tbl:"));
}

TEST_CASE("const_static_fold: a const object with external linkage isn't folded") {
  // Another unit might refer to it.
  const std::string text = suite::codegen(
      "unsigned char * const buf = (unsigned char * const)0x4000;\n"
      "void put(unsigned char x) { *buf = x; }\n"
      "int main(void) { put(0x42); return 0; }\n",
      true);
  CHECK(suite::contains(text, "buf:"));
}

TEST_CASE("const_static_fold: a static that isn't const isn't folded") {
  // Nothing writes it here, but only the const qualifier admits the
  // fold, since no pass proves that nothing writes an object.
  const std::string text = suite::codegen(
      "static unsigned char * buf = (unsigned char *)0x4000;\n"
      "void put(unsigned char x) { *buf = x; }\n"
      "int main(void) { put(0x42); return 0; }\n",
      true);
  CHECK(suite::contains(text, "buf:"));
}

TEST_CASE("const_static_fold: a static const int folds") {
  const std::string text = suite::codegen(
      "static const int magic = 0x1234;\n"
      "int main(void) { return magic; }\n",
      true);
  CHECK_FALSE(suite::contains(text, "magic:"));
}

TEST_CASE("const_static_fold: a static const pointer is read as the address it holds") {
  // Both the read of B and the access through it had read the pointer
  // from memory.
  const std::string text =
      suite::codegen(R"(#include <stdint.h>
uint8_t buf[8] = {1, 2, 3, 4, 5, 6, 7, 8};
static const uint8_t *const B = buf + 3;
uint8_t at(uint8_t i) { return B[i]; }
const uint8_t *ptr(void) { return B; }
int main(void) {
    int bad = 0;
    if (at(0) != 4 || at(4) != 8) bad |= 1;
    if (ptr() != &buf[3]) bad |= 2;
    return bad;
}
)", true);
  const std::string at = between(text, "\nat:", "\nptr:");
  CHECK(suite::contains(at, "buf+3,Y"));
  CHECK_FALSE(suite::contains(at, "DPTR"));
  CHECK_FALSE(suite::contains(text, "\nB:"));
}
