#include <string>

#include "compile_case.h"
#include "doctest.h"
#include "passes/ir_check.h"

namespace {

namespace suite = c6502::suite;
using suite::Stage;

const char kSource[] =
    "static unsigned char add1(unsigned char x) { return x + 1; }\n"
    "int main(void) { return add1(2); }\n";

suite::Compiled compiled(Stage stage, const std::string& source, bool optimize,
                         bool inline_calls = true, bool check_ir = false, bool preprocess = true) {
  suite::Options options;
  options.optimize = optimize;
  options.inline_calls = inline_calls;
  options.check_ir = check_ir;
  options.preprocess = preprocess;
  return suite::compile(stage, source, options);
}

bool gives(const suite::Compiled& c, const std::string& text) {
  return !c.error && c.err.empty() && c.warnings.empty() && c.text == text;
}

const char kUnoptimized[] = R"req(SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

add1:
   SUBROUTINE

   ; prologue: 1 arg bytes, 5 local bytes
   SEC
   LDA   SSP
   SBC   #$07
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$06
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   INY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$03
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$04
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$05
   STA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$08
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   INY
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   PLA
   RTS

main:
   SUBROUTINE

   ; prologue: 0 arg bytes, 4 local bytes
   SEC
   LDA   SSP
   SBC   #$06
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$05
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   #$02
   LDY   #$01
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$01
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   (FP),Y
   STA   (SSP),Y
   JSR   add1
   LDY   #$02
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   TAX
   DEY
   LDA   (FP),Y

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$06
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$05
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   PLA
   TAX
   PLA
   RTS
)req";

const char kOptimized[] = R"req(SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

; @zp-link-meta-begin
; def main params= locals= indirect=false in_cycle=false
; ext add1 params=__zpabi_add1__x param_regs=A
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   LDX   #$00
   LDA   #$03
   RTS
)req";

const char kUnnamedSource[] =
    "[[c6502::zp_abi]] unsigned char put(unsigned char);\n"
    "unsigned char f(void) { return put(3); }\n";

const char kUnnamed[] = R"req(SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

; @zp-link-meta-begin
; def f params= locals=__local_f__0 indirect=false in_cycle=false
; ext put params=__zpabi_put__0 param_regs=A
; call f -> put
; item f section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

f:
   SUBROUTINE

   LDA   #$03
   JMP   put
)req";

}  // namespace

TEST_CASE("compile stages: the -S text without -O") {
  CHECK(gives(compiled(Stage::kCodegen, kSource, false), kUnoptimized));
}

TEST_CASE("compile stages: the -S text with -O") {
  CHECK(gives(compiled(Stage::kCodegen, kSource, true), kOptimized));
  CHECK(std::string(kOptimized).find("\nadd1:") == std::string::npos);
}

TEST_CASE("compile stages: text the preprocessor has already read") {
  CHECK(gives(compiled(Stage::kCodegen, kSource, true, true, false, false), kOptimized));
}

TEST_CASE("compile stages: the calls stay when inlining is off") {
  const suite::Compiled c = compiled(Stage::kCodegen, kSource, true, false);
  CHECK(c.text.find("\nadd1:") != std::string::npos);
  CHECK(c.text.find("JSR   add1") != std::string::npos);
}

TEST_CASE("compile stages: an unnamed parameter's slot is spelled by its position") {
  const suite::Compiled c = compiled(Stage::kCodegen, kUnnamedSource, true);
  CHECK(gives(c, kUnnamed));
  CHECK(c.text.find("__zpabi_put__0") != std::string::npos);
}

TEST_CASE("compile stages: the IR checks leave the text as it is") {
  const suite::Compiled c = compiled(Stage::kCodegen, kSource, true, true, true);
  CHECK(gives(c, kOptimized));
  CHECK_FALSE(c6502::passes::ir_check::enabled());
}

TEST_CASE("compile stages: the warnings and the error of a rejected program") {
  const suite::Compiled c =
      compiled(Stage::kCodegen, "_Bool b;\nint main(void) { return c; }\n", false);
  REQUIRE(c.warnings.size() == 1);
  CHECK(c.warnings[0].where == "line 1");
  CHECK(c.warnings[0].message.starts_with("`_Bool` is deprecated: "));
  REQUIRE(c.error);
  CHECK(c.error->type == "IdentifierResolutionError");
  CHECK(c.error->message == "undeclared identifier 'c'");
}

