"""Pretty-printer for dataclass trees.

Generic: walks any `@dataclass` instance and produces a multi-line Python-
ish rendering with 2-space indentation. Nested dataclasses recurse. Lists
are broken onto their own lines. Other values go through `repr()`.

The output is valid Python given a namespace with the AST classes in
scope, which is handy for diffing and reconstructing nodes in tests.
"""

from __future__ import annotations

from dataclasses import fields, is_dataclass
from typing import Any


_INDENT = "  "


def pretty(node: Any, depth: int = 0) -> str:
    if is_dataclass(node) and not isinstance(node, type):
        cls = type(node).__name__
        fs = fields(node)
        if not fs:
            return f"{cls}()"
        lines = [f"{cls}("]
        for f in fs:
            val = pretty(getattr(node, f.name), depth + 1)
            lines.append(f"{_INDENT * (depth + 1)}{f.name}={val},")
        lines.append(f"{_INDENT * depth})")
        return "\n".join(lines)
    if isinstance(node, list):
        if not node:
            return "[]"
        lines = ["["]
        for item in node:
            lines.append(f"{_INDENT * (depth + 1)}{pretty(item, depth + 1)},")
        lines.append(f"{_INDENT * depth}]")
        return "\n".join(lines)
    return repr(node)
