// Writes 6502 assembly in dasm's syntax from an asm2 program.
//
// The formatting rules:
//
//   * labels start in column 1,
//   * opcodes, in capitals, start in column 4,
//   * operands start in column 10,
//   * the functions and statics of a program are written in order, one
//     blank line apart, and a run of `Blank` atoms inside a function
//     gives one blank line.
//
// The asm2 IR has one node per logical 6502 instruction. The compound
// nodes `AllocateStack`, `FunctionPrologue`, and `Ret`, which this stage
// once expanded, arrive already expanded by `passes::lower_to_asm2`. A
// `Pseudo` operand can't reach this stage, since
// `replace_pseudoregisters` resolves each into a `Frame`, `Stack`,
// `Data`, or `ZP` operand before `asm_to_asm2` runs, and the asm2 IR
// has no `Pseudo`. Three atoms exist only in asm2:
//
//   * `Return` is `RTS`, the bare end of an epilogue, and what `Ret`
//     becomes in a function with no frame.
//   * `Comment(text)` is a "; ..." line at the opcode column, with which
//     the prologue and epilogue lowerings mark their boilerplate.
//   * `Blank` is a blank line between the prologue, the body, and the
//     epilogue.
//
// The soft stack (the README's "Function stack frame layout"):
//
//   * the soft stack pointer is the symbol `SSP`, a 16-bit zero-page
//     value with its low byte at `SSP` and its high byte at `SSP+1`,
//   * the frame pointer is the symbol `FP`, a 16-bit zero-page value
//     that the function sets once at entry and that stays put when SSP
//     moves in the body,
//   * `Stack(off)` is the byte at `SSP+off` and `Frame(off)` the byte at
//     `FP+off`, each written as `LDY #off` and then `LDA (PTR),Y` or
//     `STA (PTR),Y`,
//   * any indirect access changes Y.
//
// Each node of the asm2 IR is one 6502 opcode, where the setup of an
// addressing mode, such as the `LDY` of an indirect-Y access, counts as
// part of the opcode. Larger operations are lowered into atoms by
// `asm_to_asm2` before they reach this stage.
//
// The arithmetic and flag atoms:
//
//   * `ClearCarry` is `CLC` and `SetCarry` is `SEC`.
//   * `Inc(dst)` and `Dec(dst)` take `Reg(X)`, `Reg(Y)`, a `Data` or
//     `ZP` byte, or an `IndexedData` indexed by X: `INX`, `INY`, `DEX`,
//     `DEY`, `INC name+off`, `INC name+off,X`. The 6502 has no `INA`, so
//     a step of A goes through `CLC; ADC #1`.
//   * `Push(Reg(A))` is `PHA`, `Pop(Reg(A))` is `PLA`, `PushStatus` is
//     `PHP`, and `PullStatus` is `PLP`.
//   * `SetDecimal` is `SED` and `ClearDecimal` is `CLD`, which only the
//     expansion of a packed BCD `DecimalArith` makes.
//   * `Xor(src1, src2, dst)` is `EOR <src>`, with `dst` and one source
//     `Reg(A)`. The other source gives the addressing mode.
//   * `And` is `AND` and `Or` is `ORA`, with the operands of `Add`.
//   * `ArithmeticShiftLeft`, `LogicalShiftRight`, `RotateLeft`, and
//     `RotateRight` are `ASL`, `LSR`, `ROL`, and `ROR` of A, of a `Data`
//     or `ZP` byte, or of an `IndexedData` indexed by X. The 6502 has no
//     indirect-Y form of them, so a `Stack` or `Frame` byte goes through
//     A. The runtime helpers shift their HARGS bytes in place.
//   * `Add(src, Reg(A))` is `ADC <src>` and `Sub` is `SBC`. The carry is
//     set up by the `ClearCarry` or `SetCarry` before it.
//   * `Call` is `JSR`, `Jump` is `JMP`, and `Branch(cond, target)` is
//     `B<cond>`, whose displacement the assembler computes from the
//     label.
//   * `Label(name)` is `name:` in column 1.
//   * `Compare(left, right)` is `CMP`, `CPX`, or `CPY` by the register
//     on the left. `CPX` and `CPY` have immediate and absolute modes but
//     no indirect-Y, so a soft-stack operand is compared only with A.
//
// `Mov(src, dst)` covers the transfers between registers (`TAX`, `TAY`,
// `TXA`, `TYA`), the loads and stores, and the loads of a constant. The
// 6502 has no `TXY` or `TYX`, so `Mov(Reg(X), Reg(Y))` is `TXA; TAY`,
// in this one atom. A `Mov` whose source and destination are the same
// operand is dropped. Register allocation leaves one when a Phi's
// source and destination get the same color.
//
// `BitTest(src)` is the NMOS 6502's `BIT src`, which has only zero-page
// and absolute addressing. It sets N to bit 7 of `src`, V to bit 6, and
// Z from `A & src`, and leaves A alone. `and_sign_bit_branch` makes it
// from a test of the sign bit.
//
// A `Data(name, offset)` operand is a static object by name, written
// with absolute addressing, `name+offset`. A `ZP(address, offset)`
// operand is a zero-page byte that register allocation chose, written
// as the byte `$XX` of `address + offset`.
//
// A `StaticVariable` is its label and a directive for each initializer
// item: `DC.B` for a `CharInit`, `DC.W` for an `IntInit` (masked to 16
// bits, so a negative value is its two's complement), `DC.L` for a
// `LongInit` and a `FloatInit`, two `DC.L`s, the low half first, for a
// `LongLongInit` and a `DoubleInit`, since dasm has no 8-byte
// directive, `DC.W name+offset` for an `AddressInit`, `DC.B <(...)` or
// `DC.B >(...)` for an `AddressByteInit`, rows of `DC.B` bytes for a
// `StringInit`, and `DS.B n` for a `ZeroInit`. dasm lays every
// directive's bytes down low byte first, as the soft stack does.
//
// `is_global` doesn't change the text, since dasm has no export
// directive. It goes into the link metadata's `item` line instead.
#include "asm_emit.h"

#include <algorithm>
#include <optional>
#include <string_view>

#include "errors.h"
#include "passes/y_peephole.h"
#include "pyrepr.h"

