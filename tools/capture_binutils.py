"""Records what the Python assembler, linker, nm.py, and objdump.py are
given and what they return, over a corpus and during the Python tests,
for checking the C++ port of step 7 (docs/cpp_port.md).

    # The corpus: objects, links, and listings.
    C6502_CPP_BACKEND=1 uv run python tools/capture_binutils.py -j 12

    # The Python tests' calls, as a pytest plugin.
    C6502_BINUTILS_CAPTURES=captures/binutils PYTHONPATH=tools \\
        uv run pytest -p capture_binutils -n 12 -q

    cpp/build/bin/c6502cpp check-binutils captures/binutils

The harness wraps `binutils.assembler.assemble`,
`passes.zp_link_metadata.parse_metadata`, `binutils.elf.read`,
`passes.link_script.parse_script`, `passes.linker.link_objects`,
`passes.runtime_library.needed`, the layout's `lay_out`, `default_script`, `fall_through`, `tail_jump`, and
`startup_lines`, `binutils.disasm.disassemble`, and the `main`
functions of nm.py and
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
- `parse_metadata`: no arguments, the text as the input, and as the
  result the tagged JSON of the `LinkMetadata`
  (tools/capture_passes.py's `encode`) with the lines `format_metadata`
  writes of it, `{"meta": ..., "formatted": [...]}`. The assembler and
  the linker call it on the metadata block alone, and the tests on
  whole texts too.
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
- `runtime_library`: a call of `passes.runtime_library.needed` that a
  link makes, with the argument `defined` (sorted), the objects as
  `{"objects": [[name, hash], ...]}`, and as the result `{"names":
  [...], "text": ...}`, the helpers it returned and the text
  `library_text` gives for them (null for none), which the library's
  object is assembled from.
- `lay_out`, `default_script`, `fall_through`, `tail_jump`, and
  `startup_lines` (passes/link_layout.py), whose inputs only a link
  builds, so the records hold what each reads. A unit is `{"path",
  "index", "items": [[name, section, local, zero, merge], ...],
  "sections": [[size, flags], ...], "item_sections": [the index in
  "sections" of each item's input section], "tails": [[name, target],
  ...]}`, with each input section listed once per unit. `lay_out`'s
  input is `{"script": <tagged JSON>, "units": [...], "lookups":
  [[name, value or null], ...]}`, the names the layout asked the
  linker's lookup for and its answers, and its result is `{"outputs":
  [{"name", "addr", "load_addr", "size", "noload", "executable",
  "parts": [[unit, [position of the unit in the units, index of the
  section], addr, size], ...]}, ...], "symbols": [[name, value],
  ...]}`. `default_script` takes the argument `origin` and the units,
  and gives the tagged JSON of its `Script`. `fall_through` takes the
  argument `reorder` and `{"entries": [[name, function, tail], ...]}`,
  and gives `[[index of the entry, dropped], ...]`. `tail_jump` takes
  the section's `type`, `size`, `data` (hex), and `relocs` (`[[offset,
  type, addend, symbol name], ...]`), and the `symbols` that the
  section defines (`[[name, value, type], ...]`), and gives the name or
  null. `startup_lines` takes the tagged JSON of the script and gives
  the lines.
- `global_definitions`, `relax`, and `executable` (binutils/ld.py), and
  `linker_runtime_library`, `object_items`, `item_symbol`,
  `without_init`, `unit`, `apply_renames`, `pointer_names`,
  `reassemble`, `output_symbols`, `merge_identical`, and `contents`
  (passes/linker.py's `_runtime_library` and the others of those
  names). A link changes its objects in place between these calls, so
  a record names an object by the hash of the blob of its JSON form,
  `{"type", "entry", "flags", "nsections", "sections", "nsymbols",
  "symbols", "segments"}`. A section is `[name, type, flags, data (hex),
  nobits_size, addr, align, [[offset, type, symbol, addend], ...]]`, a
  symbol `[name, value, size, bind, type, section, other]`, and a
  segment `[vaddr, paddr, data (hex), memsz, flags]`. A symbol's
  section is its index in "sections", or `-1 - n` for the section number
  n (`SHN_UNDEF` or `SHN_ABS`), and a relocation's symbol its index in
  "symbols". The first "nsections" sections and "nsymbols" symbols are
  the object's lists, and the rest are those a symbol or a relocation
  names that the lists don't hold. A list of objects is `[[name,
  hash], ...]`, a section of one `[object, section]`, and a list of
  items `[[name, section, local, zero, merge], ...]`.
  `global_definitions` takes the objects and gives `{"defs": [[name,
  object, symbol], ...], "absolute": [[name, value], ...]}`. `relax`
  takes the section in the form above, with its relocations' symbols
  as indices into "symbols", which are `[name, value, size, bind, type,
  in the section, other]` for the object's symbols ("nsymbols" of them)
  and then the others the relocations name, and "values", what
  `value_of` gives each relocation's symbol. It gives `{"saved", "data",
  "relocs", "own"}`, the section's bytes and relocations after the call
  and `[value, size]` of each of its own symbols. `executable` takes the
  arguments `entry` and `link_text` and the objects, the output
  sections in `lay_out`'s form, the resolver's "placed" sections
  (`[[section, addr], ...]`) and "extra" names, and the symbols, whose
  section is `["output", i]`, `["section", k, j]`, or `["shndx", n]`,
  and gives `{"executable": <hash of its bytes>}`.
  `linker_runtime_library` takes the argument `defsyms` and the objects,
  and gives null or `{"name", "object" (the hash of its bytes), "meta"
  (tagged JSON)}`. `object_items` takes the argument `path`, the
  metadata's items, and the object, and gives the items.
  `item_symbol` gives the symbol's index, `without_init` `{"same",
  "elf"}`, `unit` `{"path", "index", "items", "sections": [[name,
  section index], ...], "tails"}`, `apply_renames` the symbols' names
  after it, `pointer_names` the names sorted, `reassemble` and
  `merge_identical` the objects after them (and the metadatas' items),
  `output_symbols` the symbols in `executable`'s form, and `contents`
  null or `[name, type, flags, align, size, data, [[offset, type,
  target, addend], ...]]`, with a local target as `[unit, name]`.
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

KINDS = ("assemble", "parse_metadata", "elf_read", "parse_script", "link",
         "disassemble", "nm", "objdump", "runtime_library", "lay_out",
         "default_script", "fall_through", "tail_jump", "startup_lines",
         "global_definitions", "relax", "executable", "linker_runtime_library",
         "object_items", "item_symbol", "without_init", "unit",
         "apply_renames", "pointer_names", "reassemble", "output_symbols",
         "merge_identical", "contents")

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


def _wrap_parse_metadata(real):
    def parse_metadata(asm_text):
        if _QUIET:
            return real(asm_text)
        from capture_passes import encode
        from passes.zp_link_metadata import format_metadata
        start = time.perf_counter()
        try:
            meta = real(asm_text)
        except Exception as e:
            _count("parse_metadata", time.perf_counter() - start)
            _record("parse_metadata", {}, asm_text, error=e)
            raise
        _count("parse_metadata", time.perf_counter() - start)
        _record("parse_metadata", {}, asm_text,
                {"meta": encode(meta), "formatted": format_metadata(meta)})
        return meta
    return parse_metadata


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


def _wrap_needed(real):
    def needed(objects, defined):
        if _QUIET:
            return real(objects, defined)
        from binutils import elf as E
        from passes.runtime_library import library_text
        with _quiet():
            try:
                datas = [(name, E.write(obj)) for name, obj in objects]
            except Exception:  # noqa: BLE001 - an object E can't write
                datas = None
        start = time.perf_counter()
        names = real(objects, defined)
        _count("runtime_library", time.perf_counter() - start)
        if datas is None:
            _anomaly("runtime_library-unrecorded", _SOURCE)
            return names
        with _quiet():
            text = library_text(names) if names else None
        _record("runtime_library", {"defined": sorted(defined)},
                {"objects": [[name, _blob(data)] for name, data in datas]},
                {"names": names, "text": text})
        return names
    return needed
# --- The layout's functions (passes/link_layout.py) ------------------------
#
# Their inputs are the linker's `Unit`s and sections, which only a link
# builds, so their records hold the values those functions read in a
# JSON form of the harness's own, which the module docstring gives.


def _units_json(units) -> tuple[list, dict]:
    """The JSON of `units`, and a map from each input section's `id()`
    to `[position of its unit in units, its index in the unit's
    sections]`."""
    out = []
    where: dict[int, list[int]] = {}
    for pos, u in enumerate(units):
        sections: list = []
        refs = []
        local: dict[int, int] = {}
        for it in u.items:
            sec = u.sections[it.name]
            if id(sec) not in local:
                local[id(sec)] = len(sections)
                where.setdefault(id(sec), [pos, len(sections)])
                sections.append([sec.size, sec.flags])
            refs.append(local[id(sec)])
        out.append({
            "path": u.path, "index": u.index,
            "items": [[it.name, it.section, it.local, it.zero, it.merge]
                      for it in u.items],
            "sections": sections, "item_sections": refs,
            "tails": [[k, v] for k, v in u.tails.items()],
        })
    return out, where


def _layout_json(layout, where: dict[int, list[int]]) -> dict:
    return {
        "outputs": [{
            "name": o.name, "addr": o.addr, "load_addr": o.load_addr,
            "size": o.size, "noload": o.noload, "executable": o.executable,
            "parts": [[p.unit, where[id(p.section)], p.addr, p.size]
                      for p in o.parts],
        } for o in layout.outputs],
        "symbols": [[k, v] for k, v in layout.symbols.items()],
    }


def _wrap_lay_out(real):
    def lay_out(script, units, lookup=lambda name: None):
        if _QUIET:
            return real(script, units, lookup)
        from capture_passes import encode
        asked: dict[str, int | None] = {}

        def recording(name):
            value = lookup(name)
            asked.setdefault(name, value)
            return value
        units_json, where = _units_json(units)
        start = time.perf_counter()
        try:
            layout = real(script, units, recording)
        except Exception as e:
            _count("lay_out", time.perf_counter() - start)
            _record("lay_out", {}, {
                "script": encode(script), "units": units_json,
                "lookups": [[k, v] for k, v in asked.items()]}, error=e)
            raise
        _count("lay_out", time.perf_counter() - start)
        _record("lay_out", {}, {
            "script": encode(script), "units": units_json,
            "lookups": [[k, v] for k, v in asked.items()]},
            _layout_json(layout, where))
        return layout
    return lay_out


def _wrap_default_script(real):
    def default_script(units, origin):
        if _QUIET:
            return real(units, origin)
        from capture_passes import encode
        start = time.perf_counter()
        script = real(units, origin)
        _count("default_script", time.perf_counter() - start)
        _record("default_script", {"origin": origin},
                {"units": _units_json(units)[0]}, encode(script))
        return script
    return default_script


def _wrap_fall_through(real):
    def fall_through(entries, reorder=True):
        if _QUIET:
            return real(entries, reorder)
        start = time.perf_counter()
        out = real(entries, reorder)
        _count("fall_through", time.perf_counter() - start)
        index = {id(e): i for i, e in enumerate(entries)}
        _record("fall_through", {"reorder": reorder},
                {"entries": [[e.name, e.function, e.tail] for e in entries]},
                [[index[id(e)], dropped] for e, dropped in out])
        return out
    return fall_through


def _wrap_tail_jump(real):
    def tail_jump(sec, symbols):
        if _QUIET:
            return real(sec, symbols)
        start = time.perf_counter()
        out = real(sec, symbols)
        _count("tail_jump", time.perf_counter() - start)
        # Only the symbols in `sec` change the result, and the others
        # would make each object's records distinct.
        _record("tail_jump", {}, {
            "type": sec.type, "size": sec.size, "data": bytes(sec.data).hex(),
            "relocs": [[r.offset, r.type, r.addend, r.symbol.name]
                       for r in sec.relocs],
            "symbols": [[s.name, s.value, s.type] for s in symbols
                        if s.section is sec],
        }, out)
        return out
    return tail_jump


def _wrap_startup_lines(real):
    def startup_lines(script, instr_line):
        if _QUIET:
            return real(script, instr_line)
        from capture_passes import encode
        start = time.perf_counter()
        out = real(script, instr_line)
        _count("startup_lines", time.perf_counter() - start)
        _record("startup_lines", {}, encode(script), out)
        return out
    return startup_lines


# --- binutils/ld.py, and the linker's functions on the objects -------------
#
# A link changes its objects in place between these calls (renames,
# `relax`, `_merge_identical`), so each record names an object by the
# hash of its JSON form, `_elf_json`, stored as a blob, which keeps what
# an ELF file can't: the symbols' order, and symbols and sections that
# the object's lists no longer hold but a relocation or a symbol names.


def _elf_json(obj) -> dict:
    """The JSON form of `obj`, which the module docstring gives."""
    from binutils import elf as E
    secs = list(obj.sections)
    sec_ix: dict[int, int] = {}
    for i, s in enumerate(secs):
        sec_ix[id(s)] = i
    syms = list(obj.symbols)
    sym_ix: dict[int, int] = {}
    for i, s in enumerate(syms):
        sym_ix.setdefault(id(s), i)

    def sec_ref(sec) -> int:
        if not isinstance(sec, E.Section):
            return -1 - sec
        if id(sec) not in sec_ix:
            sec_ix[id(sec)] = len(secs)
            secs.append(sec)
        return sec_ix[id(sec)]

    def sym_ref(s) -> int:
        if id(s) not in sym_ix:
            sym_ix[id(s)] = len(syms)
            syms.append(s)
        return sym_ix[id(s)]
    out_syms: list = []
    out_secs: list = []
    i = j = 0
    while i < len(syms) or j < len(secs):
        while i < len(syms):
            s = syms[i]
            i += 1
            out_syms.append([s.name, s.value, s.size, s.bind, s.type,
                             sec_ref(s.section), s.other])
        while j < len(secs):
            c = secs[j]
            j += 1
            out_secs.append([
                c.name, c.type, c.flags, bytes(c.data).hex(), c.nobits_size,
                c.addr, c.align,
                [[r.offset, r.type, sym_ref(r.symbol), r.addend]
                 for r in c.relocs],
            ])
    return {
        "type": obj.type, "entry": obj.entry, "flags": obj.flags,
        "nsections": len(obj.sections), "sections": out_secs,
        "nsymbols": len(obj.symbols), "symbols": out_syms,
        "segments": [[g.vaddr, g.paddr, bytes(g.data).hex(), g.memsz, g.flags]
                     for g in obj.segments],
    }


def _ref(obj) -> str:
    """The hash of the blob holding `obj`'s JSON form."""
    return _blob(json.dumps(_elf_json(obj)).encode("utf-8"))


