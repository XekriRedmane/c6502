"""Inlines small `static` functions into their callers.

A call of a function costs the caller the stores of its arguments, a
`JSR`, and the copies of its result, and the function costs an `RTS`
and often the copies of its parameters out of their slots. For a small
function, a copy of its body in place of the call is shorter than all
of that, and the optimizer then works on the copy with the caller's
own values: an argument that is a constant folds into the body, a
`bool` result that the caller only branches on becomes the branch, and
a pointer result that the caller reads through is read where it was
computed. C programmers write such functions as function-like macros
for this reason. This pass makes that unnecessary under `-O`.

A function is a candidate when

- it's defined in this unit with internal linkage (`static`), so every
  call of it is in this unit,
- its address isn't taken (`abi_selection._address_taken`), so every
  call is a direct `FunctionCall`,
- it calls itself through no chain of calls, which a walk of the
  unit's call graph checks,
- its declaration has none of the c6502 attributes that set a calling
  convention or a placement: `[[c6502::zp_abi]]`, `section`,
  `address`, `return_bool_in_carry`, `[[noreturn]]`, a `register` or
  `in_*` parameter, a `zeropage(addr)` parameter, or a `register`
  local (`abi_selection._has_register_request`),
- its parameters and its result are scalars (an integer, a pointer,
  `float`, or `double`), so that one `Copy` moves each of them,
- its body has no `IndirectCall`, no call marked
  `[[c6502::save_hardware_stack]]`, no `GetAddress` of one of its own
  locals, and no block-scope `static` object, whose name starts with
  `@` (`identifier_resolution` renames only block-scope names).

A call of a candidate is inlined when its caller has no `register`
local, whose pin to Y the inlined body could break, when the call
isn't marked `[[c6502::save_hardware_stack]]`, and when each argument
has the width and kind (integer or floating) of its parameter, which
the type checker's conversions give every call.

Each function's calls are all inlined, or none are. They're inlined
when an estimate of the bytes that inlining adds is at most the
estimate of the bytes it removes (`_Inliner._pays`). It removes, at
each of the n calls, the `JSR` and the moves of the arguments and the
result, `CALL_BYTES + ARGUMENT_BYTE_COST * a + RESULT_BYTE_COST * r`
for `a` bytes of arguments and `r` of result, and once the function's
`RTS`. It adds n - 1 copies of the body beyond the one the function
held, `BYTES_PER_COST` bytes for each unit of `inline_cost` of the
function after the TAC optimizer, and `EARLY_RETURN_BYTES` for each
`Ret` before the body's end at each call whose result the caller
doesn't return at once, since such a `Ret` becomes a store and a jump
where the function had an `RTS`. So a function with one call is
inlined unless it has several early returns, and one with several
calls only when its body is about as small as a call. The constants
were fitted to the example programs compiled with each candidate
inlined alone, which docs/optimizations/inline_functions.md describes.
When every call of a function has been inlined, the function is
dropped from the program.

The functions are taken leaves first, so a candidate that calls
another candidate is costed and copied with that call already
inlined.

In TAC, the call `FunctionCall(f, [a1, ..., an], d)` becomes

    Copy(a1, p1') ... Copy(an, pn')
    <f's body, with each local and label renamed>
    Label(end)

where `pi'` is the renamed i-th parameter. Each `Ret(v)` in the copy
becomes `Copy(v, d)` (when the call has a result and `Ret` a value)
and `Jump(end)`, which is left out after the last instruction. When
the caller's next instruction is a `Ret` of the call's result, or a
`Ret` with no value, the copy replaces that `Ret` too, and each `Ret`
in the copy stays a `Ret`, of its own value or of none. The
arguments were evaluated into the `ai` before the call in the order
`c99_to_tac` gives them, and the copies read each of them once, at
the place the call read them, so the order of side effects and of
volatile accesses is that of the call. Every instruction keeps its
`is_volatile` flag, and each renamed local keeps its symbol's type,
`volatile` included. Copy propagation removes the parameter copies
where the argument isn't written again before the parameter is read.

A renamed local is `@N.name` with a fresh N, or `%N` with a fresh N
for a temporary, so the zero-page slot names still carry the C
variable's name (passes/zp_slot_naming.py). The k-th copy's labels are
`.ik_` followed by the original label without its leading `.`, which
keeps the `_start`, `_continue`, and `_break` endings that loop
rotation reads and the `_keep` mark that dead-loop elimination reads,
and its end label is `.ik@end`.

The pass runs once, over the whole program, on the TAC from
`c99_to_tac`, in `compile.translate_for_codegen` before every other
`-O` pass, so the TAC optimizer, ABI selection, the call graph, the
carry-return choice, zero-page promotion, and the link metadata read
only the functions that are left. It doesn't run in the unoptimized
pipeline."""

