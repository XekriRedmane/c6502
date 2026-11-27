// The tests of tests/test_parser.py that compiled a program: pointers,
// their operators and casts, static pointer initializers, and calls
// through a function pointer, from the parser to the `-S` text. Each
// source goes in as compile.py's `_run_stage` takes it, without the
// preprocessor, and an ill-typed one is compiled through `--tac`, so that
// the type checker's error comes back before any later pass runs.
#include <string>

#include "compile_case.h"
#include "doctest.h"

namespace {

namespace suite = c6502::suite;

suite::Options unpreprocessed() {
  suite::Options options;
  options.preprocess = false;
  return options;
}

std::string codegen(const std::string& src) {
  return suite::run_source(suite::Stage::kCodegen, src, unpreprocessed());
}

suite::Error tac_error(const std::string& src) {
  return suite::compile_error(suite::Stage::kTac, src, unpreprocessed());
}

bool type_check_error(const std::string& src) {
  return suite::is_a(tac_error(src).type, "TypeCheckError");
}

}  // namespace

// --- Pointer declarations ---

TEST_CASE("parser: a pointer variable compiles to two zero bytes") {
  // A pointer is 2 bytes, so a tentative `int *p;` lays down as
  // `DS.B 2`.
  const std::string text = codegen("int *p;\n");
  CHECK(suite::contains(text, "p:"));
  CHECK(suite::contains(text, "DS.B  2"));
}

TEST_CASE("parser: a pointer local takes two frame bytes") {
  // M=2 (one pointer local) + 2 (saved-FP slot) = 4, so SSP -= 4.
  const std::string text = codegen("int main(void) { int *p; p = p; return 0; }\n");
  CHECK(suite::contains(text, "prologue: 0 arg bytes, 2 local bytes"));
  CHECK(suite::contains(text, "SBC   #$04"));
}

TEST_CASE("parser: a function returns a pointer in A and X") {
  // The soft-stack epilogue keeps the high byte on the hardware stack
  // while it tears down the frame. The pointer parameter sits in the
  // caller's pushed arguments, so the function has no local bytes.
  const std::string text = codegen(
      "int *foo(int *p) { return p; }\n"
      "int main(void) { return 0; }\n");
  CHECK(suite::contains(text, "foo:"));
  CHECK_FALSE(suite::contains(text, "STA   HARGS"));
  CHECK(suite::contains(text, "   PLA\n   TAX\n   PLA\n   RTS\n"));
  CHECK(suite::contains(text, "prologue: 2 arg bytes, 0 local bytes"));
}

// --- Dereference and address-of ---

TEST_CASE("parser: the address of a local adds its offset to FP") {
  const std::string text = codegen("int main(void) { int x; int *p; p = &x; return 0; }\n");
  CHECK(suite::contains(text, "CLC"));
  CHECK(suite::contains(text, "LDA   FP"));
  CHECK(suite::contains(text, "LDA   FP+1"));
  CHECK(suite::contains(text, "ADC   #$00"));
}

TEST_CASE("parser: the address of a static loads its label's bytes as immediates") {
  const std::string text = codegen("int g;\nint main(void) { int *p; p = &g; return 0; }\n");
  CHECK(suite::contains(text, "LDA   #<g"));
  CHECK(suite::contains(text, "LDA   #>g"));
}

TEST_CASE("parser: a read through a pointer goes through DPTR") {
  const std::string text = codegen("int main(void) { int x; int *p; p = &x; return *p; }\n");
  CHECK(suite::contains(text, "STA   DPTR"));
  CHECK(suite::contains(text, "STA   DPTR+1"));
  CHECK(suite::contains(text, "LDA   (DPTR),Y"));
}

TEST_CASE("parser: a write through a pointer goes through DPTR") {
  const std::string text =
      codegen("int main(void) { int x; int *p; p = &x; *p = 5; return 0; }\n");
  CHECK(suite::contains(text, "STA   DPTR"));
  CHECK(suite::contains(text, "STA   (DPTR),Y"));
  CHECK(suite::contains(text, "LDA   #$05"));
}

TEST_CASE("parser: a store through a long pointer writes two bytes through DPTR") {
  // The Y-tracking peephole rewrites the second `LDY #$01` as `INY`,
  // since Y is known to be 0.
  const std::string text = codegen(
      "int main(void) { long y; long *lp; lp = &y; "
      "*lp = 0x1234L; return 0; }\n");
  CHECK(suite::contains(text, "LDA   #$34"));
  CHECK(suite::contains(text, "LDA   #$12"));
  CHECK(suite::contains(text, "LDY   #$00"));
  CHECK(suite::contains(text, "   INY"));
  CHECK(suite::contains(text, "STA   (DPTR),Y"));
}

