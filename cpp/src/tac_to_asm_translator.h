// The `Translator` of tac_to_asm.py and the helpers its lowerings share.
// The lowering is split over four files, in the parts of
// docs/cpp_port.md's step 4:
//
// - src/tac_to_asm.cpp: the driver, the queries every part reads, the
//   returns and copies, and the module's free helpers;
// - src/tac_to_asm_calls.cpp: calls and the ABI, and memory and
//   pointers (part C1);
// - src/tac_to_asm_arith.cpp: arithmetic, the inline shifts, divisions,
//   and products, and the shift and divide helpers (part C2);
// - src/tac_to_asm_compare.cpp: casts, unary operators, conditional
//   jumps, and comparisons (part C3).
//
// The struct declares the lowerings one part calls in another. Each
// part keeps the rest of its helpers in its own file, as free functions
// that take the `Translator`.
#pragma once

#include <map>
#include <optional>
#include <set>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "asm_ast.h"
#include "c99_ast.h"
#include "ordered_map.h"
#include "passes/abi_selection.h"
#include "passes/asm_context.h"
#include "passes/recursion_saves.h"
#include "passes/slot_sharing.h"
#include "passes/type_checking.h"
#include "tac_ast.h"
#include "tac_to_asm.h"

namespace c6502::tac_to_asm {

namespace ir = ::c6502::asm_ast;
namespace tac = ::c6502::tac_ast;
namespace c99 = ::c6502::c99_ast;

using Instrs = std::vector<ir::Type_instruction>;

// --- Operands ---

ir::Type_operand reg_a();
ir::Type_operand reg_x();
ir::Type_operand reg_y();

// Returns the k-th byte (0 = low) of a multi-byte operand. For `Imm`
// it is that byte of the constant, so a negative value folds to its
// two's-complement bytes (-1 gives $FF, $FF), as Python's arithmetic
// `>>` gives them. For `Pseudo`, `Stack`, `Frame`, and `Data` it is the
// operand with its offset bumped by k. Any other operand throws
// `InternalError`, as Python raises `TypeError`.
ir::Type_operand byte_at(const ir::Type_operand& op, asdl::Int k);

// Returns `HARGS+k`, the k-th byte of the runtime helpers' exchange
// block in zero page.
ir::Type_operand hargs(asdl::Int k);

// Returns the asm operand of a TAC value: `Imm` of a constant's bit
// pattern (a `ConstChar`'s value masked to a byte, since it may be
// negative and a 1-byte lowering may write it without `byte_at`), and
// `Pseudo(name, 0)` of a variable.
ir::Type_operand translate_val(const tac::Type_val& val);

// Returns whether `op` is the constant 0.
bool imm_zero(const ir::Type_operand& op);

// Returns `set_c`, which leaves an answer in C, then `LDA #0; ROL A`,
// which makes A that answer as 0 or 1, and `EOR #1` when `invert`.
Instrs carry_to_value(Instrs set_c, bool invert);

// Returns the setting of Y to the index of an indirect-indexed access.
// A constant index is a single `LDY #k`. Anything else goes through A
// (`LDA idx; TAY`), because loading a Frame operand sets Y itself for
// the `(FP),Y` read.
Instrs load_y_index(const ir::Type_operand& idx_op);

// Returns the read of `n` bytes through DPTR (already staged with the
// source address) into `dst`, each through A: `LDY #k; LDA (DPTR),Y;
// STA dst+k`. With `is_volatile` the read of memory carries the bit,
// and the store into `dst`, internal storage, doesn't.
Instrs indirect_read(const ir::Type_operand& dst, int n, bool is_volatile = false);

// Returns the write of `n` bytes from `src` through DPTR (already staged
// with the destination address), each through A. With `is_volatile`
// the write to memory carries the bit.
Instrs indirect_write(const ir::Type_operand& src, int n, bool is_volatile = false);

// One input of a runtime helper: the operand and its number of bytes.
using HelperInput = std::pair<ir::Type_operand, int>;

// Returns the lowering of a TAC operation that a runtime helper does
// through the HARGS block: each input's bytes packed from `HARGS+0`
// upward, the call, and the copy of `output_size` bytes from
// `HARGS+output_offset` into `dst_op`, each byte through A, the only
// register that loads from any operand and stores to a `Data`. The
// inputs survive the call, so the destination may overlap an input.
Instrs translate_helper_call(const std::vector<HelperInput>& inputs, const std::string& helper,
                             int output_offset, int output_size, const ir::Type_operand& dst_op);

// The runtime symbols and the helpers named in more than one part.
inline constexpr char kHargs[] = "HARGS";
inline constexpr char kDptr[] = "DPTR";
inline constexpr char kIcall[] = "icall";

// Returns the HARGS offset of the return slot of a value of `size`
// bytes, 2 to 8 (Python's `_RET_HARGS_OFFSET`).
int ret_hargs_offset(int size);

// --- The plans of a function's lowering ---

// A rotate-in, `(x << 1) | b` with `b` a byte that's 0 or 1
// (`plan_rotations`). `carried` is set when C already holds `b`.
struct Rotation {
  tac::Type_val x;
  tac::Type_val b;
  bool carried = false;
};

// The destinations of a divide and a remainder of the same operands
// that one loop or one helper call computes (`plan_divmod_pairs`).
struct DivmodPair {
  tac::Type_val quot;
  tac::Type_val rem;
};

// The state of one program's lowering. There is one Translator per
// program, so the label counters run over the whole program. Python
// keeps it in a class with private methods, and here every member is
// public, so that the helpers each part keeps in its own file can read
// it.
struct Translator {
  Translator(passes::SymbolTable* symbols, const passes::TypeTable* types,
             const passes::AsmContext& ctx, const Options& options);

