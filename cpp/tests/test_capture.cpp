// Tests of the capture file reader and the JSON reader.
#include <string>

#include "capture.h"
#include "doctest.h"
#include "json.h"

namespace json = c6502::json;

TEST_CASE("json::parse reads the harness's JSON") {
  const json::Value v = json::parse(
      R"({"args": [1, -2, 18446744073709551616], "kwargs": {"s": "aé\n",)"
      R"( "t": {"tuple": [true, false, null]}}})");
  REQUIRE(v.kind == json::Value::Kind::kObject);
  const json::Value* args = v.get("args");
  REQUIRE(args != nullptr);
  REQUIRE(args->items.size() == 3);
  CHECK(args->items[1].integer == -2);
  CHECK(args->items[2].integer == (c6502::asdl::Int(1) << 64));
  const json::Value* kwargs = v.get("kwargs");
  CHECK(kwargs->get("s")->string == "a\xc3\xa9\n");
  CHECK(kwargs->get("t")->get("tuple")->items[2].kind == json::Value::Kind::kNull);
  CHECK(v.get("missing") == nullptr);
  CHECK_THROWS(json::parse("[1.5]"));
  CHECK_THROWS(json::parse("[1] 2"));
}

TEST_CASE("capture::parse splits a capture file into its sections") {
  const std::string text =
      "c6502-capture 1\n"
      "pass split_mem_to_mem\n"
      "source examples/x.c -S -O\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "])\n"
      "output\n"
      "Program([])\n";
  const auto call = c6502::capture::parse(text);
  CHECK(call.pass == "split_mem_to_mem");
  CHECK(call.source == "examples/x.c -S -O");
  CHECK(call.args_json == "{\"args\": [], \"kwargs\": {}}");
  CHECK(call.input == "Program([\n])\n");
  CHECK(call.output == "Program([])\n");
  CHECK_FALSE(call.output_is_json);

  const auto json_call = c6502::capture::parse(
      "c6502-capture 1\npass p\nsource s\nargs {}\ninput\nProgram([])\n"
      "output-json\n{\"tuple\": []}\n");
  CHECK(json_call.output_is_json);
  CHECK(json_call.input == "Program([])\n");
  CHECK(json_call.output == "{\"tuple\": []}\n");
}
