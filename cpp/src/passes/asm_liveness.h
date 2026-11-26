#pragma once

#include <cstddef>
#include <span>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <vector>

#include "asm_ast.h"

namespace c6502::passes {

using Instrs = std::span<const asm_ast::Type_instruction>;

// The index of each `Label` in a list of instructions, by name, the
// last one where a name repeats. Every walk below finds its jump
// targets here, and builds one when it isn't given one.
class LabelMap {
 public:
  LabelMap() = default;
  explicit LabelMap(Instrs instrs);

  // Returns the index of the label `name`, or -1 when there is none.
  long find(const std::string& name) const;
  bool contains(const std::string& name) const { return find(name) >= 0; }

  // Returns every (name, index) pair, sorted by name.
  std::vector<std::pair<std::string, long>> entries() const;

 private:
  std::unordered_map<std::string, long> index_;
};

LabelMap label_map(Instrs instrs);

bool is_reg_a(const asm_ast::Type_operand& op);
bool is_reg_x(const asm_ast::Type_operand& op);
bool is_reg_y(const asm_ast::Type_operand& op);

// Returns whether the value of A at `instrs[idx]` is dead: every path
// forward writes A before anything reads it. The walk follows jumps and
// branches through `labels`, and treats a `Ret` / `Return` as a read
// when `save_a` is set and as the end of the path otherwise.
bool a_dead_at(Instrs instrs, std::size_t idx);
bool a_dead_at(Instrs instrs, const LabelMap& labels, std::size_t idx);

// Returns whether the value of X at `instrs[idx]` is dead, by the walk
// of `a_dead_at` with X's reads and writes. A `Ret` / `Return` with
// `save_x` reads X (the high byte of a 2-byte register return), and one
// without it ends the path, since the soft-stack epilogue scratches X.
// A tail call reads X when the callee names it in `reg_args`.
bool x_dead_at(Instrs instrs, std::size_t idx);
bool x_dead_at(Instrs instrs, const LabelMap& labels, std::size_t idx);

// Returns whether the value of Y at `instrs[idx]` is dead, as
// `x_dead_at` does for X. Every `Ret` / `Return` ends the path, since
// the soft-stack epilogue scratches Y for its `(FP),Y` reload.
bool y_dead_at(Instrs instrs, std::size_t idx);
bool y_dead_at(Instrs instrs, const LabelMap& labels, std::size_t idx);

// Returns whether `instr` reads A, as a source or by a read-modify-write
// of `dst = A`.
bool reads_a(const asm_ast::Type_instruction& instr);
// Returns whether `instr` writes A without reading it first. That
// covers every move `asm_emit` writes through A: a load into A, a move
// from memory or a constant into memory, a move between X and Y, and a
// move from X or Y to the soft stack or through a pointer. A `Call`, a
// `LoadAddress`, a `FunctionPrologue`, an `AllocateStack`, and a
// `DecimalArith` write A too.
bool kills_a(const asm_ast::Type_instruction& instr);
// Returns whether `instr` reads X, as a source, as the index of an
// `IndexedData` operand, or by `INX` / `DEX`.
bool reads_x(const asm_ast::Type_instruction& instr);
// Returns whether `instr` writes X without reading it first. A call
// does, unless the callee preserves X.
bool kills_x(const asm_ast::Type_instruction& instr);
// Returns whether `instr` reads Y, with the shapes of `reads_x` and the
// `IndirectY` / `IndirectZpY` operands, which take Y as their offset.
bool reads_y(const asm_ast::Type_instruction& instr);
// Returns whether `instr` writes Y without reading it first, counting
// the `LDY #off` that an `Indirect` or `IndirectZp` operand of a `Mov`
// or the destination of a `LoadAddress` is written with.
bool kills_y(const asm_ast::Type_instruction& instr);

// Returns whether `instr` is a call to a routine declared to leave
// `reg` ("X" or "Y") unchanged (`[[c6502::preserves_x]]` /
// `preserves_y`).
bool call_preserves(const asm_ast::Type_instruction& instr, std::string_view reg);

// Returns whether any operand of `instr` is lowered with an `LDY #off`
// setup (`Indirect`, `IndirectZp`, `Frame`, `Stack`), so the
// instruction writes Y whatever it does otherwise.
bool sets_up_y(const asm_ast::Type_instruction& instr);

// Returns whether `instr` is a return from a `return_bool_in_carry`
// function, which hands its result back in the carry flag. C is live
// there. N, Z, and V aren't, and A isn't unless `save_a` is set.
bool returns_carry(const asm_ast::Type_instruction& instr);

// Returns whether the N/Z flags at `instrs[idx]` are dead: no `Branch`
// on N or Z reads them before an instruction that sets them. The walk
// goes through labels, since the flags reach a label by fall-through,
// and follows a `Jump` to its target. A `Ret` / `Return` ends the path.
// `BCC` / `BCS` / `BVC` / `BVS` don't read N or Z, and the walk follows
// both of their successors.
bool flags_dead_at(Instrs instrs, std::size_t idx);
bool flags_dead_at(Instrs instrs, const LabelMap& labels, std::size_t idx);

// Returns whether every flag at `instrs[idx]` is dead: no instruction
// reads N, Z, C, or V before an instruction that writes N, Z, and C.
// Any branch counts as a read, and so do ADC, SBC, ROL, ROR, and a
// carry-flag return.
bool all_flags_dead_at(Instrs instrs, std::size_t idx);
bool all_flags_dead_at(Instrs instrs, const LabelMap& labels, std::size_t idx);

// Returns whether the carry flag at `instrs[idx]` is dead: on every path
// forward something writes C before anything reads it. The readers are
// ADC, SBC, ROL, ROR, BCC, BCS, and a carry-flag return. A branch on N,
// Z, or V doesn't read C. A `JMP` out of the function (a tail call)
// writes C as a `Call` does.
bool c_dead_at(Instrs instrs, std::size_t idx);
bool c_dead_at(Instrs instrs, const LabelMap& labels, std::size_t idx);

// Returns whether the overflow flag at `instrs[idx]` is dead. Only BVC
// and BVS read it.
bool v_dead_at(Instrs instrs, std::size_t idx);
bool v_dead_at(Instrs instrs, const LabelMap& labels, std::size_t idx);

// Returns whether the 6502 code for `instr` overwrites both N and Z.
bool sets_flags(const asm_ast::Type_instruction& instr);

}  // namespace c6502::passes
