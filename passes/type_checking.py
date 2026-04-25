"""Type checking pass: c99_ast -> (c99_ast, SymbolTable).

Walks the program once after identifier_resolution, label_resolution,
and loop_labeling have all run. Validates that every identifier is
used in a way consistent with its declaration, and produces a
`SymbolTable` mapping each program-unique identifier to its type
(plus a `defined` flag for functions). The SymbolTable is the
canonical "what does this name mean?" source for every later pass —
codegen will need at least the function arity / return-type info,
and any future linking step will need the defined/declared
distinction.

Type vocabulary today
---------------------
- `Int()`: the only object type. Every variable, every parameter,
  every constant, every binary/unary/postfix result.
- `FunType(params=tuple[Type, ...], ret=Type)`: function types.
  Every function we accept right now has return type `Int()` and a
  list of `Int()` parameters. Constructor stores params as a tuple
  so the dataclass stays hashable / value-comparable.

Both `Type` subclasses are frozen dataclasses, so `Int() == Int()`
and `hash(Int())` work without any boilerplate. Equality on
`FunType` is structural — two function declarations with the same
parameter count match because their `params` tuples compare equal
element-wise.

Errors raised (`TypeCheckError`)
--------------------------------
- **Function used as a variable**: `int foo(void); int x = foo;`.
  A `Var(name)` reference where the symbol is a `FunType`.
- **Variable called as a function**: `int x; x();`. A `FunctionCall
  (name, ...)` where the symbol is non-`FunType`.
- **Wrong arity**: `int foo(int a, int b); foo(1);`. Number of
  arguments doesn't match the declared parameter count.
- **Argument type mismatch**: today every arg/param is `Int`, so
  this only fires from synthetic ASTs; once richer types land,
  this is the natural place to enforce passing-compatibility.
- **Incompatible redeclaration of a function**: `int foo(int a);
  int foo(int a, int b);` — two declarations of the same function
  with different signatures.
- **Redefinition of a function**: `int foo(void) { return 1; }
  int foo(void) { return 2; }`. Strictly a linker concern in C,
  but the symbol table makes it cheap to catch here.
- **Identifier kind switched**: `int foo; int foo(void);` (variable
  redeclared as function). Caught when `add_function` finds the
  existing entry isn't a `FunType`.
- **Initializer type mismatch**: `int x = some_func;` — a variable's
  initializer doesn't match its declared type.

Symbol table semantics
----------------------
- `add_variable(name, type)`: NONE-linkage names are unique after
  identifier_resolution, so a duplicate add is an internal-
  consistency bug rather than a user error.
- `add_function(name, type, defined)`: handles the multi-decl /
  decl-plus-def cases. First add creates the entry; subsequent
  adds validate signature equality and update `defined` (which
  transitions False → True on the first definition and raises if
  it'd transition True → True).

The pass does **not** modify the AST. Its return contract is the
input program (returned as-is) plus the populated SymbolTable;
callers thread the table to later passes that need it.

Where this pass sits
--------------------
After identifier_resolution / label_resolution / loop_labeling so
the AST is name-stable. Before c99_to_tac so the type checker can
reject anything c99_to_tac couldn't lower (a call to a non-function
name, a function used as a value — neither has a TAC representation).
"""

from __future__ import annotations

from dataclasses import dataclass

import c99_ast


class TypeCheckError(Exception):
    """Raised on any type-level inconsistency in the program."""


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


@dataclass
class Symbol:
    """One row of the symbol table. `defined` is meaningful only
    when `type` is a `FunType` — True iff a definition (with body)
    has been seen for this function name. For NONE-linkage variables
    today, every declaration is also a definition (block-scope
    automatic storage), so `defined` stays False (the field is
    irrelevant for them). When `extern` / `static` objects land,
    `defined` will mean the same thing for them as for functions."""
    type: Type
    defined: bool = False


