"""Splits a table of 2-byte elements read by a byte index into byte
planes.

A `static const` array of 2-byte elements (or wider), such as a
`uint16_t` or a pointer table, read as `T[i]` with `i` an unsigned
byte, compiles to

    LDA i; ASL; TAX; LDA T,X; ...; LDA T+1,X

for a table of at most 127 elements, and to a 16-bit shift, a 16-bit
add, and a read through a pointer for a longer one. 6502 programmers
keep such a table as two tables, the low bytes of every element and
then the high bytes, so that the element number is the index:

    LDX i; LDA T,X; ...; LDA T+N,X

This pass does the same when every use of the table is such a read.
The table's bytes are rewritten in that order, and each read becomes
`IndexedLoad(T, i, dst, offset=k, plane=N)`: byte j of the element
at `i + k` is at `T + k + i + j * N`. The table keeps its name, and
its symbol becomes an array of `const` bytes, which is what it is
now.

A table qualifies when both of these are true.

  * It's a `StaticVariable` with internal linkage and a one-
    dimensional array type whose elements are a `const` scalar of at
    least 2 bytes, with at most 256 elements, outside zero page, and
    with an initializer this pass can lay out byte by byte (integers,
    floating constants, and addresses).
  * Every reference to it, in every function and every static's
    initializer, is a read the pass rewrites.

A read is one of these, each with a non-volatile destination as wide
as an element.

  * `IndexedLoad(T, s, dst, offset=o)`, with `s` a constant multiple
    of the element size, or `s` defined once in the function, in the
    same block, by `i << log2(w)` or `i * w` for a one-byte `i`, and
    `o` a multiple of the element size `w`. This is the shape
    `c99_to_tac` gives a table of at most 255 bytes.
  * `Load(p, dst)` with `p = a + s` (either order), `a =
    GetAddress(T)`, `s = z << log2(w)` or `z * w`, and `z =
    ZeroExtend(i)` for a one-byte unsigned `i`, each defined once in
    the function, in the same block as the load. This is the shape of
    a longer table.

Since the function is no longer in SSA form here, `i` must hold the
same value at the load as where the chain read it. Nothing between
may write `i`, and when `i` isn't an automatic variable, nothing
between may call a function or store through a pointer. A volatile
`i` is left alone. Each instruction of a chain is dropped once
nothing reads its result.

The pass runs once over the whole program, after the TAC optimizer
and the dispatch on small pointer tables, which handles some pointer
tables better. It doesn't run in the unoptimized pipeline.
"""

from __future__ import annotations

import c99_ast
import tac_ast
from passes.optimization.var_visit import count_uses, defs_in, uses_in
from passes.type_checking import Initial, StaticAttr, Symbol


def apply_split_tables(prog: tac_ast.Program, symbols) -> tac_ast.Program:
    """Splits each qualifying table of `prog` into byte planes and
    rewrites its reads. `symbols` gives each name's c99 type, and the
    split tables' entries are replaced."""
    if symbols is None:
        return prog
    tables = _candidates(prog, symbols)
    if not tables:
        return prog
    plans: dict[str, list | None] = {name: [] for name in tables}
    for tl in prog.top_level:
        if isinstance(tl, tac_ast.Function):
            _find_reads(tl, tables, symbols, plans)
        elif isinstance(tl, tac_ast.StaticVariable):
            # An address in another static's initializer.
            for item in tl.init:
                if isinstance(item, (
                    tac_ast.AddressInit, tac_ast.AddressByteInit,
                )) and item.name in plans:
                    plans[item.name] = None
    chosen = {name for name, reads in plans.items()
              if reads is not None and reads}
    if not chosen:
        return prog
    top: list[tac_ast.Type_top_level] = []
    for tl in prog.top_level:
        if isinstance(tl, tac_ast.Function):
            reads = [r for name in chosen for r in plans[name]
                     if r.function == tl.name]
            top.append(_rewrite_function(tl, reads, tables) if reads else tl)
        elif isinstance(tl, tac_ast.StaticVariable) and tl.name in chosen:
            width, count, cells = tables[tl.name]
            planes = [[cells[e * width + k] for e in range(count)]
                      for k in range(width)]
            init, values = plane_init(planes)
            top.append(tac_ast.StaticVariable(
                name=tl.name, is_global=False, data_type=tl.data_type,
                init=init,
            ))
            sym = symbols.get(tl.name)
            symbols[tl.name] = Symbol(
                type=c99_ast.Array(
                    element_type=c99_ast.Const(referenced_type=c99_ast.UChar()),
                    size=len(values),
                ),
                attrs=StaticAttr(
                    initial_value=Initial(value=values),
                    is_global=False, section=sym.attrs.section,
                ),
            )
        else:
            top.append(tl)
    return tac_ast.Program(top_level=top)