TEST_CASE("parser: the address of a dereference is the pointer itself") {
  // `&*p` is `p` (C99 §6.5.3.2.3), so `q = &*p` is a copy with no
  // address arithmetic. Each `&local` lowers to one CLC for its add to
  // FP, and the epilogue has a CLC of its own, so the form with `&*p`
  // has one CLC fewer than the form with `&x`.
  const std::string with_amp_x = codegen(
      "int main(void) { int x; int *p; int *q; "
      "p = &x; q = &x; return 0; }\n");
  const std::string with_amp_deref = codegen(
      "int main(void) { int x; int *p; int *q; "
      "p = &x; q = &*p; return 0; }\n");
  CHECK(suite::count(with_amp_x, "CLC") == suite::count(with_amp_deref, "CLC") + 1);
}

// --- Equality and ordering ---

TEST_CASE("parser: == of two pointers of one type compares both bytes") {
  // The BNE short-circuits out of the high byte's compare, and branch
  // threading sends it past the low byte's BEQ (`.thread@N`).
  const std::string text = codegen(
      "int main(void) {\n"
      "  int x;\n"
      "  int *p; int *q;\n"
      "  p = &x; q = &x;\n"
      "  return p == q;\n"
      "}\n");
  CHECK(suite::count(text, "CMP   (FP),Y") == 2);
  CHECK(suite::search(R"(BNE   \.(cmp_differ|thread)@)", text));
}

TEST_CASE("parser: != of two pointers of one type compares both bytes") {
  const std::string text = codegen(
      "int main(void) {\n"
      "  int x;\n"
      "  int *p; int *q;\n"
      "  p = &x; q = &x;\n"
      "  return p != q;\n"
      "}\n");
  CHECK(suite::count(text, "CMP   (FP),Y") == 2);
}

TEST_CASE("parser: a pointer compares with a null pointer constant on the right") {
  CHECK_NOTHROW(codegen("int main(void) { int *p; if (p == 0) return 1; return 0; }\n"));
}

TEST_CASE("parser: a pointer compares with a null pointer constant on the left") {
  CHECK_NOTHROW(codegen("int main(void) { int *p; if (0 == p) return 1; return 0; }\n"));
}

TEST_CASE("parser: a long zero is a null pointer constant") {
  CHECK_NOTHROW(codegen("int main(void) { int *p; if (p != 0L) return 1; return 0; }\n"));
}

TEST_CASE("parser: a zero under a cast is a null pointer constant") {
  CHECK_NOTHROW(codegen("int main(void) { int *p; if (p == (long)0) return 1; return 0; }\n"));
}

TEST_CASE("parser: a pointer compared with a nonzero integer is rejected") {
  // 5 isn't a null pointer constant.
  CHECK(type_check_error("int main(void) { int *p; if (p == 5) return 1; return 0; }\n"));
}

TEST_CASE("parser: == of pointers of different types is rejected") {
  CHECK(type_check_error(
      "int main(void) {\n"
      "  int *p; long *q;\n"
      "  if (p == q) return 1;\n"
      "  return 0;\n"
      "}\n"));
}

TEST_CASE("parser: < of two pointers of one type is an unsigned compare") {
  // A pointer takes the unsigned ordering, with no correction for V: a
  // subtraction of each byte, and the carry as the 0 or 1.
  const std::string text = codegen(
      "int main(void) {\n"
      "  int x;\n"
      "  int *p; int *q;\n"
      "  p = &x; q = &x;\n"
      "  return p < q;\n"
      "}\n");
  CHECK(suite::count(text, "SBC   (FP),Y") == 2);
  CHECK(suite::contains(text, "ROL"));
  CHECK_FALSE(suite::contains(text, "EOR   #$01"));
  CHECK_FALSE(suite::contains(text, ".cmp_novf@"));
}

TEST_CASE("parser: < of pointers of different types is rejected") {
  CHECK(type_check_error(
      "int main(void) { int *p; long *q; "
      "if (p < q) return 1; return 0; }\n"));
}

