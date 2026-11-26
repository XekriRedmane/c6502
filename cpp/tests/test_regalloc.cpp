// The register allocation of the asm optimizer: liveness, interference,
// move coalescing, the eligibility for X and Y, and the coloring. The
// tests of coalescing and of the eligibility are the direct calls of
// tests/test_coalescing.py, tests/test_carry_inc.py, and
// tests/test_hwreg_pinning.py. The tests of the coloring check the
// results the Python functions gave on the same inputs, among them the
// SSA forms that `to_ssa` gave the functions of tests/test_asm_regalloc.py
// and tests/test_pointer_aware_regalloc.py, and the paths the corpus
// reaches least: a `register` local that can't be pinned, a spill, a
// packed BCD chain, a pair of carry bytes, and a call that preserves Y.
#include <string>
#include <string_view>

#include "asm_text.h"
#include "doctest.h"
#include "errors.h"
#include "passes/optimization/register_allocation.h"
#include "passes/optimization_asm/coalescing.h"
#include "passes/optimization_asm/hwreg_eligibility.h"
#include "passes/optimization_asm/interference.h"
#include "passes/optimization_asm/liveness.h"
#include "passes/optimization_asm/regalloc.h"
#include "passes/optimization_asm/regalloc_stage.h"
#include "record_case.h"
#include "tagged_json.h"

