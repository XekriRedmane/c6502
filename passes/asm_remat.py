"""Narrow rematerialization peephole.

A common shape after asm-SSA + byte-granular regalloc is "stage a
value through a body-local ZP cell because A gets clobbered between
producer and consumer":

```
LDA   <recomputable_src>
STA   __local_<fn>__<stage>          ; def of the stage cell
... (A clobbered by other marshaling: LDA #c; STA other_arg) ...
LDA   __local_<fn>__<stage>           ; use of the stage cell
STA   <consumer_dst>
```

When `<recomputable_src>` is cheap to re-compute at the use site (no
dependencies, or only on registers / memory that hasn't been touched
in the intervening run), the staging round-trip is pure overhead:
the same `LDA <recomputable_src>` can replace the `LDA <local>`, and
the original `STA <local>` is dead (no more readers — `byte_dce` /
`apply_redundant_store_elimination` / `apply_asm_dead_store` drop it
on the next iteration of the peephole fixedpoint, which then drops
the now-isolated first `LDA <recomputable_src>` too).

Net cost vs. the staging shape (single-instruction recomputes):

- `Imm(v)` / `ImmLabelLow` / `ImmLabelHigh`: 2-byte LDA #imm
  replaces a 2-byte LDA from-zp. Even.
- `Data(name, off)`: 2-or-3-byte abs/zp LDA replaces a 2-byte
  LDA-from-zp. Even or +1 byte.
- `IndexedData(name, off, X|Y)`: 3-byte abs,X/Y LDA replaces a
  2-byte LDA-from-zp. +1 byte at the use site.

But in every case, the dropped `STA <stage>` (2 bytes / 3 cycles)
plus the dropped initial `LDA <recomputable_src>` (2-3 bytes /
2-4 cycles, since A was being clobbered anyway) more than cover
the +1 byte at the use site. Net: −2 to −4 bytes, −3 to −5 cycles
per occurrence, plus the staging slot is freed from the function's
private ZP pool.

# Eligibility

A `Mov(<src>, Data(__local_<fn>__*))` def is a remat candidate if
`<src>` is recomputable AND no intervening instruction invalidates
that recompute between def and the first matching use:

- `Imm`, `ImmLabelLow`, `ImmLabelHigh`: trivially safe — no
  runtime dependencies.
- `Data(name, off)`: safe if `name` doesn't appear as the
  destination of any write within this function AND there's no
  `Call` between def and use (calls may write through external
  pointers we don't track here).
- `IndexedData(name, off, reg)`: same `name`-immutability check,
  PLUS the index register (`X` or `Y`) must not be written between
  def and use. Calls clobber X/Y per the zp_abi convention, so the
  `no Call between` constraint subsumes the reg-clobber check.

Other src shapes (`Frame` / `Stack` / `Indirect*`) aren't handled —
indirect addressing depends on a ZP pointer pair the caller may
modify, and re-deriving an FP-relative address would race with
intervening pushes.

# Walk

Per basic block (linear walk; stop at any block boundary or write
to the stage local that would invalidate the def). Match
`Mov(<recomp>, Data(__local_<fn>__*))` as the def; scan forward
for the first `Mov(Data(<same_local>), <any_dst>)` use; if
eligible, rewrite the use's src to `<recomp>` directly. The dead
`STA` and the now-isolated original `LDA` get dropped on the next
fixedpoint iteration by the existing DSE/DCE peepholes.
"""
from __future__ import annotations

import asm_ast


