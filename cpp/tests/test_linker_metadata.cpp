// Tests of the linker's functions on the link metadata
// (src/passes/linker_metadata.*) and of `merge_shares` and
// `check_linked_shares` (src/passes/slot_sharing.*): Python's message for
// every raise site of those functions, with the allocators' errors that a
// link's synthetic program reaches, and the orders and text of Python
// that the results keep. Each expected value is what the Python function
// gives for the same input. `c6502cpp check-binutils` checks the
// functions on every call the corpus and the suite make of them.
#include <optional>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "doctest.h"
#include "errors.h"
#include "passes/linker_metadata.h"
#include "passes/slot_sharing.h"
#include "passes/zp_local_allocation.h"
#include "passes/zp_slot_allocation.h"
#include "pyrepr.h"

namespace {

using c6502::CompileError;
using c6502::asdl::Int;
using c6502::passes::ExternMeta;
using c6502::passes::FunctionMeta;
using c6502::passes::ItemMeta;
using c6502::passes::LinkMetadata;
using c6502::passes::NameSet;
using c6502::passes::SlotShare;
using c6502::passes::ZpObjectMeta;
namespace linker = c6502::passes::linker;
namespace elf = c6502::binutils::elf;

using Strings = std::vector<std::string>;
using Pinned = std::vector<std::optional<Int>>;

FunctionMeta def(std::string name, Strings params = {}, Strings regs = {}, Pinned pinned = {}) {
  FunctionMeta d;
  d.name = std::move(name);
  d.params = std::move(params);
  d.param_regs = std::move(regs);
  d.pinned = std::move(pinned);
  return d;
}

ExternMeta ext(std::string name, Strings params = {}, Strings regs = {}, Pinned pinned = {},
               bool carry = false) {
  return ExternMeta{std::move(name), std::move(params), std::move(regs), std::move(pinned),
                    carry};
}

ZpObjectMeta zp(std::string name, Int size, std::optional<Int> at = std::nullopt,
                bool defined = true, std::optional<Int> weight = std::nullopt,
                Strings init = {}) {
  ZpObjectMeta z;
  z.name = std::move(name);
  z.size = size;
  z.at = at;
  z.defined = defined;
  z.weight = weight;
  z.init = std::move(init);
  return z;
}

LinkMetadata with_defs(std::vector<FunctionMeta> defs) {
  LinkMetadata m;
  m.defs = std::move(defs);
  return m;
}

LinkMetadata with_externs(std::vector<ExternMeta> externs) {
  LinkMetadata m;
  m.externs = std::move(externs);
  return m;
}

LinkMetadata with_zp(std::vector<ZpObjectMeta> objects) {
  LinkMetadata m;
  m.zp_objects = std::move(objects);
  return m;
}

// Returns the message of the CompileError `f` throws, prefixed with its
// Python type, or "no error".
template <class F>
std::string error_of(F&& f) {
  try {
    f();
  } catch (const CompileError& e) {
    return e.python_type() + ": " + e.what();
  }
  return "no error";
}

const linker::LayoutDict kClassic{
    {"SSP", 0}, {"FP", 2}, {"HARGS", 4}, {"DPTR", 0x24}, {"HARGS size", 32}};

linker::ZpObjects objects(const std::vector<ZpObjectMeta>& zs) {
  linker::ZpObjects out;
  for (const auto& z : zs) out.insert_or_assign(z.name, std::vector<ZpObjectMeta>{z});
  return out;
}

Strings names_of(const linker::SymbolAddrs& addrs) {
  Strings out;
  for (const auto& [name, addr] : addrs) out.push_back(name + "=" + c6502::pyrepr::integer(addr));
  return out;
}

SlotShare share(std::vector<std::pair<std::string, Strings>> members, Strings holders = {}) {
  SlotShare s;
  s.members = std::move(members);
  s.holders = std::move(holders);
  return s;
}

}  // namespace

