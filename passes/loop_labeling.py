"""Loop labeling pass: c99_ast -> c99_ast.

For each iteration statement (`while`, `do-while`, `for`), mint a
unique label and stamp it onto the loop's `label` field. Then walk
the loop body with that label as the *current loop*; every `break` /
`continue` we encounter there gets the same label stamped onto its
own `label` field. Nested loops push a fresh current-loop label for
the duration of their body and pop back to the enclosing loop's
label when their body ends.

Errors (`LoopLabelingError`):
  - `break;` outside any iteration statement
  - `continue;` outside any iteration statement

C99 §6.8.6.3 also lets `break` appear inside a `switch` statement,
where it targets the switch's end rather than any enclosing loop.
c6502 doesn't have `switch` yet; once it lands, the switch lowering
will keep its own break-target label that's separate from the
current loop label tracked here.

Naming scheme: each label is `.loop@<N>`. Leading `.` makes it a
dasm-style local label (scoped to the enclosing SUBROUTINE, so two
functions can each have a `.loop@0` without colliding). The `@`
separator (illegal in a C identifier) means loop labels can never
be confused with anything the user could write — both with goto/
labeled-stmt labels (already mangled to `.<funcname>@<orig>`, so
they share the @-marks-non-user-label property) and with anything
that survives raw from C source. The `<N>` counter is per-program
so labels stay globally unique. Codegen derives concrete control-
flow labels (loop start, continue target, break target) by
appending suffixes (`_start`, `_continue`, `_break`) to this base.

This pass runs after `label_resolution`. Loop labels are translator-
minted (not user-written), so they should slot in only once the
user-defined goto / labeled-statement names have already been
resolved — that way `label_resolution` has nothing to say about loop
labels and `loop_labeling` has nothing to say about user labels. The
two label namespaces are disjoint by construction: a user label is
`.<funcname>@<orig>` where the part after `@` is a C identifier
(starts with a letter or underscore), while a loop label is
`.loop@<N>` where the part after `@` is a string of digits — so the
two forms can't ever match the same string regardless of what the
user names their functions or labels.

Like the other resolution passes, this one builds a new AST rather
than mutating in place.
"""

from __future__ import annotations

import c99_ast


class LoopLabelingError(Exception):
    """Raised for `break` or `continue` outside any iteration statement."""


