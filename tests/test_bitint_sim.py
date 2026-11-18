"""Runs every operator on the bit-precise integer types, `_BitInt(N)`
for N from 2 to 64 and `unsigned _BitInt(N)` for N from 1 to 64, in
the simulator, and checks each result against a Python model of C23's
rules with c6502's choices (bit_precise.py).

Each case is one small function that applies one operator to its
parameters, so that neither pipeline can fold it, and `main` calls it
twice with different operands and stores each result in a global. The
operators are `+`, `-`, `*`, `/`, `%`, `&`, `|`, `^`, `<<` and `>>` by
a parameter and by a constant, unary `-` and `~`, `<`, `<=`, `==`,
conversions to and from `long long`, `unsigned long long`, `int`, and
`unsigned char`, and conversions to and from `double` and `float`.

A result of a bit-precise type is compared byte for byte, padding
bits included, so the test also checks that every operation leaves
them as the sign or zero extension of the value. The operands sit on
the edges of each type's range as well as inside it, so `+`, `-`, `*`,
and `<<` wrap at N bits, and signed overflow wraps too, which is what
c6502 does with it. Division by zero and the minimum divided by -1
are left out, since C leaves them undefined, and shift counts are
less than N.

A second grid applies `++`, postfix `--`, `+=`, `*=`, `<<=`, and `>>=`
to a bit-precise object in each storage class of tests/test_op_matrix_sim.py
(an automatic local, a block-scope `static`, a global, a zero-page
global, `*p`, `a[i]`, and a struct member), for a set of widths that
covers each storage size and padding."""

from __future__ import annotations

import random
import shutil
import struct
import unittest
from dataclasses import dataclass

import bit_precise
from sim.harness import build_sim

_CHUNK = 40
# Bytes of call results per program. Without -O, main holds each
# call's result in a frame temp, and a frame is at most 253 bytes.
_RESULT_BUDGET = 150
_ZP_BUDGET = 60


def _ctype(width: int, signed: bool) -> str:
    return f"{'' if signed else 'unsigned '}_BitInt({width})"


def _types() -> list[tuple[int, bool]]:
    return [(n, s) for n in range(1, 65) for s in (False, True)
            if n >= bit_precise.min_width(s)]


def _bytes(value: int, size: int) -> bytes:
    return (value & ((1 << (8 * size)) - 1)).to_bytes(size, "little")


def _literal(v: int, size: int = 8) -> str:
    """`v` modulo 2^(8 * size) as an unsigned constant of `size` bytes
    (2 or 8). A cast of it to a type of that size or to a bit-precise
    type converts it to `v` (modulo 2^N, which wraps a signed type as
    c6502 does), and c99_to_tac folds the cast, so no call's operand
    takes a temporary in main's frame."""
    suffix = "ULL" if size == 8 else "U"
    return f"0x{v & ((1 << (8 * size)) - 1):X}{suffix}"


@dataclass
class _Case:
    name: str
    text: str            # the function, named f_<name>
    calls: list[str]     # the argument lists of the two calls
    rtype: str           # the C type of the result
    size: int            # bytes of the result
    want: list[bytes]    # the expected bytes of each result
    what: str


