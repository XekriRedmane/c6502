// The tests of tests/test_address_taken_zp.py that compiled a whole
// program: under -O, a zp_abi function's address-taken local gets a
// byte of its private pool in zero page, named by a slot symbol, and
// its address is an immediate pair.
#include <algorithm>
#include <optional>
#include <string>
#include <utility>
#include <variant>

#include "asm_ast.h"
#include "compile_case.h"
#include "doctest.h"
#include "passes/codegen_stage.h"
#include "passes/front_end.h"
#include "passes/ir_check.h"
#include "passes/zp_globals.h"

namespace {

namespace suite = c6502::suite;
namespace asm_ast = c6502::asm_ast;
namespace passes = c6502::passes;

// Returns the finished asm program and the slot symbols that
// `sim.harness.compile_to_asm(source, optimize=True)` returned, which
// is where `codegen` stops without `emit`.
passes::StageResult compile_to_asm(const std::string& source) {
  const passes::ir_check::Enabled checks(true);
  passes::FrontEndOptions front;
  front.include_dir = suite::repo_path("include");
  front.optimize = true;
  std::string err;
  passes::StageTables tables;
  passes::FrontEndResult checked = passes::front_end(source, front, err);
  tables.c99 = std::move(checked.prog);
  tables.symbols = std::move(checked.symbols);
  tables.types = std::move(checked.types);
  tables.footprints = passes::helper_footprints();
  passes::CodegenOptions options;
  options.optimize = true;
  options.emit = false;
  return passes::codegen(tables, options);
}

// Returns the address of the slot symbol `name`, or none.
std::optional<c6502::asdl::Int> slot(const passes::StageResult& r, const std::string& name) {
  for (const auto& [sym, addr] : r.harness_symbols) {
    if (sym == name) return addr;
  }
  return std::nullopt;
}

}  // namespace

TEST_CASE("address_taken_zp: an address-taken unsigned char lands in zero page") {
  const auto r = compile_to_asm(R"(
    #include <stdint.h>
    [[c6502::zp_abi]]
    void writer(uint8_t *out) { *out = 0x42; }
    [[c6502::zp_abi]]
    void caller(void) {
        uint8_t x;
        writer(&x);
    }
    int main(void) { caller(); return 0; }
  )");
  const auto addr = slot(r, "__local_caller__x");
  REQUIRE_MESSAGE(addr.has_value(),
                  "the address-taken local should get an EQU binding for __local_caller__x");
  CHECK_MESSAGE(*addr < 0x100, "the address-taken slot should land in zero page");
}

TEST_CASE("address_taken_zp: a callee's pointer argument to a local is an immediate address") {
  // The shape of `entity_proximity`: a zp_abi caller passes the address
  // of a uchar local to a zp_abi callee that writes through it. When the
  // local is assigned a slot, it's a Data operand, and
  // `lower_data_load_address` makes `LoadAddress(src=Data(sym))` the
  // pair `LDA #<sym; ...; LDA #>sym`. Otherwise it's a
  // `LoadAddress(src=Frame(...))`, which lowers to the six instructions
  // that add the offset to FP.
  const auto r = compile_to_asm(R"(
    #include <stdint.h>
    #include <stdbool.h>
    uint8_t target;
    [[c6502::zp_abi]]
    bool finder(uint8_t *out) {
        *out = target;
        return true;
    }
    [[c6502::zp_abi]]
    void caller(void) {
        uint8_t row;
        if (!finder(&row)) return;
        target = row;
    }
    int main(void) { caller(); return 0; }
  )");
  const std::string sym = "__local_caller__row";
  CHECK(slot(r, sym).has_value());
  const asm_ast::Function* caller = nullptr;
  for (const auto& tl : r.emitted.asm_prog.top_level) {
    if (const auto* fn = std::get_if<asm_ast::Function>(&tl); fn && fn->name == "caller") {
      caller = fn;
      break;
    }
  }
  REQUIRE(caller != nullptr);
  const bool has_immediate_address =
      std::any_of(caller->instructions.begin(), caller->instructions.end(),
                  [&](const asm_ast::Type_instruction& instr) {
                    const auto* mov = std::get_if<asm_ast::Mov>(&instr);
                    if (mov == nullptr) return false;
                    const auto* low = std::get_if<asm_ast::ImmLabelLow>(&mov->src);
                    return low != nullptr && low->name == sym;
                  });
  CHECK_MESSAGE(has_immediate_address,
                "expected `LDA #<__local_caller__row` in the caller's body; the address-taken "
                "local didn't get the immediate address");
}
