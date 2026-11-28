// The tests of tests/test_tac_sim_optimized.py: fuzz/tac_passes.py's
// `first_changing_pass`, which runs the TAC interpreter after each step
// of the TAC optimizer, names no step for a program the optimizer
// compiles correctly. Its C++ form is `first_changing_pass` in
// src/tac_sim_modes.h.
#include <string>

#include "compile_case.h"
#include "doctest.h"
#include "passes/ir_check.h"
#include "tac_sim_modes.h"

namespace {

namespace suite = c6502::suite;

// Returns check-golden's `found` line for `source`, with the IR checks
// on, as the Python test ran it under `C6502_CHECK_IR=1`.
std::string found(const std::string& source) {
  const c6502::passes::ir_check::Enabled checks(true);
  c6502::passes::FrontEndOptions options;
  options.include_dir = suite::repo_path("include");
  std::string err;
  c6502::tac_sim_modes::Translated t = c6502::tac_sim_modes::translate(source, options, err);
  return "found " +
         c6502::tac_sim_modes::changing_pass_json(c6502::tac_sim_modes::first_changing_pass(t)) +
         "\n";
}

}  // namespace

TEST_CASE("tac_sim_optimized: no pass changes a correct program") {
  CHECK(found(R"(
        unsigned char g = 10;
        unsigned char h(void) { g = 77; return 1; }
        int main(void) {
            int t = g + 1;
            unsigned char k = h();
            unsigned char x = (unsigned char)t;
            return x + k;
        }
    )") == "found null\n");
}

TEST_CASE("tac_sim_optimized: a variable without a value before a loop raises nothing") {
  // SSA gives `c` a Phi at the loop's header whose argument from the
  // entry is `c` itself, which has no value there, and SSA destruction
  // copies it. Neither may raise, since nothing reads the value
  // (tests/chapter_8/valid/continue.c).
  CHECK(found(R"(
            int main(void) {
                int c;
                for (int i = 0; i < 4; i++) c = i;
                return c;
            }
        )") == "found null\n");
}

TEST_CASE("tac_sim_optimized: a store to an object another unit defines runs") {
  // DirectMemberAccess stores to `ext.a[1]` by name, and `ext` has no
  // StaticVariable here, since another unit defines it
  // (tests/chapter_18/valid/parameters/pass_args_on_page_boundary.c).
  CHECK(found(R"(
            struct s { char a[3]; };
            extern struct s ext;
            int main(void) { ext.a[1] = 5; return ext.a[1]; }
        )") == "found null\n");
}
