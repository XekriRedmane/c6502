#include "backend.h"

#include <cerrno>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <filesystem>
#include <optional>
#include <stdexcept>
#include <string_view>
#include <utility>
#include <vector>

#include "asdl/runtime.h"
#include "asm_ast.h"
#include "binutils/assemble_command.h"
#include "binutils/assembler.h"
#include "binutils/elf.h"
#include "binutils/ld.h"
#include "c99_ast.h"
#include "diagnostics.h"
#include "errors.h"
#include "json.h"
#include "passes/capture_args.h"
#include "passes/codegen_stage.h"
#include "passes/front_end.h"
#include "passes/link_script.h"
#include "passes/linker.h"
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

// The result of compiling one C source: the lines of the response that
// carry its diagnostics, and either its error line's JSON or the stage's
// result.
struct Compiled {
  std::string diagnostics;
  std::optional<std::string> error;
  passes::StageResult result;
};

// Compiles `source` with the front end's options `front` and code
// generation's `options`. The warnings and what the preprocessor wrote
// to stderr go back even when a pass rejects the program, since
// compile.py prints them before the error.
Compiled compile_one(const std::string& source, const passes::FrontEndOptions& front,
                     const passes::CodegenOptions& options) {
  std::string err;
  std::vector<diagnostics::CompileWarning> found;
  Compiled out;
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
    out.result = passes::codegen(tables, options);
  } catch (const CompileError& e) {
    out.error = stage_checks::error_text(e.python_type(), e.what());
  }
  out.diagnostics = diagnostics_lines(err, found);
  return out;
}

// Returns the front end's options for the request's arguments `kwargs`,
// without the file name, which a request of several inputs gives for
// each one.
passes::FrontEndOptions front_options(const json::Value& kwargs) {
  passes::FrontEndOptions front;
  front.pcpp_args = strings_of(required(kwargs, "pcpp_args"));
  front.include_dir = required(kwargs, "include_dir").string;
  front.optimize = ca::boolean(&required(kwargs, "optimize"), false);
  return front;
}

// Returns `v` as a 64-bit value, the width the C++ linker holds an
// address in. compile.py reads an origin or a `--defsym` with Python's
// unbounded integers, so a wider one throws `InternalError`, which says
// the port doesn't take it.
std::int64_t address(const json::Value& v, std::string_view what) {
  if (v.kind != json::Value::Kind::kInt) {
    throw std::runtime_error("the backend request's " + std::string(what) +
                             " isn't an integer");
  }
  if (v.integer < asdl::Int(INT64_MIN) || v.integer > asdl::Int(INT64_MAX)) {
    throw InternalError("the C++ linker holds an address in 64 bits, and the " +
                        std::string(what) + " " + asdl::int_to_string(v.integer) + " is wider");
  }
  return static_cast<std::int64_t>(v.integer);
}

// Writes `data` to the file at `path`, as Python's `open(path, "wb")`
// and `write` do, and returns the errno of a file that can't be opened
// or written, or 0 when the file was written.
int write_file(const std::string& path, const std::string& data) {
  const std::filesystem::path p(std::u8string(path.begin(), path.end()));
  errno = 0;
#ifdef _WIN32
  std::FILE* f = nullptr;
  if (const errno_t opened = _wfopen_s(&f, p.c_str(), L"wb"); opened != 0) return opened;
#else
  std::FILE* f = std::fopen(p.c_str(), "wb");
#endif
  if (f == nullptr) return errno != 0 ? errno : EIO;
  const bool wrote = std::fwrite(data.data(), 1, data.size(), f) == data.size();
  const int saved = errno;
  const bool closed = std::fclose(f) == 0;
  if (!wrote || !closed) return saved != 0 ? saved : (errno != 0 ? errno : EIO);
  return 0;
}

// Returns the C library's message for the errno `error`, which Python's
// OSError takes as its strerror.
std::string error_message(int error) {
#ifdef _WIN32
  char buf[256];
  strerror_s(buf, sizeof buf, error);
  return buf;
#else
  return std::strerror(error);
#endif
}