TEST_CASE("merge: the declarations' calling conventions, the carry, and the duplicates") {
  CHECK(error_of([] {
          linker::merge({with_externs({ext("f", {"a"}, {"A"})}),
                         with_externs({ext("f", {"b"}, {""})})});
        }) ==
        "LinkError: the units declare `f` with different parameters in registers or at "
        "zero-page addresses; every declaration has to give the same calling convention");
  CHECK(error_of([] {
          linker::merge({with_defs({def("f", {"a"}, {}, {0x10})}),
                         with_externs({ext("f", {"b"})})});
        }) ==
        "LinkError: the units declare `f` with different parameters in registers or at "
        "zero-page addresses; every declaration has to give the same calling convention");
  CHECK(error_of([] {
          FunctionMeta f = def("f");
          f.carry = true;
          linker::merge({with_defs({f}), with_externs({ext("f")})});
        }) ==
        "LinkError: the units disagree on whether `f` returns its `bool` in the carry flag. "
        "Under -O, a `bool` function with external linkage does when it's zp_abi, so a unit "
        "that takes its address, or one that can't give it zp_abi, sees it differently from "
        "the others. A function that returns `unsigned char` returns it in A in every unit");
  // Code-point order puts "B" before "a" and "é" last, and Python's repr
  // quotes "it's" with double quotes. Each name is listed once.
  CHECK(error_of([] {
          linker::merge({with_defs({def("z"), def("é"), def("B")}),
                         with_defs({def("é"), def("B"), def("a")}),
                         with_defs({def("é"), def("it's")}), with_defs({def("it's")})});
        }) == "LinkError: function(s) defined in multiple TUs: ['B', \"it's\", 'é']");
  CHECK(error_of([] {
          linker::merge({with_externs({ext("g", {"a"}), ext("f", {"a"})}),
                         with_externs({ext("g", {"a", "b"}), ext("f", {})})});
        }) ==
        "LinkError: zp_abi extern(s) declared with conflicting param_bytes across TUs: "
        "['f', 'g']");
}

TEST_CASE("merge: the definitions, externs, and edges in code-point order, once each") {
  LinkMetadata a = with_defs({def("z"), def("é")});
  a.externs = {ext("q")};
  a.calls = {{"z", "é"}, {"B", "a"}, {"z", "a"}};
  a.saved_calls = {{"z", "é"}};
  LinkMetadata b = with_defs({def("B"), def("a")});
  b.externs = {ext("q"), ext("Q")};
  b.calls = {{"z", "a"}, {"a", "B"}};
  const LinkMetadata m = linker::merge({a, b});
  Strings defs;
  for (const auto& d : m.defs) defs.push_back(d.name);
  CHECK(defs == Strings{"B", "a", "z", "é"});
  REQUIRE(m.externs.size() == 2);
  CHECK(m.externs[0].name == "Q");
  CHECK(m.externs[1].name == "q");
  using Edges = std::vector<std::pair<std::string, std::string>>;
  CHECK(m.calls == Edges{{"B", "a"}, {"a", "B"}, {"z", "a"}, {"z", "é"}});
  CHECK(m.saved_calls == Edges{{"z", "é"}});
}

TEST_CASE("arrivals pads to the parameter bytes and stops at the shorter list") {
  using A = linker::Arrivals;
  CHECK(linker::arrivals(def("f", {"a", "b", "c"}, {"A"}, {std::nullopt, 5})) ==
        A{{"A", std::nullopt}, {"", 5}, {"", std::nullopt}});
  CHECK(linker::arrivals(ext("f", {"a"}, {"A", "X"}, {})) == A{{"A", std::nullopt}});
  CHECK(linker::arrivals(ext("f", {}, {"A", "X"}, {1, 2, 3})) == A{{"A", 1}, {"X", 2}});
  CHECK(linker::regs({"A"}, 3) == Strings{"A", "", ""});
  CHECK(linker::regs({"A", "X"}, 1) == Strings{"A", "X"});
}

TEST_CASE("validate_eligibility") {
  FunctionMeta z = def("z");
  z.indirect = true;
  FunctionMeta b = def("b");
  b.indirect = true;
  CHECK(error_of([&] { linker::validate_eligibility(with_defs({z, b, def("c")})); }) ==
        "LinkError: function(s) contain IndirectCall and can't be linked under the "
        "private-pool model: ['b', 'z']");
  FunctionMeta f = def("f");
  f.in_cycle = true;
  FunctionMeta g = def("g");
  g.in_cycle = true;
  LinkMetadata cyclic = with_defs({g, f});
  cyclic.calls = {{"f", "g"}, {"g", "f"}};
  CHECK(error_of([&] { linker::validate_eligibility(cyclic); }) ==
        "LinkError: function(s) on a cycle in their own TU's call graph (recursion is "
        "supported only when every call in the cycle is marked "
        "`[[c6502::save_hardware_stack]]`): ['f', 'g']");
  cyclic.saved_calls = cyclic.calls;
  CHECK(error_of([&] { linker::validate_eligibility(cyclic); }) == "no error");
  // Neither unit saw the cycle, so neither marked its functions
  // `in_cycle`.
  LinkMetadata across = with_defs({def("g"), def("f"), def("main")});
  across.calls = {{"f", "g"}, {"g", "f"}, {"main", "g"}};
  CHECK(error_of([&] { linker::validate_eligibility(across); }) ==
        "LinkError: function(s) on a cycle of calls between units, which can't have private "
        "pools (recursion is supported only within one unit, when every call in the cycle is "
        "marked `[[c6502::save_hardware_stack]]`): ['f', 'g']");
  LinkMetadata bad = with_defs({def("main"), def("f")});
  bad.externs = {ext("e")};
  bad.calls = {{"main", "puts"}, {"main", "e"}, {"main", "abort"}, {"f", "it's"}};
  CHECK(error_of([&] { linker::validate_eligibility(bad); }) ==
        "LinkError: function(s) call non-zp_abi externs that aren't defined in any input TU: "
        "f → [\"it's\"]; main → ['abort', 'puts']");
}

