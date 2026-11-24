// The modes of `c6502cpp backend` that run the TAC interpreter
// (src/tac_sim.h), for the Python callers of tac_sim.py under
// `C6502_CPP_BACKEND=1` (cpp_backend.py's `tac_run`,
// `tac_first_changing_pass`, and `tac_falls_off_end`).
#pragma once

#include <string>

#include "json.h"

namespace c6502::tac_sim_modes {

// Returns whether `mode` is one of the modes below.
bool is_tac_mode(const std::string& mode);

// Returns the response to a request of `mode` whose arguments are
// `kwargs` and whose source text is `source`. The arguments are those
// of the mode `source` that the front end reads (`pcpp_args`,
// `filename`, `include_dir`, and `preprocess`), and the front end is
// `tests.test_sim_chapters._run_program`'s: the preprocessor, the
// parser, the three passes that resolve names, string lifting, the type
// checker, and `c99_to_tac`, without `-O`'s unrolling or inlining. Every
// response begins with the `stderr` and `warnings` lines of the mode
// `source` (src/backend.h), and a program that a pass rejects ends there
// with its `error` line.
//
// `tac-run` runs `main` on the TAC, as `_run_program` does, and with the
// argument `optimized` true also on the TAC after the TAC optimizer
// (`passes.optimization.optimize_program`), as fuzz/reduce.py's
// `_tac_results` does. The rest of the response is
//
//     result <JSON>
//     optimized <JSON>          (with `optimized`)
//
// where each JSON is `{"value": <int or null>}`, or `{"error": {"type":
// ..., "message": ...}}` with the Python class and message of what the
// interpreter raised. The optimizer runs only when the first run gives a
// value, since Python's `_tac_results` raises the first run's error.
//
// `tac-observe` is fuzz/tac_passes.py's `first_changing_pass`: it runs
// `main` on the TAC, then optimizes the program, and after each step of
// the optimizer's driver (`framework::set_observer`) runs `main` on the
// program with the function at that step. The rest of the response is
//
//     found <JSON>
//
// with `null` when no step changes `main`'s result, or `[function,
// step, expected, got]` for the first that does. A step is named by
// the Python class of its pass (`NamedPass::class_name`), as
// fuzz/tac_passes.py names it, or `to_ssa` or `from_ssa`. A result is an integer, `null` for no value,
// or `"ERR <type>: <message>"`.
//
// `tac-falls-off` is fuzz/reduce.py's `falls_off_end`. It declares
// `void __reduce_fell_off(void);` before the source, appends a call of
// it to the body of each function definition that returns a value,
// other than `main`, after parsing and before identifier resolution,
// and runs `main`. The rest of the response is
//
//     fell <true or false>
//
// which is true when the run raised the interpreter's error for the
// call of `__reduce_fell_off`, a function that no program defines. A
// program that a pass rejects, or that raises any other error, gives
// false with no `error` line, since reduce.py treats it as an invalid
// candidate.
std::string respond(const std::string& mode, const json::Value& kwargs, const std::string& source);

}  // namespace c6502::tac_sim_modes
