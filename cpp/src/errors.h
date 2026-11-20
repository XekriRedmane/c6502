// The exceptions of the C++ compiler, which stand for the Python
// compiler's (cpp/README.md, "Errors").
#pragma once

#include <stdexcept>
#include <string>
#include <utility>

namespace c6502 {

// A problem with the program being compiled, which the Python compiler
// raises as one of the types `compile._source_errors` lists. The CLI
// prints the message as a one-line error, as compile.py does.
// `python_type` names the Python exception (`ParserError`,
// `TypeCheckError`, `ValueError`, ...), so a test can check that both
// compilers reject a program for the same reason.
class CompileError : public std::runtime_error {
 public:
  CompileError(std::string python_type, const std::string& message)
      : std::runtime_error(message), python_type_(std::move(python_type)) {}

  const std::string& python_type() const noexcept { return python_type_; }

 private:
  std::string python_type_;
};

// A bug in the compiler, where the Python code raises `AssertionError`
// or `IRCheckError`. The CLI prints it with "internal error" in front.
class InternalError : public std::logic_error {
 public:
  using std::logic_error::logic_error;
};

}  // namespace c6502
