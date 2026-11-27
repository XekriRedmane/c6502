// tests/test_asm_aliasing.py's tests of `may_alias`, ported by hand,
// one case per Python test with each of its assertions. The Python
// tests' module-level state, `set_user_zp_globals` and the
// `pointer_proof` block, is the `AsmContext` that each call takes here.
#include <memory>

#include "asm_ast.h"
#include "doctest.h"
#include "passes/asm_aliasing.h"
#include "passes/asm_context.h"

namespace ir = c6502::asm_ast;
using c6502::passes::AsmContext;
using c6502::passes::kDefaultPoolLo;
using c6502::passes::MayAliasOptions;

namespace {

ir::Type_operand zp(int addr, int off = 0) { return ir::ZP{.address = addr, .offset = off}; }
ir::Type_operand data(const char* name, int off = 0) {
  return ir::Data{.name = name, .offset = off};
}
ir::Type_operand idata(const char* name, int off = 0, ir::Type_reg idx = ir::X{}) {
  return ir::IndexedData{.name = name, .offset = off, .index = idx};
}
ir::Type_operand frame(int off) { return ir::Frame{.offset = off}; }
ir::Type_operand stack(int off) { return ir::Stack{.offset = off}; }
ir::Type_operand indirect(int off) { return ir::Indirect{.offset = off}; }
ir::Type_operand indy() { return ir::IndirectY{}; }
ir::Type_operand izp(int addr, int off) {
  return ir::IndirectZp{.address = addr, .offset = off};
}
ir::Type_operand izpy(int addr) { return ir::IndirectZpY{.address = addr}; }

const AsmContext kCtx{};

bool alias(const ir::Type_operand& a, const ir::Type_operand& b,
           const AsmContext& ctx = kCtx, const MayAliasOptions& o = {}) {
  return c6502::passes::may_alias(a, b, ctx, o);
}

}  // namespace

TEST_CASE("asm_aliasing: TestImmediates::test_imm_never_aliases_anything") {
  const ir::Type_operand imm = ir::Imm{.value = 5};
  CHECK_FALSE(alias(imm, imm));
  CHECK_FALSE(alias(imm, zp(0x80)));
  CHECK_FALSE(alias(data("DPTR"), imm));
}

TEST_CASE("asm_aliasing: TestImmediates::test_imm_label_low_never_aliases_anything") {
  // `ImmLabelLow(name)` is `LDA #<name`, a load of the low byte of
  // `&name`, which reads no memory, even when the names match.
  const ir::Type_operand iml = ir::ImmLabelLow{.name = "slot", .offset = 0};
  CHECK_FALSE(alias(iml, data("slot")));
  CHECK_FALSE(alias(data("slot"), iml));
  CHECK_FALSE(alias(iml, zp(0x80)));
  CHECK_FALSE(alias(iml, iml));
}

TEST_CASE("asm_aliasing: TestImmediates::test_imm_label_high_never_aliases_anything") {
  const ir::Type_operand imh = ir::ImmLabelHigh{.name = "slot", .offset = 0};
  CHECK_FALSE(alias(imh, data("slot")));
  CHECK_FALSE(alias(data("slot"), imh));
  CHECK_FALSE(alias(imh, zp(0x80)));
}

TEST_CASE("asm_aliasing: TestZpVsZp::test_same_address_aliases") {
  CHECK(alias(zp(0x80), zp(0x80)));
}

TEST_CASE("asm_aliasing: TestZpVsZp::test_different_addresses_do_not_alias") {
  CHECK_FALSE(alias(zp(0x80), zp(0x81)));
}

TEST_CASE("asm_aliasing: TestZpVsZp::test_offset_resolves_to_same_byte") {
  // ZP(0x80, off=1) and ZP(0x81, off=0) both name byte $81.
  CHECK(alias(zp(0x80, 1), zp(0x81, 0)));
}

TEST_CASE("asm_aliasing: TestZpVsData::test_zp_and_data_are_disjoint_namespaces") {
  CHECK_FALSE(alias(zp(0x80), data("g")));
  CHECK_FALSE(alias(zp(0x80), data("DPTR")));
}

TEST_CASE("asm_aliasing: TestDataVsData::test_same_name_offset_aliases") {
  CHECK(alias(data("g", 2), data("g", 2)));
}

TEST_CASE("asm_aliasing: TestDataVsData::test_different_offsets_do_not_alias") {
  CHECK_FALSE(alias(data("g", 0), data("g", 1)));
}