TEST_CASE("parser: < of a pointer and an integer is rejected") {
  // The exception for a null pointer constant covers only == and !=,
  // so `p < 0` is rejected where `p == 0` compiles.
  CHECK(type_check_error(
      "int main(void) { int *p; if (p < 0) return 1; "
      "return 0; }\n"));
}

TEST_CASE("parser: == of pointers is an int") {
  // C99 §6.5.9.1. The result is returned from a function that returns
  // int.
  CHECK_NOTHROW(codegen(
      "int main(void) { int *p; int *q; "
      "p = q; return p == q; }\n"));
}

// --- Casts between pointers and integers ---

TEST_CASE("parser: a pointer converts to int") {
  const std::string text = codegen(
      "int main(void) {\n"
      "  int x; int a; int *p;\n"
      "  p = &x;\n"
      "  a = (int)p;\n"
      "  return a;\n"
      "}\n");
  CHECK(suite::contains(text, "RTS"));
}

TEST_CASE("parser: a pointer converts to long") {
  CHECK_NOTHROW(codegen(
      "int main(void) {\n"
      "  int x; long l; int *p;\n"
      "  p = &x;\n"
      "  l = (long)p;\n"
      "  return 0;\n"
      "}\n"));
}

TEST_CASE("parser: a signed char converts to a pointer by sign extension") {
  const std::string text = codegen(
      "int main(void) {\n"
      "  signed char a; int *p;\n"
      "  a = 5;\n"
      "  p = (int *)a;\n"
      "  return 0;\n"
      "}\n");
  CHECK(suite::contains(text, ".sx_neg@"));
  CHECK(suite::contains(text, "LDA   #$FF"));
  CHECK(suite::contains(text, "LDA   #$00"));
}

TEST_CASE("parser: an unsigned int converts to a pointer with no sign extension") {
  const std::string text = codegen(
      "int main(void) {\n"
      "  unsigned int u; int *p;\n"
      "  u = 200u;\n"
      "  p = (int *)u;\n"
      "  return 0;\n"
      "}\n");
  CHECK_FALSE(suite::contains(text, ".sx_neg@"));
  CHECK(suite::contains(text, "LDA   #$00"));
}

TEST_CASE("parser: a long converts to a pointer") {
  CHECK_NOTHROW(codegen(
      "int main(void) {\n"
      "  long l; int *p;\n"
      "  l = 0x1234L;\n"
      "  p = (int *)l;\n"
      "  return 0;\n"
      "}\n"));
}

TEST_CASE("parser: a pointer converts to a pointer to another type") {
  CHECK_NOTHROW(codegen(
      "int main(void) {\n"
      "  int x; int *p; long *lp;\n"
      "  p = &x;\n"
      "  lp = (long *)p;\n"
      "  return 0;\n"
      "}\n"));
}

TEST_CASE("parser: an int converts to a pointer to a pointer") {
  CHECK_NOTHROW(codegen(
      "int main(void) {\n"
      "  int a; int **pp;\n"
      "  a = 1;\n"
      "  pp = (int **)a;\n"
      "  return 0;\n"
      "}\n"));
}

TEST_CASE("parser: a float doesn't convert to a pointer") {
  // C99 §6.3.2.3 defines no conversion between a floating type and a
  // pointer.
  const suite::Error e = tac_error(
      "int main(void) { float f; int *p; "
      "f = 1.5f; p = (int *)f; return 0; }\n");
  CHECK(suite::is_a(e.type, "TypeCheckError"));
  CHECK(suite::contains(e.message, "pointer"));
  CHECK(suite::contains(e.message, "floating"));
}

TEST_CASE("parser: a double doesn't convert to a pointer") {
  CHECK(type_check_error(
      "int main(void) { double d; long *lp; "
      "d = 2.5; lp = (long *)d; return 0; }\n"));
}

TEST_CASE("parser: a pointer doesn't convert to a float") {
  CHECK(type_check_error(
      "int main(void) { int x; int *p; float f; "
      "p = &x; f = (float)p; return 0; }\n"));
}

TEST_CASE("parser: a pointer doesn't convert to a double") {
  CHECK(type_check_error(
      "int main(void) { int x; int *p; double d; "
      "p = &x; d = (double)p; return 0; }\n"));
}

// --- Unary and binary operators on pointers ---

TEST_CASE("parser: - of a pointer is rejected") {
  const suite::Error e = tac_error("int main(void) { int x; int *p; p = &x; return -p; }\n");
  CHECK(suite::is_a(e.type, "TypeCheckError"));
  CHECK(suite::contains(e.message, "'-'"));
}

