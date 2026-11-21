// Python's `repr` and formatting of the values that the messages and
// the text of the ported code show, so that what the C++ compiler
// writes is what the Python one writes, character for character.
#pragma once

#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

#include "asdl/runtime.h"

namespace c6502::pyrepr {

// Returns Python's `repr` of the string `s`: in single quotes, or in
// double quotes when `s` holds a single quote and no double quote,
// with a backslash, the quote, and the control characters escaped as
// Python escapes them, and the code points U+0080 to U+00A0 and U+00AD
// as `\x80` and so on, as Python writes those of a `str` that holds
// bytes. Every other code point from U+0080 up is copied, which gives
// Python's form for a UTF-8 string of printable characters. Python
// escapes the unprintable ones above U+00FF too, which no message has
// needed so far.
std::string string(std::string_view s);

// Returns `v` in decimal, as Python's `str` and `repr` give an int.
std::string integer(asdl::Int v);

// Returns `v` in hexadecimal with uppercase digits, padded with zeros
// to `width` characters, as Python's `f"{v:0{width}X}"` gives it. A
// negative value has a `-` in front, which counts toward the width.
std::string hex(asdl::Int v, int width);

// Returns `v` with its sign, as Python's `f"{v:+d}"` gives it.
std::string signed_integer(asdl::Int v);

// Returns the code points of the UTF-8 string `s`. A Python `str` that
// holds bytes, such as `StringInit.str`, holds them as the code points
// 0 to 255, and its C++ string holds their UTF-8.
std::vector<std::uint32_t> code_points(std::string_view s);

}  // namespace c6502::pyrepr
