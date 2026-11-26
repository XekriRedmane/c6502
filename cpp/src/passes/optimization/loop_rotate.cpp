#include "passes/optimization/loop_rotate.h"

#include <cstddef>
#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "passes/optimization/var_visit.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::tac_ast;
namespace c99 = ::c6502::c99_ast;
using K = ir::Kind_instruction;
using Instrs = std::vector<ir::Type_instruction>;

std::optional<asdl::Int> max_positive(const c99::Type_data_type& t) {
  switch (c99::kind(t)) {
    case c99::Kind_data_type::SChar:
      return asdl::Int{0x7F};
    case c99::Kind_data_type::Int:
      return asdl::Int{0x7FFF};
    case c99::Kind_data_type::Long:
      return asdl::Int{0x7FFFFFFF};
    case c99::Kind_data_type::LongLong:
      return asdl::Int{0x7FFFFFFFFFFFFFFF};
    default:
      return std::nullopt;
  }
}

std::optional<asdl::Int> const_int_value(const ir::Type_const& c) {
  switch (ir::kind(c)) {
    case ir::Kind_const::ConstChar:
      return std::get<ir::ConstChar>(c).value;
    case ir::Kind_const::ConstUChar:
      return std::get<ir::ConstUChar>(c).value;
    case ir::Kind_const::ConstInt:
      return std::get<ir::ConstInt>(c).value;
    case ir::Kind_const::ConstUInt:
      return std::get<ir::ConstUInt>(c).value;
    case ir::Kind_const::ConstLong:
      return std::get<ir::ConstLong>(c).value;
    case ir::Kind_const::ConstULong:
      return std::get<ir::ConstULong>(c).value;
    case ir::Kind_const::ConstLongLong:
      return std::get<ir::ConstLongLong>(c).value;
    case ir::Kind_const::ConstULongLong:
      return std::get<ir::ConstULongLong>(c).value;
    default:
      return std::nullopt;
  }
}

bool breaks_cond_block(const ir::Type_instruction& instr) {
  switch (ir::kind(instr)) {
    case K::Label:
    case K::Jump:
    case K::JumpIfTrue:
    case K::JumpIfCmp:
    case K::JumpIfMasked:
    case K::Ret:
    case K::FunctionCall:
    case K::IndirectCall:
      return true;
    default:
      return false;
  }
}

bool breaks_post_block(const ir::Type_instruction& instr) {
  return breaks_cond_block(instr) || asdl::isa<ir::JumpIfFalse>(instr);
}

const ir::Type_val* cast_source(const ir::Type_instruction& instr, bool truncate = true) {
  switch (ir::kind(instr)) {
    case K::Copy:
      return &std::get<ir::Copy>(instr).src;
    case K::SignExtend:
      return &std::get<ir::SignExtend>(instr).src;
    case K::ZeroExtend:
      return &std::get<ir::ZeroExtend>(instr).src;
    case K::Truncate:
      return truncate ? &std::get<ir::Truncate>(instr).src : nullptr;
    default:
      return nullptr;
  }
}

bool writes(const ir::Type_instruction& instr, std::string_view name) {
  for (const auto d : defs_in(instr)) {
    if (d == name) {
      return true;
    }
  }
  return false;
}

struct Match {
  std::size_t start_idx;
  std::size_t cond_jif_idx;
  std::size_t cont_idx;
  std::size_t jump_back_idx;
  std::size_t break_idx;
  std::string cond_var_name;
  std::string start_label;
  std::string x_name;
  bool body_iv_immutable;
};

const ir::Binary* find_ge_producer(const Instrs& cond_block, const std::string& cond_var) {
  const ir::Binary* producer = nullptr;
  for (const auto& ins : cond_block) {
    const auto* b = asdl::as<ir::Binary>(ins);
    if (b == nullptr) {
      continue;
    }
    const auto* d = asdl::as<ir::Var>(b->dst);
    if (d == nullptr || d->name != cond_var) {
      continue;
    }
    producer = b;
  }
  if (producer == nullptr || !asdl::isa<ir::GreaterOrEqual>(producer->op)) {
    return nullptr;
  }
  return producer;
}

