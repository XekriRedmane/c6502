// The calls that the tests of tests/test_c99_to_tac.py made of the functions
// that `check-captures` runs, each with what the Python function
// returned, changed, or raised, which the C++ port must return,
// change, or raise too. tools/record_test_calls.py recorded them
// while the Python tests ran, before they were removed (step 8,
// groups G6 and G7). A call that another C++ case already checks
// isn't repeated here.
#include "doctest.h"
#include "record_case.h"
#include "c99_session_case.h"

TEST_CASE("c99_session: test_c99_to_tac.py::TestTranslateExp::test_binary_emits_instruction_and_returns_dst_var (call 1)") {
  c6502::testing::check_c99_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}",
      {
        {
          "translate_exp",
          "Binary(Add Constant(ConstInt(1) None) Constant(ConstInt(2) None) None)\n",
          true,
          {},
          "Var(\"%0\")\n",
          {
            "Binary(Add Constant(ConstInt(1)) Constant(ConstInt(2)) Var(\"%0\"))\n"},
        }
      },
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}");
}

TEST_CASE("c99_session: test_c99_to_tac.py::TestTranslateExp::test_binary_left_translated_before_right (call 1)") {
  c6502::testing::check_c99_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}",
      {
        {
          "translate_exp",
          "Binary(Add Unary(Negate Constant(ConstInt(1) None) None) Unary(Negate Constant(ConstInt(2) None) None) None)\n",
          true,
          {},
          "Var(\"%2\")\n",
          {
            "Unary(Negate Constant(ConstInt(1)) Var(\"%0\"))\n",
            "Unary(Negate Constant(ConstInt(2)) Var(\"%1\"))\n",
            "Binary(Add Var(\"%0\") Var(\"%1\") Var(\"%2\"))\n"},
        }
      },
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}");
}

TEST_CASE("c99_session: test_c99_to_tac.py::TestTranslateExp::test_constant_returns_tac_constant_emits_nothing (call 1)") {
  c6502::testing::check_c99_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}",
      {
        {
          "translate_exp",
          "Constant(ConstInt(42) None)\n",
          true,
          {},
          "Constant(ConstInt(42))\n",
          {},
        }
      },
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}");
}

TEST_CASE("c99_session: test_c99_to_tac.py::TestTranslateExp::test_each_binary_op_translates (call 2)") {
  c6502::testing::check_c99_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}",
      {
        {
          "translate_exp",
          "Binary(Subtract Constant(ConstInt(1) None) Constant(ConstInt(2) None) None)\n",
          true,
          {},
          "Var(\"%0\")\n",
          {
            "Binary(Subtract Constant(ConstInt(1)) Constant(ConstInt(2)) Var(\"%0\"))\n"},
        }
      },
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}");
}

TEST_CASE("c99_session: test_c99_to_tac.py::TestTranslateExp::test_each_binary_op_translates (call 3)") {
  c6502::testing::check_c99_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}",
      {
        {
          "translate_exp",
          "Binary(Multiply Constant(ConstInt(1) None) Constant(ConstInt(2) None) None)\n",
          true,
          {},
          "Var(\"%0\")\n",
          {
            "Binary(Multiply Constant(ConstInt(1)) Constant(ConstInt(2)) Var(\"%0\"))\n"},
        }
      },
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}");
}

TEST_CASE("c99_session: test_c99_to_tac.py::TestTranslateExp::test_each_binary_op_translates (call 4)") {
  c6502::testing::check_c99_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}",
      {
        {
          "translate_exp",
          "Binary(Divide Constant(ConstInt(1) None) Constant(ConstInt(2) None) None)\n",
          true,
          {},
          "Var(\"%0\")\n",
          {
            "Binary(Divide Constant(ConstInt(1)) Constant(ConstInt(2)) Var(\"%0\"))\n"},
        }
      },
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}");
}

TEST_CASE("c99_session: test_c99_to_tac.py::TestTranslateExp::test_each_binary_op_translates (call 5)") {
  c6502::testing::check_c99_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}",
      {
        {
          "translate_exp",
          "Binary(Modulo Constant(ConstInt(1) None) Constant(ConstInt(2) None) None)\n",
          true,
          {},
          "Var(\"%0\")\n",
          {
            "Binary(Modulo Constant(ConstInt(1)) Constant(ConstInt(2)) Var(\"%0\"))\n"},
        }
      },
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}");
}

TEST_CASE("c99_session: test_c99_to_tac.py::TestTranslateExp::test_each_binary_op_translates (call 6)") {
  c6502::testing::check_c99_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}",
      {
        {
          "translate_exp",
          "Binary(BitwiseAnd Constant(ConstInt(1) None) Constant(ConstInt(2) None) None)\n",
          true,
          {},
          "Var(\"%0\")\n",
          {
            "Binary(BitwiseAnd Constant(ConstInt(1)) Constant(ConstInt(2)) Var(\"%0\"))\n"},
        }
      },
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}");
}

TEST_CASE("c99_session: test_c99_to_tac.py::TestTranslateExp::test_each_binary_op_translates (call 7)") {
  c6502::testing::check_c99_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}",
      {
        {
          "translate_exp",
          "Binary(BitwiseOr Constant(ConstInt(1) None) Constant(ConstInt(2) None) None)\n",
          true,
          {},
          "Var(\"%0\")\n",
          {
            "Binary(BitwiseOr Constant(ConstInt(1)) Constant(ConstInt(2)) Var(\"%0\"))\n"},
        }
      },
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}");
}

TEST_CASE("c99_session: test_c99_to_tac.py::TestTranslateExp::test_each_binary_op_translates (call 8)") {
  c6502::testing::check_c99_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}",
      {
        {
          "translate_exp",
          "Binary(BitwiseXor Constant(ConstInt(1) None) Constant(ConstInt(2) None) None)\n",
          true,
          {},
          "Var(\"%0\")\n",
          {
            "Binary(BitwiseXor Constant(ConstInt(1)) Constant(ConstInt(2)) Var(\"%0\"))\n"},
        }
      },
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}");
}

TEST_CASE("c99_session: test_c99_to_tac.py::TestTranslateExp::test_each_binary_op_translates (call 9)") {
  c6502::testing::check_c99_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}",
      {
        {
          "translate_exp",
          "Binary(LeftShift Constant(ConstInt(1) None) Constant(ConstInt(2) None) None)\n",
          true,
          {},
          "Var(\"%0\")\n",
          {
            "Binary(LeftShift Constant(ConstInt(1)) Constant(ConstInt(2)) Var(\"%0\"))\n"},
        }
      },
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}");
}

TEST_CASE("c99_session: test_c99_to_tac.py::TestTranslateExp::test_each_binary_op_translates (call 10)") {
  c6502::testing::check_c99_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}",
      {
        {
          "translate_exp",
          "Binary(RightShift Constant(ConstInt(1) None) Constant(ConstInt(2) None) None)\n",
          true,
          {},
          "Var(\"%0\")\n",
          {
            "Binary(RightShift Constant(ConstInt(1)) Constant(ConstInt(2)) Var(\"%0\"))\n"},
        }
      },
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}");
}

TEST_CASE("c99_session: test_c99_to_tac.py::TestTranslateExp::test_each_binary_op_translates (call 11)") {
  c6502::testing::check_c99_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}",
      {
        {
          "translate_exp",
          "Binary(Equal Constant(ConstInt(1) None) Constant(ConstInt(2) None) None)\n",
          true,
          {},
          "Var(\"%0\")\n",
          {
            "Binary(Equal Constant(ConstInt(1)) Constant(ConstInt(2)) Var(\"%0\"))\n"},
        }
      },
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}");
}

TEST_CASE("c99_session: test_c99_to_tac.py::TestTranslateExp::test_each_binary_op_translates (call 12)") {
  c6502::testing::check_c99_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}",
      {
        {
          "translate_exp",
          "Binary(NotEqual Constant(ConstInt(1) None) Constant(ConstInt(2) None) None)\n",
          true,
          {},
          "Var(\"%0\")\n",
          {
            "Binary(NotEqual Constant(ConstInt(1)) Constant(ConstInt(2)) Var(\"%0\"))\n"},
        }
      },
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}");
}

TEST_CASE("c99_session: test_c99_to_tac.py::TestTranslateExp::test_each_binary_op_translates (call 13)") {
  c6502::testing::check_c99_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}",
      {
        {
          "translate_exp",
          "Binary(LessThan Constant(ConstInt(1) None) Constant(ConstInt(2) None) None)\n",
          true,
          {},
          "Var(\"%0\")\n",
          {
            "Binary(LessThan Constant(ConstInt(1)) Constant(ConstInt(2)) Var(\"%0\"))\n"},
        }
      },
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}");
}

TEST_CASE("c99_session: test_c99_to_tac.py::TestTranslateExp::test_each_binary_op_translates (call 14)") {
  c6502::testing::check_c99_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}",
      {
        {
          "translate_exp",
          "Binary(GreaterThan Constant(ConstInt(1) None) Constant(ConstInt(2) None) None)\n",
          true,
          {},
          "Var(\"%0\")\n",
          {
            "Binary(GreaterThan Constant(ConstInt(1)) Constant(ConstInt(2)) Var(\"%0\"))\n"},
        }
      },
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}");
}

TEST_CASE("c99_session: test_c99_to_tac.py::TestTranslateExp::test_each_binary_op_translates (call 15)") {
  c6502::testing::check_c99_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}",
      {
        {
          "translate_exp",
          "Binary(LessOrEqual Constant(ConstInt(1) None) Constant(ConstInt(2) None) None)\n",
          true,
          {},
          "Var(\"%0\")\n",
          {
            "Binary(LessOrEqual Constant(ConstInt(1)) Constant(ConstInt(2)) Var(\"%0\"))\n"},
        }
      },
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}");
}

TEST_CASE("c99_session: test_c99_to_tac.py::TestTranslateExp::test_each_binary_op_translates (call 16)") {
  c6502::testing::check_c99_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}",
      {
        {
          "translate_exp",
          "Binary(GreaterOrEqual Constant(ConstInt(1) None) Constant(ConstInt(2) None) None)\n",
          true,
          {},
          "Var(\"%0\")\n",
          {
            "Binary(GreaterOrEqual Constant(ConstInt(1)) Constant(ConstInt(2)) Var(\"%0\"))\n"},
        }
      },
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}");
}

TEST_CASE("c99_session: test_c99_to_tac.py::TestTranslateExp::test_each_unary_op_translates (call 1)") {
  c6502::testing::check_c99_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}",
      {
        {
          "translate_exp",
          "Unary(Negate Constant(ConstInt(1) None) None)\n",
          true,
          {},
          "Var(\"%0\")\n",
          {
            "Unary(Negate Constant(ConstInt(1)) Var(\"%0\"))\n"},
        }
      },
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}");
}

TEST_CASE("c99_session: test_c99_to_tac.py::TestTranslateExp::test_each_unary_op_translates (call 2)") {
  c6502::testing::check_c99_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}",
      {
        {
          "translate_exp",
          "Unary(Complement Constant(ConstInt(1) None) None)\n",
          true,
          {},
          "Var(\"%0\")\n",
          {
            "Unary(Complement Constant(ConstInt(1)) Var(\"%0\"))\n"},
        }
      },
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}");
}

TEST_CASE("c99_session: test_c99_to_tac.py::TestTranslateExp::test_each_unary_op_translates (call 3)") {
  c6502::testing::check_c99_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}",
      {
        {
          "translate_exp",
          "Unary(LogicalNot Constant(ConstInt(1) None) None)\n",
          true,
          {},
          "Var(\"%0\")\n",
          {
            "Unary(LogicalNot Constant(ConstInt(1)) Var(\"%0\"))\n"},
        }
      },
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}");
}

TEST_CASE("c99_session: test_c99_to_tac.py::TestTranslateExp::test_nested_unary_chains_temps (call 1)") {
  c6502::testing::check_c99_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}",
      {
        {
          "translate_exp",
          "Unary(Negate Unary(Complement Constant(ConstInt(5) None) None) None)\n",
          true,
          {},
          "Var(\"%1\")\n",
          {
            "Unary(Complement Constant(ConstInt(5)) Var(\"%0\"))\n",
            "Unary(Negate Var(\"%0\") Var(\"%1\"))\n"},
        }
      },
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}");
}

TEST_CASE("c99_session: test_c99_to_tac.py::TestTranslateExp::test_unary_emits_instruction_and_returns_dst_var (call 1)") {
  c6502::testing::check_c99_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}",
      {
        {
          "translate_exp",
          "Unary(Negate Constant(ConstInt(5) None) None)\n",
          true,
          {},
          "Var(\"%0\")\n",
          {
            "Unary(Negate Constant(ConstInt(5)) Var(\"%0\"))\n"},
        }
      },
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}");
}

TEST_CASE("c99_session: test_c99_to_tac.py::TestTranslateVarAndAssignment::test_assignment_constant_to_var_emits_copy (call 1)") {
  c6502::testing::check_c99_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}",
      {
        {
          "translate_exp",
          "Assignment(Var(\"@0.a\" None) Constant(ConstInt(5) None) None)\n",
          true,
          {},
          "Var(\"@0.a\")\n",
          {
            "Copy(Constant(ConstInt(5)) Var(\"@0.a\"))\n"},
        }
      },
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}");
}

TEST_CASE("c99_session: test_c99_to_tac.py::TestTranslateVarAndAssignment::test_assignment_var_to_var_emits_copy (call 1)") {
  c6502::testing::check_c99_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}",
      {
        {
          "translate_exp",
          "Assignment(Var(\"@0.a\" None) Var(\"@1.b\" None) None)\n",
          true,
          {},
          "Var(\"@0.a\")\n",
          {
            "Copy(Var(\"@1.b\") Var(\"@0.a\"))\n"},
        }
      },
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}");
}

TEST_CASE("c99_session: test_c99_to_tac.py::TestTranslateVarAndAssignment::test_assignment_with_compound_rval_emits_eval_then_copy (call 1)") {
  c6502::testing::check_c99_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}",
      {
        {
          "translate_exp",
          "Assignment(Var(\"@0.a\" None) Binary(Add Constant(ConstInt(1) None) Constant(ConstInt(2) None) None) None)\n",
          true,
          {},
          "Var(\"@0.a\")\n",
          {
            "Binary(Add Constant(ConstInt(1)) Constant(ConstInt(2)) Var(\"%0\"))\n",
            "Copy(Var(\"%0\") Var(\"@0.a\"))\n"},
        }
      },
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}");
}

TEST_CASE("c99_session: test_c99_to_tac.py::TestTranslateVarAndAssignment::test_chained_assignment_composes_right_to_left (call 1)") {
  c6502::testing::check_c99_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}",
      {
        {
          "translate_exp",
          "Assignment(Var(\"@1.b\" None) Assignment(Var(\"@0.a\" None) Constant(ConstInt(5) None) None) None)\n",
          true,
          {},
          "Var(\"@1.b\")\n",
          {
            "Copy(Constant(ConstInt(5)) Var(\"@0.a\"))\n",
            "Copy(Var(\"@0.a\") Var(\"@1.b\"))\n"},
        }
      },
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}");
}

TEST_CASE("c99_session: test_c99_to_tac.py::TestTranslateVarAndAssignment::test_non_var_lval_raises_type_error (call 1)") {
  c6502::testing::check_c99_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}",
      {
        {
          "translate_exp",
          "Assignment(Constant(ConstInt(1) None) Constant(ConstInt(2) None) None)\n",
          true,
          {},
          "exception: TypeError: assignment lval must be Var, Dereference, Subscript, Dot, or Arrow (identifier_resolution should have enforced this); got Constant(const=ConstInt(value=1), data_type=None)\n",
          {},
        }
      },
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}");
}

TEST_CASE("c99_session: test_c99_to_tac.py::TestTranslateVarAndAssignment::test_var_passthrough_emits_no_instructions (call 1)") {
  c6502::testing::check_c99_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}",
      {
        {
          "translate_exp",
          "Var(\"@0.x\" None)\n",
          true,
          {},
          "Var(\"@0.x\")\n",
          {},
        }
      },
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}");
}

TEST_CASE("c99_session: test_c99_to_tac.py::TestTranslateBlockItems::test_bare_declaration_emits_nothing (call 1)") {
  c6502::testing::check_c99_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}",
      {
        {
          "translate_declaration",
          "VarDecl(var_decl(\"@0.x\" None Int None None None None false))\n",
          true,
          {},
          "",
          {},
        }
      },
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}");
}

TEST_CASE("c99_session: test_c99_to_tac.py::TestTranslateBlockItems::test_block_item_dispatches_to_declaration (call 1)") {
  c6502::testing::check_c99_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}",
      {
        {
          "translate_block_item",
          "D(VarDecl(var_decl(\"@0.x\" Constant(ConstInt(7) None) Int None None None None false)))\n",
          true,
          {},
          "",
          {
            "Copy(Constant(ConstInt(7)) Var(\"@0.x\"))\n"},
        }
      },
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}");
}

TEST_CASE("c99_session: test_c99_to_tac.py::TestTranslateBlockItems::test_block_item_dispatches_to_statement (call 1)") {
  c6502::testing::check_c99_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}",
      {
        {
          "translate_block_item",
          "S(Null)\n",
          true,
          {},
          "",
          {},
        }
      },
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}");
}

TEST_CASE("c99_session: test_c99_to_tac.py::TestTranslateBlockItems::test_expression_statement_emits_inner_instructions (call 1)") {
  c6502::testing::check_c99_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}",
      {
        {
          "translate_statement",
          "Expression(Assignment(Var(\"@0.a\" None) Constant(ConstInt(5) None) None))\n",
          true,
          {},
          "",
          {
            "Copy(Constant(ConstInt(5)) Var(\"@0.a\"))\n"},
        }
      },
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}");
}

TEST_CASE("c99_session: test_c99_to_tac.py::TestTranslateBlockItems::test_function_decl_emits_nothing (call 1)") {
  c6502::testing::check_c99_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}",
      {
        {
          "translate_declaration",
          "FunctionDecl(function_decl(\"foo\" [] None FunType([] Int) None None [] false None [] None false))\n",
          true,
          {},
          "",
          {},
        }
      },
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}");
}

TEST_CASE("c99_session: test_c99_to_tac.py::TestTranslateBlockItems::test_initialized_declaration_emits_copy (call 1)") {
  c6502::testing::check_c99_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}",
      {
        {
          "translate_declaration",
          "VarDecl(var_decl(\"@0.x\" Constant(ConstInt(5) None) Int None None None None false))\n",
          true,
          {},
          "",
          {
            "Copy(Constant(ConstInt(5)) Var(\"@0.x\"))\n"},
        }
      },
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}");
}

TEST_CASE("c99_session: test_c99_to_tac.py::TestTranslateBlockItems::test_initialized_declaration_with_compound_init (call 1)") {
  c6502::testing::check_c99_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}",
      {
        {
          "translate_declaration",
          "VarDecl(var_decl(\"@0.x\" Binary(Add Constant(ConstInt(1) None) Constant(ConstInt(2) None) None) Int None None None None false))\n",
          true,
          {},
          "",
          {
            "Binary(Add Constant(ConstInt(1)) Constant(ConstInt(2)) Var(\"%0\"))\n",
            "Copy(Var(\"%0\") Var(\"@0.x\"))\n"},
        }
      },
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}");
}

TEST_CASE("c99_session: test_c99_to_tac.py::TestTranslateBlockItems::test_null_statement_emits_nothing (call 1)") {
  c6502::testing::check_c99_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}",
      {
        {
          "translate_statement",
          "Null\n",
          true,
          {},
          "",
          {},
        }
      },
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}");
}

TEST_CASE("c99_session: test_c99_to_tac.py::TestTranslateIfStatement::test_if_with_else_emits_split_branches (call 1)") {
  c6502::testing::check_c99_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}",
      {
        {
          "translate_statement",
          "IfStmt(Constant(ConstInt(1) None) Return(Constant(ConstInt(2) None)) Return(Constant(ConstInt(3) None)))\n",
          true,
          {},
          "",
          {
            "JumpIfFalse(Constant(ConstInt(1)) \".if_else@1\")\n",
            "Ret(Constant(ConstInt(2)))\n",
            "Jump(\".if_end@0\")\n",
            "Label(\".if_else@1\")\n",
            "Ret(Constant(ConstInt(3)))\n",
            "Label(\".if_end@0\")\n"},
        }
      },
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}");
}

TEST_CASE("c99_session: test_c99_to_tac.py::TestTranslateIfStatement::test_if_with_var_condition_evaluates_first (call 1)") {
  c6502::testing::check_c99_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}",
      {
        {
          "translate_statement",
          "IfStmt(Var(\"a\" None) Null None)\n",
          true,
          {},
          "",
          {
            "JumpIfFalse(Var(\"a\") \".if_end@0\")\n",
            "Label(\".if_end@0\")\n"},
        }
      },
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}");
}

TEST_CASE("c99_session: test_c99_to_tac.py::TestTranslateIfStatement::test_if_without_else_emits_jump_around_then (call 1)") {
  c6502::testing::check_c99_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}",
      {
        {
          "translate_statement",
          "IfStmt(Constant(ConstInt(1) None) Return(Constant(ConstInt(2) None)) None)\n",
          true,
          {},
          "",
          {
            "JumpIfFalse(Constant(ConstInt(1)) \".if_end@0\")\n",
            "Ret(Constant(ConstInt(2)))\n",
            "Label(\".if_end@0\")\n"},
        }
      },
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}");
}

TEST_CASE("c99_session: test_c99_to_tac.py::TestTranslateIfStatement::test_nested_if_each_gets_unique_labels (call 1)") {
  c6502::testing::check_c99_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}",
      {
        {
          "translate_statement",
          "IfStmt(Constant(ConstInt(1) None) IfStmt(Constant(ConstInt(2) None) Return(Constant(ConstInt(3) None)) None) None)\n",
          true,
          {},
          "",
          {
            "JumpIfFalse(Constant(ConstInt(1)) \".if_end@0\")\n",
            "JumpIfFalse(Constant(ConstInt(2)) \".if_end@1\")\n",
            "Ret(Constant(ConstInt(3)))\n",
            "Label(\".if_end@1\")\n",
            "Label(\".if_end@0\")\n"},
        }
      },
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}");
}

TEST_CASE("c99_session: test_c99_to_tac.py::TestTranslateGotoAndLabeled::test_goto_emits_jump (call 1)") {
  c6502::testing::check_c99_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}",
      {
        {
          "translate_statement",
          "Goto(\".main@foo\")\n",
          true,
          {},
          "",
          {
            "Jump(\".main@foo\")\n"},
        }
      },
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}");
}

TEST_CASE("c99_session: test_c99_to_tac.py::TestTranslateGotoAndLabeled::test_goto_in_function_body (call 1)") {
  c6502::testing::check_c99_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}",
      {
        {
          "translate_function",
          "Function(\"main\" [] Block([\n"
          "  S(LabeledStmt(\".main@foo\" Goto(\".main@foo\")))\n"
          "]))\n",
          false,
          {},
          "Function(\"main\" true [] [\n"
          "  Label(\".main@foo\")\n"
          "  Jump(\".main@foo\")\n"
          "  Ret(Constant(ConstInt(0)))\n"
          "])\n",
          {},
        }
      },
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}");
}

TEST_CASE("c99_session: test_c99_to_tac.py::TestTranslateGotoAndLabeled::test_labeled_null_statement_emits_just_the_label (call 1)") {
  c6502::testing::check_c99_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}",
      {
        {
          "translate_statement",
          "LabeledStmt(\".main@foo\" Null)\n",
          true,
          {},
          "",
          {
            "Label(\".main@foo\")\n"},
        }
      },
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}");
}

TEST_CASE("c99_session: test_c99_to_tac.py::TestTranslateGotoAndLabeled::test_labeled_statement_emits_label_then_inner (call 1)") {
  c6502::testing::check_c99_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}",
      {
        {
          "translate_statement",
          "LabeledStmt(\".main@foo\" Return(Constant(ConstInt(0) None)))\n",
          true,
          {},
          "",
          {
            "Label(\".main@foo\")\n",
            "Ret(Constant(ConstInt(0)))\n"},
        }
      },
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}");
}

TEST_CASE("c99_session: test_c99_to_tac.py::TestTranslateGotoAndLabeled::test_nested_labeled_statements (call 1)") {
  c6502::testing::check_c99_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}",
      {
        {
          "translate_statement",
          "LabeledStmt(\".main@a\" LabeledStmt(\".main@b\" Null))\n",
          true,
          {},
          "",
          {
            "Label(\".main@a\")\n",
            "Label(\".main@b\")\n"},
        }
      },
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}");
}

TEST_CASE("c99_session: test_c99_to_tac.py::TestTranslateCompound::test_compound_emits_inner_items_in_order (call 1)") {
  c6502::testing::check_c99_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}",
      {
        {
          "translate_statement",
          "Compound(Block([\n"
          "  D(VarDecl(var_decl(\"@0.x\" Constant(ConstInt(1) None) Int None None None None false)))\n"
          "  S(Return(Var(\"@0.x\" None)))\n"
          "]))\n",
          true,
          {},
          "",
          {
            "Copy(Constant(ConstInt(1)) Var(\"@0.x\"))\n",
            "Ret(Var(\"@0.x\"))\n"},
        }
      },
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}");
}

TEST_CASE("c99_session: test_c99_to_tac.py::TestTranslateCompound::test_compound_in_if_branch (call 1)") {
  c6502::testing::check_c99_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}",
      {
        {
          "translate_statement",
          "IfStmt(Constant(ConstInt(1) None) Compound(Block([\n"
          "  S(Return(Constant(ConstInt(2) None)))\n"
          "])) None)\n",
          true,
          {},
          "",
          {
            "JumpIfFalse(Constant(ConstInt(1)) \".if_end@0\")\n",
            "Ret(Constant(ConstInt(2)))\n",
            "Label(\".if_end@0\")\n"},
        }
      },
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}");
}

TEST_CASE("c99_session: test_c99_to_tac.py::TestTranslateCompound::test_compound_inside_function_body (call 1)") {
  c6502::testing::check_c99_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}",
      {
        {
          "translate_function",
          "Function(\"main\" [] Block([\n"
          "  S(Compound(Block([\n"
          "    S(Return(Constant(ConstInt(7) None)))\n"
          "  ])))\n"
          "]))\n",
          false,
          {},
          "Function(\"main\" true [] [\n"
          "  Ret(Constant(ConstInt(7)))\n"
          "])\n",
          {},
        }
      },
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}");
}

TEST_CASE("c99_session: test_c99_to_tac.py::TestTranslateCompound::test_compound_with_distinct_shadowed_decls (call 1)") {
  c6502::testing::check_c99_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}",
      {
        {
          "translate_statement",
          "Compound(Block([\n"
          "  D(VarDecl(var_decl(\"@1.x\" Constant(ConstInt(2) None) Int None None None None false)))\n"
          "]))\n",
          true,
          {},
          "",
          {
            "Copy(Constant(ConstInt(2)) Var(\"@1.x\"))\n"},
        }
      },
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}");
}

TEST_CASE("c99_session: test_c99_to_tac.py::TestTranslateCompound::test_empty_compound_emits_nothing (call 1)") {
  c6502::testing::check_c99_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}",
      {
        {
          "translate_statement",
          "Compound(Block([]))\n",
          true,
          {},
          "",
          {},
        }
      },
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}");
}

TEST_CASE("c99_session: test_c99_to_tac.py::TestTranslateCompound::test_nested_compound_flattens (call 1)") {
  c6502::testing::check_c99_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}",
      {
        {
          "translate_statement",
          "Compound(Block([\n"
          "  S(Compound(Block([\n"
          "    S(Return(Constant(ConstInt(1) None)))\n"
          "  ])))\n"
          "]))\n",
          true,
          {},
          "",
          {
            "Ret(Constant(ConstInt(1)))\n"},
        }
      },
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}");
}

TEST_CASE("c99_session: test_c99_to_tac.py::TestTranslateConditional::test_basic_lowers_to_jump_copy_copy (call 1)") {
  c6502::testing::check_c99_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}",
      {
        {
          "translate_exp",
          "Conditional(Constant(ConstInt(1) None) Constant(ConstInt(2) None) Constant(ConstInt(3) None) None)\n",
          true,
          {},
          "Var(\"%0\")\n",
          {
            "JumpIfFalse(Constant(ConstInt(1)) \".cond_else@0\")\n",
            "Copy(Constant(ConstInt(2)) Var(\"%0\"))\n",
            "Jump(\".cond_end@1\")\n",
            "Label(\".cond_else@0\")\n",
            "Copy(Constant(ConstInt(3)) Var(\"%0\"))\n",
            "Label(\".cond_end@1\")\n"},
        }
      },
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}");
}

TEST_CASE("c99_session: test_c99_to_tac.py::TestTranslateConditional::test_nested_conditional_gets_unique_labels (call 1)") {
  c6502::testing::check_c99_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}",
      {
        {
          "translate_exp",
          "Conditional(Constant(ConstInt(1) None) Conditional(Constant(ConstInt(2) None) Constant(ConstInt(3) None) Constant(ConstInt(4) None) None) Constant(ConstInt(5) None) None)\n",
          true,
          {},
          "Var(\"%0\")\n",
          {
            "JumpIfFalse(Constant(ConstInt(1)) \".cond_else@0\")\n",
            "JumpIfFalse(Constant(ConstInt(2)) \".cond_else@2\")\n",
            "Copy(Constant(ConstInt(3)) Var(\"%1\"))\n",
            "Jump(\".cond_end@3\")\n",
            "Label(\".cond_else@2\")\n",
            "Copy(Constant(ConstInt(4)) Var(\"%1\"))\n",
            "Label(\".cond_end@3\")\n",
            "Copy(Var(\"%1\") Var(\"%0\"))\n",
            "Jump(\".cond_end@1\")\n",
            "Label(\".cond_else@0\")\n",
            "Copy(Constant(ConstInt(5)) Var(\"%0\"))\n",
            "Label(\".cond_end@1\")\n"},
        }
      },
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}");
}

TEST_CASE("c99_session: test_c99_to_tac.py::TestTranslateFunctionFallThrough::test_block_items_processed_in_order (call 1)") {
  c6502::testing::check_c99_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}",
      {
        {
          "translate_function",
          "Function(\"main\" [] Block([\n"
          "  D(VarDecl(var_decl(\"@0.a\" Constant(ConstInt(1) None) Int None None None None false)))\n"
          "  D(VarDecl(var_decl(\"@1.b\" Constant(ConstInt(2) None) Int None None None None false)))\n"
          "  S(Return(Var(\"@1.b\" None)))\n"
          "]))\n",
          false,
          {},
          "Function(\"main\" true [] [\n"
          "  Copy(Constant(ConstInt(1)) Var(\"@0.a\"))\n"
          "  Copy(Constant(ConstInt(2)) Var(\"@1.b\"))\n"
          "  Ret(Var(\"@1.b\"))\n"
          "])\n",
          {},
        }
      },
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}");
}

TEST_CASE("c99_session: test_c99_to_tac.py::TestTranslateFunctionFallThrough::test_body_without_return_gets_implicit_return_zero (call 1)") {
  c6502::testing::check_c99_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}",
      {
        {
          "translate_function",
          "Function(\"main\" [] Block([\n"
          "  D(VarDecl(var_decl(\"@0.x\" Constant(ConstInt(5) None) Int None None None None false)))\n"
          "]))\n",
          false,
          {},
          "Function(\"main\" true [] [\n"
          "  Copy(Constant(ConstInt(5)) Var(\"@0.x\"))\n"
          "  Ret(Constant(ConstInt(0)))\n"
          "])\n",
          {},
        }
      },
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}");
}

TEST_CASE("c99_session: test_c99_to_tac.py::TestTranslateFunctionFallThrough::test_empty_function_gets_implicit_return_zero (call 1)") {
  c6502::testing::check_c99_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}",
      {
        {
          "translate_function",
          "Function(\"main\" [] Block([]))\n",
          false,
          {},
          "Function(\"main\" true [] [\n"
          "  Ret(Constant(ConstInt(0)))\n"
          "])\n",
          {},
        }
      },
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}");
}

TEST_CASE("c99_session: test_c99_to_tac.py::TestTranslateFunctionFallThrough::test_explicit_return_does_not_append_a_second_one (call 1)") {
  c6502::testing::check_c99_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}",
      {
        {
          "translate_function",
          "Function(\"main\" [] Block([\n"
          "  S(Return(Constant(ConstInt(7) None)))\n"
          "]))\n",
          false,
          {},
          "Function(\"main\" true [] [\n"
          "  Ret(Constant(ConstInt(7)))\n"
          "])\n",
          {},
        }
      },
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}");
}

TEST_CASE("c99_session: test_c99_to_tac.py::TestTranslateFunctionFallThrough::test_null_after_return_does_not_trigger_second_return (call 1)") {
  c6502::testing::check_c99_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}",
      {
        {
          "translate_function",
          "Function(\"main\" [] Block([\n"
          "  S(Return(Constant(ConstInt(7) None)))\n"
          "  S(Null)\n"
          "]))\n",
          false,
          {},
          "Function(\"main\" true [] [\n"
          "  Ret(Constant(ConstInt(7)))\n"
          "])\n",
          {},
        }
      },
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}");
}

