#include "passes/y_peephole.h"

#include <string_view>
#include <utility>

namespace c6502::passes {

namespace {

enum class LineKind { kBlank, kComment, kLabel, kDirective, kLdyImm, kIny, kDey, kYClobber, kOther };

bool is_space(char c) {
  return c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\v' || c == '\f' ||
         (c >= '\x1c' && c <= '\x1f');
}

std::string_view strip(std::string_view s) {
  while (!s.empty() && is_space(s.front())) {
    s.remove_prefix(1);
  }
  while (!s.empty() && is_space(s.back())) {
    s.remove_suffix(1);
  }
  return s;
}

std::pair<std::string_view, std::string_view> split_first(std::string_view stripped) {
  std::size_t i = 0;
  while (i < stripped.size() && !is_space(stripped[i])) {
    ++i;
  }
  std::string_view head = stripped.substr(0, i);
  std::string_view rest = stripped.substr(i);
  while (!rest.empty() && is_space(rest.front())) {
    rest.remove_prefix(1);
  }
  return {head, rest};
}

std::string upper(std::string_view s) {
  std::string out(s);
  for (char& c : out) {
    if (c >= 'a' && c <= 'z') {
      c = static_cast<char>(c - 'a' + 'A');
    }
  }
  return out;
}

bool is_directive(std::string_view op) {
  for (std::string_view d : {"SUBROUTINE", "DC.B", "DC.W", "DC.L", "DS.B", "DS.W", "DS.L", "EQU",
                             "ORG", "PROCESSOR"}) {
    if (op == d) {
      return true;
    }
  }
  return false;
}

LineKind classify(std::string_view line) {
  if (line.empty()) {
    return LineKind::kBlank;
  }
  const std::string_view stripped = strip(line);
  if (stripped.empty()) {
    return LineKind::kBlank;
  }
  if (stripped.front() == ';') {
    return LineKind::kComment;
  }
  if (stripped.back() == ':') {
    return LineKind::kLabel;
  }
  const auto [head, rest] = split_first(stripped);
  const std::string opcode = upper(head);
  if (is_directive(opcode)) {
    return LineKind::kDirective;
  }
  if (opcode == "LDY") {
    if (!rest.empty() && rest.front() == '#' && rest.substr(0, 2) != "#<" &&
        rest.substr(0, 2) != "#>") {
      return LineKind::kLdyImm;
    }
    return LineKind::kYClobber;
  }
  if (opcode == "INY") {
    return LineKind::kIny;
  }
  if (opcode == "DEY") {
    return LineKind::kDey;
  }
  if (opcode == "JSR" || opcode == "TAY") {
    return LineKind::kYClobber;
  }
  return LineKind::kOther;
}

int ldy_imm_value(std::string_view line) {
  const std::string_view operand = strip(split_first(strip(line)).second);
  std::string_view body = operand.substr(1);
  int base = 10;
  if (!body.empty() && body.front() == '$') {
    body.remove_prefix(1);
    base = 16;
  }
  return std::stoi(std::string(body), nullptr, base);
}

}  // namespace

std::vector<std::string> apply_y_peephole(const std::vector<std::string>& lines) {
  std::vector<std::string> out;
  out.reserve(lines.size());
  bool known = false;
  int y = 0;
  for (const std::string& line : lines) {
    switch (classify(line)) {
      case LineKind::kLabel:
        known = false;
        out.push_back(line);
        break;
      case LineKind::kBlank:
      case LineKind::kComment:
      case LineKind::kDirective:
      case LineKind::kOther:
        out.push_back(line);
        break;
      case LineKind::kLdyImm: {
        const int imm = ldy_imm_value(line);
        if (known && imm == y) {
          break;
        }
        if (known && imm == ((y + 1) & 0xFF)) {
          out.emplace_back("   INY");
          y = imm;
          break;
        }
        if (known && imm == ((y - 1) & 0xFF)) {
          out.emplace_back("   DEY");
          y = imm;
          break;
        }
        out.push_back(line);
        y = imm;
        known = true;
        break;
      }
      case LineKind::kIny:
        out.push_back(line);
        if (known) {
          y = (y + 1) & 0xFF;
        }
        break;
      case LineKind::kDey:
        out.push_back(line);
        if (known) {
          y = (y - 1) & 0xFF;
        }
        break;
      case LineKind::kYClobber:
        out.push_back(line);
        known = false;
        break;
    }
  }
  return out;
}

}  // namespace c6502::passes
