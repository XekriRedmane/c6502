// The multi-byte INC peephole. The pass isn't ported yet, only the
// operand tests that `carry_inc`, `dec_peephole`, `element_in_place`,
// `redundant_load_after_rmw`, and `shift_in_a` share with it.
#include "passes/inc_peephole.h"

namespace c6502::passes {

namespace ir = ::c6502::asm_ast;
using ::c6502::asdl::as;
using ::c6502::asdl::isa;

bool is_x_indexed(const ir::Type_operand& op) {
  const auto* d = as<ir::IndexedData>(op);
  return d != nullptr && isa<ir::X>(d->index);
}

namespace inc_peephole {

bool is_inc_eligible_operand(const ir::Type_operand& op) {
  if (isa<ir::Data, ir::ZP>(op) || is_x_indexed(op)) {
    return true;
  }
  if (const auto* r = as<ir::Reg>(op)) {
    return isa<ir::X, ir::Y>(r->reg);
  }
  return false;
}

bool operands_equal(const ir::Type_operand& a, const ir::Type_operand& b) {
  if (const auto* da = as<ir::Data>(a)) {
    const auto* db = as<ir::Data>(b);
    return db != nullptr && da->name == db->name && da->offset == db->offset;
  }
  if (const auto* za = as<ir::ZP>(a)) {
    const auto* zb = as<ir::ZP>(b);
    return zb != nullptr && za->address == zb->address && za->offset == zb->offset;
  }
  if (const auto* ra = as<ir::Reg>(a)) {
    const auto* rb = as<ir::Reg>(b);
    return rb != nullptr && ra->reg.index() == rb->reg.index();
  }
  if (const auto* xa = as<ir::IndexedData>(a)) {
    const auto* xb = as<ir::IndexedData>(b);
    return xb != nullptr && xa->name == xb->name && xa->offset == xb->offset &&
           xa->index.index() == xb->index.index();
  }
  return false;
}

}  // namespace inc_peephole

}  // namespace c6502::passes
