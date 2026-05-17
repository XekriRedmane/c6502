"""Tests for asm-level move coalescing.

The pass identifies move-related Pseudo pairs (connected by an
explicit Mov or by a Phi argument) and merges them in the
interference graph when they don't interfere. After coloring,
the merged class shares one ZP slot, and Phi-destruction Movs
between members become self-Movs that `asm_emit`'s self-Mov
peephole drops.

Coverage:
  * Direct unit tests on synthetic functions: simple Phi
    coalescing, Mov coalescing, interfering pair NOT coalesced,
    different-width pair NOT coalesced, chained merges
    (transitive), excluded-name (static) NOT coalesced.
  * End-to-end via the optimizer: a `for (uchar i; i < N; i++)`
    loop where the asm-SSA Phi destruction would otherwise route
    through a temp — coalescing eliminates the temp and the INC
    peephole collapses the increment to a bare `INC` (or
    `INC + BNE + INC` for multi-byte i).
"""
from __future__ import annotations

import shutil
import unittest

import asm_ast
from passes.optimization.interference import (
    InterferenceGraph,
    InterferenceNode,
)
from passes.optimization_asm.coalescing import coalesce_moves
from sim.harness import build_sim


def _node(name: str, *, width: int = 1, lac: bool = False) -> InterferenceNode:
    return InterferenceNode(
        name=name, width=width, lives_across_call=lac,
    )


def _pseudo(name: str, offset: int = 0) -> asm_ast.Pseudo:
    return asm_ast.Pseudo(name=name, offset=offset)


def _fn(instrs: list[asm_ast.Type_instruction]) -> asm_ast.Function:
    return asm_ast.Function(
        name="f", is_global=True, params=[], instructions=instrs,
    )


def _graph_with_nodes(*names: str) -> InterferenceGraph:
    g = InterferenceGraph()
    for n in names:
        g.nodes[n] = _node(n)
    return g


