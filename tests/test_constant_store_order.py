"""passes/constant_store_order.py: a run of constant stores grouped by
value, so each value is loaded once."""

import unittest

import asm_ast
from passes.constant_store_order import apply_constant_store_order

_A = asm_ast.Reg(reg=asm_ast.A())


def _const(value, name, volatile=False):
    """`Mov(#value, name)`, which `asm_emit` writes as `LDA; STA`."""
    return asm_ast.Mov(src=asm_ast.Imm(value=value),
                       dst=asm_ast.Data(name=name, offset=0),
                       is_volatile=volatile)


def _load(value):
    return asm_ast.Mov(src=asm_ast.Imm(value=value), dst=_A)


def _store(name):
    return asm_ast.Mov(src=_A, dst=asm_ast.Data(name=name, offset=0))


def _run(instrs):
    prog = asm_ast.Program(top_level=[asm_ast.Function(
        name="f", is_global=True, params=[],
        instructions=[*instrs, asm_ast.Return()],
    )])
    return apply_constant_store_order(prog).top_level[0].instructions[:-1]


class TestConstantStoreOrder(unittest.TestCase):

    def test_repeated_value_loaded_once(self):
        # `csw = 0xC100; valid = 0;` stores 0, $C1, 0.
        out = _run([_const(0, "lo"), _const(0xC1, "hi"), _const(0, "valid")])
        self.assertEqual(out, [
            _load(0xC1), _store("hi"),
            _load(0), _store("lo"), _store("valid"),
        ])

    def test_last_value_stays_last(self):
        # A and N/Z hold 7 after the run, as before.
        out = _run([_const(7, "a"), _const(1, "b"), _const(7, "c"),
                    _const(1, "d")])
        self.assertEqual(out, [
            _load(7), _store("a"), _store("c"),
            _load(1), _store("b"), _store("d"),
        ])

    def test_no_repeat_left_alone(self):
        instrs = [_const(1, "a"), _const(2, "b")]
        self.assertEqual(_run(instrs), instrs)

    def test_same_byte_twice_left_alone(self):
        # The second store of `a` has to come after the first.
        instrs = [_const(0, "a"), _const(1, "a"), _const(0, "b")]
        self.assertEqual(_run(instrs), instrs)

    def test_volatile_store_ends_the_run(self):
        instrs = [_const(0, "a"), _const(1, "io", volatile=True),
                  _const(0, "b")]
        self.assertEqual(_run(instrs), instrs)

    def test_two_functions_slots_left_alone(self):
        # The linker may give these two slots one byte.
        instrs = [_const(0, "__zpabi_g__p"), _const(1, "__local_h__t"),
                  _const(0, "x")]
        self.assertEqual(_run(instrs), instrs)

    def test_label_halves_count_as_constants(self):
        lo = asm_ast.Mov(src=asm_ast.ImmLabelLow(name="t", offset=0),
                         dst=asm_ast.Data(name="p", offset=0))
        hi = asm_ast.Mov(src=asm_ast.ImmLabelHigh(name="t", offset=0),
                         dst=asm_ast.Data(name="p", offset=1))
        lo2 = asm_ast.Mov(src=asm_ast.ImmLabelLow(name="t", offset=0),
                          dst=asm_ast.Data(name="q", offset=0))
        out = _run([lo, hi, lo2])
        self.assertEqual(len([i for i in out if i.dst == _A]), 2)


if __name__ == "__main__":
    unittest.main()
