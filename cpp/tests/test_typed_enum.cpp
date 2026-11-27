// tests/test_typed_enum.py's program of enumerations with a fixed
// underlying type on the TAC interpreter. It returns 0 when every check
// passes, and otherwise the number of the first check that fails. The
// simulator's runs are in test_typed_enum_sim.cpp.
#include <string>

#include "compile_case.h"
#include "doctest.h"

namespace suite = c6502::suite;

TEST_CASE("typed_enum: the program returns 0 on the TAC interpreter") {
  const auto value = suite::run_program(R"(
#include <stdint.h>

enum kind : uint8_t;
struct node { enum kind k; uint8_t v; };
int score(enum kind k);

enum kind : uint8_t { LEAF = 1, PAIR, LIST = 200, LAST };
enum big : unsigned int { BIG = 40000 };
enum F : uint8_t { M = 200 };
enum sc : signed char { NEG = -3, POS = 3 };
enum wide : long { MEGA = 1000000 };
enum flag : bool { OFF, ON };
enum hw : unsigned short { TOP = 65535 };

static const uint8_t weights[PAIR + 1] = { [LEAF] = 5, [PAIR] = 7 };
struct node nodes[3] = { { LEAF, 10 }, { PAIR, 20 }, { LIST, 30 } };
static enum kind last = LAST;
static int sum = M + M;

int score(enum kind k) {
    switch (k) {
    case LEAF: return 1;
    case PAIR: return 2;
    case LIST: return 4;
    default: return 8;
    }
}

enum kind next(enum kind k) { return k + 1; }

// Each part is a function of its own, so that the unoptimized
// pipeline's frames stay under 253 bytes.
int uses(void) {
    int r = 0;
    int below = 0;
    for (uint8_t i = 0; i < 3; i++) {
        r += score(nodes[i].k);
        if (nodes[i].k < LIST) below++;
    }
    if (r != 7) return 3;
    if (below != 2) return 4;
    if (weights[PAIR] != 7 || weights[LEAF] != 5) return 5;
    return 0;
}

int promotions(void) {
    if (M + M != 400 || sum != 400) return 6;
    if (!(BIG > 32767)) return 7;
    if (BIG / 3 != 13333) return 8;
    if (!(NEG < 0) || NEG + POS != 0 || sizeof NEG != 1) return 9;
    if (MEGA / 1000 != 1000 || sizeof(MEGA) != 4) return 10;
    if (ON + ON != 2 || sizeof(ON) != 1) return 11;
    if (LAST != 201 || last != 201) return 12;
    // unsigned short promotes to unsigned int, which wraps.
    if (TOP != 65535u || TOP + 1 != 0) return 21;
    if (-M != -200) return 22;
    return 0;
}

int objects(void) {
    enum kind k = LIST;
    k++;
    if (k != LAST) return 13;
    enum kind w = LAST;
    w += 60;
    if (w != 5) return 14;
    uint8_t t[4] = {0};
    t[nodes[1].k] = 9;
    if (t[PAIR] != 9) return 15;
    return 0;
}

int inferred(void) {
    auto a = M;
    typeof(BIG) b = BIG;
    if (sizeof a != 1 || b != 40000u || sizeof b != 2) return 16;
    constexpr uint8_t c = M;
    if (c != 200) return 17;
    uint8_t x = nodes[2].k;
    switch (x) {
    case LIST: break;
    default: return 18;
    }
    return 0;
}

int pointers(void) {
    uint8_t *p = &nodes[0].k;
    *p = PAIR;
    if (score(nodes[0].k) != 2) return 19;
    if (next(LIST) != LAST || next(LAST) != 202) return 20;
    return 0;
}

int main(void) {
    if (sizeof(struct node) != 2) return 1;
    if (sizeof(LEAF) != 1 || sizeof(enum kind) != 1) return 2;
    int r = uses();
    if (r == 0) r = promotions();
    if (r == 0) r = objects();
    if (r == 0) r = inferred();
    if (r == 0) r = pointers();
    return r;
}
)");
  REQUIRE(value.has_value());
  CHECK(static_cast<long long>(*value) == 0);
}
