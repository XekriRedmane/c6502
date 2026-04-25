"""Frame-layout pass: replace Pseudo operands, lay out the stack
frame, and inject prologue/epilogue dimensions.

Each function gets its own frame, partitioned per the soft-stack
convention (see README "Function stack frame layout"):

    FP+1 ... FP+M           local-byte slots (M = local_bytes)
    FP+M+1, FP+M+2          saved caller FP (the 2-byte gap)
    FP+M+3 ... FP+M+2+N     arg slots (N = arg_bytes = len(params)),
                            arg j (1-indexed) at offset M+2+j

This pass walks each asm Function once, and:

  1. Identifies every distinct Pseudo name. Names that match the
     function's `params` list are *parameters* and get deferred
     offsets; everything else is a *local* and gets assigned the
     next sequential offset starting at 1.

     We assign locals in encounter order (same scheme the previous
     replace-only pass used), so M = (count of distinct local
     pseudos) once the walk finishes. The locals are densely packed
     1..M.

  2. Computes param offsets after M is known: param j (1-indexed)
     gets `Frame(M + 2 + j)` — that is, `M + 3` for the first param,
     `M + 4` for the second, and so on. Params not actually
     referenced in the body still get an entry, but the entry only
     matters if some instruction references the name.

  3. Walks the instructions a second time, replacing each Pseudo
     with its computed Frame.

  4. Prepends `FunctionPrologue(arg_bytes=N, local_bytes=M)` and
     rewrites every `Ret(...)` to carry the same `N` and `M`. The
     emitter consumes those dimensions to lay down the prologue
     boilerplate (allocate `M+2` bytes, save caller FP, capture FP)
     and the epilogue (PHA return value, rewind SSP by `N+M+2`,
     restore caller FP, PLA, RTS).

The result is a fully-laid-out Function — Pseudos all gone, frame
dims baked into the prologue/Ret. There's no separate
`allocate_stack` pass anymore: the two jobs need shared state (M
and N) and merging them is simpler than threading `M` through a
side channel.

Operands other than `Pseudo` pass through unchanged. Instructions
without operand fields (`AllocateStack`, `Call`, `Jump`, …) pass
through too, except `Ret` which is patched with the function's
dims.
"""

from __future__ import annotations

import asm_ast


def _operands_in(instr: asm_ast.Type_instruction):
    """Yield each operand-typed field of an instruction, in source
    order. Used by the first walk to discover Pseudos. Instructions
    without operand fields yield nothing."""
    match instr:
        case asm_ast.Mov(src=src, dst=dst):
            yield src
            yield dst
        case asm_ast.Add(src=src, dst=dst):
            yield src
            yield dst
        case asm_ast.Sub(src=src, dst=dst):
            yield src
            yield dst
        case asm_ast.And(src=src, dst=dst):
            yield src
            yield dst
        case asm_ast.Or(src=src, dst=dst):
            yield src
            yield dst
        case asm_ast.Xor(src1=s1, src2=s2, dst=dst):
            yield s1
            yield s2
            yield dst
        case asm_ast.Inc(dst=dst):
            yield dst
        case asm_ast.Dec(dst=dst):
            yield dst
        case asm_ast.ArithmeticShiftLeft(dst=dst):
            yield dst
        case asm_ast.LogicalShiftRight(dst=dst):
            yield dst
        case asm_ast.RotateLeft(dst=dst):
            yield dst
        case asm_ast.RotateRight(dst=dst):
            yield dst
        case asm_ast.Push(src=src):
            yield src
        case asm_ast.Pop(dst=dst):
            yield dst
        case asm_ast.Compare(left=left, right=right):
            yield left
            yield right


