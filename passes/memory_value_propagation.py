"""CFG-aware memory-value propagation.

Tracks, at every program point in a function, the recomputable
source expression that each ZP byte currently holds. The analysis
is a forward must-equal dataflow over the function's CFG; meets at
join points keep only facts agreed by every predecessor.

The rewriter walks each instruction using its in-state and rewrites
operand reads of tracked cells to read from the canonical source
directly. Milestone 1 emits a single rewrite: indirect-via-DPTR
accesses (`Indirect(off)` / `IndirectY()` operands) get rewritten
to `IndirectZp(N, off)` / `IndirectZpY(N)` whenever the dataflow
proves DPTR currently holds the bytes of a stable ZP pair at
address `N`. Later milestones add the `apply_remat`-style rewrite
(reads of a stage cell whose source is a recomputable
`Imm`/`Data`/`IndexedData`) and the X-save-slot rewrite (reads of
`M` rewritten to `TXA`/`STX` when M tracks X across DEX/INX).

# Why CFG-aware

The pre-existing `apply_indirect_base_prop` performed the same
rewrite but block-locally — every `Label` / `Jump` / `Branch`
cleared its equivalence. For shapes like

    .preheader:
       LDA  __zpabi_fn_p0
       STA  DPTR
       LDA  __zpabi_fn_p1
       STA  DPTR+1
    .loop_start:
       ...
       STA  (DPTR),Y      ; ← rewrite missed because of the label

the equivalence dropped at `.loop_start` and the indirect access
inside the loop kept the (now-redundant) DPTR staging in the
output. CFG-aware dataflow propagates the equivalence into and
around the loop body as long as no instruction kills it.

# Lattice

State at a program point:

  - `a_value: Expr | None` — what `Reg(A)` currently holds, when
    expressible as a tracked Expr. None = unknown.
  - `cells: dict[int, Expr]` — what each tracked ZP byte address
    currently holds.

`Expr` is initially just `ZPRef(addr)` — "the byte value at ZP
address `addr`, as of the last time `addr` was written before the
fact was established." A `ZPRef(X)` fact for cell K means "K's
value equals X's value, AS LONG AS neither K nor X has been
written since the fact was established."

Meet at joins is set-intersection: a fact survives the meet iff
every predecessor's out-state agrees on it. `None` (TOP)
represents "unvisited"; `meet(None, state) = state`.

# Transfer function — what kills facts

A write to ZP byte W:
  - Kills `cells[W]` (W's value changed).
  - Kills any `cells[K]` whose Expr mentions W (the recorded
    equivalence referred to W's old value, which is now gone).
  - Kills `a_value` if it mentions W.

Indirect or unknown writes (`Frame`, `Indirect*`, `Stack`,
`IndexedData` whose range can't be bounded):
  - Conservatively kill all `cells` entries and `a_value`.

`Call`:
  - Conservatively kill everything. (A more precise version would
    keep facts about cells in the caller's private pool, since the
    pool allocator guarantees no callee can write to them. Deferred
    to a future milestone.)

`FunctionPrologue` / `AllocateStack` / `LoadAddress`:
  - Compound atoms that lower to multiple Movs; conservatively kill
    everything.

# Transfer function — what establishes facts

`Mov(src, Reg(A))` where src resolves to a stable ZP cell at
address X: sets `a_value := ZPRef(X)`.

`Mov(Reg(A), dst)` where dst resolves to a ZP cell at address Y
and `a_value` is a known Expr E: sets `cells[Y] := E`.

`Mov(src, dst)` (mem-to-mem) where both src and dst resolve to ZP
cells at addresses X and Y: at emit time this lowers to `LDA src;
STA dst`, so A becomes ZPRef(X) AND cells[Y] := ZPRef(X). The
analysis captures both effects.

`Mov(Imm(_), …)` or other A-clobbering ops: clear `a_value`.

# Operand resolution

ZP byte address of an operand is determined by:

  - `ZP(addr, off)` → `addr + off`.
  - `Data(name, off)` → `zp_symbol_addrs[name] + off` if the name
    resolves to a ZP byte (`<= $FF`).
  - Anything else → None (not a tracked ZP cell).

# Where to run

After `replace_pseudoregisters_bare_exit` (operands concrete) and
inside the `_peephole_fixedpoint` loop. Replaces the former
`apply_indirect_base_prop` (deleted; this pass is its CFG-aware
successor) and overlaps with `apply_remat`'s rewrites for Imm /
Data / ImmLabel sources (the remaining apply_remat coverage is
the IndexedData source, which this pass doesn't yet handle).
"""
from __future__ import annotations

from dataclasses import dataclass, field
from typing import Optional