TEST_CASE("synthesize drives the allocators to Python's errors in a link") {
  // The options `link_objects` gives the allocators with a script: the
  // whole pool, the bytes below it, and the spill region of `__spill`.
  c6502::passes::ZpAllocationOptions options;
  options.reserved = linker::runtime_bytes(kClassic);
  options.window = c6502::passes::Range{0x80, 0x100, 1};
  options.fallback = c6502::passes::Range{0, 0x80, 1};
  options.spill_start = 0xF000;
  options.spill_end = 0x10000;
  Strings params;
  Strings locals;
  for (int k = 0; k < 4097; ++k) {
    params.push_back("__zpabi_f__" + std::to_string(k));
    locals.push_back("__local_g__" + std::to_string(k));
  }
  FunctionMeta g = def("g");
  g.locals = locals;
  LinkMetadata merged = with_defs({def("f", params), g, def("main")});
  merged.calls = {{"main", "f"}, {"main", "g"}};
  const linker::Synthesized s = linker::synthesize(merged);
  CHECK(error_of([&] { c6502::passes::allocate_zp_slots(s.prog, s.abi, options); }) ==
        "ZpSlotAllocationError: can't allocate 4097-byte slot range for function `f`: no "
        "contiguous gap in either the ZP window ($80-$FF) or the spill region ($F000-$FFFF) "
        "avoids every already-allocated caller's range");
  LinkMetadata only_g = with_defs({g, def("main")});
  only_g.calls = {{"main", "g"}};
  const linker::Synthesized t = linker::synthesize(only_g);
  const auto abi = c6502::passes::allocate_zp_slots(t.prog, t.abi, options).first;
  CHECK(error_of([&] {
          c6502::passes::allocate_function_locals(t.prog, abi, t.local_bytes, options);
        }) ==
        "ZpLocalAllocationError: can't allocate 4097-byte local pool for function `g`: no "
        "contiguous gap in either ZP ($80-$FF) or spill region ($F000-$FFFF) avoids the 38 "
        "forbidden bytes");
}

TEST_CASE("synthesize builds a call per edge and the layouts of the slot symbols") {
  FunctionMeta f = def("f", {"f0", "f1"}, {"A"});
  f.locals = {"l0", "l1", "l2"};
  LinkMetadata merged = with_defs({f, def("main")});
  merged.externs = {ext("f", {"x"}), ext("put", {"p0"}, {"", "X"}, {0x40})};
  merged.calls = {{"main", "f"}, {"main", "put"}, {"other", "f"}};
  merged.saved_calls = {{"main", "put"}};
  const linker::Synthesized s = linker::synthesize(merged);
  REQUIRE(s.prog.top_level.size() == 2);
  const auto& main_fn = std::get<c6502::tac_ast::Function>(s.prog.top_level[1]);
  REQUIRE(main_fn.instructions.size() == 2);
  const auto& put = std::get<c6502::tac_ast::FunctionCall>(main_fn.instructions[1]);
  CHECK(put.name == "put");
  CHECK(put.hw_stack_save);
  CHECK(!std::get<c6502::tac_ast::FunctionCall>(main_fn.instructions[0]).hw_stack_save);
  REQUIRE(s.abi.size() == 3);
  const auto& fl = std::get<c6502::passes::ZpLayout>(s.abi.at("f"));
  CHECK(fl.slot_symbols == Strings{"f0", "f1"});
  CHECK(fl.param_registers == std::vector<std::optional<std::string>>{"A", std::nullopt});
  CHECK(std::holds_alternative<c6502::passes::SoftStackLayout>(s.abi.at("main")));
  const auto& pl = std::get<c6502::passes::ZpLayout>(s.abi.at("put"));
  CHECK(pl.param_registers == std::vector<std::optional<std::string>>{std::nullopt, "X"});
  CHECK(pl.pinned == Pinned{0x40});
  CHECK(s.local_bytes.at("f") == 3);
}

