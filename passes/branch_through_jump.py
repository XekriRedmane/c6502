"""Tail-jump merging: `Branch(cond, L); …; L: Jump(T)` → `Branch(cond, T); …`.

Motivating shape (snd_delay_up's outer loop tail):

    DEX
    BNE .ssa_split@0
    RTS
    .ssa_split@0:
    JMP .loop_start

The conditional branch lands on a label whose body is just an
unconditional jump. The compiler emits this when the natural
fall-through after the branch is the function exit and the loop-
continuation target is too far for a short BNE: every iteration
pays a 3-byte 3-cycle trampoline.

Rewriting the Branch's target to the Jump's target — `BNE
.loop_start; RTS` — eliminates the trampoline on the branch path.
The Label and the now-unreachable Jump become dead and are dropped
when L has no other references.

Soundness:
  * Original: `Branch(cond, L)` either goes to L (which Jumps to T)
    or falls through. The fall-through path executes the
    instructions between the Branch and the Label.
  * Rewritten: `Branch(cond, T)` either goes directly to T or
    falls through to the same instructions. The fall-through never
    reaches L (because the Branch retargeted past it), so the
    Label/Jump pair is safe to drop when L isn't referenced
    elsewhere.
  * Branch's cond, the in-between instructions, and the Jump's
    target are all preserved. Only the trampoline indirection
    changes.

Conservative: if the in-between instructions don't terminate
(don't unconditionally exit, return, or jump elsewhere), the
fall-through path WOULD reach the Label, and dropping the
Label+Jump would change semantics. In that case we still retarget
the Branch (a strict win — direct target instead of via the
trampoline) but leave the Label+Jump in place for the fall-through
path. The `apply_dead_label_drop` pass picks them up if they end
up orphaned by a subsequent rewrite.

Where to run: in the asm-peephole fixed-point loop, after
`apply_branch_invert`. Together they collapse the two trampoline
shapes `tac_to_asm` emits for loop tails.
"""
from __future__ import annotations

import asm_ast


def apply_branch_through_jump(prog: asm_ast.Program) -> asm_ast.Program:
    """Walk every Function and retarget any `Branch(cond, L)` whose
    target Label is immediately followed by an unconditional
    `Jump(T)`. When L has only one reference (this Branch), also
    drop the orphaned Label+Jump pair."""
    new_top: list[asm_ast.Type_top_level] = []
    for tl in prog.top_level:
        if isinstance(tl, asm_ast.Function):
            new_top.append(_rewrite_function(tl))
        else:
            new_top.append(tl)
    return asm_ast.Program(top_level=new_top)


