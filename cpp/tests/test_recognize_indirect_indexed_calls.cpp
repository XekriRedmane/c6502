// The calls that the tests of tests/test_recognize_indirect_indexed.py made of the functions
// that `check-captures` runs, each with what the Python function
// returned, changed, or raised, which the C++ port must return,
// change, or raise too. tools/record_test_calls.py recorded them
// while the Python tests ran, before they were removed (step 8,
// groups G6 and G7). A call that another C++ case already checks
// isn't repeated here.
#include "doctest.h"
#include "record_case.h"

TEST_CASE("tac_recognize_indirect_indexed: test_recognize_indirect_indexed.py::TestRecognizeIndirectIndexedLoadUnit::test_constant_addend_does_not_fold (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass tac_recognize_indirect_indexed\n"
      "source tests/test_recognize_indirect_indexed.py::TestRecognizeIndirectIndexedLoadUnit::test_constant_addend_does_not_fold\n"
      "args {\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": null}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"y\") Var(\"%ext\"))\n"
      "  Binary(Add Constant(ConstUInt(8192)) Var(\"%ext\") Var(\"%addr\"))\n"
      "  Load(Var(\"%addr\") Var(\"b\") false)\n"
      "])\n"
      "output\n"
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"y\") Var(\"%ext\"))\n"
      "  Binary(Add Constant(ConstUInt(8192)) Var(\"%ext\") Var(\"%addr\"))\n"
      "  Load(Var(\"%addr\") Var(\"b\") false)\n"
      "])\n");
}

TEST_CASE("tac_recognize_indirect_indexed: test_recognize_indirect_indexed.py::TestRecognizeIndirectIndexedLoadUnit::test_multi_use_addr_does_not_fold (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass tac_recognize_indirect_indexed\n"
      "source tests/test_recognize_indirect_indexed.py::TestRecognizeIndirectIndexedLoadUnit::test_multi_use_addr_does_not_fold\n"
      "args {\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": null}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"y\") Var(\"%ext\"))\n"
      "  Binary(Add Var(\"p\") Var(\"%ext\") Var(\"%addr\"))\n"
      "  Load(Var(\"%addr\") Var(\"b\") false)\n"
      "  Copy(Var(\"%addr\") Var(\"%other\"))\n"
      "])\n"
      "output\n"
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"y\") Var(\"%ext\"))\n"
      "  Binary(Add Var(\"p\") Var(\"%ext\") Var(\"%addr\"))\n"
      "  Load(Var(\"%addr\") Var(\"b\") false)\n"
      "  Copy(Var(\"%addr\") Var(\"%other\"))\n"
      "])\n");
}

TEST_CASE("tac_recognize_indirect_indexed: test_recognize_indirect_indexed.py::TestRecognizeIndirectIndexedLoadUnit::test_no_symbols_is_noop (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass tac_recognize_indirect_indexed\n"
      "source tests/test_recognize_indirect_indexed.py::TestRecognizeIndirectIndexedLoadUnit::test_no_symbols_is_noop\n"
      "args {\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"y\") Var(\"%ext\"))\n"
      "  Binary(Add Var(\"p\") Var(\"%ext\") Var(\"%addr\"))\n"
      "  Load(Var(\"%addr\") Var(\"b\") false)\n"
      "])\n"
      "output\n"
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"y\") Var(\"%ext\"))\n"
      "  Binary(Add Var(\"p\") Var(\"%ext\") Var(\"%addr\"))\n"
      "  Load(Var(\"%addr\") Var(\"b\") false)\n"
      "])\n");
}

TEST_CASE("tac_recognize_indirect_indexed: test_recognize_indirect_indexed.py::TestRecognizeIndirectIndexedLoadUnit::test_non_uchar_index_does_not_fold (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass tac_recognize_indirect_indexed\n"
      "source tests/test_recognize_indirect_indexed.py::TestRecognizeIndirectIndexedLoadUnit::test_non_uchar_index_does_not_fold\n"
      "args {\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": null}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"y\") Var(\"%ext\"))\n"
      "  Binary(Add Var(\"p\") Var(\"%ext\") Var(\"%addr\"))\n"
      "  Load(Var(\"%addr\") Var(\"b\") false)\n"
      "])\n"
      "output\n"
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"y\") Var(\"%ext\"))\n"
      "  Binary(Add Var(\"p\") Var(\"%ext\") Var(\"%addr\"))\n"
      "  Load(Var(\"%addr\") Var(\"b\") false)\n"
      "])\n");
}

