"""Writes the golden files of the corpus with the frozen Python compiler,
which `c6502cpp check-golden` checks the C++ compiler against once the
Python compiler is gone (docs/cpp_port.md, "The checks after Python").

    uv run python tools/write_golden.py -j 12
    uv run python tools/write_golden.py --modes O,unopt -j 12

The jobs are tools/compare_backends.py's: each program of the corpus
(the examples, the games' units, the chapters' programs, and seeds 0 to
99 of `fuzz.stmts`, `fuzz.calls`, and `fuzz.exprs`) in each mode of
`MODES`, and the `links` jobs, which compile and link the three games
and link the damaged files and the bad command lines. Each job runs its
commands with compare_backends.py's functions, in an empty directory,
with the C++ back end off, and the writer records what each command
gave: its exit status, what it wrote to stdout and to stderr, and the
type and message of an exception it raised. It records the files the
job leaves too, as text beside the manifest for the outputs `MODES`
names, which are the `-S` texts, and as a SHA-256 for every other file.

The files go under `golden/` (`--out`), with LF line endings:

- `golden/<mode>.txt`, the manifest of each mode's jobs, and
  `golden/links.txt`, the manifest of the `links` jobs.
- `golden/<mode>/<source>.s`, the `-S` text of each program that
  compiled, named for its source without `.c`, so that
  `examples/sfx_tone.c` gives `golden/O/examples/sfx_tone.s`. A fuzz
  program's source is `golden/fuzz/<tool><seed>.c`, and its text is
  `golden/<mode>/fuzz/<tool><seed>.s`.
- `golden/fuzz/<tool><seed>.c`, the source of each fuzz program, which
  the generators of fuzz/ give and which the commands compile, since
  the generators stay in Python until step 9.
- `golden/blobs/<sha256>`, the bytes of each file over 120 bytes that a
  job writes before a command, such as the linker script of the modes
  `c` and `cO` and the damaged objects of the `links` jobs.

A manifest is text, one line per item:

    c6502-golden 1
    define <name> <JSON list>
    text <file name>
    job <label>
    mkdir <path>
    cd <path>
    write <path> text <JSON string>
    write <path> blob <sha256>
    run <JSON list, the command line>
    status <exit status, or "none" when the command raised>
    stdout <JSON string>
    stderr <JSON string>
    raised <JSON [type, message]>
    file <path> text <path under golden/>
    file <path> sha256 <hex digest> <size>
    end

The `define` lines at the top name lists of arguments that many
commands share, and a command line holds the name in place of the
list. The `text` lines name the files of the mode's jobs that are kept
as text (`MODES`). Each job begins with `job` and ends with `end`. `mkdir`, `cd`, `write`,
and `run` are its steps, in the order the job took them, and the
`status` line and the `stdout`, `stderr`, and `raised` lines that
aren't empty follow each `run`. The paths of `mkdir`, `cd`, `write`, and
`file` are relative to the job's directory, and `cd` names the
directory relative to that one (`.` for itself). After the last step
come the files the job left, sorted by path.

A command names each file of the repository by its path relative to
the repository's root, such as `examples/zork/main.c`, and the files
the job writes by their paths in the job's directory. The writer runs
it with the repository's files at `<root>/<path>` and replaces
`<root>/` with nothing in what the command writes and raises, so the
messages name the files as the command line does and the manifests are
the same on every machine. The checker does the same with its own root.
A command's argument is such a path if and only if it begins with one
of `ROOT_DIRS`.

The exit status is 0 when every job ran. A job that leaves the root's
path in a message in another form, which the replacement misses,
stops the writer with an error.
"""
from __future__ import annotations

import argparse
import contextlib
import hashlib
import importlib
import io
import json
import os
import shutil
import sys
import tempfile
import time
from concurrent.futures import ProcessPoolExecutor
from pathlib import Path

_ROOT = Path(__file__).resolve().parent.parent
if str(_ROOT) not in sys.path:
    sys.path.insert(0, str(_ROOT))
if str(_ROOT / "tools") not in sys.path:
    sys.path.insert(0, str(_ROOT / "tools"))

import compare_backends  # noqa: E402
from capture_passes import corpus  # noqa: E402

