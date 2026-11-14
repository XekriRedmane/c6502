"""Systematic test of the assignment operators over every integer type,
storage class, and way of using the result.

Each case is one small function that applies one operator to one
object and uses the result one way:

  * types: the eight integer types, `unsigned char` to `long long`;
  * storage: an automatic local, a block-scope `static`, a global, a
    zero-page global, `*p` for a pointer to a global, `a[i]` with a
    runtime index, and a struct member;
  * operators: `++` and `--` before and after, `+=`, `-=`, `*=`,
    `&=`, `|=`, `^=`, `<<=`, and `>>=` (on a value with the sign bit
    set as well as without);
  * uses: the expression's value returned, the object read back after
    the operator, the value compared in an `if`, and the value passed
    through a call.

The starting values sit on a carry or borrow boundary (`0x00FF`
before `++`, `0x0100` before `--`, and their wider forms), so a
multi-byte operator has to carry into every byte. Each expected
result is computed in Python under C99's rules with c6502's widths.
Every program runs with and without --optimize.

This shape grid is what missed `return ++c;` for a static `unsigned
char`: the INC peephole left A without the new value, and neither the
chapter corpus nor the fuzzers had a 1-byte object incremented in
place and then returned.

Cases are split into programs of at most `_CHUNK` functions, at most
`_ZP_BUDGET` bytes of zero-page globals, and at most `_RESULT_BUDGET`
bytes of results, so the unoptimized code stays clear of the soft
stack (see `tests/test_const_shift_mul_sim.py`), its `main` frame
stays under 253 bytes, and the automatic zero-page range doesn't
fill."""

from __future__ import annotations

import shutil
import unittest
from dataclasses import dataclass

from sim.harness import build_sim

# (C type, bytes, signed)
_TYPES = [
    ("unsigned char", 1, False),
    ("signed char", 1, True),
    ("unsigned int", 2, False),
    ("int", 2, True),
    ("unsigned long", 4, False),
    ("long", 4, True),
    ("unsigned long long", 8, False),
    ("long long", 8, True),
]

_STORAGE = ["local", "static", "global", "zp", "ptr", "array", "member"]

# Operator name -> (C text applied to `X`, the starting-value kind).
_OPS = {
    "preinc": ("++X", "carry"),
    "predec": ("--X", "borrow"),
    "postinc": ("X++", "carry"),
    "postdec": ("X--", "borrow"),
    "add": ("X += 3", "carry"),
    "sub": ("X -= 5", "borrow"),
    "mul": ("X *= 3", "small"),
    "and": ("X &= 0x5A", "pattern"),
    "or": ("X |= 0x81", "pattern"),
    "xor": ("X ^= 0xFF", "pattern"),
    "shl": ("X <<= 1", "pattern"),
    "shr": ("X >>= 1", "pattern"),
    "shr_neg": ("X >>= 1", "negative"),
}

_USES = ["ret", "after", "if", "arg"]

_CHUNK = 40
_ZP_BUDGET = 80
# Bytes of results per program. Without --optimize, main holds each
# call's result in a frame temp, and a frame is at most 253 bytes.
_RESULT_BUDGET = 160


def _wrap(v: int, size: int, signed: bool) -> int:
    bits = size * 8
    v &= (1 << bits) - 1
    if signed and v >> (bits - 1):
        v -= 1 << bits
    return v


def _start(kind: str, size: int) -> int:
    """The starting value, as a bit pattern of `size` bytes."""
    bits = size * 8
    if kind == "carry":
        # Every byte but the top one is $FF, so ++ carries to the top.
        return (1 << (bits - 8)) - 1 if size > 1 else 0xFF
    if kind == "borrow":
        # Only the top byte is nonzero, so -- borrows from it.
        return 1 << (bits - 8) if size > 1 else 0x00
    if kind == "small":
        return int("12" + "34" * (size - 1), 16) if size > 1 else 0x5A
    if kind == "pattern":
        return int("5A" * size, 16)
    if kind == "negative":
        return int("A5" * size, 16)
    raise ValueError(kind)


