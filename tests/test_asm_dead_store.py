"""Tests for `passes.asm_dead_store.apply_asm_dead_store`.

The pass drops or morphs Mov-into-memory atoms whose written byte
isn't observed by any subsequent instruction reachable in the CFG.
The CFG walk treats `Call` / `FunctionPrologue` / `AllocateStack`
as opaque (they may read any memory), but `LoadAddress` is modeled
precisely: it writes 2 bytes to `dst`, reads `FP` / `FP+1` if `src`
is `Frame`, and reads nothing if `src` is `Data` (link-time
immediates). Without precise modeling, any `STA DPTR` whose forward
walk happens to pass through a downstream `LoadAddress` (very
common when the next pointer-write reloads the indirect base via
`&static`) is conservatively kept LIVE, leaving the previous DPTR
stage as dead code.
"""

from __future__ import annotations

import unittest

import asm_ast
from passes.asm_dead_store import apply_asm_dead_store


_REG_A = asm_ast.Reg(reg=asm_ast.A())


def _fn(instrs):
    return asm_ast.Function(
        name="f", is_global=True, params=[], instructions=instrs,
    )


def _prog(instrs):
    return asm_ast.Program(top_level=[_fn(instrs)])


def _run(instrs, **kwargs):
    return apply_asm_dead_store(_prog(instrs), **kwargs).top_level[0].instructions


def _dptr(off):
    return asm_ast.Data(name="DPTR", offset=off)


def _data(name, off=0):
    return asm_ast.Data(name=name, offset=off)


