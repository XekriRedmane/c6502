"""Loop / switch labeling pass: c99_ast -> c99_ast.

Mints a unique label for each iteration statement (`while`, `do-while`,
`for`) and each `switch`, stamps it onto the statement's `label` field,
and stamps the matching label onto every `break` / `continue` whose
target is that statement. Also collects the case / default labels of
each switch into the SwitchStmt's `cases` / `default_label` fields so
the TAC translator can emit the dispatch chain without re-walking the
body.

Per C99 §6.8.6.3 a `break` targets the innermost enclosing iteration
*or* switch statement; per §6.8.6.2 a `continue` only targets an
iteration statement. So we thread two pieces of state through the
walk:

  current_loop          — innermost iteration statement's label, or
                          None. Used to resolve `continue`.
  current_break_target  — innermost iteration *or* switch statement's
                          label, or None. Used to resolve `break`.

Iteration statements push to BOTH; switch pushes only to
`current_break_target` (so a `continue` inside a switch inside a loop
still finds the loop). The third bit of state is `current_switch`,
the innermost enclosing switch's collector, used to attach case /
default labels to their owning switch (Duff's-device-style nesting:
case labels can sit inside if / loop / compound bodies, all of which
preserve `current_switch`; only a nested switch starts a new
collector for ITS body).

Errors (`LoopLabelingError`):
  - `break;` outside any iteration / switch statement
  - `continue;` outside any iteration statement (including inside
    a switch with no enclosing loop)
  - `case <e>:` outside any switch
  - `default:` outside any switch
  - duplicate `default:` within a single switch (structural — not a
    case-value-uniqueness check; that's the type checker's job)

Naming scheme:
  - iteration statement labels: `.loop@<N>`
  - switch statement labels:    `.switch@<N>`
  - per-case labels:            `.case@<N>`
  - per-default labels:         `.default@<N>`

All three forms share the program-wide `<N>` counter, so labels stay
globally unique. The leading `.` makes them dasm local labels (scoped
to the SUBROUTINE, so two functions can each have a `.loop@0` without
colliding); the `@` separator (illegal in a C identifier) keeps them
disjoint from any user-written goto / labeled-statement (which the
label_resolution pass mangles to `.<funcname>@<orig>` — same `@`
property, but the part after `@` is a C identifier, not digits, so
the two forms can't ever match).

Codegen derives concrete control-flow labels for iteration statements
by appending suffixes (`_start`, `_continue`, `_break`) to the loop's
base label. Switches use only `_break` (the dispatch chain emits the
case / default labels directly).

This pass runs after `label_resolution`. Loop / switch / case /
default labels are translator-minted, not user-written, so they slot
in only once user-defined goto / labeled-statement names have already
been resolved — keeping the two label namespaces disjoint by walk
order as well as by spelling.

Like the other resolution passes, this one builds a new AST rather
than mutating in place.
"""

from __future__ import annotations

from dataclasses import dataclass, field

import c99_ast


class LoopLabelingError(Exception):
    """Raised for `break` / `continue` outside the right enclosing
    statement, or `case` / `default` outside any switch, or a
    duplicate `default` within a single switch."""


@dataclass
class _SwitchCollector:
    """Per-switch case-collection state. The labeling pass minted
    `label` (the switch's base `.switch@<N>`) when it entered the
    switch; `cases` and `default_label` accumulate as the pass
    descends into the body and finds CaseStmt / DefaultStmt nodes
    whose owning switch is this one (i.e. not a nested switch)."""
    label: str
    cases: list[c99_ast.Type_switch_case] = field(default_factory=list)
    default_label: str | None = None


@dataclass
class _LabelState:
    """State threaded through the labeling walk. Independent fields
    rather than a stack because each statement type updates exactly
    the fields it owns; copying the dataclass on push is one line."""
    current_loop: str | None = None
    current_break_target: str | None = None
    current_switch: _SwitchCollector | None = None


