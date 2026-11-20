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

TEST_CASE("capture::parse reads a version 2 capture's context") {
  const auto call = c6502::capture::parse(
      "c6502-capture 2\npass p\nsource s\nargs {}\n"
      "context {\"layout\":[0,2,64,8,72],\"user_zp_globals\":[[\"G\",133]],"
      "\"user_zp_bytes\":[133,134]}\n"
      "input\nProgram([])\noutput\nProgram([])\n");
  CHECK(call.input == "Program([])\n");
  const auto ctx = c6502::capture::context(call);
  CHECK(ctx.layout.hargs == 64);
  CHECK(ctx.layout.hargs_size == 8);
  CHECK(ctx.layout.dptr == 72);
  CHECK(ctx.is_user_zp_global("G"));
  CHECK_FALSE(ctx.is_user_zp_global("H"));
  CHECK(*ctx.user_zp_address("G") == 133);
  CHECK(ctx.is_user_zp_byte(134));
  CHECK_FALSE(ctx.is_user_zp_byte(135));

  // A version 1 capture has no context, which reads as the default.
  const auto old = c6502::capture::context(c6502::capture::parse(
      "c6502-capture 1\npass p\nsource s\nargs {}\ninput\nProgram([])\n"
      "output\nProgram([])\n"));
  CHECK(old.layout == c6502::passes::RuntimeLayout{});
  CHECK_FALSE(old.is_user_zp_global("G"));
}
