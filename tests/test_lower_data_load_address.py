import unittest

import asm_ast
from passes.lower_data_load_address import lower_data_load_address


def _fn(*instrs, name="main", params=(), is_global=True):
    return asm_ast.Function(
        name=name,
        is_global=is_global,
        params=list(params),
        instructions=list(instrs),
    )


def _prog(*tops):
    return asm_ast.Program(top_level=list(tops))


def _ret():
    return asm_ast.Ret(arg_bytes=0, local_bytes=0, save_a=True)


class TestSplitDataSrc(unittest.TestCase):
    """`LoadAddress(src=Data(...), dst=mem)` splits into two
    ImmLabel Movs, low byte first then high. Same byte ordering
    `asm_emit._emit_load_address` uses."""

    def test_data_src_zp_dst(self):
        out = lower_data_load_address(_prog(_fn(
            asm_ast.LoadAddress(
                src=asm_ast.Data(name="target", offset=0),
                dst=asm_ast.ZP(address=0x80, offset=0),
            ),
            _ret(),
        )))
        self.assertEqual(out, _prog(_fn(
            asm_ast.Mov(
                src=asm_ast.ImmLabelLow(name="target", offset=0),
                dst=asm_ast.ZP(address=0x80, offset=0),
            ),
            asm_ast.Mov(
                src=asm_ast.ImmLabelHigh(name="target", offset=0),
                dst=asm_ast.ZP(address=0x80, offset=1),
            ),
            _ret(),
        )))

    def test_data_src_data_dst(self):
        """ZP-pool-routed dst — typical address-taken-local-routed-
        to-ZP shape after `replace_pseudoregisters_bare_exit`."""
        out = lower_data_load_address(_prog(_fn(
            asm_ast.LoadAddress(
                src=asm_ast.Data(name="entity_row_slot", offset=0),
                dst=asm_ast.Data(name="ptr_temp", offset=0),
            ),
            _ret(),
        )))
        self.assertEqual(out, _prog(_fn(
            asm_ast.Mov(
                src=asm_ast.ImmLabelLow(
                    name="entity_row_slot", offset=0,
                ),
                dst=asm_ast.Data(name="ptr_temp", offset=0),
            ),
            asm_ast.Mov(
                src=asm_ast.ImmLabelHigh(
                    name="entity_row_slot", offset=0,
                ),
                dst=asm_ast.Data(name="ptr_temp", offset=1),
            ),
            _ret(),
        )))

    def test_data_src_nonzero_offset_preserved(self):
        """`&(arr+3)` shape — the src.offset propagates onto each
        ImmLabel*'s offset field, matching emit's
        `LDA #<(name+offset)` rendering."""
        out = lower_data_load_address(_prog(_fn(
            asm_ast.LoadAddress(
                src=asm_ast.Data(name="arr", offset=3),
                dst=asm_ast.ZP(address=0x80, offset=0),
            ),
            _ret(),
        )))
        self.assertEqual(out, _prog(_fn(
            asm_ast.Mov(
                src=asm_ast.ImmLabelLow(name="arr", offset=3),
                dst=asm_ast.ZP(address=0x80, offset=0),
            ),
            asm_ast.Mov(
                src=asm_ast.ImmLabelHigh(name="arr", offset=3),
                dst=asm_ast.ZP(address=0x80, offset=1),
            ),
            _ret(),
        )))

    def test_data_src_dst_with_offset(self):
        """A dst that already has a nonzero offset (uncommon but
        the IR allows it) gets +1 on the high byte too."""
        out = lower_data_load_address(_prog(_fn(
            asm_ast.LoadAddress(
                src=asm_ast.Data(name="target", offset=0),
                dst=asm_ast.Data(name="multi", offset=2),
            ),
            _ret(),
        )))
        self.assertEqual(out, _prog(_fn(
            asm_ast.Mov(
                src=asm_ast.ImmLabelLow(name="target", offset=0),
                dst=asm_ast.Data(name="multi", offset=2),
            ),
            asm_ast.Mov(
                src=asm_ast.ImmLabelHigh(name="target", offset=0),
                dst=asm_ast.Data(name="multi", offset=3),
            ),
            _ret(),
        )))


class TestFrameSrcUntouched(unittest.TestCase):
    """`LoadAddress(src=Frame(off), ...)` keeps its compound form.
    The `FP + off` arithmetic genuinely requires the CLC/LDA/ADC
    chain that emit lowers it to; no ImmLabel form exists for an
    auto-storage local on the soft stack."""

    def test_frame_src_passes_through(self):
        instrs = (
            asm_ast.LoadAddress(
                src=asm_ast.Frame(offset=4),
                dst=asm_ast.ZP(address=0x80, offset=0),
            ),
            _ret(),
        )
        out = lower_data_load_address(_prog(_fn(*instrs)))
        self.assertEqual(out, _prog(_fn(*instrs)))


class TestNoRewriteIdentity(unittest.TestCase):
    """Functions with no Data-source LoadAddress return unchanged
    (same identity is fine; the test asserts equality, which is
    sufficient for the snapshot tests downstream)."""

    def test_function_without_load_address(self):
        instrs = (
            asm_ast.Mov(
                src=asm_ast.Imm(value=5),
                dst=asm_ast.ZP(address=0x80, offset=0),
            ),
            _ret(),
        )
        out = lower_data_load_address(_prog(_fn(*instrs)))
        self.assertEqual(out, _prog(_fn(*instrs)))


class TestNonFunctionTopLevels(unittest.TestCase):
    """StaticVariable / other non-Function tops pass through."""

    def test_static_variable_preserved(self):
        sv = asm_ast.StaticVariable(
            name="g",
            is_global=False,
            init=[asm_ast.IntInit(value=5)],
        )
        out = lower_data_load_address(_prog(sv, _fn(_ret())))
        self.assertEqual(out, _prog(sv, _fn(_ret())))


class TestMixedInstructionStream(unittest.TestCase):
    """LoadAddress in the middle of a stream: only that atom
    rewrites, surrounding instructions are untouched and order
    is preserved."""

    def test_split_in_middle(self):
        before = asm_ast.Mov(
            src=asm_ast.Imm(value=7),
            dst=asm_ast.ZP(address=0x82, offset=0),
        )
        la = asm_ast.LoadAddress(
            src=asm_ast.Data(name="target", offset=0),
            dst=asm_ast.ZP(address=0x80, offset=0),
        )
        after = asm_ast.Mov(
            src=asm_ast.Imm(value=9),
            dst=asm_ast.ZP(address=0x83, offset=0),
        )
        out = lower_data_load_address(_prog(_fn(before, la, after, _ret())))
        self.assertEqual(out, _prog(_fn(
            before,
            asm_ast.Mov(
                src=asm_ast.ImmLabelLow(name="target", offset=0),
                dst=asm_ast.ZP(address=0x80, offset=0),
            ),
            asm_ast.Mov(
                src=asm_ast.ImmLabelHigh(name="target", offset=0),
                dst=asm_ast.ZP(address=0x80, offset=1),
            ),
            after,
            _ret(),
        )))


if __name__ == "__main__":
    unittest.main()
