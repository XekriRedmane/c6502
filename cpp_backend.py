"""Runs the asm back end's region through the C++ port, for compile.py
under `C6502_CPP_BACKEND=1`.

The region is what `compile.optimized_asm` runs from `apply_licm`
through `apply_outline_runs`, and, without `-O`, `_codegen`'s peephole
fixed point and `expand_long_branches`. Each call starts `c6502cpp
backend` (cpp/src/backend.h describes the request and the response),
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


def _run(mode: str, prog: asm_ast.Program, kwargs: dict) -> dict[str, str]:
    """Sends `prog` and the keyword arguments `kwargs` to `c6502cpp
    backend` in `mode` and returns the response's lines by key, with
    the result's dump under `output`. A bug the C++ compiler reports
    (exit status 3) is raised as an `AssertionError` with its message,
    since that is what the C++ `InternalError` stands for."""
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
    if done.returncode != 0:
        message = done.stderr.decode("utf-8", "replace").strip()
        if done.returncode == 3:
            raise AssertionError(message.removeprefix("internal error: "))
        raise RuntimeError(f"c6502cpp backend failed: {message}")
    text = done.stdout.decode("utf-8")
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
    for name, owner in json.loads(out["new_functions"]):
        sym = symbols.get(owner)
        if sym is not None:
            symbols[name] = replace(
                sym, attrs=replace(sym.attrs, is_global=False),
            )
    coalesced = {fn: slots for fn, slots in json.loads(out["coalesced"])}
    return ir_dump.load(out["output"], asm_ast), coalesced


def unoptimized_region(prog: asm_ast.Program, *, zp_addrs) -> asm_ast.Program:
    """Returns `expand_long_branches(_peephole_fixedpoint(prog,
    zp_global_symbols=zp_addrs))`, computed in C++."""
    out = _run("unoptimized", prog, {"zp_addrs": zp_addrs})
    return ir_dump.load(out["output"], asm_ast)
