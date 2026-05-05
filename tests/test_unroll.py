"""Behavioral tests for `#pragma c6502 loop unroll(...)`.

Coverage:
  Surface plumbing (commit 1):
  - Pragma recognition by the preprocessor (enable / disable /
    unknown / other vendor / pragma once / whitespace flex).
  - Parser captures `unroll_annotation` on the matching ForStmt.
  - Annotation rides through every AST-level pass.

  Unroll pass (commit 2):
  - Canonical loops unroll: ascending / descending, increment /
    decrement / += K / -= K, all six supported integer types.
  - Body with locals: each iteration scopes its own copy.
  - Nested annotated loops: outer × inner clones.
  - Zero-iteration loops produce empty Compounds.
  - Unannotated loops untouched.
  - Recognizer rejects: break / continue / goto / labeled-stmt
    in body, induction-var assignment / address-of / re-mod,
    inner shadow declaration, non-canonical condition / init /
    post shapes, infinite loops, iteration cap exceeded.
"""
from __future__ import annotations

import unittest

import c99_ast
from parser import ParserError, parse
from passes.identifier_resolution import resolve_program
from passes.label_resolution import resolve_program as resolve_labels
from passes.loop_labeling import label_program
from passes.optimization_ast.unroll import (
    MAX_ITERATIONS,
    UnrollError,
    unroll_program,
)
from passes.string_lifting import lift_program
from preprocessor import PreprocessorError, preprocess


def _for_stmts(src: str) -> list[c99_ast.ForStmt]:
    """Preprocess + parse + return every ForStmt in source order."""
    pp = preprocess(src)
    prog = parse(pp)
    found: list[c99_ast.ForStmt] = []

    def walk(node):
        if isinstance(node, c99_ast.ForStmt):
            found.append(node)
        if hasattr(node, "__dataclass_fields__"):
            for f in node.__dataclass_fields__:
                walk(getattr(node, f))
        elif isinstance(node, list):
            for item in node:
                walk(item)

    walk(prog)
    return found


_PROG = """
int main(void) {{
    {pragma}
    for (int i = 0; i < 4; i++) {{ }}
    return 0;
}}
"""


class TestUnrollPragmaPreprocess(unittest.TestCase):
    def test_enable_rewrites_to_sentinel(self) -> None:
        src = "#pragma c6502 loop unroll(enable)\nint x;\n"
        out = preprocess(src)
        self.assertIn("__c6502_pragma_unroll__", out)
        # Line count preserved so error messages reference the right line.
        self.assertEqual(out.count("\n"), src.count("\n"))

    def test_disable_strips_to_blank(self) -> None:
        src = "#pragma c6502 loop unroll(disable)\nint x;\n"
        out = preprocess(src)
        self.assertNotIn("c6502", out)
        self.assertEqual(out.count("\n"), src.count("\n"))

    def test_unknown_c6502_pragma_rejected(self) -> None:
        with self.assertRaises(PreprocessorError) as cm:
            preprocess("#pragma c6502 wat\nint x;\n")
        self.assertIn("c6502", str(cm.exception))
        self.assertIn("line 1", str(cm.exception))

    def test_other_vendor_pragma_stripped(self) -> None:
        out = preprocess("#pragma GCC something\nint x;\n")
        self.assertNotIn("pragma", out)

    def test_pragma_once_handled_by_pcpp(self) -> None:
        # pcpp consumes `#pragma once`; our post-scan never sees it.
        out = preprocess("#pragma once\nint x;\n")
        self.assertNotIn("pragma", out)

    def test_whitespace_flex(self) -> None:
        out = preprocess(
            "#  pragma   c6502   loop  unroll ( enable )\nint x;\n",
        )
        self.assertIn("__c6502_pragma_unroll__", out)


