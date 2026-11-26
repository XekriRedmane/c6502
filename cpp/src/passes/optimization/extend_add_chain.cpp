#include "passes/optimization/extend_add_chain.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::tac_ast;
namespace c99 = ::c6502::c99_ast;
using framework::DefUseEnv;
using framework::PassContext;

constexpr int kMaxCopyHops = 4;

const ir::Type_instruction* extend_def(const ir::Var& start, const DefUseEnv& env) {
  const ir::Var* v = &start;
  for (int hop = 0; hop < kMaxCopyHops + 1; ++hop) {
    const ir::Type_instruction* instr = env.def_of(*v);
    if (instr == nullptr) {
      return nullptr;
    }
    if (std::holds_alternative<ir::ZeroExtend>(*instr) ||
        std::holds_alternative<ir::SignExtend>(*instr)) {
      return instr;
    }
    const auto* copy = std::get_if<ir::Copy>(instr);
    if (copy == nullptr) {
      return nullptr;
    }
    v = std::get_if<ir::Var>(&copy->src);
    if (v == nullptr) {
      return nullptr;
    }
  }
  return nullptr;
}

bool defined_by_extend(const ir::Type_val& v, const DefUseEnv& env) {
  const auto* var = std::get_if<ir::Var>(&v);
  return var != nullptr && extend_def(*var, env) != nullptr;
}

}  // namespace

std::size_t use_count(const UseCounts& uses, std::string_view name) {
  const std::size_t* n = uses.find(name);
  return n == nullptr ? 0 : *n;
}

std::vector<std::string> ExtendAddChain::drop_defs() const {
  if (ext_droppable) {
    return {addr_var.name, ext_var.name};
  }
  return {addr_var.name};
}

std::optional<ExtendAddChain> recognize_extend_add_chain(const ir::Var& addr_var,
                                                         const DefUseEnv& env,
                                                         const PassContext& ctx,
                                                         const UseCounts& uses,
                                                         bool single_use) {
  if (ctx.symbols == nullptr) {
    return std::nullopt;
  }
  if (!ctx.is_ssa(addr_var.name)) {
    return std::nullopt;
  }
  if (single_use && use_count(uses, addr_var.name) != 1) {
    return std::nullopt;
  }
  const ir::Type_instruction* def = env.def_of(addr_var);
  const auto* binary = def != nullptr ? std::get_if<ir::Binary>(def) : nullptr;
  if (binary == nullptr || !std::holds_alternative<ir::Add>(binary->op)) {
    return std::nullopt;
  }
  const ir::Type_val* base = nullptr;
  const ir::Var* ext_var = nullptr;
  if (defined_by_extend(binary->src2, env)) {
    base = &binary->src1;
    ext_var = &std::get<ir::Var>(binary->src2);
  } else if (defined_by_extend(binary->src1, env)) {
    base = &binary->src2;
    ext_var = &std::get<ir::Var>(binary->src1);
  } else {
    return std::nullopt;
  }
  if (!ctx.is_ssa(ext_var->name)) {
    return std::nullopt;
  }
  // Only a ZeroExtend is accepted. The 6502's X and Y are unsigned, so
  // `base,X` and `(base),Y` address `base + 255` for an index of -1,
  // where a SignExtend gives `base - 1`.
  const auto* ext = std::get_if<ir::ZeroExtend>(extend_def(*ext_var, env));
  if (ext == nullptr) {
    return std::nullopt;
  }
  const auto* idx_var = std::get_if<ir::Var>(&ext->src);
  if (idx_var == nullptr || !is_1_byte_var(*idx_var, ctx.symbols)) {
    return std::nullopt;
  }
  return ExtendAddChain{
      .base = *base,
      .idx_var = *idx_var,
      .addr_var = addr_var,
      .ext_var = *ext_var,
      .ext_droppable = use_count(uses, ext_var->name) == 1,
  };
}

bool is_1_byte_var(const ir::Var& v, const SymbolTable* symbols) {
  if (symbols == nullptr) {
    return false;
  }
  const Symbol* sym = symbols->find(v.name);
  if (sym == nullptr) {
    return false;
  }
  const c99::Type_data_type* t = &sym->type;
  for (;;) {
    if (const auto* c = std::get_if<c99::Const>(t)) {
      t = &*c->referenced_type;
    } else if (const auto* vol = std::get_if<c99::Volatile>(t)) {
      t = &*vol->referenced_type;
    } else {
      break;
    }
  }
  return std::holds_alternative<c99::Char>(*t) || std::holds_alternative<c99::SChar>(*t) ||
         std::holds_alternative<c99::UChar>(*t);
}

bool is_1_byte_val(const ir::Type_val& v, const SymbolTable* symbols) {
  if (const auto* c = std::get_if<ir::Constant>(&v)) {
    return std::holds_alternative<ir::ConstChar>(c->const_) ||
           std::holds_alternative<ir::ConstUChar>(c->const_);
  }
  return is_1_byte_var(std::get<ir::Var>(v), symbols);
}

NameSet all_dsts(const ir::Function& fn) {
  NameSet out;
  for (const auto& instr : fn.instructions) {
    if (const auto name = framework::dst_name(instr)) {
      out.insert(std::string(*name));
    }
  }
  return out;
}

}  // namespace c6502::passes