TEST_CASE("compile stages: what the preprocessor writes to stderr") {
  const suite::Compiled c =
      compiled(Stage::kCodegen, "#if 1 +\n#endif\nint main(void) { return 0; }\n", false);
  CHECK(c.err.find("error: Could not evaluate expression") != std::string::npos);
  CHECK_FALSE(c.error);
  CHECK(c.text.find("\nmain:") != std::string::npos);
}

TEST_CASE("compile stages: --lex gives the tokens") {
  const std::string tokens =
      "1:1\tkeyword\tint\n1:5\tidentifier\tx\n1:7\tsymbol\t=\n1:9\tconstant\t0x1F\n"
      "1:13\tsymbol\t;\n2:1\tkeyword\tchar\n2:6\tsymbol\t*\n2:7\tidentifier\ts\n"
      "2:9\tsymbol\t=\n2:11\tstring-literal\t\"a b\"\n2:16\tsymbol\t;\n";
  CHECK(gives(compiled(Stage::kLex, "int x = 0x1F; /* c */\nchar *s = \"a b\";\n", false),
              tokens));
}

TEST_CASE("compile stages: --lex of text the preprocessor has already read") {
  const std::string tokens =
      "1:1\tsymbol\t#\n1:2\tidentifier\tdefine\n1:9\tidentifier\tN\n1:11\tconstant\t1\n"
      "2:1\tkeyword\tint\n";
  CHECK(gives(compiled(Stage::kLex, "#define N 1\nint\n", false, true, false, false), tokens));
}

TEST_CASE("compile stages: --lex of a character the lexer has no token for") {
  const suite::Compiled c = compiled(Stage::kLex, "int a = 1;\n  @\n", false);
  REQUIRE(c.error);
  CHECK(c.error->type == "LexError");
  CHECK(c.error->message == "2:3: unexpected character '@'");
  CHECK(c.warnings.empty());
}

TEST_CASE("compile stages: the warnings and the error of --lex") {
  const suite::Compiled c = compiled(Stage::kLex, "_Bool b;\nint x = 1';\n", false);
  REQUIRE(c.warnings.size() == 1);
  CHECK(c.warnings[0].where == "line 1");
  CHECK(c.warnings[0].message.starts_with("`_Bool` is deprecated: "));
  REQUIRE(c.error);
  CHECK(c.error->type == "LexError");
  CHECK(c.error->message ==
        "2:9: a digit separator has to be between two digits, and the one after '1' in "
        "\"1'\" isn't followed by a digit");
}

TEST_CASE("compile stages: --parse gives pretty.py's tree") {
  const std::string tree =
      "Program(\n"
      "  declaration=[\n"
      "    VarDecl(\n"
      "      var_decl=Type_var_decl(\n"
      "        name='x',\n"
      "        init=Constant(\n"
      "          const=ConstInt(\n"
      "            value=1,\n"
      "          ),\n"
      "          data_type=None,\n"
      "        ),\n"
      "        data_type=Int(),\n"
      "        storage_class=None,\n"
      "        abi_annotation=None,\n"
      "        placement=None,\n"
      "        section=None,\n"
      "        is_constexpr=False,\n"
      "      ),\n"
      "    ),\n"
      "  ],\n"
      ")\n";
  CHECK(gives(compiled(Stage::kParse, "int x = 1;\n", false), tree));
}

TEST_CASE("compile stages: --resolve gives the error of the pass it stops after") {
  const std::string source = "int main(void) { return y; }\n";
  const suite::Compiled c = compiled(Stage::kResolve, source, false);
  REQUIRE(c.error);
  CHECK(c.error->type == "IdentifierResolutionError");
  CHECK(c.error->message == "undeclared identifier 'y'");
  CHECK_FALSE(compiled(Stage::kParse, source, false).error);
}

TEST_CASE("compile stages: --tac -O gives the optimized TAC") {
  const std::string tree =
      "Program(\n"
      "  top_level=[\n"
      "    Function(\n"
      "      name='main',\n"
      "      is_global=True,\n"
      "      params=[],\n"
      "      instructions=[\n"
      "        Ret(\n"
      "          val=Constant(\n"
      "            const=ConstInt(\n"
      "              value=3,\n"
      "            ),\n"
      "          ),\n"
      "        ),\n"
      "      ],\n"
      "    ),\n"
      "  ],\n"
      ")\n";
  CHECK(gives(compiled(Stage::kTac, "int main(void) { return 1 + 2; }\n", true, true, true),
              tree));
}