import asm_ast
from passes.optimization_asm.cfg import (
    BasicBlock, CFG, build_cfg, ENTRY_ID, EXIT_ID,
)


# ---- Tracked source expressions ----

@dataclass(frozen=True)
class ZPRef:
    """The byte value currently at ZP address `addr`. Invalidated
    when `addr` is written."""
    addr: int


@dataclass(frozen=True)
class ImmExpr:
    """An 8-bit literal value. Never invalidated."""
    value: int


@dataclass(frozen=True)
class ImmLabelLowExpr:
    """The low byte of the link-time address `name + offset`. Never
    invalidated (link-time symbols are immutable at runtime)."""
    name: str
    offset: int


@dataclass(frozen=True)
class ImmLabelHighExpr:
    """The high byte of the link-time address `name + offset`."""
    name: str
    offset: int


@dataclass(frozen=True)
class DataExpr:
    """The byte value at the link-time address `name + offset`,
    where `name` resolves to a non-ZP memory location (statics).
    Invalidated when any byte of `name` is written."""
    name: str
    offset: int


@dataclass(frozen=True)
class IndexedDataExpr:
    """The byte value at `name + offset + index_reg's value`, where
    `name` resolves to a non-ZP memory location. The `index_token`
    captures the current "identity" of the index register at the
    time the fact was established — it must match the index
    register's identity at the use site for the recompute to be
    sound. Identity is a positive integer that gets bumped every
    time the index register is written; the same integer at two
    points means the register's value has been continuously stable
    between them within the analysis's tracking ability."""
    name: str
    offset: int
    idx_is_x: bool       # True for X-indexed, False for Y-indexed
    idx_token: int


# Union of recomputable expression types.
Expr = (
    ZPRef | ImmExpr | ImmLabelLowExpr | ImmLabelHighExpr
    | DataExpr | IndexedDataExpr
)


# ---- Lattice state ----

@dataclass
class State:
    a_value: Optional[Expr] = None
    cells: dict[int, Expr] = field(default_factory=dict)
    # Identity tokens for the X and Y registers. Bumped every time
    # the corresponding register is written. None when the register
    # was never written within the analysis's reach (treated as a
    # distinct value from any previously-bumped token).
    #
    # The token is purely an opaque identifier for "the index
    # register's current value." Two states with the same token for
    # X mean X has been continuously unchanged on every path
    # between the two — sound for an IndexedData recompute. Two
    # states with different tokens mean X has been written
    # somewhere; the recompute is unsound.
    x_token: Optional[int] = None
    y_token: Optional[int] = None

    def copy(self) -> "State":
        return State(
            a_value=self.a_value,
            cells=dict(self.cells),
            x_token=self.x_token,
            y_token=self.y_token,
        )

    def __eq__(self, other):
        if not isinstance(other, State):
            return False
        return (
            self.a_value == other.a_value
            and self.cells == other.cells
            and self.x_token == other.x_token
            and self.y_token == other.y_token
        )


# `None` is TOP (unvisited / "every possible fact"). A `State`
# instance is concrete.
_StateOrTop = Optional[State]


# ---- Runtime symbol addresses (mirrors apply_indirect_base_prop) ----

_DPTR_LO = 0x24
_DPTR_HI = 0x25

_RUNTIME_ZP_ADDRS = {
    "SSP": 0x00,
    "FP": 0x02,
    "HARGS": 0x04,
    "DPTR": _DPTR_LO,
}


def apply_memory_value_propagation(
    prog: asm_ast.Program,
    *,
    zp_symbol_addrs: dict[str, int] | None = None,
) -> asm_ast.Program:
    """Top-level entry. `zp_symbol_addrs` extends the runtime-symbol
    table with caller-supplied `Data(name)` → byte-address bindings
    (typically the `__zpabi_*` and `__local_*` slot symbols)."""
    addrs = dict(_RUNTIME_ZP_ADDRS)
    if zp_symbol_addrs:
        addrs.update(zp_symbol_addrs)
    new_top: list[asm_ast.Type_top_level] = []
    for tl in prog.top_level:
        if isinstance(tl, asm_ast.Function):
            new_top.append(_rewrite_function(tl, addrs))
        else:
            new_top.append(tl)
    return asm_ast.Program(top_level=new_top)


