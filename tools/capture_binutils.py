"""Records what the Python assembler, linker, nm.py, and objdump.py are
given and what they return, over a corpus and during the Python tests,
for checking the C++ port of step 7 (docs/cpp_port.md).

    # The corpus: objects, links, and listings.
    C6502_CPP_BACKEND=1 uv run python tools/capture_binutils.py -j 12

    # The Python tests' calls, as a pytest plugin.
    C6502_BINUTILS_CAPTURES=captures/binutils PYTHONPATH=tools \\
        uv run pytest -p capture_binutils -n 12 -q

    cpp/build/bin/c6502cpp check-binutils captures/binutils

The harness wraps `binutils.assembler.assemble`, `binutils.elf.read`,
`passes.link_script.parse_script`, `passes.linker.link_objects`,
`binutils.disasm.disassemble`, and the `main` functions of nm.py and
objdump.py, so each call is recorded with its arguments and its result
(or the exception it raised) and the program goes on as without the
harness. A compile under `C6502_CPP_BACKEND=1` gives the same `-S`
text (tools/compare_backends.py), and the assembler and the linker stay
in Python, so the corpus can be compiled by the C++ port, which is
faster.

The records have the form of tools/capture_passes.py's, one call to a
file, `<out>/<kind>/<sha1>.txt`:

    c6502-capture 2
    pass <kind>
    source <the compile, link, or test that made the call>
    args <JSON>
    context {}
    input-json
    <JSON>
    output-json
    <JSON>

A call that raised has an `output-error` section in place of its output,
`{"type": <the exception's class>, "message": <str(e)>}`. Every file a
call reads or writes, an object, an executable, or a damaged file, is
in `<out>/blobs/<sha1>`, named by the SHA-1 of its bytes, and the
records name it by that hash. The kinds:

- `assemble`: the arguments `filename`, `link_symbols` (a list of
  `[name, address]` pairs in the dict's order, `null` for an address
  outside zero page, or `null` for no dict), and `keep_source`; the
  text as the input; the object as the result, `{"object": <hash>}`.
- `elf_read`: the argument `name`, the bytes as the input
  (`{"data": <hash>}`), and as the result the bytes `elf.write` gives
  for what `read` returned, `{"rewritten": <hash>}`, which are the
  input's own bytes for every file c6502 writes.
- `parse_script`: the script's text as the input and the tagged JSON
  of the `Script` (tools/capture_passes.py's `encode`) as the result.
- `link`: the arguments `origin` and `defsyms` (pairs), the objects as
  `{"objects": [[name, hash], ...], "script": <its text or null>}`, and
  the executable as the result, `{"executable": <hash>}`. A link whose
  script didn't come from `parse_script` isn't recorded.
- `disassemble`: the arguments `all_sections`, `only` (sorted, or
  null), `raw`, and `addresses`, the file as `{"elf": <hash>}`, and the
  listing's lines as the result.
- `nm` and `objdump`: the command line after the program's name as the
  arguments, with `columns`, the terminal width argparse wraps its
  usage and help to (`shutil.get_terminal_size`), and the files the
  command line names that exist, `{"files": [[path, hash], ...]}`, as
  the input. The result is `{"stdout": ..., "stderr": ..., "status":
  <exit status>}`.

The corpus is tools/capture_passes.py's (`--corpus`), compiled with `-c`
and `-c -O` (`--modes`). Each object is listed with several sets of nm
and objdump options, linked alone without a script and with
`_SCRIPT`, a script with ROM and RAM regions, `AT>`, and `NOLOAD`
(given the simulator's helper addresses, so that most programs link),
and each executable is listed too. The `links` corpus links the three
games with their scripts as their tests do, and without them, and
reads and lists a set of damaged files and bad command lines for the
errors. Each job reports the time the Python functions took, by kind.
"""
from __future__ import annotations

import argparse
import contextlib
import hashlib
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

KINDS = ("assemble", "elf_read", "parse_script", "link", "disassemble",
         "nm", "objdump")

