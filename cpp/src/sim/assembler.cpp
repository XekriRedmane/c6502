// The instruction sizes of the simulator's in-process assembler
// (sim/assembler.py), which `long_branches`, `short_jumps`,
// `tail_merge`, `shared_tails`, `outline_runs`, and `rotate_loop_test`
// read. Each size must equal the length of the bytes the Python
// assembler emits for the instruction, and `asm_emit` writes the same
// instructions. The prologue and epilogue sizes follow the naive
// expansions of `passes.asm_to_asm2`, in which each `Mov` loads its own
// Y.
#include "sim/assembler.h"

#include <algorithm>
#include <string>

#include "errors.h"

namespace c6502::sim {

namespace {

namespace ir = ::c6502::asm_ast;
using ::c6502::asdl::as;
using ::c6502::asdl::Int;
using ::c6502::asdl::isa;

[[noreturn]] void fail(const std::string& message) {
  throw InternalError("AssemblerError: " + message);
}

// The sizes, with the set of names that resolve to zero page.
class Sizer {
 public:
  explicit Sizer(const passes::NameSet* zp_names) : zp_names_(zp_names) {}

  int instr_size(const ir::Type_instruction& instr) const;

 private:
  bool is_reg_a(const ir::Type_operand& op) const {
    const auto* r = as<ir::Reg>(op);
    return r != nullptr && isa<ir::A>(r->reg);
  }
  bool is_reg_xy(const ir::Type_operand& op) const {
    const auto* r = as<ir::Reg>(op);
    return r != nullptr && isa<ir::X, ir::Y>(r->reg);
  }
  // The operands written with 6502 `(zp),Y` addressing and an `LDY #`
  // in front: Stack (through SSP), Frame (through FP), and Indirect
  // (through DPTR).
  static bool is_indirect_y(const ir::Type_operand& op) {
    return isa<ir::Stack, ir::Frame, ir::Indirect>(op);
  }
  static bool is_memory(const ir::Type_operand& op) {
    return isa<ir::Stack, ir::Frame, ir::Data, ir::Indirect, ir::IndirectY,
               ir::IndirectZp, ir::IndirectZpY, ir::ZP>(op);
  }
  static bool is_data_or_zp(const ir::Type_operand& op) {
    return isa<ir::Data, ir::ZP>(op);
  }
  static bool is_imm_or_label(const ir::Type_operand& op) {
    return isa<ir::Imm, ir::ImmLabelLow, ir::ImmLabelHigh>(op);
  }
  static bool is_x_indexed(const ir::Type_operand& op) {
    const auto* d = as<ir::IndexedData>(op);
    return d != nullptr && isa<ir::X>(d->index);
  }

  // Returns whether a `Data` operand's address is in zero page. The
  // only `Data` names that can be are the zero-page symbols, and since
  // only the name's category is known, the offset is capped at $FB,
  // which every offset `tac_to_asm` writes for the runtime's symbols
  // fits.
  bool data_fits_zp(const ir::Data& d) const {
    const bool zp = zp_names_ != nullptr
                        ? zp_names_->find(d.name) != zp_names_->end()
                        : (d.name == "HARGS" || d.name == "SSP" ||
                           d.name == "FP" || d.name == "DPTR");
    return zp && 0 <= d.offset && d.offset <= 0xFB;
  }
  bool abs_fits_zp(const ir::Type_operand& op) const {
    if (const auto* zp = as<ir::ZP>(op)) {
      return 0 <= zp->address + zp->offset && zp->address + zp->offset <= 0xFF;
    }
    return data_fits_zp(std::get<ir::Data>(op));
  }

  static void reject_pseudo(const ir::Type_operand& op) {
    if (const auto* p = as<ir::Pseudo>(op)) {
      fail("Pseudo('" + p->name + "') reached assembler; "
           "replace_pseudoregisters must run first");
    }
  }

