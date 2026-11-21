// Tests of the `c6502cpp backend` command's requests and responses
// (src/backend.h). The requests are the ones cpp_backend.py sent for
// compiles of this program with `-S` and with `-S -O`, and the expected
// responses are the ones c6502cpp gave, whose text is Python's:
//
//     unsigned char add1(unsigned char x) { return x + 1; }
//     int main(void) { return add1(2); }
#include <stdexcept>
#include <string>

#include "backend.h"
#include "doctest.h"

namespace {

const char kUnoptimizedRequest[] = R"req(c6502-backend 1
mode stage-unoptimized-text
args {"symbols": {"object": "passes.type_checking.SymbolTable", "fields": {"_table": {"dict": [["add1", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "FunType([UChar] UChar)\n"}, "attrs": {"object": "passes.type_checking.FunAttr", "fields": {"defined": true, "is_global": true, "carry_return": false, "address": null, "preserves": {"set": []}, "section": null, "param_zeropage": {"tuple": []}, "returns_bool": false, "noreturn": false}}}}], ["@0.x", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "UChar\n"}, "attrs": {"object": "passes.type_checking.LocalAttr", "fields": {"register": false, "was_bool": false}}}}], ["main", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "FunType([] Int)\n"}, "attrs": {"object": "passes.type_checking.FunAttr", "fields": {"defined": true, "is_global": true, "carry_return": false, "address": null, "preserves": {"set": []}, "section": null, "param_zeropage": {"tuple": []}, "returns_bool": false, "noreturn": false}}}}], ["%0", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Int\n"}, "attrs": {"object": "passes.type_checking.LocalAttr", "fields": {"register": false, "was_bool": false}}}}], ["%1", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Int\n"}, "attrs": {"object": "passes.type_checking.LocalAttr", "fields": {"register": false, "was_bool": false}}}}], ["%2", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "UChar\n"}, "attrs": {"object": "passes.type_checking.LocalAttr", "fields": {"register": false, "was_bool": false}}}}], ["%3", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "UChar\n"}, "attrs": {"object": "passes.type_checking.LocalAttr", "fields": {"register": false, "was_bool": false}}}}], ["%4", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "UChar\n"}, "attrs": {"object": "passes.type_checking.LocalAttr", "fields": {"register": false, "was_bool": false}}}}], ["%5", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Int\n"}, "attrs": {"object": "passes.type_checking.LocalAttr", "fields": {"register": false, "was_bool": false}}}}]]}}}, "types": {"object": "passes.type_checking.TypeTable", "fields": {"_table": {"dict": []}}}, "statics": {"set": []}, "footprints": {"dict": [["mul8", 5], ["mul16", 10], ["mul32", 20], ["udivmod8", 4], ["sdivmod8", 6], ["udivmod16", 9], ["sdivmod16", 11], ["udivmod32", 20], ["sdivmod32", 22], ["asl8", 3], ["asr8", 3], ["lsr8", 3], ["asl16", 5], ["asr16", 5], ["lsr16", 5], ["asl32", 9], ["asr32", 9], ["lsr32", 9], ["bin16_to_bcd", 7]]}, "check_ir": false, "link_metadata": false}
c99 459
Program([
  FunctionDecl(function_decl("add1" ["@0.x"] Block([
    S(Return(Cast(UChar Binary(Add Cast(Int Var("@0.x" UChar) Int) Constant(ConstInt(1) Int) Int) UChar)))
  ]) FunType([UChar] UChar) None None [""] false None [] None false))
  FunctionDecl(function_decl("main" [] Block([
    S(Return(Cast(Int FunctionCall("add1" [Cast(UChar Constant(ConstInt(2) Int) UChar)] UChar false) Int)))
  ]) FunType([] Int) None None [] false None [] None false))
])
tac 394
Program([
  Function("add1" true ["@0.x"] [
    ZeroExtend(Var("@0.x") Var("%0"))
    Binary(Add Var("%0") Constant(ConstInt(1)) Var("%1"))
    Truncate(Var("%1") Var("%2"))
    Ret(Var("%2"))
  ])
  Function("main" true [] [
    Truncate(Constant(ConstInt(2)) Var("%3"))
    FunctionCall("add1" [Var("%3")] Var("%4") false false)
    ZeroExtend(Var("%4") Var("%5"))
    Ret(Var("%5"))
  ])
])
)req";

