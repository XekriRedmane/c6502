"""The dump format: a canonical text form of any value of the four
ASDL-defined IRs, which the C++ port reads and writes too.

`dump(value)` returns the text and `load(text, module)` returns the
value, equal (`==`) to the one dumped. The C++ side's readers and
writers are generated from the same .asdl files by `asdl.py --cpp`
(cpp/src/asdl/runtime.h and cpp/generated/), and a value dumped by
either side and read by the other is dumped again as the same bytes.
This module builds its readers and writers from the .asdl files when
it is first used on a module, so the two sides follow the same schema.

The grammar, with `|` for alternatives:

    value   = ctor | ctor "(" fields ")"       a sum type's value
            | name "(" fields ")"              a product type's value
            | int | bool | string | "None" | seq
    fields  = value (" " value)*               in the .asdl's field order
    seq     = "[" "]" | "[" value (" " value)* "]"
    int     = -?[0-9]+                         any size
    bool    = "true" | "false"
    string  = '"' ... '"'

A constructor with no fields is its bare name (`IndirectY`), and one
with fields always has parentheses. A product type's value starts with
the type's name (`var_decl(...)`), so every value names its type and
`load` needs only the module. A sum type's attributes, which no module
uses, follow the constructor's own fields. An optional field holds
`None` or its value. A string's characters are written as themselves
when they are printable ASCII, other than `"` and `\\`, which are
written `\\"` and `\\\\`. Every other character is `\\xHH` when its code
is below 0x100 and `\\u{H...}` above, in lowercase hex. The C++ side
holds strings in UTF-8.

Every element of a sequence of the types in `asdl.DUMP_LINE_TYPES`
(top-level items, instructions, declarations, block items) goes on a
line of its own, indented by two spaces for each such sequence it is
in, and the closing `]` goes on a line of its own. Every other value
is on one line, so a function's instructions are one per line:

    Program([
      Function("f" true [] [
        Mov(Imm(1) Reg(A) false)
        Return(true false false)
      ])
    ])

A dump ends with a newline, and the readers skip any whitespace
between tokens.

The writer checks each value against the schema. A field that holds
`None` where the .asdl gives no `?`, a tuple where it gives `*`, or a
value of another type raises `DumpError`, since the value read back
would differ from it. A `bool` in an `int` field is written as 0 or 1,
which Python takes as equal, and so is an int 0 or 1 in a `bool` field.
"""
from __future__ import annotations

import importlib
import os
import re
from types import ModuleType
from typing import Any, Callable

import asdl


class DumpError(Exception):
    """Raised for a value the dump format can't hold exactly, or for
    text that isn't in the format."""


_ROOT = os.path.dirname(os.path.abspath(__file__))

# Each IR module and the .asdl file it is generated from.
_ASDL_FILES = {
    "asm_ast": "asm.asdl",
    "asm2_ast": "asm2.asdl",
    "tac_ast": "tac.asdl",
    "c99_ast": "c99.asdl",
}


# --- Strings ---

_PLAIN = re.compile(r'[ !#-\[\]-~]*\Z')


def _quote(s: str) -> str:
    if _PLAIN.match(s):
        return '"' + s + '"'
    out = ['"']
    for ch in s:
        c = ord(ch)
        if ch == '"' or ch == "\\":
            out.append("\\" + ch)
        elif 0x20 <= c < 0x7F:
            out.append(ch)
        elif c < 0x100:
            out.append(f"\\x{c:02x}")
        else:
            out.append(f"\\u{{{c:x}}}")
    out.append('"')
    return "".join(out)


_ESCAPE = re.compile(r'\\(?:(["\\])|x([0-9a-fA-F]{2})|u\{([0-9a-fA-F]{1,6})\}|(.))')


def _unescape_match(m: re.Match) -> str:
    if m.group(1) is not None:
        return m.group(1)
    if m.group(2) is not None:
        return chr(int(m.group(2), 16))
    if m.group(3) is not None:
        return chr(int(m.group(3), 16))
    raise DumpError(f"unknown escape \\{m.group(4)} in a string")


def _unquote(token: str) -> str:
    body = token[1:-1]
    if "\\" not in body:
        return body
    return _ESCAPE.sub(_unescape_match, body)


# --- The schema of one module ---

