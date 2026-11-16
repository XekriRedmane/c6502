"""TAC pass: split a 2-byte ordering compare and an equality compare
of the same two values into a compare of each byte.

# What it does

A lookup in a sorted table tests the key against an entry twice, once
for order and once for equality:

```c
if (key < e) return 0;
if (key == e) return found;
```

which reaches `tac_to_asm` as

    JumpIfCmp(GreaterOrEqual, key, e, .if_end@0)
    Ret(0)
  .if_end@0:
    JumpIfCmp(NotEqual, key, e, .if_end@1)
    <found>
  .if_end@1:

and lowers to a 2-byte subtract for the first compare and a 2-byte
equality test for the second, 22 bytes with zero-page operands. Both
results come from the high bytes unless the high bytes are equal, and
then from the low bytes, which is how the 6502 code a person writes
does it:

    LDA key+1; CMP e+1; BCC less; BNE greater
    LDA key;   CMP e;   BCC less; BNE greater
    <equal>

That's 16 bytes, and most entries take only the first two branches.
Zork's `match_dictionary_word`
(examples/zork/interpreter/input.c) compares each of a word's two
2-byte halves with an entry's this way.

The pass writes the byte compares in TAC. Each of the three outcomes,
less, equal, and greater, goes where the two compares send it:

  * Two compares in a row, `JumpIfCmp(op1, a, b, T1); JumpIfCmp(op2, a,
    b, T2)` with `op2` an equality: an outcome goes to T1 if `op1`
    is true of it, else to T2 if `op2` is true of it, else to the
    instruction after the second compare. The two become, for the
    high bytes and then the low ones, a `<` to the less outcome's
    place and a `!=` to the greater one's, except for the low bytes'
    fall-through.
  * An ordering compare whose target starts with the equality
    compare, `JumpIfCmp(>=, a, b, T1); X; T1: JumpIfCmp(op2, a, b,
    T2)`, where X, the less outcome, ends in a `Ret` or `Jump`. This
    is the two `if`s above. The first compare becomes

        JumpIfCmp(<, a_hi, b_hi, .less)
        JumpIfCmp(!=, a_hi, b_hi, <greater's place>)
        JumpIfCmp(>=, a_lo, b_lo, .low_same_or_more)
      .less:
        X
      .low_same_or_more:
        JumpIfCmp(op2, a_lo, b_lo, T2)

    and the second compare goes, when nothing else jumps to it. The
    low bytes keep a branch over X so that the asm `tail_merge` can
    send it to another copy of X, as the less outcome of every such
    pair in a function shares one `return 0`. The second compare of
    the low bytes is the first's, which `redundant_compare` drops in
    the asm. `<=` is the same with the operands swapped.

# Pattern

  * The ordering compare is unsigned (an operand is unsigned or a
    pointer, the rule `tac_to_asm` uses) and both operands are 2
    bytes. Neither is volatile, and they aren't both constants.
  * The equality compare has the same two operands, in either order.
  * The three outcomes go to three different places.
  * The ordering compare doesn't follow a subtract of its operands,
    which `tac_to_asm` would branch on instead
    (`compare_subtract.py`).

A byte of an operand is a byte constant for a constant, the two bytes
for a value built as `(ZeroExtend(h) << 8) | ZeroExtend(l)` of two
1-byte temporaries, and 0 and the byte for `ZeroExtend(l)`. Each of
those temporaries and the instructions that build the value must have
one definition each, in one basic block, so the bytes hold at the
compare what they held when the value was built. Any other operand's
bytes are new `Truncate(a)` and `Truncate(a >> 8)` temporaries, which
`tac_to_asm` lowers to reads of the one byte
(`significant_bytes.low_byte_only`). A high byte of 0 makes the less
outcome `b_hi != 0` and the greater one impossible there (and the
other way around for `b`).

# Where it runs

Once, after SSA destruction. Its new edges would require Phi arguments
in SSA form, and the labels it adds split blocks that a Phi names as
predecessors.
"""

