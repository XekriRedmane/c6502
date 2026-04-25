"""Variable resolution pass: c99_ast -> c99_ast.

Rewrites every user-written variable name to a program-unique name of
the form `@<N>.<original>`, so later passes can treat identifiers as
unambiguous without re-implementing scope. Declarations mint new
mappings; `Var` references look up existing ones.

The name scheme is deliberate: `@` and `.` are both illegal in a C
identifier, so a resolved name can never collide with anything the
user could write. The leading counter guarantees uniqueness across
the whole program, not just within one scope, which makes later
debugging easier (every `@7.x` is the same x, wherever it appears).

Errors (raised as `VariableResolutionError`):
  - declaring the same name twice in the same block (shadowing an
    outer block's name is fine — see scope semantics below)
  - referencing a name that hasn't been declared yet
  - an Assignment whose lval isn't a `Var` (e.g. `1+2 = 3`,
    `-a = 5`, `(a = b) = c`). The grammar intentionally accepts
    these so a clear diagnostic can be produced here rather than a
    cryptic syntax error. Note C's richer set of lvalues
    (`*p = x`, `a[i] = x`, `s.f = x`) doesn't exist yet; when those
    land, this check widens to "is-lvalue" rather than "is-Var".

Scope semantics
---------------
Each block owns a `dict[str, tuple[str, bool]]` mapping each visible
user name to `(unique_name, inner)` where `inner` is True iff the
name was declared in *this* block. Entering a nested block clones
the parent's map and flips every entry's `inner` flag to False — so
the inner block sees the outer block's variables but knows they
weren't declared in it.

This makes the rules tiny:
  - declaration: collide only against an already-inner-scoped entry.
    An outer-scoped entry just means we're shadowing it — overwrite
    with a fresh unique name and flag the new entry as inner.
  - reference: `scope[name][0]` is the unique name to use, regardless
    of whether it's inner or outer.
  - exit: the inner scope dict goes out of Python scope and is
    discarded; the outer map is untouched (we cloned it, not aliased
    it).

So `int a; { int a; }` resolves the inner `a` to a fresh `@N.a`
distinct from the outer one; `int a; int a;` (same block) raises;
and `int a; { a = 1; }` resolves the inner-block reference to the
outer `a`'s unique name.

The pass builds a new AST rather than mutating in place. Stateless
nodes (operators, `Null`) could safely be shared, but we allocate
fresh copies for consistency so the output tree is fully independent
of the input.
"""

from __future__ import annotations

import c99_ast


# Scope value: (unique_name, inner_scoped_in_current_block).
_Scope = dict[str, tuple[str, bool]]


class VariableResolutionError(Exception):
    """Raised for duplicate declarations or uses of undeclared variables."""