class LoopLabeler:
    """Holds the per-program label counter. Module-level wrappers
    (`label_program`, `label_function`) build a fresh LoopLabeler per
    call so the counter starts at 0 for each invocation."""

    def __init__(self) -> None:
        self._counter = 0

    def make_label(self) -> str:
        name = f".loop@{self._counter}"
        self._counter += 1
        return name

    def label_program(
        self, prog: c99_ast.Type_program,
    ) -> c99_ast.Type_program:
        match prog:
            case c99_ast.Program(function_definition=fn):
                return c99_ast.Program(
                    function_definition=self.label_function(fn),
                )
        raise TypeError(f"unexpected program: {prog!r}")

    def label_function(
        self, fn: c99_ast.Type_function_definition,
    ) -> c99_ast.Type_function_definition:
        match fn:
            case c99_ast.Function(name=name, body=body):
                # Function bodies start outside any loop: a top-level
                # `break;` / `continue;` is an error. The counter
                # keeps running across functions so labels stay
                # globally unique.
                return c99_ast.Function(
                    name=name,
                    body=self.label_block(body, current_loop=None),
                )
        raise TypeError(f"unexpected function: {fn!r}")

    def label_block(
        self,
        block: c99_ast.Type_block,
        current_loop: str | None,
    ) -> c99_ast.Type_block:
        match block:
            case c99_ast.Block(block_item=items):
                return c99_ast.Block(block_item=[
                    self.label_block_item(item, current_loop)
                    for item in items
                ])
        raise TypeError(f"unexpected block: {block!r}")

    def label_block_item(
        self,
        item: c99_ast.Type_block_item,
        current_loop: str | None,
    ) -> c99_ast.Type_block_item:
        match item:
            case c99_ast.S(statement=stmt):
                return c99_ast.S(
                    statement=self.label_statement(stmt, current_loop),
                )
            case c99_ast.D():
                # Declarations don't host break/continue or loops, so
                # nothing to label inside them.
                return item
        raise TypeError(f"unexpected block item: {item!r}")

    def label_statement(
        self,
        stmt: c99_ast.Type_statement,
        current_loop: str | None,
    ) -> c99_ast.Type_statement:
        match stmt:
            case c99_ast.BreakStmt():
                if current_loop is None:
                    raise LoopLabelingError(
                        "'break' statement not inside a loop"
                    )
                return c99_ast.BreakStmt(label=current_loop)
            case c99_ast.ContinueStmt():
                if current_loop is None:
                    raise LoopLabelingError(
                        "'continue' statement not inside a loop"
                    )
                return c99_ast.ContinueStmt(label=current_loop)
            case c99_ast.WhileStmt(condition=cond, body=body):
                # Each loop pushes its own current label for the
                # duration of its body. The condition itself can't
                # contain break/continue (it's an expression), so it
                # passes through unchanged.
                lbl = self.make_label()
                return c99_ast.WhileStmt(
                    condition=cond,
                    body=self.label_statement(body, current_loop=lbl),
                    label=lbl,
                )
            case c99_ast.DoWhileStmt(body=body, condition=cond):
                lbl = self.make_label()
                return c99_ast.DoWhileStmt(
                    body=self.label_statement(body, current_loop=lbl),
                    condition=cond,
                    label=lbl,
                )
            case c99_ast.ForStmt(
                init=init, condition=cond, post_clause=post,
                body=body,
            ):
                # The header (init / condition / post_clause) is built
                # of expressions and a possible declaration — none of
                # which can contain break/continue — so it passes
                # through unchanged. Only the body picks up the new
                # current-loop label.
                lbl = self.make_label()
                return c99_ast.ForStmt(
                    init=init,
                    condition=cond,
                    post_clause=post,
                    body=self.label_statement(body, current_loop=lbl),
                    label=lbl,
                )
            case c99_ast.IfStmt(
                condition=cond, then_clause=then, else_clause=else_,
            ):
                # `if` doesn't change loop scoping — break/continue in
                # either branch still target whatever loop encloses
                # the `if`. So we thread `current_loop` through both
                # branches unchanged.
                return c99_ast.IfStmt(
                    condition=cond,
                    then_clause=self.label_statement(then, current_loop),
                    else_clause=(
                        self.label_statement(else_, current_loop)
                        if else_ is not None else None
                    ),
                )
            case c99_ast.Compound(block=block):
                # A compound inside a loop body is still inside that
                # loop — pass `current_loop` through. A compound at
                # the function top level just keeps `current_loop` =
                # None, which makes any break/continue inside it an
                # error, as it should be.
                return c99_ast.Compound(
                    block=self.label_block(block, current_loop),
                )
            case c99_ast.LabeledStmt(label=label, statement=inner):
                # A `label: <stmt>` doesn't affect loop scoping — the
                # inner statement still sees the enclosing loop. The
                # goto-label string is rewritten by label_resolution,
                # not here.
                return c99_ast.LabeledStmt(
                    label=label,
                    statement=self.label_statement(inner, current_loop),
                )
            case (
                c99_ast.Return()
                | c99_ast.Expression()
                | c99_ast.Goto()
                | c99_ast.Null()
            ):
                # No nested statements that could contain break /
                # continue; nothing to label. We return the input
                # unchanged — these nodes are immutable for our
                # purposes (label_resolution / variable_resolution
                # have already rewritten any name fields they needed
                # to touch).
                return stmt
        raise TypeError(f"unexpected statement: {stmt!r}")


def label_program(prog: c99_ast.Type_program) -> c99_ast.Type_program:
    return LoopLabeler().label_program(prog)


def label_function(
    fn: c99_ast.Type_function_definition,
) -> c99_ast.Type_function_definition:
    return LoopLabeler().label_function(fn)
