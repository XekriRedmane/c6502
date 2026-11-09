"""Random integer-expression fuzzer.

Each program declares six variables of random integer types (as
initialized globals, or as parameters of `check`, chosen per seed),
then evaluates eight random expressions over them. Every expression's
value is computed here as well, under C99's rules with c6502's widths
(char 8 bits, int 16, long 32, plain char unsigned), and the program
compares each result against that constant. `main` returns a bitmask
with bit i set when expression i came out wrong, so the expected
return is 0 on every path.

Expressions that would hit undefined behavior are discarded and
regenerated: signed overflow, division by zero, a shift count that
is negative or not less than the width, and a left shift of a
negative value. Right shift of a negative value is implementation-
defined, and c6502 shifts arithmetically, so the model does too.

The parameter form exercises zp_abi parameters under --optimize. The
global form exercises static storage and static initializers.

    uv run python -m fuzz.exprs --start 0 --count 300
"""

from __future__ import annotations

import random
import sys

from fuzz.runner import fuzz_main

# key -> (C spelling, width in bits, signed)
TYPES = {
    "schar": ("signed char", 8, True),
    "uchar": ("unsigned char", 8, False),
    "int": ("int", 16, True),
    "uint": ("unsigned int", 16, False),
    "long": ("long", 32, True),
    "ulong": ("unsigned long", 32, False),
}
# Conversion rank (C99 §6.3.1.1) of the promoted types.
RANK = {"int": 1, "uint": 1, "long": 2, "ulong": 2}


class _UB(Exception):
    """The expression being generated would be undefined behavior."""


def wrap(v: int, t: str) -> int:
    """`v` converted to type `t` (modulo its width, then read with
    its signedness)."""
    _, w, signed = TYPES[t]
    v &= (1 << w) - 1
    if signed and v >> (w - 1):
        v -= 1 << w
    return v


def promote(t: str) -> str:
    """C99 §6.3.1.1.2 integer promotion. With a 16-bit int, both char
    types promote to int."""
    return "int" if t in ("schar", "uchar") else t


def common(a: str, b: str) -> str:
    """C99 §6.3.1.8 usual arithmetic conversions on two integer types."""
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
    return {"long": "ulong"}[s]


def _in_range(v: int, t: str) -> bool:
    return wrap(v, t) == v


def lit(v: int, t: str) -> str:
    """A C expression of type `t` with value `v`. Negative values are
    written as a cast of the unsigned bit pattern, which is a plain
    integer constant expression."""
    spelling, w, _ = TYPES[t]
    if t in ("schar", "uchar"):
        return f"(({spelling}){v & 0xFF})"
    if v < 0:
        return f"(({spelling}){v & ((1 << w) - 1)}UL)"
    suffix = {"int": "", "uint": "u", "long": "L", "ulong": "UL"}[t]
    return f"(({spelling}){v}{suffix})"