bool val_resolves_to_zero(const ir::Type_val& val, const Instrs& cond_block) {
  if (const auto* c = asdl::as<ir::Constant>(val)) {
    const auto v = const_int_value(c->const_);
    return v && *v == 0;
  }
  const auto* var = asdl::as<ir::Var>(val);
  if (var == nullptr) {
    return false;
  }
  for (const auto& ins : cond_block) {
    const auto d = framework::dst_name(ins);
    if (!d || *d != var->name) {
      continue;
    }
    const ir::Type_val* src = cast_source(ins);
    return src != nullptr && val_resolves_to_zero(*src, cond_block);
  }
  return false;
}

bool computes_temporary(const ir::Type_instruction& ins) {
  switch (ir::kind(ins)) {
    case K::Copy:
    case K::SignExtend:
    case K::ZeroExtend:
    case K::Truncate:
    case K::Unary:
    case K::Binary: {
      const auto d = framework::dst_name(ins);
      return d && d->starts_with('%');
    }
    default:
      return false;
  }
}

std::optional<std::string> trace_to_var(const ir::Type_val& val, const Instrs& cond_block) {
  const auto* var = asdl::as<ir::Var>(val);
  if (var == nullptr) {
    return std::nullopt;
  }
  if (!var->name.starts_with('%')) {
    return var->name;
  }
  for (const auto& ins : cond_block) {
    const auto d = framework::dst_name(ins);
    if (!d || *d != var->name) {
      continue;
    }
    const ir::Type_val* src = cast_source(ins, false);
    if (src == nullptr) {
      return std::nullopt;
    }
    return trace_to_var(*src, cond_block);
  }
  return std::nullopt;
}

bool post_decrements_x_by_one(const Instrs& post, std::size_t begin, std::size_t end,
                              const std::string& x_name) {
  if (end - begin < 2) {
    return false;
  }
  const auto* sub = asdl::as<ir::Binary>(post[end - 2]);
  if (sub == nullptr || !asdl::isa<ir::Subtract>(sub->op)) {
    return false;
  }
  const auto* s1 = asdl::as<ir::Var>(sub->src1);
  if (s1 == nullptr || s1->name != x_name) {
    return false;
  }
  const auto* s2 = asdl::as<ir::Constant>(sub->src2);
  if (s2 == nullptr) {
    return false;
  }
  const auto one = const_int_value(s2->const_);
  if (!one || *one != 1) {
    return false;
  }
  const auto* sub_dst = asdl::as<ir::Var>(sub->dst);
  if (sub_dst == nullptr) {
    return false;
  }
  const auto* cp = asdl::as<ir::Copy>(post[end - 1]);
  if (cp == nullptr) {
    return false;
  }
  const auto* cs = asdl::as<ir::Var>(cp->src);
  if (cs == nullptr || cs->name != sub_dst->name) {
    return false;
  }
  const auto* cd = asdl::as<ir::Var>(cp->dst);
  return cd != nullptr && cd->name == x_name;
}

bool address_taken(const Instrs& instrs, const std::string& name) {
  for (const auto& ins : instrs) {
    const auto* ga = asdl::as<ir::GetAddress>(ins);
    if (ga == nullptr) {
      continue;
    }
    const auto* v = asdl::as<ir::Var>(ga->operand);
    if (v != nullptr && v->name == name) {
      return true;
    }
  }
  return false;
}

std::size_t straight_line_start(const Instrs& instrs, std::size_t start_idx) {
  for (std::size_t k = start_idx; k-- > 0;) {
    if (asdl::isa<ir::Label>(instrs[k])) {
      return k + 1;
    }
  }
  return 0;
}

