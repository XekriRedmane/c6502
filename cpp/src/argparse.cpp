#include "argparse.h"

#include <algorithm>
#include <cstdlib>
#include <string_view>
#include <utility>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#else
#include <sys/ioctl.h>
#include <unistd.h>
#endif

#include "pyrepr.h"

namespace c6502::argparse {

namespace {

bool starts_with(std::string_view s, std::string_view prefix) {
  return s.substr(0, prefix.size()) == prefix;
}

// textwrap's `_whitespace`, the characters its `wordsep_re` splits at.
bool is_wrap_space(char c) {
  return c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r' || c == ' ';
}

// The classes of textwrap's `wordsep_re` on a byte: `\w`, `letter`
// (`[^\d\W]`, a word character that isn't a digit), and `word_punct`
// (`[\w!"'&.,?]`). A byte of a multibyte UTF-8 character counts as a
// letter, which Python's Unicode classes give the letters of other
// alphabets.
bool is_word(char c) {
  const auto u = static_cast<unsigned char>(c);
  return u >= 0x80 || (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') ||
         (c >= 'A' && c <= 'Z') || c == '_';
}

bool is_letter(char c) { return is_word(c) && !(c >= '0' && c <= '9'); }

bool is_word_punct(char c) {
  return is_word(c) || c == '!' || c == '"' || c == '\'' || c == '&' || c == '.' || c == ',' ||
         c == '?';
}

// Returns the chunks `TextWrapper._split` gives: `wordsep_re.split(text)`
// with the empty pieces dropped. The pattern is, in order of its
// alternatives,
//
//     ( [ws]+                                    a run of whitespace
//     | (?<=wp) -{2,} (?=\w)                     an em-dash between words
//     | [^ws]+? (?: -(?: (?<=lt{2}-) | (?<=lt-lt-) ) (?= lt -? lt)
//                 | (?=[ws]|\Z)
//                 | (?<=wp) (?=-{2,}\w) ) )
//
// so a word ends after a hyphen that has two letters (or a letter, a
// hyphen, and a letter) before it and a letter after it, at whitespace
// or the end, or before an em-dash. The pattern matches at every
// position, so the chunks cover the text, and each is found here as
// the regex engine finds it, with the shortest word tried first.
std::vector<std::string> split_chunks(const std::string& t) {
  const std::size_t n = t.size();
  const auto at = [&](std::size_t k) { return k < n ? t[k] : '\0'; };
  // Whether `-{2,}\w` matches at `k`.
  const auto em_dash_at = [&](std::size_t k) {
    if (at(k) != '-' || at(k + 1) != '-') return false;
    std::size_t m = k;
    while (at(m) == '-') ++m;
    return m < n && is_word(t[m]);
  };
  std::vector<std::string> chunks;
  std::size_t i = 0;
  while (i < n) {
    std::size_t end = i;
    if (is_wrap_space(t[i])) {
      while (end < n && is_wrap_space(t[end])) ++end;
    } else if (i > 0 && is_word_punct(t[i - 1]) && em_dash_at(i)) {
      while (at(end) == '-') ++end;
    } else {
      for (std::size_t j = i + 1;; ++j) {
        // A hyphen that ends the word, with the letters around it.
        if (at(j) == '-') {
          const bool before = (j >= 2 && is_letter(t[j - 2]) && is_letter(t[j - 1])) ||
                              (j >= 3 && is_letter(t[j - 3]) && t[j - 2] == '-' &&
                               is_letter(t[j - 1]));
          const bool after =
              is_letter(at(j + 1)) &&
              (is_letter(at(j + 2)) || (at(j + 2) == '-' && is_letter(at(j + 3))));
          if (before && after) {
            end = j + 1;
            break;
          }
        }
        if (j >= n || is_wrap_space(t[j])) {
          end = j;
          break;
        }
        if (is_word_punct(t[j - 1]) && em_dash_at(j)) {
          end = j;
          break;
        }
      }
    }
    chunks.push_back(t.substr(i, end - i));
    i = end;
  }
  return chunks;
}

bool all_space(const std::string& s) {
  return std::all_of(s.begin(), s.end(), is_wrap_space);
}

// Returns `text` with each run of whitespace one space and the ends
// stripped, as `_whitespace_matcher.sub(' ', text).strip()` gives it.
std::string normalize(const std::string& text) {
  std::string out;
  bool space = false;
  for (char c : text) {
    if (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f' || c == '\v') {
      space = true;
      continue;
    }
    if (space && !out.empty()) out += ' ';
    space = false;
    out += c;
  }
  return out;
}

// Returns the parts `_format_usage` wraps the usage text in:
// `re.findall(r'\(.*?\)+(?=\s|$)|\[.*?\]+(?=\s|$)|\S+', text)`.
std::vector<std::string> usage_parts(const std::string& text) {
  std::vector<std::string> parts;
  std::size_t i = 0;
  const auto is_space = [](char c) { return c == ' ' || c == '\t' || c == '\n'; };
  while (i < text.size()) {
    if (is_space(text[i])) {
      ++i;
      continue;
    }
    const char open = text[i];
    if (open == '(' || open == '[') {
      const char close = open == '(' ? ')' : ']';
      // `.*?` stops at the first closer whose run of closers is followed
      // by whitespace or the end. `.` takes no newline.
      bool found = false;
      for (std::size_t j = i + 1; j < text.size() && text[j] != '\n'; ++j) {
        if (text[j] != close) continue;
        std::size_t k = j;
        while (k < text.size() && text[k] == close) ++k;
        if (k == text.size() || is_space(text[k])) {
          parts.push_back(text.substr(i, k - i));
          i = k;
          found = true;
          break;
        }
      }
      if (found) continue;
    }
    std::size_t k = i;
    while (k < text.size() && !is_space(text[k])) ++k;
    parts.push_back(text.substr(i, k - i));
    i = k;
  }
  return parts;
}

std::string join(const std::vector<std::string>& parts, const std::string& sep) {
  std::string out;
  for (std::size_t k = 0; k < parts.size(); ++k) {
    if (k != 0) out += sep;
    out += parts[k];
  }
  return out;
}

std::string upper(std::string s) {
  for (char& c : s) {
    if (c >= 'a' && c <= 'z') c = static_cast<char>(c - 'a' + 'A');
  }
  return s;
}

// The argument a positional takes, or an option's metavar
// (`_format_args`).
std::string format_args(const Action& a) {
  if (a.kind == ActionKind::kOneOrMore) {
    const std::string& m = a.metavar.empty() ? a.dest : a.metavar;
    return m + " [" + m + " ...]";
  }
  return a.metavar.empty() ? upper(a.dest) : a.metavar;
}

bool takes_no_argument(const Action& a) {
  return a.kind == ActionKind::kStoreTrue || a.kind == ActionKind::kStoreConst ||
         a.kind == ActionKind::kHelp;
}

// `_format_action_invocation`.
std::string invocation(const Action& a) {
  if (a.option_strings.empty()) return a.metavar.empty() ? a.dest : a.metavar;
  if (takes_no_argument(a)) return join(a.option_strings, ", ");
  std::vector<std::string> parts;
  for (const auto& s : a.option_strings) parts.push_back(s + " " + format_args(a));
  return join(parts, ", ");
}

// `_get_action_name`: an option's strings joined with `/`, or a
// positional's metavar or name.
std::string action_name(const Action& a) {
  if (!a.option_strings.empty()) return join(a.option_strings, "/");
  return a.metavar.empty() ? a.dest : a.metavar;
}

std::string spaces(int n) { return std::string(n > 0 ? n : 0, ' '); }

bool is_open(char c) { return c == '[' || c == '('; }
bool is_close(char c) { return c == ']' || c == ')'; }

// The cleanup at the end of `_format_actions_usage`, which joins the
// separators of a mutually exclusive group to their options:
// `re.sub(r'([\[(]) ', r'\1', text)`, then `re.sub(r' ([\])])', r'\1',
// text)`, then `re.sub(r'[\[(] *[\])]', '', text)`, then `strip()`.
std::string clean_group_separators(const std::string& text) {
  std::string a;
  for (std::size_t i = 0; i < text.size();) {
    a += text[i];
    i += is_open(text[i]) && i + 1 < text.size() && text[i + 1] == ' ' ? 2 : 1;
  }
  std::string b;
  for (std::size_t i = 0; i < a.size();) {
    if (a[i] == ' ' && i + 1 < a.size() && is_close(a[i + 1])) {
      b += a[i + 1];
      i += 2;
    } else {
      b += a[i++];
    }
  }
  std::string c;
  for (std::size_t i = 0; i < b.size();) {
    if (is_open(b[i])) {
      std::size_t j = i + 1;
      while (j < b.size() && b[j] == ' ') ++j;
      if (j < b.size() && is_close(b[j])) {
        i = j + 1;
        continue;
      }
    }
    c += b[i++];
  }
  const std::size_t first = c.find_first_not_of(" \t\n\r\f\v");
  if (first == std::string::npos) return "";
  const std::size_t last = c.find_last_not_of(" \t\n\r\f\v");
  return c.substr(first, last - first + 1);
}

}  // namespace

std::vector<std::string> wrap_text(const std::string& text, int width) {
  std::vector<std::string> chunks = split_chunks(text);
  std::reverse(chunks.begin(), chunks.end());
  std::vector<std::string> lines;
  while (!chunks.empty()) {
    std::vector<std::string> line;
    int cur_len = 0;
    if (all_space(chunks.back()) && !lines.empty()) chunks.pop_back();
    while (!chunks.empty()) {
      const int l = static_cast<int>(chunks.back().size());
      if (cur_len + l > width) break;
      line.push_back(chunks.back());
      chunks.pop_back();
      cur_len += l;
    }
    if (!chunks.empty() && static_cast<int>(chunks.back().size()) > width) {
      // `_handle_long_word`, which breaks the word after its last hyphen
      // that fits, when something other than hyphens comes before it.
      const int space_left = width < 1 ? 1 : width - cur_len;
      std::string& chunk = chunks.back();
      std::size_t end = static_cast<std::size_t>(space_left);
      if (chunk.size() > end) {
        const std::size_t hyphen = chunk.rfind('-', end == 0 ? 0 : end - 1);
        if (hyphen != std::string::npos && hyphen > 0 && hyphen < end &&
            chunk.find_first_not_of('-') < hyphen) {
          end = hyphen + 1;
        }
      }
      line.push_back(chunk.substr(0, end));
      chunk = chunk.substr(std::min(end, chunk.size()));
    }
    if (!line.empty() && all_space(line.back())) line.pop_back();
    if (!line.empty()) lines.push_back(join(line, ""));
  }
  return lines;
}

bool Namespace::flag(const std::string& dest) const {
  const auto it = flags.find(dest);
  return it != flags.end() && it->second;
}

const std::vector<std::string>* Namespace::list(const std::string& dest) const {
  const auto it = lists.find(dest);
  return it == lists.end() ? nullptr : &it->second;
}

const std::string* Namespace::value(const std::string& dest) const {
  const auto it = values.find(dest);
  return it == values.end() ? nullptr : &it->second;
}

Parser::Parser(std::string prog, std::string description, bool add_help_option)
    : prog_(std::move(prog)), description_(std::move(description)) {
  if (add_help_option) add_help({"-h", "--help"}, "show this help message and exit");
}

void Parser::add(Action a) {
  if (a.group >= 0) groups_.at(static_cast<std::size_t>(a.group)).push_back(actions_.size());
  actions_.push_back(std::move(a));
}

void Parser::add_store_true(std::vector<std::string> option_strings, std::string help,
                            std::string dest) {
  if (dest.empty()) {
    for (const auto& s : option_strings) {
      if (starts_with(s, "--")) {
        dest = s.substr(2);
        break;
      }
    }
    if (dest.empty()) dest = option_strings.front().substr(1);
    std::replace(dest.begin(), dest.end(), '-', '_');
  }
  add(Action{ActionKind::kStoreTrue, std::move(option_strings), dest, std::move(help)});
}

void Parser::add_store_const(std::vector<std::string> option_strings, std::string dest,
                             std::string constant, std::string help, int group) {
  Action a{ActionKind::kStoreConst, std::move(option_strings), std::move(dest), std::move(help)};
  a.constant = std::move(constant);
  a.group = group;
  add(std::move(a));
}

void Parser::add_store(std::vector<std::string> option_strings, std::string dest,
                       std::string help, std::string metavar, std::string type_name,
                       TypeCheck type) {
  Action a{ActionKind::kStore, std::move(option_strings), std::move(dest), std::move(help)};
  a.metavar = std::move(metavar);
  a.type_name = std::move(type_name);
  a.type = std::move(type);
  add(std::move(a));
}

void Parser::add_append(std::vector<std::string> option_strings, std::string dest,
                        std::string help, std::string metavar) {
  Action a{ActionKind::kAppend, std::move(option_strings), std::move(dest), std::move(help)};
  a.metavar = std::move(metavar);
  add(std::move(a));
}

void Parser::add_help(std::vector<std::string> option_strings, std::string help) {
  add(Action{ActionKind::kHelp, std::move(option_strings), "help", std::move(help)});
}

void Parser::add_positional(std::string dest, std::string help) {
  add(Action{ActionKind::kOneOrMore, {}, std::move(dest), std::move(help)});
}

int Parser::add_mutually_exclusive_group() {
  groups_.emplace_back();
  return static_cast<int>(groups_.size()) - 1;
}

const Action* Parser::find(const std::string& option_string) const {
  for (const auto& a : actions_) {
    for (const auto& s : a.option_strings) {
      if (s == option_string) return &a;
    }
  }
  return nullptr;
}

void Parser::error(const std::string& message, int columns) const {
  throw Exit{2, "", format_usage(columns) + prog_ + ": error: " + message + "\n"};
}

std::optional<Parser::Tuple> Parser::parse_optional(const std::string& arg, int columns) const {
  if (arg.empty() || arg[0] != '-') return std::nullopt;
  if (const Action* a = find(arg)) return Tuple{a, arg, std::nullopt};
  if (arg.size() == 1) return std::nullopt;
  if (const std::size_t eq = arg.find('='); eq != std::string::npos) {
    const std::string option_string = arg.substr(0, eq);
    if (const Action* a = find(option_string)) return Tuple{a, option_string, arg.substr(eq + 1)};
  }
  // `_get_option_tuples`, over the option strings in the order the
  // actions were added.
  std::vector<Tuple> tuples;
  if (arg[1] == '-') {
    std::string prefix = arg;
    std::optional<std::string> explicit_arg;
    if (const std::size_t eq = arg.find('='); eq != std::string::npos) {
      prefix = arg.substr(0, eq);
      explicit_arg = arg.substr(eq + 1);
    }
    for (const auto& a : actions_) {
      for (const auto& s : a.option_strings) {
        if (starts_with(s, prefix)) tuples.push_back(Tuple{&a, s, explicit_arg});
      }
    }
  } else {
    const std::string short_prefix = arg.substr(0, 2);
    for (const auto& a : actions_) {
      for (const auto& s : a.option_strings) {
        if (s == short_prefix) {
          tuples.push_back(Tuple{&a, s, arg.substr(2)});
        } else if (starts_with(s, arg)) {
          tuples.push_back(Tuple{&a, s, std::nullopt});
        }
      }
    }
  }
  if (tuples.size() > 1) {
    std::vector<std::string> names;
    for (const auto& t : tuples) names.push_back(t.option_string);
    error("ambiguous option: " + arg + " could match " + join(names, ", "), columns);
  }
  if (tuples.size() == 1) return tuples.front();
  // `^-\d+$|^-\d*\.\d+$`: a negative number is a positional argument,
  // since no option looks like one.
  {
    std::size_t k = 1;
    while (k < arg.size() && arg[k] >= '0' && arg[k] <= '9') ++k;
    const bool digits = k > 1;
    if (k == arg.size() && digits) return std::nullopt;
    if (k < arg.size() && arg[k] == '.') {
      std::size_t m = k + 1;
      while (m < arg.size() && arg[m] >= '0' && arg[m] <= '9') ++m;
      if (m == arg.size() && m > k + 1) return std::nullopt;
    }
  }
  if (arg.find(' ') != std::string::npos) return std::nullopt;
  return Tuple{nullptr, arg, std::nullopt};
}

Namespace Parser::parse_args(const std::vector<std::string>& args, int columns) const {
  std::vector<std::string> extras;
  Namespace ns = parse_known_args(args, columns, extras);
  if (!extras.empty()) error("unrecognized arguments: " + join(extras, " "), columns);
  return ns;
}

Namespace Parser::parse_known_args(const std::vector<std::string>& args, int columns,
                                   std::vector<std::string>& extras) const {
  // The pattern of `O` for an option, `A` for an argument, and `-` for
  // the `--` after which everything is an argument.
  std::string pattern;
  std::map<std::size_t, Tuple> options;
  for (std::size_t i = 0; i < args.size(); ++i) {
    if (args[i] == "--") {
      pattern += '-';
      pattern += std::string(args.size() - i - 1, 'A');
      break;
    }
    if (auto t = parse_optional(args[i], columns)) {
      options.emplace(i, *t);
      pattern += 'O';
    } else {
      pattern += 'A';
    }
  }
  Namespace ns;
  for (const auto& a : actions_) {
    if (a.kind == ActionKind::kStoreTrue) ns.flags[a.dest] = false;
  }
  std::vector<const Action*> seen;
  // Every action that takes an argument here stores something other
  // than its default object, so `seen_non_default_actions` holds each
  // action taken, which the conflicts of a mutually exclusive group are
  // found in.
  const auto argument_error = [&](const Action& a, const std::string& message) {
    error("argument " + action_name(a) + ": " + message, columns);
  };
  const auto take_action = [&](const Action& a, std::vector<std::string> values) {
    // `_get_values`: `--` is dropped, and a value goes through the
    // `type=` function before the conflicts are looked for.
    if (a.kind == ActionKind::kOneOrMore) {
      const auto dashes = std::find(values.begin(), values.end(), "--");
      if (dashes != values.end()) values.erase(dashes);
    }
    if (a.type && !values.empty() && !a.type(values.front())) {
      argument_error(a, "invalid " + a.type_name + " value: " + pyrepr::string(values.front()));
    }
    seen.push_back(&a);
    if (a.group >= 0) {
      for (const std::size_t k : groups_.at(static_cast<std::size_t>(a.group))) {
        const Action* other = &actions_[k];
        if (other != &a && std::find(seen.begin(), seen.end(), other) != seen.end()) {
          argument_error(a, "not allowed with argument " + action_name(*other));
        }
      }
    }
    switch (a.kind) {
      case ActionKind::kStoreTrue:
        ns.flags[a.dest] = true;
        break;
      case ActionKind::kStoreConst:
        ns.values[a.dest] = a.constant;
        break;
      case ActionKind::kStore:
        ns.values[a.dest] = values.front();
        break;
      case ActionKind::kAppend:
        ns.lists[a.dest].push_back(values.front());
        break;
      case ActionKind::kHelp:
        throw Exit{0, format_help(columns), ""};
      case ActionKind::kOneOrMore:
        ns.lists[a.dest] = std::move(values);
        break;
    }
  };
  const auto consume_optional = [&](std::size_t start) -> std::size_t {
    Tuple t = options.at(start);
    std::vector<std::pair<const Action*, std::vector<std::string>>> tuples;
    std::size_t stop = start + 1;
    while (true) {
      if (t.action == nullptr) {
        extras.push_back(args[start]);
        return start + 1;
      }
      const bool no_argument = takes_no_argument(*t.action);
      if (t.explicit_arg) {
        if (no_argument && t.option_string[1] != '-' && !t.explicit_arg->empty()) {
          // `-ag` is `-a -g`.
          tuples.push_back({t.action, {}});
          const std::string next = std::string(1, t.option_string[0]) + (*t.explicit_arg)[0];
          std::optional<std::string> rest;
          if (t.explicit_arg->size() > 1) rest = t.explicit_arg->substr(1);
          if (const Action* a = find(next)) {
            t = Tuple{a, next, rest};
          } else {
            argument_error(*t.action,
                           "ignored explicit argument " + pyrepr::string(*t.explicit_arg));
          }
        } else if (!no_argument) {
          tuples.push_back({t.action, {*t.explicit_arg}});
          stop = start + 1;
          break;
        } else {
          argument_error(*t.action,
                         "ignored explicit argument " + pyrepr::string(*t.explicit_arg));
        }
      } else {
        const std::size_t first = start + 1;
        std::size_t count = 0;
        if (!no_argument) {
          if (first >= pattern.size() || pattern[first] != 'A') {
            argument_error(*t.action, "expected one argument");
          }
          count = 1;
        }
        stop = first + count;
        tuples.push_back({t.action, std::vector<std::string>(args.begin() + first,
                                                             args.begin() + stop)});
        break;
      }
    }
    for (auto& [a, values] : tuples) take_action(*a, std::move(values));
    return stop;
  };
  std::vector<const Action*> positionals;
  for (const auto& a : actions_) {
    if (a.option_strings.empty()) positionals.push_back(&a);
  }
  const auto consume_positionals = [&](std::size_t start) -> std::size_t {
    // `-*A[A-]*` for the one positional of one or more values.
    if (positionals.empty()) return start;
    std::size_t k = start;
    while (k < pattern.size() && pattern[k] == '-') ++k;
    if (k >= pattern.size() || pattern[k] != 'A') return start;
    ++k;
    while (k < pattern.size() && (pattern[k] == 'A' || pattern[k] == '-')) ++k;
    const Action* a = positionals.front();
    positionals.erase(positionals.begin());
    take_action(*a, std::vector<std::string>(args.begin() + start, args.begin() + k));
    return k;
  };
  std::size_t start = 0;
  const std::size_t last_option = options.empty() ? 0 : options.rbegin()->first;
  while (!options.empty() && start <= last_option) {
    const std::size_t next_option = options.lower_bound(start)->first;
    if (start != next_option) {
      const std::size_t end = consume_positionals(start);
      if (end > start) {
        start = end;
        continue;
      }
      start = end;
    }
    if (!options.contains(start)) {
      extras.insert(extras.end(), args.begin() + start, args.begin() + next_option);
      start = next_option;
    }
    start = consume_optional(start);
  }
  const std::size_t stop = consume_positionals(start);
  extras.insert(extras.end(), args.begin() + stop, args.end());
  std::vector<std::string> required;
  for (const auto& a : actions_) {
    if (a.kind == ActionKind::kOneOrMore && std::find(seen.begin(), seen.end(), &a) == seen.end()) {
      required.push_back(action_name(a));
    }
  }
  if (!required.empty()) {
    error("the following arguments are required: " + join(required, ", "), columns);
  }
  return ns;
}

std::string Parser::actions_usage(bool optionals, bool positionals) const {
  // `_format_actions_usage` of the optionals, the positionals, or both
  // (the optionals first, whatever their order).
  std::vector<const Action*> actions;
  if (optionals) {
    for (const auto& a : actions_) {
      if (!a.option_strings.empty()) actions.push_back(&a);
    }
  }
  if (positionals) {
    for (const auto& a : actions_) {
      if (a.option_strings.empty()) actions.push_back(&a);
    }
  }
  // A group whose actions come together is bracketed, with `|` between
  // its actions, which then lose their own brackets.
  std::map<std::size_t, std::string> inserts;
  std::vector<bool> in_group(actions.size(), false);
  for (const auto& group : groups_) {
    if (group.empty()) continue;
    const auto first = std::find(actions.begin(), actions.end(), &actions_[group.front()]);
    if (first == actions.end()) continue;
    const std::size_t begin = static_cast<std::size_t>(first - actions.begin());
    const std::size_t end = begin + group.size();
    bool together = end <= actions.size();
    for (std::size_t k = 0; together && k < group.size(); ++k) {
      together = actions[begin + k] == &actions_[group[k]];
    }
    if (!together) continue;
    for (std::size_t k = begin; k < end; ++k) in_group[k] = true;
    const bool taken = inserts.count(begin) != 0;
    inserts[begin] += taken ? " [" : "[";
    inserts[end] += "]";
    for (std::size_t k = begin + 1; k < end; ++k) inserts[k] = "|";
  }
  std::vector<std::string> parts;
  for (std::size_t k = 0; k < actions.size(); ++k) {
    const Action& a = *actions[k];
    std::string part;
    if (a.option_strings.empty()) {
      part = format_args(a);
    } else if (takes_no_argument(a)) {
      part = a.option_strings.front();
    } else {
      part = a.option_strings.front() + " " + format_args(a);
    }
    if (!a.option_strings.empty() && !in_group[k]) part = "[" + part + "]";
    parts.push_back(part);
  }
  for (auto it = inserts.rbegin(); it != inserts.rend(); ++it) {
    parts.insert(parts.begin() + static_cast<std::ptrdiff_t>(it->first), it->second);
  }
  return clean_group_separators(join(parts, " "));
}

std::string Parser::format_usage(int columns) const {
  const int width = columns - 2;
  const std::string prefix = "usage: ";
  const std::string& prog = prog_;
  const std::string action_usage = actions_usage(true, true);
  std::string usage = action_usage.empty() ? prog : prog + " " + action_usage;
  const int text_width = width;
  if (static_cast<int>(prefix.size() + usage.size()) > text_width) {
    const std::vector<std::string> opt_parts = usage_parts(actions_usage(true, false));
    const std::vector<std::string> pos_parts = usage_parts(actions_usage(false, true));
    const auto get_lines = [&](const std::vector<std::string>& parts, const std::string& indent,
                               const std::string* first_prefix) {
      std::vector<std::string> lines;
      std::vector<std::string> line;
      int line_len = static_cast<int>((first_prefix ? first_prefix->size() : indent.size())) - 1;
      for (const auto& part : parts) {
        if (line_len + 1 + static_cast<int>(part.size()) > text_width && !line.empty()) {
          lines.push_back(indent + join(line, " "));
          line.clear();
          line_len = static_cast<int>(indent.size()) - 1;
        }
        line.push_back(part);
        line_len += static_cast<int>(part.size()) + 1;
      }
      if (!line.empty()) lines.push_back(indent + join(line, " "));
      if (first_prefix != nullptr) lines[0] = lines[0].substr(indent.size());
      return lines;
    };
    std::vector<std::string> lines;
    // `len(prefix) + len(prog) <= 0.75 * text_width`, in integers.
    if (4 * static_cast<long>(prefix.size() + prog.size()) <= 3L * text_width) {
      const std::string indent = spaces(static_cast<int>(prefix.size() + prog.size()) + 1);
      std::vector<std::string> first{prog};
      if (!opt_parts.empty()) {
        first.insert(first.end(), opt_parts.begin(), opt_parts.end());
        lines = get_lines(first, indent, &prefix);
        const auto more = get_lines(pos_parts, indent, nullptr);
        lines.insert(lines.end(), more.begin(), more.end());
      } else if (!pos_parts.empty()) {
        first.insert(first.end(), pos_parts.begin(), pos_parts.end());
        lines = get_lines(first, indent, &prefix);
      } else {
        lines = {prog};
      }
    } else {
      const std::string indent = spaces(static_cast<int>(prefix.size()));
      std::vector<std::string> parts = opt_parts;
      parts.insert(parts.end(), pos_parts.begin(), pos_parts.end());
      lines = get_lines(parts, indent, nullptr);
      if (lines.size() > 1) {
        lines = get_lines(opt_parts, indent, nullptr);
        const auto more = get_lines(pos_parts, indent, nullptr);
        lines.insert(lines.end(), more.begin(), more.end());
      }
      lines.insert(lines.begin(), prog);
    }
    usage = join(lines, "\n");
  }
  return prefix + usage + "\n";
}

std::string Parser::format_action(const Action& a, int width, int max_help_position,
                                  int action_max_length) const {
  const int indent = 2;
  const int help_position = std::min(action_max_length + 2, max_help_position);
  const int help_width = std::max(width - help_position, 11);
  const int action_width = help_position - indent - 2;
  std::string header = invocation(a);
  int indent_first = 0;
  std::string out;
  if (a.help.empty()) {
    out = spaces(indent) + header + "\n";
  } else if (static_cast<int>(header.size()) <= action_width) {
    header.resize(std::max<std::size_t>(header.size(), action_width), ' ');
    out = spaces(indent) + header + "  ";
  } else {
    out = spaces(indent) + header + "\n";
    indent_first = help_position;
  }
  if (!a.help.empty()) {
    const std::vector<std::string> lines = wrap_text(normalize(a.help), help_width);
    out += spaces(indent_first) + lines.front() + "\n";
    for (std::size_t k = 1; k < lines.size(); ++k) out += spaces(help_position) + lines[k] + "\n";
  }
  return out;
}

std::string Parser::format_help(int columns) const {
  const int width = columns - 2;
  const int max_help_position = std::min(24, std::max(width - 20, 4));
  int action_max_length = 0;
  for (const auto& a : actions_) {
    action_max_length = std::max(action_max_length, static_cast<int>(invocation(a).size()) + 2);
  }
  std::string help = format_usage(columns) + "\n";
  if (!description_.empty()) {
    help += join(wrap_text(normalize(description_), std::max(width, 11)), "\n") + "\n\n";
  }
  // The groups: the positionals, then the options.
  for (const bool positional : {true, false}) {
    std::string items;
    for (const auto& a : actions_) {
      if (a.option_strings.empty() == positional) {
        items += format_action(a, width, max_help_position, action_max_length);
      }
    }
    if (items.empty()) continue;
    help += std::string("\n") + (positional ? "positional arguments" : "options") + ":\n" + items +
            "\n";
  }
  // `_long_break_matcher.sub('\n\n', help)`, then `strip('\n') + '\n'`.
  std::string squeezed;
  for (std::size_t i = 0; i < help.size();) {
    if (help[i] == '\n') {
      std::size_t k = i;
      while (k < help.size() && help[k] == '\n') ++k;
      squeezed.append(std::min<std::size_t>(k - i, 2), '\n');
      i = k;
    } else {
      squeezed += help[i++];
    }
  }
  const std::size_t first = squeezed.find_first_not_of('\n');
  const std::size_t last = squeezed.find_last_not_of('\n');
  if (first == std::string::npos) return "\n";
  return squeezed.substr(first, last - first + 1) + "\n";
}

int terminal_columns() {
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4996)
#endif
  const char* env = std::getenv("COLUMNS");
#ifdef _MSC_VER
#pragma warning(pop)
#endif
  if (env != nullptr) {
    // `int(os.environ['COLUMNS'])`, with spaces around the digits. Python
    // also takes an underscore between digits, which this reads as no
    // number. A value it can't read counts as none.
    std::string s(env);
    const std::size_t first = s.find_first_not_of(" \t\n\r\f\v");
    const std::size_t last = s.find_last_not_of(" \t\n\r\f\v");
    s = first == std::string::npos ? "" : s.substr(first, last - first + 1);
    char* end = nullptr;
    const long v = std::strtol(s.c_str(), &end, 10);
    if (!s.empty() && *end == '\0' && v > 0) return static_cast<int>(v);
  }
#ifdef _WIN32
  CONSOLE_SCREEN_BUFFER_INFO info;
  if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info)) {
    return info.srWindow.Right - info.srWindow.Left + 1;
  }
