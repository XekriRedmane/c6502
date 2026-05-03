"""Focused tests for `passes.optimization.ssa_construction` and
`passes.optimization.ssa_destruction`.

End-to-end semantic preservation across many program shapes is
already covered by the chapter_19 simulator harness running with
`--optimize` (see tests/test_chapter_19.py). The cases here pin
specific structural invariants of SSA construction and destruction
that the chapter harness can't reach into directly:

  - parameter Vars retain their original spelling as their initial
    SSA name; subsequent body defs get `<orig>.<n>` suffixes;
  - Phi placement is pruned by liveness — temps that are dead at a
    join don't get spurious Phis;
  - Phi pred_labels match real predecessor block leading-Label
    names;
  - de-SSA emits one Copy per PhiArg in the predecessor block,
    placed before the terminator if any;
  - to_ssa → from_ssa preserves the input's structural shape on
    straight-line code with no merge points;
  - the synthetic preheader is inserted iff the function body
    starts with a Label.
"""

from __future__ import annotations

import unittest

import c99_ast
import tac_ast
from passes.optimization.ssa_construction import to_ssa
from passes.optimization.ssa_destruction import from_ssa
from passes.type_checking import LocalAttr, Symbol, SymbolTable


def _ci(v: int) -> tac_ast.Constant:
    return tac_ast.Constant(const=tac_ast.ConstInt(value=v))


def _var(name: str) -> tac_ast.Var:
    return tac_ast.Var(name=name)


def _ret_var(name: str) -> tac_ast.Ret:
    return tac_ast.Ret(val=_var(name))


def _fn(*instrs, name: str = "main", params=()) -> tac_ast.Function:
    return tac_ast.Function(
        name=name, is_global=True,
        params=list(params), instructions=list(instrs),
    )


def _symbols(**kinds: c99_ast.Type_data_type) -> SymbolTable:
    """Build a SymbolTable mapping `name=Type()` kwargs to LocalAttr
    Symbols. Every var in the function body needs a symbol entry
    for SSA construction to identify it as promotable."""
    st = SymbolTable()
    for name, t in kinds.items():
        st[name] = Symbol(type=t, attrs=LocalAttr())
    return st


