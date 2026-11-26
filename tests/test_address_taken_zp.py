"""Unit tests for `passes.address_taken_zp.compute_address_taken_assignments`.

Covers the per-function allocation of address-taken locals into the
private ZP pool: identifies candidates from `LoadAddress.src=Pseudo`,
allocates them in the unused portion of `local_pools[fn]`, and
yields a stable `{pseudo_name: first_byte_address}` map per
function. Names that don't fit (no contiguous run of the required
size) are omitted so the caller's Frame fallback path runs.

The tests that called the Python functions directly are doctest cases of
the C++ port now, each named for the test it came from
(docs/cpp_port.md, step 8, group G6).
"""

import unittest

import asm_ast
from tests.cpp_build import needs_c6502cpp


@needs_c6502cpp
class TestAddressTakenZp(unittest.TestCase):
    """The allocation function reads sizes via the type-checker's
    `size_of_name`. We test through the full compile pipeline so the
    symbol table is real."""

    def _compile(self, src):
        from sim.harness import compile_to_asm
        return compile_to_asm(src, optimize=True)

    def test_uchar_address_taken_lands_in_zp(self):
        # A zp_abi function with a single `uint8_t` address-taken
        # local should have that local routed to a ZP byte and the
        # soft-stack prologue/epilogue collapsed.
        src = r"""
        #include <stdint.h>
        [[c6502::zp_abi]]
        void writer(uint8_t *out) { *out = 0x42; }
        [[c6502::zp_abi]]
        void caller(void) {
            uint8_t x;
            writer(&x);
        }
        int main(void) { caller(); return 0; }
        """
        asm, slot_syms = self._compile(src)
        # The address-taken local should resolve to a ZP slot whose
        # symbol is named `__local_caller__x`.
        sym_name = "__local_caller__x"
        self.assertIn(sym_name, slot_syms,
                      f"address-taken local should get an EQU "
                      f"binding for {sym_name}")
        # Slot symbol resolves to a ZP byte (< 256).
        self.assertLess(slot_syms[sym_name], 0x100,
                        "address-taken slot should land in zero page")

    def test_address_taken_in_loop_body(self):
        # The same shape as `entity_proximity`: a zp_abi caller takes
        # the address of a uchar local and passes it to a zp_abi
        # callee that writes through the pointer. We verify both
        # the EQU binding and that the asm body uses LDA #<sym /
        # LDA #>sym to materialize the address (instead of the FP+off
        # 6-byte compute).
        src = r"""
        #include <stdint.h>
        #include <stdbool.h>
        uint8_t target;
        [[c6502::zp_abi]]
        bool finder(uint8_t *out) {
            *out = target;
            return true;
        }
        [[c6502::zp_abi]]
        void caller(void) {
            uint8_t row;
            if (!finder(&row)) return;
            target = row;
        }
        int main(void) { caller(); return 0; }
        """
        asm, slot_syms = self._compile(src)
        sym = "__local_caller__row"
        self.assertIn(sym, slot_syms)
        # If the assignment worked, the address-taken local resolves
        # to a Data operand, and `lower_data_load_address` turns
        # `LoadAddress(src=Data(sym))` into the immediate pair `LDA
        # #<sym; ...; LDA #>sym`. If it didn't, we'd see a
        # `LoadAddress(src=Frame(...))` instead, which lowers to the
        # 6-instruction CLC; LDA FP; ADC #imm; ... runtime compute.
        caller_fn = next(
            tl for tl in asm.top_level
            if isinstance(tl, asm_ast.Function) and tl.name == "caller"
        )
        has_immediate_address = any(
            isinstance(instr, asm_ast.Mov)
            and isinstance(instr.src, asm_ast.ImmLabelLow)
            and instr.src.name == sym
            for instr in caller_fn.instructions
        )
        self.assertTrue(
            has_immediate_address,
            "expected `LDA #<__local_caller__row` in the caller body; "
            "the address-taken local didn't get routed to the "
            "immediate-address path",
        )


if __name__ == "__main__":
    unittest.main()
