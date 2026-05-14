"""Promote a uchar loop counter from a ZP slot to the X register.

# Motivating case

`refresh_hit_entities`'s outer loop:

    LDA p0                      ; init from param
    STA b4
.loop_start:
    LDX b4                      ; reload at loop top
    LDA arr,X                   ; use as index
    ...
    LDX p2                      ; X clobbered
    LDA other,X
    ...
    LDX b4                      ; mid-iter reload after clobber
    ...
    JSR helper                  ; X clobbered
    DEC b4                      ; decrement memory
.loop_continue:
    BPL .loop_start             ; branch on memory's flag (already
                                 ; folded from DEC; LDA; BPL via
                                 ; dec_inc_branch_fold)

Two loop-overhead instructions can disappear if `b4`'s
canonical home moves to X:

    LDX b4 at the loop top  →  drop (X carries the value from the
                                  init's TAX OR the prior iteration's
                                  DEX)
    DEC b4 at the tail      →  DEX; STX b4 (decrement X, sync memory
                                  so mid-iter reloads still see the
                                  current value)

Net: -3 bytes / -3 cycles per iteration for the loop-top LDX
drop, +1 byte / +0 cycles for the DEC→DEX+STX swap (DEC zp = 2
bytes 5 cycles; DEX = 1 byte 2 cycles; STX zp = 2 bytes 3 cycles
— DEX+STX = 3 bytes 5 cycles vs DEC's 2 bytes 5 cycles, so +1
byte / 0 cycles). One-time +1 byte at init for the TAX. For the
12-iteration outer loop in refresh_hit_entities this saves ~24
bytes / 36 cycles per call.

# Pattern detection

A ZP/Data slot `M` is a promotion candidate iff its function-wide
use pattern matches:

  * exactly one `Mov(_, Data(M))` or `Mov(_, ZP(M))` (the init).
    Source can be Reg(A) — i.e., `STA M` after some LDA — that's
    where we'll insert the `TAX`.
  * exactly one `Dec(Data/ZP(M))` (the decrement).
  * one or more `Mov(Data/ZP(M), Reg(X))` (LDX M reloads).
  * NO other Mov src/dst, no Compare, no Inc, no Add/Sub/And/Or
    with M, no IndexedData using M as either base or index.

Plus a structural check:

  * The `Dec(M)` is immediately followed by an optional passive
    Label and then a `Branch(<flag-NZ cond>, L)` where L is the
    label of a basic block whose first non-Label instruction is
    one of the `LDX M` reloads. That LDX M is the "loop-top reload"
    we drop.

# Soundness

After the rewrite, X holds the current counter value at every
program point that previously read M:

  * After `LDA p0; STA M; TAX`: X = M.
  * After each `DEX; STX M`: X = M = old_M - 1.
  * At the loop top: X = M (preserved from the prior `DEX; STX M`
    or the init's `TAX`).

The dropped LDX M at the loop top is functionally a no-op because
X already mirrors M (from the back-edge or init paths). Every other
LDX M (mid-body reload after a clobber) is preserved, so the
function's observable register state matches the pre-rewrite
version after each clobber + reload pair.

The N/Z flags at the BPL: previously `DEC M` set N=bit7(new M);
now `DEX` sets N=bit7(new X) and `STX M` doesn't touch flags. Same
flag at the branch.

# Where to run

After `replace_pseudoregisters` (so operands are concrete Data/ZP)
and after the asm-peephole fixed-point loop (so DEC has already
been folded with its trailing LDA/BPL via `dec_inc_branch_fold`).
Before `expand_long_branches` so any branches stay short-enough."""

from __future__ import annotations

import asm_ast


_FLAG_NZ_BRANCHES: tuple[type, ...] = (
    asm_ast.EQ, asm_ast.NE, asm_ast.MI, asm_ast.PL,
)


def apply_loop_counter_to_x(prog: asm_ast.Program) -> asm_ast.Program:
    new_top: list[asm_ast.Type_top_level] = []
    for tl in prog.top_level:
        if isinstance(tl, asm_ast.Function):
            new_top.append(_rewrite_function(tl))
        else:
            new_top.append(tl)
    return asm_ast.Program(top_level=new_top)


def _operands_equal(a, b) -> bool:
    if isinstance(a, asm_ast.Data) and isinstance(b, asm_ast.Data):
        return a.name == b.name and a.offset == b.offset
    if isinstance(a, asm_ast.ZP) and isinstance(b, asm_ast.ZP):
        return a.address == b.address and a.offset == b.offset
    return False


def _operand_key(op) -> tuple | None:
    if isinstance(op, asm_ast.Data):
        return ("data", op.name, op.offset)
    if isinstance(op, asm_ast.ZP):
        return ("zp", op.address, op.offset)
    return None


def _is_reg_a(op) -> bool:
    return isinstance(op, asm_ast.Reg) and isinstance(op.reg, asm_ast.A)


def _is_reg_x(op) -> bool:
    return isinstance(op, asm_ast.Reg) and isinstance(op.reg, asm_ast.X)


