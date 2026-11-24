"""Freezes the records of the capture harnesses into the regression inputs
that `c6502cpp check-frozen` checks the C++ port against once the Python
compiler is gone (docs/cpp_port.md, "The checks after Python").

    uv run python tools/capture_passes.py --passes all -j 12
    uv run python tools/freeze_captures.py passes captures

    C6502_CPP_BACKEND=1 uv run python tools/capture_binutils.py -j 12
    uv run python tools/freeze_captures.py binutils captures/binutils

For each function under the capture directory, the freezer keeps every
record of a call that raised and the 50 smallest records (`--keep`) of
the calls that changed the result, and writes them to one archive,
compressed with xz, `cpp/tests/frozen/<kind>/<function>.xz`, in place
of the archive that was there. An archive of the kind `binutils` holds
the blobs its records name too, so that it reads as a capture directory
of its own.

A record of tools/capture_passes.py changed the result when the call
raised, when its output differs from its input (both are IR dumps, or
both tagged JSON), when it changed an argument in place (its `mutated`
line), or when its input and its output are of different kinds, as for
a call that takes a program and returns a table. When a function has
fewer than `--keep` such calls, the smallest of its other calls make up
the number, so that every function recorded is checked on something.
Every record of tools/capture_binutils.py counts as changing the result,
since its functions read files and write others, except a record that
names a file by a Windows path, which is left out. A record's size is
its length in bytes, with the blobs it names for `binutils`.

The archive is an xz stream (CRC32 check, which the vendored decoder in
cpp/third_party/xz-embedded reads) of

    c6502-frozen 1
    file <path> <length>
    <the file's bytes>
    ...

with each path relative to the capture directory, `<function>/<name>.txt`
for a record and `blobs/<sha1>` for a blob, in sorted order. The
`source` line of each record is kept, since it names the compile that
made the call.
"""
from __future__ import annotations

import argparse
import lzma
import re
from pathlib import Path

_ROOT = Path(__file__).resolve().parent.parent
_FROZEN = _ROOT / "cpp" / "tests" / "frozen"
_FORMAT = b"c6502-frozen 1\n"

_HASH = re.compile(rb"\b[0-9a-f]{40}\b")

# A Windows path in a record's JSON, such as "C:\\Users\\...\\u0.o". A
# record of the suite that names an object by such a path gives another
# result on Linux, where `Path(path).name` is the whole path (cpp/README.md,
# "The link"), so the freezer leaves those records out.
_WINDOWS_PATH = re.compile(rb"[A-Za-z]:\\\\")


def _sections(text: bytes) -> tuple[dict[bytes, bytes], bytes, bytes, bytes, bytes]:
    """The header lines of a record by their first word, the kind of its
    input line and the input, and the kind of its output line and the
    output."""
    head: dict[bytes, bytes] = {}
    pos = 0
    while True:
        end = text.index(b"\n", pos)
        line = text[pos:end]
        if line.startswith(b"input"):
            input_kind = line
            pos = end + 1
            break
        word, _, rest = line.partition(b" ")
        head[word] = rest
        pos = end + 1
    marks = [(text.find(b"\n" + m + b"\n", pos - 1), m)
             for m in (b"output", b"output-json", b"output-error")]
    at, output_kind = min((a, m) for a, m in marks if a >= 0)
    return (head, input_kind, text[pos:at + 1], output_kind,
            text[at + len(output_kind) + 2:])


def _pass_record(text: bytes) -> tuple[bool, bool]:
    """Whether a record of tools/capture_passes.py raised, and whether
    it changed the result."""
    head, input_kind, inp, output_kind, out = _sections(text)
    if output_kind == b"output-error":
        return True, True
    if b"mutated" in head:
        return False, True
    same_kind = (input_kind == b"input") == (output_kind == b"output")
    return False, not same_kind or inp != out


def _archive(files: dict[str, bytes]) -> bytes:
    parts = [_FORMAT]
    for path in sorted(files):
        data = files[path]
        parts.append(f"file {path} {len(data)}\n".encode("utf-8"))
        parts.append(data)
    return lzma.compress(b"".join(parts), format=lzma.FORMAT_XZ,
                         check=lzma.CHECK_CRC32,
                         preset=9 | lzma.PRESET_EXTREME)


def _select(records: list[tuple[Path, bytes, int, bool, bool]],
            keep: int) -> list[tuple[Path, bytes]]:
    """The records to keep of one function's, each given as its path,
    its bytes, its size, whether it raised, and whether it changed the
    result."""
    raised = [r for r in records if r[3]]
    changed = sorted((r for r in records if r[4] and not r[3]),
                     key=lambda r: (r[2], r[0].name))
    others = sorted((r for r in records if not r[4] and not r[3]),
                    key=lambda r: (r[2], r[0].name))
    chosen = raised + changed[:keep]
    if len(changed) < keep:
        chosen += others[:keep - len(changed)]
    return [(r[0], r[1]) for r in chosen]


def freeze(kind: str, captures: Path, keep: int, names: list[str] | None) -> int:
    out_dir = _FROZEN / kind
    out_dir.mkdir(parents=True, exist_ok=True)
    blobs = captures / "blobs"
    total_raw = total_xz = 0
    rows = []
    for d in sorted(p for p in captures.iterdir() if p.is_dir()):
        if d.name == "blobs" or (names and d.name not in names):
            continue
        records = []
        for path in sorted(d.glob("*.txt")):
            text = path.read_bytes()
            if kind == "binutils":
                if _WINDOWS_PATH.search(text):
                    continue
                named = {h.decode() for h in _HASH.findall(text)
                         if (blobs / h.decode()).is_file()}
                size = len(text) + sum((blobs / h).stat().st_size for h in named)
                raised = b"\noutput-error\n" in text
                records.append((path, text, size, raised, True))
            else:
                raised, changed = _pass_record(text)
                records.append((path, text, len(text), raised, changed))
        if not records:
            continue
        files: dict[str, bytes] = {}
        for path, text in _select(records, keep):
            files[f"{d.name}/{path.name}"] = text
            if kind == "binutils":
                for h in _HASH.findall(text):
                    blob = blobs / h.decode()
                    if blob.is_file():
                        files[f"blobs/{h.decode()}"] = blob.read_bytes()
        data = _archive(files)
        (out_dir / f"{d.name}.xz").write_bytes(data)
        raw = sum(len(v) for v in files.values())
        total_raw += raw
        total_xz += len(data)
        n = sum(1 for p in files if not p.startswith("blobs/"))
        rows.append((d.name, len(records), n, raw, len(data)))
    for name, recorded, kept, raw, packed in rows:
        print(f"{name}: {recorded} records, {kept} kept, {raw} bytes, {packed} with xz")
    print(f"{len(rows)} functions, {total_raw} bytes, {total_xz} with xz")
    return 0


def main(argv: list[str] | None = None) -> int:
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("kind", choices=("passes", "binutils"))
    ap.add_argument("captures", help="the capture directory")
    ap.add_argument("--keep", type=int, default=50)
    ap.add_argument("--only", help="comma-separated functions to freeze")
    ns = ap.parse_args(argv)
    return freeze(ns.kind, Path(ns.captures), ns.keep,
                  ns.only.split(",") if ns.only else None)


if __name__ == "__main__":
    raise SystemExit(main())
