"""Runs the C++ port of each ported pass on every recorded call of it
and reports the count and any mismatch.

    uv run python tools/capture_passes.py --passes split_mem_to_mem
    uv run python tools/check_captures.py split_mem_to_mem

With no pass named, every pass under the capture directory is checked,
and the calls of the passes not ported yet have their dumps round-
tripped through the C++ reader and writer. The work is done by
`c6502cpp check-captures` (cpp/src/main.cpp), from cpp/build/bin or
the path in `C6502_CPP`. The exit status is 0 when every call
matched.
"""
from __future__ import annotations

import argparse
import os
import subprocess
import sys
from pathlib import Path

_ROOT = Path(__file__).resolve().parent.parent


def cpp_cli() -> Path | None:
    """Returns the path of the built C++ CLI, or None."""
    env = os.environ.get("C6502_CPP")
    if env:
        return Path(env)
    for name in ("c6502cpp.exe", "c6502cpp"):
        path = _ROOT / "cpp" / "build" / "bin" / name
        if path.exists():
            return path
    return None


def main(argv: list[str] | None = None) -> int:
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("passes", nargs="*", help="the passes to check")
    ap.add_argument("--captures", default=str(_ROOT / "captures"))
    ap.add_argument("--verbose", action="store_true")
    ns = ap.parse_args(argv)
    cli = cpp_cli()
    if cli is None:
        print("check_captures: build the C++ port first (cpp/README.md)",
              file=sys.stderr)
        return 2
    captures = Path(ns.captures)
    status = 0
    targets = [captures / p for p in ns.passes] or [captures]
    for target in targets:
        if not target.exists():
            print(f"check_captures: no captures in {target}", file=sys.stderr)
            status = 2
            continue
        cmd = [str(cli), "check-captures", str(target)]
        if ns.verbose:
            cmd.append("--verbose")
        status = max(status, subprocess.run(cmd).returncode)
    return status


if __name__ == "__main__":
    raise SystemExit(main())