def _promoted(size: int, signed: bool) -> tuple[int, bool]:
    """The type an operand of this type is promoted to (C99 6.3.1.1),
    with c6502's 16-bit int: char types become int."""
    if size == 1:
        return 2, True
    return size, signed


def _apply(op: str, x: int, size: int, signed: bool) -> tuple[int, int]:
    """Returns `(expression value, object afterwards)`, both as values
    of the object's type."""
    psize, psigned = _promoted(size, signed)
    p = _wrap(x, psize, psigned)
    w = lambda v: _wrap(v, size, signed)
    match op:
        case "preinc":
            new = w(p + 1)
            return new, new
        case "predec":
            new = w(p - 1)
            return new, new
        case "postinc":
            return x, w(p + 1)
        case "postdec":
            return x, w(p - 1)
        case "add":
            new = w(p + 3)
        case "sub":
            new = w(p - 5)
        case "mul":
            new = w(p * 3)
        case "and":
            new = w(p & 0x5A)
        case "or":
            new = w(p | 0x81)
        case "xor":
            new = w(p ^ 0xFF)
        case "shl":
            new = w(p << 1)
        case "shr" | "shr_neg":
            # Arithmetic for a negative signed value, as c6502 does.
            new = w(p >> 1)
        case _:
            raise ValueError(op)
    return new, new


def _literal(v: int, signed: bool) -> str:
    if signed:
        return f"{v}LL"
    return f"{v}ULL"


@dataclass
class _Case:
    name: str
    ctype: str
    size: int
    signed: bool
    storage: str
    op: str
    use: str
    start: int
    want: int        # as a value of the function's return type
    ret_size: int    # bytes of the function's return value

    def describe(self) -> str:
        return (f"{self.op} on {self.storage} {self.ctype} "
                f"(start {self.start:#x}), use {self.use}")


def _cases() -> list[_Case]:
    out: list[_Case] = []
    n = 0
    for ctype, size, signed in _TYPES:
        for storage in _STORAGE:
            for op, (_, kind) in _OPS.items():
                if op == "shr_neg" and not signed:
                    continue
                start = _wrap(_start(kind, size), size, signed)
                value, after = _apply(op, start, size, signed)
                for use in _USES:
                    if use == "if":
                        want, ret_size = 1, 2
                    elif use == "after":
                        want, ret_size = after, size
                    else:
                        want, ret_size = value, size
                    out.append(_Case(
                        f"c{n}", ctype, size, signed, storage, op, use,
                        start, want, ret_size,
                    ))
                    n += 1
    return out


def _tag(ctype: str) -> str:
    return ctype.replace(" ", "_")


def _function(c: _Case) -> tuple[list[str], str]:
    """Returns `(file-scope declarations, the function)` for case `c`."""
    t = c.ctype
    init = _literal(c.start, c.signed)
    decls: list[str] = []
    pre = ""
    match c.storage:
        case "local":
            pre = f"    {t} x = ({t})({init});\n"
            obj = "x"
        case "static":
            pre = f"    static {t} x = ({t})({init});\n"
            obj = "x"
        case "global":
            decls.append(f"{t} g_{c.name} = ({t})({init});")
            obj = f"g_{c.name}"
        case "zp":
            decls.append(
                f"{t} z_{c.name} [[c6502::zeropage]] = ({t})({init});"
            )
            obj = f"z_{c.name}"
        case "ptr":
            decls.append(f"{t} t_{c.name} = ({t})({init});")
            pre = f"    {t} *p = &t_{c.name};\n"
            obj = "(*p)"
        case "array":
            decls.append(f"{t} a_{c.name}[3] = {{0, ({t})({init}), 0}};")
            obj = f"a_{c.name}[one]"
        case "member":
            decls.append(
                f"struct S_{_tag(t)} s_{c.name} = {{0, ({t})({init})}};"
            )
            obj = f"s_{c.name}.m"
    expr = "(" + _OPS[c.op][0].replace("X", obj) + ")"
    rtype = "int" if c.use == "if" else t
    match c.use:
        case "ret":
            body = f"    return {expr};\n"
        case "after":
            body = f"    {expr};\n    return {obj};\n"
        case "if":
            value, _ = _apply(c.op, c.start, c.size, c.signed)
            body = (f"    if ({expr} == ({t})({_literal(value, c.signed)})) "
                    f"return 1;\n    return 0;\n")
        case "arg":
            body = f"    return id_{_tag(t)}({expr});\n"
    fn = f"{rtype} f_{c.name}(void) {{\n{pre}{body}}}\n"
    return decls, fn


