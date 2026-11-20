"""ASDL parser and Python code generator.

Supports a small subset of the Zephyr ASDL grammar:

    module Name { <type-definitions> }
    type = Constructor(field, ...) | Constructor | ...  [attributes (field, ...)]
    type = (field, ...)                                  -- product type
    field = type [? | *] [name]
    comments start with --

Primitive type names map to Python builtins; everything else is treated
as a user-defined type referenced by name.
"""

from __future__ import annotations

import sys
from dataclasses import dataclass, field
from typing import Optional


# --- Parsed representation of an ASDL source file. ---

@dataclass
class Field:
    type: str
    name: Optional[str]
    optional: bool = False
    sequence: bool = False


@dataclass
class Constructor:
    name: str
    fields: list[Field]


@dataclass
class Type:
    name: str
    is_product: bool
    # For products, these are the product's fields.
    # For sums, these are the attributes clause (empty if absent).
    fields: list[Field]
    constructors: list[Constructor]  # empty for products


@dataclass
class Module:
    name: str
    types: list[Type]


# --- Parser ---

class ASDLError(Exception):
    pass


class _Parser:
    def __init__(self, text: str) -> None:
        self.text = text
        self.i = 0
        self.line = 1
        self.col = 1

    def parse(self) -> Module:
        if not self._eat_id("module"):
            self._error("expected 'module'")
        name = self._read_id()
        if name is None:
            self._error("expected module name")
        self._eat("{")
        types: list[Type] = []
        while True:
            self._skip_ws()
            if self._at_end() or self._peek() == "}":
                break
            types.append(self._parse_type())
        self._eat("}")
        self._skip_ws()
        if not self._at_end():
            self._error("trailing input after module")
        return Module(name=name, types=types)

    def _parse_type(self) -> Type:
        name = self._expect_type_name()
        self._eat("=")
        self._skip_ws()
        if self._peek() == "(":
            fields = self._parse_fields()
            return Type(name=name, is_product=True, fields=fields, constructors=[])
        constructors = [self._parse_constructor()]
        while True:
            self._skip_ws()
            if self._peek() != "|":
                break
            self._advance()  # '|'
            constructors.append(self._parse_constructor())
        attributes: list[Field] = []
        if self._eat_id("attributes"):
            attributes = self._parse_fields()
        return Type(
            name=name, is_product=False,
            fields=attributes, constructors=constructors,
        )

    def _parse_constructor(self) -> Constructor:
        name = self._expect_constructor_name()
        self._skip_ws()
        fields: list[Field] = []
        if self._peek() == "(":
            fields = self._parse_fields()
        return Constructor(name=name, fields=fields)

    def _parse_fields(self) -> list[Field]:
        self._eat("(")
        fields: list[Field] = []
        self._skip_ws()
        if self._peek() != ")":
            fields.append(self._parse_field())
            while True:
                self._skip_ws()
                if self._peek() != ",":
                    break
                self._advance()  # ','
                fields.append(self._parse_field())
        self._eat(")")
        return fields

    def _parse_field(self) -> Field:
        type_name = self._expect_type_name(role="field type")
        self._skip_ws()
        optional = False
        sequence = False
        if self._peek() == "?":
            optional = True
            self._advance()
        elif self._peek() == "*":
            sequence = True
            self._advance()
        name = self._read_id()  # optional
        return Field(type=type_name, name=name, optional=optional, sequence=sequence)

    # --- Low-level scanning. ---

    def _at_end(self) -> bool:
        return self.i >= len(self.text)

    def _peek(self, offset: int = 0) -> str:
        j = self.i + offset
        return self.text[j] if j < len(self.text) else ""

    def _advance(self) -> None:
        ch = self._peek()
        if ch == "\n":
            self.line += 1
            self.col = 1
        else:
            self.col += 1
        self.i += 1

    def _skip_ws(self) -> None:
        while not self._at_end():
            ch = self._peek()
            if ch in " \t\r\n":
                self._advance()
            elif ch == "-" and self._peek(1) == "-":
                while not self._at_end() and self._peek() != "\n":
                    self._advance()
            else:
                return

    def _eat(self, ch: str) -> None:
        self._skip_ws()
        if self._peek() != ch:
            self._error(f"expected {ch!r}")
        self._advance()

    def _read_id(self) -> Optional[str]:
        self._skip_ws()
        first = self._peek()
        if not (first.isalpha() or first == "_"):
            return None
        start = self.i
        while not self._at_end() and (self._peek().isalnum() or self._peek() == "_"):
            self._advance()
        return self.text[start : self.i]

    def _expect_type_name(self, role: str = "type name") -> str:
        name = self._read_id()
        if name is None:
            self._error(f"expected {role}")
        if not name[0].islower():
            self._error(
                f"{role} must start with a lowercase letter: {name!r}"
            )
        return name

    def _expect_constructor_name(self) -> str:
        name = self._read_id()
        if name is None:
            self._error("expected constructor name")
        if not name[0].isupper():
            self._error(
                f"constructor name must start with an uppercase letter: {name!r}"
            )
        return name

    def _eat_id(self, expected: str) -> bool:
        # Try to consume the given identifier; rewind if it doesn't match.
        saved = (self.i, self.line, self.col)
        got = self._read_id()
        if got == expected:
            return True
        self.i, self.line, self.col = saved
        return False

    def _error(self, msg: str) -> None:
        raise ASDLError(f"{self.line}:{self.col}: {msg}")