class _Schema:
    """The readers and writers of one IR module, built from its .asdl
    file."""

    def __init__(self, module: ModuleType, asdl_path: str) -> None:
        with open(asdl_path, "r", encoding="utf-8") as f:
            parsed = asdl.parse(f.read())
        self.module = module
        self.types = {t.name: t for t in parsed.types}
        # The writer of each class, by class: (type name, function).
        self.writers: dict[type, tuple[str, Callable]] = {}
        # The reader of each name a value can start with: a
        # constructor's or a product type's. (type name, function)
        self.readers: dict[str, tuple[str, Callable]] = {}
        for t in parsed.types:
            if t.is_product:
                cls = getattr(module, f"Type_{t.name}")
                self._add(t.name, t.name, cls, t.fields)
            else:
                for c in t.constructors:
                    cls = getattr(module, c.name)
                    self._add(t.name, c.name, cls, list(c.fields) + list(t.fields))

    def _add(self, type_name: str, dump_name: str, cls: type,
             fields: list[asdl.Field]) -> None:
        names = [asdl._field_name(f) for f in fields]
        field_writers = [self._field_writer(f) for f in fields]
        field_readers = [self._field_reader(f) for f in fields]

        if not fields:
            def write(v, depth: int, out: list[str]) -> None:
                out.append(dump_name)

            def read(tokens: "_Tokens"):
                return cls()
        else:
            pairs = list(zip(names, field_writers))
            head = dump_name + "("

            def write(v, depth: int, out: list[str]) -> None:
                out.append(head)
                first = True
                for name, fw in pairs:
                    if not first:
                        out.append(" ")
                    first = False
                    fw(getattr(v, name), depth, out)
                out.append(")")

            read_pairs = list(zip(names, field_readers))

            def read(tokens: "_Tokens"):
                tokens.expect("(")
                kwargs = {}
                for name, fr in read_pairs:
                    kwargs[name] = fr(tokens)
                tokens.expect(")")
                return cls(**kwargs)

        self.writers[cls] = (type_name, write)
        self.readers[dump_name] = (type_name, read)

    # Writers of one field, by the field's ASDL type.

    def _element_writer(self, f: asdl.Field) -> Callable:
        t = f.type
        if t in ("identifier", "string"):
            def w(v, depth, out):
                if type(v) is not str:
                    raise DumpError(f"expected a str for {t}, got {v!r}")
                out.append(_quote(v))
            return w
        if t == "int":
            def w(v, depth, out):
                if type(v) is int:
                    out.append(str(v))
                elif type(v) is bool:
                    out.append("1" if v else "0")
                else:
                    raise DumpError(f"expected an int, got {v!r}")
            return w
        if t == "bool":
            def w(v, depth, out):
                if v is True or (type(v) is int and v == 1):
                    out.append("true")
                elif v is False or (type(v) is int and v == 0):
                    out.append("false")
                else:
                    raise DumpError(f"expected a bool, got {v!r}")
            return w
        if t in asdl._PRIMITIVES:
            raise DumpError(f"the dump format has no form for {t}")
        writers = self.writers

        def w(v, depth, out):
            entry = writers.get(type(v))
            if entry is None or entry[0] != t:
                raise DumpError(f"expected a value of type {t}, got {v!r}")
            entry[1](v, depth, out)
        return w

    def _field_writer(self, f: asdl.Field) -> Callable:
        elem = self._element_writer(f)
        if f.sequence:
            if f.type in asdl.DUMP_LINE_TYPES:
                def w(v, depth, out):
                    if type(v) is not list:
                        raise DumpError(
                            f"expected a list of {f.type}, got {type(v).__name__}"
                        )
                    if not v:
                        out.append("[]")
                        return
                    out.append("[")
                    inner = "\n" + "  " * (depth + 1)
                    for e in v:
                        out.append(inner)
                        elem(e, depth + 1, out)
                    out.append("\n" + "  " * depth + "]")
                return w

            def w(v, depth, out):
                if type(v) is not list:
                    raise DumpError(
                        f"expected a list of {f.type}, got {type(v).__name__}"
                    )
                out.append("[")
                first = True
                for e in v:
                    if not first:
                        out.append(" ")
                    first = False
                    elem(e, depth, out)
                out.append("]")
            return w
        if f.optional:
            def w(v, depth, out):
                if v is None:
                    out.append("None")
                else:
                    elem(v, depth, out)
            return w

        def w(v, depth, out):
            if v is None:
                raise DumpError(
                    f"None in field {asdl._field_name(f)!r}, which isn't optional"
                )
            elem(v, depth, out)
        return w

    # Readers of one field.

    def _element_reader(self, f: asdl.Field) -> Callable:
        t = f.type
        if t in ("identifier", "string"):
            def r(tokens):
                tok = tokens.next()
                if tok[0] != '"':
                    tokens.fail(f"expected a string, got {tok!r}")
                return _unquote(tok)
            return r
        if t == "int":
            def r(tokens):
                tok = tokens.next()
                try:
                    return int(tok)
                except ValueError:
                    tokens.fail(f"expected an int, got {tok!r}")
            return r
        if t == "bool":
            def r(tokens):
                tok = tokens.next()
                if tok == "true":
                    return True
                if tok == "false":
                    return False
                tokens.fail(f"expected true or false, got {tok!r}")
            return r
        readers = self.readers

        def r(tokens):
            tok = tokens.next()
            entry = readers.get(tok)
            if entry is None or entry[0] != t:
                tokens.fail(f"expected a value of type {t}, got {tok!r}")
            return entry[1](tokens)
        return r

    def _field_reader(self, f: asdl.Field) -> Callable:
        elem = self._element_reader(f)
        if f.sequence:
            def r(tokens):
                tokens.expect("[")
                out = []
                while not tokens.take("]"):
                    out.append(elem(tokens))
                return out
            return r
        if f.optional:
            def r(tokens):
                if tokens.take("None"):
                    return None
                return elem(tokens)
            return r
        return elem


