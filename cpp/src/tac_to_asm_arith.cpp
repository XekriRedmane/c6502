// Part C2 of tac_to_asm: arithmetic, the inline shifts, divisions, and
// products, and the shift and divide helpers. Not ported yet. The two
// plans return no rotations and no pairs, so the other parts can be
// checked on the functions that have none.
#include "tac_to_asm_translator.h"

namespace c6502::tac_to_asm {

std::pair<std::map<int, Rotation>, std::set<int>> Translator::plan_rotations(
    const std::vector<tac::Type_instruction>&) {
  return {};
}

Instrs Translator::rotate_in(const tac::Type_val&, const Rotation&) { not_ported("rotate_in"); }

std::pair<std::map<int, DivmodPair>, std::set<int>> Translator::plan_divmod_pairs(
    const std::vector<tac::Type_instruction>&, const std::set<int>&) {
  return {};
}

Instrs Translator::translate_divmod_pair(const tac::Type_val&, const tac::Type_val&,
                                         const DivmodPair&) {
  not_ported("a divide and remainder pair");
}

Instrs Translator::translate_binary(const tac::Type_binary_operator&, const tac::Type_val&,
                                    const tac::Type_val&, const tac::Type_val&) {
  not_ported("Binary");
}

}  // namespace c6502::tac_to_asm