def apply_remat(
    prog: asm_ast.Program,
    *,
    zp_slot_symbols: dict[str, int] | None = None,
) -> asm_ast.Program:
    """Forward recomputable defs of `__local_<fn>__*` staging cells
    to their consumers, leaving the staging Mov dead for downstream
    DSE/DCE to drop.

    `zp_slot_symbols` maps slot names like `__local_<fn>__<sym>` to
    their concrete ZP addresses. Used by `_drop_dead_stage_dsts` to
    detect when a slot is referenced indirectly (via an
    `IndirectZp(addr, off)` pointer pair, for instance) — without
    address resolution we'd miss pointer-aliased reads and drop
    stores that were keeping a multi-byte pointer pair alive."""
    new_top: list[asm_ast.Type_top_level] = []
    for tl in prog.top_level:
        if isinstance(tl, asm_ast.Function):
            new_top.append(_rewrite_function(tl, zp_slot_symbols or {}))
        else:
            new_top.append(tl)
    return asm_ast.Program(top_level=new_top)


def _rewrite_function(
    fn: asm_ast.Function,
    zp_slot_symbols: dict[str, int],
) -> asm_ast.Function:
    writable = _collect_writable_names(fn.instructions)
    out = _rewrite_uses(list(fn.instructions), writable)
    out = _drop_dead_stage_dsts(out, zp_slot_symbols)
    return asm_ast.Function(
        name=fn.name,
        is_global=fn.is_global,
        params=list(fn.params),
        instructions=out,
    )


def _rewrite_uses(
    instrs: list[asm_ast.Type_instruction],
    writable: set[str],
) -> list[asm_ast.Type_instruction]:
    out: list[asm_ast.Type_instruction] = list(instrs)
    for i, defn in enumerate(out):
        stage = _classify_stage_def(out, i)
        if stage is None:
            continue
        recomp, range_start = stage
        local_name = defn.dst.name
        # Walk forward in the same block for matching uses; rewrite
        # each that's still eligible. Multiple uses are allowed —
        # each gets the same recomputed src.
        for j in range(i + 1, len(out)):
            cur = out[j]
            if _is_block_boundary(cur):
                break
            if _writes_local(cur, local_name):
                break
            if not (
                isinstance(cur, asm_ast.Mov)
                and not cur.is_volatile
                and _matches_local(cur.src, local_name)
            ):
                continue
            if not _can_remat(recomp, out, range_start, j, writable):
                break
            out[j] = asm_ast.Mov(
                src=recomp,
                dst=cur.dst,
                is_volatile=cur.is_volatile,
            )
    return out


def _classify_stage_def(
    instrs: list[asm_ast.Type_instruction], i: int,
) -> tuple[asm_ast.Type_operand, int] | None:
    """Determine the recomputable source for a staging def at index
    `i`, plus the range-start index from which `_can_remat` should
    validate. Returns `(recomp, range_start)` or None if `i` isn't a
    staging def.

    Two shapes:

      1. `Mov(<recomputable>, Data(__local_<fn>__*))` — single
         mem-to-mem atom. `recomp = instrs[i].src`, range_start = i.

      2. `Mov(Reg(A), Data(__local_<fn>__*))` — two-atom shape,
         where the producer is an immediately preceding (modulo
         flag-only ops) `Mov(<recomputable>, Reg(A))`. The two-atom
         form appears after the asm-SSA round-trip splits a
         mem-to-mem Mov into its emit-time `LDA src; STA dst`
         pair. `recomp = producer.src`, range_start = producer_idx.
         The validation range covers the producer through the use,
         catching any X / Y / Call that would invalidate the
         recompute.
    """
    defn = instrs[i]
    if not isinstance(defn, asm_ast.Mov):
        return None
    if defn.is_volatile:
        return None
    if not isinstance(defn.dst, asm_ast.Data):
        return None
    if not defn.dst.name.startswith("__local_"):
        return None
    if defn.dst.offset != 0:
        return None
    # Shape 1.
    if _is_recomputable_shape(defn.src):
        return defn.src, i
    # Shape 2: src is Reg(A); find producer.
    if isinstance(defn.src, asm_ast.Reg) and isinstance(
        defn.src.reg, asm_ast.A,
    ):
        prod_idx = _find_a_producer(instrs, i)
        if prod_idx is None:
            return None
        producer = instrs[prod_idx]
        if not isinstance(producer, asm_ast.Mov):
            return None
        if producer.is_volatile:
            return None
        if not _is_recomputable_shape(producer.src):
            return None
        return producer.src, prod_idx
    return None


