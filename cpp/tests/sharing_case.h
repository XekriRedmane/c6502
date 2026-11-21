// Checks of the calls of group B's functions that the Python unit tests
// make (tests/test_sharing_stage_calls.cpp), each with what the Python
// function returned or the exception it raised.
#pragma once

#include <exception>
#include <string>
#include <string_view>

#include "capture.h"
#include "doctest.h"
#include "errors.h"
#include "json.h"
#include "passes/runtime_layout.h"
#include "passes/stage_registry.h"
#include "passes/zp_globals.h"
#include "stage_case.h"
#include "tagged_json.h"

namespace c6502::testing {

// Returns the text a check records for an exception: `exception:
// <Python type>: <message>`.
inline std::string exception_text(const std::exception_ptr& e) {
  try {
    std::rethrow_exception(e);
  } catch (const CompileError& x) {
    return "exception: " + x.python_type() + ": " + x.what();
  } catch (const InternalError& x) {
    return std::string("exception: AssertionError: ") + x.what();
  } catch (const std::exception& x) {
    return std::string("exception: ") + x.what();
  }
}

// Runs the stage call `name`'s runner as `check_stage_call` does, and
// gives the call its recorded result too, since `plan_zero_page`'s
// runner takes the HARGS size from it when the runtime's block moves
// (passes/zp_globals.h).
inline void check_sharing_call(std::string_view name, std::string_view args_json,
                               std::string_view context_json, bool input_is_json,
                               std::string_view input, std::string_view output) {
  const passes::StageSignature* sig = passes::find_stage(name);
  REQUIRE(sig != nullptr);
  REQUIRE(sig->run != nullptr);
  capture::Call call;
  call.pass = std::string(name);
  call.args_json = std::string(args_json);
  call.context_json = std::string(context_json);
  call.input = std::string(input);
  call.input_is_json = input_is_json;
  call.output_is_json = output.rfind("exception: ", 0) != 0;
  if (call.output_is_json) {
    call.output = std::string(output);
  }
  const json::Value args = json::parse(args_json);
  std::string got;
  try {
    const passes::StageOutput out =
        sig->run(call, capture::context(call), *args.get("args"), *args.get("kwargs"));
    got = out.output + "\n";
    CHECK(out.mutated.empty());
  } catch (...) {
    got = exception_text(std::current_exception()) + "\n";
  }
  CHECK(got == output);
}

// Returns a `HargsNeed` that gives `need`, or, for -1, one that fails
// the test, since the Python call didn't ask.
inline passes::HargsNeed need_of(long long need) {
  return [need]() -> asdl::Int {
    if (need < 0) {
      FAIL("the call asked for the HARGS need, which Python's didn't");
    }
    return need;
  };
}

// Runs `choose_layout` on `reserved` (a JSON list of bytes) with a need
// of `need` HARGS bytes (-1 when Python's call didn't ask), and checks
// that it gives the layout `output` records as `[ssp, fp, hargs,
// hargs_size, dptr]`, or throws the exception it records.
inline void check_choose_layout(std::string_view reserved_json, long long need,
                                std::string_view output) {
  std::set<asdl::Int> reserved;
  for (const auto& b : json::parse(reserved_json).items) {
    reserved.insert(b.integer);
  }
  std::string got;
  try {
    const passes::RuntimeLayout l = passes::choose_layout(reserved, need_of(need));
    got = "[" + asdl::int_to_string(l.ssp) + ", " + asdl::int_to_string(l.fp) + ", " +
          asdl::int_to_string(l.hargs) + ", " + asdl::int_to_string(l.hargs_size) + ", " +
          asdl::int_to_string(l.dptr) + "]";
  } catch (...) {
    got = exception_text(std::current_exception());
  }
  CHECK(got == output);
}

// Runs `allocate_zp_globals` on the tagged JSON of its arguments, with a
// `hargs_need` when `given` that gives `need` (-1 when Python's call
// didn't ask), and checks that it returns the `ZpGlobals` `output`
// records, or throws the exception it records.
inline void check_allocate_zp_globals(std::string_view symbols_json, std::string_view types_json,
                                      std::string_view weights_json, bool given, long long need,
                                      std::string_view output) {
  using namespace ::c6502::tagged;
  const passes::SymbolTable symbols = SymbolTableC::decode(json::parse(symbols_json));
  const auto types = Optional<TypeTableC>::decode(json::parse(types_json));
  const auto weights = Optional<DictOf<Str, IntC>>::decode(json::parse(weights_json));
  std::string got;
  try {
    const passes::ZpGlobals zp = passes::allocate_zp_globals(
        symbols, types ? &*types : nullptr, given ? need_of(need) : passes::HargsNeed{},
        weights ? &*weights : nullptr);
    got = encode<ZpGlobalsC>(zp) + "\n";
  } catch (...) {
    got = exception_text(std::current_exception()) + "\n";
  }
  CHECK(got == output);
}

}  // namespace c6502::testing
