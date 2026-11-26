// The tests of the shared asm helpers: tests/test_asm_liveness.py
// ported, with every assertion of its tests in a case below, the
// liveness tests of test_carry_return.py and test_divide_shapes.py, a
// first port of tests/test_asm_aliasing.py and tests/test_asm_cfg.py,
// whose tests have a case each in test_asm_aliasing.cpp and
// test_asm_cfg.cpp, and cases from the docstrings of decimal_arith,
// asm_dead_store, inc_peephole, dead_a_arith, call_edges, and
// sim/assembler.py.
#include <set>
#include <string>
#include <vector>

#include "asm_ast.h"
#include "doctest.h"
#include "errors.h"
#include "passes/asm_aliasing.h"
#include "passes/asm_context.h"
#include "passes/asm_dead_store.h"
#include "passes/asm_liveness.h"
#include "passes/call_edges.h"
#include "passes/dead_a_arith.h"
#include "passes/decimal_arith.h"
#include "passes/inc_peephole.h"
#include "passes/optimization_asm/cfg.h"
#include "passes/redundant_load.h"
#include "sim/assembler.h"
#include "tac_ast.h"

namespace ir = c6502::asm_ast;
namespace tac = c6502::tac_ast;
using namespace c6502::passes;
using List = std::vector<ir::Type_instruction>;

namespace {

const ir::Type_operand kA = ir::Reg{.reg = ir::A{}};
const ir::Type_operand kX = ir::Reg{.reg = ir::X{}};
const ir::Type_operand kY = ir::Reg{.reg = ir::Y{}};

ir::Type_operand imm(int v) { return ir::Imm{.value = v}; }
ir::Type_operand data(const char* name, int off = 0) {
  return ir::Data{.name = name, .offset = off};
}
ir::Type_operand zp(int addr, int off = 0) { return ir::ZP{.address = addr, .offset = off}; }
ir::Type_operand idata(const char* name, ir::Type_reg idx = ir::X{}) {
  return ir::IndexedData{.name = name, .offset = 0, .index = idx};
}
ir::Type_operand frame(int off) { return ir::Frame{.offset = off}; }
ir::Type_operand stack(int off) { return ir::Stack{.offset = off}; }
ir::Type_operand indirect(int off) { return ir::Indirect{.offset = off}; }
ir::Type_operand izp(int addr, int off) {
  return ir::IndirectZp{.address = addr, .offset = off};
}
ir::Type_operand izpy(int addr) { return ir::IndirectZpY{.address = addr}; }

ir::Type_instruction mov(ir::Type_operand src, ir::Type_operand dst) {
  return ir::Mov{.src = src, .dst = dst};
}
ir::Type_instruction label(const char* name) { return ir::Label{.name = name}; }
ir::Type_instruction branch(ir::Type_condition cond, const char* target) {
  return ir::Branch{.cond = cond, .target = target};
}
ir::Type_instruction ret(bool save_a = false) { return ir::Return{.save_a = save_a}; }

// The end of tac_to_asm's signed ordering compare:
// `SBC hi; BVC .novf; EOR #$80; .novf: BMI .true; ...`.
List signed_compare_tail() {
  return {
      ir::Sub{.src = imm(0), .dst = kA},
      branch(ir::VC{}, ".novf"),
      ir::Xor{.src1 = kA, .src2 = imm(0x80), .dst = kA},
      label(".novf"),
      branch(ir::MI{}, ".true"),
      mov(imm(0), kA),
      ret(true),
      label(".true"),
      mov(imm(1), kA),
      ret(true),
  };
}

const AsmContext kCtx{};

bool alias(const ir::Type_operand& a, const ir::Type_operand& b,
           const AsmContext& ctx = kCtx, const MayAliasOptions& o = {}) {
  return may_alias(a, b, ctx, o);
}

}  // namespace

