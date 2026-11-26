// dasm computes `#>(r+k)` from the full address `r+k`, so the high half
// takes the carry out of the low one as the `ADC` did.
#include "passes/label_offset_fold.h"

#include <cstddef>
#include <optional>
#include <utility>
#include <vector>

#include "passes/asm_liveness.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::asm_ast;
using ::c6502::asdl::as;
using ::c6502::asdl::isa;

const ir::Type_operand kA = ir::Reg{.reg = ir::A{}};

struct ByteAdd {
  ir::Type_operand label;
  asdl::Int constant = 0;
  ir::Type_operand dst;
  std::size_t next = 0;
};

const std::string& label_name(const ir::Type_operand& op) {
  if (const auto* lo = as<ir::ImmLabelLow>(op)) {
    return lo->name;
  }
  return std::get<ir::ImmLabelHigh>(op).name;
}

asdl::Int label_offset(const ir::Type_operand& op) {
  if (const auto* lo = as<ir::ImmLabelLow>(op)) {
    return lo->offset;
  }
  return std::get<ir::ImmLabelHigh>(op).offset;
}

template <class Label>
std::optional<ByteAdd> byte_add(const std::vector<ir::Type_instruction>& instrs,
                                std::size_t i, bool first) {
  std::size_t j = i;
  bool clc = false;
  if (first && j < instrs.size() && isa<ir::ClearCarry>(instrs[j])) {
    clc = true;
    ++j;
  }
  if (j >= instrs.size()) {
    return std::nullopt;
  }
  const auto* load = as<ir::Mov>(instrs[j]);
  if (load == nullptr || load->dst != kA || load->is_volatile) {
    return std::nullopt;
  }
  ++j;
  if (first && !clc) {
    if (j >= instrs.size() || !isa<ir::ClearCarry>(instrs[j])) {
      return std::nullopt;
    }
    ++j;
  }
  if (j + 1 >= instrs.size()) {
    return std::nullopt;
  }
  const auto* add = as<ir::Add>(instrs[j]);
  const auto* store = as<ir::Mov>(instrs[j + 1]);
  if (add == nullptr || add->dst != kA) {
    return std::nullopt;
  }
  if (store == nullptr || store->src != kA || store->is_volatile) {
    return std::nullopt;
  }
  bool dst_ok = isa<ir::Data, ir::ZP>(store->dst);
  if (const auto* r = as<ir::Reg>(store->dst)) {
    dst_ok = isa<ir::X, ir::Y>(r->reg);
  }
  if (!dst_ok) {
    return std::nullopt;
  }
  const ir::Type_operand* pairs[2][2] = {{&load->src, &add->src},
                                         {&add->src, &load->src}};
  for (const auto& pair : pairs) {
    const auto* imm = as<ir::Imm>(*pair[1]);
    if (isa<Label>(*pair[0]) && imm != nullptr) {
      return ByteAdd{*pair[0], imm->value, store->dst, j + 2};
    }
  }
  return std::nullopt;
}

void rewrite_function(ir::Function& fn) {
  const auto& instrs = fn.instructions;
  const LabelMap labels(instrs);
  std::vector<ir::Type_instruction> out;
  bool changed = false;
  std::size_t i = 0;
  while (i < instrs.size()) {
    const auto low = byte_add<ir::ImmLabelLow>(instrs, i, true);
    std::optional<ByteAdd> high;
    if (low) {
      high = byte_add<ir::ImmLabelHigh>(instrs, low->next, false);
    }
    if (low && high && label_name(low->label) == label_name(high->label) &&
        label_offset(low->label) == label_offset(high->label) &&
        c_dead_at(instrs, labels, high->next) &&
        v_dead_at(instrs, labels, high->next)) {
      const std::string name = label_name(low->label);
      const asdl::Int off =
          label_offset(low->label) + ((low->constant + 256 * high->constant) & 0xFFFF);
      out.push_back(ir::Mov{.src = ir::ImmLabelLow{.name = name, .offset = off}, .dst = kA});
      out.push_back(ir::Mov{.src = kA, .dst = low->dst});
      out.push_back(ir::Mov{.src = ir::ImmLabelHigh{.name = name, .offset = off}, .dst = kA});
      out.push_back(ir::Mov{.src = kA, .dst = high->dst});
      i = high->next;
      changed = true;
      continue;
    }
    if (low && c_dead_at(instrs, labels, low->next) &&
        v_dead_at(instrs, labels, low->next)) {
      const asdl::Int off = label_offset(low->label) + low->constant;
      out.push_back(ir::Mov{
          .src = ir::ImmLabelLow{.name = label_name(low->label), .offset = off}, .dst = kA});
      out.push_back(ir::Mov{.src = kA, .dst = low->dst});
      i = low->next;
      changed = true;
      continue;
    }
    out.push_back(instrs[i]);
    ++i;
  }
  if (changed) {
    fn.instructions = std::move(out);
  }
}

}  // namespace

asm_ast::Program apply_label_offset_fold(asm_ast::Program prog) {
  for (auto& tl : prog.top_level) {
    if (auto* fn = as<ir::Function>(tl)) {
      rewrite_function(*fn);
    }
  }
  return prog;
}

}  // namespace c6502::passes
