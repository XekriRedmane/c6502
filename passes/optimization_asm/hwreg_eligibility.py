"""HwReg (X / Y) coloring eligibility for asm-level SSA Pseudos.

The asm regalloc colors most Pseudos to ZP byte addresses, but a
subset is eligible to live in the 6502's X or Y index register
across the entire (intra-block) live range. The headline win is
eliminating the LDX / LDY setup before each `IndexedData(name,
offset, index=R)` access where the index value is the HwReg-pinned
Pseudo: tac_to_asm always emits

    Mov(P, Reg(A)); Mov(Reg(A), Reg(X)); IndexedData(... index=X)

(LDA / TAX / LDA name,X). When P is HwReg-pinned to X, the setup
collapses to nothing — X already holds P's value.

# Eligibility rule

A Pseudo `P` is HwReg-eligible iff EVERY def and use of `P` in the
function falls into the supported instruction set. Defs:

  * `Mov(<Imm | Reg(A) | ZP | Data>, P)` — LDX / LDY (the 6502 has
    LDX / LDY zp / abs / # but no indirect-Y, so Stack / Frame /
    Indirect sources fall back to A as a conduit).
  * `Inc(P)` — INX / INY.
  * `Dec(P)` — DEX / DEY.
  * `Phi(dst=P)` — SSA Phi node merging values from predecessors.
    The de-SSA copies become Mov(<src>, P) at the predecessor tail,
    where <src> is itself another Pseudo (likely also HwReg-pinned
    after coalescing) or one of the LDX/LDY-able shapes.

Uses:

  * `Mov(P, <Reg(A) | ZP | Data>)` — TXA / TYA / STX / STY (zp/abs
    only; Stack/Frame/Indirect dests need A).
  * `Mov(P, <other Pseudo>)` — kept for completeness; emit handles
    Reg(X|Y)→Reg(X|Y) cross-moves via A. (After coalescing, this
    case typically disappears.)
  * `Inc(P)` / `Dec(P)` — already covered by defs (RMW).
  * `Compare(P, <Imm | ZP | Data>)` — CPX / CPY.
  * IndexedData index slot — the value is consumed implicitly by
    the absolute,X / absolute,Y addressing mode. There's no
    Pseudo→Reg explicit Mov in this case; the consumer is the
    `Mov(P, Reg(A)); Mov(Reg(A), Reg(X|Y))` setup chain that
    tac_to_asm emits before each IndexedData access. We detect
    that chain and treat the IndexedData operand as a use of P.

Hard constraints:

  * Single-byte (offset == 0). Multi-byte values can't ride in X/Y.
  * `lives_across_call == False`. The 6502 helper-call ABI clobbers
    A, X, and Y; any value live across a `Call` instruction can't
    be HwReg-pinned. Reads from `lives_across_call` set by the
    interference builder.
  * Width 1 in the interference graph. (Same as offset==0; defense
    against an unusual multi-byte name slipping through.)

If ANY def or use of `P` doesn't match the rules above, `P` is
ineligible. The eligibility scan visits every instruction once.

# Hints

A subset of eligible Pseudos have a *strong preference* for a
specific HwReg, derived from the existing IR shape:

  * **X hint**: P appears in a chain `Mov(P, Reg(A)); Mov(Reg(A),
    Reg(X))` (typical IndexedData index setup). After coloring P
    to X, the chain becomes `Mov(Reg(X), Reg(A)); Mov(Reg(A),
    Reg(X))` — apply_coloring drops both as redundant. P is then
    "free" for downstream IndexedData uses.
  * **Y hint**: same pattern with Reg(Y). Less common in current
    tac_to_asm output (every IndexedData uses X today), but
    surfaces after the apply_coloring rewrite of IndexedData.index
    when one Pseudo is X-pinned and another wants the same role.

Pseudos appearing in BOTH hint sets typically don't exist (they'd
have to be index-setup'd for both X and Y in the same function),
but if they do, the coloring pass picks whichever HwReg is still
free.

# Output

`scan_function(fn)` returns a `HwRegEligibility` with:

  * `eligible`: set of Pseudo names that pass the per-instruction
    rules. Membership is checked at coloring time; non-eligible
    names CANNOT be HwReg-pinned regardless of hints.
  * `hints_x`, `hints_y`: subsets of `eligible` with the
    corresponding pref. May overlap with each other (rare).

The interference-graph node's `lives_across_call` flag is checked
separately by the regalloc driver — this module's eligibility
scan is purely instruction-shape-based.
"""