def parse(text: str) -> Module:
    return _Parser(text).parse()


# --- Python code generator ---

_PRIMITIVES = {
    "identifier": "str",
    "string": "str",
    "int": "int",
    "bool": "bool",
    "bytes": "bytes",
    "float": "float",
    "constant": "object",
}

_PRIMITIVE_DEFAULTS = {
    "identifier": "''",
    "string": "''",
    "int": "0",
    "bool": "False",
    "bytes": "b''",
    "float": "0.0",
    "constant": "None",
}


def _py_type(f: Field) -> str:
    if f.type in _PRIMITIVES:
        base = _PRIMITIVES[f.type]
    else:
        base = f"Type_{f.type}"
    if f.sequence:
        return f"list[{base}]"
    if f.optional:
        return f"{base} | None"
    return base


def _field_name(f: Field) -> str:
    return f.name if f.name else f.type


def _constructor_field_line(f: Field) -> str:
    name = _field_name(f)
    ann = _py_type(f)
    if f.sequence:
        return f"    {name}: {ann} = field(default_factory=list)"
    if f.optional:
        return f"    {name}: {ann} = None"
    # Primitives get a default value so call sites that don't care
    # about the field (e.g. a freshly-added `bool is_volatile` on
    # Load / Store) keep working without churning the entire codebase.
    # This matches `_attribute_line`'s convention; the asymmetry that
    # existed previously (defaults only for attributes, not for
    # constructor fields) was incidental.
    default = _PRIMITIVE_DEFAULTS.get(f.type)
    if default is not None:
        return f"    {name}: {ann} = {default}"
    return f"    {name}: {ann}"


def _attribute_line(f: Field) -> str:
    # Attributes live on a kw_only base class. Give primitives a default so
    # subclasses needn't specify them; leave user types required.
    name = _field_name(f)
    ann = _py_type(f)
    if f.sequence:
        return f"    {name}: {ann} = field(default_factory=list)"
    if f.optional:
        return f"    {name}: {ann} = None"
    default = _PRIMITIVE_DEFAULTS.get(f.type)
    if default is not None:
        return f"    {name}: {ann} = {default}"
    return f"    {name}: {ann}"


