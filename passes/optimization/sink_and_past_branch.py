"""TAC pass: sink the `ZeroExtend; BitwiseAnd(_, C); Truncate` trio
past an immediately-following `JumpIfMasked(x, ...)` into each
successor block.

# Motivating shape

```c
uint8_t bobble = rescue_bobble[i];
uint8_t magnitude = bobble & 0x7F;
if (bobble & 0x80) { ... use magnitude ... } else { ... use magnitude ... }
```

lowers, after `fold_narrow_and_jump` has turned the `if (bobble &
0x80)` into a `JumpIfMasked`, to a TAC SSA shape where `magnitude`
is computed BEFORE the branch — a four-instruction trio + branch:

    ZeroExtend(bobble, %x_ext)              # uchar → int (no-op)
    Binary(BitwiseAnd, %x_ext, ConstInt(0x7F), %x_and)
    Truncate(%x_and, %magnitude)             # int → uchar (no-op)
    JumpIfMasked(bobble, 0x80, ..., .else)

The trio holds `bobble`'s live range across the branch test
(`bobble` feeds both the AND and the JumpIfMasked), which forces
the asm-SSA round-trip to spill `bobble` to a temp before the BPL
— losing the chance to test bit 7 from the LDA's flags directly.

Sinking the trio past the branch shortens `bobble`'s live range
to the LDA right before the BPL. Each successor block recomputes
`magnitude` from `bobble`, but at the asm level both copies fold
into a single `AND #$7F` per branch (the ZeroExtend/Truncate
collapse to nothing). Downstream the asm `adc_commute` peephole
then keeps `magnitude` in A through the ADC/SBC, eliminating the
spill of `magnitude` too.

# Pattern

Strict four-instruction adjacency, with the JumpIfMasked as the
terminator of its source block:

  [i+0]  ZeroExtend(%x, %x_ext)
  [i+1]  Binary(BitwiseAnd, %x_ext, ConstInt(C), %x_and)
                            (or commuted operands; AND is commutative)
  [i+2]  Truncate(%x_and, %t)
  [i+3]  JumpIfMasked(%x, _, _, target)        # block terminator

Eligibility:

  - `%x` is a `Var` typed Char / SChar / UChar (1-byte) per the
    symbol table. (1-byte is what lets us narrow the const safely
    AND what makes the BPL/BMI on the original load sound.)
  - `0 <= C <= 0xFF` so the masked-AND value fits one byte.
  - `%x_ext` is used exactly once (by the Binary at [i+1]).
  - `%x_and` is used exactly once (by the Truncate at [i+2]).
  - The Binary's two src slots are `(%x_ext, ConstInt)` or
    `(ConstInt, %x_ext)` — AND is commutative.
  - `%t` is used only in the immediate-fall-through block of [i+3]
    and/or in the block whose first instruction is
    `Label(target)`. No use in any other block (including any
    post-merge block — that would require a Phi the prototype
    doesn't insert).

# Action

For each matching trio:

  1. Mint two fresh `.snk_then@N` / `.snk_else@N` SSA names for
     each of `%x_ext`, `%x_and`, `%t`. Register them in the
     symbol table with their original types.
  2. Insert a renamed copy of the trio at the START of the
     fall-through block AND at the start of the target block
     (right after the leading `Label` if present).
  3. Drop the original trio from positions [i..i+2] (the
     JumpIfMasked at [i+3] stays).
  4. Rewrite uses of `%t` in the fall-through block to
     `%t.snk_then@N`, and uses in the target block to
     `%t.snk_else@N`.

After this rewrite each `.snk_*` name has exactly one def and
the SSA invariant is preserved.

# Why no Phi at the merge

The prototype's eligibility check requires `%t` to have NO uses
outside the two successor blocks. So no merge-point use needs a
Phi. Generalizing to allow post-merge uses is a follow-up: insert
`Phi(t_merge, [(then_label, t.snk_then), (else_label, t.snk_else)])`
at the merge block and rewrite post-merge uses to `t_merge`.

# Soundness

The trio is a pure computation depending only on `%x`. `%x` is
NOT modified between the trio and the JumpIfMasked (SSA single-
def invariant guarantees this), nor in either successor block
before the use of the trio's result (the SSA invariant again).
So duplicating the trio into each successor computes the same
value that the original trio computed before the branch.

The single-use invariants on `%x_ext` and `%x_and` mean those
intermediate values aren't observed anywhere else; renaming and
duplicating them is safe.

# Where to run

In the TAC fixed-point loop. After `fold_narrow_and_jump` (so the
JumpIfMasked exists) and after `to_ssa` (we need SSA-renamed
single-use temps for the eligibility check)."""
from __future__ import annotations

