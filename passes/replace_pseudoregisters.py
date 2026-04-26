"""Frame-layout pass: replace Pseudo operands, lay out the stack
frame, and inject prologue/epilogue dimensions.

Each function gets its own frame, partitioned per the soft-stack
convention (see README "Function stack frame layout"):

    FP+1 ... FP+M           local-byte slots (M = local_bytes)
    FP+M+1, FP+M+2          saved caller FP (the 2-byte gap)
    FP+M+3 ... FP+M+2+N     arg slots (N = arg_bytes = len(params)),
                            arg j (1-indexed) at offset M+2+j

A Pseudo can refer to one of three things, distinguished by name:

  * **a static-storage object** — a name that appears as a top-level
    `StaticVariable` in the same program. References are absolute-
    addressed (the symbol is at a fixed memory address), so the
    Pseudo lowers to a `Data(name)` operand. The asm emitter then
    renders `LDA name` / `STA name` / `ADC name` etc.
  * **a function parameter** — a name in the enclosing function's
    `params` list. The arg byte sits in the caller's frame at
    `Frame(M + 2 + j)` for the j-th param.
  * **a local temporary** — anything else. Each distinct local name
    gets a fresh `Frame(off)` slot starting at 1, in encounter order.

Per-function steps:

  1. Walk the function's instructions, classifying every Pseudo
     (static / param / local) and minting local offsets in
     encounter order. After the walk, M = (count of distinct local
     pseudos).
  2. Now that M is known, compute param offsets: param j
     (1-indexed) → `Frame(M + 2 + j)`. Params not actually
     referenced in the body still get an entry, but the entry only
     matters if some instruction touches the name.
  3. Walk the instructions a second time, replacing each Pseudo
     with its computed `Data` / `Frame` operand.
  4. Prepend `FunctionPrologue(arg_bytes=N, local_bytes=M)` and
     rewrite every `Ret(...)` to carry the same `N` and `M`. The
     emitter consumes those dimensions to lay down the prologue
     boilerplate (allocate `M+2` bytes, save caller FP, capture FP)
     and the epilogue (PHA return value, rewind SSP by `N+M+2`,
     restore caller FP, PLA, RTS).

The set of static-storage names is collected from `program.top_
level` once, before any function is rewritten — every `StaticVariable`
top-level entry contributes its name. Block-scope statics arrive
with the `@<N>.<orig>` rename from identifier_resolution; file-
scope statics keep their source spelling. Both reach the asm side
verbatim and are matched here by exact string.

`StaticVariable` top-level entries pass through unchanged — the
frame-layout pass has nothing to do for them. Instructions inside
a function with no operand fields (`AllocateStack`, `Call`, `Jump`,
…) also pass through, except `Ret` which is patched with the
function's dims.
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
        case asm_ast.Movsx(src=src, dst=dst):
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

    def __init__(
        self, params: list[str], statics: frozenset[str],
    ) -> None:
        self.params = params
        self.param_set = set(params)
        # Pseudos whose names appear in `statics` are static-storage
        # objects (file-scope variables and block-scope statics) —
        # they get `Data(name)` not a frame slot.
        self.statics = statics
        # Locals get sequential offsets in encounter order, starting
        # at 1 (FP points at the next-free byte; FP+1 is the first
        # writable slot).
        self.local_offsets: dict[str, int] = {}
        # Filled in by `finalize` once the local count is known.
        self.param_offsets: dict[str, int] = {}

    def discover(self, op: asm_ast.Type_operand) -> None:
        """First-pass: assign local offsets to non-param, non-static
        Pseudos as we see them. Params are skipped — their offsets
        depend on M (= the final count of locals), which we don't
        know until the walk finishes. Statics are skipped — they
        don't live in the frame at all."""
        if not isinstance(op, asm_ast.Pseudo):
            return
        if op.name in self.statics:
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
        """Second-pass: turn each Pseudo into its computed `Data` or
        `Frame`. Other operands pass through unchanged. The
        static-set check comes first — a name reusing a value that
        would otherwise look like a local would still resolve to
        Data here, which matches the C semantics (static-storage
        objects own their names module-wide)."""
        if not isinstance(op, asm_ast.Pseudo):
            return op
        if op.name in self.statics:
            return asm_ast.Data(name=op.name)
        if op.name in self.local_offsets:
            return asm_ast.Frame(offset=self.local_offsets[op.name])
        if op.name in self.param_offsets:
            return asm_ast.Frame(offset=self.param_offsets[op.name])
        # Unrecognized Pseudo — would mean a name not in any of the
        # three maps, which is a bug in an upstream pass. The emitter
        # would reject it later anyway, but raising here pinpoints
        # the cause.
        raise ValueError(
            f"Pseudo({op.name!r}) is neither a static, a local, nor "
            f"a declared parameter; check tac_to_asm output"
        )

    def replace_instruction(
        self, instr: asm_ast.Type_instruction,
        arg_bytes: int, local_bytes: int,
    ) -> asm_ast.Type_instruction:
        """Rewrite an instruction's operand fields, and patch `Ret`
        to carry the function's dims. The `asm_type` field on typed
        instructions rides through unchanged — it tags the
        instruction's width (Byte / DoubleByte) regardless of which
        operand kind sits in src / dst."""
        match instr:
            case asm_ast.Mov(asm_type=t, src=src, dst=dst):
                return asm_ast.Mov(
                    asm_type=t,
                    src=self.replace(src), dst=self.replace(dst),
                )
            case asm_ast.Movsx(src=src, dst=dst):
                # Sign-extend has implicit Byte→DoubleByte semantics
                # (no asm_type field). Operands still go through
                # the Pseudo replacement.
                return asm_ast.Movsx(
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
            case asm_ast.And(asm_type=t, src=src, dst=dst):
                return asm_ast.And(
                    asm_type=t,
                    src=self.replace(src), dst=self.replace(dst),
                )
            case asm_ast.Or(asm_type=t, src=src, dst=dst):
                return asm_ast.Or(
                    asm_type=t,
                    src=self.replace(src), dst=self.replace(dst),
                )
            case asm_ast.Xor(asm_type=t, src1=s1, src2=s2, dst=dst):
                return asm_ast.Xor(
                    asm_type=t,
                    src1=self.replace(s1),
                    src2=self.replace(s2),
                    dst=self.replace(dst),
                )
            case asm_ast.Inc(asm_type=t, dst=dst):
                return asm_ast.Inc(asm_type=t, dst=self.replace(dst))
            case asm_ast.Dec(asm_type=t, dst=dst):
                return asm_ast.Dec(asm_type=t, dst=self.replace(dst))
            case asm_ast.ArithmeticShiftLeft(asm_type=t, dst=dst):
                return asm_ast.ArithmeticShiftLeft(
                    asm_type=t, dst=self.replace(dst),
                )
            case asm_ast.LogicalShiftRight(asm_type=t, dst=dst):
                return asm_ast.LogicalShiftRight(
                    asm_type=t, dst=self.replace(dst),
                )
            case asm_ast.RotateLeft(asm_type=t, dst=dst):
                return asm_ast.RotateLeft(
                    asm_type=t, dst=self.replace(dst),
                )
            case asm_ast.RotateRight(asm_type=t, dst=dst):
                return asm_ast.RotateRight(
                    asm_type=t, dst=self.replace(dst),
                )
            case asm_ast.Push(asm_type=t, src=src):
                return asm_ast.Push(asm_type=t, src=self.replace(src))
            case asm_ast.Pop(asm_type=t, dst=dst):
                return asm_ast.Pop(asm_type=t, dst=self.replace(dst))
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
    fn: asm_ast.Function,
    statics: frozenset[str] = frozenset(),
) -> asm_ast.Function:
    """Lay out a single function's frame and rewrite Pseudo operands.

    `statics` is the set of static-storage names visible at the
    program top level. A Pseudo whose name is in this set lowers to
    `Data(name)` (absolute addressing); everything else becomes a
    `Frame(off)` (frame-pointer-relative). The default of an empty
    set is a unit-test convenience for callers that aren't dealing
    with statics — `replace_program` always passes the program-wide
    set.
    """
    match fn:
        case asm_ast.Function(
            name=name, is_global=is_global,
            params=params, instructions=instrs,
        ):
            r = Replacer(params=list(params), statics=statics)
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
                is_global=is_global,
                params=list(params),
                instructions=[prologue] + new_instrs,
            )
        case _:
            raise TypeError(f"unexpected function node: {fn!r}")


