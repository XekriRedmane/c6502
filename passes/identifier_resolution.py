"""Identifier resolution pass: c99_ast -> c99_ast.

Resolves every user-written identifier — variables and function names
both — to a form later passes can treat as unambiguous, and rejects
references that don't match any declaration.

Variables get program-uniquely renamed to `@<N>.<original>`, so later
passes can flatten scope without re-implementing it. Function names
are left alone: they have external linkage (C99 §6.2.2), so the
linker resolves them by their original spelling, and two declarations
of the same function name refer to the same code. If we renamed
`int foo(void); foo();`, the call would no longer resolve to the
user's actual `foo`.

The pass is named "identifier resolution" rather than "variable
resolution" because it owns *both* identifier kinds — variables get
the rename treatment, functions get the registration-and-validation
treatment, and a `FunctionCall` to an undeclared name is one of the
errors raised here. Naming it after just one half (variables) was a
hangover from when the language only had variables.

Variables in the current language all have *no* linkage (every var
declaration we accept today is a block-scope `int x;` — automatic
storage, no `extern`/`static` keyword), so renaming them program-
uniquely is safe. When file-scope or `extern`/`static` variables
land, this pass widens to skip names with external/internal linkage,
same as it already does for functions.

The unique-name scheme is `@<N>.<original>`: `@` and `.` are both
illegal in a C identifier, so a resolved name can never collide with
anything the user could write. The leading counter guarantees
uniqueness across the whole program.

Errors (raised as `IdentifierResolutionError`):
  - declaring the same *variable* name twice in the same block
    (shadowing an outer block's name is fine — see scope semantics
    below)
  - referencing a variable name that hasn't been declared yet
  - calling a function that hasn't been declared anywhere in the
    program
  - an Assignment whose lval isn't a `Var` (e.g. `1+2 = 3`,
    `-a = 5`, `(a = b) = c`). The grammar intentionally accepts
    these so a clear diagnostic can be produced here rather than a
    cryptic syntax error. Note C's richer set of lvalues
    (`*p = x`, `a[i] = x`, `s.f = x`) doesn't exist yet; when those
    land, this check widens to "is-lvalue" rather than "is-Var".

Function declarations
---------------------
A `FunctionDecl` block item registers its name in a per-program
function-name set. Multiple declarations of the same function are
legal — every one refers to the same code. Function declarations
themselves carry their identifier through unchanged; the parameter
names attached to the declaration (used by the future type-checking
pass to validate calls) also pass through verbatim. The body slot
on `function_decl` is unused at block scope (C99 forbids nested
function definitions) — the parser leaves it as None.

Function definitions (top-level `Function(name, body)`) likewise
register their name in the function-name set so `main()` can call
itself or be referenced from another function. Definitions and
declarations share one set: `int foo(void); ... int foo(void) { ...
}` is two registrations of `foo`, same as two declarations.

Duplicate *definitions* aren't this pass's problem — they're a
type-checker concern (C99 §6.9). We just collect names and reject
calls to names that nothing has declared.

Scope semantics
---------------
Each block owns a `dict[str, tuple[str, bool, Linkage]]` mapping
each visible *variable* name to `(resolved_name, inner, linkage)`
where `inner` is True iff the name was declared in *this* block and
`linkage` is the C99 §6.2.2 linkage kind for the declaration.
Entering a nested block clones the parent's map and flips every
entry's `inner` flag to False — so the inner block sees the outer
block's variables but knows they weren't declared in it. Linkage
rides along unchanged, since linkage is fixed at the declaration
site.

This makes the rules tiny:
  - declaration: collide only against an already-inner-scoped entry.
    An outer-scoped entry just means we're shadowing it — overwrite
    with a fresh entry and flag it as inner. Renaming itself is
    gated on linkage (NONE → mint `@<N>.<orig>`; INTERNAL/EXTERNAL →
    keep the source spelling so the linker can find it).
  - reference: `scope[name][0]` is the resolved name to use,
    regardless of linkage or inner/outer.
  - exit: the inner scope dict goes out of Python scope and is
    discarded; the outer map is untouched (we cloned it, not aliased
    it).

So `int a; { int a; }` resolves the inner `a` to a fresh `@N.a`
distinct from the outer one; `int a; int a;` (same block) raises;
and `int a; { a = 1; }` resolves the inner-block reference to the
outer `a`'s unique name.

Function names go in a separate per-program table (`_functions`)
keyed by source name and carrying the linkage kind. Today every
entry is `Linkage.EXTERNAL`, but the dict shape is right for
`static int foo(void);` once the storage-class specifier lands.

The pass builds a new AST rather than mutating in place. Stateless
nodes (operators, `Null`) could safely be shared, but we allocate
fresh copies for consistency so the output tree is fully independent
of the input.
"""

