#include "passes/dual_index_promotion.h"

#include <algorithm>
#include <cstddef>
#include <optional>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <variant>
#include <vector>

#include "passes/asm_liveness.h"
#include "passes/asm_operands.h"
#include "passes/decimal_arith.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::asm_ast;
using ::c6502::asdl::as;
using ::c6502::asdl::isa;

using InstrList = std::vector<ir::Type_instruction>;

bool is_reg(const ir::Type_operand& op, bool (*test)(const ir::Type_reg&)) {
  const auto* r = as<ir::Reg>(op);
  return r != nullptr && test(r->reg);
}

bool is_x(const ir::Type_reg& r) { return isa<ir::X>(r); }
bool is_y(const ir::Type_reg& r) { return isa<ir::Y>(r); }

bool is_reg_x(const ir::Type_operand& op) { return is_reg(op, is_x); }
bool is_reg_y(const ir::Type_operand& op) { return is_reg(op, is_y); }

bool has_reg_arg(const std::vector<std::string>& reg_args, const char* reg) {
  return std::find(reg_args.begin(), reg_args.end(), reg) != reg_args.end();
}

bool references_y(const ir::Type_instruction& instr) {
  if (const auto* da = as<ir::DecimalArith>(instr)) {
    return decimal_arith::writes_y(*da);
  }
  if (const auto* call = as<ir::Call>(instr); call != nullptr && has_reg_arg(call->reg_args, "Y")) {
    return true;
  }
  if (const auto* jump = as<ir::Jump>(instr); jump != nullptr && has_reg_arg(jump->reg_args, "Y")) {
    return true;
  }
  bool found = false;
  for_each_operand<false>(instr, [&](const ir::Type_operand& op) {
    if (is_reg_y(op)) {
      found = true;
    } else if (const auto* ix = as<ir::IndexedData>(op); ix != nullptr && isa<ir::Y>(ix->index)) {
      found = true;
    } else if (isa<ir::Indirect, ir::IndirectY, ir::IndirectZp, ir::IndirectZpY, ir::Frame,
                   ir::Stack>(op)) {
      found = true;
    }
  });
  return found;
}

const std::string* ldx_data_source_name(const ir::Type_instruction& instr) {
  const auto* mov = as<ir::Mov>(instr);
  if (mov == nullptr || !is_reg_x(mov->dst)) {
    return nullptr;
  }
  const auto* src = as<ir::Data>(mov->src);
  if (src == nullptr || src->offset != 0) {
    return nullptr;
  }
  return &src->name;
}

bool is_ldx_of(const ir::Type_instruction& instr, const std::string& name) {
  const std::string* n = ldx_data_source_name(instr);
  return n != nullptr && *n == name;
}

std::vector<std::pair<std::string, int>> ldx_data_candidates(const InstrList& instrs) {
  std::vector<std::pair<std::string, int>> counts;
  for (const auto& instr : instrs) {
    const std::string* name = ldx_data_source_name(instr);
    if (name == nullptr) {
      continue;
    }
    auto it = std::find_if(counts.begin(), counts.end(),
                           [&](const auto& nc) { return nc.first == *name; });
    if (it == counts.end()) {
      counts.emplace_back(*name, 1);
    } else {
      ++it->second;
    }
  }
  std::vector<std::pair<std::string, int>> out;
  for (auto& nc : counts) {
    if (nc.second >= 2) {
      out.push_back(std::move(nc));
    }
  }
  return out;
}

bool x_has_other_uses(const InstrList& instrs, const std::string& promoted_name) {
  for (const auto& instr : instrs) {
    const auto* mov = as<ir::Mov>(instr);
    if (mov == nullptr || !is_reg_x(mov->dst)) {
      continue;
    }
    if (const auto* src = as<ir::Data>(mov->src);
        src != nullptr && src->name == promoted_name && src->offset == 0) {
      continue;
    }
    return true;
  }
  return false;
}

bool writes_x(const ir::Type_instruction& instr) {
  if (const auto* mov = as<ir::Mov>(instr)) {
    return is_reg_x(mov->dst);
  }
  if (const auto* inc = as<ir::Inc>(instr)) {
    return is_reg_x(inc->dst);
  }
  if (const auto* dec = as<ir::Dec>(instr)) {
    return is_reg_x(dec->dst);
  }
  return false;
}