def _rewrite_function(
    fn: asm_ast.Function, zp_addrs: dict[str, int],
) -> asm_ast.Function:
    # `build_cfg` raises if any `Jump` / `Branch` targets a label
    # name not present in this function — `apply_tail_call`'s
    # JSR-then-RTS → JMP rewrite produces exactly this shape (the
    # JMP target is another function's name, not a local label).
    # When that happens, bail and leave the function unchanged.
    # Functions with tail calls are rare; the optimization gain from
    # rewriting around them is small.
    try:
        cfg = build_cfg(fn)
    except KeyError:
        return fn
    # Precompute the set of `Data(name)` symbols this function
    # writes anywhere. A DataExpr fact about an unmutated name is
    # safe to recompute at any program point (modulo the kill rules
    # for direct writes the dataflow already handles). A DataExpr
    # whose name appears here is too risky — we conservatively
    # exclude it.
    writable = _writable_data_names(fn)
    # Precompute per-instruction tokens for X / Y writes. The token
    # is the instruction's overall position in the function (an
    # int); it's stable across re-iterations of the worklist, and
    # two states agreeing on a token means they reached this point
    # via the same most-recent X (or Y) write. See
    # `_index_register_tokens` for the encoding rules.
    x_tokens, y_tokens = _index_register_tokens(fn)
    ctx = _Ctx(
        zp_addrs=zp_addrs, writable=writable,
        x_tokens=x_tokens, y_tokens=y_tokens,
    )
    in_states = _solve(cfg, ctx)
    new_instrs = _rewrite(cfg, in_states, ctx)
    return asm_ast.Function(
        name=fn.name, is_global=fn.is_global,
        params=list(fn.params), instructions=new_instrs,
    )


@dataclass
class _Ctx:
    """Per-function precomputed context threaded through the
    dataflow and rewriter. Keeps the signature of `_transfer` and
    friends compact."""
    zp_addrs: dict[str, int]
    writable: frozenset[str]
    x_tokens: dict[int, int]
    y_tokens: dict[int, int]


def _index_register_tokens(
    fn: asm_ast.Function,
) -> tuple[dict[int, int], dict[int, int]]:
    """For each instruction in `fn` that writes Reg(X) or Reg(Y),
    record a unique token. Returns
    `({id(instr): token}, {id(instr): token})` — separate maps for
    X and Y. The token is just the instruction's 1-based position
    in the function's instruction list; this is stable, unique
    across writes, and distinct from the initial-state token (0)."""
    x_tokens: dict[int, int] = {}
    y_tokens: dict[int, int] = {}
    for i, instr in enumerate(fn.instructions, start=1):
        if _writes_x(instr):
            x_tokens[id(instr)] = i
        if _writes_y(instr):
            y_tokens[id(instr)] = i
    return x_tokens, y_tokens


def _writes_x(instr: asm_ast.Type_instruction) -> bool:
    return _writes_reg(instr, asm_ast.X)


def _writes_y(instr: asm_ast.Type_instruction) -> bool:
    return _writes_reg(instr, asm_ast.Y)


def _writes_reg(instr: asm_ast.Type_instruction, reg_cls) -> bool:
    if isinstance(instr, asm_ast.Mov):
        return (isinstance(instr.dst, asm_ast.Reg)
                and isinstance(instr.dst.reg, reg_cls))
    if isinstance(instr, (asm_ast.Inc, asm_ast.Dec)):
        return (isinstance(instr.dst, asm_ast.Reg)
                and isinstance(instr.dst.reg, reg_cls))
    if isinstance(instr, asm_ast.Pop):
        return (isinstance(instr.dst, asm_ast.Reg)
                and isinstance(instr.dst.reg, reg_cls))
    # Calls clobber all registers; we model that via the token-
    # bumping at the Call instruction itself, but `_writes_reg`
    # returns False here since `_index_register_tokens` shouldn't
    # bump for a Call (the bump happens in `_transfer` via
    # the Call's `id(instr)` lookup OR via a separate path —
    # for simplicity we treat Call as "X/Y unknown" by killing the
    # tokens entirely in `_transfer`).
    return False


def _writable_data_names(fn: asm_ast.Function) -> frozenset[str]:
    """Set of Data symbol names that appear as the destination of
    any write in `fn`. A `DataExpr(name, _)` is safe to recompute
    only when `name` is NOT in this set."""
    out: set[str] = set()
    for instr in fn.instructions:
        dst = _instr_write_dst(instr)
        if dst is None:
            continue
        if isinstance(dst, asm_ast.Data):
            out.add(dst.name)
        elif isinstance(dst, asm_ast.IndexedData):
            out.add(dst.name)
    return frozenset(out)


def _instr_write_dst(
    instr: asm_ast.Type_instruction,
) -> asm_ast.Type_operand | None:
    if isinstance(instr, asm_ast.Mov):
        return instr.dst
    if isinstance(instr, (
        asm_ast.Add, asm_ast.Sub, asm_ast.And, asm_ast.Or,
        asm_ast.Inc, asm_ast.Dec, asm_ast.ArithmeticShiftLeft,
        asm_ast.LogicalShiftRight, asm_ast.RotateLeft,
        asm_ast.RotateRight,
    )):
        return instr.dst
    if isinstance(instr, asm_ast.Xor):
        return instr.dst
    if isinstance(instr, asm_ast.Pop):
        return instr.dst
    return None