def _objects_json(objects) -> list:
    return [[path, _ref(obj)] for path, obj in objects]


def _where(objects) -> dict[int, list[int]]:
    """Each section of `objects` by `id()`, as `[object, section]`."""
    out: dict[int, list[int]] = {}
    for k, (_, obj) in enumerate(objects):
        for j, sec in enumerate(obj.sections):
            out.setdefault(id(sec), [k, j])
    return out


def _items_json(items) -> list:
    return [[it.name, it.section, it.local, it.zero, it.merge] for it in items]


def _index(symbols, sym) -> int:
    return next(i for i, s in enumerate(symbols) if s is sym)


def _pairs(table) -> list:
    return [[k, v] for k, v in (table or {}).items()]


def _outputs_json(outputs, where) -> list:
    return [{
        "name": o.name, "addr": o.addr, "load_addr": o.load_addr,
        "size": o.size, "noload": o.noload, "executable": o.executable,
        "parts": [[p.unit, where[id(p.section)], p.addr, p.size]
                  for p in o.parts],
    } for o in outputs]


def _symbols_json(symbols, outputs, where) -> list:
    """The symbols of an executable's table before `executable` writes
    it, each section as `["output", i]`, `["section", k, j]`, or
    `["shndx", n]`."""
    from binutils import elf as E
    out_ix = {id(o): i for i, o in enumerate(outputs)}
    out = []
    for s in symbols:
        if id(s.section) in out_ix:
            sec = ["output", out_ix[id(s.section)]]
        elif isinstance(s.section, E.Section):
            sec = ["section", *where[id(s.section)]]
        else:
            sec = ["shndx", s.section]
        out.append([s.name, s.value, s.size, s.bind, s.type, sec, s.other])
    return out


