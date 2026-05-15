"""TAC pre-SSA loop rotation for signed-countdown for-loops.

Recognises the canonical `c99_to_tac` for-loop shape

    [<init>]                                ; ends with x_var := nonneg-const
    Label(<L>_start)
    [<entry cond>]                          ; emits cond_var
    JumpIfFalse(cond_var, <L>_break)
    [<body>]
    Label(<L>_continue)
    [<post>]                                ; ends with x_var := x_var - 1
    Jump(<L>_start)
    Label(<L>_break)

and rewrites it to test-at-bottom

    [<init>]
    Label(<L>_start)
    [<body, possibly with SignExtend(x_var) → ZeroExtend rewrite>]
    Label(<L>_continue)
    [<post>]
    [<entry cond>]                          ; same insns, NEW position
    JumpIfTrue(cond_var, <L>_start)         ; replaces both old jumps
    Label(<L>_break)

eliminating the entry test (provable to pass on first iteration)
and folding the unconditional `Jump(<L>_start)` into the now-
conditional back-edge.

# SignExtend → ZeroExtend rewrite

Given the rotation's preconditions — init `>= 0`, post is `x -= 1`,
test is `x >= 0` at the bottom — the loop body always sees `x_var`
in `[0, init_value]`, never negative. For non-negative bytes,
`SignExtend(byte) ≡ ZeroExtend(byte)` at the bit-pattern level
(both produce the byte with a high zero byte). So every
`SignExtend(Var(x_var.name), _)` instruction in the rotated body
is rewritten to a `ZeroExtend` of the same source / dst.

The win comes downstream: `tac_to_asm` lowers `ZeroExtend` as a
2-instruction `LDA src; LDA #0; STA dst.hi` (no branch), vs.
SignExtend's 6-instruction `LDA src; BMI .neg; LDA #0; JMP .done;
.neg: LDA #$FF; .done: STA dst.hi`. More importantly, the
IndexedStore / IndexedConstLoad recognizers gate on ZeroExtend
of the index, so this rewrite unblocks them for signed-iv loops
— a `for (int8_t x = N; x >= 0; x--)` body that uses `x` as an
index into a static array now collapses to absolute,X stores
just like the unsigned-counter version would.

# Soundness gate

The rewrite is only sound when the loop body never modifies
`x_var`. Any def of `x_var` in the body could push it negative
(e.g., `for (int8_t i = 5; i >= 0; i--) i--;`), at which point
SignExtend ≠ ZeroExtend at later use sites. We refuse the rewrite
(but still rotate) when the body contains any def of `x_var`.

# Eligibility

The C-source shape we target is

    for (T x = N; x >= 0; x--) { ... }

with N a non-negative literal and T a signed integer type
(`int8_t`, `int`, `long`, `long long`).  Translated into matcher
language:

  * The cond block evaluates `x_var >= 0`. We accept either
    `Binary(GE, x_var, Constant(0), cond_var)` directly, or a
    SignExtend / Copy chain of `x_var` followed by the GE — the
    latter arises from C99's integer-promotion of narrower
    operands (`SChar` → `Int` for the comparison).
  * The post block ends with `Binary(Subtract, x_var, Constant(1),
    %t); Copy(%t, x_var)`.  Postfix `x--` additionally prepends a
    `Copy(x_var, %old)` capturing the pre-mutation value; that
    Copy lives in the post block but is dead in for-post context
    (and gets DCE'd downstream), so we ignore any insns before
    the trailing two.
  * `x_var`'s c99 type is `SChar` / `Int` / `Long` / `LongLong`.
    Char and the unsigned variants reject — `>= 0` is trivially
    true on an unsigned type and the rotation would loop forever.
  * `x_var`'s last def before `Label(<L>_start)` resolves to a
    constant in `[0, max_signed_for_x_type]` — guaranteeing the
    first iteration would have entered the original test-at-top
    loop, which is the precondition for safely deferring the
    test past one body execution.

# Why pre-SSA

Pre-SSA the variable `x_var` keeps a single canonical name across
init, body, and post; the rewrite is a structural shuffle of
instruction ranges with no name updates.  Post-SSA the entry-
test cond uses the Phi'd name (the merge of init and post-
decrement) while the post-decrement defines a fresh SSA name —
so the same shuffle would also need a Phi argument retag, which
isn't worth the complexity for what's structurally identical.

Runs once per function as a one-shot before `to_ssa`. Idempotent
in the trivial sense: a rotated for-loop has no preceding
cond+JumpIfFalse before its body, so the matcher refuses to fire
a second time.

# Why not also `while` / `do-while`

`do-while` is already test-at-bottom — nothing to rotate.
`while (cond) body` could be rotated by peeling the cond test
before the loop, but proving the entry test passes for an
arbitrary `cond` is harder than for a literal-bound `for`. The
limited target keeps the soundness gate tight; widening is a
follow-up.
"""

