"""Which direct calls are edges of the call graph.

Every pass that walks the static call graph (zp_abi eligibility, the
private pools, the parameter slots, the shared slots, the recursion
saves, and the link metadata the linker reallocates from) takes its
edges from `graph_calls`, so that they all leave out the same calls.

# A call that abandons its caller

A call to a function declared `[[noreturn]]` never comes back, so
once it's made, nothing reads the caller's parameters or locals
again, and nothing reads those of any function the caller was called
from. The caller's frame and theirs are dead from that point on. The
callee may then reuse their bytes, and it may even call the caller
again, which is the shape of an old game's `JMP RESET_GAME` from deep
inside nested subroutines. Such a call is left out of the call graph,
so it doesn't put the caller on a cycle, and the allocators don't
keep the callee's bytes apart from the caller's.

The call only abandons its caller when it passes no arguments. The
caller writes a callee's arguments into the callee's parameter slots
while its own frame is still live, reading its locals as it goes, so
those slots have to stay clear of the caller's bytes. And when the
callee is on a cycle with the caller, the slots are the bytes that
an earlier activation of the callee was reading its arguments
from, so the copy could overwrite one argument before reading it to
pass on another. A call with arguments stays an edge, and a cycle
through one is an ordinary cycle.

C keeps the caller's objects alive while the callee runs, since the
caller's block never ends. c6502 doesn't, so a pointer to a local of
the caller, or of a function it was called from, must not be used
after the call. Nothing checks this. In a cycle through such calls,
keeping those objects alive would take a new copy of each frame on
every trip around the cycle.

The hardware stack keeps the return address of every `JSR` that led
to the call, and nothing pulls them, so each trip around a cycle
leaves those bytes behind. The 6502's stack pointer wraps around
within page 1, so later trips overwrite the oldest of them, which
nothing reads again. A function that still uses the soft stack (one
with an indirect call, or any function in the unoptimized pipeline)
leaves its frame behind in the same way, and the soft stack doesn't
wrap, so a cycle through such a function runs out of memory after
enough trips.
"""
from __future__ import annotations

import tac_ast


def abandons_caller(instr: tac_ast.Type_instruction) -> bool:
    """True for a direct call to a `[[noreturn]]` function that passes
    no arguments, which isn't an edge of the call graph."""
    return (
        isinstance(instr, tac_ast.FunctionCall)
        and instr.noreturn
        and not instr.args
    )


def graph_calls(fn: tac_ast.Function) -> list[tac_ast.FunctionCall]:
    """The direct calls in `fn` that are edges of the call graph: every
    `FunctionCall` but those that abandon `fn`."""
    return [
        i for i in fn.instructions
        if isinstance(i, tac_ast.FunctionCall) and not abandons_caller(i)
    ]