class TestUnrollPragmaParse(unittest.TestCase):
    def test_enable_sets_unroll_annotation(self) -> None:
        loops = _for_stmts(_PROG.format(
            pragma="#pragma c6502 loop unroll(enable)",
        ))
        self.assertEqual(len(loops), 1)
        self.assertEqual(loops[0].unroll_annotation, "unroll")

    def test_disable_leaves_annotation_none(self) -> None:
        loops = _for_stmts(_PROG.format(
            pragma="#pragma c6502 loop unroll(disable)",
        ))
        self.assertEqual(len(loops), 1)
        self.assertIsNone(loops[0].unroll_annotation)

    def test_no_pragma_leaves_annotation_none(self) -> None:
        loops = _for_stmts(_PROG.format(pragma=""))
        self.assertEqual(len(loops), 1)
        self.assertIsNone(loops[0].unroll_annotation)

    def test_pragma_only_attaches_to_immediately_next_loop(self) -> None:
        src = """
        int main(void) {
            #pragma c6502 loop unroll(enable)
            for (int i = 0; i < 4; i++) { }
            for (int j = 0; j < 4; j++) { }
            return 0;
        }
        """
        loops = _for_stmts(src)
        self.assertEqual(len(loops), 2)
        self.assertEqual(loops[0].unroll_annotation, "unroll")
        self.assertIsNone(loops[1].unroll_annotation)

    def test_pragma_without_following_for_is_parse_error(self) -> None:
        src = """
        int main(void) {
            #pragma c6502 loop unroll(enable)
            return 0;
        }
        """
        with self.assertRaises(Exception):
            parse(preprocess(src))


class TestUnrollPragmaPipelinePassthrough(unittest.TestCase):
    """The annotation rides through every AST-level pass unchanged."""

    def _annotated_after(self, transform) -> bool:
        src = _PROG.format(pragma="#pragma c6502 loop unroll(enable)")
        prog = transform(parse(preprocess(src)))
        loops: list[c99_ast.ForStmt] = []

        def walk(node):
            if isinstance(node, c99_ast.ForStmt):
                loops.append(node)
            if hasattr(node, "__dataclass_fields__"):
                for f in node.__dataclass_fields__:
                    walk(getattr(node, f))
            elif isinstance(node, list):
                for item in node:
                    walk(item)

        walk(prog)
        self.assertEqual(len(loops), 1)
        return loops[0].unroll_annotation == "unroll"

    def test_string_lifting_preserves_annotation(self) -> None:
        self.assertTrue(self._annotated_after(lift_program))

    def test_identifier_resolution_preserves_annotation(self) -> None:
        self.assertTrue(self._annotated_after(
            lambda p: resolve_program(lift_program(p)),
        ))

    def test_label_resolution_preserves_annotation(self) -> None:
        self.assertTrue(self._annotated_after(
            lambda p: resolve_labels(resolve_program(lift_program(p))),
        ))

    def test_loop_labeling_preserves_annotation(self) -> None:
        self.assertTrue(self._annotated_after(
            lambda p: label_program(
                resolve_labels(resolve_program(lift_program(p))),
            ),
        ))


def _unrolled(body: str) -> c99_ast.Type_program:
    """Wrap `body` in `int main(void) { ... }`, run preprocess →
    parse → unroll, return the resulting program."""
    src = "int main(void) {\n" + body + "\n    return 0;\n}\n"
    return unroll_program(parse(preprocess(src)))


def _main_block_items(prog: c99_ast.Type_program) -> list[c99_ast.Type_block_item]:
    fn = prog.declaration[0].function_decl
    return fn.body.block_item


def _collect_constants(node) -> list[int]:
    """Pre-order int values of every integer Constant in subtree."""
    out: list[int] = []
    if isinstance(node, c99_ast.Constant) and hasattr(node.const, "value"):
        out.append(node.const.value)
    if hasattr(node, "__dataclass_fields__"):
        for f in node.__dataclass_fields__:
            out.extend(_collect_constants(getattr(node, f)))
    elif isinstance(node, list):
        for item in node:
            out.extend(_collect_constants(item))
    return out


