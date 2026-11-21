#include "backend.h"

#include <stdexcept>
#include <string_view>
#include <utility>

#include "asdl/runtime.h"
#include "asm_ast.h"
#include "c99_ast.h"
#include "capture.h"
#include "json.h"
#include "passes/capture_args.h"
#include "passes/codegen.h"
#include "passes/codegen_stage.h"
#include "passes/emission_args.h"
#include "passes/optimized_asm.h"
#include "passes/registry_control_flow.h"
#include "tac_ast.h"
#include "tagged_json.h"

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

// Returns the arguments of the work after the region and of emission.
passes::EmitArgs emit_args(const json::Value& kwargs, bool optimized) {
  namespace ea = passes::emission_args;
  passes::EmitArgs args;
  args.zp = ea::zp_globals(required(kwargs, "zp"));
  args.footprints = ea::footprints(required(kwargs, "footprints"));
  args.sections = ea::section_infos(required(kwargs, "sections"));
  args.equs = ea::ordered_addrs(required(kwargs, "equs"));
  const json::Value& meta = required(kwargs, "meta");
  if (meta.kind != json::Value::Kind::kNull) {
    args.meta = ea::link_metadata(meta);
  }
  if (optimized) {
    args.slot_symbols = ea::ordered_addrs(required(kwargs, "slot_symbols"));
    args.preferred_symbols = ea::preferred_symbols(required(kwargs, "preferred_symbols"));
  }
  return args;
}

// Returns a section of the response, its name and its length in bytes
// on a line, and then its bytes.
std::string section(std::string_view name, const std::string& body) {
  return std::string(name) + " " + std::to_string(body.size()) + "\n" + body;
}

// Returns the response of a run through emission: the functions made,
// the finished asm and asm2 programs when the request sets `check_ir`, to
// check, and the text.
std::string emit_response(const passes::EmitResult& result, bool check_ir) {
  std::string out = "new_functions [";
  for (std::size_t i = 0; i < result.new_functions.size(); ++i) {
    if (i != 0) {
      out += ", ";
    }
    const auto& made = result.new_functions[i];
    out += '[' + passes::registry::json_quote(made.name) + ", " +
           passes::registry::json_quote(made.owner) + ']';
  }
  out += "]\n";
  if (check_ir) {
    out += section("asm", asdl::dump(result.asm_prog));
    out += section("asm2", asdl::dump(result.asm2_prog));
  }
  out += section("text", result.text);
  return out;
}

passes::RegionArgs region_args(const json::Value& kwargs) {
  passes::RegionArgs args;
  args.slot_symbols = ca::addrs_of(required(kwargs, "slot_symbols"));
  args.zp_addrs = ca::addrs_of(required(kwargs, "zp_addrs"));
  args.noreturn = ca::names_of(required(kwargs, "noreturn"));
  args.abi = ca::abi_layouts(required(kwargs, "abi"));
  args.shares = ca::slot_shares(required(kwargs, "shares"));
  args.ret_eligible = ca::names_of(required(kwargs, "ret_eligible"));
  args.zp_names = ca::names_of(required(kwargs, "zp_names"));
  args.slot_addrs = ca::ints_of(required(kwargs, "slot_addrs"));
  return args;
}

// Returns the response lines of the changes the stage made to the
// symbol table, which cpp_backend.py enters in Python's.
std::string changes_lines(const passes::SymbolChanges& changes) {
  std::string out = "new_functions [";
  for (std::size_t i = 0; i < changes.new_functions.size(); ++i) {
    if (i != 0) {
      out += ", ";
    }
    const auto& made = changes.new_functions[i];
    out += '[' + passes::registry::json_quote(made.name) + ", " +
           passes::registry::json_quote(made.owner) + ']';
  }
  out += "]\nreg_locals " + json_strings(changes.reg_locals) + "\ncarry " +
         json_strings(changes.carry) + "\n";
  return out;
}

// Returns the response to a request of a stage mode, whose lines after
// the mode `lines` reads.
std::string stage_response(const std::string& mode, Lines& lines) {
  const bool optimized = mode == "stage-optimized" || mode == "stage-optimized-text";
  const bool emit = mode == "stage-optimized-text" || mode == "stage-unoptimized-text";
  if (!optimized && !emit && mode != "stage-unoptimized") {
    throw std::runtime_error("unknown backend mode " + mode);
  }
  const json::Value kwargs = json::parse(lines.value("args"));
  passes::StageTables tables;
  tables.symbols = tagged::SymbolTableC::decode(required(kwargs, "symbols"));
  tables.types = tagged::TypeTableC::decode(required(kwargs, "types"));
  tables.statics = tagged::NameSetC::decode(required(kwargs, "statics"));
  tables.footprints = passes::emission_args::footprints(required(kwargs, "footprints"));
  const bool check_ir = ca::boolean(kwargs.get("check_ir"), false);
  const bool link_metadata = ca::boolean(kwargs.get("link_metadata"), false);
  tables.c99 = asdl::load<c99_ast::Program>(lines.section("c99"));
  const auto tac = asdl::load<tac_ast::Program>(lines.section("tac"));
  passes::StageResult result;
  if (optimized) {
    const auto optimized_tac = asdl::load<tac_ast::Program>(lines.section("optimized-tac"));
    result = passes::optimized_stage(tac, optimized_tac, tables, emit);
  } else {
    result = passes::unoptimized_stage(tac, tables, link_metadata, emit);
  }
  std::string out = changes_lines(result.changes);
  if (emit) {
    if (check_ir) {
      out += section("asm", asdl::dump(result.emitted.asm_prog));
      out += section("asm2", asdl::dump(result.emitted.asm2_prog));
    }
    out += section("text", result.emitted.text);
    return out;
  }
  out += "symbols [";
  for (std::size_t i = 0; i < result.harness_symbols.size(); ++i) {
    if (i != 0) {
      out += ", ";
    }
    const auto& [name, addr] = result.harness_symbols[i];
    out += '[' + passes::registry::json_quote(name) + ", " + asdl::int_to_string(addr) + ']';
  }
  out += "]\n";
  out += section("asm", asdl::dump(result.emitted.asm_prog));
  return out;
}

}  // namespace

std::string respond(const std::string& text) {
  Lines lines(text);
  if (lines.next() != "c6502-backend 1") {
    throw std::runtime_error("not a backend request");
  }
  const std::string mode(lines.value("mode"));
  if (mode.starts_with("stage-")) {
    return stage_response(mode, lines);
  }
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
  if (mode == "optimized-text" || mode == "unoptimized-text") {
    const bool optimized = mode == "optimized-text";
    const bool check_ir = ca::boolean(kwargs.get("check_ir"), false);
    const passes::EmitArgs args = emit_args(kwargs, optimized);
    if (optimized) {
      return emit_response(
          passes::optimized_text(std::move(prog), ctx, region_args(kwargs), args), check_ir);
    }
    const passes::SymbolAddrs zp_addrs = ca::addrs_of(required(kwargs, "zp_addrs"));
    return emit_response(passes::unoptimized_text(std::move(prog), ctx, zp_addrs, args),
                         check_ir);
  }
  if (mode != "optimized") {
    throw std::runtime_error("unknown backend mode " + mode);
  }
  const passes::RegionResult result =
      passes::optimized_region(std::move(prog), ctx, region_args(kwargs));
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