TEST_CASE("asm_aliasing: TestDataVsData::test_different_names_do_not_alias") {
  CHECK_FALSE(alias(data("g"), data("h")));
}

// Frame and Stack reads go through FP and SSP, which point into the soft
// stack in main RAM, so they never reach zero page.
TEST_CASE("asm_aliasing: TestFrameStackVsZp::test_frame_does_not_alias_zp") {
  CHECK_FALSE(alias(frame(0), zp(0x80)));
  CHECK_FALSE(alias(zp(0x84), frame(5)));
  CHECK_FALSE(alias(zp(0x00), frame(0)));
}

TEST_CASE("asm_aliasing: TestFrameStackVsZp::test_stack_does_not_alias_zp") {
  CHECK_FALSE(alias(stack(3), zp(0x80)));
  CHECK_FALSE(alias(zp(0x90), stack(0)));
}

// Indirect and IndirectY go through DPTR, which holds a user pointer.
// No user pointer points into the register allocator's pool ($80-$FF by
// default), because a local whose address is taken goes to the frame.
TEST_CASE("asm_aliasing: TestIndirectVsZpPool::test_indirect_does_not_alias_pool_zp") {
  CHECK_FALSE(alias(indirect(0), zp(0x80)));
  CHECK_FALSE(alias(zp(0x84), indirect(7)));
  CHECK_FALSE(alias(indirect(0), zp(0xFF)));
}

TEST_CASE("asm_aliasing: TestIndirectVsZpPool::test_indirect_y_does_not_alias_pool_zp") {
  CHECK_FALSE(alias(indy(), zp(0x80)));
  CHECK_FALSE(alias(zp(0xC0), indy()));
}

TEST_CASE("asm_aliasing: TestIndirectVsZpPool::test_indirect_does_alias_non_pool_zp") {
  // Below the pool are HARGS, DPTR, FP, and SSP, which a user pointer
  // could reach.
  CHECK(alias(indirect(0), zp(0x04)));
  CHECK(alias(zp(0x24), indy()));
}

TEST_CASE("asm_aliasing: TestIndirectVsZpPool::test_pool_boundary") {
  CHECK_FALSE(alias(indirect(0), zp(kDefaultPoolLo)));
  CHECK(alias(indirect(0), zp(kDefaultPoolLo - 1)));
}

TEST_CASE("asm_aliasing: TestIndirectVsZpPool::test_custom_pool_range") {
  // With the pool starting at $90, $80..$8F isn't in it.
  CHECK(alias(indirect(0), zp(0x80), kCtx, MayAliasOptions{.pool_lo = 0x90}));
  CHECK_FALSE(alias(indirect(0), zp(0x90), kCtx, MayAliasOptions{.pool_lo = 0x90}));
}

TEST_CASE("asm_aliasing: TestSameKindIndirects::test_frame_same_offset_aliases") {
  CHECK(alias(frame(3), frame(3)));
}

TEST_CASE("asm_aliasing: TestSameKindIndirects::test_frame_different_offset_does_not_alias") {
  CHECK_FALSE(alias(frame(3), frame(4)));
}

TEST_CASE("asm_aliasing: TestSameKindIndirects::test_stack_same_offset_aliases") {
  CHECK(alias(stack(0), stack(0)));
}

TEST_CASE("asm_aliasing: TestSameKindIndirects::test_stack_different_offset_does_not_alias") {
  CHECK_FALSE(alias(stack(0), stack(1)));
}

TEST_CASE("asm_aliasing: TestSameKindIndirects::test_indirect_same_offset_aliases") {
  CHECK(alias(indirect(2), indirect(2)));
}

TEST_CASE("asm_aliasing: TestSameKindIndirects::test_indirect_different_offset_does_not_alias") {
  CHECK_FALSE(alias(indirect(0), indirect(1)));
}

TEST_CASE("asm_aliasing: TestSameKindIndirects::test_frame_vs_stack_do_not_alias") {
  // FP and SSP are separate pointers.
  CHECK_FALSE(alias(frame(5), stack(5)));
  CHECK_FALSE(alias(stack(0), frame(0)));
}

// `(zp),Y` through a pointer kept in zero page. One base is one pointer,
// so its offsets decide, and two bases are two pointers that may hold
// the same address.
TEST_CASE("asm_aliasing: TestZeroPagePointers::test_same_base_same_offset_aliases") {
  CHECK(alias(izp(0x8A, 2), izp(0x8A, 2)));
}