TEST_CASE("tac_recognize_indirect_indexed: test_recognize_indirect_indexed.py::TestRecognizeIndirectIndexedLoadUnit::test_sign_extend_def_does_not_fold (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass tac_recognize_indirect_indexed\n"
      "source tests/test_recognize_indirect_indexed.py::TestRecognizeIndirectIndexedLoadUnit::test_sign_extend_def_does_not_fold\n"
      "args {\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": null}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Function(\"f\" true [] [\n"
      "  SignExtend(Var(\"y\") Var(\"%ext\"))\n"
      "  Binary(Add Var(\"p\") Var(\"%ext\") Var(\"%addr\"))\n"
      "  Load(Var(\"%addr\") Var(\"b\") false)\n"
      "])\n"
      "output\n"
      "Function(\"f\" true [] [\n"
      "  SignExtend(Var(\"y\") Var(\"%ext\"))\n"
      "  Binary(Add Var(\"p\") Var(\"%ext\") Var(\"%addr\"))\n"
      "  Load(Var(\"%addr\") Var(\"b\") false)\n"
      "])\n");
}

TEST_CASE("tac_recognize_indirect_indexed: test_recognize_indirect_indexed.py::TestRecognizeIndirectIndexedLoadUnit::test_uint_dst_does_not_fold (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass tac_recognize_indirect_indexed\n"
      "source tests/test_recognize_indirect_indexed.py::TestRecognizeIndirectIndexedLoadUnit::test_uint_dst_does_not_fold\n"
      "args {\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": null}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"y\") Var(\"%ext\"))\n"
      "  Binary(Add Var(\"p\") Var(\"%ext\") Var(\"%addr\"))\n"
      "  Load(Var(\"%addr\") Var(\"b\") false)\n"
      "])\n"
      "output\n"
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"y\") Var(\"%ext\"))\n"
      "  Binary(Add Var(\"p\") Var(\"%ext\") Var(\"%addr\"))\n"
      "  Load(Var(\"%addr\") Var(\"b\") false)\n"
      "])\n");
}

TEST_CASE("tac_recognize_indirect_indexed: test_recognize_indirect_indexed.py::TestRecognizeIndirectIndexedConstOffset::test_global_pointer_changed_does_not_fold (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass tac_recognize_indirect_indexed\n"
      "source tests/test_recognize_indirect_indexed.py::TestRecognizeIndirectIndexedConstOffset::test_global_pointer_changed_does_not_fold\n"
      "args {\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": null}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Function(\"f\" true [] [\n"
      "  Binary(Add Var(\"gp\") Constant(ConstInt(1)) Var(\"%addr\"))\n"
      "  Load(Var(\"%addr\") Var(\"b\") false)\n"
      "  FunctionCall(\"h\" [] None false false)\n"
      "  Binary(Add Var(\"b\") Constant(ConstUChar(1)) Var(\"value\"))\n"
      "  Store(Var(\"value\") Var(\"%addr\") false)\n"
      "])\n"
      "output\n"
      "Function(\"f\" true [] [\n"
      "  Binary(Add Var(\"gp\") Constant(ConstInt(1)) Var(\"%addr\"))\n"
      "  Load(Var(\"%addr\") Var(\"b\") false)\n"
      "  FunctionCall(\"h\" [] None false false)\n"
      "  Binary(Add Var(\"b\") Constant(ConstUChar(1)) Var(\"value\"))\n"
      "  Store(Var(\"value\") Var(\"%addr\") false)\n"
      "])\n");
}

TEST_CASE("tac_recognize_indirect_indexed: test_recognize_indirect_indexed.py::TestRecognizeIndirectIndexedConstOffset::test_offset_outside_a_byte_does_not_fold (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass tac_recognize_indirect_indexed\n"
      "source tests/test_recognize_indirect_indexed.py::TestRecognizeIndirectIndexedConstOffset::test_offset_outside_a_byte_does_not_fold\n"
      "args {\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": null}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Function(\"f\" true [] [\n"
      "  Binary(Add Var(\"p\") Constant(ConstInt(256)) Var(\"%addr\"))\n"
      "  Load(Var(\"%addr\") Var(\"b\") false)\n"
      "])\n"
      "output\n"
      "Function(\"f\" true [] [\n"
      "  Binary(Add Var(\"p\") Constant(ConstInt(256)) Var(\"%addr\"))\n"
      "  Load(Var(\"%addr\") Var(\"b\") false)\n"
      "])\n");
}

TEST_CASE("tac_recognize_indirect_indexed: test_recognize_indirect_indexed.py::TestRecognizeIndirectIndexedConstOffset::test_offset_outside_a_byte_does_not_fold (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass tac_recognize_indirect_indexed\n"
      "source tests/test_recognize_indirect_indexed.py::TestRecognizeIndirectIndexedConstOffset::test_offset_outside_a_byte_does_not_fold\n"
      "args {\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": null}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Function(\"f\" true [] [\n"
      "  Binary(Add Var(\"p\") Constant(ConstInt(-1)) Var(\"%addr\"))\n"
      "  Load(Var(\"%addr\") Var(\"b\") false)\n"
      "])\n"
      "output\n"
      "Function(\"f\" true [] [\n"
      "  Binary(Add Var(\"p\") Constant(ConstInt(-1)) Var(\"%addr\"))\n"
      "  Load(Var(\"%addr\") Var(\"b\") false)\n"
      "])\n");
}

