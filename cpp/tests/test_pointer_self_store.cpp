// The tests of TestPointerSelfStore in tests/test_pointer_member_access.py
// and of the element store in tests/test_element_rmw.py, ported.
#include <string>

#include "asm_text.h"
#include "doctest.h"
#include "passes/asm_context.h"
#include "passes/pointer_self_store.h"

namespace {

using c6502::asm_text::expect;
using c6502::asm_text::instructions;
using c6502::asm_text::program;

const c6502::passes::SymbolAddrs kZp = {
    {"__local_f__t", 0x80}, {"__local_f__u", 0x81}, {"P", 0xF4}};

// Runs the pass with the slots of tests/test_pointer_member_access.py.
std::string run(const std::string& instrs) {
  return instructions(c6502::passes::apply_pointer_self_store_drop(
      program(instrs), c6502::passes::AsmContext{}, &kZp));
}

// The byte at `(P),Y` with `Y = k`, P at $F4.
std::string through(int k) { return "IndirectZp(244 " + std::to_string(k) + ")"; }

const std::string kT = R"(Data("__local_f__t" 0))";

std::string mov(const std::string& src, const std::string& dst,
                const char* is_volatile = "false") {
  return "Mov(" + src + " " + dst + " " + is_volatile + ") ";
}

}  // namespace

TEST_CASE("pointer_self_store drops the store of the byte read") {
  const std::string in = mov(through(8), "Reg(A)") + mov("Reg(A)", kT) +
                         R"(Inc(Data("__local_f__u" 0)) )" + mov(kT, "Reg(A)") +
                         mov("Reg(A)", through(8)) + "Return(false false false)";
  CHECK(run(in).find("Mov(Reg(A) IndirectZp(244 8)") == std::string::npos);
}

TEST_CASE("pointer_self_store keeps the fact past a store at another offset") {
  const std::string in = mov(through(8), "Reg(A)") + mov("Reg(A)", kT) +
                         mov("Imm(1)", "Reg(A)") + mov("Reg(A)", through(9)) +
                         mov(kT, "Reg(A)") + mov("Reg(A)", through(8));
  CHECK(run(in).find("Mov(Reg(A) IndirectZp(244 8)") == std::string::npos);
}

TEST_CASE("pointer_self_store ends the fact at a store that may reach the byte") {
  // Another pointer, a static, and the pointer itself.
  for (const char* dst : {"IndirectZp(144 0)", R"(Data("G" 0))", R"(Data("P" 1))"}) {
    CAPTURE(dst);
    const std::string in = mov(through(8), "Reg(A)") + mov("Reg(A)", kT) +
                           mov("Imm(1)", "Reg(A)") + mov("Reg(A)", dst) +
                           mov(kT, "Reg(A)") + mov("Reg(A)", through(8));
    CHECK(run(in) == expect(in));
  }
}

TEST_CASE("pointer_self_store keeps a volatile read's store and a store after a label") {
  const std::string volatile_read =
      mov(through(8), "Reg(A)", "true") + mov("Reg(A)", through(8));
  CHECK(run(volatile_read) == expect(volatile_read));
  const std::string label =
      mov(through(8), "Reg(A)") + R"(Label(".l") )" + mov("Reg(A)", through(8));
  CHECK(run(label) == expect(label));
}

TEST_CASE("pointer_self_store reads a slot's byte through the pointer again") {
  const std::string in = mov(through(13), "Reg(A)") + mov("Reg(A)", kT) +
                         mov(R"(Data("W" 0))", "Reg(A)") +
                         mov("Reg(A)", R"(Data("__local_f__u" 0))") +
                         mov(kT, "Reg(A)") + "Return(true false false)";
  const std::string out = run(in);
  const std::string expected = mov(through(13), "Reg(A)") + mov("Reg(A)", kT) +
                               mov(R"(Data("W" 0))", "Reg(A)") +
                               mov("Reg(A)", R"(Data("__local_f__u" 0))") +
                               mov(through(13), "Reg(A)") + "Return(true false false)";
  CHECK(out == expect(expected));
}

TEST_CASE("pointer_self_store ends the fact about a byte at a store to its element") {
  // `buf+1` holds the byte read through the pointer at $40, and `STA
  // buf,X` may change it, so the store of `buf+1` back through the
  // pointer stays.
  const std::string through40 = "IndirectZp(64 0)";
  const std::string buf1 = R"(Data("buf" 1))";
  const std::string in = mov(through40, "Reg(A)") + mov("Reg(A)", buf1) +
                         mov("Imm(9)", "Reg(A)") +
                         mov("Reg(A)", R"(IndexedData("buf" 0 X))") +
                         mov(buf1, "Reg(A)") + mov("Reg(A)", through40) +
                         "Return(false false false)";
  CHECK(instructions(c6502::passes::apply_pointer_self_store_drop(
            program(in), c6502::passes::AsmContext{})) == expect(in));
}