def _outer_iter_count(prog: c99_ast.Type_program) -> int:
    """The unrolled for-loop becomes a `Compound` of N S(Compound) items.
    Count those iteration-statements in main's first non-init item."""
    items = _main_block_items(prog)
    # Skip leading declarations to reach the unrolled loop (first S).
    for bi in items:
        if isinstance(bi, c99_ast.S) and isinstance(bi.statement, c99_ast.Compound):
            return len(bi.statement.block.block_item)
    raise AssertionError("no unrolled compound found in main")


class TestUnrollAscending(unittest.TestCase):
    def test_simple_lt_increment(self) -> None:
        prog = _unrolled("""
        int s = 0;
        #pragma c6502 loop unroll(enable)
        for (int i = 0; i < 4; i++) s += i;
        """)
        self.assertEqual(_outer_iter_count(prog), 4)
        # The substituted iv values appear in source-order in the body
        # cloned per iteration. Each iteration adds one body clone with
        # one int constant (the iv) plus the literal 0 in `s = 0` and
        # 0 in the implicit return — extract just the iteration values.
        items = _main_block_items(prog)
        outer = next(bi for bi in items
                     if isinstance(bi, c99_ast.S)
                     and isinstance(bi.statement, c99_ast.Compound))
        iv_vals = []
        for inner_s in outer.statement.block.block_item:
            iv_vals.extend(_collect_constants(inner_s.statement))
        self.assertEqual(iv_vals, [0, 1, 2, 3])

    def test_lt_eq_includes_endpoint(self) -> None:
        prog = _unrolled("""
        #pragma c6502 loop unroll(enable)
        for (int i = 0; i <= 3; i++) i + 1;
        """)
        self.assertEqual(_outer_iter_count(prog), 4)

    def test_compound_assign_step(self) -> None:
        prog = _unrolled("""
        int s = 0;
        #pragma c6502 loop unroll(enable)
        for (int i = 0; i < 10; i += 2) s += i;
        """)
        self.assertEqual(_outer_iter_count(prog), 5)
        items = _main_block_items(prog)
        outer = next(bi for bi in items
                     if isinstance(bi, c99_ast.S)
                     and isinstance(bi.statement, c99_ast.Compound))
        iv_vals = []
        for inner_s in outer.statement.block.block_item:
            iv_vals.extend(_collect_constants(inner_s.statement))
        self.assertEqual(iv_vals, [0, 2, 4, 6, 8])

    def test_prefix_increment(self) -> None:
        prog = _unrolled("""
        #pragma c6502 loop unroll(enable)
        for (int i = 0; i < 3; ++i) i;
        """)
        self.assertEqual(_outer_iter_count(prog), 3)


class TestUnrollDescending(unittest.TestCase):
    def test_gt_decrement(self) -> None:
        prog = _unrolled("""
        int s = 0;
        #pragma c6502 loop unroll(enable)
        for (int i = 5; i > 0; i--) s += i;
        """)
        self.assertEqual(_outer_iter_count(prog), 5)
        items = _main_block_items(prog)
        outer = next(bi for bi in items
                     if isinstance(bi, c99_ast.S)
                     and isinstance(bi.statement, c99_ast.Compound))
        iv_vals = []
        for inner_s in outer.statement.block.block_item:
            iv_vals.extend(_collect_constants(inner_s.statement))
        self.assertEqual(iv_vals, [5, 4, 3, 2, 1])

    def test_ge_decrement_to_zero(self) -> None:
        prog = _unrolled("""
        #pragma c6502 loop unroll(enable)
        for (int i = 3; i >= 0; --i) i;
        """)
        self.assertEqual(_outer_iter_count(prog), 4)

    def test_minus_eq_step(self) -> None:
        prog = _unrolled("""
        #pragma c6502 loop unroll(enable)
        for (int i = 10; i > 0; i -= 2) i;
        """)
        self.assertEqual(_outer_iter_count(prog), 5)


