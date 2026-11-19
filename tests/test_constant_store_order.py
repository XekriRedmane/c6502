"""passes/constant_store_order.py: a run of stores grouped by the value
stored, constant or loaded from memory, so each value is loaded once."""

import shutil
import unittest

import asm_ast
from passes.constant_store_order import apply_constant_store_order
from sim.harness import build_sim

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


def _mem_load(src, volatile=False):
    return asm_ast.Mov(src=src, dst=_A, is_volatile=volatile)


def _indexed(name, index=asm_ast.X):
    return asm_ast.IndexedData(name=name, offset=0, index=index())


def _store_at(name, offset):
    return asm_ast.Mov(src=_A, dst=asm_ast.Data(name=name, offset=offset))


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


class TestLoadedValueStoreOrder(unittest.TestCase):
    """Runs whose values are loaded from memory, as in Lode Runner's
    `fill_hole`."""

    _ROWS = _indexed("ROWS")
    _COLS = _indexed("COLS")

    def _fill_hole(self):
        return [
            _mem_load(self._ROWS), _store("row"),
            _mem_load(self._COLS), _store("col"),
            _mem_load(self._ROWS), _store("GAME_ROWNUM"),
            _mem_load(self._COLS), _store("GAME_COLNUM"),
        ]

    def test_indexed_values_loaded_once(self):
        self.assertEqual(_run(self._fill_hole()), [
            _mem_load(self._ROWS), _store("row"), _store("GAME_ROWNUM"),
            _mem_load(self._COLS), _store("col"), _store("GAME_COLNUM"),
        ])

    def test_data_value_loaded_once(self):
        g = asm_ast.Data(name="g", offset=0)
        out = _run([_mem_load(g), _store("a"), _load(1), _store("b"),
                    _mem_load(g), _store("c"), _load(1), _store("d")])
        self.assertEqual(out, [
            _mem_load(g), _store("a"), _store("c"),
            _load(1), _store("b"), _store("d"),
        ])

    def test_volatile_load_left_alone(self):
        # A volatile byte may change between the two reads.
        io = asm_ast.Data(name="io", offset=0)
        instrs = [_mem_load(io, volatile=True), _store("a"),
                  _load(1), _store("b"),
                  _mem_load(io, volatile=True), _store("c")]
        self.assertEqual(_run(instrs), instrs)

    def test_store_into_indexed_source_left_alone(self):
        # `STA ROWS+3` may write the byte `ROWS,X` reads, so the second
        # load may read a different value.
        instrs = [_mem_load(self._ROWS), _store("a"),
                  _load(1), _store_at("ROWS", 3),
                  _mem_load(self._ROWS), _store("c")]
        self.assertEqual(_run(instrs), instrs)

    def test_store_into_data_source_left_alone(self):
        g = asm_ast.Data(name="g", offset=0)
        instrs = [_mem_load(g), _store("a"), _load(1), _store("g"),
                  _mem_load(g), _store("c")]
        self.assertEqual(_run(instrs), instrs)

    def test_last_value_moved_past_a_store_of_its_byte_left_alone(self):
        # Grouping would move the load of `g` after `STA g`.
        g = asm_ast.Data(name="g", offset=0)
        instrs = [_mem_load(g), _store("a"), _load(1), _store("b"),
                  _load(1), _store("g"), _mem_load(g), _store("c")]
        self.assertEqual(_run(instrs), instrs)

    def test_index_register_changed_left_alone(self):
        # After `LDX`, `ROWS,X` names another byte.
        ldx = asm_ast.Mov(src=asm_ast.Data(name="j", offset=0),
                          dst=asm_ast.Reg(reg=asm_ast.X()))
        instrs = [_mem_load(self._ROWS), _store("a"),
                  _mem_load(self._COLS), _store("b"), ldx,
                  _mem_load(self._ROWS), _store("c")]
        self.assertEqual(_run(instrs), instrs)

    def test_other_index_register_is_another_value(self):
        instrs = [_mem_load(self._ROWS), _store("a"),
                  _mem_load(self._COLS), _store("b"),
                  _mem_load(_indexed("ROWS", asm_ast.Y)), _store("c")]
        self.assertEqual(_run(instrs), instrs)

    def test_load_from_another_functions_slot_left_alone(self):
        # The linker may give `g`'s and `h`'s slots one byte, so the
        # store to `h`'s slot may change what `g`'s slot holds.
        g = asm_ast.Data(name="__zpabi_g__p", offset=0)
        instrs = [_mem_load(g), _store("a"), _load(1),
                  _store("__zpabi_h__q"), _mem_load(g), _store("c")]
        self.assertEqual(_run(instrs), instrs)

    def test_indexed_slot_source_left_alone(self):
        # A slot's bytes may be covered by other slot names.
        slot = _indexed("__local_f__buf", asm_ast.Y)
        instrs = [_mem_load(slot), _store("a"), _load(1), _store("b"),
                  _mem_load(slot), _store("c")]
        self.assertEqual(_run(instrs), instrs)