class LoopLabeler:
    """Holds the per-program label counter. Module-level wrappers
    (`label_program`, `label_function`) build a fresh LoopLabeler per
    call so the counter starts at 0 for each invocation."""

    def __init__(self) -> None:
        self._counter = 0

    def _make_label(self, prefix: str) -> str:
        name = f".{prefix}@{self._counter}"
        self._counter += 1
        return name

    def make_label(self) -> str:
        # Backwards-compatible name used by tests; mints a loop label.
        return self._make_label("loop")

    def label_program(
        self, prog: c99_ast.Type_program,
    ) -> c99_ast.Type_program:
        match prog:
            case c99_ast.Program(declaration=decls):
                # Walk each top-level declaration in turn; the per-
                # program counter keeps loop labels globally unique
                # across functions. Variable declarations and body-less
                # function declarations pass through verbatim — they
                # don't host iteration statements.
                return c99_ast.Program(declaration=[
                    self._label_declaration(d) for d in decls
                ])
        raise TypeError(f"unexpected program: {prog!r}")

    def _label_declaration(
        self, decl: c99_ast.Type_declaration,
    ) -> c99_ast.Type_declaration:
        match decl:
            case c99_ast.VarDecl() | c99_ast.StructDecl():
                return decl
            case c99_ast.FunctionDecl(function_decl=fd):
                if fd.body is None:
                    return decl
                return c99_ast.FunctionDecl(
                    function_decl=self._label_function_decl(fd),
                )
        raise TypeError(f"unexpected declaration: {decl!r}")

    def _label_function_decl(
        self, fd: c99_ast.Type_function_decl,
    ) -> c99_ast.Type_function_decl:
        # Function bodies start outside any loop or switch: a top-
        # level `break;` / `continue;` / `case` / `default` is an
        # error. Params don't host any of these — they pass through
        # verbatim.
        assert fd.body is not None
        return c99_ast.Type_function_decl(
            name=fd.name,
            params=list(fd.params),
            body=self.label_block(fd.body, _LabelState()),
            data_type=fd.data_type,
            storage_class=fd.storage_class,
        )

    def label_function(
        self, fn: c99_ast.Type_function_definition,
    ) -> c99_ast.Type_function_definition:
        # Test-convenience entry point — accepts the legacy
        # `Function(...)` shape and unwraps. See `label_function` at
        # module scope for the public version.
        match fn:
            case c99_ast.Function(name=name, params=params, body=body):
                ftype = c99_ast.FunType(
                    params=[c99_ast.Int() for _ in params],
                    ret=c99_ast.Int(),
                )
                fd = c99_ast.Type_function_decl(
                    name=name,
                    params=list(params),
                    body=body,
                    data_type=ftype,
                    storage_class=None,
                )
                new_fd = self._label_function_decl(fd)
                return c99_ast.Function(
                    name=new_fd.name,
                    params=list(new_fd.params),
                    body=new_fd.body,
                )
        raise TypeError(f"unexpected function: {fn!r}")

    def label_block(
        self,
        block: c99_ast.Type_block,
        state: _LabelState,
    ) -> c99_ast.Type_block:
        match block:
            case c99_ast.Block(block_item=items):
                return c99_ast.Block(block_item=[
                    self.label_block_item(item, state)
                    for item in items
                ])
        raise TypeError(f"unexpected block: {block!r}")

    def label_block_item(
        self,
        item: c99_ast.Type_block_item,
        state: _LabelState,
    ) -> c99_ast.Type_block_item:
        match item:
            case c99_ast.S(statement=stmt):
                return c99_ast.S(
                    statement=self.label_statement(stmt, state),
                )
            case c99_ast.D():
                # Declarations don't host break/continue or loops, so
                # nothing to label inside them.
                return item
        raise TypeError(f"unexpected block item: {item!r}")

    def label_statement(
        self,
        stmt: c99_ast.Type_statement,
        state: _LabelState,
    ) -> c99_ast.Type_statement:
        match stmt:
            case c99_ast.BreakStmt():
                # `break` targets the innermost enclosing iteration OR
                # switch statement (C99 §6.8.6.3).
                if state.current_break_target is None:
                    raise LoopLabelingError(
                        "'break' statement not inside a loop or switch"
                    )
                return c99_ast.BreakStmt(
                    label=state.current_break_target,
                )
            case c99_ast.ContinueStmt():
                # `continue` targets the innermost enclosing iteration
                # statement only (C99 §6.8.6.2). A `continue` inside a
                # switch but outside any loop is an error — switches
                # don't push current_loop.
                if state.current_loop is None:
                    raise LoopLabelingError(
                        "'continue' statement not inside a loop"
                    )
                return c99_ast.ContinueStmt(label=state.current_loop)
            case c99_ast.WhileStmt(condition=cond, body=body):
                # Each iteration statement pushes its own label as both
                # current_loop and current_break_target for the
                # duration of its body. current_switch threads through
                # unchanged — case labels are still owned by the
                # surrounding switch (Duff's device).
                lbl = self._make_label("loop")
                inner = _LabelState(
                    current_loop=lbl,
                    current_break_target=lbl,
                    current_switch=state.current_switch,
                )
                return c99_ast.WhileStmt(
                    condition=cond,
                    body=self.label_statement(body, inner),
                    label=lbl,
                )
            case c99_ast.DoWhileStmt(body=body, condition=cond):
                lbl = self._make_label("loop")
                inner = _LabelState(
                    current_loop=lbl,
                    current_break_target=lbl,
                    current_switch=state.current_switch,
                )
                return c99_ast.DoWhileStmt(
                    body=self.label_statement(body, inner),
                    condition=cond,
                    label=lbl,
                )
            case c99_ast.ForStmt(
                init=init, condition=cond, post_clause=post,
                body=body,
            ):
                # The header (init / condition / post_clause) is built
                # of expressions and a possible declaration — none of
                # which can contain break/continue/case/default — so
                # it passes through unchanged. Only the body picks up
                # the new current-loop / break-target.
                lbl = self._make_label("loop")
                inner = _LabelState(
                    current_loop=lbl,
                    current_break_target=lbl,
                    current_switch=state.current_switch,
                )
                return c99_ast.ForStmt(
                    init=init,
                    condition=cond,
                    post_clause=post,
                    body=self.label_statement(body, inner),
                    label=lbl,
                )
            case c99_ast.SwitchStmt(
                control=control, body=body,
                promoted_type=promoted_type,
            ):
                # Switch pushes a fresh break target (its own label)
                # and a fresh case-collector for ITS body. current_loop
                # threads through unchanged so a `continue` inside the
                # switch but inside an enclosing loop still finds that
                # loop. The body walk fills in the collector via the
                # CaseStmt / DefaultStmt cases below.
                lbl = self._make_label("switch")
                collector = _SwitchCollector(label=lbl)
                inner = _LabelState(
                    current_loop=state.current_loop,
                    current_break_target=lbl,
                    current_switch=collector,
                )
                new_body = self.label_statement(body, inner)
                return c99_ast.SwitchStmt(
                    control=control,
                    body=new_body,
                    label=lbl,
                    cases=collector.cases,
                    default_label=collector.default_label,
                    promoted_type=promoted_type,
                )
            case c99_ast.CaseStmt(value=value, body=body):
                # `case` is a labeled-statement form that's only legal
                # inside a switch (§6.8.1.2). The owning switch is
                # current_switch — which can sit several levels up the
                # walk, since case can hide inside if / loop /
                # compound bodies (Duff's device).
                if state.current_switch is None:
                    raise LoopLabelingError(
                        "'case' label not within a switch statement"
                    )
                case_label = self._make_label("case")
                state.current_switch.cases.append(
                    c99_ast.Type_switch_case(
                        value=value, label=case_label,
                    )
                )
                # Recurse into the inner statement with the same state
                # — case labels don't open a scope, change the break
                # target, or anything else.
                return c99_ast.CaseStmt(
                    value=value,
                    body=self.label_statement(body, state),
                    label=case_label,
                )
            case c99_ast.DefaultStmt(body=body):
                if state.current_switch is None:
                    raise LoopLabelingError(
                        "'default' label not within a switch statement"
                    )
                if state.current_switch.default_label is not None:
                    raise LoopLabelingError(
                        "multiple default labels in one switch"
                    )
                default_label = self._make_label("default")
                state.current_switch.default_label = default_label
                return c99_ast.DefaultStmt(
                    body=self.label_statement(body, state),
                    label=default_label,
                )
            case c99_ast.IfStmt(
                condition=cond, then_clause=then, else_clause=else_,
            ):
                # `if` doesn't change loop / switch / break scoping —
                # break/continue/case/default in either branch still
                # target whatever encloses the `if`.
                return c99_ast.IfStmt(
                    condition=cond,
                    then_clause=self.label_statement(then, state),
                    else_clause=(
                        self.label_statement(else_, state)
                        if else_ is not None else None
                    ),
                )
            case c99_ast.Compound(block=block):
                # A compound inside a loop / switch is still inside it
                # — pass `state` through. A compound at the function
                # top level just keeps the empty state, which makes
                # any break/continue/case/default inside it an error.
                return c99_ast.Compound(
                    block=self.label_block(block, state),
                )
            case c99_ast.LabeledStmt(label=label, statement=inner):
                # A `label: <stmt>` doesn't affect scoping — the inner
                # statement still sees the enclosing loop / switch.
                # The goto-label string is rewritten by
                # label_resolution, not here.
                return c99_ast.LabeledStmt(
                    label=label,
                    statement=self.label_statement(inner, state),
                )
            case (
                c99_ast.Return()
                | c99_ast.Expression()
                | c99_ast.Goto()
                | c99_ast.Null()
            ):
                # No nested statements that could contain break /
                # continue / case / default; nothing to label.
                return stmt
        raise TypeError(f"unexpected statement: {stmt!r}")


def label_program(prog: c99_ast.Type_program) -> c99_ast.Type_program:
    return LoopLabeler().label_program(prog)


def label_function(
    fn: c99_ast.Type_function_definition,
) -> c99_ast.Type_function_definition:
    return LoopLabeler().label_function(fn)
