"""Assembles dasm text, as `asm_emit` writes it, into a relocatable ELF
object (`binutils/elf.py`). `compile.py -c` runs this on the text that
`compile.py -S` would write, as gcc runs `as` on the output of `cc1`.

The input is the subset of dasm that c6502 writes:

* `name EQU expr` (or `name = expr`), anywhere in the file;
* a label `name:` in column 1, where a name that starts with `.` is
  local to the item it's in, as dasm scopes it to its `SUBROUTINE`;
* in the columns after it, an instruction (`LDA #<(buf+2)`, `STA
  (DPTR),Y`, `ASL`, `BNE .loop@3`) or one of the directives
  `SUBROUTINE`, `DC.B`, `DC.W`, `DC.L`, `DS.B`, and `PROCESSOR`, which
  is ignored;
* comments after `;`, of which the link metadata block
  (`passes/zp_link_metadata.py`) is kept, in the section `.c6502.link`.

Expressions have numbers (`$1F`, `%101`, `12`, `'c'`), symbols,
`+ - * / & | ^ << >>`, unary `-`, `~`, `<` (low byte), and `>` (high
byte), and parentheses or dasm's brackets for grouping. A parenthesis
that opens an operand means indirection, as in dasm, so `(DPTR),Y` is
indirect and `#<(buf+2)` groups.

Each item, a function or object that a label in column 1 starts, goes
in a section of its own, named for the section the link metadata's
`item` line gives it. Without metadata, a function goes in `.text`, an
object of `DS.B` alone in `.bss`, and any other object in `.data`.

Symbols. An item's label is a global symbol, or a local one when the
metadata marks it internal. A `.` label is a local symbol. An `EQU`
gives an absolute symbol, except for the names the linker places
(`LINK_PLACED`): SSP, FP, HARGS, DPTR, the `__zpabi_*` and `__local_*`
slots, and the unit's zero-page objects. Those stay undefined in the
object, and their `EQU` values only decide the addressing mode, since
the linker gives them their addresses for the whole program.

Addressing modes are decided here, once. An operand gets zero-page
addressing when its value is known to be below $100: a number, an
`EQU`, or a runtime name (SSP, FP, HARGS, DPTR, which are always in
zero page). A label in the unit or an undefined name gets absolute
addressing. Every other reference becomes a relocation, and the
field holds 0 until the linker fills it in. A branch to a label in
the same item is resolved here. The linker reports a relocation
whose value doesn't fit its field, such as a zero-page slot it had to
put above $FF.

Code and data in one section are told apart by mapping symbols, as
ARM's ELF does: `$d` where data starts in a function (a jump table)
and `$c` where code starts again.
"""
from __future__ import annotations

import re
from dataclasses import dataclass, field
from pathlib import Path

from binutils import elf as E
from binutils.opcodes import (
    MNEMONICS, OPCODES, SIZES, ZP_FORM, modes as op_modes,
)


class AssemblerError(Exception):
    """A line the assembler can't assemble."""


# The names the runtime's block uses, which are always in zero page.
RUNTIME_NAMES = frozenset({"SSP", "FP", "HARGS", "DPTR"})

_SHIFTS = frozenset({"ASL", "LSR", "ROL", "ROR"})
_DIRECTIVES = frozenset({
    "SUBROUTINE", "DC.B", "DC.W", "DC.L", "DS.B", "PROCESSOR",
})
_META_BEGIN = "; @zp-link-meta-begin"
_META_END = "; @zp-link-meta-end"

# A `%` can't start a symbol, where it starts a binary number, but it
# can be inside one: the compiler names the slot of an address-taken
# temporary after it (`__local_main__addr_%22`).
_SYMBOL = r"[A-Za-z_.@][A-Za-z0-9_.@%]*"
_EQU = re.compile(rf"^({_SYMBOL})\s+(?:EQU|equ|=)\s+(.+?)\s*$")
_LABEL = re.compile(rf"^({_SYMBOL}):?\s*$")


def is_link_placed(name: str, zp_objects: frozenset[str] = frozenset()) -> bool:
    """True iff the linker gives `name` its address: a runtime name, a
    zp_abi parameter slot, a body-local slot, or a zero-page object."""
    return (
        name in RUNTIME_NAMES
        or name.startswith(("__zpabi_", "__local_"))
        or name in zp_objects
    )


