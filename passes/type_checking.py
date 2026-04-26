"""Type checking pass: c99_ast -> (c99_ast, SymbolTable).

Walks the program once after identifier_resolution / label_resolution /
loop_labeling have all run. Validates that every identifier is used in
a way consistent with its declaration, computes each declaration's
initial value (for objects with static storage duration) and
defined-ness (for functions), and produces a `SymbolTable` keyed by
each identifier's resolved name.

The SymbolTable is the canonical "what does this name mean?" source for
every later pass:
  * `c99_to_tac` consumes it twice — once to set `is_global` on each
    TAC `Function`, and once at the end to enumerate every
    static-storage-duration object and emit a TAC `StaticVariable`
    for each one with a definition.
  * later codegen passes will use it to distinguish module-local
    NONE-linkage statics from translation-unit-global EXTERNAL
    symbols when laying out asm sections / picking label conventions.

Type vocabulary
---------------
- `Int()`: the only object type today.
- `FunType(params=tuple[Type, ...], ret=Type)`: function types,
  always `Int -> ... -> Int` for now.

Both `Type` subclasses are frozen dataclasses, so equality is
structural and arity differences distinguish function types.

Symbol attributes
-----------------
A `Symbol` carries a `type` plus an `IdAttr` describing how the
symbol exists at runtime. The three `IdAttr` subclasses encode the
three runtime categories C99 distinguishes:

- `LocalAttr`: an automatic-storage object — block-scope `int x;`,
  function parameter. Lives on the soft stack with a fresh slot per
  function activation. No `is_global`, no initial value tracked
  (the initializer is lowered as a TAC `Copy` at the declaration
  site, same as before).
- `StaticAttr(initial_value, is_global)`: an object with static
  storage duration. Covers every file-scope object plus block-scope
  `static int x;`. The `initial_value` is one of `Initial(c)`,
  `Tentative`, or `NoInitializer`; `is_global` is True iff the
  symbol has external linkage. `c99_to_tac` emits a TAC
  `StaticVariable` for each StaticAttr whose `initial_value` is
  `Initial(c)` (use `c`) or `Tentative` (use `0`); `NoInitializer`
  entries describe a reference to a symbol defined in some other
  declaration / TU and emit nothing.
- `FunAttr(defined, is_global)`: a function name. `defined` flips
  to True the first time we see a definition (a `FunctionDecl` whose
  `function_decl.body` is non-None); subsequent definitions raise.
  `is_global` is True iff the function has external linkage.

`is_global` is the bool that asm output ultimately cares about
(visible-outside-the-TU vs. not), so we materialize it here once
rather than threading the three-way `Linkage` enum through every
later pass.

Initial-value rules (C99 §6.7.8 / §6.9.2)
-----------------------------------------
- Block-scope `static int x;` (no initializer) → `Initial(0)` —
  C99 §6.7.8.10: "If an object that has static storage duration is
  not initialized explicitly, ... if it has arithmetic type, it is
  initialized to (positive or unsigned) zero."
- Block-scope `static int x = e;` → `Initial(c)` where `c` is the
  constant value of `e`. Initializers for static-storage objects
  must be constant expressions; we accept only integer literals
  today and reject anything else as a `TypeCheckError`.
- Block-scope `extern int x;` → `NoInitializer`; the declaration is
  a reference, not a definition. Resolver guarantees the linkage is
  EXTERNAL or INTERNAL (matching the prior visible decl).
- File-scope `int x;` (no initializer) → `Tentative`. C99 §6.9.2.2
  defers tentative definitions to the end of the TU and resolves
  any unresolved tentative to an `Initial(0)` definition.
- File-scope `int x = e;` → `Initial(c)`. Same constant-expression
  restriction as block-scope `static`.
- File-scope `extern int x;` (no initializer) → `NoInitializer`.
- File-scope `extern int x = e;` → `Initial(c)` (the initializer
  promotes the declaration to a definition; this is unusual but
  legal).

Merging on redeclaration
------------------------
Multiple declarations of the same identifier at file scope are
common (`int foo(void); int foo(void) { ... }`, `int x; int x = 5;`,
`extern int x; static int x = 5;` — the last one is UB but
identifier_resolution already rejects linkage changes). The merge
rules:
- Function: signatures must be equal; `defined` becomes True if either
  side is True; raise on True+True.
- Object: `is_global` must be equal (resolver enforces this); the
  initial-value lattice merges as
    Initial(a) ∨ Initial(b)         → error if a != b (multiple
                                       definitions of the same object)
    Initial(a) ∨ (Tentative or
                  NoInitializer)    → Initial(a)
    Tentative ∨ Tentative           → Tentative
    Tentative ∨ NoInitializer       → Tentative
    NoInitializer ∨ NoInitializer   → NoInitializer

Errors raised (`TypeCheckError`)
--------------------------------
Carried over from before:
- Function used as a variable (`Var(name)` where `name` resolves to
  a function symbol).
- Variable called as a function.
- Wrong arity at a call site.
- Argument or return-type mismatch (degenerate today since every
  type is `Int`).
- Incompatible function redeclaration (signature differs).
- Function redefinition (two definitions of the same name).

New for this pass:
- Initializer for a static-storage object isn't a compile-time
  constant.
- Multiple definitions of an object (`int x = 1; int x = 2;`).
- Initializer with `extern` at *block* scope (C99 §6.7.8 forbids
  it: "If the declaration of an identifier has block scope, and
  the identifier has external or internal linkage, the declaration
  shall have no initializer for the identifier.").

The pass does **not** modify the AST. Its return contract is the
input program (returned as-is) plus the populated SymbolTable.
"""

