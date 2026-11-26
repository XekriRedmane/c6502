"""`TypeCheckError`, the error of the type checker
(cpp/src/passes/type_checking.h), which errors.py re-exports.

It has a module of its own so that passes/runtime_layout.py can derive
`RuntimeLayoutError` from it without importing errors.py.
"""


class TypeCheckError(Exception):
    """Raised on any type-level inconsistency in the program."""