std::optional<std::size_t> last_def_pos(const Instrs& instrs, std::size_t end_excl,
                                        std::string_view name) {
  for (std::size_t k = end_excl; k-- > 0;) {
    if (writes(instrs[k], name)) {
      return k;
    }
  }
  return std::nullopt;
}

bool resolves_to_int_in_range(const Instrs& instrs, std::size_t def_idx, asdl::Int low,
                              asdl::Int high, std::size_t floor, int depth = 0) {
  if (depth > 4) {
    return false;
  }
  const ir::Type_val* src = cast_source(instrs[def_idx]);
  if (src == nullptr) {
    return false;
  }
  if (const auto* c = asdl::as<ir::Constant>(*src)) {
    const auto v = const_int_value(c->const_);
    return v && low <= *v && *v <= high;
  }
  const auto* inner = asdl::as<ir::Var>(*src);
  if (inner == nullptr) {
    return false;
  }
  const auto inner_def = last_def_pos(instrs, def_idx, inner->name);
  if (!inner_def || *inner_def < floor) {
    return false;
  }
  return resolves_to_int_in_range(instrs, *inner_def, low, high, floor, depth + 1);
}

bool init_value_is_nonnegative(const Instrs& instrs, std::size_t start_idx,
                               const std::string& x_name, const c99::Type_data_type& x_type) {
  const auto max_pos = max_positive(x_type);
  if (!max_pos) {
    return false;
  }
  if (address_taken(instrs, x_name)) {
    return false;
  }
  const std::size_t floor = straight_line_start(instrs, start_idx);
  const auto def_idx = last_def_pos(instrs, start_idx, x_name);
  if (!def_idx || *def_idx < floor) {
    return false;
  }
  return resolves_to_int_in_range(instrs, *def_idx, 0, *max_pos, floor);
}

std::optional<Match> try_match(const Instrs& instrs, std::size_t i, const SymbolTable& symbols) {
  const auto* label = asdl::as<ir::Label>(instrs[i]);
  if (label == nullptr) {
    return std::nullopt;
  }
  const std::string& l_start = label->name;
  if (!l_start.ends_with("_start")) {
    return std::nullopt;
  }
  const std::string base = l_start.substr(0, l_start.size() - 6);
  const std::string l_continue = base + "_continue";
  const std::string l_break = base + "_break";

  const std::size_t n = instrs.size();
  std::optional<std::size_t> cond_jif_idx;
  std::string cond_var_name;
  for (std::size_t j = i + 1; j < n; ++j) {
    const auto& instr = instrs[j];
    if (const auto* jf = asdl::as<ir::JumpIfFalse>(instr); jf != nullptr && jf->target == l_break) {
      const auto* cond = asdl::as<ir::Var>(jf->condition);
      if (cond == nullptr) {
        return std::nullopt;
      }
      cond_var_name = cond->name;
      cond_jif_idx = j;
      break;
    }
    if (breaks_cond_block(instr)) {
      return std::nullopt;
    }
  }
  if (!cond_jif_idx) {
    return std::nullopt;
  }

  const Instrs cond_block(instrs.begin() + static_cast<std::ptrdiff_t>(i + 1),
                          instrs.begin() + static_cast<std::ptrdiff_t>(*cond_jif_idx));
  for (const auto& ins : cond_block) {
    if (!computes_temporary(ins)) {
      return std::nullopt;
    }
  }
  const ir::Binary* ge = find_ge_producer(cond_block, cond_var_name);
  if (ge == nullptr || !val_resolves_to_zero(ge->src2, cond_block)) {
    return std::nullopt;
  }
  const auto x_name = trace_to_var(ge->src1, cond_block);
  if (!x_name) {
    return std::nullopt;
  }

  const Symbol* sym = symbols.find(*x_name);
  if (sym == nullptr || !max_positive(sym->type)) {
    return std::nullopt;
  }

  std::optional<std::size_t> cont_idx;
  for (std::size_t j = *cond_jif_idx + 1; j < n; ++j) {
    const auto* cur = asdl::as<ir::Label>(instrs[j]);
    if (cur != nullptr && cur->name == l_continue) {
      cont_idx = j;
      break;
    }
  }
  if (!cont_idx) {
    return std::nullopt;
  }

  std::optional<std::size_t> jump_back_idx;
  for (std::size_t j = *cont_idx + 1; j < n; ++j) {
    const auto& instr = instrs[j];
    if (const auto* jmp = asdl::as<ir::Jump>(instr); jmp != nullptr && jmp->target == l_start) {
      jump_back_idx = j;
      break;
    }
    if (breaks_post_block(instr)) {
      return std::nullopt;
    }
  }
  if (!jump_back_idx || *jump_back_idx + 1 >= n) {
    return std::nullopt;
  }
  const auto* after_jump = asdl::as<ir::Label>(instrs[*jump_back_idx + 1]);
  if (after_jump == nullptr || after_jump->name != l_break) {
    return std::nullopt;
  }
  const std::size_t break_idx = *jump_back_idx + 1;

  if (!post_decrements_x_by_one(instrs, *cont_idx + 1, *jump_back_idx, *x_name)) {
    return std::nullopt;
  }
  if (!init_value_is_nonnegative(instrs, i, *x_name, sym->type)) {
    return std::nullopt;
  }

  bool body_iv_immutable = true;
  for (std::size_t k = *cond_jif_idx + 1; k < *cont_idx; ++k) {
    if (writes(instrs[k], *x_name)) {
      body_iv_immutable = false;
      break;
    }
  }

  return Match{
      .start_idx = i,
      .cond_jif_idx = *cond_jif_idx,
      .cont_idx = *cont_idx,
      .jump_back_idx = *jump_back_idx,
      .break_idx = break_idx,
      .cond_var_name = cond_var_name,
      .start_label = l_start,
      .x_name = *x_name,
      .body_iv_immutable = body_iv_immutable,
  };
}