class TestLoadAddressNotOpaque(unittest.TestCase):
    """`LoadAddress` is no longer in `_OPAQUE_TYPES`; the DSE walks
    past it under precise read/write modeling. These tests pin the
    behavior."""

    def test_dptr_stage_then_loadaddress_then_dptr_overwrite_is_dead(self):
        """The canonical floor_enemy_advance shape: stage DPTR from
        a ZP-resolved pair, then immediately overwrite that pair
        (via LoadAddress of a different static) and re-stage DPTR
        before the next indirect use. The FIRST stage's STA DPTR /
        STA DPTR+1 are dead because nothing reads DPTR before the
        kill at the second stage.

        Pre-fix: `LoadAddress` was opaque, so the DSE walk from the
        first STA DPTR hit the LoadAddress and returned LIVE,
        leaving the dead stores in place.
        """
        b0 = _data("b0", 0)
        b1 = _data("b1", 0)
        instrs = [
            # First DPTR stage from (b0, b1).
            asm_ast.Mov(src=b0, dst=_REG_A),
            asm_ast.Mov(src=_REG_A, dst=_dptr(0)),
            asm_ast.Mov(src=b1, dst=_REG_A),
            asm_ast.Mov(src=_REG_A, dst=_dptr(1)),
            # LoadAddress overwrites (b0, b0+1) with a new static's
            # 2-byte address. Reads no memory (Data src = link-time
            # immediates). Writes the b0 byte pair.
            asm_ast.LoadAddress(src=_data("enemy_col"), dst=b0),
            # Second DPTR stage from the new (b0, b1).
            asm_ast.Mov(src=b0, dst=_REG_A),
            asm_ast.Mov(src=_REG_A, dst=_dptr(0)),
            asm_ast.Mov(src=b1, dst=_REG_A),
            asm_ast.Mov(src=_REG_A, dst=_dptr(1)),
            # Live use of DPTR — keeps the second stage non-dead.
            asm_ast.Mov(src=asm_ast.Imm(0x99), dst=_REG_A),
            asm_ast.Mov(src=_REG_A, dst=asm_ast.IndirectY()),
            asm_ast.Return(save_a=False),
        ]
        out = _run(instrs)
        # The first stage's writes are killed before any read (the
        # LoadAddress doesn't read DPTR; the second stage's STAs
        # overwrite the same bytes). The second stage's writes are
        # live because the trailing `STA (DPTR),Y` reads them.
        dptr_stores = [
            i for i in out
            if isinstance(i, asm_ast.Mov)
            and isinstance(i.dst, asm_ast.Data)
            and i.dst.name == "DPTR"
        ]
        self.assertEqual(len(dptr_stores), 2)

    def test_loadaddress_frame_src_reads_fp(self):
        """A `LoadAddress(Frame, ...)` lowers to `LDA FP; ADC #off;
        ...; LDA FP+1; ADC #0; ...` so it reads `Data("FP", 0)` and
        `Data("FP", 1)`. A prior live store to either FP byte must
        not be dropped."""
        b0 = _data("b0", 0)
        # `STA FP; ...; LoadAddress(Frame(2), b0)` — FP write feeds
        # the LoadAddress, so it must survive DSE.
        instrs = [
            asm_ast.Mov(src=asm_ast.Imm(0xAB), dst=_REG_A),
            asm_ast.Mov(src=_REG_A, dst=_data("FP", 0)),
            asm_ast.LoadAddress(src=asm_ast.Frame(offset=2), dst=b0),
            asm_ast.Return(save_a=False),
        ]
        out = _run(instrs)
        # The STA FP must survive — LoadAddress reads it.
        fp_writes = [
            i for i in out
            if isinstance(i, asm_ast.Mov)
            and isinstance(i.dst, asm_ast.Data)
            and i.dst.name == "FP"
        ]
        self.assertEqual(len(fp_writes), 1)

    def test_loadaddress_data_src_does_not_read_fp(self):
        """`LoadAddress(Data, ...)` lowers to `LDA #<name; STA dst.lo;
        LDA #>name; STA dst.hi` — pure immediates, no memory reads.
        A prior dead store to FP whose only "use" in the walk is a
        downstream `LoadAddress(Data, _)` must be dropped (FP is
        callee-saved at the runtime level but a fresh write to it
        whose flow doesn't escape this function still counts as dead
        if no read observes it)."""
        b0 = _data("b0", 0)
        # `STA $80; LoadAddress(Data, b0); ZP $80 is dead-at-exit`
        # — the $80 store IS dead.
        zp80 = asm_ast.ZP(address=0x80, offset=0)
        instrs = [
            asm_ast.Mov(src=asm_ast.Imm(0xAB), dst=_REG_A),
            asm_ast.Mov(src=_REG_A, dst=zp80),
            asm_ast.LoadAddress(src=_data("enemy_col"), dst=b0),
            asm_ast.Return(save_a=False),
        ]
        out = _run(instrs)
        # The STA $80 must be gone — LoadAddress(Data) doesn't
        # read it, ZP-in-pool is dead-at-exit.
        zp_writes = [
            i for i in out
            if isinstance(i, asm_ast.Mov) and isinstance(i.dst, asm_ast.ZP)
        ]
        self.assertEqual(len(zp_writes), 0)

    def test_loadaddress_writes_kill_target_byte(self):
        """`LoadAddress(_, dst)` overwrites `dst` (low byte) — a
        prior STA to that exact byte is killed by the LoadAddress
        even though LoadAddress is no longer in `_OPAQUE_TYPES`.
        Surfaced through `_write_operand` returning `dst`."""
        b0 = _data("b0", 0)
        # `STA b0; LoadAddress(_, b0); Ret` — the first STA is
        # killed by the LoadAddress that follows.
        instrs = [
            asm_ast.Mov(src=asm_ast.Imm(0x42), dst=_REG_A),
            asm_ast.Mov(src=_REG_A, dst=b0),
            asm_ast.LoadAddress(src=_data("enemy_col"), dst=b0),
            asm_ast.Return(save_a=False),
        ]
        out = _run(instrs)
        # Only one Mov-to-memory survives (the LoadAddress's own
        # writes happen at emit time, not at the IR level).
        movs_to_b0 = [
            i for i in out
            if isinstance(i, asm_ast.Mov)
            and isinstance(i.dst, asm_ast.Data) and i.dst.name == "b0"
        ]
        self.assertEqual(len(movs_to_b0), 0)


