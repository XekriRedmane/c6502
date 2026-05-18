"""Frame-layout pass: replace Pseudo operands, lay out the stack
frame, and inject prologue/epilogue dimensions.

Each function gets its own frame, partitioned per the soft-stack
convention (see README "Function stack frame layout"):

    FP+1 ... FP+M           local-byte slots (M = local_bytes)
    FP+M+1, FP+M+2          saved caller FP (the 2-byte gap)
    FP+M+3 ... FP+M+2+N     arg slots (N = arg_bytes), with each
                            param j (1-indexed) starting at offset
                            M+3 + sum-of-prior-param-sizes

A Pseudo can refer to one of four things, distinguished by name:

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
    Params always lower to `Frame` regardless of any color the
    register allocator may have happened to assign — the calling
    convention writes them to specific soft-stack offsets, so
    they're inherently frame-resident on entry. (A future
    optimization could copy-into-ZP at the prologue for hot params;
    out of scope for now.)
  * **a colored local** — a Pseudo whose name appears in the
    optional `Coloring.assignments` map. Lowers to
    `ZP(address=base, offset=k)` where `base` is the regalloc-
    assigned ZP byte and `k` is the Pseudo's `offset`. Colored
    names skip frame allocation entirely (they don't contribute to
    `local_bytes`), so the prologue's M is just the spill / never-
    colored / address-taken locals.
  * **an ordinary (uncolored / spilled) local** — anything else.
    Each distinct local name gets `size_of(name)` consecutive
    `Frame(off)` slots starting at 1, in encounter order, where
    `size_of` reads the symbol-table type for the name (Long → 4
    bytes, Int → 2 bytes, etc.). Spilled values from regalloc
    reach this path automatically because they're absent from the
    coloring's assignments.

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

from dataclasses import dataclass, field

import asm_ast
import c99_ast
from passes.type_checking import SymbolTable


@dataclass
class FrameDims:
    """Per-function frame metrics computed by the replace pass.
    Returned alongside the program in `--optimize-asm` mode so the
    downstream synthesis pass can decide on prologue / epilogue
    shape without re-running the layout walk. The fields mirror
    `FunctionPrologue` / `Ret`'s payload so synthesis is a direct
    rewrite."""
    arg_bytes: int
    local_bytes: int
    callee_saved_addrs: list[int] = field(default_factory=list)


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
        case asm_ast.Phi():
            raise TypeError(
                "replace_pseudoregisters: Phi node leaked past "
                "SSA destruction",
            )
            yield dst


def size_of_name(name: str, symbols: SymbolTable | None, types=None) -> int:
    """How many bytes the named pseudo occupies. Reads the symbol
    table — Char/SChar/UChar → 1, Int/UInt → 2, Long/ULong → 4,
    LongLong/ULongLong → 8, Float → 4, Double → 8, Pointer → 2
    (the 6502's address width), Array → recursive element size ×
    count, Structure/Union → layout size (from TypeTable). A None
    symbol table or an absent entry both default to 1, which
    matches the unit-test backstop for synthetic ASTs."""
    if symbols is None:
        return 1
    sym = symbols.get(name)
    if sym is None:
        return 1
    return sizeof(sym.type, types)


def sizeof(t: c99_ast.Type_data_type, types=None) -> int:
    """Bytes occupied by a value of type `t`. Recursive for Array.
    `Const` and `Volatile` are transparent."""
    if isinstance(t, (c99_ast.Const, c99_ast.Volatile)):
        return sizeof(t.referenced_type, types)
    if isinstance(t, (c99_ast.Char, c99_ast.SChar, c99_ast.UChar)):
        return 1
    if isinstance(t, (c99_ast.Int, c99_ast.UInt, c99_ast.Pointer)):
        return 2
    if isinstance(t, (c99_ast.Long, c99_ast.ULong, c99_ast.Float)):
        return 4
    if isinstance(t, (c99_ast.LongLong, c99_ast.ULongLong, c99_ast.Double)):
        return 8
    if isinstance(t, c99_ast.Array):
        return sizeof(t.element_type, types) * t.size
    if isinstance(t, (c99_ast.Structure, c99_ast.Union)):
        if types is None:
            return 1
        layout = types.get(t.tag)
        if layout is None or not layout.complete:
            return 1
        return layout.size
    return 1


class Replacer:
    """Per-function offset state. Built up in the first walk
    (locals) and finalized once M is known (params)."""

    def __init__(
        self,
        params: list[str],
        statics: frozenset[str],
        symbols: SymbolTable | None = None,
        types=None,
        coloring=None,
        param_layout=None,  # ParamLayout from passes.abi_selection
        private_pool_addrs: frozenset[int] = frozenset(),
        address_taken_zp: dict[str, int] | None = None,
        address_taken_symbols: dict[str, str] | None = None,
    ) -> None:
        self.params = params
        self.param_set = set(params)
        self.symbols = symbols
        self.types = types
        # Pseudos whose names appear in `statics` are static-storage
        # objects (file-scope variables and block-scope statics) —
        # they get `Data(name, offset)` not a frame slot.
        self.statics = statics
        # Optional Coloring from register allocation. A name in
        # `coloring.assignments` lowers to ZP and skips frame
        # allocation; everything else (spilled, uncolored, address-
        # taken) flows through the existing Frame path.
        self.coloring = coloring
        # Optional `ParamLayout` for the enclosing function. When
        # the layout is a `ZpLayout`, each parameter's bytes live
        # at fixed ZP addresses on entry — the caller wrote them
        # there before JSR, per the ZP-passing calling convention.
        # `replace` resolves param Pseudos to those ZP addresses
        # instead of the soft-stack Frame slots. `param_total`
        # stays at 0 in that case (no AllocateStack on the caller's
        # side to mirror). When the layout is `SoftStackLayout` or
        # `None`, the existing Frame-based param resolution applies.
        self.param_layout = param_layout
        # Flat byte index of each param's first byte, by param
        # name. Indexes into the `ZpLayout.addrs` list when the
        # layout is ZpLayout. Computed in `finalize`.
        self.param_flat_offsets: dict[str, int] = {}
        # ZP byte addresses that belong to this function's private
        # pool from `passes.zp_local_allocation`. These are
        # by-construction safe across calls (call-graph-disjoint
        # allocation guarantees no coexisting function touches
        # them), so they're EXCLUDED from `callee_saved_addrs`
        # even when they happen to fall in the default Pool's
        # callee_saved() range. For functions without a private
        # pool, this set is empty and the existing logic applies.
        self.private_pool_addrs = private_pool_addrs
        # Address-taken locals routed to ZP via the function's
        # private pool. Map: pseudo_name → first byte's ZP address.
        # Pseudos in this map get resolved to `Data(slot_symbol,
        # offset)` instead of falling through to a Frame slot. Names
        # NOT in this map (because no contiguous run was available)
        # still get a Frame slot via the existing path.
        self.address_taken_zp: dict[str, int] = dict(
            address_taken_zp or {}
        )
        self.address_taken_symbols: dict[str, str] = dict(
            address_taken_symbols or {}
        )
        # Compute the set of callee-saved ZP byte addresses this
        # function uses. Each byte gets a slot at the bottom of the
        # frame (FP+1..FP+S), so locals start at offset S+1. The
        # prologue saves each byte to its slot; the epilogue
        # restores. See `callee_saved_addrs`.
        self.callee_saved_addrs: list[int] = (
            self._compute_callee_saved_addrs()
        )
        s = len(self.callee_saved_addrs)
        # Locals get a *base* offset per distinct name (the offset
        # of byte 0); the byte at `Pseudo(name, offset=k)` is at
        # base+k. Encounter order; each name advances the running
        # cursor by `size_of(name)`. The first S frame bytes are
        # reserved for callee-saves, so locals start after them
        # (cursor begins at S, first local lands at offset S+1).
        self.local_bases: dict[str, int] = {}
        self.local_total: int = s
        # Filled in by `finalize` once the local total is known.
        self.param_bases: dict[str, int] = {}
        self.param_total: int = 0

    def _compute_callee_saved_addrs(self) -> list[int]:
        """Set of zero-page byte addresses this function uses from
        the callee-saved pool. Each byte needs to be saved to the
        frame in the prologue and restored in the epilogue so the
        caller can rely on the slot's contents surviving the call.

        For each colored Var, we enumerate every byte it occupies
        (`[base, base+width)`) and include the ones that fall in
        `coloring.pool.callee_saved()`. Addresses in this function's
        `private_pool_addrs` are excluded — `zp_local_allocation`
        guarantees no coexisting function touches them, so the
        save/restore would be wasted even when the address lands in
        the conventional callee-saved range. Returns sorted
        ascending so the prologue / epilogue emit in deterministic
        order."""
        if self.coloring is None or not self.coloring.assignments:
            return []
        callee_range = self.coloring.pool.callee_saved()
        used: set[int] = set()
        for name, base in self.coloring.assignments.items():
            width = size_of_name(name, self.symbols, self.types)
            for k in range(width):
                byte_addr = base + k
                if byte_addr in callee_range and byte_addr not in self.private_pool_addrs:
                    used.add(byte_addr)
        return sorted(used)

    def _is_colored(self, name: str) -> bool:
        """A name is colored iff a Coloring was supplied AND that
        name appears in its `assignments` map. Spilled names
        (`coloring.spilled`) are NOT colored — they fall through to
        the frame path, which is exactly the right behavior."""
        return (
            self.coloring is not None
            and name in self.coloring.assignments
        )

    def discover(self, op: asm_ast.Type_operand) -> None:
        """First-pass: assign local base offsets to non-param, non-
        static, non-colored Pseudos as we see them. Params are
        skipped — their offsets depend on M (= the final local-byte
        count), which we don't know until the walk finishes.
        Statics are skipped — they don't live in the frame at all.
        Colored Pseudos are skipped — they live in ZP, not frame."""
        if not isinstance(op, asm_ast.Pseudo):
            return
        if op.name in self.statics:
            return
        if op.name in self.param_set:
            return
        if self._is_colored(op.name):
            return
        if op.name in self.address_taken_zp:
            # Address-taken local that's been routed to ZP via the
            # private pool. No Frame slot needed.
            return
        if op.name in self.local_bases:
            return
        # First sighting of this name — allocate a fresh base offset
        # and advance the cursor by the symbol's size. FP+1 is the
        # first writable slot (FP itself points at the next-free
        # byte), so the first local lands at offset 1.
        size = size_of_name(op.name, self.symbols, self.types)
        self.local_bases[op.name] = self.local_total + 1
        self.local_total += size

    def finalize(self) -> tuple[int, int]:
        """Compute param base offsets given the now-known local
        total. Returns `(arg_bytes, local_bytes)` for the
        prologue / Ret patches.

        SoftStackLayout (the default): Param j (1-indexed) sits at
        Frame offset M + 2 + (sum of prior param sizes) + 1 —
        i.e., the first byte of the first param is M+3, the next
        param starts after the first one's bytes, etc. The 2-byte
        gap (M+1, M+2) holds the saved caller FP. arg_bytes = sum
        of param byte sizes.

        ZpLayout: params live at fixed ZP addresses (from the
        layout's `addrs`); `param_bases` stays empty (no Frame
        slots reserved); `arg_bytes` = 0 (no soft-stack args). We
        record each param's flat byte index into `addrs` so
        `replace` can resolve `Pseudo(p, k)` to
        `ZP(addrs[flat_idx + k], 0)`."""
        from passes.abi_selection import ZpLayout
        m = self.local_total
        if isinstance(self.param_layout, ZpLayout):
            flat = 0
            for name in self.params:
                self.param_flat_offsets[name] = flat
                flat += size_of_name(name, self.symbols, self.types)
            self.param_total = 0
            return 0, m
        cursor = m + 3  # first param's first byte
        for name in self.params:
            self.param_bases[name] = cursor
            cursor += size_of_name(name, self.symbols, self.types)
        self.param_total = cursor - (m + 3)
        return self.param_total, m

    def replace(self, op: asm_ast.Type_operand) -> asm_ast.Type_operand:
        """Second-pass: turn each Pseudo into its computed `Data`,
        `ZP`, or `Frame`. Other operands pass through unchanged.

        Decision order:
          1. static → `Data(name, offset)` (absolute, link-time
             address). Highest priority: static-storage objects own
             their names module-wide.
          2. param → `Frame(...)` from `param_bases`. Forced even if
             the regalloc happened to color the param's name —
             params arrive on the soft stack via the calling
             convention, so they must be at their declared Frame
             offsets on entry.
          3. colored local → `ZP(address, offset)` from
             `coloring.assignments`. Skips frame allocation; lives
             in zero-page.
          4. local (uncolored / spilled / address-taken) →
             `Frame(...)` from `local_bases`.

        The Pseudo's `offset` field selects which byte of the
        named value this reference is; we add it to the resolved
        base address — Data's `offset`, ZP's `offset`, or the
        local/param's Frame offset."""
        if not isinstance(op, asm_ast.Pseudo):
            return op
        if op.name in self.statics:
            return asm_ast.Data(name=op.name, offset=op.offset)
        if op.name in self.param_flat_offsets:
            # ZP-ABI param: bytes live at fixed addresses per the
            # function's `ZpLayout`. Resolve to a symbolic `Data`
            # operand whose name is the slot symbol; the asm-emit
            # stage prints `<sym> EQU $<addr>` directives at the
            # top of the output, and dasm picks zp vs. absolute
            # addressing from the resolved value. Spill above $FF
            # therefore needs no IR change.
            from passes.abi_selection import ZpLayout
            assert isinstance(self.param_layout, ZpLayout)
            flat_idx = self.param_flat_offsets[op.name] + op.offset
            return asm_ast.Data(
                name=self.param_layout.slot_symbols[flat_idx],
                offset=0,
            )
        if op.name in self.param_bases:
            # SoftStack-ABI param: even if the regalloc assigned a
            # color to a param's name, the calling convention demands
            # the param sit at its Frame offset on entry.
            return asm_ast.Frame(
                offset=self.param_bases[op.name] + op.offset,
            )
        if self._is_colored(op.name):
            base = self.coloring.assignments[op.name]
            return asm_ast.ZP(address=base, offset=op.offset)
        if op.name in self.address_taken_zp:
            # Address-taken local routed into the function's private
            # ZP pool. Resolve to the slot-symbol `Data` form so the
            # asm-emit stage can pick zp vs. absolute addressing from
            # the EQU'd address, and so `LoadAddress(src=Data(slot))`
            # lowers to a 2-byte `LDA #<slot; STA dst.lo; LDA #>slot;
            # STA dst.hi` immediate pair instead of the 6-byte FP+off
            # runtime add.
            slot_name = self.address_taken_symbols.get(op.name)
            if slot_name is not None:
                return asm_ast.Data(
                    name=slot_name, offset=op.offset,
                )
            return asm_ast.ZP(
                address=self.address_taken_zp[op.name],
                offset=op.offset,
            )
        if op.name in self.local_bases:
            return asm_ast.Frame(
                offset=self.local_bases[op.name] + op.offset,
            )
        # Unrecognized Pseudo — would mean a name not in any of the
        # four classifications, which is a bug in an upstream pass.
        # The emitter would reject it later anyway, but raising here
        # pinpoints the cause.
        raise ValueError(
            f"Pseudo({op.name!r}) is neither a static, a colored "
            f"local, an ordinary local, nor a declared parameter; "
            f"check tac_to_asm output"
        )

    def replace_instruction(
        self, instr: asm_ast.Type_instruction,
        arg_bytes: int, local_bytes: int,
    ) -> asm_ast.Type_instruction:
        """Rewrite an instruction's operand fields, and patch `Ret`
        to carry the function's dims. Instructions without operand
        fields (or only register operands) pass through unchanged."""
        match instr:
            case asm_ast.Mov(src=src, dst=dst, is_volatile=v):
                return asm_ast.Mov(
                    src=self.replace(src), dst=self.replace(dst),
                    is_volatile=v,
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
                # registers (Int / Long) or in HARGS (LongLong /
                # Float / Double). Pass through the function's
                # callee-saved address list so the epilogue can
                # restore them before SSP/FP teardown.
                return asm_ast.Ret(
                    arg_bytes=arg_bytes, local_bytes=local_bytes,
                    save_a=save_a,
                    callee_saved_addrs=list(self.callee_saved_addrs),
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
    types=None,
    coloring=None,
) -> asm_ast.Function:
    """Lay out a single function's frame and rewrite Pseudo operands.

    `statics` is the set of static-storage names visible at the
    program top level. A Pseudo whose name is in this set lowers to
    `Data(name, offset)` (absolute addressing). `symbols` is the
    type-checker's symbol table, consulted to size each pseudo
    (Long → 4 bytes, Int → 2 bytes, etc.). `coloring` is an optional
    `passes.optimization.register_allocation.Coloring`; when supplied,
    each Pseudo whose name is in `coloring.assignments` lowers to
    `ZP(address, offset)` and skips frame allocation entirely.
    Spilled / never-colored / address-taken Pseudos continue to use
    the existing Frame path. All defaults match the pre-regalloc
    behavior (no ZP operands produced, all Pseudos to Frame /
    Data).
    """
    fn_out, _dims = _replace_function_impl(
        fn, statics=statics, symbols=symbols, types=types,
        coloring=coloring, bare_exit=False,
    )
    return fn_out


def replace_function_bare_exit(
    fn: asm_ast.Function,
    statics: frozenset[str] = frozenset(),
    symbols: SymbolTable | None = None,
    types=None,
    coloring=None,
    param_layout=None,
    private_pool_addrs: frozenset[int] = frozenset(),
    address_taken_zp: dict[str, int] | None = None,
    address_taken_symbols: dict[str, str] | None = None,
) -> tuple[asm_ast.Function, FrameDims]:
    """`--optimize-asm` variant: same Pseudo / Frame / ZP rewrite,
    but skips the `FunctionPrologue` prepend and leaves each bare
    `Return(save_a)` atom unpatched. Returns the function alongside
    the computed `FrameDims` so the synthesis pass can decide on the
    eventual prologue / epilogue shape based on what actually
    spilled.

    `param_layout` is the function's own `ParamLayout` from
    `passes.abi_selection`. When `ZpLayout`, parameter Pseudos
    resolve to fixed ZP addresses on entry (caller wrote the bytes
    before JSR per the ZP-passing convention) and `arg_bytes`
    stays at 0. When `SoftStackLayout` or `None`, the existing
    Frame-based param resolution applies.

    `private_pool_addrs` is the set of byte addresses returned by
    `passes.zp_local_allocation.allocate_function_locals` for
    this function. Addresses in this set are excluded from the
    `callee_saved_addrs` computation regardless of where they
    land — the private-pool allocator already guarantees no
    coexisting function touches them, so save/restore would be
    pure waste."""
    return _replace_function_impl(
        fn, statics=statics, symbols=symbols, types=types,
        coloring=coloring, bare_exit=True,
        param_layout=param_layout,
        private_pool_addrs=private_pool_addrs,
        address_taken_zp=address_taken_zp,
        address_taken_symbols=address_taken_symbols,
    )


def _replace_function_impl(
    fn: asm_ast.Function,
    statics: frozenset[str],
    symbols: SymbolTable | None,
    types,
    coloring,
    bare_exit: bool,
    param_layout=None,
    private_pool_addrs: frozenset[int] = frozenset(),
    address_taken_zp: dict[str, int] | None = None,
    address_taken_symbols: dict[str, str] | None = None,
) -> tuple[asm_ast.Function, FrameDims]:
    match fn:
        case asm_ast.Function(
            name=name, is_global=is_global,
            params=params, instructions=instrs,
        ):
            r = Replacer(
                params=list(params), statics=statics,
                symbols=symbols, types=types, coloring=coloring,
                param_layout=param_layout,
                private_pool_addrs=private_pool_addrs,
                address_taken_zp=address_taken_zp,
                address_taken_symbols=address_taken_symbols,
            )
            # Pass 1: discover all local Pseudos in encounter order.
            for instr in instrs:
                for op in _operands_in(instr):
                    r.discover(op)
            # Compute final dims now that all locals are accounted
            # for; populate param offsets.
            arg_bytes, local_bytes = r.finalize()
            dims = FrameDims(
                arg_bytes=arg_bytes,
                local_bytes=local_bytes,
                callee_saved_addrs=list(r.callee_saved_addrs),
            )
            # Pass 2: replace operands. In bare_exit mode the body
            # ends with `Return(save_a)` (no payload to patch); in
            # normal mode each `Ret` carries the function's dims.
            new_instrs = [
                r.replace_instruction(i, arg_bytes, local_bytes)
                for i in instrs
            ]
            if bare_exit:
                # Leave prologue insertion to the synthesis pass.
                return (
                    asm_ast.Function(
                        name=name,
                        is_global=is_global,
                        params=list(params),
                        instructions=new_instrs,
                    ),
                    dims,
                )
            # Prepend the prologue. The emitter takes
            # arg_bytes+local_bytes==0 as a special case (no FP
            # setup needed when there are no args or locals).
            # Pass the callee-saved address list so the prologue
            # can save each ZP byte to its frame slot after FP
            # setup (and the matching epilogue restores them).
            prologue = asm_ast.FunctionPrologue(
                arg_bytes=arg_bytes, local_bytes=local_bytes,
                callee_saved_addrs=list(r.callee_saved_addrs),
            )
            return (
                asm_ast.Function(
                    name=name,
                    is_global=is_global,
                    params=list(params),
                    instructions=[prologue] + new_instrs,
                ),
                dims,
            )
        case _:
            raise TypeError(f"unexpected function node: {fn!r}")


def replace_program(
    prog: asm_ast.Type_program,
    extra_statics: frozenset[str] = frozenset(),
    symbols: SymbolTable | None = None,
    types=None,
    colorings=None,
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
    so `replace_function` can size each pseudo (Long → 4 bytes,
    Int → 2 bytes, etc.).

    `colorings` is an optional `dict[str, Coloring]` mapping each
    function's name to its register-allocator-produced coloring.
    `None` (the default) reproduces today's all-Frame behavior;
    a missing function-name key in the dict has the same effect
    on a per-function basis.
    """
    match prog:
        case asm_ast.Program(top_level=top_levels):
            # Top-level static-storage names: every StaticVariable
            # (file-scope and block-scope `static` variables) plus
            # every Function (function names also resolve to a
            # link-time-known absolute address — needed so an
            # `&foo` GetAddress on a function name reaches the
            # `Data(name)` lowering path instead of failing the
            # "Pseudo is neither static, local, nor param" guard).
            statics = frozenset(
                tl.name for tl in top_levels
                if isinstance(tl, (asm_ast.StaticVariable, asm_ast.Function))
            ) | extra_statics
            new_top: list[asm_ast.Type_top_level] = []
            for tl in top_levels:
                if isinstance(tl, asm_ast.StaticVariable):
                    # Nothing to lay out; pass through.
                    new_top.append(tl)
                else:
                    coloring = (
                        colorings.get(tl.name) if colorings is not None else None
                    )
                    new_top.append(replace_function(
                        tl, statics, symbols, types, coloring=coloring,
                    ))
            return asm_ast.Program(top_level=new_top)
        case _:
            raise TypeError(f"unexpected program node: {prog!r}")


def replace_program_bare_exit(
    prog: asm_ast.Type_program,
    extra_statics: frozenset[str] = frozenset(),
    symbols: SymbolTable | None = None,
    types=None,
    colorings=None,
    param_layouts=None,
    local_pools: dict[str, list[int]] | None = None,
    address_taken_assignments: dict[str, dict[str, int]] | None = None,
    address_taken_symbols: dict[str, dict[str, str]] | None = None,
) -> tuple[asm_ast.Type_program, dict[str, FrameDims]]:
    """`--optimize-asm` variant of `replace_program`. Produces an asm
    program whose functions still end with bare `Return(save_a)`
    atoms (no `FunctionPrologue` prepended, no `Ret` payload patches)
    and a per-function `dict[name, FrameDims]` mapping each function
    to the metrics the synthesis pass will need to materialize the
    eventual prologue / epilogue.

    Same arguments as `replace_program`; same coloring semantics
    (uncolored / spilled / address-taken Pseudos still get Frame
    slots, computed exactly as in the prologue path).

    `param_layouts` is an optional `dict[name, ParamLayout]` from
    `passes.abi_selection.select_abi`. When supplied, each
    function's ParamLayout drives how its own params are
    resolved: a `ZpLayout` function's params lower to ZP operands
    (caller wrote the bytes there pre-JSR); the resulting
    `FrameDims.arg_bytes` is 0. Without this dict, every function
    is treated as `SoftStackLayout` (existing behavior)."""
    match prog:
        case asm_ast.Program(top_level=top_levels):
            statics = frozenset(
                tl.name for tl in top_levels
                if isinstance(tl, (asm_ast.StaticVariable, asm_ast.Function))
            ) | extra_statics
            new_top: list[asm_ast.Type_top_level] = []
            dims_by_fn: dict[str, FrameDims] = {}
            for tl in top_levels:
                if isinstance(tl, asm_ast.StaticVariable):
                    new_top.append(tl)
                else:
                    coloring = (
                        colorings.get(tl.name) if colorings is not None else None
                    )
                    layout = (
                        param_layouts.get(tl.name)
                        if param_layouts is not None else None
                    )
                    private = (
                        frozenset(local_pools.get(tl.name, ()))
                        if local_pools is not None else frozenset()
                    )
                    addr_taken_zp = (
                        address_taken_assignments.get(tl.name, {})
                        if address_taken_assignments is not None
                        else {}
                    )
                    addr_taken_syms = (
                        address_taken_symbols.get(tl.name, {})
                        if address_taken_symbols is not None
                        else {}
                    )
                    fn_out, dims = replace_function_bare_exit(
                        tl, statics, symbols, types, coloring=coloring,
                        param_layout=layout,
                        private_pool_addrs=private,
                        address_taken_zp=addr_taken_zp,
                        address_taken_symbols=addr_taken_syms,
                    )
                    new_top.append(fn_out)
                    dims_by_fn[tl.name] = dims
            return asm_ast.Program(top_level=new_top), dims_by_fn
        case _:
            raise TypeError(f"unexpected program node: {prog!r}")