TEST_CASE("parser: ~ of a pointer is rejected") {
  const suite::Error e = tac_error("int main(void) { int x; int *p; p = &x; return ~p; }\n");
  CHECK(suite::is_a(e.type, "TypeCheckError"));
  CHECK(suite::contains(e.message, "'~'"));
}

TEST_CASE("parser: ! of a pointer ORs its two bytes") {
  // Z is set if and only if both bytes are zero, i.e. the pointer is
  // null.
  const std::string text = codegen("int main(void) { int x; int *p; p = &x; return !p; }\n");
  CHECK(suite::contains(text, "ORA   (FP),Y"));
  CHECK(suite::contains(text, ".lnot_true@"));
}

TEST_CASE("parser: ! of a pointer to a pointer compiles") {
  CHECK_NOTHROW(codegen("int main(void) { int *p; int **pp; pp = &p; return !pp; }\n"));
}

TEST_CASE("parser: - of a long pointer is rejected") {
  // The pointee's type makes no difference.
  CHECK(type_check_error("int main(void) { long y; long *lp; lp = &y; return -lp; }\n"));
}

TEST_CASE("parser: && of two pointers tests each pointer's two bytes") {
  const std::string text = codegen(
      "int main(void) { int x; int y; int *p; int *q; "
      "p = &x; q = &y; if (p && q) return 1; return 0; }\n");
  CHECK(suite::count(text, "ORA   (FP),Y") >= 2);
  CHECK(suite::contains(text, ".and_false@"));
}

TEST_CASE("parser: || of two pointers tests each pointer's two bytes") {
  const std::string text = codegen(
      "int main(void) { int x; int y; int *p; int *q; "
      "p = &x; q = &y; if (p || q) return 1; return 0; }\n");
  CHECK(suite::count(text, "ORA   (FP),Y") >= 2);
  CHECK(suite::contains(text, ".or_true@"));
}

TEST_CASE("parser: && takes an integer and a pointer") {
  // Each operand is tested by itself, so the two need no common type.
  CHECK_NOTHROW(codegen(
      "int main(void) { int a; int x; int *p; "
      "a = 1; p = &x; if (a && p) return 1; return 0; }\n"));
}

TEST_CASE("parser: || takes an integer and a pointer") {
  CHECK_NOTHROW(codegen(
      "int main(void) { int a; int x; int *p; "
      "a = 0; p = &x; if (a || p) return 1; return 0; }\n"));
}

TEST_CASE("parser: && takes pointers of different types") {
  // `==` and `!=` take a common type, and `&&` and `||` don't.
  CHECK_NOTHROW(codegen(
      "int main(void) { int x; long y; int *p; long *lp; "
      "p = &x; lp = &y; if (p && lp) return 1; return 0; }\n"));
}

TEST_CASE("parser: && of pointers is an int") {
  // C99 §6.5.13.3 and §6.5.14.3, whatever the operands' types.
  CHECK_NOTHROW(codegen(
      "int main(void) { int *p; int *q; "
      "return p && q; }\n"));
}

TEST_CASE("parser: * of a pointer is rejected") {
  // C99 §6.5.5 takes arithmetic operands on both sides.
  const suite::Error e = tac_error(
      "int main(void) { int x; int *p; p = &x; "
      "return p * 2; }\n");
  CHECK(suite::is_a(e.type, "TypeCheckError"));
  CHECK(suite::contains(e.message, "'*'"));
}

TEST_CASE("parser: / of two pointers is rejected") {
  const suite::Error e = tac_error(
      "int main(void) { int *p; int *q; "
      "return p / q; }\n");
  CHECK(suite::is_a(e.type, "TypeCheckError"));
  CHECK(suite::contains(e.message, "'/'"));
}

TEST_CASE("parser: % with a pointer on the right is rejected") {
  const suite::Error e = tac_error(
      "int main(void) { int a; int *p; "
      "return a % p; }\n");
  CHECK(suite::is_a(e.type, "TypeCheckError"));
  CHECK(suite::contains(e.message, "'%'"));
}

TEST_CASE("parser: / of a pointer by an integer is rejected") {
  CHECK(type_check_error(
      "int main(void) { int x; int *p; p = &x; "
      "return p / 2; }\n"));
}

// --- Static pointer initializers ---

