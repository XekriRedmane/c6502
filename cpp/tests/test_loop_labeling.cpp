// The calls of the loop labeling pass that the Python unit tests make
// (tests/test_loop_labeling.py), each recorded with Python's result:
// the input program's dump, and the output's dump or the message of
// the LoopLabelingError.
#include <string>

#include "c99_ast.h"
#include "doctest.h"
#include "errors.h"
#include "passes/loop_labeling.h"

namespace {

struct Case {
  const char* input;
  bool ok;
  const char* expected;
};

const Case kCases[] = {
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(WhileStmt(Constant(ConstInt(1) None) Null ""))
    S(BreakStmt(""))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~('break' statement not inside a loop or switch)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(BreakStmt(""))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~('break' statement not inside a loop or switch)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(Compound(Block([
      S(BreakStmt(""))
    ])))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~('break' statement not inside a loop or switch)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(IfStmt(Constant(ConstInt(1) None) BreakStmt("") None))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~('break' statement not inside a loop or switch)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(LabeledStmt("foo" BreakStmt("")))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~('break' statement not inside a loop or switch)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(ContinueStmt(""))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~('continue' statement not inside a loop)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(WhileStmt(Constant(ConstInt(1) None) IfStmt(Constant(ConstInt(1) None) Null ContinueStmt("")) ""))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     true,
     R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(WhileStmt(Constant(ConstInt(1) None) IfStmt(Constant(ConstInt(1) None) Null ContinueStmt(".loop@0")) ".loop@0"))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(WhileStmt(Constant(ConstInt(1) None) IfStmt(Constant(ConstInt(1) None) BreakStmt("") None) ""))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     true,
     R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(WhileStmt(Constant(ConstInt(1) None) IfStmt(Constant(ConstInt(1) None) BreakStmt(".loop@0") None) ".loop@0"))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(WhileStmt(Constant(ConstInt(1) None) LabeledStmt("foo" BreakStmt("")) ""))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     true,
     R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(WhileStmt(Constant(ConstInt(1) None) LabeledStmt("foo" BreakStmt(".loop@0")) ".loop@0"))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(IfStmt(Constant(ConstInt(1) None) BreakStmt("") None))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~('break' statement not inside a loop or switch)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(BreakStmt(""))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~('break' statement not inside a loop or switch)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(ContinueStmt(""))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~('continue' statement not inside a loop)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(ForStmt(InitExp(None) None None ContinueStmt("") "" None))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     true,
     R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(ForStmt(InitExp(None) None None ContinueStmt(".loop@0") ".loop@0" None))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(WhileStmt(Constant(ConstInt(1) None) Compound(Block([
      S(ForStmt(InitExp(None) None None Compound(Block([
        S(BreakStmt(""))
        S(ContinueStmt(""))
      ])) "" None))
      S(BreakStmt(""))
    ])) ""))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     true,
     R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(WhileStmt(Constant(ConstInt(1) None) Compound(Block([
      S(ForStmt(InitExp(None) None None Compound(Block([
        S(BreakStmt(".loop@1"))
        S(ContinueStmt(".loop@1"))
      ])) ".loop@1" None))
      S(BreakStmt(".loop@0"))
    ])) ".loop@0"))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(WhileStmt(Constant(ConstInt(1) None) BreakStmt("") ""))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     true,
     R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(WhileStmt(Constant(ConstInt(1) None) BreakStmt(".loop@0") ".loop@0"))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(WhileStmt(Constant(ConstInt(1) None) BreakStmt("") ""))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     true,
     R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(WhileStmt(Constant(ConstInt(1) None) BreakStmt(".loop@0") ".loop@0"))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(WhileStmt(Constant(ConstInt(1) None) Compound(Block([
      S(WhileStmt(Constant(ConstInt(1) None) Null ""))
      S(BreakStmt(""))
    ])) ""))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     true,
     R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(WhileStmt(Constant(ConstInt(1) None) Compound(Block([
      S(WhileStmt(Constant(ConstInt(1) None) Null ".loop@1"))
      S(BreakStmt(".loop@0"))
    ])) ".loop@0"))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(WhileStmt(Constant(ConstInt(1) None) Compound(Block([
      S(WhileStmt(Constant(ConstInt(1) None) BreakStmt("") ""))
    ])) ""))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     true,
     R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(WhileStmt(Constant(ConstInt(1) None) Compound(Block([
      S(WhileStmt(Constant(ConstInt(1) None) BreakStmt(".loop@1") ".loop@1"))
    ])) ".loop@0"))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(WhileStmt(Constant(ConstInt(1) None) ForStmt(InitExp(None) None None ContinueStmt("") "" None) ""))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     true,
     R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(WhileStmt(Constant(ConstInt(1) None) ForStmt(InitExp(None) None None ContinueStmt(".loop@1") ".loop@1" None) ".loop@0"))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(ForStmt(InitExp(None) None None Compound(Block([
      S(WhileStmt(Constant(ConstInt(1) None) Compound(Block([
        S(DoWhileStmt(Compound(Block([
          S(BreakStmt(""))
        ])) Constant(ConstInt(1) None) ""))
        S(BreakStmt(""))
      ])) ""))
      S(BreakStmt(""))
    ])) "" None))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     true,
     R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(ForStmt(InitExp(None) None None Compound(Block([
      S(WhileStmt(Constant(ConstInt(1) None) Compound(Block([
        S(DoWhileStmt(Compound(Block([
          S(BreakStmt(".loop@2"))
        ])) Constant(ConstInt(1) None) ".loop@2"))
        S(BreakStmt(".loop@1"))
      ])) ".loop@1"))
      S(BreakStmt(".loop@0"))
    ])) ".loop@0" None))
  ]) FunType([] Int) None None [] false None [] None false))
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
    S(Goto(".main@end"))
    S(LabeledStmt(".main@end" Return(Constant(ConstInt(0) None))))
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
    S(WhileStmt(Constant(ConstInt(1) None) BreakStmt("") ""))
    S(WhileStmt(Constant(ConstInt(1) None) BreakStmt("") ""))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     true,
     R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(WhileStmt(Constant(ConstInt(1) None) BreakStmt(".loop@0") ".loop@0"))
    S(WhileStmt(Constant(ConstInt(1) None) BreakStmt(".loop@1") ".loop@1"))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(DoWhileStmt(BreakStmt("") Constant(ConstInt(1) None) ""))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     true,
     R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(DoWhileStmt(BreakStmt(".loop@0") Constant(ConstInt(1) None) ".loop@0"))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(ForStmt(InitExp(None) None None BreakStmt("") "" None))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     true,
     R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(ForStmt(InitExp(None) None None BreakStmt(".loop@0") ".loop@0" None))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(WhileStmt(Constant(ConstInt(1) None) ContinueStmt("") ""))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     true,
     R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(WhileStmt(Constant(ConstInt(1) None) ContinueStmt(".loop@0") ".loop@0"))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(SwitchStmt(Constant(ConstInt(1) None) Compound(Block([
      S(CaseStmt(Constant(ConstInt(0) None) ForStmt(InitExp(None) None None Compound(Block([
        S(BreakStmt(""))
      ])) "" None) ""))
    ])) "" [] None None))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     true,
     R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(SwitchStmt(Constant(ConstInt(1) None) Compound(Block([
      S(CaseStmt(Constant(ConstInt(0) None) ForStmt(InitExp(None) None None Compound(Block([
        S(BreakStmt(".loop@2"))
      ])) ".loop@2" None) ".case@1"))
    ])) ".switch@0" [switch_case(Constant(ConstInt(0) None) ".case@1")] None None))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(SwitchStmt(Constant(ConstInt(0) None) Compound(Block([
      S(CaseStmt(Constant(ConstInt(0) None) BreakStmt("") ""))
    ])) "" [] None None))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     true,
     R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(SwitchStmt(Constant(ConstInt(0) None) Compound(Block([
      S(CaseStmt(Constant(ConstInt(0) None) BreakStmt(".switch@0") ".case@1"))
    ])) ".switch@0" [switch_case(Constant(ConstInt(0) None) ".case@1")] None None))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(SwitchStmt(Constant(ConstInt(3) None) Compound(Block([
      S(CaseStmt(Constant(ConstInt(0) None) Null ""))
      S(CaseStmt(Constant(ConstInt(5) None) Null ""))
      S(DefaultStmt(Null ""))
    ])) "" [] None None))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     true,
     R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(SwitchStmt(Constant(ConstInt(3) None) Compound(Block([
      S(CaseStmt(Constant(ConstInt(0) None) Null ".case@1"))
      S(CaseStmt(Constant(ConstInt(5) None) Null ".case@2"))
      S(DefaultStmt(Null ".default@3"))
    ])) ".switch@0" [switch_case(Constant(ConstInt(0) None) ".case@1") switch_case(Constant(ConstInt(5) None) ".case@2")] ".default@3" None))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(ForStmt(InitExp(None) None None Compound(Block([
      S(CaseStmt(Constant(ConstInt(0) None) Return(Constant(ConstInt(0) None)) ""))
    ])) "" None))
    S(Return(Constant(ConstInt(9) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~('case' label not within a switch statement)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(ForStmt(InitExp(None) None None SwitchStmt(Constant(ConstInt(1) None) Compound(Block([
      S(CaseStmt(Constant(ConstInt(0) None) ContinueStmt("") ""))
    ])) "" [] None None) "" None))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     true,
     R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(ForStmt(InitExp(None) None None SwitchStmt(Constant(ConstInt(1) None) Compound(Block([
      S(CaseStmt(Constant(ConstInt(0) None) ContinueStmt(".loop@0") ".case@2"))
    ])) ".switch@1" [switch_case(Constant(ConstInt(0) None) ".case@2")] None None) ".loop@0" None))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(SwitchStmt(Constant(ConstInt(0) None) Compound(Block([
      S(CaseStmt(Constant(ConstInt(0) None) ContinueStmt("") ""))
    ])) "" [] None None))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~('continue' statement not inside a loop)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(Compound(Block([
      S(DefaultStmt(Return(Constant(ConstInt(0) None)) ""))
    ])))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~('default' label not within a switch statement)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(SwitchStmt(Constant(ConstInt(0) None) Compound(Block([
      S(CaseStmt(Constant(ConstInt(0) None) WhileStmt(Constant(ConstInt(1) None) DefaultStmt(Null "") "") ""))
      S(DefaultStmt(Null ""))
    ])) "" [] None None))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~(multiple default labels in one switch)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(SwitchStmt(Constant(ConstInt(0) None) Compound(Block([
      S(DefaultStmt(Null ""))
      S(DefaultStmt(Null ""))
    ])) "" [] None None))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~(multiple default labels in one switch)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(SwitchStmt(Constant(ConstInt(1) None) Compound(Block([
      S(CaseStmt(Constant(ConstInt(0) None) SwitchStmt(Constant(ConstInt(2) None) Compound(Block([
        S(CaseStmt(Constant(ConstInt(1) None) Null ""))
        S(DefaultStmt(Null ""))
      ])) "" [] None None) ""))
      S(CaseStmt(Constant(ConstInt(9) None) Null ""))
    ])) "" [] None None))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     true,
     R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(SwitchStmt(Constant(ConstInt(1) None) Compound(Block([
      S(CaseStmt(Constant(ConstInt(0) None) SwitchStmt(Constant(ConstInt(2) None) Compound(Block([
        S(CaseStmt(Constant(ConstInt(1) None) Null ".case@3"))
        S(DefaultStmt(Null ".default@4"))
      ])) ".switch@2" [switch_case(Constant(ConstInt(1) None) ".case@3")] ".default@4" None) ".case@1"))
      S(CaseStmt(Constant(ConstInt(9) None) Null ".case@5"))
    ])) ".switch@0" [switch_case(Constant(ConstInt(0) None) ".case@1") switch_case(Constant(ConstInt(9) None) ".case@5")] None None))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(SwitchStmt(Constant(ConstInt(1) None) Compound(Block([
      S(CaseStmt(Constant(ConstInt(0) None) BreakStmt("") ""))
      S(DefaultStmt(Null ""))
    ])) "" [] None None))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     true,
     R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(SwitchStmt(Constant(ConstInt(1) None) Compound(Block([
      S(CaseStmt(Constant(ConstInt(0) None) BreakStmt(".switch@0") ".case@1"))
      S(DefaultStmt(Null ".default@2"))
    ])) ".switch@0" [switch_case(Constant(ConstInt(0) None) ".case@1")] ".default@2" None))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"},
};

}  // namespace

TEST_CASE("loop labeling gives the Python unit tests' results") {
  for (const Case& c : kCases) {
    auto prog = c6502::asdl::load<c6502::c99_ast::Program>(c.input);
    std::string got;
    try {
      got = c6502::asdl::dump(c6502::passes::label_program(std::move(prog)));
    } catch (const c6502::CompileError& e) {
      CHECK(e.python_type() == "LoopLabelingError");
      got = e.what();
    }
    CHECK(got == c.expected);
  }
}