def _rewrite_function(fn: asm_ast.Function) -> asm_ast.Function:
    instrs = fn.instructions
    candidate = _find_promotion_candidate(instrs)
    if candidate is None:
        return fn
    return _do_promotion(fn, candidate)


def _find_promotion_candidate(instrs):
    """Find a single ZP/Data slot M satisfying the pattern. Returns
    a dict with the indices we need to rewrite, or None.

    The dict has keys:
      'init_sta_idx' — index of the `Mov(Reg(A), Data/ZP(M))` init.
      'dec_idx'      — index of the `Dec(M)`.
      'loop_top_lda_idx' — index of the loop-top `LDX M` to drop.
      'mem_op'       — the operand object representing M, for emitting
                       new instructions."""
    # First pass: scan all uses of every memory cell, classify by
    # role. A cell M is a candidate iff its uses fit exactly:
    #   init: exactly 1
    #   dec: exactly 1
    #   ldx: >= 1
    #   any other: 0
    init_idx: dict[tuple, int] = {}
    init_count: dict[tuple, int] = {}
    dec_idx: dict[tuple, int] = {}
    dec_count: dict[tuple, int] = {}
    ldx_idx: dict[tuple, list[int]] = {}
    disqualified: set[tuple] = set()

    for i, instr in enumerate(instrs):
        for role, op in _operand_roles(instr):
            key = _operand_key(op)
            if key is None:
                continue
            if role == "init":
                init_count[key] = init_count.get(key, 0) + 1
                init_idx[key] = i
            elif role == "ldx":
                ldx_idx.setdefault(key, []).append(i)
            elif role == "dec":
                dec_count[key] = dec_count.get(key, 0) + 1
                dec_idx[key] = i
            else:
                # any other use disqualifies
                disqualified.add(key)
    for key in list(init_count) + list(dec_count) + list(ldx_idx):
        if key in disqualified:
            continue
        if init_count.get(key, 0) != 1:
            continue
        if dec_count.get(key, 0) != 1:
            continue
        if not ldx_idx.get(key):
            continue
        # Structural check: DEC at dec_idx, optional Label, Branch(PL|MI|EQ|NE).
        match = _find_loop_top_lda(
            instrs, dec_idx[key], ldx_idx[key],
        )
        if match is None:
            continue
        loop_top_lda_idx = match
        return {
            'init_sta_idx': init_idx[key],
            'dec_idx': dec_idx[key],
            'loop_top_lda_idx': loop_top_lda_idx,
            'mem_op': _mem_op_from_key(key),
        }
    return None


def _mem_op_from_key(key):
    kind = key[0]
    if kind == "data":
        return asm_ast.Data(name=key[1], offset=key[2])
    return asm_ast.ZP(address=key[1], offset=key[2])


def _find_loop_top_lda(instrs, dec_idx, ldx_indices):
    """Verify the DEC is immediately followed by an optional passive
    Label and then a flag-NZ Branch whose target chain (resolving
    bare Jump indirections) lands at a basic block whose first
    non-Label instruction is one of `ldx_indices`. Returns that
    index, or None.

    The back-edge from a do-while loop typically goes via a
    trampoline like:
        BPL .asm_ssa_split@0
      .asm_ssa_split@0:
        JMP .loop@0_start
      .loop@0_start:
        LDX M
    We follow the JMP transitively (capped to avoid cycles)."""
    # Build the set of labels that are jump/branch targets — any
    # such label between DEC and Branch indicates a `continue` (or
    # other) path that bypasses our DEC. The transform would be
    # unsound on those paths because X wouldn't get the DEX update.
    branch_targets: set[str] = set()
    for inst in instrs:
        if isinstance(inst, (asm_ast.Jump, asm_ast.Branch)):
            branch_targets.add(inst.target)
    j = dec_idx + 1
    while j < len(instrs) and isinstance(instrs[j], asm_ast.Label):
        if instrs[j].name in branch_targets:
            return None
        j += 1
    if j >= len(instrs):
        return None
    br = instrs[j]
    if not (isinstance(br, asm_ast.Branch)
            and isinstance(br.cond, _FLAG_NZ_BRANCHES)):
        return None
    target = br.target
    label_to_idx = {
        inst.name: k for k, inst in enumerate(instrs)
        if isinstance(inst, asm_ast.Label)
    }
    # Follow JMP indirections through label trampolines. Also skip
    # self-Movs (asm-emit drops them but they may appear in the
    # post-peephole IR as residue from SSA destruction).
    seen: set[str] = set()
    while target in label_to_idx and target not in seen:
        seen.add(target)
        k = label_to_idx[target] + 1
        # Skip contiguous Labels and self-Movs.
        while k < len(instrs):
            inst = instrs[k]
            if isinstance(inst, asm_ast.Label):
                k += 1
                continue
            if (isinstance(inst, asm_ast.Mov)
                    and _stable_mem_eq(inst.src, inst.dst)):
                k += 1
                continue
            break
        if k >= len(instrs):
            return None
        if isinstance(instrs[k], asm_ast.Jump):
            target = instrs[k].target
            continue
        # Found the loop body's first real instruction.
        if k in ldx_indices:
            return k
        return None
    return None


