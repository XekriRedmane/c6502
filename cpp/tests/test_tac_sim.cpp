// Tests of the TAC interpreter (src/tac_sim.h).
//
// The first cases are tests/test_tac_sim.py's: each compiles a C
// program with the front end and `c99_to_tac`, without the
// preprocessor, as that module's `_compile_to_tac` does, and runs it.
// The cases after them build TAC by hand and reach each place where
// tac_sim.py raises, with the message Python gave for the same TAC
// (each was printed by running tac_sim.py on it). tac_sim.py also raises
// for a constant, a static initializer, an instruction, an operand, or
// an integer operator of a class it doesn't know, which the C++ IR's
// closed variants can't hold, so those have no case.
#include <optional>
#include <string>
#include <vector>

#include "c99_to_tac.h"
#include "doctest.h"
#include "fp_arith.h"
#include "parser.h"
#include "passes/identifier_resolution.h"
#include "passes/label_resolution.h"
#include "passes/loop_labeling.h"
#include "passes/string_lifting.h"
#include "passes/type_checker.h"
#include "tac_sim.h"

namespace {

namespace ir = c6502::tac_ast;
namespace c99 = c6502::c99_ast;
namespace passes = c6502::passes;
using c6502::asdl::Int;
using c6502::tac_sim::SimError;
using c6502::tac_sim::Simulator;

// A program's TAC with the tables it was translated with, which a
// `Simulator` must not outlive.
struct Compiled {
  ir::Program tac;
  passes::SymbolTable symbols;
  passes::TypeTable types;
};

Compiled compile_to_tac(const std::string& source) {
  c99::Program prog = c6502::parser::parse(source);
  prog = passes::label_program(passes::resolve_labels(
      passes::lift_strings(passes::resolve_identifiers(std::move(prog)))));
  auto tables = passes::type_checking::check_program(prog);
  Compiled out{.symbols = std::move(tables.symbols), .types = std::move(tables.types)};
  out.tac = c6502::c99_to_tac::translate_program(prog, out.symbols, &out.types);
  return out;
}

std::optional<Int> run(const std::string& source, const std::string& fn = "main",
                       const std::vector<Int>& args = {}) {
  const Compiled c = compile_to_tac(source);
  Simulator sim(c.tac, c.symbols, &c.types);
  return sim.call(fn, args);
}

Int f32(const char* s) { return c6502::fp_arith::single_string_to_bits(s); }
Int f64(const char* s) { return c6502::fp_arith::double_string_to_bits(s); }

}  // namespace

TEST_CASE("tac_sim: integers and control flow") {
  CHECK(run("int main(void) { return 1 + 2 * 3; }") == Int(7));
  // c99_to_tac reads and writes a named volatile scalar with
  // IndexedLoad and IndexedSymbolStore at index 0, for a local as well
  // as a static.
  CHECK(run("volatile int g = 4;\n"
            "int main(void) {\n"
            "  volatile unsigned char x = 5;\n"
            "  x++; x += 2; g *= x;\n"
            "  return g;\n"
            "}\n") == Int(32));
  // C99 §6.5.5p6: the quotient truncates toward zero.
  CHECK(run("int main(void) { return -7 / 2; }") == Int(-3));
  CHECK(run("int main(void) { return -7 % 2; }") == Int(-1));
  CHECK(run("long main(void) { long a = 2000000000; return a + a; }") == Int(-294967296));
  CHECK(run("int main(void) { unsigned int a = 200u; unsigned int b = 1u; return a > b; }") ==
        Int(1));
  // As a 2-byte `int`, 50000 is -15536, which is less than 1.
  CHECK(run("int main(void) { int a = 50000; int b = 1; return a > b; }") == Int(0));
  CHECK(run("int main(void) { long x = 5; if (x > 3) return 11; else return 22; }") == Int(11));
  CHECK(run("int main(void) { long sum = 0; long i = 0;"
            " while (i < 10) { sum = sum + i; i = i + 1; } return sum; }") == Int(45));
  CHECK(run("long fact(long n) { if (n < 2) return 1; return n * fact(n - 1); }\n"
            "long main(void) { return fact(7); }") == Int(5040));
  CHECK(run("long add(long a, long b) { return a + b; }\n"
            "int main(void) { return add(40, 2); }") == Int(42));
  CHECK(run("int main(void) { long sum = 0; long i = 0; while (i < 100) { i = i + 1;"
            " if (i > 7) break; if ((i & 1) == 0) continue; sum = sum + i; } return sum; }") ==
        Int(16));
  CHECK(run("long main(void) { int x = -1; unsigned long y = (unsigned long) x; return y; }") ==
        Int(-1));
  CHECK(run("int main(void) { long x = 305419896L; return (int) x; }") == Int(0x5678));
}

