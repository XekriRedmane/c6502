"""Summarizes a capture of tools/capture_binutils.py: for each kind, the
number of records, the number that raised, and each distinct error
with the number of records that give it. For `nm` and `objdump`, a
record whose exit status isn't 0 counts as an error, and its error is
its stderr's last line.

    uv run python tools/binutils_errors.py captures/binutils
"""
from __future__ import annotations

import json
import re
import sys
from collections import Counter
from pathlib import Path


def _shape(message: str) -> str:
    """`message` with its names, numbers, and paths replaced by
    placeholders, so that the errors of one raise site group together."""
    text = re.sub(r"\[[^\]]*\]", "[...]", message)
    text = re.sub(r"`[^`]*`", "`N`", text)
    # A quoted name, but not the apostrophe of "can't".
    text = re.sub(r"(?<![A-Za-z])'[^']*'", "'N'", text)
    text = re.sub(r"\$[0-9A-Fa-f]+|\b\d+\b", "#", text)
    return re.sub(r"^[^:\s]+\.(o|c|asm|ld|s): ", "<file>: ", text)


def summarize(root: Path) -> None:
    for kind_dir in sorted(p for p in root.iterdir()
                           if p.is_dir() and p.name not in ("blobs", "timing")):
        records = list(kind_dir.glob("*.txt"))
        errors: Counter = Counter()
        for path in records:
            text = path.read_text(encoding="utf-8")
            if "\noutput-error\n" in text:
                err = json.loads(text.split("\noutput-error\n", 1)[1])
                errors[f"{err['type']}: {_shape(err['message'])}"] += 1
            elif kind_dir.name in ("nm", "objdump"):
                out = json.loads(text.split("\noutput-json\n", 1)[1])
                if out["status"] != 0:
                    last = out["stderr"].rstrip("\n").splitlines()[-1:] or [""]
                    errors[f"status {out['status']}: {_shape(last[0])}"] += 1
        print(f"{kind_dir.name}: {len(records)} records, "
              f"{sum(errors.values())} errors, {len(errors)} shapes")
        for shape, n in sorted(errors.items(), key=lambda kv: -kv[1]):
            print(f"  {n:6d}  {shape}")


if __name__ == "__main__":
    # A message of the linker's holds an arrow, which the console's
    # code page may not have.
    sys.stdout.reconfigure(encoding="utf-8")
    for arg in sys.argv[1:]:
        summarize(Path(arg))