TEST_CASE("parser: a static pointer to a static is a word holding its label") {
  // The address is resolved when the program is linked, and with an
  // offset of 0 the operand is the bare symbol.
  const std::string text = codegen(
      "static int a;\n"
      "static int *a_ptr = &a;\n"
      "int main(void) { return 0; }\n");
  CHECK(suite::contains(text, "a_ptr:"));
  CHECK(suite::contains(text, "DC.W  a"));
}

TEST_CASE("parser: an external pointer to a static is a word holding its label") {
  const std::string text = codegen(
      "int a;\n"
      "int *p = &a;\n"
      "int main(void) { return 0; }\n");
  CHECK(suite::contains(text, "p:"));
  CHECK(suite::contains(text, "DC.W  a"));
}

TEST_CASE("parser: a block-scope static pointer to a file-scope object is a word") {
  // The object keeps its block-scope name, `@N.p`.
  const std::string text = codegen(
      "int g;\n"
      "int main(void) {\n"
      "  static int *p = &g;\n"
      "  return 0;\n"
      "}\n");
  CHECK(suite::contains(text, "DC.W  g"));
}

TEST_CASE("parser: a static pointer to a long is a word") {
  const std::string text = codegen(
      "long y;\n"
      "long *lp = &y;\n"
      "int main(void) { return 0; }\n");
  CHECK(suite::contains(text, "DC.W  y"));
}

TEST_CASE("parser: the address of a local can't initialize a static") {
  // An automatic object's address isn't known when the program is
  // linked, so it isn't a constant expression.
  const suite::Error e = tac_error(
      "int main(void) {\n"
      "  int local;\n"
      "  static int *p = &local;\n"
      "  return 0;\n"
      "}\n");
  CHECK(suite::is_a(e.type, "TypeCheckError"));
  CHECK(suite::contains(e.message, "static storage duration"));
}

TEST_CASE("parser: a static pointer initialized to 0 is two zero bytes") {
  // The typed zero becomes a 2-byte zero run, `DS.B 2`, where it could
  // have been `DC.W $0000`.
  const std::string text = codegen(
      "static int *p = 0;\n"
      "int main(void) { return 0; }\n");
  CHECK(suite::contains(text, "p:"));
  CHECK(suite::contains(text, "DS.B  2"));
}

TEST_CASE("parser: a pointer with no initializer is two zero bytes") {
  const std::string text = codegen(
      "int *p;\n"
      "int main(void) { return 0; }\n");
  CHECK(suite::contains(text, "p:"));
  CHECK(suite::contains(text, "DS.B  2"));
}

TEST_CASE("parser: a static function pointer is a word holding the function's label") {
  const std::string text = codegen(
      "int foo(void) { return 7; }\n"
      "int (*fp)(void) = &foo;\n"
      "int main(void) { return 0; }\n");
  CHECK(suite::contains(text, "fp:"));
  CHECK(suite::contains(text, "DC.W  foo"));
}

TEST_CASE("parser: a static function pointer may point to a function declared only") {
  // The linker resolves the reference.
  const std::string text = codegen(
      "int foo(int x);\n"
      "int (*fp)(int) = &foo;\n"
      "int main(void) { return 0; }\n");
  CHECK(suite::contains(text, "DC.W  foo"));
}

TEST_CASE("parser: a function's address assigned at run time is an immediate label") {
  // `foo` has static storage, so its address loads as `#<foo` and
  // `#>foo` and isn't an offset from FP.
  const std::string text = codegen(
      "int foo(void) { return 7; }\n"
      "int main(void) {\n"
      "  int (*fp)(void);\n"
      "  fp = &foo;\n"
      "  return 0;\n"
      "}\n");
  CHECK(suite::contains(text, "LDA   #<foo"));
  CHECK(suite::contains(text, "LDA   #>foo"));
}

TEST_CASE("parser: a block-scope static function pointer is a word") {
  const std::string text = codegen(
      "int foo(void) { return 7; }\n"
      "int main(void) {\n"
      "  static int (*fp)(void) = &foo;\n"
      "  return 0;\n"
      "}\n");
  CHECK(suite::contains(text, "DC.W  foo"));
}

// --- Calls through a function pointer ---
//
// The 6502 has no indirect JSR, so the pointer's two bytes go into
// DPTR and the call is `JSR icall`, a trampoline whose body is
// `JMP (DPTR)`.

