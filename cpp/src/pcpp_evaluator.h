// pcpp 1.30's evaluator of `#if` and `#elif` expressions
// (pcpp/evaluator.py), with the parse loop of PLY's yacc that runs it.
//
// pcpp evaluates an expression with a PLY LALR parser whose actions
// compute on `Value`, a Python int held to 64 bits that carries a flag
// for unsigned and, in place of an exception, the exception an operation
// raised. The C++ runs the same tables (pcpp_evaluator_tables.h,
// generated from the live parser) with the same loop, and computes each
// value and each exception as Python does, so that the preprocessor's
// message for an expression it can't evaluate, which shows `repr` of the
// exception, comes out the same.
#pragma once

#include <cstdint>
#include <exception>
#include <functional>
#include <optional>
#include <string>
#include <string_view>
#include <variant>

#include "pcpp_token.h"

namespace c6502::pcpp {

// A Python exception that escapes the evaluator, with its class name,
// `str(e)`, and `repr(e)`.
struct PyException : std::exception {
  std::string type;
  std::string message;
  std::string repr;

  const char* what() const noexcept override { return message.c_str(); }
};

// Returns the exception `type(message)`, with Python's repr of an
// exception made from one string argument.
PyException py_exception(std::string type, std::string message);

// The Python int that an expression gives, as its 64 bits and whether
// Python holds it as an unsigned value (0 to 2**64 - 1) or a signed one
// (-2**63 to 2**63 - 1).
struct EvalResult {
  std::uint64_t bits = 0;
  bool is_unsigned = false;

  // Returns the int in decimal, as Python's `str` gives it.
  std::string str() const;

  // Returns whether the int is nonzero.
  bool truthy() const { return bits != 0; }
};

// The identifiers of an expression, as `Preprocessor.evalexpr`'s
// `IndirectToMacroHook` gives them, which is the int it returns for the
// name, or the name itself, which it returns when the preprocessor's
// hook returns None.
using IdentifierHook = std::function<std::variant<std::int64_t, std::string>(const std::string& key)>;

// The functions of an expression, as `IndirectToMacroFunctionHook`
// gives them, which is nullopt for a callable that returns 0 (pcpp's
// default), or the name itself, a string whose call raises
// `TypeError("'str' object is not callable")`.
using FunctionHook = std::function<std::optional<std::string>(const std::string& key)>;

// Evaluates the tokens of an expression after macro expansion, as
// `Evaluator.__call__(tokens, functions, identifiers).value()` does, and
// returns the int, or throws the PyException Python raises. A hook may
// throw a PyException, which becomes the exception of the identifier's
// or call's value, as a Python exception in pcpp's hook does.
//
// A `\N{name}` escape in a character constant throws InternalError,
// since the Unicode database of character names isn't ported.
// preprocessor.cpp rewrites each character constant with
// `expand_escapes_without_names` before it calls the evaluator, as
// preprocessor.py does, so no compile reaches that error.
EvalResult evaluate(const Tokens& tokens, const FunctionHook& functions,
                    const IdentifierHook& identifiers);

// The expansion of the escapes in the text between a character
// constant's quotes that pcpp's evaluator would make if its
// `_expand_escape_sequences_pat` had no `\N{name}` alternative.
// `expanded` holds the expansion (UTF-8) up to the first escape the
// codec fails to decode, and `rest` holds the text from that escape on,
// which is empty when every escape decodes.
struct EscapeExpansion {
  std::string expanded;
  std::string rest;
};

// Returns that expansion of `inner` (UTF-8).
EscapeExpansion expand_escapes_without_names(std::string_view inner);

// Returns Python's `repr` of the string `s` (UTF-8), escaping every code
// point that `str.isprintable` rejects as Python does.
std::string repr_str(std::string_view s);

}  // namespace c6502::pcpp
