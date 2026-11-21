#include "pcpp_text.h"

#include <algorithm>
#include <cctype>
#include <cstdio>

#include "errors.h"
#include "pcpp_unicode.h"

namespace c6502::pcpp {

namespace {

template <std::size_t N>
bool in_ranges(const unicode::Range (&table)[N], std::uint32_t cp) {
  const auto* end = table + N;
  const auto* it = std::upper_bound(table, end, cp, [](std::uint32_t c, const unicode::Range& r) {
    return c < r.first;
  });
  return it != table && cp <= (it - 1)->last;
}

bool is_sep(PathFlavor flavor, char c) {
  return flavor == PathFlavor::Nt ? (c == '\\' || c == '/') : c == '/';
}

std::string replace_all(std::string s, char from, char to) {
  std::replace(s.begin(), s.end(), from, to);
  return s;
}

std::string hex_byte(unsigned b) {
  char buf[8];
  std::snprintf(buf, sizeof buf, "0x%02x", b);
  return buf;
}

[[noreturn]] void decode_error(const std::string& codec, std::string_view bytes, std::size_t start,
                               std::size_t end, const std::string& reason) {
  std::string message = "'" + codec + "' codec can't decode ";
  if (end - start == 1) {
    message += "byte " + hex_byte(static_cast<unsigned char>(bytes[start])) + " in position " +
               std::to_string(start);
  } else {
    message += "bytes in position " + std::to_string(start) + "-" + std::to_string(end - 1);
  }
  throw CompileError("UnicodeDecodeError", message + ": " + reason);
}

// Decodes `bytes` as strict UTF-8, with CPython's messages for the
// first invalid sequence.
std::string decode_utf8(std::string_view bytes) {
  const std::size_t n = bytes.size();
  std::size_t i = 0;
  auto byte = [&](std::size_t k) { return static_cast<unsigned char>(bytes[k]); };
  while (i < n) {
    const unsigned c = byte(i);
    if (c < 0x80) {
      ++i;
      continue;
    }
    std::size_t len = 0;
    unsigned lo = 0x80;
    unsigned hi = 0xBF;
    if (c >= 0xC2 && c <= 0xDF) {
      len = 2;
    } else if (c >= 0xE0 && c <= 0xEF) {
      len = 3;
      if (c == 0xE0) lo = 0xA0;
      if (c == 0xED) hi = 0x9F;
    } else if (c >= 0xF0 && c <= 0xF4) {
      len = 4;
      if (c == 0xF0) lo = 0x90;
      if (c == 0xF4) hi = 0x8F;
    } else {
      decode_error("utf-8", bytes, i, i + 1, "invalid start byte");
    }
    for (std::size_t k = 1; k < len; ++k) {
      if (i + k >= n) {
        decode_error("utf-8", bytes, i, i + k, "unexpected end of data");
      }
      const unsigned b = byte(i + k);
      const bool ok = k == 1 ? (b >= lo && b <= hi) : (b >= 0x80 && b <= 0xBF);
      if (!ok) {
        decode_error("utf-8", bytes, i, i + k, "invalid continuation byte");
      }
    }
    i += len;
  }
  return std::string(bytes);
}

// The code points of cp1252's bytes 0x80 to 0x9F, 0 where the byte is
// undefined.
constexpr std::uint32_t kCp1252High[32] = {
    0x20AC, 0,      0x201A, 0x0192, 0x201E, 0x2026, 0x2020, 0x2021, 0x02C6, 0x2030, 0x0160,
    0x2039, 0x0152, 0,      0x017D, 0,      0,      0x2018, 0x2019, 0x201C, 0x201D, 0x2022,
    0x2013, 0x2014, 0x02DC, 0x2122, 0x0161, 0x203A, 0x0153, 0,      0x017E, 0x0178,
};

// Returns Python's name of the codec `encoding`, as
// `encodings.normalize_encoding` and the alias table give it, for the
// codecs the port knows, or "" for any other.
std::string codec_name(std::string_view encoding) {
  std::string norm;
  bool punct = false;
  for (const char ch : encoding) {
    const unsigned char c = static_cast<unsigned char>(ch);
    if (std::isalnum(c) || c == '.') {
      if (punct && !norm.empty()) norm += '_';
      norm += static_cast<char>(std::tolower(c));
      punct = false;
    } else {
      punct = true;
    }
  }
  static const std::pair<const char*, const char*> kAliases[] = {
      {"utf_8", "utf-8"},          {"utf8", "utf-8"},
      {"u8", "utf-8"},             {"utf", "utf-8"},
      {"utf8_ucs2", "utf-8"},      {"utf8_ucs4", "utf-8"},
      {"cp65001", "utf-8"},        {"utf_8_sig", "utf-8-sig"},
      {"utf8_sig", "utf-8-sig"},   {"latin_1", "latin-1"},
      {"latin1", "latin-1"},       {"latin", "latin-1"},
      {"l1", "latin-1"},           {"iso8859_1", "latin-1"},
      {"iso_8859_1", "latin-1"},   {"8859", "latin-1"},
      {"cp819", "latin-1"},        {"iso_ir_100", "latin-1"},
      {"csisolatin1", "latin-1"},  {"iso_8859_1_1987", "latin-1"},
      {"ibm819", "latin-1"},       {"cp1252", "cp1252"},
      {"windows_1252", "cp1252"},  {"1252", "cp1252"},
      {"ascii", "ascii"},          {"us_ascii", "ascii"},
      {"646", "ascii"},            {"us", "ascii"},
      {"ansi_x3.4_1968", "ascii"}, {"ansi_x3_4_1968", "ascii"},
      {"ansi_x3.4_1986", "ascii"}, {"cp367", "ascii"},
      {"csascii", "ascii"},        {"ibm367", "ascii"},
      {"iso646_us", "ascii"},      {"iso_646.irv_1991", "ascii"},
      {"iso_ir_6", "ascii"},
  };
  for (const auto& [alias, name] : kAliases) {
    if (norm == alias) return name;
  }
  return "";
}

}  // namespace

std::uint32_t decode_at(std::string_view s, std::size_t pos, std::size_t& len) {
  const unsigned char c = static_cast<unsigned char>(s[pos]);
  len = 1;
  if (c < 0x80) return c;
  std::size_t n = 0;
  std::uint32_t cp = 0;
  if ((c & 0xE0) == 0xC0) {
    n = 2;
    cp = c & 0x1F;
  } else if ((c & 0xF0) == 0xE0) {
    n = 3;
    cp = c & 0x0F;
  } else if ((c & 0xF8) == 0xF0) {
    n = 4;
    cp = c & 0x07;
  } else {
    return c;
  }
  if (pos + n > s.size()) return c;
  for (std::size_t k = 1; k < n; ++k) {
    const unsigned char b = static_cast<unsigned char>(s[pos + k]);
    if ((b & 0xC0) != 0x80) return c;
    cp = (cp << 6) | (b & 0x3F);
  }
  len = n;
  return cp;
}

void append_utf8(std::string& out, std::uint32_t cp) {
  if (cp < 0x80) {
    out += static_cast<char>(cp);
  } else if (cp < 0x800) {
    out += static_cast<char>(0xC0 | (cp >> 6));
    out += static_cast<char>(0x80 | (cp & 0x3F));
  } else if (cp < 0x10000) {
    out += static_cast<char>(0xE0 | (cp >> 12));
    out += static_cast<char>(0x80 | ((cp >> 6) & 0x3F));
    out += static_cast<char>(0x80 | (cp & 0x3F));
  } else {
    out += static_cast<char>(0xF0 | (cp >> 18));
    out += static_cast<char>(0x80 | ((cp >> 12) & 0x3F));
    out += static_cast<char>(0x80 | ((cp >> 6) & 0x3F));
    out += static_cast<char>(0x80 | (cp & 0x3F));
  }
}

std::size_t code_point_count(std::string_view s) {
  std::size_t count = 0;
  for (std::size_t i = 0; i < s.size();) {
    std::size_t len = 0;
    decode_at(s, i, len);
    i += len;
    ++count;
  }
  return count;
}

bool is_word(std::uint32_t cp) { return in_ranges(unicode::kWord, cp); }
bool is_digit(std::uint32_t cp) { return in_ranges(unicode::kDigit, cp); }
bool is_space(std::uint32_t cp) { return in_ranges(unicode::kSpace, cp); }
bool is_line_break(std::uint32_t cp) { return in_ranges(unicode::kLineBreak, cp); }
bool is_printable(std::uint32_t cp) { return in_ranges(unicode::kPrintable, cp); }

std::vector<std::string> splitlines(std::string_view s) {
  std::vector<std::string> lines;
  std::size_t start = 0;
  std::size_t i = 0;
  while (i < s.size()) {
    std::size_t len = 0;
    const std::uint32_t cp = decode_at(s, i, len);
    if (is_line_break(cp)) {
      lines.emplace_back(s.substr(start, i - start));
      i += len;
      if (cp == '\r' && i < s.size() && s[i] == '\n') ++i;
      start = i;
    } else {
      i += len;
    }
  }
  if (start < s.size()) lines.emplace_back(s.substr(start));
  return lines;
}

namespace {

// Returns the offset just past the last code point of `s` that isn't
// a space.
std::size_t end_of_text(std::string_view s) {
  std::size_t end = 0;
  for (std::size_t i = 0; i < s.size();) {
    std::size_t len = 0;
    const std::uint32_t cp = decode_at(s, i, len);
    i += len;
    if (!is_space(cp)) end = i;
  }
  return end;
}

}  // namespace

std::string rstrip(std::string_view s) { return std::string(s.substr(0, end_of_text(s))); }

std::string strip(std::string_view s) {
  std::size_t i = 0;
  while (i < s.size()) {
    std::size_t len = 0;
    if (!is_space(decode_at(s, i, len))) break;
    i += len;
  }
  return rstrip(s.substr(i));
}

std::vector<std::string> split_whitespace(std::string_view s) {
  std::vector<std::string> words;
  std::string word;
  bool in_word = false;
  for (std::size_t i = 0; i < s.size();) {
    std::size_t len = 0;
    const std::uint32_t cp = decode_at(s, i, len);
    if (is_space(cp)) {
      if (in_word) words.push_back(word);
      word.clear();
      in_word = false;
    } else {
      word.append(s.substr(i, len));
      in_word = true;
    }
    i += len;
  }
  if (in_word) words.push_back(word);
  return words;
}

std::string py_repr(std::string_view s) {
  const bool has_single = s.find('\'') != std::string_view::npos;
  const bool has_double = s.find('"') != std::string_view::npos;
  const char quote = has_single && !has_double ? '"' : '\'';
  std::string out(1, quote);
  for (std::size_t i = 0; i < s.size();) {
    std::size_t len = 0;
    const std::uint32_t cp = decode_at(s, i, len);
    i += len;
    char buf[16];
    if (cp == static_cast<std::uint32_t>(quote) || cp == '\\') {
      out += '\\';
      out += static_cast<char>(cp);
    } else if (cp == '\t') {
      out += "\\t";
    } else if (cp == '\n') {
      out += "\\n";
    } else if (cp == '\r') {
      out += "\\r";
    } else if (is_printable(cp)) {
      append_utf8(out, cp);
    } else if (cp < 0x100) {
      std::snprintf(buf, sizeof buf, "\\x%02x", cp);
      out += buf;
    } else if (cp < 0x10000) {
      std::snprintf(buf, sizeof buf, "\\u%04x", cp);
      out += buf;
    } else {
      std::snprintf(buf, sizeof buf, "\\U%08x", cp);
      out += buf;
    }
  }
  out += quote;
  return out;
}

std::string py_bytes_repr(std::string_view b) {
  const bool has_single = b.find('\'') != std::string_view::npos;
  const bool has_double = b.find('"') != std::string_view::npos;
  const char quote = has_single && !has_double ? '"' : '\'';
  std::string out = "b";
  out += quote;
  for (const char ch : b) {
    const unsigned char c = static_cast<unsigned char>(ch);
    char buf[8];
    if (c == static_cast<unsigned char>(quote) || c == '\\') {
      out += '\\';
      out += ch;
    } else if (c == '\t') {
      out += "\\t";
    } else if (c == '\n') {
      out += "\\n";
    } else if (c == '\r') {
      out += "\\r";
    } else if (c < 0x20 || c >= 0x7F) {
      std::snprintf(buf, sizeof buf, "\\x%02x", c);
      out += buf;
    } else {
      out += ch;
    }
  }
  out += quote;
  return out;
}

PathFlavor native_flavor() {
#ifdef _WIN32
  return PathFlavor::Nt;
#else
  return PathFlavor::Posix;
#endif
}

std::pair<std::string, std::string> Paths::splitdrive(std::string_view p) const {
  if (flavor == PathFlavor::Posix || p.size() < 2) return {"", std::string(p)};
  const std::string normp = replace_all(std::string(p), '/', '\\');
  if (normp.compare(0, 2, "\\\\") == 0) {
    std::string upper8 = normp.substr(0, 8);
    for (auto& c : upper8) c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
    const std::size_t start = upper8 == "\\\\?\\UNC\\" ? 8 : 2;
    const std::size_t index = normp.find('\\', start);
    if (index == std::string::npos) return {std::string(p), ""};
    const std::size_t index2 = normp.find('\\', index + 1);
    if (index2 == std::string::npos) return {std::string(p), ""};
    return {std::string(p.substr(0, index2)), std::string(p.substr(index2))};
  }
  if (normp[1] == ':') return {std::string(p.substr(0, 2)), std::string(p.substr(2))};
  return {"", std::string(p)};
}

bool Paths::isabs(std::string_view p) const {
  if (flavor == PathFlavor::Posix) return !p.empty() && p[0] == '/';
  const std::string s = replace_all(std::string(p.substr(0, 3)), '/', '\\');
  return (!s.empty() && s[0] == '\\') || (s.size() >= 3 && s.compare(1, 2, ":\\") == 0);
}

std::string Paths::join(std::string_view a, std::string_view b) const {
  if (flavor == PathFlavor::Posix) {
    std::string path(a);
    if (!b.empty() && b[0] == '/') {
      path = std::string(b);
    } else if (path.empty() || path.back() == '/') {
      path += b;
    } else {
      path += '/';
      path += b;
    }
    return path;
  }
  auto [result_drive, result_path] = splitdrive(a);
  auto [p_drive, p_path] = splitdrive(b);
  auto lower = [](std::string s) {
    for (auto& c : s) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    return s;
  };
  if (!p_path.empty() && is_sep(flavor, p_path[0])) {
    if (!p_drive.empty() || result_drive.empty()) result_drive = p_drive;
    result_path = p_path;
  } else {
    bool absolute_done = false;
    if (!p_drive.empty() && p_drive != result_drive) {
      if (lower(p_drive) != lower(result_drive)) {
        result_drive = p_drive;
        result_path = p_path;
        absolute_done = true;
      } else {
        result_drive = p_drive;
      }
    }
    if (!absolute_done) {
      if (!result_path.empty() && !is_sep(flavor, result_path.back())) result_path += '\\';
      result_path += p_path;
    }
  }
  if (!result_path.empty() && !is_sep(flavor, result_path[0]) && !result_drive.empty() &&
      result_drive.back() != ':') {
    return result_drive + "\\" + result_path;
  }
  return result_drive + result_path;
}

std::pair<std::string, std::string> Paths::split(std::string_view p) const {
  if (flavor == PathFlavor::Posix) {
    const std::size_t found = p.rfind('/');
    const std::size_t i = found == std::string_view::npos ? 0 : found + 1;
    std::string head(p.substr(0, i));
    std::string tail(p.substr(i));
    if (!head.empty() && head.find_first_not_of('/') != std::string::npos) {
      head.erase(head.find_last_not_of('/') + 1);
    }
    return {head, tail};
  }
  auto [d, rest] = splitdrive(p);
  std::size_t i = rest.size();
  while (i > 0 && !is_sep(flavor, rest[i - 1])) --i;
  std::string head = rest.substr(0, i);
  std::string tail = rest.substr(i);
  const std::size_t last = head.find_last_not_of("\\/");
  if (last != std::string::npos) head.erase(last + 1);
  return {d + head, tail};
}

std::string Paths::normpath(std::string_view p) const {
  if (flavor == PathFlavor::Posix) {
    if (p.empty()) return ".";
    int initial_slashes = p[0] == '/' ? 1 : 0;
    if (initial_slashes && p.size() >= 2 && p[1] == '/' && !(p.size() >= 3 && p[2] == '/')) {
      initial_slashes = 2;
    }
    std::vector<std::string> comps;
    std::size_t start = 0;
    const std::string s(p);
    while (true) {
      const std::size_t end = s.find('/', start);
      const std::string comp = s.substr(start, end == std::string::npos ? std::string::npos : end - start);
      if (!comp.empty() && comp != ".") {
        if (comp != ".." || (!initial_slashes && comps.empty()) ||
            (!comps.empty() && comps.back() == "..")) {
          comps.push_back(comp);
        } else if (!comps.empty()) {
          comps.pop_back();
        }
      }
      if (end == std::string::npos) break;
      start = end + 1;
    }
    std::string out;
    for (std::size_t k = 0; k < comps.size(); ++k) {
      if (k) out += '/';
      out += comps[k];
    }
    out = std::string(static_cast<std::size_t>(initial_slashes), '/') + out;
    return out.empty() ? "." : out;
  }
  const std::string path = replace_all(std::string(p), '/', '\\');
  auto [prefix, rest] = splitdrive(path);
  if (!rest.empty() && rest[0] == '\\') {
    prefix += '\\';
    rest.erase(0, rest.find_first_not_of('\\') == std::string::npos ? rest.size()
                                                                     : rest.find_first_not_of('\\'));
  }
  std::vector<std::string> comps;
  std::size_t start = 0;
  while (true) {
    const std::size_t end = rest.find('\\', start);
    comps.push_back(rest.substr(start, end == std::string::npos ? std::string::npos : end - start));
    if (end == std::string::npos) break;
    start = end + 1;
  }
  std::size_t i = 0;
  while (i < comps.size()) {
    if (comps[i].empty() || comps[i] == ".") {
      comps.erase(comps.begin() + static_cast<std::ptrdiff_t>(i));
    } else if (comps[i] == "..") {
      if (i > 0 && comps[i - 1] != "..") {
        comps.erase(comps.begin() + static_cast<std::ptrdiff_t>(i - 1),
                    comps.begin() + static_cast<std::ptrdiff_t>(i + 1));
        --i;
      } else if (i == 0 && !prefix.empty() && prefix.back() == '\\') {
        comps.erase(comps.begin());
      } else {
        ++i;
      }
    } else {
      ++i;
    }
  }
  if (prefix.empty() && comps.empty()) comps.push_back(".");
  std::string out = prefix;
  for (std::size_t k = 0; k < comps.size(); ++k) {
    if (k) out += '\\';
    out += comps[k];
  }
  return out;
}

std::string Paths::abspath(std::string_view p) const {
  if (flavor == PathFlavor::Nt && isabs(p) && splitdrive(p).first.empty() && !p.empty() &&
      !(p.size() >= 2 && is_sep(flavor, p[0]) && is_sep(flavor, p[1]))) {
    // `GetFullPathNameW` gives a path rooted on no drive, `\x`, the drive
    // of the current directory.
    return normpath(splitdrive(cwd).first + std::string(p));
  }
  if (isabs(p)) return normpath(p);
  return normpath(join(cwd, p));
}

std::string Paths::normcase(std::string_view p) const {
  if (flavor == PathFlavor::Posix) return std::string(p);
  std::string s = replace_all(std::string(p), '/', '\\');
  for (auto& c : s) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
  return s;
}

std::optional<std::string> Paths::relpath(std::string_view path) const {
  if (path.empty()) return std::nullopt;
  auto parts = [&](const std::string& s) {
    std::vector<std::string> out;
    std::size_t start = 0;
    while (true) {
      const std::size_t end = s.find(sep(), start);
      std::string comp = s.substr(start, end == std::string::npos ? std::string::npos : end - start);
      if (!comp.empty()) out.push_back(comp);
      if (end == std::string::npos) break;
      start = end + 1;
    }
    return out;
  };
  std::vector<std::string> start_list;
  std::vector<std::string> path_list;
  if (flavor == PathFlavor::Nt) {
    const auto [start_drive, start_rest] = splitdrive(abspath(normpath(".")));
    const auto [path_drive, path_rest] = splitdrive(abspath(normpath(path)));
    if (normcase(start_drive) != normcase(path_drive)) return std::nullopt;
    start_list = parts(start_rest);
    path_list = parts(path_rest);
  } else {
    start_list = parts(abspath("."));
    path_list = parts(abspath(path));
  }
  std::size_t i = 0;
  while (i < start_list.size() && i < path_list.size() &&
         (flavor == PathFlavor::Nt ? normcase(start_list[i]) == normcase(path_list[i])
                                   : start_list[i] == path_list[i])) {
    ++i;
  }
  std::vector<std::string> rel(start_list.size() - i, "..");
  rel.insert(rel.end(), path_list.begin() + static_cast<std::ptrdiff_t>(i), path_list.end());
  if (rel.empty()) return ".";
  std::string out = rel[0];
  for (std::size_t k = 1; k < rel.size(); ++k) out = join(out, rel[k]);
  return out;
}

std::string decode_text_file(std::string_view bytes, std::string_view encoding) {
  const std::string codec = codec_name(encoding);
  std::string text;
  if (codec == "utf-8" || codec == "utf-8-sig") {
    if (codec == "utf-8-sig" && bytes.substr(0, 3) == "\xEF\xBB\xBF") bytes.remove_prefix(3);
    text = decode_utf8(bytes);
  } else if (codec == "latin-1" || codec == "cp1252" || codec == "ascii") {
    for (std::size_t i = 0; i < bytes.size(); ++i) {
      const unsigned char c = static_cast<unsigned char>(bytes[i]);
      if (c < 0x80) {
        text += static_cast<char>(c);
      } else if (codec == "ascii") {
        decode_error("ascii", bytes, i, i + 1, "ordinal not in range(128)");
      } else if (codec == "cp1252" && c < 0xA0) {
        const std::uint32_t cp = kCp1252High[c - 0x80];
        if (cp == 0) decode_error("charmap", bytes, i, i + 1, "character maps to <undefined>");
        append_utf8(text, cp);
      } else {
        append_utf8(text, c);
      }
    }
  } else {
    throw CompileError("LookupError", "unknown encoding: " + std::string(encoding));
  }
  std::string out;
  out.reserve(text.size());
  for (std::size_t i = 0; i < text.size(); ++i) {
    if (text[i] == '\r') {
      out += '\n';
      if (i + 1 < text.size() && text[i + 1] == '\n') ++i;
    } else {
      out += text[i];
    }
  }
  return out;
}

}  // namespace c6502::pcpp