from __future__ import annotations

import c99_ast
import tac_ast
from passes.optimization.framework import PostDestructionPass
from passes.optimization.var_visit import count_uses, uses_in
from passes.type_checking import LocalAttr, Symbol

_LT, _EQ, _GT = "less", "equal", "greater"
_OUTCOMES = (_LT, _EQ, _GT)
_FALL = object()

_HOLDS = {
    tac_ast.LessThan: {_LT},
    tac_ast.LessOrEqual: {_LT, _EQ},
    tac_ast.GreaterThan: {_GT},
    tac_ast.GreaterOrEqual: {_EQ, _GT},
    tac_ast.Equal: {_EQ},
    tac_ast.NotEqual: {_LT, _GT},
}
_SWAPPED = {
    tac_ast.LessThan: tac_ast.GreaterThan,
    tac_ast.GreaterThan: tac_ast.LessThan,
    tac_ast.LessOrEqual: tac_ast.GreaterOrEqual,
    tac_ast.GreaterOrEqual: tac_ast.LessOrEqual,
}
_ORDERING = tuple(_SWAPPED)
_EQUALITY = (tac_ast.Equal, tac_ast.NotEqual)
_TERMINATORS = (tac_ast.Ret, tac_ast.Jump)
_JUMPS = (tac_ast.Jump, tac_ast.JumpIfTrue, tac_ast.JumpIfFalse,
          tac_ast.JumpIfCmp, tac_ast.JumpIfMasked)


class SplitThreeWayCompares(PostDestructionPass):
    name = "split_three_way_compares"

    def run(self, fn, ctx):
        return split_three_way_compares(fn, ctx)


def split_three_way_compares(fn: tac_ast.Function, ctx) -> tac_ast.Function:
    if ctx.symbols is None:
        return fn
    names = _Names(fn, ctx.symbols)
    instrs = list(fn.instructions)
    changed = False
    i = 0
    while i < len(instrs):
        new = _rewrite_at(instrs, i, ctx.symbols, names)
        if new is None:
            i += 1
            continue
        instrs = new
        changed = True
        i += 1
    if not changed:
        return fn
    return tac_ast.Function(
        name=fn.name, is_global=fn.is_global, params=list(fn.params),
        instructions=instrs,
    )


class _Names:
    """Fresh temporary and label names for one function."""

    def __init__(self, fn, symbols):
        self.fn = fn.name
        self.symbols = symbols
        self.labels = {
            ins.name for ins in fn.instructions
            if isinstance(ins, tac_ast.Label)
        }
        self.n = 0

    def byte(self) -> tac_ast.Var:
        while True:
            self.n += 1
            name = f"%{self.fn}.cmp3.{self.n}"
            if self.symbols.get(name) is None:
                break
        self.symbols[name] = Symbol(type=c99_ast.UChar(), attrs=LocalAttr())
        return tac_ast.Var(name=name)

    def word(self) -> tac_ast.Var:
        v = self.byte()
        self.symbols[v.name] = Symbol(type=c99_ast.UInt(), attrs=LocalAttr())
        return v

    def label(self) -> str:
        while True:
            self.n += 1
            name = f".{self.fn}@cmp3@{self.n}"
            if name not in self.labels:
                self.labels.add(name)
                return name


def _rewrite_at(instrs, i, symbols, names):
    """The instruction list with the compares at `instrs[i]` split, or
    None."""
    j1 = instrs[i]
    if not (isinstance(j1, tac_ast.JumpIfCmp)
            and isinstance(j1.op, _ORDERING)
            and _qualifies(j1.src1, j1.src2, symbols)):
        return None
    if i > 0 and _subtracts(instrs[i - 1], j1):
        return None
    nxt = instrs[i + 1] if i + 1 < len(instrs) else None
    if _same_equality(nxt, j1, instrs):
        return _rewrite_adjacent(instrs, i, symbols, names)
    return _rewrite_at_target(instrs, i, symbols, names)


