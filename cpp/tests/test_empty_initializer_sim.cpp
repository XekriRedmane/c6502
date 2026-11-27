#include <string>
#include <tuple>
#include <vector>

#include "doctest.h"
#include "sim_case.h"

namespace c6502::suite {

namespace {

// The bytes of `struct s` are a (2), b (3), and c (4).
const std::string kProgram = R"c(struct s { int a; char b[3]; long c; };
union u { char c; long l; double d; };
struct n { struct s s; union u u; int k[2][3]; float f; };
typedef typeof_unqual(nullptr) nullptr_t;

static int failed;
static int checks;

/* Notes the number of the first check whose bytes differ from `want`
   (all zero when `want` is null). */
static void expect(const void *object, unsigned n,
                   const unsigned char *want) {
    const unsigned char *p = object;
    ++checks;
    for (unsigned i = 0; i < n; i++) {
        unsigned char w = want ? want[i] : 0;
        if (p[i] != w) {
            if (!failed)
                failed = checks;
            return;
        }
    }
}

#define ZERO(x) expect(&(x), sizeof(x), 0)

static const unsigned char s_braced[9] = { 3, 0, 0, 0, 0, 4, 0, 0, 0 };
static const unsigned char k_partial[12] = {
    0, 0, 0, 0, 0, 0, 1, 0, 2, 0, 0, 0 };
static const unsigned char five[2] = { 5, 0 };
static const unsigned char minus7[4] = { 0xF9, 0xFF, 0xFF, 0xFF };

/* Leaves 0xA5 in the bytes the next call's locals will take. */
static void dirty(void) {
    volatile unsigned char junk[110];
    for (int i = 0; i < 110; i++)
        junk[i] = 0xA5;
}

struct s g_s = {};
union u g_u = {};
struct n g_n = {};
int g_i = {};
int *g_p = {};
double g_d = {};
bool g_b = {};
_BitInt(12) g_w = {};
nullptr_t g_np = {};
int g_a[5] = {};
struct s g_braced = { {3}, {}, {4} };
int g_k[2][3] = { {}, {1, 2} };
int g_five = {5};
long g_minus7 = {-7,};
struct s *g_ps = &(struct s){};
int *g_pa = (int[4]){};

static void file_scope(void) {
    ZERO(g_s); ZERO(g_u); ZERO(g_n); ZERO(g_i); ZERO(g_p); ZERO(g_d);
    ZERO(g_b); ZERO(g_w); ZERO(g_np); ZERO(g_a);
    expect(&g_braced, 9, s_braced);
    expect(g_k, 12, k_partial);
    expect(&g_five, 2, five);
    expect(&g_minus7, 4, minus7);
    expect(g_ps, sizeof(struct s), 0);
    expect(g_pa, 4 * sizeof(int), 0);
}

static void block_statics(void) {
    static struct s s = {};
    static union u u = {};
    static struct n n = {};
    static int i = {};
    static int *p = {};
    static float f = {};
    static bool b = {};
    static _BitInt(12) w = {};
    static char c[7] = {};
    static struct s braced = { {3}, {}, {4} };
    static int five_ = {5};
    ZERO(s); ZERO(u); ZERO(n); ZERO(i); ZERO(p); ZERO(f); ZERO(b);
    ZERO(w); ZERO(c);
    expect(&braced, 9, s_braced);
    expect(&five_, 2, five);
}

static void automatic_aggregates(void) {
    struct s s = {};
    union u u = {};
    struct n n = {};
    int a[5] = {};
    char text[12] = {};
    struct s braced = { {3}, {}, {4} };
    int k[2][3] = { {}, {1, 2} };
    ZERO(s); ZERO(u); ZERO(n); ZERO(a); ZERO(text);
    expect(&braced, 9, s_braced);
    expect(k, 12, k_partial);
}

static void automatic_scalars(void) {
    int i = {};
    int *p = {};
    float f = {};
    double d = {};
    bool b = {};
    _BitInt(12) w = {};
    nullptr_t np = {};
    char c = {};
    long long ll = {};
    int five_ = {5};
    long m7 = {-7,};
    struct s *ps = &(struct s){};
    int *pa = (int[4]){};
    int ci = (int){};
    ZERO(i); ZERO(p); ZERO(f); ZERO(d); ZERO(b); ZERO(w); ZERO(np);
    ZERO(c); ZERO(ll);
    expect(&five_, 2, five);
    expect(&m7, 4, minus7);
    expect(ps, sizeof(struct s), 0);
    expect(pa, 4 * sizeof(int), 0);
    ZERO(ci);
    for (int j = {}; j < 2; j++) {
        union u again = {};
        ZERO(again);
        again.l = -1;
    }
}

int main(void) {
    file_scope();
    dirty();
    block_statics();
    dirty();
    automatic_aggregates();
    dirty();
    automatic_scalars();
    return failed ? failed : 1000 + checks;
}
)c";

// Every check passes: 16 at file scope, 11 for the block statics, 7 for
// the automatic aggregates, and 14 plus 2 passes of the loop for the
// automatic scalars.
constexpr int kExpected = 1000 + 16 + 11 + 7 + 16;

// An automatic object over 256 bytes, which the loop clears in two runs.
// The unoptimized pipeline can't address a frame that big.
const std::string kLarge = R"c(static void dirty(void) {
    volatile unsigned char junk[300];
    for (int i = 0; i < 300; i++)
        junk[i] = 0xA5;
}

