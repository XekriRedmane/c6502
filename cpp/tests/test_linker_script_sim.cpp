#include <optional>
#include <string>
#include <vector>

#include "compile_case.h"
#include "doctest.h"
#include "sim_case.h"

namespace c6502::suite {

namespace {

// Compiles each source with `-O` to an object and links them with the
// simulator's helper addresses and `script` as the linker script.
// Returns the executable's bytes, and fails the case when a step fails.
std::string link_program(const std::vector<std::string>& sources,
                         const std::optional<std::string>& script = std::nullopt) {
  const TempDir tmp;
  std::vector<std::string> paths;
  for (std::size_t k = 0; k < sources.size(); ++k) {
    const std::string path = tmp.file("u" + std::to_string(k) + ".o");
    REQUIRE(compile_object(sources[k], path) == 0);
    paths.push_back(path);
  }
  std::optional<std::string> ld;
  if (script) {
    ld = tmp.file("prog.ld");
    write_file(*ld, *script);
  }
  const std::string out = tmp.file("out");
  const auto o = link(paths, out, ld);
  INFO(o.err());
  REQUIRE(o.status.value_or(-1) == 0);
  return read_file(out);
}

}  // namespace

TEST_CASE("linker_script sim: a scripted link runs with .data copied and .bss cleared") {
  // `main` goes first, `squares` is on a page of its own, `seed` is in
  // .data and `buf` in .bss (an array, which promotion leaves alone),
  // and `vectors` is at $BFFA.
  const std::string rom_script = R"c(
MEMORY
{
    ram (rwx) : ORIGIN = 0x0800, LENGTH = 0x5800
    rom (rx)  : ORIGIN = 0x8000, LENGTH = 16K
}

SECTIONS
{
    .text : { *(.text.boot) *(.text .text.*) } > rom
    .rodata : { *(.rodata .rodata.*) } > rom
    .page ALIGN(256) : { *(.page) } > rom
    .data : { *(.data .data.*) } > ram AT> rom
    .bss (NOLOAD) : { *(.bss .bss.*) } > ram
    .vectors 0xBFFA : { KEEP(*(.vectors)) }
    __free_ram = ADDR(.bss) + SIZEOF(.bss);
}
)c";
  const std::string unit_a = R"c(
[[c6502::section(".text.boot")]] int main(void);
const unsigned char table[4] = {10, 20, 30, 40};
unsigned char squares [[c6502::section(".page")]][4] = {0, 1, 4, 9};
unsigned char seed = 7;
unsigned char buf[3];
unsigned char pick(unsigned char i);
[[c6502::section(".vectors")]]
int (*const vectors[3])(void) = {main, main, main};
int main(void) {
    return pick(2) + seed + squares[3] + buf[0] + buf[1] + buf[2];
}
)c";
  const std::string unit_b = R"c(
extern const unsigned char table[4];
static unsigned char hits = 100;
unsigned char pick(unsigned char i) { hits++; return table[i] + hits; }
)c";
  sim::Simulation s = sim::build_linked_sim(link_program({unit_a, unit_b}, rom_script));
  CHECK(s.symbol("main") == 0x8000);
  CHECK((s.symbol("squares") & 0xFF) == 0);
  CHECK(s.symbol("__page_start") == s.symbol("squares"));
  CHECK(s.symbol("seed") == 0x0800);
  CHECK(s.symbol("__data_load") >= 0x8000);
  CHECK(s.symbol("vectors") == 0xBFFA);
  CHECK(s.symbol("__free_ram") == s.symbol("__bss_end"));
  // RAM holds garbage until the startup code copies and clears.
  for (int a = 0x0800; a < 0x0900; ++a) s.cpu.memory[a] = 0xFF;
  const sim::SimResult r = s.run();
  // 30 + 101 + 7 + 9, with buf cleared.
  CHECK(r.return_int() == 147);
  CHECK(r.word(0xBFFA) == 0x8000);
}

TEST_CASE("linker_script sim: a function that ends by calling another unit's falls into it") {
  // `other` comes between `step` and `finish` in the script's order.
  const std::string main_unit = R"c(unsigned char step(unsigned char x);
unsigned char finish(unsigned char x);
unsigned char other(void);
int main(void) { return finish(1) + step(2) + other(); }
unsigned char step(unsigned char x) {
    return finish(x + 3);
}
)c";
  const std::string finish_unit = R"c(unsigned char other(void) { return 5; }
unsigned char finish(unsigned char x) {
    return x * 2 + 1;
}
)c";
  std::string script = R"c(
MEMORY { ram (rwx) : ORIGIN = 0x0800, LENGTH = 0x5800 }
SECTIONS
{
    .text%s : { *(.text .text.*) } > ram
    .rodata : { *(.rodata .rodata.*) } > ram
    .data : { *(.data .data.*) } > ram
    .bss : { *(.bss .bss.*) } > ram
}
)c";
  script.replace(script.find("%s"), 2, "");
  CHECK(sim::build_linked_sim(link_program({main_unit, finish_unit}, script)).run().return_int() == 3 + 11 + 5);
}

TEST_CASE("linker_script sim: internal names two units share are renamed in one of them") {
  const std::string a = R"c(static unsigned char hits = 1;
unsigned char b(void);
int main(void) { const char *s = "A"; hits++; return hits + s[0] + b(); }
)c";
  const std::string b = R"c(static unsigned char hits = 10;
unsigned char b(void) { const char *s = "B"; hits++; return hits + s[0]; }
)c";
  CHECK(sim::build_linked_sim(link_program({a, b})).run().return_int() == 2 + 'A' + 11 + 'B');
}

}  // namespace c6502::suite