def _find_a_producer(
    instrs: list[asm_ast.Type_instruction], i: int,
) -> int | None:
    """Walk backward from index `i` to find the most recent
    instruction whose destination is `Reg(A)`. Returns its index, or
    None if a block boundary is hit first or A's value comes from an
    instruction whose effect we can't model as a clean producer
    (e.g. an arithmetic op like `Add` / `Sub` / `And` that combines
    A with another value — those clobber A but their "produced"
    value isn't recomputable from a single operand).

    Producer shapes accepted:
      * `Mov(_, Reg(A))` — load into A.

    Anything else that writes A (`Add`, `Sub`, `And`, `Or`, `Xor`,
    `ASL`/`LSR`/`ROL`/`ROR` on Reg(A), `Pop(Reg(A))`) blocks the
    search — A's value at `i` isn't the result of a single
    recomputable load.
    """
    for k in range(i - 1, -1, -1):
        instr = instrs[k]
        if _is_block_boundary(instr):
            return None
        if isinstance(instr, asm_ast.Mov):
            if (isinstance(instr.dst, asm_ast.Reg)
                    and isinstance(instr.dst.reg, asm_ast.A)):
                return k
            continue
        # Any other instruction that writes A blocks the search.
        if _writes_a(instr):
            return None
    return None


def _writes_a(instr: asm_ast.Type_instruction) -> bool:
    """True iff `instr` writes Reg(A) via something other than a
    Mov (those are caught by the caller). Arithmetic / logic ops
    on Reg(A) write A; in-place shifts on Reg(A) too; Pop(A)
    writes A."""
    if isinstance(instr, (
        asm_ast.Add, asm_ast.Sub, asm_ast.And, asm_ast.Or,
    )):
        return (isinstance(instr.dst, asm_ast.Reg)
                and isinstance(instr.dst.reg, asm_ast.A))
    if isinstance(instr, asm_ast.Xor):
        return (isinstance(instr.dst, asm_ast.Reg)
                and isinstance(instr.dst.reg, asm_ast.A))
    if isinstance(instr, (
        asm_ast.ArithmeticShiftLeft, asm_ast.LogicalShiftRight,
        asm_ast.RotateLeft, asm_ast.RotateRight,
    )):
        return (isinstance(instr.dst, asm_ast.Reg)
                and isinstance(instr.dst.reg, asm_ast.A))
    if isinstance(instr, asm_ast.Pop):
        return (isinstance(instr.dst, asm_ast.Reg)
                and isinstance(instr.dst.reg, asm_ast.A))
    return False


def _drop_dead_stage_dsts(
    instrs: list[asm_ast.Type_instruction],
    zp_slot_symbols: dict[str, int],
) -> list[asm_ast.Type_instruction]:
    """For every `Mov(<src>, Data(__local_<fn>__<...>))` atom whose
    stage byte is no longer read ANYWHERE in the function, collapse
    it to `Mov(<src>, Reg(A))`. The dropped mem-dst is what made the
    atom survive `apply_asm_dead_store` (the DSE only matches
    STA-shaped `Mov`s with a register src, not mem-to-mem Movs).
    After the collapse, the atom is a pure `LDA <src>` — and if
    `<src>`'s A-value is dead at the next instruction (the common
    case after remat, since the next thing is usually another
    marshaling `LDA #c`), `apply_dead_a_arith_elimination` or
    `apply_redundant_load_elimination` drops it on a subsequent
    fixedpoint iteration.

    "Read" includes indirect-pointer references via
    `IndirectZp(addr, off)` / `IndirectZpY(addr)` — those operands
    name a ZP cell whose 2 bytes are dereferenced. A `__local_*`
    slot that's part of an indirect-pointer pair must stay alive
    even if its symbolic name doesn't appear in any source operand
    elsewhere; we resolve names through `zp_slot_symbols` and
    check by address."""
    referenced_addrs = _collect_referenced_addrs(instrs, zp_slot_symbols)
    reg_a = asm_ast.Reg(reg=asm_ast.A())
    out: list[asm_ast.Type_instruction] = []
    for instr in instrs:
        if (
            isinstance(instr, asm_ast.Mov)
            and not instr.is_volatile
            and isinstance(instr.dst, asm_ast.Data)
            and instr.dst.name.startswith("__local_")
            and instr.dst.offset == 0
            and instr.dst.name in zp_slot_symbols
            and zp_slot_symbols[instr.dst.name] not in referenced_addrs
        ):
            # If src is already Reg(A) the staging def collapsed
            # from `STA local` was the second half of a two-atom
            # `LDA <src>; STA local` pair (the first half is still
            # in the IR as the producer Mov). Re-emitting it as
            # `Mov(Reg(A), Reg(A))` would just be a self-Mov that
            # no peephole drops at IR level; just omit the
            # instruction outright — A already holds the value.
            if (isinstance(instr.src, asm_ast.Reg)
                    and isinstance(instr.src.reg, asm_ast.A)):
                continue
            out.append(asm_ast.Mov(
                src=instr.src,
                dst=reg_a,
                is_volatile=False,
            ))
            continue
        out.append(instr)
    return out