def _wrap_call(kind: str, real, describe, result):
    """A wrapper of `real` that records each call as `kind`: `describe`
    gives the arguments and the input from the call's arguments before
    the call, and `result` the output from them and the returned value
    after it."""
    def wrapper(*a, **kw):
        if _QUIET:
            return real(*a, **kw)
        with _quiet():
            try:
                args, value = describe(*a, **kw)
            except Exception:  # noqa: BLE001 - a value the form can't hold
                _anomaly(f"{kind}-unrecorded", _SOURCE)
                return real(*a, **kw)
        start = time.perf_counter()
        try:
            out = real(*a, **kw)
        except Exception as e:
            _count(kind, time.perf_counter() - start)
            _record(kind, args, value, error=e)
            raise
        _count(kind, time.perf_counter() - start)
        with _quiet():
            try:
                output = result(out, *a, **kw)
            except Exception:  # noqa: BLE001 - a value the form can't hold
                _anomaly(f"{kind}-unrecorded", _SOURCE)
                return out
            _record(kind, args, value, output)
        return out
    return wrapper


def _wrap_global_definitions(real):
    def describe(objects):
        return {}, {"objects": _objects_json(objects)}

    def result(out, objects):
        defs, absolute = out
        return {"defs": [[n, k, _index(objects[k][1].symbols, s)]
                         for n, (k, s) in defs.items()],
                "absolute": _pairs(absolute)}
    return _wrap_call("global_definitions", real, describe, result)


