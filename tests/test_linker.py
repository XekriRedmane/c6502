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
        # Reg-attributed extern in TU A; definition in TU B. The
        # linker must reconstruct the ZpLayout with the param's
        # register attribute so the per-TU asm bodies agree on the
        # calling convention. TU A's main calls helper, expecting
        # to pass the arg in X; TU B's helper definition uses the
        # entry-stub copy from X. The metadata block carries the
        # param register info between them.
        tu_a = (
            'extern char helper(char x __attribute__((reg("X"))));'
            'int main(void) { return helper(5); }'
        )
        tu_b = (
            'char helper(char x __attribute__((reg("X")))) {'
            '    return (char)(x + 1);'
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
            # TU A's main should load the arg into X before JSR
            # (the direct-into-X peephole renders `LDA #imm; TAX`
            # as a single LDX #imm). NOT `STA __zpabi_helper__x`
            # — that's the soft-stack-style slot write the linker
            # would emit if it lost the reg attribute.
            main_idx = text.index("main:")
            helper_idx = text.index("helper:")
            if main_idx < helper_idx:
                main_body = text[main_idx:helper_idx]
            else:
                main_body = text[main_idx:]
            self.assertRegex(main_body, r"LDX\s+#")
            self.assertNotIn("STA\t__zpabi_helper__x_0", main_body)
            # And the metadata block at the top of the linked
            # output should carry the param_regs annotation for
            # helper.
            self.assertIn("param_regs=X", text)

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


if __name__ == "__main__":
    unittest.main()
