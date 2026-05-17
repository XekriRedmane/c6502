"""Tail-call peephole: `Call(name); Return(_)` → `Jump(name)`.

# Pattern

Two consecutive asm_ast instructions at the end of a function:

    Call(name)               # JSR name
    Return(save_a)           # RTS (bare-exit; no frame teardown)

→

    Jump(target=name)        # JMP name

# Soundness

The bare `Return(save_a)` atom collapses to a single RTS at lowering
(asm_to_asm2._ret on the no-frame branch ignores save_a and emits a
bare RTS). So the original pair is:

    JSR name        ; pushes return-addr R, jumps to name
    [name's body]
    RTS             ; pops R, jumps to R = the RTS just below
    RTS             ; pops the OUTER return-addr (caller of this fn),
                    ; jumps to caller

After the rewrite:

    JMP name        ; jumps to name with no stack change
    [name's body]
    RTS             ; pops the OUTER return-addr (caller of this fn),
                    ; jumps to caller

Same caller-visible behavior; one fewer push/pop pair, 1 fewer byte
(JSR+RTS = 4, JMP = 3), 7 cycles saved per call (JSR 6 + RTS 6 minus
JMP 3 minus the RTS we'd have run anyway = 9, minus the callee's RTS
which now serves both roles, net 7).

`save_a` on `Return` is irrelevant to tail-call correctness — A's
value after JMP `name` is whatever `name` last loaded into A,
identical to what JSR `name` would have left in A. The bare-RTS
lowering doesn't actually emit a PHA/PLA save (see
`asm_to_asm2._ret`'s no-frame branch), so save_a is purely an
informational tag in the bare case.

# Frame requirement

The peephole only matches `Return(_)` — NOT `Ret(arg, local, ...)`.
`Ret` carries a non-trivial frame teardown sequence (restore SSP /
FP / callee-saved bytes) that MUST run before control leaves the
function. Tail-calling past `Ret` would skip the teardown and corrupt
the soft stack. `prologue_synthesis.synthesize_prologue` is the
canonical splitter: it emits `Return(save_a)` exactly when the
function has no frame (arg_bytes == local_bytes ==
callee_saved_bytes == 0) and `Ret(...)` otherwise. The optimized
pipeline's `--bare-exit` path is where tail-call fires most often,
since every zp_abi leaf / non-recursive function ends in
`Return(_)`.

# Where to run

Inside the asm-peephole fixed-point loop. Composition: turning a
`Call` into a `Jump` makes the Call's `name` no longer participate
in the static call graph at that site — `asm_dead_store` and any
other call-aware pass that scans for `Call` atoms still sees other
calls in the same body, and the tail-called function is still
referenced by name (any other JSR or JMP target keeps its symbol
live). Position within the fixedpoint loop is not critical — this
is a 2-instruction-window pass that produces a Jump (terminator)
which doesn't itself enable any later peephole; later loop passes
are no-ops on the rewritten pair.
"""

from __future__ import annotations

import asm_ast


def apply_tail_call(prog: asm_ast.Program) -> asm_ast.Program:
    new_top: list[asm_ast.Type_top_level] = []
    for tl in prog.top_level:
        if isinstance(tl, asm_ast.Function):
            new_top.append(_rewrite_function(tl))
        else:
            new_top.append(tl)
    return asm_ast.Program(top_level=new_top)


def _rewrite_function(fn: asm_ast.Function) -> asm_ast.Function:
    instrs = fn.instructions
    out: list[asm_ast.Type_instruction] = []
    i = 0
    while i < len(instrs):
        if (i + 1 < len(instrs)
                and isinstance(instrs[i], asm_ast.Call)
                and isinstance(instrs[i + 1], asm_ast.Return)):
            out.append(asm_ast.Jump(target=instrs[i].name))
            i += 2
            continue
        out.append(instrs[i])
        i += 1
    return asm_ast.Function(
        name=fn.name, is_global=fn.is_global,
        params=list(fn.params), instructions=out,
    )
