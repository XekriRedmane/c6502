// The C++ side of the `tacopt` records of tools/capture_helpers.py, the
// helpers that the TAC optimizer's passes share (step 5 of the port).
// The encoding of each value is `tac_opt_values`'s.
#include <string>
#include <vector>

#include "helper_checks.h"
#include "passes/optimization/framework.h"
#include "passes/optimization/value_range.h"
#include "passes/optimization/var_visit.h"
#include "tagged_json.h"

namespace c6502::helper_checks {

namespace {

namespace tac = ::c6502::tac_ast;
using namespace ::c6502::passes;

// Joins `xs` with spaces, each empty one written "-", or returns "-"
// for none, as `_fields` does.
std::string fields(const std::vector<std::string>& xs) {
  if (xs.empty()) {
    return "-";
  }
  std::string out;
  for (std::size_t i = 0; i < xs.size(); ++i) {
    if (i != 0) {
      out += ' ';
    }
    out += xs[i].empty() ? "-" : xs[i];
  }
  return out;
}

std::string opt(const std::optional<asdl::Int>& x) {
  return x ? asdl::int_to_string(*x) : ".";
}

// Returns the fields of `rewrite_uses_in` of each instruction with
// `mapper`: "=" for an instruction that comes back unchanged, and the
// one-line dump of the new one with "_" for each space otherwise.
std::vector<std::string> rewritten(const std::vector<tac::Type_instruction>& instrs,
                                   const ValMapper& mapper) {
  std::vector<std::string> out;
  for (const auto& ins : instrs) {
    const auto got = rewrite_uses_in(ins, mapper);
    if (!got) {
      out.emplace_back("=");
      continue;
    }
    std::string d = asdl::dump(*got);
    d.pop_back();
    for (char& c : d) {
      if (c == ' ') {
        c = '_';
      }
    }
    out.push_back(std::move(d));
  }
  return out;
}

}  // namespace

Values tac_opt_values(const json::Value& v) {
  const auto top = tagged::Asdl<tac::Type_top_level, tagged::kTacAst>::decode(*v.get("function"));
  const auto& fn = std::get<tac::Function>(top);
  const SymbolTable symbols = tagged::SymbolTableC::decode(*v.get("symbols"));
  const std::optional<NameSet> ssa = tagged::Optional<tagged::NameSetC>::decode(*v.get("ssa_dsts"));
  const auto& instrs = fn.instructions;
  const auto def_idx = framework::build_def_idx(instrs);
  const value_range::DefOf def_of = [&](const tac::Var& var) -> const tac::Type_instruction* {
    const auto it = def_idx.find(var.name);
    return it == def_idx.end() ? nullptr : &instrs[it->second];
  };
  std::vector<std::string> bounds;
  for (const auto& ins : instrs) {
    std::string field;
    bool first = true;
    for (const tac::Type_val* val : vals_in(ins)) {
      if (!first) {
        field += ',';
      }
      first = false;
      field += opt(value_range::max_value(*val, def_of, &symbols, ssa ? &*ssa : nullptr));
    }
    bounds.push_back(std::move(field));
  }
  std::vector<std::string> queries;
  for (const auto& [name, sym] : symbols) {
    const auto w = value_range::int_width(sym.type);
    queries.push_back((w ? std::to_string(*w) : std::string(".")) + "," +
                      opt(value_range::type_max(sym.type)) + "," +
                      (value_range::is_unsigned_type(sym.type) ? "1" : "0"));
  }
  const ValMapper rename = [](const tac::Type_val& val) -> std::optional<tac::Type_val> {
    if (const auto* var = std::get_if<tac::Var>(&val)) {
      return tac::Var{.name = var->name + "'"};
    }
    return std::nullopt;
  };
  const ValMapper constants = [](const tac::Type_val& val) -> std::optional<tac::Type_val> {
    if (std::holds_alternative<tac::Constant>(val)) {
      return tac::Constant{.const_ = tac::ConstInt{.value = 7}};
    }
    return std::nullopt;
  };
  return {
      {"max_value", fields(bounds)},
      {"type_queries", fields(queries)},
      {"rewrite_vars", fields(rewritten(instrs, rename))},
      {"rewrite_consts", fields(rewritten(instrs, constants))},
  };
}

}  // namespace c6502::helper_checks
