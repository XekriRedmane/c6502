// The calls of string lifting that the Python unit tests make, each
// recorded with Python's result: the input program's dump, and the
// output's dump or the message of the TypeCheckError that
// `normalize_initializer` raised. They are the distinct calls of
// tests/test_string_lifting.py, the smallest call of the suite that
// reaches each line of passes/string_lifting.py no call of the corpus
// reaches (`char s[] = {"abc"};`, which is `char s[] = "abc";`, and a
// call through an expression), and from the rest of the suite one call
// for each message the pass raised, since no call of the corpus raises.
#include <string>

#include "c99_ast.h"
#include "doctest.h"
#include "errors.h"
#include "passes/string_lifting.h"

namespace {

struct Case {
  const char* input;
  bool ok;
  const char* expected;
};

const Case kCases[] = {
    {R"~~(Program([
  VarDecl(var_decl("arr" String("abc") Array(Char 4) None None None None false))
])
)~~",
     true,
     R"~~(Program([
  VarDecl(var_decl("arr" String("abc") Array(Char 4) None None None None false))
])
)~~"},
    {R"~~(Program([
  VarDecl(var_decl("arr" String("abc") Array(SChar 4) None None None None false))
])
)~~",
     true,
     R"~~(Program([
  VarDecl(var_decl("arr" String("abc") Array(SChar 4) None None None None false))
])
)~~"},
    {R"~~(Program([
  VarDecl(var_decl("arr" String("abc") Array(UChar 4) None None None None false))
])
)~~",
     true,
     R"~~(Program([
  VarDecl(var_decl("arr" String("abc") Array(UChar 4) None None None None false))
])
)~~"},
    {R"~~(Program([
  VarDecl(var_decl("arr" String("abc") Array(Int 4) None None None None false))
])
)~~",
     true,
     R"~~(Program([
  VarDecl(var_decl("str@0" String("abc") Array(Char 4) Static None None None false))
  VarDecl(var_decl("arr" Var("str@0" None) Array(Int 4) None None None None false))
])
)~~"},
    {R"~~(Program([
  VarDecl(var_decl("p" AddressOf(String("abc") None) Pointer(Array(Char 4)) None None None None false))
])
)~~",
     true,
     R"~~(Program([
  VarDecl(var_decl("str@0" String("abc") Array(Char 4) Static None None None false))
  VarDecl(var_decl("p" AddressOf(Var("str@0" None) None) Pointer(Array(Char 4)) None None None None false))
])
)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("get" [] Block([
    S(Return(String("hi")))
  ]) FunType([] Pointer(Char)) None None [] false None [] None false))
])
)~~",
     true,
     R"~~(Program([
  VarDecl(var_decl("str@0" String("hi") Array(Char 3) Static None None None false))
  FunctionDecl(function_decl("get" [] Block([
    S(Return(Var("str@0" None)))
  ]) FunType([] Pointer(Char)) None None [] false None [] None false))
])
)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    S(Return(Subscript(String("abc") Constant(ConstInt(0) None) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     true,
     R"~~(Program([
  VarDecl(var_decl("str@0" String("abc") Array(Char 4) Static None None None false))
  FunctionDecl(function_decl("main" [] Block([
    S(Return(Subscript(Var("str@0" None) Constant(ConstInt(0) None) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"},
    {R"~~(Program([
  VarDecl(var_decl("a" String("first") Pointer(Char) None None None None false))
  VarDecl(var_decl("b" String("second") Pointer(Char) None None None None false))
])
)~~",
     true,
     R"~~(Program([
  VarDecl(var_decl("str@0" String("first") Array(Char 6) Static None None None false))
  VarDecl(var_decl("str@1" String("second") Array(Char 7) Static None None None false))
  VarDecl(var_decl("a" Var("str@0" None) Pointer(Char) None None None None false))
  VarDecl(var_decl("b" Var("str@1" None) Pointer(Char) None None None None false))
])
)~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("@0.p" String("hello") Pointer(Char) None None None None false)))
    S(Return(Subscript(Var("@0.p" None) Constant(ConstInt(0) None) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     true,
     R"~~(Program([
  VarDecl(var_decl("str@0" String("hello") Array(Char 6) Static None None None false))
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("@0.p" Var("str@0" None) Pointer(Char) None None None None false)))
    S(Return(Subscript(Var("@0.p" None) Constant(ConstInt(0) None) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"},
    {R"~~(Program([
  VarDecl(var_decl("t" InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None) Constant(ConstInt(3) None)] None None) Array(Int 0) None None None None false))
  VarDecl(var_decl("s" String("abc") Array(Char 0) None None None None false))
  VarDecl(var_decl("k" InitList([Constant(ConstInt(9) None) Constant(ConstInt(8) None)] None None) Array(Const(UChar) 0) None None None None false))
  VarDecl(var_decl("b" InitList([String("xy")] None None) Array(Char 0) None None None None false))
  VarDecl(var_decl("m" InitList([InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) InitList([Constant(ConstInt(3) None) Constant(ConstInt(4) None)] None None) InitList([Constant(ConstInt(5) None) Constant(ConstInt(6) None)] None None)] None None) Array(Array(Int 2) 0) None None None None false))
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("@0.u" InitList([Constant(ConstInt(4) None) Constant(ConstInt(5) None) Constant(ConstInt(6) None) Constant(ConstInt(7) None)] None None) Array(Int 0) Static None None None false)))
    D(VarDecl(var_decl("@1.v" InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) Array(Long 0) None None None None false)))
    S(Return(Binary(Add Binary(Add Binary(Add Binary(Add Binary(Add Binary(Add SizeOfExp(Var("t" None) None) SizeOfExp(Var("s" None) None) None) SizeOfExp(Var("k" None) None) None) SizeOfExp(Var("b" None) None) None) SizeOfExp(Var("m" None) None) None) SizeOfExp(Var("@0.u" None) None) None) SizeOfExp(Var("@1.v" None) None) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     true,
     R"~~(Program([
  VarDecl(var_decl("t" InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None) Constant(ConstInt(3) None)] None None) Array(Int 0) None None None None false))
  VarDecl(var_decl("s" String("abc") Array(Char 0) None None None None false))
  VarDecl(var_decl("k" InitList([Constant(ConstInt(9) None) Constant(ConstInt(8) None)] None None) Array(Const(UChar) 0) None None None None false))
  VarDecl(var_decl("b" String("xy") Array(Char 0) None None None None false))
  VarDecl(var_decl("m" InitList([InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) InitList([Constant(ConstInt(3) None) Constant(ConstInt(4) None)] None None) InitList([Constant(ConstInt(5) None) Constant(ConstInt(6) None)] None None)] None None) Array(Array(Int 2) 0) None None None None false))
  FunctionDecl(function_decl("main" [] Block([
    D(VarDecl(var_decl("@0.u" InitList([Constant(ConstInt(4) None) Constant(ConstInt(5) None) Constant(ConstInt(6) None) Constant(ConstInt(7) None)] None None) Array(Int 0) Static None None None false)))
    D(VarDecl(var_decl("@1.v" InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) Array(Long 0) None None None None false)))
    S(Return(Binary(Add Binary(Add Binary(Add Binary(Add Binary(Add Binary(Add SizeOfExp(Var("t" None) None) SizeOfExp(Var("s" None) None) None) SizeOfExp(Var("k" None) None) None) SizeOfExp(Var("b" None) None) None) SizeOfExp(Var("m" None) None) None) SizeOfExp(Var("@0.u" None) None) None) SizeOfExp(Var("@1.v" None) None) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"},
    {R"~~(Program([
  VarDecl(var_decl("x" None Int None None None None false))
  FunctionDecl(function_decl("main" [] Block([
    S(Return(IndirectCall(Dereference(AddressOf(Var("x" None) None) None) [] None false)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     true,
     R"~~(Program([
  VarDecl(var_decl("x" None Int None None None None false))
  FunctionDecl(function_decl("main" [] Block([
    S(Return(IndirectCall(Dereference(AddressOf(Var("x" None) None) None) [] None false)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~"},
    {R"~~(Program([
  VarDecl(var_decl("a" InitList([Designation([IndexDesignator(3)] Constant(ConstInt(1) None))] None None) Array(Int 3) None None None None false))
])
)~~",
     false,
     R"~~(the designator `[3]` is past the end of `a`, an array of 3 elements (C23 §6.7.11p6))~~"},
    {R"~~(Program([
  VarDecl(var_decl("a" InitList([Designation([MemberDesignator("x")] Constant(ConstInt(1) None))] None None) Array(Int 3) None None None None false))
])
)~~",
     false,
     R"~~(the designator `.x` names a member of a struct or union, and `a` has type `int[3]` (C23 §6.7.11p7))~~"},
    {R"~~(Program([
  VarDecl(var_decl("a" InitList([Designation([IndexDesignator(2)] Constant(ConstInt(1) None)) Constant(ConstInt(2) None)] None None) Array(Int 3) None None None None false))
])
)~~",
     false,
     R"~~(too many initializers for array `a`: the item for index 3 is past the end of an array of 3 elements)~~"},
    {R"~~(Program([
  VarDecl(var_decl("x" InitList([Designation([IndexDesignator(0)] Constant(ConstInt(1) None))] None None) Int None None None None false))
])
)~~",
     false,
     R"~~(`x` is a scalar of type `int`, so its initializer can't have the designator `[0]`, which names an element of an array or a member of a struct or union (C23 §6.7.11p6-7))~~"},
    {R"~~(Program([
  VarDecl(var_decl("x" InitList([Designation([MemberDesignator("x")] Constant(ConstInt(1) None))] None None) Int None None None None false))
])
)~~",
     false,
     R"~~(`x` is a scalar of type `int`, so its initializer can't have the designator `.x`, which names an element of an array or a member of a struct or union (C23 §6.7.11p6-7))~~"},
    {R"~~(Program([
  VarDecl(var_decl("a" InitList([Designation([IndexDesignator(0)] InitList([Designation([IndexDesignator(0)] Constant(ConstInt(1) None))] None None))] None None) Array(Int 2) None None None None false))
])
)~~",
     false,
     R"~~(`a[0]` is a scalar of type `int`, so its initializer can't have the designator `[0]`, which names an element of an array or a member of a struct or union (C23 §6.7.11p6-7))~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("f" [] Block([
    S(Return(CompoundLiteral("@0.compound_literal" Int InitList([Designation([MemberDesignator("x")] Constant(ConstInt(1) None))] None None) None None false)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~(`(int){...}` is a scalar of type `int`, so its initializer can't have the designator `.x`, which names an element of an array or a member of a struct or union (C23 §6.7.11p6-7))~~"},
    {R"~~(Program([
  VarDecl(var_decl("a" InitList([Designation([IndexDesignator(0) IndexDesignator(1)] Constant(ConstInt(1) None)) Constant(ConstInt(2) None)] None None) Array(Array(Int 2) 2) None None None None false))
])
)~~",
     false,
     R"~~(in the initializer of `a`, the item after `[0][1] = ...` would initialize the subobject after `a[0][1]` inside `a[0]`, which takes brace elision, and c6502 doesn't elide braces. Put the initializers of `a[0]` in braces of their own, as in `[0] = {...}`)~~"},
    {R"~~(Program([
  StructDecl(struct_decl("p" false [member_decl("x" Int) member_decl("y" Int)]))
  StructDecl(struct_decl("s" false [member_decl("n" Char) member_decl("pt" Array(Structure("p") 2)) member_decl("name" Array(Char 4))]))
  StructDecl(struct_decl("u" true [member_decl("c" Char) member_decl("l" Long) member_decl("i" Int)]))
  StructDecl(struct_decl("w" false [member_decl("tag" Int) member_decl("v" Union("u"))]))
  VarDecl(var_decl("v" InitList([Designation([MemberDesignator("x")] Constant(ConstInt(1) None))] None None) Union("u") None None None None false))
])
)~~",
     false,
     R"~~(the designator `.x` names no member of `v`, which has type `union u` (C23 §6.7.11p7))~~"},
    {R"~~(Program([
  StructDecl(struct_decl("p" false [member_decl("x" Int) member_decl("y" Int)]))
  StructDecl(struct_decl("s" false [member_decl("n" Char) member_decl("pt" Array(Structure("p") 2)) member_decl("name" Array(Char 4))]))
  StructDecl(struct_decl("u" true [member_decl("c" Char) member_decl("l" Long) member_decl("i" Int)]))
  StructDecl(struct_decl("w" false [member_decl("tag" Int) member_decl("v" Union("u"))]))
  VarDecl(var_decl("q" InitList([Designation([MemberDesignator("z")] Constant(ConstInt(1) None))] None None) Structure("p") None None None None false))
])
)~~",
     false,
     R"~~(the designator `.z` names no member of `q`, which has type `struct p` (C23 §6.7.11p7))~~"},
    {R"~~(Program([
  StructDecl(struct_decl("p" false [member_decl("x" Int) member_decl("y" Int)]))
  StructDecl(struct_decl("s" false [member_decl("n" Char) member_decl("pt" Array(Structure("p") 2)) member_decl("name" Array(Char 4))]))
  StructDecl(struct_decl("u" true [member_decl("c" Char) member_decl("l" Long) member_decl("i" Int)]))
  StructDecl(struct_decl("w" false [member_decl("tag" Int) member_decl("v" Union("u"))]))
  VarDecl(var_decl("q" InitList([Designation([IndexDesignator(0)] Constant(ConstInt(1) None))] None None) Structure("p") None None None None false))
])
)~~",
     false,
     R"~~(the designator `[0]` names an element of an array, and `q` has type `struct p` (C23 §6.7.11p6))~~"},
    {R"~~(Program([
  StructDecl(struct_decl("p" false [member_decl("x" Int) member_decl("y" Int)]))
  StructDecl(struct_decl("s" false [member_decl("n" Char) member_decl("pt" Array(Structure("p") 2)) member_decl("name" Array(Char 4))]))
  StructDecl(struct_decl("u" true [member_decl("c" Char) member_decl("l" Long) member_decl("i" Int)]))
  StructDecl(struct_decl("w" false [member_decl("tag" Int) member_decl("v" Union("u"))]))
  VarDecl(var_decl("v" InitList([Designation([MemberDesignator("n") IndexDesignator(0)] Constant(ConstInt(1) None))] None None) Structure("s") None None None None false))
])
)~~",
     false,
     R"~~(the designator `[0]` names an element of an array, and `v.n` has type `char` (C23 §6.7.11p6))~~"},
    {R"~~(Program([
  StructDecl(struct_decl("p" false [member_decl("x" Int) member_decl("y" Int)]))
  StructDecl(struct_decl("s" false [member_decl("n" Char) member_decl("pt" Array(Structure("p") 2)) member_decl("name" Array(Char 4))]))
  StructDecl(struct_decl("u" true [member_decl("c" Char) member_decl("l" Long) member_decl("i" Int)]))
  StructDecl(struct_decl("w" false [member_decl("tag" Int) member_decl("v" Union("u"))]))
  VarDecl(var_decl("q" InitList([Designation([MemberDesignator("x") MemberDesignator("y")] Constant(ConstInt(1) None))] None None) Structure("p") None None None None false))
])
)~~",
     false,
     R"~~(the designator `.y` names a member of a struct or union, and `q.x` has type `int` (C23 §6.7.11p7))~~"},
    {R"~~(Program([
  StructDecl(struct_decl("p" false [member_decl("x" Int) member_decl("y" Int)]))
  StructDecl(struct_decl("s" false [member_decl("n" Char) member_decl("pt" Array(Structure("p") 2)) member_decl("name" Array(Char 4))]))
  StructDecl(struct_decl("u" true [member_decl("c" Char) member_decl("l" Long) member_decl("i" Int)]))
  StructDecl(struct_decl("w" false [member_decl("tag" Int) member_decl("v" Union("u"))]))
  VarDecl(var_decl("v" InitList([Designation([MemberDesignator("pt") IndexDesignator(2) MemberDesignator("x")] Constant(ConstInt(1) None))] None None) Structure("s") None None None None false))
])
)~~",
     false,
     R"~~(the designator `[2]` is past the end of `v.pt`, an array of 2 elements (C23 §6.7.11p6))~~"},
    {R"~~(Program([
  StructDecl(struct_decl("p" false [member_decl("x" Int) member_decl("y" Int)]))
  StructDecl(struct_decl("s" false [member_decl("n" Char) member_decl("pt" Array(Structure("p") 2)) member_decl("name" Array(Char 4))]))
  StructDecl(struct_decl("u" true [member_decl("c" Char) member_decl("l" Long) member_decl("i" Int)]))
  StructDecl(struct_decl("w" false [member_decl("tag" Int) member_decl("v" Union("u"))]))
  VarDecl(var_decl("q" InitList([Designation([MemberDesignator("y")] Constant(ConstInt(1) None)) Constant(ConstInt(2) None)] None None) Structure("p") None None None None false))
])
)~~",
     false,
     R"~~(too many initializers for struct `q`: the struct has 2 members, and the item after the last one has no member to initialize)~~"},
    {R"~~(Program([
  StructDecl(struct_decl("p" false [member_decl("x" Int) member_decl("y" Int)]))
  StructDecl(struct_decl("s" false [member_decl("n" Char) member_decl("pt" Array(Structure("p") 2)) member_decl("name" Array(Char 4))]))
  StructDecl(struct_decl("u" true [member_decl("c" Char) member_decl("l" Long) member_decl("i" Int)]))
  StructDecl(struct_decl("w" false [member_decl("tag" Int) member_decl("v" Union("u"))]))
  VarDecl(var_decl("v" InitList([Designation([MemberDesignator("c")] Constant(ConstInt(1) None)) Constant(ConstInt(2) None)] None None) Union("u") None None None None false))
])
)~~",
     false,
     R"~~(too many initializers for union `v`: a union's list initializes one member, and an item without a designator can't follow another item (C23 §6.7.11p17))~~"},
    {R"~~(Program([
  FunctionDecl(function_decl("f" [] None FunType([] Int) None None [] false None [] None false))
  FunctionDecl(function_decl("g" [] Block([
    D(VarDecl(var_decl("@0.n" Constant(ConstInt(0) None) Int None None None None false)))
    D(VarDecl(var_decl("@1.a" InitList([Designation([IndexDesignator(0)] FunctionCall("f" [] None false)) Designation([IndexDesignator(0)] Constant(ConstInt(2) None))] None None) Array(Int 3) None None None None false)))
    S(Return(Subscript(Var("@1.a" None) Constant(ConstInt(0) None) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~(a later designator overrides the initializer of `a[0]`, which has side effects. C leaves it unspecified whether they happen (C23 §6.7.11p19), and c6502 doesn't evaluate an overridden initializer, so it rejects one with side effects)~~"},
    {R"~~(Program([
  StructDecl(struct_decl("p" false [member_decl("x" Int) member_decl("y" Int)]))
  StructDecl(struct_decl("s" false [member_decl("n" Char) member_decl("pt" Array(Structure("p") 2)) member_decl("name" Array(Char 4))]))
  StructDecl(struct_decl("u" true [member_decl("c" Char) member_decl("l" Long) member_decl("i" Int)]))
  StructDecl(struct_decl("w" false [member_decl("tag" Int) member_decl("v" Union("u"))]))
  VarDecl(var_decl("v" InitList([Designation([MemberDesignator("pt") IndexDesignator(0) MemberDesignator("x")] Constant(ConstInt(1) None)) Constant(ConstInt(2) None)] None None) Structure("s") None None None None false))
])
)~~",
     false,
     R"~~(in the initializer of `v`, the item after `.pt[0].x = ...` would initialize the subobject after `v.pt[0].x` inside `v.pt`, which takes brace elision, and c6502 doesn't elide braces. Put the initializers of `v.pt` in braces of their own, as in `.pt = {...}`)~~"},
    {R"~~(Program([
  StructDecl(struct_decl("p" false [member_decl("x" Int) member_decl("y" Int)]))
  StructDecl(struct_decl("s" false [member_decl("n" Char) member_decl("pt" Array(Structure("p") 2)) member_decl("name" Array(Char 4))]))
  StructDecl(struct_decl("u" true [member_decl("c" Char) member_decl("l" Long) member_decl("i" Int)]))
  StructDecl(struct_decl("w" false [member_decl("tag" Int) member_decl("v" Union("u"))]))
  FunctionDecl(function_decl("f" [] None FunType([] Int) None None [] false None [] None false))
  FunctionDecl(function_decl("g" [] Block([
    D(VarDecl(var_decl("@0.v" InitList([Designation([MemberDesignator("c")] FunctionCall("f" [] None false)) Designation([MemberDesignator("l")] Constant(ConstInt(2) None))] None None) Union("u") None None None None false)))
    S(Return(Dot(Var("@0.v" None) "c" None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~(a later designator overrides the initializer of `v.c`, which has side effects. C leaves it unspecified whether they happen (C23 §6.7.11p19), and c6502 doesn't evaluate an overridden initializer, so it rejects one with side effects)~~"},
    {R"~~(Program([
  StructDecl(struct_decl("p" false [member_decl("x" Int) member_decl("y" Int)]))
  StructDecl(struct_decl("s" false [member_decl("n" Char) member_decl("pt" Array(Structure("p") 2)) member_decl("name" Array(Char 4))]))
  StructDecl(struct_decl("u" true [member_decl("c" Char) member_decl("l" Long) member_decl("i" Int)]))
  StructDecl(struct_decl("w" false [member_decl("tag" Int) member_decl("v" Union("u"))]))
  VarDecl(var_decl("p0" None Structure("p") None None None None false))
  StructDecl(struct_decl("q" false [member_decl("a" Structure("p"))]))
  FunctionDecl(function_decl("g" [] Block([
    D(VarDecl(var_decl("@0.v" InitList([Designation([MemberDesignator("a")] Var("p0" None)) Designation([MemberDesignator("a") MemberDesignator("y")] Constant(ConstInt(2) None))] None None) Structure("q") None None None None false)))
    S(Return(Constant(ConstInt(0) None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~(a designator overrides part of `v.a`, which an expression initializes, and c6502 can only override part of a subobject that a braced list initializes. Initialize `v.a` with a braced list instead)~~"},
    {R"~~(Program([
  StructDecl(struct_decl("p" false [member_decl("x" Int) member_decl("y" Int)]))
  StructDecl(struct_decl("s" false [member_decl("n" Char) member_decl("pt" Array(Structure("p") 2)) member_decl("name" Array(Char 4))]))
  StructDecl(struct_decl("u" true [member_decl("c" Char) member_decl("l" Long) member_decl("i" Int)]))
  StructDecl(struct_decl("w" false [member_decl("tag" Int) member_decl("v" Union("u"))]))
  FunctionDecl(function_decl("f" [] None FunType([] Int) None None [] false None [] None false))
  FunctionDecl(function_decl("g" [] Block([
    D(VarDecl(var_decl("@0.v" InitList([Designation([MemberDesignator("pt") IndexDesignator(0)] InitList([FunctionCall("f" [] None false) Constant(ConstInt(1) None)] None None)) Designation([MemberDesignator("pt") IndexDesignator(0) MemberDesignator("x")] Constant(ConstInt(2) None))] None None) Structure("s") None None None None false)))
    S(Return(Dot(Var("@0.v" None) "n" None)))
  ]) FunType([] Int) None None [] false None [] None false))
])
)~~",
     false,
     R"~~(a later designator overrides the initializer of `v.pt[0].x`, which has side effects. C leaves it unspecified whether they happen (C23 §6.7.11p19), and c6502 doesn't evaluate an overridden initializer, so it rejects one with side effects)~~"},
};

}  // namespace

TEST_CASE("string lifting gives the Python unit tests' results") {
  for (const Case& c : kCases) {
    auto prog = c6502::asdl::load<c6502::c99_ast::Program>(c.input);
    std::string got;
    try {
      got = c6502::asdl::dump(c6502::passes::lift_strings(std::move(prog)));
    } catch (const c6502::CompileError& e) {
      CHECK(e.python_type() == "TypeCheckError");
      got = e.what();
    }
    CHECK(got == c.expected);
  }
}
