"""End-to-end harness for the chapter_18 corpus.

chapter_18 covers `struct` and `union`. c6502 implements neither
yet, so the entire valid/ bucket is pinned as
`_EXPECTED_FAILURES_CODEGEN` — all 108 valid programs use struct
syntax (declarations, compound initializers, member access via
`.` / `->`, struct return types, pointer-to-struct, ...) that
the parser doesn't accept. The pins flip individually as struct
support lands.

The invalid_* buckets work the way they do for the other chapters:
each file must be rejected somewhere in the pipeline. Most of these
contain `struct` keywords which our parser already rejects (with an
UnexpectedInput error), so they pass the "rejected somewhere"
check trivially even though the failure is "the struct keyword
itself isn't accepted" rather than the test's intended cause.

invalid_lex contains two pp-number tests (`.1l`, `.0foo`) — c6502's
lexer doesn't have the C preprocessing-number concept, so these
DON'T fail at lex time the way they do for upstream's compiler.
They DO fail at parse (because of struct), but that's at the wrong
stage for the invalid_lex bucket. They're pinned via
`_INVALID_LEX_NOT_REJECTED_TODAY`.
"""

from __future__ import annotations

import shutil
import unittest
from pathlib import Path

from compile import _run_stage
from lark.exceptions import UnexpectedInput
from lexer import LexError, tokenize
from parser import ParserError, parse
from preprocessor import preprocess


_TESTS_DIR = Path(__file__).parent
_C18 = _TESTS_DIR / "chapter_18"

_PARSE_FAILURES = (LexError, ParserError, UnexpectedInput)


# Permanently incompatible: features c6502 fundamentally can't
# compile. The chapter 18 corpus is all about struct / union, and
# c6502 has neither yet, so this set is empty here — files that
# don't compile because of struct are pinned in
# `_EXPECTED_FAILURES_CODEGEN` instead (so they auto-flip when
# struct support lands).
_INCOMPATIBLE_VALID: frozenset[str] = frozenset()


# A handful of `valid/` files don't actually need struct support to
# compile — they're testing edge cases (e.g. lexer disambiguation
# of `-->` between postfix `--` and `>`) that happen to land in
# this chapter's directory. List them here so the harness checks
# them as ordinary "must compile" tests; every OTHER valid file
# is treated as an expected failure.
_VALID_PASSES_TODAY: frozenset[str] = frozenset({
    # `ptr-->arr` is `ptr-- > arr` — only exercises lexer max-munch
    # for `--` vs. `-->`, doesn't actually declare a struct.
    "extra_credit/other_features/decr_arrow_lexing.c",
    # The chapter-18 valid programs that compile end-to-end now
    # that struct support is in. These exercise: file-scope struct
    # decls, member access (`.` / `->`), compound initializers,
    # struct address-of and pointer-to-struct, basic struct copies,
    # and unions (member access, copy, address-of). Programs that
    # need struct-by-value parameter passing or struct-by-value
    # return values are NOT in this set yet — those need ABI work
    # (HARGS / soft-stack adjustments).
    "extra_credit/other_features/label_tag_member_namespace.c",
    "extra_credit/semantic_analysis/cast_union_to_void.c",
    "extra_credit/semantic_analysis/redeclare_union.c",
    "extra_credit/semantic_analysis/struct_shadows_union.c",
    "extra_credit/semantic_analysis/union_members_same_type.c",
    "extra_credit/size_and_offset/compare_union_pointers.c",
    "extra_credit/size_and_offset/union_sizes.c",
    "extra_credit/semantic_analysis/union_self_pointer.c",
    "extra_credit/member_access/nested_union_access.c",
    "extra_credit/union_copy/assign_to_union.c",
    "extra_credit/union_copy/copy_non_scalar_members.c",
    "extra_credit/union_copy/unions_in_conditionals.c",
    "no_structure_parameters/scalar_member_access/linked_list.c",
    "no_structure_parameters/parse_and_lex/postfix_precedence.c",
    "no_structure_parameters/parse_and_lex/space_around_struct_member.c",
    "no_structure_parameters/parse_and_lex/struct_member_looks_like_const.c",
    "no_structure_parameters/parse_and_lex/trailing_comma.c",
    "no_structure_parameters/semantic_analysis/cast_struct_to_void.c",
    "no_structure_parameters/size_and_offset_calculations/member_comparisons.c",
    "no_structure_parameters/size_and_offset_calculations/member_offsets.c",
    "no_structure_parameters/size_and_offset_calculations/sizeof_exps.c",
    "no_structure_parameters/size_and_offset_calculations/sizeof_type.c",
    "no_structure_parameters/smoke_tests/simple.c",
    "no_structure_parameters/smoke_tests/static_vs_auto.c",
    "no_structure_parameters/struct_copy/copy_struct.c",
    "no_structure_parameters/struct_copy/copy_struct_through_pointer.c",
    "no_structure_parameters/struct_copy/copy_struct_with_arrow_operator.c",
    "no_structure_parameters/struct_copy/copy_struct_with_dot_operator.c",
    "no_structure_parameters/struct_copy/stack_clobber.c",
    "parameters/pass_args_on_page_boundary.c",
    "parameters/simple.c",
    "parameters/stack_clobber.c",
    "params_and_returns/stack_clobber.c",
    "params_and_returns/ignore_retval.c",
    "params_and_returns/return_big_struct_on_page_boundary.c",
    "params_and_returns/return_pointer_in_rax.c",
    "params_and_returns/return_space_overlap.c",
    "params_and_returns/return_struct_on_page_boundary.c",
    "params_and_returns/simple.c",
    "params_and_returns/temporary_lifetime.c",
})


