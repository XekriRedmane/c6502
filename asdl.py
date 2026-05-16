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


# --- CLI ---

def main(argv: list[str]) -> int:
    if len(argv) not in (2, 3):
        print("usage: asdl.py <input.asdl> [output.py]", file=sys.stderr)
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
        with open(argv[2], "w", encoding="utf-8") as f:
            f.write(code)
    else:
        sys.stdout.write(code)
    return 0


if __name__ == "__main__":
    raise SystemExit(main(sys.argv))