TEST_CASE("flags_dead_at follows labels, jumps, and branches") {
  SUBCASE("the EOR's N flag reaches the BMI after .novf") {
    const List instrs = signed_compare_tail();
    CHECK_FALSE(flags_dead_at(instrs, 3));
    CHECK_FALSE(all_flags_dead_at(instrs, 3));
  }
  SUBCASE("through a jump") {
    const List instrs = {ir::Jump{.target = ".l"}, label(".l"),
                           branch(ir::EQ{}, ".l")};
    CHECK_FALSE(flags_dead_at(instrs, 0));
  }
  SUBCASE("a flag setter after a label kills") {
    const List instrs = {label(".l"), mov(imm(0), kA), branch(ir::EQ{}, ".l")};
    CHECK(flags_dead_at(instrs, 0));
  }
  SUBCASE("BCS reads only C") {
    const List instrs = {branch(ir::CS{}, ".l"), mov(imm(0), kA),
                           branch(ir::EQ{}, ".l"), label(".l"),
                           mov(imm(1), kA), ret(true)};
    CHECK(flags_dead_at(instrs, 0));
    CHECK_FALSE(all_flags_dead_at(instrs, 0));
  }
  SUBCASE("a carry branch's target reads N/Z") {
    const List instrs = {branch(ir::CS{}, ".l"), mov(imm(0), kA), ret(true),
                           label(".l"), branch(ir::EQ{}, ".l")};
    CHECK_FALSE(flags_dead_at(instrs, 0));
  }
  SUBCASE("a return ends liveness") {
    const List instrs = {label(".l"), ret(true)};
    CHECK(flags_dead_at(instrs, 0));
    CHECK(all_flags_dead_at(instrs, 0));
  }
}

TEST_CASE("c_dead_at and v_dead_at follow one flag") {
  // A carry-flag return reads C, and nothing but BVC / BVS reads V.
  const List carry = {mov(imm(0), kA), ir::Return{.save_c = true}};
  CHECK_FALSE(c_dead_at(carry, 0));
  CHECK(v_dead_at(carry, 0));
  const List adc = {ir::Add{.src = imm(1), .dst = kA}, ret()};
  CHECK_FALSE(c_dead_at(adc, 0));
  CHECK(v_dead_at(adc, 0));
  const List bvs = {branch(ir::VS{}, ".l"), label(".l"), ret()};
  CHECK_FALSE(v_dead_at(bvs, 0));
  CHECK(c_dead_at(bvs, 0));
  // A tail call writes C and V as a call does.
  const List tail = {ir::Jump{.target = "f"}};
  CHECK(c_dead_at(tail, 0));
  CHECK(v_dead_at(tail, 0));
  // The compound atoms read every flag.
  const List prologue = {ir::FunctionPrologue{.arg_bytes = 1}};
  CHECK_FALSE(c_dead_at(prologue, 0));
}

TEST_CASE("c_dead_at: test_carry_return.py::TestCarryLiveAtReturn::test_liveness") {
  const List carry = {ir::SetCarry{}, ir::Return{.save_c = true}};
  CHECK_FALSE(c_dead_at(carry, 1));
  CHECK_FALSE(all_flags_dead_at(carry, 1));
  const List plain = {ir::SetCarry{}, ret()};
  CHECK(c_dead_at(plain, 1));
}

TEST_CASE("kills_a counts the moves asm_emit writes through A") {
  SUBCASE("a constant or a byte into memory") {
    CHECK(kills_a(mov(imm(0), data("m"))));
    CHECK(kills_a(mov(data("n"), data("m"))));
  }
  SUBCASE("between X and Y") {
    CHECK(kills_a(mov(kY, kX)));
    CHECK(kills_a(mov(kX, kY)));
  }
  SUBCASE("from X to the soft stack") { CHECK(kills_a(mov(kX, frame(3)))); }
  SUBCASE("moves that leave A") {
    CHECK_FALSE(kills_a(mov(kX, data("m"))));  // STX
    CHECK_FALSE(kills_a(mov(kA, data("m"))));  // STA
    CHECK_FALSE(kills_a(mov(data("m"), data("m"))));
    CHECK_FALSE(kills_a(mov(data("m"), kX)));  // LDX
  }
  SUBCASE("a load before a constant store is dead") {
    // divu16's entry: the first load is overwritten by the `LDA #$00`
    // of the constant store before anything reads A.
    const List instrs = {mov(data("d"), kA), mov(imm(0), data("p")),
                           mov(kA, data("p", 1)), ret()};
    CHECK(a_dead_at(instrs, 1));
  }
}

TEST_CASE("sets_flags") {
  CHECK_FALSE(sets_flags(mov(kA, data("g"))));
  CHECK_FALSE(sets_flags(mov(kY, data("g"))));
  CHECK_FALSE(sets_flags(mov(kX, kX)));
  // Each of these emits `LDY #off` ahead of the `STA`, and LDY sets N/Z
  // from the offset.
  for (const auto& dst : {frame(3), stack(1), indirect(0), izp(0x80, 2)}) {
    CHECK(sets_flags(mov(kA, dst)));
    CHECK(sets_up_y(mov(kA, dst)));
  }
}