from __future__ import annotations

from dataclasses import dataclass, field

import asm_ast


@dataclass
class HwRegEligibility:
    """Per-function HwReg coloring eligibility + hint sets +
    per-name use-count weights.

    Eligibility is per-HwReg: `eligible_x` and `eligible_y` are
    independent sets, because some operand shapes work in only one
    of the two. The canonical asymmetric case is `Mov(IndexedData,
    P)` — pinning P to Y is fine when the IndexedData indexes by X
    (`LDY abs,X` exists), but pinning to X would require `LDX abs,X`
    which doesn't exist on the 6502; the reverse holds for an
    IndexedData indexed by Y. Most other peer shapes (Imm, ZP, Data,
    Reg) are symmetric: `eligible_x` and `eligible_y` agree.

    `use_count[name]` is the number of times `name` appears as a
    Pseudo operand anywhere in the function (any role). Used to
    prioritize candidates at coloring time: a name that appears
    50× (a long-lived loop-iv used in many indexed accesses)
    should win HwReg pinning over a name that appears 2× (a
    short-lived intermediate). The count is approximate — it
    treats each operand position equally regardless of static
    weight, and doesn't model loop nesting — but tracks the
    intuition that more references = more savings."""
    eligible_x: set[str] = field(default_factory=set)
    eligible_y: set[str] = field(default_factory=set)
    hints_x: set[str] = field(default_factory=set)
    hints_y: set[str] = field(default_factory=set)
    use_count: dict[str, int] = field(default_factory=dict)

    @property
    def eligible(self) -> set[str]:
        """Union of `eligible_x` and `eligible_y` — a name is in
        `eligible` iff it can be pinned to at least one HwReg.
        Provided for callers that only need a "could this be
        HwReg-pinned at all?" check."""
        return self.eligible_x | self.eligible_y


def scan_function(fn: asm_ast.Function) -> HwRegEligibility:
    """Compute the per-Pseudo eligibility sets for HwReg coloring,
    plus X/Y hint sets derived from the index-setup chain pattern.
    Single forward pass over `fn.instructions`.

    Eligibility is per-HwReg AND conservative: a Pseudo is in
    `eligible_x` iff every def/use is X-encodable, and similarly
    for `eligible_y`. A name can be in both (the common case for
    Imm / ZP / Data / Reg peers), in one (when an IndexedData
    peer is asymmetric), or in neither (any disqualifying role)."""
    seen: set[str] = set()
    disqualified_x: set[str] = set()
    disqualified_y: set[str] = set()
    hints_x: set[str] = set()
    hints_y: set[str] = set()
    use_count: dict[str, int] = {}

    instrs = fn.instructions
    for i, instr in enumerate(instrs):
        for op_role, op in _operand_roles(instr):
            if not isinstance(op, asm_ast.Pseudo):
                continue
            seen.add(op.name)
            use_count[op.name] = use_count.get(op.name, 0) + 1
            if op.offset != 0:
                disqualified_x.add(op.name)
                disqualified_y.add(op.name)
                continue
            x_ok, y_ok = _role_ok(op_role, instr)
            if not x_ok:
                disqualified_x.add(op.name)
            if not y_ok:
                disqualified_y.add(op.name)
        # Detect the index-setup chain at this instruction position:
        #   instrs[i-1] = Mov(P, Reg(A))
        #   instrs[i  ] = Mov(Reg(A), Reg(X|Y))
        # If matched, P picks up the corresponding HwReg hint. The
        # chain is local (two adjacent instructions); a longer chain
        # broken by other code in between doesn't qualify (defensive
        # — anything else might clobber A or the index reg between).
        if i > 0 and isinstance(instr, asm_ast.Mov):
            prev = instrs[i - 1]
            if (
                isinstance(prev, asm_ast.Mov)
                and isinstance(prev.dst, asm_ast.Reg)
                and isinstance(prev.dst.reg, asm_ast.A)
                and isinstance(prev.src, asm_ast.Pseudo)
                and prev.src.offset == 0
                and isinstance(instr.src, asm_ast.Reg)
                and isinstance(instr.src.reg, asm_ast.A)
                and isinstance(instr.dst, asm_ast.Reg)
            ):
                p_name = prev.src.name
                if isinstance(instr.dst.reg, asm_ast.X):
                    hints_x.add(p_name)
                elif isinstance(instr.dst.reg, asm_ast.Y):
                    hints_y.add(p_name)

    eligible_x = seen - disqualified_x
    eligible_y = seen - disqualified_y
    # Hints are restricted to the UNION of eligibility — a name with
    # hint_x but only y-eligible is still a useful preference signal
    # (the regalloc's fallback path tries the other HwReg). A name
    # disqualified from BOTH X and Y can't be HwReg-pinned at all, so
    # hints for it are dropped.
    eligible_any = eligible_x | eligible_y
    hints_x &= eligible_any
    hints_y &= eligible_any
    return HwRegEligibility(
        eligible_x=eligible_x, eligible_y=eligible_y,
        hints_x=hints_x, hints_y=hints_y,
        use_count=use_count,
    )


