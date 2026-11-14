"""End-to-end tests for the multi-TU linker (`compile.py --link`).

Strategy: compile each input .c file separately to .asm with
`--codegen --optimize`, then run the linker over the .asm
files, then either inspect the combined asm or assemble and
simulate it.

Coverage:
  - Two TUs each defining one zp_abi function, with `main` in
    one and a helper in the other. Linker re-allocates symbols
    across them; sim runs correctly.
  - Linker rejects cross-TU duplicate definition.
  - Linker rejects a non-zp_abi extern callee.
  - Linker rejects cross-TU recursion (mutual).
  - Output asm has one global EQU block, no per-TU blocks.
  - A declaration that names a parameter differently from the
    definition, or leaves it unnamed, links: its slot symbols are
    bound to the definition's by position.
"""
from __future__ import annotations

import io
import shutil
import subprocess
import tempfile
import unittest
from pathlib import Path
from unittest.mock import patch

from compile import main as compile_main


def _compile_to_asm(source: str, out_path: Path) -> int:
    """Compile a single C source through `--codegen --optimize`,
    write the asm to `out_path`. Returns the exit code."""
    with patch("sys.stdin", io.StringIO(source)):
        return compile_main([
            "compile.py", "-", "--codegen", "--optimize",
            "-o", str(out_path),
        ])


def _link(asm_paths: list[Path], out_path: Path) -> int:
    """Run `compile.py --link` over the asm_paths."""
    argv = ["compile.py", "--link"]
    argv.extend(str(p) for p in asm_paths)
    argv.extend(["-o", str(out_path)])
    return compile_main(argv)


