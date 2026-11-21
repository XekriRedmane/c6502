"""Tests for tools/capture_passes.py, the recorder of the asm passes'
calls that the C++ port is checked against."""
from __future__ import annotations

import os
import re
import subprocess
import sys
import tempfile
import unittest
from dataclasses import dataclass
from pathlib import Path
from unittest import mock

import asm_ast
import compile as compile_module
import ir_dump

sys.path.insert(0, os.path.join(os.path.dirname(os.path.dirname(
    os.path.abspath(__file__))), "tools"))
import capture_passes  # noqa: E402

from tests.test_ir_dump import cpp_cli  # noqa: E402


_ROOT = Path(__file__).resolve().parent.parent


class TestPassTable(unittest.TestCase):
    def test_sweep_names_match_compile(self):
        # The table lists the sweep of `_peephole_fixedpoint` under the
        # sweep's names, in its order, with the function each one calls.
        with open(_ROOT / "compile.py", encoding="utf-8") as f:
            source = f.read()
        body = source[source.index("    sweep = ("):source.index(
            "    for _ in range(_PEEPHOLE_FIXEDPOINT_CAP)")]
        entries = re.findall(
            r'\((?:F|P|_ALWAYS), "(\w+)",\s*lambda p, m: (\w+)\(', body,
        )
        self.assertEqual(entries, list(capture_passes.SWEEP_PASSES.items()))

    def test_every_function_is_in_compile(self):
        for name, attr in capture_passes.PASSES.items():
            with self.subTest(name=name):
                self.assertTrue(callable(getattr(compile_module, attr)))

    def test_every_stage_function_resolves(self):
        for name, target in capture_passes.STAGE_PASSES.items():
            with self.subTest(name=name):
                module, attr = capture_passes.resolve(target)
                self.assertTrue(callable(getattr(module, attr)))

    def test_every_stage_function_has_a_cpp_signature(self):
        # Each stage call's codecs are in cpp/src/passes/stage_registry.cpp,
        # in the table's order. The corpus check (`check-captures` on a
        # capture of the stage calls) tests the codecs themselves.
        registry = (
            _ROOT / "cpp" / "src" / "passes" / "stage_registry.cpp"
        ).read_text(encoding="utf-8")
        names = re.findall(r'\{\.name = "(\w+)"', registry)
        self.assertEqual(names, list(capture_passes.STAGE_PASSES))


@dataclass
class _Point:
    x: int
    tags: frozenset


class TestEncode(unittest.TestCase):
    def test_forms(self):
        enc = capture_passes.encode
        self.assertEqual(enc([1, "a", None, True]), [1, "a", None, True])
        self.assertEqual(enc((1, 2)), {"tuple": [1, 2]})
        self.assertEqual(enc({"b", "a"}), {"set": ["a", "b"]})
        self.assertEqual(enc({2: "x"}), {"dict": [[2, "x"]]})
        self.assertEqual(enc(range(0, 0x80)), {"range": [0, 128, 1]})
        # A float is the bits of its IEEE 754 double, so -0.0 and a
        # NaN's payload make the trip.
        self.assertEqual(enc(1.0), {"float": 0x3FF0000000000000})
        self.assertEqual(enc(-0.0), {"float": 0x8000000000000000})
        self.assertEqual(
            enc(asm_ast.Imm(value=3)), {"asdl": "asm_ast", "dump": "Imm(3)\n"},
        )
        self.assertEqual(
            enc(_Point(1, frozenset({3}))),
            {"object": f"{__name__}._Point",
             "fields": {"x": 1, "tags": {"set": [3]}}},
        )