class TestUnrollIvTypes(unittest.TestCase):
    def test_long_iv(self) -> None:
        prog = _unrolled("""
        #pragma c6502 loop unroll(enable)
        for (long i = 0; i < 3; i++) i;
        """)
        self.assertEqual(_outer_iter_count(prog), 3)
        # The substituted constant must be ConstLong, not ConstInt.
        items = _main_block_items(prog)
        outer = next(bi for bi in items
                     if isinstance(bi, c99_ast.S)
                     and isinstance(bi.statement, c99_ast.Compound))
        first_inner = outer.statement.block.block_item[0].statement
        consts = []
        for n in [first_inner]:
            if hasattr(n, "__dataclass_fields__"):
                for f in n.__dataclass_fields__:
                    val = getattr(n, f)
                    if isinstance(val, c99_ast.Constant):
                        consts.append(val)
        # Expression(exp=Constant(...)) — drill in.

        def find_constants(node, found):
            if isinstance(node, c99_ast.Constant):
                found.append(node)
            if hasattr(node, "__dataclass_fields__"):
                for f in node.__dataclass_fields__:
                    find_constants(getattr(node, f), found)
            elif isinstance(node, list):
                for item in node:
                    find_constants(item, found)

        all_consts: list[c99_ast.Constant] = []
        find_constants(first_inner, all_consts)
        self.assertTrue(any(isinstance(c.const, c99_ast.ConstLong)
                            for c in all_consts))

    def test_unsigned_int_iv(self) -> None:
        prog = _unrolled("""
        #pragma c6502 loop unroll(enable)
        for (unsigned int i = 0; i < 2; i++) i;
        """)
        self.assertEqual(_outer_iter_count(prog), 2)

    def test_char_iv(self) -> None:
        prog = _unrolled("""
        #pragma c6502 loop unroll(enable)
        for (char i = 0; i < 3; i++) i;
        """)
        self.assertEqual(_outer_iter_count(prog), 3)

    def test_signed_char_iv(self) -> None:
        prog = _unrolled("""
        #pragma c6502 loop unroll(enable)
        for (signed char i = 0; i < 3; i++) i;
        """)
        self.assertEqual(_outer_iter_count(prog), 3)

    def test_unsigned_char_iv(self) -> None:
        prog = _unrolled("""
        #pragma c6502 loop unroll(enable)
        for (unsigned char i = 0; i < 3; i++) i;
        """)
        self.assertEqual(_outer_iter_count(prog), 3)


