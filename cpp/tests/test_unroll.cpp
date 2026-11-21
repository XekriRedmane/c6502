// The calls of the unroller that the Python unit tests make, each
// recorded with Python's result: the input program's dump, and the
// output's dump or the message of the UnrollError. They are the
// distinct calls of tests/test_unroll.py whose records are short enough
// to keep here, and from the rest of the suite one call for each other
// message the pass raised. No program of the corpus is rejected by the
// unroller, so every message is checked here. Two of the calls are the
// loops over a `short` and an `unsigned short` that come back to 0
// after 256 passes, for which the Python pass raised a KeyError until
// its table of type names for the message had both types.
#include <string>

#include "c99_ast.h"
#include "doctest.h"
#include "errors.h"
#include "passes/optimization_ast/unroll.h"

namespace {

struct Case {
  const char* input;
  bool ok;
  const char* expected;
};

const Case kCases[] = {
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(ForStmt(InitDecl([var_decl("i" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var("i" None) Constant(ConstInt(4) None) None) None Expression(Var("i" None)) "" "unroll"))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~(unroll: for-loop post-clause is required)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(ForStmt(InitDecl([var_decl("i" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var("i" None) Constant(ConstInt(257) None) None) Postfix(Increment Var("i" None) None) Expression(Var("i" None)) "" "unroll"))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~(unroll: iteration count exceeds cap of 256)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(ForStmt(InitDecl([var_decl("i" Constant(ConstInt(0) None) Int None None None None false)]) Binary(Equal Var("i" None) Constant(ConstInt(0) None) None) Postfix(Increment Var("i" None) None) Expression(Var("i" None)) "" "unroll"))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~(unroll: comparison op Equal not supported (use <, <=, >, or >=))~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(ForStmt(InitDecl([var_decl("i" Constant(ConstInt(0) None) Int None None None None false)]) Binary(NotEqual Var("i" None) Constant(ConstInt(4) None) None) Postfix(Increment Var("i" None) None) Expression(Var("i" None)) "" "unroll"))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~(unroll: comparison op NotEqual not supported (use <, <=, >, or >=))~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(ForStmt(InitDecl([var_decl("i" Constant(ConstInt(0) None) Int None None None None false)]) Binary(GreaterThan Constant(ConstInt(4) None) Var("i" None) None) Postfix(Increment Var("i" None) None) Expression(Var("i" None)) "" "unroll"))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~(unroll: condition's left operand must be the induction variable `i`)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(ForStmt(InitDecl([var_decl("i" Constant(ConstInt(0) None) Int None None None None false)]) None Postfix(Increment Var("i" None) None) Compound(Block([
      S(IfStmt(Binary(GreaterThan Var("i" None) Constant(ConstInt(4) None) None) BreakStmt("") None))
    ])) "" "unroll"))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~(unroll: for-loop condition is required)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(ForStmt(InitDecl([var_decl("i" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var("i" None) Constant(ConstInt(4) None) None) CompoundAssignment(Add Var("i" None) Constant(ConstInt(0) None) None None) Expression(Var("i" None)) "" "unroll"))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~(unroll: post-clause step must be a positive integer constant)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(ForStmt(InitDecl([var_decl("i" Constant(ConstInt(0) None) SChar None None None None false)]) Binary(LessThan Var("i" None) Constant(ConstInt(200) None) None) Postfix(Increment Var("i" None) None) Expression(Var("i" None)) "" "unroll"))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~(unroll: loop never terminates (`i` wraps at the width of `signed char` and the condition stays true))~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(ForStmt(InitDecl([var_decl("i" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var("i" None) Constant(ConstInt(4) None) None) Postfix(Increment Var("i" None) None) Compound(Block([
      S(Expression(Postfix(Increment Var("i" None) None)))
    ])) "" "unroll"))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~(unroll: induction variable `i` is modified in body)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(ForStmt(InitDecl([var_decl("i" Constant(ConstInt(0) None) UChar None None None None false)]) Binary(LessOrEqual Var("i" None) Constant(ConstInt(255) None) None) Postfix(Increment Var("i" None) None) Expression(Var("i" None)) "" "unroll"))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~(unroll: loop never terminates (`i` wraps at the width of `unsigned char` and the condition stays true))~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(ForStmt(InitDecl([var_decl("i" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var("i" None) Constant(ConstInt(4) None) None) Postfix(Increment Var("i" None) None) Compound(Block([
      S(Goto("out"))
    ])) "" "unroll"))
    S(LabeledStmt("out" Null))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~(unroll: `goto` inside an unrolled loop body is not supported)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(ForStmt(InitDecl([var_decl("i" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var("i" None) Constant(ConstInt(4) None) None) Postfix(Increment Var("i" None) None) Compound(Block([
      S(LabeledStmt("lbl" Expression(Var("i" None))))
    ])) "" "unroll"))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~(unroll: labeled statements inside an unrolled loop body are not supported)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(ForStmt(InitDecl([var_decl("i" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var("i" None) Constant(ConstInt(4) None) None) Postfix(Increment Var("i" None) None) Compound(Block([
      S(Expression(Assignment(Var("i" None) Constant(ConstInt(5) None) None)))
    ])) "" "unroll"))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~(unroll: induction variable `i` is reassigned in body)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("x" None Int None None None None false)))
    S(ForStmt(InitDecl([var_decl("i" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var("x" None) Constant(ConstInt(4) None) None) Postfix(Increment Var("i" None) None) Expression(Var("i" None)) "" "unroll"))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~(unroll: condition's left operand must be the induction variable `i`)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("i" None Int None None None None false)))
    S(ForStmt(InitExp(Assignment(Var("i" None) Constant(ConstInt(0) None) None)) Binary(LessThan Var("i" None) Constant(ConstInt(4) None) None) Postfix(Increment Var("i" None) None) Expression(Var("i" None)) "" "unroll"))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~(unroll: for-init must declare the induction variable (`for (T i = <const>; ...; ...)`))~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("x" Constant(ConstInt(4) None) Int None None None None false)))
    S(ForStmt(InitDecl([var_decl("i" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var("i" None) Var("x" None) None) Postfix(Increment Var("i" None) None) Expression(Var("i" None)) "" "unroll"))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~(unroll: condition's right operand must be an integer constant)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(ForStmt(InitDecl([var_decl("i" Constant(ConstInt(0) None) Short None None None None false)]) Binary(LessOrEqual Var("i" None) Constant(ConstInt(32767) None) None) CompoundAssignment(Add Var("i" None) Constant(ConstInt(256) None) None None) Expression(Var("i" None)) "" "unroll"))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~(unroll: loop never terminates (`i` wraps at the width of `short` and the condition stays true))~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("x" Constant(ConstInt(0) None) Int None None None None false)))
    S(ForStmt(InitDecl([var_decl("i" Var("x" None) Int None None None None false)]) Binary(LessThan Var("i" None) Constant(ConstInt(4) None) None) Postfix(Increment Var("i" None) None) Expression(Var("i" None)) "" "unroll"))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~(unroll: induction variable must be initialized to an integer constant)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(ForStmt(InitDecl([var_decl("i" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var("i" None) Constant(ConstInt(4) None) None) Postfix(Increment Var("i" None) None) Compound(Block([
      S(Expression(CompoundAssignment(Add Var("i" None) Constant(ConstInt(5) None) None None)))
    ])) "" "unroll"))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~(unroll: induction variable `i` is modified in body)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(ForStmt(InitDecl([var_decl("i" Constant(ConstInt(0) None) UShort None None None None false)]) Binary(LessThan Var("i" None) Constant(ConstUInt(65535) None) None) CompoundAssignment(Add Var("i" None) Constant(ConstInt(256) None) None None) Expression(Var("i" None)) "" "unroll"))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~(unroll: loop never terminates (`i` wraps at the width of `unsigned short` and the condition stays true))~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(ForStmt(InitDecl([var_decl("i" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var("i" None) Constant(ConstInt(4) None) None) Postfix(Increment Var("i" None) None) Compound(Block([
      D(VarDecl(var_decl("i" Constant(ConstInt(99) None) Int None None None None false)))
    ])) "" "unroll"))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~(unroll: induction variable `i` is shadowed by an inner declaration)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(ForStmt(InitDecl([var_decl("i" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var("i" None) Constant(ConstInt(4) None) None) Postfix(Increment Var("i" None) None) Compound(Block([
      S(IfStmt(Binary(Equal Var("i" None) Constant(ConstInt(2) None) None) BreakStmt("") None))
    ])) "" "unroll"))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~(unroll: `break` inside an unrolled loop body is not supported)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(ForStmt(InitDecl([var_decl("i" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var("i" None) Constant(ConstInt(4) None) None) Postfix(Increment Var("i" None) None) Compound(Block([
      S(IfStmt(Binary(Equal Var("i" None) Constant(ConstInt(2) None) None) ContinueStmt("") None))
    ])) "" "unroll"))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~(unroll: `continue` inside an unrolled loop body is not supported)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("x" Constant(ConstInt(1) None) Int None None None None false)))
    S(ForStmt(InitDecl([var_decl("i" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var("i" None) Constant(ConstInt(4) None) None) CompoundAssignment(Add Var("i" None) Var("x" None) None None) Expression(Var("i" None)) "" "unroll"))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~(unroll: post-clause step must be a positive integer constant)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(ForStmt(InitDecl([var_decl("i" Constant(ConstInt(5) None) Int None None None None false)]) Binary(LessThan Var("i" None) Constant(ConstInt(0) None) None) Postfix(Increment Var("i" None) None) Expression(Var("i" None)) "" "unroll"))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     true,
     R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(Compound(Block([])))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("p" None Pointer(Int) None None None None false)))
    S(ForStmt(InitDecl([var_decl("i" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var("i" None) Constant(ConstInt(4) None) None) Postfix(Increment Var("i" None) None) Compound(Block([
      S(Expression(Assignment(Var("p" None) AddressOf(Var("i" None) None) None)))
    ])) "" "unroll"))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~(unroll: address of induction variable `i` is taken in body)~~"},
    {R"~~(Program([
  VarDecl(var_decl("row" None Array(UChar 256) None None None None false))
  FunctionDecl(function_decl("fill" ["v"] Block([
    S(ForStmt(InitDecl([var_decl("i" Constant(ConstInt(0) None) UChar None None None None false)]) Binary(LessOrEqual Var("i" None) Constant(ConstInt(255) None) None) Postfix(Increment Var("i" None) None) Expression(Assignment(Subscript(Var("row" None) Var("i" None) None) Var("v" None) None)) "" "unroll"))
  ]) FunType([UChar] Void) None None [""] false None [] None false))
])
)~~",
     false,
     R"~~(unroll: loop never terminates (`i` wraps at the width of `unsigned char` and the condition stays true))~~"},
    {R"~~(Program([
  VarDecl(var_decl("BOUNDS" InitList([Constant(ConstInt(3) None) Constant(ConstInt(4) None) Constant(ConstInt(5) None)] None None) Array(Int 3) Static None None None false))
  FunctionDecl(function_decl("main" [] Block([
    S(ForStmt(InitDecl([var_decl("i" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var("i" None) Subscript(Var("BOUNDS" None) Constant(ConstInt(2) None) None) None) Postfix(Increment Var("i" None) None) Expression(Var("i" None)) "" "unroll"))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~(unroll: condition's right operand must be an integer constant)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("BOUNDS" InitList([Constant(ConstInt(3) None) Constant(ConstInt(4) None) Constant(ConstInt(5) None)] None None) Array(Const(Int) 3) None None None None false)))
    S(ForStmt(InitDecl([var_decl("i" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var("i" None) Subscript(Var("BOUNDS" None) Constant(ConstInt(2) None) None) None) Postfix(Increment Var("i" None) None) Expression(Var("i" None)) "" "unroll"))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~(unroll: condition's right operand must be an integer constant)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(ForStmt(InitDecl([var_decl("i" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var("i" None) Constant(ConstInt(4) None) None) Postfix(Increment Var("i" None) None) Compound(Block([
      S(ForStmt(InitDecl([var_decl("i" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var("i" None) Constant(ConstInt(2) None) None) Postfix(Increment Var("i" None) None) Expression(Var("i" None)) "" None))
    ])) "" "unroll"))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~(unroll: induction variable `i` is shadowed by an inner for-loop init)~~"},
    {R"~~(Program([
  VarDecl(var_decl("BOUNDS" InitList([Constant(ConstInt(3) None) Constant(ConstInt(4) None) Constant(ConstInt(5) None)] None None) Array(Const(Int) 3) Static None None None false))
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("j" Constant(ConstInt(1) None) Int None None None None false)))
    S(ForStmt(InitDecl([var_decl("i" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var("i" None) Subscript(Var("BOUNDS" None) Var("j" None) None) None) Postfix(Increment Var("i" None) None) Expression(Var("i" None)) "" "unroll"))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~(unroll: condition's right operand must be an integer constant)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(ForStmt(InitDecl([var_decl("i" Constant(ConstInt(0) None) UInt None None None None false)]) Binary(LessThan Var("i" None) Constant(ConstInt(2) None) None) Postfix(Increment Var("i" None) None) Expression(Var("i" None)) "" "unroll"))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     true,
     R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(Compound(Block([
      S(Compound(Block([
        S(Expression(Constant(ConstUInt(0) None)))
      ])))
      S(Compound(Block([
        S(Expression(Constant(ConstUInt(1) None)))
      ])))
    ])))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(ForStmt(InitDecl([var_decl("i" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var("i" None) Constant(ConstInt(4) None) None) Postfix(Increment Var("i" None) None) Expression(Var("i" None)) "" None))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     true,
     R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(ForStmt(InitDecl([var_decl("i" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var("i" None) Constant(ConstInt(4) None) None) Postfix(Increment Var("i" None) None) Expression(Var("i" None)) "" None))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(ForStmt(InitDecl([var_decl("i" Constant(ConstInt(250) None) UChar None None None None false)]) Binary(GreaterThan Var("i" None) Constant(ConstInt(3) None) None) CompoundAssignment(Add Var("i" None) Constant(ConstInt(4) None) None None) Expression(Var("i" None)) "" "unroll"))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     true,
     R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(Compound(Block([
      S(Compound(Block([
        S(Expression(Constant(ConstUChar(250) None)))
      ])))
      S(Compound(Block([
        S(Expression(Constant(ConstUChar(254) None)))
      ])))
    ])))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(ForStmt(InitDecl([var_decl("i" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var("i" None) Constant(ConstInt(3) None) None) Prefix(Increment Var("i" None) None) Expression(Var("i" None)) "" "unroll"))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     true,
     R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(Compound(Block([
      S(Compound(Block([
        S(Expression(Constant(ConstInt(0) None)))
      ])))
      S(Compound(Block([
        S(Expression(Constant(ConstInt(1) None)))
      ])))
      S(Compound(Block([
        S(Expression(Constant(ConstInt(2) None)))
      ])))
    ])))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(ForStmt(InitDecl([var_decl("i" Constant(ConstInt(0) None) Long None None None None false)]) Binary(LessThan Var("i" None) Constant(ConstInt(3) None) None) Postfix(Increment Var("i" None) None) Expression(Var("i" None)) "" "unroll"))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     true,
     R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(Compound(Block([
      S(Compound(Block([
        S(Expression(Constant(ConstLong(0) None)))
      ])))
      S(Compound(Block([
        S(Expression(Constant(ConstLong(1) None)))
      ])))
      S(Compound(Block([
        S(Expression(Constant(ConstLong(2) None)))
      ])))
    ])))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(ForStmt(InitDecl([var_decl("i" Constant(ConstInt(0) None) SChar None None None None false)]) Binary(LessThan Var("i" None) Constant(ConstInt(3) None) None) Postfix(Increment Var("i" None) None) Expression(Var("i" None)) "" "unroll"))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     true,
     R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(Compound(Block([
      S(Compound(Block([
        S(Expression(Constant(ConstChar(0) None)))
      ])))
      S(Compound(Block([
        S(Expression(Constant(ConstChar(1) None)))
      ])))
      S(Compound(Block([
        S(Expression(Constant(ConstChar(2) None)))
      ])))
    ])))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(ForStmt(InitDecl([var_decl("i" Constant(ConstInt(0) None) Char None None None None false)]) Binary(LessThan Var("i" None) Constant(ConstInt(3) None) None) Postfix(Increment Var("i" None) None) Expression(Var("i" None)) "" "unroll"))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     true,
     R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(Compound(Block([
      S(Compound(Block([
        S(Expression(Constant(ConstUChar(0) None)))
      ])))
      S(Compound(Block([
        S(Expression(Constant(ConstUChar(1) None)))
      ])))
      S(Compound(Block([
        S(Expression(Constant(ConstUChar(2) None)))
      ])))
    ])))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(ForStmt(InitDecl([var_decl("i" Constant(ConstInt(0) None) UChar None None None None false)]) Binary(LessThan Var("i" None) Constant(ConstInt(3) None) None) Postfix(Increment Var("i" None) None) Expression(Var("i" None)) "" "unroll"))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     true,
     R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(Compound(Block([
      S(Compound(Block([
        S(Expression(Constant(ConstUChar(0) None)))
      ])))
      S(Compound(Block([
        S(Expression(Constant(ConstUChar(1) None)))
      ])))
      S(Compound(Block([
        S(Expression(Constant(ConstUChar(2) None)))
      ])))
    ])))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(ForStmt(InitDecl([var_decl("i" Cast(Int Constant(ConstUInt(65533) None) None) Int None None None None false)]) Binary(GreaterThan Var("i" None) Constant(ConstUInt(65530) None) None) Postfix(Increment Var("i" None) None) Expression(Var("i" None)) "" "unroll"))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     true,
     R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(Compound(Block([
      S(Compound(Block([
        S(Expression(Constant(ConstInt(-3) None)))
      ])))
      S(Compound(Block([
        S(Expression(Constant(ConstInt(-2) None)))
      ])))
      S(Compound(Block([
        S(Expression(Constant(ConstInt(-1) None)))
      ])))
    ])))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(ForStmt(InitDecl([var_decl("i" Constant(ConstInt(3) None) Int None None None None false)]) Binary(GreaterOrEqual Var("i" None) Constant(ConstInt(0) None) None) Prefix(Decrement Var("i" None) None) Expression(Var("i" None)) "" "unroll"))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     true,
     R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(Compound(Block([
      S(Compound(Block([
        S(Expression(Constant(ConstInt(3) None)))
      ])))
      S(Compound(Block([
        S(Expression(Constant(ConstInt(2) None)))
      ])))
      S(Compound(Block([
        S(Expression(Constant(ConstInt(1) None)))
      ])))
      S(Compound(Block([
        S(Expression(Constant(ConstInt(0) None)))
      ])))
    ])))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(ForStmt(InitDecl([var_decl("i" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var("i" None) Cast(UChar Constant(ConstInt(260) None) None) None) Postfix(Increment Var("i" None) None) Expression(Var("i" None)) "" "unroll"))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     true,
     R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(Compound(Block([
      S(Compound(Block([
        S(Expression(Constant(ConstInt(0) None)))
      ])))
      S(Compound(Block([
        S(Expression(Constant(ConstInt(1) None)))
      ])))
      S(Compound(Block([
        S(Expression(Constant(ConstInt(2) None)))
      ])))
      S(Compound(Block([
        S(Expression(Constant(ConstInt(3) None)))
      ])))
    ])))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(ForStmt(InitDecl([var_decl("i" Constant(ConstInt(10) None) Int None None None None false)]) Binary(GreaterThan Var("i" None) Constant(ConstInt(0) None) None) CompoundAssignment(Subtract Var("i" None) Constant(ConstInt(2) None) None None) Expression(Var("i" None)) "" "unroll"))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     true,
     R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(Compound(Block([
      S(Compound(Block([
        S(Expression(Constant(ConstInt(10) None)))
      ])))
      S(Compound(Block([
        S(Expression(Constant(ConstInt(8) None)))
      ])))
      S(Compound(Block([
        S(Expression(Constant(ConstInt(6) None)))
      ])))
      S(Compound(Block([
        S(Expression(Constant(ConstInt(4) None)))
      ])))
      S(Compound(Block([
        S(Expression(Constant(ConstInt(2) None)))
      ])))
    ])))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(ForStmt(InitDecl([var_decl("i" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessOrEqual Var("i" None) Constant(ConstInt(3) None) None) Postfix(Increment Var("i" None) None) Expression(Binary(Add Var("i" None) Constant(ConstInt(1) None) None)) "" "unroll"))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     true,
     R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(Compound(Block([
      S(Compound(Block([
        S(Expression(Binary(Add Constant(ConstInt(0) None) Constant(ConstInt(1) None) None)))
      ])))
      S(Compound(Block([
        S(Expression(Binary(Add Constant(ConstInt(1) None) Constant(ConstInt(1) None) None)))
      ])))
      S(Compound(Block([
        S(Expression(Binary(Add Constant(ConstInt(2) None) Constant(ConstInt(1) None) None)))
      ])))
      S(Compound(Block([
        S(Expression(Binary(Add Constant(ConstInt(3) None) Constant(ConstInt(1) None) None)))
      ])))
    ])))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"},
    {R"~~(Program([
  VarDecl(var_decl("M" InitList([InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None) Constant(ConstInt(3) None)] None None) InitList([Constant(ConstInt(7) None)] None None)] None None) Array(Array(Const(Int) 3) 2) Static None None None false))
  FunctionDecl(function_decl("main" [] Block([
    S(ForStmt(InitDecl([var_decl("i" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var("i" None) Subscript(Subscript(Var("M" None) Constant(ConstInt(1) None) None) Constant(ConstInt(2) None) None) None) Postfix(Increment Var("i" None) None) Expression(Var("i" None)) "" "unroll"))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     true,
     R"~~(Program([
  VarDecl(var_decl("M" InitList([InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None) Constant(ConstInt(3) None)] None None) InitList([Constant(ConstInt(7) None)] None None)] None None) Array(Array(Const(Int) 3) 2) Static None None None false))
  FunctionDecl(function_decl("main" [] Block([
    S(Compound(Block([])))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(ForStmt(InitDecl([var_decl("i" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var("i" None) Constant(ConstInt(2) None) None) Postfix(Increment Var("i" None) None) Expression(Var("i" None)) "" "unroll"))
    S(ForStmt(InitDecl([var_decl("j" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var("j" None) Constant(ConstInt(2) None) None) Postfix(Increment Var("j" None) None) Expression(Var("j" None)) "" None))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     true,
     R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(Compound(Block([
      S(Compound(Block([
        S(Expression(Constant(ConstInt(0) None)))
      ])))
      S(Compound(Block([
        S(Expression(Constant(ConstInt(1) None)))
      ])))
    ])))
    S(ForStmt(InitDecl([var_decl("j" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var("j" None) Constant(ConstInt(2) None) None) Postfix(Increment Var("j" None) None) Expression(Var("j" None)) "" None))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("s" Constant(ConstInt(0) None) Int None None None None false)))
    S(ForStmt(InitDecl([var_decl("i" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var("i" None) Constant(ConstInt(4) None) None) Postfix(Increment Var("i" None) None) Expression(CompoundAssignment(Add Var("s" None) Var("i" None) None None)) "" "unroll"))
    S(Return(Var("s" None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     true,
     R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("s" Constant(ConstInt(0) None) Int None None None None false)))
    S(Compound(Block([
      S(Compound(Block([
        S(Expression(CompoundAssignment(Add Var("s" None) Constant(ConstInt(0) None) None None)))
      ])))
      S(Compound(Block([
        S(Expression(CompoundAssignment(Add Var("s" None) Constant(ConstInt(1) None) None None)))
      ])))
      S(Compound(Block([
        S(Expression(CompoundAssignment(Add Var("s" None) Constant(ConstInt(2) None) None None)))
      ])))
      S(Compound(Block([
        S(Expression(CompoundAssignment(Add Var("s" None) Constant(ConstInt(3) None) None None)))
      ])))
    ])))
    S(Return(Var("s" None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("s" Constant(ConstInt(0) None) Int None None None None false)))
    S(ForStmt(InitDecl([var_decl("i" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var("i" None) Constant(ConstInt(4) None) None) Postfix(Increment Var("i" None) None) Expression(CompoundAssignment(Add Var("s" None) Var("i" None) None None)) "" "unroll"))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     true,
     R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("s" Constant(ConstInt(0) None) Int None None None None false)))
    S(Compound(Block([
      S(Compound(Block([
        S(Expression(CompoundAssignment(Add Var("s" None) Constant(ConstInt(0) None) None None)))
      ])))
      S(Compound(Block([
        S(Expression(CompoundAssignment(Add Var("s" None) Constant(ConstInt(1) None) None None)))
      ])))
      S(Compound(Block([
        S(Expression(CompoundAssignment(Add Var("s" None) Constant(ConstInt(2) None) None None)))
      ])))
      S(Compound(Block([
        S(Expression(CompoundAssignment(Add Var("s" None) Constant(ConstInt(3) None) None None)))
      ])))
    ])))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"},
    {R"~~(Program([
  VarDecl(var_decl("BOUNDS" InitList([Constant(ConstInt(3) None) Constant(ConstInt(4) None) Constant(ConstInt(5) None)] None None) Array(Const(Int) 3) Static None None None false))
  FunctionDecl(function_decl("main" [] Block([
    S(ForStmt(InitDecl([var_decl("i" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var("i" None) Subscript(Var("BOUNDS" None) Constant(ConstInt(2) None) None) None) Postfix(Increment Var("i" None) None) Expression(Var("i" None)) "" "unroll"))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     true,
     R"~~(Program([
  VarDecl(var_decl("BOUNDS" InitList([Constant(ConstInt(3) None) Constant(ConstInt(4) None) Constant(ConstInt(5) None)] None None) Array(Const(Int) 3) Static None None None false))
  FunctionDecl(function_decl("main" [] Block([
    S(Compound(Block([
      S(Compound(Block([
        S(Expression(Constant(ConstInt(0) None)))
      ])))
      S(Compound(Block([
        S(Expression(Constant(ConstInt(1) None)))
      ])))
      S(Compound(Block([
        S(Expression(Constant(ConstInt(2) None)))
      ])))
      S(Compound(Block([
        S(Expression(Constant(ConstInt(3) None)))
      ])))
      S(Compound(Block([
        S(Expression(Constant(ConstInt(4) None)))
      ])))
    ])))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("s" Constant(ConstInt(0) None) Int None None None None false)))
    S(ForStmt(InitDecl([var_decl("i" Constant(ConstInt(5) None) Int None None None None false)]) Binary(GreaterThan Var("i" None) Constant(ConstInt(0) None) None) Postfix(Decrement Var("i" None) None) Expression(CompoundAssignment(Add Var("s" None) Var("i" None) None None)) "" "unroll"))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     true,
     R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("s" Constant(ConstInt(0) None) Int None None None None false)))
    S(Compound(Block([
      S(Compound(Block([
        S(Expression(CompoundAssignment(Add Var("s" None) Constant(ConstInt(5) None) None None)))
      ])))
      S(Compound(Block([
        S(Expression(CompoundAssignment(Add Var("s" None) Constant(ConstInt(4) None) None None)))
      ])))
      S(Compound(Block([
        S(Expression(CompoundAssignment(Add Var("s" None) Constant(ConstInt(3) None) None None)))
      ])))
      S(Compound(Block([
        S(Expression(CompoundAssignment(Add Var("s" None) Constant(ConstInt(2) None) None None)))
      ])))
      S(Compound(Block([
        S(Expression(CompoundAssignment(Add Var("s" None) Constant(ConstInt(1) None) None None)))
      ])))
    ])))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("s" Constant(ConstInt(0) None) Int None None None None false)))
    S(ForStmt(InitDecl([var_decl("i" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var("i" None) Constant(ConstInt(10) None) None) CompoundAssignment(Add Var("i" None) Constant(ConstInt(2) None) None None) Expression(CompoundAssignment(Add Var("s" None) Var("i" None) None None)) "" "unroll"))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     true,
     R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("s" Constant(ConstInt(0) None) Int None None None None false)))
    S(Compound(Block([
      S(Compound(Block([
        S(Expression(CompoundAssignment(Add Var("s" None) Constant(ConstInt(0) None) None None)))
      ])))
      S(Compound(Block([
        S(Expression(CompoundAssignment(Add Var("s" None) Constant(ConstInt(2) None) None None)))
      ])))
      S(Compound(Block([
        S(Expression(CompoundAssignment(Add Var("s" None) Constant(ConstInt(4) None) None None)))
      ])))
      S(Compound(Block([
        S(Expression(CompoundAssignment(Add Var("s" None) Constant(ConstInt(6) None) None None)))
      ])))
      S(Compound(Block([
        S(Expression(CompoundAssignment(Add Var("s" None) Constant(ConstInt(8) None) None None)))
      ])))
    ])))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("s" Constant(ConstInt(0) None) Int None None None None false)))
    S(ForStmt(InitDecl([var_decl("i" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var("i" None) Constant(ConstInt(3) None) None) Postfix(Increment Var("i" None) None) Compound(Block([
      D(VarDecl(var_decl("t" Binary(Add Var("i" None) Constant(ConstInt(1) None) None) Int None None None None false)))
      S(Expression(CompoundAssignment(Add Var("s" None) Var("t" None) None None)))
    ])) "" "unroll"))
    S(Return(Var("s" None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     true,
     R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("s" Constant(ConstInt(0) None) Int None None None None false)))
    S(Compound(Block([
      S(Compound(Block([
        D(VarDecl(var_decl("t" Binary(Add Constant(ConstInt(0) None) Constant(ConstInt(1) None) None) Int None None None None false)))
        S(Expression(CompoundAssignment(Add Var("s" None) Var("t" None) None None)))
      ])))
      S(Compound(Block([
        D(VarDecl(var_decl("t" Binary(Add Constant(ConstInt(1) None) Constant(ConstInt(1) None) None) Int None None None None false)))
        S(Expression(CompoundAssignment(Add Var("s" None) Var("t" None) None None)))
      ])))
      S(Compound(Block([
        D(VarDecl(var_decl("t" Binary(Add Constant(ConstInt(2) None) Constant(ConstInt(1) None) None) Int None None None None false)))
        S(Expression(CompoundAssignment(Add Var("s" None) Var("t" None) None None)))
      ])))
    ])))
    S(Return(Var("s" None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"},
    {R"~~(Program([
  VarDecl(var_decl("M" InitList([InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None) Constant(ConstInt(3) None)] None None) InitList([Constant(ConstInt(4) None) Constant(ConstInt(5) None) Constant(ConstInt(6) None)] None None)] None None) Array(Array(Const(Int) 3) 2) Static None None None false))
  FunctionDecl(function_decl("main" [] Block([
    S(ForStmt(InitDecl([var_decl("i" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var("i" None) Subscript(Subscript(Var("M" None) Constant(ConstInt(1) None) None) Constant(ConstInt(2) None) None) None) Postfix(Increment Var("i" None) None) Expression(Var("i" None)) "" "unroll"))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     true,
     R"~~(Program([
  VarDecl(var_decl("M" InitList([InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None) Constant(ConstInt(3) None)] None None) InitList([Constant(ConstInt(4) None) Constant(ConstInt(5) None) Constant(ConstInt(6) None)] None None)] None None) Array(Array(Const(Int) 3) 2) Static None None None false))
  FunctionDecl(function_decl("main" [] Block([
    S(Compound(Block([
      S(Compound(Block([
        S(Expression(Constant(ConstInt(0) None)))
      ])))
      S(Compound(Block([
        S(Expression(Constant(ConstInt(1) None)))
      ])))
      S(Compound(Block([
        S(Expression(Constant(ConstInt(2) None)))
      ])))
      S(Compound(Block([
        S(Expression(Constant(ConstInt(3) None)))
      ])))
      S(Compound(Block([
        S(Expression(Constant(ConstInt(4) None)))
      ])))
      S(Compound(Block([
        S(Expression(Constant(ConstInt(5) None)))
      ])))
    ])))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"},
    {R"~~(Program([
  VarDecl(var_decl("COUNT" InitList([Constant(ConstInt(2) None) Constant(ConstInt(4) None) Constant(ConstInt(1) None)] None None) Array(Const(Int) 3) Static None None None false))
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("s" Constant(ConstInt(0) None) Int None None None None false)))
    S(ForStmt(InitDecl([var_decl("b" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var("b" None) Constant(ConstInt(3) None) None) Postfix(Increment Var("b" None) None) Compound(Block([
      S(ForStmt(InitDecl([var_decl("i" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var("i" None) Subscript(Var("COUNT" None) Var("b" None) None) None) Postfix(Increment Var("i" None) None) Expression(Postfix(Increment Var("s" None) None)) "" "unroll"))
    ])) "" "unroll"))
    S(Return(Var("s" None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     true,
     R"~~(Program([
  VarDecl(var_decl("COUNT" InitList([Constant(ConstInt(2) None) Constant(ConstInt(4) None) Constant(ConstInt(1) None)] None None) Array(Const(Int) 3) Static None None None false))
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("s" Constant(ConstInt(0) None) Int None None None None false)))
    S(Compound(Block([
      S(Compound(Block([
        S(Compound(Block([
          S(Compound(Block([
            S(Expression(Postfix(Increment Var("s" None) None)))
          ])))
          S(Compound(Block([
            S(Expression(Postfix(Increment Var("s" None) None)))
          ])))
        ])))
      ])))
      S(Compound(Block([
        S(Compound(Block([
          S(Compound(Block([
            S(Expression(Postfix(Increment Var("s" None) None)))
          ])))
          S(Compound(Block([
            S(Expression(Postfix(Increment Var("s" None) None)))
          ])))
          S(Compound(Block([
            S(Expression(Postfix(Increment Var("s" None) None)))
          ])))
          S(Compound(Block([
            S(Expression(Postfix(Increment Var("s" None) None)))
          ])))
        ])))
      ])))
      S(Compound(Block([
        S(Compound(Block([
          S(Compound(Block([
            S(Expression(Postfix(Increment Var("s" None) None)))
          ])))
        ])))
      ])))
    ])))
    S(Return(Var("s" None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("s" Constant(ConstInt(0) None) Int None None None None false)))
    S(ForStmt(InitDecl([var_decl("i" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var("i" None) Constant(ConstInt(4) None) None) Postfix(Increment Var("i" None) None) Compound(Block([
      S(ForStmt(InitDecl([var_decl("j" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var("j" None) Constant(ConstInt(3) None) None) Postfix(Increment Var("j" None) None) Compound(Block([
        S(Expression(CompoundAssignment(Add Var("s" None) Constant(ConstInt(1) None) None None)))
      ])) "" "unroll"))
    ])) "" "unroll"))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     true,
     R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("s" Constant(ConstInt(0) None) Int None None None None false)))
    S(Compound(Block([
      S(Compound(Block([
        S(Compound(Block([
          S(Compound(Block([
            S(Expression(CompoundAssignment(Add Var("s" None) Constant(ConstInt(1) None) None None)))
          ])))
          S(Compound(Block([
            S(Expression(CompoundAssignment(Add Var("s" None) Constant(ConstInt(1) None) None None)))
          ])))
          S(Compound(Block([
            S(Expression(CompoundAssignment(Add Var("s" None) Constant(ConstInt(1) None) None None)))
          ])))
        ])))
      ])))
      S(Compound(Block([
        S(Compound(Block([
          S(Compound(Block([
            S(Expression(CompoundAssignment(Add Var("s" None) Constant(ConstInt(1) None) None None)))
          ])))
          S(Compound(Block([
            S(Expression(CompoundAssignment(Add Var("s" None) Constant(ConstInt(1) None) None None)))
          ])))
          S(Compound(Block([
            S(Expression(CompoundAssignment(Add Var("s" None) Constant(ConstInt(1) None) None None)))
          ])))
        ])))
      ])))
      S(Compound(Block([
        S(Compound(Block([
          S(Compound(Block([
            S(Expression(CompoundAssignment(Add Var("s" None) Constant(ConstInt(1) None) None None)))
          ])))
          S(Compound(Block([
            S(Expression(CompoundAssignment(Add Var("s" None) Constant(ConstInt(1) None) None None)))
          ])))
          S(Compound(Block([
            S(Expression(CompoundAssignment(Add Var("s" None) Constant(ConstInt(1) None) None None)))
          ])))
        ])))
      ])))
      S(Compound(Block([
        S(Compound(Block([
          S(Compound(Block([
            S(Expression(CompoundAssignment(Add Var("s" None) Constant(ConstInt(1) None) None None)))
          ])))
          S(Compound(Block([
            S(Expression(CompoundAssignment(Add Var("s" None) Constant(ConstInt(1) None) None None)))
          ])))
          S(Compound(Block([
            S(Expression(CompoundAssignment(Add Var("s" None) Constant(ConstInt(1) None) None None)))
          ])))
        ])))
      ])))
    ])))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"},
};

}  // namespace

TEST_CASE("the unroller gives the Python unit tests' results") {
  for (const Case& c : kCases) {
    auto prog = c6502::asdl::load<c6502::c99_ast::Program>(c.input);
    std::string got;
    try {
      got = c6502::asdl::dump(c6502::passes::unroll_program(std::move(prog)));
    } catch (const c6502::CompileError& e) {
      CHECK(e.python_type() == "UnrollError");
      got = e.what();
    }
    CHECK(got == c.expected);
  }
}
