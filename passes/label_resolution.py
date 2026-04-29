"""Label resolution pass: c99_ast -> c99_ast.

Validates and rewrites labeled statements (`label: stmt`, C99 §6.8.1)
and `goto label;` statements (C99 §6.8.6) within each function:

  - Every label declared in a function must be unique within that
    function (C99 §6.2.3 — labels live in their own namespace, but
    duplicates within the same function are forbidden).
  - Every `goto label;` must target a label that is declared somewhere
    in the enclosing function.

Both checks are per-function: a label declared in one function is not
visible from another, and `goto` cannot escape the function it appears
in (C99 §6.8.6.1: "The identifier in a goto statement shall name a
label located somewhere in the enclosing function.").

C99 §6.8.6.1 also forbids jumping into the scope of an identifier with
a *variably modified type* (i.e. a VLA-typed declaration). c6502
doesn't support VLAs (no array types at all yet), so that constraint
is vacuously satisfied — there's nothing to enforce.

Naming scheme: each declared label is rewritten to
`.<funcname>@<orig>`. The leading `.` makes it a dasm-style local
label — valid only between the enclosing `SUBROUTINE` directive and
the next one — which gives us per-function label namespaces for free
(two functions can each have a label `foo` without colliding in the
emitted asm). The `@` separator (illegal in a C identifier) keeps
user labels disjoint from any user-written identifier in the source.
Translator-generated labels also embed `@` (`.<prefix>@<N>` like
`.if_end@0`), so they share the marks-non-user-label property; the
two forms stay disjoint because the part after `@` is a C identifier
here but a digit run there.

The `<funcname>` segment isn't strictly needed for correctness
(local-label scoping already isolates per-function), but it's kept
so the emitted asm names a label by its origin function — useful
when reading codegen output.

Errors (`LabelResolutionError`):
  - declaring the same label twice in the same function
  - `goto` to a label that wasn't declared in the same function

Like identifier_resolution, this pass builds a new AST rather than
mutating in place. The pass runs after identifier_resolution; both order
and isolation are fine because labels and variables share no namespace.
"""

from __future__ import annotations

import c99_ast


class LabelResolutionError(Exception):
    """Raised for duplicate label declarations or undefined goto targets."""


