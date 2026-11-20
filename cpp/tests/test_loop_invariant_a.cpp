// The tests of tests/test_loop_invariant_a.py, ported.
#include <string>
#include <vector>

#include "asm_ast.h"
#include "doctest.h"
#include "passes/asm_context.h"
#include "passes/loop_invariant_a.h"

namespace ir = c6502::asm_ast;
using c6502::passes::apply_loop_invariant_a;
using c6502::passes::AsmContext;

namespace {

using Instrs = std::vector<ir::Type_instruction>;

const ir::Type_operand kA = ir::Reg{.reg = ir::A{}};
const ir::Type_operand kY = ir::Reg{.reg = ir::Y{}};
const ir::Type_operand kKey = ir::Data{.name = "key", .offset = 0};
const ir::Type_operand kKeyHi = ir::Data{.name = "key", .offset = 1};
const ir::Type_operand kCount = ir::Data{.name = "count", .offset = 0};

ir::Type_operand tab(int base) {
  return ir::IndexedData{.name = "", .offset = base, .index = ir::Y{}};
}

ir::Type_instruction lda(ir::Type_operand src) {
  return ir::Mov{.src = src, .dst = kA, .is_volatile = false};
}

ir::Type_instruction label(const char* name) { return ir::Label{.name = name}; }

Instrs run(Instrs instrs) {
  ir::Program prog{.top_level = {ir::Function{
                       .name = "f", .is_global = true, .instructions = instrs}}};
  prog = apply_loop_invariant_a(std::move(prog), AsmContext{});
  return std::get<ir::Function>(prog.top_level[0]).instructions;
}

// Returns the shape `find_index_of_page_table` compiles to before the
// pass, which compares table bytes against a two-byte key, with
// `extra` at the start of `.next`.
Instrs search_loop(const Instrs& extra = {}) {
  Instrs out = {
      ir::Mov{.src = ir::Imm{.value = 0}, .dst = kY, .is_volatile = false},
      label(".loop"),
      lda(tab(0x2200)),
      ir::Compare{.left = kA, .right = kKey},
      ir::Branch{.cond = ir::NE{}, .target = ".next"},
      lda(tab(0x2280)),
      ir::Compare{.left = kA, .right = kKeyHi},
      ir::Branch{.cond = ir::NE{}, .target = ".next"},
      ir::Mov{.src = kY, .dst = kA, .is_volatile = false},
      ir::Return{.save_a = true},
      label(".next"),
  };
  out.insert(out.end(), extra.begin(), extra.end());
  const Instrs tail = {
      ir::Inc{.dst = kY},
      ir::Dec{.dst = kCount},
      ir::Branch{.cond = ir::NE{}, .target = ".loop"},
      ir::Mov{.src = ir::Imm{.value = 0xFF}, .dst = kA, .is_volatile = false},
      ir::Return{.save_a = true},
  };
  out.insert(out.end(), tail.begin(), tail.end());
  return out;
}

long index_of(const Instrs& instrs, const ir::Type_instruction& ins) {
  for (std::size_t i = 0; i < instrs.size(); ++i) {
    if (instrs[i] == ins) {
      return static_cast<long>(i);
    }
  }
  return -1;
}

}  // namespace

TEST_CASE("loop_invariant_a hoists the key with a reload on the clobbering path") {
  const Instrs out = run(search_loop());
  const long loop = index_of(out, label(".loop"));
  REQUIRE(loop > 0);
  // The key is loaded once before the loop, and the loop compares the
  // table against A.
  CHECK(out[loop - 1] == lda(kKey));
  CHECK(out[loop + 1] == ir::Type_instruction{ir::Compare{.left = kA, .right = tab(0x2200)}});
  // The branch on the high byte's mismatch goes through a reload stub
  // placed after the RTS, which falls into `.next`.
  const long next = index_of(out, label(".next"));
  REQUIRE(next > 1);
  CHECK(out[next - 1] == lda(kKey));
  const auto* stub = std::get_if<ir::Label>(&out[next - 2]);
  REQUIRE(stub != nullptr);
  CHECK(out[loop + 5] ==
        ir::Type_instruction{ir::Branch{.cond = ir::NE{}, .target = stub->name}});
  // The fast path, the low byte's mismatch, has no load.
  CHECK(out[loop + 2] == ir::Type_instruction{ir::Branch{.cond = ir::NE{}, .target = ".next"}});
}

TEST_CASE("loop_invariant_a leaves loops it can't rewrite") {
  SUBCASE("the key written in the loop") {
    const Instrs instrs = search_loop({ir::Inc{.dst = kKey}});
    CHECK(run(instrs) == instrs);
  }
  SUBCASE("a call in the loop") {
    const Instrs instrs = search_loop({ir::Call{.name = "g"}});
    CHECK(run(instrs) == instrs);
  }
  SUBCASE("A read after the compare") {
    // `.next` stores A, so A must still hold the table byte there.
    const Instrs instrs = search_loop(
        {ir::Mov{.src = kA, .dst = ir::Data{.name = "t", .offset = 0}, .is_volatile = false}});
    CHECK(run(instrs) == instrs);
  }
  SUBCASE("C read after the compare") {
    // Swapping the CMP's operands changes C, so a BCC after it keeps
    // the original order.
    const Instrs instrs = search_loop({ir::Branch{.cond = ir::CC{}, .target = ".loop"}});
    CHECK(run(instrs) == instrs);
  }
}