# ---- Dataflow ----

def _solve(
    cfg: CFG, ctx: _Ctx,
) -> dict[int, _StateOrTop]:
    """Forward worklist dataflow. Returns per-block in-states. Block
    in-state is the meet of its predecessors' out-states; out-state
    is the transfer of all instructions in the block applied to the
    in-state."""
    in_state: dict[int, _StateOrTop] = {}
    out_state: dict[int, _StateOrTop] = {}
    for bid in cfg.blocks:
        in_state[bid] = None
        out_state[bid] = None
    # At function entry: X and Y were assigned by the caller — we
    # consider that an opaque "initial" value with token 0. Any
    # in-function X/Y write will bump to a positive token, so a
    # later state with x_token=0 means "X hasn't been written
    # since entry."
    in_state[ENTRY_ID] = State(x_token=0, y_token=0)
    out_state[ENTRY_ID] = State(x_token=0, y_token=0)
    worklist: list[int] = list(cfg.block_order) + [EXIT_ID]
    seen: set[int] = set()
    iterations = 0
    # The worklist algorithm is bounded by the number of blocks
    # times the lattice height. For a function with N blocks and
    # constant-bounded state size, this is small. The hard cap is a
    # safety net against runaway iteration if a transfer is
    # non-monotonic (which would be a bug).
    cap = max(1024, len(cfg.blocks) * 16)
    while worklist:
        iterations += 1
        if iterations > cap:
            # Bail conservatively: clear all states to bottom so the
            # rewriter doesn't apply any unsound rewrites. Production
            # code shouldn't hit this; raise loudly so tests catch
            # regressions in transfer monotonicity.
            raise RuntimeError(
                "memory_value_propagation: worklist did not "
                f"converge after {cap} iterations"
            )
        bid = worklist.pop(0)
        if bid == ENTRY_ID:
            continue
        preds = cfg.blocks[bid].predecessors
        new_in: _StateOrTop = None
        for p in preds:
            new_in = _meet(new_in, out_state[p])
        if new_in is None:
            # No processed predecessor yet — wait until at least one
            # is processed. (Will be re-added when a predecessor's
            # out-state changes.)
            continue
        first_visit = bid not in seen
        if not first_visit and new_in == in_state[bid]:
            continue
        seen.add(bid)
        in_state[bid] = new_in
        new_out = _transfer_block(cfg.blocks[bid], new_in, ctx)
        if new_out != out_state[bid]:
            out_state[bid] = new_out
            for s in cfg.blocks[bid].successors:
                if s not in worklist:
                    worklist.append(s)
    return in_state


def _meet(a: _StateOrTop, b: _StateOrTop) -> _StateOrTop:
    if a is None:
        return b
    if b is None:
        return a
    a_val = a.a_value if a.a_value == b.a_value else None
    x_token = a.x_token if a.x_token == b.x_token else None
    y_token = a.y_token if a.y_token == b.y_token else None
    cells = {
        k: v for k, v in a.cells.items()
        if b.cells.get(k) == v
    }
    return State(
        a_value=a_val, cells=cells,
        x_token=x_token, y_token=y_token,
    )


def _transfer_block(
    block: BasicBlock, in_state: State, ctx: _Ctx,
) -> State:
    state = in_state.copy()
    for instr in block.instructions:
        _transfer(instr, state, ctx)
    return state