from __future__ import annotations

from enum import Enum

import c99_ast


class Linkage(Enum):
    """C99 §6.2.2 linkage kinds. Every declared identifier has at most
    one of these, fixed at its declaration site:

      - NONE: identifier names a unique entity each time the
        declaration is reached. Block-scope automatic variables (the
        only variables we accept today) are NONE-linkage.
      - INTERNAL: identifier denotes the same object/function within
        a translation unit. Produced by `static` at file scope (not
        yet supported).
      - EXTERNAL: identifier denotes the same object/function across
        all translation units the program is linked from. Produced by
        a function declaration / definition (always, today) or by an
        `extern` declaration (not yet supported).

    Renaming is keyed off linkage rather than "is it a function":
    NONE-linkage names are renamed to globally-unique `@<N>.<orig>`
    strings; INTERNAL and EXTERNAL names keep their original spelling
    because the linker (or later TU passes, for INTERNAL) must be able
    to find them by name. Today every variable we see is NONE and
    every function is EXTERNAL, so the behavior matches the simpler
    "rename variables, leave functions alone" rule — but storing the
    linkage explicitly lets `extern`/`static` slot in cleanly when
    they land.
    """
    NONE = "none"
    INTERNAL = "internal"
    EXTERNAL = "external"


# Scope value: (resolved_name, inner_scoped_in_current_block, linkage).
# `resolved_name` is the unique `@<N>.<orig>` for NONE-linkage entries
# and the original spelling for INTERNAL/EXTERNAL entries.
_Scope = dict[str, tuple[str, bool, Linkage]]


class IdentifierResolutionError(Exception):
    """Raised for duplicate declarations or uses of undeclared names."""


