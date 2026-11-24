"""Freezes the records of the capture harnesses into the regression inputs
that `c6502cpp check-frozen` checks the C++ port against once the Python
compiler is gone (docs/cpp_port.md, "The checks after Python").

    uv run python tools/capture_passes.py --passes all -j 12
    uv run python tools/freeze_captures.py passes captures

    uv run python tools/capture_binutils.py -j 12
    C6502_BINUTILS_CAPTURES=captures/binutils PYTHONPATH=tools \\
        uv run pytest -p capture_binutils -n 12 -q
    uv run python tools/freeze_captures.py binutils captures/binutils

The binutils corpus is captured without `C6502_CPP_BACKEND`, since with
it the C++ port assembles and links, and the Python assembler and
linker that the harness records don't run.

For each function under the capture directory, the freezer keeps the
smallest record of a call that raised for each form of the error's
message, and the 50 smallest records (`--keep`) of the calls that
changed the result, and writes them to one archive,
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

The form of a message is its exception's class and its text with each
run of digits, each name in backquotes or single quotes, and each
bracketed list replaced by a placeholder (`_message_form`). Every call
that raised would be about 70 MB of the links' records alone, most of
them the 552 links of the corpus's programs that leave a helper or a
name of their game undefined, which give two forms of one message, so
the freezer keeps one call of each form, which reaches each raise
site with each wording the corpus gives it.

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
import json
import lzma
import os
import re
from concurrent.futures import ProcessPoolExecutor
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


_FORM_PARTS = (
    (re.compile(r"`[^`]*`"), "`X`"),
    (re.compile(r"'[^']*'"), "'X'"),
    (re.compile(r"\[[^\]]*\]"), "[X]"),
    (re.compile(r"[0-9]+"), "N"),
)


def _message_form(text: bytes) -> str:
    """The form of the error a raising record holds: its class and its
    message with the parts that name things replaced."""
    error = json.loads(text[text.index(b"\noutput-error\n") + 14:].split(b"\n")[0])
    message = error["message"]
    for pattern, placeholder in _FORM_PARTS:
        message = pattern.sub(placeholder, message)
    return f"{error['type']}: {message}"


def _select(records: list[tuple[Path, bytes, int, bool, bool]],
            keep: int) -> list[tuple[Path, bytes]]:
    """The records to keep of one function's, each given as its path,
    its bytes, its size, whether it raised, and whether it changed the
    result."""
    raised = []
    forms = set()
    for r in sorted((r for r in records if r[3]), key=lambda r: (r[2], r[0].name)):
        form = _message_form(r[1])
        if form not in forms:
            forms.add(form)
            raised.append(r)
    changed = sorted((r for r in records if r[4] and not r[3]),
                     key=lambda r: (r[2], r[0].name))
    others = sorted((r for r in records if not r[4] and not r[3]),
                    key=lambda r: (r[2], r[0].name))
    chosen = raised + changed[:keep]
    if len(changed) < keep:
        chosen += others[:keep - len(changed)]
    return [(r[0], r[1]) for r in chosen]


def _freeze_one(job) -> tuple | None:
    """Freezes the records of the function directory `d` and returns
    its row for the summary, or None when it holds none."""
    kind, d, blob_sizes, keep, out_dir = job
    blobs = d.parent / "blobs"
    records = []
    for path in sorted(d.glob("*.txt")):
        text = path.read_bytes()
        if kind == "binutils":
            if _WINDOWS_PATH.search(text):
                continue
            named = {h.decode() for h in _HASH.findall(text)} & blob_sizes.keys()
            size = len(text) + sum(blob_sizes[h] for h in named)
            raised = b"\noutput-error\n" in text
            records.append((path, text, size, raised, True))
        else:
            raised, changed = _pass_record(text)
            records.append((path, text, len(text), raised, changed))
    if not records:
        return None
    files: dict[str, bytes] = {}
    for path, text in _select(records, keep):
        files[f"{d.name}/{path.name}"] = text
        if kind == "binutils":
            for h in {h.decode() for h in _HASH.findall(text)} & blob_sizes.keys():
                files[f"blobs/{h}"] = (blobs / h).read_bytes()
    data = _archive(files)
    (out_dir / f"{d.name}.xz").write_bytes(data)
    raw = sum(len(v) for v in files.values())
    n = sum(1 for p in files if not p.startswith("blobs/"))
    return d.name, len(records), n, raw, len(data)


def freeze(kind: str, captures: Path, keep: int, names: list[str] | None,
           jobs: int) -> int:
    out_dir = _FROZEN / kind
    out_dir.mkdir(parents=True, exist_ok=True)
    blob_dir = captures / "blobs"
    blob_sizes = ({p.name: p.stat().st_size for p in blob_dir.iterdir()}
                  if blob_dir.is_dir() else {})
    dirs = [d for d in sorted(p for p in captures.iterdir() if p.is_dir())
            if d.name != "blobs" and (not names or d.name in names)]
    with ProcessPoolExecutor(max_workers=jobs) as pool:
        rows = [r for r in pool.map(
            _freeze_one, [(kind, d, blob_sizes, keep, out_dir) for d in dirs])
            if r is not None]
    for name, recorded, kept, raw, packed in rows:
        print(f"{name}: {recorded} records, {kept} kept, {raw} bytes, {packed} with xz")
    print(f"{len(rows)} functions, {sum(r[3] for r in rows)} bytes, "
          f"{sum(r[4] for r in rows)} with xz")
    return 0


def main(argv: list[str] | None = None) -> int:
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("kind", choices=("passes", "binutils"))
    ap.add_argument("captures", help="the capture directory")
    ap.add_argument("--keep", type=int, default=50)
    ap.add_argument("--only", help="comma-separated functions to freeze")
    ap.add_argument("-j", type=int, default=os.cpu_count() or 1)
    ns = ap.parse_args(argv)
    return freeze(ns.kind, Path(ns.captures), ns.keep,
                  ns.only.split(",") if ns.only else None, ns.j)


if __name__ == "__main__":
    raise SystemExit(main())
