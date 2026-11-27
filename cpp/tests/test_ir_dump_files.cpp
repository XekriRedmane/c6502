// tests/test_ir_dump.py's `test_cpp_reads_and_writes_the_same_bytes`:
// the dumps of tests/ir_dump/, the four IRs of
// examples/paint_hud_strip_p1.c compiled with -O as the Python compiler
// wrote them on 25 Nov 2026, read and written again byte for byte.
// test_ir.cpp round-trips the sample program of that module.
#include <string>

#include "asdl/runtime.h"
#include "asm2_ast.h"
#include "asm_ast.h"
#include "c99_ast.h"
#include "compile_case.h"
#include "doctest.h"
#include "tac_ast.h"

namespace {

namespace asdl = c6502::asdl;

template <class Program>
void check_round_trip(const char* module) {
  INFO(module);
  const std::string text = c6502::suite::read_repo_file(
      std::string("tests/ir_dump/paint_hud_strip_p1.") + module + ".txt");
  REQUIRE_FALSE(text.empty());
  CHECK(asdl::dump(asdl::load<Program>(text)) == text);
}

}  // namespace

TEST_CASE("ir_dump: the dumps of a compiled example read and write back as the same bytes") {
  check_round_trip<c6502::c99_ast::Program>("c99_ast");
  check_round_trip<c6502::tac_ast::Program>("tac_ast");
  check_round_trip<c6502::asm_ast::Program>("asm_ast");
  check_round_trip<c6502::asm2_ast::Program>("asm2_ast");
}
