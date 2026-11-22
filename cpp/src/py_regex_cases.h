// The check of a record of Python's `re` on one pattern, which the C++
// tests of py_regex and `c6502cpp check-regex` run.
//
// A record is a JSON object with the pattern (`pattern`), what
// `re.compile(pattern)` raised (`error`, `[type, message]`, or null),
// and, for a pattern that compiled, the subjects (`subjects`) and the
// end of the match that `.match(subject)` found on each (`ends`, -1 for
// none). The strings are JSON strings of their code points.
#pragma once

#include <string>
#include <string_view>

namespace c6502::py_regex {

struct RecordCheck {
  enum class Outcome {
    kMatch,        // the C++ gives Python's error or every match
    kUnsupported,  // the C++ reports the pattern as unsupported
    kDiffer,       // the C++ gives something else
  };
  Outcome outcome = Outcome::kMatch;
  // For kUnsupported, the C++ message, and for kDiffer, the difference.
  std::string detail;
};

// Compiles and matches the record's pattern in C++ and compares the
// results with Python's.
RecordCheck check_record(std::string_view record);

}  // namespace c6502::py_regex