_OUT = _ROOT / "captures" / "binutils"
_SOURCE = ""
# The number of calls of each kind, and the seconds they took.
_COUNTS: dict[str, int] = {}
_SECONDS: dict[str, float] = {}
# Calls whose result differed when Python did it again from the bytes
# of its inputs (`link` from the objects read back, and `elf_read`
# whose rewrite isn't the input), by kind, with one example.
_ANOMALIES: dict[str, list] = {}
# True while the harness itself calls a wrapped function, whose calls
# aren't recorded.
_QUIET = False
# The attribute that holds the text of each script `parse_script`
# returned.
_TEXT = "_capture_text"

# The script the corpus links each object with: code and constants in
# ROM, data copied there from ROM by the startup code, and `.bss`
# cleared by it.
_SCRIPT = """\
MEMORY
{
    ram (rwx) : ORIGIN = 0x0800, LENGTH = 0x7800
    rom (rx)  : ORIGIN = 0xC000, LENGTH = 0x2000
}

SECTIONS
{
    .text : { *(.text .text.*) } > rom
    .rodata : { *(.rodata .rodata.*) } > rom
    .data : { *(.data .data.*) } > ram AT> rom
    .bss (NOLOAD) : { *(.bss .bss.*) } > ram
    __end = .;
}
"""

_NM_OPTIONS = (
    [], ["-a"], ["-g"], ["-u"], ["--defined-only", "-S"], ["-n"],
    ["-p", "-r"], ["-A", "-a", "-n", "-S"],
)
_OBJDUMP_OPTIONS = (
    ["-x"], ["-d"], ["-D"], ["-s"],
    ["-d", "--no-show-raw-insn", "--no-addresses"],
    ["-D", "-j", ".text", "-r", "-t"], ["-h", "-f", "--link-metadata"],
)
# Command lines that argparse or the tools reject, with `{f}` for a
# good file and `{bad}` for each damaged one.
_BAD_COMMANDS = (
    ["nm.py"], ["nm.py", "-z", "{f}"], ["nm.py", "--de", "{f}"],
    ["nm.py", "--debug-syms=1", "{f}"], ["nm.py", "-ag", "{f}"],
    ["nm.py", "-ax", "{f}"], ["nm.py", "{f}", "-a", "{f}"],
    ["nm.py", "-h"], ["nm.py", "--help", "{f}"], ["nm.py", "--", "{f}"],
    ["nm.py", "-gun", "{f}", "{f}"], ["nm.py", "missing.o"],
    ["nm.py", "{f}", "missing.o", "{bad}"], ["nm.py", "-A", "{bad}", "{f}"],
    ["nm.py", "-", "{f}"], ["nm.py", "--print", "{f}"],
    ["objdump.py"], ["objdump.py", "{f}"], ["objdump.py", "-q", "{f}"],
    ["objdump.py", "--help"], ["objdump.py", "-x", "missing.o", "{bad}"],
    ["objdump.py", "-j"], ["objdump.py", "-dj", ".text", "{f}"],
)


def _blob(data: bytes) -> str:
    """Stores `data` under its SHA-1 and returns the hash."""
    key = hashlib.sha1(data).hexdigest()
    path = _OUT / "blobs" / key
    if not path.exists():
        path.parent.mkdir(parents=True, exist_ok=True)
        tmp = path.with_name(f"{key}.{os.getpid()}.tmp")
        tmp.write_bytes(data)
        try:
            os.replace(tmp, path)
        except PermissionError:
            if not path.exists():
                raise
            tmp.unlink()
    return key


