"""Looks up the address of a row of a 2-D array in a table of row
addresses.

`A[i][j]` on an array of N rows of R bytes, more than 255 bytes in
all, computes `&A + i * R` with 16-bit arithmetic. When R isn't a
power of two, that's two 16-bit shifts and an add or subtract (or a
call of `mul16`), and then a 16-bit add of the array's address,
about 40 bytes for each access. 6502 programmers look the row up in a
table instead, with its addresses' low bytes in one half and their
high bytes in the other:

    LDX i; LDA rows,X; STA p; LDA rows+N+1,X; STA p+1

and then read `(p),Y`. This pass does the same. For each `&A + i *
R`, with `i` an unsigned byte, plus or minus a constant `k`, it makes
the pointer `IndexedLoad(rows, i, p, offset=k, plane=N+1)`, which
reads byte j of the row's address at `rows + k + i + j * (N + 1)`.

The table holds N+1 addresses, `A`, `A+R`, ..., `A+N*R`, so a
pointer one past the last row works too. C99 §6.5.6.8 makes any
other index undefined, so the table needs no more. For an array with
a fixed address (`[[c6502::address(addr)]]`) the bytes are numbers.
For any other array they're link-time address bytes, `DC.B
<(A+R*r)` and `DC.B >(A+R*r)`.

The pass runs once, over the whole program, on the TAC from
`c99_to_tac`, before the TAC optimizer, whose strength reduction
would otherwise turn the multiply into shifts. It doesn't run in the
unoptimized pipeline.

The shape it looks for, with every temporary defined once and read
once in the function:

    GetAddress(A, a)
    ZeroExtend(i, z)
    Binary(+ or -, z, k, x)        ; any number of these, or none
    Binary(*, x, R, m)
    Binary(+, a, m, p)

`A` is a static-storage array that isn't in zero page, `R` is the
size of its elements and not a power of two, `i` is an unsigned byte
that isn't volatile, and the constants add up to a `k` from -255 to
N. `c99_to_tac` writes this for `A[i]` when A spans more than 255
bytes, whatever A's element type is, so `A[i][j]`, `&A[i]`,
`&A[i][j]`, `A[i]` as a row pointer, and an array of structs all
qualify. The pointer is computed where the `ZeroExtend` read `i`.

A table costs 2(N+1) bytes in each unit that makes it, and each
access saves what the multiply and the add would have cost
(`_saving`). The pass builds a table for an array only when the
unit's accesses to it save more than the table costs. Arrays whose
tables have the same number of rows share bytes: two arrays with the
same row addresses share one table, and two whose row addresses have
the same low bytes (arrays 256 bytes apart) share the half with the
low bytes.
"""

from __future__ import annotations

import c99_ast
import tac_ast
from c99_to_tac import _sizeof
from passes.optimization.split_tables import plane_init
from passes.optimization.strength_reduction import _two_power_terms
from passes.optimization.var_visit import count_uses, defs_in
from passes.shift_cost import inline_shift_ok
from passes.type_checking import Initial, StaticAttr, Symbol

# The bytes a lookup saves (`_saving`), and the most arrays of a
# group whose every set `_choose` tries.
_TWO_TERM_SAVING = 40
_HELPER_SAVING = 10
_EXHAUSTIVE = 10


def apply_row_address_tables(
    prog: tac_ast.Program, symbols, types=None,
) -> tac_ast.Program:
    """Rewrites each row address of a qualifying array in `prog` as a
    lookup in a table of row addresses, when the unit's accesses save
    more than the tables cost. Adds the tables to `prog` and to
    `symbols`."""
    if symbols is None:
        return prog
    accesses: dict[str, list] = {}
    for tl in prog.top_level:
        if isinstance(tl, tac_ast.Function):
            for acc in _find_accesses(tl, symbols, types):
                accesses.setdefault(acc.array, []).append(acc)
    if not accesses:
        return prog
    layout = _choose(accesses, symbols, types)
    if not layout:
        return prog
    top: list[tac_ast.Type_top_level] = []
    for tl in prog.top_level:
        if isinstance(tl, tac_ast.Function):
            mine = [a for name in layout.arrays
                    for a in accesses[name] if a.function == tl.name]
            top.append(_rewrite(tl, mine, layout) if mine else tl)
        else:
            top.append(tl)
    for table in layout.tables:
        init, values = plane_init(table.planes)
        top.append(tac_ast.StaticVariable(
            name=table.name, is_global=False, data_type=tac_ast.Pointer(),
            init=init,
        ))
        symbols[table.name] = Symbol(
            type=c99_ast.Array(
                element_type=c99_ast.Const(referenced_type=c99_ast.UChar()),
                size=len(values),
            ),
            attrs=StaticAttr(
                initial_value=Initial(value=values), is_global=False,
            ),
        )
    return tac_ast.Program(top_level=top)


class _Access:
    """One row address to rewrite in `function`: the instruction at
    `at` (the `ZeroExtend`) becomes the lookup of row `index + k` of
    `array` into `dst`, and the instructions at `drop` go."""

    def __init__(self, function, array, at, index, k, dst, drop):
        self.function = function
        self.array = array
        self.at = at
        self.index = index
        self.k = k
        self.dst = dst
        self.drop = drop


