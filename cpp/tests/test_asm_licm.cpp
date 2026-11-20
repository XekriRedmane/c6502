// The tests of tests/test_asm_licm.py, ported.
#include <algorithm>
#include <cstddef>
#include <string>
#include <vector>

#include "asm_ast.h"
#include "doctest.h"
#include "passes/asm_context.h"
#include "passes/asm_licm.h"

namespace ir = c6502::asm_ast;
using c6502::asdl::as;
using c6502::passes::apply_licm;
using c6502::passes::AsmContext;

namespace {

using Instrs = std::vector<ir::Type_instruction>;

const ir::Type_operand kA = ir::Reg{.reg = ir::A{}};

ir::Type_operand data(const char* name) { return ir::Data{.name = name}; }

ir::Type_instruction mov(ir::Type_operand src, ir::Type_operand dst,
                         bool is_volatile = false) {
  return ir::Mov{.src = src, .dst = dst, .is_volatile = is_volatile};
}

ir::Type_instruction imm_mov(int value, ir::Type_operand dst) {
  return mov(ir::Imm{.value = value}, dst);
}

Instrs run(const Instrs& instrs) {
  ir::Program prog{.top_level = {ir::Function{
                       .name = "f", .is_global = false, .instructions = instrs}}};
  prog = apply_licm(std::move(prog), AsmContext{});
  return std::get<ir::Function>(prog.top_level[0]).instructions;
}

std::size_t label_index(const Instrs& instrs, const std::string& name) {
  for (std::size_t i = 0; i < instrs.size(); ++i) {
    const auto* label = as<ir::Label>(instrs[i]);
    if (label != nullptr && label->name == name) {
      return i;
    }
  }
  FAIL("no label " << name);
  return 0;
}

// Returns whether a Mov in `instrs[begin, end)` writes the Data cell
// `name`.
bool stores_to(const Instrs& instrs, std::size_t begin, std::size_t end,
               const std::string& name) {
  for (std::size_t i = begin; i < end; ++i) {
    const auto* m = as<ir::Mov>(instrs[i]);
    if (m == nullptr) {
      continue;
    }
    const auto* d = as<ir::Data>(m->dst);
    if (d != nullptr && d->name == name) {
      return true;
    }
  }
  return false;
}

}  // namespace

TEST_CASE("licm hoists a constant store pair to the preheader") {
  // LDA #c; STA M inside a loop body, with no other write of M in the
  // loop and no Call, goes to just before the loop header.
  const Instrs instrs = {
      ir::Label{.name = ".prehdr"},
      ir::Label{.name = ".loop_start"},
      imm_mov(0x42, kA),
      mov(kA, data("slot")),
      mov(data("counter"), kA),
      ir::Compare{.left = kA, .right = ir::Imm{.value = 0}},
      ir::Branch{.cond = ir::NE{}, .target = ".loop_start"},
      ir::Return{},
  };
  const Instrs out = run(instrs);
  const std::size_t loop = label_index(out, ".loop_start");
  CHECK_FALSE(stores_to(out, loop + 1, out.size(), "slot"));
  CHECK(stores_to(out, 0, loop, "slot"));
}

TEST_CASE("licm leaves a loop with a call alone") {
  // Any Call in the loop body disqualifies the hoist, because the
  // callee might write the dst.
  const Instrs instrs = {
      ir::Label{.name = ".loop_start"},
      imm_mov(0x42, kA),
      mov(kA, data("slot")),
      ir::Call{.name = "helper"},
      mov(data("counter"), kA),
      ir::Compare{.left = kA, .right = ir::Imm{.value = 0}},
      ir::Branch{.cond = ir::NE{}, .target = ".loop_start"},
      ir::Return{},
  };
  CHECK(run(instrs) == instrs);
}

TEST_CASE("licm leaves a store whose dst the loop writes again") {
  const Instrs instrs = {
      ir::Label{.name = ".loop_start"},
      imm_mov(0x42, kA),
      mov(kA, data("slot")),
      mov(data("other"), kA),
      mov(kA, data("slot")),
      ir::Branch{.cond = ir::NE{}, .target = ".loop_start"},
      ir::Return{},
  };
  CHECK(run(instrs) == instrs);
}

TEST_CASE("licm hoists a constant store to a ZP byte") {
  const Instrs instrs = {
      ir::Label{.name = ".loop_start"},
      imm_mov(0x07, ir::ZP{.address = 0x90, .offset = 0}),
      mov(data("counter"), kA),
      ir::Branch{.cond = ir::NE{}, .target = ".loop_start"},
      ir::Return{},
  };
  const Instrs out = run(instrs);
  const std::size_t loop = label_index(out, ".loop_start");
  const bool hoisted = std::any_of(
      out.begin(), out.begin() + static_cast<std::ptrdiff_t>(loop),
      [](const ir::Type_instruction& i) {
        const auto* m = as<ir::Mov>(i);
        return m != nullptr && c6502::asdl::isa<ir::Imm>(m->src) &&
               c6502::asdl::isa<ir::ZP>(m->dst);
      });
  CHECK(hoisted);
}