const char kUnoptimizedResponse[] = R"req(new_functions []
reg_locals []
carry []
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

const char kOptimizedRequest[] = R"req(c6502-backend 1
mode stage-optimized-text
args {"symbols": {"object": "passes.type_checking.SymbolTable", "fields": {"_table": {"dict": [["add1", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "FunType([UChar] UChar)\n"}, "attrs": {"object": "passes.type_checking.FunAttr", "fields": {"defined": true, "is_global": true, "carry_return": false, "address": null, "preserves": {"set": []}, "section": null, "param_zeropage": {"tuple": []}, "returns_bool": false, "noreturn": false}}}}], ["@0.x", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "UChar\n"}, "attrs": {"object": "passes.type_checking.LocalAttr", "fields": {"register": false, "was_bool": false}}}}], ["main", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "FunType([] Int)\n"}, "attrs": {"object": "passes.type_checking.FunAttr", "fields": {"defined": true, "is_global": true, "carry_return": false, "address": null, "preserves": {"set": []}, "section": null, "param_zeropage": {"tuple": []}, "returns_bool": false, "noreturn": false}}}}], ["%0", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Int\n"}, "attrs": {"object": "passes.type_checking.LocalAttr", "fields": {"register": false, "was_bool": false}}}}], ["%1", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Int\n"}, "attrs": {"object": "passes.type_checking.LocalAttr", "fields": {"register": false, "was_bool": false}}}}], ["%2", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "UChar\n"}, "attrs": {"object": "passes.type_checking.LocalAttr", "fields": {"register": false, "was_bool": false}}}}], ["%3", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "UChar\n"}, "attrs": {"object": "passes.type_checking.LocalAttr", "fields": {"register": false, "was_bool": false}}}}], ["%4", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "UChar\n"}, "attrs": {"object": "passes.type_checking.LocalAttr", "fields": {"register": false, "was_bool": false}}}}], ["%5", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Int\n"}, "attrs": {"object": "passes.type_checking.LocalAttr", "fields": {"register": false, "was_bool": false}}}}], ["%0.1", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Int\n"}, "attrs": {"object": "passes.type_checking.LocalAttr", "fields": {"register": false, "was_bool": false}}}}], ["%1.1", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Int\n"}, "attrs": {"object": "passes.type_checking.LocalAttr", "fields": {"register": false, "was_bool": false}}}}], ["%2.1", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "UChar\n"}, "attrs": {"object": "passes.type_checking.LocalAttr", "fields": {"register": false, "was_bool": false}}}}], ["%3.1", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "UChar\n"}, "attrs": {"object": "passes.type_checking.LocalAttr", "fields": {"register": false, "was_bool": false}}}}], ["%4.1", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "UChar\n"}, "attrs": {"object": "passes.type_checking.LocalAttr", "fields": {"register": false, "was_bool": false}}}}], ["%5.1", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Int\n"}, "attrs": {"object": "passes.type_checking.LocalAttr", "fields": {"register": false, "was_bool": false}}}}]]}}}, "types": {"object": "passes.type_checking.TypeTable", "fields": {"_table": {"dict": []}}}, "statics": {"set": []}, "footprints": {"dict": [["mul8", 5], ["mul16", 10], ["mul32", 20], ["udivmod8", 4], ["sdivmod8", 6], ["udivmod16", 9], ["sdivmod16", 11], ["udivmod32", 20], ["sdivmod32", 22], ["asl8", 3], ["asr8", 3], ["lsr8", 3], ["asl16", 5], ["asr16", 5], ["lsr16", 5], ["asl32", 9], ["asr32", 9], ["lsr32", 9], ["bin16_to_bcd", 7]]}, "check_ir": false}
c99 459
Program([
  FunctionDecl(function_decl("add1" ["@0.x"] Block([
    S(Return(Cast(UChar Binary(Add Cast(Int Var("@0.x" UChar) Int) Constant(ConstInt(1) Int) Int) UChar)))
  ]) FunType([UChar] UChar) None None [""] false None [] None false))
  FunctionDecl(function_decl("main" [] Block([
    S(Return(Cast(Int FunctionCall("add1" [Cast(UChar Constant(ConstInt(2) Int) UChar)] UChar false) Int)))
  ]) FunType([] Int) None None [] false None [] None false))
])
tac 394
Program([
  Function("add1" true ["@0.x"] [
    ZeroExtend(Var("@0.x") Var("%0"))
    Binary(Add Var("%0") Constant(ConstInt(1)) Var("%1"))
    Truncate(Var("%1") Var("%2"))
    Ret(Var("%2"))
  ])
  Function("main" true [] [
    Truncate(Constant(ConstInt(2)) Var("%3"))
    FunctionCall("add1" [Var("%3")] Var("%4") false false)
    ZeroExtend(Var("%4") Var("%5"))
    Ret(Var("%5"))
  ])
])
optimized-tac 306
Program([
  Function("add1" true ["@0.x"] [
    Binary(Add Var("@0.x") Constant(ConstUChar(1)) Var("%2.1"))
    Ret(Var("%2.1"))
  ])
  Function("main" true [] [
    FunctionCall("add1" [Constant(ConstUChar(2))] Var("%4.1") false false)
    ZeroExtend(Var("%4.1") Var("%5.1"))
    Ret(Var("%5.1"))
  ])
])
)req";

