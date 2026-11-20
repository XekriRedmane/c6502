// The tests of tests/test_dual_index_promotion.py that check the
// pass's instructions, ported. The tests that compile C and run it in
// the simulator stay in Python.
#include <string>
#include <vector>

#include "asm_ast.h"
#include "doctest.h"
#include "passes/dual_index_promotion.h"

namespace ir = c6502::asm_ast;
using c6502::passes::apply_dual_index_promotion;

namespace {

using Instrs = std::vector<ir::Type_instruction>;

const ir::Type_operand kA = ir::Reg{.reg = ir::A{}};
const ir::Type_operand kX = ir::Reg{.reg = ir::X{}};
const ir::Type_operand kY = ir::Reg{.reg = ir::Y{}};

ir::Type_operand data(const std::string& name) { return ir::Data{.name = name, .offset = 0}; }
ir::Type_operand ix_x(const std::string& name) {
  return ir::IndexedData{.name = name, .offset = 0, .index = ir::X{}};
}
ir::Type_operand ix_y(const std::string& name) {
  return ir::IndexedData{.name = name, .offset = 0, .index = ir::Y{}};
}
ir::Type_instruction ldx(ir::Type_operand src) {
  return ir::Mov{.src = src, .dst = kX, .is_volatile = false};
}
ir::Type_instruction ldy(ir::Type_operand src) {
  return ir::Mov{.src = src, .dst = kY, .is_volatile = false};
}
ir::Type_instruction lda(ir::Type_operand src) {
  return ir::Mov{.src = src, .dst = kA, .is_volatile = false};
}
ir::Type_instruction sta(ir::Type_operand dst) {
  return ir::Mov{.src = kA, .dst = dst, .is_volatile = false};
}
ir::Type_instruction label(const char* name) { return ir::Label{.name = name}; }
ir::Type_instruction ret() { return ir::Return{.save_a = false}; }

Instrs run(Instrs instrs) {
  ir::Program prog{.top_level = {ir::Function{
                       .name = "f", .is_global = false, .instructions = instrs}}};
  prog = apply_dual_index_promotion(std::move(prog));
  return std::get<ir::Function>(prog.top_level[0]).instructions;
}

bool contains(const Instrs& instrs, const ir::Type_instruction& ins) {
  for (const auto& i : instrs) {
    if (i == ins) {
      return true;
    }
  }
  return false;
}

// Returns every `IndexedData` operand named `name` in the sources and
// destinations of `instrs`.
std::vector<ir::IndexedData> indexed_named(const Instrs& instrs, const std::string& name) {
  std::vector<ir::IndexedData> out;
  auto take = [&](const ir::Type_operand& op) {
    if (const auto* d = std::get_if<ir::IndexedData>(&op); d != nullptr && d->name == name) {
      out.push_back(*d);
    }
  };
  for (const auto& i : instrs) {
    std::visit(
        [&](const auto& ins) {
          if constexpr (requires { ins.src; }) {
            if constexpr (std::is_same_v<std::remove_cvref_t<decltype(ins.src)>, ir::Type_operand>) {
              take(ins.src);
            }
          }
          if constexpr (requires { ins.dst; }) {
            if constexpr (std::is_same_v<std::remove_cvref_t<decltype(ins.dst)>, ir::Type_operand>) {
              take(ins.dst);
            }
          }
        },
        i);
  }
  return out;
}

// Returns the promotable shape with `middle` between the two uses.
Instrs promotable_around(const Instrs& middle) {
  Instrs out = {label(".entry"), ldx(data("__zpabi_f_p1")), lda(ix_x("rescue")),
                ldx(data("__local_f__1")), lda(ix_x("entity"))};
  out.insert(out.end(), middle.begin(), middle.end());
  out.push_back(ldx(data("__local_f__1")));
  out.push_back(sta(ix_x("entity")));
  out.push_back(ret());
  return out;
}

// Returns `o = t[k]; a = u[d]; if (a) v[d] = a;` followed by
// `after_label` at the join, with d the promoted symbol.
Instrs branch_shape(const Instrs& after_label) {
  Instrs out = {ldx(data("k")),  lda(ix_x("t")), sta(data("o")),
                ldx(data("d")),  lda(ix_x("u")),
                ir::Branch{.cond = ir::EQ{}, .target = ".end"},
                sta(ix_x("v")),  label(".end")};
  out.insert(out.end(), after_label.begin(), after_label.end());
  out.push_back(ret());
  return out;
}

// Returns the shape of the tests of operands that use Y, with `middle`
// between the second and third uses.
Instrs with_middle(const Instrs& middle) {
  Instrs out = {ldx(data("k")), lda(ix_x("t")), ldx(data("d")),
                lda(ix_x("u")), ldx(data("k")), lda(ix_x("t"))};
  out.insert(out.end(), middle.begin(), middle.end());
  out.push_back(ldx(data("d")));
  out.push_back(sta(ix_x("w")));
  out.push_back(ret());
  return out;
}

}  // namespace

