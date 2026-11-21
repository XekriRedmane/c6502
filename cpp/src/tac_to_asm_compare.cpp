// Part C3 of tac_to_asm: casts, unary operators, conditional jumps, and
// comparisons. Not ported yet.
#include "tac_to_asm_translator.h"

namespace c6502::tac_to_asm {

Instrs Translator::translate_sign_extend(const tac::Type_val&, const tac::Type_val&) {
  not_ported("SignExtend");
}
Instrs Translator::translate_zero_extend(const tac::Type_val&, const tac::Type_val&) {
  not_ported("ZeroExtend");
}
Instrs Translator::translate_int_to_fp(const tac::Type_val&, const tac::Type_val&, bool) {
  not_ported("an integer to floating conversion");
}
Instrs Translator::translate_fp_to_int(const tac::Type_val&, const tac::Type_val&, bool) {
  not_ported("a floating to integer conversion");
}
Instrs Translator::translate_unary(const tac::Type_unary_operator&, const tac::Type_val&,
                                   const tac::Type_val&) {
  not_ported("Unary");
}
Instrs Translator::translate_cond_jump(const tac::Type_val&, const std::string&,
                                       const ir::Type_condition&) {
  not_ported("JumpIfTrue and JumpIfFalse");
}
Instrs Translator::translate_jump_if_cmp(const tac::Type_binary_operator&, const tac::Type_val&,
                                         const tac::Type_val&, const std::string&) {
  not_ported("JumpIfCmp");
}
Instrs Translator::translate_jump_if_masked(const tac::Type_val&, asdl::Int, bool,
                                            const std::string&) {
  not_ported("JumpIfMasked");
}
int Translator::equality_bytes(const tac::Type_val&, const tac::Type_val&, int, bool) {
  not_ported("equality_bytes");
}
Instrs Translator::translate_equality(const ir::Type_operand&, const ir::Type_operand&,
                                      const ir::Type_operand&, int, const ir::Type_condition&,
                                      int, bool) {
  not_ported("an equality");
}
Instrs Translator::translate_signed_ordering(const ir::Type_operand&, const ir::Type_operand&,
                                             const ir::Type_operand&, int,
                                             const ir::Type_condition&, int) {
  not_ported("a signed ordering");
}
Instrs Translator::translate_unsigned_ordering(const ir::Type_operand&, const ir::Type_operand&,
                                               const ir::Type_operand&, int,
                                               const ir::Type_condition&, int) {
  not_ported("an unsigned ordering");
}
Instrs Translator::translate_fp_ordering(const ir::Type_operand&, const ir::Type_operand&,
                                         const ir::Type_operand&, int, int, bool) {
  not_ported("a floating ordering");
}

}  // namespace c6502::tac_to_asm