def _rewrite_adjacent(instrs, i, symbols, names):
    j1, j2 = instrs[i], instrs[i + 1]
    a, b = j1.src1, j1.src2
    dest = {}
    for o in _OUTCOMES:
        if o in _HOLDS[type(j1.op)]:
            dest[o] = j1.target
        elif o in _HOLDS[type(j2.op)]:
            dest[o] = j2.target
        else:
            dest[o] = _FALL
    if len({id(d) if d is _FALL else d for d in dest.values()}) != 3:
        return None
    split = _split(a, b, instrs, i, symbols, names)
    if split is None:
        return None
    (ah, al), (bh, bl) = split.pairs
    fall = None

    def place(d):
        nonlocal fall
        if d is _FALL:
            if fall is None:
                fall = names.label()
            return fall
        return d

    out = (split.high_pre + split.high_loads
           + _high_jumps(ah, bh, place(dest[_LT]), place(dest[_GT]))
           + split.low_pre + split.low_loads)
    if dest[_EQ] is _FALL:
        out += [_jcmp(tac_ast.LessThan, al, bl, dest[_LT]),
                _jcmp(tac_ast.NotEqual, al, bl, dest[_GT])]
    elif dest[_LT] is _FALL:
        out += [_jcmp(tac_ast.Equal, al, bl, dest[_EQ]),
                _jcmp(tac_ast.GreaterOrEqual, al, bl, dest[_GT])]
    else:
        out += [_jcmp(tac_ast.LessThan, al, bl, dest[_LT]),
                _jcmp(tac_ast.Equal, al, bl, dest[_EQ])]
    if fall is not None:
        out.append(tac_ast.Label(name=fall))
    return _before(instrs, i, split) + out + instrs[i + 2:]


def _rewrite_at_target(instrs, i, symbols, names):
    j1 = instrs[i]
    a, b, op = j1.src1, j1.src2, j1.op
    if isinstance(op, tac_ast.LessOrEqual):
        a, b = b, a
    elif not isinstance(op, tac_ast.GreaterOrEqual):
        return None
    t = next((k for k in range(i + 1, len(instrs))
              if isinstance(instrs[k], tac_ast.Label)
              and instrs[k].name == j1.target), None)
    # X runs from the compare to T1 and ends in a terminator, so
    # nothing falls into the block that goes in front of T1.
    if t is None or t == i + 1 or not isinstance(instrs[t - 1], _TERMINATORS):
        return None
    k = t
    while k < len(instrs) and isinstance(instrs[k], tac_ast.Label):
        k += 1
    # Copies that only the equality compare reads, such as the `int`
    # to `unsigned` cast of a widened byte, go with it.
    uses = count_uses(instrs)
    while (k < len(instrs) and isinstance(instrs[k], tac_ast.Copy)
           and instrs[k].dst.name.startswith("%")
           and uses.get(instrs[k].dst.name, 0) == 1):
        k += 1
    if k >= len(instrs) or not _same_equality(instrs[k], j1, instrs):
        return None
    j2 = instrs[k]
    split = _split(a, b, instrs, i, symbols, names)
    if split is None:
        return None
    (ah, al), (bh, bl) = split.pairs
    refs = _label_refs(instrs)
    entry_labels = [ins for ins in instrs[t:k]
                    if isinstance(ins, tac_ast.Label)]
    removable = all(
        refs.get(lab.name, 0) == (1 if lab.name == j1.target else 0)
        for lab in entry_labels
    )
    # Where the second compare sends the equal and greater outcomes.
    after = k + 1
    after_label = None
    if (isinstance(j2.op, tac_ast.Equal)):
        if after < len(instrs) and isinstance(instrs[after], tac_ast.Label):
            after_label = instrs[after].name
        else:
            after_label = names.label()
        greater = after_label
        low_op = tac_ast.Equal
    else:
        greater = j2.target
        low_op = tac_ast.NotEqual
    less = names.label()
    same_or_more = names.label()
    high = _high_jumps(ah, bh, less, greater)
    out = (split.high_pre + split.high_loads + high
           + split.low_pre + split.low_loads
           + [_jcmp(tac_ast.GreaterOrEqual, al, bl, same_or_more)])
    if any(getattr(ins, "target", None) == less for ins in high):
        out.append(tac_ast.Label(name=less))
    out += instrs[i + 1:t]
    out += [tac_ast.Label(name=same_or_more),
            _jcmp(low_op, al, bl, j2.target)]
    if not removable:
        out += instrs[t:k + 1]
    rest = instrs[after:]
    if after_label is not None and not (
        rest and isinstance(rest[0], tac_ast.Label)
        and rest[0].name == after_label
    ):
        rest = [tac_ast.Label(name=after_label)] + rest
    return _before(instrs, i, split) + out + rest