def _collect_referenced_addrs(
    instrs: list[asm_ast.Type_instruction],
    zp_slot_symbols: dict[str, int],
) -> set[int]:
    """Set of ZP addresses that are READ anywhere in the function
    (any operand shape, resolved to concrete byte addresses). A
    stage byte can be collapsed only if its address is NOT here —
    even a single indirect-pointer read of the byte (or its
    pair-partner one byte over, since the indirect-mode addressing
    reads both bytes of the pointer) keeps it live.

    Source shapes:
      * `Data(name, off)` / `IndexedData(name, off, _)` — single
        byte at `zp_slot_symbols[name] + off` (when name is a
        slot symbol; else skip — link-time-addressed globals
        aren't in our pool).
      * `ZP(addr, off)` — single byte at `addr + off`.
      * `IndirectZp(addr, off)` — the pointer pair at
        `addr + off` and `addr + off + 1` is read.
      * `IndirectZpY(addr)` — the pointer pair at `addr` and
        `addr + 1` is read.
      * Other shapes (`Reg`, `Imm`, `ImmLabel*`, `Frame`, `Stack`,
        `Indirect`, `IndirectY`) don't reference our pool."""
    out: set[int] = set()
    for instr in instrs:
        for src in _read_operands(instr):
            _add_referenced_addrs(src, zp_slot_symbols, out)
    return out


def _add_referenced_addrs(
    op: asm_ast.Type_operand,
    zp_slot_symbols: dict[str, int],
    referenced: set[int],
) -> None:
    if isinstance(op, asm_ast.Data):
        base = zp_slot_symbols.get(op.name)
        if base is not None:
            referenced.add(base + op.offset)
    elif isinstance(op, asm_ast.IndexedData):
        base = zp_slot_symbols.get(op.name)
        if base is not None:
            referenced.add(base + op.offset)
    elif isinstance(op, asm_ast.ZP):
        referenced.add(op.address + op.offset)
    elif isinstance(op, asm_ast.IndirectZp):
        # Pointer pair: low byte at addr+off, high at addr+off+1.
        referenced.add(op.address + op.offset)
        referenced.add(op.address + op.offset + 1)
    elif isinstance(op, asm_ast.IndirectZpY):
        referenced.add(op.address)
        referenced.add(op.address + 1)


