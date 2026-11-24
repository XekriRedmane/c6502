// Lowers an asm program to the asm2 IR.
//
// The asm2 IR is the asm IR with every node one logical 6502
// instruction. The three compound nodes that `asm_emit` once expanded
// as it wrote the text, `AllocateStack`, `FunctionPrologue`, and `Ret`,
// don't exist in asm2. This pass rewrites each into the atoms it
// stands for, laid out in the IR where `asm_emit` used to spell them
// out. Every other instruction is copied at the asm2 type. `LoadAddress`
// stays one asm2 node, since its expansion is short enough to be one
// logical step that computes an address into two bytes, as it is in the
// asm IR.
//
// In Python the operands, the static initializers, the registers, and
// the conditions are distinct classes in the two modules and have to be
// rebuilt at the asm2 type. The C++ IR classes are distinct types for
// the same reason, and the walk below is as mechanical.
//
// The expansion is naive. The lowerings of the compound nodes leave out
// the `INY`, `TAX`, and `STX` tricks that the old expansions in
// `asm_emit` used to save bytes. Each `Mov(Reg(A), Stack(off))` loads Y
// again, and the path that restores FP moves the low byte through X
// with `TXA; STA` where a direct `STX` would do. The result is a byte
// longer per `FunctionPrologue` and two bytes longer per `Ret` with a
// frame than the old output, and in return the asm2 IR has a much
// smaller set of atoms and one shape for every operand.
// `sim.assembler._prologue_size`, `_ret_size`, `_emit_prologue`, and
// `_emit_ret` follow the same lowering, so that `instruction_size` and
// `assemble` stay byte for byte with what `asm_emit` writes.
#include "passes/asm_to_asm2.h"

#include <algorithm>
#include <string>
#include <utility>

#include "errors.h"
#include "passes/ir_check.h"
#include "pyrepr.h"

