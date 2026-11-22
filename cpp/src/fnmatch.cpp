// A port of `fnmatch.translate` and `fnmatchcase` from Python 3.11's
// Lib/fnmatch.py, which work on code points, matched with py_regex,
// which matches as Python's `re` does.
#include "fnmatch.h"

#include <map>
#include <optional>
#include <vector>

#include "errors.h"
#include "lark_lexer.h"
#include "py_regex.h"

namespace c6502::fnmatch {

namespace {

// Returns Python 3.11's `re.escape` of one character, which puts a
// backslash before each of `()[]{}?*+-|^$\.&~# \t\n\r\v\f`.
std::u32string escape(char32_t c) {
  static const std::u32string_view kSpecial = U"()[]{}?*+-|^$\\.&~# \t\n\r\v\f";
  if (kSpecial.find(c) != std::u32string_view::npos) return std::u32string{U'\\', c};
  return std::u32string(1, c);
}

std::u32string replace_all(std::u32string s, std::u32string_view from, std::u32string_view to) {
  std::size_t at = 0;
  while ((at = s.find(from, at)) != std::u32string::npos) {
    s.replace(at, from.size(), to);
    at += to.size();
  }
  return s;
}

[[noreturn]] void index_error() {
  throw InternalError("fnmatch.translate raises IndexError for this pattern");
}

}  // namespace

std::string translate(std::string_view pattern) {
  const std::u32string pat = lark::decode_utf8(pattern);
  // Each piece is a regular expression, or none for a `*`.
  std::vector<std::optional<std::u32string>> res;
  std::size_t i = 0;
  const std::size_t n = pat.size();
  while (i < n) {
    const char32_t c = pat[i];
    i = i + 1;
    if (c == U'*') {
      // Consecutive `*`s are one.
      if (res.empty() || res.back().has_value()) res.emplace_back(std::nullopt);
    } else if (c == U'?') {
      res.emplace_back(U".");
    } else if (c == U'[') {
      std::size_t j = i;
      if (j < n && pat[j] == U'!') j = j + 1;
      if (j < n && pat[j] == U']') j = j + 1;
      while (j < n && pat[j] != U']') j = j + 1;
      if (j >= n) {
        res.emplace_back(U"\\[");
        continue;
      }
      std::u32string stuff = pat.substr(i, j - i);
      if (stuff.find(U'-') == std::u32string::npos) {
        stuff = replace_all(stuff, U"\\", U"\\\\");
      } else {
        std::vector<std::u32string> chunks;
        std::size_t k = pat[i] == U'!' ? i + 2 : i + 1;
        while (true) {
          // `pat.find('-', k, j)`.
          std::size_t found = std::u32string::npos;
          for (std::size_t m = k; m < j; ++m) {
            if (pat[m] == U'-') {
              found = m;
              break;
            }
          }
          if (found == std::u32string::npos) break;
          chunks.push_back(pat.substr(i, found - i));
          i = found + 1;
          k = found + 3;
        }
        const std::u32string chunk = pat.substr(i, j - i);
        if (!chunk.empty()) {
          chunks.push_back(chunk);
        } else {
          chunks.back() += U'-';
        }
        // Remove empty ranges, which are invalid in a regular
        // expression.
        for (std::size_t m = chunks.size() - 1; m > 0; --m) {
          if (chunks[m - 1].empty() || chunks[m].empty()) index_error();
          if (chunks[m - 1].back() > chunks[m].front()) {
            chunks[m - 1] = chunks[m - 1].substr(0, chunks[m - 1].size() - 1) + chunks[m].substr(1);
            chunks.erase(chunks.begin() + static_cast<std::ptrdiff_t>(m));
          }
        }
        // Escape backslashes and hyphens for set difference (--).
        // Hyphens that create ranges aren't escaped.
        stuff.clear();
        for (std::size_t m = 0; m < chunks.size(); ++m) {
          if (m > 0) stuff += U'-';
          stuff += replace_all(replace_all(chunks[m], U"\\", U"\\\\"), U"-", U"\\-");
        }
      }
      // Escape set operations (&&, ~~ and ||).
      std::u32string escaped;
      for (const char32_t ch : stuff) {
        if (ch == U'&' || ch == U'~' || ch == U'|') escaped += U'\\';
        escaped += ch;
      }
      stuff = escaped;
      i = j + 1;
      if (stuff.empty()) {
        // An empty range never matches.
        res.emplace_back(U"(?!)");
      } else if (stuff == U"!") {
        // A negated empty range matches any character.
        res.emplace_back(U".");
      } else {
        if (stuff[0] == U'!') {
          stuff = U"^" + stuff.substr(1);
        } else if (stuff[0] == U'^' || stuff[0] == U'[') {
          stuff = U"\\" + stuff;
        }
        res.emplace_back(U"[" + stuff + U"]");
      }
    } else {
      res.emplace_back(escape(c));
    }
  }
  // Each interior `*` and the fixed text after it is a minimal `.*?`
  // match of the text in an atomic group, so nothing backtracks into
  // it, and a final `*` is `.*`.
  std::u32string out;
  std::size_t p = 0;
  while (p < res.size() && res[p].has_value()) out += *res[p++];
  while (p < res.size()) {
    ++p;  // the `*`
    if (p == res.size()) {
      out += U".*";
      break;
    }
    std::u32string fixed;
    while (p < res.size() && res[p].has_value()) fixed += *res[p++];
    if (p == res.size()) {
      out += U".*" + fixed;
    } else {
      out += U"(?>.*?" + fixed + U")";
    }
  }
  return lark::encode_utf8(U"(?s:" + out + U")\\Z");
}

bool fnmatchcase(std::string_view name, std::string_view pat) {
  // Python keeps the compiled patterns in an `lru_cache`.
  static std::map<std::string, py_regex::Regex, std::less<>> cache;
  auto it = cache.find(pat);
  if (it == cache.end()) {
    try {
      it = cache.emplace(std::string(pat), py_regex::Regex(translate(pat))).first;
    } catch (const py_regex::Unsupported& e) {
      throw InternalError(std::string("fnmatch: ") + e.what());
    }
  }
  return it->second.match(lark::decode_utf8(name), 0).has_value();
}

}  // namespace c6502::fnmatch