TEST_CASE("rename_locals and rename_meta") {
  linker::Objects objects{{"a.o", elf::ElfFile{}}, {"b.o", elf::ElfFile{}}};
  std::vector<LinkMetadata> metas(2);
  metas[0].items = {ItemMeta{.name = "f", .section = ".text"}};
  metas[1].items = {ItemMeta{.name = "f", .section = ".text"}};
  CHECK(error_of([&] { linker::rename_locals(objects, metas, {}); }) ==
        "LinkError: `f` is defined in multiple TUs");

  // Unit 0's `s` and its slots, and its symbol `s`, become `s.0`, unit
  // 1's `s` becomes `s.1`, and `t`, which another name of the program
  // takes, `t.1`. A global definition keeps its name.
  auto symbol = [](std::string name, bool global, bool defined) {
    auto s = std::make_shared<elf::Symbol>();
    s->name = std::move(name);
    s->bind = global ? elf::STB_GLOBAL : elf::STB_LOCAL;
    s->shndx = defined ? elf::SHN_ABS : elf::SHN_UNDEF;
    return s;
  };
  objects[0].second.symbols = {symbol("s", false, true), symbol("__local_s__t", false, false)};
  objects[1].second.symbols = {symbol("s", false, true), symbol("t", true, true),
                               symbol("t", false, false)};
  metas[0] = LinkMetadata{};
  metas[0].items = {ItemMeta{.name = "s", .section = ".text", .local = true}};
  FunctionMeta s0 = def("s", {"__zpabi_s__x"});
  s0.locals = {"__local_s__t"};
  metas[0].defs = {s0};
  metas[0].calls = {{"main", "s"}};
  metas[1] = LinkMetadata{};
  metas[1].items = {ItemMeta{.name = "s", .section = ".text", .local = true},
                    ItemMeta{.name = "t", .section = ".data", .local = true}};
  metas[1].zp_objects = {zp("z", 1)};
  const auto renames = linker::rename_locals(objects, metas, NameSet{"t", "z"});
  REQUIRE(renames.size() == 2);
  Strings r0;
  for (const auto& [from, to] : renames[0]) r0.push_back(from + ">" + to);
  CHECK(r0 == Strings{"s>s.0", "__zpabi_s__x>__zpabi_s__x.0", "__local_s__t>__local_s__t.0"});
  Strings r1;
  for (const auto& [from, to] : renames[1]) r1.push_back(from + ">" + to);
  CHECK(r1 == Strings{"s>s.1", "t>t.1"});
  CHECK(objects[0].second.symbols[0]->name == "s.0");
  CHECK(objects[0].second.symbols[1]->name == "__local_s__t.0");
  CHECK(objects[1].second.symbols[1]->name == "t");
  CHECK(objects[1].second.symbols[2]->name == "t.1");
  CHECK(metas[0].defs[0].name == "s.0");
  CHECK(metas[0].defs[0].params == Strings{"__zpabi_s__x.0"});
  CHECK(metas[0].calls[0].second == "s.0");
  CHECK(metas[1].items[1].name == "t.1");
  // `z` isn't internal, so it keeps its name.
  CHECK(metas[1].zp_objects[0].name == "z");

  LinkMetadata m;
  SlotShare sh = share({{"f", {"a", "b"}}}, {"f", "h"});
  sh.live = {{"f", {"f", "g"}}};
  sh.forwards = {{"f", "g"}};
  sh.writers = {"f"};
  m.shares = {sh};
  m.externs = {ext("f", {"a"})};
  m.saved_calls = {{"f", "f"}};
  linker::rename_meta(m, linker::Renames{{"f", "f.2"}, {"a", "a.2"}});
  const SlotShare& r = m.shares[0];
  CHECK(r.members[0].first == "f.2");
  CHECK(r.members[0].second == Strings{"a.2", "b"});
  CHECK(r.holders == Strings{"f.2", "h"});
  CHECK(r.live[0].first == "f.2");
  CHECK(r.live[0].second == Strings{"f.2", "g"});
  CHECK(r.forwards[0] == std::pair<std::string, std::string>{"f.2", "g"});
  CHECK(r.writers == Strings{"f.2"});
  CHECK(m.externs[0].name == "f.2");
  CHECK(m.externs[0].params == Strings{"a.2"});
  CHECK(m.saved_calls[0] == std::pair<std::string, std::string>{"f.2", "f.2"});
}