namespace c6502::passes {

namespace {

namespace in = ::c6502::asm_ast;
namespace out = ::c6502::asm2_ast;

using Instrs = std::vector<out::Type_instruction>;

// The runtime's zero-page symbols that the prologue and epilogue do
// their SSP and FP arithmetic on. The runtime header binds each with an
// `EQU`.
constexpr const char* kSsp = "SSP";
constexpr const char* kFp = "FP";

// A function's frame is too large for `LDY #offset` to reach. The
// message is FrameSizeError's, and the driver reports it as a compile
// error, since it is a limit of the program being compiled.
class FrameSize {
 public:
  explicit FrameSize(std::string message) : message(std::move(message)) {}
  std::string message;
};

out::Type_reg reg(const in::Type_reg& r) {
  switch (in::kind(r)) {
    case in::Kind_reg::A:
      return out::A{};
    case in::Kind_reg::X:
      return out::X{};
    case in::Kind_reg::Y:
      return out::Y{};
  }
  throw InternalError("unexpected reg");
}

out::Type_condition condition(const in::Type_condition& c) {
  switch (in::kind(c)) {
    case in::Kind_condition::CC:
      return out::CC{};
    case in::Kind_condition::CS:
      return out::CS{};
    case in::Kind_condition::EQ:
      return out::EQ{};
    case in::Kind_condition::MI:
      return out::MI{};
    case in::Kind_condition::NE:
      return out::NE{};
    case in::Kind_condition::PL:
      return out::PL{};
    case in::Kind_condition::VC:
      return out::VC{};
    case in::Kind_condition::VS:
      return out::VS{};
  }
  throw InternalError("unexpected condition");
}

void check_frame_offset(asdl::Int offset);

out::Type_operand operand(const in::Type_operand& op) {
  switch (in::kind(op)) {
    case in::Kind_operand::Imm:
      return out::Imm{.value = std::get<in::Imm>(op).value};
    case in::Kind_operand::Reg:
      return out::Reg{.reg = reg(std::get<in::Reg>(op).reg)};
    case in::Kind_operand::Pseudo:
      throw CompileError("ValueError",
                         "Pseudo(" + pyrepr::string(std::get<in::Pseudo>(op).name) +
                             ") reached asm_to_asm2; replace_pseudoregisters must run first");
    case in::Kind_operand::Stack:
      return out::Stack{.offset = std::get<in::Stack>(op).offset};
    case in::Kind_operand::Frame: {
      const asdl::Int offset = std::get<in::Frame>(op).offset;
      check_frame_offset(offset);
      return out::Frame{.offset = offset};
    }
    case in::Kind_operand::Data: {
      const auto& d = std::get<in::Data>(op);
      return out::Data{.name = d.name, .offset = d.offset};
    }
    case in::Kind_operand::Indirect:
      return out::Indirect{.offset = std::get<in::Indirect>(op).offset};
    case in::Kind_operand::IndirectY:
      return out::IndirectY{};
    case in::Kind_operand::IndirectZp: {
      const auto& z = std::get<in::IndirectZp>(op);
      return out::IndirectZp{.address = z.address, .offset = z.offset};
    }
    case in::Kind_operand::IndirectZpY:
      return out::IndirectZpY{.address = std::get<in::IndirectZpY>(op).address};
    case in::Kind_operand::ZP: {
      const auto& z = std::get<in::ZP>(op);
      return out::ZP{.address = z.address, .offset = z.offset};
    }
    case in::Kind_operand::ImmLabelLow: {
      const auto& l = std::get<in::ImmLabelLow>(op);
      return out::ImmLabelLow{.name = l.name, .offset = l.offset};
    }
    case in::Kind_operand::ImmLabelHigh: {
      const auto& h = std::get<in::ImmLabelHigh>(op);
      return out::ImmLabelHigh{.name = h.name, .offset = h.offset};
    }
    case in::Kind_operand::IndexedData: {
      const auto& x = std::get<in::IndexedData>(op);
      return out::IndexedData{.name = x.name, .offset = x.offset, .index = reg(x.index)};
    }
  }
  throw InternalError("unexpected operand");
}

out::Type_static_init static_init(const in::Type_static_init& it) {
  switch (in::kind(it)) {
    case in::Kind_static_init::CharInit:
      return out::CharInit{.value = std::get<in::CharInit>(it).value};
    case in::Kind_static_init::IntInit:
      return out::IntInit{.value = std::get<in::IntInit>(it).value};
    case in::Kind_static_init::LongInit:
      return out::LongInit{.value = std::get<in::LongInit>(it).value};
    case in::Kind_static_init::LongLongInit:
      return out::LongLongInit{.value = std::get<in::LongLongInit>(it).value};
    case in::Kind_static_init::FloatInit:
      return out::FloatInit{.bits = std::get<in::FloatInit>(it).bits};
    case in::Kind_static_init::DoubleInit:
      return out::DoubleInit{.bits = std::get<in::DoubleInit>(it).bits};
    case in::Kind_static_init::AddressInit: {
      const auto& a = std::get<in::AddressInit>(it);
      return out::AddressInit{.name = a.name, .offset = a.offset};
    }
    case in::Kind_static_init::AddressByteInit: {
      const auto& a = std::get<in::AddressByteInit>(it);
      return out::AddressByteInit{.name = a.name, .offset = a.offset, .high = a.high};
    }
    case in::Kind_static_init::StringInit: {
      const auto& s = std::get<in::StringInit>(it);
      return out::StringInit{.str = s.str, .bytes = s.bytes};
    }
    case in::Kind_static_init::ZeroInit:
      return out::ZeroInit{.bytes = std::get<in::ZeroInit>(it).bytes};
  }
  throw InternalError("unexpected static_init");
}

// The compound-node expansions. Each returns a list of asm2 atoms. The
// shapes are those `asm_emit._emit_function_prologue`, `_emit_ret`, and
// `_emit_ssp_sub` once wrote, without the `INY`, `TAX`, and `STX`
// tricks, which give way to two self-contained `Mov` atoms per word.

out::Reg reg_a() { return out::Reg{.reg = out::A{}}; }
out::Reg reg_x() { return out::Reg{.reg = out::X{}}; }
out::Data data(const char* name, asdl::Int offset) {
  return out::Data{.name = name, .offset = offset};
}
out::Imm imm(asdl::Int value) { return out::Imm{.value = value}; }

// Returns `SSP -= amt` as a 16-bit subtract: nothing when `amt` is 0,
// and otherwise `SEC` and an `LDA`, `SBC`, `STA` for each byte.
void ssp_sub(asdl::Int amt, Instrs& outs) {
  if (amt == 0) {
    return;
  }
  if (!(0 <= amt && amt <= 0xFFFF)) {
    throw CompileError("ValueError",
                       "AllocateStack amt " + pyrepr::integer(amt) + " out of range 0..65535");
  }
  const asdl::Int lo = amt & 0xFF;
  const asdl::Int hi = (amt >> 8) & 0xFF;
  outs.push_back(out::SetCarry{});
  outs.push_back(out::Mov{.src = data(kSsp, 0), .dst = reg_a()});
  outs.push_back(out::Sub{.src = imm(lo), .dst = reg_a()});
  outs.push_back(out::Mov{.src = reg_a(), .dst = data(kSsp, 0)});
  outs.push_back(out::Mov{.src = data(kSsp, 1), .dst = reg_a()});
  outs.push_back(out::Sub{.src = imm(hi), .dst = reg_a()});
  outs.push_back(out::Mov{.src = reg_a(), .dst = data(kSsp, 1)});
}

// Returns `FP = SSP` (16-bit).
void set_fp_to_ssp(Instrs& outs) {
  outs.push_back(out::Mov{.src = data(kSsp, 0), .dst = reg_a()});
  outs.push_back(out::Mov{.src = reg_a(), .dst = data(kFp, 0)});
  outs.push_back(out::Mov{.src = data(kSsp, 1), .dst = reg_a()});
  outs.push_back(out::Mov{.src = reg_a(), .dst = data(kFp, 1)});
}

// Returns `SSP = FP + amt` (16-bit).
void set_ssp_to_fp_plus(asdl::Int amt, Instrs& outs) {
  if (!(0 <= amt && amt <= 0xFFFF)) {
    throw CompileError("ValueError",
                       "epilogue rewind " + pyrepr::integer(amt) + " out of range 0..65535");
  }
  if (amt == 0) {
    // The shape of `FP = SSP`, with the source and destination swapped.
    outs.push_back(out::Mov{.src = data(kFp, 0), .dst = reg_a()});
    outs.push_back(out::Mov{.src = reg_a(), .dst = data(kSsp, 0)});
    outs.push_back(out::Mov{.src = data(kFp, 1), .dst = reg_a()});
    outs.push_back(out::Mov{.src = reg_a(), .dst = data(kSsp, 1)});
    return;
  }
  const asdl::Int lo = amt & 0xFF;
  const asdl::Int hi = (amt >> 8) & 0xFF;
  outs.push_back(out::ClearCarry{});
  outs.push_back(out::Mov{.src = data(kFp, 0), .dst = reg_a()});
  outs.push_back(out::Add{.src = imm(lo), .dst = reg_a()});
  outs.push_back(out::Mov{.src = reg_a(), .dst = data(kSsp, 0)});
  outs.push_back(out::Mov{.src = data(kFp, 1), .dst = reg_a()});
  outs.push_back(out::Add{.src = imm(hi), .dst = reg_a()});
  outs.push_back(out::Mov{.src = reg_a(), .dst = data(kSsp, 1)});
}

// Checks that `m` local bytes leave `Stack(m+2)` and `Frame(m+2)` in
// reach of an `LDY` immediate, since m+2 must fit in a byte.
void check_local_bytes(asdl::Int m) {
  if (!(0 <= m && m <= 253)) {
    const std::string n = pyrepr::integer(m);
    throw FrameSize("its locals need " + n +
                    " bytes on the soft stack, but a frame holds at most 253 (local_bytes " +
                    n +
                    " out of range 0..253, limited by the `LDY #offset` of each frame "
                    "access); a large array fits better as a `static` or a global");
  }
}

// Checks that a frame access's offset is in reach of the `LDY #offset`
// before its `(FP),Y`. The arguments sit above the locals and the saved
// FP, and a function whose locals fit in 253 bytes may still read an
// argument byte past Frame(255). Only the accesses count: an argument
// that's never read through the frame, such as a struct copied out by
// address, may sit past Frame(255) without harm. Without this check
// `asm_emit` met such an access and raised a ValueError with no
// function named (`fuzz.exprs` seed 43 without -O).
void check_frame_offset(asdl::Int offset) {
  if (offset > 0xFF) {
    const std::string n = pyrepr::integer(offset);
    throw FrameSize("it reads or writes the byte at frame offset " + n +
                    ", but the `LDY #offset` of each frame access reaches at most 255 (frame "
                    "offset " +
                    n + " out of range 1..255); a large array fits better as a `static` or a "
                    "global");
  }
}

void check_byte(const char* label, asdl::Int v) {
  if (!(0 <= v && v <= 0xFF)) {
    throw CompileError("ValueError",
                       std::string(label) + " " + pyrepr::integer(v) + " out of range 0..255");
  }
}

// Writes the current FP into Stack(m+1) and Stack(m+2). Each
// `Mov(Reg(A), Stack(off))` loads Y again, which is a byte more than
// the `INY` form `_emit_function_prologue` used to write, and keeps
// every atom self-contained.
void save_fp_into_slot(asdl::Int m, Instrs& outs) {
  check_local_bytes(m);
  outs.push_back(out::Mov{.src = data(kFp, 0), .dst = reg_a()});
  outs.push_back(out::Mov{.src = reg_a(), .dst = out::Stack{.offset = m + 1}});
  outs.push_back(out::Mov{.src = data(kFp, 1), .dst = reg_a()});
  outs.push_back(out::Mov{.src = reg_a(), .dst = out::Stack{.offset = m + 2}});
}

// Restores FP from Frame(m+1) and Frame(m+2). The low byte goes through
// X (`TAX`, `TXA`) so that it survives the read and the store of the
// high byte. X is free to change, since no return convention holds data
// there. That is two bytes more than the `INY` and `STX` of the old
// `_emit_ret`.
void restore_fp_from_slot(asdl::Int m, Instrs& outs) {
  check_local_bytes(m);
  outs.push_back(out::Mov{.src = out::Frame{.offset = m + 1}, .dst = reg_a()});
  outs.push_back(out::Mov{.src = reg_a(), .dst = reg_x()});
  outs.push_back(out::Mov{.src = out::Frame{.offset = m + 2}, .dst = reg_a()});
  outs.push_back(out::Mov{.src = reg_a(), .dst = data(kFp, 1)});
  outs.push_back(out::Mov{.src = reg_x(), .dst = reg_a()});
  outs.push_back(out::Mov{.src = reg_a(), .dst = data(kFp, 0)});
}

// Saves the byte at `$zp_addr` into the frame slot at
// `FP+frame_offset`, for the prologue's callee-saved bytes.
void save_zp_byte_to_frame(asdl::Int zp_addr, asdl::Int frame_offset, Instrs& outs) {
  check_byte("callee-saved address", zp_addr);
  outs.push_back(out::Mov{.src = out::ZP{.address = zp_addr, .offset = 0}, .dst = reg_a()});
  outs.push_back(out::Mov{.src = reg_a(), .dst = out::Frame{.offset = frame_offset}});
}

// Restores the byte at `$zp_addr` from `FP+frame_offset`, for the
// epilogue's callee-saved bytes.
void restore_zp_byte_from_frame(asdl::Int zp_addr, asdl::Int frame_offset, Instrs& outs) {
  check_byte("callee-saved address", zp_addr);
  outs.push_back(out::Mov{.src = out::Frame{.offset = frame_offset}, .dst = reg_a()});
  outs.push_back(out::Mov{.src = reg_a(), .dst = out::ZP{.address = zp_addr, .offset = 0}});
}

void function_prologue(const in::FunctionPrologue& p, Instrs& outs) {
  if (p.arg_bytes + p.local_bytes == 0) {
    // No arguments were pushed and no locals need room, so there is no
    // frame to set up. Callee-saved bytes would have nowhere to go
    // without a frame, so the saves are part of the frame's setup.
    return;
  }
  std::string note = "prologue: " + pyrepr::integer(p.arg_bytes) + " arg bytes, " +
                     pyrepr::integer(p.local_bytes) + " local bytes";
  if (!p.callee_saved_addrs.empty()) {
    note += ", " + std::to_string(p.callee_saved_addrs.size()) + " callee-saved bytes";
  }
  outs.push_back(out::Comment{.text = std::move(note)});
  ssp_sub(p.local_bytes + 2, outs);
  save_fp_into_slot(p.local_bytes, outs);
  set_fp_to_ssp(outs);
  for (std::size_t i = 0; i < p.callee_saved_addrs.size(); ++i) {
    save_zp_byte_to_frame(p.callee_saved_addrs[i], static_cast<asdl::Int>(static_cast<int>(i) + 1),
                          outs);
  }
  outs.push_back(out::Blank{});
}

void ret(const in::Ret& r, Instrs& outs) {
  if (r.arg_bytes + r.local_bytes == 0) {
    // No frame to tear down, so the return is a bare `RTS`, which
    // leaves A and X as they are.
    outs.push_back(out::Return{});
    return;
  }
  const asdl::Int rewind = r.arg_bytes + r.local_bytes + 2;
  outs.push_back(out::Blank{});
  outs.push_back(out::Comment{.text = "epilogue"});
  // The return bytes wait on the hardware stack through the whole
  // epilogue. Restoring the callee-saved bytes uses A as scratch, and
  // the SSP and FP teardown uses A for its 16-bit add and X for FP's low
  // byte in `restore_fp_from_slot`, so either would destroy them. A
  // 2-byte register return has its low byte in A and its high byte in X,
  // and a 1-byte return only A. They are saved before the restore.
  if (r.save_x) {
    // The low byte (A) goes first, then the high byte through A.
    outs.push_back(out::Push{.src = reg_a()});
    outs.push_back(out::Mov{.src = reg_x(), .dst = reg_a()});
    outs.push_back(out::Push{.src = reg_a()});
  } else if (r.save_a) {
    outs.push_back(out::Push{.src = reg_a()});
  }
  // A carry-flag return (`return_bool_in_carry`) has its value in C,
  // which the teardown's `CLC; ADC` overwrites, so the status register
  // waits on the hardware stack around it.
  if (r.save_c) {
    outs.push_back(out::PushStatus{});
  }
  // The callee-saved bytes are restored before the SSP and FP teardown,
  // while FP is still valid for the indirect-Y reads.
  for (std::size_t i = 0; i < r.callee_saved_addrs.size(); ++i) {
    restore_zp_byte_from_frame(r.callee_saved_addrs[i],
                               static_cast<asdl::Int>(static_cast<int>(i) + 1), outs);
  }
  set_ssp_to_fp_plus(rewind, outs);
  restore_fp_from_slot(r.local_bytes, outs);
  if (r.save_c) {
    outs.push_back(out::PullStatus{});
  }
  if (r.save_x) {
    // The high byte goes to X, then the low byte to A, which is the
    // convention (A low, X high) for the `RTS`.
    outs.push_back(out::Pop{.dst = reg_a()});
    outs.push_back(out::Mov{.src = reg_a(), .dst = reg_x()});
    outs.push_back(out::Pop{.dst = reg_a()});
  } else if (r.save_a) {
    outs.push_back(out::Pop{.dst = reg_a()});
  }
  outs.push_back(out::Return{});
}

// Appends the asm2 atoms of one asm instruction to `outs`. Most
// instructions become one atom, and the three compound nodes
// (`AllocateStack`, `FunctionPrologue`, `Ret`) become several.
void instruction(const in::Type_instruction& instr, Instrs& outs) {
  switch (in::kind(instr)) {
    case in::Kind_instruction::Mov: {
      const auto& m = std::get<in::Mov>(instr);
      if (m.is_volatile && m.src == m.dst && !asdl::isa<in::Reg>(m.src)) {
        // `v = v;` on a volatile `v` reads and writes it. The emitter
        // drops a `Mov` whose operands match, so the `LDA v; STA v` pair
        // is spelled out.
        outs.push_back(out::Mov{.src = operand(m.src), .dst = reg_a()});
        outs.push_back(out::Mov{.src = reg_a(), .dst = operand(m.dst)});
        return;
      }
      outs.push_back(out::Mov{.src = operand(m.src), .dst = operand(m.dst)});
      return;
    }
    case in::Kind_instruction::Add: {
      const auto& a = std::get<in::Add>(instr);
      outs.push_back(out::Add{.src = operand(a.src), .dst = operand(a.dst)});
      return;
    }
    case in::Kind_instruction::Sub: {
      const auto& s = std::get<in::Sub>(instr);
      outs.push_back(out::Sub{.src = operand(s.src), .dst = operand(s.dst)});
      return;
    }
    case in::Kind_instruction::Call:
      outs.push_back(out::Call{.name = std::get<in::Call>(instr).name});
      return;
    case in::Kind_instruction::ClearCarry:
      outs.push_back(out::ClearCarry{});
      return;
    case in::Kind_instruction::SetCarry:
      outs.push_back(out::SetCarry{});
      return;
    case in::Kind_instruction::Inc:
      outs.push_back(out::Inc{.dst = operand(std::get<in::Inc>(instr).dst)});
      return;
    case in::Kind_instruction::Dec:
      outs.push_back(out::Dec{.dst = operand(std::get<in::Dec>(instr).dst)});
      return;
    case in::Kind_instruction::Push:
      outs.push_back(out::Push{.src = operand(std::get<in::Push>(instr).src)});
      return;
    case in::Kind_instruction::Pop:
      outs.push_back(out::Pop{.dst = operand(std::get<in::Pop>(instr).dst)});
      return;
    case in::Kind_instruction::DecimalArith:
      for (auto& atom : expand_decimal_arith(std::get<in::DecimalArith>(instr))) {
        outs.push_back(std::move(atom));
      }
      return;
    case in::Kind_instruction::SetDecimal:
      outs.push_back(out::SetDecimal{});
      return;
    case in::Kind_instruction::ClearDecimal:
      outs.push_back(out::ClearDecimal{});
      return;
    case in::Kind_instruction::Xor: {
      const auto& x = std::get<in::Xor>(instr);
      outs.push_back(
          out::Xor{.src1 = operand(x.src1), .src2 = operand(x.src2), .dst = operand(x.dst)});
      return;
    }
    case in::Kind_instruction::And: {
      const auto& a = std::get<in::And>(instr);
      outs.push_back(out::And{.src = operand(a.src), .dst = operand(a.dst)});
      return;
    }
    case in::Kind_instruction::Or: {
      const auto& o = std::get<in::Or>(instr);
      outs.push_back(out::Or{.src = operand(o.src), .dst = operand(o.dst)});
      return;
    }
    case in::Kind_instruction::ArithmeticShiftLeft:
      outs.push_back(
          out::ArithmeticShiftLeft{.dst = operand(std::get<in::ArithmeticShiftLeft>(instr).dst)});
      return;
    case in::Kind_instruction::LogicalShiftRight:
      outs.push_back(
          out::LogicalShiftRight{.dst = operand(std::get<in::LogicalShiftRight>(instr).dst)});
      return;
    case in::Kind_instruction::RotateLeft:
      outs.push_back(out::RotateLeft{.dst = operand(std::get<in::RotateLeft>(instr).dst)});
      return;
    case in::Kind_instruction::RotateRight:
      outs.push_back(out::RotateRight{.dst = operand(std::get<in::RotateRight>(instr).dst)});
      return;
    case in::Kind_instruction::Label:
      outs.push_back(out::Label{.name = std::get<in::Label>(instr).name});
      return;
    case in::Kind_instruction::Jump:
      outs.push_back(out::Jump{.target = std::get<in::Jump>(instr).target});
      return;
    case in::Kind_instruction::JumpTable: {
      const auto& t = std::get<in::JumpTable>(instr);
      outs.push_back(
          out::JumpTable{.table = t.table, .index = t.index, .base = t.base, .targets = t.targets});
      return;
    }
    case in::Kind_instruction::Branch: {
      const auto& b = std::get<in::Branch>(instr);
      outs.push_back(out::Branch{.cond = condition(b.cond), .target = b.target});
      return;
    }
    case in::Kind_instruction::Compare: {
      const auto& c = std::get<in::Compare>(instr);
      outs.push_back(out::Compare{.left = operand(c.left), .right = operand(c.right)});
      return;
    }
    case in::Kind_instruction::BitTest:
      outs.push_back(out::BitTest{.src = operand(std::get<in::BitTest>(instr).src)});
      return;
    case in::Kind_instruction::LoadAddress: {
      const auto& l = std::get<in::LoadAddress>(instr);
      outs.push_back(out::LoadAddress{.src = operand(l.src), .dst = operand(l.dst)});
      return;
    }
    case in::Kind_instruction::AllocateStack:
      ssp_sub(std::get<in::AllocateStack>(instr).bytes, outs);
      return;
    case in::Kind_instruction::FunctionPrologue:
      function_prologue(std::get<in::FunctionPrologue>(instr), outs);
      return;
    case in::Kind_instruction::Ret:
      ret(std::get<in::Ret>(instr), outs);
      return;
    case in::Kind_instruction::Return:
      // A bare exit, just `RTS`, with no SSP or FP teardown. By the time
      // the program reaches asm2, `synthesize_prologue` has chosen
      // between this and a `Ret` with a frame, so the `save_a` the node
      // carries is dropped.
      outs.push_back(out::Return{});
      return;
    case in::Kind_instruction::Phi:
      // A Phi is an SSA node, which SSA destruction lowers to copies
      // long before this pass sees the program.
      throw CompileError("TypeError", "asm_to_asm2: Phi node leaked past SSA destruction");
  }
  throw InternalError("unexpected instruction");
}

}  // namespace

std::vector<asm2_ast::Type_instruction> expand_decimal_arith(
    const asm_ast::DecimalArith& instr) {
  // The decimal flag is set for the ADC or SBC of each byte and for
  // nothing else. A soft-stack operand's `LDY #off` is part of its
  // access. The IR check (`ir_check.check_asm2`) reads the result back
  // to confirm the shape.
  Instrs outs;
  outs.push_back(out::SetDecimal{});
  if (instr.subtract) {
    outs.push_back(out::SetCarry{});
  } else {
    outs.push_back(out::ClearCarry{});
  }
  const std::size_t n =
      std::min({instr.left.size(), instr.right.size(), instr.dst.size()});
  for (std::size_t k = 0; k < n; ++k) {
    outs.push_back(out::Mov{.src = operand(instr.left[k]), .dst = reg_a()});
    if (instr.subtract) {
      outs.push_back(out::Sub{.src = operand(instr.right[k]), .dst = reg_a()});
    } else {
      outs.push_back(out::Add{.src = operand(instr.right[k]), .dst = reg_a()});
    }
    outs.push_back(out::Mov{.src = reg_a(), .dst = operand(instr.dst[k])});
  }
  outs.push_back(out::ClearDecimal{});
  return outs;
}

asm2_ast::Program lower_to_asm2(const asm_ast::Program& prog) {
  out::Program result;
  result.top_level.reserve(prog.top_level.size());
  for (const auto& tl : prog.top_level) {
    if (const auto* fn = asdl::as<in::Function>(tl)) {
      out::Function f{.name = fn->name, .is_global = fn->is_global, .params = fn->params};
      f.instructions.reserve(fn->instructions.size());
      try {
        for (const auto& instr : fn->instructions) {
          instruction(instr, f.instructions);
        }
      } catch (const FrameSize& e) {
        throw CompileError("FrameSizeError",
                           "function " + pyrepr::string(fn->name) + ": " + e.message);
      }
      result.top_level.emplace_back(std::move(f));
    } else {
      const auto& sv = std::get<in::StaticVariable>(tl);
      out::StaticVariable s{.name = sv.name, .is_global = sv.is_global};
      s.init.reserve(sv.init.size());
      for (const auto& it : sv.init) {
        s.init.push_back(static_init(it));
      }
      result.top_level.emplace_back(std::move(s));
    }
  }
  // With the IR checks on, the decimal flag must be set only across a
  // packed BCD chain (src/passes/ir_check.h).
  ir_check::check_asm2(result, "asm_to_asm2");
  return result;
}

}  // namespace c6502::passes
