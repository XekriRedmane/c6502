"""TAC SSA-aware "sink Var-increment past last use" pass.

# What it does

Looks for the pattern

    Binary(Add | Subtract, X, c, Y)   # def Y, last use is in-line below
    ... no use of Y ...
    <last use of X>                    # X read here
    ... rest ...

and moves the Binary instruction to immediately AFTER X's last use:

    ... no use of Y ...                # Y not yet defined
    <last use of X>                    # X read here (X's last use)
    Binary(Add | Subtract, X, c, Y)   # MOVED HERE; now X is dead before Y is defined
    ... rest ...

After the move, X's live range ends at the original last-use site
and Y's live range starts at the new position. They no longer
OVERLAP, so they don't interfere in the register allocator —
freeing a chain of `Y_i = Y_{i-1} + 1` increments to all share
one HwReg color (typically Y for monotonic counters used as array
indices).

# Why it matters

In a postfix-increment-then-use idiom

    pixels = arr[y++];

the c99 → TAC lowering produces (after SSA + copy-prop):

    Binary(Add, y_phi, 1, y.1)
    IndexedConstLoad(arr_addr, y_phi, pixels.0)

— the Add is FIRST (defines y.1), THEN the IndexedConstLoad uses
y_phi (the pre-increment value). Both y_phi and y.1 are alive at
the IndexedConstLoad position: y_phi is the `index` operand, y.1
was just defined. The interference graph adds an edge between
them, forcing different colors.

For a 7-stage unrolled chain (`pixels = arr[y++]` repeated 7
times), every `y_after_bN` interferes with `y_after_b(N-1)` AND
with `y_after_b(N+1)`. Only one of the eight values can win a
single-register HwReg color; the rest spill to ZP.

After this pass, each Add gets sunk past the corresponding
IndexedConstLoad. The chain becomes:

    IndexedConstLoad(arr_addr, y_phi, pixels.0)
    Binary(Add, y_phi, 1, y.1)
    IndexedConstLoad(arr_addr, y.1, pixels.1)
    Binary(Add, y.1, 1, y.2)
    ...

— each y.N is dead before y.(N+1) is defined. The interference
edges are gone; ALL of them can share one HwReg, and downstream
the asm-level INC peephole turns each `Add(Reg(Y), 1, Reg(Y))`
into `INY`. Net: 8 `LDA Y; CLC; ADC #1; STA <slot>` chains
collapse to 7 `INY`s.

# Algorithm

Walk each linear basic-block segment (instructions between two
control-flow boundaries — Label, Jump, Branch, conditional jumps,
function calls, etc.). For each `Binary(Add | Subtract, X, c, Y)`
where X is a `Var` (not Constant) and c is a `Constant`:

  1. Find the last use of X within this same block segment, AT or
     AFTER this instruction's position. The last use is the
     furthest-forward instruction that reads X. If no use exists
     past this position, no move is possible.
  2. Verify NO instruction between (this Binary, last_use_of_X]
     reads or writes Y. (Reads of Y: would observe an uninitialized
     value after the move; writes: SSA would prevent in any case,
     defensive check.) Skip on conflict.
  3. Verify the LAST use of X isn't the Binary instruction itself
     (i.e., X isn't BOTH src1 and dst — already disqualified by
     the SSA single-def rule, defensive check).
  4. Move the Binary to immediately AFTER the last-use instruction.

# Soundness

  * SSA invariant: every Var has exactly one def. So X isn't
    re-defined between the original Binary and any forward
    instruction; reading X at the new "last use" position gives
    the same value as before.
  * Y's def moves later. No instruction between observed Y in
    the original (Y wasn't defined yet OR was just-defined and
    not yet read — the verify-no-uses check rejects the latter
    case). After the move, Y's def precedes its first use
    correctly.
  * Side-effecting instructions (FunctionCall, IndirectCall,
    IndexedStore, Store) are NOT moved themselves — the Binary
    is the one moved, and its semantics are pure (arithmetic on
    SSA Vars). Reordering a pure Binary across other operations
    is sound provided no name dependency is violated.
  * We don't move across basic-block boundaries — the
    block-segment walk handles this implicitly.

# Where to run

In the TAC fixed-point loop, after copy_propagate (so Copy
chains have settled and the Binary's src1 is stable). Each pass
through the loop applies sinking once per chain link; iteration
to fixpoint handles long chains.

# Restriction: constant operand on src2

Only `Add(X, Constant, Y)` and `Subtract(X, Constant, Y)` are
sinkable. Reasons:

  * `Add(X, Z, Y)` where Z is also a Var: moving the Add later
    delays the read of Z. Z might be defined or modified by
    instructions between Binary and X's last use. (In SSA, Z
    can't be re-defined, but the modification side-channel for
    statics or address-taken vars is still a concern; defensively
    we restrict.)
  * Multiplicative / shift / divide / etc.: not in scope; the
    motivating case is loop-iv increments.
"""

from __future__ import annotations

import tac_ast
from passes.optimization.var_visit import defs_in, uses_in