namespace {

using namespace c6502;
using passes::InterferenceGraph;
using passes::InterferenceNode;
using passes::NameSet;

asm_ast::Function function(std::string_view instrs) {
  auto prog = asm_text::program(instrs, "f");
  return std::get<asm_ast::Function>(prog.top_level.at(0));
}

void add_node(InterferenceGraph& g, const std::string& name, int width = 1, bool lac = false) {
  g.nodes[name] = InterferenceNode{.name = name, .width = width, .lives_across_call = lac};
}

InterferenceGraph graph_with(std::initializer_list<const char*> names) {
  InterferenceGraph g;
  for (const char* n : names) {
    add_node(g, n);
  }
  return g;
}

std::string graph_json(const InterferenceGraph& g) {
  return tagged::encode<tagged::InterferenceGraphC>(g);
}

std::string coloring_json(const passes::Coloring& c) {
  return tagged::encode<tagged::ColoringC>(c);
}

std::string carry_byte(std::string_view src, std::string_view dst, bool sub = false) {
  return "Mov(" + std::string(src) + " Reg(A) false) " + (sub ? "Sub" : "Add") +
         "(Imm(0) Reg(A)) Mov(Reg(A) " + std::string(dst) + " false) ";
}

// --- Coalescing ---

TEST_CASE("coalescing: a move between two names that don't interfere merges them") {
  auto g = graph_with({"a", "b"});
  const auto r = passes::coalesce_moves(function(R"(Mov(Pseudo("a" 0) Pseudo("b" 0) false))"), g);
  CHECK(g.nodes.size() == 1);
  CHECK(g.nodes.contains("a"));
  CHECK(r.resolve("b") == "a");
  CHECK(r.resolve("a") == "a");
}

TEST_CASE("coalescing: a copy through A merges its ends") {
  auto g = graph_with({"a", "b"});
  const auto r = passes::coalesce_moves(
      function(R"(Mov(Pseudo("a" 0) Reg(A) false) Mov(Reg(A) Pseudo("b" 0) false))"), g);
  CHECK_FALSE(g.nodes.contains("b"));
  CHECK(r.resolve("b") == "a");
}

TEST_CASE("coalescing: a copy through A with a volatile half isn't merged") {
  auto g = graph_with({"a", "b"});
  passes::coalesce_moves(
      function(R"(Mov(Pseudo("a" 0) Reg(A) true) Mov(Reg(A) Pseudo("b" 0) false))"), g);
  CHECK(g.nodes.contains("a"));
  CHECK(g.nodes.contains("b"));
}

TEST_CASE("coalescing: a Phi's arguments merge with its destination") {
  auto g = graph_with({"phi", "a", "b"});
  const auto r = passes::coalesce_moves(
      function(R"(Phi(Pseudo("phi" 0) [AsmPhiArg("L1" Pseudo("a" 0)) AsmPhiArg("L2" Pseudo("b" 0))]))"),
      g);
  REQUIRE(g.nodes.size() == 1);
  const std::string survivor = g.nodes.begin()->first;
  CHECK(r.resolve("phi") == survivor);
  CHECK(r.resolve("a") == survivor);
  CHECK(r.resolve("b") == survivor);
}

TEST_CASE("coalescing: an interfering pair isn't merged") {
  auto g = graph_with({"a", "b"});
  g.add_edge("a", "b");
  const auto r = passes::coalesce_moves(function(R"(Mov(Pseudo("a" 0) Pseudo("b" 0) false))"), g);
  CHECK(g.nodes.size() == 2);
  CHECK(r.representative.empty());
}

TEST_CASE("coalescing: a copy into byte 1 of a block merges at that byte") {
  InterferenceGraph g;
  add_node(g, "a");
  add_node(g, "b", 2);
  const auto r = passes::coalesce_moves(function(R"(Mov(Pseudo("a" 0) Pseudo("b" 1) false))"), g);
  CHECK_FALSE(g.nodes.contains("a"));
  CHECK(r.resolve("a") == "b");
  CHECK(r.offset_of("a") == 1);
}

namespace byte_level {

InterferenceGraph pointer_and_s() {
  InterferenceGraph g;
  add_node(g, "p", 2);
  add_node(g, "s");
  g.add_edge("p", "s");
  return g;
}

}  // namespace byte_level

TEST_CASE("coalescing: the check by bytes allows a byte that is dead") {
  auto g = byte_level::pointer_and_s();
  const auto r = passes::coalesce_moves(function(R"(
    Mov(Pseudo("p" 0) Reg(A) false)
    Mov(Imm(7) Pseudo("s" 0) false)
    Mov(Pseudo("p" 1) Reg(A) false)
    Mov(Pseudo("s" 0) Pseudo("p" 0) false)
    Mov(Pseudo("p" 0) Reg(A) false)
    Return(true false false))"),
                                        g);
  CHECK(r.resolve("s") == "p");
  CHECK(r.offset_of("s") == 0);
}

TEST_CASE("coalescing: the check by bytes rejects a byte that is live") {
  auto g = byte_level::pointer_and_s();
  const auto r = passes::coalesce_moves(function(R"(
    Mov(Imm(7) Pseudo("s" 0) false)
    Mov(Pseudo("p" 0) Reg(A) false)
    Mov(Pseudo("s" 0) Pseudo("p" 0) false)
    Mov(Pseudo("p" 0) Reg(A) false)
    Return(true false false))"),
                                        g);
  CHECK(r.resolve("s") == "s");
}

TEST_CASE("coalescing: the check by bytes allows a copy of a live byte") {
  auto g = byte_level::pointer_and_s();
  const auto r = passes::coalesce_moves(function(R"(
    Mov(Imm(7) Pseudo("s" 0) false)
    Mov(Imm(1) Pseudo("p" 1) false)
    Mov(Pseudo("s" 0) Pseudo("p" 0) false)
    Mov(Pseudo("p" 1) Reg(A) false)
    Mov(Pseudo("s" 0) Reg(A) false)
    Mov(Pseudo("p" 0) Reg(A) false)
    Return(true false false))"),
                                        g);
  CHECK(r.resolve("s") == "p");
}

TEST_CASE("coalescing: the check by bytes rejects a later write") {
  auto g = byte_level::pointer_and_s();
  const auto r = passes::coalesce_moves(function(R"(
    Mov(Imm(7) Pseudo("s" 0) false)
    Mov(Imm(1) Pseudo("p" 1) false)
    Mov(Pseudo("s" 0) Pseudo("p" 0) false)
    Mov(Imm(9) Pseudo("p" 0) false)
    Mov(Pseudo("p" 1) Reg(A) false)
    Mov(Pseudo("s" 0) Reg(A) false)
    Mov(Pseudo("p" 0) Reg(A) false)
    Return(true false false))"),
                                        g);
  CHECK(r.resolve("s") == "s");
}

TEST_CASE("coalescing: a name outside the graph isn't merged") {
  auto g = graph_with({"a"});
  const auto r =
      passes::coalesce_moves(function(R"(Mov(Pseudo("static_name" 0) Pseudo("a" 0) false))"), g);
  CHECK(g.nodes.contains("a"));
  CHECK(r.representative.empty());
}

TEST_CASE("coalescing: chained merges resolve transitively") {
  auto g = graph_with({"a", "b", "c"});
  const auto r = passes::coalesce_moves(
      function(R"(Mov(Pseudo("a" 0) Pseudo("b" 0) false) Mov(Pseudo("b" 0) Pseudo("c" 0) false))"),
      g);
  REQUIRE(g.nodes.size() == 1);
  const std::string survivor = g.nodes.begin()->first;
  CHECK(r.resolve("a") == survivor);
  CHECK(r.resolve("b") == survivor);
  CHECK(r.resolve("c") == survivor);
}

TEST_CASE("coalescing: a pair merged already is left alone") {
  auto g = graph_with({"a", "b"});
  passes::coalesce_moves(
      function(R"(Mov(Pseudo("a" 0) Pseudo("b" 0) false) Mov(Pseudo("b" 0) Pseudo("a" 0) false))"),
      g);
  CHECK(g.nodes.size() == 1);
}

TEST_CASE("coalescing: lives_across_call is the OR of the two") {
  InterferenceGraph g;
  add_node(g, "a", 1, false);
  add_node(g, "b", 1, true);
  passes::coalesce_moves(function(R"(Mov(Pseudo("a" 0) Pseudo("b" 0) false))"), g);
  REQUIRE(g.nodes.size() == 1);
  CHECK(g.nodes.begin()->second.lives_across_call);
}

TEST_CASE("coalescing: the merged name's neighbors move to the survivor") {
  auto g = graph_with({"a", "b", "c"});
  g.add_edge("b", "c");
  passes::coalesce_moves(function(R"(Mov(Pseudo("a" 0) Pseudo("b" 0) false))"), g);
  CHECK(g.adj.at("a").contains("c"));
  CHECK(g.adj.at("c").contains("a"));
  CHECK_FALSE(g.adj.at("c").contains("b"));
  CHECK_FALSE(g.adj.contains("b"));
}

TEST_CASE("coalescing: a pseudo at a nonzero offset isn't a move pair") {
  auto g = graph_with({"a", "b"});
  const auto r = passes::coalesce_moves(function(R"(Mov(Pseudo("a" 0) Pseudo("b" 1) false))"), g);
  CHECK(g.nodes.size() == 2);
  CHECK(r.representative.empty());
}

TEST_CASE("coalescing: a carry byte merges into a byte of a block") {
  InterferenceGraph g;
  add_node(g, "t", 2);
  add_node(g, "s");
  const auto r =
      passes::coalesce_moves(function(carry_byte(R"(Pseudo("t" 1))", R"(Pseudo("s" 0))")), g);
  CHECK(r.resolve("s") == "t");
  CHECK(r.offset_of("s") == 1);
  CHECK_FALSE(g.nodes.contains("s"));
}

TEST_CASE("coalescing: a carry byte between two single bytes merges them") {
  auto g = graph_with({"a", "b"});
  const auto r = passes::coalesce_moves(
      function(carry_byte(R"(Pseudo("a" 0))", R"(Pseudo("b" 0))", true)), g);
  CHECK(r.resolve("a") == r.resolve("b"));
  CHECK(r.offset_of("a") == 0);
  CHECK(r.offset_of("b") == 0);
}

TEST_CASE("coalescing: an interfering carry byte isn't merged") {
  InterferenceGraph g;
  add_node(g, "t", 2);
  add_node(g, "s");
  g.add_edge("t", "s");
  const auto r = passes::coalesce_moves(
      function(carry_byte(R"(Pseudo("t" 1))", R"(Pseudo("s" 0))") +
               R"(Mov(Pseudo("t" 1) Reg(A) false) Mov(Pseudo("s" 0) Reg(A) false)
                  Return(false false false))"),
      g);
  CHECK(r.resolve("s") == "s");
}

TEST_CASE("coalescing: the byte offset survives the compression of paths") {
  InterferenceGraph g;
  add_node(g, "t", 2);
  add_node(g, "s");
  add_node(g, "c");
  const auto r = passes::coalesce_moves(
      function(R"(Mov(Pseudo("s" 0) Pseudo("c" 0) false) )" +
               carry_byte(R"(Pseudo("t" 1))", R"(Pseudo("s" 0))")),
      g);
  CHECK(r.resolve("c") == "t");
  CHECK(r.offset_of("c") == 1);
  CHECK(r.offset_of("s") == 1);
}

// --- The eligibility for X and Y ---

passes::HwRegEligibility scan(std::string_view instrs) {
  return passes::scan_function(function(instrs));
}

TEST_CASE("hwreg: the setup of an index in X gives an X hint") {
  const auto e = scan(R"(Mov(Pseudo("P" 0) Reg(A) false) Mov(Reg(A) Reg(X) false)
                         Mov(IndexedData("arr" 0 X) Reg(A) false))");
  CHECK(e.hints_x.contains("P"));
  CHECK_FALSE(e.hints_y.contains("P"));
}

TEST_CASE("hwreg: the setup of an index in Y gives a Y hint") {
  const auto e = scan(R"(Mov(Pseudo("P" 0) Reg(A) false) Mov(Reg(A) Reg(Y) false)
                         Mov(IndexedData("arr" 0 Y) Reg(A) false))");
  CHECK(e.hints_y.contains("P"));
  CHECK_FALSE(e.hints_x.contains("P"));
}

TEST_CASE("hwreg: an operand of an add disqualifies") {
  const auto e = scan(R"(Add(Pseudo("P" 0) Reg(A)))");
  CHECK_FALSE(e.eligible().contains("P"));
}

TEST_CASE("hwreg: a load from indirect memory disqualifies") {
  for (const char* src : {"Frame(1)", "Stack(1)", "Indirect(0)"}) {
    CAPTURE(src);
    const auto e = scan(std::string("Mov(") + src + R"( Pseudo("P" 0) false))");
    CHECK_FALSE(e.eligible().contains("P"));
  }
}

TEST_CASE("hwreg: a store to indirect memory disqualifies") {
  const auto e = scan(R"(Mov(Imm(3) Pseudo("P" 0) false) Mov(Pseudo("P" 0) Frame(1) false))");
  CHECK_FALSE(e.eligible().contains("P"));
}

TEST_CASE("hwreg: an address-taken peer disqualifies") {
  const auto e = scan(R"(LoadAddress(Pseudo("E" 0) Pseudo("PTR" 0))
                         Mov(Pseudo("E" 1) Pseudo("P" 0) false)
                         Mov(Pseudo("P" 0) Reg(A) false) Mov(Reg(A) Reg(X) false))");
  CHECK_FALSE(e.eligible().contains("P"));
}

TEST_CASE("hwreg: a colorable pseudo peer stays eligible") {
  const auto e =
      scan(R"(Mov(Pseudo("Q" 0) Pseudo("P" 0) false) Mov(Pseudo("P" 0) Reg(A) false))");
  CHECK(e.eligible_x.contains("P"));
}

TEST_CASE("hwreg: the use count counts references") {
  const auto e = scan(R"(Mov(Pseudo("P" 0) Reg(A) false) Mov(Reg(A) Reg(X) false)
                         Mov(Pseudo("P" 0) Reg(A) false) Mov(Reg(A) Reg(Y) false))");
  REQUIRE(e.use_count.find(std::string("P")) != nullptr);
  CHECK(*e.use_count.find(std::string("P")) == 2);
}

TEST_CASE("hwreg: a load from an element indexed by X goes in Y only") {
  const auto e = scan(R"(Mov(IndexedData("arr" 0 X) Pseudo("P" 0) false)
                         Mov(Pseudo("P" 0) Reg(A) false) Mov(Reg(A) Reg(X) false)
                         Mov(IndexedData("arr2" 0 X) Reg(A) false))");
  CHECK_FALSE(e.eligible_x.contains("P"));
  CHECK(e.eligible_y.contains("P"));
  CHECK(e.hints_x.contains("P"));
}

TEST_CASE("hwreg: a load from an element indexed by Y goes in X only") {
  const auto e = scan(R"(Mov(IndexedData("arr" 0 Y) Pseudo("P" 0) false)
                         Mov(Pseudo("P" 0) Reg(A) false))");
  CHECK(e.eligible_x.contains("P"));
  CHECK_FALSE(e.eligible_y.contains("P"));
}

TEST_CASE("hwreg: eligible() is the union of the two sets") {
  const auto e = scan(R"(Mov(IndexedData("arr" 0 X) Pseudo("P" 0) false)
                         Mov(Pseudo("P" 0) Reg(A) false))");
  CHECK_FALSE(e.eligible_x.contains("P"));
  CHECK(e.eligible_y.contains("P"));
  CHECK(e.eligible().contains("P"));
}

TEST_CASE("hwreg: a store to an indexed element disqualifies") {
  const auto e = scan(R"(Mov(Imm(5) Pseudo("P" 0) false)
                         Mov(Pseudo("P" 0) IndexedData("arr" 0 X) false))");
  CHECK_FALSE(e.eligible_x.contains("P"));
  CHECK_FALSE(e.eligible_y.contains("P"));
}

TEST_CASE("hwreg: every operand of a BCD chain disqualifies") {
  const auto e = scan(R"(DecimalArith(false [Pseudo("a" 0)] [Pseudo("b" 0)] [Pseudo("d" 0)])
                         Mov(Pseudo("d" 0) Reg(A) false))");
  CHECK(e.eligible().empty());
}

TEST_CASE("hwreg: register_pins and register_hints match each SSA version of a base") {
  const OrderedMap<std::string, std::string> pins{{"@1.y", "Y"}};
  const OrderedMap<std::string, std::string> hints{{"@2.n", "X"}};
  const auto e = passes::scan_function(function(R"(
    Mov(Imm(0) Pseudo("@1.y.3.b0.v1" 0) false)
    Inc(Pseudo("@1.y.3.b0.v1" 0))
    Mov(Pseudo("@2.n.b0.v2" 0) Reg(A) false)
    Mov(Pseudo("@2.n.reg.b0.v1" 0) Reg(A) false))"),
                                       &pins, &hints);
  CHECK(e.required_y == NameSet{"@1.y.3.b0.v1"});
  CHECK(e.hints_x == NameSet{"@2.n.b0.v2"});
}

TEST_CASE("hwreg: ssa_base strips the asm suffix and one TAC suffix") {
  using passes::hwreg_eligibility::ssa_base;
  CHECK(ssa_base("@1.acc.2.b0.v3") == "@1.acc");
  CHECK(ssa_base("@1.acc.b0.v3") == "@1.acc");
  CHECK(ssa_base("@1.acc.2") == "@1.acc");
  CHECK(ssa_base("@1.acc") == "@1.acc");
  CHECK(ssa_base("%t.1.2") == "%t.1");
  CHECK(ssa_base("x.b0") == "x.b0");
  CHECK(ssa_base("x.bv1") == "x.bv1");
  CHECK(ssa_base(".b1.v2") == "");
  CHECK(ssa_base("@3.n.reg.b1.v4") == "@3.n.reg");
}

// --- Liveness and interference ---

TEST_CASE("liveness: a Phi's source is live out of its predecessor") {
  const auto fn = function(R"(
    Label(".f@0") Mov(Imm(0) Pseudo("h0" 0) false) Label(".loop")
    Phi(Pseudo("h1" 0) [AsmPhiArg(".f@0" Pseudo("h0" 0)) AsmPhiArg(".loop" Pseudo("h2" 0))])
    Mov(Pseudo("h1" 0) Reg(A) false) Add(Imm(0) Reg(A)) Mov(Reg(A) Pseudo("h2" 0) false)
    Branch(NE ".loop") Return(false false false))");
  const auto live = passes::compute_liveness(fn);
  CHECK(tagged::encode<tagged::LivenessC>(passes::liveness_record(live)) ==
        R"({"object": "passes.optimization_asm.liveness.Liveness", "fields": {"blocks": [{"tuple": [0, 0, [], [2]]}, {"tuple": [1, 0, [4], []]}, {"tuple": [2, 2, [0], [3]]}, {"tuple": [3, 6, [2, 3], [3, 4]]}, {"tuple": [4, 1, [3], [1]]}], "block_order": [2, 3, 4], "live_in": {"dict": [[0, {"set": []}], [1, {"set": []}], [2, {"set": []}], [3, {"set": []}], [4, {"set": []}]]}, "live_out": {"dict": [[0, {"set": []}], [1, {"set": []}], [2, {"set": ["h0"]}], [3, {"set": ["h2"]}], [4, {"set": []}]]}}})");
  // Block 3 is the loop: its label, the Phi, the load of h1, the add,
  // the store of h2, and the branch. The Phi's destination is live from
  // the block's entry.
  CHECK(live.live_before(3, 0) == NameSet{"h1"});
  CHECK(live.live_after(3, 1) == NameSet{"h1"});
  CHECK(live.live_after(3, 2) == NameSet{});
  CHECK(live.live_before(3, 3) == NameSet{});
  CHECK(live.live_after(3, 4) == NameSet{"h2"});
}

TEST_CASE("interference: the SSA forms of the Python tests") {
  struct Case {
    const char* instrs;
    const char* graph;
    const char* coloring;
  };
  const Case cases[] = {
      // Two locals whose lifetimes are disjoint share a byte.
      {R"(Label(".main@asm_ssa_block@0") Mov(Imm(1) Pseudo("%a.b0.v1" 0) false)
          Mov(Pseudo("%a.b0.v1" 0) Reg(A) false) Mov(Imm(2) Pseudo("%b.b0.v1" 0) false)
          Mov(Pseudo("%b.b0.v1" 0) Reg(A) false) Return(true false false))",
       R"({"object": "passes.optimization.interference.InterferenceGraph", "fields": {"nodes": {"dict": [["%a.b0.v1", {"object": "passes.optimization.interference.InterferenceNode", "fields": {"name": "%a.b0.v1", "width": 1, "lives_across_call": false}}], ["%b.b0.v1", {"object": "passes.optimization.interference.InterferenceNode", "fields": {"name": "%b.b0.v1", "width": 1, "lives_across_call": false}}]]}, "adj": {"dict": []}}})",
       R"({"object": "passes.optimization.register_allocation.Coloring", "fields": {"assignments": {"dict": [["%b.b0.v1", 128], ["%a.b0.v1", 128]]}, "spilled": {"set": []}, "pool": {"object": "passes.optimization.pool.Pool", "fields": {"start": 128}}, "hwreg_assignments": {"dict": []}}})"},
      // Two that overlap get different bytes.
      {R"(Label(".main@asm_ssa_block@0") Mov(Imm(1) Pseudo("%a.b0.v1" 0) false)
          Mov(Imm(2) Pseudo("%b.b0.v1" 0) false)
          Compare(Pseudo("%a.b0.v1" 0) Pseudo("%b.b0.v1" 0)) Return(true false false))",
       R"({"object": "passes.optimization.interference.InterferenceGraph", "fields": {"nodes": {"dict": [["%a.b0.v1", {"object": "passes.optimization.interference.InterferenceNode", "fields": {"name": "%a.b0.v1", "width": 1, "lives_across_call": false}}], ["%b.b0.v1", {"object": "passes.optimization.interference.InterferenceNode", "fields": {"name": "%b.b0.v1", "width": 1, "lives_across_call": false}}]]}, "adj": {"dict": [["%a.b0.v1", {"set": ["%b.b0.v1"]}], ["%b.b0.v1", {"set": ["%a.b0.v1"]}]]}}})",
       R"({"object": "passes.optimization.register_allocation.Coloring", "fields": {"assignments": {"dict": [["%b.b0.v1", 128], ["%a.b0.v1", 129]]}, "spilled": {"set": []}, "pool": {"object": "passes.optimization.pool.Pool", "fields": {"start": 128}}, "hwreg_assignments": {"dict": []}}})"},
      // Two pointers live at once get blocks of 2 bytes that don't
      // overlap.
      {R"(Label(".main@asm_ssa_block@0") LoadAddress(Pseudo("%x" 0) Pseudo("%p" 0))
          LoadAddress(Pseudo("%y" 0) Pseudo("%q" 0)) Mov(Pseudo("%p" 0) Reg(A) false)
          Mov(Pseudo("%q" 0) Reg(A) false) Mov(Pseudo("%p" 1) Reg(A) false)
          Mov(Pseudo("%q" 1) Reg(A) false) Return(false false false))",
       R"({"object": "passes.optimization.interference.InterferenceGraph", "fields": {"nodes": {"dict": [["%p", {"object": "passes.optimization.interference.InterferenceNode", "fields": {"name": "%p", "width": 2, "lives_across_call": false}}], ["%q", {"object": "passes.optimization.interference.InterferenceNode", "fields": {"name": "%q", "width": 2, "lives_across_call": false}}]]}, "adj": {"dict": [["%p", {"set": ["%q"]}], ["%q", {"set": ["%p"]}]]}}})",
       R"({"object": "passes.optimization.register_allocation.Coloring", "fields": {"assignments": {"dict": [["%q", 128], ["%p", 130]]}, "spilled": {"set": []}, "pool": {"object": "passes.optimization.pool.Pool", "fields": {"start": 128}}, "hwreg_assignments": {"dict": []}}})"},
      // A byte live across a LoadAddress avoids both bytes of the
      // pointer.
      {R"(Label(".main@asm_ssa_block@0") Mov(Imm(42) Pseudo("%v.b0.v1" 0) false)
          LoadAddress(Pseudo("%x" 0) Pseudo("%p" 0)) Mov(Pseudo("%v.b0.v1" 0) Reg(A) false)
          Mov(Pseudo("%p" 0) Reg(A) false) Mov(Pseudo("%p" 1) Reg(A) false)
          Return(false false false))",
       R"({"object": "passes.optimization.interference.InterferenceGraph", "fields": {"nodes": {"dict": [["%p", {"object": "passes.optimization.interference.InterferenceNode", "fields": {"name": "%p", "width": 2, "lives_across_call": false}}], ["%v.b0.v1", {"object": "passes.optimization.interference.InterferenceNode", "fields": {"name": "%v.b0.v1", "width": 1, "lives_across_call": false}}]]}, "adj": {"dict": [["%p", {"set": ["%v.b0.v1"]}], ["%v.b0.v1", {"set": ["%p"]}]]}}})",
       R"({"object": "passes.optimization.register_allocation.Coloring", "fields": {"assignments": {"dict": [["%p", 128], ["%v.b0.v1", 130]]}, "spilled": {"set": []}, "pool": {"object": "passes.optimization.pool.Pool", "fields": {"start": 128}}, "hwreg_assignments": {"dict": []}}})"},
  };
  for (const auto& c : cases) {
    CAPTURE(c.instrs);
    const auto fn = function(c.instrs);
    const auto live = passes::compute_liveness(fn);
    const auto g = passes::build_interference(fn, live);
    CHECK(graph_json(g) == c.graph);
    CHECK(coloring_json(passes::color_graph(fn, g)) == c.coloring);
  }
}