def _transfer(
    instr: asm_ast.Type_instruction, state: State, ctx: _Ctx,
) -> None:
    """In-place transfer of `instr`'s effects on `state`."""
    # Calls / compound atoms: opaque, kill everything (cells, A
    # register, and the X/Y tokens since a callee can clobber them).
    if isinstance(instr, (
        asm_ast.Call, asm_ast.FunctionPrologue,
        asm_ast.AllocateStack, asm_ast.LoadAddress,
        asm_ast.Phi,
    )):
        state.a_value = None
        state.cells.clear()
        # The Call could write X/Y; bump tokens to "unknown" so any
        # IndexedDataExpr fact established before is no longer
        # recoverable.
        state.x_token = None
        state.y_token = None
        return
    # Block-terminator atoms are visited too (they end blocks, but
    # the dataflow framework still calls transfer on them); they
    # don't have memory effects, so no-op. Compare / BitTest only
    # set flags; SetCarry / ClearCarry only set the C flag.
    if isinstance(instr, (
        asm_ast.Label, asm_ast.Jump, asm_ast.Branch,
        asm_ast.Ret, asm_ast.Return,
        asm_ast.SetCarry, asm_ast.ClearCarry,
        asm_ast.Compare, asm_ast.BitTest,
    )):
        return
    # Mov: the main vehicle for establishing and killing facts.
    if isinstance(instr, asm_ast.Mov):
        _transfer_mov(instr, state, ctx)
        # Update X/Y tokens if this Mov writes to X or Y.
        _bump_index_tokens(instr, state, ctx)
        return
    # Arithmetic / logic on Reg(A): clobbers A; some kill cells.
    if isinstance(instr, (
        asm_ast.Add, asm_ast.Sub, asm_ast.And, asm_ast.Or,
    )):
        if _is_reg_a(instr.dst):
            state.a_value = None
        else:
            _kill_writes(instr.dst, state, ctx.zp_addrs)
        return
    if isinstance(instr, asm_ast.Xor):
        if _is_reg_a(instr.dst):
            state.a_value = None
        else:
            _kill_writes(instr.dst, state, ctx.zp_addrs)
        return
    # In-place RMW on a memory cell (or register).
    if isinstance(instr, (
        asm_ast.Inc, asm_ast.Dec, asm_ast.ArithmeticShiftLeft,
        asm_ast.LogicalShiftRight, asm_ast.RotateLeft,
        asm_ast.RotateRight,
    )):
        if isinstance(instr.dst, asm_ast.Reg):
            if _is_reg_a(instr.dst):
                state.a_value = None
            else:
                _bump_index_tokens(instr, state, ctx)
            return
        _kill_writes(instr.dst, state, ctx.zp_addrs)
        return
    # Pop: stack pop into A / X / Y / memory.
    if isinstance(instr, asm_ast.Pop):
        if isinstance(instr.dst, asm_ast.Reg):
            if _is_reg_a(instr.dst):
                state.a_value = None
            else:
                _bump_index_tokens(instr, state, ctx)
            return
        _kill_writes(instr.dst, state, ctx.zp_addrs)
        return
    # Push: reads only, no memory effect on tracked cells.
    if isinstance(instr, asm_ast.Push):
        return


def _transfer_mov(
    instr: asm_ast.Mov, state: State, ctx: _Ctx,
) -> None:
    src, dst = instr.src, instr.dst
    # `src_expr` is the recomputable Expr that `src` represents at
    # this point, if any. None means "src isn't recomputable as
    # tracked" (or it's a Reg, handled separately).
    src_expr = _src_expr(src, state, ctx)
    dst_zp = _zp_addr(dst, ctx.zp_addrs)

    # Compute the new A value and whether A is clobbered.
    new_a: Optional[Expr] = state.a_value
    a_clobbered = False
    if isinstance(dst, asm_ast.Reg):
        if _is_reg_a(dst):
            # `Mov(src, Reg(A))` — LDA src / TXA / TYA. A := src.
            if isinstance(src, asm_ast.Reg):
                # TXA / TYA: inter-register; we don't track X/Y
                # values yet, so A's new value is unknown.
                new_a = None
            else:
                new_a = src_expr
            a_clobbered = True
        # Mov into Reg(X) / Reg(Y): A is preserved.
    else:
        # Mov(_, memory_dst): at emit time this is `LDA src; STA
        # dst` (or just STA when src is Reg(A); STX/STY for X/Y).
        if isinstance(src, asm_ast.Reg):
            # STA/STX/STY dst: A unchanged.
            pass
        else:
            # LDA src; STA dst: A := src's value.
            new_a = src_expr
            a_clobbered = True

    # Apply the kill side of the dst write FIRST.
    if not isinstance(dst, asm_ast.Reg):
        _kill_writes(dst, state, ctx.zp_addrs)

    # Apply the A update.
    if a_clobbered:
        state.a_value = new_a

    # Now establish a new fact for the dst cell (if dst is a tracked
    # ZP byte and src is a tracked recomputable expression).
    if dst_zp is None:
        return
    fact: Optional[Expr] = None
    if isinstance(src, asm_ast.Reg):
        if _is_reg_a(src):
            fact = state.a_value
        # STX/STY: dst := X/Y. We don't track X/Y values yet.
    else:
        fact = src_expr
    if fact is not None:
        # Don't record `cells[M] = ZPRef(M)` — a trivial self-
        # reference; rewriting M to itself is a no-op and the fact
        # adds noise to the lattice.
        if isinstance(fact, ZPRef) and fact.addr == dst_zp:
            return
        state.cells[dst_zp] = fact