class TestUnrollEdgeCases(unittest.TestCase):
    def test_zero_iterations(self) -> None:
        prog = _unrolled("""
        #pragma c6502 loop unroll(enable)
        for (int i = 5; i < 0; i++) i;
        """)
        self.assertEqual(_outer_iter_count(prog), 0)

    def test_unannotated_loop_left_alone(self) -> None:
        prog = _unrolled("""
        for (int i = 0; i < 4; i++) i;
        """)
        items = _main_block_items(prog)
        # Should still contain a ForStmt, not a Compound of clones.
        for_stmts = [bi for bi in items
                     if isinstance(bi, c99_ast.S)
                     and isinstance(bi.statement, c99_ast.ForStmt)]
        self.assertEqual(len(for_stmts), 1)

    def test_mixed_annotated_unannotated(self) -> None:
        prog = _unrolled("""
        #pragma c6502 loop unroll(enable)
        for (int i = 0; i < 2; i++) i;
        for (int j = 0; j < 2; j++) j;
        """)
        items = _main_block_items(prog)
        # First S is the unrolled compound; second is a ForStmt.
        s_stmts = [bi.statement for bi in items if isinstance(bi, c99_ast.S)]
        self.assertEqual(len(s_stmts), 3)  # compound, for, return
        self.assertIsInstance(s_stmts[0], c99_ast.Compound)
        self.assertIsInstance(s_stmts[1], c99_ast.ForStmt)

    def test_nested_annotated_loops(self) -> None:
        prog = _unrolled("""
        int s = 0;
        #pragma c6502 loop unroll(enable)
        for (int i = 0; i < 4; i++) {
            #pragma c6502 loop unroll(enable)
            for (int j = 0; j < 3; j++) {
                s += 1;
            }
        }
        """)
        # Outer wrapper has 4 S items (one per outer iter); each
        # outer iter's body contains an inner Compound with 3 items.
        items = _main_block_items(prog)
        outer = next(bi for bi in items
                     if isinstance(bi, c99_ast.S)
                     and isinstance(bi.statement, c99_ast.Compound))
        outer_iters = outer.statement.block.block_item
        self.assertEqual(len(outer_iters), 4)
        # Drill into one outer iter and find its inner compound.
        first_iter_body = outer_iters[0].statement.block.block_item
        self.assertEqual(len(first_iter_body), 1)  # the inner compound
        inner = first_iter_body[0].statement
        self.assertIsInstance(inner, c99_ast.Compound)
        self.assertEqual(len(inner.block.block_item), 3)

    def test_body_locals_get_per_iteration_scope(self) -> None:
        # Each iteration's `t` resolves to its own renamed `@N.t`
        # because it sits in its own Compound scope.
        from passes.identifier_resolution import resolve_program
        prog = unroll_program(parse(preprocess("""
            int main(void) {
                int s = 0;
                #pragma c6502 loop unroll(enable)
                for (int i = 0; i < 3; i++) {
                    int t = i + 1;
                    s += t;
                }
                return s;
            }
        """)))
        prog = resolve_program(prog)
        # Collect every VarDecl name; the three `t` declarations
        # must each get a distinct mangled name.
        names: list[str] = []

        def walk(n):
            if isinstance(n, c99_ast.VarDecl):
                names.append(n.var_decl.name)
            if hasattr(n, "__dataclass_fields__"):
                for f in n.__dataclass_fields__:
                    walk(getattr(n, f))
            elif isinstance(n, list):
                for item in n:
                    walk(item)

        walk(prog)
        t_names = [n for n in names if n.endswith(".t")]
        self.assertEqual(len(t_names), 3)
        self.assertEqual(len(set(t_names)), 3)  # all distinct


