// Tests of part R3 of the parser's descent (src/parser_statements.cpp
// and src/parser_expressions.cpp): statement fragments with the dumps
// of Python's parse of the same fragment inside `void f(void) { ... }`,
// or Python's error, recorded by a script. The fragments are parsed
// from the start rule `lex_only`, whose recognizer takes any token, so
// they need no declaration, which part R2 parses, and hold no `for`
// statement, whose scope ends at a reduction of the start rule `start`.
#include <string>

#include "asdl/runtime.h"
#include "c99_ast.h"
#include "doctest.h"
#include "errors.h"
#include "parser_descent.h"

namespace P = c6502::parser;

namespace {

struct Case {
  const char* source;
  bool ok;
  const char* expected;
};

struct InitCase {
  const char* source;
  const char* expected;
};

const Case kCases[] = {
    {R"~~(x = a + b * c - d / e % f;)~~", true, R"~~(Expression(Assignment(Var("x" None) Binary(Subtract Binary(Add Var("a" None) Binary(Multiply Var("b" None) Var("c" None) None) None) Binary(Modulo Binary(Divide Var("d" None) Var("e" None) None) Var("f" None) None) None) None))
)~~"},
    {R"~~(a = b = c;)~~", true, R"~~(Expression(Assignment(Var("a" None) Assignment(Var("b" None) Var("c" None) None) None))
)~~"},
    {R"~~(x += 1; )~~", true, R"~~(Expression(CompoundAssignment(Add Var("x" None) Constant(ConstInt(1) None) None None))
)~~"},
    {R"~~(x <<= y >> 2 << 3;)~~", true, R"~~(Expression(CompoundAssignment(LeftShift Var("x" None) Binary(LeftShift Binary(RightShift Var("y" None) Constant(ConstInt(2) None) None) Constant(ConstInt(3) None) None) None None))
)~~"},
    {R"~~(a || b && c | d ^ e & f == g != h < i > j <= k >= l;)~~", true, R"~~(Expression(Binary(LogicalOr Var("a" None) Binary(LogicalAnd Var("b" None) Binary(BitwiseOr Var("c" None) Binary(BitwiseXor Var("d" None) Binary(BitwiseAnd Var("e" None) Binary(NotEqual Binary(Equal Var("f" None) Var("g" None) None) Binary(GreaterOrEqual Binary(LessOrEqual Binary(GreaterThan Binary(LessThan Var("h" None) Var("i" None) None) Var("j" None) None) Var("k" None) None) Var("l" None) None) None) None) None) None) None) None))
)~~"},
    {R"~~(x = c ? a : b ? d : e;)~~", true, R"~~(Expression(Assignment(Var("x" None) Conditional(Var("c" None) Var("a" None) Conditional(Var("b" None) Var("d" None) Var("e" None) None) None) None))
)~~"},
    {R"~~(f(1, g(2), h());)~~", true, R"~~(Expression(FunctionCall("f" [Constant(ConstInt(1) None) FunctionCall("g" [Constant(ConstInt(2) None)] None false) FunctionCall("h" [] None false)] None false))
)~~"},
    {R"~~((*fp)(x);)~~", true, R"~~(Expression(IndirectCall(Dereference(Var("fp" None) None) [Var("x" None)] None false))
)~~"},
    {R"~~(t[i](x);)~~", true, R"~~(Expression(IndirectCall(Subscript(Var("t" None) Var("i" None) None) [Var("x" None)] None false))
)~~"},
    {R"~~(p->m.n[3]++;)~~", true, R"~~(Expression(Postfix(Increment Subscript(Dot(Arrow(Var("p" None) "m" None) "n" None) Constant(ConstInt(3) None) None) None))
)~~"},
    {R"~~(--x; ++x; x--;)~~", true, R"~~(Expression(Prefix(Decrement Var("x" None) None))
Expression(Prefix(Increment Var("x" None) None))
Expression(Postfix(Decrement Var("x" None) None))
)~~"},
    {R"~~(y = -x + +x + ~x + !x;)~~", true, R"~~(Expression(Assignment(Var("y" None) Binary(Add Binary(Add Binary(Add Unary(Negate Var("x" None) None) Unary(Plus Var("x" None) None) None) Unary(Complement Var("x" None) None) None) Unary(LogicalNot Var("x" None) None) None) None))
)~~"},
    {R"~~(y = *p + &q;)~~", true, R"~~(Expression(Assignment(Var("y" None) Binary(Add Dereference(Var("p" None) None) AddressOf(Var("q" None) None) None) None))
)~~"},
    {R"~~(y = sizeof x + sizeof (x) + sizeof (x)[0];)~~", true, R"~~(Expression(Assignment(Var("y" None) Binary(Add Binary(Add SizeOfExp(Var("x" None) None) SizeOfExp(Var("x" None) None) None) SizeOfExp(Subscript(Var("x" None) Constant(ConstInt(0) None) None) None) None) None))
)~~"},
    {R"~~(a, b, c;)~~", true, R"~~(Expression(Comma(Comma(Var("a" None) Var("b" None) None) Var("c" None) None))
)~~"},
    {R"~~(s = "ab" "c\n\x41\101\0";)~~", true, R"~~(Expression(Assignment(Var("s" None) String("abc\x0aAA\x00") None))
)~~"},
    {R"~~(c = 'a' + '\n' + '\x7F' + '\377' + L'b';)~~", true, R"~~(Expression(Assignment(Var("c" None) Binary(Add Binary(Add Binary(Add Binary(Add Constant(ConstInt(97) None) Constant(ConstInt(10) None) None) Constant(ConstInt(127) None) None) Constant(ConstInt(255) None) None) Constant(ConstInt(98) None) None) None))
)~~"},
    {R"~~(n = 0 + 32767 + 32768 + 0x8000 + 0xFFFF + 0x10000 + 2147483648 + 0xFFFFFFFF;)~~", true, R"~~(Expression(Assignment(Var("n" None) Binary(Add Binary(Add Binary(Add Binary(Add Binary(Add Binary(Add Binary(Add Constant(ConstInt(0) None) Constant(ConstInt(32767) None) None) Constant(ConstLong(32768) None) None) Constant(ConstUInt(32768) None) None) Constant(ConstUInt(65535) None) None) Constant(ConstLong(65536) None) None) Constant(ConstLongLong(2147483648) None) None) Constant(ConstULong(4294967295) None) None) None))
)~~"},
    {R"~~(n = 1L + 1U + 1UL + 1LL + 1ULL + 0x8000000000000000 + 9223372036854775807;)~~", true, R"~~(Expression(Assignment(Var("n" None) Binary(Add Binary(Add Binary(Add Binary(Add Binary(Add Binary(Add Constant(ConstLong(1) None) Constant(ConstUInt(1) None) None) Constant(ConstULong(1) None) None) Constant(ConstLongLong(1) None) None) Constant(ConstULongLong(1) None) None) Constant(ConstULongLong(9223372036854775808) None) None) Constant(ConstLongLong(9223372036854775807) None) None) None))
)~~"},
    {R"~~(n = 1'000 + 0b1010 + 017 + 0;)~~", true, R"~~(Expression(Assignment(Var("n" None) Binary(Add Binary(Add Binary(Add Constant(ConstInt(1000) None) Constant(ConstInt(10) None) None) Constant(ConstInt(15) None) None) Constant(ConstInt(0) None) None) None))
)~~"},
    {R"~~(n = 3wb + 3uwb + 0uwb + 255uwb;)~~", true, R"~~(Expression(Assignment(Var("n" None) Binary(Add Binary(Add Binary(Add Constant(ConstBitInt(3 3 true) None) Constant(ConstBitInt(3 2 false) None) None) Constant(ConstBitInt(0 1 false) None) None) Constant(ConstBitInt(255 8 false) None) None) None))
)~~"},
    {R"~~(d = 1.5 + 2.5f + 1e10 + .5 + 1.;)~~", true, R"~~(Expression(Assignment(Var("d" None) Binary(Add Binary(Add Binary(Add Binary(Add Constant(ConstDouble(4609434218613702656) None) Constant(ConstFloat(1075838976) None) None) Constant(ConstDouble(4756540486875873280) None) None) Constant(ConstDouble(4602678819172646912) None) None) Constant(ConstDouble(4607182418800017408) None) None) None))
)~~"},
    {R"~~(b = true + false; p = nullptr;)~~", true, R"~~(Expression(Assignment(Var("b" None) Binary(Add Constant(ConstBool(1) None) Constant(ConstBool(0) None) None) None))
Expression(Assignment(Var("p" None) Constant(ConstNullptr None) None))
)~~"},
    {R"~~(return;)~~", true, R"~~(Return(None)
)~~"},
    {R"~~(return x + 1;)~~", true, R"~~(Return(Binary(Add Var("x" None) Constant(ConstInt(1) None) None))
)~~"},
    {R"~~(if (a) b; else if (c) d; else e;)~~", true, R"~~(IfStmt(Var("a" None) Expression(Var("b" None)) IfStmt(Var("c" None) Expression(Var("d" None)) Expression(Var("e" None))))
)~~"},
    {R"~~(if (a) if (b) c; else d;)~~", true, R"~~(IfStmt(Var("a" None) IfStmt(Var("b" None) Expression(Var("c" None)) Expression(Var("d" None))) None)
)~~"},
    {R"~~(while (x) x--;)~~", true, R"~~(WhileStmt(Var("x" None) Expression(Postfix(Decrement Var("x" None) None)) "")
)~~"},
    {R"~~(do x++; while (x < 10);)~~", true, R"~~(DoWhileStmt(Expression(Postfix(Increment Var("x" None) None)) Binary(LessThan Var("x" None) Constant(ConstInt(10) None) None) "")
)~~"},
    {R"~~(switch (x) { case 1: y; break; case 2: default: z; })~~", true, R"~~(SwitchStmt(Var("x" None) Compound(Block([
  S(CaseStmt(Constant(ConstInt(1) None) Expression(Var("y" None)) ""))
  S(BreakStmt(""))
  S(CaseStmt(Constant(ConstInt(2) None) DefaultStmt(Expression(Var("z" None)) "") ""))
])) "" [] None None)
)~~"},
    {R"~~(L: goto L;)~~", true, R"~~(LabeledStmt("L" Goto("L"))
)~~"},
    {R"~~({ continue; break; ; })~~", true, R"~~(Compound(Block([
  S(ContinueStmt(""))
  S(BreakStmt(""))
  S(Null)
]))
)~~"},
    {R"~~(;)~~", true, R"~~(Null
)~~"},
    {R"~~(x = 1(2);)~~", false, R"~~(ParserError: called object is not a function)~~"},
    {R"~~(x = "s"(2);)~~", false, R"~~(ParserError: called object is not a function)~~"},
    {R"~~(c = 'ab';)~~", false, R"~~(ParserError: multi-character character constant 'ab' is not supported)~~"},
    {R"~~(d = 0x1.0p3;)~~", false, R"~~(ParserError: hex floating literal '0x1.0p3' is not supported)~~"},
    {R"~~(d = 1.0L;)~~", false, R"~~(ParserError: `long double` is not supported (literal '1.0L'))~~"},
    {R"~~(n = 18446744073709551616;)~~", false, R"~~(ParserError: integer constant '18446744073709551616' doesn't fit any supported type (value > 2^32 - 1))~~"},
    {R"~~(s = "\u0041";)~~", false, R"~~(ParserError: Unicode escapes (\u / \U) are not supported (c6502 has no wide-char model))~~"},
};

