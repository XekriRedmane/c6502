// Follows the Y register through the emitted text.
//
// The emitter writes an `LDY #<off>` for every access through an
// indirect-Y addressing mode (a Frame, Stack, or Indirect operand). Two
// accesses in a row with the same offset give two identical `LDY`s, of
// which the second does nothing, and two with adjacent offsets give two
// `LDY`s where the second could be a shorter `INY` or `DEY`.
//
// This pass walks the lines of the text, follows what Y is known to
// hold, and rewrites
//
//   * `LDY #<imm>` when Y is known to equal `imm`: the line goes.
//   * `LDY #<imm>` when Y is known to equal `(imm - 1) & 0xFF`: `INY`,
//     a byte shorter.
//   * `LDY #<imm>` when Y is known to equal `(imm + 1) & 0xFF`: `DEY`.
//
// Y becomes unknown at a label, since a label can be reached from any
// predecessor, and at a `JSR`, since the callee may change Y (the
// runtime helpers do), and at `TAY`. `INY`, `DEY`, `TAY`, `LDY`, `JSR`,
// and labels are the only lines that matter. `PHP`, `PLP`, and `PLA`
// leave Y alone.
//
// Branches and jumps don't change Y, so the state carries on past one
// to the next line. A branch's target is reached through its own label,
// where Y is unknown again, so the target never sees the branch's
// state.
//
// The pass works on the text alone, and reads each line only far enough
// to recognize an `LDY`, `INY`, `DEY`, `TAY`, `JSR`, or label. A line it
// doesn't recognize is taken to leave Y alone, since most 6502
// instructions do.
#include "passes/y_peephole.h"

#include <string_view>
#include <utility>

namespace c6502::passes {

namespace {

enum class LineKind { kBlank, kComment, kLabel, kDirective, kLdyImm, kIny, kDey, kYClobber, kOther };

// Whether `c` is a character Python's `str.split()` and `str.strip()`
// take as whitespace, among those a line of ASCII can hold.
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

// Splits `stripped` (with no whitespace at either end) at its first run
// of whitespace, as Python's `split(None, 1)` does.
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

// Returns what `line` does to Y. Only the lines that matter are
// recognized, and everything else is `kOther`, which leaves Y alone.
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
  // An instruction line: whitespace, an opcode, and maybe an operand.
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
    // `LDY <addr>` reads memory into Y, and `LDY #<name` loads a byte of
    // an address known only when the program is linked. Either way the
    // value isn't known here, which is what a write of Y gives.
    return LineKind::kYClobber;
  }
  if (opcode == "INY") {
    return LineKind::kIny;
  }
  if (opcode == "DEY") {
    return LineKind::kDey;
  }
  // `JSR` is the one that matters most, since a callee may use Y, and
  // `TAY` overwrites Y from A.
  if (opcode == "JSR" || opcode == "TAY") {
    return LineKind::kYClobber;
  }
  return LineKind::kOther;
}

// Returns the immediate of `LDY #$XX` (hexadecimal) or `LDY #NN`
// (decimal), for a line `classify` found to be `kLdyImm`.
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
  // The state is local to a block: every label makes Y unknown, and so
  // does a call. Within a block the rules are mechanical.
  std::vector<std::string> out;
  out.reserve(lines.size());
  // What Y holds, when `known`.
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
          // Y already holds the value.
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