from __future__ import annotations

import copy
import dataclasses
import re

import c99_ast
import tac_ast
from c99_to_tac import _byte_width_of
from passes.abi_selection import (
    _address_taken, _function_decls_by_name, _has_register_local,
    _has_register_request,
)
from passes.dataclass_fields import fields_of
from passes.type_checking import FunAttr, LocalAttr, StaticAttr, Symbol


# The estimates of bytes that choose whether a function's calls are
# inlined (see the module docstring). Each was fitted to the examples,
# whose units were compiled with each candidate inlined alone
# (docs/optimizations/inline_functions.md).
#
# Code bytes for each unit of `inline_cost` after the TAC optimizer.
BYTES_PER_COST = 3
# A call's `JSR`.
CALL_BYTES = 3
# Each byte of the arguments, which the call stores into the callee's
# parameter bytes and the callee reads from there.
ARGUMENT_BYTE_COST = 2
# Each byte of the result, which the callee stores and the caller
# reads back.
RESULT_BYTE_COST = 4
# Each return before the end of the body, at a call whose result isn't
# returned at once: a store of the result and a `JMP` where the
# function had its own `RTS`.
EARLY_RETURN_BYTES = 4


def inline_functions(
    tac: tac_ast.Program, symbols, prog: c99_ast.Program,
) -> tac_ast.Program:
    """Inlines the calls of small `static` functions (see the module
    docstring) and returns the new program, adding a symbol for each
    renamed local to `symbols`. `prog` is the type-checked c99
    program, which holds the attributes of each declaration. A
    program with nothing to inline comes back unchanged."""
    return _Inliner(tac, symbols, prog).run()


def inline_cost(fn: tac_ast.Function, symbols) -> int:
    """The size measure of `fn`'s body that the choice to inline its
    calls multiplies by `BYTES_PER_COST`. It adds, for each
    instruction other than a `Label` and a `Ret` that ends the body,
    the width in bytes of its widest operand, or 1 for an instruction
    without one, such as a `Jump`. A call counts as 3, a `JSR`."""
    total = 0
    body = fn.instructions
    for idx, ins in enumerate(body):
        if isinstance(ins, tac_ast.Label):
            continue
        if isinstance(ins, tac_ast.Ret) and idx == len(body) - 1:
            continue
        if isinstance(ins, (tac_ast.FunctionCall, tac_ast.IndirectCall)):
            total += 3
            continue
        widths = [_val_width(v, symbols) for v in _vals(ins)]
        total += max([w for w in widths if w] or [1])
    return total


# ---------------------------------------------------------------------------


_CONST_WIDTH = {
    tac_ast.ConstChar: 1, tac_ast.ConstUChar: 1,
    tac_ast.ConstInt: 2, tac_ast.ConstUInt: 2,
    tac_ast.ConstLong: 4, tac_ast.ConstULong: 4,
    tac_ast.ConstLongLong: 8, tac_ast.ConstULongLong: 8,
    tac_ast.ConstFloat: 4, tac_ast.ConstDouble: 8,
    tac_ast.ConstInt24: 3, tac_ast.ConstUInt24: 3,
    tac_ast.ConstInt40: 5, tac_ast.ConstUInt40: 5,
    tac_ast.ConstInt48: 6, tac_ast.ConstUInt48: 6,
    tac_ast.ConstInt56: 7, tac_ast.ConstUInt56: 7,
}