# --- Expressions -----------------------------------------------------------


_TOKEN = re.compile(
    rf"\s*(?:(\$[0-9A-Fa-f]+)|(%[01]+)|(\d+)|('.')|({_SYMBOL})"
    r"|(<<|>>|[-+*/&|^~<>()\[\],#]))"
)


def _tokens(text: str, where: str) -> list[str]:
    out = []
    pos = 0
    text = text.rstrip()
    while pos < len(text):
        m = _TOKEN.match(text, pos)
        if m is None or m.end() == pos:
            raise AssemblerError(f"{where}: can't read {text[pos:]!r}")
        out.append(m.group(m.lastindex))
        pos = m.end()
    return out


@dataclass
class _Num:
    value: int


@dataclass
class _Sym:
    name: str


@dataclass
class _Un:
    op: str
    operand: object


@dataclass
class _Bin:
    op: str
    left: object
    right: object


# Binary operators by precedence, lowest first.
_LEVELS = [("|",), ("^",), ("&",), ("<<", ">>"), ("+", "-"), ("*", "/")]


class _ExprParser:
    def __init__(self, tokens: list[str], where: str) -> None:
        self.t = tokens
        self.i = 0
        self.where = where

    def peek(self) -> str | None:
        return self.t[self.i] if self.i < len(self.t) else None

    def take(self, want: str | None = None) -> str:
        tok = self.peek()
        if tok is None or (want is not None and tok != want):
            raise AssemblerError(
                f"{self.where}: expected {want or 'more'}, found "
                f"{tok or 'the end of the line'}"
            )
        self.i += 1
        return tok

    def expr(self, level: int = 0):
        if level == len(_LEVELS):
            return self.unary()
        left = self.expr(level + 1)
        while self.peek() in _LEVELS[level]:
            op = self.take()
            left = _Bin(op, left, self.expr(level + 1))
        return left

    def unary(self):
        tok = self.take()
        if tok in ("-", "~", "<", ">"):
            return _Un(tok, self.unary())
        if tok in ("(", "["):
            inner = self.expr()
            self.take(")" if tok == "(" else "]")
            return inner
        if tok.startswith("$"):
            return _Num(int(tok[1:], 16))
        if tok.startswith("%"):
            return _Num(int(tok[1:], 2))
        if tok.isdigit():
            return _Num(int(tok))
        if tok.startswith("'"):
            return _Num(ord(tok[1]))
        if re.fullmatch(_SYMBOL, tok):
            return _Sym(tok)
        raise AssemblerError(f"{self.where}: unexpected {tok!r}")


def parse_expr(text: str, where: str = "expression"):
    p = _ExprParser(_tokens(text, where), where)
    e = p.expr()
    if p.peek() is not None:
        raise AssemblerError(f"{where}: unexpected {p.peek()!r} in {text!r}")
    return e


@dataclass
class Value:
    """A symbol plus a constant, or a constant when `symbol` is None,
    with `select` "lo" or "hi" for `<` or `>` of it."""
    symbol: str | None
    addend: int
    select: str | None = None


def _const_op(op: str, a: int, b: int, where: str) -> int:
    if op == "+":
        return a + b
    if op == "-":
        return a - b
    if op == "*":
        return a * b
    if op == "/":
        if b == 0:
            raise AssemblerError(f"{where}: division by zero")
        return a // b
    if op == "&":
        return a & b
    if op == "|":
        return a | b
    if op == "^":
        return a ^ b
    if op == "<<":
        return a << b
    if op == ">>":
        return a >> b
    raise AssemblerError(f"{where}: unknown operator {op}")


