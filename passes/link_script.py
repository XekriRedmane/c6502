"""Linker scripts for `compile.py --link -T script.ld`: a subset of GNU
ld's script language.

A script names the memory regions of the machine and says which
sections go where:

    MEMORY
    {
        ram (rwx) : ORIGIN = 0x0800, LENGTH = 0x5800
        rom (rx)  : ORIGIN = 0xD000, LENGTH = 0x2FFA
    }

    SECTIONS
    {
        .text : { *(.text.boot) *(.text .text.*) } > rom
        .rodata : { *(.rodata .rodata.*) } > rom
        .data : { *(.data .data.*) } > ram AT> rom
        .bss (NOLOAD) : { *(.bss .bss.*) } > ram
        .vectors 0xFFFA : { KEEP(*(.vectors)) }
        __end = .;
    }

`parse_script` turns the text into a `Script`; `passes/linker.py` lays
the program out from it. The subset:

* `MEMORY` regions: `name [(attrs)] : ORIGIN = expr, LENGTH = expr`,
  with `org`, `o`, `len`, and `l` as GNU spells them. The attributes
  are accepted and ignored.
* In `SECTIONS`, output sections and symbol assignments. An output
  section is `name [address] [(NOLOAD)] : [AT(lma)] [ALIGN(n)]
  { ... } [> region] [AT> region]`, with an optional `;` after it.
  Inside the braces: input section descriptions `*(pattern ...)` and
  `KEEP(*(pattern ...))` (every section is kept, since the linker
  doesn't discard anything), symbol assignments, `. = ALIGN(n);`, and
  `. = . + n;` or `. += n;` to skip `n` bytes. Patterns use `*`, `?`,
  and `[...]` as in GNU ld. The file part is `*`, or a pattern for an
  input file's name without its directory, as in `zstring.asm(.text)`.
* Symbol assignments `sym = expr;` and `PROVIDE(sym = expr);`, inside
  or after the output sections.
* Expressions: numbers (decimal, `0x` hex, and a `K` or `M` suffix),
  symbols, `.`, `+ - * / & | ^ << >>`, unary `-` and `~`,
  parentheses, and `ALIGN(n)`, `ALIGN(expr, n)`, `ORIGIN(region)`,
  `LENGTH(region)`, `ADDR(section)`, `LOADADDR(section)`, and
  `SIZEOF(section)`.
* Comments `/* ... */` and `//` to the end of the line.

Anything else (`ENTRY`, `INCLUDE`, `/DISCARD/`, `FILL`, `BYTE`,
`SORT`, `.` assigned outside an output section) is a `ScriptError`.
"""
from __future__ import annotations

import re
from dataclasses import dataclass, field


class ScriptError(Exception):
    """A linker script that isn't in the supported subset."""


# --- Expressions ----------------------------------------------------------

@dataclass
class Num:
    value: int


@dataclass
class Sym:
    name: str


@dataclass
class Dot:
    """`.`, the location counter."""


@dataclass
class Unary:
    op: str
    operand: object


@dataclass
class Binary:
    op: str
    left: object
    right: object


@dataclass
class Call:
    """`ALIGN`, `ORIGIN`, `LENGTH`, `ADDR`, `LOADADDR`, or `SIZEOF`.
    The region and section functions take a name, as a `Sym`."""
    fn: str
    args: list


# --- Statements -----------------------------------------------------------

@dataclass
class Region:
    name: str
    origin: int
    length: int

    @property
    def end(self) -> int:
        """One past the region's last byte."""
        return self.origin + self.length


@dataclass
class Assign:
    """`symbol = expr;`. `symbol` is `.` for a location assignment."""
    symbol: str
    expr: object


@dataclass
class Input:
    """`file(pattern ...)`, or `KEEP(file(pattern ...))`. `file` is
    `*` for every input file, or a pattern for the input file's name
    without its directory, such as `zstring.asm`."""
    patterns: list[str]
    file: str = "*"


@dataclass
class OutputSection:
    name: str
    address: object | None = None
    noload: bool = False
    at: object | None = None
    align: object | None = None
    contents: list = field(default_factory=list)
    region: str | None = None
    load_region: str | None = None


@dataclass
class Script:
    regions: dict[str, Region] = field(default_factory=dict)
    statements: list = field(default_factory=list)

    def output_sections(self) -> list[OutputSection]:
        return [s for s in self.statements if isinstance(s, OutputSection)]


# --- Lexer ----------------------------------------------------------------

