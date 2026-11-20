#include "backend.h"

#include <stdexcept>
#include <string_view>
#include <utility>

#include "asdl/runtime.h"
#include "asm_ast.h"
#include "capture.h"
#include "json.h"
#include "passes/capture_args.h"
#include "passes/optimized_asm.h"
#include "passes/registry_control_flow.h"

namespace c6502::backend {

namespace {

namespace ca = passes::capture_args;

// Reads the lines of a request in order.
class Lines {
 public:
  explicit Lines(const std::string& text) : text_(text) {}

  // Returns the next line, without its LF.
  std::string_view next() {
    const std::size_t end = text_.find('\n', pos_);
    if (end == std::string::npos) {
      throw std::runtime_error("the backend request ends early");
    }
    std::string_view line(text_.data() + pos_, end - pos_);
    pos_ = end + 1;
    return line;
  }

  // Returns the rest of the line `<key> <value>` after the key and its
  // space.
  std::string_view value(std::string_view key) {
    const std::string_view line = next();
    if (line.substr(0, key.size()) != key || line.size() <= key.size() ||
        line[key.size()] != ' ') {
      throw std::runtime_error("the backend request has no " + std::string(key) +
                               " line");
    }
    return line.substr(key.size() + 1);
  }

  // Returns the text after the lines read so far.
  std::string_view rest() const { return std::string_view(text_).substr(pos_); }

 private:
  const std::string& text_;
  std::size_t pos_ = 0;
};

// Returns the keyword argument `name`, which the request must have.
const json::Value& required(const json::Value& kwargs, std::string_view name) {
  const json::Value* v = kwargs.get(name);
  if (v == nullptr) {
    throw std::runtime_error("the backend request has no " + std::string(name));
  }
  return *v;
}

std::string json_strings(const std::vector<std::string>& items) {
  std::string out = "[";
  for (std::size_t i = 0; i < items.size(); ++i) {
    if (i != 0) {
      out += ", ";
    }
    out += passes::registry::json_quote(items[i]);
  }
  out += ']';
  return out;
}

}  // namespace

std::string respond(const std::string& text) {
  Lines lines(text);
  if (lines.next() != "c6502-backend 1") {
    throw std::runtime_error("not a backend request");
  }
  const std::string mode(lines.value("mode"));
  const passes::AsmContext ctx = capture::context_from_json(json::parse(lines.value("context")));
  const json::Value kwargs = json::parse(lines.value("args"));
  if (lines.next() != "input") {
    throw std::runtime_error("the backend request has no input line");
  }
  auto prog = asdl::load<asm_ast::Program>(lines.rest());
  if (mode == "unoptimized") {
    const passes::SymbolAddrs zp_addrs = ca::addrs_of(required(kwargs, "zp_addrs"));
    return "output\n" + asdl::dump(passes::unoptimized_region(std::move(prog), ctx, zp_addrs));
  }
  if (mode != "optimized") {
    throw std::runtime_error("unknown backend mode " + mode);
  }
  passes::RegionArgs args;
  args.slot_symbols = ca::addrs_of(required(kwargs, "slot_symbols"));
  args.zp_addrs = ca::addrs_of(required(kwargs, "zp_addrs"));
  args.noreturn = ca::names_of(required(kwargs, "noreturn"));
  args.abi = ca::abi_layouts(required(kwargs, "abi"));
  args.shares = ca::slot_shares(required(kwargs, "shares"));
  args.ret_eligible = ca::names_of(required(kwargs, "ret_eligible"));
  args.zp_names = ca::names_of(required(kwargs, "zp_names"));
  args.slot_addrs = ca::ints_of(required(kwargs, "slot_addrs"));
  const passes::RegionResult result = passes::optimized_region(std::move(prog), ctx, args);
  std::string out = "coalesced [";
  for (std::size_t i = 0; i < result.coalesced_params.size(); ++i) {
    if (i != 0) {
      out += ", ";
    }
    const auto& [fn, slots] = result.coalesced_params[i];
    out += '[' + passes::registry::json_quote(fn) + ", " + json_strings(slots) + ']';
  }
  out += "]\nnew_functions [";
  for (std::size_t i = 0; i < result.new_functions.size(); ++i) {
    if (i != 0) {
      out += ", ";
    }
    const auto& made = result.new_functions[i];
    out += '[' + passes::registry::json_quote(made.name) + ", " +
           passes::registry::json_quote(made.owner) + ']';
  }
  out += "]\noutput\n";
  out += asdl::dump(result.prog);
  return out;
}

}  // namespace c6502::backend
