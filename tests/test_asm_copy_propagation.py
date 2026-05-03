"""Behavioral tests for `passes.optimization_asm.copy_propagation`.

Coverage:
  - Mov(Imm, Pseudo) — Imm propagates to every use of the Pseudo.
  - Mov(Pseudo, Pseudo) — source Pseudo propagates to every use.
  - Chains: Mov(#1, %a); Mov(%a, %b); use %b → use #1.
  - Phi.args.source rewritten the same way as ordinary uses.
  - Pseudo USES inside Add/Sub/Compare also get rewritten.
  - DEFs are NOT rewritten (the Pseudo dst's identity stays).
  - Sources that alias mutable cells (Reg / Stack / Frame / Data /
    ZP / Indirect) are NOT propagated — those cells can be written
    by other instructions between def and use.
  - Static dsts are NOT propagated FROM (writes are externally
    observable).
  - Address-taken Pseudos are excluded BOTH as copy dsts AND as
    copy srcs — their value can change via a Store through the
    pointer.
  - LoadAddress.src is never substituted (it names a storage cell,
    not a value).
  - Phi-defined Pseudos are NOT treated as copies.
"""
from __future__ import annotations

import unittest

import asm_ast
from passes.optimization_asm.copy_propagation import copy_propagate


def _imm(v: int) -> asm_ast.Imm:
    return asm_ast.Imm(value=v)


def _A() -> asm_ast.Reg:
    return asm_ast.Reg(reg=asm_ast.A())


def _ps(name: str, off: int = 0) -> asm_ast.Pseudo:
    return asm_ast.Pseudo(name=name, offset=off)


def _mov(src, dst) -> asm_ast.Mov:
    return asm_ast.Mov(src=src, dst=dst)


def _ret_bare(save_a: bool = True) -> asm_ast.Return:
    return asm_ast.Return(save_a=save_a)


def _fn(*instrs, name: str = "main", params=()) -> asm_ast.Function:
    return asm_ast.Function(
        name=name,
        is_global=True,
        params=list(params),
        instructions=list(instrs),
    )


