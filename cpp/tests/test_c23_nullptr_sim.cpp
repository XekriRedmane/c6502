#include "doctest.h"
#include "sim_case.h"

namespace c6502::suite {

TEST_CASE("c23_nullptr sim: the program runs to its result") {
  const char* src = R"c(#include <stddef.h>

struct node { int value; struct node *next; };

static struct node c = { 3, nullptr };
static struct node b = { 2, &c };
static struct node a = { 1, &b };
static int *gp = nullptr;
static nullptr_t gn;
static bool gb = nullptr;
int g;

int length(struct node *n) {
    int k = 0;
    for (; n != nullptr; n = n->next)
        k++;
    return k;
}

int *pick(int which) { return which ? &g : nullptr; }

nullptr_t none(void) { return nullptr; }

int is_null(void *p) { return p == nullptr; }

int main(void) {
    int *p = nullptr;
    nullptr_t n = 0;
    int r = 0;
    if (nullptr == 0 && 0 == nullptr) r += 1;
    if (nullptr == NULL && NULL == 0 && nullptr == (void *)0) r += 2;
    if (p == nullptr && !(nullptr != p)) r += 4;
    if (!nullptr && !(bool)nullptr && !n) r += 8;
    if (is_null(nullptr) && is_null(NULL) && !is_null(&g)) r += 16;
    if (pick(0) == nullptr && pick(1) == &g) r += 32;
    if (gp == nullptr && !gb && gn == nullptr && n == nullptr) r += 64;
    if (none() == nullptr && !none()) r += 128;
    if (sizeof(nullptr) == 2 && sizeof(nullptr_t) == 2) r += 256;
    p = r & 1 ? &g : nullptr;
    if (p == &g) r += 512;
    r += 1000 * length(&a) + 10000 * (int)offsetof(struct node, next);
    return r;
}
)c";
  // 1 + 2 + 4 + 8 + 16 + 32 + 64 + 128 + 256 + 512, three nodes, and
  // `next` 2 bytes into a node.
  const std::int64_t expected = 1023 + 3000 + 20000;
  for (const bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    const sim::SimResult r = build_sim(src, optimize).run(2'000'000);
    CHECK_FALSE(r.timed_out);
    CHECK(r.return_int_signed() == expected);
  }
}

}  // namespace c6502::suite
