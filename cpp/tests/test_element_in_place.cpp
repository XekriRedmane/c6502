// The tests of `element_in_place` in tests/test_element_rmw.py
// (TestElementInPlace), ported.
#include <memory>
#include <vector>

#include "asm_ast.h"
#include "doctest.h"
#include "passes/asm_context.h"
#include "passes/element_in_place.h"

namespace ir = c6502::asm_ast;
using c6502::passes::apply_element_in_place;
using c6502::passes::AsmContext;
using c6502::passes::SymbolAddrs;
using c6502::passes::UserZpGlobals;

namespace {

using Instrs = std::vector<ir::Type_instruction>;

const ir::Type_operand kA = ir::Reg{.reg = ir::A{}};
const ir::Type_operand kX = ir::Reg{.reg = ir::X{}};
const ir::Type_operand kE = ir::IndexedData{.name = "tab", .offset = 0, .index = ir::X{}};
const ir::Type_operand kT = ir::Data{.name = "__local_f__t", .offset = 0};
const ir::Type_instruction kRts = ir::Return{};
const SymbolAddrs kSlots = {{"__local_f__t", 0x90}};

ir::Type_instruction mov(ir::Type_operand src, ir::Type_operand dst, bool is_volatile = false) {
  return ir::Mov{.src = src, .dst = dst, .is_volatile = is_volatile};
}

ir::Type_operand data(const char* name) { return ir::Data{.name = name, .offset = 0}; }

ir::Type_operand imm(int v) { return ir::Imm{.value = v}; }

ir::Type_instruction branch(ir::Type_condition cond, const char* target) {
  return ir::Branch{.cond = cond, .target = target};
}

Instrs run(Instrs instrs, const SymbolAddrs* slots = &kSlots, const AsmContext& ctx = {}) {
  ir::Program prog{.top_level = {ir::Function{
                       .name = "f", .is_global = true, .instructions = instrs}}};
  prog = apply_element_in_place(std::move(prog), ctx, slots);
  return std::get<ir::Function>(prog.top_level[0]).instructions;
}

// `handle_timers`'s shape, with `between` the instructions between the
// site and the compare's load of the slot.
Instrs timer(const Instrs& between) {
  Instrs out = {
      mov(kE, kA), mov(kA, kT), branch(ir::NE{}, ".skip"), kRts,
      ir::Label{.name = ".skip"}, ir::Dec{.dst = kT}, mov(kT, kA), mov(kA, kE),
      branch(ir::EQ{}, ".done"),
  };
  out.insert(out.end(), between.begin(), between.end());
  const Instrs tail = {
      mov(kT, kA),
      ir::Compare{.left = kA, .right = imm(20)},
      branch(ir::NE{}, ".done"),
      mov(imm(1), data("hit")),
      ir::Label{.name = ".done"},
      kRts,
  };
  out.insert(out.end(), tail.begin(), tail.end());
  return out;
}

}  // namespace

TEST_CASE("element_in_place steps the element a slot copies") {
  const Instrs out = run(timer({}));
  CHECK(Instrs(out.begin() + 5, out.begin() + 8) ==
        Instrs{ir::Dec{.dst = kE}, mov(kE, kA), branch(ir::EQ{}, ".done")});
  CHECK(out[8] == mov(kE, kA));
  for (const auto& ins : out) {
    if (const auto* m = std::get_if<ir::Mov>(&ins)) {
      CHECK(m->src != kT);
    }
  }
}

TEST_CASE("element_in_place passes a store to a zero-page object") {
  AsmContext ctx;
  auto user = std::make_shared<UserZpGlobals>();
  user->addrs = {{"col", 0x85}};
  user->bytes = {0x85};
  ctx.user_zp = user;
  const Instrs out = run(timer({mov(imm(3), data("col"))}), &kSlots, ctx);
  bool found = false;
  for (const auto& ins : out) {
    found = found || ins == ir::Type_instruction{ir::Dec{.dst = kE}};
  }
  CHECK(found);
}

TEST_CASE("element_in_place leaves the slot alone") {
  SUBCASE("a store to another absolute object between") {
    // `other` might overlap `tab` at a fixed address.
    const Instrs instrs = timer({mov(imm(3), data("other"))});
    CHECK(run(instrs) == instrs);
  }
  SUBCASE("the index changed between") {
    const Instrs instrs = timer({mov(imm(3), kX)});
    CHECK(run(instrs) == instrs);
  }
  SUBCASE("a call between") {
    const Instrs instrs = timer({ir::Call{.name = "g"}});
    CHECK(run(instrs) == instrs);
  }
  SUBCASE("a read into X") {
    // There's no `LDX abs,X`.
    const Instrs instrs = timer({mov(kT, kX)});
    CHECK(run(instrs) == instrs);
  }
  SUBCASE("a slot that isn't a copy of the element") {
    Instrs instrs = timer({});
    instrs[1] = mov(imm(4), kT);
    CHECK(run(instrs) == instrs);
  }
  SUBCASE("a volatile element") {
    Instrs instrs = timer({});
    instrs[0] = mov(kE, kA, true);
    CHECK(run(instrs) == instrs);
  }
  SUBCASE("no slot map") {
    const Instrs instrs = timer({});
    CHECK(run(instrs, nullptr) == instrs);
  }
}