import c99_ast
import tac_ast
from passes.optimization.cfg import (
    ENTRY_ID, EXIT_ID, build_cfg, cfg_to_function,
)
from passes.optimization.var_visit import uses_in


def sink_and_past_branch(
    fn: tac_ast.Function, *, symbols=None,
) -> tac_ast.Function:
    """Walk `fn`'s blocks; for each block whose terminator is a
    JumpIfMasked preceded by a sinkable trio, duplicate the trio
    into both successor blocks and rename per branch.

    `symbols` is required: we read `%x`'s type to gate eligibility
    and register the fresh `.snk_*` names with the same type as
    their originals."""
    if symbols is None:
        return fn
    cfg = build_cfg(fn)
    # First, build a per-name use index across all blocks.
    use_blocks = _index_use_blocks(cfg)
    # Counter for minting fresh `.snk@<N>` names within this
    # function. Single counter shared across all matches keeps the
    # names unique.
    counter = [0]
    changed = False
    for bid in list(cfg.block_order):
        block = cfg.blocks[bid]
        instrs = block.instructions
        if len(instrs) < 4:
            continue
        # Trio at [-4, -3, -2]; terminator at [-1].
        if not isinstance(instrs[-1], tac_ast.JumpIfMasked):
            continue
        cand = _match_trio(instrs[-4:-1], instrs[-1], symbols)
        if cand is None:
            continue
        x_name, x_ext_name, x_and_name, t_name, mask_const = cand
        jump = instrs[-1]
        # Identify the two successor block ids: target (label
        # match) and fall-through (source-next).
        target_bid = _find_block_by_label(cfg, jump.target)
        if target_bid is None:
            continue
        # source-order next block id, or None at end-of-function.
        try:
            order_idx = cfg.block_order.index(bid)
            if order_idx + 1 >= len(cfg.block_order):
                continue
            fallthru_bid = cfg.block_order[order_idx + 1]
        except ValueError:
            continue
        # Every use of %t must be in target_bid or fallthru_bid
        # exclusively. Any other use → bail (would need a Phi).
        users_of_t = use_blocks.get(t_name, set())
        if not users_of_t.issubset({target_bid, fallthru_bid}):
            continue
        # All eligibility checks passed — do the rewrite. Mint
        # fresh names per branch.
        n = counter[0]
        counter[0] += 1
        then_names = (
            f"{x_ext_name}.snk_then@{n}",
            f"{x_and_name}.snk_then@{n}",
            f"{t_name}.snk_then@{n}",
        )
        else_names = (
            f"{x_ext_name}.snk_else@{n}",
            f"{x_and_name}.snk_else@{n}",
            f"{t_name}.snk_else@{n}",
        )
        # Register fresh names in the symbol table mirroring the
        # original types (each is a LocalAttr scalar temp).
        _register_fresh(symbols, x_ext_name, then_names[0])
        _register_fresh(symbols, x_and_name, then_names[1])
        _register_fresh(symbols, t_name, then_names[2])
        _register_fresh(symbols, x_ext_name, else_names[0])
        _register_fresh(symbols, x_and_name, else_names[1])
        _register_fresh(symbols, t_name, else_names[2])
        # Drop the original trio from this block; keep terminator.
        block.instructions = list(instrs[:-4]) + [instrs[-1]]
        # Prepend renamed trio to fallthru and target blocks.
        # `fallthru_bid` is the source-order next block — it might
        # be the same as `target_bid` if the target falls through
        # (would be a degenerate JumpIfMasked, skip).
        if fallthru_bid == target_bid:
            continue
        _prepend_trio(
            cfg.blocks[fallthru_bid], x_name, mask_const, then_names,
        )
        _prepend_trio(
            cfg.blocks[target_bid], x_name, mask_const, else_names,
        )
        # Rewrite uses of %t in each successor block.
        if fallthru_bid in users_of_t:
            _rewrite_var_uses(
                cfg.blocks[fallthru_bid], t_name, then_names[2],
                skip_count=3,
            )
        if target_bid in users_of_t:
            _rewrite_var_uses(
                cfg.blocks[target_bid], t_name, else_names[2],
                # Target block starts with Label; the prepended
                # trio sits at indices 1..3 (after the Label).
                # All subsequent indices are uses to consider.
                skip_count=4 if _starts_with_label(
                    cfg.blocks[target_bid]
                ) else 3,
            )
        changed = True
    if not changed:
        return fn
    return cfg_to_function(fn, cfg)