TEST_CASE("tac_sim: memory") {
  CHECK(run("int main(void) { int x = 7; int *p = &x; return *p; }") == Int(7));
  CHECK(run("int main(void) { int x = 1; int *p = &x; *p = 42; return x; }") == Int(42));
  CHECK(run("long main(void) { long a[4]; a[0] = 10; a[1] = 20; a[2] = 30; a[3] = 40;"
            " return a[2]; }") == Int(30));
  CHECK(run("long sum(void) { long a[5] = {1, 2, 3, 4, 5}; long s = 0; long i = 0;"
            " while (i < 5) { s = s + a[i]; i = i + 1; } return s; }\n"
            "long main(void) { return sum(); }") == Int(15));
  CHECK(run("long main(void) { long a[3] = {100, 200, 300}; long *p = a; return *(p + 2); }") ==
        Int(300));
  {
    const Compiled c = compile_to_tac("int g = 99;\nint main(void) { return g; }\n");
    Simulator sim(c.tac, c.symbols, &c.types);
    CHECK(sim.call("main", {}) == Int(99));
    CHECK(sim.read_static("g") == Int(99));
  }
  CHECK(run("long g;\nlong main(void) { return g; }") == Int(0));
  {
    // A static keeps its value from one call to the next.
    const Compiled c = compile_to_tac(
        "int counter = 0;\n"
        "int bump(void) { counter = counter + 1; return counter; }\n"
        "int main(void) { return 0; }\n");
    Simulator sim(c.tac, c.symbols, &c.types);
    CHECK(sim.call("bump", {}) == Int(1));
    CHECK(sim.call("bump", {}) == Int(2));
    CHECK(sim.call("bump", {}) == Int(3));
    CHECK(sim.read_static("counter") == Int(3));
  }
  {
    const Compiled c = compile_to_tac(
        "int next(void) { static int n = 10; n = n + 1; return n; }\n"
        "int main(void) { return 0; }\n");
    Simulator sim(c.tac, c.symbols, &c.types);
    CHECK(sim.call("next", {}) == Int(11));
    CHECK(sim.call("next", {}) == Int(12));
  }
  // `int *q = &g;` at file scope is an AddressInit of `g`'s address.
  CHECK(run("int g = 77;\nint *q = &g;\nint main(void) { return *q; }") == Int(77));
  CHECK(run("int main(void) { char *s = \"hello\"; return s[1]; }") == Int('e'));
  // An address-taken parameter lives in memory from the frame's start.
  CHECK(run("long deref(long x) { long *p = &x; return *p + 1; }\n"
            "long main(void) { return deref(41); }") == Int(42));
}

TEST_CASE("tac_sim: floating point") {
  CHECK(run("float main(void) { return 1.0f + 2.0f; }") == f32("3.0"));
  CHECK(run("float main(void) { return 3.0f * 0.5f; }") == f32("1.5"));
  CHECK(run("double main(void) { return 7.0 / 2.0; }") == f64("3.5"));
  CHECK(run("float main(void) { return 5.5f - 2.25f; }") == f32("3.25"));
  CHECK(run("float main(void) { float x = 1.5f; return -x; }") == f32("-1.5"));
  CHECK(run("int main(void) { float a = 1.5f; float b = 2.5f; return a < b; }") == Int(1));
  CHECK(run("int main(void) { float a = 1.5f; float b = 2.5f; return a > b; }") == Int(0));
  CHECK(run("int main(void) { float pz = 0.0f; float nz = -0.0f; return pz == nz; }") == Int(1));
  CHECK(run("double main(void) { int x = 42; return (double)x; }") == f64("42.0"));
  CHECK(run("int main(void) { double x = 3.7; return (int)x; }") == Int(3));
  CHECK(run("int main(void) { double x = -3.7; return (int)x; }") == Int(-3));
  CHECK(run("double main(void) { float x = 1.5f; return (double)x; }") == f64("1.5"));
  CHECK(run("float main(void) { double x = 1.5; return (float)x; }") == f32("1.5"));
  CHECK(run("double main(void) { unsigned long x = 65535u; return (double)x; }") ==
        f64("65535.0"));
  CHECK(run("double main(void) { int x = -1; return (double)x; }") == f64("-1.0"));
  // -0.0 compares equal to 0, so it is false (C99 §6.3.1.2).
  CHECK(run("int main(void) { float x = -0.0f; return !x; }") == Int(1));
  CHECK(run("int main(void) { float x = 1.5f; if (x) return 11; else return 22; }") == Int(11));
  {
    const Compiled c =
        compile_to_tac("float g = 2.5f;\nint main(void) { g = g + 0.5f; return 0; }\n");
    Simulator sim(c.tac, c.symbols, &c.types);
    sim.call("main", {});
    CHECK(sim.read_static("g") == f32("3.0"));
  }
  CHECK(run("double pi = 3.14;\ndouble main(void) { return pi; }") == f64("3.14"));
}