class TestCapture(unittest.TestCase):
    """Records split_mem_to_mem's calls on one example and checks the
    files."""

    @classmethod
    def setUpClass(cls):
        cls.tmp = tempfile.TemporaryDirectory()
        out = Path(cls.tmp.name) / "captures"
        saved = compile_module.apply_split_mem_to_mem
        try:
            capture_passes.install(["split_mem_to_mem"], out)
            capture_passes._SOURCE = "examples/sfx_tone.c -S -O"
            src = _ROOT / "examples" / "sfx_tone.c"
            # The harness records the Python passes, which don't run
            # under C6502_CPP_BACKEND=1.
            with mock.patch.dict(os.environ):
                os.environ.pop("C6502_CPP_BACKEND", None)
                assert compile_module.main([
                    "compile.py", str(src), "-S", "-O",
                    "-o", os.path.join(cls.tmp.name, "out.s"),
                ]) == 0
        finally:
            compile_module.apply_split_mem_to_mem = saved
        cls.files = sorted((out / "split_mem_to_mem").glob("*.txt"))

    @classmethod
    def tearDownClass(cls):
        cls.tmp.cleanup()

    def test_files_hold_the_call(self):
        self.assertTrue(self.files)
        from passes.split_mem_to_mem import apply_split_mem_to_mem
        for path in self.files:
            text = path.read_bytes().decode("utf-8")
            head, _, rest = text.partition("\ninput\n")
            self.assertEqual(head.split("\n")[:3], [
                "c6502-capture 2", "pass split_mem_to_mem",
                "source examples/sfx_tone.c -S -O",
            ])
            self.assertEqual(head.split("\n")[3],
                             'args {"args": [], "kwargs": {}}')
            # sfx_tone.c has no zero-page globals, so the runtime's
            # block keeps its classic layout.
            self.assertEqual(
                head.split("\n")[4],
                'context {"layout":[0,2,4,32,36],"user_zp_globals":[],'
                '"user_zp_bytes":[]}',
            )
            input_text, _, output_text = rest.partition("\noutput\n")
            prog = ir_dump.load(input_text, asm_ast)
            self.assertEqual(
                ir_dump.dump(apply_split_mem_to_mem(prog)), output_text,
            )

    @unittest.skipUnless(cpp_cli(), "the C++ CLI isn't built (cpp/README.md)")
    def test_cpp_port_matches(self):
        result = subprocess.run(
            [cpp_cli(), "check-captures", str(self.files[0].parent)],
            capture_output=True, text=True,
        )
        self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
        self.assertIn(
            f"split_mem_to_mem: {len(self.files)} calls, "
            f"{len(self.files)} match, 0 differ", result.stdout,
        )


class TestStageCapture(unittest.TestCase):
    """Records the stage calls on one example and checks the records:
    a call whose first argument isn't a program, a call that changes
    its arguments, and that the C++ side reads every value back."""

    @classmethod
    def setUpClass(cls):
        cls.tmp = tempfile.TemporaryDirectory()
        cls.out = Path(cls.tmp.name) / "captures"
        saved = []
        for target in capture_passes.STAGE_PASSES.values():
            module, attr = capture_passes.resolve(target)
            saved.append((module, attr, getattr(module, attr)))
        try:
            capture_passes.install(list(capture_passes.STAGE_PASSES), cls.out)
            capture_passes._SOURCE = "examples/sfx_tone.c -S -O"
            src = _ROOT / "examples" / "sfx_tone.c"
            with mock.patch.dict(os.environ):
                os.environ.pop("C6502_CPP_BACKEND", None)
                assert compile_module.main([
                    "compile.py", str(src), "-S", "-O",
                    "-o", os.path.join(cls.tmp.name, "out.s"),
                ]) == 0
        finally:
            for module, attr, fn in saved:
                setattr(module, attr, fn)

    @classmethod
    def tearDownClass(cls):
        cls.tmp.cleanup()

    def _texts(self, name):
        return [p.read_bytes().decode("utf-8")
                for p in sorted((self.out / name).glob("*.txt"))]

    def test_records(self):
        [text] = self._texts("local_slot_symbols")
        # The pools come first, so they are the record's `input-json`.
        self.assertIn("\ninput-json\n{\"dict\": ", text)
        [text] = self._texts("synthesize_prologue")
        self.assertIn("\ninput\nProgram([", text)
        # tac_to_asm adds the `.reg` locals of register parameters to
        # the symbol table, which the record keeps on its `mutated` line
        # only when there are some.
        for text in self._texts("tac_to_asm"):
            lines = text.split("\n")
            self.assertTrue(lines[5].startswith(("mutated ", "input")))

    @unittest.skipUnless(cpp_cli(), "the C++ CLI isn't built (cpp/README.md)")
    def test_cpp_reads_every_value_back(self):
        result = subprocess.run(
            [cpp_cli(), "check-captures", str(self.out)],
            capture_output=True, text=True,
        )
        self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
        self.assertIn("synthesize_prologue: 1 calls, 1 round-trip, 1 match, 0 differ",
                      result.stdout)


class TestTacTables(unittest.TestCase):
    def test_every_tac_stage_function_resolves(self):
        for name, target in capture_passes.TAC_STAGE_PASSES.items():
            with self.subTest(name=name):
                module, attr = capture_passes.resolve(target)
                self.assertTrue(callable(getattr(module, attr)))

    def test_tac_names_are_new(self):
        # The names of step 5 don't take one of the asm passes' or the
        # stage calls', which `install` would wrap instead.
        older = {**capture_passes.PASSES, **capture_passes.STAGE_PASSES}
        for name in [*capture_passes.tac_passes(),
                     *capture_passes.TAC_STAGE_PASSES]:
            self.assertNotIn(name, older)

    def test_every_tac_call_has_a_cpp_signature(self):
        # The signatures of step 5 are in
        # cpp/src/passes/optimization/tac_stage.cpp, in the harness's
        # order: the driver's calls on one function, then the
        # program-level ones.
        source = (
            _ROOT / "cpp" / "src" / "passes" / "optimization" / "tac_stage.cpp"
        ).read_text(encoding="utf-8")
        names = re.findall(r'(?:\{\.name = |tac_pass\()"(\w+)"', source)
        self.assertEqual(names, [*capture_passes.tac_passes(),
                                 *capture_passes.TAC_STAGE_PASSES])