# ---- Tables and their bytes -------------------------------------------------


def plane_init(
    planes: list[list],
) -> tuple[list[tac_ast.Type_static_init], tuple]:
    """The static initializer that lays `planes` down one after
    another, and the tuple of the same bytes for the table's
    `Initial`. Each cell is an integer byte or an `AddressByteInit`.
    A run of integer bytes becomes one `StringInit`, which `asm_emit`
    writes as one `DC.B` line of hex bytes."""
    cells = [c for plane in planes for c in plane]
    init: list[tac_ast.Type_static_init] = []
    run: list[int] = []
    for c in cells + [None]:
        if isinstance(c, int):
            run.append(c)
            continue
        if run:
            init.append(tac_ast.StringInit(
                str="".join(map(chr, run)), bytes=len(run),
            ))
            run = []
        if c is not None:
            init.append(c)
    return init, tuple(cells)


def _init_cells(
    init: list[tac_ast.Type_static_init],
) -> list | None:
    """The bytes `init` lays down, in order, each an integer or an
    `AddressByteInit` for one byte of a link-time address. None when
    `init` holds something else (a string can't be in a table of
    2-byte elements, so it isn't handled)."""
    out: list = []

    def num(v: int, n: int) -> None:
        v &= (1 << (8 * n)) - 1
        out.extend((v >> (8 * k)) & 0xFF for k in range(n))

    for item in init:
        match item:
            case tac_ast.CharInit(value=v) | tac_ast.UCharInit(value=v):
                num(v, 1)
            case tac_ast.IntInit(value=v) | tac_ast.UIntInit(value=v):
                num(v, 2)
            case tac_ast.LongInit(value=v) | tac_ast.ULongInit(value=v):
                num(v, 4)
            case tac_ast.LongLongInit(value=v) | tac_ast.ULongLongInit(value=v):
                num(v, 8)
            case tac_ast.FloatInit(bits=b):
                num(b, 4)
            case tac_ast.DoubleInit(bits=b):
                num(b, 8)
            case tac_ast.AddressInit(name=n, offset=o):
                out.append(tac_ast.AddressByteInit(name=n, offset=o, high=False))
                out.append(tac_ast.AddressByteInit(name=n, offset=o, high=True))
            case tac_ast.ZeroInit(bytes=n):
                out.extend([0] * n)
            case _:
                return None
    return out


def _width(t) -> int | None:
    """Byte width of the scalar type `t`, or None."""
    if isinstance(t, (c99_ast.Char, c99_ast.SChar, c99_ast.UChar,
                      c99_ast.Bool)):
        return 1
    if isinstance(t, (c99_ast.Int, c99_ast.UInt, c99_ast.Short,
                      c99_ast.UShort, c99_ast.Pointer)):
        return 2
    if isinstance(t, (c99_ast.Long, c99_ast.ULong, c99_ast.Float)):
        return 4
    if isinstance(t, (c99_ast.LongLong, c99_ast.ULongLong, c99_ast.Double)):
        return 8
    if isinstance(t, c99_ast.BitInt):
        return (t.width + 7) // 8
    return None