# ---------------------------------------------------------------------------
# Per-instruction operand role enumeration.
# ---------------------------------------------------------------------------

# Operand roles distinguish "what kind of position is this Pseudo
# in" — the same Pseudo at different roles has different HwReg-
# representability constraints. Roles:
#   "mov_src"    — Mov(P, X)
#   "mov_dst"    — Mov(_, P)
#   "inc_dec"    — Inc(P) / Dec(P)
#   "cmp_left"   — Compare(P, _)
#   "cmp_right"  — Compare(_, P)
#   "phi_dst"    — Phi(P, ...)
#   "phi_arg"    — Phi(_, args=[..., (_, P), ...])
#   "other"      — any other position (binary ops, Push/Pop,
#                  LoadAddress, etc.) — disqualifies.


def _operand_roles(instr: asm_ast.Type_instruction):
    """Yield `(role, operand)` tuples for every operand in `instr`.
    The role string drives `_role_ok` to decide if a Pseudo at that
    position can survive HwReg pinning."""
    match instr:
        case asm_ast.Mov(src=src, dst=dst):
            yield ("mov_src", src)
            yield ("mov_dst", dst)
        case asm_ast.Inc(dst=dst) | asm_ast.Dec(dst=dst):
            yield ("inc_dec", dst)
        case asm_ast.Compare(left=left, right=right):
            yield ("cmp_left", left)
            yield ("cmp_right", right)
        case asm_ast.Phi(dst=dst, args=args):
            yield ("phi_dst", dst)
            for a in args:
                yield ("phi_arg", a.source)
        case (
            asm_ast.Add(src=s, dst=d)
            | asm_ast.Sub(src=s, dst=d)
            | asm_ast.And(src=s, dst=d)
            | asm_ast.Or(src=s, dst=d)
        ):
            yield ("other", s)
            yield ("other", d)
        case asm_ast.Xor(src1=s1, src2=s2, dst=d):
            yield ("other", s1)
            yield ("other", s2)
            yield ("other", d)
        case (
            asm_ast.ArithmeticShiftLeft(dst=d)
            | asm_ast.LogicalShiftRight(dst=d)
            | asm_ast.RotateLeft(dst=d)
            | asm_ast.RotateRight(dst=d)
        ):
            yield ("other", d)
        case asm_ast.Push(src=src):
            yield ("other", src)
        case asm_ast.Pop(dst=dst):
            yield ("other", dst)
        case asm_ast.LoadAddress(src=src, dst=dst):
            yield ("other", src)
            yield ("other", dst)