# A field gets a Python default whenever it's optional, a sequence, or
# a primitive scalar (covered by `_PRIMITIVE_DEFAULTS`). Python
# dataclasses forbid a field-with-default being followed by a field-
# without-default in positional order, so when an ASDL constructor
# mixes the two with defaults appearing first we emit
# `@dataclass(kw_only=True)` to dodge the ordering rule rather than
# silently reordering the user's ASDL fields.
def _has_default(f: Field) -> bool:
    return f.optional or f.sequence or f.type in _PRIMITIVE_DEFAULTS


def _needs_kw_only(fs: list[Field]) -> bool:
    seen_default = False
    for f in fs:
        if _has_default(f):
            seen_default = True
        elif seen_default:
            return True
    return False


def _gen_product_block(t: Type) -> list[str]:
    decorator = (
        "@dataclass(kw_only=True)" if _needs_kw_only(t.fields) else "@dataclass"
    )
    lines = [decorator, f"class Type_{t.name}:"]
    if not t.fields:
        lines.append("    pass")
    else:
        for f in t.fields:
            lines.append(_constructor_field_line(f))
    return lines


def _gen_sum_blocks(t: Type) -> list[list[str]]:
    blocks: list[list[str]] = []
    base: list[str] = []
    if t.fields:
        base.append("@dataclass(kw_only=True)")
        base.append(f"class Type_{t.name}:")
        for f in t.fields:
            base.append(_attribute_line(f))
    else:
        base.append("@dataclass")
        base.append(f"class Type_{t.name}:")
        base.append("    pass")
    blocks.append(base)
    for c in t.constructors:
        decorator = (
            "@dataclass(kw_only=True)"
            if _needs_kw_only(c.fields) else "@dataclass"
        )
        lines = [decorator, f"class {c.name}(Type_{t.name}):"]
        if not c.fields:
            lines.append("    pass")
        else:
            for f in c.fields:
                lines.append(_constructor_field_line(f))
        blocks.append(lines)
    return blocks


def generate(mod: Module, source: str = "<asdl>") -> str:
    header = [
        f"# Generated from {source}. Do not edit.",
        "from __future__ import annotations",
        "",
        "from dataclasses import dataclass, field",
    ]
    blocks: list[list[str]] = []
    for t in mod.types:
        if t.is_product:
            blocks.append(_gen_product_block(t))
        else:
            blocks.extend(_gen_sum_blocks(t))
    body = "\n\n\n".join("\n".join(b) for b in blocks)
    if body:
        return "\n".join(header) + "\n\n\n" + body + "\n"
    return "\n".join(header) + "\n"


# --- The dump format's line breaks ---

# The element types whose sequences the dump format (ir_dump.py and the
# generated C++ readers and writers) writes one element per line. Every
# other sequence goes on one line. The readers ignore whitespace, so
# this only shapes the text, and both writers must agree on it.
DUMP_LINE_TYPES = frozenset({
    "top_level", "instruction", "declaration", "block_item",
})


# --- C++ code generator ---
#
# `generate_cpp` writes a header and a source file for a module. Each
# constructor is a struct with the constructor's name, each sum type is
# `Type_<name>`, a `std::variant` of its constructors' structs, and
# each product type is a struct `Type_<name>`, so the names match the
# Python module's. cpp/README.md describes the representation and the
# reasons for it.

_CPP_PRIMITIVES = {
    "identifier": "std::string",
    "string": "std::string",
    "int": "::c6502::asdl::Int",
    "bool": "bool",
}

_CPP_DEFAULTS = {
    "int": " = 0",
    "bool": " = false",
}

# The primitive readers and writers of `asdl::Reader` and `asdl::Writer`
# (cpp/src/asdl/runtime.h), by ASDL primitive.
_CPP_PRIMITIVE_IO = {
    "identifier": "string",
    "string": "string",
    "int": "int",
    "bool": "bool",
}

_CPP_KEYWORDS = frozenset("""
    alignas alignof and and_eq asm auto bitand bitor bool break case catch
    char char8_t char16_t char32_t class compl concept const consteval
    constexpr constinit const_cast continue co_await co_return co_yield
    decltype default delete do double dynamic_cast else enum explicit
    export extern false float for friend goto if inline int long mutable
    namespace new noexcept not not_eq nullptr operator or or_eq private
    protected public register reinterpret_cast requires return short
    signed sizeof static static_assert static_cast struct switch template
    this thread_local throw true try typedef typeid typename union
    unsigned using virtual void volatile wchar_t while xor xor_eq
""".split())

