// The tests of tests/test_slot_sharing.py that read a program's `-O`
// text or a link's errors. They check the values kept in a callee's
// parameter bytes across calls (the `share` records of the link
// metadata), parameters at a fixed zero-page address, and register
// parameters, which get no slot.
//
// Under `-O`, a function whose declaration names no register takes its
// leading parameters in A and X, and a parameter in a register has no
// bytes to share. So each function here whose parameters are to arrive
// in slots names a register for one parameter, such as
// `[[c6502::in_a]]` on a byte, and the default is off for the rest of
// its parameters.
#include <cstdint>
#include <map>
#include <set>
#include <string>
#include <string_view>
#include <tuple>
#include <vector>

#include "compile_case.h"
#include "doctest.h"

namespace {

namespace suite = c6502::suite;
using Equs = std::map<std::string, std::int64_t>;
using Lines = std::vector<std::string>;

suite::Options optimized(bool inline_calls) {
  suite::Options o;
  o.optimize = true;
  o.inline_calls = inline_calls;
  return o;
}

// The `-O` text, with the calls of small `static` functions inlined
// unless `inline_calls` is false.
std::string codegen(const std::string& src, bool inline_calls = true) {
  return suite::codegen(src, optimized(inline_calls));
}

// Returns the `share` records of the link metadata, each without its
// `; share ` prefix.
Lines shares(const std::string& asm_text) {
  Lines out;
  for (const std::string& line : suite::lines(asm_text)) {
    if (line.starts_with("; share ")) out.push_back(line.substr(8));
  }
  return out;
}

// Returns the instruction lines of `fn`'s body, each with its runs of
// whitespace made one space, up to the first line that isn't indented
// or a local label.
Lines body(const std::string& asm_text, const std::string& fn) {
  const std::string head = "\n" + fn + ":\n";
  const std::size_t at = asm_text.find(head);
  REQUIRE(at != std::string::npos);
  const std::size_t start = at + head.size();
  const std::size_t next = asm_text.find(head, start);
  const std::string text =
      asm_text.substr(start, next == std::string::npos ? std::string::npos : next - start);
  Lines out;
  for (const std::string& line : suite::lines(text)) {
    if (!line.empty() && line[0] != ' ' && line[0] != '\t' && line[0] != '.') break;
    if (!line.starts_with("   ")) continue;
    std::string joined;
    std::size_t k = 0;
    while (k < line.size()) {
      const std::size_t word = line.find_first_not_of(" \t", k);
      if (word == std::string::npos) break;
      const std::size_t end = line.find_first_of(" \t", word);
      if (!joined.empty()) joined += ' ';
      joined += line.substr(word, end - word);
      k = end;
    }
    if (joined != "SUBROUTINE") out.push_back(joined);
  }
  return out;
}

// Returns the bindings of the lines that `^(\S+)\tEQU\t\$([0-9A-F]+)$`
// matches.
Equs equs(const std::string& asm_text) {
  Equs out;
  for (const std::string& line : suite::lines(asm_text)) {
    const std::size_t tab = line.find("\tEQU\t$");
    if (tab == std::string::npos || tab == 0) continue;
    const std::string name = line.substr(0, tab);
    const std::string hex = line.substr(tab + 6);
    if (name.find_first_of(" \t") != std::string::npos || hex.empty() ||
        hex.find_first_not_of("0123456789ABCDEF") != std::string::npos) {
      continue;
    }
    out[name] = std::stoll(hex, nullptr, 16);
  }
  return out;
}

}  // namespace

TEST_CASE("slot_sharing: a global forwarder is dropped before its share") {
  // `store` passes its parameter on to `push` with it live across
  // `next_byte`, in another unit, which might call `store` (rule 3), so
  // `store` can't keep it in `push`'s bytes. `peek` can keep the value
  // it pops there across `push(7, v)`, since `push` is static. It had
  // been dropped along with `store`, whose external linkage made the
  // share's owners global.
  const std::string asm_text = codegen(R"(
        #include <stdint.h>
        uint8_t next_byte(void);
        void fail(void);
        uint16_t stack[8];
        uint8_t sp;
        static void push(uint8_t top [[c6502::in_a]], uint16_t value) {
            stack[sp++] = value;
            if (sp > top) fail();
        }
        static uint16_t pop(void) { return stack[--sp]; }
        void store(uint8_t top [[c6502::in_a]], uint16_t value) {
            if (next_byte() == 0) push(top, value);
        }
        uint16_t peek(void) { uint16_t v = pop(); push(7, v); return v; }
        )",
                                       false);
  const Lines s = shares(asm_text);
  REQUIRE(s.size() == 1);
  CHECK(suite::contains(s[0], "holder=peek"));
  CHECK_FALSE(suite::contains(s[0], "member=store:"));
  for (const std::string& line : body(asm_text, "peek")) {
    CHECK_FALSE(suite::contains(line, "__local_peek__"));
  }
}

