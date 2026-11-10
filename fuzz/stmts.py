"""Random statement-level fuzzer: control flow, the assignment
operators, and every storage class.

Each program has a few helper functions and `main`. Their bodies are
random statements:

  * `if` / `else`, `for`, `while`, and `do` loops (nested up to three
    deep, each run 1 to 4 times), `switch` with fall-through and
    `default`, `break`, `continue`, and early `return` in helpers;
  * assignments, and the operators `++` / `--` (before and after) and
    `+=` `-=` `*=` `&=` `|=` `^=` `<<=` `>>=`, as statements and with
    their value used in an assignment, a `mix()` call, an `if` or
    `switch` condition, a `return`, or an argument;
  * objects that are locals (some passed to a helper by pointer, so
    they're address-taken), block-scope statics, globals, zero-page
    globals, `*gp` for a global pointer to a global, elements of a
    global array (constant or loop-counter index), and members of a
    global struct.

The helpers form a DAG (a helper calls only earlier ones), so there's
no recursion. `mix(v)` folds a value into the global checksum `acc`,
and the bodies call it often, so intermediate values are observed;
`main` ends by mixing every global and returns `acc & 0x7FFF`.

The generator builds the program as a tree, renders it to C, and runs
the same tree in a Python interpreter (`_Interp`) under C99's rules
with c6502's widths (char 8 bits, int 16, long 32, long long 64). The
result is written as `// expect: N`, so every path (TAC interpreter,
unoptimized, optimized) is checked against it.

Undefined behavior is avoided by construction where it's structural
and by rejection where it depends on values:

  * An expression contains at most one modification (an operator or
    a call), and the object it modifies appears nowhere else in the
    statement. A global pointer always points to the same global, so
    `*gp` and that global count as one object. A pointer parameter
    points to a caller's local, which the callee can't name.
  * A call appears only as a whole right-hand side, a `mix()`
    argument, or an expression statement, with pure arguments, so
    nothing else in its statement can see what it changes.
  * Loop counters are never written by the loop body, so every loop
    ends. A `while` increments its counter in the condition and a
    `do` in the bottom test, so `continue` can't skip the increment.
  * Signed overflow (including `++` of a signed maximum), a shift
    count outside the promoted width, and a left shift of a negative
    value raise `_UB` in the interpreter, and the program is replaced
    by another drawn from the same seed. Conversion to a narrower
    signed type wraps, as c6502 implements it.

    uv run python -m fuzz.stmts --start 0 --count 200
"""

from __future__ import annotations

import random
import sys
from dataclasses import dataclass, field

from fuzz.runner import fuzz_main

# key -> (C spelling, width in bits, signed)
TYPES = {
    "schar": ("signed char", 8, True),
    "uchar": ("unsigned char", 8, False),
    "int": ("int", 16, True),
    "uint": ("unsigned int", 16, False),
    "long": ("long", 32, True),
    "ulong": ("unsigned long", 32, False),
    "llong": ("long long", 64, True),
    "ullong": ("unsigned long long", 64, False),
}
# Object types, weighted toward the narrow ones the 6502 handles in
# fewer instructions, where the 1- and 2-byte lowerings live.
_TYPE_WEIGHTS = {
    "schar": 6, "uchar": 8, "int": 6, "uint": 8,
    "long": 3, "ulong": 3, "llong": 1, "ullong": 1,
}
# Statements per function, counting nested ones. Without --optimize
# every TAC temporary gets its own frame byte, and a frame is at most
# 253 bytes, so a long body is rejected on that path.
_STMT_BUDGET = 12
# `mix()` calls per checksum function at the end of main, for the
# same reason.
_MIX_PER_FN = 4
RANK = {"int": 1, "uint": 1, "long": 2, "ulong": 2, "llong": 3, "ullong": 3}
_UNSIGNED_OF = {"int": "uint", "long": "ulong", "llong": "ullong"}


class _UB(Exception):
    """The program would hit undefined behavior."""


def wrap(v: int, t: str) -> int:
    _, w, signed = TYPES[t]
    v &= (1 << w) - 1
    if signed and v >> (w - 1):
        v -= 1 << w
    return v


def promote(t: str) -> str:
    return "int" if t in ("schar", "uchar") else t


def common(a: str, b: str) -> str:
    """C99 §6.3.1.8 usual arithmetic conversions."""
    a, b = promote(a), promote(b)
    if a == b:
        return a
    sa, sb = TYPES[a][2], TYPES[b][2]
    if sa == sb:
        return a if RANK[a] >= RANK[b] else b
    u, s = (a, b) if not sa else (b, a)
    if RANK[u] >= RANK[s]:
        return u
    if TYPES[s][1] > TYPES[u][1]:
        return s
    return _UNSIGNED_OF[s]