def _record(kind: str, args, input_value, result=None,
            error: BaseException | None = None) -> None:
    """Writes the record of a call, unless an identical one is there."""
    args_json = json.dumps(args)
    input_json = json.dumps(input_value)
    key = hashlib.sha1(
        f"{kind}\n{args_json}\n{input_json}".encode("utf-8")).hexdigest()
    path = _OUT / kind / f"{key}.txt"
    if path.exists():
        return
    if error is not None:
        output = "output-error\n" + json.dumps(
            {"type": type(error).__name__, "message": str(error)}) + "\n"
    else:
        output = "output-json\n" + json.dumps(result) + "\n"
    text = (f"c6502-capture 2\npass {kind}\nsource {_SOURCE}\n"
            f"args {args_json}\ncontext {{}}\ninput-json\n{input_json}\n"
            f"{output}")
    path.parent.mkdir(parents=True, exist_ok=True)
    tmp = path.with_name(f"{key}.{os.getpid()}.tmp")
    tmp.write_bytes(text.encode("utf-8"))
    try:
        os.replace(tmp, path)
    except PermissionError:
        if not path.exists():
            raise
        tmp.unlink()


def _count(kind: str, seconds: float) -> None:
    _COUNTS[kind] = _COUNTS.get(kind, 0) + 1
    _SECONDS[kind] = _SECONDS.get(kind, 0.0) + seconds


def _anomaly(kind: str, what) -> None:
    _ANOMALIES.setdefault(kind, []).append(what)


@contextlib.contextmanager
def _quiet():
    global _QUIET
    was, _QUIET = _QUIET, True
    try:
        yield
    finally:
        _QUIET = was


# --- The wrappers ----------------------------------------------------------


def _wrap_assemble(real):
    def assemble(text, filename="<input>", link_symbols=None,
                 keep_source=True):
        if _QUIET:
            return real(text, filename, link_symbols, keep_source)
        from binutils import elf as E
        args = {
            "filename": filename,
            "link_symbols": None if link_symbols is None
            else [[k, v] for k, v in link_symbols.items()],
            "keep_source": keep_source,
        }
        start = time.perf_counter()
        try:
            obj = real(text, filename, link_symbols, keep_source)
        except Exception as e:
            _count("assemble", time.perf_counter() - start)
            _record("assemble", args, text, error=e)
            raise
        _count("assemble", time.perf_counter() - start)
        with _quiet():
            data = E.write(obj)
        _record("assemble", args, text, {"object": _blob(data)})
        return obj
    return assemble


def _wrap_read(real):
    def read(data, name="file"):
        if _QUIET:
            return real(data, name)
        from binutils import elf as E
        start = time.perf_counter()
        try:
            elf = real(data, name)
        except Exception as e:
            _count("elf_read", time.perf_counter() - start)
            _record("elf_read", {"name": name}, {"data": _blob(bytes(data))},
                    error=e)
            raise
        _count("elf_read", time.perf_counter() - start)
        with _quiet():
            back = E.write(real(data, name))
        if back != bytes(data):
            _anomaly("elf_read", name)
        _record("elf_read", {"name": name}, {"data": _blob(bytes(data))},
                {"rewritten": _blob(back)})
        return elf
    return read


def _wrap_parse_script(real):
    def parse_script(text):
        if _QUIET:
            return real(text)
        from capture_passes import encode
        start = time.perf_counter()
        try:
            script = real(text)
        except Exception as e:
            _count("parse_script", time.perf_counter() - start)
            _record("parse_script", {}, text, error=e)
            raise
        _count("parse_script", time.perf_counter() - start)
        setattr(script, _TEXT, text)
        _record("parse_script", {}, text, encode(script))
        return script
    return parse_script