def _before(instrs, i, split):
    """The instructions before the compare at `i`, less the loads moved
    down to it."""
    return [ins for k, ins in enumerate(instrs[:i]) if k not in split.moved]


def _high_jumps(ah, bh, less, greater):
    """The jumps on the high bytes: to `less` when a's is below b's,
    to `greater` when it's above."""
    if _is_zero(ah):
        return [_jcmp(tac_ast.NotEqual, bh, ah, less)]
    if _is_zero(bh):
        return [_jcmp(tac_ast.NotEqual, ah, bh, greater)]
    return [_jcmp(tac_ast.LessThan, ah, bh, less),
            _jcmp(tac_ast.NotEqual, ah, bh, greater)]


def _jcmp(op, src1, src2, target):
    return tac_ast.JumpIfCmp(op=op(), src1=src1, src2=src2, target=target)


def _is_zero(v) -> bool:
    return isinstance(v, tac_ast.Constant) and v.const.value == 0


def _label_refs(instrs) -> dict[str, int]:
    refs: dict[str, int] = {}
    for ins in instrs:
        if isinstance(ins, _JUMPS):
            refs[ins.target] = refs.get(ins.target, 0) + 1
    return refs


def _same_equality(ins, j1, instrs) -> bool:
    """True when `ins` is an equality compare of the values `j1`
    compares, each the same variable or a copy of the same one."""
    if not (isinstance(ins, tac_ast.JumpIfCmp)
            and isinstance(ins.op, _EQUALITY)):
        return False
    defs = _defs(instrs)
    keys = {_key(_root(v, instrs, defs)) for v in (ins.src1, ins.src2)}
    return (len(keys) == 2 and keys == {
        _key(_root(v, instrs, defs)) for v in (j1.src1, j1.src2)
    })


def _root(v, instrs, defs):
    """The variable `v` is a copy of, through temporaries that have one
    definition each, which are all the same value."""
    for _ in range(8):
        d = _only_def(defs, v) if (
            isinstance(v, tac_ast.Var) and v.name.startswith("%")
        ) else None
        if d is None or not isinstance(instrs[d], tac_ast.Copy):
            return v
        src = instrs[d].src
        # A variable that isn't a single-definition temporary may
        # change between the two copies.
        if not (isinstance(src, tac_ast.Var) and src.name.startswith("%")
                and _only_def(defs, src) is not None):
            return v
        v = src
    return v


def _key(v):
    if isinstance(v, tac_ast.Var):
        return ("var", v.name)
    return ("const", type(v.const).__name__, v.const.value)


def _subtracts(ins, j1) -> bool:
    return (isinstance(ins, tac_ast.Binary)
            and isinstance(ins.op, tac_ast.Subtract)
            and {_key(ins.src1), _key(ins.src2)}
            == {_key(j1.src1), _key(j1.src2)})