def _check(v: int, t: str) -> int:
    """`v` as type `t`, raising `_UB` on signed overflow."""
    if TYPES[t][2] and wrap(v, t) != v:
        raise _UB
    return wrap(v, t)


def lit(v: int, t: str) -> str:
    spelling, w, _ = TYPES[t]
    pattern = v & ((1 << w) - 1)
    return f"(({spelling}){pattern}ULL)"


# ---------------------------------------------------------------------------
# The program tree. Expressions and statements are tuples, tagged by
# their first element.
#
#   lvalue: ("var", name) | ("idx", array, index_expr) | ("mem", struct,
#           member) | ("deref", pointer_name)
#   expr:   ("const", v, t) | ("read", lvalue) | ("cast", t, e) |
#           ("un", op, e) | ("bin", op, a, b) | ("shift", op, a, k) |
#           ("cmp", op, a, b) | ("logic", op, a, b) | ("cond", c, a, b) |
#           ("op", kind, lvalue, k) | ("call", fn, args)
#   stmt:   ("assign", lvalue, e) | ("expr", e) | ("mix", e) |
#           ("if", c, then, else) | ("for", ctr, n, body) |
#           ("while", ctr, n, body) | ("do", ctr, n, body) |
#           ("switch", e, [(value, body, breaks)], default_or_None) |
#           ("break",) | ("continue",) | ("return", e)
# ---------------------------------------------------------------------------

_OPS = {
    "preinc": "++{x}", "predec": "--{x}", "postinc": "{x}++",
    "postdec": "{x}--", "add": "{x} += {k}", "sub": "{x} -= {k}",
    "mul": "{x} *= {k}", "and": "{x} &= {k}", "or": "{x} |= {k}",
    "xor": "{x} ^= {k}", "shl": "{x} <<= {k}", "shr": "{x} >>= {k}",
}


@dataclass
class Obj:
    """A named object: its C type and where it lives."""
    name: str
    t: str
    kind: str          # local, param, static, global, zp, array,
                       # struct, gptr, pparam
    init: int = 0
    target: str = ""   # gptr: the global it points to
    members: list = field(default_factory=list)   # struct: [(name, t)]


@dataclass
class Fn:
    name: str
    ret: str
    params: list       # [Obj]; a "pparam" Obj is a pointer parameter
    locals: list       # [Obj]
    statics: list      # [Obj]
    counters: list     # names of uchar loop counters for while / do
    body: list


@dataclass
class Prog:
    globals: list      # [Obj]
    fns: list          # [Fn], callee before caller; main last


# ---------------------------------------------------------------------------
# Rendering.
# ---------------------------------------------------------------------------

def _lv(lv) -> str:
    match lv:
        case ("var", n):
            return n
        case ("idx", a, i):
            return f"{a}[{_ex(i)}]"
        case ("mem", s, m):
            return f"{s}.{m}"
        case ("deref", p):
            return f"(*{p})"


def _ex(e) -> str:
    match e:
        case ("const", v, t):
            return lit(v, t)
        case ("read", lv):
            return _lv(lv)
        case ("cast", t, a):
            return f"(({TYPES[t][0]})({_ex(a)}))"
        case ("un", op, a):
            return f"({op}{_ex(a)})"
        case ("bin", op, a, b) | ("cmp", op, a, b) | ("logic", op, a, b):
            return f"({_ex(a)} {op} {_ex(b)})"
        case ("shift", op, a, k):
            return f"({_ex(a)} {op} {k})"
        case ("cond", c, a, b):
            return f"({_ex(c)} ? {_ex(a)} : {_ex(b)})"
        case ("op", kind, lv, k):
            return "(" + _OPS[kind].format(x=_lv(lv), k=k) + ")"
        case ("call", fn, args):
            return f"{fn}({', '.join(_arg(a) for a in args)})"


def _arg(a) -> str:
    if a[0] == "addr":
        return f"&{a[1]}"
    return _ex(a)


def _stmts(body, ind: str) -> list[str]:
    out: list[str] = []
    for s in body:
        out += _stmt(s, ind)
    return out


