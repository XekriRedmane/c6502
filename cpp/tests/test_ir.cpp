// Tests of the generated IR classes' equality and dump format, with
// the runtime's box and dispatch helpers (src/asdl/runtime.h).
#include <string>

#include "asdl/runtime.h"
#include "asm_ast.h"
#include "c99_ast.h"
#include "doctest.h"
#include "tac_ast.h"

namespace asdl = c6502::asdl;
namespace ir = c6502::asm_ast;

TEST_CASE("equality compares the constructor and every field") {
  const ir::Type_operand a = ir::Data{.name = "x", .offset = 0};
  const ir::Type_operand b = ir::Data{.name = "x", .offset = 0};
  const ir::Type_operand c = ir::Data{.name = "x", .offset = 1};
  const ir::Type_operand low = ir::ImmLabelLow{.name = "x", .offset = 0};
  const ir::Type_operand high = ir::ImmLabelHigh{.name = "x", .offset = 0};
  CHECK(a == b);
  CHECK(a != c);
  // Two constructors with the same fields differ, as two dataclasses of
  // different classes do.
  CHECK(low != high);
  CHECK(ir::Reg{.reg = ir::A{}} == ir::Reg{.reg = ir::A{}});
  CHECK(ir::Reg{.reg = ir::A{}} != ir::Reg{.reg = ir::X{}});
}

TEST_CASE("isa and as stand in for isinstance") {
  const ir::Type_operand op = ir::ZP{.address = 0x80, .offset = 1};
  CHECK(asdl::isa<ir::ZP>(op));
  CHECK(asdl::isa<ir::Data, ir::ZP>(op));
  CHECK_FALSE(asdl::isa<ir::Data, ir::Frame>(op));
  REQUIRE(asdl::as<ir::ZP>(op) != nullptr);
  CHECK(asdl::as<ir::ZP>(op)->offset == 1);
  CHECK(asdl::as<ir::Data>(op) == nullptr);
  CHECK(ir::kind(op) == ir::Kind_operand::ZP);
}

TEST_CASE("a box copies its value and compares by value") {
  namespace c99 = c6502::c99_ast;
  c99::Type_data_type ptr = c99::Pointer{.referenced_type = c99::Int{}};
  c99::Type_data_type copy = ptr;
  CHECK(copy == ptr);
  std::get<c99::Pointer>(copy).referenced_type = c99::Char{};
  CHECK(copy != ptr);
  CHECK(std::holds_alternative<c99::Int>(
      *std::get<c99::Pointer>(ptr).referenced_type));
}

namespace {

ir::Program sample_program() {
  ir::Function fn{
      .name = "f",
      .is_global = true,
      .params = {"@0.a", "odd \"name\"\\"},
      .instructions = {
          ir::Label{.name = ".f@start"},
          ir::Mov{.src = ir::Imm{.value = -5},
                  .dst = ir::Reg{.reg = ir::A{}},
                  .is_volatile = false},
          ir::Mov{.src = ir::IndirectY{},
                  .dst = ir::Data{.name = "x", .offset = 1},
                  .is_volatile = true},
          ir::Call{.name = "g", .reg_args = {"A", "X"}, .preserves = {}},
          ir::Ret{.arg_bytes = 0,
                  .local_bytes = 2,
                  .save_a = true,
                  .save_x = false,
                  .callee_saved_addrs = {0xC0, 0xC1},
                  .save_c = false},
      }};
  ir::StaticVariable sv{
      .name = "s",
      .is_global = false,
      .init = {ir::StringInit{.str = "a\n\xc3\xa9\xe2\x82\xac", .bytes = 6},
               ir::DoubleInit{.bits = (asdl::Int(1) << 64) - 1},
               ir::ZeroInit{.bytes = 3}}};
  return ir::Program{.top_level = {fn, sv, ir::Function{.name = "empty"}}};
}

}  // namespace

TEST_CASE("the dump format writes one instruction per line") {
  const std::string text = asdl::dump(sample_program());
  const std::string expected =
      "Program([\n"
      "  Function(\"f\" true [\"@0.a\" \"odd \\\"name\\\"\\\\\"] [\n"
      "    Label(\".f@start\")\n"
      "    Mov(Imm(-5) Reg(A) false)\n"
      "    Mov(IndirectY Data(\"x\" 1) true)\n"
      "    Call(\"g\" [\"A\" \"X\"] [])\n"
      "    Ret(0 2 true false [192 193] false)\n"
      "  ])\n"
      "  StaticVariable(\"s\" false [StringInit(\"a\\x0a\\xe9\\u{20ac}\" 6) "
      "DoubleInit(18446744073709551615) ZeroInit(3)])\n"
      "  Function(\"empty\" false [] [])\n"
      "])\n";
  CHECK(text == expected);
}

TEST_CASE("a dump reads back as an equal value") {
  const ir::Program prog = sample_program();
  const std::string text = asdl::dump(prog);
  const auto back = asdl::load<ir::Program>(text);
  CHECK(back == prog);
  CHECK(asdl::dump(back) == text);
}

TEST_CASE("the reader takes any whitespace and rejects bad text") {
  const auto op = asdl::load<ir::Type_operand>("  Data( \"x\"\n -3 )  ");
  CHECK(op == ir::Type_operand(ir::Data{.name = "x", .offset = -3}));
  CHECK_THROWS_AS(asdl::load<ir::Type_operand>("Data(\"x\")"),
                  asdl::ParseError);
  CHECK_THROWS_AS(asdl::load<ir::Type_operand>("Nope(1)"), asdl::ParseError);
  CHECK_THROWS_AS(asdl::load<ir::Type_operand>("Imm(1) Imm(2)"),
                  asdl::ParseError);
}

TEST_CASE("optional fields and product types round-trip") {
  namespace tac = c6502::tac_ast;
  const tac::Type_instruction ret_none = tac::Ret{};
  const tac::Type_instruction ret_val = tac::Ret{.val = tac::Var{.name = "v"}};
  CHECK(asdl::dump(ret_none) == "Ret(None)\n");
  CHECK(asdl::dump(ret_val) == "Ret(Var(\"v\"))\n");
  CHECK(asdl::load<tac::Type_instruction>("Ret(None)") == ret_none);

  namespace c99 = c6502::c99_ast;
  const c99::Type_member_decl m{.name = "m", .data_type = c99::UChar{}};
  CHECK(asdl::dump(m) == "member_decl(\"m\" UChar)\n");
  CHECK(asdl::load<c99::Type_member_decl>(asdl::dump(m)) == m);
}