TEST_CASE("interference: a BCD chain's destination byte interferes with later sources") {
  const auto fn = function(R"(
    Mov(Imm(1) Pseudo("x0" 0) false) Mov(Imm(2) Pseudo("x1" 0) false)
    Mov(Imm(3) Pseudo("y0" 0) false) Mov(Imm(4) Pseudo("y1" 0) false)
    DecimalArith(false [Pseudo("x0" 0) Pseudo("x1" 0)] [Pseudo("y0" 0) Pseudo("y1" 0)]
                 [Pseudo("d0" 0) Pseudo("d1" 0)])
    Mov(Pseudo("d0" 0) Reg(A) false) Mov(Pseudo("d1" 0) Reg(X) false)
    Return(true true false))");
  const auto live = passes::compute_liveness(fn);
  const auto g = passes::build_interference(fn, live);
  CHECK(graph_json(g) ==
        R"({"object": "passes.optimization.interference.InterferenceGraph", "fields": {"nodes": {"dict": [["d0", {"object": "passes.optimization.interference.InterferenceNode", "fields": {"name": "d0", "width": 1, "lives_across_call": false}}], ["d1", {"object": "passes.optimization.interference.InterferenceNode", "fields": {"name": "d1", "width": 1, "lives_across_call": false}}], ["x0", {"object": "passes.optimization.interference.InterferenceNode", "fields": {"name": "x0", "width": 1, "lives_across_call": false}}], ["x1", {"object": "passes.optimization.interference.InterferenceNode", "fields": {"name": "x1", "width": 1, "lives_across_call": false}}], ["y0", {"object": "passes.optimization.interference.InterferenceNode", "fields": {"name": "y0", "width": 1, "lives_across_call": false}}], ["y1", {"object": "passes.optimization.interference.InterferenceNode", "fields": {"name": "y1", "width": 1, "lives_across_call": false}}]]}, "adj": {"dict": [["d0", {"set": ["d1", "x1", "y1"]}], ["d1", {"set": ["d0"]}], ["x0", {"set": ["x1", "y0", "y1"]}], ["x1", {"set": ["d0", "x0", "y0", "y1"]}], ["y0", {"set": ["x0", "x1", "y1"]}], ["y1", {"set": ["d0", "x0", "x1", "y0"]}]]}}})");
  CHECK(coloring_json(passes::color_graph(fn, g)) ==
        R"({"object": "passes.optimization.register_allocation.Coloring", "fields": {"assignments": {"dict": [["d1", 128], ["d0", 129], ["y1", 128], ["y0", 129], ["x1", 130], ["x0", 131]]}, "spilled": {"set": []}, "pool": {"object": "passes.optimization.pool.Pool", "fields": {"start": 128}}, "hwreg_assignments": {"dict": []}}})");
}