TEST_CASE("sets_flags: test_divide_shapes.py::TestStoresAndIndexedAccesses::test_stx_sty_leave_the_flags") {
  CHECK_FALSE(sets_flags(mov(kX, data("m"))));
  CHECK_FALSE(sets_flags(mov(kY, data("m"))));
  // Through A, to the soft stack: TXA sets them.
  CHECK(sets_flags(mov(kX, frame(1))));
}

TEST_CASE("BitTest reads its index register") {
  const ir::Type_instruction bit_x = ir::BitTest{.src = idata("t", ir::X{})};
  const ir::Type_instruction bit_y = ir::BitTest{.src = idata("t", ir::Y{})};
  const ir::Type_instruction plain = ir::BitTest{.src = data("t")};
  CHECK(reads_x(bit_x));
  CHECK(reads_y(bit_y));
  CHECK_FALSE(reads_x(plain));
  CHECK_FALSE(x_dead_at(List{bit_x, ret()}, 0));
  CHECK(y_dead_at(List{plain, ret()}, 0));
}

TEST_CASE("a JumpTable reads A, writes its index, and goes to each target") {
  const auto instrs = [](bool case_reads_x) {
    List out = {mov(imm(1), kX),
                  ir::JumpTable{.table = ".jtab@0",
                                .index = "Y",
                                .targets = {".c0", "elsewhere"}},
                  label(".c0")};
    if (case_reads_x) {
      out.push_back(mov(kX, data("g")));
    }
    out.push_back(ret());
    return out;
  };
  CHECK_FALSE(x_dead_at(instrs(true), 1));
  CHECK(x_dead_at(instrs(false), 1));
  CHECK_FALSE(a_dead_at(instrs(false), 1));
  CHECK(y_dead_at(instrs(false), 1));
}

TEST_CASE("register liveness at returns and calls") {
  // A 2-byte register return reads X, and a return never reads Y.
  CHECK_FALSE(x_dead_at(List{ir::Return{.save_x = true}}, 0));
  CHECK(x_dead_at(List{ret()}, 0));
  CHECK(y_dead_at(List{ir::Return{.save_a = true, .save_x = true}}, 0));
  // A call that takes A reads it, and a callee declared `preserves_x`
  // leaves X.
  const ir::Type_instruction call_a = ir::Call{.name = "f", .reg_args = {"A"}};
  CHECK(reads_a(call_a));
  CHECK(kills_a(call_a));
  const ir::Type_instruction keeps_x = ir::Call{.name = "f", .preserves = {"X"}};
  CHECK(call_preserves(keeps_x, "X"));
  CHECK_FALSE(call_preserves(keeps_x, "Y"));
  CHECK_FALSE(kills_x(keeps_x));
  CHECK(kills_y(keeps_x));
  CHECK_FALSE(x_dead_at(List{keeps_x, mov(kX, data("g")), ret()}, 0));
  // A tail call with X in `reg_args` reads X.
  CHECK_FALSE(x_dead_at(List{ir::Jump{.target = "f", .reg_args = {"X"}}}, 0));
}

TEST_CASE("label_map gives the last index of each label") {
  const List instrs = {label("a"), ret(), label("b"), label("a")};
  const LabelMap labels = label_map(instrs);
  CHECK(labels.find("a") == 3);
  CHECK(labels.find("b") == 2);
  CHECK(labels.find("c") == -1);
}

TEST_CASE("collect_address_taken_names reads a label's halves in every operand") {
  const ir::Type_operand lo_c = ir::ImmLabelLow{.name = "__local_f__c"};
  const ir::Type_operand hi_e = ir::ImmLabelHigh{.name = "__local_f__e"};
  const List instrs = {
      ir::LoadAddress{.src = data("__local_f__a"), .dst = data("__local_f__p")},
      mov(ir::ImmLabelLow{.name = "__local_f__b"}, kA),
      ir::Compare{.left = kA, .right = lo_c},
      ir::Xor{.src1 = kA, .src2 = ir::ImmLabelHigh{.name = "__local_f__d"}, .dst = kA},
      ir::DecimalArith{.left = {hi_e}, .right = {imm(1)}, .dst = {data("__local_f__q")}},
      mov(data("__local_f__x"), kA),
      ret(),
  };
  const NameSet got = asm_dead_store::collect_address_taken_names(instrs);
  CHECK(got == NameSet{"__local_f__a", "__local_f__b", "__local_f__c", "__local_f__d",
                       "__local_f__e"});
}