from __future__ import annotations

from typing import NamedTuple

import c99_ast
import tac_ast


# c99 types eligible: signed integers wide enough to compare
# meaningfully against zero. `Char` is c6502's plain-char
# (unsigned 0..255) so reject; `SChar` (signed) accepts.
_ELIGIBLE_TYPES: tuple[type, ...] = (
    c99_ast.SChar, c99_ast.Int, c99_ast.Long, c99_ast.LongLong,
)


# Inclusive upper bound on a non-negative init constant per signed
# integer type. Init values in [0, max] truncate / sign-extend to a
# non-negative byte pattern at x_var's declared width — the runtime
# property the rotation requires.
_MAX_POSITIVE: dict[type, int] = {
    c99_ast.SChar:    0x7F,
    c99_ast.Int:      0x7FFF,
    c99_ast.Long:     0x7FFFFFFF,
    c99_ast.LongLong: 0x7FFFFFFFFFFFFFFF,
}


# Const variants whose `value` field is a Python int. Excludes the
# float variants (their `bits` field is an int but represents an
# IEEE 754 bit pattern, not a value we'd compare against 0 in this
# pass).
_INT_CONST_VARIANTS: tuple[type, ...] = (
    tac_ast.ConstChar, tac_ast.ConstUChar,
    tac_ast.ConstInt, tac_ast.ConstUInt,
    tac_ast.ConstLong, tac_ast.ConstULong,
    tac_ast.ConstLongLong, tac_ast.ConstULongLong,
)


# Instruction kinds that end a straight-line region. The cond
# block (entry test) and the post block must each be straight-
# line — labels / jumps / calls inside either are not what
# `c99_to_tac` emits for the shapes we target, so refusing them
# is a soundness rail (it could never have been a `>= 0` test
# block or a single-decrement post block).
_COND_BLOCK_BREAKERS: tuple[type, ...] = (
    tac_ast.Label,
    tac_ast.Jump, tac_ast.JumpIfTrue, tac_ast.JumpIfCmp,
    tac_ast.JumpIfMasked,
    tac_ast.Ret,
    tac_ast.FunctionCall, tac_ast.IndirectCall,
)
_POST_BLOCK_BREAKERS: tuple[type, ...] = (
    tac_ast.Label,
    tac_ast.Jump, tac_ast.JumpIfTrue, tac_ast.JumpIfFalse,
    tac_ast.JumpIfCmp, tac_ast.JumpIfMasked,
    tac_ast.Ret,
    tac_ast.FunctionCall, tac_ast.IndirectCall,
)


class _Match(NamedTuple):
    """Resolved index ranges + cond-var for a rotatable for-loop."""
    start_idx: int          # Label(L_start)
    cond_jif_idx: int       # JumpIfFalse(cond_var, L_break)
    cont_idx: int           # Label(L_continue)
    jump_back_idx: int      # Jump(L_start)
    break_idx: int          # Label(L_break)
    cond_var_name: str
    start_label: str
    x_name: str             # iv variable's resolved name
    body_iv_immutable: bool # body has no defs of x_name → SE→ZE safe