TEST_CASE("tac_sim: calls through pointers") {
  CHECK(run("int twice(int x) { return x + x; }\n"
            "int main(void) { int (*fp)(int) = &twice; return fp(21); }") == Int(42));
  CHECK(run("int add1(int x) { return x + 1; }\n"
            "int add100(int x) { return x + 100; }\n"
            "int main(void) { int (*fp)(int); fp = &add1; int a = fp(5);"
            " fp = &add100; int b = fp(5); return a + b; }") == Int(111));
  CHECK(run("int counter = 0;\n"
            "void bump(void) { counter = counter + 7; }\n"
            "int main(void) { void (*fp)(void) = &bump; fp(); fp(); return counter; }") ==
        Int(14));
  CHECK(run("long big(long x) { return x * x; }\n"
            "long main(void) { long (*fp)(long) = &big; return fp(100); }") == Int(10000));
  CHECK(run("long square(long x) { return x * x; }\n"
            "long (*fp)(long) = &square;\n"
            "long main(void) { return fp(7); }") == Int(49));
}

TEST_CASE("tac_sim: structs and unions") {
  CHECK(run("struct point { int x; int y; };\n"
            "struct point make_point(void) { struct point p; p.x = 3; p.y = 4; return p; }\n"
            "int main(void) { struct point q = make_point(); return q.x + q.y; }") == Int(7));
  CHECK(run("struct point { int x; int y; };\n"
            "int sum_pt(struct point p) { return p.x + p.y; }\n"
            "int main(void) { struct point q; q.x = 10; q.y = 32; return sum_pt(q); }") ==
        Int(42));
  // The callee has its own copy.
  CHECK(run("struct box { long v; };\n"
            "long zero_it(struct box b) { b.v = 0; return b.v; }\n"
            "long main(void) { struct box k; k.v = 99; zero_it(k); return k.v; }") == Int(99));
  CHECK(run("struct vec { long a; long b; long c; };\n"
            "long main(void) { struct vec u; u.a = 1; u.b = 2; u.c = 3; struct vec v;"
            " v = u; v.a = 100; return u.a + v.a + v.b + v.c; }") == Int(106));
  CHECK(run("struct ref { int *p; };\n"
            "int main(void) { int x = 17; struct ref r; r.p = &x; *r.p = 99; return x; }") ==
        Int(99));
  CHECK(run("struct inner { int a; int b; };\n"
            "struct outer { struct inner i; int c; };\n"
            "int main(void) { struct outer o; o.i.a = 1; o.i.b = 2; o.c = 3;"
            " return o.i.a + o.i.b + o.c; }") == Int(6));
  CHECK(run("struct pair { int x; int y; };\n"
            "struct pair make(int a, int b) { struct pair p; p.x = a; p.y = b; return p; }\n"
            "int sum(struct pair p) { return p.x + p.y; }\n"
            "int main(void) { return sum(make(20, 22)); }") == Int(42));
  CHECK(run("union u { int b; long w; };\n"
            "long main(void) { union u v; v.w = 0; v.b = 5; return v.w; }") == Int(5));
}

