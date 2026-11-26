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
