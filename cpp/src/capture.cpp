#include "capture.h"

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string_view>

namespace c6502::capture {

namespace {

// Reads the line at `pos`, which must start with `prefix`, and returns
// the rest of it, moving `pos` past its newline.
std::string take_line(const std::string& text, std::size_t& pos,
                      std::string_view prefix) {
  const std::size_t end = text.find('\n', pos);
  if (end == std::string::npos ||
      text.compare(pos, prefix.size(), prefix) != 0) {
    throw std::runtime_error("expected a line starting with '" +
                             std::string(prefix) + "'");
  }
  std::string rest = text.substr(pos + prefix.size(), end - pos - prefix.size());
  pos = end + 1;
  return rest;
}

}  // namespace

Call parse(const std::string& text) {
  Call call;
  std::size_t pos = 0;
  if (take_line(text, pos, "c6502-capture ") != "1") {
    throw std::runtime_error("unknown capture file version");
  }
  call.pass = take_line(text, pos, "pass ");
  call.source = take_line(text, pos, "source ");
  call.args_json = take_line(text, pos, "args ");
  take_line(text, pos, "input");
  // The input's dump ends with a newline, and the `output` line follows
  // it. No line of a dump can be `output`, since every line of one
  // starts with a name, a bracket, or indentation, followed by more.
  const std::size_t plain = text.find("\noutput\n", pos - 1);
  const std::size_t json = text.find("\noutput-json\n", pos - 1);
  std::size_t mark;
  if (plain != std::string::npos && (json == std::string::npos || plain < json)) {
    mark = plain;
    call.output = text.substr(plain + 8);
  } else if (json != std::string::npos) {
    mark = json;
    call.output_is_json = true;
    call.output = text.substr(json + 13);
  } else {
    throw std::runtime_error("no output section");
  }
  call.input = text.substr(pos, mark + 1 - pos);
  return call;
}

std::string read_file(const std::filesystem::path& path) {
  std::ifstream in(path, std::ios::binary);
  if (!in) {
    throw std::runtime_error("can't read " + path.string());
  }
  std::ostringstream ss;
  ss << in.rdbuf();
  return ss.str();
}

}  // namespace c6502::capture
