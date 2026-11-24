import unittest

from lexer import KEYWORDS


class TestKeywords(unittest.TestCase):

    def test_keyword_count(self):
        # The 37 standard C99 keywords, C23's `bool`, `true`, `false`,
        # `nullptr`, `_BitInt`, `constexpr`, `thread_local`, `typeof`,
        # and `typeof_unqual`, and C11's `_Thread_local`. Attributes
        # are C23's `[[...]]`, which takes no keyword.
        self.assertEqual(len(KEYWORDS), 47)


if __name__ == "__main__":
    unittest.main()