class LabelResolver:
    def resolve_program(
        self, prog: c99_ast.Type_program,
    ) -> c99_ast.Type_program:
        match prog:
            case c99_ast.Program(declaration=decls):
                # The top-level shape is `Program(declaration*)` —
                # each entry is either a variable declaration (no
                # body) or a function declaration (which may or may
                # not have a body — only definitions do). We only
                # have label work to do for function bodies; non-
                # function declarations and body-less function
                # declarations pass through verbatim.
                return c99_ast.Program(declaration=[
                    self._resolve_declaration(d) for d in decls
                ])
        raise TypeError(f"unexpected program: {prog!r}")

    def _resolve_declaration(
        self, decl: c99_ast.Type_declaration,
    ) -> c99_ast.Type_declaration:
        match decl:
            case c99_ast.VarDecl():
                # Variable declarations can't host labels.
                return decl
            case c99_ast.FunctionDecl(function_decl=fd):
                if fd.body is None:
                    return decl
                return c99_ast.FunctionDecl(
                    function_decl=self._resolve_function_decl(fd),
                )
        raise TypeError(f"unexpected declaration: {decl!r}")

    def _resolve_function_decl(
        self, fd: c99_ast.Type_function_decl,
    ) -> c99_ast.Type_function_decl:
        # Pass 1: collect every labeled statement in the body, minting
        # unique names and rejecting duplicates. Pass 2: rewrite the
        # AST, validating each Goto's target along the way. Params
        # don't host labels — they pass through verbatim.
        labels: dict[str, str] = {}
        assert fd.body is not None
        self._collect_block(fd.body, fd.name, labels)
        return c99_ast.Type_function_decl(
            name=fd.name,
            params=list(fd.params),
            body=self._rewrite_block(fd.body, labels),
            data_type=fd.data_type,
            storage_class=fd.storage_class,
        )

    def resolve_function(
        self, fn: c99_ast.Type_function_definition,
    ) -> c99_ast.Type_function_definition:
        # Test-convenience entry point — accepts the legacy
        # `Function(...)` shape, threads through the new function-
        # decl path, and unwraps. See `resolve_function` at module
        # scope for the public version.
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
                new_fd = self._resolve_function_decl(fd)
                return c99_ast.Function(
                    name=new_fd.name,
                    params=list(new_fd.params),
                    body=new_fd.body,
                )
        raise TypeError(f"unexpected function: {fn!r}")

    def _collect_block(
        self,
        block: c99_ast.Type_block,
        fn_name: str,
        labels: dict[str, str],
    ) -> None:
        match block:
            case c99_ast.Block(block_item=items):
                for item in items:
                    self._collect_block_item(item, fn_name, labels)
                return
        raise TypeError(f"unexpected block: {block!r}")

    def _collect_block_item(
        self,
        item: c99_ast.Type_block_item,
        fn_name: str,
        labels: dict[str, str],
    ) -> None:
        match item:
            case c99_ast.S(statement=stmt):
                self._collect_statement(stmt, fn_name, labels)
            case c99_ast.D():
                # Declarations can't introduce labels.
                pass

    def _collect_statement(
        self,
        stmt: c99_ast.Type_statement,
        fn_name: str,
        labels: dict[str, str],
    ) -> None:
        match stmt:
            case c99_ast.LabeledStmt(label=label, statement=inner):
                if label in labels:
                    raise LabelResolutionError(
                        f"duplicate label {label!r}"
                    )
                labels[label] = f".{fn_name}@{label}"
                # The labeled statement's body might itself be a
                # labeled statement (`a: b: ;`), an if (whose branches
                # might contain labels), etc. — keep walking.
                self._collect_statement(inner, fn_name, labels)
            case c99_ast.IfStmt(
                condition=_, then_clause=then, else_clause=else_,
            ):
                self._collect_statement(then, fn_name, labels)
                if else_ is not None:
                    self._collect_statement(else_, fn_name, labels)
            case c99_ast.Compound(block=block):
                # Labels inside a `{ ... }` block are still scoped to
                # the enclosing function — descend.
                self._collect_block(block, fn_name, labels)
            case c99_ast.WhileStmt(body=body) | c99_ast.DoWhileStmt(body=body):
                self._collect_statement(body, fn_name, labels)
            case c99_ast.ForStmt(body=body):
                # The for-header (init/condition/post) can't host
                # labeled statements — only the body can.
                self._collect_statement(body, fn_name, labels)
            case c99_ast.SwitchStmt(body=body):
                # Switch bodies can contain user labels (and gotos to
                # them) just like any other statement context — descend
                # so they participate in the per-function namespace.
                # case / default labels are translator-minted, not user
                # labels, and are ignored here.
                self._collect_statement(body, fn_name, labels)
            case c99_ast.CaseStmt(body=body) | c99_ast.DefaultStmt(body=body):
                self._collect_statement(body, fn_name, labels)
            case _:
                # Return / Expression / Goto / Break / Continue / Null
                # can't contain nested statements that would introduce
                # labels.
                pass

    def _rewrite_block(
        self,
        block: c99_ast.Type_block,
        labels: dict[str, str],
    ) -> c99_ast.Type_block:
        match block:
            case c99_ast.Block(block_item=items):
                return c99_ast.Block(block_item=[
                    self._rewrite_block_item(item, labels) for item in items
                ])
        raise TypeError(f"unexpected block: {block!r}")

    def _rewrite_block_item(
        self,
        item: c99_ast.Type_block_item,
        labels: dict[str, str],
    ) -> c99_ast.Type_block_item:
        match item:
            case c99_ast.S(statement=stmt):
                return c99_ast.S(
                    statement=self._rewrite_statement(stmt, labels),
                )
            case c99_ast.D():
                return item
        raise TypeError(f"unexpected block item: {item!r}")

    def _rewrite_statement(
        self,
        stmt: c99_ast.Type_statement,
        labels: dict[str, str],
    ) -> c99_ast.Type_statement:
        match stmt:
            case c99_ast.Goto(label=label):
                if label not in labels:
                    raise LabelResolutionError(
                        f"goto to undefined label {label!r}"
                    )
                return c99_ast.Goto(label=labels[label])
            case c99_ast.LabeledStmt(label=label, statement=inner):
                # `labels[label]` was populated in pass 1 so the
                # lookup can't fail here.
                return c99_ast.LabeledStmt(
                    label=labels[label],
                    statement=self._rewrite_statement(inner, labels),
                )
            case c99_ast.IfStmt(
                condition=cond, then_clause=then, else_clause=else_,
            ):
                return c99_ast.IfStmt(
                    condition=cond,
                    then_clause=self._rewrite_statement(then, labels),
                    else_clause=(
                        self._rewrite_statement(else_, labels)
                        if else_ is not None else None
                    ),
                )
            case c99_ast.Compound(block=block):
                return c99_ast.Compound(
                    block=self._rewrite_block(block, labels),
                )
            case c99_ast.WhileStmt(
                condition=cond, body=body, label=label,
            ):
                return c99_ast.WhileStmt(
                    condition=cond,
                    body=self._rewrite_statement(body, labels),
                    label=label,
                )
            case c99_ast.DoWhileStmt(
                body=body, condition=cond, label=label,
            ):
                return c99_ast.DoWhileStmt(
                    body=self._rewrite_statement(body, labels),
                    condition=cond,
                    label=label,
                )
            case c99_ast.ForStmt(
                init=init, condition=cond, post_clause=post,
                body=body, label=label,
            ):
                return c99_ast.ForStmt(
                    init=init,
                    condition=cond,
                    post_clause=post,
                    body=self._rewrite_statement(body, labels),
                    label=label,
                )
            case c99_ast.SwitchStmt(
                control=control, body=body, label=label,
                cases=cases, default_label=default_label,
                promoted_type=promoted_type,
            ):
                return c99_ast.SwitchStmt(
                    control=control,
                    body=self._rewrite_statement(body, labels),
                    label=label,
                    cases=list(cases),
                    default_label=default_label,
                    promoted_type=promoted_type,
                )
            case c99_ast.CaseStmt(value=value, body=body, label=label):
                return c99_ast.CaseStmt(
                    value=value,
                    body=self._rewrite_statement(body, labels),
                    label=label,
                )
            case c99_ast.DefaultStmt(body=body, label=label):
                return c99_ast.DefaultStmt(
                    body=self._rewrite_statement(body, labels),
                    label=label,
                )
            case _:
                # Return / Expression / Break / Continue / Null don't
                # reference labels and have no nested statements —
                # pass through.
                return stmt


def resolve_program(prog: c99_ast.Type_program) -> c99_ast.Type_program:
    return LabelResolver().resolve_program(prog)


def resolve_function(
    fn: c99_ast.Type_function_definition,
) -> c99_ast.Type_function_definition:
    return LabelResolver().resolve_function(fn)
