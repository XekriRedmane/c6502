"""Tests for the const-pointer-array dispatch pass."""

import unittest

import shutil

import c99_ast
import tac_ast
from passes.optimization.dispatch_pointer_array import (
    dispatch_const_pointer_arrays,
)
from passes.type_checking import Symbol, StaticAttr, Initial


def _table_symbol(elem, size):
    return Symbol(
        type=c99_ast.Array(element_type=elem, size=size),
        attrs=StaticAttr(initial_value=Initial(value=()), is_global=False),
    )


_CONST_ELEM = c99_ast.Const(
    referenced_type=c99_ast.Pointer(referenced_type=c99_ast.UChar()),
)

# The tables the tests below use, typed `static uint8_t * const
# name[N]`, so their elements are const.
_SYMBOLS = {
    "arr": _table_symbol(_CONST_ELEM, 3),
    "big": _table_symbol(_CONST_ELEM, 9),
    "int_arr": _table_symbol(c99_ast.Const(referenced_type=c99_ast.Int()), 4),
    "ext_arr": _table_symbol(_CONST_ELEM, 2),
}


def _v(n):
    return tac_ast.Var(name=n)


def _c(v):
    return tac_ast.Constant(const=tac_ast.ConstInt(value=v))


def _const_pointer_array(name, targets):
    return tac_ast.StaticVariable(
        name=name, is_global=False,
        data_type=tac_ast.Pointer(),
        init=[tac_ast.AddressInit(name=t, offset=0) for t in targets],
    )


def _wrap(instrs, *extra_top_level):
    return tac_ast.Program(top_level=[
        *extra_top_level,
        tac_ast.Function(
            name="f", is_global=True, params=[], instructions=instrs,
        ),
    ])


def _instrs(prog):
    for tl in prog.top_level:
        if isinstance(tl, tac_ast.Function):
            return tl.instructions
    raise AssertionError("no function in prog")