class SymbolTable:
    """Flat dict[str, Symbol] keyed by resolved identifier name.

    identifier_resolution gives every NONE-linkage name a unique
    `@<N>.<orig>` and leaves every EXTERNAL-linkage name (functions
    today) at its source spelling, so a single dict is enough — no
    nested scopes. The table is built up in source-program order
    by a single TypeChecker walk and then exposed to later passes.
    """

    def __init__(self) -> None:
        self._table: dict[str, Symbol] = {}

    def __contains__(self, name: str) -> bool:
        return name in self._table

    def __getitem__(self, name: str) -> Symbol:
        return self._table[name]

    def get(self, name: str) -> Symbol | None:
        return self._table.get(name)

    def items(self):
        return self._table.items()

    def __len__(self) -> int:
        return len(self._table)

    def __repr__(self) -> str:
        return f"SymbolTable({self._table!r})"

    def add_variable(self, name: str, t: Type) -> None:
        """Record a variable declaration. Raises if `name` is
        already present — variable names are globally unique after
        identifier_resolution, so a duplicate add is an internal
        consistency error, not a user-level one."""
        if name in self._table:
            raise TypeCheckError(
                f"internal: variable {name!r} already in symbol "
                f"table (identifier_resolution should have made "
                f"every variable name unique)"
            )
        self._table[name] = Symbol(type=t)

    def add_function(
        self, name: str, t: FunType, defined: bool,
    ) -> None:
        """Record a function declaration or definition. Validates
        signature compatibility with any prior entry and updates
        the `defined` flag, raising on a second definition or a
        switch of identifier kind."""
        existing = self._table.get(name)
        if existing is None:
            self._table[name] = Symbol(type=t, defined=defined)
            return
        # Re-encountering a previously-seen name. Whether or not it
        # was a function before, the new add must agree with the
        # old one for the program to type-check.
        if not isinstance(existing.type, FunType):
            raise TypeCheckError(
                f"{name!r} previously declared as {existing.type!r}, "
                f"now redeclared as a function"
            )
        if existing.type != t:
            raise TypeCheckError(
                f"incompatible redeclaration of {name!r}: "
                f"previous {existing.type!r}, new {t!r}"
            )
        if defined and existing.defined:
            raise TypeCheckError(
                f"redefinition of function {name!r}"
            )
        if defined:
            existing.defined = True


