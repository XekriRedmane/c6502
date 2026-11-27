# Translating Python tests to doctest with a draft generator

In batch 1 of G4 of step 9 (27 Nov 2026), 66 simulator test modules (278 tests, 10,000 lines) were ported by first writing a script that turned each module into a draft C++ file from its AST, and then fixing each draft by hand. The script (C:/w/G4a/py2cpp.py, a scratch copy) turned string constants into raw string literals, `build_sim` and `run_c_program` calls, `subTest` loops, and the assert methods into their C++ forms, and marked everything else `/*PY: ...*/` for the hand pass. The drafts of about half the modules compiled and passed with no edit.

- Comments come from `tokenize`, attached to the next statement by line number. A comment inside a list literal (the note above one case of a table of programs) lands after the whole list. Move them back from the Python list's element line numbers, as C:/w/G4a/move_comments.py did.
- Parenthesize by C++ precedence, not Python's. Python's `&` binds tighter than `==` and C++'s looser, so a translation that keeps Python's grouping without parentheses changes the meaning. Doctest's `CHECK(a << k == b)` doesn't compile, since `<<` on the left operand binds to doctest's decomposer, so a shift or bitwise operand on the left of a comparison needs parentheses too.
- An f-string used as a value becomes a call of a variadic `cat`, not `<<` chains, which only work inside `INFO` and the `_MESSAGE` macros.
- A helper the draft writes as a template, such as `auto codegen(const auto& src)` or `auto tac_main(const auto& src)`, loses to the library's non-template overload of the same name (`suite::codegen(const std::string&, bool = false)`) in a file inside `namespace c6502::suite`. The call then compiles and silently drops `-O`. Name such helpers apart from the library's functions.
- A Python class turned into a namespace named `sim` hides `c6502::sim`, and a function named `assert` collides with the macro. Rename both in the generator.
- Expected values computed in Python with unbounded integers overflow `int` in C++, as `a * b` of two values up to 65535 did in test_slot_coalescing. Use `std::int64_t` for the arithmetic of expected values.
- A dict or list of programs written as triple-quoted strings is easier to carry over with a script that imports the module and prints each value as a raw string literal than by hand.
- `$(cat paths.txt)` in a git command is refused by the worktree guard, so each `git add` names its paths literally.

Related: [[codebase/rewriting-tests-with-scripts]], [[codebase/parallel-port-agents]], [[codebase/worktree-guard]]