// --- The coloring ---

// Colors `instrs` as the optimizer's driver does, through the
// coalescing, with the eligibility of the names before coalescing.
passes::Coloring color(std::string_view instrs,
                       const OrderedMap<std::string, std::string>* pins = nullptr,
                       const std::optional<passes::Range>& allowed = std::nullopt,
                       const std::set<asdl::Int>* blocked = nullptr) {
  const auto fn = function(instrs);
  const auto live = passes::compute_liveness(fn);
  auto g = passes::build_interference(fn, live);
  const auto co = passes::coalesce_moves(fn, g);
  const auto elig = passes::scan_function(fn, pins);
  const passes::HwRegPinning pinning{.eligibility = elig, .liveness = live};
  return passes::color_graph(fn, g, std::nullopt, blocked, &pinning, allowed,
                             &co.representative);
}

TEST_CASE("regalloc: find_fit and blocked_bytes") {
  using passes::register_allocation::blocked_bytes;
  using passes::register_allocation::find_fit;
  const passes::Range r{.start = 0x80, .stop = 0x84};
  CHECK(find_fit(r, 1, {}) == asdl::Int(0x80));
  CHECK(find_fit(r, 2, {0x80}) == asdl::Int(0x81));
  CHECK(find_fit(r, 2, {0x81, 0x83}) == std::nullopt);
  CHECK(find_fit(r, 4, {}) == asdl::Int(0x80));
  CHECK(find_fit(r, 5, {}) == std::nullopt);
  CHECK(find_fit(r, 0, {}) == std::nullopt);
  InterferenceGraph g;
  add_node(g, "a");
  add_node(g, "p", 2);
  add_node(g, "u");
  g.add_edge("a", "p");
  g.add_edge("a", "u");
  const OrderedMap<std::string, asdl::Int> assigned{{"p", 0x90}};
  CHECK(blocked_bytes("a", g, assigned) == std::set<asdl::Int>{0x90, 0x91});
}