TEST_CASE("tac_sim: a recursion deeper than Python's limit runs") {
  CHECK(run("int f(int n) { if (n == 0) return 0; return 1 + f(n - 1); }\n"
            "int main(void) { return f(5000); }") == Int(5000));
  CHECK_THROWS_WITH_AS(run("int f(int n) { return f(n + 1); }\nint main(void) { return f(0); }"),
                       "maximum recursion depth exceeded", SimError);
}

namespace {

// Hand-built TAC for the raise sites.

passes::Symbol fun(c99::Type_data_type ret = c99::Int{}) {
  return {.type = c99::FunType{.params = {}, .ret = std::move(ret)},
          .attrs = passes::FunAttr{.defined = true, .is_global = true}};
}

passes::Symbol local(c99::Type_data_type t) {
  return {.type = std::move(t), .attrs = passes::LocalAttr{}};
}

ir::Type_val I(int v) { return ir::Constant{.const_ = ir::ConstInt{.value = Int(v)}}; }
ir::Type_val F(std::uint32_t bits) {
  return ir::Constant{.const_ = ir::ConstFloat{.bits = Int(bits)}};
}
ir::Type_val V(const char* n) { return ir::Var{.name = n}; }
ir::Type_instruction R(std::optional<ir::Type_val> v = std::nullopt) { return ir::Ret{.val = v}; }

ir::Type_top_level main_fn(std::vector<ir::Type_instruction> ins,
                           std::vector<std::string> params = {}) {
  return ir::Function{.name = "main", .is_global = true, .params = std::move(params),
                      .instructions = std::move(ins)};
}

ir::Type_top_level static_var(const char* name, std::vector<ir::Type_static_init> init) {
  return ir::StaticVariable{.name = name, .data_type = ir::Int{}, .init = std::move(init)};
}

// Runs `main` and returns `<type>: <message>` of what it raised, or
// `value <v>`.
std::string outcome(std::vector<ir::Type_top_level> top,
                    std::vector<std::pair<std::string, passes::Symbol>> syms,
                    const passes::TypeTable* types = nullptr) {
  passes::SymbolTable table;
  for (auto& [name, sym] : syms) table.insert_or_assign(name, sym);
  const ir::Program prog{.top_level = std::move(top)};
  const auto o = c6502::tac_sim::run_main(prog, table, types);
  if (o.error_type) return *o.error_type + ": " + o.error_message;
  return "value " + (o.value ? c6502::asdl::int_to_string(*o.value) : std::string("None"));
}

}  // namespace

