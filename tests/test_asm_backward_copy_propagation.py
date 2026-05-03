"""Behavioral tests for `passes.optimization_asm.backward_copy_propagation`.

The pass collapses the asm-level pattern

    Mov(Reg(A), Pseudo P)        # def of P
    ... region R ...
    Mov(Pseudo P, Reg(A))        # last use of P
    Mov(Reg(A), D)               # immediately following

into

    Mov(Reg(A), D)               # relocated def
    ... region R ...
    [pair deleted]

Coverage:
  - User's exact pattern (two-byte Long return through HARGS):
    both bytes get rewritten.
  - Single-use precondition: multi-use P → no rewrite.
  - D shape: Pseudo D is rejected (different optimization).
  - Region-R safety: Call in R → no rewrite; aliased Data write
    → no rewrite; Label between → no rewrite.
  - A-liveness: A live after the pair → no rewrite.
  - Flag liveness: Branch right after the pair → no rewrite.
  - Statics: P named in `statics` → no rewrite.
  - Address-taken P: excluded.
  - Idempotent: a function with no candidates passes through.
"""
from __future__ import annotations

import unittest

import asm_ast
from passes.optimization_asm.backward_copy_propagation import (
    backward_copy_propagate,
)


def _imm(v: int) -> asm_ast.Imm:
    return asm_ast.Imm(value=v)


def _A() -> asm_ast.Reg:
    return asm_ast.Reg(reg=asm_ast.A())


def _ps(name: str, off: int = 0) -> asm_ast.Pseudo:
    return asm_ast.Pseudo(name=name, offset=off)


def _data(name: str, off: int = 0) -> asm_ast.Data:
    return asm_ast.Data(name=name, offset=off)


def _stack(off: int) -> asm_ast.Stack:
    return asm_ast.Stack(offset=off)


def _mov(src, dst) -> asm_ast.Mov:
    return asm_ast.Mov(src=src, dst=dst)


def _ret(save_a: bool = False) -> asm_ast.Return:
    return asm_ast.Return(save_a=save_a)


def _fn(*instrs, name: str = "main", params=()) -> asm_ast.Function:
    return asm_ast.Function(
        name=name,
        is_global=True,
        params=list(params),
        instructions=list(instrs),
    )