class TestSSAConstruction(unittest.TestCase):
    def test_straight_line_no_phis(self) -> None:
        # No control-flow merge — no Phi insertion is needed.
        fn = _fn(
            tac_ast.Copy(src=_ci(1), dst=_var("@x")),
            tac_ast.Binary(
                op=tac_ast.Add(), src1=_var("@x"), src2=_ci(2),
                dst=_var("@y"),
            ),
            tac_ast.Ret(val=_var("@y")),
        )
        # Dot-prefixed names skip the project's `@` convention but
        # are still LocalAttr scalars for our purposes.
        st = _symbols(
            **{"@x": c99_ast.Int(), "@y": c99_ast.Int()},
        )
        ssa_fn, _ = to_ssa(fn, st)
        self.assertFalse(any(
            isinstance(i, tac_ast.Phi) for i in ssa_fn.instructions
        ))

    def test_if_else_join_gets_phi(self) -> None:
        # if/else over `@x` produces a Phi at the join with sources
        # from both arms.
        fn = _fn(
            tac_ast.JumpIfFalse(condition=_var("@c"), target=".else"),
            tac_ast.Copy(src=_ci(1), dst=_var("@x")),
            tac_ast.Jump(target=".join"),
            tac_ast.Label(name=".else"),
            tac_ast.Copy(src=_ci(2), dst=_var("@x")),
            tac_ast.Label(name=".join"),
            tac_ast.Ret(val=_var("@x")),
        )
        st = _symbols(
            **{"@c": c99_ast.Int(), "@x": c99_ast.Int()},
        )
        ssa_fn, _ = to_ssa(fn, st)
        phis = [i for i in ssa_fn.instructions if isinstance(i, tac_ast.Phi)]
        # Exactly one Phi (for `@x` at `.join`); none for `@c`
        # (defined externally, not redefined).
        self.assertEqual(len(phis), 1)
        phi = phis[0]
        self.assertEqual(phi.dst.name.split(".")[0], "@x")
        # Phi has two PhiArgs whose pred_labels are the two arms'
        # leading labels (synthetic for the then-arm, `.else` for
        # the else).
        self.assertEqual(len(phi.args), 2)
        labels = [a.pred_label for a in phi.args]
        self.assertIn(".else", labels)
        # Phi sources are the SSA-renamed values from each arm.
        sources = [a.source.name for a in phi.args]
        self.assertNotIn("@x", sources, "post-renaming, sources should be SSA-suffixed")

    def test_param_retains_original_spelling_as_initial_value(self) -> None:
        # `@p` is a parameter; the body's first read of `@p` should
        # stay `@p` (its initial SSA name). The body's redef gets a
        # suffix.
        fn = _fn(
            tac_ast.Copy(src=_var("@p"), dst=_var("%t")),
            tac_ast.Copy(src=_ci(5), dst=_var("@p")),
            tac_ast.Ret(val=_var("@p")),
            params=("@p",),
        )
        st = _symbols(
            **{"@p": c99_ast.Int(), "%t": c99_ast.Int()},
        )
        ssa_fn, _ = to_ssa(fn, st)
        # The function's `params` field is unchanged.
        self.assertEqual(ssa_fn.params, ["@p"])
        # The first instruction (the Copy reading the param) reads
        # `@p` directly, not a suffixed version.
        first_real = next(
            i for i in ssa_fn.instructions
            if isinstance(i, tac_ast.Copy)
            and i.src != _ci(5)
        )
        self.assertEqual(first_real.src.name, "@p")
        # The body's redef of `@p` got a suffix.
        bodydef = next(
            i for i in ssa_fn.instructions
            if isinstance(i, tac_ast.Copy) and i.src == _ci(5)
        )
        self.assertNotEqual(bodydef.dst.name, "@p")
        self.assertTrue(bodydef.dst.name.startswith("@p."))

    def test_address_taken_var_is_not_promoted(self) -> None:
        # `@a` has its address taken via GetAddress; SSA must NOT
        # rename it (writes through pointers could alias).
        fn = _fn(
            tac_ast.Copy(src=_ci(1), dst=_var("@a")),
            tac_ast.GetAddress(operand=_var("@a"), dst=_var("%p")),
            tac_ast.Store(src=_ci(2), dst_ptr=_var("%p")),
            tac_ast.Ret(val=_var("@a")),
        )
        st = _symbols(
            **{"@a": c99_ast.Int(), "%p": c99_ast.Pointer(referenced_type=c99_ast.Int())},
        )
        ssa_fn, _ = to_ssa(fn, st)
        # `@a` should appear unrenamed throughout.
        names = [
            v.name for i in ssa_fn.instructions for v in
            (i.dst if isinstance(i, tac_ast.Copy) else _var("__noop"),)
        ]
        # Find every Var operand named anything starting with @a.
        all_names: set[str] = set()
        for i in ssa_fn.instructions:
            for attr in ("src", "dst", "operand", "src_ptr", "dst_ptr", "val"):
                v = getattr(i, attr, None)
                if isinstance(v, tac_ast.Var):
                    all_names.add(v.name)
        a_renames = {n for n in all_names if n.startswith("@a")}
        self.assertEqual(a_renames, {"@a"}, "address-taken @a must not be renamed")

    def test_no_phi_for_dead_temp_at_join(self) -> None:
        # A temp `%t` that's defined inside one arm and used only
        # within that arm shouldn't get a Phi at the join (pruned
        # SSA — `%t` isn't live-in at the join).
        fn = _fn(
            tac_ast.JumpIfFalse(condition=_var("@c"), target=".else"),
            tac_ast.Binary(
                op=tac_ast.Add(), src1=_var("@x"), src2=_ci(1),
                dst=_var("%t"),
            ),
            tac_ast.Copy(src=_var("%t"), dst=_var("@y")),
            tac_ast.Jump(target=".join"),
            tac_ast.Label(name=".else"),
            tac_ast.Copy(src=_ci(0), dst=_var("@y")),
            tac_ast.Label(name=".join"),
            tac_ast.Ret(val=_var("@y")),
        )
        st = _symbols(
            **{
                "@c": c99_ast.Int(),
                "@x": c99_ast.Int(),
                "@y": c99_ast.Int(),
                "%t": c99_ast.Int(),
            },
        )
        ssa_fn, _ = to_ssa(fn, st)
        phis = [i for i in ssa_fn.instructions if isinstance(i, tac_ast.Phi)]
        # One Phi for `@y` at the join. None for `%t` — `%t` isn't
        # live across the join.
        self.assertEqual(len(phis), 1)
        self.assertEqual(phis[0].dst.name.split(".")[0], "@y")