_RT = "::c6502::asdl"


def cpp_module_name(path: str) -> str:
    """Returns the C++ module (and namespace) name for an .asdl file,
    which is the Python module's name: `asm.asdl` gives `asm_ast`."""
    import os
    stem = os.path.splitext(os.path.basename(path))[0]
    return f"{stem}_ast"


def _cpp_member(f: Field) -> str:
    """Returns the C++ member name of a field. A name that is a C++
    keyword, such as `const`, gets a trailing underscore."""
    name = _field_name(f)
    return name + "_" if name in _CPP_KEYWORDS else name


def _type_sccs(mod: Module) -> list[list[Type]]:
    """Returns the module's types grouped into strongly connected
    components of the graph whose edges run from a type to the types
    of its fields, dependencies first. A type's struct can hold a
    field of a type from an earlier group by value, and holds one from
    its own group in a `Box`."""
    by_name = {t.name: t for t in mod.types}

    def deps(t: Type) -> list[str]:
        out: list[str] = []
        fields = list(t.fields)
        for c in t.constructors:
            fields.extend(c.fields)
        for f in fields:
            if f.type in by_name and f.type not in out:
                out.append(f.type)
        return out

    index: dict[str, int] = {}
    low: dict[str, int] = {}
    on_stack: set[str] = set()
    stack: list[str] = []
    groups: list[list[Type]] = []

    def visit(name: str) -> None:
        index[name] = low[name] = len(index)
        stack.append(name)
        on_stack.add(name)
        for d in deps(by_name[name]):
            if d not in index:
                visit(d)
                low[name] = min(low[name], low[d])
            elif d in on_stack:
                low[name] = min(low[name], index[d])
        if low[name] == index[name]:
            group: list[str] = []
            while True:
                n = stack.pop()
                on_stack.discard(n)
                group.append(n)
                if n == name:
                    break
            order = [t.name for t in mod.types]
            groups.append(sorted(
                (by_name[n] for n in group),
                key=lambda t: order.index(t.name),
            ))

    for t in mod.types:
        if t.name not in index:
            visit(t.name)
    return groups


def _cpp_base_type(f: Field, group: set[str]) -> str:
    """Returns the C++ type of one element of a field: a primitive, a
    `Type_<name>`, or a `Box` of one for a field of a type in the
    struct's own group that isn't a sequence."""
    if f.type in _CPP_PRIMITIVES:
        return _CPP_PRIMITIVES[f.type]
    if f.type in _PRIMITIVES:
        # `bytes`, `float`, and `constant`, which no module uses.
        raise ASDLError(f"the C++ generator has no type for `{f.type}`")
    base = f"Type_{f.type}"
    if f.type in group and not f.sequence:
        return f"{_RT}::Box<{base}>"
    return base


def _cpp_field_type(f: Field, group: set[str]) -> str:
    base = _cpp_base_type(f, group)
    if f.sequence:
        return f"std::vector<{base}>"
    if f.optional:
        return f"std::optional<{base}>"
    return base


def _cpp_struct(name: str, fields: list[Field], group: set[str]) -> list[str]:
    lines = [f"struct {name} {{"]
    for f in fields:
        default = ""
        if not f.optional and not f.sequence:
            default = _CPP_DEFAULTS.get(f.type, "")
        lines.append(
            f"  {_cpp_field_type(f, group)} {_cpp_member(f)}{default};"
        )
    lines.append(f"  bool operator==(const {name}& other) const;")
    lines.append("};")
    return lines