def _programs():
    """Yields `(source, cases)` per program."""
    cases = _cases()
    chunk: list[_Case] = []
    zp_bytes = 0
    result_bytes = 0

    def emit(chunk):
        types = sorted({c.ctype for c in chunk})
        head = ["unsigned char one = 1;"]
        for t in types:
            head.append(f"struct S_{_tag(t)} {{ unsigned char pad; {t} m; }};")
            head.append(f"{t} id_{_tag(t)}({t} v) {{ return v; }}")
        decls, fns, results, calls = [], [], [], []
        for c in chunk:
            d, f = _function(c)
            decls += d
            fns.append(f)
            rtype = "int" if c.use == "if" else c.ctype
            results.append(f"{rtype} r_{c.name};")
            calls.append(f"    r_{c.name} = f_{c.name}();")
        src = "\n".join(head + decls + results) + "\n" + "".join(fns)
        src += "int main(void) {\n" + "\n".join(calls) + "\n    return 0;\n}\n"
        return src, list(chunk)

    for c in cases:
        extra = c.size if c.storage == "zp" else 0
        if chunk and (
            len(chunk) >= _CHUNK
            or zp_bytes + extra > _ZP_BUDGET
            or result_bytes + c.ret_size > _RESULT_BUDGET
        ):
            yield emit(chunk)
            chunk, zp_bytes, result_bytes = [], 0, 0
        chunk.append(c)
        zp_bytes += extra
        result_bytes += c.ret_size
    if chunk:
        yield emit(chunk)


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestOperatorMatrix(unittest.TestCase):

    def _check(self, optimize: bool, shard: int) -> None:
        failures: list[str] = []
        for k, (src, cases) in enumerate(_programs()):
            if k % _SHARDS != shard:
                continue
            try:
                sim = build_sim(src, optimize=optimize)
                sim.run()
            except Exception as e:  # report the program, keep going
                failures.append(f"{cases[0].name}..: {type(e).__name__}: {e}")
                continue
            mem = sim.mpu.memory
            for c in cases:
                addr = sim.symbols[f"r_{c.name}"]
                raw = int.from_bytes(
                    bytes(mem[addr:addr + c.ret_size]), "little",
                )
                want = c.want & ((1 << (8 * c.ret_size)) - 1)
                if raw != want:
                    failures.append(
                        f"{c.name}: {c.describe()}: got {raw:#x}, "
                        f"want {want:#x}"
                    )
        self.assertEqual(failures, [], msg="\n" + "\n".join(failures[:40]))



# The programs split into shards, one test method each, so a parallel
# run (`pytest -n auto`) spreads them over its workers.
_SHARDS = 8
for _k in range(_SHARDS):
    setattr(TestOperatorMatrix, f"test_unoptimized_{_k}",
            lambda self, k=_k: self._check(optimize=False, shard=k))
    setattr(TestOperatorMatrix, f"test_optimized_{_k}",
            lambda self, k=_k: self._check(optimize=True, shard=k))


if __name__ == "__main__":
    unittest.main()
