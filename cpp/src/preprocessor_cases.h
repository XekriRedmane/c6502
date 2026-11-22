// The check of a recorded call of `preprocessor.preprocess`, which the
// C++ tests of the preprocessor and `c6502cpp check-preprocess` run.
//
// A record is the JSON object that the recorder of the Python suite's
// calls writes: `source`, `argv` (a list or null), `filename`, `cwd`,
// `nt` (whether the paths are Windows paths), `include_dir` (c6502's
// headers), `files` (each file pcpp opened, by its path, with its bytes
// as the code points 0 to 255), and Python's `output` (the text, or
// null), `error` (`[type, message]`, or null), `warnings`
// (`[[where, message], ...]`), and `stderr`. A record of a call with
// `--debug` may hold `debug_log`, the bytes Python left in
// `pcpp_debug.log` as the code points 0 to 255, or null when the call
// didn't create the file. The C++ preprocessor runs
// on a host that holds only those files, with the paths of the system
// the call was recorded on, so a record made on Windows is checked the
// same way on Linux.
#pragma once

#include <map>
#include <string>
#include <string_view>

namespace c6502::preprocessor {

// Runs the recorded call and returns "" when the C++ gives Python's
// result, warnings, and stderr, or a description of the first
// difference. A file whose bytes in the record are `@<key>` has the
// bytes of `(*shared)[key]`, so that the records of a test can share
// the text of a header they all include.
std::string check_recorded_call(std::string_view record,
                                const std::map<std::string, std::string>* shared = nullptr);

}  // namespace c6502::preprocessor
