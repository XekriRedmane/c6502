// Folds link-time address arithmetic, a label's address plus a
// constant, into one immediate.
//
// The pattern
//
// `&r[2].a` on a static array, or `prompts + 15`, reaches the asm as an
// add of 2 bytes of a label's halves and a constant:
//
//     LDA   #$2F                Mov(Imm(lo), A)
//     CLC                       ClearCarry          (before or after the LDA)
//     ADC   #<r                 Add(ImmLabelLow(r, o), A)
//     STA   d0                  Mov(A, D0)
//     LDA   #$00                Mov(Imm(hi), A)
//     ADC   #>r                 Add(ImmLabelHigh(r, o), A)
//     STA   d1                  Mov(A, D1)
//
// Either operand of each `ADC` may be the label. The sum is a constant
// at link time, so the pass writes
//
//     LDA   #<(r+47)
//     STA   d0
//     LDA   #>(r+47)
//     STA   d1
//
// dasm computes `r+47` as a full address, so the high half takes the
// carry out of the low one as the `ADC` did. That's 4 instructions in
// place of 7.
//
// The low byte alone, `LDA #<r; CLC; ADC #9; STA d0` with no high byte
// after it, becomes `LDA #<(r+9); STA d0`, since the low byte of the
// sum doesn't depend on the carry out. `(uint8_t)(uint16_t)&LOCS[1]`
// compiles to it.
//
// A byte may go to X or Y in place of memory (`TAX`), as the high byte
// of a returned pointer does, so `return T + 7;` becomes `LDA
// #<(T+7); STA t; LDA #>(T+7); TAX; LDA t`, which later passes shorten.
//
// Conditions
//
// C and V must be dead after the last store (the low byte's store, in
// the second form), since the `ADC`s set them and the loads don't. A
// and N/Z end the same, since A holds the high byte of the sum either
// way and both `ADC` and `LDA` set N/Z from it.
//
// Where it runs
//
// In the asm peephole fixed point, which both pipelines run. Memory
// value propagation then forwards the immediates to where `d0` and `d1`
// are read.
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

// An add of one byte that `byte_add` found: the label operand, the
// constant, the destination, and the index after the store.
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

// Returns the add of a byte that starts at `instrs[i]`: `LDA x; ADC y;
// STA dst` with one of x and y a label operand of the kind `Label`
// and the other an immediate, and for the low byte (`first`) a `CLC`
// before or after the `LDA`.
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