def rotate_signed_countdown_loops(
    fn: tac_ast.Function, symbols,
) -> tac_ast.Function:
    """Apply the rotation to every eligible for-loop in `fn`.

    Pre-SSA expected: instruction operands are c99_to_tac's raw
    `Var(@N.orig)` / `Var(%n)` names with no Phi merges. The
    rewrite is name-preserving; running this pass before
    `to_ssa` is the supported call site.
    """
    instrs = list(fn.instructions)
    out: list[tac_ast.Type_instruction] = []
    i = 0
    n = len(instrs)
    while i < n:
        m = _try_match(instrs, i, symbols)
        if m is None:
            out.append(instrs[i])
            i += 1
            continue
        out.extend(_emit_rotated(instrs, m))
        i = m.break_idx + 1
    return tac_ast.Function(
        name=fn.name, is_global=fn.is_global,
        params=list(fn.params), instructions=out,
    )


# ---------------------------------------------------------------------------
# Matcher
# ---------------------------------------------------------------------------


def _try_match(
    instrs: list[tac_ast.Type_instruction], i: int, symbols,
) -> _Match | None:
    """Attempt to match a rotatable for-loop whose `Label(L_start)`
    sits at `instrs[i]`. Returns the populated `_Match` on success
    or `None` on any eligibility failure."""
    label = instrs[i]
    if not isinstance(label, tac_ast.Label):
        return None
    L_start = label.name
    # Loop labels are minted as `<base>_start` / `<base>_continue` /
    # `<base>_break` by `c99_to_tac`'s for-stmt lowering. Other
    # callers (do-while-start, ssa-preheader) use other suffixes.
    if not L_start.endswith("_start"):
        return None
    base = L_start[: -len("_start")]
    L_continue = base + "_continue"
    L_break = base + "_break"

    n = len(instrs)
    # Locate the JumpIfFalse(_, L_break) capping the entry test.
    cond_jif_idx: int | None = None
    cond_var_name: str | None = None
    j = i + 1
    while j < n:
        instr = instrs[j]
        if isinstance(instr, tac_ast.JumpIfFalse) and instr.target == L_break:
            cond = instr.condition
            if not isinstance(cond, tac_ast.Var):
                return None
            cond_var_name = cond.name
            cond_jif_idx = j
            break
        if isinstance(instr, _COND_BLOCK_BREAKERS):
            return None
        j += 1
    if cond_jif_idx is None or cond_var_name is None:
        return None

    cond_block = instrs[i + 1 : cond_jif_idx]
    ge = _find_ge_producer(cond_block, cond_var_name)
    if ge is None:
        return None
    if not _val_resolves_to_zero(ge.src2, cond_block):
        return None
    x_name = _trace_to_var(ge.src1, cond_block)
    if x_name is None:
        return None

    sym = symbols.get(x_name) if hasattr(symbols, "get") else symbols[x_name]
    if sym is None:
        return None
    x_type = sym.type
    if not isinstance(x_type, _ELIGIBLE_TYPES):
        return None

    # Find Label(L_continue) past the body. The body is whatever
    # sits between the JumpIfFalse and the continue label; we don't
    # walk into it, so labels / nested loops / calls are all fine.
    cont_idx: int | None = None
    j = cond_jif_idx + 1
    while j < n:
        cur = instrs[j]
        if isinstance(cur, tac_ast.Label) and cur.name == L_continue:
            cont_idx = j
            break
        j += 1
    if cont_idx is None:
        return None

    # Find Jump(L_start) past the post block. The post block must
    # be straight-line: the c99_to_tac for-loop lowering emits the
    # post-clause expression for side effects only, and `x--` is a
    # straight-line shape.
    jump_back_idx: int | None = None
    j = cont_idx + 1
    while j < n:
        instr = instrs[j]
        if isinstance(instr, tac_ast.Jump) and instr.target == L_start:
            jump_back_idx = j
            break
        if isinstance(instr, _POST_BLOCK_BREAKERS):
            return None
        j += 1
    if jump_back_idx is None:
        return None

    if jump_back_idx + 1 >= n:
        return None
    after_jump = instrs[jump_back_idx + 1]
    if not (isinstance(after_jump, tac_ast.Label) and after_jump.name == L_break):
        return None
    break_idx = jump_back_idx + 1

    post_block = instrs[cont_idx + 1 : jump_back_idx]
    if not _post_decrements_x_by_one(post_block, x_name):
        return None

    if not _init_value_is_nonnegative(instrs, i, x_name, type(x_type)):
        return None

    body_block = instrs[cond_jif_idx + 1 : cont_idx]
    body_iv_immutable = not any(
        x_name in _defs_of(ins) for ins in body_block
    )

    return _Match(
        start_idx=i,
        cond_jif_idx=cond_jif_idx,
        cont_idx=cont_idx,
        jump_back_idx=jump_back_idx,
        break_idx=break_idx,
        cond_var_name=cond_var_name,
        start_label=L_start,
        x_name=x_name,
        body_iv_immutable=body_iv_immutable,
    )


