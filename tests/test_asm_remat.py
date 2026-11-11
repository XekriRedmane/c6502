"""Unit tests for `passes.asm_remat.apply_remat`.

Covers the single-atom mem-to-mem staging shape, the two-atom
`Mov(<recomp>, Reg(A)); Mov(Reg(A), Data(local))` shape (post-
SSA round-trip), and the dead-stage-dst collapse that converts a
no-longer-needed `Mov(_, Data(local))` to a bare `Mov(_, Reg(A))`
(or omits it entirely when the src is already `Reg(A)`).
"""

import unittest

import asm_ast
from passes.asm_remat import apply_remat


def _A():
    return asm_ast.Reg(reg=asm_ast.A())


def _X():
    return asm_ast.Reg(reg=asm_ast.X())


def _wrap(instrs, slot_symbols=None):
    fn = asm_ast.Function(
        name="f", is_global=True, params=[], instructions=instrs,
    )
    prog = asm_ast.Program(top_level=[fn])
    return apply_remat(prog, zp_slot_symbols=slot_symbols or {})


def _instrs(prog):
    return prog.top_level[0].instructions


def _data(name="__local_f__0"):
    return asm_ast.Data(name=name, offset=0)


def _indexed(name="arr"):
    return asm_ast.IndexedData(name=name, offset=0, index=asm_ast.X())


class TestApplyRemat(unittest.TestCase):

    def test_single_atom_mem_to_mem_use_rewrite(self):
        # Mov(IndexedData(arr, X), Data(local)); Mov(Data(local), A)
        # → use becomes Mov(IndexedData(arr, X), A).
        prog = _wrap([
            asm_ast.Mov(src=_indexed(), dst=_data(), is_volatile=False),
            asm_ast.Mov(src=_data(), dst=_A(), is_volatile=False),
        ], slot_symbols={"__local_f__0": 0x80})
        out = _instrs(prog)
        self.assertEqual(out[-1].src, _indexed())
        self.assertEqual(out[-1].dst, _A())

    def test_two_atom_pattern_use_rewrite(self):
        # LDA arr,X; STA local; (A clobber) LDA other,X; STA other_local;
        # use: LDA local
        # Use should rewrite to LDA arr,X via the producer.
        other_local = _data("__local_f__1")
        prog = _wrap([
            asm_ast.Mov(src=_indexed("arr"), dst=_A(), is_volatile=False),
            asm_ast.Mov(src=_A(), dst=_data(), is_volatile=False),
            asm_ast.Mov(src=_indexed("arr2"), dst=_A(), is_volatile=False),
            asm_ast.Mov(src=_A(), dst=other_local, is_volatile=False),
            asm_ast.Mov(src=_data(), dst=_A(), is_volatile=False),
        ], slot_symbols={"__local_f__0": 0x80, "__local_f__1": 0x81})
        out = _instrs(prog)
        # The use (last Mov) should now read from arr,X.
        self.assertEqual(out[-1].src, _indexed("arr"))
        self.assertEqual(out[-1].dst, _A())

    def test_two_atom_blocked_by_x_write(self):
        # If X is written between the producer and the use, the
        # IndexedData(arr, X) recompute is unsound. Use is left alone.
        prog = _wrap([
            asm_ast.Mov(src=_indexed("arr"), dst=_A(), is_volatile=False),
            asm_ast.Mov(src=_A(), dst=_data(), is_volatile=False),
            asm_ast.Mov(src=asm_ast.Imm(value=5), dst=_X(),
                        is_volatile=False),  # X clobber
            asm_ast.Mov(src=_data(), dst=_A(), is_volatile=False),
        ], slot_symbols={"__local_f__0": 0x80})
        out = _instrs(prog)
        # Use is unchanged.
        self.assertEqual(out[-1].src, _data())

    def test_two_atom_blocked_by_call(self):
        # A Call between producer and use invalidates the recompute
        # (calls can write to arr).
        prog = _wrap([
            asm_ast.Mov(src=_indexed("arr"), dst=_A(), is_volatile=False),
            asm_ast.Mov(src=_A(), dst=_data(), is_volatile=False),
            asm_ast.Call(name="callee"),
            asm_ast.Mov(src=_data(), dst=_A(), is_volatile=False),
        ], slot_symbols={"__local_f__0": 0x80})
        out = _instrs(prog)
        self.assertEqual(out[-1].src, _data())

    def test_two_atom_blocked_by_a_arith_producer(self):
        # If the value of A at the staging def came from an
        # arithmetic op (Add/Sub/And/...), not a Mov, then the
        # "producer" isn't a single-operand recomputable load —
        # bail.
        prog = _wrap([
            asm_ast.Mov(src=_indexed("arr"), dst=_A(), is_volatile=False),
            asm_ast.Add(src=asm_ast.Imm(value=1), dst=_A()),
            asm_ast.Mov(src=_A(), dst=_data(), is_volatile=False),
            asm_ast.Mov(src=_data(), dst=_A(), is_volatile=False),
        ], slot_symbols={"__local_f__0": 0x80})
        out = _instrs(prog)
        self.assertEqual(out[-1].src, _data())

    def test_dead_stage_dst_two_atom_omits_self_mov(self):
        # After remat rewrites the use, the staging def's local has
        # no readers. The dead-stage-dst sweep should omit the
        # `Mov(Reg(A), Data(local))` entirely (rather than collapse
        # to a useless `Mov(Reg(A), Reg(A))` self-Mov).
        prog = _wrap([
            asm_ast.Mov(src=_indexed("arr"), dst=_A(), is_volatile=False),
            asm_ast.Mov(src=_A(), dst=_data(), is_volatile=False),
            asm_ast.Mov(src=_data(), dst=_A(), is_volatile=False),
        ], slot_symbols={"__local_f__0": 0x80})
        out = _instrs(prog)
        # Should have just the producer + the rewritten use; no
        # self-Mov in between.
        for instr in out:
            if isinstance(instr, asm_ast.Mov):
                self.assertFalse(
                    (isinstance(instr.src, asm_ast.Reg)
                     and isinstance(instr.src.reg, asm_ast.A)
                     and isinstance(instr.dst, asm_ast.Reg)
                     and isinstance(instr.dst.reg, asm_ast.A)),
                    f"unwanted self-Mov in output: {instr}",
                )



