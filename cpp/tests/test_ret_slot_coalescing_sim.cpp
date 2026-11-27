// A program whose local pair holding a copy of `F@ret` becomes `F@ret`
// (src/passes/ret_slot_coalescing.h), run in both pipelines, from
// tests/test_ret_slot_coalescing.py.
#include <vector>

#include "doctest.h"
#include "sim_case.h"

namespace c6502::suite {

namespace {

// What the program returns: its walks over `text`, done on the host.
std::int64_t expected() {
  std::int64_t total = 0;
  const auto add = [&total](int c) { total = (total * 3 + c) & 0xFFFF; };
  const std::vector<int> text = {2, 'a', 'b', 1, 'c', 0, 3, 'd', 'e', 'f', 0};
  for (const int start : {0, 6}) {
    int p = start;
    while (text[p] != 0) {
      add(text[p]);
      add(text[p + 1]);
      p = p + text[p] + 1;
      if (text[p] == 0) add(0xFF);
    }
  }
  return total;
}

}  // namespace

TEST_CASE("ret_slot_coalescing sim: the walks compute their total") {
  const char* src = R"c(
#include <stdint.h>
static const uint8_t text[] = {2, 'a', 'b', 1, 'c', 0, 3, 'd', 'e', 'f', 0};
uint16_t total;
static const uint8_t *next(const uint8_t *s) { return s + s[0] + 1; }
static void add(uint8_t c) { total = total * 3 + c; }
static const uint8_t *group(uint8_t index) {
    const uint8_t *p = text;
    for (; index != 0; index--) {
        while (*p != 0)
            p = next(p);
        p++;
    }
    return p;
}
static void walk(const uint8_t *p) {
    while (*p != 0) {
        add(*p);
        add(p[1]);
        p = next(p);
        if (*p == 0)
            add(0xFF);
    }
}
uint16_t main(void) {
    walk(group(0));
    walk(group(1));
    return total;
}
)c";
  for (const bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    CHECK(build_sim(src, optimize).run().return_int() == expected());
  }
}

}  // namespace c6502::suite
