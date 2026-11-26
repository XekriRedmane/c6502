// The asm tests of tests/test_loop_counter_to_x.py, ported. The tests
// that compile and run C stay in Python.
#include <algorithm>
#include <string>
#include <vector>

#include "asm_ast.h"
#include "doctest.h"
#include "passes/loop_counter_to_x.h"

namespace ir = c6502::asm_ast;
using c6502::passes::apply_loop_counter_to_x;
using Instrs = std::vector<ir::Type_instruction>;

namespace {

const ir::Type_operand kA = ir::Reg{.reg = ir::A{}};
const ir::Type_operand kX = ir::Reg{.reg = ir::X{}};
const ir::Type_operand kY = ir::Reg{.reg = ir::Y{}};

ir::Program wrap(const Instrs& instrs) {
  return ir::Program{.top_level = {ir::Function{
                         .name = "f", .is_global = false, .instructions = instrs}}};
}

Instrs run(const Instrs& instrs) {
  const ir::Program prog = apply_loop_counter_to_x(wrap(instrs));
  return std::get<ir::Function>(prog.top_level[0]).instructions;
}

// A body-local slot. The pass promotes only those, parameter slots,
// and regalloc ZP bytes, never a user global.
ir::Type_operand m(const std::string& name = "b") {
  return ir::Data{.name = "__local_f__" + name, .offset = 0};
}

ir::Type_operand ix(const char* name) {
  return ir::IndexedData{.name = name, .offset = 0, .index = ir::X{}};
}

ir::Type_operand iy(const char* name) {
  return ir::IndexedData{.name = name, .offset = 0, .index = ir::Y{}};
}

ir::Type_instruction mov(ir::Type_operand src, ir::Type_operand dst) {
  return ir::Mov{.src = src, .dst = dst};
}

ir::Type_instruction label(const char* name) { return ir::Label{.name = name}; }

ir::Type_instruction bpl(const char* target) {
  return ir::Branch{.cond = ir::PL{}, .target = target};
}

bool contains(const Instrs& instrs, const ir::Type_instruction& i) {
  return std::find(instrs.begin(), instrs.end(), i) != instrs.end();
}

// The canonical count-down shape on slot `slot`, optionally with a
// read of it before its init.
Instrs canonical(const ir::Type_operand& slot, bool read_first) {
  Instrs out;
  if (read_first) {
    out.push_back(mov(slot, kX));
  }
  const Instrs rest{
      mov(m("p"), kA), mov(kA, slot),       label(".loop_start"),
      mov(slot, kX),   mov(ix("arr"), kA),  ir::Dec{.dst = slot},
      bpl(".loop_start"), ir::Return{.save_a = false},
  };
  out.insert(out.end(), rest.begin(), rest.end());
  return out;
}

// Builds `LDA p; STA b; .top: LDX b; LDA arr,X; <body>; DEC b; BPL
// .top; <after>`, with an `RTS` for `after` when it is empty.
Instrs loop(const Instrs& body, Instrs after = {}) {
  if (after.empty()) {
    after.push_back(ir::Return{.save_a = false});
  }
  Instrs out{mov(m("p"), kA), mov(kA, m("b")), label(".top"), mov(m("b"), kX),
             mov(ix("arr"), kA)};
  out.insert(out.end(), body.begin(), body.end());
  out.push_back(ir::Dec{.dst = m("b")});
  out.push_back(bpl(".top"));
  out.insert(out.end(), after.begin(), after.end());
  return out;
}

}  // namespace

