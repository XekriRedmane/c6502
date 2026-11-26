// Splits each `LoadAddress` of a `Data` source into two single-byte
// moves of `ImmLabel` immediates.
//
// `LoadAddress(src=Data(label, off), dst=<memory>)` writes the 2-byte
// address `&(label+off)` into `dst`. Emission
// (`Emitter::load_address` in asm_emit.cpp) expands it to four
// instructions,
// `LDA #<(label+off); STA dst.lo; LDA #>(label+off); STA dst.hi`, but
// hidden inside one IR instruction, which keeps two peepholes that
// reason byte by byte from seeing it.
//
//   * `apply_memory_value_propagation` treats `LoadAddress` as a barrier
//     that clears its state, with `Call`, `FunctionPrologue`,
//     `AllocateStack`, and `Phi`, so it loses the facts that `dst.lo`
//     holds `ImmLabelLow(label)` and `dst.hi` holds `ImmLabelHigh(label)`
//     that the spelled-out moves give. Reads of those bytes later in the
//     function survive where they would fold to the immediates.
//   * `Emitter::load_address` writes the high byte to
//     `Data(dst.name, dst.offset + 1)` (`shift_offset`). When that byte
//     has a slot symbol of its own, as each byte of a 2-byte numbered
//     temporary of the pool does (`__local_fn__0` and `__local_fn__1`),
//     `dst.name+1` is the second symbol's address under another name,
//     and the peepholes keyed by name can't see that the two are one
//     byte.
//
// A `LoadAddress` of a `Frame` source (the address of a local on the
// soft stack) stays, since its `FP + off` needs the compound
// `CLC; LDA; ADC` chain. The pass runs after
// `replace_program_bare_exit`, which resolves the source to a
// `Data` or a `Frame`, and before the peephole fixed point, so that
// `apply_memory_value_propagation` sees the split form.

#include "passes/lower_data_load_address.h"

#include <utility>
#include <vector>

#include "errors.h"

namespace c6502::passes {

namespace ir = ::c6502::asm_ast;

namespace {

using SlotSymbols = OrderedMap<std::string, asdl::Int>;
using ByteNames = OrderedMap<asdl::Int, std::string>;

bool is_memory_operand(const ir::Type_operand& op) {
  return asdl::isa<ir::Data, ir::ZP, ir::Frame, ir::Stack>(op);
}

ir::Type_operand shift_offset(const ir::Type_operand& op, asdl::Int k) {
  using K = ir::Kind_operand;
  switch (ir::kind(op)) {
    case K::Data: {
      const auto& d = std::get<ir::Data>(op);
      return ir::Data{.name = d.name, .offset = d.offset + k};
    }
    case K::ZP: {
      const auto& z = std::get<ir::ZP>(op);
      return ir::ZP{.address = z.address, .offset = z.offset + k};
    }
    case K::Frame:
      return ir::Frame{.offset = std::get<ir::Frame>(op).offset + k};
    case K::Stack:
      return ir::Stack{.offset = std::get<ir::Stack>(op).offset + k};
    default:
      throw InternalError("can't shift offset on operand " + asdl::dump(op));
  }
}

ir::Type_operand high_byte(const ir::Type_operand& dst, const SlotSymbols& slot_symbols,
                           const ByteNames& byte_names) {
  if (const auto* d = asdl::as<ir::Data>(dst)) {
    if (const asdl::Int* base = slot_symbols.find(d->name)) {
      const asdl::Int addr = *base + d->offset + 1;
      if (const std::string* name = byte_names.find(addr)) {
        const asdl::Int* at = slot_symbols.find(*name);
        if (at != nullptr && *at == addr) {
          return ir::Data{.name = *name, .offset = 0};
        }
      }
    }
  }
  return shift_offset(dst, 1);
}

void lower_in_function(ir::Function& fn, const SlotSymbols& slot_symbols,
                       const ByteNames& byte_names) {
  bool changed = false;
  for (const auto& instr : fn.instructions) {
    const auto* la = asdl::as<ir::LoadAddress>(instr);
    if (la != nullptr && asdl::isa<ir::Data>(la->src) && is_memory_operand(la->dst)) {
      changed = true;
      break;
    }
  }
  if (!changed) {
    return;
  }
  std::vector<ir::Type_instruction> out;
  out.reserve(fn.instructions.size() + 4);
  for (auto& instr : fn.instructions) {
    const auto* la = asdl::as<ir::LoadAddress>(instr);
    if (la == nullptr || !asdl::isa<ir::Data>(la->src) || !is_memory_operand(la->dst)) {
      out.push_back(std::move(instr));
      continue;
    }
    const auto& src = std::get<ir::Data>(la->src);
    ir::Type_operand dst_hi = high_byte(la->dst, slot_symbols, byte_names);
    out.push_back(
        ir::Mov{.src = ir::ImmLabelLow{.name = src.name, .offset = src.offset}, .dst = la->dst});
    out.push_back(ir::Mov{.src = ir::ImmLabelHigh{.name = src.name, .offset = src.offset},
                          .dst = std::move(dst_hi)});
  }
  fn.instructions = std::move(out);
}

}  // namespace

ir::Program lower_data_load_address(ir::Program prog, const SlotSymbols* slot_symbols,
                                    const ByteNamesByFunction* byte_names) {
  static const SlotSymbols kNoSymbols;
  static const ByteNames kNoNames;
  for (auto& tl : prog.top_level) {
    auto* fn = asdl::as<ir::Function>(tl);
    if (fn == nullptr) {
      continue;
    }
    const ByteNames* names = byte_names != nullptr ? byte_names->find(fn->name) : nullptr;
    lower_in_function(*fn, slot_symbols != nullptr ? *slot_symbols : kNoSymbols,
                      names != nullptr ? *names : kNoNames);
  }
  return prog;
}

}  // namespace c6502::passes
