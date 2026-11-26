# Boundary Discipline

Place validation, type narrowing, and error reporting at system boundaries. Inside, code trusts the data it is given and carries no defensive recovery.

**Why:** Validation scattered throughout is noisy, redundant, and gives a false sense of safety. Concentrating it at boundaries means each piece of data is validated exactly once, and a user's error is reported in one place with one message.

**The Pattern:**
- **At boundaries**: validate and report. In c6502 these are the preprocessor, the parser, and the type checker, which give a program that breaks a rule an error naming the rule, and the command lines of `c6502cpp`. `cpp_backend._exception` maps the C++ errors to Python classes in one place.
- **Inside the system**: no defensive recovery code. The passes after the type checker trust the IR. Internal invariants are checked by `ir_check` (cpp/src/passes/ir_check.h), which runs after each pass under `C6502_CHECK_IR=1` and fails with the pass's name, and never by code that silently repairs bad IR.

**The Tests:**
- "Is this data crossing a system boundary right now?" If not, validation is redundant.
- "Would this check repair the data, or report it?" Inside the system, only a check that fails loudly belongs.

**Evidence:** the "rule violations must error" rule in the main session's memory, [[codebase/asm-operand-side-effects]] (six miscompiles of internal invariants, found by tests and the fuzzers rather than by guards in the passes).