  // --- State ---

  // The symbol table, or null. The lowering adds the `<param>.reg`
  // locals to it.
  passes::SymbolTable* symbols;
  const passes::TypeTable* types;
  const passes::AsmContext& ctx;
  bool bare_exit;
  // The tables of `Options`, each an empty one where it was null.
  const passes::Abi& abi;
  const passes::Cycles& recursion_saves;
  const passes::Bindings& bindings;
  const PinnedCopies& pinned_copies;
  const SlotAliases& slot_aliases;

  // The counter of `make_label`, over the whole program.
  int label_counter = 0;
  // The counter of the call sites whose zp_abi argument writes are
  // ordered as a parallel copy, which numbers their cycle-break
  // temporaries.
  int zp_call_site_counter = 0;

  // The function being translated, or none outside one.
  std::optional<std::string> current_fn_name;
  // The bindings of the function being translated, whose variables
  // become their slots' `Data` operands once it's translated.
  const OrderedMap<std::string, std::vector<std::string>>* current_bindings = nullptr;
  // The TAC instruction before the one being translated, for the fused
  // subtract of a compare, or null.
  const tac::Type_instruction* prev_tac = nullptr;
  // The instructions of the function being translated and the index of
  // the one being translated.
  const std::vector<tac::Type_instruction>* tac_list = nullptr;
  int tac_index = -1;
  // Per local of the function being translated, how many low bytes can
  // be nonzero, when fewer than its size (passes/significant_bytes.h).
  std::map<std::string, int, std::less<>> significant;
  // The locals only read through a `Truncate` to one byte.
  passes::NameSet low_byte_only;
  // Each name's defining instructions in the function being translated,
  // in order, as pointers into its instructions (recursion_saves'
  // `defs_in`), and how many times each name is read.
  std::map<std::string, std::vector<const tac::Type_instruction*>, std::less<>> tac_defs;
  OrderedMap<std::string, std::size_t> tac_uses;
  // The rotate-ins and the divide and remainder pairs of the function,
  // by the index of the instruction that lowers each.
  std::map<int, Rotation> rotations;
  std::map<int, DivmodPair> divmod_pairs;

  // --- The queries every part reads (src/tac_to_asm.cpp) ---

  // Returns the byte width of a TAC value: 1 for the character types,
  // 2 for `int`, `unsigned int`, and pointers, 4 for the `long`s and
  // `float`, 8 for the `long long`s and `double`, 3, 5, 6, or 7 for what
  // is left of a bit-precise type, and the layout's size for a struct
  // or union. Constants go by their class and variables by their
  // symbol's type. A variable without a symbol is 1 byte.
  int size_of(const tac::Type_val& val) const;
  // Returns whether `val` is a variable whose type is a pointer.
  bool is_pointer_val(const tac::Type_val& val) const;
  // Returns whether `val` is a `float` or `double` operand.
  bool is_fp_val(const tac::Type_val& val) const;
  // Returns whether `val` is an unsigned integer or a pointer.
  bool is_unsigned_val(const tac::Type_val& val) const;
  // Returns whether `src` is the byte of slot symbol `sym`, under
  // another spelling of the same share.
  bool same_slot(const ir::Type_operand& src, const std::string& sym) const;
  // Returns a new local label `.<prefix>@<N>`, numbered over the whole
  // program.
  std::string make_label(std::string_view prefix);
  // Returns the declared return type of `fn_name`, or none.
  const c99::Type_data_type* fn_return_type(const std::optional<std::string>& fn_name) const;
  // Returns whether `fn_name` returns its `bool` in the carry flag.
  bool returns_carry(const std::optional<std::string>& fn_name) const;
  // Returns whether `fn_name` returns in the carry flag a struct of a
  // value and a `bool`.
  bool returns_carry_struct(const std::optional<std::string>& fn_name) const;
  // Returns the symbol table's type of a variable, or null.
  const c99::Type_data_type* type_of_val(const tac::Type_val& val) const;
  // Returns the capture of a register return into `dst_op`.
  Instrs capture_reg_return(const ir::Type_operand& dst_op, int reg_bytes) const;
  // Returns the number of return bytes that come back in registers.
  int reg_return_bytes(const c99::Type_data_type* ret_type, int size) const;
  static int return_reg_bytes(const c99::Type_data_type& ret_type);
  // Returns the function's exit, a bare `Return` under `bare_exit` and
  // the compound `Ret` otherwise.
  ir::Type_instruction exit(bool save_a, bool save_x, bool save_c = false) const;
  // Returns byte `k` of the object `name` for an indexed access with a
  // constant index.
  ir::Type_operand named_byte(const std::string& name, asdl::Int k) const;