class TestRematYSetup(unittest.TestCase):
    """An operand lowered with an `LDY #off` setup writes Y, so an
    `abs,Y` load can't be recomputed past it."""

    def _indexed_y(self):
        return asm_ast.IndexedData(name="arr", offset=0, index=asm_ast.Y())

    def test_blocked_by_indirect_zp_setup(self):
        prog = _wrap([
            asm_ast.Mov(src=self._indexed_y(), dst=_A(), is_volatile=False),
            asm_ast.Mov(src=_A(), dst=_data(), is_volatile=False),
            asm_ast.Mov(src=asm_ast.IndirectZp(address=0x90, offset=0),
                        dst=_A(), is_volatile=False),
            asm_ast.Mov(src=_data(), dst=_A(), is_volatile=False),
        ], slot_symbols={"__local_f__0": 0x80})
        self.assertEqual(_instrs(prog)[-1].src, _data())

    def test_not_blocked_without_y_write(self):
        prog = _wrap([
            asm_ast.Mov(src=self._indexed_y(), dst=_A(), is_volatile=False),
            asm_ast.Mov(src=_A(), dst=_data(), is_volatile=False),
            asm_ast.Mov(src=asm_ast.Imm(value=1), dst=_A(),
                        is_volatile=False),
            asm_ast.Mov(src=_data(), dst=_A(), is_volatile=False),
        ], slot_symbols={"__local_f__0": 0x80})
        self.assertEqual(_instrs(prog)[-1].src, self._indexed_y())


class TestRematPointerStore(unittest.TestCase):
    """A store through a pointer between the load and the read may
    write the static, unless the static is internal and its address
    is never formed. `uint8_t t = g; *p = 7; return t;` called with
    `p == &g` returned 7 instead of the old value of `g`."""

    def _body(self, ptr_dst):
        g = asm_ast.Data(name="g", offset=0)
        return [
            asm_ast.Mov(src=g, dst=_A(), is_volatile=False),
            asm_ast.Mov(src=_A(), dst=_data(), is_volatile=False),
            asm_ast.Mov(src=asm_ast.Imm(value=7), dst=_A(),
                        is_volatile=False),
            asm_ast.Mov(src=_A(), dst=ptr_dst, is_volatile=False),
            asm_ast.Mov(src=_data(), dst=_A(), is_volatile=False),
            asm_ast.Return(save_a=True),
        ]

    def _prog(self, instrs, *, g_global, g_address_taken=False):
        fns = [asm_ast.Function(
            name="f", is_global=True, params=[], instructions=instrs,
        )]
        if g_address_taken:
            fns.append(asm_ast.Function(
                name="main", is_global=True, params=[], instructions=[
                    asm_ast.Mov(src=asm_ast.ImmLabelLow(name="g", offset=0),
                                dst=_A(), is_volatile=False),
                    asm_ast.Return(save_a=True),
                ],
            ))
        g = asm_ast.StaticVariable(
            name="g", is_global=g_global,
            init=[asm_ast.ZeroInit(bytes=1)],
        )
        return apply_remat(
            asm_ast.Program(top_level=[*fns, g]),
            zp_slot_symbols={"__local_f__0": 0x80},
        )

    def test_blocked_by_indirect_zp_store_to_global(self):
        for dst in (
            asm_ast.IndirectZp(address=0x90, offset=0),
            asm_ast.IndirectZpY(address=0x90),
            asm_ast.Indirect(offset=0),
            asm_ast.IndirectY(),
            asm_ast.IndexedData(name="", offset=0x2000, index=asm_ast.X()),
        ):
            with self.subTest(dst=dst):
                out = self._prog(self._body(dst), g_global=True)
                self.assertEqual(
                    out.top_level[0].instructions[-2].src, _data(),
                )

    def test_blocked_for_internal_static_whose_address_is_taken(self):
        out = self._prog(
            self._body(asm_ast.IndirectZp(address=0x90, offset=0)),
            g_global=False, g_address_taken=True,
        )
        self.assertEqual(out.top_level[0].instructions[-2].src, _data())

    def test_private_static_survives_pointer_store(self):
        # No pointer can point into an internal static whose address
        # nothing forms, as with `static const uint8_t glyph[8]`.
        out = self._prog(
            self._body(asm_ast.IndirectZp(address=0x90, offset=0)),
            g_global=False,
        )
        self.assertEqual(
            out.top_level[0].instructions[-2].src,
            asm_ast.Data(name="g", offset=0),
        )

    def test_simulated_pointer_store_to_global(self):
        import shutil
        if not shutil.which("pcpp"):
            self.skipTest("pcpp not installed")
        from sim.harness import build_sim
        src = (
            "unsigned char g;\n"
            "unsigned char f(unsigned char *p) {\n"
            "    unsigned char t = g;\n"
            "    *p = 7;\n"
            "    return t;\n"
            "}\n"
            "int main(void) { g = 3; return f(&g); }\n"
        )
        self.assertEqual(build_sim(src, optimize=True).run().return_int(), 3)


if __name__ == "__main__":
    unittest.main()