const char kOptimizedResponse[] = R"req(new_functions []
reg_locals ["@0.x.reg"]
carry []
text 541
SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

; @zp-link-meta-begin
; def add1 params=__zpabi_add1__x locals=__local_add1__x indirect=false in_cycle=false param_regs=A
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; call main -> add1
; item add1 section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

add1:
   SUBROUTINE

   CLC
   ADC   #$01
   RTS

main:
   SUBROUTINE

   LDA   #$02
   JSR   add1
   LDX   #$00
   RTS
)req";

// The request and response of `-S -O` for a program that declares a
// parameter without a name, which cpp_backend.py sends as the empty
// string and whose slot is spelled by its position, `__zpabi_put__0`:
//
//     [[c6502::zp_abi]] unsigned char put(unsigned char);
//     unsigned char f(void) { return put(3); }
const char kUnnamedRequest[] = R"req(c6502-backend 1
mode stage-optimized-text
args {"symbols": {"object": "passes.type_checking.SymbolTable", "fields": {"_table": {"dict": [["put", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "FunType([UChar] UChar)\n"}, "attrs": {"object": "passes.type_checking.FunAttr", "fields": {"defined": false, "is_global": true, "carry_return": false, "address": null, "preserves": {"set": []}, "section": null, "param_zeropage": {"tuple": []}, "returns_bool": false, "noreturn": false}}}}], ["f", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "FunType([] UChar)\n"}, "attrs": {"object": "passes.type_checking.FunAttr", "fields": {"defined": true, "is_global": true, "carry_return": false, "address": null, "preserves": {"set": []}, "section": null, "param_zeropage": {"tuple": []}, "returns_bool": false, "noreturn": false}}}}], ["%0", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "UChar\n"}, "attrs": {"object": "passes.type_checking.LocalAttr", "fields": {"register": false, "was_bool": false}}}}], ["%1", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "UChar\n"}, "attrs": {"object": "passes.type_checking.LocalAttr", "fields": {"register": false, "was_bool": false}}}}], ["%0.1", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "UChar\n"}, "attrs": {"object": "passes.type_checking.LocalAttr", "fields": {"register": false, "was_bool": false}}}}], ["%1.1", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "UChar\n"}, "attrs": {"object": "passes.type_checking.LocalAttr", "fields": {"register": false, "was_bool": false}}}}]]}}}, "types": {"object": "passes.type_checking.TypeTable", "fields": {"_table": {"dict": []}}}, "statics": {"set": []}, "footprints": {"dict": [["mul8", 5], ["mul16", 10], ["mul32", 20], ["udivmod8", 4], ["sdivmod8", 6], ["udivmod16", 9], ["sdivmod16", 11], ["udivmod32", 20], ["sdivmod32", 22], ["asl8", 3], ["asr8", 3], ["lsr8", 3], ["asl16", 5], ["asr16", 5], ["lsr16", 5], ["asl32", 9], ["asr32", 9], ["lsr32", 9], ["bin16_to_bcd", 7]]}, "check_ir": false}
c99 327
Program([
  FunctionDecl(function_decl("put" [""] None FunType([UChar] UChar) None "zp_abi" [""] false None [] None false))
  FunctionDecl(function_decl("f" [] Block([
    S(Return(FunctionCall("put" [Cast(UChar Constant(ConstInt(3) Int) UChar)] UChar false)))
  ]) FunType([] UChar) None None [] false None [] None false))
])
tac 166
Program([
  Function("f" true [] [
    Truncate(Constant(ConstInt(3)) Var("%0"))
    FunctionCall("put" [Var("%0")] Var("%1") false false)
    Ret(Var("%1"))
  ])
])
optimized-tac 138
Program([
  Function("f" true [] [
    FunctionCall("put" [Constant(ConstUChar(3))] Var("%1.1") false false)
    Ret(Var("%1.1"))
  ])
])
)req";