TEST_CASE("regalloc: a value that fits nowhere is spilled") {
  const std::set<asdl::Int> blocked{0x81};
  const auto c = color(R"(Mov(Imm(1) Pseudo("a" 0) false) Mov(Imm(2) Pseudo("b" 0) false)
                          Mov(Imm(3) Pseudo("c" 0) false) Compare(Pseudo("a" 0) Pseudo("b" 0))
                          Compare(Pseudo("c" 0) Imm(0)) Return(false false false))",
                       nullptr, passes::Range{.start = 0x80, .stop = 0x83}, &blocked);
  CHECK(coloring_json(c) ==
        R"({"object": "passes.optimization.register_allocation.Coloring", "fields": {"assignments": {"dict": [["c", 128], ["b", 130]]}, "spilled": {"set": ["a"]}, "pool": {"object": "passes.optimization.pool.Pool", "fields": {"start": 128}}, "hwreg_assignments": {"dict": []}}})");
}

TEST_CASE("regalloc: a register local that can't be pinned is an error") {
  const OrderedMap<std::string, std::string> pins{{"P", "Y"}};
  try {
    color(R"(Mov(Imm(3) Pseudo("P" 0) false) Add(Pseudo("P" 0) Reg(A)) Return(true false false))",
          &pins);
    FAIL("no error");
  } catch (const CompileError& e) {
    CHECK(e.python_type() == "RegallocError");
    CHECK(std::string(e.what()) ==
          "can't pin `P` to Y \xE2\x80\x94 the `register` declaration conflicts with the IR's use "
          "of Y for this value (operand shape, call-clobber, or interference with another "
          "required-Y value)");
  }
}