def _candidates(prog: tac_ast.Program, symbols) -> dict[str, tuple]:
    """Maps each table that qualifies by its declaration to (element
    width, element count, bytes)."""
    out: dict[str, tuple] = {}
    for tl in prog.top_level:
        if not isinstance(tl, tac_ast.StaticVariable) or tl.is_global:
            continue
        sym = symbols.get(tl.name)
        if sym is None or not isinstance(sym.attrs, StaticAttr):
            continue
        if sym.attrs.zeropage or sym.attrs.address is not None:
            continue
        t = sym.type
        if not isinstance(t, c99_ast.Array) or not 2 <= t.size <= 256:
            continue
        elem = t.element_type
        if not isinstance(elem, c99_ast.Const):
            continue
        elem = elem.referenced_type
        if isinstance(elem, c99_ast.Volatile):
            continue
        width = _width(elem)
        if width is None or width < 2:
            continue
        cells = _init_cells(tl.init)
        if cells is None or len(cells) != width * t.size:
            continue
        out[tl.name] = (width, t.size, cells)
    return out


# ---- Reads ------------------------------------------------------------------


class _Read:
    """One read to rewrite: the load at `at` in `function` becomes
    `IndexedLoad(table, index, dst, offset=element, plane=count)`, and
    `chain` lists the instructions that computed its address, which go
    when nothing else reads their results."""

    def __init__(self, function, table, at, index, dst, element, chain):
        self.function = function
        self.table = table
        self.at = at
        self.index = index
        self.dst = dst
        self.element = element
        self.chain = chain


def _block_starts(instrs: list) -> list[int]:
    """The number of the basic block each instruction is in."""
    out: list[int] = []
    block = 0
    for k, ins in enumerate(instrs):
        if isinstance(ins, tac_ast.Label) and k:
            block += 1
        out.append(block)
        if isinstance(ins, (
            tac_ast.Jump, tac_ast.JumpIfTrue, tac_ast.JumpIfFalse,
            tac_ast.JumpIfCmp, tac_ast.JumpIfMasked, tac_ast.Ret,
        )):
            block += 1
    return out


def _is_byte(v: tac_ast.Type_val, symbols, unsigned: bool) -> bool:
    """True iff `v` is a one-byte variable (unsigned when `unsigned`)
    that isn't volatile."""
    if not isinstance(v, tac_ast.Var):
        return False
    sym = symbols.get(v.name)
    if sym is None:
        return False
    t = sym.type
    if isinstance(t, c99_ast.Volatile) or (
        isinstance(t, c99_ast.Const)
        and isinstance(t.referenced_type, c99_ast.Volatile)
    ):
        return False
    while isinstance(t, c99_ast.Const):
        t = t.referenced_type
    kinds = (c99_ast.Char, c99_ast.UChar, c99_ast.Bool)
    if not unsigned:
        kinds += (c99_ast.SChar,)
    return isinstance(t, kinds)


def _const(v: tac_ast.Type_val) -> int | None:
    if isinstance(v, tac_ast.Constant) and hasattr(v.const, "value"):
        return v.const.value
    return None


def _scaled_by(ins, width: int) -> tac_ast.Type_val | None:
    """The operand `x` when `ins` is `x << log2(width)` or `x *
    width`, else None."""
    if not isinstance(ins, tac_ast.Binary):
        return None
    shift = width.bit_length() - 1
    if isinstance(ins.op, tac_ast.LeftShift) and _const(ins.src2) == shift:
        return ins.src1
    if isinstance(ins.op, tac_ast.Multiply):
        if _const(ins.src2) == width:
            return ins.src1
        if _const(ins.src1) == width:
            return ins.src2
    return None


