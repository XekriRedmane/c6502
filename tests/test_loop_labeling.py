import unittest

import c99_ast
from passes.loop_labeling import LoopLabeler


class TestMakeLabel(unittest.TestCase):
    def test_format_is_dot_loop_underscore_counter(self):
        lbl = LoopLabeler()
        self.assertEqual(lbl.make_label(), ".loop@0")
        self.assertEqual(lbl.make_label(), ".loop@1")

    def test_counter_is_program_unique(self):
        # One LoopLabeler per program; the counter doesn't reset
        # between functions visited by the same instance.
        lbl = LoopLabeler()
        self.assertEqual(lbl.make_label(), ".loop@0")
        self.assertEqual(lbl.make_label(), ".loop@1")
        self.assertEqual(lbl.make_label(), ".loop@2")


class TestErrors(unittest.TestCase):
    def test_unknown_statement_raises_type_error(self):
        from passes.loop_labeling import _LabelState
        stub = type("Stub", (c99_ast.Type_statement,), {})
        with self.assertRaises(TypeError):
            LoopLabeler().label_statement(stub(), _LabelState())

    def test_unknown_block_item_raises_type_error(self):
        from passes.loop_labeling import _LabelState
        stub = type("Stub", (c99_ast.Type_block_item,), {})
        with self.assertRaises(TypeError):
            LoopLabeler().label_block_item(stub(), _LabelState())

    def test_unknown_block_raises_type_error(self):
        from passes.loop_labeling import _LabelState
        stub = type("Stub", (c99_ast.Type_block,), {})
        with self.assertRaises(TypeError):
            LoopLabeler().label_block(stub(), _LabelState())


if __name__ == "__main__":
    unittest.main()