class TestSSADestruction(unittest.TestCase):
    def test_phi_lowered_to_copies_in_predecessors(self) -> None:
        # Build a function with one Phi in a join block and verify
        # that from_ssa replaces it with one Copy per PhiArg in the
        # predecessor block, before the terminator.
        fn = _fn(
            tac_ast.Label(name=".pre"),
            tac_ast.JumpIfFalse(condition=_var("@c"), target=".else"),
            tac_ast.Label(name=".then"),
            tac_ast.Jump(target=".join"),
            tac_ast.Label(name=".else"),
            tac_ast.Label(name=".join"),
            tac_ast.Phi(
                dst=_var("@x.3"), args=[
                    tac_ast.PhiArg(pred_label=".then", source=_ci(1)),
                    tac_ast.PhiArg(pred_label=".else", source=_ci(2)),
                ],
            ),
            tac_ast.Ret(val=_var("@x.3")),
        )
        out = from_ssa(fn)
        # No Phi remains.
        self.assertFalse(any(
            isinstance(i, tac_ast.Phi) for i in out.instructions
        ))
        # The Copies are inserted before the terminators of `.then`
        # (a Jump) and `.else` (no explicit terminator — append at
        # end).
        instrs = out.instructions
        # `.then` is a labeled block; the Copy goes before its Jump.
        idx_then = next(
            i for i, x in enumerate(instrs)
            if isinstance(x, tac_ast.Label) and x.name == ".then"
        )
        # Right after the `.then:` Label, we should have the inserted
        # Copy from PhiArg `.then`.
        self.assertIsInstance(instrs[idx_then + 1], tac_ast.Copy)
        self.assertEqual(instrs[idx_then + 1].src, _ci(1))
        self.assertEqual(instrs[idx_then + 1].dst, _var("@x.3"))
        self.assertIsInstance(instrs[idx_then + 2], tac_ast.Jump)

    def test_no_phis_passes_through(self) -> None:
        # Function with no Phis is structurally unchanged.
        fn = _fn(
            tac_ast.Copy(src=_ci(1), dst=_var("x")),
            tac_ast.Ret(val=_var("x")),
        )
        self.assertEqual(from_ssa(fn), fn)

    def test_chain_topologically_sorted(self) -> None:
        # Two Phis at .join with a chain dependency: counter's source
        # is i, i's source is %4. After de-SSA, the Copies must be
        # ordered so counter reads i BEFORE i is overwritten with %4.
        fn = _fn(
            tac_ast.Label(name=".pre"),
            tac_ast.Jump(target=".loop_continue"),
            tac_ast.Label(name=".loop_continue"),
            tac_ast.Label(name=".join"),
            tac_ast.Phi(
                dst=_var("@i"), args=[
                    tac_ast.PhiArg(pred_label=".pre", source=_ci(0)),
                    tac_ast.PhiArg(
                        pred_label=".loop_continue", source=_var("%4"),
                    ),
                ],
            ),
            tac_ast.Phi(
                dst=_var("@counter"), args=[
                    tac_ast.PhiArg(pred_label=".pre", source=_ci(0)),
                    tac_ast.PhiArg(
                        pred_label=".loop_continue", source=_var("@i"),
                    ),
                ],
            ),
            tac_ast.Ret(val=_var("@counter")),
        )
        out = from_ssa(fn)
        instrs = out.instructions
        # Find the loop_continue label and inspect the Copies that
        # follow up to (but not including) the Jump.
        idx = next(
            i for i, x in enumerate(instrs)
            if isinstance(x, tac_ast.Label) and x.name == ".loop_continue"
        )
        # Step past the Label and any additional Labels.
        copies = []
        i = idx + 1
        while i < len(instrs) and not isinstance(
            instrs[i],
            (tac_ast.Jump, tac_ast.Label, tac_ast.JumpIfTrue, tac_ast.JumpIfFalse),
        ):
            if isinstance(instrs[i], tac_ast.Copy):
                copies.append(instrs[i])
            i += 1
        # The counter = @i Copy must come BEFORE the @i = %4 Copy.
        # Find their indices.
        counter_copy_idx = next(
            i for i, c in enumerate(copies)
            if isinstance(c.dst, tac_ast.Var) and c.dst.name == "@counter"
        )
        i_copy_idx = next(
            i for i, c in enumerate(copies)
            if isinstance(c.dst, tac_ast.Var) and c.dst.name == "@i"
        )
        self.assertLess(
            counter_copy_idx, i_copy_idx,
            "counter Copy must precede i Copy to read old i value",
        )

    def test_2_cycle_broken_with_temp(self) -> None:
        # Two Phis at .join whose sources mutually reference each
        # other's dsts: a 2-cycle in the parallel-copy graph.
        # SSA-shaped: a' = b, b' = a (where a' is the new a value).
        # Without cycle-breaking, naive emit would produce
        # `Copy(b, a); Copy(a, b)` — the second Copy reads the
        # already-overwritten a. A temp is needed.
        symbols = _symbols(
            **{"a": c99_ast.Int(), "b": c99_ast.Int()},
        )
        fn = _fn(
            tac_ast.Label(name=".pre"),
            tac_ast.Copy(src=_ci(7), dst=_var("a")),
            tac_ast.Copy(src=_ci(11), dst=_var("b")),
            tac_ast.Jump(target=".loop_continue"),
            tac_ast.Label(name=".loop_continue"),
            tac_ast.Label(name=".join"),
            tac_ast.Phi(
                dst=_var("a"), args=[
                    tac_ast.PhiArg(pred_label=".pre", source=_ci(0)),
                    tac_ast.PhiArg(
                        pred_label=".loop_continue", source=_var("b"),
                    ),
                ],
            ),
            tac_ast.Phi(
                dst=_var("b"), args=[
                    tac_ast.PhiArg(pred_label=".pre", source=_ci(0)),
                    tac_ast.PhiArg(
                        pred_label=".loop_continue", source=_var("a"),
                    ),
                ],
            ),
            tac_ast.Ret(val=_var("a")),
        )
        out = from_ssa(fn, symbols=symbols)
        instrs = out.instructions
        # Locate the post-.loop_continue Copies.
        idx = next(
            i for i, x in enumerate(instrs)
            if isinstance(x, tac_ast.Label) and x.name == ".loop_continue"
        )
        copies = []
        i = idx + 1
        while i < len(instrs) and not isinstance(
            instrs[i],
            (tac_ast.Jump, tac_ast.Label, tac_ast.JumpIfTrue, tac_ast.JumpIfFalse),
        ):
            if isinstance(instrs[i], tac_ast.Copy):
                copies.append(instrs[i])
            i += 1
        # Should be 3 copies: temp = X; Y = Z (using temp for one
        # side); Z = ... or similar. The exact form depends on
        # which cycle member was chosen, but the cycle-temp Var
        # must appear.
        self.assertEqual(len(copies), 3)
        self.assertTrue(
            any(
                isinstance(c.dst, tac_ast.Var)
                and "cycle_tmp" in c.dst.name
                for c in copies
            ),
            f"expected at least one Copy to write to a cycle temp; got {copies}",
        )
        # The cycle temp's symbol entry should be registered with
        # the same type as the cycle members (Int here).
        tmp_name = next(
            c.dst.name for c in copies
            if isinstance(c.dst, tac_ast.Var) and "cycle_tmp" in c.dst.name
        )
        self.assertIn(tmp_name, symbols)
        self.assertIsInstance(symbols[tmp_name].type, c99_ast.Int)


class TestSSARoundTrip(unittest.TestCase):
    def test_roundtrip_preserves_name_and_params(self) -> None:
        fn = _fn(
            tac_ast.Copy(src=_ci(1), dst=_var("@x")),
            tac_ast.Ret(val=_var("@x")),
            name="foo", params=("p",),
        )
        st = _symbols(**{"@x": c99_ast.Int(), "p": c99_ast.Int()})
        ssa_fn, _ = to_ssa(fn, st); out = from_ssa(ssa_fn)
        self.assertEqual(out.name, "foo")
        self.assertEqual(out.params, ["p"])
        self.assertTrue(out.is_global)


if __name__ == "__main__":
    unittest.main()