  // --- The driver (src/tac_to_asm.cpp) ---

  ir::Program translate_program(const tac::Program& prog);
  ir::Function translate_function(const tac::Function& fn);
  Instrs translate_instruction(const tac::Type_instruction& instr);
  Instrs translate_ret(const std::optional<tac::Type_val>& val);
  Instrs translate_copy(const tac::Type_val& src, const tac::Type_val& dst) const;

  // --- Part C1, calls and memory (src/tac_to_asm_calls.cpp) ---

  Instrs translate_function_call(const tac::FunctionCall& call);
  Instrs translate_indirect_call(const tac::IndirectCall& call);
  Instrs translate_get_address(const tac::GetAddress& instr);
  Instrs translate_load(const tac::Load& instr);
  Instrs translate_store(const tac::Store& instr);
  Instrs translate_indexed_load(const tac::IndexedLoad& instr);
  Instrs translate_indexed_store(const tac::IndexedStore& instr);
  Instrs translate_indexed_symbol_store(const tac::IndexedSymbolStore& instr);
  Instrs translate_indexed_const_load(const tac::IndexedConstLoad& instr);
  Instrs translate_indirect_indexed_load(const tac::IndirectIndexedLoad& instr);
  Instrs translate_indirect_indexed_store(const tac::IndirectIndexedStore& instr);

  // --- Part C2, arithmetic (src/tac_to_asm_arith.cpp) ---

  // Returns the rotate-ins of the function's instructions and the
  // indices of the instructions they absorb.
  std::pair<std::map<int, Rotation>, std::set<int>> plan_rotations(
      const std::vector<tac::Type_instruction>& instrs);
  Instrs rotate_in(const tac::Type_val& dst, const Rotation& rotation);
  // Returns the divide and remainder pairs of the function's
  // instructions, leaving out those in `exclude`, and the indices of
  // the seconds of the pairs.
  std::pair<std::map<int, DivmodPair>, std::set<int>> plan_divmod_pairs(
      const std::vector<tac::Type_instruction>& instrs, const std::set<int>& exclude);
  Instrs translate_divmod_pair(const tac::Type_val& src1, const tac::Type_val& src2,
                               const DivmodPair& pair);
  Instrs translate_binary(const tac::Type_binary_operator& op, const tac::Type_val& src1,
                          const tac::Type_val& src2, const tac::Type_val& dst);

  // --- Part C3, casts and comparisons (src/tac_to_asm_compare.cpp) ---

  Instrs translate_sign_extend(const tac::Type_val& src, const tac::Type_val& dst);
  Instrs translate_zero_extend(const tac::Type_val& src, const tac::Type_val& dst);
  Instrs translate_int_to_fp(const tac::Type_val& src, const tac::Type_val& dst,
                             bool target_double);
  Instrs translate_fp_to_int(const tac::Type_val& src, const tac::Type_val& dst,
                             bool source_double);
  Instrs translate_unary(const tac::Type_unary_operator& op, const tac::Type_val& src,
                         const tac::Type_val& dst);
  Instrs translate_cond_jump(const tac::Type_val& cond, const std::string& target,
                             const ir::Type_condition& branch_cond);
  Instrs translate_jump_if_cmp(const tac::Type_binary_operator& op, const tac::Type_val& src1,
                               const tac::Type_val& src2, const std::string& target);
  Instrs translate_jump_if_masked(const tac::Type_val& val, asdl::Int mask,
                                  bool jump_when_nonzero, const std::string& target);
  // The comparisons `translate_binary` lowers a `Binary` of `==`, `!=`,
  // or an ordering to.
  int equality_bytes(const tac::Type_val& src1, const tac::Type_val& src2, int size, bool fp);
  Instrs translate_equality(const ir::Type_operand& src1_op, const ir::Type_operand& src2_op,
                            const ir::Type_operand& dst_op, int size,
                            const ir::Type_condition& cond, int dst_size, bool fp = false);
  Instrs translate_signed_ordering(const ir::Type_operand& left_op,
                                   const ir::Type_operand& right_op,
                                   const ir::Type_operand& dst_op, int size,
                                   const ir::Type_condition& cond, int dst_size);
  Instrs translate_unsigned_ordering(const ir::Type_operand& left_op,
                                     const ir::Type_operand& right_op,
                                     const ir::Type_operand& dst_op, int size,
                                     const ir::Type_condition& cond, int dst_size);
  Instrs translate_fp_ordering(const ir::Type_operand& left_op, const ir::Type_operand& right_op,
                               const ir::Type_operand& dst_op, int size, int dst_size, bool lt);
};

// Throws the error a part that isn't ported yet gives for `what`.
[[noreturn]] void not_ported(std::string_view what);

}  // namespace c6502::tac_to_asm
