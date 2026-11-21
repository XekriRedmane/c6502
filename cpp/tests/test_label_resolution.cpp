// The calls of label resolution that the Python unit tests make, each
// recorded with Python's result: the input program's dump, and the
// output's dump or the message of the LabelResolutionError. They are
// the distinct calls of tests/test_label_resolution.py, a call of
// `resolve_function` on the legacy `Function` recorded as the call of
// `resolve_program` on the program of the function_decl it builds, and
// from the rest of the suite one call for each other message the pass
// raised.
#include <string>

#include "c99_ast.h"
#include "doctest.h"
#include "errors.h"
#include "passes/label_resolution.h"

namespace {

struct Case {
  const char* input;
  bool ok;
  const char* expected;
};

const Case kCases[] = {
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(Goto("foo"))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~(goto to undefined label 'foo')~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(LabeledStmt("a" LabeledStmt("a" Null)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~(duplicate label 'a')~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(LabeledStmt("foo" Null))
    S(LabeledStmt("foo" Null))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~(duplicate label 'foo')~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(IfStmt(Constant(ConstInt(1) None) Goto("missing") None))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~(goto to undefined label 'missing')~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(Goto("nowhere"))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~(goto to undefined label 'nowhere')~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(IfStmt(Constant(ConstInt(1) None) LabeledStmt("x" Null) LabeledStmt("x" Null)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~(duplicate label 'x')~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(LabeledStmt("foo" Null))
    S(LabeledStmt("foo" Null))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~(duplicate label 'foo')~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(ForStmt(InitExp(None) None None Goto("missing") "" None))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~(goto to undefined label 'missing')~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(WhileStmt(Constant(ConstInt(1) None) LabeledStmt("foo" Null) ""))
    S(LabeledStmt("foo" Null))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~(duplicate label 'foo')~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("other" [] Block([
    S(LabeledStmt("foo" Null))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     true,
     R"~~(Program([
  FunctionDecl(function_decl("other" [] Block([
    S(LabeledStmt(".other@foo" Null))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(Goto("foo"))
    S(LabeledStmt("foo" Null))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     true,
     R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(Goto(".main@foo"))
    S(LabeledStmt(".main@foo" Null))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(LabeledStmt("foo" Null))
    S(Goto("foo"))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     true,
     R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(LabeledStmt(".main@foo" Null))
    S(Goto(".main@foo"))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(LabeledStmt("foo" Return(Constant(ConstInt(0) None))))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     true,
     R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(LabeledStmt(".main@foo" Return(Constant(ConstInt(0) None))))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(WhileStmt(Constant(ConstInt(1) None) Null ".main@loop_3"))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     true,
     R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(WhileStmt(Constant(ConstInt(1) None) Null ".main@loop_3"))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(Goto("end"))
    S(LabeledStmt("end" Return(Constant(ConstInt(0) None))))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     true,
     R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(Goto(".main@end"))
    S(LabeledStmt(".main@end" Return(Constant(ConstInt(0) None))))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(LabeledStmt("foo" Goto("foo")))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     true,
     R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(LabeledStmt(".main@foo" Goto(".main@foo")))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(IfStmt(Constant(ConstInt(1) None) LabeledStmt("foo" Null) None))
    S(Goto("foo"))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     true,
     R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(IfStmt(Constant(ConstInt(1) None) LabeledStmt(".main@foo" Null) None))
    S(Goto(".main@foo"))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(LabeledStmt("foo" Null))
    S(IfStmt(Constant(ConstInt(1) None) Goto("foo") None))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     true,
     R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(LabeledStmt(".main@foo" Null))
    S(IfStmt(Constant(ConstInt(1) None) Goto(".main@foo") None))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(IfStmt(Constant(ConstInt(1) None) Return(Constant(ConstInt(2) None)) Return(Constant(ConstInt(3) None))))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     true,
     R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(IfStmt(Constant(ConstInt(1) None) Return(Constant(ConstInt(2) None)) Return(Constant(ConstInt(3) None))))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("x" Constant(ConstInt(5) None) Int None None None None false)))
    S(Return(Var("x" None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     true,
     R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("x" Constant(ConstInt(5) None) Int None None None None false)))
    S(Return(Var("x" None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(WhileStmt(Constant(ConstInt(1) None) Compound(Block([
      S(BreakStmt(""))
      S(ContinueStmt(""))
    ])) ""))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     true,
     R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(WhileStmt(Constant(ConstInt(1) None) Compound(Block([
      S(BreakStmt(""))
      S(ContinueStmt(""))
    ])) ""))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(ForStmt(InitExp(None) None None LabeledStmt("foo" Null) "" None))
    S(Goto("foo"))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     true,
     R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(ForStmt(InitExp(None) None None LabeledStmt(".main@foo" Null) "" None))
    S(Goto(".main@foo"))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(LabeledStmt("foo" Null))
    S(WhileStmt(Constant(ConstInt(1) None) Goto("foo") ""))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     true,
     R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(LabeledStmt(".main@foo" Null))
    S(WhileStmt(Constant(ConstInt(1) None) Goto(".main@foo") ""))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(WhileStmt(Constant(ConstInt(1) None) LabeledStmt("foo" Null) ""))
    S(Goto("foo"))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     true,
     R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(WhileStmt(Constant(ConstInt(1) None) LabeledStmt(".main@foo" Null) ""))
    S(Goto(".main@foo"))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(DoWhileStmt(LabeledStmt("foo" Null) Constant(ConstInt(1) None) ""))
    S(Goto("foo"))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     true,
     R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(DoWhileStmt(LabeledStmt(".main@foo" Null) Constant(ConstInt(1) None) ""))
    S(Goto(".main@foo"))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(Goto("label"))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~(goto to undefined label 'label')~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("@0.a" None Int None None None None false)))
    S(Goto("a"))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~(goto to undefined label 'a')~~"},
    {R"~~(Program([
  VarDecl(var_decl("x" Constant(ConstInt(10) None) Int None None None None false))
  FunctionDecl(function_decl("main" [] Block([
    S(Goto("x"))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~(goto to undefined label 'x')~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(DoWhileStmt(Compound(Block([
      S(LabeledStmt("lbl" Return(Constant(ConstInt(1) None))))
      S(LabeledStmt("lbl" Return(Constant(ConstInt(2) None))))
    ])) Constant(ConstInt(1) None) ""))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~(duplicate label 'lbl')~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("@0.x" Constant(ConstInt(0) None) Int None None None None false)))
    S(LabeledStmt("label" Expression(Assignment(Var("@0.x" None) Constant(ConstInt(1) None) None))))
    S(LabeledStmt("label" Return(Constant(ConstInt(2) None))))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~(duplicate label 'label')~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("@0.x" Constant(ConstInt(0) None) Int None None None None false)))
    S(IfStmt(Var("@0.x" None) Compound(Block([
      S(Expression(Assignment(Var("@0.x" None) Constant(ConstInt(5) None) None)))
      S(Goto("l"))
      S(Return(Constant(ConstInt(0) None)))
      S(LabeledStmt("l" Return(Var("@0.x" None))))
    ])) Compound(Block([
      S(Goto("l"))
      S(Return(Constant(ConstInt(0) None)))
      S(LabeledStmt("l" Return(Var("@0.x" None))))
    ]))))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~(duplicate label 'l')~~"},
};

}  // namespace

TEST_CASE("label resolution gives the Python unit tests' results") {
  for (const Case& c : kCases) {
    auto prog = c6502::asdl::load<c6502::c99_ast::Program>(c.input);
    std::string got;
    try {
      got = c6502::asdl::dump(c6502::passes::resolve_labels(std::move(prog)));
    } catch (const c6502::CompileError& e) {
      CHECK(e.python_type() == "LabelResolutionError");
      got = e.what();
    }
    CHECK(got == c.expected);
  }
}