# Each mode of compare_backends.py's programs, with the names of the
# files its jobs leave that are kept as text, which a diff shows line by
# line. Every other file is kept as its hash. A mode added to
# compare_backends.py's `_program_commands` needs an entry here, and
# `c6502cpp check-golden` runs its commands as compile.py would.
MODES = {
    "O": ("out.s",),
    "unopt": ("out.s",),
    "cO": (),
    "c": (),
    "lex": (),
}

# The directories at the root whose files the commands name.
ROOT_DIRS = ("examples/", "tests/", "golden/")

# Files a job writes before a command that are this long or shorter
# are kept in the manifest, and longer ones in golden/blobs.
_INLINE_LIMIT = 120

_FORMAT = "c6502-golden 1"


def _defines() -> dict[str, list[str]]:
    """The lists of arguments that many commands share, by the name a
    command line holds in their place: the `--defsym`s of the
    simulator's helper addresses, which each link with a script in the
    modes `c` and `cO` gives."""
    from sim.linked import defsym_args
    return {"@sim_defsyms": list(defsym_args())}


def _shorten(argv: list[str]) -> list[str]:
    """`argv` with each run of arguments that `_defines` names replaced
    by its name."""
    for name, run in _defines().items():
        out, i = [], 0
        while i < len(argv):
            if argv[i:i + len(run)] == run:
                out.append(name)
                i += len(run)
            else:
                out.append(argv[i])
                i += 1
        argv = out
    return argv


def _root_forms() -> list[str]:
    """The spellings of the root that a command or a message can hold,
    with the separator after them, longest first."""
    forms = {_ROOT.as_posix() + "/", str(_ROOT) + os.sep}
    return sorted(forms, key=len, reverse=True)


def _normalize(text: str) -> str:
    """`text` with each spelling of `<root>/` replaced by nothing."""
    for form in _root_forms():
        text = text.replace(form, "")
    return text


def _command_args(argv: list[str]) -> tuple[list[str], list[str]]:
    """The command line as the manifest holds it and as it is run: each
    argument that names a file of the repository relative to the root,
    and with `<root>/` in front."""
    shown, real = [], []
    for a in argv:
        rel = a
        for form in _root_forms():
            if a.startswith(form):
                rel = a[len(form):].replace(os.sep, "/")
                break
        if rel != a and not rel.startswith(ROOT_DIRS):
            raise AssertionError(f"a command names {a}, outside {ROOT_DIRS}")
        shown.append(rel)
        real.append(_ROOT.as_posix() + "/" + rel if rel != a else a)
    return shown, real


def _compile(argv: list[str]) -> tuple:
    """Runs `compile.main(argv)` with the C++ back end off and returns
    the exit status (None when it raised), what it wrote to stdout and
    to stderr, and the type and message of the exception it raised, or
    None."""
    import compile as compile_module
    os.environ.pop("C6502_CPP_BACKEND", None)
    out, err = io.StringIO(), io.StringIO()
    status = None
    raised = None
    try:
        with contextlib.redirect_stdout(out), contextlib.redirect_stderr(err):
            status = compile_module.main(argv)
    except SystemExit as e:
        status = e.code
    except Exception as e:  # noqa: BLE001 - a crash is recorded too
        raised = (type(e).__name__, str(e))
    return status, out.getvalue(), err.getvalue(), raised


def _tree(work: Path) -> dict[str, bytes | None]:
    """Every file and directory under `work` by its path, with a file's
    bytes and None for a directory."""
    found = {
        p.relative_to(work).as_posix(): None if p.is_dir() else p.read_bytes()
        for p in work.rglob("*")
    }
    return dict(sorted(found.items()))


def _json(v) -> str:
    return json.dumps(v)


