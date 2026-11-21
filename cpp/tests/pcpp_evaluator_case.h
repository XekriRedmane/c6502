// A recorded call of pcpp's `#if` evaluator, for
// tests/test_pcpp_evaluator.cpp, with its tokens, its hooks, and what
// Python returned or raised.
#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <variant>

#include "doctest.h"
#include "pcpp_evaluator.h"
#include "pcpp_token.h"

namespace c6502::testing {

struct EvaluatorCase {
  // With "zero" an unknown name is 0 and an unknown function returns 0,
  // as in CompilePreprocessor. With "none" both hooks give back the
  // name, as with --passthru-unknown-exprs. With "dict" both raise
  // SyntaxError, as evaluator.py's empty dicts do.
  const char* mode;
  // Each token is its type, \001, and its value, and the tokens are
  // separated by \002.
  const char* tokens;
  bool ok;
  // The result's str when `ok`, and otherwise the exception's class,
  // str, and repr.
  const char* a;
  const char* b;
  const char* c;
};

inline pcpp::Tokens decode_tokens(std::string_view text) {
  pcpp::Tokens out;
  if (text.empty()) {
    return out;
  }
  std::size_t pos = 0;
  while (true) {
    const std::size_t end = text.find('\x02', pos);
    const std::string_view item =
        text.substr(pos, end == std::string_view::npos ? std::string_view::npos : end - pos);
    const std::size_t sep = item.find('\x01');
    const std::string type(item.substr(0, sep));
    auto tok = std::make_shared<pcpp::Token>();
    tok->value = std::string(item.substr(sep + 1));
    tok->type = pcpp::Type::Char;
    for (int t = 0; t <= static_cast<int>(pcpp::Type::None); ++t) {
      if (type == pcpp::type_name(static_cast<pcpp::Type>(t))) {
        tok->type = static_cast<pcpp::Type>(t);
      }
    }
    if (tok->type == pcpp::Type::Char) {
      tok->chr = type;
    }
    out.push_back(tok);
    if (end == std::string_view::npos) {
      return out;
    }
    pos = end + 1;
  }
}

inline void check_evaluator_case(const EvaluatorCase& c) {
  const std::string mode = c.mode;
  const pcpp::FunctionHook functions = [&](const std::string& key) -> std::optional<std::string> {
    if (mode == "dict") throw pcpp::py_exception("SyntaxError", "Unknown function " + key);
    if (mode == "none") return key;
    return std::nullopt;
  };
  const pcpp::IdentifierHook identifiers =
      [&](const std::string& key) -> std::variant<std::int64_t, std::string> {
    if (mode == "dict") throw pcpp::py_exception("SyntaxError", "Unknown identifier " + key);
    if (key.rfind("defined(", 0) == 0) return std::int64_t{0};
    if (key == "true") return std::int64_t{1};
    if (key == "false") return std::int64_t{0};
    if (mode == "none") return key;
    return std::int64_t{0};
  };
  std::string got;
  try {
    const pcpp::EvalResult r = pcpp::evaluate(decode_tokens(c.tokens), functions, identifiers);
    got = "ok " + r.str();
  } catch (const pcpp::PyException& e) {
    got = "exc " + e.type + " | " + e.message + " | " + e.repr;
  }
  const std::string want = c.ok ? std::string("ok ") + c.a
                                : std::string("exc ") + c.a + " | " + c.b + " | " + c.c;
  INFO("mode ", mode, ", tokens ", std::string(c.tokens));
  CHECK(got == want);
}

}  // namespace c6502::testing