class Resolver:
    """Holds the unique-name counter. One Resolver per program; each
    declaration bumps the counter. Module-level `resolve_*` wrappers
    build a fresh Resolver per call."""

    def __init__(self) -> None:
        self._counter = 0

    def make_unique(self, original: str) -> str:
        name = f"@{self._counter}.{original}"
        self._counter += 1
        return name

    def resolve_program(
        self, prog: c99_ast.Type_program,
    ) -> c99_ast.Type_program:
        match prog:
            case c99_ast.Program(function_definition=fn):
                return c99_ast.Program(
                    function_definition=self.resolve_function(fn),
                )
        raise TypeError(f"unexpected program: {prog!r}")

    def resolve_function(
        self, fn: c99_ast.Type_function_definition,
    ) -> c99_ast.Type_function_definition:
        match fn:
            case c99_ast.Function(name=name, body=body):
                # Empty parent scope — function bodies have no
                # enclosing variables today (no globals, no params).
                # `resolve_block` clones this empty dict to give the
                # body its own fresh scope. The unique-name counter
                # keeps running across functions so resolved names
                # stay globally unique.
                return c99_ast.Function(
                    name=name, body=self.resolve_block(body, {}),
                )
        raise TypeError(f"unexpected function: {fn!r}")

    def resolve_block(
        self,
        block: c99_ast.Type_block,
        parent_scope: _Scope,
    ) -> c99_ast.Type_block:
        # Entering a new block: clone the parent scope, flipping every
        # entry's inner-scoped flag to False. The resulting `local`
        # scope is what the block's items resolve in; it goes out of
        # scope (and so is discarded) when this method returns,
        # leaving `parent_scope` untouched.
        local: _Scope = {
            name: (uniq, False) for name, (uniq, _) in parent_scope.items()
        }
        match block:
            case c99_ast.Block(block_item=items):
                return c99_ast.Block(block_item=[
                    self.resolve_block_item(item, local) for item in items
                ])
        raise TypeError(f"unexpected block: {block!r}")

    def resolve_block_item(
        self,
        item: c99_ast.Type_block_item,
        scope: _Scope,
    ) -> c99_ast.Type_block_item:
        match item:
            case c99_ast.S(statement=stmt):
                return c99_ast.S(
                    statement=self.resolve_statement(stmt, scope),
                )
            case c99_ast.D(declaration=decl):
                return c99_ast.D(
                    declaration=self.resolve_declaration(decl, scope),
                )
        raise TypeError(f"unexpected block item: {item!r}")

    def resolve_declaration(
        self,
        decl: c99_ast.Type_declaration,
        scope: _Scope,
    ) -> c99_ast.Type_declaration:
        match decl:
            case c99_ast.Declaration(name=name, init=init):
                # Duplicate-check fires only when an already-inner-
                # scoped entry would be overwritten — i.e. two
                # declarations of the same name in the *same* block.
                # An outer-scoped entry is the parent's binding
                # bleeding through; declaring `name` here legally
                # shadows it.
                existing = scope.get(name)
                if existing is not None and existing[1]:
                    raise VariableResolutionError(
                        f"duplicate declaration of {name!r}"
                    )
                unique = self.make_unique(name)
                # Bind before resolving the initializer so `int a = a;`
                # resolves to the new `a` (self-initialization — UB in
                # C, but syntactically the identifier on the RHS refers
                # to the one being declared). Importantly this also
                # means a shadowing decl's initializer can *not* see
                # the outer `a` — `int a = 5; { int a = a; }` reads
                # the inner uninitialized `a`, matching C's rule.
                scope[name] = (unique, True)
                new_init = (
                    self.resolve_exp(init, scope) if init is not None else None
                )
                return c99_ast.Declaration(name=unique, init=new_init)
        raise TypeError(f"unexpected declaration: {decl!r}")

    def resolve_statement(
        self,
        stmt: c99_ast.Type_statement,
        scope: _Scope,
    ) -> c99_ast.Type_statement:
        match stmt:
            case c99_ast.Return(exp=exp):
                return c99_ast.Return(exp=self.resolve_exp(exp, scope))
            case c99_ast.Expression(exp=exp):
                return c99_ast.Expression(exp=self.resolve_exp(exp, scope))
            case c99_ast.IfStmt(
                condition=cond, then_clause=then_stmt, else_clause=else_stmt,
            ):
                # `if (cond) stmt [else stmt]` — neither branch is its
                # own block in C99 §6.8.4. Only when a branch is itself
                # a Compound statement does a new scope open, and that
                # case is handled by the Compound branch below when
                # the recursion descends into it.
                return c99_ast.IfStmt(
                    condition=self.resolve_exp(cond, scope),
                    then_clause=self.resolve_statement(then_stmt, scope),
                    else_clause=(
                        self.resolve_statement(else_stmt, scope)
                        if else_stmt is not None else None
                    ),
                )
            case c99_ast.Compound(block=block):
                # `{ ... }` opens a new lexical scope (C99 §6.8.3).
                # `resolve_block` does the clone-and-flag-outer dance.
                return c99_ast.Compound(
                    block=self.resolve_block(block, scope),
                )
            case c99_ast.Goto(label=label):
                # Labels live in their own namespace — variable
                # resolution doesn't touch them. label_resolution
                # owns the validity / uniqueness check.
                return c99_ast.Goto(label=label)
            case c99_ast.LabeledStmt(label=label, statement=inner):
                return c99_ast.LabeledStmt(
                    label=label,
                    statement=self.resolve_statement(inner, scope),
                )
            case c99_ast.BreakStmt(label=label):
                # Loop labels live in their own namespace and are minted
                # by the loop_labeling pass — pass through here.
                return c99_ast.BreakStmt(label=label)
            case c99_ast.ContinueStmt(label=label):
                return c99_ast.ContinueStmt(label=label)
            case c99_ast.WhileStmt(
                condition=cond, body=body, label=label,
            ):
                # `while (cond) body` doesn't introduce its own
                # variable scope (no place to declare in the header).
                # If `body` is a Compound, that opens its own scope via
                # the Compound branch — same story as IfStmt.
                return c99_ast.WhileStmt(
                    condition=self.resolve_exp(cond, scope),
                    body=self.resolve_statement(body, scope),
                    label=label,
                )
            case c99_ast.DoWhileStmt(
                body=body, condition=cond, label=label,
            ):
                return c99_ast.DoWhileStmt(
                    body=self.resolve_statement(body, scope),
                    condition=self.resolve_exp(cond, scope),
                    label=label,
                )
            case c99_ast.ForStmt(
                init=init, condition=cond, post_clause=post,
                body=body, label=label,
            ):
                # C99 §6.8.5.3: the for-header opens its own block-
                # scope, and the controlling expression, post-iteration
                # expression, and body all live in that scope. So a
                # `for (int a = 1; ...; ...) ...` shadows any outer
                # `a` for the duration of the loop, and the inner `a`
                # is visible in the condition / post / body.
                #
                # Mechanics match Compound: clone the parent scope and
                # flip all entries to outer-scoped so a header
                # declaration of an outer name is allowed (legal
                # shadow), then resolve init/cond/post/body all in
                # that cloned scope. The body's own scope (if it is a
                # Compound) opens via the Compound branch.
                for_scope: _Scope = {
                    n: (u, False) for n, (u, _) in scope.items()
                }
                new_init = self.resolve_for_init(init, for_scope)
                new_cond = (
                    self.resolve_exp(cond, for_scope)
                    if cond is not None else None
                )
                new_post = (
                    self.resolve_exp(post, for_scope)
                    if post is not None else None
                )
                new_body = self.resolve_statement(body, for_scope)
                return c99_ast.ForStmt(
                    init=new_init,
                    condition=new_cond,
                    post_clause=new_post,
                    body=new_body,
                    label=label,
                )
            case c99_ast.Null():
                return c99_ast.Null()
        raise TypeError(f"unexpected statement: {stmt!r}")

    def resolve_for_init(
        self,
        init: c99_ast.Type_for_init,
        scope: _Scope,
    ) -> c99_ast.Type_for_init:
        # InitDecl runs through the same resolve_declaration as a top-
        # level declaration so duplicate-decl and shadowing rules apply
        # uniformly. InitExp is just an optional expression, evaluated
        # in the for-header scope so any prior outer name is visible.
        match init:
            case c99_ast.InitDecl(declaration=decl):
                return c99_ast.InitDecl(
                    declaration=self.resolve_declaration(decl, scope),
                )
            case c99_ast.InitExp(exp=exp):
                return c99_ast.InitExp(
                    exp=self.resolve_exp(exp, scope) if exp is not None else None,
                )
        raise TypeError(f"unexpected for_init: {init!r}")

    def resolve_exp(
        self, exp: c99_ast.Type_exp, scope: _Scope,
    ) -> c99_ast.Type_exp:
        match exp:
            case c99_ast.Constant(value=v):
                return c99_ast.Constant(value=v)
            case c99_ast.Var(name=name):
                if name not in scope:
                    raise VariableResolutionError(
                        f"undeclared identifier {name!r}"
                    )
                return c99_ast.Var(name=scope[name][0])
            case c99_ast.Unary(op=op, exp=inner):
                return c99_ast.Unary(
                    op=op, exp=self.resolve_exp(inner, scope),
                )
            case c99_ast.Binary(op=op, left=left, right=right):
                return c99_ast.Binary(
                    op=op,
                    left=self.resolve_exp(left, scope),
                    right=self.resolve_exp(right, scope),
                )
            case c99_ast.Assignment(lval=lval, rval=rval):
                # The grammar accepts any expression on the LHS so we
                # can produce a clear diagnostic here. Today the only
                # legal lval is a plain identifier — when pointer
                # deref / array index / struct field land, widen this
                # check. Checked pre-resolution: resolution preserves
                # node shape, so the check would be equivalent either
                # way, but checking first avoids the pointless descent
                # into an invalid LHS.
                if not isinstance(lval, c99_ast.Var):
                    raise VariableResolutionError(
                        f"invalid lvalue in assignment: {lval!r}"
                    )
                return c99_ast.Assignment(
                    lval=self.resolve_exp(lval, scope),
                    rval=self.resolve_exp(rval, scope),
                )
            case c99_ast.Conditional(
                condition=cond,
                true_clause=true_clause,
                false_clause=false_clause,
            ):
                return c99_ast.Conditional(
                    condition=self.resolve_exp(cond, scope),
                    true_clause=self.resolve_exp(true_clause, scope),
                    false_clause=self.resolve_exp(false_clause, scope),
                )
            case c99_ast.Postfix(op=op, operand=operand):
                # Same lvalue rule as Assignment: postfix `a++` /
                # `a--` mutates its operand, so the operand has to
                # name a storage location. Prefix `++a` / `--a` is
                # already desugared to an Assignment by the parser,
                # so the Assignment branch above catches its lvalue
                # check.
                if not isinstance(operand, c99_ast.Var):
                    raise VariableResolutionError(
                        f"invalid lvalue in postfix: {operand!r}"
                    )
                return c99_ast.Postfix(
                    op=op, operand=self.resolve_exp(operand, scope),
                )
        raise TypeError(f"unexpected exp: {exp!r}")


def resolve_program(prog: c99_ast.Type_program) -> c99_ast.Type_program:
    return Resolver().resolve_program(prog)


def resolve_function(
    fn: c99_ast.Type_function_definition,
) -> c99_ast.Type_function_definition:
    return Resolver().resolve_function(fn)
