// Programs that run the shift of a value with one significant byte, the
// closed form of the loop in Zork's `please_insert_save_diskette`, and
// `==` / `!=` comparing only the significant bytes, in both pipelines,
// from tests/test_significant_bytes.py.
#include <cstdint>
#include <string>
#include <vector>

#include "doctest.h"
#include "sim/cpu_run.h"
#include "sim_case.h"

namespace c6502::suite {

namespace {

std::string join_lines(const std::vector<std::string>& lines) {
  std::string out;
  for (std::size_t i = 0; i < lines.size(); ++i) {
    if (i) out += "\n";
    out += lines[i];
  }
  return out;
}

// Each `(c_type)x << k` for `k` in `counts`, checked for every byte `x`
// against adding `1 << k` (found by doubling) for each step of `x`.
// `main` returns 0, or 1000 * (index of the count + 1) + x at the first
// difference. Each count has a function of its own, which keeps the
// unoptimized frames small.
std::string shift_program(const std::string& c_type, const std::vector<int>& counts) {
  std::vector<std::string> lines = {
      "static " + c_type + " ref(" + c_type + " e, unsigned char k) {",
      "    while (k--) e = e + e;",
      "    return e;",
      "}",
  };
  for (std::size_t i = 0; i < counts.size(); ++i) {
    const std::string k = std::to_string(counts[i]);
    lines.push_back("static " + c_type + " s" + k + "(unsigned char x) { return (" + c_type + ")x << " + k + "; }");
    lines.push_back("static int check" + k + "(void) {");
    lines.push_back("    unsigned char x = 0;");
    lines.push_back("    " + c_type + " want = 0, step = ref(1, " + k + ");");
    lines.push_back("    do {");
    lines.push_back("        if (s" + k + "(x) != want) return " + std::to_string(1000 * (i + 1)) + " + x;");
    lines.push_back("        want = want + step;");
    lines.push_back("    } while (++x != 0);");
    lines.push_back("    return 0;");
    lines.push_back("}");
  }
  lines.push_back("int main(void) {");
  lines.push_back("    int r;");
  for (const int k : counts) {
    lines.push_back("    if ((r = check" + std::to_string(k) + "()) != 0) return r;");
  }
  lines.push_back("    return 0;");
  lines.push_back("}");
  return join_lines(lines);
}

// `(uint8_t)(w >> k)` for each `k` in `counts`, against the shift by a
// variable count.
std::string truncated_shift_program(const std::string& c_type, const std::vector<int>& counts) {
  std::vector<std::string> lines = {
      "static unsigned char ref(" + c_type + " w, unsigned char k) { return (unsigned char)(w >> k); }",
  };
  for (std::size_t i = 0; i < counts.size(); ++i) {
    const std::string k = std::to_string(counts[i]);
    lines.push_back("static unsigned char s" + k + "(" + c_type + " w) { return (unsigned char)(w >> " + k + "); }");
    lines.push_back("static int check" + k + "(void) {");
    lines.push_back("    unsigned char i = 0;");
    lines.push_back("    do {");
    lines.push_back("        " + c_type + " w = (" + c_type + ")i * 0x01010101UL ^ 0x5AC3E10FUL;");
    lines.push_back("        if (s" + k + "(w) != ref(w, " + k + ")) return " + std::to_string(1000 * (i + 1)) + " + i;");
    lines.push_back("    } while (++i != 0);");
    lines.push_back("    return 0;");
    lines.push_back("}");
  }
  lines.push_back("int main(void) {");
  lines.push_back("    int r;");
  for (const int k : counts) {
    lines.push_back("    if ((r = check" + std::to_string(k) + "()) != 0) return r;");
  }
  lines.push_back("    return 0;");
  lines.push_back("}");
  return join_lines(lines);
}

std::vector<int> counts_off_bytes(int lo, int hi) {
  std::vector<int> out;
  for (int k = lo; k < hi; ++k) {
    if (k % 8) out.push_back(k);
  }
  return out;
}

void check_returns_zero(const std::string& src) {
  for (const bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    const sim::SimResult r = build_sim(src, optimize).run(50'000'000);
    CHECK_FALSE(r.timed_out);
    CHECK(r.return_int() == 0);
  }
}

}  // namespace

// The CRC-32s are of the programs the Python generators wrote on
// 27 Nov 2026.
TEST_CASE("significant_bytes sim: a widened byte shifted by every 16-bit count") {
  const std::string src = shift_program("unsigned int", counts_off_bytes(1, 16));
  CHECK(sim::cpu_run::crc32(src) == 0x053F0F67u);
  check_returns_zero(src);
}

TEST_CASE("significant_bytes sim: a widened byte shifted by every 32-bit count") {
  const std::string src = shift_program("unsigned long", counts_off_bytes(1, 32));
  CHECK(sim::cpu_run::crc32(src) == 0xED6ABBF6u);
  check_returns_zero(src);
}

TEST_CASE("significant_bytes sim: shifts through & and >> of one significant byte") {
  // `w & 0x3F` and `w >> 8` have one significant byte.
  check_returns_zero(R"c(
        static unsigned int ref(unsigned int e, unsigned char k) {
            while (k--) e = e + e;
            return e;
        }
        static unsigned int masked(unsigned int w) { return (w & 0x3F) << 6; }
        static unsigned int top(unsigned int w) { return (w >> 8) << 5; }
        static int sgn(signed char c) { return (int)c << 5; }
        int main(void) {
            unsigned char i = 0;
            do {
                unsigned int w = ((unsigned int)i << 8)
                    | (unsigned char)(i * 7 + 3);
                if (masked(w) != ref(w & 0x3F, 6)) return 1;
                if (top(w) != ref(w >> 8, 5)) return 2;
            } while (++i != 0);
            if (sgn(-3) != -96) return 3;
            return 0;
        }
        )c");
}

TEST_CASE("significant_bytes sim: the closed form of the SCRATCH1 loop agrees with the loop") {
  // `please_insert_save_diskette` sets SCRATCH1 to one less than 64
  // times the position. The loop it was written with and the closed
  // form agree for every byte the answer could hold.
  const char* src = R"c(
        #include <stdint.h>
        uint16_t S;
        uint8_t answer;
        static void by_loop(void) {
            S = 0xFFFF;
            for (uint8_t n = answer & 0x07; n != 0; n--)
                S += 64;
        }
        static void closed(void) {
            S = (64 * (answer & 0x07)) - 1;
        }
        int main(void) {
            uint8_t a = 0;
            do {
                answer = a;
                by_loop();
                uint16_t want = S;
                S = 0x1234;
                closed();
                if (S != want) return 1000 + a;
            } while (++a != 0);
            return 0;
        }
        )c";
  for (const bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    const sim::SimResult r = build_sim(src, optimize).run();
    CHECK_FALSE(r.timed_out);
    CHECK(r.return_int() == 0);
  }
}