def _find_ge_producer(
    cond_block: list[tac_ast.Type_instruction], cond_var: str,
) -> tac_ast.Binary | None:
    """The cond block's `cond_var` def, if it's a single
    `Binary(GreaterOrEqual, ?, ?, cond_var)`. Returns None on any
    other shape."""
    producer = None
    for ins in cond_block:
        if not isinstance(ins, tac_ast.Binary):
            continue
        if not isinstance(ins.dst, tac_ast.Var):
            continue
        if ins.dst.name != cond_var:
            continue
        producer = ins
    if producer is None:
        return None
    if not isinstance(producer.op, tac_ast.GreaterOrEqual):
        return None
    return producer


def _val_resolves_to_zero(
    val: tac_ast.Type_val, cond_block: list[tac_ast.Type_instruction],
) -> bool:
    """True if `val` evaluates to 0 either as a direct integer
    Constant, or via a SignExtend / ZeroExtend / Copy chain within
    `cond_block` rooted at an integer Constant(0). c99_to_tac
    emits the chain when the literal 0's type is narrower than
    the comparison's working type (e.g. `long x; x >= 0` produces
    a SignExtend(ConstInt(0), %t) before the GE)."""
    if isinstance(val, tac_ast.Constant):
        return _const_int_value(val.const) == 0
    if not isinstance(val, tac_ast.Var):
        return False
    name = val.name
    for ins in cond_block:
        if not isinstance(ins.dst if hasattr(ins, "dst") else None, tac_ast.Var):
            continue
        if ins.dst.name != name:
            continue
        match ins:
            case tac_ast.SignExtend(src=src) | tac_ast.ZeroExtend(src=src) \
                    | tac_ast.Truncate(src=src) | tac_ast.Copy(src=src):
                return _val_resolves_to_zero(src, cond_block)
            case _:
                return False
    return False


def _trace_to_var(
    val: tac_ast.Type_val, cond_block: list[tac_ast.Type_instruction],
) -> str | None:
    """Resolve `val` to the source variable name on the LHS of the
    GE comparison. Accepts `val` as a direct `Var(@N.x)` or as a
    `Var(%t)` whose def in `cond_block` is a SignExtend / Copy of
    a Var. Returns the underlying x_var's name, or None on any
    indirection that doesn't terminate at a non-temp Var."""
    if not isinstance(val, tac_ast.Var):
        return None
    name = val.name
    if not name.startswith("%"):
        # Names starting with `@` (locals / params) or unprefixed
        # (statics) are storage names, not temps. Stop here.
        return name
    for ins in cond_block:
        d = ins.dst if hasattr(ins, "dst") else None
        if not isinstance(d, tac_ast.Var) or d.name != name:
            continue
        match ins:
            case tac_ast.SignExtend(src=src) | tac_ast.ZeroExtend(src=src) \
                    | tac_ast.Copy(src=src):
                return _trace_to_var(src, cond_block)
            case _:
                return None
    return None