_SCALARS = (
    c99_ast.Char, c99_ast.SChar, c99_ast.UChar, c99_ast.Bool,
    c99_ast.Int, c99_ast.UInt, c99_ast.Long, c99_ast.ULong,
    c99_ast.LongLong, c99_ast.ULongLong, c99_ast.BitInt,
    c99_ast.Float, c99_ast.Double, c99_ast.Pointer,
)


def _unqualified(t):
    while isinstance(t, (c99_ast.Const, c99_ast.Volatile)):
        t = t.referenced_type
    return t


def _is_scalar(t) -> bool:
    return isinstance(_unqualified(t), _SCALARS)


def _shape(t) -> tuple[int, bool] | None:
    """The width in bytes of the scalar type `t` and whether it's
    floating, or None for any other type."""
    t = _unqualified(t)
    if not isinstance(t, _SCALARS):
        return None
    return _byte_width_of(t), isinstance(t, (c99_ast.Float, c99_ast.Double))


def _val_shape(v, symbols) -> tuple[int, bool] | None:
    if isinstance(v, tac_ast.Constant):
        w = _CONST_WIDTH.get(type(v.const))
        if w is None:
            return None
        return w, isinstance(v.const, (tac_ast.ConstFloat, tac_ast.ConstDouble))
    sym = symbols.get(v.name)
    return None if sym is None else _shape(sym.type)


def _val_width(v, symbols) -> int | None:
    s = _val_shape(v, symbols)
    return None if s is None else s[0]


def _vals(ins):
    from passes.optimization.var_visit import vals_in
    return list(vals_in(ins))


def _calls(fn: tac_ast.Function) -> list[str]:
    return [
        ins.name for ins in fn.instructions
        if isinstance(ins, tac_ast.FunctionCall)
    ]


_AT_NAME = re.compile(r"^@(\d+)\.(.*)$", re.S)
_TEMP_NAME = re.compile(r"^%(\d+)$")


