"""Runs the asm back end through the C++ port, for compile.py under
`C6502_CPP_BACKEND=1`.

`optimized_text` and `unoptimized_text` give the `-S` text: the region
that `compile.optimized_asm` runs from `apply_licm` through
`apply_outline_runs` (or, without `-O`, `_codegen`'s peephole fixed
point and `expand_long_branches`), the work after it that reads the asm
program (`link_records`, `place_zp_globals`, `check_shares`,
`prune_unused_locals`, `hargs_need`, and `section_items`),
`asm_to_asm2`, and `asm_emit`. Python doesn't read the program back,
except under `C6502_CHECK_IR=1`, where the response holds the finished
asm and asm2 programs for `ir_check`, which isn't ported.
`optimized_region` and `unoptimized_region` run only the region and
return the program, for the simulator harness, which assembles it.

Each call starts `c6502cpp backend` (cpp/src/backend.h describes the
request and the response),
found as tools/check_captures.py finds it: at the path in `C6502_CPP`,
or in cpp/build/bin. The program goes in the dump format (ir_dump.py),
the context of the compile as the capture harness's `context` line,
and the other arguments in its tagged JSON
(`tools.capture_passes.encode`), so the C++ side reads them with the
decoders it checks the captured calls with.

compile.py imports this module only when the variable is set, so a
compile without it imports nothing new.
"""
from __future__ import annotations

import json
import subprocess
from dataclasses import replace

import asm_ast
import ir_dump
from tools.capture_passes import context_json, encode
from tools.check_captures import cpp_cli


def _error_type(name: str) -> type[Exception]:
    """The Python exception that the C++ `CompileError` of Python type
    `name` stands for."""
    import builtins
    if name == "FrameSizeError":
        from passes.asm_to_asm2 import FrameSizeError
        return FrameSizeError
    if name == "RuntimeLayoutError":
        from passes.runtime_layout import RuntimeLayoutError
        return RuntimeLayoutError
    found = getattr(builtins, name, None)
    if isinstance(found, type) and issubclass(found, Exception):
        return found
    return RuntimeError