def _stmt(s, ind: str) -> list[str]:
    nxt = ind + "    "
    match s:
        case ("assign", lv, e):
            return [f"{ind}{_lv(lv)} = {_ex(e)};"]
        case ("expr", e):
            return [f"{ind}{_ex(e)};"]
        case ("mix", e):
            return [f"{ind}mix({_ex(e)});"]
        case ("if", c, then, els):
            out = [f"{ind}if ({_ex(c)}) {{"] + _stmts(then, nxt)
            if els:
                out += [f"{ind}}} else {{"] + _stmts(els, nxt)
            return out + [f"{ind}}}"]
        case ("for", ctr, n, body):
            return ([f"{ind}for (unsigned char {ctr} = 0; {ctr} < {n}; "
                     f"{ctr}++) {{"] + _stmts(body, nxt) + [f"{ind}}}"])
        case ("while", ctr, n, body):
            return ([f"{ind}{ctr} = 0;", f"{ind}while ({ctr}++ < {n}) {{"]
                    + _stmts(body, nxt) + [f"{ind}}}"])
        case ("do", ctr, n, body):
            return ([f"{ind}{ctr} = 0;", f"{ind}do {{"] + _stmts(body, nxt)
                    + [f"{ind}}} while (++{ctr} < {n});"])
        case ("switch", e, cases, default):
            out = [f"{ind}switch ({_ex(e)}) {{"]
            # A label needs a statement after it, so an empty body is
            # rendered as `;`.
            for value, body, breaks in cases:
                out.append(f"{ind}case {value}:")
                out += _stmts(body, nxt) or ([] if breaks else [f"{nxt};"])
                if breaks:
                    out.append(f"{nxt}break;")
            if default is not None:
                out.append(f"{ind}default:")
                out += _stmts(default, nxt) or [f"{nxt};"]
            return out + [f"{ind}}}"]
        case ("break",):
            return [f"{ind}break;"]
        case ("continue",):
            return [f"{ind}continue;"]
        case ("return", e):
            return [f"{ind}return {_ex(e)};"]


def render(p: Prog, expected: int) -> str:
    lines = [f"// expect: {expected}", "unsigned int acc;",
             "void mix(unsigned long v) {",
             "    acc = (unsigned int)((acc << 1) | (acc >> 15))"
             " ^ (unsigned int)v ^ (unsigned int)(v >> 16);",
             "}"]
    for g in p.globals:
        c = TYPES[g.t][0] if g.t in TYPES else ""
        match g.kind:
            case "global":
                lines.append(f"{c} {g.name} = {lit(g.init, g.t)};")
            case "zp":
                lines.append(f"{c} {g.name} __attribute__((zeropage)) = "
                             f"{lit(g.init, g.t)};")
            case "array":
                vals = ", ".join(lit(v, g.t) for v in g.init)
                lines.append(f"{c} {g.name}[4] = {{{vals}}};")
            case "struct":
                fields = " ".join(f"{TYPES[t][0]} {m};" for m, t in g.members)
                lines.append(f"struct S_{g.name} {{ unsigned char pad; "
                             f"{fields} }};")
                vals = ", ".join(lit(v, t) for (_, t), v in
                                 zip(g.members, g.init))
                lines.append(f"struct S_{g.name} {g.name} = {{0, {vals}}};")
            case "gptr":
                lines.append(f"{c} *{g.name} = &{g.target};")
    # main ends by mixing every global, a few per function.
    reads = [r for g in p.globals for r in _global_reads(g)]
    fins = [reads[i:i + _MIX_PER_FN] for i in range(0, len(reads),
                                                      _MIX_PER_FN)]
    for k, chunk in enumerate(fins):
        lines.append(f"void fin{k}(void) {{")
        lines += [f"    mix({_lv(r)});" for r in chunk]
        lines.append("}")
    for f in p.fns:
        if f.name == "main":
            lines.append("int main(void) {")
        else:
            params = ", ".join(
                f"{TYPES[o.t][0]} *{o.name}" if o.kind == "pparam"
                else f"{TYPES[o.t][0]} {o.name}" for o in f.params
            ) or "void"
            lines.append(f"{TYPES[f.ret][0]} {f.name}({params}) {{")
        for o in f.statics:
            lines.append(f"    static {TYPES[o.t][0]} {o.name} = "
                         f"{lit(o.init, o.t)};")
        for o in f.locals:
            lines.append(f"    {TYPES[o.t][0]} {o.name} = {lit(o.init, o.t)};")
        for c in f.counters:
            lines.append(f"    unsigned char {c};")
        lines += _stmts(f.body, "    ")
        if f.name == "main":
            lines += [f"    fin{k}();" for k in range(len(fins))]
            lines.append("    return (int)(acc & 0x7FFF);")
        lines.append("}")
    return "\n".join(lines) + "\n"


def _global_reads(g: Obj) -> list:
    match g.kind:
        case "global" | "zp":
            return [("var", g.name)]
        case "array":
            return [("idx", g.name, ("const", i, "int")) for i in range(4)]
        case "struct":
            return [("mem", g.name, m) for m, _ in g.members]
    return []


