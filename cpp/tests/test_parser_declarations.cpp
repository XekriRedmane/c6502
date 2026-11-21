// The declarations parsed by part R2 of the parser (src/parser_declarations.cpp),
// each recorded with what Python's `parse` gives: the program's dump, or
// the class and message of its error. Written by a script from records
// of tools/capture_passes.py's wrapper of `parse`.
#include <string>

#include "asdl/runtime.h"
#include "c99_ast.h"
#include "doctest.h"
#include "errors.h"
#include "parser.h"

namespace {

struct Case {
  const char* source;
  bool ok;
  const char* type;
  const char* expected;
};

const Case kCases[] = {
    {R"~~(int;)~~", false, R"~~(ParserError)~~", R"~~(declaration does not declare anything (a tag, a member of an enumeration, or an object/function))~~"},
    {R"~~(int x;)~~", true, R"~~()~~", R"~~(Program([
  VarDecl(var_decl("x" None Int None None None None false))
])
)~~"},
    {R"~~(enum e;)~~", false, R"~~(ParserError)~~", R"~~(enum 'e' isn't defined. Only an enum with a fixed underlying type can be declared before its constants, as in `enum e : int;` (C23 §6.7.3.3))~~"},
    {R"~~(int int x;)~~", false, R"~~(ParserError)~~", R"~~(multiple type specifiers in a declaration (at most one 'int' is permitted))~~"},
    {R"~~(auto int x;)~~", false, R"~~(ParserError)~~", R"~~(`auto` at file scope only marks a type to infer, and this declaration has a type specifier (C23 §6.7.2p4))~~"},
    {R"~~(int f(a, b);)~~", false, R"~~(NotImplementedError)~~", R"~~(K&R-style identifier-list functions aren't supported)~~"},
    {R"~~(enum : int x;)~~", false, R"~~(ParserError)~~", R"~~(`enum e : T` without a list of constants can only be a whole declaration, `enum e : T;`, with no storage class, `typedef`, or declarator, and not in a struct, a `for`, a cast, or `sizeof` (C23 §6.7.3.3). Declare the tag that way, and then name the type as `enum e`. The parser stopped at 'x' on line 1)~~"},
    {R"~~(enum e : int;)~~", true, R"~~()~~", R"~~(Program([])
)~~"},
    {R"~~(long double d;)~~", false, R"~~(ParserError)~~", R"~~('long double' is not supported (only 'float' and 'double' are modeled today))~~"},
    {R"~~(const struct s;)~~", false, R"~~(ParserError)~~", R"~~(struct/union specifier cannot combine with another type specifier)~~"},
    {R"~~([[fallthrough]];)~~", false, R"~~(ParserError)~~", R"~~(`[[fallthrough]];` belongs in a switch statement, not at file scope)~~"},
    {R"~~(int f(int a), g;)~~", true, R"~~()~~", R"~~(Program([
  FunctionDecl(function_decl("f" ["a"] None FunType([Int] Int) None None [""] false None [] None false))
  VarDecl(var_decl("g" None Int None None None None false))
])
)~~"},
    {R"~~(static struct s;)~~", false, R"~~(ParserError)~~", R"~~(storage-class specifier isn't permitted on a struct/union declaration without a declarator)~~"},
    {R"~~(constexpr int *p;)~~", false, R"~~(ParserError)~~", R"~~(`constexpr` object `p` has no initializer, and a constexpr declaration must have one (C23 §6.7.2p5))~~"},
    {R"~~(int *p, **q, a[];)~~", true, R"~~()~~", R"~~(Program([
  VarDecl(var_decl("p" None Pointer(Int) None None None None false))
  VarDecl(var_decl("q" None Pointer(Pointer(Int)) None None None None false))
  VarDecl(var_decl("a" None Array(Int 0) None None None None false))
])
)~~"},
    {R"~~(int f(int a, ...);)~~", false, R"~~(ParserError)~~", R"~~(variadic functions ('...') aren't supported yet)~~"},
    {R"~~(int f(void)(void);)~~", false, R"~~(ParserError)~~", R"~~(function declarator cannot specify a function-typed return (C99 §6.7.5.3.1))~~"},
    {R"~~(int f(void x, int);)~~", true, R"~~()~~", R"~~(Program([
  FunctionDecl(function_decl("f" ["x" ""] None FunType([Void Int] Int) None None ["" ""] false None [] None false))
])
)~~"},
    {R"~~(_Thread_local int t;)~~", false, R"~~(ParserError)~~", R"~~(`_Thread_local` gives each thread its own object, and c6502 has no threads, so it doesn't take `_Thread_local`)~~"},
    {R"~~([[gnu::unused]] int g1;)~~", true, R"~~()~~", R"~~(Program([
  VarDecl(var_decl("g1" None Int None None None None false))
])
)~~"},
    {R"~~(struct s; enum s : int;)~~", false, R"~~(ParserError)~~", R"~~('s' is a struct or union tag in this scope, so it can't be an enum tag too)~~"},
    {R"~~([[zp_abi]] int zf(void);)~~", false, R"~~(ParserError)~~", R"~~(`zp_abi` is a c6502 attribute, so it needs c6502's prefix: `[[c6502::zp_abi]]`)~~"},
    {R"~~(int f(int (x), int (T));)~~", true, R"~~()~~", R"~~(Program([
  FunctionDecl(function_decl("f" ["x" "T"] None FunType([Int Int] Int) None None ["" ""] false None [] None false))
])
)~~"},
    {R"~~([[c6502::packed]] int g1;)~~", false, R"~~(ParserError)~~", R"~~(`[[c6502::packed]]` was removed. C23's fixed underlying type gives an enum its size: `enum e : uint8_t { ... }` is one byte (C23 §6.7.3.3))~~"},
    {R"~~(struct s { int f(void); };)~~", false, R"~~(ParserError)~~", R"~~(struct/union member 'f' cannot have function type)~~"},
    {R"~~(int x [[deprecated("no")]];)~~", true, R"~~()~~", R"~~(Program([
  VarDecl(var_decl("x" None Int None None None None false))
])
)~~"},
    {R"~~(struct s { int a; int a; };)~~", false, R"~~(ParserError)~~", R"~~(duplicate member 'a' in struct/union 's')~~"},
    {R"~~(typedef int T; int T2; T T;)~~", false, R"~~(ParserError)~~", R"~~('T' is declared both as a typedef name and as an object or function in the same scope)~~"},
    {R"~~([[noreturn]] void die(void);)~~", true, R"~~()~~", R"~~(Program([
  FunctionDecl(function_decl("die" [] None FunType([] Void) None None [] false None [] None true))
])
)~~"},
    {R"~~(enum e : int; enum e : long;)~~", false, R"~~(ParserError)~~", R"~~(enum 'e' was declared with the underlying type `int`, and here with `long`, which isn't compatible with it (C23 §6.7.3.3))~~"},
    {R"~~(typedef struct q *QP; QP qp;)~~", true, R"~~()~~", R"~~(Program([
  StructDecl(struct_decl("q" false []))
  VarDecl(var_decl("qp" None Pointer(Structure("q")) None None None None false))
])
)~~"},
    {R"~~([[c6502::nope]] int zf(void);)~~", false, R"~~(ParserError)~~", R"~~(unknown attribute `c6502::nope`; c6502's attributes are address, keep, preserves_x, preserves_y, return_bool_in_carry, save_hardware_stack, section, unroll, zeropage, zp_abi, and the parameter registers `in_a`, `in_x`, `in_y`, `in_ay`, and so on)~~"},
    {R"~~(int f(struct s { int a; } x);)~~", false, R"~~(ParserError)~~", R"~~(struct/union definition isn't permitted in a parameter declaration)~~"},
    {R"~~(int x __attribute__((unused));)~~", false, R"~~(ParserError)~~", R"~~(Unexpected token Token('IDENTIFIER', '__attribute__') at line 1, column 7.
Expected one of: 
	* ASSIGN
	* ATTR_OPEN
	* COMMA
	* LBRACE
	* LBRACKET
	* LPAREN
	* SEMICOLON

c6502 doesn't take GCC's `__attribute__((...))`; it takes C23 attributes, such as `[[c6502::zp_abi]]` before a declaration or `uint8_t n [[c6502::in_x]]` after a name)~~"},
    {R"~~(typedef int T; int g(int (T));)~~", true, R"~~()~~", R"~~(Program([
  FunctionDecl(function_decl("g" [""] None FunType([Pointer(FunType([Int] Int))] Int) None None [""] false None [] None false))
])
)~~"},
    {R"~~([[c6502::zp_abi]] int zf(void);)~~", true, R"~~()~~", R"~~(Program([
  FunctionDecl(function_decl("zf" [] None FunType([] Int) None "zp_abi" [] false None [] None false))
])
)~~"},
    {R"~~(struct s { int a; } ; struct s;)~~", true, R"~~()~~", R"~~(Program([
  StructDecl(struct_decl("s" false [member_decl("a" Int)]))
  StructDecl(struct_decl("s" false []))
])
)~~"},
    {R"~~(struct { int a; } anon1, anon2;)~~", true, R"~~()~~", R"~~(Program([
  StructDecl(struct_decl("__anonymous_struct_1" false [member_decl("a" Int)]))
  VarDecl(var_decl("anon1" None Structure("__anonymous_struct_1") None None None None false))
  VarDecl(var_decl("anon2" None Structure("__anonymous_struct_1") None None None None false))
])
)~~"},
    {R"~~([[nodiscard("x")]] int nf(void);)~~", true, R"~~()~~", R"~~(Program([
  FunctionDecl(function_decl("nf" [] None FunType([] Int) None None [] false None [] None false))
])
)~~"},
    {R"~~(void (*fp)(int), (*fpa[])(void);)~~", true, R"~~()~~", R"~~(Program([
  VarDecl(var_decl("fp" None Pointer(FunType([Int] Void)) None None None None false))
  VarDecl(var_decl("fpa" None Array(Pointer(FunType([] Void)) 0) None None None None false))
])
)~~"},
    {R"~~(int (*pick(int w))(int a, int b);)~~", true, R"~~()~~", R"~~(Program([
  FunctionDecl(function_decl("pick" ["w"] None FunType([Int] Pointer(FunType([Int Int] Int))) None None [""] false None [] None false))
])
)~~"},
    {R"~~(typedef int F(int); F ff; F *fpp;)~~", true, R"~~()~~", R"~~(Program([
  FunctionDecl(function_decl("ff" [""] None FunType([Int] Int) None None [""] false None [] None false))
  VarDecl(var_decl("fpp" None Pointer(FunType([Int] Int)) None None None None false))
])
)~~"},
    {R"~~(int * const * pc; int *volatile pv;)~~", true, R"~~()~~", R"~~(Program([
  VarDecl(var_decl("pc" None Pointer(Const(Pointer(Int))) None None None None false))
  VarDecl(var_decl("pv" None Volatile(Pointer(Int)) None None None None false))
])
)~~"},
    {R"~~(typedef int T; T y; T *z; int m(T);)~~", true, R"~~()~~", R"~~(Program([
  VarDecl(var_decl("y" None Int None None None None false))
  VarDecl(var_decl("z" None Pointer(Int) None None None None false))
  FunctionDecl(function_decl("m" [""] None FunType([Int] Int) None None [""] false None [] None false))
])
)~~"},
    {R"~~(struct a { struct b { int c; } d; };)~~", false, R"~~(ParserError)~~", R"~~(nested struct/union definition inside a member isn't supported — declare the inner type at outer scope first)~~"},
    {R"~~([[maybe_unused, maybe_unused]] int mm;)~~", false, R"~~(ParserError)~~", R"~~(`maybe_unused` appears twice in one attribute list)~~"},
    {R"~~(int f(void); int g(int a, char *b); int h();)~~", true, R"~~()~~", R"~~(Program([
  FunctionDecl(function_decl("f" [] None FunType([] Int) None None [] false None [] None false))
  FunctionDecl(function_decl("g" ["a" "b"] None FunType([Int Pointer(Char)] Int) None None ["" ""] false None [] None false))
  FunctionDecl(function_decl("h" [] None FunType([] Int) None None [] false None [] None false))
])
)~~"},
    {R"~~(typedef struct s { int a; char *b; } S; S v;)~~", true, R"~~()~~", R"~~(Program([
  StructDecl(struct_decl("s" false [member_decl("a" Int) member_decl("b" Pointer(Char))]))
  VarDecl(var_decl("v" None Structure("s") None None None None false))
])
)~~"},
    {R"~~(int k(int (*)(int), int [], void (*cb)(void));)~~", true, R"~~()~~", R"~~(Program([
  FunctionDecl(function_decl("k" ["" "" "cb"] None FunType([Pointer(FunType([Int] Int)) Pointer(Int) Pointer(FunType([] Void))] Int) None None ["" "" ""] false None [] None false))
])
)~~"},
    {R"~~(static int s1; extern int e1; register int r1;)~~", true, R"~~()~~", R"~~(Program([
  VarDecl(var_decl("s1" None Int Static None None None false))
  VarDecl(var_decl("e1" None Int Extern None None None false))
  VarDecl(var_decl("r1" None Int Register None None None false))
])
)~~"},
    {R"~~([[deprecated]] int dep; int q [[maybe_unused]];)~~", true, R"~~()~~", R"~~(Program([
  VarDecl(var_decl("dep" None Int None None None None false))
  VarDecl(var_decl("q" None Int None None None None false))
])
)~~"},
    {R"~~([[c6502::section(".text boot")]] void boot(void);)~~", false, R"~~(ParserError)~~", R"~~(section name '.text boot' on function declaration 'boot' has to be letters, digits, `_`, and `.`, not starting with a digit)~~"},
    {R"~~([[c6502::section(".text.boot")]] void boot(void);)~~", true, R"~~()~~", R"~~(Program([
  FunctionDecl(function_decl("boot" [] None FunType([] Void) None None [] false None [] ".text.boot" false))
])
)~~"},
    {R"~~(struct p; struct p *pp; union u { int i; long l; } uu;)~~", true, R"~~()~~", R"~~(Program([
  StructDecl(struct_decl("p" false []))
  VarDecl(var_decl("pp" None Pointer(Structure("p")) None None None None false))
  StructDecl(struct_decl("u" true [member_decl("i" Int) member_decl("l" Long)]))
  VarDecl(var_decl("uu" None Union("u") None None None None false))
])
)~~"},
    {R"~~(typedef struct s S; typedef struct s S; typedef int S;
)~~", false, R"~~(ParserError)~~", R"~~(typedef 'S' is declared again with a different type)~~"},
    {R"~~(int f(register int a [[c6502::in_y]], int b [[c6502::in_xy]]);)~~", true, R"~~()~~", R"~~(Program([
  FunctionDecl(function_decl("f" ["a" "b"] None FunType([Int Int] Int) None None ["Y" "XY"] false None [] None false))
])
)~~"},
    {R"~~(unsigned long long u; signed char c; _Bool b; bool b2; const volatile int cv;)~~", true, R"~~()~~", R"~~(Program([
  VarDecl(var_decl("u" None ULongLong None None None None false))
  VarDecl(var_decl("c" None SChar None None None None false))
  VarDecl(var_decl("b" None Bool None None None None false))
  VarDecl(var_decl("b2" None Bool None None None None false))
  VarDecl(var_decl("cv" None Const(Volatile(Int)) None None None None false))
])
)~~"},
    {R"~~(int * const * volatile p; unsigned n = sizeof(char * const *);)~~", true, R"~~()~~", R"~~(Program([
  VarDecl(var_decl("p" None Volatile(Pointer(Const(Pointer(Int)))) None None None None false))
  VarDecl(var_decl("n" SizeOfType(Pointer(Const(Pointer(Char))) None) UInt None None None None false))
])
)~~"},
    {R"~~(int h(int x[const]);)~~", false, R"~~(NotImplementedError)~~", R"~~(type-qualified array sizes aren't supported)~~"},
    {R"~~(int a[*"ab" + 1];)~~", false, R"~~(NotImplementedError)~~", R"~~(array size must be an integer constant expression: expression is not an integer constant expression: Dereference(exp=AddressOf(exp=String(str='ab'), data_type=Pointer(referenced_type=Char())), data_type=Char()))~~"},
    {R"~~(int a[*(int*)0 + 1];)~~", false, R"~~(NotImplementedError)~~", R"~~(array size must be an integer constant expression: expression is not an integer constant expression: Dereference(exp=Cast(target_type=Pointer(referenced_type=Int()), exp=Constant(const=ConstInt(value=0), data_type=Int()), data_type=Pointer(referenced_type=Int())), data_type=Int()))~~"},
};

}  // namespace

TEST_CASE("the declarations parse as Python parses them") {
  for (const Case& c : kCases) {
    CAPTURE(c.source);
    try {
      const std::string got = c6502::asdl::dump(c6502::parser::parse(c.source));
      CHECK(c.ok);
      CHECK(got == c.expected);
    } catch (const c6502::CompileError& e) {
      CHECK_FALSE(c.ok);
      CHECK(e.python_type() == c.type);
      CHECK(std::string(e.what()) == c.expected);
    }
  }
}