// `(uint8_t)(w >> k)` computes only the low byte, from two bytes of `w`,
// for every count that needs two.
TEST_CASE("significant_bytes sim: a truncated right shift of an unsigned int") {
  const std::string src = truncated_shift_program("unsigned int", {1, 2, 3, 4, 5, 6, 7});
  CHECK(sim::cpu_run::crc32(src) == 0xDB91D37Fu);
  check_returns_zero(src);
}

TEST_CASE("significant_bytes sim: a truncated right shift of an int") {
  const std::string src = truncated_shift_program("int", {1, 2, 3, 4, 5, 6, 7});
  CHECK(sim::cpu_run::crc32(src) == 0xF0607368u);
  check_returns_zero(src);
}

TEST_CASE("significant_bytes sim: a truncated right shift of an unsigned long") {
  const std::string src = truncated_shift_program("unsigned long", counts_off_bytes(1, 24));
  CHECK(sim::cpu_run::crc32(src) == 0xE2A8B78Au);
  check_returns_zero(src);
}

TEST_CASE("significant_bytes sim: == and != compare only the significant bytes") {
  // `==` and `!=` compare only the low bytes either operand can have
  // nonzero, as a value and as a branch, checked over every byte.
  const char* src = R"c(
    #include <stdbool.h>
    unsigned char k = 3;
    int main(void) {
        unsigned int sum = 0;
        unsigned char x = 0;
        do {
            bool a = (x & 0x80) != 0;
            bool b = ((x & 0x0F) | 0x100) == 0x103;
            bool c = (x & 0x7F) == k;
            bool d = (unsigned)x + 0x100 != 0x1FF;
            if ((x & 0x3F) == 0) sum += 1;
            if (((unsigned)x | 0x200) == 0x2C0) sum += 7;
            sum += a + 2 * b + 4 * c + 8 * d;
        } while (++x != 0);
        return sum;
    }
    )c";
  int want = 0;
  for (int x = 0; x < 256; ++x) {
    want += (x & 0x3F) == 0;
    want += 7 * ((x | 0x200) == 0x2C0);
    want += ((x & 0x80) != 0) + 2 * (((x & 0x0F) | 0x100) == 0x103);
    want += 4 * ((x & 0x7F) == 3) + 8 * (x + 0x100 != 0x1FF);
  }
  for (const bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    const sim::SimResult r = build_sim(src, optimize).run(50'000'000);
    CHECK_FALSE(r.timed_out);
    CHECK((r.return_int() & 0xFFFF) == want);
  }
}

}  // namespace c6502::suite