TEST_CASE("merge_zp_objects") {
  CHECK(error_of([] {
          linker::merge_zp_objects({with_zp({zp("v", 1)}), with_zp({zp("v", 2, {}, false)})});
        }) == "LinkError: zero-page object `v` has different sizes in different units");
  CHECK(error_of([] {
          linker::merge_zp_objects(
              {with_zp({zp("v", 1, 0x30, false)}), with_zp({zp("v", 1, 0x31, false)})});
        }) == "LinkError: zero-page object `v` is declared at different addresses in different "
              "units");
  CHECK(error_of([] { linker::merge_zp_objects({with_zp({zp("v", 1)}), with_zp({zp("v", 1)})}); }) ==
        "LinkError: zero-page object `v` is defined in 2 units");
  CHECK(error_of([] {
          linker::merge_zp_objects(
              {with_zp({zp("v", 1, {}, false)}), with_zp({zp("v", 1, {}, false)})});
        }) == "LinkError: zero-page object `v` is declared `extern` but no unit defines it");
  CHECK(error_of([] { linker::merge_zp_objects({with_zp({zp("v", 0, {}, true, 1)})}); }) ==
        "LinkError: zero-page object `v` has 0 bytes, where an object has at least one");
  CHECK(error_of([] { linker::merge_zp_objects({with_zp({zp("v", -1)})}); }) ==
        "LinkError: zero-page object `v` has -1 bytes, where an object has at least one");
  // The objects in the order the units list them.
  const auto merged = linker::merge_zp_objects(
      {with_zp({zp("b", 1), zp("a", 1, {}, false)}), with_zp({zp("a", 1), zp("c", 2)})});
  Strings names;
  for (const auto& [name, records] : merged) names.push_back(name + std::to_string(records.size()));
  CHECK(names == Strings{"b1", "a2", "c1"});
}

TEST_CASE("place_zp_objects") {
  CHECK(error_of([] { linker::place_zp_objects(objects({zp("v", 2, 0x23)}), kClassic); }) ==
        "LinkError: zero-page object `v` at $23 overlaps the runtime's zero-page block");
  CHECK(error_of([] {
          linker::place_zp_objects(objects({zp("v", 2, 0x40), zp("w", 2, 0x41)}), kClassic);
        }) == "LinkError: zero-page object `w` at $41 overlaps `v` at byte $41");
  CHECK(error_of([] { linker::place_zp_objects(objects({zp("v", 0x5B)}), kClassic); }) ==
        "LinkError: no room for zero-page object `v` (91 bytes) below $80 in the linked program");
  CHECK(error_of([] {
          linker::place_zp_objects(objects({zp("v", 0x5A), zp("w", 1)}), kClassic);
        }) ==
        "LinkError: no room for zero-page object `w` (1 bytes) below $80 in the linked program");
  CHECK(error_of([] {
          linker::place_zp_objects(objects({zp("v", 0x5A), zp("p", 2, {}, true, 3)}), kClassic,
                                   {}, NameSet{"p"});
        }) ==
        "LinkError: no room for `p` (2 bytes) below $80 in the linked program, and its unit "
        "uses it as an `(zp),Y` pointer, which has to be in zero page");
  // The promoted objects are ordered by weight per byte, compared
  // exactly. 2**60 + 1 and 2**60 are one double, so a float key would
  // tie them and put `a` first by name.
  {
    const Int big = Int{1} << 60;
    const auto [addrs, demoted] = linker::place_zp_objects(
        objects({zp("a", 1, {}, true, big), zp("b", 1, {}, true, big + 1)}), kClassic);
    CHECK(names_of(addrs) == Strings{"b=38", "a=39"});
  }

  // A promoted object that doesn't fit goes back in the data segment,
  // and a smaller one after it still fits.
  {
    const auto [addrs, demoted] = linker::place_zp_objects(
        objects({zp("v", 0x59), zp("p", 2, {}, true, 3), zp("q", 1, {}, true, 1)}), kClassic);
    CHECK(names_of(addrs) == Strings{"v=38", "q=127"});
    CHECK(demoted == Strings{"p"});
  }
  // The declared object first, then the promoted ones by weight per
  // byte, highest first, and then the name: `d` (7/3), then `a` (6/3),
  // `b` (2/1), and `c` (4/2), which tie at 2, then `e` (0/1). Byte $27
  // is a parameter's.
  {
    const auto [addrs, demoted] = linker::place_zp_objects(
        objects({zp("c", 2, {}, true, 4), zp("b", 1, {}, true, 2), zp("a", 3, {}, true, 6),
                 zp("d", 3, {}, true, 7), zp("e", 1, {}, true, 0), zp("u", 1)}),
        kClassic, {0x27});
    CHECK(names_of(addrs) == Strings{"u=38", "d=40", "a=43", "b=46", "c=47", "e=49"});
    CHECK(demoted.empty());
  }
  // The largest weight of every unit's records counts, and an object
  // one unit didn't promote isn't promoted.
  {
    linker::ZpObjects zs;
    zs.insert_or_assign("m", std::vector<ZpObjectMeta>{zp("m", 2, {}, false, 5),
                                                       zp("m", 2, {}, true, 9)});
    zs.insert_or_assign("n", std::vector<ZpObjectMeta>{zp("n", 1, {}, true, 3),
                                                       zp("n", 1, {}, false)});
    const auto [addrs, demoted] = linker::place_zp_objects(zs, kClassic);
    CHECK(names_of(addrs) == Strings{"n=38", "m=39"});
  }
}