class _Inliner:
    def __init__(self, tac, symbols, prog) -> None:
        self.tac = tac
        self.symbols = symbols
        self.decls = _function_decls_by_name(prog)
        self.functions: dict[str, tac_ast.Function] = {
            tl.name: tl for tl in tac.top_level
            if isinstance(tl, tac_ast.Function)
        }
        self.copies = 0
        # The next free N for `@N.name` and `%N`.
        self.next_at = 0
        self.next_temp = 0
        for name, _ in symbols.items():
            m = _AT_NAME.match(name)
            if m:
                self.next_at = max(self.next_at, int(m.group(1)) + 1)
            m = _TEMP_NAME.match(name)
            if m:
                self.next_temp = max(self.next_temp, int(m.group(1)) + 1)
        self.labels: set[str] = set()
        for fn in self.functions.values():
            for ins in fn.instructions:
                if isinstance(ins, tac_ast.Label):
                    self.labels.add(ins.name)

    # -- choosing ---------------------------------------------------------

    def _recursive(self) -> set[str]:
        """The functions that can reach themselves through calls."""
        graph = {
            name: set(_calls(fn)) & set(self.functions)
            for name, fn in self.functions.items()
        }
        out: set[str] = set()
        for start in graph:
            seen: set[str] = set()
            stack = list(graph[start])
            while stack:
                n = stack.pop()
                if n == start:
                    out.add(start)
                    break
                if n in seen:
                    continue
                seen.add(n)
                stack.extend(graph.get(n, ()))
        return out

    def _candidate(self, fn: tac_ast.Function, taken, recursive) -> bool:
        if fn.is_global or fn.name in taken or fn.name in recursive:
            return False
        sym = self.symbols.get(fn.name)
        if sym is None or not isinstance(sym.attrs, FunAttr):
            return False
        a = sym.attrs
        if (a.carry_return or a.address is not None or a.section is not None
                or any(z is not None for z in a.param_zeropage)
                or a.noreturn):
            return False
        decl = self.decls.get(fn.name)
        if decl is None or decl.abi_annotation is not None:
            return False
        if decl.noreturn or _has_register_request(decl):
            return False
        ftype = sym.type
        if not isinstance(ftype, c99_ast.FunType):
            return False
        if not (isinstance(_unqualified(ftype.ret), c99_ast.Void)
                or _is_scalar(ftype.ret)):
            return False
        if not all(_is_scalar(p) for p in ftype.params):
            return False
        if len(ftype.params) != len(fn.params):
            return False
        for p in fn.params:
            psym = self.symbols.get(p)
            if psym is None or not isinstance(psym.attrs, LocalAttr):
                return False
            if psym.attrs.register:
                return False
        for ins in fn.instructions:
            if isinstance(ins, tac_ast.IndirectCall):
                return False
            if isinstance(ins, tac_ast.FunctionCall) and ins.hw_stack_save:
                return False
            if (isinstance(ins, tac_ast.GetAddress)
                    and isinstance(ins.operand, tac_ast.Var)):
                osym = self.symbols.get(ins.operand.name)
                if osym is None or isinstance(osym.attrs, LocalAttr):
                    return False
            for v in _vals(ins):
                if not isinstance(v, tac_ast.Var):
                    continue
                vsym = self.symbols.get(v.name)
                if vsym is None:
                    return False
                if (isinstance(vsym.attrs, StaticAttr)
                        and v.name.startswith("@")):
                    return False
                if isinstance(vsym.attrs, LocalAttr) and vsym.attrs.register:
                    return False
            for name in _named_objects(ins):
                nsym = self.symbols.get(name)
                if nsym is None or isinstance(nsym.attrs, LocalAttr):
                    return False
                if isinstance(nsym.attrs, StaticAttr) and name.startswith("@"):
                    return False
        return True

    def _site_ok(self, caller: tac_ast.Function, call, callee) -> bool:
        if call.hw_stack_save:
            return False
        if len(call.args) != len(callee.params):
            return False
        for arg, p in zip(call.args, callee.params):
            want = _shape(self.symbols[p].type)
            got = _val_shape(arg, self.symbols)
            if want is None or got != want:
                return False
        if call.dst is not None:
            ret = self.symbols[callee.name].type.ret
            if _shape(ret) is None or _val_shape(
                    call.dst, self.symbols) != _shape(ret):
                return False
        return True

    def _caller_ok(self, caller: tac_ast.Function) -> bool:
        decl = self.decls.get(caller.name)
        return decl is None or not _has_register_local(decl.body)

    def run(self) -> tac_ast.Program:
        taken = _address_taken(self.tac, set(self.decls))
        recursive = self._recursive()
        candidates = {
            name for name, fn in self.functions.items()
            if self._candidate(fn, taken, recursive)
        }
        if not candidates:
            return self.tac
        dropped: set[str] = set()
        done: set[str] = set()
        # Leaves first: a candidate is taken once every candidate it
        # calls has been taken, which the absence of recursion makes
        # possible for all of them.
        while True:
            ready = sorted(
                c for c in candidates - done
                if not (set(_calls(self.functions[c])) & (candidates - done))
            )
            if not ready:
                break
            for name in ready:
                done.add(name)
                if self._inline_all(name):
                    dropped.add(name)
        if not dropped and all(
            fn is self._original(fn.name) for fn in self.functions.values()
        ):
            return self.tac
        top = []
        for tl in self.tac.top_level:
            if isinstance(tl, tac_ast.Function):
                if tl.name in dropped:
                    continue
                top.append(self.functions[tl.name])
            else:
                top.append(tl)
        return tac_ast.Program(top_level=top)

    def _original(self, name):
        for tl in self.tac.top_level:
            if isinstance(tl, tac_ast.Function) and tl.name == name:
                return tl
        return None

    def _inline_all(self, name: str) -> bool:
        """Inlines every call of `name` when the choice rule (module
        docstring) allows it, and returns True when no call of it is
        left, so that it can be dropped."""
        callee = self.functions[name]
        sites = 0
        tails = 0
        ok = True
        for caller in self.functions.values():
            if caller.name == name:
                continue
            body = caller.instructions
            for idx, ins in enumerate(body):
                if isinstance(ins, tac_ast.FunctionCall) and ins.name == name:
                    sites += 1
                    nxt = body[idx + 1] if idx + 1 < len(body) else None
                    if _returns_result(ins, nxt):
                        tails += 1
                    if not (self._caller_ok(caller)
                            and self._site_ok(caller, ins, callee)):
                        ok = False
        if sites == 0 or not ok:
            return False
        if not self._pays(callee, sites, tails):
            return False
        for cname in list(self.functions):
            caller = self.functions[cname]
            if cname == name or not any(
                isinstance(i, tac_ast.FunctionCall) and i.name == name
                for i in caller.instructions
            ):
                continue
            out: list[tac_ast.Type_instruction] = []
            body = caller.instructions
            idx = 0
            while idx < len(body):
                ins = body[idx]
                idx += 1
                if not (isinstance(ins, tac_ast.FunctionCall)
                        and ins.name == name):
                    out.append(ins)
                    continue
                nxt = body[idx] if idx < len(body) else None
                if _returns_result(ins, nxt):
                    out.extend(self._copy(callee, ins, tail=nxt))
                    idx += 1
                else:
                    out.extend(self._copy(callee, ins))
            self.functions[cname] = tac_ast.Function(
                name=caller.name, is_global=caller.is_global,
                params=list(caller.params), instructions=out,
            )
        return True

    def _pays(self, callee: tac_ast.Function, sites: int,
              tails: int) -> bool:
        """True when inlining all `sites` calls of `callee`, `tails` of
        them in tail position, is estimated to take no more bytes than
        the calls and the function (module docstring)."""
        ftype = self.symbols[callee.name].type
        arg_bytes = sum(_shape(t)[0] for t in ftype.params)
        ret = _shape(ftype.ret)
        call = (CALL_BYTES + ARGUMENT_BYTE_COST * arg_bytes
                + RESULT_BYTE_COST * (ret[0] if ret else 0))
        early = sum(
            1 for idx, ins in enumerate(callee.instructions)
            if isinstance(ins, tac_ast.Ret)
            and idx != len(callee.instructions) - 1
        )
        added = EARLY_RETURN_BYTES * early * (sites - tails)
        if sites > 1:
            added += (sites - 1) * BYTES_PER_COST * self._optimized_cost(
                callee)
        return added <= sites * call + 1

    def _optimized_cost(self, callee: tac_ast.Function) -> int:
        """`inline_cost` of a copy of `callee` after the TAC optimizer,
        which drops what the unoptimized TAC carries and an inlined
        copy wouldn't keep either. The copy's new names go in a copy of
        the symbol table."""
        from passes.optimization.optimizer import optimize_function
        from passes.type_checking import SymbolTable
        table = SymbolTable()
        for k, v in self.symbols.items():
            table[k] = v
        opt = optimize_function(copy.deepcopy(callee), symbols=table)
        return inline_cost(opt, table)

    # -- copying ----------------------------------------------------------

    def _fresh_local(self, name: str) -> str:
        m = _AT_NAME.match(name)
        if m:
            new = f"@{self.next_at}.{m.group(2)}"
            self.next_at += 1
        elif _TEMP_NAME.match(name):
            new = f"%{self.next_temp}"
            self.next_temp += 1
        else:
            new = f"%{self.next_temp}"
            self.next_temp += 1
        assert new not in self.symbols, new
        old = self.symbols[name]
        self.symbols[new] = Symbol(type=old.type, attrs=old.attrs)
        return new

    def _copy(self, callee: tac_ast.Function, call, tail=None) -> list:
        """The instructions that replace `call` (module docstring).
        `tail` is the caller's `Ret` right after the call when it
        returns the call's result or nothing (`_returns_result`), and
        then the copy replaces both, and each `Ret` in it stays a
        `Ret`."""
        k = self.copies
        self.copies += 1
        prefix = f".i{k}_"
        end = f".i{k}@end"
        assert end not in self.labels, end
        locals_: dict[str, str] = {}

        def local(name: str) -> str:
            if name not in locals_:
                locals_[name] = self._fresh_local(name)
            return locals_[name]

        for p in callee.params:
            local(p)
        for ins in callee.instructions:
            for v in _vals(ins):
                if isinstance(v, tac_ast.Var):
                    sym = self.symbols.get(v.name)
                    if sym is not None and isinstance(sym.attrs, LocalAttr):
                        local(v.name)
        labels: dict[str, str] = {}
        for ins in callee.instructions:
            if isinstance(ins, tac_ast.Label):
                new = prefix + ins.name.lstrip(".")
                assert new not in self.labels, new
                labels[ins.name] = new
                self.labels.add(new)
        self.labels.add(end)

        out: list[tac_ast.Type_instruction] = [
            tac_ast.Copy(src=arg, dst=tac_ast.Var(name=locals_[p]))
            for arg, p in zip(call.args, callee.params)
        ]
        jumped = False
        body = callee.instructions
        for idx, ins in enumerate(body):
            if isinstance(ins, tac_ast.Ret) and tail is not None:
                val = None
                if tail.val is not None and ins.val is not None:
                    val = _rename(ins.val, locals_, labels)
                out.append(tac_ast.Ret(val=val))
                continue
            if isinstance(ins, tac_ast.Ret):
                if call.dst is not None and ins.val is not None:
                    out.append(tac_ast.Copy(
                        src=_rename(ins.val, locals_, labels), dst=call.dst,
                    ))
                if idx != len(body) - 1:
                    out.append(tac_ast.Jump(target=end))
                    jumped = True
                continue
            out.append(_rename(ins, locals_, labels))
        if jumped:
            out.append(tac_ast.Label(name=end))
        return out


