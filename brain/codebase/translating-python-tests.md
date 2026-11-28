# Pitfalls in doctest cases, found translating the Python tests

G4 of step 9 (27 Nov 2026) ported 66 simulator test modules to doctest from drafts that a script wrote from each module's AST. The Python tests are gone since group R of step 9, and these are the pitfalls of the C++ side, which a case written by hand meets too.

- Parenthesize by C++ precedence. C++'s `&` binds looser than `==`, where Python's binds tighter. Doctest's `CHECK(a << k == b)` doesn't compile, since `<<` on the left operand binds to doctest's decomposer, so a shift or bitwise operand on the left of a comparison needs parentheses.
- A formatted message used as a value is a call of a variadic `cat`. `<<` chains work only inside `INFO` and the `_MESSAGE` macros.
- In a file inside `namespace c6502::suite`, overload resolution prefers the library's non-template function (`suite::codegen(const std::string&, bool = false)`) to a helper of the same name written as a template, such as `auto codegen(const auto& src)` or `auto tac_main(const auto& src)`. The call then compiles and silently drops `-O`. Name such helpers apart from the library's functions.
- A namespace named `sim` in a test file hides `c6502::sim`, and a function named `assert` collides with the macro.
- An expected value computed as `a * b` of two values up to 65535 overflows `int`, as it did in test_slot_coalescing. Use `std::int64_t` for the arithmetic of expected values.

Related: [[codebase/parallel-port-agents]], [[codebase/simulator-harness]]