class TestTacCapture(unittest.TestCase):
    """Records the TAC optimizer's calls of `fold_truncate_extend` and
    the calls of `c99_to_tac` on one example, and checks that the
    symbols a record holds are enough to run the pass again."""

    @classmethod
    def setUpClass(cls):
        cls.tmp = tempfile.TemporaryDirectory()
        cls.out = Path(cls.tmp.name) / "captures"
        saved = compile_module.translate_to_tac
        try:
            capture_passes.install(
                ["tac_fold_truncate_extend", "tac_to_ssa", "c99_to_tac"],
                cls.out,
            )
            capture_passes._SOURCE = "examples/apply_bobble.c -S -O"
            src = _ROOT / "examples" / "apply_bobble.c"
            with mock.patch.dict(os.environ):
                os.environ.pop("C6502_CPP_BACKEND", None)
                assert compile_module.main([
                    "compile.py", str(src), "-S", "-O",
                    "-o", os.path.join(cls.tmp.name, "out.s"),
                ]) == 0
        finally:
            compile_module.translate_to_tac = saved
            capture_passes.uninstall_tac()

    @classmethod
    def tearDownClass(cls):
        cls.tmp.cleanup()

    def _texts(self, name):
        return [p.read_bytes().decode("utf-8")
                for p in sorted((self.out / name).glob("*.txt"))]

    def test_recorded_symbols_are_enough(self):
        import json
        import tac_ast
        from passes.optimization.framework import PassContext
        from passes.optimization.truncate_extend_fold import FoldTruncateExtend
        texts = self._texts("tac_fold_truncate_extend")
        self.assertTrue(texts)
        for text in texts:
            head, _, rest = text.partition("\ninput\n")
            args = json.loads(head.split("\n")[3][len("args "):])
            symbols = _decode_table(args["kwargs"]["symbols"])
            ssa = set(args["kwargs"]["ssa_dsts"]["set"])
            input_text, _, output_text = rest.partition("\noutput\n")
            fn = ir_dump.load(input_text, tac_ast)
            got = FoldTruncateExtend().run(
                fn, PassContext(symbols=symbols, ssa_dsts=ssa))
            self.assertEqual(ir_dump.dump(got), output_text)

    def test_to_ssa_records_the_symbols_it_adds(self):
        [text] = self._texts("tac_to_ssa")
        self.assertIn("\nmutated {\"args\": {}, \"kwargs\": {\"symbols\": ",
                      text)
        self.assertIn("\noutput-json\n{\"tuple\": [", text)

    def test_c99_to_tac_records_its_tables(self):
        [text] = self._texts("c99_to_tac")
        self.assertIn("\ninput\nProgram([", text)
        # It adds its temporaries to the symbol table.
        self.assertIn("\nmutated {\"args\": {\"0\": ", text)

    @unittest.skipUnless(cpp_cli(), "the C++ CLI isn't built (cpp/README.md)")
    def test_cpp_port_matches(self):
        result = subprocess.run(
            [cpp_cli(), "check-captures", str(self.out)],
            capture_output=True, text=True,
        )
        self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
        n = len(self._texts("tac_fold_truncate_extend"))
        self.assertIn(f"tac_fold_truncate_extend: {n} calls, {n} round-trip, "
                      f"{n} match, 0 differ", result.stdout)


class TestFrontTables(unittest.TestCase):
    def test_every_front_function_resolves(self):
        for name, target in capture_passes.FRONT.items():
            with self.subTest(name=name):
                module, attr = capture_passes.resolve(target)
                self.assertTrue(callable(getattr(module, attr)))

    def test_front_names_are_new(self):
        older = {**capture_passes.PASSES, **capture_passes.STAGE_PASSES,
                 **capture_passes.TAC_STAGE_PASSES,
                 **capture_passes.tac_passes()}
        for name in capture_passes.FRONT:
            self.assertNotIn(name, older)

    def test_every_front_call_has_a_cpp_signature(self):
        # The signatures of step 6 are in cpp/src/passes/front_stage.cpp,
        # in the harness's order.
        source = (
            _ROOT / "cpp" / "src" / "passes" / "front_stage.cpp"
        ).read_text(encoding="utf-8")
        names = re.findall(r'\{\.name = "(\w+)"', source)
        self.assertEqual(names, list(capture_passes.FRONT))


