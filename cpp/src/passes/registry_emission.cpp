#include "passes/registry_emission.h"

#include <stdexcept>
#include <utility>

#include "asdl/runtime.h"
#include "asm2_ast.h"
#include "asm_emit.h"
#include "passes/asm_to_asm2.h"
#include "passes/capture_args.h"
#include "passes/emission_args.h"
#include "passes/prune_unused_slots.h"
#include "passes/registry_control_flow.h"
#include "passes/sections.h"
#include "passes/slot_sharing.h"
#include "passes/zp_globals.h"
#include "passes/zp_link_metadata.h"

namespace c6502::passes::registry {

namespace {

namespace ea = emission_args;
using json::Value;

const Value& positional(const Value& args, std::size_t i) {
  if (i >= args.items.size()) {
    throw std::runtime_error("the call has too few positional arguments");
  }
  return args.items[i];
}

// Returns the footprints that the captures record beside the arguments
// of the functions that measure HARGS.
HelperFootprints recorded_footprints(const Value& kwargs) {
  const Value* v = capture_args::kwarg(kwargs, "footprints");
  if (v == nullptr) {
    throw std::runtime_error("the capture has no footprints");
  }
  return ea::footprints(*v);
}

std::string run_link_records(asm_ast::Program input, const AsmContext&, const Value& args,
                             const Value&) {
  return ea::zp_objects_json(link_records(input, ea::zp_globals(positional(args, 0)))) + "\n";
}

std::string run_place_zp_globals(asm_ast::Program input, const AsmContext&, const Value& args,
                                 const Value& kwargs) {
  return asdl::dump(place_zp_globals(std::move(input), ea::zp_globals(positional(args, 0)),
                                     recorded_footprints(kwargs)));
}

std::string run_check_shares(asm_ast::Program input, const AsmContext&, const Value& args,
                             const Value&) {
  check_shares(input, capture_args::slot_shares(positional(args, 0)));
  return "null\n";
}

std::string run_prune_unused_locals(asm_ast::Program input, const AsmContext&,
                                    const Value& args, const Value&) {
  return ea::ordered_addrs_json(prune_unused_locals(input, ea::ordered_addrs(positional(args, 0)))) +
         "\n";
}

std::string run_lower_to_asm2(asm_ast::Program input, const AsmContext&, const Value&,
                              const Value&) {
  return asdl::dump(lower_to_asm2(input));
}

std::string run_hargs_need(asm_ast::Program input, const AsmContext&, const Value&,
                           const Value& kwargs) {
  return asdl::int_to_string(hargs_need(input, recorded_footprints(kwargs))) + "\n";
}

std::string run_section_items(asm_ast::Program input, const AsmContext&, const Value& args,
                              const Value&) {
  return ea::items_json(section_items(input, ea::section_infos(positional(args, 0)))) + "\n";
}

std::string run_format_metadata(const std::string& input, const AsmContext&, const Value&,
                                const Value&) {
  return ea::strings_json(format_metadata(ea::link_metadata(json::parse(input)))) + "\n";
}

std::string run_emit_program(const std::string& input, const AsmContext& ctx, const Value&,
                             const Value& kwargs) {
  asm_emit::EmitOptions options;
  const Value* slots = capture_args::kwarg(kwargs, "zp_slot_symbols");
  if (slots != nullptr && slots->kind != Value::Kind::kNull) {
    options.zp_slot_symbols = ea::ordered_addrs(*slots);
  }
  const Value* lines = capture_args::kwarg(kwargs, "link_metadata_lines");
  if (lines != nullptr && lines->kind != Value::Kind::kNull) {
    for (const auto& line : capture_args::items_of(*lines)) {
      options.link_metadata_lines.push_back(line.string);
    }
  }
  const Value* preferred = capture_args::kwarg(kwargs, "preferred_symbols");
  if (preferred != nullptr && preferred->kind != Value::Kind::kNull) {
    options.preferred_symbols = ea::preferred_symbols(*preferred);
  }
  const std::string text =
      asm_emit::emit_program(asdl::load<asm2_ast::Program>(input), ctx.layout, options);
  return json_quote(text) + "\n";
}

}  // namespace

const std::vector<RegisteredPass>& emission_passes() {
  static const std::vector<RegisteredPass> passes = {
      {.name = "link_records", .run = run_link_records, .output_is_json = true},
      {.name = "place_zp_globals", .run = run_place_zp_globals},
      {.name = "check_shares", .run = run_check_shares, .output_is_json = true},
      {.name = "prune_unused_locals", .run = run_prune_unused_locals, .output_is_json = true},
      {.name = "lower_to_asm2", .run = run_lower_to_asm2, .output_module = "asm2_ast"},
      {.name = "hargs_need", .run = run_hargs_need, .output_is_json = true},
      {.name = "section_items", .run = run_section_items, .output_is_json = true},
      {.name = "format_metadata",
       .run = nullptr,
       .output_is_json = true,
       .run_text = run_format_metadata,
       .input_module = "json"},
      {.name = "emit_program",
       .run = nullptr,
       .output_is_json = true,
       .run_text = run_emit_program,
       .input_module = "asm2_ast"},
  };
  return passes;
}

}  // namespace c6502::passes::registry