from __future__ import annotations

from dataclasses import dataclass

import c99_ast
from passes.identifier_resolution import Linkage


class TypeCheckError(Exception):
    """Raised on any type-level inconsistency in the program."""


# ---------------------------------------------------------------------------
# Types
# ---------------------------------------------------------------------------


@dataclass(frozen=True)
class Type:
    """Marker base class for the type AST. Frozen + dataclass so
    subclasses get value equality and hashability for free."""


@dataclass(frozen=True)
class Int(Type):
    """The only object type today."""


@dataclass(frozen=True)
class FunType(Type):
    """A C function type: an ordered tuple of parameter types and a
    return type. Stored as a tuple (not list) so the dataclass is
    hashable and value-comparable."""
    params: tuple[Type, ...]
    ret: Type


# ---------------------------------------------------------------------------
# Initial values for static-storage objects (C99 §6.9.2)
# ---------------------------------------------------------------------------


@dataclass(frozen=True)
class InitialValue:
    """Marker base class for the three initial-value cases. Each
    static-storage object's symbol carries one of these."""


@dataclass(frozen=True)
class Tentative(InitialValue):
    """File-scope object declared without an initializer, no extern.
    Per C99 §6.9.2.2 a tentative definition is upgraded to an
    `Initial(0)` definition at end-of-TU if no other initialization
    appears."""


@dataclass(frozen=True)
class Initial(InitialValue):
    """Object declared with an initializer. Today the only legal
    initializer for a static-storage object is an integer
    constant; `value` carries that integer."""
    value: int


@dataclass(frozen=True)
class NoInitializer(InitialValue):
    """`extern int x;` — the declaration is a reference to a symbol
    whose definition lives elsewhere. Emits nothing during
    `c99_to_tac`'s static-variable enumeration."""


# ---------------------------------------------------------------------------
# Symbol attributes (which runtime category an identifier belongs to)
# ---------------------------------------------------------------------------


@dataclass(frozen=True)
class IdAttr:
    """Marker base class for the three symbol-attribute cases:
    LocalAttr (automatic storage), StaticAttr (static-storage object),
    FunAttr (function name)."""


@dataclass(frozen=True)
class LocalAttr(IdAttr):
    """Automatic-storage object — block-scope `int x;` or function
    parameter. No `is_global` (it isn't visible across TUs / can't
    be), no `initial_value` (the initializer, if any, is lowered as
    a regular TAC `Copy` at the declaration's source position)."""


@dataclass(frozen=True)
class StaticAttr(IdAttr):
    """An object with static storage duration. Covers every file-
    scope object and every block-scope `static int x;`. `is_global`
    is True iff the symbol has external linkage; the codegen will
    use it to decide between a TU-local label and a global symbol.
    `initial_value` carries the resolved init expression if known,
    or one of the deferred markers (Tentative, NoInitializer)."""
    initial_value: InitialValue
    is_global: bool