def _relax_symbols(sec, symbols):
    """The symbols `relax` reads, which are `symbols` and then each
    other one a relocation of `sec` names, and each one's index."""
    syms = list(symbols)
    ix: dict[int, int] = {}
    for i, s in enumerate(syms):
        ix.setdefault(id(s), i)
    for r in sec.relocs:
        if id(r.symbol) not in ix:
            ix[id(r.symbol)] = len(syms)
            syms.append(r.symbol)
    return syms, ix


def _wrap_relax(real):
    def describe(sec, symbols, value_of):
        syms, ix = _relax_symbols(sec, symbols)
        return {}, {
            "section": [
                sec.name, sec.type, sec.flags, bytes(sec.data).hex(),
                sec.nobits_size, sec.addr, sec.align,
                [[r.offset, r.type, ix[id(r.symbol)], r.addend]
                 for r in sec.relocs],
            ],
            "symbols": [[s.name, s.value, s.size, s.bind, s.type,
                         s.section is sec, s.other] for s in syms],
            "nsymbols": len(symbols),
            "values": [value_of(r.symbol) for r in sec.relocs],
        }

    def result(saved, sec, symbols, value_of):
        _, ix = _relax_symbols(sec, symbols)
        return {"saved": saved, "data": bytes(sec.data).hex(),
                "relocs": [[r.offset, r.type, ix[id(r.symbol)], r.addend]
                           for r in sec.relocs],
                "own": [[s.value, s.size] for s in symbols
                        if s.section is sec]}
    return _wrap_call("relax", real, describe, result)