# Instructions whose presence ends a "basic-block segment" for the
# purpose of this pass. We refuse to move Binary across any of
# them — labels and control flow change the program's reachability
# structure; calls have unknown side effects we don't reason about.
_BLOCK_TERMINATORS: tuple[type, ...] = (
    tac_ast.Label,
    tac_ast.Jump,
    tac_ast.JumpIfTrue,
    tac_ast.JumpIfFalse,
    tac_ast.JumpIfCmp,
    tac_ast.JumpIfMasked,
    tac_ast.Ret,
    tac_ast.FunctionCall,
    tac_ast.IndirectCall,
)


def sink_increments(fn: tac_ast.Function) -> tac_ast.Function:
    """Walk `fn.instructions` and move sinkable `Binary(Add |
    Subtract, X, Constant, Y)` instructions past X's last use
    within the enclosing block segment. Returns the rewritten
    function. SSA-form input expected; the move preserves the
    one-def-per-name invariant since we don't change the def
    location of any name other than Y (and Y still has only
    one def, just at a different position)."""
    instrs = list(fn.instructions)
    out: list[tac_ast.Type_instruction] = []
    i = 0
    n = len(instrs)
    while i < n:
        instr = instrs[i]
        # Identify the candidate Binary; if not a candidate, copy
        # through.
        cand = _sink_candidate(instr)
        if cand is None:
            out.append(instr)
            i += 1
            continue
        x_name, y_name = cand
        # Find X's last use within the current block segment, and
        # verify no intervening Y-read.
        offset = _find_sink_offset(instrs, i, x_name, y_name)
        if offset is None:
            # No legal move; keep the Binary in place.
            out.append(instr)
            i += 1
            continue
        # Move: emit instructions[i+1 .. i+1+offset] then the
        # Binary, advancing i past all of them.
        for k in range(1, offset + 1):
            out.append(instrs[i + k])
        out.append(instr)
        i += offset + 1
    return tac_ast.Function(
        name=fn.name,
        is_global=fn.is_global,
        params=list(fn.params),
        instructions=out,
    )


def _sink_candidate(
    instr: tac_ast.Type_instruction,
) -> tuple[str, str] | None:
    """If `instr` is a sinkable `Binary(Add | Subtract, X, c, Y)`,
    return `(X.name, Y.name)`. Otherwise return None.

    Eligibility: src1 is a `Var`, src2 is a `Constant`, dst is a
    `Var`. The op must be `Add` or `Subtract`. (Other ops aren't
    in scope; see module docstring.)"""
    if not isinstance(instr, tac_ast.Binary):
        return None
    if not isinstance(instr.op, (tac_ast.Add, tac_ast.Subtract)):
        return None
    if not isinstance(instr.src1, tac_ast.Var):
        return None
    if not isinstance(instr.src2, tac_ast.Constant):
        return None
    if not isinstance(instr.dst, tac_ast.Var):
        return None
    if instr.src1.name == instr.dst.name:
        return None
    return (instr.src1.name, instr.dst.name)


def _find_sink_offset(
    instrs: list[tac_ast.Type_instruction],
    start: int,
    x_name: str,
    y_name: str,
) -> int | None:
    """Return the offset (in instructions) past which the Binary
    at `instrs[start]` can be sunk, or None if no legal move.

    The offset is the number of instructions to walk forward FROM
    `instrs[start+1]` such that:
      * The instruction at `instrs[start + offset]` reads `x_name`
        (and is the LATEST such instruction within the block
        segment).
      * No instruction in `instrs[start + 1 .. start + offset]`
        reads or writes `y_name`.
      * No instruction in `instrs[start + 1 .. start + offset]`
        is a block terminator.

    A 0 result means the immediately-following instruction reads
    X (sink past it). A None result means no sink is possible —
    either no future X-reading instruction exists in the block,
    or the path is blocked by a Y-use or a block terminator."""
    last_x_use_offset: int | None = None
    n = len(instrs)
    j = start + 1
    while j < n:
        instr = instrs[j]
        if isinstance(instr, _BLOCK_TERMINATORS):
            break
        # A use or def of Y caps our scan window: we can't move
        # the Binary past such an instruction (Y's def has to come
        # before any read of Y; defs of Y would violate single-def
        # SSA, defensive). But we keep any X-use we've already
        # found at this point — that's still a valid sink target.
        instr_uses = {
            u.name for u in uses_in(instr) if isinstance(u, tac_ast.Var)
        }
        instr_defs = {d.name for d in defs_in(instr)}
        if y_name in instr_uses or y_name in instr_defs:
            break
        # Anti-oscillation: if `instr` is itself a sinkable Binary
        # that ALSO uses x_name, STOP scanning. Otherwise two
        # sinkable Binarys sharing an `src1` would each try to
        # sink past the other forever (each thinks the other is
        # a "future X consumer worth sinking past"). By treating
        # sinkable peers as scan boundaries, the pass is
        # idempotent: each Binary settles right before the next
        # sinkable peer (if any) or right after its last
        # consuming use (otherwise).
        if _sink_candidate(instr) is not None and x_name in instr_uses:
            break
        if x_name in instr_uses:
            last_x_use_offset = j - start
        j += 1
    return last_x_use_offset