def _cpp_equality(name: str, fields: list[Field]) -> list[str]:
    if not fields:
        return [
            f"inline bool {name}::operator==(const {name}&) const {{",
            "  return true;",
            "}",
        ]
    terms = [
        f"{_cpp_member(f)} == other.{_cpp_member(f)}" for f in fields
    ]
    lines = [f"inline bool {name}::operator==(const {name}& other) const {{"]
    lines.append("  return " + "\n      && ".join(terms) + ";")
    lines.append("}")
    return lines


def _ctor_fields(t: Type, c: Constructor) -> list[Field]:
    """Returns a constructor's fields followed by its sum type's
    attributes, the order of the C++ struct's members and of the
    dump's fields."""
    return list(c.fields) + list(t.fields)


def _cpp_header(mod: Module, source: str, name: str) -> str:
    out: list[str] = [
        f"// Generated from {source} by `asdl.py --cpp`. Do not edit.",
        "#pragma once",
        "",
        "#include <cstdint>",
        "#include <optional>",
        "#include <string>",
        "#include <variant>",
        "#include <vector>",
        "",
        '#include "asdl/runtime.h"',
        "",
        f"namespace c6502::{name} {{",
    ]
    equalities: list[str] = []
    for group_types in _type_sccs(mod):
        group = {t.name for t in group_types}
        out.append("")
        out.append("// " + ", ".join(t.name for t in group_types))
        for t in group_types:
            if t.is_product:
                out.append(f"struct Type_{t.name};")
            else:
                for c in t.constructors:
                    out.append(f"struct {c.name};")
        for t in group_types:
            if not t.is_product:
                alts = ", ".join(c.name for c in t.constructors)
                out.append(f"using Type_{t.name} = std::variant<{alts}>;")
        for t in group_types:
            if t.is_product:
                out.extend(_cpp_struct(f"Type_{t.name}", t.fields, group))
                equalities.extend(_cpp_equality(f"Type_{t.name}", t.fields))
            else:
                for c in t.constructors:
                    fields = _ctor_fields(t, c)
                    out.extend(_cpp_struct(c.name, fields, group))
                    equalities.extend(_cpp_equality(c.name, fields))
    out.append("")
    out.append("// The constructor of a sum type's value, numbered as the")
    out.append("// variant's alternatives are, for a switch.")
    for t in mod.types:
        if t.is_product:
            continue
        out.append(f"enum class Kind_{t.name} : std::uint8_t {{")
        for c in t.constructors:
            out.append(f"  {c.name},")
        out.append("};")
        out.append(
            f"inline Kind_{t.name} kind(const Type_{t.name}& v) noexcept {{"
        )
        out.append(f"  return static_cast<Kind_{t.name}>(v.index());")
        out.append("}")
    out.append("")
    out.extend(equalities)
    out.append("")
    out.append("// The dump format's writers and readers (ir_dump.py describes")
    out.append("// the format). `asdl::dump` and `asdl::load` call them.")
    for t in mod.types:
        out.append(f"void write({_RT}::Writer& w, const Type_{t.name}& v);")
        out.append(
            f"Type_{t.name} read({_RT}::Reader& r, "
            f"{_RT}::Tag<Type_{t.name}>);"
        )
        if not t.is_product:
            for c in t.constructors:
                out.append(f"void write({_RT}::Writer& w, const {c.name}& v);")
                out.append(
                    f"{c.name} read({_RT}::Reader& r, {_RT}::Tag<{c.name}>);"
                )
    out.append("")
    out.append(f"}}  // namespace c6502::{name}")
    return "\n".join(out) + "\n"


def _cpp_write_value(f: Field, expr: str, group: set[str],
                     indent: str) -> list[str]:
    """Returns the statements that write one element `expr` of field
    `f`."""
    if f.type in _CPP_PRIMITIVE_IO:
        return [f"{indent}w.write_{_CPP_PRIMITIVE_IO[f.type]}({expr});"]
    if f.type in group and not f.sequence:
        return [f"{indent}write(w, *{expr});"]
    return [f"{indent}write(w, {expr});"]