def _read_operands(
    instr: asm_ast.Type_instruction,
) -> list[asm_ast.Type_operand]:
    """Yield every operand `instr` reads (the src side). RMW atoms
    like `Inc` / `Dec` / shifts read AND write the same operand —
    we yield it as a read here so the referenced-locals scan stays
    conservative on them. `Mov.src` is read; `Mov.dst` is not
    (Mov is a pure write to its dst)."""
    out: list[asm_ast.Type_operand] = []
    if isinstance(instr, asm_ast.Mov):
        out.append(instr.src)
    elif isinstance(instr, (asm_ast.Add, asm_ast.Sub, asm_ast.And, asm_ast.Or)):
        out.append(instr.src)
        out.append(instr.dst)
    elif isinstance(instr, asm_ast.Xor):
        out.append(instr.src1)
        out.append(instr.src2)
    elif isinstance(instr, (
        asm_ast.Inc, asm_ast.Dec,
        asm_ast.ArithmeticShiftLeft, asm_ast.LogicalShiftRight,
        asm_ast.RotateLeft, asm_ast.RotateRight,
    )):
        out.append(instr.dst)
    elif isinstance(instr, asm_ast.Push):
        out.append(instr.src)
    elif isinstance(instr, asm_ast.Compare):
        out.append(instr.left)
        out.append(instr.right)
    elif isinstance(instr, asm_ast.BitTest):
        out.append(instr.src)
    elif isinstance(instr, asm_ast.LoadAddress):
        out.append(instr.src)
    return out


def _is_stage_def(instr: asm_ast.Type_instruction) -> bool:
    """True iff `instr` is `Mov(<recomputable>, Data(__local_<fn>__*))`."""
    if not isinstance(instr, asm_ast.Mov):
        return False
    if instr.is_volatile:
        return False
    if not isinstance(instr.dst, asm_ast.Data):
        return False
    if not instr.dst.name.startswith("__local_"):
        return False
    if instr.dst.offset != 0:
        return False
    return _is_recomputable_shape(instr.src)


def _is_recomputable_shape(src: asm_ast.Type_operand) -> bool:
    """A coarse `isinstance` filter; full eligibility (immutability
    of the source memory, register stability) lives in `_can_remat`,
    where the def-to-use range is known."""
    return isinstance(src, (
        asm_ast.Imm,
        asm_ast.ImmLabelLow,
        asm_ast.ImmLabelHigh,
        asm_ast.Data,
        asm_ast.IndexedData,
    ))


def _matches_local(op: asm_ast.Type_operand, name: str) -> bool:
    return (
        isinstance(op, asm_ast.Data)
        and op.name == name
        and op.offset == 0
    )


def _writes_local(
    instr: asm_ast.Type_instruction, name: str,
) -> bool:
    """True iff `instr` writes to the named local cell. Walks the
    same per-atom dst slots `_dsts_of` covers."""
    for dst in _dsts_of(instr):
        if (
            isinstance(dst, asm_ast.Data)
            and dst.name == name
            and dst.offset == 0
        ):
            return True
    return False


def _can_remat(
    recomp: asm_ast.Type_operand,
    instrs: list[asm_ast.Type_instruction],
    def_idx: int,
    use_idx: int,
    writable: set[str],
) -> bool:
    """Recompute eligibility for `recomp` at `use_idx`, given that
    it was the def at `def_idx`. Returns True iff the recompute at
    the use site yields the same byte value as the def site did.

    Imm / ImmLabel: trivially yes.
    Data / IndexedData: name must be immutable in this function,
    and no `Call` may sit between (calls may write to the named
    storage through callees we don't model). IndexedData additionally
    requires the index register (X or Y) to be unwritten between."""
    if isinstance(recomp, (
        asm_ast.Imm, asm_ast.ImmLabelLow, asm_ast.ImmLabelHigh,
    )):
        return True
    if isinstance(recomp, asm_ast.Data):
        if recomp.name in writable:
            return False
        return _no_call_between(instrs, def_idx, use_idx)
    if isinstance(recomp, asm_ast.IndexedData):
        if recomp.name in writable:
            return False
        if not _no_call_between(instrs, def_idx, use_idx):
            return False
        idx = recomp.index
        if isinstance(idx, asm_ast.X):
            return not _any_writes(_writes_x, instrs, def_idx, use_idx)
        if isinstance(idx, asm_ast.Y):
            return not _any_writes(_writes_y, instrs, def_idx, use_idx)
        return False
    return False