TEST_CASE("licm leaves a loop with a side entry alone") {
  // A label inside the loop body that a jump outside it names is a side
  // entry, which would bypass the hoisted store.
  const Instrs instrs = {
      ir::Jump{.target = ".side_entry"},
      ir::Label{.name = ".loop_start"},
      imm_mov(0x42, kA),
      mov(kA, data("slot")),
      ir::Label{.name = ".side_entry"},
      mov(data("counter"), kA),
      ir::Branch{.cond = ir::NE{}, .target = ".loop_start"},
      ir::Return{},
  };
  CHECK(run(instrs) == instrs);
}

TEST_CASE("licm hoists a copy of a stable byte") {
  // `LDA src; STA dst` where nothing in the loop writes src, as in the
  // DPTR staging for a volatile pointer dereference in an outer loop.
  const Instrs instrs = {
      ir::Label{.name = ".loop_start"},
      mov(data("sfx_click_ptr"), kA),
      mov(kA, data("DPTR")),
      mov(data("counter"), kA),
      ir::Compare{.left = kA, .right = ir::Imm{.value = 0}},
      ir::Branch{.cond = ir::NE{}, .target = ".loop_start"},
      ir::Return{},
  };
  const Instrs out = run(instrs);
  const std::size_t loop = label_index(out, ".loop_start");
  const bool hoisted = std::any_of(
      out.begin(), out.begin() + static_cast<std::ptrdiff_t>(loop),
      [](const ir::Type_instruction& i) {
        const auto* m = as<ir::Mov>(i);
        const auto* d = m != nullptr ? as<ir::Data>(m->src) : nullptr;
        return d != nullptr && d->name == "sfx_click_ptr";
      });
  CHECK(hoisted);
  CHECK_FALSE(stores_to(out, loop + 1, out.size(), "DPTR"));
}

TEST_CASE("licm leaves a copy whose source the loop writes") {
  // The `Inc(ptr)` changes the source on each iteration, and LICM can't
  // hoist it itself, so the test isolates the refusal.
  const Instrs instrs = {
      ir::Label{.name = ".loop_start"},
      mov(data("ptr"), kA),
      mov(kA, data("DPTR")),
      ir::Inc{.dst = data("ptr")},
      ir::Branch{.cond = ir::NE{}, .target = ".loop_start"},
      ir::Return{},
  };
  CHECK(run(instrs) == instrs);
}

TEST_CASE("licm leaves a volatile move in the loop") {
  // A volatile access happens on every iteration.
  const Instrs instrs = {
      ir::Label{.name = ".loop_start"},
      mov(data("ptr"), kA, true),
      mov(kA, data("DPTR")),
      ir::Branch{.cond = ir::NE{}, .target = ".loop_start"},
      ir::Return{},
  };
  CHECK(run(instrs) == instrs);
}

TEST_CASE("licm leaves a store whose dst the header reads first") {
  // `for (p = &x; p != 0; ) { ...; p = 0; }`: the header reads `p`
  // before the body's store, so the first iteration has to see the
  // value from before the loop.
  const ir::Type_operand p = data("p");
  const Instrs instrs = {
      imm_mov(7, p),
      ir::Label{.name = ".loop_start"},
      mov(p, kA),
      ir::Branch{.cond = ir::EQ{}, .target = ".loop_break"},
      imm_mov(0, p),
      ir::Jump{.target = ".loop_start"},
      ir::Label{.name = ".loop_break"},
      ir::Return{},
  };
  CHECK(run(instrs) == instrs);
}

TEST_CASE("licm leaves a store that an exit before it can skip") {
  // The loop can exit before the store runs, and `g` is read after the
  // loop, so hoisting would change what that read sees when the loop
  // runs zero times.
  const ir::Type_operand g = data("g");
  const Instrs instrs = {
      ir::Label{.name = ".loop_start"},
      mov(data("n"), kA),
      ir::Branch{.cond = ir::EQ{}, .target = ".loop_break"},
      imm_mov(5, g),
      ir::Dec{.dst = data("n")},
      ir::Jump{.target = ".loop_start"},
      ir::Label{.name = ".loop_break"},
      mov(g, kA),
      ir::Return{.save_a = true},
  };
  CHECK(run(instrs) == instrs);
}