class Replacer:
    """Per-function offset state. Built up in the first walk
    (locals) and finalized once M is known (params)."""

    def __init__(self, params: list[str]) -> None:
        self.params = params
        self.param_set = set(params)
        # Locals get sequential offsets in encounter order, starting
        # at 1 (FP points at the next-free byte; FP+1 is the first
        # writable slot).
        self.local_offsets: dict[str, int] = {}
        # Filled in by `finalize` once the local count is known.
        self.param_offsets: dict[str, int] = {}

    def discover(self, op: asm_ast.Type_operand) -> None:
        """First-pass: assign local offsets to non-param Pseudos as
        we see them. Params are skipped — their offsets depend on M
        (= the final count of locals), which we don't know until
        the walk finishes."""
        if not isinstance(op, asm_ast.Pseudo):
            return
        if op.name in self.param_set:
            return
        if op.name not in self.local_offsets:
            self.local_offsets[op.name] = len(self.local_offsets) + 1

    def finalize(self) -> tuple[int, int]:
        """Compute param offsets given the now-known local count.
        Returns `(arg_bytes, local_bytes)` for the prologue/Ret."""
        m = len(self.local_offsets)
        n = len(self.params)
        # Param j (1-indexed) sits at Frame offset M + 2 + j —
        # i.e., M+3 for the first param, M+4 for the second, etc.
        # The 2-byte gap (M+1, M+2) holds the saved caller FP.
        for j, name in enumerate(self.params, start=1):
            self.param_offsets[name] = m + 2 + j
        return n, m

    def replace(self, op: asm_ast.Type_operand) -> asm_ast.Type_operand:
        """Second-pass: turn each Pseudo into its computed Frame.
        Other operands pass through unchanged."""
        if not isinstance(op, asm_ast.Pseudo):
            return op
        if op.name in self.local_offsets:
            return asm_ast.Frame(offset=self.local_offsets[op.name])
        if op.name in self.param_offsets:
            return asm_ast.Frame(offset=self.param_offsets[op.name])
        # Unrecognized Pseudo — would mean a name not in either map,
        # which is a bug in an upstream pass. The emitter would
        # reject it later anyway, but raising here pinpoints the
        # cause.
        raise ValueError(
            f"Pseudo({op.name!r}) is neither a local nor a "
            "declared parameter; check tac_to_asm output"
        )

    def replace_instruction(
        self, instr: asm_ast.Type_instruction,
        arg_bytes: int, local_bytes: int,
    ) -> asm_ast.Type_instruction:
        """Rewrite an instruction's operand fields, and patch `Ret`
        to carry the function's dims."""
        match instr:
            case asm_ast.Mov(src=src, dst=dst):
                return asm_ast.Mov(
                    src=self.replace(src), dst=self.replace(dst),
                )
            case asm_ast.Add(src=src, dst=dst):
                return asm_ast.Add(
                    src=self.replace(src), dst=self.replace(dst),
                )
            case asm_ast.Sub(src=src, dst=dst):
                return asm_ast.Sub(
                    src=self.replace(src), dst=self.replace(dst),
                )
            case asm_ast.And(src=src, dst=dst):
                return asm_ast.And(
                    src=self.replace(src), dst=self.replace(dst),
                )
            case asm_ast.Or(src=src, dst=dst):
                return asm_ast.Or(
                    src=self.replace(src), dst=self.replace(dst),
                )
            case asm_ast.Xor(src1=s1, src2=s2, dst=dst):
                return asm_ast.Xor(
                    src1=self.replace(s1),
                    src2=self.replace(s2),
                    dst=self.replace(dst),
                )
            case asm_ast.Inc(dst=dst):
                return asm_ast.Inc(dst=self.replace(dst))
            case asm_ast.Dec(dst=dst):
                return asm_ast.Dec(dst=self.replace(dst))
            case asm_ast.ArithmeticShiftLeft(dst=dst):
                return asm_ast.ArithmeticShiftLeft(
                    dst=self.replace(dst),
                )
            case asm_ast.LogicalShiftRight(dst=dst):
                return asm_ast.LogicalShiftRight(
                    dst=self.replace(dst),
                )
            case asm_ast.RotateLeft(dst=dst):
                return asm_ast.RotateLeft(dst=self.replace(dst))
            case asm_ast.RotateRight(dst=dst):
                return asm_ast.RotateRight(dst=self.replace(dst))
            case asm_ast.Push(src=src):
                return asm_ast.Push(src=self.replace(src))
            case asm_ast.Pop(dst=dst):
                return asm_ast.Pop(dst=self.replace(dst))
            case asm_ast.Compare(left=left, right=right):
                return asm_ast.Compare(
                    left=self.replace(left),
                    right=self.replace(right),
                )
            case asm_ast.Ret():
                return asm_ast.Ret(
                    arg_bytes=arg_bytes, local_bytes=local_bytes,
                )
            case _:
                # AllocateStack / Call / Jump / Branch / Label /
                # ClearCarry / SetCarry / FunctionPrologue all have
                # no operand fields (or no Pseudo-typed ones), so
                # they pass through unchanged.
                return instr


def replace_function(
    fn: asm_ast.Type_function_definition,
) -> asm_ast.Type_function_definition:
    match fn:
        case asm_ast.Function(name=name, params=params, instructions=instrs):
            r = Replacer(params=list(params))
            # Pass 1: discover all local Pseudos in encounter order.
            for instr in instrs:
                for op in _operands_in(instr):
                    r.discover(op)
            # Compute final dims now that all locals are accounted
            # for; populate param offsets.
            arg_bytes, local_bytes = r.finalize()
            # Pass 2: replace operands and patch Ret instructions
            # with the function's dims.
            new_instrs = [
                r.replace_instruction(i, arg_bytes, local_bytes)
                for i in instrs
            ]
            # Prepend the prologue. The emitter takes
            # arg_bytes+local_bytes==0 as a special case (no FP
            # setup needed when there are no args or locals).
            prologue = asm_ast.FunctionPrologue(
                arg_bytes=arg_bytes, local_bytes=local_bytes,
            )
            return asm_ast.Function(
                name=name,
                params=list(params),
                instructions=[prologue] + new_instrs,
            )
        case _:
            raise TypeError(f"unexpected function node: {fn!r}")


def replace_program(
    prog: asm_ast.Type_program,
) -> asm_ast.Type_program:
    match prog:
        case asm_ast.Program(function_definition=fns):
            return asm_ast.Program(function_definition=[
                replace_function(fn) for fn in fns
            ])
        case _:
            raise TypeError(f"unexpected program node: {prog!r}")
