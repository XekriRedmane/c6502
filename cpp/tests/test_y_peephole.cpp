// The calls that the unit tests of tests/test_y_peephole.py make of
// passes/y_peephole.py's `apply_y_peephole`, each with what the test gives the function
// and what the Python function returned or raised, which the C++
// port must return or throw too. They were recorded by wrapping the
// Python functions while their unit tests ran.
#include "doctest.h"
#include "call_case.h"

TEST_CASE("y_peephole: TestYPeephole.test_blank_and_comment_lines_pass_through (call 1)") {
  c6502::testing::check_y_peephole(
      {
        "",
        "   ; a comment",
        "   LDY   #$00",
        "   LDA   (DPTR),Y",
        "",
        "   ; another comment",
        "   LDY   #$00",
      },
      {
        "",
        "   ; a comment",
        "   LDY   #$00",
        "   LDA   (DPTR),Y",
        "",
        "   ; another comment",
      });
}

TEST_CASE("y_peephole: TestYPeephole.test_drop_redundant_ldy (call 1)") {
  c6502::testing::check_y_peephole(
      {
        "   LDY   #$00",
        "   LDA   (DPTR),Y",
        "   LDY   #$00",
        "   LDA   (DPTR),Y",
      },
      {
        "   LDY   #$00",
        "   LDA   (DPTR),Y",
        "   LDA   (DPTR),Y",
      });
}

TEST_CASE("y_peephole: TestYPeephole.test_iny_dey_update_tracker (call 1)") {
  c6502::testing::check_y_peephole(
      {
        "   LDY   #$00",
        "   LDA   (DPTR),Y",
        "   INY",
        "   LDA   (DPTR),Y",
        "   LDY   #$01",
        "   LDA   (DPTR),Y",
      },
      {
        "   LDY   #$00",
        "   LDA   (DPTR),Y",
        "   INY",
        "   LDA   (DPTR),Y",
        "   LDA   (DPTR),Y",
      });
}

TEST_CASE("y_peephole: TestYPeephole.test_iny_underflow_unknown (call 1)") {
  c6502::testing::check_y_peephole(
      {
        "   INY",
        "   LDY   #$05",
      },
      {
        "   INY",
        "   LDY   #$05",
      });
}

TEST_CASE("y_peephole: TestYPeephole.test_jsr_resets_state (call 1)") {
  c6502::testing::check_y_peephole(
      {
        "   LDY   #$00",
        "   JSR   somefn",
        "   LDY   #$00",
      },
      {
        "   LDY   #$00",
        "   JSR   somefn",
        "   LDY   #$00",
      });
}

TEST_CASE("y_peephole: TestYPeephole.test_label_resets_state (call 1)") {
  c6502::testing::check_y_peephole(
      {
        "   LDY   #$00",
        "   LDA   (DPTR),Y",
        ".loop:",
        "   LDY   #$00",
        "   LDA   (DPTR),Y",
      },
      {
        "   LDY   #$00",
        "   LDA   (DPTR),Y",
        ".loop:",
        "   LDY   #$00",
        "   LDA   (DPTR),Y",
      });
}

TEST_CASE("y_peephole: TestYPeephole.test_modulo_arithmetic (call 1)") {
  c6502::testing::check_y_peephole(
      {
        "   LDY   #$00",
        "   LDY   #$FF",
      },
      {
        "   LDY   #$00",
        "   DEY",
      });
}

TEST_CASE("y_peephole: TestYPeephole.test_replace_with_dey (call 1)") {
  c6502::testing::check_y_peephole(
      {
        "   LDY   #$01",
        "   LDA   (DPTR),Y",
        "   LDY   #$00",
        "   LDA   (DPTR),Y",
      },
      {
        "   LDY   #$01",
        "   LDA   (DPTR),Y",
        "   DEY",
        "   LDA   (DPTR),Y",
      });
}

TEST_CASE("y_peephole: TestYPeephole.test_replace_with_iny (call 1)") {
  c6502::testing::check_y_peephole(
      {
        "   LDY   #$00",
        "   LDA   (DPTR),Y",
        "   LDY   #$01",
        "   LDA   (DPTR),Y",
      },
      {
        "   LDY   #$00",
        "   LDA   (DPTR),Y",
        "   INY",
        "   LDA   (DPTR),Y",
      });
}

TEST_CASE("y_peephole: TestYPeephole.test_tay_invalidates (call 1)") {
  c6502::testing::check_y_peephole(
      {
        "   LDY   #$00",
        "   TAY",
        "   LDY   #$00",
      },
      {
        "   LDY   #$00",
        "   TAY",
        "   LDY   #$00",
      });
}