TEST_CASE("asm_dead_store: DPTR dies at a direct call and at a return") {
  CHECK(asm_dead_store::is_dead_at_exit(data("DPTR", 1), nullptr));
  CHECK_FALSE(asm_dead_store::is_dead_at_exit(data("g"), nullptr));
  const auto after_call = [](const char* callee) {
    const List instrs = {
        mov(kA, data("DPTR")),
        ir::Call{.name = callee},
        mov(data("DPTR"), kA),
        ret(true),
    };
    return asm_dead_store::mem_dead_after(instrs, 0, data("DPTR"), kCtx, "f");
  };
  CHECK(after_call("g"));
  CHECK_FALSE(after_call("icall"));
  const List to_return = {mov(kA, data("DPTR")), ret()};
  CHECK(asm_dead_store::mem_dead_after(to_return, 0, data("DPTR"), kCtx, "f"));
  const List read = {mov(kA, data("DPTR")), mov(data("DPTR"), kA), ret(true)};
  CHECK_FALSE(asm_dead_store::mem_dead_after(read, 0, data("DPTR"), kCtx, "f"));
}

TEST_CASE("asm_dead_store::successors") {
  const List instrs = {
      branch(ir::EQ{}, ".t"),
      mov(imm(1), kA),
      ir::Jump{.target = ".gone"},
      label(".t"),
      ret(),
      branch(ir::NE{}, ".t"),
  };
  const LabelMap labels = label_map(instrs);
  using V = std::vector<std::size_t>;
  CHECK(asm_dead_store::successors(instrs, 0, labels) == V{1, 3});
  CHECK(asm_dead_store::successors(instrs, 1, labels) == V{2});
  CHECK(asm_dead_store::successors(instrs, 2, labels).empty());
  CHECK(asm_dead_store::successors(instrs, 3, labels) == V{4});
  CHECK(asm_dead_store::successors(instrs, 4, labels).empty());
  CHECK(asm_dead_store::successors(instrs, 5, labels) == V{3});
}

TEST_CASE("may_alias: immediates never alias") {
  const ir::Type_operand i = imm(5);
  CHECK_FALSE(alias(i, i));
  CHECK_FALSE(alias(i, zp(0x80)));
  CHECK_FALSE(alias(data("DPTR"), i));
  // `ImmLabelLow(name)` is `LDA #<name`, which loads the address's low
  // byte and reads no memory.
  const ir::Type_operand lo = ir::ImmLabelLow{.name = "slot"};
  const ir::Type_operand hi = ir::ImmLabelHigh{.name = "slot"};
  CHECK_FALSE(alias(lo, data("slot")));
  CHECK_FALSE(alias(data("slot"), hi));
  CHECK_FALSE(alias(lo, lo));
}

TEST_CASE("may_alias: zero page and Data") {
  CHECK(alias(zp(0x80), zp(0x80)));
  CHECK_FALSE(alias(zp(0x80), zp(0x81)));
  CHECK(alias(zp(0x80, 1), zp(0x81, 0)));
  CHECK_FALSE(alias(zp(0x80), data("g")));
  // DPTR is at $24 in the classic layout.
  CHECK_FALSE(alias(zp(0x80), data("DPTR")));
  CHECK(alias(zp(0x25), data("DPTR", 1)));
  CHECK(alias(data("g", 2), data("g", 2)));
  CHECK_FALSE(alias(data("g", 0), data("g", 1)));
  CHECK_FALSE(alias(data("g"), data("h")));
  // A slot resolves through the slot map, and without it may alias.
  const SymbolAddrs slots = {{"__local_f__x", 0x8A}};
  const MayAliasOptions with{.zp_slot_symbols = &slots};
  CHECK(alias(zp(0x8A), data("__local_f__x")));
  CHECK(alias(zp(0x8A), data("__local_f__x"), kCtx, with));
  CHECK_FALSE(alias(zp(0x8B), data("__local_f__x"), kCtx, with));
}