def _wrap_link(real):
    def link_objects(inputs, script=None, *, origin=0x0800, defsyms=None):
        if _QUIET:
            return real(inputs, script, origin=origin, defsyms=defsyms)
        from binutils import elf as E
        from passes.link_script import parse_script
        script_text = None if script is None else getattr(script, _TEXT, None)
        record = script is None or script_text is not None
        if record:
            with _quiet():
                try:
                    datas = [(name, E.write(obj)) for name, obj in inputs]
                except Exception:  # noqa: BLE001 - an object E can't write
                    record = False
        start = time.perf_counter()
        try:
            exe = real(inputs, script, origin=origin, defsyms=defsyms)
        except Exception as e:
            _count("link", time.perf_counter() - start)
            if record:
                _record("link", _link_args(origin, defsyms),
                        _link_input(datas, script_text), error=e)
            raise
        _count("link", time.perf_counter() - start)
        if not record:
            _anomaly("link-unrecorded", _SOURCE)
            return exe
        with _quiet():
            out = E.write(exe)
            # The C++ linker reads the objects' bytes, so the link must
            # give the same executable from the objects read back.
            again = E.write(real(
                [(name, E.read(data, name)) for name, data in datas],
                None if script_text is None else parse_script(script_text),
                origin=origin, defsyms=dict(defsyms or {}),
            ))
        if again != out:
            _anomaly("link", _SOURCE)
        _record("link", _link_args(origin, defsyms),
                _link_input(datas, script_text), {"executable": _blob(out)})
        return exe
    return link_objects


def _link_args(origin, defsyms) -> dict:
    return {"origin": origin,
            "defsyms": [[k, v] for k, v in (defsyms or {}).items()]}


def _link_input(datas, script_text) -> dict:
    return {"objects": [[name, _blob(data)] for name, data in datas],
            "script": script_text}


def _wrap_disassemble(real):
    def disassemble(elf, *, all_sections=False, only=None, options=None):
        if _QUIET:
            return real(elf, all_sections=all_sections, only=only,
                        options=options)
        from binutils import elf as E
        from binutils.disasm import Options
        opt = options or Options()
        with _quiet():
            data = E.write(elf)
        args = {"all_sections": all_sections,
                "only": None if only is None else sorted(only),
                "raw": opt.raw, "addresses": opt.addresses}
        start = time.perf_counter()
        lines = real(elf, all_sections=all_sections, only=only,
                     options=options)
        _count("disassemble", time.perf_counter() - start)
        _record("disassemble", args, {"elf": _blob(data)}, lines)
        return lines
    return disassemble


def _wrap_main(kind: str, real):
    def main(argv):
        if _QUIET:
            return real(argv)
        files = []
        for a in argv[1:]:
            p = Path(a)
            if a != "-" and p.is_file() and a not in (f for f, _ in files):
                files.append((a, _blob(p.read_bytes())))
        out, err = io.StringIO(), io.StringIO()
        start = time.perf_counter()
        exit_ = None
        # The record is of the Python tool, which C6502_CPP_BACKEND=1
        # would hand to the C++ port.
        switch = os.environ.pop("C6502_CPP_BACKEND", None)
        with contextlib.redirect_stdout(out), contextlib.redirect_stderr(err):
            try:
                status = real(argv)
            except SystemExit as e:
                exit_ = e
                status = e.code if isinstance(e.code, int) else (
                    0 if e.code is None else 1)
            finally:
                if switch is not None:
                    os.environ["C6502_CPP_BACKEND"] = switch
        _count(kind, time.perf_counter() - start)
        _record(kind, {"argv": list(argv[1:]),
                       "columns": shutil.get_terminal_size().columns},
                {"files": [list(f) for f in files]},
                {"stdout": out.getvalue(), "stderr": err.getvalue(),
                 "status": status})
        sys.stdout.write(out.getvalue())
        sys.stderr.write(err.getvalue())
        if exit_ is not None:
            raise exit_
        return status
    return main


def install(out: Path) -> None:
    """Wraps the functions the harness records, writing under `out`."""
    global _OUT
    _OUT = out
    from binutils import assembler, disasm, elf
    import nm
    import objdump
    from passes import link_script, linker
    assembler.assemble = _wrap_assemble(assembler.assemble)
    elf.read = _wrap_read(elf.read)
    link_script.parse_script = _wrap_parse_script(link_script.parse_script)
    linker.link_objects = _wrap_link(linker.link_objects)
    disasm.disassemble = _wrap_disassemble(disasm.disassemble)
    # objdump.py imported `disassemble` by name.
    objdump.disassemble = disasm.disassemble
    nm.main = _wrap_main("nm", nm.main)
    objdump.main = _wrap_main("objdump", objdump.main)