def _wrap_executable(real):
    def describe(objects, outputs, resolver, symbols, entry, link_text=None):
        if resolver.objects is not objects:
            raise ValueError("the resolver has other objects")
        where = _where(objects)
        by_id = {i: w for i, w in where.items()}
        return {"entry": entry, "link_text": link_text}, {
            "objects": _objects_json(objects),
            "outputs": _outputs_json(outputs, where),
            "placed": [[by_id[i], addr] for i, addr in resolver.addr.items()],
            "extra": _pairs(resolver.extra),
            "symbols": _symbols_json(symbols, outputs, where),
        }

    def result(exe, *a, **kw):
        from binutils import elf as E
        return {"executable": _blob(E.write(exe))}
    return _wrap_call("executable", real, describe, result)


def _wrap_runtime_object(real):
    def describe(objects, defsyms):
        return {"defsyms": _pairs(defsyms)}, {"objects": _objects_json(objects)}

    def result(out, objects, defsyms):
        from binutils import elf as E
        from capture_passes import encode
        if out is None:
            return None
        (name, obj), meta = out
        return {"name": name, "object": _blob(E.write(obj)),
                "meta": encode(meta)}
    return _wrap_call("linker_runtime_library", real, describe, result)


def _wrap_object_items(real):
    def describe(meta, obj, path):
        return {"path": path}, {"items": _items_json(meta.items),
                                "elf": _ref(obj)}

    def result(items, meta, obj, path):
        return _items_json(items)
    return _wrap_call("object_items", real, describe, result)


