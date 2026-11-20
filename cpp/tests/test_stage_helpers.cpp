// Tests of the helpers the code generation passes share: `sizeof` and
// `size_of_name`, the slot-name spellings of zp_slot_naming, the TAC
// operand walks of var_visit, and the call-graph reach of slot_sharing.
// `check-helpers` compares each on the corpus's `tables` and `pseudos`
// records (tools/capture_helpers.py --stage-only).
#include <string>
#include <vector>

#include "doctest.h"
#include "passes/asm_context.h"
#include "passes/optimization/pool.h"
#include "passes/optimization/var_visit.h"
#include "passes/replace_pseudoregisters.h"
#include "passes/slot_sharing.h"
#include "passes/zp_slot_naming.h"

namespace c99 = c6502::c99_ast;
namespace tac = c6502::tac_ast;
using namespace c6502::passes;

TEST_CASE("sizeof_type follows the widths of the C types") {
  TypeTable types;
  types.insert_or_assign("s", StructLayout{.tag = "s", .size = 5, .complete = true});
  types.insert_or_assign("t", StructLayout{.tag = "t", .size = 9, .complete = false});
  CHECK(sizeof_type(c99::Const{.referenced_type = c99::Pointer{.referenced_type = c99::Int{}}},
                    &types) == 2);
  CHECK(sizeof_type(c99::Array{.element_type = c99::Long{}, .size = 3}, &types) == 12);
  CHECK(sizeof_type(c99::BitInt{.width = 17, .is_signed = true}, &types) == 3);
  CHECK(sizeof_type(c99::Structure{.tag = "s"}, &types) == 5);
  CHECK(sizeof_type(c99::Structure{.tag = "s"}, nullptr) == 1);
  CHECK(sizeof_type(c99::Structure{.tag = "t"}, &types) == 1);
  SymbolTable symbols;
  symbols.insert_or_assign("x", Symbol{.type = c99::Double{}, .attrs = LocalAttr{}});
  CHECK(size_of_name("x", &symbols, &types) == 8);
  CHECK(size_of_name("y", &symbols, &types) == 1);
  CHECK(size_of_name("x", nullptr, &types) == 1);
}

TEST_CASE("parse_pseudo_name strips the SSA suffixes in order") {
  CHECK(parse_pseudo_name("@5.sprite_x.0.b1.v3") == std::pair{std::optional<std::string>("sprite_x"),
                                                               c6502::asdl::Int(1)});
  CHECK(parse_pseudo_name("@2.n.reg.b0.v1").first == "n");
  CHECK(parse_pseudo_name("%7.b2.v1") == std::pair{std::optional<std::string>(),
                                                     c6502::asdl::Int(2)});
  // `.vx` isn't a version, so it stays part of the name.
  CHECK(parse_pseudo_name("@1.a.vx").first == "a.vx");
  CHECK(!parse_pseudo_name("@3.").first);
  CHECK(source_spelling("@12.count") == "count");
  CHECK(source_spelling("table") == "table");
  CHECK(register_param_local("@1.p") == "@1.p.reg");
}

TEST_CASE("param_slot_symbols spells each byte of each parameter") {
  CHECK(param_slot_symbols("put", {std::string("@1.c"), std::nullopt, std::string("@2.w")},
                           {1, 1, 2}) ==
        std::vector<std::string>{"__zpabi_put__c", "__zpabi_put__1", "__zpabi_put__w_0",
                                 "__zpabi_put__w_1"});
}

TEST_CASE("the var_visit walks read and write what Python's do") {
  const tac::Type_instruction load = tac::IndexedLoad{
      .name = "arr", .index = tac::Var{.name = "i"}, .dst = tac::Var{.name = "t"}};
  CHECK(vals_in(load).size() == 2);
  CHECK(uses_in(load) == std::vector<std::string_view>{"arr", "i"});
  CHECK(defs_in(load) == std::vector<std::string_view>{"t"});
  // An `IndirectIndexedLoad`'s destination isn't among the definitions
  // Python's `defs_in` gives.
  const tac::Type_instruction indirect = tac::IndirectIndexedLoad{
      .ptr = tac::Var{.name = "p"}, .index = tac::Var{.name = "i"}, .dst = tac::Var{.name = "t"}};
  CHECK(defs_in(indirect).empty());
  const tac::Type_instruction addr =
      tac::GetAddress{.operand = tac::Var{.name = "x"}, .dst = tac::Var{.name = "p"}};
  CHECK(uses_in(addr).empty());
  const auto counts = count_uses({load, load, addr});
  CHECK(*counts.find("arr") == 2);
  CHECK(counts.begin()->first == "arr");
}

TEST_CASE("reach_sets and coexist follow the call graph") {
  const auto call = [](const char* name) {
    return tac::Type_instruction{tac::FunctionCall{.name = name}};
  };
  tac::Program prog{.top_level = {
                        tac::Function{.name = "main", .instructions = {call("a"), call("b")}},
                        tac::Function{.name = "a", .instructions = {call("c")}},
                        tac::Function{.name = "b"},
                    }};
  const ReachSets reach = reach_sets(prog);
  CHECK(reach.find("main")->size() == 3);
  CHECK(coexist("c", "main", reach));
  CHECK(!coexist("a", "b", reach));
  const std::vector<SlotShare> shares = {
      SlotShare{.members = {{"b", {"__zpabi_b__x"}}}},
      SlotShare{.members = {{"c", {"__zpabi_c__x"}}}, .holders = {"a"}},
  };
  CHECK(share_conflicts("a", shares, reach) == std::vector<std::size_t>{1});
  CHECK(shares[1].functions() == NameSet{"a", "c"});
}

TEST_CASE("Pool and RuntimeLayout give Python's ranges and bytes") {
  const Pool pool{.start = 0x90};
  CHECK(pool.mid() == 0xC8);
  CHECK(pool.callee_saved() == Range{.start = 0xC8, .stop = 0x100});
  CHECK(pool.caller_saved().contains(0x90));
  CHECK(!pool.caller_saved().contains(0xC8));
  CHECK_THROWS_AS(Pool{.start = 0x81}.validate(), std::invalid_argument);
  CHECK(RuntimeLayout{}.bytes().size() == 38);
}