TEST_CASE("tac_sim: the messages of each raise site") {
  const c99::Type_data_type s = c99::Structure{.tag = "s"};
  CHECK(outcome({main_fn({ir::Copy{.src = I(1), .dst = V("x")}, R(I(0))})},
                {{"main", fun()}, {"x", local(c99::Bool{})}}) ==
        "TacSimError: unsupported type in simulator: Bool");
  CHECK(outcome({main_fn({ir::Copy{.src = V("a"), .dst = V("b")}, R(I(0))})},
                {{"main", fun()}, {"a", local(s)}, {"b", local(s)}}) ==
        "TacSimError: cannot size aggregate type without TypeTable: Structure(tag='s')");
  passes::TypeTable tt;
  tt.insert_or_assign("s", passes::StructLayout{.tag = "s"});
  CHECK(outcome({main_fn({ir::Copy{.src = V("a"), .dst = V("b")}, R(I(0))})},
                {{"main", fun()}, {"a", local(s)}, {"b", local(s)}}, &tt) ==
        "TacSimError: incomplete aggregate type: 's'");
  CHECK(outcome({static_var("p", {ir::AddressInit{.name = "nowhere"}}), main_fn({R(I(0))})},
                {{"main", fun()}}) == "TacSimError: AddressInit references unknown 'nowhere'");
  CHECK(outcome({static_var("p", {ir::AddressByteInit{.name = "nowhere", .high = true}}),
                 main_fn({R(I(0))})},
                {{"main", fun()}}) ==
        "TacSimError: AddressByteInit references unknown 'nowhere'");
  CHECK(outcome({main_fn({ir::FunctionCall{.name = "f"}, R(I(0))})}, {{"main", fun()}}) ==
        "TacSimError: unknown function: 'f'");
  CHECK(outcome({main_fn({R(I(0))}, {"a"})}, {{"main", fun()}, {"a", local(c99::Int{})}}) ==
        "TacSimError: main: expected 1 args, got 0");
  {
    passes::SymbolTable table;
    table.insert_or_assign("main", fun());
    table.insert_or_assign("x", local(c99::Int{}));
    const ir::Program prog{.top_level = {main_fn({R(I(0))})}};
    Simulator sim(prog, table, nullptr);
    CHECK_THROWS_WITH_AS(sim.read_static("x"), "not a static variable: 'x'", SimError);
  }
  CHECK(outcome({main_fn({ir::Copy{.src = I(1), .dst = V("x")}})},
                {{"main", fun()}, {"x", local(c99::Int{})}}) ==
        "TacSimError: main: ran past end without Ret");
  CHECK(outcome({main_fn({ir::IndexedLoad{.name = "g", .index = I(0), .dst = V("x")}, R(I(0))})},
                {{"main", fun()}, {"x", local(c99::Int{})}}) ==
        "TacSimError: IndexedLoad on unknown static 'g'");
  CHECK(outcome({main_fn({ir::IndexedSymbolStore{.name = "g", .index = I(0), .src = I(1)},
                          R(I(0))})},
                {{"main", fun()}}) == "TacSimError: IndexedSymbolStore on unknown static 'g'");
  CHECK(outcome({main_fn({ir::IndirectCall{.ptr = I(0x1234)}, R(I(0))})}, {{"main", fun()}}) ==
        "TacSimError: IndirectCall to unknown address 0x1234");
  const ir::Type_instruction phi =
      ir::Phi{.dst = V("x"), .args = {ir::PhiArg{.pred_label = "L1", .source = I(1)}}};
  CHECK(outcome({main_fn({phi, R(V("x"))})}, {{"main", fun()}, {"x", local(c99::Int{})}}) ==
        "TacSimError: Phi for x has no argument from None");
  CHECK(outcome({main_fn({ir::Label{.name = "L0"}, ir::Label{.name = "L2"}, phi, R(V("x"))})},
                {{"main", fun()}, {"x", local(c99::Int{})}}) ==
        "TacSimError: Phi for x has no argument from 'L0'");
  CHECK(outcome({main_fn({ir::Label{.name = "L1"}, ir::Label{.name = "L2"}, phi, R(V("x"))})},
                {{"main", fun()}, {"x", local(c99::Int{})}}) == "value 1");
  const c99::Type_data_type p = c99::Pointer{.referenced_type = c99::Int{}};
  CHECK(outcome({main_fn({ir::GetAddress{.operand = I(1), .dst = V("p")}, R(I(0))})},
                {{"main", fun()}, {"p", local(p)}}) ==
        "TacSimError: GetAddress operand must be a Var, got Constant(const=ConstInt(value=1))");
  CHECK(outcome({main_fn({ir::GetAddress{.operand = V("q"), .dst = V("p")}, R(I(0))})},
                {{"main", fun()}, {"p", local(p)}}) == "TacSimError: unknown var: 'q'");
  CHECK(outcome({main_fn({R(V("x"))})}, {{"main", fun()}, {"x", local(c99::Int{})}}) ==
        "TacSimError: uninitialized var: x");
  CHECK(outcome({main_fn({ir::Copy{.src = I(1), .dst = I(2)}, R(I(0))})}, {{"main", fun()}}) ==
        "TacSimError: can only write to Var, got Constant");
  CHECK(outcome({main_fn({ir::Load{.src_ptr = I(0x1000), .dst = I(2)}, R(I(0))})},
                {{"main", fun()}}) == "TacSimError: expected Var as dst, got Constant");
  const ir::Type_val f1 = F(0x3F800000);
  CHECK(outcome({main_fn({ir::Unary{.op = ir::Complement{}, .src = f1, .dst = V("f")}, R(I(0))})},
                {{"main", fun()}, {"f", local(c99::Float{})}}) ==
        "TacSimError: Complement is not defined for FP types");
  CHECK(outcome({main_fn({ir::Unary{.op = ir::BinToBcd{}, .src = f1, .dst = V("f")}, R(I(0))})},
                {{"main", fun()}, {"f", local(c99::Float{})}}) ==
        "TacSimError: unsupported FP unary op: BinToBcd");
  CHECK(outcome({main_fn({ir::Binary{.op = ir::Divide{}, .src1 = I(1), .src2 = I(0), .dst = V("x")},
                          R(V("x"))})},
                {{"main", fun()}, {"x", local(c99::Int{})}}) == "TacSimError: division by zero");
  CHECK(outcome({main_fn({ir::Binary{.op = ir::Modulo{}, .src1 = I(1), .src2 = I(0), .dst = V("x")},
                          R(V("x"))})},
                {{"main", fun()}, {"x", local(c99::Int{})}}) == "TacSimError: modulo by zero");
  CHECK(outcome({main_fn({ir::Binary{.op = ir::Modulo{}, .src1 = f1, .src2 = f1, .dst = V("f")},
                          R(I(0))})},
                {{"main", fun()}, {"f", local(c99::Float{})}}) ==
        "TacSimError: unsupported FP binary op: Modulo");
}