class TestUnrollRecognizerRejections(unittest.TestCase):
    def _expect_unroll_error(self, body: str, msg_substr: str) -> None:
        with self.assertRaises(UnrollError) as cm:
            _unrolled(body)
        self.assertIn(msg_substr, str(cm.exception))

    def test_break_in_body_rejected(self) -> None:
        self._expect_unroll_error(
            """
            #pragma c6502 loop unroll(enable)
            for (int i = 0; i < 4; i++) { if (i == 2) break; }
            """,
            "break",
        )

    def test_continue_in_body_rejected(self) -> None:
        self._expect_unroll_error(
            """
            #pragma c6502 loop unroll(enable)
            for (int i = 0; i < 4; i++) { if (i == 2) continue; }
            """,
            "continue",
        )

    def test_goto_in_body_rejected(self) -> None:
        self._expect_unroll_error(
            """
            #pragma c6502 loop unroll(enable)
            for (int i = 0; i < 4; i++) { goto out; }
        out: ;
            """,
            "goto",
        )

    def test_labeled_stmt_in_body_rejected(self) -> None:
        self._expect_unroll_error(
            """
            #pragma c6502 loop unroll(enable)
            for (int i = 0; i < 4; i++) { lbl: i; }
            """,
            "labeled",
        )

    def test_iv_assignment_rejected(self) -> None:
        self._expect_unroll_error(
            """
            #pragma c6502 loop unroll(enable)
            for (int i = 0; i < 4; i++) { i = 5; }
            """,
            "reassigned",
        )

    def test_iv_compound_assign_rejected(self) -> None:
        self._expect_unroll_error(
            """
            #pragma c6502 loop unroll(enable)
            for (int i = 0; i < 4; i++) { i += 5; }
            """,
            "modified",
        )

    def test_iv_inner_increment_rejected(self) -> None:
        self._expect_unroll_error(
            """
            #pragma c6502 loop unroll(enable)
            for (int i = 0; i < 4; i++) { i++; }
            """,
            "modified",
        )

    def test_iv_address_taken_rejected(self) -> None:
        self._expect_unroll_error(
            """
            int *p;
            #pragma c6502 loop unroll(enable)
            for (int i = 0; i < 4; i++) { p = &i; }
            """,
            "address",
        )

    def test_iv_inner_shadow_rejected(self) -> None:
        self._expect_unroll_error(
            """
            #pragma c6502 loop unroll(enable)
            for (int i = 0; i < 4; i++) { int i = 99; }
            """,
            "shadowed",
        )

    def test_inner_for_reusing_iv_name_rejected(self) -> None:
        self._expect_unroll_error(
            """
            #pragma c6502 loop unroll(enable)
            for (int i = 0; i < 4; i++) {
                for (int i = 0; i < 2; i++) i;
            }
            """,
            "shadowed",
        )

    def test_non_constant_init_rejected(self) -> None:
        self._expect_unroll_error(
            """
            int x = 0;
            #pragma c6502 loop unroll(enable)
            for (int i = x; i < 4; i++) i;
            """,
            "integer constant",
        )

    def test_non_constant_bound_rejected(self) -> None:
        self._expect_unroll_error(
            """
            int x = 4;
            #pragma c6502 loop unroll(enable)
            for (int i = 0; i < x; i++) i;
            """,
            "integer constant",
        )

    def test_non_constant_step_rejected(self) -> None:
        self._expect_unroll_error(
            """
            int x = 1;
            #pragma c6502 loop unroll(enable)
            for (int i = 0; i < 4; i += x) i;
            """,
            "post-clause step",
        )

    def test_zero_step_rejected(self) -> None:
        self._expect_unroll_error(
            """
            #pragma c6502 loop unroll(enable)
            for (int i = 0; i < 4; i += 0) i;
            """,
            "positive integer",
        )

    def test_equality_op_rejected(self) -> None:
        self._expect_unroll_error(
            """
            #pragma c6502 loop unroll(enable)
            for (int i = 0; i == 0; i++) i;
            """,
            "comparison op",
        )

    def test_not_equal_op_rejected(self) -> None:
        self._expect_unroll_error(
            """
            #pragma c6502 loop unroll(enable)
            for (int i = 0; i != 4; i++) i;
            """,
            "comparison op",
        )

    def test_condition_not_against_iv_rejected(self) -> None:
        self._expect_unroll_error(
            """
            int x;
            #pragma c6502 loop unroll(enable)
            for (int i = 0; x < 4; i++) i;
            """,
            "induction variable",
        )

    def test_swapped_condition_form_rejected(self) -> None:
        # We accept only `iv <op> const`, not `const <op> iv`.
        self._expect_unroll_error(
            """
            #pragma c6502 loop unroll(enable)
            for (int i = 0; 4 > i; i++) i;
            """,
            "left operand",
        )

    def test_iteration_cap_exceeded_rejected(self) -> None:
        self._expect_unroll_error(
            f"""
            #pragma c6502 loop unroll(enable)
            for (int i = 0; i < {MAX_ITERATIONS + 1}; i++) i;
            """,
            "cap",
        )

    def test_init_must_declare(self) -> None:
        self._expect_unroll_error(
            """
            int i;
            #pragma c6502 loop unroll(enable)
            for (i = 0; i < 4; i++) i;
            """,
            "must declare",
        )

    def test_missing_condition_rejected(self) -> None:
        self._expect_unroll_error(
            """
            #pragma c6502 loop unroll(enable)
            for (int i = 0; ; i++) { if (i > 4) break; }
            """,
            "condition is required",
        )

    def test_missing_post_rejected(self) -> None:
        self._expect_unroll_error(
            """
            #pragma c6502 loop unroll(enable)
            for (int i = 0; i < 4; ) i;
            """,
            "post-clause is required",
        )