@dataclass(frozen=True)
class FunAttr(IdAttr):
    """A function name. `defined` flips True the first time a
    definition is seen for this name (a FunctionDecl whose body is
    non-None); a second definition raises. `is_global` is True iff
    the function has external linkage."""
    defined: bool
    is_global: bool


# ---------------------------------------------------------------------------
# Symbol table
# ---------------------------------------------------------------------------


@dataclass
class Symbol:
    """One row of the symbol table. Pairs the identifier's type with
    its `IdAttr` (which storage category it belongs to and what
    extra metadata that category needs)."""
    type: Type
    attrs: IdAttr


class SymbolTable:
    """Flat dict[str, Symbol] keyed by resolved identifier name.

    identifier_resolution gives every NONE-linkage name a unique
    `@<N>.<orig>` and every INTERNAL/EXTERNAL name its source
    spelling, so a single dict is enough — no nested scopes. The
    table is built up in source-program order by a single
    `TypeChecker` walk and then exposed to later passes (mainly
    `c99_to_tac`).
    """

    def __init__(self) -> None:
        self._table: dict[str, Symbol] = {}

    def __contains__(self, name: str) -> bool:
        return name in self._table

    def __getitem__(self, name: str) -> Symbol:
        return self._table[name]

    def __setitem__(self, name: str, sym: Symbol) -> None:
        self._table[name] = sym

    def get(self, name: str) -> Symbol | None:
        return self._table.get(name)

    def items(self):
        return self._table.items()

    def __len__(self) -> int:
        return len(self._table)

    def __repr__(self) -> str:
        return f"SymbolTable({self._table!r})"


# ---------------------------------------------------------------------------
# Type checker
# ---------------------------------------------------------------------------


def _linkage_to_is_global(linkage: Linkage) -> bool:
    """Convert the three-way C99 linkage classification into the
    binary "visible across translation units" flag the asm
    backend cares about. EXTERNAL is the only kind that survives
    the linker's TU boundary."""
    return linkage is Linkage.EXTERNAL


def _const_init_value(exp: c99_ast.Type_exp, name: str) -> int:
    """Static-storage initializers must be compile-time constant
    expressions (C99 §6.7.8.4). Today we accept only integer
    literals; anything richer (constant folding of `1+2`,
    address-of, etc.) needs a constant-expression evaluator we
    don't have yet."""
    match exp:
        case c99_ast.Constant(value=v):
            return v
    raise TypeCheckError(
        f"initializer for static-storage object {name!r} is not a "
        f"constant expression"
    )


def _merge_initial_value(
    name: str,
    old: InitialValue,
    new: InitialValue,
) -> InitialValue:
    """Combine two initial-value tags for the same file-scope
    object across declarations. Two `Initial`s with different
    values is "multiple definitions" — the one error case here.
    Otherwise the more-defined of the two wins (Initial > Tentative >
    NoInitializer)."""
    if isinstance(old, Initial) and isinstance(new, Initial):
        # `int x = 1; int x = 2;` — two definitions of the same
        # object. (Even when the values agree this is a §6.9.2
        # constraint violation; we reject any case where both are
        # Initial.)
        raise TypeCheckError(
            f"redefinition of object {name!r}: prior initializer "
            f"{old.value}, new initializer {new.value}"
        )
    if isinstance(new, Initial):
        return new
    if isinstance(old, Initial):
        return old
    if isinstance(old, Tentative) or isinstance(new, Tentative):
        return Tentative()
    return NoInitializer()