namespace {

const std::string kUnitUse = R"(
#include <stdint.h>
extern uint16_t total;
void add_page(uint8_t i [[c6502::in_a]], const uint8_t *const p);
uint8_t buf[512];
int main(void) {
    for (uint16_t k = 0; k < 512; k++) buf[k] = (uint8_t)(k * 7);
    const uint8_t *p = buf;
    uint8_t pages = 2;
    do {
        add_page(0, p);
        p += 0x100;
    } while (--pages != 0);
    return total;
}
)";

// Compiles each source with `-c -O`, and inlining as `inline_calls`
// says, and links them. Returns the exit status, the linked program as
// `linked_text` gives it (empty when the link fails), and stderr.
std::tuple<long long, std::string, std::string> link_units(const std::vector<std::string>& sources,
                                                            bool inline_calls = true) {
  const suite::TempDir tmp;
  std::vector<std::string> paths;
  suite::MainOptions options;
  options.inline_calls = inline_calls;
  for (std::size_t k = 0; k < sources.size(); ++k) {
    const std::string path = tmp.file("u" + std::to_string(k) + ".o");
    const auto o = suite::compile_main({"compile.py", "-", "-c", "-O", "-o", path}, sources[k],
                                       options);
    REQUIRE(o.status == 0);
    paths.push_back(path);
  }
  const std::string out = tmp.file("out");
  const auto o = suite::link(paths, out);
  const long long rc = o.status.value_or(-1);
  return {rc, rc == 0 ? suite::linked_text(out) : "", o.err()};
}

}  // namespace

TEST_CASE("slot_sharing: a const declaration lets the caller hold a value in another unit's bytes") {
  const Lines s = shares(codegen(kUnitUse, false));
  REQUIRE(s.size() == 1);
  CHECK(suite::contains(s[0], "member=add_page:"));
  CHECK(suite::contains(s[0], "holder=main"));
  CHECK(suite::contains(s[0], "live=main:add_page"));
}

TEST_CASE("slot_sharing: without const nothing is held in another unit's bytes") {
  std::string src = kUnitUse;
  const std::size_t at = src.find("*const p");
  REQUIRE(at != std::string::npos);
  src.replace(at, 8, "*p");
  CHECK(shares(codegen(src, false)).empty());
}

TEST_CASE("slot_sharing: a link fails when something the callee calls writes the held bytes") {
  // `add_page` passes its parameter on to `scan`, so the two share
  // bytes, and then `reset` passes `scan` another pointer while `main`
  // still holds its own there.
  const auto [rc, text, err] = link_units({R"(
        #include <stdint.h>
        uint16_t total;
        uint8_t zeros[256];
        static void scan(uint8_t i [[c6502::in_a]], const uint8_t *p) {
            do { total += p[i]; i++; } while (i != 0);
        }
        static void reset(void) { scan(0, zeros); }
        void add_page(uint8_t i [[c6502::in_a]], const uint8_t *const p) {
            scan(i, p);
            reset();
        }
        )",
                                           kUnitUse},
                                          false);
  CHECK(rc != 0);
  CHECK(suite::contains(err, "`reset`"));
}

TEST_CASE("slot_sharing: a pinned parameter is copied when the function calls another unit") {
  // `elsewhere` might call a function that takes its argument at $30.
  const std::string asm_text = codegen(R"(
        void elsewhere(void);
        unsigned char f(
            const unsigned char *p [[c6502::zeropage(0x30)]]) {
            elsewhere();
            return *p;
        }
        )");
  const Lines b = body(asm_text, "f");
  REQUIRE(!b.empty());
  CHECK(b[0] == "LDA __zpabi_f__p_0");
}