TEST_CASE("tac_recognize_indirect_indexed: test_recognize_indirect_indexed.py::TestRecognizeIndirectIndexedConstOffset::test_other_reader_of_offset_does_not_fold (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass tac_recognize_indirect_indexed\n"
      "source tests/test_recognize_indirect_indexed.py::TestRecognizeIndirectIndexedConstOffset::test_other_reader_of_offset_does_not_fold\n"
      "args {\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": null}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Function(\"f\" true [] [\n"
      "  Binary(Add Var(\"p\") Constant(ConstInt(1)) Var(\"%addr\"))\n"
      "  Load(Var(\"%addr\") Var(\"b\") false)\n"
      "  Copy(Var(\"%addr\") Var(\"q\"))\n"
      "  Binary(Add Var(\"b\") Constant(ConstUChar(1)) Var(\"value\"))\n"
      "  Store(Var(\"value\") Var(\"%addr\") false)\n"
      "])\n"
      "output\n"
      "Function(\"f\" true [] [\n"
      "  Binary(Add Var(\"p\") Constant(ConstInt(1)) Var(\"%addr\"))\n"
      "  Load(Var(\"%addr\") Var(\"b\") false)\n"
      "  Copy(Var(\"%addr\") Var(\"q\"))\n"
      "  Binary(Add Var(\"b\") Constant(ConstUChar(1)) Var(\"value\"))\n"
      "  Store(Var(\"value\") Var(\"%addr\") false)\n"
      "])\n");
}

TEST_CASE("tac_recognize_indirect_indexed: test_recognize_indirect_indexed.py::TestRecognizeIndirectIndexedConstOffset::test_wide_offset_load_folds (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass tac_recognize_indirect_indexed\n"
      "source tests/test_recognize_indirect_indexed.py::TestRecognizeIndirectIndexedConstOffset::test_wide_offset_load_folds\n"
      "args {\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": null}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Function(\"f\" true [] [\n"
      "  Binary(Add Var(\"p\") Constant(ConstInt(255)) Var(\"%addr\"))\n"
      "  Load(Var(\"%addr\") Var(\"b\") false)\n"
      "])\n"
      "output\n"
      "Function(\"f\" true [] [\n"
      "  Binary(Add Var(\"p\") Constant(ConstInt(255)) Var(\"%addr\"))\n"
      "  Load(Var(\"%addr\") Var(\"b\") false)\n"
      "])\n");
}

TEST_CASE("tac_recognize_indirect_indexed: test_recognize_indirect_indexed.py::TestSharedByteIndex::test_other_reader_does_not_fold (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass tac_recognize_indirect_indexed\n"
      "source tests/test_recognize_indirect_indexed.py::TestSharedByteIndex::test_other_reader_does_not_fold\n"
      "args {\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": null}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Function(\"f\" true [] [\n"
      "  Copy(Var(\"value\") Var(\"y\"))\n"
      "  ZeroExtend(Var(\"y\") Var(\"%ext\"))\n"
      "  Binary(Add Var(\"p\") Var(\"%ext\") Var(\"%addr\"))\n"
      "  Load(Var(\"%addr\") Var(\"b\") false)\n"
      "  Store(Var(\"value\") Var(\"%addr\") false)\n"
      "  Copy(Var(\"%addr\") Var(\"%other\"))\n"
      "])\n"
      "output\n"
      "Function(\"f\" true [] [\n"
      "  Copy(Var(\"value\") Var(\"y\"))\n"
      "  ZeroExtend(Var(\"y\") Var(\"%ext\"))\n"
      "  Binary(Add Var(\"p\") Var(\"%ext\") Var(\"%addr\"))\n"
      "  Load(Var(\"%addr\") Var(\"b\") false)\n"
      "  Store(Var(\"value\") Var(\"%addr\") false)\n"
      "  Copy(Var(\"%addr\") Var(\"%other\"))\n"
      "])\n");
}