def _gen(depth: int, vars_: list, rng: random.Random) -> tuple[str, int, str]:
    """A random expression as `(C text, value, type)`. Raises `_UB`
    when the expression would be undefined."""
    if depth == 0 or rng.random() < 0.25:
        if rng.random() < 0.7:
            name, t, v = rng.choice(vars_)
            return name, v, t
        t = rng.choice(list(TYPES))
        _, w, _ = TYPES[t]
        v = wrap(rng.choice([0, 1, 2, 7, 127, 128, 255, 256, 0x7FFF,
                             0x8000, 0xFFFF, 0x12345, 0x7FFFFFFF,
                             0x80000000, rng.getrandbits(w)]), t)
        return lit(v, t), v, t
    kind = rng.random()
    if kind < 0.12:
        a, av, _ = _gen(depth - 1, vars_, rng)
        t = rng.choice(list(TYPES))
        return f"(({TYPES[t][0]})({a}))", wrap(av, t), t
    if kind < 0.2:
        a, av, at = _gen(depth - 1, vars_, rng)
        op = rng.choice(["-", "~", "!"])
        if op == "!":
            return f"(!{a})", int(av == 0), "int"
        pt = promote(at)
        r = -av if op == "-" else ~av
        if TYPES[pt][2] and not _in_range(r, pt):
            raise _UB
        return f"({op}{a})", wrap(r, pt), pt
    a, av, at = _gen(depth - 1, vars_, rng)
    b, bv, bt = _gen(depth - 1, vars_, rng)
    op = rng.choice(["+", "-", "*", "/", "%", "&", "|", "^", "<<", ">>",
                     "<", ">", "<=", ">=", "==", "!=", "&&", "||"])
    text = f"({a} {op} {b})"
    if op in ("&&", "||"):
        r = (bool(av) and bool(bv)) if op == "&&" else (bool(av) or bool(bv))
        return text, int(r), "int"
    if op in ("<<", ">>"):
        # C99 §6.5.7: each operand promotes on its own, and the result
        # has the promoted left operand's type.
        pt = promote(at)
        if bv < 0 or bv >= TYPES[pt][1]:
            raise _UB
        if op == ">>":
            return text, av >> bv, pt
        if av < 0:
            raise _UB
        r = av << bv
        if TYPES[pt][2] and not _in_range(r, pt):
            raise _UB
        return text, wrap(r, pt), pt
    ct = common(at, bt)
    x, y = wrap(av, ct), wrap(bv, ct)
    if op in ("<", ">", "<=", ">=", "==", "!="):
        r = {"<": x < y, ">": x > y, "<=": x <= y, ">=": x >= y,
             "==": x == y, "!=": x != y}[op]
        return text, int(r), "int"
    if op in ("/", "%"):
        if y == 0:
            raise _UB
        # C99 §6.5.5.6: the quotient truncates toward zero.
        q = abs(x) // abs(y)
        if (x < 0) != (y < 0):
            q = -q
        r = q if op == "/" else x - q * y
    else:
        r = {"+": x + y, "-": x - y, "*": x * y, "&": x & y,
             "|": x | y, "^": x ^ y}[op]
    if TYPES[ct][2] and not _in_range(r, ct):
        raise _UB
    return text, wrap(r, ct), ct


def make_program(seed: int, n_exprs: int = 8, depth: int = 3) -> tuple[str, int]:
    """The program for `seed`, and main()'s correct return (0)."""
    rng = random.Random(seed)
    vars_ = []
    for i in range(6):
        t = rng.choice(list(TYPES))
        _, w, _ = TYPES[t]
        v = wrap(rng.choice([rng.getrandbits(w), rng.getrandbits(4),
                             1 << (w - 1), (1 << w) - 1, 1, 0]), t)
        vars_.append((f"v{i}", t, v))
    exprs = []
    while len(exprs) < n_exprs:
        try:
            exprs.append(_gen(depth, vars_, rng))
        except _UB:
            pass
    use_params = rng.random() < 0.5
    lines = []
    if use_params:
        params = ", ".join(f"{TYPES[t][0]} {n}" for n, t, _ in vars_)
        lines.append(f"int check({params}) {{")
    else:
        for n, t, v in vars_:
            lines.append(f"{TYPES[t][0]} {n} = {lit(v, t)};")
        lines.append("int check(void) {")
    lines.append("    int fail = 0;")
    for i, (text, val, t) in enumerate(exprs):
        lines.append(
            f"    {{ {TYPES[t][0]} r = {text}; "
            f"if (r != {lit(val, t)}) fail |= {1 << i}; }}"
        )
    lines.append("    return fail;")
    lines.append("}")
    if use_params:
        args = ", ".join(lit(v, t) for _, t, v in vars_)
        lines.append(f"int main(void) {{ return check({args}); }}")
    else:
        lines.append("int main(void) { return check(); }")
    return "\n".join(lines) + "\n", 0


if __name__ == "__main__":
    sys.exit(fuzz_main(make_program, "exprs", sys.argv[1:]))