class TestDispatchPointerArray(unittest.TestCase):

    def test_chain_recognized_and_rewritten(self):
        # i*2 -> IndexedLoad(arr, _, %ptr) -> IndirectIndexedLoad(%ptr, j, %v).
        # The pass rewrites to a CMP/BEQ dispatch on i.
        prog = _wrap(
            [
                tac_ast.Binary(
                    op=tac_ast.LeftShift(),
                    src1=_v("%i"), src2=_c(1), dst=_v("%scaled"),
                ),
                tac_ast.IndexedLoad(
                    name="arr", index=_v("%scaled"), dst=_v("%ptr"),
                ),
                tac_ast.IndirectIndexedLoad(
                    ptr=_v("%ptr"), index=_v("%j"), dst=_v("%val"),
                ),
                tac_ast.Ret(val=_v("%val")),
            ],
            _const_pointer_array("arr", ["sub_a", "sub_b", "sub_c"]),
        )
        out = _instrs(dispatch_const_pointer_arrays(prog, _SYMBOLS))
        # Original 3 chain instructions are gone.
        self.assertFalse(any(
            isinstance(i, tac_ast.IndirectIndexedLoad) for i in out
        ))
        # 2 JumpIfCmp checks (for cases 0 and 1; case 2 is
        # fallthrough).
        cmps = [i for i in out if isinstance(i, tac_ast.JumpIfCmp)]
        self.assertEqual(len(cmps), 2)
        # 3 IndexedLoads (one per case) — replacing the original chain.
        ils = [i for i in out if isinstance(i, tac_ast.IndexedLoad)]
        self.assertEqual(len(ils), 3)
        # Each IndexedLoad targets the matching sub-array.
        names = sorted(i.name for i in ils)
        self.assertEqual(names, ["sub_a", "sub_b", "sub_c"])

    def test_too_large_array_skipped(self):
        # A 9-element pointer array exceeds _DISPATCH_THRESHOLD (8)
        # and isn't rewritten. The original chain stays intact.
        targets = [f"sub_{k}" for k in range(9)]
        prog = _wrap(
            [
                tac_ast.Binary(
                    op=tac_ast.LeftShift(),
                    src1=_v("%i"), src2=_c(1), dst=_v("%scaled"),
                ),
                tac_ast.IndexedLoad(
                    name="big", index=_v("%scaled"), dst=_v("%ptr"),
                ),
                tac_ast.IndirectIndexedLoad(
                    ptr=_v("%ptr"), index=_v("%j"), dst=_v("%val"),
                ),
                tac_ast.Ret(val=_v("%val")),
            ],
            _const_pointer_array("big", targets),
        )
        out = _instrs(dispatch_const_pointer_arrays(prog, _SYMBOLS))
        self.assertTrue(any(
            isinstance(i, tac_ast.IndirectIndexedLoad) for i in out
        ))
        self.assertFalse(any(
            isinstance(i, tac_ast.JumpIfCmp) for i in out
        ))

    def test_non_address_init_skipped(self):
        # A static array of integer initializers (not pointers) is
        # not eligible; the chain stays.
        prog = _wrap(
            [
                tac_ast.Binary(
                    op=tac_ast.LeftShift(),
                    src1=_v("%i"), src2=_c(1), dst=_v("%scaled"),
                ),
                tac_ast.IndexedLoad(
                    name="int_arr", index=_v("%scaled"), dst=_v("%ptr"),
                ),
                tac_ast.IndirectIndexedLoad(
                    ptr=_v("%ptr"), index=_v("%j"), dst=_v("%val"),
                ),
                tac_ast.Ret(val=_v("%val")),
            ],
            tac_ast.StaticVariable(
                name="int_arr", is_global=False,
                data_type=tac_ast.Int(),
                init=[tac_ast.IntInit(value=k) for k in range(4)],
            ),
        )
        out = _instrs(dispatch_const_pointer_arrays(prog, _SYMBOLS))
        self.assertTrue(any(
            isinstance(i, tac_ast.IndirectIndexedLoad) for i in out
        ))

    def test_global_pointer_array_skipped(self):
        # External-linkage pointer arrays aren't rewritten — another
        # TU could observe / replace the bindings.
        prog = _wrap(
            [
                tac_ast.Binary(
                    op=tac_ast.LeftShift(),
                    src1=_v("%i"), src2=_c(1), dst=_v("%scaled"),
                ),
                tac_ast.IndexedLoad(
                    name="ext_arr", index=_v("%scaled"), dst=_v("%ptr"),
                ),
                tac_ast.IndirectIndexedLoad(
                    ptr=_v("%ptr"), index=_v("%j"), dst=_v("%val"),
                ),
                tac_ast.Ret(val=_v("%val")),
            ],
            tac_ast.StaticVariable(
                name="ext_arr", is_global=True,
                data_type=tac_ast.Pointer(),
                init=[tac_ast.AddressInit(name="sub_a", offset=0),
                      tac_ast.AddressInit(name="sub_b", offset=0)],
            ),
        )
        out = _instrs(dispatch_const_pointer_arrays(prog, _SYMBOLS))
        self.assertTrue(any(
            isinstance(i, tac_ast.IndirectIndexedLoad) for i in out
        ))

    def test_multi_use_intermediate_skipped(self):
        # If the IndexedLoad's dst (%ptr) is also used elsewhere
        # (besides the IndirectIndexedLoad), removing the chain
        # would lose that other use.
        prog = _wrap(
            [
                tac_ast.Binary(
                    op=tac_ast.LeftShift(),
                    src1=_v("%i"), src2=_c(1), dst=_v("%scaled"),
                ),
                tac_ast.IndexedLoad(
                    name="arr", index=_v("%scaled"), dst=_v("%ptr"),
                ),
                tac_ast.IndirectIndexedLoad(
                    ptr=_v("%ptr"), index=_v("%j"), dst=_v("%val"),
                ),
                # Second use of %ptr — disqualifies the rewrite.
                tac_ast.Copy(src=_v("%ptr"), dst=_v("%saved")),
                tac_ast.Ret(val=_v("%val")),
            ],
            _const_pointer_array("arr", ["sub_a", "sub_b"]),
        )
        out = _instrs(dispatch_const_pointer_arrays(prog, _SYMBOLS))
        self.assertTrue(any(
            isinstance(i, tac_ast.IndirectIndexedLoad) for i in out
        ))


    def _chain_prog(self):
        return _wrap(
            [
                tac_ast.Binary(
                    op=tac_ast.LeftShift(),
                    src1=_v("%i"), src2=_c(1), dst=_v("%scaled"),
                ),
                tac_ast.IndexedLoad(
                    name="arr", index=_v("%scaled"), dst=_v("%ptr"),
                ),
                tac_ast.IndirectIndexedLoad(
                    ptr=_v("%ptr"), index=_v("%j"), dst=_v("%val"),
                ),
                tac_ast.Ret(val=_v("%val")),
            ],
            _const_pointer_array("arr", ["sub_a", "sub_b", "sub_c"]),
        )

    def test_writable_elements_skipped(self):
        # `static uint8_t *arr[3]`: another function may store a new
        # pointer into the table, so its initial targets can't be
        # hard-coded.
        symbols = {"arr": _table_symbol(
            c99_ast.Pointer(referenced_type=c99_ast.UChar()), 3,
        )}
        out = _instrs(dispatch_const_pointer_arrays(
            self._chain_prog(), symbols,
        ))
        self.assertTrue(any(
            isinstance(i, tac_ast.IndirectIndexedLoad) for i in out
        ))

    def test_const_pointees_only_skipped(self):
        # `static const uint8_t *arr[3]` has const pointees and
        # writable elements.
        symbols = {"arr": _table_symbol(
            c99_ast.Pointer(
                referenced_type=c99_ast.Const(referenced_type=c99_ast.UChar()),
            ),
            3,
        )}
        out = _instrs(dispatch_const_pointer_arrays(
            self._chain_prog(), symbols,
        ))
        self.assertTrue(any(
            isinstance(i, tac_ast.IndirectIndexedLoad) for i in out
        ))

    def test_no_symbols_skipped(self):
        out = _instrs(dispatch_const_pointer_arrays(self._chain_prog()))
        self.assertTrue(any(
            isinstance(i, tac_ast.IndirectIndexedLoad) for i in out
        ))


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestDispatchSim(unittest.TestCase):
    def _both(self, src, expected):
        from sim.harness import build_sim
        for optimize in (False, True):
            r = build_sim(src, optimize=optimize).run(max_cycles=200_000)
            self.assertEqual(r.return_int(), expected, f"optimize={optimize}")

    def test_table_stored_into_by_another_function(self):
        # swap() repoints frames[0]. The dispatch read walk[1] = 2
        # from the initial contents in place of other[1] = 9.
        src = (
            "#include <stdint.h>\n"
            "static uint8_t walk[4] = {1, 2, 3, 4};\n"
            "static uint8_t jump[4] = {5, 6, 7, 8};\n"
            "static uint8_t other[4] = {9, 9, 9, 9};\n"
            "static uint8_t *frames[2] = {walk, jump};\n"
            "void swap(void) { frames[0] = other; }\n"
            "uint8_t frame(uint8_t anim, uint8_t step) {\n"
            "    return frames[anim][step];\n"
            "}\n"
            "int main(void) { swap(); return frame(0, 1) + frame(1, 2) * 16; }\n"
        )
        self._both(src, 9 + 7 * 16)

    def test_const_table(self):
        src = (
            "#include <stdint.h>\n"
            "static uint8_t walk[4] = {1, 2, 3, 4};\n"
            "static uint8_t jump[4] = {5, 6, 7, 8};\n"
            "static uint8_t * const frames[2] = {walk, jump};\n"
            "uint8_t frame(uint8_t anim, uint8_t step) {\n"
            "    return frames[anim][step];\n"
            "}\n"
            "int main(void) { walk[1] = 11; return frame(0, 1) + frame(1, 2) * 16; }\n"
        )
        self._both(src, 11 + 7 * 16)


if __name__ == "__main__":
    unittest.main()
