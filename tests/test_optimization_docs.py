"""Checks docs/optimizations/README.md against the pipeline code.

The README lists every optimization twice, alphabetically and in
pipeline order. These tests fail when a document is added without an
index entry, when an index entry has no document, and when a pass is
added to or moved within the pipeline without the pipeline-order list
being updated to match.

A pass is matched to its document through the module that defines the
pass's entry point. A module's last name component is the document's
file stem, except for the modules in `_STEM_ALIASES`."""

import inspect
import re
import unittest
from pathlib import Path

import compile as compile_module
from passes.optimization import optimizer as tac_optimizer
from passes.optimization.framework import RuleSet
from passes.optimization_asm import optimizer as asm_optimizer

_DOCS = Path(__file__).resolve().parent.parent / "docs" / "optimizations"
_README = _DOCS / "README.md"

# Modules whose document has a different stem, because one document
# covers several modules or because two layers have a module of the
# same name.
_STEM_ALIASES = {
    "passes.abi_selection": "zp_abi",
    "passes.zp_slot_allocation": "zp_abi",
    "passes.zp_local_allocation": "zp_local_pools",
    "passes.function_local_sizing": "zp_local_pools",
    "passes.optimization_asm.copy_propagation": "asm_copy_propagation",
    "passes.optimization_asm.hwreg_eligibility": "regalloc",
    "passes.optimization_asm.apply_coloring": "regalloc",
}

_LINK = re.compile(r"\]\(([a-z0-9_]+)\.md\)")


def _doc_stem(module_name: str) -> str | None:
    """Returns the stem of the document for the pass defined in
    `module_name`, or None if no document covers that module."""
    stem = _STEM_ALIASES.get(module_name, module_name.rsplit(".", 1)[-1])
    return stem if (_DOCS / f"{stem}.md").exists() else None


def _dedupe(stems: list[str]) -> list[str]:
    """Collapses runs of the same stem, since one document can cover
    several passes that run back to back (the carry-return peepholes)."""
    out: list[str] = []
    for s in stems:
        if not out or out[-1] != s:
            out.append(s)
    return out


def _called_stems(func, module, pattern: str) -> list[str]:
    """Returns the document stems of the calls in `func`'s source whose
    names match `pattern`, in source order. A name is resolved in
    `module`'s globals, or through a `from passes... import name`
    inside the function body."""
    src = inspect.getsource(func)
    local_imports = {
        name: mod
        for mod, names in re.findall(
            r"from (passes[\w.]*) import \(?([\w\s,]+)\)?", src,
        )
        for name in re.findall(r"\w+", names)
    }
    stems = []
    for name in re.findall(rf"\b({pattern})\(", src):
        obj = getattr(module, name, None)
        mod = getattr(obj, "__module__", None) or local_imports.get(name)
        stem = _doc_stem(mod) if mod else None
        if stem is not None:
            stems.append(stem)
    return _dedupe(stems)


def _is_subsequence(needle: list[str], haystack: list[str]) -> bool:
    it = iter(haystack)
    return all(any(h == n for h in it) for n in needle)


class TestOptimizationDocs(unittest.TestCase):
    def setUp(self):
        text = _README.read_text(encoding="utf-8")
        alpha_start = text.index("## Alphabetical list")
        order_start = text.index("## Pipeline order")
        peep_start = text.index(
            "The peephole fixed point (`compile._peephole_fixedpoint`)",
        )
        unopt_start = text.index("### Without `--optimize`")
        self.alpha = _LINK.findall(text[alpha_start:order_start])
        self.order = _LINK.findall(text[order_start:peep_start])
        self.peephole = _LINK.findall(text[peep_start:unopt_start])
        self.docs = sorted(
            p.stem for p in _DOCS.glob("*.md") if p.name != "README.md"
        )

    def test_every_document_is_in_the_alphabetical_list_once(self):
        self.assertEqual(sorted(self.alpha), self.docs)

    def test_every_document_is_in_the_pipeline_order(self):
        self.assertEqual(
            sorted(set(self.order) | set(self.peephole)), self.docs,
        )

    def test_peephole_list_matches_the_fixed_point(self):
        code = _called_stems(
            compile_module._peephole_fixedpoint, compile_module,
            r"apply_\w+",
        )
        self.assertEqual(self.peephole, code)

    def test_tac_driver_order(self):
        driver = tac_optimizer._DRIVER
        stems = []
        for slot in ("pre_ssa", "pre_fixedpoint", "fixedpoint",
                     "post_fixedpoint", "post_destruction",
                     "post_destruction_fixedpoint"):
            for p in getattr(driver, slot):
                if isinstance(p, RuleSet):
                    mods = [r.build.__module__ for r in p.rules]
                else:
                    mods = [type(p).__module__]
                stems += [s for s in map(_doc_stem, mods) if s]
        stems = _dedupe(stems)
        self.assertTrue(
            _is_subsequence(stems, self.order),
            f"TAC driver order {stems} isn't in the README's order",
        )

    def test_asm_optimizer_order(self):
        top = inspect.getsource(asm_optimizer.optimize_program)
        before, _, after = top.partition("_optimize_function(")
        names = r"\w+"
        stems = (
            [_doc_stem(asm_optimizer.fold_const_statics.__module__)]
            + _called_stems(
                asm_optimizer._optimize_function, asm_optimizer, names,
            )
            + [_doc_stem(
                asm_optimizer.apply_dead_static_elimination.__module__,
            )]
        )
        self.assertIn("fold_const_statics(", before)
        self.assertIn("apply_dead_static_elimination(", after)
        self.assertTrue(
            _is_subsequence(stems, self.order),
            f"asm optimizer order {stems} isn't in the README's order",
        )

    def test_optimized_asm_order(self):
        code = _called_stems(
            compile_module.optimized_asm, compile_module,
            r"apply_\w+|synthesize_prologue|lower_data_load_address"
            r"|dispatch_const_pointer_arrays|select_abi"
            r"|allocate_function_locals",
        )
        self.assertTrue(
            _is_subsequence(code, self.order),
            f"optimized_asm order {code} isn't in the README's order",
        )


if __name__ == "__main__":
    unittest.main()