_TWO_BYTE_TYPES = (c99_ast.Int, c99_ast.UInt, c99_ast.Pointer)
_UNSIGNED_TYPES = (c99_ast.UInt, c99_ast.Pointer)
_TWO_BYTE_CONSTS = (tac_ast.ConstInt, tac_ast.ConstUInt)


def _type(v, symbols):
    sym = symbols.get(v.name)
    return None if sym is None else sym.type


def _qualifies(a, b, symbols) -> bool:
    if isinstance(a, tac_ast.Constant) and isinstance(b, tac_ast.Constant):
        return False
    unsigned = False
    for v in (a, b):
        if isinstance(v, tac_ast.Constant):
            if not isinstance(v.const, _TWO_BYTE_CONSTS):
                return False
            unsigned |= isinstance(v.const, tac_ast.ConstUInt)
            continue
        t = _type(v, symbols)
        if not isinstance(t, _TWO_BYTE_TYPES):
            return False
        unsigned |= isinstance(t, _UNSIGNED_TYPES)
    return unsigned


class _Split:
    """The bytes of the two operands. `high_pre` and `low_pre` compute
    any new temporaries for the high and low bytes, and `high_loads`
    and `low_loads` are the loads of bytes moved from `moved` (their
    indices) to just before the compares of those bytes."""

    def __init__(self):
        self.high_pre: list = []
        self.low_pre: list = []
        self.pairs: list = []
        self.high_loads: list = []
        self.low_loads: list = []
        self.moved: set[int] = set()


def _split(a, b, instrs, i, symbols, names):
    """The `_Split` of `a` and `b` at the compare `instrs[i]`, or None."""
    out = _Split()
    uses = count_uses(instrs)
    for v in (a, b):
        found = _bytes(v, instrs, i, symbols, names)
        if found is None:
            return None
        (high_pre, low_pre), hi, lo, sources = found
        out.high_pre += high_pre
        out.low_pre += low_pre
        out.pairs.append((hi, lo))
        for d, loads in zip(sources, (out.high_loads, out.low_loads)):
            if d is not None and _movable_load(instrs, d, i, uses):
                loads.append(instrs[d])
                out.moved.add(d)
    return out


def _byte_const(value: int) -> tac_ast.Constant:
    return tac_ast.Constant(const=tac_ast.ConstUChar(value=value & 0xFF))


def _bytes(v, instrs, i, symbols, names):
    """`((high_pre, low_pre), hi, lo, (hi_def, lo_def))`: the
    instructions that compute new temporaries for each byte, the two
    bytes, and the indices of the instructions that define the bytes
    where `v` is built from them, or None."""
    if isinstance(v, tac_ast.Constant):
        value = v.const.value & 0xFFFF
        return (([], []), _byte_const(value >> 8), _byte_const(value),
                (None, None))
    built = _built_from_bytes(v, instrs, symbols)
    if built is not None:
        hi, lo = built
        defs = _defs(instrs)
        return (([], []), hi, lo,
                (_only_def(defs, hi), _only_def(defs, lo)))
    if isinstance(_type(v, symbols), c99_ast.Pointer):
        return None
    lo = names.byte()
    shifted = names.word()
    hi = names.byte()
    # `v` is read where the compare read it, so the low byte may be
    # read after the jumps on the high bytes.
    return ([
        tac_ast.Binary(
            op=tac_ast.RightShift(), src1=v,
            src2=tac_ast.Constant(const=tac_ast.ConstInt(value=8)),
            dst=shifted,
        ),
        tac_ast.Truncate(src=shifted, dst=hi),
    ], [tac_ast.Truncate(src=v, dst=lo)]), hi, lo, (None, None)


