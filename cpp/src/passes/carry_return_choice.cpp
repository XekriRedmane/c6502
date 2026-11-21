// Returns a `bool` in the carry flag without the annotation, when every
// call to the function gains by it.
//
// `[[c6502::return_bool_in_carry]]` makes a function return `e != 0` in
// C, and a caller then branches on it with `BCC` / `BCS`. A function
// returning its `bool` in A leaves the flags as whatever its last
// instruction set, so a caller that branches on the result tests it
// first, `ORA #$00` or `CMP #$00`, 2 bytes and 2 cycles. But a caller
// that keeps the result as a value pays for the carry, `LDA #0; ROL A`,
// 3 bytes, where the value in A costs nothing. So the compiler chooses
// the carry itself only for a function whose every call it can see and
// whose every call uses the result in a way the carry suits.
//
// Eligibility
//
// `choose_carry_returns` picks each function that
//
// - returns `bool` (`FunAttr::returns_bool`), has internal linkage, and
//   is defined in this unit, so that every call to it is here;
// - isn't already declared `return_bool_in_carry`, has no address, and
//   never has its address taken, since an indirect call can't know the
//   result is in C;
// - uses each call's result in one of three ways: not at all, as the
//   condition of a `JumpIfTrue` / `JumpIfFalse` right after the call
//   (past any label nothing jumps to), or as the value of a `Ret` right
//   after it in a function that returns in C too, chosen here, by
//   `choose_global_carry_returns`, or by the annotation, which then
//   passes the carry on;
// - and uses at least one call's result in one of the last two ways.
//
// The conditions are decided for all the candidates together, since
// dropping one can drop the candidates whose result it returns.
//
// Functions with external linkage
//
// Another unit can call a function with external linkage, so the unit
// that defines it can't see every call, and a unit that calls it can't
// see the others. Each unit has to make the same choice from what they
// all see, which is the declaration. `choose_global_carry_returns` gives
// the carry return to every function that
//
// - returns `bool`, has external linkage, and has no fixed address
//   (`[[c6502::address]]`, whose convention its declaration gives);
// - isn't already declared `return_bool_in_carry`;
// - and is zp_abi in this unit (a `ZpLayout` in `select_abi`'s result),
//   which also means its address isn't taken here.
//
// The unit that defines the function and every unit that calls it each
// record the choice in the link metadata (`carry=true` on the `def` and
// `ext` lines, from `zp_link_metadata`), and the linker rejects units
// that disagree. They can disagree when one unit takes the function's
// address, or when the defining unit can't give it zp_abi.
//
// A caller that branches on the result saves its `ORA #$00`, and the
// function itself usually gets shorter, since a comparison it returns
// leaves its answer in C. A caller that keeps the result as a value
// pays `LDA #0; ROL A`.
//
// Where to run
//
// On the optimized TAC, before `tac_to_asm`, which lowers the returns of
// a function whose `FunAttr::carry_return` is set, and the calls to it,
// the way it does for the annotation. `choose_global_carry_returns` runs
// right after `select_abi`, and `choose_carry_returns` after it, so that
// a static function can pass its carry through a function with external
// linkage. Only under `-O`.
#include "passes/carry_return_choice.h"

#include <map>
#include <string>
#include <utility>
#include <vector>

#include "asdl/runtime.h"
#include "passes/optimization/var_visit.h"

