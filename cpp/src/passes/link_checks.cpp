#include "passes/link_checks.h"

#include <map>
#include <optional>
#include <utility>
#include <vector>

#include "errors.h"
#include "passes/link_layout.h"

namespace c6502::passes::link_checks {

namespace {

namespace ls = ::c6502::passes::link_script;
namespace ll = ::c6502::passes::link_layout;
namespace elf = ::c6502::binutils::elf;
using json::Value;
using ls::Expr;

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

std::vector<ll::Unit> units_from(const Value& v,
                                 std::map<const elf::Section*, std::pair<int, int>>* where) {
  std::vector<ll::Unit> units;
  int pos = 0;
  for (const Value& u : v.items) {
    ll::Unit unit;
    unit.path = field(u, "path").string;
    unit.index = static_cast<int>(field(u, "index").integer);
    std::vector<elf::SectionPtr> sections;
    int k = 0;
    for (const Value& s : field(u, "sections").items) {
      auto sec = std::make_shared<elf::Section>();
      sec->type = elf::SHT_NOBITS;
      sec->nobits_size = static_cast<std::int64_t>(s.items.at(0).integer);
      sec->flags = static_cast<std::uint32_t>(s.items.at(1).integer);
      if (where != nullptr) where->emplace(sec.get(), std::make_pair(pos, k));
      sections.push_back(std::move(sec));
      ++k;
    }
    const auto& refs = field(u, "item_sections").items;
    const auto& items = field(u, "items").items;
    for (std::size_t i = 0; i < items.size(); ++i) {
      const Value& it = items[i];
      ItemMeta meta{.name = it.items.at(0).string,
                    .section = it.items.at(1).string,
                    .local = it.items.at(2).boolean,
                    .zero = it.items.at(3).boolean,
                    .merge = it.items.at(4).boolean};
      unit.sections.insert_or_assign(meta.name,
                                     sections.at(static_cast<std::size_t>(refs.at(i).integer)));
      unit.items.push_back(std::move(meta));
    }
    for (const Value& t : field(u, "tails").items) {
      unit.tails.insert_or_assign(t.items.at(0).string, t.items.at(1).string);
    }
    units.push_back(std::move(unit));
    ++pos;
  }
  return units;
}

std::string from_hex(std::string_view hex) {
  std::string out;
  const auto nibble = [](char c) { return c <= '9' ? c - '0' : (c | 0x20) - 'a' + 10; };
  for (std::size_t i = 0; i + 1 < hex.size(); i += 2) {
    out += static_cast<char>(nibble(hex[i]) << 4 | nibble(hex[i + 1]));
  }
  return out;
}

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

bool handles(std::string_view kind) {
  return kind == "parse_script" || kind == "lay_out" || kind == "default_script" ||
         kind == "fall_through" || kind == "tail_jump" || kind == "startup_lines";
}

std::string check(std::string_view kind, const json::Value& args, const json::Value& input,
                  const json::Value& output, bool is_error) {
  std::string extra;
  Value got;
  try {
    if (kind == "parse_script") {
      got = script_json(ls::parse_script(input.string));
    } else if (kind == "startup_lines") {
      std::vector<std::string> lines = ll::startup_lines(script_from_json(input));
      got = strings(lines);
    } else if (kind == "default_script") {
      const auto units = units_from(field(input, "units"), nullptr);
      got = script_json(
          ll::default_script(units, static_cast<std::int64_t>(field(args, "origin").integer)));
    } else if (kind == "fall_through") {
      std::vector<ll::Entry> entries;
      for (const Value& e : field(input, "entries").items) {
        ll::Entry entry;
        entry.name = e.items.at(0).string;
        entry.function = e.items.at(1).boolean;
        if (e.items.at(2).kind != Value::Kind::kNull) entry.tail = e.items.at(2).string;
        entry.payload = entries.size();
        entries.push_back(std::move(entry));
      }
      std::vector<Value> out;
      for (const auto& [e, dropped] : ll::fall_through(entries, field(args, "reorder").boolean)) {
        out.push_back(array({integer(static_cast<asdl::Int>(e.payload)), boolean(dropped)}));
      }
      got = array(std::move(out));
    } else if (kind == "tail_jump") {
      auto sec = std::make_shared<elf::Section>();
      sec->type = static_cast<std::uint32_t>(field(input, "type").integer);
      if (sec->type == elf::SHT_NOBITS) {
        sec->nobits_size = static_cast<std::int64_t>(field(input, "size").integer);
      } else {
        sec->data = from_hex(field(input, "data").string);
      }
      for (const Value& r : field(input, "relocs").items) {
        auto sym = std::make_shared<elf::Symbol>();
        sym->name = r.items.at(3).string;
        sec->relocs.push_back(elf::Reloc{static_cast<std::int64_t>(r.items.at(0).integer),
                                         static_cast<std::uint8_t>(r.items.at(1).integer), sym,
                                         static_cast<std::int64_t>(r.items.at(2).integer)});
      }
      std::vector<elf::SymbolPtr> symbols;
      auto other = std::make_shared<elf::Symbol>();
      other->name = "$d";
      other->value = 1 << 20;
      other->section = std::make_shared<elf::Section>();
      symbols.push_back(other);
      for (const Value& s : field(input, "symbols").items) {
        auto sym = std::make_shared<elf::Symbol>();
        sym->name = s.items.at(0).string;
        sym->value = static_cast<std::int64_t>(s.items.at(1).integer);
        sym->type = static_cast<std::uint8_t>(s.items.at(2).integer);
        sym->section = sec;
        symbols.push_back(std::move(sym));
      }
      const auto name = ll::tail_jump(sec, symbols);
      got = name ? string(*name) : null();
    } else if (kind == "lay_out") {
      std::map<const elf::Section*, std::pair<int, int>> where;
      const auto units = units_from(field(input, "units"), &where);
      std::map<std::string, std::optional<std::int64_t>, std::less<>> answers;
      for (const Value& pair : field(input, "lookups").items) {
        const Value& value = pair.items.at(1);
        answers.emplace(pair.items.at(0).string,
                        value.kind == Value::Kind::kNull
                            ? std::nullopt
                            : std::optional<std::int64_t>(static_cast<std::int64_t>(value.integer)));
      }
      const ll::Lookup lookup = [&](const std::string& name) -> std::optional<std::int64_t> {
        const auto it = answers.find(name);
        if (it == answers.end()) {
          extra = "C++ looked up `" + name + "`, which Python didn't";
          return std::nullopt;
        }
        return it->second;
      };
      const ll::Layout layout = ll::lay_out(script_from_json(field(input, "script")), units, lookup);
      std::vector<Value> outputs;
      for (const auto& o : layout.outputs) {
        std::vector<Value> parts;
        for (const auto& p : o.parts) {
          const auto [pos, k] = where.at(p.section.get());
          parts.push_back(array({integer(p.unit), array({integer(pos), integer(k)}), integer(p.addr),
                                 integer(p.size)}));
        }
        outputs.push_back(object({{"name", string(o.name)},
                                  {"addr", integer(o.addr)},
                                  {"load_addr", integer(o.load_addr)},
                                  {"size", integer(o.size)},
                                  {"noload", boolean(o.noload)},
                                  {"executable", boolean(o.executable)},
                                  {"parts", array(std::move(parts))}}));
      }
      std::vector<Value> symbols;
      for (const auto& [name, v] : layout.symbols) symbols.push_back(array({string(name), integer(v)}));
      got = object({{"outputs", array(std::move(outputs))}, {"symbols", array(std::move(symbols))}});
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