def _no_call_between(
    instrs: list[asm_ast.Type_instruction],
    def_idx: int,
    use_idx: int,
) -> bool:
    for k in range(def_idx + 1, use_idx):
        if isinstance(instrs[k], asm_ast.Call):
            return False
    return True


def _any_writes(
    predicate, instrs: list[asm_ast.Type_instruction],
    def_idx: int, use_idx: int,
) -> bool:
    return any(predicate(instrs[k]) for k in range(def_idx + 1, use_idx))


def _writes_x(instr: asm_ast.Type_instruction) -> bool:
    """True iff `instr` writes to Reg(X). LDX = `Mov(_, X)`; INX/DEX
    = `Inc(X)` / `Dec(X)`; PLX = `Pop(X)` (NMOS doesn't have PLX
    but defensively model it). Calls clobber X but are excluded
    upstream by `_no_call_between`."""
    return _writes_reg(instr, asm_ast.X)


def _writes_y(instr: asm_ast.Type_instruction) -> bool:
    return _writes_reg(instr, asm_ast.Y)


def _writes_reg(instr: asm_ast.Type_instruction, reg_cls) -> bool:
    if isinstance(instr, asm_ast.Mov):
        if isinstance(instr.dst, asm_ast.Reg) and isinstance(
            instr.dst.reg, reg_cls,
        ):
            return True
    if isinstance(instr, (asm_ast.Inc, asm_ast.Dec)):
        if isinstance(instr.dst, asm_ast.Reg) and isinstance(
            instr.dst.reg, reg_cls,
        ):
            return True
    if isinstance(instr, asm_ast.Pop):
        if isinstance(instr.dst, asm_ast.Reg) and isinstance(
            instr.dst.reg, reg_cls,
        ):
            return True
    return False


def _is_block_boundary(instr: asm_ast.Type_instruction) -> bool:
    """Linear walk terminators. Control flow into/out of the block
    means we can't reason about which atoms ran between def and use,
    so stop the scan here."""
    return isinstance(instr, (
        asm_ast.Label, asm_ast.Jump, asm_ast.Branch,
        asm_ast.Call, asm_ast.Ret, asm_ast.Return,
        asm_ast.FunctionPrologue, asm_ast.AllocateStack,
        asm_ast.LoadAddress,
    ))


def _collect_writable_names(
    instrs: list[asm_ast.Type_instruction],
) -> set[str]:
    """Set of static-storage `name` strings written anywhere in this
    function. A name absent from this set is, for the duration of
    this function's execution, immutable — provided we also see no
    `Call` between the def and use (which is checked separately;
    calls can reach external writers)."""
    out: set[str] = set()
    for instr in instrs:
        for dst in _dsts_of(instr):
            if isinstance(dst, (asm_ast.Data, asm_ast.IndexedData)):
                out.add(dst.name)
    return out


def _dsts_of(
    instr: asm_ast.Type_instruction,
) -> list[asm_ast.Type_operand]:
    """Yield every operand `instr` writes (the dst side; reads aren't
    yielded). Used to compute the set of statically-written names."""
    out: list[asm_ast.Type_operand] = []
    if isinstance(instr, asm_ast.Mov):
        out.append(instr.dst)
    elif isinstance(instr, (
        asm_ast.Add, asm_ast.Sub, asm_ast.And, asm_ast.Or, asm_ast.Xor,
    )):
        out.append(instr.dst)
    elif isinstance(instr, (
        asm_ast.Inc, asm_ast.Dec,
        asm_ast.ArithmeticShiftLeft, asm_ast.LogicalShiftRight,
        asm_ast.RotateLeft, asm_ast.RotateRight,
    )):
        out.append(instr.dst)
    elif isinstance(instr, asm_ast.Pop):
        out.append(instr.dst)
    elif isinstance(instr, asm_ast.LoadAddress):
        out.append(instr.dst)
    return out
