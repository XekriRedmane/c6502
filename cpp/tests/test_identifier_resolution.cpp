// The calls of identifier resolution that the Python unit tests make,
// each recorded with Python's result: the input program's dump, and the
// output's dump or the message of the IdentifierResolutionError. They
// are the distinct calls of tests/test_identifier_resolution.py, a call
// of `resolve_function` on the legacy `Function` recorded as the call of
// `resolve_program` on the program it builds, and from the rest of the
// suite one call for each other message the pass raised. The corpus
// rejects few programs, so most of the messages are checked here.
#include <string>

#include "c99_ast.h"
#include "doctest.h"
#include "errors.h"
#include "passes/identifier_resolution.h"

namespace {

struct Case {
  const char* input;
  bool ok;
  const char* expected;
};

const Case kCases[] = {
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(Return(Var("a" None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~(undeclared identifier 'a')~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(Return(Var("x" None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~(undeclared identifier 'x')~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(LabeledStmt("foo" Return(Var("a" None))))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~(undeclared identifier 'a')~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" ["a" "a"] Block([
    S(Return(Var("a" None)))
  ]) FunType([Int Int] Int) None None ["" ""] false None [] None false))
])
)~~",
     false,
     R"~~(duplicate parameter name 'a')~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(Return(FunctionCall("foo" [] None false)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~(undeclared identifier 'foo')~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(Compound(Block([
      S(Return(Var("b" None)))
    ])))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~(undeclared identifier 'b')~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(IfStmt(Constant(ConstInt(1) None) Return(Var("a" None)) None))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~(undeclared identifier 'a')~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(IfStmt(Var("a" None) Return(Constant(ConstInt(0) None)) None))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~(undeclared identifier 'a')~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("a" Var("b" None) Int None None None None false)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~(undeclared identifier 'b')~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(WhileStmt(Var("a" None) Null ""))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~(undeclared identifier 'a')~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(IfStmt(Constant(ConstInt(1) None) Return(Constant(ConstInt(0) None)) Return(Var("a" None))))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~(undeclared identifier 'a')~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(Return(Conditional(Constant(ConstInt(1) None) Constant(ConstInt(2) None) Var("a" None) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~(undeclared identifier 'a')~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(Return(Conditional(Constant(ConstInt(1) None) Var("a" None) Constant(ConstInt(2) None) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~(undeclared identifier 'a')~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(Return(Conditional(Var("a" None) Constant(ConstInt(1) None) Constant(ConstInt(2) None) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~(undeclared identifier 'a')~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(Null)
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     true,
     R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(Null)
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("foo" ["a"] Block([
    D(VarDecl(var_decl("a" Constant(ConstInt(3) None) Int None None None None false)))
    S(Return(Var("a" None)))
  ]) FunType([Int] Int) None None [""] false None [] None false))
])
)~~",
     false,
     R"~~(duplicate declaration of 'a')~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(ForStmt(InitExp(None) None Postfix(Increment Var("a" None) None) Null "" None))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~(undeclared identifier 'a')~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("a" None Int None None None None false)))
    D(VarDecl(var_decl("a" None Int None None None None false)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~(duplicate declaration of 'a')~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(Goto("foo"))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     true,
     R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(Goto("foo"))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(SwitchStmt(Constant(ConstInt(1) None) Compound(Block([
      S(DefaultStmt(Return(Var("a" None)) ""))
    ])) "" [] None None))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~(undeclared identifier 'a')~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(Expression(Assignment(Constant(ConstInt(5) None) Constant(ConstInt(3) None) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~(invalid lvalue in assignment: Constant(const=ConstInt(value=5), data_type=None))~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(ForStmt(InitExp(None) Binary(LessThan Var("a" None) Constant(ConstInt(10) None) None) None Null "" None))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~(undeclared identifier 'a')~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("f" ["a" "a"] None FunType([Int Int] Int) None None ["" ""] false None [] None false))
  FunctionDecl(function_decl("main" [] Block([
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~(duplicate parameter name 'a')~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(DoWhileStmt(Expression(Assignment(Var("a" None) Constant(ConstInt(1) None) None)) Constant(ConstInt(1) None) ""))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~(undeclared identifier 'a')~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(Expression(Postfix(Increment Constant(ConstInt(1) None) None)))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~(invalid lvalue in postfix: Constant(const=ConstInt(value=1), data_type=None))~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(FunctionDecl(function_decl("foo" ["a" "a"] None FunType([Int Int] Int) None None ["" ""] false None [] None false)))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~(duplicate parameter name 'a')~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(Compound(Block([
      D(VarDecl(var_decl("a" None Int None None None None false)))
      D(VarDecl(var_decl("a" None Int None None None None false)))
    ])))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~(duplicate declaration of 'a')~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("a" None Int None None None None false)))
    D(VarDecl(var_decl("a" None Int None None None None false)))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~(duplicate declaration of 'a')~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(Expression(Prefix(Increment Constant(ConstInt(1) None) None)))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~(invalid lvalue in prefix increment/decrement: Constant(const=ConstInt(value=1), data_type=None))~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("foo" [] Block([
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) Static None [] false None [] None false))
])
)~~",
     true,
     R"~~(Program([
  FunctionDecl(function_decl("foo" [] Block([
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) Static None [] false None [] None false))
])
)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(Return(Constant(ConstInt(42) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     true,
     R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(Return(Constant(ConstInt(42) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(FunctionDecl(function_decl("foo" [] None FunType([] Int) Static None [] false None [] None false)))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~(static is not allowed on a block-scope function declaration: 'foo')~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(Return(FunctionCall("main" [] None false)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     true,
     R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(Return(FunctionCall("main" [] None false)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(SwitchStmt(Constant(ConstInt(1) None) Compound(Block([
      S(CaseStmt(Constant(ConstInt(0) None) Return(Var("a" None)) ""))
    ])) "" [] None None))
    S(Return(Constant(ConstInt(9) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~(undeclared identifier 'a')~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(SwitchStmt(Var("a" None) Compound(Block([
      S(CaseStmt(Constant(ConstInt(0) None) Return(Constant(ConstInt(0) None)) ""))
    ])) "" [] None None))
    S(Return(Constant(ConstInt(9) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~(undeclared identifier 'a')~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(Compound(Block([
      D(VarDecl(var_decl("a" None Int None None None None false)))
      D(VarDecl(var_decl("a" None Int None None None None false)))
    ])))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~(duplicate declaration of 'a')~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(Expression(CompoundAssignment(Add Constant(ConstInt(1) None) Constant(ConstInt(2) None) None None)))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~(invalid lvalue in compound assignment: Constant(const=ConstInt(value=1), data_type=None))~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("a" None Int None None None None false)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     true,
     R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("@0.a" None Int None None None None false)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"},
    {R"~~(Program([
  VarDecl(var_decl("x" None Int Static None None None false))
  VarDecl(var_decl("x" None Int None None None None false))
  FunctionDecl(function_decl("main" [] Block([
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~(file-scope identifier 'x' declared with external linkage after a prior internal declaration)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("a" None Int None None None None false)))
    S(Compound(Block([
      D(VarDecl(var_decl("a" None Int None None None None false)))
    ])))
    D(VarDecl(var_decl("a" None Int None None None None false)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~(duplicate declaration of 'a')~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("f" [""] Block([
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([Int] Int) None None [""] false None [] None false))
  FunctionDecl(function_decl("main" [] Block([
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~(parameter name omitted in a function definition (C99 §6.9.1.5))~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("a" None Int None None None None false)))
    S(Expression(Assignment(Unary(Negate Var("a" None) None) Constant(ConstInt(5) None) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~(invalid lvalue in assignment: Unary(op=Negate(), exp=Var(name='a', data_type=None), data_type=None))~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("a" Var("a" None) Int None None None None false)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     true,
     R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("@0.a" Var("@0.a" None) Int None None None None false)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("foo" ["x"] Block([
    S(Return(Binary(Add Var("x" None) Constant(ConstInt(1) None) None)))
  ]) FunType([Int] Int) None None [""] false None [] None false))
])
)~~",
     true,
     R"~~(Program([
  FunctionDecl(function_decl("foo" ["@0.x"] Block([
    S(Return(Binary(Add Var("@0.x" None) Constant(ConstInt(1) None) None)))
  ]) FunType([Int] Int) None None [""] false None [] None false))
])
)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" ["x" "y"] Block([
    S(Return(Binary(Add Var("x" None) Var("y" None) None)))
  ]) FunType([Int Int] Int) None None ["" ""] false None [] None false))
])
)~~",
     true,
     R"~~(Program([
  FunctionDecl(function_decl("main" ["@0.x" "@1.y"] Block([
    S(Return(Binary(Add Var("@0.x" None) Var("@1.y" None) None)))
  ]) FunType([Int Int] Int) None None ["" ""] false None [] None false))
])
)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("x" Constant(ConstInt(1) None) Int None None None None false)))
    S(SwitchStmt(Var("x" None) Compound(Block([
      S(CaseStmt(Var("y" None) Return(Constant(ConstInt(0) None)) ""))
    ])) "" [] None None))
    S(Return(Constant(ConstInt(9) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~(undeclared identifier 'y')~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(Expression(Assignment(Binary(Add Constant(ConstInt(1) None) Constant(ConstInt(2) None) None) Constant(ConstInt(3) None) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~(invalid lvalue in assignment: Binary(op=Add(), left=Constant(const=ConstInt(value=1), data_type=None), right=Constant(const=ConstInt(value=2), data_type=None), data_type=None))~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("x" None Int None None None None false)))
    S(Return(Var("x" None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     true,
     R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("@0.x" None Int None None None None false)))
    S(Return(Var("@0.x" None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("a" None Int None None None None false)))
    S(LabeledStmt("foo" Return(Var("a" None))))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     true,
     R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("@0.a" None Int None None None None false)))
    S(LabeledStmt("foo" Return(Var("@0.a" None))))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("x" None Int None None None None false)))
    S(Return(Unary(Negate Var("x" None) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     true,
     R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("@0.x" None Int None None None None false)))
    S(Return(Unary(Negate Var("@0.x" None) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("a" Constant(ConstInt(5) None) Int None None None None false)))
    S(Return(Var("a" None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     true,
     R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("@0.a" Constant(ConstInt(5) None) Int None None None None false)))
    S(Return(Var("@0.a" None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("x" Constant(ConstInt(5) None) Int Static None None None false)))
    S(Return(Var("x" None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     true,
     R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("@0.x" Constant(ConstInt(5) None) Int Static None None None false)))
    S(Return(Var("@0.x" None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("a" None Int None None None None false)))
    S(IfStmt(Var("a" None) Return(Var("a" None)) None))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     true,
     R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("@0.a" None Int None None None None false)))
    S(IfStmt(Var("@0.a" None) Return(Var("@0.a" None)) None))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("i" None Int None None None None false)))
    S(ForStmt(InitExp(None) None None Expression(Var("i" None)) "" None))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     true,
     R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("@0.i" None Int None None None None false)))
    S(ForStmt(InitExp(None) None None Expression(Var("@0.i" None)) "" None))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("foo" [] None FunType([] Int) Static None [] false None [] None false))
  FunctionDecl(function_decl("foo" [] Block([
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) Extern None [] false None [] None false))
])
)~~",
     true,
     R"~~(Program([
  FunctionDecl(function_decl("foo" [] None FunType([] Int) Static None [] false None [] None false))
  FunctionDecl(function_decl("foo" [] Block([
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) Extern None [] false None [] None false))
])
)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("a" None Int None None None None false)))
    D(VarDecl(var_decl("b" None Int None None None None false)))
    D(VarDecl(var_decl("c" None Int None None None None false)))
    S(Expression(Assignment(Assignment(Var("a" None) Var("b" None) None) Var("c" None) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~(invalid lvalue in assignment: Assignment(lval=Var(name='a', data_type=None), rval=Var(name='b', data_type=None), data_type=None))~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(FunctionDecl(function_decl("foo" [] None FunType([] Int) None None [] false None [] None false)))
    S(Return(FunctionCall("foo" [] None false)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     true,
     R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(FunctionDecl(function_decl("foo" [] None FunType([] Int) None None [] false None [] None false)))
    S(Return(FunctionCall("foo" [] None false)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("a" Constant(ConstInt(1) None) Int None None None None false)))
    S(Compound(Block([
      S(Return(Var("a" None)))
    ])))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     true,
     R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("@0.a" Constant(ConstInt(1) None) Int None None None None false)))
    S(Compound(Block([
      S(Return(Var("@0.a" None)))
    ])))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("a" None Int None None None None false)))
    S(Expression(Postfix(Increment Var("a" None) None)))
    S(Return(Var("a" None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     true,
     R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("@0.a" None Int None None None None false)))
    S(Expression(Postfix(Increment Var("@0.a" None) None)))
    S(Return(Var("@0.a" None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("f" ["" ""] None FunType([Int Int] Int) None None ["" ""] false None [] None false))
  FunctionDecl(function_decl("main" [] Block([
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     true,
     R"~~(Program([
  FunctionDecl(function_decl("f" ["" ""] None FunType([Int Int] Int) None None ["" ""] false None [] None false))
  FunctionDecl(function_decl("main" [] Block([
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(WhileStmt(Constant(ConstInt(1) None) Compound(Block([
      S(BreakStmt(""))
      S(ContinueStmt(""))
    ])) ""))
    S(Return(Constant(ConstInt(0) None)))
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
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"},
    {R"~~(Program([
  VarDecl(var_decl("g" None Int None None None None false))
  VarDecl(var_decl("s" None Int Static None None None false))
  FunctionDecl(function_decl("main" [] Block([
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     true,
     R"~~(Program([
  VarDecl(var_decl("g" None Int None None None None false))
  VarDecl(var_decl("s" None Int Static None None None false))
  FunctionDecl(function_decl("main" [] Block([
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("a" Constant(ConstInt(3) None) Int None None None None false)))
    D(VarDecl(var_decl("b" Var("a" None) Int None None None None false)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     true,
     R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("@0.a" Constant(ConstInt(3) None) Int None None None None false)))
    D(VarDecl(var_decl("@1.b" Var("@0.a" None) Int None None None None false)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(FunctionDecl(function_decl("foo" ["x" "y"] None FunType([Int Int] Int) None None ["" ""] false None [] None false)))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     true,
     R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(FunctionDecl(function_decl("foo" ["@0.x" "@1.y"] None FunType([Int Int] Int) None None ["" ""] false None [] None false)))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("a" None Int None None None None false)))
    S(Compound(Block([
      S(Compound(Block([
        S(Return(Var("a" None)))
      ])))
    ])))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     true,
     R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("@0.a" None Int None None None None false)))
    S(Compound(Block([
      S(Compound(Block([
        S(Return(Var("@0.a" None)))
      ])))
    ])))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("a" None Int None None None None false)))
    S(Expression(Assignment(Conditional(Constant(ConstInt(1) None) Constant(ConstInt(2) None) Var("a" None) None) Constant(ConstInt(5) None) None)))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~(invalid lvalue in assignment: Conditional(condition=Constant(const=ConstInt(value=1), data_type=None), true_clause=Constant(const=ConstInt(value=2), data_type=None), false_clause=Var(name='a', data_type=None), data_type=None))~~"},
    {R"~~(Program([
  VarDecl(var_decl("x" Constant(ConstInt(5) None) Int Static None None None false))
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("x" None Int Extern None None None false)))
    S(Return(Var("x" None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     true,
     R"~~(Program([
  VarDecl(var_decl("x" Constant(ConstInt(5) None) Int Static None None None false))
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("x" None Int Extern None None None false)))
    S(Return(Var("x" None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("foo" [] Block([
    S(Return(Constant(ConstInt(1) None)))
  ]) FunType([] Int) None None [] false None [] None false))
  FunctionDecl(function_decl("main" [] Block([
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     true,
     R"~~(Program([
  FunctionDecl(function_decl("foo" [] Block([
    S(Return(Constant(ConstInt(1) None)))
  ]) FunType([] Int) None None [] false None [] None false))
  FunctionDecl(function_decl("main" [] Block([
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("a" None Int None None None None false)))
    S(Expression(CompoundAssignment(Add Var("a" None) Constant(ConstInt(1) None) None None)))
    S(Return(Var("a" None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     true,
     R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("@0.a" None Int None None None None false)))
    S(Expression(CompoundAssignment(Add Var("@0.a" None) Constant(ConstInt(1) None) None None)))
    S(Return(Var("@0.a" None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("foo" ["a"] Block([
    S(Compound(Block([
      D(VarDecl(var_decl("a" Constant(ConstInt(3) None) Int None None None None false)))
      S(Return(Var("a" None)))
    ])))
    S(Return(Var("a" None)))
  ]) FunType([Int] Int) None None [""] false None [] None false))
])
)~~",
     true,
     R"~~(Program([
  FunctionDecl(function_decl("foo" ["@0.a"] Block([
    S(Compound(Block([
      D(VarDecl(var_decl("@1.a" Constant(ConstInt(3) None) Int None None None None false)))
      S(Return(Var("@1.a" None)))
    ])))
    S(Return(Var("@0.a" None)))
  ]) FunType([Int] Int) None None [""] false None [] None false))
])
)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("a" None Int None None None None false)))
    D(VarDecl(var_decl("b" None Int None None None None false)))
    S(Return(Binary(Add Var("a" None) Var("b" None) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     true,
     R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("@0.a" None Int None None None None false)))
    D(VarDecl(var_decl("@1.b" None Int None None None None false)))
    S(Return(Binary(Add Var("@0.a" None) Var("@1.b" None) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("foo" [] Block([
    S(Return(Constant(ConstInt(1) None)))
  ]) FunType([] Int) None None [] false None [] None false))
  FunctionDecl(function_decl("main" [] Block([
    S(Return(FunctionCall("foo" [] None false)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     true,
     R"~~(Program([
  FunctionDecl(function_decl("foo" [] Block([
    S(Return(Constant(ConstInt(1) None)))
  ]) FunType([] Int) None None [] false None [] None false))
  FunctionDecl(function_decl("main" [] Block([
    S(Return(FunctionCall("foo" [] None false)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(FunctionDecl(function_decl("foo" [] None FunType([] Int) None None [] false None [] None false)))
    S(Expression(FunctionCall("foo" [] None false)))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     true,
     R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(FunctionDecl(function_decl("foo" [] None FunType([] Int) None None [] false None [] None false)))
    S(Expression(FunctionCall("foo" [] None false)))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("a" Constant(ConstInt(5) None) Int None None None None false)))
    S(Compound(Block([
      D(VarDecl(var_decl("a" Var("a" None) Int None None None None false)))
    ])))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     true,
     R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("@0.a" Constant(ConstInt(5) None) Int None None None None false)))
    S(Compound(Block([
      D(VarDecl(var_decl("@1.a" Var("@1.a" None) Int None None None None false)))
    ])))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("a" None Int None None None None false)))
    D(VarDecl(var_decl("b" None Int None None None None false)))
    S(Expression(Assignment(Var("a" None) Var("b" None) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     true,
     R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("@0.a" None Int None None None None false)))
    D(VarDecl(var_decl("@1.b" None Int None None None None false)))
    S(Expression(Assignment(Var("@0.a" None) Var("@1.b" None) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(ForStmt(InitDecl([var_decl("i" Constant(ConstInt(0) None) Int None None None None false)]) None None Compound(Block([
      S(Expression(Var("i" None)))
      S(BreakStmt(""))
    ])) "" None))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     true,
     R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(ForStmt(InitDecl([var_decl("@0.i" Constant(ConstInt(0) None) Int None None None None false)]) None None Compound(Block([
      S(Expression(Var("@0.i" None)))
      S(BreakStmt(""))
    ])) "" None))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("a" Constant(ConstInt(1) None) Int None None None None false)))
    S(Null)
    S(Expression(Assignment(Var("a" None) Constant(ConstInt(2) None) None)))
    S(Return(Var("a" None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     true,
     R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("@0.a" Constant(ConstInt(1) None) Int None None None None false)))
    S(Null)
    S(Expression(Assignment(Var("@0.a" None) Constant(ConstInt(2) None) None)))
    S(Return(Var("@0.a" None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(FunctionDecl(function_decl("foo" ["x"] None FunType([Int] Int) None None [""] false None [] None false)))
    D(VarDecl(var_decl("x" None Int None None None None false)))
    S(Return(Var("x" None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     true,
     R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(FunctionDecl(function_decl("foo" ["@0.x"] None FunType([Int] Int) None None [""] false None [] None false)))
    D(VarDecl(var_decl("@1.x" None Int None None None None false)))
    S(Return(Var("@1.x" None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("a" None Int None None None None false)))
    D(FunctionDecl(function_decl("foo" ["a"] None FunType([Int] Int) None None [""] false None [] None false)))
    S(Return(Var("a" None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     true,
     R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("@0.a" None Int None None None None false)))
    D(FunctionDecl(function_decl("foo" ["@1.a"] None FunType([Int] Int) None None [""] false None [] None false)))
    S(Return(Var("@0.a" None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("a" None Int None None None None false)))
    D(VarDecl(var_decl("b" None Int None None None None false)))
    S(IfStmt(Var("a" None) Return(Var("a" None)) Return(Var("b" None))))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     true,
     R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("@0.a" None Int None None None None false)))
    D(VarDecl(var_decl("@1.b" None Int None None None None false)))
    S(IfStmt(Var("@0.a" None) Return(Var("@0.a" None)) Return(Var("@1.b" None))))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("a" Constant(ConstInt(1) None) Int None None None None false)))
    S(Compound(Block([
      D(VarDecl(var_decl("a" Constant(ConstInt(2) None) Int None None None None false)))
    ])))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     true,
     R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("@0.a" Constant(ConstInt(1) None) Int None None None None false)))
    S(Compound(Block([
      D(VarDecl(var_decl("@1.a" Constant(ConstInt(2) None) Int None None None None false)))
    ])))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("a" None Int None None None None false)))
    S(DoWhileStmt(Expression(Assignment(Var("a" None) Constant(ConstInt(1) None) None)) Var("a" None) ""))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     true,
     R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("@0.a" None Int None None None None false)))
    S(DoWhileStmt(Expression(Assignment(Var("@0.a" None) Constant(ConstInt(1) None) None)) Var("@0.a" None) ""))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(FunctionDecl(function_decl("f" ["x"] None FunType([Int] Int) None None [""] false None [] None false)))
    D(VarDecl(var_decl("a" None Int None None None None false)))
    S(Return(FunctionCall("f" [Var("a" None)] None false)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     true,
     R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(FunctionDecl(function_decl("f" ["@0.x"] None FunType([Int] Int) None None [""] false None [] None false)))
    D(VarDecl(var_decl("@1.a" None Int None None None None false)))
    S(Return(FunctionCall("f" [Var("@1.a" None)] None false)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("a" Constant(ConstInt(1) None) Int None None None None false)))
    S(Compound(Block([
      D(VarDecl(var_decl("a" Constant(ConstInt(2) None) Int None None None None false)))
    ])))
    S(Return(Var("a" None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     true,
     R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("@0.a" Constant(ConstInt(1) None) Int None None None None false)))
    S(Compound(Block([
      D(VarDecl(var_decl("@1.a" Constant(ConstInt(2) None) Int None None None None false)))
    ])))
    S(Return(Var("@0.a" None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("a" None Int None None None None false)))
    S(WhileStmt(Var("a" None) Expression(Assignment(Var("a" None) Binary(Add Var("a" None) Constant(ConstInt(1) None) None) None)) ""))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     true,
     R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("@0.a" None Int None None None None false)))
    S(WhileStmt(Var("@0.a" None) Expression(Assignment(Var("@0.a" None) Binary(Add Var("@0.a" None) Constant(ConstInt(1) None) None) None)) ""))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(FunctionDecl(function_decl("foo" [] None FunType([] Int) None None [] false None [] None false)))
    D(FunctionDecl(function_decl("foo" [] None FunType([] Int) None None [] false None [] None false)))
    S(Return(FunctionCall("foo" [] None false)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     true,
     R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(FunctionDecl(function_decl("foo" [] None FunType([] Int) None None [] false None [] None false)))
    D(FunctionDecl(function_decl("foo" [] None FunType([] Int) None None [] false None [] None false)))
    S(Return(FunctionCall("foo" [] None false)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(ForStmt(InitDecl([var_decl("i" Constant(ConstInt(0) None) Int None None None None false)]) None None Compound(Block([
      D(VarDecl(var_decl("i" Constant(ConstInt(1) None) Int None None None None false)))
      S(BreakStmt(""))
    ])) "" None))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     true,
     R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(ForStmt(InitDecl([var_decl("@0.i" Constant(ConstInt(0) None) Int None None None None false)]) None None Compound(Block([
      D(VarDecl(var_decl("@1.i" Constant(ConstInt(1) None) Int None None None None false)))
      S(BreakStmt(""))
    ])) "" None))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(ForStmt(InitDecl([var_decl("i" Constant(ConstInt(0) None) Int None None None None false)]) None None Compound(Block([
      D(VarDecl(var_decl("i" Constant(ConstInt(5) None) Int None None None None false)))
      S(BreakStmt(""))
    ])) "" None))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     true,
     R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(ForStmt(InitDecl([var_decl("@0.i" Constant(ConstInt(0) None) Int None None None None false)]) None None Compound(Block([
      D(VarDecl(var_decl("@1.i" Constant(ConstInt(5) None) Int None None None None false)))
      S(BreakStmt(""))
    ])) "" None))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("a" None Int None None None None false)))
    S(Compound(Block([
      D(VarDecl(var_decl("a" None Int None None None None false)))
    ])))
    S(Compound(Block([
      D(VarDecl(var_decl("a" None Int None None None None false)))
    ])))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     true,
     R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("@0.a" None Int None None None None false)))
    S(Compound(Block([
      D(VarDecl(var_decl("@1.a" None Int None None None None false)))
    ])))
    S(Compound(Block([
      D(VarDecl(var_decl("@2.a" None Int None None None None false)))
    ])))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("a" None Int None None None None false)))
    D(VarDecl(var_decl("b" None Int None None None None false)))
    D(VarDecl(var_decl("c" None Int None None None None false)))
    S(Return(Conditional(Var("a" None) Var("b" None) Var("c" None) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     true,
     R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("@0.a" None Int None None None None false)))
    D(VarDecl(var_decl("@1.b" None Int None None None None false)))
    D(VarDecl(var_decl("@2.c" None Int None None None None false)))
    S(Return(Conditional(Var("@0.a" None) Var("@1.b" None) Var("@2.c" None) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("a" Constant(ConstInt(1) None) Int None None None None false)))
    S(Compound(Block([
      D(VarDecl(var_decl("a" Constant(ConstInt(2) None) Int None None None None false)))
      S(Compound(Block([
        S(Return(Var("a" None)))
      ])))
    ])))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     true,
     R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("@0.a" Constant(ConstInt(1) None) Int None None None None false)))
    S(Compound(Block([
      D(VarDecl(var_decl("@1.a" Constant(ConstInt(2) None) Int None None None None false)))
      S(Compound(Block([
        S(Return(Var("@1.a" None)))
      ])))
    ])))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(ForStmt(InitDecl([var_decl("i" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var("i" None) Constant(ConstInt(10) None) None) Postfix(Increment Var("i" None) None) Expression(Var("i" None)) "" None))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     true,
     R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(ForStmt(InitDecl([var_decl("@0.i" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var("@0.i" None) Constant(ConstInt(10) None) None) Postfix(Increment Var("@0.i" None) None) Expression(Var("@0.i" None)) "" None))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("a" Constant(ConstInt(1) None) Int None None None None false)))
    S(SwitchStmt(Var("a" None) Compound(Block([
      S(CaseStmt(Constant(ConstInt(0) None) Return(Constant(ConstInt(0) None)) ""))
    ])) "" [] None None))
    S(Return(Constant(ConstInt(9) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     true,
     R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("@0.a" Constant(ConstInt(1) None) Int None None None None false)))
    S(SwitchStmt(Var("@0.a" None) Compound(Block([
      S(CaseStmt(Constant(ConstInt(0) None) Return(Constant(ConstInt(0) None)) ""))
    ])) "" [] None None))
    S(Return(Constant(ConstInt(9) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("a" None Int None None None None false)))
    D(VarDecl(var_decl("b" None Int None None None None false)))
    D(VarDecl(var_decl("c" None Int None None None None false)))
    S(Expression(Assignment(Var("a" None) Assignment(Var("b" None) Var("c" None) None) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     true,
     R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("@0.a" None Int None None None None false)))
    D(VarDecl(var_decl("@1.b" None Int None None None None false)))
    D(VarDecl(var_decl("@2.c" None Int None None None None false)))
    S(Expression(Assignment(Var("@0.a" None) Assignment(Var("@1.b" None) Var("@2.c" None) None) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("i" None Int None None None None false)))
    S(ForStmt(InitExp(Assignment(Var("i" None) Constant(ConstInt(0) None) None)) Binary(LessThan Var("i" None) Constant(ConstInt(5) None) None) Postfix(Increment Var("i" None) None) Expression(Var("i" None)) "" None))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     true,
     R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("@0.i" None Int None None None None false)))
    S(ForStmt(InitExp(Assignment(Var("@0.i" None) Constant(ConstInt(0) None) None)) Binary(LessThan Var("@0.i" None) Constant(ConstInt(5) None) None) Postfix(Increment Var("@0.i" None) None) Expression(Var("@0.i" None)) "" None))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(FunctionDecl(function_decl("f" ["x"] None FunType([Int] Int) None None [""] false None [] None false)))
    D(VarDecl(var_decl("a" Constant(ConstInt(5) None) Int None None None None false)))
    S(Expression(FunctionCall("f" [Binary(Add Var("a" None) Constant(ConstInt(1) None) None)] None false)))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     true,
     R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(FunctionDecl(function_decl("f" ["@0.x"] None FunType([Int] Int) None None [""] false None [] None false)))
    D(VarDecl(var_decl("@1.a" Constant(ConstInt(5) None) Int None None None None false)))
    S(Expression(FunctionCall("f" [Binary(Add Var("@1.a" None) Constant(ConstInt(1) None) None)] None false)))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("a" None Int None None None None false)))
    S(ForStmt(InitDecl([var_decl("a" Constant(ConstInt(1) None) Int None None None None false)]) Binary(LessThan Var("a" None) Constant(ConstInt(10) None) None) Assignment(Var("a" None) Binary(Add Var("a" None) Constant(ConstInt(1) None) None) None) Expression(Assignment(Var("a" None) Binary(Add Var("a" None) Constant(ConstInt(5) None) None) None)) "" None))
    S(Return(Var("a" None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     true,
     R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("@0.a" None Int None None None None false)))
    S(ForStmt(InitDecl([var_decl("@1.a" Constant(ConstInt(1) None) Int None None None None false)]) Binary(LessThan Var("@1.a" None) Constant(ConstInt(10) None) None) Assignment(Var("@1.a" None) Binary(Add Var("@1.a" None) Constant(ConstInt(1) None) None) None) Expression(Assignment(Var("@1.a" None) Binary(Add Var("@1.a" None) Constant(ConstInt(5) None) None) None)) "" None))
    S(Return(Var("@0.a" None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"},
    {R"~~(Program([
  VarDecl(var_decl("__c6502_stdc_count_ones" None Int None None None None false))
])
)~~",
     false,
     R"~~(__c6502_stdc_count_ones is a builtin and can't be declared)~~"},
    {R"~~(Program([
  VarDecl(var_decl("p" Var("__c6502_bcd_add" None) Pointer(Void) None None None None false))
])
)~~",
     false,
     R"~~(__c6502_bcd_add is a builtin, which can only be called)~~"},
    {R"~~(Program([
  VarDecl(var_decl("p" Var("__c6502_bin_to_bcd" None) Pointer(Void) None None None None false))
])
)~~",
     false,
     R"~~(__c6502_bin_to_bcd is a builtin, which can only be called)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("f" ["__c6502_bcd_add"] Block([]) FunType([Int] Void) None None [""] false None [] None false))
])
)~~",
     false,
     R"~~(__c6502_bcd_add is a builtin and can't be declared)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("__c6502_bin_to_bcd" ["x"] None FunType([UInt] ULong) None None [""] false None [] None false))
])
)~~",
     false,
     R"~~(__c6502_bin_to_bcd is a builtin and can't be declared)~~"},
    {R"~~(Program([
  VarDecl(var_decl("p" Var("__c6502_stdc_count_ones_uc" None) Pointer(FunType([UChar] UInt)) None None None None false))
])
)~~",
     false,
     R"~~(__c6502_stdc_count_ones_uc is a builtin, which can only be called)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(LabeledStmt("lbl" Expression(Dereference(Var("lbl" None) None))))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~(undeclared identifier 'lbl')~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("f" [] Block([
    D(VarDecl(var_decl("__c6502_bcd_sub" Constant(ConstInt(1) None) Int None None None None false)))
  ]) FunType([] Void) None None [] false None [] None false))
])
)~~",
     false,
     R"~~(__c6502_bcd_sub is a builtin and can't be declared)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(Return(Prefix(Decrement Constant(ConstInt(3) None) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~(invalid lvalue in prefix increment/decrement: Constant(const=ConstInt(value=3), data_type=None))~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(IfStmt(Constant(ConstInt(1) None) Return(Var("c" None)) None))
    D(VarDecl(var_decl("c" Constant(ConstInt(0) None) Int None None None None false)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~(undeclared identifier 'c')~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("x" Constant(ConstInt(1) None) Int None None None None false)))
    D(VarDecl(var_decl("x" None Int Static None None None false)))
    S(Return(Var("x" None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~(duplicate declaration of 'x')~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(FunctionDecl(function_decl("f" ["a"] None FunType([Int] Int) None None [""] false None [] None false)))
    S(Return(FunctionCall("f" [FunctionCall("bar" [] None false)] None false)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~(undeclared identifier 'bar')~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("ptr" AddressOf(Constant(ConstInt(10) None) None) Pointer(Int) None None None None false)))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~(invalid operand of unary '&': Constant(const=ConstInt(value=10), data_type=None))~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("x" [] None FunType([] Int) None None [] false None [] None false))
  FunctionDecl(function_decl("main" [] Block([
    S(Expression(Postfix(Decrement FunctionCall("x" [] None false) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~(invalid lvalue in postfix: FunctionCall(name='x', args=[], data_type=None, hw_stack_save=False))~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("a" Constant(ConstInt(10) None) Int None None None None false)))
    S(Return(Postfix(Decrement Postfix(Increment Var("a" None) None) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~(invalid lvalue in postfix: Postfix(op=Increment(), operand=Var(name='a', data_type=None), data_type=None))~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("x" Constant(ConstInt(0) None) Int None None None None false)))
    S(Expression(Assignment(AddressOf(Var("x" None) None) Constant(ConstInt(10) None) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~(invalid lvalue in assignment: AddressOf(exp=Var(name='x', data_type=None), data_type=None))~~"},
    {R"~~(Program([
  VarDecl(var_decl("foo" None Int Static None None None false))
  FunctionDecl(function_decl("main" [] Block([
    S(Return(Var("foo" None)))
  ]) FunType([] Int) None None [] false None [] None false))
  VarDecl(var_decl("foo" Constant(ConstInt(3) None) Int None None None None false))
])
)~~",
     false,
     R"~~(file-scope identifier 'foo' declared with external linkage after a prior internal declaration)~~"},
    {R"~~(Program([
  VarDecl(var_decl("i" Constant(ConstInt(10) None) Int None None None None false))
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("i" None Int Extern None None None false)))
    D(VarDecl(var_decl("i" None Int None None None None false)))
    S(Return(Var("i" None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~(duplicate declaration of 'i')~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("x" [] None FunType([] Int) None None [] false None [] None false))
  FunctionDecl(function_decl("main" [] Block([
    S(Expression(Prefix(Increment FunctionCall("x" [] None false) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~(invalid lvalue in prefix increment/decrement: FunctionCall(name='x', args=[], data_type=None, hw_stack_save=False))~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("a" Constant(ConstInt(2) None) Int None None None None false)))
    S(Expression(Assignment(Unary(LogicalNot Var("a" None) None) Constant(ConstInt(3) None) None)))
    S(Return(Var("a" None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~(invalid lvalue in assignment: Unary(op=LogicalNot(), exp=Var(name='a', data_type=None), data_type=None))~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("x" [] None FunType([] Int) None None [] false None [] None false))
  FunctionDecl(function_decl("main" [] Block([
    S(Expression(Assignment(FunctionCall("x" [] None false) Constant(ConstInt(1) None) None)))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~(invalid lvalue in assignment: FunctionCall(name='x', args=[], data_type=None, hw_stack_save=False))~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("a" Constant(ConstInt(0) None) Int None None None None false)))
    S(Expression(Postfix(Increment Assignment(Var("a" None) Constant(ConstInt(4) None) None) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~(invalid lvalue in postfix: Assignment(lval=Var(name='a', data_type=None), rval=Constant(const=ConstInt(value=4), data_type=None), data_type=None))~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("a" Constant(ConstInt(0) None) Int None None None None false)))
    S(Expression(CompoundAssignment(Add Unary(Negate Var("a" None) None) Constant(ConstInt(1) None) None None)))
    S(Return(Var("a" None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~(invalid lvalue in compound assignment: Unary(op=Negate(), exp=Var(name='a', data_type=None), data_type=None))~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(ForStmt(InitExp(Assignment(Var("i" None) Constant(ConstInt(0) None) None)) Binary(LessThan Var("i" None) Constant(ConstInt(1) None) None) Assignment(Var("i" None) Binary(Add Var("i" None) Constant(ConstInt(1) None) None) None) Compound(Block([
      S(Return(Constant(ConstInt(0) None)))
    ])) "" None))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~(undeclared identifier 'i')~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("x" [] None FunType([] Int) None None [] false None [] None false))
  FunctionDecl(function_decl("main" [] Block([
    S(Expression(CompoundAssignment(Add FunctionCall("x" [] None false) Constant(ConstInt(1) None) None None)))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~(invalid lvalue in compound assignment: FunctionCall(name='x', args=[], data_type=None, hw_stack_save=False))~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("i" Constant(ConstInt(0) None) Int None None None None false)))
    S(Expression(Assignment(Var("i" None) Assignment(Cast(Long Var("i" None) None) Constant(ConstInt(10) None) None) None)))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~(invalid lvalue in assignment: Cast(target_type=Long(), exp=Var(name='i', data_type=None), data_type=None))~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("i" Constant(ConstInt(10) None) Int None None None None false)))
    D(VarDecl(var_decl("ptr" AddressOf(Prefix(Increment Var("i" None) None) None) Pointer(Int) None None None None false)))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~(invalid operand of unary '&': Prefix(op=Increment(), operand=Var(name='i', data_type=None), data_type=None))~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("i" Constant(ConstInt(10) None) Int None None None None false)))
    D(VarDecl(var_decl("ptr" AddressOf(Postfix(Decrement Var("i" None) None) None) Pointer(Int) None None None None false)))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~(invalid operand of unary '&': Postfix(op=Decrement(), operand=Var(name='i', data_type=None), data_type=None))~~"},
    {R"~~(Program([
  VarDecl(var_decl("x" None Pointer(Void) Extern None None None false))
  FunctionDecl(function_decl("main" [] Block([
    S(Expression(Prefix(Increment Postfix(Decrement Dereference(Var("x" None) None) None) None)))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~(invalid lvalue in prefix increment/decrement: Postfix(op=Decrement(), operand=Dereference(exp=Var(name='x', data_type=None), data_type=None), data_type=None))~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("foo" [] None FunType([] Int) None None [] false None [] None false))
  FunctionDecl(function_decl("main" [] Block([
    S(Return(FunctionCall("foo" [] None false)))
  ]) FunType([] Int) None None [] false None [] None false))
  FunctionDecl(function_decl("foo" [] Block([
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) Static None [] false None [] None false))
])
)~~",
     false,
     R"~~(file-scope identifier 'foo' declared with internal linkage after a prior external declaration)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("a" Constant(ConstInt(2) None) Int None None None None false)))
    S(Expression(Assignment(Binary(Add Var("a" None) Constant(ConstInt(3) None) None) Constant(ConstInt(4) None) None)))
    S(Return(Var("a" None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~(invalid lvalue in assignment: Binary(op=Add(), left=Var(name='a', data_type=None), right=Constant(const=ConstInt(value=3), data_type=None), data_type=None))~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("a" Constant(ConstInt(1) None) Int None None None None false)))
    S(Expression(Prefix(Increment Binary(Add Var("a" None) Constant(ConstInt(1) None) None) None)))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~(invalid lvalue in prefix increment/decrement: Binary(op=Add(), left=Var(name='a', data_type=None), right=Constant(const=ConstInt(value=1), data_type=None), data_type=None))~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(FunctionDecl(function_decl("foo" [] None FunType([] Int) None None [] false None [] None false)))
    D(VarDecl(var_decl("foo" Constant(ConstInt(1) None) Int None None None None false)))
    S(Return(Var("foo" None)))
  ]) FunType([] Int) None None [] false None [] None false))
  FunctionDecl(function_decl("foo" [] Block([
    S(Return(Constant(ConstInt(1) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~(duplicate declaration of 'foo')~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("x" Constant(ConstInt(0) None) Int None None None None false)))
    D(VarDecl(var_decl("y" AddressOf(Var("x" None) None) Pointer(Int) None None None None false)))
    D(VarDecl(var_decl("z" AddressOf(AddressOf(Var("x" None) None) None) Pointer(Pointer(Int)) None None None None false)))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~(invalid operand of unary '&': AddressOf(exp=Var(name='x', data_type=None), data_type=None))~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("a" Constant(ConstInt(10) None) Int None None None None false)))
    S(Expression(CompoundAssignment(Subtract CompoundAssignment(Add Var("a" None) Constant(ConstInt(1) None) None None) Constant(ConstInt(2) None) None None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~(invalid lvalue in compound assignment: CompoundAssignment(op=Add(), lval=Var(name='a', data_type=None), rval=Constant(const=ConstInt(value=1), data_type=None), intermediate_type=None, data_type=None))~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("x" Constant(ConstInt(0) None) Int None None None None false)))
    D(VarDecl(var_decl("y" Constant(ConstInt(0) None) Int None None None None false)))
    D(VarDecl(var_decl("ptr" AddressOf(Assignment(Var("x" None) Var("y" None) None) None) Pointer(Int) None None None None false)))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~(invalid operand of unary '&': Assignment(lval=Var(name='x', data_type=None), rval=Var(name='y', data_type=None), data_type=None))~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("a" Constant(ConstInt(1) None) Int None None None None false)))
    D(VarDecl(var_decl("b" Constant(ConstInt(2) None) Int None None None None false)))
    S(Expression(Assignment(Var("a" None) Assignment(Binary(Multiply Constant(ConstInt(3) None) Var("b" None) None) Var("a" None) None) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~(invalid lvalue in assignment: Binary(op=Multiply(), left=Constant(const=ConstInt(value=3), data_type=None), right=Var(name='b', data_type=None), data_type=None))~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("i" Constant(ConstInt(100) None) Int None None None None false)))
    D(VarDecl(var_decl("ptr" AddressOf(CompoundAssignment(Add Var("i" None) Constant(ConstInt(200) None) None None) None) Pointer(Int) None None None None false)))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~(invalid operand of unary '&': CompoundAssignment(op=Add(), lval=Var(name='i', data_type=None), rval=Constant(const=ConstInt(value=200), data_type=None), intermediate_type=None, data_type=None))~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("a" Constant(ConstInt(1) None) Int None None None None false)))
    S(SwitchStmt(Var("a" None) Compound(Block([
      D(VarDecl(var_decl("b" Constant(ConstInt(2) None) Int None None None None false)))
      S(CaseStmt(Constant(ConstInt(0) None) Expression(Assignment(Var("a" None) Constant(ConstInt(3) None) None)) ""))
      D(VarDecl(var_decl("b" Constant(ConstInt(2) None) Int None None None None false)))
    ])) "" [] None None))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~(duplicate declaration of 'b')~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("x" Constant(ConstInt(1) None) Int None None None None false)))
    D(VarDecl(var_decl("y" Constant(ConstInt(2) None) Int None None None None false)))
    D(VarDecl(var_decl("z" Constant(ConstInt(3) None) Int None None None None false)))
    D(VarDecl(var_decl("ptr" AddressOf(Conditional(Var("x" None) Var("y" None) Var("z" None) None) None) Pointer(Int) None None None None false)))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~(invalid operand of unary '&': Conditional(condition=Var(name='x', data_type=None), true_clause=Var(name='y', data_type=None), false_clause=Var(name='z', data_type=None), data_type=None))~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("i" Constant(ConstInt(100) None) Int None None None None false)))
    D(VarDecl(var_decl("ptr" AddressOf(Var("i" None) None) Pointer(Int) None None None None false)))
    D(VarDecl(var_decl("ptr2" AddressOf(CompoundAssignment(Subtract Dereference(Var("ptr" None) None) Constant(ConstInt(10) None) None None) None) Pointer(Int) None None None None false)))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~(invalid operand of unary '&': CompoundAssignment(op=Subtract(), lval=Dereference(exp=Var(name='ptr', data_type=None), data_type=None), rval=Constant(const=ConstInt(value=10), data_type=None), intermediate_type=None, data_type=None))~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("a" Constant(ConstInt(2) None) Int None None None None false)))
    D(VarDecl(var_decl("b" Constant(ConstInt(1) None) Int None None None None false)))
    S(Expression(Assignment(Conditional(Binary(GreaterThan Var("a" None) Var("b" None) None) Assignment(Var("a" None) Constant(ConstInt(1) None) None) Var("a" None) None) Constant(ConstInt(0) None) None)))
    S(Return(Var("a" None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~(invalid lvalue in assignment: Conditional(condition=Binary(op=GreaterThan(), left=Var(name='a', data_type=None), right=Var(name='b', data_type=None), data_type=None), true_clause=Assignment(lval=Var(name='a', data_type=None), rval=Constant(const=ConstInt(value=1), data_type=None), data_type=None), false_clause=Var(name='a', data_type=None), data_type=None))~~"},
    {R"~~(Program([
  StructDecl(struct_decl("inner" true [member_decl("y" Int) member_decl("z" Long)]))
  StructDecl(struct_decl("u" true [member_decl("x" Int) member_decl("i" Union("inner"))]))
  FunctionDecl(function_decl("return_union" [] Block([
    D(VarDecl(var_decl("result" InitList([Constant(ConstInt(1) None)] None None) Union("u") None None None None false)))
    S(Return(Var("result" None)))
  ]) FunType([] Union("u")) None None [] false None [] None false))
  FunctionDecl(function_decl("main" [] Block([
    S(Expression(Assignment(Dot(Dot(FunctionCall("return_union" [] None false) "i" None) "y" None) Constant(ConstInt(1) None) None)))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~(invalid lvalue in assignment: Dot(operand=Dot(operand=FunctionCall(name='return_union', args=[], data_type=None, hw_stack_save=False), member='i', data_type=None), member='y', data_type=None))~~"},
    {R"~~(Program([
  StructDecl(struct_decl("u" true [member_decl("arr" Array(Int 3)) member_decl("d" Double)]))
  FunctionDecl(function_decl("get_union" [] Block([
    D(VarDecl(var_decl("result" InitList([InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None) Constant(ConstInt(3) None)] None None) Constant(ConstDouble(4616189618054758400) None)] None None) Union("u") None None None None false)))
    S(Return(Var("result" None)))
  ]) FunType([] Union("u")) None None [] false None [] None false))
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("ptr" AddressOf(Dot(FunctionCall("get_union" [] None false) "arr" None) None) Array(Pointer(Int) 3) None None None None false)))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~(invalid operand of unary '&': Dot(operand=FunctionCall(name='get_union', args=[], data_type=None, hw_stack_save=False), member='arr', data_type=None))~~"},
    {R"~~(Program([
  StructDecl(struct_decl("inner" false [member_decl("x" Int) member_decl("y" Int)]))
  StructDecl(struct_decl("outer" false [member_decl("a" Int) member_decl("b" Structure("inner"))]))
  FunctionDecl(function_decl("return_struct" [] Block([
    D(VarDecl(var_decl("result" InitList([Constant(ConstInt(1) None) InitList([Constant(ConstInt(2) None) Constant(ConstInt(3) None)] None None)] None None) Structure("outer") None None None None false)))
    S(Return(Var("result" None)))
  ]) FunType([] Structure("outer")) None None [] false None [] None false))
  FunctionDecl(function_decl("main" [] Block([
    S(Expression(Assignment(Dot(Dot(FunctionCall("return_struct" [] None false) "b" None) "x" None) Constant(ConstInt(10) None) None)))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~(invalid lvalue in assignment: Dot(operand=Dot(operand=FunctionCall(name='return_struct', args=[], data_type=None, hw_stack_save=False), member='b', data_type=None), member='x', data_type=None))~~"},
    {R"~~(Program([
  StructDecl(struct_decl("s" false [member_decl("arr" Array(Int 3)) member_decl("d" Double)]))
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("x" InitList([InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None) Constant(ConstInt(3) None)] None None) Constant(ConstDouble(4616189618054758400) None)] None None) Structure("s") None None None None false)))
    D(VarDecl(var_decl("y" InitList([InitList([Constant(ConstInt(9) None) Constant(ConstInt(8) None) Constant(ConstInt(7) None)] None None) Constant(ConstDouble(4618441417868443648) None)] None None) Structure("s") None None None None false)))
    S(Expression(Assignment(Dot(Conditional(Constant(ConstInt(1) None) Var("x" None) Var("y" None) None) "d" None) Constant(ConstDouble(0) None) None)))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~(invalid lvalue in assignment: Dot(operand=Conditional(condition=Constant(const=ConstInt(value=1), data_type=None), true_clause=Var(name='x', data_type=None), false_clause=Var(name='y', data_type=None), data_type=None), member='d', data_type=None))~~"},
    {R"~~(Program([
  StructDecl(struct_decl("s" false [member_decl("arr" Array(Int 3)) member_decl("d" Double)]))
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("x" InitList([InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None) Constant(ConstInt(3) None)] None None) Constant(ConstDouble(4616189618054758400) None)] None None) Structure("s") None None None None false)))
    D(VarDecl(var_decl("y" InitList([InitList([Constant(ConstInt(9) None) Constant(ConstInt(8) None) Constant(ConstInt(7) None)] None None) Constant(ConstDouble(4618441417868443648) None)] None None) Structure("s") None None None None false)))
    D(VarDecl(var_decl("arr" AddressOf(Dot(Conditional(Constant(ConstInt(1) None) Var("x" None) Var("y" None) None) "arr" None) None) Array(Pointer(Int) 3) None None None None false)))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~(invalid operand of unary '&': Dot(operand=Conditional(condition=Constant(const=ConstInt(value=1), data_type=None), true_clause=Var(name='x', data_type=None), false_clause=Var(name='y', data_type=None), data_type=None), member='arr', data_type=None))~~"},
};

}  // namespace

TEST_CASE("identifier resolution gives the Python unit tests' results") {
  for (const Case& c : kCases) {
    auto prog = c6502::asdl::load<c6502::c99_ast::Program>(c.input);
    std::string got;
    try {
      got = c6502::asdl::dump(c6502::passes::resolve_identifiers(std::move(prog)));
    } catch (const c6502::CompileError& e) {
      CHECK(e.python_type() == "IdentifierResolutionError");
      got = e.what();
    }
    CHECK(got == c.expected);
  }
}