def _operand_roles(instr):
    """Yield (role, operand) pairs for each memory operand involved
    in `instr`. Roles:
      'init' — Mov(_, <mem>) that writes M (any source). Covers
               `STA M` (Reg(A) src), `LDA other; STA M` mem-to-mem
               (non-Reg src). Self-Movs (src == dst) are filtered.
      'ldx'  — Mov(<mem>, Reg(X)): LDX M.
      'dec'  — Dec(<mem>): DEC M.
      'other' — any other use (disqualifies the cell).

    Reg-Reg Movs and self-Movs are ignored. Operands that aren't
    Data/ZP are skipped — only stable-address memory cells are
    candidates."""
    if isinstance(instr, asm_ast.Mov):
        src, dst = instr.src, instr.dst
        # Self-Movs (src == dst structurally) are no-ops at emit
        # time; ignore.
        if _stable_mem_eq(src, dst):
            return
        # Mov(<mem>, Reg(X)) — LDX M.
        if _is_reg_x(dst) and not isinstance(src, asm_ast.Reg):
            yield ('ldx', src)
            return
        # Mov(<anything>, <mem>) — write to memory. Could be STA
        # (Reg(A) src) or mem-to-mem (non-Reg src). Both ok.
        if not isinstance(dst, asm_ast.Reg):
            yield ('init', dst)
            # The src side, if memory, is read — that's an 'other'
            # use of that cell. We don't yield for src here because
            # we don't want to promote the source — only the dst.
            # If src happens to be the same cell as dst, we already
            # filtered via _stable_mem_eq above.
            return
        # Anything else: dst is a register (X/Y, since A was handled
        # in the LDX branch indirectly). The src, if memory, is a
        # read of that cell — counts as 'ldy' or 'other' depending.
        if not isinstance(src, asm_ast.Reg):
            yield ('other', src)
        return
    if isinstance(instr, asm_ast.Dec):
        if not isinstance(instr.dst, asm_ast.Reg):
            yield ('dec', instr.dst)
        return
    if isinstance(instr, asm_ast.Inc):
        if not isinstance(instr.dst, asm_ast.Reg):
            yield ('other', instr.dst)
        return
    if isinstance(instr, (asm_ast.Add, asm_ast.Sub,
                          asm_ast.And, asm_ast.Or)):
        if not isinstance(instr.src, asm_ast.Reg):
            yield ('other', instr.src)
        if not isinstance(instr.dst, asm_ast.Reg):
            yield ('other', instr.dst)
        return
    if isinstance(instr, asm_ast.Compare):
        if not isinstance(instr.left, asm_ast.Reg):
            yield ('other', instr.left)
        if not isinstance(instr.right, asm_ast.Reg):
            yield ('other', instr.right)
        return


def _stable_mem_eq(a, b) -> bool:
    if isinstance(a, asm_ast.Data) and isinstance(b, asm_ast.Data):
        return a.name == b.name and a.offset == b.offset
    if isinstance(a, asm_ast.ZP) and isinstance(b, asm_ast.ZP):
        return a.address == b.address and a.offset == b.offset
    return False


def _do_promotion(fn, candidate):
    """Apply the three rewrites:
      1. After init `Mov(Reg(A), M)`: insert `Mov(Reg(A), Reg(X))`
         (TAX) so X starts the loop holding M's value.
      2. At the loop-top index: drop the `Mov(M, Reg(X))` (LDX M).
      3. At the dec index: replace `Dec(M)` with `Dec(Reg(X)); Mov(
         Reg(X), M)` (DEX; STX M) — keep X decremented AND memory
         synced for mid-body reloads."""
    instrs = fn.instructions
    out: list[asm_ast.Type_instruction] = []
    init_idx = candidate['init_sta_idx']
    loop_top_lda_idx = candidate['loop_top_lda_idx']
    dec_idx = candidate['dec_idx']
    mem_op = candidate['mem_op']
    reg_x = asm_ast.Reg(reg=asm_ast.X())
    reg_a = asm_ast.Reg(reg=asm_ast.A())

    for i, instr in enumerate(instrs):
        if i == loop_top_lda_idx:
            # Drop the loop-top LDX M.
            continue
        out.append(instr)
        if i == init_idx:
            # Append TAX right after the init STA M.
            out.append(asm_ast.Mov(src=reg_a, dst=reg_x))
        if i == dec_idx:
            # We've already appended the DEC M; replace it now.
            out.pop()
            out.append(asm_ast.Dec(dst=reg_x))
            out.append(asm_ast.Mov(src=reg_x, dst=mem_op))
    return asm_ast.Function(
        name=fn.name, is_global=fn.is_global,
        params=list(fn.params), instructions=out,
    )
