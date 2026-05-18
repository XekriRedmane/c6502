"""Unit tests for `passes.x_save_slot_load.apply_x_save_slot_load`.

The pass rewrites reads of a memory slot M to reads of `Reg(X)`
when M is used as an X-save slot (i.e., is the destination of at
least one `Mov(Reg(X), M)`). See the module docstring for the
motivating bug.
"""

import unittest

import asm_ast
from passes.x_save_slot_load import apply_x_save_slot_load


def _A():
    return asm_ast.Reg(reg=asm_ast.A())


def _X():
    return asm_ast.Reg(reg=asm_ast.X())


def _Y():
    return asm_ast.Reg(reg=asm_ast.Y())


def _M(name="M"):
    return asm_ast.Data(name=name, offset=0)


def _wrap(instrs):
    fn = asm_ast.Function(
        name="f", is_global=True, params=[], instructions=instrs,
    )
    return asm_ast.Program(top_level=[fn])


def _instrs(prog):
    return prog.top_level[0].instructions


class TestXSaveSlotLoad(unittest.TestCase):

    def test_rewrites_lda_m_to_txa(self):
        # `Mov(M, Reg(A))` becomes `Mov(Reg(X), Reg(A))` when M
        # is the dst of `Mov(Reg(X), M)` (an X-save slot).
        prog = _wrap([
            asm_ast.Mov(src=_X(), dst=_M(), is_volatile=False),  # STX M
            asm_ast.Mov(src=_M(), dst=_A(), is_volatile=False),  # LDA M
        ])
        result = apply_x_save_slot_load(prog)
        out = _instrs(result)
        self.assertEqual(out[1].src, _X(), "LDA M should rewrite to TXA")
        self.assertEqual(out[1].dst, _A())

    def test_rewrites_mem_to_mem_mov(self):
        # `Mov(M, Data(other))` (mem-to-mem, emits as
        # `LDA M; STA other`) becomes `Mov(Reg(X), Data(other))`
        # (STX other), since STX supports zp/abs.
        other = _M("other")
        prog = _wrap([
            asm_ast.Mov(src=_X(), dst=_M(), is_volatile=False),
            asm_ast.Mov(src=_M(), dst=other, is_volatile=False),
        ])
        result = apply_x_save_slot_load(prog)
        out = _instrs(result)
        self.assertEqual(out[1].src, _X())
        self.assertEqual(out[1].dst, other)

    def test_leaves_mov_to_reg_x_untouched(self):
        # `Mov(M, Reg(X))` (LDX M restore) is not rewritten —
        # downstream peepholes (direct_index_load / copy-prop)
        # handle it; rewriting to `Mov(Reg(X), Reg(X))` is a
        # self-Mov dropped by self-Mov peephole anyway, but
        # leave it as-is so downstream sees a recognizable
        # save-restore shape.
        prog = _wrap([
            asm_ast.Mov(src=_X(), dst=_M(), is_volatile=False),
            asm_ast.Mov(src=_M(), dst=_X(), is_volatile=False),
        ])
        result = apply_x_save_slot_load(prog)
        out = _instrs(result)
        # The LDX M (Mov(M, Reg(X))) stays as-is.
        self.assertEqual(out[1].src, _M())
        self.assertEqual(out[1].dst, _X())

    def test_no_op_when_no_stx_m_anywhere(self):
        # Without any `Mov(Reg(X), M)`, M is not an X-save slot
        # and no rewrite happens.
        prog = _wrap([
            asm_ast.Mov(src=asm_ast.Imm(value=1), dst=_M(),
                        is_volatile=False),
            asm_ast.Mov(src=_M(), dst=_A(), is_volatile=False),
        ])
        result = apply_x_save_slot_load(prog)
        out = _instrs(result)
        self.assertEqual(out[1].src, _M(),
                         "LDA M should be left alone — no STX M")

    def test_disqualifies_when_inc_dec_into_m(self):
        # An in-place `Inc(M)` could leave M != X. Disqualify.
        prog = _wrap([
            asm_ast.Mov(src=_X(), dst=_M(), is_volatile=False),
            asm_ast.Inc(dst=_M()),
            asm_ast.Mov(src=_M(), dst=_A(), is_volatile=False),
        ])
        result = apply_x_save_slot_load(prog)
        out = _instrs(result)
        self.assertEqual(out[2].src, _M())

    def test_disqualifies_sty_m(self):
        # `Mov(Reg(Y), M)` (STY M) leaves A unchanged, so a
        # following TAX wouldn't pick up M's value. Disqualify.
        prog = _wrap([
            asm_ast.Mov(src=_X(), dst=_M(), is_volatile=False),
            asm_ast.Mov(src=_Y(), dst=_M(), is_volatile=False),
            asm_ast.Mov(src=_A(), dst=_X(), is_volatile=False),  # TAX
            asm_ast.Mov(src=_M(), dst=_A(), is_volatile=False),
        ])
        result = apply_x_save_slot_load(prog)
        out = _instrs(result)
        self.assertEqual(out[3].src, _M())

    def test_accepts_init_via_imm_followed_by_tax(self):
        # `Mov(Imm, M); ... Mov(Reg(A), Reg(X))` is the init
        # shape `LDA #c; STA M; TAX` (the mem-to-mem Mov hides
        # the implicit LDA-into-A at the IR level). After this,
        # M = X = c, so the M == X invariant holds. The LDA M
        # rewrite is sound.
        prog = _wrap([
            asm_ast.Mov(src=asm_ast.Imm(value=1), dst=_M(),
                        is_volatile=False),
            asm_ast.Mov(src=_A(), dst=_X(), is_volatile=False),  # TAX
            asm_ast.Mov(src=_X(), dst=_M(), is_volatile=False),  # STX M
            asm_ast.Mov(src=_M(), dst=_A(), is_volatile=False),  # LDA M
        ])
        result = apply_x_save_slot_load(prog)
        out = _instrs(result)
        self.assertEqual(out[3].src, _X(),
                         "LDA M should rewrite to TXA after init pattern")

    def test_loop_with_dex_buggy_pattern(self):
        # The headline bug shape: loop counter X-promoted, with
        # mem-to-mem Mov(M, callee_slot) at call sites and DEX
        # at the tail. The Mov(M, callee_slot) must be rewritten
        # to Mov(Reg(X), callee_slot) so the callee sees the
        # post-DEX X value, not the stale M.
        callee_slot = _M("zpabi_callee_slot")
        prog = _wrap([
            asm_ast.Mov(src=asm_ast.Imm(value=1), dst=_M(),
                        is_volatile=False),
            asm_ast.Mov(src=_A(), dst=_X(), is_volatile=False),
            asm_ast.Label(name=".loop"),
            asm_ast.Mov(src=_M(), dst=callee_slot,
                        is_volatile=False),  # ← the bug
            asm_ast.Mov(src=_X(), dst=_M(), is_volatile=False),
            asm_ast.Call(name="callee"),
            asm_ast.Mov(src=_M(), dst=_X(), is_volatile=False),
            asm_ast.Dec(dst=_X()),
            asm_ast.Branch(cond=asm_ast.PL(), target=".loop"),
            asm_ast.Return(save_a=False),
        ])
        result = apply_x_save_slot_load(prog)
        out = _instrs(result)
        # The mem-to-mem Mov should be rewritten to STX callee_slot.
        self.assertEqual(out[3].src, _X(),
                         "Mov(M, callee_slot) should rewrite to "
                         "Mov(Reg(X), callee_slot)")
        self.assertEqual(out[3].dst, callee_slot)


if __name__ == "__main__":
    unittest.main()