# --- The corpus ------------------------------------------------------------


def _compile(argv: list[str]) -> bool:
    import compile as compile_module
    sink = io.StringIO()
    try:
        with contextlib.redirect_stderr(sink), contextlib.redirect_stdout(sink):
            return compile_module.main(argv) == 0
    except SystemExit:
        return False
    except Exception:  # noqa: BLE001 - a crash is a failed compile here
        return False


def _list(path: str) -> None:
    """Runs nm and objdump on `path` with each set of options."""
    import nm
    import objdump
    sink = io.StringIO()
    with contextlib.redirect_stderr(sink), contextlib.redirect_stdout(sink):
        for opts in _NM_OPTIONS:
            nm.main(["nm.py", *opts, path])
        for opts in _OBJDUMP_OPTIONS:
            objdump.main(["objdump.py", *opts, path])


def _source_args(args: list[str], tmp: str) -> list[str]:
    """The compile.py arguments of a corpus program, with a fuzz
    program written to a file."""
    import importlib
    if args[0].startswith("fuzz:"):
        _, tool, seed = args[0].split(":")
        src = importlib.import_module(f"fuzz.{tool}").make_program(int(seed))[0]
        path = os.path.join(tmp, f"{tool}{seed}.c")
        with open(path, "w", encoding="utf-8", newline="\n") as f:
            f.write(src)
        return [path] + args[1:]
    return args


def _extern_defsyms(path: str) -> list[str]:
    """`--defsym` arguments that give an address to each name the
    object at `path` leaves to another unit, so that an example, which
    names things the rest of its game defines, links alone. The names
    the linker places and the runtime's helpers keep their sources."""
    from binutils import elf as E
    from binutils.assembler import is_link_placed
    from passes.runtime_library import helper_names
    with _quiet():
        obj = E.read_file(path)
    from passes.zp_link_metadata import parse_metadata
    text = obj.link_text()
    zp = frozenset(z.name for z in parse_metadata(text).zp_objects) \
        if text else frozenset()
    names = sorted({
        s.name for s in obj.symbols
        if not s.defined and not is_link_placed(s.name, zp)
        and s.name not in helper_names()
    })
    return [f"--defsym={n}=0x{0x9000 + 2 * k:04X}" for k, n in enumerate(names)]


def _init_worker(out: str) -> None:
    install(Path(out))


def _job_result(ok: bool):
    return (_SOURCE, ok, dict(_COUNTS), dict(_SECONDS),
            {k: list(v) for k, v in _ANOMALIES.items()})


def _reset(source: str) -> None:
    global _SOURCE
    _SOURCE = source
    _COUNTS.clear()
    _SECONDS.clear()
    _ANOMALIES.clear()


def _run_program(job):
    """Compiles one program to an object, lists it, links it alone with
    and without `_SCRIPT`, and lists the executables."""
    global _SOURCE
    label, args, mode = job
    flag = ["-O"] if mode == "O" else []
    _reset(f"{label} -c{' -O' if flag else ''}")
    from sim.linked import defsym_args
    here = os.getcwd()
    with tempfile.TemporaryDirectory() as tmp:
        os.chdir(tmp)
        try:
            argv = _source_args(args, tmp)
            ok = _compile(["compile.py", *argv, "-c", *flag, "-o", "unit.o"])
            if ok:
                _list("unit.o")
                Path("script.ld").write_text(_SCRIPT, encoding="utf-8")
                scripted = ["-T", "script.ld", *defsym_args(),
                            *_extern_defsyms("unit.o")]
                for extra, exe in (([], "prog"), (scripted, "prog_t")):
                    _SOURCE = f"{label} {mode} link {' '.join(extra[:2])}"
                    if _compile(["compile.py", "unit.o", *extra, "-o", exe]):
                        _list(exe)
        finally:
            os.chdir(here)
    return _job_result(ok)