const char kUnnamedResponse[] = R"req(new_functions []
reg_locals []
carry []
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

// Returns `request` with its mode line `mode <from>` made `mode <to>`.
std::string with_mode(std::string request, const std::string& from, const std::string& to) {
  const std::string line = "\nmode " + from + "\n";
  const std::size_t at = request.find(line);
  REQUIRE(at != std::string::npos);
  return request.replace(at, line.size(), "\nmode " + to + "\n");
}

}  // namespace

TEST_CASE("backend answers an unoptimized request with the text") {
  CHECK(c6502::backend::respond(kUnoptimizedRequest) == kUnoptimizedResponse);
}

TEST_CASE("backend answers an optimized request with the text") {
  CHECK(c6502::backend::respond(kOptimizedRequest) == kOptimizedResponse);
}

TEST_CASE("backend spells an unnamed parameter's slot by its position") {
  const std::string got = c6502::backend::respond(kUnnamedRequest);
  CHECK(got == kUnnamedResponse);
  CHECK(got.find("__zpabi_put__0") != std::string::npos);
}

TEST_CASE("backend answers a request for the harness with the program") {
  const std::string got = c6502::backend::respond(
      with_mode(kOptimizedRequest, "stage-optimized-text", "stage-optimized"));
  CHECK(got.starts_with("new_functions []\nreg_locals [\"@0.x.reg\"]\ncarry []\nsymbols ["));
  CHECK(got.find("\nasm ") != std::string::npos);
  CHECK(got.find("Function(\"main\"") != std::string::npos);
}

TEST_CASE("backend rejects a request it can't read") {
  CHECK_THROWS_AS(c6502::backend::respond(with_mode(kOptimizedRequest, "stage-optimized-text",
                                                    "optimized")),
                  std::runtime_error);
  CHECK_THROWS_AS(c6502::backend::respond("c6502-backend 2\n"), std::runtime_error);
  CHECK_THROWS_AS(c6502::backend::respond("c6502-backend 1\nmode stage-optimized\nargs {}\n"),
                  std::runtime_error);
}
