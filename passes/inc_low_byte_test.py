"""Testing the low byte of a value just incremented by an INC chain.

The multi-byte INC peephole (`passes/inc_peephole.py`) turns an add
of 1 to a 2- or more-byte value in memory into

    INC M0
    BNE done
    INC M1
    ...
  done:

and a test of the low byte right after it, `if ((uint8_t)x == 0)` on
the value just incremented, follows as

  done:
    LDA M0
    BNE T          ; or BEQ T

The first `BNE done` is taken exactly when M0 is nonzero, and the
other paths to `done` (the fall-through and the `BNE`s after the
higher bytes) all have M0 zero. So the test's outcome is known on
every path into `done`:

- `BNE T`: the first `BNE done` goes to T instead, and the `BNE T`
  after the load is dropped, since M0 is zero wherever it's reached.
- `BEQ T`: the first `BNE done` goes past the `BEQ`, and the `BEQ`
  becomes a `JMP`.

Each path then skips a load and a branch it used to run. The flags
where the moved branch arrives are the same: `INC M0` set N and Z
from M0's new value, as the load did, and neither touches C or V.
Only A differs, since the moved branch skips the load, so A must be
dead where it arrives (`a_dead_at`). M0 is a `Data` or `ZP` byte,
the load isn't volatile, and `done` is reached only from the chain.

`get_next_code_byte2` in examples/zork/print_zstring/pages.c is

    Z_PC2.lo++;
    if ((uint8_t)Z_PC2.lo == 0) { ... }

which becomes `INC Z_PC2; BNE .if_end` per byte read.

Runs in the peephole fixed point, right after the INC peephole.
"""
from __future__ import annotations

import asm_ast
from passes.asm_liveness import a_dead_at


def apply_inc_low_byte_test(prog: asm_ast.Program) -> asm_ast.Program:
    top: list[asm_ast.Type_top_level] = []
    for tl in prog.top_level:
        if isinstance(tl, asm_ast.Function):
            tl = _rewrite_function(tl)
        top.append(tl)
    return asm_ast.Program(top_level=top)


def _rewrite_function(fn: asm_ast.Function) -> asm_ast.Function:
    instrs = list(fn.instructions)
    changed = False
    i = 0
    while i < len(instrs):
        new = _try_rewrite(instrs, i)
        if new is not None:
            instrs = new
            changed = True
        i += 1
    if not changed:
        return fn
    return asm_ast.Function(
        name=fn.name, is_global=fn.is_global,
        params=list(fn.params), instructions=instrs,
    )


def _try_rewrite(instrs: list, i: int) -> list | None:
    """If an INC chain starts at `instrs[i]` and its low byte is tested
    after its label, returns the rewritten instructions, else None."""
    first = instrs[i]
    if not (isinstance(first, asm_ast.Inc)
            and isinstance(first.dst, (asm_ast.Data, asm_ast.ZP))):
        return None
    m0 = first.dst
    br = instrs[i + 1] if i + 1 < len(instrs) else None
    if not (isinstance(br, asm_ast.Branch) and isinstance(br.cond, asm_ast.NE)):
        return None
    done = br.target
    # The rest of the chain: INC, then BNE done or the label.
    chain_branches = {i + 1}
    k = i + 2
    while True:
        if not (k < len(instrs) and isinstance(instrs[k], asm_ast.Inc)):
            return None
        k += 1
        nxt = instrs[k] if k < len(instrs) else None
        if (isinstance(nxt, asm_ast.Branch) and isinstance(nxt.cond, asm_ast.NE)
                and nxt.target == done):
            chain_branches.add(k)
            k += 1
            continue
        break
    if not (isinstance(nxt, asm_ast.Label) and nxt.name == done):
        return None
    label = k
    if any(
        isinstance(ins, (asm_ast.Branch, asm_ast.Jump)) and ins.target == done
        and n not in chain_branches
        for n, ins in enumerate(instrs)
    ):
        return None
    if label + 2 >= len(instrs):
        return None
    lda, test = instrs[label + 1], instrs[label + 2]
    if not (isinstance(lda, asm_ast.Mov) and not lda.is_volatile
            and isinstance(lda.dst, asm_ast.Reg)
            and isinstance(lda.dst.reg, asm_ast.A)
            and lda.src == m0
            and isinstance(test, asm_ast.Branch)
            and isinstance(test.cond, (asm_ast.NE, asm_ast.EQ))):
        return None
    out = list(instrs)
    if isinstance(test.cond, asm_ast.NE):
        target = _label_index(instrs, test.target)
        if target is None or not a_dead_at(instrs, target):
            return None
        out[i + 1] = asm_ast.Branch(cond=asm_ast.NE(), target=test.target)
        del out[label + 2]
    else:
        if not a_dead_at(instrs, label + 3):
            return None
        after = _fresh_label(instrs)
        out[i + 1] = asm_ast.Branch(cond=asm_ast.NE(), target=after)
        out[label + 2] = asm_ast.Jump(target=test.target)
        out.insert(label + 3, asm_ast.Label(name=after))
    if len(chain_branches) == 1:
        del out[label]
    return out


def _label_index(instrs: list, name: str) -> int | None:
    return next(
        (n for n, ins in enumerate(instrs)
         if isinstance(ins, asm_ast.Label) and ins.name == name),
        None,
    )


def _fresh_label(instrs: list) -> str:
    """A `.inc_test@N` label not yet in `instrs`."""
    used = {
        int(ins.name[len(".inc_test@"):])
        for ins in instrs
        if isinstance(ins, asm_ast.Label)
        and ins.name.startswith(".inc_test@")
        and ins.name[len(".inc_test@"):].isdigit()
    }
    n = 0
    while n in used:
        n += 1
    return f".inc_test@{n}"