def _array_shape(name: str, symbols, types) -> tuple[int, int] | None:
    """(rows, row size) of the static array `name` when a table of its
    row addresses can be made, else None."""
    sym = symbols.get(name)
    if sym is None or not isinstance(sym.attrs, StaticAttr):
        return None
    if sym.attrs.zeropage:
        return None
    t = sym.type
    while isinstance(t, (c99_ast.Const, c99_ast.Volatile)):
        t = t.referenced_type
    if not isinstance(t, c99_ast.Array) or not t.size or t.size < 1:
        return None
    try:
        row = _sizeof(t.element_type, types)
    except Exception:
        return None
    if row < 3 or row & (row - 1) == 0:
        return None
    return t.size, row


def _is_unsigned_byte(v: tac_ast.Type_val, symbols) -> bool:
    if not isinstance(v, tac_ast.Var):
        return False
    sym = symbols.get(v.name)
    if sym is None:
        return False
    t = sym.type
    while isinstance(t, c99_ast.Const):
        t = t.referenced_type
    return isinstance(t, (c99_ast.Char, c99_ast.UChar, c99_ast.Bool))


def _const(v: tac_ast.Type_val) -> int | None:
    if isinstance(v, tac_ast.Constant) and hasattr(v.const, "value"):
        return v.const.value
    return None


def _find_accesses(fn: tac_ast.Function, symbols, types) -> list[_Access]:
    instrs = fn.instructions
    defs: dict[str, list[int]] = {}
    for k, ins in enumerate(instrs):
        for d in defs_in(ins):
            defs.setdefault(d.name, []).append(k)
    uses = count_uses(instrs)

    def only_def(v) -> int | None:
        """The instruction defining temporary `v`, when it has one
        definition and one reader."""
        if not isinstance(v, tac_ast.Var) or uses[v.name] != 1:
            return None
        ks = defs.get(v.name, [])
        return ks[0] if len(ks) == 1 else None

    out: list[_Access] = []
    for at, ins in enumerate(instrs):
        if not (isinstance(ins, tac_ast.Binary)
                and isinstance(ins.op, tac_ast.Add)
                and isinstance(ins.dst, tac_ast.Var)
                and len(defs.get(ins.dst.name, [])) == 1):
            continue
        acc = _match(fn, instrs, at, ins, only_def, symbols, types)
        if acc is not None:
            out.append(acc)
    return out


def _match(fn, instrs, at, add, only_def, symbols, types) -> _Access | None:
    for a_val, m_val in ((add.src1, add.src2), (add.src2, add.src1)):
        a = only_def(a_val)
        if a is None:
            continue
        get = instrs[a]
        if not (isinstance(get, tac_ast.GetAddress)
                and isinstance(get.operand, tac_ast.Var)):
            continue
        shape = _array_shape(get.operand.name, symbols, types)
        if shape is None:
            return None
        rows, row = shape
        m = only_def(m_val)
        if m is None:
            return None
        mul = instrs[m]
        if not (isinstance(mul, tac_ast.Binary)
                and isinstance(mul.op, tac_ast.Multiply)):
            return None
        if _const(mul.src2) == row:
            x_val = mul.src1
        elif _const(mul.src1) == row:
            x_val = mul.src2
        else:
            return None
        drop = [a, m, at]
        k = 0
        while True:
            x = only_def(x_val)
            if x is None:
                return None
            step = instrs[x]
            if isinstance(step, tac_ast.ZeroExtend):
                break
            if not isinstance(step, tac_ast.Binary):
                return None
            if isinstance(step.op, tac_ast.Add) and _const(step.src2) is not None:
                k, x_val = k + _const(step.src2), step.src1
            elif isinstance(step.op, tac_ast.Add) and _const(step.src1) is not None:
                k, x_val = k + _const(step.src1), step.src2
            elif (isinstance(step.op, tac_ast.Subtract)
                  and _const(step.src2) is not None):
                k, x_val = k - _const(step.src2), step.src1
            else:
                return None
            drop.append(x)
        i = step.src
        if not _is_unsigned_byte(i, symbols) or _volatile(i, symbols):
            return None
        if not -255 <= k <= rows:
            return None
        return _Access(
            fn.name, get.operand.name, x, i, k, add.dst, drop,
        )
    return None


def _volatile(v: tac_ast.Var, symbols) -> bool:
    t = symbols.get(v.name).type
    while isinstance(t, c99_ast.Const):
        t = t.referenced_type
    return isinstance(t, c99_ast.Volatile)


# ---- Cost and layout --------------------------------------------------------