TEST_CASE("tac_recognize_indirect_indexed: test_recognize_indirect_indexed.py::TestRecognizeIndirectIndexedDefersToIndexedStore::test_copy_chain_to_constant_defers (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass tac_recognize_indirect_indexed\n"
      "source tests/test_recognize_indirect_indexed.py::TestRecognizeIndirectIndexedDefersToIndexedStore::test_copy_chain_to_constant_defers\n"
      "args {\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": null}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Function(\"f\" true [] [\n"
      "  Copy(Constant(ConstUInt(16384)) Var(\"%p1\"))\n"
      "  Copy(Var(\"%p1\") Var(\"%p2\"))\n"
      "  ZeroExtend(Var(\"y\") Var(\"%ext\"))\n"
      "  Binary(Add Var(\"%p2\") Var(\"%ext\") Var(\"%addr\"))\n"
      "  Load(Var(\"%addr\") Var(\"b\") false)\n"
      "])\n"
      "output\n"
      "Function(\"f\" true [] [\n"
      "  Copy(Constant(ConstUInt(16384)) Var(\"%p1\"))\n"
      "  Copy(Var(\"%p1\") Var(\"%p2\"))\n"
      "  ZeroExtend(Var(\"y\") Var(\"%ext\"))\n"
      "  Binary(Add Var(\"%p2\") Var(\"%ext\") Var(\"%addr\"))\n"
      "  Load(Var(\"%addr\") Var(\"b\") false)\n"
      "])\n");
}

TEST_CASE("tac_recognize_indirect_indexed: test_recognize_indirect_indexed.py::TestRecognizeIndirectIndexedDefersToIndexedStore::test_copy_constant_ptr_defers_load (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass tac_recognize_indirect_indexed\n"
      "source tests/test_recognize_indirect_indexed.py::TestRecognizeIndirectIndexedDefersToIndexedStore::test_copy_constant_ptr_defers_load\n"
      "args {\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": null}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Function(\"f\" true [] [\n"
      "  Copy(Constant(ConstUInt(16384)) Var(\"%ptr\"))\n"
      "  ZeroExtend(Var(\"y\") Var(\"%ext\"))\n"
      "  Binary(Add Var(\"%ptr\") Var(\"%ext\") Var(\"%addr\"))\n"
      "  Load(Var(\"%addr\") Var(\"b\") false)\n"
      "])\n"
      "output\n"
      "Function(\"f\" true [] [\n"
      "  Copy(Constant(ConstUInt(16384)) Var(\"%ptr\"))\n"
      "  ZeroExtend(Var(\"y\") Var(\"%ext\"))\n"
      "  Binary(Add Var(\"%ptr\") Var(\"%ext\") Var(\"%addr\"))\n"
      "  Load(Var(\"%addr\") Var(\"b\") false)\n"
      "])\n");
}

TEST_CASE("tac_recognize_indirect_indexed: test_recognize_indirect_indexed.py::TestRecognizeIndirectIndexedDefersToIndexedStore::test_copy_constant_ptr_defers_store (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass tac_recognize_indirect_indexed\n"
      "source tests/test_recognize_indirect_indexed.py::TestRecognizeIndirectIndexedDefersToIndexedStore::test_copy_constant_ptr_defers_store\n"
      "args {\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": null}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Function(\"f\" true [] [\n"
      "  Copy(Constant(ConstUInt(16384)) Var(\"%ptr\"))\n"
      "  ZeroExtend(Var(\"y\") Var(\"%ext\"))\n"
      "  Binary(Add Var(\"%ptr\") Var(\"%ext\") Var(\"%addr\"))\n"
      "  Store(Var(\"value\") Var(\"%addr\") false)\n"
      "])\n"
      "output\n"
      "Function(\"f\" true [] [\n"
      "  Copy(Constant(ConstUInt(16384)) Var(\"%ptr\"))\n"
      "  ZeroExtend(Var(\"y\") Var(\"%ext\"))\n"
      "  Binary(Add Var(\"%ptr\") Var(\"%ext\") Var(\"%addr\"))\n"
      "  Store(Var(\"value\") Var(\"%addr\") false)\n"
      "])\n");
}

TEST_CASE("tac_recognize_indirect_indexed: test_recognize_indirect_indexed.py::TestRecognizeIndirectIndexedStoreUnit::test_uint_src_does_not_fold (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass tac_recognize_indirect_indexed\n"
      "source tests/test_recognize_indirect_indexed.py::TestRecognizeIndirectIndexedStoreUnit::test_uint_src_does_not_fold\n"
      "args {\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": null}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"y\") Var(\"%ext\"))\n"
      "  Binary(Add Var(\"p\") Var(\"%ext\") Var(\"%addr\"))\n"
      "  Store(Var(\"value\") Var(\"%addr\") false)\n"
      "])\n"
      "output\n"
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"y\") Var(\"%ext\"))\n"
      "  Binary(Add Var(\"p\") Var(\"%ext\") Var(\"%addr\"))\n"
      "  Store(Var(\"value\") Var(\"%addr\") false)\n"
      "])\n");
}