_GAMES = {
    "zork": ("interpreter.ld",
             ["main", "decode", "objects", "numbers", "input", "pages",
              "zstring", "output", "disk", "prompts", "save"]),
    "lode_runner": ("lode_runner.ld",
                    ["main", "level", "player", "input", "guards", "timers",
                     "screen", "hiscore", "editor", "sprites", "sound",
                     "wipe"]),
    "ali_baba": ("ali_baba.ld",
                 ["main", "init", "turn", "input", "combat", "ai", "events",
                  "entities", "map", "display", "util", "text", "sound",
                  "hrcg"]),
}


def _run_game(game: str):
    """Compiles a game's units, links them as its tests do, with its
    script (and for Ali Baba also with the simulator's helpers), and
    without a script, and lists the objects and the executables."""
    global _SOURCE
    _reset(f"examples/{game} link")
    from sim.linked import defsym_args
    script, units = _GAMES[game]
    d = _ROOT / "examples" / game
    here = os.getcwd()
    ok = True
    with tempfile.TemporaryDirectory() as tmp:
        os.chdir(tmp)
        try:
            objects = []
            for u in units:
                if _compile(["compile.py", str(d / f"{u}.c"), "-c", "-O",
                             "-o", f"{u}.o"]):
                    objects.append(f"{u}.o")
                    _list(f"{u}.o")
                else:
                    ok = False
            links = [(["-T", str(d / script)], game),
                     ([], f"{game}_default")]
            if game == "ali_baba":
                links.append((["-T", str(d / script), *defsym_args()],
                              f"{game}_sim"))
            for extra, exe in links:
                _SOURCE = f"examples/{game} link {' '.join(extra[:1])}"
                if _compile(["compile.py", *extra, *objects, "-o", exe]):
                    _list(exe)
                elif extra:
                    # Without a script a game may not fit, which is a
                    # recorded error and not a failure of the job.
                    ok = False
        finally:
            os.chdir(here)
    return _job_result(ok)


