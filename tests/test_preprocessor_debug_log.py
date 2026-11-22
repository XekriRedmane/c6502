"""Tests for the `--debug` log, `pcpp_debug.log` in the current
directory, which pcpp writes a line to for each directive and each
decision about an include (preprocessor.py's `CompilePreprocessor.run`).

Coverage:

  * The log holds every line once `preprocess` returns, and the garbage
    collector doesn't lose it. pcpp never closed the log, and the
    preprocessor sits in a reference cycle, so a collection that freed
    it during a compile used to leave the file empty.
  * When the preprocessor raises, the log holds the lines before the
    error.
  * Each preprocess truncates the log, so it holds the last call's lines.
"""
from __future__ import annotations

import gc
import os
import tempfile
import unittest

from preprocessor import PreprocessorError, preprocess

_LOG = (
    "1:0:0 None:1 #define A 1\n"
    "1:0:0 None:1      A(None)=[LexToken(CPP_INTEGER,'1',1,10)]\n"
    '1:0:0 None:1 Determined that #include "None" is not entirely wrapped '
    "in an include guard macro, disabling auto-applying #pragma once\n"
    "1:0:0 None:2 #define F(x, ...) x\n"
    "1:0:0 None:2      F(['x', '__VA_ARGS__'])=[LexToken(CPP_ID,'x',2,30)]\n"
    "1:0:0 None:3 #if A\n"
    "1:1:0 None:5 #endif \n"
    "1:0:0 None:5      (None:3 #if A\n"
    ")\n"
    "1:0:0 None:0 Did not auto apply #pragma once to this file due to "
    "auto_pragma_once_possible=0, include_guard=None\n"
)

_SOURCE = "#define A 1\n#define F(x, ...) x\n#if A\nA\n#endif\n"


class TestDebugLog(unittest.TestCase):
    def setUp(self) -> None:
        self._cwd = os.getcwd()
        self._dir = tempfile.TemporaryDirectory()
        os.chdir(self._dir.name)

    def tearDown(self) -> None:
        os.chdir(self._cwd)
        self._dir.cleanup()

    def _log(self) -> str:
        with open("pcpp_debug.log") as f:
            return f.read()

    def test_log_is_complete_after_a_collection(self) -> None:
        preprocess(_SOURCE, ["--debug"])
        # The collection used to free the preprocessor with the log's
        # text still in its buffer, which was then lost.
        gc.collect()
        self.assertEqual(self._log(), _LOG)

    def test_log_after_an_error(self) -> None:
        with self.assertRaises(PreprocessorError):
            preprocess("#define A 1\n#pragma c6502 loopkeep\n", ["--debug"])
        gc.collect()
        self.assertEqual(
            self._log(),
            "1:0:0 None:1 #define A 1\n"
            "1:0:0 None:1      A(None)=[LexToken(CPP_INTEGER,'1',1,10)]\n"
            '1:0:0 None:1 Determined that #include "None" is not entirely '
            "wrapped in an include guard macro, disabling auto-applying "
            "#pragma once\n"
            "1:0:0 None:2 #pragma c6502 loopkeep\n",
        )

    def test_each_call_truncates_the_log(self) -> None:
        preprocess(_SOURCE, ["--debug"])
        preprocess("#undef B\n", ["--debug"])
        gc.collect()
        self.assertEqual(
            self._log(),
            "1:0:0 None:1 #undef B\n"
            '1:0:0 None:1 Determined that #include "None" is not entirely '
            "wrapped in an include guard macro, disabling auto-applying "
            "#pragma once\n"
            "1:0:0 None:0 Did not auto apply #pragma once to this file due to "
            "auto_pragma_once_possible=0, include_guard=None\n",
        )


if __name__ == "__main__":
    unittest.main()
