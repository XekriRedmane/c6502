// Tests of the `c6502cpp backend` command's requests and responses
// (src/backend.h). The expected texts are Python's for compiles of this
// program with `-S` and with `-S -O`:
//
//     static unsigned char add1(unsigned char x) { return x + 1; }
//     int main(void) { return add1(2); }
//
// Under `-O`, `add1` is inlined into `main` and dropped.
#include <stdexcept>
#include <string>

#include "backend.h"
#include "doctest.h"

namespace {

const char kSource[] =
    "static unsigned char add1(unsigned char x) { return x + 1; }\n"
    "int main(void) { return add1(2); }\n";

// Returns the request cpp_backend.py sends for `source`, read from
// stdin, in `mode` with the JSON values `optimize`, `inline`, `check_ir`,
// and `preprocess`.
std::string request(const std::string& source, bool optimize, const std::string& mode = "source-text",
                    bool inline_calls = true, bool check_ir = false, bool preprocess = true) {
  const auto b = [](bool v) { return std::string(v ? "true" : "false"); };
  const std::string args = R"({"pcpp_args": [], "filename": null, "include_dir": "include", )"
                           R"("preprocess": )" + b(preprocess) +
                           R"(, "optimize": )" + b(optimize) +
                           R"(, "inline": )" + b(inline_calls) + R"(, "check_ir": )" +
                           b(check_ir) + R"(, "link_metadata": false})";
  return "c6502-backend 3\nmode " + mode + "\nargs " + args + "\nsource " +
         std::to_string(source.size()) + "\n" + source;
}

const char kUnoptimizedResponse[] = R"req(stderr 0
warnings []
text 1675
SSP	EQU	$00
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

const char kOptimizedResponse[] = R"req(stderr 0
warnings []
text 340
SSP	EQU	$00
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

// The response of `-S -O` for a program that declares a parameter
// without a name, whose slot is spelled by its position,
// `__zpabi_put__0`.
const char kUnnamedSource[] =
    "[[c6502::zp_abi]] unsigned char put(unsigned char);\n"
    "unsigned char f(void) { return put(3); }\n";

const char kUnnamedResponse[] = R"req(stderr 0
warnings []
text 349
SSP	EQU	$00
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

TEST_CASE("backend answers an unoptimized request with the text") {
  CHECK(c6502::backend::respond(request(kSource, false)) == kUnoptimizedResponse);
}

TEST_CASE("backend answers an optimized request with the text") {
  CHECK(c6502::backend::respond(request(kSource, true)) == kOptimizedResponse);
  CHECK(std::string(kOptimizedResponse).find("\nadd1:") == std::string::npos);
}

TEST_CASE("backend takes text the preprocessor has already read") {
  CHECK(c6502::backend::respond(request(kSource, true, "source-text", true, false, false)) ==
        kOptimizedResponse);
}

TEST_CASE("backend leaves the calls alone when the request turns inlining off") {
  const std::string got = c6502::backend::respond(request(kSource, true, "source-text", false));
  CHECK(got.find("\nadd1:") != std::string::npos);
  CHECK(got.find("JSR   add1") != std::string::npos);
}

TEST_CASE("backend spells an unnamed parameter's slot by its position") {
  const std::string got = c6502::backend::respond(request(kUnnamedSource, true));
  CHECK(got == kUnnamedResponse);
  CHECK(got.find("__zpabi_put__0") != std::string::npos);
}

TEST_CASE("backend answers a request for the harness with the program") {
  const std::string got = c6502::backend::respond(request(kSource, true, "source"));
  CHECK(got.starts_with("stderr 0\nwarnings []\nsymbols ["));
  CHECK(got.find("\nasm ") != std::string::npos);
  CHECK(got.find("Function(\"main\"") != std::string::npos);
  CHECK(got.find("\ntext ") == std::string::npos);
}

TEST_CASE("backend gives the TAC and the programs with the IR checks on") {
  const std::string got =
      c6502::backend::respond(request(kSource, true, "source-text", true, true));
  CHECK(got.starts_with("stderr 0\nwarnings []\ntac "));
  CHECK(got.find("\nasm ") != std::string::npos);
  CHECK(got.find("\nasm2 ") != std::string::npos);
  CHECK(got.find("\ntext ") != std::string::npos);
}

TEST_CASE("backend gives the warnings and the error of a rejected program") {
  // The preprocessor warns of `_Bool`, and identifier resolution
  // rejects `c`.
  const std::string got =
      c6502::backend::respond(request("_Bool b;\nint main(void) { return c; }\n", false));
  CHECK(got.starts_with("stderr 0\nwarnings [[\"line 1\", \"`_Bool` is deprecated: "));
  CHECK(got.ends_with("]]\nerror {\"type\": \"IdentifierResolutionError\", \"message\": "
                      "\"undeclared identifier 'c'\"}\n"));
}

TEST_CASE("backend gives what the preprocessor writes to stderr") {
  // pcpp reports an `#if` it can't evaluate and goes on.
  const std::string got = c6502::backend::respond(
      request("#if 1 +\n#endif\nint main(void) { return 0; }\n", false));
  CHECK(got.starts_with("stderr "));
  CHECK(got.find("error: Could not evaluate expression") != std::string::npos);
  CHECK(got.find("\ntext ") != std::string::npos);
}

TEST_CASE("backend rejects a request it can't read") {
  CHECK_THROWS_AS(c6502::backend::respond(request(kSource, true, "codegen-text")),
                  std::runtime_error);
  CHECK_THROWS_AS(c6502::backend::respond("c6502-backend 2\n"), std::runtime_error);
  CHECK_THROWS_AS(c6502::backend::respond("c6502-backend 3\nmode source\nargs {}\n"),
                  std::runtime_error);
}

TEST_CASE("backend answers a request for --lex with the tokens") {
  // Python's `compile.py --lex` prints these lines for the program.
  const std::string got = c6502::backend::respond(
      request("int x = 0x1F; /* c */\nchar *s = \"a b\";\n", false, "lex"));
  const std::string tokens =
      "1:1\tkeyword\tint\n1:5\tidentifier\tx\n1:7\tsymbol\t=\n1:9\tconstant\t0x1F\n"
      "1:13\tsymbol\t;\n2:1\tkeyword\tchar\n2:6\tsymbol\t*\n2:7\tidentifier\ts\n"
      "2:9\tsymbol\t=\n2:11\tstring-literal\t\"a b\"\n2:16\tsymbol\t;\n";
  CHECK(got == "stderr 0\nwarnings []\ntext " + std::to_string(tokens.size()) + "\n" + tokens);
}

TEST_CASE("backend lexes text the preprocessor has already read") {
  // Without the preprocessor, a `#define` line reaches the lexer, which
  // takes the `#` as a symbol, and Python's lexer gives these lines.
  const std::string got = c6502::backend::respond(
      request("#define N 1\nint\n", false, "lex", true, false, false));
  const std::string tokens =
      "1:1\tsymbol\t#\n1:2\tidentifier\tdefine\n1:9\tidentifier\tN\n1:11\tconstant\t1\n"
      "2:1\tkeyword\tint\n";
  CHECK(got == "stderr 0\nwarnings []\ntext " + std::to_string(tokens.size()) + "\n" + tokens);
}

TEST_CASE("backend gives the error of a character the lexer has no token for") {
  const std::string got = c6502::backend::respond(request("int a = 1;\n  @\n", false, "lex"));
  CHECK(got == "stderr 0\nwarnings []\nerror {\"type\": \"LexError\", \"message\": "
               "\"2:3: unexpected character '@'\"}\n");
}

TEST_CASE("backend gives the warnings and the error of --lex") {
  const std::string got =
      c6502::backend::respond(request("_Bool b;\nint x = 1';\n", false, "lex"));
  CHECK(got.starts_with("stderr 0\nwarnings [[\"line 1\", \"`_Bool` is deprecated: "));
  CHECK(got.ends_with("]]\nerror {\"type\": \"LexError\", \"message\": \"2:9: a digit "
                      "separator has to be between two digits, and the one after '1' in "
                      "\\\"1'\\\" isn't followed by a digit\"}\n"));
}