def _saving(row: int) -> int:
    """The bytes one lookup saves over `i * row` plus the array's
    address. These were measured on `M[r][c]`, `M[r][c] = v`, and
    `M[r]` for a 16-row `M` with row sizes from 20 to 104. When `row`
    is a sum or difference of two powers of two whose shifts are
    written inline, strength reduction makes the multiply two 16-bit
    shifts and an add or subtract, and the lookup saved 37 to 46
    bytes. Otherwise the multiply calls `mul16`, and the code that
    sets up the call is one that `outline_runs` shares between the
    accesses, so a lookup saved from 1 byte (with the code shared by
    three accesses) to about 20 (with one access)."""
    terms = _two_power_terms(row)
    if terms is not None:
        a, b, _ = terms
        shifts = [a] if b == 0 else [b, a - b]
        if all(inline_shift_ok(2, s) for s in shifts):
            return _TWO_TERM_SAVING
    return _HELPER_SAVING


class _Table:
    def __init__(self, name: str, planes: list[list]):
        self.name = name
        self.planes = planes


class _Layout:
    """The tables to make, and for each array the table its lookups
    read, the offset of its low bytes in it, and the distance from
    its low bytes to its high bytes."""

    def __init__(self):
        self.tables: list[_Table] = []
        self.arrays: dict[str, tuple[str, int, int]] = {}


def _row_bytes(name: str, rows: int, row: int, symbols) -> tuple[list, list]:
    """The low and high bytes of the N+1 row addresses of `name`."""
    address = symbols.get(name).attrs.address
    lo: list = []
    hi: list = []
    for r in range(rows + 1):
        if address is not None:
            v = (address + r * row) & 0xFFFF
            lo.append(v & 0xFF)
            hi.append(v >> 8)
        else:
            lo.append(tac_ast.AddressByteInit(name=name, offset=r * row, high=False))
            hi.append(tac_ast.AddressByteInit(name=name, offset=r * row, high=True))
    return lo, hi


def _choose(accesses: dict[str, list], symbols, types) -> _Layout:
    """Picks the arrays whose lookups save more than the bytes their
    tables add, and lays out the tables. Arrays with the same number
    of rows go in one table, which holds each distinct half once:
    every low half first and then every high half, so that an array's
    high bytes follow its low ones. Since arrays can share halves,
    the arrays of a group are chosen together, trying every set of
    them when there are at most `_EXHAUSTIVE` and adding them one at a
    time, the most accessed first, otherwise."""
    layout = _Layout()
    groups: dict[int, list[str]] = {}
    halves: dict[str, tuple[list, list]] = {}
    gain: dict[str, int] = {}
    for name in sorted(accesses):
        rows, row = _array_shape(name, symbols, types)
        halves[name] = _row_bytes(name, rows, row, symbols)
        gain[name] = len(accesses[name]) * _saving(row)
        groups.setdefault(rows + 1, []).append(name)
    for length, names in sorted(groups.items()):
        chosen = _best_set(names, halves, gain, length)
        if not chosen:
            continue
        los: list[list] = []
        his: list[list] = []
        for n in chosen:
            lo, hi = halves[n]
            if lo not in los and lo not in his:
                los.append(lo)
            if hi not in his and hi not in los:
                his.append(hi)
        planes = los + his
        table = _Table(f"{chosen[0]}@rows", planes)
        layout.tables.append(table)
        for n in chosen:
            lo, hi = halves[n]
            first = planes.index(lo)
            second = planes.index(hi)
            layout.arrays[n] = (
                table.name, first * length, (second - first) * length,
            )
    return layout


def _net(names: list[str], halves: dict, gain: dict, length: int) -> int:
    """The bytes the lookups of `names` save, less their table."""
    distinct: list[list] = []
    for n in names:
        for h in halves[n]:
            if h not in distinct:
                distinct.append(h)
    return sum(gain[n] for n in names) - length * len(distinct)


def _best_set(
    names: list[str], halves: dict, gain: dict, length: int,
) -> list[str]:
    """The arrays of one group to make lookups for, in a fixed order,
    or an empty list when no set saves bytes."""
    if len(names) <= _EXHAUSTIVE:
        best: list[str] = []
        best_net = 0
        for mask in range(1, 1 << len(names)):
            pick = [n for b, n in enumerate(names) if mask >> b & 1]
            net = _net(pick, halves, gain, length)
            if net > best_net:
                best, best_net = pick, net
        return best
    chosen: list[str] = []
    for n in sorted(names, key=lambda n: (-gain[n], n)):
        if (_net(chosen + [n], halves, gain, length)
                > _net(chosen, halves, gain, length)):
            chosen.append(n)
    return sorted(chosen)


def _rewrite(
    fn: tac_ast.Function, accesses: list[_Access], layout: _Layout,
) -> tac_ast.Function:
    instrs: list = list(fn.instructions)
    drop: set[int] = set()
    for acc in accesses:
        table, start, plane = layout.arrays[acc.array]
        instrs[acc.at] = tac_ast.IndexedLoad(
            name=table, index=acc.index, dst=acc.dst, is_volatile=False,
            offset=start + acc.k, plane=plane,
        )
        drop.update(acc.drop)
    return tac_ast.Function(
        name=fn.name, is_global=fn.is_global, params=list(fn.params),
        instructions=[ins for k, ins in enumerate(instrs) if k not in drop],
    )
