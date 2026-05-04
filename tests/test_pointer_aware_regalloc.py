"""Tests for pointer-aware (multi-byte) ZP coloring at the asm level.

Before this change, `LoadAddress.dst` names were excluded from the
asm-level interference graph entirely — they fell back to a Frame
slot via `replace_pseudoregisters_bare_exit`'s default sizing. That
was the safety fix for the byte-1-clobber bug, but it meant pointer
locals could never be ZP-coloreable.

This change recategorizes `LoadAddress.dst` as a width=2 multi-byte
coloring candidate. The TAC-level `color_graph`'s
`_blocked_bytes`/`_find_fit` already handle widths; `apply_coloring`
already substitutes `Pseudo(name, offset=k) → ZP(base+k, 0)`. So
once the interference graph reports `width=2` for these nodes, the
rest of the pipeline lights up.

Coverage:
  * The interference graph reports the LoadAddress.dst name with
    `width=2` (and its high-byte storage isn't double-allocated).
  * Two simultaneously-live address-of-static loads get distinct
    2-byte ZP blocks (no overlap).
  * A LoadAddress.dst that's only used to dereference compiles
    end-to-end and runs correctly.
  * A leaf function (zp_abi) whose only Frame need was the
    LoadAddress.dst now collapses to bare body + RTS — no
    prologue, no SSP/FP arithmetic, no callee-saved bytes.
  * Existing single-byte coloring isn't disturbed.
"""
from __future__ import annotations

import unittest

import asm_ast
from passes.optimization_asm.interference import build_interference
from passes.optimization_asm.liveness import compute_liveness
from passes.optimization_asm.ssa_construction import to_ssa
from passes.optimization_asm.regalloc import color_graph
from sim.harness import build_sim, run_c_program


# ---------------------------------------------------------------------------
# Direct interference / coloring checks (asm-IR level)
# ---------------------------------------------------------------------------


def _ps(name: str, off: int = 0) -> asm_ast.Pseudo:
    return asm_ast.Pseudo(name=name, offset=off)


def _A() -> asm_ast.Reg:
    return asm_ast.Reg(reg=asm_ast.A())


def _ret_bare() -> asm_ast.Return:
    return asm_ast.Return(save_a=False)


def _fn(*instrs, name: str = "main", params=()) -> asm_ast.Function:
    return asm_ast.Function(
        name=name,
        is_global=True,
        params=list(params),
        instructions=list(instrs),
    )


class TestInterferenceMultiByteNode(unittest.TestCase):
    def test_loadaddress_dst_is_width_2(self) -> None:
        # `int x; int *p = &x;` — at the asm IR level, this has a
        # LoadAddress(src=%x, dst=%p). %p must be a width=2 node.
        fn = _fn(
            asm_ast.LoadAddress(src=_ps("%x"), dst=_ps("%p")),
            asm_ast.Mov(src=_ps("%p", 0), dst=_A()),
            asm_ast.Mov(src=_ps("%p", 1), dst=_A()),
            _ret_bare(),
        )
        ssa = to_ssa(fn)
        liveness = compute_liveness(ssa)
        graph = build_interference(ssa, liveness)
        self.assertIn("%p", graph.nodes)
        self.assertEqual(graph.nodes["%p"].width, 2)
        # %x stays excluded (address-taken).
        self.assertNotIn("%x", graph.nodes)

    def test_two_loadaddresses_get_distinct_2byte_blocks(self) -> None:
        # Both pointers live at the same time → no overlap.
        fn = _fn(
            asm_ast.LoadAddress(src=_ps("%x"), dst=_ps("%p")),
            asm_ast.LoadAddress(src=_ps("%y"), dst=_ps("%q")),
            asm_ast.Mov(src=_ps("%p", 0), dst=_A()),
            asm_ast.Mov(src=_ps("%q", 0), dst=_A()),
            asm_ast.Mov(src=_ps("%p", 1), dst=_A()),
            asm_ast.Mov(src=_ps("%q", 1), dst=_A()),
            _ret_bare(),
        )
        ssa = to_ssa(fn)
        liveness = compute_liveness(ssa)
        graph = build_interference(ssa, liveness)
        coloring = color_graph(ssa, graph)
        p_base = coloring.assignments.get("%p")
        q_base = coloring.assignments.get("%q")
        self.assertIsNotNone(p_base)
        self.assertIsNotNone(q_base)
        # Each gets a 2-byte block; the blocks don't overlap.
        p_bytes = {p_base, p_base + 1}
        q_bytes = {q_base, q_base + 1}
        self.assertEqual(p_bytes & q_bytes, set())

    def test_pointer_coexists_with_singlebyte_neighbor(self) -> None:
        # %v is a single-byte SSA value live across the LoadAddress.
        # %p is a 2-byte value defined by LoadAddress. They interfere;
        # %v must avoid both bytes of %p's color.
        fn = _fn(
            asm_ast.Mov(src=asm_ast.Imm(value=42), dst=_ps("%v")),
            asm_ast.LoadAddress(src=_ps("%x"), dst=_ps("%p")),
            asm_ast.Mov(src=_ps("%v"), dst=_A()),
            asm_ast.Mov(src=_ps("%p", 0), dst=_A()),
            asm_ast.Mov(src=_ps("%p", 1), dst=_A()),
            _ret_bare(),
        )
        ssa = to_ssa(fn)
        liveness = compute_liveness(ssa)
        graph = build_interference(ssa, liveness)
        coloring = color_graph(ssa, graph)
        # The byte-versioned name for %v after SSA renaming.
        v_name = next(
            n for n in coloring.assignments
            if n.startswith("%v.b0.v")
        )
        v_addr = coloring.assignments[v_name]
        p_base = coloring.assignments["%p"]
        self.assertNotIn(v_addr, {p_base, p_base + 1})