class TypeChecker:
    """Walks one program, populating `self.symbols`. The same
    instance is used for the whole program so the symbol table
    accumulates across all top-level declarations."""

    def __init__(self) -> None:
        self.symbols = SymbolTable()
        # Per-function context for `Return` checking. The current
        # language only has `Int` so this is unused for now, but the
        # field is kept so a future widening to richer return types
        # has a place to land.
        self._current_function: str | None = None

    def check_program(
        self, prog: c99_ast.Type_program,
    ) -> tuple[c99_ast.Type_program, SymbolTable]:
        match prog:
            case c99_ast.Program(declaration=decls):
                for d in decls:
                    self._check_file_scope_declaration(d)
                return prog, self.symbols
        raise TypeError(f"unexpected program: {prog!r}")

    # ------------------------------------------------------------------
    # File-scope declarations
    # ------------------------------------------------------------------

    def _check_file_scope_declaration(
        self, decl: c99_ast.Type_declaration,
    ) -> None:
        match decl:
            case c99_ast.VarDecl(var_decl=vd):
                self._check_file_scope_var(vd)
            case c99_ast.FunctionDecl(function_decl=fd):
                self._check_function_decl(fd, file_scope=True)
            case _:
                raise TypeError(f"unexpected declaration: {decl!r}")

    def _check_file_scope_var(
        self, vd: c99_ast.Type_var_decl,
    ) -> None:
        # File-scope linkage rules (per identifier_resolution):
        # `static`        → INTERNAL
        # `extern`        → matches prior visible (EXTERNAL if none)
        # no specifier    → EXTERNAL
        # The init-value rule keys off the storage class itself, not
        # the linkage:
        # `extern` + no init → NoInitializer (just a reference)
        # `extern` + init    → Initial(c) (definition by initializer)
        # else (no spec / static), no init → Tentative
        # else (no spec / static), init    → Initial(c)
        is_extern = isinstance(vd.storage_class, c99_ast.Extern)
        if vd.init is None:
            initial: InitialValue = (
                NoInitializer() if is_extern else Tentative()
            )
        else:
            initial = Initial(_const_init_value(vd.init, vd.name))
        # Recover linkage from the storage class. Recomputing here is
        # cleaner than threading the resolver's table through, and the
        # rules are the same the resolver applied:
        if isinstance(vd.storage_class, c99_ast.Static):
            linkage = Linkage.INTERNAL
        elif is_extern:
            # File-scope extern: matches prior visible if any.
            prior = self.symbols.get(vd.name)
            if prior is not None and isinstance(prior.attrs, StaticAttr):
                linkage = (
                    Linkage.EXTERNAL if prior.attrs.is_global
                    else Linkage.INTERNAL
                )
            else:
                linkage = Linkage.EXTERNAL
        else:
            linkage = Linkage.EXTERNAL
        is_global = _linkage_to_is_global(linkage)
        self._add_or_merge_static_object(
            name=vd.name,
            type_=Int(),
            initial=initial,
            is_global=is_global,
        )

    def _check_function_decl(
        self,
        fd: c99_ast.Type_function_decl,
        *,
        file_scope: bool,
    ) -> None:
        # Both declarations and definitions land here, distinguished
        # by `body is None`.
        ftype = FunType(
            params=tuple(Int() for _ in fd.params),
            ret=Int(),
        )
        defined = fd.body is not None
        # Linkage: file-scope follows static / extern / default rules;
        # block-scope is always EXTERNAL by virtue of the resolver
        # accepting only no-specifier and `extern`. The resolver also
        # rejects `static` at block scope, so we don't need to check
        # for it.
        if isinstance(fd.storage_class, c99_ast.Static):
            linkage = Linkage.INTERNAL
        else:
            # Default-or-extern: matches prior visible if any (only
            # meaningful at file scope; at block scope the prior
            # visible is also a function or unrelated and we can
            # only see EXTERNAL/INTERNAL via file-scope inheritance).
            prior = self.symbols.get(fd.name)
            if prior is not None and isinstance(prior.attrs, FunAttr):
                linkage = (
                    Linkage.EXTERNAL if prior.attrs.is_global
                    else Linkage.INTERNAL
                )
            else:
                linkage = Linkage.EXTERNAL
        is_global = _linkage_to_is_global(linkage)
        self._add_or_merge_function(
            name=fd.name,
            ftype=ftype,
            defined=defined,
            is_global=is_global,
        )
        if defined:
            # Walk the body. Parameters share the body's outermost
            # scope per §6.9.1.7 (so adding them to the symbol table
            # *before* the body, with LocalAttr, lets references
            # inside the body type-check against them).
            for p in fd.params:
                self.symbols[p] = Symbol(type=Int(), attrs=LocalAttr())
            saved = self._current_function
            self._current_function = fd.name
            assert fd.body is not None
            self._check_block(fd.body)
            self._current_function = saved
        # `file_scope` flag is currently unused — it'll matter once
        # the language grows constraints that differ between the
        # two scopes (e.g. inline / _Noreturn placement). Kept to
        # preserve call-site clarity.
        del file_scope

    # ------------------------------------------------------------------
    # Symbol-table merging
    # ------------------------------------------------------------------

    def _add_or_merge_static_object(
        self,
        *,
        name: str,
        type_: Type,
        initial: InitialValue,
        is_global: bool,
    ) -> None:
        existing = self.symbols.get(name)
        if existing is None:
            self.symbols[name] = Symbol(
                type=type_,
                attrs=StaticAttr(initial_value=initial, is_global=is_global),
            )
            return
        if not isinstance(existing.attrs, StaticAttr):
            raise TypeCheckError(
                f"{name!r} previously declared as a "
                f"{type(existing.attrs).__name__}, now redeclared as "
                f"a static-storage object"
            )
        if existing.type != type_:
            raise TypeCheckError(
                f"incompatible redeclaration of {name!r}: "
                f"previous {existing.type!r}, new {type_!r}"
            )
        if existing.attrs.is_global != is_global:
            # The resolver already rejects file-scope linkage changes,
            # but defense-in-depth here keeps the symbol table
            # internally consistent if a future caller skips the
            # resolver.
            raise TypeCheckError(
                f"linkage of {name!r} disagrees with prior "
                f"declaration"
            )
        merged = _merge_initial_value(
            name, existing.attrs.initial_value, initial,
        )
        self.symbols[name] = Symbol(
            type=type_,
            attrs=StaticAttr(initial_value=merged, is_global=is_global),
        )

    def _add_or_merge_function(
        self,
        *,
        name: str,
        ftype: FunType,
        defined: bool,
        is_global: bool,
    ) -> None:
        existing = self.symbols.get(name)
        if existing is None:
            self.symbols[name] = Symbol(
                type=ftype,
                attrs=FunAttr(defined=defined, is_global=is_global),
            )
            return
        if not isinstance(existing.attrs, FunAttr):
            raise TypeCheckError(
                f"{name!r} previously declared as a "
                f"{type(existing.attrs).__name__}, now redeclared as "
                f"a function"
            )
        if existing.type != ftype:
            raise TypeCheckError(
                f"incompatible redeclaration of {name!r}: "
                f"previous {existing.type!r}, new {ftype!r}"
            )
        if existing.attrs.is_global != is_global:
            raise TypeCheckError(
                f"linkage of {name!r} disagrees with prior "
                f"declaration"
            )
        if defined and existing.attrs.defined:
            raise TypeCheckError(
                f"redefinition of function {name!r}"
            )
        # `defined` only flips False → True; `is_global` is already
        # checked equal above.
        new_defined = existing.attrs.defined or defined
        self.symbols[name] = Symbol(
            type=ftype,
            attrs=FunAttr(defined=new_defined, is_global=is_global),
        )

    # ------------------------------------------------------------------
    # Block items
    # ------------------------------------------------------------------

    def _check_block(self, block: c99_ast.Type_block) -> None:
        match block:
            case c99_ast.Block(block_item=items):
                for item in items:
                    self._check_block_item(item)
                return
        raise TypeError(f"unexpected block: {block!r}")

    def _check_block_item(
        self, item: c99_ast.Type_block_item,
    ) -> None:
        match item:
            case c99_ast.S(statement=stmt):
                self._check_statement(stmt)
                return
            case c99_ast.D(declaration=decl):
                self._check_block_declaration(decl)
                return
        raise TypeError(f"unexpected block item: {item!r}")

    def _check_block_declaration(
        self, decl: c99_ast.Type_declaration,
    ) -> None:
        match decl:
            case c99_ast.VarDecl(var_decl=vd):
                self._check_block_var(vd)
                return
            case c99_ast.FunctionDecl(function_decl=fd):
                # Block-scope function declarations have no body
                # (C99 forbids nested function definitions). Treat
                # them like file-scope decls but with `file_scope=
                # False` for future-proofing.
                self._check_function_decl(fd, file_scope=False)
                return
        raise TypeError(f"unexpected declaration: {decl!r}")

    def _check_block_var(
        self, vd: c99_ast.Type_var_decl,
    ) -> None:
        # Block-scope variable declarations split three ways:
        #   `extern int x;`  → reference; static storage, linkage from
        #                      prior visible. NO initializer allowed
        #                      (§6.7.8.5). Recorded as StaticAttr with
        #                      NoInitializer iff this is the first
        #                      declaration for the name; otherwise
        #                      merged with the prior entry.
        #   `static int x;`  → static-storage local, NONE linkage,
        #                      module-private (is_global=False). The
        #                      default initializer is zero; an explicit
        #                      initializer must be a constant
        #                      expression. Each block-scope `static`
        #                      gets its own unique resolved name from
        #                      identifier_resolution, so symbol-table
        #                      collisions don't happen.
        #   plain `int x;`   → automatic storage, LocalAttr. The
        #                      initializer is a regular runtime
        #                      expression and is type-checked here;
        #                      c99_to_tac lowers it to a `Copy` at
        #                      the declaration site.
        if isinstance(vd.storage_class, c99_ast.Extern):
            if vd.init is not None:
                raise TypeCheckError(
                    f"block-scope `extern` declaration of {vd.name!r} "
                    f"may not have an initializer"
                )
            # Linkage matches prior visible (resolver already enforced
            # this, but we recompute from the symbol table to set
            # is_global). If no prior is in the symbol table yet, the
            # resolver would have given it EXTERNAL.
            prior = self.symbols.get(vd.name)
            if prior is not None and isinstance(prior.attrs, StaticAttr):
                is_global = prior.attrs.is_global
            elif prior is not None and isinstance(prior.attrs, FunAttr):
                raise TypeCheckError(
                    f"{vd.name!r} previously declared as a function, "
                    f"now redeclared as an object"
                )
            else:
                is_global = True
            self._add_or_merge_static_object(
                name=vd.name,
                type_=Int(),
                initial=NoInitializer(),
                is_global=is_global,
            )
            return
        if isinstance(vd.storage_class, c99_ast.Static):
            # Block-scope static: NONE linkage → is_global=False.
            # Resolver gave the name a unique `@<N>.<orig>` so the
            # symbol-table key won't collide with any other
            # static-storage object.
            if vd.init is None:
                initial: InitialValue = Initial(0)
            else:
                initial = Initial(_const_init_value(vd.init, vd.name))
            self.symbols[vd.name] = Symbol(
                type=Int(),
                attrs=StaticAttr(initial_value=initial, is_global=False),
            )
            return
        # Plain `int x;` — automatic storage. The initializer is a
        # runtime expression; type-check it here.
        self.symbols[vd.name] = Symbol(type=Int(), attrs=LocalAttr())
        if vd.init is not None:
            init_type = self._check_exp(vd.init)
            if init_type != Int():
                raise TypeCheckError(
                    f"cannot initialize variable {vd.name!r} of type "
                    f"Int with value of type {init_type}"
                )

    # ------------------------------------------------------------------
    # Statements / expressions
    # ------------------------------------------------------------------

    def _check_statement(
        self, stmt: c99_ast.Type_statement,
    ) -> None:
        match stmt:
            case c99_ast.Return(exp=exp):
                t = self._check_exp(exp)
                if t != Int():
                    raise TypeCheckError(
                        f"return value of type {t}, expected Int"
                    )
                return
            case c99_ast.Expression(exp=exp):
                self._check_exp(exp)
                return
            case c99_ast.IfStmt(
                condition=cond, then_clause=then_, else_clause=else_,
            ):
                self._check_exp(cond)
                self._check_statement(then_)
                if else_ is not None:
                    self._check_statement(else_)
                return
            case c99_ast.Compound(block=block):
                self._check_block(block)
                return
            case c99_ast.WhileStmt(condition=cond, body=body):
                self._check_exp(cond)
                self._check_statement(body)
                return
            case c99_ast.DoWhileStmt(body=body, condition=cond):
                self._check_statement(body)
                self._check_exp(cond)
                return
            case c99_ast.ForStmt(
                init=init, condition=cond, post_clause=post,
                body=body,
            ):
                self._check_for_init(init)
                if cond is not None:
                    self._check_exp(cond)
                if post is not None:
                    self._check_exp(post)
                self._check_statement(body)
                return
            case c99_ast.LabeledStmt(statement=inner):
                self._check_statement(inner)
                return
            case (
                c99_ast.Goto()
                | c99_ast.BreakStmt()
                | c99_ast.ContinueStmt()
                | c99_ast.Null()
            ):
                # Pure control flow: no expressions to type-check.
                return
        raise TypeError(f"unexpected statement: {stmt!r}")

    def _check_for_init(
        self, init: c99_ast.Type_for_init,
    ) -> None:
        match init:
            case c99_ast.InitDecl(var_decl=vd):
                # The for-init-decl rule (resolver) forbids storage-
                # class specifiers, so this is always plain
                # `int <name> = <exp>;` and lands as a LocalAttr.
                self.symbols[vd.name] = Symbol(
                    type=Int(), attrs=LocalAttr(),
                )
                if vd.init is not None:
                    init_type = self._check_exp(vd.init)
                    if init_type != Int():
                        raise TypeCheckError(
                            f"cannot initialize variable {vd.name!r} "
                            f"of type Int with value of type "
                            f"{init_type}"
                        )
                return
            case c99_ast.InitExp(exp=exp):
                if exp is not None:
                    self._check_exp(exp)
                return
        raise TypeError(f"unexpected for_init: {init!r}")

    def _check_exp(self, exp: c99_ast.Type_exp) -> Type:
        match exp:
            case c99_ast.Constant():
                return Int()
            case c99_ast.Var(name=name):
                sym = self.symbols.get(name)
                if sym is None:
                    raise TypeCheckError(
                        f"undeclared identifier {name!r}"
                    )
                if isinstance(sym.type, FunType):
                    raise TypeCheckError(
                        f"function {name!r} used as a variable"
                    )
                return sym.type
            case c99_ast.Unary(exp=inner):
                t = self._check_exp(inner)
                if t != Int():
                    raise TypeCheckError(
                        f"unary operator on non-Int type {t}"
                    )
                return Int()
            case c99_ast.Binary(left=lhs, right=rhs):
                tl = self._check_exp(lhs)
                tr = self._check_exp(rhs)
                if tl != Int() or tr != Int():
                    raise TypeCheckError(
                        f"binary operator on non-Int types: "
                        f"{tl}, {tr}"
                    )
                return Int()
            case c99_ast.Assignment(lval=lv, rval=rv):
                tl = self._check_exp(lv)
                tr = self._check_exp(rv)
                if tl != tr:
                    raise TypeCheckError(
                        f"assignment type mismatch: "
                        f"target {tl}, value {tr}"
                    )
                return tl
            case c99_ast.Postfix(operand=op):
                t = self._check_exp(op)
                if t != Int():
                    raise TypeCheckError(
                        f"postfix operator on non-Int type {t}"
                    )
                return Int()
            case c99_ast.Conditional(
                condition=cond,
                true_clause=t_clause,
                false_clause=f_clause,
            ):
                self._check_exp(cond)
                tt = self._check_exp(t_clause)
                tf = self._check_exp(f_clause)
                if tt != tf:
                    raise TypeCheckError(
                        f"conditional branches have mismatched "
                        f"types: {tt}, {tf}"
                    )
                return tt
            case c99_ast.FunctionCall(name=name, args=args):
                sym = self.symbols.get(name)
                if sym is None:
                    raise TypeCheckError(
                        f"undeclared identifier {name!r}"
                    )
                if not isinstance(sym.type, FunType):
                    raise TypeCheckError(
                        f"variable {name!r} called as a function"
                    )
                if len(args) != len(sym.type.params):
                    plural = "s" if len(args) != 1 else ""
                    raise TypeCheckError(
                        f"function {name!r} called with {len(args)} "
                        f"argument{plural}, expected "
                        f"{len(sym.type.params)}"
                    )
                for i, (arg, expected) in enumerate(
                    zip(args, sym.type.params),
                ):
                    actual = self._check_exp(arg)
                    if actual != expected:
                        raise TypeCheckError(
                            f"argument {i + 1} of call to {name!r}: "
                            f"got {actual}, expected {expected}"
                        )
                return sym.type.ret
        raise TypeError(f"unexpected exp: {exp!r}")


def check_program(
    prog: c99_ast.Type_program,
) -> tuple[c99_ast.Type_program, SymbolTable]:
    """Type-check a c99 program. Returns the (unchanged) AST plus
    the populated SymbolTable. Raises `TypeCheckError` on any type
    error encountered."""
    return TypeChecker().check_program(prog)