def _src_expr(
    src: asm_ast.Type_operand,
    state: State, ctx: _Ctx,
) -> Optional[Expr]:
    """Return the Expr that `src` represents at the current state,
    if recomputable. Returns None for sources we don't track
    (Frame, Stack, Indirect, Reg, mutable Data names, or
    IndexedData with an unknown index-register token)."""
    if isinstance(src, asm_ast.Imm):
        return ImmExpr(value=src.value)
    if isinstance(src, asm_ast.ImmLabelLow):
        return ImmLabelLowExpr(name=src.name, offset=src.offset)
    if isinstance(src, asm_ast.ImmLabelHigh):
        return ImmLabelHighExpr(name=src.name, offset=src.offset)
    if isinstance(src, asm_ast.ZP):
        return ZPRef(addr=src.address + src.offset)
    if isinstance(src, asm_ast.Data):
        zp = _zp_addr(src, ctx.zp_addrs)
        if zp is not None:
            return ZPRef(addr=zp)
        if src.name in ctx.writable:
            return None
        return DataExpr(name=src.name, offset=src.offset)
    if isinstance(src, asm_ast.IndexedData):
        if src.name in ctx.writable:
            return None
        idx_is_x = isinstance(src.index, asm_ast.X)
        token = state.x_token if idx_is_x else state.y_token
        if token is None:
            return None
        return IndexedDataExpr(
            name=src.name, offset=src.offset,
            idx_is_x=idx_is_x, idx_token=token,
        )
    return None


def _bump_index_tokens(
    instr: asm_ast.Type_instruction, state: State, ctx: _Ctx,
) -> None:
    """Update `state.x_token` / `state.y_token` if `instr` writes
    to Reg(X) / Reg(Y). Uses the precomputed per-instruction token
    from `ctx`. Also kills any IndexedDataExpr facts whose
    idx_token no longer matches the new state token."""
    new_x = ctx.x_tokens.get(id(instr))
    new_y = ctx.y_tokens.get(id(instr))
    if new_x is not None:
        state.x_token = new_x
        _kill_stale_index_facts(state, is_x=True)
    if new_y is not None:
        state.y_token = new_y
        _kill_stale_index_facts(state, is_x=False)


def _kill_stale_index_facts(state: State, is_x: bool) -> None:
    """Remove any cell / a_value fact whose value is an
    IndexedDataExpr indexed by the just-bumped register but whose
    captured token no longer matches the new state token."""
    current_token = state.x_token if is_x else state.y_token
    for k, v in list(state.cells.items()):
        if (isinstance(v, IndexedDataExpr)
                and v.idx_is_x == is_x
                and v.idx_token != current_token):
            state.cells.pop(k, None)
    if (isinstance(state.a_value, IndexedDataExpr)
            and state.a_value.idx_is_x == is_x
            and state.a_value.idx_token != current_token):
        state.a_value = None


def _kill_writes(
    dst: asm_ast.Type_operand, state: State,
    zp_addrs: dict[str, int],
) -> None:
    """Kill every fact invalidated by a write to `dst`."""
    if isinstance(dst, (asm_ast.ZP, asm_ast.Data)):
        addr = _zp_addr(dst, zp_addrs)
        if addr is None:
            # Non-ZP static cell: kill any fact referring to this
            # Data symbol's byte.
            if isinstance(dst, asm_ast.Data):
                _kill_data_name(dst.name, dst.offset, state)
            return
        _kill_cell(addr, state)
        return
    if isinstance(dst, asm_ast.IndexedData):
        # Indexed write — base+0..base+255 range. Conservatively
        # invalidate facts whose addresses could fall in this range.
        # For tracked ZP-resolved Data symbols, the array would have
        # to extend INTO zero page — possible (`__local_*` arrays
        # are in ZP, though typically scalars). Be safe: kill all.
        state.cells.clear()
        state.a_value = None
        return
    # Frame / Stack / Indirect / IndirectY / IndirectZp /
    # IndirectZpY: indirect write through an unknown pointer.
    # Conservatively kill everything.
    state.cells.clear()
    state.a_value = None


def _kill_cell(addr: int, state: State) -> None:
    state.cells.pop(addr, None)
    for k, v in list(state.cells.items()):
        if isinstance(v, ZPRef) and v.addr == addr:
            state.cells.pop(k, None)
    if isinstance(state.a_value, ZPRef) and state.a_value.addr == addr:
        state.a_value = None


def _kill_data_name(name: str, offset: int, state: State) -> None:
    """Kill facts that depend on the named static-storage byte. Used
    when a `Mov(_, Data(name, offset))` writes to a non-ZP static
    cell — any tracked fact whose RHS is `DataExpr(name, offset)`
    becomes stale."""
    target = (name, offset)
    for k, v in list(state.cells.items()):
        if isinstance(v, DataExpr) and (v.name, v.offset) == target:
            state.cells.pop(k, None)
    if (isinstance(state.a_value, DataExpr)
            and (state.a_value.name, state.a_value.offset) == target):
        state.a_value = None