TEST_CASE("asm_aliasing: TestZeroPagePointers::test_same_base_different_offset_does_not_alias") {
  CHECK_FALSE(alias(izp(0x8A, 0), izp(0x8A, 1)));
}

TEST_CASE("asm_aliasing: TestZeroPagePointers::test_different_bases_may_alias") {
  // `p = q; *p = 1; return *q;` reads the byte it just wrote.
  CHECK(alias(izp(0x8A, 0), izp(0x8C, 0)));
  CHECK(alias(izp(0x8A, 0), izp(0x8C, 3)));
  CHECK(alias(izpy(0x8A), izpy(0x8C)));
  CHECK(alias(izp(0x8A, 0), izpy(0x8C)));
}

TEST_CASE("asm_aliasing: TestZeroPagePointers::test_same_base_indexed_aliases") {
  CHECK(alias(izpy(0x8A), izpy(0x8A)));
  CHECK(alias(izp(0x8A, 4), izpy(0x8A)));
}

TEST_CASE("asm_aliasing: TestZeroPagePointers::test_dptr_pointer_may_alias") {
  CHECK(alias(izp(0x8A, 0), indirect(0)));
  CHECK(alias(indy(), izpy(0x8A)));
}

TEST_CASE("asm_aliasing: TestIndexedData::test_indexed_data_vs_zp") {
  // An absolute indexed access doesn't reach zero page.
  CHECK_FALSE(alias(idata("", 0x4000), zp(0x80)));
  CHECK_FALSE(alias(zp(0xFF), idata("g", 0)));
}

TEST_CASE("asm_aliasing: TestIndexedStaticAgainstPointer::test_pointer_proof") {
  // An indexed access to an object no pointer can reach doesn't alias a
  // store through a pointer.
  const ir::Type_operand row = idata("T@lo");
  const ir::Type_operand through = izpy(0x90);
  CHECK(alias(row, through));
  const AsmContext proof = kCtx.with_pointer_proof({"T@lo"});
  CHECK_FALSE(alias(row, through, proof));
  CHECK_FALSE(alias(through, row, proof));
}

namespace {

// The context of `TestUserZeroPageObjectAgainstIndexed`'s `setUp`, with
// Lode Runner's `GAME_COLNUM` at $85 and a 3-byte `ZARR` at $40.
AsmContext user_zp_context() {
  AsmContext ctx;
  auto user = std::make_shared<c6502::passes::UserZpGlobals>();
  user->addrs = {{"GAME_COLNUM", 0x85}, {"ZARR", 0x40}};
  user->bytes = {0x85, 0x40, 0x41, 0x42};
  ctx.user_zp = user;
  return ctx;
}

}  // namespace

TEST_CASE("asm_aliasing: TestUserZeroPageObjectAgainstIndexed::test_other_object_outside_zero_page") {
  const AsmContext ctx = user_zp_context();
  const ir::Type_operand col = data("GAME_COLNUM");
  const ir::Type_operand rows = idata("LADDER_LOCS_ROW");
  CHECK_FALSE(alias(col, rows, ctx));
  CHECK_FALSE(alias(rows, col, ctx));
}

TEST_CASE("asm_aliasing: TestUserZeroPageObjectAgainstIndexed::test_indexed_zero_page_object_may_reach_it") {
  const AsmContext ctx = user_zp_context();
  CHECK(alias(data("GAME_COLNUM"), idata("ZARR"), ctx));
}

TEST_CASE("asm_aliasing: TestUserZeroPageObjectAgainstIndexed::test_same_object_indexed") {
  const AsmContext ctx = user_zp_context();
  CHECK(alias(data("ZARR", 1), idata("ZARR"), ctx));
}

TEST_CASE("asm_aliasing: TestUserZeroPageObjectAgainstIndexed::test_not_a_zero_page_object") {
  // Without a zero-page binding, `G` may be any byte, which an indexed
  // access may reach.
  const AsmContext ctx = user_zp_context();
  CHECK(alias(data("G"), idata("LADDER_LOCS_ROW"), ctx));
}

TEST_CASE("asm_aliasing: test_divide_shapes.py::TestStoresAndIndexedAccesses::test_indexed_local_aliases_its_bytes") {
  const ir::Type_operand local = data("__local_f__d", 2);
  const ir::Type_operand indexed = idata("__local_f__d", 0, ir::Y{});
  CHECK(alias(local, indexed));
  CHECK(alias(indexed, local));
  CHECK(alias(zp(0x84), indexed));
  CHECK_FALSE(alias(data("__local_f__e"), indexed));
}
