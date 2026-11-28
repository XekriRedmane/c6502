// Translates a TAC program into the asm IR.
//
// The asm IR is 1:1 with 6502 opcodes. Every node maps to one
// instruction, with the documented exceptions of `Ret`,
// `FunctionPrologue`, and `AllocateStack`. The 6502 is an 8-bit machine,
// so the IR has no width tagging and every operand is one byte.
//
// `tac_to_asm` is therefore the home of all multi-byte lowering. For each
// TAC instruction whose operands are wider than 1 byte (by the symbol
// table), the translator emits a sequence of byte-level asm instructions,
// typically one pass per byte with the 6502's carry flag threading
// between them for arithmetic. Multi-byte operands are addressed through
// the `offset` field of `Pseudo`, `Stack`, `Frame`, and `Data`:
// `Pseudo(name, 0)` is the low byte of `name` and `Pseudo(name, k)` the
// (k+1)-th byte, and an `Imm` constant splits into its bytes by shift
// and mask (`byte_at`).
//
// The Translator holds two counters, so the labels of the inline
// lowerings (comparisons, `!`, sign extension) and the cycle-break
// temporaries of the zp_abi argument writes are unique across the whole
// program. The order of the lowering is therefore part of its output.
//
// The mapping, with the per-operation detail in `translate_binary` and
// `translate_instruction`:
//
//   Program, Function       -> one asm item per TAC item, with each
//                              function's instructions flat-mapped.
//   StaticVariable          -> the same object, with each initializer
//                              rewrapped one to one.
//   Ret(val)                -> `val` staged in A (and X for the high byte
//                              of a 2-byte return, a pointer included),
//                              or in HARGS for wider returns, then the
//                              exit.
//   Copy(src, dst)          -> one Mov per byte.
//   SignExtend(src, dst)    -> each source byte copied to the matching
//                              byte of dst. The last LDA's N flag is the
//                              sign byte's, and a BMI chooses $00 or $FF
//                              for each remaining high byte.
//   ZeroExtend(src, dst)    -> each source byte copied, then `LDA #$00;
//                              STA` into each remaining high byte.
//   Truncate(src, dst)      -> the low `size_of(dst)` bytes of src copied.
//                              Memory is little-endian, so offset 0 is the
//                              low byte and the high bytes are dropped.
//   Unary(op, src, dst)     -> src into A, the operation on A, A into dst,
//                              per byte for a wider operand. A multi-byte
//                              negate is `0 - x`: SEC, then `LDA #0; SBC
//                              x_k; STA` per byte.
//   Binary(Add)             -> `LDA; CLC; ADC; STA` per byte, with no CLC
//                              after the low byte, since each ADC's carry
//                              goes into the next (LDA changes only N and
//                              Z).
//   Binary(Subtract)        -> the same with SEC and SBC, whose borrow
//                              threads through the bytes in the same way.
//   Binary(BitwiseAnd/Or/Xor) -> the byte operation on each pair of bytes.
//   Binary(BcdAdd/BcdSubtract) -> one DecimalArith, with each byte of
//                              src1, src2, and dst in its lists, low byte
//                              first. asm_to_asm2 expands it to `SED; CLC
//                              (SEC); LDA/ADC (SBC)/STA` per byte and
//                              `CLD`, and keeping it one instruction until
//                              then keeps every asm pass out of the
//                              decimal-mode region (passes/decimal_arith.py).
//   Binary(Equal/NotEqual)  -> a compare, a branch, and a 0 or 1. Wider
//                              operands are compared from the high byte
//                              down, with a BNE out after each byte but
//                              the lowest, whose CMP's Z is the answer.
//   Binary(orderings)       -> the SBCs with the V correction, then the
//                              corrected sign as the 0 or 1 (`ASL; LDA
//                              #0; ROL`). An unsigned ordering takes C
//                              after the SBCs (`LDA #0; ROL`). `>` and
//                              `<=` swap the operands of `<` and `>=`.
//   Binary(Multiply/Divide/Modulo/LeftShift/RightShift)
//                           -> calls of runtime helpers, which exchange
//                              operands and results through the zero-page
//                              block HARGS, chosen by width (mul8,
//                              udivmod16, asl32, ...) and, for `/`, `%`,
//                              and `>>`, by the signedness of src1
//                              (`is_unsigned_val`). The caller writes the
//                              inputs at HARGS+0..N-1, calls, and reads
//                              the result at a fixed offset later in the
//                              block. The inline forms below are the
//                              exceptions, and a `/` and a `%` of the same
//                              operands share one call
//                              (`plan_divmod_pairs`).
//   FunctionCall            -> the argument bytes written to the callee's
//                              frame (`AllocateStack` and `Stack(k)`) or
//                              its zero-page slots, the call, and the
//                              capture of the return value.
//   Jump, Label             -> the same instruction.
//   JumpIfTrue/JumpIfFalse  -> `LDA cond[0]`, then `ORA cond[k]` for each
//                              higher byte, then BNE or BEQ. The ORs set Z
//                              when every byte is zero, which is the test
//                              of the whole value.
//   Constant(c)             -> `Imm` of the value's bit pattern, which
//                              `byte_at` splits.
//   Var(name)               -> `Pseudo(name, 0)`.
//
// Calling convention
//
// A callee's argument takes `size_of(arg)` consecutive stack bytes, low
// byte at the lower offset. A return value comes back in registers when
// it fits and in HARGS otherwise: a 1-byte integer in A, an `int`, an
// `unsigned int`, or a pointer in A and X, a 4-byte value at
// HARGS+8..11, and an 8-byte value at HARGS+16..23. The FP slots are
// those of the FP arithmetic helpers' outputs, so a function ending in
// `return a + b;` on floating operands needs no copy, and the 4-byte
// integers share the slot with `float`, since `mul32` and `divmod32`
// write their results there too. A return in HARGS skips the epilogue's
// `PHA` / `PLA`, since the SSP and FP arithmetic doesn't touch HARGS.
// HARGS is caller-saved, so a caller must capture a value returned there
// right after the `JSR`, before any other helper call. The callee's
// epilogue moves SSP back to the caller's value before the call, so
// there's no cleanup per call. The runtime helpers' calls don't follow
// this convention. They are bare `Call`s with no `AllocateStack`, and
// their operands go through HARGS.
//
// The runtime helpers' layouts
//
// HARGS is 32 bytes at $04-$23 in the classic layout
// (passes/runtime_layout.py places it, and sizes it to what the
// program's helpers need when a fixed zero-page object takes those
// bytes). The inputs sit at the low offsets and survive the call, and
// the outputs follow them. The signed and unsigned divides follow C99
// §6.5.5.6: `sdivmod*` truncates toward zero and gives the remainder the
// dividend's sign, and `udivmod*` divides unsigned values.
//
//   mul8       A:+0, B:+1               -> the low byte of A*B:+2, since
//                                          the product wraps to the type
//   udivmod8/  num:+0, den:+1           -> quot:+2, rem:+3
//    sdivmod8
//   asl8/asr8/ val:+0, count:+1         -> result:+2
//    lsr8
//   mul16      A:+0..+1, B:+2..+3       -> the low half of A*B:+4..+5
//   udivmod16/ num:+0..+1, den:+2..+3   -> quot:+4..+5, rem:+6..+7
//    sdivmod16
//   asl16/...  val:+0..+1, count:+2     -> result:+3..+4 (a shift of 16
//                                          or more is undefined, so the
//                                          count's high byte is dropped)
//   mul32      A:+0..+3, B:+4..+7       -> the low half:+8..+11
//   udivmod32/ num:+0..+3, den:+4..+7   -> quot:+8..+11, rem:+12..+15
//    sdivmod32
//   asl32/...  val:+0..+3, count:+4     -> result:+5..+8
//   mul64      A:+0..+7, B:+8..+15      -> the low half:+16..+23
//   udivmod64/ num:+0..+7, den:+8..+15  -> quot:+16..+23, rem:+24..+31
//    sdivmod64
//   asl64/...  val:+0..+7, count:+8     -> result:+9..+16
//   fadd/...   A:+0..+3, B:+4..+7       -> result:+8..+11
//   dadd/...   A:+0..+7, B:+8..+15      -> result:+16..+23
//   flt/fle    as fadd                  -> 0 or 1 at +8
//   dlt/dle    as dadd                  -> 0 or 1 at +16
//   bin16_to_  val:+0..+1               -> five packed BCD digits:+2..+4,
//    bcd                                   ones in the low nibble of +2
//                                          (it shifts a copy of val in
//                                          +5..+6, so val survives)
//
// The conversions between integers and floating types (i2f, u2f, l2f,
// ul2f, ll2f, ull2f, the same into double, f2i to f2ull, d2i to d2ull,
// f2d, and d2f) take their input at HARGS+0 and write the output right
// after it. The repository has no 6502 code for them, and the
// simulator's are hooks (src/sim/runtime.h), whose sizes the
// lowering follows. `>` and `>=` on floating values swap the operands of
// `<` and `<=`, and `==` and `!=` compare the bit patterns, which is
// wrong for the two zeros of IEEE 754 and stays that way until the
// corpus needs it.
//
// Inline forms
//
// A shift by a constant count `8q + r` is inlined when
// `shift_cost::inline_shift_ok` accepts it: `q` byte moves, then `r`
// one-bit ASL / ROL (or LSR / ROR) steps over the bytes that aren't
// known to be zero. A signed right shift is inlined only with `q == 0`,
// since the vacated high bytes would need a sign fill. A count of 1, or
// of a multiple of 8, is always inlined. A 1-byte shift runs in A (`LDA
// src; ASL A ...; STA dst`), which is 3 cycles a step faster than `ASL`
// on a zero-page byte. A left shift by a constant of a value with one
// significant byte (a widened byte, or one masked to a byte,
// passes/significant_bytes.h) is always inlined, as two byte shifts in A
// whatever the count: `(unsigned int)c << 6` is `LDA c; LSR A; LSR A;
// STA hi; LDA c; ROR A; ROR A; ROR A; AND #$C0`. A 1- or 2-byte shift by
// a variable, or by any other constant, is a loop of one-bit shifts with
// the count in X, and a signed right shift of a value whose top byte is
// zero is logical. An unsigned 1-byte `/` or `%` by a constant from 1 to
// 255 is a loop of subtractions with the quotient in X and the remainder
// in A, and a 16-bit `*` of two values with one significant byte is an
// 8-by-8 shift-and-add loop. An `IndexedLoad` or `IndexedSymbolStore`
// with a constant index needs no X, since it reads or writes `name+k`
// with absolute addressing.
//
// The comparisons are lowered inline too.
//
// - `!x` is `LDA x; BEQ true; LDA #0; JMP end; true: LDA #1; end: STA`.
//   The `LDA` already sets Z, so no compare is needed.
// - `==` and `!=` compare only the low bytes either operand can have
//   nonzero (`equality_bytes`), here and in the compare-and-branch
//   lowering. A comparison with 0 whose destination is 1 byte (a
//   conversion to `bool`) ORs the bytes instead (`AND #$7F` on a
//   floating value's high byte first) and, for `!=`, ends `BEQ end; LDA
//   #1; end:`.
// - `<` and `>=` are `LDA src1; SEC; SBC src2; BVC novf; EOR #$80; novf:
//   ASL A; LDA #0; ROL A` (with `EOR #1` after it for `>=`), since bit 7
//   of A is the corrected N on both paths into `novf`. CMP can't be used
//   for a signed ordering because it leaves V alone, and N is wrong when
//   the signed subtraction overflows, which the `BVC novf; EOR #$80` pair
//   corrects. A signed `x < 0` or `x >= 0` is x's sign bit, `LDA
//   x+N-1; ASL; LDA #0; ROL` (and `EOR #1` for `>=`), with no subtract.
// - `>` and `<=` swap the operands of `<` and `>=`. Z isn't reliable
//   after the EOR correction, so testing "not less and not equal" would
//   need a second compare, and the swap costs less.
//
// The casts
//
// SignExtend, ZeroExtend, and Truncate carry no widths. The lowering
// reads the symbol table's widths of src and dst, so the three nodes
// cover every widening and narrowing pair. A sign extension is `BMI
// sx_neg@N; LDA #$00; JMP sx_done@N; sx_neg@N: LDA #$FF; sx_done@N:` after
// the copies, then a store into each high byte, with two labels per use.
// The FP conversions are helper calls chosen by the symbol table's types
// of src and dst.
//
// Sizes and signedness
//
// `Translator::size_of` gives each value's width, and a bit-precise
// type's storage of 3, 5, 6, or 7 bytes (`BitInt(24)` to `BitInt(56)`,
// constants `ConstInt24` to `ConstUInt56`, bit_precise.h) is that many
// bytes, which the per-byte lowerings take as they take any other width.
// A multiply, divide, remainder, or helper shift of one goes to the
// helper of the next of 4 and 8 bytes with each operand extended in
// HARGS, and a return of one comes back in the HARGS slot of that width.
// Signedness matters only for the orderings, right shifts, and the
// conversions between integers and floating types. `is_unsigned_val`
// chooses the ordering and right shift, and the conversions read the
// const class of a constant and the symbol table's type of a variable.
//
// The static initializers
//
// The asm IR has five kinds of initializer for numbers: `CharInit`,
// `IntInit`, `LongInit`, `LongLongInit`, `FloatInit`, and `DoubleInit`,
// with no signedness, since the 6502 doesn't care about it at the byte
// level. `UCharInit` becomes `CharInit`, `UIntInit` `IntInit`, and so on,
// with the value unchanged, and `asm_emit` checks each cell's range. The
// floating ones keep their bit patterns.
//
// The output is correct but redundant, since every intermediate value
// goes through a slot of its own. The TAC optimizer and the asm optimizer
// remove most of that.
#include "tac_to_asm.h"