def _cpp_write_field(f: Field, expr: str, group: set[str]) -> list[str]:
    if f.sequence:
        lines = "true" if f.type in DUMP_LINE_TYPES else "false"
        out = [f"  w.begin_seq({lines});"]
        out.append(f"  for (const auto& e : {expr}) {{")
        out.append(f"    w.seq_item({lines});")
        out.extend(_cpp_write_value(f, "e", group, "    "))
        out.append("  }")
        out.append(f"  w.end_seq({lines}, {expr}.empty());")
        return out
    if f.optional:
        out = [f"  if ({expr}) {{"]
        out.extend(_cpp_write_value(f, f"(*{expr})", group, "    "))
        out.append("  } else {")
        out.append("    w.write_none();")
        out.append("  }")
        return out
    return _cpp_write_value(f, expr, group, "  ")


def _cpp_read_value(f: Field, group: set[str]) -> str:
    if f.type in _CPP_PRIMITIVE_IO:
        return f"r.read_{_CPP_PRIMITIVE_IO[f.type]}()"
    value = f"read(r, {_RT}::Tag<Type_{f.type}>{{}})"
    if f.type in group and not f.sequence:
        return f"{_RT}::Box<Type_{f.type}>({value})"
    return value


def _cpp_read_field(f: Field, local: str, group: set[str]) -> list[str]:
    ty = _cpp_field_type(f, group)
    if f.sequence:
        return [
            f"  {ty} {local};",
            "  r.begin_seq();",
            "  while (!r.end_seq()) {",
            f"    {local}.push_back({_cpp_read_value(f, group)});",
            "  }",
        ]
    if f.optional:
        return [
            f"  {ty} {local};",
            "  if (!r.read_none()) {",
            f"    {local} = {_cpp_read_value(f, group)};",
            "  }",
        ]
    return [f"  {ty} {local} = {_cpp_read_value(f, group)};"]


def _cpp_body_functions(
    struct: str, dump_name: str, fields: list[Field], group: set[str],
) -> list[str]:
    """Returns the writer of a constructor's (or product's) struct and
    the reader of its fields, which follows its name."""
    param = " v" if fields else ""
    out = [f"void write({_RT}::Writer& w, const {struct}&{param}) {{"]
    out.append(f'  w.write_name("{dump_name}");')
    if fields:
        out.append("  w.open();")
        for i, f in enumerate(fields):
            if i:
                out.append("  w.space();")
            out.extend(_cpp_write_field(f, f"v.{_cpp_member(f)}", group))
        out.append("  w.close();")
    out.append("}")
    out.append("")
    out.append(f"static {struct} read_body_{struct}({_RT}::Reader& r) {{")
    if not fields:
        out.append("  (void)r;")
        out.append(f"  return {struct}{{}};")
    else:
        out.append("  r.open();")
        locals_: list[str] = []
        for i, f in enumerate(fields):
            local = f"f{i}"
            locals_.append(f"std::move({local})")
            out.extend(_cpp_read_field(f, local, group))
        out.append("  r.close();")
        out.append(f"  return {struct}{{{', '.join(locals_)}}};")
    out.append("}")
    out.append("")
    return out


