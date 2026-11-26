#include "passes/optimization/sink_and_past_branch.h"

#include <algorithm>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "bit_precise.h"
#include "passes/optimization/cfg.h"
#include "passes/optimization/extend_add_chain.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::tac_ast;
namespace c99 = ::c6502::c99_ast;
using asdl::Int;
using framework::Instrs;
using framework::PassContext;
using IK = ir::Kind_instruction;
using tac_cfg::BasicBlock;
using tac_cfg::Cfg;

struct Trio {
  std::string x;
  std::string x_ext;
  std::string x_and;
  std::string t;
  Int mask = 0;
};

bool is_1byte_unsigned(const SymbolTable& symbols, const std::string& name) {
  const Symbol* sym = symbols.find(name);
  if (sym == nullptr) {
    return false;
  }
  return std::holds_alternative<c99::Char>(sym->type) ||
         std::holds_alternative<c99::SChar>(sym->type) ||
         std::holds_alternative<c99::UChar>(sym->type);
}

std::optional<Int> try_extract_byte_mask(const ir::Binary& binop, const std::string& x_ext) {
  bool has_var = false;
  std::optional<Int> value;
  for (const ir::Type_val* s : {&binop.src1, &binop.src2}) {
    if (const auto* v = std::get_if<ir::Var>(s)) {
      if (v->name == x_ext) {
        has_var = true;
      }
    } else {
      const auto& c = std::get<ir::Constant>(*s).const_;
      if (std::holds_alternative<ir::ConstInt>(c) || std::holds_alternative<ir::ConstUInt>(c) ||
          std::holds_alternative<ir::ConstChar>(c) || std::holds_alternative<ir::ConstUChar>(c)) {
        value = bit_precise::raw_value(c);
      }
    }
  }
  if (!has_var || !value) {
    return std::nullopt;
  }
  if (!(0 <= *value && *value <= 0xFF)) {
    return std::nullopt;
  }
  return value;
}

std::optional<Trio> match_trio(const Instrs& instrs, const ir::JumpIfMasked& jump,
                               const SymbolTable& symbols) {
  const std::size_t n = instrs.size();
  const auto* ze = std::get_if<ir::ZeroExtend>(&instrs[n - 4]);
  if (ze == nullptr) {
    return std::nullopt;
  }
  const auto* x = std::get_if<ir::Var>(&ze->src);
  const auto* x_ext = std::get_if<ir::Var>(&ze->dst);
  if (x == nullptr || x_ext == nullptr) {
    return std::nullopt;
  }
  if (!is_1byte_unsigned(symbols, x->name)) {
    return std::nullopt;
  }
  const auto* binop = std::get_if<ir::Binary>(&instrs[n - 3]);
  if (binop == nullptr || !std::holds_alternative<ir::BitwiseAnd>(binop->op)) {
    return std::nullopt;
  }
  const auto* x_and = std::get_if<ir::Var>(&binop->dst);
  if (x_and == nullptr) {
    return std::nullopt;
  }
  const auto mask = try_extract_byte_mask(*binop, x_ext->name);
  if (!mask) {
    return std::nullopt;
  }
  const auto* tr = std::get_if<ir::Truncate>(&instrs[n - 2]);
  if (tr == nullptr) {
    return std::nullopt;
  }
  const auto* tr_src = std::get_if<ir::Var>(&tr->src);
  if (tr_src == nullptr || tr_src->name != x_and->name) {
    return std::nullopt;
  }
  const auto* t = std::get_if<ir::Var>(&tr->dst);
  if (t == nullptr) {
    return std::nullopt;
  }
  const auto* jv = std::get_if<ir::Var>(&jump.val);
  if (jv == nullptr || jv->name != x->name) {
    return std::nullopt;
  }
  return Trio{x->name, x_ext->name, x_and->name, t->name, *mask};
}

bool phi_reads(const BasicBlock& block, const std::string& name) {
  for (const auto& ins : block.instructions) {
    if (const auto* phi = std::get_if<ir::Phi>(&ins)) {
      for (const auto& a : phi->args) {
        if (const auto* v = std::get_if<ir::Var>(&std::get<ir::PhiArg>(a).source); v && v->name == name) {
          return true;
        }
      }
    }
  }
  return false;
}

void register_fresh(SymbolTable& symbols, const std::string& src_name,
                    const std::string& new_name) {
  if (symbols.contains(new_name)) {
    return;
  }
  const Symbol* src = symbols.find(src_name);
  if (src == nullptr) {
    return;
  }
  c99::Type_data_type type = src->type;
  symbols.insert_or_assign(new_name, Symbol{.type = std::move(type), .attrs = LocalAttr{}});
}

std::map<std::string, std::set<int>, std::less<>> index_use_blocks(const Cfg& cfg) {
  std::map<std::string, std::set<int>, std::less<>> out;
  for (const auto& [bid, block] : cfg.blocks) {
    if (bid == tac_cfg::kEntryId || bid == tac_cfg::kExitId) {
      continue;
    }
    for (const auto& instr : block.instructions) {
      for (const auto u : uses_in(instr)) {
        out[std::string(u)].insert(bid);
      }
    }
  }
  return out;
}