class _Recorder:
    """Runs a job's commands in the directory `work` and keeps the lines
    of its manifest. Before each command it adds a step for each
    directory and file the job made since the last command and for a
    change of the current directory, which compare_backends.py's
    functions make with `os.mkdir`, `Path.write_text`, and `os.chdir`."""

    def __init__(self, work: Path, blobs: dict[str, bytes]) -> None:
        self.work = work
        self.blobs = blobs
        self.lines: list[str] = []
        self.before = _tree(work)
        self.cwd = "."
        self.leftover: list[str] = []

    def _steps(self) -> None:
        now = _tree(self.work)
        for path, data in now.items():
            if path in self.before and self.before[path] == data:
                continue
            if data is None:
                self.lines.append(f"mkdir {path}")
            elif len(data) <= _INLINE_LIMIT and _is_text(data):
                self.lines.append(f"write {path} text {_json(data.decode('utf-8'))}")
            else:
                digest = hashlib.sha256(data).hexdigest()
                self.blobs[digest] = data
                self.lines.append(f"write {path} blob {digest}")
        gone = [p for p in self.before if p not in now]
        if gone:
            raise AssertionError(f"a job removed {gone} between commands")
        cwd = Path(os.path.relpath(Path.cwd(), self.work)).as_posix()
        if cwd != self.cwd:
            self.lines.append(f"cd {cwd}")
            self.cwd = cwd

    def run(self, argv: list[str]) -> bool:
        self._steps()
        shown, real = _command_args(argv)
        status, out, err, raised = _compile(real)
        out, err = _normalize(out), _normalize(err)
        if raised is not None:
            raised = (raised[0], _normalize(raised[1]))
        self.lines.append(f"run {_json(_shorten(shown))}")
        self.lines.append(f"status {'none' if status is None else status}")
        if out:
            self.lines.append(f"stdout {_json(out)}")
        if err:
            self.lines.append(f"stderr {_json(err)}")
        if raised is not None:
            self.lines.append(f"raised {_json(list(raised))}")
        self.leftover += [t for t in (out, err, *(raised or ())) if _mentions_root(t)]
        self.before = _tree(self.work)
        return status == 0 and raised is None


def _mentions_root(text: str) -> bool:
    """Whether `text` names the root or the temporary directory in a
    spelling `_normalize` left, as a path with the other separator."""
    lowered = text.lower().replace("\\", "/")
    root = _ROOT.as_posix().lower()
    temp = Path(tempfile.gettempdir()).as_posix().lower()
    return root in lowered or temp in lowered


def _is_text(data: bytes) -> bool:
    try:
        data.decode("utf-8")
    except UnicodeDecodeError:
        return False
    return b"\0" not in data and b"\r" not in data


def _text_path(mode: str, label: str, name: str) -> str:
    """The path under golden/ of the text output `name` of the program
    `label` in `mode`."""
    stem = label.removesuffix(".c").removeprefix("golden/")
    return f"{mode}/{stem}{Path(name).suffix}"


def _run_job(job) -> tuple[str, str, list[str], dict[str, bytes], dict[str, bytes], list[str]]:
    """Runs one job and returns its manifest's name, its label, its
    lines, the text outputs by their path under golden/, the blobs it
    wrote, and the messages that still named the root."""
    kind, label, args, mode = job
    blobs: dict[str, bytes] = {}
    texts: dict[str, bytes] = {}
    here = os.getcwd()
    with tempfile.TemporaryDirectory() as tmp:
        work = Path(tmp) / "work"
        work.mkdir()
        rec = _Recorder(work, blobs)
        os.chdir(work)
        try:
            if kind == "program":
                compare_backends._program_commands(rec.run, args, mode)
            elif kind == "game":
                compare_backends._game_commands(rec.run, label)
            else:
                compare_backends._error_commands(rec.run)
        finally:
            os.chdir(here)
        lines = [f"job {label if kind == 'program' else kind + ' ' + label}"]
        lines += rec.lines
        keep = MODES.get(mode, ())
        for path, data in _tree(work).items():
            if data is None:
                continue
            if kind == "program" and path in keep:
                where = _text_path(mode, label, path)
                texts[where] = data
                lines.append(f"file {path} text {where}")
            else:
                lines.append(
                    f"file {path} sha256 {hashlib.sha256(data).hexdigest()} {len(data)}")
        lines.append("end")
    manifest = mode if kind == "program" else "links"
    return manifest, label, lines, texts, blobs, rec.leftover