namespace c6502::passes {

namespace tac = ::c6502::tac_ast;
using ::c6502::asdl::as;

namespace {

enum class Use { kNone, kBranch, kReturn, kOther };

// Returns every name the program uses as a value or in a static's
// initializer. A function's name is a value only where its address is
// taken.
NameSet taken_names(const tac::Program& prog) {
  NameSet out;
  for (const auto& tl : prog.top_level) {
    if (const auto* fn = std::get_if<tac::Function>(&tl)) {
      for (const auto& ins : fn->instructions) {
        for (const auto u : uses_in(ins)) {
          out.emplace(u);
        }
        if (const auto* get = as<tac::GetAddress>(ins)) {
          if (const auto* var = std::get_if<tac::Var>(&get->operand)) {
            out.insert(var->name);
          }
        }
      }
    } else {
      for (const auto& init : std::get<tac::StaticVariable>(tl).init) {
        if (const auto* a = std::get_if<tac::AddressInit>(&init)) {
          out.insert(a->name);
        } else if (const auto* b = std::get_if<tac::AddressByteInit>(&init)) {
          out.insert(b->name);
        }
      }
    }
  }
  return out;
}

// Returns the labels some jump in `instrs` goes to.
NameSet jump_targets(const std::vector<tac::Type_instruction>& instrs) {
  NameSet out;
  for (const auto& ins : instrs) {
    switch (tac::kind(ins)) {
      case tac::Kind_instruction::Jump:
        out.insert(std::get<tac::Jump>(ins).target);
        break;
      case tac::Kind_instruction::JumpIfTrue:
        out.insert(std::get<tac::JumpIfTrue>(ins).target);
        break;
      case tac::Kind_instruction::JumpIfFalse:
        out.insert(std::get<tac::JumpIfFalse>(ins).target);
        break;
      case tac::Kind_instruction::JumpIfCmp:
        out.insert(std::get<tac::JumpIfCmp>(ins).target);
        break;
      case tac::Kind_instruction::JumpIfMasked:
        out.insert(std::get<tac::JumpIfMasked>(ins).target);
        break;
      default:
        break;
    }
  }
  return out;
}

// Returns the first instruction from `instrs[k]` on that isn't a label
// nothing jumps to, such as one SSA left between a call and the jump
// that tests its result, or null at the end.
const tac::Type_instruction* next_instruction(const std::vector<tac::Type_instruction>& instrs,
                                              std::size_t k, const NameSet& targets) {
  for (; k < instrs.size(); ++k) {
    const auto* label = as<tac::Label>(instrs[k]);
    if (label == nullptr || targets.count(label->name) != 0) {
      return &instrs[k];
    }
  }
  return nullptr;
}

// Returns how `call`'s result is used: not at all, tested by a jump
// right after the call, returned right after it, or otherwise.
Use use_of_result(const tac::FunctionCall& call, const tac::Type_instruction* nxt,
                  const OrderedMap<std::string, std::size_t>& counts) {
  if (!call.dst) {
    return Use::kNone;
  }
  const auto* dst = std::get_if<tac::Var>(&*call.dst);
  if (dst == nullptr) {
    return Use::kOther;
  }
  const std::size_t* n = counts.find(dst->name);
  if (n == nullptr || *n == 0) {
    return Use::kNone;
  }
  if (*n != 1) {
    return Use::kOther;
  }
  if (nxt != nullptr) {
    if (const auto* jt = as<tac::JumpIfTrue>(*nxt); jt != nullptr && jt->condition == *call.dst) {
      return Use::kBranch;
    }
    if (const auto* jf = as<tac::JumpIfFalse>(*nxt); jf != nullptr && jf->condition == *call.dst) {
      return Use::kBranch;
    }
    if (const auto* ret = as<tac::Ret>(*nxt); ret != nullptr && ret->val == call.dst) {
      return Use::kReturn;
    }
  }
  return Use::kOther;
}

}  // namespace

NameSet choose_carry_returns(const tac::Program& prog, SymbolTable& symbols) {
  std::vector<const tac::Function*> functions;
  for (const auto& tl : prog.top_level) {
    if (const auto* fn = std::get_if<tac::Function>(&tl)) {
      functions.push_back(fn);
    }
  }
  const NameSet taken = taken_names(prog);
  // Functions that already return in C, by the annotation or by
  // `choose_global_carry_returns`. A call returned by one of them passes
  // its carry on, as one returned by a chosen function does.
  NameSet carrying;
  for (const auto& [name, sym] : symbols) {
    if (const auto* attrs = std::get_if<FunAttr>(&sym.attrs); attrs != nullptr && attrs->carry_return) {
      carrying.insert(name);
    }
  }
  NameSet candidates;
  for (const auto* fn : functions) {
    const Symbol* sym = symbols.find(fn->name);
    if (sym == nullptr) {
      continue;
    }
    const auto* attrs = std::get_if<FunAttr>(&sym->attrs);
    if (attrs != nullptr && attrs->returns_bool && !attrs->is_global && !attrs->carry_return &&
        !attrs->address && taken.count(fn->name) == 0) {
      candidates.insert(fn->name);
    }
  }
  if (candidates.empty()) {
    return {};
  }
  // Each call to a candidate: the function it's in, and how its result
  // is used.
  std::map<std::string, std::vector<std::pair<std::string, Use>>, std::less<>> calls;
  for (const auto& n : candidates) {
    calls[n];
  }
  for (const auto* fn : functions) {
    const auto counts = count_uses(fn->instructions);
    const auto& instrs = fn->instructions;
    const NameSet targets = jump_targets(instrs);
    for (std::size_t i = 0; i < instrs.size(); ++i) {
      const auto* call = as<tac::FunctionCall>(instrs[i]);
      if (call == nullptr || candidates.count(call->name) == 0) {
        continue;
      }
      calls[call->name].emplace_back(
          fn->name, use_of_result(*call, next_instruction(instrs, i + 1, targets), counts));
    }
  }
  // Start from every candidate whose calls all suit the carry, and drop
  // those that don't gain, until nothing changes: a candidate stays when
  // each call's result is unused, branched on, or returned by a function
  // that stays, and at least one call is branched on or returned that
  // way. Each drop only makes the others' conditions harder, so the
  // order of the drops doesn't change the result, which is the largest
  // set that meets them.
  NameSet chosen;
  for (const auto& n : candidates) {
    bool all_suit = true;
    for (const auto& [caller, use] : calls[n]) {
      all_suit = all_suit && use != Use::kOther;
    }
    if (all_suit) {
      chosen.insert(n);
    }
  }
  bool changed = true;
  while (changed) {
    changed = false;
    const NameSet snapshot = chosen;
    for (const auto& name : snapshot) {
      NameSet passes_on = chosen;
      passes_on.insert(carrying.begin(), carrying.end());
      bool fits = true;
      bool gains = false;
      for (const auto& [caller, use] : calls[name]) {
        const bool returned = use == Use::kReturn && passes_on.count(caller) != 0;
        fits = fits && (use == Use::kNone || use == Use::kBranch || returned);
        gains = gains || use == Use::kBranch || returned;
      }
      if (!(fits && gains)) {
        chosen.erase(name);
        changed = true;
      }
    }
  }
  for (const auto& name : chosen) {
    std::get<FunAttr>(symbols.find(name)->attrs).carry_return = true;
  }
  return chosen;
}

NameSet choose_global_carry_returns(const tac::Program& prog, SymbolTable& symbols, const Abi& abi) {
  const NameSet taken = taken_names(prog);
  NameSet chosen;
  for (auto& [name, sym] : symbols) {
    auto* attrs = std::get_if<FunAttr>(&sym.attrs);
    if (attrs == nullptr || !attrs->returns_bool || !attrs->is_global || attrs->carry_return ||
        attrs->address || taken.count(name) != 0) {
      continue;
    }
    const ParamLayout* layout = abi.find(name);
    if (layout == nullptr || !std::holds_alternative<ZpLayout>(*layout)) {
      continue;
    }
    attrs->carry_return = true;
    chosen.insert(name);
  }
  return chosen;
}

}  // namespace c6502::passes
