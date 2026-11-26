// The checks of the frozen records of `parse_script`, `lay_out`,
// `default_script`, `fall_through`, `tail_jump`, and `startup_lines`,
// which `c6502cpp check-binutils` runs, and the tagged JSON of a
// `Script` that those records hold.
#pragma once

#include <string>
#include <string_view>

#include "json.h"
#include "passes/link_script.h"

namespace c6502::passes::link_checks {

// Returns the tagged JSON of `script`, in the form the records hold.
json::Value script_json(const link_script::Script& script);

// Returns the `Script` whose tagged JSON is `v`. Throws
// `InternalError` for JSON of another shape.
link_script::Script script_from_json(const json::Value& v);

// Returns whether records of `kind` are checked here.
bool handles(std::string_view kind);

// Runs the C++ function of a record of `kind` on its arguments and
// input, and returns what differs from the recorded `output` (an error
// `{"type", "message"}` when `is_error`), or the empty string.
std::string check(std::string_view kind, const json::Value& args, const json::Value& input,
                  const json::Value& output, bool is_error);

}  // namespace c6502::passes::link_checks