def _post_decrements_x_by_one(
    post_block: list[tac_ast.Type_instruction], x_name: str,
) -> bool:
    """True if `post_block`'s last two instructions are
    `Binary(Subtract, x_var, Constant(1), %t); Copy(%t, x_var)`,
    and `x_var.name == x_name`. The leading instructions of the
    post block (e.g. Postfix's pre-mutation `Copy(x, %old)`) are
    permitted but not inspected — the trailing two define the
    semantically-relevant rmw."""
    if len(post_block) < 2:
        return False
    sub, cp = post_block[-2], post_block[-1]
    if not isinstance(sub, tac_ast.Binary):
        return False
    if not isinstance(sub.op, tac_ast.Subtract):
        return False
    if not (isinstance(sub.src1, tac_ast.Var) and sub.src1.name == x_name):
        return False
    if not isinstance(sub.src2, tac_ast.Constant):
        return False
    if _const_int_value(sub.src2.const) != 1:
        return False
    if not isinstance(sub.dst, tac_ast.Var):
        return False
    sub_dst = sub.dst.name
    if not isinstance(cp, tac_ast.Copy):
        return False
    if not (isinstance(cp.src, tac_ast.Var) and cp.src.name == sub_dst):
        return False
    if not (isinstance(cp.dst, tac_ast.Var) and cp.dst.name == x_name):
        return False
    return True


def _init_value_is_nonnegative(
    instrs: list[tac_ast.Type_instruction], start_idx: int,
    x_name: str, x_type_cls: type,
) -> bool:
    """True if the last def of `x_name` strictly before `instrs[
    start_idx]` resolves to an integer constant in `[0, _MAX_
    POSITIVE[x_type_cls]]`.

    Walks linearly backward, following Copy / Cast chains through
    temp Vars. Refuses on the first non-resolvable step (multi-def
    temps, opaque producers, cross-block predecessors) — the only
    shape we accept is `c99_to_tac`'s straight-line lowering of a
    constant init expression."""
    max_pos = _MAX_POSITIVE.get(x_type_cls)
    if max_pos is None:
        return False
    # Locate x_name's last def position before start_idx.
    def_idx = _last_def_pos(instrs, start_idx, x_name)
    if def_idx is None:
        return False
    return _resolves_to_int_in_range(
        instrs, def_idx, x_name, low=0, high=max_pos,
    )


def _last_def_pos(
    instrs: list[tac_ast.Type_instruction], end_excl: int, name: str,
) -> int | None:
    """Last index `< end_excl` whose instruction defines `name`,
    or None if no such index exists."""
    for k in range(end_excl - 1, -1, -1):
        for d in _defs_of(instrs[k]):
            if d == name:
                return k
    return None


def _resolves_to_int_in_range(
    instrs: list[tac_ast.Type_instruction], def_idx: int,
    name: str, *, low: int, high: int, depth: int = 0,
) -> bool:
    """The def at `instrs[def_idx]` (which writes `name`) reduces,
    via Copy / Cast chains over single-defined temps, to an
    integer constant in `[low, high]`."""
    if depth > 4:
        return False
    ins = instrs[def_idx]
    src: tac_ast.Type_val
    match ins:
        case tac_ast.Copy(src=s) | tac_ast.SignExtend(src=s) \
                | tac_ast.ZeroExtend(src=s) | tac_ast.Truncate(src=s):
            src = s
        case _:
            return False
    if isinstance(src, tac_ast.Constant):
        v = _const_int_value(src.const)
        return v is not None and low <= v <= high
    if not isinstance(src, tac_ast.Var):
        return False
    inner_name = src.name
    inner_def = _last_def_pos(instrs, def_idx, inner_name)
    if inner_def is None:
        return False
    return _resolves_to_int_in_range(
        instrs, inner_def, inner_name, low=low, high=high, depth=depth + 1,
    )


def _const_int_value(c: tac_ast.Type_const) -> int | None:
    """Integer payload of an int-typed Const variant; None for
    floating variants (which carry a `bits` field representing an
    IEEE 754 pattern, not the numerical value)."""
    if isinstance(c, _INT_CONST_VARIANTS):
        return c.value
    return None