def _call(mode: str, prog: asm_ast.Program, kwargs: dict) -> bytes:
    """Sends `prog` and the keyword arguments `kwargs` to `c6502cpp
    backend` in `mode` and returns the response's bytes. A bug the C++
    compiler reports (exit status 3) is raised as an `AssertionError`
    with its message, since that is what the C++ `InternalError` stands
    for, and a problem with the program (exit status 4) as the Python
    exception its first line names, with the rest as the message."""
    cli = cpp_cli()
    if cli is None:
        raise RuntimeError(
            "C6502_CPP_BACKEND=1 is set, but c6502cpp isn't built "
            "(cpp/README.md has the build)",
        )
    args = json.dumps({k: encode(v) for k, v in kwargs.items()})
    request = (
        f"c6502-backend 1\nmode {mode}\ncontext {context_json()}\n"
        f"args {args}\ninput\n{ir_dump.dump(prog)}"
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


def _run(mode: str, prog: asm_ast.Program, kwargs: dict) -> dict[str, str]:
    """Runs `_call` in a mode that returns a program and returns the
    response's lines by key, with the result's dump under `output`."""
    text = _call(mode, prog, kwargs).decode("utf-8")
    head, sep, dump = text.partition("output\n")
    if not sep:
        raise RuntimeError("c6502cpp backend wrote no output")
    out = {"output": dump}
    for line in head.splitlines():
        key, _, value = line.partition(" ")
        out[key] = value
    return out


def optimized_region(
    prog: asm_ast.Program, symbols, *, slot_symbols, zp_addrs, noreturn,
    abi, shares, ret_eligible, zp_names, slot_addrs,
) -> tuple[asm_ast.Program, dict[str, list[str]]]:
    """Returns the program that `optimized_asm`'s passes from
    `apply_licm` through `apply_outline_runs` make of `prog`, and the
    slots `apply_slot_coalescing` made each function write. Each
    function that `apply_shared_tails` and `apply_outline_runs` made
    gets a symbol in `symbols`, a copy of its owner's with internal
    linkage, as the Python passes give it."""
    out = _run("optimized", prog, {
        "slot_symbols": slot_symbols, "zp_addrs": zp_addrs,
        "noreturn": noreturn, "abi": abi, "shares": shares,
        "ret_eligible": ret_eligible, "zp_names": zp_names,
        "slot_addrs": slot_addrs,
    })
    _enter_new_functions(symbols, json.loads(out["new_functions"]))
    coalesced = {fn: slots for fn, slots in json.loads(out["coalesced"])}
    return ir_dump.load(out["output"], asm_ast), coalesced


def _enter_new_functions(symbols, made) -> None:
    """Gives each function `[name, owner]` of `made` a symbol in
    `symbols`, a copy of its owner's with internal linkage, as the
    Python passes give it."""
    for name, owner in made:
        sym = symbols.get(owner)
        if sym is not None:
            symbols[name] = replace(
                sym, attrs=replace(sym.attrs, is_global=False),
            )


def unoptimized_region(prog: asm_ast.Program, *, zp_addrs) -> asm_ast.Program:
    """Returns `expand_long_branches(_peephole_fixedpoint(prog,
    zp_global_symbols=zp_addrs))`, computed in C++."""
    out = _run("unoptimized", prog, {"zp_addrs": zp_addrs})
    return ir_dump.load(out["output"], asm_ast)


def _sections(data: bytes) -> tuple[dict[str, str], dict[str, str]]:
    """Splits a response of the `*-text` modes into its lines by key and
    its sections by name (cpp/src/backend.h)."""
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


def _emit(mode: str, prog: asm_ast.Program, symbols, kwargs: dict) -> str:
    """Runs `mode` (`optimized-text` or `unoptimized-text`) and returns
    the text. Enters each function the region made in `symbols`, as
    `optimized_region` does, and under `C6502_CHECK_IR=1` checks the
    finished asm and asm2 programs, as compile.py checks them after the
    region and after `asm_to_asm2`."""
    from passes import ir_check
    from passes.runtime_layout import helper_footprints
    from passes.sections import section_info
    check = ir_check.enabled()
    lines, sections = _sections(_call(mode, prog, {
        **kwargs, "footprints": helper_footprints(),
        "sections": section_info(prog, symbols), "check_ir": check,
    }))
    _enter_new_functions(symbols, json.loads(lines["new_functions"]))
    if check:
        import asm2_ast
        ir_check.check_asm(
            ir_dump.load(sections["asm"], asm_ast), after="the C++ back end",
        )
        ir_check.check_asm2(
            ir_dump.load(sections["asm2"], asm2_ast), after="asm_to_asm2",
        )
    return sections["text"]


def optimized_text(
    prog: asm_ast.Program, symbols, *, zp, meta, preferred_symbols,
    slot_symbols, zp_addrs, noreturn, abi, shares, ret_eligible, zp_names,
    slot_addrs,
) -> str:
    """Returns the `-S -O` text of `prog`, the program after
    `synthesize_prologue`. `zp` is the unit's `ZpGlobals`, `meta` the
    `LinkMetadata` that `build_metadata` gives, with the shares and the
    HARGS size set, and `preferred_symbols` what
    `slot_sharing.preferred_symbols` gives. The other arguments are
    `optimized_region`'s."""
    return _emit("optimized-text", prog, symbols, {
        "slot_symbols": slot_symbols, "zp_addrs": zp_addrs,
        "noreturn": noreturn, "abi": abi, "shares": shares,
        "ret_eligible": ret_eligible, "zp_names": zp_names,
        "slot_addrs": slot_addrs, "zp": zp, "equs": zp.equs(),
        "meta": meta, "preferred_symbols": preferred_symbols,
    })


def unoptimized_text(
    prog: asm_ast.Program, symbols, *, zp, equs, meta,
) -> str:
    """Returns the `-S` text of `prog`, the program after
    `replace_pseudoregisters`, without `-O`. `equs` is the whole EQU
    block, and `meta`, for `-c`, the `LinkMetadata` with its `ext`
    records and HARGS size, or None for no metadata block."""
    return _emit("unoptimized-text", prog, symbols, {
        "zp_addrs": zp.addrs, "zp": zp, "equs": equs, "meta": meta,
    })