def evaluate(e, constants: dict[str, int], where: str) -> Value:
    """`e` as a `Value`. A symbol in `constants` folds to its value
    where an operator needs a number; any other symbol stays a symbol,
    which only `+`, `-`, `<`, and `>` may combine with constants."""
    def num(v: Value, what: str) -> int:
        if v.symbol is None:
            return _select(v)
        if v.symbol in constants:
            return _select(Value(None, constants[v.symbol] + v.addend,
                                 v.select))
        raise AssemblerError(
            f"{where}: `{v.symbol}` can't be used in {what}, since its "
            f"address is only known when linking"
        )

    def go(e) -> Value:
        if isinstance(e, _Num):
            return Value(None, e.value)
        if isinstance(e, _Sym):
            return Value(e.name, 0)
        if isinstance(e, _Un):
            v = go(e.operand)
            if e.op in ("<", ">"):
                if v.select is not None:
                    raise AssemblerError(f"{where}: `{e.op}` of a byte")
                sel = "lo" if e.op == "<" else "hi"
                if v.symbol is None:
                    return Value(None, _select(Value(None, v.addend, sel)))
                return Value(v.symbol, v.addend, sel)
            n = num(v, f"unary `{e.op}`")
            return Value(None, -n if e.op == "-" else ~n)
        assert isinstance(e, _Bin)
        a, b = go(e.left), go(e.right)
        if e.op in ("+", "-") and a.select is None and b.select is None:
            if b.symbol is None:
                return Value(
                    a.symbol, _const_op(e.op, a.addend, b.addend, where),
                )
            if e.op == "+" and a.symbol is None:
                return Value(b.symbol, a.addend + b.addend)
            if (e.op == "-" and a.symbol == b.symbol):
                return Value(None, a.addend - b.addend)
        return Value(
            None, _const_op(e.op, num(a, f"`{e.op}`"), num(b, f"`{e.op}`"),
                            where),
        )
    return go(e)


def _select(v: Value) -> int:
    if v.select == "lo":
        return v.addend & 0xFF
    if v.select == "hi":
        return (v.addend >> 8) & 0xFF
    return v.addend


# --- Statements ------------------------------------------------------------


@dataclass
class _Stmt:
    line: int
    op: str                      # mnemonic or directive, uppercase
    operand: str
    mode: str = ""
    expr: object = None          # parsed operand expression
    size: int = 0


@dataclass
class _Item:
    name: str
    line: int
    function: bool = False
    stmts: list[_Stmt] = field(default_factory=list)
    # Local label -> index of the statement it precedes.
    labels: dict[str, int] = field(default_factory=dict)
    label_lines: dict[str, int] = field(default_factory=dict)


def _split_operand(op: str, operand: str, where: str) -> tuple[str, object]:
    """The addressing syntax of an instruction's operand and its
    expression: "imp", "acc", "imm", "ind", "indx", "indy", "x", "y",
    or "plain"."""
    s = operand.strip()
    if not s:
        return ("acc" if op in _SHIFTS else "imp"), None
    if s.upper() == "A" and op in _SHIFTS:
        return "acc", None
    if s.startswith("#"):
        return "imm", parse_expr(s[1:], where)
    if s.startswith("("):
        m = re.fullmatch(r"\((.*)\)\s*,\s*[Yy]", s)
        if m and _balanced(m.group(1)):
            return "indy", parse_expr(m.group(1), where)
        m = re.fullmatch(r"\((.*),\s*[Xx]\s*\)", s)
        if m and _balanced(m.group(1)):
            return "indx", parse_expr(m.group(1), where)
        if s.endswith(")") and _balanced(s[1:-1]):
            return "ind", parse_expr(s[1:-1], where)
    m = re.fullmatch(r"(.*?)\s*,\s*([XxYy])", s)
    if m:
        return m.group(2).lower(), parse_expr(m.group(1), where)
    return "plain", parse_expr(s, where)


def _balanced(s: str) -> bool:
    depth = 0
    for c in s:
        depth += c in "(["
        depth -= c in ")]"
        if depth < 0:
            return False
    return depth == 0


def _split_list(s: str) -> list[str]:
    """`s` split at its top-level commas, keeping quoted strings."""
    out, cur, depth, quote = [], "", 0, None
    for c in s:
        if quote:
            cur += c
            if c == quote:
                quote = None
            continue
        if c in "\"":
            quote = c
        elif c in "([":
            depth += 1
        elif c in ")]":
            depth -= 1
        elif c == "," and depth == 0:
            out.append(cur.strip())
            cur = ""
            continue
        cur += c
    if cur.strip():
        out.append(cur.strip())
    return out


def _strip_comment(line: str) -> str:
    quote = None
    for i, c in enumerate(line):
        if quote:
            if c == quote:
                quote = None
        elif c in "\"'":
            # A character constant `'c'` takes one character.
            if c == "'" and i + 2 < len(line) and line[i + 2] == "'":
                continue
            quote = c
        elif c == ";":
            return line[:i]
    return line


