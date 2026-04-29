"""Identifier resolution pass: c99_ast -> c99_ast.

Resolves every user-written identifier — variables and function names
both — to a form later passes can treat as unambiguous, rejects
references that don't match any declaration, and tags every
declaration with its C99 §6.2.2 linkage kind.

Linkage drives renaming: identifiers with NONE linkage get a program-
unique `@<N>.<original>` rename so later passes can flatten scope
without re-implementing it; INTERNAL- and EXTERNAL-linkage identifiers
keep their source spelling because the linker (or later TU passes,
for INTERNAL) resolves them by name. The `@<N>.<original>` scheme is
collision-proof: `@` is illegal in a C identifier, so a resolved
NONE-linkage name can never alias anything the user could write or
any external symbol.

Linkage rules implemented (C99 §6.2.2)
--------------------------------------
File scope (top-level declarations):
  * `static int x;` / `static int foo(...);`     → INTERNAL
  * `extern int x;` / `extern int foo(...);`     → matches the linkage
    of the prior visible declaration of the same name (INTERNAL or
    EXTERNAL); EXTERNAL otherwise.
  * `int x;`                                     → EXTERNAL (file-scope
    object with no specifier — §6.2.2.5).
  * `int foo(...);` / `int foo(...) { ... }`     → as if `extern` —
    matches prior visible if any, else EXTERNAL (§6.2.2.5).

Block scope:
  * `int x;` / `static int x;`                   → NONE.
    `static` at block scope changes storage duration, not linkage —
    §6.2.2 still classifies "a block scope identifier for an object
    declared without the storage-class specifier extern" as NONE.
  * `extern int x;`                              → matches prior
    visible declaration's linkage if it has any (INTERNAL/EXTERNAL),
    else EXTERNAL. The "prior visible" lookup walks the current scope
    chain — including the file-scope parent that's cloned into every
    function body — so a block-scope `extern int x;` after a
    file-scope `static int x;` correctly inherits INTERNAL.
  * `int foo(...);` (function decl, no body)     → as if `extern` —
    same prior-visible rule. `static` is not legal on a block-scope
    function declaration (§6.2.2: "A function declaration can contain
    the storage-class specifier static only if it is at file scope.").
  * function parameter                           → NONE.

Renaming
--------
NONE-linkage names get `@<N>.<original>`, INTERNAL/EXTERNAL keep their
source spelling. The unique-counter is bumped only on a rename, so
EXTERNAL declarations sharing a name (e.g. `int foo(void); int
foo(void) { ... }`) don't perturb the numbering.

Errors raised (`IdentifierResolutionError`)
-------------------------------------------
  * declaring a NONE-linkage name twice in the same block
  * declaring the same name twice in the same scope with different
    linkages (a constraint violation per C99 §6.7 / §6.2.2 — e.g.
    `static int x; int x;` at file scope)
  * referencing a variable name that hasn't been declared
  * calling a function that hasn't been declared anywhere visible
  * `static` on a block-scope function declaration
  * an Assignment / Postfix whose lval isn't a Var (when richer
    lvalues land — `*p`, `a[i]`, `s.f` — this widens to an "is-
    lvalue" predicate)

Two-pass walk over the program
------------------------------
A function body can call (or read) a name declared *later* at file
scope — `int main(void) { return foo(); } int foo(void) { return 1; }`
is well-formed. To keep that working we register every file-scope
declaration's linkage in a first pass *before* descending into any
body in a second pass. Both passes walk in source order so each
declaration's linkage is computed against the visible-prior table at
that point.

Scope structure
---------------
A `_Scope` is `dict[str, tuple[str, bool, Linkage]]` mapping each
visible name to `(resolved_name, inner, linkage)`. `inner` is True iff
the binding was introduced in the *current* block. Entering a nested
block clones the parent and flips every entry's `inner` flag to False.
The file-scope identifier table seeds the per-function body's scope
the same way: every entry from `_file_scope` arrives as outer-scoped,
and any function-body or block-scope declaration that shadows it gets
its own inner-scoped entry. This is what makes the prior-visible rule
fall out cleanly from `scope.get(name)`.

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
        declaration is reached. Block-scope automatic variables and
        block-scope `static` objects (storage duration changes but
        linkage doesn't), function parameters, and "anything other
        than an object or a function" all carry NONE linkage.
      - INTERNAL: identifier denotes the same object/function within
        a translation unit. Produced by `static` at file scope, and
        inherited by an `extern` redeclaration of an internally-linked
        prior decl.
      - EXTERNAL: identifier denotes the same object/function across
        all translation units the program is linked from. Produced by
        any file-scope object declaration without a specifier, by any
        function declaration without a specifier, and (often) by
        `extern` declarations.

    Renaming is keyed off linkage rather than "is it a function":
    NONE-linkage names get globally-unique `@<N>.<orig>` strings;
    INTERNAL and EXTERNAL names keep their original spelling because
    the linker (or later TU passes, for INTERNAL) must be able to find
    them by name.
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


def _storage_is(sc, kind):
    """True iff the storage-class node is of the given kind class
    (`c99_ast.Static` / `c99_ast.Extern`). `None` matches nothing."""
    return sc is not None and isinstance(sc, kind)


def _decl_name(decl: c99_ast.Type_declaration) -> str:
    """Source-level name of a top-level declaration."""
    match decl:
        case c99_ast.VarDecl(var_decl=vd):
            return vd.name
        case c99_ast.FunctionDecl(function_decl=fd):
            return fd.name
    raise TypeError(f"unexpected declaration: {decl!r}")


class Resolver:
    """Holds the unique-name counter and the file-scope identifier
    table. One Resolver per program; each NONE-linkage declaration
    bumps the counter, and every file-scope declaration registers a
    name with its computed linkage. Module-level `resolve_*` wrappers
    build a fresh Resolver per call."""

    def __init__(self) -> None:
        self._counter = 0
        # File-scope ordinary identifiers (objects + functions —
        # they share one C namespace). Maps source name to
        # (resolved_name, linkage). Resolved name == source name for
        # every entry here, because file-scope identifiers are always
        # INTERNAL or EXTERNAL and aren't renamed; carrying it
        # explicitly keeps the shape uniform with the per-block
        # `_Scope` so the same lookup-and-shadow logic works for both.
        # Populated by the first pass over `Program.declaration`;
        # consumed when seeding each function body's outer scope and
        # when validating top-level Var / FunctionCall references.
        self._file_scope: dict[str, tuple[str, Linkage]] = {}
        # Names whose file-scope declarator has been completed in
        # source order — used to filter `_file_scope_seed` so a
        # function body only sees prior file-scope identifiers, not
        # later ones. C99 §6.2.1.4: "The scope of a file-scope
        # identifier... begins at the point after the declarator..."
        self._seen_at_file_scope: set[str] = set()

    def make_unique(self, original: str) -> str:
        name = f"@{self._counter}.{original}"
        self._counter += 1
        return name

    # ------------------------------------------------------------------
    # Top-level program walk
    # ------------------------------------------------------------------

    def resolve_program(
        self, prog: c99_ast.Type_program,
    ) -> c99_ast.Type_program:
        match prog:
            case c99_ast.Program(declaration=decls):
                # Pass 1: walk every file-scope declaration in source
                # order, computing each one's linkage against the
                # already-registered prior decls and recording it in
                # `_file_scope`. This *only* populates the file-scope
                # table — it doesn't recurse into bodies or
                # initializers, so a body in any function can later
                # reference a global / function declared further down
                # in the file.
                for d in decls:
                    self._register_file_scope(d)
                # Pass 2: walk each declaration again, this time
                # resolving initializers, bodies, and parameter scopes.
                # Mark each name in `_seen_at_file_scope` BEFORE
                # processing its body / initializer so the declared
                # name is visible to itself (function self-recursion;
                # `int x = x;` per C99). Forward references to later
                # decls aren't visible.
                new_decls = []
                for d in decls:
                    self._seen_at_file_scope.add(_decl_name(d))
                    new_decls.append(self._resolve_file_scope_decl(d))
                return c99_ast.Program(declaration=new_decls)
        raise TypeError(f"unexpected program: {prog!r}")

    # ------------------------------------------------------------------
    # File-scope linkage determination
    # ------------------------------------------------------------------

    def _register_file_scope(
        self, decl: c99_ast.Type_declaration,
    ) -> None:
        match decl:
            case c99_ast.VarDecl(var_decl=vd):
                linkage = self._file_scope_object_linkage(
                    vd.name, vd.storage_class,
                )
                self._record_file_scope(vd.name, linkage)
            case c99_ast.FunctionDecl(function_decl=fd):
                linkage = self._file_scope_function_linkage(
                    fd.name, fd.storage_class,
                )
                self._record_file_scope(fd.name, linkage)
            case _:
                raise TypeError(f"unexpected declaration: {decl!r}")

    def _file_scope_object_linkage(
        self,
        name: str,
        storage_class: c99_ast.Type_storage_class | None,
    ) -> Linkage:
        # File-scope object linkage (C99 §6.2.2.3 / §6.2.2.4 / §6.2.2.5):
        #   static  → INTERNAL
        #   extern  → match prior visible (INTERNAL or EXTERNAL); else EXTERNAL
        #   none    → EXTERNAL (file-scope objects without a specifier
        #             have external linkage)
        if _storage_is(storage_class, c99_ast.Static):
            return Linkage.INTERNAL
        if _storage_is(storage_class, c99_ast.Extern):
            return self._extern_inherited_linkage(self._file_scope.get(name))
        return Linkage.EXTERNAL

    def _file_scope_function_linkage(
        self,
        name: str,
        storage_class: c99_ast.Type_storage_class | None,
    ) -> Linkage:
        # File-scope function linkage (§6.2.2.3 / §6.2.2.5):
        #   static  → INTERNAL
        #   extern  → match prior visible; else EXTERNAL
        #   none    → as if extern: match prior visible; else EXTERNAL
        if _storage_is(storage_class, c99_ast.Static):
            return Linkage.INTERNAL
        return self._extern_inherited_linkage(self._file_scope.get(name))

    @staticmethod
    def _extern_inherited_linkage(
        prior: tuple[str, Linkage] | None,
    ) -> Linkage:
        """Apply the C99 §6.2.2.4 rule: an `extern` (or no-specifier
        function) declaration takes its linkage from the prior visible
        declaration if that prior has linkage; otherwise EXTERNAL."""
        if prior is not None and prior[1] in (
            Linkage.INTERNAL, Linkage.EXTERNAL,
        ):
            return prior[1]
        return Linkage.EXTERNAL

    def _record_file_scope(self, name: str, linkage: Linkage) -> None:
        # File-scope identifiers aren't renamed (linker uses the
        # source spelling), so resolved_name == name. Reject a
        # change-of-linkage redeclaration; a same-linkage redeclaration
        # is idempotent and legal (the multi-decl case for both
        # functions and tentative-definition objects).
        prior = self._file_scope.get(name)
        if prior is not None and prior[1] != linkage:
            # C99 §6.2.2.7 says this is undefined behavior, but we have
            # the visibility to give a clean diagnostic, so we do.
            raise IdentifierResolutionError(
                f"file-scope identifier {name!r} declared with "
                f"{linkage.value} linkage after a prior "
                f"{prior[1].value} declaration"
            )
        self._file_scope[name] = (name, linkage)

    # ------------------------------------------------------------------
    # Pass 2: resolve each top-level declaration's body / initializer
    # ------------------------------------------------------------------

    def _resolve_file_scope_decl(
        self, decl: c99_ast.Type_declaration,
    ) -> c99_ast.Type_declaration:
        match decl:
            case c99_ast.VarDecl(var_decl=vd):
                # File-scope objects keep their source name (linkage is
                # never NONE here). The initializer (if any) resolves
                # against the file-scope table — references to other
                # globals/functions are valid because everything has
                # already been registered in pass 1.
                seed_scope = self._file_scope_seed()
                new_init = (
                    self.resolve_exp(vd.init, seed_scope)
                    if vd.init is not None else None
                )
                return c99_ast.VarDecl(
                    var_decl=c99_ast.Type_var_decl(
                        name=vd.name,
                        init=new_init,
                        data_type=vd.data_type,
                        storage_class=vd.storage_class,
                    ),
                )
            case c99_ast.FunctionDecl(function_decl=fd):
                return c99_ast.FunctionDecl(
                    function_decl=self._resolve_function_decl(
                        fd, file_scope=True,
                    ),
                )
        raise TypeError(f"unexpected declaration: {decl!r}")

    def _file_scope_seed(self) -> _Scope:
        """Build a `_Scope` view of the file-scope table for use as
        the outer scope of a function body or as the lookup scope for
        a file-scope variable's initializer. Filtered to entries
        whose declarator has already been completed in source order
        (C99 §6.2.1.4: "The scope of a file-scope identifier...
        begins at the point after the declarator..."). Forward refs
        to later file-scope decls aren't included.

        Every entry comes through as outer-scoped (inner=False) so
        block declarations can legally shadow it."""
        return {
            name: (resolved, False, link)
            for name, (resolved, link) in self._file_scope.items()
            if name in self._seen_at_file_scope
        }

    # ------------------------------------------------------------------
    # Function declarations and definitions (file or block scope)
    # ------------------------------------------------------------------

    def _resolve_function_decl(
        self,
        fd: c99_ast.Type_function_decl,
        *,
        file_scope: bool,
    ) -> c99_ast.Type_function_decl:
        """Resolve a `function_decl` (forward declaration *or* function
        definition — they're the same shape in c99_ast, distinguished
        by whether `body` is None). Used for both file-scope and
        block-scope function declarations; `file_scope` only affects
        whether the function body's outermost scope is seeded from
        `_file_scope` (definitions only appear at file scope today;
        the flag is a future-proofing courtesy)."""
        new_params, param_scope = self._resolve_params(fd.params)
        new_body: c99_ast.Type_block | None
        if fd.body is None:
            new_body = None
        else:
            # C99 §6.9.1.7: parameters share the body's outermost scope
            # (so an outer-block decl that reuses a param name is a
            # duplicate-decl error). Combine the file-scope parent with
            # the param scope to form the body's seed scope. A param
            # in the param scope shadows any same-named file-scope
            # entry in the seed (which is fine — params have NONE
            # linkage, file-scope entries have EXTERNAL/INTERNAL, and
            # the param value wins for the shadow).
            seed: _Scope = self._file_scope_seed() if file_scope else {}
            for p_orig, p_resolved in zip(fd.params, new_params):
                seed[p_orig] = (p_resolved, True, Linkage.NONE)
            match fd.body:
                case c99_ast.Block(block_item=items):
                    new_body = c99_ast.Block(block_item=[
                        self.resolve_block_item(item, seed)
                        for item in items
                    ])
                case _:
                    raise TypeError(f"unexpected body: {fd.body!r}")
        return c99_ast.Type_function_decl(
            name=fd.name,
            params=new_params,
            body=new_body,
            data_type=fd.data_type,
            storage_class=fd.storage_class,
        )

    def _resolve_params(
        self, params: list[str],
    ) -> tuple[list[str], _Scope]:
        """Validate parameter-name uniqueness and rename each param to
        a fresh `@<N>.<orig>`. Returns the renamed names (in order)
        and the param scope dict."""
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

    # ------------------------------------------------------------------
    # Block-scope declarations
    # ------------------------------------------------------------------

    def resolve_block(
        self,
        block: c99_ast.Type_block,
        parent_scope: _Scope,
    ) -> c99_ast.Type_block:
        # Entering a new block: clone the parent scope, flipping every
        # entry's inner-scoped flag to False. Linkage tags ride along
        # unchanged.
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
                linkage = self._block_scope_object_linkage(vd, scope)
                return c99_ast.VarDecl(
                    var_decl=self.resolve_var_decl(vd, scope, linkage),
                )
            case c99_ast.FunctionDecl(function_decl=fd):
                linkage = self._block_scope_function_linkage(fd, scope)
                # Stash the block-scope function declaration in the
                # current scope under its source name and EXTERNAL/
                # INTERNAL linkage. References (Var / FunctionCall) in
                # the same scope chain see it; an inner block can
                # shadow it just like any other identifier.
                self._record_block_decl(fd.name, fd.name, linkage, scope)
                # The param names get the same NONE-linkage treatment
                # as variables, but in their own scope — the scope is
                # discarded after this method returns (a block-scope
                # decl has no body). `_resolve_function_decl` does
                # both jobs.
                return c99_ast.FunctionDecl(
                    function_decl=self._resolve_function_decl(
                        fd, file_scope=False,
                    ),
                )
        raise TypeError(f"unexpected declaration: {decl!r}")

    def _block_scope_object_linkage(
        self,
        vd: c99_ast.Type_var_decl,
        scope: _Scope,
    ) -> Linkage:
        # Block-scope object linkage (§6.2.2.4 / §6.2.2.6):
        #   extern → prior-visible rule; else EXTERNAL
        #   else   → NONE (including `static`, which only changes
        #            storage duration, not linkage)
        if _storage_is(vd.storage_class, c99_ast.Extern):
            prior = scope.get(vd.name)
            prior_link = prior[2] if prior is not None else None
            if prior_link in (Linkage.INTERNAL, Linkage.EXTERNAL):
                return prior_link
            return Linkage.EXTERNAL
        return Linkage.NONE

    def _block_scope_function_linkage(
        self,
        fd: c99_ast.Type_function_decl,
        scope: _Scope,
    ) -> Linkage:
        # Block-scope function declaration (§6.2.2.5):
        #   no specifier → as if `extern`: prior-visible rule
        #   extern       → same prior-visible rule
        #   static       → forbidden at block scope (§6.2.2: "A
        #                  function declaration can contain the
        #                  storage-class specifier static only if it
        #                  is at file scope")
        if _storage_is(fd.storage_class, c99_ast.Static):
            raise IdentifierResolutionError(
                f"static is not allowed on a block-scope function "
                f"declaration: {fd.name!r}"
            )
        prior = scope.get(fd.name)
        prior_link = prior[2] if prior is not None else None
        if prior_link in (Linkage.INTERNAL, Linkage.EXTERNAL):
            return prior_link
        return Linkage.EXTERNAL

    def _record_block_decl(
        self,
        original: str,
        resolved: str,
        linkage: Linkage,
        scope: _Scope,
    ) -> None:
        """Add a block-scope binding to `scope`, raising on an
        incompatible same-block redeclaration. Callers compute the
        resolved name (for NONE-linkage) or pass the source name
        (for INTERNAL/EXTERNAL)."""
        existing = scope.get(original)
        if existing is not None and existing[1]:
            # Same-block redeclaration. Allowed only when both old and
            # new have non-NONE linkage that matches — that's the
            # "two declarations of the same external object/function"
            # case. Anything else (NONE-NONE, NONE-EXTERNAL, etc.) is
            # a constraint violation.
            existing_link = existing[2]
            if existing_link != linkage or linkage is Linkage.NONE:
                raise IdentifierResolutionError(
                    f"duplicate declaration of {original!r}"
                )
            # Same external symbol redeclared — keep the existing
            # entry (resolved name and inner flag don't change).
            return
        scope[original] = (resolved, True, linkage)

    def resolve_var_decl(
        self,
        vd: c99_ast.Type_var_decl,
        scope: _Scope,
        linkage: Linkage,
    ) -> c99_ast.Type_var_decl:
        match vd:
            case c99_ast.Type_var_decl(name=name, init=init):
                # Renaming is gated on linkage: NONE → fresh
                # `@<N>.<orig>`; INTERNAL/EXTERNAL → keep source
                # spelling.
                if linkage is Linkage.NONE:
                    resolved = self.make_unique(name)
                else:
                    resolved = name
                # Bind before resolving the initializer so `int a = a;`
                # resolves to the new binding (matches C's rule that
                # `a` on the RHS refers to the one being declared,
                # even though the read of an uninitialized object is
                # UB at runtime). The same rule lets a shadowing
                # decl's initializer NOT see the outer binding —
                # `int a = 5; { int a = a; }` reads the inner
                # uninitialized `a`.
                self._record_block_decl(name, resolved, linkage, scope)
                # `extern int x = ...;` is a tentative-definition / one-
                # def-rule concern best handled by the type checker; we
                # let the initializer resolve normally.
                new_init = (
                    self.resolve_exp(init, scope) if init is not None else None
                )
                return c99_ast.Type_var_decl(
                    name=resolved,
                    init=new_init,
                    data_type=vd.data_type,
                    storage_class=vd.storage_class,
                )
        raise TypeError(f"unexpected var_decl: {vd!r}")

    # ------------------------------------------------------------------
    # Statements and expressions (mostly unchanged from before — only
    # the file-scope identifier lookup and the matching-on-c99_ast.Var
    # branch differ from the per-block scope logic).
    # ------------------------------------------------------------------

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
                return c99_ast.IfStmt(
                    condition=self.resolve_exp(cond, scope),
                    then_clause=self.resolve_statement(then_stmt, scope),
                    else_clause=(
                        self.resolve_statement(else_stmt, scope)
                        if else_stmt is not None else None
                    ),
                )
            case c99_ast.Compound(block=block):
                return c99_ast.Compound(
                    block=self.resolve_block(block, scope),
                )
            case c99_ast.Goto(label=label):
                return c99_ast.Goto(label=label)
            case c99_ast.LabeledStmt(label=label, statement=inner):
                return c99_ast.LabeledStmt(
                    label=label,
                    statement=self.resolve_statement(inner, scope),
                )
            case c99_ast.BreakStmt(label=label):
                return c99_ast.BreakStmt(label=label)
            case c99_ast.ContinueStmt(label=label):
                return c99_ast.ContinueStmt(label=label)
            case c99_ast.WhileStmt(
                condition=cond, body=body, label=label,
            ):
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
                # scope. Mechanics match Compound: clone the parent
                # scope flipping all entries to outer-scoped, then
                # resolve init/cond/post/body in the clone.
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
        match init:
            case c99_ast.InitDecl(var_decl=vd):
                # C99 §6.8.5.3: the for-init is restricted to a *non-
                # extern, non-static* declaration. Reject either
                # storage class up front so the resolver doesn't
                # silently accept ill-formed C.
                if vd.storage_class is not None:
                    raise IdentifierResolutionError(
                        f"storage-class specifier not allowed on a "
                        f"for-init declaration: {vd.name!r}"
                    )
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
            case c99_ast.Constant(const=c):
                # `const` is a `Type_const` (ConstInt or ConstLong) —
                # an inert value, not an identifier. Pass through.
                return c99_ast.Constant(const=c)
            case c99_ast.Cast(target_type=t, exp=inner):
                # The target type is plain syntax (Int / Long / FunType
                # nodes); only the inner expression has identifiers to
                # resolve.
                return c99_ast.Cast(
                    target_type=t,
                    exp=self.resolve_exp(inner, scope),
                )
            case c99_ast.Var(name=name):
                # Variables and functions share one C namespace, so a
                # single lookup in the per-block scope is enough — file-
                # scope entries arrive there too via the seed clone, and
                # block-scope function decls live in the same map. The
                # type checker decides whether using the name in a Var
                # context is legal; we just hand off the resolved name.
                if name in scope:
                    return c99_ast.Var(name=scope[name][0])
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
                # Lvalue forms accepted today: a bare Var, a
                # Dereference (`*p = …`), or a Subscript (`a[i] = …`,
                # which the type checker desugars to `*(a + i) = …`).
                # Per C99 §6.3.2.1.1, the result of a unary `*` is an
                # lvalue and so is the result of `[]` (`a[i]` is
                # defined as `*(a + i)`).
                if not isinstance(lval, (
                    c99_ast.Var, c99_ast.Dereference, c99_ast.Subscript,
                )):
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
                # Same lvalue rule as Assignment — Var, Dereference,
                # or Subscript (the three syntactic lvalue forms
                # supported today).
                if not isinstance(operand, (
                    c99_ast.Var, c99_ast.Dereference, c99_ast.Subscript,
                )):
                    raise IdentifierResolutionError(
                        f"invalid lvalue in postfix: {operand!r}"
                    )
                return c99_ast.Postfix(
                    op=op, operand=self.resolve_exp(operand, scope),
                )
            case c99_ast.Dereference(exp=inner):
                # `*e` — recurse into the operand. The lvalue check
                # for `&(*e)` is structural (handled in the AddressOf
                # case), so we don't need to validate `e` here.
                return c99_ast.Dereference(
                    exp=self.resolve_exp(inner, scope),
                )
            case c99_ast.Subscript(array=arr, index=idx):
                # `a[i]` — both subexpressions are recursive contexts.
                # The lvalue check for `a[i] = …` is the structural
                # one in the Assignment / Postfix cases above; here we
                # just thread name resolution through.
                return c99_ast.Subscript(
                    array=self.resolve_exp(arr, scope),
                    index=self.resolve_exp(idx, scope),
                )
            case c99_ast.InitList(items=items):
                # `{e1, e2, ...}` — recurse into each item. The type
                # checker enforces that this only appears as a
                # var_decl init slot for an Array; here we just
                # resolve names. Items can themselves be InitLists
                # (for nested / multi-dim init), so the recursion
                # naturally handles both shapes.
                return c99_ast.InitList(
                    items=[self.resolve_exp(it, scope) for it in items],
                )
            case c99_ast.AddressOf(exp=inner):
                # `&e` — operand must be an lvalue. The three
                # syntactic lvalue forms supported today are Var
                # (`&x`), Dereference (`&*p`, equivalent to `p` per
                # C99 §6.5.3.2.3), and Subscript (`&a[i]`, equivalent
                # to `a + i` per the same paragraph). The type
                # checker enforces additional constraints (operand
                # must denote an object, not a function or `register`
                # storage); here we just enforce the syntactic
                # lvalue restriction.
                if not isinstance(inner, (
                    c99_ast.Var, c99_ast.Dereference, c99_ast.Subscript,
                )):
                    raise IdentifierResolutionError(
                        f"invalid operand of unary '&': {inner!r}"
                    )
                return c99_ast.AddressOf(
                    exp=self.resolve_exp(inner, scope),
                )
            case c99_ast.FunctionCall(name=name, args=args):
                # Same single-namespace lookup as Var. The call is
                # syntactically legal as long as the name is declared
                # (block scope, file scope, or as an inherited file-
                # scope entry); the type checker enforces "the name
                # actually denotes a function".
                new_args = [self.resolve_exp(a, scope) for a in args]
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
    """Test convenience: resolve a single function in isolation.

    The c99 AST top-level shape is `Program(declaration*)` now, with
    function definitions encoded as `FunctionDecl(function_decl=...,
    body=Block(...))`. The legacy `Function(...)` node is still
    declared in `c99_ast` but no longer produced by the parser. This
    wrapper accepts a `Function` node, threads it through
    `resolve_program` as a one-element program, and unwraps the
    resolved function back into the legacy shape so existing unit
    tests don't have to construct full Programs by hand."""
    # The legacy `Function(name, params, body)` shape doesn't carry
    # type information, so synthesize an Int-returning, Int-param
    # FunType — that's what every test that uses this helper assumed
    # implicitly anyway.
    ftype = c99_ast.FunType(
        params=[c99_ast.Int() for _ in fn.params],
        ret=c99_ast.Int(),
    )
    fd = c99_ast.Type_function_decl(
        name=fn.name,
        params=list(fn.params),
        body=fn.body,
        data_type=ftype,
        storage_class=None,
    )
    prog = c99_ast.Program(declaration=[
        c99_ast.FunctionDecl(function_decl=fd),
    ])
    resolved = Resolver().resolve_program(prog)
    new_fd = resolved.declaration[0].function_decl
    return c99_ast.Function(
        name=new_fd.name,
        params=list(new_fd.params),
        body=new_fd.body,
    )
