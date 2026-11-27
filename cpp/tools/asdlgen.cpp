#include "asdlgen.h"

#include <algorithm>
#include <cstddef>
#include <functional>
#include <map>
#include <set>

namespace c6502::asdlgen {

namespace {

// --- The parser ---

bool is_alpha(char c) { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'); }
bool is_alnum(char c) { return is_alpha(c) || (c >= '0' && c <= '9'); }

class Parser {
 public:
  explicit Parser(std::string text) : text_(std::move(text)) {}

  Module parse() {
    if (!eat_id("module")) {
      error("expected 'module'");
    }
    std::optional<std::string> name = read_id();
    if (!name) {
      error("expected module name");
    }
    eat('{');
    Module mod{.name = *name, .types = {}};
    while (true) {
      skip_ws();
      if (at_end() || peek() == '}') {
        break;
      }
      mod.types.push_back(parse_type());
    }
    eat('}');
    skip_ws();
    if (!at_end()) {
      error("trailing input after module");
    }
    return mod;
  }

 private:
  Type parse_type() {
    std::string name = expect_type_name("type name");
    eat('=');
    skip_ws();
    if (peek() == '(') {
      return Type{.name = name, .is_product = true, .fields = parse_fields(), .constructors = {}};
    }
    std::vector<Constructor> constructors{parse_constructor()};
    while (true) {
      skip_ws();
      if (peek() != '|') {
        break;
      }
      advance();
      constructors.push_back(parse_constructor());
    }
    std::vector<Field> attributes;
    if (eat_id("attributes")) {
      attributes = parse_fields();
    }
    return Type{.name = name,
                .is_product = false,
                .fields = std::move(attributes),
                .constructors = std::move(constructors)};
  }

  Constructor parse_constructor() {
    std::string name = expect_constructor_name();
    skip_ws();
    std::vector<Field> fields;
    if (peek() == '(') {
      fields = parse_fields();
    }
    return Constructor{.name = name, .fields = std::move(fields)};
  }

  std::vector<Field> parse_fields() {
    eat('(');
    std::vector<Field> fields;
    skip_ws();
    if (peek() != ')') {
      fields.push_back(parse_field());
      while (true) {
        skip_ws();
        if (peek() != ',') {
          break;
        }
        advance();
        fields.push_back(parse_field());
      }
    }
    eat(')');
    return fields;
  }

  Field parse_field() {
    Field f;
    f.type = expect_type_name("field type");
    skip_ws();
    if (peek() == '?') {
      f.optional = true;
      advance();
    } else if (peek() == '*') {
      f.sequence = true;
      advance();
    }
    f.name = read_id();
    return f;
  }

  bool at_end() const { return i_ >= text_.size(); }

  // NUL at the end of the text, which no .asdl file holds.
  char peek(std::size_t offset = 0) const {
    const std::size_t j = i_ + offset;
    return j < text_.size() ? text_[j] : '\0';
  }

  void advance() {
    if (peek() == '\n') {
      ++line_;
      col_ = 1;
    } else {
      ++col_;
    }
    ++i_;
  }

  void skip_ws() {
    while (!at_end()) {
      const char ch = peek();
      if (ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n') {
        advance();
      } else if (ch == '-' && peek(1) == '-') {
        while (!at_end() && peek() != '\n') {
          advance();
        }
      } else {
        return;
      }
    }
  }

  void eat(char ch) {
    skip_ws();
    if (peek() != ch) {
      error(std::string("expected '") + ch + "'");
    }
    advance();
  }

  std::optional<std::string> read_id() {
    skip_ws();
    const char first = peek();
    if (!(is_alpha(first) || first == '_')) {
      return std::nullopt;
    }
    const std::size_t start = i_;
    while (!at_end() && (is_alnum(peek()) || peek() == '_')) {
      advance();
    }
    return text_.substr(start, i_ - start);
  }

  std::string expect_type_name(const std::string& role) {
    std::optional<std::string> name = read_id();
    if (!name) {
      error("expected " + role);
    }
    if (!((*name)[0] >= 'a' && (*name)[0] <= 'z')) {
      error(role + " must start with a lowercase letter: '" + *name + "'");
    }
    return *name;
  }

  std::string expect_constructor_name() {
    std::optional<std::string> name = read_id();
    if (!name) {
      error("expected constructor name");
    }
    if (!((*name)[0] >= 'A' && (*name)[0] <= 'Z')) {
      error("constructor name must start with an uppercase letter: '" + *name + "'");
    }
    return *name;
  }

  // Takes the identifier `expected`, or leaves the position where it was.
  bool eat_id(std::string_view expected) {
    const std::size_t i = i_;
    const int line = line_;
    const int col = col_;
    if (read_id() == expected) {
      return true;
    }
    i_ = i;
    line_ = line;
    col_ = col;
    return false;
  }

  [[noreturn]] void error(const std::string& msg) const {
    throw Error(std::to_string(line_) + ":" + std::to_string(col_) + ": " + msg);
  }

  std::string text_;
  std::size_t i_ = 0;
  int line_ = 1;
  int col_ = 1;
};

// Reads CR LF and a lone CR as LF, as Python's text files do, so that a
// parse error gives the line and column asdl.py gives.
std::string universal_newlines(std::string_view text) {
  std::string out;
  out.reserve(text.size());
  for (std::size_t i = 0; i < text.size(); ++i) {
    if (text[i] == '\r') {
      out += '\n';
      if (i + 1 < text.size() && text[i + 1] == '\n') {
        ++i;
      }
    } else {
      out += text[i];
    }
  }
  return out;
}

// --- The C++ generator ---
//
// Each constructor is a struct with the constructor's name, each sum
// type is `Type_<name>`, a `std::variant` of its constructors' structs,
// and each product type is a struct `Type_<name>`, so the names match
// the Python module's. cpp/README.md describes the representation and
// the reasons for it.

using Lines = std::vector<std::string>;
using Group = std::set<std::string>;

const std::map<std::string, std::string, std::less<>>& cpp_primitives() {
  static const std::map<std::string, std::string, std::less<>> m{
      {"identifier", "std::string"},
      {"string", "std::string"},
      {"int", "::c6502::asdl::Int"},
      {"bool", "bool"},
  };
  return m;
}

// The ASDL primitives asdl.py gives a Python type and the C++ generator
// gives none.
bool python_only_primitive(std::string_view type) {
  return type == "bytes" || type == "float" || type == "constant";
}

// The element types whose sequences the dump format writes one element
// to a line, as `asdl.DUMP_LINE_TYPES` gives them for ir_dump.py. Every
// other sequence goes on one line.
bool dump_line_type(std::string_view type) {
  return type == "top_level" || type == "instruction" || type == "declaration" ||
         type == "block_item";
}

bool is_cpp_keyword(std::string_view name) {
  static const std::set<std::string, std::less<>> keywords{
      "alignas",      "alignof",      "and",       "and_eq",      "asm",
      "auto",         "bitand",       "bitor",     "bool",        "break",
      "case",         "catch",        "char",      "char8_t",     "char16_t",
      "char32_t",     "class",        "compl",     "concept",     "const",
      "consteval",    "constexpr",    "constinit", "const_cast",  "continue",
      "co_await",     "co_return",    "co_yield",  "decltype",    "default",
      "delete",       "do",           "double",    "dynamic_cast", "else",
      "enum",         "explicit",     "export",    "extern",      "false",
      "float",        "for",          "friend",    "goto",        "if",
      "inline",       "int",          "long",      "mutable",     "namespace",
      "new",          "noexcept",     "not",       "not_eq",      "nullptr",
      "operator",     "or",           "or_eq",     "private",     "protected",
      "public",       "register",     "reinterpret_cast", "requires", "return",
      "short",        "signed",       "sizeof",    "static",      "static_assert",
      "static_cast",  "struct",       "switch",    "template",    "this",
      "thread_local", "throw",        "true",      "try",         "typedef",
      "typeid",       "typename",     "union",     "unsigned",    "using",
      "virtual",      "void",         "volatile",  "wchar_t",     "while",
      "xor",          "xor_eq",
  };
  return keywords.contains(name);
}

const std::string kRt = "::c6502::asdl";

std::string field_name(const Field& f) { return f.name ? *f.name : f.type; }

// A field named after a C++ keyword, such as `const`, gets a trailing
// underscore.
std::string cpp_member(const Field& f) {
  std::string name = field_name(f);
  return is_cpp_keyword(name) ? name + "_" : name;
}

std::string join(const Lines& items, std::string_view sep) {
  std::string out;
  for (std::size_t i = 0; i < items.size(); ++i) {
    if (i) out += sep;
    out += items[i];
  }
  return out;
}

void extend(Lines& out, const Lines& more) { out.insert(out.end(), more.begin(), more.end()); }

std::string text_of(const Lines& lines) { return join(lines, "\n") + "\n"; }

// Returns the module's types grouped into the strongly connected
// components of the graph whose edges run from a type to the types of
// its fields, dependencies first, each group in the module's order. A
// type's struct can hold a field of a type from an earlier group by
// value, and holds one from its own group in a `Box`.
std::vector<std::vector<const Type*>> type_sccs(const Module& mod) {
  std::map<std::string, const Type*, std::less<>> by_name;
  std::map<std::string, std::size_t, std::less<>> order;
  for (std::size_t k = 0; k < mod.types.size(); ++k) {
    by_name[mod.types[k].name] = &mod.types[k];
    order[mod.types[k].name] = k;
  }
  const auto deps = [&](const Type& t) {
    std::vector<std::string> out;
    std::vector<const Field*> fields;
    for (const Field& f : t.fields) fields.push_back(&f);
    for (const Constructor& c : t.constructors) {
      for (const Field& f : c.fields) fields.push_back(&f);
    }
    for (const Field* f : fields) {
      if (by_name.contains(f->type) &&
          std::find(out.begin(), out.end(), f->type) == out.end()) {
        out.push_back(f->type);
      }
    }
    return out;
  };

  std::map<std::string, std::size_t, std::less<>> index;
  std::map<std::string, std::size_t, std::less<>> low;
  std::set<std::string, std::less<>> on_stack;
  std::vector<std::string> stack;
  std::vector<std::vector<const Type*>> groups;

  const std::function<void(const std::string&)> visit = [&](const std::string& name) {
    const std::size_t n = index.size();
    index[name] = n;
    low[name] = n;
    stack.push_back(name);
    on_stack.insert(name);
    for (const std::string& d : deps(*by_name.at(name))) {
      if (!index.contains(d)) {
        visit(d);
        low[name] = std::min(low[name], low[d]);
      } else if (on_stack.contains(d)) {
        low[name] = std::min(low[name], index[d]);
      }
    }
    if (low[name] == index[name]) {
      std::vector<const Type*> group;
      while (true) {
        std::string top = stack.back();
        stack.pop_back();
        on_stack.erase(top);
        group.push_back(by_name.at(top));
        if (top == name) {
          break;
        }
      }
      std::sort(group.begin(), group.end(), [&](const Type* a, const Type* b) {
        return order.at(a->name) < order.at(b->name);
      });
      groups.push_back(std::move(group));
    }
  };

  for (const Type& t : mod.types) {
    if (!index.contains(t.name)) {
      visit(t.name);
    }
  }
  return groups;
}

Group group_names(const std::vector<const Type*>& types) {
  Group group;
  for (const Type* t : types) group.insert(t->name);
  return group;
}

bool boxed(const Field& f, const Group& group) {
  return group.contains(f.type) && !f.sequence;
}

// Returns the C++ type of one element of a field: a primitive, a
// `Type_<name>`, or a `Box` of one for a field of a type in the
// struct's own group that isn't a sequence.
std::string cpp_base_type(const Field& f, const Group& group) {
  if (const auto it = cpp_primitives().find(f.type); it != cpp_primitives().end()) {
    return it->second;
  }
  if (python_only_primitive(f.type)) {
    throw Error("the C++ generator has no type for `" + f.type + "`");
  }
  const std::string base = "Type_" + f.type;
  if (boxed(f, group)) {
    return kRt + "::Box<" + base + ">";
  }
  return base;
}

std::string cpp_field_type(const Field& f, const Group& group) {
  const std::string base = cpp_base_type(f, group);
  if (f.sequence) return "std::vector<" + base + ">";
  if (f.optional) return "std::optional<" + base + ">";
  return base;
}

Lines cpp_struct(const std::string& name, const std::vector<Field>& fields,
                 const Group& group) {
  Lines lines{"struct " + name + " {"};
  for (const Field& f : fields) {
    std::string def;
    if (!f.optional && !f.sequence) {
      if (f.type == "int") def = " = 0";
      if (f.type == "bool") def = " = false";
    }
    lines.push_back("  " + cpp_field_type(f, group) + " " + cpp_member(f) + def + ";");
  }
  lines.push_back("  bool operator==(const " + name + "& other) const;");
  lines.push_back("};");
  return lines;
}

Lines cpp_equality(const std::string& name, const std::vector<Field>& fields) {
  if (fields.empty()) {
    return {"inline bool " + name + "::operator==(const " + name + "&) const {",
            "  return true;", "}"};
  }
  Lines terms;
  for (const Field& f : fields) {
    terms.push_back(cpp_member(f) + " == other." + cpp_member(f));
  }
  return {"inline bool " + name + "::operator==(const " + name + "& other) const {",
          "  return " + join(terms, "\n      && ") + ";", "}"};
}

// A constructor's fields followed by its sum type's attributes, the
// order of the C++ struct's members and of the dump's fields.
std::vector<Field> ctor_fields(const Type& t, const Constructor& c) {
  std::vector<Field> out = c.fields;
  out.insert(out.end(), t.fields.begin(), t.fields.end());
  return out;
}

// A constructor's fields in the order `dataclasses.fields` gives them
// for its Python class: the sum type's attributes, which live on the
// base class, and then the constructor's own fields.
std::vector<Field> py_fields(const Type& t, const Constructor& c) {
  std::vector<Field> out = t.fields;
  out.insert(out.end(), c.fields.begin(), c.fields.end());
  return out;
}

std::string cpp_header(const Module& mod, std::string_view source, std::string_view name) {
  const std::string ns(name);
  Lines out{
      "// Generated from " + std::string(source) + " by `asdl.py --cpp`. Do not edit.",
      "#pragma once",
      "",
      "#include <cstdint>",
      "#include <optional>",
      "#include <string>",
      "#include <variant>",
      "#include <vector>",
      "",
      "#include \"asdl/runtime.h\"",
      "",
      "namespace c6502::" + ns + " {",
  };
  Lines equalities;
  for (const std::vector<const Type*>& group_types : type_sccs(mod)) {
    const Group group = group_names(group_types);
    out.push_back("");
    Lines names;
    for (const Type* t : group_types) names.push_back(t->name);
    out.push_back("// " + join(names, ", "));
    for (const Type* t : group_types) {
      if (t->is_product) {
        out.push_back("struct Type_" + t->name + ";");
      } else {
        for (const Constructor& c : t->constructors) out.push_back("struct " + c.name + ";");
      }
    }
    for (const Type* t : group_types) {
      if (!t->is_product) {
        Lines alts;
        for (const Constructor& c : t->constructors) alts.push_back(c.name);
        out.push_back("using Type_" + t->name + " = std::variant<" + join(alts, ", ") + ">;");
      }
    }
    for (const Type* t : group_types) {
      if (t->is_product) {
        extend(out, cpp_struct("Type_" + t->name, t->fields, group));
        extend(equalities, cpp_equality("Type_" + t->name, t->fields));
      } else {
        for (const Constructor& c : t->constructors) {
          const std::vector<Field> fields = ctor_fields(*t, c);
          extend(out, cpp_struct(c.name, fields, group));
          extend(equalities, cpp_equality(c.name, fields));
        }
      }
    }
  }
  out.push_back("");
  out.push_back("// The constructor of a sum type's value, numbered as the");
  out.push_back("// variant's alternatives are, for a switch.");
  for (const Type& t : mod.types) {
    if (t.is_product) continue;
    out.push_back("enum class Kind_" + t.name + " : std::uint8_t {");
    for (const Constructor& c : t.constructors) out.push_back("  " + c.name + ",");
    out.push_back("};");
    out.push_back("inline Kind_" + t.name + " kind(const Type_" + t.name + "& v) noexcept {");
    out.push_back("  return static_cast<Kind_" + t.name + ">(v.index());");
    out.push_back("}");
  }
  out.push_back("");
  extend(out, equalities);
  out.push_back("");
  out.push_back("// The dump format's writers and readers (ir_dump.py describes");
  out.push_back("// the format). `asdl::dump` and `asdl::load` call them.");
  for (const Type& t : mod.types) {
    out.push_back("void write(" + kRt + "::Writer& w, const Type_" + t.name + "& v);");
    out.push_back("Type_" + t.name + " read(" + kRt + "::Reader& r, " + kRt + "::Tag<Type_" +
                  t.name + ">);");
    if (!t.is_product) {
      for (const Constructor& c : t.constructors) {
        out.push_back("void write(" + kRt + "::Writer& w, const " + c.name + "& v);");
        out.push_back(c.name + " read(" + kRt + "::Reader& r, " + kRt + "::Tag<" + c.name +
                      ">);");
      }
    }
  }
  out.push_back("");
  out.push_back("// Python's `repr` of each value, the dataclass's: the class name");
  out.push_back("// and each field as `name=value`. `asdl::py_repr` returns it.");
  for (const Type& t : mod.types) {
    out.push_back("void repr(std::string& out, const Type_" + t.name + "& v);");
    if (!t.is_product) {
      for (const Constructor& c : t.constructors) {
        out.push_back("void repr(std::string& out, const " + c.name + "& v);");
      }
    }
  }
  out.push_back("");
  out.push_back("// pretty.py's text of each value at the indentation level `depth`:");
  out.push_back("// the class name, a `name=value,` line for each field one level");
  out.push_back("// deeper, lists one item to a line, and Python's `repr` of each leaf.");
  out.push_back("// `asdl::py_pretty` returns it.");
  for (const Type& t : mod.types) {
    out.push_back("void pretty(std::string& out, const Type_" + t.name + "& v, int depth);");
    if (!t.is_product) {
      for (const Constructor& c : t.constructors) {
        out.push_back("void pretty(std::string& out, const " + c.name + "& v, int depth);");
      }
    }
  }
  out.push_back("");
  out.push_back("}  // namespace c6502::" + ns);
  return text_of(out);
}

std::string primitive_io(std::string_view type) {
  if (type == "identifier" || type == "string") return "string";
  return std::string(type);
}

bool has_primitive_io(std::string_view type) {
  return type == "identifier" || type == "string" || type == "int" || type == "bool";
}

// The statements that write one element `expr` of field `f`.
Lines cpp_write_value(const Field& f, const std::string& expr, const Group& group,
                      const std::string& indent) {
  if (has_primitive_io(f.type)) {
    return {indent + "w.write_" + primitive_io(f.type) + "(" + expr + ");"};
  }
  if (boxed(f, group)) {
    return {indent + "write(w, *" + expr + ");"};
  }
  return {indent + "write(w, " + expr + ");"};
}

Lines cpp_write_field(const Field& f, const std::string& expr, const Group& group) {
  if (f.sequence) {
    const std::string lines = dump_line_type(f.type) ? "true" : "false";
    Lines out{"  w.begin_seq(" + lines + ");"};
    out.push_back("  for (const auto& e : " + expr + ") {");
    out.push_back("    w.seq_item(" + lines + ");");
    extend(out, cpp_write_value(f, "e", group, "    "));
    out.push_back("  }");
    out.push_back("  w.end_seq(" + lines + ", " + expr + ".empty());");
    return out;
  }
  if (f.optional) {
    Lines out{"  if (" + expr + ") {"};
    extend(out, cpp_write_value(f, "(*" + expr + ")", group, "    "));
    out.push_back("  } else {");
    out.push_back("    w.write_none();");
    out.push_back("  }");
    return out;
  }
  return cpp_write_value(f, expr, group, "  ");
}

std::string cpp_read_value(const Field& f, const Group& group) {
  if (has_primitive_io(f.type)) {
    return "r.read_" + primitive_io(f.type) + "()";
  }
  const std::string value = "read(r, " + kRt + "::Tag<Type_" + f.type + ">{})";
  if (boxed(f, group)) {
    return kRt + "::Box<Type_" + f.type + ">(" + value + ")";
  }
  return value;
}

Lines cpp_read_field(const Field& f, const std::string& local, const Group& group) {
  const std::string ty = cpp_field_type(f, group);
  if (f.sequence) {
    return {"  " + ty + " " + local + ";", "  r.begin_seq();", "  while (!r.end_seq()) {",
            "    " + local + ".push_back(" + cpp_read_value(f, group) + ");", "  }"};
  }
  if (f.optional) {
    return {"  " + ty + " " + local + ";", "  if (!r.read_none()) {",
            "    " + local + " = " + cpp_read_value(f, group) + ";", "  }"};
  }
  return {"  " + ty + " " + local + " = " + cpp_read_value(f, group) + ";"};
}

// The writer of a constructor's (or product's) struct and the reader of
// its fields, which follows its name.
Lines cpp_body_functions(const std::string& name, const std::string& dump_name,
                         const std::vector<Field>& fields, const Group& group) {
  const std::string param = fields.empty() ? "" : " v";
  Lines out{"void write(" + kRt + "::Writer& w, const " + name + "&" + param + ") {"};
  out.push_back("  w.write_name(\"" + dump_name + "\");");
  if (!fields.empty()) {
    out.push_back("  w.open();");
    for (std::size_t i = 0; i < fields.size(); ++i) {
      if (i) out.push_back("  w.space();");
      extend(out, cpp_write_field(fields[i], "v." + cpp_member(fields[i]), group));
    }
    out.push_back("  w.close();");
  }
  out.push_back("}");
  out.push_back("");
  out.push_back("static " + name + " read_body_" + name + "(" + kRt + "::Reader& r) {");
  if (fields.empty()) {
    out.push_back("  (void)r;");
    out.push_back("  return " + name + "{};");
  } else {
    out.push_back("  r.open();");
    Lines locals;
    for (std::size_t i = 0; i < fields.size(); ++i) {
      const std::string local = "f" + std::to_string(i);
      locals.push_back("std::move(" + local + ")");
      extend(out, cpp_read_field(fields[i], local, group));
    }
    out.push_back("  r.close();");
    out.push_back("  return " + name + "{" + join(locals, ", ") + "};");
  }
  out.push_back("}");
  out.push_back("");
  return out;
}

// The statements that append Python's `repr` of one element `expr` of
// field `f` to `out`.
Lines cpp_repr_value(const Field& f, const std::string& expr, const Group& group,
                     const std::string& indent) {
  if (f.type == "identifier" || f.type == "string") {
    return {indent + "out += ::c6502::pyrepr::string(" + expr + ");"};
  }
  if (f.type == "int") return {indent + "out += " + kRt + "::int_to_string(" + expr + ");"};
  if (f.type == "bool") return {indent + "out += " + expr + " ? \"True\" : \"False\";"};
  if (boxed(f, group)) return {indent + "repr(out, *" + expr + ");"};
  return {indent + "repr(out, " + expr + ");"};
}

Lines cpp_repr_field(const Field& f, const std::string& expr, const Group& group) {
  if (f.sequence) {
    Lines out{"  out += \"[\";", "  {", "    bool first = true;"};
    out.push_back("    for (const auto& e : " + expr + ") {");
    out.push_back("      if (!first) out += \", \";");
    out.push_back("      first = false;");
    extend(out, cpp_repr_value(f, "e", group, "      "));
    out.push_back("    }");
    out.push_back("  }");
    out.push_back("  out += \"]\";");
    return out;
  }
  if (f.optional) {
    Lines out{"  if (" + expr + ") {"};
    extend(out, cpp_repr_value(f, "(*" + expr + ")", group, "    "));
    out.push_back("  } else {");
    out.push_back("    out += \"None\";");
    out.push_back("  }");
    return out;
  }
  return cpp_repr_value(f, expr, group, "  ");
}

// The function that appends the Python dataclass's `repr` of a
// constructor's (or product's) struct to `out`: the class name and each
// field as `name=value`, in the order of the fields.
Lines cpp_repr_function(const std::string& name, const std::vector<Field>& fields,
                        const Group& group) {
  const std::string param = fields.empty() ? "" : " v";
  Lines out{"void repr(std::string& out, const " + name + "&" + param + ") {"};
  out.push_back("  out += \"" + name + "(\";");
  for (std::size_t i = 0; i < fields.size(); ++i) {
    const std::string sep = i ? ", " : "";
    out.push_back("  out += \"" + sep + field_name(fields[i]) + "=\";");
    extend(out, cpp_repr_field(fields[i], "v." + cpp_member(fields[i]), group));
  }
  out.push_back("  out += \")\";");
  out.push_back("}");
  out.push_back("");
  return out;
}

// The statements that append pretty.py's text of one element `expr` of
// field `f`, at the indentation level `depth`, to `out`. A leaf is
// Python's `repr` of its value, and a string's is `pcpp::py_repr`,
// which escapes every code point that `str.isprintable()` rejects.
Lines cpp_pretty_value(const Field& f, const std::string& expr, const Group& group,
                       const std::string& indent, const std::string& depth) {
  if (f.type == "identifier" || f.type == "string") {
    return {indent + "out += ::c6502::pcpp::py_repr(" + expr + ");"};
  }
  if (f.type == "int") return {indent + "out += " + kRt + "::int_to_string(" + expr + ");"};
  if (f.type == "bool") return {indent + "out += " + expr + " ? \"True\" : \"False\";"};
  if (boxed(f, group)) return {indent + "pretty(out, *" + expr + ", " + depth + ");"};
  return {indent + "pretty(out, " + expr + ", " + depth + ");"};
}

// The statements that append pretty.py's text of field `f`, whose
// `name=` is at the level `depth + 1`. A list that isn't empty has one
// item to a line, each followed by a comma, at the level `depth + 2`,
// and its `]` on a line of its own at the field's level.
Lines cpp_pretty_field(const Field& f, const std::string& expr, const Group& group) {
  if (f.sequence) {
    Lines out{"  if (" + expr + ".empty()) {", "    out += \"[]\";", "  } else {"};
    out.push_back("    out += \"[\";");
    out.push_back("    for (const auto& e : " + expr + ") {");
    out.push_back("      newline(out, depth + 2);");
    extend(out, cpp_pretty_value(f, "e", group, "      ", "depth + 2"));
    out.push_back("      out += ',';");
    out.push_back("    }");
    out.push_back("    newline(out, depth + 1);");
    out.push_back("    out += ']';");
    out.push_back("  }");
    return out;
  }
  if (f.optional) {
    Lines out{"  if (" + expr + ") {"};
    extend(out, cpp_pretty_value(f, "(*" + expr + ")", group, "    ", "depth + 1"));
    out.push_back("  } else {");
    out.push_back("    out += \"None\";");
    out.push_back("  }");
    return out;
  }
  return cpp_pretty_value(f, expr, group, "  ", "depth + 1");
}

// The function that appends pretty.py's text of a constructor's (or
// product's) struct at the indentation level `depth` to `out`: `Name()`
// for one without fields, and otherwise `Name(`, a `name=value,` line
// for each field one level deeper, and `)` on a line of its own at
// `depth`.
Lines cpp_pretty_function(const std::string& name, const std::vector<Field>& fields,
                          const Group& group) {
  if (fields.empty()) {
    return {"void pretty(std::string& out, const " + name + "&, int) {",
            "  out += \"" + name + "()\";", "}", ""};
  }
  Lines out{"void pretty(std::string& out, const " + name + "& v, int depth) {"};
  out.push_back("  out += \"" + name + "(\";");
  for (const Field& f : fields) {
    out.push_back("  newline(out, depth + 1);");
    out.push_back("  out += \"" + field_name(f) + "=\";");
    extend(out, cpp_pretty_field(f, "v." + cpp_member(f), group));
    out.push_back("  out += ',';");
  }
  out.push_back("  newline(out, depth);");
  out.push_back("  out += ')';");
  out.push_back("}");
  out.push_back("");
  return out;
}

Lines read_by_name(const std::string& name, const std::string& dump_name) {
  return {name + " read(" + kRt + "::Reader& r, " + kRt + "::Tag<" + name + ">) {",
          "  r.expect_name(\"" + dump_name + "\");", "  return read_body_" + name + "(r);", "}",
          ""};
}

// The writer, `repr`, or `pretty` of a sum type, which switches on the
// variant's index and calls the alternative's function, `before`
// value `after`.
Lines index_switch(const std::string& head, const Type& t, const std::string& before,
                   const std::string& after) {
  Lines out{head, "  switch (v.index()) {"};
  for (std::size_t i = 0; i < t.constructors.size(); ++i) {
    out.push_back("    case " + std::to_string(i) + ":");
    out.push_back("      " + before + "*std::get_if<" + std::to_string(i) + ">(&v)" + after);
    out.push_back("      return;");
  }
  out.push_back("  }");
  out.push_back("}");
  out.push_back("");
  return out;
}

std::string cpp_source(const Module& mod, std::string_view source, std::string_view name) {
  const std::string ns(name);
  Lines out{
      "// Generated from " + std::string(source) + " by `asdl.py --cpp`. Do not edit.",
      "#include \"" + ns + ".h\"",
      "",
      "#include <utility>",
      "",
      "#include \"pcpp_text.h\"",
      "#include \"pyrepr.h\"",
      "",
      "namespace c6502::" + ns + " {",
      "",
      "// Starts a line of pretty.py's text at the indentation level",
      "// `depth`, two spaces to a level.",
      "[[maybe_unused]] static void newline(std::string& out, int depth) {",
      "  out += '\\n';",
      "  out.append(static_cast<std::size_t>(2 * depth), ' ');",
      "}",
      "",
  };
  for (const std::vector<const Type*>& group_types : type_sccs(mod)) {
    const Group group = group_names(group_types);
    for (const Type* t : group_types) {
      if (t->is_product) {
        const std::string name_t = "Type_" + t->name;
        extend(out, cpp_body_functions(name_t, t->name, t->fields, group));
        extend(out, cpp_repr_function(name_t, t->fields, group));
        extend(out, cpp_pretty_function(name_t, t->fields, group));
        extend(out, read_by_name(name_t, t->name));
        continue;
      }
      for (const Constructor& c : t->constructors) {
        extend(out, cpp_body_functions(c.name, c.name, ctor_fields(*t, c), group));
        extend(out, cpp_repr_function(c.name, ctor_fields(*t, c), group));
        extend(out, cpp_pretty_function(c.name, py_fields(*t, c), group));
        extend(out, read_by_name(c.name, c.name));
      }
      const std::string sum_t = "Type_" + t->name;
      extend(out, index_switch("void write(" + kRt + "::Writer& w, const " + sum_t + "& v) {",
                               *t, "write(w, ", ");"));
      extend(out, index_switch("void repr(std::string& out, const " + sum_t + "& v) {", *t,
                               "repr(out, ", ");"));
      extend(out, index_switch("void pretty(std::string& out, const " + sum_t +
                                   "& v, int depth) {",
                               *t, "pretty(out, ", ", depth);"));
      out.push_back(sum_t + " read(" + kRt + "::Reader& r, " + kRt + "::Tag<" + sum_t + ">) {");
      out.push_back("  const std::string_view name = r.read_name();");
      std::map<std::size_t, Lines> by_len;
      for (const Constructor& c : t->constructors) by_len[c.name.size()].push_back(c.name);
      out.push_back("  switch (name.size()) {");
      for (const auto& [length, names] : by_len) {
        out.push_back("    case " + std::to_string(length) + ":");
        for (const std::string& cname : names) {
          out.push_back("      if (name == \"" + cname + "\") {");
          out.push_back("        return read_body_" + cname + "(r);");
          out.push_back("      }");
        }
        out.push_back("      break;");
      }
      out.push_back("  }");
      out.push_back("  r.unknown_constructor(name, \"" + t->name + "\");");
      out.push_back("}");
      out.push_back("");
    }
  }
  out.push_back("}  // namespace c6502::" + ns);
  return text_of(out);
}

}  // namespace

Module parse(std::string_view text) { return Parser(universal_newlines(text)).parse(); }

std::string cpp_module_name(std::string_view path) {
  std::string_view base = path;
  if (const std::size_t slash = base.find_last_of("/\\"); slash != std::string_view::npos) {
    base = base.substr(slash + 1);
  }
  // `os.path.splitext` keeps a leading dot with the stem, as in `.asdl`.
  const std::size_t dot = base.find_last_of('.');
  std::string_view stem = base;
  if (dot != std::string_view::npos && base.find_first_not_of('.') < dot) {
    stem = base.substr(0, dot);
  }
  return std::string(stem) + "_ast";
}

std::vector<std::pair<std::string, std::string>> generate_cpp(const Module& mod,
                                                               std::string_view source,
                                                               std::string_view name) {
  const std::string n(name);
  return {{n + ".h", cpp_header(mod, source, name)}, {n + ".cpp", cpp_source(mod, source, name)}};
}

}  // namespace c6502::asdlgen