class TestUnrollConstArraySubscriptBound(unittest.TestCase):
    """The unroll recognizer accepts a const-array subscript as
    an integer-bound, where every index is itself an integer-bound
    (i.e., either a literal Constant or a recursively-foldable
    Subscript)."""

    def test_1d_const_array_bound(self) -> None:
        # The bound is `BOUNDS[2]` — should fold to the third
        # element (5) and produce 5 cloned bodies.
        src = """
        static const int BOUNDS[3] = {3, 4, 5};
        int main(void) {
            #pragma c6502 loop unroll(enable)
            for (int i = 0; i < BOUNDS[2]; i++) i;
            return 0;
        }
        """
        prog = unroll_program(parse(preprocess(src)))
        # Find main and inspect the unrolled compound.
        main = next(d for d in prog.declaration
                    if isinstance(d, c99_ast.FunctionDecl))
        outer = next(bi for bi in main.function_decl.body.block_item
                     if isinstance(bi, c99_ast.S)
                     and isinstance(bi.statement, c99_ast.Compound))
        self.assertEqual(len(outer.statement.block.block_item), 5)

    def test_2d_const_array_bound(self) -> None:
        src = """
        static const int M[2][3] = { {1, 2, 3}, {4, 5, 6} };
        int main(void) {
            #pragma c6502 loop unroll(enable)
            for (int i = 0; i < M[1][2]; i++) i;
            return 0;
        }
        """
        prog = unroll_program(parse(preprocess(src)))
        main = next(d for d in prog.declaration
                    if isinstance(d, c99_ast.FunctionDecl))
        outer = next(bi for bi in main.function_decl.body.block_item
                     if isinstance(bi, c99_ast.S)
                     and isinstance(bi.statement, c99_ast.Compound))
        # M[1][2] = 6, so 6 iterations.
        self.assertEqual(len(outer.statement.block.block_item), 6)

    def test_const_array_zero_pads_missing_inner(self) -> None:
        # Inner row {7} pads to size 3 with two zeros. The bound
        # M[1][2] is the second padded zero — 0 iterations.
        src = """
        static const int M[2][3] = { {1, 2, 3}, {7} };
        int main(void) {
            #pragma c6502 loop unroll(enable)
            for (int i = 0; i < M[1][2]; i++) i;
            return 0;
        }
        """
        prog = unroll_program(parse(preprocess(src)))
        main = next(d for d in prog.declaration
                    if isinstance(d, c99_ast.FunctionDecl))
        outer = next(bi for bi in main.function_decl.body.block_item
                     if isinstance(bi, c99_ast.S)
                     and isinstance(bi.statement, c99_ast.Compound))
        self.assertEqual(len(outer.statement.block.block_item), 0)

    def test_outer_iv_substituted_into_inner_bound(self) -> None:
        # The motivating case: middle loop's iv `b` is substituted
        # into the inner loop's bound `COUNT[b]` per outer iter.
        src = """
        static const int COUNT[3] = {2, 4, 1};
        int main(void) {
            int s = 0;
            #pragma c6502 loop unroll(enable)
            for (int b = 0; b < 3; b++) {
                #pragma c6502 loop unroll(enable)
                for (int i = 0; i < COUNT[b]; i++) s++;
            }
            return s;
        }
        """
        prog = unroll_program(parse(preprocess(src)))
        # Each outer iter should produce a Compound with N inner
        # clones where N = COUNT[b]. Total inner clones = 2+4+1 = 7.
        # The resulting tree is Compound([Compound, Compound, Compound])
        # at the outer level; each child is a Compound containing the
        # substituted body, which is itself a Compound containing the
        # inner unroll's Compound of clones.
        main = next(d for d in prog.declaration
                    if isinstance(d, c99_ast.FunctionDecl))
        # Count CompoundAssignment / Postfix nodes — one per inner iter.
        count_postfix = [0]

        def walk(n):
            if isinstance(n, c99_ast.Postfix):
                count_postfix[0] += 1
            if hasattr(n, "__dataclass_fields__"):
                for f in n.__dataclass_fields__:
                    walk(getattr(n, f))
            elif isinstance(n, list):
                for item in n:
                    walk(item)

        walk(main.function_decl.body)
        # 7 cloned `s++;` postfixes. (The outer `b++` and inner
        # `i++` are gone — the for-loops were dissolved.)
        self.assertEqual(count_postfix[0], 7)

    def test_non_static_array_not_folded(self) -> None:
        # Bound depends on a non-static (block-scope) array — not
        # in the const map. Must reject.
        src = """
        int main(void) {
            const int BOUNDS[3] = {3, 4, 5};
            #pragma c6502 loop unroll(enable)
            for (int i = 0; i < BOUNDS[2]; i++) i;
            return 0;
        }
        """
        with self.assertRaises(UnrollError):
            unroll_program(parse(preprocess(src)))

    def test_non_const_array_not_folded(self) -> None:
        # File-scope static but no const qualifier — not in the
        # map. Must reject.
        src = """
        static int BOUNDS[3] = {3, 4, 5};
        int main(void) {
            #pragma c6502 loop unroll(enable)
            for (int i = 0; i < BOUNDS[2]; i++) i;
            return 0;
        }
        """
        with self.assertRaises(UnrollError):
            unroll_program(parse(preprocess(src)))

    def test_subscript_with_non_constant_index_rejected(self) -> None:
        # The array is foldable but the index isn't.
        src = """
        static const int BOUNDS[3] = {3, 4, 5};
        int main(void) {
            int j = 1;
            #pragma c6502 loop unroll(enable)
            for (int i = 0; i < BOUNDS[j]; i++) i;
            return 0;
        }
        """
        with self.assertRaises(UnrollError):
            unroll_program(parse(preprocess(src)))