// Initializers, with the dumps of Python's parse of `int v = ...;`.
const InitCase kInitCases[] = {
    {R"~~({})~~", R"~~(InitList([] None None)
)~~"},
    {R"~~({1, 2, 3,})~~", R"~~(InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None) Constant(ConstInt(3) None)] None None)
)~~"},
    {R"~~({[2] = 3, .a.b = {4}, [1][0] = 5, 6})~~", R"~~(InitList([Designation([IndexDesignator(2)] Constant(ConstInt(3) None)) Designation([MemberDesignator("a") MemberDesignator("b")] InitList([Constant(ConstInt(4) None)] None None)) Designation([IndexDesignator(1) IndexDesignator(0)] Constant(ConstInt(5) None)) Constant(ConstInt(6) None)] None None)
)~~"},
    {R"~~({[2 * 3 + 1] = 1, [sizeof 1L] = 2})~~", R"~~(InitList([Designation([IndexDesignator(7)] Constant(ConstInt(1) None)) Designation([IndexDesignator(4)] Constant(ConstInt(2) None))] None None)
)~~"},
    {R"~~({{1}, {}, "ab"})~~", R"~~(InitList([InitList([Constant(ConstInt(1) None)] None None) InitList([] None None) String("ab")] None None)
)~~"},
    {R"~~(x + 1)~~", R"~~(Binary(Add Var("x" None) Constant(ConstInt(1) None) None)
)~~"},
    {R"~~({[-1] = 2})~~", R"~~(ParserError: an array designator's index is -1, and it must not be negative (C23 §6.7.11p6))~~"},
    {R"~~({[x] = 1})~~", R"~~(ParserError: an array designator's index must be an integer constant expression (C23 §6.7.11p6): undeclared identifier 'x')~~"},
    {R"~~({[1.5] = 1})~~", R"~~(ParserError: an array designator's index must be an integer constant expression (C23 §6.7.11p6): floating constant ConstDouble(bits=4609434218613702656) is not an integer constant)~~"},
};

}  // namespace

TEST_CASE("statement fragments parse as Python parses them") {
  for (const Case& c : kCases) {
    CAPTURE(c.source);
    std::string got;
    try {
      P::ParseContext ctx;
      P::Descent d(c.source, ctx, "lex_only");
      while (!d.stream().at_end()) {
        got += c6502::asdl::dump(d.parse_statement());
      }
    } catch (const c6502::CompileError& e) {
      got = e.python_type() + ": " + e.what();
    }
    CHECK(got == c.expected);
  }
}

TEST_CASE("initializers parse as Python parses them, designators folded") {
  for (const InitCase& c : kInitCases) {
    CAPTURE(c.source);
    std::string got;
    try {
      P::ParseContext ctx;
      P::Descent d(c.source, ctx, "lex_only");
      got = c6502::asdl::dump(d.parse_initializer());
    } catch (const c6502::CompileError& e) {
      got = e.python_type() + ": " + e.what();
    }
    CHECK(got == c.expected);
  }
}
