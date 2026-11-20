#include "stage_checks.h"

#include <exception>
#include <stdexcept>
#include <string_view>

#include "asm_ast.h"
#include "json.h"
#include "tac_ast.h"
#include "tagged_json.h"

namespace c6502::stage_checks {

namespace {

using json::Value;

// Returns the dump of the program `text` holds in module `module`,
// read and written again.
std::string redump(std::string_view module, const std::string& text) {
  if (module == "asm_ast" && text.rfind("Function(", 0) == 0) return asdl::dump(asdl::load<asm_ast::Type_top_level>(text));
  if (module == "tac_ast") {
    return asdl::dump(asdl::load<tac_ast::Program>(text));
  }
  return asdl::dump(asdl::load<asm_ast::Program>(text));
}

passes::RoundTrip kwarg_codec(const passes::StageSignature& sig, std::string_view name) {
  for (const auto& [k, codec] : sig.kwargs) {
    if (k == name) {
      return codec;
    }
  }
  throw std::runtime_error("keyword argument '" + std::string(name) +
                           "' isn't in the C++ signature");
}

passes::RoundTrip arg_codec(const passes::StageSignature& sig, std::size_t i) {
  if (i >= sig.args.size()) {
    throw std::runtime_error("positional argument " + std::to_string(i) +
                             " isn't in the C++ signature");
  }
  return sig.args[i];
}

// Returns the `args` line of the record for the call's arguments, each
// read into its C++ form and written again.
std::string redo_args(const passes::StageSignature& sig, const Value& call_args) {
  const Value* positional = call_args.get("args");
  const Value* keywords = call_args.get("kwargs");
  if (positional == nullptr || keywords == nullptr) {
    throw std::runtime_error("the args line isn't {\"args\": ..., \"kwargs\": ...}");
  }
  std::string out = "{\"args\": [";
  for (std::size_t i = 0; i < positional->items.size(); ++i) {
    if (i != 0) {
      out += ", ";
    }
    out += arg_codec(sig, i)(positional->items[i]);
  }
  out += "], \"kwargs\": {";
  for (std::size_t i = 0; i < keywords->items.size(); ++i) {
    if (i != 0) {
      out += ", ";
    }
    tagged::write_string(out, keywords->keys[i]);
    out += ": ";
    out += kwarg_codec(sig, keywords->keys[i])(keywords->items[i]);
  }
  out += "}}";
  return out;
}

// Returns the `mutated` line for the arguments the call changed, each
// read and written again with its argument's codec.
std::string redo_mutated(const passes::StageSignature& sig, const Value& mutated) {
  const Value* positional = mutated.get("args");
  const Value* keywords = mutated.get("kwargs");
  if (positional == nullptr || keywords == nullptr) {
    throw std::runtime_error("the mutated line isn't {\"args\": ..., \"kwargs\": ...}");
  }
  std::string out = "{\"args\": {";
  for (std::size_t i = 0; i < positional->items.size(); ++i) {
    if (i != 0) {
      out += ", ";
    }
    const std::string& index = positional->keys[i];
    tagged::write_string(out, index);
    out += ": ";
    out += arg_codec(sig, std::stoul(index))(positional->items[i]);
  }
  out += "}, \"kwargs\": {";
  for (std::size_t i = 0; i < keywords->items.size(); ++i) {
    if (i != 0) {
      out += ", ";
    }
    tagged::write_string(out, keywords->keys[i]);
    out += ": ";
    out += kwarg_codec(sig, keywords->keys[i])(keywords->items[i]);
  }
  out += "}}";
  return out;
}

// Returns the 1-based number of the first line where `a` and `b`
// differ.
std::size_t first_line(const std::string& a, const std::string& b) {
  std::size_t line = 1;
  for (std::size_t i = 0; i < a.size() && i < b.size(); ++i) {
    if (a[i] != b[i]) {
      return line;
    }
    line += a[i] == '\n';
  }
  return line;
}

void check_tables(const capture::Call& call, const passes::StageSignature& sig) {
  if ((sig.input_json != nullptr) != call.input_is_json) {
    throw std::runtime_error("the record's input doesn't match the signature's");
  }
  if (call.input_is_json) {
    // The harness ends the input's JSON with a newline.
    if (sig.input_json(json::parse(call.input)) + "\n" != call.input) {
      throw std::runtime_error("the input's JSON differs from the recorded one");
    }
  } else if (redump(sig.input, call.input) != call.input) {
    throw std::runtime_error("the input's dump differs from the recorded one");
  }
  if (redo_args(sig, json::parse(call.args_json)) != call.args_json) {
    throw std::runtime_error("the arguments' JSON differs from the recorded one");
  }
  if (!call.mutated_json.empty() &&
      redo_mutated(sig, json::parse(call.mutated_json)) != call.mutated_json) {
    throw std::runtime_error("the changed arguments' JSON differs from the recorded one");
  }
  if (call.output_is_json) {
    if (sig.output == nullptr) {
      throw std::runtime_error("the result is JSON, and the signature has no codec for it");
    }
    // The harness ends the result's JSON with a newline.
    if (sig.output(json::parse(call.output)) + "\n" != call.output) {
      throw std::runtime_error("the result's JSON differs from the recorded one");
    }
  } else if (sig.output != nullptr) {
    throw std::runtime_error("the result is a dump, and the signature has a codec for JSON");
  } else if (redump("asm_ast", call.output) != call.output) {
    throw std::runtime_error("the result's dump differs from the recorded one");
  }
}

}  // namespace

Result check(const capture::Call& call, const passes::StageSignature& sig) {
  Result r;
  try {
    check_tables(call, sig);
  } catch (const std::exception& e) {
    r.tables_error = e.what();
  }
  if (sig.run == nullptr) {
    return r;
  }
  r.ported = true;
  passes::StageOutput got;
  try {
    const Value call_args = json::parse(call.args_json);
    got = sig.run(call, capture::context(call), *call_args.get("args"), *call_args.get("kwargs"));
    if (call.output_is_json) {
      got.output += "\n";
    }
  } catch (const std::exception& e) {
    got.output = std::string("exception: ") + e.what() + "\n";
  }
  if (got.output != call.output) {
    r.mismatch = "the result differs, first on line " +
                 std::to_string(first_line(got.output, call.output));
  } else if (got.mutated != call.mutated_json) {
    r.mismatch = "the arguments it changed differ";
  } else {
    r.matched = true;
  }
  return r;
}

}  // namespace c6502::stage_checks