def _wrap_item_symbol(real):
    def describe(obj, name):
        return {"name": name}, {"elf": _ref(obj)}

    def result(sym, obj, name):
        return _index(obj.symbols, sym)
    return _wrap_call("item_symbol", real, describe, result)


def _wrap_without_init(real):
    def describe(obj):
        return {}, {"elf": _ref(obj)}

    def result(out, obj):
        return {"same": out is obj, "elf": _ref(out)}
    return _wrap_call("without_init", real, describe, result)


def _wrap_unit(real):
    def describe(k, path, obj, items, tail_jump):
        return {"k": k, "path": path}, {"elf": _ref(obj),
                                        "items": _items_json(items)}

    def result(u, k, path, obj, items, tail_jump):
        return {"path": u.path, "index": u.index,
                "items": _items_json(u.items),
                "sections": [[name, _index(obj.sections, sec)]
                             for name, sec in u.sections.items()],
                "tails": _pairs(u.tails)}
    return _wrap_call("unit", real, describe, result)


def _wrap_apply_renames(real):
    def describe(obj, renames):
        return {"renames": _pairs(renames)}, {"elf": _ref(obj)}

    def result(out, obj, renames):
        return [s.name for s in obj.symbols]
    return _wrap_call("apply_renames", real, describe, result)


def _wrap_pointer_names(real):
    def describe(objects):
        return {}, {"objects": _objects_json(objects)}

    def result(out, objects):
        return sorted(out)
    return _wrap_call("pointer_names", real, describe, result)


def _wrap_reassemble(real):
    def describe(objects, renames, placed, outside):
        return {"renames": [_pairs(r) for r in renames],
                "placed": _pairs(placed), "outside": sorted(outside)}, {
            "objects": _objects_json(objects)}

    def result(out, objects, renames, placed, outside):
        return {"objects": _objects_json(objects)}
    return _wrap_call("reassemble", real, describe, result)


