// The check of a recorded call of lexer.py's `tokenize`, which
// tools/record_test_calls.py records as a `tokens` record while a Python
// unit test runs (docs/cpp_port.md, step 8, group G8): the source, and
// what `compile.py --lex` prints for its tokens or the error the call
// raised.
#pragma once

#include <exception>
#include <string>

#include "doctest.h"
#include "errors.h"
#include "json.h"
#include "lexer.h"

namespace c6502::testing {

// Checks that the C++ lexer gives `source` the tokens Python gave, as
// the lines `format_tokens` writes (`<line>:<col>`, tab, kind, tab,
// value), or, when `is_error` is set, that it raises the error whose
// `{"type": ..., "message": ...}` JSON `expected` holds, with the same
// Python class and message.
inline void check_tokens(const std::string& source, const std::string& expected,
                         bool is_error) {
  INFO("source: ", source);
  std::string got;
  std::string type;
  std::string message;
  bool raised = false;
  try {
    got = lexer::format_tokens(source);
  } catch (const CompileError& e) {
    raised = true;
    type = e.python_type();
    message = e.what();
  } catch (const std::exception& e) {
    raised = true;
    type = "AssertionError";
    message = e.what();
  }
  if (!is_error) {
    CHECK_MESSAGE(!raised, type, ": ", message);
    CHECK(got == expected);
    return;
  }
  const json::Value want = json::parse(expected);
  REQUIRE(raised);
  CHECK(type == want.get("type")->string);
  CHECK(message == want.get("message")->string);
}

}  // namespace c6502::testing
