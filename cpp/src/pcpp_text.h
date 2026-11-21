// The parts of Python's `str`, `os.path`, and text files that pcpp and
// preprocessor.py read, for the C++ port of the preprocessor.
//
// Python holds text as code points, and the C++ holds it as UTF-8, so
// these functions decode a code point wherever Python's answer depends
// on one: the Unicode classes of `\w` and `\d`, the line breaks of
// `str.splitlines()`, and the spaces of `str.strip()`. The paths follow
// `ntpath` on Windows and `posixpath` elsewhere, and a `PathFlavor`
// chooses between them, so that a call recorded on one system can be
// checked on the other.
#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace c6502::pcpp {

// Returns the code point that starts at `s[pos]` and sets `len` to its
// length in bytes. A byte that doesn't start a valid UTF-8 sequence is
// returned as itself, with a length of 1.
std::uint32_t decode_at(std::string_view s, std::size_t pos, std::size_t& len);

// Appends the UTF-8 of the code point `cp` to `out`.
void append_utf8(std::string& out, std::uint32_t cp);

// Returns the number of code points in `s`, Python's `len()` of it.
std::size_t code_point_count(std::string_view s);

// Return whether a code point is in the class Python gives `\w`, `\d`,
// `str.isspace()`, a line break of `str.splitlines()`, or
// `str.isprintable()`.
bool is_word(std::uint32_t cp);
bool is_digit(std::uint32_t cp);
bool is_space(std::uint32_t cp);
bool is_line_break(std::uint32_t cp);
bool is_printable(std::uint32_t cp);

// Returns `str.splitlines()` of `s`: the lines without their breaks,
// with "\r\n" as one break and no empty line after a final break.
std::vector<std::string> splitlines(std::string_view s);

// Return `s.rstrip()`, `s.strip()`, and `s.split()`, which strip and
// split at the code points of `str.isspace()`.
std::string rstrip(std::string_view s);
std::string strip(std::string_view s);
std::vector<std::string> split_whitespace(std::string_view s);

// Returns Python's `repr` of the string `s`, with every code point that
// `str.isprintable()` rejects escaped as `\xhh`, `\uhhhh`, or
// `\Uhhhhhhhh`.
std::string py_repr(std::string_view s);

// Returns Python's `repr` of the bytes `b`, as `b'...'`.
std::string py_bytes_repr(std::string_view b);

// The rules of `os.path`: `ntpath` on Windows, `posixpath` elsewhere.
enum class PathFlavor { Nt, Posix };

// The flavor of the system this runs on.
PathFlavor native_flavor();

// The functions of `os.path` that pcpp calls, for a flavor and, for the
// ones that read it, the current directory. `abspath` on Windows is
// ntpath's `_abspath_fallback`, the join with the current directory and
// `normpath`, where Python calls `GetFullPathNameW`, except that a path
// rooted on no drive, `\x`, gets the drive of the current directory, as
// `GetFullPathNameW` gives it. The two still differ for names Windows
// treats specially, such as a component that ends in a period or a
// space, and for a drive-relative path on another drive, `D:x`.
struct Paths {
  PathFlavor flavor = PathFlavor::Nt;
  std::string cwd;

  char sep() const { return flavor == PathFlavor::Nt ? '\\' : '/'; }
  std::pair<std::string, std::string> splitdrive(std::string_view p) const;
  bool isabs(std::string_view p) const;
  std::string join(std::string_view a, std::string_view b) const;
  std::pair<std::string, std::string> split(std::string_view p) const;
  std::string dirname(std::string_view p) const { return split(p).first; }
  std::string normpath(std::string_view p) const;
  std::string abspath(std::string_view p) const;
  std::string normcase(std::string_view p) const;
  // Returns `os.path.relpath(path)` from the current directory, or
  // nullopt where Python raises ValueError: for an empty path, and on
  // Windows for a path on another drive.
  std::optional<std::string> relpath(std::string_view path) const;
};

// Decodes the bytes of a file as Python's `open(path, 'r',
// encoding=...)` and `read()` do: with the codec `encoding` (cp1252,
// UTF-8, Latin-1, or ASCII, under any of Python's names for them), and
// with universal newlines, which make "\r\n" and "\r" each "\n". Throws
// CompileError of type "UnicodeDecodeError" or "LookupError" with
// Python's message where Python raises one.
std::string decode_text_file(std::string_view bytes, std::string_view encoding);

}  // namespace c6502::pcpp