TEST_CASE("may_alias: the soft stack and pointers") {
  CHECK_FALSE(alias(frame(0), zp(0x80)));
  CHECK_FALSE(alias(zp(0x90), stack(0)));
  CHECK_FALSE(alias(indirect(0), zp(0x80)));
  CHECK_FALSE(alias(zp(0xC0), ir::IndirectY{}));
  CHECK(alias(indirect(0), zp(0x04)));
  CHECK(alias(indirect(0), zp(kDefaultPoolLo - 1)));
  CHECK(alias(indirect(0), zp(0x80), kCtx, MayAliasOptions{.pool_lo = 0x90}));
  CHECK_FALSE(alias(indirect(0), zp(0x90), kCtx, MayAliasOptions{.pool_lo = 0x90}));
  CHECK(alias(frame(3), frame(3)));
  CHECK_FALSE(alias(frame(3), frame(4)));
  CHECK_FALSE(alias(frame(5), stack(5)));
  CHECK(alias(indirect(2), indirect(2)));
  CHECK_FALSE(alias(indirect(0), indirect(1)));
  // One base is one pointer, and two bases may hold one address.
  CHECK(alias(izp(0x8A, 2), izp(0x8A, 2)));
  CHECK_FALSE(alias(izp(0x8A, 0), izp(0x8A, 1)));
  CHECK(alias(izp(0x8A, 0), izp(0x8C, 0)));
  CHECK(alias(izpy(0x8A), izpy(0x8C)));
  CHECK(alias(izp(0x8A, 4), izpy(0x8A)));
  CHECK(alias(izp(0x8A, 0), indirect(0)));
  // Through a zero-page base, no runtime symbol is reached, but DPTR is
  // the pointer of `Indirect`.
  CHECK_FALSE(alias(izpy(0x8A), data("DPTR")));
  CHECK(alias(indirect(0), data("DPTR")));
  CHECK_FALSE(alias(idata("", ir::X{}), zp(0x80)));
}

TEST_CASE("may_alias: the pointer proof") {
  const ir::Type_operand row = idata("T@lo");
  const ir::Type_operand through = izpy(0x90);
  CHECK(alias(row, through));
  const AsmContext proof = kCtx.with_pointer_proof({"T@lo"});
  CHECK_FALSE(alias(row, through, proof));
  CHECK_FALSE(alias(through, row, proof));
  CHECK_FALSE(alias(data("T@lo"), through, proof));
}

TEST_CASE("may_alias: a user zero-page object against an indexed access") {
  // Lode Runner's `GAME_COLNUM` at $85.
  AsmContext ctx;
  auto user = std::make_shared<UserZpGlobals>();
  user->addrs = {{"GAME_COLNUM", 0x85}, {"ZARR", 0x40}};
  user->bytes = {0x85, 0x40, 0x41, 0x42};
  ctx.user_zp = user;
  CHECK_FALSE(alias(data("GAME_COLNUM"), idata("LADDER_LOCS_ROW"), ctx));
  CHECK_FALSE(alias(idata("LADDER_LOCS_ROW"), data("GAME_COLNUM"), ctx));
  CHECK(alias(data("GAME_COLNUM"), idata("ZARR"), ctx));
  CHECK(alias(data("ZARR", 1), idata("ZARR"), ctx));
  CHECK(alias(data("G"), idata("LADDER_LOCS_ROW"), ctx));
  // A user zero-page byte is compared by its address, and a pointer may
  // reach it.
  CHECK(alias(zp(0x85), data("GAME_COLNUM"), ctx));
  CHECK(alias(izpy(0x90), zp(0x85), ctx));
  CHECK_FALSE(alias(izpy(0x90), zp(0x86), ctx));
}

TEST_CASE("unreachable_by_pointer") {
  ir::Program prog{.top_level = {
                       ir::StaticVariable{.name = "s", .is_global = false},
                       ir::StaticVariable{.name = "t", .is_global = false},
                       ir::StaticVariable{
                           .name = "p",
                           .is_global = true,
                           .init = {ir::AddressInit{.name = "t"}}},
                       ir::Function{.name = "f",
                                    .is_global = true,
                                    .instructions = {mov(data("__local_f__a"), kA),
                                                     mov(data("__local_g__b"), kA),
                                                     ir::Call{.name = "f__x"},
                                                     mov(data("__local_f__x__c"), kA),
                                                     ret()}},
                   }};
  // `t`'s address is taken. `__local_g__b` belongs to no function
  // defined here, and `__local_f__x__c` could be `f`'s or `f__x`'s.
  CHECK(unreachable_by_pointer(prog) == NameSet{"__local_f__a", "s"});
}