_SCHEMAS: dict[str, _Schema] = {}


def _schema(module: ModuleType | str) -> _Schema:
    name = module if isinstance(module, str) else module.__name__
    schema = _SCHEMAS.get(name)
    if schema is None:
        if name not in _ASDL_FILES:
            raise DumpError(f"no .asdl file for module {name!r}")
        mod = importlib.import_module(name)
        schema = _Schema(mod, os.path.join(_ROOT, _ASDL_FILES[name]))
        _SCHEMAS[name] = schema
    return schema


def _module_of(value: Any) -> str:
    return type(value).__module__


def dump(value: Any) -> str:
    """Returns the dump of `value`, a value of any type of one of the
    four IR modules, ending with a newline."""
    schema = _schema(_module_of(value))
    entry = schema.writers.get(type(value))
    if entry is None:
        raise DumpError(f"not an IR value: {value!r}")
    out: list[str] = []
    entry[1](value, 0, out)
    out.append("\n")
    return "".join(out)


# --- Reading ---

_TOKEN = re.compile(
    r'\s*(?:([A-Za-z_]\w*|-?[0-9]+|"(?:[^"\\]|\\.)*"|[()\[\]]))'
)


class _Tokens:
    def __init__(self, text: str) -> None:
        self.text = text
        self.tokens: list[tuple[str, int]] = []
        pos = 0
        end = len(text)
        match = _TOKEN.match
        while True:
            m = match(text, pos)
            if m is None:
                rest = text[pos:]
                if rest.strip():
                    self.i = len(self.tokens)
                    self._fail_at(pos + len(rest) - len(rest.lstrip()),
                                  "unexpected character")
                break
            self.tokens.append((m.group(1), m.start(1)))
            pos = m.end()
            if pos >= end:
                break
        self.i = 0

    def next(self) -> str:
        if self.i >= len(self.tokens):
            self.fail("unexpected end of text")
        tok = self.tokens[self.i][0]
        self.i += 1
        return tok

    def take(self, tok: str) -> bool:
        if self.i < len(self.tokens) and self.tokens[self.i][0] == tok:
            self.i += 1
            return True
        return False

    def expect(self, tok: str) -> None:
        if not self.take(tok):
            got = self.tokens[self.i][0] if self.i < len(self.tokens) else "end of text"
            self.fail(f"expected {tok!r}, got {got!r}")

    def at_end(self) -> bool:
        return self.i >= len(self.tokens)

    def fail(self, message: str):
        k = min(max(self.i - 1, 0), len(self.tokens) - 1)
        pos = self.tokens[k][1] if self.tokens else 0
        self._fail_at(pos, message)

    def _fail_at(self, pos: int, message: str):
        line = self.text.count("\n", 0, pos) + 1
        col = pos - (self.text.rfind("\n", 0, pos) + 1) + 1
        raise DumpError(f"{line}:{col}: {message}")


def load(text: str, module: ModuleType | str) -> Any:
    """Returns the value that `text`, in the dump format, holds. `module`
    is the IR module (`asm_ast`) or its name."""
    schema = _schema(module)
    tokens = _Tokens(text)
    head = tokens.next()
    entry = schema.readers.get(head)
    if entry is None:
        tokens.fail(f"unknown constructor or type {head!r}")
    value = entry[1](tokens)
    if not tokens.at_end():
        tokens.i += 1
        tokens.fail("unexpected text after the value")
    return value