#include <algorithm>
#include <utility>
#include <variant>

#include "asdl/runtime.h"
#include "bit_precise.h"
#include "errors.h"
#include "passes/in_place_member_update.h"
#include "passes/optimization/split_tables.h"
#include "passes/optimization/var_visit.h"
#include "passes/operand_rewrite.h"
#include "passes/replace_pseudoregisters.h"
#include "passes/significant_bytes.h"
#include "passes/zp_slot_naming.h"
#include "tac_to_asm_translator.h"

namespace c6502::tac_to_asm {

using ::c6502::asdl::as;
using ::c6502::asdl::isa;

namespace {

// The empty tables that stand for a keyword argument of None.
const passes::Abi kNoAbi;
const passes::Cycles kNoCycles;
const passes::Bindings kNoBindings;
const PinnedCopies kNoPinnedCopies;
const SlotAliases kNoSlotAliases;
const OrderedMap<std::string, std::vector<std::string>> kNoFunctionBindings;
const passes::SymbolTable kNoSymbols;

// Returns `t` without its `Const` and `Volatile` qualifiers.
const c99::Type_data_type& strip_qualifiers(const c99::Type_data_type* t) {
  while (true) {
    if (const auto* c = as<c99::Const>(*t)) {
      t = &*c->referenced_type;
    } else if (const auto* v = as<c99::Volatile>(*t)) {
      t = &*v->referenced_type;
    } else {
      return *t;
    }
  }
}

// Returns the symbol of `name`, or null when the table is null or has
// none.
const passes::Symbol* symbol_of(const passes::SymbolTable* symbols, std::string_view name) {
  return symbols == nullptr ? nullptr : symbols->find(name);
}

// Returns the asm form of a TAC static initializer. The unsigned
// classes collapse onto the asm class of the same width, since the asm
// side has no signedness, and the value passes through unchanged.
ir::Type_static_init to_asm_static_init(const tac::Type_static_init& init) {
  using K = tac::Kind_static_init;
  switch (tac::kind(init)) {
    case K::CharInit:
      return ir::CharInit{std::get<tac::CharInit>(init).value};
    case K::UCharInit:
      return ir::CharInit{std::get<tac::UCharInit>(init).value};
    case K::IntInit:
      return ir::IntInit{std::get<tac::IntInit>(init).value};
    case K::LongInit:
      return ir::LongInit{std::get<tac::LongInit>(init).value};
    case K::LongLongInit:
      return ir::LongLongInit{std::get<tac::LongLongInit>(init).value};
    case K::UIntInit:
      return ir::IntInit{std::get<tac::UIntInit>(init).value};
    case K::ULongInit:
      return ir::LongInit{std::get<tac::ULongInit>(init).value};
    case K::ULongLongInit:
      return ir::LongLongInit{std::get<tac::ULongLongInit>(init).value};
    case K::FloatInit:
      return ir::FloatInit{std::get<tac::FloatInit>(init).bits};
    case K::DoubleInit:
      return ir::DoubleInit{std::get<tac::DoubleInit>(init).bits};
    case K::AddressInit: {
      const auto& a = std::get<tac::AddressInit>(init);
      return ir::AddressInit{a.name, a.offset};
    }
    case K::AddressByteInit: {
      const auto& a = std::get<tac::AddressByteInit>(init);
      return ir::AddressByteInit{a.name, a.offset, a.high};
    }
    case K::StringInit: {
      const auto& s = std::get<tac::StringInit>(init);
      return ir::StringInit{s.str, s.bytes};
    }
    case K::ZeroInit:
      return ir::ZeroInit{std::get<tac::ZeroInit>(init).bytes};
  }
  throw InternalError("unexpected static_init");
}

std::vector<ir::Type_static_init> to_asm_static_inits(const std::vector<tac::Type_static_init>& in) {
  std::vector<ir::Type_static_init> out;
  out.reserve(in.size());
  for (const auto& i : in) {
    out.push_back(to_asm_static_init(i));
  }
  return out;
}

// What a renamed pseudo becomes: another pseudo's name, or the operands
// of its bytes (a variable kept in a callee's parameter slots, or a
// member computed in place).
using Rename = std::variant<std::string, std::vector<ir::Type_operand>>;
using Renames = std::map<std::string, Rename, std::less<>>;

// Returns `op` with its offset set to `offset`, as Python's
// `dataclasses.replace(op, offset=offset)` gives it.
ir::Type_operand with_offset(ir::Type_operand op, asdl::Int offset) {
  switch (ir::kind(op)) {
    case ir::Kind_operand::Pseudo:
      std::get<ir::Pseudo>(op).offset = offset;
      break;
    case ir::Kind_operand::Data:
      std::get<ir::Data>(op).offset = offset;
      break;
    case ir::Kind_operand::Stack:
      std::get<ir::Stack>(op).offset = offset;
      break;
    case ir::Kind_operand::Frame:
      std::get<ir::Frame>(op).offset = offset;
      break;
    default:
      throw InternalError("rename_pseudos: an operand without an offset");
  }
  return op;
}

// Returns `instrs` with each `Pseudo(name, k)` whose name is in
// `renames` replaced, by `Pseudo(new_name, k)` for a name or by byte `k`
// of a list of operands. It drops a `Mov` that the replacement turns
// into a copy of a byte to itself, the argument write of a call that
// passes the value where it already is.
Instrs rename_pseudos(Instrs instrs, const Renames& renames) {
  Instrs out;
  out.reserve(instrs.size());
  for (auto& instr : instrs) {
    passes::for_each_operand_mut(instr, [&](ir::Type_operand& op) {
      const auto* p = as<ir::Pseudo>(op);
      if (p == nullptr) {
        return;
      }
      const auto it = renames.find(p->name);
      if (it == renames.end()) {
        return;
      }
      const asdl::Int offset = p->offset;
      if (const auto* name = std::get_if<std::string>(&it->second)) {
        op = ir::Pseudo{*name, offset};
        return;
      }
      const auto& list = std::get<std::vector<ir::Type_operand>>(it->second);
      if (offset < static_cast<asdl::Int>(list.size())) {
        op = list[static_cast<std::size_t>(offset)];
      } else {
        op = with_offset(list[0], offset);
      }
    });
    if (const auto* mov = as<ir::Mov>(instr)) {
      if (mov->src == mov->dst && isa<ir::Data>(mov->dst) && !mov->is_volatile) {
        continue;
      }
    }
    out.push_back(std::move(instr));
  }
  return out;
}

// Returns the elements `[start, start + n)` of `v`, clipped to its
// size, as a Python slice gives them.
template <class T>
std::vector<T> slice(const std::vector<T>& v, std::size_t start, std::size_t n) {
  if (start >= v.size()) {
    return {};
  }
  const std::size_t end = std::min(v.size(), start + n);
  return {v.begin() + static_cast<std::ptrdiff_t>(start),
          v.begin() + static_cast<std::ptrdiff_t>(end)};
}

}  // namespace

// --- Operands and the free helpers ---

ir::Type_operand reg_a() { return ir::Reg{ir::A{}}; }
ir::Type_operand reg_x() { return ir::Reg{ir::X{}}; }
ir::Type_operand reg_y() { return ir::Reg{ir::Y{}}; }

ir::Type_operand byte_at(const ir::Type_operand& op, asdl::Int k) {
  switch (ir::kind(op)) {
    case ir::Kind_operand::Imm:
      return ir::Imm{(std::get<ir::Imm>(op).value >> static_cast<int>(8 * k)) & 0xFF};
    case ir::Kind_operand::Pseudo: {
      const auto& p = std::get<ir::Pseudo>(op);
      return ir::Pseudo{p.name, p.offset + k};
    }
    case ir::Kind_operand::Stack:
      return ir::Stack{std::get<ir::Stack>(op).offset + k};
    case ir::Kind_operand::Frame:
      return ir::Frame{std::get<ir::Frame>(op).offset + k};
    case ir::Kind_operand::Data: {
      const auto& d = std::get<ir::Data>(op);
      return ir::Data{d.name, d.offset + k};
    }
    default:
      throw InternalError("can't address byte " + asdl::int_to_string(k) + " of " +
                          asdl::dump(op));
  }
}

ir::Type_operand hargs(asdl::Int k) { return ir::Data{kHargs, k}; }

ir::Type_operand translate_val(const tac::Type_val& val) {
  if (const auto* var = as<tac::Var>(val)) {
    return ir::Pseudo{var->name, 0};
  }
  const auto& c = std::get<tac::Constant>(val).const_;
  switch (tac::kind(c)) {
    case tac::Kind_const::ConstFloat:
      return ir::Imm{std::get<tac::ConstFloat>(c).bits};
    case tac::Kind_const::ConstDouble:
      return ir::Imm{std::get<tac::ConstDouble>(c).bits};
    case tac::Kind_const::ConstChar:
      // A signed byte, whose value may be negative. A 1-byte lowering
      // may write it without `byte_at`, so give its bit pattern here.
      return ir::Imm{std::get<tac::ConstChar>(c).value & 0xFF};
    default:
      return ir::Imm{*bit_precise::raw_value(c)};
  }
}

bool imm_zero(const ir::Type_operand& op) {
  const auto* imm = as<ir::Imm>(op);
  return imm != nullptr && imm->value == 0;
}

Instrs carry_to_value(Instrs set_c, bool invert) {
  set_c.push_back(ir::Mov{.src = ir::Imm{0}, .dst = reg_a()});
  set_c.push_back(ir::RotateLeft{.dst = reg_a()});
  if (invert) {
    set_c.push_back(ir::Xor{.src1 = reg_a(), .src2 = ir::Imm{1}, .dst = reg_a()});
  }
  return set_c;
}

Instrs load_y_index(const ir::Type_operand& idx_op) {
  if (isa<ir::Imm>(idx_op)) {
    return {ir::Mov{.src = idx_op, .dst = reg_y()}};
  }
  return {ir::Mov{.src = idx_op, .dst = reg_a()}, ir::Mov{.src = reg_a(), .dst = reg_y()}};
}

Instrs indirect_read(const ir::Type_operand& dst, int n, bool is_volatile) {
  Instrs out;
  for (int k = 0; k < n; ++k) {
    out.push_back(ir::Mov{.src = ir::Indirect{k}, .dst = reg_a(), .is_volatile = is_volatile});
    out.push_back(ir::Mov{.src = reg_a(), .dst = byte_at(dst, k)});
  }
  return out;
}

Instrs indirect_write(const ir::Type_operand& src, int n, bool is_volatile) {
  Instrs out;
  for (int k = 0; k < n; ++k) {
    out.push_back(ir::Mov{.src = byte_at(src, k), .dst = reg_a()});
    out.push_back(ir::Mov{.src = reg_a(), .dst = ir::Indirect{k}, .is_volatile = is_volatile});
  }
  return out;
}

Instrs translate_helper_call(const std::vector<HelperInput>& inputs, const std::string& helper,
                             int output_offset, int output_size, const ir::Type_operand& dst_op) {
  Instrs out;
  int slot = 0;
  for (const auto& [src_op, sz] : inputs) {
    for (int k = 0; k < sz; ++k) {
      out.push_back(ir::Mov{.src = byte_at(src_op, k), .dst = reg_a()});
      out.push_back(ir::Mov{.src = reg_a(), .dst = hargs(slot)});
      ++slot;
    }
  }
  out.push_back(ir::Call{.name = helper});
  for (int k = 0; k < output_size; ++k) {
    out.push_back(ir::Mov{.src = hargs(output_offset + k), .dst = reg_a()});
    out.push_back(ir::Mov{.src = reg_a(), .dst = byte_at(dst_op, k)});
  }
  return out;
}

// The 2-byte slot, HARGS+0..1, is for a 2-byte return without a symbol
// table (`reg_return_bytes`), since an `int` and a pointer come back in
// A and X. It is apart from the helper outputs at HARGS+2..7, so
// capturing such a return doesn't conflict with the layout of `mul8`,
// `divmod8`, and the 8-bit shifts. The 4-byte values sit in the FP
// arithmetic helpers' output slot at HARGS+8..11, which is `mul32`'s and
// `divmod32`'s output slot too, so a function ending `return a OP b;`
// for those types skips the copy. The 8-byte values sit at HARGS+16..23,
// the double helpers' output slot. A 3-byte integer (a bit-precise type
// of 17 to 24 bits) comes back in the 4-byte slot, and one of 5 to 7
// bytes in the 8-byte slot.
int ret_hargs_offset(int size) {
  switch (size) {
    case 2:
      return 0;
    case 3:
    case 4:
      return 8;
    case 5:
    case 6:
    case 7:
    case 8:
      return 16;
    default:
      throw InternalError("no HARGS return slot for " + std::to_string(size) + " bytes");
  }
}

void not_ported(std::string_view what) {
  throw InternalError("tac_to_asm: " + std::string(what) + " isn't ported yet");
}

// --- The Translator ---

Translator::Translator(passes::SymbolTable* symbols_, const passes::TypeTable* types_,
                       const passes::AsmContext& ctx_, const Options& options)
    : symbols(symbols_),
      types(types_),
      ctx(ctx_),
      bare_exit(options.bare_exit),
      abi(options.abi != nullptr ? *options.abi : kNoAbi),
      recursion_saves(options.recursion_saves != nullptr ? *options.recursion_saves : kNoCycles),
      bindings(options.bindings != nullptr ? *options.bindings : kNoBindings),
      pinned_copies(options.pinned_copies != nullptr ? *options.pinned_copies : kNoPinnedCopies),
      slot_aliases(options.slot_aliases != nullptr ? *options.slot_aliases : kNoSlotAliases) {}

int Translator::size_of(const tac::Type_val& val) const {
  if (const auto* var = as<tac::Var>(val)) {
    return static_cast<int>(passes::size_of_name(var->name, symbols, types));
  }
  const auto& c = std::get<tac::Constant>(val).const_;
  using K = tac::Kind_const;
  switch (tac::kind(c)) {
    case K::ConstChar:
    case K::ConstUChar:
      return 1;
    case K::ConstInt:
    case K::ConstUInt:
      return 2;
    case K::ConstLong:
    case K::ConstULong:
    case K::ConstFloat:
      return 4;
    case K::ConstLongLong:
    case K::ConstULongLong:
    case K::ConstDouble:
      return 8;
    default:
      return bit_precise::odd_const_layout(c)->bytes;
  }
}

bool Translator::is_pointer_val(const tac::Type_val& val) const {
  // Constants never qualify, since TAC's constants have no pointer
  // class. The ordering operators read this to compare pointers, which
  // are 16-bit unsigned addresses, as unsigned values.
  const auto* var = as<tac::Var>(val);
  if (var == nullptr) {
    return false;
  }
  const passes::Symbol* sym = symbol_of(symbols, var->name);
  return sym != nullptr && isa<c99::Pointer>(strip_qualifiers(&sym->type));
}

bool Translator::is_fp_val(const tac::Type_val& val) const {
  // A negate reads this to flip the sign bit of a floating value, which
  // IEEE 754 keeps in bit 7 of the top byte, where an integer gets a
  // two's-complement negate.
  if (const auto* c = as<tac::Constant>(val)) {
    return isa<tac::ConstFloat, tac::ConstDouble>(c->const_);
  }
  const passes::Symbol* sym = symbol_of(symbols, std::get<tac::Var>(val).name);
  return sym != nullptr && isa<c99::Float, c99::Double>(strip_qualifiers(&sym->type));
}

bool Translator::is_unsigned_val(const tac::Type_val& val) const {
  // The orderings read this to choose the BCC/BCS lowering without the V
  // correction, and `>>` to choose the logical shift helpers `lsr*`
  // over the arithmetic `asr*`.
  if (const auto* c = as<tac::Constant>(val)) {
    if (isa<tac::ConstUChar, tac::ConstUInt, tac::ConstULong, tac::ConstULongLong>(c->const_)) {
      return true;
    }
    const auto odd = bit_precise::odd_const_layout(c->const_);
    return odd.has_value() && !odd->is_signed;
  }
  const passes::Symbol* sym = symbol_of(symbols, std::get<tac::Var>(val).name);
  if (sym == nullptr) {
    return false;
  }
  const auto& t = strip_qualifiers(&sym->type);
  if (const auto* b = as<c99::BitInt>(t)) {
    return !b->is_signed;
  }
  return isa<c99::UInt, c99::ULong, c99::ULongLong, c99::Char, c99::UChar, c99::Pointer>(t);
}

bool Translator::same_slot(const ir::Type_operand& src, const std::string& sym) const {
  // A bound variable is still a `Pseudo` here, and becomes its slot's
  // `Data` operand once the function is translated.
  const std::string* name = nullptr;
  if (const auto* p = as<ir::Pseudo>(src)) {
    const std::vector<std::string>* slots =
        current_bindings == nullptr ? nullptr : current_bindings->find(p->name);
    if (slots == nullptr || p->offset >= static_cast<asdl::Int>(slots->size())) {
      return false;
    }
    name = &(*slots)[static_cast<std::size_t>(p->offset)];
  } else if (const auto* d = as<ir::Data>(src); d != nullptr && d->offset == 0) {
    name = &d->name;
  } else {
    return false;
  }
  if (*name == sym) {
    return true;
  }
  const std::string* canon = slot_aliases.find(sym);
  if (canon == nullptr) {
    return false;
  }
  const std::string* other = slot_aliases.find(*name);
  return other != nullptr && *other == *canon;
}

std::string Translator::make_label(std::string_view prefix) {
  // The leading `.` makes this a local label of dasm, scoped to the
  // enclosing SUBROUTINE. The `@` separator, which no C identifier
  // holds, keeps these apart from every name in the source.
  std::string name = "." + std::string(prefix) + "@" + std::to_string(label_counter);
  ++label_counter;
  return name;
}

const c99::Type_data_type* Translator::fn_return_type(
    const std::optional<std::string>& fn_name) const {
  // Both the callee's return and the caller's capture read this, so
  // they agree on the convention.
  if (!fn_name) {
    return nullptr;
  }
  const passes::Symbol* sym = symbol_of(symbols, *fn_name);
  if (sym == nullptr) {
    return nullptr;
  }
  const auto* ft = as<c99::FunType>(sym->type);
  return ft == nullptr ? nullptr : &*ft->ret;
}

bool Translator::returns_carry(const std::optional<std::string>& fn_name) const {
  if (!fn_name) {
    return false;
  }
  const passes::Symbol* sym = symbol_of(symbols, *fn_name);
  if (sym == nullptr) {
    return false;
  }
  const auto* fun = std::get_if<passes::FunAttr>(&sym->attrs);
  return fun != nullptr && fun->carry_return;
}

bool Translator::returns_carry_struct(const std::optional<std::string>& fn_name) const {
  if (!returns_carry(fn_name)) {
    return false;
  }
  const c99::Type_data_type* ret = fn_return_type(fn_name);
  return ret != nullptr && isa<c99::Structure>(strip_qualifiers(ret));
}

const c99::Type_data_type* Translator::type_of_val(const tac::Type_val& val) const {
  // The return convention at an indirect call site, where there's no
  // callee name to read a FunType from, is keyed by this.
  const auto* var = as<tac::Var>(val);
  if (var == nullptr) {
    return nullptr;
  }
  const passes::Symbol* sym = symbol_of(symbols, var->name);
  return sym == nullptr ? nullptr : &sym->type;
}

// Captures a register return into `dst_op`: the low byte from A, and
// the high byte of a 2-byte return from X.
//
// Under `-O` (`bare_exit`) the temporary that holds the result colors to
// a zero-page byte, so the high byte is stored with a direct `STX`, with
// no staging through A. The unoptimized pipeline puts the temporary in a
// Frame slot, which `STX` can't address (there's no `STX (zp),Y`), so the
// high byte goes through A (`TXA; STA`), with the low byte stored first
// to free A. Keeping the `TXA` out of the optimized path keeps the
// peepholes that follow X (`via_a_store_fold`, `x_save_slot_load`) from
// seeing two values in X.
Instrs Translator::capture_reg_return(const ir::Type_operand& dst_op, int reg_bytes) const {
  Instrs out{ir::Mov{.src = reg_a(), .dst = byte_at(dst_op, 0)}};
  if (reg_bytes >= 2) {
    if (bare_exit) {
      out.push_back(ir::Mov{.src = reg_x(), .dst = byte_at(dst_op, 1)});
    } else {
      out.push_back(ir::Mov{.src = reg_x(), .dst = reg_a()});
      out.push_back(ir::Mov{.src = reg_a(), .dst = byte_at(dst_op, 1)});
    }
  }
  return out;
}

// Returns the number of return bytes carried in registers, the
// type-driven rule of `return_reg_bytes`, or for synthetic IR that has
// no symbol table the older rule by size, which puts a 1-byte return in
// A and anything wider in HARGS.
int Translator::reg_return_bytes(const c99::Type_data_type* ret_type, int size) const {
  if (ret_type != nullptr) {
    return return_reg_bytes(*ret_type);
  }
  return size == 1 ? 1 : 0;
}

// Returns the number of bytes a return of type `ret_type` passes in the
// 6502's registers: 1 (the low byte in A) for a 1-byte integer, and 2
// (low in A, high in X) for `int`, `unsigned int`, and a pointer. A
// wider type, void, and anything else come back in HARGS. A pointer
// comes back as an `unsigned int` does. The rule against a positional
// `register` parameter that's a pointer is about passing arguments, and
// the caller stores the result in memory anyway, as a `(zp),Y` base
// takes it.
int Translator::return_reg_bytes(const c99::Type_data_type& ret_type) {
  const auto& t = strip_qualifiers(&ret_type);
  if (isa<c99::Char, c99::SChar, c99::UChar>(t)) {
    return 1;
  }
  if (isa<c99::Int, c99::UInt, c99::Pointer>(t)) {
    return 2;
  }
  return 0;
}

// Returns the compound `Ret`, whose frame sizes `replace_pseudoregisters`
// fills in later, or under `-O` (`bare_exit`) a bare `Return` that
// prologue synthesis picks up after register allocation. `save_a` and
// `save_x` say which return bytes are in A and X, so the epilogue keeps
// them across the frame teardown, and `save_c` marks a return in the
// carry flag.
ir::Type_instruction Translator::exit(bool save_a, bool save_x, bool save_c) const {
  if (bare_exit) {
    return ir::Return{.save_a = save_a, .save_x = save_x, .save_c = save_c};
  }
  return ir::Ret{.arg_bytes = 0,
                 .local_bytes = 0,
                 .save_a = save_a,
                 .save_x = save_x,
                 .save_c = save_c};
}

// Byte `k` of the object `name`, for an indexed access with a constant
// index. An object with static storage is `Data(name, k)`. Any other
// name is an automatic object, and it's `Pseudo(name, k)`, the operand a
// plain read of the local gets. That's either a volatile scalar, which
// `c99_to_tac` accesses through `IndexedLoad` and `IndexedSymbolStore`
// so the TAC passes keep each access, or a struct, union, or array whose
// member `direct_member_access` reads or writes in place.
ir::Type_operand Translator::named_byte(const std::string& name, asdl::Int k) const {
  if (const passes::Symbol* sym = symbol_of(symbols, name)) {
    if (!std::holds_alternative<passes::StaticAttr>(sym->attrs)) {
      return ir::Pseudo{name, k};
    }
  }
  return ir::Data{name, k};
}

// --- The driver ---

ir::Program Translator::translate_program(const tac::Program& prog) {
  // Each TAC `Function` lowers to an asm `Function`, and each
  // `StaticVariable` becomes an asm `StaticVariable` with its
  // initializers rewrapped one to one. The class of an initializer
  // alone decides the cell size at emission.
  ir::Program out;
  passes::NameSet emitted;
  for (const auto& tl : prog.top_level) {
    const auto* sv = as<tac::StaticVariable>(tl);
    if (sv == nullptr) {
      out.top_level.push_back(translate_function(std::get<tac::Function>(tl)));
      continue;
    }
    // The objects that hold the byte planes of a table of row addresses
    // (passes/optimization/row_address_tables.py), which several tables
    // may share.
    const std::vector<std::string>* planes = nullptr;
    if (const passes::Symbol* sym = symbol_of(symbols, sv->name)) {
      if (const auto* st = std::get_if<passes::StaticAttr>(&sym->attrs)) {
        if (st->planes && !st->planes->empty()) {
          planes = &*st->planes;
        }
      }
    }
    if (planes != nullptr) {
      for (const auto& name : *planes) {
        if (emitted.insert(name).second) {
          // The object of one plane, from the bytes its symbol gives.
          const auto& attr = std::get<passes::StaticAttr>(symbols->at(name).attrs);
          const auto& cells = std::get<passes::Initial>(attr.initial_value).value.items;
          out.top_level.push_back(ir::StaticVariable{
              .name = name,
              .is_global = false,
              .init = to_asm_static_inits(passes::split_tables::plane_init({cells}))});
        }
      }
      continue;
    }
    out.top_level.push_back(ir::StaticVariable{
        .name = sv->name, .is_global = sv->is_global, .init = to_asm_static_inits(sv->init)});
  }
  return out;
}

namespace {

// Appends to `out` the entry stub that copies the registers each
// register parameter arrives in (`layout.param_registers`) into the
// parameter's local, adds that local to the symbol table, and returns
// `{parameter: local}`.
//
// The copies from A come first, since a copy from X or Y into a register
// other than its own goes through A (`TXA; TAY`). The regalloc keeps
// each local out of the other parameters' registers
// (`optimizer._entry_register_limits`), so no copy overwrites a register
// another copy still reads.
Renames register_param_locals(Translator& t, const std::vector<std::string>& params,
                              const passes::ZpLayout& layout, Instrs& out) {
  Renames renames;
  std::vector<std::pair<int, ir::Type_instruction>> copies;
  std::size_t flat = 0;
  for (const auto& p : params) {
    const int n = t.size_of(tac::Var{p});
    const auto regs = slice(layout.param_registers, flat, static_cast<std::size_t>(n));
    flat += static_cast<std::size_t>(n);
    if (std::none_of(regs.begin(), regs.end(), [](const auto& r) { return r.has_value(); })) {
      continue;
    }
    const std::string local = passes::register_param_local(p);
    renames.insert_or_assign(p, local);
    if (t.symbols != nullptr) {
      if (const passes::Symbol* sym = t.symbols->find(p)) {
        passes::Symbol copy{.type = sym->type, .attrs = passes::LocalAttr{.register_ = false}};
        t.symbols->insert_or_assign(local, std::move(copy));
      }
    }
    for (std::size_t k = 0; k < regs.size(); ++k) {
      if (!regs[k]) {
        continue;
      }
      const std::string& r = *regs[k];
      const int order = r == "A" ? 0 : r == "X" ? 1 : 2;
      const ir::Type_operand src = order == 0 ? reg_a() : order == 1 ? reg_x() : reg_y();
      copies.emplace_back(order, ir::Mov{.src = src, .dst = ir::Pseudo{local, asdl::Int(k)}});
    }
  }
  // A stable sort by register, A, then X, then Y.
  for (int order = 0; order < 3; ++order) {
    for (auto& [r, m] : copies) {
      if (r == order) {
        out.push_back(std::move(m));
      }
    }
  }
  return renames;
}

// Appends to `out`, for each `zeropage(addr)` parameter in `copied`, the
// copy of its bytes into a local of its own, which the body reads in
// place of the parameter, and returns `{parameter: local}`. Something
// the function calls stores its own argument at those bytes. These
// copies go after the register parameters', since they go through A.
Renames pinned_param_locals(Translator& t, const std::vector<std::string>& params,
                            const passes::ZpLayout& layout, Instrs& out,
                            const passes::NameSet& copied) {
  Renames renames;
  std::size_t flat = 0;
  for (const auto& p : params) {
    const int n = t.size_of(tac::Var{p});
    const auto syms = slice(layout.slot_symbols, flat, static_cast<std::size_t>(n));
    flat += static_cast<std::size_t>(n);
    if (!copied.contains(p)) {
      continue;
    }
    const std::string local = passes::register_param_local(p);
    renames.insert_or_assign(p, local);
    if (t.symbols != nullptr) {
      if (const passes::Symbol* sym = t.symbols->find(p)) {
        passes::Symbol copy{.type = sym->type, .attrs = passes::LocalAttr{.register_ = false}};
        t.symbols->insert_or_assign(local, std::move(copy));
      }
    }
    for (std::size_t k = 0; k < syms.size(); ++k) {
      out.push_back(ir::Mov{.src = ir::Data{syms[k], 0}, .dst = ir::Pseudo{local, asdl::Int(k)}});
    }
  }
  return renames;
}

// Plans the members of static objects that `fn` computes in place
// (passes/in_place_member_update.h), adds the renames that name each
// planned local by the member's bytes to `renames`, and returns the
// indexes of the loads and stores to leave out.
std::set<int> in_place_members(const Translator& t, const tac::Function& fn, Renames& renames) {
  const auto is_local = [&](std::string_view name) {
    const passes::Symbol* sym = symbol_of(t.symbols, name);
    return sym != nullptr && std::holds_alternative<passes::LocalAttr>(sym->attrs);
  };
  const auto size_of = [&](const tac::Type_val& v) { return t.size_of(v); };
  std::set<int> skip;
  for (const auto& up : passes::plan_in_place_updates(fn.instructions, fn.params, is_local, size_of)) {
    const auto& store = std::get<tac::IndexedSymbolStore>(fn.instructions[up.store]);
    const int n = t.size_of(store.src);
    const auto sig = t.significant.find(up.u);
    if (renames.contains(up.obj) || (up.t && renames.contains(*up.t)) || renames.contains(up.u) ||
        isa<ir::Pseudo>(t.named_byte(up.obj, 0)) ||
        (sig != t.significant.end() && sig->second < n) || t.low_byte_only.contains(up.u)) {
      continue;
    }
    std::vector<ir::Type_operand> member;
    for (int k = 0; k < n; ++k) {
      member.push_back(t.named_byte(up.obj, up.base + k));
    }
    renames.insert_or_assign(up.u, member);
    skip.insert(up.store);
    if (up.t) {
      renames.insert_or_assign(*up.t, member);
      skip.insert(*up.load);
    }
  }
  return skip;
}

// Returns whether `op` is a `Pseudo` of a variable whose type is
// volatile. TAC temporaries (`%N`) and the runtime's names (DPTR, HARGS)
// have no volatile type in the symbol table.
bool operand_is_volatile_pseudo(const Translator& t, const ir::Type_operand& op) {
  const auto* p = as<ir::Pseudo>(op);
  if (p == nullptr) {
    return false;
  }
  const passes::Symbol* sym = symbol_of(t.symbols, p->name);
  if (sym == nullptr) {
    return false;
  }
  const c99::Type_data_type* type = &sym->type;
  while (const auto* c = as<c99::Const>(*type)) {
    type = &*c->referenced_type;
  }
  return isa<c99::Volatile>(*type);
}

// Sets the volatile bit of a `Mov` whose source or destination is a
// `Pseudo` of a volatile variable, and leaves every other instruction,
// and a bit already set, alone.
void mark_volatile_var_access(const Translator& t, ir::Type_instruction& instr) {
  auto* mov = as<ir::Mov>(instr);
  if (mov == nullptr || mov->is_volatile) {
    return;
  }
  if (operand_is_volatile_pseudo(t, mov->src) || operand_is_volatile_pseudo(t, mov->dst)) {
    mov->is_volatile = true;
  }
}

// Returns each name's defining instructions in `instrs`.
std::map<std::string, std::vector<const tac::Type_instruction*>, std::less<>> defs_by_name(
    const std::vector<tac::Type_instruction>& instrs) {
  std::map<std::string, std::vector<const tac::Type_instruction*>, std::less<>> out;
  for (const auto& ins : instrs) {
    for (const auto d : passes::recursion_saves::defs_in(ins)) {
      out[std::string(d)].push_back(&ins);
    }
  }
  return out;
}

// Returns `LDA; PHA` for each byte of each saved value, low byte first.
// `saves` holds a name and the value to push it from.
Instrs push_saved(const Translator& t, const std::vector<passes::SavedValue>* saves) {
  Instrs out;
  if (saves == nullptr) {
    return out;
  }
  for (const auto& [n, src] : *saves) {
    const ir::Type_operand op = translate_val(src);
    const int size = t.size_of(tac::Var{n});
    for (int k = 0; k < size; ++k) {
      out.push_back(ir::Mov{.src = byte_at(op, k), .dst = reg_a()});
      out.push_back(ir::Push{.src = reg_a()});
    }
  }
  return out;
}

// Returns `PLA; STA` for each byte of each saved value, the reverse of
// `push_saved`. `saves` is already in the order of the pulls, a name and
// the value to store it in.
Instrs pop_saved(const Translator& t, const std::vector<passes::SavedValue>* saves) {
  Instrs out;
  if (saves == nullptr) {
    return out;
  }
  for (const auto& [n, dst] : *saves) {
    const ir::Type_operand op = translate_val(dst);
    for (int k = t.size_of(tac::Var{n}) - 1; k >= 0; --k) {
      out.push_back(ir::Pop{.dst = reg_a()});
      out.push_back(ir::Mov{.src = reg_a(), .dst = byte_at(op, k)});
    }
  }
  return out;
}

void append(Instrs& out, Instrs more) {
  out.insert(out.end(), std::make_move_iterator(more.begin()), std::make_move_iterator(more.end()));
}

}  // namespace

ir::Function Translator::translate_function(const tac::Function& fn) {
  // The parameter names go through to the asm Function, so the frame
  // layout can place them. A reference to a parameter in the body is a
  // TAC `Var(@<N>.<orig>)`, which lowers to `Pseudo(@<N>.<orig>, 0)` as
  // any other variable does, and `replace_pseudoregisters` sizes each
  // pseudo from the symbol table and resolves `Pseudo(name, k)` to the
  // k-th byte of its slot. A static object is a `Pseudo` here too, which
  // `replace_pseudoregisters` makes `Data(name, k)` by its name.
  const std::optional<std::string> prior_fn = current_fn_name;
  current_fn_name = fn.name;
  const std::string& name = fn.name;
  Instrs out;
  Renames renames;
  // A parameter that arrives in a register is copied at entry into a
  // local of its own (`register_param_local`), which the body reads in
  // place of the parameter. The regalloc keeps the local in the register
  // when it can, and in the function's pool otherwise, so the parameter
  // needs no slot.
  if (const passes::ParamLayout* own = abi.find(name)) {
    if (const auto* zp = std::get_if<passes::ZpLayout>(own)) {
      for (auto& [k, v] : register_param_locals(*this, fn.params, *zp, out)) {
        renames.insert_or_assign(k, std::move(v));
      }
      static const passes::NameSet kNone;
      const passes::NameSet* copied = pinned_copies.find(name);
      for (auto& [k, v] :
           pinned_param_locals(*this, fn.params, *zp, out, copied != nullptr ? *copied : kNone)) {
        renames.insert_or_assign(k, std::move(v));
      }
    }
  }
  current_bindings = bindings.find(name);
  if (current_bindings == nullptr) {
    current_bindings = &kNoFunctionBindings;
  }
  for (const auto& [var, slots] : *current_bindings) {
    std::vector<ir::Type_operand> ops;
    for (const auto& s : slots) {
      ops.push_back(ir::Data{s, 0});
    }
    renames.insert_or_assign(var, std::move(ops));
  }
  std::optional<passes::SavePlan> plan;
  if (const passes::NameSet* members = recursion_saves.find(name)) {
    plan = passes::plan_saves(fn, *members, symbols != nullptr ? *symbols : kNoSymbols);
  }
  const auto size_of_fn = [this](const tac::Type_val& v) { return size_of(v); };
  const auto unsigned_fn = [this](const tac::Type_val& v) { return is_unsigned_val(v); };
  significant = passes::significant_bytes(fn, symbols, size_of_fn, unsigned_fn);
  low_byte_only = passes::low_byte_only(fn, symbols, size_of_fn);
  const std::set<int> in_place = in_place_members(*this, fn, renames);
  const auto& instrs = fn.instructions;
  tac_defs = defs_by_name(instrs);
  tac_uses = passes::count_uses(instrs);
  auto [rots, rotate_skip] = plan_rotations(instrs);
  rotations = std::move(rots);
  std::set<int> exclude = in_place;
  exclude.insert(rotate_skip.begin(), rotate_skip.end());
  if (plan) {
    exclude.insert(plan->skip.begin(), plan->skip.end());
  }
  auto [pairs, pair_skip] = plan_divmod_pairs(instrs, exclude);
  divmod_pairs = std::move(pairs);
  rotate_skip.insert(pair_skip.begin(), pair_skip.end());
  for (int i = 0; i < static_cast<int>(instrs.size()); ++i) {
    prev_tac = i != 0 ? &instrs[i - 1] : nullptr;
    tac_list = &instrs;
    tac_index = i;
    if (plan) {
      append(out, push_saved(*this, plan->push_before.find(i)));
    }
    if (in_place.contains(i) || rotate_skip.contains(i)) {
      // Lowered with another instruction.
    } else if (!plan || !plan->skip.contains(i)) {
      append(out, translate_instruction(instrs[i]));
    }
    if (plan) {
      append(out, pop_saved(*this, plan->pop_after.find(i)));
      append(out, push_saved(*this, plan->push_after.find(i)));
    }
  }
  current_fn_name = prior_fn;
  prev_tac = nullptr;
  if (!renames.empty()) {
    out = rename_pseudos(std::move(out), renames);
  }
  // A Mov whose source or destination is a `Pseudo` of a volatile
  // variable gets the volatile bit. The Movs that the loads and stores
  // through a volatile pointer emit already carry it, and this covers a
  // direct access to a volatile local, parameter, or static through a
  // Copy, Binary, Unary, and so on. `c99_to_tac` reads and writes a named
  // volatile scalar through IndexedLoad and IndexedSymbolStore, so what
  // reaches this is an aggregate or a `register` object.
  for (auto& m : out) {
    mark_volatile_var_access(*this, m);
  }
  return ir::Function{
      .name = name, .is_global = fn.is_global, .params = fn.params, .instructions = std::move(out)};
}

Instrs Translator::translate_instruction(const tac::Type_instruction& instr) {
  using K = tac::Kind_instruction;
  switch (tac::kind(instr)) {
    case K::Ret:
      return translate_ret(std::get<tac::Ret>(instr).val);
    case K::SignExtend: {
      const auto& i = std::get<tac::SignExtend>(instr);
      return translate_sign_extend(i.src, i.dst);
    }
    case K::ZeroExtend: {
      const auto& i = std::get<tac::ZeroExtend>(instr);
      return translate_zero_extend(i.src, i.dst);
    }
    case K::Truncate: {
      // Memory is little-endian, so byte k of a multi-byte value sits at
      // offset+k. A truncation copies the destination's width of low
      // bytes from src and drops the higher ones.
      const auto& i = std::get<tac::Truncate>(instr);
      const ir::Type_operand src_op = translate_val(i.src);
      const ir::Type_operand dst_op = translate_val(i.dst);
      Instrs out;
      const int tgt_w = size_of(i.dst);
      for (int k = 0; k < tgt_w; ++k) {
        out.push_back(ir::Mov{.src = byte_at(src_op, k), .dst = byte_at(dst_op, k)});
      }
      return out;
    }
    case K::IntToFloat: {
      const auto& i = std::get<tac::IntToFloat>(instr);
      return translate_int_to_fp(i.src, i.dst, false);
    }
    case K::IntToDouble: {
      const auto& i = std::get<tac::IntToDouble>(instr);
      return translate_int_to_fp(i.src, i.dst, true);
    }
    case K::FloatToInt: {
      const auto& i = std::get<tac::FloatToInt>(instr);
      return translate_fp_to_int(i.src, i.dst, false);
    }
    case K::DoubleToInt: {
      const auto& i = std::get<tac::DoubleToInt>(instr);
      return translate_fp_to_int(i.src, i.dst, true);
    }
    case K::FloatToDouble: {
      // Widens an IEEE 754 single (4 bytes) to a double (8 bytes)
      // through HARGS, with the layout of `f2d`.
      const auto& i = std::get<tac::FloatToDouble>(instr);
      return translate_helper_call({{translate_val(i.src), 4}}, "f2d", 4, 8, translate_val(i.dst));
    }
    case K::DoubleToFloat: {
      const auto& i = std::get<tac::DoubleToFloat>(instr);
      return translate_helper_call({{translate_val(i.src), 8}}, "d2f", 8, 4, translate_val(i.dst));
    }
    case K::Unary: {
      const auto& i = std::get<tac::Unary>(instr);
      return translate_unary(i.op, i.src, i.dst);
    }
    case K::Binary: {
      const auto& i = std::get<tac::Binary>(instr);
      if (const auto it = rotations.find(tac_index); it != rotations.end()) {
        return rotate_in(i.dst, it->second);
      }
      if (const auto it = divmod_pairs.find(tac_index); it != divmod_pairs.end()) {
        return translate_divmod_pair(i.src1, i.src2, it->second);
      }
      return translate_binary(i.op, i.src1, i.src2, i.dst);
    }
    case K::Copy: {
      const auto& i = std::get<tac::Copy>(instr);
      return translate_copy(i.src, i.dst);
    }
    case K::Jump:
      return {ir::Jump{.target = std::get<tac::Jump>(instr).target}};
    case K::Label:
      return {ir::Label{.name = std::get<tac::Label>(instr).name}};
    case K::JumpIfTrue: {
      const auto& i = std::get<tac::JumpIfTrue>(instr);
      return translate_cond_jump(i.condition, i.target, ir::NE{});
    }
    case K::JumpIfFalse: {
      const auto& i = std::get<tac::JumpIfFalse>(instr);
      return translate_cond_jump(i.condition, i.target, ir::EQ{});
    }
    case K::JumpIfCmp: {
      const auto& i = std::get<tac::JumpIfCmp>(instr);
      return translate_jump_if_cmp(i.op, i.src1, i.src2, i.target);
    }
    case K::JumpIfMasked: {
      const auto& i = std::get<tac::JumpIfMasked>(instr);
      return translate_jump_if_masked(i.val, i.mask, i.jump_when_nonzero, i.target);
    }
    case K::FunctionCall:
      return translate_function_call(std::get<tac::FunctionCall>(instr));
    case K::IndirectCall:
      return translate_indirect_call(std::get<tac::IndirectCall>(instr));
    case K::GetAddress:
      return translate_get_address(std::get<tac::GetAddress>(instr));
    case K::Load:
      return translate_load(std::get<tac::Load>(instr));
    case K::Store:
      return translate_store(std::get<tac::Store>(instr));
    case K::IndexedLoad:
      return translate_indexed_load(std::get<tac::IndexedLoad>(instr));
    case K::IndexedStore:
      return translate_indexed_store(std::get<tac::IndexedStore>(instr));
    case K::IndexedSymbolStore:
      return translate_indexed_symbol_store(std::get<tac::IndexedSymbolStore>(instr));
    case K::IndexedConstLoad:
      return translate_indexed_const_load(std::get<tac::IndexedConstLoad>(instr));
    case K::IndirectIndexedLoad:
      return translate_indirect_indexed_load(std::get<tac::IndirectIndexedLoad>(instr));
    case K::IndirectIndexedStore:
      return translate_indirect_indexed_store(std::get<tac::IndirectIndexedStore>(instr));
    case K::Phi:
      break;
  }
  throw InternalError("unexpected instruction node: " + asdl::dump(instr));
}

// Stages the return value, then exits. By the declared return type, a
// 1-byte integer comes back in A, an `int`, an `unsigned int`, or a
// pointer in A (low) and X (high), a 4-byte value at HARGS+8..11, and an
// 8-byte one at HARGS+16..23. The epilogue keeps A and X across the
// frame teardown (`save_a`, `save_x`). A wider value is left in HARGS,
// which the SSP and FP arithmetic doesn't touch. The FP arithmetic
// helpers write their results to the same slots, so `return a OP b;` on
// floating operands leaves the result in place. HARGS is caller-saved,
// so the caller has to capture a value there right after the `JSR`. A
// void return stages nothing.
Instrs Translator::translate_ret(const std::optional<tac::Type_val>& val) {
  if (!val) {
    return {exit(false, false)};
  }
  const ir::Type_operand src_op = translate_val(*val);
  const int size = size_of(*val);
  if (returns_carry_struct(current_fn_name)) {
    // A struct of a value and a `bool`: the flag is the last byte, and
    // the value the `size - 1` bytes before it. C is set from the flag
    // first, since loading the value with LDA (and TAX) leaves C alone.
    const int n = size - 1;
    Instrs seq{ir::Mov{.src = byte_at(src_op, n), .dst = reg_a()},
               ir::Compare{.left = reg_a(), .right = ir::Imm{1}}};
    if (n == 2) {
      seq.push_back(ir::Mov{.src = byte_at(src_op, 1), .dst = reg_a()});
      seq.push_back(ir::Mov{.src = reg_a(), .dst = reg_x()});
    }
    seq.push_back(ir::Mov{.src = byte_at(src_op, 0), .dst = reg_a()});
    seq.push_back(exit(true, n == 2, true));
    return seq;
  }
  if (returns_carry(current_fn_name)) {
    // The type checker made the value `e != 0`, so it's 0 or 1, and `CMP
    // #1` sets C exactly when it's 1. A carries nothing out, and
    // `carry_const_fold` turns `LDA #k; CMP #1` into `SEC` or `CLC`.
    return {ir::Mov{.src = byte_at(src_op, 0), .dst = reg_a()},
            ir::Compare{.left = reg_a(), .right = ir::Imm{1}}, exit(false, false, true)};
  }
  // The register convention is keyed by the function's declared return
  // type and not by the value's, so a constant null (`return 0;` from a
  // pointer function) and the caller's capture agree. The type checker
  // has converted the value to the return type, so its width matches.
  const int reg_bytes = reg_return_bytes(fn_return_type(current_fn_name), size);
  if (reg_bytes != 0) {
    Instrs seq;
    if (reg_bytes >= 2) {
      // The high byte goes to X through A (`LDA hi; TAX`) first, so the
      // `LDA lo` after it leaves the low byte in A. There's no `LDX
      // (zp),Y`, so a high byte in the frame has to go through A, and
      // the `direct_index_load` peephole makes `LDA M; TAX` an `LDX M`
      // when M can be read with X.
      seq.push_back(ir::Mov{.src = byte_at(src_op, 1), .dst = reg_a()});
      seq.push_back(ir::Mov{.src = reg_a(), .dst = reg_x()});
    }
    seq.push_back(ir::Mov{.src = byte_at(src_op, 0), .dst = reg_a()});
    seq.push_back(exit(true, reg_bytes >= 2));
    return seq;
  }
  // A wider return writes its bytes into the HARGS slot of its width and
  // exits without keeping any register.
  const int out_off = ret_hargs_offset(size);
  Instrs seq;
  for (int k = 0; k < size; ++k) {
    seq.push_back(ir::Mov{.src = byte_at(src_op, k), .dst = reg_a()});
    seq.push_back(ir::Mov{.src = reg_a(), .dst = hargs(out_off + k)});
  }
  seq.push_back(exit(false, false));
  return seq;
}

Instrs Translator::translate_copy(const tac::Type_val& src, const tac::Type_val& dst) const {
  // One Mov per byte. The emitter handles every pair of addressing
  // modes and goes through A from memory to memory.
  const ir::Type_operand src_op = translate_val(src);
  const ir::Type_operand dst_op = translate_val(dst);
  const int size = size_of(src);
  Instrs out;
  for (int k = 0; k < size; ++k) {
    out.push_back(ir::Mov{.src = byte_at(src_op, k), .dst = byte_at(dst_op, k)});
  }
  return out;
}

ir::Program translate_program(const tac::Program& prog, passes::SymbolTable* symbols,
                              const passes::TypeTable* types, const passes::AsmContext& ctx,
                              const Options& options) {
  Translator t(symbols, types, ctx, options);
  return t.translate_program(prog);
}

}  // namespace c6502::tac_to_asm