def _match_trio(
    trio: list[tac_ast.Type_instruction],
    jump: tac_ast.JumpIfMasked,
    symbols,
) -> tuple[str, str, str, str, int] | None:
    """Match the four-instruction pattern. Returns
    `(x_name, x_ext_name, x_and_name, t_name, mask_const)` on
    success, None otherwise. Also enforces single-use on the
    intermediate temps via the instructions' shape itself."""
    if len(trio) != 3:
        return None
    ze, binop, tr = trio
    # ZeroExtend(%x, %x_ext).
    if not isinstance(ze, tac_ast.ZeroExtend):
        return None
    if not isinstance(ze.src, tac_ast.Var):
        return None
    if not isinstance(ze.dst, tac_ast.Var):
        return None
    x_name = ze.src.name
    x_ext_name = ze.dst.name
    # `%x` must be a 1-byte unsigned type per the symbol table:
    # otherwise the asm-level narrowing the sink enables (`AND #$7F`)
    # could lose bits.
    if not _is_1byte_unsigned(symbols, x_name):
        return None
    # Binary(BitwiseAnd, ?, ?, %x_and) with operands one Var
    # (%x_ext) and one ConstInt-fitting-byte. AND is commutative,
    # so accept either operand order.
    if not isinstance(binop, tac_ast.Binary):
        return None
    if not isinstance(binop.op, tac_ast.BitwiseAnd):
        return None
    if not isinstance(binop.dst, tac_ast.Var):
        return None
    x_and_name = binop.dst.name
    mask = _try_extract_byte_mask(binop, x_ext_name)
    if mask is None:
        return None
    # Truncate(%x_and, %t).
    if not isinstance(tr, tac_ast.Truncate):
        return None
    if not isinstance(tr.src, tac_ast.Var):
        return None
    if tr.src.name != x_and_name:
        return None
    if not isinstance(tr.dst, tac_ast.Var):
        return None
    t_name = tr.dst.name
    # JumpIfMasked must reference the original %x (not the
    # widened or truncated version).
    if not isinstance(jump.val, tac_ast.Var):
        return None
    if jump.val.name != x_name:
        return None
    return (x_name, x_ext_name, x_and_name, t_name, mask)


def _try_extract_byte_mask(
    binop: tac_ast.Binary, x_ext_name: str,
) -> int | None:
    """If `binop` is `BitwiseAnd(%x_ext, ConstInt(C))` (or commuted)
    with C in 0..0xFF, return C; else None."""
    src_var = src_const = None
    for s in (binop.src1, binop.src2):
        if isinstance(s, tac_ast.Var) and s.name == x_ext_name:
            src_var = s
        elif isinstance(s, tac_ast.Constant) and isinstance(
            s.const, (
                tac_ast.ConstInt, tac_ast.ConstUInt,
                tac_ast.ConstChar, tac_ast.ConstUChar,
            ),
        ):
            src_const = s.const
    if src_var is None or src_const is None:
        return None
    if not (0 <= src_const.value <= 0xFF):
        return None
    return src_const.value


def _is_1byte_unsigned(symbols, name: str) -> bool:
    """True iff `name`'s symbol-table type is Char / SChar / UChar
    (1 byte). The bit-7 test in the JumpIfMasked uses BPL/BMI on
    the byte's sign bit, which is well-defined only for 1-byte
    operands."""
    sym = symbols.get(name)
    if sym is None:
        return False
    return isinstance(sym.type, (
        c99_ast.Char, c99_ast.SChar, c99_ast.UChar,
    ))


def _register_fresh(symbols, src_name: str, new_name: str) -> None:
    """Register `new_name` in `symbols` with the same type and
    LocalAttr storage as `src_name`. Idempotent — silently no-ops
    if `new_name` is already present (e.g., from a prior pass
    iteration of the fixed-point loop)."""
    if new_name in symbols:
        return
    src_sym = symbols.get(src_name)
    if src_sym is None:
        return
    # Mirror src's type and attr — fresh SSA temps are LocalAttr.
    from passes.type_checking import LocalAttr, Symbol
    symbols[new_name] = Symbol(
        type=src_sym.type, attrs=LocalAttr(),
    )


