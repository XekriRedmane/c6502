#include "sim/assembler.h"

#include <algorithm>
#include <cctype>
#include <optional>
#include <string>
#include <string_view>
#include <utility>

#include "binutils/opcodes.h"
#include "errors.h"
#include "pyrepr.h"

namespace c6502::sim {

namespace {

namespace ir = ::c6502::asm_ast;
using ::c6502::asdl::as;
using ::c6502::asdl::Int;
using ::c6502::asdl::isa;
using ::c6502::asdl::py_repr;
using Mode = ::c6502::binutils::opcodes::Mode;
using Bytes = std::vector<std::uint8_t>;
using Symbols = std::map<std::string, std::int64_t>;

[[noreturn]] void fail(const std::string& message) { throw AssemblerError(message); }

std::string repr(const ir::Type_operand& op) { return py_repr(op); }

std::string str(Int v) { return pyrepr::integer(v); }

// Python's `f"{v:#x}"`.
std::string hex_alt(Int v) {
  std::string digits = pyrepr::hex(v < 0 ? -v : v, 1);
  std::transform(digits.begin(), digits.end(), digits.begin(),
                 [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
  return (v < 0 ? "-0x" : "0x") + digits;
}

std::int64_t to_i64(Int v) { return static_cast<std::int64_t>(static_cast<long long>(v)); }

// A byte that Python's `bytes([...])` takes without a check of its own,
// which raises ValueError outside 0..255.
std::uint8_t byte(Int v) {
  if (v < 0 || v > 0xFF) {
    throw InternalError("ValueError: bytes must be in range(0, 256)");
  }
  return static_cast<std::uint8_t>(static_cast<int>(v));
}

// sim/assembler.py looks each opcode up in a table of the forms it
// emits, and a missing one is its KeyError.
std::uint8_t op_code(std::string_view mnemonic, Mode mode) {
  const std::optional<std::uint8_t> op = binutils::opcodes::opcode(mnemonic, mode);
  if (!op) {
    throw InternalError("KeyError: " + pyrepr::string(mnemonic));
  }
  return *op;
}

std::uint8_t implied(std::string_view mnemonic) { return op_code(mnemonic, Mode::kImp); }

bool is_reg_a(const ir::Type_operand& op) {
  const auto* r = as<ir::Reg>(op);
  return r != nullptr && isa<ir::A>(r->reg);
}
bool is_reg_x(const ir::Type_operand& op) {
  const auto* r = as<ir::Reg>(op);
  return r != nullptr && isa<ir::X>(r->reg);
}
bool is_reg_y(const ir::Type_operand& op) {
  const auto* r = as<ir::Reg>(op);
  return r != nullptr && isa<ir::Y>(r->reg);
}
bool is_reg_xy(const ir::Type_operand& op) { return is_reg_x(op) || is_reg_y(op); }
bool needs_ldy_setup(const ir::Type_operand& op) {
  return isa<ir::Stack, ir::Frame, ir::Indirect>(op);
}
bool is_memory(const ir::Type_operand& op) {
  return isa<ir::Stack, ir::Frame, ir::Data, ir::Indirect, ir::IndirectY, ir::IndirectZp,
             ir::IndirectZpY, ir::ZP>(op);
}
bool is_data_or_zp(const ir::Type_operand& op) { return isa<ir::Data, ir::ZP>(op); }
bool is_imm_label(const ir::Type_operand& op) {
  return isa<ir::ImmLabelLow, ir::ImmLabelHigh>(op);
}
bool is_imm_or_label(const ir::Type_operand& op) {
  return isa<ir::Imm>(op) || is_imm_label(op);
}
bool is_x_indexed(const ir::Type_operand& op) {
  const auto* d = as<ir::IndexedData>(op);
  return d != nullptr && isa<ir::X>(d->index);
}

void reject_pseudo(const ir::Type_operand& op) {
  if (const auto* p = as<ir::Pseudo>(op)) {
    fail("Pseudo(" + pyrepr::string(p->name) +
         ") reached assembler; replace_pseudoregisters must run first");
  }
}

bool is_volatile_self_mov(const ir::Mov& m) {
  return m.is_volatile && m.src == m.dst && !isa<ir::Reg>(m.src);
}

// The sizes, with the set of names that resolve to zero page.
class Sizer {
 public:
  explicit Sizer(const passes::NameSet* zp_names) : zp_names_(zp_names) {}

  int instr_size(const ir::Type_instruction& instr) const;

 private:
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
  if (is_reg_xy(src) && needs_ldy_setup(dst)) {
    return 1 + 4;
  }
  if (is_reg_a(src) && is_memory(dst)) {
    return store_a_to_mem_size(dst);
  }
  if (is_memory(src) && is_memory(dst)) {
    return load_mem_to_a_size(src) + store_a_to_mem_size(dst);
  }
  fail("unsupported Mov: " + repr(src) + " -> " + repr(dst));
}

int Sizer::load_mem_to_a_size(const ir::Type_operand& src) const {
  // dasm picks zero-page addressing when the address is below $100.
  if (is_data_or_zp(src)) {
    return abs_fits_zp(src) ? 2 : 3;
  }
  // `LDY #` and `LDA (zp),Y`.
  if (needs_ldy_setup(src)) {
    return 4;
  }
  // `LDA (zp),Y`, with Y loaded already.
  if (isa<ir::IndirectY, ir::IndirectZpY>(src)) {
    return 2;
  }
  if (isa<ir::IndirectZp>(src)) {
    return 4;
  }
  fail("can't load " + repr(src) + " into A");
}

int Sizer::store_a_to_mem_size(const ir::Type_operand& dst) const {
  if (is_data_or_zp(dst)) {
    return abs_fits_zp(dst) ? 2 : 3;
  }
  if (needs_ldy_setup(dst)) {
    return 4;
  }
  if (isa<ir::IndirectY, ir::IndirectZpY>(dst)) {
    return 2;
  }
  if (isa<ir::IndirectZp>(dst)) {
    return 4;
  }
  fail("can't store A into " + repr(dst));
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
  if (needs_ldy_setup(src)) {
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
  fail("unsupported accum-arith src: " + repr(src));
}

int Sizer::compare_size(const ir::Type_operand& left,
                        const ir::Type_operand& right) const {
  const auto* left_reg = as<ir::Reg>(left);
  if (left_reg == nullptr) {
    fail("Compare left must be a register; got " + repr(left));
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
      fail("Compare with left=" + repr(left) +
           " can't use an IndexedData right (CPX/CPY have no absolute,X / absolute,Y "
           "addressing modes)");
    }
    return 3;
  }
  if (needs_ldy_setup(right)) {
    if (!left_a) {
      fail("Compare with left=" + repr(left) +
           " requires Imm/Data/ZP right (no indirect-Y for CPX/CPY)");
    }
    return 4;
  }
  if (isa<ir::IndirectY>(right)) {
    if (!left_a) {
      fail("Compare with left=" + repr(left) +
           " can't use an IndirectY right (no indirect-Y for CPX/CPY)");
    }
    return 2;
  }
  if (isa<ir::IndirectZpY, ir::IndirectZp>(right)) {
    if (!left_a) {
      fail("Compare with left=" + repr(left) +
           " can't use an indirect right (no indirect-Y for CPX/CPY)");
    }
    return isa<ir::IndirectZpY>(right) ? 2 : 4;
  }
  fail("unsupported Compare right: " + repr(right));
}

int Sizer::bit_test_size(const ir::Type_operand& src) const {
  if (!is_data_or_zp(src)) {
    fail("BitTest src must be Data / ZP (NMOS 6502 has no `BIT #imm`); got " + repr(src));
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
  fail("Inc/Dec dst unsupported: " + repr(dst));
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
  fail("Shift dst unsupported: " + repr(dst));
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
  fail("LoadAddress src must be Data or Frame; got " + repr(src));
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

int jump_table_dispatch_size();

int Sizer::instr_size(const ir::Type_instruction& instr) const {
  const ir::Type_operand a = ir::Reg{.reg = ir::A{}};
  switch (ir::kind(instr)) {
    case ir::Kind_instruction::Mov: {
      const auto& m = std::get<ir::Mov>(instr);
      if (is_volatile_self_mov(m)) {
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
      return jump_table_dispatch_size() + 2 * static_cast<int>(std::get<ir::JumpTable>(instr).targets.size());
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

std::int64_t init_size(const ir::Type_static_init& item) {
  switch (ir::kind(item)) {
    case ir::Kind_static_init::CharInit:
    case ir::Kind_static_init::AddressByteInit:
      return 1;
    case ir::Kind_static_init::IntInit:
    case ir::Kind_static_init::AddressInit:
      return 2;
    case ir::Kind_static_init::LongInit:
    case ir::Kind_static_init::FloatInit:
      return 4;
    case ir::Kind_static_init::LongLongInit:
    case ir::Kind_static_init::DoubleInit:
      return 8;
    case ir::Kind_static_init::StringInit:
    case ir::Kind_static_init::ZeroInit:
      break;
  }
  const Int n = isa<ir::StringInit>(item) ? std::get<ir::StringInit>(item).bytes
                                         : std::get<ir::ZeroInit>(item).bytes;
  return to_i64(n);
}

struct Runtime {
  std::int64_t ssp;
  std::int64_t fp;
  std::int64_t dptr;
};

class Emitter {
 public:
  Emitter(const Runtime& runtime, const Symbols& symbols)
      : rt_(runtime), syms_(symbols) {}

  void instr(const ir::Type_instruction& instr, std::int64_t addr, Bytes& out) const;
  void init(const ir::Type_static_init& item, Bytes& out) const;
  static void jump_table_dispatch(const std::string& index, Int index_0_entry, Bytes& out);

 private:
  std::int64_t lookup(const std::string& name, const char* what = "undefined symbol ") const {
    const auto it = syms_.find(name);
    if (it == syms_.end()) {
      fail(what + pyrepr::string(name));
    }
    return it->second;
  }
  std::int64_t address(const std::string& name, Int offset) const {
    return to_i64((Int(lookup(name)) + offset) & Int(0xFFFF));
  }

  static std::uint8_t imm_byte(Int v, const char* label = "immediate") {
    if (v < 0 || v > 0xFF) {
      fail(std::string(label) + " " + str(v) + " out of range 0..255");
    }
    return static_cast<std::uint8_t>(static_cast<int>(v));
  }
  static void abs_word(Int v, Bytes& out) {
    if (v < 0 || v > 0xFFFF) {
      fail("address " + str(v) + " out of range 0..65535");
    }
    const int w = static_cast<int>(v);
    out.push_back(static_cast<std::uint8_t>(w & 0xFF));
    out.push_back(static_cast<std::uint8_t>(w >> 8));
  }

  std::int64_t resolve_indexed(const ir::IndexedData& op) const {
    if (op.name.empty()) {
      return to_i64(op.offset & Int(0xFFFF));
    }
    return address(op.name, op.offset);
  }
  std::int64_t resolve_abs(const ir::Type_operand& op) const;
  std::int64_t resolve_imm_label(const ir::Type_operand& op) const;
  std::int64_t indirect_zp(const ir::Type_operand& op) const;

  static void ldy_imm(Int off, Bytes& out) {
    out.push_back(op_code("LDY", Mode::kImm));
    out.push_back(imm_byte(off, "frame offset"));
  }
  static void lda_imm(Int v, Bytes& out) {
    out.push_back(op_code("LDA", Mode::kImm));
    out.push_back(imm_byte(v));
  }
  void indy(std::string_view mn, const ir::Type_operand& op, Bytes& out) const;
  void indy_no_y_setup(std::string_view mn, Bytes& out) const {
    out.push_back(op_code(mn, Mode::kIndy));
    out.push_back(byte(rt_.dptr));
  }
  static void indy_zp(std::string_view mn, Int zp, Int off, Bytes& out) {
    ldy_imm(off, out);
    out.push_back(op_code(mn, Mode::kIndy));
    out.push_back(imm_byte(zp, "zp address"));
  }
  static void indy_zp_no_y_setup(std::string_view mn, Int zp, Bytes& out) {
    out.push_back(op_code(mn, Mode::kIndy));
    out.push_back(imm_byte(zp, "zp address"));
  }
  static void absolute(std::string_view mn, Int addr, Bytes& out) {
    const std::uint8_t op = op_code(mn, Mode::kAbs);
    Bytes word;
    abs_word(addr, word);
    out.push_back(op);
    out.insert(out.end(), word.begin(), word.end());
  }
  static void abs_indexed(std::string_view mn, Int addr, const ir::Type_reg& index, Bytes& out) {
    Bytes word;
    abs_word(addr, word);
    out.push_back(op_code(mn, isa<ir::X>(index) ? Mode::kAbsx : Mode::kAbsy));
    out.insert(out.end(), word.begin(), word.end());
  }
  static void zp(std::string_view mn, Int zp, Bytes& out) {
    out.push_back(op_code(mn, Mode::kZp));
    out.push_back(imm_byte(zp, "zp address"));
  }
  // dasm picks zero-page addressing for an address below $100.
  static void zp_or_abs(std::string_view mn, Int addr, Bytes& out) {
    if (0 <= addr && addr <= 0xFF && binutils::opcodes::opcode(mn, Mode::kZp)) {
      zp(mn, addr, out);
    } else {
      absolute(mn, addr, out);
    }
  }

  void mov(const ir::Type_operand& src, const ir::Type_operand& dst, Bytes& out) const;
  void load_mem_to_a(const ir::Type_operand& op, Bytes& out) const;
  void store_a_to_mem(const ir::Type_operand& op, Bytes& out) const;
  void accum_arith(std::string_view mn, const ir::Type_operand& src, Bytes& out) const;
  void compare(const ir::Type_operand& left, const ir::Type_operand& right, Bytes& out) const;
  void bit_test(const ir::Type_operand& src, Bytes& out) const;
  void inc_dec(const ir::Type_operand& op, bool inc, Bytes& out) const;
  void shift(std::string_view mn, const ir::Type_operand& dst, Bytes& out) const;
  void jump_table(std::int64_t addr, const ir::JumpTable& jt, Bytes& out) const;
  void branch(const ir::Type_condition& cond, const std::string& target, std::int64_t addr,
              Bytes& out) const;
  void ssp_sub(Int amt, Bytes& out) const;
  void prologue(const ir::FunctionPrologue& p, Bytes& out) const;
  void ret(const ir::Ret& r, Bytes& out) const;
  void decimal_arith(const ir::DecimalArith& d, Bytes& out) const;
  void load_address(const ir::Type_operand& src, const ir::Type_operand& dst, Bytes& out) const;

  Runtime rt_;
  const Symbols& syms_;
};

std::int64_t Emitter::resolve_abs(const ir::Type_operand& op) const {
  if (const auto* d = as<ir::Data>(op)) {
    return address(d->name, d->offset);
  }
  if (const auto* z = as<ir::ZP>(op)) {
    const Int addr = z->address + z->offset;
    if (addr < 0 || addr > 0xFFFF) {
      fail("ZP address " + hex_alt(addr) + " out of range");
    }
    return to_i64(addr);
  }
  throw InternalError("TypeError: _resolve_abs_addr only handles Data / ZP, got " + repr(op));
}

std::int64_t Emitter::resolve_imm_label(const ir::Type_operand& op) const {
  if (const auto* lo = as<ir::ImmLabelLow>(op)) {
    return address(lo->name, lo->offset) & 0xFF;
  }
  const auto& hi = std::get<ir::ImmLabelHigh>(op);
  return address(hi.name, hi.offset) >> 8;
}

std::int64_t Emitter::indirect_zp(const ir::Type_operand& op) const {
  if (isa<ir::Stack>(op)) return rt_.ssp;
  if (isa<ir::Frame>(op)) return rt_.fp;
  return rt_.dptr;
}

void Emitter::indy(std::string_view mn, const ir::Type_operand& op, Bytes& out) const {
  Int off = 0;
  if (const auto* s = as<ir::Stack>(op)) off = s->offset;
  if (const auto* f = as<ir::Frame>(op)) off = f->offset;
  if (const auto* i = as<ir::Indirect>(op)) off = i->offset;
  ldy_imm(off, out);
  out.push_back(op_code(mn, Mode::kIndy));
  out.push_back(byte(indirect_zp(op)));
}

// sim/assembler.py's `_emit_mov`.
void Emitter::mov(const ir::Type_operand& src, const ir::Type_operand& dst, Bytes& out) const {
  reject_pseudo(src);
  reject_pseudo(dst);
  if (src == dst) {
    return;
  }
  const auto* src_reg = as<ir::Reg>(src);
  const auto* dst_reg = as<ir::Reg>(dst);
  if (src_reg != nullptr && dst_reg != nullptr) {
    const ir::Type_reg& s = src_reg->reg;
    const ir::Type_reg& d = dst_reg->reg;
    if (isa<ir::X>(s) && isa<ir::A>(d)) {
      out.push_back(implied("TXA"));
    } else if (isa<ir::Y>(s) && isa<ir::A>(d)) {
      out.push_back(implied("TYA"));
    } else if (isa<ir::A>(s) && isa<ir::X>(d)) {
      out.push_back(implied("TAX"));
    } else if (isa<ir::A>(s) && isa<ir::Y>(d)) {
      out.push_back(implied("TAY"));
    } else if (isa<ir::X>(s) && isa<ir::Y>(d)) {
      out.push_back(implied("TXA"));
      out.push_back(implied("TAY"));
    } else if (isa<ir::Y>(s) && isa<ir::X>(d)) {
      out.push_back(implied("TYA"));
      out.push_back(implied("TAX"));
    } else {
      fail("unsupported Mov reg->reg: " + repr(src) + " -> " + repr(dst));
    }
    return;
  }
  const char* load_mn = nullptr;
  if (dst_reg != nullptr) {
    load_mn = isa<ir::A>(dst_reg->reg) ? "LDA" : isa<ir::X>(dst_reg->reg) ? "LDX" : "LDY";
  }
  if (const auto* imm = as<ir::Imm>(src); imm != nullptr && dst_reg != nullptr) {
    const std::uint8_t v = imm_byte(imm->value);
    out.push_back(op_code(load_mn, Mode::kImm));
    out.push_back(v);
    return;
  }
  if (is_imm_label(src) && dst_reg != nullptr) {
    out.push_back(op_code(load_mn, Mode::kImm));
    out.push_back(static_cast<std::uint8_t>(resolve_imm_label(src)));
    return;
  }
  if (const auto* imm = as<ir::Imm>(src); imm != nullptr && is_memory(dst)) {
    lda_imm(imm->value, out);
    store_a_to_mem(dst, out);
    return;
  }
  if (is_imm_label(src) && is_memory(dst)) {
    out.push_back(op_code("LDA", Mode::kImm));
    out.push_back(static_cast<std::uint8_t>(resolve_imm_label(src)));
    store_a_to_mem(dst, out);
    return;
  }
  if (is_memory(src) && is_reg_a(dst)) {
    load_mem_to_a(src, out);
    return;
  }
  const auto* src_indexed = as<ir::IndexedData>(src);
  if (src_indexed != nullptr && is_reg_a(dst)) {
    abs_indexed("LDA", resolve_indexed(*src_indexed), src_indexed->index, out);
    return;
  }
  if (src_indexed != nullptr && is_memory(dst)) {
    abs_indexed("LDA", resolve_indexed(*src_indexed), src_indexed->index, out);
    store_a_to_mem(dst, out);
    return;
  }
  if (const auto* d = as<ir::IndexedData>(dst); d != nullptr && is_reg_a(src)) {
    abs_indexed("STA", resolve_indexed(*d), d->index, out);
    return;
  }
  // The 6502 has no `LDX abs,X` or `LDY abs,Y`.
  if (src_indexed != nullptr && dst_reg != nullptr) {
    if (isa<ir::X>(dst_reg->reg) && isa<ir::Y>(src_indexed->index)) {
      abs_indexed("LDX", resolve_indexed(*src_indexed), src_indexed->index, out);
      return;
    }
    if (isa<ir::Y>(dst_reg->reg) && isa<ir::X>(src_indexed->index)) {
      abs_indexed("LDY", resolve_indexed(*src_indexed), src_indexed->index, out);
      return;
    }
  }
  if (is_data_or_zp(src) && dst_reg != nullptr && isa<ir::X, ir::Y>(dst_reg->reg)) {
    zp_or_abs(load_mn, resolve_abs(src), out);
    return;
  }
  if (is_reg_xy(src) && is_data_or_zp(dst)) {
    zp_or_abs(is_reg_x(src) ? "STX" : "STY", resolve_abs(dst), out);
    return;
  }
  if (is_reg_xy(src) && needs_ldy_setup(dst)) {
    out.push_back(implied(is_reg_x(src) ? "TXA" : "TYA"));
    indy("STA", dst, out);
    return;
  }
  if (is_reg_a(src) && is_memory(dst)) {
    store_a_to_mem(dst, out);
    return;
  }
  if (is_memory(src) && is_memory(dst)) {
    load_mem_to_a(src, out);
    store_a_to_mem(dst, out);
    return;
  }
  fail("unsupported Mov: " + repr(src) + " -> " + repr(dst));
}

void Emitter::load_mem_to_a(const ir::Type_operand& op, Bytes& out) const {
  if (is_data_or_zp(op)) {
    zp_or_abs("LDA", resolve_abs(op), out);
  } else if (needs_ldy_setup(op)) {
    indy("LDA", op, out);
  } else if (isa<ir::IndirectY>(op)) {
    indy_no_y_setup("LDA", out);
  } else if (const auto* zy = as<ir::IndirectZpY>(op)) {
    indy_zp_no_y_setup("LDA", zy->address, out);
  } else if (const auto* z = as<ir::IndirectZp>(op)) {
    indy_zp("LDA", z->address, z->offset, out);
  } else {
    fail("can't load " + repr(op) + " into A");
  }
}

void Emitter::store_a_to_mem(const ir::Type_operand& op, Bytes& out) const {
  if (is_data_or_zp(op)) {
    zp_or_abs("STA", resolve_abs(op), out);
  } else if (needs_ldy_setup(op)) {
    indy("STA", op, out);
  } else if (isa<ir::IndirectY>(op)) {
    indy_no_y_setup("STA", out);
  } else if (const auto* zy = as<ir::IndirectZpY>(op)) {
    indy_zp_no_y_setup("STA", zy->address, out);
  } else if (const auto* z = as<ir::IndirectZp>(op)) {
    indy_zp("STA", z->address, z->offset, out);
  } else {
    fail("can't store A into " + repr(op));
  }
}

void Emitter::accum_arith(std::string_view mn, const ir::Type_operand& src, Bytes& out) const {
  reject_pseudo(src);
  if (const auto* imm = as<ir::Imm>(src)) {
    out.push_back(op_code(mn, Mode::kImm));
    out.push_back(imm_byte(imm->value));
  } else if (is_imm_label(src)) {
    out.push_back(op_code(mn, Mode::kImm));
    out.push_back(static_cast<std::uint8_t>(resolve_imm_label(src)));
  } else if (is_data_or_zp(src)) {
    zp_or_abs(mn, resolve_abs(src), out);
  } else if (needs_ldy_setup(src)) {
    indy(mn, src, out);
  } else if (const auto* d = as<ir::IndexedData>(src)) {
    abs_indexed(mn, resolve_indexed(*d), d->index, out);
  } else if (const auto* z = as<ir::IndirectZp>(src)) {
    indy_zp(mn, z->address, z->offset, out);
  } else if (const auto* zy = as<ir::IndirectZpY>(src)) {
    indy_zp_no_y_setup(mn, zy->address, out);
  } else {
    fail("unsupported " + std::string(mn) + " src: " + repr(src));
  }
}

// sim/assembler.py's `_emit_compare`.
void Emitter::compare(const ir::Type_operand& left, const ir::Type_operand& right,
                      Bytes& out) const {
  reject_pseudo(left);
  reject_pseudo(right);
  const auto* left_reg = as<ir::Reg>(left);
  if (left_reg == nullptr) {
    fail("Compare left must be a register; got " + repr(left));
  }
  const char* mn = isa<ir::A>(left_reg->reg) ? "CMP" : isa<ir::X>(left_reg->reg) ? "CPX" : "CPY";
  if (const auto* imm = as<ir::Imm>(right)) {
    out.push_back(op_code(mn, Mode::kImm));
    out.push_back(imm_byte(imm->value));
  } else if (is_imm_label(right)) {
    out.push_back(op_code(mn, Mode::kImm));
    out.push_back(static_cast<std::uint8_t>(resolve_imm_label(right)));
  } else if (is_data_or_zp(right)) {
    zp_or_abs(mn, resolve_abs(right), out);
  } else if (const auto* d = as<ir::IndexedData>(right)) {
    abs_indexed(mn, resolve_indexed(*d), d->index, out);
  } else if (needs_ldy_setup(right)) {
    indy(mn, right, out);
  } else if (isa<ir::IndirectY>(right)) {
    indy_no_y_setup(mn, out);
  } else if (isa<ir::IndirectZpY, ir::IndirectZp>(right)) {
    if (const auto* zy = as<ir::IndirectZpY>(right)) {
      indy_zp_no_y_setup(mn, zy->address, out);
    } else {
      const auto& z = std::get<ir::IndirectZp>(right);
      indy_zp(mn, z.address, z.offset, out);
    }
  } else {
    fail("unsupported Compare right: " + repr(right));
  }
}

void Emitter::bit_test(const ir::Type_operand& src, Bytes& out) const {
  reject_pseudo(src);
  if (!is_data_or_zp(src)) {
    fail("BitTest src must be Data / ZP; got " + repr(src));
  }
  zp_or_abs("BIT", resolve_abs(src), out);
}

void Emitter::inc_dec(const ir::Type_operand& op, bool inc, Bytes& out) const {
  if (is_reg_x(op)) {
    out.push_back(implied(inc ? "INX" : "DEX"));
    return;
  }
  if (is_reg_y(op)) {
    out.push_back(implied(inc ? "INY" : "DEY"));
    return;
  }
  const char* mn = inc ? "INC" : "DEC";
  if (is_data_or_zp(op)) {
    zp_or_abs(mn, resolve_abs(op), out);
  } else if (is_x_indexed(op)) {
    const auto& d = std::get<ir::IndexedData>(op);
    abs_indexed(mn, resolve_indexed(d), d.index, out);
  } else {
    fail(std::string(inc ? "Inc" : "Dec") +
         " dst must be Reg(X), Reg(Y), Data, ZP, or IndexedData indexed by X; got " + repr(op));
  }
}

// A shift of a soft-stack byte goes through A, since the 6502 has no
// `(zp),Y` form of the shifts.
void Emitter::shift(std::string_view mn, const ir::Type_operand& dst, Bytes& out) const {
  if (is_reg_a(dst)) {
    out.push_back(op_code(mn, Mode::kAcc));
  } else if (is_data_or_zp(dst)) {
    zp_or_abs(mn, resolve_abs(dst), out);
  } else if (is_x_indexed(dst)) {
    const auto& d = std::get<ir::IndexedData>(dst);
    abs_indexed(mn, resolve_indexed(d), d.index, out);
  } else if (isa<ir::Stack, ir::Frame>(dst)) {
    indy("LDA", dst, out);
    out.push_back(op_code(mn, Mode::kAcc));
    indy("STA", dst, out);
  } else {
    fail(std::string(mn) +
         " dst must be Reg(A), Data, ZP, IndexedData indexed by X, Stack, or Frame; got " +
         repr(dst));
  }
}

void Emitter::jump_table_dispatch(const std::string& index, Int index_0_entry, Bytes& out) {
  const ir::Type_reg reg = index == "X" ? ir::Type_reg{ir::X{}} : ir::Type_reg{ir::Y{}};
  out.push_back(op_code("ASL", Mode::kAcc));
  out.push_back(implied("TA" + index));
  abs_indexed("LDA", index_0_entry + 1, reg, out);
  out.push_back(implied("PHA"));
  abs_indexed("LDA", index_0_entry, reg, out);
  out.push_back(implied("PHA"));
  out.push_back(implied("RTS"));
}

int jump_table_dispatch_size() {
  static const int size = [] {
    Bytes dispatch;
    Emitter::jump_table_dispatch("X", 0, dispatch);
    return static_cast<int>(dispatch.size());
  }();
  return size;
}

void Emitter::jump_table(std::int64_t addr, const ir::JumpTable& jt, Bytes& out) const {
  const std::int64_t start = addr + jump_table_dispatch_size();
  if (const auto it = syms_.find(jt.table); it != syms_.end() && it->second != start) {
    fail("jump table " + pyrepr::string(jt.table) + " isn't where expected");
  }
  jump_table_dispatch(jt.index, Int(start) - jt.base * 2, out);
  for (const std::string& t : jt.targets) {
    abs_word((Int(lookup(t)) - 1) & Int(0xFFFF), out);
  }
}

void Emitter::branch(const ir::Type_condition& cond, const std::string& target,
                     std::int64_t addr, Bytes& out) const {
  const std::int64_t to = lookup(target, "undefined branch target ");
  static constexpr const char* kMnemonics[] = {"BCC", "BCS", "BEQ", "BMI",
                                               "BNE", "BPL", "BVC", "BVS"};
  const std::uint8_t op = op_code(kMnemonics[cond.index()], Mode::kRel);
  // The 6502 adds the displacement to the address after the branch.
  const std::int64_t disp = to - (addr + 2);
  if (disp < -128 || disp > 127) {
    fail("branch to " + pyrepr::string(target) + " out of range: disp=" + std::to_string(disp));
  }
  out.push_back(op);
  out.push_back(static_cast<std::uint8_t>(disp & 0xFF));
}

void Emitter::ssp_sub(Int amt, Bytes& out) const {
  if (amt < 0 || amt > 0xFFFF) {
    fail("AllocateStack amt " + str(amt) + " out of range");
  }
  if (amt == 0) {
    return;
  }
  const int n = static_cast<int>(amt);
  out.push_back(implied("SEC"));
  zp("LDA", rt_.ssp, out);
  out.push_back(op_code("SBC", Mode::kImm));
  out.push_back(static_cast<std::uint8_t>(n & 0xFF));
  zp("STA", rt_.ssp, out);
  zp("LDA", rt_.ssp + 1, out);
  out.push_back(op_code("SBC", Mode::kImm));
  out.push_back(static_cast<std::uint8_t>(n >> 8));
  zp("STA", rt_.ssp + 1, out);
}

void Emitter::prologue(const ir::FunctionPrologue& p, Bytes& out) const {
  if (p.arg_bytes + p.local_bytes == 0) {
    return;
  }
  if (p.local_bytes < 0 || p.local_bytes > 253) {
    fail("local_bytes " + str(p.local_bytes) + " out of range 0..253");
  }
  const std::uint8_t ldy = op_code("LDY", Mode::kImm);
  const std::uint8_t sta_indy = op_code("STA", Mode::kIndy);
  ssp_sub(p.local_bytes + 2, out);
  zp("LDA", rt_.fp, out);
  out.insert(out.end(), {ldy, byte(p.local_bytes + 1), sta_indy, byte(rt_.ssp)});
  zp("LDA", rt_.fp + 1, out);
  out.insert(out.end(), {ldy, byte(p.local_bytes + 2), sta_indy, byte(rt_.ssp)});
  zp("LDA", rt_.ssp, out);
  zp("STA", rt_.fp, out);
  zp("LDA", rt_.ssp + 1, out);
  zp("STA", rt_.fp + 1, out);
  for (std::size_t i = 0; i < p.callee_saved_addrs.size(); ++i) {
    const Int addr = p.callee_saved_addrs[i];
    if (addr < 0 || addr > 0xFF) {
      fail("callee-saved address " + hex_alt(addr) + " out of range 0..0xFF");
    }
    zp("LDA", addr, out);
    out.insert(out.end(),
               {ldy, byte(static_cast<long long>(i) + 1), sta_indy, byte(rt_.fp)});
  }
}

void Emitter::ret(const ir::Ret& r, Bytes& out) const {
  if (r.arg_bytes + r.local_bytes == 0) {
    out.push_back(implied("RTS"));
    return;
  }
  if (r.local_bytes < 0 || r.local_bytes > 253) {
    fail("local_bytes " + str(r.local_bytes) + " out of range 0..253");
  }
  const std::uint8_t ldy = op_code("LDY", Mode::kImm);
  const std::uint8_t lda_indy = op_code("LDA", Mode::kIndy);
  const Int rewind = r.arg_bytes + r.local_bytes + 2;
  if (r.save_x) {
    out.insert(out.end(), {implied("PHA"), implied("TXA"), implied("PHA")});
  } else if (r.save_a) {
    out.push_back(implied("PHA"));
  }
  if (r.save_c) {
    out.push_back(implied("PHP"));
  }
  for (std::size_t i = 0; i < r.callee_saved_addrs.size(); ++i) {
    const Int addr = r.callee_saved_addrs[i];
    if (addr < 0 || addr > 0xFF) {
      fail("callee-saved address " + hex_alt(addr) + " out of range 0..0xFF");
    }
    out.insert(out.end(), {ldy, byte(static_cast<long long>(i) + 1), lda_indy, byte(rt_.fp)});
    zp("STA", addr, out);
  }
  if (rewind == 0) {
    zp("LDA", rt_.fp, out);
    zp("STA", rt_.ssp, out);
    zp("LDA", rt_.fp + 1, out);
    zp("STA", rt_.ssp + 1, out);
  } else {
    const int w = static_cast<int>(rewind & Int(0xFFFF));
    out.push_back(implied("CLC"));
    zp("LDA", rt_.fp, out);
    out.push_back(op_code("ADC", Mode::kImm));
    out.push_back(static_cast<std::uint8_t>(w & 0xFF));
    zp("STA", rt_.ssp, out);
    zp("LDA", rt_.fp + 1, out);
    out.push_back(op_code("ADC", Mode::kImm));
    out.push_back(static_cast<std::uint8_t>(w >> 8));
    zp("STA", rt_.ssp + 1, out);
  }
  out.insert(out.end(), {ldy, byte(r.local_bytes + 1), lda_indy, byte(rt_.fp), implied("TAX")});
  out.insert(out.end(), {ldy, byte(r.local_bytes + 2), lda_indy, byte(rt_.fp)});
  zp("STA", rt_.fp + 1, out);
  out.push_back(implied("TXA"));
  zp("STA", rt_.fp, out);
  if (r.save_c) {
    out.push_back(implied("PLP"));
  }
  if (r.save_x) {
    out.insert(out.end(), {implied("PLA"), implied("TAX"), implied("PLA")});
  } else if (r.save_a) {
    out.push_back(implied("PLA"));
  }
  out.push_back(implied("RTS"));
}

void Emitter::decimal_arith(const ir::DecimalArith& d, Bytes& out) const {
  const ir::Type_operand a = ir::Reg{.reg = ir::A{}};
  out.push_back(implied("SED"));
  out.push_back(implied(d.subtract ? "SEC" : "CLC"));
  const char* mn = d.subtract ? "SBC" : "ADC";
  const std::size_t k = std::min({d.left.size(), d.right.size(), d.dst.size()});
  for (std::size_t i = 0; i < k; ++i) {
    mov(d.left[i], a, out);
    accum_arith(mn, d.right[i], out);
    mov(a, d.dst[i], out);
  }
  out.push_back(implied("CLD"));
}

ir::Type_operand offset_by(const ir::Type_operand& op, Int k) {
  if (const auto* f = as<ir::Frame>(op)) return ir::Frame{.offset = f->offset + k};
  if (const auto* s = as<ir::Stack>(op)) return ir::Stack{.offset = s->offset + k};
  if (const auto* d = as<ir::Data>(op)) return ir::Data{.name = d->name, .offset = d->offset + k};
  if (const auto* i = as<ir::Indirect>(op)) return ir::Indirect{.offset = i->offset + k};
  if (const auto* z = as<ir::ZP>(op)) return ir::ZP{.address = z->address, .offset = z->offset + k};
  throw InternalError("TypeError: can't shift offset on operand " + repr(op));
}

void Emitter::load_address(const ir::Type_operand& src, const ir::Type_operand& dst,
                           Bytes& out) const {
  reject_pseudo(src);
  reject_pseudo(dst);
  if (!is_memory(dst)) {
    fail("LoadAddress dst must be memory; got " + repr(dst));
  }
  if (const auto* d = as<ir::Data>(src)) {
    const std::int64_t addr = address(d->name, d->offset);
    lda_imm(addr & 0xFF, out);
    store_a_to_mem(dst, out);
    lda_imm(addr >> 8, out);
    store_a_to_mem(offset_by(dst, 1), out);
    return;
  }
  if (const auto* f = as<ir::Frame>(src)) {
    if (f->offset < 0 || f->offset > 0xFF) {
      fail("LoadAddress Frame offset " + str(f->offset) + " oor");
    }
    out.push_back(implied("CLC"));
    zp("LDA", rt_.fp, out);
    out.push_back(op_code("ADC", Mode::kImm));
    out.push_back(static_cast<std::uint8_t>(static_cast<int>(f->offset)));
    store_a_to_mem(dst, out);
    zp("LDA", rt_.fp + 1, out);
    out.push_back(op_code("ADC", Mode::kImm));
    out.push_back(0);
    store_a_to_mem(offset_by(dst, 1), out);
    return;
  }
  fail("LoadAddress src must be Data or Frame; got " + repr(src));
}

void check_dst_a(const ir::Type_operand& op, const char* name) {
  if (!is_reg_a(op)) {
    fail(std::string(name) + " dst must be Reg(A); got " + repr(op));
  }
}

// sim/assembler.py's `_emit_instr`.
void Emitter::instr(const ir::Type_instruction& instr, std::int64_t addr, Bytes& out) const {
  const ir::Type_operand a = ir::Reg{.reg = ir::A{}};
  switch (ir::kind(instr)) {
    case ir::Kind_instruction::Mov: {
      const auto& m = std::get<ir::Mov>(instr);
      if (is_volatile_self_mov(m)) {
        mov(m.src, a, out);
        mov(a, m.dst, out);
      } else {
        mov(m.src, m.dst, out);
      }
      return;
    }
    case ir::Kind_instruction::Add: {
      const auto& x = std::get<ir::Add>(instr);
      check_dst_a(x.dst, "Add");
      accum_arith("ADC", x.src, out);
      return;
    }
    case ir::Kind_instruction::Sub: {
      const auto& x = std::get<ir::Sub>(instr);
      check_dst_a(x.dst, "Sub");
      accum_arith("SBC", x.src, out);
      return;
    }
    case ir::Kind_instruction::And: {
      const auto& x = std::get<ir::And>(instr);
      check_dst_a(x.dst, "And");
      accum_arith("AND", x.src, out);
      return;
    }
    case ir::Kind_instruction::Or: {
      const auto& x = std::get<ir::Or>(instr);
      check_dst_a(x.dst, "Or");
      accum_arith("ORA", x.src, out);
      return;
    }
    case ir::Kind_instruction::Xor: {
      const auto& x = std::get<ir::Xor>(instr);
      check_dst_a(x.dst, "Xor");
      if (is_reg_a(x.src1)) {
        accum_arith("EOR", x.src2, out);
      } else if (is_reg_a(x.src2)) {
        accum_arith("EOR", x.src1, out);
      } else {
        fail("Xor srcs must include Reg(A); got " + repr(x.src1) + ", " + repr(x.src2));
      }
      return;
    }
    case ir::Kind_instruction::Compare: {
      const auto& c = std::get<ir::Compare>(instr);
      compare(c.left, c.right, out);
      return;
    }
    case ir::Kind_instruction::BitTest:
      bit_test(std::get<ir::BitTest>(instr).src, out);
      return;
    case ir::Kind_instruction::ClearCarry:
      out.push_back(implied("CLC"));
      return;
    case ir::Kind_instruction::SetCarry:
      out.push_back(implied("SEC"));
      return;
    case ir::Kind_instruction::SetDecimal:
      out.push_back(implied("SED"));
      return;
    case ir::Kind_instruction::ClearDecimal:
      out.push_back(implied("CLD"));
      return;
    case ir::Kind_instruction::Inc:
      inc_dec(std::get<ir::Inc>(instr).dst, true, out);
      return;
    case ir::Kind_instruction::Dec:
      inc_dec(std::get<ir::Dec>(instr).dst, false, out);
      return;
    case ir::Kind_instruction::Push: {
      const auto& src = std::get<ir::Push>(instr).src;
      if (!is_reg_a(src)) {
        fail("Push src must be Reg(A); got " + repr(src));
      }
      out.push_back(implied("PHA"));
      return;
    }
    case ir::Kind_instruction::Pop: {
      const auto& dst = std::get<ir::Pop>(instr).dst;
      if (!is_reg_a(dst)) {
        fail("Pop dst must be Reg(A); got " + repr(dst));
      }
      out.push_back(implied("PLA"));
      return;
    }
    case ir::Kind_instruction::ArithmeticShiftLeft:
      shift("ASL", std::get<ir::ArithmeticShiftLeft>(instr).dst, out);
      return;
    case ir::Kind_instruction::LogicalShiftRight:
      shift("LSR", std::get<ir::LogicalShiftRight>(instr).dst, out);
      return;
    case ir::Kind_instruction::RotateLeft:
      shift("ROL", std::get<ir::RotateLeft>(instr).dst, out);
      return;
    case ir::Kind_instruction::RotateRight:
      shift("ROR", std::get<ir::RotateRight>(instr).dst, out);
      return;
    case ir::Kind_instruction::Call: {
      const std::int64_t to = lookup(std::get<ir::Call>(instr).name);
      out.push_back(op_code("JSR", Mode::kAbs));
      abs_word(to, out);
      return;
    }
    case ir::Kind_instruction::Jump: {
      const std::int64_t to = lookup(std::get<ir::Jump>(instr).target);
      out.push_back(op_code("JMP", Mode::kAbs));
      abs_word(to, out);
      return;
    }
    case ir::Kind_instruction::JumpTable:
      jump_table(addr, std::get<ir::JumpTable>(instr), out);
      return;
    case ir::Kind_instruction::Branch: {
      const auto& b = std::get<ir::Branch>(instr);
      branch(b.cond, b.target, addr, out);
      return;
    }
    case ir::Kind_instruction::Label:
      return;
    case ir::Kind_instruction::AllocateStack:
      ssp_sub(std::get<ir::AllocateStack>(instr).bytes, out);
      return;
    case ir::Kind_instruction::FunctionPrologue:
      prologue(std::get<ir::FunctionPrologue>(instr), out);
      return;
    case ir::Kind_instruction::Ret:
      ret(std::get<ir::Ret>(instr), out);
      return;
    case ir::Kind_instruction::Return:
      out.push_back(implied("RTS"));
      return;
    case ir::Kind_instruction::LoadAddress: {
      const auto& la = std::get<ir::LoadAddress>(instr);
      load_address(la.src, la.dst, out);
      return;
    }
    case ir::Kind_instruction::DecimalArith:
      decimal_arith(std::get<ir::DecimalArith>(instr), out);
      return;
    case ir::Kind_instruction::Phi:
      throw InternalError("TypeError: sim.assembler: Phi node leaked past SSA destruction");
  }
  throw InternalError("TypeError: unexpected instruction");
}

void Emitter::init(const ir::Type_static_init& item, Bytes& out) const {
  const auto little_endian = [&out](Int v, int n) {
    for (int i = 0; i < n; ++i) {
      out.push_back(static_cast<std::uint8_t>(static_cast<int>((v >> (8 * i)) & Int(0xFF))));
    }
  };
  switch (ir::kind(item)) {
    case ir::Kind_static_init::CharInit: {
      const Int v = std::get<ir::CharInit>(item).value;
      if (v < -128 || v > 0xFF) {
        fail("CharInit " + str(v) + " out of range");
      }
      little_endian(v, 1);
      return;
    }
    case ir::Kind_static_init::IntInit: {
      const Int v = std::get<ir::IntInit>(item).value;
      if (v < -32768 || v > 0xFFFF) {
        fail("IntInit " + str(v) + " out of range");
      }
      little_endian(v, 2);
      return;
    }
    case ir::Kind_static_init::LongInit:
      little_endian(std::get<ir::LongInit>(item).value, 4);
      return;
    case ir::Kind_static_init::LongLongInit:
      little_endian(std::get<ir::LongLongInit>(item).value, 8);
      return;
    case ir::Kind_static_init::FloatInit:
      little_endian(std::get<ir::FloatInit>(item).bits, 4);
      return;
    case ir::Kind_static_init::DoubleInit:
      little_endian(std::get<ir::DoubleInit>(item).bits, 8);
      return;
    case ir::Kind_static_init::AddressInit: {
      const auto& x = std::get<ir::AddressInit>(item);
      little_endian(address(x.name, x.offset), 2);
      return;
    }
    case ir::Kind_static_init::AddressByteInit: {
      const auto& x = std::get<ir::AddressByteInit>(item);
      const std::int64_t addr = address(x.name, x.offset);
      out.push_back(static_cast<std::uint8_t>(x.high ? addr >> 8 : addr & 0xFF));
      return;
    }
    case ir::Kind_static_init::StringInit: {
      const auto& s = std::get<ir::StringInit>(item);
      const std::vector<std::uint32_t> chars = pyrepr::code_points(s.str);
      const Int len = static_cast<long long>(chars.size());
      if (s.bytes < len) {
        fail("StringInit bytes=" + str(s.bytes) + " < len(str)=" + str(len));
      }
      for (const std::uint32_t c : chars) {
        out.push_back(static_cast<std::uint8_t>(c & 0xFF));
      }
      out.insert(out.end(), static_cast<std::size_t>(to_i64(s.bytes - len)), 0);
      return;
    }
    case ir::Kind_static_init::ZeroInit: {
      const Int n = std::get<ir::ZeroInit>(item).bytes;
      if (n <= 0) {
        fail("ZeroInit byte count " + str(n) + " not positive");
      }
      out.insert(out.end(), static_cast<std::size_t>(to_i64(n)), 0);
      return;
    }
  }
}

// sim/assembler.py's `_lay_down`.
void lay_down(std::vector<std::uint8_t>& image, std::int64_t addr, const Bytes& bytes,
              std::int64_t code_end) {
  if (bytes.empty()) {
    return;
  }
  if (addr < 0) {
    fail("the program's bytes start at -$" + pyrepr::hex(-addr, 4) + ", below $0000");
  }
  if (addr + static_cast<std::int64_t>(bytes.size()) > 0x10000) {
    fail("the program's bytes end at $" + pyrepr::hex(code_end, 4) + ", past $FFFF");
  }
  std::copy(bytes.begin(), bytes.end(), image.begin() + addr);
}

}  // namespace

int instruction_size(const ir::Type_instruction& instr,
                     const passes::NameSet* zp_names) {
  try {
    return Sizer(zp_names).instr_size(instr);
  } catch (const AssemblerError& e) {
    throw InternalError(std::string("AssemblerError: ") + e.what());
  }
}

// sim/assembler.py's `assemble`.
AssembledProgram assemble(const ir::Program& program, std::int64_t origin,
                          const std::map<std::string, std::int64_t>& extra_symbols) {
  Symbols symbols = {{"SSP", 0x00}, {"FP", 0x02}, {"HARGS", 0x04}, {"DPTR", 0x24}};
  for (const auto& [name, addr] : extra_symbols) {
    symbols[name] = addr;
  }
  const Runtime runtime{symbols.at("SSP"), symbols.at("FP"), symbols.at("DPTR")};
  passes::NameSet zp_names;
  for (const auto& [name, addr] : symbols) {
    if (0 <= addr && addr <= 0xFF) {
      zp_names.insert(name);
    }
  }
  const Sizer sizer(&zp_names);

  const auto define = [&symbols](const std::string& name, std::int64_t addr, const char* what) {
    if (!symbols.emplace(name, addr).second) {
      fail(what + pyrepr::string(name));
    }
  };
  std::int64_t addr = origin;
  for (const ir::Type_top_level& tl : program.top_level) {
    if (const auto* fn = as<ir::Function>(tl)) {
      define(fn->name, addr, "duplicate symbol ");
      for (const ir::Type_instruction& instr : fn->instructions) {
        if (const auto* label = as<ir::Label>(instr)) {
          define(label->name, addr, "duplicate label ");
        } else {
          addr += sizer.instr_size(instr);
        }
      }
    } else {
      const auto& sv = std::get<ir::StaticVariable>(tl);
      define(sv.name, addr, "duplicate symbol ");
      for (const ir::Type_static_init& item : sv.init) {
        addr += init_size(item);
      }
    }
  }
  const std::int64_t code_end = addr;

  AssembledProgram out;
  out.image.assign(0x10000, 0);
  const Emitter emitter(runtime, symbols);
  addr = origin;
  Bytes bytes;
  for (const ir::Type_top_level& tl : program.top_level) {
    if (const auto* fn = as<ir::Function>(tl)) {
      for (const ir::Type_instruction& instr : fn->instructions) {
        if (isa<ir::Label>(instr)) {
          continue;
        }
        bytes.clear();
        emitter.instr(instr, addr, bytes);
        const int expected = sizer.instr_size(instr);
        if (static_cast<int>(bytes.size()) != expected) {
          throw InternalError("AssemblerError: size mismatch for " + py_repr(instr) +
                              ": pass1=" + std::to_string(expected) +
                              ", pass2=" + std::to_string(bytes.size()));
        }
        lay_down(out.image, addr, bytes, code_end);
        addr += static_cast<std::int64_t>(bytes.size());
      }
    } else {
      for (const ir::Type_static_init& item : std::get<ir::StaticVariable>(tl).init) {
        bytes.clear();
        emitter.init(item, bytes);
        if (static_cast<std::int64_t>(bytes.size()) != init_size(item)) {
          throw InternalError("AssemblerError: size mismatch for static_init " + py_repr(item));
        }
        lay_down(out.image, addr, bytes, code_end);
        addr += static_cast<std::int64_t>(bytes.size());
      }
    }
  }
  if (addr != code_end) {
    throw InternalError("sim::assemble: pass 2 ended at another address than pass 1");
  }
  out.symbols = std::move(symbols);
  out.origin = origin;
  out.code_end = code_end;
  return out;
}

}  // namespace c6502::sim