TEST_CASE("loop_counter_to_x promotes a loop counter to X") {
  SUBCASE("a shifted slot isn't promoted") {
    // `x * 9` builds its high byte with `STA hi; ROL hi; ...; INC hi;
    // LDX hi`. The ROL is a use the promotion doesn't rewrite, so
    // moving `hi` to X would lose it.
    const Instrs instrs{
        mov(ir::Imm{.value = 0}, kA), mov(kA, m("hi")), ir::RotateLeft{.dst = m("hi")},
        ir::Inc{.dst = m("hi")},      mov(m("hi"), kX),
        ir::Return{.save_a = true, .save_x = true},
    };
    CHECK(run(instrs) == instrs);
  }
  SUBCASE("a count-up loop") {
    // LDA #0; STA i; top: LDA i; CMP n; BEQ done; LDX i; LDA arr,X;
    // INC i; JMP top -> LDX #0 ... CPX n ... INX.
    const Instrs out = run({
        mov(ir::Imm{.value = 0}, kA),
        mov(kA, m("i")),
        label(".top"),
        mov(m("i"), kA),
        ir::Compare{.left = kA, .right = m("n")},
        ir::Branch{.cond = ir::EQ{}, .target = ".done"},
        mov(m("i"), kX),
        mov(ix("arr"), kA),
        ir::Inc{.dst = m("i")},
        ir::Jump{.target = ".top"},
        label(".done"),
        ir::Return{.save_a = false},
    });
    CHECK(contains(out, ir::Compare{.left = kX, .right = m("n")}));
    CHECK(contains(out, ir::Inc{.dst = kX}));
    CHECK_FALSE(contains(out, mov(m("i"), kX)));
    CHECK_FALSE(contains(out, ir::Inc{.dst = m("i")}));
  }
  SUBCASE("the canonical pattern fires") {
    // LDA p; STA b; loop_start: LDX b; ...; DEC b; BPL loop_start.
    const Instrs out = run(canonical(m("b"), false));
    // TAX is inserted after the init STA, the LDX at the loop top is
    // dropped, and the DEC becomes DEX.
    CHECK(out == Instrs{mov(m("p"), kA), mov(kA, m("b")), mov(kA, kX),
                        label(".loop_start"), mov(ix("arr"), kA), ir::Dec{.dst = kX},
                        bpl(".loop_start"), ir::Return{.save_a = false}});
  }
  SUBCASE("a Phi that reads the slot keeps it in memory") {
    // The pass runs after SSA destruction, where no Phi is left, but a
    // Phi argument is a read of its source like any other.
    Instrs instrs = canonical(m("b"), false);
    instrs.insert(instrs.end() - 1,
                  ir::Phi{.dst = m("c"),
                          .args = {ir::AsmPhiArg{.pred_label = ".loop_start", .source = m("b")}}});
    CHECK(run(instrs) == instrs);
  }
  SUBCASE("a user global isn't promoted") {
    // A global has a value at entry and after the return, and DEX
    // would leave its memory stale.
    const Instrs instrs = canonical(ir::Data{.name = "z", .offset = 0}, false);
    CHECK(run(instrs) == instrs);
  }
  SUBCASE("a read before the init isn't promoted") {
    const Instrs instrs = canonical(m("b"), true);
    CHECK(run(instrs) == instrs);
  }
  SUBCASE("another use of M disqualifies it") {
    const Instrs instrs{
        mov(m("p"), kA),
        mov(kA, m("b")),
        label(".loop_start"),
        mov(m("b"), kX),
        ir::Compare{.left = kA, .right = m("b")},
        ir::Dec{.dst = m("b")},
        bpl(".loop_start"),
        ir::Return{.save_a = false},
    };
    CHECK(run(instrs) == instrs);
  }
  SUBCASE("a passive label between the DEC and the branch is allowed") {
    const Instrs out = run({
        mov(m("p"), kA),
        mov(kA, m("b")),
        label(".loop_start"),
        mov(m("b"), kX),
        ir::Dec{.dst = m("b")},
        label(".loop_continue"),
        bpl(".loop_start"),
        ir::Return{.save_a = false},
    });
    CHECK(contains(out, mov(kA, kX)));
    CHECK(contains(out, ir::Dec{.dst = kX}));
  }
  SUBCASE("LDA M becomes TXA") {
    // `Mov(M, Reg(A))` gives the same value as TXA after the
    // promotion, since X = M is the invariant.
    const Instrs out = run({
        mov(m("p"), kA),
        mov(kA, m("b")),
        label(".loop_start"),
        mov(m("b"), kX),
        mov(m("b"), kA),
        mov(kA, m("scratch")),
        ir::Dec{.dst = m("b")},
        bpl(".loop_start"),
        ir::Return{.save_a = false},
    });
    CHECK(contains(out, mov(kX, kA)));
    CHECK(contains(out, ir::Dec{.dst = kX}));
  }
  SUBCASE("a call is wrapped with STX and LDX") {
    // The callee clobbers X, and the counter is kept across it in M.
    const Instrs out = run({
        mov(m("p"), kA),
        mov(kA, m("b")),
        label(".loop_start"),
        mov(m("b"), kX),
        mov(ix("arr"), kA),
        ir::Call{.name = "helper"},
        ir::Dec{.dst = m("b")},
        bpl(".loop_start"),
        ir::Return{.save_a = false},
    });
    const auto call = std::find_if(out.begin(), out.end(), [](const auto& i) {
      return std::holds_alternative<ir::Call>(i);
    });
    REQUIRE(call != out.end());
    CHECK(*(call - 1) == mov(kX, m("b")));
    CHECK(*(call + 1) == mov(m("b"), kX));
  }
  SUBCASE("an indirect-Y access blocks the pivot") {
    // The range that a non-counter `LDX <other>` opens holds an
    // Indirect-Y operand, which reads Y for its addressing mode, so
    // rewriting LDX to LDY would clobber it.
    const Instrs instrs{
        mov(m("p"), kA),
        mov(kA, m("b")),
        label(".loop_start"),
        mov(m("b"), kX),
        mov(m("temp"), kX),
        mov(ir::IndirectZpY{.address = 0x10}, kA),
        ir::Dec{.dst = m("b")},
        bpl(".loop_start"),
        ir::Return{.save_a = false},
    };
    CHECK(run(instrs) == instrs);
  }
  SUBCASE("an active label between the DEC and the branch blocks it") {
    // A label between the DEC and the Branch that something else jumps
    // to gives a path that bypasses the DEC.
    const Instrs instrs{
        ir::Jump{.target = ".active"},
        mov(m("p"), kA),
        mov(kA, m("b")),
        label(".loop_start"),
        mov(m("b"), kX),
        ir::Dec{.dst = m("b")},
        label(".active"),
        bpl(".loop_start"),
        ir::Return{.save_a = false},
    };
    CHECK(run(instrs) == instrs);
  }
}

