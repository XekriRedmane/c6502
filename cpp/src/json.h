// A reader of the JSON that the capture harness writes for a pass's
// arguments (tools/capture_passes.py describes the encoding of each
// Python value).
#pragma once

#include <cstddef>
#include <string>
#include <string_view>
#include <vector>

#include "asdl/runtime.h"

namespace c6502::json {

// A JSON value. Numbers are integers, since the harness writes no
// floats. An object keeps its members in the order of the text, with
// `keys[i]` naming `items[i]`.
struct Value {
  enum class Kind { kNull, kBool, kInt, kString, kArray, kObject };

  Kind kind = Kind::kNull;
  bool boolean = false;
  asdl::Int integer = 0;
  std::string string;
  std::vector<Value> items;
  std::vector<std::string> keys;

  // Returns the member named `key` of an object, or null when it has
  // none.
  const Value* get(std::string_view key) const;
};

// Returns the value `text` holds. Raises `asdl::ParseError` for text
// that isn't JSON or that holds a fraction or an exponent.
Value parse(std::string_view text);

// Returns the text of the UTF-8 string `s` as Python's `json.dumps`
// writes a `str`, in double quotes, with `"`, `\`, and every character
// outside printable ASCII escaped.
std::string quote(std::string_view s);

// Returns the text Python's `json.dumps` writes for `v`, with its
// default separators (", " and ": ") and every string escaped as
// `quote` escapes it, so that a value read from the capture harness's
// JSON and written again gives the same text.
std::string dump(const Value& v);

}  // namespace c6502::json