def _built_from_bytes(v, instrs, symbols):
    """`(hi, lo)` when `v` is `(ZeroExtend(h) << 8) | ZeroExtend(l)`, or
    `ZeroExtend(l)` with a high byte of 0, of 1-byte temporaries that
    hold at every read of `v` what they held when `v` was built."""
    defs = _defs(instrs)
    v = _root(v, instrs, defs)
    top = _only_def(defs, v)
    if top is None:
        return None
    block = [top]
    top_ins = instrs[top]
    if isinstance(top_ins, tac_ast.ZeroExtend):
        lo = top_ins.src
        if not _byte_temp(lo, symbols):
            return None
        hi = _byte_const(0)
        sources = [lo]
    elif (isinstance(top_ins, tac_ast.Binary)
          and isinstance(top_ins.op, tac_ast.BitwiseOr)):
        found = None
        for s, z in ((top_ins.src1, top_ins.src2),
                     (top_ins.src2, top_ins.src1)):
            shift = _only_def(defs, s)
            ext = _only_def(defs, z)
            if shift is None or ext is None:
                continue
            si, zi = instrs[shift], instrs[ext]
            if not (isinstance(si, tac_ast.Binary)
                    and isinstance(si.op, tac_ast.LeftShift)
                    and isinstance(si.src2, tac_ast.Constant)
                    and si.src2.const.value == 8
                    and isinstance(zi, tac_ast.ZeroExtend)):
                continue
            hext = _only_def(defs, si.src1)
            if hext is None or not isinstance(instrs[hext], tac_ast.ZeroExtend):
                continue
            found = (instrs[hext].src, zi.src, [shift, ext, hext])
            break
        if found is None:
            return None
        hi, lo, more = found
        if not (_byte_temp(hi, symbols) and _byte_temp(lo, symbols)):
            return None
        block += more
        sources = [hi, lo]
    else:
        return None
    for s in sources:
        d = _only_def(defs, s)
        if d is None:
            return None
        block.append(d)
    # Every definition in one basic block, so the bytes are redefined
    # only when the value is rebuilt.
    first, last = min(block), max(block)
    if last != top:
        return None
    for ins in instrs[first + 1:last + 1]:
        if isinstance(ins, (tac_ast.Label, *_JUMPS, tac_ast.Ret)):
            return None
    return hi, lo


_LOADS = (tac_ast.Load, tac_ast.IndirectIndexedLoad, tac_ast.IndexedLoad)
# Instructions that write nothing but their dst.
_PURE = (
    tac_ast.Binary, tac_ast.Unary, tac_ast.Copy, tac_ast.ZeroExtend,
    tac_ast.SignExtend, tac_ast.Truncate, tac_ast.GetAddress,
) + _LOADS


def _movable_load(instrs, d, i, uses) -> bool:
    """True when the load at `instrs[d]` reads the same byte at the
    compare `instrs[i]`: it isn't volatile, the value it builds is its
    only reader, and nothing between it and the compare writes memory
    or the load's operands."""
    load = instrs[d]
    if not isinstance(load, _LOADS) or load.is_volatile:
        return False
    if uses.get(load.dst.name, 0) != 1:
        return False
    read = {v.name for v in uses_in(load)}
    for ins in instrs[d + 1:i]:
        if not isinstance(ins, _PURE) or getattr(ins, "is_volatile", False):
            return False
        if not ins.dst.name.startswith("%") or ins.dst.name in read:
            return False
    return True


def _defs(instrs) -> dict[str, list[int]]:
    from passes.recursion_saves import defs_in
    out: dict[str, list[int]] = {}
    for k, ins in enumerate(instrs):
        for d in defs_in(ins):
            out.setdefault(d.name, []).append(k)
    return out


def _only_def(defs, v):
    if not isinstance(v, tac_ast.Var):
        return None
    found = defs.get(v.name, [])
    return found[0] if len(found) == 1 else None


def _byte_temp(v, symbols) -> bool:
    if not (isinstance(v, tac_ast.Var) and v.name.startswith("%")):
        return False
    t = _type(v, symbols)
    return isinstance(t, (c99_ast.UChar, c99_ast.Char))