bool reads_x_value(const ir::Type_instruction& instr) {
  if (const auto* call = as<ir::Call>(instr)) {
    return has_reg_arg(call->reg_args, "X");
  }
  if (const auto* jump = as<ir::Jump>(instr)) {
    return has_reg_arg(jump->reg_args, "X");
  }
  if (const auto* ret = as<ir::Ret>(instr)) {
    return ret->save_x;
  }
  if (const auto* ret = as<ir::Return>(instr)) {
    return ret->save_x;
  }
  if (const auto* mov = as<ir::Mov>(instr)) {
    return is_reg_x(mov->src);
  }
  if (isa<ir::Pop>(instr)) {
    return false;
  }
  if (const auto* la = as<ir::LoadAddress>(instr)) {
    return is_reg_x(la->src);
  }
  bool found = false;
  for_each_operand<false>(instr, [&](const ir::Type_operand& op) {
    found = found || is_reg_x(op);
  });
  return found;
}

bool is_indexed_data_x(const ir::Type_operand& op) {
  const auto* ix = as<ir::IndexedData>(op);
  return ix != nullptr && isa<ir::X>(ix->index);
}

void fix_op(ir::Type_operand& op) {
  if (auto* ix = as<ir::IndexedData>(op); ix != nullptr && isa<ir::X>(ix->index)) {
    ix->index = ir::Y{};
  }
}

bool rewritable_kind(const ir::Type_instruction& instr) {
  return isa<ir::Mov, ir::Add, ir::Sub, ir::And, ir::Or, ir::Xor, ir::Compare>(instr);
}

void rewrite_x_to_y(ir::Type_instruction& instr) {
  if (!rewritable_kind(instr)) {
    return;
  }
  std::visit(
      [](auto& ins) {
        auto fix = [](auto& field) {
          if constexpr (std::is_same_v<std::decay_t<decltype(field)>, ir::Type_operand>) {
            fix_op(field);
          }
        };
        if constexpr (requires { ins.src; }) {
          fix(ins.src);
        }
        if constexpr (requires { ins.src1; }) {
          fix(ins.src1);
        }
        if constexpr (requires { ins.src2; }) {
          fix(ins.src2);
        }
        if constexpr (requires { ins.left; }) {
          fix(ins.left);
        }
        if constexpr (requires { ins.right; }) {
          fix(ins.right);
        }
        if constexpr (requires { ins.dst; }) {
          fix(ins.dst);
        }
      },
      instr);
}

// The 6502 has `INC abs,X`, `DEC abs,X`, and the shifts on abs,X, and
// no abs,Y form of any of them.
bool dst_x_only_kind(const ir::Type_instruction& instr) {
  return isa<ir::Inc, ir::Dec, ir::ArithmeticShiftLeft, ir::LogicalShiftRight,
             ir::RotateLeft, ir::RotateRight>(instr);
}

std::vector<std::size_t> successors(
    const InstrList& instrs, std::size_t k,
    const std::unordered_map<std::string, std::size_t>& label_idx) {
  const auto& instr = instrs[k];
  if (isa<ir::Ret, ir::Return>(instr)) {
    return {};
  }
  if (const auto* jump = as<ir::Jump>(instr)) {
    const auto it = label_idx.find(jump->target);
    if (it == label_idx.end()) {
      return {};
    }
    return {it->second};
  }
  std::vector<std::size_t> out;
  if (k + 1 < instrs.size()) {
    out.push_back(k + 1);
  }
  if (const auto* br = as<ir::Branch>(instr)) {
    if (const auto it = label_idx.find(br->target); it != label_idx.end()) {
      out.push_back(it->second);
    }
  }
  return out;
}

constexpr unsigned kXIsD = 1;
constexpr unsigned kXIsOther = 2;

std::optional<std::vector<bool>> plan_x_reads(const InstrList& instrs,
                                              const std::string& promoted_name) {
  std::unordered_map<std::string, std::size_t> label_idx;
  for (std::size_t k = 0; k < instrs.size(); ++k) {
    if (const auto* l = as<ir::Label>(instrs[k])) {
      label_idx[l->name] = k;
    }
  }
  const std::size_t n = instrs.size();
  std::vector<bool> rewrite(n, false);
  if (n == 0) {
    return rewrite;
  }
  std::vector<unsigned> state_in(n, 0);
  state_in[0] = kXIsOther;
  std::vector<std::size_t> work = {0};
  while (!work.empty()) {
    const std::size_t k = work.back();
    work.pop_back();
    const auto& instr = instrs[k];
    unsigned out;
    if (is_ldx_of(instr, promoted_name)) {
      out = kXIsD;
    } else if (writes_x(instr)) {
      out = kXIsOther;
    } else {
      out = state_in[k];
    }
    for (const std::size_t succ : successors(instrs, k, label_idx)) {
      const unsigned old = state_in[succ];
      const unsigned now = old | out;
      if (now != old) {
        state_in[succ] = now;
        work.push_back(succ);
      }
    }
  }
  for (std::size_t k = 0; k < n; ++k) {
    const auto& instr = instrs[k];
    const unsigned state = state_in[k];
    if ((state & kXIsD) == 0) {
      continue;
    }
    if (reads_x_value(instr)) {
      return std::nullopt;
    }
    bool indexed = false;
    for_each_operand<false>(instr, [&](const ir::Type_operand& op) {
      indexed = indexed || is_indexed_data_x(op);
    });
    if (!indexed) {
      continue;
    }
    if (state != kXIsD) {
      return std::nullopt;
    }
    if (dst_x_only_kind(instr)) {
      return std::nullopt;
    }
    if (!rewritable_kind(instr)) {
      return std::nullopt;
    }
    rewrite[k] = true;
  }
  return rewrite;
}