def replace_program(
    prog: asm_ast.Type_program,
    extra_statics: frozenset[str] = frozenset(),
) -> asm_ast.Type_program:
    """Lay out frames and lower Pseudo operands for every Function in
    `prog`.

    The static-name set is the union of:
      * every `StaticVariable` name declared at top level in `prog`,
        and
      * `extra_statics` — names of objects with static storage
        duration that don't have a `StaticVariable` definition in
        this TU. The canonical caller (`compile.py`) populates this
        from the type-checker's symbol table: any `StaticAttr` entry
        whose `initial_value` is `NoInitializer` (a block-scope or
        file-scope `extern int x;` reference) belongs here. Without
        the union, those references would look like undeclared
        Pseudos and the layout pass would mistake them for locals.

    A future cleanup could thread the SymbolTable directly and
    derive both sources from it; for now `extra_statics` is the
    minimal extra surface needed to fix the extern case without
    rewiring the pipeline.
    """
    match prog:
        case asm_ast.Program(top_level=top_levels):
            statics = frozenset(
                tl.name for tl in top_levels
                if isinstance(tl, asm_ast.StaticVariable)
            ) | extra_statics
            new_top: list[asm_ast.Type_top_level] = []
            for tl in top_levels:
                if isinstance(tl, asm_ast.StaticVariable):
                    # Nothing to lay out; pass through.
                    new_top.append(tl)
                else:
                    new_top.append(replace_function(tl, statics))
            return asm_ast.Program(top_level=new_top)
        case _:
            raise TypeError(f"unexpected program node: {prog!r}")