def _fuzz_sources(out: Path, seeds: range) -> dict[str, str]:
    """Writes the source of each fuzz program to out/fuzz and returns
    the path each `fuzz:<tool>:<seed>` argument stands for."""
    paths = {}
    (out / "fuzz").mkdir(parents=True, exist_ok=True)
    for tool in ("stmts", "calls", "exprs"):
        make = importlib.import_module(f"fuzz.{tool}").make_program
        for seed in seeds:
            src = make(seed)[0]
            path = out / "fuzz" / f"{tool}{seed}.c"
            path.write_bytes(src.encode("utf-8"))
            paths[f"fuzz:{tool}:{seed}"] = path.as_posix()
    return paths


def _init_worker() -> None:
    os.environ.pop("C6502_CPP_BACKEND", None)
    os.environ.pop("C6502_CHECK_IR", None)


def _parse_seeds(text: str) -> range:
    lo, _, hi = text.partition("-")
    return range(int(lo), int(hi or lo) + 1)


def main(argv: list[str] | None = None) -> int:
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("--out", default=str(_ROOT / "golden"))
    ap.add_argument("--modes", default=",".join(MODES),
                    help="the modes to write, of " + ", ".join(MODES))
    ap.add_argument("--no-links", action="store_true",
                    help="leave golden/links.txt as it is")
    ap.add_argument("--corpus", default="examples,games,chapters,fuzz",
                    help="the kinds of program, all of them unless a trial "
                         "run leaves some out")
    ap.add_argument("--fuzz-seeds", default="0-99")
    ap.add_argument("-j", type=int, default=os.cpu_count() or 1)
    ns = ap.parse_args(argv)
    out = Path(ns.out)
    if out.resolve() != (_ROOT / "golden").resolve():
        # The commands name the fuzz sources under golden/, which must
        # be inside the repository.
        ap.error("--out must be the repository's golden/ directory")
    modes = ns.modes.split(",")
    unknown = [m for m in modes if m not in MODES]
    if unknown:
        ap.error(f"unknown modes: {', '.join(unknown)}")
    for m in modes:
        shutil.rmtree(out / m, ignore_errors=True)
    seeds = _parse_seeds(ns.fuzz_seeds)
    fuzz = _fuzz_sources(out, seeds)
    jobs = []
    if not ns.no_links:
        import capture_binutils
        jobs += [("game", g, [], "") for g in capture_binutils._GAMES]
        jobs.append(("errors", "damaged files and bad links", [], ""))
    for label, args in corpus(ns.corpus.split(","), seeds):
        if args[0] in fuzz:
            args = [fuzz[args[0]]] + args[1:]
            label = Path(args[0]).relative_to(_ROOT).as_posix()
        for mode in modes:
            jobs.append(("program", label, args, mode))
    start = time.time()
    manifests: dict[str, list[str]] = {m: [] for m in modes}
    if not ns.no_links:
        manifests["links"] = []
    blobs: dict[str, bytes] = {}
    leftover: list[str] = []
    with ProcessPoolExecutor(max_workers=ns.j, initializer=_init_worker) as pool:
        for manifest, _, lines, texts, job_blobs, left in pool.map(
                _run_job, jobs, chunksize=4):
            manifests[manifest] += lines
            blobs.update(job_blobs)
            leftover += left
            for where, data in texts.items():
                path = out / where
                path.parent.mkdir(parents=True, exist_ok=True)
                path.write_bytes(data)
    for name, lines in manifests.items():
        defines = [f"define {k} {_json(v)}" for k, v in _defines().items()]
        kept = [f"text {t}" for t in MODES.get(name, ())]
        text = "\n".join([_FORMAT, *defines, *kept, *lines]) + "\n"
        (out / f"{name}.txt").write_bytes(text.encode("utf-8"))
    (out / "blobs").mkdir(exist_ok=True)
    for digest, data in blobs.items():
        (out / "blobs" / digest).write_bytes(data)
    print(f"{len(jobs)} jobs written in {time.time() - start:.0f} s")
    for text in leftover[:10]:
        print(f"a message names the root in another form: {text[:300]!r}")
    return 1 if leftover else 0


if __name__ == "__main__":
    raise SystemExit(main())