_TOKEN = re.compile(r"""
    (?P<space>\s+|/\*.*?\*/|//[^\n]*)
  | (?P<num>(?:0[xX][0-9A-Fa-f]+|\d+)[KM]?\b)
  | (?P<op><<|>>|\+=|-=|AT>|[{}():;=,>+\-*/&|^~<])
  | (?P<name>[A-Za-z_.*?\[\]$][A-Za-z0-9_.*?\[\]$\-]*)
""", re.S | re.X)


def _tokens(text: str) -> list[str]:
    out: list[str] = []
    pos = 0
    while pos < len(text):
        m = _TOKEN.match(text, pos)
        if m is None:
            line = text.count("\n", 0, pos) + 1
            raise ScriptError(
                f"line {line}: can't read {text[pos:pos + 20]!r}"
            )
        pos = m.end()
        if m.lastgroup != "space":
            out.append(m.group())
    return out


# --- Parser ---------------------------------------------------------------

_FUNCTIONS = ("ALIGN", "ORIGIN", "LENGTH", "ADDR", "LOADADDR", "SIZEOF")


class _Parser:
    def __init__(self, text: str) -> None:
        self.toks = _tokens(text)
        self.i = 0

    def peek(self, k: int = 0) -> str | None:
        j = self.i + k
        return self.toks[j] if j < len(self.toks) else None

    def take(self, want: str | None = None) -> str:
        tok = self.peek()
        if tok is None:
            raise ScriptError(f"unexpected end of script; expected {want}")
        if want is not None and tok != want:
            raise ScriptError(f"expected {want!r}, found {tok!r}")
        self.i += 1
        return tok

    def script(self) -> Script:
        out = Script()
        while self.peek() is not None:
            word = self.take()
            if word == "MEMORY":
                self.memory(out)
            elif word == "SECTIONS":
                self.sections(out)
            else:
                raise ScriptError(
                    f"{word!r} isn't supported; a script has MEMORY and "
                    f"SECTIONS commands"
                )
        return out

    def memory(self, out: Script) -> None:
        self.take("{")
        while self.peek() != "}":
            name = self.take()
            if self.peek() == "(":
                while self.take() != ")":
                    pass
            self.take(":")
            values: dict[str, int] = {}
            for _ in range(2):
                key = self.take()
                key = {"org": "ORIGIN", "o": "ORIGIN", "len": "LENGTH",
                       "l": "LENGTH"}.get(key, key)
                if key not in ("ORIGIN", "LENGTH"):
                    raise ScriptError(
                        f"region {name!r}: expected ORIGIN or LENGTH, "
                        f"found {key!r}"
                    )
                self.take("=")
                values[key] = _constant(self.expr(), f"{key} of {name!r}")
                if self.peek() == ",":
                    self.take()
            if name in out.regions:
                raise ScriptError(f"region {name!r} is defined twice")
            out.regions[name] = Region(
                name=name, origin=values["ORIGIN"], length=values["LENGTH"],
            )
        self.take("}")

    def sections(self, out: Script) -> None:
        self.take("{")
        while self.peek() != "}":
            if self.peek() == ";":
                self.take()
                continue
            assign = self.assignment()
            if assign is not None:
                if assign.symbol == ".":
                    raise ScriptError(
                        "`. =` outside an output section isn't supported; "
                        "give the output section an address or a region"
                    )
                out.statements.append(assign)
                continue
            out.statements.append(self.output_section())
        self.take("}")

    def assignment(self) -> Assign | None:
        """A `sym = expr;`, `PROVIDE(sym = expr);`, `. = expr;`, or
        `. += n;` statement, or None if one doesn't start here."""
        if self.peek() == "PROVIDE" and self.peek(1) == "(":
            self.take()
            self.take("(")
            name = self.take()
            self.take("=")
            expr = self.expr()
            self.take(")")
            self.take(";")
            return Assign(name, expr)
        if self.peek(1) in ("=", "+=", "-=") and self.peek() is not None \
                and self.peek() not in ("{", "}"):
            name = self.take()
            op = self.take()
            expr = self.expr()
            self.take(";")
            if op != "=":
                expr = Binary(op[0], Dot() if name == "." else Sym(name), expr)
            return Assign(name, expr)
        return None

    def output_section(self) -> OutputSection:
        sec = OutputSection(name=self.take())
        if self.peek() not in (":", "("):
            sec.address = self.expr()
        elif self.peek() == "(" and self.peek(1) != "NOLOAD":
            sec.address = self.expr()
        if self.peek() == "(":
            self.take()
            self.take("NOLOAD")
            self.take(")")
            sec.noload = True
        self.take(":")
        while self.peek() in ("AT", "ALIGN"):
            word = self.take()
            self.take("(")
            value = self.expr()
            self.take(")")
            if word == "AT":
                sec.at = value
            else:
                sec.align = value
        self.take("{")
        while self.peek() != "}":
            sec.contents.append(self.content(sec.name))
        self.take("}")
        while self.peek() in (">", "AT>", "AT"):
            tok = self.take()
            if tok == "AT":
                self.take(">")
                tok = "AT>"
            region = self.take()
            if tok == ">":
                sec.region = region
            else:
                sec.load_region = region
        if self.peek() == ";":
            self.take()
        return sec

    def content(self, where: str):
        if self.peek() == ";":
            self.take()
            return self.content(where)
        assign = self.assignment()
        if assign is not None:
            return assign
        keep = self.peek() == "KEEP"
        if keep:
            self.take()
            self.take("(")
        files = self.take()
        # `*zstring.asm` comes as `*` and `zstring.asm`.
        if files == "*" and self.peek() not in ("(", None):
            files += self.take()
        if files in ("(", ")", "{", "}", ";", ":", "="):
            raise ScriptError(
                f"in {where}: an input section description starts with "
                f"the file it takes sections from, such as `*` or "
                f"`zstring.asm`, not {files!r}"
            )
        self.take("(")
        patterns = []
        while self.peek() != ")":
            pattern = self.take()
            if pattern == ",":
                continue
            patterns.append(pattern)
        self.take(")")
        if keep:
            self.take(")")
        return Input(patterns=patterns, file=files)

    # Expressions, by precedence from loosest to tightest.
    _LEVELS = [("|",), ("^",), ("&",), ("<<", ">>"), ("+", "-"),
               ("*", "/")]

    def expr(self, level: int = 0):
        if level == len(self._LEVELS):
            return self.unary()
        left = self.expr(level + 1)
        while self.peek() in self._LEVELS[level]:
            op = self.take()
            left = Binary(op, left, self.expr(level + 1))
        return left

    def unary(self):
        tok = self.take()
        if tok in ("-", "~"):
            return Unary(tok, self.unary())
        if tok == "(":
            inner = self.expr()
            self.take(")")
            return inner
        if tok == ".":
            return Dot()
        if re.fullmatch(r"(?:0[xX][0-9A-Fa-f]+|\d+)[KM]?", tok):
            scale = {"K": 1024, "M": 1024 * 1024}.get(tok[-1], 1)
            digits = tok[:-1] if scale != 1 else tok
            return Num(int(digits, 0) * scale)
        if tok in _FUNCTIONS and self.peek() == "(":
            self.take("(")
            args = [self.expr()]
            while self.peek() == ",":
                self.take()
                args.append(self.expr())
            self.take(")")
            return Call(tok, args)
        if re.fullmatch(r"[A-Za-z_.][A-Za-z0-9_.]*", tok):
            return Sym(tok)
        raise ScriptError(f"expected an expression, found {tok!r}")


