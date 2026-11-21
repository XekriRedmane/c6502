#include "capture.h"

#include <fstream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string_view>
#include <utility>

#include "json.h"

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
  // Version 2 added the context line.
  const std::string version = take_line(text, pos, "c6502-capture ");
  if (version != "1" && version != "2") {
    throw std::runtime_error("unknown capture file version");
  }
  call.pass = take_line(text, pos, "pass ");
  call.source = take_line(text, pos, "source ");
  call.args_json = take_line(text, pos, "args ");
  if (version == "2") {
    call.context_json = take_line(text, pos, "context ");
  }
  if (text.compare(pos, 8, "mutated ") == 0) {
    call.mutated_json = take_line(text, pos, "mutated ");
  }
  if (text.compare(pos, 9, "warnings ") == 0) {
    call.warnings_json = take_line(text, pos, "warnings ");
  }
  if (text.compare(pos, 7, "stderr ") == 0) {
    call.stderr_json = take_line(text, pos, "stderr ");
  }
  // `input-json` holds the tagged JSON of a first argument that isn't an
  // IR value, on one line.
  const std::string input_kind = take_line(text, pos, "input");
  if (input_kind == "-json") {
    call.input_is_json = true;
  } else if (!input_kind.empty()) {
    throw std::runtime_error("expected an input line");
  }
  // The input's dump ends with a newline, and the `output` line follows
  // it. No line of a dump can be `output`, since every line of one
  // starts with a name, a bracket, or indentation, followed by more.
  // The output section is the first of the three kinds of output line.
  struct Marker {
    std::string_view line;
    bool is_json;
    bool is_error;
  };
  static constexpr Marker kMarkers[] = {
      {"\noutput\n", false, false},
      {"\noutput-json\n", true, false},
      {"\noutput-error\n", false, true},
  };
  std::size_t mark = std::string::npos;
  const Marker* found = nullptr;
  for (const Marker& m : kMarkers) {
    const std::size_t at = text.find(m.line, pos - 1);
    if (at != std::string::npos && (mark == std::string::npos || at < mark)) {
      mark = at;
      found = &m;
    }
  }
  if (found == nullptr) {
    throw std::runtime_error("no output section");
  }
  call.output_is_json = found->is_json;
  call.output_is_error = found->is_error;
  call.output = text.substr(mark + found->line.size());
  call.input = text.substr(pos, mark + 1 - pos);
  return call;
}

passes::AsmContext context(const Call& call) {
  if (call.context_json.empty()) {
    return passes::AsmContext{};
  }
  return context_from_json(json::parse(call.context_json));
}

passes::AsmContext context_from_json(const json::Value& v) {
  passes::AsmContext ctx;
  const json::Value& layout = *v.get("layout");
  ctx.layout = passes::RuntimeLayout{
      .ssp = layout.items.at(0).integer,
      .fp = layout.items.at(1).integer,
      .hargs = layout.items.at(2).integer,
      .hargs_size = layout.items.at(3).integer,
      .dptr = layout.items.at(4).integer,
  };
  auto user = std::make_shared<passes::UserZpGlobals>();
  for (const auto& pair : v.get("user_zp_globals")->items) {
    user->addrs[pair.items.at(0).string] = pair.items.at(1).integer;
  }
  for (const auto& b : v.get("user_zp_bytes")->items) {
    user->bytes.insert(b.integer);
  }
  ctx.user_zp = std::move(user);
  return ctx;
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
