// diagnostics.py: warnings, which report a problem in the program being
// compiled without stopping the compile.
//
// A pass that finds something to warn about calls `warn` with the place
// and the message. The warning goes to the innermost `Collecting` block
// that is open, and is dropped when none is, so a caller that opens no
// such block never receives them. compile.py collects the warnings of
// each C input and prints them to stderr as
// `<file>: warning: line N: <message>`, the form its errors take.
//
//     std::vector<diagnostics::CompileWarning> found;
//     {
//       diagnostics::Collecting block(found);
//       text = preprocess(source);
//     }
//     for (const auto& w : found) print("x.c: warning: " + w.str());
//
// The only warning so far is the preprocessor's, for each `_Bool`.
#pragma once

#include <string>
#include <vector>

namespace c6502::diagnostics {

// One warning. `where` names the line, as "line 3" in the file being
// compiled or "line 3 of foo.h" in a file it includes, the way the
// preprocessor's errors name it.
struct CompileWarning {
  std::string where;
  std::string message;

  // Returns the text after `warning: `, as Python's `str()` gives it.
  std::string str() const { return where + ": " + message; }

  bool operator==(const CompileWarning&) const = default;
};

// Records a warning at `where` in the innermost open `Collecting` block,
// or drops it outside one.
void warn(const std::string& where, const std::string& message);

// Collects the warnings issued while it lives, in the order they were
// issued, into the vector it is given. A block opened inside another
// takes its warnings for itself, and the outer vector doesn't get them.
// The innermost block is kept per thread, as Python keeps it in a
// context variable.
class Collecting {
 public:
  explicit Collecting(std::vector<CompileWarning>& found);
  ~Collecting();
  Collecting(const Collecting&) = delete;
  Collecting& operator=(const Collecting&) = delete;

 private:
  std::vector<CompileWarning>* previous_;
};

}  // namespace c6502::diagnostics
