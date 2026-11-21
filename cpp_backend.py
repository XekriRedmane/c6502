"""Runs code generation through the C++ port, for compile.py and the
simulator harness under `C6502_CPP_BACKEND=1`.

`optimized_stage` and `unoptimized_stage` run everything after the TAC:
`plan_zero_page`, code generation (under `-O` the part of
`compile._optimized_front` after the TAC passes, without it
`fixed_address_layouts`, `tac_to_asm`, and `replace_pseudoregisters`),
the asm back end's region (or the unoptimized peephole fixed point and
long branches), the work after it that reads the asm program,
`asm_to_asm2`, and `asm_emit`, and return the `-S` text. Python doesn't
read the program back, except under `C6502_CHECK_IR=1`, where the
response holds the finished asm and asm2 programs for `ir_check`, which
isn't ported. Without `emit`, the C++ side stops where
`compile.optimized_asm` (or the harness's unoptimized path) does, and
returns the finished asm program with its slot symbols, which the
simulator harness assembles.

Each call starts `c6502cpp backend` (cpp/src/backend.h describes the
request and the response), found as tools/check_captures.py finds it: at
the path in `C6502_CPP`, or in cpp/build/bin. The programs go in the
dump format (ir_dump.py), and the tables in the capture harness's tagged
JSON (`tools.capture_passes.encode`), so the C++ side reads them with
the decoders it checks the captured calls with. The response gives the
changes the stage made to the symbol table, which are entered in
Python's: the `<param>.reg` locals of `tac_to_asm`, the carry-flag
returns that `carry_return_choice` chose, and the functions that
`shared_tails` and `outline_runs` made.

compile.py imports this module only when the variable is set, so a
compile without it imports nothing new.
"""
from __future__ import annotations

import json
import subprocess
from dataclasses import replace

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
        if key in ("asm", "asm2", "text"):
            n = int(value)
            sections[key] = data[pos:pos + n].decode("utf-8")
            pos += n
        else:
            lines[key] = value
    return lines, sections


def _enter_changes(symbols, lines: dict[str, str]) -> None:
    """Enters in `symbols` the changes the response's lines give, as the
    Python passes make them: each `<param>.reg` local as a local of its
    parameter's type, each carry-flag return chosen, and each function
    `[name, owner]` the region made as a copy of its owner's symbol with
    internal linkage."""
    from passes.type_checking import LocalAttr, Symbol
    for local in json.loads(lines["reg_locals"]):
        param = symbols[local.removesuffix(".reg")]
        symbols[local] = Symbol(
            type=param.type, attrs=LocalAttr(register=False),
        )
    for name in json.loads(lines["carry"]):
        # The attributes may be shared with another symbol, so they are
        # replaced, as `carry_return_choice` replaces them.
        sym = symbols[name]
        sym.attrs = replace(sym.attrs, carry_return=True)
    for name, owner in json.loads(lines["new_functions"]):
        sym = symbols.get(owner)
        if sym is not None:
            symbols[name] = replace(
                sym, attrs=replace(sym.attrs, is_global=False),
            )


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


def dump_tac(tac) -> str:
    """The dump of the TAC program `tac`, taken before the TAC passes of
    `-O` change it, for `plan_zero_page`."""
    return ir_dump.dump(tac)


def _stage(
    mode: str, prog, symbols, types, statics, tac_sections: str, *,
    emit: bool, **flags,
):
    """Runs the stage mode `mode` and returns the text, or without
    `emit` the finished asm program and its slot symbols, after entering
    the stage's changes in `symbols`. Under `C6502_CHECK_IR=1` it
    checks the finished asm program, and the asm2 program of a text,
    as compile.py checks them after the pipeline and `asm_to_asm2`."""
    from passes import ir_check
    from passes.runtime_layout import helper_footprints
    check = ir_check.enabled()
    args = json.dumps({
        "symbols": encode(symbols), "types": encode(types),
        "statics": encode(statics), "footprints": encode(helper_footprints()),
        "check_ir": check, **flags,
    })
    request = (
        f"c6502-backend 1\nmode {mode}{'-text' if emit else ''}\n"
        f"args {args}\n{_section('c99', _dump_c99(prog))}{tac_sections}"
    )
    lines, sections = _sections(_send(request))
    _enter_changes(symbols, lines)
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


def optimized_stage(
    prog, symbols, types, statics, unoptimized: str, tac, *,
    emit: bool = True,
):
    """Returns the `-S -O` text of the unit whose type-checked program
    is `prog`, from `tac`, the TAC after `compile._optimized_tac`.
    `unoptimized` is `dump_tac` of the TAC before those passes, which
    `plan_zero_page` reads, and `statics` the static-storage names of
    `symbols` before them, as `_codegen` computes them. Without `emit`,
    returns the finished asm program and `{**slot_symbols,
    **zp.equs()}`, as the simulator harness takes them from
    `optimized_asm`."""
    tac_sections = (
        _section("tac", unoptimized)
        + _section("optimized-tac", ir_dump.dump(tac))
    )
    return _stage(
        "stage-optimized", prog, symbols, types, statics, tac_sections,
        emit=emit,
    )


def unoptimized_stage(
    prog, symbols, types, statics, tac, *, link_metadata: bool,
    emit: bool = True,
):
    """Returns the `-S` text of the unit whose type-checked program is
    `prog`, from `tac`, the TAC of `translate_for_codegen`, with the
    link metadata block `-c` assembles when `link_metadata` is set.
    Without `emit`, returns the finished asm program and
    `{**pinned_slot_symbols(fixed_abi), **zp.equs()}`, as the simulator
    harness computes them."""
    return _stage(
        "stage-unoptimized", prog, symbols, types, statics,
        _section("tac", ir_dump.dump(tac)), emit=emit,
        link_metadata=link_metadata,
    )
