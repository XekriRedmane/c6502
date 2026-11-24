// The calls that the tests of tests/test_preprocessor_debug_log.py made of the functions
// that `check-captures` runs, each with what the Python function
// returned, changed, or raised, which the C++ port must return,
// change, or raise too. tools/record_test_calls.py recorded them
// while the Python tests ran, before they were removed (step 8,
// groups G6 to G8). A call that another C++ case already checks
// isn't repeated here.
#include "doctest.h"
#include "record_case.h"
#include "preprocessor_case.h"

TEST_CASE("preprocess: test_preprocessor_debug_log.py::TestDebugLog::test_each_call_truncates_the_log (call 1)") {
  c6502::testing::check_preprocess_case(
      "{\"argv\": [\"--debug\"], \"cwd\": \"C:\\\\Users\\\\rober\\\\AppData\\\\Local\\\\Temp\\\\tmp2329h68y\", \"debug_log\": \"1:0:0 None:1 #define A 1\\r\\n1:0:0 None:1      A(None)=[LexToken(CPP_INTEGER,'1',1,10)]\\r\\n1:0:0 None:1 Determined that #include \\\"None\\\" is not entirely wrapped in an include guard macro, disabling auto-applying #pragma once\\r\\n1:0:0 None:2 #define F(x, ...) x\\r\\n1:0:0 None:2      F(['x', '__VA_ARGS__'])=[LexToken(CPP_ID,'x',2,30)]\\r\\n1:0:0 None:3 #if A\\r\\n1:1:0 None:5 #endif \\r\\n1:0:0 None:5      (None:3 #if A\\r\\n)\\r\\n1:0:0 None:0 Did not auto apply #pragma once to this file due to auto_pragma_once_possible=0, include_guard=None\\r\\n\", \"error\": null, \"filename\": null, \"files\": {}, \"include_dir\": \"C:\\\\Users\\\\rober\\\\c6502\\\\.claude\\\\worktrees\\\\agent-a491cc35feb6adb51\\\\include\", \"nt\": true, \"output\": \"\\n\\n\\n1\\n\", \"source\": \"#define A 1\\n#define F(x, ...) x\\n#if A\\nA\\n#endif\\n\", \"stderr\": \"\", \"warnings\": []}", {});
}

TEST_CASE("preprocess: test_preprocessor_debug_log.py::TestDebugLog::test_each_call_truncates_the_log (call 2)") {
  c6502::testing::check_preprocess_case(
      "{\"argv\": [\"--debug\"], \"cwd\": \"C:\\\\Users\\\\rober\\\\AppData\\\\Local\\\\Temp\\\\tmp2329h68y\", \"debug_log\": \"1:0:0 None:1 #undef B\\r\\n1:0:0 None:1 Determined that #include \\\"None\\\" is not entirely wrapped in an include guard macro, disabling auto-applying #pragma once\\r\\n1:0:0 None:0 Did not auto apply #pragma once to this file due to auto_pragma_once_possible=0, include_guard=None\\r\\n\", \"error\": null, \"filename\": null, \"files\": {}, \"include_dir\": \"C:\\\\Users\\\\rober\\\\c6502\\\\.claude\\\\worktrees\\\\agent-a491cc35feb6adb51\\\\include\", \"nt\": true, \"output\": \"\", \"source\": \"#undef B\\n\", \"stderr\": \"\", \"warnings\": []}", {});
}

TEST_CASE("preprocess: test_preprocessor_debug_log.py::TestDebugLog::test_log_after_an_error (call 1)") {
  c6502::testing::check_preprocess_case(
      "{\"argv\": [\"--debug\"], \"cwd\": \"C:\\\\Users\\\\rober\\\\AppData\\\\Local\\\\Temp\\\\tmp9vfm4dhr\", \"debug_log\": \"1:0:0 None:1 #define A 1\\r\\n1:0:0 None:1      A(None)=[LexToken(CPP_INTEGER,'1',1,10)]\\r\\n1:0:0 None:1 Determined that #include \\\"None\\\" is not entirely wrapped in an include guard macro, disabling auto-applying #pragma once\\r\\n1:0:0 None:2 #pragma c6502 loopkeep\\r\\n\", \"error\": [\"PreprocessorError\", \"line 2: c6502 no longer takes `#pragma c6502 loopkeep`; write the statement attribute `[[c6502::keep]]` before the loop instead, as in `[[c6502::keep]] for (...)`\"], \"filename\": null, \"files\": {}, \"include_dir\": \"C:\\\\Users\\\\rober\\\\c6502\\\\.claude\\\\worktrees\\\\agent-a491cc35feb6adb51\\\\include\", \"nt\": true, \"output\": null, \"source\": \"#define A 1\\n#pragma c6502 loopkeep\\n\", \"stderr\": \"\", \"warnings\": []}", {});
}

TEST_CASE("preprocess: test_preprocessor_debug_log.py::TestDebugLog::test_log_is_complete_after_a_collection (call 1)") {
  c6502::testing::check_preprocess_case(
      "{\"argv\": [\"--debug\"], \"cwd\": \"C:\\\\Users\\\\rober\\\\AppData\\\\Local\\\\Temp\\\\tmpko7lnyp4\", \"debug_log\": \"1:0:0 None:1 #define A 1\\r\\n1:0:0 None:1      A(None)=[LexToken(CPP_INTEGER,'1',1,10)]\\r\\n1:0:0 None:1 Determined that #include \\\"None\\\" is not entirely wrapped in an include guard macro, disabling auto-applying #pragma once\\r\\n1:0:0 None:2 #define F(x, ...) x\\r\\n1:0:0 None:2      F(['x', '__VA_ARGS__'])=[LexToken(CPP_ID,'x',2,30)]\\r\\n1:0:0 None:3 #if A\\r\\n1:1:0 None:5 #endif \\r\\n1:0:0 None:5      (None:3 #if A\\r\\n)\\r\\n1:0:0 None:0 Did not auto apply #pragma once to this file due to auto_pragma_once_possible=0, include_guard=None\\r\\n\", \"error\": null, \"filename\": null, \"files\": {}, \"include_dir\": \"C:\\\\Users\\\\rober\\\\c6502\\\\.claude\\\\worktrees\\\\agent-a491cc35feb6adb51\\\\include\", \"nt\": true, \"output\": \"\\n\\n\\n1\\n\", \"source\": \"#define A 1\\n#define F(x, ...) x\\n#if A\\nA\\n#endif\\n\", \"stderr\": \"\", \"warnings\": []}", {});
}
