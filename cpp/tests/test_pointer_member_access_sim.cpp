// Struct members reached through pointers, run in the simulator in both
// pipelines, from tests/test_pointer_member_access.py: wide stores and
// loads through a pointer, a node that holds its own address, a compound
// assignment with a call, the choice of a member's offset, a member
// compare, and `&p[n - 1]` with its other products.
#include <string>
#include <vector>

#include "doctest.h"
#include "sim/cpu_run.h"
#include "sim_case.h"

namespace c6502::suite {

namespace {

const std::string kRun = R"c(
#include <stdint.h>
struct s { uint8_t a[13]; uint16_t g; uint8_t *buf; };
struct node { struct node *next; uint16_t v; };
struct mob { uint8_t pad[7]; uint8_t sword, armor, dagger, level; uint16_t gold; };
struct s obj;
struct mob m;
struct s *P [[c6502::zeropage(0xF4)]];
struct node *N [[c6502::zeropage(0xF6)]];
struct mob *E [[c6502::zeropage(0xF8)]];
uint16_t W;
uint8_t calls;
void put(uint16_t v) { P->g = v; }
void put2(struct s *r, uint16_t v) { r->g = v; }
uint16_t get(struct s *r) { return r->g; }
void step(void) { P->buf += 0x100; }
uint8_t h(uint8_t x) { calls++; return x + 1; }
void choose(uint8_t n, uint8_t v) {
    uint8_t *w = n < 3 ? &E->sword : &E->dagger;
    *w = v;
}
uint8_t le(void) { return W <= E->gold; }
struct mob m2;
void choose_if(uint8_t n, uint8_t v) {
    uint8_t *w = &E->sword;
    if (n > 5 && E->level == 3) w = &E->dagger;
    *w = v;
}
void choose_moved(uint8_t n, uint8_t v) {
    uint8_t *w = &E->sword;
    if (n > 5) { E = &m2; w = &E->dagger; }
    *w = v;
}
struct o { uint8_t b[9]; };
struct o *option(struct o *p, uint8_t n) { return &p[n - 1]; }
long scaled(uint8_t n) { return ((long)n - 5) * 100000L; }
uint16_t times37(uint8_t n) { return (n + 3) * 37; }
)c";

std::string program(const std::string& body) { return kRun + "int main(void) {" + body + "}"; }

void check_runs(const std::string& src, std::int64_t expected) {
  for (const bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    const sim::SimResult r = build_sim(src, optimize).run(2'000'000);
    CHECK_FALSE(r.timed_out);
    CHECK(r.return_int() == expected);
  }
}

}  // namespace

TEST_CASE("pointer_member_access sim: wide member stores and loads") {
  check_runs(program(R"c(
            P = &obj;
            put(0x1234);
            put2(&obj, obj.g + 0x0101);
            obj.buf = (uint8_t *)0x20FF;
            step();
            return get(&obj) == 0x1335 && obj.buf == (uint8_t *)0x21FF
                && obj.a[12] == 0;)c"), 1);
}

TEST_CASE("pointer_member_access sim: a node holding its own address") {
  check_runs(program(R"c(
            N = (struct node *)&N;
            N->next = (struct node *)0x1234;
            return (uint16_t)N;)c"), 0x1234);
}

TEST_CASE("pointer_member_access sim: a compound assignment with a call") {
  check_runs(program(R"c(
            E = &m;
            m.level = 0x40;
            E->level |= h(2);
            E->level -= h(0);
            return m.level * 16 + calls;)c"), (0x43 - 1) * 16 + 2);
}

TEST_CASE("pointer_member_access sim: the choice of a member") {
  check_runs(program(R"c(
            E = &m;
            choose(2, 11);
            choose(3, 22);
            choose(255, 33);
            return m.sword * 256 + m.dagger + m.armor;)c"), 11 * 256 + 33);
}

TEST_CASE("pointer_member_access sim: the choice of a member after a test") {
  check_runs(program(R"c(
            E = &m;
            m.level = 3;
            choose_if(5, 11);
            choose_if(6, 22);
            m.level = 4;
            choose_if(7, 33);
            return m.sword * 256 + m.dagger;)c"), 33 * 256 + 22);
}

TEST_CASE("pointer_member_access sim: the choice of a member after the pointer moves") {
  check_runs(program(R"c(
            E = &m;
            choose_moved(5, 11);
            choose_moved(6, 22);
            return m.sword * 256 + m2.dagger + m.dagger;)c"), 11 * 256 + 22);
}

TEST_CASE("pointer_member_access sim: a member compare") {
  check_runs(program(R"c(
            E = &m;
            m.gold = 0x1200;
            W = 0x11FF; uint8_t a = le();
            W = 0x1200; uint8_t b = le();
            W = 0x1201; uint8_t c = le();
            W = 0x1300; uint8_t d = le();
            return a + 2 * b + 4 * c + 8 * d;)c"), 3);
}

TEST_CASE("pointer_member_access sim: an index minus one") {
  const std::pair<int, int> cases[] = {{0, -9}, {1, 0}, {30, 261}, {255, 2286}};
  std::string all;
  for (const auto& [n, expected] : cases) {
    INFO("n=" << n);
    const std::string src = program(
        "\n                    struct o *base = (struct o *)0x4000;\n"
        "                    return (uint16_t)option(base, " +
        std::to_string(n) + ") - 0x4000;");
    all += src;
    check_runs(src, expected & 0xFFFF);
  }
  // The CRC-32 of the four programs tests/test_pointer_member_access.py
  // generated on 27 Nov 2026.
  CHECK(sim::cpu_run::crc32(all) == 0x202f58aau);
}

TEST_CASE("pointer_member_access sim: the other products") {
  check_runs(program(R"c(return scaled(0) == -500000L && scaled(255) == 25000000L && times37(0) == 111 && times37(255) == (uint16_t)(258 * 37);)c"), 1);
}

}  // namespace c6502::suite