def _defs_of(instr: tac_ast.Type_instruction) -> list[str]:
    """Var operand names defined by `instr`. Local copy of the
    canonical helper to avoid an import cycle with `var_visit`."""
    out: list[str] = []
    match instr:
        case tac_ast.SignExtend(dst=d) | tac_ast.ZeroExtend(dst=d) \
                | tac_ast.Truncate(dst=d) \
                | tac_ast.IntToFloat(dst=d) | tac_ast.IntToDouble(dst=d) \
                | tac_ast.FloatToInt(dst=d) | tac_ast.DoubleToInt(dst=d) \
                | tac_ast.FloatToDouble(dst=d) | tac_ast.DoubleToFloat(dst=d) \
                | tac_ast.Unary(dst=d) | tac_ast.Binary(dst=d) \
                | tac_ast.Copy(dst=d) \
                | tac_ast.GetAddress(dst=d) \
                | tac_ast.Load(dst=d) \
                | tac_ast.IndexedLoad(dst=d) \
                | tac_ast.IndexedConstLoad(dst=d) \
                | tac_ast.Phi(dst=d):
            if isinstance(d, tac_ast.Var):
                out.append(d.name)
        case tac_ast.FunctionCall(dst=d) | tac_ast.IndirectCall(dst=d):
            if d is not None and isinstance(d, tac_ast.Var):
                out.append(d.name)
    return out


# ---------------------------------------------------------------------------
# Emit
# ---------------------------------------------------------------------------


def _emit_rotated(
    instrs: list[tac_ast.Type_instruction], m: _Match,
) -> list[tac_ast.Type_instruction]:
    """Build the rotated instruction sequence for the matched
    range `instrs[m.start_idx .. m.break_idx]`. The init insns
    (everything BEFORE start_idx) are emitted by the caller's
    pass-through; this function emits exactly the
    `[start_idx .. break_idx]` slice in rotated order."""
    body_block = instrs[m.cond_jif_idx + 1 : m.cont_idx]
    if m.body_iv_immutable:
        body_block = _rewrite_signextend_iv_to_zeroextend(
            body_block, m.x_name,
        )
    out: list[tac_ast.Type_instruction] = []
    # Label(<L>_start)
    out.append(instrs[m.start_idx])
    # body insns (rewritten if SE→ZE was applied)
    out.extend(body_block)
    # Label(<L>_continue)
    out.append(instrs[m.cont_idx])
    # post insns (between Label(<L>_continue) and Jump(<L>_start), exclusive)
    out.extend(instrs[m.cont_idx + 1 : m.jump_back_idx])
    # cond compute insns (between Label(<L>_start) and JumpIfFalse, exclusive),
    # MOVED here. Same instructions, same temp names — the producer
    # of `cond_var` and any precomputed casts read x_var, which now
    # holds the post-decrement value at this point in the program.
    out.extend(instrs[m.start_idx + 1 : m.cond_jif_idx])
    # JumpIfTrue(cond_var, <L>_start) — replaces both the original
    # JumpIfFalse-to-break and the trailing Jump-to-start.
    out.append(tac_ast.JumpIfTrue(
        condition=tac_ast.Var(name=m.cond_var_name),
        target=m.start_label,
    ))
    # Label(<L>_break)
    out.append(instrs[m.break_idx])
    return out


def _rewrite_signextend_iv_to_zeroextend(
    body: list[tac_ast.Type_instruction], x_name: str,
) -> list[tac_ast.Type_instruction]:
    """Return a copy of `body` in which every
    `SignExtend(Var(x_name), dst)` is replaced with
    `ZeroExtend(Var(x_name), dst)`. Sound only when `x_name` is
    statically non-negative throughout the body — caller's
    responsibility to verify (the rotated countdown's `x_var`
    qualifies because init `>= 0`, post is `x -= 1`, and the
    test rejects negative on each iteration's exit edge).

    Other operands of SignExtend (anything other than `Var(x_name)`)
    pass through unchanged — the rewrite is opt-in by source
    operand, not blanket."""
    out: list[tac_ast.Type_instruction] = []
    for ins in body:
        if (
            isinstance(ins, tac_ast.SignExtend)
            and isinstance(ins.src, tac_ast.Var)
            and ins.src.name == x_name
        ):
            out.append(tac_ast.ZeroExtend(src=ins.src, dst=ins.dst))
        else:
            out.append(ins)
    return out
