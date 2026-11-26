#include "passes/optimization/framework.h"

#include <stdexcept>

namespace c6502::passes::framework {

namespace ir = ::c6502::tac_ast;
using K = ir::Kind_instruction;

ir::Function with_instructions(const ir::Function& fn, Instrs instructions) {
  return ir::Function{
      .name = fn.name,
      .is_global = fn.is_global,
      .params = fn.params,
      .instructions = std::move(instructions),
  };
}

std::optional<std::string_view> dst_name(const ir::Type_instruction& instr) {
  const ir::Type_val* dst = nullptr;
  switch (ir::kind(instr)) {
#define C6502_DST(Name)                       \
  case K::Name:                               \
    dst = &std::get<ir::Name>(instr).dst;     \
    break;
    C6502_DST(SignExtend)
    C6502_DST(ZeroExtend)
    C6502_DST(Truncate)
    C6502_DST(IntToFloat)
    C6502_DST(IntToDouble)
    C6502_DST(FloatToInt)
    C6502_DST(DoubleToInt)
    C6502_DST(FloatToDouble)
    C6502_DST(DoubleToFloat)
    C6502_DST(GetAddress)
    C6502_DST(Load)
    C6502_DST(IndexedLoad)
    C6502_DST(IndexedConstLoad)
    C6502_DST(IndirectIndexedLoad)
    C6502_DST(Unary)
    C6502_DST(Binary)
    C6502_DST(Copy)
    C6502_DST(Phi)
#undef C6502_DST
    case K::FunctionCall: {
      const auto& d = std::get<ir::FunctionCall>(instr).dst;
      dst = d ? &*d : nullptr;
      break;
    }
    case K::IndirectCall: {
      const auto& d = std::get<ir::IndirectCall>(instr).dst;
      dst = d ? &*d : nullptr;
      break;
    }
    default:
      break;
  }
  if (dst == nullptr) {
    return std::nullopt;
  }
  if (const auto* v = std::get_if<ir::Var>(dst)) {
    return std::string_view(v->name);
  }
  return std::nullopt;
}

const ir::Type_instruction* DefUseEnv::def_of(std::string_view name) const {
  const auto it = def_idx.find(name);
  if (it == def_idx.end()) {
    return nullptr;
  }
  const auto& entry = instructions[it->second];
  return entry ? &*entry : nullptr;
}

DefUseRewrite DefUseRewrite::one(ir::Type_instruction instr) {
  DefUseRewrite r;
  r.replacement.push_back(std::move(instr));
  return r;
}

DefUseRewrite DefUseRewrite::dropping(ir::Type_instruction instr,
                                      std::vector<std::string> drop_defs) {
  DefUseRewrite r = one(std::move(instr));
  r.is_rewrite = true;
  r.drop_defs = std::move(drop_defs);
  return r;
}

std::map<std::string, std::size_t, std::less<>> build_def_idx(const Instrs& instrs) {
  std::map<std::string, std::size_t, std::less<>> out;
  for (std::size_t i = 0; i < instrs.size(); ++i) {
    if (const auto name = dst_name(instrs[i])) {
      out.insert_or_assign(std::string(*name), i);
    }
  }
  return out;
}

Instrs rebuild_def_use(const Instrs& instrs, std::map<std::size_t, DefUseRewrite>& rewrites,
                       const std::vector<bool>& dropped) {
  Instrs out;
  for (std::size_t i = 0; i < instrs.size(); ++i) {
    const auto it = rewrites.find(i);
    if (dropped[i]) {
      if (it != rewrites.end() && it->second.is_list) {
        auto& rep = it->second.replacement;
        for (std::size_t k = 0; k + 1 < rep.size(); ++k) {
          out.push_back(std::move(rep[k]));
        }
      }
      continue;
    }
    if (it == rewrites.end()) {
      out.push_back(instrs[i]);
      continue;
    }
    for (auto& ins : it->second.replacement) {
      out.push_back(std::move(ins));
    }
  }
  return out;
}

const ir::Type_instruction* RuleEnv::def_of(const ir::Type_val& v) const {
  const auto* var = std::get_if<ir::Var>(&v);
  if (var == nullptr) {
    return nullptr;
  }
  const auto it = def_idx.find(var->name);
  return it == def_idx.end() ? nullptr : &(*instructions)[it->second];
}

bool RuleEnv::is_single_use(const ir::Type_val& v) const {
  const auto* var = std::get_if<ir::Var>(&v);
  if (var == nullptr) {
    return false;
  }
  const std::size_t* n = use_count.find(var->name);
  return n != nullptr && *n == 1;
}

RuleEnv make_rule_env(const Instrs& instrs) {
  RuleEnv env;
  env.use_count = count_uses(instrs);
  for (std::size_t i = 0; i < instrs.size(); ++i) {
    for (const auto name : defs_in(instrs[i])) {
      env.def_idx.insert_or_assign(std::string(name), i);
    }
  }
  env.instructions = &instrs;
  return env;
}

const NameSet& observable_names(RuleEnv& env, const PassContext& ctx) {
  if (!env.observable) {
    NameSet names;
    for (const auto& ins : *env.instructions) {
      if (const auto* g = std::get_if<ir::GetAddress>(&ins)) {
        if (const auto* v = std::get_if<ir::Var>(&g->operand)) {
          names.insert(v->name);
        }
      }
      if (ctx.symbols == nullptr) {
        continue;
      }
      const auto add_static = [&](std::string_view name) {
        const Symbol* sym = ctx.symbols->find(std::string(name));
        if (sym != nullptr && std::holds_alternative<StaticAttr>(sym->attrs)) {
          names.insert(std::string(name));
        }
      };
      for (const auto name : uses_in(ins)) {
        add_static(name);
      }
      for (const auto name : defs_in(ins)) {
        add_static(name);
      }
    }
    env.observable = std::move(names);
  }
  return *env.observable;
}

bool single_use(const ir::Type_val& v, RuleEnv& env, const PassContext& ctx) {
  if (!env.is_single_use(v)) {
    return false;
  }
  const auto& name = std::get<ir::Var>(v).name;
  if (ctx.is_ssa(name)) {
    return true;
  }
  return !observable_names(env, ctx).contains(name);
}

ir::Function run_operand_rewrite(ir::Function fn, const ValMapper& mapper) {
  Instrs out;
  bool changed = false;
  for (const auto& instr : fn.instructions) {
    if (auto rewritten = rewrite_uses_in(instr, mapper)) {
      out.push_back(std::move(*rewritten));
      changed = true;
    } else {
      out.push_back(instr);
    }
  }
  if (!changed) {
    return fn;
  }
  return with_instructions(fn, std::move(out));
}

}  // namespace c6502::passes::framework
