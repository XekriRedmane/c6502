import unittest

import c99_ast
from passes.label_resolution import LabelResolver


class TestErrors(unittest.TestCase):
    def test_unknown_block_item_raises_type_error(self):
        stub = type("Stub", (c99_ast.Type_block_item,), {})
        with self.assertRaises(TypeError):
            LabelResolver()._rewrite_block_item(stub(), {})


if __name__ == "__main__":
    unittest.main()