def _damaged(data: bytes) -> dict[str, bytes]:
    """Damaged copies of the object `data`."""
    shoff = int.from_bytes(data[32:36], "little")
    out = {
        "short.o": data[:20],
        "half.o": data[:len(data) // 2],
        "cut.o": data[:-1],
        "notelf.o": b"not an ELF file, but long enough to be one" * 2,
        "class64.o": data[:4] + b"\x02" + data[5:],
        "machine.o": data[:18] + (62).to_bytes(2, "little") + data[20:],
        "shstrndx.o": data[:50] + (0x7FFF).to_bytes(2, "little") + data[52:],
        "secsize.o": data[:shoff + 60] + (0xFFFFFF).to_bytes(4, "little")
        + data[shoff + 64:],
    }
    at = data.find(b"; @zp-link-meta-begin")
    if at >= 0:
        out["meta.o"] = data[:at] + b"\xff" + data[at + 1:]
    return out


def _run_errors(_unused: str):
    """Reads, lists, and links damaged files, and runs nm and objdump
    with bad command lines."""
    _reset("damaged files and bad command lines")
    import nm
    import objdump
    here = os.getcwd()
    with tempfile.TemporaryDirectory() as tmp:
        os.chdir(tmp)
        try:
            src = _ROOT / "examples" / "apply_bobble.c"
            ok = _compile(["compile.py", str(src), "-c", "-O", "-o", "good.o"])
            data = Path("good.o").read_bytes()
            bad = _damaged(data)
            for name, b in bad.items():
                Path(name).write_bytes(b)
            sink = io.StringIO()
            with contextlib.redirect_stderr(sink), \
                    contextlib.redirect_stdout(sink):
                for name in bad:
                    nm.main(["nm.py", name])
                    objdump.main(["objdump.py", "-x", name])
                    _compile(["compile.py", name, "-o", "prog"])
                for cmd in _BAD_COMMANDS:
                    for name in bad:
                        argv = [a.replace("{f}", "good.o").replace(
                            "{bad}", name) for a in cmd]
                        main = nm.main if cmd[0] == "nm.py" else objdump.main
                        try:
                            main(argv)
                        except SystemExit:
                            pass
                        if "{bad}" not in cmd:
                            break
        finally:
            os.chdir(here)
    return _job_result(ok)


def _run(job):
    kind = job[0]
    if kind == "program":
        return _run_program(job[1:])
    if kind == "game":
        return _run_game(job[1])
    return _run_errors(job[1])


def main(argv: list[str] | None = None) -> int:
    import capture_passes
    ap = argparse.ArgumentParser(
        description="Record the assembler's, the linker's, nm's, and "
                    "objdump's calls over the corpus.")
    ap.add_argument("--out", default=str(_ROOT / "captures" / "binutils"))
    ap.add_argument("--corpus", default="examples,games,chapters,fuzz,links")
    ap.add_argument("--fuzz-seeds", default="0-99")
    ap.add_argument("--modes", default="O,unopt",
                    help="O for -c -O, unopt for -c")
    ap.add_argument("-j", type=int, default=os.cpu_count() or 1)
    ns = ap.parse_args(argv)
    kinds = ns.corpus.split(",")
    jobs = []
    if "links" in kinds:
        jobs += [("game", g) for g in _GAMES] + [("errors", "")]
    lo, _, hi = ns.fuzz_seeds.partition("-")
    seeds = range(int(lo), int(hi or lo) + 1)
    for label, args in capture_passes.corpus(
            [k for k in kinds if k != "links"], seeds):
        for mode in ns.modes.split(","):
            jobs.append(("program", label, args, mode))
    out = Path(ns.out)
    out.mkdir(parents=True, exist_ok=True)
    start = time.time()
    counts: dict[str, int] = {}
    seconds: dict[str, float] = {}
    anomalies: dict[str, list] = {}
    failed = 0
    with ProcessPoolExecutor(max_workers=ns.j, initializer=_init_worker,
                             initargs=(str(out),)) as pool:
        for label, ok, c, s, a in pool.map(_run, jobs, chunksize=2):
            failed += not ok
            if not ok and not label.startswith("tests/chapter_"):
                print(f"failed: {label}")
            for k, n in c.items():
                counts[k] = counts.get(k, 0) + n
            for k, t in s.items():
                seconds[k] = seconds.get(k, 0.0) + t
            for k, v in a.items():
                anomalies.setdefault(k, []).extend(v)
    print(f"{len(jobs)} jobs ({failed} failed) in {time.time() - start:.0f} s")
    for k in KINDS:
        stored = len(list((out / k).glob("*.txt"))) if (out / k).exists() else 0
        print(f"{k}: {counts.get(k, 0)} calls in {seconds.get(k, 0.0):.1f} s, "
              f"{stored} distinct stored")
    blobs = list((out / "blobs").glob("*")) if (out / "blobs").exists() else []
    print(f"blobs: {len(blobs)}, {sum(p.stat().st_size for p in blobs)} bytes")
    for k, v in anomalies.items():
        print(f"{k}: {len(v)} anomalies, such as {v[0]}")
    return 1 if anomalies else 0


# --- As a pytest plugin ----------------------------------------------------

if os.environ.get("C6502_BINUTILS_CAPTURES") and "pytest" in sys.modules:
    install(Path(os.environ["C6502_BINUTILS_CAPTURES"]))
    _SOURCE = "the Python tests"

    def pytest_runtest_setup(item):
        global _SOURCE
        _SOURCE = item.nodeid

    def pytest_sessionfinish(session):
        path = _OUT / "timing" / f"{os.getpid()}.json"
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_text(json.dumps({"counts": _COUNTS, "seconds": _SECONDS,
                                    "anomalies": _ANOMALIES}))


if __name__ == "__main__":
    raise SystemExit(main())
