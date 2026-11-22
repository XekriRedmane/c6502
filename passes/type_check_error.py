"""`TypeCheckError`, the error of the type checker (passes/type_checking.py),
which re-exports it.

It has a module of its own so that a module that only raises it or
derives an error from it, as passes/runtime_layout.py derives
`RuntimeLayoutError`, doesn't import the type checker. Under
`C6502_CPP_BACKEND=1`, compile.py measures the runtime helpers' HARGS
footprints with runtime_layout and runs the whole front end in C++, and
this keeps the type checker and the passes it imports out of that
process.
"""


class TypeCheckError(Exception):
    """Raised on any type-level inconsistency in the program."""
