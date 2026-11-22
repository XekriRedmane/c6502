// Python 3.11's `fnmatch.fnmatchcase`, which the linker's layout uses to
// match a script's file and section patterns.
#pragma once

#include <string>
#include <string_view>

namespace c6502::fnmatch {

// Returns Python's `fnmatch.translate(pat)`: the regular expression,
// as UTF-8, that matches what the shell pattern `pat` (UTF-8) matches.
// `*` matches any run of characters, `?` any one character, and
// `[...]` one character of a set, with `!` in front for its complement
// and `a-z` for a range. A `[` with no `]` after it is itself. Throws
// `InternalError` where Python's `translate` raises IndexError, which
// is for a set whose ranges run backward twice in a row, such as
// `[c-b-a]`.
std::string translate(std::string_view pat);

// Returns whether `name` matches the shell pattern `pat`, both UTF-8,
// with case significant, as `fnmatch.fnmatchcase(name, pat)` gives it:
// `translate(pat)` matched with Python's `re` (py_regex) at the start
// of `name`. Throws `InternalError` for a translation that py_regex
// doesn't take, which is one where Python's `re` prints a
// FutureWarning, such as `[[` in a set.
bool fnmatchcase(std::string_view name, std::string_view pat);

}  // namespace c6502::fnmatch