# Per-bucket pinning for invalid tests c6502 doesn't reject today.
# c6502's lexer has no preprocessing-number concept, so `.1l` (a
# DOT followed by a valid LONG_INTEGER) lexes cleanly even though
# the standard would reject the whole sequence as one ill-formed
# pp-number. The companion case `.0foo` (DOT followed by `0foo`)
# is caught — c6502's INVALID_NUMBER regex flags numeric / digit
# / non-letter abutting an identifier — so only one of the pair
# pins here. The file DOES fail at parse time because of the
# surrounding struct keyword, but that's at the wrong stage for
# this bucket.
_INVALID_LEX_NOT_REJECTED_TODAY: frozenset[str] = frozenset({
    "dot_bad_token.c",
})
_INVALID_PARSE_NOT_REJECTED_TODAY: frozenset[str] = frozenset()
# Type-check edge cases c6502 doesn't reject yet: incomplete-type
# operations (assignment, dereference, sizeof at the use site, cast
# through incomplete pointer), struct-as-controlling-expression in
# `if`/`while`, and tag-shadowing scenarios that would need full
# scope-aware tag-name disambiguation. The valid programs work fine
# without these checks — these are diagnostic gaps, not codegen
# blockers.
_INVALID_TYPES_NOT_REJECTED_TODAY: frozenset[str] = frozenset({
    "extra_credit/scalar_required/union_as_controlling_expression.c",
    "extra_credit/union_struct_conflicts/conflicting_tag_decl_and_use.c",
    "extra_credit/union_struct_conflicts/conflicting_tag_decl_and_use_self_reference.c",
    "extra_credit/union_tag_resolution/distinct_union_types.c",
    "invalid_incomplete_structs/assign_to_incomplete_var.c",
    "invalid_incomplete_structs/cast_incomplete_struct.c",
    "invalid_incomplete_structs/deref_incomplete_struct_pointer.c",
    "invalid_incomplete_structs/incomplete_return_type_funcall.c",
    "invalid_incomplete_structs/incomplete_struct_full_expr.c",
    "scalar_required/struct_controlling_expression.c",
    "tag_resolution/conflicting_fun_ret_types.c",
    "tag_resolution/distinct_struct_types.c",
    "tag_resolution/incomplete_shadows_complete.c",
    "tag_resolution/incomplete_shadows_complete_cast.c",
    "tag_resolution/shadow_struct.c",
})
# `deref_undeclared.c` dereferences a pointer-to-incomplete-struct
# in an expression statement (`*ptr;`). The standard rejects this
# because you can't form an lvalue of incomplete type; c6502
# accepts it because we don't track that constraint at the
# Dereference site (the result type is just an incomplete
# struct value that's then discarded).
_INVALID_STRUCT_TAGS_NOT_REJECTED_TODAY: frozenset[str] = frozenset({
    "deref_undeclared.c",
})