class TestCoalescingUnit(unittest.TestCase):
    """Direct calls to coalesce_moves on synthetic functions/graphs."""

    def test_mov_pair_no_interference_coalesces(self) -> None:
        # Mov(a, b) with a and b non-interfering → b merged into a.
        instrs = [
            asm_ast.Mov(src=_pseudo("a"), dst=_pseudo("b")),
        ]
        graph = _graph_with_nodes("a", "b")
        result = coalesce_moves(_fn(instrs), graph)
        # One of {a, b} is removed from the graph; the other stays.
        survivor = "a" if "a" in graph.nodes else "b"
        removed = "b" if survivor == "a" else "a"
        self.assertNotIn(removed, graph.nodes)
        self.assertEqual(result.resolve(removed), survivor)
        self.assertEqual(result.resolve(survivor), survivor)

    def test_a_routed_pair_no_interference_coalesces(self) -> None:
        # `Mov(a, A); Mov(A, b)` is a logical copy a → b that the
        # extended `_move_related_pairs` enumeration picks up.
        # Without this, the `<<8 | byte` byte-construction idiom
        # (`Mov(Imm(0), A); Or(a, A); Mov(A, b)` → after the
        # absorb_zero_load fold) leaves a and b at distinct ZP
        # colors and the b2 round-trip survives in step_pos.asm.
        a_reg = asm_ast.Reg(reg=asm_ast.A())
        instrs = [
            asm_ast.Mov(src=_pseudo("a"), dst=a_reg),
            asm_ast.Mov(src=a_reg, dst=_pseudo("b")),
        ]
        graph = _graph_with_nodes("a", "b")
        result = coalesce_moves(_fn(instrs), graph)
        survivor = "a" if "a" in graph.nodes else "b"
        removed = "b" if survivor == "a" else "a"
        self.assertNotIn(removed, graph.nodes)
        self.assertEqual(result.resolve(removed), survivor)

    def test_a_routed_volatile_not_coalesced(self) -> None:
        # A volatile half of the pair guards an observable load /
        # store that must not be merged away.
        a_reg = asm_ast.Reg(reg=asm_ast.A())
        instrs = [
            asm_ast.Mov(
                src=_pseudo("a"), dst=a_reg, is_volatile=True,
            ),
            asm_ast.Mov(src=a_reg, dst=_pseudo("b")),
        ]
        graph = _graph_with_nodes("a", "b")
        coalesce_moves(_fn(instrs), graph)
        # Both names remain in the graph — no merge.
        self.assertIn("a", graph.nodes)
        self.assertIn("b", graph.nodes)

    def test_phi_args_coalesce_with_dst(self) -> None:
        # A Phi with two arg sources merges all three into one
        # equivalence class (assuming none interfere).
        instrs = [
            asm_ast.Phi(
                dst=_pseudo("phi"),
                args=[
                    asm_ast.AsmPhiArg(pred_label="L1", source=_pseudo("a")),
                    asm_ast.AsmPhiArg(pred_label="L2", source=_pseudo("b")),
                ],
            ),
        ]
        graph = _graph_with_nodes("phi", "a", "b")
        result = coalesce_moves(_fn(instrs), graph)
        # Only one node remains in the graph.
        self.assertEqual(len(graph.nodes), 1)
        survivor = next(iter(graph.nodes))
        # Every member resolves to the survivor.
        self.assertEqual(result.resolve("phi"), survivor)
        self.assertEqual(result.resolve("a"), survivor)
        self.assertEqual(result.resolve("b"), survivor)

    def test_interfering_pair_not_coalesced(self) -> None:
        # Mov(a, b) but a-b interfere → no merge.
        instrs = [
            asm_ast.Mov(src=_pseudo("a"), dst=_pseudo("b")),
        ]
        graph = _graph_with_nodes("a", "b")
        graph.add_edge("a", "b")
        result = coalesce_moves(_fn(instrs), graph)
        self.assertIn("a", graph.nodes)
        self.assertIn("b", graph.nodes)
        self.assertEqual(result.representative, {})

    def test_different_widths_not_coalesced(self) -> None:
        # 1-byte and 2-byte nodes can't coalesce — the coloring
        # pool's width-aware fit assumes uniform width.
        instrs = [
            asm_ast.Mov(src=_pseudo("a"), dst=_pseudo("b")),
        ]
        graph = InterferenceGraph()
        graph.nodes["a"] = _node("a", width=1)
        graph.nodes["b"] = _node("b", width=2)
        result = coalesce_moves(_fn(instrs), graph)
        self.assertIn("a", graph.nodes)
        self.assertIn("b", graph.nodes)
        self.assertEqual(result.representative, {})

    def test_excluded_name_not_coalesced(self) -> None:
        # Static / address-taken / param names aren't in the graph
        # at all. Coalescing skips them — no merge attempted.
        instrs = [
            asm_ast.Mov(src=_pseudo("static_name"), dst=_pseudo("a")),
        ]
        graph = _graph_with_nodes("a")  # static_name absent
        result = coalesce_moves(_fn(instrs), graph)
        self.assertIn("a", graph.nodes)
        self.assertEqual(result.representative, {})

    def test_chained_merges_resolve_transitively(self) -> None:
        # Mov(a, b); Mov(b, c). After processing, all three merge
        # into the same class.
        instrs = [
            asm_ast.Mov(src=_pseudo("a"), dst=_pseudo("b")),
            asm_ast.Mov(src=_pseudo("b"), dst=_pseudo("c")),
        ]
        graph = _graph_with_nodes("a", "b", "c")
        result = coalesce_moves(_fn(instrs), graph)
        survivor = next(iter(graph.nodes))
        self.assertEqual(result.resolve("a"), survivor)
        self.assertEqual(result.resolve("b"), survivor)
        self.assertEqual(result.resolve("c"), survivor)
        self.assertEqual(len(graph.nodes), 1)

    def test_self_loop_avoided_via_chain(self) -> None:
        # Mov(a, b); Mov(b, a) — the second pair is already merged.
        # Coalescing treats it as a no-op.
        instrs = [
            asm_ast.Mov(src=_pseudo("a"), dst=_pseudo("b")),
            asm_ast.Mov(src=_pseudo("b"), dst=_pseudo("a")),
        ]
        graph = _graph_with_nodes("a", "b")
        result = coalesce_moves(_fn(instrs), graph)
        self.assertEqual(len(graph.nodes), 1)

    def test_lives_across_call_ored(self) -> None:
        # Merging two nodes ORs their lives_across_call flags. If
        # either side lives across a call, the merged class needs
        # callee-saved.
        instrs = [
            asm_ast.Mov(src=_pseudo("a"), dst=_pseudo("b")),
        ]
        graph = InterferenceGraph()
        graph.nodes["a"] = _node("a", lac=False)
        graph.nodes["b"] = _node("b", lac=True)
        coalesce_moves(_fn(instrs), graph)
        survivor = next(iter(graph.nodes))
        self.assertTrue(graph.nodes[survivor].lives_across_call)

    def test_neighbor_edges_redirected(self) -> None:
        # When b is merged into a, b's neighbors (e.g., c) become
        # neighbors of a.
        instrs = [
            asm_ast.Mov(src=_pseudo("a"), dst=_pseudo("b")),
        ]
        graph = _graph_with_nodes("a", "b", "c")
        graph.add_edge("b", "c")
        coalesce_moves(_fn(instrs), graph)
        # c should now be a neighbor of whichever node survived.
        survivor = "a" if "a" in graph.nodes else "b"
        self.assertIn("c", graph.adj[survivor])

    def test_offset_nonzero_pseudo_not_coalesced(self) -> None:
        # Asm-SSA gives renamed Pseudos offset=0; non-zero offset
        # marks an unrenamed multi-byte name. Don't coalesce
        # those — they need contiguous bytes, not the same byte.
        instrs = [
            asm_ast.Mov(
                src=_pseudo("a", offset=0),
                dst=_pseudo("b", offset=1),
            ),
        ]
        graph = _graph_with_nodes("a", "b")
        result = coalesce_moves(_fn(instrs), graph)
        self.assertIn("a", graph.nodes)
        self.assertIn("b", graph.nodes)
        self.assertEqual(result.representative, {})


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestCoalescingEndToEnd(unittest.TestCase):
    """Programs going through the full pipeline."""

    def _compile(self, src: str) -> str:
        from compile import _run_stage
        from preprocessor import preprocess
        return _run_stage("codegen", preprocess(src), optimize=True)

    def test_uchar_loop_increment_collapses_to_inc(self) -> None:
        # A loop counter that asm-SSA Phi destruction would route
        # through a temp without coalescing. Coalescing merges the
        # Phi-related SSA names into one ZP slot, the round-trip
        # Mov becomes a self-Mov dropped at emit, and the INC
        # peephole collapses the in-place ADC chain to a bare INC.
        src = (
            "#include <stdint.h>\n"
            "uint8_t total = 0;\n"
            "int main(void) {\n"
            "    for (uint8_t i = 0; i < 5; i++) total = total + i;\n"
            "    return total;\n"
            "}\n"
        )
        asm = self._compile(src)
        # Find the .loop@0_continue: block — between that label
        # and the next label, we should see a single `INC $XX`
        # for `i++`, not the LDA-CLC-ADC-STA-LDA-STA chain.
        cont_idx = asm.find(".loop@0_continue:")
        self.assertNotEqual(cont_idx, -1)
        # Slice from cont_idx to the next label (line starting with
        # `.` after a newline).
        rest = asm[cont_idx:]
        # The continue block should contain `INC` somewhere before
        # the next label or branch out — and crucially not `ADC #$01`
        # (which would be the unfolded ADC-chain shape).
        # Simple substring check: between continue and break/start.
        end_idx = rest.find(".loop@0_break")
        if end_idx == -1:
            end_idx = len(rest)
        block = rest[:end_idx]
        import re
        self.assertRegex(block, r"INC\s+__local_\w+__\w+")
        self.assertNotIn("ADC   #$01", block)

    def test_uchar_loop_correct_sum(self) -> None:
        # Same loop as above, but verify the simulator actually
        # computes 0+1+2+3+4 = 10 with the coalesced code.
        src = (
            "#include <stdint.h>\n"
            "int main(void) {\n"
            "    uint8_t total = 0;\n"
            "    for (uint8_t i = 0; i < 5; i++) total = total + i;\n"
            "    return total;\n"
            "}\n"
        )
        sim = build_sim(src, optimize=True)
        result = sim.run(max_cycles=5_000_000)
        self.assertFalse(result.timed_out)
        self.assertEqual(result.return_int() & 0xFFFF, 10)


if __name__ == "__main__":
    unittest.main()