static int check(void) {
    unsigned char big[300] = {};
    for (int i = 0; i < 300; i++)
        if (big[i])
            return i + 1;
    return 0;
}

int main(void) {
    dirty();
    return check();
}
)c";

std::string replace_all(std::string text, const std::string& old, const std::string& with) {
  for (std::size_t at = text.find(old); at != std::string::npos; at = text.find(old, at + with.size())) {
    text.replace(at, old.size(), with);
  }
  return text;
}

}  // namespace

TEST_CASE("empty_initializer sim: every object initialized by {} or a braced scalar holds its bytes") {
  for (const bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    const sim::SimResult r = build_sim(kProgram, optimize).run(4'000'000);
    CHECK_FALSE(r.timed_out);
    CHECK(r.return_int_signed() == kExpected);
  }
}

TEST_CASE("empty_initializer sim: the bytes dirty leaves reach the objects") {
  // Without `= {}`, an automatic object holds the bytes `dirty` left, so
  // the programs above catch a missing clear. A union initialized by
  // `{1}` has the same bytes past its first member, which `{}` zeroes.
  // Each change makes the check of that object the first to fail.
  const std::vector<std::tuple<std::string, std::string, int>> changes = {
      {R"c(    struct s s = {};
)c", R"c(    struct s s;
)c", 16 + 11 + 1},
      {R"c(    union u u = {};
)c", R"c(    union u u = {1};
)c", 16 + 11 + 2},
      {R"c(    int i = {};
)c", R"c(    int i;
)c", 16 + 11 + 7 + 1},
  };
  for (const auto& [old, with, check] : changes) {
    const std::string src = replace_all(kProgram, old, with);
    CHECK(src != kProgram);
    for (const bool optimize : {false, true}) {
      INFO("old=" << old << " optimize=" << optimize);
      CHECK(build_sim(src, optimize).run(4'000'000).return_int_signed() == check);
    }
  }
  const std::string src = replace_all(kLarge, " = {}", "");
  CHECK(build_sim(src, true).run(2'000'000).return_int_signed() == 1);
}

TEST_CASE("empty_initializer sim: an automatic object over 256 bytes is cleared") {
  const sim::SimResult r = build_sim(kLarge, true).run(2'000'000);
  CHECK_FALSE(r.timed_out);
  CHECK(r.return_int_signed() == 0);
}

}  // namespace c6502::suite
