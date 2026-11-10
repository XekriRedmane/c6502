"""Synthesize per-function prologue / epilogue from the bare-exit
asm shape.

The `--optimize-asm` pipeline runs `tac_to_asm` in `bare_exit=True`
mode, which produces an `asm_ast.Program` where each function:

  * has NO `FunctionPrologue` prepended, and
  * ends with a bare `Return(save_a)` atom instead of the compound
    `Ret(arg_bytes, local_bytes, save_a, callee_saved_addrs)` node
    that today's pipeline patches via `replace_pseudoregisters`.

`replace_pseudoregisters.replace_program_bare_exit` keeps the body
in that shape and ALSO returns a `dict[str, FrameDims]` carrying the
per-function frame dimensions it computed (M = local bytes, N = arg
bytes, S = callee-saved bytes). This pass picks up there: it walks
each function, prepends `FunctionPrologue(N, M, callee_saved_addrs)`,
and rewrites every `Return(save_a)` → `Ret(N, M, save_a,
callee_saved_addrs)`. The result is an asm program in the regular
`asm_ast` shape that the rest of the pipeline (`asm_to_asm2`,
`asm_emit`, `sim/assembler`) consumes unchanged.

The intent is for later steps (asm-level SSA opts, byte-granular
regalloc) to slot in BEFORE this pass — they'll see only atomic
asm instructions plus the bare exit boundary, which is exactly the
"function body sans frame ceremony" view that asm-level opts want.

Step 3 baseline: this pass always inserts the full prologue /
epilogue, byte-equivalent to what `replace_program` produces today.
Step 4 will add the M=0 / S=0 collapse so functions whose state
fits entirely in ZP shrink their epilogue to a bare RTS.
"""

from __future__ import annotations

import asm_ast
from passes.asm_liveness import a_dead_at
from passes.replace_pseudoregisters import FrameDims


def synthesize_program(
    prog: asm_ast.Type_program,
    dims_by_fn: dict[str, FrameDims],
) -> asm_ast.Type_program:
    """Walk each `Function` top-level, prepend its prologue and
    rewrite each bare `Return` to a full `Ret(...)`. `StaticVariable`
    top-levels pass through unchanged."""
    match prog:
        case asm_ast.Program(top_level=top_levels):
            new_top: list[asm_ast.Type_top_level] = []
            for tl in top_levels:
                if isinstance(tl, asm_ast.StaticVariable):
                    new_top.append(tl)
                    continue
                if isinstance(tl, asm_ast.Function):
                    dims = dims_by_fn.get(tl.name)
                    if dims is None:
                        raise KeyError(
                            f"prologue_synthesis: no FrameDims for "
                            f"function {tl.name!r}",
                        )
                    new_top.append(_synthesize_function(tl, dims))
                    continue
                raise TypeError(f"unexpected top-level: {tl!r}")
            return asm_ast.Program(top_level=new_top)
        case _:
            raise TypeError(f"unexpected program node: {prog!r}")


_REG_A = asm_ast.Reg(reg=asm_ast.A())


def _split_entry_stubs(instrs, fn_name):
    """Pull the register-parameter entry stubs out of the start of
    `instrs`: the leading `Mov(Reg(A|X), Data(__zpabi_<fn>__...))`
    stores, skipping labels. Returns `(stubs, rest)` with the labels
    left in `rest`."""
    prefix = f"__zpabi_{fn_name}__"
    stubs: list = []
    rest: list = []
    i = 0
    while i < len(instrs):
        ins = instrs[i]
        if isinstance(ins, asm_ast.Label):
            rest.append(ins)
            i += 1
            continue
        if (
            isinstance(ins, asm_ast.Mov) and not ins.is_volatile
            and isinstance(ins.src, asm_ast.Reg)
            and isinstance(ins.src.reg, (asm_ast.A, asm_ast.X))
            and isinstance(ins.dst, asm_ast.Data)
            and ins.dst.name.startswith(prefix)
        ):
            stubs.append(ins)
            i += 1
            continue
        break
    rest.extend(instrs[i:])
    return stubs, rest


def _synthesize_function(
    fn: asm_ast.Function, dims: FrameDims,
) -> asm_ast.Function:
    # Validate the bare-exit invariant before we decide what to do
    # with the body: there should be no leftover Ret / FunctionPrologue
    # nodes in the input — `replace_program_bare_exit` is supposed to
    # leave the body in pure-atomic shape with bare `Return(save_a)`
    # atoms at function exits.
    for instr in fn.instructions:
        if isinstance(instr, asm_ast.Ret):
            raise AssertionError(
                f"prologue_synthesis: function {fn.name!r} already "
                f"has a Ret(...) node — bare-exit invariant broken",
            )
        if isinstance(instr, asm_ast.FunctionPrologue):
            raise AssertionError(
                f"prologue_synthesis: function {fn.name!r} already "
                f"has a FunctionPrologue node — bare-exit invariant "
                f"broken",
            )
    needs_frame = (
        dims.arg_bytes > 0
        or dims.local_bytes > 0
        or bool(dims.callee_saved_addrs)
    )
    if not needs_frame:
        # No args, no Frame-resident locals, no callee-saved ZP
        # bytes — there's nothing for SSP/FP arithmetic to do.
        # Leave the bare `Return(save_a)` atoms in place; they
        # lower straight to RTS in `asm_to_asm2`. `save_a` is
        # immaterial when there's no 16-bit SSP add to bracket
        # (RTS preserves A on its own), so we don't need to
        # rewrite the atoms in any way. This is byte-equivalent
        # to the legacy `Ret(0, 0, save_a, [])` path, which
        # `asm_to_asm2._ret` collapses to `[Return()]` anyway —
        # we're just hoisting the decision to a place where
        # later passes can see "this function has no frame" by
        # looking at the asm tree directly.
        return fn
    prologue = asm_ast.FunctionPrologue(
        arg_bytes=dims.arg_bytes,
        local_bytes=dims.local_bytes,
        callee_saved_addrs=list(dims.callee_saved_addrs),
    )
    # The prologue's SSP / FP arithmetic runs through A (and Y), so it
    # would overwrite a `register` parameter that arrives in A. The
    # entry stubs that store register parameters into their zp_abi
    # slots move in front of it; they only write those slots. If the
    # body still reads A on entry after that (a parameter kept in A
    # with its stub dropped), the prologue is wrapped in PHA / PLA.
    # X is never touched by the prologue, and no parameter arrives
    # in Y.
    body = list(fn.instructions)
    stubs, body = _split_entry_stubs(body, fn.name)
    new_instrs: list[asm_ast.Type_instruction] = list(stubs)
    a_live = not a_dead_at(body, 0)
    if a_live:
        new_instrs.append(asm_ast.Push(src=_REG_A))
    new_instrs.append(prologue)
    if a_live:
        new_instrs.append(asm_ast.Pop(dst=_REG_A))
    for instr in body:
        if isinstance(instr, asm_ast.Return):
            new_instrs.append(asm_ast.Ret(
                arg_bytes=dims.arg_bytes,
                local_bytes=dims.local_bytes,
                save_a=instr.save_a,
                save_x=instr.save_x,
                callee_saved_addrs=list(dims.callee_saved_addrs),
                save_c=instr.save_c,
            ))
            continue
        new_instrs.append(instr)
    return asm_ast.Function(
        name=fn.name,
        is_global=fn.is_global,
        params=list(fn.params),
        instructions=new_instrs,
    )