def _returns_result(call, nxt) -> bool:
    """True when `nxt`, the instruction after `call`, is a `Ret` that
    returns the call's result, or returns nothing."""
    if not isinstance(nxt, tac_ast.Ret):
        return False
    if nxt.val is None:
        return True
    return (call.dst is not None and isinstance(nxt.val, tac_ast.Var)
            and nxt.val.name == call.dst.name)


def _named_objects(ins) -> list[str]:
    """The objects that `ins` names by a string field instead of a
    `Var`."""
    if isinstance(ins, (tac_ast.IndexedLoad, tac_ast.IndexedSymbolStore)):
        return [ins.name]
    return []


_LABEL_FIELDS = {
    tac_ast.Label: "name",
    tac_ast.Jump: "target",
    tac_ast.JumpIfTrue: "target",
    tac_ast.JumpIfFalse: "target",
    tac_ast.JumpIfCmp: "target",
    tac_ast.JumpIfMasked: "target",
}


def _rename(node, locals_: dict[str, str], labels: dict[str, str]):
    """A copy of `node` with each local in `locals_` and each label in
    `labels` renamed."""
    if isinstance(node, tac_ast.Var):
        return tac_ast.Var(name=locals_.get(node.name, node.name))
    if isinstance(node, list):
        return [_rename(x, locals_, labels) for x in node]
    if not dataclasses.is_dataclass(node):
        return node
    label_field = _LABEL_FIELDS.get(type(node))
    kwargs = {}
    for f in fields_of(node):
        value = getattr(node, f.name)
        if f.name == label_field:
            value = labels.get(value, value)
        elif (f.name == "name"
              and isinstance(node, (tac_ast.IndexedLoad,
                                    tac_ast.IndexedSymbolStore))):
            value = locals_.get(value, value)
        else:
            value = _rename(value, locals_, labels)
        kwargs[f.name] = value
    return type(node)(**kwargs)
