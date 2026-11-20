// Tests of the `c6502cpp backend` command's requests and responses
// (src/backend.h).
#include <stdexcept>
#include <string>

#include "backend.h"
#include "doctest.h"

namespace {

const char kContext[] =
    R"({"layout":[0,2,4,32,36],"user_zp_globals":[],"user_zp_bytes":[]})";

const char kProgram[] = R"(Program([
  Function("f" true [] [
    Mov(ZP(144 0) Reg(A) false)
    ClearCarry
    Add(Imm(1) Reg(A))
    Mov(Reg(A) ZP(144 0) false)
    Return(false false false)
  ])
])
)";

const char kExpected[] = R"(Program([
  Function("f" true [] [
    Inc(ZP(144 0))
    Return(false false false)
  ])
])
)";

// Returns a request of `mode` for `kProgram`, whose arguments `args` are
// JSON on one line.
std::string request(const std::string& mode, const std::string& args) {
  return "c6502-backend 1\nmode " + mode + "\ncontext " + kContext + "\nargs " + args +
         "\ninput\n" + kProgram;
}

}  // namespace

TEST_CASE("backend answers an unoptimized request with the program") {
  const std::string got =
      c6502::backend::respond(request("unoptimized", R"({"zp_addrs": {"dict": []}})"));
  CHECK(got == std::string("output\n") + kExpected);
}

TEST_CASE("backend answers an optimized request with the results' lines") {
  const std::string got = c6502::backend::respond(request(
      "optimized",
      R"({"slot_symbols": {"dict": []}, "zp_addrs": {"dict": []}, )"
      R"("noreturn": {"set": []}, "abi": {"dict": [["f", null]]}, "shares": [], )"
      R"("ret_eligible": {"set": []}, "zp_names": {"set": []}, )"
      R"("slot_addrs": {"set": []}})"));
  CHECK(got == std::string("coalesced []\nnew_functions []\noutput\n") + kExpected);
}

TEST_CASE("backend rejects a request without its arguments") {
  CHECK_THROWS_AS(c6502::backend::respond(request("optimized", "{}")), std::runtime_error);
  CHECK_THROWS_AS(c6502::backend::respond("c6502-backend 2\n"), std::runtime_error);
}