TEST_CASE("parse: test_c99_to_tac.py::TestTranslateProgram::test_compound_assignment_lowers_like_desugared_form (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c99_to_tac.py::TestTranslateProgram::test_compound_assignment_lowers_like_desugared_form\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { int a; a += 1; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" None Int None None None None false)))\n"
      "    S(Expression(CompoundAssignment(Add Var(\"a\" None) Constant(ConstInt(1) None) None None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_c99_to_tac.py::TestTranslateProgram::test_compound_assignment_lowers_like_desugared_form (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c99_to_tac.py::TestTranslateProgram::test_compound_assignment_lowers_like_desugared_form\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"a\\\" None Int None None None None false)))\\n    S(Expression(CompoundAssignment(Add Var(\\\"a\\\" Int) Constant(ConstInt(1) Int) Int Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" None Int None None None None false)))\n"
      "    S(Expression(CompoundAssignment(Add Var(\"a\" None) Constant(ConstInt(1) None) None None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"a\\\" None Int None None None None false)))\\n    S(Expression(CompoundAssignment(Add Var(\\\"a\\\" Int) Constant(ConstInt(1) Int) Int Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("c99_to_tac: test_c99_to_tac.py::TestTranslateProgram::test_compound_assignment_lowers_like_desugared_form (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass c99_to_tac\n"
      "source tests/test_c99_to_tac.py::TestTranslateProgram::test_compound_assignment_lowers_like_desugared_form\n"
      "args {\"args\": [{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"args\": {\"0\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" None Int None None None None false)))\n"
      "    S(Expression(CompoundAssignment(Add Var(\"a\" Int) Constant(ConstInt(1) Int) Int Int)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Binary(Add Var(\"a\") Constant(ConstInt(1)) Var(\"%0\"))\n"
      "    Copy(Var(\"%0\") Var(\"a\"))\n"
      "    Ret(Constant(ConstInt(0)))\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("parse: test_c99_to_tac.py::TestTranslateProgram::test_counter_resets_per_translate_program_call (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c99_to_tac.py::TestTranslateProgram::test_counter_resets_per_translate_program_call\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { return -5; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Unary(Negate Constant(ConstInt(5) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_c99_to_tac.py::TestTranslateProgram::test_counter_resets_per_translate_program_call (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c99_to_tac.py::TestTranslateProgram::test_counter_resets_per_translate_program_call\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(Unary(Negate Constant(ConstInt(5) Int) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Unary(Negate Constant(ConstInt(5) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(Unary(Negate Constant(ConstInt(5) Int) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("c99_to_tac: test_c99_to_tac.py::TestTranslateProgram::test_counter_resets_per_translate_program_call (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass c99_to_tac\n"
      "source tests/test_c99_to_tac.py::TestTranslateProgram::test_counter_resets_per_translate_program_call\n"
      "args {\"args\": [{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"args\": {\"0\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Unary(Negate Constant(ConstInt(5) Int) Int)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Unary(Negate Constant(ConstInt(5)) Var(\"%0\"))\n"
      "    Ret(Var(\"%0\"))\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("parse: test_c99_to_tac.py::TestTranslateProgram::test_end_to_end_binary_precedence (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c99_to_tac.py::TestTranslateProgram::test_end_to_end_binary_precedence\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { return 1 + 2 * 3; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Binary(Add Constant(ConstInt(1) None) Binary(Multiply Constant(ConstInt(2) None) Constant(ConstInt(3) None) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_c99_to_tac.py::TestTranslateProgram::test_end_to_end_binary_precedence (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c99_to_tac.py::TestTranslateProgram::test_end_to_end_binary_precedence\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(Binary(Add Constant(ConstInt(1) Int) Binary(Multiply Constant(ConstInt(2) Int) Constant(ConstInt(3) Int) Int) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Binary(Add Constant(ConstInt(1) None) Binary(Multiply Constant(ConstInt(2) None) Constant(ConstInt(3) None) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(Binary(Add Constant(ConstInt(1) Int) Binary(Multiply Constant(ConstInt(2) Int) Constant(ConstInt(3) Int) Int) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("c99_to_tac: test_c99_to_tac.py::TestTranslateProgram::test_end_to_end_binary_precedence (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass c99_to_tac\n"
      "source tests/test_c99_to_tac.py::TestTranslateProgram::test_end_to_end_binary_precedence\n"
      "args {\"args\": [{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"args\": {\"0\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Binary(Add Constant(ConstInt(1) Int) Binary(Multiply Constant(ConstInt(2) Int) Constant(ConstInt(3) Int) Int) Int)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Binary(Multiply Constant(ConstInt(2)) Constant(ConstInt(3)) Var(\"%0\"))\n"
      "    Binary(Add Constant(ConstInt(1)) Var(\"%0\") Var(\"%1\"))\n"
      "    Ret(Var(\"%1\"))\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("parse: test_c99_to_tac.py::TestTranslateProgram::test_end_to_end_nested_from_source (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c99_to_tac.py::TestTranslateProgram::test_end_to_end_nested_from_source\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { return -(~5); }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Unary(Negate Unary(Complement Constant(ConstInt(5) None) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_c99_to_tac.py::TestTranslateProgram::test_end_to_end_nested_from_source (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c99_to_tac.py::TestTranslateProgram::test_end_to_end_nested_from_source\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(Unary(Negate Unary(Complement Constant(ConstInt(5) Int) Int) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Unary(Negate Unary(Complement Constant(ConstInt(5) None) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(Unary(Negate Unary(Complement Constant(ConstInt(5) Int) Int) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("c99_to_tac: test_c99_to_tac.py::TestTranslateProgram::test_end_to_end_nested_from_source (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass c99_to_tac\n"
      "source tests/test_c99_to_tac.py::TestTranslateProgram::test_end_to_end_nested_from_source\n"
      "args {\"args\": [{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"args\": {\"0\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Unary(Negate Unary(Complement Constant(ConstInt(5) Int) Int) Int)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Unary(Complement Constant(ConstInt(5)) Var(\"%0\"))\n"
      "    Unary(Negate Var(\"%0\") Var(\"%1\"))\n"
      "    Ret(Var(\"%1\"))\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("c99_session: test_c99_to_tac.py::TestTranslateProgram::test_postfix_decrement_uses_subtract (call 1)") {
  c6502::testing::check_c99_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}",
      {
        {
          "translate_exp",
          "Postfix(Decrement Var(\"a\" None) None)\n",
          true,
          {},
          "Var(\"%0\")\n",
          {
            "Copy(Var(\"a\") Var(\"%0\"))\n",
            "Binary(Subtract Var(\"a\") Constant(ConstInt(1)) Var(\"%1\"))\n",
            "Copy(Var(\"%1\") Var(\"a\"))\n"},
        }
      },
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}");
}

TEST_CASE("c99_session: test_c99_to_tac.py::TestTranslateProgram::test_postfix_in_assignment_returns_old_value (call 1)") {
  c6502::testing::check_c99_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}",
      {
        {
          "translate_exp",
          "Assignment(Var(\"b\" None) Postfix(Increment Var(\"a\" None) None) None)\n",
          true,
          {},
          "Var(\"b\")\n",
          {
            "Copy(Var(\"a\") Var(\"%0\"))\n",
            "Binary(Add Var(\"a\") Constant(ConstInt(1)) Var(\"%1\"))\n",
            "Copy(Var(\"%1\") Var(\"a\"))\n",
            "Copy(Var(\"%0\") Var(\"b\"))\n"},
        }
      },
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}");
}

TEST_CASE("c99_session: test_c99_to_tac.py::TestTranslateProgram::test_postfix_increment_captures_old_value_then_updates (call 1)") {
  c6502::testing::check_c99_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}",
      {
        {
          "translate_exp",
          "Postfix(Increment Var(\"a\" None) None)\n",
          true,
          {},
          "Var(\"%0\")\n",
          {
            "Copy(Var(\"a\") Var(\"%0\"))\n",
            "Binary(Add Var(\"a\") Constant(ConstInt(1)) Var(\"%1\"))\n",
            "Copy(Var(\"%1\") Var(\"a\"))\n"},
        }
      },
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}");
}

TEST_CASE("c99_session: test_c99_to_tac.py::TestTranslateProgram::test_postfix_non_var_operand_raises_type_error (call 1)") {
  c6502::testing::check_c99_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}",
      {
        {
          "translate_exp",
          "Postfix(Increment Constant(ConstInt(1) None) None)\n",
          true,
          {},
          "exception: TypeError: increment/decrement operand must be Var, Subscript, Dereference, Dot, or Arrow (identifier_resolution should have enforced this); got Constant(const=ConstInt(value=1), data_type=None)\n",
          {},
        }
      },
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}");
}

TEST_CASE("parse: test_c99_to_tac.py::TestTranslateProgram::test_prefix_lowers_via_assignment_branch (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c99_to_tac.py::TestTranslateProgram::test_prefix_lowers_via_assignment_branch\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { int a; ++a; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" None Int None None None None false)))\n"
      "    S(Expression(Prefix(Increment Var(\"a\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_c99_to_tac.py::TestTranslateProgram::test_prefix_lowers_via_assignment_branch (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c99_to_tac.py::TestTranslateProgram::test_prefix_lowers_via_assignment_branch\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"a\\\" None Int None None None None false)))\\n    S(Expression(Prefix(Increment Var(\\\"a\\\" Int) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" None Int None None None None false)))\n"
      "    S(Expression(Prefix(Increment Var(\"a\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"a\\\" None Int None None None None false)))\\n    S(Expression(Prefix(Increment Var(\\\"a\\\" Int) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("c99_to_tac: test_c99_to_tac.py::TestTranslateProgram::test_prefix_lowers_via_assignment_branch (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass c99_to_tac\n"
      "source tests/test_c99_to_tac.py::TestTranslateProgram::test_prefix_lowers_via_assignment_branch\n"
      "args {\"args\": [{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"args\": {\"0\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" None Int None None None None false)))\n"
      "    S(Expression(Prefix(Increment Var(\"a\" Int) Int)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Binary(Add Var(\"a\") Constant(ConstInt(1)) Var(\"%0\"))\n"
      "    Copy(Var(\"%0\") Var(\"a\"))\n"
      "    Ret(Constant(ConstInt(0)))\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("type_checking: test_c99_to_tac.py::TestTranslateProgram::test_return_constant (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c99_to_tac.py::TestTranslateProgram::test_return_constant\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(Constant(ConstInt(42) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(42) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(Constant(ConstInt(42) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("c99_to_tac: test_c99_to_tac.py::TestTranslateProgram::test_return_constant (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass c99_to_tac\n"
      "source tests/test_c99_to_tac.py::TestTranslateProgram::test_return_constant\n"
      "args {\"args\": [{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(42) Int)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Ret(Constant(ConstInt(42)))\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("type_checking: test_c99_to_tac.py::TestTranslateProgram::test_return_unary (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c99_to_tac.py::TestTranslateProgram::test_return_unary\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(Unary(Negate Constant(ConstInt(5) Int) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Unary(Negate Constant(ConstInt(5) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(Unary(Negate Constant(ConstInt(5) Int) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("c99_to_tac: test_c99_to_tac.py::TestTranslateProgram::test_return_unary (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass c99_to_tac\n"
      "source tests/test_c99_to_tac.py::TestTranslateProgram::test_return_unary\n"
      "args {\"args\": [{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"args\": {\"0\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Unary(Negate Constant(ConstInt(5) Int) Int)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Unary(Negate Constant(ConstInt(5)) Var(\"%0\"))\n"
      "    Ret(Var(\"%0\"))\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("c99_session: test_c99_to_tac.py::TestTranslateLoops::test_do_while_basic_shape (call 1)") {
  c6502::testing::check_c99_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}",
      {
        {
          "translate_statement",
          "DoWhileStmt(Null Constant(ConstInt(1) None) \".loop@0\")\n",
          true,
          {},
          "",
          {
            "Label(\".loop@0_start\")\n",
            "Label(\".loop@0_continue\")\n",
            "JumpIfTrue(Constant(ConstInt(1)) \".loop@0_start\")\n",
            "Label(\".loop@0_break\")\n"},
        }
      },
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}");
}

TEST_CASE("c99_session: test_c99_to_tac.py::TestTranslateLoops::test_do_while_continue_targets_continue_label (call 1)") {
  c6502::testing::check_c99_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}",
      {
        {
          "translate_statement",
          "DoWhileStmt(ContinueStmt(\".loop@0\") Constant(ConstInt(1) None) \".loop@0\")\n",
          true,
          {},
          "",
          {
            "Label(\".loop@0_start\")\n",
            "Jump(\".loop@0_continue\")\n",
            "Label(\".loop@0_continue\")\n",
            "JumpIfTrue(Constant(ConstInt(1)) \".loop@0_start\")\n",
            "Label(\".loop@0_break\")\n"},
        }
      },
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}");
}

TEST_CASE("c99_session: test_c99_to_tac.py::TestTranslateLoops::test_for_basic_shape (call 1)") {
  c6502::testing::check_c99_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}",
      {
        {
          "translate_statement",
          "ForStmt(InitExp(None) None None Null \".loop@0\" None)\n",
          true,
          {},
          "",
          {
            "Label(\".loop@0_start\")\n",
            "Label(\".loop@0_continue\")\n",
            "Jump(\".loop@0_start\")\n",
            "Label(\".loop@0_break\")\n"},
        }
      },
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}");
}

TEST_CASE("c99_session: test_c99_to_tac.py::TestTranslateLoops::test_for_full_header (call 1)") {
  c6502::testing::check_c99_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}",
      {
        {
          "translate_statement",
          "ForStmt(InitExp(Assignment(Var(\"i\" None) Constant(ConstInt(0) None) None)) Constant(ConstInt(1) None) Postfix(Increment Var(\"i\" None) None) BreakStmt(\".loop@0\") \".loop@0\" None)\n",
          true,
          {},
          "",
          {
            "Copy(Constant(ConstInt(0)) Var(\"i\"))\n",
            "Label(\".loop@0_start\")\n",
            "JumpIfFalse(Constant(ConstInt(1)) \".loop@0_break\")\n",
            "Jump(\".loop@0_break\")\n",
            "Label(\".loop@0_continue\")\n",
            "Copy(Var(\"i\") Var(\"%0\"))\n",
            "Binary(Add Var(\"i\") Constant(ConstInt(1)) Var(\"%1\"))\n",
            "Copy(Var(\"%1\") Var(\"i\"))\n",
            "Jump(\".loop@0_start\")\n",
            "Label(\".loop@0_break\")\n"},
        }
      },
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}");
}

TEST_CASE("c99_session: test_c99_to_tac.py::TestTranslateLoops::test_for_init_decl (call 1)") {
  c6502::testing::check_c99_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}",
      {
        {
          "translate_statement",
          "ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(0) None) Int None None None None false)]) None None Null \".loop@0\" None)\n",
          true,
          {},
          "",
          {
            "Copy(Constant(ConstInt(0)) Var(\"i\"))\n",
            "Label(\".loop@0_start\")\n",
            "Label(\".loop@0_continue\")\n",
            "Jump(\".loop@0_start\")\n",
            "Label(\".loop@0_break\")\n"},
        }
      },
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}");
}

TEST_CASE("c99_session: test_c99_to_tac.py::TestTranslateLoops::test_for_init_decl_without_initializer_emits_nothing (call 1)") {
  c6502::testing::check_c99_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}",
      {
        {
          "translate_statement",
          "ForStmt(InitDecl([var_decl(\"i\" None Int None None None None false)]) None None Null \".loop@0\" None)\n",
          true,
          {},
          "",
          {
            "Label(\".loop@0_start\")\n",
            "Label(\".loop@0_continue\")\n",
            "Jump(\".loop@0_start\")\n",
            "Label(\".loop@0_break\")\n"},
        }
      },
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}");
}

TEST_CASE("c99_session: test_c99_to_tac.py::TestTranslateLoops::test_for_no_condition_omits_jump_if_false (call 1)") {
  c6502::testing::check_c99_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}",
      {
        {
          "translate_statement",
          "ForStmt(InitExp(None) None None BreakStmt(\".loop@0\") \".loop@0\" None)\n",
          true,
          {},
          "",
          {
            "Label(\".loop@0_start\")\n",
            "Jump(\".loop@0_break\")\n",
            "Label(\".loop@0_continue\")\n",
            "Jump(\".loop@0_start\")\n",
            "Label(\".loop@0_break\")\n"},
        }
      },
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}");
}

TEST_CASE("c99_session: test_c99_to_tac.py::TestTranslateLoops::test_for_no_post_omits_post_insns (call 1)") {
  c6502::testing::check_c99_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}",
      {
        {
          "translate_statement",
          "ForStmt(InitExp(None) Constant(ConstInt(1) None) None Null \".loop@0\" None)\n",
          true,
          {},
          "",
          {
            "Label(\".loop@0_start\")\n",
            "JumpIfFalse(Constant(ConstInt(1)) \".loop@0_break\")\n",
            "Label(\".loop@0_continue\")\n",
            "Jump(\".loop@0_start\")\n",
            "Label(\".loop@0_break\")\n"},
        }
      },
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}");
}

TEST_CASE("c99_session: test_c99_to_tac.py::TestTranslateLoops::test_for_post_clause_result_is_discarded (call 1)") {
  c6502::testing::check_c99_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}",
      {
        {
          "translate_statement",
          "ForStmt(InitExp(None) None Postfix(Increment Var(\"i\" None) None) Null \".loop@0\" None)\n",
          true,
          {},
          "",
          {
            "Label(\".loop@0_start\")\n",
            "Label(\".loop@0_continue\")\n",
            "Copy(Var(\"i\") Var(\"%0\"))\n",
            "Binary(Add Var(\"i\") Constant(ConstInt(1)) Var(\"%1\"))\n",
            "Copy(Var(\"%1\") Var(\"i\"))\n",
            "Jump(\".loop@0_start\")\n",
            "Label(\".loop@0_break\")\n"},
        }
      },
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}");
}

TEST_CASE("c99_session: test_c99_to_tac.py::TestTranslateLoops::test_while_basic_shape (call 1)") {
  c6502::testing::check_c99_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}",
      {
        {
          "translate_statement",
          "WhileStmt(Constant(ConstInt(1) None) Null \".loop@0\")\n",
          true,
          {},
          "",
          {
            "Label(\".loop@0_continue\")\n",
            "JumpIfFalse(Constant(ConstInt(1)) \".loop@0_break\")\n",
            "Jump(\".loop@0_continue\")\n",
            "Label(\".loop@0_break\")\n"},
        }
      },
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}");
}

TEST_CASE("c99_session: test_c99_to_tac.py::TestTranslateLoops::test_while_break_continue_jump_to_sub_labels (call 1)") {
  c6502::testing::check_c99_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}",
      {
        {
          "translate_statement",
          "WhileStmt(Constant(ConstInt(1) None) Compound(Block([\n"
          "  S(BreakStmt(\".loop@0\"))\n"
          "  S(ContinueStmt(\".loop@0\"))\n"
          "])) \".loop@0\")\n",
          true,
          {},
          "",
          {
            "Label(\".loop@0_continue\")\n",
            "JumpIfFalse(Constant(ConstInt(1)) \".loop@0_break\")\n",
            "Jump(\".loop@0_break\")\n",
            "Jump(\".loop@0_continue\")\n",
            "Jump(\".loop@0_continue\")\n",
            "Label(\".loop@0_break\")\n"},
        }
      },
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}");
}

TEST_CASE("c99_session: test_c99_to_tac.py::TestTranslateBreakContinueDirectly::test_break_emits_jump_to_break_label (call 1)") {
  c6502::testing::check_c99_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}",
      {
        {
          "translate_statement",
          "BreakStmt(\".loop@3\")\n",
          true,
          {},
          "",
          {
            "Jump(\".loop@3_break\")\n"},
        }
      },
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}");
}

TEST_CASE("c99_session: test_c99_to_tac.py::TestTranslateBreakContinueDirectly::test_continue_emits_jump_to_continue_label (call 1)") {
  c6502::testing::check_c99_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}",
      {
        {
          "translate_statement",
          "ContinueStmt(\".loop@3\")\n",
          true,
          {},
          "",
          {
            "Jump(\".loop@3_continue\")\n"},
        }
      },
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}");
}

TEST_CASE("c99_session: test_c99_to_tac.py::TestTranslateNestedLoops::test_inner_break_targets_inner_outer_break_targets_outer (call 1)") {
  c6502::testing::check_c99_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}",
      {
        {
          "translate_statement",
          "WhileStmt(Constant(ConstInt(1) None) Compound(Block([\n"
          "  S(WhileStmt(Constant(ConstInt(1) None) BreakStmt(\".loop@1\") \".loop@1\"))\n"
          "  S(BreakStmt(\".loop@0\"))\n"
          "])) \".loop@0\")\n",
          true,
          {},
          "",
          {
            "Label(\".loop@0_continue\")\n",
            "JumpIfFalse(Constant(ConstInt(1)) \".loop@0_break\")\n",
            "Label(\".loop@1_continue\")\n",
            "JumpIfFalse(Constant(ConstInt(1)) \".loop@1_break\")\n",
            "Jump(\".loop@1_break\")\n",
            "Jump(\".loop@1_continue\")\n",
            "Label(\".loop@1_break\")\n",
            "Jump(\".loop@0_break\")\n",
            "Jump(\".loop@0_continue\")\n",
            "Label(\".loop@0_break\")\n"},
        }
      },
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}");
}

TEST_CASE("parse: test_c99_to_tac.py::TestEndToEndLoops::test_for_with_continue_jumps_to_continue_label (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c99_to_tac.py::TestEndToEndLoops::test_for_with_continue_jumps_to_continue_label\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { for (;;) continue; return 0; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitExp(None) None None ContinueStmt(\"\") \"\" None))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_c99_to_tac.py::TestEndToEndLoops::test_for_with_continue_jumps_to_continue_label (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_c99_to_tac.py::TestEndToEndLoops::test_for_with_continue_jumps_to_continue_label\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitExp(None) None None ContinueStmt(\"\") \"\" None))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitExp(None) None None ContinueStmt(\"\") \"\" None))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_c99_to_tac.py::TestEndToEndLoops::test_for_with_continue_jumps_to_continue_label (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_c99_to_tac.py::TestEndToEndLoops::test_for_with_continue_jumps_to_continue_label\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitExp(None) None None ContinueStmt(\"\") \"\" None))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitExp(None) None None ContinueStmt(\"\") \"\" None))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_c99_to_tac.py::TestEndToEndLoops::test_for_with_continue_jumps_to_continue_label (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_c99_to_tac.py::TestEndToEndLoops::test_for_with_continue_jumps_to_continue_label\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitExp(None) None None ContinueStmt(\"\") \"\" None))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitExp(None) None None ContinueStmt(\".loop@0\") \".loop@0\" None))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_c99_to_tac.py::TestEndToEndLoops::test_for_with_continue_jumps_to_continue_label (call 5)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c99_to_tac.py::TestEndToEndLoops::test_for_with_continue_jumps_to_continue_label\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(ForStmt(InitExp(None) None None ContinueStmt(\\\".loop@0\\\") \\\".loop@0\\\" None))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitExp(None) None None ContinueStmt(\".loop@0\") \".loop@0\" None))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(ForStmt(InitExp(None) None None ContinueStmt(\\\".loop@0\\\") \\\".loop@0\\\" None))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("c99_to_tac: test_c99_to_tac.py::TestEndToEndLoops::test_for_with_continue_jumps_to_continue_label (call 6)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass c99_to_tac\n"
      "source tests/test_c99_to_tac.py::TestEndToEndLoops::test_for_with_continue_jumps_to_continue_label\n"
      "args {\"args\": [{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitExp(None) None None ContinueStmt(\".loop@0\") \".loop@0\" None))\n"
      "    S(Return(Constant(ConstInt(0) Int)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Label(\".loop@0_start\")\n"
      "    Jump(\".loop@0_continue\")\n"
      "    Label(\".loop@0_continue\")\n"
      "    Jump(\".loop@0_start\")\n"
      "    Label(\".loop@0_break\")\n"
      "    Ret(Constant(ConstInt(0)))\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("parse: test_c99_to_tac.py::TestEndToEndLoops::test_nested_loops_get_distinct_sub_labels (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c99_to_tac.py::TestEndToEndLoops::test_nested_loops_get_distinct_sub_labels\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { while (1) { while (1) break; break; } return 0; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(WhileStmt(Constant(ConstInt(1) None) Compound(Block([\n"
      "      S(WhileStmt(Constant(ConstInt(1) None) BreakStmt(\"\") \"\"))\n"
      "      S(BreakStmt(\"\"))\n"
      "    ])) \"\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_c99_to_tac.py::TestEndToEndLoops::test_nested_loops_get_distinct_sub_labels (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_c99_to_tac.py::TestEndToEndLoops::test_nested_loops_get_distinct_sub_labels\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(WhileStmt(Constant(ConstInt(1) None) Compound(Block([\n"
      "      S(WhileStmt(Constant(ConstInt(1) None) BreakStmt(\"\") \"\"))\n"
      "      S(BreakStmt(\"\"))\n"
      "    ])) \"\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(WhileStmt(Constant(ConstInt(1) None) Compound(Block([\n"
      "      S(WhileStmt(Constant(ConstInt(1) None) BreakStmt(\"\") \"\"))\n"
      "      S(BreakStmt(\"\"))\n"
      "    ])) \"\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_c99_to_tac.py::TestEndToEndLoops::test_nested_loops_get_distinct_sub_labels (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_c99_to_tac.py::TestEndToEndLoops::test_nested_loops_get_distinct_sub_labels\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(WhileStmt(Constant(ConstInt(1) None) Compound(Block([\n"
      "      S(WhileStmt(Constant(ConstInt(1) None) BreakStmt(\"\") \"\"))\n"
      "      S(BreakStmt(\"\"))\n"
      "    ])) \"\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(WhileStmt(Constant(ConstInt(1) None) Compound(Block([\n"
      "      S(WhileStmt(Constant(ConstInt(1) None) BreakStmt(\"\") \"\"))\n"
      "      S(BreakStmt(\"\"))\n"
      "    ])) \"\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_c99_to_tac.py::TestEndToEndLoops::test_nested_loops_get_distinct_sub_labels (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_c99_to_tac.py::TestEndToEndLoops::test_nested_loops_get_distinct_sub_labels\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(WhileStmt(Constant(ConstInt(1) None) Compound(Block([\n"
      "      S(WhileStmt(Constant(ConstInt(1) None) BreakStmt(\"\") \"\"))\n"
      "      S(BreakStmt(\"\"))\n"
      "    ])) \"\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(WhileStmt(Constant(ConstInt(1) None) Compound(Block([\n"
      "      S(WhileStmt(Constant(ConstInt(1) None) BreakStmt(\".loop@1\") \".loop@1\"))\n"
      "      S(BreakStmt(\".loop@0\"))\n"
      "    ])) \".loop@0\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_c99_to_tac.py::TestEndToEndLoops::test_nested_loops_get_distinct_sub_labels (call 5)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c99_to_tac.py::TestEndToEndLoops::test_nested_loops_get_distinct_sub_labels\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(WhileStmt(Constant(ConstInt(1) Int) Compound(Block([\\n      S(WhileStmt(Constant(ConstInt(1) Int) BreakStmt(\\\".loop@1\\\") \\\".loop@1\\\"))\\n      S(BreakStmt(\\\".loop@0\\\"))\\n    ])) \\\".loop@0\\\"))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(WhileStmt(Constant(ConstInt(1) None) Compound(Block([\n"
      "      S(WhileStmt(Constant(ConstInt(1) None) BreakStmt(\".loop@1\") \".loop@1\"))\n"
      "      S(BreakStmt(\".loop@0\"))\n"
      "    ])) \".loop@0\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(WhileStmt(Constant(ConstInt(1) Int) Compound(Block([\\n      S(WhileStmt(Constant(ConstInt(1) Int) BreakStmt(\\\".loop@1\\\") \\\".loop@1\\\"))\\n      S(BreakStmt(\\\".loop@0\\\"))\\n    ])) \\\".loop@0\\\"))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("c99_to_tac: test_c99_to_tac.py::TestEndToEndLoops::test_nested_loops_get_distinct_sub_labels (call 6)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass c99_to_tac\n"
      "source tests/test_c99_to_tac.py::TestEndToEndLoops::test_nested_loops_get_distinct_sub_labels\n"
      "args {\"args\": [{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(WhileStmt(Constant(ConstInt(1) Int) Compound(Block([\n"
      "      S(WhileStmt(Constant(ConstInt(1) Int) BreakStmt(\".loop@1\") \".loop@1\"))\n"
      "      S(BreakStmt(\".loop@0\"))\n"
      "    ])) \".loop@0\"))\n"
      "    S(Return(Constant(ConstInt(0) Int)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Label(\".loop@0_continue\")\n"
      "    JumpIfFalse(Constant(ConstInt(1)) \".loop@0_break\")\n"
      "    Label(\".loop@1_continue\")\n"
      "    JumpIfFalse(Constant(ConstInt(1)) \".loop@1_break\")\n"
      "    Jump(\".loop@1_break\")\n"
      "    Jump(\".loop@1_continue\")\n"
      "    Label(\".loop@1_break\")\n"
      "    Jump(\".loop@0_break\")\n"
      "    Jump(\".loop@0_continue\")\n"
      "    Label(\".loop@0_break\")\n"
      "    Ret(Constant(ConstInt(0)))\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("parse: test_c99_to_tac.py::TestEndToEndLoops::test_while_with_break_jumps_to_break_label (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c99_to_tac.py::TestEndToEndLoops::test_while_with_break_jumps_to_break_label\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { while (1) break; return 0; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(WhileStmt(Constant(ConstInt(1) None) BreakStmt(\"\") \"\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_c99_to_tac.py::TestEndToEndLoops::test_while_with_break_jumps_to_break_label (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_c99_to_tac.py::TestEndToEndLoops::test_while_with_break_jumps_to_break_label\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(WhileStmt(Constant(ConstInt(1) None) BreakStmt(\"\") \"\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(WhileStmt(Constant(ConstInt(1) None) BreakStmt(\"\") \"\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_c99_to_tac.py::TestEndToEndLoops::test_while_with_break_jumps_to_break_label (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_c99_to_tac.py::TestEndToEndLoops::test_while_with_break_jumps_to_break_label\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(WhileStmt(Constant(ConstInt(1) None) BreakStmt(\"\") \"\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(WhileStmt(Constant(ConstInt(1) None) BreakStmt(\"\") \"\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_c99_to_tac.py::TestEndToEndLoops::test_while_with_break_jumps_to_break_label (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_c99_to_tac.py::TestEndToEndLoops::test_while_with_break_jumps_to_break_label\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(WhileStmt(Constant(ConstInt(1) None) BreakStmt(\"\") \"\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(WhileStmt(Constant(ConstInt(1) None) BreakStmt(\".loop@0\") \".loop@0\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_c99_to_tac.py::TestEndToEndLoops::test_while_with_break_jumps_to_break_label (call 5)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c99_to_tac.py::TestEndToEndLoops::test_while_with_break_jumps_to_break_label\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(WhileStmt(Constant(ConstInt(1) Int) BreakStmt(\\\".loop@0\\\") \\\".loop@0\\\"))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(WhileStmt(Constant(ConstInt(1) None) BreakStmt(\".loop@0\") \".loop@0\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(WhileStmt(Constant(ConstInt(1) Int) BreakStmt(\\\".loop@0\\\") \\\".loop@0\\\"))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("c99_to_tac: test_c99_to_tac.py::TestEndToEndLoops::test_while_with_break_jumps_to_break_label (call 6)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass c99_to_tac\n"
      "source tests/test_c99_to_tac.py::TestEndToEndLoops::test_while_with_break_jumps_to_break_label\n"
      "args {\"args\": [{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(WhileStmt(Constant(ConstInt(1) Int) BreakStmt(\".loop@0\") \".loop@0\"))\n"
      "    S(Return(Constant(ConstInt(0) Int)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Label(\".loop@0_continue\")\n"
      "    JumpIfFalse(Constant(ConstInt(1)) \".loop@0_break\")\n"
      "    Jump(\".loop@0_break\")\n"
      "    Jump(\".loop@0_continue\")\n"
      "    Label(\".loop@0_break\")\n"
      "    Ret(Constant(ConstInt(0)))\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("parse: test_c99_to_tac.py::TestEndToEndSwitch::test_break_in_switch_targets_break_label (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c99_to_tac.py::TestEndToEndSwitch::test_break_in_switch_targets_break_label\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { switch (1) { case 1: break; default: return 0; } return 9; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(SwitchStmt(Constant(ConstInt(1) None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstInt(1) None) BreakStmt(\"\") \"\"))\n"
      "      S(DefaultStmt(Return(Constant(ConstInt(0) None)) \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "    S(Return(Constant(ConstInt(9) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_c99_to_tac.py::TestEndToEndSwitch::test_break_in_switch_targets_break_label (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_c99_to_tac.py::TestEndToEndSwitch::test_break_in_switch_targets_break_label\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(SwitchStmt(Constant(ConstInt(1) None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstInt(1) None) BreakStmt(\"\") \"\"))\n"
      "      S(DefaultStmt(Return(Constant(ConstInt(0) None)) \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "    S(Return(Constant(ConstInt(9) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(SwitchStmt(Constant(ConstInt(1) None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstInt(1) None) BreakStmt(\"\") \"\"))\n"
      "      S(DefaultStmt(Return(Constant(ConstInt(0) None)) \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "    S(Return(Constant(ConstInt(9) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_c99_to_tac.py::TestEndToEndSwitch::test_break_in_switch_targets_break_label (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_c99_to_tac.py::TestEndToEndSwitch::test_break_in_switch_targets_break_label\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(SwitchStmt(Constant(ConstInt(1) None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstInt(1) None) BreakStmt(\"\") \"\"))\n"
      "      S(DefaultStmt(Return(Constant(ConstInt(0) None)) \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "    S(Return(Constant(ConstInt(9) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(SwitchStmt(Constant(ConstInt(1) None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstInt(1) None) BreakStmt(\"\") \"\"))\n"
      "      S(DefaultStmt(Return(Constant(ConstInt(0) None)) \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "    S(Return(Constant(ConstInt(9) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_c99_to_tac.py::TestEndToEndSwitch::test_break_in_switch_targets_break_label (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_c99_to_tac.py::TestEndToEndSwitch::test_break_in_switch_targets_break_label\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(SwitchStmt(Constant(ConstInt(1) None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstInt(1) None) BreakStmt(\"\") \"\"))\n"
      "      S(DefaultStmt(Return(Constant(ConstInt(0) None)) \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "    S(Return(Constant(ConstInt(9) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(SwitchStmt(Constant(ConstInt(1) None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstInt(1) None) BreakStmt(\".switch@0\") \".case@1\"))\n"
      "      S(DefaultStmt(Return(Constant(ConstInt(0) None)) \".default@2\"))\n"
      "    ])) \".switch@0\" [switch_case(Constant(ConstInt(1) None) \".case@1\")] \".default@2\" None))\n"
      "    S(Return(Constant(ConstInt(9) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_c99_to_tac.py::TestEndToEndSwitch::test_break_in_switch_targets_break_label (call 5)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c99_to_tac.py::TestEndToEndSwitch::test_break_in_switch_targets_break_label\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(SwitchStmt(Constant(ConstInt(1) Int) Compound(Block([\\n      S(CaseStmt(Constant(ConstInt(1) Int) BreakStmt(\\\".switch@0\\\") \\\".case@1\\\"))\\n      S(DefaultStmt(Return(Constant(ConstInt(0) Int)) \\\".default@2\\\"))\\n    ])) \\\".switch@0\\\" [switch_case(Constant(ConstInt(1) Int) \\\".case@1\\\")] \\\".default@2\\\" Int))\\n    S(Return(Constant(ConstInt(9) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(SwitchStmt(Constant(ConstInt(1) None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstInt(1) None) BreakStmt(\".switch@0\") \".case@1\"))\n"
      "      S(DefaultStmt(Return(Constant(ConstInt(0) None)) \".default@2\"))\n"
      "    ])) \".switch@0\" [switch_case(Constant(ConstInt(1) None) \".case@1\")] \".default@2\" None))\n"
      "    S(Return(Constant(ConstInt(9) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(SwitchStmt(Constant(ConstInt(1) Int) Compound(Block([\\n      S(CaseStmt(Constant(ConstInt(1) Int) BreakStmt(\\\".switch@0\\\") \\\".case@1\\\"))\\n      S(DefaultStmt(Return(Constant(ConstInt(0) Int)) \\\".default@2\\\"))\\n    ])) \\\".switch@0\\\" [switch_case(Constant(ConstInt(1) Int) \\\".case@1\\\")] \\\".default@2\\\" Int))\\n    S(Return(Constant(ConstInt(9) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("c99_session: test_c99_to_tac.py::TestEndToEndSwitch::test_break_in_switch_targets_break_label (call 6)") {
  c6502::testing::check_c99_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}",
      {
        {
          "translate_program",
          "Program([\n"
          "  FunctionDecl(function_decl(\"main\" [] Block([\n"
          "    S(SwitchStmt(Constant(ConstInt(1) Int) Compound(Block([\n"
          "      S(CaseStmt(Constant(ConstInt(1) Int) BreakStmt(\".switch@0\") \".case@1\"))\n"
          "      S(DefaultStmt(Return(Constant(ConstInt(0) Int)) \".default@2\"))\n"
          "    ])) \".switch@0\" [switch_case(Constant(ConstInt(1) Int) \".case@1\")] \".default@2\" Int))\n"
          "    S(Return(Constant(ConstInt(9) Int)))\n"
          "  ]) FunType([] Int) None None [] false None [] None false))\n"
          "])\n",
          false,
          {},
          "Program([\n"
          "  Function(\"main\" true [] [\n"
          "    Binary(Equal Constant(ConstInt(1)) Constant(ConstInt(1)) Var(\"%0\"))\n"
          "    JumpIfTrue(Var(\"%0\") \".case@1\")\n"
          "    Jump(\".default@2\")\n"
          "    Label(\".case@1\")\n"
          "    Jump(\".switch@0_break\")\n"
          "    Label(\".default@2\")\n"
          "    Ret(Constant(ConstInt(0)))\n"
          "    Label(\".switch@0_break\")\n"
          "    Ret(Constant(ConstInt(9)))\n"
          "  ])\n"
          "])\n",
          {},
        }
      },
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}");
}

TEST_CASE("parse: test_c99_to_tac.py::TestEndToEndSwitch::test_dispatch_chain_then_body_then_break (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c99_to_tac.py::TestEndToEndSwitch::test_dispatch_chain_then_body_then_break\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { switch (3) { case 0: return 0; case 5: return 5; default: return 99; } }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(SwitchStmt(Constant(ConstInt(3) None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstInt(0) None) Return(Constant(ConstInt(0) None)) \"\"))\n"
      "      S(CaseStmt(Constant(ConstInt(5) None) Return(Constant(ConstInt(5) None)) \"\"))\n"
      "      S(DefaultStmt(Return(Constant(ConstInt(99) None)) \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_c99_to_tac.py::TestEndToEndSwitch::test_dispatch_chain_then_body_then_break (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_c99_to_tac.py::TestEndToEndSwitch::test_dispatch_chain_then_body_then_break\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(SwitchStmt(Constant(ConstInt(3) None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstInt(0) None) Return(Constant(ConstInt(0) None)) \"\"))\n"
      "      S(CaseStmt(Constant(ConstInt(5) None) Return(Constant(ConstInt(5) None)) \"\"))\n"
      "      S(DefaultStmt(Return(Constant(ConstInt(99) None)) \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(SwitchStmt(Constant(ConstInt(3) None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstInt(0) None) Return(Constant(ConstInt(0) None)) \"\"))\n"
      "      S(CaseStmt(Constant(ConstInt(5) None) Return(Constant(ConstInt(5) None)) \"\"))\n"
      "      S(DefaultStmt(Return(Constant(ConstInt(99) None)) \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_c99_to_tac.py::TestEndToEndSwitch::test_dispatch_chain_then_body_then_break (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_c99_to_tac.py::TestEndToEndSwitch::test_dispatch_chain_then_body_then_break\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(SwitchStmt(Constant(ConstInt(3) None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstInt(0) None) Return(Constant(ConstInt(0) None)) \"\"))\n"
      "      S(CaseStmt(Constant(ConstInt(5) None) Return(Constant(ConstInt(5) None)) \"\"))\n"
      "      S(DefaultStmt(Return(Constant(ConstInt(99) None)) \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(SwitchStmt(Constant(ConstInt(3) None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstInt(0) None) Return(Constant(ConstInt(0) None)) \"\"))\n"
      "      S(CaseStmt(Constant(ConstInt(5) None) Return(Constant(ConstInt(5) None)) \"\"))\n"
      "      S(DefaultStmt(Return(Constant(ConstInt(99) None)) \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_c99_to_tac.py::TestEndToEndSwitch::test_dispatch_chain_then_body_then_break (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_c99_to_tac.py::TestEndToEndSwitch::test_dispatch_chain_then_body_then_break\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(SwitchStmt(Constant(ConstInt(3) None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstInt(0) None) Return(Constant(ConstInt(0) None)) \"\"))\n"
      "      S(CaseStmt(Constant(ConstInt(5) None) Return(Constant(ConstInt(5) None)) \"\"))\n"
      "      S(DefaultStmt(Return(Constant(ConstInt(99) None)) \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(SwitchStmt(Constant(ConstInt(3) None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstInt(0) None) Return(Constant(ConstInt(0) None)) \".case@1\"))\n"
      "      S(CaseStmt(Constant(ConstInt(5) None) Return(Constant(ConstInt(5) None)) \".case@2\"))\n"
      "      S(DefaultStmt(Return(Constant(ConstInt(99) None)) \".default@3\"))\n"
      "    ])) \".switch@0\" [switch_case(Constant(ConstInt(0) None) \".case@1\") switch_case(Constant(ConstInt(5) None) \".case@2\")] \".default@3\" None))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_c99_to_tac.py::TestEndToEndSwitch::test_dispatch_chain_then_body_then_break (call 5)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c99_to_tac.py::TestEndToEndSwitch::test_dispatch_chain_then_body_then_break\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(SwitchStmt(Constant(ConstInt(3) Int) Compound(Block([\\n      S(CaseStmt(Constant(ConstInt(0) Int) Return(Constant(ConstInt(0) Int)) \\\".case@1\\\"))\\n      S(CaseStmt(Constant(ConstInt(5) Int) Return(Constant(ConstInt(5) Int)) \\\".case@2\\\"))\\n      S(DefaultStmt(Return(Constant(ConstInt(99) Int)) \\\".default@3\\\"))\\n    ])) \\\".switch@0\\\" [switch_case(Constant(ConstInt(0) Int) \\\".case@1\\\") switch_case(Constant(ConstInt(5) Int) \\\".case@2\\\")] \\\".default@3\\\" Int))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(SwitchStmt(Constant(ConstInt(3) None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstInt(0) None) Return(Constant(ConstInt(0) None)) \".case@1\"))\n"
      "      S(CaseStmt(Constant(ConstInt(5) None) Return(Constant(ConstInt(5) None)) \".case@2\"))\n"
      "      S(DefaultStmt(Return(Constant(ConstInt(99) None)) \".default@3\"))\n"
      "    ])) \".switch@0\" [switch_case(Constant(ConstInt(0) None) \".case@1\") switch_case(Constant(ConstInt(5) None) \".case@2\")] \".default@3\" None))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(SwitchStmt(Constant(ConstInt(3) Int) Compound(Block([\\n      S(CaseStmt(Constant(ConstInt(0) Int) Return(Constant(ConstInt(0) Int)) \\\".case@1\\\"))\\n      S(CaseStmt(Constant(ConstInt(5) Int) Return(Constant(ConstInt(5) Int)) \\\".case@2\\\"))\\n      S(DefaultStmt(Return(Constant(ConstInt(99) Int)) \\\".default@3\\\"))\\n    ])) \\\".switch@0\\\" [switch_case(Constant(ConstInt(0) Int) \\\".case@1\\\") switch_case(Constant(ConstInt(5) Int) \\\".case@2\\\")] \\\".default@3\\\" Int))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("c99_session: test_c99_to_tac.py::TestEndToEndSwitch::test_dispatch_chain_then_body_then_break (call 6)") {
  c6502::testing::check_c99_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}",
      {
        {
          "translate_program",
          "Program([\n"
          "  FunctionDecl(function_decl(\"main\" [] Block([\n"
          "    S(SwitchStmt(Constant(ConstInt(3) Int) Compound(Block([\n"
          "      S(CaseStmt(Constant(ConstInt(0) Int) Return(Constant(ConstInt(0) Int)) \".case@1\"))\n"
          "      S(CaseStmt(Constant(ConstInt(5) Int) Return(Constant(ConstInt(5) Int)) \".case@2\"))\n"
          "      S(DefaultStmt(Return(Constant(ConstInt(99) Int)) \".default@3\"))\n"
          "    ])) \".switch@0\" [switch_case(Constant(ConstInt(0) Int) \".case@1\") switch_case(Constant(ConstInt(5) Int) \".case@2\")] \".default@3\" Int))\n"
          "  ]) FunType([] Int) None None [] false None [] None false))\n"
          "])\n",
          false,
          {},
          "Program([\n"
          "  Function(\"main\" true [] [\n"
          "    Binary(Equal Constant(ConstInt(3)) Constant(ConstInt(0)) Var(\"%0\"))\n"
          "    JumpIfTrue(Var(\"%0\") \".case@1\")\n"
          "    Binary(Equal Constant(ConstInt(3)) Constant(ConstInt(5)) Var(\"%1\"))\n"
          "    JumpIfTrue(Var(\"%1\") \".case@2\")\n"
          "    Jump(\".default@3\")\n"
          "    Label(\".case@1\")\n"
          "    Ret(Constant(ConstInt(0)))\n"
          "    Label(\".case@2\")\n"
          "    Ret(Constant(ConstInt(5)))\n"
          "    Label(\".default@3\")\n"
          "    Ret(Constant(ConstInt(99)))\n"
          "    Label(\".switch@0_break\")\n"
          "    Ret(Constant(ConstInt(0)))\n"
          "  ])\n"
          "])\n",
          {},
        }
      },
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}");
}

TEST_CASE("parse: test_c99_to_tac.py::TestEndToEndSwitch::test_no_default_falls_through_to_break_label (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c99_to_tac.py::TestEndToEndSwitch::test_no_default_falls_through_to_break_label\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { switch (3) { case 0: return 0; } return 9; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(SwitchStmt(Constant(ConstInt(3) None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstInt(0) None) Return(Constant(ConstInt(0) None)) \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "    S(Return(Constant(ConstInt(9) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_c99_to_tac.py::TestEndToEndSwitch::test_no_default_falls_through_to_break_label (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_c99_to_tac.py::TestEndToEndSwitch::test_no_default_falls_through_to_break_label\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(SwitchStmt(Constant(ConstInt(3) None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstInt(0) None) Return(Constant(ConstInt(0) None)) \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "    S(Return(Constant(ConstInt(9) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(SwitchStmt(Constant(ConstInt(3) None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstInt(0) None) Return(Constant(ConstInt(0) None)) \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "    S(Return(Constant(ConstInt(9) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_c99_to_tac.py::TestEndToEndSwitch::test_no_default_falls_through_to_break_label (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_c99_to_tac.py::TestEndToEndSwitch::test_no_default_falls_through_to_break_label\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(SwitchStmt(Constant(ConstInt(3) None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstInt(0) None) Return(Constant(ConstInt(0) None)) \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "    S(Return(Constant(ConstInt(9) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(SwitchStmt(Constant(ConstInt(3) None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstInt(0) None) Return(Constant(ConstInt(0) None)) \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "    S(Return(Constant(ConstInt(9) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_c99_to_tac.py::TestEndToEndSwitch::test_no_default_falls_through_to_break_label (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_c99_to_tac.py::TestEndToEndSwitch::test_no_default_falls_through_to_break_label\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(SwitchStmt(Constant(ConstInt(3) None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstInt(0) None) Return(Constant(ConstInt(0) None)) \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "    S(Return(Constant(ConstInt(9) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(SwitchStmt(Constant(ConstInt(3) None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstInt(0) None) Return(Constant(ConstInt(0) None)) \".case@1\"))\n"
      "    ])) \".switch@0\" [switch_case(Constant(ConstInt(0) None) \".case@1\")] None None))\n"
      "    S(Return(Constant(ConstInt(9) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_c99_to_tac.py::TestEndToEndSwitch::test_no_default_falls_through_to_break_label (call 5)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c99_to_tac.py::TestEndToEndSwitch::test_no_default_falls_through_to_break_label\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(SwitchStmt(Constant(ConstInt(3) Int) Compound(Block([\\n      S(CaseStmt(Constant(ConstInt(0) Int) Return(Constant(ConstInt(0) Int)) \\\".case@1\\\"))\\n    ])) \\\".switch@0\\\" [switch_case(Constant(ConstInt(0) Int) \\\".case@1\\\")] None Int))\\n    S(Return(Constant(ConstInt(9) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(SwitchStmt(Constant(ConstInt(3) None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstInt(0) None) Return(Constant(ConstInt(0) None)) \".case@1\"))\n"
      "    ])) \".switch@0\" [switch_case(Constant(ConstInt(0) None) \".case@1\")] None None))\n"
      "    S(Return(Constant(ConstInt(9) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(SwitchStmt(Constant(ConstInt(3) Int) Compound(Block([\\n      S(CaseStmt(Constant(ConstInt(0) Int) Return(Constant(ConstInt(0) Int)) \\\".case@1\\\"))\\n    ])) \\\".switch@0\\\" [switch_case(Constant(ConstInt(0) Int) \\\".case@1\\\")] None Int))\\n    S(Return(Constant(ConstInt(9) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("c99_session: test_c99_to_tac.py::TestEndToEndSwitch::test_no_default_falls_through_to_break_label (call 6)") {
  c6502::testing::check_c99_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}",
      {
        {
          "translate_program",
          "Program([\n"
          "  FunctionDecl(function_decl(\"main\" [] Block([\n"
          "    S(SwitchStmt(Constant(ConstInt(3) Int) Compound(Block([\n"
          "      S(CaseStmt(Constant(ConstInt(0) Int) Return(Constant(ConstInt(0) Int)) \".case@1\"))\n"
          "    ])) \".switch@0\" [switch_case(Constant(ConstInt(0) Int) \".case@1\")] None Int))\n"
          "    S(Return(Constant(ConstInt(9) Int)))\n"
          "  ]) FunType([] Int) None None [] false None [] None false))\n"
          "])\n",
          false,
          {},
          "Program([\n"
          "  Function(\"main\" true [] [\n"
          "    Binary(Equal Constant(ConstInt(3)) Constant(ConstInt(0)) Var(\"%0\"))\n"
          "    JumpIfTrue(Var(\"%0\") \".case@1\")\n"
          "    Jump(\".switch@0_break\")\n"
          "    Label(\".case@1\")\n"
          "    Ret(Constant(ConstInt(0)))\n"
          "    Label(\".switch@0_break\")\n"
          "    Ret(Constant(ConstInt(9)))\n"
          "  ])\n"
          "])\n",
          {},
        }
      },
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}");
}

TEST_CASE("c99_session: test_c99_to_tac.py::TestTranslateFunctionCall::test_args_evaluated_left_to_right (call 1)") {
  c6502::testing::check_c99_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}",
      {
        {
          "translate_exp",
          "FunctionCall(\"f\" [FunctionCall(\"g\" [Constant(ConstInt(1) None)] None false) FunctionCall(\"g\" [Constant(ConstInt(2) None)] None false)] None false)\n",
          true,
          {},
          "Var(\"%2\")\n",
          {
            "FunctionCall(\"g\" [Constant(ConstInt(1))] Var(\"%0\") false false)\n",
            "FunctionCall(\"g\" [Constant(ConstInt(2))] Var(\"%1\") false false)\n",
            "FunctionCall(\"f\" [Var(\"%0\") Var(\"%1\")] Var(\"%2\") false false)\n"},
        }
      },
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}");
}

TEST_CASE("c99_session: test_c99_to_tac.py::TestTranslateFunctionCall::test_call_in_return_position_passes_value (call 1)") {
  c6502::testing::check_c99_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}",
      {
        {
          "translate_statement",
          "Return(FunctionCall(\"f\" [] None false))\n",
          true,
          {},
          "",
          {
            "FunctionCall(\"f\" [] Var(\"%0\") false false)\n",
            "Ret(Var(\"%0\"))\n"},
        }
      },
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}");
}

TEST_CASE("parse: test_c99_to_tac.py::TestTranslateFunctionCall::test_call_via_full_pipeline (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c99_to_tac.py::TestTranslateFunctionCall::test_call_via_full_pipeline\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int foo(int a) { return a; } int main(void) { return foo(42); }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"foo\" [\"a\"] Block([\n"
      "    S(Return(Var(\"a\" None)))\n"
      "  ]) FunType([Int] Int) None None [\"\"] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(FunctionCall(\"foo\" [Constant(ConstInt(42) None)] None false)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_c99_to_tac.py::TestTranslateFunctionCall::test_call_via_full_pipeline (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_c99_to_tac.py::TestTranslateFunctionCall::test_call_via_full_pipeline\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"foo\" [\"a\"] Block([\n"
      "    S(Return(Var(\"a\" None)))\n"
      "  ]) FunType([Int] Int) None None [\"\"] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(FunctionCall(\"foo\" [Constant(ConstInt(42) None)] None false)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"foo\" [\"@0.a\"] Block([\n"
      "    S(Return(Var(\"@0.a\" None)))\n"
      "  ]) FunType([Int] Int) None None [\"\"] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(FunctionCall(\"foo\" [Constant(ConstInt(42) None)] None false)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_c99_to_tac.py::TestTranslateFunctionCall::test_call_via_full_pipeline (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c99_to_tac.py::TestTranslateFunctionCall::test_call_via_full_pipeline\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"foo\\\" [\\\"@0.a\\\"] Block([\\n    S(Return(Var(\\\"@0.a\\\" Int)))\\n  ]) FunType([Int] Int) None None [\\\"\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(FunctionCall(\\\"foo\\\" [Constant(ConstInt(42) Int)] Int false)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"foo\" [\"@0.a\"] Block([\n"
      "    S(Return(Var(\"@0.a\" None)))\n"
      "  ]) FunType([Int] Int) None None [\"\"] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(FunctionCall(\"foo\" [Constant(ConstInt(42) None)] None false)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"foo\\\" [\\\"@0.a\\\"] Block([\\n    S(Return(Var(\\\"@0.a\\\" Int)))\\n  ]) FunType([Int] Int) None None [\\\"\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(FunctionCall(\\\"foo\\\" [Constant(ConstInt(42) Int)] Int false)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"foo\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([Int] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("c99_to_tac: test_c99_to_tac.py::TestTranslateFunctionCall::test_call_via_full_pipeline (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass c99_to_tac\n"
      "source tests/test_c99_to_tac.py::TestTranslateFunctionCall::test_call_via_full_pipeline\n"
      "args {\"args\": [{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"foo\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([Int] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"args\": {\"0\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"foo\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([Int] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"foo\" [\"@0.a\"] Block([\n"
      "    S(Return(Var(\"@0.a\" Int)))\n"
      "  ]) FunType([Int] Int) None None [\"\"] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(FunctionCall(\"foo\" [Constant(ConstInt(42) Int)] Int false)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"foo\" true [\"@0.a\"] [\n"
      "    Ret(Var(\"@0.a\"))\n"
      "  ])\n"
      "  Function(\"main\" true [] [\n"
      "    FunctionCall(\"foo\" [Constant(ConstInt(42))] Var(\"%0\") false false)\n"
      "    Ret(Var(\"%0\"))\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("c99_session: test_c99_to_tac.py::TestTranslateFunctionCall::test_compound_arg_evaluated_first (call 1)") {
  c6502::testing::check_c99_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}",
      {
        {
          "translate_exp",
          "FunctionCall(\"f\" [Binary(Add Constant(ConstInt(1) None) Constant(ConstInt(2) None) None)] None false)\n",
          true,
          {},
          "Var(\"%1\")\n",
          {
            "Binary(Add Constant(ConstInt(1)) Constant(ConstInt(2)) Var(\"%0\"))\n",
            "FunctionCall(\"f\" [Var(\"%0\")] Var(\"%1\") false false)\n"},
        }
      },
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}");
}

TEST_CASE("c99_session: test_c99_to_tac.py::TestTranslateFunctionCall::test_constant_args_pass_through_as_constants (call 1)") {
  c6502::testing::check_c99_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}",
      {
        {
          "translate_exp",
          "FunctionCall(\"f\" [Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None false)\n",
          true,
          {},
          "Var(\"%0\")\n",
          {
            "FunctionCall(\"f\" [Constant(ConstInt(1)) Constant(ConstInt(2))] Var(\"%0\") false false)\n"},
        }
      },
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}");
}

TEST_CASE("c99_session: test_c99_to_tac.py::TestTranslateFunctionCall::test_no_args_emits_single_call (call 1)") {
  c6502::testing::check_c99_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}",
      {
        {
          "translate_exp",
          "FunctionCall(\"f\" [] None false)\n",
          true,
          {},
          "Var(\"%0\")\n",
          {
            "FunctionCall(\"f\" [] Var(\"%0\") false false)\n"},
        }
      },
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}");
}

TEST_CASE("parse: test_c99_to_tac.py::TestTranslateMultipleFunctions::test_each_function_gets_its_own_implicit_ret_zero (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c99_to_tac.py::TestTranslateMultipleFunctions::test_each_function_gets_its_own_implicit_ret_zero\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int foo(void) { } int main(void) { }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"foo\" [] Block([]) FunType([] Int) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_c99_to_tac.py::TestTranslateMultipleFunctions::test_each_function_gets_its_own_implicit_ret_zero (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_c99_to_tac.py::TestTranslateMultipleFunctions::test_each_function_gets_its_own_implicit_ret_zero\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"foo\" [] Block([]) FunType([] Int) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"foo\" [] Block([]) FunType([] Int) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_c99_to_tac.py::TestTranslateMultipleFunctions::test_each_function_gets_its_own_implicit_ret_zero (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c99_to_tac.py::TestTranslateMultipleFunctions::test_each_function_gets_its_own_implicit_ret_zero\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"foo\" [] Block([]) FunType([] Int) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"foo\\\" [] Block([]) FunType([] Int) None None [] false None [] None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"foo\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("c99_to_tac: test_c99_to_tac.py::TestTranslateMultipleFunctions::test_each_function_gets_its_own_implicit_ret_zero (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass c99_to_tac\n"
      "source tests/test_c99_to_tac.py::TestTranslateMultipleFunctions::test_each_function_gets_its_own_implicit_ret_zero\n"
      "args {\"args\": [{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"foo\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"foo\" [] Block([]) FunType([] Int) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"foo\" true [] [\n"
      "    Ret(Constant(ConstInt(0)))\n"
      "  ])\n"
      "  Function(\"main\" true [] [\n"
      "    Ret(Constant(ConstInt(0)))\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("parse: test_c99_to_tac.py::TestTranslateMultipleFunctions::test_function_params_pass_through_to_tac (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c99_to_tac.py::TestTranslateMultipleFunctions::test_function_params_pass_through_to_tac\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int foo(int a, int b) { return a + b; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"foo\" [\"a\" \"b\"] Block([\n"
      "    S(Return(Binary(Add Var(\"a\" None) Var(\"b\" None) None)))\n"
      "  ]) FunType([Int Int] Int) None None [\"\" \"\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_c99_to_tac.py::TestTranslateMultipleFunctions::test_function_params_pass_through_to_tac (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_c99_to_tac.py::TestTranslateMultipleFunctions::test_function_params_pass_through_to_tac\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"foo\" [\"a\" \"b\"] Block([\n"
      "    S(Return(Binary(Add Var(\"a\" None) Var(\"b\" None) None)))\n"
      "  ]) FunType([Int Int] Int) None None [\"\" \"\"] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"foo\" [\"@0.a\" \"@1.b\"] Block([\n"
      "    S(Return(Binary(Add Var(\"@0.a\" None) Var(\"@1.b\" None) None)))\n"
      "  ]) FunType([Int Int] Int) None None [\"\" \"\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_c99_to_tac.py::TestTranslateMultipleFunctions::test_function_params_pass_through_to_tac (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c99_to_tac.py::TestTranslateMultipleFunctions::test_function_params_pass_through_to_tac\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"foo\\\" [\\\"@0.a\\\" \\\"@1.b\\\"] Block([\\n    S(Return(Binary(Add Var(\\\"@0.a\\\" Int) Var(\\\"@1.b\\\" Int) Int)))\\n  ]) FunType([Int Int] Int) None None [\\\"\\\" \\\"\\\"] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"foo\" [\"@0.a\" \"@1.b\"] Block([\n"
      "    S(Return(Binary(Add Var(\"@0.a\" None) Var(\"@1.b\" None) None)))\n"
      "  ]) FunType([Int Int] Int) None None [\"\" \"\"] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"foo\\\" [\\\"@0.a\\\" \\\"@1.b\\\"] Block([\\n    S(Return(Binary(Add Var(\\\"@0.a\\\" Int) Var(\\\"@1.b\\\" Int) Int)))\\n  ]) FunType([Int Int] Int) None None [\\\"\\\" \\\"\\\"] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"foo\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([Int Int] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("c99_to_tac: test_c99_to_tac.py::TestTranslateMultipleFunctions::test_function_params_pass_through_to_tac (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass c99_to_tac\n"
      "source tests/test_c99_to_tac.py::TestTranslateMultipleFunctions::test_function_params_pass_through_to_tac\n"
      "args {\"args\": [{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"foo\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([Int Int] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"args\": {\"0\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"foo\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([Int Int] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"foo\" [\"@0.a\" \"@1.b\"] Block([\n"
      "    S(Return(Binary(Add Var(\"@0.a\" Int) Var(\"@1.b\" Int) Int)))\n"
      "  ]) FunType([Int Int] Int) None None [\"\" \"\"] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"foo\" true [\"@0.a\" \"@1.b\"] [\n"
      "    Binary(Add Var(\"@0.a\") Var(\"@1.b\") Var(\"%0\"))\n"
      "    Ret(Var(\"%0\"))\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("parse: test_c99_to_tac.py::TestTranslateMultipleFunctions::test_two_functions_appear_in_source_order (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c99_to_tac.py::TestTranslateMultipleFunctions::test_two_functions_appear_in_source_order\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int foo(void) { return 1; } int bar(void) { return 2; } int main(void) { return 0; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"foo\" [] Block([\n"
      "    S(Return(Constant(ConstInt(1) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"bar\" [] Block([\n"
      "    S(Return(Constant(ConstInt(2) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_c99_to_tac.py::TestTranslateMultipleFunctions::test_two_functions_appear_in_source_order (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_c99_to_tac.py::TestTranslateMultipleFunctions::test_two_functions_appear_in_source_order\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"foo\" [] Block([\n"
      "    S(Return(Constant(ConstInt(1) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"bar\" [] Block([\n"
      "    S(Return(Constant(ConstInt(2) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"foo\" [] Block([\n"
      "    S(Return(Constant(ConstInt(1) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"bar\" [] Block([\n"
      "    S(Return(Constant(ConstInt(2) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_c99_to_tac.py::TestTranslateMultipleFunctions::test_two_functions_appear_in_source_order (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c99_to_tac.py::TestTranslateMultipleFunctions::test_two_functions_appear_in_source_order\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"foo\\\" [] Block([\\n    S(Return(Constant(ConstInt(1) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n  FunctionDecl(function_decl(\\\"bar\\\" [] Block([\\n    S(Return(Constant(ConstInt(2) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"foo\" [] Block([\n"
      "    S(Return(Constant(ConstInt(1) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"bar\" [] Block([\n"
      "    S(Return(Constant(ConstInt(2) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"foo\\\" [] Block([\\n    S(Return(Constant(ConstInt(1) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n  FunctionDecl(function_decl(\\\"bar\\\" [] Block([\\n    S(Return(Constant(ConstInt(2) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"foo\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"bar\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("c99_to_tac: test_c99_to_tac.py::TestTranslateMultipleFunctions::test_two_functions_appear_in_source_order (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass c99_to_tac\n"
      "source tests/test_c99_to_tac.py::TestTranslateMultipleFunctions::test_two_functions_appear_in_source_order\n"
      "args {\"args\": [{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"foo\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"bar\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"foo\" [] Block([\n"
      "    S(Return(Constant(ConstInt(1) Int)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"bar\" [] Block([\n"
      "    S(Return(Constant(ConstInt(2) Int)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) Int)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"foo\" true [] [\n"
      "    Ret(Constant(ConstInt(1)))\n"
      "  ])\n"
      "  Function(\"bar\" true [] [\n"
      "    Ret(Constant(ConstInt(2)))\n"
      "  ])\n"
      "  Function(\"main\" true [] [\n"
      "    Ret(Constant(ConstInt(0)))\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("c99_session: test_c99_to_tac.py::TestMakeTemporaryVariableName::test_sequential_counter (call 1)") {
  c6502::testing::check_c99_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}",
      {
        {
          "make_temporary_variable_name",
          "",
          false,
          {},
          "%0",
          {},
        },
        {
          "make_temporary_variable_name",
          "",
          false,
          {},
          "%1",
          {},
        },
        {
          "make_temporary_variable_name",
          "",
          false,
          {},
          "%2",
          {},
        }
      },
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}");
}

TEST_CASE("c99_session: test_c99_to_tac.py::TestMakeTemporaryVariableName::test_temp_registers_in_symbol_table_with_passed_type (call 1)") {
  c6502::testing::check_c99_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}",
      {
        {
          "make_temporary_variable_name",
          "Int\n",
          false,
          {},
          "%0",
          {},
        },
        {
          "make_temporary_variable_name",
          "Long\n",
          false,
          {},
          "%1",
          {},
        }
      },
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}");
}

TEST_CASE("c99_session: test_c99_to_tac.py::TestMakeTemporaryVariableName::test_temp_without_type_defaults_to_int (call 1)") {
  c6502::testing::check_c99_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}",
      {
        {
          "make_temporary_variable_name",
          "",
          false,
          {},
          "%0",
          {},
        }
      },
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}");
}

TEST_CASE("type_checking: test_c99_to_tac.py::TestCastAndStaticVariableTypes::test_address_init_does_not_merge_with_zeros (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c99_to_tac.py::TestCastAndStaticVariableTypes::test_address_init_does_not_merge_with_zeros\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"x\\\" None Int None None None None false))\\n  VarDecl(var_decl(\\\"p\\\" AddressOf(Var(\\\"x\\\" Int) Pointer(Int)) Pointer(Int) None None None None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" None Int None None None None false))\n"
      "  VarDecl(var_decl(\"p\" AddressOf(Var(\"x\" None) None) Pointer(Int) None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"x\\\" None Int None None None None false))\\n  VarDecl(var_decl(\\\"p\\\" AddressOf(Var(\\\"x\\\" Int) Pointer(Int)) Pointer(Int) None None None None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": {\"object\": \"passes.type_checking.AddressInit\", \"fields\": {\"name\": \"x\", \"offset\": 0}}}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("c99_to_tac: test_c99_to_tac.py::TestCastAndStaticVariableTypes::test_address_init_does_not_merge_with_zeros (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass c99_to_tac\n"
      "source tests/test_c99_to_tac.py::TestCastAndStaticVariableTypes::test_address_init_does_not_merge_with_zeros\n"
      "args {\"args\": [{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": {\"object\": \"passes.type_checking.AddressInit\", \"fields\": {\"name\": \"x\", \"offset\": 0}}}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" None Int None None None None false))\n"
      "  VarDecl(var_decl(\"p\" AddressOf(Var(\"x\" Int) Pointer(Int)) Pointer(Int) None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) Int)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Ret(Constant(ConstInt(0)))\n"
      "  ])\n"
      "  StaticVariable(\"x\" true Int [ZeroInit(2)])\n"
      "  StaticVariable(\"p\" true Pointer [AddressInit(\"x\" 0)])\n"
      "])\n");
}

TEST_CASE("type_checking: test_c99_to_tac.py::TestCastAndStaticVariableTypes::test_array_partial_init_coalesces_trailing_zeros (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c99_to_tac.py::TestCastAndStaticVariableTypes::test_array_partial_init_coalesces_trailing_zeros\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" InitList([Constant(ConstInt(1) Int)] Array(Int 5) None) Array(Int 5) Static None None None false)))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" InitList([Constant(ConstInt(1) None)] None None) Array(Int 5) Static None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" InitList([Constant(ConstInt(1) Int)] Array(Int 5) None) Array(Int 5) Static None None None false)))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Int 5)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": {\"tuple\": [1, 0, 0, 0, 0]}}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("c99_to_tac: test_c99_to_tac.py::TestCastAndStaticVariableTypes::test_array_partial_init_coalesces_trailing_zeros (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass c99_to_tac\n"
      "source tests/test_c99_to_tac.py::TestCastAndStaticVariableTypes::test_array_partial_init_coalesces_trailing_zeros\n"
      "args {\"args\": [{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Int 5)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": {\"tuple\": [1, 0, 0, 0, 0]}}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" InitList([Constant(ConstInt(1) Int)] Array(Int 5) None) Array(Int 5) Static None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) Int)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Ret(Constant(ConstInt(0)))\n"
      "  ])\n"
      "  StaticVariable(\"@0.a\" false Pointer [IntInit(1) ZeroInit(8)])\n"
      "])\n");
}

TEST_CASE("type_checking: test_c99_to_tac.py::TestCastAndStaticVariableTypes::test_block_scope_char_array_string_init_emits_per_byte_stores (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c99_to_tac.py::TestCastAndStaticVariableTypes::test_block_scope_char_array_string_init_emits_per_byte_stores\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.arr\\\" String(\\\"Hi\\\") Array(Char 5) None None None None false)))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.arr\" String(\"Hi\") Array(Char 5) None None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.arr\\\" String(\\\"Hi\\\") Array(Char 5) None None None None false)))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.arr\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Char 5)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("c99_to_tac: test_c99_to_tac.py::TestCastAndStaticVariableTypes::test_block_scope_char_array_string_init_emits_per_byte_stores (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass c99_to_tac\n"
      "source tests/test_c99_to_tac.py::TestCastAndStaticVariableTypes::test_block_scope_char_array_string_init_emits_per_byte_stores\n"
      "args {\"args\": [{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.arr\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Char 5)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"args\": {\"0\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.arr\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Char 5)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Char)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UInt)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%3\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.arr\" String(\"Hi\") Array(Char 5) None None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) Int)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    GetAddress(Var(\"@0.arr\") Var(\"%0\"))\n"
      "    Store(Constant(ConstUChar(72)) Var(\"%0\") false)\n"
      "    Binary(Add Var(\"%0\") Constant(ConstInt(1)) Var(\"%1\"))\n"
      "    Store(Constant(ConstUChar(105)) Var(\"%1\") false)\n"
      "    Binary(Add Var(\"%0\") Constant(ConstInt(2)) Var(\"%2\"))\n"
      "    Store(Constant(ConstUInt(0)) Var(\"%2\") false)\n"
      "    Binary(Add Var(\"%0\") Constant(ConstInt(4)) Var(\"%3\"))\n"
      "    Store(Constant(ConstUChar(0)) Var(\"%3\") false)\n"
      "    Ret(Constant(ConstInt(0)))\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("type_checking: test_c99_to_tac.py::TestCastAndStaticVariableTypes::test_constant_lowers_with_const_variant_preserved (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c99_to_tac.py::TestCastAndStaticVariableTypes::test_constant_lowers_with_const_variant_preserved\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(Constant(ConstInt(5) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(5) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(Constant(ConstInt(5) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("c99_to_tac: test_c99_to_tac.py::TestCastAndStaticVariableTypes::test_constant_lowers_with_const_variant_preserved (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass c99_to_tac\n"
      "source tests/test_c99_to_tac.py::TestCastAndStaticVariableTypes::test_constant_lowers_with_const_variant_preserved\n"
      "args {\"args\": [{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(5) Int)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Ret(Constant(ConstInt(5)))\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("type_checking: test_c99_to_tac.py::TestCastAndStaticVariableTypes::test_file_scope_char_array_string_init_emits_static_with_bytes (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c99_to_tac.py::TestCastAndStaticVariableTypes::test_file_scope_char_array_string_init_emits_static_with_bytes\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"arr\" String(\"Hi\") Array(Char 5) None None None None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"arr\\\" String(\\\"Hi\\\") Array(Char 5) None None None None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"arr\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Char 5)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": {\"tuple\": [72, 105, 0, 0, 0]}}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("c99_to_tac: test_c99_to_tac.py::TestCastAndStaticVariableTypes::test_file_scope_char_array_string_init_emits_static_with_bytes (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass c99_to_tac\n"
      "source tests/test_c99_to_tac.py::TestCastAndStaticVariableTypes::test_file_scope_char_array_string_init_emits_static_with_bytes\n"
      "args {\"args\": [{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"arr\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Char 5)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": {\"tuple\": [72, 105, 0, 0, 0]}}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"arr\" String(\"Hi\") Array(Char 5) None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  StaticVariable(\"arr\" true Pointer [StringInit(\"Hi\\x00\\x00\\x00\" 5)])\n"
      "])\n");
}

TEST_CASE("type_checking: test_c99_to_tac.py::TestCastAndStaticVariableTypes::test_identity_cast_emits_no_conversion (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c99_to_tac.py::TestCastAndStaticVariableTypes::test_identity_cast_emits_no_conversion\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.x\\\" Constant(ConstInt(5) Int) Int None None None None false)))\\n    S(Return(Cast(Int Var(\\\"@0.x\\\" Int) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.x\" Constant(ConstInt(5) None) Int None None None None false)))\n"
      "    S(Return(Cast(Int Var(\"@0.x\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.x\\\" Constant(ConstInt(5) Int) Int None None None None false)))\\n    S(Return(Cast(Int Var(\\\"@0.x\\\" Int) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("c99_to_tac: test_c99_to_tac.py::TestCastAndStaticVariableTypes::test_identity_cast_emits_no_conversion (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass c99_to_tac\n"
      "source tests/test_c99_to_tac.py::TestCastAndStaticVariableTypes::test_identity_cast_emits_no_conversion\n"
      "args {\"args\": [{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.x\" Constant(ConstInt(5) Int) Int None None None None false)))\n"
      "    S(Return(Cast(Int Var(\"@0.x\" Int) Int)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Copy(Constant(ConstInt(5)) Var(\"@0.x\"))\n"
      "    Ret(Var(\"@0.x\"))\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("type_checking: test_c99_to_tac.py::TestCastAndStaticVariableTypes::test_int_to_long_cast_emits_sign_extend (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c99_to_tac.py::TestCastAndStaticVariableTypes::test_int_to_long_cast_emits_sign_extend\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.x\\\" Constant(ConstInt(5) Int) Int None None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.y\\\" Cast(Long Var(\\\"@0.x\\\" Int) Long) Long None None None None false)))\\n    S(Return(Cast(Int Var(\\\"@1.y\\\" Long) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.x\" Constant(ConstInt(5) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.y\" Cast(Long Var(\"@0.x\" None) None) Long None None None None false)))\n"
      "    S(Return(Cast(Int Var(\"@1.y\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.x\\\" Constant(ConstInt(5) Int) Int None None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.y\\\" Cast(Long Var(\\\"@0.x\\\" Int) Long) Long None None None None false)))\\n    S(Return(Cast(Int Var(\\\"@1.y\\\" Long) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.y\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("c99_to_tac: test_c99_to_tac.py::TestCastAndStaticVariableTypes::test_int_to_long_cast_emits_sign_extend (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass c99_to_tac\n"
      "source tests/test_c99_to_tac.py::TestCastAndStaticVariableTypes::test_int_to_long_cast_emits_sign_extend\n"
      "args {\"args\": [{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.y\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"args\": {\"0\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.y\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.x\" Constant(ConstInt(5) Int) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.y\" Cast(Long Var(\"@0.x\" Int) Long) Long None None None None false)))\n"
      "    S(Return(Cast(Int Var(\"@1.y\" Long) Int)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Copy(Constant(ConstInt(5)) Var(\"@0.x\"))\n"
      "    SignExtend(Var(\"@0.x\") Var(\"%0\"))\n"
      "    Copy(Var(\"%0\") Var(\"@1.y\"))\n"
      "    Truncate(Var(\"@1.y\") Var(\"%1\"))\n"
      "    Ret(Var(\"%1\"))\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("type_checking: test_c99_to_tac.py::TestCastAndStaticVariableTypes::test_int_to_long_long_cast_emits_sign_extend (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c99_to_tac.py::TestCastAndStaticVariableTypes::test_int_to_long_long_cast_emits_sign_extend\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.x\\\" Constant(ConstInt(5) Int) Int None None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.y\\\" Cast(LongLong Var(\\\"@0.x\\\" Int) LongLong) LongLong None None None None false)))\\n    S(Return(Cast(Int Var(\\\"@1.y\\\" LongLong) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.x\" Constant(ConstInt(5) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.y\" Cast(LongLong Var(\"@0.x\" None) None) LongLong None None None None false)))\n"
      "    S(Return(Cast(Int Var(\"@1.y\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.x\\\" Constant(ConstInt(5) Int) Int None None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.y\\\" Cast(LongLong Var(\\\"@0.x\\\" Int) LongLong) LongLong None None None None false)))\\n    S(Return(Cast(Int Var(\\\"@1.y\\\" LongLong) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.y\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"LongLong\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("c99_to_tac: test_c99_to_tac.py::TestCastAndStaticVariableTypes::test_int_to_long_long_cast_emits_sign_extend (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass c99_to_tac\n"
      "source tests/test_c99_to_tac.py::TestCastAndStaticVariableTypes::test_int_to_long_long_cast_emits_sign_extend\n"
      "args {\"args\": [{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.y\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"LongLong\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"args\": {\"0\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.y\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"LongLong\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"LongLong\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.x\" Constant(ConstInt(5) Int) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.y\" Cast(LongLong Var(\"@0.x\" Int) LongLong) LongLong None None None None false)))\n"
      "    S(Return(Cast(Int Var(\"@1.y\" LongLong) Int)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Copy(Constant(ConstInt(5)) Var(\"@0.x\"))\n"
      "    SignExtend(Var(\"@0.x\") Var(\"%0\"))\n"
      "    Copy(Var(\"%0\") Var(\"@1.y\"))\n"
      "    Truncate(Var(\"@1.y\") Var(\"%1\"))\n"
      "    Ret(Var(\"%1\"))\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("type_checking: test_c99_to_tac.py::TestCastAndStaticVariableTypes::test_long_long_static_carries_long_long_init (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c99_to_tac.py::TestCastAndStaticVariableTypes::test_long_long_static_carries_long_long_init\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"g\\\" Constant(ConstLongLong(1234567890) LongLong) LongLong None None None None false))\\n  VarDecl(var_decl(\\\"u\\\" Constant(ConstULongLong(4000000000) ULongLong) ULongLong None None None None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"g\" Constant(ConstLongLong(1234567890) None) LongLong None None None None false))\n"
      "  VarDecl(var_decl(\"u\" Constant(ConstULongLong(4000000000) None) ULongLong None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"g\\\" Constant(ConstLongLong(1234567890) LongLong) LongLong None None None None false))\\n  VarDecl(var_decl(\\\"u\\\" Constant(ConstULongLong(4000000000) ULongLong) ULongLong None None None None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"g\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"LongLong\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 1234567890}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"u\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"ULongLong\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 4000000000}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("c99_to_tac: test_c99_to_tac.py::TestCastAndStaticVariableTypes::test_long_long_static_carries_long_long_init (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass c99_to_tac\n"
      "source tests/test_c99_to_tac.py::TestCastAndStaticVariableTypes::test_long_long_static_carries_long_long_init\n"
      "args {\"args\": [{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"g\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"LongLong\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 1234567890}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"u\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"ULongLong\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 4000000000}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"g\" Constant(ConstLongLong(1234567890) LongLong) LongLong None None None None false))\n"
      "  VarDecl(var_decl(\"u\" Constant(ConstULongLong(4000000000) ULongLong) ULongLong None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) Int)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Ret(Constant(ConstInt(0)))\n"
      "  ])\n"
      "  StaticVariable(\"g\" true LongLong [LongLongInit(1234567890)])\n"
      "  StaticVariable(\"u\" true ULongLong [ULongLongInit(4000000000)])\n"
      "])\n");
}

TEST_CASE("type_checking: test_c99_to_tac.py::TestCastAndStaticVariableTypes::test_long_returning_function_implicit_zero_uses_const_long (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c99_to_tac.py::TestCastAndStaticVariableTypes::test_long_returning_function_implicit_zero_uses_const_long\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([]) FunType([] Long) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([]) FunType([] Long) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Long)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("c99_to_tac: test_c99_to_tac.py::TestCastAndStaticVariableTypes::test_long_returning_function_implicit_zero_uses_const_long (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass c99_to_tac\n"
      "source tests/test_c99_to_tac.py::TestCastAndStaticVariableTypes::test_long_returning_function_implicit_zero_uses_const_long\n"
      "args {\"args\": [{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Long)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([]) FunType([] Long) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Ret(Constant(ConstLong(0)))\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("type_checking: test_c99_to_tac.py::TestCastAndStaticVariableTypes::test_multi_dim_array_init_with_zero_holes (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c99_to_tac.py::TestCastAndStaticVariableTypes::test_multi_dim_array_init_with_zero_holes\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" InitList([InitList([Cast(Long Constant(ConstInt(100) Int) Long)] Array(Long 2) None) InitList([Cast(Long Constant(ConstInt(200) Int) Long) Cast(Long Constant(ConstInt(300) Int) Long)] Array(Long 2) None)] Array(Array(Long 2) 3) None) Array(Array(Long 2) 3) Static None None None false)))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" InitList([InitList([Constant(ConstInt(100) None)] None None) InitList([Constant(ConstInt(200) None) Constant(ConstInt(300) None)] None None)] None None) Array(Array(Long 2) 3) Static None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" InitList([InitList([Cast(Long Constant(ConstInt(100) Int) Long)] Array(Long 2) None) InitList([Cast(Long Constant(ConstInt(200) Int) Long) Cast(Long Constant(ConstInt(300) Int) Long)] Array(Long 2) None)] Array(Array(Long 2) 3) None) Array(Array(Long 2) 3) Static None None None false)))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Array(Long 2) 3)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": {\"tuple\": [{\"tuple\": [100, 0]}, {\"tuple\": [200, 300]}, {\"tuple\": [0, 0]}]}}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("c99_to_tac: test_c99_to_tac.py::TestCastAndStaticVariableTypes::test_multi_dim_array_init_with_zero_holes (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass c99_to_tac\n"
      "source tests/test_c99_to_tac.py::TestCastAndStaticVariableTypes::test_multi_dim_array_init_with_zero_holes\n"
      "args {\"args\": [{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Array(Long 2) 3)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": {\"tuple\": [{\"tuple\": [100, 0]}, {\"tuple\": [200, 300]}, {\"tuple\": [0, 0]}]}}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" InitList([InitList([Cast(Long Constant(ConstInt(100) Int) Long)] Array(Long 2) None) InitList([Cast(Long Constant(ConstInt(200) Int) Long) Cast(Long Constant(ConstInt(300) Int) Long)] Array(Long 2) None)] Array(Array(Long 2) 3) None) Array(Array(Long 2) 3) Static None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) Int)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Ret(Constant(ConstInt(0)))\n"
      "  ])\n"
      "  StaticVariable(\"@0.a\" false Pointer [LongInit(100) ZeroInit(4) LongInit(200) LongInit(300) ZeroInit(8)])\n"
      "])\n");
}

TEST_CASE("type_checking: test_c99_to_tac.py::TestCastAndStaticVariableTypes::test_out_of_range_fp_cast_folds_into_the_type (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c99_to_tac.py::TestCastAndStaticVariableTypes::test_out_of_range_fp_cast_folds_into_the_type\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    S(Return(Cast(ULong Constant(ConstDouble(9129546652158286818) Double) ULong)))\\n  ]) FunType([] ULong) None None [] false None [] None false))\\n  FunctionDecl(function_decl(\\\"g\\\" [] Block([\\n    S(Return(Cast(Int Constant(ConstDouble(4746794007250599936) Double) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n  FunctionDecl(function_decl(\\\"h\\\" [] Block([\\n    S(Return(Cast(Int Constant(ConstDouble(4674736413210574848) Double) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Cast(ULong Constant(ConstDouble(9129546652158286818) None) None)))\n"
      "  ]) FunType([] ULong) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"g\" [] Block([\n"
      "    S(Return(Cast(Int Constant(ConstDouble(4746794007250599936) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"h\" [] Block([\n"
      "    S(Return(Cast(Int Constant(ConstDouble(4674736413210574848) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    S(Return(Cast(ULong Constant(ConstDouble(9129546652158286818) Double) ULong)))\\n  ]) FunType([] ULong) None None [] false None [] None false))\\n  FunctionDecl(function_decl(\\\"g\\\" [] Block([\\n    S(Return(Cast(Int Constant(ConstDouble(4746794007250599936) Double) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n  FunctionDecl(function_decl(\\\"h\\\" [] Block([\\n    S(Return(Cast(Int Constant(ConstDouble(4674736413210574848) Double) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] ULong)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"g\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"h\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("c99_to_tac: test_c99_to_tac.py::TestCastAndStaticVariableTypes::test_out_of_range_fp_cast_folds_into_the_type (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass c99_to_tac\n"
      "source tests/test_c99_to_tac.py::TestCastAndStaticVariableTypes::test_out_of_range_fp_cast_folds_into_the_type\n"
      "args {\"args\": [{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] ULong)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"g\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"h\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"args\": {\"0\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] ULong)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"g\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"h\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"ULong\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Cast(ULong Constant(ConstDouble(9129546652158286818) Double) ULong)))\n"
      "  ]) FunType([] ULong) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"g\" [] Block([\n"
      "    S(Return(Cast(Int Constant(ConstDouble(4746794007250599936) Double) Int)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"h\" [] Block([\n"
      "    S(Return(Cast(Int Constant(ConstDouble(4674736413210574848) Double) Int)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Ret(Constant(ConstULong(0)))\n"
      "  ])\n"
      "  Function(\"g\" true [] [\n"
      "    Ret(Constant(ConstInt(1)))\n"
      "  ])\n"
      "  Function(\"h\" true [] [\n"
      "    Ret(Constant(ConstInt(-32768)))\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("type_checking: test_c99_to_tac.py::TestCastAndStaticVariableTypes::test_static_double_zero_uses_zero_init (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c99_to_tac.py::TestCastAndStaticVariableTypes::test_static_double_zero_uses_zero_init\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"x\\\" Cast(Double Constant(ConstInt(0) Int) Double) Double Static None None None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" Constant(ConstInt(0) None) Double Static None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"x\\\" Cast(Double Constant(ConstInt(0) Int) Double) Double Static None None None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Double\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 0}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("c99_to_tac: test_c99_to_tac.py::TestCastAndStaticVariableTypes::test_static_double_zero_uses_zero_init (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass c99_to_tac\n"
      "source tests/test_c99_to_tac.py::TestCastAndStaticVariableTypes::test_static_double_zero_uses_zero_init\n"
      "args {\"args\": [{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Double\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 0}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" Cast(Double Constant(ConstInt(0) Int) Double) Double Static None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) Int)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Ret(Constant(ConstInt(0)))\n"
      "  ])\n"
      "  StaticVariable(\"x\" false Double [ZeroInit(8)])\n"
      "])\n");
}

TEST_CASE("type_checking: test_c99_to_tac.py::TestCastAndStaticVariableTypes::test_static_double_zero_uses_zero_init (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c99_to_tac.py::TestCastAndStaticVariableTypes::test_static_double_zero_uses_zero_init\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"x\\\" Constant(ConstDouble(0) Double) Double Static None None None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" Constant(ConstDouble(0) None) Double Static None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"x\\\" Constant(ConstDouble(0) Double) Double Static None None None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Double\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 0}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("c99_to_tac: test_c99_to_tac.py::TestCastAndStaticVariableTypes::test_static_double_zero_uses_zero_init (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass c99_to_tac\n"
      "source tests/test_c99_to_tac.py::TestCastAndStaticVariableTypes::test_static_double_zero_uses_zero_init\n"
      "args {\"args\": [{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Double\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 0}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" Constant(ConstDouble(0) Double) Double Static None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) Int)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Ret(Constant(ConstInt(0)))\n"
      "  ])\n"
      "  StaticVariable(\"x\" false Double [ZeroInit(8)])\n"
      "])\n");
}

TEST_CASE("type_checking: test_c99_to_tac.py::TestCastAndStaticVariableTypes::test_static_float_array_zero_init (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c99_to_tac.py::TestCastAndStaticVariableTypes::test_static_float_array_zero_init\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" InitList([Constant(ConstFloat(1069547520) Float)] Array(Float 3) None) Array(Float 3) Static None None None false)))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" InitList([Constant(ConstFloat(1069547520) None)] None None) Array(Float 3) Static None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" InitList([Constant(ConstFloat(1069547520) Float)] Array(Float 3) None) Array(Float 3) Static None None None false)))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Float 3)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": {\"tuple\": [1069547520, 0, 0]}}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("c99_to_tac: test_c99_to_tac.py::TestCastAndStaticVariableTypes::test_static_float_array_zero_init (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass c99_to_tac\n"
      "source tests/test_c99_to_tac.py::TestCastAndStaticVariableTypes::test_static_float_array_zero_init\n"
      "args {\"args\": [{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Float 3)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": {\"tuple\": [1069547520, 0, 0]}}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" InitList([Constant(ConstFloat(1069547520) Float)] Array(Float 3) None) Array(Float 3) Static None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) Int)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Ret(Constant(ConstInt(0)))\n"
      "  ])\n"
      "  StaticVariable(\"@0.a\" false Pointer [FloatInit(1069547520) ZeroInit(8)])\n"
      "])\n");
}

TEST_CASE("type_checking: test_c99_to_tac.py::TestCastAndStaticVariableTypes::test_static_float_zero_uses_zero_init (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c99_to_tac.py::TestCastAndStaticVariableTypes::test_static_float_zero_uses_zero_init\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"x\\\" Cast(Float Constant(ConstInt(0) Int) Float) Float Static None None None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" Constant(ConstInt(0) None) Float Static None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"x\\\" Cast(Float Constant(ConstInt(0) Int) Float) Float Static None None None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Float\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 0}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("c99_to_tac: test_c99_to_tac.py::TestCastAndStaticVariableTypes::test_static_float_zero_uses_zero_init (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass c99_to_tac\n"
      "source tests/test_c99_to_tac.py::TestCastAndStaticVariableTypes::test_static_float_zero_uses_zero_init\n"
      "args {\"args\": [{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Float\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 0}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" Cast(Float Constant(ConstInt(0) Int) Float) Float Static None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) Int)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Ret(Constant(ConstInt(0)))\n"
      "  ])\n"
      "  StaticVariable(\"x\" false Float [ZeroInit(4)])\n"
      "])\n");
}

TEST_CASE("type_checking: test_c99_to_tac.py::TestCastAndStaticVariableTypes::test_static_float_zero_uses_zero_init (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c99_to_tac.py::TestCastAndStaticVariableTypes::test_static_float_zero_uses_zero_init\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"x\\\" Cast(Float Constant(ConstDouble(0) Double) Float) Float Static None None None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" Constant(ConstDouble(0) None) Float Static None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"x\\\" Cast(Float Constant(ConstDouble(0) Double) Float) Float Static None None None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Float\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 0}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("c99_to_tac: test_c99_to_tac.py::TestCastAndStaticVariableTypes::test_static_float_zero_uses_zero_init (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass c99_to_tac\n"
      "source tests/test_c99_to_tac.py::TestCastAndStaticVariableTypes::test_static_float_zero_uses_zero_init\n"
      "args {\"args\": [{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Float\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 0}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" Cast(Float Constant(ConstDouble(0) Double) Float) Float Static None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) Int)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Ret(Constant(ConstInt(0)))\n"
      "  ])\n"
      "  StaticVariable(\"x\" false Float [ZeroInit(4)])\n"
      "])\n");
}

TEST_CASE("type_checking: test_c99_to_tac.py::TestCastAndStaticVariableTypes::test_static_variable_carries_data_type_and_typed_init (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c99_to_tac.py::TestCastAndStaticVariableTypes::test_static_variable_carries_data_type_and_typed_init\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"g_long\\\" Cast(Long Constant(ConstInt(7) Int) Long) Long None None None None false))\\n  VarDecl(var_decl(\\\"g_int\\\" Constant(ConstInt(3) Int) Int None None None None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(Var(\\\"g_int\\\" Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"g_long\" Constant(ConstInt(7) None) Long None None None None false))\n"
      "  VarDecl(var_decl(\"g_int\" Constant(ConstInt(3) None) Int None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Var(\"g_int\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"g_long\\\" Cast(Long Constant(ConstInt(7) Int) Long) Long None None None None false))\\n  VarDecl(var_decl(\\\"g_int\\\" Constant(ConstInt(3) Int) Int None None None None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(Var(\\\"g_int\\\" Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"g_long\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 7}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"g_int\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 3}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("c99_to_tac: test_c99_to_tac.py::TestCastAndStaticVariableTypes::test_static_variable_carries_data_type_and_typed_init (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass c99_to_tac\n"
      "source tests/test_c99_to_tac.py::TestCastAndStaticVariableTypes::test_static_variable_carries_data_type_and_typed_init\n"
      "args {\"args\": [{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"g_long\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 7}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"g_int\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 3}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"g_long\" Cast(Long Constant(ConstInt(7) Int) Long) Long None None None None false))\n"
      "  VarDecl(var_decl(\"g_int\" Constant(ConstInt(3) Int) Int None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Var(\"g_int\" Int)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Ret(Var(\"g_int\"))\n"
      "  ])\n"
      "  StaticVariable(\"g_long\" true Long [LongInit(7)])\n"
      "  StaticVariable(\"g_int\" true Int [IntInit(3)])\n"
      "])\n");
}

TEST_CASE("type_checking: test_c99_to_tac.py::TestCastAndStaticVariableTypes::test_tentative_double_resolves_to_zero_init (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c99_to_tac.py::TestCastAndStaticVariableTypes::test_tentative_double_resolves_to_zero_init\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"x\\\" None Double None None None None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" None Double None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"x\\\" None Double None None None None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Double\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("c99_to_tac: test_c99_to_tac.py::TestCastAndStaticVariableTypes::test_tentative_double_resolves_to_zero_init (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass c99_to_tac\n"
      "source tests/test_c99_to_tac.py::TestCastAndStaticVariableTypes::test_tentative_double_resolves_to_zero_init\n"
      "args {\"args\": [{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Double\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" None Double None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) Int)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Ret(Constant(ConstInt(0)))\n"
      "  ])\n"
      "  StaticVariable(\"x\" true Double [ZeroInit(8)])\n"
      "])\n");
}

TEST_CASE("type_checking: test_c99_to_tac.py::TestCastAndStaticVariableTypes::test_tentative_float_resolves_to_zero_init (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c99_to_tac.py::TestCastAndStaticVariableTypes::test_tentative_float_resolves_to_zero_init\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"x\\\" None Float None None None None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" None Float None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"x\\\" None Float None None None None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Float\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("c99_to_tac: test_c99_to_tac.py::TestCastAndStaticVariableTypes::test_tentative_float_resolves_to_zero_init (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass c99_to_tac\n"
      "source tests/test_c99_to_tac.py::TestCastAndStaticVariableTypes::test_tentative_float_resolves_to_zero_init\n"
      "args {\"args\": [{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Float\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" None Float None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) Int)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Ret(Constant(ConstInt(0)))\n"
      "  ])\n"
      "  StaticVariable(\"x\" true Float [ZeroInit(4)])\n"
      "])\n");
}

TEST_CASE("type_checking: test_c99_to_tac.py::TestCastAndStaticVariableTypes::test_tentative_int_resolves_to_zero_init (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c99_to_tac.py::TestCastAndStaticVariableTypes::test_tentative_int_resolves_to_zero_init\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"x\\\" None Int None None None None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" None Int None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"x\\\" None Int None None None None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("c99_to_tac: test_c99_to_tac.py::TestCastAndStaticVariableTypes::test_tentative_int_resolves_to_zero_init (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass c99_to_tac\n"
      "source tests/test_c99_to_tac.py::TestCastAndStaticVariableTypes::test_tentative_int_resolves_to_zero_init\n"
      "args {\"args\": [{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" None Int None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) Int)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Ret(Constant(ConstInt(0)))\n"
      "  ])\n"
      "  StaticVariable(\"x\" true Int [ZeroInit(2)])\n"
      "])\n");
}

TEST_CASE("type_checking: test_c99_to_tac.py::TestCastAndStaticVariableTypes::test_tentative_long_resolves_to_zero_init (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c99_to_tac.py::TestCastAndStaticVariableTypes::test_tentative_long_resolves_to_zero_init\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"x\\\" None Long None None None None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" None Long None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"x\\\" None Long None None None None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("c99_to_tac: test_c99_to_tac.py::TestCastAndStaticVariableTypes::test_tentative_long_resolves_to_zero_init (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass c99_to_tac\n"
      "source tests/test_c99_to_tac.py::TestCastAndStaticVariableTypes::test_tentative_long_resolves_to_zero_init\n"
      "args {\"args\": [{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" None Long None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) Int)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Ret(Constant(ConstInt(0)))\n"
      "  ])\n"
      "  StaticVariable(\"x\" true Long [ZeroInit(4)])\n"
      "])\n");
}

TEST_CASE("type_checking: test_c99_to_tac.py::TestCastAndStaticVariableTypes::test_uint_to_unsigned_long_long_cast_emits_zero_extend (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c99_to_tac.py::TestCastAndStaticVariableTypes::test_uint_to_unsigned_long_long_cast_emits_zero_extend\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.x\\\" Constant(ConstUInt(5) UInt) UInt None None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.y\\\" Cast(ULongLong Var(\\\"@0.x\\\" UInt) ULongLong) ULongLong None None None None false)))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.x\" Constant(ConstUInt(5) None) UInt None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.y\" Cast(ULongLong Var(\"@0.x\" None) None) ULongLong None None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.x\\\" Constant(ConstUInt(5) UInt) UInt None None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.y\\\" Cast(ULongLong Var(\\\"@0.x\\\" UInt) ULongLong) ULongLong None None None None false)))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.y\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"ULongLong\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("c99_to_tac: test_c99_to_tac.py::TestCastAndStaticVariableTypes::test_uint_to_unsigned_long_long_cast_emits_zero_extend (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass c99_to_tac\n"
      "source tests/test_c99_to_tac.py::TestCastAndStaticVariableTypes::test_uint_to_unsigned_long_long_cast_emits_zero_extend\n"
      "args {\"args\": [{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.y\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"ULongLong\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"args\": {\"0\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.y\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"ULongLong\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"ULongLong\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.x\" Constant(ConstUInt(5) UInt) UInt None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.y\" Cast(ULongLong Var(\"@0.x\" UInt) ULongLong) ULongLong None None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) Int)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Copy(Constant(ConstUInt(5)) Var(\"@0.x\"))\n"
      "    ZeroExtend(Var(\"@0.x\") Var(\"%0\"))\n"
      "    Copy(Var(\"%0\") Var(\"@1.y\"))\n"
      "    Ret(Constant(ConstInt(0)))\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("type_checking: test_c99_to_tac.py::TestCastAndStaticVariableTypes::test_user_written_zeros_also_coalesce (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c99_to_tac.py::TestCastAndStaticVariableTypes::test_user_written_zeros_also_coalesce\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" InitList([Constant(ConstInt(1) Int) Constant(ConstInt(0) Int) Constant(ConstInt(0) Int) Constant(ConstInt(0) Int) Constant(ConstInt(0) Int)] Array(Int 5) None) Array(Int 5) Static None None None false)))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" InitList([Constant(ConstInt(1) None) Constant(ConstInt(0) None) Constant(ConstInt(0) None) Constant(ConstInt(0) None) Constant(ConstInt(0) None)] None None) Array(Int 5) Static None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" InitList([Constant(ConstInt(1) Int) Constant(ConstInt(0) Int) Constant(ConstInt(0) Int) Constant(ConstInt(0) Int) Constant(ConstInt(0) Int)] Array(Int 5) None) Array(Int 5) Static None None None false)))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Int 5)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": {\"tuple\": [1, 0, 0, 0, 0]}}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("c99_to_tac: test_c99_to_tac.py::TestCastAndStaticVariableTypes::test_user_written_zeros_also_coalesce (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass c99_to_tac\n"
      "source tests/test_c99_to_tac.py::TestCastAndStaticVariableTypes::test_user_written_zeros_also_coalesce\n"
      "args {\"args\": [{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Int 5)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": {\"tuple\": [1, 0, 0, 0, 0]}}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" InitList([Constant(ConstInt(1) Int) Constant(ConstInt(0) Int) Constant(ConstInt(0) Int) Constant(ConstInt(0) Int) Constant(ConstInt(0) Int)] Array(Int 5) None) Array(Int 5) Static None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) Int)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Ret(Constant(ConstInt(0)))\n"
      "  ])\n"
      "  StaticVariable(\"@0.a\" false Pointer [IntInit(1) ZeroInit(8)])\n"
      "])\n");
}

TEST_CASE("type_checking: test_c99_to_tac.py::TestCastAndStaticVariableTypes::test_zero_filled_char_array_emits_zero_init (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c99_to_tac.py::TestCastAndStaticVariableTypes::test_zero_filled_char_array_emits_zero_init\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"arr\\\" None Array(Char 5) None None None None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"arr\" None Array(Char 5) None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"arr\\\" None Array(Char 5) None None None None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"arr\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Char 5)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("c99_to_tac: test_c99_to_tac.py::TestCastAndStaticVariableTypes::test_zero_filled_char_array_emits_zero_init (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass c99_to_tac\n"
      "source tests/test_c99_to_tac.py::TestCastAndStaticVariableTypes::test_zero_filled_char_array_emits_zero_init\n"
      "args {\"args\": [{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"arr\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Char 5)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"arr\" None Array(Char 5) None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) Int)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Ret(Constant(ConstInt(0)))\n"
      "  ])\n"
      "  StaticVariable(\"arr\" true Pointer [ZeroInit(5)])\n"
      "])\n");
}

TEST_CASE("type_checking: test_c99_to_tac.py::TestTempSymbolTableRegistration::test_int_binary_temp_registered_as_int (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c99_to_tac.py::TestTempSymbolTableRegistration::test_int_binary_temp_registered_as_int\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" Constant(ConstInt(1) Int) Int None None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.b\\\" Constant(ConstInt(2) Int) Int None None None None false)))\\n    S(Return(Binary(Add Var(\\\"@0.a\\\" Int) Var(\\\"@1.b\\\" Int) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Constant(ConstInt(1) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.b\" Constant(ConstInt(2) None) Int None None None None false)))\n"
      "    S(Return(Binary(Add Var(\"@0.a\" None) Var(\"@1.b\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" Constant(ConstInt(1) Int) Int None None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.b\\\" Constant(ConstInt(2) Int) Int None None None None false)))\\n    S(Return(Binary(Add Var(\\\"@0.a\\\" Int) Var(\\\"@1.b\\\" Int) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("c99_to_tac: test_c99_to_tac.py::TestTempSymbolTableRegistration::test_int_binary_temp_registered_as_int (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass c99_to_tac\n"
      "source tests/test_c99_to_tac.py::TestTempSymbolTableRegistration::test_int_binary_temp_registered_as_int\n"
      "args {\"args\": [{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"args\": {\"0\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Constant(ConstInt(1) Int) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.b\" Constant(ConstInt(2) Int) Int None None None None false)))\n"
      "    S(Return(Binary(Add Var(\"@0.a\" Int) Var(\"@1.b\" Int) Int)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Copy(Constant(ConstInt(1)) Var(\"@0.a\"))\n"
      "    Copy(Constant(ConstInt(2)) Var(\"@1.b\"))\n"
      "    Binary(Add Var(\"@0.a\") Var(\"@1.b\") Var(\"%0\"))\n"
      "    Ret(Var(\"%0\"))\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("type_checking: test_c99_to_tac.py::TestTempSymbolTableRegistration::test_long_promotion_temp_registered_as_long (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c99_to_tac.py::TestTempSymbolTableRegistration::test_long_promotion_temp_registered_as_long\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" Constant(ConstInt(1) Int) Int None None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.b\\\" Cast(Long Constant(ConstInt(2) Int) Long) Long None None None None false)))\\n    S(Return(Cast(Int Binary(Add Cast(Long Var(\\\"@0.a\\\" Int) Long) Var(\\\"@1.b\\\" Long) Long) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Constant(ConstInt(1) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.b\" Cast(Long Constant(ConstInt(2) None) None) Long None None None None false)))\n"
      "    S(Return(Cast(Int Binary(Add Cast(Long Var(\"@0.a\" None) None) Var(\"@1.b\" None) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" Constant(ConstInt(1) Int) Int None None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.b\\\" Cast(Long Constant(ConstInt(2) Int) Long) Long None None None None false)))\\n    S(Return(Cast(Int Binary(Add Cast(Long Var(\\\"@0.a\\\" Int) Long) Var(\\\"@1.b\\\" Long) Long) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("c99_to_tac: test_c99_to_tac.py::TestTempSymbolTableRegistration::test_long_promotion_temp_registered_as_long (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass c99_to_tac\n"
      "source tests/test_c99_to_tac.py::TestTempSymbolTableRegistration::test_long_promotion_temp_registered_as_long\n"
      "args {\"args\": [{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"args\": {\"0\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%3\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Constant(ConstInt(1) Int) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.b\" Cast(Long Constant(ConstInt(2) Int) Long) Long None None None None false)))\n"
      "    S(Return(Cast(Int Binary(Add Cast(Long Var(\"@0.a\" Int) Long) Var(\"@1.b\" Long) Long) Int)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Copy(Constant(ConstInt(1)) Var(\"@0.a\"))\n"
      "    SignExtend(Constant(ConstInt(2)) Var(\"%0\"))\n"
      "    Copy(Var(\"%0\") Var(\"@1.b\"))\n"
      "    SignExtend(Var(\"@0.a\") Var(\"%1\"))\n"
      "    Binary(Add Var(\"%1\") Var(\"@1.b\") Var(\"%2\"))\n"
      "    Truncate(Var(\"%2\") Var(\"%3\"))\n"
      "    Ret(Var(\"%3\"))\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("type_checking: test_c99_to_tac.py::TestPointerArithmeticLowering::test_char_ptr_minus_char_ptr_subtracts_no_divide (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c99_to_tac.py::TestPointerArithmeticLowering::test_char_ptr_minus_char_ptr_subtracts_no_divide\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" Cast(Char Constant(ConstInt(0) Int) Char) Char None None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.p\\\" AddressOf(Var(\\\"@0.a\\\" Char) Pointer(Char)) Pointer(Char) None None None None false)))\\n    D(VarDecl(var_decl(\\\"@2.q\\\" AddressOf(Var(\\\"@0.a\\\" Char) Pointer(Char)) Pointer(Char) None None None None false)))\\n    S(Return(Cast(Long Binary(Subtract Var(\\\"@1.p\\\" Pointer(Char)) Var(\\\"@2.q\\\" Pointer(Char)) Int) Long)))\\n  ]) FunType([] Long) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Constant(ConstInt(0) None) Char None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.p\" AddressOf(Var(\"@0.a\" None) None) Pointer(Char) None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@2.q\" AddressOf(Var(\"@0.a\" None) None) Pointer(Char) None None None None false)))\n"
      "    S(Return(Binary(Subtract Var(\"@1.p\" None) Var(\"@2.q\" None) None)))\n"
      "  ]) FunType([] Long) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" Cast(Char Constant(ConstInt(0) Int) Char) Char None None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.p\\\" AddressOf(Var(\\\"@0.a\\\" Char) Pointer(Char)) Pointer(Char) None None None None false)))\\n    D(VarDecl(var_decl(\\\"@2.q\\\" AddressOf(Var(\\\"@0.a\\\" Char) Pointer(Char)) Pointer(Char) None None None None false)))\\n    S(Return(Cast(Long Binary(Subtract Var(\\\"@1.p\\\" Pointer(Char)) Var(\\\"@2.q\\\" Pointer(Char)) Int) Long)))\\n  ]) FunType([] Long) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Long)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Char\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Char)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@2.q\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Char)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("c99_to_tac: test_c99_to_tac.py::TestPointerArithmeticLowering::test_char_ptr_minus_char_ptr_subtracts_no_divide (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass c99_to_tac\n"
      "source tests/test_c99_to_tac.py::TestPointerArithmeticLowering::test_char_ptr_minus_char_ptr_subtracts_no_divide\n"
      "args {\"args\": [{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Long)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Char\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Char)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@2.q\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Char)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"args\": {\"0\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Long)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Char\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Char)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@2.q\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Char)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Char\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Char)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Char)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%3\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%4\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Cast(Char Constant(ConstInt(0) Int) Char) Char None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.p\" AddressOf(Var(\"@0.a\" Char) Pointer(Char)) Pointer(Char) None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@2.q\" AddressOf(Var(\"@0.a\" Char) Pointer(Char)) Pointer(Char) None None None None false)))\n"
      "    S(Return(Cast(Long Binary(Subtract Var(\"@1.p\" Pointer(Char)) Var(\"@2.q\" Pointer(Char)) Int) Long)))\n"
      "  ]) FunType([] Long) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Truncate(Constant(ConstInt(0)) Var(\"%0\"))\n"
      "    Copy(Var(\"%0\") Var(\"@0.a\"))\n"
      "    GetAddress(Var(\"@0.a\") Var(\"%1\"))\n"
      "    Copy(Var(\"%1\") Var(\"@1.p\"))\n"
      "    GetAddress(Var(\"@0.a\") Var(\"%2\"))\n"
      "    Copy(Var(\"%2\") Var(\"@2.q\"))\n"
      "    Binary(Subtract Var(\"@1.p\") Var(\"@2.q\") Var(\"%3\"))\n"
      "    SignExtend(Var(\"%3\") Var(\"%4\"))\n"
      "    Ret(Var(\"%4\"))\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("type_checking: test_c99_to_tac.py::TestPointerArithmeticLowering::test_char_ptr_plus_int_no_scaling (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c99_to_tac.py::TestPointerArithmeticLowering::test_char_ptr_plus_int_no_scaling\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" Cast(Char Constant(ConstInt(0) Int) Char) Char None None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.p\\\" AddressOf(Var(\\\"@0.a\\\" Char) Pointer(Char)) Pointer(Char) None None None None false)))\\n    S(Return(Cast(Long Binary(Add Var(\\\"@1.p\\\" Pointer(Char)) Constant(ConstInt(1) Int) Pointer(Char)) Long)))\\n  ]) FunType([] Long) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Constant(ConstInt(0) None) Char None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.p\" AddressOf(Var(\"@0.a\" None) None) Pointer(Char) None None None None false)))\n"
      "    S(Return(Cast(Long Binary(Add Var(\"@1.p\" None) Constant(ConstInt(1) None) None) None)))\n"
      "  ]) FunType([] Long) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" Cast(Char Constant(ConstInt(0) Int) Char) Char None None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.p\\\" AddressOf(Var(\\\"@0.a\\\" Char) Pointer(Char)) Pointer(Char) None None None None false)))\\n    S(Return(Cast(Long Binary(Add Var(\\\"@1.p\\\" Pointer(Char)) Constant(ConstInt(1) Int) Pointer(Char)) Long)))\\n  ]) FunType([] Long) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Long)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Char\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Char)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("c99_to_tac: test_c99_to_tac.py::TestPointerArithmeticLowering::test_char_ptr_plus_int_no_scaling (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass c99_to_tac\n"
      "source tests/test_c99_to_tac.py::TestPointerArithmeticLowering::test_char_ptr_plus_int_no_scaling\n"
      "args {\"args\": [{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Long)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Char\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Char)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"args\": {\"0\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Long)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Char\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Char)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Char\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Char)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Char)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%3\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Cast(Char Constant(ConstInt(0) Int) Char) Char None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.p\" AddressOf(Var(\"@0.a\" Char) Pointer(Char)) Pointer(Char) None None None None false)))\n"
      "    S(Return(Cast(Long Binary(Add Var(\"@1.p\" Pointer(Char)) Constant(ConstInt(1) Int) Pointer(Char)) Long)))\n"
      "  ]) FunType([] Long) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Truncate(Constant(ConstInt(0)) Var(\"%0\"))\n"
      "    Copy(Var(\"%0\") Var(\"@0.a\"))\n"
      "    GetAddress(Var(\"@0.a\") Var(\"%1\"))\n"
      "    Copy(Var(\"%1\") Var(\"@1.p\"))\n"
      "    Binary(Add Var(\"@1.p\") Constant(ConstInt(1)) Var(\"%2\"))\n"
      "    ZeroExtend(Var(\"%2\") Var(\"%3\"))\n"
      "    Ret(Var(\"%3\"))\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("type_checking: test_c99_to_tac.py::TestPointerArithmeticLowering::test_double_ptr_plus_int_scales_by_eight (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c99_to_tac.py::TestPointerArithmeticLowering::test_double_ptr_plus_int_scales_by_eight\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" Constant(ConstDouble(0) Double) Double None None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.p\\\" AddressOf(Var(\\\"@0.a\\\" Double) Pointer(Double)) Pointer(Double) None None None None false)))\\n    S(Return(Cast(Long Binary(Add Var(\\\"@1.p\\\" Pointer(Double)) Constant(ConstInt(1) Int) Pointer(Double)) Long)))\\n  ]) FunType([] Long) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Constant(ConstDouble(0) None) Double None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.p\" AddressOf(Var(\"@0.a\" None) None) Pointer(Double) None None None None false)))\n"
      "    S(Return(Cast(Long Binary(Add Var(\"@1.p\" None) Constant(ConstInt(1) None) None) None)))\n"
      "  ]) FunType([] Long) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" Constant(ConstDouble(0) Double) Double None None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.p\\\" AddressOf(Var(\\\"@0.a\\\" Double) Pointer(Double)) Pointer(Double) None None None None false)))\\n    S(Return(Cast(Long Binary(Add Var(\\\"@1.p\\\" Pointer(Double)) Constant(ConstInt(1) Int) Pointer(Double)) Long)))\\n  ]) FunType([] Long) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Long)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Double\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Double)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("c99_to_tac: test_c99_to_tac.py::TestPointerArithmeticLowering::test_double_ptr_plus_int_scales_by_eight (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass c99_to_tac\n"
      "source tests/test_c99_to_tac.py::TestPointerArithmeticLowering::test_double_ptr_plus_int_scales_by_eight\n"
      "args {\"args\": [{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Long)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Double\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Double)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"args\": {\"0\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Long)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Double\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Double)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Double)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Double)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%3\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Constant(ConstDouble(0) Double) Double None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.p\" AddressOf(Var(\"@0.a\" Double) Pointer(Double)) Pointer(Double) None None None None false)))\n"
      "    S(Return(Cast(Long Binary(Add Var(\"@1.p\" Pointer(Double)) Constant(ConstInt(1) Int) Pointer(Double)) Long)))\n"
      "  ]) FunType([] Long) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Copy(Constant(ConstDouble(0)) Var(\"@0.a\"))\n"
      "    GetAddress(Var(\"@0.a\") Var(\"%0\"))\n"
      "    Copy(Var(\"%0\") Var(\"@1.p\"))\n"
      "    Binary(Multiply Constant(ConstInt(1)) Constant(ConstInt(8)) Var(\"%1\"))\n"
      "    Binary(Add Var(\"@1.p\") Var(\"%1\") Var(\"%2\"))\n"
      "    ZeroExtend(Var(\"%2\") Var(\"%3\"))\n"
      "    Ret(Var(\"%3\"))\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("type_checking: test_c99_to_tac.py::TestPointerArithmeticLowering::test_int_plus_int_ptr_swaps_operand_order (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c99_to_tac.py::TestPointerArithmeticLowering::test_int_plus_int_ptr_swaps_operand_order\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" Constant(ConstInt(0) Int) Int None None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.p\\\" AddressOf(Var(\\\"@0.a\\\" Int) Pointer(Int)) Pointer(Int) None None None None false)))\\n    S(Return(Cast(Long Binary(Add Constant(ConstInt(1) Int) Var(\\\"@1.p\\\" Pointer(Int)) Pointer(Int)) Long)))\\n  ]) FunType([] Long) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.p\" AddressOf(Var(\"@0.a\" None) None) Pointer(Int) None None None None false)))\n"
      "    S(Return(Cast(Long Binary(Add Constant(ConstInt(1) None) Var(\"@1.p\" None) None) None)))\n"
      "  ]) FunType([] Long) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" Constant(ConstInt(0) Int) Int None None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.p\\\" AddressOf(Var(\\\"@0.a\\\" Int) Pointer(Int)) Pointer(Int) None None None None false)))\\n    S(Return(Cast(Long Binary(Add Constant(ConstInt(1) Int) Var(\\\"@1.p\\\" Pointer(Int)) Pointer(Int)) Long)))\\n  ]) FunType([] Long) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Long)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("c99_to_tac: test_c99_to_tac.py::TestPointerArithmeticLowering::test_int_plus_int_ptr_swaps_operand_order (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass c99_to_tac\n"
      "source tests/test_c99_to_tac.py::TestPointerArithmeticLowering::test_int_plus_int_ptr_swaps_operand_order\n"
      "args {\"args\": [{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Long)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"args\": {\"0\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Long)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%3\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Constant(ConstInt(0) Int) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.p\" AddressOf(Var(\"@0.a\" Int) Pointer(Int)) Pointer(Int) None None None None false)))\n"
      "    S(Return(Cast(Long Binary(Add Constant(ConstInt(1) Int) Var(\"@1.p\" Pointer(Int)) Pointer(Int)) Long)))\n"
      "  ]) FunType([] Long) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Copy(Constant(ConstInt(0)) Var(\"@0.a\"))\n"
      "    GetAddress(Var(\"@0.a\") Var(\"%0\"))\n"
      "    Copy(Var(\"%0\") Var(\"@1.p\"))\n"
      "    Binary(Multiply Constant(ConstInt(1)) Constant(ConstInt(2)) Var(\"%1\"))\n"
      "    Binary(Add Var(\"@1.p\") Var(\"%1\") Var(\"%2\"))\n"
      "    ZeroExtend(Var(\"%2\") Var(\"%3\"))\n"
      "    Ret(Var(\"%3\"))\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("type_checking: test_c99_to_tac.py::TestPointerArithmeticLowering::test_int_ptr_minus_int_emits_subtract (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c99_to_tac.py::TestPointerArithmeticLowering::test_int_ptr_minus_int_emits_subtract\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" Constant(ConstInt(0) Int) Int None None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.p\\\" AddressOf(Var(\\\"@0.a\\\" Int) Pointer(Int)) Pointer(Int) None None None None false)))\\n    S(Return(Cast(Long Binary(Subtract Var(\\\"@1.p\\\" Pointer(Int)) Constant(ConstInt(1) Int) Pointer(Int)) Long)))\\n  ]) FunType([] Long) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.p\" AddressOf(Var(\"@0.a\" None) None) Pointer(Int) None None None None false)))\n"
      "    S(Return(Cast(Long Binary(Subtract Var(\"@1.p\" None) Constant(ConstInt(1) None) None) None)))\n"
      "  ]) FunType([] Long) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" Constant(ConstInt(0) Int) Int None None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.p\\\" AddressOf(Var(\\\"@0.a\\\" Int) Pointer(Int)) Pointer(Int) None None None None false)))\\n    S(Return(Cast(Long Binary(Subtract Var(\\\"@1.p\\\" Pointer(Int)) Constant(ConstInt(1) Int) Pointer(Int)) Long)))\\n  ]) FunType([] Long) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Long)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("c99_to_tac: test_c99_to_tac.py::TestPointerArithmeticLowering::test_int_ptr_minus_int_emits_subtract (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass c99_to_tac\n"
      "source tests/test_c99_to_tac.py::TestPointerArithmeticLowering::test_int_ptr_minus_int_emits_subtract\n"
      "args {\"args\": [{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Long)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"args\": {\"0\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Long)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%3\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Constant(ConstInt(0) Int) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.p\" AddressOf(Var(\"@0.a\" Int) Pointer(Int)) Pointer(Int) None None None None false)))\n"
      "    S(Return(Cast(Long Binary(Subtract Var(\"@1.p\" Pointer(Int)) Constant(ConstInt(1) Int) Pointer(Int)) Long)))\n"
      "  ]) FunType([] Long) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Copy(Constant(ConstInt(0)) Var(\"@0.a\"))\n"
      "    GetAddress(Var(\"@0.a\") Var(\"%0\"))\n"
      "    Copy(Var(\"%0\") Var(\"@1.p\"))\n"
      "    Binary(Multiply Constant(ConstInt(1)) Constant(ConstInt(2)) Var(\"%1\"))\n"
      "    Binary(Subtract Var(\"@1.p\") Var(\"%1\") Var(\"%2\"))\n"
      "    ZeroExtend(Var(\"%2\") Var(\"%3\"))\n"
      "    Ret(Var(\"%3\"))\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("type_checking: test_c99_to_tac.py::TestPointerArithmeticLowering::test_int_ptr_minus_int_ptr_subtracts_then_divides_by_two (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c99_to_tac.py::TestPointerArithmeticLowering::test_int_ptr_minus_int_ptr_subtracts_then_divides_by_two\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" Constant(ConstInt(0) Int) Int None None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.p\\\" AddressOf(Var(\\\"@0.a\\\" Int) Pointer(Int)) Pointer(Int) None None None None false)))\\n    D(VarDecl(var_decl(\\\"@2.q\\\" AddressOf(Var(\\\"@0.a\\\" Int) Pointer(Int)) Pointer(Int) None None None None false)))\\n    S(Return(Cast(Long Binary(Subtract Var(\\\"@1.p\\\" Pointer(Int)) Var(\\\"@2.q\\\" Pointer(Int)) Int) Long)))\\n  ]) FunType([] Long) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.p\" AddressOf(Var(\"@0.a\" None) None) Pointer(Int) None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@2.q\" AddressOf(Var(\"@0.a\" None) None) Pointer(Int) None None None None false)))\n"
      "    S(Return(Binary(Subtract Var(\"@1.p\" None) Var(\"@2.q\" None) None)))\n"
      "  ]) FunType([] Long) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" Constant(ConstInt(0) Int) Int None None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.p\\\" AddressOf(Var(\\\"@0.a\\\" Int) Pointer(Int)) Pointer(Int) None None None None false)))\\n    D(VarDecl(var_decl(\\\"@2.q\\\" AddressOf(Var(\\\"@0.a\\\" Int) Pointer(Int)) Pointer(Int) None None None None false)))\\n    S(Return(Cast(Long Binary(Subtract Var(\\\"@1.p\\\" Pointer(Int)) Var(\\\"@2.q\\\" Pointer(Int)) Int) Long)))\\n  ]) FunType([] Long) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Long)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@2.q\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("c99_to_tac: test_c99_to_tac.py::TestPointerArithmeticLowering::test_int_ptr_minus_int_ptr_subtracts_then_divides_by_two (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass c99_to_tac\n"
      "source tests/test_c99_to_tac.py::TestPointerArithmeticLowering::test_int_ptr_minus_int_ptr_subtracts_then_divides_by_two\n"
      "args {\"args\": [{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Long)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@2.q\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"args\": {\"0\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Long)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@2.q\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%3\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%4\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Constant(ConstInt(0) Int) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.p\" AddressOf(Var(\"@0.a\" Int) Pointer(Int)) Pointer(Int) None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@2.q\" AddressOf(Var(\"@0.a\" Int) Pointer(Int)) Pointer(Int) None None None None false)))\n"
      "    S(Return(Cast(Long Binary(Subtract Var(\"@1.p\" Pointer(Int)) Var(\"@2.q\" Pointer(Int)) Int) Long)))\n"
      "  ]) FunType([] Long) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Copy(Constant(ConstInt(0)) Var(\"@0.a\"))\n"
      "    GetAddress(Var(\"@0.a\") Var(\"%0\"))\n"
      "    Copy(Var(\"%0\") Var(\"@1.p\"))\n"
      "    GetAddress(Var(\"@0.a\") Var(\"%1\"))\n"
      "    Copy(Var(\"%1\") Var(\"@2.q\"))\n"
      "    Binary(Subtract Var(\"@1.p\") Var(\"@2.q\") Var(\"%2\"))\n"
      "    Binary(Divide Var(\"%2\") Constant(ConstInt(2)) Var(\"%3\"))\n"
      "    SignExtend(Var(\"%3\") Var(\"%4\"))\n"
      "    Ret(Var(\"%4\"))\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("type_checking: test_c99_to_tac.py::TestPointerArithmeticLowering::test_int_ptr_plus_int_scales_by_two (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c99_to_tac.py::TestPointerArithmeticLowering::test_int_ptr_plus_int_scales_by_two\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" Constant(ConstInt(0) Int) Int None None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.p\\\" AddressOf(Var(\\\"@0.a\\\" Int) Pointer(Int)) Pointer(Int) None None None None false)))\\n    S(Return(Cast(Long Binary(Add Var(\\\"@1.p\\\" Pointer(Int)) Constant(ConstInt(1) Int) Pointer(Int)) Long)))\\n  ]) FunType([] Long) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.p\" AddressOf(Var(\"@0.a\" None) None) Pointer(Int) None None None None false)))\n"
      "    S(Return(Cast(Long Binary(Add Var(\"@1.p\" None) Constant(ConstInt(1) None) None) None)))\n"
      "  ]) FunType([] Long) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" Constant(ConstInt(0) Int) Int None None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.p\\\" AddressOf(Var(\\\"@0.a\\\" Int) Pointer(Int)) Pointer(Int) None None None None false)))\\n    S(Return(Cast(Long Binary(Add Var(\\\"@1.p\\\" Pointer(Int)) Constant(ConstInt(1) Int) Pointer(Int)) Long)))\\n  ]) FunType([] Long) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Long)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("c99_to_tac: test_c99_to_tac.py::TestPointerArithmeticLowering::test_int_ptr_plus_int_scales_by_two (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass c99_to_tac\n"
      "source tests/test_c99_to_tac.py::TestPointerArithmeticLowering::test_int_ptr_plus_int_scales_by_two\n"
      "args {\"args\": [{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Long)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"args\": {\"0\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Long)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%3\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Constant(ConstInt(0) Int) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.p\" AddressOf(Var(\"@0.a\" Int) Pointer(Int)) Pointer(Int) None None None None false)))\n"
      "    S(Return(Cast(Long Binary(Add Var(\"@1.p\" Pointer(Int)) Constant(ConstInt(1) Int) Pointer(Int)) Long)))\n"
      "  ]) FunType([] Long) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Copy(Constant(ConstInt(0)) Var(\"@0.a\"))\n"
      "    GetAddress(Var(\"@0.a\") Var(\"%0\"))\n"
      "    Copy(Var(\"%0\") Var(\"@1.p\"))\n"
      "    Binary(Multiply Constant(ConstInt(1)) Constant(ConstInt(2)) Var(\"%1\"))\n"
      "    Binary(Add Var(\"@1.p\") Var(\"%1\") Var(\"%2\"))\n"
      "    ZeroExtend(Var(\"%2\") Var(\"%3\"))\n"
      "    Ret(Var(\"%3\"))\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("type_checking: test_c99_to_tac.py::TestPointerArithmeticLowering::test_pointer_arithmetic_temp_is_pointer_typed (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c99_to_tac.py::TestPointerArithmeticLowering::test_pointer_arithmetic_temp_is_pointer_typed\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" Constant(ConstInt(0) Int) Int None None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.p\\\" AddressOf(Var(\\\"@0.a\\\" Int) Pointer(Int)) Pointer(Int) None None None None false)))\\n    S(Return(Cast(Long Binary(Add Var(\\\"@1.p\\\" Pointer(Int)) Constant(ConstInt(1) Int) Pointer(Int)) Long)))\\n  ]) FunType([] Long) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.p\" AddressOf(Var(\"@0.a\" None) None) Pointer(Int) None None None None false)))\n"
      "    S(Return(Cast(Long Binary(Add Var(\"@1.p\" None) Constant(ConstInt(1) None) None) None)))\n"
      "  ]) FunType([] Long) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" Constant(ConstInt(0) Int) Int None None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.p\\\" AddressOf(Var(\\\"@0.a\\\" Int) Pointer(Int)) Pointer(Int) None None None None false)))\\n    S(Return(Cast(Long Binary(Add Var(\\\"@1.p\\\" Pointer(Int)) Constant(ConstInt(1) Int) Pointer(Int)) Long)))\\n  ]) FunType([] Long) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Long)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("c99_to_tac: test_c99_to_tac.py::TestPointerArithmeticLowering::test_pointer_arithmetic_temp_is_pointer_typed (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass c99_to_tac\n"
      "source tests/test_c99_to_tac.py::TestPointerArithmeticLowering::test_pointer_arithmetic_temp_is_pointer_typed\n"
      "args {\"args\": [{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Long)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"args\": {\"0\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Long)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%3\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Constant(ConstInt(0) Int) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.p\" AddressOf(Var(\"@0.a\" Int) Pointer(Int)) Pointer(Int) None None None None false)))\n"
      "    S(Return(Cast(Long Binary(Add Var(\"@1.p\" Pointer(Int)) Constant(ConstInt(1) Int) Pointer(Int)) Long)))\n"
      "  ]) FunType([] Long) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Copy(Constant(ConstInt(0)) Var(\"@0.a\"))\n"
      "    GetAddress(Var(\"@0.a\") Var(\"%0\"))\n"
      "    Copy(Var(\"%0\") Var(\"@1.p\"))\n"
      "    Binary(Multiply Constant(ConstInt(1)) Constant(ConstInt(2)) Var(\"%1\"))\n"
      "    Binary(Add Var(\"@1.p\") Var(\"%1\") Var(\"%2\"))\n"
      "    ZeroExtend(Var(\"%2\") Var(\"%3\"))\n"
      "    Ret(Var(\"%3\"))\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("type_checking: test_c99_to_tac.py::TestSubscriptLowering::test_array_decay_in_pointer_init (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c99_to_tac.py::TestSubscriptLowering::test_array_decay_in_pointer_init\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" None Array(Int 10) None None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.p\\\" AddressOf(Var(\\\"@0.a\\\" Array(Int 10)) Pointer(Int)) Pointer(Int) None None None None false)))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" None Array(Int 10) None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.p\" Var(\"@0.a\" None) Pointer(Int) None None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" None Array(Int 10) None None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.p\\\" AddressOf(Var(\\\"@0.a\\\" Array(Int 10)) Pointer(Int)) Pointer(Int) None None None None false)))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Int 10)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("c99_to_tac: test_c99_to_tac.py::TestSubscriptLowering::test_array_decay_in_pointer_init (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass c99_to_tac\n"
      "source tests/test_c99_to_tac.py::TestSubscriptLowering::test_array_decay_in_pointer_init\n"
      "args {\"args\": [{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Int 10)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"args\": {\"0\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Int 10)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" None Array(Int 10) None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.p\" AddressOf(Var(\"@0.a\" Array(Int 10)) Pointer(Int)) Pointer(Int) None None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) Int)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    GetAddress(Var(\"@0.a\") Var(\"%0\"))\n"
      "    Copy(Var(\"%0\") Var(\"@1.p\"))\n"
      "    Ret(Constant(ConstInt(0)))\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("type_checking: test_c99_to_tac.py::TestSubscriptLowering::test_int_subscript_scales_by_two (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c99_to_tac.py::TestSubscriptLowering::test_int_subscript_scales_by_two\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" None Array(Int 10) None None None None false)))\\n    S(Return(Subscript(AddressOf(Var(\\\"@0.a\\\" Array(Int 10)) Pointer(Int)) Constant(ConstInt(3) Int) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" None Array(Int 10) None None None None false)))\n"
      "    S(Return(Subscript(Var(\"@0.a\" None) Constant(ConstInt(3) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" None Array(Int 10) None None None None false)))\\n    S(Return(Subscript(AddressOf(Var(\\\"@0.a\\\" Array(Int 10)) Pointer(Int)) Constant(ConstInt(3) Int) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Int 10)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("c99_to_tac: test_c99_to_tac.py::TestSubscriptLowering::test_int_subscript_scales_by_two (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass c99_to_tac\n"
      "source tests/test_c99_to_tac.py::TestSubscriptLowering::test_int_subscript_scales_by_two\n"
      "args {\"args\": [{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Int 10)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"args\": {\"0\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Int 10)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%3\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" None Array(Int 10) None None None None false)))\n"
      "    S(Return(Subscript(AddressOf(Var(\"@0.a\" Array(Int 10)) Pointer(Int)) Constant(ConstInt(3) Int) Int)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    GetAddress(Var(\"@0.a\") Var(\"%0\"))\n"
      "    Binary(Multiply Constant(ConstInt(3)) Constant(ConstInt(2)) Var(\"%1\"))\n"
      "    Binary(Add Var(\"%0\") Var(\"%1\") Var(\"%2\"))\n"
      "    Load(Var(\"%2\") Var(\"%3\") false)\n"
      "    Ret(Var(\"%3\"))\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("type_checking: test_c99_to_tac.py::TestSubscriptLowering::test_subscript_read_emits_load (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c99_to_tac.py::TestSubscriptLowering::test_subscript_read_emits_load\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" None Array(Int 10) None None None None false)))\\n    S(Return(Subscript(AddressOf(Var(\\\"@0.a\\\" Array(Int 10)) Pointer(Int)) Constant(ConstInt(3) Int) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" None Array(Int 10) None None None None false)))\n"
      "    S(Return(Subscript(Var(\"@0.a\" None) Constant(ConstInt(3) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" None Array(Int 10) None None None None false)))\\n    S(Return(Subscript(AddressOf(Var(\\\"@0.a\\\" Array(Int 10)) Pointer(Int)) Constant(ConstInt(3) Int) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Int 10)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("c99_to_tac: test_c99_to_tac.py::TestSubscriptLowering::test_subscript_read_emits_load (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass c99_to_tac\n"
      "source tests/test_c99_to_tac.py::TestSubscriptLowering::test_subscript_read_emits_load\n"
      "args {\"args\": [{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Int 10)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"args\": {\"0\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Int 10)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%3\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" None Array(Int 10) None None None None false)))\n"
      "    S(Return(Subscript(AddressOf(Var(\"@0.a\" Array(Int 10)) Pointer(Int)) Constant(ConstInt(3) Int) Int)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    GetAddress(Var(\"@0.a\") Var(\"%0\"))\n"
      "    Binary(Multiply Constant(ConstInt(3)) Constant(ConstInt(2)) Var(\"%1\"))\n"
      "    Binary(Add Var(\"%0\") Var(\"%1\") Var(\"%2\"))\n"
      "    Load(Var(\"%2\") Var(\"%3\") false)\n"
      "    Ret(Var(\"%3\"))\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("type_checking: test_c99_to_tac.py::TestSubscriptLowering::test_subscript_write_emits_store (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c99_to_tac.py::TestSubscriptLowering::test_subscript_write_emits_store\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" None Array(Int 10) None None None None false)))\\n    S(Expression(Assignment(Subscript(AddressOf(Var(\\\"@0.a\\\" Array(Int 10)) Pointer(Int)) Constant(ConstInt(3) Int) Int) Constant(ConstInt(5) Int) Int)))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" None Array(Int 10) None None None None false)))\n"
      "    S(Expression(Assignment(Subscript(Var(\"@0.a\" None) Constant(ConstInt(3) None) None) Constant(ConstInt(5) None) None)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" None Array(Int 10) None None None None false)))\\n    S(Expression(Assignment(Subscript(AddressOf(Var(\\\"@0.a\\\" Array(Int 10)) Pointer(Int)) Constant(ConstInt(3) Int) Int) Constant(ConstInt(5) Int) Int)))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Int 10)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("c99_to_tac: test_c99_to_tac.py::TestSubscriptLowering::test_subscript_write_emits_store (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass c99_to_tac\n"
      "source tests/test_c99_to_tac.py::TestSubscriptLowering::test_subscript_write_emits_store\n"
      "args {\"args\": [{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Int 10)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"args\": {\"0\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Int 10)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" None Array(Int 10) None None None None false)))\n"
      "    S(Expression(Assignment(Subscript(AddressOf(Var(\"@0.a\" Array(Int 10)) Pointer(Int)) Constant(ConstInt(3) Int) Int) Constant(ConstInt(5) Int) Int)))\n"
      "    S(Return(Constant(ConstInt(0) Int)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    GetAddress(Var(\"@0.a\") Var(\"%0\"))\n"
      "    Binary(Multiply Constant(ConstInt(3)) Constant(ConstInt(2)) Var(\"%1\"))\n"
      "    Binary(Add Var(\"%0\") Var(\"%1\") Var(\"%2\"))\n"
      "    Store(Constant(ConstInt(5)) Var(\"%2\") false)\n"
      "    Ret(Constant(ConstInt(0)))\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("type_checking: test_c99_to_tac.py::TestIncrementDecrementLowering::test_postfix_on_char_pointer_no_scaling (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c99_to_tac.py::TestIncrementDecrementLowering::test_postfix_on_char_pointer_no_scaling\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" None Char None None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.p\\\" AddressOf(Var(\\\"@0.a\\\" Char) Pointer(Char)) Pointer(Char) None None None None false)))\\n    S(Expression(Postfix(Increment Var(\\\"@1.p\\\" Pointer(Char)) Pointer(Char))))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" None Char None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.p\" AddressOf(Var(\"@0.a\" None) None) Pointer(Char) None None None None false)))\n"
      "    S(Expression(Postfix(Increment Var(\"@1.p\" None) None)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" None Char None None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.p\\\" AddressOf(Var(\\\"@0.a\\\" Char) Pointer(Char)) Pointer(Char) None None None None false)))\\n    S(Expression(Postfix(Increment Var(\\\"@1.p\\\" Pointer(Char)) Pointer(Char))))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Char\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Char)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("c99_to_tac: test_c99_to_tac.py::TestIncrementDecrementLowering::test_postfix_on_char_pointer_no_scaling (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass c99_to_tac\n"
      "source tests/test_c99_to_tac.py::TestIncrementDecrementLowering::test_postfix_on_char_pointer_no_scaling\n"
      "args {\"args\": [{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Char\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Char)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"args\": {\"0\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Char\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Char)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Char)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Char)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Char)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" None Char None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.p\" AddressOf(Var(\"@0.a\" Char) Pointer(Char)) Pointer(Char) None None None None false)))\n"
      "    S(Expression(Postfix(Increment Var(\"@1.p\" Pointer(Char)) Pointer(Char))))\n"
      "    S(Return(Constant(ConstInt(0) Int)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    GetAddress(Var(\"@0.a\") Var(\"%0\"))\n"
      "    Copy(Var(\"%0\") Var(\"@1.p\"))\n"
      "    Copy(Var(\"@1.p\") Var(\"%1\"))\n"
      "    Binary(Add Var(\"@1.p\") Constant(ConstInt(1)) Var(\"%2\"))\n"
      "    Copy(Var(\"%2\") Var(\"@1.p\"))\n"
      "    Ret(Constant(ConstInt(0)))\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("type_checking: test_c99_to_tac.py::TestIncrementDecrementLowering::test_postfix_on_dereference (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c99_to_tac.py::TestIncrementDecrementLowering::test_postfix_on_dereference\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" None Int None None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.p\\\" AddressOf(Var(\\\"@0.a\\\" Int) Pointer(Int)) Pointer(Int) None None None None false)))\\n    S(Expression(Postfix(Increment Dereference(Var(\\\"@1.p\\\" Pointer(Int)) Int) Int)))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" None Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.p\" AddressOf(Var(\"@0.a\" None) None) Pointer(Int) None None None None false)))\n"
      "    S(Expression(Postfix(Increment Dereference(Var(\"@1.p\" None) None) None)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" None Int None None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.p\\\" AddressOf(Var(\\\"@0.a\\\" Int) Pointer(Int)) Pointer(Int) None None None None false)))\\n    S(Expression(Postfix(Increment Dereference(Var(\\\"@1.p\\\" Pointer(Int)) Int) Int)))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("c99_to_tac: test_c99_to_tac.py::TestIncrementDecrementLowering::test_postfix_on_dereference (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass c99_to_tac\n"
      "source tests/test_c99_to_tac.py::TestIncrementDecrementLowering::test_postfix_on_dereference\n"
      "args {\"args\": [{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"args\": {\"0\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" None Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.p\" AddressOf(Var(\"@0.a\" Int) Pointer(Int)) Pointer(Int) None None None None false)))\n"
      "    S(Expression(Postfix(Increment Dereference(Var(\"@1.p\" Pointer(Int)) Int) Int)))\n"
      "    S(Return(Constant(ConstInt(0) Int)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    GetAddress(Var(\"@0.a\") Var(\"%0\"))\n"
      "    Copy(Var(\"%0\") Var(\"@1.p\"))\n"
      "    Load(Var(\"@1.p\") Var(\"%1\") false)\n"
      "    Binary(Add Var(\"%1\") Constant(ConstInt(1)) Var(\"%2\"))\n"
      "    Store(Var(\"%2\") Var(\"@1.p\") false)\n"
      "    Ret(Constant(ConstInt(0)))\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("type_checking: test_c99_to_tac.py::TestIncrementDecrementLowering::test_postfix_on_int_pointer_var_scales_by_pointee_size (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c99_to_tac.py::TestIncrementDecrementLowering::test_postfix_on_int_pointer_var_scales_by_pointee_size\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" Constant(ConstInt(0) Int) Int None None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.p\\\" AddressOf(Var(\\\"@0.a\\\" Int) Pointer(Int)) Pointer(Int) None None None None false)))\\n    S(Expression(Postfix(Increment Var(\\\"@1.p\\\" Pointer(Int)) Pointer(Int))))\\n    S(Return(Cast(Long Var(\\\"@1.p\\\" Pointer(Int)) Long)))\\n  ]) FunType([] Long) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.p\" AddressOf(Var(\"@0.a\" None) None) Pointer(Int) None None None None false)))\n"
      "    S(Expression(Postfix(Increment Var(\"@1.p\" None) None)))\n"
      "    S(Return(Cast(Long Var(\"@1.p\" None) None)))\n"
      "  ]) FunType([] Long) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" Constant(ConstInt(0) Int) Int None None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.p\\\" AddressOf(Var(\\\"@0.a\\\" Int) Pointer(Int)) Pointer(Int) None None None None false)))\\n    S(Expression(Postfix(Increment Var(\\\"@1.p\\\" Pointer(Int)) Pointer(Int))))\\n    S(Return(Cast(Long Var(\\\"@1.p\\\" Pointer(Int)) Long)))\\n  ]) FunType([] Long) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Long)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("c99_to_tac: test_c99_to_tac.py::TestIncrementDecrementLowering::test_postfix_on_int_pointer_var_scales_by_pointee_size (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass c99_to_tac\n"
      "source tests/test_c99_to_tac.py::TestIncrementDecrementLowering::test_postfix_on_int_pointer_var_scales_by_pointee_size\n"
      "args {\"args\": [{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Long)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"args\": {\"0\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Long)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%3\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%4\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Constant(ConstInt(0) Int) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.p\" AddressOf(Var(\"@0.a\" Int) Pointer(Int)) Pointer(Int) None None None None false)))\n"
      "    S(Expression(Postfix(Increment Var(\"@1.p\" Pointer(Int)) Pointer(Int))))\n"
      "    S(Return(Cast(Long Var(\"@1.p\" Pointer(Int)) Long)))\n"
      "  ]) FunType([] Long) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Copy(Constant(ConstInt(0)) Var(\"@0.a\"))\n"
      "    GetAddress(Var(\"@0.a\") Var(\"%0\"))\n"
      "    Copy(Var(\"%0\") Var(\"@1.p\"))\n"
      "    Copy(Var(\"@1.p\") Var(\"%1\"))\n"
      "    Binary(Multiply Constant(ConstInt(1)) Constant(ConstInt(2)) Var(\"%2\"))\n"
      "    Binary(Add Var(\"@1.p\") Var(\"%2\") Var(\"%3\"))\n"
      "    Copy(Var(\"%3\") Var(\"@1.p\"))\n"
      "    ZeroExtend(Var(\"@1.p\") Var(\"%4\"))\n"
      "    Ret(Var(\"%4\"))\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("type_checking: test_c99_to_tac.py::TestIncrementDecrementLowering::test_postfix_on_pointer_to_array_scales_by_array_size (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c99_to_tac.py::TestIncrementDecrementLowering::test_postfix_on_pointer_to_array_scales_by_array_size\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" None Array(Char 10) None None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.p\\\" AddressOf(Var(\\\"@0.a\\\" Array(Char 10)) Pointer(Array(Char 10))) Pointer(Array(Char 10)) None None None None false)))\\n    S(Expression(Postfix(Increment Var(\\\"@1.p\\\" Pointer(Array(Char 10))) Pointer(Array(Char 10)))))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" None Array(Char 10) None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.p\" AddressOf(Var(\"@0.a\" None) None) Pointer(Array(Char 10)) None None None None false)))\n"
      "    S(Expression(Postfix(Increment Var(\"@1.p\" None) None)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" None Array(Char 10) None None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.p\\\" AddressOf(Var(\\\"@0.a\\\" Array(Char 10)) Pointer(Array(Char 10))) Pointer(Array(Char 10)) None None None None false)))\\n    S(Expression(Postfix(Increment Var(\\\"@1.p\\\" Pointer(Array(Char 10))) Pointer(Array(Char 10)))))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Char 10)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Array(Char 10))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("c99_to_tac: test_c99_to_tac.py::TestIncrementDecrementLowering::test_postfix_on_pointer_to_array_scales_by_array_size (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass c99_to_tac\n"
      "source tests/test_c99_to_tac.py::TestIncrementDecrementLowering::test_postfix_on_pointer_to_array_scales_by_array_size\n"
      "args {\"args\": [{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Char 10)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Array(Char 10))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"args\": {\"0\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Char 10)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Array(Char 10))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Array(Char 10))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Array(Char 10))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%3\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Array(Char 10))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" None Array(Char 10) None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.p\" AddressOf(Var(\"@0.a\" Array(Char 10)) Pointer(Array(Char 10))) Pointer(Array(Char 10)) None None None None false)))\n"
      "    S(Expression(Postfix(Increment Var(\"@1.p\" Pointer(Array(Char 10))) Pointer(Array(Char 10)))))\n"
      "    S(Return(Constant(ConstInt(0) Int)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    GetAddress(Var(\"@0.a\") Var(\"%0\"))\n"
      "    Copy(Var(\"%0\") Var(\"@1.p\"))\n"
      "    Copy(Var(\"@1.p\") Var(\"%1\"))\n"
      "    Binary(Multiply Constant(ConstInt(1)) Constant(ConstInt(10)) Var(\"%2\"))\n"
      "    Binary(Add Var(\"@1.p\") Var(\"%2\") Var(\"%3\"))\n"
      "    Copy(Var(\"%3\") Var(\"@1.p\"))\n"
      "    Ret(Constant(ConstInt(0)))\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("type_checking: test_c99_to_tac.py::TestIncrementDecrementLowering::test_postfix_on_subscript_loads_and_stores_through_one_address (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c99_to_tac.py::TestIncrementDecrementLowering::test_postfix_on_subscript_loads_and_stores_through_one_address\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" None Array(Int 10) None None None None false)))\\n    S(Expression(Postfix(Increment Subscript(AddressOf(Var(\\\"@0.a\\\" Array(Int 10)) Pointer(Int)) Constant(ConstInt(3) Int) Int) Int)))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" None Array(Int 10) None None None None false)))\n"
      "    S(Expression(Postfix(Increment Subscript(Var(\"@0.a\" None) Constant(ConstInt(3) None) None) None)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" None Array(Int 10) None None None None false)))\\n    S(Expression(Postfix(Increment Subscript(AddressOf(Var(\\\"@0.a\\\" Array(Int 10)) Pointer(Int)) Constant(ConstInt(3) Int) Int) Int)))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Int 10)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("c99_to_tac: test_c99_to_tac.py::TestIncrementDecrementLowering::test_postfix_on_subscript_loads_and_stores_through_one_address (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass c99_to_tac\n"
      "source tests/test_c99_to_tac.py::TestIncrementDecrementLowering::test_postfix_on_subscript_loads_and_stores_through_one_address\n"
      "args {\"args\": [{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Int 10)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"args\": {\"0\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Int 10)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%3\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%4\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" None Array(Int 10) None None None None false)))\n"
      "    S(Expression(Postfix(Increment Subscript(AddressOf(Var(\"@0.a\" Array(Int 10)) Pointer(Int)) Constant(ConstInt(3) Int) Int) Int)))\n"
      "    S(Return(Constant(ConstInt(0) Int)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    GetAddress(Var(\"@0.a\") Var(\"%0\"))\n"
      "    Binary(Multiply Constant(ConstInt(3)) Constant(ConstInt(2)) Var(\"%1\"))\n"
      "    Binary(Add Var(\"%0\") Var(\"%1\") Var(\"%2\"))\n"
      "    Load(Var(\"%2\") Var(\"%3\") false)\n"
      "    Binary(Add Var(\"%3\") Constant(ConstInt(1)) Var(\"%4\"))\n"
      "    Store(Var(\"%4\") Var(\"%2\") false)\n"
      "    Ret(Constant(ConstInt(0)))\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("type_checking: test_c99_to_tac.py::TestIncrementDecrementLowering::test_postfix_on_var_returns_old_value (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c99_to_tac.py::TestIncrementDecrementLowering::test_postfix_on_var_returns_old_value\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" Constant(ConstInt(0) Int) Int None None None None false)))\\n    S(Expression(Postfix(Increment Var(\\\"@0.a\\\" Int) Int)))\\n    S(Return(Var(\\\"@0.a\\\" Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    S(Expression(Postfix(Increment Var(\"@0.a\" None) None)))\n"
      "    S(Return(Var(\"@0.a\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" Constant(ConstInt(0) Int) Int None None None None false)))\\n    S(Expression(Postfix(Increment Var(\\\"@0.a\\\" Int) Int)))\\n    S(Return(Var(\\\"@0.a\\\" Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("c99_to_tac: test_c99_to_tac.py::TestIncrementDecrementLowering::test_postfix_on_var_returns_old_value (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass c99_to_tac\n"
      "source tests/test_c99_to_tac.py::TestIncrementDecrementLowering::test_postfix_on_var_returns_old_value\n"
      "args {\"args\": [{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"args\": {\"0\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Constant(ConstInt(0) Int) Int None None None None false)))\n"
      "    S(Expression(Postfix(Increment Var(\"@0.a\" Int) Int)))\n"
      "    S(Return(Var(\"@0.a\" Int)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Copy(Constant(ConstInt(0)) Var(\"@0.a\"))\n"
      "    Copy(Var(\"@0.a\") Var(\"%0\"))\n"
      "    Binary(Add Var(\"@0.a\") Constant(ConstInt(1)) Var(\"%1\"))\n"
      "    Copy(Var(\"%1\") Var(\"@0.a\"))\n"
      "    Ret(Var(\"@0.a\"))\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("type_checking: test_c99_to_tac.py::TestIncrementDecrementLowering::test_prefix_on_pointer_var_emits_pointer_arithmetic (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c99_to_tac.py::TestIncrementDecrementLowering::test_prefix_on_pointer_var_emits_pointer_arithmetic\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" Constant(ConstInt(0) Int) Int None None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.p\\\" AddressOf(Var(\\\"@0.a\\\" Int) Pointer(Int)) Pointer(Int) None None None None false)))\\n    S(Expression(Prefix(Increment Var(\\\"@1.p\\\" Pointer(Int)) Pointer(Int))))\\n    S(Return(Cast(Long Var(\\\"@1.p\\\" Pointer(Int)) Long)))\\n  ]) FunType([] Long) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.p\" AddressOf(Var(\"@0.a\" None) None) Pointer(Int) None None None None false)))\n"
      "    S(Expression(Prefix(Increment Var(\"@1.p\" None) None)))\n"
      "    S(Return(Cast(Long Var(\"@1.p\" None) None)))\n"
      "  ]) FunType([] Long) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" Constant(ConstInt(0) Int) Int None None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.p\\\" AddressOf(Var(\\\"@0.a\\\" Int) Pointer(Int)) Pointer(Int) None None None None false)))\\n    S(Expression(Prefix(Increment Var(\\\"@1.p\\\" Pointer(Int)) Pointer(Int))))\\n    S(Return(Cast(Long Var(\\\"@1.p\\\" Pointer(Int)) Long)))\\n  ]) FunType([] Long) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Long)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("c99_to_tac: test_c99_to_tac.py::TestIncrementDecrementLowering::test_prefix_on_pointer_var_emits_pointer_arithmetic (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass c99_to_tac\n"
      "source tests/test_c99_to_tac.py::TestIncrementDecrementLowering::test_prefix_on_pointer_var_emits_pointer_arithmetic\n"
      "args {\"args\": [{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Long)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"args\": {\"0\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Long)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%3\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Constant(ConstInt(0) Int) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.p\" AddressOf(Var(\"@0.a\" Int) Pointer(Int)) Pointer(Int) None None None None false)))\n"
      "    S(Expression(Prefix(Increment Var(\"@1.p\" Pointer(Int)) Pointer(Int))))\n"
      "    S(Return(Cast(Long Var(\"@1.p\" Pointer(Int)) Long)))\n"
      "  ]) FunType([] Long) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Copy(Constant(ConstInt(0)) Var(\"@0.a\"))\n"
      "    GetAddress(Var(\"@0.a\") Var(\"%0\"))\n"
      "    Copy(Var(\"%0\") Var(\"@1.p\"))\n"
      "    Binary(Multiply Constant(ConstInt(1)) Constant(ConstInt(2)) Var(\"%1\"))\n"
      "    Binary(Add Var(\"@1.p\") Var(\"%1\") Var(\"%2\"))\n"
      "    Copy(Var(\"%2\") Var(\"@1.p\"))\n"
      "    ZeroExtend(Var(\"@1.p\") Var(\"%3\"))\n"
      "    Ret(Var(\"%3\"))\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("type_checking: test_c99_to_tac.py::TestIncrementDecrementLowering::test_prefix_on_subscript_with_side_effecting_index_fires_once (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c99_to_tac.py::TestIncrementDecrementLowering::test_prefix_on_subscript_with_side_effecting_index_fires_once\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" None Array(Int 10) None None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.i\\\" Constant(ConstInt(0) Int) Int None None None None false)))\\n    S(Expression(Prefix(Increment Subscript(AddressOf(Var(\\\"@0.a\\\" Array(Int 10)) Pointer(Int)) Postfix(Increment Var(\\\"@1.i\\\" Int) Int) Int) Int)))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" None Array(Int 10) None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.i\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    S(Expression(Prefix(Increment Subscript(Var(\"@0.a\" None) Postfix(Increment Var(\"@1.i\" None) None) None) None)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" None Array(Int 10) None None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.i\\\" Constant(ConstInt(0) Int) Int None None None None false)))\\n    S(Expression(Prefix(Increment Subscript(AddressOf(Var(\\\"@0.a\\\" Array(Int 10)) Pointer(Int)) Postfix(Increment Var(\\\"@1.i\\\" Int) Int) Int) Int)))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Int 10)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.i\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("c99_to_tac: test_c99_to_tac.py::TestIncrementDecrementLowering::test_prefix_on_subscript_with_side_effecting_index_fires_once (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass c99_to_tac\n"
      "source tests/test_c99_to_tac.py::TestIncrementDecrementLowering::test_prefix_on_subscript_with_side_effecting_index_fires_once\n"
      "args {\"args\": [{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Int 10)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.i\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"args\": {\"0\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Int 10)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.i\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%3\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%4\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%5\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%6\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%7\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%8\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" None Array(Int 10) None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.i\" Constant(ConstInt(0) Int) Int None None None None false)))\n"
      "    S(Expression(Prefix(Increment Subscript(AddressOf(Var(\"@0.a\" Array(Int 10)) Pointer(Int)) Postfix(Increment Var(\"@1.i\" Int) Int) Int) Int)))\n"
      "    S(Return(Constant(ConstInt(0) Int)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Copy(Constant(ConstInt(0)) Var(\"@1.i\"))\n"
      "    GetAddress(Var(\"@0.a\") Var(\"%0\"))\n"
      "    Copy(Var(\"@1.i\") Var(\"%1\"))\n"
      "    Binary(Add Var(\"@1.i\") Constant(ConstInt(1)) Var(\"%2\"))\n"
      "    Copy(Var(\"%2\") Var(\"@1.i\"))\n"
      "    Truncate(Var(\"%1\") Var(\"%3\"))\n"
      "    Binary(Multiply Var(\"%3\") Constant(ConstUChar(2)) Var(\"%4\"))\n"
      "    ZeroExtend(Var(\"%4\") Var(\"%5\"))\n"
      "    Binary(Add Var(\"%0\") Var(\"%5\") Var(\"%6\"))\n"
      "    Load(Var(\"%6\") Var(\"%7\") false)\n"
      "    Binary(Add Var(\"%7\") Constant(ConstInt(1)) Var(\"%8\"))\n"
      "    Store(Var(\"%8\") Var(\"%6\") false)\n"
      "    Ret(Constant(ConstInt(0)))\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("type_checking: test_c99_to_tac.py::TestIncrementDecrementLowering::test_prefix_on_var_returns_new_value (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c99_to_tac.py::TestIncrementDecrementLowering::test_prefix_on_var_returns_new_value\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" Constant(ConstInt(0) Int) Int None None None None false)))\\n    S(Expression(Prefix(Increment Var(\\\"@0.a\\\" Int) Int)))\\n    S(Return(Var(\\\"@0.a\\\" Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    S(Expression(Prefix(Increment Var(\"@0.a\" None) None)))\n"
      "    S(Return(Var(\"@0.a\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" Constant(ConstInt(0) Int) Int None None None None false)))\\n    S(Expression(Prefix(Increment Var(\\\"@0.a\\\" Int) Int)))\\n    S(Return(Var(\\\"@0.a\\\" Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("c99_to_tac: test_c99_to_tac.py::TestIncrementDecrementLowering::test_prefix_on_var_returns_new_value (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass c99_to_tac\n"
      "source tests/test_c99_to_tac.py::TestIncrementDecrementLowering::test_prefix_on_var_returns_new_value\n"
      "args {\"args\": [{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"args\": {\"0\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Constant(ConstInt(0) Int) Int None None None None false)))\n"
      "    S(Expression(Prefix(Increment Var(\"@0.a\" Int) Int)))\n"
      "    S(Return(Var(\"@0.a\" Int)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Copy(Constant(ConstInt(0)) Var(\"@0.a\"))\n"
      "    Binary(Add Var(\"@0.a\") Constant(ConstInt(1)) Var(\"%0\"))\n"
      "    Copy(Var(\"%0\") Var(\"@0.a\"))\n"
      "    Ret(Var(\"@0.a\"))\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("type_checking: test_c99_to_tac.py::TestCompoundAssignmentLowering::test_compound_on_dereference_evaluates_pointer_once (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c99_to_tac.py::TestCompoundAssignmentLowering::test_compound_on_dereference_evaluates_pointer_once\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" None Int None None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.p\\\" AddressOf(Var(\\\"@0.a\\\" Int) Pointer(Int)) Pointer(Int) None None None None false)))\\n    S(Expression(CompoundAssignment(Add Dereference(Var(\\\"@1.p\\\" Pointer(Int)) Int) Constant(ConstInt(1) Int) Int Int)))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" None Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.p\" AddressOf(Var(\"@0.a\" None) None) Pointer(Int) None None None None false)))\n"
      "    S(Expression(CompoundAssignment(Add Dereference(Var(\"@1.p\" None) None) Constant(ConstInt(1) None) None None)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" None Int None None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.p\\\" AddressOf(Var(\\\"@0.a\\\" Int) Pointer(Int)) Pointer(Int) None None None None false)))\\n    S(Expression(CompoundAssignment(Add Dereference(Var(\\\"@1.p\\\" Pointer(Int)) Int) Constant(ConstInt(1) Int) Int Int)))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("c99_to_tac: test_c99_to_tac.py::TestCompoundAssignmentLowering::test_compound_on_dereference_evaluates_pointer_once (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass c99_to_tac\n"
      "source tests/test_c99_to_tac.py::TestCompoundAssignmentLowering::test_compound_on_dereference_evaluates_pointer_once\n"
      "args {\"args\": [{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"args\": {\"0\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" None Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.p\" AddressOf(Var(\"@0.a\" Int) Pointer(Int)) Pointer(Int) None None None None false)))\n"
      "    S(Expression(CompoundAssignment(Add Dereference(Var(\"@1.p\" Pointer(Int)) Int) Constant(ConstInt(1) Int) Int Int)))\n"
      "    S(Return(Constant(ConstInt(0) Int)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    GetAddress(Var(\"@0.a\") Var(\"%0\"))\n"
      "    Copy(Var(\"%0\") Var(\"@1.p\"))\n"
      "    Load(Var(\"@1.p\") Var(\"%1\") false)\n"
      "    Binary(Add Var(\"%1\") Constant(ConstInt(1)) Var(\"%2\"))\n"
      "    Store(Var(\"%2\") Var(\"@1.p\") false)\n"
      "    Ret(Constant(ConstInt(0)))\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("type_checking: test_c99_to_tac.py::TestCompoundAssignmentLowering::test_compound_on_subscript_one_load_for_the_compound (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c99_to_tac.py::TestCompoundAssignmentLowering::test_compound_on_subscript_one_load_for_the_compound\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.arr\\\" InitList([Constant(ConstInt(1) Int) Constant(ConstInt(2) Int) Constant(ConstInt(3) Int)] Array(Int 3) None) Array(Int 3) None None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.i\\\" Constant(ConstInt(0) Int) Int None None None None false)))\\n    S(Expression(CompoundAssignment(Add Subscript(AddressOf(Var(\\\"@0.arr\\\" Array(Int 3)) Pointer(Int)) Var(\\\"@1.i\\\" Int) Int) Constant(ConstInt(1) Int) Int Int)))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.arr\" InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None) Constant(ConstInt(3) None)] None None) Array(Int 3) None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.i\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    S(Expression(CompoundAssignment(Add Subscript(Var(\"@0.arr\" None) Var(\"@1.i\" None) None) Constant(ConstInt(1) None) None None)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.arr\\\" InitList([Constant(ConstInt(1) Int) Constant(ConstInt(2) Int) Constant(ConstInt(3) Int)] Array(Int 3) None) Array(Int 3) None None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.i\\\" Constant(ConstInt(0) Int) Int None None None None false)))\\n    S(Expression(CompoundAssignment(Add Subscript(AddressOf(Var(\\\"@0.arr\\\" Array(Int 3)) Pointer(Int)) Var(\\\"@1.i\\\" Int) Int) Constant(ConstInt(1) Int) Int Int)))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.arr\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Int 3)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.i\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("c99_to_tac: test_c99_to_tac.py::TestCompoundAssignmentLowering::test_compound_on_subscript_one_load_for_the_compound (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass c99_to_tac\n"
      "source tests/test_c99_to_tac.py::TestCompoundAssignmentLowering::test_compound_on_subscript_one_load_for_the_compound\n"
      "args {\"args\": [{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.arr\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Int 3)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.i\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"args\": {\"0\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.arr\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Int 3)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.i\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%3\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%4\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%5\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%6\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%7\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%8\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%9\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.arr\" InitList([Constant(ConstInt(1) Int) Constant(ConstInt(2) Int) Constant(ConstInt(3) Int)] Array(Int 3) None) Array(Int 3) None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.i\" Constant(ConstInt(0) Int) Int None None None None false)))\n"
      "    S(Expression(CompoundAssignment(Add Subscript(AddressOf(Var(\"@0.arr\" Array(Int 3)) Pointer(Int)) Var(\"@1.i\" Int) Int) Constant(ConstInt(1) Int) Int Int)))\n"
      "    S(Return(Constant(ConstInt(0) Int)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    GetAddress(Var(\"@0.arr\") Var(\"%0\"))\n"
      "    Store(Constant(ConstInt(1)) Var(\"%0\") false)\n"
      "    Binary(Add Var(\"%0\") Constant(ConstInt(2)) Var(\"%1\"))\n"
      "    Store(Constant(ConstInt(2)) Var(\"%1\") false)\n"
      "    Binary(Add Var(\"%0\") Constant(ConstInt(4)) Var(\"%2\"))\n"
      "    Store(Constant(ConstInt(3)) Var(\"%2\") false)\n"
      "    Copy(Constant(ConstInt(0)) Var(\"@1.i\"))\n"
      "    GetAddress(Var(\"@0.arr\") Var(\"%3\"))\n"
      "    Truncate(Var(\"@1.i\") Var(\"%4\"))\n"
      "    Binary(Multiply Var(\"%4\") Constant(ConstUChar(2)) Var(\"%5\"))\n"
      "    ZeroExtend(Var(\"%5\") Var(\"%6\"))\n"
      "    Binary(Add Var(\"%3\") Var(\"%6\") Var(\"%7\"))\n"
      "    Load(Var(\"%7\") Var(\"%8\") false)\n"
      "    Binary(Add Var(\"%8\") Constant(ConstInt(1)) Var(\"%9\"))\n"
      "    Store(Var(\"%9\") Var(\"%7\") false)\n"
      "    Ret(Constant(ConstInt(0)))\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("type_checking: test_c99_to_tac.py::TestCompoundAssignmentLowering::test_compound_on_subscript_with_side_effecting_index_fires_once (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c99_to_tac.py::TestCompoundAssignmentLowering::test_compound_on_subscript_with_side_effecting_index_fires_once\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.arr\\\" InitList([Constant(ConstInt(1) Int) Constant(ConstInt(2) Int) Constant(ConstInt(3) Int)] Array(Int 3) None) Array(Int 3) None None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.i\\\" Constant(ConstInt(0) Int) Int None None None None false)))\\n    S(Expression(CompoundAssignment(Add Subscript(AddressOf(Var(\\\"@0.arr\\\" Array(Int 3)) Pointer(Int)) Postfix(Increment Var(\\\"@1.i\\\" Int) Int) Int) Constant(ConstInt(1) Int) Int Int)))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.arr\" InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None) Constant(ConstInt(3) None)] None None) Array(Int 3) None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.i\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    S(Expression(CompoundAssignment(Add Subscript(Var(\"@0.arr\" None) Postfix(Increment Var(\"@1.i\" None) None) None) Constant(ConstInt(1) None) None None)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.arr\\\" InitList([Constant(ConstInt(1) Int) Constant(ConstInt(2) Int) Constant(ConstInt(3) Int)] Array(Int 3) None) Array(Int 3) None None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.i\\\" Constant(ConstInt(0) Int) Int None None None None false)))\\n    S(Expression(CompoundAssignment(Add Subscript(AddressOf(Var(\\\"@0.arr\\\" Array(Int 3)) Pointer(Int)) Postfix(Increment Var(\\\"@1.i\\\" Int) Int) Int) Constant(ConstInt(1) Int) Int Int)))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.arr\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Int 3)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.i\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("c99_to_tac: test_c99_to_tac.py::TestCompoundAssignmentLowering::test_compound_on_subscript_with_side_effecting_index_fires_once (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass c99_to_tac\n"
      "source tests/test_c99_to_tac.py::TestCompoundAssignmentLowering::test_compound_on_subscript_with_side_effecting_index_fires_once\n"
      "args {\"args\": [{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.arr\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Int 3)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.i\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"args\": {\"0\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.arr\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Int 3)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.i\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%3\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%4\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%5\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%6\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%7\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%8\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%9\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%10\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%11\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\""
      "register\": false, \"was_bool\": false}}}}]]}}}}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.arr\" InitList([Constant(ConstInt(1) Int) Constant(ConstInt(2) Int) Constant(ConstInt(3) Int)] Array(Int 3) None) Array(Int 3) None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.i\" Constant(ConstInt(0) Int) Int None None None None false)))\n"
      "    S(Expression(CompoundAssignment(Add Subscript(AddressOf(Var(\"@0.arr\" Array(Int 3)) Pointer(Int)) Postfix(Increment Var(\"@1.i\" Int) Int) Int) Constant(ConstInt(1) Int) Int Int)))\n"
      "    S(Return(Constant(ConstInt(0) Int)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    GetAddress(Var(\"@0.arr\") Var(\"%0\"))\n"
      "    Store(Constant(ConstInt(1)) Var(\"%0\") false)\n"
      "    Binary(Add Var(\"%0\") Constant(ConstInt(2)) Var(\"%1\"))\n"
      "    Store(Constant(ConstInt(2)) Var(\"%1\") false)\n"
      "    Binary(Add Var(\"%0\") Constant(ConstInt(4)) Var(\"%2\"))\n"
      "    Store(Constant(ConstInt(3)) Var(\"%2\") false)\n"
      "    Copy(Constant(ConstInt(0)) Var(\"@1.i\"))\n"
      "    GetAddress(Var(\"@0.arr\") Var(\"%3\"))\n"
      "    Copy(Var(\"@1.i\") Var(\"%4\"))\n"
      "    Binary(Add Var(\"@1.i\") Constant(ConstInt(1)) Var(\"%5\"))\n"
      "    Copy(Var(\"%5\") Var(\"@1.i\"))\n"
      "    Truncate(Var(\"%4\") Var(\"%6\"))\n"
      "    Binary(Multiply Var(\"%6\") Constant(ConstUChar(2)) Var(\"%7\"))\n"
      "    ZeroExtend(Var(\"%7\") Var(\"%8\"))\n"
      "    Binary(Add Var(\"%3\") Var(\"%8\") Var(\"%9\"))\n"
      "    Load(Var(\"%9\") Var(\"%10\") false)\n"
      "    Binary(Add Var(\"%10\") Constant(ConstInt(1)) Var(\"%11\"))\n"
      "    Store(Var(\"%11\") Var(\"%9\") false)\n"
      "    Ret(Constant(ConstInt(0)))\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("type_checking: test_c99_to_tac.py::TestCompoundAssignmentLowering::test_compound_on_var_emits_no_load (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c99_to_tac.py::TestCompoundAssignmentLowering::test_compound_on_var_emits_no_load\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" Constant(ConstInt(0) Int) Int None None None None false)))\\n    S(Expression(CompoundAssignment(Add Var(\\\"@0.a\\\" Int) Constant(ConstInt(1) Int) Int Int)))\\n    S(Return(Var(\\\"@0.a\\\" Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    S(Expression(CompoundAssignment(Add Var(\"@0.a\" None) Constant(ConstInt(1) None) None None)))\n"
      "    S(Return(Var(\"@0.a\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" Constant(ConstInt(0) Int) Int None None None None false)))\\n    S(Expression(CompoundAssignment(Add Var(\\\"@0.a\\\" Int) Constant(ConstInt(1) Int) Int Int)))\\n    S(Return(Var(\\\"@0.a\\\" Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("c99_to_tac: test_c99_to_tac.py::TestCompoundAssignmentLowering::test_compound_on_var_emits_no_load (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass c99_to_tac\n"
      "source tests/test_c99_to_tac.py::TestCompoundAssignmentLowering::test_compound_on_var_emits_no_load\n"
      "args {\"args\": [{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"args\": {\"0\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Constant(ConstInt(0) Int) Int None None None None false)))\n"
      "    S(Expression(CompoundAssignment(Add Var(\"@0.a\" Int) Constant(ConstInt(1) Int) Int Int)))\n"
      "    S(Return(Var(\"@0.a\" Int)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Copy(Constant(ConstInt(0)) Var(\"@0.a\"))\n"
      "    Binary(Add Var(\"@0.a\") Constant(ConstInt(1)) Var(\"%0\"))\n"
      "    Copy(Var(\"%0\") Var(\"@0.a\"))\n"
      "    Ret(Var(\"@0.a\"))\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("type_checking: test_c99_to_tac.py::TestArrayInitList::test_full_init_emits_n_stores (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c99_to_tac.py::TestArrayInitList::test_full_init_emits_n_stores\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" InitList([Constant(ConstInt(1) Int) Constant(ConstInt(2) Int) Constant(ConstInt(3) Int)] Array(Int 3) None) Array(Int 3) None None None None false)))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None) Constant(ConstInt(3) None)] None None) Array(Int 3) None None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" InitList([Constant(ConstInt(1) Int) Constant(ConstInt(2) Int) Constant(ConstInt(3) Int)] Array(Int 3) None) Array(Int 3) None None None None false)))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Int 3)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("c99_to_tac: test_c99_to_tac.py::TestArrayInitList::test_full_init_emits_n_stores (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass c99_to_tac\n"
      "source tests/test_c99_to_tac.py::TestArrayInitList::test_full_init_emits_n_stores\n"
      "args {\"args\": [{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Int 3)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"args\": {\"0\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Int 3)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" InitList([Constant(ConstInt(1) Int) Constant(ConstInt(2) Int) Constant(ConstInt(3) Int)] Array(Int 3) None) Array(Int 3) None None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) Int)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    GetAddress(Var(\"@0.a\") Var(\"%0\"))\n"
      "    Store(Constant(ConstInt(1)) Var(\"%0\") false)\n"
      "    Binary(Add Var(\"%0\") Constant(ConstInt(2)) Var(\"%1\"))\n"
      "    Store(Constant(ConstInt(2)) Var(\"%1\") false)\n"
      "    Binary(Add Var(\"%0\") Constant(ConstInt(4)) Var(\"%2\"))\n"
      "    Store(Constant(ConstInt(3)) Var(\"%2\") false)\n"
      "    Ret(Constant(ConstInt(0)))\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("type_checking: test_c99_to_tac.py::TestArrayInitList::test_init_emits_get_address (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c99_to_tac.py::TestArrayInitList::test_init_emits_get_address\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" InitList([Constant(ConstInt(1) Int) Constant(ConstInt(2) Int) Constant(ConstInt(3) Int)] Array(Int 3) None) Array(Int 3) None None None None false)))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None) Constant(ConstInt(3) None)] None None) Array(Int 3) None None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" InitList([Constant(ConstInt(1) Int) Constant(ConstInt(2) Int) Constant(ConstInt(3) Int)] Array(Int 3) None) Array(Int 3) None None None None false)))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Int 3)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("c99_to_tac: test_c99_to_tac.py::TestArrayInitList::test_init_emits_get_address (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass c99_to_tac\n"
      "source tests/test_c99_to_tac.py::TestArrayInitList::test_init_emits_get_address\n"
      "args {\"args\": [{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Int 3)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"args\": {\"0\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Int 3)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" InitList([Constant(ConstInt(1) Int) Constant(ConstInt(2) Int) Constant(ConstInt(3) Int)] Array(Int 3) None) Array(Int 3) None None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) Int)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    GetAddress(Var(\"@0.a\") Var(\"%0\"))\n"
      "    Store(Constant(ConstInt(1)) Var(\"%0\") false)\n"
      "    Binary(Add Var(\"%0\") Constant(ConstInt(2)) Var(\"%1\"))\n"
      "    Store(Constant(ConstInt(2)) Var(\"%1\") false)\n"
      "    Binary(Add Var(\"%0\") Constant(ConstInt(4)) Var(\"%2\"))\n"
      "    Store(Constant(ConstInt(3)) Var(\"%2\") false)\n"
      "    Ret(Constant(ConstInt(0)))\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("type_checking: test_c99_to_tac.py::TestArrayInitList::test_long_array_init_uses_long_offsets (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c99_to_tac.py::TestArrayInitList::test_long_array_init_uses_long_offsets\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" InitList([Constant(ConstLong(1) Long) Constant(ConstLong(2) Long) Constant(ConstLong(3) Long)] Array(Long 3) None) Array(Long 3) None None None None false)))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" InitList([Constant(ConstLong(1) None) Constant(ConstLong(2) None) Constant(ConstLong(3) None)] None None) Array(Long 3) None None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" InitList([Constant(ConstLong(1) Long) Constant(ConstLong(2) Long) Constant(ConstLong(3) Long)] Array(Long 3) None) Array(Long 3) None None None None false)))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Long 3)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("c99_to_tac: test_c99_to_tac.py::TestArrayInitList::test_long_array_init_uses_long_offsets (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass c99_to_tac\n"
      "source tests/test_c99_to_tac.py::TestArrayInitList::test_long_array_init_uses_long_offsets\n"
      "args {\"args\": [{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Long 3)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"args\": {\"0\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Long 3)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Long)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Long)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" InitList([Constant(ConstLong(1) Long) Constant(ConstLong(2) Long) Constant(ConstLong(3) Long)] Array(Long 3) None) Array(Long 3) None None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) Int)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    GetAddress(Var(\"@0.a\") Var(\"%0\"))\n"
      "    Store(Constant(ConstLong(1)) Var(\"%0\") false)\n"
      "    Binary(Add Var(\"%0\") Constant(ConstInt(4)) Var(\"%1\"))\n"
      "    Store(Constant(ConstLong(2)) Var(\"%1\") false)\n"
      "    Binary(Add Var(\"%0\") Constant(ConstInt(8)) Var(\"%2\"))\n"
      "    Store(Constant(ConstLong(3)) Var(\"%2\") false)\n"
      "    Ret(Constant(ConstInt(0)))\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("type_checking: test_c99_to_tac.py::TestArrayInitList::test_partial_init_pads_with_zeros (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c99_to_tac.py::TestArrayInitList::test_partial_init_pads_with_zeros\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" InitList([Constant(ConstInt(1) Int) Constant(ConstInt(2) Int)] Array(Int 5) None) Array(Int 5) None None None None false)))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) Array(Int 5) None None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" InitList([Constant(ConstInt(1) Int) Constant(ConstInt(2) Int)] Array(Int 5) None) Array(Int 5) None None None None false)))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Int 5)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("c99_to_tac: test_c99_to_tac.py::TestArrayInitList::test_partial_init_pads_with_zeros (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass c99_to_tac\n"
      "source tests/test_c99_to_tac.py::TestArrayInitList::test_partial_init_pads_with_zeros\n"
      "args {\"args\": [{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Int 5)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"args\": {\"0\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Int 5)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(ULong)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%3\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UInt)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" InitList([Constant(ConstInt(1) Int) Constant(ConstInt(2) Int)] Array(Int 5) None) Array(Int 5) None None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) Int)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    GetAddress(Var(\"@0.a\") Var(\"%0\"))\n"
      "    Store(Constant(ConstInt(1)) Var(\"%0\") false)\n"
      "    Binary(Add Var(\"%0\") Constant(ConstInt(2)) Var(\"%1\"))\n"
      "    Store(Constant(ConstInt(2)) Var(\"%1\") false)\n"
      "    Binary(Add Var(\"%0\") Constant(ConstInt(4)) Var(\"%2\"))\n"
      "    Store(Constant(ConstULong(0)) Var(\"%2\") false)\n"
      "    Binary(Add Var(\"%0\") Constant(ConstInt(8)) Var(\"%3\"))\n"
      "    Store(Constant(ConstUInt(0)) Var(\"%3\") false)\n"
      "    Ret(Constant(ConstInt(0)))\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("type_checking: test_c99_to_tac.py::TestAddressOfSubscript::test_address_of_char_subscript_no_load (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c99_to_tac.py::TestAddressOfSubscript::test_address_of_char_subscript_no_load\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.x\\\" Cast(Char Constant(ConstInt(0) Int) Char) Char None None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.p\\\" AddressOf(Var(\\\"@0.x\\\" Char) Pointer(Char)) Pointer(Char) None None None None false)))\\n    D(VarDecl(var_decl(\\\"@2.q\\\" AddressOf(Subscript(Var(\\\"@1.p\\\" Pointer(Char)) Constant(ConstInt(3) Int) Char) Pointer(Char)) Pointer(Char) None None None None false)))\\n    S(Return(Cast(Long Var(\\\"@2.q\\\" Pointer(Char)) Long)))\\n  ]) FunType([] Long) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.x\" Constant(ConstInt(0) None) Char None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.p\" AddressOf(Var(\"@0.x\" None) None) Pointer(Char) None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@2.q\" AddressOf(Subscript(Var(\"@1.p\" None) Constant(ConstInt(3) None) None) None) Pointer(Char) None None None None false)))\n"
      "    S(Return(Cast(Long Var(\"@2.q\" None) None)))\n"
      "  ]) FunType([] Long) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.x\\\" Cast(Char Constant(ConstInt(0) Int) Char) Char None None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.p\\\" AddressOf(Var(\\\"@0.x\\\" Char) Pointer(Char)) Pointer(Char) None None None None false)))\\n    D(VarDecl(var_decl(\\\"@2.q\\\" AddressOf(Subscript(Var(\\\"@1.p\\\" Pointer(Char)) Constant(ConstInt(3) Int) Char) Pointer(Char)) Pointer(Char) None None None None false)))\\n    S(Return(Cast(Long Var(\\\"@2.q\\\" Pointer(Char)) Long)))\\n  ]) FunType([] Long) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Long)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Char\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Char)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@2.q\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Char)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("c99_to_tac: test_c99_to_tac.py::TestAddressOfSubscript::test_address_of_char_subscript_no_load (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass c99_to_tac\n"
      "source tests/test_c99_to_tac.py::TestAddressOfSubscript::test_address_of_char_subscript_no_load\n"
      "args {\"args\": [{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Long)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Char\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Char)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@2.q\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Char)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"args\": {\"0\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Long)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Char\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Char)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@2.q\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Char)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Char\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Char)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Char)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%3\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.x\" Cast(Char Constant(ConstInt(0) Int) Char) Char None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.p\" AddressOf(Var(\"@0.x\" Char) Pointer(Char)) Pointer(Char) None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@2.q\" AddressOf(Subscript(Var(\"@1.p\" Pointer(Char)) Constant(ConstInt(3) Int) Char) Pointer(Char)) Pointer(Char) None None None None false)))\n"
      "    S(Return(Cast(Long Var(\"@2.q\" Pointer(Char)) Long)))\n"
      "  ]) FunType([] Long) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Truncate(Constant(ConstInt(0)) Var(\"%0\"))\n"
      "    Copy(Var(\"%0\") Var(\"@0.x\"))\n"
      "    GetAddress(Var(\"@0.x\") Var(\"%1\"))\n"
      "    Copy(Var(\"%1\") Var(\"@1.p\"))\n"
      "    Binary(Add Var(\"@1.p\") Constant(ConstInt(3)) Var(\"%2\"))\n"
      "    Copy(Var(\"%2\") Var(\"@2.q\"))\n"
      "    ZeroExtend(Var(\"@2.q\") Var(\"%3\"))\n"
      "    Ret(Var(\"%3\"))\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("type_checking: test_c99_to_tac.py::TestAddressOfSubscript::test_address_of_int_subscript_emits_scale (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c99_to_tac.py::TestAddressOfSubscript::test_address_of_int_subscript_emits_scale\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.x\\\" Constant(ConstInt(0) Int) Int None None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.p\\\" AddressOf(Var(\\\"@0.x\\\" Int) Pointer(Int)) Pointer(Int) None None None None false)))\\n    D(VarDecl(var_decl(\\\"@2.q\\\" AddressOf(Subscript(Var(\\\"@1.p\\\" Pointer(Int)) Constant(ConstInt(3) Int) Int) Pointer(Int)) Pointer(Int) None None None None false)))\\n    S(Return(Cast(Long Var(\\\"@2.q\\\" Pointer(Int)) Long)))\\n  ]) FunType([] Long) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.x\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.p\" AddressOf(Var(\"@0.x\" None) None) Pointer(Int) None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@2.q\" AddressOf(Subscript(Var(\"@1.p\" None) Constant(ConstInt(3) None) None) None) Pointer(Int) None None None None false)))\n"
      "    S(Return(Cast(Long Var(\"@2.q\" None) None)))\n"
      "  ]) FunType([] Long) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.x\\\" Constant(ConstInt(0) Int) Int None None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.p\\\" AddressOf(Var(\\\"@0.x\\\" Int) Pointer(Int)) Pointer(Int) None None None None false)))\\n    D(VarDecl(var_decl(\\\"@2.q\\\" AddressOf(Subscript(Var(\\\"@1.p\\\" Pointer(Int)) Constant(ConstInt(3) Int) Int) Pointer(Int)) Pointer(Int) None None None None false)))\\n    S(Return(Cast(Long Var(\\\"@2.q\\\" Pointer(Int)) Long)))\\n  ]) FunType([] Long) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Long)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@2.q\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("c99_to_tac: test_c99_to_tac.py::TestAddressOfSubscript::test_address_of_int_subscript_emits_scale (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass c99_to_tac\n"
      "source tests/test_c99_to_tac.py::TestAddressOfSubscript::test_address_of_int_subscript_emits_scale\n"
      "args {\"args\": [{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Long)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@2.q\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"args\": {\"0\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Long)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@2.q\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%3\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.x\" Constant(ConstInt(0) Int) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.p\" AddressOf(Var(\"@0.x\" Int) Pointer(Int)) Pointer(Int) None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@2.q\" AddressOf(Subscript(Var(\"@1.p\" Pointer(Int)) Constant(ConstInt(3) Int) Int) Pointer(Int)) Pointer(Int) None None None None false)))\n"
      "    S(Return(Cast(Long Var(\"@2.q\" Pointer(Int)) Long)))\n"
      "  ]) FunType([] Long) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Copy(Constant(ConstInt(0)) Var(\"@0.x\"))\n"
      "    GetAddress(Var(\"@0.x\") Var(\"%0\"))\n"
      "    Copy(Var(\"%0\") Var(\"@1.p\"))\n"
      "    Binary(Multiply Constant(ConstInt(3)) Constant(ConstInt(2)) Var(\"%1\"))\n"
      "    Binary(Add Var(\"@1.p\") Var(\"%1\") Var(\"%2\"))\n"
      "    Copy(Var(\"%2\") Var(\"@2.q\"))\n"
      "    ZeroExtend(Var(\"@2.q\") Var(\"%3\"))\n"
      "    Ret(Var(\"%3\"))\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("type_checking: test_c99_to_tac.py::TestAddressOfSubscript::test_address_of_subscript_matches_pointer_add (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c99_to_tac.py::TestAddressOfSubscript::test_address_of_subscript_matches_pointer_add\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.x\\\" Constant(ConstInt(0) Int) Int None None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.p\\\" AddressOf(Var(\\\"@0.x\\\" Int) Pointer(Int)) Pointer(Int) None None None None false)))\\n    D(VarDecl(var_decl(\\\"@2.q1\\\" AddressOf(Subscript(Var(\\\"@1.p\\\" Pointer(Int)) Constant(ConstInt(3) Int) Int) Pointer(Int)) Pointer(Int) None None None None false)))\\n    D(VarDecl(var_decl(\\\"@3.q2\\\" Binary(Add Var(\\\"@1.p\\\" Pointer(Int)) Constant(ConstInt(3) Int) Pointer(Int)) Pointer(Int) None None None None false)))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.x\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.p\" AddressOf(Var(\"@0.x\" None) None) Pointer(Int) None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@2.q1\" AddressOf(Subscript(Var(\"@1.p\" None) Constant(ConstInt(3) None) None) None) Pointer(Int) None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@3.q2\" Binary(Add Var(\"@1.p\" None) Constant(ConstInt(3) None) None) Pointer(Int) None None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.x\\\" Constant(ConstInt(0) Int) Int None None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.p\\\" AddressOf(Var(\\\"@0.x\\\" Int) Pointer(Int)) Pointer(Int) None None None None false)))\\n    D(VarDecl(var_decl(\\\"@2.q1\\\" AddressOf(Subscript(Var(\\\"@1.p\\\" Pointer(Int)) Constant(ConstInt(3) Int) Int) Pointer(Int)) Pointer(Int) None None None None false)))\\n    D(VarDecl(var_decl(\\\"@3.q2\\\" Binary(Add Var(\\\"@1.p\\\" Pointer(Int)) Constant(ConstInt(3) Int) Pointer(Int)) Pointer(Int) None None None None false)))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@2.q1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@3.q2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("c99_to_tac: test_c99_to_tac.py::TestAddressOfSubscript::test_address_of_subscript_matches_pointer_add (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass c99_to_tac\n"
      "source tests/test_c99_to_tac.py::TestAddressOfSubscript::test_address_of_subscript_matches_pointer_add\n"
      "args {\"args\": [{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@2.q1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@3.q2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"args\": {\"0\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@2.q1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@3.q2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%3\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%4\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.x\" Constant(ConstInt(0) Int) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.p\" AddressOf(Var(\"@0.x\" Int) Pointer(Int)) Pointer(Int) None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@2.q1\" AddressOf(Subscript(Var(\"@1.p\" Pointer(Int)) Constant(ConstInt(3) Int) Int) Pointer(Int)) Pointer(Int) None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@3.q2\" Binary(Add Var(\"@1.p\" Pointer(Int)) Constant(ConstInt(3) Int) Pointer(Int)) Pointer(Int) None None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) Int)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Copy(Constant(ConstInt(0)) Var(\"@0.x\"))\n"
      "    GetAddress(Var(\"@0.x\") Var(\"%0\"))\n"
      "    Copy(Var(\"%0\") Var(\"@1.p\"))\n"
      "    Binary(Multiply Constant(ConstInt(3)) Constant(ConstInt(2)) Var(\"%1\"))\n"
      "    Binary(Add Var(\"@1.p\") Var(\"%1\") Var(\"%2\"))\n"
      "    Copy(Var(\"%2\") Var(\"@2.q1\"))\n"
      "    Binary(Multiply Constant(ConstInt(3)) Constant(ConstInt(2)) Var(\"%3\"))\n"
      "    Binary(Add Var(\"@1.p\") Var(\"%3\") Var(\"%4\"))\n"
      "    Copy(Var(\"%4\") Var(\"@3.q2\"))\n"
      "    Ret(Constant(ConstInt(0)))\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("type_checking: test_c99_to_tac.py::TestMultiDimArrays::test_partial_two_dim_init_zero_pads (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c99_to_tac.py::TestMultiDimArrays::test_partial_two_dim_init_zero_pads\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" InitList([InitList([Constant(ConstInt(1) Int) Constant(ConstInt(2) Int)] Array(Int 3) None)] Array(Array(Int 3) 2) None) Array(Array(Int 3) 2) None None None None false)))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" InitList([InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None)] None None) Array(Array(Int 3) 2) None None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" InitList([InitList([Constant(ConstInt(1) Int) Constant(ConstInt(2) Int)] Array(Int 3) None)] Array(Array(Int 3) 2) None) Array(Array(Int 3) 2) None None None None false)))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Array(Int 3) 2)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("c99_to_tac: test_c99_to_tac.py::TestMultiDimArrays::test_partial_two_dim_init_zero_pads (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass c99_to_tac\n"
      "source tests/test_c99_to_tac.py::TestMultiDimArrays::test_partial_two_dim_init_zero_pads\n"
      "args {\"args\": [{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Array(Int 3) 2)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"args\": {\"0\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Array(Int 3) 2)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(ULongLong)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" InitList([InitList([Constant(ConstInt(1) Int) Constant(ConstInt(2) Int)] Array(Int 3) None)] Array(Array(Int 3) 2) None) Array(Array(Int 3) 2) None None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) Int)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    GetAddress(Var(\"@0.a\") Var(\"%0\"))\n"
      "    Store(Constant(ConstInt(1)) Var(\"%0\") false)\n"
      "    Binary(Add Var(\"%0\") Constant(ConstInt(2)) Var(\"%1\"))\n"
      "    Store(Constant(ConstInt(2)) Var(\"%1\") false)\n"
      "    Binary(Add Var(\"%0\") Constant(ConstInt(4)) Var(\"%2\"))\n"
      "    Store(Constant(ConstULongLong(0)) Var(\"%2\") false)\n"
      "    Ret(Constant(ConstInt(0)))\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("type_checking: test_c99_to_tac.py::TestMultiDimArrays::test_two_dim_init_emits_six_stores (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c99_to_tac.py::TestMultiDimArrays::test_two_dim_init_emits_six_stores\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" InitList([InitList([Constant(ConstInt(1) Int) Constant(ConstInt(2) Int) Constant(ConstInt(3) Int)] Array(Int 3) None) InitList([Constant(ConstInt(4) Int) Constant(ConstInt(5) Int) Constant(ConstInt(6) Int)] Array(Int 3) None)] Array(Array(Int 3) 2) None) Array(Array(Int 3) 2) None None None None false)))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" InitList([InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None) Constant(ConstInt(3) None)] None None) InitList([Constant(ConstInt(4) None) Constant(ConstInt(5) None) Constant(ConstInt(6) None)] None None)] None None) Array(Array(Int 3) 2) None None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" InitList([InitList([Constant(ConstInt(1) Int) Constant(ConstInt(2) Int) Constant(ConstInt(3) Int)] Array(Int 3) None) InitList([Constant(ConstInt(4) Int) Constant(ConstInt(5) Int) Constant(ConstInt(6) Int)] Array(Int 3) None)] Array(Array(Int 3) 2) None) Array(Array(Int 3) 2) None None None None false)))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Array(Int 3) 2)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("c99_to_tac: test_c99_to_tac.py::TestMultiDimArrays::test_two_dim_init_emits_six_stores (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass c99_to_tac\n"
      "source tests/test_c99_to_tac.py::TestMultiDimArrays::test_two_dim_init_emits_six_stores\n"
      "args {\"args\": [{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Array(Int 3) 2)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"args\": {\"0\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Array(Int 3) 2)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%3\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%4\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%5\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" InitList([InitList([Constant(ConstInt(1) Int) Constant(ConstInt(2) Int) Constant(ConstInt(3) Int)] Array(Int 3) None) InitList([Constant(ConstInt(4) Int) Constant(ConstInt(5) Int) Constant(ConstInt(6) Int)] Array(Int 3) None)] Array(Array(Int 3) 2) None) Array(Array(Int 3) 2) None None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) Int)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    GetAddress(Var(\"@0.a\") Var(\"%0\"))\n"
      "    Store(Constant(ConstInt(1)) Var(\"%0\") false)\n"
      "    Binary(Add Var(\"%0\") Constant(ConstInt(2)) Var(\"%1\"))\n"
      "    Store(Constant(ConstInt(2)) Var(\"%1\") false)\n"
      "    Binary(Add Var(\"%0\") Constant(ConstInt(4)) Var(\"%2\"))\n"
      "    Store(Constant(ConstInt(3)) Var(\"%2\") false)\n"
      "    Binary(Add Var(\"%0\") Constant(ConstInt(6)) Var(\"%3\"))\n"
      "    Store(Constant(ConstInt(4)) Var(\"%3\") false)\n"
      "    Binary(Add Var(\"%0\") Constant(ConstInt(8)) Var(\"%4\"))\n"
      "    Store(Constant(ConstInt(5)) Var(\"%4\") false)\n"
      "    Binary(Add Var(\"%0\") Constant(ConstInt(10)) Var(\"%5\"))\n"
      "    Store(Constant(ConstInt(6)) Var(\"%5\") false)\n"
      "    Ret(Constant(ConstInt(0)))\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("type_checking: test_c99_to_tac.py::TestMultiDimArrays::test_two_dim_init_uses_correct_byte_offsets (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c99_to_tac.py::TestMultiDimArrays::test_two_dim_init_uses_correct_byte_offsets\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" InitList([InitList([Constant(ConstInt(1) Int) Constant(ConstInt(2) Int) Constant(ConstInt(3) Int)] Array(Int 3) None) InitList([Constant(ConstInt(4) Int) Constant(ConstInt(5) Int) Constant(ConstInt(6) Int)] Array(Int 3) None)] Array(Array(Int 3) 2) None) Array(Array(Int 3) 2) None None None None false)))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" InitList([InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None) Constant(ConstInt(3) None)] None None) InitList([Constant(ConstInt(4) None) Constant(ConstInt(5) None) Constant(ConstInt(6) None)] None None)] None None) Array(Array(Int 3) 2) None None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" InitList([InitList([Constant(ConstInt(1) Int) Constant(ConstInt(2) Int) Constant(ConstInt(3) Int)] Array(Int 3) None) InitList([Constant(ConstInt(4) Int) Constant(ConstInt(5) Int) Constant(ConstInt(6) Int)] Array(Int 3) None)] Array(Array(Int 3) 2) None) Array(Array(Int 3) 2) None None None None false)))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Array(Int 3) 2)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("c99_to_tac: test_c99_to_tac.py::TestMultiDimArrays::test_two_dim_init_uses_correct_byte_offsets (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass c99_to_tac\n"
      "source tests/test_c99_to_tac.py::TestMultiDimArrays::test_two_dim_init_uses_correct_byte_offsets\n"
      "args {\"args\": [{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Array(Int 3) 2)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"args\": {\"0\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Array(Int 3) 2)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%3\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%4\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%5\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" InitList([InitList([Constant(ConstInt(1) Int) Constant(ConstInt(2) Int) Constant(ConstInt(3) Int)] Array(Int 3) None) InitList([Constant(ConstInt(4) Int) Constant(ConstInt(5) Int) Constant(ConstInt(6) Int)] Array(Int 3) None)] Array(Array(Int 3) 2) None) Array(Array(Int 3) 2) None None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) Int)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    GetAddress(Var(\"@0.a\") Var(\"%0\"))\n"
      "    Store(Constant(ConstInt(1)) Var(\"%0\") false)\n"
      "    Binary(Add Var(\"%0\") Constant(ConstInt(2)) Var(\"%1\"))\n"
      "    Store(Constant(ConstInt(2)) Var(\"%1\") false)\n"
      "    Binary(Add Var(\"%0\") Constant(ConstInt(4)) Var(\"%2\"))\n"
      "    Store(Constant(ConstInt(3)) Var(\"%2\") false)\n"
      "    Binary(Add Var(\"%0\") Constant(ConstInt(6)) Var(\"%3\"))\n"
      "    Store(Constant(ConstInt(4)) Var(\"%3\") false)\n"
      "    Binary(Add Var(\"%0\") Constant(ConstInt(8)) Var(\"%4\"))\n"
      "    Store(Constant(ConstInt(5)) Var(\"%4\") false)\n"
      "    Binary(Add Var(\"%0\") Constant(ConstInt(10)) Var(\"%5\"))\n"
      "    Store(Constant(ConstInt(6)) Var(\"%5\") false)\n"
      "    Ret(Constant(ConstInt(0)))\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("type_checking: test_c99_to_tac.py::TestMultiDimArrays::test_two_dim_int_subscript_scales_outer_by_six (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c99_to_tac.py::TestMultiDimArrays::test_two_dim_int_subscript_scales_outer_by_six\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" None Array(Array(Int 3) 2) None None None None false)))\\n    S(Return(Subscript(AddressOf(Subscript(AddressOf(Var(\\\"@0.a\\\" Array(Array(Int 3) 2)) Pointer(Array(Int 3))) Constant(ConstInt(1) Int) Array(Int 3)) Pointer(Int)) Constant(ConstInt(2) Int) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" None Array(Array(Int 3) 2) None None None None false)))\n"
      "    S(Return(Subscript(Subscript(Var(\"@0.a\" None) Constant(ConstInt(1) None) None) Constant(ConstInt(2) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" None Array(Array(Int 3) 2) None None None None false)))\\n    S(Return(Subscript(AddressOf(Subscript(AddressOf(Var(\\\"@0.a\\\" Array(Array(Int 3) 2)) Pointer(Array(Int 3))) Constant(ConstInt(1) Int) Array(Int 3)) Pointer(Int)) Constant(ConstInt(2) Int) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Array(Int 3) 2)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("c99_to_tac: test_c99_to_tac.py::TestMultiDimArrays::test_two_dim_int_subscript_scales_outer_by_six (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass c99_to_tac\n"
      "source tests/test_c99_to_tac.py::TestMultiDimArrays::test_two_dim_int_subscript_scales_outer_by_six\n"
      "args {\"args\": [{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Array(Int 3) 2)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"args\": {\"0\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Array(Int 3) 2)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Array(Int 3))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Array(Int 3))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%3\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%4\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%5\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" None Array(Array(Int 3) 2) None None None None false)))\n"
      "    S(Return(Subscript(AddressOf(Subscript(AddressOf(Var(\"@0.a\" Array(Array(Int 3) 2)) Pointer(Array(Int 3))) Constant(ConstInt(1) Int) Array(Int 3)) Pointer(Int)) Constant(ConstInt(2) Int) Int)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    GetAddress(Var(\"@0.a\") Var(\"%0\"))\n"
      "    Binary(Multiply Constant(ConstInt(1)) Constant(ConstInt(6)) Var(\"%1\"))\n"
      "    Binary(Add Var(\"%0\") Var(\"%1\") Var(\"%2\"))\n"
      "    Binary(Multiply Constant(ConstInt(2)) Constant(ConstInt(2)) Var(\"%3\"))\n"
      "    Binary(Add Var(\"%2\") Var(\"%3\") Var(\"%4\"))\n"
      "    Load(Var(\"%4\") Var(\"%5\") false)\n"
      "    Ret(Var(\"%5\"))\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("type_checking: test_c99_to_tac.py::TestMultiDimArrays::test_two_dim_subscript_lowers_with_two_scales (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c99_to_tac.py::TestMultiDimArrays::test_two_dim_subscript_lowers_with_two_scales\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" None Array(Array(Char 4) 3) None None None None false)))\\n    S(Return(Subscript(AddressOf(Subscript(AddressOf(Var(\\\"@0.a\\\" Array(Array(Char 4) 3)) Pointer(Array(Char 4))) Constant(ConstInt(1) Int) Array(Char 4)) Pointer(Char)) Constant(ConstInt(2) Int) Char)))\\n  ]) FunType([] Char) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" None Array(Array(Char 4) 3) None None None None false)))\n"
      "    S(Return(Subscript(Subscript(Var(\"@0.a\" None) Constant(ConstInt(1) None) None) Constant(ConstInt(2) None) None)))\n"
      "  ]) FunType([] Char) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" None Array(Array(Char 4) 3) None None None None false)))\\n    S(Return(Subscript(AddressOf(Subscript(AddressOf(Var(\\\"@0.a\\\" Array(Array(Char 4) 3)) Pointer(Array(Char 4))) Constant(ConstInt(1) Int) Array(Char 4)) Pointer(Char)) Constant(ConstInt(2) Int) Char)))\\n  ]) FunType([] Char) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Char)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Array(Char 4) 3)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("c99_to_tac: test_c99_to_tac.py::TestMultiDimArrays::test_two_dim_subscript_lowers_with_two_scales (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass c99_to_tac\n"
      "source tests/test_c99_to_tac.py::TestMultiDimArrays::test_two_dim_subscript_lowers_with_two_scales\n"
      "args {\"args\": [{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Char)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Array(Char 4) 3)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"args\": {\"0\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Char)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Array(Char 4) 3)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Array(Char 4))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Array(Char 4))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%3\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Char)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%4\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Char\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" None Array(Array(Char 4) 3) None None None None false)))\n"
      "    S(Return(Subscript(AddressOf(Subscript(AddressOf(Var(\"@0.a\" Array(Array(Char 4) 3)) Pointer(Array(Char 4))) Constant(ConstInt(1) Int) Array(Char 4)) Pointer(Char)) Constant(ConstInt(2) Int) Char)))\n"
      "  ]) FunType([] Char) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    GetAddress(Var(\"@0.a\") Var(\"%0\"))\n"
      "    Binary(Multiply Constant(ConstInt(1)) Constant(ConstInt(4)) Var(\"%1\"))\n"
      "    Binary(Add Var(\"%0\") Var(\"%1\") Var(\"%2\"))\n"
      "    Binary(Add Var(\"%2\") Constant(ConstInt(2)) Var(\"%3\"))\n"
      "    Load(Var(\"%3\") Var(\"%4\") false)\n"
      "    Ret(Var(\"%4\"))\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("type_checking: test_c99_to_tac.py::TestArrayFrameLayout::test_arrays_of_different_sizes_differ_by_size_difference (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c99_to_tac.py::TestArrayFrameLayout::test_arrays_of_different_sizes_differ_by_size_difference\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" None Array(Int 10) None None None None false)))\\n    S(Expression(Assignment(Subscript(AddressOf(Var(\\\"@0.a\\\" Array(Int 10)) Pointer(Int)) Constant(ConstInt(0) Int) Int) Constant(ConstInt(1) Int) Int)))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" None Array(Int 10) None None None None false)))\n"
      "    S(Expression(Assignment(Subscript(Var(\"@0.a\" None) Constant(ConstInt(0) None) None) Constant(ConstInt(1) None) None)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" None Array(Int 10) None None None None false)))\\n    S(Expression(Assignment(Subscript(AddressOf(Var(\\\"@0.a\\\" Array(Int 10)) Pointer(Int)) Constant(ConstInt(0) Int) Int) Constant(ConstInt(1) Int) Int)))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Int 10)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("c99_to_tac: test_c99_to_tac.py::TestArrayFrameLayout::test_arrays_of_different_sizes_differ_by_size_difference (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass c99_to_tac\n"
      "source tests/test_c99_to_tac.py::TestArrayFrameLayout::test_arrays_of_different_sizes_differ_by_size_difference\n"
      "args {\"args\": [{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Int 10)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"args\": {\"0\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Int 10)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" None Array(Int 10) None None None None false)))\n"
      "    S(Expression(Assignment(Subscript(AddressOf(Var(\"@0.a\" Array(Int 10)) Pointer(Int)) Constant(ConstInt(0) Int) Int) Constant(ConstInt(1) Int) Int)))\n"
      "    S(Return(Constant(ConstInt(0) Int)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    GetAddress(Var(\"@0.a\") Var(\"%0\"))\n"
      "    Binary(Multiply Constant(ConstInt(0)) Constant(ConstInt(2)) Var(\"%1\"))\n"
      "    Binary(Add Var(\"%0\") Var(\"%1\") Var(\"%2\"))\n"
      "    Store(Constant(ConstInt(1)) Var(\"%2\") false)\n"
      "    Ret(Constant(ConstInt(0)))\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("tac_to_asm: test_c99_to_tac.py::TestArrayFrameLayout::test_arrays_of_different_sizes_differ_by_size_difference (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass tac_to_asm\n"
      "source tests/test_c99_to_tac.py::TestArrayFrameLayout::test_arrays_of_different_sizes_differ_by_size_difference\n"
      "args {\"args\": [{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Int 10)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    GetAddress(Var(\"@0.a\") Var(\"%0\"))\n"
      "    Binary(Multiply Constant(ConstInt(0)) Constant(ConstInt(2)) Var(\"%1\"))\n"
      "    Binary(Add Var(\"%0\") Var(\"%1\") Var(\"%2\"))\n"
      "    Store(Constant(ConstInt(1)) Var(\"%2\") false)\n"
      "    Ret(Constant(ConstInt(0)))\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    LoadAddress(Pseudo(\"@0.a\" 0) Pseudo(\"%0\" 0))\n"
      "    Mov(Imm(0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"HARGS\" 0) false)\n"
      "    Mov(Imm(0) Reg(A) false)\n"
      "    Mov(Imm(8) Reg(X) false)\n"
      "    LogicalShiftRight(Data(\"HARGS\" 0))\n"
      "    Label(\".mul_loop@0\")\n"
      "    Branch(CC \".mul_skip@1\")\n"
      "    ClearCarry\n"
      "    Add(Imm(2) Reg(A))\n"
      "    Label(\".mul_skip@1\")\n"
      "    RotateRight(Reg(A))\n"
      "    RotateRight(Data(\"HARGS\" 0))\n"
      "    Dec(Reg(X))\n"
      "    Branch(NE \".mul_loop@0\")\n"
      "    Mov(Reg(A) Pseudo(\"%1\" 1) false)\n"
      "    Mov(Data(\"HARGS\" 0) Reg(A) false)\n"
      "    Mov(Reg(A) Pseudo(\"%1\" 0) false)\n"
      "    Mov(Pseudo(\"%0\" 0) Reg(A) false)\n"
      "    ClearCarry\n"
      "    Add(Pseudo(\"%1\" 0) Reg(A))\n"
      "    Mov(Reg(A) Pseudo(\"%2\" 0) false)\n"
      "    Mov(Pseudo(\"%0\" 1) Reg(A) false)\n"
      "    Add(Pseudo(\"%1\" 1) Reg(A))\n"
      "    Mov(Reg(A) Pseudo(\"%2\" 1) false)\n"
      "    Mov(Pseudo(\"%2\" 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 0) false)\n"
      "    Mov(Pseudo(\"%2\" 1) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 1) false)\n"
      "    Mov(Imm(1) Reg(A) false)\n"
      "    Mov(Reg(A) Indirect(0) false)\n"
      "    Mov(Imm(0) Reg(A) false)\n"
      "    Mov(Reg(A) Indirect(1) false)\n"
      "    Mov(Imm(0) Reg(A) false)\n"
      "    Mov(Reg(A) Reg(X) false)\n"
      "    Mov(Imm(0) Reg(A) false)\n"
      "    Ret(0 0 true true [] false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("replace_pseudoregisters: test_c99_to_tac.py::TestArrayFrameLayout::test_arrays_of_different_sizes_differ_by_size_difference (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass replace_pseudoregisters\n"
      "source tests/test_c99_to_tac.py::TestArrayFrameLayout::test_arrays_of_different_sizes_differ_by_size_difference\n"
      "args {\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Int 10)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    LoadAddress(Pseudo(\"@0.a\" 0) Pseudo(\"%0\" 0))\n"
      "    Mov(Imm(0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"HARGS\" 0) false)\n"
      "    Mov(Imm(0) Reg(A) false)\n"
      "    Mov(Imm(8) Reg(X) false)\n"
      "    LogicalShiftRight(Data(\"HARGS\" 0))\n"
      "    Label(\".mul_loop@0\")\n"
      "    Branch(CC \".mul_skip@1\")\n"
      "    ClearCarry\n"
      "    Add(Imm(2) Reg(A))\n"
      "    Label(\".mul_skip@1\")\n"
      "    RotateRight(Reg(A))\n"
      "    RotateRight(Data(\"HARGS\" 0))\n"
      "    Dec(Reg(X))\n"
      "    Branch(NE \".mul_loop@0\")\n"
      "    Mov(Reg(A) Pseudo(\"%1\" 1) false)\n"
      "    Mov(Data(\"HARGS\" 0) Reg(A) false)\n"
      "    Mov(Reg(A) Pseudo(\"%1\" 0) false)\n"
      "    Mov(Pseudo(\"%0\" 0) Reg(A) false)\n"
      "    ClearCarry\n"
      "    Add(Pseudo(\"%1\" 0) Reg(A))\n"
      "    Mov(Reg(A) Pseudo(\"%2\" 0) false)\n"
      "    Mov(Pseudo(\"%0\" 1) Reg(A) false)\n"
      "    Add(Pseudo(\"%1\" 1) Reg(A))\n"
      "    Mov(Reg(A) Pseudo(\"%2\" 1) false)\n"
      "    Mov(Pseudo(\"%2\" 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 0) false)\n"
      "    Mov(Pseudo(\"%2\" 1) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 1) false)\n"
      "    Mov(Imm(1) Reg(A) false)\n"
      "    Mov(Reg(A) Indirect(0) false)\n"
      "    Mov(Imm(0) Reg(A) false)\n"
      "    Mov(Reg(A) Indirect(1) false)\n"
      "    Mov(Imm(0) Reg(A) false)\n"
      "    Mov(Reg(A) Reg(X) false)\n"
      "    Mov(Imm(0) Reg(A) false)\n"
      "    Ret(0 0 true true [] false)\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    FunctionPrologue(0 26 [])\n"
      "    LoadAddress(Frame(1) Frame(23))\n"
      "    Mov(Imm(0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"HARGS\" 0) false)\n"
      "    Mov(Imm(0) Reg(A) false)\n"
      "    Mov(Imm(8) Reg(X) false)\n"
      "    LogicalShiftRight(Data(\"HARGS\" 0))\n"
      "    Label(\".mul_loop@0\")\n"
      "    Branch(CC \".mul_skip@1\")\n"
      "    ClearCarry\n"
      "    Add(Imm(2) Reg(A))\n"
      "    Label(\".mul_skip@1\")\n"
      "    RotateRight(Reg(A))\n"
      "    RotateRight(Data(\"HARGS\" 0))\n"
      "    Dec(Reg(X))\n"
      "    Branch(NE \".mul_loop@0\")\n"
      "    Mov(Reg(A) Frame(22) false)\n"
      "    Mov(Data(\"HARGS\" 0) Reg(A) false)\n"
      "    Mov(Reg(A) Frame(21) false)\n"
      "    Mov(Frame(23) Reg(A) false)\n"
      "    ClearCarry\n"
      "    Add(Frame(21) Reg(A))\n"
      "    Mov(Reg(A) Frame(25) false)\n"
      "    Mov(Frame(24) Reg(A) false)\n"
      "    Add(Frame(22) Reg(A))\n"
      "    Mov(Reg(A) Frame(26) false)\n"
      "    Mov(Frame(25) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 0) false)\n"
      "    Mov(Frame(26) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 1) false)\n"
      "    Mov(Imm(1) Reg(A) false)\n"
      "    Mov(Reg(A) Indirect(0) false)\n"
      "    Mov(Imm(0) Reg(A) false)\n"
      "    Mov(Reg(A) Indirect(1) false)\n"
      "    Mov(Imm(0) Reg(A) false)\n"
      "    Mov(Reg(A) Reg(X) false)\n"
      "    Mov(Imm(0) Reg(A) false)\n"
      "    Ret(0 26 true true [] false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("type_checking: test_c99_to_tac.py::TestArrayFrameLayout::test_arrays_of_different_sizes_differ_by_size_difference (call 5)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c99_to_tac.py::TestArrayFrameLayout::test_arrays_of_different_sizes_differ_by_size_difference\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" None Array(Int 20) None None None None false)))\\n    S(Expression(Assignment(Subscript(AddressOf(Var(\\\"@0.a\\\" Array(Int 20)) Pointer(Int)) Constant(ConstInt(0) Int) Int) Constant(ConstInt(1) Int) Int)))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" None Array(Int 20) None None None None false)))\n"
      "    S(Expression(Assignment(Subscript(Var(\"@0.a\" None) Constant(ConstInt(0) None) None) Constant(ConstInt(1) None) None)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" None Array(Int 20) None None None None false)))\\n    S(Expression(Assignment(Subscript(AddressOf(Var(\\\"@0.a\\\" Array(Int 20)) Pointer(Int)) Constant(ConstInt(0) Int) Int) Constant(ConstInt(1) Int) Int)))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Int 20)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("c99_to_tac: test_c99_to_tac.py::TestArrayFrameLayout::test_arrays_of_different_sizes_differ_by_size_difference (call 6)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass c99_to_tac\n"
      "source tests/test_c99_to_tac.py::TestArrayFrameLayout::test_arrays_of_different_sizes_differ_by_size_difference\n"
      "args {\"args\": [{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Int 20)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"args\": {\"0\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Int 20)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" None Array(Int 20) None None None None false)))\n"
      "    S(Expression(Assignment(Subscript(AddressOf(Var(\"@0.a\" Array(Int 20)) Pointer(Int)) Constant(ConstInt(0) Int) Int) Constant(ConstInt(1) Int) Int)))\n"
      "    S(Return(Constant(ConstInt(0) Int)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    GetAddress(Var(\"@0.a\") Var(\"%0\"))\n"
      "    Binary(Multiply Constant(ConstInt(0)) Constant(ConstInt(2)) Var(\"%1\"))\n"
      "    Binary(Add Var(\"%0\") Var(\"%1\") Var(\"%2\"))\n"
      "    Store(Constant(ConstInt(1)) Var(\"%2\") false)\n"
      "    Ret(Constant(ConstInt(0)))\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("tac_to_asm: test_c99_to_tac.py::TestArrayFrameLayout::test_arrays_of_different_sizes_differ_by_size_difference (call 7)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass tac_to_asm\n"
      "source tests/test_c99_to_tac.py::TestArrayFrameLayout::test_arrays_of_different_sizes_differ_by_size_difference\n"
      "args {\"args\": [{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Int 20)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    GetAddress(Var(\"@0.a\") Var(\"%0\"))\n"
      "    Binary(Multiply Constant(ConstInt(0)) Constant(ConstInt(2)) Var(\"%1\"))\n"
      "    Binary(Add Var(\"%0\") Var(\"%1\") Var(\"%2\"))\n"
      "    Store(Constant(ConstInt(1)) Var(\"%2\") false)\n"
      "    Ret(Constant(ConstInt(0)))\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    LoadAddress(Pseudo(\"@0.a\" 0) Pseudo(\"%0\" 0))\n"
      "    Mov(Imm(0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"HARGS\" 0) false)\n"
      "    Mov(Imm(0) Reg(A) false)\n"
      "    Mov(Imm(8) Reg(X) false)\n"
      "    LogicalShiftRight(Data(\"HARGS\" 0))\n"
      "    Label(\".mul_loop@0\")\n"
      "    Branch(CC \".mul_skip@1\")\n"
      "    ClearCarry\n"
      "    Add(Imm(2) Reg(A))\n"
      "    Label(\".mul_skip@1\")\n"
      "    RotateRight(Reg(A))\n"
      "    RotateRight(Data(\"HARGS\" 0))\n"
      "    Dec(Reg(X))\n"
      "    Branch(NE \".mul_loop@0\")\n"
      "    Mov(Reg(A) Pseudo(\"%1\" 1) false)\n"
      "    Mov(Data(\"HARGS\" 0) Reg(A) false)\n"
      "    Mov(Reg(A) Pseudo(\"%1\" 0) false)\n"
      "    Mov(Pseudo(\"%0\" 0) Reg(A) false)\n"
      "    ClearCarry\n"
      "    Add(Pseudo(\"%1\" 0) Reg(A))\n"
      "    Mov(Reg(A) Pseudo(\"%2\" 0) false)\n"
      "    Mov(Pseudo(\"%0\" 1) Reg(A) false)\n"
      "    Add(Pseudo(\"%1\" 1) Reg(A))\n"
      "    Mov(Reg(A) Pseudo(\"%2\" 1) false)\n"
      "    Mov(Pseudo(\"%2\" 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 0) false)\n"
      "    Mov(Pseudo(\"%2\" 1) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 1) false)\n"
      "    Mov(Imm(1) Reg(A) false)\n"
      "    Mov(Reg(A) Indirect(0) false)\n"
      "    Mov(Imm(0) Reg(A) false)\n"
      "    Mov(Reg(A) Indirect(1) false)\n"
      "    Mov(Imm(0) Reg(A) false)\n"
      "    Mov(Reg(A) Reg(X) false)\n"
      "    Mov(Imm(0) Reg(A) false)\n"
      "    Ret(0 0 true true [] false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("replace_pseudoregisters: test_c99_to_tac.py::TestArrayFrameLayout::test_arrays_of_different_sizes_differ_by_size_difference (call 8)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass replace_pseudoregisters\n"
      "source tests/test_c99_to_tac.py::TestArrayFrameLayout::test_arrays_of_different_sizes_differ_by_size_difference\n"
      "args {\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Int 20)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    LoadAddress(Pseudo(\"@0.a\" 0) Pseudo(\"%0\" 0))\n"
      "    Mov(Imm(0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"HARGS\" 0) false)\n"
      "    Mov(Imm(0) Reg(A) false)\n"
      "    Mov(Imm(8) Reg(X) false)\n"
      "    LogicalShiftRight(Data(\"HARGS\" 0))\n"
      "    Label(\".mul_loop@0\")\n"
      "    Branch(CC \".mul_skip@1\")\n"
      "    ClearCarry\n"
      "    Add(Imm(2) Reg(A))\n"
      "    Label(\".mul_skip@1\")\n"
      "    RotateRight(Reg(A))\n"
      "    RotateRight(Data(\"HARGS\" 0))\n"
      "    Dec(Reg(X))\n"
      "    Branch(NE \".mul_loop@0\")\n"
      "    Mov(Reg(A) Pseudo(\"%1\" 1) false)\n"
      "    Mov(Data(\"HARGS\" 0) Reg(A) false)\n"
      "    Mov(Reg(A) Pseudo(\"%1\" 0) false)\n"
      "    Mov(Pseudo(\"%0\" 0) Reg(A) false)\n"
      "    ClearCarry\n"
      "    Add(Pseudo(\"%1\" 0) Reg(A))\n"
      "    Mov(Reg(A) Pseudo(\"%2\" 0) false)\n"
      "    Mov(Pseudo(\"%0\" 1) Reg(A) false)\n"
      "    Add(Pseudo(\"%1\" 1) Reg(A))\n"
      "    Mov(Reg(A) Pseudo(\"%2\" 1) false)\n"
      "    Mov(Pseudo(\"%2\" 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 0) false)\n"
      "    Mov(Pseudo(\"%2\" 1) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 1) false)\n"
      "    Mov(Imm(1) Reg(A) false)\n"
      "    Mov(Reg(A) Indirect(0) false)\n"
      "    Mov(Imm(0) Reg(A) false)\n"
      "    Mov(Reg(A) Indirect(1) false)\n"
      "    Mov(Imm(0) Reg(A) false)\n"
      "    Mov(Reg(A) Reg(X) false)\n"
      "    Mov(Imm(0) Reg(A) false)\n"
      "    Ret(0 0 true true [] false)\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    FunctionPrologue(0 46 [])\n"
      "    LoadAddress(Frame(1) Frame(43))\n"
      "    Mov(Imm(0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"HARGS\" 0) false)\n"
      "    Mov(Imm(0) Reg(A) false)\n"
      "    Mov(Imm(8) Reg(X) false)\n"
      "    LogicalShiftRight(Data(\"HARGS\" 0))\n"
      "    Label(\".mul_loop@0\")\n"
      "    Branch(CC \".mul_skip@1\")\n"
      "    ClearCarry\n"
      "    Add(Imm(2) Reg(A))\n"
      "    Label(\".mul_skip@1\")\n"
      "    RotateRight(Reg(A))\n"
      "    RotateRight(Data(\"HARGS\" 0))\n"
      "    Dec(Reg(X))\n"
      "    Branch(NE \".mul_loop@0\")\n"
      "    Mov(Reg(A) Frame(42) false)\n"
      "    Mov(Data(\"HARGS\" 0) Reg(A) false)\n"
      "    Mov(Reg(A) Frame(41) false)\n"
      "    Mov(Frame(43) Reg(A) false)\n"
      "    ClearCarry\n"
      "    Add(Frame(41) Reg(A))\n"
      "    Mov(Reg(A) Frame(45) false)\n"
      "    Mov(Frame(44) Reg(A) false)\n"
      "    Add(Frame(42) Reg(A))\n"
      "    Mov(Reg(A) Frame(46) false)\n"
      "    Mov(Frame(45) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 0) false)\n"
      "    Mov(Frame(46) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 1) false)\n"
      "    Mov(Imm(1) Reg(A) false)\n"
      "    Mov(Reg(A) Indirect(0) false)\n"
      "    Mov(Imm(0) Reg(A) false)\n"
      "    Mov(Reg(A) Indirect(1) false)\n"
      "    Mov(Imm(0) Reg(A) false)\n"
      "    Mov(Reg(A) Reg(X) false)\n"
      "    Mov(Imm(0) Reg(A) false)\n"
      "    Ret(0 46 true true [] false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("type_checking: test_c99_to_tac.py::TestArrayFrameLayout::test_char_array_takes_at_least_count_bytes (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c99_to_tac.py::TestArrayFrameLayout::test_char_array_takes_at_least_count_bytes\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" None Array(Char 10) None None None None false)))\\n    S(Expression(Assignment(Subscript(AddressOf(Var(\\\"@0.a\\\" Array(Char 10)) Pointer(Char)) Constant(ConstInt(0) Int) Char) Cast(Char Constant(ConstInt(1) Int) Char) Char)))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" None Array(Char 10) None None None None false)))\n"
      "    S(Expression(Assignment(Subscript(Var(\"@0.a\" None) Constant(ConstInt(0) None) None) Constant(ConstInt(1) None) None)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" None Array(Char 10) None None None None false)))\\n    S(Expression(Assignment(Subscript(AddressOf(Var(\\\"@0.a\\\" Array(Char 10)) Pointer(Char)) Constant(ConstInt(0) Int) Char) Cast(Char Constant(ConstInt(1) Int) Char) Char)))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Char 10)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("c99_to_tac: test_c99_to_tac.py::TestArrayFrameLayout::test_char_array_takes_at_least_count_bytes (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass c99_to_tac\n"
      "source tests/test_c99_to_tac.py::TestArrayFrameLayout::test_char_array_takes_at_least_count_bytes\n"
      "args {\"args\": [{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Char 10)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"args\": {\"0\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Char 10)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Char\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Char)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Char)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" None Array(Char 10) None None None None false)))\n"
      "    S(Expression(Assignment(Subscript(AddressOf(Var(\"@0.a\" Array(Char 10)) Pointer(Char)) Constant(ConstInt(0) Int) Char) Cast(Char Constant(ConstInt(1) Int) Char) Char)))\n"
      "    S(Return(Constant(ConstInt(0) Int)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Truncate(Constant(ConstInt(1)) Var(\"%0\"))\n"
      "    GetAddress(Var(\"@0.a\") Var(\"%1\"))\n"
      "    Binary(Add Var(\"%1\") Constant(ConstInt(0)) Var(\"%2\"))\n"
      "    Store(Var(\"%0\") Var(\"%2\") false)\n"
      "    Ret(Constant(ConstInt(0)))\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("tac_to_asm: test_c99_to_tac.py::TestArrayFrameLayout::test_char_array_takes_at_least_count_bytes (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass tac_to_asm\n"
      "source tests/test_c99_to_tac.py::TestArrayFrameLayout::test_char_array_takes_at_least_count_bytes\n"
      "args {\"args\": [{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Char 10)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Char\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Char)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Char)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Truncate(Constant(ConstInt(1)) Var(\"%0\"))\n"
      "    GetAddress(Var(\"@0.a\") Var(\"%1\"))\n"
      "    Binary(Add Var(\"%1\") Constant(ConstInt(0)) Var(\"%2\"))\n"
      "    Store(Var(\"%0\") Var(\"%2\") false)\n"
      "    Ret(Constant(ConstInt(0)))\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Mov(Imm(1) Pseudo(\"%0\" 0) false)\n"
      "    LoadAddress(Pseudo(\"@0.a\" 0) Pseudo(\"%1\" 0))\n"
      "    Mov(Pseudo(\"%1\" 0) Reg(A) false)\n"
      "    ClearCarry\n"
      "    Add(Imm(0) Reg(A))\n"
      "    Mov(Reg(A) Pseudo(\"%2\" 0) false)\n"
      "    Mov(Pseudo(\"%1\" 1) Reg(A) false)\n"
      "    Add(Imm(0) Reg(A))\n"
      "    Mov(Reg(A) Pseudo(\"%2\" 1) false)\n"
      "    Mov(Pseudo(\"%2\" 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 0) false)\n"
      "    Mov(Pseudo(\"%2\" 1) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 1) false)\n"
      "    Mov(Pseudo(\"%0\" 0) Reg(A) false)\n"
      "    Mov(Reg(A) Indirect(0) false)\n"
      "    Mov(Imm(0) Reg(A) false)\n"
      "    Mov(Reg(A) Reg(X) false)\n"
      "    Mov(Imm(0) Reg(A) false)\n"
      "    Ret(0 0 true true [] false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("replace_pseudoregisters: test_c99_to_tac.py::TestArrayFrameLayout::test_char_array_takes_at_least_count_bytes (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass replace_pseudoregisters\n"
      "source tests/test_c99_to_tac.py::TestArrayFrameLayout::test_char_array_takes_at_least_count_bytes\n"
      "args {\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Char 10)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Char\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Char)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Char)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Mov(Imm(1) Pseudo(\"%0\" 0) false)\n"
      "    LoadAddress(Pseudo(\"@0.a\" 0) Pseudo(\"%1\" 0))\n"
      "    Mov(Pseudo(\"%1\" 0) Reg(A) false)\n"
      "    ClearCarry\n"
      "    Add(Imm(0) Reg(A))\n"
      "    Mov(Reg(A) Pseudo(\"%2\" 0) false)\n"
      "    Mov(Pseudo(\"%1\" 1) Reg(A) false)\n"
      "    Add(Imm(0) Reg(A))\n"
      "    Mov(Reg(A) Pseudo(\"%2\" 1) false)\n"
      "    Mov(Pseudo(\"%2\" 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 0) false)\n"
      "    Mov(Pseudo(\"%2\" 1) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 1) false)\n"
      "    Mov(Pseudo(\"%0\" 0) Reg(A) false)\n"
      "    Mov(Reg(A) Indirect(0) false)\n"
      "    Mov(Imm(0) Reg(A) false)\n"
      "    Mov(Reg(A) Reg(X) false)\n"
      "    Mov(Imm(0) Reg(A) false)\n"
      "    Ret(0 0 true true [] false)\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    FunctionPrologue(0 15 [])\n"
      "    Mov(Imm(1) Frame(1) false)\n"
      "    LoadAddress(Frame(2) Frame(12))\n"
      "    Mov(Frame(12) Reg(A) false)\n"
      "    ClearCarry\n"
      "    Add(Imm(0) Reg(A))\n"
      "    Mov(Reg(A) Frame(14) false)\n"
      "    Mov(Frame(13) Reg(A) false)\n"
      "    Add(Imm(0) Reg(A))\n"
      "    Mov(Reg(A) Frame(15) false)\n"
      "    Mov(Frame(14) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 0) false)\n"
      "    Mov(Frame(15) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 1) false)\n"
      "    Mov(Frame(1) Reg(A) false)\n"
      "    Mov(Reg(A) Indirect(0) false)\n"
      "    Mov(Imm(0) Reg(A) false)\n"
      "    Mov(Reg(A) Reg(X) false)\n"
      "    Mov(Imm(0) Reg(A) false)\n"
      "    Ret(0 15 true true [] false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("type_checking: test_c99_to_tac.py::TestArrayFrameLayout::test_int_array_takes_at_least_two_x_count_bytes (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c99_to_tac.py::TestArrayFrameLayout::test_int_array_takes_at_least_two_x_count_bytes\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" None Array(Int 5) None None None None false)))\\n    S(Expression(Assignment(Subscript(AddressOf(Var(\\\"@0.a\\\" Array(Int 5)) Pointer(Int)) Constant(ConstInt(0) Int) Int) Constant(ConstInt(1) Int) Int)))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" None Array(Int 5) None None None None false)))\n"
      "    S(Expression(Assignment(Subscript(Var(\"@0.a\" None) Constant(ConstInt(0) None) None) Constant(ConstInt(1) None) None)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" None Array(Int 5) None None None None false)))\\n    S(Expression(Assignment(Subscript(AddressOf(Var(\\\"@0.a\\\" Array(Int 5)) Pointer(Int)) Constant(ConstInt(0) Int) Int) Constant(ConstInt(1) Int) Int)))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Int 5)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("c99_to_tac: test_c99_to_tac.py::TestArrayFrameLayout::test_int_array_takes_at_least_two_x_count_bytes (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass c99_to_tac\n"
      "source tests/test_c99_to_tac.py::TestArrayFrameLayout::test_int_array_takes_at_least_two_x_count_bytes\n"
      "args {\"args\": [{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Int 5)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"args\": {\"0\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Int 5)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" None Array(Int 5) None None None None false)))\n"
      "    S(Expression(Assignment(Subscript(AddressOf(Var(\"@0.a\" Array(Int 5)) Pointer(Int)) Constant(ConstInt(0) Int) Int) Constant(ConstInt(1) Int) Int)))\n"
      "    S(Return(Constant(ConstInt(0) Int)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    GetAddress(Var(\"@0.a\") Var(\"%0\"))\n"
      "    Binary(Multiply Constant(ConstInt(0)) Constant(ConstInt(2)) Var(\"%1\"))\n"
      "    Binary(Add Var(\"%0\") Var(\"%1\") Var(\"%2\"))\n"
      "    Store(Constant(ConstInt(1)) Var(\"%2\") false)\n"
      "    Ret(Constant(ConstInt(0)))\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("tac_to_asm: test_c99_to_tac.py::TestArrayFrameLayout::test_int_array_takes_at_least_two_x_count_bytes (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass tac_to_asm\n"
      "source tests/test_c99_to_tac.py::TestArrayFrameLayout::test_int_array_takes_at_least_two_x_count_bytes\n"
      "args {\"args\": [{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Int 5)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    GetAddress(Var(\"@0.a\") Var(\"%0\"))\n"
      "    Binary(Multiply Constant(ConstInt(0)) Constant(ConstInt(2)) Var(\"%1\"))\n"
      "    Binary(Add Var(\"%0\") Var(\"%1\") Var(\"%2\"))\n"
      "    Store(Constant(ConstInt(1)) Var(\"%2\") false)\n"
      "    Ret(Constant(ConstInt(0)))\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    LoadAddress(Pseudo(\"@0.a\" 0) Pseudo(\"%0\" 0))\n"
      "    Mov(Imm(0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"HARGS\" 0) false)\n"
      "    Mov(Imm(0) Reg(A) false)\n"
      "    Mov(Imm(8) Reg(X) false)\n"
      "    LogicalShiftRight(Data(\"HARGS\" 0))\n"
      "    Label(\".mul_loop@0\")\n"
      "    Branch(CC \".mul_skip@1\")\n"
      "    ClearCarry\n"
      "    Add(Imm(2) Reg(A))\n"
      "    Label(\".mul_skip@1\")\n"
      "    RotateRight(Reg(A))\n"
      "    RotateRight(Data(\"HARGS\" 0))\n"
      "    Dec(Reg(X))\n"
      "    Branch(NE \".mul_loop@0\")\n"
      "    Mov(Reg(A) Pseudo(\"%1\" 1) false)\n"
      "    Mov(Data(\"HARGS\" 0) Reg(A) false)\n"
      "    Mov(Reg(A) Pseudo(\"%1\" 0) false)\n"
      "    Mov(Pseudo(\"%0\" 0) Reg(A) false)\n"
      "    ClearCarry\n"
      "    Add(Pseudo(\"%1\" 0) Reg(A))\n"
      "    Mov(Reg(A) Pseudo(\"%2\" 0) false)\n"
      "    Mov(Pseudo(\"%0\" 1) Reg(A) false)\n"
      "    Add(Pseudo(\"%1\" 1) Reg(A))\n"
      "    Mov(Reg(A) Pseudo(\"%2\" 1) false)\n"
      "    Mov(Pseudo(\"%2\" 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 0) false)\n"
      "    Mov(Pseudo(\"%2\" 1) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 1) false)\n"
      "    Mov(Imm(1) Reg(A) false)\n"
      "    Mov(Reg(A) Indirect(0) false)\n"
      "    Mov(Imm(0) Reg(A) false)\n"
      "    Mov(Reg(A) Indirect(1) false)\n"
      "    Mov(Imm(0) Reg(A) false)\n"
      "    Mov(Reg(A) Reg(X) false)\n"
      "    Mov(Imm(0) Reg(A) false)\n"
      "    Ret(0 0 true true [] false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("replace_pseudoregisters: test_c99_to_tac.py::TestArrayFrameLayout::test_int_array_takes_at_least_two_x_count_bytes (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass replace_pseudoregisters\n"
      "source tests/test_c99_to_tac.py::TestArrayFrameLayout::test_int_array_takes_at_least_two_x_count_bytes\n"
      "args {\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Int 5)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    LoadAddress(Pseudo(\"@0.a\" 0) Pseudo(\"%0\" 0))\n"
      "    Mov(Imm(0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"HARGS\" 0) false)\n"
      "    Mov(Imm(0) Reg(A) false)\n"
      "    Mov(Imm(8) Reg(X) false)\n"
      "    LogicalShiftRight(Data(\"HARGS\" 0))\n"
      "    Label(\".mul_loop@0\")\n"
      "    Branch(CC \".mul_skip@1\")\n"
      "    ClearCarry\n"
      "    Add(Imm(2) Reg(A))\n"
      "    Label(\".mul_skip@1\")\n"
      "    RotateRight(Reg(A))\n"
      "    RotateRight(Data(\"HARGS\" 0))\n"
      "    Dec(Reg(X))\n"
      "    Branch(NE \".mul_loop@0\")\n"
      "    Mov(Reg(A) Pseudo(\"%1\" 1) false)\n"
      "    Mov(Data(\"HARGS\" 0) Reg(A) false)\n"
      "    Mov(Reg(A) Pseudo(\"%1\" 0) false)\n"
      "    Mov(Pseudo(\"%0\" 0) Reg(A) false)\n"
      "    ClearCarry\n"
      "    Add(Pseudo(\"%1\" 0) Reg(A))\n"
      "    Mov(Reg(A) Pseudo(\"%2\" 0) false)\n"
      "    Mov(Pseudo(\"%0\" 1) Reg(A) false)\n"
      "    Add(Pseudo(\"%1\" 1) Reg(A))\n"
      "    Mov(Reg(A) Pseudo(\"%2\" 1) false)\n"
      "    Mov(Pseudo(\"%2\" 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 0) false)\n"
      "    Mov(Pseudo(\"%2\" 1) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 1) false)\n"
      "    Mov(Imm(1) Reg(A) false)\n"
      "    Mov(Reg(A) Indirect(0) false)\n"
      "    Mov(Imm(0) Reg(A) false)\n"
      "    Mov(Reg(A) Indirect(1) false)\n"
      "    Mov(Imm(0) Reg(A) false)\n"
      "    Mov(Reg(A) Reg(X) false)\n"
      "    Mov(Imm(0) Reg(A) false)\n"
      "    Ret(0 0 true true [] false)\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    FunctionPrologue(0 16 [])\n"
      "    LoadAddress(Frame(1) Frame(13))\n"
      "    Mov(Imm(0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"HARGS\" 0) false)\n"
      "    Mov(Imm(0) Reg(A) false)\n"
      "    Mov(Imm(8) Reg(X) false)\n"
      "    LogicalShiftRight(Data(\"HARGS\" 0))\n"
      "    Label(\".mul_loop@0\")\n"
      "    Branch(CC \".mul_skip@1\")\n"
      "    ClearCarry\n"
      "    Add(Imm(2) Reg(A))\n"
      "    Label(\".mul_skip@1\")\n"
      "    RotateRight(Reg(A))\n"
      "    RotateRight(Data(\"HARGS\" 0))\n"
      "    Dec(Reg(X))\n"
      "    Branch(NE \".mul_loop@0\")\n"
      "    Mov(Reg(A) Frame(12) false)\n"
      "    Mov(Data(\"HARGS\" 0) Reg(A) false)\n"
      "    Mov(Reg(A) Frame(11) false)\n"
      "    Mov(Frame(13) Reg(A) false)\n"
      "    ClearCarry\n"
      "    Add(Frame(11) Reg(A))\n"
      "    Mov(Reg(A) Frame(15) false)\n"
      "    Mov(Frame(14) Reg(A) false)\n"
      "    Add(Frame(12) Reg(A))\n"
      "    Mov(Reg(A) Frame(16) false)\n"
      "    Mov(Frame(15) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 0) false)\n"
      "    Mov(Frame(16) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 1) false)\n"
      "    Mov(Imm(1) Reg(A) false)\n"
      "    Mov(Reg(A) Indirect(0) false)\n"
      "    Mov(Imm(0) Reg(A) false)\n"
      "    Mov(Reg(A) Indirect(1) false)\n"
      "    Mov(Imm(0) Reg(A) false)\n"
      "    Mov(Reg(A) Reg(X) false)\n"
      "    Mov(Imm(0) Reg(A) false)\n"
      "    Ret(0 16 true true [] false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("type_checking: test_c99_to_tac.py::TestQualifiedTypeLowering::test_arrow_through_const_pointer (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c99_to_tac.py::TestQualifiedTypeLowering::test_arrow_through_const_pointer\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  StructDecl(struct_decl(\\\"s\\\" false [member_decl(\\\"a\\\" UChar) member_decl(\\\"b\\\" UChar)]))\\n  FunctionDecl(function_decl(\\\"f\\\" [\\\"@0.q\\\"] Block([\\n    D(VarDecl(var_decl(\\\"@1.p\\\" Var(\\\"@0.q\\\" Pointer(Structure(\\\"s\\\"))) Const(Pointer(Structure(\\\"s\\\"))) None None None None false)))\\n    S(Expression(Assignment(Arrow(Var(\\\"@1.p\\\" Const(Pointer(Structure(\\\"s\\\")))) \\\"b\\\" UChar) Cast(UChar Constant(ConstInt(5) Int) UChar) UChar)))\\n  ]) FunType([Pointer(Structure(\\\"s\\\"))] Void) None None [\\\"\\\"] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"a\" UChar) member_decl(\"b\" UChar)]))\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.q\"] Block([\n"
      "    D(VarDecl(var_decl(\"@1.p\" Var(\"@0.q\" None) Const(Pointer(Structure(\"s\"))) None None None None false)))\n"
      "    S(Expression(Assignment(Arrow(Var(\"@1.p\" None) \"b\" None) Constant(ConstInt(5) None) None)))\n"
      "  ]) FunType([Pointer(Structure(\"s\"))] Void) None None [\"\"] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  StructDecl(struct_decl(\\\"s\\\" false [member_decl(\\\"a\\\" UChar) member_decl(\\\"b\\\" UChar)]))\\n  FunctionDecl(function_decl(\\\"f\\\" [\\\"@0.q\\\"] Block([\\n    D(VarDecl(var_decl(\\\"@1.p\\\" Var(\\\"@0.q\\\" Pointer(Structure(\\\"s\\\"))) Const(Pointer(Structure(\\\"s\\\"))) None None None None false)))\\n    S(Expression(Assignment(Arrow(Var(\\\"@1.p\\\" Const(Pointer(Structure(\\\"s\\\")))) \\\"b\\\" UChar) Cast(UChar Constant(ConstInt(5) Int) UChar) UChar)))\\n  ]) FunType([Pointer(Structure(\\\"s\\\"))] Void) None None [\\\"\\\"] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([Pointer(Structure(\\\"s\\\"))] Void)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.q\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Structure(\\\"s\\\"))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Const(Pointer(Structure(\\\"s\\\")))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": [[\"s\", {\"object\": \"passes.type_checking.StructLayout\", \"fields\": {\"tag\": \"s\", \"is_union\": false, \"members\": [{\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"a\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"byte_offset\": 0}}, {\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"b\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"byte_offset\": 1}}], \"size\": 2, \"complete\": true}}]]}}}]}\n");
}

TEST_CASE("c99_to_tac: test_c99_to_tac.py::TestQualifiedTypeLowering::test_arrow_through_const_pointer (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass c99_to_tac\n"
      "source tests/test_c99_to_tac.py::TestQualifiedTypeLowering::test_arrow_through_const_pointer\n"
      "args {\"args\": [{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([Pointer(Structure(\\\"s\\\"))] Void)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.q\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Structure(\\\"s\\\"))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Const(Pointer(Structure(\\\"s\\\")))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": [[\"s\", {\"object\": \"passes.type_checking.StructLayout\", \"fields\": {\"tag\": \"s\", \"is_union\": false, \"members\": [{\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"a\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"byte_offset\": 0}}, {\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"b\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"byte_offset\": 1}}], \"size\": 2, \"complete\": true}}]]}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"args\": {\"0\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([Pointer(Structure(\\\"s\\\"))] Void)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.q\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Structure(\\\"s\\\"))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Const(Pointer(Structure(\\\"s\\\")))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"a\" UChar) member_decl(\"b\" UChar)]))\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.q\"] Block([\n"
      "    D(VarDecl(var_decl(\"@1.p\" Var(\"@0.q\" Pointer(Structure(\"s\"))) Const(Pointer(Structure(\"s\"))) None None None None false)))\n"
      "    S(Expression(Assignment(Arrow(Var(\"@1.p\" Const(Pointer(Structure(\"s\")))) \"b\" UChar) Cast(UChar Constant(ConstInt(5) Int) UChar) UChar)))\n"
      "  ]) FunType([Pointer(Structure(\"s\"))] Void) None None [\"\"] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" true [\"@0.q\"] [\n"
      "    Copy(Var(\"@0.q\") Var(\"@1.p\"))\n"
      "    Truncate(Constant(ConstInt(5)) Var(\"%0\"))\n"
      "    Binary(Add Var(\"@1.p\") Constant(ConstInt(1)) Var(\"%1\"))\n"
      "    Store(Var(\"%0\") Var(\"%1\") false)\n"
      "    Ret(None)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("type_checking: test_c99_to_tac.py::TestQualifiedTypeLowering::test_const_struct_local_stores_each_member (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c99_to_tac.py::TestQualifiedTypeLowering::test_const_struct_local_stores_each_member\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  StructDecl(struct_decl(\\\"s\\\" false [member_decl(\\\"a\\\" UChar) member_decl(\\\"b\\\" Int)]))\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.l\\\" InitList([Cast(UChar Constant(ConstInt(7) Int) UChar) Constant(ConstInt(8) Int)] Structure(\\\"s\\\") None) Const(Structure(\\\"s\\\")) None None None None false)))\\n    S(Return(Dot(Var(\\\"@0.l\\\" Const(Structure(\\\"s\\\"))) \\\"b\\\" Const(Int))))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"a\" UChar) member_decl(\"b\" Int)]))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.l\" InitList([Constant(ConstInt(7) None) Constant(ConstInt(8) None)] None None) Const(Structure(\"s\")) None None None None false)))\n"
      "    S(Return(Dot(Var(\"@0.l\" None) \"b\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  StructDecl(struct_decl(\\\"s\\\" false [member_decl(\\\"a\\\" UChar) member_decl(\\\"b\\\" Int)]))\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.l\\\" InitList([Cast(UChar Constant(ConstInt(7) Int) UChar) Constant(ConstInt(8) Int)] Structure(\\\"s\\\") None) Const(Structure(\\\"s\\\")) None None None None false)))\\n    S(Return(Dot(Var(\\\"@0.l\\\" Const(Structure(\\\"s\\\"))) \\\"b\\\" Const(Int))))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.l\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Const(Structure(\\\"s\\\"))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": [[\"s\", {\"object\": \"passes.type_checking.StructLayout\", \"fields\": {\"tag\": \"s\", \"is_union\": false, \"members\": [{\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"a\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"byte_offset\": 0}}, {\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"b\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"byte_offset\": 1}}], \"size\": 3, \"complete\": true}}]]}}}]}\n");
}

TEST_CASE("c99_to_tac: test_c99_to_tac.py::TestQualifiedTypeLowering::test_const_struct_local_stores_each_member (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass c99_to_tac\n"
      "source tests/test_c99_to_tac.py::TestQualifiedTypeLowering::test_const_struct_local_stores_each_member\n"
      "args {\"args\": [{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.l\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Const(Structure(\\\"s\\\"))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": [[\"s\", {\"object\": \"passes.type_checking.StructLayout\", \"fields\": {\"tag\": \"s\", \"is_union\": false, \"members\": [{\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"a\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"byte_offset\": 0}}, {\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"b\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"byte_offset\": 1}}], \"size\": 3, \"complete\": true}}]]}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"args\": {\"0\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.l\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Const(Structure(\\\"s\\\"))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%3\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Structure(\\\"s\\\"))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%4\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%5\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"a\" UChar) member_decl(\"b\" Int)]))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.l\" InitList([Cast(UChar Constant(ConstInt(7) Int) UChar) Constant(ConstInt(8) Int)] Structure(\"s\") None) Const(Structure(\"s\")) None None None None false)))\n"
      "    S(Return(Dot(Var(\"@0.l\" Const(Structure(\"s\"))) \"b\" Const(Int))))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Truncate(Constant(ConstInt(7)) Var(\"%0\"))\n"
      "    GetAddress(Var(\"@0.l\") Var(\"%1\"))\n"
      "    Store(Var(\"%0\") Var(\"%1\") false)\n"
      "    Binary(Add Var(\"%1\") Constant(ConstInt(1)) Var(\"%2\"))\n"
      "    Store(Constant(ConstInt(8)) Var(\"%2\") false)\n"
      "    GetAddress(Var(\"@0.l\") Var(\"%3\"))\n"
      "    Binary(Add Var(\"%3\") Constant(ConstInt(1)) Var(\"%4\"))\n"
      "    Load(Var(\"%4\") Var(\"%5\") false)\n"
      "    Ret(Var(\"%5\"))\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("type_checking: test_c99_to_tac.py::TestQualifiedTypeLowering::test_const_struct_static_lays_out_its_members (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c99_to_tac.py::TestQualifiedTypeLowering::test_const_struct_static_lays_out_its_members\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  StructDecl(struct_decl(\\\"s\\\" false [member_decl(\\\"a\\\" UChar) member_decl(\\\"b\\\" Int)]))\\n  VarDecl(var_decl(\\\"g\\\" InitList([InitList([Cast(UChar Constant(ConstInt(1) Int) UChar) Constant(ConstInt(2) Int)] Structure(\\\"s\\\") None) InitList([Cast(UChar Constant(ConstInt(3) Int) UChar) Constant(ConstInt(4) Int)] Structure(\\\"s\\\") None)] Array(Const(Structure(\\\"s\\\")) 2) None) Array(Const(Structure(\\\"s\\\")) 2) None None None None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"a\" UChar) member_decl(\"b\" Int)]))\n"
      "  VarDecl(var_decl(\"g\" InitList([InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) InitList([Constant(ConstInt(3) None) Constant(ConstInt(4) None)] None None)] None None) Array(Const(Structure(\"s\")) 2) None None None None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  StructDecl(struct_decl(\\\"s\\\" false [member_decl(\\\"a\\\" UChar) member_decl(\\\"b\\\" Int)]))\\n  VarDecl(var_decl(\\\"g\\\" InitList([InitList([Cast(UChar Constant(ConstInt(1) Int) UChar) Constant(ConstInt(2) Int)] Structure(\\\"s\\\") None) InitList([Cast(UChar Constant(ConstInt(3) Int) UChar) Constant(ConstInt(4) Int)] Structure(\\\"s\\\") None)] Array(Const(Structure(\\\"s\\\")) 2) None) Array(Const(Structure(\\\"s\\\")) 2) None None None None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"g\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Const(Structure(\\\"s\\\")) 2)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": {\"tuple\": [{\"tuple\": [1, 2]}, {\"tuple\": [3, 4]}]}}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": [[\"s\", {\"object\": \"passes.type_checking.StructLayout\", \"fields\": {\"tag\": \"s\", \"is_union\": false, \"members\": [{\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"a\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"byte_offset\": 0}}, {\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"b\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"byte_offset\": 1}}], \"size\": 3, \"complete\": true}}]]}}}]}\n");
}

TEST_CASE("c99_to_tac: test_c99_to_tac.py::TestQualifiedTypeLowering::test_const_struct_static_lays_out_its_members (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass c99_to_tac\n"
      "source tests/test_c99_to_tac.py::TestQualifiedTypeLowering::test_const_struct_static_lays_out_its_members\n"
      "args {\"args\": [{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"g\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Const(Structure(\\\"s\\\")) 2)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": {\"tuple\": [{\"tuple\": [1, 2]}, {\"tuple\": [3, 4]}]}}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": [[\"s\", {\"object\": \"passes.type_checking.StructLayout\", \"fields\": {\"tag\": \"s\", \"is_union\": false, \"members\": [{\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"a\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"byte_offset\": 0}}, {\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"b\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"byte_offset\": 1}}], \"size\": 3, \"complete\": true}}]]}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"a\" UChar) member_decl(\"b\" Int)]))\n"
      "  VarDecl(var_decl(\"g\" InitList([InitList([Cast(UChar Constant(ConstInt(1) Int) UChar) Constant(ConstInt(2) Int)] Structure(\"s\") None) InitList([Cast(UChar Constant(ConstInt(3) Int) UChar) Constant(ConstInt(4) Int)] Structure(\"s\") None)] Array(Const(Structure(\"s\")) 2) None) Array(Const(Structure(\"s\")) 2) None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  StaticVariable(\"g\" true Pointer [UCharInit(1) IntInit(2) UCharInit(3) IntInit(4)])\n"
      "])\n");
}

TEST_CASE("type_checking: test_c99_to_tac.py::TestQualifiedTypeLowering::test_member_array_of_const_struct (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c99_to_tac.py::TestQualifiedTypeLowering::test_member_array_of_const_struct\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  StructDecl(struct_decl(\\\"s\\\" false [member_decl(\\\"x\\\" UChar) member_decl(\\\"a\\\" Array(UChar 3))]))\\n  FunctionDecl(function_decl(\\\"f\\\" [\\\"@0.p\\\"] Block([\\n    S(Return(Subscript(AddressOf(Arrow(Var(\\\"@0.p\\\" Pointer(Const(Structure(\\\"s\\\")))) \\\"a\\\" Array(Const(UChar) 3)) Pointer(Const(UChar))) Constant(ConstInt(2) Int) Const(UChar))))\\n  ]) FunType([Pointer(Const(Structure(\\\"s\\\")))] UChar) None None [\\\"\\\"] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"x\" UChar) member_decl(\"a\" Array(UChar 3))]))\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.p\"] Block([\n"
      "    S(Return(Subscript(Arrow(Var(\"@0.p\" None) \"a\" None) Constant(ConstInt(2) None) None)))\n"
      "  ]) FunType([Pointer(Const(Structure(\"s\")))] UChar) None None [\"\"] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  StructDecl(struct_decl(\\\"s\\\" false [member_decl(\\\"x\\\" UChar) member_decl(\\\"a\\\" Array(UChar 3))]))\\n  FunctionDecl(function_decl(\\\"f\\\" [\\\"@0.p\\\"] Block([\\n    S(Return(Subscript(AddressOf(Arrow(Var(\\\"@0.p\\\" Pointer(Const(Structure(\\\"s\\\")))) \\\"a\\\" Array(Const(UChar) 3)) Pointer(Const(UChar))) Constant(ConstInt(2) Int) Const(UChar))))\\n  ]) FunType([Pointer(Const(Structure(\\\"s\\\")))] UChar) None None [\\\"\\\"] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([Pointer(Const(Structure(\\\"s\\\")))] UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Const(Structure(\\\"s\\\")))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": [[\"s\", {\"object\": \"passes.type_checking.StructLayout\", \"fields\": {\"tag\": \"s\", \"is_union\": false, \"members\": [{\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"x\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"byte_offset\": 0}}, {\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"a\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(UChar 3)\\n\"}, \"byte_offset\": 1}}], \"size\": 4, \"complete\": true}}]]}}}]}\n");
}

TEST_CASE("c99_to_tac: test_c99_to_tac.py::TestQualifiedTypeLowering::test_member_array_of_const_struct (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass c99_to_tac\n"
      "source tests/test_c99_to_tac.py::TestQualifiedTypeLowering::test_member_array_of_const_struct\n"
      "args {\"args\": [{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([Pointer(Const(Structure(\\\"s\\\")))] UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Const(Structure(\\\"s\\\")))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": [[\"s\", {\"object\": \"passes.type_checking.StructLayout\", \"fields\": {\"tag\": \"s\", \"is_union\": false, \"members\": [{\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"x\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"byte_offset\": 0}}, {\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"a\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(UChar 3)\\n\"}, \"byte_offset\": 1}}], \"size\": 4, \"complete\": true}}]]}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"args\": {\"0\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([Pointer(Const(Structure(\\\"s\\\")))] UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Const(Structure(\\\"s\\\")))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Array(UChar 3))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"x\" UChar) member_decl(\"a\" Array(UChar 3))]))\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.p\"] Block([\n"
      "    S(Return(Subscript(AddressOf(Arrow(Var(\"@0.p\" Pointer(Const(Structure(\"s\")))) \"a\" Array(Const(UChar) 3)) Pointer(Const(UChar))) Constant(ConstInt(2) Int) Const(UChar))))\n"
      "  ]) FunType([Pointer(Const(Structure(\"s\")))] UChar) None None [\"\"] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" true [\"@0.p\"] [\n"
      "    Binary(Add Var(\"@0.p\") Constant(ConstInt(1)) Var(\"%0\"))\n"
      "    Binary(Add Var(\"%0\") Constant(ConstInt(2)) Var(\"%1\"))\n"
      "    Load(Var(\"%1\") Var(\"%2\") false)\n"
      "    Ret(Var(\"%2\"))\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("type_checking: test_c99_to_tac.py::TestQualifiedTypeLowering::test_member_of_const_struct (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c99_to_tac.py::TestQualifiedTypeLowering::test_member_of_const_struct\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  StructDecl(struct_decl(\\\"s\\\" false [member_decl(\\\"a\\\" UChar) member_decl(\\\"b\\\" UChar)]))\\n  FunctionDecl(function_decl(\\\"f\\\" [\\\"@0.p\\\"] Block([\\n    S(Return(Arrow(Var(\\\"@0.p\\\" Pointer(Const(Structure(\\\"s\\\")))) \\\"b\\\" Const(UChar))))\\n  ]) FunType([Pointer(Const(Structure(\\\"s\\\")))] UChar) None None [\\\"\\\"] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"a\" UChar) member_decl(\"b\" UChar)]))\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.p\"] Block([\n"
      "    S(Return(Arrow(Var(\"@0.p\" None) \"b\" None)))\n"
      "  ]) FunType([Pointer(Const(Structure(\"s\")))] UChar) None None [\"\"] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  StructDecl(struct_decl(\\\"s\\\" false [member_decl(\\\"a\\\" UChar) member_decl(\\\"b\\\" UChar)]))\\n  FunctionDecl(function_decl(\\\"f\\\" [\\\"@0.p\\\"] Block([\\n    S(Return(Arrow(Var(\\\"@0.p\\\" Pointer(Const(Structure(\\\"s\\\")))) \\\"b\\\" Const(UChar))))\\n  ]) FunType([Pointer(Const(Structure(\\\"s\\\")))] UChar) None None [\\\"\\\"] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([Pointer(Const(Structure(\\\"s\\\")))] UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Const(Structure(\\\"s\\\")))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": [[\"s\", {\"object\": \"passes.type_checking.StructLayout\", \"fields\": {\"tag\": \"s\", \"is_union\": false, \"members\": [{\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"a\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"byte_offset\": 0}}, {\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"b\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"byte_offset\": 1}}], \"size\": 2, \"complete\": true}}]]}}}]}\n");
}

TEST_CASE("c99_to_tac: test_c99_to_tac.py::TestQualifiedTypeLowering::test_member_of_const_struct (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass c99_to_tac\n"
      "source tests/test_c99_to_tac.py::TestQualifiedTypeLowering::test_member_of_const_struct\n"
      "args {\"args\": [{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([Pointer(Const(Structure(\\\"s\\\")))] UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Const(Structure(\\\"s\\\")))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": [[\"s\", {\"object\": \"passes.type_checking.StructLayout\", \"fields\": {\"tag\": \"s\", \"is_union\": false, \"members\": [{\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"a\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"byte_offset\": 0}}, {\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"b\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"byte_offset\": 1}}], \"size\": 2, \"complete\": true}}]]}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"args\": {\"0\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([Pointer(Const(Structure(\\\"s\\\")))] UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Const(Structure(\\\"s\\\")))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"a\" UChar) member_decl(\"b\" UChar)]))\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.p\"] Block([\n"
      "    S(Return(Arrow(Var(\"@0.p\" Pointer(Const(Structure(\"s\")))) \"b\" Const(UChar))))\n"
      "  ]) FunType([Pointer(Const(Structure(\"s\")))] UChar) None None [\"\"] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" true [\"@0.p\"] [\n"
      "    Binary(Add Var(\"@0.p\") Constant(ConstInt(1)) Var(\"%0\"))\n"
      "    Load(Var(\"%0\") Var(\"%1\") false)\n"
      "    Ret(Var(\"%1\"))\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("type_checking: test_c99_to_tac.py::TestQualifiedTypeLowering::test_volatile_pointer_increment_steps_by_the_element_size (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c99_to_tac.py::TestQualifiedTypeLowering::test_volatile_pointer_increment_steps_by_the_element_size\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"arr\\\" None Array(Int 4) None None None None false))\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.p\\\" AddressOf(Var(\\\"arr\\\" Array(Int 4)) Pointer(Int)) Volatile(Pointer(Int)) None None None None false)))\\n    S(Expression(Postfix(Increment Var(\\\"@0.p\\\" Volatile(Pointer(Int))) Volatile(Pointer(Int)))))\\n    S(Return(Var(\\\"@0.p\\\" Volatile(Pointer(Int)))))\\n  ]) FunType([] Pointer(Int)) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"arr\" None Array(Int 4) None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.p\" Var(\"arr\" None) Volatile(Pointer(Int)) None None None None false)))\n"
      "    S(Expression(Postfix(Increment Var(\"@0.p\" None) None)))\n"
      "    S(Return(Var(\"@0.p\" None)))\n"
      "  ]) FunType([] Pointer(Int)) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"arr\\\" None Array(Int 4) None None None None false))\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.p\\\" AddressOf(Var(\\\"arr\\\" Array(Int 4)) Pointer(Int)) Volatile(Pointer(Int)) None None None None false)))\\n    S(Expression(Postfix(Increment Var(\\\"@0.p\\\" Volatile(Pointer(Int))) Volatile(Pointer(Int)))))\\n    S(Return(Var(\\\"@0.p\\\" Volatile(Pointer(Int)))))\\n  ]) FunType([] Pointer(Int)) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"arr\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Int 4)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Pointer(Int))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Volatile(Pointer(Int))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("c99_to_tac: test_c99_to_tac.py::TestQualifiedTypeLowering::test_volatile_pointer_increment_steps_by_the_element_size (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass c99_to_tac\n"
      "source tests/test_c99_to_tac.py::TestQualifiedTypeLowering::test_volatile_pointer_increment_steps_by_the_element_size\n"
      "args {\"args\": [{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"arr\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Int 4)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Pointer(Int))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Volatile(Pointer(Int))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"args\": {\"0\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"arr\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Int 4)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Pointer(Int))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Volatile(Pointer(Int))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%3\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%4\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"arr\" None Array(Int 4) None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.p\" AddressOf(Var(\"arr\" Array(Int 4)) Pointer(Int)) Volatile(Pointer(Int)) None None None None false)))\n"
      "    S(Expression(Postfix(Increment Var(\"@0.p\" Volatile(Pointer(Int))) Volatile(Pointer(Int)))))\n"
      "    S(Return(Var(\"@0.p\" Volatile(Pointer(Int)))))\n"
      "  ]) FunType([] Pointer(Int)) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    GetAddress(Var(\"arr\") Var(\"%0\"))\n"
      "    IndexedSymbolStore(\"@0.p\" Constant(ConstUChar(0)) Var(\"%0\") true 0)\n"
      "    IndexedLoad(\"@0.p\" Constant(ConstUChar(0)) Var(\"%1\") true 0 0)\n"
      "    Binary(Multiply Constant(ConstInt(1)) Constant(ConstInt(2)) Var(\"%2\"))\n"
      "    Binary(Add Var(\"%1\") Var(\"%2\") Var(\"%3\"))\n"
      "    IndexedSymbolStore(\"@0.p\" Constant(ConstUChar(0)) Var(\"%3\") true 0)\n"
      "    IndexedLoad(\"@0.p\" Constant(ConstUChar(0)) Var(\"%4\") true 0 0)\n"
      "    Ret(Var(\"%4\"))\n"
      "  ])\n"
      "  StaticVariable(\"arr\" true Pointer [ZeroInit(8)])\n"
      "])\n");
}