TEST_CASE("tac_sim: Python's other exceptions") {
  CHECK(outcome({main_fn({R(V("nope"))})}, {{"main", fun()}}) == "KeyError: 'nope'");
  CHECK(outcome({main_fn({ir::Jump{.target = "L9"}, R(I(0))})}, {{"main", fun()}}) ==
        "KeyError: 'L9'");
  CHECK(outcome({main_fn({ir::FloatToInt{.src = F(0x7FC00000), .dst = V("x")}, R(V("x"))})},
                {{"main", fun()}, {"x", local(c99::Int{})}}) ==
        "ValueError: cannot convert float NaN to integer");
  CHECK(outcome({main_fn({ir::FloatToInt{.src = F(0x7F800000), .dst = V("x")}, R(V("x"))})},
                {{"main", fun()}, {"x", local(c99::Int{})}}) ==
        "OverflowError: cannot convert float infinity to integer");
  CHECK(outcome({main_fn({ir::IndexedLoad{.name = "x", .index = F(0x3F800000), .dst = V("y")},
                          R(I(0))})},
                {{"main", fun()}, {"x", local(c99::Int{})}, {"y", local(c99::Int{})}}) ==
        "AttributeError: 'ConstFloat' object has no attribute 'value'");
  CHECK(outcome({main_fn({R(I(0))})}, {{"main", local(c99::Int{})}}) ==
        "AttributeError: 'Int' object has no attribute 'ret'");
  CHECK(outcome({static_var("s", {ir::StringInit{.str = "a€€b", .bytes = 4}}),
                 main_fn({R(I(0))})},
                {{"main", fun()}}) ==
        "UnicodeEncodeError: 'latin-1' codec can't encode characters in position 1-2: ordinal "
        "not in range(256)");
  CHECK(outcome({static_var("s", {ir::StringInit{.str = "a€b", .bytes = 3}}),
                 main_fn({R(I(0))})},
                {{"main", fun()}}) ==
        "UnicodeEncodeError: 'latin-1' codec can't encode character '\\u20ac' in position 1: "
        "ordinal not in range(256)");
}

TEST_CASE("tac_sim: arithmetic at the edges of 64 bits") {
  const auto ll = [](Int v) -> ir::Type_val {
    return ir::Constant{.const_ = ir::ConstLongLong{.value = v}};
  };
  const Int min = -(Int(1) << 63);
  CHECK(outcome({main_fn({ir::Binary{.op = ir::Divide{}, .src1 = ll(min), .src2 = ll(-1),
                                     .dst = V("x")},
                          R(V("x"))})},
                {{"main", fun(c99::LongLong{})}, {"x", local(c99::LongLong{})}}) ==
        "value -9223372036854775808");
  // A shift by 200 places gives 0, and -5 >> 100 gives -1, as Python's
  // unbounded integers do.
  CHECK(outcome({main_fn({ir::Binary{.op = ir::LeftShift{}, .src1 = I(1), .src2 = I(200),
                                     .dst = V("x")},
                          ir::Binary{.op = ir::RightShift{}, .src1 = I(-5), .src2 = I(100),
                                     .dst = V("y")},
                          ir::Binary{.op = ir::Add{}, .src1 = V("x"), .src2 = V("y"),
                                     .dst = V("x")},
                          R(V("x"))})},
                {{"main", fun()}, {"x", local(c99::Int{})}, {"y", local(c99::Int{})}}) ==
        "value -1");
}
