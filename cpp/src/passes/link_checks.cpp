#include "passes/link_checks.h"

#include <optional>
#include <utility>
#include <vector>

#include "errors.h"

namespace c6502::passes::link_checks {

namespace {

namespace ls = ::c6502::passes::link_script;
using json::Value;
using ls::Expr;

// --- Building JSON ---

Value null() { return Value{}; }

Value boolean(bool b) {
  Value v;
  v.kind = Value::Kind::kBool;
  v.boolean = b;
  return v;
}

Value integer(asdl::Int i) {
  Value v;
  v.kind = Value::Kind::kInt;
  v.integer = i;
  return v;
}

Value string(std::string s) {
  Value v;
  v.kind = Value::Kind::kString;
  v.string = std::move(s);
  return v;
}

Value array(std::vector<Value> items = {}) {
  Value v;
  v.kind = Value::Kind::kArray;
  v.items = std::move(items);
  return v;
}

Value object(std::vector<std::pair<std::string, Value>> members) {
  Value v;
  v.kind = Value::Kind::kObject;
  for (auto& [k, m] : members) {
    v.keys.push_back(k);
    v.items.push_back(std::move(m));
  }
  return v;
}

// The tagged JSON of a dataclass of passes/link_script.py.
Value tagged(const std::string& cls, std::vector<std::pair<std::string, Value>> fields) {
  return object({{"object", string("passes.link_script." + cls)}, {"fields", object(std::move(fields))}});
}

Value expr_json(const Expr& e) {
  switch (e.kind) {
    case Expr::Kind::Num:
      return tagged("Num", {{"value", integer(e.value)}});
    case Expr::Kind::Sym:
      return tagged("Sym", {{"name", string(e.name)}});
    case Expr::Kind::Dot:
      return tagged("Dot", {});
    case Expr::Kind::Unary:
      return tagged("Unary", {{"op", string(e.op)}, {"operand", expr_json(e.args[0])}});
    case Expr::Kind::Binary:
      return tagged("Binary", {{"op", string(e.op)},
                               {"left", expr_json(e.args[0])},
                               {"right", expr_json(e.args[1])}});
    case Expr::Kind::Call: {
      std::vector<Value> args;
      for (const Expr& a : e.args) args.push_back(expr_json(a));
      return tagged("Call", {{"fn", string(e.fn)}, {"args", array(std::move(args))}});
    }
  }
  return null();
}

Value opt_expr(const std::optional<Expr>& e) { return e ? expr_json(*e) : null(); }

Value opt_string(const std::optional<std::string>& s) { return s ? string(*s) : null(); }

Value assign_json(const ls::Assign& a) {
  return tagged("Assign", {{"symbol", string(a.symbol)}, {"expr", expr_json(a.expr)}});
}

Value strings(const std::vector<std::string>& v) {
  std::vector<Value> out;
  for (const auto& s : v) out.push_back(string(s));
  return array(std::move(out));
}

Value section_json(const ls::OutputSection& sec) {
  std::vector<Value> contents;
  for (const ls::Content& c : sec.contents) {
    if (const auto* a = std::get_if<ls::Assign>(&c)) {
      contents.push_back(assign_json(*a));
    } else {
      const auto& in = std::get<ls::Input>(c);
      contents.push_back(
          tagged("Input", {{"patterns", strings(in.patterns)}, {"file", string(in.file)}}));
    }
  }
  return tagged("OutputSection", {{"name", string(sec.name)},
                                  {"address", opt_expr(sec.address)},
                                  {"noload", boolean(sec.noload)},
                                  {"reorder", boolean(sec.reorder)},
                                  {"at", opt_expr(sec.at)},
                                  {"align", opt_expr(sec.align)},
                                  {"contents", array(std::move(contents))},
                                  {"region", opt_string(sec.region)},
                                  {"load_region", opt_string(sec.load_region)}});
}

// --- Reading JSON ---

[[noreturn]] void bad(const std::string& what) {
  throw InternalError("link_checks: unexpected JSON for " + what);
}

const Value& field(const Value& v, std::string_view key) {
  const Value* f = v.get(key);
  if (f == nullptr) bad(std::string(key));
  return *f;
}

const Value& fields(const Value& tagged_value, std::string_view cls) {
  const Value* o = tagged_value.get("object");
  if (o == nullptr || o->string != "passes.link_script." + std::string(cls)) bad(std::string(cls));
  return field(tagged_value, "fields");
}

std::string class_of(const Value& v) {
  const Value* o = v.get("object");
  if (o == nullptr) bad("a tagged object");
  const std::string prefix = "passes.link_script.";
  return o->string.substr(prefix.size());
}

Expr expr_from(const Value& v) {
  const std::string cls = class_of(v);
  const Value& f = field(v, "fields");
  Expr e;
  if (cls == "Num") {
    e.kind = Expr::Kind::Num;
    e.value = field(f, "value").integer;
  } else if (cls == "Sym") {
    e.kind = Expr::Kind::Sym;
    e.name = field(f, "name").string;
  } else if (cls == "Dot") {
    e.kind = Expr::Kind::Dot;
  } else if (cls == "Unary") {
    e.kind = Expr::Kind::Unary;
    e.op = field(f, "op").string;
    e.args.push_back(expr_from(field(f, "operand")));
  } else if (cls == "Binary") {
    e.kind = Expr::Kind::Binary;
    e.op = field(f, "op").string;
    e.args.push_back(expr_from(field(f, "left")));
    e.args.push_back(expr_from(field(f, "right")));
  } else if (cls == "Call") {
    e.kind = Expr::Kind::Call;
    e.fn = field(f, "fn").string;
    for (const Value& a : field(f, "args").items) e.args.push_back(expr_from(a));
  } else {
    bad(cls);
  }
  return e;
}

std::optional<Expr> opt_expr_from(const Value& v) {
  if (v.kind == Value::Kind::kNull) return std::nullopt;
  return expr_from(v);
}

std::optional<std::string> opt_string_from(const Value& v) {
  if (v.kind == Value::Kind::kNull) return std::nullopt;
  return v.string;
}

ls::Assign assign_from(const Value& v) {
  const Value& f = fields(v, "Assign");
  return ls::Assign{field(f, "symbol").string, expr_from(field(f, "expr"))};
}

std::vector<std::string> strings_from(const Value& v) {
  std::vector<std::string> out;
  for (const Value& s : v.items) out.push_back(s.string);
  return out;
}

// Compares the JSON C++ gives with the recorded JSON.
std::string compare(const Value& got, const Value& want) {
  const std::string a = json::dump(got);
  const std::string b = json::dump(want);
  if (a == b) return "";
  return "the result differs\nC++:    " + a + "\nPython: " + b;
}

}  // namespace

json::Value script_json(const ls::Script& script) {
  std::vector<Value> regions;
  for (const auto& [name, r] : script.regions) {
    regions.push_back(array({string(name), tagged("Region", {{"name", string(r.name)},
                                                             {"origin", integer(r.origin)},
                                                             {"length", integer(r.length)}})}));
  }
  std::vector<Value> statements;
  for (const ls::Statement& s : script.statements) {
    if (const auto* a = std::get_if<ls::Assign>(&s)) {
      statements.push_back(assign_json(*a));
    } else {
      statements.push_back(section_json(std::get<ls::OutputSection>(s)));
    }
  }
  return tagged("Script", {{"regions", object({{"dict", array(std::move(regions))}})},
                           {"statements", array(std::move(statements))}});
}

ls::Script script_from_json(const json::Value& v) {
  const Value& f = fields(v, "Script");
  ls::Script script;
  for (const Value& pair : field(field(f, "regions"), "dict").items) {
    const Value& r = fields(pair.items.at(1), "Region");
    script.regions.insert_or_assign(
        pair.items.at(0).string,
        ls::Region{field(r, "name").string, field(r, "origin").integer, field(r, "length").integer});
  }
  for (const Value& s : field(f, "statements").items) {
    if (class_of(s) == "Assign") {
      script.statements.emplace_back(assign_from(s));
      continue;
    }
    const Value& g = fields(s, "OutputSection");
    ls::OutputSection sec;
    sec.name = field(g, "name").string;
    sec.address = opt_expr_from(field(g, "address"));
    sec.noload = field(g, "noload").boolean;
    sec.reorder = field(g, "reorder").boolean;
    sec.at = opt_expr_from(field(g, "at"));
    sec.align = opt_expr_from(field(g, "align"));
    for (const Value& c : field(g, "contents").items) {
      if (class_of(c) == "Assign") {
        sec.contents.emplace_back(assign_from(c));
      } else {
        const Value& in = fields(c, "Input");
        sec.contents.emplace_back(
            ls::Input{strings_from(field(in, "patterns")), field(in, "file").string});
      }
    }
    sec.region = opt_string_from(field(g, "region"));
    sec.load_region = opt_string_from(field(g, "load_region"));
    script.statements.emplace_back(std::move(sec));
  }
  return script;
}

bool handles(std::string_view kind) { return kind == "parse_script"; }

std::string check(std::string_view kind, const json::Value& args, const json::Value& input,
                  const json::Value& output, bool is_error) {
  std::string extra;
  Value got;
  try {
    if (kind == "parse_script") {
      got = script_json(ls::parse_script(input.string));
    }
  } catch (const CompileError& e) {
    if (!is_error) return std::string("C++ raised ") + e.python_type() + ": " + e.what();
    if (e.python_type() != field(output, "type").string || e.what() != field(output, "message").string) {
      return std::string("C++ raised ") + e.python_type() + ": " + e.what() + "\nPython raised " +
             field(output, "type").string + ": " + field(output, "message").string;
    }
    return extra;
  }
  if (is_error) {
    return "C++ gave a result where Python raised " + field(output, "type").string + ": " +
           field(output, "message").string;
  }
  if (!extra.empty()) return extra;
  return compare(got, output);
}

}  // namespace c6502::passes::link_checks