TEST_CASE("regalloc: a register local is pinned to Y") {
  const OrderedMap<std::string, std::string> pins{{"P", "Y"}};
  const auto c = color(R"(Mov(Imm(0) Pseudo("P.b0.v1" 0) false) Inc(Pseudo("P.b0.v1" 0))
                          Compare(Pseudo("P.b0.v1" 0) Imm(5)) Return(false false false))",
                       &pins);
  CHECK(coloring_json(c) ==
        R"({"object": "passes.optimization.register_allocation.Coloring", "fields": {"assignments": {"dict": []}, "spilled": {"set": []}, "pool": {"object": "passes.optimization.pool.Pool", "fields": {"start": 128}}, "hwreg_assignments": {"dict": [["P.b0.v1", "Y"]]}}})");
}

constexpr const char* kCarryLoop = R"(
  Label(".f@0") Mov(Imm(0) Pseudo("h0" 0) false) Label(".loop")
  Phi(Pseudo("h1" 0) [AsmPhiArg(".f@0" Pseudo("h0" 0)) AsmPhiArg(".loop" Pseudo("h2" 0))])
  Mov(Pseudo("h1" 0) Reg(A) false) Add(Imm(0) Reg(A)) Mov(Reg(A) Pseudo("h2" 0) false)
  Branch(NE ".loop") Return(false false false))";