TEST_CASE("loop_counter_to_x bounds its pivot ranges") {
  // Each of these shapes was miscompiled by the Y-pivot or the init
  // rewrite before the range checks were tightened.
  SUBCASE("the pivot range stops before the counter's step") {
    // `LDX k; LDA t,X; STA s2` runs straight into `DEC b`. The range
    // used to swallow the DEC, which then stayed a DEC of memory while
    // the loop-top `LDX b` was dropped, so X never changed and the loop
    // never ended.
    const Instrs out = run(loop({mov(m("k"), kX), mov(ix("t"), kA), mov(kA, m("s2"))}));
    CHECK(contains(out, ir::Dec{.dst = kX}));
    CHECK_FALSE(contains(out, ir::Dec{.dst = m("b")}));
    CHECK(contains(out, mov(iy("t"), kA)));
  }
  SUBCASE("an LDX after the loop isn't pivoted") {
    // `return s;` for a 2-byte `s` ends in `LDA s; LDX s+1; RTS` with
    // the high byte in X. The counter is dead there, so the LDX stays.
    const ir::Type_operand s1 = ir::Data{.name = "s", .offset = 1};
    const Instrs out = run(loop({mov(kA, m("s"))}, {mov(m("s"), kA), mov(s1, kX),
                                                    ir::Return{.save_a = true, .save_x = true}}));
    CHECK(contains(out, ir::Dec{.dst = kX}));
    CHECK(out[out.size() - 2] == mov(s1, kX));
  }
  SUBCASE("CMP abs,X in a pivot range is rewritten") {
    // `CMP u,X` reads the pivoted value, so it has to become `CMP u,Y`
    // along with the LDA.
    const Instrs out = run(loop({mov(m("k"), kX), mov(ix("t"), kA),
                                 ir::Compare{.left = kA, .right = ix("u")}, mov(kA, m("s"))}));
    CHECK(contains(out, ir::Compare{.left = kA, .right = iy("u")}));
    CHECK_FALSE(contains(out, ir::Compare{.left = kA, .right = ix("u")}));
  }
  SUBCASE("a read of Y in a pivot range blocks it") {
    // `ADC u,Y` reads the Y the code had before the range, which the
    // pivot's `LDY k` would replace.
    const Instrs instrs = loop({mov(m("k"), kX), mov(ix("t"), kA),
                                ir::Add{.src = iy("u"), .dst = kA}, mov(kA, m("s"))});
    CHECK(run(instrs) == instrs);
  }
  SUBCASE("a frame access in a pivot range blocks it") {
    // A soft-stack access is lowered as `LDY #off; LDA (FP),Y`, which
    // overwrites the pivoted value in Y.
    const Instrs instrs = loop({mov(m("k"), kX), mov(ir::Frame{.offset = 3}, kA),
                                mov(ix("t"), kA), mov(kA, m("s"))});
    CHECK(run(instrs) == instrs);
  }
  SUBCASE("a read of X after a label blocks it") {
    // The range ends at `.mid`, but `LDA u,X` after it still reads the
    // value of `LDX k`.
    const Instrs instrs = loop({mov(m("k"), kX), mov(ix("t"), kA), label(".mid"),
                                mov(ix("u"), kA), mov(kA, m("s"))});
    CHECK(run(instrs) == instrs);
  }
  SUBCASE("an STX init gets no TAX") {
    // The init `STX b` leaves the initial value in X already.
    const Instrs out = run({
        mov(kX, m("b")),
        label(".top"),
        mov(m("b"), kX),
        mov(ix("arr"), kA),
        mov(kA, m("s")),
        ir::Dec{.dst = m("b")},
        bpl(".top"),
        ir::Return{.save_a = false},
    });
    CHECK(contains(out, ir::Dec{.dst = kX}));
    CHECK_FALSE(contains(out, mov(kA, kX)));
  }
  SUBCASE("an STY init reloads X from the slot") {
    // After `STY b` the value is in Y and b, not in A.
    const Instrs out = run({
        mov(kY, m("b")),
        label(".top"),
        mov(m("b"), kX),
        mov(ix("arr"), kA),
        mov(kA, m("s")),
        ir::Dec{.dst = m("b")},
        bpl(".top"),
        ir::Return{.save_a = false},
    });
    CHECK(out[1] == mov(m("b"), kX));
    CHECK_FALSE(contains(out, mov(kA, kX)));
  }
}