  int mov_size(const ir::Type_operand& src, const ir::Type_operand& dst) const;
  int load_mem_to_a_size(const ir::Type_operand& src) const;
  int store_a_to_mem_size(const ir::Type_operand& dst) const;
  int accum_arith_size(const ir::Type_operand& src) const;
  int compare_size(const ir::Type_operand& left, const ir::Type_operand& right) const;
  int bit_test_size(const ir::Type_operand& src) const;
  int inc_dec_size(const ir::Type_operand& dst) const;
  int shift_size(const ir::Type_operand& dst) const;
  int load_address_size(const ir::Type_operand& src, const ir::Type_operand& dst) const;
  int decimal_arith_size(const ir::DecimalArith& instr) const;

  const passes::NameSet* zp_names_;
};

int Sizer::mov_size(const ir::Type_operand& src, const ir::Type_operand& dst) const {
  reject_pseudo(src);
  reject_pseudo(dst);
  // A move of an operand to itself emits nothing, as `asm_emit` drops
  // it.
  if (src == dst) {
    return 0;
  }
  const auto* src_reg = as<ir::Reg>(src);
  const auto* dst_reg = as<ir::Reg>(dst);
  // TXA, TYA, TAX, and TAY are one byte. X to Y and Y to X go through A
  // as two transfers.
  if (src_reg != nullptr && dst_reg != nullptr) {
    if ((isa<ir::X>(src_reg->reg) && isa<ir::Y>(dst_reg->reg)) ||
        (isa<ir::Y>(src_reg->reg) && isa<ir::X>(dst_reg->reg))) {
      return 2;
    }
    return 1;
  }
  // `LD# #imm` into a register.
  if (is_imm_or_label(src) && dst_reg != nullptr) {
    return 2;
  }
  // `LDA #` and a store.
  if (is_imm_or_label(src)) {
    return 2 + store_a_to_mem_size(dst);
  }
  if (is_memory(src) && is_reg_a(dst)) {
    return load_mem_to_a_size(src);
  }
  const auto* src_indexed = as<ir::IndexedData>(src);
  // `LDA name+off,X` or `,Y`, absolute indexed.
  if (src_indexed != nullptr && is_reg_a(dst)) {
    return 3;
  }
  if (src_indexed != nullptr && is_memory(dst)) {
    return 3 + store_a_to_mem_size(dst);
  }
  // `LDX abs,Y` and `LDY abs,X` are 3 bytes. `LDX abs,X` and `LDY abs,Y`
  // don't exist, and hwreg_eligibility never colors a value into a
  // shape that would need one.
  if (src_indexed != nullptr && dst_reg != nullptr) {
    if (isa<ir::X>(dst_reg->reg) && isa<ir::Y>(src_indexed->index)) {
      return 3;
    }
    if (isa<ir::Y>(dst_reg->reg) && isa<ir::X>(src_indexed->index)) {
      return 3;
    }
  }
  // `STA abs,X` / `abs,Y`.
  if (is_reg_a(src) && isa<ir::IndexedData>(dst)) {
    return 3;
  }
  // `LDX` / `LDY` from zero page or an absolute address.
  if (is_data_or_zp(src) && dst_reg != nullptr && isa<ir::X, ir::Y>(dst_reg->reg)) {
    return abs_fits_zp(src) ? 2 : 3;
  }
  // `STX` / `STY` to zero page or an absolute address.
  if (is_reg_xy(src) && is_data_or_zp(dst)) {
    return abs_fits_zp(dst) ? 2 : 3;
  }
  // `TXA` / `TYA`, `LDY #`, and `STA (zp),Y`.
  if (is_reg_xy(src) && is_indirect_y(dst)) {
    return 1 + 4;
  }
  if (is_reg_a(src) && is_memory(dst)) {
    return store_a_to_mem_size(dst);
  }
  if (is_memory(src) && is_memory(dst)) {
    return load_mem_to_a_size(src) + store_a_to_mem_size(dst);
  }
  fail("unsupported Mov");
}

int Sizer::load_mem_to_a_size(const ir::Type_operand& src) const {
  // dasm picks zero-page addressing when the address is below $100.
  if (is_data_or_zp(src)) {
    return abs_fits_zp(src) ? 2 : 3;
  }
  // `LDY #` and `LDA (zp),Y`.
  if (is_indirect_y(src)) {
    return 4;
  }
  // `LDA (zp),Y`, with Y loaded already.
  if (isa<ir::IndirectY, ir::IndirectZpY>(src)) {
    return 2;
  }
  if (isa<ir::IndirectZp>(src)) {
    return 4;
  }
  fail("can't load into A");
}

int Sizer::store_a_to_mem_size(const ir::Type_operand& dst) const {
  if (is_data_or_zp(dst)) {
    return abs_fits_zp(dst) ? 2 : 3;
  }
  if (is_indirect_y(dst)) {
    return 4;
  }
  if (isa<ir::IndirectY, ir::IndirectZpY>(dst)) {
    return 2;
  }
  if (isa<ir::IndirectZp>(dst)) {
    return 4;
  }
  fail("can't store A");
}

// ADC, SBC, AND, ORA, and EOR, which all take A as their implied
// destination.
int Sizer::accum_arith_size(const ir::Type_operand& src) const {
  if (is_imm_or_label(src)) {
    return 2;
  }
  if (is_data_or_zp(src)) {
    return abs_fits_zp(src) ? 2 : 3;
  }
  if (is_indirect_y(src)) {
    return 4;
  }
  // abs,X or abs,Y.
  if (isa<ir::IndexedData>(src)) {
    return 3;
  }
  if (isa<ir::IndirectZp>(src)) {
    return 4;
  }
  if (isa<ir::IndirectZpY>(src)) {
    return 2;
  }
  fail("unsupported accum-arith src");
}

int Sizer::compare_size(const ir::Type_operand& left,
                        const ir::Type_operand& right) const {
  const auto* left_reg = as<ir::Reg>(left);
  if (left_reg == nullptr) {
    fail("Compare left must be a register");
  }
  const bool left_a = isa<ir::A>(left_reg->reg);
  if (is_imm_or_label(right)) {
    return 2;
  }
  if (is_data_or_zp(right)) {
    return abs_fits_zp(right) ? 2 : 3;
  }
  // CPX and CPY have no absolute indexed or indirect-Y forms.
  if (isa<ir::IndexedData>(right)) {
    if (!left_a) {
      fail("Compare can't use an IndexedData right");
    }
    return 3;
  }
  if (is_indirect_y(right)) {
    if (!left_a) {
      fail("Compare requires Imm/Data/ZP right");
    }
    return 4;
  }
  if (isa<ir::IndirectY>(right)) {
    if (!left_a) {
      fail("Compare can't use an IndirectY right");
    }
    return 2;
  }
  if (isa<ir::IndirectZpY, ir::IndirectZp>(right)) {
    if (!left_a) {
      fail("Compare can't use an indirect right");
    }
    return isa<ir::IndirectZpY>(right) ? 2 : 4;
  }
  fail("unsupported Compare right");
}

int Sizer::bit_test_size(const ir::Type_operand& src) const {
  if (!is_data_or_zp(src)) {
    fail("BitTest src must be Data / ZP");
  }
  return abs_fits_zp(src) ? 2 : 3;
}

// INX, INY, DEX, and DEY are one byte, INC and DEC of a `Data` / `ZP`
// byte two or three, and of an element indexed by X three, which the
// assembler always encodes as absolute, as it does an indexed load.
int Sizer::inc_dec_size(const ir::Type_operand& dst) const {
  if (is_reg_xy(dst)) {
    return 1;
  }
  if (is_data_or_zp(dst)) {
    return abs_fits_zp(dst) ? 2 : 3;
  }
  if (is_x_indexed(dst)) {
    return 3;
  }
  fail("Inc/Dec dst unsupported");
}

// A shift of A is one byte, of a `Data` / `ZP` byte two or three, of an
// element indexed by X three, and of a soft-stack byte the load, the
// shift of A, and the store.
int Sizer::shift_size(const ir::Type_operand& dst) const {
  if (is_reg_a(dst)) {
    return 1;
  }
  if (is_data_or_zp(dst)) {
    return abs_fits_zp(dst) ? 2 : 3;
  }
  if (is_x_indexed(dst)) {
    return 3;
  }
  if (isa<ir::Stack, ir::Frame>(dst)) {
    return load_mem_to_a_size(dst) + 1 + store_a_to_mem_size(dst);
  }
  fail("Shift dst unsupported");
}

// A `Data` source is `LDA #lo`, a store, `LDA #hi`, and a store. A
// `Frame` source is a 16-bit add: `CLC; LDA FP; ADC #off`, a store,
// `LDA FP+1; ADC #0`, and a store.
int Sizer::load_address_size(const ir::Type_operand& src,
                             const ir::Type_operand& dst) const {
  const int store = store_a_to_mem_size(dst);
  if (isa<ir::Data>(src)) {
    return 2 + store + 2 + store;
  }
  if (isa<ir::Frame>(src)) {
    return 1 + 2 + 2 + store + 2 + 2 + store;
  }
  fail("LoadAddress src must be Data or Frame");
}

// `SED`, `CLC` or `SEC`, an LDA / ADC (SBC) / STA per byte, and `CLD`,
// as `passes.asm_to_asm2.expand_decimal_arith` writes it.
int Sizer::decimal_arith_size(const ir::DecimalArith& instr) const {
  const ir::Type_operand a = ir::Reg{.reg = ir::A{}};
  int n = 3;
  const std::size_t k =
      std::min({instr.left.size(), instr.right.size(), instr.dst.size()});
  for (std::size_t i = 0; i < k; ++i) {
    n += mov_size(instr.left[i], a) + accum_arith_size(instr.right[i]) +
         mov_size(a, instr.dst[i]);
  }
  return n;
}

// `SSP -= amt`: SEC, LDA SSP, SBC #lo, STA SSP, LDA SSP+1, SBC #hi, STA
// SSP+1, with SSP in zero page. Nothing when `amt` is 0.
int ssp_sub_size(Int amt) { return amt == 0 ? 0 : 13; }

int prologue_size(const ir::FunctionPrologue& p) {
  if (p.arg_bytes + p.local_bytes == 0) {
    return 0;
  }
  // The locals and the 2-byte slot for the caller's FP.
  int size = ssp_sub_size(p.local_bytes + 2);
  // The caller's FP goes in the slot at SSP+M+1 / SSP+M+2 through two
  // `Mov(Reg(A), Stack(...))` atoms that each load Y:
  // `LDA FP; LDY #M+1; STA (SSP),Y; LDA FP+1; LDY #M+2; STA (SSP),Y`.
  size += 12;
  // FP = SSP: `LDA SSP; STA FP; LDA SSP+1; STA FP+1`.
  size += 8;
  // Each callee-saved byte: `LDA $addr; LDY #off; STA (FP),Y`.
  size += static_cast<int>(p.callee_saved_addrs.size()) * 6;
  return size;
}

int ret_size(const ir::Ret& r) {
  if (r.arg_bytes + r.local_bytes == 0) {
    return 1;  // RTS
  }
  // SSP = FP + (N+M+2): `CLC; LDA FP; ADC #lo; STA SSP; LDA FP+1; ADC
  // #hi; STA SSP+1`, or the four moves when the amount is 0.
  const Int rewind = r.arg_bytes + r.local_bytes + 2;
  int size = rewind == 0 ? 8 : 13;
  // The caller's FP comes back from its slot through X, which no return
  // convention puts data in: `LDY #M+1; LDA (FP),Y; TAX; LDY #M+2; LDA
  // (FP),Y; STA FP+1; TXA; STA FP`.
  size += 14;
  // Each callee-saved byte: `LDY #off; LDA (FP),Y; STA $addr`.
  size += static_cast<int>(r.callee_saved_addrs.size()) * 6;
  // The return bytes kept across the teardown: A and X with `PHA; TXA;
  // PHA` ... `PLA; TAX; PLA`, or A alone with `PHA` ... `PLA`.
  if (r.save_x) {
    size += 6;
  } else if (r.save_a) {
    size += 2;
  }
  if (r.save_c) {
    size += 2;  // PHP ... PLP around the teardown
  }
  return size + 1;  // RTS
}

int Sizer::instr_size(const ir::Type_instruction& instr) const {
  const ir::Type_operand a = ir::Reg{.reg = ir::A{}};
  switch (ir::kind(instr)) {
    case ir::Kind_instruction::Mov: {
      const auto& m = std::get<ir::Mov>(instr);
      // A volatile move of a memory byte to itself is a load and a
      // store, which `asm_emit` keeps.
      if (m.is_volatile && m.src == m.dst && !isa<ir::Reg>(m.src)) {
        return mov_size(m.src, a) + mov_size(a, m.dst);
      }
      return mov_size(m.src, m.dst);
    }
    case ir::Kind_instruction::Add:
      return accum_arith_size(std::get<ir::Add>(instr).src);
    case ir::Kind_instruction::Sub:
      return accum_arith_size(std::get<ir::Sub>(instr).src);
    case ir::Kind_instruction::And:
      return accum_arith_size(std::get<ir::And>(instr).src);
    case ir::Kind_instruction::Or:
      return accum_arith_size(std::get<ir::Or>(instr).src);
    case ir::Kind_instruction::Xor: {
      const auto& x = std::get<ir::Xor>(instr);
      return accum_arith_size(is_reg_a(x.src1) ? x.src2 : x.src1);
    }
    case ir::Kind_instruction::Compare: {
      const auto& c = std::get<ir::Compare>(instr);
      return compare_size(c.left, c.right);
    }
    case ir::Kind_instruction::BitTest:
      return bit_test_size(std::get<ir::BitTest>(instr).src);
    case ir::Kind_instruction::ClearCarry:
    case ir::Kind_instruction::SetCarry:
    case ir::Kind_instruction::SetDecimal:
    case ir::Kind_instruction::ClearDecimal:
    case ir::Kind_instruction::Push:
    case ir::Kind_instruction::Pop:
      return 1;
    case ir::Kind_instruction::Inc:
      return inc_dec_size(std::get<ir::Inc>(instr).dst);
    case ir::Kind_instruction::Dec:
      return inc_dec_size(std::get<ir::Dec>(instr).dst);
    case ir::Kind_instruction::ArithmeticShiftLeft:
      return shift_size(std::get<ir::ArithmeticShiftLeft>(instr).dst);
    case ir::Kind_instruction::LogicalShiftRight:
      return shift_size(std::get<ir::LogicalShiftRight>(instr).dst);
    case ir::Kind_instruction::RotateLeft:
      return shift_size(std::get<ir::RotateLeft>(instr).dst);
    case ir::Kind_instruction::RotateRight:
      return shift_size(std::get<ir::RotateRight>(instr).dst);
    case ir::Kind_instruction::Call:
    case ir::Kind_instruction::Jump:
      return 3;  // JSR abs, JMP abs
    case ir::Kind_instruction::JumpTable:
      // `ASL; TAY; LDA t+1,Y; PHA; LDA t,Y; PHA; RTS` and a word per
      // target.
      return 11 + 2 * static_cast<int>(std::get<ir::JumpTable>(instr).targets.size());
    case ir::Kind_instruction::Branch:
      return 2;  // Bxx rel
    case ir::Kind_instruction::Label:
      return 0;
    case ir::Kind_instruction::AllocateStack:
      return ssp_sub_size(std::get<ir::AllocateStack>(instr).bytes);
    case ir::Kind_instruction::FunctionPrologue:
      return prologue_size(std::get<ir::FunctionPrologue>(instr));
    case ir::Kind_instruction::Ret:
      return ret_size(std::get<ir::Ret>(instr));
    case ir::Kind_instruction::Return:
      return 1;  // RTS
    case ir::Kind_instruction::LoadAddress: {
      const auto& la = std::get<ir::LoadAddress>(instr);
      return load_address_size(la.src, la.dst);
    }
    case ir::Kind_instruction::DecimalArith:
      return decimal_arith_size(std::get<ir::DecimalArith>(instr));
    case ir::Kind_instruction::Phi:
      throw InternalError("TypeError: sim.assembler: Phi node leaked past SSA destruction");
  }
  throw InternalError("TypeError: unexpected instruction");
}

}  // namespace

int instruction_size(const ir::Type_instruction& instr,
                     const passes::NameSet* zp_names) {
  return Sizer(zp_names).instr_size(instr);
}

}  // namespace c6502::sim