def _unchanged(
    instrs: list, start: int, end: int, var: tac_ast.Var, symbols,
) -> bool:
    """True iff nothing in `instrs[start+1:end]` can change `var`.
    An automatic variable whose address the function never takes
    changes only where an instruction defines it."""
    sym = symbols.get(var.name)
    automatic = (
        sym is not None and not isinstance(sym.attrs, StaticAttr)
        and not any(
            isinstance(ins, tac_ast.GetAddress)
            and isinstance(ins.operand, tac_ast.Var)
            and ins.operand.name == var.name
            for ins in instrs
        )
    )
    for ins in instrs[start + 1:end]:
        if any(d.name == var.name for d in defs_in(ins)):
            return False
        if automatic:
            continue
        if isinstance(ins, (
            tac_ast.FunctionCall, tac_ast.IndirectCall, tac_ast.Store,
            tac_ast.IndexedStore, tac_ast.IndexedSymbolStore,
            tac_ast.IndirectIndexedStore,
        )):
            return False
    return True


def _find_reads(
    fn: tac_ast.Function, tables: dict, symbols, plans: dict,
) -> None:
    """Adds `fn`'s reads of each table to `plans`, or sets a table's
    entry to None when `fn` uses it some other way."""
    instrs = fn.instructions
    defs: dict[str, list[int]] = {}
    for k, ins in enumerate(instrs):
        for d in defs_in(ins):
            defs.setdefault(d.name, []).append(k)
    blocks = _block_starts(instrs)
    uses = count_uses(instrs)

    def single_def(v) -> int | None:
        if not isinstance(v, tac_ast.Var):
            return None
        ks = defs.get(v.name, [])
        return ks[0] if len(ks) == 1 else None

    def disqualify(name: str) -> None:
        plans[name] = None

    # The GetAddress results each chain accounts for.
    covered: dict[str, int] = {}
    for k, ins in enumerate(instrs):
        if isinstance(ins, tac_ast.IndexedLoad) and ins.name in tables:
            name = ins.name
            width, count, _ = tables[name]
            read = _indexed_read(
                fn, instrs, k, ins, width, count, symbols, single_def,
                blocks,
            )
            if read is None:
                disqualify(name)
            elif plans[name] is not None:
                plans[name].append(read)
            continue
        if isinstance(ins, tac_ast.Load):
            read = _load_read(
                fn, instrs, k, ins, tables, symbols, single_def, blocks,
                uses,
            )
            if read is not None:
                if plans[read.table] is not None:
                    plans[read.table].append(read)
                a = read.chain[0]
                covered[a] = covered.get(a, 0) + 1
    # Every other reference to a table disqualifies it.
    for k, ins in enumerate(instrs):
        if isinstance(ins, tac_ast.GetAddress) and isinstance(
            ins.operand, tac_ast.Var,
        ) and ins.operand.name in tables:
            d = ins.dst
            if not (isinstance(d, tac_ast.Var)
                    and single_def(d) == k
                    and covered.get(d.name, 0) == uses[d.name]
                    and uses[d.name] > 0):
                disqualify(ins.operand.name)
            continue
        if isinstance(ins, tac_ast.IndexedLoad):
            continue
        for v in uses_in(ins):
            if v.name in tables:
                disqualify(v.name)
        for v in defs_in(ins):
            if v.name in tables:
                disqualify(v.name)
        if isinstance(ins, tac_ast.IndexedSymbolStore) and ins.name in tables:
            disqualify(ins.name)


