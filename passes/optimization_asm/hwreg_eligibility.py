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

    `use_count[name]` is the number of times `name` appears as a
    Pseudo operand anywhere in the function (any role). Used to
    prioritize candidates at coloring time: a name that appears
    50× (a long-lived loop-iv used in many indexed accesses)
    should win HwReg pinning over a name that appears 2× (a
    short-lived intermediate). The count is approximate — it
    treats each operand position equally regardless of static
    weight, and doesn't model loop nesting — but tracks the
    intuition that more references = more savings."""
    eligible: set[str] = field(default_factory=set)
    hints_x: set[str] = field(default_factory=set)
    hints_y: set[str] = field(default_factory=set)
    use_count: dict[str, int] = field(default_factory=dict)


def scan_function(fn: asm_ast.Function) -> HwRegEligibility:
    """Compute the per-Pseudo eligibility set for HwReg coloring,
    plus X/Y hint sets derived from the index-setup chain pattern.
    Single forward pass over `fn.instructions`.

    Eligibility is conservative: if any operand position involving a
    Pseudo doesn't match the supported shapes (Mov sources/dests,
    Inc/Dec, Compare, Mov-into-Reg(A) followed by TAX/TAY chain), the
    Pseudo is dropped from `eligible`. The dropped name then can't
    be HwReg-pinned even if it would otherwise have a hint."""
    # Collect every Pseudo name that appears anywhere, with the
    # offset they appear at (eligible names must only appear at
    # offset 0). Track per-name "all OK so far" — flips False on
    # any disqualifying use.
    seen: set[str] = set()
    disqualified: set[str] = set()
    hints_x: set[str] = set()
    hints_y: set[str] = set()
    use_count: dict[str, int] = {}

    def disqualify(name: str) -> None:
        disqualified.add(name)

    instrs = fn.instructions
    n = len(instrs)
    for i, instr in enumerate(instrs):
        for op_role, op in _operand_roles(instr):
            if not isinstance(op, asm_ast.Pseudo):
                continue
            seen.add(op.name)
            use_count[op.name] = use_count.get(op.name, 0) + 1
            if op.offset != 0:
                disqualify(op.name)
                continue
            # Each role determines whether this site is HwReg-
            # representable. Any "no" disqualifies the name.
            if not _role_ok(op_role, instr):
                disqualify(op.name)
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

    eligible = seen - disqualified
    # Hints are restricted to the eligible set — a hinted-but-
    # disqualified name can't be HwReg-pinned anyway.
    hints_x &= eligible
    hints_y &= eligible
    return HwRegEligibility(
        eligible=eligible, hints_x=hints_x, hints_y=hints_y,
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


def _role_ok(role: str, instr: asm_ast.Type_instruction) -> bool:
    """Is a Pseudo in `role` of `instr` representable when colored
    to X or Y? Only certain (role, peer-operand-shape) combinations
    work — e.g. Compare(P, Imm) is fine (CPX #imm) but Compare(P,
    Frame) needs A as scratch (CPX can't load through indirect-Y),
    so we reject it for HwReg eligibility."""
    if role == "other":
        return False
    if role == "inc_dec":
        return True
    if role == "phi_dst" or role == "phi_arg":
        # Phi-arg substitution happens in apply_coloring AFTER
        # SSA destruction lowers each PhiArg to a Mov on the
        # predecessor edge. The Mov's compatibility is checked at
        # the destruction site as a regular mov_src/mov_dst pair,
        # so we accept Phi unconditionally here.
        return True
    if isinstance(instr, asm_ast.Mov):
        # The Pseudo at `role` must have a peer operand that's
        # one of the HwReg-friendly shapes. Specifically:
        #   mov_dst (Pseudo is the dst): src must be Imm /
        #     Reg(A) / ZP / Data / IndexedData. Stack/Frame/
        #     Indirect/ImmLabel/Pseudo->Reg(X|Y) ok (they go
        #     through A in emit). Reg(X|Y) sources are also fine
        #     (TXA/TYA via A, then TAY/TAX).
        #   mov_src (Pseudo is the src): dst must be Reg(A) /
        #     ZP / Data / Reg(X|Y). Stack/Frame/Indirect dsts
        #     would need TXA + STA via indirect-Y, which we
        #     accept (they go through A). Other Pseudos as dst
        #     accepted (post-coalescing, the other Pseudo may
        #     also be HwReg-colored).
        # We allow all the source/dst shapes that asm_emit can
        # render — see _emit_mov in asm_emit.py.
        peer = instr.dst if role == "mov_src" else instr.src
        return _peer_is_hwreg_friendly(peer)
    if role == "cmp_left":
        # Compare(Reg(X|Y), <Imm | ZP | Data | Reg(A) | Reg(X|Y)>)
        # → CPX/CPY <imm/zp/abs>. Frame/Stack/Indirect right
        # operands aren't supported — CPX/CPY have no indirect-Y.
        right = instr.right  # type: ignore[attr-defined]
        return _cmp_peer_is_hwreg_friendly(right)
    if role == "cmp_right":
        # Compare with Pseudo on the right side: this is unusual
        # (typical lowering puts the Pseudo on the left), and CPX
        # doesn't have a "compare with X" with an arbitrary left
        # — the `left` would have to be Reg(A). Treating as
        # disqualifying defensively; if a left==Reg(A) check
        # turns out to matter, lift it later.
        return False
    return False


def _peer_is_hwreg_friendly(peer: asm_ast.Type_operand) -> bool:
    """Is `peer` a Mov peer (src or dst) that asm_emit can render
    when the other side is Reg(X|Y)?"""
    if isinstance(peer, asm_ast.Imm):
        return True
    if isinstance(peer, asm_ast.Reg):
        # A, X, Y are all OK — TXA/TYA/TAX/TAY exist; same-reg
        # self-Movs are dropped by the self-Mov peephole.
        return True
    if isinstance(peer, (asm_ast.Data, asm_ast.ZP)):
        # LDX/LDY zp/abs ; STX/STY zp/abs.
        return True
    if isinstance(peer, asm_ast.Pseudo):
        # Post-coalescing the peer may or may not be HwReg-colored;
        # apply_coloring resolves both sides before emit.
        return True
    if isinstance(peer, (asm_ast.Stack, asm_ast.Frame, asm_ast.Indirect)):
        # Indirect-Y addressing — emit goes through A.
        return True
    if isinstance(peer, asm_ast.IndexedData):
        # LDA name,Y / LDA name,X — emit goes through A.
        return True
    if isinstance(peer, (asm_ast.ImmLabelLow, asm_ast.ImmLabelHigh)):
        # LDA #<name (immediate) — only as src; only into A in
        # current emit. Treating as "ok if peer is sensible"; if
        # this turns out to need finer dispatch, refine later.
        return True
    return False


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