TEST_CASE("dual_index_promotion promotes apply_bobble's shape") {
  // Two LDX reloads of __zpabi_f_p0, one on each arm, with an LDX of
  // another parameter at the entry. Both collapse into one LDY at the
  // entry.
  const Instrs instrs = {
      label(".entry"),
      ldx(data("__zpabi_f_p1")),
      lda(ix_x("rescue_bobble")),
      ir::Branch{.cond = ir::PL{}, .target = ".else"},
      ir::And{.src = ir::Imm{.value = 0x7F}, .dst = kA},
      ldx(data("__zpabi_f_p0")),
      ir::ClearCarry{},
      ir::Add{.src = ix_x("entity"), .dst = kA},
      sta(ix_x("entity")),
      ir::Jump{.target = ".end"},
      label(".else"),
      ir::And{.src = ir::Imm{.value = 0x7F}, .dst = kA},
      ldx(data("__zpabi_f_p0")),
      lda(ix_x("entity")),
      ir::SetCarry{},
      ir::Sub{.src = data("temp"), .dst = kA},
      sta(ix_x("entity")),
      label(".end"),
      ret(),
  };
  const Instrs out = run(instrs);
  CHECK(std::holds_alternative<ir::Label>(out[0]));
  CHECK(out[1] == ldy(data("__zpabi_f_p0")));
  CHECK_FALSE(contains(out, ldx(data("__zpabi_f_p0"))));
  const auto entity = indexed_named(out, "entity");
  CHECK(entity.size() > 0);
  for (const auto& op : entity) {
    CHECK(std::holds_alternative<ir::Y>(op.index));
  }
  const auto rescue = indexed_named(out, "rescue_bobble");
  REQUIRE(rescue.size() == 1);
  CHECK(std::holds_alternative<ir::X>(rescue[0].index));
}

TEST_CASE("dual_index_promotion leaves functions that aren't eligible") {
  SUBCASE("a single LDX use") {
    // Promoting would only move the load to LDY.
    const Instrs instrs = {label(".entry"), ldx(data("__zpabi_f_p1")), ldx(data("__zpabi_f_p0")),
                           lda(ix_x("entity")), sta(ix_x("entity")), ret()};
    CHECK(run(instrs) == instrs);
  }
  SUBCASE("Y already used") {
    const Instrs instrs = {label(".entry"),      ldy(ir::Imm{.value = 0}),
                           ldx(data("__zpabi_f_p0")), lda(ix_x("entity")),
                           sta(ix_x("entity")),  ldx(data("__zpabi_f_p0")),
                           lda(ix_x("entity")),  sta(ix_x("entity")),
                           ret()};
    CHECK(run(instrs) == instrs);
  }
  SUBCASE("no other user of X") {
    // X could hold p0 throughout, so Y gains nothing.
    const Instrs instrs = {label(".entry"),      ldx(data("__zpabi_f_p0")),
                           lda(ix_x("entity")),  ldx(data("__zpabi_f_p0")),
                           sta(ix_x("entity")),  ret()};
    CHECK(run(instrs) == instrs);
  }
  SUBCASE("an INC of an element indexed by X") {
    // The 6502 has no `INC abs,Y`.
    const Instrs instrs = {label(".entry"),
                           ldx(data("__zpabi_f_p1")),
                           lda(ix_x("rescue")),
                           ldx(data("__zpabi_f_p0")),
                           ir::Inc{.dst = ix_x("entity")},
                           ldx(data("__zpabi_f_p0")),
                           lda(ix_x("entity")),
                           ret()};
    CHECK(run(instrs) == instrs);
  }
  SUBCASE("the promotable shape promotes") {
    const Instrs instrs = promotable_around({});
    CHECK(run(instrs) != instrs);
  }
  SUBCASE("a write of the symbol") {
    // Y, loaded once at the entry, would hold the value from before the
    // store.
    const Instrs instrs = promotable_around({sta(data("__local_f__1"))});
    CHECK(run(instrs) == instrs);
  }
  SUBCASE("a call") {
    // A callee may change Y before the second use.
    const Instrs instrs = promotable_around({ir::Call{.name = "g"}});
    CHECK(run(instrs) == instrs);
  }
}