namespace c6502::asm_emit {

namespace {

namespace ir = ::c6502::asm2_ast;

using Lines = std::vector<std::string>;

// The 0-based columns of the opcode and the operand.
constexpr std::size_t kOpcodeCol = 3;
constexpr std::size_t kOperandCol = 9;

// The runtime's zero-page pointers, which the runtime header binds with
// `EQU`s. DPTR is the pointer scratch behind `Indirect(off)`.
constexpr const char* kSsp = "SSP";
constexpr const char* kFp = "FP";
constexpr const char* kDptr = "DPTR";

[[noreturn]] void value_error(const std::string& message) {
  throw CompileError("ValueError", message);
}

[[noreturn]] void type_error(const std::string& message) {
  throw CompileError("TypeError", message);
}

std::string repr_reg(const ir::Type_reg& r) {
  switch (ir::kind(r)) {
    case ir::Kind_reg::A:
      return "A()";
    case ir::Kind_reg::X:
      return "X()";
    case ir::Kind_reg::Y:
      return "Y()";
  }
  return "?";
}

// Returns Python's `repr` of an asm2 operand, for the messages that
// quote one.
std::string repr(const ir::Type_operand& op) {
  using pyrepr::integer;
  using pyrepr::string;
  switch (ir::kind(op)) {
    case ir::Kind_operand::Imm:
      return "Imm(value=" + integer(std::get<ir::Imm>(op).value) + ")";
    case ir::Kind_operand::Reg:
      return "Reg(reg=" + repr_reg(std::get<ir::Reg>(op).reg) + ")";
    case ir::Kind_operand::Stack:
      return "Stack(offset=" + integer(std::get<ir::Stack>(op).offset) + ")";
    case ir::Kind_operand::Frame:
      return "Frame(offset=" + integer(std::get<ir::Frame>(op).offset) + ")";
    case ir::Kind_operand::Data: {
      const auto& d = std::get<ir::Data>(op);
      return "Data(name=" + string(d.name) + ", offset=" + integer(d.offset) + ")";
    }
    case ir::Kind_operand::Indirect:
      return "Indirect(offset=" + integer(std::get<ir::Indirect>(op).offset) + ")";
    case ir::Kind_operand::IndirectY:
      return "IndirectY()";
    case ir::Kind_operand::IndirectZp: {
      const auto& z = std::get<ir::IndirectZp>(op);
      return "IndirectZp(address=" + integer(z.address) + ", offset=" + integer(z.offset) + ")";
    }
    case ir::Kind_operand::IndirectZpY:
      return "IndirectZpY(address=" + integer(std::get<ir::IndirectZpY>(op).address) + ")";
    case ir::Kind_operand::ZP: {
      const auto& z = std::get<ir::ZP>(op);
      return "ZP(address=" + integer(z.address) + ", offset=" + integer(z.offset) + ")";
    }
    case ir::Kind_operand::ImmLabelLow: {
      const auto& l = std::get<ir::ImmLabelLow>(op);
      return "ImmLabelLow(name=" + string(l.name) + ", offset=" + integer(l.offset) + ")";
    }
    case ir::Kind_operand::ImmLabelHigh: {
      const auto& h = std::get<ir::ImmLabelHigh>(op);
      return "ImmLabelHigh(name=" + string(h.name) + ", offset=" + integer(h.offset) + ")";
    }
    case ir::Kind_operand::IndexedData: {
      const auto& x = std::get<ir::IndexedData>(op);
      return "IndexedData(name=" + string(x.name) + ", offset=" + integer(x.offset) +
             ", index=" + repr_reg(x.index) + ")";
    }
  }
  return "?";
}

std::string upper(std::string_view s) {
  std::string out(s);
  for (char& c : out) {
    if (c >= 'a' && c <= 'z') {
      c = static_cast<char>(c - 'a' + 'A');
    }
  }
  return out;
}

}  // namespace

std::string instr_line(std::string_view opcode, std::string_view operand) {
  std::string line(kOpcodeCol, ' ');
  line += upper(opcode);
  if (!operand.empty()) {
    const std::size_t pad = line.size() < kOperandCol ? kOperandCol - line.size() : 1;
    line.append(pad > 0 ? pad : 1, ' ');
    line += operand;
  }
  return line;
}

namespace {

// Returns a comment line at the opcode column, with which the prologue
// and epilogue mark their boilerplate.
std::string comment_line(std::string_view text) {
  return std::string(kOpcodeCol, ' ') + "; " + std::string(text);
}

void check_byte(std::string_view label, asdl::Int v) {
  if (!(0 <= v && v <= 255)) {
    value_error(std::string(label) + " " + pyrepr::integer(v) +
                " out of range for 6502 (expected 0..255)");
  }
}

std::string hex2(asdl::Int v) { return pyrepr::hex(v, 2); }

const char* reg_letter(const ir::Type_reg& r) {
  switch (ir::kind(r)) {
    case ir::Kind_reg::A:
      return "A";
    case ir::Kind_reg::X:
      return "X";
    case ir::Kind_reg::Y:
      return "Y";
  }
  return "?";
}

// Returns the two letters of a branch opcode (`CC` for `BCC`), which
// are the condition's constructor name.
const char* cond_suffix(const ir::Type_condition& c) {
  switch (ir::kind(c)) {
    case ir::Kind_condition::CC:
      return "CC";
    case ir::Kind_condition::CS:
      return "CS";
    case ir::Kind_condition::EQ:
      return "EQ";
    case ir::Kind_condition::MI:
      return "MI";
    case ir::Kind_condition::NE:
      return "NE";
    case ir::Kind_condition::PL:
      return "PL";
    case ir::Kind_condition::VC:
      return "VC";
    case ir::Kind_condition::VS:
      return "VS";
  }
  return "?";
}

bool is_reg(const ir::Type_operand& op, ir::Kind_reg r) {
  const auto* reg = asdl::as<ir::Reg>(op);
  return reg != nullptr && ir::kind(reg->reg) == r;
}

bool is_reg_a(const ir::Type_operand& op) { return is_reg(op, ir::Kind_reg::A); }

// Whether `op` is a memory operand, which needs a load or a store where
// a register takes a transfer.
bool is_memory_operand(const ir::Type_operand& op) {
  switch (ir::kind(op)) {
    case ir::Kind_operand::Stack:
    case ir::Kind_operand::Frame:
    case ir::Kind_operand::Data:
    case ir::Kind_operand::Indirect:
    case ir::Kind_operand::IndirectY:
    case ir::Kind_operand::IndirectZp:
    case ir::Kind_operand::IndirectZpY:
    case ir::Kind_operand::ZP:
      return true;
    default:
      return false;
  }
}

// Whether `op` is written with absolute or zero-page addressing, a
// `Data` symbol or a `ZP` byte, without the `LDY` of an indirect-Y
// access.
bool is_data_or_zp(const ir::Type_operand& op) { return asdl::isa<ir::Data, ir::ZP>(op); }

bool is_imm_label(const ir::Type_operand& op) {
  return asdl::isa<ir::ImmLabelLow, ir::ImmLabelHigh>(op);
}

// Whether `op` is an `IndexedData` indexed by X, the one indexed form
// that `INC`, `DEC`, `ASL`, `LSR`, `ROL`, and `ROR` take. The 6502 has
// no `abs,Y` mode for them.
bool is_x_indexed(const ir::Type_operand& op) {
  const auto* x = asdl::as<ir::IndexedData>(op);
  return x != nullptr && ir::kind(x->index) == ir::Kind_reg::X;
}

// Returns the operand of a `Data` byte: `name` at offset 0, `name+k`,
// or `name-k`.
std::string data_addr(const ir::Data& d) {
  if (d.offset == 0) {
    return d.name;
  }
  return d.name + pyrepr::signed_integer(d.offset);
}

// Returns the operand of a `ZP` byte, `$XX` of `address + offset`. A
// byte outside zero page would be a bug of register allocation.
std::string zp_addr(const ir::ZP& z) {
  const asdl::Int addr = z.address + z.offset;
  check_byte("ZP address", addr);
  return "$" + hex2(addr);
}

// Returns the `abs,X` or `abs,Y` operand of an indexed byte:
// `name+offset,X`, or `$XXXX,X` for an empty name, which the
// IndexedStore lowering uses for a constant address. A negative offset,
// which a lookup of row `i - 1` in a table of row addresses has, is
// `name-k,X`.
std::string indexed_data_addr(const ir::IndexedData& d) {
  std::string base;
  if (!d.name.empty()) {
    base = d.offset == 0 ? d.name : d.name + pyrepr::signed_integer(d.offset);
  } else {
    base = "$" + pyrepr::hex(d.offset, 4);
  }
  return base + "," + reg_letter(d.index);
}

std::string abs_addr(const ir::Type_operand& op) {
  if (const auto* d = asdl::as<ir::Data>(op)) {
    return data_addr(*d);
  }
  if (const auto* z = asdl::as<ir::ZP>(op)) {
    return zp_addr(*z);
  }
  type_error("_abs_addr only handles Data / ZP, got " + repr(op));
}

// Returns the indirect-Y operand of a Stack, Frame, or Indirect
// operand, through SSP, FP, or DPTR. The `LDY` before the access gives
// the index.
std::string indirect_addr(const ir::Type_operand& op) {
  switch (ir::kind(op)) {
    case ir::Kind_operand::Stack:
      return std::string("(") + kSsp + "),Y";
    case ir::Kind_operand::Frame:
      return std::string("(") + kFp + "),Y";
    case ir::Kind_operand::Indirect:
      return std::string("(") + kDptr + "),Y";
    default:
      type_error("not an indirect operand: " + repr(op));
  }
}

// Returns the `offset` of an operand that has one, for the `LDY` of an
// indirect-Y access.
asdl::Int operand_offset(const ir::Type_operand& op) {
  switch (ir::kind(op)) {
    case ir::Kind_operand::Stack:
      return std::get<ir::Stack>(op).offset;
    case ir::Kind_operand::Frame:
      return std::get<ir::Frame>(op).offset;
    case ir::Kind_operand::Indirect:
      return std::get<ir::Indirect>(op).offset;
    case ir::Kind_operand::IndirectZp:
      return std::get<ir::IndirectZp>(op).offset;
    case ir::Kind_operand::Data:
      return std::get<ir::Data>(op).offset;
    case ir::Kind_operand::ZP:
      return std::get<ir::ZP>(op).offset;
    case ir::Kind_operand::ImmLabelLow:
      return std::get<ir::ImmLabelLow>(op).offset;
    case ir::Kind_operand::ImmLabelHigh:
      return std::get<ir::ImmLabelHigh>(op).offset;
    case ir::Kind_operand::IndexedData:
      return std::get<ir::IndexedData>(op).offset;
    default:
      throw CompileError("AttributeError", "operand has no offset: " + repr(op));
  }
}

std::string load_y(asdl::Int off) {
  check_byte("offset", off);
  return instr_line("LDY", "#$" + hex2(off));
}

// Returns an `ImmLabelLow` or `ImmLabelHigh` operand as dasm's
// immediate: `#<name` or `#>name`, and `#<(name+offset)` with the sum in
// parentheses, so that dasm applies the `<` to all of it.
std::string imm_label_text(const ir::Type_operand& op) {
  std::string prefix;
  const std::string* name = nullptr;
  asdl::Int offset = 0;
  if (const auto* l = asdl::as<ir::ImmLabelLow>(op)) {
    prefix = "<";
    name = &l->name;
    offset = l->offset;
  } else if (const auto* h = asdl::as<ir::ImmLabelHigh>(op)) {
    prefix = ">";
    name = &h->name;
    offset = h->offset;
  } else {
    type_error("not an ImmLabel operand: " + repr(op));
  }
  if (offset == 0) {
    return "#" + prefix + *name;
  }
  return "#" + prefix + "(" + *name + "+" + pyrepr::integer(offset) + ")";
}

void check_dst_is_a(const ir::Type_operand& dst, std::string_view op_name) {
  if (!is_reg_a(dst)) {
    value_error(std::string(op_name) + " dst must be Reg(A), got " + repr(dst));
  }
}

class Emitter {
 public:
  Emitter(std::map<asdl::Int, std::vector<std::string>> addr_to_syms,
          const std::map<std::string, passes::NameSet, std::less<>>& preferred)
      : addr_to_syms_(std::move(addr_to_syms)), preferred_(preferred) {}