class Resolver:
    """Holds the unique-name counter and the program-wide function
    table. One Resolver per program; each *NONE-linkage* declaration
    bumps the counter, and every function declaration / definition
    registers a name with its linkage. Module-level `resolve_*`
    wrappers build a fresh Resolver per call."""

    def __init__(self) -> None:
        self._counter = 0
        # Functions visible somewhere in the program, keyed by
        # original name and carrying the linkage kind for the
        # declaration. Today every function is EXTERNAL, but the
        # dict is the right shape for `static int foo(void);` (which
        # would land here as INTERNAL). Populated by top-level
        # function definitions and by `FunctionDecl` block items;
        # used to validate `FunctionCall` targets.
        self._functions: dict[str, Linkage] = {}

    def make_unique(self, original: str) -> str:
        name = f"@{self._counter}.{original}"
        self._counter += 1
        return name

    def resolve_program(
        self, prog: c99_ast.Type_program,
    ) -> c99_ast.Type_program:
        match prog:
            case c99_ast.Program(function_definition=fns):
                # Pre-register every top-level function definition's
                # name so a call inside one function can target
                # another function defined later in the file. Without
                # this pre-pass, forward calls would fail "undeclared
                # function" because we hadn't yet visited the
                # definition. Block-scope `FunctionDecl` items are
                # registered as we encounter them during the walk.
                for fn in fns:
                    self._register_function_definition(fn)
                return c99_ast.Program(function_definition=[
                    self.resolve_function(fn) for fn in fns
                ])
        raise TypeError(f"unexpected program: {prog!r}")

    def _register_function_definition(
        self, fn: c99_ast.Type_function_definition,
    ) -> None:
        # Idempotent — duplicate registrations of the same name are
        # legal (multiple decls / decls plus a definition all refer
        # to the same external symbol). Duplicate-*definition*
        # detection is a type-checker concern, not ours. All function
        # definitions today have external linkage; once `static`
        # lands at file scope, it'll downgrade this to INTERNAL.
        match fn:
            case c99_ast.Function(name=name):
                self._functions[name] = Linkage.EXTERNAL
                return
        raise TypeError(f"unexpected function: {fn!r}")

    def resolve_function(
        self, fn: c99_ast.Type_function_definition,
    ) -> c99_ast.Type_function_definition:
        match fn:
            case c99_ast.Function(name=name, params=params, body=body):
                # C99 §6.9.1.7: parameters and the function body's
                # outermost local variables share one scope. So we
                # build the param scope here and process the body's
                # block items directly into it (no clone-flip), so a
                # body decl that reuses a param name raises duplicate-
                # decl. Compound statements *inside* the body do open
                # nested scopes via the usual resolve_block path.
                #
                # The unique-name counter keeps running across
                # functions so resolved names stay globally unique
                # and a call in one function can target a function
                # declared elsewhere.
                new_params, scope = self._resolve_params(params)
                match body:
                    case c99_ast.Block(block_item=items):
                        new_body = c99_ast.Block(block_item=[
                            self.resolve_block_item(item, scope)
                            for item in items
                        ])
                    case _:
                        raise TypeError(f"unexpected body: {body!r}")
                return c99_ast.Function(
                    name=name, params=new_params, body=new_body,
                )
        raise TypeError(f"unexpected function: {fn!r}")

    def _resolve_params(
        self, params: list[str],
    ) -> tuple[list[str], _Scope]:
        """Validate parameter-name uniqueness and rename each param to
        a fresh `@<N>.<orig>`. Returns the renamed names (in order)
        and the scope dict the params populate.

        Same shape as resolving a list of NONE-linkage variable
        declarations: each param is added to a fresh scope as
        inner=True; a duplicate raises. Used by both `FunctionDecl`
        (where the returned scope is discarded — the param scope
        dies at the end of the declarator) and by function
        definitions (where the returned scope IS the body's outer
        scope, per C99 §6.9.1.7).
        """
        scope: _Scope = {}
        renamed: list[str] = []
        for original in params:
            if original in scope and scope[original][1]:
                raise IdentifierResolutionError(
                    f"duplicate parameter name {original!r}"
                )
            unique = self.make_unique(original)
            scope[original] = (unique, True, Linkage.NONE)
            renamed.append(unique)
        return renamed, scope

    def resolve_block(
        self,
        block: c99_ast.Type_block,
        parent_scope: _Scope,
    ) -> c99_ast.Type_block:
        # Entering a new block: clone the parent scope, flipping every
        # entry's inner-scoped flag to False. Linkage tags ride along
        # unchanged — an identifier's linkage is fixed at its
        # declaration site, not by which block it's visible in. The
        # resulting `local` scope is what the block's items resolve
        # in; it goes out of scope (and so is discarded) when this
        # method returns, leaving `parent_scope` untouched.
        local: _Scope = {
            name: (resolved, False, link)
            for name, (resolved, _, link) in parent_scope.items()
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
            case c99_ast.VarDecl(var_decl=vd):
                # Block-scope `int x;` — the only variable form we
                # accept today — is automatic storage with NONE
                # linkage. When `extern` / `static` declarators land,
                # this is where the storage-class specifier maps to
                # INTERNAL or EXTERNAL and `resolve_var_decl` skips
                # the rename for non-NONE linkage.
                return c99_ast.VarDecl(
                    var_decl=self.resolve_var_decl(vd, scope, Linkage.NONE),
                )
            case c99_ast.FunctionDecl(function_decl=fd):
                # Function declarations have external linkage today —
                # the linker resolves them by their original spelling,
                # so we leave the function *name* alone and just
                # register it. Multiple declarations of the same
                # function are legal (each refers to the same external
                # symbol). Parameter names get the same treatment as
                # local variables: validate uniqueness within the
                # parameter list, mint a `@<N>.<orig>` unique name for
                # each, and replace the originals in the AST. The
                # param scope built up during this is discarded — for
                # a body-less declaration the params have nowhere to
                # be looked up. (For a definition, the param scope
                # *is* the body's outer scope; see `resolve_function`.)
                # `int a; int foo(int a);` is legal because the param
                # scope is independent of the surrounding block scope.
                self._functions[fd.name] = Linkage.EXTERNAL
                new_params, _ = self._resolve_params(fd.params)
                return c99_ast.FunctionDecl(
                    function_decl=c99_ast.Type_function_decl(
                        name=fd.name,
                        params=new_params,
                        body=fd.body,
                    ),
                )
        raise TypeError(f"unexpected declaration: {decl!r}")

    def resolve_var_decl(
        self,
        vd: c99_ast.Type_var_decl,
        scope: _Scope,
        linkage: Linkage,
    ) -> c99_ast.Type_var_decl:
        match vd:
            case c99_ast.Type_var_decl(name=name, init=init):
                # Duplicate-check fires only when an already-inner-
                # scoped entry would be overwritten — i.e. two
                # declarations of the same name in the *same* block.
                # An outer-scoped entry is the parent's binding
                # bleeding through; declaring `name` here legally
                # shadows it.
                # (When linked redeclarations land — `extern int x;`
                # twice in one block, both EXTERNAL — this rule will
                # need to permit the second one if the linkages
                # agree. Today the only declarable form is NONE-
                # linkage, so the rule stays "any same-block redecl
                # raises".)
                existing = scope.get(name)
                if existing is not None and existing[1]:
                    raise IdentifierResolutionError(
                        f"duplicate declaration of {name!r}"
                    )
                # Renaming is gated on linkage: NONE-linkage gets a
                # fresh `@<N>.<orig>`; INTERNAL/EXTERNAL keep the
                # source spelling because the linker (or later TU
                # passes, for INTERNAL) needs to find them by name.
                if linkage is Linkage.NONE:
                    resolved = self.make_unique(name)
                else:
                    resolved = name
                # Bind before resolving the initializer so `int a = a;`
                # resolves to the new `a` (self-initialization — UB in
                # C, but syntactically the identifier on the RHS refers
                # to the one being declared). Importantly this also
                # means a shadowing decl's initializer can *not* see
                # the outer `a` — `int a = 5; { int a = a; }` reads
                # the inner uninitialized `a`, matching C's rule.
                scope[name] = (resolved, True, linkage)
                new_init = (
                    self.resolve_exp(init, scope) if init is not None else None
                )
                return c99_ast.Type_var_decl(name=resolved, init=new_init)
        raise TypeError(f"unexpected var_decl: {vd!r}")

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
                    n: (resolved, False, link)
                    for n, (resolved, _, link) in scope.items()
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
        # InitDecl runs through resolve_var_decl so duplicate-decl and
        # shadowing rules apply uniformly. C99 §6.8.5 forbids function
        # declarations in for-init, and the AST reflects that — InitDecl
        # carries `var_decl`, not the wider `declaration` sum.
        # InitExp is just an optional expression, evaluated in the for-
        # header scope so any prior outer name is visible.
        match init:
            case c99_ast.InitDecl(var_decl=vd):
                return c99_ast.InitDecl(
                    var_decl=self.resolve_var_decl(vd, scope, Linkage.NONE),
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
                # Variables and functions share a single ordinary-
                # identifier namespace in C, so we look up the name
                # in both the per-block variable scope and the
                # program-wide function table. A hit in the variable
                # scope returns the renamed name; a hit in the
                # function table returns the original name unchanged
                # (its EXTERNAL linkage forbids renaming). Either way
                # the type-checking pass decides whether using the
                # name in a Var context is legal — `int foo(void);
                # return foo;` is a "function used as a variable"
                # type error, not a name-resolution error, so we let
                # it through to the right diagnostic.
                if name in scope:
                    return c99_ast.Var(name=scope[name][0])
                if name in self._functions:
                    return c99_ast.Var(name=name)
                raise IdentifierResolutionError(
                    f"undeclared identifier {name!r}"
                )
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
                    raise IdentifierResolutionError(
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
                    raise IdentifierResolutionError(
                        f"invalid lvalue in postfix: {operand!r}"
                    )
                return c99_ast.Postfix(
                    op=op, operand=self.resolve_exp(operand, scope),
                )
            case c99_ast.FunctionCall(name=name, args=args):
                # Same dual lookup as `Var`: prefer the function
                # table (the common case for legal calls), but fall
                # back to the variable scope so that `int x; x();`
                # reaches the type checker with a "variable called
                # as a function" diagnostic instead of a misleading
                # "undeclared" here. A truly undeclared name (in
                # neither namespace) still raises locally.
                new_args = [self.resolve_exp(a, scope) for a in args]
                if name in self._functions:
                    return c99_ast.FunctionCall(
                        name=name, args=new_args,
                    )
                if name in scope:
                    return c99_ast.FunctionCall(
                        name=scope[name][0], args=new_args,
                    )
                raise IdentifierResolutionError(
                    f"undeclared identifier {name!r}"
                )
        raise TypeError(f"unexpected exp: {exp!r}")


def resolve_program(prog: c99_ast.Type_program) -> c99_ast.Type_program:
    return Resolver().resolve_program(prog)


def resolve_function(
    fn: c99_ast.Type_function_definition,
) -> c99_ast.Type_function_definition:
    # Pre-register the function's own name so it can recurse, then
    # walk its body. Mostly useful for unit tests that exercise a
    # single function in isolation.
    r = Resolver()
    r._register_function_definition(fn)
    return r.resolve_function(fn)