TEST_CASE("parser: a call through a function pointer goes through icall") {
  const std::string text = codegen(
      "int foo(void) { return 7; }\n"
      "int main(void) {\n"
      "  int (*fp)(void) = &foo;\n"
      "  return fp();\n"
      "}\n");
  CHECK(suite::contains(text, "STA   DPTR"));
  CHECK(suite::contains(text, "STA   DPTR+1"));
  CHECK(suite::contains(text, "JSR   icall"));
}

TEST_CASE("parser: an indirect call passes its arguments on the soft stack") {
  // An int argument takes 2 bytes, so SSP -= 2.
  const std::string text = codegen(
      "int foo(int x) { return x + 1; }\n"
      "int main(void) {\n"
      "  int (*fp)(int) = &foo;\n"
      "  return fp(5);\n"
      "}\n");
  CHECK(suite::contains(text, "JSR   icall"));
  CHECK(suite::contains(text, "LDA   #$05"));
  CHECK(suite::contains(text, "SBC   #$02"));
}

TEST_CASE("parser: a direct and an indirect call go in one function") {
  const std::string text = codegen(
      "int foo(int x) { return x; }\n"
      "int main(void) {\n"
      "  int (*fp)(int) = &foo;\n"
      "  int a = foo(1);\n"
      "  int b = fp(2);\n"
      "  return a + b;\n"
      "}\n");
  CHECK(suite::contains(text, "JSR   foo"));
  CHECK(suite::contains(text, "JSR   icall"));
}

TEST_CASE("parser: an indirect call returns a long as a direct call does") {
  // The caller reads the result back from HARGS+0 and HARGS+1.
  const std::string text = codegen(
      "long foo(void) { return 0x1234L; }\n"
      "int main(void) {\n"
      "  long (*fp)(void) = &foo;\n"
      "  long y = fp();\n"
      "  return 0;\n"
      "}\n");
  CHECK(suite::contains(text, "JSR   icall"));
  CHECK(suite::contains(text, "LDA   HARGS"));
  CHECK(suite::contains(text, "LDA   HARGS+1"));
}

TEST_CASE("parser: an indirect call goes through a pointer assigned at run time") {
  const std::string text = codegen(
      "int foo(void) { return 7; }\n"
      "int main(void) {\n"
      "  int (*fp)(void);\n"
      "  fp = &foo;\n"
      "  return fp();\n"
      "}\n");
  CHECK(suite::contains(text, "LDA   #<foo"));
  CHECK(suite::contains(text, "LDA   #>foo"));
  CHECK(suite::contains(text, "JSR   icall"));
}

TEST_CASE("parser: (*fp)(args) is an indirect call") {
  // A function designator converts to a pointer (C99 §6.3.2.1.4), so
  // `(*fp)(5)` is `fp(5)`.
  const std::string text = codegen(
      "int foo(int x) { return x + 1; }\n"
      "int main(void) {\n"
      "  int (*fp)(int) = &foo;\n"
      "  return (*fp)(5);\n"
      "}\n");
  CHECK(suite::contains(text, "JSR   icall"));
}

TEST_CASE("parser: (foo)(args) is a direct call") {
  const std::string text = codegen(
      "int foo(int x) { return x + 1; }\n"
      "int main(void) {\n"
      "  return (foo)(5);\n"
      "}\n");
  CHECK(suite::contains(text, "JSR   foo"));
  CHECK_FALSE(suite::contains(text, "JSR   icall"));
}

TEST_CASE("parser: (&foo)(args) is a direct call") {
  // The `&` and the conversion of `foo` to a pointer cancel out.
  const std::string text = codegen(
      "int foo(int x) { return x + 1; }\n"
      "int main(void) {\n"
      "  return (&foo)(5);\n"
      "}\n");
  CHECK(suite::contains(text, "JSR   foo"));
  CHECK_FALSE(suite::contains(text, "JSR   icall"));
}

TEST_CASE("parser: a call of a constant is a parser error") {
  CHECK(suite::is_a(tac_error("int main(void) { return (1)(5); }\n").type, "ParserError"));
}

TEST_CASE("parser: ~ of a float is rejected") {
  // C99 §6.5.3.3.4 takes an integer operand.
  const suite::Error e = tac_error("int main(void) { float f; f = 1.5f; return ~f; }\n");
  CHECK(suite::is_a(e.type, "TypeCheckError"));
  CHECK(suite::contains(e.message, "'~'"));
  CHECK(suite::contains(e.message, "integer"));
}

TEST_CASE("parser: ~ of a double is rejected") {
  CHECK(type_check_error("int main(void) { double d; d = 1.5; return ~d; }\n"));
}