TEST_CASE("choose_layout and runtime_bytes") {
  LinkMetadata need40;
  need40.hargs_need = 40;
  CHECK(error_of([&] { linker::choose_layout({need40}, {}, 0); }) ==
        "LinkError: the program needs 40 HARGS bytes, and the runtime's block has 32");
  LinkMetadata need2;
  need2.hargs_need = 2;
  CHECK(names_of(linker::choose_layout({need2}, {}, 4)) ==
        Strings{"SSP=0", "FP=2", "HARGS=4", "DPTR=36", "HARGS size=32"});
  // A unit without a need takes the classic block's 32 bytes.
  LinkMetadata need8;
  need8.hargs_need = 8;
  CHECK(names_of(linker::choose_layout({need8, LinkMetadata{}}, objects({zp("v", 2, 0)}), 0)) ==
        Strings{"SSP=34", "FP=36", "HARGS=2", "DPTR=38", "HARGS size=32"});
  CHECK(names_of(linker::choose_layout({need8}, objects({zp("v", 2, 0)}), 0, {0x2A})) ==
        Strings{"SSP=10", "FP=12", "HARGS=2", "DPTR=14", "HARGS size=8"});
  CHECK(error_of([&] { linker::choose_layout({need8}, objects({zp("v", 0x7E, 1)}), 0); }) ==
        "LinkError: no room below $80 for the runtime's HARGS block (8 bytes) around the "
        "reserved zero-page bytes $01-$7E");
  const linker::LayoutDict moved{
      {"SSP", 0x30}, {"FP", 0x32}, {"HARGS", 0x40}, {"DPTR", 0x34}, {"HARGS size", 3}};
  CHECK(linker::runtime_bytes(moved) == std::set<Int>{48, 49, 50, 51, 52, 53, 64, 65, 66});
}

TEST_CASE("link_shares, merge_shares, and check_linked_shares") {
  auto merged_with = [](std::vector<FunctionMeta> defs,
                        std::vector<std::pair<std::string, std::string>> calls = {}) {
    LinkMetadata m = with_defs(std::move(defs));
    m.calls = std::move(calls);
    return m;
  };
  auto unit = [](std::vector<SlotShare> shares, std::vector<ExternMeta> externs = {}) {
    LinkMetadata m;
    m.shares = std::move(shares);
    m.externs = std::move(externs);
    return m;
  };
  CHECK(error_of([&] {
          linker::link_shares(merged_with({def("g", {"g0"}), def("h", {"h0"})}),
                              {unit({share({{"g", {"g0"}}}, {"h"})}),
                               unit({share({{"h", {"h0"}}}, {"g"})}),
                               unit({share({{"h", {"h0"}}, {"g", {"g0"}}})})});
        }) == "LinkError: `g` would keep two values in the parameter bytes `g0` share");
  CHECK(error_of([&] {
          linker::link_shares(merged_with({def("main")}),
                              {unit({share({{"put", {"__zpabi_put__x"}}}, {"main"})})});
        }) ==
        "LinkError: the parameter bytes `__zpabi_put__x` hold a value across calls to `put`, "
        "which no unit defines, so nothing checks that it leaves its `const` parameter alone");
  FunctionMeta put = def("put", {"p0"});
  put.changes = {"p0"};
  CHECK(error_of([&] {
          linker::link_shares(merged_with({put}), {unit({share({{"put", {"p0"}}})})});
        }) ==
        "LinkError: a unit keeps a value in the parameter bytes `p0` across a call to `put` "
        "because its declaration makes the parameter `const`, but `put` changes it");
  SlotShare live = share({{"put", {"p0"}}}, {"main"});
  live.live = {{"main", {"w"}}};
  CHECK(error_of([&] {
          linker::link_shares(
              merged_with({def("main"), def("put", {"p0"}), def("w")},
                          {{"main", "put"}, {"main", "w"}, {"w", "put"}}),
              {unit({live})});
        }) ==
        "LinkError: `main` keeps a value in the parameter bytes `p0` across its call to `w`, "
        "because a declaration makes the parameter `const`, but `w`, which `w` reaches, "
        "writes those bytes; declare the parameter without `const`");
  CHECK(linker::link_shares(merged_with({def("main")}), {unit({})}).empty());

  // A declaration's spelling of the slot joins the definition's share,
  // and the joined share's lists are sorted.
  SlotShare a = share({{"put", {"__zpabi_put__0"}}}, {"main"});
  a.live = {{"main", {"zz", "put"}}};
  a.forwards = {{"main", "put"}};
  a.writers = {"main"};
  SlotShare b = share({{"put", {"__zpabi_put__x"}}}, {"b"});
  b.live = {{"main", {"a"}}, {"b", {"c"}}};
  const auto out = linker::link_shares(
      merged_with({def("put", {"__zpabi_put__x"}), def("main")}, {{"main", "put"}}),
      {unit({a}, {ext("put", {"__zpabi_put__0"})}), unit({b})});
  REQUIRE(out.size() == 1);
  CHECK(out[0].members == std::vector<std::pair<std::string, Strings>>{{"put", {"__zpabi_put__x"}}});
  CHECK(out[0].holders == Strings{"b", "main"});
  CHECK(out[0].live ==
        std::vector<std::pair<std::string, Strings>>{{"b", {"c"}}, {"main", {"a", "put", "zz"}}});
  CHECK(out[0].forwards == std::vector<std::pair<std::string, std::string>>{{"main", "put"}});
  CHECK(out[0].writers == Strings{"main"});

  // Shares with no slot in common stay apart, in the order of their
  // first share.
  const auto two = c6502::passes::merge_shares(
      {share({{"g", {"g0"}}}), share({{"f", {"f0"}}}), share({{"h", {"g0"}}})}, {});
  REQUIRE(two.size() == 2);
  CHECK(two[0].members == std::vector<std::pair<std::string, Strings>>{{"g", {"g0"}}, {"h", {"g0"}}});
  CHECK(two[1].members[0].first == "f");
}