def _role_ok(role: str, instr: asm_ast.Type_instruction) -> tuple[bool, bool]:
    """Is a Pseudo in `role` of `instr` representable when colored
    to X or Y? Returns `(x_ok, y_ok)` — independent per HwReg.

    Most roles are symmetric (the same answer for X and Y), but
    `Mov` with an `IndexedData` peer is asymmetric: `LDX abs,Y` and
    `LDY abs,X` exist, but `LDX abs,X` and `LDY abs,Y` don't. So a
    Pseudo at `mov_dst` with peer `IndexedData(index=X)` is
    Y-eligible but not X-eligible.
    """
    if role == "other":
        return (False, False)
    if role == "inc_dec":
        return (True, True)
    if role == "phi_dst" or role == "phi_arg":
        # Phi-arg substitution happens in apply_coloring AFTER
        # SSA destruction lowers each PhiArg to a Mov on the
        # predecessor edge. The Mov's compatibility is checked at
        # the destruction site as a regular mov_src/mov_dst pair,
        # so we accept Phi unconditionally here.
        return (True, True)
    if isinstance(instr, asm_ast.Mov):
        peer = instr.dst if role == "mov_src" else instr.src
        return _peer_is_hwreg_friendly(peer, role)
    if role == "cmp_left":
        # Compare(Reg(X|Y), <Imm | ZP | Data>) → CPX/CPY
        # <imm/zp/abs>. Frame/Stack/Indirect right operands aren't
        # supported — CPX/CPY have no indirect-Y.
        right = instr.right  # type: ignore[attr-defined]
        ok = _cmp_peer_is_hwreg_friendly(right)
        return (ok, ok)
    if role == "cmp_right":
        # Compare with Pseudo on the right side: unusual; CPX/CPY
        # don't have a form with X/Y on the right unless `left` is
        # Reg(A) — disqualifying defensively.
        return (False, False)
    return (False, False)


def _peer_is_hwreg_friendly(
    peer: asm_ast.Type_operand, role: str,
) -> tuple[bool, bool]:
    """Is `peer` a Mov peer (src or dst) that asm_emit can render
    when the other side is Reg(X) / Reg(Y)? Returns `(x_ok, y_ok)`.

    `role` is one of "mov_src" (the Pseudo is the Mov's src; peer
    is the dst) or "mov_dst" (the Pseudo is the dst; peer is the
    src). The split matters only for IndexedData: loads through
    `LDX abs,Y` / `LDY abs,X` are valid (mov_dst path); stores
    through `STX abs,Y` / `STY abs,X` etc. don't exist on the
    6502, so no `Mov(Reg(X|Y), IndexedData)` shape is emittable.
    """
    if isinstance(peer, asm_ast.Imm):
        return (True, True)
    if isinstance(peer, asm_ast.Reg):
        # A, X, Y are all OK — TXA/TYA/TAX/TAY exist; same-reg
        # self-Movs are dropped by the self-Mov peephole.
        return (True, True)
    if isinstance(peer, (asm_ast.Data, asm_ast.ZP)):
        # LDX/LDY zp/abs ; STX/STY zp/abs.
        return (True, True)
    if isinstance(peer, asm_ast.Pseudo):
        # Post-coalescing the peer may or may not be HwReg-colored;
        # apply_coloring resolves both sides before emit.
        return (True, True)
    if isinstance(peer, (asm_ast.Stack, asm_ast.Frame, asm_ast.Indirect)):
        # Indirect-Y addressing — emit goes through A.
        return (True, True)
    if isinstance(peer, asm_ast.IndexedData):
        if role == "mov_dst":
            # The Pseudo is the load target; peer is IndexedData
            # src. Valid only when the load opcode's destination
            # register differs from the IndexedData index register
            # (`LDX abs,Y`, `LDY abs,X`).
            x_ok = isinstance(peer.index, asm_ast.Y)
            y_ok = isinstance(peer.index, asm_ast.X)
            return (x_ok, y_ok)
        # mov_src: store. No `STX abs,X|Y` / `STY abs,X|Y` on the
        # 6502 — emit routes A→IndexedData only.
        return (False, False)
    if isinstance(peer, (asm_ast.ImmLabelLow, asm_ast.ImmLabelHigh)):
        # LDA #<name (immediate) — only as src; only into A in
        # current emit. Treating as "ok if peer is sensible"; if
        # this turns out to need finer dispatch, refine later.
        return (True, True)
    return (False, False)


def _cmp_peer_is_hwreg_friendly(peer: asm_ast.Type_operand) -> bool:
    """Is `peer` a Compare right-operand that CPX/CPY can render?
    CPX/CPY have only #imm / zp / abs addressing — no indirect-Y."""
    if isinstance(peer, asm_ast.Imm):
        return True
    if isinstance(peer, (asm_ast.Data, asm_ast.ZP)):
        return True
    # Reg / Pseudo / Stack / Frame / Indirect / IndexedData /
    # ImmLabel as the right operand of CPX/CPY isn't directly
    # representable. (A Pseudo right operand could become a Reg or
    # ZP after coloring, but we'd have to re-validate post-coloring;
    # easier to disqualify here.)
    return False