class TypeChecker:
    """Walks one program, populating `self.symbols`. The same
    instance is used for the whole program so the symbol table
    accumulates across all top-level functions."""

    def __init__(self) -> None:
        self.symbols = SymbolTable()

    def check_program(
        self, prog: c99_ast.Type_program,
    ) -> tuple[c99_ast.Type_program, SymbolTable]:
        match prog:
            case c99_ast.Program(function_definition=fns):
                for fn in fns:
                    self.check_function(fn)
                return prog, self.symbols
        raise TypeError(f"unexpected program: {prog!r}")

    def check_function(
        self, fn: c99_ast.Type_function_definition,
    ) -> None:
        match fn:
            case c99_ast.Function(name=name, params=params, body=body):
                # Register the function with `defined=True` *before*
                # checking its body so the body can recurse. Today
                # every function is `int -> int -> ... -> int`.
                ftype = FunType(
                    params=tuple(Int() for _ in params),
                    ret=Int(),
                )
                self.symbols.add_function(name, ftype, defined=True)
                # Each parameter goes in the table as Int. Their
                # resolved names are unique program-wide, so there's
                # no clash even when multiple functions share param
                # names like `x`.
                for p in params:
                    self.symbols.add_variable(p, Int())
                self.check_block(body)
                return
        raise TypeError(f"unexpected function: {fn!r}")

    def check_block(self, block: c99_ast.Type_block) -> None:
        match block:
            case c99_ast.Block(block_item=items):
                for item in items:
                    self.check_block_item(item)
                return
        raise TypeError(f"unexpected block: {block!r}")

    def check_block_item(
        self, item: c99_ast.Type_block_item,
    ) -> None:
        match item:
            case c99_ast.S(statement=stmt):
                self.check_statement(stmt)
                return
            case c99_ast.D(declaration=decl):
                self.check_declaration(decl)
                return
        raise TypeError(f"unexpected block item: {item!r}")

    def check_declaration(
        self, decl: c99_ast.Type_declaration,
    ) -> None:
        match decl:
            case c99_ast.VarDecl(var_decl=vd):
                self.symbols.add_variable(vd.name, Int())
                if vd.init is not None:
                    init_type = self.check_exp(vd.init)
                    if init_type != Int():
                        raise TypeCheckError(
                            f"cannot initialize variable {vd.name!r} "
                            f"of type Int with value of type "
                            f"{init_type}"
                        )
                return
            case c99_ast.FunctionDecl(function_decl=fd):
                ftype = FunType(
                    params=tuple(Int() for _ in fd.params),
                    ret=Int(),
                )
                self.symbols.add_function(
                    fd.name, ftype, defined=False,
                )
                # Don't add the param names to the symbol table —
                # they're not in any usable scope (block-scope
                # function declarations have no body). Their unique
                # names exist purely so the signature is preserved
                # in the AST for the type checker; the table
                # entries would never be looked up.
                return
        raise TypeError(f"unexpected declaration: {decl!r}")

    def check_statement(
        self, stmt: c99_ast.Type_statement,
    ) -> None:
        match stmt:
            case c99_ast.Return(exp=exp):
                # All functions return Int today; this widens to
                # "matches the enclosing function's declared return
                # type" once we have richer return types. The
                # enclosing function isn't tracked yet because the
                # check is currently trivial.
                t = self.check_exp(exp)
                if t != Int():
                    raise TypeCheckError(
                        f"return value of type {t}, expected Int"
                    )
                return
            case c99_ast.Expression(exp=exp):
                self.check_exp(exp)
                return
            case c99_ast.IfStmt(
                condition=cond, then_clause=then_, else_clause=else_,
            ):
                self.check_exp(cond)
                self.check_statement(then_)
                if else_ is not None:
                    self.check_statement(else_)
                return
            case c99_ast.Compound(block=block):
                self.check_block(block)
                return
            case c99_ast.WhileStmt(condition=cond, body=body):
                self.check_exp(cond)
                self.check_statement(body)
                return
            case c99_ast.DoWhileStmt(body=body, condition=cond):
                self.check_statement(body)
                self.check_exp(cond)
                return
            case c99_ast.ForStmt(
                init=init, condition=cond, post_clause=post,
                body=body,
            ):
                self.check_for_init(init)
                if cond is not None:
                    self.check_exp(cond)
                if post is not None:
                    self.check_exp(post)
                self.check_statement(body)
                return
            case c99_ast.LabeledStmt(statement=inner):
                self.check_statement(inner)
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

    def check_for_init(
        self, init: c99_ast.Type_for_init,
    ) -> None:
        match init:
            case c99_ast.InitDecl(var_decl=vd):
                self.symbols.add_variable(vd.name, Int())
                if vd.init is not None:
                    init_type = self.check_exp(vd.init)
                    if init_type != Int():
                        raise TypeCheckError(
                            f"cannot initialize variable {vd.name!r} "
                            f"of type Int with value of type "
                            f"{init_type}"
                        )
                return
            case c99_ast.InitExp(exp=exp):
                if exp is not None:
                    self.check_exp(exp)
                return
        raise TypeError(f"unexpected for_init: {init!r}")

    def check_exp(self, exp: c99_ast.Type_exp) -> Type:
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
                t = self.check_exp(inner)
                if t != Int():
                    raise TypeCheckError(
                        f"unary operator on non-Int type {t}"
                    )
                return Int()
            case c99_ast.Binary(left=lhs, right=rhs):
                tl = self.check_exp(lhs)
                tr = self.check_exp(rhs)
                if tl != Int() or tr != Int():
                    raise TypeCheckError(
                        f"binary operator on non-Int types: "
                        f"{tl}, {tr}"
                    )
                return Int()
            case c99_ast.Assignment(lval=lv, rval=rv):
                tl = self.check_exp(lv)
                tr = self.check_exp(rv)
                if tl != tr:
                    raise TypeCheckError(
                        f"assignment type mismatch: "
                        f"target {tl}, value {tr}"
                    )
                return tl
            case c99_ast.Postfix(operand=op):
                t = self.check_exp(op)
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
                self.check_exp(cond)
                tt = self.check_exp(t_clause)
                tf = self.check_exp(f_clause)
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
                    actual = self.check_exp(arg)
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
