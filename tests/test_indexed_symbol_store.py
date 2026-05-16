"""Tests for the static-array IndexedSymbolStore fast path.

When `arr[i] = v` writes to a static-storage array whose total byte
size ≤ 256, `c99_to_tac._try_indexed_store_subscript` emits
`IndexedSymbolStore(name, byte_index, src)` instead of the general
`GetAddress + pointer arithmetic + Store` chain. tac_to_asm lowers
as 6502 absolute,X addressing on the link-time label — saves a
DPTR setup and an indirect-Y dereference, mirroring the
`IndexedLoad` rvalue fast path.

Coverage:
  * The asm shape: `STA arr,X` (and `STA arr+1,X` per byte for
    multi-byte sources) instead of `STA (DPTR),Y` chains.
  * End-to-end correctness via the sim — both single-byte and
    multi-byte stores write all bytes correctly.
  * The optimization fires only for static-storage arrays whose
    total byte size ≤ 256.
"""
from __future__ import annotations

import shutil
import unittest

from sim.harness import build_sim


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestIndexedSymbolStoreAsmShape(unittest.TestCase):
    """Source-level tests asserting the emitted asm uses absolute,X."""

    def _compile(self, src: str) -> str:
        from compile import _run_stage
        from preprocessor import preprocess
        return _run_stage("codegen", preprocess(src), optimize=True)

    def test_static_uint8_array_uses_absolute_x_store(self) -> None:
        src = (
            "#include <stdint.h>\n"
            "static uint8_t arr[100];\n"
            "void f(uint8_t i, uint8_t v) { arr[i] = v; }\n"
            "int main(void) { f(3, 7); return arr[3]; }\n"
        )
        asm = self._compile(src)
        self.assertIn("STA   arr,X", asm)
        # No DPTR staging for this access.
        # (Other code paths may stage DPTR; this is a sanity-check
        # confirming the fast path fired.)

    def test_extern_uint8_array_uses_absolute_x_store(self) -> None:
        # The fast path also fires for `extern T name[]` — the
        # incomplete-array form has `_sizeof = 0`, which trivially
        # passes the ≤ 256 check (the contract: programmer ensures
        # the defining TU sizes it to ≤ 256 bytes).
        src = (
            "#include <stdint.h>\n"
            "extern uint8_t entity_active[];\n"
            "void f(uint8_t slot) { entity_active[slot] = 1; }\n"
            "int main(void) { return 0; }\n"
        )
        asm = self._compile(src)
        self.assertIn("STA   entity_active,X", asm)

    def test_static_uint16_array_emits_two_sta(self) -> None:
        # 2-byte element type — IndexedSymbolStore lowering emits
        # two STA atoms (low byte and high byte).
        src = (
            "#include <stdint.h>\n"
            "static uint16_t arr[10];\n"
            "void f(uint8_t i, uint16_t v) { arr[i] = v; }\n"
            "int main(void) { return 0; }\n"
        )
        asm = self._compile(src)
        self.assertIn("STA   arr,X", asm)
        self.assertIn("STA   arr+1,X", asm)

    def test_large_static_array_does_not_use_fast_path(self) -> None:
        # ≥ 257 bytes — byte index can exceed 255, falls back to
        # the generic pointer-arithmetic + indirect-Y store.
        src = (
            "#include <stdint.h>\n"
            "static uint8_t big[300];\n"
            "void f(uint16_t i, uint8_t v) { big[i] = v; }\n"
            "int main(void) { return 0; }\n"
        )
        asm = self._compile(src)
        # Should NOT have `STA big,X`; should have a DPTR-staged
        # indirect-Y store instead.
        self.assertNotIn("STA   big,X", asm)


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestIndexedSymbolStoreEndToEnd(unittest.TestCase):
    """Sim-level tests: the emitted code actually writes the right
    bytes."""

    def _sim_return(self, src: str) -> int:
        sim = build_sim(src, optimize=True)
        result = sim.run(max_cycles=5_000_000)
        self.assertFalse(result.timed_out, "simulator timed out")
        return result.return_int()

    def test_uint8_store_then_read_back(self) -> None:
        src = (
            "#include <stdint.h>\n"
            "static uint8_t arr[10];\n"
            "int main(void) {\n"
            "    arr[3] = 42;\n"
            "    arr[7] = 99;\n"
            "    return arr[3] + arr[7];\n"
            "}\n"
        )
        self.assertEqual(self._sim_return(src), 42 + 99)

    def test_uint16_store_then_read_back(self) -> None:
        # 2-byte element — both bytes of the store reach memory.
        src = (
            "#include <stdint.h>\n"
            "static uint16_t arr[10];\n"
            "int main(void) {\n"
            "    arr[5] = 0x1234;\n"
            "    return arr[5];\n"
            "}\n"
        )
        self.assertEqual(self._sim_return(src) & 0xFFFF, 0x1234)

    def test_runtime_index_store(self) -> None:
        # Index isn't a compile-time constant — the runtime-index
        # path in `_try_indexed_store_subscript` fires.
        src = (
            "#include <stdint.h>\n"
            "static uint8_t arr[20];\n"
            "void put(uint8_t i, uint8_t v) { arr[i] = v; }\n"
            "int main(void) {\n"
            "    put(5, 50);\n"
            "    put(12, 120);\n"
            "    return arr[5] + arr[12];\n"
            "}\n"
        )
        self.assertEqual(self._sim_return(src), 50 + 120)