def _zp_addr(
    op: asm_ast.Type_operand, zp_addrs: dict[str, int],
) -> Optional[int]:
    if isinstance(op, asm_ast.ZP):
        return op.address + op.offset
    if isinstance(op, asm_ast.Data):
        base = zp_addrs.get(op.name)
        if base is None:
            return None
        addr = base + op.offset
        if addr > 0xFF:
            return None
        return addr
    return None


def _is_reg_a(op: asm_ast.Type_operand) -> bool:
    return isinstance(op, asm_ast.Reg) and isinstance(op.reg, asm_ast.A)


# ---- Rewriter ----

def _rewrite(
    cfg: CFG, in_states: dict[int, _StateOrTop], ctx: _Ctx,
) -> list[asm_ast.Type_instruction]:
    """Walk every block in CFG order, computing per-instruction
    in-states from the block's in-state and rewriting operands."""
    out: list[asm_ast.Type_instruction] = []
    for bid in cfg.block_order:
        block = cfg.blocks[bid]
        bin = in_states.get(bid) or State()
        state = bin.copy()
        for instr in block.instructions:
            out.append(_rewrite_instr(instr, state, ctx))
            _transfer(instr, state, ctx)
    return out


def _rewrite_instr(
    instr: asm_ast.Type_instruction, state: State, ctx: _Ctx,
) -> asm_ast.Type_instruction:
    """Apply per-operand rewrites based on `state`. Two rewrite
    families:

      1. Indirect-via-DPTR (`Indirect` / `IndirectY`) operands
         rewrite to `IndirectZp` / `IndirectZpY` when DPTR's bytes
         match a known stable ZP pair.

      2. Reads of a tracked ZP cell whose value is a recomputable
         Expr rewrite to read from the canonical source directly
         (subsumes `apply_remat`'s rewrite at CFG scope)."""
    # First pass: DPTR substitution (depends on cells[DPTR]).
    base = _dptr_base(state)
    if base is not None:
        instr = _rewrite_operands_for_dptr(instr, base)
    # Second pass: substitute cell-reads with their tracked Expr.
    return _rewrite_cell_reads(instr, state, ctx)


def _rewrite_cell_reads(
    instr: asm_ast.Type_instruction, state: State, ctx: _Ctx,
) -> asm_ast.Type_instruction:
    """For each src operand that resolves to a tracked ZP cell `M`
    where `state.cells[M]` is a recomputable Expr different from
    `ZPRef(M)`, rewrite the operand to express the Expr directly.

    Conservative on which contexts accept IndexedData substitution:
    LDA-shaped reads (`Mov(_, Reg(A))` and mem-to-mem Movs) always
    work; LDX-/LDY-shaped reads must not have the destination's
    register match the IndexedData's index (the 6502 has no
    LDX abs,X or LDY abs,Y). Compare-right and ALU-source positions
    accept Imm / Data / ZP substitutions but skip IndexedData
    until the asm_emit / sim assembler dispatch is wired up for
    those shapes."""
    if isinstance(instr, asm_ast.Mov):
        new_src = _rewrite_src(
            instr.src, state, ctx, allow_indexed=True,
        )
        if new_src is instr.src:
            return instr
        # Reject `Mov(IndexedData(...,X), Reg(X))` and
        # `Mov(IndexedData(...,Y), Reg(Y))` — the 6502 has no
        # `LDX abs,X` or `LDY abs,Y`.
        if (isinstance(new_src, asm_ast.IndexedData)
                and isinstance(instr.dst, asm_ast.Reg)
                and isinstance(instr.dst.reg, type(new_src.index))):
            return instr
        return asm_ast.Mov(
            src=new_src, dst=instr.dst, is_volatile=instr.is_volatile,
        )
    if isinstance(instr, (
        asm_ast.Add, asm_ast.Sub, asm_ast.And, asm_ast.Or,
    )):
        new_src = _rewrite_src(
            instr.src, state, ctx, allow_indexed=False,
        )
        if new_src is instr.src:
            return instr
        return type(instr)(src=new_src, dst=instr.dst)
    if isinstance(instr, asm_ast.Xor):
        new_s1 = _rewrite_src(
            instr.src1, state, ctx, allow_indexed=False,
        )
        new_s2 = _rewrite_src(
            instr.src2, state, ctx, allow_indexed=False,
        )
        if new_s1 is instr.src1 and new_s2 is instr.src2:
            return instr
        return asm_ast.Xor(
            src1=new_s1, src2=new_s2, dst=instr.dst,
        )
    if isinstance(instr, asm_ast.Compare):
        new_right = _rewrite_src(
            instr.right, state, ctx, allow_indexed=False,
        )
        if new_right is instr.right:
            return instr
        return asm_ast.Compare(left=instr.left, right=new_right)
    if isinstance(instr, asm_ast.Push):
        new_src = _rewrite_src(
            instr.src, state, ctx, allow_indexed=False,
        )
        if new_src is instr.src:
            return instr
        return asm_ast.Push(src=new_src)
    return instr