def _rewrite_function(fn: asm_ast.Function) -> asm_ast.Function:
    instrs = fn.instructions
    # In-function label names (only these are valid Branch targets;
    # Branch is a 6502 relative-branch instruction and can't reach
    # outside the function). Tail-call Jumps target external
    # function names — must NOT be promoted to Branch targets.
    in_fn_labels: set[str] = {
        instr.name for instr in instrs
        if isinstance(instr, asm_ast.Label)
    }
    # Find labels whose immediate next non-label instruction is an
    # unconditional Jump. Maps the Label's name to that Jump's
    # target. A Label followed by another Label simply chains
    # through to whatever the inner Label resolves to.
    label_to_jump_target: dict[str, str] = {}
    for i, instr in enumerate(instrs):
        if not isinstance(instr, asm_ast.Label):
            continue
        # Walk past any consecutive labels AND any self-Movs (no-op
        # `TXA;TAX` /  `Mov(R, R)` shapes that asm_emit's self-Mov
        # peephole drops at emit time but persist in the IR — common
        # leftovers from SSA-destruction parallel-copy ordering when
        # the rep merge made the copy a self-copy). The first
        # meaningful instruction after the Label is what determines
        # the trivial-jump shape.
        j = i + 1
        while j < len(instrs) and (
            isinstance(instrs[j], asm_ast.Label)
            or _is_noop_mov(instrs[j])
        ):
            j += 1
        if j >= len(instrs):
            continue
        nxt = instrs[j]
        if (
            isinstance(nxt, asm_ast.Jump)
            and nxt.target in in_fn_labels
        ):
            label_to_jump_target[instr.name] = nxt.target
    if not label_to_jump_target:
        return fn
    # Resolve chains: a label that jumps to another label that
    # jumps to T should ultimately retarget to T.
    def _resolve(target: str, depth: int = 0) -> str:
        # Bound the chain walk in case of pathological inputs;
        # 16 levels is well past anything tac_to_asm emits.
        if depth > 16:
            return target
        nxt = label_to_jump_target.get(target)
        if nxt is None or nxt == target:
            return target
        return _resolve(nxt, depth + 1)
    # Reference counts on labels so we can decide which Label+Jump
    # pairs are safe to drop after the retarget. Count Branch /
    # Jump / Phi targets; the Function name itself doesn't count
    # (the function header isn't a Label).
    ref_count: dict[str, int] = {}
    for instr in instrs:
        for ref in _label_refs(instr):
            ref_count[ref] = ref_count.get(ref, 0) + 1
    # Pass 1: retarget Branches whose target Label has a trivial
    # Jump body. Track which labels lose a reference so we can
    # decide whether to drop them.
    new_instrs: list[asm_ast.Type_instruction] = []
    drop_decremented: dict[str, int] = {}
    for instr in instrs:
        if (
            isinstance(instr, asm_ast.Branch)
            and instr.target in label_to_jump_target
        ):
            new_target = _resolve(instr.target)
            if new_target != instr.target:
                drop_decremented[instr.target] = (
                    drop_decremented.get(instr.target, 0) + 1
                )
                new_instrs.append(asm_ast.Branch(
                    cond=instr.cond, target=new_target,
                ))
                continue
        new_instrs.append(instr)
    # Pass 2: drop Label+(noop-Mov)*+Jump trampolines whose Label
    # has zero remaining references AND whose IR position is
    # unreachable by fall-through (the preceding instruction is a
    # control-flow terminator: Return / Ret / Jump / Call-to-noreturn).
    # Without the fall-through guard, a Label sitting between two
    # reachable basic blocks would be dropped along with the no-ops
    # and Jump that the preceding block falls into.
    fully_drop: set[str] = set()
    for name, dec in drop_decremented.items():
        if ref_count.get(name, 0) - dec <= 0:
            fully_drop.add(name)
    if not fully_drop:
        return asm_ast.Function(
            name=fn.name, is_global=fn.is_global,
            params=list(fn.params), instructions=new_instrs,
        )
    out: list[asm_ast.Type_instruction] = []
    i = 0
    while i < len(new_instrs):
        instr = new_instrs[i]
        if (
            isinstance(instr, asm_ast.Label)
            and instr.name in fully_drop
            and out  # there's a preceding instruction to inspect
            and _is_terminator(out[-1])
        ):
            # Skip the Label and any leading no-op Movs / orphan
            # Labels up to and including the closing Jump.
            i += 1
            while i < len(new_instrs):
                nxt = new_instrs[i]
                if isinstance(nxt, asm_ast.Jump):
                    i += 1
                    break
                if isinstance(nxt, asm_ast.Label):
                    if nxt.name in fully_drop:
                        i += 1
                        continue
                    break
                if _is_noop_mov(nxt):
                    i += 1
                    continue
                break
            continue
        out.append(instr)
        i += 1
    return asm_ast.Function(
        name=fn.name, is_global=fn.is_global,
        params=list(fn.params), instructions=out,
    )


def _is_terminator(instr: asm_ast.Type_instruction) -> bool:
    """True iff `instr` unconditionally transfers control away —
    nothing falls through to the next IR instruction."""
    return isinstance(instr, (
        asm_ast.Jump, asm_ast.Ret, asm_ast.Return,
    ))


def _is_noop_mov(instr: asm_ast.Type_instruction) -> bool:
    """A `Mov` whose src and dst structurally match (same register,
    or same Data/ZP address) — emit drops these at codegen time
    via the self-Mov peephole. We treat them as transparent for
    pattern-matching purposes too."""
    if not isinstance(instr, asm_ast.Mov):
        return False
    if instr.is_volatile:
        return False
    return instr.src == instr.dst


def _label_refs(instr: asm_ast.Type_instruction):
    """Yield every label name referenced by `instr`'s control flow
    fields."""
    if isinstance(instr, asm_ast.Branch):
        yield instr.target
    elif isinstance(instr, asm_ast.Jump):
        yield instr.target
    elif isinstance(instr, asm_ast.Phi):
        for arg in instr.args:
            yield arg.pred_label
