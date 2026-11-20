"""Records which functions each pass of the peephole fixed point left
unchanged, so that the next run of the pass on the same function can
be skipped.

`compile._peephole_fixedpoint` runs about 55 passes over the whole
program and repeats the sweep until a sweep changes nothing. Most
runs of most passes leave most functions as they were, and the last
sweep leaves every function as it was. A pass's rewrite of one
function is a deterministic computation whose inputs are the
function, the pass's keyword arguments, and, for some passes, a value
the pass computes from the whole program first (the function's
*context*, such as the names no pointer reaches). When the pass left
a function unchanged, the same pass on the same function with an
equal context leaves it unchanged again, so the result is known
without running the pass.

`FunctionMemo` holds that record for one pass. The fixed point keeps
one per pass in a `PeepholeMemo`, which `compile.optimized_asm`
shares among its five fixed points, since the passes there have the
same keyword arguments.

Two functions count as the same (`same_function`) when they have the
same name, `is_global`, and params, and their instruction lists hold
the same instruction objects in the same order. Equality by value
would not be enough, because a pass may read the identity of an
instruction: `memory_value_propagation` numbers the writes of X and Y
by `id(instr)`, so one object at two places in a list and two equal
objects there give it different inputs. The passes never change an
instruction or an instruction list in place (each builds new lists,
which is also what the fixed point's `new_prog == prog` test relies
on), so the same objects stay the same values.

The record for a function is valid while these hold, and nothing
else is read.

- The function's instructions are the same objects as when the pass
  last left it unchanged.
- The context the pass passes in is equal to the context then. A
  pass that reads nothing beyond the function passes None.
- The pass's keyword arguments are equal to those of the run that
  made the record. `PeepholeMemo.for_arguments` drops every record
  when they differ.
- The rest of the compiler's state that the passes read (the runtime
  layout, the user zero-page globals in `asm_aliasing`) is fixed for
  the whole compile, and a `PeepholeMemo` lives for one compile."""

from __future__ import annotations

import operator

import asm_ast


def same_function(a: asm_ast.Function, b: asm_ast.Function) -> bool:
    """Returns True if `a` and `b` have the same name, `is_global`, and
    params, and hold the same instruction objects in the same order.
    This takes one identity test per instruction, since the passes
    pass the objects of an unchanged instruction through."""
    if a is b:
        return True
    if (a.name != b.name or a.is_global != b.is_global
            or a.params != b.params):
        return False
    ai, bi = a.instructions, b.instructions
    return ai is bi or (
        len(ai) == len(bi) and all(map(operator.is_, ai, bi))
    )


class FunctionMemo:
    """Represents, for one pass, the functions it last left unchanged,
    each with the context it read then."""

    def __init__(self) -> None:
        # Function name -> (the function, its context).
        self._entries: dict[str, tuple[asm_ast.Function, object]] = {}
        self._children: dict[str, FunctionMemo] = {}
        self._last_context: object = None

    def child(self, stage: str) -> FunctionMemo:
        """Returns the memo of one stage of a pass that rewrites each
        function in more than one stage, with a context of its own
        for each (`adc_commute`)."""
        memo = self._children.get(stage)
        if memo is None:
            memo = self._children[stage] = FunctionMemo()
        return memo

    def context(self, ctx: object) -> object:
        """Returns the context object an earlier run recorded if it's
        equal to `ctx`, and `ctx` otherwise. A pass calls this once
        per run so that the test of each function's context is an
        identity test."""
        last = self._last_context
        if last is not None and (last is ctx or last == ctx):
            return last
        self._last_context = ctx
        return ctx

    def unchanged(self, fn: asm_ast.Function, ctx: object = None) -> bool:
        """Returns True if the pass left a function that's the same as
        `fn` unchanged, with a context equal to `ctx`."""
        entry = self._entries.get(fn.name)
        return (
            entry is not None
            and same_function(entry[0], fn)
            and (entry[1] is ctx or entry[1] == ctx)
        )

    def record(
        self, before: asm_ast.Function, after: asm_ast.Function,
        ctx: object = None,
    ) -> asm_ast.Function:
        """Records the pass's result `after` for `before` with context
        `ctx`, and returns the function to put in the program. That's
        `before` when the two are the same, so that the passes after
        this one find the object their own records hold."""
        if same_function(before, after):
            self._entries[before.name] = (before, ctx)
            return before
        self._entries.pop(before.name, None)
        return after

    def forget(self, fn: asm_ast.Function) -> None:
        """Drops the record of the function named `fn.name`."""
        self._entries.pop(fn.name, None)


def rewrite_function(
    memo: FunctionMemo | None, fn: asm_ast.Function, ctx: object,
    rewrite,
) -> asm_ast.Function:
    """Returns `rewrite(fn)`, which a pass computes from `fn` and the
    context `ctx` alone. With a `memo` that records `fn` unchanged
    under `ctx`, it returns `fn` without calling `rewrite`, and
    otherwise it records the result."""
    if memo is None:
        return rewrite(fn)
    if memo.unchanged(fn, ctx):
        return fn
    return memo.record(fn, rewrite(fn), ctx)


def rewrite_numbered(
    memo: FunctionMemo | None, fn: asm_ast.Function, counter: int,
    rewrite,
) -> tuple[asm_ast.Function, int]:
    """Returns `rewrite(fn, counter)`, a function and the next label
    number, for a pass that numbers the labels it adds across the
    program. The counter is the context. A run counts as leaving
    `fn` unchanged only when it returned `counter` too, so a skipped
    run returns `(fn, counter)`, as the run would have."""
    if memo is None:
        return rewrite(fn, counter)
    if memo.unchanged(fn, counter):
        return fn, counter
    new, after = rewrite(fn, counter)
    if after == counter:
        return memo.record(fn, new, counter), after
    memo.forget(fn)
    return new, after


class PeepholeMemo:
    """Holds a `FunctionMemo` for each pass of the peephole fixed
    point, for the keyword arguments it was made with."""

    def __init__(self) -> None:
        self._arguments: object = None
        self._memos: dict[str, FunctionMemo] = {}

    def for_arguments(self, arguments: object) -> PeepholeMemo:
        """Drops every record unless `arguments` (the fixed point's
        keyword arguments, copied) equals those of the earlier runs,
        and returns this memo."""
        if self._arguments is None or self._arguments != arguments:
            self._arguments = arguments
            self._memos = {}
        return self

    def of(self, name: str) -> FunctionMemo:
        """Returns the memo of the pass called `name`."""
        memo = self._memos.get(name)
        if memo is None:
            memo = self._memos[name] = FunctionMemo()
        return memo
