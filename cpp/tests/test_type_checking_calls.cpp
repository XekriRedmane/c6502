// The calls of the type checker's `check_program` that the Python unit
// tests make and the corpus doesn't cover, each recorded with Python's
// result: the 151 calls that reach the 615 lines of
// type_checking.py, constant_expression.py, designators.py, and
// stdbit_functions.py that only the unit tests reach, and for each error
// message the corpus never gives, the smallest call that gives it. Each
// case is the program's dump, and either the JSON of the program
// stamped in place with the symbol and type tables and the `mutated`
// line, or the error's JSON, which `check-captures` compares too.
#include <string>
#include <vector>

#include "capture.h"
#include "doctest.h"
#include "errors.h"
#include "json.h"
#include "passes/stage_registry.h"
#include "stage_checks.h"
#include "stage_case.h"

namespace {

using c6502::testing::text;

struct Case {
  std::string input;
  bool error;
  std::string expected;
  std::string mutated;
};

std::vector<Case> cases() {
  return {
    {text({R"~~(Program([
  FunctionDecl(function_decl("f" [] None FunType([] Int) None None [] false 4096 ["X"] None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "function 'f' has `preserves_x` but returns an int, whose high byte comes back in X"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  VarDecl(var_decl("a" None Array(Int 0) None None None None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "file-scope object 'a': incomplete array type (`[]`) is allowed only at the outermost type of an `extern` declaration"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  FunctionDecl(function_decl("f" [] None FunType([] UInt) None None [] false 4096 ["X"] None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "function 'f' has `preserves_x` but returns an unsigned int, whose high byte comes back in X"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  StructDecl(struct_decl("s" false [member_decl("a" Array(Int 0))]))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "member 'a' of struct/union 's': incomplete array type (`[]`) is allowed only at the outermost type of an `extern` declaration"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(Return(SizeOfType(FunType([Int] Int) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "sizeof of a function type is illegal (C99 \u00a76.5.3.4.1)"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  FunctionDecl(function_decl("f" [] Block([
    S(Expression(FunctionCall("__c6502_bin_to_bcd" [] None false)))
  ]) FunType([] Void) None None [] false None [] None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "__c6502_bin_to_bcd takes 1 arguments, got 0"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  VarDecl(var_decl("x" None UChar Extern None Absolute(49152) None false))
  VarDecl(var_decl("x" None UChar Extern None Absolute(49153) None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "two declarations of 'x' give it different addresses, $C000 and $C001"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  VarDecl(var_decl("x" None UChar None None Absolute(49152) None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "'x' has `[[c6502::address(...)]]`, which names memory the program doesn't own, so it can't be defined; declare it `extern` without an initializer"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  VarDecl(var_decl("x" CompoundLiteral("@0.compound_literal" Int Constant(ConstInt(1) None) None Register false) Int None None None None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "a compound literal at file scope can't be `register` (C23 \u00a76.5.3.6p4)"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  FunctionDecl(function_decl("f" ["@0.x"] Block([
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([Void] Int) None None [""] false None [] None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "parameter of function 'f' cannot have void type (C99 \u00a76.7.5.3.4)"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  VarDecl(var_decl("a" InitList([] None None) Array(Int 0) None None None None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "array 'a' has unknown size, so it can't be initialized by the empty initializer `{}`, which has no elements to give it a size (C23 \u00a76.7.11p4)"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  VarDecl(var_decl("v" Constant(ConstInt(1) None) Const(Volatile(Int)) None None None None true))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "`constexpr` object 'v' has the volatile type volatile int, and a constexpr object can't have a volatile type, or a member of one (C23 \u00a76.7.2p5)"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  VarDecl(var_decl("b" Constant(ConstInt(2) None) Const(Bool) None None None None true))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "the initializer of `constexpr` object 'b' has the value 2, which bool can't hold exactly, and a constexpr initializer can't change a value (C23 \u00a76.7.2p5)"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  VarDecl(var_decl("x" InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) Int None None None None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "too many initializers for scalar variable 'x': 2 given, and a scalar's braces hold one expression (C23 \u00a76.7.11p12)"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  VarDecl(var_decl("s" String("\xff") Array(Const(SChar) 0) None None None None true))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "the string that initializes `constexpr` array 's' has the character 255, which signed char can't hold, and a constexpr initializer can't change a value (C23 \u00a76.7.2p5)"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  VarDecl(var_decl("D" Constant(ConstLong(16777217) None) Const(Float) None None None None true))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "the initializer of `constexpr` object 'D' has the value 16777217, which float can't hold exactly, and a constexpr initializer can't change a value (C23 \u00a76.7.2p5)"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  VarDecl(var_decl("b5" Constant(ConstInt(16) None) Const(BitInt(5 true)) None None None None true))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "the initializer of `constexpr` object 'b5' has the value 16, which _BitInt(5) can't hold exactly, and a constexpr initializer can't change a value (C23 \u00a76.7.2p5)"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  VarDecl(var_decl("x" Constant(ConstDouble(4613937818241073152) None) Const(Int) None None None None true))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "the initializer of `constexpr` object 'x' has the floating type double, and an object of integer type takes an integer constant expression (C23 \u00a76.7.2p6)"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  FunctionDecl(function_decl("f" [] Block([
    S(Expression(FunctionCall("__c6502_bin_to_bcd" [Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None false)))
  ]) FunType([] Void) None None [] false None [] None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "__c6502_bin_to_bcd takes 1 arguments, got 2"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  VarDecl(var_decl("f" Constant(ConstDouble(4591870180066957722) None) Const(Float) None None None None true))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "the initializer of `constexpr` object 'f' has a double value that float can't hold exactly, and a constexpr initializer can't change a value (C23 \u00a76.7.2p5)"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  VarDecl(var_decl("g" None Int None None None None false))
  VarDecl(var_decl("p" AddressOf(Var("g" None) None) Const(Pointer(Int)) None None None None true))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "`constexpr` object 'p' holds a pointer that isn't null, and a constexpr pointer must be null (C23 \u00a76.7.2p6)"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  FunctionDecl(function_decl("g" [] Block([
    S(Return(Designation([IndexDesignator(0)] Constant(ConstInt(1) None))))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "a designator such as `[2] =` or `.x =` goes only in front of an item of a braced initializer"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  FunctionDecl(function_decl("f" [] None FunType([] Void) None None [] false 4096 [] None false))
  FunctionDecl(function_decl("f" [] None FunType([] Void) None None [] false 8192 [] None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "two declarations of function 'f' give it different addresses, $1000 and $2000"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  VarDecl(var_decl("g" None UChar None None ZeroPage(None) ".d" false))
  FunctionDecl(function_decl("main" [] Block([
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "'g' is in zero page, so it can't also be in section '.d'"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  FunctionDecl(function_decl("f" [] None FunType([] Void) None None [] false None ["X"] None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "function 'f' has `preserves_x` but no `[[c6502::address(...)]]`; only a routine at a fixed address, whose code the compiler doesn't compile, can declare which registers it preserves"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  VarDecl(var_decl("x" InitList([InitList([] None None)] None None) Int None None None None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "unexpected nested initializer for scalar variable 'x': a scalar's initializer is one expression, optionally in one pair of braces, or the empty initializer `{}` (C23 \u00a76.7.11p12)"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  VarDecl(var_decl("a" None UChar None None None None false))
  FunctionDecl(function_decl("f" [] Block([
    S(Expression(FunctionCall("__c6502_bcd_add" [Var("a" None)] None false)))
  ]) FunType([] Void) None None [] false None [] None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "__c6502_bcd_add takes 2 arguments, got 1"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(Return(InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "brace-enclosed initializer (`{...}`) is only valid as a variable initializer for an array"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  VarDecl(var_decl("minusOne" Unary(Negate Constant(ConstInt(1) None) None) Const(UInt) None None None None true))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "the initializer of `constexpr` object 'minusOne' has the value -1, which unsigned int can't hold exactly, and a constexpr initializer can't change a value (C23 \u00a76.7.2p5)"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("x" Constant(ConstInt(5) None) Int Extern None None None false)))
    S(Return(Var("x" None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "block-scope `extern` declaration of 'x' may not have an initializer"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  VarDecl(var_decl("B" Constant(ConstULongLong(18446744073709551615) None) Const(Short) None None None None true))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "the initializer of `constexpr` object 'B' has the value 18446744073709551615, which short can't hold exactly, and a constexpr initializer can't change a value (C23 \u00a76.7.2p5)"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  VarDecl(var_decl("g" None UChar Extern None Absolute(49152) ".d" false))
  FunctionDecl(function_decl("main" [] Block([
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "'g' has `address`, so it's already placed and can't also be in section '.d'"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  VarDecl(var_decl("a" None Array(Int 0) Extern None None None false))
  FunctionDecl(function_decl("main" [] Block([
    S(Return(Cast(Int SizeOfExp(Var("a" None) None) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "sizeof of an incomplete array type is illegal (C99 \u00a76.5.3.4.1)"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("@0.a" Constant(ConstInt(5) None) Array(Int 3) None None None None false)))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "array '@0.a' requires a brace-enclosed initializer (`{...}`)"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  FunctionDecl(function_decl("f" [] Block([]) FunType([] Void) None None [] false None [] None false))
  FunctionDecl(function_decl("f" [] None FunType([] Void) None None [] false 4096 [] None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "function 'f' has `[[c6502::address(...)]]`, which names code that's already at $1000, so it can't also be defined"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  VarDecl(var_decl("a" None UChar None None None None false))
  FunctionDecl(function_decl("f" [] Block([
    S(Expression(FunctionCall("__c6502_bcd_sub" [Var("a" None) Var("a" None) Var("a" None)] None false)))
  ]) FunType([] Void) None None [] false None [] None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "__c6502_bcd_sub takes 2 arguments, got 3"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  VarDecl(var_decl("arr" String("abcd") Array(Char 3) None None None None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "string literal initializer for array 'arr' has 4 bytes but the declared array size is 3 (C99 \u00a76.7.8.14 requires the literal length to be \u2264 the array size, with the null terminator omitted when the array has no room for it)"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  FunctionDecl(function_decl("f" [] Block([
    S(Return(AddressOf(CompoundLiteral("@0.compound_literal" UChar Constant(ConstInt(1) None) None Register false) None)))
  ]) FunType([] Pointer(UChar)) None None [] false None [] None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "can't take the address of a `register` compound literal (C99 \u00a76.5.3.2.1)"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  VarDecl(var_decl("a" None UChar None None ZeroPage(64) None false))
  VarDecl(var_decl("a" None UChar None None ZeroPage(65) None false))
  FunctionDecl(function_decl("main" [] Block([
    S(Return(Var("a" None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "'a' is placed at zero-page $40 and at $41"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  VarDecl(var_decl("p" CompoundLiteral("@0.compound_literal" Array(Int 0) InitList([] None None) None None false) Pointer(Int) None None None None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "the compound literal's array has unknown size, so it can't be initialized by the empty initializer `{}`, which has no elements to give it a size (C23 \u00a76.7.11p4)"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("@0.x" Constant(ConstInt(7) None) Int Register None None None false)))
    S(Return(Var("@0.x" None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "register local '@0.x': a 2-byte value doesn't fit the register budget (at most 1 byte)"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  VarDecl(var_decl("K" Constant(ConstInt(1) None) Const(Int) None None None None true))
  VarDecl(var_decl("K" None Int Extern None None None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "'K' is declared again after its `constexpr` declaration, which c6502 takes only as the one declaration of a name (C23 \u00a76.7.1p12 leaves any other to the implementation)"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  VarDecl(var_decl("x" None Int None None None None false))
  FunctionDecl(function_decl("main" [] Block([
    S(Return(IndirectCall(Dereference(AddressOf(Var("x" None) None) None) [] None false)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "called object is not a function or function pointer (type Int())"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("@0.x" None UChar None None Absolute(49152) None false)))
    S(Return(Var("@0.x" None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "`[[c6502::address(...)]]` needs an `extern` declaration, since 'x' names memory the program doesn't own"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  StructDecl(struct_decl("s" false [member_decl("a" Volatile(Int))]))
  VarDecl(var_decl("S" InitList([Constant(ConstInt(1) None)] None None) Const(Structure("s")) None None None None true))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "`constexpr` object 'S' has a volatile member 'a', and a constexpr object can't have a volatile type, or a member of one (C23 \u00a76.7.2p5)"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  VarDecl(var_decl("p" None Pointer(Int) None None None None false))
  VarDecl(var_decl("q" AddressOf(Subscript(Var("p" None) Constant(ConstInt(1) None) None) None) Pointer(Int) Static None None None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "initializer for static-storage object 'q' is not an address constant: Var(name='p', data_type=Pointer(referenced_type=Int()))"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("@0.a" None Array(Int 0) None None None None false)))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "object '@0.a': incomplete array type (`[]`) is allowed only at the outermost type of an `extern` declaration"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  VarDecl(var_decl("u" InitList([Unary(Negate Constant(ConstInt(1) None) None) Constant(ConstInt(0) None)] None None) Array(Const(UChar) 0) None None None None true))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "the initializer of `constexpr` object 'u' has the value -1, which unsigned char can't hold exactly, and a constexpr initializer can't change a value (C23 \u00a76.7.2p5)"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  VarDecl(var_decl("t" None Array(Int 0) None None None None false))
  FunctionDecl(function_decl("main" [] Block([
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "file-scope object 't': incomplete array type (`[]`) is allowed only at the outermost type of an `extern` declaration"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("@0.t" None Array(Int 0) None None None None false)))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "object '@0.t': incomplete array type (`[]`) is allowed only at the outermost type of an `extern` declaration"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  VarDecl(var_decl("a" InitList([InitList([InitList([Constant(ConstInt(1) None)] None None)] None None)] None None) Array(Int 2) None None None None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "unexpected nested initializer for element 0 of 'a': a scalar's initializer is one expression, optionally in one pair of braces, or the empty initializer `{}` (C23 \u00a76.7.11p12)"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  VarDecl(var_decl("a" None Array(UChar 91) None None ZeroPage(None) None false))
  FunctionDecl(function_decl("main" [] Block([
    S(Return(Var("a" None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "cannot implicitly convert pointer type Pointer(referenced_type=UChar()) to arithmetic type Int(); use an explicit cast"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(Return(Cast(FunType([] Int) Constant(ConstInt(0) None) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "cast target type must be an object type (Int / Long / LongLong / UInt / ULong / ULongLong / Float / Double / Pointer) or void, got FunType(params=[], ret=Int())"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  VarDecl(var_decl("g" None Int None None None ".a" false))
  VarDecl(var_decl("g" None Int None None None ".b" false))
  FunctionDecl(function_decl("main" [] Block([
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "two declarations of 'g' put it in different sections, '.a' and '.b'"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  VarDecl(var_decl("E" Binary(Divide Constant(ConstDouble(4607182418800017408) None) Constant(ConstDouble(4613937818241073152) None) None) Const(Float) None None None None true))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "the initializer of `constexpr` object 'E' has a double value that float can't hold exactly, and a constexpr initializer can't change a value (C23 \u00a76.7.2p5)"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  VarDecl(var_decl("p" None Pointer(Int) None None None None false))
  FunctionDecl(function_decl("main" [] Block([
    S(Expression(Unary(Plus Var("p" None) None)))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "unary '+' is not defined on pointer type Pointer(referenced_type=Int())"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  FunctionDecl(function_decl("f" ["@0.p"] Block([
    S(Return(Binary(Add Var("@0.p" None) Constant(ConstInt(1) None) None)))
  ]) FunType([Pointer(Const(Void))] Pointer(Const(Void))) None None [""] false None [] None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "binary '+' is not defined on void pointer operands (Pointer(referenced_type=Const(referenced_type=Void())) + Int())"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  VarDecl(var_decl("a" None Array(Int 2) None None None None false))
  FunctionDecl(function_decl("main" [] Block([
    S(Return(IndirectCall(Subscript(Var("a" None) Constant(ConstInt(0) None) None) [] None false)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "called object is not a function or function pointer (type Int())"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  StructDecl(struct_decl("t" false [member_decl("a" Int)]))
  VarDecl(var_decl("v" InitList([InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None)] None None) Structure("t") None None None None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "too many initializers for member 'a' of 'v': 2 given, and a scalar's braces hold one expression (C23 \u00a76.7.11p12)"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  FunctionDecl(function_decl("f" ["@0.x"] Block([
    S(Return(Dereference(CompoundLiteral("@1.compound_literal" Array(Int 0) InitList([Var("@0.x" None)] None None) None Static false) None)))
  ]) FunType([Int] Int) None None [""] false None [] None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "initializer for static-storage object '(compound literal)' is not a constant expression"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  VarDecl(var_decl("K" None Int None None None None false))
  VarDecl(var_decl("K" Constant(ConstInt(1) None) Const(Int) None None None None true))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "'K' is declared `constexpr` after another declaration of it, and c6502 takes a `constexpr` declaration only as the one declaration of a name (C23 \u00a76.7.1p12 leaves any other to the implementation)"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  FunctionDecl(function_decl("f" ["@0.p" "@1.q"] Block([
    S(Return(Binary(LessThan Var("@0.p" None) Var("@1.q" None) None)))
  ]) FunType([Pointer(Const(Void)) Pointer(Int)] Int) None None ["" ""] false None [] None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "comparison of distinct pointer types: Pointer(referenced_type=Const(referenced_type=Void())) < Pointer(referenced_type=Int())"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  VarDecl(var_decl("t" Constant(ConstInt(5) None) Array(Int 0) None None None None false))
  FunctionDecl(function_decl("main" [] Block([
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "file-scope object 't': incomplete array type (`[]`) is allowed only at the outermost type of an `extern` declaration"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  FunctionDecl(function_decl("f" ["@0.a"] Block([]) FunType([UInt] Void) None None ["Y"] false None [] None false))
  FunctionDecl(function_decl("main" [] Block([
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "parameter 'a' is 2 bytes, but `in_y` names 1 register; name one register for each byte, low byte first"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("@0.f" None Float None None None None false)))
    S(Expression(Assignment(Var("@0.f" None) Constant(ConstFloat(1069547520) None) None)))
    S(Return(Unary(Complement Var("@0.f" None) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "unary '~' requires integer operand, got Float"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  VarDecl(var_decl("d" None Double None None None None false))
  FunctionDecl(function_decl("f" [] Block([
    S(Expression(FunctionCall("__c6502_bin_to_bcd" [Var("d" None)] None false)))
  ]) FunType([] Void) None None [] false None [] None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "the argument of __c6502_bin_to_bcd has type double; it converts an integer to packed BCD, and takes an argument of integer type"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  FunctionDecl(function_decl("f" ["@0.a"] None FunType([Short] Int) None None [""] false None [] None false))
  FunctionDecl(function_decl("f" ["@1.a"] Block([
    S(Return(Var("@1.a" None)))
  ]) FunType([Int] Int) None None [""] false None [] None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "incompatible redeclaration of 'f': previous FunType(params=[Short()], ret=Int()), new FunType(params=[Int()], ret=Int())"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  FunctionDecl(function_decl("f" ["@0.p"] Block([]) FunType([Pointer(UChar)] Void) None None ["Y"] false None [] None false))
  FunctionDecl(function_decl("main" [] Block([
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "parameter 'p' is 2 bytes, but `in_y` names 1 register; name one register for each byte, low byte first"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  VarDecl(var_decl("x" Constant(ConstInt(1) None) Int None None None None false))
  VarDecl(var_decl("x" Constant(ConstInt(2) None) Int None None None None false))
  FunctionDecl(function_decl("main" [] Block([
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "redefinition of object 'x': prior initializer 1, new initializer 2"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  VarDecl(var_decl("a" None UChar None None None None false))
  FunctionDecl(function_decl("f" [] Block([
    S(Expression(FunctionCall("__c6502_bcd_add" [Var("a" None) Unary(Negate Constant(ConstInt(1) None) None)] None false)))
  ]) FunType([] Void) None None [] false None [] None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "argument 2 of __c6502_bcd_add is the negative constant -1; packed BCD operands are unsigned"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  VarDecl(var_decl("d" None Double None None None None false))
  FunctionDecl(function_decl("f" [] Block([
    S(Expression(FunctionCall("__c6502_bcd_add" [Var("d" None) Constant(ConstInt(1) None)] None false)))
  ]) FunType([] Void) None None [] false None [] None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "argument 1 of __c6502_bcd_add has floating type double; packed BCD operands must have an unsigned integer type"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("a" InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None) Constant(ConstInt(3) None)] None None) Array(Int 3) Extern None None None false)))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "block-scope `extern` declaration of 'a' may not have an initializer"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  FunctionDecl(function_decl("f" [] Block([
    S(Return(CompoundLiteral("@0.compound_literal" Int InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) None None false)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "too many initializers for scalar variable '(compound literal)': 2 given, and a scalar's braces hold one expression (C23 \u00a76.7.11p12)"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("@0.t" InitList([] None None) Array(Int 0) Static None None None false)))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "array 't' has unknown size, so it can't be initialized by the empty initializer `{}`, which has no elements to give it a size (C23 \u00a76.7.11p4)"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  VarDecl(var_decl("t" None Array(UChar 2) None None None None false))
  FunctionDecl(function_decl("f" [] Block([
    S(Expression(FunctionCall("__c6502_bin_to_bcd" [Var("t" None)] None false)))
  ]) FunType([] Void) None None [] false None [] None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "the argument of __c6502_bin_to_bcd has type unsigned char *; it converts an integer to packed BCD, and takes an argument of integer type"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("@0.a" InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None) Constant(ConstInt(3) None)] None None) Array(Int 2) None None None None false)))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "too many initializers for array '@0.a': 3 given, array has 2 elements"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("@0.a" Constant(ConstInt(1) None) Int None None None None false)))
    D(VarDecl(var_decl("@1.x" Var("@0.a" None) Int Static None None None false)))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "initializer for static-storage object '@1.x' is not a constant expression"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  VarDecl(var_decl("p" None Pointer(UChar) None None None None false))
  FunctionDecl(function_decl("f" [] Block([
    S(Expression(FunctionCall("__c6502_bcd_add" [Var("p" None) Constant(ConstInt(1) None)] None false)))
  ]) FunType([] Void) None None [] false None [] None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "argument 1 of __c6502_bcd_add is a pointer (unsigned char *); packed BCD operands must have an unsigned integer type"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  VarDecl(var_decl("fp" None Pointer(FunType([] Int)) None None None None false))
  FunctionDecl(function_decl("main" [] Block([
    S(Return(IndirectCall(AddressOf(Var("fp" None) None) [] None false)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "called object is not a function or function pointer (type Pointer(referenced_type=Pointer(referenced_type=FunType(params=[], ret=Int()))))"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  VarDecl(var_decl("a" None UChar None None None None false))
  FunctionDecl(function_decl("f" [] Block([
    S(Expression(FunctionCall("__c6502_bcd_add" [Var("a" None) Constant(ConstInt(256) None)] None false)))
  ]) FunType([] Void) None None [] false None [] None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "argument 2 of __c6502_bcd_add, 0x100, doesn't fit in unsigned char, the type of the result (cast the other operand to widen it)"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  VarDecl(var_decl("K" Constant(ConstInt(1) None) Const(Int) None None None None true))
  FunctionDecl(function_decl("f" [] Block([
    S(Expression(Assignment(Var("K" None) Constant(ConstInt(3) None) None)))
  ]) FunType([] Void) None None [] false None [] None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "cannot assign to const-qualified lvalue: Var(name='K', data_type=Const(referenced_type=Int())) has type Const(referenced_type=Int())"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  VarDecl(var_decl("x" None BitInt(3 true) None None None None false))
  VarDecl(var_decl("p" AddressOf(Var("x" None) None) Pointer(BitInt(4 true)) None None None None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "cannot implicitly convert Pointer(referenced_type=BitInt(width=3, is_signed=True)) to Pointer(referenced_type=BitInt(width=4, is_signed=True)); pointer-to-pointer assignment requires matching pointee types per C99 \u00a76.5.16.1.1"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  VarDecl(var_decl("a" None Array(Int 4) None None None None false))
  VarDecl(var_decl("i" None Int None None None None false))
  VarDecl(var_decl("q" AddressOf(Subscript(Var("a" None) Var("i" None) None) None) Pointer(Int) Static None None None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "initializer for static-storage object 'q' is not a constant expression: expression is not an integer constant expression: Var(name='i', data_type=Int())"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("@0.a" None Int None None None None false)))
    D(VarDecl(var_decl("@1.p" None Pointer(Int) None None None None false)))
    S(Return(Binary(Modulo Var("@0.a" None) Var("@1.p" None) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "binary '%' is not defined on pointer operands (Int() % Pointer(referenced_type=Int()))"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(FunctionDecl(function_decl("foo" [] None FunType([] Int) None None [] false None [] None false)))
    S(Return(Binary(Add Var("foo" None) Constant(ConstInt(1) None) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "binary '+' is not defined on pointer-to-function operands (Pointer(referenced_type=FunType(params=[], ret=Int())) + Int())"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  VarDecl(var_decl("a" None Array(Int 3) None None None None false))
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("a" None Array(Int 5) Extern None None None false)))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "incompatible redeclaration of 'a': previous Array(element_type=Int(), size=3), new Array(element_type=Int(), size=5)"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("@0.a" None UChar None None ZeroPage(None) None false)))
    S(Expression(Assignment(Var("@0.a" None) Constant(ConstInt(1) None) None)))
    S(Return(Var("@0.a" None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "`[[c6502::zeropage]]` needs an object with static storage; 'a' is automatic (declare it `static` or at file scope)"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  FunctionDecl(function_decl("f" [] Block([
    S(Return(CompoundLiteral("@0.compound_literal" Const(UInt) Unary(Negate Constant(ConstInt(1) None) None) None None true)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "the initializer of `constexpr` object '(compound literal)' has the value -1, which unsigned int can't hold exactly, and a constexpr initializer can't change a value (C23 \u00a76.7.2p5)"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(FunctionDecl(function_decl("foo" [] None FunType([] Int) None None [] false None [] None false)))
    D(VarDecl(var_decl("@0.x" FunctionCall("foo" [] None false) Int Static None None None false)))
    S(Return(Var("@0.x" None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "initializer for static-storage object '@0.x' is not a constant expression"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  VarDecl(var_decl("x" Constant(ConstInt(5) None) Const(Int) None None None None false))
  FunctionDecl(function_decl("main" [] Block([
    S(Expression(Prefix(Decrement Var("x" None) None)))
    S(Return(Var("x" None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "cannot use ++/-- on const-qualified lvalue: Var(name='x', data_type=Const(referenced_type=Int())) has type Const(referenced_type=Int())"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  VarDecl(var_decl("a" None SChar None None None None false))
  FunctionDecl(function_decl("f" [] Block([
    S(Expression(FunctionCall("__c6502_bcd_add" [Var("a" None) Var("a" None)] None false)))
  ]) FunType([] Void) None None [] false None [] None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "argument 1 of __c6502_bcd_add has the signed type signed char; packed BCD operands must have an unsigned integer type (cast it, or give a constant a `u` suffix)"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  VarDecl(var_decl("b" None Bool None None None None false))
  FunctionDecl(function_decl("f" [] Block([
    S(Expression(FunctionCall("__c6502_bcd_add" [Var("b" None) Constant(ConstInt(1) None)] None false)))
  ]) FunType([] Void) None None [] false None [] None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "argument 1 of __c6502_bcd_add has type bool; packed BCD operands must have an unsigned integer type other than bool and plain char (use unsigned char)"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  VarDecl(var_decl("c" None Char None None None None false))
  FunctionDecl(function_decl("f" [] Block([
    S(Expression(FunctionCall("__c6502_bcd_add" [Var("c" None) Constant(ConstInt(1) None)] None false)))
  ]) FunType([] Void) None None [] false None [] None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "argument 1 of __c6502_bcd_add has type char; packed BCD operands must have an unsigned integer type other than bool and plain char (use unsigned char)"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  VarDecl(var_decl("a" None Int None None None None false))
  FunctionDecl(function_decl("f" [] Block([
    S(Expression(FunctionCall("__c6502_bcd_sub" [Constant(ConstInt(1) None) Var("a" None)] None false)))
  ]) FunType([] Void) None None [] false None [] None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "argument 2 of __c6502_bcd_sub has the signed type int; packed BCD operands must have an unsigned integer type (cast it, or give a constant a `u` suffix)"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  FunctionDecl(function_decl("f" ["@0.p" "@1.q"] Block([
    S(Return(Binary(Equal Var("@0.p" None) Var("@1.q" None) None)))
  ]) FunType([Pointer(Pointer(Const(UChar))) Pointer(Pointer(UChar))] Int) None None ["" ""] false None [] None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "comparison of distinct pointer types: Pointer(referenced_type=Pointer(referenced_type=Const(referenced_type=UChar()))) vs Pointer(referenced_type=Pointer(referenced_type=UChar()))"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  VarDecl(var_decl("t" None Array(Pointer(FunType([Int] Int)) 1) None None None None false))
  FunctionDecl(function_decl("main" [] Block([
    S(Return(IndirectCall(Subscript(Var("t" None) Constant(ConstInt(0) None) None) [Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None false)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "function through a function pointer called with 2 arguments, expected 1"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  FunctionDecl(function_decl("f" [] Block([
    S(ForStmt(InitDecl([var_decl("@0.i" InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) Int None None None None false)]) None None Return(Constant(ConstInt(0) None)) "" None))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "too many initializers for scalar variable 'i': 2 given, and a scalar's braces hold one expression (C23 \u00a76.7.11p12)"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  VarDecl(var_decl("g" None Int None None None None false))
  FunctionDecl(function_decl("f" [] Block([
    S(Return(Dereference(CompoundLiteral("@0.compound_literal" Const(Pointer(Int)) AddressOf(Var("g" None) None) None None true) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "`constexpr` object '(compound literal)' holds a pointer that isn't null, and a constexpr pointer must be null (C23 \u00a76.7.2p6)"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  VarDecl(var_decl("x" None BitInt(8 false) None None None None false))
  FunctionDecl(function_decl("f" [] Block([
    S(Return(FunctionCall("__c6502_bcd_add" [Var("x" None) Constant(ConstUInt(1) None)] None false)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "argument 1 of __c6502_bcd_add has the bit-precise type unsigned _BitInt(8); packed BCD operands must have a standard unsigned integer type (cast it to one)"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  VarDecl(var_decl("x" Constant(ConstInt(5) None) Const(Int) None None None None false))
  FunctionDecl(function_decl("main" [] Block([
    S(Expression(Assignment(Var("x" None) Constant(ConstInt(10) None) None)))
    S(Return(Var("x" None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "cannot assign to const-qualified lvalue: Var(name='x', data_type=Const(referenced_type=Int())) has type Const(referenced_type=Int())"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("@0.p" None Pointer(Int) None None None None false)))
    S(IfStmt(Binary(LessThan Var("@0.p" None) Constant(ConstInt(0) None) None) Return(Constant(ConstInt(1) None)) None))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "binary '<' between pointer and non-pointer operand: Pointer(referenced_type=Int()) < Int()"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("@0.p" None Pointer(Int) None None None None false)))
    D(VarDecl(var_decl("@1.q" None Pointer(Int) None None None None false)))
    S(Return(Binary(Divide Var("@0.p" None) Var("@1.q" None) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "binary '/' is not defined on pointer operands (Pointer(referenced_type=Int()) / Pointer(referenced_type=Int()))"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("@0.a" InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None) Constant(ConstInt(3) None)] None None) Array(Array(Int 3) 3) None None None None false)))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "expected nested initializer (`{...}`) at index 0 of '@0.a'; element type is Array(element_type=Int(), size=3)"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  StructDecl(struct_decl("s" false [member_decl("x" Int)]))
  VarDecl(var_decl("v" None Structure("s") None None None None false))
  FunctionDecl(function_decl("f" [] Block([
    S(Expression(FunctionCall("__c6502_bcd_add" [Var("v" None) Constant(ConstInt(1) None)] None false)))
  ]) FunType([] Void) None None [] false None [] None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "argument 1 of __c6502_bcd_add has type struct s; packed BCD operands must have an unsigned integer type"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("@0.p" None Pointer(Int) None None None None false)))
    S(IfStmt(Binary(Equal Var("@0.p" None) Constant(ConstInt(5) None) None) Return(Constant(ConstInt(1) None)) None))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "comparison between pointer and non-null-constant non-pointer: Pointer(referenced_type=Int()) vs Int()"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  StructDecl(struct_decl("s" false [member_decl("x" Int)]))
  VarDecl(var_decl("v" None Structure("s") None None None None false))
  FunctionDecl(function_decl("f" [] Block([
    S(Expression(FunctionCall("__c6502_bin_to_bcd" [Var("v" None)] None false)))
  ]) FunType([] Void) None None [] false None [] None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "the argument of __c6502_bin_to_bcd has type struct s; it converts an integer to packed BCD, and takes an argument of integer type"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  FunctionDecl(function_decl("f" [] Block([
    S(Return(CompoundLiteral("@0.compound_literal" Int InitList([InitList([Constant(ConstInt(1) None)] None None)] None None) None None false)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "unexpected nested initializer for scalar variable '(compound literal)': a scalar's initializer is one expression, optionally in one pair of braces, or the empty initializer `{}` (C23 \u00a76.7.11p12)"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  StructDecl(struct_decl("s" false [member_decl("a" UChar) member_decl("b" UChar)]))
  FunctionDecl(function_decl("f" ["@0.v"] Block([]) FunType([Structure("s")] Void) None None ["XY"] false None [] None false))
  FunctionDecl(function_decl("main" [] Block([
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "parameter 'v': `in_xy` passes an integer or a pointer, one byte in each register it names"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("@0.local" None Int None None None None false)))
    D(VarDecl(var_decl("@1.p" AddressOf(Var("@0.local" None) None) Pointer(Int) Static None None None false)))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "initializer for static-storage object '@1.p' takes the address of '@0.local', which doesn't have static storage duration"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("@0.a" None Int None None None None false)))
    D(VarDecl(var_decl("@1.p" AddressOf(Var("@0.a" None) None) Pointer(Int) None None None None false)))
    S(Return(Cast(Int Binary(LeftShift Var("@1.p" None) Constant(ConstInt(1) None) None) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "binary '<<' requires integer operands (Pointer(referenced_type=Int()) << Int())"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("@0.p" None Pointer(Int) None None None None false)))
    D(VarDecl(var_decl("@1.q" None Pointer(Int) None None None None false)))
    S(Return(Subscript(Var("@0.p" None) Var("@1.q" None) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "subscript needs a pointer/array operand and an integer operand; got Pointer(referenced_type=Int()) and Pointer(referenced_type=Int())"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("@0.a" InitList([InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None) Constant(ConstInt(3) None)] None None)] None None) Array(Int 3) None None None None false)))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "too many initializers for element 0 of 'a': 3 given, and a scalar's braces hold one expression (C23 \u00a76.7.11p12)"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  FunctionDecl(function_decl("f" ["@0.p"] Block([]) FunType([Pointer(UChar)] Void) None None ["auto"] false None [] None false))
  FunctionDecl(function_decl("main" [] Block([
    S(Expression(FunctionCall("f" [Constant(ConstInt(0) None)] None false)))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "register parameter 'p': a pointer can't be placed in a register (`register` is not allowed on pointer types)"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  VarDecl(var_decl("x" None Int None None None None false))
  VarDecl(var_decl("p" CompoundLiteral("@0.compound_literal" Array(Int 0) InitList([Var("x" None)] None None) None None false) Pointer(Int) None None None None false))
  FunctionDecl(function_decl("main" [] Block([
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "initializer for static-storage object '(compound literal)' is not a constant expression"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("@0.y" None Long None None None None false)))
    D(VarDecl(var_decl("@1.lp" None Pointer(Long) None None None None false)))
    S(Expression(Assignment(Var("@1.lp" None) AddressOf(Var("@0.y" None) None) None)))
    S(Return(Unary(Negate Var("@1.lp" None) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "unary '-' is not defined on pointer type Pointer(referenced_type=Long())"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  VarDecl(var_decl("x" Constant(ConstInt(5) None) Const(Int) None None None None false))
  FunctionDecl(function_decl("main" [] Block([
    S(Expression(CompoundAssignment(Add Var("x" None) Constant(ConstInt(1) None) None None)))
    S(Return(Var("x" None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "cannot modify const-qualified lvalue with compound assignment: Var(name='x', data_type=Const(referenced_type=Int())) has type Const(referenced_type=Int())"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("@0.a" Constant(ConstInt(0) None) Int None None None None false)))
    D(VarDecl(var_decl("@1.p" AddressOf(Var("@0.a" None) None) Pointer(Int) None None None None false)))
    S(Return(Binary(LessThan Constant(ConstInt(0) None) Var("@1.p" None) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "binary '<' between pointer and non-pointer operand: Int() < Pointer(referenced_type=Int())"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("@0.a" None Array(Int 10) None None None None false)))
    D(VarDecl(var_decl("@1.i" Constant(ConstDouble(4607182418800017408) None) Double None None None None false)))
    S(Return(Subscript(Var("@0.a" None) Var("@1.i" None) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "subscript needs a pointer/array operand and an integer operand; got Pointer(referenced_type=Int()) and Double()"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("@0.p" CompoundLiteral("@1.compound_literal" Array(Int 2) InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None) Constant(ConstInt(3) None)] None None) None None false) Pointer(Int) None None None None false)))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "too many initializers for array '(compound literal)': 3 given, array has 2 elements"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("@0.a" None Array(Int 10) None None None None false)))
    D(VarDecl(var_decl("@1.b" None Array(Int 10) None None None None false)))
    S(Expression(Assignment(Var("@0.a" None) Var("@1.b" None) None)))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "cannot assign to an array (use subscript): Var(name='@0.a', data_type=Array(element_type=Int(), size=10))"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("@0.a" InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None) Constant(ConstInt(3) None) Constant(ConstInt(4) None) Constant(ConstInt(5) None) Constant(ConstInt(6) None)] None None) Array(Array(Int 3) 2) None None None None false)))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "too many initializers for array '@0.a': 6 given, array has 2 elements"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(ForStmt(InitDecl([var_decl("@0.i" Constant(ConstInt(0) None) Int None None ZeroPage(None) None false)]) Binary(LessThan Var("@0.i" None) Constant(ConstInt(2) None) None) Postfix(Increment Var("@0.i" None) None) Null ".loop@0" None))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "`[[c6502::zeropage]]` and `section` need an object with static storage; for-init '@0.i' is automatic"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  FunctionDecl(function_decl("f" [] Block([
    D(VarDecl(var_decl("@0.x" Constant(ConstInt(5) None) Int None None None None false)))
    S(Return(CompoundLiteral("@1.compound_literal" Const(Int) Var("@0.x" None) None None true)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "the initializer of `constexpr` object '(compound literal)' isn't an integer constant expression (C23 \u00a76.7.2p6): expression is not an integer constant expression: Var(name='@0.x', data_type=Int())"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("@0.a" Constant(ConstInt(0) None) Int None None None None false)))
    D(VarDecl(var_decl("@1.p" AddressOf(Var("@0.a" None) None) Pointer(Int) None None None None false)))
    S(Return(Cast(Long Binary(Subtract Constant(ConstInt(1) None) Var("@1.p" None) None) None)))
  ]) FunType([] Long) None None [] false None [] None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "binary '-' between integer and pointer is not defined (Int() - Pointer(referenced_type=Int()))"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("@0.p" CompoundLiteral("@1.compound_literal" Array(Int 0) InitList([Constant(ConstInt(1) None)] None None) None None false) Pointer(Int) Static None None None false)))
    S(Return(Dereference(Var("@0.p" None) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "initializer for static-storage object '@0.p' takes the address of a compound literal inside a function, which doesn't have static storage duration"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  FunctionDecl(function_decl("f" [] Block([
    D(VarDecl(var_decl("@0.x" Constant(ConstInt(1) None) Int None None None ".d" false)))
    S(Return(Var("@0.x" None)))
  ]) FunType([] Int) None None [] false None [] None false))
  FunctionDecl(function_decl("main" [] Block([
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "`[[c6502::section(...)]]` needs an object with static storage; 'x' is automatic (declare it `static` or at file scope)"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  FunctionDecl(function_decl("f" [] Block([
    D(VarDecl(var_decl("@0.y" Constant(ConstInt(5) None) Int None None None None false)))
    D(VarDecl(var_decl("@1.z" Var("@0.y" None) Const(Int) None None None None true)))
    S(Return(Var("@1.z" None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "the initializer of `constexpr` object 'z' isn't an integer constant expression (C23 \u00a76.7.2p6): expression is not an integer constant expression: Var(name='@0.y', data_type=Int())"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("@0.x" None Int None None None None false)))
    D(VarDecl(var_decl("@1.p" None Pointer(Int) None None None None false)))
    S(Expression(Assignment(Var("@1.p" None) AddressOf(Var("@0.x" None) None) None)))
    S(Return(Binary(Multiply Var("@1.p" None) Constant(ConstInt(2) None) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "binary '*' is not defined on pointer operands (Pointer(referenced_type=Int()) * Int())"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("@0.p" None Pointer(Int) None None None None false)))
    D(VarDecl(var_decl("@1.q" None Pointer(Long) None None None None false)))
    S(IfStmt(Binary(Equal Var("@0.p" None) Var("@1.q" None) None) Return(Constant(ConstInt(1) None)) None))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "comparison of distinct pointer types: Pointer(referenced_type=Int()) vs Pointer(referenced_type=Long())"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("@0.p" None Pointer(Int) None None None None false)))
    D(VarDecl(var_decl("@1.q" None Pointer(Long) None None None None false)))
    S(IfStmt(Binary(LessThan Var("@0.p" None) Var("@1.q" None) None) Return(Constant(ConstInt(1) None)) None))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "comparison of distinct pointer types: Pointer(referenced_type=Int()) < Pointer(referenced_type=Long())"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("@0.a" Constant(ConstInt(0) None) Int None None None None false)))
    D(VarDecl(var_decl("@1.p" AddressOf(Var("@0.a" None) None) Pointer(Int) None None None None false)))
    S(Return(Cast(Long Binary(Add Var("@1.p" None) Constant(ConstDouble(4607182418800017408) None) None) None)))
  ]) FunType([] Long) None None [] false None [] None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "binary '+' is not defined on a pointer and a floating-point operand (Pointer(referenced_type=Int()) + Double())"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("@0.a" Constant(ConstInt(0) None) Int None None None None false)))
    D(VarDecl(var_decl("@1.p" AddressOf(Var("@0.a" None) None) Pointer(Int) None None None None false)))
    S(Return(Cast(Long Binary(Subtract Var("@1.p" None) Constant(ConstDouble(4607182418800017408) None) None) None)))
  ]) FunType([] Long) None None [] false None [] None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "binary '-' is not defined on a pointer and a floating-point operand (Pointer(referenced_type=Int()) - Double())"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  VarDecl(var_decl("p" None Pointer(Short) None None None None false))
  VarDecl(var_decl("q" None Pointer(Int) None None None None false))
  FunctionDecl(function_decl("main" [] Block([
    S(Expression(Assignment(Var("q" None) Var("p" None) None)))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "cannot implicitly convert Pointer(referenced_type=Short()) to Pointer(referenced_type=Int()); pointer-to-pointer assignment requires matching pointee types per C99 \u00a76.5.16.1.1"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  FunctionDecl(function_decl("f" ["@0.k"] Block([
    S(SwitchStmt(Var("@0.k" None) Compound(Block([
      S(CaseStmt(Constant(ConstInt(1) None) CaseStmt(Constant(ConstInt(5) None) Return(Constant(ConstInt(1) None)) ".case@2") ".case@1"))
    ])) ".switch@0" [switch_case(Constant(ConstInt(1) None) ".case@1") switch_case(Constant(ConstInt(5) None) ".case@2")] None None))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([BitInt(2 false)] Int) None None [""] false None [] None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "duplicate case value 1 in switch statement (C99 \u00a76.8.4.2.3)"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("@0.ip" None Pointer(Int) None None None None false)))
    D(VarDecl(var_decl("@1.lp" None Pointer(Long) None None None None false)))
    S(Expression(Assignment(Var("@0.ip" None) Var("@1.lp" None) None)))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "cannot implicitly convert Pointer(referenced_type=Long()) to Pointer(referenced_type=Int()); pointer-to-pointer assignment requires matching pointee types per C99 \u00a76.5.16.1.1"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  FunctionDecl(function_decl("f" ["@0.x"] Block([
    S(SwitchStmt(Var("@0.x" None) Compound(Block([
      S(CaseStmt(Constant(ConstUChar(200) None) CaseStmt(Constant(ConstInt(200) None) Return(Constant(ConstInt(1) None)) ".case@2") ".case@1"))
    ])) ".switch@0" [switch_case(Constant(ConstUChar(200) None) ".case@1") switch_case(Constant(ConstInt(200) None) ".case@2")] None None))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([UChar] Int) None None [""] false None [] None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "duplicate case value 200 in switch statement (C99 \u00a76.8.4.2.3)"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("@0.f" None Float None None None None false)))
    D(VarDecl(var_decl("@1.p" None Pointer(Int) None None None None false)))
    S(Expression(Assignment(Var("@0.f" None) Constant(ConstFloat(1069547520) None) None)))
    S(Expression(Assignment(Var("@1.p" None) Cast(Pointer(Int) Var("@0.f" None) None) None)))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "cannot cast between pointer and floating-point type: Float() \u2192 Pointer(referenced_type=Int())"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  FunctionDecl(function_decl("fst" ["@0.x"] Block([
    D(VarDecl(var_decl("@1.p" AddressOf(Var("@0.x" None) None) Pointer(Char) None None None None false)))
    S(Return(Dereference(Var("@1.p" None) None)))
  ]) FunType([Char] Char) None None ["auto"] false None [] None false))
  FunctionDecl(function_decl("main" [] Block([
    S(Return(FunctionCall("fst" [Constant(ConstInt(5) None)] None false)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "can't take the address of `@0.x`: declared `register` so it lives in a 6502 register, not memory (C99 \u00a76.5.3.2.1)"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("@0.d" None Double None None None None false)))
    D(VarDecl(var_decl("@1.lp" None Pointer(Long) None None None None false)))
    S(Expression(Assignment(Var("@0.d" None) Constant(ConstDouble(4612811918334230528) None) None)))
    S(Expression(Assignment(Var("@1.lp" None) Cast(Pointer(Long) Var("@0.d" None) None) None)))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "cannot cast between pointer and floating-point type: Double() \u2192 Pointer(referenced_type=Long())"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  StructDecl(struct_decl("S" false [member_decl("x" Const(Int)) member_decl("y" Int)]))
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("@0.s" None Structure("S") None None None None false)))
    S(Expression(Assignment(Dot(Var("@0.s" None) "x" None) Constant(ConstInt(5) None) None)))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "cannot assign to const-qualified lvalue: Dot(operand=Var(name='@0.s', data_type=Structure(tag='S')), member='x', data_type=Const(referenced_type=Int())) has type Const(referenced_type=Int())"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("@0.a" Constant(ConstInt(0) None) Int None None None None false)))
    D(VarDecl(var_decl("@1.p" AddressOf(Var("@0.a" None) None) Pointer(Int) None None None None false)))
    D(VarDecl(var_decl("@2.q" AddressOf(Var("@0.a" None) None) Pointer(Int) None None None None false)))
    S(Return(Cast(Int Binary(Add Var("@1.p" None) Var("@2.q" None) None) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "binary '+' is not defined on two pointer operands (Pointer(referenced_type=Int()) + Pointer(referenced_type=Int()))"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  FunctionDecl(function_decl("f" [] Block([
    D(VarDecl(var_decl("@0.K" CompoundLiteral("@1.compound_literal" Int Constant(ConstInt(5) None) None None false) Const(Int) None None None None true)))
    S(Return(Var("@0.K" None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "the initializer of `constexpr` object 'K' isn't an integer constant expression (C23 \u00a76.7.2p6): expression is not an integer constant expression: CompoundLiteral(name='@1.compound_literal', target_type=Int(), init=Constant(const=ConstInt(value=5), data_type=Int()), data_type=Int(), storage_class=None, is_constexpr=False)"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("@0.x" None Int None None None None false)))
    D(VarDecl(var_decl("@1.p" None Pointer(Int) None None None None false)))
    D(VarDecl(var_decl("@2.f" None Float None None None None false)))
    S(Expression(Assignment(Var("@1.p" None) AddressOf(Var("@0.x" None) None) None)))
    S(Expression(Assignment(Var("@2.f" None) Cast(Float Var("@1.p" None) None) None)))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "cannot cast between pointer and floating-point type: Pointer(referenced_type=Int()) \u2192 Float()"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("@0.x" Constant(ConstInt(5) None) Int None None None None false)))
    D(VarDecl(var_decl("@1.p" AddressOf(Var("@0.x" None) None) Pointer(Const(Int)) None None None None false)))
    S(Expression(Assignment(Dereference(Var("@1.p" None) None) Constant(ConstInt(10) None) None)))
    S(Return(Dereference(Var("@1.p" None) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "cannot assign to const-qualified lvalue: Dereference(exp=Var(name='@1.p', data_type=Pointer(referenced_type=Const(referenced_type=Int()))), data_type=Const(referenced_type=Int())) has type Const(referenced_type=Int())"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("@0.a" Constant(ConstInt(0) None) Int None None None None false)))
    D(VarDecl(var_decl("@1.b" Cast(Long Constant(ConstInt(0) None) None) Long None None None None false)))
    D(VarDecl(var_decl("@2.p" AddressOf(Var("@0.a" None) None) Pointer(Int) None None None None false)))
    D(VarDecl(var_decl("@3.q" AddressOf(Var("@1.b" None) None) Pointer(Long) None None None None false)))
    S(Return(Binary(Subtract Var("@2.p" None) Var("@3.q" None) None)))
  ]) FunType([] Long) None None [] false None [] None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "subtraction of distinct pointer types: Pointer(referenced_type=Int()) - Pointer(referenced_type=Long())"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  StructDecl(struct_decl("S" false [member_decl("x" Int) member_decl("y" Int)]))
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("@0.s" InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) Const(Structure("S")) None None None None false)))
    S(Expression(Assignment(Dot(Var("@0.s" None) "y" None) Constant(ConstInt(99) None) None)))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "cannot assign to const-qualified lvalue: Dot(operand=Var(name='@0.s', data_type=Const(referenced_type=Structure(tag='S'))), member='y', data_type=Const(referenced_type=Int())) has type Const(referenced_type=Int())"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("@0.x" Constant(ConstInt(5) None) Int None None None None false)))
    D(VarDecl(var_decl("@1.y" Constant(ConstInt(10) None) Int None None None None false)))
    D(VarDecl(var_decl("@2.p" AddressOf(Var("@0.x" None) None) Const(Pointer(Int)) None None None None false)))
    S(Expression(Assignment(Var("@2.p" None) AddressOf(Var("@1.y" None) None) None)))
    S(Return(Dereference(Var("@2.p" None) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "cannot assign to const-qualified lvalue: Var(name='@2.p', data_type=Const(referenced_type=Pointer(referenced_type=Int()))) has type Const(referenced_type=Pointer(referenced_type=Int()))"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  StructDecl(struct_decl("u" true [member_decl("a" Int) member_decl("c" Char)]))
  VarDecl(var_decl("U" InitList([Constant(ConstInt(1) None)] None None) Const(Union("u")) None None None None true))
  FunctionDecl(function_decl("f" ["@0.x"] Block([
    S(SwitchStmt(Var("@0.x" None) Compound(Block([
      S(CaseStmt(Dot(Var("U" None) "c" None) Return(Constant(ConstInt(1) None)) ".case@1"))
    ])) ".switch@0" [switch_case(Dot(Var("U" None) "c" None) ".case@1")] None None))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([Int] Int) None None [""] false None [] None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "case label is not an integer constant expression: member 'c' of a union constant isn't the member its initializer sets, and only that one is a constant (C23 \u00a76.6p16)"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  FunctionDecl(function_decl("main" ["@0.x"] Block([
    D(VarDecl(var_decl("@1.y" None Long None None None None false)))
    S(SwitchStmt(Var("@0.x" None) Compound(Block([
      S(CaseStmt(SizeOfExp(Var("@1.y" None) None) Return(Constant(ConstInt(1) None)) ".case@1"))
      S(CaseStmt(SizeOfExp(Binary(Add Var("@1.y" None) Var("@1.y" None) None) None) Return(Constant(ConstInt(2) None)) ".case@2"))
      S(DefaultStmt(Return(Constant(ConstInt(0) None)) ".default@3"))
    ])) ".switch@0" [switch_case(SizeOfExp(Var("@1.y" None) None) ".case@1") switch_case(SizeOfExp(Binary(Add Var("@1.y" None) Var("@1.y" None) None) None) ".case@2")] ".default@3" None))
  ]) FunType([Int] Int) None None [""] false None [] None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "duplicate case value 4 in switch statement (C99 \u00a76.8.4.2.3)"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  StructDecl(struct_decl("s" false [member_decl("a" Array(UChar 3))]))
  FunctionDecl(function_decl("f" ["@0.p"] Block([
    S(Expression(Assignment(Subscript(Arrow(Var("@0.p" None) "a" None) Constant(ConstInt(1) None) None) Constant(ConstInt(5) None) None)))
  ]) FunType([Pointer(Const(Structure("s")))] Void) None None [""] false None [] None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "cannot assign to const-qualified lvalue: Subscript(array=AddressOf(exp=Arrow(operand=Var(name='@0.p', data_type=Pointer(referenced_type=Const(referenced_type=Structure(tag='s')))), member='a', data_type=Array(element_type=Const(referenced_type=UChar()), size=3)), data_type=Pointer(referenced_type=Const(referenced_type=UChar()))), index=Constant(const=ConstInt(value=1), data_type=Int()), data_type=Const(referenced_type=UChar())) has type Const(referenced_type=UChar())"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  StructDecl(struct_decl("S" false [member_decl("x" Int) member_decl("y" Int)]))
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("@0.s" InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) Structure("S") None None None None false)))
    D(VarDecl(var_decl("@1.p" AddressOf(Var("@0.s" None) None) Pointer(Const(Structure("S"))) None None None None false)))
    S(Expression(Assignment(Arrow(Var("@1.p" None) "y" None) Constant(ConstInt(99) None) None)))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "cannot assign to const-qualified lvalue: Arrow(operand=Var(name='@1.p', data_type=Pointer(referenced_type=Const(referenced_type=Structure(tag='S')))), member='y', data_type=Const(referenced_type=Int())) has type Const(referenced_type=Int())"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("@0.arr" InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None) Constant(ConstInt(3) None)] None None) Array(Const(Int) 3) None None None None false)))
    S(Expression(Assignment(Subscript(Var("@0.arr" None) Constant(ConstInt(0) None) None) Constant(ConstInt(99) None) None)))
    S(Return(Subscript(Var("@0.arr" None) Constant(ConstInt(0) None) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "cannot assign to const-qualified lvalue: Subscript(array=AddressOf(exp=Var(name='@0.arr', data_type=Array(element_type=Const(referenced_type=Int()), size=3)), data_type=Pointer(referenced_type=Const(referenced_type=Int()))), index=Constant(const=ConstInt(value=0), data_type=Int()), data_type=Const(referenced_type=Int())) has type Const(referenced_type=Int())"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  VarDecl(var_decl("v" SizeOfType(UInt None) Long Static None None None false))
])
)~~"}),
     false,
     text({R"~~({"tuple": [{"asdl": "c99_ast", "dump": "Program([\n  VarDecl(var_decl(\"v\" Cast(Long SizeOfType(UInt UInt) Long) Long Static None None None false))\n])\n"}, {"object": "passes.type_checking.SymbolTable", "fields": {"_table": {"dict": [["v", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Long\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Initial", "fields": {"value": 2}}, "is_global": false, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}]]}}}, {"object": "passes.type_checking.TypeTable", "fields": {"_table": {"dict": []}}}]})~~"}),
     text({R"~~({"input": {"asdl": "c99_ast", "dump": "Program([\n  VarDecl(var_decl(\"v\" Cast(Long SizeOfType(UInt UInt) Long) Long Static None None None false))\n])\n"}, "args": {}, "kwargs": {}})~~"})},
    {text({R"~~(Program([
  VarDecl(var_decl("n" Constant(ConstNullptr None) NullptrT None None None None false))
])
)~~"}),
     false,
     text({R"~~({"tuple": [{"asdl": "c99_ast", "dump": "Program([\n  VarDecl(var_decl(\"n\" Constant(ConstNullptr NullptrT) NullptrT None None None None false))\n])\n"}, {"object": "passes.type_checking.SymbolTable", "fields": {"_table": {"dict": [["n", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "NullptrT\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Initial", "fields": {"value": 0}}, "is_global": true, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}]]}}}, {"object": "passes.type_checking.TypeTable", "fields": {"_table": {"dict": []}}}]})~~"}),
     text({R"~~({"input": {"asdl": "c99_ast", "dump": "Program([\n  VarDecl(var_decl(\"n\" Constant(ConstNullptr NullptrT) NullptrT None None None None false))\n])\n"}, "args": {}, "kwargs": {}})~~"})},
    {text({R"~~(Program([
  VarDecl(var_decl("v" SizeOfType(UChar None) Long Static None None None false))
])
)~~"}),
     false,
     text({R"~~({"tuple": [{"asdl": "c99_ast", "dump": "Program([\n  VarDecl(var_decl(\"v\" Cast(Long SizeOfType(UChar UInt) Long) Long Static None None None false))\n])\n"}, {"object": "passes.type_checking.SymbolTable", "fields": {"_table": {"dict": [["v", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Long\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Initial", "fields": {"value": 1}}, "is_global": false, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}]]}}}, {"object": "passes.type_checking.TypeTable", "fields": {"_table": {"dict": []}}}]})~~"}),
     text({R"~~({"input": {"asdl": "c99_ast", "dump": "Program([\n  VarDecl(var_decl(\"v\" Cast(Long SizeOfType(UChar UInt) Long) Long Static None None None false))\n])\n"}, "args": {}, "kwargs": {}})~~"})},
    {text({R"~~(Program([
  VarDecl(var_decl("s" String("\xff") Array(Const(Char) 0) None None None None true))
])
)~~"}),
     false,
     text({R"~~({"tuple": [{"asdl": "c99_ast", "dump": "Program([\n  VarDecl(var_decl(\"s\" String(\"\\xff\") Array(Const(Char) 2) Static None None None true))\n])\n"}, {"object": "passes.type_checking.SymbolTable", "fields": {"_table": {"dict": [["s", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Array(Const(Char) 2)\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Initial", "fields": {"value": {"tuple": [255, 0]}}}, "is_global": false, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}]]}}}, {"object": "passes.type_checking.TypeTable", "fields": {"_table": {"dict": []}}}]})~~"}),
     text({R"~~({"input": {"asdl": "c99_ast", "dump": "Program([\n  VarDecl(var_decl(\"s\" String(\"\\xff\") Array(Const(Char) 2) Static None None None true))\n])\n"}, "args": {}, "kwargs": {}})~~"})},
    {text({R"~~(Program([
  VarDecl(var_decl("s" SizeOfType(BitInt(7 false) None) ULong None None None None false))
])
)~~"}),
     false,
     text({R"~~({"tuple": [{"asdl": "c99_ast", "dump": "Program([\n  VarDecl(var_decl(\"s\" Cast(ULong SizeOfType(BitInt(7 false) UInt) ULong) ULong None None None None false))\n])\n"}, {"object": "passes.type_checking.SymbolTable", "fields": {"_table": {"dict": [["s", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "ULong\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Initial", "fields": {"value": 1}}, "is_global": true, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}]]}}}, {"object": "passes.type_checking.TypeTable", "fields": {"_table": {"dict": []}}}]})~~"}),
     text({R"~~({"input": {"asdl": "c99_ast", "dump": "Program([\n  VarDecl(var_decl(\"s\" Cast(ULong SizeOfType(BitInt(7 false) UInt) ULong) ULong None None None None false))\n])\n"}, "args": {}, "kwargs": {}})~~"})},
    {text({R"~~(Program([
  VarDecl(var_decl("b" Constant(ConstInt(1) None) Const(Bool) None None None None true))
])
)~~"}),
     false,
     text({R"~~({"tuple": [{"asdl": "c99_ast", "dump": "Program([\n  VarDecl(var_decl(\"b\" Cast(Const(Bool) Constant(ConstInt(1) Int) Const(Bool)) Const(Bool) Static None None None true))\n])\n"}, {"object": "passes.type_checking.SymbolTable", "fields": {"_table": {"dict": [["b", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Const(Bool)\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Initial", "fields": {"value": 1}}, "is_global": false, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}]]}}}, {"object": "passes.type_checking.TypeTable", "fields": {"_table": {"dict": []}}}]})~~"}),
     text({R"~~({"input": {"asdl": "c99_ast", "dump": "Program([\n  VarDecl(var_decl(\"b\" Cast(Const(Bool) Constant(ConstInt(1) Int) Const(Bool)) Const(Bool) Static None None None true))\n])\n"}, "args": {}, "kwargs": {}})~~"})},
    {text({R"~~(Program([
  VarDecl(var_decl("x" Cast(Int Constant(ConstDouble(4613937818241073152) None) None) Const(Int) None None None None true))
])
)~~"}),
     false,
     text({R"~~({"tuple": [{"asdl": "c99_ast", "dump": "Program([\n  VarDecl(var_decl(\"x\" Cast(Int Constant(ConstDouble(4613937818241073152) Double) Int) Const(Int) Static None None None true))\n])\n"}, {"object": "passes.type_checking.SymbolTable", "fields": {"_table": {"dict": [["x", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Const(Int)\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Initial", "fields": {"value": 3}}, "is_global": false, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}]]}}}, {"object": "passes.type_checking.TypeTable", "fields": {"_table": {"dict": []}}}]})~~"}),
     text({R"~~({"input": {"asdl": "c99_ast", "dump": "Program([\n  VarDecl(var_decl(\"x\" Cast(Int Constant(ConstDouble(4613937818241073152) Double) Int) Const(Int) Static None None None true))\n])\n"}, "args": {}, "kwargs": {}})~~"})},
    {text({R"~~(Program([
  VarDecl(var_decl("p" InitList([] None None) Const(Pointer(Int)) None None None None true))
])
)~~"}),
     false,
     text({R"~~({"tuple": [{"asdl": "c99_ast", "dump": "Program([\n  VarDecl(var_decl(\"p\" Cast(Const(Pointer(Int)) Constant(ConstInt(0) Int) Const(Pointer(Int))) Const(Pointer(Int)) Static None None None true))\n])\n"}, {"object": "passes.type_checking.SymbolTable", "fields": {"_table": {"dict": [["p", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Const(Pointer(Int))\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Initial", "fields": {"value": 0}}, "is_global": false, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}]]}}}, {"object": "passes.type_checking.TypeTable", "fields": {"_table": {"dict": []}}}]})~~"}),
     text({R"~~({"input": {"asdl": "c99_ast", "dump": "Program([\n  VarDecl(var_decl(\"p\" Cast(Const(Pointer(Int)) Constant(ConstInt(0) Int) Const(Pointer(Int))) Const(Pointer(Int)) Static None None None true))\n])\n"}, "args": {}, "kwargs": {}})~~"})},
    {text({R"~~(Program([
  VarDecl(var_decl("x" Unary(Negate Constant(ConstBitInt(3 3 true) None) None) Int None None None None false))
])
)~~"}),
     false,
     text({R"~~({"tuple": [{"asdl": "c99_ast", "dump": "Program([\n  VarDecl(var_decl(\"x\" Cast(Int Unary(Negate Constant(ConstBitInt(3 3 true) BitInt(3 true)) BitInt(3 true)) Int) Int None None None None false))\n])\n"}, {"object": "passes.type_checking.SymbolTable", "fields": {"_table": {"dict": [["x", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Int\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Initial", "fields": {"value": -3}}, "is_global": true, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}]]}}}, {"object": "passes.type_checking.TypeTable", "fields": {"_table": {"dict": []}}}]})~~"}),
     text({R"~~({"input": {"asdl": "c99_ast", "dump": "Program([\n  VarDecl(var_decl(\"x\" Cast(Int Unary(Negate Constant(ConstBitInt(3 3 true) BitInt(3 true)) BitInt(3 true)) Int) Int None None None None false))\n])\n"}, "args": {}, "kwargs": {}})~~"})},
    {text({R"~~(Program([
  VarDecl(var_decl("d" Constant(ConstFloat(1036831949) None) Const(Double) None None None None true))
])
)~~"}),
     false,
     text({R"~~({"tuple": [{"asdl": "c99_ast", "dump": "Program([\n  VarDecl(var_decl(\"d\" Cast(Const(Double) Constant(ConstFloat(1036831949) Float) Const(Double)) Const(Double) Static None None None true))\n])\n"}, {"object": "passes.type_checking.SymbolTable", "fields": {"_table": {"dict": [["d", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Const(Double)\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Initial", "fields": {"value": 4591870180174331904}}, "is_global": false, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}]]}}}, {"object": "passes.type_checking.TypeTable", "fields": {"_table": {"dict": []}}}]})~~"}),
     text({R"~~({"input": {"asdl": "c99_ast", "dump": "Program([\n  VarDecl(var_decl(\"d\" Cast(Const(Double) Constant(ConstFloat(1036831949) Float) Const(Double)) Const(Double) Static None None None true))\n])\n"}, "args": {}, "kwargs": {}})~~"})},
    {text({R"~~(Program([
  VarDecl(var_decl("x" Constant(ConstBitInt(3 3 true) None) BitInt(2 true) None None None None false))
])
)~~"}),
     false,
     text({R"~~({"tuple": [{"asdl": "c99_ast", "dump": "Program([\n  VarDecl(var_decl(\"x\" Cast(BitInt(2 true) Constant(ConstBitInt(3 3 true) BitInt(3 true)) BitInt(2 true)) BitInt(2 true) None None None None false))\n])\n"}, {"object": "passes.type_checking.SymbolTable", "fields": {"_table": {"dict": [["x", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "BitInt(2 true)\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Initial", "fields": {"value": -1}}, "is_global": true, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}]]}}}, {"object": "passes.type_checking.TypeTable", "fields": {"_table": {"dict": []}}}]})~~"}),
     text({R"~~({"input": {"asdl": "c99_ast", "dump": "Program([\n  VarDecl(var_decl(\"x\" Cast(BitInt(2 true) Constant(ConstBitInt(3 3 true) BitInt(3 true)) BitInt(2 true)) BitInt(2 true) None None None None false))\n])\n"}, "args": {}, "kwargs": {}})~~"})},
    {text({R"~~(Program([
  VarDecl(var_decl("v" Binary(Divide Constant(ConstLong(1000000) None) Constant(ConstInt(1000) None) None) Long Static None None None false))
])
)~~"}),
     false,
     text({R"~~({"tuple": [{"asdl": "c99_ast", "dump": "Program([\n  VarDecl(var_decl(\"v\" Binary(Divide Constant(ConstLong(1000000) Long) Cast(Long Constant(ConstInt(1000) Int) Long) Long) Long Static None None None false))\n])\n"}, {"object": "passes.type_checking.SymbolTable", "fields": {"_table": {"dict": [["v", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Long\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Initial", "fields": {"value": 1000}}, "is_global": false, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}]]}}}, {"object": "passes.type_checking.TypeTable", "fields": {"_table": {"dict": []}}}]})~~"}),
     text({R"~~({"input": {"asdl": "c99_ast", "dump": "Program([\n  VarDecl(var_decl(\"v\" Binary(Divide Constant(ConstLong(1000000) Long) Cast(Long Constant(ConstInt(1000) Int) Long) Long) Long Static None None None false))\n])\n"}, "args": {}, "kwargs": {}})~~"})},
    {text({R"~~(Program([
  VarDecl(var_decl("x" Unary(Complement Cast(BitInt(6 false) Constant(ConstInt(0) None) None) None) Long None None None None false))
])
)~~"}),
     false,
     text({R"~~({"tuple": [{"asdl": "c99_ast", "dump": "Program([\n  VarDecl(var_decl(\"x\" Cast(Long Unary(Complement Cast(BitInt(6 false) Constant(ConstInt(0) Int) BitInt(6 false)) BitInt(6 false)) Long) Long None None None None false))\n])\n"}, {"object": "passes.type_checking.SymbolTable", "fields": {"_table": {"dict": [["x", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Long\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Initial", "fields": {"value": 63}}, "is_global": true, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}]]}}}, {"object": "passes.type_checking.TypeTable", "fields": {"_table": {"dict": []}}}]})~~"}),
     text({R"~~({"input": {"asdl": "c99_ast", "dump": "Program([\n  VarDecl(var_decl(\"x\" Cast(Long Unary(Complement Cast(BitInt(6 false) Constant(ConstInt(0) Int) BitInt(6 false)) BitInt(6 false)) Long) Long None None None None false))\n])\n"}, "args": {}, "kwargs": {}})~~"})},
    {text({R"~~(Program([
  VarDecl(var_decl("v" Binary(Add Constant(ConstBool(1) None) Constant(ConstBool(1) None) None) Long Static None None None false))
])
)~~"}),
     false,
     text({R"~~({"tuple": [{"asdl": "c99_ast", "dump": "Program([\n  VarDecl(var_decl(\"v\" Cast(Long Binary(Add Cast(Int Constant(ConstBool(1) Bool) Int) Cast(Int Constant(ConstBool(1) Bool) Int) Int) Long) Long Static None None None false))\n])\n"}, {"object": "passes.type_checking.SymbolTable", "fields": {"_table": {"dict": [["v", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Long\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Initial", "fields": {"value": 2}}, "is_global": false, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}]]}}}, {"object": "passes.type_checking.TypeTable", "fields": {"_table": {"dict": []}}}]})~~"}),
     text({R"~~({"input": {"asdl": "c99_ast", "dump": "Program([\n  VarDecl(var_decl(\"v\" Cast(Long Binary(Add Cast(Int Constant(ConstBool(1) Bool) Int) Cast(Int Constant(ConstBool(1) Bool) Int) Int) Long) Long Static None None None false))\n])\n"}, "args": {}, "kwargs": {}})~~"})},
    {text({R"~~(Program([
  VarDecl(var_decl("v" Binary(GreaterThan Constant(ConstUInt(40000) None) Constant(ConstInt(32767) None) None) Long Static None None None false))
])
)~~"}),
     false,
     text({R"~~({"tuple": [{"asdl": "c99_ast", "dump": "Program([\n  VarDecl(var_decl(\"v\" Cast(Long Binary(GreaterThan Constant(ConstUInt(40000) UInt) Cast(UInt Constant(ConstInt(32767) Int) UInt) Int) Long) Long Static None None None false))\n])\n"}, {"object": "passes.type_checking.SymbolTable", "fields": {"_table": {"dict": [["v", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Long\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Initial", "fields": {"value": 1}}, "is_global": false, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}]]}}}, {"object": "passes.type_checking.TypeTable", "fields": {"_table": {"dict": []}}}]})~~"}),
     text({R"~~({"input": {"asdl": "c99_ast", "dump": "Program([\n  VarDecl(var_decl(\"v\" Cast(Long Binary(GreaterThan Constant(ConstUInt(40000) UInt) Cast(UInt Constant(ConstInt(32767) Int) UInt) Int) Long) Long Static None None None false))\n])\n"}, "args": {}, "kwargs": {}})~~"})},
    {text({R"~~(Program([
  VarDecl(var_decl("F" Binary(Divide Constant(ConstFloat(1065353216) None) Constant(ConstFloat(1077936128) None) None) Const(Float) None None None None true))
])
)~~"}),
     false,
     text({R"~~({"tuple": [{"asdl": "c99_ast", "dump": "Program([\n  VarDecl(var_decl(\"F\" Binary(Divide Constant(ConstFloat(1065353216) Float) Constant(ConstFloat(1077936128) Float) Float) Const(Float) Static None None None true))\n])\n"}, {"object": "passes.type_checking.SymbolTable", "fields": {"_table": {"dict": [["F", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Const(Float)\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Initial", "fields": {"value": 1051372203}}, "is_global": false, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}]]}}}, {"object": "passes.type_checking.TypeTable", "fields": {"_table": {"dict": []}}}]})~~"}),
     text({R"~~({"input": {"asdl": "c99_ast", "dump": "Program([\n  VarDecl(var_decl(\"F\" Binary(Divide Constant(ConstFloat(1065353216) Float) Constant(ConstFloat(1077936128) Float) Float) Const(Float) Static None None None true))\n])\n"}, "args": {}, "kwargs": {}})~~"})},
    {text({R"~~(Program([
  VarDecl(var_decl("v" Binary(LessThan Cast(SChar Constant(ConstInt(65533) None) None) Constant(ConstInt(0) None) None) Long Static None None None false))
])
)~~"}),
     false,
     text({R"~~({"tuple": [{"asdl": "c99_ast", "dump": "Program([\n  VarDecl(var_decl(\"v\" Cast(Long Binary(LessThan Cast(Int Cast(SChar Constant(ConstInt(65533) Int) SChar) Int) Constant(ConstInt(0) Int) Int) Long) Long Static None None None false))\n])\n"}, {"object": "passes.type_checking.SymbolTable", "fields": {"_table": {"dict": [["v", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Long\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Initial", "fields": {"value": 1}}, "is_global": false, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}]]}}}, {"object": "passes.type_checking.TypeTable", "fields": {"_table": {"dict": []}}}]})~~"}),
     text({R"~~({"input": {"asdl": "c99_ast", "dump": "Program([\n  VarDecl(var_decl(\"v\" Cast(Long Binary(LessThan Cast(Int Cast(SChar Constant(ConstInt(65533) Int) SChar) Int) Constant(ConstInt(0) Int) Int) Long) Long Static None None None false))\n])\n"}, "args": {}, "kwargs": {}})~~"})},
    {text({R"~~(Program([
  VarDecl(var_decl("x" Binary(LeftShift Cast(BitInt(9 false) Constant(ConstInt(1) None) None) Constant(ConstInt(8) None) None) Long None None None None false))
])
)~~"}),
     false,
     text({R"~~({"tuple": [{"asdl": "c99_ast", "dump": "Program([\n  VarDecl(var_decl(\"x\" Cast(Long Binary(LeftShift Cast(BitInt(9 false) Constant(ConstInt(1) Int) BitInt(9 false)) Constant(ConstInt(8) Int) BitInt(9 false)) Long) Long None None None None false))\n])\n"}, {"object": "passes.type_checking.SymbolTable", "fields": {"_table": {"dict": [["x", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Long\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Initial", "fields": {"value": 256}}, "is_global": true, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}]]}}}, {"object": "passes.type_checking.TypeTable", "fields": {"_table": {"dict": []}}}]})~~"}),
     text({R"~~({"input": {"asdl": "c99_ast", "dump": "Program([\n  VarDecl(var_decl(\"x\" Cast(Long Binary(LeftShift Cast(BitInt(9 false) Constant(ConstInt(1) Int) BitInt(9 false)) Constant(ConstInt(8) Int) BitInt(9 false)) Long) Long None None None None false))\n])\n"}, "args": {}, "kwargs": {}})~~"})},
    {text({R"~~(Program([
  FunctionDecl(function_decl("f" [] Block([
    S(Return(FunctionCall("__c6502_bin_to_bcd" [Constant(ConstLong(54321) None)] None false)))
  ]) FunType([] ULong) None None [] false None [] None false))
])
)~~"}),
     false,
     text({R"~~({"tuple": [{"asdl": "c99_ast", "dump": "Program([\n  FunctionDecl(function_decl(\"f\" [] Block([\n    S(Return(FunctionCall(\"__c6502_bin_to_bcd\" [Constant(ConstUInt(54321) UInt)] ULong false)))\n  ]) FunType([] ULong) None None [] false None [] None false))\n])\n"}, {"object": "passes.type_checking.SymbolTable", "fields": {"_table": {"dict": [["f", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "FunType([] ULong)\n"}, "attrs": {"object": "passes.type_checking.FunAttr", "fields": {"defined": true, "is_global": true, "carry_return": false, "address": null, "preserves": {"set": []}, "section": null, "param_zeropage": {"tuple": []}, "returns_bool": false, "noreturn": false}}}}]]}}}, {"object": "passes.type_checking.TypeTable", "fields": {"_table": {"dict": []}}}]})~~"}),
     text({R"~~({"input": {"asdl": "c99_ast", "dump": "Program([\n  FunctionDecl(function_decl(\"f\" [] Block([\n    S(Return(FunctionCall(\"__c6502_bin_to_bcd\" [Constant(ConstUInt(54321) UInt)] ULong false)))\n  ]) FunType([] ULong) None None [] false None [] None false))\n])\n"}, "args": {}, "kwargs": {}})~~"})},
    {text({R"~~(Program([
  VarDecl(var_decl("a" InitList([Designation([IndexDesignator(2)] Constant(ConstInt(5) None)) Constant(ConstInt(6) None)] None None) Array(Int 4) None None None None false))
])
)~~"}),
     false,
     text({R"~~({"tuple": [{"asdl": "c99_ast", "dump": "Program([\n  VarDecl(var_decl(\"a\" InitList([Constant(ConstInt(0) Int) Constant(ConstInt(0) Int) Constant(ConstInt(5) Int) Constant(ConstInt(6) Int)] Array(Int 4) None) Array(Int 4) None None None None false))\n])\n"}, {"object": "passes.type_checking.SymbolTable", "fields": {"_table": {"dict": [["a", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Array(Int 4)\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Initial", "fields": {"value": {"tuple": [0, 0, 5, 6]}}}, "is_global": true, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}]]}}}, {"object": "passes.type_checking.TypeTable", "fields": {"_table": {"dict": []}}}]})~~"}),
     text({R"~~({"input": {"asdl": "c99_ast", "dump": "Program([\n  VarDecl(var_decl(\"a\" InitList([Constant(ConstInt(0) Int) Constant(ConstInt(0) Int) Constant(ConstInt(5) Int) Constant(ConstInt(6) Int)] Array(Int 4) None) Array(Int 4) None None None None false))\n])\n"}, "args": {}, "kwargs": {}})~~"})},
    {text({R"~~(Program([
  VarDecl(var_decl("x" Binary(RightShift Cast(BitInt(5 true) Unary(Negate Constant(ConstInt(1) None) None) None) Constant(ConstInt(1) None) None) Long None None None None false))
])
)~~"}),
     false,
     text({R"~~({"tuple": [{"asdl": "c99_ast", "dump": "Program([\n  VarDecl(var_decl(\"x\" Cast(Long Binary(RightShift Cast(BitInt(5 true) Unary(Negate Constant(ConstInt(1) Int) Int) BitInt(5 true)) Constant(ConstInt(1) Int) BitInt(5 true)) Long) Long None None None None false))\n])\n"}, {"object": "passes.type_checking.SymbolTable", "fields": {"_table": {"dict": [["x", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Long\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Initial", "fields": {"value": -1}}, "is_global": true, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}]]}}}, {"object": "passes.type_checking.TypeTable", "fields": {"_table": {"dict": []}}}]})~~"}),
     text({R"~~({"input": {"asdl": "c99_ast", "dump": "Program([\n  VarDecl(var_decl(\"x\" Cast(Long Binary(RightShift Cast(BitInt(5 true) Unary(Negate Constant(ConstInt(1) Int) Int) BitInt(5 true)) Constant(ConstInt(1) Int) BitInt(5 true)) Long) Long None None None None false))\n])\n"}, "args": {}, "kwargs": {}})~~"})},
    {text({R"~~(Program([
  VarDecl(var_decl("v" Binary(Equal Binary(Divide Constant(ConstUInt(40000) None) Constant(ConstInt(2) None) None) Constant(ConstUInt(20000) None) None) Long Static None None None false))
])
)~~"}),
     false,
     text({R"~~({"tuple": [{"asdl": "c99_ast", "dump": "Program([\n  VarDecl(var_decl(\"v\" Cast(Long Binary(Equal Binary(Divide Constant(ConstUInt(40000) UInt) Cast(UInt Constant(ConstInt(2) Int) UInt) UInt) Constant(ConstUInt(20000) UInt) Int) Long) Long Static None None None false))\n])\n"}, {"object": "passes.type_checking.SymbolTable", "fields": {"_table": {"dict": [["v", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Long\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Initial", "fields": {"value": 1}}, "is_global": false, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}]]}}}, {"object": "passes.type_checking.TypeTable", "fields": {"_table": {"dict": []}}}]})~~"}),
     text({R"~~({"input": {"asdl": "c99_ast", "dump": "Program([\n  VarDecl(var_decl(\"v\" Cast(Long Binary(Equal Binary(Divide Constant(ConstUInt(40000) UInt) Cast(UInt Constant(ConstInt(2) Int) UInt) UInt) Constant(ConstUInt(20000) UInt) Int) Long) Long Static None None None false))\n])\n"}, "args": {}, "kwargs": {}})~~"})},
    {text({R"~~(Program([
  VarDecl(var_decl("u" InitList([Constant(ConstInt(255) None) Constant(ConstInt(0) None)] None None) Array(Const(UChar) 0) None None None None true))
])
)~~"}),
     false,
     text({R"~~({"tuple": [{"asdl": "c99_ast", "dump": "Program([\n  VarDecl(var_decl(\"u\" InitList([Cast(Const(UChar) Constant(ConstInt(255) Int) Const(UChar)) Cast(Const(UChar) Constant(ConstInt(0) Int) Const(UChar))] Array(Const(UChar) 2) None) Array(Const(UChar) 2) Static None None None true))\n])\n"}, {"object": "passes.type_checking.SymbolTable", "fields": {"_table": {"dict": [["u", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Array(Const(UChar) 2)\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Initial", "fields": {"value": {"tuple": [255, 0]}}}, "is_global": false, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}]]}}}, {"object": "passes.type_checking.TypeTable", "fields": {"_table": {"dict": []}}}]})~~"}),
     text({R"~~({"input": {"asdl": "c99_ast", "dump": "Program([\n  VarDecl(var_decl(\"u\" InitList([Cast(Const(UChar) Constant(ConstInt(255) Int) Const(UChar)) Cast(Const(UChar) Constant(ConstInt(0) Int) Const(UChar))] Array(Const(UChar) 2) None) Array(Const(UChar) 2) Static None None None true))\n])\n"}, "args": {}, "kwargs": {}})~~"})},
    {text({R"~~(Program([
  FunctionDecl(function_decl("f" [] Block([
    S(Return(FunctionCall("__c6502_bcd_add" [Constant(ConstInt(2457) None) Constant(ConstInt(1) None)] None false)))
  ]) FunType([] UInt) None None [] false None [] None false))
])
)~~"}),
     false,
     text({R"~~({"tuple": [{"asdl": "c99_ast", "dump": "Program([\n  FunctionDecl(function_decl(\"f\" [] Block([\n    S(Return(FunctionCall(\"__c6502_bcd_add\" [Constant(ConstUInt(2457) UInt) Constant(ConstUInt(1) UInt)] UInt false)))\n  ]) FunType([] UInt) None None [] false None [] None false))\n])\n"}, {"object": "passes.type_checking.SymbolTable", "fields": {"_table": {"dict": [["f", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "FunType([] UInt)\n"}, "attrs": {"object": "passes.type_checking.FunAttr", "fields": {"defined": true, "is_global": true, "carry_return": false, "address": null, "preserves": {"set": []}, "section": null, "param_zeropage": {"tuple": []}, "returns_bool": false, "noreturn": false}}}}]]}}}, {"object": "passes.type_checking.TypeTable", "fields": {"_table": {"dict": []}}}]})~~"}),
     text({R"~~({"input": {"asdl": "c99_ast", "dump": "Program([\n  FunctionDecl(function_decl(\"f\" [] Block([\n    S(Return(FunctionCall(\"__c6502_bcd_add\" [Constant(ConstUInt(2457) UInt) Constant(ConstUInt(1) UInt)] UInt false)))\n  ]) FunType([] UInt) None None [] false None [] None false))\n])\n"}, "args": {}, "kwargs": {}})~~"})},
    {text({R"~~(Program([
  VarDecl(var_decl("Z" Constant(ConstInt(0) None) Const(Int) None None None None true))
  VarDecl(var_decl("p" Var("Z" None) Pointer(Int) None None None None false))
])
)~~"}),
     false,
     text({R"~~({"tuple": [{"asdl": "c99_ast", "dump": "Program([\n  VarDecl(var_decl(\"Z\" Constant(ConstInt(0) Int) Const(Int) Static None None None true))\n  VarDecl(var_decl(\"p\" Cast(Pointer(Int) Var(\"Z\" Const(Int)) Pointer(Int)) Pointer(Int) None None None None false))\n])\n"}, {"object": "passes.type_checking.SymbolTable", "fields": {"_table": {"dict": [["Z", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Const(Int)\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Initial", "fields": {"value": 0}}, "is_global": false, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}], ["p", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Pointer(Int)\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Initial", "fields": {"value": 0}}, "is_global": true, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}]]}}}, {"object": "passes.type_checking.TypeTable", "fields": {"_table": {"dict": []}}}]})~~"}),
     text({R"~~({"input": {"asdl": "c99_ast", "dump": "Program([\n  VarDecl(var_decl(\"Z\" Constant(ConstInt(0) Int) Const(Int) Static None None None true))\n  VarDecl(var_decl(\"p\" Cast(Pointer(Int) Var(\"Z\" Const(Int)) Pointer(Int)) Pointer(Int) None None None None false))\n])\n"}, "args": {}, "kwargs": {}})~~"})},
    {text({R"~~(Program([
  VarDecl(var_decl("s" Binary(Multiply CompoundLiteral("@0.compound_literal" Const(Int) Constant(ConstInt(4) None) None None true) Constant(ConstInt(2) None) None) Int Static None None None false))
])
)~~"}),
     false,
     text({R"~~({"tuple": [{"asdl": "c99_ast", "dump": "Program([\n  VarDecl(var_decl(\"s\" Binary(Multiply CompoundLiteral(\"@0.compound_literal\" Const(Int) Constant(ConstInt(4) Int) Const(Int) None true) Constant(ConstInt(2) Int) Int) Int Static None None None false))\n])\n"}, {"object": "passes.type_checking.SymbolTable", "fields": {"_table": {"dict": [["@0.compound_literal", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Const(Int)\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Initial", "fields": {"value": 4}}, "is_global": false, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}], ["s", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Int\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Initial", "fields": {"value": 8}}, "is_global": false, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}]]}}}, {"object": "passes.type_checking.TypeTable", "fields": {"_table": {"dict": []}}}]})~~"}),
     text({R"~~({"input": {"asdl": "c99_ast", "dump": "Program([\n  VarDecl(var_decl(\"s\" Binary(Multiply CompoundLiteral(\"@0.compound_literal\" Const(Int) Constant(ConstInt(4) Int) Const(Int) None true) Constant(ConstInt(2) Int) Int) Int Static None None None false))\n])\n"}, "args": {}, "kwargs": {}})~~"})},
    {text({R"~~(Program([
  VarDecl(var_decl("D" Constant(ConstDouble(4612811918334230528) None) Const(Double) None None None None true))
  VarDecl(var_decl("x" Cast(Int Var("D" None) None) Const(Int) None None None None true))
])
)~~"}),
     false,
     text({R"~~({"tuple": [{"asdl": "c99_ast", "dump": "Program([\n  VarDecl(var_decl(\"D\" Constant(ConstDouble(4612811918334230528) Double) Const(Double) Static None None None true))\n  VarDecl(var_decl(\"x\" Cast(Int Var(\"D\" Const(Double)) Int) Const(Int) Static None None None true))\n])\n"}, {"object": "passes.type_checking.SymbolTable", "fields": {"_table": {"dict": [["D", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Const(Double)\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Initial", "fields": {"value": 4612811918334230528}}, "is_global": false, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}], ["x", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Const(Int)\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Initial", "fields": {"value": 2}}, "is_global": false, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}]]}}}, {"object": "passes.type_checking.TypeTable", "fields": {"_table": {"dict": []}}}]})~~"}),
     text({R"~~({"input": {"asdl": "c99_ast", "dump": "Program([\n  VarDecl(var_decl(\"D\" Constant(ConstDouble(4612811918334230528) Double) Const(Double) Static None None None true))\n  VarDecl(var_decl(\"x\" Cast(Int Var(\"D\" Const(Double)) Int) Const(Int) Static None None None true))\n])\n"}, "args": {}, "kwargs": {}})~~"})},
    {text({R"~~(Program([
  VarDecl(var_decl("c" None UShort None None None None false))
  FunctionDecl(function_decl("main" [] Block([
    S(Expression(Unary(Plus Var("c" None) None)))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"}),
     false,
     text({R"~~({"tuple": [{"asdl": "c99_ast", "dump": "Program([\n  VarDecl(var_decl(\"c\" None UShort None None None None false))\n  FunctionDecl(function_decl(\"main\" [] Block([\n    S(Expression(Unary(Plus Cast(UInt Var(\"c\" UShort) UInt) UInt)))\n    S(Return(Constant(ConstInt(0) Int)))\n  ]) FunType([] Int) None None [] false None [] None false))\n])\n"}, {"object": "passes.type_checking.SymbolTable", "fields": {"_table": {"dict": [["c", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "UShort\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Tentative", "fields": {}}, "is_global": true, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}], ["main", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "FunType([] Int)\n"}, "attrs": {"object": "passes.type_checking.FunAttr", "fields": {"defined": true, "is_global": true, "carry_return": false, "address": null, "preserves": {"set": []}, "section": null, "param_zeropage": {"tuple": []}, "returns_bool": false, "noreturn": false}}}}]]}}}, {"object": "passes.type_checking.TypeTable", "fields": {"_table": {"dict": []}}}]})~~"}),
     text({R"~~({"input": {"asdl": "c99_ast", "dump": "Program([\n  VarDecl(var_decl(\"c\" None UShort None None None None false))\n  FunctionDecl(function_decl(\"main\" [] Block([\n    S(Expression(Unary(Plus Cast(UInt Var(\"c\" UShort) UInt) UInt)))\n    S(Return(Constant(ConstInt(0) Int)))\n  ]) FunType([] Int) None None [] false None [] None false))\n])\n"}, "args": {}, "kwargs": {}})~~"})},
    {text({R"~~(Program([
  VarDecl(var_decl("a" None UChar None None None None false))
  FunctionDecl(function_decl("f" [] Block([
    S(Expression(Cast(Void FunctionCall("__c6502_bcd_add" [Var("a" None) Constant(ConstInt(1) None)] None false) None)))
  ]) FunType([] Void) None None [] false None [] None false))
])
)~~"}),
     false,
     text({R"~~({"tuple": [{"asdl": "c99_ast", "dump": "Program([\n  VarDecl(var_decl(\"a\" None UChar None None None None false))\n  FunctionDecl(function_decl(\"f\" [] Block([\n    S(Expression(Cast(Void FunctionCall(\"__c6502_bcd_add\" [Var(\"a\" UChar) Constant(ConstUChar(1) UChar)] UChar false) Void)))\n  ]) FunType([] Void) None None [] false None [] None false))\n])\n"}, {"object": "passes.type_checking.SymbolTable", "fields": {"_table": {"dict": [["a", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "UChar\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Tentative", "fields": {}}, "is_global": true, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}], ["f", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "FunType([] Void)\n"}, "attrs": {"object": "passes.type_checking.FunAttr", "fields": {"defined": true, "is_global": true, "carry_return": false, "address": null, "preserves": {"set": []}, "section": null, "param_zeropage": {"tuple": []}, "returns_bool": false, "noreturn": false}}}}]]}}}, {"object": "passes.type_checking.TypeTable", "fields": {"_table": {"dict": []}}}]})~~"}),
     text({R"~~({"input": {"asdl": "c99_ast", "dump": "Program([\n  VarDecl(var_decl(\"a\" None UChar None None None None false))\n  FunctionDecl(function_decl(\"f\" [] Block([\n    S(Expression(Cast(Void FunctionCall(\"__c6502_bcd_add\" [Var(\"a\" UChar) Constant(ConstUChar(1) UChar)] UChar false) Void)))\n  ]) FunType([] Void) None None [] false None [] None false))\n])\n"}, "args": {}, "kwargs": {}})~~"})},
    {text({R"~~(Program([
  FunctionDecl(function_decl("t" [] Block([
    S(Return(Constant(ConstBool(1) None)))
  ]) FunType([] Bool) Static None [] true None [] None false))
  FunctionDecl(function_decl("f" [] Block([
    S(Return(FunctionCall("t" [] None false)))
  ]) FunType([] Bool) None None [] false None [] None false))
])
)~~"}),
     false,
     text({R"~~({"tuple": [{"asdl": "c99_ast", "dump": "Program([\n  FunctionDecl(function_decl(\"t\" [] Block([\n    S(Return(Constant(ConstBool(1) Bool)))\n  ]) FunType([] Bool) Static None [] true None [] None false))\n  FunctionDecl(function_decl(\"f\" [] Block([\n    S(Return(FunctionCall(\"t\" [] Bool false)))\n  ]) FunType([] Bool) None None [] false None [] None false))\n])\n"}, {"object": "passes.type_checking.SymbolTable", "fields": {"_table": {"dict": [["t", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "FunType([] Bool)\n"}, "attrs": {"object": "passes.type_checking.FunAttr", "fields": {"defined": true, "is_global": false, "carry_return": true, "address": null, "preserves": {"set": []}, "section": null, "param_zeropage": {"tuple": []}, "returns_bool": false, "noreturn": false}}}}], ["f", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "FunType([] Bool)\n"}, "attrs": {"object": "passes.type_checking.FunAttr", "fields": {"defined": true, "is_global": true, "carry_return": false, "address": null, "preserves": {"set": []}, "section": null, "param_zeropage": {"tuple": []}, "returns_bool": false, "noreturn": false}}}}]]}}}, {"object": "passes.type_checking.TypeTable", "fields": {"_table": {"dict": []}}}]})~~"}),
     text({R"~~({"input": {"asdl": "c99_ast", "dump": "Program([\n  FunctionDecl(function_decl(\"t\" [] Block([\n    S(Return(Constant(ConstBool(1) Bool)))\n  ]) FunType([] Bool) Static None [] true None [] None false))\n  FunctionDecl(function_decl(\"f\" [] Block([\n    S(Return(FunctionCall(\"t\" [] Bool false)))\n  ]) FunType([] Bool) None None [] false None [] None false))\n])\n"}, "args": {}, "kwargs": {}})~~"})},
    {text({R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("@0.p" None Pointer(Int) None None None None false)))
    S(IfStmt(Binary(Equal Constant(ConstInt(0) None) Var("@0.p" None) None) Return(Constant(ConstInt(1) None)) None))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"}),
     false,
     text({R"~~({"tuple": [{"asdl": "c99_ast", "dump": "Program([\n  FunctionDecl(function_decl(\"main\" [] Block([\n    D(VarDecl(var_decl(\"@0.p\" None Pointer(Int) None None None None false)))\n    S(IfStmt(Binary(Equal Cast(Pointer(Int) Constant(ConstInt(0) Int) Pointer(Int)) Var(\"@0.p\" Pointer(Int)) Int) Return(Constant(ConstInt(1) Int)) None))\n    S(Return(Constant(ConstInt(0) Int)))\n  ]) FunType([] Int) None None [] false None [] None false))\n])\n"}, {"object": "passes.type_checking.SymbolTable", "fields": {"_table": {"dict": [["main", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "FunType([] Int)\n"}, "attrs": {"object": "passes.type_checking.FunAttr", "fields": {"defined": true, "is_global": true, "carry_return": false, "address": null, "preserves": {"set": []}, "section": null, "param_zeropage": {"tuple": []}, "returns_bool": false, "noreturn": false}}}}], ["@0.p", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Pointer(Int)\n"}, "attrs": {"object": "passes.type_checking.LocalAttr", "fields": {"register": false, "was_bool": false}}}}]]}}}, {"object": "passes.type_checking.TypeTable", "fields": {"_table": {"dict": []}}}]})~~"}),
     text({R"~~({"input": {"asdl": "c99_ast", "dump": "Program([\n  FunctionDecl(function_decl(\"main\" [] Block([\n    D(VarDecl(var_decl(\"@0.p\" None Pointer(Int) None None None None false)))\n    S(IfStmt(Binary(Equal Cast(Pointer(Int) Constant(ConstInt(0) Int) Pointer(Int)) Var(\"@0.p\" Pointer(Int)) Int) Return(Constant(ConstInt(1) Int)) None))\n    S(Return(Constant(ConstInt(0) Int)))\n  ]) FunType([] Int) None None [] false None [] None false))\n])\n"}, "args": {}, "kwargs": {}})~~"})},
    {text({R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("@0.p" None Pointer(Int) None None None None false)))
    S(IfStmt(Binary(Equal Var("@0.p" None) Cast(Long Constant(ConstInt(0) None) None) None) Return(Constant(ConstInt(1) None)) None))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"}),
     false,
     text({R"~~({"tuple": [{"asdl": "c99_ast", "dump": "Program([\n  FunctionDecl(function_decl(\"main\" [] Block([\n    D(VarDecl(var_decl(\"@0.p\" None Pointer(Int) None None None None false)))\n    S(IfStmt(Binary(Equal Var(\"@0.p\" Pointer(Int)) Cast(Pointer(Int) Cast(Long Constant(ConstInt(0) Int) Long) Pointer(Int)) Int) Return(Constant(ConstInt(1) Int)) None))\n    S(Return(Constant(ConstInt(0) Int)))\n  ]) FunType([] Int) None None [] false None [] None false))\n])\n"}, {"object": "passes.type_checking.SymbolTable", "fields": {"_table": {"dict": [["main", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "FunType([] Int)\n"}, "attrs": {"object": "passes.type_checking.FunAttr", "fields": {"defined": true, "is_global": true, "carry_return": false, "address": null, "preserves": {"set": []}, "section": null, "param_zeropage": {"tuple": []}, "returns_bool": false, "noreturn": false}}}}], ["@0.p", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Pointer(Int)\n"}, "attrs": {"object": "passes.type_checking.LocalAttr", "fields": {"register": false, "was_bool": false}}}}]]}}}, {"object": "passes.type_checking.TypeTable", "fields": {"_table": {"dict": []}}}]})~~"}),
     text({R"~~({"input": {"asdl": "c99_ast", "dump": "Program([\n  FunctionDecl(function_decl(\"main\" [] Block([\n    D(VarDecl(var_decl(\"@0.p\" None Pointer(Int) None None None None false)))\n    S(IfStmt(Binary(Equal Var(\"@0.p\" Pointer(Int)) Cast(Pointer(Int) Cast(Long Constant(ConstInt(0) Int) Long) Pointer(Int)) Int) Return(Constant(ConstInt(1) Int)) None))\n    S(Return(Constant(ConstInt(0) Int)))\n  ]) FunType([] Int) None None [] false None [] None false))\n])\n"}, "args": {}, "kwargs": {}})~~"})},
    {text({R"~~(Program([
  VarDecl(var_decl("D" Unary(Negate Constant(ConstDouble(4604930618986332160) None) None) Const(Double) Static None None None false))
  FunctionDecl(function_decl("main" [] Block([
    S(Return(Binary(LessThan Var("D" None) Constant(ConstInt(0) None) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"}),
     false,
     text({R"~~({"tuple": [{"asdl": "c99_ast", "dump": "Program([\n  VarDecl(var_decl(\"D\" Unary(Negate Constant(ConstDouble(4604930618986332160) Double) Double) Const(Double) Static None None None false))\n  FunctionDecl(function_decl(\"main\" [] Block([\n    S(Return(Binary(LessThan Var(\"D\" Const(Double)) Cast(Double Constant(ConstInt(0) Int) Double) Int)))\n  ]) FunType([] Int) None None [] false None [] None false))\n])\n"}, {"object": "passes.type_checking.SymbolTable", "fields": {"_table": {"dict": [["D", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Const(Double)\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Initial", "fields": {"value": 13828302655841107968}}, "is_global": false, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}], ["main", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "FunType([] Int)\n"}, "attrs": {"object": "passes.type_checking.FunAttr", "fields": {"defined": true, "is_global": true, "carry_return": false, "address": null, "preserves": {"set": []}, "section": null, "param_zeropage": {"tuple": []}, "returns_bool": false, "noreturn": false}}}}]]}}}, {"object": "passes.type_checking.TypeTable", "fields": {"_table": {"dict": []}}}]})~~"}),
     text({R"~~({"input": {"asdl": "c99_ast", "dump": "Program([\n  VarDecl(var_decl(\"D\" Unary(Negate Constant(ConstDouble(4604930618986332160) Double) Double) Const(Double) Static None None None false))\n  FunctionDecl(function_decl(\"main\" [] Block([\n    S(Return(Binary(LessThan Var(\"D\" Const(Double)) Cast(Double Constant(ConstInt(0) Int) Double) Int)))\n  ]) FunType([] Int) None None [] false None [] None false))\n])\n"}, "args": {}, "kwargs": {}})~~"})},
    {text({R"~~(Program([
  FunctionDecl(function_decl("f" [] Block([
    D(VarDecl(var_decl("@0.x" InitList([Constant(ConstInt(5) None)] None None) Int None None None None false)))
    S(ForStmt(InitDecl([var_decl("@1.i" InitList([Constant(ConstInt(2) None)] None None) Int None None None None false)]) Var("@1.i" None) None BreakStmt("") "" None))
    S(Return(Var("@0.x" None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"}),
     false,
     text({R"~~({"tuple": [{"asdl": "c99_ast", "dump": "Program([\n  FunctionDecl(function_decl(\"f\" [] Block([\n    D(VarDecl(var_decl(\"@0.x\" Constant(ConstInt(5) Int) Int None None None None false)))\n    S(ForStmt(InitDecl([var_decl(\"@1.i\" Constant(ConstInt(2) Int) Int None None None None false)]) Var(\"@1.i\" Int) None BreakStmt(\"\") \"\" None))\n    S(Return(Var(\"@0.x\" Int)))\n  ]) FunType([] Int) None None [] false None [] None false))\n])\n"}, {"object": "passes.type_checking.SymbolTable", "fields": {"_table": {"dict": [["f", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "FunType([] Int)\n"}, "attrs": {"object": "passes.type_checking.FunAttr", "fields": {"defined": true, "is_global": true, "carry_return": false, "address": null, "preserves": {"set": []}, "section": null, "param_zeropage": {"tuple": []}, "returns_bool": false, "noreturn": false}}}}], ["@0.x", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Int\n"}, "attrs": {"object": "passes.type_checking.LocalAttr", "fields": {"register": false, "was_bool": false}}}}], ["@1.i", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Int\n"}, "attrs": {"object": "passes.type_checking.LocalAttr", "fields": {"register": false, "was_bool": false}}}}]]}}}, {"object": "passes.type_checking.TypeTable", "fields": {"_table": {"dict": []}}}]})~~"}),
     text({R"~~({"input": {"asdl": "c99_ast", "dump": "Program([\n  FunctionDecl(function_decl(\"f\" [] Block([\n    D(VarDecl(var_decl(\"@0.x\" Constant(ConstInt(5) Int) Int None None None None false)))\n    S(ForStmt(InitDecl([var_decl(\"@1.i\" Constant(ConstInt(2) Int) Int None None None None false)]) Var(\"@1.i\" Int) None BreakStmt(\"\") \"\" None))\n    S(Return(Var(\"@0.x\" Int)))\n  ]) FunType([] Int) None None [] false None [] None false))\n])\n"}, "args": {}, "kwargs": {}})~~"})},
    {text({R"~~(Program([
  StructDecl(struct_decl("s" false [member_decl("a" Array(UChar 3))]))
  FunctionDecl(function_decl("f" ["@0.p"] Block([
    S(Return(Subscript(Arrow(Var("@0.p" None) "a" None) Constant(ConstInt(1) None) None)))
  ]) FunType([Pointer(Volatile(Structure("s")))] UChar) None None [""] false None [] None false))
])
)~~"}),
     false,
     text({R"~~({"tuple": [{"asdl": "c99_ast", "dump": "Program([\n  StructDecl(struct_decl(\"s\" false [member_decl(\"a\" Array(UChar 3))]))\n  FunctionDecl(function_decl(\"f\" [\"@0.p\"] Block([\n    S(Return(Subscript(AddressOf(Arrow(Var(\"@0.p\" Pointer(Volatile(Structure(\"s\")))) \"a\" Array(Volatile(UChar) 3)) Pointer(Volatile(UChar))) Constant(ConstInt(1) Int) Volatile(UChar))))\n  ]) FunType([Pointer(Volatile(Structure(\"s\")))] UChar) None None [\"\"] false None [] None false))\n])\n"}, {"object": "passes.type_checking.SymbolTable", "fields": {"_table": {"dict": [["f", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "FunType([Pointer(Volatile(Structure(\"s\")))] UChar)\n"}, "attrs": {"object": "passes.type_checking.FunAttr", "fields": {"defined": true, "is_global": true, "carry_return": false, "address": null, "preserves": {"set": []}, "section": null, "param_zeropage": {"tuple": []}, "returns_bool": false, "noreturn": false}}}}], ["@0.p", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Pointer(Volatile(Structure(\"s\")))\n"}, "attrs": {"object": "passes.type_checking.LocalAttr", "fields": {"register": false, "was_bool": false}}}}]]}}}, {"object": "passes.type_checking.TypeTable", "fields": {"_table": {"dict": [["s", {"object": "passes.type_checking.StructLayout", "fields": {"tag": "s", "is_union": false, "members": [{"object": "passes.type_checking.MemberInfo", "fields": {"name": "a", "type": {"asdl": "c99_ast", "dump": "Array(UChar 3)\n"}, "byte_offset": 0}}], "size": 3, "complete": true}}]]}}}]})~~"}),
     text({R"~~({"input": {"asdl": "c99_ast", "dump": "Program([\n  StructDecl(struct_decl(\"s\" false [member_decl(\"a\" Array(UChar 3))]))\n  FunctionDecl(function_decl(\"f\" [\"@0.p\"] Block([\n    S(Return(Subscript(AddressOf(Arrow(Var(\"@0.p\" Pointer(Volatile(Structure(\"s\")))) \"a\" Array(Volatile(UChar) 3)) Pointer(Volatile(UChar))) Constant(ConstInt(1) Int) Volatile(UChar))))\n  ]) FunType([Pointer(Volatile(Structure(\"s\")))] UChar) None None [\"\"] false None [] None false))\n])\n"}, "args": {}, "kwargs": {}})~~"})},
    {text({R"~~(Program([
  FunctionDecl(function_decl("elsewhere" [] None FunType([] Void) None None [] false None [] None false))
  FunctionDecl(function_decl("f" ["@0.p"] Block([
    S(Expression(FunctionCall("elsewhere" [] None false)))
    S(Return(Dereference(Var("@0.p" None) None)))
  ]) FunType([Pointer(Const(UChar))] UChar) None None ["@48"] false None [] None false))
])
)~~"}),
     false,
     text({R"~~({"tuple": [{"asdl": "c99_ast", "dump": "Program([\n  FunctionDecl(function_decl(\"elsewhere\" [] None FunType([] Void) None None [] false None [] None false))\n  FunctionDecl(function_decl(\"f\" [\"@0.p\"] Block([\n    S(Expression(FunctionCall(\"elsewhere\" [] Void false)))\n    S(Return(Dereference(Var(\"@0.p\" Pointer(Const(UChar))) Const(UChar))))\n  ]) FunType([Pointer(Const(UChar))] UChar) None None [\"@48\"] false None [] None false))\n])\n"}, {"object": "passes.type_checking.SymbolTable", "fields": {"_table": {"dict": [["elsewhere", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "FunType([] Void)\n"}, "attrs": {"object": "passes.type_checking.FunAttr", "fields": {"defined": false, "is_global": true, "carry_return": false, "address": null, "preserves": {"set": []}, "section": null, "param_zeropage": {"tuple": []}, "returns_bool": false, "noreturn": false}}}}], ["f", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "FunType([Pointer(Const(UChar))] UChar)\n"}, "attrs": {"object": "passes.type_checking.FunAttr", "fields": {"defined": true, "is_global": true, "carry_return": false, "address": null, "preserves": {"set": []}, "section": null, "param_zeropage": {"tuple": [48]}, "returns_bool": false, "noreturn": false}}}}], ["@0.p", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Pointer(Const(UChar))\n"}, "attrs": {"object": "passes.type_checking.LocalAttr", "fields": {"register": false, "was_bool": false}}}}]]}}}, {"object": "passes.type_checking.TypeTable", "fields": {"_table": {"dict": []}}}]})~~"}),
     text({R"~~({"input": {"asdl": "c99_ast", "dump": "Program([\n  FunctionDecl(function_decl(\"elsewhere\" [] None FunType([] Void) None None [] false None [] None false))\n  FunctionDecl(function_decl(\"f\" [\"@0.p\"] Block([\n    S(Expression(FunctionCall(\"elsewhere\" [] Void false)))\n    S(Return(Dereference(Var(\"@0.p\" Pointer(Const(UChar))) Const(UChar))))\n  ]) FunType([Pointer(Const(UChar))] UChar) None None [\"@48\"] false None [] None false))\n])\n"}, "args": {}, "kwargs": {}})~~"})},
    {text({R"~~(Program([
  FunctionDecl(function_decl("f" ["@0.x"] Block([
    S(Return(Binary(Add Var("@0.x" None) Constant(ConstInt(1) None) None)))
  ]) FunType([Int] Int) None "zp_abi" [""] false None [] None false))
  FunctionDecl(function_decl("main" [] Block([
    S(Return(Cast(Int AddressOf(Var("f" None) None) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"}),
     false,
     text({R"~~({"tuple": [{"asdl": "c99_ast", "dump": "Program([\n  FunctionDecl(function_decl(\"f\" [\"@0.x\"] Block([\n    S(Return(Binary(Add Var(\"@0.x\" Int) Constant(ConstInt(1) Int) Int)))\n  ]) FunType([Int] Int) None \"zp_abi\" [\"\"] false None [] None false))\n  FunctionDecl(function_decl(\"main\" [] Block([\n    S(Return(Cast(Int AddressOf(Var(\"f\" FunType([Int] Int)) Pointer(FunType([Int] Int))) Int)))\n  ]) FunType([] Int) None None [] false None [] None false))\n])\n"}, {"object": "passes.type_checking.SymbolTable", "fields": {"_table": {"dict": [["f", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "FunType([Int] Int)\n"}, "attrs": {"object": "passes.type_checking.FunAttr", "fields": {"defined": true, "is_global": true, "carry_return": false, "address": null, "preserves": {"set": []}, "section": null, "param_zeropage": {"tuple": []}, "returns_bool": false, "noreturn": false}}}}], ["@0.x", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Int\n"}, "attrs": {"object": "passes.type_checking.LocalAttr", "fields": {"register": false, "was_bool": false}}}}], ["main", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "FunType([] Int)\n"}, "attrs": {"object": "passes.type_checking.FunAttr", "fields": {"defined": true, "is_global": true, "carry_return": false, "address": null, "preserves": {"set": []}, "section": null, "param_zeropage": {"tuple": []}, "returns_bool": false, "noreturn": false}}}}]]}}}, {"object": "passes.type_checking.TypeTable", "fields": {"_table": {"dict": []}}}]})~~"}),
     text({R"~~({"input": {"asdl": "c99_ast", "dump": "Program([\n  FunctionDecl(function_decl(\"f\" [\"@0.x\"] Block([\n    S(Return(Binary(Add Var(\"@0.x\" Int) Constant(ConstInt(1) Int) Int)))\n  ]) FunType([Int] Int) None \"zp_abi\" [\"\"] false None [] None false))\n  FunctionDecl(function_decl(\"main\" [] Block([\n    S(Return(Cast(Int AddressOf(Var(\"f\" FunType([Int] Int)) Pointer(FunType([Int] Int))) Int)))\n  ]) FunType([] Int) None None [] false None [] None false))\n])\n"}, "args": {}, "kwargs": {}})~~"})},
    {text({R"~~(Program([
  StructDecl(struct_decl("s" false [member_decl("p" Pointer(Void))]))
  VarDecl(var_decl("A" InitList([Constant(ConstNullptr None)] None None) Const(Structure("s")) None None None None true))
  VarDecl(var_decl("B" Var("A" None) Const(Structure("s")) None None None None true))
])
)~~"}),
     false,
     text({R"~~({"tuple": [{"asdl": "c99_ast", "dump": "Program([\n  StructDecl(struct_decl(\"s\" false [member_decl(\"p\" Pointer(Void))]))\n  VarDecl(var_decl(\"A\" InitList([Cast(Pointer(Void) Constant(ConstNullptr NullptrT) Pointer(Void))] Structure(\"s\") None) Const(Structure(\"s\")) Static None None None true))\n  VarDecl(var_decl(\"B\" Var(\"A\" Const(Structure(\"s\"))) Const(Structure(\"s\")) Static None None None true))\n])\n"}, {"object": "passes.type_checking.SymbolTable", "fields": {"_table": {"dict": [["A", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Const(Structure(\"s\"))\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Initial", "fields": {"value": {"tuple": [0]}}}, "is_global": false, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}], ["B", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Const(Structure(\"s\"))\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Initial", "fields": {"value": {"tuple": [0]}}}, "is_global": false, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}]]}}}, {"object": "passes.type_checking.TypeTable", "fields": {"_table": {"dict": [["s", {"object": "passes.type_checking.StructLayout", "fields": {"tag": "s", "is_union": false, "members": [{"object": "passes.type_checking.MemberInfo", "fields": {"name": "p", "type": {"asdl": "c99_ast", "dump": "Pointer(Void)\n"}, "byte_offset": 0}}], "size": 2, "complete": true}}]]}}}]})~~"}),
     text({R"~~({"input": {"asdl": "c99_ast", "dump": "Program([\n  StructDecl(struct_decl(\"s\" false [member_decl(\"p\" Pointer(Void))]))\n  VarDecl(var_decl(\"A\" InitList([Cast(Pointer(Void) Constant(ConstNullptr NullptrT) Pointer(Void))] Structure(\"s\") None) Const(Structure(\"s\")) Static None None None true))\n  VarDecl(var_decl(\"B\" Var(\"A\" Const(Structure(\"s\"))) Const(Structure(\"s\")) Static None None None true))\n])\n"}, "args": {}, "kwargs": {}})~~"})},
    {text({R"~~(Program([
  FunctionDecl(function_decl("foo" ["@0.x"] Block([
    S(Return(Binary(Add Var("@0.x" None) Constant(ConstInt(1) None) None)))
  ]) FunType([Int] Int) None None [""] false None [] None false))
  FunctionDecl(function_decl("main" [] Block([
    S(Return(IndirectCall(AddressOf(Var("foo" None) None) [Constant(ConstInt(5) None)] None false)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"}),
     false,
     text({R"~~({"tuple": [{"asdl": "c99_ast", "dump": "Program([\n  FunctionDecl(function_decl(\"foo\" [\"@0.x\"] Block([\n    S(Return(Binary(Add Var(\"@0.x\" Int) Constant(ConstInt(1) Int) Int)))\n  ]) FunType([Int] Int) None None [\"\"] false None [] None false))\n  FunctionDecl(function_decl(\"main\" [] Block([\n    S(Return(IndirectCall(Var(\"foo\" FunType([Int] Int)) [Constant(ConstInt(5) Int)] Int false)))\n  ]) FunType([] Int) None None [] false None [] None false))\n])\n"}, {"object": "passes.type_checking.SymbolTable", "fields": {"_table": {"dict": [["foo", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "FunType([Int] Int)\n"}, "attrs": {"object": "passes.type_checking.FunAttr", "fields": {"defined": true, "is_global": true, "carry_return": false, "address": null, "preserves": {"set": []}, "section": null, "param_zeropage": {"tuple": []}, "returns_bool": false, "noreturn": false}}}}], ["@0.x", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Int\n"}, "attrs": {"object": "passes.type_checking.LocalAttr", "fields": {"register": false, "was_bool": false}}}}], ["main", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "FunType([] Int)\n"}, "attrs": {"object": "passes.type_checking.FunAttr", "fields": {"defined": true, "is_global": true, "carry_return": false, "address": null, "preserves": {"set": []}, "section": null, "param_zeropage": {"tuple": []}, "returns_bool": false, "noreturn": false}}}}]]}}}, {"object": "passes.type_checking.TypeTable", "fields": {"_table": {"dict": []}}}]})~~"}),
     text({R"~~({"input": {"asdl": "c99_ast", "dump": "Program([\n  FunctionDecl(function_decl(\"foo\" [\"@0.x\"] Block([\n    S(Return(Binary(Add Var(\"@0.x\" Int) Constant(ConstInt(1) Int) Int)))\n  ]) FunType([Int] Int) None None [\"\"] false None [] None false))\n  FunctionDecl(function_decl(\"main\" [] Block([\n    S(Return(IndirectCall(Var(\"foo\" FunType([Int] Int)) [Constant(ConstInt(5) Int)] Int false)))\n  ]) FunType([] Int) None None [] false None [] None false))\n])\n"}, "args": {}, "kwargs": {}})~~"})},
    {text({R"~~(Program([
  VarDecl(var_decl("m" Binary(Subtract Unary(Negate Constant(ConstLong(2147483647) None) None) Constant(ConstInt(1) None) None) Long None None None None false))
  FunctionDecl(function_decl("main" [] Block([
    S(Return(Binary(Add Binary(LessThan Constant(ConstLong(0) None) Var("m" None) None) Binary(Multiply Binary(LessThan Var("m" None) Constant(ConstLong(5) None) None) Constant(ConstInt(2) None) None) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"}),
     false,
     text({R"~~({"tuple": [{"asdl": "c99_ast", "dump": "Program([\n  VarDecl(var_decl(\"m\" Binary(Subtract Unary(Negate Constant(ConstLong(2147483647) Long) Long) Cast(Long Constant(ConstInt(1) Int) Long) Long) Long None None None None false))\n  FunctionDecl(function_decl(\"main\" [] Block([\n    S(Return(Binary(Add Binary(LessThan Constant(ConstLong(0) Long) Var(\"m\" Long) Int) Binary(Multiply Binary(LessThan Var(\"m\" Long) Constant(ConstLong(5) Long) Int) Constant(ConstInt(2) Int) Int) Int)))\n  ]) FunType([] Int) None None [] false None [] None false))\n])\n"}, {"object": "passes.type_checking.SymbolTable", "fields": {"_table": {"dict": [["m", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Long\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Initial", "fields": {"value": -2147483648}}, "is_global": true, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}], ["main", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "FunType([] Int)\n"}, "attrs": {"object": "passes.type_checking.FunAttr", "fields": {"defined": true, "is_global": true, "carry_return": false, "address": null, "preserves": {"set": []}, "section": null, "param_zeropage": {"tuple": []}, "returns_bool": false, "noreturn": false}}}}]]}}}, {"object": "passes.type_checking.TypeTable", "fields": {"_table": {"dict": []}}}]})~~"}),
     text({R"~~({"input": {"asdl": "c99_ast", "dump": "Program([\n  VarDecl(var_decl(\"m\" Binary(Subtract Unary(Negate Constant(ConstLong(2147483647) Long) Long) Cast(Long Constant(ConstInt(1) Int) Long) Long) Long None None None None false))\n  FunctionDecl(function_decl(\"main\" [] Block([\n    S(Return(Binary(Add Binary(LessThan Constant(ConstLong(0) Long) Var(\"m\" Long) Int) Binary(Multiply Binary(LessThan Var(\"m\" Long) Constant(ConstLong(5) Long) Int) Constant(ConstInt(2) Int) Int) Int)))\n  ]) FunType([] Int) None None [] false None [] None false))\n])\n"}, "args": {}, "kwargs": {}})~~"})},
    {text({R"~~(Program([
  VarDecl(var_decl("a" None ULongLong None None None None false))
  FunctionDecl(function_decl("f" [] Block([
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
  FunctionDecl(function_decl("g" [] Block([
    S(Return(FunctionCall("__c6502_bcd_add" [Var("a" None) Constant(ConstInt(1) None)] None false)))
  ]) FunType([] ULongLong) None None [] false None [] None false))
])
)~~"}),
     false,
     text({R"~~({"tuple": [{"asdl": "c99_ast", "dump": "Program([\n  VarDecl(var_decl(\"a\" None ULongLong None None None None false))\n  FunctionDecl(function_decl(\"f\" [] Block([\n    S(Return(Constant(ConstInt(0) Int)))\n  ]) FunType([] Int) None None [] false None [] None false))\n  FunctionDecl(function_decl(\"g\" [] Block([\n    S(Return(FunctionCall(\"__c6502_bcd_add\" [Var(\"a\" ULongLong) Constant(ConstULongLong(1) ULongLong)] ULongLong false)))\n  ]) FunType([] ULongLong) None None [] false None [] None false))\n])\n"}, {"object": "passes.type_checking.SymbolTable", "fields": {"_table": {"dict": [["a", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "ULongLong\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Tentative", "fields": {}}, "is_global": true, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}], ["f", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "FunType([] Int)\n"}, "attrs": {"object": "passes.type_checking.FunAttr", "fields": {"defined": true, "is_global": true, "carry_return": false, "address": null, "preserves": {"set": []}, "section": null, "param_zeropage": {"tuple": []}, "returns_bool": false, "noreturn": false}}}}], ["g", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "FunType([] ULongLong)\n"}, "attrs": {"object": "passes.type_checking.FunAttr", "fields": {"defined": true, "is_global": true, "carry_return": false, "address": null, "preserves": {"set": []}, "section": null, "param_zeropage": {"tuple": []}, "returns_bool": false, "noreturn": false}}}}]]}}}, {"object": "passes.type_checking.TypeTable", "fields": {"_table": {"dict": []}}}]})~~"}),
     text({R"~~({"input": {"asdl": "c99_ast", "dump": "Program([\n  VarDecl(var_decl(\"a\" None ULongLong None None None None false))\n  FunctionDecl(function_decl(\"f\" [] Block([\n    S(Return(Constant(ConstInt(0) Int)))\n  ]) FunType([] Int) None None [] false None [] None false))\n  FunctionDecl(function_decl(\"g\" [] Block([\n    S(Return(FunctionCall(\"__c6502_bcd_add\" [Var(\"a\" ULongLong) Constant(ConstULongLong(1) ULongLong)] ULongLong false)))\n  ]) FunType([] ULongLong) None None [] false None [] None false))\n])\n"}, "args": {}, "kwargs": {}})~~"})},
    {text({R"~~(Program([
  FunctionDecl(function_decl("main" ["@0.x"] Block([
    S(SwitchStmt(Var("@0.x" None) Compound(Block([
      S(CaseStmt(SizeOfType(Char None) Return(Constant(ConstInt(1) None)) ".case@1"))
      S(CaseStmt(SizeOfType(Long None) Return(Constant(ConstInt(2) None)) ".case@2"))
      S(CaseStmt(SizeOfType(Array(Int 5) None) Return(Constant(ConstInt(5) None)) ".case@3"))
      S(DefaultStmt(Return(Constant(ConstInt(0) None)) ".default@4"))
    ])) ".switch@0" [switch_case(SizeOfType(Char None) ".case@1") switch_case(SizeOfType(Long None) ".case@2") switch_case(SizeOfType(Array(Int 5) None) ".case@3")] ".default@4" None))
  ]) FunType([Int] Int) None None [""] false None [] None false))
])
)~~"}),
     false,
     text({R"~~({"tuple": [{"asdl": "c99_ast", "dump": "Program([\n  FunctionDecl(function_decl(\"main\" [\"@0.x\"] Block([\n    S(SwitchStmt(Var(\"@0.x\" Int) Compound(Block([\n      S(CaseStmt(SizeOfType(Char UInt) Return(Constant(ConstInt(1) Int)) \".case@1\"))\n      S(CaseStmt(SizeOfType(Long UInt) Return(Constant(ConstInt(2) Int)) \".case@2\"))\n      S(CaseStmt(SizeOfType(Array(Int 5) UInt) Return(Constant(ConstInt(5) Int)) \".case@3\"))\n      S(DefaultStmt(Return(Constant(ConstInt(0) Int)) \".default@4\"))\n    ])) \".switch@0\" [switch_case(Constant(ConstInt(1) Int) \".case@1\") switch_case(Constant(ConstInt(4) Int) \".case@2\") switch_case(Constant(ConstInt(10) Int) \".case@3\")] \".default@4\" Int))\n  ]) FunType([Int] Int) None None [\"\"] false None [] None false))\n])\n"}, {"object": "passes.type_checking.SymbolTable", "fields": {"_table": {"dict": [["main", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "FunType([Int] Int)\n"}, "attrs": {"object": "passes.type_checking.FunAttr", "fields": {"defined": true, "is_global": true, "carry_return": false, "address": null, "preserves": {"set": []}, "section": null, "param_zeropage": {"tuple": []}, "returns_bool": false, "noreturn": false}}}}], ["@0.x", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Int\n"}, "attrs": {"object": "passes.type_checking.LocalAttr", "fields": {"register": false, "was_bool": false}}}}]]}}}, {"object": "passes.type_checking.TypeTable", "fields": {"_table": {"dict": []}}}]})~~"}),
     text({R"~~({"input": {"asdl": "c99_ast", "dump": "Program([\n  FunctionDecl(function_decl(\"main\" [\"@0.x\"] Block([\n    S(SwitchStmt(Var(\"@0.x\" Int) Compound(Block([\n      S(CaseStmt(SizeOfType(Char UInt) Return(Constant(ConstInt(1) Int)) \".case@1\"))\n      S(CaseStmt(SizeOfType(Long UInt) Return(Constant(ConstInt(2) Int)) \".case@2\"))\n      S(CaseStmt(SizeOfType(Array(Int 5) UInt) Return(Constant(ConstInt(5) Int)) \".case@3\"))\n      S(DefaultStmt(Return(Constant(ConstInt(0) Int)) \".default@4\"))\n    ])) \".switch@0\" [switch_case(Constant(ConstInt(1) Int) \".case@1\") switch_case(Constant(ConstInt(4) Int) \".case@2\") switch_case(Constant(ConstInt(10) Int) \".case@3\")] \".default@4\" Int))\n  ]) FunType([Int] Int) None None [\"\"] false None [] None false))\n])\n"}, "args": {}, "kwargs": {}})~~"})},
    {text({R"~~(Program([
  VarDecl(var_decl("a" Constant(ConstInt(1104) None) UInt None None None None false))
  VarDecl(var_decl("b" Constant(ConstInt(629) None) UInt None None None None false))
  FunctionDecl(function_decl("main" [] Block([
    S(Return(FunctionCall("__c6502_bcd_sub" [FunctionCall("__c6502_bcd_add" [Var("a" None) Var("b" None)] None false) FunctionCall("__c6502_bcd_add" [Var("b" None) Constant(ConstInt(5) None)] None false)] None false)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"}),
     false,
     text({R"~~({"tuple": [{"asdl": "c99_ast", "dump": "Program([\n  VarDecl(var_decl(\"a\" Cast(UInt Constant(ConstInt(1104) Int) UInt) UInt None None None None false))\n  VarDecl(var_decl(\"b\" Cast(UInt Constant(ConstInt(629) Int) UInt) UInt None None None None false))\n  FunctionDecl(function_decl(\"main\" [] Block([\n    S(Return(Cast(Int FunctionCall(\"__c6502_bcd_sub\" [FunctionCall(\"__c6502_bcd_add\" [Var(\"a\" UInt) Var(\"b\" UInt)] UInt false) FunctionCall(\"__c6502_bcd_add\" [Var(\"b\" UInt) Constant(ConstUInt(5) UInt)] UInt false)] UInt false) Int)))\n  ]) FunType([] Int) None None [] false None [] None false))\n])\n"}, {"object": "passes.type_checking.SymbolTable", "fields": {"_table": {"dict": [["a", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "UInt\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Initial", "fields": {"value": 1104}}, "is_global": true, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}], ["b", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "UInt\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Initial", "fields": {"value": 629}}, "is_global": true, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}], ["main", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "FunType([] Int)\n"}, "attrs": {"object": "passes.type_checking.FunAttr", "fields": {"defined": true, "is_global": true, "carry_return": false, "address": null, "preserves": {"set": []}, "section": null, "param_zeropage": {"tuple": []}, "returns_bool": false, "noreturn": false}}}}]]}}}, {"object": "passes.type_checking.TypeTable", "fields": {"_table": {"dict": []}}}]})~~"}),
     text({R"~~({"input": {"asdl": "c99_ast", "dump": "Program([\n  VarDecl(var_decl(\"a\" Cast(UInt Constant(ConstInt(1104) Int) UInt) UInt None None None None false))\n  VarDecl(var_decl(\"b\" Cast(UInt Constant(ConstInt(629) Int) UInt) UInt None None None None false))\n  FunctionDecl(function_decl(\"main\" [] Block([\n    S(Return(Cast(Int FunctionCall(\"__c6502_bcd_sub\" [FunctionCall(\"__c6502_bcd_add\" [Var(\"a\" UInt) Var(\"b\" UInt)] UInt false) FunctionCall(\"__c6502_bcd_add\" [Var(\"b\" UInt) Constant(ConstUInt(5) UInt)] UInt false)] UInt false) Int)))\n  ]) FunType([] Int) None None [] false None [] None false))\n])\n"}, "args": {}, "kwargs": {}})~~"})},
    {text({R"~~(Program([
  FunctionDecl(function_decl("tick" [] Block([
    D(VarDecl(var_decl("@0.c" Constant(ConstInt(10) None) UChar Static None ZeroPage(None) None false)))
    S(Return(Prefix(Increment Var("@0.c" None) None)))
  ]) FunType([] UChar) None None [] false None [] None false))
  FunctionDecl(function_decl("main" [] Block([
    S(Expression(FunctionCall("tick" [] None false)))
    S(Expression(FunctionCall("tick" [] None false)))
    S(Return(FunctionCall("tick" [] None false)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"}),
     false,
     text({R"~~({"tuple": [{"asdl": "c99_ast", "dump": "Program([\n  FunctionDecl(function_decl(\"tick\" [] Block([\n    D(VarDecl(var_decl(\"@0.c\" Cast(UChar Constant(ConstInt(10) Int) UChar) UChar Static None ZeroPage(None) None false)))\n    S(Return(Prefix(Increment Var(\"@0.c\" UChar) UChar)))\n  ]) FunType([] UChar) None None [] false None [] None false))\n  FunctionDecl(function_decl(\"main\" [] Block([\n    S(Expression(FunctionCall(\"tick\" [] UChar false)))\n    S(Expression(FunctionCall(\"tick\" [] UChar false)))\n    S(Return(Cast(Int FunctionCall(\"tick\" [] UChar false) Int)))\n  ]) FunType([] Int) None None [] false None [] None false))\n])\n"}, {"object": "passes.type_checking.SymbolTable", "fields": {"_table": {"dict": [["tick", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "FunType([] UChar)\n"}, "attrs": {"object": "passes.type_checking.FunAttr", "fields": {"defined": true, "is_global": true, "carry_return": false, "address": null, "preserves": {"set": []}, "section": null, "param_zeropage": {"tuple": []}, "returns_bool": false, "noreturn": false}}}}], ["@0.c", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "UChar\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Initial", "fields": {"value": 10}}, "is_global": false, "zeropage": true, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}], ["main", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "FunType([] Int)\n"}, "attrs": {"object": "passes.type_checking.FunAttr", "fields": {"defined": true, "is_global": true, "carry_return": false, "address": null, "preserves": {"set": []}, "section": null, "param_zeropage": {"tuple": []}, "returns_bool": false, "noreturn": false}}}}]]}}}, {"object": "passes.type_checking.TypeTable", "fields": {"_table": {"dict": []}}}]})~~"}),
     text({R"~~({"input": {"asdl": "c99_ast", "dump": "Program([\n  FunctionDecl(function_decl(\"tick\" [] Block([\n    D(VarDecl(var_decl(\"@0.c\" Cast(UChar Constant(ConstInt(10) Int) UChar) UChar Static None ZeroPage(None) None false)))\n    S(Return(Prefix(Increment Var(\"@0.c\" UChar) UChar)))\n  ]) FunType([] UChar) None None [] false None [] None false))\n  FunctionDecl(function_decl(\"main\" [] Block([\n    S(Expression(FunctionCall(\"tick\" [] UChar false)))\n    S(Expression(FunctionCall(\"tick\" [] UChar false)))\n    S(Return(Cast(Int FunctionCall(\"tick\" [] UChar false) Int)))\n  ]) FunType([] Int) None None [] false None [] None false))\n])\n"}, "args": {}, "kwargs": {}})~~"})},
    {text({R"~~(Program([
  FunctionDecl(function_decl("foo" ["@0.x"] Block([
    S(Return(Binary(Add Var("@0.x" None) Constant(ConstInt(1) None) None)))
  ]) FunType([Int] Int) None None [""] false None [] None false))
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("@1.fp" AddressOf(Var("foo" None) None) Pointer(FunType([Int] Int)) None None None None false)))
    S(Return(IndirectCall(Dereference(Var("@1.fp" None) None) [Constant(ConstInt(5) None)] None false)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"}),
     false,
     text({R"~~({"tuple": [{"asdl": "c99_ast", "dump": "Program([\n  FunctionDecl(function_decl(\"foo\" [\"@0.x\"] Block([\n    S(Return(Binary(Add Var(\"@0.x\" Int) Constant(ConstInt(1) Int) Int)))\n  ]) FunType([Int] Int) None None [\"\"] false None [] None false))\n  FunctionDecl(function_decl(\"main\" [] Block([\n    D(VarDecl(var_decl(\"@1.fp\" AddressOf(Var(\"foo\" FunType([Int] Int)) Pointer(FunType([Int] Int))) Pointer(FunType([Int] Int)) None None None None false)))\n    S(Return(IndirectCall(Var(\"@1.fp\" Pointer(FunType([Int] Int))) [Constant(ConstInt(5) Int)] Int false)))\n  ]) FunType([] Int) None None [] false None [] None false))\n])\n"}, {"object": "passes.type_checking.SymbolTable", "fields": {"_table": {"dict": [["foo", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "FunType([Int] Int)\n"}, "attrs": {"object": "passes.type_checking.FunAttr", "fields": {"defined": true, "is_global": true, "carry_return": false, "address": null, "preserves": {"set": []}, "section": null, "param_zeropage": {"tuple": []}, "returns_bool": false, "noreturn": false}}}}], ["@0.x", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Int\n"}, "attrs": {"object": "passes.type_checking.LocalAttr", "fields": {"register": false, "was_bool": false}}}}], ["main", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "FunType([] Int)\n"}, "attrs": {"object": "passes.type_checking.FunAttr", "fields": {"defined": true, "is_global": true, "carry_return": false, "address": null, "preserves": {"set": []}, "section": null, "param_zeropage": {"tuple": []}, "returns_bool": false, "noreturn": false}}}}], ["@1.fp", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Pointer(FunType([Int] Int))\n"}, "attrs": {"object": "passes.type_checking.LocalAttr", "fields": {"register": false, "was_bool": false}}}}]]}}}, {"object": "passes.type_checking.TypeTable", "fields": {"_table": {"dict": []}}}]})~~"}),
     text({R"~~({"input": {"asdl": "c99_ast", "dump": "Program([\n  FunctionDecl(function_decl(\"foo\" [\"@0.x\"] Block([\n    S(Return(Binary(Add Var(\"@0.x\" Int) Constant(ConstInt(1) Int) Int)))\n  ]) FunType([Int] Int) None None [\"\"] false None [] None false))\n  FunctionDecl(function_decl(\"main\" [] Block([\n    D(VarDecl(var_decl(\"@1.fp\" AddressOf(Var(\"foo\" FunType([Int] Int)) Pointer(FunType([Int] Int))) Pointer(FunType([Int] Int)) None None None None false)))\n    S(Return(IndirectCall(Var(\"@1.fp\" Pointer(FunType([Int] Int))) [Constant(ConstInt(5) Int)] Int false)))\n  ]) FunType([] Int) None None [] false None [] None false))\n])\n"}, "args": {}, "kwargs": {}})~~"})},
    {text({R"~~(Program([
  FunctionDecl(function_decl("f" [] Block([
    D(VarDecl(var_decl("@0.f" InitList([] None None) Float None None None None false)))
    D(VarDecl(var_decl("@1.d" InitList([] None None) Double None None None None false)))
    D(VarDecl(var_decl("@2.p" InitList([] None None) Pointer(Int) None None None None false)))
    D(VarDecl(var_decl("@3.c" InitList([] None None) Char None None None None false)))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"}),
     false,
     text({R"~~({"tuple": [{"asdl": "c99_ast", "dump": "Program([\n  FunctionDecl(function_decl(\"f\" [] Block([\n    D(VarDecl(var_decl(\"@0.f\" Constant(ConstFloat(0) Float) Float None None None None false)))\n    D(VarDecl(var_decl(\"@1.d\" Constant(ConstDouble(0) Double) Double None None None None false)))\n    D(VarDecl(var_decl(\"@2.p\" Cast(Pointer(Int) Constant(ConstInt(0) Int) Pointer(Int)) Pointer(Int) None None None None false)))\n    D(VarDecl(var_decl(\"@3.c\" Cast(Char Constant(ConstInt(0) Int) Char) Char None None None None false)))\n    S(Return(Constant(ConstInt(0) Int)))\n  ]) FunType([] Int) None None [] false None [] None false))\n])\n"}, {"object": "passes.type_checking.SymbolTable", "fields": {"_table": {"dict": [["f", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "FunType([] Int)\n"}, "attrs": {"object": "passes.type_checking.FunAttr", "fields": {"defined": true, "is_global": true, "carry_return": false, "address": null, "preserves": {"set": []}, "section": null, "param_zeropage": {"tuple": []}, "returns_bool": false, "noreturn": false}}}}], ["@0.f", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Float\n"}, "attrs": {"object": "passes.type_checking.LocalAttr", "fields": {"register": false, "was_bool": false}}}}], ["@1.d", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Double\n"}, "attrs": {"object": "passes.type_checking.LocalAttr", "fields": {"register": false, "was_bool": false}}}}], ["@2.p", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Pointer(Int)\n"}, "attrs": {"object": "passes.type_checking.LocalAttr", "fields": {"register": false, "was_bool": false}}}}], ["@3.c", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Char\n"}, "attrs": {"object": "passes.type_checking.LocalAttr", "fields": {"register": false, "was_bool": false}}}}]]}}}, {"object": "passes.type_checking.TypeTable", "fields": {"_table": {"dict": []}}}]})~~"}),
     text({R"~~({"input": {"asdl": "c99_ast", "dump": "Program([\n  FunctionDecl(function_decl(\"f\" [] Block([\n    D(VarDecl(var_decl(\"@0.f\" Constant(ConstFloat(0) Float) Float None None None None false)))\n    D(VarDecl(var_decl(\"@1.d\" Constant(ConstDouble(0) Double) Double None None None None false)))\n    D(VarDecl(var_decl(\"@2.p\" Cast(Pointer(Int) Constant(ConstInt(0) Int) Pointer(Int)) Pointer(Int) None None None None false)))\n    D(VarDecl(var_decl(\"@3.c\" Cast(Char Constant(ConstInt(0) Int) Char) Char None None None None false)))\n    S(Return(Constant(ConstInt(0) Int)))\n  ]) FunType([] Int) None None [] false None [] None false))\n])\n"}, "args": {}, "kwargs": {}})~~"})},
    {text({R"~~(Program([
  FunctionDecl(function_decl("main" ["@0.x"] Block([
    D(VarDecl(var_decl("@1.y" None Long None None None None false)))
    D(VarDecl(var_decl("@2.z" None LongLong None None None None false)))
    S(SwitchStmt(Var("@0.x" None) Compound(Block([
      S(CaseStmt(SizeOfExp(Var("@1.y" None) None) Return(Constant(ConstInt(2) None)) ".case@1"))
      S(CaseStmt(SizeOfExp(Var("@2.z" None) None) Return(Constant(ConstInt(4) None)) ".case@2"))
      S(DefaultStmt(Return(Constant(ConstInt(0) None)) ".default@3"))
    ])) ".switch@0" [switch_case(SizeOfExp(Var("@1.y" None) None) ".case@1") switch_case(SizeOfExp(Var("@2.z" None) None) ".case@2")] ".default@3" None))
  ]) FunType([Int] Int) None None [""] false None [] None false))
])
)~~"}),
     false,
     text({R"~~({"tuple": [{"asdl": "c99_ast", "dump": "Program([\n  FunctionDecl(function_decl(\"main\" [\"@0.x\"] Block([\n    D(VarDecl(var_decl(\"@1.y\" None Long None None None None false)))\n    D(VarDecl(var_decl(\"@2.z\" None LongLong None None None None false)))\n    S(SwitchStmt(Var(\"@0.x\" Int) Compound(Block([\n      S(CaseStmt(SizeOfExp(Var(\"@1.y\" Long) UInt) Return(Constant(ConstInt(2) Int)) \".case@1\"))\n      S(CaseStmt(SizeOfExp(Var(\"@2.z\" LongLong) UInt) Return(Constant(ConstInt(4) Int)) \".case@2\"))\n      S(DefaultStmt(Return(Constant(ConstInt(0) Int)) \".default@3\"))\n    ])) \".switch@0\" [switch_case(Constant(ConstInt(4) Int) \".case@1\") switch_case(Constant(ConstInt(8) Int) \".case@2\")] \".default@3\" Int))\n  ]) FunType([Int] Int) None None [\"\"] false None [] None false))\n])\n"}, {"object": "passes.type_checking.SymbolTable", "fields": {"_table": {"dict": [["main", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "FunType([Int] Int)\n"}, "attrs": {"object": "passes.type_checking.FunAttr", "fields": {"defined": true, "is_global": true, "carry_return": false, "address": null, "preserves": {"set": []}, "section": null, "param_zeropage": {"tuple": []}, "returns_bool": false, "noreturn": false}}}}], ["@0.x", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Int\n"}, "attrs": {"object": "passes.type_checking.LocalAttr", "fields": {"register": false, "was_bool": false}}}}], ["@1.y", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Long\n"}, "attrs": {"object": "passes.type_checking.LocalAttr", "fields": {"register": false, "was_bool": false}}}}], ["@2.z", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "LongLong\n"}, "attrs": {"object": "passes.type_checking.LocalAttr", "fields": {"register": false, "was_bool": false}}}}]]}}}, {"object": "passes.type_checking.TypeTable", "fields": {"_table": {"dict": []}}}]})~~"}),
     text({R"~~({"input": {"asdl": "c99_ast", "dump": "Program([\n  FunctionDecl(function_decl(\"main\" [\"@0.x\"] Block([\n    D(VarDecl(var_decl(\"@1.y\" None Long None None None None false)))\n    D(VarDecl(var_decl(\"@2.z\" None LongLong None None None None false)))\n    S(SwitchStmt(Var(\"@0.x\" Int) Compound(Block([\n      S(CaseStmt(SizeOfExp(Var(\"@1.y\" Long) UInt) Return(Constant(ConstInt(2) Int)) \".case@1\"))\n      S(CaseStmt(SizeOfExp(Var(\"@2.z\" LongLong) UInt) Return(Constant(ConstInt(4) Int)) \".case@2\"))\n      S(DefaultStmt(Return(Constant(ConstInt(0) Int)) \".default@3\"))\n    ])) \".switch@0\" [switch_case(Constant(ConstInt(4) Int) \".case@1\") switch_case(Constant(ConstInt(8) Int) \".case@2\")] \".default@3\" Int))\n  ]) FunType([Int] Int) None None [\"\"] false None [] None false))\n])\n"}, "args": {}, "kwargs": {}})~~"})},
    {text({R"~~(Program([
  VarDecl(var_decl("t" None Array(Int 0) Extern None None None false))
  FunctionDecl(function_decl("first" [] Block([
    S(Return(Subscript(Var("t" None) Constant(ConstInt(0) None) None)))
  ]) FunType([] Int) None None [] false None [] None false))
  VarDecl(var_decl("t" InitList([Constant(ConstInt(7) None) Constant(ConstInt(8) None) Constant(ConstInt(9) None)] None None) Array(Int 0) None None None None false))
  FunctionDecl(function_decl("main" [] Block([
    S(Return(Binary(Add Binary(Add FunctionCall("first" [] None false) Subscript(Var("t" None) Constant(ConstInt(2) None) None) None) SizeOfExp(Var("t" None) None) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"}),
     false,
     text({R"~~({"tuple": [{"asdl": "c99_ast", "dump": "Program([\n  VarDecl(var_decl(\"t\" None Array(Int 0) Extern None None None false))\n  FunctionDecl(function_decl(\"first\" [] Block([\n    S(Return(Subscript(AddressOf(Var(\"t\" Array(Int 0)) Pointer(Int)) Constant(ConstInt(0) Int) Int)))\n  ]) FunType([] Int) None None [] false None [] None false))\n  VarDecl(var_decl(\"t\" InitList([Constant(ConstInt(7) Int) Constant(ConstInt(8) Int) Constant(ConstInt(9) Int)] Array(Int 3) None) Array(Int 3) None None None None false))\n  FunctionDecl(function_decl(\"main\" [] Block([\n    S(Return(Cast(Int Binary(Add Cast(UInt Binary(Add FunctionCall(\"first\" [] Int false) Subscript(AddressOf(Var(\"t\" Array(Int 3)) Pointer(Int)) Constant(ConstInt(2) Int) Int) Int) UInt) SizeOfExp(Var(\"t\" Array(Int 3)) UInt) UInt) Int)))\n  ]) FunType([] Int) None None [] false None [] None false))\n])\n"}, {"object": "passes.type_checking.SymbolTable", "fields": {"_table": {"dict": [["t", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Array(Int 3)\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Initial", "fields": {"value": {"tuple": [7, 8, 9]}}}, "is_global": true, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}], ["first", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "FunType([] Int)\n"}, "attrs": {"object": "passes.type_checking.FunAttr", "fields": {"defined": true, "is_global": true, "carry_return": false, "address": null, "preserves": {"set": []}, "section": null, "param_zeropage": {"tuple": []}, "returns_bool": false, "noreturn": false}}}}], ["main", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "FunType([] Int)\n"}, "attrs": {"object": "passes.type_checking.FunAttr", "fields": {"defined": true, "is_global": true, "carry_return": false, "address": null, "preserves": {"set": []}, "section": null, "param_zeropage": {"tuple": []}, "returns_bool": false, "noreturn": false}}}}]]}}}, {"object": "passes.type_checking.TypeTable", "fields": {"_table": {"dict": []}}}]})~~"}),
     text({R"~~({"input": {"asdl": "c99_ast", "dump": "Program([\n  VarDecl(var_decl(\"t\" None Array(Int 0) Extern None None None false))\n  FunctionDecl(function_decl(\"first\" [] Block([\n    S(Return(Subscript(AddressOf(Var(\"t\" Array(Int 0)) Pointer(Int)) Constant(ConstInt(0) Int) Int)))\n  ]) FunType([] Int) None None [] false None [] None false))\n  VarDecl(var_decl(\"t\" InitList([Constant(ConstInt(7) Int) Constant(ConstInt(8) Int) Constant(ConstInt(9) Int)] Array(Int 3) None) Array(Int 3) None None None None false))\n  FunctionDecl(function_decl(\"main\" [] Block([\n    S(Return(Cast(Int Binary(Add Cast(UInt Binary(Add FunctionCall(\"first\" [] Int false) Subscript(AddressOf(Var(\"t\" Array(Int 3)) Pointer(Int)) Constant(ConstInt(2) Int) Int) Int) UInt) SizeOfExp(Var(\"t\" Array(Int 3)) UInt) UInt) Int)))\n  ]) FunType([] Int) None None [] false None [] None false))\n])\n"}, "args": {}, "kwargs": {}})~~"})},
    {text({R"~~(Program([
  VarDecl(var_decl("buf" None Array(UChar 4) None None None None false))
  FunctionDecl(function_decl("f" ["@0.p"] Block([
    S(Return(Binary(NotEqual Var("@0.p" None) Binary(Add Var("buf" None) Constant(ConstInt(4) None) None) None)))
  ]) FunType([Pointer(Const(UChar))] Int) None None [""] false None [] None false))
  FunctionDecl(function_decl("g" ["@1.p"] Block([
    S(Return(Binary(Equal Var("buf" None) Var("@1.p" None) None)))
  ]) FunType([Pointer(Volatile(UChar))] Int) None None [""] false None [] None false))
])
)~~"}),
     false,
     text({R"~~({"tuple": [{"asdl": "c99_ast", "dump": "Program([\n  VarDecl(var_decl(\"buf\" None Array(UChar 4) None None None None false))\n  FunctionDecl(function_decl(\"f\" [\"@0.p\"] Block([\n    S(Return(Binary(NotEqual Var(\"@0.p\" Pointer(Const(UChar))) Binary(Add AddressOf(Var(\"buf\" Array(UChar 4)) Pointer(UChar)) Constant(ConstInt(4) Int) Pointer(UChar)) Int)))\n  ]) FunType([Pointer(Const(UChar))] Int) None None [\"\"] false None [] None false))\n  FunctionDecl(function_decl(\"g\" [\"@1.p\"] Block([\n    S(Return(Binary(Equal AddressOf(Var(\"buf\" Array(UChar 4)) Pointer(UChar)) Var(\"@1.p\" Pointer(Volatile(UChar))) Int)))\n  ]) FunType([Pointer(Volatile(UChar))] Int) None None [\"\"] false None [] None false))\n])\n"}, {"object": "passes.type_checking.SymbolTable", "fields": {"_table": {"dict": [["buf", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Array(UChar 4)\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Tentative", "fields": {}}, "is_global": true, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}], ["f", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "FunType([Pointer(Const(UChar))] Int)\n"}, "attrs": {"object": "passes.type_checking.FunAttr", "fields": {"defined": true, "is_global": true, "carry_return": false, "address": null, "preserves": {"set": []}, "section": null, "param_zeropage": {"tuple": []}, "returns_bool": false, "noreturn": false}}}}], ["@0.p", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Pointer(Const(UChar))\n"}, "attrs": {"object": "passes.type_checking.LocalAttr", "fields": {"register": false, "was_bool": false}}}}], ["g", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "FunType([Pointer(Volatile(UChar))] Int)\n"}, "attrs": {"object": "passes.type_checking.FunAttr", "fields": {"defined": true, "is_global": true, "carry_return": false, "address": null, "preserves": {"set": []}, "section": null, "param_zeropage": {"tuple": []}, "returns_bool": false, "noreturn": false}}}}], ["@1.p", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Pointer(Volatile(UChar))\n"}, "attrs": {"object": "passes.type_checking.LocalAttr", "fields": {"register": false, "was_bool": false}}}}]]}}}, {"object": "passes.type_checking.TypeTable", "fields": {"_table": {"dict": []}}}]})~~"}),
     text({R"~~({"input": {"asdl": "c99_ast", "dump": "Program([\n  VarDecl(var_decl(\"buf\" None Array(UChar 4) None None None None false))\n  FunctionDecl(function_decl(\"f\" [\"@0.p\"] Block([\n    S(Return(Binary(NotEqual Var(\"@0.p\" Pointer(Const(UChar))) Binary(Add AddressOf(Var(\"buf\" Array(UChar 4)) Pointer(UChar)) Constant(ConstInt(4) Int) Pointer(UChar)) Int)))\n  ]) FunType([Pointer(Const(UChar))] Int) None None [\"\"] false None [] None false))\n  FunctionDecl(function_decl(\"g\" [\"@1.p\"] Block([\n    S(Return(Binary(Equal AddressOf(Var(\"buf\" Array(UChar 4)) Pointer(UChar)) Var(\"@1.p\" Pointer(Volatile(UChar))) Int)))\n  ]) FunType([Pointer(Volatile(UChar))] Int) None None [\"\"] false None [] None false))\n])\n"}, "args": {}, "kwargs": {}})~~"})},
    {text({R"~~(Program([
  StructDecl(struct_decl("p" false [member_decl("x" Int) member_decl("y" Int)]))
  StructDecl(struct_decl("s" false [member_decl("n" Char) member_decl("pt" Array(Structure("p") 2)) member_decl("name" Array(Char 4))]))
  StructDecl(struct_decl("u" true [member_decl("c" Char) member_decl("l" Long) member_decl("i" Int)]))
  StructDecl(struct_decl("w" false [member_decl("tag" Int) member_decl("v" Union("u"))]))
  VarDecl(var_decl("v" InitList([Constant(ConstInt(5) None)] None "c") Union("u") None None None None false))
])
)~~"}),
     false,
     text({R"~~({"tuple": [{"asdl": "c99_ast", "dump": "Program([\n  StructDecl(struct_decl(\"p\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" Char) member_decl(\"pt\" Array(Structure(\"p\") 2)) member_decl(\"name\" Array(Char 4))]))\n  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long) member_decl(\"i\" Int)]))\n  StructDecl(struct_decl(\"w\" false [member_decl(\"tag\" Int) member_decl(\"v\" Union(\"u\"))]))\n  VarDecl(var_decl(\"v\" InitList([Cast(Char Constant(ConstInt(5) Int) Char)] Union(\"u\") \"c\") Union(\"u\") None None None None false))\n])\n"}, {"object": "passes.type_checking.SymbolTable", "fields": {"_table": {"dict": [["v", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Union(\"u\")\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Initial", "fields": {"value": {"tuple": [5]}}}, "is_global": true, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}]]}}}, {"object": "passes.type_checking.TypeTable", "fields": {"_table": {"dict": [["p", {"object": "passes.type_checking.StructLayout", "fields": {"tag": "p", "is_union": false, "members": [{"object": "passes.type_checking.MemberInfo", "fields": {"name": "x", "type": {"asdl": "c99_ast", "dump": "Int\n"}, "byte_offset": 0}}, {"object": "passes.type_checking.MemberInfo", "fields": {"name": "y", "type": {"asdl": "c99_ast", "dump": "Int\n"}, "byte_offset": 2}}], "size": 4, "complete": true}}], ["s", {"object": "passes.type_checking.StructLayout", "fields": {"tag": "s", "is_union": false, "members": [{"object": "passes.type_checking.MemberInfo", "fields": {"name": "n", "type": {"asdl": "c99_ast", "dump": "Char\n"}, "byte_offset": 0}}, {"object": "passes.type_checking.MemberInfo", "fields": {"name": "pt", "type": {"asdl": "c99_ast", "dump": "Array(Structure(\"p\") 2)\n"}, "byte_offset": 1}}, {"object": "passes.type_checking.MemberInfo", "fields": {"name": "name", "type": {"asdl": "c99_ast", "dump": "Array(Char 4)\n"}, "byte_offset": 9}}], "size": 13, "complete": true}}], ["u", {"object": "passes.type_checking.StructLayout", "fields": {"tag": "u", "is_union": true, "members": [{"object": "passes.type_checking.MemberInfo", "fields": {"name": "c", "type": {"asdl": "c99_ast", "dump": "Char\n"}, "byte_offset": 0}}, {"object": "passes.type_checking.MemberInfo", "fields": {"name": "l", "type": {"asdl": "c99_ast", "dump": "Long\n"}, "byte_offset": 0}}, {"object": "passes.type_checking.MemberInfo", "fields": {"name": "i", "type": {"asdl": "c99_ast", "dump": "Int\n"}, "byte_offset": 0}}], "size": 4, "complete": true}}], ["w", {"object": "passes.type_checking.StructLayout", "fields": {"tag": "w", "is_union": false, "members": [{"object": "passes.type_checking.MemberInfo", "fields": {"name": "tag", "type": {"asdl": "c99_ast", "dump": "Int\n"}, "byte_offset": 0}}, {"object": "passes.type_checking.MemberInfo", "fields": {"name": "v", "type": {"asdl": "c99_ast", "dump": "Union(\"u\")\n"}, "byte_offset": 2}}], "size": 6, "complete": true}}]]}}}]})~~"}),
     text({R"~~({"input": {"asdl": "c99_ast", "dump": "Program([\n  StructDecl(struct_decl(\"p\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" Char) member_decl(\"pt\" Array(Structure(\"p\") 2)) member_decl(\"name\" Array(Char 4))]))\n  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long) member_decl(\"i\" Int)]))\n  StructDecl(struct_decl(\"w\" false [member_decl(\"tag\" Int) member_decl(\"v\" Union(\"u\"))]))\n  VarDecl(var_decl(\"v\" InitList([Cast(Char Constant(ConstInt(5) Int) Char)] Union(\"u\") \"c\") Union(\"u\") None None None None false))\n])\n"}, "args": {}, "kwargs": {}})~~"})},
    {text({R"~~(Program([
  StructDecl(struct_decl("p" false [member_decl("x" Int) member_decl("y" Int)]))
  StructDecl(struct_decl("s" false [member_decl("n" Char) member_decl("pt" Array(Structure("p") 2)) member_decl("name" Array(Char 4))]))
  StructDecl(struct_decl("u" true [member_decl("c" Char) member_decl("l" Long) member_decl("i" Int)]))
  StructDecl(struct_decl("w" false [member_decl("tag" Int) member_decl("v" Union("u"))]))
  VarDecl(var_decl("v" InitList([Constant(ConstInt(5) None)] None "i") Union("u") None None None None false))
])
)~~"}),
     false,
     text({R"~~({"tuple": [{"asdl": "c99_ast", "dump": "Program([\n  StructDecl(struct_decl(\"p\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" Char) member_decl(\"pt\" Array(Structure(\"p\") 2)) member_decl(\"name\" Array(Char 4))]))\n  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long) member_decl(\"i\" Int)]))\n  StructDecl(struct_decl(\"w\" false [member_decl(\"tag\" Int) member_decl(\"v\" Union(\"u\"))]))\n  VarDecl(var_decl(\"v\" InitList([Constant(ConstInt(5) Int)] Union(\"u\") \"i\") Union(\"u\") None None None None false))\n])\n"}, {"object": "passes.type_checking.SymbolTable", "fields": {"_table": {"dict": [["v", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Union(\"u\")\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Initial", "fields": {"value": {"object": "passes.type_checking.UnionValue", "fields": {"member": 2, "value": 5}}}}, "is_global": true, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}]]}}}, {"object": "passes.type_checking.TypeTable", "fields": {"_table": {"dict": [["p", {"object": "passes.type_checking.StructLayout", "fields": {"tag": "p", "is_union": false, "members": [{"object": "passes.type_checking.MemberInfo", "fields": {"name": "x", "type": {"asdl": "c99_ast", "dump": "Int\n"}, "byte_offset": 0}}, {"object": "passes.type_checking.MemberInfo", "fields": {"name": "y", "type": {"asdl": "c99_ast", "dump": "Int\n"}, "byte_offset": 2}}], "size": 4, "complete": true}}], ["s", {"object": "passes.type_checking.StructLayout", "fields": {"tag": "s", "is_union": false, "members": [{"object": "passes.type_checking.MemberInfo", "fields": {"name": "n", "type": {"asdl": "c99_ast", "dump": "Char\n"}, "byte_offset": 0}}, {"object": "passes.type_checking.MemberInfo", "fields": {"name": "pt", "type": {"asdl": "c99_ast", "dump": "Array(Structure(\"p\") 2)\n"}, "byte_offset": 1}}, {"object": "passes.type_checking.MemberInfo", "fields": {"name": "name", "type": {"asdl": "c99_ast", "dump": "Array(Char 4)\n"}, "byte_offset": 9}}], "size": 13, "complete": true}}], ["u", {"object": "passes.type_checking.StructLayout", "fields": {"tag": "u", "is_union": true, "members": [{"object": "passes.type_checking.MemberInfo", "fields": {"name": "c", "type": {"asdl": "c99_ast", "dump": "Char\n"}, "byte_offset": 0}}, {"object": "passes.type_checking.MemberInfo", "fields": {"name": "l", "type": {"asdl": "c99_ast", "dump": "Long\n"}, "byte_offset": 0}}, {"object": "passes.type_checking.MemberInfo", "fields": {"name": "i", "type": {"asdl": "c99_ast", "dump": "Int\n"}, "byte_offset": 0}}], "size": 4, "complete": true}}], ["w", {"object": "passes.type_checking.StructLayout", "fields": {"tag": "w", "is_union": false, "members": [{"object": "passes.type_checking.MemberInfo", "fields": {"name": "tag", "type": {"asdl": "c99_ast", "dump": "Int\n"}, "byte_offset": 0}}, {"object": "passes.type_checking.MemberInfo", "fields": {"name": "v", "type": {"asdl": "c99_ast", "dump": "Union(\"u\")\n"}, "byte_offset": 2}}], "size": 6, "complete": true}}]]}}}]})~~"}),
     text({R"~~({"input": {"asdl": "c99_ast", "dump": "Program([\n  StructDecl(struct_decl(\"p\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" Char) member_decl(\"pt\" Array(Structure(\"p\") 2)) member_decl(\"name\" Array(Char 4))]))\n  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long) member_decl(\"i\" Int)]))\n  StructDecl(struct_decl(\"w\" false [member_decl(\"tag\" Int) member_decl(\"v\" Union(\"u\"))]))\n  VarDecl(var_decl(\"v\" InitList([Constant(ConstInt(5) Int)] Union(\"u\") \"i\") Union(\"u\") None None None None false))\n])\n"}, "args": {}, "kwargs": {}})~~"})},
    {text({R"~~(Program([
  StructDecl(struct_decl("m" false [member_decl("x" UChar) member_decl("m" Array(Int 4))]))
  VarDecl(var_decl("S" None Structure("m") None None None None false))
  VarDecl(var_decl("u" InitList([AddressOf(Subscript(Dot(Var("S" None) "m" None) Constant(ConstInt(1) None) None) None) Binary(Add Dot(Var("S" None) "m" None) Constant(ConstInt(3) None) None) AddressOf(Subscript(Arrow(AddressOf(Var("S" None) None) "m" None) Constant(ConstInt(2) None) None) None) AddressOf(Subscript(Dot(Dereference(AddressOf(Var("S" None) None) None) "m" None) Constant(ConstInt(0) None) None) None)] None None) Array(Const(Pointer(Int)) 0) Static None None None false))
])
)~~"}),
     false,
     text({R"~~({"tuple": [{"asdl": "c99_ast", "dump": "Program([\n  StructDecl(struct_decl(\"m\" false [member_decl(\"x\" UChar) member_decl(\"m\" Array(Int 4))]))\n  VarDecl(var_decl(\"S\" None Structure(\"m\") None None None None false))\n  VarDecl(var_decl(\"u\" InitList([AddressOf(Subscript(AddressOf(Dot(Var(\"S\" Structure(\"m\")) \"m\" Array(Int 4)) Pointer(Int)) Constant(ConstInt(1) Int) Int) Pointer(Int)) Binary(Add AddressOf(Dot(Var(\"S\" Structure(\"m\")) \"m\" Array(Int 4)) Pointer(Int)) Constant(ConstInt(3) Int) Pointer(Int)) AddressOf(Subscript(AddressOf(Arrow(AddressOf(Var(\"S\" Structure(\"m\")) Pointer(Structure(\"m\"))) \"m\" Array(Int 4)) Pointer(Int)) Constant(ConstInt(2) Int) Int) Pointer(Int)) AddressOf(Subscript(AddressOf(Dot(Dereference(AddressOf(Var(\"S\" Structure(\"m\")) Pointer(Structure(\"m\"))) Structure(\"m\")) \"m\" Array(Int 4)) Pointer(Int)) Constant(ConstInt(0) Int) Int) Pointer(Int))] Array(Const(Pointer(Int)) 4) None) Array(Const(Pointer(Int)) 4) Static None None None false))\n])\n"}, {"object": "passes.type_checking.SymbolTable", "fields": {"_table": {"dict": [["S", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Structure(\"m\")\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Tentative", "fields": {}}, "is_global": true, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}], ["u", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Array(Const(Pointer(Int)) 4)\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Initial", "fields": {"value": {"tuple": [{"object": "passes.type_checking.AddressInit", "fields": {"name": "S", "offset": 3}}, {"object": "passes.type_checking.AddressInit", "fields": {"name": "S", "offset": 7}}, {"object": "passes.type_checking.AddressInit", "fields": {"name": "S", "offset": 5}}, {"object": "passes.type_checking.AddressInit", "fields": {"name": "S", "offset": 1}}]}}}, "is_global": false, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}]]}}}, {"object": "passes.type_checking.TypeTable", "fields": {"_table": {"dict": [["m", {"object": "passes.type_checking.StructLayout", "fields": {"tag": "m", "is_union": false, "members": [{"object": "passes.type_checking.MemberInfo", "fields": {"name": "x", "type": {"asdl": "c99_ast", "dump": "UChar\n"}, "byte_offset": 0}}, {"object": "passes.type_checking.MemberInfo", "fields": {"name": "m", "type": {"asdl": "c99_ast", "dump": "Array(Int 4)\n"}, "byte_offset": 1}}], "size": 9, "complete": true}}]]}}}]})~~"}),
     text({R"~~({"input": {"asdl": "c99_ast", "dump": "Program([\n  StructDecl(struct_decl(\"m\" false [member_decl(\"x\" UChar) member_decl(\"m\" Array(Int 4))]))\n  VarDecl(var_decl(\"S\" None Structure(\"m\") None None None None false))\n  VarDecl(var_decl(\"u\" InitList([AddressOf(Subscript(AddressOf(Dot(Var(\"S\" Structure(\"m\")) \"m\" Array(Int 4)) Pointer(Int)) Constant(ConstInt(1) Int) Int) Pointer(Int)) Binary(Add AddressOf(Dot(Var(\"S\" Structure(\"m\")) \"m\" Array(Int 4)) Pointer(Int)) Constant(ConstInt(3) Int) Pointer(Int)) AddressOf(Subscript(AddressOf(Arrow(AddressOf(Var(\"S\" Structure(\"m\")) Pointer(Structure(\"m\"))) \"m\" Array(Int 4)) Pointer(Int)) Constant(ConstInt(2) Int) Int) Pointer(Int)) AddressOf(Subscript(AddressOf(Dot(Dereference(AddressOf(Var(\"S\" Structure(\"m\")) Pointer(Structure(\"m\"))) Structure(\"m\")) \"m\" Array(Int 4)) Pointer(Int)) Constant(ConstInt(0) Int) Int) Pointer(Int))] Array(Const(Pointer(Int)) 4) None) Array(Const(Pointer(Int)) 4) Static None None None false))\n])\n"}, "args": {}, "kwargs": {}})~~"})},
    {text({R"~~(Program([
  VarDecl(var_decl("s" FunctionCall("__c6502_bcd_add" [Constant(ConstInt(2457) None) Constant(ConstInt(1) None)] None false) UInt Static None None None false))
  VarDecl(var_decl("t" FunctionCall("__c6502_bcd_sub" [Constant(ConstInt(4096) None) Constant(ConstInt(1) None)] None false) UInt None None None None false))
  VarDecl(var_decl("a" None Array(Int 16) None None None None false))
  FunctionDecl(function_decl("f" ["@0.k"] Block([
    S(SwitchStmt(Var("@0.k" None) Compound(Block([
      S(CaseStmt(FunctionCall("__c6502_bcd_add" [Constant(ConstInt(25) None) Constant(ConstInt(1) None)] None false) Return(Constant(ConstInt(1) None)) ""))
    ])) "" [] None None))
    S(Return(SizeOfExp(Var("a" None) None)))
  ]) FunType([UChar] Int) None None [""] false None [] None false))
])
)~~"}),
     false,
     text({R"~~({"tuple": [{"asdl": "c99_ast", "dump": "Program([\n  VarDecl(var_decl(\"s\" FunctionCall(\"__c6502_bcd_add\" [Constant(ConstUInt(2457) UInt) Constant(ConstUInt(1) UInt)] UInt false) UInt Static None None None false))\n  VarDecl(var_decl(\"t\" FunctionCall(\"__c6502_bcd_sub\" [Constant(ConstUInt(4096) UInt) Constant(ConstUInt(1) UInt)] UInt false) UInt None None None None false))\n  VarDecl(var_decl(\"a\" None Array(Int 16) None None None None false))\n  FunctionDecl(function_decl(\"f\" [\"@0.k\"] Block([\n    S(SwitchStmt(Cast(Int Var(\"@0.k\" UChar) Int) Compound(Block([\n      S(CaseStmt(FunctionCall(\"__c6502_bcd_add\" [Constant(ConstInt(25) None) Constant(ConstInt(1) None)] None false) Return(Constant(ConstInt(1) Int)) \"\"))\n    ])) \"\" [] None Int))\n    S(Return(Cast(Int SizeOfExp(Var(\"a\" Array(Int 16)) UInt) Int)))\n  ]) FunType([UChar] Int) None None [\"\"] false None [] None false))\n])\n"}, {"object": "passes.type_checking.SymbolTable", "fields": {"_table": {"dict": [["s", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "UInt\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Initial", "fields": {"value": 4096}}, "is_global": false, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}], ["t", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "UInt\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Initial", "fields": {"value": 2457}}, "is_global": true, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}], ["a", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Array(Int 16)\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Tentative", "fields": {}}, "is_global": true, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}], ["f", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "FunType([UChar] Int)\n"}, "attrs": {"object": "passes.type_checking.FunAttr", "fields": {"defined": true, "is_global": true, "carry_return": false, "address": null, "preserves": {"set": []}, "section": null, "param_zeropage": {"tuple": []}, "returns_bool": false, "noreturn": false}}}}], ["@0.k", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "UChar\n"}, "attrs": {"object": "passes.type_checking.LocalAttr", "fields": {"register": false, "was_bool": false}}}}]]}}}, {"object": "passes.type_checking.TypeTable", "fields": {"_table": {"dict": []}}}]})~~"}),
     text({R"~~({"input": {"asdl": "c99_ast", "dump": "Program([\n  VarDecl(var_decl(\"s\" FunctionCall(\"__c6502_bcd_add\" [Constant(ConstUInt(2457) UInt) Constant(ConstUInt(1) UInt)] UInt false) UInt Static None None None false))\n  VarDecl(var_decl(\"t\" FunctionCall(\"__c6502_bcd_sub\" [Constant(ConstUInt(4096) UInt) Constant(ConstUInt(1) UInt)] UInt false) UInt None None None None false))\n  VarDecl(var_decl(\"a\" None Array(Int 16) None None None None false))\n  FunctionDecl(function_decl(\"f\" [\"@0.k\"] Block([\n    S(SwitchStmt(Cast(Int Var(\"@0.k\" UChar) Int) Compound(Block([\n      S(CaseStmt(FunctionCall(\"__c6502_bcd_add\" [Constant(ConstInt(25) None) Constant(ConstInt(1) None)] None false) Return(Constant(ConstInt(1) Int)) \"\"))\n    ])) \"\" [] None Int))\n    S(Return(Cast(Int SizeOfExp(Var(\"a\" Array(Int 16)) UInt) Int)))\n  ]) FunType([UChar] Int) None None [\"\"] false None [] None false))\n])\n"}, "args": {}, "kwargs": {}})~~"})},
    {text({R"~~(Program([
  StructDecl(struct_decl("s" false [member_decl("tag" UChar) member_decl("score" UInt)]))
  VarDecl(var_decl("p" None Structure("s") None None None None false))
  FunctionDecl(function_decl("add" ["@0.x"] Block([
    S(Expression(Assignment(Dot(Var("p" None) "score" None) FunctionCall("__c6502_bcd_add" [Dot(Var("p" None) "score" None) Var("@0.x" None)] None false) None)))
  ]) FunType([UChar] Void) None None [""] false None [] None false))
  FunctionDecl(function_decl("add2" ["@1.x"] Block([
    S(Expression(Assignment(Dot(Var("p" None) "score" None) FunctionCall("__c6502_bcd_add" [Var("@1.x" None) Dot(Var("p" None) "score" None)] None false) None)))
  ]) FunType([UInt] Void) None None [""] false None [] None false))
])
)~~"}),
     false,
     text({R"~~({"tuple": [{"asdl": "c99_ast", "dump": "Program([\n  StructDecl(struct_decl(\"s\" false [member_decl(\"tag\" UChar) member_decl(\"score\" UInt)]))\n  VarDecl(var_decl(\"p\" None Structure(\"s\") None None None None false))\n  FunctionDecl(function_decl(\"add\" [\"@0.x\"] Block([\n    S(Expression(Assignment(Dot(Var(\"p\" Structure(\"s\")) \"score\" UInt) FunctionCall(\"__c6502_bcd_add\" [Dot(Var(\"p\" Structure(\"s\")) \"score\" UInt) Cast(UInt Var(\"@0.x\" UChar) UInt)] UInt false) UInt)))\n  ]) FunType([UChar] Void) None None [\"\"] false None [] None false))\n  FunctionDecl(function_decl(\"add2\" [\"@1.x\"] Block([\n    S(Expression(Assignment(Dot(Var(\"p\" Structure(\"s\")) \"score\" UInt) FunctionCall(\"__c6502_bcd_add\" [Var(\"@1.x\" UInt) Dot(Var(\"p\" Structure(\"s\")) \"score\" UInt)] UInt false) UInt)))\n  ]) FunType([UInt] Void) None None [\"\"] false None [] None false))\n])\n"}, {"object": "passes.type_checking.SymbolTable", "fields": {"_table": {"dict": [["p", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Structure(\"s\")\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Tentative", "fields": {}}, "is_global": true, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}], ["add", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "FunType([UChar] Void)\n"}, "attrs": {"object": "passes.type_checking.FunAttr", "fields": {"defined": true, "is_global": true, "carry_return": false, "address": null, "preserves": {"set": []}, "section": null, "param_zeropage": {"tuple": []}, "returns_bool": false, "noreturn": false}}}}], ["@0.x", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "UChar\n"}, "attrs": {"object": "passes.type_checking.LocalAttr", "fields": {"register": false, "was_bool": false}}}}], ["add2", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "FunType([UInt] Void)\n"}, "attrs": {"object": "passes.type_checking.FunAttr", "fields": {"defined": true, "is_global": true, "carry_return": false, "address": null, "preserves": {"set": []}, "section": null, "param_zeropage": {"tuple": []}, "returns_bool": false, "noreturn": false}}}}], ["@1.x", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "UInt\n"}, "attrs": {"object": "passes.type_checking.LocalAttr", "fields": {"register": false, "was_bool": false}}}}]]}}}, {"object": "passes.type_checking.TypeTable", "fields": {"_table": {"dict": [["s", {"object": "passes.type_checking.StructLayout", "fields": {"tag": "s", "is_union": false, "members": [{"object": "passes.type_checking.MemberInfo", "fields": {"name": "tag", "type": {"asdl": "c99_ast", "dump": "UChar\n"}, "byte_offset": 0}}, {"object": "passes.type_checking.MemberInfo", "fields": {"name": "score", "type": {"asdl": "c99_ast", "dump": "UInt\n"}, "byte_offset": 1}}], "size": 3, "complete": true}}]]}}}]})~~"}),
     text({R"~~({"input": {"asdl": "c99_ast", "dump": "Program([\n  StructDecl(struct_decl(\"s\" false [member_decl(\"tag\" UChar) member_decl(\"score\" UInt)]))\n  VarDecl(var_decl(\"p\" None Structure(\"s\") None None None None false))\n  FunctionDecl(function_decl(\"add\" [\"@0.x\"] Block([\n    S(Expression(Assignment(Dot(Var(\"p\" Structure(\"s\")) \"score\" UInt) FunctionCall(\"__c6502_bcd_add\" [Dot(Var(\"p\" Structure(\"s\")) \"score\" UInt) Cast(UInt Var(\"@0.x\" UChar) UInt)] UInt false) UInt)))\n  ]) FunType([UChar] Void) None None [\"\"] false None [] None false))\n  FunctionDecl(function_decl(\"add2\" [\"@1.x\"] Block([\n    S(Expression(Assignment(Dot(Var(\"p\" Structure(\"s\")) \"score\" UInt) FunctionCall(\"__c6502_bcd_add\" [Var(\"@1.x\" UInt) Dot(Var(\"p\" Structure(\"s\")) \"score\" UInt)] UInt false) UInt)))\n  ]) FunType([UInt] Void) None None [\"\"] false None [] None false))\n])\n"}, "args": {}, "kwargs": {}})~~"})},
    {text({R"~~(Program([
  VarDecl(var_decl("s" FunctionCall("__c6502_bin_to_bcd" [Constant(ConstInt(12345) None)] None false) ULong Static None None None false))
  VarDecl(var_decl("t" FunctionCall("__c6502_bin_to_bcd" [Unary(Negate Constant(ConstInt(1) None) None)] None false) ULong None None None None false))
  VarDecl(var_decl("u" FunctionCall("__c6502_bin_to_bcd" [Constant(ConstLong(70000) None)] None false) ULong None None None None false))
  VarDecl(var_decl("a" None Array(Int 16) None None None None false))
  FunctionDecl(function_decl("f" ["@0.k"] Block([
    S(SwitchStmt(Var("@0.k" None) Compound(Block([
      S(CaseStmt(FunctionCall("__c6502_bin_to_bcd" [Constant(ConstInt(42) None)] None false) Return(Constant(ConstInt(1) None)) ""))
    ])) "" [] None None))
    S(Return(SizeOfExp(Var("a" None) None)))
  ]) FunType([UChar] Int) None None [""] false None [] None false))
])
)~~"}),
     false,
     text({R"~~({"tuple": [{"asdl": "c99_ast", "dump": "Program([\n  VarDecl(var_decl(\"s\" FunctionCall(\"__c6502_bin_to_bcd\" [Constant(ConstUInt(12345) UInt)] ULong false) ULong Static None None None false))\n  VarDecl(var_decl(\"t\" FunctionCall(\"__c6502_bin_to_bcd\" [Constant(ConstUInt(65535) UInt)] ULong false) ULong None None None None false))\n  VarDecl(var_decl(\"u\" FunctionCall(\"__c6502_bin_to_bcd\" [Constant(ConstUInt(4464) UInt)] ULong false) ULong None None None None false))\n  VarDecl(var_decl(\"a\" None Array(Int 16) None None None None false))\n  FunctionDecl(function_decl(\"f\" [\"@0.k\"] Block([\n    S(SwitchStmt(Cast(Int Var(\"@0.k\" UChar) Int) Compound(Block([\n      S(CaseStmt(FunctionCall(\"__c6502_bin_to_bcd\" [Constant(ConstInt(42) None)] None false) Return(Constant(ConstInt(1) Int)) \"\"))\n    ])) \"\" [] None Int))\n    S(Return(Cast(Int SizeOfExp(Var(\"a\" Array(Int 16)) UInt) Int)))\n  ]) FunType([UChar] Int) None None [\"\"] false None [] None false))\n])\n"}, {"object": "passes.type_checking.SymbolTable", "fields": {"_table": {"dict": [["s", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "ULong\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Initial", "fields": {"value": 74565}}, "is_global": false, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}], ["t", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "ULong\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Initial", "fields": {"value": 415029}}, "is_global": true, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}], ["u", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "ULong\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Initial", "fields": {"value": 17508}}, "is_global": true, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}], ["a", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Array(Int 16)\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Tentative", "fields": {}}, "is_global": true, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}], ["f", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "FunType([UChar] Int)\n"}, "attrs": {"object": "passes.type_checking.FunAttr", "fields": {"defined": true, "is_global": true, "carry_return": false, "address": null, "preserves": {"set": []}, "section": null, "param_zeropage": {"tuple": []}, "returns_bool": false, "noreturn": false}}}}], ["@0.k", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "UChar\n"}, "attrs": {"object": "passes.type_checking.LocalAttr", "fields": {"register": false, "was_bool": false}}}}]]}}}, {"object": "passes.type_checking.TypeTable", "fields": {"_table": {"dict": []}}}]})~~"}),
     text({R"~~({"input": {"asdl": "c99_ast", "dump": "Program([\n  VarDecl(var_decl(\"s\" FunctionCall(\"__c6502_bin_to_bcd\" [Constant(ConstUInt(12345) UInt)] ULong false) ULong Static None None None false))\n  VarDecl(var_decl(\"t\" FunctionCall(\"__c6502_bin_to_bcd\" [Constant(ConstUInt(65535) UInt)] ULong false) ULong None None None None false))\n  VarDecl(var_decl(\"u\" FunctionCall(\"__c6502_bin_to_bcd\" [Constant(ConstUInt(4464) UInt)] ULong false) ULong None None None None false))\n  VarDecl(var_decl(\"a\" None Array(Int 16) None None None None false))\n  FunctionDecl(function_decl(\"f\" [\"@0.k\"] Block([\n    S(SwitchStmt(Cast(Int Var(\"@0.k\" UChar) Int) Compound(Block([\n      S(CaseStmt(FunctionCall(\"__c6502_bin_to_bcd\" [Constant(ConstInt(42) None)] None false) Return(Constant(ConstInt(1) Int)) \"\"))\n    ])) \"\" [] None Int))\n    S(Return(Cast(Int SizeOfExp(Var(\"a\" Array(Int 16)) UInt) Int)))\n  ]) FunType([UChar] Int) None None [\"\"] false None [] None false))\n])\n"}, "args": {}, "kwargs": {}})~~"})},
    {text({R"~~(Program([
  VarDecl(var_decl("A" None Array(Array(Const(UChar) 14) 11) Extern None None None false))
  VarDecl(var_decl("t" InitList([Subscript(Var("A" None) Constant(ConstInt(5) None) None) AddressOf(Subscript(Subscript(Var("A" None) Constant(ConstInt(2) None) None) Constant(ConstInt(3) None) None) None) Binary(Add Subscript(Var("A" None) Constant(ConstInt(5) None) None) Constant(ConstInt(2) None) None) Binary(Add AddressOf(Subscript(Subscript(Var("A" None) Constant(ConstInt(1) None) None) Constant(ConstInt(0) None) None) None) Constant(ConstInt(1) None) None) Binary(Subtract Subscript(Var("A" None) Constant(ConstInt(5) None) None) Constant(ConstInt(1) None) None)] None None) Array(Const(Pointer(Const(UChar))) 0) Static None None None false))
])
)~~"}),
     false,
     text({R"~~({"tuple": [{"asdl": "c99_ast", "dump": "Program([\n  VarDecl(var_decl(\"A\" None Array(Array(Const(UChar) 14) 11) Extern None None None false))\n  VarDecl(var_decl(\"t\" InitList([AddressOf(Subscript(AddressOf(Var(\"A\" Array(Array(Const(UChar) 14) 11)) Pointer(Array(Const(UChar) 14))) Constant(ConstInt(5) Int) Array(Const(UChar) 14)) Pointer(Const(UChar))) AddressOf(Subscript(AddressOf(Subscript(AddressOf(Var(\"A\" Array(Array(Const(UChar) 14) 11)) Pointer(Array(Const(UChar) 14))) Constant(ConstInt(2) Int) Array(Const(UChar) 14)) Pointer(Const(UChar))) Constant(ConstInt(3) Int) Const(UChar)) Pointer(Const(UChar))) Binary(Add AddressOf(Subscript(AddressOf(Var(\"A\" Array(Array(Const(UChar) 14) 11)) Pointer(Array(Const(UChar) 14))) Constant(ConstInt(5) Int) Array(Const(UChar) 14)) Pointer(Const(UChar))) Constant(ConstInt(2) Int) Pointer(Const(UChar))) Binary(Add AddressOf(Subscript(AddressOf(Subscript(AddressOf(Var(\"A\" Array(Array(Const(UChar) 14) 11)) Pointer(Array(Const(UChar) 14))) Constant(ConstInt(1) Int) Array(Const(UChar) 14)) Pointer(Const(UChar))) Constant(ConstInt(0) Int) Const(UChar)) Pointer(Const(UChar))) Constant(ConstInt(1) Int) Pointer(Const(UChar))) Binary(Subtract AddressOf(Subscript(AddressOf(Var(\"A\" Array(Array(Const(UChar) 14) 11)) Pointer(Array(Const(UChar) 14))) Constant(ConstInt(5) Int) Array(Const(UChar) 14)) Pointer(Const(UChar))) Constant(ConstInt(1) Int) Pointer(Const(UChar)))] Array(Const(Pointer(Const(UChar))) 5) None) Array(Const(Pointer(Const(UChar))) 5) Static None None None false))\n])\n"}, {"object": "passes.type_checking.SymbolTable", "fields": {"_table": {"dict": [["A", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Array(Array(Const(UChar) 14) 11)\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.NoInitializer", "fields": {}}, "is_global": true, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}], ["t", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Array(Const(Pointer(Const(UChar))) 5)\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Initial", "fields": {"value": {"tuple": [{"object": "passes.type_checking.AddressInit", "fields": {"name": "A", "offset": 70}}, {"object": "passes.type_checking.AddressInit", "fields": {"name": "A", "offset": 31}}, {"object": "passes.type_checking.AddressInit", "fields": {"name": "A", "offset": 72}}, {"object": "passes.type_checking.AddressInit", "fields": {"name": "A", "offset": 15}}, {"object": "passes.type_checking.AddressInit", "fields": {"name": "A", "offset": 69}}]}}}, "is_global": false, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}]]}}}, {"object": "passes.type_checking.TypeTable", "fields": {"_table": {"dict": []}}}]})~~"}),
     text({R"~~({"input": {"asdl": "c99_ast", "dump": "Program([\n  VarDecl(var_decl(\"A\" None Array(Array(Const(UChar) 14) 11) Extern None None None false))\n  VarDecl(var_decl(\"t\" InitList([AddressOf(Subscript(AddressOf(Var(\"A\" Array(Array(Const(UChar) 14) 11)) Pointer(Array(Const(UChar) 14))) Constant(ConstInt(5) Int) Array(Const(UChar) 14)) Pointer(Const(UChar))) AddressOf(Subscript(AddressOf(Subscript(AddressOf(Var(\"A\" Array(Array(Const(UChar) 14) 11)) Pointer(Array(Const(UChar) 14))) Constant(ConstInt(2) Int) Array(Const(UChar) 14)) Pointer(Const(UChar))) Constant(ConstInt(3) Int) Const(UChar)) Pointer(Const(UChar))) Binary(Add AddressOf(Subscript(AddressOf(Var(\"A\" Array(Array(Const(UChar) 14) 11)) Pointer(Array(Const(UChar) 14))) Constant(ConstInt(5) Int) Array(Const(UChar) 14)) Pointer(Const(UChar))) Constant(ConstInt(2) Int) Pointer(Const(UChar))) Binary(Add AddressOf(Subscript(AddressOf(Subscript(AddressOf(Var(\"A\" Array(Array(Const(UChar) 14) 11)) Pointer(Array(Const(UChar) 14))) Constant(ConstInt(1) Int) Array(Const(UChar) 14)) Pointer(Const(UChar))) Constant(ConstInt(0) Int) Const(UChar)) Pointer(Const(UChar))) Constant(ConstInt(1) Int) Pointer(Const(UChar))) Binary(Subtract AddressOf(Subscript(AddressOf(Var(\"A\" Array(Array(Const(UChar) 14) 11)) Pointer(Array(Const(UChar) 14))) Constant(ConstInt(5) Int) Array(Const(UChar) 14)) Pointer(Const(UChar))) Constant(ConstInt(1) Int) Pointer(Const(UChar)))] Array(Const(Pointer(Const(UChar))) 5) None) Array(Const(Pointer(Const(UChar))) 5) Static None None None false))\n])\n"}, "args": {}, "kwargs": {}})~~"})},
    {text({R"~~(Program([
  FunctionDecl(function_decl("f" ["@0.p" "@1.q"] Block([
    S(Return(Binary(Add Binary(Add Binary(Subtract Var("@0.p" None) Var("@1.q" None) None) Binary(LessThan Var("@0.p" None) Var("@1.q" None) None) None) Binary(Equal Var("@0.p" None) Var("@1.q" None) None) None)))
  ]) FunType([Const(Pointer(Int)) Pointer(Int)] Int) None None ["" ""] false None [] None false))
  FunctionDecl(function_decl("g" ["@2.c" "@3.p"] Block([
    S(Return(Conditional(Var("@2.c" None) Var("@3.p" None) Constant(ConstInt(0) None) None)))
  ]) FunType([Int Const(Pointer(Int))] Pointer(Int)) None None ["" ""] false None [] None false))
  VarDecl(var_decl("cp" Constant(ConstInt(0) None) Const(Pointer(Int)) None None None None false))
  FunctionDecl(function_decl("h" [] Block([
    D(VarDecl(var_decl("@4.v" Var("cp" None) Pointer(Void) None None None None false)))
    S(Return(Var("@4.v" None)))
  ]) FunType([] Pointer(Void)) None None [] false None [] None false))
])
)~~"}),
     false,
     text({R"~~({"tuple": [{"asdl": "c99_ast", "dump": "Program([\n  FunctionDecl(function_decl(\"f\" [\"@0.p\" \"@1.q\"] Block([\n    S(Return(Binary(Add Binary(Add Binary(Subtract Var(\"@0.p\" Const(Pointer(Int))) Var(\"@1.q\" Pointer(Int)) Int) Binary(LessThan Var(\"@0.p\" Const(Pointer(Int))) Var(\"@1.q\" Pointer(Int)) Int) Int) Binary(Equal Var(\"@0.p\" Const(Pointer(Int))) Var(\"@1.q\" Pointer(Int)) Int) Int)))\n  ]) FunType([Const(Pointer(Int)) Pointer(Int)] Int) None None [\"\" \"\"] false None [] None false))\n  FunctionDecl(function_decl(\"g\" [\"@2.c\" \"@3.p\"] Block([\n    S(Return(Conditional(Var(\"@2.c\" Int) Var(\"@3.p\" Const(Pointer(Int))) Cast(Pointer(Int) Constant(ConstInt(0) Int) Pointer(Int)) Pointer(Int))))\n  ]) FunType([Int Const(Pointer(Int))] Pointer(Int)) None None [\"\" \"\"] false None [] None false))\n  VarDecl(var_decl(\"cp\" Cast(Const(Pointer(Int)) Constant(ConstInt(0) Int) Const(Pointer(Int))) Const(Pointer(Int)) None None None None false))\n  FunctionDecl(function_decl(\"h\" [] Block([\n    D(VarDecl(var_decl(\"@4.v\" Cast(Pointer(Void) Var(\"cp\" Const(Pointer(Int))) Pointer(Void)) Pointer(Void) None None None None false)))\n    S(Return(Var(\"@4.v\" Pointer(Void))))\n  ]) FunType([] Pointer(Void)) None None [] false None [] None false))\n])\n"}, {"object": "passes.type_checking.SymbolTable", "fields": {"_table": {"dict": [["f", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "FunType([Const(Pointer(Int)) Pointer(Int)] Int)\n"}, "attrs": {"object": "passes.type_checking.FunAttr", "fields": {"defined": true, "is_global": true, "carry_return": false, "address": null, "preserves": {"set": []}, "section": null, "param_zeropage": {"tuple": []}, "returns_bool": false, "noreturn": false}}}}], ["@0.p", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Const(Pointer(Int))\n"}, "attrs": {"object": "passes.type_checking.LocalAttr", "fields": {"register": false, "was_bool": false}}}}], ["@1.q", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Pointer(Int)\n"}, "attrs": {"object": "passes.type_checking.LocalAttr", "fields": {"register": false, "was_bool": false}}}}], ["g", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "FunType([Int Const(Pointer(Int))] Pointer(Int))\n"}, "attrs": {"object": "passes.type_checking.FunAttr", "fields": {"defined": true, "is_global": true, "carry_return": false, "address": null, "preserves": {"set": []}, "section": null, "param_zeropage": {"tuple": []}, "returns_bool": false, "noreturn": false}}}}], ["@2.c", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Int\n"}, "attrs": {"object": "passes.type_checking.LocalAttr", "fields": {"register": false, "was_bool": false}}}}], ["@3.p", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Const(Pointer(Int))\n"}, "attrs": {"object": "passes.type_checking.LocalAttr", "fields": {"register": false, "was_bool": false}}}}], ["cp", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Const(Pointer(Int))\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Initial", "fields": {"value": 0}}, "is_global": true, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}], ["h", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "FunType([] Pointer(Void))\n"}, "attrs": {"object": "passes.type_checking.FunAttr", "fields": {"defined": true, "is_global": true, "carry_return": false, "address": null, "preserves": {"set": []}, "section": null, "param_zeropage": {"tuple": []}, "returns_bool": false, "noreturn": false}}}}], ["@4.v", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Pointer(Void)\n"}, "attrs": {"object": "passes.type_checking.LocalAttr", "fields": {"register": false, "was_bool": false}}}}]]}}}, {"object": "passes.type_checking.TypeTable", "fields": {"_table": {"dict": []}}}]})~~"}),
     text({R"~~({"input": {"asdl": "c99_ast", "dump": "Program([\n  FunctionDecl(function_decl(\"f\" [\"@0.p\" \"@1.q\"] Block([\n    S(Return(Binary(Add Binary(Add Binary(Subtract Var(\"@0.p\" Const(Pointer(Int))) Var(\"@1.q\" Pointer(Int)) Int) Binary(LessThan Var(\"@0.p\" Const(Pointer(Int))) Var(\"@1.q\" Pointer(Int)) Int) Int) Binary(Equal Var(\"@0.p\" Const(Pointer(Int))) Var(\"@1.q\" Pointer(Int)) Int) Int)))\n  ]) FunType([Const(Pointer(Int)) Pointer(Int)] Int) None None [\"\" \"\"] false None [] None false))\n  FunctionDecl(function_decl(\"g\" [\"@2.c\" \"@3.p\"] Block([\n    S(Return(Conditional(Var(\"@2.c\" Int) Var(\"@3.p\" Const(Pointer(Int))) Cast(Pointer(Int) Constant(ConstInt(0) Int) Pointer(Int)) Pointer(Int))))\n  ]) FunType([Int Const(Pointer(Int))] Pointer(Int)) None None [\"\" \"\"] false None [] None false))\n  VarDecl(var_decl(\"cp\" Cast(Const(Pointer(Int)) Constant(ConstInt(0) Int) Const(Pointer(Int))) Const(Pointer(Int)) None None None None false))\n  FunctionDecl(function_decl(\"h\" [] Block([\n    D(VarDecl(var_decl(\"@4.v\" Cast(Pointer(Void) Var(\"cp\" Const(Pointer(Int))) Pointer(Void)) Pointer(Void) None None None None false)))\n    S(Return(Var(\"@4.v\" Pointer(Void))))\n  ]) FunType([] Pointer(Void)) None None [] false None [] None false))\n])\n"}, "args": {}, "kwargs": {}})~~"})},
    {text({R"~~(Program([
  VarDecl(var_decl("g" None Int None None None None false))
  VarDecl(var_decl("a" Constant(ConstInt(256) None) Bool None None None None false))
  VarDecl(var_decl("b" AddressOf(Var("g" None) None) Bool None None None None false))
  VarDecl(var_decl("c" Constant(ConstDouble(0) None) Bool None None None None false))
  VarDecl(var_decl("d" Unary(Negate Constant(ConstDouble(0) None) None) Bool None None None None false))
  VarDecl(var_decl("e" Constant(ConstDouble(4602678819172646912) None) Bool None None None None false))
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("@0.s" Constant(ConstInt(7) None) Bool Static None None None false)))
    S(Return(Binary(Add Binary(Add Binary(Add Binary(Add Binary(Add Var("a" None) Binary(Multiply Var("b" None) Constant(ConstInt(2) None) None) None) Binary(Multiply Var("c" None) Constant(ConstInt(4) None) None) None) Binary(Multiply Var("d" None) Constant(ConstInt(8) None) None) None) Binary(Multiply Var("e" None) Constant(ConstInt(16) None) None) None) Binary(Multiply Var("@0.s" None) Constant(ConstInt(32) None) None) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"}),
     false,
     text({R"~~({"tuple": [{"asdl": "c99_ast", "dump": "Program([\n  VarDecl(var_decl(\"g\" None Int None None None None false))\n  VarDecl(var_decl(\"a\" Cast(Bool Constant(ConstInt(256) Int) Bool) Bool None None None None false))\n  VarDecl(var_decl(\"b\" Cast(Bool AddressOf(Var(\"g\" Int) Pointer(Int)) Bool) Bool None None None None false))\n  VarDecl(var_decl(\"c\" Cast(Bool Constant(ConstDouble(0) Double) Bool) Bool None None None None false))\n  VarDecl(var_decl(\"d\" Cast(Bool Unary(Negate Constant(ConstDouble(0) Double) Double) Bool) Bool None None None None false))\n  VarDecl(var_decl(\"e\" Cast(Bool Constant(ConstDouble(4602678819172646912) Double) Bool) Bool None None None None false))\n  FunctionDecl(function_decl(\"main\" [] Block([\n    D(VarDecl(var_decl(\"@0.s\" Cast(Bool Constant(ConstInt(7) Int) Bool) Bool Static None None None false)))\n    S(Return(Binary(Add Binary(Add Binary(Add Binary(Add Binary(Add Cast(Int Var(\"a\" Bool) Int) Binary(Multiply Cast(Int Var(\"b\" Bool) Int) Constant(ConstInt(2) Int) Int) Int) Binary(Multiply Cast(Int Var(\"c\" Bool) Int) Constant(ConstInt(4) Int) Int) Int) Binary(Multiply Cast(Int Var(\"d\" Bool) Int) Constant(ConstInt(8) Int) Int) Int) Binary(Multiply Cast(Int Var(\"e\" Bool) Int) Constant(ConstInt(16) Int) Int) Int) Binary(Multiply Cast(Int Var(\"@0.s\" Bool) Int) Constant(ConstInt(32) Int) Int) Int)))\n  ]) FunType([] Int) None None [] false None [] None false))\n])\n"}, {"object": "passes.type_checking.SymbolTable", "fields": {"_table": {"dict": [["g", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Int\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Tentative", "fields": {}}, "is_global": true, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}], ["a", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Bool\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Initial", "fields": {"value": 1}}, "is_global": true, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}], ["b", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Bool\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Initial", "fields": {"value": 1}}, "is_global": true, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}], ["c", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Bool\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Initial", "fields": {"value": 0}}, "is_global": true, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}], ["d", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Bool\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Initial", "fields": {"value": 0}}, "is_global": true, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}], ["e", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Bool\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Initial", "fields": {"value": 1}}, "is_global": true, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}], ["main", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "FunType([] Int)\n"}, "attrs": {"object": "passes.type_checking.FunAttr", "fields": {"defined": true, "is_global": true, "carry_return": false, "address": null, "preserves": {"set": []}, "section": null, "param_zeropage": {"tuple": []}, "returns_bool": false, "noreturn": false}}}}], ["@0.s", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Bool\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Initial", "fields": {"value": 1}}, "is_global": false, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}]]}}}, {"object": "passes.type_checking.TypeTable", "fields": {"_table": {"dict": []}}}]})~~"}),
     text({R"~~({"input": {"asdl": "c99_ast", "dump": "Program([\n  VarDecl(var_decl(\"g\" None Int None None None None false))\n  VarDecl(var_decl(\"a\" Cast(Bool Constant(ConstInt(256) Int) Bool) Bool None None None None false))\n  VarDecl(var_decl(\"b\" Cast(Bool AddressOf(Var(\"g\" Int) Pointer(Int)) Bool) Bool None None None None false))\n  VarDecl(var_decl(\"c\" Cast(Bool Constant(ConstDouble(0) Double) Bool) Bool None None None None false))\n  VarDecl(var_decl(\"d\" Cast(Bool Unary(Negate Constant(ConstDouble(0) Double) Double) Bool) Bool None None None None false))\n  VarDecl(var_decl(\"e\" Cast(Bool Constant(ConstDouble(4602678819172646912) Double) Bool) Bool None None None None false))\n  FunctionDecl(function_decl(\"main\" [] Block([\n    D(VarDecl(var_decl(\"@0.s\" Cast(Bool Constant(ConstInt(7) Int) Bool) Bool Static None None None false)))\n    S(Return(Binary(Add Binary(Add Binary(Add Binary(Add Binary(Add Cast(Int Var(\"a\" Bool) Int) Binary(Multiply Cast(Int Var(\"b\" Bool) Int) Constant(ConstInt(2) Int) Int) Int) Binary(Multiply Cast(Int Var(\"c\" Bool) Int) Constant(ConstInt(4) Int) Int) Int) Binary(Multiply Cast(Int Var(\"d\" Bool) Int) Constant(ConstInt(8) Int) Int) Int) Binary(Multiply Cast(Int Var(\"e\" Bool) Int) Constant(ConstInt(16) Int) Int) Int) Binary(Multiply Cast(Int Var(\"@0.s\" Bool) Int) Constant(ConstInt(32) Int) Int) Int)))\n  ]) FunType([] Int) None None [] false None [] None false))\n])\n"}, "args": {}, "kwargs": {}})~~"})},
    {text({R"~~(Program([
  StructDecl(struct_decl("p" false [member_decl("x" Int) member_decl("y" Int)]))
  StructDecl(struct_decl("s" false [member_decl("n" Char) member_decl("pt" Array(Structure("p") 2)) member_decl("name" Array(Char 4))]))
  StructDecl(struct_decl("u" true [member_decl("c" Char) member_decl("l" Long) member_decl("i" Int)]))
  StructDecl(struct_decl("w" false [member_decl("tag" Int) member_decl("v" Union("u"))]))
  VarDecl(var_decl("K" Constant(ConstInt(2) None) Const(Int) None None None None true))
  VarDecl(var_decl("a" InitList([InitList([] None None) InitList([] None None) Constant(ConstInt(5) None) Constant(ConstInt(6) None)] None None) Array(Int 4) None None None None false))
  VarDecl(var_decl("c" InitList([InitList([] None None) Constant(ConstInt(4) None)] None None) Const(Structure("p")) None None None None true))
  VarDecl(var_decl("b" None Array(Int 4) None None None None false))
  VarDecl(var_decl("cu" InitList([Constant(ConstInt(3) None)] None "i") Const(Union("u")) None None None None true))
  VarDecl(var_decl("d" None Array(Int 3) None None None None false))
])
)~~"}),
     false,
     text({R"~~({"tuple": [{"asdl": "c99_ast", "dump": "Program([\n  StructDecl(struct_decl(\"p\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" Char) member_decl(\"pt\" Array(Structure(\"p\") 2)) member_decl(\"name\" Array(Char 4))]))\n  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long) member_decl(\"i\" Int)]))\n  StructDecl(struct_decl(\"w\" false [member_decl(\"tag\" Int) member_decl(\"v\" Union(\"u\"))]))\n  VarDecl(var_decl(\"K\" Constant(ConstInt(2) Int) Const(Int) Static None None None true))\n  VarDecl(var_decl(\"a\" InitList([Constant(ConstInt(0) Int) Constant(ConstInt(0) Int) Constant(ConstInt(5) Int) Constant(ConstInt(6) Int)] Array(Int 4) None) Array(Int 4) None None None None false))\n  VarDecl(var_decl(\"c\" InitList([Constant(ConstInt(0) Int) Constant(ConstInt(4) Int)] Structure(\"p\") None) Const(Structure(\"p\")) Static None None None true))\n  VarDecl(var_decl(\"b\" None Array(Int 4) None None None None false))\n  VarDecl(var_decl(\"cu\" InitList([Constant(ConstInt(3) Int)] Union(\"u\") \"i\") Const(Union(\"u\")) Static None None None true))\n  VarDecl(var_decl(\"d\" None Array(Int 3) None None None None false))\n])\n"}, {"object": "passes.type_checking.SymbolTable", "fields": {"_table": {"dict": [["K", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Const(Int)\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Initial", "fields": {"value": 2}}, "is_global": false, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}], ["a", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Array(Int 4)\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Initial", "fields": {"value": {"tuple": [0, 0, 5, 6]}}}, "is_global": true, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}], ["c", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Const(Structure(\"p\"))\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Initial", "fields": {"value": {"tuple": [0, 4]}}}, "is_global": false, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}], ["b", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Array(Int 4)\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Tentative", "fields": {}}, "is_global": true, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}], ["cu", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Const(Union(\"u\"))\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Initial", "fields": {"value": {"object": "passes.type_checking.UnionValue", "fields": {"member": 2, "value": 3}}}}, "is_global": false, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}], ["d", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Array(Int 3)\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Tentative", "fields": {}}, "is_global": true, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}]]}}}, {"object": "passes.type_checking.TypeTable", "fields": {"_table": {"dict": [["p", {"object": "passes.type_checking.StructLayout", "fields": {"tag": "p", "is_union": false, "members": [{"object": "passes.type_checking.MemberInfo", "fields": {"name": "x", "type": {"asdl": "c99_ast", "dump": "Int\n"}, "byte_offset": 0}}, {"object": "passes.type_checking.MemberInfo", "fields": {"name": "y", "type": {"asdl": "c99_ast", "dump": "Int\n"}, "byte_offset": 2}}], "size": 4, "complete": true}}], ["s", {"object": "passes.type_checking.StructLayout", "fields": {"tag": "s", "is_union": false, "members": [{"object": "passes.type_checking.MemberInfo", "fields": {"name": "n", "type": {"asdl": "c99_ast", "dump": "Char\n"}, "byte_offset": 0}}, {"object": "passes.type_checking.MemberInfo", "fields": {"name": "pt", "type": {"asdl": "c99_ast", "dump": "Array(Structure(\"p\") 2)\n"}, "byte_offset": 1}}, {"object": "passes.type_checking.MemberInfo", "fields": {"name": "name", "type": {"asdl": "c99_ast", "dump": "Array(Char 4)\n"}, "byte_offset": 9}}], "size": 13, "complete": true}}], ["u", {"object": "passes.type_checking.StructLayout", "fields": {"tag": "u", "is_union": true, "members": [{"object": "passes.type_checking.MemberInfo", "fields": {"name": "c", "type": {"asdl": "c99_ast", "dump": "Char\n"}, "byte_offset": 0}}, {"object": "passes.type_checking.MemberInfo", "fields": {"name": "l", "type": {"asdl": "c99_ast", "dump": "Long\n"}, "byte_offset": 0}}, {"object": "passes.type_checking.MemberInfo", "fields": {"name": "i", "type": {"asdl": "c99_ast", "dump": "Int\n"}, "byte_offset": 0}}], "size": 4, "complete": true}}], ["w", {"object": "passes.type_checking.StructLayout", "fields": {"tag": "w", "is_union": false, "members": [{"object": "passes.type_checking.MemberInfo", "fields": {"name": "tag", "type": {"asdl": "c99_ast", "dump": "Int\n"}, "byte_offset": 0}}, {"object": "passes.type_checking.MemberInfo", "fields": {"name": "v", "type": {"asdl": "c99_ast", "dump": "Union(\"u\")\n"}, "byte_offset": 2}}], "size": 6, "complete": true}}]]}}}]})~~"}),
     text({R"~~({"input": {"asdl": "c99_ast", "dump": "Program([\n  StructDecl(struct_decl(\"p\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" Char) member_decl(\"pt\" Array(Structure(\"p\") 2)) member_decl(\"name\" Array(Char 4))]))\n  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long) member_decl(\"i\" Int)]))\n  StructDecl(struct_decl(\"w\" false [member_decl(\"tag\" Int) member_decl(\"v\" Union(\"u\"))]))\n  VarDecl(var_decl(\"K\" Constant(ConstInt(2) Int) Const(Int) Static None None None true))\n  VarDecl(var_decl(\"a\" InitList([Constant(ConstInt(0) Int) Constant(ConstInt(0) Int) Constant(ConstInt(5) Int) Constant(ConstInt(6) Int)] Array(Int 4) None) Array(Int 4) None None None None false))\n  VarDecl(var_decl(\"c\" InitList([Constant(ConstInt(0) Int) Constant(ConstInt(4) Int)] Structure(\"p\") None) Const(Structure(\"p\")) Static None None None true))\n  VarDecl(var_decl(\"b\" None Array(Int 4) None None None None false))\n  VarDecl(var_decl(\"cu\" InitList([Constant(ConstInt(3) Int)] Union(\"u\") \"i\") Const(Union(\"u\")) Static None None None true))\n  VarDecl(var_decl(\"d\" None Array(Int 3) None None None None false))\n])\n"}, "args": {}, "kwargs": {}})~~"})},
    {text({R"~~(Program([
  VarDecl(var_decl("c" Constant(ConstInt(200) None) UChar None None None None false))
  VarDecl(var_decl("s" Unary(Negate Constant(ConstInt(5) None) None) SChar None None None None false))
  VarDecl(var_decl("n" Unary(Plus Constant(ConstInt(7) None) None) Int None None None None false))
  VarDecl(var_decl("d" Unary(Plus Constant(ConstDouble(4609434218613702656) None) None) Double None None None None false))
  VarDecl(var_decl("arr" None Array(Int 4) None None None None false))
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("@0.a" Unary(Plus Var("c" None) None) Int None None None None false)))
    D(VarDecl(var_decl("@1.b" Unary(Plus Var("s" None) None) Int None None None None false)))
    D(VarDecl(var_decl("@2.l" Unary(Plus Cast(Long Var("s" None) None) None) Long None None None None false)))
    S(SwitchStmt(Var("@0.a" None) Compound(Block([
      S(CaseStmt(Unary(Plus Constant(ConstInt(200) None) None) Expression(Assignment(Var("@0.a" None) Binary(Add Var("@0.a" None) Constant(ConstInt(1) None) None) None)) ".case@1"))
    ])) ".switch@0" [switch_case(Unary(Plus Constant(ConstInt(200) None) None) ".case@1")] None None))
    S(Return(Binary(Add Binary(Add Binary(Add Binary(Add Binary(Add Binary(Add Binary(Add Var("@0.a" None) Var("@1.b" None) None) Cast(Int Var("@2.l" None) None) None) Var("n" None) None) Constant(ConstInt(3) None) None) Cast(Int SizeOfExp(Var("arr" None) None) None) None) Binary(GreaterThan Var("d" None) Constant(ConstDouble(4607182418800017408) None) None) None) SizeOfExp(Unary(Plus Var("c" None) None) None) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"}),
     false,
     text({R"~~({"tuple": [{"asdl": "c99_ast", "dump": "Program([\n  VarDecl(var_decl(\"c\" Cast(UChar Constant(ConstInt(200) Int) UChar) UChar None None None None false))\n  VarDecl(var_decl(\"s\" Cast(SChar Unary(Negate Constant(ConstInt(5) Int) Int) SChar) SChar None None None None false))\n  VarDecl(var_decl(\"n\" Unary(Plus Constant(ConstInt(7) Int) Int) Int None None None None false))\n  VarDecl(var_decl(\"d\" Unary(Plus Constant(ConstDouble(4609434218613702656) Double) Double) Double None None None None false))\n  VarDecl(var_decl(\"arr\" None Array(Int 4) None None None None false))\n  FunctionDecl(function_decl(\"main\" [] Block([\n    D(VarDecl(var_decl(\"@0.a\" Unary(Plus Cast(Int Var(\"c\" UChar) Int) Int) Int None None None None false)))\n    D(VarDecl(var_decl(\"@1.b\" Unary(Plus Cast(Int Var(\"s\" SChar) Int) Int) Int None None None None false)))\n    D(VarDecl(var_decl(\"@2.l\" Unary(Plus Cast(Long Var(\"s\" SChar) Long) Long) Long None None None None false)))\n    S(SwitchStmt(Var(\"@0.a\" Int) Compound(Block([\n      S(CaseStmt(Unary(Plus Constant(ConstInt(200) Int) Int) Expression(Assignment(Var(\"@0.a\" Int) Binary(Add Var(\"@0.a\" Int) Constant(ConstInt(1) Int) Int) Int)) \".case@1\"))\n    ])) \".switch@0\" [switch_case(Constant(ConstInt(200) Int) \".case@1\")] None Int))\n    S(Return(Cast(Int Binary(Add Cast(UInt Binary(Add Binary(Add Binary(Add Binary(Add Binary(Add Binary(Add Var(\"@0.a\" Int) Var(\"@1.b\" Int) Int) Cast(Int Var(\"@2.l\" Long) Int) Int) Var(\"n\" Int) Int) Constant(ConstInt(3) Int) Int) Cast(Int SizeOfExp(Var(\"arr\" Array(Int 4)) UInt) Int) Int) Binary(GreaterThan Var(\"d\" Double) Constant(ConstDouble(4607182418800017408) Double) Int) Int) UInt) SizeOfExp(Unary(Plus Cast(Int Var(\"c\" UChar) Int) Int) UInt) UInt) Int)))\n  ]) FunType([] Int) None None [] false None [] None false))\n])\n"}, {"object": "passes.type_checking.SymbolTable", "fields": {"_table": {"dict": [["c", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "UChar\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Initial", "fields": {"value": 200}}, "is_global": true, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}], ["s", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "SChar\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Initial", "fields": {"value": -5}}, "is_global": true, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}], ["n", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Int\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Initial", "fields": {"value": 7}}, "is_global": true, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}], ["d", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Double\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Initial", "fields": {"value": 4609434218613702656}}, "is_global": true, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}], ["arr", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Array(Int 4)\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Tentative", "fields": {}}, "is_global": true, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}], ["main", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "FunType([] Int)\n"}, "attrs": {"object": "passes.type_checking.FunAttr", "fields": {"defined": true, "is_global": true, "carry_return": false, "address": null, "preserves": {"set": []}, "section": null, "param_zeropage": {"tuple": []}, "returns_bool": false, "noreturn": false}}}}], ["@0.a", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Int\n"}, "attrs": {"object": "passes.type_checking.LocalAttr", "fields": {"register": false, "was_bool": false}}}}], ["@1.b", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Int\n"}, "attrs": {"object": "passes.type_checking.LocalAttr", "fields": {"register": false, "was_bool": false}}}}], ["@2.l", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Long\n"}, "attrs": {"object": "passes.type_checking.LocalAttr", "fields": {"register": false, "was_bool": false}}}}]]}}}, {"object": "passes.type_checking.TypeTable", "fields": {"_table": {"dict": []}}}]})~~"}),
     text({R"~~({"input": {"asdl": "c99_ast", "dump": "Program([\n  VarDecl(var_decl(\"c\" Cast(UChar Constant(ConstInt(200) Int) UChar) UChar None None None None false))\n  VarDecl(var_decl(\"s\" Cast(SChar Unary(Negate Constant(ConstInt(5) Int) Int) SChar) SChar None None None None false))\n  VarDecl(var_decl(\"n\" Unary(Plus Constant(ConstInt(7) Int) Int) Int None None None None false))\n  VarDecl(var_decl(\"d\" Unary(Plus Constant(ConstDouble(4609434218613702656) Double) Double) Double None None None None false))\n  VarDecl(var_decl(\"arr\" None Array(Int 4) None None None None false))\n  FunctionDecl(function_decl(\"main\" [] Block([\n    D(VarDecl(var_decl(\"@0.a\" Unary(Plus Cast(Int Var(\"c\" UChar) Int) Int) Int None None None None false)))\n    D(VarDecl(var_decl(\"@1.b\" Unary(Plus Cast(Int Var(\"s\" SChar) Int) Int) Int None None None None false)))\n    D(VarDecl(var_decl(\"@2.l\" Unary(Plus Cast(Long Var(\"s\" SChar) Long) Long) Long None None None None false)))\n    S(SwitchStmt(Var(\"@0.a\" Int) Compound(Block([\n      S(CaseStmt(Unary(Plus Constant(ConstInt(200) Int) Int) Expression(Assignment(Var(\"@0.a\" Int) Binary(Add Var(\"@0.a\" Int) Constant(ConstInt(1) Int) Int) Int)) \".case@1\"))\n    ])) \".switch@0\" [switch_case(Constant(ConstInt(200) Int) \".case@1\")] None Int))\n    S(Return(Cast(Int Binary(Add Cast(UInt Binary(Add Binary(Add Binary(Add Binary(Add Binary(Add Binary(Add Var(\"@0.a\" Int) Var(\"@1.b\" Int) Int) Cast(Int Var(\"@2.l\" Long) Int) Int) Var(\"n\" Int) Int) Constant(ConstInt(3) Int) Int) Cast(Int SizeOfExp(Var(\"arr\" Array(Int 4)) UInt) Int) Int) Binary(GreaterThan Var(\"d\" Double) Constant(ConstDouble(4607182418800017408) Double) Int) Int) UInt) SizeOfExp(Unary(Plus Cast(Int Var(\"c\" UChar) Int) Int) UInt) UInt) Int)))\n  ]) FunType([] Int) None None [] false None [] None false))\n])\n"}, "args": {}, "kwargs": {}})~~"})},
    {text({R"~~(Program([
  FunctionDecl(function_decl("stdc_leading_zeros_uc" ["@0.__value"] None FunType([UChar] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_leading_zeros_us" ["@1.__value"] None FunType([UShort] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_leading_zeros_ui" ["@2.__value"] None FunType([UInt] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_leading_zeros_ul" ["@3.__value"] None FunType([ULong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_leading_zeros_ull" ["@4.__value"] None FunType([ULongLong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_leading_ones_uc" ["@5.__value"] None FunType([UChar] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_leading_ones_us" ["@6.__value"] None FunType([UShort] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_leading_ones_ui" ["@7.__value"] None FunType([UInt] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_leading_ones_ul" ["@8.__value"] None FunType([ULong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_leading_ones_ull" ["@9.__value"] None FunType([ULongLong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_trailing_zeros_uc" ["@10.__value"] None FunType([UChar] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_trailing_zeros_us" ["@11.__value"] None FunType([UShort] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_trailing_zeros_ui" ["@12.__value"] None FunType([UInt] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_trailing_zeros_ul" ["@13.__value"] None FunType([ULong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_trailing_zeros_ull" ["@14.__value"] None FunType([ULongLong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_trailing_ones_uc" ["@15.__value"] None FunType([UChar] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_trailing_ones_us" ["@16.__value"] None FunType([UShort] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_trailing_ones_ui" ["@17.__value"] None FunType([UInt] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_trailing_ones_ul" ["@18.__value"] None FunType([ULong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_trailing_ones_ull" ["@19.__value"] None FunType([ULongLong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_leading_zero_uc" ["@20.__value"] None FunType([UChar] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_leading_zero_us" ["@21.__value"] None FunType([UShort] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_leading_zero_ui" ["@22.__value"] None FunType([UInt] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_leading_zero_ul" ["@23.__value"] None FunType([ULong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_leading_zero_ull" ["@24.__value"] None FunType([ULongLong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_leading_one_uc" ["@25.__value"] None FunType([UChar] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_leading_one_us" ["@26.__value"] None FunType([UShort] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_leading_one_ui" ["@27.__value"] None FunType([UInt] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_leading_one_ul" ["@28.__value"] None FunType([ULong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_leading_one_ull" ["@29.__value"] None FunType([ULongLong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_trailing_zero_uc" ["@30.__value"] None FunType([UChar] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_trailing_zero_us" ["@31.__value"] None FunType([UShort] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_trailing_zero_ui" ["@32.__value"] None FunType([UInt] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_trailing_zero_ul" ["@33.__value"] None FunType([ULong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_trailing_zero_ull" ["@34.__value"] None FunType([ULongLong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_trailing_one_uc" ["@35.__value"] None FunType([UChar] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_trailing_one_us" ["@36.__value"] None FunType([UShort] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_trailing_one_ui" ["@37.__value"] None FunType([UInt] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_trailing_one_ul" ["@38.__value"] None FunType([ULong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_trailing_one_ull" ["@39.__value"] None FunType([ULongLong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_count_zeros_uc" ["@40.__value"] None FunType([UChar] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_count_zeros_us" ["@41.__value"] None FunType([UShort] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_count_zeros_ui" ["@42.__value"] None FunType([UInt] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_count_zeros_ul" ["@43.__value"] None FunType([ULong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_count_zeros_ull" ["@44.__value"] None FunType([ULongLong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_count_ones_uc" ["@45.__value"] None FunType([UChar] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_count_ones_us" ["@46.__value"] None FunType([UShort] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_count_ones_ui" ["@47.__value"] None FunType([UInt] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_count_ones_ul" ["@48.__value"] None FunType([ULong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_count_ones_ull" ["@49.__value"] None FunType([ULongLong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_has_single_bit_uc" ["@50.__value"] None FunType([UChar] Bool) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_has_single_bit_us" ["@51.__value"] None FunType([UShort] Bool) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_has_single_bit_ui" ["@52.__value"] None FunType([UInt] Bool) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_has_single_bit_ul" ["@53.__value"] None FunType([ULong] Bool) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_has_single_bit_ull" ["@54.__value"] None FunType([ULongLong] Bool) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_bit_width_uc" ["@55.__value"] None FunType([UChar] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_bit_width_us" ["@56.__value"] None FunType([UShort] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_bit_width_ui" ["@57.__value"] None FunType([UInt] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_bit_width_ul" ["@58.__value"] None FunType([ULong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_bit_width_ull" ["@59.__value"] None FunType([ULongLong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_bit_floor_uc" ["@60.__value"] None FunType([UChar] UChar) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_bit_floor_us" ["@61.__value"] None FunType([UShort] UShort) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_bit_floor_ui" ["@62.__value"] None FunType([UInt] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_bit_floor_ul" ["@63.__value"] None FunType([ULong] ULong) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_bit_floor_ull" ["@64.__value"] None FunType([ULongLong] ULongLong) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_bit_ceil_uc" ["@65.__value"] None FunType([UChar] UChar) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_bit_ceil_us" ["@66.__value"] None FunType([UShort] UShort) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_bit_ceil_ui" ["@67.__value"] None FunType([UInt] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_bit_ceil_ul" ["@68.__value"] None FunType([ULong] ULong) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_bit_ceil_ull" ["@69.__value"] None FunType([ULongLong] ULongLong) None None [""] false None [] None false))
  VarDecl(var_decl("x" None BitInt(16 true) None None None None false))
  FunctionDecl(function_decl("f" [] Block([
    S(Return(FunctionCall("__c6502_stdc_count_ones" [Var("x" None)] None false)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "the argument of stdc_count_ones has type _BitInt(16), and C23 \u00a77.18 defines the type-generic functions on unsigned char, unsigned short, unsigned int, unsigned long, unsigned long long, and the unsigned _BitInt of 8, 16, 32, and 64 bits, with no integer promotion (cast it to one, or call a function such as stdc_count_ones_ui)"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  FunctionDecl(function_decl("stdc_leading_zeros_uc" ["@0.__value"] None FunType([UChar] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_leading_zeros_us" ["@1.__value"] None FunType([UShort] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_leading_zeros_ui" ["@2.__value"] None FunType([UInt] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_leading_zeros_ul" ["@3.__value"] None FunType([ULong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_leading_zeros_ull" ["@4.__value"] None FunType([ULongLong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_leading_ones_uc" ["@5.__value"] None FunType([UChar] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_leading_ones_us" ["@6.__value"] None FunType([UShort] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_leading_ones_ui" ["@7.__value"] None FunType([UInt] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_leading_ones_ul" ["@8.__value"] None FunType([ULong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_leading_ones_ull" ["@9.__value"] None FunType([ULongLong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_trailing_zeros_uc" ["@10.__value"] None FunType([UChar] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_trailing_zeros_us" ["@11.__value"] None FunType([UShort] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_trailing_zeros_ui" ["@12.__value"] None FunType([UInt] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_trailing_zeros_ul" ["@13.__value"] None FunType([ULong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_trailing_zeros_ull" ["@14.__value"] None FunType([ULongLong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_trailing_ones_uc" ["@15.__value"] None FunType([UChar] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_trailing_ones_us" ["@16.__value"] None FunType([UShort] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_trailing_ones_ui" ["@17.__value"] None FunType([UInt] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_trailing_ones_ul" ["@18.__value"] None FunType([ULong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_trailing_ones_ull" ["@19.__value"] None FunType([ULongLong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_leading_zero_uc" ["@20.__value"] None FunType([UChar] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_leading_zero_us" ["@21.__value"] None FunType([UShort] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_leading_zero_ui" ["@22.__value"] None FunType([UInt] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_leading_zero_ul" ["@23.__value"] None FunType([ULong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_leading_zero_ull" ["@24.__value"] None FunType([ULongLong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_leading_one_uc" ["@25.__value"] None FunType([UChar] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_leading_one_us" ["@26.__value"] None FunType([UShort] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_leading_one_ui" ["@27.__value"] None FunType([UInt] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_leading_one_ul" ["@28.__value"] None FunType([ULong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_leading_one_ull" ["@29.__value"] None FunType([ULongLong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_trailing_zero_uc" ["@30.__value"] None FunType([UChar] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_trailing_zero_us" ["@31.__value"] None FunType([UShort] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_trailing_zero_ui" ["@32.__value"] None FunType([UInt] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_trailing_zero_ul" ["@33.__value"] None FunType([ULong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_trailing_zero_ull" ["@34.__value"] None FunType([ULongLong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_trailing_one_uc" ["@35.__value"] None FunType([UChar] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_trailing_one_us" ["@36.__value"] None FunType([UShort] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_trailing_one_ui" ["@37.__value"] None FunType([UInt] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_trailing_one_ul" ["@38.__value"] None FunType([ULong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_trailing_one_ull" ["@39.__value"] None FunType([ULongLong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_count_zeros_uc" ["@40.__value"] None FunType([UChar] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_count_zeros_us" ["@41.__value"] None FunType([UShort] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_count_zeros_ui" ["@42.__value"] None FunType([UInt] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_count_zeros_ul" ["@43.__value"] None FunType([ULong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_count_zeros_ull" ["@44.__value"] None FunType([ULongLong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_count_ones_uc" ["@45.__value"] None FunType([UChar] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_count_ones_us" ["@46.__value"] None FunType([UShort] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_count_ones_ui" ["@47.__value"] None FunType([UInt] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_count_ones_ul" ["@48.__value"] None FunType([ULong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_count_ones_ull" ["@49.__value"] None FunType([ULongLong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_has_single_bit_uc" ["@50.__value"] None FunType([UChar] Bool) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_has_single_bit_us" ["@51.__value"] None FunType([UShort] Bool) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_has_single_bit_ui" ["@52.__value"] None FunType([UInt] Bool) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_has_single_bit_ul" ["@53.__value"] None FunType([ULong] Bool) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_has_single_bit_ull" ["@54.__value"] None FunType([ULongLong] Bool) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_bit_width_uc" ["@55.__value"] None FunType([UChar] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_bit_width_us" ["@56.__value"] None FunType([UShort] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_bit_width_ui" ["@57.__value"] None FunType([UInt] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_bit_width_ul" ["@58.__value"] None FunType([ULong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_bit_width_ull" ["@59.__value"] None FunType([ULongLong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_bit_floor_uc" ["@60.__value"] None FunType([UChar] UChar) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_bit_floor_us" ["@61.__value"] None FunType([UShort] UShort) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_bit_floor_ui" ["@62.__value"] None FunType([UInt] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_bit_floor_ul" ["@63.__value"] None FunType([ULong] ULong) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_bit_floor_ull" ["@64.__value"] None FunType([ULongLong] ULongLong) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_bit_ceil_uc" ["@65.__value"] None FunType([UChar] UChar) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_bit_ceil_us" ["@66.__value"] None FunType([UShort] UShort) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_bit_ceil_ui" ["@67.__value"] None FunType([UInt] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_bit_ceil_ul" ["@68.__value"] None FunType([ULong] ULong) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_bit_ceil_ull" ["@69.__value"] None FunType([ULongLong] ULongLong) None None [""] false None [] None false))
  VarDecl(var_decl("x" None BitInt(12 false) None None None None false))
  FunctionDecl(function_decl("f" [] Block([
    S(Return(FunctionCall("__c6502_stdc_count_ones" [Var("x" None)] None false)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"}),
     true,
     text({R"~~({"type": "TypeCheckError", "message": "the argument of stdc_count_ones has type unsigned _BitInt(12), and C23 \u00a77.18 defines the type-generic functions on unsigned char, unsigned short, unsigned int, unsigned long, unsigned long long, and the unsigned _BitInt of 8, 16, 32, and 64 bits, with no integer promotion (cast it to one, or call a function such as stdc_count_ones_ui)"})~~"}),
     text({R"~~()~~"})},
    {text({R"~~(Program([
  VarDecl(var_decl("i" InitList([] None None) Int None None None None false))
  VarDecl(var_decl("ul" InitList([] None None) ULong None None None None false))
  VarDecl(var_decl("c" InitList([] None None) Char None None None None false))
  VarDecl(var_decl("sc" InitList([] None None) SChar None None None None false))
  VarDecl(var_decl("ll" InitList([] None None) LongLong None None None None false))
  VarDecl(var_decl("p" InitList([] None None) Pointer(Int) None None None None false))
  VarDecl(var_decl("fp" InitList([] None None) Pointer(FunType([] Void)) None None None None false))
  VarDecl(var_decl("f" InitList([] None None) Float None None None None false))
  VarDecl(var_decl("d" InitList([] None None) Double None None None None false))
  VarDecl(var_decl("b" InitList([] None None) Bool None None None None false))
  VarDecl(var_decl("w" InitList([] None None) BitInt(12 true) None None None None false))
  VarDecl(var_decl("uw" InitList([] None None) BitInt(40 false) None None None None false))
  VarDecl(var_decl("n" InitList([] None None) NullptrT None None None None false))
  VarDecl(var_decl("cv" InitList([] None None) Const(Volatile(Int)) None None None None false))
])
)~~"}),
     false,
     text({R"~~({"tuple": [{"asdl": "c99_ast", "dump": "Program([\n  VarDecl(var_decl(\"i\" Constant(ConstInt(0) Int) Int None None None None false))\n  VarDecl(var_decl(\"ul\" Cast(ULong Constant(ConstInt(0) Int) ULong) ULong None None None None false))\n  VarDecl(var_decl(\"c\" Cast(Char Constant(ConstInt(0) Int) Char) Char None None None None false))\n  VarDecl(var_decl(\"sc\" Cast(SChar Constant(ConstInt(0) Int) SChar) SChar None None None None false))\n  VarDecl(var_decl(\"ll\" Cast(LongLong Constant(ConstInt(0) Int) LongLong) LongLong None None None None false))\n  VarDecl(var_decl(\"p\" Cast(Pointer(Int) Constant(ConstInt(0) Int) Pointer(Int)) Pointer(Int) None None None None false))\n  VarDecl(var_decl(\"fp\" Cast(Pointer(FunType([] Void)) Constant(ConstInt(0) Int) Pointer(FunType([] Void))) Pointer(FunType([] Void)) None None None None false))\n  VarDecl(var_decl(\"f\" Constant(ConstFloat(0) Float) Float None None None None false))\n  VarDecl(var_decl(\"d\" Constant(ConstDouble(0) Double) Double None None None None false))\n  VarDecl(var_decl(\"b\" Cast(Bool Constant(ConstInt(0) Int) Bool) Bool None None None None false))\n  VarDecl(var_decl(\"w\" Cast(BitInt(12 true) Constant(ConstInt(0) Int) BitInt(12 true)) BitInt(12 true) None None None None false))\n  VarDecl(var_decl(\"uw\" Cast(BitInt(40 false) Constant(ConstInt(0) Int) BitInt(40 false)) BitInt(40 false) None None None None false))\n  VarDecl(var_decl(\"n\" Cast(NullptrT Constant(ConstInt(0) Int) NullptrT) NullptrT None None None None false))\n  VarDecl(var_decl(\"cv\" Constant(ConstInt(0) Int) Const(Volatile(Int)) None None None None false))\n])\n"}, {"object": "passes.type_checking.SymbolTable", "fields": {"_table": {"dict": [["i", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Int\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Initial", "fields": {"value": 0}}, "is_global": true, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}], ["ul", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "ULong\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Initial", "fields": {"value": 0}}, "is_global": true, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}], ["c", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Char\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Initial", "fields": {"value": 0}}, "is_global": true, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}], ["sc", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "SChar\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Initial", "fields": {"value": 0}}, "is_global": true, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}], ["ll", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "LongLong\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Initial", "fields": {"value": 0}}, "is_global": true, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}], ["p", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Pointer(Int)\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Initial", "fields": {"value": 0}}, "is_global": true, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}], ["fp", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Pointer(FunType([] Void))\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Initial", "fields": {"value": 0}}, "is_global": true, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}], ["f", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Float\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Initial", "fields": {"value": 0}}, "is_global": true, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}], ["d", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Double\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Initial", "fields": {"value": 0}}, "is_global": true, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}], ["b", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Bool\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Initial", "fields": {"value": 0}}, "is_global": true, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}], ["w", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "BitInt(12 true)\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Initial", "fields": {"value": 0}}, "is_global": true, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}], ["uw", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "BitInt(40 false)\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Initial", "fields": {"value": 0}}, "is_global": true, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}], ["n", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "NullptrT\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Initial", "fields": {"value": 0}}, "is_global": true, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}], ["cv", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Const(Volatile(Int))\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Initial", "fields": {"value": 0}}, "is_global": true, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}]]}}}, {"object": "passes.type_checking.TypeTable", "fields": {"_table": {"dict": []}}}]})~~"}),
     text({R"~~({"input": {"asdl": "c99_ast", "dump": "Program([\n  VarDecl(var_decl(\"i\" Constant(ConstInt(0) Int) Int None None None None false))\n  VarDecl(var_decl(\"ul\" Cast(ULong Constant(ConstInt(0) Int) ULong) ULong None None None None false))\n  VarDecl(var_decl(\"c\" Cast(Char Constant(ConstInt(0) Int) Char) Char None None None None false))\n  VarDecl(var_decl(\"sc\" Cast(SChar Constant(ConstInt(0) Int) SChar) SChar None None None None false))\n  VarDecl(var_decl(\"ll\" Cast(LongLong Constant(ConstInt(0) Int) LongLong) LongLong None None None None false))\n  VarDecl(var_decl(\"p\" Cast(Pointer(Int) Constant(ConstInt(0) Int) Pointer(Int)) Pointer(Int) None None None None false))\n  VarDecl(var_decl(\"fp\" Cast(Pointer(FunType([] Void)) Constant(ConstInt(0) Int) Pointer(FunType([] Void))) Pointer(FunType([] Void)) None None None None false))\n  VarDecl(var_decl(\"f\" Constant(ConstFloat(0) Float) Float None None None None false))\n  VarDecl(var_decl(\"d\" Constant(ConstDouble(0) Double) Double None None None None false))\n  VarDecl(var_decl(\"b\" Cast(Bool Constant(ConstInt(0) Int) Bool) Bool None None None None false))\n  VarDecl(var_decl(\"w\" Cast(BitInt(12 true) Constant(ConstInt(0) Int) BitInt(12 true)) BitInt(12 true) None None None None false))\n  VarDecl(var_decl(\"uw\" Cast(BitInt(40 false) Constant(ConstInt(0) Int) BitInt(40 false)) BitInt(40 false) None None None None false))\n  VarDecl(var_decl(\"n\" Cast(NullptrT Constant(ConstInt(0) Int) NullptrT) NullptrT None None None None false))\n  VarDecl(var_decl(\"cv\" Constant(ConstInt(0) Int) Const(Volatile(Int)) None None None None false))\n])\n"}, "args": {}, "kwargs": {}})~~"})},
    {text({R"~~(Program([
  VarDecl(var_decl("a" Unary(Negate Constant(ConstInt(5) None) None) Int None None None None false))
  VarDecl(var_decl("b" Binary(LeftShift Constant(ConstLong(1) None) Constant(ConstInt(20) None) None) Long None None None None false))
  VarDecl(var_decl("c" Unary(Complement Constant(ConstUInt(0) None) None) UInt None None None None false))
  VarDecl(var_decl("d" Unary(Negate Constant(ConstInt(1) None) None) Char None None None None false))
  VarDecl(var_decl("f" InitList([Unary(Negate Constant(ConstInt(1) None) None) Binary(Multiply Constant(ConstInt(2) None) Constant(ConstInt(3) None) None) Unary(Negate Constant(ConstInt(4) None) None)] None None) Array(Int 3) None None None None false))
  VarDecl(var_decl("g" Binary(Add SizeOfType(Long None) Conditional(Binary(GreaterThan Constant(ConstInt(3) None) Constant(ConstInt(2) None) None) Constant(ConstInt(10) None) Constant(ConstInt(20) None) None) None) Int None None None None false))
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("@0.s" Unary(Negate Constant(ConstInt(300) None) None) Int Static None None None false)))
    S(Return(Binary(Add Binary(Add Binary(Add Binary(Add Binary(Add Binary(Add Binary(Equal Var("a" None) Unary(Negate Constant(ConstInt(5) None) None) None) Binary(Multiply Binary(Equal Var("b" None) Constant(ConstLong(1048576) None) None) Constant(ConstInt(2) None) None) None) Binary(Multiply Binary(Equal Var("c" None) Constant(ConstUInt(65535) None) None) Constant(ConstInt(4) None) None) None) Binary(Multiply Binary(Equal Var("d" None) Constant(ConstInt(255) None) None) Constant(ConstInt(8) None) None) None) Binary(Multiply Binary(Equal Binary(Add Binary(Add Subscript(Var("f" None) Constant(ConstInt(0) None) None) Subscript(Var("f" None) Constant(ConstInt(1) None) None) None) Subscript(Var("f" None) Constant(ConstInt(2) None) None) None) Constant(ConstInt(1) None) None) Constant(ConstInt(16) None) None) None) Binary(Multiply Binary(Equal Var("g" None) Constant(ConstInt(14) None) None) Constant(ConstInt(32) None) None) None) Binary(Multiply Binary(Equal Var("@0.s" None) Unary(Negate Constant(ConstInt(300) None) None) None) Constant(ConstInt(64) None) None) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"}),
     false,
     text({R"~~({"tuple": [{"asdl": "c99_ast", "dump": "Program([\n  VarDecl(var_decl(\"a\" Unary(Negate Constant(ConstInt(5) Int) Int) Int None None None None false))\n  VarDecl(var_decl(\"b\" Binary(LeftShift Constant(ConstLong(1) Long) Constant(ConstInt(20) Int) Long) Long None None None None false))\n  VarDecl(var_decl(\"c\" Unary(Complement Constant(ConstUInt(0) UInt) UInt) UInt None None None None false))\n  VarDecl(var_decl(\"d\" Cast(Char Unary(Negate Constant(ConstInt(1) Int) Int) Char) Char None None None None false))\n  VarDecl(var_decl(\"f\" InitList([Unary(Negate Constant(ConstInt(1) Int) Int) Binary(Multiply Constant(ConstInt(2) Int) Constant(ConstInt(3) Int) Int) Unary(Negate Constant(ConstInt(4) Int) Int)] Array(Int 3) None) Array(Int 3) None None None None false))\n  VarDecl(var_decl(\"g\" Cast(Int Binary(Add SizeOfType(Long UInt) Cast(UInt Conditional(Binary(GreaterThan Constant(ConstInt(3) Int) Constant(ConstInt(2) Int) Int) Constant(ConstInt(10) Int) Constant(ConstInt(20) Int) Int) UInt) UInt) Int) Int None None None None false))\n  FunctionDecl(function_decl(\"main\" [] Block([\n    D(VarDecl(var_decl(\"@0.s\" Unary(Negate Constant(ConstInt(300) Int) Int) Int Static None None None false)))\n    S(Return(Binary(Add Binary(Add Binary(Add Binary(Add Binary(Add Binary(Add Binary(Equal Var(\"a\" Int) Unary(Negate Constant(ConstInt(5) Int) Int) Int) Binary(Multiply Binary(Equal Var(\"b\" Long) Constant(ConstLong(1048576) Long) Int) Constant(ConstInt(2) Int) Int) Int) Binary(Multiply Binary(Equal Var(\"c\" UInt) Constant(ConstUInt(65535) UInt) Int) Constant(ConstInt(4) Int) Int) Int) Binary(Multiply Binary(Equal Cast(Int Var(\"d\" Char) Int) Constant(ConstInt(255) Int) Int) Constant(ConstInt(8) Int) Int) Int) Binary(Multiply Binary(Equal Binary(Add Binary(Add Subscript(AddressOf(Var(\"f\" Array(Int 3)) Pointer(Int)) Constant(ConstInt(0) Int) Int) Subscript(AddressOf(Var(\"f\" Array(Int 3)) Pointer(Int)) Constant(ConstInt(1) Int) Int) Int) Subscript(AddressOf(Var(\"f\" Array(Int 3)) Pointer(Int)) Constant(ConstInt(2) Int) Int) Int) Constant(ConstInt(1) Int) Int) Constant(ConstInt(16) Int) Int) Int) Binary(Multiply Binary(Equal Var(\"g\" Int) Constant(ConstInt(14) Int) Int) Constant(ConstInt(32) Int) Int) Int) Binary(Multiply Binary(Equal Var(\"@0.s\" Int) Unary(Negate Constant(ConstInt(300) Int) Int) Int) Constant(ConstInt(64) Int) Int) Int)))\n  ]) FunType([] Int) None None [] false None [] None false))\n])\n"}, {"object": "passes.type_checking.SymbolTable", "fields": {"_table": {"dict": [["a", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Int\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Initial", "fields": {"value": -5}}, "is_global": true, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}], ["b", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Long\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Initial", "fields": {"value": 1048576}}, "is_global": true, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}], ["c", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "UInt\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Initial", "fields": {"value": 65535}}, "is_global": true, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}], ["d", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Char\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Initial", "fields": {"value": 255}}, "is_global": true, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}], ["f", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Array(Int 3)\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Initial", "fields": {"value": {"tuple": [-1, 6, -4]}}}, "is_global": true, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}], ["g", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Int\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Initial", "fields": {"value": 14}}, "is_global": true, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}], ["main", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "FunType([] Int)\n"}, "attrs": {"object": "passes.type_checking.FunAttr", "fields": {"defined": true, "is_global": true, "carry_return": false, "address": null, "preserves": {"set": []}, "section": null, "param_zeropage": {"tuple": []}, "returns_bool": false, "noreturn": false}}}}], ["@0.s", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Int\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Initial", "fields": {"value": -300}}, "is_global": false, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}]]}}}, {"object": "passes.type_checking.TypeTable", "fields": {"_table": {"dict": []}}}]})~~"}),
     text({R"~~({"input": {"asdl": "c99_ast", "dump": "Program([\n  VarDecl(var_decl(\"a\" Unary(Negate Constant(ConstInt(5) Int) Int) Int None None None None false))\n  VarDecl(var_decl(\"b\" Binary(LeftShift Constant(ConstLong(1) Long) Constant(ConstInt(20) Int) Long) Long None None None None false))\n  VarDecl(var_decl(\"c\" Unary(Complement Constant(ConstUInt(0) UInt) UInt) UInt None None None None false))\n  VarDecl(var_decl(\"d\" Cast(Char Unary(Negate Constant(ConstInt(1) Int) Int) Char) Char None None None None false))\n  VarDecl(var_decl(\"f\" InitList([Unary(Negate Constant(ConstInt(1) Int) Int) Binary(Multiply Constant(ConstInt(2) Int) Constant(ConstInt(3) Int) Int) Unary(Negate Constant(ConstInt(4) Int) Int)] Array(Int 3) None) Array(Int 3) None None None None false))\n  VarDecl(var_decl(\"g\" Cast(Int Binary(Add SizeOfType(Long UInt) Cast(UInt Conditional(Binary(GreaterThan Constant(ConstInt(3) Int) Constant(ConstInt(2) Int) Int) Constant(ConstInt(10) Int) Constant(ConstInt(20) Int) Int) UInt) UInt) Int) Int None None None None false))\n  FunctionDecl(function_decl(\"main\" [] Block([\n    D(VarDecl(var_decl(\"@0.s\" Unary(Negate Constant(ConstInt(300) Int) Int) Int Static None None None false)))\n    S(Return(Binary(Add Binary(Add Binary(Add Binary(Add Binary(Add Binary(Add Binary(Equal Var(\"a\" Int) Unary(Negate Constant(ConstInt(5) Int) Int) Int) Binary(Multiply Binary(Equal Var(\"b\" Long) Constant(ConstLong(1048576) Long) Int) Constant(ConstInt(2) Int) Int) Int) Binary(Multiply Binary(Equal Var(\"c\" UInt) Constant(ConstUInt(65535) UInt) Int) Constant(ConstInt(4) Int) Int) Int) Binary(Multiply Binary(Equal Cast(Int Var(\"d\" Char) Int) Constant(ConstInt(255) Int) Int) Constant(ConstInt(8) Int) Int) Int) Binary(Multiply Binary(Equal Binary(Add Binary(Add Subscript(AddressOf(Var(\"f\" Array(Int 3)) Pointer(Int)) Constant(ConstInt(0) Int) Int) Subscript(AddressOf(Var(\"f\" Array(Int 3)) Pointer(Int)) Constant(ConstInt(1) Int) Int) Int) Subscript(AddressOf(Var(\"f\" Array(Int 3)) Pointer(Int)) Constant(ConstInt(2) Int) Int) Int) Constant(ConstInt(1) Int) Int) Constant(ConstInt(16) Int) Int) Int) Binary(Multiply Binary(Equal Var(\"g\" Int) Constant(ConstInt(14) Int) Int) Constant(ConstInt(32) Int) Int) Int) Binary(Multiply Binary(Equal Var(\"@0.s\" Int) Unary(Negate Constant(ConstInt(300) Int) Int) Int) Constant(ConstInt(64) Int) Int) Int)))\n  ]) FunType([] Int) None None [] false None [] None false))\n])\n"}, "args": {}, "kwargs": {}})~~"})},
    {text({R"~~(Program([
  FunctionDecl(function_decl("stdc_leading_zeros_uc" ["@0.__value"] None FunType([UChar] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_leading_zeros_us" ["@1.__value"] None FunType([UShort] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_leading_zeros_ui" ["@2.__value"] None FunType([UInt] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_leading_zeros_ul" ["@3.__value"] None FunType([ULong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_leading_zeros_ull" ["@4.__value"] None FunType([ULongLong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_leading_ones_uc" ["@5.__value"] None FunType([UChar] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_leading_ones_us" ["@6.__value"] None FunType([UShort] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_leading_ones_ui" ["@7.__value"] None FunType([UInt] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_leading_ones_ul" ["@8.__value"] None FunType([ULong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_leading_ones_ull" ["@9.__value"] None FunType([ULongLong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_trailing_zeros_uc" ["@10.__value"] None FunType([UChar] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_trailing_zeros_us" ["@11.__value"] None FunType([UShort] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_trailing_zeros_ui" ["@12.__value"] None FunType([UInt] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_trailing_zeros_ul" ["@13.__value"] None FunType([ULong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_trailing_zeros_ull" ["@14.__value"] None FunType([ULongLong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_trailing_ones_uc" ["@15.__value"] None FunType([UChar] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_trailing_ones_us" ["@16.__value"] None FunType([UShort] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_trailing_ones_ui" ["@17.__value"] None FunType([UInt] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_trailing_ones_ul" ["@18.__value"] None FunType([ULong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_trailing_ones_ull" ["@19.__value"] None FunType([ULongLong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_leading_zero_uc" ["@20.__value"] None FunType([UChar] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_leading_zero_us" ["@21.__value"] None FunType([UShort] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_leading_zero_ui" ["@22.__value"] None FunType([UInt] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_leading_zero_ul" ["@23.__value"] None FunType([ULong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_leading_zero_ull" ["@24.__value"] None FunType([ULongLong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_leading_one_uc" ["@25.__value"] None FunType([UChar] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_leading_one_us" ["@26.__value"] None FunType([UShort] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_leading_one_ui" ["@27.__value"] None FunType([UInt] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_leading_one_ul" ["@28.__value"] None FunType([ULong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_leading_one_ull" ["@29.__value"] None FunType([ULongLong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_trailing_zero_uc" ["@30.__value"] None FunType([UChar] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_trailing_zero_us" ["@31.__value"] None FunType([UShort] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_trailing_zero_ui" ["@32.__value"] None FunType([UInt] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_trailing_zero_ul" ["@33.__value"] None FunType([ULong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_trailing_zero_ull" ["@34.__value"] None FunType([ULongLong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_trailing_one_uc" ["@35.__value"] None FunType([UChar] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_trailing_one_us" ["@36.__value"] None FunType([UShort] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_trailing_one_ui" ["@37.__value"] None FunType([UInt] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_trailing_one_ul" ["@38.__value"] None FunType([ULong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_trailing_one_ull" ["@39.__value"] None FunType([ULongLong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_count_zeros_uc" ["@40.__value"] None FunType([UChar] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_count_zeros_us" ["@41.__value"] None FunType([UShort] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_count_zeros_ui" ["@42.__value"] None FunType([UInt] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_count_zeros_ul" ["@43.__value"] None FunType([ULong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_count_zeros_ull" ["@44.__value"] None FunType([ULongLong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_count_ones_uc" ["@45.__value"] None FunType([UChar] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_count_ones_us" ["@46.__value"] None FunType([UShort] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_count_ones_ui" ["@47.__value"] None FunType([UInt] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_count_ones_ul" ["@48.__value"] None FunType([ULong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_count_ones_ull" ["@49.__value"] None FunType([ULongLong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_has_single_bit_uc" ["@50.__value"] None FunType([UChar] Bool) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_has_single_bit_us" ["@51.__value"] None FunType([UShort] Bool) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_has_single_bit_ui" ["@52.__value"] None FunType([UInt] Bool) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_has_single_bit_ul" ["@53.__value"] None FunType([ULong] Bool) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_has_single_bit_ull" ["@54.__value"] None FunType([ULongLong] Bool) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_bit_width_uc" ["@55.__value"] None FunType([UChar] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_bit_width_us" ["@56.__value"] None FunType([UShort] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_bit_width_ui" ["@57.__value"] None FunType([UInt] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_bit_width_ul" ["@58.__value"] None FunType([ULong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_bit_width_ull" ["@59.__value"] None FunType([ULongLong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_bit_floor_uc" ["@60.__value"] None FunType([UChar] UChar) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_bit_floor_us" ["@61.__value"] None FunType([UShort] UShort) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_bit_floor_ui" ["@62.__value"] None FunType([UInt] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_bit_floor_ul" ["@63.__value"] None FunType([ULong] ULong) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_bit_floor_ull" ["@64.__value"] None FunType([ULongLong] ULongLong) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_bit_ceil_uc" ["@65.__value"] None FunType([UChar] UChar) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_bit_ceil_us" ["@66.__value"] None FunType([UShort] UShort) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_bit_ceil_ui" ["@67.__value"] None FunType([UInt] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_bit_ceil_ul" ["@68.__value"] None FunType([ULong] ULong) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_bit_ceil_ull" ["@69.__value"] None FunType([ULongLong] ULongLong) None None [""] false None [] None false))
  VarDecl(var_decl("x" None BitInt(64 false) None None None None false))
  FunctionDecl(function_decl("probe" [] Block([
    S(Expression(FunctionCall("__c6502_stdc_count_ones" [Var("x" None)] None false)))
  ]) FunType([] Void) None None [] false None [] None false))
])
)~~"}),
     false,
     text({R"~~({"tuple": [{"asdl": "c99_ast", "dump": "Program([\n  VarDecl(var_decl(\"x\" None BitInt(64 false) None None None None false))\n  FunctionDecl(function_decl(\"probe\" [] Block([\n    S(Expression(FunctionCall(\"__c6502_stdc_count_ones\" [Var(\"x\" BitInt(64 false))] UInt false)))\n  ]) FunType([] Void) None None [] false None [] None false))\n])\n"}, {"object": "passes.type_checking.SymbolTable", "fields": {"_table": {"dict": [["x", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "BitInt(64 false)\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Tentative", "fields": {}}, "is_global": true, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}], ["probe", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "FunType([] Void)\n"}, "attrs": {"object": "passes.type_checking.FunAttr", "fields": {"defined": true, "is_global": true, "carry_return": false, "address": null, "preserves": {"set": []}, "section": null, "param_zeropage": {"tuple": []}, "returns_bool": false, "noreturn": false}}}}]]}}}, {"object": "passes.type_checking.TypeTable", "fields": {"_table": {"dict": []}}}]})~~"}),
     text({R"~~({"input": {"asdl": "c99_ast", "dump": "Program([\n  VarDecl(var_decl(\"x\" None BitInt(64 false) None None None None false))\n  FunctionDecl(function_decl(\"probe\" [] Block([\n    S(Expression(FunctionCall(\"__c6502_stdc_count_ones\" [Var(\"x\" BitInt(64 false))] UInt false)))\n  ]) FunType([] Void) None None [] false None [] None false))\n])\n"}, "args": {}, "kwargs": {}})~~"})},
    {text({R"~~(Program([
  FunctionDecl(function_decl("stdc_leading_zeros_uc" ["@0.__value"] None FunType([UChar] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_leading_zeros_us" ["@1.__value"] None FunType([UShort] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_leading_zeros_ui" ["@2.__value"] None FunType([UInt] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_leading_zeros_ul" ["@3.__value"] None FunType([ULong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_leading_zeros_ull" ["@4.__value"] None FunType([ULongLong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_leading_ones_uc" ["@5.__value"] None FunType([UChar] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_leading_ones_us" ["@6.__value"] None FunType([UShort] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_leading_ones_ui" ["@7.__value"] None FunType([UInt] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_leading_ones_ul" ["@8.__value"] None FunType([ULong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_leading_ones_ull" ["@9.__value"] None FunType([ULongLong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_trailing_zeros_uc" ["@10.__value"] None FunType([UChar] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_trailing_zeros_us" ["@11.__value"] None FunType([UShort] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_trailing_zeros_ui" ["@12.__value"] None FunType([UInt] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_trailing_zeros_ul" ["@13.__value"] None FunType([ULong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_trailing_zeros_ull" ["@14.__value"] None FunType([ULongLong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_trailing_ones_uc" ["@15.__value"] None FunType([UChar] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_trailing_ones_us" ["@16.__value"] None FunType([UShort] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_trailing_ones_ui" ["@17.__value"] None FunType([UInt] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_trailing_ones_ul" ["@18.__value"] None FunType([ULong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_trailing_ones_ull" ["@19.__value"] None FunType([ULongLong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_leading_zero_uc" ["@20.__value"] None FunType([UChar] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_leading_zero_us" ["@21.__value"] None FunType([UShort] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_leading_zero_ui" ["@22.__value"] None FunType([UInt] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_leading_zero_ul" ["@23.__value"] None FunType([ULong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_leading_zero_ull" ["@24.__value"] None FunType([ULongLong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_leading_one_uc" ["@25.__value"] None FunType([UChar] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_leading_one_us" ["@26.__value"] None FunType([UShort] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_leading_one_ui" ["@27.__value"] None FunType([UInt] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_leading_one_ul" ["@28.__value"] None FunType([ULong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_leading_one_ull" ["@29.__value"] None FunType([ULongLong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_trailing_zero_uc" ["@30.__value"] None FunType([UChar] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_trailing_zero_us" ["@31.__value"] None FunType([UShort] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_trailing_zero_ui" ["@32.__value"] None FunType([UInt] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_trailing_zero_ul" ["@33.__value"] None FunType([ULong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_trailing_zero_ull" ["@34.__value"] None FunType([ULongLong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_trailing_one_uc" ["@35.__value"] None FunType([UChar] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_trailing_one_us" ["@36.__value"] None FunType([UShort] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_trailing_one_ui" ["@37.__value"] None FunType([UInt] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_trailing_one_ul" ["@38.__value"] None FunType([ULong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_trailing_one_ull" ["@39.__value"] None FunType([ULongLong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_count_zeros_uc" ["@40.__value"] None FunType([UChar] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_count_zeros_us" ["@41.__value"] None FunType([UShort] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_count_zeros_ui" ["@42.__value"] None FunType([UInt] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_count_zeros_ul" ["@43.__value"] None FunType([ULong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_count_zeros_ull" ["@44.__value"] None FunType([ULongLong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_count_ones_uc" ["@45.__value"] None FunType([UChar] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_count_ones_us" ["@46.__value"] None FunType([UShort] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_count_ones_ui" ["@47.__value"] None FunType([UInt] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_count_ones_ul" ["@48.__value"] None FunType([ULong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_count_ones_ull" ["@49.__value"] None FunType([ULongLong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_has_single_bit_uc" ["@50.__value"] None FunType([UChar] Bool) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_has_single_bit_us" ["@51.__value"] None FunType([UShort] Bool) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_has_single_bit_ui" ["@52.__value"] None FunType([UInt] Bool) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_has_single_bit_ul" ["@53.__value"] None FunType([ULong] Bool) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_has_single_bit_ull" ["@54.__value"] None FunType([ULongLong] Bool) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_bit_width_uc" ["@55.__value"] None FunType([UChar] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_bit_width_us" ["@56.__value"] None FunType([UShort] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_bit_width_ui" ["@57.__value"] None FunType([UInt] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_bit_width_ul" ["@58.__value"] None FunType([ULong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_bit_width_ull" ["@59.__value"] None FunType([ULongLong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_bit_floor_uc" ["@60.__value"] None FunType([UChar] UChar) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_bit_floor_us" ["@61.__value"] None FunType([UShort] UShort) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_bit_floor_ui" ["@62.__value"] None FunType([UInt] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_bit_floor_ul" ["@63.__value"] None FunType([ULong] ULong) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_bit_floor_ull" ["@64.__value"] None FunType([ULongLong] ULongLong) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_bit_ceil_uc" ["@65.__value"] None FunType([UChar] UChar) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_bit_ceil_us" ["@66.__value"] None FunType([UShort] UShort) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_bit_ceil_ui" ["@67.__value"] None FunType([UInt] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_bit_ceil_ul" ["@68.__value"] None FunType([ULong] ULong) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_bit_ceil_ull" ["@69.__value"] None FunType([ULongLong] ULongLong) None None [""] false None [] None false))
  VarDecl(var_decl("x" None BitInt(16 false) None None None None false))
  FunctionDecl(function_decl("probe" [] Block([
    S(Expression(FunctionCall("__c6502_stdc_bit_floor" [Var("x" None)] None false)))
  ]) FunType([] Void) None None [] false None [] None false))
])
)~~"}),
     false,
     text({R"~~({"tuple": [{"asdl": "c99_ast", "dump": "Program([\n  VarDecl(var_decl(\"x\" None BitInt(16 false) None None None None false))\n  FunctionDecl(function_decl(\"probe\" [] Block([\n    S(Expression(FunctionCall(\"__c6502_stdc_bit_floor\" [Var(\"x\" BitInt(16 false))] BitInt(16 false) false)))\n  ]) FunType([] Void) None None [] false None [] None false))\n])\n"}, {"object": "passes.type_checking.SymbolTable", "fields": {"_table": {"dict": [["x", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "BitInt(16 false)\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Tentative", "fields": {}}, "is_global": true, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}], ["probe", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "FunType([] Void)\n"}, "attrs": {"object": "passes.type_checking.FunAttr", "fields": {"defined": true, "is_global": true, "carry_return": false, "address": null, "preserves": {"set": []}, "section": null, "param_zeropage": {"tuple": []}, "returns_bool": false, "noreturn": false}}}}]]}}}, {"object": "passes.type_checking.TypeTable", "fields": {"_table": {"dict": []}}}]})~~"}),
     text({R"~~({"input": {"asdl": "c99_ast", "dump": "Program([\n  VarDecl(var_decl(\"x\" None BitInt(16 false) None None None None false))\n  FunctionDecl(function_decl(\"probe\" [] Block([\n    S(Expression(FunctionCall(\"__c6502_stdc_bit_floor\" [Var(\"x\" BitInt(16 false))] BitInt(16 false) false)))\n  ]) FunType([] Void) None None [] false None [] None false))\n])\n"}, "args": {}, "kwargs": {}})~~"})},
    {text({R"~~(Program([
  StructDecl(struct_decl("in" false [member_decl("a" Char) member_decl("b" Int)]))
  StructDecl(struct_decl("s" false [member_decl("x" Char) member_decl("in" Structure("in")) member_decl("y" Long)]))
  VarDecl(var_decl("S" InitList([Constant(ConstInt(3) None) InitList([Constant(ConstInt(4) None) Unary(Negate Constant(ConstInt(300) None) None)] None None) Constant(ConstLong(70000) None)] None None) Const(Structure("s")) None None None None true))
  VarDecl(var_decl("K" Constant(ConstInt(47) None) Const(Int) None None None None true))
  VarDecl(var_decl("b" Binary(Add Var("K" None) Constant(ConstInt(1) None) None) Int Static None None None false))
  VarDecl(var_decl("c" Binary(Add Dot(Var("S" None) "y" None) Dot(Dot(Var("S" None) "in" None) "b" None) None) Long Static None None None false))
  VarDecl(var_decl("S3" Var("S" None) Structure("s") Static None None None false))
  VarDecl(var_decl("D" Constant(ConstDouble(4612811918334230528) None) Const(Double) None None None None true))
  VarDecl(var_decl("e" Cast(Int Var("D" None) None) Int Static None None None false))
  VarDecl(var_decl("third" Binary(Divide Constant(ConstDouble(4607182418800017408) None) Constant(ConstDouble(4613937818241073152) None) None) Const(Double) None None None None true))
  VarDecl(var_decl("i" Cast(Int Binary(Multiply Var("third" None) Constant(ConstDouble(4618441417868443648) None) None) None) Int Static None None None false))
  StructDecl(struct_decl("u" true [member_decl("w" UInt) member_decl("b" Array(UChar 2))]))
  VarDecl(var_decl("U" InitList([Constant(ConstInt(4660) None)] None None) Const(Union("u")) None None None None true))
  VarDecl(var_decl("d" Dot(Var("U" None) "w" None) UInt Static None None None false))
])
)~~"}),
     false,
     text({R"~~({"tuple": [{"asdl": "c99_ast", "dump": "Program([\n  StructDecl(struct_decl(\"in\" false [member_decl(\"a\" Char) member_decl(\"b\" Int)]))\n  StructDecl(struct_decl(\"s\" false [member_decl(\"x\" Char) member_decl(\"in\" Structure(\"in\")) member_decl(\"y\" Long)]))\n  VarDecl(var_decl(\"S\" InitList([Cast(Char Constant(ConstInt(3) Int) Char) InitList([Cast(Char Constant(ConstInt(4) Int) Char) Unary(Negate Constant(ConstInt(300) Int) Int)] Structure(\"in\") None) Constant(ConstLong(70000) Long)] Structure(\"s\") None) Const(Structure(\"s\")) Static None None None true))\n  VarDecl(var_decl(\"K\" Constant(ConstInt(47) Int) Const(Int) Static None None None true))\n  VarDecl(var_decl(\"b\" Binary(Add Var(\"K\" Const(Int)) Constant(ConstInt(1) Int) Int) Int Static None None None false))\n  VarDecl(var_decl(\"c\" Binary(Add Dot(Var(\"S\" Const(Structure(\"s\"))) \"y\" Const(Long)) Cast(Long Dot(Dot(Var(\"S\" Const(Structure(\"s\"))) \"in\" Const(Structure(\"in\"))) \"b\" Const(Int)) Long) Long) Long Static None None None false))\n  VarDecl(var_decl(\"S3\" Var(\"S\" Const(Structure(\"s\"))) Structure(\"s\") Static None None None false))\n  VarDecl(var_decl(\"D\" Constant(ConstDouble(4612811918334230528) Double) Const(Double) Static None None None true))\n  VarDecl(var_decl(\"e\" Cast(Int Var(\"D\" Const(Double)) Int) Int Static None None None false))\n  VarDecl(var_decl(\"third\" Binary(Divide Constant(ConstDouble(4607182418800017408) Double) Constant(ConstDouble(4613937818241073152) Double) Double) Const(Double) Static None None None true))\n  VarDecl(var_decl(\"i\" Cast(Int Binary(Multiply Var(\"third\" Const(Double)) Constant(ConstDouble(4618441417868443648) Double) Double) Int) Int Static None None None false))\n  StructDecl(struct_decl(\"u\" true [member_decl(\"w\" UInt) member_decl(\"b\" Array(UChar 2))]))\n  VarDecl(var_decl(\"U\" InitList([Cast(UInt Constant(ConstInt(4660) Int) UInt)] Union(\"u\") None) Const(Union(\"u\")) Static None None None true))\n  VarDecl(var_decl(\"d\" Dot(Var(\"U\" Const(Union(\"u\"))) \"w\" Const(UInt)) UInt Static None None None false))\n])\n"}, {"object": "passes.type_checking.SymbolTable", "fields": {"_table": {"dict": [["S", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Const(Structure(\"s\"))\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Initial", "fields": {"value": {"tuple": [3, {"tuple": [4, -300]}, 70000]}}}, "is_global": false, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}], ["K", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Const(Int)\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Initial", "fields": {"value": 47}}, "is_global": false, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}], ["b", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Int\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Initial", "fields": {"value": 48}}, "is_global": false, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}], ["c", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Long\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Initial", "fields": {"value": 69700}}, "is_global": false, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}], ["S3", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Structure(\"s\")\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Initial", "fields": {"value": {"tuple": [3, {"tuple": [4, -300]}, 70000]}}}, "is_global": false, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}], ["D", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Const(Double)\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Initial", "fields": {"value": 4612811918334230528}}, "is_global": false, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}], ["e", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Int\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Initial", "fields": {"value": 2}}, "is_global": false, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}], ["third", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Const(Double)\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Initial", "fields": {"value": 4599676419421066581}}, "is_global": false, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}], ["i", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Int\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Initial", "fields": {"value": 2}}, "is_global": false, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}], ["U", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Const(Union(\"u\"))\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Initial", "fields": {"value": {"tuple": [4660]}}}, "is_global": false, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}], ["d", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "UInt\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Initial", "fields": {"value": 4660}}, "is_global": false, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}]]}}}, {"object": "passes.type_checking.TypeTable", "fields": {"_table": {"dict": [["in", {"object": "passes.type_checking.StructLayout", "fields": {"tag": "in", "is_union": false, "members": [{"object": "passes.type_checking.MemberInfo", "fields": {"name": "a", "type": {"asdl": "c99_ast", "dump": "Char\n"}, "byte_offset": 0}}, {"object": "passes.type_checking.MemberInfo", "fields": {"name": "b", "type": {"asdl": "c99_ast", "dump": "Int\n"}, "byte_offset": 1}}], "size": 3, "complete": true}}], ["s", {"object": "passes.type_checking.StructLayout", "fields": {"tag": "s", "is_union": false, "members": [{"object": "passes.type_checking.MemberInfo", "fields": {"name": "x", "type": {"asdl": "c99_ast", "dump": "Char\n"}, "byte_offset": 0}}, {"object": "passes.type_checking.MemberInfo", "fields": {"name": "in", "type": {"asdl": "c99_ast", "dump": "Structure(\"in\")\n"}, "byte_offset": 1}}, {"object": "passes.type_checking.MemberInfo", "fields": {"name": "y", "type": {"asdl": "c99_ast", "dump": "Long\n"}, "byte_offset": 4}}], "size": 8, "complete": true}}], ["u", {"object": "passes.type_checking.StructLayout", "fields": {"tag": "u", "is_union": true, "members": [{"object": "passes.type_checking.MemberInfo", "fields": {"name": "w", "type": {"asdl": "c99_ast", "dump": "UInt\n"}, "byte_offset": 0}}, {"object": "passes.type_checking.MemberInfo", "fields": {"name": "b", "type": {"asdl": "c99_ast", "dump": "Array(UChar 2)\n"}, "byte_offset": 0}}], "size": 2, "complete": true}}]]}}}]})~~"}),
     text({R"~~({"input": {"asdl": "c99_ast", "dump": "Program([\n  StructDecl(struct_decl(\"in\" false [member_decl(\"a\" Char) member_decl(\"b\" Int)]))\n  StructDecl(struct_decl(\"s\" false [member_decl(\"x\" Char) member_decl(\"in\" Structure(\"in\")) member_decl(\"y\" Long)]))\n  VarDecl(var_decl(\"S\" InitList([Cast(Char Constant(ConstInt(3) Int) Char) InitList([Cast(Char Constant(ConstInt(4) Int) Char) Unary(Negate Constant(ConstInt(300) Int) Int)] Structure(\"in\") None) Constant(ConstLong(70000) Long)] Structure(\"s\") None) Const(Structure(\"s\")) Static None None None true))\n  VarDecl(var_decl(\"K\" Constant(ConstInt(47) Int) Const(Int) Static None None None true))\n  VarDecl(var_decl(\"b\" Binary(Add Var(\"K\" Const(Int)) Constant(ConstInt(1) Int) Int) Int Static None None None false))\n  VarDecl(var_decl(\"c\" Binary(Add Dot(Var(\"S\" Const(Structure(\"s\"))) \"y\" Const(Long)) Cast(Long Dot(Dot(Var(\"S\" Const(Structure(\"s\"))) \"in\" Const(Structure(\"in\"))) \"b\" Const(Int)) Long) Long) Long Static None None None false))\n  VarDecl(var_decl(\"S3\" Var(\"S\" Const(Structure(\"s\"))) Structure(\"s\") Static None None None false))\n  VarDecl(var_decl(\"D\" Constant(ConstDouble(4612811918334230528) Double) Const(Double) Static None None None true))\n  VarDecl(var_decl(\"e\" Cast(Int Var(\"D\" Const(Double)) Int) Int Static None None None false))\n  VarDecl(var_decl(\"third\" Binary(Divide Constant(ConstDouble(4607182418800017408) Double) Constant(ConstDouble(4613937818241073152) Double) Double) Const(Double) Static None None None true))\n  VarDecl(var_decl(\"i\" Cast(Int Binary(Multiply Var(\"third\" Const(Double)) Constant(ConstDouble(4618441417868443648) Double) Double) Int) Int Static None None None false))\n  StructDecl(struct_decl(\"u\" true [member_decl(\"w\" UInt) member_decl(\"b\" Array(UChar 2))]))\n  VarDecl(var_decl(\"U\" InitList([Cast(UInt Constant(ConstInt(4660) Int) UInt)] Union(\"u\") None) Const(Union(\"u\")) Static None None None true))\n  VarDecl(var_decl(\"d\" Dot(Var(\"U\" Const(Union(\"u\"))) \"w\" Const(UInt)) UInt Static None None None false))\n])\n"}, "args": {}, "kwargs": {}})~~"})},
    {text({R"~~(Program([
  FunctionDecl(function_decl("stdc_leading_zeros_uc" ["@0.__value"] None FunType([UChar] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_leading_zeros_us" ["@1.__value"] None FunType([UShort] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_leading_zeros_ui" ["@2.__value"] None FunType([UInt] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_leading_zeros_ul" ["@3.__value"] None FunType([ULong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_leading_zeros_ull" ["@4.__value"] None FunType([ULongLong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_leading_ones_uc" ["@5.__value"] None FunType([UChar] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_leading_ones_us" ["@6.__value"] None FunType([UShort] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_leading_ones_ui" ["@7.__value"] None FunType([UInt] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_leading_ones_ul" ["@8.__value"] None FunType([ULong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_leading_ones_ull" ["@9.__value"] None FunType([ULongLong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_trailing_zeros_uc" ["@10.__value"] None FunType([UChar] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_trailing_zeros_us" ["@11.__value"] None FunType([UShort] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_trailing_zeros_ui" ["@12.__value"] None FunType([UInt] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_trailing_zeros_ul" ["@13.__value"] None FunType([ULong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_trailing_zeros_ull" ["@14.__value"] None FunType([ULongLong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_trailing_ones_uc" ["@15.__value"] None FunType([UChar] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_trailing_ones_us" ["@16.__value"] None FunType([UShort] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_trailing_ones_ui" ["@17.__value"] None FunType([UInt] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_trailing_ones_ul" ["@18.__value"] None FunType([ULong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_trailing_ones_ull" ["@19.__value"] None FunType([ULongLong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_leading_zero_uc" ["@20.__value"] None FunType([UChar] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_leading_zero_us" ["@21.__value"] None FunType([UShort] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_leading_zero_ui" ["@22.__value"] None FunType([UInt] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_leading_zero_ul" ["@23.__value"] None FunType([ULong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_leading_zero_ull" ["@24.__value"] None FunType([ULongLong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_leading_one_uc" ["@25.__value"] None FunType([UChar] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_leading_one_us" ["@26.__value"] None FunType([UShort] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_leading_one_ui" ["@27.__value"] None FunType([UInt] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_leading_one_ul" ["@28.__value"] None FunType([ULong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_leading_one_ull" ["@29.__value"] None FunType([ULongLong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_trailing_zero_uc" ["@30.__value"] None FunType([UChar] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_trailing_zero_us" ["@31.__value"] None FunType([UShort] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_trailing_zero_ui" ["@32.__value"] None FunType([UInt] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_trailing_zero_ul" ["@33.__value"] None FunType([ULong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_trailing_zero_ull" ["@34.__value"] None FunType([ULongLong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_trailing_one_uc" ["@35.__value"] None FunType([UChar] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_trailing_one_us" ["@36.__value"] None FunType([UShort] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_trailing_one_ui" ["@37.__value"] None FunType([UInt] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_trailing_one_ul" ["@38.__value"] None FunType([ULong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_trailing_one_ull" ["@39.__value"] None FunType([ULongLong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_count_zeros_uc" ["@40.__value"] None FunType([UChar] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_count_zeros_us" ["@41.__value"] None FunType([UShort] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_count_zeros_ui" ["@42.__value"] None FunType([UInt] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_count_zeros_ul" ["@43.__value"] None FunType([ULong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_count_zeros_ull" ["@44.__value"] None FunType([ULongLong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_count_ones_uc" ["@45.__value"] None FunType([UChar] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_count_ones_us" ["@46.__value"] None FunType([UShort] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_count_ones_ui" ["@47.__value"] None FunType([UInt] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_count_ones_ul" ["@48.__value"] None FunType([ULong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_count_ones_ull" ["@49.__value"] None FunType([ULongLong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_has_single_bit_uc" ["@50.__value"] None FunType([UChar] Bool) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_has_single_bit_us" ["@51.__value"] None FunType([UShort] Bool) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_has_single_bit_ui" ["@52.__value"] None FunType([UInt] Bool) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_has_single_bit_ul" ["@53.__value"] None FunType([ULong] Bool) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_has_single_bit_ull" ["@54.__value"] None FunType([ULongLong] Bool) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_bit_width_uc" ["@55.__value"] None FunType([UChar] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_bit_width_us" ["@56.__value"] None FunType([UShort] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_bit_width_ui" ["@57.__value"] None FunType([UInt] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_bit_width_ul" ["@58.__value"] None FunType([ULong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_bit_width_ull" ["@59.__value"] None FunType([ULongLong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_bit_floor_uc" ["@60.__value"] None FunType([UChar] UChar) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_bit_floor_us" ["@61.__value"] None FunType([UShort] UShort) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_bit_floor_ui" ["@62.__value"] None FunType([UInt] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_bit_floor_ul" ["@63.__value"] None FunType([ULong] ULong) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_bit_floor_ull" ["@64.__value"] None FunType([ULongLong] ULongLong) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_bit_ceil_uc" ["@65.__value"] None FunType([UChar] UChar) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_bit_ceil_us" ["@66.__value"] None FunType([UShort] UShort) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_bit_ceil_ui" ["@67.__value"] None FunType([UInt] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_bit_ceil_ul" ["@68.__value"] None FunType([ULong] ULong) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_bit_ceil_ull" ["@69.__value"] None FunType([ULongLong] ULongLong) None None [""] false None [] None false))
  VarDecl(var_decl("g" None UInt None None None None false))
  VarDecl(var_decl("r" None UInt None None None None false))
  FunctionDecl(function_decl("f" [] Block([
    S(Expression(Assignment(Var("r" None) FunctionCall("__c6502_stdc_bit_ceil" [Var("g" None)] None false) None)))
  ]) FunType([] Void) None None [] false None [] None false))
])
)~~"}),
     false,
     text({R"~~({"tuple": [{"asdl": "c99_ast", "dump": "Program([\n  VarDecl(var_decl(\"g\" None UInt None None None None false))\n  VarDecl(var_decl(\"r\" None UInt None None None None false))\n  FunctionDecl(function_decl(\"f\" [] Block([\n    S(Expression(Assignment(Var(\"r\" UInt) FunctionCall(\"__c6502_stdc_bit_ceil\" [Var(\"g\" UInt)] UInt false) UInt)))\n  ]) FunType([] Void) None None [] false None [] None false))\n])\n"}, {"object": "passes.type_checking.SymbolTable", "fields": {"_table": {"dict": [["g", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "UInt\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Tentative", "fields": {}}, "is_global": true, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}], ["r", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "UInt\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Tentative", "fields": {}}, "is_global": true, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}], ["f", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "FunType([] Void)\n"}, "attrs": {"object": "passes.type_checking.FunAttr", "fields": {"defined": true, "is_global": true, "carry_return": false, "address": null, "preserves": {"set": []}, "section": null, "param_zeropage": {"tuple": []}, "returns_bool": false, "noreturn": false}}}}]]}}}, {"object": "passes.type_checking.TypeTable", "fields": {"_table": {"dict": []}}}]})~~"}),
     text({R"~~({"input": {"asdl": "c99_ast", "dump": "Program([\n  VarDecl(var_decl(\"g\" None UInt None None None None false))\n  VarDecl(var_decl(\"r\" None UInt None None None None false))\n  FunctionDecl(function_decl(\"f\" [] Block([\n    S(Expression(Assignment(Var(\"r\" UInt) FunctionCall(\"__c6502_stdc_bit_ceil\" [Var(\"g\" UInt)] UInt false) UInt)))\n  ]) FunType([] Void) None None [] false None [] None false))\n])\n"}, "args": {}, "kwargs": {}})~~"})},
    {text({R"~~(Program([
  FunctionDecl(function_decl("stdc_leading_zeros_uc" ["@0.__value"] None FunType([UChar] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_leading_zeros_us" ["@1.__value"] None FunType([UShort] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_leading_zeros_ui" ["@2.__value"] None FunType([UInt] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_leading_zeros_ul" ["@3.__value"] None FunType([ULong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_leading_zeros_ull" ["@4.__value"] None FunType([ULongLong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_leading_ones_uc" ["@5.__value"] None FunType([UChar] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_leading_ones_us" ["@6.__value"] None FunType([UShort] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_leading_ones_ui" ["@7.__value"] None FunType([UInt] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_leading_ones_ul" ["@8.__value"] None FunType([ULong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_leading_ones_ull" ["@9.__value"] None FunType([ULongLong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_trailing_zeros_uc" ["@10.__value"] None FunType([UChar] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_trailing_zeros_us" ["@11.__value"] None FunType([UShort] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_trailing_zeros_ui" ["@12.__value"] None FunType([UInt] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_trailing_zeros_ul" ["@13.__value"] None FunType([ULong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_trailing_zeros_ull" ["@14.__value"] None FunType([ULongLong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_trailing_ones_uc" ["@15.__value"] None FunType([UChar] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_trailing_ones_us" ["@16.__value"] None FunType([UShort] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_trailing_ones_ui" ["@17.__value"] None FunType([UInt] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_trailing_ones_ul" ["@18.__value"] None FunType([ULong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_trailing_ones_ull" ["@19.__value"] None FunType([ULongLong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_leading_zero_uc" ["@20.__value"] None FunType([UChar] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_leading_zero_us" ["@21.__value"] None FunType([UShort] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_leading_zero_ui" ["@22.__value"] None FunType([UInt] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_leading_zero_ul" ["@23.__value"] None FunType([ULong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_leading_zero_ull" ["@24.__value"] None FunType([ULongLong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_leading_one_uc" ["@25.__value"] None FunType([UChar] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_leading_one_us" ["@26.__value"] None FunType([UShort] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_leading_one_ui" ["@27.__value"] None FunType([UInt] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_leading_one_ul" ["@28.__value"] None FunType([ULong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_leading_one_ull" ["@29.__value"] None FunType([ULongLong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_trailing_zero_uc" ["@30.__value"] None FunType([UChar] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_trailing_zero_us" ["@31.__value"] None FunType([UShort] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_trailing_zero_ui" ["@32.__value"] None FunType([UInt] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_trailing_zero_ul" ["@33.__value"] None FunType([ULong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_trailing_zero_ull" ["@34.__value"] None FunType([ULongLong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_trailing_one_uc" ["@35.__value"] None FunType([UChar] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_trailing_one_us" ["@36.__value"] None FunType([UShort] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_trailing_one_ui" ["@37.__value"] None FunType([UInt] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_trailing_one_ul" ["@38.__value"] None FunType([ULong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_trailing_one_ull" ["@39.__value"] None FunType([ULongLong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_count_zeros_uc" ["@40.__value"] None FunType([UChar] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_count_zeros_us" ["@41.__value"] None FunType([UShort] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_count_zeros_ui" ["@42.__value"] None FunType([UInt] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_count_zeros_ul" ["@43.__value"] None FunType([ULong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_count_zeros_ull" ["@44.__value"] None FunType([ULongLong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_count_ones_uc" ["@45.__value"] None FunType([UChar] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_count_ones_us" ["@46.__value"] None FunType([UShort] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_count_ones_ui" ["@47.__value"] None FunType([UInt] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_count_ones_ul" ["@48.__value"] None FunType([ULong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_count_ones_ull" ["@49.__value"] None FunType([ULongLong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_has_single_bit_uc" ["@50.__value"] None FunType([UChar] Bool) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_has_single_bit_us" ["@51.__value"] None FunType([UShort] Bool) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_has_single_bit_ui" ["@52.__value"] None FunType([UInt] Bool) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_has_single_bit_ul" ["@53.__value"] None FunType([ULong] Bool) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_has_single_bit_ull" ["@54.__value"] None FunType([ULongLong] Bool) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_bit_width_uc" ["@55.__value"] None FunType([UChar] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_bit_width_us" ["@56.__value"] None FunType([UShort] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_bit_width_ui" ["@57.__value"] None FunType([UInt] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_bit_width_ul" ["@58.__value"] None FunType([ULong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_bit_width_ull" ["@59.__value"] None FunType([ULongLong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_bit_floor_uc" ["@60.__value"] None FunType([UChar] UChar) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_bit_floor_us" ["@61.__value"] None FunType([UShort] UShort) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_bit_floor_ui" ["@62.__value"] None FunType([UInt] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_bit_floor_ul" ["@63.__value"] None FunType([ULong] ULong) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_bit_floor_ull" ["@64.__value"] None FunType([ULongLong] ULongLong) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_bit_ceil_uc" ["@65.__value"] None FunType([UChar] UChar) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_bit_ceil_us" ["@66.__value"] None FunType([UShort] UShort) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_bit_ceil_ui" ["@67.__value"] None FunType([UInt] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_bit_ceil_ul" ["@68.__value"] None FunType([ULong] ULong) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_bit_ceil_ull" ["@69.__value"] None FunType([ULongLong] ULongLong) None None [""] false None [] None false))
  VarDecl(var_decl("g" None UInt None None None None false))
  VarDecl(var_decl("r" None Bool None None None None false))
  FunctionDecl(function_decl("f" [] Block([
    S(Expression(Assignment(Var("r" None) FunctionCall("__c6502_stdc_has_single_bit" [Var("g" None)] None false) None)))
  ]) FunType([] Void) None None [] false None [] None false))
])
)~~"}),
     false,
     text({R"~~({"tuple": [{"asdl": "c99_ast", "dump": "Program([\n  VarDecl(var_decl(\"g\" None UInt None None None None false))\n  VarDecl(var_decl(\"r\" None Bool None None None None false))\n  FunctionDecl(function_decl(\"f\" [] Block([\n    S(Expression(Assignment(Var(\"r\" Bool) FunctionCall(\"__c6502_stdc_has_single_bit\" [Var(\"g\" UInt)] Bool false) Bool)))\n  ]) FunType([] Void) None None [] false None [] None false))\n])\n"}, {"object": "passes.type_checking.SymbolTable", "fields": {"_table": {"dict": [["g", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "UInt\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Tentative", "fields": {}}, "is_global": true, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}], ["r", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Bool\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Tentative", "fields": {}}, "is_global": true, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}], ["f", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "FunType([] Void)\n"}, "attrs": {"object": "passes.type_checking.FunAttr", "fields": {"defined": true, "is_global": true, "carry_return": false, "address": null, "preserves": {"set": []}, "section": null, "param_zeropage": {"tuple": []}, "returns_bool": false, "noreturn": false}}}}]]}}}, {"object": "passes.type_checking.TypeTable", "fields": {"_table": {"dict": []}}}]})~~"}),
     text({R"~~({"input": {"asdl": "c99_ast", "dump": "Program([\n  VarDecl(var_decl(\"g\" None UInt None None None None false))\n  VarDecl(var_decl(\"r\" None Bool None None None None false))\n  FunctionDecl(function_decl(\"f\" [] Block([\n    S(Expression(Assignment(Var(\"r\" Bool) FunctionCall(\"__c6502_stdc_has_single_bit\" [Var(\"g\" UInt)] Bool false) Bool)))\n  ]) FunType([] Void) None None [] false None [] None false))\n])\n"}, "args": {}, "kwargs": {}})~~"})},
    {text({R"~~(Program([
  FunctionDecl(function_decl("stdc_leading_zeros_uc" ["@0.__value"] None FunType([UChar] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_leading_zeros_us" ["@1.__value"] None FunType([UShort] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_leading_zeros_ui" ["@2.__value"] None FunType([UInt] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_leading_zeros_ul" ["@3.__value"] None FunType([ULong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_leading_zeros_ull" ["@4.__value"] None FunType([ULongLong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_leading_ones_uc" ["@5.__value"] None FunType([UChar] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_leading_ones_us" ["@6.__value"] None FunType([UShort] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_leading_ones_ui" ["@7.__value"] None FunType([UInt] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_leading_ones_ul" ["@8.__value"] None FunType([ULong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_leading_ones_ull" ["@9.__value"] None FunType([ULongLong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_trailing_zeros_uc" ["@10.__value"] None FunType([UChar] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_trailing_zeros_us" ["@11.__value"] None FunType([UShort] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_trailing_zeros_ui" ["@12.__value"] None FunType([UInt] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_trailing_zeros_ul" ["@13.__value"] None FunType([ULong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_trailing_zeros_ull" ["@14.__value"] None FunType([ULongLong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_trailing_ones_uc" ["@15.__value"] None FunType([UChar] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_trailing_ones_us" ["@16.__value"] None FunType([UShort] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_trailing_ones_ui" ["@17.__value"] None FunType([UInt] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_trailing_ones_ul" ["@18.__value"] None FunType([ULong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_trailing_ones_ull" ["@19.__value"] None FunType([ULongLong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_leading_zero_uc" ["@20.__value"] None FunType([UChar] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_leading_zero_us" ["@21.__value"] None FunType([UShort] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_leading_zero_ui" ["@22.__value"] None FunType([UInt] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_leading_zero_ul" ["@23.__value"] None FunType([ULong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_leading_zero_ull" ["@24.__value"] None FunType([ULongLong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_leading_one_uc" ["@25.__value"] None FunType([UChar] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_leading_one_us" ["@26.__value"] None FunType([UShort] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_leading_one_ui" ["@27.__value"] None FunType([UInt] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_leading_one_ul" ["@28.__value"] None FunType([ULong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_leading_one_ull" ["@29.__value"] None FunType([ULongLong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_trailing_zero_uc" ["@30.__value"] None FunType([UChar] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_trailing_zero_us" ["@31.__value"] None FunType([UShort] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_trailing_zero_ui" ["@32.__value"] None FunType([UInt] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_trailing_zero_ul" ["@33.__value"] None FunType([ULong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_trailing_zero_ull" ["@34.__value"] None FunType([ULongLong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_trailing_one_uc" ["@35.__value"] None FunType([UChar] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_trailing_one_us" ["@36.__value"] None FunType([UShort] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_trailing_one_ui" ["@37.__value"] None FunType([UInt] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_trailing_one_ul" ["@38.__value"] None FunType([ULong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_first_trailing_one_ull" ["@39.__value"] None FunType([ULongLong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_count_zeros_uc" ["@40.__value"] None FunType([UChar] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_count_zeros_us" ["@41.__value"] None FunType([UShort] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_count_zeros_ui" ["@42.__value"] None FunType([UInt] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_count_zeros_ul" ["@43.__value"] None FunType([ULong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_count_zeros_ull" ["@44.__value"] None FunType([ULongLong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_count_ones_uc" ["@45.__value"] None FunType([UChar] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_count_ones_us" ["@46.__value"] None FunType([UShort] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_count_ones_ui" ["@47.__value"] None FunType([UInt] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_count_ones_ul" ["@48.__value"] None FunType([ULong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_count_ones_ull" ["@49.__value"] None FunType([ULongLong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_has_single_bit_uc" ["@50.__value"] None FunType([UChar] Bool) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_has_single_bit_us" ["@51.__value"] None FunType([UShort] Bool) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_has_single_bit_ui" ["@52.__value"] None FunType([UInt] Bool) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_has_single_bit_ul" ["@53.__value"] None FunType([ULong] Bool) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_has_single_bit_ull" ["@54.__value"] None FunType([ULongLong] Bool) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_bit_width_uc" ["@55.__value"] None FunType([UChar] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_bit_width_us" ["@56.__value"] None FunType([UShort] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_bit_width_ui" ["@57.__value"] None FunType([UInt] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_bit_width_ul" ["@58.__value"] None FunType([ULong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_bit_width_ull" ["@59.__value"] None FunType([ULongLong] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_bit_floor_uc" ["@60.__value"] None FunType([UChar] UChar) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_bit_floor_us" ["@61.__value"] None FunType([UShort] UShort) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_bit_floor_ui" ["@62.__value"] None FunType([UInt] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_bit_floor_ul" ["@63.__value"] None FunType([ULong] ULong) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_bit_floor_ull" ["@64.__value"] None FunType([ULongLong] ULongLong) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_bit_ceil_uc" ["@65.__value"] None FunType([UChar] UChar) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_bit_ceil_us" ["@66.__value"] None FunType([UShort] UShort) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_bit_ceil_ui" ["@67.__value"] None FunType([UInt] UInt) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_bit_ceil_ul" ["@68.__value"] None FunType([ULong] ULong) None None [""] false None [] None false))
  FunctionDecl(function_decl("stdc_bit_ceil_ull" ["@69.__value"] None FunType([ULongLong] ULongLong) None None [""] false None [] None false))
  StructDecl(struct_decl("rec" false [member_decl("tag" UChar) member_decl("x" BitInt(12 true)) member_decl("big" BitInt(24 false)) member_decl("small" BitInt(5 true))]))
  VarDecl(var_decl("recs" InitList([InitList([Constant(ConstInt(1) None) Unary(Negate Constant(ConstInt(5) None) None) Constant(ConstLong(11259375) None) Unary(Negate Constant(ConstInt(16) None) None)] None None) InitList([Constant(ConstInt(2) None) Constant(ConstInt(2047) None) Constant(ConstInt(7) None) Constant(ConstInt(15) None)] None None)] None None) Array(Structure("rec") 3) None None None None false))
  VarDecl(var_decl("table" InitList([Constant(ConstInt(1) None) Unary(Negate Constant(ConstInt(2) None) None) Constant(ConstBitInt(549755813887 40 true) None) Unary(Negate Constant(ConstBitInt(549755813888 41 true) None) None)] None None) Array(Const(BitInt(40 true)) 4) Static None None None false))
  VarDecl(var_decl("counter" Constant(ConstLong(16777214) None) BitInt(24 false) None None None None false))
  VarDecl(var_decl("small" Constant(ConstInt(100) None) BitInt(5 true) None None None None false))
  VarDecl(var_decl("results" None Array(Long 16) None None None None false))
  FunctionDecl(function_decl("add24" ["@70.a" "@71.b"] Block([
    S(Return(Binary(Add Var("@70.a" None) Var("@71.b" None) None)))
  ]) FunType([BitInt(24 false) BitInt(24 false)] BitInt(24 false)) None None ["" ""] false None [] None false))
  FunctionDecl(function_decl("twice" ["@72.p"] Block([
    S(Expression(CompoundAssignment(Multiply Dereference(Var("@72.p" None) None) Constant(ConstInt(2) None) None None)))
    S(Return(Dereference(Var("@72.p" None) None)))
  ]) FunType([Pointer(BitInt(12 true))] BitInt(12 true)) None None [""] false None [] None false))
  FunctionDecl(function_decl("pick" ["@73.i"] Block([
    S(Return(Subscript(Var("table" None) Binary(BitwiseAnd Var("@73.i" None) Constant(ConstInt(3) None) None) None)))
  ]) FunType([BitInt(3 true)] BitInt(40 true)) None None [""] false None [] None false))
  FunctionDecl(function_decl("classify" ["@74.v"] Block([
    S(SwitchStmt(Var("@74.v" None) Compound(Block([
      S(CaseStmt(Unary(Negate Constant(ConstInt(16) None) None) Return(Constant(ConstInt(1) None)) ".case@1"))
      S(CaseStmt)~~", R"~~((Constant(ConstInt(15) None) Return(Constant(ConstInt(2) None)) ".case@2"))
      S(CaseStmt(Constant(ConstInt(0) None) Return(Constant(ConstInt(3) None)) ".case@3"))
      S(DefaultStmt(Return(Constant(ConstInt(4) None)) ".default@4"))
    ])) ".switch@0" [switch_case(Unary(Negate Constant(ConstInt(16) None) None) ".case@1") switch_case(Constant(ConstInt(15) None) ".case@2") switch_case(Constant(ConstInt(0) None) ".case@3")] ".default@4" None))
  ]) FunType([BitInt(5 true)] Int) None None [""] false None [] None false))
  FunctionDecl(function_decl("more" ["@75.r"] None FunType([Pointer(Long)] Void) None None [""] false None [] None false))
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("@76.r" Var("results" None) Pointer(Long) None None None None false)))
    D(VarDecl(var_decl("@77.x" Dot(Subscript(Var("recs" None) Constant(ConstInt(0) None) None) "x" None) BitInt(12 true) None None None None false)))
    S(Expression(Assignment(Dereference(Postfix(Increment Var("@76.r" None) None) None) FunctionCall("add24" [Var("counter" None) Constant(ConstInt(3) None)] None false) None)))
    S(Expression(Postfix(Increment Var("counter" None) None)))
    S(Expression(Assignment(Dereference(Postfix(Increment Var("@76.r" None) None) None) Var("counter" None) None)))
    S(Expression(Assignment(Dereference(Postfix(Increment Var("@76.r" None) None) None) FunctionCall("twice" [AddressOf(Var("@77.x" None) None)] None false) None)))
    S(Expression(Assignment(Dereference(Postfix(Increment Var("@76.r" None) None) None) FunctionCall("twice" [AddressOf(Dot(Subscript(Var("recs" None) Constant(ConstInt(1) None) None) "x" None) None)] None false) None)))
    S(Expression(Assignment(Dereference(Postfix(Increment Var("@76.r" None) None) None) FunctionCall("pick" [Constant(ConstInt(2) None)] None false) None)))
    S(Expression(Assignment(Dereference(Postfix(Increment Var("@76.r" None) None) None) Cast(Long Binary(RightShift FunctionCall("pick" [Constant(ConstInt(3) None)] None false) Constant(ConstInt(8) None) None) None) None)))
    S(Expression(Assignment(Dereference(Postfix(Increment Var("@76.r" None) None) None) Binary(Add Binary(Multiply FunctionCall("classify" [Dot(Subscript(Var("recs" None) Constant(ConstInt(0) None) None) "small" None)] None false) Constant(ConstInt(10) None) None) FunctionCall("classify" [Dot(Subscript(Var("recs" None) Constant(ConstInt(1) None) None) "small" None)] None false) None) None)))
    S(Expression(Assignment(Dereference(Postfix(Increment Var("@76.r" None) None) None) Binary(Add Binary(Multiply FunctionCall("classify" [Var("small" None)] None false) Constant(ConstInt(10) None) None) FunctionCall("classify" [Binary(Subtract Var("small" None) Constant(ConstInt(4) None) None)] None false) None) None)))
    S(Expression(FunctionCall("more" [Var("@76.r" None)] None false)))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
  FunctionDecl(function_decl("more" ["@78.r"] Block([
    S(Expression(Assignment(Dereference(Postfix(Increment Var("@78.r" None) None) None) Binary(RightShift Dot(Subscript(Var("recs" None) Constant(ConstInt(0) None) None) "big" None) Constant(ConstInt(4) None) None) None)))
    S(Expression(Assignment(Dereference(Postfix(Increment Var("@78.r" None) None) None) Cast(BitInt(4 false) Binary(Add Dot(Subscript(Var("recs" None) Constant(ConstInt(0) None) None) "tag" None) Constant(ConstInt(20) None) None) None) None)))
    S(Expression(Assignment(Dereference(Postfix(Increment Var("@78.r" None) None) None) FunctionCall("__c6502_stdc_bit_width" [Cast(BitInt(16 false) Constant(ConstInt(1000) None) None)] None false) None)))
    S(Expression(Assignment(Dereference(Postfix(Increment Var("@78.r" None) None) None) SizeOfType(Structure("rec") None) None)))
    S(Expression(Assignment(Dereference(Postfix(Increment Var("@78.r" None) None) None) Binary(GreaterThan Cast(BitInt(24 false) Unary(Negate Constant(ConstInt(1) None) None) None) Constant(ConstInt(0) None) None) None)))
    S(Expression(Assignment(Dereference(Postfix(Increment Var("@78.r" None) None) None) Binary(LessThan Cast(BitInt(12 true) Unary(Negate Constant(ConstInt(1) None) None) None) Constant(ConstInt(0) None) None) None)))
    S(Expression(Assignment(Dereference(Postfix(Increment Var("@78.r" None) None) None) Cast(BitInt(20 true) Cast(Double Unary(Negate Constant(ConstDouble(4683220296470102016) None) None) None) None) None)))
    S(Expression(Assignment(Dereference(Postfix(Increment Var("@78.r" None) None) None) Cast(Long Cast(Float Cast(BitInt(24 false) Constant(ConstLong(1193046) None) None) None) None) None)))
  ]) FunType([Pointer(Long)] Void) None None [""] false None [] None false))
])
)~~"}),
     false,
     text({R"~~({"tuple": [{"asdl": "c99_ast", "dump": "Program([\n  StructDecl(struct_decl(\"rec\" false [member_decl(\"tag\" UChar) member_decl(\"x\" BitInt(12 true)) member_decl(\"big\" BitInt(24 false)) member_decl(\"small\" BitInt(5 true))]))\n  VarDecl(var_decl(\"recs\" InitList([InitList([Cast(UChar Constant(ConstInt(1) Int) UChar) Cast(BitInt(12 true) Unary(Negate Constant(ConstInt(5) Int) Int) BitInt(12 true)) Cast(BitInt(24 false) Constant(ConstLong(11259375) Long) BitInt(24 false)) Cast(BitInt(5 true) Unary(Negate Constant(ConstInt(16) Int) Int) BitInt(5 true))] Structure(\"rec\") None) InitList([Cast(UChar Constant(ConstInt(2) Int) UChar) Cast(BitInt(12 true) Constant(ConstInt(2047) Int) BitInt(12 true)) Cast(BitInt(24 false) Constant(ConstInt(7) Int) BitInt(24 false)) Cast(BitInt(5 true) Constant(ConstInt(15) Int) BitInt(5 true))] Structure(\"rec\") None)] Array(Structure(\"rec\") 3) None) Array(Structure(\"rec\") 3) None None None None false))\n  VarDecl(var_decl(\"table\" InitList([Cast(Const(BitInt(40 true)) Constant(ConstInt(1) Int) Const(BitInt(40 true))) Cast(Const(BitInt(40 true)) Unary(Negate Constant(ConstInt(2) Int) Int) Const(BitInt(40 true))) Constant(ConstBitInt(549755813887 40 true) BitInt(40 true)) Cast(Const(BitInt(40 true)) Unary(Negate Constant(ConstBitInt(549755813888 41 true) BitInt(41 true)) BitInt(41 true)) Const(BitInt(40 true)))] Array(Const(BitInt(40 true)) 4) None) Array(Const(BitInt(40 true)) 4) Static None None None false))\n  VarDecl(var_decl(\"counter\" Cast(BitInt(24 false) Constant(ConstLong(16777214) Long) BitInt(24 false)) BitInt(24 false) None None None None false))\n  VarDecl(var_decl(\"small\" Cast(BitInt(5 true) Constant(ConstInt(100) Int) BitInt(5 true)) BitInt(5 true) None None None None false))\n  VarDecl(var_decl(\"results\" None Array(Long 16) None None None None false))\n  FunctionDecl(function_decl(\"add24\" [\"@70.a\" \"@71.b\"] Block([\n    S(Return(Binary(Add Var(\"@70.a\" BitInt(24 false)) Var(\"@71.b\" BitInt(24 false)) BitInt(24 false))))\n  ]) FunType([BitInt(24 false) BitInt(24 false)] BitInt(24 false)) None None [\"\" \"\"] false None [] None false))\n  FunctionDecl(function_decl(\"twice\" [\"@72.p\"] Block([\n    S(Expression(CompoundAssignment(Multiply Dereference(Var(\"@72.p\" Pointer(BitInt(12 true))) BitInt(12 true)) Constant(ConstInt(2) Int) Int BitInt(12 true))))\n    S(Return(Dereference(Var(\"@72.p\" Pointer(BitInt(12 true))) BitInt(12 true))))\n  ]) FunType([Pointer(BitInt(12 true))] BitInt(12 true)) None None [\"\"] false None [] None false))\n  FunctionDecl(function_decl(\"pick\" [\"@73.i\"] Block([\n    S(Return(Subscript(AddressOf(Var(\"table\" Array(Const(BitInt(40 true)) 4)) Pointer(Const(BitInt(40 true)))) Binary(BitwiseAnd Cast(Int Var(\"@73.i\" BitInt(3 true)) Int) Constant(ConstInt(3) Int) Int) Const(BitInt(40 true)))))\n  ]) FunType([BitInt(3 true)] BitInt(40 true)) None None [\"\"] false None [] None false))\n  FunctionDecl(function_decl(\"classify\" [\"@74.v\"] Block([\n    S(SwitchStmt(Var(\"@74.v\" BitInt(5 true)) Compound(Block([\n      S(CaseStmt(Unary(Negate Constant(ConstInt(16) Int) Int) Return(Constant(ConstInt(1) Int)) \".case@1\"))\n      S(CaseStmt(Constant(ConstInt(15) Int) Return(Constant(ConstInt(2) Int)) \".case@2\"))\n      S(CaseStmt(Constant(ConstInt(0) Int) Return(Constant(ConstInt(3) Int)) \".case@3\"))\n      S(DefaultStmt(Return(Constant(ConstInt(4) Int)) \".default@4\"))\n    ])) \".switch@0\" [switch_case(Constant(ConstBitInt(16 5 true) BitInt(5 true)) \".case@1\") switch_case(Constant(ConstBitInt(15 5 true) BitInt(5 true)) \".case@2\") switch_case(Constant(ConstBitInt(0 5 true) BitInt(5 true)) \".case@3\")] \".default@4\" BitInt(5 true)))\n  ]) FunType([BitInt(5 true)] Int) None None [\"\"] false None [] None false))\n  FunctionDecl(function_decl(\"more\" [\"@75.r\"] None FunType([Pointer(Long)] Void) None None [\"\"] false None [] None false))\n  FunctionDecl(function_decl(\"main\" [] Block([\n    D(VarDecl(var_decl(\"@76.r\" AddressOf(Var(\"results\" Array(Long 16)) Pointer(Long)) Pointer(Long) None None None None false)))\n    D(VarDecl(var_decl(\"@77.x\" Dot(Subscript(AddressOf(Var(\"recs\" Array(Structure(\"rec\") 3)) Pointer(Structure(\"rec\"))) Constant(ConstInt(0) Int) Structure(\"rec\")) \"x\" BitInt(12 true)) BitInt(12 true) None None None None false)))\n    S(Expression(Assignment(Dereference(Postfix(Increment Var(\"@76.r\" Pointer(Long)) Pointer(Long)) Long) Cast(Long FunctionCall(\"add24\" [Var(\"counter\" BitInt(24 false)) Cast(BitInt(24 false) Constant(ConstInt(3) Int) BitInt(24 false))] BitInt(24 false) false) Long) Long)))\n    S(Expression(Postfix(Increment Var(\"counter\" BitInt(24 false)) BitInt(24 false))))\n    S(Expression(Assignment(Dereference(Postfix(Increment Var(\"@76.r\" Pointer(Long)) Pointer(Long)) Long) Cast(Long Var(\"counter\" BitInt(24 false)) Long) Long)))\n    S(Expression(Assignment(Dereference(Postfix(Increment Var(\"@76.r\" Pointer(Long)) Pointer(Long)) Long) Cast(Long FunctionCall(\"twice\" [AddressOf(Var(\"@77.x\" BitInt(12 true)) Pointer(BitInt(12 true)))] BitInt(12 true) false) Long) Long)))\n    S(Expression(Assignment(Dereference(Postfix(Increment Var(\"@76.r\" Pointer(Long)) Pointer(Long)) Long) Cast(Long FunctionCall(\"twice\" [AddressOf(Dot(Subscript(AddressOf(Var(\"recs\" Array(Structure(\"rec\") 3)) Pointer(Structure(\"rec\"))) Constant(ConstInt(1) Int) Structure(\"rec\")) \"x\" BitInt(12 true)) Pointer(BitInt(12 true)))] BitInt(12 true) false) Long) Long)))\n    S(Expression(Assignment(Dereference(Postfix(Increment Var(\"@76.r\" Pointer(Long)) Pointer(Long)) Long) Cast(Long FunctionCall(\"pick\" [Cast(BitInt(3 true) Constant(ConstInt(2) Int) BitInt(3 true))] BitInt(40 true) false) Long) Long)))\n    S(Expression(Assignment(Dereference(Postfix(Increment Var(\"@76.r\" Pointer(Long)) Pointer(Long)) Long) Cast(Long Binary(RightShift FunctionCall(\"pick\" [Cast(BitInt(3 true) Constant(ConstInt(3) Int) BitInt(3 true))] BitInt(40 true) false) Constant(ConstInt(8) Int) BitInt(40 true)) Long) Long)))\n    S(Expression(Assignment(Dereference(Postfix(Increment Var(\"@76.r\" Pointer(Long)) Pointer(Long)) Long) Cast(Long Binary(Add Binary(Multiply FunctionCall(\"classify\" [Dot(Subscript(AddressOf(Var(\"recs\" Array(Structure(\"rec\") 3)) Pointer(Structure(\"rec\"))) Constant(ConstInt(0) Int) Structure(\"rec\")) \"small\" BitInt(5 true))] Int false) Constant(ConstInt(10) Int) Int) FunctionCall(\"classify\" [Dot(Subscript(AddressOf(Var(\"recs\" Array(Structure(\"rec\") 3)) Pointer(Structure(\"rec\"))) Constant(ConstInt(1) Int) Structure(\"rec\")) \"small\" BitInt(5 true))] Int false) Int) Long) Long)))\n    S(Expression(Assignment(Dereference(Postfix(Increment Var(\"@76.r\" Pointer(Long)) Pointer(Long)) Long) Cast(Long Binary(Add Binary(Multiply FunctionCall(\"classify\" [Var(\"small\" BitInt(5 true))] Int false) Constant(ConstInt(10) Int) Int) FunctionCall(\"classify\" [Cast(BitInt(5 true) Binary(Subtract Cast(Int Var(\"small\" BitInt(5 true)) Int) Constant(ConstInt(4) Int) Int) BitInt(5 true))] Int false) Int) Long) Long)))\n    S(Expression(FunctionCall(\"more\" [Var(\"@76.r\" Pointer(Long))] Void false)))\n    S(Return(Constant(ConstInt(0) Int)))\n  ]) FunType([] Int) None None [] false None [] None false))\n  FunctionDecl(function_decl(\"more\" [\"@78.r\"] Block([\n    S(Expression(Assignment(Dereference(Postfix(Increment Var(\"@78.r\" Pointer(Long)) Pointer(Long)) Long) Cast(Long Binary(RightShift Dot(Subscript(AddressOf(Var(\"recs\" Array(Structure(\"rec\") 3)) Pointer(Structure(\"rec\"))) Constant(ConstInt(0) Int) Structure(\"rec\")) \"big\" BitInt(24 false)) Constant(ConstInt(4) Int) BitInt(24 false)) Long) Long)))\n    S(Expression(Assignment(Dereference(Postfix(Increment Var(\"@78.r\" Pointer(Long)) Pointer(Long)) Long) Cast(Long Cast(BitInt(4 false) Binary(Add Cast(Int Dot(Subscript(AddressOf(Var(\"recs\" Array(Structure(\"rec\") 3)) Pointer(Structure(\"rec\"))) Constant(ConstInt(0) Int) Structure(\"rec\")) \"tag\" UChar) Int) Constant(ConstInt(20) Int) Int) BitInt(4 false)) Long) Long)))\n    S(Expression(Assignment(Dereference(Postfix(Increment Var(\"@78.r\" Pointer(Long)) Pointer(Long)) Long) Cast(Long FunctionCall(\"__c6502_stdc_bit_width\" [Constant(ConstBitInt(1000 16 false) BitInt(16 false))] UInt false) Long) Long)))\n    S(Expression(Assignment(Dereference(Postfix(Increment Var(\"@78.r\" Pointer(Long)) Pointer(Long)) Long) Cast(Long SizeOfType(Structure(\"rec\") UInt) Long) Long)))\n    S(Expression(Assignment(Dereference(Postfix(Increment Var(\"@78.r\" Pointer(Long)) Pointer(Long)) Long) Cast(Long Binary(GreaterThan Cast(BitInt(24 false) Unary(Negate Constant(ConstInt(1) Int) Int) BitInt(24 false)) Cast(BitInt(24 false) Constant(ConstInt(0) Int) BitInt(24 false)) Int) Long) Long)))\n    S(Expression(Assignment(Dereference(Postfix(Increment Var(\"@78.r\" Pointer(Long)) Pointer(Long)) Long) Cast(Long Binary(LessThan Cast(Int Cast(BitInt(12 true) Unary(Negate Constant(ConstInt(1) Int) Int) BitInt(12 true)) Int) Constant(ConstInt(0) Int) Int) Long) Long)))\n    S(Expression(Assignment(Dereference(Postfix(Increment Var(\"@78.r\" Pointer(Long)) Pointer(Long)) Long) Cast(Long Cast(BitInt(20 true) Cast(Double Unary(Negate Constant(ConstDouble(4683220296470102016) Double) Double) Double) BitInt(20 true)) Long) Long)))\n    S(Expression(Assignment(Dereference(Postfix(Increment Var(\"@78.r\" Pointer(Long)) Pointer(Long)) Long) Cast(Long Cast(Float Cast(BitInt(24 false) Constant(ConstLong(1193046) Long) BitInt(24 false)) Float) Long) Long)))\n  ]) FunType([Pointer(Long)] Void) None None [\"\"] false None [] None false))\n])\n"}, {"object": "passes.type_checking.SymbolTable", "fields": {"_table": {"dict": [["recs", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Array(Structure(\"rec\") 3)\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Initial", "fields": {"value": {"tuple": [{"tuple": [1, -5, 11259375, -16]}, {"tuple": [2, 2047, 7, 15]}, {"tuple": [0, 0, 0, 0]}]}}}, "is_global": true, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}], ["table", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Array(Const(BitInt(40 true)) 4)\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Initial", "fields": {"value": {"tuple": [1, -2, 549755813887, -549755813888]}}}, "is_global": false, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}], ["counter", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "BitInt(24 false)\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Initial", "fields": {"value": 16777214}}, "is_global": true, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}], ["small", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "BitInt(5 true)\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Initial", "fields": {"value": 4}}, "is_global": true, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}], ["results", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Array(Long 16)\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Tentative", "fields": {}}, "is_global": true, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": )~~", R"~~(false, "was_bool": false}}}}], ["add24", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "FunType([BitInt(24 false) BitInt(24 false)] BitInt(24 false))\n"}, "attrs": {"object": "passes.type_checking.FunAttr", "fields": {"defined": true, "is_global": true, "carry_return": false, "address": null, "preserves": {"set": []}, "section": null, "param_zeropage": {"tuple": []}, "returns_bool": false, "noreturn": false}}}}], ["@70.a", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "BitInt(24 false)\n"}, "attrs": {"object": "passes.type_checking.LocalAttr", "fields": {"register": false, "was_bool": false}}}}], ["@71.b", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "BitInt(24 false)\n"}, "attrs": {"object": "passes.type_checking.LocalAttr", "fields": {"register": false, "was_bool": false}}}}], ["twice", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "FunType([Pointer(BitInt(12 true))] BitInt(12 true))\n"}, "attrs": {"object": "passes.type_checking.FunAttr", "fields": {"defined": true, "is_global": true, "carry_return": false, "address": null, "preserves": {"set": []}, "section": null, "param_zeropage": {"tuple": []}, "returns_bool": false, "noreturn": false}}}}], ["@72.p", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Pointer(BitInt(12 true))\n"}, "attrs": {"object": "passes.type_checking.LocalAttr", "fields": {"register": false, "was_bool": false}}}}], ["pick", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "FunType([BitInt(3 true)] BitInt(40 true))\n"}, "attrs": {"object": "passes.type_checking.FunAttr", "fields": {"defined": true, "is_global": true, "carry_return": false, "address": null, "preserves": {"set": []}, "section": null, "param_zeropage": {"tuple": []}, "returns_bool": false, "noreturn": false}}}}], ["@73.i", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "BitInt(3 true)\n"}, "attrs": {"object": "passes.type_checking.LocalAttr", "fields": {"register": false, "was_bool": false}}}}], ["classify", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "FunType([BitInt(5 true)] Int)\n"}, "attrs": {"object": "passes.type_checking.FunAttr", "fields": {"defined": true, "is_global": true, "carry_return": false, "address": null, "preserves": {"set": []}, "section": null, "param_zeropage": {"tuple": []}, "returns_bool": false, "noreturn": false}}}}], ["@74.v", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "BitInt(5 true)\n"}, "attrs": {"object": "passes.type_checking.LocalAttr", "fields": {"register": false, "was_bool": false}}}}], ["more", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "FunType([Pointer(Long)] Void)\n"}, "attrs": {"object": "passes.type_checking.FunAttr", "fields": {"defined": true, "is_global": true, "carry_return": false, "address": null, "preserves": {"set": []}, "section": null, "param_zeropage": {"tuple": []}, "returns_bool": false, "noreturn": false}}}}], ["main", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "FunType([] Int)\n"}, "attrs": {"object": "passes.type_checking.FunAttr", "fields": {"defined": true, "is_global": true, "carry_return": false, "address": null, "preserves": {"set": []}, "section": null, "param_zeropage": {"tuple": []}, "returns_bool": false, "noreturn": false}}}}], ["@76.r", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Pointer(Long)\n"}, "attrs": {"object": "passes.type_checking.LocalAttr", "fields": {"register": false, "was_bool": false}}}}], ["@77.x", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "BitInt(12 true)\n"}, "attrs": {"object": "passes.type_checking.LocalAttr", "fields": {"register": false, "was_bool": false}}}}], ["@78.r", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Pointer(Long)\n"}, "attrs": {"object": "passes.type_checking.LocalAttr", "fields": {"register": false, "was_bool": false}}}}]]}}}, {"object": "passes.type_checking.TypeTable", "fields": {"_table": {"dict": [["rec", {"object": "passes.type_checking.StructLayout", "fields": {"tag": "rec", "is_union": false, "members": [{"object": "passes.type_checking.MemberInfo", "fields": {"name": "tag", "type": {"asdl": "c99_ast", "dump": "UChar\n"}, "byte_offset": 0}}, {"object": "passes.type_checking.MemberInfo", "fields": {"name": "x", "type": {"asdl": "c99_ast", "dump": "BitInt(12 true)\n"}, "byte_offset": 1}}, {"object": "passes.type_checking.MemberInfo", "fields": {"name": "big", "type": {"asdl": "c99_ast", "dump": "BitInt(24 false)\n"}, "byte_offset": 3}}, {"object": "passes.type_checking.MemberInfo", "fields": {"name": "small", "type": {"asdl": "c99_ast", "dump": "BitInt(5 true)\n"}, "byte_offset": 6}}], "size": 7, "complete": true}}]]}}}]})~~"}),
     text({R"~~({"input": {"asdl": "c99_ast", "dump": "Program([\n  StructDecl(struct_decl(\"rec\" false [member_decl(\"tag\" UChar) member_decl(\"x\" BitInt(12 true)) member_decl(\"big\" BitInt(24 false)) member_decl(\"small\" BitInt(5 true))]))\n  VarDecl(var_decl(\"recs\" InitList([InitList([Cast(UChar Constant(ConstInt(1) Int) UChar) Cast(BitInt(12 true) Unary(Negate Constant(ConstInt(5) Int) Int) BitInt(12 true)) Cast(BitInt(24 false) Constant(ConstLong(11259375) Long) BitInt(24 false)) Cast(BitInt(5 true) Unary(Negate Constant(ConstInt(16) Int) Int) BitInt(5 true))] Structure(\"rec\") None) InitList([Cast(UChar Constant(ConstInt(2) Int) UChar) Cast(BitInt(12 true) Constant(ConstInt(2047) Int) BitInt(12 true)) Cast(BitInt(24 false) Constant(ConstInt(7) Int) BitInt(24 false)) Cast(BitInt(5 true) Constant(ConstInt(15) Int) BitInt(5 true))] Structure(\"rec\") None)] Array(Structure(\"rec\") 3) None) Array(Structure(\"rec\") 3) None None None None false))\n  VarDecl(var_decl(\"table\" InitList([Cast(Const(BitInt(40 true)) Constant(ConstInt(1) Int) Const(BitInt(40 true))) Cast(Const(BitInt(40 true)) Unary(Negate Constant(ConstInt(2) Int) Int) Const(BitInt(40 true))) Constant(ConstBitInt(549755813887 40 true) BitInt(40 true)) Cast(Const(BitInt(40 true)) Unary(Negate Constant(ConstBitInt(549755813888 41 true) BitInt(41 true)) BitInt(41 true)) Const(BitInt(40 true)))] Array(Const(BitInt(40 true)) 4) None) Array(Const(BitInt(40 true)) 4) Static None None None false))\n  VarDecl(var_decl(\"counter\" Cast(BitInt(24 false) Constant(ConstLong(16777214) Long) BitInt(24 false)) BitInt(24 false) None None None None false))\n  VarDecl(var_decl(\"small\" Cast(BitInt(5 true) Constant(ConstInt(100) Int) BitInt(5 true)) BitInt(5 true) None None None None false))\n  VarDecl(var_decl(\"results\" None Array(Long 16) None None None None false))\n  FunctionDecl(function_decl(\"add24\" [\"@70.a\" \"@71.b\"] Block([\n    S(Return(Binary(Add Var(\"@70.a\" BitInt(24 false)) Var(\"@71.b\" BitInt(24 false)) BitInt(24 false))))\n  ]) FunType([BitInt(24 false) BitInt(24 false)] BitInt(24 false)) None None [\"\" \"\"] false None [] None false))\n  FunctionDecl(function_decl(\"twice\" [\"@72.p\"] Block([\n    S(Expression(CompoundAssignment(Multiply Dereference(Var(\"@72.p\" Pointer(BitInt(12 true))) BitInt(12 true)) Constant(ConstInt(2) Int) Int BitInt(12 true))))\n    S(Return(Dereference(Var(\"@72.p\" Pointer(BitInt(12 true))) BitInt(12 true))))\n  ]) FunType([Pointer(BitInt(12 true))] BitInt(12 true)) None None [\"\"] false None [] None false))\n  FunctionDecl(function_decl(\"pick\" [\"@73.i\"] Block([\n    S(Return(Subscript(AddressOf(Var(\"table\" Array(Const(BitInt(40 true)) 4)) Pointer(Const(BitInt(40 true)))) Binary(BitwiseAnd Cast(Int Var(\"@73.i\" BitInt(3 true)) Int) Constant(ConstInt(3) Int) Int) Const(BitInt(40 true)))))\n  ]) FunType([BitInt(3 true)] BitInt(40 true)) None None [\"\"] false None [] None false))\n  FunctionDecl(function_decl(\"classify\" [\"@74.v\"] Block([\n    S(SwitchStmt(Var(\"@74.v\" BitInt(5 true)) Compound(Block([\n      S(CaseStmt(Unary(Negate Constant(ConstInt(16) Int) Int) Return(Constant(ConstInt(1) Int)) \".case@1\"))\n      S(CaseStmt(Constant(ConstInt(15) Int) Return(Constant(ConstInt(2) Int)) \".case@2\"))\n      S(CaseStmt(Constant(ConstInt(0) Int) Return(Constant(ConstInt(3) Int)) \".case@3\"))\n      S(DefaultStmt(Return(Constant(ConstInt(4) Int)) \".default@4\"))\n    ])) \".switch@0\" [switch_case(Constant(ConstBitInt(16 5 true) BitInt(5 true)) \".case@1\") switch_case(Constant(ConstBitInt(15 5 true) BitInt(5 true)) \".case@2\") switch_case(Constant(ConstBitInt(0 5 true) BitInt(5 true)) \".case@3\")] \".default@4\" BitInt(5 true)))\n  ]) FunType([BitInt(5 true)] Int) None None [\"\"] false None [] None false))\n  FunctionDecl(function_decl(\"more\" [\"@75.r\"] None FunType([Pointer(Long)] Void) None None [\"\"] false None [] None false))\n  FunctionDecl(function_decl(\"main\" [] Block([\n    D(VarDecl(var_decl(\"@76.r\" AddressOf(Var(\"results\" Array(Long 16)) Pointer(Long)) Pointer(Long) None None None None false)))\n    D(VarDecl(var_decl(\"@77.x\" Dot(Subscript(AddressOf(Var(\"recs\" Array(Structure(\"rec\") 3)) Pointer(Structure(\"rec\"))) Constant(ConstInt(0) Int) Structure(\"rec\")) \"x\" BitInt(12 true)) BitInt(12 true) None None None None false)))\n    S(Expression(Assignment(Dereference(Postfix(Increment Var(\"@76.r\" Pointer(Long)) Pointer(Long)) Long) Cast(Long FunctionCall(\"add24\" [Var(\"counter\" BitInt(24 false)) Cast(BitInt(24 false) Constant(ConstInt(3) Int) BitInt(24 false))] BitInt(24 false) false) Long) Long)))\n    S(Expression(Postfix(Increment Var(\"counter\" BitInt(24 false)) BitInt(24 false))))\n    S(Expression(Assignment(Dereference(Postfix(Increment Var(\"@76.r\" Pointer(Long)) Pointer(Long)) Long) Cast(Long Var(\"counter\" BitInt(24 false)) Long) Long)))\n    S(Expression(Assignment(Dereference(Postfix(Increment Var(\"@76.r\" Pointer(Long)) Pointer(Long)) Long) Cast(Long FunctionCall(\"twice\" [AddressOf(Var(\"@77.x\" BitInt(12 true)) Pointer(BitInt(12 true)))] BitInt(12 true) false) Long) Long)))\n    S(Expression(Assignment(Dereference(Postfix(Increment Var(\"@76.r\" Pointer(Long)) Pointer(Long)) Long) Cast(Long FunctionCall(\"twice\" [AddressOf(Dot(Subscript(AddressOf(Var(\"recs\" Array(Structure(\"rec\") 3)) Pointer(Structure(\"rec\"))) Constant(ConstInt(1) Int) Structure(\"rec\")) \"x\" BitInt(12 true)) Pointer(BitInt(12 true)))] BitInt(12 true) false) Long) Long)))\n    S(Expression(Assignment(Dereference(Postfix(Increment Var(\"@76.r\" Pointer(Long)) Pointer(Long)) Long) Cast(Long FunctionCall(\"pick\" [Cast(BitInt(3 true) Constant(ConstInt(2) Int) BitInt(3 true))] BitInt(40 true) false) Long) Long)))\n    S(Expression(Assignment(Dereference(Postfix(Increment Var(\"@76.r\" Pointer(Long)) Pointer(Long)) Long) Cast(Long Binary(RightShift FunctionCall(\"pick\" [Cast(BitInt(3 true) Constant(ConstInt(3) Int) BitInt(3 true))] BitInt(40 true) false) Constant(ConstInt(8) Int) BitInt(40 true)) Long) Long)))\n    S(Expression(Assignment(Dereference(Postfix(Increment Var(\"@76.r\" Pointer(Long)) Pointer(Long)) Long) Cast(Long Binary(Add Binary(Multiply FunctionCall(\"classify\" [Dot(Subscript(AddressOf(Var(\"recs\" Array(Structure(\"rec\") 3)) Pointer(Structure(\"rec\"))) Constant(ConstInt(0) Int) Structure(\"rec\")) \"small\" BitInt(5 true))] Int false) Constant(ConstInt(10) Int) Int) FunctionCall(\"classify\" [Dot(Subscript(AddressOf(Var(\"recs\" Array(Structure(\"rec\") 3)) Pointer(Structure(\"rec\"))) Constant(ConstInt(1) Int) Structure(\"rec\")) \"small\" BitInt(5 true))] Int false) Int) Long) Long)))\n    S(Expression(Assignment(Dereference(Postfix(Increment Var(\"@76.r\" Pointer(Long)) Pointer(Long)) Long) Cast(Long Binary(Add Binary(Multiply FunctionCall(\"classify\" [Var(\"small\" BitInt(5 true))] Int false) Constant(ConstInt(10) Int) Int) FunctionCall(\"classify\" [Cast(BitInt(5 true) Binary(Subtract Cast(Int Var(\"small\" BitInt(5 true)) Int) Constant(ConstInt(4) Int) Int) BitInt(5 true))] Int false) Int) Long) Long)))\n    S(Expression(FunctionCall(\"more\" [Var(\"@76.r\" Pointer(Long))] Void false)))\n    S(Return(Constant(ConstInt(0) Int)))\n  ]) FunType([] Int) None None [] false None [] None false))\n  FunctionDecl(function_decl(\"more\" [\"@78.r\"] Block([\n    S(Expression(Assignment(Dereference(Postfix(Increment Var(\"@78.r\" Pointer(Long)) Pointer(Long)) Long) Cast(Long Binary(RightShift Dot(Subscript(AddressOf(Var(\"recs\" Array(Structure(\"rec\") 3)) Pointer(Structure(\"rec\"))) Constant(ConstInt(0) Int) Structure(\"rec\")) \"big\" BitInt(24 false)) Constant(ConstInt(4) Int) BitInt(24 false)) Long) Long)))\n    S(Expression(Assignment(Dereference(Postfix(Increment Var(\"@78.r\" Pointer(Long)) Pointer(Long)) Long) Cast(Long Cast(BitInt(4 false) Binary(Add Cast(Int Dot(Subscript(AddressOf(Var(\"recs\" Array(Structure(\"rec\") 3)) Pointer(Structure(\"rec\"))) Constant(ConstInt(0) Int) Structure(\"rec\")) \"tag\" UChar) Int) Constant(ConstInt(20) Int) Int) BitInt(4 false)) Long) Long)))\n    S(Expression(Assignment(Dereference(Postfix(Increment Var(\"@78.r\" Pointer(Long)) Pointer(Long)) Long) Cast(Long FunctionCall(\"__c6502_stdc_bit_width\" [Constant(ConstBitInt(1000 16 false) BitInt(16 false))] UInt false) Long) Long)))\n    S(Expression(Assignment(Dereference(Postfix(Increment Var(\"@78.r\" Pointer(Long)) Pointer(Long)) Long) Cast(Long SizeOfType(Structure(\"rec\") UInt) Long) Long)))\n    S(Expression(Assignment(Dereference(Postfix(Increment Var(\"@78.r\" Pointer(Long)) Pointer(Long)) Long) Cast(Long Binary(GreaterThan Cast(BitInt(24 false) Unary(Negate Constant(ConstInt(1) Int) Int) BitInt(24 false)) Cast(BitInt(24 false) Constant(ConstInt(0) Int) BitInt(24 false)) Int) Long) Long)))\n    S(Expression(Assignment(Dereference(Postfix(Increment Var(\"@78.r\" Pointer(Long)) Pointer(Long)) Long) Cast(Long Binary(LessThan Cast(Int Cast(BitInt(12 true) Unary(Negate Constant(ConstInt(1) Int) Int) BitInt(12 true)) Int) Constant(ConstInt(0) Int) Int) Long) Long)))\n    S(Expression(Assignment(Dereference(Postfix(Increment Var(\"@78.r\" Pointer(Long)) Pointer(Long)) Long) Cast(Long Cast(BitInt(20 true) Cast(Double Unary(Negate Constant(ConstDouble(4683220296470102016) Double) Double) Double) BitInt(20 true)) Long) Long)))\n    S(Expression(Assignment(Dereference(Postfix(Increment Var(\"@78.r\" Pointer(Long)) Pointer(Long)) Long) Cast(Long Cast(Float Cast(BitInt(24 false) Constant(ConstLong(1193046) Long) BitInt(24 false)) Float) Long) Long)))\n  ]) FunType([Pointer(Long)] Void) None None [\"\"] false None [] None false))\n])\n"}, "args": {}, "kwargs": {}})~~"})},
    {text({R"~~(Program([
  VarDecl(var_decl("K" Constant(ConstInt(47) None) Const(Int) None None None None true))
  VarDecl(var_decl("b" Binary(Add Var("K" None) Constant(ConstInt(1) None) None) Int Static None None None false))
  VarDecl(var_decl("array" None Array(Int 47) None None None None false))
  VarDecl(var_decl("UC" Constant(ConstInt(200) None) Const(UChar) None None None None true))
  VarDecl(var_decl("SC" Unary(Negate Constant(ConstInt(5) None) None) Const(SChar) None None None None true))
  VarDecl(var_decl("BIG" Constant(ConstLong(100000) None) Const(Long) None None None None true))
  VarDecl(var_decl("T" Constant(ConstBool(1) None) Const(Bool) None None None None true))
  VarDecl(var_decl("D" Constant(ConstDouble(4612811918334230528) None) Const(Double) None None None None true))
  VarDecl(var_decl("F" Constant(ConstUInt(47) None) Const(Float) None None None None true))
  VarDecl(var_decl("P" Constant(ConstNullptr None) Const(Pointer(Int)) None None None None true))
  VarDecl(var_decl("N" Constant(ConstNullptr None) Const(NullptrT) None None None None true))
  StructDecl(struct_decl("inner" false [member_decl("a" UChar) member_decl("b" Int)]))
  StructDecl(struct_decl("s" false [member_decl("x" UChar) member_decl("in" Structure("inner")) member_decl("y" Long)]))
  VarDecl(var_decl("S" InitList([Constant(ConstInt(3) None) InitList([Constant(ConstInt(4) None) Unary(Negate Constant(ConstInt(300) None) None)] None None) Constant(ConstLong(70000) None)] None None) Const(Structure("s")) None None None None true))
  VarDecl(var_decl("S2" Var("S" None) Const(Structure("s")) None None None None true))
  VarDecl(var_decl("S3" Var("S" None) Structure("s") Static None None None false))
  VarDecl(var_decl("c" Binary(Add Dot(Var("S" None) "y" None) Dot(Dot(Var("S" None) "in" None) "b" None) None) Long Static None None None false))
  VarDecl(var_decl("e" Cast(Int Var("D" None) None) Int Static None None None false))
  VarDecl(var_decl("q" Var("P" None) Pointer(Int) Static None None None false))
  VarDecl(var_decl("STR" String("hi") Array(Const(Char) 0) None None None None true))
  VarDecl(var_decl("ARR" InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None) Constant(ConstInt(3) None)] None None) Array(Const(Int) 3) None None None None true))
  StructDecl(struct_decl("u" true [member_decl("w" UInt) member_decl("b" Array(UChar 2))]))
  VarDecl(var_decl("U" InitList([Constant(ConstInt(4660) None)] None None) Const(Union("u")) None None None None true))
  VarDecl(var_decl("d" Dot(Var("U" None) "w" None) UInt Static None None None false))
  VarDecl(var_decl("W" Constant(ConstInt(12) None) Const(Int) None None None None true))
  VarDecl(var_decl("bi" None BitInt(12 true) None None None None false))
  VarDecl(var_decl("sized" None Array(Int 4) None None None None false))
  FunctionDecl(function_decl("depth" ["@0.n"] Block([
    D(VarDecl(var_decl("@1.STEP" Constant(ConstInt(2) None) Const(Int) None None None None true)))
    D(VarDecl(var_decl("@2.p" AddressOf(Var("@1.STEP" None) None) Pointer(Const(Int)) None None None None false)))
    S(IfStmt(Binary(Equal Var("@0.n" None) Constant(ConstInt(0) None) None) Return(Dereference(Var("@2.p" None) None)) None))
    S(Return(Binary(Add FunctionCall("depth" [Binary(Subtract Var("@0.n" None) Constant(ConstInt(1) None) None)] None false) Var("@1.STEP" None) None)))
  ]) FunType([Int] Int) None None [""] false None [] None false))
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("@3.M" Constant(ConstInt(5) None) Const(UChar) None None None None true)))
    D(VarDecl(var_decl("@4.SM" Unary(Negate Constant(ConstInt(7) None) None) Const(Int) Static None None None true)))
    D(VarDecl(var_decl("@5.buf" None Array(UChar 5) None None None None false)))
    S(ForStmt(InitDecl([var_decl("@6.i" Constant(ConstInt(0) None) UChar None None None None false)]) Binary(LessThan Var("@6.i" None) Var("@3.M" None) None) Postfix(Increment Var("@6.i" None) None) Expression(Assignment(Subscript(Var("@5.buf" None) Var("@6.i" None) None) Var("@6.i" None) None)) ".loop@0" None))
    S(IfStmt(Binary(NotEqual SizeOfExp(Var("array" None) None) Binary(Multiply Constant(ConstInt(2) None) Var("K" None) None) None) Return(Constant(ConstInt(1) None)) None))
    S(IfStmt(Binary(NotEqual Constant(ConstInt(48) None) Constant(ConstInt(48) None) None) Return(Constant(ConstInt(2) None)) None))
    S(IfStmt(Binary(NotEqual Var("b" None) Constant(ConstInt(48) None) None) Return(Constant(ConstInt(3) None)) None))
    S(IfStmt(Binary(NotEqual Binary(Add Var("UC" None) Var("SC" None) None) Constant(ConstInt(195) None) None) Return(Constant(ConstInt(4) None)) None))
    S(IfStmt(Binary(NotEqual Binary(Divide Var("BIG" None) Constant(ConstInt(4) None) None) Constant(ConstInt(25000) None) None) Return(Constant(ConstInt(5) None)) None))
    S(IfStmt(Unary(LogicalNot Var("T" None) None) Return(Constant(ConstInt(6) None)) None))
    S(IfStmt(Binary(NotEqual Cast(Int Binary(Multiply Var("D" None) Constant(ConstInt(2) None) None) None) Constant(ConstInt(5) None) None) Return(Constant(ConstInt(7) None)) None))
    S(IfStmt(Binary(NotEqual Var("F" None) Constant(ConstFloat(1111228416) None) None) Return(Constant(ConstInt(8) None)) None))
    S(IfStmt(Binary(LogicalOr Binary(NotEqual Var("P" None) Constant(ConstInt(0) None) None) Binary(NotEqual Var("q" None) Constant(ConstInt(0) None) None) None) Return(Constant(ConstInt(9) None)) None))
    S(IfStmt(Binary(LogicalOr Binary(NotEqual Dot(Dot(Var("S2" None) "in" None) "b" None) Unary(Negate Constant(ConstInt(300) None) None) None) Binary(NotEqual Dot(Var("S3" None) "y" None) Constant(ConstLong(70000) None) None) None) Return(Constant(ConstInt(10) None)) None))
    S(IfStmt(Binary(NotEqual Var("c" None) Binary(Subtract Constant(ConstLong(70000) None) Constant(ConstInt(300) None) None) None) Return(Constant(ConstInt(11) None)) None))
    S(IfStmt(Binary(NotEqual Var("e" None) Constant(ConstInt(2) None) None) Return(Constant(ConstInt(12) None)) None))
    S(IfStmt(Binary(LogicalOr Binary(NotEqual Subscript(Var("STR" None) Constant(ConstInt(1) None) None) Constant(ConstInt(105) None) None) Binary(NotEqual SizeOfExp(Var("STR" None) None) Constant(ConstInt(3) None) None) None) Return(Constant(ConstInt(13) None)) None))
    S(IfStmt(Binary(NotEqual Subscript(Var("ARR" None) Constant(ConstInt(2) None) None) Constant(ConstInt(3) None) None) Return(Constant(ConstInt(14) None)) None))
    S(IfStmt(Binary(NotEqual Var("d" None) Constant(ConstInt(4660) None) None) Return(Constant(ConstInt(15) None)) None))
    S(IfStmt(Binary(LogicalOr Binary(NotEqual SizeOfExp(Var("bi" None) None) Constant(ConstInt(2) None) None) Binary(NotEqual SizeOfExp(Var("sized" None) None) Constant(ConstInt(8) None) None) None) Return(Constant(ConstInt(16) None)) None))
    S(SwitchStmt(Subscript(Var("@5.buf" None) Constant(ConstInt(4) None) None) Compound(Block([
      S(CaseStmt(Binary(Subtract Var("@3.M" None) Constant(ConstInt(1) None) None) BreakStmt(".switch@1") ".case@2"))
      S(DefaultStmt(Return(Constant(ConstInt(17) None)) ".default@3"))
    ])) ".switch@1" [switch_case(Binary(Subtract Var("@3.M" None) Constant(ConstInt(1) None) None) ".case@2")] ".default@3" None))
    S(SwitchStmt(Var("@4.SM" None) Compound(Block([
      S(CaseStmt(Var("@4.SM" None) BreakStmt(".switch@4") ".case@5"))
      S(DefaultStmt(Return(Constant(ConstInt(18) None)) ".default@6"))
    ])) ".switch@4" [switch_case(Var("@4.SM" None) ".case@5")] ".default@6" None))
    S(IfStmt(Binary(NotEqual FunctionCall("depth" [Constant(ConstInt(3) None)] None false) Constant(ConstInt(8) None) None) Return(Constant(ConstInt(19) None)) None))
    D(VarDecl(var_decl("@7.pk" AddressOf(Var("K" None) None) Pointer(Const(Int)) None None None None false)))
    S(IfStmt(Binary(NotEqual Dereference(Var("@7.pk" None) None) Constant(ConstInt(47) None) None) Return(Constant(ConstInt(20) None)) None))
    D(VarDecl(var_decl("@8.x" Constant(ConstInt(3) None) Int None None None None false)))
    S(SwitchStmt(Var("@8.x" None) Compound(Block([
      S(CaseStmt(Dot(Var("S" None) "x" None) BreakStmt(".switch@7") ".case@8"))
      S(DefaultStmt(Return(Constant(ConstInt(21) None)) ".default@9"))
    ])) ".switch@7" [switch_case(Dot(Var("S" None) "x" None) ".case@8")] ".default@9" None))
    S(IfStmt(Binary(NotEqual Var("N" None) Constant(ConstNullptr None) None) Return(Constant(ConstInt(22) None)) None))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"}),
     false,
     text({R"~~({"tuple": [{"asdl": "c99_ast", "dump": "Program([\n  VarDecl(var_decl(\"K\" Constant(ConstInt(47) Int) Const(Int) Static None None None true))\n  VarDecl(var_decl(\"b\" Binary(Add Var(\"K\" Const(Int)) Constant(ConstInt(1) Int) Int) Int Static None None None false))\n  VarDecl(var_decl(\"array\" None Array(Int 47) None None None None false))\n  VarDecl(var_decl(\"UC\" Cast(Const(UChar) Constant(ConstInt(200) Int) Const(UChar)) Const(UChar) Static None None None true))\n  VarDecl(var_decl(\"SC\" Cast(Const(SChar) Unary(Negate Constant(ConstInt(5) Int) Int) Const(SChar)) Const(SChar) Static None None None true))\n  VarDecl(var_decl(\"BIG\" Constant(ConstLong(100000) Long) Const(Long) Static None None None true))\n  VarDecl(var_decl(\"T\" Constant(ConstBool(1) Bool) Const(Bool) Static None None None true))\n  VarDecl(var_decl(\"D\" Constant(ConstDouble(4612811918334230528) Double) Const(Double) Static None None None true))\n  VarDecl(var_decl(\"F\" Cast(Const(Float) Constant(ConstUInt(47) UInt) Const(Float)) Const(Float) Static None None None true))\n  VarDecl(var_decl(\"P\" Cast(Const(Pointer(Int)) Constant(ConstNullptr NullptrT) Const(Pointer(Int))) Const(Pointer(Int)) Static None None None true))\n  VarDecl(var_decl(\"N\" Constant(ConstNullptr NullptrT) Const(NullptrT) Static None None None true))\n  StructDecl(struct_decl(\"inner\" false [member_decl(\"a\" UChar) member_decl(\"b\" Int)]))\n  StructDecl(struct_decl(\"s\" false [member_decl(\"x\" UChar) member_decl(\"in\" Structure(\"inner\")) member_decl(\"y\" Long)]))\n  VarDecl(var_decl(\"S\" InitList([Cast(UChar Constant(ConstInt(3) Int) UChar) InitList([Cast(UChar Constant(ConstInt(4) Int) UChar) Unary(Negate Constant(ConstInt(300) Int) Int)] Structure(\"inner\") None) Constant(ConstLong(70000) Long)] Structure(\"s\") None) Const(Structure(\"s\")) Static None None None true))\n  VarDecl(var_decl(\"S2\" Var(\"S\" Const(Structure(\"s\"))) Const(Structure(\"s\")) Static None None None true))\n  VarDecl(var_decl(\"S3\" Var(\"S\" Const(Structure(\"s\"))) Structure(\"s\") Static None None None false))\n  VarDecl(var_decl(\"c\" Binary(Add Dot(Var(\"S\" Const(Structure(\"s\"))) \"y\" Const(Long)) Cast(Long Dot(Dot(Var(\"S\" Const(Structure(\"s\"))) \"in\" Const(Structure(\"inner\"))) \"b\" Const(Int)) Long) Long) Long Static None None None false))\n  VarDecl(var_decl(\"e\" Cast(Int Var(\"D\" Const(Double)) Int) Int Static None None None false))\n  VarDecl(var_decl(\"q\" Var(\"P\" Const(Pointer(Int))) Pointer(Int) Static None None None false))\n  VarDecl(var_decl(\"STR\" String(\"hi\") Array(Const(Char) 3) Static None None None true))\n  VarDecl(var_decl(\"ARR\" InitList([Constant(ConstInt(1) Int) Constant(ConstInt(2) Int) Constant(ConstInt(3) Int)] Array(Const(Int) 3) None) Array(Const(Int) 3) Static None None None true))\n  StructDecl(struct_decl(\"u\" true [member_decl(\"w\" UInt) member_decl(\"b\" Array(UChar 2))]))\n  VarDecl(var_decl(\"U\" InitList([Cast(UInt Constant(ConstInt(4660) Int) UInt)] Union(\"u\") None) Const(Union(\"u\")) Static None None None true))\n  VarDecl(var_decl(\"d\" Dot(Var(\"U\" Const(Union(\"u\"))) \"w\" Const(UInt)) UInt Static None None None false))\n  VarDecl(var_decl(\"W\" Constant(ConstInt(12) Int) Const(Int) Static None None None true))\n  VarDecl(var_decl(\"bi\" None BitInt(12 true) None None None None false))\n  VarDecl(var_decl(\"sized\" None Array(Int 4) None None None None false))\n  FunctionDecl(function_decl(\"depth\" [\"@0.n\"] Block([\n    D(VarDecl(var_decl(\"@1.STEP\" Constant(ConstInt(2) Int) Const(Int) None None None None true)))\n    D(VarDecl(var_decl(\"@2.p\" AddressOf(Var(\"@1.STEP\" Const(Int)) Pointer(Const(Int))) Pointer(Const(Int)) None None None None false)))\n    S(IfStmt(Binary(Equal Var(\"@0.n\" Int) Constant(ConstInt(0) Int) Int) Return(Dereference(Var(\"@2.p\" Pointer(Const(Int))) Const(Int))) None))\n    S(Return(Binary(Add FunctionCall(\"depth\" [Binary(Subtract Var(\"@0.n\" Int) Constant(ConstInt(1) Int) Int)] Int false) Var(\"@1.STEP\" Const(Int)) Int)))\n  ]) FunType([Int] Int) None None [\"\"] false None [] None false))\n  FunctionDecl(function_decl(\"main\" [] Block([\n    D(VarDecl(var_decl(\"@3.M\" Cast(Const(UChar) Constant(ConstInt(5) Int) Const(UChar)) Const(UChar) None None None None true)))\n    D(VarDecl(var_decl(\"@4.SM\" Unary(Negate Constant(ConstInt(7) Int) Int) Const(Int) Static None None None true)))\n    D(VarDecl(var_decl(\"@5.buf\" None Array(UChar 5) None None None None false)))\n    S(ForStmt(InitDecl([var_decl(\"@6.i\" Cast(UChar Constant(ConstInt(0) Int) UChar) UChar None None None None false)]) Binary(LessThan Cast(Int Var(\"@6.i\" UChar) Int) Cast(Int Var(\"@3.M\" Const(UChar)) Int) Int) Postfix(Increment Var(\"@6.i\" UChar) UChar) Expression(Assignment(Subscript(AddressOf(Var(\"@5.buf\" Array(UChar 5)) Pointer(UChar)) Cast(Int Var(\"@6.i\" UChar) Int) UChar) Var(\"@6.i\" UChar) UChar)) \".loop@0\" None))\n    S(IfStmt(Binary(NotEqual SizeOfExp(Var(\"array\" Array(Int 47)) UInt) Cast(UInt Binary(Multiply Constant(ConstInt(2) Int) Var(\"K\" Const(Int)) Int) UInt) Int) Return(Constant(ConstInt(1) Int)) None))\n    S(IfStmt(Binary(NotEqual Constant(ConstInt(48) Int) Constant(ConstInt(48) Int) Int) Return(Constant(ConstInt(2) Int)) None))\n    S(IfStmt(Binary(NotEqual Var(\"b\" Int) Constant(ConstInt(48) Int) Int) Return(Constant(ConstInt(3) Int)) None))\n    S(IfStmt(Binary(NotEqual Binary(Add Cast(Int Var(\"UC\" Const(UChar)) Int) Cast(Int Var(\"SC\" Const(SChar)) Int) Int) Constant(ConstInt(195) Int) Int) Return(Constant(ConstInt(4) Int)) None))\n    S(IfStmt(Binary(NotEqual Binary(Divide Var(\"BIG\" Const(Long)) Cast(Long Constant(ConstInt(4) Int) Long) Long) Cast(Long Constant(ConstInt(25000) Int) Long) Int) Return(Constant(ConstInt(5) Int)) None))\n    S(IfStmt(Unary(LogicalNot Var(\"T\" Const(Bool)) Int) Return(Constant(ConstInt(6) Int)) None))\n    S(IfStmt(Binary(NotEqual Cast(Int Binary(Multiply Var(\"D\" Const(Double)) Cast(Double Constant(ConstInt(2) Int) Double) Double) Int) Constant(ConstInt(5) Int) Int) Return(Constant(ConstInt(7) Int)) None))\n    S(IfStmt(Binary(NotEqual Var(\"F\" Const(Float)) Constant(ConstFloat(1111228416) Float) Int) Return(Constant(ConstInt(8) Int)) None))\n    S(IfStmt(Binary(LogicalOr Binary(NotEqual Var(\"P\" Const(Pointer(Int))) Cast(Pointer(Int) Constant(ConstInt(0) Int) Pointer(Int)) Int) Binary(NotEqual Var(\"q\" Pointer(Int)) Cast(Pointer(Int) Constant(ConstInt(0) Int) Pointer(Int)) Int) Int) Return(Constant(ConstInt(9) Int)) None))\n    S(IfStmt(Binary(LogicalOr Binary(NotEqual Dot(Dot(Var(\"S2\" Const(Structure(\"s\"))) \"in\" Const(Structure(\"inner\"))) \"b\" Const(Int)) Unary(Negate Constant(ConstInt(300) Int) Int) Int) Binary(NotEqual Dot(Var(\"S3\" Structure(\"s\")) \"y\" Long) Constant(ConstLong(70000) Long) Int) Int) Return(Constant(ConstInt(10) Int)) None))\n    S(IfStmt(Binary(NotEqual Var(\"c\" Long) Binary(Subtract Constant(ConstLong(70000) Long) Cast(Long Constant(ConstInt(300) Int) Long) Long) Int) Return(Constant(ConstInt(11) Int)) None))\n    S(IfStmt(Binary(NotEqual Var(\"e\" Int) Constant(ConstInt(2) Int) Int) Return(Constant(ConstInt(12) Int)) None))\n    S(IfStmt(Binary(LogicalOr Binary(NotEqual Cast(Int Subscript(AddressOf(Var(\"STR\" Array(Const(Char) 3)) Pointer(Const(Char))) Constant(ConstInt(1) Int) Const(Char)) Int) Constant(ConstInt(105) Int) Int) Binary(NotEqual SizeOfExp(Var(\"STR\" Array(Const(Char) 3)) UInt) Cast(UInt Constant(ConstInt(3) Int) UInt) Int) Int) Return(Constant(ConstInt(13) Int)) None))\n    S(IfStmt(Binary(NotEqual Subscript(AddressOf(Var(\"ARR\" Array(Const(Int) 3)) Pointer(Const(Int))) Constant(ConstInt(2) Int) Const(Int)) Constant(ConstInt(3) Int) Int) Return(Constant(ConstInt(14) Int)) None))\n    S(IfStmt(Binary(NotEqual Var(\"d\" UInt) Cast(UInt Constant(ConstInt(4660) Int) UInt) Int) Return(Constant(ConstInt(15) Int)) None))\n    S(IfStmt(Binary(LogicalOr Binary(NotEqual SizeOfExp(Var(\"bi\" BitInt(12 true)) UInt) Cast(UInt Constant(ConstInt(2) Int) UInt) Int) Binary(NotEqual SizeOfExp(Var(\"sized\" Array(Int 4)) UInt) Cast(UInt Constant(ConstInt(8) Int) UInt) Int) Int) Return(Constant(ConstInt(16) Int)) None))\n    S(SwitchStmt(Cast(Int Subscript(AddressOf(Var(\"@5.buf\" Array(UChar 5)) Pointer(UChar)) Constant(ConstInt(4) Int) UChar) Int) Compound(Block([\n      S(CaseStmt(Binary(Subtract Cast(Int Var(\"@3.M\" Const(UChar)) Int) Constant(ConstInt(1) Int) Int) BreakStmt(\".switch@1\") \".case@2\"))\n      S(DefaultStmt(Return(Constant(ConstInt(17) Int)) \".default@3\"))\n    ])) \".switch@1\" [switch_case(Constant(ConstInt(4) Int) \".case@2\")] \".default@3\" Int))\n    S(SwitchStmt(Var(\"@4.SM\" Const(Int)) Compound(Block([\n      S(CaseStmt(Var(\"@4.SM\" Const(Int)) BreakStmt(\".switch@4\") \".case@5\"))\n      S(DefaultStmt(Return(Constant(ConstInt(18) Int)) \".default@6\"))\n    ])) \".switch@4\" [switch_case(Constant(ConstInt(65529) Int) \".case@5\")] \".default@6\" Int))\n    S(IfStmt(Binary(NotEqual FunctionCall(\"depth\" [Constant(ConstInt(3) Int)] Int false) Constant(ConstInt(8) Int) Int) Return(Constant(ConstInt(19) Int)) None))\n    D(VarDecl(var_decl(\"@7.pk\" AddressOf(Var(\"K\" Const(Int)) Pointer(Const(Int))) Pointer(Const(Int)) None None None None false)))\n    S(IfStmt(Binary(NotEqual Dereference(Var(\"@7.pk\" Pointer(Const(Int))) Const(Int)) Constant(ConstInt(47) Int) Int) Return(Constant(ConstInt(20) Int)) None))\n    D(VarDecl(var_decl(\"@8.x\" Constant(ConstInt(3) Int) Int None None None None false)))\n    S(SwitchStmt(Var(\"@8.x\" Int) Compound(Block([\n      S(CaseStmt(Dot(Var(\"S\" Const(Structure(\"s\"))) \"x\" Const(UChar)) BreakStmt(\".switch@7\") \".case@8\"))\n      S(DefaultStmt(Return(Constant(ConstInt(21) Int)) \".default@9\"))\n    ])) \".switch@7\" [switch_case(Constant(ConstInt(3) Int) \".case@8\")] \".default@9\" Int))\n    S(IfStmt(Binary(NotEqual Cast(Pointer(Void) Var(\"N\" Const(NullptrT)) Pointer(Void)) Cast(Pointer(Void) Constant(ConstNullptr NullptrT) Pointer(Void)) Int) Return(Constant(ConstInt(22) Int)) None))\n    S(Return(Constant(ConstInt(0) Int)))\n  ]) FunType([] Int) None None [] false None [] None false))\n])\n"}, {"object": "passes.type_checking.SymbolTable", "fields": {"_table": {"dict": [["K", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Const(Int)\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Initial", "fields": {"value": 47}}, "is_global": false, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}], ["b", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Int\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Initial", "fields": {"value": 48}}, "is_global": false, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}], ["array", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Array(Int 47)\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Tentative", "fields": {}}, "is_global": true, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}], ["UC", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Const(UChar)\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Initial", "fields": {"value": 200}}, "is_global": false, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}], ["SC", {"object": "passes.type_checking.Symbol", "fields": {"type": )~~", R"~~({"asdl": "c99_ast", "dump": "Const(SChar)\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Initial", "fields": {"value": -5}}, "is_global": false, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}], ["BIG", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Const(Long)\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Initial", "fields": {"value": 100000}}, "is_global": false, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}], ["T", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Const(Bool)\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Initial", "fields": {"value": 1}}, "is_global": false, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}], ["D", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Const(Double)\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Initial", "fields": {"value": 4612811918334230528}}, "is_global": false, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}], ["F", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Const(Float)\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Initial", "fields": {"value": 1111228416}}, "is_global": false, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}], ["P", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Const(Pointer(Int))\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Initial", "fields": {"value": 0}}, "is_global": false, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}], ["N", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Const(NullptrT)\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Initial", "fields": {"value": 0}}, "is_global": false, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}], ["S", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Const(Structure(\"s\"))\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Initial", "fields": {"value": {"tuple": [3, {"tuple": [4, -300]}, 70000]}}}, "is_global": false, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}], ["S2", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Const(Structure(\"s\"))\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Initial", "fields": {"value": {"tuple": [3, {"tuple": [4, -300]}, 70000]}}}, "is_global": false, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}], ["S3", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Structure(\"s\")\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Initial", "fields": {"value": {"tuple": [3, {"tuple": [4, -300]}, 70000]}}}, "is_global": false, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}], ["c", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Long\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Initial", "fields": {"value": 69700}}, "is_global": false, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}], ["e", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Int\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Initial", "fields": {"value": 2}}, "is_global": false, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}], ["q", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Pointer(Int)\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Initial", "fields": {"value": 0}}, "is_global": false, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}], ["STR", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Array(Const(Char) 3)\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Initial", "fields": {"value": {"tuple": [104, 105, 0]}}}, "is_global": false, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}], ["ARR", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Array(Const(Int) 3)\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Initial", "fields": {"value": {"tuple": [1, 2, 3]}}}, "is_global": false, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}], ["U", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Const(Union(\"u\"))\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Initial", "fields": {"value": {"tuple": [4660]}}}, "is_global": false, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}], ["d", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "UInt\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Initial", "fields": {"value": 4660}}, "is_global": false, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}], ["W", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Const(Int)\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Initial", "fields": {"value": 12}}, "is_global": false, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}], ["bi", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "BitInt(12 true)\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Tentative", "fields": {}}, "is_global": true, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}], ["sized", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Array(Int 4)\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Tentative", "fields": {}}, "is_global": true, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}], ["depth", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "FunType([Int] Int)\n"}, "attrs": {"object": "passes.type_checking.FunAttr", "fields": {"defined": true, "is_global": true, "carry_return": false, "address": null, "preserves": {"set": []}, "section": null, "param_zeropage": {"tuple": []}, "returns_bool": false, "noreturn": false}}}}], ["@0.n", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Int\n"}, "attrs": {"object": "passes.type_checking.LocalAttr", "fields": {"register": false, "was_bool": false}}}}], ["@1.STEP", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Const(Int)\n"}, "attrs": {"object": "passes.type_checking.LocalAttr", "fields": {"register": false, "was_bool": false}}}}], ["@2.p", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Pointer(Const(Int))\n"}, "attrs": {"object": "passes.type_checking.LocalAttr", "fields": {"register": false, "was_bool": false}}}}], ["main", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "FunType([] Int)\n"}, "attrs": {"object": "passes.type_checking.FunAttr", "fields": {"defined": true, "is_global": true, "carry_return": false, "address": null, "preserves": {"set": []}, "section": null, "param_zeropage": {"tuple": []}, "returns_bool": false, "noreturn": false}}}}], ["@3.M", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Const(UChar)\n"}, "attrs": {"object": "passes.type_checking.LocalAttr", "fields": {"register": false, "was_bool": false}}}}], ["@4.SM", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Const(Int)\n"}, "attrs": {"object": "passes.type_checking.StaticAttr", "fields": {"initial_value": {"object": "passes.type_checking.Initial", "fields": {"value": -7}}, "is_global": false, "zeropage": false, "zp_address": null, "address": null, "section": null, "planes": null, "mergeable": false, "was_bool": false}}}}], ["@5.buf", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Array(UChar 5)\n"}, "attrs": {"object": "passes.type_checking.LocalAttr", "fields": {"register": false, "was_bool": false}}}}], ["@6.i", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "UChar\n"}, "attrs": {"object": "passes.type_checking.LocalAttr", "fields": {"register": false, "was_bool": false}}}}], ["@7.pk", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Pointer(Const(Int))\n"}, "attrs": {"object": "passes.type_checking.LocalAttr", "fields": {"register": false, "was_bool": false}}}}], ["@8.x", {"object": "passes.type_checking.Symbol", "fields": {"type": {"asdl": "c99_ast", "dump": "Int\n"}, "attrs": {"object": "passes.type_checking.LocalAttr", "fields": {"register": false, "was_bool": false}}}}]]}}}, {"object": "passes.type_checking.TypeTable", "fields": {"_table": {"dict": [["inner", {"object": "passes.type_checking.StructLayout", "fields": {"tag": "inner", "is_union": false, "members": [{"object": "passes.type_checking.MemberInfo", "fields": {"name": "a", "type": {"asdl": "c99_ast", "dump": "UChar\n"}, "byte_offset": 0}}, {"object": "passes.type_checking.MemberInfo", "fields": {"name": "b", "type": {"asdl": "c99_ast", "dump": "Int\n"}, "byte_offset": 1}}],)~~", R"~~( "size": 3, "complete": true}}], ["s", {"object": "passes.type_checking.StructLayout", "fields": {"tag": "s", "is_union": false, "members": [{"object": "passes.type_checking.MemberInfo", "fields": {"name": "x", "type": {"asdl": "c99_ast", "dump": "UChar\n"}, "byte_offset": 0}}, {"object": "passes.type_checking.MemberInfo", "fields": {"name": "in", "type": {"asdl": "c99_ast", "dump": "Structure(\"inner\")\n"}, "byte_offset": 1}}, {"object": "passes.type_checking.MemberInfo", "fields": {"name": "y", "type": {"asdl": "c99_ast", "dump": "Long\n"}, "byte_offset": 4}}], "size": 8, "complete": true}}], ["u", {"object": "passes.type_checking.StructLayout", "fields": {"tag": "u", "is_union": true, "members": [{"object": "passes.type_checking.MemberInfo", "fields": {"name": "w", "type": {"asdl": "c99_ast", "dump": "UInt\n"}, "byte_offset": 0}}, {"object": "passes.type_checking.MemberInfo", "fields": {"name": "b", "type": {"asdl": "c99_ast", "dump": "Array(UChar 2)\n"}, "byte_offset": 0}}], "size": 2, "complete": true}}]]}}}]})~~"}),
     text({R"~~({"input": {"asdl": "c99_ast", "dump": "Program([\n  VarDecl(var_decl(\"K\" Constant(ConstInt(47) Int) Const(Int) Static None None None true))\n  VarDecl(var_decl(\"b\" Binary(Add Var(\"K\" Const(Int)) Constant(ConstInt(1) Int) Int) Int Static None None None false))\n  VarDecl(var_decl(\"array\" None Array(Int 47) None None None None false))\n  VarDecl(var_decl(\"UC\" Cast(Const(UChar) Constant(ConstInt(200) Int) Const(UChar)) Const(UChar) Static None None None true))\n  VarDecl(var_decl(\"SC\" Cast(Const(SChar) Unary(Negate Constant(ConstInt(5) Int) Int) Const(SChar)) Const(SChar) Static None None None true))\n  VarDecl(var_decl(\"BIG\" Constant(ConstLong(100000) Long) Const(Long) Static None None None true))\n  VarDecl(var_decl(\"T\" Constant(ConstBool(1) Bool) Const(Bool) Static None None None true))\n  VarDecl(var_decl(\"D\" Constant(ConstDouble(4612811918334230528) Double) Const(Double) Static None None None true))\n  VarDecl(var_decl(\"F\" Cast(Const(Float) Constant(ConstUInt(47) UInt) Const(Float)) Const(Float) Static None None None true))\n  VarDecl(var_decl(\"P\" Cast(Const(Pointer(Int)) Constant(ConstNullptr NullptrT) Const(Pointer(Int))) Const(Pointer(Int)) Static None None None true))\n  VarDecl(var_decl(\"N\" Constant(ConstNullptr NullptrT) Const(NullptrT) Static None None None true))\n  StructDecl(struct_decl(\"inner\" false [member_decl(\"a\" UChar) member_decl(\"b\" Int)]))\n  StructDecl(struct_decl(\"s\" false [member_decl(\"x\" UChar) member_decl(\"in\" Structure(\"inner\")) member_decl(\"y\" Long)]))\n  VarDecl(var_decl(\"S\" InitList([Cast(UChar Constant(ConstInt(3) Int) UChar) InitList([Cast(UChar Constant(ConstInt(4) Int) UChar) Unary(Negate Constant(ConstInt(300) Int) Int)] Structure(\"inner\") None) Constant(ConstLong(70000) Long)] Structure(\"s\") None) Const(Structure(\"s\")) Static None None None true))\n  VarDecl(var_decl(\"S2\" Var(\"S\" Const(Structure(\"s\"))) Const(Structure(\"s\")) Static None None None true))\n  VarDecl(var_decl(\"S3\" Var(\"S\" Const(Structure(\"s\"))) Structure(\"s\") Static None None None false))\n  VarDecl(var_decl(\"c\" Binary(Add Dot(Var(\"S\" Const(Structure(\"s\"))) \"y\" Const(Long)) Cast(Long Dot(Dot(Var(\"S\" Const(Structure(\"s\"))) \"in\" Const(Structure(\"inner\"))) \"b\" Const(Int)) Long) Long) Long Static None None None false))\n  VarDecl(var_decl(\"e\" Cast(Int Var(\"D\" Const(Double)) Int) Int Static None None None false))\n  VarDecl(var_decl(\"q\" Var(\"P\" Const(Pointer(Int))) Pointer(Int) Static None None None false))\n  VarDecl(var_decl(\"STR\" String(\"hi\") Array(Const(Char) 3) Static None None None true))\n  VarDecl(var_decl(\"ARR\" InitList([Constant(ConstInt(1) Int) Constant(ConstInt(2) Int) Constant(ConstInt(3) Int)] Array(Const(Int) 3) None) Array(Const(Int) 3) Static None None None true))\n  StructDecl(struct_decl(\"u\" true [member_decl(\"w\" UInt) member_decl(\"b\" Array(UChar 2))]))\n  VarDecl(var_decl(\"U\" InitList([Cast(UInt Constant(ConstInt(4660) Int) UInt)] Union(\"u\") None) Const(Union(\"u\")) Static None None None true))\n  VarDecl(var_decl(\"d\" Dot(Var(\"U\" Const(Union(\"u\"))) \"w\" Const(UInt)) UInt Static None None None false))\n  VarDecl(var_decl(\"W\" Constant(ConstInt(12) Int) Const(Int) Static None None None true))\n  VarDecl(var_decl(\"bi\" None BitInt(12 true) None None None None false))\n  VarDecl(var_decl(\"sized\" None Array(Int 4) None None None None false))\n  FunctionDecl(function_decl(\"depth\" [\"@0.n\"] Block([\n    D(VarDecl(var_decl(\"@1.STEP\" Constant(ConstInt(2) Int) Const(Int) None None None None true)))\n    D(VarDecl(var_decl(\"@2.p\" AddressOf(Var(\"@1.STEP\" Const(Int)) Pointer(Const(Int))) Pointer(Const(Int)) None None None None false)))\n    S(IfStmt(Binary(Equal Var(\"@0.n\" Int) Constant(ConstInt(0) Int) Int) Return(Dereference(Var(\"@2.p\" Pointer(Const(Int))) Const(Int))) None))\n    S(Return(Binary(Add FunctionCall(\"depth\" [Binary(Subtract Var(\"@0.n\" Int) Constant(ConstInt(1) Int) Int)] Int false) Var(\"@1.STEP\" Const(Int)) Int)))\n  ]) FunType([Int] Int) None None [\"\"] false None [] None false))\n  FunctionDecl(function_decl(\"main\" [] Block([\n    D(VarDecl(var_decl(\"@3.M\" Cast(Const(UChar) Constant(ConstInt(5) Int) Const(UChar)) Const(UChar) None None None None true)))\n    D(VarDecl(var_decl(\"@4.SM\" Unary(Negate Constant(ConstInt(7) Int) Int) Const(Int) Static None None None true)))\n    D(VarDecl(var_decl(\"@5.buf\" None Array(UChar 5) None None None None false)))\n    S(ForStmt(InitDecl([var_decl(\"@6.i\" Cast(UChar Constant(ConstInt(0) Int) UChar) UChar None None None None false)]) Binary(LessThan Cast(Int Var(\"@6.i\" UChar) Int) Cast(Int Var(\"@3.M\" Const(UChar)) Int) Int) Postfix(Increment Var(\"@6.i\" UChar) UChar) Expression(Assignment(Subscript(AddressOf(Var(\"@5.buf\" Array(UChar 5)) Pointer(UChar)) Cast(Int Var(\"@6.i\" UChar) Int) UChar) Var(\"@6.i\" UChar) UChar)) \".loop@0\" None))\n    S(IfStmt(Binary(NotEqual SizeOfExp(Var(\"array\" Array(Int 47)) UInt) Cast(UInt Binary(Multiply Constant(ConstInt(2) Int) Var(\"K\" Const(Int)) Int) UInt) Int) Return(Constant(ConstInt(1) Int)) None))\n    S(IfStmt(Binary(NotEqual Constant(ConstInt(48) Int) Constant(ConstInt(48) Int) Int) Return(Constant(ConstInt(2) Int)) None))\n    S(IfStmt(Binary(NotEqual Var(\"b\" Int) Constant(ConstInt(48) Int) Int) Return(Constant(ConstInt(3) Int)) None))\n    S(IfStmt(Binary(NotEqual Binary(Add Cast(Int Var(\"UC\" Const(UChar)) Int) Cast(Int Var(\"SC\" Const(SChar)) Int) Int) Constant(ConstInt(195) Int) Int) Return(Constant(ConstInt(4) Int)) None))\n    S(IfStmt(Binary(NotEqual Binary(Divide Var(\"BIG\" Const(Long)) Cast(Long Constant(ConstInt(4) Int) Long) Long) Cast(Long Constant(ConstInt(25000) Int) Long) Int) Return(Constant(ConstInt(5) Int)) None))\n    S(IfStmt(Unary(LogicalNot Var(\"T\" Const(Bool)) Int) Return(Constant(ConstInt(6) Int)) None))\n    S(IfStmt(Binary(NotEqual Cast(Int Binary(Multiply Var(\"D\" Const(Double)) Cast(Double Constant(ConstInt(2) Int) Double) Double) Int) Constant(ConstInt(5) Int) Int) Return(Constant(ConstInt(7) Int)) None))\n    S(IfStmt(Binary(NotEqual Var(\"F\" Const(Float)) Constant(ConstFloat(1111228416) Float) Int) Return(Constant(ConstInt(8) Int)) None))\n    S(IfStmt(Binary(LogicalOr Binary(NotEqual Var(\"P\" Const(Pointer(Int))) Cast(Pointer(Int) Constant(ConstInt(0) Int) Pointer(Int)) Int) Binary(NotEqual Var(\"q\" Pointer(Int)) Cast(Pointer(Int) Constant(ConstInt(0) Int) Pointer(Int)) Int) Int) Return(Constant(ConstInt(9) Int)) None))\n    S(IfStmt(Binary(LogicalOr Binary(NotEqual Dot(Dot(Var(\"S2\" Const(Structure(\"s\"))) \"in\" Const(Structure(\"inner\"))) \"b\" Const(Int)) Unary(Negate Constant(ConstInt(300) Int) Int) Int) Binary(NotEqual Dot(Var(\"S3\" Structure(\"s\")) \"y\" Long) Constant(ConstLong(70000) Long) Int) Int) Return(Constant(ConstInt(10) Int)) None))\n    S(IfStmt(Binary(NotEqual Var(\"c\" Long) Binary(Subtract Constant(ConstLong(70000) Long) Cast(Long Constant(ConstInt(300) Int) Long) Long) Int) Return(Constant(ConstInt(11) Int)) None))\n    S(IfStmt(Binary(NotEqual Var(\"e\" Int) Constant(ConstInt(2) Int) Int) Return(Constant(ConstInt(12) Int)) None))\n    S(IfStmt(Binary(LogicalOr Binary(NotEqual Cast(Int Subscript(AddressOf(Var(\"STR\" Array(Const(Char) 3)) Pointer(Const(Char))) Constant(ConstInt(1) Int) Const(Char)) Int) Constant(ConstInt(105) Int) Int) Binary(NotEqual SizeOfExp(Var(\"STR\" Array(Const(Char) 3)) UInt) Cast(UInt Constant(ConstInt(3) Int) UInt) Int) Int) Return(Constant(ConstInt(13) Int)) None))\n    S(IfStmt(Binary(NotEqual Subscript(AddressOf(Var(\"ARR\" Array(Const(Int) 3)) Pointer(Const(Int))) Constant(ConstInt(2) Int) Const(Int)) Constant(ConstInt(3) Int) Int) Return(Constant(ConstInt(14) Int)) None))\n    S(IfStmt(Binary(NotEqual Var(\"d\" UInt) Cast(UInt Constant(ConstInt(4660) Int) UInt) Int) Return(Constant(ConstInt(15) Int)) None))\n    S(IfStmt(Binary(LogicalOr Binary(NotEqual SizeOfExp(Var(\"bi\" BitInt(12 true)) UInt) Cast(UInt Constant(ConstInt(2) Int) UInt) Int) Binary(NotEqual SizeOfExp(Var(\"sized\" Array(Int 4)) UInt) Cast(UInt Constant(ConstInt(8) Int) UInt) Int) Int) Return(Constant(ConstInt(16) Int)) None))\n    S(SwitchStmt(Cast(Int Subscript(AddressOf(Var(\"@5.buf\" Array(UChar 5)) Pointer(UChar)) Constant(ConstInt(4) Int) UChar) Int) Compound(Block([\n      S(CaseStmt(Binary(Subtract Cast(Int Var(\"@3.M\" Const(UChar)) Int) Constant(ConstInt(1) Int) Int) BreakStmt(\".switch@1\") \".case@2\"))\n      S(DefaultStmt(Return(Constant(ConstInt(17) Int)) \".default@3\"))\n    ])) \".switch@1\" [switch_case(Constant(ConstInt(4) Int) \".case@2\")] \".default@3\" Int))\n    S(SwitchStmt(Var(\"@4.SM\" Const(Int)) Compound(Block([\n      S(CaseStmt(Var(\"@4.SM\" Const(Int)) BreakStmt(\".switch@4\") \".case@5\"))\n      S(DefaultStmt(Return(Constant(ConstInt(18) Int)) \".default@6\"))\n    ])) \".switch@4\" [switch_case(Constant(ConstInt(65529) Int) \".case@5\")] \".default@6\" Int))\n    S(IfStmt(Binary(NotEqual FunctionCall(\"depth\" [Constant(ConstInt(3) Int)] Int false) Constant(ConstInt(8) Int) Int) Return(Constant(ConstInt(19) Int)) None))\n    D(VarDecl(var_decl(\"@7.pk\" AddressOf(Var(\"K\" Const(Int)) Pointer(Const(Int))) Pointer(Const(Int)) None None None None false)))\n    S(IfStmt(Binary(NotEqual Dereference(Var(\"@7.pk\" Pointer(Const(Int))) Const(Int)) Constant(ConstInt(47) Int) Int) Return(Constant(ConstInt(20) Int)) None))\n    D(VarDecl(var_decl(\"@8.x\" Constant(ConstInt(3) Int) Int None None None None false)))\n    S(SwitchStmt(Var(\"@8.x\" Int) Compound(Block([\n      S(CaseStmt(Dot(Var(\"S\" Const(Structure(\"s\"))) \"x\" Const(UChar)) BreakStmt(\".switch@7\") \".case@8\"))\n      S(DefaultStmt(Return(Constant(ConstInt(21) Int)) \".default@9\"))\n    ])) \".switch@7\" [switch_case(Constant(ConstInt(3) Int) \".case@8\")] \".default@9\" Int))\n    S(IfStmt(Binary(NotEqual Cast(Pointer(Void) Var(\"N\" Const(NullptrT)) Pointer(Void)) Cast(Pointer(Void) Constant(ConstNullptr NullptrT) Pointer(Void)) Int) Return(Constant(ConstInt(22) Int)) None))\n    S(Return(Constant(ConstInt(0) Int)))\n  ]) FunType([] Int) None None [] false None [] None false))\n])\n"}, "args": {}, "kwargs": {}})~~"})},
  };
}

}  // namespace

TEST_CASE("type_checking gives Python's results on the unit tests' calls") {
  using namespace c6502;
  const passes::StageSignature* sig = passes::find_stage("type_checking");
  REQUIRE(sig != nullptr);
  REQUIRE(sig->run != nullptr);
  const json::Value args = json::parse(R"({"args": [], "kwargs": {}})");
  for (const Case& c : cases()) {
    CAPTURE(c.input);
    capture::Call call;
    call.pass = "type_checking";
    call.input = c.input;
    std::string got;
    std::string got_mutated;
    bool raised = false;
    try {
      const passes::StageOutput out =
          sig->run(call, passes::AsmContext{}, *args.get("args"), *args.get("kwargs"));
      got = out.output;
      got_mutated = out.mutated;
    } catch (const CompileError& e) {
      raised = true;
      got = stage_checks::error_text(e.python_type(), e.what());
    }
    CHECK(raised == c.error);
    CHECK(got == c.expected);
    CHECK(got_mutated == c.mutated);
  }
}