# --- The assembler ---------------------------------------------------------


class _Assembler:
    def __init__(self, text: str, filename: str,
                 link_symbols: dict[str, int | None],
                 keep_source: bool) -> None:
        self.filename = filename
        self.lines = text.splitlines()
        self.link_symbols = dict(link_symbols)
        self.keep_source = keep_source

    def where(self, line: int) -> str:
        return f"{self.filename}:{line}"

    def run(self) -> E.ElfFile:
        from passes.zp_link_metadata import parse_metadata
        meta_lines: list[str] = []
        in_meta = False
        equs: list[tuple[str, str, int]] = []
        items: list[_Item] = []
        current: _Item | None = None
        for n, raw in enumerate(self.lines, 1):
            stripped = raw.strip()
            if stripped == _META_BEGIN:
                in_meta = True
            if in_meta:
                meta_lines.append(stripped)
                if stripped == _META_END:
                    in_meta = False
                continue
            line = _strip_comment(raw).rstrip()
            if not line.strip():
                continue
            m = _EQU.match(line)
            if m:
                equs.append((m.group(1), m.group(2), n))
                continue
            if not line[0].isspace():
                m = _LABEL.match(line)
                if m is None:
                    raise AssemblerError(
                        f"{self.where(n)}: can't read the label {line!r}"
                    )
                name = m.group(1)
                if name.startswith("."):
                    if current is None:
                        raise AssemblerError(
                            f"{self.where(n)}: local label {name} before "
                            f"any item"
                        )
                    if name in current.labels:
                        raise AssemblerError(
                            f"{self.where(n)}: {name} is defined twice in "
                            f"{current.name}"
                        )
                    current.labels[name] = len(current.stmts)
                    current.label_lines[name] = n
                else:
                    if any(it.name == name for it in items):
                        raise AssemblerError(
                            f"{self.where(n)}: {name} is defined twice"
                        )
                    current = _Item(name=name, line=n)
                    items.append(current)
                continue
            parts = line.strip().split(None, 1)
            op = parts[0].upper()
            operand = parts[1].strip() if len(parts) > 1 else ""
            if op == "PROCESSOR":
                continue
            if current is None:
                raise AssemblerError(
                    f"{self.where(n)}: {op} comes before any label"
                )
            if op == "SUBROUTINE":
                current.function = True
                continue
            if op not in _DIRECTIVES and op not in MNEMONICS:
                raise AssemblerError(
                    f"{self.where(n)}: unknown instruction or directive "
                    f"{parts[0]}"
                )
            current.stmts.append(_Stmt(line=n, op=op, operand=operand))

        self.meta_text = "\n".join(meta_lines) + "\n" if meta_lines else None
        meta = parse_metadata(self.meta_text) if self.meta_text else None
        self.meta = meta
        self.zp_objects = frozenset(
            z.name for z in meta.zp_objects) if meta else frozenset()
        self.item_meta = {it.name: it for it in meta.items} if meta else {}

        # EQUs: absolute symbols, or the values the linker replaces.
        self.constants: dict[str, int] = {}
        self.abs_symbols: dict[str, int] = {}
        for name, expr_text, n in equs:
            known = {**self.constants, **{
                k: v for k, v in self.link_symbols.items() if v is not None
            }}
            v = evaluate(parse_expr(expr_text, self.where(n)), known,
                         self.where(n))
            if v.symbol is not None:
                if v.symbol in known:
                    value = known[v.symbol]
                    value = _select(Value(None, value + v.addend, v.select))
                else:
                    raise AssemblerError(
                        f"{self.where(n)}: {name} has to be a number"
                    )
            else:
                value = v.addend
            if name in self.constants and self.constants[name] != value:
                raise AssemblerError(
                    f"{self.where(n)}: {name} is given two values"
                )
            if is_link_placed(name, self.zp_objects):
                # A value the caller gives (the linker's, when it
                # assembles a unit again) wins over the unit's own.
                self.link_symbols.setdefault(name, value)
            else:
                self.constants[name] = value
                self.abs_symbols[name] = value
        self.items = items
        self.item_names = {it.name for it in items}
        return self.build()

    # Sizing --------------------------------------------------------------

    def known_value(self, v: Value) -> int | None:
        """The value of `v` when the assembler can see it, for choosing
        an addressing mode."""
        if v.symbol is None:
            return v.addend
        if v.symbol in self.constants:
            return self.constants[v.symbol] + v.addend
        if self.link_symbols.get(v.symbol) is not None:
            return self.link_symbols[v.symbol] + v.addend
        return None

    def in_zero_page(self, v: Value) -> bool:
        if v.select is not None:
            return True
        value = self.known_value(v)
        if value is not None:
            return 0 <= value <= 0xFF
        if v.symbol in self.link_symbols:
            # Given as None: the linker put it outside zero page.
            return False
        return v.symbol in RUNTIME_NAMES and v.addend < 0x100

    def value_of(self, expr, where: str) -> Value:
        return evaluate(expr, self.constants, where)

    def size_stmt(self, st: _Stmt) -> None:
        where = self.where(st.line)
        if st.op == "DC.B":
            st.expr = [
                s for s in _split_list(st.operand)
            ]
            st.size = sum(
                len(s) - 2 if s.startswith('"') else 1 for s in st.expr
            )
            return
        if st.op in ("DC.W", "DC.L"):
            st.expr = [parse_expr(s, where) for s in _split_list(st.operand)]
            st.size = len(st.expr) * (2 if st.op == "DC.W" else 4)
            return
        if st.op == "DS.B":
            args = [parse_expr(s, where) for s in _split_list(st.operand)]
            if not args or len(args) > 2:
                raise AssemblerError(f"{where}: DS.B takes a count")
            count = self.value_of(args[0], where)
            if count.symbol is not None or count.addend < 0:
                raise AssemblerError(f"{where}: DS.B needs a constant count")
            fill = 0
            if len(args) == 2:
                f = self.value_of(args[1], where)
                if f.symbol is not None:
                    raise AssemblerError(f"{where}: DS.B needs a constant fill")
                fill = f.addend & 0xFF
            st.expr = (count.addend, fill)
            st.size = count.addend
            return
        syntax, expr = _split_operand(st.op, st.operand, where)
        st.expr = expr
        have = op_modes(st.op)
        if syntax in ("imp", "acc", "imm", "ind", "indx", "indy"):
            mode = syntax
        elif "rel" in have:
            if syntax != "plain":
                raise AssemblerError(f"{where}: {st.op} takes a label")
            mode = "rel"
        else:
            base = {"plain": "abs", "x": "absx", "y": "absy"}[syntax]
            v = self.value_of(expr, where)
            zp = ZP_FORM[base]
            mode = zp if zp in have and self.in_zero_page(v) else base
        if mode not in have:
            raise AssemblerError(
                f"{where}: {st.op} has no {mode} addressing mode"
            )
        st.mode = mode
        st.size = SIZES[mode]

    # Building ------------------------------------------------------------

    def section_for(self, it: _Item) -> E.Section:
        meta = self.item_meta.get(it.name)
        only_ds = all(st.op == "DS.B" for st in it.stmts)
        if meta is not None:
            name, zero = meta.section, meta.zero
        elif it.function:
            name, zero = ".text", False
        elif only_ds and it.stmts:
            name, zero = ".bss", True
        else:
            name, zero = ".data", False
        if zero and not only_ds:
            raise AssemblerError(
                f"{self.where(it.line)}: the link metadata says {it.name} "
                f"is all zero bytes, and it has data"
            )
        if it.function:
            flags = E.SHF_ALLOC | E.SHF_EXECINSTR
        elif name == ".rodata" or name.startswith(".rodata."):
            flags = E.SHF_ALLOC
        else:
            flags = E.SHF_ALLOC | E.SHF_WRITE
        return E.Section(
            name=name, type=E.SHT_NOBITS if zero else E.SHT_PROGBITS,
            flags=flags,
        )

    def build(self) -> E.ElfFile:
        out = E.ElfFile(type=E.ET_REL)
        out.symbols.append(E.Symbol(
            name=Path(self.filename).name, bind=E.STB_LOCAL, type=E.STT_FILE,
            section=E.SHN_ABS,
        ))
        self.global_syms: dict[str, E.Symbol] = {}
        sections: list[tuple[_Item, E.Section, dict[str, int]]] = []
        for it in self.items:
            for st in it.stmts:
                self.size_stmt(st)
            offsets = []
            pos = 0
            for st in it.stmts:
                offsets.append(pos)
                pos += st.size
            offsets.append(pos)
            labels = {name: offsets[i] for name, i in it.labels.items()}
            sec = self.section_for(it)
            meta = self.item_meta.get(it.name)
            local = meta.local if meta is not None else False
            sym = E.Symbol(
                name=it.name, value=0, size=pos,
                bind=E.STB_LOCAL if local else E.STB_GLOBAL,
                type=E.STT_FUNC if it.function else E.STT_OBJECT,
                section=sec,
            )
            out.sections.append(sec)
            out.symbols.append(sym)
            self.global_syms[it.name] = sym
            sections.append((it, sec, labels))
        for name, value in self.abs_symbols.items():
            if name in self.global_syms:
                raise AssemblerError(
                    f"{self.filename}: {name} is both a label and an EQU"
                )
            sym = E.Symbol(name=name, value=value, bind=E.STB_GLOBAL,
                           section=E.SHN_ABS)
            out.symbols.append(sym)
            self.global_syms[name] = sym
        self.out = out
        for it, sec, labels in sections:
            self.emit_item(it, sec, labels)
        if self.meta_text is not None:
            out.sections.append(E.Section(
                name=E.LINK_SECTION, type=E.SHT_PROGBITS, flags=0,
                data=bytearray(self.meta_text.encode("utf-8")),
            ))
        if self.keep_source:
            out.sections.append(E.Section(
                name=E.SOURCE_SECTION, type=E.SHT_PROGBITS, flags=0,
                data=bytearray("\n".join(self.lines).encode("utf-8") + b"\n"),
            ))
        return out

    def symbol(self, name: str, local: dict[str, E.Symbol],
               where: str) -> E.Symbol:
        if name.startswith("."):
            if name not in local:
                raise AssemblerError(
                    f"{where}: {name} isn't a label in this function"
                )
            return local[name]
        sym = self.global_syms.get(name)
        if sym is None:
            sym = E.Symbol(name=name, bind=E.STB_GLOBAL, section=E.SHN_UNDEF)
            self.out.symbols.append(sym)
            self.global_syms[name] = sym
        return sym

    def emit_item(self, it: _Item, sec: E.Section,
                  labels: dict[str, int]) -> None:
        local: dict[str, E.Symbol] = {}
        for name, off in sorted(labels.items(), key=lambda kv: kv[1]):
            sym = E.Symbol(name=name, value=off, bind=E.STB_LOCAL,
                           section=sec)
            local[name] = sym
            self.out.symbols.append(sym)
        data = bytearray()
        in_data = False
        for st in it.stmts:
            where = self.where(st.line)
            pc = len(data)
            is_data = st.op in ("DC.B", "DC.W", "DC.L", "DS.B")
            if it.function and is_data != in_data:
                self.out.symbols.append(E.Symbol(
                    name="$d" if is_data else "$c", value=pc,
                    bind=E.STB_LOCAL, section=sec,
                ))
                in_data = is_data
            if st.op == "DC.B":
                for s in st.expr:
                    if s.startswith('"'):
                        if not s.endswith('"') or len(s) < 2:
                            raise AssemblerError(f"{where}: bad string {s}")
                        data += s[1:-1].encode("latin-1")
                        continue
                    v = self.value_of(parse_expr(s, where), where)
                    self.field(sec, data, v, 1, local, where, data_byte=True)
            elif st.op in ("DC.W", "DC.L"):
                width = 2 if st.op == "DC.W" else 4
                for e in st.expr:
                    v = self.value_of(e, where)
                    self.field(sec, data, v, width, local, where)
            elif st.op == "DS.B":
                count, fill = st.expr
                if sec.type == E.SHT_NOBITS:
                    sec.nobits_size += count
                    continue
                data += bytes([fill]) * count
            else:
                self.emit_instr(st, sec, data, labels, local, where)
            if sec.type != E.SHT_NOBITS and len(data) - pc != st.size:
                raise AssemblerError(
                    f"{where}: {st.op} took {len(data) - pc} bytes where "
                    f"{st.size} were counted"
                )
        if sec.type != E.SHT_NOBITS:
            sec.data = data

    def emit_instr(self, st: _Stmt, sec: E.Section, data: bytearray,
                   labels: dict[str, int], local: dict[str, E.Symbol],
                   where: str) -> None:
        data.append(OPCODES[(st.op, st.mode)])
        if st.mode in ("imp", "acc"):
            return
        v = self.value_of(st.expr, where)
        if st.mode == "rel":
            if v.select is not None:
                raise AssemblerError(f"{where}: a branch takes a label")
            if v.symbol is not None and v.symbol in labels:
                disp = labels[v.symbol] + v.addend - (len(data) + 1)
                if not -128 <= disp <= 127:
                    raise AssemblerError(
                        f"{where}: branch to {v.symbol} is {disp} bytes "
                        f"away, out of reach"
                    )
                data.append(disp & 0xFF)
                return
            if v.symbol is None:
                raise AssemblerError(f"{where}: a branch takes a label")
            sec.relocs.append(E.Reloc(
                len(data), E.R_MOS_PCREL_8,
                self.symbol(v.symbol, local, where), v.addend - 1,
            ))
            data.append(0)
            return
        if st.mode == "imm":
            self.field(sec, data, v, 1, local, where, data_byte=True)
            return
        width = SIZES[st.mode] - 1
        if width == 1 and v.select is None and v.symbol is None:
            if not 0 <= v.addend <= 0xFF:
                raise AssemblerError(
                    f"{where}: ${v.addend:X} isn't in zero page"
                )
        if width == 1 and v.symbol is not None and not self.in_zero_page(v):
            raise AssemblerError(
                f"{where}: {st.op} ({st.mode}) needs a zero-page address, "
                f"and {v.symbol} isn't known to be in zero page"
            )
        self.field(sec, data, v, width, local, where, zero_page=width == 1)

    def field(self, sec: E.Section, data: bytearray, v: Value, width: int,
              local: dict[str, E.Symbol], where: str, *,
              data_byte: bool = False, zero_page: bool = False) -> None:
        """Appends a `width`-byte field holding `v`, with a relocation
        when `v` names a symbol."""
        if v.symbol is None:
            value = _select(v)
            lo, hi = -(1 << (8 * width - 1)), (1 << (8 * width)) - 1
            if not lo <= value <= hi:
                raise AssemblerError(
                    f"{where}: {value} doesn't fit in {width} byte(s)"
                )
            data += (value & ((1 << (8 * width)) - 1)).to_bytes(width, "little")
            return
        sym = self.symbol(v.symbol, local, where)
        if v.select is not None:
            if width != 1:
                raise AssemblerError(
                    f"{where}: `<` or `>` gives one byte, in a "
                    f"{width}-byte field"
                )
            rtype = E.R_MOS_ADDR16_LO if v.select == "lo" else E.R_MOS_ADDR16_HI
        elif width == 1:
            rtype = E.R_MOS_ADDR8 if zero_page else E.R_MOS_IMM8
            if zero_page and not sym.defined:
                sym.other |= E.STO_MOS_ZEROPAGE
        elif width == 2:
            rtype = E.R_MOS_ADDR16
        else:
            raise AssemblerError(
                f"{where}: a {width}-byte field can't hold the address "
                f"of {v.symbol}"
            )
        sec.relocs.append(E.Reloc(len(data), rtype, sym, v.addend))
        data += bytes(width)


def assemble(
    text: str, filename: str = "<input>",
    link_symbols: dict[str, int | None] | None = None,
    keep_source: bool = True,
) -> E.ElfFile:
    """The relocatable object for the dasm text `text`. `filename`
    names it in messages and in the object's `STT_FILE` symbol.

    `link_symbols` gives the addresses of names the linker places, for
    choosing addressing modes, over the values the text's `EQU`s give
    them. None for a name means it's outside zero page. They stay
    undefined in the object.

    With `keep_source`, the object keeps `text` in its section
    `.c6502.asm`, so that the linker can assemble the unit again when
    the program's addresses contradict the unit's (passes/linker.py).
    Raises `AssemblerError`."""
    return _Assembler(text, filename, link_symbols or {}, keep_source).run()