// Returns the response to a request of mode `objects` (`-c`) or `link`
// (`link` true), whose arguments are `kwargs` and whose input sections
// `lines` holds next (backend.h). Each input is compiled, assembled, or
// read in order, as compile.py's `_object` does, and the first one that
// fails ends the response.
std::string respond_inputs(Lines& lines, const json::Value& kwargs, bool link) {
  const passes::FrontEndOptions front = front_options(kwargs);
  passes::CodegenOptions options;
  options.optimize = front.optimize;
  options.inline_calls = ca::boolean(kwargs.get("inline"), true);
  // `_object` compiles every C input with the metadata block, for `-c`
  // and for a link.
  options.link_metadata = true;
  options.emit = true;
  const bool check_ir = ca::boolean(kwargs.get("check_ir"), false);
  const json::Value& inputs = required(kwargs, "inputs");
  if (inputs.kind != json::Value::Kind::kArray) {
    throw std::runtime_error("the backend request's inputs isn't a list");
  }
  std::vector<std::string_view> bodies;
  for (std::size_t k = 0; k < inputs.items.size(); ++k) {
    bodies.push_back(lines.section("input"));
  }

  std::string out;
  binutils::ld::Objects objects;
  for (std::size_t k = 0; k < inputs.items.size(); ++k) {
    const json::Value& input = inputs.items[k];
    const std::string kind = required(input, "kind").string;
    const std::string name = required(input, "name").string;
    out += "input " + std::to_string(k) + "\n";
    std::optional<binutils::elf::ElfFile> obj;
    try {
      if (kind == "c") {
        passes::FrontEndOptions with_name = front;
        const json::Value& filename = required(input, "filename");
        if (filename.kind == json::Value::Kind::kString) with_name.filename = filename.string;
        Compiled compiled = compile_one(std::string(bodies[k]), with_name, options);
        out += compiled.diagnostics;
        if (compiled.error) return out + "error " + *compiled.error + "\n";
        if (check_ir) {
          out += section("tac", asdl::dump(compiled.result.tac));
          out += section("asm", asdl::dump(compiled.result.emitted.asm_prog));
          out += section("asm2", asdl::dump(compiled.result.emitted.asm2_prog));
        }
        obj = binutils::assembler::assemble(compiled.result.emitted.text, name);
      } else if (kind == "asm") {
        obj = binutils::assembler::assemble(bodies[k], name);
      } else if (kind == "object" && link) {
        obj = binutils::elf::read(bodies[k], name);
      } else {
        throw std::runtime_error("the backend request has an input of kind " + kind);
      }
    } catch (const CompileError& e) {
      return out + "error " + stage_checks::error_text(e.python_type(), e.what()) + "\n";
    }
    if (!link) {
      // compile.py opens the object's file before `elf.write` packs the
      // fields, so a `struct.error` leaves the file empty, which
      // cpp_backend.py reproduces.
      try {
        binutils::check_write_fields(*obj);
      } catch (const CompileError& e) {
        return out + "write-error " + stage_checks::error_text(e.python_type(), e.what()) + "\n";
      }
      out += section("object", binutils::elf::write(*obj));
      continue;
    }
    // The link takes the object the assembler made, as compile.py's
    // does, without writing and reading it.
    out += "ok\n";
    objects.emplace_back(required(input, "link_name").string, std::move(*obj));
  }
  if (!link || !ca::boolean(kwargs.get("run_link"), true)) return out;

  // compile.py reads the script after the inputs, links, opens the
  // output, and then writes the executable's bytes.
  const json::Value& script_path = required(kwargs, "script");
  const json::Value& defsyms = required(kwargs, "defsyms");
  binutils::ld::Names names;
  for (const json::Value& pair : defsyms.items) {
    if (pair.items.size() != 2) throw std::runtime_error("a defsym isn't a pair");
    names.insert_or_assign(pair.items[0].string, address(pair.items[1], "--defsym value"));
  }
  // A null origin is `DEFAULT_ORIGIN`, as compile.py gives the linker
  // when the command line has none.
  const json::Value& given = required(kwargs, "origin");
  const std::int64_t origin = given.kind == json::Value::Kind::kNull
                                  ? passes::linker::kDefaultOrigin
                                  : address(given, "origin");
  binutils::elf::ElfFile exe;
  try {
    std::optional<passes::link_script::Script> script;
    if (script_path.kind == json::Value::Kind::kString) {
      script = passes::link_script::read_script(script_path.string);
    }
    exe = passes::linker::link_objects(std::move(objects), script, origin, names);
  } catch (const CompileError& e) {
    return out + "link-error " + stage_checks::error_text(e.python_type(), e.what()) + "\n";
  }
  try {
    binutils::check_write_fields(exe);
  } catch (const CompileError& e) {
    return out + "write-error " + stage_checks::error_text(e.python_type(), e.what()) + "\n";
  }
  const std::string data = binutils::elf::write(exe);
  if (const int error = write_file(required(kwargs, "output").string, data); error != 0) {
    return out + "output-error {\"errno\": " + std::to_string(error) +
           ", \"strerror\": " + passes::registry::json_quote(error_message(error)) + "}\n";
  }
  return out + "executable " + std::to_string(data.size()) + "\n";
}

}  // namespace

std::string respond(const std::string& text) {
  Lines lines(text);
  if (lines.next() != "c6502-backend 3") {
    throw std::runtime_error("not a backend request");
  }
  const std::string mode(lines.value("mode"));
  if (mode != "source" && mode != "source-text" && mode != "objects" && mode != "link") {
    throw std::runtime_error("unknown backend mode " + mode);
  }
  const json::Value kwargs = json::parse(lines.value("args"));
  if (mode == "objects" || mode == "link") return respond_inputs(lines, kwargs, mode == "link");
  passes::FrontEndOptions front = front_options(kwargs);
  const json::Value& filename = required(kwargs, "filename");
  if (filename.kind == json::Value::Kind::kString) {
    front.filename = filename.string;
  }
  front.preprocess = ca::boolean(&required(kwargs, "preprocess"), true);
  passes::CodegenOptions options;
  options.optimize = front.optimize;
  options.inline_calls = ca::boolean(kwargs.get("inline"), true);
  options.link_metadata = ca::boolean(kwargs.get("link_metadata"), false);
  options.emit = mode == "source-text";
  const bool check_ir = ca::boolean(kwargs.get("check_ir"), false);
  const std::string source(lines.section("source"));

  const Compiled compiled = compile_one(source, front, options);
  std::string out = compiled.diagnostics;
  if (compiled.error) return out + "error " + *compiled.error + "\n";
  const passes::StageResult& result = compiled.result;
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
