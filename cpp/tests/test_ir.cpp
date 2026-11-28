// Tests of the generated IR classes' equality and dump format, with
// the runtime's box and dispatch helpers (src/asdl/runtime.h).
#include <string>
#include <utility>
#include <vector>

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

  // The string "None" is a string, and the bare word is the empty
  // optional.
  const tac::Type_instruction ret_none_name = tac::Ret{.val = tac::Var{.name = "None"}};
  CHECK(asdl::dump(ret_none_name) == "Ret(Var(\"None\"))\n");
  CHECK(asdl::load<tac::Type_instruction>(asdl::dump(ret_none_name)) == ret_none_name);
}

TEST_CASE("the reader rejects a malformed value") {
  CHECK_THROWS_AS(asdl::load<ir::Type_operand>("Imm(x)"), asdl::ParseError);
  CHECK_THROWS_AS(asdl::load<ir::Type_instruction>("Label(\"\\q\")"), asdl::ParseError);
  CHECK_THROWS_AS(asdl::load<ir::Type_operand>("Reg(Imm(1))"), asdl::ParseError);
  CHECK_THROWS_AS(asdl::load<ir::Type_operand>("Imm(1"), asdl::ParseError);
}

TEST_CASE("a string of any code point round-trips") {
  // The code points 0 to U+017F, then U+1F600, then the lone surrogate
  // U+D800, which ir_dump.py wrote as `\u{d800}` too.
  const auto utf8 = [](char32_t c) {
    std::string out;
    if (c < 0x80) {
      out += static_cast<char>(c);
    } else if (c < 0x800) {
      out += static_cast<char>(0xC0 | (c >> 6));
      out += static_cast<char>(0x80 | (c & 0x3F));
    } else if (c < 0x10000) {
      out += static_cast<char>(0xE0 | (c >> 12));
      out += static_cast<char>(0x80 | ((c >> 6) & 0x3F));
      out += static_cast<char>(0x80 | (c & 0x3F));
    } else {
      out += static_cast<char>(0xF0 | (c >> 18));
      out += static_cast<char>(0x80 | ((c >> 12) & 0x3F));
      out += static_cast<char>(0x80 | ((c >> 6) & 0x3F));
      out += static_cast<char>(0x80 | (c & 0x3F));
    }
    return out;
  };
  std::string s;
  for (char32_t c = 0; c < 0x180; ++c) s += utf8(c);
  s += utf8(0x1F600);
  s += utf8(0xD800);
  const ir::Type_instruction label = ir::Label{.name = s};
  const std::string text = asdl::dump(label);
  // ir_dump.py's text for the same value had 1,664 characters and ended
  // so.
  CHECK(text.size() == 1664);
  CHECK(text.ends_with("\\u{17e}\\u{17f}\\u{1f600}\\u{d800}\")\n"));
  CHECK(asdl::load<ir::Type_instruction>(text) == label);
}

TEST_CASE("large and negative integers round-trip") {
  namespace tac = c6502::tac_ast;
  const asdl::Int ten15 = 1000000000000000LL;
  const std::pair<asdl::Int, std::string> cases[] = {
      {0, "0"},
      {-1, "-1"},
      {asdl::Int(1) << 63, "9223372036854775808"},
      {-(asdl::Int(1) << 63), "-9223372036854775808"},
      {(asdl::Int(1) << 64) - 1, "18446744073709551615"},
      {ten15 * ten15, "1000000000000000000000000000000"},
  };
  for (const auto& [n, digits] : cases) {
    CAPTURE(digits);
    const tac::Type_val v = tac::Constant{.const_ = tac::ConstULongLong{.value = n}};
    const std::string text = asdl::dump(v);
    CHECK(text == "Constant(ConstULongLong(" + digits + "))\n");
    CHECK(asdl::load<tac::Type_val>(text) == v);
  }
}

TEST_CASE("c99 data types compare by constructor and fields") {
  namespace c99 = c6502::c99_ast;
  const c99::Type_data_type i = c99::Int{};
  CHECK(i == c99::Type_data_type(c99::Int{}));
  CHECK(i != c99::Type_data_type(c99::Long{}));
  const auto fun = [](std::vector<c99::Type_data_type> params) {
    return c99::Type_data_type(c99::FunType{.params = std::move(params), .ret = c99::Int{}});
  };
  CHECK(fun({c99::Int{}, c99::Int{}}) == fun({c99::Int{}, c99::Int{}}));
  CHECK(fun({c99::Int{}, c99::Int{}}) != fun({c99::Int{}, c99::Long{}}));
  CHECK(fun({c99::Int{}}) != fun({c99::Int{}, c99::Int{}}));
}

TEST_CASE("pretty writes what pretty.py writes") {
  // Python's pretty.py gives these texts for the same values. A string
  // holds the code points 0 to 255 of a C string's bytes as UTF-8, and
  // Python's `repr` escapes those that `str.isprintable()` rejects, here
  // U+00AD, U+00A0, and U+0085, and copies U+00E9 and U+00FF.
  namespace tac = c6502::tac_ast;
  const tac::Type_instruction ret = tac::Ret{.val = tac::Var{.name = "it's \"q\" \\"}};
  CHECK(asdl::py_pretty(ret) ==
        "Ret(\n"
        "  val=Var(\n"
        "    name='it\\'s \"q\" \\\\',\n"
        "  ),\n"
        ")");
  CHECK(asdl::py_pretty(tac::Type_instruction(tac::Ret{})) == "Ret(\n  val=None,\n)");
  // The UTF-8 of U+00E9, U+00AD, U+00FF, U+00A0, and U+0085.
  const tac::Type_val bytes =
      tac::Var{.name = "\x01\t\x7f\xc3\xa9\xc2\xad\xc3\xbf\xc2\xa0\xc2\x85'"};
  CHECK(asdl::py_pretty(bytes) ==
        "Var(\n  name=\"\\x01\\t\\x7f\xc3\xa9\\xad\xc3\xbf\\xa0\\x85'\",\n)");
  const ir::Type_instruction call = ir::Call{.name = "f", .reg_args = {"A", "X"}};
  CHECK(asdl::py_pretty(call) ==
        "Call(\n"
        "  name='f',\n"
        "  reg_args=[\n"
        "    'A',\n"
        "    'X',\n"
        "  ],\n"
        "  preserves=[],\n"
        ")");
  CHECK(asdl::py_pretty(ir::Type_operand(ir::IndirectY{})) == "IndirectY()");
}