def _constant(e, what: str) -> int:
    """The value of `e`, which has to be made of numbers."""
    match e:
        case Num(value=v):
            return v
        case Unary(op="-", operand=o):
            return -_constant(o, what)
        case Unary(op="~", operand=o):
            return ~_constant(o, what) & 0xFFFF
        case Binary(op=op, left=l, right=r):
            a, b = _constant(l, what), _constant(r, what)
            return {
                "+": a + b, "-": a - b, "*": a * b, "/": a // b if b else 0,
                "&": a & b, "|": a | b, "^": a ^ b, "<<": a << b,
                ">>": a >> b,
            }[op]
    raise ScriptError(f"{what} has to be a number")


def parse_script(text: str) -> Script:
    """Parses the linker script `text`. Raises `ScriptError` for
    anything outside the subset, an unknown region, or a section
    listed twice."""
    script = _Parser(text).script()
    seen: set[str] = set()
    for sec in script.output_sections():
        if sec.name in seen:
            raise ScriptError(f"output section {sec.name!r} is listed twice")
        seen.add(sec.name)
        for region in (sec.region, sec.load_region):
            if region is not None and region not in script.regions:
                raise ScriptError(
                    f"output section {sec.name!r} names region {region!r}, "
                    f"which MEMORY doesn't define"
                )
        if sec.noload and (sec.load_region or sec.at is not None):
            raise ScriptError(
                f"output section {sec.name!r} is NOLOAD, so it has no load "
                f"address for AT to give"
            )
    return script