class TestUnrollCli(unittest.TestCase):
    """`--unroll` CLI flag — invocation, gating on `--optimize`,
    and an end-to-end pipeline check that an annotated loop
    actually compiles down."""

    def _run(self, argv: list[str], stdin: str = "") -> tuple[int, str, str]:
        import io
        from unittest.mock import patch

        from compile import main
        with patch("sys.stdin", io.StringIO(stdin)), \
             patch("sys.stdout", new_callable=io.StringIO) as out, \
             patch("sys.stderr", new_callable=io.StringIO) as err:
            rc = main(argv)
        return rc, out.getvalue(), err.getvalue()

    def test_unroll_without_optimize_rejected(self) -> None:
        rc, _, err = self._run(
            ["compile.py", "-", "--tac", "--unroll"],
            stdin="int main(void) { return 0; }",
        )
        self.assertEqual(rc, 2)
        self.assertIn("--unroll requires --optimize", err)

    def test_unroll_with_optimize_compiles(self) -> None:
        src = """
        int main(void) {
            int s = 0;
            #pragma c6502 loop unroll(enable)
            for (int i = 0; i < 4; i++) s += i;
            return s;
        }
        """
        rc, out, _ = self._run(
            ["compile.py", "-", "--tac", "--optimize", "--unroll"],
            stdin=src,
        )
        self.assertEqual(rc, 0)
        # The for-loop is gone (no `_continue` / `_break` labels in
        # the TAC) — proof that the unrolling fired before the
        # loop_labeling pass would have minted those.
        self.assertNotIn("_continue", out)
        self.assertNotIn("_break", out)

    def test_unroll_without_flag_leaves_loop_intact(self) -> None:
        # Pragma is parsed but ignored when --unroll isn't passed;
        # the loop lowers normally with continue / break labels.
        src = """
        int main(void) {
            int s = 0;
            #pragma c6502 loop unroll(enable)
            for (int i = 0; i < 4; i++) s += i;
            return s;
        }
        """
        rc, out, _ = self._run(
            ["compile.py", "-", "--tac", "--optimize"], stdin=src,
        )
        self.assertEqual(rc, 0)
        self.assertIn("_continue", out)
        self.assertIn("_break", out)


if __name__ == "__main__":
    unittest.main()