TEST_CASE("pointer bytes and slot pointers") {
  const List instrs = {mov(izpy(0x8A), kA), mov(kA, izp(0x90, 1))};
  const std::set<c6502::asdl::Int> bytes = pointer_byte_addresses(instrs);
  CHECK(bytes == std::set<c6502::asdl::Int>{0x8A, 0x8B, 0x90, 0x91});
  const SymbolAddrs slots = {{"__local_f__p", 0x8B}};
  CHECK(is_pointer_byte(zp(0x8B), bytes, nullptr));
  CHECK_FALSE(is_pointer_byte(zp(0x8C), bytes, nullptr));
  CHECK(is_pointer_byte(data("__local_f__p"), bytes, &slots));
  // A slot that isn't resolved may be a pointer byte.
  CHECK(is_pointer_byte(data("__local_f__q"), bytes, &slots));
  CHECK_FALSE(is_pointer_byte(data("g"), bytes, &slots));
  CHECK_FALSE(is_pointer_byte(zp(0x8B), {}, nullptr));
  CHECK(through_slot_pointer(instrs[0], {0x8B}));
  CHECK_FALSE(through_slot_pointer(instrs[0], {0x8C}));
}

TEST_CASE("decimal_arith gives the chain's operands") {
  const ir::DecimalArith d{.subtract = false,
                           .left = {data("a"), data("a", 1)},
                           .right = {imm(1), imm(0)},
                           .dst = {frame(1), frame(2)}};
  CHECK(decimal_arith::reads(d).size() == 4);
  CHECK(decimal_arith::writes(d) == d.dst);
  CHECK(decimal_arith::operands(d).size() == 6);
  CHECK(decimal_arith::writes_y(d));
  const ir::DecimalArith in_zp{.left = {zp(0x80)}, .right = {zp(0x81)}, .dst = {zp(0x80)}};
  CHECK_FALSE(decimal_arith::writes_y(in_zp));
  // The chain kills A and writes every flag.
  CHECK(kills_a(in_zp));
  CHECK(kills_y(d));
  CHECK(c_dead_at(List{in_zp, ir::Add{.src = imm(0), .dst = kA}}, 0));
  CHECK(decimal_arith::map_operands(in_zp, [](const ir::Type_operand&) {
          return data("z");
        }).dst == std::vector<ir::Type_operand>{data("z")});
}

TEST_CASE("mem_dead_after walks the function") {
  // A store to the function's own slot is dead past a call, which
  // can't reach it, and at the return.
  const List instrs = {mov(kA, data("__local_f__t")), ir::Call{.name = "g"},
                         ret()};
  CHECK(asm_dead_store::mem_dead_after(instrs, 0, data("__local_f__t"), kCtx, "f"));
  // A global is live at the return.
  CHECK_FALSE(asm_dead_store::mem_dead_after(instrs, 0, data("g"), kCtx, "f"));
  // Another function's slot is live across the call.
  CHECK_FALSE(asm_dead_store::mem_dead_after(instrs, 0, data("__local_h__t"), kCtx, "f"));
  // A slot whose address is taken is live across the call.
  const List taken = {mov(kA, data("__local_f__t")),
                        mov(ir::ImmLabelLow{.name = "__local_f__t"}, kA),
                        ir::Call{.name = "g"}, ret()};
  CHECK_FALSE(asm_dead_store::mem_dead_after(taken, 0, data("__local_f__t"), kCtx, "f"));
  // A DPTR byte dies at a direct call, and the `icall` trampoline reads
  // it.
  CHECK(asm_dead_store::mem_dead_after(
      List{mov(kA, data("DPTR")), ir::Call{.name = "g"}, mov(kA, data("g"))}, 0,
      data("DPTR"), kCtx, "f"));
  CHECK_FALSE(asm_dead_store::mem_dead_after(
      List{mov(kA, data("DPTR")), ir::Call{.name = "icall"}}, 0, data("DPTR"),
      kCtx, "f"));
  // A store through `(DPTR),Y` reads the pointer.
  CHECK_FALSE(asm_dead_store::mem_dead_after(
      List{mov(kA, data("DPTR")), mov(kA, ir::IndirectY{}), ret()}, 0, data("DPTR"),
      kCtx, "f"));
  // A second store of the same byte ends the path.
  CHECK(asm_dead_store::mem_dead_after(
      List{mov(kA, data("g")), mov(kX, data("g")), ret()}, 0, data("g"), kCtx, "f"));
  // A `LoadAddress` of a frame slot reads FP.
  const ir::Type_instruction la = ir::LoadAddress{.src = frame(2), .dst = zp(0x80)};
  CHECK(asm_dead_store::read_operands(la) ==
        std::vector<ir::Type_operand>{data("FP"), data("FP", 1)});
  CHECK(asm_dead_store::write_operand(la) == zp(0x80));
  CHECK(asm_dead_store::ptr_source_reads(izpy(0x8A)) ==
        std::vector<ir::Type_operand>{zp(0x8A), zp(0x8B)});
}