def _cpp_source(mod: Module, source: str, name: str) -> str:
    out: list[str] = [
        f"// Generated from {source} by `asdl.py --cpp`. Do not edit.",
        f'#include "{name}.h"',
        "",
        "#include <utility>",
        "",
        f"namespace c6502::{name} {{",
        "",
    ]
    for group_types in _type_sccs(mod):
        group = {t.name for t in group_types}
        for t in group_types:
            if t.is_product:
                struct = f"Type_{t.name}"
                out.extend(_cpp_body_functions(struct, t.name, t.fields, group))
                out.append(
                    f"{struct} read({_RT}::Reader& r, {_RT}::Tag<{struct}>) {{"
                )
                out.append(f'  r.expect_name("{t.name}");')
                out.append(f"  return read_body_{struct}(r);")
                out.append("}")
                out.append("")
                continue
            for c in t.constructors:
                out.extend(_cpp_body_functions(
                    c.name, c.name, _ctor_fields(t, c), group,
                ))
                out.append(
                    f"{c.name} read({_RT}::Reader& r, {_RT}::Tag<{c.name}>) {{"
                )
                out.append(f'  r.expect_name("{c.name}");')
                out.append(f"  return read_body_{c.name}(r);")
                out.append("}")
                out.append("")
            sum_t = f"Type_{t.name}"
            out.append(f"void write({_RT}::Writer& w, const {sum_t}& v) {{")
            out.append("  switch (v.index()) {")
            for i, c in enumerate(t.constructors):
                out.append(f"    case {i}:")
                out.append(f"      write(w, *std::get_if<{i}>(&v));")
                out.append("      return;")
            out.append("  }")
            out.append("}")
            out.append("")
            out.append(f"{sum_t} read({_RT}::Reader& r, {_RT}::Tag<{sum_t}>) {{")
            out.append("  const std::string_view name = r.read_name();")
            by_len: dict[int, list[str]] = {}
            for c in t.constructors:
                by_len.setdefault(len(c.name), []).append(c.name)
            out.append("  switch (name.size()) {")
            for length in sorted(by_len):
                out.append(f"    case {length}:")
                for cname in by_len[length]:
                    out.append(f'      if (name == "{cname}") {{')
                    out.append(f"        return read_body_{cname}(r);")
                    out.append("      }")
                out.append("      break;")
            out.append("  }")
            out.append(f'  r.unknown_constructor(name, "{t.name}");')
            out.append("}")
            out.append("")
    out.append(f"}}  // namespace c6502::{name}")
    return "\n".join(out) + "\n"


def generate_cpp(mod: Module, source: str, name: str) -> dict[str, str]:
    """Returns the C++ files for module `mod` as a map from file name
    to text: `<name>.h` with the types, their equality, and the dump
    format's declarations, and `<name>.cpp` with the dump format's
    readers and writers. `source` is the .asdl file's name, for the
    header comment."""
    return {
        f"{name}.h": _cpp_header(mod, source, name),
        f"{name}.cpp": _cpp_source(mod, source, name),
    }


def _main_cpp(argv: list[str]) -> int:
    import os
    if len(argv) != 3:
        print("usage: asdl.py --cpp <input.asdl> <output-dir>",
              file=sys.stderr)
        return 2
    path, out_dir = argv[1], argv[2]
    try:
        with open(path, "r", encoding="utf-8") as f:
            module = parse(f.read())
        files = generate_cpp(
            module, os.path.basename(path), cpp_module_name(path),
        )
    except (OSError, ASDLError) as e:
        print(f"{path}: {e}", file=sys.stderr)
        return 1
    for file_name, text in files.items():
        with open(os.path.join(out_dir, file_name), "w",
                  encoding="utf-8", newline="\n") as f:
            f.write(text)
    return 0


# --- CLI ---

def main(argv: list[str]) -> int:
    if len(argv) >= 2 and argv[1] == "--cpp":
        return _main_cpp(argv[1:])
    if len(argv) not in (2, 3):
        print("usage: asdl.py <input.asdl> [output.py]\n"
              "       asdl.py --cpp <input.asdl> <output-dir>",
              file=sys.stderr)
        return 2
    try:
        with open(argv[1], "r", encoding="utf-8") as f:
            source_text = f.read()
        module = parse(source_text)
        code = generate(module, source=argv[1])
    except (OSError, ASDLError) as e:
        print(f"{argv[1]}: {e}", file=sys.stderr)
        return 1
    if len(argv) == 3:
        # LF on every platform, so regenerating on Windows doesn't
        # turn every line of the module into a change.
        with open(argv[2], "w", encoding="utf-8", newline="\n") as f:
            f.write(code)
    else:
        sys.stdout.write(code)
    return 0


if __name__ == "__main__":
    raise SystemExit(main(sys.argv))
