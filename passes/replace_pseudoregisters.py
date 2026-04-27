"""Frame-layout pass: replace Pseudo operands, lay out the stack
frame, and inject prologue/epilogue dimensions.

Each function gets its own frame, partitioned per the soft-stack
convention (see README "Function stack frame layout"):

    FP+1 ... FP+M           local-byte slots (M = local_bytes)
    FP+M+1, FP+M+2          saved caller FP (the 2-byte gap)
    FP+M+3 ... FP+M+2+N     arg slots (N = arg_bytes), with each
                            param j (1-indexed) starting at offset
                            M+3 + sum-of-prior-param-sizes

A Pseudo can refer to one of three things, distinguished by name:

  * **a static-storage object** — a name that appears as a top-level
    `StaticVariable` in the same program (or in `extra_statics`,
    for extern references with no local definition). The pseudo
    lowers to a `Data(name, offset=k)` operand — absolute
    addressing, with `offset` selecting the byte of a multi-byte
    static.
  * **a function parameter** — a name in the enclosing function's
    `params` list. The arg byte sits in the caller's frame at
    `Frame(M + 2 + j_offset + k)` for the j-th param's k-th byte,
    where `j_offset` is the running sum of prior param sizes.
  * **a local temporary** — anything else. Each distinct local name
    gets `size_of(name)` consecutive `Frame(off)` slots starting at
    1, in encounter order, where `size_of` reads the symbol-table
    type for the name (Long → 2 bytes, Int / unknown → 1 byte).

Pseudo's `offset` field selects which byte of the allocated slot
this reference is — `Pseudo(name, offset=0)` is the low byte (or
the only byte of an Int), `Pseudo(name, offset=1)` is the high
byte of a Long. The replace step adds the encountered offset to
the slot's base offset.

Per-function steps:

  1. Walk the function's instructions, classifying every Pseudo
     (static / param / local) by name. Mint local *base* offsets
     (the offset of the low byte) in encounter order, advancing
     by `size_of(name)` each time. After the walk,
     M = (total bytes of distinct local pseudos).
  2. Compute param base offsets analogously, advancing by each
     param's size in `params` order, starting from M+3.
     N = total arg bytes.
  3. Walk the instructions a second time, replacing each Pseudo
     with its computed `Data(name, offset)` / `Frame(base+offset)`
     operand.
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
import c99_ast
from passes.type_checking import SymbolTable


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
        case asm_ast.LoadAddress(src=src, dst=dst):
            # `src` is a Pseudo naming the lvalue whose address we
            # want; discovering it ensures the local gets a frame
            # slot allocated even if the function body never reads
            # / writes its value (just `&x` with no other use).
            # `dst` is the 2-byte temp that receives the address.
            yield src
            yield dst


def _size_of_name(name: str, symbols: SymbolTable | None) -> int:
    """How many bytes the named pseudo occupies. Reads the symbol
    table — Int/UInt → 1, Long/ULong → 2, Float → 4, Double → 8,
    Pointer → 2 (the 6502's address width). A None symbol table or
    an absent entry both default to 1, which matches the Int-only
    world unit tests assume."""
    if symbols is None:
        return 1
    sym = symbols.get(name)
    if sym is None:
        return 1
    if isinstance(sym.type, (c99_ast.Long, c99_ast.ULong, c99_ast.Pointer)):
        return 2
    if isinstance(sym.type, c99_ast.Float):
        return 4
    if isinstance(sym.type, c99_ast.Double):
        return 8
    return 1


class Replacer:
    """Per-function offset state. Built up in the first walk
    (locals) and finalized once M is known (params)."""

    def __init__(
        self,
        params: list[str],
        statics: frozenset[str],
        symbols: SymbolTable | None = None,
    ) -> None:
        self.params = params
        self.param_set = set(params)
        self.symbols = symbols
        # Pseudos whose names appear in `statics` are static-storage
        # objects (file-scope variables and block-scope statics) —
        # they get `Data(name, offset)` not a frame slot.
        self.statics = statics
        # Locals get a *base* offset per distinct name (the offset
        # of byte 0); the byte at `Pseudo(name, offset=k)` is at
        # base+k. Encounter order; each name advances the running
        # cursor by `size_of(name)`.
        self.local_bases: dict[str, int] = {}
        self.local_total: int = 0
        # Filled in by `finalize` once the local total is known.
        self.param_bases: dict[str, int] = {}
        self.param_total: int = 0

    def discover(self, op: asm_ast.Type_operand) -> None:
        """First-pass: assign local base offsets to non-param, non-
        static Pseudos as we see them. Params are skipped — their
        offsets depend on M (= the final local-byte count), which
        we don't know until the walk finishes. Statics are skipped
        — they don't live in the frame at all."""
        if not isinstance(op, asm_ast.Pseudo):
            return
        if op.name in self.statics:
            return
        if op.name in self.param_set:
            return
        if op.name in self.local_bases:
            return
        # First sighting of this name — allocate a fresh base offset
        # and advance the cursor by the symbol's size. FP+1 is the
        # first writable slot (FP itself points at the next-free
        # byte), so the first local lands at offset 1.
        size = _size_of_name(op.name, self.symbols)
        self.local_bases[op.name] = self.local_total + 1
        self.local_total += size

    def finalize(self) -> tuple[int, int]:
        """Compute param base offsets given the now-known local
        total. Returns `(arg_bytes, local_bytes)` for the
        prologue / Ret patches.

        Param j (1-indexed) sits at Frame offset M + 2 + (sum of
        prior param sizes) + 1 — i.e., the first byte of the
        first param is M+3, the next param starts after the first
        one's bytes, etc. The 2-byte gap (M+1, M+2) holds the
        saved caller FP."""
        m = self.local_total
        cursor = m + 3  # first param's first byte
        for name in self.params:
            self.param_bases[name] = cursor
            cursor += _size_of_name(name, self.symbols)
        self.param_total = cursor - (m + 3)
        return self.param_total, m

    def replace(self, op: asm_ast.Type_operand) -> asm_ast.Type_operand:
        """Second-pass: turn each Pseudo into its computed `Data` or
        `Frame`. Other operands pass through unchanged. The static-
        set check comes first — a name reusing a value that would
        otherwise look like a local would still resolve to Data
        here, which matches the C semantics (static-storage
        objects own their names module-wide).

        The Pseudo's `offset` field selects which byte of the
        named value this reference is; we add it to the resolved
        base address — Data's own `offset`, or the local/param's
        Frame offset."""
        if not isinstance(op, asm_ast.Pseudo):
            return op
        if op.name in self.statics:
            return asm_ast.Data(name=op.name, offset=op.offset)
        if op.name in self.local_bases:
            return asm_ast.Frame(
                offset=self.local_bases[op.name] + op.offset,
            )
        if op.name in self.param_bases:
            return asm_ast.Frame(
                offset=self.param_bases[op.name] + op.offset,
            )
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
        to carry the function's dims. Instructions without operand
        fields (or only register operands) pass through unchanged."""
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
                return asm_ast.ArithmeticShiftLeft(dst=self.replace(dst))
            case asm_ast.LogicalShiftRight(dst=dst):
                return asm_ast.LogicalShiftRight(dst=self.replace(dst))
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
            case asm_ast.Ret(save_a=save_a):
                # Patch arg_bytes / local_bytes from the function's
                # totals; carry save_a through unchanged — tac_to_asm
                # set it based on whether the return value is in
                # registers (Int / Long) or in HARGS (Float / Double).
                return asm_ast.Ret(
                    arg_bytes=arg_bytes, local_bytes=local_bytes,
                    save_a=save_a,
                )
            case asm_ast.LoadAddress(src=src, dst=dst):
                # Resolve the Pseudo operands; asm_emit expands the
                # compound node into the right sequence of atomic
                # Movs based on the resolved src kind (Frame → 16-bit
                # FP-relative add; Data → ImmLabelLow/High pair).
                return asm_ast.LoadAddress(
                    src=self.replace(src), dst=self.replace(dst),
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
    symbols: SymbolTable | None = None,
) -> asm_ast.Function:
    """Lay out a single function's frame and rewrite Pseudo operands.

    `statics` is the set of static-storage names visible at the
    program top level. A Pseudo whose name is in this set lowers to
    `Data(name, offset)` (absolute addressing); everything else
    becomes a `Frame(off)`. `symbols` is the type-checker's symbol
    table, consulted to size each pseudo (Long → 2 bytes, Int →
    1 byte). Both default to empty / None for unit-test
    convenience — `replace_program` always passes the program-wide
    set and the real symbol table.
    """
    match fn:
        case asm_ast.Function(
            name=name, is_global=is_global,
            params=params, instructions=instrs,
        ):
            r = Replacer(
                params=list(params), statics=statics, symbols=symbols,
            )
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
    symbols: SymbolTable | None = None,
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

    `symbols` is the same type-checker symbol table — passed through
    so `replace_function` can size each pseudo (Long → 2 bytes,
    Int → 1 byte).
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
                    new_top.append(replace_function(tl, statics, symbols))
            return asm_ast.Program(top_level=new_top)
        case _:
            raise TypeError(f"unexpected program node: {prog!r}")