std::size_t entry_insertion_point(const InstrList& instrs) {
  std::size_t i = 0;
  while (i < instrs.size() &&
         isa<ir::Label, ir::FunctionPrologue, ir::AllocateStack>(instrs[i])) {
    ++i;
  }
  return i;
}

bool writes_name(const ir::Type_instruction& instr, const std::string& name) {
  if (const auto* da = as<ir::DecimalArith>(instr)) {
    for (const auto& w : decimal_arith::writes(*da)) {
      if (const auto* d = as<ir::Data>(w); d != nullptr && d->name == name) {
        return true;
      }
    }
    return false;
  }
  if (isa<ir::Push, ir::Compare, ir::BitTest>(instr)) {
    return false;
  }
  bool found = false;
  std::visit(
      [&](const auto& ins) {
        if constexpr (requires { ins.dst; }) {
          using D = std::decay_t<decltype(ins.dst)>;
          if constexpr (std::is_same_v<D, ir::Type_operand>) {
            if (const auto* d = as<ir::Data>(ins.dst)) {
              found = d->name == name;
            } else if (const auto* ix = as<ir::IndexedData>(ins.dst)) {
              found = ix->name == name;
            }
          }
        }
      },
      instr);
  return found;
}

void do_promote(ir::Function& fn, const std::string& promoted_name,
                const std::vector<bool>& rewrite) {
  InstrList& instrs = fn.instructions;
  const std::size_t insert_at = entry_insertion_point(instrs);
  InstrList out;
  out.reserve(instrs.size() + 1);
  for (std::size_t k = 0; k < insert_at; ++k) {
    out.push_back(std::move(instrs[k]));
  }
  out.push_back(ir::Mov{.src = ir::Data{.name = promoted_name, .offset = 0},
                        .dst = ir::Reg{.reg = ir::Y{}},
                        .is_volatile = false});
  for (std::size_t k = insert_at; k < instrs.size(); ++k) {
    if (is_ldx_of(instrs[k], promoted_name)) {
      continue;
    }
    if (rewrite[k]) {
      rewrite_x_to_y(instrs[k]);
    }
    out.push_back(std::move(instrs[k]));
  }
  instrs = std::move(out);
}

void rewrite_function(ir::Function& fn) {
  const InstrList& instrs = fn.instructions;
  for (const auto& instr : instrs) {
    if (references_y(instr)) {
      return;
    }
  }
  auto candidates = ldx_data_candidates(instrs);
  if (candidates.empty()) {
    return;
  }
  std::stable_sort(candidates.begin(), candidates.end(), [](const auto& a, const auto& b) {
    if (a.second != b.second) {
      return a.second > b.second;
    }
    return a.first < b.first;
  });
  const std::string promoted_name = candidates[0].first;
  if (!x_has_other_uses(instrs, promoted_name)) {
    return;
  }
  for (const auto& instr : instrs) {
    if (isa<ir::Call>(instr) || writes_name(instr, promoted_name)) {
      return;
    }
  }
  const auto rewrite = plan_x_reads(instrs, promoted_name);
  if (!rewrite) {
    return;
  }
  // The dropped LDXs and the added LDY set N and Z.
  const LabelMap labels(instrs);
  if (!flags_dead_at(instrs, labels, entry_insertion_point(instrs))) {
    return;
  }
  for (std::size_t k = 0; k < instrs.size(); ++k) {
    if (is_ldx_of(instrs[k], promoted_name) && !flags_dead_at(instrs, labels, k + 1)) {
      return;
    }
  }
  do_promote(fn, promoted_name, *rewrite);
}

}  // namespace

asm_ast::Program apply_dual_index_promotion(asm_ast::Program prog) {
  for (auto& tl : prog.top_level) {
    if (auto* fn = as<ir::Function>(tl)) {
      rewrite_function(*fn);
    }
  }
  return prog;
}

}  // namespace c6502::passes