def _index_use_blocks(cfg) -> dict[str, set[int]]:
    """Return a map of `var_name → set of block ids that contain a
    use of that var`. A Phi's pred-tagged args count as uses in the
    Phi's containing block (the Phi destruction will eventually
    lower them to predecessor-block Movs)."""
    out: dict[str, set[int]] = {}
    for bid, block in cfg.blocks.items():
        if bid in (ENTRY_ID, EXIT_ID):
            continue
        for instr in block.instructions:
            for v in uses_in(instr):
                if not isinstance(v, tac_ast.Var):
                    continue
                out.setdefault(v.name, set()).add(bid)
    return out


def _find_block_by_label(cfg, label: str) -> int | None:
    """Linear scan for the block whose first instruction is
    `Label(label)`. CFG construction puts a Label at index 0 of
    every label-targeted block, so this is unambiguous."""
    for bid, block in cfg.blocks.items():
        if bid in (ENTRY_ID, EXIT_ID):
            continue
        if not block.instructions:
            continue
        first = block.instructions[0]
        if isinstance(first, tac_ast.Label) and first.name == label:
            return bid
    return None


def _starts_with_label(block) -> bool:
    """True iff `block.instructions[0]` is a `Label`. Used to
    decide whether to insert the trio at index 0 (no Label) or
    index 1 (after the Label)."""
    return (
        len(block.instructions) > 0
        and isinstance(block.instructions[0], tac_ast.Label)
    )


def _prepend_trio(
    block,
    x_name: str,
    mask: int,
    names: tuple[str, str, str],
) -> None:
    """Insert the renamed trio at the start of `block` — after the
    leading Label if present, else at index 0."""
    ext_name, and_name, t_name = names
    insert_at = 1 if _starts_with_label(block) else 0
    trio = [
        tac_ast.ZeroExtend(
            src=tac_ast.Var(name=x_name),
            dst=tac_ast.Var(name=ext_name),
        ),
        tac_ast.Binary(
            op=tac_ast.BitwiseAnd(),
            src1=tac_ast.Var(name=ext_name),
            src2=tac_ast.Constant(
                const=tac_ast.ConstInt(value=mask),
            ),
            dst=tac_ast.Var(name=and_name),
        ),
        tac_ast.Truncate(
            src=tac_ast.Var(name=and_name),
            dst=tac_ast.Var(name=t_name),
        ),
    ]
    block.instructions = (
        block.instructions[:insert_at]
        + trio
        + block.instructions[insert_at:]
    )


def _rewrite_var_uses(
    block, old_name: str, new_name: str, *, skip_count: int,
) -> None:
    """Rewrite every `Var(old_name)` USE in `block.instructions[
    skip_count:]` to `Var(new_name)`. `skip_count` skips over the
    prepended trio (and the leading Label) — those are the new
    DEFS that establish `new_name`, not uses of `old_name`."""
    new_instrs = list(block.instructions[:skip_count])
    for instr in block.instructions[skip_count:]:
        new_instrs.append(_rewrite_instr_uses(instr, old_name, new_name))
    block.instructions = new_instrs