def _rewrite_src(
    op: asm_ast.Type_operand, state: State, ctx: _Ctx,
    *,
    allow_indexed: bool = False,
) -> asm_ast.Type_operand:
    """If `op` resolves to a tracked ZP cell with a recomputable
    Expr, return the operand form of that Expr. Otherwise return
    `op` unchanged. `allow_indexed=False` disables IndexedData
    substitution (caller couldn't accept the resulting operand
    shape)."""
    addr = _zp_addr(op, ctx.zp_addrs)
    if addr is None:
        return op
    fact = state.cells.get(addr)
    if fact is None:
        return op
    if isinstance(fact, ZPRef):
        # Don't substitute a ZP cell with another ZP cell — both are
        # zp addressing at the same cycle cost; chains add noise.
        return op
    if isinstance(fact, ImmExpr):
        return asm_ast.Imm(value=fact.value)
    if isinstance(fact, ImmLabelLowExpr):
        return asm_ast.ImmLabelLow(name=fact.name, offset=fact.offset)
    if isinstance(fact, ImmLabelHighExpr):
        return asm_ast.ImmLabelHigh(name=fact.name, offset=fact.offset)
    if isinstance(fact, DataExpr):
        return asm_ast.Data(name=fact.name, offset=fact.offset)
    if isinstance(fact, IndexedDataExpr):
        if not allow_indexed:
            return op
        # Substitute only if the index register's token at this
        # state still matches the fact's captured token. Identity
        # match is the dataflow's soundness guarantee — X (or Y)
        # has been continuously unchanged on every path from the
        # fact's establishment to here.
        current_token = (
            state.x_token if fact.idx_is_x else state.y_token
        )
        if current_token is None or current_token != fact.idx_token:
            return op
        idx_reg = asm_ast.X() if fact.idx_is_x else asm_ast.Y()
        return asm_ast.IndexedData(
            name=fact.name, offset=fact.offset, index=idx_reg,
        )
    return op


def _dptr_base(state: State) -> Optional[int]:
    """Return the ZP base address `N` such that DPTR currently holds
    the pair `(N, N+1)`, if the state proves it. Both `cells[DPTR]`
    and `cells[DPTR+1]` must be `ZPRef(N)` and `ZPRef(N+1)` (i.e.,
    the source pair is contiguous)."""
    lo = state.cells.get(_DPTR_LO)
    hi = state.cells.get(_DPTR_HI)
    if not (isinstance(lo, ZPRef) and isinstance(hi, ZPRef)):
        return None
    if hi.addr != lo.addr + 1:
        return None
    return lo.addr


def _rewrite_operands_for_dptr(
    instr: asm_ast.Type_instruction, base: int,
) -> asm_ast.Type_instruction:
    if isinstance(instr, asm_ast.Mov):
        new_src = _rewrite_op_for_dptr(instr.src, base)
        new_dst = _rewrite_op_for_dptr(instr.dst, base)
        if new_src is instr.src and new_dst is instr.dst:
            return instr
        return asm_ast.Mov(
            src=new_src, dst=new_dst, is_volatile=instr.is_volatile,
        )
    if isinstance(instr, (
        asm_ast.Add, asm_ast.Sub, asm_ast.And, asm_ast.Or,
    )):
        new_src = _rewrite_op_for_dptr(instr.src, base)
        if new_src is instr.src:
            return instr
        return type(instr)(src=new_src, dst=instr.dst)
    if isinstance(instr, asm_ast.Xor):
        new_s1 = _rewrite_op_for_dptr(instr.src1, base)
        new_s2 = _rewrite_op_for_dptr(instr.src2, base)
        if new_s1 is instr.src1 and new_s2 is instr.src2:
            return instr
        return asm_ast.Xor(
            src1=new_s1, src2=new_s2, dst=instr.dst,
        )
    if isinstance(instr, asm_ast.Compare):
        new_right = _rewrite_op_for_dptr(instr.right, base)
        if new_right is instr.right:
            return instr
        return asm_ast.Compare(left=instr.left, right=new_right)
    return instr


def _rewrite_op_for_dptr(
    op: asm_ast.Type_operand, base: int,
) -> asm_ast.Type_operand:
    if isinstance(op, asm_ast.Indirect):
        return asm_ast.IndirectZp(address=base, offset=op.offset)
    if isinstance(op, asm_ast.IndirectY):
        return asm_ast.IndirectZpY(address=base)
    return op