def _indexed_read(
    fn, instrs, k, ins, width, count, symbols, single_def, blocks,
) -> _Read | None:
    """The read for `IndexedLoad` `ins` at `k`, or None when it isn't
    one the pass can rewrite."""
    if ins.is_volatile or ins.plane or ins.offset % width:
        return None
    if _dst_width(ins.dst, symbols) != width:
        return None
    element = ins.offset // width
    c = _const(ins.index)
    if c is not None:
        if c % width:
            return None
        return _Read(fn.name, ins.name, k, tac_ast.Constant(
            const=tac_ast.ConstUChar(value=0),
        ), ins.dst, element + c // width, [])
    s = single_def(ins.index)
    if s is None or s > k or blocks[s] != blocks[k]:
        return None
    i = _scaled_by(instrs[s], width)
    if i is None or not _is_byte(i, symbols, unsigned=False):
        return None
    if not _unchanged(instrs, s, k, i, symbols):
        return None
    return _Read(fn.name, ins.name, k, i, ins.dst, element, [ins.index.name])


def _load_read(
    fn, instrs, k, ins, tables, symbols, single_def, blocks, uses,
) -> _Read | None:
    """The read for `Load` `ins` at `k` through `GetAddress(T) +
    (ZeroExtend(i) << log2(w))`, or None. The address must have no
    other reader, since it would still need the table as it was."""
    if ins.is_volatile or not isinstance(ins.src_ptr, tac_ast.Var):
        return None
    if uses[ins.src_ptr.name] != 1:
        return None
    p = single_def(ins.src_ptr)
    if p is None or p > k or blocks[p] != blocks[k]:
        return None
    add = instrs[p]
    if not (isinstance(add, tac_ast.Binary) and isinstance(add.op, tac_ast.Add)):
        return None
    for a_val, s_val in ((add.src1, add.src2), (add.src2, add.src1)):
        a = single_def(a_val)
        if a is None or blocks[a] != blocks[k]:
            continue
        get = instrs[a]
        if not (isinstance(get, tac_ast.GetAddress)
                and isinstance(get.operand, tac_ast.Var)
                and get.operand.name in tables):
            continue
        name = get.operand.name
        width, count, _ = tables[name]
        if _dst_width(ins.dst, symbols) != width:
            return None
        s = single_def(s_val)
        if s is None or blocks[s] != blocks[k]:
            return None
        z_val = _scaled_by(instrs[s], width)
        z = single_def(z_val)
        if z is None or blocks[z] != blocks[k]:
            return None
        ext = instrs[z]
        if not isinstance(ext, tac_ast.ZeroExtend):
            return None
        i = ext.src
        if not _is_byte(i, symbols, unsigned=True):
            return None
        if not _unchanged(instrs, z, k, i, symbols):
            return None
        return _Read(fn.name, name, k, i, ins.dst, 0, [
            a_val.name, ins.src_ptr.name, s_val.name, z_val.name,
        ])
    return None


def _dst_width(v: tac_ast.Type_val, symbols) -> int | None:
    if not isinstance(v, tac_ast.Var):
        return None
    sym = symbols.get(v.name)
    if sym is None:
        return None
    t = sym.type
    while isinstance(t, c99_ast.Const):
        t = t.referenced_type
    return _width(t)


def _rewrite_function(
    fn: tac_ast.Function, reads: list[_Read], tables: dict,
) -> tac_ast.Function:
    instrs = list(fn.instructions)
    chain_names: set[str] = set()
    for r in reads:
        _, count, _ = tables[r.table]
        index = r.index
        offset = r.element
        if isinstance(index, tac_ast.Constant):
            index = tac_ast.Constant(const=tac_ast.ConstUChar(value=offset))
            offset = 0
        instrs[r.at] = tac_ast.IndexedLoad(
            name=r.table, index=index, dst=r.dst, is_volatile=False,
            offset=offset, plane=count,
        )
        chain_names.update(r.chain)
    # Drop each chain instruction nothing reads any more, repeating
    # since dropping one can leave the one before it unread.
    while True:
        uses = count_uses(instrs)
        keep = [
            ins for ins in instrs
            if not any(d.name in chain_names and uses[d.name] == 0
                       for d in defs_in(ins))
        ]
        if len(keep) == len(instrs):
            break
        instrs = keep
    return tac_ast.Function(
        name=fn.name, is_global=fn.is_global, params=list(fn.params),
        instructions=instrs,
    )
