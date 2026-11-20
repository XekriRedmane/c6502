"""Caches the fields of each dataclass, for the walks over IR nodes.

Many passes visit every field of an IR node, and
`dataclasses.fields` builds a new tuple of the class's fields on each
call, which in compiling 20 `fuzz.stmts` programs came to 1.5 million
calls. A dataclass's fields are fixed when the class is made, so
`fields_of` and `field_names` compute them once per class and return
the same tuple afterward. Each gives what `dataclasses.fields` gives
for the same argument, and raises the same `TypeError` for something
that isn't a dataclass or an instance of one.
"""

from __future__ import annotations

import dataclasses

# The fields of each dataclass that `fields_of` has been given an
# instance of, and their names.
_FIELDS: dict[type, tuple[dataclasses.Field, ...]] = {}
_NAMES: dict[type, tuple[str, ...]] = {}


def fields_of(obj) -> tuple[dataclasses.Field, ...]:
    """Returns `dataclasses.fields(obj)` for a dataclass or an instance
    of one, computed once per class of instance."""
    cls = type(obj)
    found = _FIELDS.get(cls)
    if found is None:
        if isinstance(obj, type):
            # A dataclass itself, whose class is `type`.
            return dataclasses.fields(obj)
        found = _FIELDS[cls] = dataclasses.fields(obj)
    return found


def field_names(obj) -> tuple[str, ...]:
    """Returns the names of `fields_of(obj)`, in order."""
    cls = type(obj)
    found = _NAMES.get(cls)
    if found is None:
        names = tuple(f.name for f in fields_of(obj))
        if isinstance(obj, type):
            return names
        found = _NAMES[cls] = names
    return found