class TestBackwardCopyPropagation(unittest.TestCase):
    def test_user_two_byte_pattern(self) -> None:
        # Recreates the exact asm-SSA shape the user pointed at:
        #   Mov(@x.b0, A); ClearCarry; Add(@y.b0, A);
        #   Mov(A, %t.b0);                          ← def %t.b0
        #   Mov(@x.b1, A); Add(@y.b1, A);
        #   Mov(A, %t.b1);                          ← def %t.b1
        #   Mov(%t.b0, A); Mov(A, HARGS+0);          ← pair for %t.b0
        #   Mov(%t.b1, A); Mov(A, HARGS+1);          ← pair for %t.b1
        #   Ret(save_a=False)
        fn = _fn(
            _mov(_ps("@x", 0), _A()),
            asm_ast.ClearCarry(),
            asm_ast.Add(src=_ps("@y", 0), dst=_A()),
            _mov(_A(), _ps("%t.b0.v1")),
            _mov(_ps("@x", 1), _A()),
            asm_ast.Add(src=_ps("@y", 1), dst=_A()),
            _mov(_A(), _ps("%t.b1.v1")),
            _mov(_ps("%t.b0.v1"), _A()),
            _mov(_A(), _data("HARGS", 0)),
            _mov(_ps("%t.b1.v1"), _A()),
            _mov(_A(), _data("HARGS", 1)),
            _ret(save_a=False),
        )
        out = backward_copy_propagate(fn)
        # Both round-trips eliminated; the two %t.bN.v1 defs now
        # write HARGS directly. The final instruction stream:
        #   Mov(@x.b0, A); ClearCarry; Add(@y.b0, A);
        #   Mov(A, HARGS+0);
        #   Mov(@x.b1, A); Add(@y.b1, A);
        #   Mov(A, HARGS+1);
        #   Return
        self.assertEqual(len(out.instructions), 8)
        # No Pseudo named %t.b0.v1 or %t.b1.v1 anywhere.
        for instr in out.instructions:
            for op in (
                getattr(instr, "src", None),
                getattr(instr, "dst", None),
            ):
                if isinstance(op, asm_ast.Pseudo):
                    self.assertNotIn(
                        op.name, {"%t.b0.v1", "%t.b1.v1"},
                    )
        # Two Movs to HARGS now exist.
        movs_to_hargs = [
            i for i in out.instructions
            if isinstance(i, asm_ast.Mov)
            and isinstance(i.dst, asm_ast.Data)
            and i.dst.name == "HARGS"
        ]
        self.assertEqual(len(movs_to_hargs), 2)
        # And both of them write Reg(A).
        for m in movs_to_hargs:
            self.assertEqual(m.src, _A())

    def test_multi_use_p_rejects(self) -> None:
        # %p is read TWICE — once into A then to D, but also into A
        # then to a different sink. The pass must not collapse: %p
        # has more than one observer of its value.
        fn = _fn(
            _mov(_A(), _ps("%p")),
            _mov(_ps("%p"), _A()),
            _mov(_A(), _data("HARGS", 0)),
            _mov(_ps("%p"), _A()),
            _mov(_A(), _data("HARGS", 1)),
            _ret(save_a=False),
        )
        out = backward_copy_propagate(fn)
        # Unchanged.
        self.assertEqual(out.instructions, fn.instructions)

    def test_pseudo_d_rejected(self) -> None:
        # The pair's second Mov writes to ANOTHER Pseudo, not to a
        # non-Pseudo memory cell. Pseudo→Pseudo merging is regalloc's
        # job; this pass leaves it alone.
        fn = _fn(
            _mov(_A(), _ps("%p")),
            _mov(_ps("%p"), _A()),
            _mov(_A(), _ps("%q")),
            _ret(save_a=False),
        )
        out = backward_copy_propagate(fn)
        self.assertEqual(out.instructions, fn.instructions)

    def test_call_in_region_rejects(self) -> None:
        # A Call appears between %p's def and its single use. Calls
        # may clobber HARGS (runtime helpers exchange args there),
        # so the pass must not relocate %p's def into HARGS.
        fn = _fn(
            _mov(_A(), _ps("%p")),
            asm_ast.Call(name="some_helper"),
            _mov(_ps("%p"), _A()),
            _mov(_A(), _data("HARGS", 0)),
            _ret(save_a=False),
        )
        out = backward_copy_propagate(fn)
        self.assertEqual(out.instructions, fn.instructions)

    def test_aliased_write_in_region_rejects(self) -> None:
        # Region R contains a Mov that writes Data(HARGS, 0) — which
        # is exactly D. Relocating %p's def earlier would clobber
        # whatever that other Mov produced, then the other Mov would
        # overwrite our value before we're done. Reject.
        fn = _fn(
            _mov(_A(), _ps("%p")),
            _mov(_imm(0), _data("HARGS", 0)),  # writes D
            _mov(_ps("%p"), _A()),
            _mov(_A(), _data("HARGS", 0)),
            _ret(save_a=False),
        )
        out = backward_copy_propagate(fn)
        self.assertEqual(out.instructions, fn.instructions)

    def test_aliased_read_in_region_rejects(self) -> None:
        # Region R reads D — the read would observe `D`'s OLD value,
        # but if we relocate the def earlier, the read sees the new
        # value. Reject.
        fn = _fn(
            _mov(_A(), _ps("%p")),
            _mov(_data("HARGS", 0), _A()),  # reads D
            _mov(_ps("%p"), _A()),
            _mov(_A(), _data("HARGS", 0)),
            _ret(save_a=False),
        )
        out = backward_copy_propagate(fn)
        self.assertEqual(out.instructions, fn.instructions)

    def test_label_in_region_rejects(self) -> None:
        # def and use straddle a basic-block boundary. The linear
        # walk doesn't model the jumps that may target the inner
        # label, so the pass conservatively bails.
        fn = _fn(
            _mov(_A(), _ps("%p")),
            asm_ast.Label(name="L"),
            _mov(_ps("%p"), _A()),
            _mov(_A(), _data("HARGS", 0)),
            _ret(save_a=False),
        )
        out = backward_copy_propagate(fn)
        self.assertEqual(out.instructions, fn.instructions)

    def test_a_live_after_pair_rejects(self) -> None:
        # After the pair, A is read by Add(_, A) before being
        # overwritten. Deleting the `Mov(P, A); Mov(A, D)` pair
        # would change the value Add sees for A.
        fn = _fn(
            _mov(_A(), _ps("%p")),
            _mov(_ps("%p"), _A()),
            _mov(_A(), _data("HARGS", 0)),
            asm_ast.Add(src=_imm(1), dst=_A()),
            _mov(_A(), _data("OUT", 0)),
            _ret(save_a=False),
        )
        out = backward_copy_propagate(fn)
        self.assertEqual(out.instructions, fn.instructions)

    def test_flags_live_after_pair_rejects(self) -> None:
        # The Mov(P, A) at the start of the pair sets N/Z (LDA).
        # If a Branch follows the pair without an intervening
        # flag-setter, deleting the pair changes the flags the
        # Branch observes.
        fn = _fn(
            _mov(_A(), _ps("%p")),
            _mov(_ps("%p"), _A()),
            _mov(_A(), _data("HARGS", 0)),
            asm_ast.Branch(
                cond=asm_ast.EQ(), target="some_label",
            ),
            asm_ast.Label(name="some_label"),
            _ret(save_a=False),
        )
        out = backward_copy_propagate(fn)
        self.assertEqual(out.instructions, fn.instructions)

    def test_static_p_rejected(self) -> None:
        # P is named in `statics` — its writes are externally
        # observable, so the single-use invariant doesn't hold and
        # the pass must not collapse the round-trip.
        fn = _fn(
            _mov(_A(), _ps("g")),
            _mov(_ps("g"), _A()),
            _mov(_A(), _data("HARGS", 0)),
            _ret(save_a=False),
        )
        out = backward_copy_propagate(fn, statics=frozenset({"g"}))
        self.assertEqual(out.instructions, fn.instructions)

    def test_address_taken_p_rejected(self) -> None:
        # &%p is taken via LoadAddress somewhere. The pass excludes
        # %p since its value can be modified through the pointer,
        # making the single-use precondition unsound.
        fn = _fn(
            asm_ast.LoadAddress(src=_ps("%p"), dst=_ps("%addr")),
            _mov(_A(), _ps("%p")),
            _mov(_ps("%p"), _A()),
            _mov(_A(), _data("HARGS", 0)),
            _ret(save_a=False),
        )
        out = backward_copy_propagate(fn)
        self.assertEqual(out.instructions, fn.instructions)

    def test_def_must_be_canonical_form(self) -> None:
        # %p's def is `Mov(Imm, %p)` (not `Mov(Reg(A), %p)`). The
        # pass restricts the def shape to keep the rewrite
        # straightforward; it bails when the def isn't canonical.
        fn = _fn(
            _mov(_imm(0x42), _ps("%p")),
            _mov(_ps("%p"), _A()),
            _mov(_A(), _data("HARGS", 0)),
            _ret(save_a=False),
        )
        out = backward_copy_propagate(fn)
        self.assertEqual(out.instructions, fn.instructions)

    def test_no_candidates_returns_unchanged(self) -> None:
        # Function with no Mov(P, A); Mov(A, mem) pairs. Should
        # pass through unchanged.
        fn = _fn(
            _mov(_imm(0), _data("HARGS", 0)),
            _ret(save_a=False),
        )
        out = backward_copy_propagate(fn)
        self.assertEqual(out.instructions, fn.instructions)

    def test_single_byte_pattern_to_data(self) -> None:
        # Smallest "win": one Pseudo, single-use, deposited into a
        # Data slot. Verify the def site changes and the pair is
        # gone.
        fn = _fn(
            _mov(_A(), _ps("%p")),
            _mov(_ps("%p"), _A()),
            _mov(_A(), _data("HARGS", 0)),
            _ret(save_a=False),
        )
        out = backward_copy_propagate(fn)
        # 4 instructions → 2 instructions: rewritten def + Return.
        self.assertEqual(len(out.instructions), 2)
        # Def now writes HARGS+0 directly.
        self.assertEqual(out.instructions[0].dst, _data("HARGS", 0))
        self.assertEqual(out.instructions[0].src, _A())

    def test_relocation_into_stack_dst(self) -> None:
        # D = Stack(off) (caller's outgoing-arg slot). Same shape,
        # different memory class — should also work.
        fn = _fn(
            _mov(_A(), _ps("%p")),
            _mov(_ps("%p"), _A()),
            _mov(_A(), _stack(1)),
            _ret(save_a=False),
        )
        out = backward_copy_propagate(fn)
        self.assertEqual(len(out.instructions), 2)
        self.assertEqual(out.instructions[0].dst, _stack(1))


if __name__ == "__main__":
    unittest.main()
