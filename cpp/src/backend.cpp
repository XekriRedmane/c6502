#include "backend.h"

#include <stdexcept>
#include <string_view>
#include <utility>
#include <vector>

#include "asdl/runtime.h"
#include "asm_ast.h"
#include "c99_ast.h"
#include "diagnostics.h"
#include "errors.h"
#include "json.h"
#include "passes/capture_args.h"
#include "passes/codegen_stage.h"
#include "passes/front_end.h"
#include "passes/registry_control_flow.h"
#include "passes/zp_globals.h"
#include "stage_checks.h"

namespace c6502::backend {

namespace {

namespace ca = passes::capture_args;

// Reads the lines and sections of a request in order.
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

  // Returns the bytes of the section `<key> <length>` that comes next,
  // the length's bytes after its line.
  std::string_view section(std::string_view key) {
    const std::string length(value(key));
    std::size_t n = 0;
    try {
      n = static_cast<std::size_t>(std::stoull(length));
    } catch (const std::exception&) {
      throw std::runtime_error("the backend request's " + std::string(key) +
                               " section has no length");
    }
    if (pos_ + n > text_.size()) {
      throw std::runtime_error("the backend request's " + std::string(key) + " section ends early");
    }
    std::string_view body(text_.data() + pos_, n);
    pos_ += n;
    return body;
  }

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

// Returns a section of the response, its name and its length in bytes
// on a line, and then its bytes.
std::string section(std::string_view name, const std::string& body) {
  return std::string(name) + " " + std::to_string(body.size()) + "\n" + body;
}

// Returns the strings of the JSON array `v`.
std::vector<std::string> strings_of(const json::Value& v) {
  if (v.kind != json::Value::Kind::kArray) {
    throw std::runtime_error("the backend request's pcpp_args isn't a list");
  }
  std::vector<std::string> out;
  for (const auto& item : v.items) {
    out.push_back(item.string);
  }
  return out;
}

// Returns the lines of the response that carry what the front end
// issued besides its result: what the preprocessor wrote to stderr and
// the warnings, as `[where, message]` pairs.
std::string diagnostics_lines(const std::string& err,
                              const std::vector<diagnostics::CompileWarning>& found) {
  std::string out = section("stderr", err);
  out += "warnings [";
  for (std::size_t i = 0; i < found.size(); ++i) {
    if (i != 0) {
      out += ", ";
    }
    out += '[' + passes::registry::json_quote(found[i].where) + ", " +
           passes::registry::json_quote(found[i].message) + ']';
  }
  out += "]\n";
  return out;
}

}  // namespace

std::string respond(const std::string& text) {
  Lines lines(text);
  if (lines.next() != "c6502-backend 3") {
    throw std::runtime_error("not a backend request");
  }
  const std::string mode(lines.value("mode"));
  if (mode != "source" && mode != "source-text") {
    throw std::runtime_error("unknown backend mode " + mode);
  }
  const json::Value kwargs = json::parse(lines.value("args"));
  passes::FrontEndOptions front;
  front.pcpp_args = strings_of(required(kwargs, "pcpp_args"));
  const json::Value& filename = required(kwargs, "filename");
  if (filename.kind == json::Value::Kind::kString) {
    front.filename = filename.string;
  }
  front.include_dir = required(kwargs, "include_dir").string;
  front.preprocess = ca::boolean(&required(kwargs, "preprocess"), true);
  passes::CodegenOptions options;
  options.optimize = ca::boolean(&required(kwargs, "optimize"), false);
  front.optimize = options.optimize;
  options.inline_calls = ca::boolean(kwargs.get("inline"), true);
  options.link_metadata = ca::boolean(kwargs.get("link_metadata"), false);
  options.emit = mode == "source-text";
  const bool check_ir = ca::boolean(kwargs.get("check_ir"), false);
  const std::string source(lines.section("source"));

  // The warnings and what the preprocessor wrote to stderr go back even
  // when a pass rejects the program, since compile.py prints them
  // before the error.
  std::string err;
  std::vector<diagnostics::CompileWarning> found;
  passes::StageResult result;
  try {
    passes::StageTables tables;
    {
      diagnostics::Collecting block(found);
      passes::FrontEndResult checked = passes::front_end(source, front, err);
      tables.c99 = std::move(checked.prog);
      tables.symbols = std::move(checked.symbols);
      tables.types = std::move(checked.types);
    }
    // The footprints come from the helpers' table compiled into the
    // program (src/rtlib/helpers.h), so the request doesn't carry them.
    tables.footprints = passes::helper_footprints();
    result = passes::codegen(tables, options);
  } catch (const CompileError& e) {
    return diagnostics_lines(err, found) + "error " +
           stage_checks::error_text(e.python_type(), e.what()) + "\n";
  }
  std::string out = diagnostics_lines(err, found);
  if (!options.emit) {
    out += "symbols [";
    for (std::size_t i = 0; i < result.harness_symbols.size(); ++i) {
      if (i != 0) {
        out += ", ";
      }
      const auto& [name, addr] = result.harness_symbols[i];
      out += '[' + passes::registry::json_quote(name) + ", " + asdl::int_to_string(addr) + ']';
    }
    out += "]\n";
  }
  if (check_ir) {
    out += section("tac", asdl::dump(result.tac));
  }
  if (!options.emit) {
    out += section("asm", asdl::dump(result.emitted.asm_prog));
    return out;
  }
  if (check_ir) {
    out += section("asm", asdl::dump(result.emitted.asm_prog));
    out += section("asm2", asdl::dump(result.emitted.asm2_prog));
  }
  out += section("text", result.emitted.text);
  return out;
}

}  // namespace c6502::backend