# ---------------------------------------------------------------------------
# Interpreter.
# ---------------------------------------------------------------------------

class _Ret(Exception):
    def __init__(self, v: int):
        self.v = v


class _Break(Exception):
    pass


class _Continue(Exception):
    pass


_STEP_LIMIT = 200_000


class _Interp:
    def __init__(self, p: Prog):
        self.p = p
        self.fns = {f.name: f for f in p.fns}
        self.mem: dict = {}
        self.types: dict = {}
        self.acc = 0
        self.steps = 0
        for g in p.globals:
            match g.kind:
                case "global" | "zp":
                    self.mem[("g", g.name)] = g.init
                    self.types[("g", g.name)] = g.t
                case "array":
                    for i, v in enumerate(g.init):
                        self.mem[("a", g.name, i)] = v
                        self.types[("a", g.name, i)] = g.t
                case "struct":
                    for (m, t), v in zip(g.members, g.init):
                        self.mem[("s", g.name, m)] = v
                        self.types[("s", g.name, m)] = t
        for f in p.fns:
            for o in f.statics:
                self.mem[("st", o.name)] = o.init
                self.types[("st", o.name)] = o.t
        self.gptr = {g.name: ("g", g.target) for g in p.globals
                     if g.kind == "gptr"}
        self.frames: list[dict] = []

    def mix(self, v: int, t: str) -> None:
        u = wrap(v, "ulong")
        rot = ((self.acc << 1) | (self.acc >> 15)) & 0xFFFF
        self.acc = rot ^ (u & 0xFFFF) ^ ((u >> 16) & 0xFFFF)

    # Locations -------------------------------------------------------

    def loc(self, lv):
        """The memory key of an lvalue."""
        match lv:
            case ("var", n):
                frame = self.frames[-1]
                if n in frame:
                    return frame[n]
                if ("g", n) in self.mem:
                    return ("g", n)
                return ("st", n)
            case ("idx", a, i):
                iv, _ = self.ev(i)
                if not 0 <= iv < 4:
                    raise _UB
                return ("a", a, iv)
            case ("mem", s, m):
                return ("s", s, m)
            case ("deref", p):
                frame = self.frames[-1]
                if p in frame:
                    return self.mem[frame[p]]   # a pointer parameter
                return self.gptr[p]

    def store(self, key, v: int) -> None:
        self.mem[key] = wrap(v, self.types[key])

    # Expressions -----------------------------------------------------

    def ev(self, e) -> tuple[int, str]:
        self.steps += 1
        if self.steps > _STEP_LIMIT:
            raise _UB
        match e:
            case ("const", v, t):
                return v, t
            case ("read", lv):
                k = self.loc(lv)
                return self.mem[k], self.types[k]
            case ("cast", t, a):
                v, _ = self.ev(a)
                return wrap(v, t), t
            case ("un", op, a):
                v, t = self.ev(a)
                if op == "!":
                    return int(v == 0), "int"
                pt = promote(t)
                v = wrap(v, pt)
                return (_check(-v, pt) if op == "-" else wrap(~v, pt)), pt
            case ("bin", op, a, b):
                av, at = self.ev(a)
                bv, bt = self.ev(b)
                ct = common(at, bt)
                return _arith(op, wrap(av, ct), wrap(bv, ct), ct), ct
            case ("shift", op, a, k):
                av, at = self.ev(a)
                pt = promote(at)
                return _shift(op, wrap(av, pt), k, pt), pt
            case ("cmp", op, a, b):
                av, at = self.ev(a)
                bv, bt = self.ev(b)
                ct = common(at, bt)
                x, y = wrap(av, ct), wrap(bv, ct)
                return int({"<": x < y, ">": x > y, "<=": x <= y,
                            ">=": x >= y, "==": x == y,
                            "!=": x != y}[op]), "int"
            case ("logic", op, a, b):
                av, _ = self.ev(a)
                if op == "&&":
                    return (int(bool(self.ev(b)[0])) if av else 0), "int"
                return (1 if av else int(bool(self.ev(b)[0]))), "int"
            case ("cond", c, a, b):
                cv, _ = self.ev(c)
                # The type is the usual arithmetic conversion of both
                # arms; only the chosen arm is evaluated (both are pure).
                at = self.type_of(a)
                bt = self.type_of(b)
                ct = common(at, bt)
                v, _ = self.ev(a if cv else b)
                return wrap(v, ct), ct
            case ("op", kind, lv, k):
                return self.op(kind, lv, k)
            case ("call", fn, args):
                return self.call(fn, args)
        raise TypeError(e)

    def type_of(self, e) -> str:
        """The type of a pure expression, without evaluating it for
        effect (it's evaluated, but pure expressions have none)."""
        saved = self.steps
        _, t = self.ev(e)
        self.steps = saved
        return t

    def op(self, kind: str, lv, k: int) -> tuple[int, str]:
        key = self.loc(lv)
        t = self.types[key]
        v = self.mem[key]
        pt = promote(t)
        if kind in ("preinc", "postinc", "predec", "postdec"):
            delta = 1 if "inc" in kind else -1
            new = wrap(_check(wrap(v, pt) + delta, pt), t)
            self.mem[key] = new
            return (new if kind.startswith("pre") else v), t
        if kind in ("shl", "shr"):
            r = _shift("<<" if kind == "shl" else ">>", wrap(v, pt), k, pt)
        else:
            ct = common(t, "int")
            op = {"add": "+", "sub": "-", "mul": "*", "and": "&",
                  "or": "|", "xor": "^"}[kind]
            r = _arith(op, wrap(v, ct), wrap(k, ct), ct)
        new = wrap(r, t)
        self.mem[key] = new
        return new, t

    def call(self, name: str, args) -> tuple[int, str]:
        f = self.fns[name]
        frame: dict = {}
        vals = []
        for p, a in zip(f.params, args):
            if p.kind == "pparam":
                vals.append(("ptr", self.loc(("var", a[1]))))
            else:
                v, _ = self.ev(a)
                vals.append(("val", wrap(v, p.t)))
        depth = len(self.frames)
        for p, (kind, v) in zip(f.params, vals):
            key = ("l", depth, p.name)
            frame[p.name] = key
            self.mem[key] = v
            self.types[key] = "ptr" if kind == "ptr" else p.t
        for o in f.locals:
            key = ("l", depth, o.name)
            frame[o.name] = key
            self.mem[key] = o.init
            self.types[key] = o.t
        for c in f.counters:
            key = ("l", depth, c)
            frame[c] = key
            self.mem[key] = 0
            self.types[key] = "uchar"
        self.frames.append(frame)
        try:
            self.run(f.body)
            result = 0
        except _Ret as r:
            result = r.v
        finally:
            self.frames.pop()
        return wrap(result, f.ret), f.ret

    # Statements ------------------------------------------------------

    def run(self, body) -> None:
        for s in body:
            self.exec(s)

    def exec(self, s) -> None:
        self.steps += 1
        if self.steps > _STEP_LIMIT:
            raise _UB
        frame = self.frames[-1]
        match s:
            case ("assign", lv, e):
                v, _ = self.ev(e)
                self.store(self.loc(lv), v)
            case ("expr", e):
                self.ev(e)
            case ("mix", e):
                v, t = self.ev(e)
                self.mix(v, t)
            case ("if", c, then, els):
                v, _ = self.ev(c)
                self.run(then if v else els)
            case ("for", ctr, n, body):
                key = ("l", len(self.frames) - 1, ctr)
                frame[ctr] = key
                self.types[key] = "uchar"
                self.mem[key] = 0
                while self.mem[key] < n:
                    try:
                        self.run(body)
                    except _Continue:
                        pass
                    except _Break:
                        break
                    self.mem[key] = wrap(self.mem[key] + 1, "uchar")
            case ("while", ctr, n, body):
                key = frame[ctr]
                self.mem[key] = 0
                while True:
                    old = self.mem[key]
                    self.mem[key] = wrap(old + 1, "uchar")
                    if not old < n:
                        break
                    try:
                        self.run(body)
                    except _Continue:
                        pass
                    except _Break:
                        break
            case ("do", ctr, n, body):
                key = frame[ctr]
                self.mem[key] = 0
                while True:
                    try:
                        self.run(body)
                    except _Continue:
                        pass
                    except _Break:
                        break
                    self.mem[key] = wrap(self.mem[key] + 1, "uchar")
                    if not self.mem[key] < n:
                        break
            case ("switch", e, cases, default):
                v, _ = self.ev(e)
                # The labeled bodies in source order; `default` is
                # rendered last. Control enters at the matching case,
                # or at `default`, and falls through until a break.
                segments = [(val, body, breaks) for val, body, breaks in cases]
                if default is not None:
                    segments.append(("default", default, False))
                start = next(
                    (i for i, seg in enumerate(segments) if seg[0] == v),
                    None,
                )
                if start is None and default is not None:
                    start = len(segments) - 1
                try:
                    if start is not None:
                        for _, body, breaks in segments[start:]:
                            self.run(body)
                            if breaks:
                                break
                except _Break:
                    pass
            case ("break",):
                raise _Break
            case ("continue",):
                raise _Continue
            case ("return", e):
                v, _ = self.ev(e)
                raise _Ret(v)


