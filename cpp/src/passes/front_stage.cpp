#include "passes/front_stage.h"

#include <stdexcept>
#include <string>
#include <utility>

#include "c99_ast.h"
#include "passes/loop_labeling.h"
#include "pp_numbers.h"
#include "tagged_json.h"

namespace c6502::passes::front_stage {

namespace {

using namespace ::c6502::tagged;

using Sym = SymbolTableC;
using Types = TypeTableC;

template <class C>
constexpr RoundTrip rt = &round_trip<C>;

// Returns the string a record holds on its `input-json` line, the
// source text of `preprocess` and `parse` and the spelling that the
// functions of pp_numbers.py take.
std::string input_string(const capture::Call& call) {
  return Str::decode(json::parse(call.input));
}

const json::Value& arg(const json::Value& args, std::size_t i) {
  if (i >= args.items.size()) {
    throw std::runtime_error("the call has too few positional arguments");
  }
  return args.items[i];
}

StageOutput run_loop_labeling(const capture::Call& call, const AsmContext&, const json::Value&,
                              const json::Value&) {
  auto prog = asdl::load<c99_ast::Program>(call.input);
  return {asdl::dump(label_program(std::move(prog))), "", ""};
}

StageOutput run_pp_number_at(const capture::Call& call, const AsmContext&,
                             const json::Value& args, const json::Value&) {
  const std::string text = input_string(call);
  const auto pos = static_cast<std::size_t>(IntC::decode(arg(args, 0)));
  return {encode<Str>(pp_numbers::pp_number_at(text, pos)), "", ""};
}

StageOutput run_pp_is_floating(const capture::Call& call, const AsmContext&, const json::Value&,
                               const json::Value&) {
  return {encode<BoolC>(pp_numbers::is_floating(input_string(call))), "", ""};
}

StageOutput run_pp_parse_integer(const capture::Call& call, const AsmContext&,
                                 const json::Value&, const json::Value&) {
  return {encode<IntegerConstantC>(pp_numbers::parse_integer(input_string(call))), "", ""};
}

StageOutput run_pp_check_floating(const capture::Call& call, const AsmContext&,
                                  const json::Value&, const json::Value&) {
  return {encode<Pair<Str, Str>>(pp_numbers::check_floating(input_string(call))), "", ""};
}

StageOutput run_pp_bit_precise_width(const capture::Call& call, const AsmContext&,
                                     const json::Value& args, const json::Value&) {
  const auto constant = IntegerConstantC::decode(json::parse(call.input));
  const std::string spelling = Str::decode(arg(args, 0));
  return {encode<IntC>(pp_numbers::bit_precise_width(constant, spelling)), "", ""};
}

StageOutput run_pp_check(const capture::Call& call, const AsmContext&, const json::Value&,
                         const json::Value&) {
  pp_numbers::check(input_string(call));
  return {encode<NoneC>({}), "", ""};
}

}  // namespace

std::vector<StageSignature> signatures() {
  return {
      // `preprocess(source, pcpp_args, filename)`, which reads the
      // files the source includes from their paths.
      {.name = "preprocess",
       .input_json = rt<Str>,
       .args = {rt<ListOf<Str>>, rt<Optional<Str>>},
       .output = rt<Str>},
      {.name = "parse", .input_json = rt<Str>, .output_module = "c99_ast"},
      {.name = "unroll_program", .input = "c99_ast", .output_module = "c99_ast"},
      {.name = "identifier_resolution", .input = "c99_ast", .output_module = "c99_ast"},
      {.name = "string_lifting", .input = "c99_ast", .output_module = "c99_ast"},
      {.name = "label_resolution", .input = "c99_ast", .output_module = "c99_ast"},
      {.name = "loop_labeling",
       .input = "c99_ast",
       .output_module = "c99_ast",
       .run = &run_loop_labeling},
      // The type checker stamps the program it is given and returns it
      // with the two tables, so its record's `mutated` line holds the
      // program after the call under `input`.
      {.name = "type_checking",
       .input = "c99_ast",
       .output = rt<Triple<C99ProgramC, Sym, Types>>},
      {.name = "pp_number_at",
       .input_json = rt<Str>,
       .args = {rt<IntC>},
       .output = rt<Str>,
       .run = &run_pp_number_at},
      {.name = "pp_is_floating",
       .input_json = rt<Str>,
       .output = rt<BoolC>,
       .run = &run_pp_is_floating},
      {.name = "pp_parse_integer",
       .input_json = rt<Str>,
       .output = rt<IntegerConstantC>,
       .run = &run_pp_parse_integer},
      {.name = "pp_check_floating",
       .input_json = rt<Str>,
       .output = rt<Pair<Str, Str>>,
       .run = &run_pp_check_floating},
      {.name = "pp_bit_precise_width",
       .input_json = rt<IntegerConstantC>,
       .args = {rt<Str>},
       .output = rt<IntC>,
       .run = &run_pp_bit_precise_width},
      {.name = "pp_check", .input_json = rt<Str>, .output = rt<NoneC>, .run = &run_pp_check},
  };
}

}  // namespace c6502::passes::front_stage
