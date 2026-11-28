#include "tac_sim_modes.h"

#include <optional>
#include <stdexcept>
#include <string_view>
#include <utility>
#include <vector>

#include "c99_ast.h"
#include "c99_to_tac.h"
#include "diagnostics.h"
#include "errors.h"
#include "parser.h"
#include "passes/front_end.h"
#include "passes/identifier_resolution.h"
#include "passes/label_resolution.h"
#include "passes/loop_labeling.h"
#include "passes/optimization/driver.h"
#include "passes/optimization/optimizer.h"
#include "passes/string_lifting.h"
#include "passes/type_checker.h"
#include "stage_checks.h"
#include "tac_sim.h"

namespace c6502::tac_sim_modes {

namespace {

namespace ir = ::c6502::tac_ast;
namespace c99 = ::c6502::c99_ast;

// The function that no program defines, whose call fuzz/reduce.py
// appends to each function that returns a value.
constexpr std::string_view kFellOff = "__reduce_fell_off";

// Appends `__reduce_fell_off();` to the body of each function
// definition of `prog` that returns a value, other than `main`, as the
// parser would have built the call.
void append_fell_off_calls(c99::Program& prog) {
  for (auto& decl : prog.declaration) {
    auto* fd = asdl::as<c99::FunctionDecl>(decl);
    if (fd == nullptr) continue;
    c99::Type_function_decl& f = *fd->function_decl;
    if (!f.body || f.name == "main") continue;
    const auto* fun = asdl::as<c99::FunType>(f.data_type);
    if (fun == nullptr || asdl::isa<c99::Void>(*fun->ret)) continue;
    c99::FunctionCall call;
    call.name = std::string(kFellOff);
    c99::Block& body = std::get<c99::Block>(**f.body);
    body.block_item.push_back(c99::S{.statement = c99::Expression{.exp = std::move(call)}});
  }
}

// Returns an outcome as the JSON of the `result` and `optimized` lines.
std::string outcome_json(const tac_sim::Outcome& o) {
  if (o.error_type) {
    return "{\"error\": {\"type\": " + json::quote(*o.error_type) +
           ", \"message\": " + json::quote(o.error_message) + "}}";
  }
  return std::string("{\"value\": ") + (o.value ? asdl::int_to_string(*o.value) : "null") + "}";
}

// Returns an outcome as the JSON of fuzz/tac_passes.py's result: the
// value, null, or `ERR <type>: <message>`.
std::string observed_json(const tac_sim::Outcome& o) {
  if (o.error_type) return json::quote("ERR " + *o.error_type + ": " + o.error_message);
  return o.value ? asdl::int_to_string(*o.value) : "null";
}

std::string section(std::string_view name, const std::string& body) {
  return std::string(name) + " " + std::to_string(body.size()) + "\n" + body;
}

std::string diagnostics_lines(const std::string& err,
                              const std::vector<diagnostics::CompileWarning>& found) {
  std::string out = section("stderr", err);
  out += "warnings [";
  for (std::size_t i = 0; i < found.size(); ++i) {
    if (i != 0) out += ", ";
    out += '[' + json::quote(found[i].where) + ", " + json::quote(found[i].message) + ']';
  }
  out += "]\n";
  return out;
}

bool boolean(const json::Value* v, bool fallback) {
  if (v == nullptr || v->kind != json::Value::Kind::kBool) return fallback;
  return v->boolean;
}

// Returns the rest of a `tac-observe` response.
std::string observe(Translated& t) {
  const std::optional<ChangingPass> found = first_changing_pass(t);
  if (!found) return "found null\n";
  return "found [" + json::quote(found->function) + ", " + json::quote(found->pass) + ", " +
         observed_json(found->expected) + ", " + observed_json(found->got) + "]\n";
}

}  // namespace

PlainAndOptimized run_plain_and_optimized(Translated t) {
  PlainAndOptimized out{tac_sim::run_main(t.tac, t.symbols, &t.types), std::nullopt};
  if (!out.plain.error_type) {
    ir::Program opt = passes::optimize_program(std::move(t.tac), &t.symbols);
    out.optimized = tac_sim::run_main(opt, t.symbols, &t.types);
  }
  return out;
}

std::optional<ChangingPass> first_changing_pass(Translated& t) {
  const tac_sim::Outcome expected = tac_sim::run_main(t.tac, t.symbols, &t.types);
  ir::Program current = t.tac;
  std::optional<ChangingPass> found;
  const passes::framework::Observer observer = [&](const ir::Function& fn, std::string_view after) {
    if (found) return;
    for (auto& top : current.top_level) {
      auto* f = asdl::as<ir::Function>(top);
      if (f != nullptr && f->name == fn.name) {
        *f = fn;
        break;
      }
    }
    const tac_sim::Outcome got = tac_sim::run_main(current, t.symbols, &t.types);
    if (got != expected) found = ChangingPass{fn.name, std::string(after), expected, got};
  };
  const passes::framework::Observer* previous = passes::framework::set_observer(&observer);
  try {
    passes::optimize_program(t.tac, &t.symbols);
  } catch (...) {
    passes::framework::set_observer(previous);
    throw;
  }
  passes::framework::set_observer(previous);
  return found;
}

bool falls_off_end(const std::string& source, const passes::FrontEndOptions& options,
                   std::string& err) {
  std::optional<Translated> t;
  try {
    t = translate("void " + std::string(kFellOff) + "(void);\n" + source, options, err, true);
  } catch (const CompileError&) {
    return false;
  } catch (const InternalError&) {
    return false;
  }
  tac_sim::Outcome o;
  try {
    o = tac_sim::run_main(t->tac, t->symbols, &t->types);
  } catch (const InternalError&) {
    return false;
  }
  return o.error_type == "TacSimError" && o.error_message.find(kFellOff) != std::string::npos;
}

Translated translate(const std::string& source, const passes::FrontEndOptions& options,
                     std::string& err, bool fell_off) {
  const std::string text = passes::preprocess_source(source, options, err);
  c99::Program prog = parser::parse(text);
  if (fell_off) append_fell_off_calls(prog);
  prog = passes::resolve_identifiers(std::move(prog));
  prog = passes::lift_strings(std::move(prog));
  prog = passes::resolve_labels(std::move(prog));
  prog = passes::label_program(std::move(prog));
  passes::type_checking::CheckedProgram tables = passes::type_checking::check_program(prog);
  Translated out{.symbols = std::move(tables.symbols), .types = std::move(tables.types)};
  out.tac = c99_to_tac::translate_program(prog, out.symbols, &out.types);
  return out;
}

bool is_tac_mode(const std::string& mode) {
  return mode == "tac-run" || mode == "tac-observe" || mode == "tac-falls-off";
}

std::string respond(const std::string& mode, const json::Value& kwargs, const std::string& source) {
  passes::FrontEndOptions options;
  if (const json::Value* args = kwargs.get("pcpp_args")) {
    for (const auto& a : args->items) options.pcpp_args.push_back(a.string);
  }
  if (const json::Value* f = kwargs.get("filename"); f != nullptr && f->kind == json::Value::Kind::kString) {
    options.filename = f->string;
  }
  if (const json::Value* d = kwargs.get("include_dir")) options.include_dir = d->string;
  options.preprocess = boolean(kwargs.get("preprocess"), true);
  std::string err;
  std::vector<diagnostics::CompileWarning> found;
  if (mode == "tac-falls-off") {
    bool fell = false;
    {
      diagnostics::Collecting block(found);
      fell = falls_off_end(source, options, err);
    }
    return diagnostics_lines(err, found) + "fell " + (fell ? "true" : "false") + "\n";
  }
  std::optional<Translated> t;
  std::optional<std::string> error;
  try {
    diagnostics::Collecting block(found);
    t = translate(source, options, err);
  } catch (const CompileError& e) {
    error = stage_checks::error_text(e.python_type(), e.what());
  }
  std::string out = diagnostics_lines(err, found);
  if (error) return out + "error " + *error + "\n";
  if (mode == "tac-observe") return out + observe(*t);
  if (!boolean(kwargs.get("optimized"), false)) {
    return out + "result " + outcome_json(tac_sim::run_main(t->tac, t->symbols, &t->types)) + "\n";
  }
  const PlainAndOptimized r = run_plain_and_optimized(std::move(*t));
  out += "result " + outcome_json(r.plain) + "\n";
  if (r.optimized) out += "optimized " + outcome_json(*r.optimized) + "\n";
  return out;
}

}  // namespace c6502::tac_sim_modes