TEST_CASE("bind_extern_params") {
  LinkMetadata merged = with_defs({def("put", {"__zpabi_put__x", "__zpabi_put__y"}, {"", "X"})});
  merged.externs = {ext("put", {"__zpabi_put__0", "__zpabi_put__1"}, {"", "X"}),
                    ext("ext", {"e0"})};
  const linker::SymbolAddrs symbols{{"__zpabi_put__x", 0x80}, {"e0", 0x90}};
  CHECK(error_of([&] {
          auto s = symbols;
          linker::bind_extern_params(merged, {ext("put", {"a"})}, s);
        }) ==
        "LinkError: `put` is declared with 1 zp_abi parameter byte(s) in one unit and has 2 "
        "where it's defined");
  CHECK(error_of([&] {
          auto s = symbols;
          linker::bind_extern_params(merged, {ext("put", {"a", "b"}, {"A", "X"})}, s);
        }) == "LinkError: `put` is declared with its parameters in different registers in two "
              "units");
  CHECK(error_of([&] {
          auto s = symbols;
          s.insert_or_assign("__zpabi_put__0", 0x81);
          linker::bind_extern_params(merged, {ext("put", {"__zpabi_put__0", "b"}, {"", "X"})}, s);
        }) ==
        "LinkError: `__zpabi_put__0` names two different parameter bytes of `put`; give the "
        "parameters the same names in its declarations and its definition");
  auto s = symbols;
  linker::bind_extern_params(merged,
                             {ext("put", {"__zpabi_put__0", "b"}, {"", "X"}), ext("ext", {"e1"}),
                              ext("put", {"__zpabi_put__0", "c"}, {"", "X"})},
                             s);
  CHECK(names_of(s) == Strings{"__zpabi_put__x=128", "e0=144", "__zpabi_put__0=128", "e1=144"});
}

TEST_CASE("drop_unused_locals keeps a pool whose address is taken") {
  elf::ElfFile obj;
  auto sec = std::make_shared<elf::Section>();
  auto sym = [](std::string name) {
    auto s = std::make_shared<elf::Symbol>();
    s->name = std::move(name);
    return s;
  };
  sec->relocs.push_back(elf::Reloc{0, elf::R_MOS_ADDR8, sym("a1"), 0});
  sec->relocs.push_back(elf::Reloc{1, elf::R_MOS_ADDR16_LO, sym("b0"), 0});
  obj.sections.push_back(sec);
  FunctionMeta a = def("a");
  a.locals = {"a0", "a1", "a2"};
  FunctionMeta b = def("b");
  b.locals = {"b0", "b1"};
  LinkMetadata merged = with_defs({a, b});
  linker::drop_unused_locals(merged, {{"x.o", obj}});
  CHECK(merged.defs[0].locals == Strings{"a1"});
  CHECK(merged.defs[1].locals == Strings{"b0", "b1"});
}