class TestCopyPropagation(unittest.TestCase):
    def test_propagates_imm_to_pseudo_use(self) -> None:
        # Mov #$05 -> %x ; Mov %x -> A ; Return.
        # Should rewrite the second Mov's src to #$05.
        fn = _fn(
            _mov(_imm(5), _ps("%x")),
            _mov(_ps("%x"), _A()),
            _ret_bare(),
        )
        out = copy_propagate(fn)
        # Second Mov's src is now Imm(5) (not %x).
        self.assertEqual(out.instructions[1].src, _imm(5))
        # First Mov is untouched (DCE will drop it later).
        self.assertEqual(out.instructions[0].dst, _ps("%x"))

    def test_propagates_pseudo_to_pseudo_use(self) -> None:
        # Mov %src -> %a ; Mov %a -> A ; Return.
        # Use of %a is rewritten to %src.
        fn = _fn(
            _mov(_ps("%src"), _ps("%a")),
            _mov(_ps("%a"), _A()),
            _ret_bare(),
        )
        out = copy_propagate(fn)
        self.assertEqual(out.instructions[1].src, _ps("%src"))

    def test_resolves_chain(self) -> None:
        # Mov #$07 -> %a ; Mov %a -> %b ; Mov %b -> A ; Return.
        # Use of %b should resolve all the way back to #$07.
        fn = _fn(
            _mov(_imm(7), _ps("%a")),
            _mov(_ps("%a"), _ps("%b")),
            _mov(_ps("%b"), _A()),
            _ret_bare(),
        )
        out = copy_propagate(fn)
        # Chain resolution: %b → %a → #$07.
        self.assertEqual(out.instructions[2].src, _imm(7))
        # The intermediate Mov's src was also rewritten (its src %a
        # resolves to #$07 too).
        self.assertEqual(out.instructions[1].src, _imm(7))

    def test_does_not_propagate_to_def_position(self) -> None:
        # Mov #$05 -> %x ; Mov %y -> %x ; ... — %x in the second
        # Mov is a DEF, not a use, and must stay as %x.
        fn = _fn(
            _mov(_imm(5), _ps("%x")),
            _mov(_ps("%y"), _ps("%x")),
            _ret_bare(),
        )
        out = copy_propagate(fn)
        # Second Mov's dst is still %x.
        self.assertEqual(out.instructions[1].dst, _ps("%x"))

    def test_propagates_into_phi_args(self) -> None:
        # Mov #$09 -> %src ; Phi(%dst, [(L, %src)]) — the phi's arg
        # source is a use, should get rewritten to #$09.
        fn = _fn(
            _mov(_imm(9), _ps("%src")),
            asm_ast.Label(name="L"),
            asm_ast.Phi(
                dst=_ps("%dst"),
                args=[asm_ast.AsmPhiArg(
                    pred_label="L", source=_ps("%src"),
                )],
            ),
            _ret_bare(),
        )
        out = copy_propagate(fn)
        phi = out.instructions[2]
        self.assertEqual(phi.args[0].source, _imm(9))

    def test_propagates_into_compare_operands(self) -> None:
        # Mov #$05 -> %x ; Mov #$0A -> %y ; Compare(%x, %y) ;
        # Both operands of Compare are uses, both get rewritten.
        fn = _fn(
            _mov(_imm(5), _ps("%x")),
            _mov(_imm(10), _ps("%y")),
            asm_ast.Compare(left=_ps("%x"), right=_ps("%y")),
            _ret_bare(),
        )
        out = copy_propagate(fn)
        cmp_ = out.instructions[2]
        self.assertEqual(cmp_.left, _imm(5))
        self.assertEqual(cmp_.right, _imm(10))

    def test_propagates_into_add_src(self) -> None:
        # Mov #$03 -> %x ; Add(%x, A) — %x in Add.src is a use.
        fn = _fn(
            _mov(_imm(3), _ps("%x")),
            asm_ast.Add(src=_ps("%x"), dst=_A()),
            _ret_bare(),
        )
        out = copy_propagate(fn)
        add = out.instructions[1]
        self.assertEqual(add.src, _imm(3))

    def test_does_not_propagate_from_reg_a_source(self) -> None:
        # Mov A -> %x ; Mov %x -> ... — A is mutable; we cannot
        # propagate %x → A everywhere. The pass leaves uses of %x
        # alone.
        fn = _fn(
            _mov(_A(), _ps("%x")),
            _mov(_ps("%x"), _ps("%y")),
            _ret_bare(),
        )
        out = copy_propagate(fn)
        # Use of %x in second Mov stays as %x (NOT replaced with A).
        self.assertEqual(out.instructions[1].src, _ps("%x"))

    def test_does_not_propagate_from_stack_source(self) -> None:
        # Mov Stack(1) -> %x ; later Mov %x -> A.
        # Stack(1) is mutable — between the two reads, an
        # intervening Mov to Stack(1) could change the value. The
        # pass conservatively skips Stack sources.
        fn = _fn(
            _mov(asm_ast.Stack(offset=1), _ps("%x")),
            _mov(_ps("%x"), _A()),
            _ret_bare(),
        )
        out = copy_propagate(fn)
        self.assertEqual(out.instructions[1].src, _ps("%x"))

    def test_does_not_propagate_from_frame_source(self) -> None:
        fn = _fn(
            _mov(asm_ast.Frame(offset=2), _ps("%x")),
            _mov(_ps("%x"), _A()),
            _ret_bare(),
        )
        out = copy_propagate(fn)
        self.assertEqual(out.instructions[1].src, _ps("%x"))

    def test_does_not_propagate_from_data_source(self) -> None:
        # Mov Data(g) -> %x ; ... — Data is mutable across calls.
        fn = _fn(
            _mov(asm_ast.Data(name="g", offset=0), _ps("%x")),
            _mov(_ps("%x"), _A()),
            _ret_bare(),
        )
        out = copy_propagate(fn)
        self.assertEqual(out.instructions[1].src, _ps("%x"))

    def test_static_dst_excluded(self) -> None:
        # Mov #$05 -> Pseudo("g") where g is in `statics`.
        # Writes to a static are externally observable (other
        # functions can read g), so the pass must NOT propagate
        # #$05 to subsequent uses of g — those uses are real reads
        # from memory, which can have been written by an
        # intervening Call.
        fn = _fn(
            _mov(_imm(5), _ps("g")),
            _mov(_ps("g"), _A()),
            _ret_bare(),
        )
        out = copy_propagate(fn, statics=frozenset({"g"}))
        # Use of g not substituted.
        self.assertEqual(out.instructions[1].src, _ps("g"))

    def test_address_taken_excluded_as_dst(self) -> None:
        # &%x is taken via LoadAddress. %x's value can be modified
        # via a Store(*p, ...), so even a Mov(#$05, %x) can't be
        # propagated to later uses of %x.
        fn = _fn(
            _mov(_imm(5), _ps("%x")),
            asm_ast.LoadAddress(src=_ps("%x"), dst=_ps("%p")),
            _mov(_ps("%x"), _A()),
            _ret_bare(),
        )
        out = copy_propagate(fn)
        # %x in the second Mov is NOT replaced with #$05 — its
        # value could have been mutated through %p.
        self.assertEqual(out.instructions[2].src, _ps("%x"))

    def test_address_taken_excluded_as_src(self) -> None:
        # Mov %addr_taken -> %x ; later use of %x. %addr_taken's
        # value isn't stable (could be mutated through &-of), so
        # we don't substitute it.
        fn = _fn(
            asm_ast.LoadAddress(src=_ps("%at"), dst=_ps("%p")),
            _mov(_ps("%at"), _ps("%x")),
            _mov(_ps("%x"), _A()),
            _ret_bare(),
        )
        out = copy_propagate(fn)
        self.assertEqual(out.instructions[2].src, _ps("%x"))

    def test_load_address_src_never_substituted(self) -> None:
        # Mov #$05 -> %target ; LoadAddress(%target, %p).
        # The "src" of LoadAddress names the storage cell whose
        # address we want — replacing it with #$05 would lose the
        # address entirely. Pass leaves it alone.
        fn = _fn(
            _mov(_imm(5), _ps("%target")),
            asm_ast.LoadAddress(src=_ps("%target"), dst=_ps("%p")),
            _ret_bare(),
        )
        out = copy_propagate(fn)
        la = out.instructions[1]
        self.assertEqual(la.src, _ps("%target"))

    def test_phi_dst_not_a_copy(self) -> None:
        # Phi(%dst, [(L, %s)]) ; Mov %dst -> A.
        # Even if %dst happens to have a single arg, this pass
        # doesn't fold Phi → Copy. Use of %dst stays as %dst.
        fn = _fn(
            asm_ast.Label(name="L"),
            asm_ast.Phi(
                dst=_ps("%dst"),
                args=[asm_ast.AsmPhiArg(
                    pred_label="L", source=_ps("%s"),
                )],
            ),
            _mov(_ps("%dst"), _A()),
            _ret_bare(),
        )
        out = copy_propagate(fn)
        # %dst use NOT rewritten — Phis aren't copies in this pass.
        self.assertEqual(out.instructions[2].src, _ps("%dst"))

    def test_byte_granular_keys(self) -> None:
        # %v at offset 0 and %v at offset 1 are distinct
        # propagation slots; rewriting %v.b0 doesn't touch %v.b1.
        fn = _fn(
            _mov(_imm(0x12), _ps("%v", 0)),
            _mov(_imm(0x34), _ps("%v", 1)),
            _mov(_ps("%v", 0), _A()),
            _mov(_ps("%v", 1), _A()),
            _ret_bare(),
        )
        out = copy_propagate(fn)
        self.assertEqual(out.instructions[2].src, _imm(0x12))
        self.assertEqual(out.instructions[3].src, _imm(0x34))

    def test_no_changes_returns_same_function(self) -> None:
        # No copy candidates → pass returns the input unchanged.
        fn = _fn(
            _mov(_A(), _ps("%x")),
            _ret_bare(),
        )
        out = copy_propagate(fn)
        # Whether identity-equal or not, instructions should match.
        self.assertEqual(out.instructions, fn.instructions)


if __name__ == "__main__":
    unittest.main()