@unittest.skipUnless(shutil.which("pcpp"), "pcpp not installed")
class TestLoadedValueStoreOrderSim(unittest.TestCase):
    """Programs with the shape of `fill_hole`, run in both pipelines."""

    # Each brick whose timer runs out copies its row and column into
    # locals and globals, and the loop counter stays in X. The values
    # include 0, 128, and 255.
    _FILL = """
#include <stdint.h>
const uint8_t ROWS[4] = {3, 0, 255, 7};
const uint8_t COLS[4] = {9, 128, 1, 0};
uint8_t TIMERS[4] = {1, 2, 1, 1};
uint8_t MAP[8][10];
uint8_t GROW, GCOL;
uint16_t sum;

static void fill(uint8_t i) {
    uint8_t row = ROWS[i];
    uint8_t col = COLS[i];
    GROW = row;
    GCOL = col;
    uint8_t r = row & 7;
    uint8_t c = col % 10;
    MAP[r][c] += 1;
    sum += (uint16_t)(row * 3) + col + MAP[r][c];
}

int main(void) {
    for (uint8_t i = 3; (int8_t)i >= 0; i--) {
        if (--TIMERS[i] != 0)
            continue;
        fill(i);
    }
    return sum + GROW + GCOL;
}
"""

    # `T[1] = col` writes a byte that `T[i]` reads on the next iteration
    # of the loop, so the value of `row` can't be loaded again
    # after it.
    _ALIAS = """
#include <stdint.h>
uint8_t T[4] = {5, 6, 7, 8};
const uint8_t C[4] = {1, 2, 3, 4};
uint8_t G, H;

int main(void) {
    uint16_t s = 0;
    for (uint8_t i = 3; (int8_t)i >= 0; i--) {
        uint8_t row = T[i];
        uint8_t col = C[i];
        T[1] = col;
        G = row;
        H = col;
        s += (uint16_t)(row * 16) + col + T[i];
    }
    return s + G + H + T[1];
}
"""

    def test_fill_runs(self):
        # Bricks 3, 2, and 0 fill: 808 summed, then GROW = 3, GCOL = 9.
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                got = build_sim(self._FILL, optimize=optimize).run()
                self.assertEqual(got.return_int(), 820)

    def test_fill_loads_each_byte_once(self):
        from compile import _run_stage
        from preprocessor import preprocess
        asm = _run_stage("codegen", preprocess(self._FILL), optimize=True)
        lines = asm.split("\nmain:\n")[1].splitlines()
        # The first load of each table byte stores it into the local
        # and then into the global.
        rows = lines.index("   LDA   ROWS,X")
        self.assertEqual(lines[rows + 2], "   STA   GROW")
        cols = lines.index("   LDA   COLS,X")
        self.assertEqual(cols, rows + 3)
        self.assertEqual(lines[cols + 2], "   STA   GCOL")

    def test_store_into_source_runs(self):
        # The rows read are 8, 7, 3 (stored by the iteration before), and 5.
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                got = build_sim(self._ALIAS, optimize=optimize).run()
                self.assertEqual(got.return_int(), 407)


if __name__ == "__main__":
    unittest.main()
