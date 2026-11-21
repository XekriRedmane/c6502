// The checks of recorded token streams (parser_token_checks.h).
#include "parser_token_checks.h"

#include <memory>
#include <set>
#include <stdexcept>

#include "json.h"
#include "lexer.h"
#include "parser_context.h"
#include "tagged_json.h"

namespace c6502::parser_token_checks {

namespace {

using passes::StageOutput;
using passes::StageSignature;

template <class C>
constexpr passes::RoundTrip rt = &tagged::round_trip<C>;

std::string any_json(const json::Value& v) { return tagged::to_text(v); }

struct Recorded {
  int type;
  std::string value;
  int line;
  int column;
  std::size_t depth;
};

// The parser's action at the end of a `for` statement, for a stream
// that no parser drives: the end of the loop's scope and the retyping
// of the lookahead, at the reduction by a `for_stmt` rule, where
// parser.py's `for_stmt` makes them.
struct ForHook : lark::Recognizer::ReduceHook {
  parser::TokenStream* stream = nullptr;
  parser::ParseContext* ctx = nullptr;

  void reduced(int rule, lark::Token&, const lark::Recognizer& recognizer) override {
    const auto& table = recognizer.table();
    // Each form of the loop is a rule of its own with this alias.
    if (std::string_view(table.rule_alias[static_cast<std::size_t>(rule)]) != "for_stmt") {
      return;
    }
    ctx->remove_scope(ctx->for_scopes.back());
    ctx->for_scopes.pop_back();
    stream->retype_lookahead(table.rule_origin[static_cast<std::size_t>(rule)],
                             recognizer.stack());
  }
};

void write_outcome(std::string& out, std::string_view kind, std::string_view type,
                   std::string_view message) {
  out += "[";
  tagged::write_string(out, kind);
  out += ", ";
  tagged::write_string(out, type);
  out += ", ";
  tagged::write_string(out, message);
  out += "]";
}

std::vector<int> int_list(const json::Value& v) {
  std::vector<int> out;
  for (const auto& item : tagged::array_items(v)) {
    out.push_back(static_cast<int>(tagged::IntC::decode(item)));
  }
  return out;
}

StageOutput run_parse_tokens(const capture::Call& call, const passes::AsmContext&,
                             const json::Value& args, const json::Value&) {
  const std::string source = tagged::Str::decode(json::parse(call.input));
  if (args.items.size() != 2) {
    throw std::runtime_error("parse_tokens takes two lists");
  }
  // A parse that failed in one of the parser's actions is run as far as
  // the tokens the parser had shifted, which the record's result holds.
  long long fed = -1;
  const json::Value result = json::parse(call.output);
  if (result.kind == json::Value::Kind::kArray && result.items.size() == 2) {
    const json::Value& outcome = result.items[1];
    if (outcome.kind == json::Value::Kind::kArray && outcome.items.size() == 2 &&
        outcome.items[0].string == "action") {
      fed = static_cast<long long>(outcome.items[1].integer);
    }
  }
  return {parse_tokens(source, int_list(args.items[0]), int_list(args.items[1]), fed), "", ""};
}

StageOutput run_tokenize(const capture::Call& call, const passes::AsmContext&, const json::Value&,
                         const json::Value&) {
  return {tokenize_json(tagged::Str::decode(json::parse(call.input))), "", ""};
}

}  // namespace

std::string parse_tokens(std::string_view source, const std::vector<int>& lex_typedefs,
                         const std::vector<int>& retype_typedefs, long long action_fed) {
  const lark_table::Table& table = lark_table::c99_table();
  const std::set<int> lex(lex_typedefs.begin(), lex_typedefs.end());
  const std::set<int> retype(retype_typedefs.begin(), retype_typedefs.end());
  parser::ParseContext ctx;
  ForHook hook;
  hook.ctx = &ctx;
  parser::StreamHooks hooks;
  hooks.typedef_query = [&](const parser::TokenStream& s, std::string_view, bool retyping) {
    const int index = static_cast<int>(retyping ? s.lexed() - 1 : s.lexed());
    return retyping ? retype.contains(index) : lex.contains(index);
  };
  hooks.reduce_hook = &hook;

  std::vector<Recorded> tokens;
  std::unique_ptr<parser::TokenStream> stream;
  std::string outcome = "null";
  const auto add_lookahead = [&] {
    if (!stream->at_end()) {
      const lark::Token& t = stream->peek();
      tokens.push_back({t.type, t.value, t.line, t.column, ctx.scopes.size()});
    }
  };
  try {
    try {
      stream = std::make_unique<parser::TokenStream>(source, ctx, "start", hooks);
      hook.stream = stream.get();
      add_lookahead();
      long long shifted = 0;
      while (true) {
        if (action_fed >= 0 && shifted == action_fed) {
          outcome = "[\"action\", " + std::to_string(action_fed) + "]";
          break;
        }
        const bool end = stream->at_end();
        const lark::Token t = stream->take();
        if (end) {
          break;
        }
        ++shifted;
        tokens.back().type = t.type;
        add_lookahead();
      }
    } catch (...) {
      // The token being fed may have been retyped on the way.
      if (stream && !stream->at_end() && !tokens.empty()) {
        tokens.back().type = stream->peek().type;
      }
      throw;
    }
  } catch (const lark::UnexpectedToken& e) {
    try {
      parser::raise_syntax_error(e, source, ctx);
    } catch (const CompileError& f) {
      outcome.clear();
      write_outcome(outcome, "syntax", f.python_type(), f.what());
    }
  } catch (const lark::UnexpectedCharacters& e) {
    try {
      parser::raise_syntax_error(e, source, ctx);
    } catch (const CompileError& f) {
      outcome.clear();
      write_outcome(outcome, "syntax", f.python_type(), f.what());
    }
  } catch (const lexer::LexError& e) {
    outcome.clear();
    write_outcome(outcome, "lex", e.python_type(), e.what());
  }

  std::string out = "[[";
  for (std::size_t i = 0; i < tokens.size(); ++i) {
    const Recorded& r = tokens[i];
    out += i == 0 ? "[" : ", [";
    tagged::write_string(out, table.name(r.type));
    out += ", ";
    tagged::write_string(out, r.value);
    out += ", " + std::to_string(r.line) + ", " + std::to_string(r.column) + ", " +
           std::to_string(r.depth) + "]";
  }
  out += "], " + outcome + "]";
  return out;
}

std::string tokenize_json(std::string_view source) {
  std::string out = "[";
  bool first = true;
  for (const lexer::Token& t : lexer::tokenize(source)) {
    out += first ? "[" : ", [";
    first = false;
    tagged::write_string(out, lexer::kind_value(t.kind));
    out += ", ";
    tagged::write_string(out, t.value);
    out += ", " + std::to_string(t.line) + ", " + std::to_string(t.col) + "]";
  }
  return out + "]";
}

std::string json_string(std::string_view s) {
  std::string out;
  tagged::write_string(out, s);
  return out;
}

std::vector<StageSignature> signatures() {
  return {
      // The tokens of a parse, with the positions of the identifiers the
      // parser's scopes named as typedefs when they were lexed and when
      // a `for` ended (tools/capture_parser_tokens.py).
      {.name = "parse_tokens",
       .input_json = rt<tagged::Str>,
       .args = {rt<tagged::ListOf<tagged::IntC>>, rt<tagged::ListOf<tagged::IntC>>},
       .output = &any_json,
       .run = &run_parse_tokens},
      {.name = "tokenize", .input_json = rt<tagged::Str>, .output = &any_json, .run = &run_tokenize},
  };
}

}  // namespace c6502::parser_token_checks
