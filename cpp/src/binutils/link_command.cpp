#include "binutils/link_command.h"

#include <algorithm>
#include <chrono>
#include <cstdint>
#include <optional>
#include <stdexcept>
#include <string_view>
#include <vector>

#include "binutils/elf.h"
#include "binutils/ld.h"
#include "errors.h"
#include "json.h"
#include "passes/link_script.h"
#include "passes/linker.h"
#include "stage_checks.h"

namespace c6502::binutils {

namespace {

class Request {
 public:
  explicit Request(const std::string& text) : text_(text) {}

  std::string_view next() {
    const std::size_t end = text_.find('\n', pos_);
    if (end == std::string::npos) throw std::runtime_error("the link request ends early");
    std::string_view line(text_.data() + pos_, end - pos_);
    pos_ = end + 1;
    return line;
  }

  std::string_view value(std::string_view key) {
    const std::string_view line = next();
    if (!line.starts_with(key) || line.size() <= key.size() || line[key.size()] != ' ') {
      throw std::runtime_error("the link request has no " + std::string(key) + " line");
    }
    return line.substr(key.size() + 1);
  }

  std::string_view section(std::string_view key) {
    const std::string length(value(key));
    std::size_t n = 0;
    try {
      n = static_cast<std::size_t>(std::stoull(length));
    } catch (const std::exception&) {
      throw std::runtime_error("the link request's " + std::string(key) +
                               " section has no length");
    }
    if (pos_ + n > text_.size()) {
      throw std::runtime_error("the link request's " + std::string(key) + " section ends early");
    }
    std::string_view body(text_.data() + pos_, n);
    pos_ += n;
    return body;
  }

 private:
  const std::string& text_;
  std::size_t pos_ = 0;
};

std::int64_t address(const json::Value& v, std::string_view what) {
  if (v.kind != json::Value::Kind::kInt) {
    throw std::runtime_error("the link request's " + std::string(what) + " isn't an integer");
  }
  if (v.integer < asdl::Int(INT64_MIN) || v.integer > asdl::Int(INT64_MAX)) {
    throw InternalError("the C++ linker holds an address in 64 bits, and the " +
                        std::string(what) + " " + asdl::int_to_string(v.integer) +
                        " is wider");
  }
  return static_cast<std::int64_t>(v.integer);
}

struct LinkRequest {
  std::vector<std::pair<std::string, std::string_view>> objects;
  std::optional<std::string> script;
  std::int64_t origin = passes::linker::kDefaultOrigin;
  ld::Names defsyms;
};

LinkRequest read_request(const std::string& text) {
  Request request(text);
  if (request.next() != "c6502-link 1") {
    throw std::runtime_error("the link request doesn't start with c6502-link 1");
  }
  const json::Value args = json::parse(request.value("args"));
  const json::Value* names = args.get("objects");
  const json::Value* script = args.get("script");
  const json::Value* origin = args.get("origin");
  const json::Value* defsyms = args.get("defsyms");
  if (names == nullptr || names->kind != json::Value::Kind::kArray || script == nullptr ||
      origin == nullptr || defsyms == nullptr || defsyms->kind != json::Value::Kind::kArray) {
    throw std::runtime_error("the link request's args need objects, script, origin, and defsyms");
  }
  LinkRequest out;
  for (const json::Value& name : names->items) {
    out.objects.emplace_back(name.string, request.section("object"));
  }
  if (script->kind == json::Value::Kind::kString) out.script = script->string;
  out.origin = address(*origin, "origin");
  for (const json::Value& pair : defsyms->items) {
    if (pair.items.size() != 2) throw std::runtime_error("a defsym isn't a pair");
    out.defsyms.insert_or_assign(pair.items[0].string, address(pair.items[1], "--defsym value"));
  }
  return out;
}

std::string error_response(const CompileError& e, std::optional<std::size_t> input) {
  std::string out = stage_checks::error_text(e.python_type(), e.what());
  out.pop_back();
  out += ", \"input\": " + (input ? std::to_string(*input) : std::string("null")) + "}";
  return "error " + out + "\n";
}

std::string run(const LinkRequest& request) {
  ld::Objects objects;
  for (std::size_t k = 0; k < request.objects.size(); ++k) {
    const auto& [name, data] = request.objects[k];
    try {
      objects.emplace_back(name, elf::read(data, name));
    } catch (const CompileError& e) {
      return error_response(e, k);
    }
  }
  try {
    std::optional<passes::link_script::Script> script;
    if (request.script) script = passes::link_script::read_script(*request.script);
    const std::string data = elf::write(
        passes::linker::link_objects(std::move(objects), script, request.origin, request.defsyms));
    return "executable " + std::to_string(data.size()) + "\n" + data;
  } catch (const CompileError& e) {
    return error_response(e, std::nullopt);
  }
}

}  // namespace

std::string link_response(const std::string& text, int repeat, double* best_ms) {
  const LinkRequest request = read_request(text);
  std::string response;
  double best = 0;
  for (int k = 0; k < std::max(repeat, 1); ++k) {
    const auto start = std::chrono::steady_clock::now();
    response = run(request);
    const double ms =
        std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now() - start)
            .count();
    best = k == 0 ? ms : std::min(best, ms);
  }
  if (best_ms != nullptr) *best_ms = best;
  return response;
}

}  // namespace c6502::binutils