std::optional<int> find_block_by_label(const Cfg& cfg, const std::string& label) {
  for (const auto& [bid, block] : cfg.blocks) {
    if (bid == tac_cfg::kEntryId || bid == tac_cfg::kExitId || block.instructions.empty()) {
      continue;
    }
    if (const auto* l = std::get_if<ir::Label>(&block.instructions.front());
        l != nullptr && l->name == label) {
      return bid;
    }
  }
  return std::nullopt;
}

bool starts_with_label(const BasicBlock& block) {
  return !block.instructions.empty() && std::holds_alternative<ir::Label>(block.instructions[0]);
}

void prepend_trio(BasicBlock& block, const std::string& x, Int mask, const std::string& ext,
                  const std::string& band, const std::string& t) {
  const std::ptrdiff_t at = starts_with_label(block) ? 1 : 0;
  Instrs trio;
  trio.push_back(ir::ZeroExtend{.src = ir::Var{.name = x}, .dst = ir::Var{.name = ext}});
  trio.push_back(ir::Binary{.op = ir::BitwiseAnd{},
                            .src1 = ir::Var{.name = ext},
                            .src2 = ir::Constant{.const_ = ir::ConstInt{.value = mask}},
                            .dst = ir::Var{.name = band}});
  trio.push_back(ir::Truncate{.src = ir::Var{.name = band}, .dst = ir::Var{.name = t}});
  block.instructions.insert(block.instructions.begin() + at, trio.begin(), trio.end());
}

ir::Type_instruction rewrite_instr_uses(ir::Type_instruction instr, const std::string& old_name,
                                        const std::string& new_name) {
  const auto fix = [&](ir::Type_val& v) {
    if (auto* var = std::get_if<ir::Var>(&v); var != nullptr && var->name == old_name) {
      var->name = new_name;
    }
  };
  switch (ir::kind(instr)) {
    case IK::Ret: {
      auto& i = std::get<ir::Ret>(instr);
      if (i.val) {
        fix(*i.val);
      }
      break;
    }
#define C6502_SRC(Name)               \
  case IK::Name:                      \
    fix(std::get<ir::Name>(instr).src); \
    break;
      C6502_SRC(SignExtend)
      C6502_SRC(ZeroExtend)
      C6502_SRC(Truncate)
      C6502_SRC(IntToFloat)
      C6502_SRC(IntToDouble)
      C6502_SRC(FloatToInt)
      C6502_SRC(DoubleToInt)
      C6502_SRC(FloatToDouble)
      C6502_SRC(DoubleToFloat)
      C6502_SRC(Unary)
      C6502_SRC(Copy)
#undef C6502_SRC
    case IK::GetAddress:
      fix(std::get<ir::GetAddress>(instr).operand);
      break;
    case IK::Load:
      fix(std::get<ir::Load>(instr).src_ptr);
      break;
    case IK::Store: {
      auto& i = std::get<ir::Store>(instr);
      fix(i.src);
      fix(i.dst_ptr);
      break;
    }
    case IK::IndexedLoad:
      fix(std::get<ir::IndexedLoad>(instr).index);
      break;
    case IK::IndexedStore: {
      auto& i = std::get<ir::IndexedStore>(instr);
      fix(i.index);
      fix(i.src);
      break;
    }
    case IK::IndexedSymbolStore: {
      auto& i = std::get<ir::IndexedSymbolStore>(instr);
      fix(i.index);
      fix(i.src);
      break;
    }
    case IK::IndexedConstLoad:
      fix(std::get<ir::IndexedConstLoad>(instr).index);
      break;
    case IK::IndirectIndexedLoad: {
      auto& i = std::get<ir::IndirectIndexedLoad>(instr);
      fix(i.ptr);
      fix(i.index);
      break;
    }
    case IK::IndirectIndexedStore: {
      auto& i = std::get<ir::IndirectIndexedStore>(instr);
      fix(i.ptr);
      fix(i.index);
      fix(i.src);
      break;
    }
    case IK::Binary: {
      auto& i = std::get<ir::Binary>(instr);
      fix(i.src1);
      fix(i.src2);
      break;
    }
    case IK::JumpIfTrue:
      fix(std::get<ir::JumpIfTrue>(instr).condition);
      break;
    case IK::JumpIfFalse:
      fix(std::get<ir::JumpIfFalse>(instr).condition);
      break;
    case IK::JumpIfCmp: {
      auto& i = std::get<ir::JumpIfCmp>(instr);
      fix(i.src1);
      fix(i.src2);
      break;
    }
    case IK::JumpIfMasked:
      fix(std::get<ir::JumpIfMasked>(instr).val);
      break;
    case IK::FunctionCall:
      for (auto& a : std::get<ir::FunctionCall>(instr).args) {
        fix(a);
      }
      break;
    case IK::IndirectCall: {
      auto& i = std::get<ir::IndirectCall>(instr);
      fix(i.ptr);
      for (auto& a : i.args) {
        fix(a);
      }
      break;
    }
    case IK::Phi:
      for (auto& a : std::get<ir::Phi>(instr).args) {
        fix(std::get<ir::PhiArg>(a).source);
      }
      break;
    default:
      break;
  }
  return instr;
}

