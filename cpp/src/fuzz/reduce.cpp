#include "fuzz/reduce.h"

#include <utility>
#include <vector>

#include "diagnostics.h"
#include "errors.h"
#include "passes/ir_check.h"
#include "tac_sim_modes.h"

namespace c6502::fuzz {

namespace {

passes::FrontEndOptions front_options(const Runner& runner) {
  passes::FrontEndOptions o;
  o.include_dir = runner.include_dir();
  return o;
}

std::vector<std::string> split_lines(const std::string& text) {
  std::vector<std::string> out;
  std::string line;
  for (std::size_t i = 0; i < text.size(); ++i) {
    const char c = text[i];
    std::size_t skip = 0;
    if (c == '\r' && i + 1 < text.size() && text[i + 1] == '\n') {
      skip = 1;
    } else if (c == '\n' || c == '\r' || c == '\v' || c == '\f' || c == '\x1c' || c == '\x1d' ||
               c == '\x1e') {
    } else if (text.compare(i, 2, "\xC2\x85") == 0) {
      skip = 1;
    } else if (text.compare(i, 3, "\xE2\x80\xA8") == 0 || text.compare(i, 3, "\xE2\x80\xA9") == 0) {
      skip = 2;
    } else {
      line += c;
      continue;
    }
    out.push_back(std::move(line));
    line.clear();
    i += skip;
  }
  if (!line.empty()) out.push_back(std::move(line));
  return out;
}

std::string join_lines(const std::vector<std::string>& lines) {
  std::string out;
  for (std::size_t i = 0; i < lines.size(); ++i) {
    if (i != 0) out += '\n';
    out += lines[i];
  }
  return out + "\n";
}

struct Binary {
  std::size_t start;
  std::size_t end;
  std::string left;
  std::string right;
};

bool is_paren(char c) { return c == '(' || c == ')'; }

bool is_space(char c) { return c == ' ' || (c >= '\t' && c <= '\r') || (c >= '\x1c' && c <= '\x1f'); }

std::optional<Binary> match_binary(const std::string& s, std::size_t pos) {
  std::size_t i = pos;
  auto literal = [&](std::string_view lit) {
    if (s.compare(i, lit.size(), lit) != 0) return false;
    i += lit.size();
    return true;
  };
  auto run = [&](bool (*take)(char)) {
    const std::size_t from = i;
    while (i < s.size() && take(s[i])) ++i;
    return s.substr(from, i - from);
  };
  if (!literal("((")) return std::nullopt;
  std::string left = run([](char c) { return !is_paren(c); });
  if (!literal(") ")) return std::nullopt;
  if (run([](char c) { return !is_paren(c) && !is_space(c); }).empty()) return std::nullopt;
  if (!literal(" (")) return std::nullopt;
  std::string right = run([](char c) { return !is_paren(c); });
  if (!literal("))")) return std::nullopt;
  return Binary{pos, i, std::move(left), std::move(right)};
}

std::vector<Binary> binaries(const std::string& s) {
  std::vector<Binary> out;
  for (std::size_t pos = 0; pos < s.size();) {
    if (std::optional<Binary> m = match_binary(s, pos)) {
      pos = m->end;
      out.push_back(std::move(*m));
    } else {
      ++pos;
    }
  }
  return out;
}

}  // namespace

bool falls_off_end(const Runner& runner, const std::string& src) {
  const passes::ir_check::Enabled checks(true);
  std::string err;
  std::vector<diagnostics::CompileWarning> warnings;
  diagnostics::Collecting block(warnings);
  return tac_sim_modes::falls_off_end(src, front_options(runner), err);
}

std::optional<TacResults> tac_results(const Runner& runner, const std::string& src) {
  const passes::ir_check::Enabled checks(true);
  std::string err;
  std::vector<diagnostics::CompileWarning> warnings;
  try {
    tac_sim_modes::Translated t;
    {
      diagnostics::Collecting block(warnings);
      t = tac_sim_modes::translate(src, front_options(runner), err);
    }
    const tac_sim_modes::PlainAndOptimized r = tac_sim_modes::run_plain_and_optimized(std::move(t));
    if (r.plain.error_type || !r.optimized || r.optimized->error_type) return std::nullopt;
    return TacResults{r.plain.value, r.optimized->value};
  } catch (const CompileError&) {
    return std::nullopt;
  }
}

Predicate make_predicate(const Runner& runner, const std::string& src, Failure failure) {
  if (failure == Failure::kTacOptimizer) {
    return [&runner](const std::string& cand) {
      if (falls_off_end(runner, cand)) return false;
      const std::optional<TacResults> r = tac_results(runner, cand);
      return r && r->plain != r->optimized;
    };
  }
  const std::optional<std::int64_t> expected = expected_of(src);
  auto signature = [expected](const Results& res) {
    std::vector<bool> errors;
    for (Mode m : kModes) errors.push_back(is_error(res[m]));
    return std::pair{disagreements(res, expected), errors};
  };
  const auto original = signature(runner.run_all(src));
  return [&runner, signature, original](const std::string& cand) {
    try {
      if (falls_off_end(runner, cand)) return false;
    } catch (const InternalError&) {
      // The failure may be this compiler bug, which run_all reports as
      // the result of the tac path.
    }
    return signature(runner.run_all(cand)) == original;
  };
}

std::string reduce(const std::string& src, const Predicate& still_fails) {
  std::vector<std::string> lines = split_lines(src);
  auto ok = [&](const std::vector<std::string>& cand) { return still_fails(join_lines(cand)); };
  bool changed = true;
  while (changed) {
    changed = false;
    for (std::size_t i = 0; i < lines.size();) {
      if (lines[i].find("expect:") != std::string::npos) {
        ++i;
        continue;
      }
      std::vector<std::string> cand = lines;
      cand.erase(cand.begin() + static_cast<std::ptrdiff_t>(i));
      if (ok(cand)) {
        lines = std::move(cand);
        changed = true;
      } else {
        ++i;
      }
    }
    for (std::size_t i = 0; i < lines.size(); ++i) {
      const std::vector<Binary> matches = binaries(lines[i]);
      for (auto m = matches.rbegin(); m != matches.rend(); ++m) {
        const std::string line = lines[i];
        for (const std::string* operand : {&m->left, &m->right}) {
          std::vector<std::string> cand = lines;
          cand[i] = line.substr(0, m->start) + "(" + *operand + ")" + line.substr(m->end);
          if (ok(cand)) {
            lines[i] = cand[i];
            changed = true;
            break;
          }
        }
      }
    }
  }
  return join_lines(lines);
}

}  // namespace c6502::fuzz