void emit_rotated(const Instrs& instrs, const Match& m, Instrs& out) {
  out.push_back(instrs[m.start_idx]);
  for (std::size_t k = m.cond_jif_idx + 1; k < m.cont_idx; ++k) {
    const auto& ins = instrs[k];
    if (m.body_iv_immutable) {
      if (const auto* se = asdl::as<ir::SignExtend>(ins)) {
        const auto* src = asdl::as<ir::Var>(se->src);
        if (src != nullptr && src->name == m.x_name) {
          out.push_back(ir::ZeroExtend{.src = se->src, .dst = se->dst});
          continue;
        }
      }
    }
    out.push_back(ins);
  }
  out.push_back(instrs[m.cont_idx]);
  for (std::size_t k = m.cont_idx + 1; k < m.jump_back_idx; ++k) {
    out.push_back(instrs[k]);
  }
  for (std::size_t k = m.start_idx + 1; k < m.cond_jif_idx; ++k) {
    out.push_back(instrs[k]);
  }
  out.push_back(ir::JumpIfTrue{.condition = ir::Var{.name = m.cond_var_name},
                               .target = m.start_label});
  out.push_back(instrs[m.break_idx]);
}

}  // namespace

ir::Function rotate_signed_countdown_loops(ir::Function fn, const framework::PassContext& ctx) {
  if (ctx.symbols == nullptr) {
    return fn;
  }
  const Instrs& instrs = fn.instructions;
  Instrs out;
  out.reserve(instrs.size());
  const std::size_t n = instrs.size();
  std::size_t i = 0;
  while (i < n) {
    const auto m = try_match(instrs, i, *ctx.symbols);
    if (!m) {
      out.push_back(instrs[i]);
      ++i;
      continue;
    }
    emit_rotated(instrs, *m, out);
    i = m->break_idx + 1;
  }
  return framework::with_instructions(fn, std::move(out));
}

}  // namespace c6502::passes