TEST_CASE("inc_peephole's operand tests") {
  CHECK(is_x_indexed(idata("t", ir::X{})));
  CHECK_FALSE(is_x_indexed(idata("t", ir::Y{})));
  CHECK(inc_peephole::is_inc_eligible_operand(kY));
  CHECK_FALSE(inc_peephole::is_inc_eligible_operand(kA));
  CHECK_FALSE(inc_peephole::is_inc_eligible_operand(frame(1)));
  CHECK(inc_peephole::operands_equal(idata("t", ir::X{}), idata("t", ir::X{})));
  CHECK_FALSE(inc_peephole::operands_equal(idata("t", ir::X{}), idata("t", ir::Y{})));
  CHECK_FALSE(inc_peephole::operands_equal(kX, kY));
}

TEST_CASE("dead_a_arith's flag tests") {
  // `LDX m; TXA` has N/Z from X already.
  const List instrs = {mov(data("m"), kX), mov(kA, data("n")), mov(kX, kA)};
  CHECK(dead_a_arith::flags_reflect_src_reg(instrs, 2));
  const List after_label = {mov(data("m"), kX), label(".l"), mov(kX, kA)};
  CHECK_FALSE(dead_a_arith::flags_reflect_src_reg(after_label, 2));
  CHECK(dead_a_arith::writes_reg_setting_n_z(ir::Add{.src = imm(1), .dst = kA}, ir::A{}));
  CHECK_FALSE(dead_a_arith::writes_reg_setting_n_z(mov(kA, data("m")), ir::A{}));
  CHECK(dead_a_arith::writes_reg_setting_n_z(ir::Dec{.dst = kY}, ir::Y{}));
}

TEST_CASE("build_cfg partitions and links blocks") {
  SUBCASE("an empty function goes from ENTRY to EXIT") {
    const Cfg cfg = build_cfg(List{});
    CHECK(cfg.block_order.empty());
    CHECK(cfg.blocks[kEntryId].successors == std::vector<int>{kExitId});
    CHECK(cfg.blocks[kExitId].predecessors == std::vector<int>{kEntryId});
  }
  SUBCASE("a branch has the target, then the fall-through") {
    const List instrs = {mov(imm(0), kA), branch(ir::EQ{}, ".t"), mov(imm(1), kA),
                           ret(true), label(".t"), ret(true)};
    const Cfg cfg = build_cfg(instrs);
    REQUIRE(cfg.block_order == std::vector<int>{2, 3, 4});
    CHECK(cfg.blocks[2].successors == std::vector<int>{4, 3});
    CHECK(cfg.instructions(4).size() == 2);
    CHECK(c6502::asdl::isa<ir::Label>(cfg.instructions(4)[0]));
  }
  SUBCASE("a call doesn't split a block") {
    const List instrs = {mov(imm(0), kA), ir::Call{.name = "g"}, mov(imm(1), kA),
                           ret()};
    const Cfg cfg = build_cfg(instrs);
    CHECK(cfg.block_order.size() == 1);
    CHECK(cfg.instructions(2).size() == 4);
  }
  SUBCASE("a loop's back edge") {
    const List instrs = {label(".l"), ir::Dec{.dst = kX}, branch(ir::NE{}, ".l"),
                           ret()};
    const Cfg cfg = build_cfg(instrs);
    CHECK(cfg.blocks[2].successors == std::vector<int>{2, 3});
    CHECK(cfg.blocks[2].predecessors == std::vector<int>{kEntryId, 2});
  }
  SUBCASE("no terminator falls through to EXIT") {
    const Cfg cfg = build_cfg(List{mov(imm(0), kA)});
    CHECK(cfg.blocks[2].successors == std::vector<int>{kExitId});
  }
  SUBCASE("cfg_to_function gives the instructions back") {
    const ir::Function fn{.name = "main",
                          .is_global = true,
                          .instructions = {mov(imm(0), kA), label(".l"), ret()}};
    const ir::Function out = cfg_to_function(fn, build_cfg(fn));
    CHECK(out == fn);
  }
  SUBCASE("a jump to a missing label") {
    CHECK_THROWS_AS(build_cfg(List{ir::Jump{.target = "f"}}), c6502::InternalError);
    // The tolerant builder gives the target a synthetic block that
    // returns.
    const ir::Function fn{.name = "g", .instructions = {ir::Jump{.target = "f"}}};
    const Cfg cfg = build_cfg_tolerant(fn);
    REQUIRE(cfg.block_order == std::vector<int>{2, 3});
    CHECK(cfg.blocks[2].successors == std::vector<int>{3});
    CHECK(cfg.blocks[3].successors == std::vector<int>{kExitId});
    CHECK(cfg.instructions(3).size() == 2);
  }
}