TEST_CASE("dual_index_promotion rewrites only where X holds the symbol") {
  SUBCASE("the rewrite stops at a reload of X") {
    const Instrs instrs = {label(".entry"),       ldx(data("OTHER1")),
                           lda(ix_x("any")),      ldx(data("__zpabi_f_p0")),
                           lda(ix_x("entity")),   ldx(data("OTHER2")),
                           lda(ix_x("other_arr")), ldx(data("__zpabi_f_p0")),
                           sta(ix_x("entity")),   ret()};
    const Instrs out = run(instrs);
    const auto other = indexed_named(out, "other_arr");
    REQUIRE(other.size() == 1);
    CHECK(std::holds_alternative<ir::X>(other[0].index));
    const auto entity = indexed_named(out, "entity");
    REQUIRE(entity.size() == 2);
    for (const auto& op : entity) {
      CHECK(std::holds_alternative<ir::Y>(op.index));
    }
  }
  SUBCASE("a read after a branch follows X") {
    // `STA v,X` comes after the `BEQ`, and X still holds d there. The
    // rewrite once stopped at the branch and left `v,X` reading X,
    // which no longer held d once the `LDX d` was dropped.
    const Instrs out = run(branch_shape({ldx(data("k")), lda(ix_x("t")), sta(data("o")),
                                         ldx(data("d")), lda(ix_x("w")), sta(data("o2"))}));
    CHECK(contains(out, sta(ix_y("v"))));
    CHECK(contains(out, lda(ix_y("u"))));
    CHECK(contains(out, lda(ix_y("w"))));
    const auto t = indexed_named(out, "t");
    REQUIRE(t.size() == 2);
    for (const auto& op : t) {
      CHECK(std::holds_alternative<ir::X>(op.index));
    }
  }
  SUBCASE("a read where X may or may not hold d") {
    // At `.end`, X holds d on the taken path and k on the fall-through
    // path, so `LDA w,X` has no one register to read.
    const Instrs instrs = {ldx(data("d")),
                           lda(ix_x("u")),
                           ir::Branch{.cond = ir::EQ{}, .target = ".end"},
                           ldx(data("k")),
                           lda(ix_x("t")),
                           label(".end"),
                           lda(ix_x("w")),
                           ldx(data("d")),
                           sta(ix_x("v")),
                           ret()};
    CHECK(run(instrs) == instrs);
  }
  SUBCASE("a read of X's value") {
    // `TXA` reads d's value from X, which won't hold it.
    const Instrs instrs = branch_shape(
        {ldx(data("d")), ir::Mov{.src = kX, .dst = kA, .is_volatile = false}, sta(data("o2"))});
    CHECK(run(instrs) == instrs);
  }
  SUBCASE("live flags after the LDX") {
    // The `BEQ` reads the Z flag of `LDX d`, which would be dropped.
    const Instrs instrs = {ldx(data("k")),
                           lda(ix_x("t")),
                           ldx(data("d")),
                           ir::Branch{.cond = ir::EQ{}, .target = ".end"},
                           lda(ix_x("u")),
                           label(".end"),
                           ldx(data("d")),
                           sta(ix_x("v")),
                           ret()};
    CHECK(run(instrs) == instrs);
  }
}

TEST_CASE("dual_index_promotion counts the operands that use Y") {
  SUBCASE("the control promotes") {
    const Instrs instrs = with_middle({});
    CHECK(run(instrs) != instrs);
  }
  SUBCASE("an IndirectZp operand") {
    // `LDA (p),Y` on a zero-page pointer is written `LDY #$00; LDA
    // (p),Y`, which overwrites d in Y.
    const Instrs instrs = with_middle({lda(ir::IndirectZp{.address = 0x80, .offset = 0})});
    CHECK(run(instrs) == instrs);
  }
  SUBCASE("an IndirectZpY operand") {
    const Instrs instrs = with_middle({lda(ir::IndirectZpY{.address = 0x80})});
    CHECK(run(instrs) == instrs);
  }
}