TEST_CASE("regalloc: both carry bytes go in X, or neither") {
  const auto fn = function(kCarryLoop);
  const auto live = passes::compute_liveness(fn);
  const auto g = passes::build_interference(fn, live);
  const auto elig = passes::scan_function(fn);
  const passes::HwRegPinning pinning{.eligibility = elig, .liveness = live};
  CHECK(coloring_json(passes::color_graph(fn, g, std::nullopt, nullptr, &pinning)) ==
        R"({"object": "passes.optimization.register_allocation.Coloring", "fields": {"assignments": {"dict": [["h0", 128]]}, "spilled": {"set": []}, "pool": {"object": "passes.optimization.pool.Pool", "fields": {"start": 128}}, "hwreg_assignments": {"dict": [["h1", "X"], ["h2", "X"]]}}})");
  // After coalescing, the pair's second name has left the graph, so the
  // first one isn't pinned either.
  CHECK(coloring_json(color(kCarryLoop)) ==
        R"({"object": "passes.optimization.register_allocation.Coloring", "fields": {"assignments": {"dict": [["h1", 128]]}, "spilled": {"set": []}, "pool": {"object": "passes.optimization.pool.Pool", "fields": {"start": 128}}, "hwreg_assignments": {"dict": []}}})");
}

TEST_CASE("regalloc: a value stays in Y across a call that preserves Y") {
  const char* body = R"(Mov(Imm(0) Pseudo("i" 0) false) Mov(Pseudo("i" 0) Reg(A) false)
    Mov(Reg(A) Reg(Y) false) Mov(IndexedData("t" 0 Y) Reg(A) false) Call("rom" [] %s)
    Mov(Pseudo("i" 0) Reg(A) false) Mov(Reg(A) Reg(Y) false)
    Mov(IndexedData("t" 0 Y) Reg(A) false) Return(true false false))";
  const auto with = [&](const char* preserves) {
    std::string text = body;
    text.replace(text.find("%s"), 2, preserves);
    return coloring_json(color(text));
  };
  CHECK(with(R"(["Y"])") ==
        R"({"object": "passes.optimization.register_allocation.Coloring", "fields": {"assignments": {"dict": []}, "spilled": {"set": []}, "pool": {"object": "passes.optimization.pool.Pool", "fields": {"start": 128}}, "hwreg_assignments": {"dict": [["i", "Y"]]}}})");
  // A call that clobbers Y sends the value to a callee-saved byte.
  CHECK(with("[]") ==
        R"({"object": "passes.optimization.register_allocation.Coloring", "fields": {"assignments": {"dict": [["i", 192]]}, "spilled": {"set": []}, "pool": {"object": "passes.optimization.pool.Pool", "fields": {"start": 128}}, "hwreg_assignments": {"dict": []}}})");
}