def _values(width: int, signed: bool, rng: random.Random) -> list[int]:
    """Operands of the type: its edges and two values inside it."""
    lo, hi = bit_precise.value_range(width, signed)
    vals = [lo, hi, 0, 1, hi // 2 + 1]
    if signed:
        vals.append(-1)
    vals += [rng.randint(lo, hi) for _ in range(2)]
    return vals


def _trunc_div(a: int, b: int) -> int:
    q = abs(a) // abs(b)
    return -q if (a < 0) != (b < 0) else q


_BINARY = {
    "add": "+", "sub": "-", "mul": "*", "div": "/", "mod": "%",
    "and": "&", "or": "|", "xor": "^", "lt": "<", "le": "<=", "eq": "==",
}


def _binary_value(op: str, a: int, b: int) -> int:
    match op:
        case "add": return a + b
        case "sub": return a - b
        case "mul": return a * b
        case "div": return _trunc_div(a, b)
        case "mod": return a - _trunc_div(a, b) * b
        case "and": return a & b
        case "or": return a | b
        case "xor": return a ^ b
        case "lt": return int(a < b)
        case "le": return int(a <= b)
        case "eq": return int(a == b)
    raise ValueError(op)


def _operator_cases() -> list[_Case]:
    rng = random.Random(23)
    out: list[_Case] = []
    for width, signed in _types():
        t = _ctype(width, signed)
        size = bit_precise.storage_bytes(width)
        vals = _values(width, signed, rng)
        wrap = lambda v: bit_precise.wrap(v, width, signed)
        tag = f"{'s' if signed else 'u'}{width}"

        def add(op, text, calls, rtype, rsize, want, what):
            out.append(_Case(f"{tag}_{op}", text, calls, rtype, rsize,
                             want, f"{what} on {t}"))

        for op, sym in _BINARY.items():
            pairs = []
            for _ in range(40):
                a, b = rng.choice(vals), rng.choice(vals)
                if op in ("div", "mod"):
                    if b == 0 or (signed and b == -1
                                  and a == bit_precise.value_range(
                                      width, signed)[0]):
                        continue
                pairs.append((a, b))
                if len(pairs) == 2:
                    break
            compare = op in ("lt", "le", "eq")
            rtype = "int" if compare else t
            rsize = 2 if compare else size
            want = []
            for a, b in pairs:
                v = _binary_value(op, a, b)
                want.append(_bytes(v if compare else wrap(v), rsize))
            add(op, f"{rtype} f_{tag}_{op}({t} a, {t} b) "
                    f"{{ return a {sym} b; }}\n",
                [f"({t}){_literal(a)}, ({t}){_literal(b)}"
                 for a, b in pairs],
                rtype, rsize, want, op)
        for op, sym in (("shl", "<<"), ("shr", ">>")):
            pairs = [(rng.choice(vals), width - 1),
                     (rng.choice(vals), rng.randrange(width))]
            want = [_bytes(wrap(a << n if op == "shl" else a >> n), size)
                    for a, n in pairs]
            add(op, f"{t} f_{tag}_{op}({t} a, int n) "
                    f"{{ return a {sym} n; }}\n",
                [f"({t}){_literal(a)}, {n}" for a, n in pairs],
                t, size, want, op)
            k = max(1, width // 3) if width > 1 else 0
            if width > 1:
                cases = [rng.choice(vals), rng.choice(vals)]
                want = [_bytes(wrap(a << k if op == "shl" else a >> k), size)
                        for a in cases]
                add(f"{op}c", f"{t} f_{tag}_{op}c({t} a) "
                              f"{{ return a {sym} {k}; }}\n",
                    [f"({t}){_literal(a)}" for a in cases],
                    t, size, want, f"{op} by {k}")
        for op, sym in (("neg", "-"), ("not", "~")):
            cases = [rng.choice(vals), rng.choice(vals)]
            want = [_bytes(wrap(-a if op == "neg" else ~a), size)
                    for a in cases]
            add(op, f"{t} f_{tag}_{op}({t} a) {{ return {sym}a; }}\n",
                [f"({t}){_literal(a)}" for a in cases], t, size, want, op)
        # Conversions out of the type and into it.
        for other, osize, osigned in (
            ("long long", 8, True), ("unsigned long long", 8, False),
            ("int", 2, True), ("unsigned char", 1, False),
        ):
            otag = other.replace(" ", "_")
            cases = [rng.choice(vals), rng.choice(vals)]
            want = [_bytes(bit_precise.wrap(a, 8 * osize, osigned), osize)
                    for a in cases]
            add(f"to_{otag}", f"{other} f_{tag}_to_{otag}({t} a) "
                              f"{{ return a; }}\n",
                [f"({t}){_literal(a)}" for a in cases], other, osize,
                want, f"conversion to {other}")
            olo, ohi = bit_precise.value_range(8 * osize, osigned)
            cases = [rng.randint(olo, ohi), rng.choice([olo, ohi])]
            want = [_bytes(wrap(x), size) for x in cases]
            add(f"from_{otag}", f"{t} f_{tag}_from_{otag}({other} x) "
                                f"{{ return x; }}\n",
                [f"({other}){_literal(x, 2 if osize == 2 else 8)}"
                 for x in cases], t, size, want,
                f"conversion from {other}")
        lo, hi = bit_precise.value_range(width, signed)
        for fp, fsize, fmt, limit in (("double", 8, "<d", 1 << 53),
                                      ("float", 4, "<f", 1 << 24)):
            flo, fhi = max(lo, -limit + 1), min(hi, limit - 1)
            cases = [rng.randint(flo, fhi), fhi]
            want = [struct.pack(fmt, float(a)) for a in cases]
            add(f"to_{fp}", f"{fp} f_{tag}_to_{fp}({t} a) {{ return a; }}\n",
                [f"({t}){_literal(a)}" for a in cases], fp, fsize, want,
                f"conversion to {fp}")
            # A value with a fraction, which the conversion drops, from
            # inside the range where the floating type is exact.
            cases = [rng.randint(flo, fhi), flo]
            frac = [a + (0.5 if a >= 0 else -0.5) if abs(a) < limit // 2
                    else float(a) for a in cases]
            want = [_bytes(int(d), size) for d in frac]
            add(f"from_{fp}", f"{t} f_{tag}_from_{fp}({fp} d) "
                              f"{{ return d; }}\n",
                [repr(d) + ("f" if fp == "float" else "") for d in frac],
                t, size, want, f"conversion from {fp}")
    return out


# The storage grid's widths: each storage size from 1 to 8 bytes, with
# and without padding.
_GRID_WIDTHS = [1, 5, 8, 12, 16, 17, 24, 31, 40, 48, 55, 64]
_STORAGE = ["local", "static", "global", "zp", "ptr", "array", "member"]
_GRID_OPS = {
    "preinc": "++X", "postdec": "X--", "add": "X += 3", "mul": "X *= 3",
    "shl": "X <<= 2", "shr": "X >>= 1",
}


def _grid_value(op: str, x: int, width: int, signed: bool) -> tuple[int, int]:
    """`(expression value, object afterwards)`. A compound assignment
    with an `int` operand computes in the common type, which is `int`
    for a bit-precise type of at most 16 bits (C23 §6.3.1.8), and the
    result converts back, so either way the object wraps at N bits."""
    w = lambda v: bit_precise.wrap(v, width, signed)
    match op:
        case "preinc":
            return w(x + 1), w(x + 1)
        case "postdec":
            return x, w(x - 1)
        case "add":
            v = w(x + 3)
        case "mul":
            v = w(x * 3)
        case "shl":
            v = w(x << 2)
        case "shr":
            v = w(x >> 1)
    return v, v


def _grid_cases() -> tuple[list[_Case], dict[str, list[str]]]:
    rng = random.Random(64)
    out: list[_Case] = []
    decls: dict[str, list[str]] = {}
    for width in _GRID_WIDTHS:
        for signed in (False, True):
            if width < bit_precise.min_width(signed):
                continue
            t = _ctype(width, signed)
            size = bit_precise.storage_bytes(width)
            lo, hi = bit_precise.value_range(width, signed)
            tag = f"{'s' if signed else 'u'}{width}"
            for storage in _STORAGE:
                for op, text in _GRID_OPS.items():
                    # A shift count must be less than the width.
                    if (op == "shl" and width <= 2
                            or op == "shr" and width == 1):
                        continue
                    start = rng.choice([lo, hi, rng.randint(lo, hi)])
                    value, after = _grid_value(op, start, width, signed)
                    name = f"g_{tag}_{storage}_{op}"
                    init = f"({t}){_literal(start)}"
                    d: list[str] = []
                    pre = ""
                    match storage:
                        case "local":
                            pre = f"    {t} x = {init};\n"
                            obj = "x"
                        case "static":
                            pre = f"    static {t} x = {init};\n"
                            obj = "x"
                        case "global":
                            d.append(f"{t} v_{name} = {init};")
                            obj = f"v_{name}"
                        case "zp":
                            d.append(f"{t} v_{name} [[c6502::zeropage]] = "
                                     f"{init};")
                            obj = f"v_{name}"
                        case "ptr":
                            d.append(f"{t} v_{name} = {init};")
                            pre = f"    {t} *p = &v_{name};\n"
                            obj = "(*p)"
                        case "array":
                            d.append(f"{t} v_{name}[3] = {{0, {init}, 0}};")
                            obj = f"v_{name}[one]"
                        case "member":
                            d.append(f"struct S_{tag} v_{name} = "
                                     f"{{0, {init}}};")
                            obj = f"v_{name}.m"
                    expr = "(" + text.replace("X", obj) + ")"
                    body = (f"    {t} r = {expr};\n"
                            f"    return r == ({t}){_literal(value)} ? "
                            f"{obj} : ({t})0;\n")
                    out.append(_Case(
                        name,
                        f"{t} f_{name}(void) {{\n{pre}{body}}}\n",
                        [""], t, size, [_bytes(after, size)],
                        f"{op} on {storage} {t} (start {start})",
                    ))
                    decls[name] = d
    return out, decls


def _programs(cases: list[_Case], decls: dict[str, list[str]]):
    """Yields `(source, cases)` for each program."""
    chunk: list[_Case] = []
    budget = 0
    zp = 0

    def emit(chunk):
        head = ["unsigned char one = 1;"]
        for width in _GRID_WIDTHS:
            for signed in (False, True):
                if width >= bit_precise.min_width(signed):
                    tag = f"{'s' if signed else 'u'}{width}"
                    head.append(f"struct S_{tag} {{ unsigned char pad; "
                                f"{_ctype(width, signed)} m; }};")
        body, results, calls = [], [], []
        for c in chunk:
            head += decls.get(c.name, [])
            body.append(c.text)
            for j, args in enumerate(c.calls):
                results.append(f"{c.rtype} r_{c.name}_{j};")
                calls.append(f"    r_{c.name}_{j} = f_{c.name}({args});")
        src = "\n".join(head + results) + "\n" + "".join(body)
        src += "int main(void) {\n" + "\n".join(calls) + "\n    return 0;\n}\n"
        return src, list(chunk)

    for c in cases:
        need = c.size * len(c.calls)
        extra = c.size if "_zp_" in c.name else 0
        if chunk and (len(chunk) >= _CHUNK or budget + need > _RESULT_BUDGET
                      or zp + extra > _ZP_BUDGET):
            yield emit(chunk)
            chunk, budget, zp = [], 0, 0
        chunk.append(c)
        budget += need
        zp += extra
    if chunk:
        yield emit(chunk)


_OPERATOR_PROGRAMS = list(_programs(_operator_cases(), {}))
_GRID_PROGRAMS = list(_programs(*_grid_cases()))


def _check(test: unittest.TestCase, programs, optimize: bool, shard: int,
           shards: int) -> None:
    failures: list[str] = []
    for k, (src, cases) in enumerate(programs):
        if k % shards != shard:
            continue
        try:
            sim = build_sim(src, optimize=optimize)
            sim.run()
        except Exception as e:  # report the program, keep going
            failures.append(f"{cases[0].name}..: {type(e).__name__}: {e}")
            continue
        mem = sim.mpu.memory
        for c in cases:
            for j, want in enumerate(c.want):
                addr = sim.symbols[f"r_{c.name}_{j}"]
                got = bytes(mem[addr:addr + c.size])
                if got != want:
                    failures.append(
                        f"{c.name}: {c.what}, call {c.calls[j]!r}: got "
                        f"{got[::-1].hex()}, want {want[::-1].hex()}"
                    )
    test.assertEqual(failures, [], msg="\n" + "\n".join(failures[:40]))


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestBitIntOperators(unittest.TestCase):
    pass


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestBitIntStorage(unittest.TestCase):
    pass


# The programs split into shards, one test method each, so a parallel
# run (`pytest -n auto`) spreads them over its workers.
_SHARDS = 16
for _k in range(_SHARDS):
    for _opt in (False, True):
        _suffix = f"{'optimized' if _opt else 'unoptimized'}_{_k}"
        setattr(TestBitIntOperators, f"test_{_suffix}",
                lambda self, k=_k, o=_opt: _check(
                    self, _OPERATOR_PROGRAMS, o, k, _SHARDS))
_GRID_SHARDS = 4
for _k in range(_GRID_SHARDS):
    for _opt in (False, True):
        _suffix = f"{'optimized' if _opt else 'unoptimized'}_{_k}"
        setattr(TestBitIntStorage, f"test_{_suffix}",
                lambda self, k=_k, o=_opt: _check(
                    self, _GRID_PROGRAMS, o, k, _GRID_SHARDS))


if __name__ == "__main__":
    unittest.main()