def _arith(op: str, x: int, y: int, ct: str) -> int:
    if op in ("/", "%"):
        if y == 0:
            raise _UB
        q = abs(x) // abs(y)
        if (x < 0) != (y < 0):
            q = -q
        r = q if op == "/" else x - q * y
    else:
        r = {"+": x + y, "-": x - y, "*": x * y, "&": x & y,
             "|": x | y, "^": x ^ y}[op]
    return _check(r, ct)


def _shift(op: str, v: int, k: int, pt: str) -> int:
    if not 0 <= k < TYPES[pt][1]:
        raise _UB
    if op == ">>":
        return v >> k
    if v < 0:
        raise _UB
    return _check(v << k, pt)


# ---------------------------------------------------------------------------
# Generation.
# ---------------------------------------------------------------------------

_INTERESTING = [0, 1, 2, 3, 0x7F, 0x80, 0xFF, 0x100, 0x7FFF, 0x8000,
                0xFFFF, 0x10000, 0x7FFFFFFF, 0x80000000]


class _Gen:
    def __init__(self, rng: random.Random):
        self.rng = rng
        self.n = 0
        self.zp_bytes = 0

    def fresh(self, prefix: str) -> str:
        self.n += 1
        return f"{prefix}{self.n}"

    def typ(self) -> str:
        ts = list(_TYPE_WEIGHTS)
        return self.rng.choices(ts, weights=[_TYPE_WEIGHTS[t] for t in ts])[0]

    def value(self, t: str) -> int:
        r = self.rng
        v = r.choice(_INTERESTING + [r.getrandbits(TYPES[t][1]),
                                      r.getrandbits(4)])
        return wrap(v, t)

    # Programs --------------------------------------------------------

    def program(self) -> Prog:
        r = self.rng
        globals_: list[Obj] = []
        for _ in range(r.randint(2, 4)):
            t = self.typ()
            globals_.append(Obj(self.fresh("g"), t, "global", self.value(t)))
        for _ in range(r.randint(0, 3)):
            t = self.typ()
            size = TYPES[t][1] // 8
            if self.zp_bytes + size > 40:
                break
            self.zp_bytes += size
            globals_.append(Obj(self.fresh("z"), t, "zp", self.value(t)))
        t = self.typ()
        globals_.append(Obj(self.fresh("ga"), t, "array",
                            [self.value(t) for _ in range(4)]))
        members = [(f"m{i}", self.typ()) for i in range(2)]
        globals_.append(Obj(self.fresh("gs"), "", "struct",
                            [self.value(t) for _, t in members],
                            members=members))
        scalars = [g for g in globals_ if g.kind in ("global", "zp")]
        for target in r.sample(scalars, k=min(len(scalars), r.randint(0, 2))):
            globals_.append(Obj(self.fresh("gp"), target.t, "gptr",
                                target=target.name))
        self.globals = globals_
        fns: list[Fn] = []
        for _ in range(r.randint(1, 3)):
            fns.append(self.function(fns, is_main=False))
        fns.append(self.function(fns, is_main=True))
        return Prog(globals_, fns)

    def function(self, callees: list[Fn], is_main: bool) -> Fn:
        r = self.rng
        name = "main" if is_main else self.fresh("f")
        params: list[Obj] = []
        if not is_main:
            for _ in range(r.randint(0, 3)):
                t = self.typ()
                params.append(Obj(self.fresh("p"), t, "param"))
            if r.random() < 0.5:
                params.append(Obj(self.fresh("pp"), self.typ(), "pparam"))
        locals_ = []
        for _ in range(r.randint(1, 3)):
            t = self.typ()
            locals_.append(Obj(self.fresh("l"), t, "local", self.value(t)))
        # One local per pointee type a callee takes by pointer, so every
        # call has a local of its own to pass.
        for c in callees:
            for p in c.params:
                if p.kind == "pparam" and not any(
                    o.t == p.t and o.kind == "local" for o in locals_
                ):
                    locals_.append(Obj(self.fresh("l"), p.t, "local",
                                       self.value(p.t)))
        statics = []
        if not is_main and r.random() < 0.6:
            t = self.typ()
            statics.append(Obj(self.fresh("s"), t, "static", self.value(t)))
        ret = "int" if is_main else self.typ()
        fn = Fn(name, ret, params, locals_, statics, [], [])
        self.fn = fn
        self.callees = callees
        self.counters_in_scope: list[str] = []
        self.budget = _STMT_BUDGET
        n = r.randint(3, 6)
        fn.body = self.block(n, depth=0, loops=0, breakable=False,
                             in_loop=False)
        if not is_main:
            fn.body.append(("return", self.expr(1)[0]))
        return fn

    # Objects ---------------------------------------------------------

    def lvalues(self) -> list[tuple]:
        """`(lvalue, type, key)` for every writable object in scope.
        `key` names the object for the one-modification rule."""
        out = []
        f = self.fn
        for o in f.locals + f.statics:
            out.append((("var", o.name), o.t, o.name))
        for o in f.params:
            if o.kind == "param":
                out.append((("var", o.name), o.t, o.name))
            else:
                out.append((("deref", o.name), o.t, "*" + o.name))
        for g in self.globals:
            match g.kind:
                case "global" | "zp":
                    out.append((("var", g.name), g.t, g.name))
                case "array":
                    out.append(((
                        "idx", g.name, self.index()), g.t, g.name))
                case "struct":
                    for m, t in g.members:
                        out.append((("mem", g.name, m), t, f"{g.name}.{m}"))
                case "gptr":
                    out.append((("deref", g.name), g.t, g.target))
        return out

    def index(self):
        r = self.rng
        if self.counters_in_scope and r.random() < 0.6:
            c = r.choice(self.counters_in_scope)
            return ("bin", "&", ("read", ("var", c)), ("const", 3, "int"))
        return ("const", r.randint(0, 3), "int")

    # Expressions -----------------------------------------------------

    def expr(self, depth: int, avoid: frozenset = frozenset()):
        """A pure expression, as `(expr, keys read)`, that reads none of
        the objects in `avoid`."""
        r = self.rng
        if depth == 0 or r.random() < 0.3:
            choices = [lv for lv in self.lvalues() if lv[2] not in avoid]
            if self.counters_in_scope and r.random() < 0.2:
                c = r.choice(self.counters_in_scope)
                return ("read", ("var", c)), frozenset()
            if choices and r.random() < 0.75:
                lv, _, key = r.choice(choices)
                return ("read", lv), frozenset([key])
            t = self.typ()
            return ("const", self.value(t), t), frozenset()
        k = r.random()
        if k < 0.1:
            a, ra = self.expr(depth - 1, avoid)
            return ("cast", self.typ(), a), ra
        if k < 0.18:
            a, ra = self.expr(depth - 1, avoid)
            return ("un", r.choice(["-", "~", "!"]), a), ra
        if k < 0.28:
            a, ra = self.expr(depth - 1, avoid)
            return ("shift", r.choice(["<<", ">>"]), a, r.randint(0, 7)), ra
        if k < 0.4:
            a, ra = self.expr(depth - 1, avoid)
            b, rb = self.expr(depth - 1, avoid)
            return ("cmp", r.choice(["<", ">", "<=", ">=", "==", "!="]),
                    a, b), ra | rb
        if k < 0.45:
            a, ra = self.expr(depth - 1, avoid)
            b, rb = self.expr(depth - 1, avoid)
            return ("logic", r.choice(["&&", "||"]), a, b), ra | rb
        if k < 0.5:
            c, rc = self.expr(depth - 1, avoid)
            a, ra = self.expr(depth - 1, avoid)
            b, rb = self.expr(depth - 1, avoid)
            return ("cond", c, a, b), rc | ra | rb
        a, ra = self.expr(depth - 1, avoid)
        b, rb = self.expr(depth - 1, avoid)
        op = r.choice(["+", "-", "*", "&", "|", "^", "+", "-", "&", "/", "%"])
        if op in ("/", "%"):
            b, rb = ("const", r.randint(1, 7), "int"), frozenset()
        return ("bin", op, a, b), ra | rb

    def side_op(self, avoid: frozenset = frozenset()):
        """An operator applied to a writable object, as `(expr, key
        written)`, or None when every object is in `avoid`."""
        r = self.rng
        choices = [lv for lv in self.lvalues() if lv[2] not in avoid]
        if not choices:
            return None
        lv, _, key = r.choice(choices)
        kind = r.choice(list(_OPS))
        k = r.randint(0, 7) if kind in ("shl", "shr") else r.choice(
            [1, 3, 5, 0x5A, 0x81, 0xFF])
        return ("op", kind, lv, k), key

    def with_op(self, depth: int):
        """An expression with at most one operator in it. Returns
        `(expr, keys read, key written or None)`."""
        r = self.rng
        if r.random() < 0.5:
            e, reads = self.expr(depth)
            return e, reads, None
        so = self.side_op()
        if so is None:
            e, reads = self.expr(depth)
            return e, reads, None
        op, key = so
        if r.random() < 0.5:
            return op, frozenset([key]), key
        rest, reads = self.expr(depth - 1, avoid=frozenset([key]))
        bop = r.choice(["+", "-", "^", "&", "|"])
        e = ("bin", bop, op, rest) if r.random() < 0.5 else (
            "bin", bop, rest, op)
        return e, reads | {key}, key

    def call(self):
        """A call to an earlier function with pure arguments, or None."""
        if not self.callees:
            return None
        r = self.rng
        f = r.choice(self.callees)
        args = []
        for p in f.params:
            if p.kind == "pparam":
                local = r.choice([o for o in self.fn.locals if o.t == p.t])
                args.append(("addr", local.name))
            else:
                args.append(self.expr(1)[0])
        return ("call", f.name, args)

    # Statements ------------------------------------------------------

    def block(self, n: int, depth: int, loops: int, breakable: bool,
              in_loop: bool) -> list:
        out = []
        while len(out) < n and self.budget > 0:
            self.budget -= 1
            out.append(self.statement(depth, loops, breakable, in_loop))
        return out

    def statement(self, depth: int, loops: int, breakable: bool,
                  in_loop: bool):
        r = self.rng
        k = r.random()
        nested = depth < 3
        if nested and k < 0.12:
            c, _, _ = self.with_op(self.rng.choice([1, 1, 2]))
            then = self.block(r.randint(1, 3), depth + 1, loops, breakable,
                              in_loop)
            els = (self.block(r.randint(1, 2), depth + 1, loops, breakable,
                              in_loop) if r.random() < 0.5 else [])
            return ("if", c, then, els)
        if nested and loops < 2 and k < 0.24:
            n = r.randint(1, 4)
            kind = r.choice(["for", "while", "do"])
            ctr = self.fresh("i" if kind == "for" else "w")
            if kind != "for":
                self.fn.counters.append(ctr)
            self.counters_in_scope.append(ctr)
            body = self.block(r.randint(1, 4), depth + 1, loops + 1, True,
                              True)
            self.counters_in_scope.remove(ctr)
            return (kind, ctr, n, body)
        if nested and k < 0.32:
            e, _, _ = self.with_op(self.rng.choice([1, 1, 2]))
            sel = ("bin", "&", ("cast", "int", e), ("const", 7, "int"))
            values = sorted(r.sample(range(8), r.randint(1, 4)))
            cases = [(v, self.block(r.randint(0, 2), depth + 1, loops, True,
                                    in_loop), r.random() < 0.6)
                     for v in values]
            default = (self.block(r.randint(1, 2), depth + 1, loops, True,
                                  in_loop) if r.random() < 0.6 else None)
            return ("switch", sel, cases, default)
        if breakable and k < 0.36:
            return ("break",)
        if in_loop and k < 0.39:
            return ("continue",)
        if self.fn.name != "main" and depth > 0 and k < 0.43:
            e, _, _ = self.with_op(self.rng.choice([1, 1, 2]))
            return ("return", e)
        if k < 0.5:
            c = self.call()
            if c is not None:
                return self.rng.choice([
                    ("expr", c), ("mix", c),
                    ("assign", ("var", r.choice(self.fn.locals).name), c),
                ])
        if k < 0.62:
            e, _, _ = self.with_op(self.rng.choice([1, 1, 2]))
            return ("mix", e)
        if k < 0.78:
            so = self.side_op()
            if so is not None:
                return ("expr", so[0])
        # An assignment, whose target isn't the one the value modifies.
        e, _, written = self.with_op(self.rng.choice([1, 1, 2]))
        targets = [lv for lv in self.lvalues() if lv[2] != written]
        lv, _, _ = r.choice(targets)
        return ("assign", lv, e)


def make_program(seed: int) -> tuple[str, int]:
    """The program for `seed` and main()'s correct return. A program
    that hits undefined behavior is replaced by the next one drawn from
    the same seed."""
    for attempt in range(200):
        rng = random.Random(seed * 1000 + attempt)
        p = _Gen(rng).program()
        interp = _Interp(p)
        try:
            interp.call("main", [])
            # The tail `render` gives main: mix every global, then
            # return the checksum.
            interp.frames.append({})
            for g in p.globals:
                for lv in _global_reads(g):
                    key = interp.loc(lv)
                    interp.mix(interp.mem[key], interp.types[key])
        except _UB:
            continue
        expected = interp.acc & 0x7FFF
        return render(p, expected), expected
    raise RuntimeError(f"seed {seed}: no program without undefined behavior")


if __name__ == "__main__":
    sys.exit(fuzz_main(make_program, "stmts", sys.argv[1:]))
