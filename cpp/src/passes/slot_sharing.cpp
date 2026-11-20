// The ported parts of passes/slot_sharing.py: the call-graph reach that
// the zero-page allocators share, `SlotShare`'s methods, and the check
// of the finished program against the shared parameter slots.
//
// `find_shares` lets a caller keep a value in a callee's parameter bytes
// across a call, when the callee never assigns the parameter. A pass
// after it that made the callee write those bytes would make the caller
// read a value that changed, so the finished asm is checked once for
// such a write.
#include "passes/slot_sharing.h"

#include <map>
#include <string>

#include "errors.h"
#include "passes/asm_context.h"
#include "passes/call_edges.h"

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

ReachSets reach_sets(const tac_ast::Program& prog) {
  OrderedMap<std::string, NameSet> direct;
  for (const auto& tl : prog.top_level) {
    if (const auto* fn = std::get_if<tac_ast::Function>(&tl)) {
      NameSet callees;
      for (const auto* call : graph_calls(*fn)) {
        callees.insert(call->name);
      }
      direct.insert_or_assign(fn->name, std::move(callees));
    }
  }
  // The closure, by a walk from each function. A set's order doesn't
  // reach the result, which is a set too.
  ReachSets out;
  for (const auto& [name, callees] : direct) {
    NameSet seen;
    std::vector<std::string> stack(callees.begin(), callees.end());
    while (!stack.empty()) {
      std::string c = std::move(stack.back());
      stack.pop_back();
      if (!seen.insert(c).second) {
        continue;
      }
      if (const NameSet* next = direct.find(c)) {
        stack.insert(stack.end(), next->begin(), next->end());
      }
    }
    out.insert_or_assign(name, std::move(seen));
  }
  return out;
}

bool coexist(std::string_view a, std::string_view b, const ReachSets& reach) {
  if (a == b) {
    return true;
  }
  const NameSet* from_a = reach.find(a);
  if (from_a != nullptr && from_a->find(b) != from_a->end()) {
    return true;
  }
  const NameSet* from_b = reach.find(b);
  return from_b != nullptr && from_b->find(a) != from_b->end();
}

std::vector<std::size_t> share_conflicts(std::string_view fn, const std::vector<SlotShare>& shares,
                                         const ReachSets& reach) {
  std::vector<std::size_t> out;
  for (std::size_t i = 0; i < shares.size(); ++i) {
    for (const auto& f : shares[i].functions()) {
      if (coexist(fn, f, reach)) {
        out.push_back(i);
        break;
      }
    }
  }
  return out;
}

std::size_t SlotShare::width() const { return members.at(0).second.size(); }

NameSet SlotShare::functions() const {
  NameSet out = owners();
  out.insert(holders.begin(), holders.end());
  return out;
}

NameSet SlotShare::owners() const {
  NameSet out;
  for (const auto& [fn, syms] : members) {
    out.insert(fn);
  }
  return out;
}

NameSet SlotShare::slots() const {
  NameSet out;
  for (const auto& [fn, syms] : members) {
    out.insert(syms.begin(), syms.end());
  }
  return out;
}

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