# Multi-TU `libraries/` subdirs aren't applicable.
def _is_libraries(rel: str) -> bool:
    return rel.startswith("libraries/") or "/libraries/" in rel


@unittest.skipUnless(shutil.which("pcpp"), "pcpp not available on PATH")
class TestChapter18Valid(unittest.TestCase):
    def test_codegen(self):
        # Every valid file is expected to fail today (no struct
        # support). The harness iterates and asserts each fails;
        # when struct support lands, individual files start passing
        # and need to be removed from `_EXPECTED_FAILURES_CODEGEN`.
        files = sorted((_C18 / "valid").rglob("*.c"))
        self.assertGreater(len(files), 0, "no chapter_18 valid files found")
        for path in files:
            rel = str(path.relative_to(_C18 / "valid"))
            if _is_libraries(rel):
                continue
            if rel in _INCOMPATIBLE_VALID:
                continue
            with self.subTest(file=rel):
                source = preprocess(
                    path.read_text(), ["-I", str(path.parent)],
                )
                if rel in _VALID_PASSES_TODAY:
                    _run_stage("codegen", source)
                else:
                    with self.assertRaises(
                        Exception,
                        msg=(
                            f"{rel} unexpectedly compiled — add it "
                            f"to _VALID_PASSES_TODAY"
                        ),
                    ):
                        _run_stage("codegen", source)


@unittest.skipUnless(shutil.which("pcpp"), "pcpp not available on PATH")
class TestChapter18InvalidLex(unittest.TestCase):
    def test_lex_fails(self):
        files = sorted((_C18 / "invalid_lex").rglob("*.c"))
        self.assertGreater(len(files), 0, "no invalid_lex files found")
        for path in files:
            rel = str(path.relative_to(_C18 / "invalid_lex"))
            with self.subTest(file=rel):
                source = preprocess(
                    path.read_text(), ["-I", str(path.parent)],
                )
                if rel in _INVALID_LEX_NOT_REJECTED_TODAY:
                    list(tokenize(source))
                else:
                    with self.assertRaises(LexError):
                        list(tokenize(source))


@unittest.skipUnless(shutil.which("pcpp"), "pcpp not available on PATH")
class TestChapter18InvalidParse(unittest.TestCase):
    def test_parse_fails(self):
        files = sorted((_C18 / "invalid_parse").rglob("*.c"))
        self.assertGreater(len(files), 0, "no invalid_parse files found")
        for path in files:
            rel = str(path.relative_to(_C18 / "invalid_parse"))
            with self.subTest(file=rel):
                source = preprocess(
                    path.read_text(), ["-I", str(path.parent)],
                )
                if rel in _INVALID_PARSE_NOT_REJECTED_TODAY:
                    parse(source)
                else:
                    with self.assertRaises(_PARSE_FAILURES):
                        parse(source)


@unittest.skipUnless(shutil.which("pcpp"), "pcpp not available on PATH")
class TestChapter18InvalidTypes(unittest.TestCase):
    def test_codegen_rejects(self):
        files = sorted((_C18 / "invalid_types").rglob("*.c"))
        self.assertGreater(len(files), 0, "no invalid_types files found")
        for path in files:
            rel = str(path.relative_to(_C18 / "invalid_types"))
            with self.subTest(file=rel):
                source = preprocess(
                    path.read_text(), ["-I", str(path.parent)],
                )
                if rel in _INVALID_TYPES_NOT_REJECTED_TODAY:
                    _run_stage("codegen", source)
                else:
                    with self.assertRaises(Exception):
                        _run_stage("codegen", source)


@unittest.skipUnless(shutil.which("pcpp"), "pcpp not available on PATH")
class TestChapter18InvalidStructTags(unittest.TestCase):
    def test_codegen_rejects(self):
        files = sorted((_C18 / "invalid_struct_tags").rglob("*.c"))
        self.assertGreater(len(files), 0, "no invalid_struct_tags files found")
        for path in files:
            rel = str(path.relative_to(_C18 / "invalid_struct_tags"))
            with self.subTest(file=rel):
                source = preprocess(
                    path.read_text(), ["-I", str(path.parent)],
                )
                if rel in _INVALID_STRUCT_TAGS_NOT_REJECTED_TODAY:
                    _run_stage("codegen", source)
                else:
                    with self.assertRaises(Exception):
                        _run_stage("codegen", source)