#else
  winsize ws{};
  if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == 0 && ws.ws_col > 0) return ws.ws_col;
#endif
  return 80;
}

std::string path_str(const std::string& path) {
#ifdef _WIN32
  std::string s = path;
  std::replace(s.begin(), s.end(), '/', '\\');
  std::string drive;
  std::string rest = s;
  if (s.size() >= 2 && s[1] == ':' &&
      ((s[0] >= 'A' && s[0] <= 'Z') || (s[0] >= 'a' && s[0] <= 'z'))) {
    drive = s.substr(0, 2);
    rest = s.substr(2);
  }
  const bool root = !rest.empty() && rest[0] == '\\';
  const char sep = '\\';
#else
  const std::string drive;
  const std::string& rest = path;
  const bool root = !rest.empty() && rest[0] == '/';
  const char sep = '/';
#endif
  std::vector<std::string> parts;
  std::size_t i = 0;
  while (i <= rest.size()) {
    std::size_t end = rest.find(sep, i);
    if (end == std::string::npos) end = rest.size();
    const std::string part = rest.substr(i, end - i);
    if (!part.empty() && part != ".") parts.push_back(part);
    i = end + 1;
  }
  std::string out = drive;
  if (root) {
    out += sep;
#ifndef _WIN32
    // POSIX keeps exactly two leading slashes.
    if (rest.size() >= 2 && rest[1] == '/' && (rest.size() == 2 || rest[2] != '/')) out += sep;
#endif
  }
  out += join(parts, std::string(1, sep));
  return out.empty() ? "." : out;
}

std::string path_name(const std::string& path) {
#ifdef _WIN32
  std::string rest = path;
  if (rest.size() >= 2 && rest[1] == ':' &&
      ((rest[0] >= 'A' && rest[0] <= 'Z') || (rest[0] >= 'a' && rest[0] <= 'z'))) {
    rest = rest.substr(2);
  }
  const auto is_sep = [](char c) { return c == '/' || c == '\\'; };
#else
  const std::string& rest = path;
  const auto is_sep = [](char c) { return c == '/'; };
#endif
  std::string last;
  std::size_t i = 0;
  while (i <= rest.size()) {
    std::size_t end = i;
    while (end < rest.size() && !is_sep(rest[end])) ++end;
    const std::string part = rest.substr(i, end - i);
    if (!part.empty() && part != ".") last = part;
    i = end + 1;
  }
  return last;
}

}  // namespace c6502::argparse
