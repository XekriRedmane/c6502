#include "passes/ir_check.h"

#include <algorithm>
#include <cstddef>
#include <map>
#include <set>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <vector>

#include "asdl/runtime.h"
#include "errors.h"
#include "passes/decimal_arith.h"
#include "passes/optimization/cfg.h"
#include "passes/optimization/var_visit.h"
#include "passes/optimization_asm/all_operands.h"
#include "passes/recursion_saves.h"
#include "rtlib/helpers.h"

namespace c6502::passes::ir_check {

namespace {

namespace tac = ::c6502::tac_ast;
namespace asm1 = ::c6502::asm_ast;
namespace asm2 = ::c6502::asm2_ast;
using ::c6502::asdl::constructor_name;
using ::c6502::asdl::isa;

bool g_enabled = false;

std::string message(std::string_view after, std::string_view name,
                    const std::vector<std::string>& problems) {
  std::string out = "after " + std::string(after) + ", function '" + std::string(name) + "': ";
  for (std::size_t i = 0; i < problems.size() && i < 5; ++i) {
    if (i != 0) {
      out += "; ";
    }
    out += problems[i];
  }
  return out;
}

const std::string* jump_target(const tac::Type_instruction& instr) {
  switch (tac::kind(instr)) {
    case tac::Kind_instruction::Jump:
      return &std::get<tac::Jump>(instr).target;
    case tac::Kind_instruction::JumpIfTrue:
      return &std::get<tac::JumpIfTrue>(instr).target;
    case tac::Kind_instruction::JumpIfFalse:
      return &std::get<tac::JumpIfFalse>(instr).target;
    case tac::Kind_instruction::JumpIfCmp:
      return &std::get<tac::JumpIfCmp>(instr).target;
    case tac::Kind_instruction::JumpIfMasked:
      return &std::get<tac::JumpIfMasked>(instr).target;
    default:
      return nullptr;
  }
}

std::string control_skeleton(const std::vector<tac::Type_instruction>& instrs) {
  std::string out = std::to_string(instrs.size());
  for (std::size_t k = 0; k < instrs.size(); ++k) {
    const tac::Type_instruction& instr = instrs[k];
    if (const auto* label = std::get_if<tac::Label>(&instr)) {
      out += '\n' + std::to_string(k) + " L " + label->name;
    } else if (tac_cfg::is_terminator(instr)) {
      out += '\n' + std::to_string(k) + ' ' + std::to_string(instr.index());
      if (const std::string* target = jump_target(instr)) {
        out += ' ' + *target;
      }
    }
  }
  return out;
}

struct Span {
  int id = 0;
  std::size_t start = 0;
  std::size_t end = 0;
};
struct Shape {
  std::vector<Span> spans;
  OrderedMap<int, int> idom;
  std::map<std::string, int, std::less<>> label_block;
};

std::unordered_map<std::string, Shape>& shapes() {
  static std::unordered_map<std::string, Shape> table;
  return table;
}
constexpr std::size_t kShapesKept = 512;

const Shape& cfg_shape(const tac::Function& fn) {
  std::string key = control_skeleton(fn.instructions);
  auto& table = shapes();
  if (const auto it = table.find(key); it != table.end()) {
    return it->second;
  }
  const tac_cfg::Cfg cfg = tac_cfg::build_cfg(fn);
  Shape shape;
  shape.idom = tac_cfg::immediate_dominators(cfg);
  std::size_t at = 0;
  for (const int bid : cfg.block_order) {
    const auto& block = cfg.blocks.find(bid)->instructions;
    shape.spans.push_back(Span{.id = bid, .start = at, .end = at + block.size()});
    at += block.size();
    for (const auto& instr : block) {
      if (const auto* label = std::get_if<tac::Label>(&instr)) {
        shape.label_block.insert_or_assign(label->name, bid);
      }
    }
  }
  if (at != fn.instructions.size()) {
    throw InternalError("ir_check: the CFG's blocks don't hold the function's instructions");
  }
  if (table.size() >= kShapesKept) {
    table.clear();
  }
  return table.emplace(std::move(key), std::move(shape)).first->second;
}

using Names = std::vector<std::vector<std::string_view>>;

std::vector<std::string> ssa_problems(const tac::Function& fn, const NameSet& ssa_dsts,
                                      const Names& uses, const Names& defs) {
  std::vector<std::string> problems;
  const auto& instrs = fn.instructions;
  const Shape& shape = cfg_shape(fn);
  std::map<int, std::size_t> block_len;
  for (const Span& s : shape.spans) {
    block_len[s.id] = s.end - s.start;
  }
  struct Where {
    int block = 0;
    std::ptrdiff_t index = 0;
  };
  std::map<std::string, Where, std::less<>> def_at;
  for (const auto& p : fn.params) {
    if (ssa_dsts.contains(p)) {
      def_at.insert_or_assign(p, Where{.block = tac_cfg::kEntryId, .index = -1});
    }
  }
  for (const Span& s : shape.spans) {
    if (!shape.idom.contains(s.id)) {
      continue;
    }
    for (std::size_t i = s.start; i < s.end; ++i) {
      for (const std::string_view d : defs[i]) {
        if (!ssa_dsts.contains(d)) {
          continue;
        }
        const Where where{.block = s.id, .index = static_cast<std::ptrdiff_t>(i - s.start)};
        if (const auto it = def_at.find(d); it != def_at.end()) {
          problems.push_back("SSA name " + std::string(d) + " defined twice");
          it->second = where;
        } else {
          def_at.emplace(std::string(d), where);
        }
      }
    }
  }

  const auto dominates = [&](int a, int b) {
    int cur = b;
    while (true) {
      if (cur == a) {
        return true;
      }
      const int* next = shape.idom.find(cur);
      if (next == nullptr || *next == cur) {
        return false;
      }
      cur = *next;
    }
  };
  const auto check_read = [&](std::string_view name, int bid, std::ptrdiff_t k,
                              const std::string& what) {
    const auto it = def_at.find(name);
    if (it == def_at.end()) {
      problems.push_back(what + " reads " + std::string(name) + ", which nothing defines");
      return;
    }
    const auto [dbid, dk] = it->second;
    if (dbid == bid) {
      if (dk >= k) {
        problems.push_back(what + " reads " + std::string(name) + " before its definition");
      }
    } else if (!dominates(dbid, bid)) {
      problems.push_back(what + " reads " + std::string(name) +
                         ", whose definition doesn't dominate it");
    }
  };

  for (const Span& s : shape.spans) {
    if (!shape.idom.contains(s.id)) {
      continue;
    }
    for (std::size_t i = s.start; i < s.end; ++i) {
      const tac::Type_instruction& instr = instrs[i];
      if (const auto* phi = std::get_if<tac::Phi>(&instr)) {
        for (const auto& arg : phi->args) {
          const auto& a = std::get<tac::PhiArg>(arg);
          const auto* src = std::get_if<tac::Var>(&a.source);
          if (src == nullptr || !ssa_dsts.contains(src->name)) {
            continue;
          }
          const auto pred = shape.label_block.find(a.pred_label);
          if (pred == shape.label_block.end()) {
            problems.push_back("Phi argument from missing label " + a.pred_label);
            continue;
          }
          if (shape.idom.contains(pred->second)) {
            check_read(src->name, pred->second,
                       static_cast<std::ptrdiff_t>(block_len[pred->second]), "Phi");
          }
        }
        continue;
      }
      for (const std::string_view u : uses[i]) {
        if (ssa_dsts.contains(u)) {
          check_read(u, s.id, static_cast<std::ptrdiff_t>(i - s.start), constructor_name(instr));
        }
      }
    }
  }
  return problems;
}

std::vector<std::string> temp_problems(const tac::Function& fn, const Names& uses,
                                       const Names& defs) {
  std::set<std::string_view, std::less<>> defined(fn.params.begin(), fn.params.end());
  for (const auto& d : defs) {
    defined.insert(d.begin(), d.end());
  }
  std::set<std::string_view, std::less<>> in_memory;
  for (const auto& instr : fn.instructions) {
    switch (tac::kind(instr)) {
      case tac::Kind_instruction::GetAddress:
        if (const auto* v = std::get_if<tac::Var>(&std::get<tac::GetAddress>(instr).operand)) {
          in_memory.insert(v->name);
        }
        break;
      case tac::Kind_instruction::IndexedLoad:
        in_memory.insert(std::get<tac::IndexedLoad>(instr).name);
        break;
      case tac::Kind_instruction::IndexedSymbolStore:
        in_memory.insert(std::get<tac::IndexedSymbolStore>(instr).name);
        break;
      default:
        break;
    }
  }
  std::vector<std::string> problems;
  for (std::size_t i = 0; i < fn.instructions.size(); ++i) {
    for (const std::string_view name : uses[i]) {
      if (name.starts_with('%') && !defined.contains(name) && !in_memory.contains(name)) {
        problems.push_back(constructor_name(fn.instructions[i]) + " reads " + std::string(name) +
                           ", which nothing defines");
      }
    }
  }
  return problems;
}

bool chain_operand(const asm2::Type_operand& op, bool dest) {
  if (dest) {
    return isa<asm2::Data, asm2::ZP, asm2::Frame, asm2::Stack>(op);
  }
  return isa<asm2::Imm, asm2::ImmLabelLow, asm2::ImmLabelHigh, asm2::Data, asm2::ZP, asm2::Frame,
             asm2::Stack>(op);
}

bool is_a(const asm2::Type_operand& op) {
  const auto* reg = std::get_if<asm2::Reg>(&op);
  return reg != nullptr && std::holds_alternative<asm2::A>(reg->reg);
}

std::vector<std::string> helper_decimal_problems(
    const std::vector<asm2::Type_instruction>& instrs) {
  std::vector<std::string> problems;
  for (std::size_t i = 0; i < instrs.size(); ++i) {
    if (std::holds_alternative<asm2::Return>(instrs[i]) &&
        !(i > 0 && std::holds_alternative<asm2::ClearDecimal>(instrs[i - 1]))) {
      problems.push_back("RTS at " + std::to_string(i) + " without a CLD before it");
    }
  }
  if (instrs.empty() || !std::holds_alternative<asm2::Return>(instrs.back())) {
    problems.push_back("the helper doesn't end with an RTS");
  }
  return problems;
}

}  // namespace

void enable(bool on) { g_enabled = on; }

bool enabled() { return g_enabled; }

Enabled::Enabled(bool on) : before_(g_enabled) { g_enabled = on; }

Enabled::~Enabled() { g_enabled = before_; }

std::vector<std::string> tac_problems(const tac::Function& fn, const SymbolTable* symbols,
                                      const NameSet* ssa_dsts) {
  std::vector<std::string> problems;
  const auto& instrs = fn.instructions;
  std::set<std::string_view, std::less<>> labels;
  for (const auto& instr : instrs) {
    if (const auto* label = std::get_if<tac::Label>(&instr)) {
      if (labels.contains(label->name)) {
        problems.push_back("label " + label->name + " defined twice");
      }
      labels.insert(label->name);
    }
  }
  for (const auto& instr : instrs) {
    if (const std::string* target = jump_target(instr); target != nullptr &&
                                                         !labels.contains(*target)) {
      problems.push_back("jump to missing label " + *target);
    }
  }
  Names uses;
  Names defs;
  uses.reserve(instrs.size());
  defs.reserve(instrs.size());
  for (const auto& instr : instrs) {
    uses.push_back(passes::uses_in(instr));
    defs.push_back(recursion_saves::defs_in(instr));
  }
  if (symbols != nullptr) {
    std::set<std::string_view> names;
    for (const auto& u : uses) {
      names.insert(u.begin(), u.end());
    }
    for (const auto& d : defs) {
      names.insert(d.begin(), d.end());
    }
    for (const auto& p : fn.params) {
      names.erase(p);
    }
    for (const std::string_view n : names) {
      if (!symbols->contains(n)) {
        problems.push_back(std::string(n) + " isn't in the symbol table");
      }
    }
  }
  if (!problems.empty()) {
    return problems;
  }
  return ssa_dsts != nullptr ? ssa_problems(fn, *ssa_dsts, uses, defs)
                             : temp_problems(fn, uses, defs);
}

void check_tac(const tac::Function& fn, std::string_view after, const SymbolTable* symbols,
               const NameSet* ssa_dsts) {
  if (!g_enabled) {
    return;
  }
  const std::vector<std::string> problems = tac_problems(fn, symbols, ssa_dsts);
  if (!problems.empty()) {
    throw InternalError(message(after, fn.name, problems));
  }
}

std::vector<std::string> asm_problems(const asm1::Function& fn, bool pseudos, bool phis) {
  std::vector<std::string> problems;
  std::set<std::string_view, std::less<>> labels;
  for (const auto& instr : fn.instructions) {
    if (const auto* label = std::get_if<asm1::Label>(&instr)) {
      if (labels.contains(label->name)) {
        problems.push_back("label " + label->name + " defined twice");
      }
      labels.insert(label->name);
    }
  }
  for (const auto& instr : fn.instructions) {
    const asm1::Kind_instruction k = asm1::kind(instr);
    switch (k) {
      case asm1::Kind_instruction::Branch: {
        const auto& b = std::get<asm1::Branch>(instr);
        if (!labels.contains(b.target)) {
          problems.push_back("branch to missing label " + b.target);
        }
        break;
      }
      case asm1::Kind_instruction::Jump: {
        const auto& j = std::get<asm1::Jump>(instr);
        if (j.target.starts_with('.') && !labels.contains(j.target)) {
          problems.push_back("jump to missing label " + j.target);
        }
        break;
      }
      case asm1::Kind_instruction::JumpTable:
        for (const auto& t : std::get<asm1::JumpTable>(instr).targets) {
          if (t.starts_with('.') && !labels.contains(t)) {
            problems.push_back("jump table to missing label " + t);
          }
        }
        break;
      default:
        break;
    }
    if (!phis && k == asm1::Kind_instruction::Phi) {
      problems.push_back("Phi outside SSA form");
    }
    if (!pseudos) {
      const auto ops = optimizer::all_operands_in(instr);
      if (std::any_of(ops.begin(), ops.end(),
                      [](const asm1::Type_operand* op) { return isa<asm1::Pseudo>(*op); })) {
        problems.push_back("Pseudo left in " + constructor_name(instr));
      }
    }
    if (k == asm1::Kind_instruction::DecimalArith) {
      for (auto& p : decimal_arith::form_problems(std::get<asm1::DecimalArith>(instr))) {
        problems.push_back(std::move(p));
      }
    }
    if (k == asm1::Kind_instruction::SetDecimal || k == asm1::Kind_instruction::ClearDecimal) {
      problems.push_back(constructor_name(instr) + " outside a runtime helper");
    }
    const asm1::Type_operand* rmw = nullptr;
    switch (k) {
      case asm1::Kind_instruction::Inc:
        rmw = &std::get<asm1::Inc>(instr).dst;
        break;
      case asm1::Kind_instruction::Dec:
        rmw = &std::get<asm1::Dec>(instr).dst;
        break;
      case asm1::Kind_instruction::ArithmeticShiftLeft:
        rmw = &std::get<asm1::ArithmeticShiftLeft>(instr).dst;
        break;
      case asm1::Kind_instruction::LogicalShiftRight:
        rmw = &std::get<asm1::LogicalShiftRight>(instr).dst;
        break;
      case asm1::Kind_instruction::RotateLeft:
        rmw = &std::get<asm1::RotateLeft>(instr).dst;
        break;
      case asm1::Kind_instruction::RotateRight:
        rmw = &std::get<asm1::RotateRight>(instr).dst;
        break;
      default:
        break;
    }
    if (rmw != nullptr) {
      if (const auto* indexed = std::get_if<asm1::IndexedData>(rmw);
          indexed != nullptr && !std::holds_alternative<asm1::X>(indexed->index)) {
        problems.push_back(constructor_name(instr) + " of " + asdl::py_repr(*rmw) +
                           ", which has no 6502 addressing mode (only abs,X)");
      }
    }
  }
  return problems;
}

void check_asm(const asm1::Program& prog, std::string_view after, bool pseudos, bool phis) {
  if (!g_enabled) {
    return;
  }
  for (const auto& tl : prog.top_level) {
    if (const auto* fn = std::get_if<asm1::Function>(&tl)) {
      const std::vector<std::string> problems = asm_problems(*fn, pseudos, phis);
      if (!problems.empty()) {
        throw InternalError(message(after, fn->name, problems));
      }
    }
  }
}

std::vector<std::string> decimal_region_problems(
    const std::vector<asm2::Type_instruction>& instrs) {
  std::vector<std::string> problems;
  const std::size_t n = instrs.size();
  std::size_t i = 0;
  while (i < n) {
    const asm2::Type_instruction& ins = instrs[i];
    if (std::holds_alternative<asm2::ClearDecimal>(ins)) {
      problems.push_back("CLD at " + std::to_string(i) + " outside a BCD chain");
    }
    if (!std::holds_alternative<asm2::SetDecimal>(ins)) {
      ++i;
      continue;
    }
    const std::size_t start = i;
    ++i;
    bool subtract = false;
    if (i < n && std::holds_alternative<asm2::ClearCarry>(instrs[i])) {
      subtract = false;
    } else if (i < n && std::holds_alternative<asm2::SetCarry>(instrs[i])) {
      subtract = true;
    } else {
      problems.push_back("SED at " + std::to_string(start) + " isn't followed by CLC or SEC");
      continue;
    }
    ++i;
    std::size_t count = 0;
    while (i + 2 < n) {
      const auto* load = std::get_if<asm2::Mov>(&instrs[i]);
      if (load == nullptr || !is_a(load->dst) || !chain_operand(load->src, false)) {
        break;
      }
      const asm2::Type_operand* op_src = nullptr;
      const asm2::Type_operand* op_dst = nullptr;
      if (const auto* add = std::get_if<asm2::Add>(&instrs[i + 1]); add != nullptr && !subtract) {
        op_src = &add->src;
        op_dst = &add->dst;
      } else if (const auto* sub = std::get_if<asm2::Sub>(&instrs[i + 1]);
                 sub != nullptr && subtract) {
        op_src = &sub->src;
        op_dst = &sub->dst;
      }
      if (op_src == nullptr || !is_a(*op_dst) || !chain_operand(*op_src, false)) {
        break;
      }
      const auto* store = std::get_if<asm2::Mov>(&instrs[i + 2]);
      if (store == nullptr || !is_a(store->src) || !chain_operand(store->dst, true)) {
        break;
      }
      ++count;
      i += 3;
    }
    if (count == 0) {
      problems.push_back("the BCD chain at " + std::to_string(start) + " has no byte");
    }
    if (i >= n || !std::holds_alternative<asm2::ClearDecimal>(instrs[i])) {
      const std::string what = i < n ? constructor_name(instrs[i]) : std::string("the end");
      problems.push_back(what + " at " + std::to_string(i) +
                         " runs with the decimal flag set (SED at " + std::to_string(start) +
                         ")");
      continue;
    }
    ++i;
  }
  return problems;
}

void check_asm2(const asm2::Program& prog, std::string_view after) {
  if (!g_enabled) {
    return;
  }
  const std::vector<std::string>& helpers = rtlib::decimal_mode_helpers();
  for (const auto& tl : prog.top_level) {
    const auto* fn = std::get_if<asm2::Function>(&tl);
    if (fn == nullptr) {
      continue;
    }
    std::vector<std::string> problems = decimal_region_problems(fn->instructions);
    if (!problems.empty() &&
        std::find(helpers.begin(), helpers.end(), fn->name) != helpers.end()) {
      problems = helper_decimal_problems(fn->instructions);
    }
    if (!problems.empty()) {
      throw InternalError(message(after, fn->name, problems));
    }
  }
}

}  // namespace c6502::passes::ir_check
