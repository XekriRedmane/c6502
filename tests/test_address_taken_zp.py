"""Unit tests for `passes.address_taken_zp.compute_address_taken_assignments`.

Covers the per-function allocation of address-taken locals into the
private ZP pool: identifies candidates from `LoadAddress.src=Pseudo`,
allocates them in the unused portion of `local_pools[fn]`, and
yields a stable `{pseudo_name: first_byte_address}` map per
function. Names that don't fit (no contiguous run of the required
size) are omitted so the caller's Frame fallback path runs.
"""

import shutil
import unittest
from dataclasses import dataclass

import asm_ast
from passes.address_taken_zp import (
    compute_address_taken_assignments,
    slot_symbols,
)


@dataclass
class _FakeColoring:
    assignments: dict[str, int]


@dataclass
class _FakeSym:
    type: object
    attrs: object


class _Local:
    pass


class _UCharType:
    pass


class _IntType:
    pass


def _sym_table(*entries):
    out = {}
    for name, t in entries:
        out[name] = _FakeSym(type=t, attrs=_Local())
    return out


def _wrap_pseudo(name, offset=0):
    return asm_ast.Pseudo(name=name, offset=offset)


def _wrap_data(name, offset=0):
    return asm_ast.Data(name=name, offset=offset)


def _fn(name, params, instrs):
    return asm_ast.Function(
        name=name, is_global=True, params=params, instructions=instrs,
    )


def _prog(*tls):
    return asm_ast.Program(top_level=list(tls))


@unittest.skipUnless(shutil.which("pcpp"), "pcpp not on PATH")
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
        __attribute__((zp_abi))
        void writer(uint8_t *out) { *out = 0x42; }
        __attribute__((zp_abi))
        void caller(void) {
            uint8_t x;
            writer(&x);
        }
        int main(void) { caller(); return 0; }
        """
        asm, _, _, slot_syms = self._compile(src)
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
        __attribute__((zp_abi))
        bool finder(uint8_t *out) {
            *out = target;
            return true;
        }
        __attribute__((zp_abi))
        void caller(void) {
            uint8_t row;
            if (!finder(&row)) return;
            target = row;
        }
        int main(void) { caller(); return 0; }
        """
        asm, _, _, slot_syms = self._compile(src)
        sym = "__local_caller__row"
        self.assertIn(sym, slot_syms)
        # Scan the caller function for `LoadAddress(src=Data(sym), ...)`.
        # If the assignment worked, the address-taken local resolves
        # to a Data operand (which asm_emit lowers as `LDA #<sym;
        # LDA #>sym` immediate pair). If it didn't, we'd see a
        # `LoadAddress(src=Frame(...))` instead which lowers to the
        # 6-instruction CLC; LDA FP; ADC #imm; ... runtime compute.
        caller_fn = next(
            tl for tl in asm.top_level
            if isinstance(tl, asm_ast.Function) and tl.name == "caller"
        )
        has_loadaddress_data = any(
            isinstance(instr, asm_ast.LoadAddress)
            and isinstance(instr.src, asm_ast.Data)
            and instr.src.name == sym
            for instr in caller_fn.instructions
        )
        self.assertTrue(
            has_loadaddress_data,
            "expected LoadAddress(src=Data(__local_caller__row)) in "
            "caller body — address-taken local didn't get routed to "
            "the immediate-address path",
        )


class TestSlotSymbols(unittest.TestCase):
    """`slot_symbols` mints `__local_<fn>__<source>` names for each
    address-taken Pseudo. Source name is recovered by stripping the
    `@N.` prefix."""

    def test_renames_source_var(self):
        out = slot_symbols(
            "caller", {"@9.entity_row": 0x8F}, None, None,
        )
        self.assertEqual(out, {"@9.entity_row": "__local_caller__entity_row"})

    def test_falls_back_for_temp(self):
        # A compiler-temp Pseudo (e.g. `%5`) without an `@N.` prefix
        # gets a synthesized `__local_<fn>__addr_<orig>` name.
        out = slot_symbols(
            "caller", {"%5": 0x90}, None, None,
        )
        self.assertEqual(out, {"%5": "__local_caller__addr_%5"})


if __name__ == "__main__":
    unittest.main()