void rewrite_var_uses(BasicBlock& block, const std::string& old_name,
                      const std::string& new_name, std::size_t skip_count) {
  for (std::size_t k = skip_count; k < block.instructions.size(); ++k) {
    block.instructions[k] = rewrite_instr_uses(std::move(block.instructions[k]), old_name, new_name);
  }
}

}  // namespace

ir::Function sink_and_past_branch(ir::Function fn, const PassContext& ctx) {
  if (ctx.symbols == nullptr) {
    return fn;
  }
  SymbolTable& symbols = *ctx.symbols;
  const UseCounts use_counts = count_uses(fn.instructions);
  Cfg cfg = tac_cfg::build_cfg(fn);
  const auto use_blocks = index_use_blocks(cfg);
  int counter = 0;
  bool changed = false;
  const std::vector<int> order = cfg.block_order;
  for (const int bid : order) {
    BasicBlock& block = *cfg.blocks.find(bid);
    const Instrs instrs = block.instructions;
    if (instrs.size() < 4) {
      continue;
    }
    const auto* jump = std::get_if<ir::JumpIfMasked>(&instrs.back());
    if (jump == nullptr) {
      continue;
    }
    const auto trio = match_trio(instrs, *jump, symbols);
    if (!trio) {
      continue;
    }
    const auto target_bid = find_block_by_label(cfg, jump->target);
    if (!target_bid) {
      continue;
    }
    const auto pos = std::find(cfg.block_order.begin(), cfg.block_order.end(), bid);
    if (pos == cfg.block_order.end() || pos + 1 == cfg.block_order.end()) {
      continue;
    }
    const int fallthru_bid = *(pos + 1);
    const auto users_it = use_blocks.find(trio->t);
    const std::set<int> no_users;
    const std::set<int>& users_of_t = users_it == use_blocks.end() ? no_users : users_it->second;
    if (!std::all_of(users_of_t.begin(), users_of_t.end(),
                     [&](int b) { return b == *target_bid || b == fallthru_bid; })) {
      continue;
    }
    if (fallthru_bid == *target_bid) {
      continue;
    }
    if (phi_reads(*cfg.blocks.find(fallthru_bid), trio->t) ||
        phi_reads(*cfg.blocks.find(*target_bid), trio->t)) {
      continue;
    }
    if (ctx.ssa_dsts == nullptr || !ctx.ssa_dsts->contains(trio->x)) {
      bool only_this = true;
      for (const int s : {fallthru_bid, *target_bid}) {
        const auto& preds = cfg.blocks.find(s)->predecessors;
        only_this = only_this && !preds.empty() &&
                    std::all_of(preds.begin(), preds.end(), [&](int p) { return p == bid; });
      }
      if (!only_this) {
        continue;
      }
    }
    const std::string n = std::to_string(counter++);
    const std::string then_ext = trio->x_ext + ".snk_then@" + n;
    const std::string then_and = trio->x_and + ".snk_then@" + n;
    const std::string then_t = trio->t + ".snk_then@" + n;
    const std::string else_ext = trio->x_ext + ".snk_else@" + n;
    const std::string else_and = trio->x_and + ".snk_else@" + n;
    const std::string else_t = trio->t + ".snk_else@" + n;
    register_fresh(symbols, trio->x_ext, then_ext);
    register_fresh(symbols, trio->x_and, then_and);
    register_fresh(symbols, trio->t, then_t);
    register_fresh(symbols, trio->x_ext, else_ext);
    register_fresh(symbols, trio->x_and, else_and);
    register_fresh(symbols, trio->t, else_t);
    const std::size_t m = instrs.size();
    Instrs rebuilt(instrs.begin(), instrs.begin() + static_cast<std::ptrdiff_t>(m - 4));
    if (use_count(use_counts, trio->x_and) > 1) {
      rebuilt.push_back(instrs[m - 4]);
      rebuilt.push_back(instrs[m - 3]);
    } else if (use_count(use_counts, trio->x_ext) > 1) {
      rebuilt.push_back(instrs[m - 4]);
    }
    rebuilt.push_back(instrs[m - 1]);
    block.instructions = std::move(rebuilt);
    BasicBlock& fallthru = *cfg.blocks.find(fallthru_bid);
    BasicBlock& target = *cfg.blocks.find(*target_bid);
    prepend_trio(fallthru, trio->x, trio->mask, then_ext, then_and, then_t);
    prepend_trio(target, trio->x, trio->mask, else_ext, else_and, else_t);
    if (users_of_t.contains(fallthru_bid)) {
      rewrite_var_uses(fallthru, trio->t, then_t, 3);
    }
    if (users_of_t.contains(*target_bid)) {
      rewrite_var_uses(target, trio->t, else_t, starts_with_label(target) ? 4 : 3);
    }
    changed = true;
  }
  if (!changed) {
    return fn;
  }
  return tac_cfg::cfg_to_function(fn, cfg);
}

}  // namespace c6502::passes