class TestCallTransparentForNonAddressTakenLocal(unittest.TestCase):
    """A `Call` to a known function cannot read or write any
    `__local_<curfn>__*` byte that the function itself never
    constructs the address of (no `ImmLabelLow` / `ImmLabelHigh`
    referencing the slot name). The call-graph-disjoint allocator
    guarantees the callee's private pool and the caller's private
    pool are disjoint, and a slot whose address is never built
    cannot have been leaked to a callee through a pointer
    parameter.

    Motivating case (from `examples/companion_update.asm`'s
    `entity_proximity`): `apply_remat` rewrites a staged
    `LDA #<entity_row; STA __local_fn__0` / `LDA __local_fn__0;
    STA <callee_arg>` chain to recompute the immediate at the use
    site, leaving the original `STA __local_fn__0` dead. The
    `JSR <callee>` that follows blocks the within-function DSE
    walk; teaching DSE that the callee can't observe
    `__local_fn__0` lets the dead STA drop.
    """

    def _fn_named(self, name, instrs):
        return asm_ast.Program(top_level=[asm_ast.Function(
            name=name, is_global=True, params=[], instructions=instrs,
        )])

    def _run_named(self, name, instrs, **kwargs):
        prog = self._fn_named(name, instrs)
        return apply_asm_dead_store(prog, **kwargs).top_level[0].instructions

    def test_call_transparent_for_compiler_temp_local(self):
        """STA `__local_fn__0` (numeric-suffix temp; never
        address-taken) followed by `JSR callee` followed by a
        kill-overwrite of the same slot. The STA is dead because
        the call-graph-disjoint allocator's guarantee says
        `callee` cannot reach `__local_fn__0`."""
        slot = _data("__local_fn__0", 0)
        instrs = [
            # The dead STA.
            asm_ast.Mov(src=asm_ast.Imm(0x42), dst=_REG_A),
            asm_ast.Mov(src=_REG_A, dst=slot),
            # JSR — currently treated opaquely; should be
            # transparent for `__local_fn__0` (no ImmLabelLow /
            # ImmLabelHigh references this slot anywhere in `fn`).
            asm_ast.Call(name="callee"),
            # Kill: overwrite the same slot before reading.
            asm_ast.Mov(src=asm_ast.Imm(0x99), dst=_REG_A),
            asm_ast.Mov(src=_REG_A, dst=slot),
            # Read of the new value keeps the second STA live.
            asm_ast.Mov(src=slot, dst=_REG_A),
            asm_ast.Return(save_a=False),
        ]
        zp_slot_symbols = {"__local_fn__0": 0x8D}
        out = self._run_named(
            "fn", instrs, zp_slot_symbols=zp_slot_symbols,
        )
        stas_to_slot = [
            i for i in out
            if isinstance(i, asm_ast.Mov)
            and isinstance(i.dst, asm_ast.Data)
            and i.dst.name == "__local_fn__0"
        ]
        # Only the live STA survives (the kill-overwrite).
        self.assertEqual(len(stas_to_slot), 1)

    def test_call_not_transparent_for_address_taken_local(self):
        """STA `__local_fn__entity_row` followed by `JSR callee`
        followed by a kill-overwrite. The function elsewhere
        constructs the address of this slot via `ImmLabelLow /
        ImmLabelHigh`, so the callee may have received a pointer
        to it. The STA must NOT be dropped — the callee may
        observe its value."""
        slot = _data("__local_fn__entity_row", 0)
        out_lo = _data("__zpabi_callee__out_lo", 0)
        out_hi = _data("__zpabi_callee__out_hi", 0)
        instrs = [
            # The candidate STA (must survive).
            asm_ast.Mov(src=asm_ast.Imm(0x42), dst=_REG_A),
            asm_ast.Mov(src=_REG_A, dst=slot),
            # Construct the address of `slot` and pass it as a
            # pointer argument — this is the "address taken" leak.
            asm_ast.Mov(
                src=asm_ast.ImmLabelLow(
                    name="__local_fn__entity_row", offset=0,
                ),
                dst=_REG_A,
            ),
            asm_ast.Mov(src=_REG_A, dst=out_lo),
            asm_ast.Mov(
                src=asm_ast.ImmLabelHigh(
                    name="__local_fn__entity_row", offset=0,
                ),
                dst=_REG_A,
            ),
            asm_ast.Mov(src=_REG_A, dst=out_hi),
            asm_ast.Call(name="callee"),
            # Kill-overwrite of the slot.
            asm_ast.Mov(src=asm_ast.Imm(0x99), dst=_REG_A),
            asm_ast.Mov(src=_REG_A, dst=slot),
            asm_ast.Mov(src=slot, dst=_REG_A),
            asm_ast.Return(save_a=False),
        ]
        zp_slot_symbols = {"__local_fn__entity_row": 0x8F}
        out = self._run_named(
            "fn", instrs, zp_slot_symbols=zp_slot_symbols,
        )
        stas_to_slot = [
            i for i in out
            if isinstance(i, asm_ast.Mov)
            and isinstance(i.dst, asm_ast.Data)
            and i.dst.name == "__local_fn__entity_row"
        ]
        # Both STAs survive — the first is potentially observable
        # by the callee through the leaked pointer.
        self.assertEqual(len(stas_to_slot), 2)

    def test_call_transparent_for_source_named_non_address_taken(self):
        """A source-named slot whose address is never constructed
        anywhere in the function is treated the same as a numeric-
        suffix compiler temp — the callee can't reach it."""
        slot = _data("__local_fn__sprite_x", 0)
        instrs = [
            asm_ast.Mov(src=asm_ast.Imm(0x42), dst=_REG_A),
            asm_ast.Mov(src=_REG_A, dst=slot),
            asm_ast.Call(name="callee"),
            asm_ast.Mov(src=asm_ast.Imm(0x99), dst=_REG_A),
            asm_ast.Mov(src=_REG_A, dst=slot),
            asm_ast.Mov(src=slot, dst=_REG_A),
            asm_ast.Return(save_a=False),
        ]
        zp_slot_symbols = {"__local_fn__sprite_x": 0x90}
        out = self._run_named(
            "fn", instrs, zp_slot_symbols=zp_slot_symbols,
        )
        stas_to_slot = [
            i for i in out
            if isinstance(i, asm_ast.Mov)
            and isinstance(i.dst, asm_ast.Data)
            and i.dst.name == "__local_fn__sprite_x"
        ]
        self.assertEqual(len(stas_to_slot), 1)

    def test_call_not_transparent_for_zpabi_arg_slot(self):
        """`__zpabi_callee__*` slots are READ by the callee — they
        carry argument bytes. A prior STA to a `__zpabi_*` slot
        followed by the JSR is LIVE; the relaxation applies only
        to `__local_<curfn>__*` slots."""
        arg = _data("__zpabi_callee__p0", 0)
        instrs = [
            asm_ast.Mov(src=asm_ast.Imm(0x42), dst=_REG_A),
            asm_ast.Mov(src=_REG_A, dst=arg),
            asm_ast.Call(name="callee"),
            asm_ast.Return(save_a=False),
        ]
        zp_slot_symbols = {"__zpabi_callee__p0": 0x80}
        out = self._run_named(
            "fn", instrs, zp_slot_symbols=zp_slot_symbols,
        )
        stas_to_arg = [
            i for i in out
            if isinstance(i, asm_ast.Mov)
            and isinstance(i.dst, asm_ast.Data)
            and i.dst.name == "__zpabi_callee__p0"
        ]
        self.assertEqual(len(stas_to_arg), 1)

    def test_call_not_transparent_for_other_function_local(self):
        """A slot belonging to a DIFFERENT function
        (`__local_other__0` when we're inside `fn`) is not in
        `fn`'s private pool — the relaxation can't assume the
        callee respects it. Conservative: stay LIVE."""
        slot = _data("__local_other__0", 0)
        instrs = [
            asm_ast.Mov(src=asm_ast.Imm(0x42), dst=_REG_A),
            asm_ast.Mov(src=_REG_A, dst=slot),
            asm_ast.Call(name="callee"),
            asm_ast.Mov(src=asm_ast.Imm(0x99), dst=_REG_A),
            asm_ast.Mov(src=_REG_A, dst=slot),
            asm_ast.Mov(src=slot, dst=_REG_A),
            asm_ast.Return(save_a=False),
        ]
        zp_slot_symbols = {"__local_other__0": 0x90}
        out = self._run_named(
            "fn", instrs, zp_slot_symbols=zp_slot_symbols,
        )
        stas_to_slot = [
            i for i in out
            if isinstance(i, asm_ast.Mov)
            and isinstance(i.dst, asm_ast.Data)
            and i.dst.name == "__local_other__0"
        ]
        # Both STAs stay live — the callee may touch the other
        # function's slot, since we have no allocator guarantee
        # about it from `fn`'s perspective.
        self.assertEqual(len(stas_to_slot), 2)

    def test_icall_remains_opaque_even_for_local(self):
        """The `icall` trampoline (`JMP (DPTR)`) dispatches to an
        unknown callee — the allocator guarantee doesn't apply.
        STA to `__local_fn__0` before a `Call(name="icall")` must
        stay LIVE."""
        slot = _data("__local_fn__0", 0)
        instrs = [
            asm_ast.Mov(src=asm_ast.Imm(0x42), dst=_REG_A),
            asm_ast.Mov(src=_REG_A, dst=slot),
            asm_ast.Call(name="icall"),
            asm_ast.Mov(src=asm_ast.Imm(0x99), dst=_REG_A),
            asm_ast.Mov(src=_REG_A, dst=slot),
            asm_ast.Mov(src=slot, dst=_REG_A),
            asm_ast.Return(save_a=False),
        ]
        zp_slot_symbols = {"__local_fn__0": 0x8D}
        out = self._run_named(
            "fn", instrs, zp_slot_symbols=zp_slot_symbols,
        )
        stas_to_slot = [
            i for i in out
            if isinstance(i, asm_ast.Mov)
            and isinstance(i.dst, asm_ast.Data)
            and i.dst.name == "__local_fn__0"
        ]
        self.assertEqual(len(stas_to_slot), 2)


if __name__ == "__main__":
    unittest.main()
