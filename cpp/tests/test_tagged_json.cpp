// Tests of the tagged JSON codecs of the compile's tables
// (src/tagged_json.h) and of OrderedMap. The corpus check is
// `check-captures` on a capture of the stage calls, which reads and
// writes back every value Python records; these cover the forms the
// corpus has few of.
#include <string>

#include "doctest.h"
#include "json.h"
#include "ordered_map.h"
#include "tagged_json.h"

namespace tagged = c6502::tagged;
using c6502::json::parse;

namespace {

template <class C>
std::string again(const std::string& text) {
  return tagged::round_trip<C>(parse(text));
}

}  // namespace

TEST_CASE("OrderedMap keeps the order of insertion, as a dict does") {
  c6502::OrderedMap<std::string, int> m;
  m.insert_or_assign("b", 1);
  m.insert_or_assign("a", 2);
  m.insert_or_assign("b", 3);
  m["c"] = 4;
  std::string keys;
  for (const auto& [k, v] : m) {
    keys += k + std::to_string(v);
  }
  CHECK(keys == "b3a2c4");
  CHECK(m.erase("a"));
  CHECK(!m.erase("a"));
  CHECK(*m.find("c") == 4);
  CHECK(m.size() == 2);
}

TEST_CASE("a set is written sorted by the JSON of its elements") {
  // Python sorts the encodings as text, so 10 comes before 9.
  CHECK(again<tagged::SetOf<tagged::IntC>>(R"({"set": [10, 9, 128]})") ==
        R"({"set": [10, 128, 9]})");
  CHECK(again<tagged::NameSetC>(R"({"set": ["b", "a"]})") == R"({"set": ["a", "b"]})");
}

TEST_CASE("the containers and Python's values make the trip unchanged") {
  const std::string pools = R"({"dict": [["f", [128, 129]], ["e", []]]})";
  CHECK(again<tagged::DictOf<tagged::Str, tagged::ListOf<tagged::IntC>>>(pools) == pools);
  const std::string range = R"({"range": [0, 128, 1]})";
  CHECK(again<tagged::Optional<tagged::RangeC>>(range) == range);
  CHECK(again<tagged::Optional<tagged::RangeC>>("null") == "null");
  const std::string pair = R"({"tuple": [{"tuple": []}, {"dict": [["x", 2]]}]})";
  CHECK(again<tagged::Pair<tagged::TupleOf<tagged::Str>, tagged::DictOf<tagged::Str, tagged::IntC>>>(
            pair) == pair);
  // A string outside printable ASCII is escaped as `json.dumps` escapes
  // it. MSVC reads a `\u` in a raw string literal as the character, so
  // the backslashes are escaped.
  const std::string escaped = "\"a\\u00e9\\n\"";
  CHECK(again<tagged::Str>(escaped) == escaped);
}

TEST_CASE("a symbol table with each kind of attribute makes the trip unchanged") {
  const std::string text =
      R"({"object": "passes.type_checking.SymbolTable", "fields": {"_table": {"dict": [)"
      R"(["g", {"object": "passes.type_checking.Symbol", "fields": {)"
      R"("type": {"asdl": "c99_ast", "dump": "Array(Double 2)\n"}, )"
      R"("attrs": {"object": "passes.type_checking.StaticAttr", "fields": {)"
      R"("initial_value": {"object": "passes.type_checking.Initial", "fields": {"value": )"
      R"({"tuple": [{"float": 4607182418800017408}, {"float": 9223372036854775808}]}}}, )"
      R"("is_global": true, "zeropage": false, "zp_address": null, "address": null, )"
      R"("section": "data2", "planes": {"tuple": ["g@lo", "g@hi"]}, "mergeable": false, )"
      R"("was_bool": false}}}}], )"
      R"(["u", {"object": "passes.type_checking.Symbol", "fields": {)"
      R"("type": {"asdl": "c99_ast", "dump": "Union(\"u\")\n"}, )"
      R"("attrs": {"object": "passes.type_checking.StaticAttr", "fields": {)"
      R"("initial_value": {"object": "passes.type_checking.Initial", "fields": {"value": )"
      R"({"object": "passes.type_checking.UnionValue", "fields": {"member": 1, "value": )"
      R"({"object": "passes.type_checking.AddressInit", "fields": {"name": "g", "offset": 2}}}}}}, )"
      R"("is_global": false, "zeropage": true, "zp_address": 128, "address": null, )"
      R"("section": null, "planes": null, "mergeable": true, "was_bool": false}}}}], )"
      R"(["f", {"object": "passes.type_checking.Symbol", "fields": {)"
      R"("type": {"asdl": "c99_ast", "dump": "FunType([] Bool)\n"}, )"
      R"("attrs": {"object": "passes.type_checking.FunAttr", "fields": {)"
      R"("defined": false, "is_global": true, "carry_return": true, "address": 64738, )"
      R"("preserves": {"set": ["X", "Y"]}, "section": null, "param_zeropage": {"tuple": [null, 2]}, )"
      R"("returns_bool": true, "noreturn": false}}}}], )"
      R"(["@1.x", {"object": "passes.type_checking.Symbol", "fields": {)"
      R"("type": {"asdl": "c99_ast", "dump": "Int\n"}, )"
      R"("attrs": {"object": "passes.type_checking.LocalAttr", "fields": {)"
      R"("register": true, "was_bool": false}}}}])"
      R"(]}}})";
  CHECK(again<tagged::SymbolTableC>(text) == text);
  const auto table = tagged::SymbolTableC::decode(parse(text));
  const auto& g = std::get<c6502::passes::StaticAttr>(table.find("g")->attrs);
  const auto& init = std::get<c6502::passes::Initial>(g.initial_value);
  CHECK(init.value.items.at(0).as_double() == 1.0);
}

TEST_CASE("a class or a field the C++ form doesn't hold is an error") {
  CHECK_THROWS_AS(tagged::PoolC::decode(parse(
                      R"({"object": "passes.optimization.pool.Pool", "fields": {"begin": 1}})")),
                  tagged::DecodeError);
  CHECK_THROWS_AS(tagged::ParamLayoutC::decode(
                      parse(R"({"object": "passes.abi_selection.Other", "fields": {}})")),
                  tagged::DecodeError);
}