TEST_CASE("slot_sharing: the declarations of a pinned parameter must agree") {
  const suite::Error e = suite::compile_error(suite::Stage::kCodegen, R"(
            void f(const char *s [[c6502::zeropage(0xE6)]]);
            void f(const char *s) { (void)s; }
            int main(void) { f("a"); return 0; }
            )",
                                              optimized(true));
  CHECK(suite::is_a(e.type, "AbiSelectionError"));
  CHECK(suite::search("disagree", e.message));
}

TEST_CASE("slot_sharing: the runtime's block moves off a pinned parameter's bytes") {
  const std::string asm_text = codegen(R"(
        [[c6502::zp_abi, c6502::address(0xD000)]]
        void put(const unsigned char *p [[c6502::zeropage(0x04)]]);
        unsigned char data[1];
        int main(void) { put(data); return 0; }
        )");
  const Equs e = equs(asm_text);
  const std::size_t at = asm_text.find("hargs_size=");
  REQUIRE(at != std::string::npos);
  const std::size_t digits = at + 11;
  const std::size_t end = asm_text.find_first_not_of("0123456789", digits);
  REQUIRE(end > digits);
  const std::int64_t size = std::stoll(asm_text.substr(digits, end - digits));
  std::set<std::int64_t> block;
  for (std::int64_t a = e.at("HARGS"); a < e.at("HARGS") + size; ++a) block.insert(a);
  for (const char* name : {"SSP", "FP", "DPTR"}) {
    block.insert(e.at(name));
    block.insert(e.at(name) + 1);
  }
  CHECK(block.count(0x04) == 0);
  CHECK(block.count(0x05) == 0);
}

TEST_CASE("slot_sharing: a pinned parameter is recorded for the linker") {
  const std::string asm_text = codegen(R"(
[[c6502::zp_abi, c6502::address(0xD000)]]
unsigned char total(const unsigned char *p [[c6502::zeropage(0xE6)]],
                    register unsigned char n [[c6502::in_x]]);

        unsigned char data[1];
        int main(void) { return total(data, 1); }
        )");
  CHECK(suite::contains(asm_text, "pinned=$E6,$E7,"));
}

TEST_CASE("slot_sharing: a register parameter has no slot") {
  const Equs e = equs(codegen(R"(
        unsigned char out;
        void put(register unsigned char k [[c6502::in_a]],
                 unsigned char *p) { *p = k; }
        int main(void) { put(5, &out); return out; }
        )"));
  CHECK(e.count("__zpabi_put__k") == 0);
  CHECK(e.at("__zpabi_put__p_0") == 0x80);
}

TEST_CASE("slot_sharing: a parameter passed on to two callees shares both callees' bytes") {
  // As Zork's `is_separator(c)` passes `c` to `is_dict_separator` and
  // then to `is_std_separator`, so the function copies it for neither.
  // Each function here takes a step count in X, so that `c` arrives in
  // its slot.
  const std::string asm_text = codegen(R"(
    #include <stdint.h>
    #include <stdbool.h>
    uint8_t calls;
    static bool is_a(uint8_t step [[c6502::in_x]], uint8_t c) {
        calls += step;
        return c == 'a';
    }
    static bool is_b(uint8_t step [[c6502::in_x]], uint8_t c) {
        calls += step;
        return c == 'b';
    }
    static bool is_ab(uint8_t step [[c6502::in_x]], uint8_t c) {
        if (is_a(step, c)) return true;
        return is_b(step, c);
    }
    int main(void) {
        uint8_t n = 0;
        for (uint8_t c = 'a'; c != 'e'; c++)
            n = (uint8_t)(n << 1) | is_ab(1, c);
        return n * 16 + calls;
    }
    )");
  const Lines s = shares(asm_text);
  REQUIRE(s.size() == 1);
  for (const char* f : {"is_a", "is_b", "is_ab"}) {
    INFO(f);
    CHECK(suite::contains(s[0], std::string("member=") + f + ":"));
  }
  const Equs e = equs(asm_text);
  CHECK(e.at("__zpabi_is_a__c") == e.at("__zpabi_is_b__c"));
  CHECK(e.at("__zpabi_is_ab__c") == e.at("__zpabi_is_b__c"));
  // Nothing is copied between the calls.
  for (const std::string& line : body(asm_text, "is_ab")) {
    CHECK_FALSE(suite::contains(line, "__zpabi_"));
  }
}