# ---------------------------------------------------------------------------
# End-to-end sim tests
# ---------------------------------------------------------------------------


class TestPointerAwareRegallocSim(unittest.TestCase):
    """Programs that exercise multi-byte ZP coloring should run
    correctly under `--optimize-asm` and match the unoptimized
    reference."""

    def _both_paths(self, src: str):
        no_opt = run_c_program(src).return_int_signed()
        opt = build_sim(src, optimize=True).run().return_int_signed()
        return no_opt, opt

    def test_pointer_to_local_dereferenced(self) -> None:
        src = (
            "int main(void) {\n"
            "    int x = 42;\n"
            "    int *p = &x;\n"
            "    return *p;\n"
            "}\n"
        )
        a, b = self._both_paths(src)
        self.assertEqual(a, 42)
        self.assertEqual(b, 42)

    def test_array_subscript_in_loop(self) -> None:
        # Same shape as the interlace example but smaller — verifies
        # the LoadAddress.dst gets a 2-byte ZP block in the loop.
        src = (
            "#include <stdint.h>\n"
            "static uint16_t table[3] = {0xAA, 0xBB, 0xCC};\n"
            "int main(void) {\n"
            "    uint16_t sum = 0;\n"
            "    int8_t i;\n"
            "    for (i = 0; i < 3; i++) sum += table[i];\n"
            "    return sum;\n"
            "}\n"
        )
        expected = 0xAA + 0xBB + 0xCC  # 0x231
        self.assertEqual(self._both_paths(src), (expected, expected))


# ---------------------------------------------------------------------------
# zp_abi prologue collapse
# ---------------------------------------------------------------------------


class TestZpAbiPrologueCollapse(unittest.TestCase):
    """A `__attribute__((zp_abi))` leaf function whose only frame
    requirement was a 2-byte LoadAddress.dst slot now collapses to
    bare body + RTS."""

    def _compile_asm(self, src: str) -> str:
        from compile import _run_stage
        from preprocessor import preprocess
        return _run_stage("codegen", preprocess(src), optimize=True)

    def test_zp_abi_with_loadaddress_collapses(self) -> None:
        # Tight version of interlace_fill_p1's shape: a leaf zp_abi
        # function that uses a static array via subscript. Pre-fix,
        # the LoadAddress.dst forced a Frame slot, which forced a
        # prologue. Post-fix, the LoadAddress.dst lives in 2 ZP
        # bytes and the prologue collapses.
        src = (
            "#include <stdint.h>\n"
            "static uint8_t table[8] = {0,1,2,3,4,5,6,7};\n"
            "static uint8_t out[8];\n"
            "__attribute__((zp_abi))\n"
            "void copy(uint8_t mask) {\n"
            "    for (int8_t i = 0; i < 8; i++)\n"
            "        out[i] = table[i] & mask;\n"
            "}\n"
            "int main(void) { copy(0xFF); return out[3]; }\n"
        )
        asm = self._compile_asm(src)
        # The function's body should NOT contain prologue ceremony:
        # no SSP arithmetic, no FP capture, no callee-save STA.
        # Find the `copy:` function body.
        body = self._extract_function_body(asm, "copy")
        self.assertNotIn("; prologue:", body)
        self.assertNotIn("; epilogue", body)
        self.assertNotIn("STA   SSP", body)
        self.assertNotIn("STA   FP", body)
        # Sim it for correctness too.
        result = build_sim(src, optimize=True).run()
        self.assertEqual(result.return_int_signed(), 3)

    @staticmethod
    def _extract_function_body(asm: str, name: str) -> str:
        """Return the lines from `<name>:` up to the next top-level
        label (function or static)."""
        lines = asm.splitlines()
        out_lines: list[str] = []
        in_fn = False
        for line in lines:
            if line.startswith(f"{name}:"):
                in_fn = True
                out_lines.append(line)
                continue
            if in_fn:
                # A new top-level label (column 1, ends with ':',
                # not a `.`-prefixed local label) terminates the
                # function.
                if (
                    line and not line.startswith((" ", "\t"))
                    and line.endswith(":") and not line.startswith(".")
                    and not line.startswith(f"{name}:")
                ):
                    break
                out_lines.append(line)
        return "\n".join(out_lines)


if __name__ == "__main__":
    unittest.main()