TEST_CASE("the dominators of a diamond") {
  // b0: branch to b2; b1: jump to b3; b2: fall to b3; b3: return.
  const List instrs = {branch(ir::EQ{}, ".else"), mov(imm(1), kA),
                         ir::Jump{.target = ".end"}, label(".else"),
                         mov(imm(2), kA), label(".end"), ret(true)};
  const Cfg cfg = build_cfg(instrs);
  REQUIRE(cfg.block_order == std::vector<int>{2, 3, 4, 5});
  const Dominators doms = immediate_dominators(cfg);
  CHECK(doms.idom[5] == 2);
  CHECK(doms.idom[3] == 2);
  CHECK(doms.idom[4] == 2);
  const auto df = dominance_frontiers(cfg);
  CHECK(df[3] == std::set<int>{5});
  CHECK(df[4] == std::set<int>{5});
  CHECK(df[2].empty());
  const auto children = dominator_tree_children(doms);
  CHECK(children[kEntryId] == std::vector<int>{2});
}

TEST_CASE("call_edges leaves out a call that abandons its caller") {
  const tac::Type_val one = tac::Constant{.const_ = tac::ConstInt{.value = 1}};
  const tac::Function fn{
      .name = "f",
      .instructions = {tac::FunctionCall{.name = "reset", .noreturn = true},
                       tac::FunctionCall{.name = "g"},
                       tac::FunctionCall{.name = "h", .args = {one}, .noreturn = true}}};
  CHECK(abandons_caller(fn.instructions[0]));
  CHECK_FALSE(abandons_caller(fn.instructions[1]));
  CHECK_FALSE(abandons_caller(fn.instructions[2]));
  const auto calls = graph_calls(fn);
  REQUIRE(calls.size() == 2);
  CHECK(calls[0]->name == "g");
  CHECK(calls[1]->name == "h");
}

TEST_CASE("instruction_size") {
  using c6502::sim::instruction_size;
  CHECK(instruction_size(mov(data("g"), kA)) == 3);
  CHECK(instruction_size(mov(data("HARGS", 2), kA)) == 2);
  CHECK(instruction_size(mov(zp(0x80), data("g"))) == 5);
  CHECK(instruction_size(mov(kX, kY)) == 2);
  CHECK(instruction_size(mov(kX, frame(3))) == 5);
  CHECK(instruction_size(mov(data("g"), data("g"))) == 0);
  CHECK(instruction_size(label(".l")) == 0);
  CHECK(instruction_size(ir::Compare{.left = kA, .right = idata("t")}) == 3);
  CHECK(instruction_size(ir::ArithmeticShiftLeft{.dst = frame(1)}) == 9);
  CHECK(instruction_size(ir::Ret{.arg_bytes = 1, .save_a = true}) == 13 + 14 + 2 + 1);
  CHECK(instruction_size(ir::FunctionPrologue{.local_bytes = 1}) == 13 + 12 + 8);
  CHECK(instruction_size(ir::JumpTable{.targets = {"a", "b"}}) == 15);
  const c6502::passes::NameSet zp_names = {"__local_f__x"};
  CHECK(instruction_size(mov(kA, data("__local_f__x")), &zp_names) == 2);
  CHECK(instruction_size(mov(kA, data("__local_f__x"))) == 3);
  CHECK_THROWS_AS(instruction_size(mov(ir::Pseudo{.name = "p"}, kA)),
                  c6502::InternalError);
  CHECK_THROWS_AS(instruction_size(ir::Inc{.dst = kA}), c6502::InternalError);
}
