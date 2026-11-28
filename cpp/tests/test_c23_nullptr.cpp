// The tests of tests/test_c23_nullptr.py that run no simulator: `nullptr`
// isn't an integer constant, <stddef.h> includes without a diagnostic,
// and a program of `nullptr` and `nullptr_t` on the TAC interpreter.
#include <optional>
#include <string>
#include <utility>

#include "compile_case.h"
#include "doctest.h"

namespace {

namespace suite = c6502::suite;

// compile.py's exit status and stderr for `src` on stdin.
std::pair<long long, std::string> compile_s(const std::string& src) {
  const auto o = suite::compile_main({"c6502", "-", "-S"}, src);
  REQUIRE(o.status.has_value());
  return {*o.status, o.err()};
}

}  // namespace

TEST_CASE("c23_nullptr: nullptr isn't an integer constant") {
  // §6.6p8 admits only operands of integer type to an integer constant
  // expression.
  const std::string message =
      "`nullptr` has type nullptr_t, which isn't an integer type, so "
      "it can't be an operand of an integer constant expression "
      "(C23 §6.6p8)";
  const auto [rc, err] =
      compile_s("int f(int x) { switch (x) { case nullptr: return 0; } return 1; }\n");
  CHECK(rc == 1);
  CHECK(suite::contains(err, "case label is not an integer constant expression: " + message));
}

TEST_CASE("c23_nullptr: <stddef.h> includes cleanly") {
  // compile.py prints each warning to stderr, so an empty stderr is the
  // check that there is none.
  const auto result = compile_s(
      "#include <stddef.h>\n"
      "#if __STDC_VERSION_STDDEF_H__ != 202311L\n"
      "#error\n"
      "#endif\n"
      "struct s { char a; int b; long c[3]; };\n"
      "nullptr_t n = nullptr;\n"
      "size_t z = sizeof(int);\n"
      "ptrdiff_t d;\n"
      "max_align_t m;\n"
      "wchar_t w;\n"
      "int *p = NULL;\n"
      "size_t o = offsetof(struct s, c[1]);\n"
      "int main(void) { unreachable(); return NULL == 0; }\n");
  CHECK(result == std::pair<long long, std::string>{0, ""});
}

TEST_CASE("c23_nullptr: the TAC interpreter runs a program of nullptr") {
  const std::string program = R"(#include <stddef.h>

struct node { int value; struct node *next; };

static struct node c = { 3, nullptr };
static struct node b = { 2, &c };
static struct node a = { 1, &b };
static int *gp = nullptr;
static nullptr_t gn;
static bool gb = nullptr;
int g;

int length(struct node *n) {
    int k = 0;
    for (; n != nullptr; n = n->next)
        k++;
    return k;
}

int *pick(int which) { return which ? &g : nullptr; }

nullptr_t none(void) { return nullptr; }

int is_null(void *p) { return p == nullptr; }

int main(void) {
    int *p = nullptr;
    nullptr_t n = 0;
    int r = 0;
    if (nullptr == 0 && 0 == nullptr) r += 1;
    if (nullptr == NULL && NULL == 0 && nullptr == (void *)0) r += 2;
    if (p == nullptr && !(nullptr != p)) r += 4;
    if (!nullptr && !(bool)nullptr && !n) r += 8;
    if (is_null(nullptr) && is_null(NULL) && !is_null(&g)) r += 16;
    if (pick(0) == nullptr && pick(1) == &g) r += 32;
    if (gp == nullptr && !gb && gn == nullptr && n == nullptr) r += 64;
    if (none() == nullptr && !none()) r += 128;
    if (sizeof(nullptr) == 2 && sizeof(nullptr_t) == 2) r += 256;
    p = r & 1 ? &g : nullptr;
    if (p == &g) r += 512;
    r += 1000 * length(&a) + 10000 * (int)offsetof(struct node, next);
    return r;
}
)";
  // 1 + 2 + 4 + 8 + 16 + 32 + 64 + 128 + 256 + 512, three nodes, and
  // `next` 2 bytes into a node.
  CHECK(suite::run_program(program) == std::optional<c6502::asdl::Int>(1023 + 3000 + 20000));
}
