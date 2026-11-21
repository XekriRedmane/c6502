// Part C1 of tac_to_asm: calls and the ABI, and memory and pointers.
// Not ported yet.
#include "tac_to_asm_translator.h"

namespace c6502::tac_to_asm {

Instrs Translator::translate_function_call(const tac::FunctionCall&) {
  not_ported("FunctionCall");
}
Instrs Translator::translate_indirect_call(const tac::IndirectCall&) {
  not_ported("IndirectCall");
}
Instrs Translator::translate_get_address(const tac::GetAddress&) { not_ported("GetAddress"); }
Instrs Translator::translate_load(const tac::Load&) { not_ported("Load"); }
Instrs Translator::translate_store(const tac::Store&) { not_ported("Store"); }
Instrs Translator::translate_indexed_load(const tac::IndexedLoad&) { not_ported("IndexedLoad"); }
Instrs Translator::translate_indexed_store(const tac::IndexedStore&) {
  not_ported("IndexedStore");
}
Instrs Translator::translate_indexed_symbol_store(const tac::IndexedSymbolStore&) {
  not_ported("IndexedSymbolStore");
}
Instrs Translator::translate_indexed_const_load(const tac::IndexedConstLoad&) {
  not_ported("IndexedConstLoad");
}
Instrs Translator::translate_indirect_indexed_load(const tac::IndirectIndexedLoad&) {
  not_ported("IndirectIndexedLoad");
}
Instrs Translator::translate_indirect_indexed_store(const tac::IndirectIndexedStore&) {
  not_ported("IndirectIndexedStore");
}

}  // namespace c6502::tac_to_asm
