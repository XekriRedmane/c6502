"""Runs code generation through the C++ port, for compile.py and the
simulator harness under `C6502_CPP_BACKEND=1`.

`codegen` runs everything after the type checker: `c99_to_tac`, under
`-O` the inliner, `return_in_zero_page`, and the TAC passes of
`compile._optimized_tac`, then `plan_zero_page`, code generation (under
`-O` the part of `compile._optimized_front` after the TAC passes,
without it `fixed_address_layouts`, `tac_to_asm`, and
`replace_pseudoregisters`), the asm back end's region (or the
unoptimized peephole fixed point and long branches), the work after it
that reads the asm program, `asm_to_asm2`, and `asm_emit`, and returns
the `-S` text. Python doesn't read a program back, except under
`C6502_CHECK_IR=1`, where the response holds the TAC that code
generation lowered and the finished asm and asm2 programs for
`ir_check`, which isn't ported. Without `emit`, the C++ side stops
where `compile.optimized_asm` (or the harness's unoptimized path) does,
and returns the finished asm program with its slot symbols, which the
simulator harness assembles.

Each call starts `c6502cpp backend` (cpp/src/backend.h describes the
request and the response), found as tools/check_captures.py finds it: at
the path in `C6502_CPP`, or in cpp/build/bin. The c99 program goes in
the dump format (ir_dump.py), and the tables in the capture harness's
tagged JSON (`tools.capture_passes.encode`), so the C++ side reads them
with the decoders it checks the captured calls with. The changes the
stage makes to the tables stay in C++, since Python reads nothing of
them after the stage, so Python's tables are left as the type checker
built them.

compile.py imports this module only when the variable is set, so a
compile without it imports nothing new.
"""
from __future__ import annotations

import json
import subprocess

import asm_ast
import ir_dump
from tools.capture_passes import encode
from tools.check_captures import cpp_cli


def _error_type(name: str) -> type[Exception]:
    """The Python exception that the C++ `CompileError` of Python type
    `name` stands for."""
    import builtins
    import importlib
    modules = {
        "AbiSelectionError": "passes.abi_selection",
        "FrameSizeError": "passes.asm_to_asm2",
        "RecursionSaveError": "passes.recursion_saves",
        "RegallocError": "passes.optimization_asm.regalloc",
        "RuntimeLayoutError": "passes.runtime_layout",
        "ZpGlobalError": "passes.zp_globals",
        "ZpLocalAllocationError": "passes.zp_local_allocation",
        "ZpSlotAllocationError": "passes.zp_slot_allocation",
    }
    if name in modules:
        return getattr(importlib.import_module(modules[name]), name)
    found = getattr(builtins, name, None)
    if isinstance(found, type) and issubclass(found, Exception):
        return found
    return RuntimeError


def _send(request: str) -> bytes:
    """Sends `request` to `c6502cpp backend` and returns the response's
    bytes. A bug the C++ compiler reports (exit status 3) is raised as
    an `AssertionError` with its message, since that is what the C++
    `InternalError` stands for, and a problem with the program (exit
    status 4) as the Python exception its first line names, with the
    rest as the message."""
    cli = cpp_cli()
    if cli is None:
        raise RuntimeError(
            "C6502_CPP_BACKEND=1 is set, but c6502cpp isn't built "
            "(cpp/README.md has the build)",
        )
    done = subprocess.run(
        [str(cli), "backend"], input=request.encode("utf-8"),
        capture_output=True,
    )
    if done.returncode == 4:
        # stderr is in text mode on Windows, which writes CRLF.
        err = done.stderr.decode("utf-8").replace("\r\n", "\n")
        kind, _, message = err.partition("\n")
        raise _error_type(kind)(message)
    if done.returncode != 0:
        message = done.stderr.decode("utf-8", "replace").strip()
        if done.returncode == 3:
            raise AssertionError(message.removeprefix("internal error: "))
        raise RuntimeError(f"c6502cpp backend failed: {message}")
    return done.stdout


def _section(name: str, text: str) -> str:
    """A section of the request: its name and its length in bytes on a
    line, then its text."""
    return f"{name} {len(text.encode('utf-8'))}\n{text}"