TEST_CASE("init_lines and data_lines") {
  CHECK(linker::init_lines({zp("a", 2, {}, true, {}, {"$01", "$00"}),
                            zp("b", 3, {}, true, {}, {"$00", "$07", "$01"})},
                           {"  start"}) ==
        Strings{"__zp_init:", "   SUBROUTINE", "", "  start", "   LDA   #$00", "   STA   a+1",
                "   STA   b", "   LDA   #$01", "   STA   a", "   STA   b+2", "   LDA   #$07",
                "   STA   b+1", "   RTS"});
  CHECK(linker::init_lines({zp("a", 2)}).empty());
  CHECK(linker::data_lines(zp("a", 3, {}, true, {}, {"$00", "$00", "$00"})) ==
        Strings{"a:", "   DS.B  3"});
  CHECK(linker::data_lines(zp("b", 2)) == Strings{"b:", "   DS.B  2"});
  CHECK(linker::data_lines(zp("c", 2, {}, true, {}, {"$01", "$00"})) ==
        Strings{"c:", "   DC.B  $01, $00"});
}

TEST_CASE("carry_conflict") {
  CHECK(linker::carry_conflict("g").rfind("the units disagree on whether `g` returns", 0) == 0);
}

// The cases below are Python unit tests of the linker's helpers, ported
// by hand, one case per test with each of its assertions.

TEST_CASE("linker: test_linker.py::TestZeroPageObjectSizes::test_no_bytes_rejected") {
  for (const Int size : {Int{0}, Int{-1}}) {
    CAPTURE(static_cast<long long>(size));
    CHECK(error_of([&] { linker::merge_zp_objects({with_zp({zp("v", size, {}, true, 1)})}); }) ==
          "LinkError: zero-page object `v` has " + c6502::pyrepr::integer(size) +
              " bytes, where an object has at least one");
  }
}

TEST_CASE("linker: test_carry_return_choice.py::TestLinked::test_units_disagree") {
  // A unit that calls the function expecting its result in A, against
  // one that defines it returning in C, in either order.
  FunctionMeta d = def("is_digit", {"__zpabi_is_digit__c"});
  d.carry = true;
  const LinkMetadata lib = with_defs({d});
  const LinkMetadata main = with_externs({ext("is_digit", {"__zpabi_is_digit__c"})});
  const std::string want = "LinkError: " + linker::carry_conflict("is_digit");
  CHECK(error_of([&] { linker::merge({lib, main}); }) == want);
  CHECK(error_of([&] { linker::merge({main, lib}); }) == want);
}

TEST_CASE("slot_sharing: test_slot_sharing.py::TestAcrossUnits::test_checks") {
  SlotShare s = share({{"add_page", {"__zpabi_add_page__p_0", "__zpabi_add_page__p_1"}}},
                      {"main"});
  s.live = {{"main", {"add_page"}}};
  s.forwards = {{"main", "add_page"}};
  s.writers = {"main"};
  using Calls = std::vector<std::pair<std::string, std::string>>;
  const Calls calls = {{"main", "add_page"}, {"add_page", "helper"}, {"helper", "add_page2"}};
  using Changed = c6502::OrderedMap<std::string, NameSet>;
  CHECK(error_of([&] {
          c6502::passes::check_linked_shares({s}, calls, {"main", "add_page"}, Changed{});
        }) == "no error");
  Changed changed;
  changed.insert_or_assign("add_page", NameSet{"__zpabi_add_page__p_1"});
  CHECK(error_of([&] {
          c6502::passes::check_linked_shares({s}, calls, {"main", "add_page"}, changed);
        }) ==
        "ValueError: a unit keeps a value in the parameter bytes `__zpabi_add_page__p_0` across a "
        "call to `add_page` because its declaration makes the parameter `const`, but `add_page` "
        "changes it");
  CHECK(error_of([&] { c6502::passes::check_linked_shares({s}, calls, {"main"}, Changed{}); }) ==
        "ValueError: the parameter bytes `__zpabi_add_page__p_0` hold a value across calls to "
        "`add_page`, which no unit defines, so nothing checks that it leaves its `const` "
        "parameter alone");
  // `other` calls the owner with something else, and `main` holds its
  // value across a call that reaches `other`.
  Calls more = calls;
  more.emplace_back("helper", "other");
  more.emplace_back("other", "add_page");
  CHECK(error_of([&] {
          c6502::passes::check_linked_shares({s}, more, {"main", "add_page", "helper", "other"},
                                             Changed{});
        }) ==
        "ValueError: `main` keeps a value in the parameter bytes `__zpabi_add_page__p_0` across "
        "its call to `add_page`, because a declaration makes the parameter `const`, but `other`, "
        "which `add_page` reaches, writes those bytes; declare the parameter without `const`");
}
