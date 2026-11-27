// The tests of tests/test_empty_initializer.py that read the TAC and the
// `-S` text or run the TAC interpreter: C23's empty initializer `{}` puts
// a static object in `.bss`, clears a large automatic one with a loop,
// and zeroes every kind of object.
#include <optional>
#include <string>
#include <utility>
#include <variant>
#include <vector>

#include "compile_case.h"
#include "doctest.h"

namespace {

namespace suite = c6502::suite;
namespace ir = c6502::tac_ast;

// compile.py's `-S` output for `src`.
std::string compile_s(const std::string& src, const std::vector<std::string>& flags = {}) {
  std::vector<std::string> argv{"compile.py", "-", "-S", "-o", "-"};
  argv.insert(argv.end(), flags.begin(), flags.end());
  const auto o = suite::compile_main(argv, src);
  REQUIRE(o.status.has_value());
  REQUIRE_MESSAGE(*o.status == 0, o.err());
  return o.out();
}

const std::string kStructs =
    "struct s { int a; char b[3]; long c; };\n"
    "union u { char c; long l; };\n";

// Bytes of `struct s`: a (2), b (3), c (4). tests/test_empty_initializer.py
// runs the same program in the simulator.
const std::string kProgram = R"(struct s { int a; char b[3]; long c; };
union u { char c; long l; double d; };
struct n { struct s s; union u u; int k[2][3]; float f; };
typedef typeof_unqual(nullptr) nullptr_t;

static int failed;
static int checks;

/* Notes the number of the first check whose bytes differ from `want`
   (all zero when `want` is null). */
static void expect(const void *object, unsigned n,
                   const unsigned char *want) {
    const unsigned char *p = object;
    ++checks;
    for (unsigned i = 0; i < n; i++) {
        unsigned char w = want ? want[i] : 0;
        if (p[i] != w) {
            if (!failed)
                failed = checks;
            return;
        }
    }
}

#define ZERO(x) expect(&(x), sizeof(x), 0)

static const unsigned char s_braced[9] = { 3, 0, 0, 0, 0, 4, 0, 0, 0 };
static const unsigned char k_partial[12] = {
    0, 0, 0, 0, 0, 0, 1, 0, 2, 0, 0, 0 };
static const unsigned char five[2] = { 5, 0 };
static const unsigned char minus7[4] = { 0xF9, 0xFF, 0xFF, 0xFF };

/* Leaves 0xA5 in the bytes the next call's locals will take. */
static void dirty(void) {
    volatile unsigned char junk[110];
    for (int i = 0; i < 110; i++)
        junk[i] = 0xA5;
}

struct s g_s = {};
union u g_u = {};
struct n g_n = {};
int g_i = {};
int *g_p = {};
double g_d = {};
bool g_b = {};
_BitInt(12) g_w = {};
nullptr_t g_np = {};
int g_a[5] = {};
struct s g_braced = { {3}, {}, {4} };
int g_k[2][3] = { {}, {1, 2} };
int g_five = {5};
long g_minus7 = {-7,};
struct s *g_ps = &(struct s){};
int *g_pa = (int[4]){};

static void file_scope(void) {
    ZERO(g_s); ZERO(g_u); ZERO(g_n); ZERO(g_i); ZERO(g_p); ZERO(g_d);
    ZERO(g_b); ZERO(g_w); ZERO(g_np); ZERO(g_a);
    expect(&g_braced, 9, s_braced);
    expect(g_k, 12, k_partial);
    expect(&g_five, 2, five);
    expect(&g_minus7, 4, minus7);
    expect(g_ps, sizeof(struct s), 0);
    expect(g_pa, 4 * sizeof(int), 0);
}

static void block_statics(void) {
    static struct s s = {};
    static union u u = {};
    static struct n n = {};
    static int i = {};
    static int *p = {};
    static float f = {};
    static bool b = {};
    static _BitInt(12) w = {};
    static char c[7] = {};
    static struct s braced = { {3}, {}, {4} };
    static int five_ = {5};
    ZERO(s); ZERO(u); ZERO(n); ZERO(i); ZERO(p); ZERO(f); ZERO(b);
    ZERO(w); ZERO(c);
    expect(&braced, 9, s_braced);
    expect(&five_, 2, five);
}

static void automatic_aggregates(void) {
    struct s s = {};
    union u u = {};
    struct n n = {};
    int a[5] = {};
    char text[12] = {};
    struct s braced = { {3}, {}, {4} };
    int k[2][3] = { {}, {1, 2} };
    ZERO(s); ZERO(u); ZERO(n); ZERO(a); ZERO(text);
    expect(&braced, 9, s_braced);
    expect(k, 12, k_partial);
}

static void automatic_scalars(void) {
    int i = {};
    int *p = {};
    float f = {};
    double d = {};
    bool b = {};
    _BitInt(12) w = {};
    nullptr_t np = {};
    char c = {};
    long long ll = {};
    int five_ = {5};
    long m7 = {-7,};
    struct s *ps = &(struct s){};
    int *pa = (int[4]){};
    int ci = (int){};
    ZERO(i); ZERO(p); ZERO(f); ZERO(d); ZERO(b); ZERO(w); ZERO(np);
    ZERO(c); ZERO(ll);
    expect(&five_, 2, five);
    expect(&m7, 4, minus7);
    expect(ps, sizeof(struct s), 0);
    expect(pa, 4 * sizeof(int), 0);
    ZERO(ci);
    for (int j = {}; j < 2; j++) {
        union u again = {};
        ZERO(again);
        again.l = -1;
    }
}

int main(void) {
    file_scope();
    dirty();
    block_statics();
    dirty();
    automatic_aggregates();
    dirty();
    automatic_scalars();
    return failed ? failed : 1000 + checks;
}
)";

}  // namespace