def _sections(data: bytes) -> tuple[dict[str, str], dict[str, str]]:
    """Splits a response into its lines by key and its sections by name
    (cpp/src/backend.h)."""
    lines: dict[str, str] = {}
    sections: dict[str, str] = {}
    pos = 0
    while pos < len(data):
        end = data.index(b"\n", pos)
        key, _, value = data[pos:end].decode("utf-8").partition(" ")
        pos = end + 1
        if key in ("tac", "asm", "asm2", "text"):
            n = int(value)
            sections[key] = data[pos:pos + n].decode("utf-8")
            pos += n
        else:
            lines[key] = value
    return lines, sections


def _dump_c99(prog) -> str:
    """The dump of the type-checked c99 program `prog`. A declaration
    may leave a parameter unnamed, which `function_decl.params` holds as
    None, and the dump format has no form for None in an `identifier*`.
    Such a program goes as a copy with each None there made the empty
    string, which no C identifier spells and which the C++
    `abi_selection` reads as an unnamed parameter."""
    try:
        return ir_dump.dump(prog)
    except ir_dump.DumpError:
        return ir_dump.dump(_with_unnamed_params(prog))


def _with_unnamed_params(v):
    """A copy of the c99 value `v` whose `function_decl`s have "" for
    each unnamed parameter."""
    import dataclasses
    import c99_ast
    from passes.dataclass_fields import fields_of
    if isinstance(v, list):
        return [_with_unnamed_params(x) for x in v]
    if not dataclasses.is_dataclass(v) or isinstance(v, type):
        return v
    changes = {}
    for f in fields_of(v):
        x = getattr(v, f.name)
        if isinstance(v, c99_ast.Type_function_decl) and f.name == "params":
            y = ["" if p is None else p for p in x]
        else:
            y = _with_unnamed_params(x)
        if y is not x:
            changes[f.name] = y
    return dataclasses.replace(v, **changes) if changes else v


def _check_tac(text: str) -> None:
    """Checks each function of the TAC that code generation lowered, as
    the Python TAC optimizer's driver checks each function after its
    last pass. The symbol table isn't given, since Python's doesn't
    hold the temporaries the C++ passes added."""
    import tac_ast
    from passes import ir_check
    for tl in ir_dump.load(text, tac_ast).top_level:
        if isinstance(tl, tac_ast.Function):
            ir_check.check_tac(tl, after="the C++ TAC passes")


def codegen(
    prog, symbols, types, *, optimize: bool, inline: bool = True,
    link_metadata: bool = False, emit: bool = True,
):
    """Returns the `-S` text of the unit whose type-checked program is
    `prog`, with the tables `symbols` and `types` the type checker
    built, which aren't changed. `optimize` is `-O`, `inline` is false
    when `-O` doesn't inline the calls of small `static` functions
    (tests/no_inlining.py), and `link_metadata` gives the unoptimized
    text the link metadata block `-c` assembles. Without `emit`, returns
    the finished asm program and its slot symbols, `{**slot_symbols,
    **zp.equs()}` under `-O` and `{**pinned_slot_symbols(fixed_abi),
    **zp.equs()}` without it, as the simulator harness computes them.
    Under `C6502_CHECK_IR=1` it checks the TAC that code generation
    lowered, the finished asm program, and the asm2 program of a text,
    as compile.py checks them."""
    from passes import ir_check
    from passes.runtime_layout import helper_footprints
    check = ir_check.enabled()
    args = json.dumps({
        "symbols": encode(symbols), "types": encode(types),
        "footprints": encode(helper_footprints()), "optimize": optimize,
        "inline": inline, "check_ir": check, "link_metadata": link_metadata,
    })
    request = (
        f"c6502-backend 2\nmode codegen{'-text' if emit else ''}\n"
        f"args {args}\n{_section('c99', _dump_c99(prog))}"
    )
    lines, sections = _sections(_send(request))
    if check:
        _check_tac(sections["tac"])
    if not emit:
        out = ir_dump.load(sections["asm"], asm_ast)
        ir_check.check_asm(out, after="the C++ back end")
        return out, dict(json.loads(lines["symbols"]))
    if check:
        import asm2_ast
        ir_check.check_asm(
            ir_dump.load(sections["asm"], asm_ast), after="the C++ back end",
        )
        ir_check.check_asm2(
            ir_dump.load(sections["asm2"], asm2_ast), after="asm_to_asm2",
        )
    return sections["text"]