class TestFrontCapture(unittest.TestCase):
    """Records the front end's calls on one example and on a program
    the parser rejects, and checks the records' forms."""

    @classmethod
    def setUpClass(cls):
        cls.tmp = tempfile.TemporaryDirectory()
        cls.out = Path(cls.tmp.name) / "captures"
        saved = []
        for target in capture_passes.FRONT.values():
            module, attr = capture_passes.resolve(target)
            saved.append((module, attr, getattr(module, attr)))
        try:
            capture_passes.install(list(capture_passes.FRONT), cls.out)
            capture_passes._SOURCE = "examples/apply_bobble.c -S -O"
            src = _ROOT / "examples" / "apply_bobble.c"
            bad = Path(cls.tmp.name) / "bad.c"
            bad.write_text("int main(void) { while (1) {} break; }\n"
                           "int f(void) { return 1 +; }\n", encoding="utf-8")
            loop = Path(cls.tmp.name) / "loop.c"
            loop.write_text("int main(void) { break; }\n", encoding="utf-8")
            with mock.patch.dict(os.environ):
                os.environ.pop("C6502_CPP_BACKEND", None)
                assert compile_module.main([
                    "compile.py", str(src), "-S", "-O",
                    "-o", os.path.join(cls.tmp.name, "out.s"),
                ]) == 0
                with mock.patch("sys.stderr"):
                    for path in (bad, loop):
                        assert compile_module.main([
                            "compile.py", str(path), "-S",
                            "-o", os.path.join(cls.tmp.name, "out.s"),
                        ]) == 1
        finally:
            for module, attr, fn in saved:
                setattr(module, attr, fn)

    @classmethod
    def tearDownClass(cls):
        cls.tmp.cleanup()

    def _texts(self, name):
        return [p.read_bytes().decode("utf-8")
                for p in sorted((self.out / name).glob("*.txt"))]

    def test_parse_records_the_text_and_the_errors(self):
        texts = self._texts("parse")
        self.assertEqual(len(texts), 3)
        for text in texts:
            self.assertIn("\ninput-json\n\"", text)
        errors = [t for t in texts if "\noutput-error\n" in t]
        self.assertEqual(len(errors), 1)
        self.assertIn('{"type": "UnexpectedToken", "message": "Unexpected '
                      "token Token('SEMICOLON', ';') at line 2", errors[0])

    def test_type_checking_records_its_changes(self):
        [text] = self._texts("type_checking")
        self.assertIn("\nmutated {\"input\": {\"asdl\": \"c99_ast\"", text)
        self.assertIn("\noutput-json\n{\"tuple\": [{\"asdl\": \"c99_ast\"",
                      text)

    def test_loop_labeling_records_its_error(self):
        errors = [t for t in self._texts("loop_labeling")
                  if "\noutput-error\n" in t]
        self.assertEqual(len(errors), 1)
        self.assertIn('{"type": "LoopLabelingError", "message": "\'break\' '
                      'statement not inside a loop or switch"}', errors[0])

    @unittest.skipUnless(cpp_cli(), "the C++ CLI isn't built (cpp/README.md)")
    def test_cpp_port_matches(self):
        result = subprocess.run(
            [cpp_cli(), "check-captures", str(self.out)],
            capture_output=True, text=True,
        )
        self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
        for name in ("parse", "pp_number_at", "loop_labeling"):
            n = len(self._texts(name))
            self.assertIn(f"{name}: {n} calls, {n} round-trip, "
                          f"{n} match, 0 differ", result.stdout)


def _decode_table(v):
    """The `SymbolTable` of a record's tagged JSON, with the symbols'
    types and attributes as the type checker makes them."""
    import c99_ast
    from passes import type_checking
    table = type_checking.SymbolTable()
    for name, sym in v["fields"]["_table"]["dict"]:
        f = sym["fields"]
        attrs = f["attrs"]
        cls = getattr(type_checking, attrs["object"].rsplit(".", 1)[1])
        table[name] = type_checking.Symbol(
            type=ir_dump.load(f["type"]["dump"], c99_ast),
            attrs=_decode_attrs(cls, attrs["fields"]),
        )
    return table


def _decode_attrs(cls, fields):
    """An attribute object of `cls` from its recorded fields. Only the
    plain fields of `LocalAttr` and the like are decoded; a field of
    another form is left at its default, since `fold_truncate_extend`
    reads only the types."""
    plain = {k: v for k, v in fields.items()
             if v is None or type(v) in (bool, int, str)}
    try:
        return cls(**plain)
    except TypeError:
        return cls.__new__(cls)


if __name__ == "__main__":
    unittest.main()