def _wrap_output_symbols(real):
    def describe(objects, outputs, addr_of, placed_names, zp_objects,
                 layout_symbols, defsyms):
        where = _where(objects)
        return {}, {
            "objects": _objects_json(objects),
            "outputs": _outputs_json(outputs, where),
            "addr_of": [[where[i], p.unit, p.addr, p.size]
                        for i, p in addr_of.items()],
            "placed_names": _pairs(placed_names),
            "zp_sizes": [[name, records[0].size]
                         for name, records in zp_objects.items()],
            "layout_symbols": _pairs(layout_symbols),
            "defsyms": _pairs(defsyms),
        }

    def result(symbols, objects, outputs, *rest):
        return _symbols_json(symbols, outputs, _where(objects))
    return _wrap_call("output_symbols", real, describe, result)


def _wrap_merge_identical(real):
    def describe(objects, metas):
        return {}, {"objects": _objects_json(objects),
                    "metas": [_items_json(m.items) for m in metas]}

    def result(out, objects, metas):
        return {"objects": _objects_json(objects),
                "metas": [_items_json(m.items) for m in metas]}
    return _wrap_call("merge_identical", real, describe, result)


def _wrap_contents(real):
    def describe(k, obj, sym):
        return {"k": k}, {"elf": _ref(obj), "symbol": _index(obj.symbols, sym)}

    def result(out, k, obj, sym):
        if out is None:
            return None
        name, type_, flags, align, size, data, relocs = out
        return [name, type_, flags, align, size, data.hex(),
                [[o, t, list(target) if isinstance(target, tuple) else target, a]
                 for o, t, target, a in relocs]]
    return _wrap_call("contents", real, describe, result)


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
    """Wraps the functions the harness records, writing under `out`,
    which is made absolute, since tests and the corpus's jobs change the
    current directory."""
    global _OUT
    _OUT = Path(out).resolve()
    from binutils import assembler, disasm, elf
    import nm
    import objdump
    from passes import (link_layout, link_script, linker, runtime_library,
                        zp_link_metadata)
    assembler.assemble = _wrap_assemble(assembler.assemble)
    # The linker imports `needed` when it adds the library.
    runtime_library.needed = _wrap_needed(runtime_library.needed)
    # The assembler imports `parse_metadata` when it runs, and the linker
    # imported it by name.
    zp_link_metadata.parse_metadata = _wrap_parse_metadata(
        zp_link_metadata.parse_metadata)
    linker.parse_metadata = zp_link_metadata.parse_metadata
    elf.read = _wrap_read(elf.read)
    link_script.parse_script = _wrap_parse_script(link_script.parse_script)
    # The linker imports these from the module at each link, and
    # `lay_out` calls `fall_through` through the module's globals.
    link_layout.lay_out = _wrap_lay_out(link_layout.lay_out)
    link_layout.default_script = _wrap_default_script(
        link_layout.default_script)
    link_layout.fall_through = _wrap_fall_through(link_layout.fall_through)
    link_layout.tail_jump = _wrap_tail_jump(link_layout.tail_jump)
    link_layout.startup_lines = _wrap_startup_lines(link_layout.startup_lines)
    linker.link_objects = _wrap_link(linker.link_objects)
    # The linker imported ld's functions by name, and the Resolver calls
    # `global_definitions` through ld's globals.
    from binutils import ld
    for name, wrap in (("global_definitions", _wrap_global_definitions),
                       ("relax", _wrap_relax),
                       ("executable", _wrap_executable)):
        setattr(ld, name, wrap(getattr(ld, name)))
        setattr(linker, name, getattr(ld, name))
    # The linker's functions call each other through its globals.
    for name, wrap in (("_runtime_library", _wrap_runtime_object),
                       ("_object_items", _wrap_object_items),
                       ("_item_symbol", _wrap_item_symbol),
                       ("_without_init", _wrap_without_init),
                       ("_unit", _wrap_unit),
                       ("_apply_renames", _wrap_apply_renames),
                       ("_pointer_names", _wrap_pointer_names),
                       ("_reassemble", _wrap_reassemble),
                       ("_output_symbols", _wrap_output_symbols),
                       ("_merge_identical", _wrap_merge_identical),
                       ("_contents", _wrap_contents)):
        setattr(linker, name, wrap(getattr(linker, name)))
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