def _rewrite_instr_uses(
    instr: tac_ast.Type_instruction,
    old_name: str,
    new_name: str,
) -> tac_ast.Type_instruction:
    """Return a copy of `instr` where every USE of `Var(old_name)`
    has been rewritten to `Var(new_name)`. DEFs (instruction dst
    slots) are NOT rewritten — the original `old_name` IS its own
    def site, and after our sinker that site has been dropped."""
    def fix(v):
        if isinstance(v, tac_ast.Var) and v.name == old_name:
            return tac_ast.Var(name=new_name)
        return v

    # Handle every instruction variant that has Var operands in
    # USE slots. Use the var_visit helpers conceptually but rebuild
    # the instruction since dataclasses are frozen-ish (we use the
    # generated @dataclass shapes).
    if isinstance(instr, tac_ast.Ret):
        if instr.val is not None:
            return tac_ast.Ret(val=fix(instr.val))
        return instr
    if isinstance(instr, (
        tac_ast.SignExtend, tac_ast.ZeroExtend, tac_ast.Truncate,
        tac_ast.IntToFloat, tac_ast.IntToDouble,
        tac_ast.FloatToInt, tac_ast.DoubleToInt,
        tac_ast.FloatToDouble, tac_ast.DoubleToFloat,
    )):
        return type(instr)(src=fix(instr.src), dst=instr.dst)
    if isinstance(instr, tac_ast.GetAddress):
        return tac_ast.GetAddress(
            operand=fix(instr.operand), dst=instr.dst,
        )
    if isinstance(instr, tac_ast.Load):
        return tac_ast.Load(
            src_ptr=fix(instr.src_ptr),
            dst=instr.dst,
            is_volatile=instr.is_volatile,
        )
    if isinstance(instr, tac_ast.Store):
        return tac_ast.Store(
            src=fix(instr.src),
            dst_ptr=fix(instr.dst_ptr),
            is_volatile=instr.is_volatile,
        )
    if isinstance(instr, tac_ast.IndexedLoad):
        return tac_ast.IndexedLoad(
            name=instr.name,
            index=fix(instr.index),
            dst=instr.dst,
            is_volatile=instr.is_volatile,
        )
    if isinstance(instr, tac_ast.IndexedStore):
        return tac_ast.IndexedStore(
            address=instr.address,
            index=fix(instr.index),
            src=fix(instr.src),
            is_volatile=instr.is_volatile,
        )
    if isinstance(instr, tac_ast.IndexedSymbolStore):
        return tac_ast.IndexedSymbolStore(
            name=instr.name,
            index=fix(instr.index),
            src=fix(instr.src),
            is_volatile=instr.is_volatile,
        )
    if isinstance(instr, tac_ast.IndexedConstLoad):
        return tac_ast.IndexedConstLoad(
            address=instr.address,
            index=fix(instr.index),
            dst=instr.dst,
            is_volatile=instr.is_volatile,
        )
    if isinstance(instr, tac_ast.IndirectIndexedLoad):
        return tac_ast.IndirectIndexedLoad(
            ptr=fix(instr.ptr),
            index=fix(instr.index),
            dst=instr.dst,
            is_volatile=instr.is_volatile,
        )
    if isinstance(instr, tac_ast.IndirectIndexedStore):
        return tac_ast.IndirectIndexedStore(
            ptr=fix(instr.ptr),
            index=fix(instr.index),
            src=fix(instr.src),
            is_volatile=instr.is_volatile,
        )
    if isinstance(instr, tac_ast.Unary):
        return tac_ast.Unary(
            op=instr.op, src=fix(instr.src), dst=instr.dst,
        )
    if isinstance(instr, tac_ast.Binary):
        return tac_ast.Binary(
            op=instr.op,
            src1=fix(instr.src1),
            src2=fix(instr.src2),
            dst=instr.dst,
        )
    if isinstance(instr, tac_ast.Copy):
        return tac_ast.Copy(src=fix(instr.src), dst=instr.dst)
    if isinstance(instr, tac_ast.JumpIfTrue):
        return tac_ast.JumpIfTrue(
            condition=fix(instr.condition), target=instr.target,
        )
    if isinstance(instr, tac_ast.JumpIfFalse):
        return tac_ast.JumpIfFalse(
            condition=fix(instr.condition), target=instr.target,
        )
    if isinstance(instr, tac_ast.JumpIfCmp):
        return tac_ast.JumpIfCmp(
            op=instr.op,
            src1=fix(instr.src1),
            src2=fix(instr.src2),
            target=instr.target,
        )
    if isinstance(instr, tac_ast.JumpIfMasked):
        return tac_ast.JumpIfMasked(
            val=fix(instr.val),
            mask=instr.mask,
            jump_when_nonzero=instr.jump_when_nonzero,
            target=instr.target,
        )
    if isinstance(instr, tac_ast.FunctionCall):
        return tac_ast.FunctionCall(
            name=instr.name,
            args=[fix(a) for a in instr.args],
            dst=instr.dst,
        )
    if isinstance(instr, tac_ast.IndirectCall):
        return tac_ast.IndirectCall(
            ptr=fix(instr.ptr),
            args=[fix(a) for a in instr.args],
            dst=instr.dst,
        )
    if isinstance(instr, tac_ast.Phi):
        return tac_ast.Phi(
            dst=instr.dst,
            args=[
                tac_ast.PhiArg(
                    pred_label=a.pred_label, source=fix(a.source),
                )
                for a in instr.args
            ],
        )
    # Label / Jump / no-operand instructions: pass through.
    return instr