TEST_CASE("regalloc: a read of a frame slot loads Y") {
  // `p` is a soft-stack parameter, read with `LDY #off; LDA (FP),Y`, so
  // `i` can't stay in Y across the read (`fuzz.calls` seed 335), and
  // goes in X instead. Python's `color_graph` gives the same.
  const auto fn = function(R"(Mov(Imm(0) Pseudo("i" 0) false) Mov(Pseudo("i" 0) Reg(A) false)
    Mov(Reg(A) Reg(Y) false) Mov(IndexedData("t" 0 Y) Reg(A) false)
    Mov(Pseudo("p" 0) Reg(A) false) Mov(Reg(A) Pseudo("u" 0) false)
    Mov(Pseudo("i" 0) Reg(A) false) Mov(Reg(A) Reg(Y) false)
    Mov(IndexedData("t" 0 Y) Reg(A) false) Return(true false false))");
  const auto live = passes::compute_liveness(fn);
  const auto g = passes::build_interference(fn, live);
  const auto elig = passes::scan_function(fn);
  const passes::HwRegPinning pinning{.eligibility = elig, .liveness = live};
  const auto pinned = [&](const NameSet* frame) {
    const auto c = passes::color_graph(fn, g, std::nullopt, nullptr, &pinning, std::nullopt,
                                       nullptr, frame);
    const std::string* letter = c.hwreg_assignments.find("i");
    return letter == nullptr ? std::string() : *letter;
  };
  CHECK(pinned(nullptr) == "Y");
  const NameSet frame{"p"};
  CHECK(pinned(&frame) == "X");
}

TEST_CASE("regalloc: a spilled value's access loads Y") {
  // With two free bytes, one of `a`, `b`, and `c` is spilled to a frame
  // slot, whose compare loads Y while `i` is live, so `i` goes in X.
  // With room for all three, `i` stays in Y. Python's `color_graph`
  // gives the same (tests/test_asm_regalloc.py).
  const auto fn = function(R"(Mov(Imm(0) Pseudo("i" 0) false) Mov(Pseudo("i" 0) Reg(A) false)
    Mov(Reg(A) Reg(Y) false) Mov(IndexedData("t" 0 Y) Reg(A) false)
    Mov(Imm(1) Pseudo("a" 0) false) Mov(Imm(2) Pseudo("b" 0) false)
    Mov(Imm(3) Pseudo("c" 0) false) Compare(Pseudo("a" 0) Pseudo("b" 0))
    Compare(Pseudo("c" 0) Imm(0)) Mov(Pseudo("i" 0) Reg(A) false) Mov(Reg(A) Reg(Y) false)
    Mov(IndexedData("t" 0 Y) Reg(A) false) Return(true false false))");
  const auto live = passes::compute_liveness(fn);
  const auto g = passes::build_interference(fn, live);
  const auto elig = passes::scan_function(fn);
  const passes::HwRegPinning pinning{.eligibility = elig, .liveness = live};
  const auto color = [&](asdl::Int stop) {
    return passes::color_graph(fn, g, std::nullopt, nullptr, &pinning,
                               passes::Range{.start = 0x80, .stop = stop});
  };
  const auto roomy = color(0x83);
  CHECK(roomy.spilled.empty());
  CHECK(roomy.hwreg_assignments == OrderedMap<std::string, std::string>{{"i", "Y"}});
  const auto tight = color(0x82);
  CHECK(tight.spilled.size() == 1);
  CHECK(tight.hwreg_assignments == OrderedMap<std::string, std::string>{{"i", "X"}});
}

TEST_CASE("regalloc: a recorded call pins only with the liveness") {
  // The pinning reads the liveness for the writes of X and Y in a live
  // range, so a record that gives the eligibility alone is refused.
  testing::check_record(R"(c6502-capture 2
pass color_graph
source cpp/tests/test_regalloc.cpp
args {"args": [{"object": "passes.optimization.interference.InterferenceGraph", "fields": {"nodes": {"dict": []}, "adj": {"dict": []}}}], "kwargs": {"hwreg_eligibility": {"object": "passes.optimization_asm.hwreg_eligibility.HwRegEligibility", "fields": {"eligible_x": {"set": []}, "eligible_y": {"set": []}, "hints_x": {"set": []}, "hints_y": {"set": []}, "use_count": {"dict": []}, "required_x": {"set": []}, "required_y": {"set": []}, "hints_carry": {"set": []}, "carry_pairs": []}}, "liveness": null}}
context {"layout":[0,2,4,32,36],"user_zp_globals":[],"user_zp_bytes":[]}
input
Function("f" true [] [
  Return(true false false)
])
output-error
{"type": "Exception", "message": "the call gives hwreg_eligibility or liveness without the other"}
)");
}

}  // namespace