TEST_CASE("empty_initializer: a static object goes in .bss") {
  const std::string asm_text =
      compile_s(kStructs +
                    "struct s v = {}; static union u un = {};"
                    "int f(void) { static int a[10] = {}; return a[1] + v.a + un.c; }",
                {"-O"});
  for (const auto& [name, size] : std::vector<std::pair<std::string, int>>{{"v", 9}, {"un", 4}}) {
    INFO(name);
    CHECK(suite::contains(asm_text, "\n" + name + ":\n   DS.B  " + std::to_string(size) + "\n"));
    CHECK(suite::contains(asm_text, "item " + name + " section=.bss"));
  }
  CHECK(suite::search("\\n@\\d+\\.a:\\n   DS\\.B  20\\n", asm_text));
}

TEST_CASE("empty_initializer: a large automatic struct is cleared by a loop") {
  const std::string src = kStructs +
                          "struct big { struct s s[4]; union u u; };"
                          "void use(struct big *p);"
                          "void f(void) { struct big v = {}; use(&v); }";
  const suite::Tac tac = suite::tac_program(src);
  const ir::Function* fn = nullptr;
  for (const auto& tl : tac.program.top_level) {
    if (const auto* f = std::get_if<ir::Function>(&tl); f != nullptr && f->name == "f") fn = f;
  }
  REQUIRE(fn != nullptr);
  std::vector<std::string> labels;
  std::size_t stores = 0;
  for (const auto& ins : fn->instructions) {
    if (const auto* l = std::get_if<ir::Label>(&ins)) labels.push_back(l->name);
    if (std::holds_alternative<ir::Store>(ins)) ++stores;
  }
  REQUIRE(labels.size() == 1);
  CHECK(labels[0].rfind(".clear@", 0) == 0);
  CHECK(stores == 1);
  // Under -O the object is in the function's pool and the loop indexes it
  // with Y: 40 bytes, counted down. The 0 is loaded before the loop, and
  // the BNE reads the flags DEY sets (cpp/src/passes/loop_constant_a.h).
  CHECK(suite::contains(compile_s(src, {"-O"}),
                        "   LDY   #$28\n   LDA   #$00\n.clear@0:\n   DEY\n"
                        "   STA   __local_f__v,Y\n   BNE   .clear@0\n"));
}

TEST_CASE("empty_initializer: the TAC interpreter finds every object zeroed") {
  // Every check passes: 16 at file scope, 11 for the block statics, 7
  // for the automatic aggregates, and 14 plus 2 passes of the loop for
  // the automatic scalars.
  CHECK(suite::run_program(kProgram) == std::optional<c6502::asdl::Int>(1000 + 16 + 11 + 7 + 16));
}
