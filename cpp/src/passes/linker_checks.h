// The checks of the frozen records (cpp/tests/frozen) of the linker's
// functions on the link metadata (passes/linker_metadata.h) and of
// `slot_sharing.merge_shares` and `check_linked_shares`, which
// `c6502cpp check-binutils` and `check-frozen` run.
#pragma once

#include <functional>
#include <string>
#include <string_view>

#include "json.h"

namespace c6502::passes::linker_checks {

// Returns the bytes of the capture's file named by its SHA-1 `hash`.
using Blob = std::function<std::string(const std::string& hash)>;

// Returns whether records of `kind` are checked here.
bool handles(std::string_view kind);

// Runs the C++ function of a record of `kind` on its arguments and
// input, and returns what differs from the recorded `output` (an error
// `{"type", "message"}` when `is_error`), or the empty string. The
// objects a record names are read with `blob`, and an object the
// function changes is compared with the recorded one byte for byte.
std::string check(std::string_view kind, const json::Value& args, const json::Value& input,
                  const json::Value& output, bool is_error, const Blob& blob);

}  // namespace c6502::passes::linker_checks
