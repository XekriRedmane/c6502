# Rewriting many test modules with a script

- Edit by the AST's positions (`lineno`, `col_offset`, `end_lineno`, `end_col_offset` turned into offsets), apply the edits of a file from its end backward, and assert that no two overlap. Write the files only after every file has been processed, since a failed assertion halfway through left 69 modules rewritten in G9.
- Fix the imports by scope. A call of a plain name binds to the nearest enclosing function, or the module, whose own body imports it. Adding a new name to every `from compile import` line of a file made unused imports wherever a line imported something else, and a function-local import has to follow the calls in that function.
- Place a new module-level import after the last `Import` or `ImportFrom` node of the module's body. A line-based scan took a docstring line that began "from the carry of" for an import and put the new import inside the docstring.
- Check the result with pyflakes before and after: run `uvx pyflakes tests/*.py` on HEAD's tests (extracted with `git archive HEAD tests | tar -x -C <dir>`) and on the rewritten ones, strip the line numbers, sort, and diff. A rewrite adds no line to the diff when it is clean.
- A rewrite can reach a test that exists to check the old form. In G9, tests of `_run_stage` given the preprocessor's output were rewritten to `_run_source` and had to be restored, so read the diff of every rewritten module before committing.

Related: [[codebase/surveying-test-dependencies]], [[codebase/git-bash-on-windows]], [[principles/make-checks-show-their-coverage]]