  Lines function(const ir::Function& fn);
  Lines static_variable(const ir::StaticVariable& sv) const;
  Lines instruction(const ir::Type_instruction& instr) const;

 private:
  std::optional<std::string> pick_symbol(asdl::Int address) const;
  std::string zpy_operand(asdl::Int address) const;
  Lines memop_load(const ir::Type_operand& op, std::string_view opcode = "LDA") const;
  Lines memop_store(const ir::Type_operand& op) const;
  Lines mov(const ir::Type_operand& src, const ir::Type_operand& dst) const;
  Lines acc_arith_src(std::string_view opcode, const ir::Type_operand& src) const;
  Lines acc_shift(std::string_view opcode, std::string_view op_name,
                  const ir::Type_operand& dst) const;
  Lines step(std::string_view opcode, std::string_view x_op, std::string_view y_op,
             std::string_view name, const ir::Type_operand& dst) const;
  Lines compare(const ir::Type_operand& left, const ir::Type_operand& right) const;
  Lines load_address(const ir::Type_operand& src, const ir::Type_operand& dst) const;

  // Each zero-page address with the symbols that name it, the runtime's
  // first, for writing a `(zp),Y` pointer by name.
  std::map<asdl::Int, std::vector<std::string>> addr_to_syms_;
  const std::map<std::string, passes::NameSet, std::less<>>& preferred_;
  // The function whose body is being written, and the names of the
  // `Data` and `IndexedData` operands in it.
  std::optional<std::string> current_;
  passes::NameSet function_names_;
};

// Whether `name` is one of the compiler's zero-page slots, which the
// linker places again for the whole program.
bool is_slot_symbol(std::string_view name) {
  return name.starts_with("__zpabi_") || name.starts_with("__local_");
}

// Returns the symbol that names the `(zp),Y` pointer at `address` in the
// current function's body, or nothing to write the address.
//
// The operand carries only the address, and several functions' slots
// can share it in this unit. The linker places each slot symbol again
// for the whole program and relocates the operand by the symbol written
// here, so the symbol must be the one whose bytes the function keeps the
// pointer in. That is, in order, one of the function's own slots
// (`__zpabi_<fn>__*`, `__local_<fn>__*`), one of its preferred symbols,
// a slot it names itself elsewhere in its body (a callee's parameter
// slot it stores the pointer in), or a name that isn't a slot (`DPTR`, a
// zero-page object). Another function's slot is never the answer. A
// function without a pool of its own uses the caller-saved bytes at
// fixed addresses, which the address names.
std::optional<std::string> Emitter::pick_symbol(asdl::Int address) const {
  const auto it = addr_to_syms_.find(address);
  if (it == addr_to_syms_.end() || it->second.empty()) {
    return std::nullopt;
  }
  const std::vector<std::string>& candidates = it->second;
  if (current_) {
    const std::string zpabi_prefix = "__zpabi_" + *current_ + "__";
    const std::string local_prefix = "__local_" + *current_ + "__";
    for (const auto& sym : candidates) {
      if (sym.starts_with(zpabi_prefix) || sym.starts_with(local_prefix)) {
        return sym;
      }
    }
    if (const auto pref = preferred_.find(*current_); pref != preferred_.end()) {
      for (const auto& sym : candidates) {
        if (pref->second.contains(sym)) {
          return sym;
        }
      }
    }
    for (const auto& sym : candidates) {
      if (function_names_.contains(sym)) {
        return sym;
      }
    }
  }
  for (const auto& sym : candidates) {
    if (!is_slot_symbol(sym)) {
      return sym;
    }
  }
  return std::nullopt;
}

// Returns the `(addr),Y` operand, by a symbol that names the address
// when there is one, and as `($XX),Y` otherwise.
std::string Emitter::zpy_operand(asdl::Int address) const {
  if (const auto sym = pick_symbol(address)) {
    return "(" + *sym + "),Y";
  }
  return "($" + hex2(address) + "),Y";
}

// Returns the load of the byte `op` addresses into A, or into another
// register for another opcode: absolute for a `Data` or `ZP` byte, and
// indirect-Y for the rest. `IndirectY` and `IndirectZpY` have no `LDY`,
// since the code before them set Y, and `IndirectZp` has an explicit
// zero-page base where `Indirect` has DPTR.
Lines Emitter::memop_load(const ir::Type_operand& op, std::string_view opcode) const {
  if (is_data_or_zp(op)) {
    return {instr_line(opcode, abs_addr(op))};
  }
  if (asdl::isa<ir::IndirectY>(op)) {
    return {instr_line(opcode, std::string("(") + kDptr + "),Y")};
  }
  if (const auto* z = asdl::as<ir::IndirectZpY>(op)) {
    return {instr_line(opcode, zpy_operand(z->address))};
  }
  if (const auto* z = asdl::as<ir::IndirectZp>(op)) {
    Lines out{load_y(z->offset)};
    out.push_back(instr_line(opcode, zpy_operand(z->address)));
    return out;
  }
  Lines out{load_y(operand_offset(op))};
  out.push_back(instr_line(opcode, indirect_addr(op)));
  return out;
}

// Returns the store of A into the byte `op` addresses, with the
// addressing modes of `memop_load`.
Lines Emitter::memop_store(const ir::Type_operand& op) const {
  if (is_data_or_zp(op)) {
    return {instr_line("STA", abs_addr(op))};
  }
  if (asdl::isa<ir::IndirectY>(op)) {
    return {instr_line("STA", std::string("(") + kDptr + "),Y")};
  }
  if (const auto* z = asdl::as<ir::IndirectZpY>(op)) {
    return {instr_line("STA", zpy_operand(z->address))};
  }
  if (const auto* z = asdl::as<ir::IndirectZp>(op)) {
    Lines out{load_y(z->offset)};
    out.push_back(instr_line("STA", zpy_operand(z->address)));
    return out;
  }
  Lines out{load_y(operand_offset(op))};
  out.push_back(instr_line("STA", indirect_addr(op)));
  return out;
}

Lines concat(Lines a, const Lines& b) {
  a.insert(a.end(), b.begin(), b.end());
  return a;
}

Lines Emitter::mov(const ir::Type_operand& src, const ir::Type_operand& dst) const {
  // A `Mov` whose source and destination are the same operand (the same
  // register, zero-page byte, or frame slot) does nothing to memory, and
  // is dropped. Register allocation leaves one when it gives a Phi's
  // source and destination the same color and SSA destruction writes the
  // copy between them. A move from memory through A to the same memory
  // changes A, but the code after it defines A before it reads A, so the
  // output means the same without it.
  if (src == dst) {
    return {};
  }
  // The cases between registers and of a constant into a register each
  // have their own opcodes, and the memory operands share the
  // `memop_load` and `memop_store` paths.
  if (const auto* i = asdl::as<ir::Imm>(src); i != nullptr && asdl::isa<ir::Reg>(dst)) {
    check_byte("immediate", i->value);
    return {instr_line(std::string("LD") + reg_letter(std::get<ir::Reg>(dst).reg),
                       "#$" + hex2(i->value))};
  }
  if (const auto* s = asdl::as<ir::Reg>(src)) {
    if (const auto* d = asdl::as<ir::Reg>(dst)) {
      const ir::Kind_reg from = ir::kind(s->reg);
      const ir::Kind_reg to = ir::kind(d->reg);
      using K = ir::Kind_reg;
      if (from == K::X && to == K::A) {
        return {instr_line("TXA")};
      }
      if (from == K::Y && to == K::A) {
        return {instr_line("TYA")};
      }
      if (from == K::A && to == K::X) {
        return {instr_line("TAX")};
      }
      if (from == K::A && to == K::Y) {
        return {instr_line("TAY")};
      }
      if (from == K::X && to == K::Y) {
        // There is no TXY, so the value goes through A.
        return {instr_line("TXA"), instr_line("TAY")};
      }
      if (from == K::Y && to == K::X) {
        // There is no TYX, so the value goes through A.
        return {instr_line("TYA"), instr_line("TAX")};
      }
    }
  }
  // A label's half loads a register as an immediate does, or loads A
  // for a store. `direct_index_load` makes `LDA #<name; TAY` into
  // `LDY #<name`, as for an argument passed in Y.
  if (is_imm_label(src) && asdl::isa<ir::Reg>(dst)) {
    return {instr_line(std::string("LD") + reg_letter(std::get<ir::Reg>(dst).reg),
                       imm_label_text(src))};
  }
  if (is_imm_label(src) && is_memory_operand(dst)) {
    return concat({instr_line("LDA", imm_label_text(src))}, memop_store(dst));
  }
  // An `IndexedData` source is an `abs,X` or `abs,Y` load from a static
  // array, after the code before it put the byte offset in the index
  // register. Its destination is A, or memory with A between.
  if (const auto* x = asdl::as<ir::IndexedData>(src)) {
    if (is_reg_a(dst)) {
      return {instr_line("LDA", indexed_data_addr(*x))};
    }
    if (is_memory_operand(dst)) {
      return concat({instr_line("LDA", indexed_data_addr(*x))}, memop_store(dst));
    }
    // The 6502 has `LDX abs,Y` and `LDY abs,X`, but no `LDX abs,X` or
    // `LDY abs,Y`, since the index register can't also be the
    // destination of the load. `hwreg_eligibility` doesn't color a
    // Pseudo in a way that would need one.
    if (const auto* d = asdl::as<ir::Reg>(dst)) {
      if (ir::kind(d->reg) == ir::Kind_reg::X && ir::kind(x->index) == ir::Kind_reg::Y) {
        return {instr_line("LDX", indexed_data_addr(*x))};
      }
      if (ir::kind(d->reg) == ir::Kind_reg::Y && ir::kind(x->index) == ir::Kind_reg::X) {
        return {instr_line("LDY", indexed_data_addr(*x))};
      }
    }
  }
  // An `IndexedData` destination is an `abs,X` or `abs,Y` store from A,
  // which the IndexedStore lowering writes as `STA $XXXX,X`.
  if (const auto* x = asdl::as<ir::IndexedData>(dst); x != nullptr && is_reg_a(src)) {
    return {instr_line("STA", indexed_data_addr(*x))};
  }
  if (const auto* i = asdl::as<ir::Imm>(src); i != nullptr && is_memory_operand(dst)) {
    check_byte("immediate", i->value);
    return concat({instr_line("LDA", "#$" + hex2(i->value))}, memop_store(dst));
  }
  if (is_memory_operand(src) && is_reg_a(dst)) {
    return memop_load(src);
  }
  // A `Data` or `ZP` byte loads X or Y directly with `LDX` or `LDY`.
  // The soft-stack and indirect operands can't, since `LDX` and `LDY`
  // have no indirect-Y mode.
  if (is_data_or_zp(src)) {
    if (const auto* d = asdl::as<ir::Reg>(dst)) {
      if (ir::kind(d->reg) == ir::Kind_reg::X) {
        return {instr_line("LDX", abs_addr(src))};
      }
      if (ir::kind(d->reg) == ir::Kind_reg::Y) {
        return {instr_line("LDY", abs_addr(src))};
      }
    }
  }
  // `STX` and `STY` have zero-page and absolute forms, so X and Y are
  // stored to a `Data` or `ZP` byte directly.
  if (const auto* s = asdl::as<ir::Reg>(src); s != nullptr && is_data_or_zp(dst)) {
    if (ir::kind(s->reg) == ir::Kind_reg::X) {
      return {instr_line("STX", abs_addr(dst))};
    }
    if (ir::kind(s->reg) == ir::Kind_reg::Y) {
      return {instr_line("STY", abs_addr(dst))};
    }
  }
  // `STX` and `STY` have no indirect-Y mode, so X or Y goes to a soft-stack
  // or indirect byte through A.
  if (const auto* s = asdl::as<ir::Reg>(src);
      s != nullptr && ir::kind(s->reg) != ir::Kind_reg::A && is_memory_operand(dst)) {
    const char* transfer = ir::kind(s->reg) == ir::Kind_reg::X ? "TXA" : "TYA";
    return concat({instr_line(transfer)}, memop_store(dst));
  }
  if (is_reg_a(src) && is_memory_operand(dst)) {
    return memop_store(dst);
  }
  if (is_memory_operand(src) && is_memory_operand(dst)) {
    return concat(memop_load(src), memop_store(dst));
  }
  value_error("cannot emit Mov(src=" + repr(src) + ", dst=" + repr(dst) + ")");
}

// Returns an `ADC`, `SBC`, `AND`, `ORA`, `EOR`, or `CMP` of `src` into A.
// The opcode gives the operation and the source the addressing mode.
Lines Emitter::acc_arith_src(std::string_view opcode, const ir::Type_operand& src) const {
  switch (ir::kind(src)) {
    case ir::Kind_operand::Imm: {
      const asdl::Int v = std::get<ir::Imm>(src).value;
      check_byte("immediate", v);
      return {instr_line(opcode, "#$" + hex2(v))};
    }
    case ir::Kind_operand::ImmLabelLow:
    case ir::Kind_operand::ImmLabelHigh:
      return {instr_line(opcode, imm_label_text(src))};
    case ir::Kind_operand::Stack:
    case ir::Kind_operand::Frame: {
      Lines out{load_y(operand_offset(src))};
      out.push_back(instr_line(opcode, indirect_addr(src)));
      return out;
    }
    case ir::Kind_operand::Data:
    case ir::Kind_operand::ZP:
      return {instr_line(opcode, abs_addr(src))};
    case ir::Kind_operand::IndexedData:
      // `ADC abs,X` and `ADC abs,Y` are 6502 opcodes ($7D, $79), and so
      // are those of SBC, AND, ORA, EOR, and CMP. `adc_commute` makes
      // one when the other value is in A and the operand is an element
      // of a static array.
      return {instr_line(opcode, indexed_data_addr(std::get<ir::IndexedData>(src)))};
    case ir::Kind_operand::IndirectZp: {
      // A byte through a zero-page pointer, which `adc_commute` makes
      // the operand when the other value is in A.
      const auto& z = std::get<ir::IndirectZp>(src);
      Lines out{load_y(z.offset)};
      out.push_back(instr_line(opcode, zpy_operand(z.address)));
      return out;
    }
    case ir::Kind_operand::IndirectZpY:
      return {instr_line(opcode, zpy_operand(std::get<ir::IndirectZpY>(src).address))};
    default:
      value_error("unsupported " + std::string(opcode) + " source: " + repr(src));
  }
}

// Returns an `ASL`, `LSR`, `ROL`, or `ROR` of `dst`. The 6502 has
// accumulator, absolute, and zero-page modes for these, and no
// indirect-Y mode:
//
//   * `Reg(A)` is the opcode alone, a byte. dasm reads `ASL A` as a
//     shift of the memory at a symbol named `A`.
//   * a `Data` or `ZP` byte is `ASL name+off` or `ASL $XX`, which the
//     runtime helpers use on HARGS and colored locals use in zero page.
//   * an `IndexedData` indexed by X is `ASL name+off,X`, an element of an
//     array shifted in place.
//   * a `Stack` or `Frame` byte goes through A, `LDY #off; LDA (PTR),Y;
//     <op>; LDY #off; STA (PTR),Y`. The carry is unchanged by the `LDY`,
//     the `LDA`, and the `STA`, so a chain of these over several bytes
//     still carries from one byte's `ASL` into the next byte's `ROL`.
Lines Emitter::acc_shift(std::string_view opcode, std::string_view op_name,
                         const ir::Type_operand& dst) const {
  if (is_reg_a(dst)) {
    return {instr_line(opcode)};
  }
  if (is_data_or_zp(dst)) {
    return {instr_line(opcode, abs_addr(dst))};
  }
  if (is_x_indexed(dst)) {
    return {instr_line(opcode, indexed_data_addr(std::get<ir::IndexedData>(dst)))};
  }
  if (asdl::isa<ir::Stack, ir::Frame>(dst)) {
    const asdl::Int off = operand_offset(dst);
    Lines out{load_y(off), instr_line("LDA", indirect_addr(dst)), instr_line(opcode)};
    out.push_back(load_y(off));
    out.push_back(instr_line("STA", indirect_addr(dst)));
    return out;
  }
  value_error(std::string(op_name) +
              " dst must be Reg(A), Data, ZP, IndexedData indexed by X, Stack, or Frame, got " +
              repr(dst));
}

// Returns an `INC` or `DEC` (`opcode`) of `dst`, or `x_op` or `y_op` for
// an index register. An `IndexedData` indexed by X is `INC name+off,X`,
// which dasm assembles as zp,X when the address is in zero page. A
// soft-stack operand has no form, since there is no indirect-Y `INC`.
Lines Emitter::step(std::string_view opcode, std::string_view x_op, std::string_view y_op,
                    std::string_view name, const ir::Type_operand& dst) const {
  if (is_reg(dst, ir::Kind_reg::X)) {
    return {instr_line(x_op)};
  }
  if (is_reg(dst, ir::Kind_reg::Y)) {
    return {instr_line(y_op)};
  }
  if (is_data_or_zp(dst)) {
    return {instr_line(opcode, abs_addr(dst))};
  }
  if (is_x_indexed(dst)) {
    return {instr_line(opcode, indexed_data_addr(std::get<ir::IndexedData>(dst)))};
  }
  value_error(std::string(name) +
              " dst must be Reg(X), Reg(Y), Data, ZP, or IndexedData indexed by X, got " +
              repr(dst));
}

// Returns `CMP`, `CPX`, or `CPY` by the register on the left, with the
// right side's addressing mode. `CPX` and `CPY` have immediate and
// absolute modes and no indirect-Y, so a soft-stack operand is compared
// only with A.
Lines Emitter::compare(const ir::Type_operand& left, const ir::Type_operand& right) const {
  const auto* reg = asdl::as<ir::Reg>(left);
  if (reg == nullptr) {
    value_error("Compare left must be a register, got " + repr(left));
  }
  std::string opcode;
  switch (ir::kind(reg->reg)) {
    case ir::Kind_reg::A:
      opcode = "CMP";
      break;
    case ir::Kind_reg::X:
      opcode = "CPX";
      break;
    case ir::Kind_reg::Y:
      opcode = "CPY";
      break;
  }
  const bool cmp = opcode == "CMP";
  switch (ir::kind(right)) {
    case ir::Kind_operand::Imm: {
      const asdl::Int v = std::get<ir::Imm>(right).value;
      check_byte("immediate", v);
      return {instr_line(opcode, "#$" + hex2(v))};
    }
    case ir::Kind_operand::ImmLabelLow:
    case ir::Kind_operand::ImmLabelHigh:
      return {instr_line(opcode, imm_label_text(right))};
    case ir::Kind_operand::Data:
    case ir::Kind_operand::ZP:
      return {instr_line(opcode, abs_addr(right))};
    case ir::Kind_operand::IndexedData:
      if (!cmp) {
        value_error("Compare with left=" + repr(left) +
                    " can't use an IndexedData right operand: CPX/CPY have no absolute,X or "
                    "absolute,Y addressing modes");
      }
      return {instr_line(opcode, indexed_data_addr(std::get<ir::IndexedData>(right)))};
    case ir::Kind_operand::Stack:
    case ir::Kind_operand::Frame: {
      if (!cmp) {
        value_error("Compare with left=" + repr(left) +
                    " requires Imm, Data, or ZP right (CPX/CPY have no indirect-Y addressing "
                    "mode); got " +
                    repr(right));
      }
      Lines out{load_y(operand_offset(right))};
      out.push_back(instr_line(opcode, indirect_addr(right)));
      return out;
    }
    case ir::Kind_operand::Indirect: {
      // `CMP (DPTR),Y`, which the volatile-void-read rewrite uses to read
      // memory for its side effect and keep A.
      if (!cmp) {
        value_error("Compare with left=" + repr(left) +
                    " can't use an Indirect right operand: CPX/CPY have no indirect-Y "
                    "addressing mode");
      }
      Lines out{load_y(std::get<ir::Indirect>(right).offset)};
      out.push_back(instr_line(opcode, "(DPTR),Y"));
      return out;
    }
    case ir::Kind_operand::IndirectY:
      if (!cmp) {
        value_error("Compare with left=" + repr(left) + " can't use an IndirectY right operand");
      }
      return {instr_line(opcode, "(DPTR),Y")};
    case ir::Kind_operand::IndirectZp: {
      if (!cmp) {
        value_error("Compare with left=" + repr(left) + " can't use an IndirectZp right operand");
      }
      const auto& z = std::get<ir::IndirectZp>(right);
      Lines out{load_y(z.offset)};
      out.push_back(instr_line(opcode, zpy_operand(z.address)));
      return out;
    }
    case ir::Kind_operand::IndirectZpY:
      if (!cmp) {
        value_error("Compare with left=" + repr(left) +
                    " can't use an IndirectZpY right operand");
      }
      return {instr_line(opcode, zpy_operand(std::get<ir::IndirectZpY>(right).address))};
    default:
      value_error("cannot emit Compare(left=" + repr(left) + ", right=" + repr(right) + ")");
  }
}

// Returns a copy of `op` with its byte offset `k` higher, for the high
// byte of a 2-byte destination of `load_address`.
ir::Type_operand shift_offset(const ir::Type_operand& op, asdl::Int k) {
  switch (ir::kind(op)) {
    case ir::Kind_operand::Frame:
      return ir::Frame{.offset = std::get<ir::Frame>(op).offset + k};
    case ir::Kind_operand::Stack:
      return ir::Stack{.offset = std::get<ir::Stack>(op).offset + k};
    case ir::Kind_operand::Data: {
      const auto& d = std::get<ir::Data>(op);
      return ir::Data{.name = d.name, .offset = d.offset + k};
    }
    case ir::Kind_operand::ZP: {
      const auto& z = std::get<ir::ZP>(op);
      return ir::ZP{.address = z.address, .offset = z.offset + k};
    }
    default:
      type_error("can't shift offset on operand " + repr(op));
  }
}

// Returns `LoadAddress(src, dst)`, which writes the 2-byte address of
// `src` into the 2-byte memory operand `dst`. A `Data` source is a
// static object whose address the linker knows, loaded as the halves
// `#<(name+off)` and `#>(name+off)`. A `Frame` source is a local or a
// parameter at `FP+off`, computed as the 16-bit add `CLC; LDA FP; ADC
// #off; STA lo; LDA FP+1; ADC #0; STA hi`, whose carry reaches the high
// byte.
Lines Emitter::load_address(const ir::Type_operand& src, const ir::Type_operand& dst) const {
  if (!is_memory_operand(dst)) {
    type_error("LoadAddress dst must be a memory operand, got " + repr(dst));
  }
  const ir::Type_operand dst_hi = shift_offset(dst, 1);
  const ir::Type_operand reg_a = ir::Reg{.reg = ir::A{}};
  if (const auto* d = asdl::as<ir::Data>(src)) {
    Lines out = mov(ir::ImmLabelLow{.name = d->name, .offset = d->offset}, reg_a);
    out = concat(std::move(out), memop_store(dst));
    out = concat(std::move(out), mov(ir::ImmLabelHigh{.name = d->name, .offset = d->offset}, reg_a));
    return concat(std::move(out), memop_store(dst_hi));
  }
  if (const auto* f = asdl::as<ir::Frame>(src)) {
    check_byte("LoadAddress offset", f->offset);
    Lines out{instr_line("CLC"), instr_line("LDA", kFp), instr_line("ADC", "#$" + hex2(f->offset))};
    out = concat(std::move(out), memop_store(dst));
    out.push_back(instr_line("LDA", std::string(kFp) + "+1"));
    out.push_back(instr_line("ADC", "#$00"));
    return concat(std::move(out), memop_store(dst_hi));
  }
  type_error("LoadAddress src must be Data or Frame (resolved Pseudo); got " + repr(src));
}

// Returns a dispatch on A through a table of addresses minus one: the
// value doubled is the index, the target's address minus one goes on
// the stack high byte first, and `RTS` pulls it and adds one.
Lines jump_table(const ir::JumpTable& t) {
  const std::string at =
      t.base == 0 ? t.table : t.table + "-" + pyrepr::integer(2 * t.base);
  Lines out{
      instr_line("ASL"),
      instr_line("TA" + t.index),
      instr_line("LDA", at + "+1," + t.index),
      instr_line("PHA"),
      instr_line("LDA", at + "," + t.index),
      instr_line("PHA"),
      instr_line("RTS"),
      t.table + ":",
  };
  for (const auto& target : t.targets) {
    out.push_back(instr_line("DC.W", target + "-1"));
  }
  return out;
}

Lines Emitter::instruction(const ir::Type_instruction& instr) const {
  switch (ir::kind(instr)) {
    case ir::Kind_instruction::Mov: {
      const auto& m = std::get<ir::Mov>(instr);
      return mov(m.src, m.dst);
    }
    case ir::Kind_instruction::Return:
      // The bare `RTS`. Any PHA and PLA around it, the SSP and FP
      // teardown, and the restores of callee-saved bytes are atoms of
      // their own, laid down by `asm_to_asm2`.
      return {instr_line("RTS")};
    case ir::Kind_instruction::Comment:
      return {comment_line(std::get<ir::Comment>(instr).text)};
    case ir::Kind_instruction::Blank:
      // A blank line between the prologue, the body, and the epilogue.
      // `function` writes one for a run of them.
      return {""};
    case ir::Kind_instruction::LoadAddress: {
      const auto& l = std::get<ir::LoadAddress>(instr);
      return load_address(l.src, l.dst);
    }
    case ir::Kind_instruction::Add: {
      // `ADC`, after the `ClearCarry` that the code before it has.
      const auto& a = std::get<ir::Add>(instr);
      check_dst_is_a(a.dst, "Add");
      return acc_arith_src("ADC", a.src);
    }
    case ir::Kind_instruction::Sub: {
      // `SBC`, after a `SetCarry`, since SBC subtracts one more when the
      // carry is clear.
      const auto& s = std::get<ir::Sub>(instr);
      check_dst_is_a(s.dst, "Sub");
      return acc_arith_src("SBC", s.src);
    }
    case ir::Kind_instruction::ClearCarry:
      return {instr_line("CLC")};
    case ir::Kind_instruction::SetCarry:
      return {instr_line("SEC")};
    case ir::Kind_instruction::Inc:
      return step("INC", "INX", "INY", "Inc", std::get<ir::Inc>(instr).dst);
    case ir::Kind_instruction::Dec:
      return step("DEC", "DEX", "DEY", "Dec", std::get<ir::Dec>(instr).dst);
    case ir::Kind_instruction::Push: {
      const auto& src = std::get<ir::Push>(instr).src;
      if (!is_reg_a(src)) {
        value_error("Push src must be Reg(A), got " + repr(src));
      }
      return {instr_line("PHA")};
    }
    case ir::Kind_instruction::Pop: {
      const auto& dst = std::get<ir::Pop>(instr).dst;
      if (!is_reg_a(dst)) {
        value_error("Pop dst must be Reg(A), got " + repr(dst));
      }
      return {instr_line("PLA")};
    }
    case ir::Kind_instruction::PushStatus:
      return {instr_line("PHP")};
    case ir::Kind_instruction::PullStatus:
      return {instr_line("PLP")};
    case ir::Kind_instruction::SetDecimal:
      return {instr_line("SED")};
    case ir::Kind_instruction::ClearDecimal:
      return {instr_line("CLD")};
    case ir::Kind_instruction::Xor: {
      // EOR is `A = A ^ <imm or mem>`. One source is A and the other
      // gives the addressing mode, in either order.
      const auto& x = std::get<ir::Xor>(instr);
      check_dst_is_a(x.dst, "Xor");
      if (is_reg_a(x.src1)) {
        return acc_arith_src("EOR", x.src2);
      }
      if (is_reg_a(x.src2)) {
        return acc_arith_src("EOR", x.src1);
      }
      value_error("Xor srcs must include Reg(A); got src1=" + repr(x.src1) +
                  ", src2=" + repr(x.src2));
    }
    case ir::Kind_instruction::And: {
      const auto& a = std::get<ir::And>(instr);
      check_dst_is_a(a.dst, "And");
      return acc_arith_src("AND", a.src);
    }
    case ir::Kind_instruction::Or: {
      const auto& o = std::get<ir::Or>(instr);
      check_dst_is_a(o.dst, "Or");
      return acc_arith_src("ORA", o.src);
    }
    case ir::Kind_instruction::ArithmeticShiftLeft:
      return acc_shift("ASL", "ArithmeticShiftLeft",
                       std::get<ir::ArithmeticShiftLeft>(instr).dst);
    case ir::Kind_instruction::LogicalShiftRight:
      return acc_shift("LSR", "LogicalShiftRight", std::get<ir::LogicalShiftRight>(instr).dst);
    case ir::Kind_instruction::RotateLeft:
      return acc_shift("ROL", "RotateLeft", std::get<ir::RotateLeft>(instr).dst);
    case ir::Kind_instruction::RotateRight:
      return acc_shift("ROR", "RotateRight", std::get<ir::RotateRight>(instr).dst);
    case ir::Kind_instruction::Call:
      return {instr_line("JSR", std::get<ir::Call>(instr).name)};
    case ir::Kind_instruction::Jump:
      return {instr_line("JMP", std::get<ir::Jump>(instr).target)};
    case ir::Kind_instruction::JumpTable:
      return jump_table(std::get<ir::JumpTable>(instr));
    case ir::Kind_instruction::Branch: {
      const auto& b = std::get<ir::Branch>(instr);
      return {instr_line(std::string("B") + cond_suffix(b.cond), b.target)};
    }
    case ir::Kind_instruction::Label:
      return {std::get<ir::Label>(instr).name + ":"};
    case ir::Kind_instruction::Compare: {
      const auto& c = std::get<ir::Compare>(instr);
      return compare(c.left, c.right);
    }
    case ir::Kind_instruction::BitTest: {
      // `BIT src`, which the NMOS 6502 has only with zero-page and
      // absolute addressing. It sets N to bit 7 of `src`, V to bit 6, and
      // Z from `A & src`, and leaves A alone.
      const auto& src = std::get<ir::BitTest>(instr).src;
      if (!is_data_or_zp(src)) {
        value_error(
            "BitTest src must be Data or ZP (NMOS 6502 has no `BIT #imm` or indirect-Y BIT); "
            "got " +
            repr(src));
      }
      return {instr_line("BIT", abs_addr(src))};
    }
  }
  type_error("unexpected instruction");
}

// Returns the names of the `Data` and `IndexedData` operands of
// `instrs`, each a field of its instruction.
passes::NameSet operand_names(const std::vector<ir::Type_instruction>& instrs) {
  passes::NameSet names;
  auto add = [&](const ir::Type_operand& op) {
    if (const auto* d = asdl::as<ir::Data>(op)) {
      names.insert(d->name);
    } else if (const auto* x = asdl::as<ir::IndexedData>(op)) {
      names.insert(x->name);
    }
  };
  for (const auto& instr : instrs) {
    switch (ir::kind(instr)) {
      case ir::Kind_instruction::Mov:
        add(std::get<ir::Mov>(instr).src);
        add(std::get<ir::Mov>(instr).dst);
        break;
      case ir::Kind_instruction::Add:
        add(std::get<ir::Add>(instr).src);
        add(std::get<ir::Add>(instr).dst);
        break;
      case ir::Kind_instruction::Sub:
        add(std::get<ir::Sub>(instr).src);
        add(std::get<ir::Sub>(instr).dst);
        break;
      case ir::Kind_instruction::Inc:
        add(std::get<ir::Inc>(instr).dst);
        break;
      case ir::Kind_instruction::Dec:
        add(std::get<ir::Dec>(instr).dst);
        break;
      case ir::Kind_instruction::Push:
        add(std::get<ir::Push>(instr).src);
        break;
      case ir::Kind_instruction::Pop:
        add(std::get<ir::Pop>(instr).dst);
        break;
      case ir::Kind_instruction::Xor:
        add(std::get<ir::Xor>(instr).src1);
        add(std::get<ir::Xor>(instr).src2);
        add(std::get<ir::Xor>(instr).dst);
        break;
      case ir::Kind_instruction::And:
        add(std::get<ir::And>(instr).src);
        add(std::get<ir::And>(instr).dst);
        break;
      case ir::Kind_instruction::Or:
        add(std::get<ir::Or>(instr).src);
        add(std::get<ir::Or>(instr).dst);
        break;
      case ir::Kind_instruction::ArithmeticShiftLeft:
        add(std::get<ir::ArithmeticShiftLeft>(instr).dst);
        break;
      case ir::Kind_instruction::LogicalShiftRight:
        add(std::get<ir::LogicalShiftRight>(instr).dst);
        break;
      case ir::Kind_instruction::RotateLeft:
        add(std::get<ir::RotateLeft>(instr).dst);
        break;
      case ir::Kind_instruction::RotateRight:
        add(std::get<ir::RotateRight>(instr).dst);
        break;
      case ir::Kind_instruction::Compare:
        add(std::get<ir::Compare>(instr).left);
        add(std::get<ir::Compare>(instr).right);
        break;
      case ir::Kind_instruction::BitTest:
        add(std::get<ir::BitTest>(instr).src);
        break;
      case ir::Kind_instruction::LoadAddress:
        add(std::get<ir::LoadAddress>(instr).src);
        add(std::get<ir::LoadAddress>(instr).dst);
        break;
      default:
        break;
    }
  }
  return names;
}

// Returns a function's lines: its label in column 1, `SUBROUTINE` at the
// opcode column, a blank line, and its instructions. A run of blank
// lines is written once, since the prologue's last line and the
// epilogue's first are blank and meet when the body is empty.
Lines Emitter::function(const ir::Function& fn) {
  current_ = fn.name;
  function_names_ = operand_names(fn.instructions);
  Lines lines{fn.name + ":", instr_line("SUBROUTINE")};
  if (!fn.instructions.empty()) {
    lines.emplace_back();
    for (const auto& instr : fn.instructions) {
      for (auto& line : instruction(instr)) {
        if (line.empty() && !lines.empty() && lines.back().empty()) {
          continue;
        }
        lines.push_back(std::move(line));
      }
    }
  }
  current_.reset();
  function_names_.clear();
  return lines;
}

void check_word(const std::string& label, asdl::Int v) {
  if (!(-32768 <= v && v <= 65535)) {
    value_error(label + " " + pyrepr::integer(v) + " out of range for 16-bit (-32768..65535)");
  }
}

void check_dword(const std::string& label, asdl::Int v) {
  const asdl::Int lo = -(asdl::Int(1) << 31);
  const asdl::Int hi = (asdl::Int(1) << 32) - 1;
  if (!(lo <= v && v <= hi)) {
    value_error(label + " " + pyrepr::integer(v) +
                " out of range for 32-bit (-2147483648..4294967295)");
  }
}

void check_qword(const std::string& label, asdl::Int v) {
  const asdl::Int lo = -(asdl::Int(1) << 63);
  const asdl::Int hi = (asdl::Int(1) << 64) - 1;
  if (!(lo <= v && v <= hi)) {
    value_error(label + " " + pyrepr::integer(v) +
                " out of range for 64-bit (-9223372036854775808..18446744073709551615)");
  }
}

asdl::Int mask(asdl::Int v, int bits) { return v & ((asdl::Int(1) << bits) - 1); }

// Returns a static object's lines: its label, then a directive for each
// initializer item, in the order of its bytes. The item's kind gives
// the width of its cell. A `FloatInit` or `DoubleInit` holds the IEEE
// 754 bit pattern, which `fp_arith` computed from the source, so it is
// split into 32-bit halves as an integer is.
Lines Emitter::static_variable(const ir::StaticVariable& sv) const {
  if (sv.init.empty()) {
    value_error("static variable " + pyrepr::string(sv.name) + " has no init");
  }
  const std::string label = "init for " + pyrepr::string(sv.name);
  Lines lines{sv.name + ":"};
  for (const auto& item : sv.init) {
    switch (ir::kind(item)) {
      case ir::Kind_static_init::CharInit: {
        // A 1-byte cell, for the character types, which takes a signed
        // value from -128 or an unsigned one up to 255, since the bits
        // are the same once masked.
        const asdl::Int v = std::get<ir::CharInit>(item).value;
        if (!(-128 <= v && v <= 0xFF)) {
          value_error(label + " " + pyrepr::integer(v) + " out of range for 1-byte (-128..255)");
        }
        lines.push_back(instr_line("dc.b", "$" + hex2(mask(v, 8))));
        break;
      }
      case ir::Kind_static_init::IntInit: {
        // A 2-byte cell, masked to 16 bits so a negative value is its
        // two's complement (-1 is $FFFF).
        const asdl::Int v = std::get<ir::IntInit>(item).value;
        check_word(label, v);
        lines.push_back(instr_line("dc.w", "$" + pyrepr::hex(mask(v, 16), 4)));
        break;
      }
      case ir::Kind_static_init::LongInit: {
        const asdl::Int v = std::get<ir::LongInit>(item).value;
        check_dword(label, v);
        lines.push_back(instr_line("dc.l", "$" + pyrepr::hex(mask(v, 32), 8)));
        break;
      }
      case ir::Kind_static_init::LongLongInit: {
        // An 8-byte cell as two `dc.l` halves, the low one first.
        const asdl::Int v = std::get<ir::LongLongInit>(item).value;
        check_qword(label, v);
        const asdl::Int masked = mask(v, 64);
        lines.push_back(instr_line("dc.l", "$" + pyrepr::hex(mask(masked, 32), 8)));
        lines.push_back(instr_line("dc.l", "$" + pyrepr::hex(mask(masked >> 32, 32), 8)));
        break;
      }
      case ir::Kind_static_init::FloatInit:
        lines.push_back(
            instr_line("dc.l", "$" + pyrepr::hex(mask(std::get<ir::FloatInit>(item).bits, 32), 8)));
        break;
      case ir::Kind_static_init::DoubleInit: {
        const asdl::Int b = std::get<ir::DoubleInit>(item).bits;
        lines.push_back(instr_line("dc.l", "$" + pyrepr::hex(mask(b, 32), 8)));
        lines.push_back(instr_line("dc.l", "$" + pyrepr::hex(mask(b >> 32, 32), 8)));
        break;
      }
      case ir::Kind_static_init::AddressInit: {
        // The two bytes of another static's address, which the linker
        // resolves.
        const auto& a = std::get<ir::AddressInit>(item);
        lines.push_back(
            instr_line("dc.w", a.offset == 0 ? a.name : a.name + "+" + pyrepr::integer(a.offset)));
        break;
      }
      case ir::Kind_static_init::AddressByteInit: {
        // One byte of an address, in a table split into planes of bytes.
        const auto& a = std::get<ir::AddressByteInit>(item);
        const std::string operand =
            a.offset == 0 ? a.name : "(" + a.name + pyrepr::signed_integer(a.offset) + ")";
        lines.push_back(instr_line("dc.b", (a.high ? ">" : "<") + operand));
        break;
      }
      case ir::Kind_static_init::StringInit: {
        // `bytes` cells: the string's bytes, then zeros. They are written
        // as hexadecimal bytes, 16 to a line, where dasm's string form
        // would need care with NULs, unprintable bytes, and quotes.
        const auto& s = std::get<ir::StringInit>(item);
        const std::vector<std::uint32_t> chars = pyrepr::code_points(s.str);
        const asdl::Int n = s.bytes;
        if (n < static_cast<asdl::Int>(static_cast<long long>(chars.size()))) {
          value_error("StringInit for " + pyrepr::string(sv.name) + ": bytes=" +
                      pyrepr::integer(n) + " less than string length " +
                      std::to_string(chars.size()));
        }
        std::vector<int> bytes;
        bytes.reserve(static_cast<std::size_t>(static_cast<long long>(n)));
        for (const std::uint32_t c : chars) {
          bytes.push_back(static_cast<int>(c & 0xFF));
        }
        while (static_cast<asdl::Int>(static_cast<long long>(bytes.size())) < n) {
          bytes.push_back(0);
        }
        for (std::size_t start = 0; start < bytes.size(); start += 16) {
          std::string row;
          for (std::size_t k = start; k < bytes.size() && k < start + 16; ++k) {
            if (k != start) {
              row += ", ";
            }
            row += '$';
            row += hex2(bytes[k]);
          }
          lines.push_back(instr_line("dc.b", row));
        }
        break;
      }
      case ir::Kind_static_init::ZeroInit: {
        // `ds.b` reserves zero bytes, so `n` zeros take one line.
        const asdl::Int n = std::get<ir::ZeroInit>(item).bytes;
        if (n <= 0) {
          value_error("ZeroInit byte count must be positive, got " + pyrepr::integer(n));
        }
        lines.push_back(instr_line("ds.b", pyrepr::integer(n)));
        break;
      }
    }
  }
  return lines;
}

// Returns the `EQU` lines of the symbols, sorted by address and then by
// name, so that the zero-page slots come first and the spilled ones
// after.
Lines equ_block(const OrderedAddrs& syms) {
  std::vector<std::pair<asdl::Int, std::string>> items;
  items.reserve(syms.size());
  for (const auto& [name, addr] : syms) {
    items.emplace_back(addr, name);
  }
  std::sort(items.begin(), items.end());
  Lines lines;
  for (const auto& [addr, name] : items) {
    lines.push_back(name + "\tEQU\t$" + pyrepr::hex(addr, addr <= 0xFF ? 2 : 4));
  }
  return lines;
}

// The preferred symbols of an emitter outside `emit_program`, which has
// none.
const std::map<std::string, passes::NameSet, std::less<>> kNoPreferred;

}  // namespace

std::vector<std::string> emit_instruction(const asm2_ast::Type_instruction& instr) {
  return Emitter({}, kNoPreferred).instruction(instr);
}

std::vector<std::string> emit_function(const asm2_ast::Function& fn) {
  return Emitter({}, kNoPreferred).function(fn);
}

std::vector<std::string> emit_static_variable(const asm2_ast::StaticVariable& sv) {
  return Emitter({}, kNoPreferred).static_variable(sv);
}

std::string emit_program(const asm2_ast::Program& prog, const passes::RuntimeLayout& layout,
                         const EmitOptions& options) {
  // The map from each zero-page address to the symbols that name it,
  // which writes an `IndirectZp` or `IndirectZpY` base as `(symbol),Y`.
  // The runtime's symbols come first, then the symbols given, in order.
  // The allocator gives functions that are never active together the
  // same bytes, so one address can have several slots, and
  // `pick_symbol` chooses among them for each function. A symbol outside
  // zero page names no `(zp),Y` base, so it is left out.
  std::map<asdl::Int, std::vector<std::string>> addr_to_syms;
  const std::pair<const char*, asdl::Int> runtime[] = {
      {"SSP", layout.ssp}, {"FP", layout.fp}, {"HARGS", layout.hargs}, {"DPTR", layout.dptr}};
  for (const auto& [name, addr] : runtime) {
    addr_to_syms[addr] = {name};
  }
  for (const auto& [name, addr] : options.zp_slot_symbols) {
    if (0 <= addr && addr <= 0xFF && name != "SSP" && name != "FP" && name != "HARGS" &&
        name != "DPTR") {
      addr_to_syms[addr].push_back(name);
    }
  }
  Emitter emitter(std::move(addr_to_syms), options.preferred_symbols);
  Lines joined;
  if (!options.zp_slot_symbols.empty()) {
    joined = equ_block(options.zp_slot_symbols);
    joined.emplace_back();
  }
  if (!options.link_metadata_lines.empty()) {
    joined.insert(joined.end(), options.link_metadata_lines.begin(),
                  options.link_metadata_lines.end());
    joined.emplace_back();
  }
  bool first = true;
  for (const auto& tl : prog.top_level) {
    Lines chunk = asdl::isa<ir::Function>(tl)
                      ? emitter.function(std::get<ir::Function>(tl))
                      : emitter.static_variable(std::get<ir::StaticVariable>(tl));
    if (!first) {
      joined.emplace_back();
    }
    first = false;
    joined.insert(joined.end(), std::make_move_iterator(chunk.begin()),
                  std::make_move_iterator(chunk.end()));
  }
  // The `LDY #imm` lines that repeat or step what Y holds are dropped or
  // shortened (passes/y_peephole.py).
  joined = passes::apply_y_peephole(joined);
  std::string text;
  for (std::size_t i = 0; i < joined.size(); ++i) {
    if (i != 0) {
      text += '\n';
    }
    text += joined[i];
  }
  text += '\n';
  return text;
}

}  // namespace c6502::asm_emit