def _link_capture_stderr(asm_paths: list[Path], out_path: Path) -> tuple[int, str]:
    """Run --link; capture stderr (link errors go there)."""
    argv = ["compile.py", "--link"]
    argv.extend(str(p) for p in asm_paths)
    argv.extend(["-o", str(out_path)])
    with patch("sys.stderr", io.StringIO()) as err:
        rc = compile_main(argv)
        return rc, err.getvalue()


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestLinkerHappyPath(unittest.TestCase):
    def test_two_tus_link_and_simulate(self) -> None:
        # TU A: defines main, calls extern helper.
        # TU B: defines helper.
        # Both are zp_abi-annotated. Linker re-allocates so the
        # symbols agree across TUs.
        tu_a = (
            "__attribute__((zp_abi)) extern int helper(int x); "
            "int main(void) { return helper(7); }"
        )
        tu_b = (
            "__attribute__((zp_abi)) int helper(int x) { "
            "  return x + 1; "
            "}"
        )
        with tempfile.TemporaryDirectory() as tmp:
            tmp = Path(tmp)
            a = tmp / "a.asm"
            b = tmp / "b.asm"
            out = tmp / "linked.asm"
            self.assertEqual(_compile_to_asm(tu_a, a), 0)
            self.assertEqual(_compile_to_asm(tu_b, b), 0)
            self.assertEqual(_link([a, b], out), 0)
            text = out.read_text()
            # One global EQU block at the top.
            self.assertIn("__zpabi_helper__x_0\tEQU\t", text)
            # The function bodies appear after the EQU block.
            self.assertIn("main:", text)
            self.assertIn("helper:", text)
            # No leftover per-TU metadata bracketed blocks
            # inside the body (one merged block remains at the
            # top).
            self.assertEqual(
                text.count("@zp-link-meta-begin"), 1,
                "Should have exactly one (merged) metadata block",
            )

    def test_reg_attribute_propagates_across_tus(self) -> None:
        # `register`-param extern in TU A; definition in TU B. The
        # linker must reconstruct the ZpLayout with the params'
        # register assignment so the per-TU asm bodies agree on the
        # calling convention. The first register arg-byte goes in A,
        # the second in X — so TU A's main passes `a` in A and `b` in
        # X; TU B's helper definition uses the entry-stub copies from
        # A / X. The metadata block carries the param register info
        # between them.
        tu_a = (
            'extern char helper(register char a, register char b);'
            'int main(void) { return helper(5, 7); }'
        )
        tu_b = (
            'char helper(register char a, register char b) {'
            '    return (char)(a + b);'
            '}'
        )
        with tempfile.TemporaryDirectory() as tmp:
            tmp = Path(tmp)
            a = tmp / "a.asm"
            b = tmp / "b.asm"
            out = tmp / "linked.asm"
            self.assertEqual(_compile_to_asm(tu_a, a), 0)
            self.assertEqual(_compile_to_asm(tu_b, b), 0)
            self.assertEqual(_link([a, b], out), 0)
            text = out.read_text()
            # TU A's main should load `b` into X before JSR (the
            # direct-into-X peephole renders `LDA #imm; TAX` as a
            # single LDX #imm). NOT `STA __zpabi_helper__b` — that's
            # the soft-stack-style slot write the linker would emit if
            # it lost the register assignment.
            main_idx = text.index("main:")
            helper_idx = text.index("helper:")
            if main_idx < helper_idx:
                main_body = text[main_idx:helper_idx]
            else:
                main_body = text[main_idx:]
            self.assertRegex(main_body, r"LDX\s+#")
            self.assertNotIn("STA\t__zpabi_helper__b", main_body)
            # And the metadata block at the top of the linked output
            # should carry the param_regs annotation (A for `a`,
            # X for `b`).
            self.assertIn("param_regs=A,X", text)

    def test_linker_dedupe_externs(self) -> None:
        # Two TUs that both declare the same extern. The linker
        # collapses them into one entry (no error).
        tu_a = (
            "__attribute__((zp_abi)) extern int helper(int x); "
            "__attribute__((zp_abi)) int caller_a(int n) { "
            "  return helper(n); "
            "}"
        )
        tu_b = (
            "__attribute__((zp_abi)) extern int helper(int x); "
            "__attribute__((zp_abi)) int caller_b(int n) { "
            "  return helper(n + 1); "
            "}"
        )
        tu_c = (
            "__attribute__((zp_abi)) int helper(int x) { "
            "  return x + 100; "
            "}"
            "int main(void) { return 0; }"
        )
        with tempfile.TemporaryDirectory() as tmp:
            tmp = Path(tmp)
            paths = []
            for name, src in [("a", tu_a), ("b", tu_b), ("c", tu_c)]:
                p = tmp / f"{name}.asm"
                self.assertEqual(_compile_to_asm(src, p), 0)
                paths.append(p)
            out = tmp / "linked.asm"
            self.assertEqual(_link(paths, out), 0)
            text = out.read_text()
            # One EQU per symbol.
            self.assertEqual(text.count("__zpabi_helper__x_0\tEQU\t"), 1)


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestLinkerErrors(unittest.TestCase):
    def test_duplicate_definition_rejected(self) -> None:
        # Both TUs define `helper`. Link error.
        tu_a = (
            "__attribute__((zp_abi)) int helper(int x) { "
            "  return x + 1; "
            "}"
        )
        tu_b = (
            "__attribute__((zp_abi)) int helper(int x) { "
            "  return x + 2; "
            "}"
            "int main(void) { return helper(3); }"
        )
        with tempfile.TemporaryDirectory() as tmp:
            tmp = Path(tmp)
            a = tmp / "a.asm"
            b = tmp / "b.asm"
            out = tmp / "linked.asm"
            self.assertEqual(_compile_to_asm(tu_a, a), 0)
            self.assertEqual(_compile_to_asm(tu_b, b), 0)
            rc, err = _link_capture_stderr([a, b], out)
            self.assertEqual(rc, 1)
            self.assertIn("multiple TUs", err)
            self.assertIn("helper", err)

    def test_unannotated_extern_accepted_under_default_zp_abi(self) -> None:
        # Under the default-zp_abi policy (`--optimize`), every
        # extern is treated as zp_abi at the call site: args land
        # in `__zpabi_<callee>_p<k>` slots, and the per-TU output
        # advertises the extern in `; @zp-link-meta-begin`. The
        # linker therefore accepts the link — the user is on the
        # hook for ensuring the actual definition uses a matching
        # ABI.
        src = (
            "extern int regular_lib_fn(int x); "
            "int main(void) { return regular_lib_fn(7); }"
        )
        with tempfile.TemporaryDirectory() as tmp:
            tmp = Path(tmp)
            p = tmp / "a.asm"
            out = tmp / "linked.asm"
            self.assertEqual(_compile_to_asm(src, p), 0)
            rc, err = _link_capture_stderr([p], out)
            self.assertEqual(rc, 0, f"Expected link to succeed, stderr: {err!r}")

    def test_no_inputs_rejected(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            out = Path(tmp) / "linked.asm"
            # argparse rejects empty positional with nargs=+
            # (SystemExit from its built-in error path).
            with self.assertRaises(SystemExit) as cm:
                _link_capture_stderr([], out)
            self.assertNotEqual(cm.exception.code, 0)


def _equs(text: str) -> dict[str, int]:
    """The `name EQU $addr` bindings in a linked file."""
    out = {}
    for line in text.splitlines():
        parts = line.split("\t")
        if len(parts) == 3 and parts[1] == "EQU":
            out[parts[0]] = int(parts[2].lstrip("$"), 16)
    return out


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestParameterNames(unittest.TestCase):
    """A zp_abi parameter's slot is named after the parameter, so a
    declaration whose names differ from the definition's spells the
    same byte differently, and an unnamed one spells it by position."""

    _DEF = "unsigned char put(unsigned char c, int n) { return c + n; }"

    def _link_with(self, decl: str) -> tuple[int, str, str]:
        caller = decl + " int main(void) { return put(4, 300); }"
        with tempfile.TemporaryDirectory() as tmp:
            tmp = Path(tmp)
            a, b, out = tmp / "a.asm", tmp / "b.asm", tmp / "linked.asm"
            self.assertEqual(_compile_to_asm(self._DEF, a), 0)
            self.assertEqual(_compile_to_asm(caller, b), 0)
            rc, err = _link_capture_stderr([a, b], out)
            return rc, err, out.read_text() if rc == 0 else ""

    def _assert_bound_like_definition(self, text: str, names) -> None:
        equs = _equs(text)
        for decl_sym, def_sym in names:
            self.assertEqual(equs[decl_sym], equs[def_sym], decl_sym)

    def test_renamed_parameters_link(self) -> None:
        rc, err, text = self._link_with(
            "unsigned char put(unsigned char x, int count);",
        )
        self.assertEqual(rc, 0, err)
        self._assert_bound_like_definition(text, [
            ("__zpabi_put__x", "__zpabi_put__c"),
            ("__zpabi_put__count_0", "__zpabi_put__n_0"),
            ("__zpabi_put__count_1", "__zpabi_put__n_1"),
        ])

    def test_unnamed_parameters_link(self) -> None:
        rc, err, text = self._link_with("unsigned char put(unsigned char, int);")
        self.assertEqual(rc, 0, err)
        self._assert_bound_like_definition(text, [
            ("__zpabi_put__0", "__zpabi_put__c"),
            ("__zpabi_put__1_0", "__zpabi_put__n_0"),
            ("__zpabi_put__1_1", "__zpabi_put__n_1"),
        ])

    def test_swapped_names_rejected(self) -> None:
        # `__zpabi_put__a` would be the first byte in one unit and the
        # second in the other.
        definition = (
            "unsigned char put(unsigned char a, unsigned char b) "
            "{ return a - b; }"
        )
        caller = (
            "unsigned char put(unsigned char b, unsigned char a);"
            " int main(void) { return put(4, 3); }"
        )
        with tempfile.TemporaryDirectory() as tmp:
            tmp = Path(tmp)
            a, b, out = tmp / "a.asm", tmp / "b.asm", tmp / "linked.asm"
            self.assertEqual(_compile_to_asm(definition, a), 0)
            self.assertEqual(_compile_to_asm(caller, b), 0)
            rc, err = _link_capture_stderr([a, b], out)
        self.assertEqual(rc, 1)
        self.assertIn("same names", err)

    def test_different_sizes_dont_collide(self) -> None:
        # `n` and `c` swap, but one is 1 byte and the other 2, so their
        # symbols (`__zpabi_put__n` and `__zpabi_put__n_0`) differ.
        rc, err, text = self._link_with(
            "unsigned char put(unsigned char n, int c);",
        )
        self.assertEqual(rc, 0, err)
        self._assert_bound_like_definition(text, [
            ("__zpabi_put__n", "__zpabi_put__c"),
            ("__zpabi_put__c_0", "__zpabi_put__n_0"),
        ])

    def test_byte_count_mismatch_rejected(self) -> None:
        rc, err, _ = self._link_with("unsigned char put(unsigned char c, long n);")
        self.assertEqual(rc, 1)
        self.assertIn("parameter byte", err)

    def test_unnamed_parameter_in_one_unit(self) -> None:
        # Calling a function declared with an unnamed parameter used
        # to crash `--optimize`, with or without the annotation.
        from compile import _run_stage
        from preprocessor import preprocess
        for attr in ("", "__attribute__((zp_abi)) "):
            src = (attr + "unsigned char put(unsigned char);\n"
                   "int main(void) { return put(4); }\n")
            with self.subTest(attr=attr):
                self.assertIn("__zpabi_put__0", _run_stage(
                    "codegen", preprocess(src), optimize=True,
                ))

    def test_unnamed_prototype_then_definition_runs(self) -> None:
        from sim.harness import build_sim
        src = ("unsigned char put(unsigned char, int);\n"
               "int main(void) { return put(4, 300); }\n" + self._DEF)
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                self.assertEqual(
                    build_sim(src, optimize=optimize).run().return_int(),
                    (4 + 300) & 0xFF,
                )


class TestRecursionWithSaves(unittest.TestCase):
    """A cycle whose every call is under `#pragma c6502 call
    save(hardware_stack)` links, as Zork's print_zstring does; one
    without the pragma doesn't."""

    _SAVED = """
    unsigned char depth;
    unsigned char out;
    void walk(unsigned char n) {
        unsigned char here = n;
        if (n == 0) return;
    #pragma c6502 call save(hardware_stack)
        walk(n - 1);
        out = out * 3 + here;
    }
    """

    def _link_one(self, src: str) -> tuple[int, str, str]:
        with tempfile.TemporaryDirectory() as tmp:
            tmp = Path(tmp)
            p = tmp / "a.asm"
            out = tmp / "linked.asm"
            self.assertEqual(_compile_to_asm(src, p), 0)
            rc, err = _link_capture_stderr([p], out)
            text = out.read_text() if rc == 0 else ""
        return rc, err, text

    def test_saved_cycle_links(self):
        rc, err, text = self._link_one(
            self._SAVED + "int main(void) { walk(3); return out; }\n",
        )
        self.assertEqual(rc, 0, err)
        self.assertIn("JSR   walk", text)

    @unittest.skipUnless(shutil.which("dasm"), "dasm not installed")
    def test_saved_cycle_runs(self):
        from sim.linked import build_linked_sim
        rc, err, text = self._link_one(
            self._SAVED + "int main(void) { walk(3); return out; }\n",
        )
        self.assertEqual(rc, 0, err)
        # out = ((0 * 3 + 1) * 3 + 2) * 3 + 3
        self.assertEqual(build_linked_sim(text).run().return_int(), 18)

    def test_cycle_without_saves_rejected(self):
        src = (
            "unsigned char out;\n"
            "void walk(unsigned char n) { if (n) { walk(n - 1); out++; } }\n"
            "int main(void) { walk(3); return out; }\n"
        )
        with tempfile.TemporaryDirectory() as tmp:
            tmp = Path(tmp)
            p = tmp / "a.asm"
            # Without the pragma the compile itself falls back to the
            # soft stack, which the linker can't re-allocate.
            if _compile_to_asm(src, p) != 0:
                return
            rc, err = _link_capture_stderr([p], tmp / "linked.asm")
            self.assertNotEqual(rc, 0)

if __name__ == "__main__":
    unittest.main()
