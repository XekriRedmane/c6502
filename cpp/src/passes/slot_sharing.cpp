// Checks the finished program against the shared parameter slots.
//
// `find_shares` (passes/slot_sharing.py) lets a caller keep a value in a
// callee's parameter bytes across a call, when the callee never assigns
// the parameter. A pass after it that made the callee write those bytes
// would make the caller read a value that changed, so the finished asm
// is checked once for such a write.
#include "passes/slot_sharing.h"

#include <map>
#include <string>

#include "errors.h"
#include "passes/asm_context.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::asm_ast;

// Calls `fn` on each operand that `instr` writes in memory.
template <class Fn>
void memory_writes(const ir::Type_instruction& instr, Fn&& fn) {
  switch (ir::kind(instr)) {
    case ir::Kind_instruction::DecimalArith:
      for (const auto& op : std::get<ir::DecimalArith>(instr).dst) {
        fn(op);
      }
      return;
    case ir::Kind_instruction::Mov:
      fn(std::get<ir::Mov>(instr).dst);
      return;
    case ir::Kind_instruction::Inc:
      fn(std::get<ir::Inc>(instr).dst);
      return;
    case ir::Kind_instruction::Dec:
      fn(std::get<ir::Dec>(instr).dst);
      return;
    case ir::Kind_instruction::ArithmeticShiftLeft:
      fn(std::get<ir::ArithmeticShiftLeft>(instr).dst);
      return;
    case ir::Kind_instruction::LogicalShiftRight:
      fn(std::get<ir::LogicalShiftRight>(instr).dst);
      return;
    case ir::Kind_instruction::RotateLeft:
      fn(std::get<ir::RotateLeft>(instr).dst);
      return;
    case ir::Kind_instruction::RotateRight:
      fn(std::get<ir::RotateRight>(instr).dst);
      return;
    case ir::Kind_instruction::Pop:
      fn(std::get<ir::Pop>(instr).dst);
      return;
    case ir::Kind_instruction::LoadAddress:
      fn(std::get<ir::LoadAddress>(instr).dst);
      return;
    default:
      return;
  }
}

}  // namespace

void check_shares(const asm_ast::Program& prog, const std::vector<SlotShare>& shares) {
  std::map<std::string, NameSet, std::less<>> own;
  for (const auto& share : shares) {
    for (const auto& [f, syms] : share.members) {
      own[f].insert(syms.begin(), syms.end());
    }
  }
  for (const auto& tl : prog.top_level) {
    const auto* fn = asdl::as<ir::Function>(tl);
    if (fn == nullptr) {
      continue;
    }
    const auto it = own.find(fn->name);
    if (it == own.end()) {
      continue;
    }
    for (const auto& instr : fn->instructions) {
      memory_writes(instr, [&](const ir::Type_operand& op) {
        const auto* d = asdl::as<ir::Data>(op);
        if (d != nullptr && it->second.contains(d->name)) {
          throw InternalError("internal error: `" + fn->name + "` writes `" + d->name +
                              "`, a parameter slot that holds a value its callers keep there "
                              "(passes/slot_sharing.py)");
        }
      });
    }
  }
}

}  // namespace c6502::passes
