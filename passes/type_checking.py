"""Type checking pass: c99_ast -> (c99_ast, SymbolTable).

Walks the program once after identifier_resolution / label_resolution /
loop_labeling have all run. Validates that every identifier is used in
a way consistent with its declaration, computes each declaration's
initial value (for objects with static storage duration) and
defined-ness (for functions), and produces a `SymbolTable` keyed by
each identifier's resolved name.

The SymbolTable is the canonical "what does this name mean?" source for
every later pass:
  * `c99_to_tac` consumes it to set `is_global` on each TAC `Function`,
    and to enumerate every static-storage-duration object at the end
    of program translation.
  * later codegen passes will use it to distinguish module-local
    NONE-linkage statics from translation-unit-global EXTERNAL
    symbols, and (eventually) to size each operand for 16-bit `long`.

Type vocabulary
---------------
The data-type hierarchy lives on the c99 AST (`c99_ast.Int`,
`c99_ast.Long`, `c99_ast.UInt`, `c99_ast.ULong`, `c99_ast.FunType`).
Every var_decl / function_decl already carries its declared type,
and the parser puts those nodes straight into the AST. This module
imports them directly and re-exports the names so existing callers
(and unit tests) can keep writing `from passes.type_checking
import Int, FunType`. Equality is structural via `@dataclass`
defaults, which is what we need for type comparisons.

`Int()` is 1-byte signed (-128..127), `Long()` is 2-byte signed
(-32768..32767), `UInt()` is 1-byte unsigned (0..255), and
`ULong()` is 2-byte unsigned (0..65535). `FunType(params, ret)`
describes a function's signature. `Type` is the marker base — the
`c99_ast.Type_data_type` it aliases is what identifier_resolution
leaves on every var_decl / function_decl AST node.

Implicit conversions
--------------------
Mixed-type arithmetic is handled by C99's usual arithmetic
conversions (§6.3.1.8): the narrower or signed-displaceable operand
is wrapped in an implicit `Cast` to the common type, so by the time
TAC sees the tree every operand has its concrete data_type and any
size- or signedness-changing conversion is an explicit Cast node.

Cast expressions are accepted in any direction (Int→Long, Long→Int,
Int→Int, Long→Long); the conversion itself is the codegen's
problem. The cast's target type is what the type checker reports
for the surrounding expression.

Symbol attributes
-----------------
A `Symbol` carries a `type` plus an `IdAttr` describing how the
symbol exists at runtime:

- `LocalAttr`: an automatic-storage object — block-scope `int x;` or
  `long x;`, function parameter. Lives on the soft stack with a
  fresh slot per function activation.
- `StaticAttr(initial_value, is_global)`: an object with static
  storage duration. Covers every file-scope object plus block-scope
  `static`. `initial_value` is one of `Initial(c)`, `Tentative`, or
  `NoInitializer`; `is_global` is True iff the symbol has external
  linkage.
- `FunAttr(defined, is_global)`: a function name. `defined` flips
  to True the first time we see a definition; subsequent definitions
  raise.

Initial-value rules (C99 §6.7.8 / §6.9.2)
-----------------------------------------
Same shape as before, but the expected initializer type now comes
from the var_decl's declared `data_type`:
- File-scope `T x;` (no init, no extern) → `Tentative`.
- File-scope `T x = c;` → `Initial(c)`. Constant-expression check.
- File-scope `extern T x;` → `NoInitializer`.
- File-scope `extern T x = c;` → `Initial(c)`.
- Block-scope `static T x;` (no init) → `Initial(0)` per §6.7.8.10.
- Block-scope `static T x = c;` → `Initial(c)`.
- Block-scope `extern T x;` → `NoInitializer`.
- Block-scope `T x [= e];` → `LocalAttr` (no init tracked here; the
  TAC pass lowers it as a runtime `Copy`).

Static-storage initializers must be constant expressions of a type
compatible with the variable's declared type. Today the parser only
produces ConstInt/ConstLong for integer literals; a Long-typed
static initialized with a ConstInt (or vice versa) is rejected here
unless the user wraps it in an explicit cast.

Errors raised (`TypeCheckError`)
--------------------------------
- Function used as a variable (`Var(name)` resolving to a `FunType`).
- Variable called as a function.
- Wrong call arity, or argument type mismatch (now meaningful with
  Int / Long).
- Mismatched binary-operator operand types.
- Mismatched assignment / conditional-branch types.
- Initializer type doesn't match the variable's declared type.
- Return value's type doesn't match the enclosing function's return
  type.
- Static-storage initializer isn't a constant expression.
- Cast target type isn't `Int` or `Long` (no `FunType` casts).
- Multiple definitions of the same object / function.
- Incompatible redeclaration of a function (signature differs).
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
#
# We re-export the c99_ast data-type classes as `Type` / `Int` /
# `Long` / `FunType` so every consumer (this pass, c99_to_tac, the
# codegen passes, the unit tests) can refer to them under stable
# `passes.type_checking.<Name>` names. The aliases are pure re-exports:
# constructing `Int()` from either module returns an instance of the
# same underlying dataclass, so `is`-checks and `==` comparisons
# behave identically across imports.

Type = c99_ast.Type_data_type
Int = c99_ast.Int
Long = c99_ast.Long
UInt = c99_ast.UInt
ULong = c99_ast.ULong
Float = c99_ast.Float
Double = c99_ast.Double
FunType = c99_ast.FunType
Pointer = c99_ast.Pointer


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
    """Object declared with an initializer. The value carries the
    constant from the initializer expression — an `int` for the
    four integer types, a `float` (Python double-precision) for
    `Float` / `Double`. The variable's declared type tells codegen
    which `StaticVariable` width to emit; the same numeric value
    can mean different things in different widths, and Float vs.
    Double also pick different IEEE 754 byte sequences."""
    value: int | float


@dataclass(frozen=True)
class NoInitializer(InitialValue):
    """`extern T x;` — the declaration is a reference to a symbol
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
    """Automatic-storage object — block-scope `int x;` / `long x;`
    or function parameter. No `is_global` (it isn't visible across
    TUs / can't be), no `initial_value` (the initializer, if any, is
    lowered as a regular TAC `Copy` at the declaration's source
    position)."""


@dataclass(frozen=True)
class StaticAttr(IdAttr):
    """An object with static storage duration. Covers every file-
    scope object and every block-scope `static`. `is_global` is True
    iff the symbol has external linkage; the codegen will use it to
    decide between a TU-local label and a global symbol.
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
    """Flat dict[str, Symbol] keyed by resolved identifier name."""

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


def _const_init_value(
    exp: c99_ast.Type_exp, name: str,
) -> int | float:
    """Static-storage initializers must be compile-time constant
    expressions (C99 §6.7.8.4). After `_check_exp` and the
    initializer-conversion rule have run, the AST shape is one of:
      * a `Constant(ConstInt|ConstLong|ConstUInt|ConstULong|
        ConstFloat|ConstDouble)` — variant doesn't have to match
        the variable's declared type, since `_convert_to(...)` has
        already wrapped a mismatch in a `Cast`.
      * a `Cast` (possibly nested) wrapping a Constant — produced
        by `_convert_to` for narrowing/widening initializers, or
        explicitly written by the user.

    Both shapes reduce to the underlying value (int for the four
    integer variants, float for ConstFloat / ConstDouble). The
    Cast target's type tells codegen the storage width when laying
    out the StaticVariable; the raw value passes through, and the
    declared-type-driven conversion (e.g. int constant initializer
    for a Float static) happens in c99_to_tac when it builds the
    typed `*Init` node.
    """
    match exp:
        case c99_ast.Constant(const=c):
            if isinstance(c, (c99_ast.ConstFloat, c99_ast.ConstDouble)):
                return c.float
            return c.int
        case c99_ast.Cast(exp=inner):
            return _const_init_value(inner, name)
    raise TypeCheckError(
        f"initializer for static-storage object {name!r} is not a "
        f"constant expression"
    )


def _types_equal(a: Type, b: Type) -> bool:
    """Structural type equality. The asdl-generated dataclasses use
    `@dataclass` (eq=True), so `Int() == Int()` and
    `Long() == Long()` work out of the box; FunType's structural
    comparison covers params + ret recursively."""
    return a == b


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


def _is_object_type(t: Type) -> bool:
    """True iff `t` is a value type that can name an object — per
    C99 §6.2.5, "A pointer type ... is an object type" along with
    the arithmetic types. Excludes only FunType (a function isn't
    an object). Used at the boundary where we expect an object —
    variable references, cast targets, arithmetic operands."""
    return isinstance(t, (Int, Long, UInt, ULong, Float, Double, Pointer))


def _is_integer_type(t: Type) -> bool:
    return isinstance(t, (Int, Long, UInt, ULong))


def _is_floating_type(t: Type) -> bool:
    return isinstance(t, (Float, Double))


def _is_pointer_type(t: Type) -> bool:
    return isinstance(t, Pointer)


def _is_null_pointer_constant(exp: c99_ast.Type_exp) -> bool:
    """Detect a null pointer constant per C99 §6.3.2.3.3 — "an
    integer constant expression with the value 0, or such an
    expression cast to type void *". c6502 has no constant-folding
    pass, so we recognize the literal form: a `Constant` whose
    integer-variant `const` is 0, optionally wrapped in any number
    of `Cast`s. (`(int)0`, `(long)0`, `((void *)0)` once void
    pointers exist, etc.) Used at the type-check boundary for
    pointer equality — `p == 0` and `0 == p` are legal even though
    the bare types don't match."""
    while isinstance(exp, c99_ast.Cast):
        exp = exp.exp
    if not isinstance(exp, c99_ast.Constant):
        return False
    c = exp.const
    if isinstance(
        c,
        (c99_ast.ConstInt, c99_ast.ConstLong,
         c99_ast.ConstUInt, c99_ast.ConstULong),
    ):
        return c.int == 0
    return False


def _is_arithmetic_type(t: Type) -> bool:
    """True iff `t` is integer or floating — the types that
    participate in C99 §6.3.1.8 usual arithmetic conversions.
    Excludes Pointer, even though Pointer is an object type."""
    return _is_integer_type(t) or _is_floating_type(t)


# Width and signedness predicates for the four integer types. Width
# determines the C99 §6.3.1.1 "rank" (Int and UInt share rank 1;
# Long and ULong share rank 2 — `long long` and friends would be
# rank 3 but c6502 doesn't model them).
def _int_width(t: Type) -> int:
    if isinstance(t, (Int, UInt)):
        return 1
    if isinstance(t, (Long, ULong)):
        return 2
    raise TypeError(f"_int_width: not an integer object type: {t!r}")


def _is_signed(t: Type) -> bool:
    return isinstance(t, (Int, Long))


def _common_type(a: Type, b: Type) -> Type:
    """Usual arithmetic conversions per C99 §6.3.1.8 paragraph 1.

    Floating types dominate per §6.3.1.8.1:
      * either operand `Double` → result `Double`
      * else either operand `Float` → result `Float`
      * else both operands integer → integer rules (below)

    Integer rules, restricted to the four types c6502 models. With
    ranks 1 (Int/UInt) and 2 (Long/ULong):
      * matching types               → that type
      * both signed (or both unsigned) → the higher-rank type wins
      * mixed; unsigned has rank ≥ signed → unsigned wins
      * mixed; signed has higher rank and can represent all of the
        unsigned type's range                → signed wins
        (Long can represent UInt's 0..255, so Long + UInt → Long)
      * otherwise → unsigned counterpart of the signed type
        (no such case arises in c6502 today since Long always
        represents UInt; kept for forward compatibility.)

    Returned types are fresh instances so callers can attach them
    to AST nodes without aliasing."""
    if isinstance(a, Double) or isinstance(b, Double):
        return Double()
    if isinstance(a, Float) or isinstance(b, Float):
        return Float()
    if _types_equal(a, b):
        return type(a)()
    a_signed, b_signed = _is_signed(a), _is_signed(b)
    a_rank, b_rank = _int_width(a), _int_width(b)
    if a_signed == b_signed:
        # Both signed or both unsigned — higher rank wins.
        return type(a)() if a_rank >= b_rank else type(b)()
    # Mixed signedness.
    signed, unsigned = (a, b) if a_signed else (b, a)
    if _int_width(unsigned) >= _int_width(signed):
        return type(unsigned)()
    # Signed has the higher rank. The C99 rule asks whether it
    # can represent every value of the unsigned type. With our
    # types the only mixed-rank case is Long (signed, rank 2) +
    # UInt (unsigned, rank 1), and Long's range -32768..32767
    # spans UInt's 0..255 entirely, so the signed type wins.
    return type(signed)()


def _convert_to(exp: c99_ast.Type_exp, target: Type) -> c99_ast.Type_exp:
    """If `exp.data_type` already equals `target`, return `exp` as-is.
    Otherwise wrap it in an implicit `Cast(target, exp)` and tag the
    Cast with `target` as its data_type. The wrapper is what TAC /
    codegen will see, so every operand reaching the back end has a
    self-describing type and any size-changing conversion is an
    explicit Cast node."""
    if exp.data_type is not None and _types_equal(exp.data_type, target):
        return exp
    cast = c99_ast.Cast(target_type=target, exp=exp, data_type=target)
    return cast


class TypeChecker:
    """Walks one program, populating `self.symbols`. The same
    instance is used for the whole program so the symbol table
    accumulates across all top-level declarations."""

    def __init__(self) -> None:
        self.symbols = SymbolTable()
        # Type the enclosing function should return — set by
        # `_check_function_decl` while walking a body, restored on
        # exit. Used by `_check_statement` to type-check `return`s.
        self._return_type: Type | None = None

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
        if not _is_object_type(vd.data_type):
            raise TypeCheckError(
                f"file-scope object {vd.name!r} declared with non-"
                f"object type {vd.data_type!r}"
            )
        is_extern = isinstance(vd.storage_class, c99_ast.Extern)
        if vd.init is None:
            initial: InitialValue = (
                NoInitializer() if is_extern else Tentative()
            )
        else:
            # Type-check the initializer normally (sets data_type
            # on every node) and convert to the declared type if
            # they differ — same shape as Assignment / Return /
            # arg conversion. After this, `vd.init` is either a
            # bare Constant (variant matches declared type) or
            # `Cast(declared_type, original_init)` for the
            # mismatched case. `_const_init_value` then drills
            # through any Cast wrappers to the underlying integer.
            self._check_exp(vd.init)
            vd.init = _convert_to(vd.init, vd.data_type)
            initial = Initial(_const_init_value(vd.init, vd.name))
        # Recover linkage from the storage class.
        if isinstance(vd.storage_class, c99_ast.Static):
            linkage = Linkage.INTERNAL
        elif is_extern:
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
            type_=vd.data_type,
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
        ftype = fd.data_type
        if not isinstance(ftype, FunType):
            raise TypeError(
                f"function decl {fd.name!r} has non-FunType "
                f"data_type {ftype!r}"
            )
        defined = fd.body is not None
        # Linkage: file-scope follows static / extern / default rules;
        # block-scope is always EXTERNAL by virtue of the resolver
        # accepting only no-specifier and `extern` for functions.
        if isinstance(fd.storage_class, c99_ast.Static):
            linkage = Linkage.INTERNAL
        else:
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
            # inside the body type-check against them). Each
            # parameter's type comes from the FunType's params list,
            # paired with the param name in the function_decl's
            # `params` array.
            for p_name, p_type in zip(fd.params, ftype.params):
                self.symbols[p_name] = Symbol(
                    type=p_type, attrs=LocalAttr(),
                )
            saved = self._return_type
            self._return_type = ftype.ret
            assert fd.body is not None
            self._check_block(fd.body)
            self._return_type = saved
        # `file_scope` flag is currently unused — it'll matter once
        # the language grows constraints that differ between the
        # two scopes. Kept for call-site clarity.
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
        if not _types_equal(existing.type, type_):
            raise TypeCheckError(
                f"incompatible redeclaration of {name!r}: "
                f"previous {existing.type!r}, new {type_!r}"
            )
        if existing.attrs.is_global != is_global:
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
        if not _types_equal(existing.type, ftype):
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
                self._check_function_decl(fd, file_scope=False)
                return
        raise TypeError(f"unexpected declaration: {decl!r}")

    def _check_block_var(
        self, vd: c99_ast.Type_var_decl,
    ) -> None:
        if not _is_object_type(vd.data_type):
            raise TypeCheckError(
                f"object {vd.name!r} declared with non-object type "
                f"{vd.data_type!r}"
            )
        if isinstance(vd.storage_class, c99_ast.Extern):
            if vd.init is not None:
                raise TypeCheckError(
                    f"block-scope `extern` declaration of {vd.name!r} "
                    f"may not have an initializer"
                )
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
                type_=vd.data_type,
                initial=NoInitializer(),
                is_global=is_global,
            )
            return
        if isinstance(vd.storage_class, c99_ast.Static):
            if vd.init is None:
                initial: InitialValue = Initial(0)
            else:
                # Same flow as the file-scope-static path: type-
                # check, apply the conversion rule (so a literal of
                # the wrong variant gets wrapped in an implicit
                # Cast), then drill through Casts to the underlying
                # integer value.
                self._check_exp(vd.init)
                vd.init = _convert_to(vd.init, vd.data_type)
                initial = Initial(_const_init_value(vd.init, vd.name))
            self.symbols[vd.name] = Symbol(
                type=vd.data_type,
                attrs=StaticAttr(initial_value=initial, is_global=False),
            )
            return
        # Plain `int x;` / `long x;` — automatic storage. The
        # initializer is a runtime expression; type-check it and
        # convert to the declared type so the AST carries an
        # explicit Cast for any narrowing/widening (same shape as
        # Assignment / Return / arg conversion).
        self.symbols[vd.name] = Symbol(
            type=vd.data_type, attrs=LocalAttr(),
        )
        if vd.init is not None:
            self._check_exp(vd.init)
            vd.init = _convert_to(vd.init, vd.data_type)

    # ------------------------------------------------------------------
    # Statements / expressions
    # ------------------------------------------------------------------

    def _check_statement(
        self, stmt: c99_ast.Type_statement,
    ) -> None:
        match stmt:
            case c99_ast.Return(exp=exp):
                self._check_exp(exp)
                expected = self._return_type
                # `expected is None` only happens if `Return` shows
                # up outside any function body, which the parser
                # doesn't allow; defensive check just in case.
                if expected is None:
                    raise TypeCheckError(
                        "return statement outside of any function"
                    )
                # Return-value conversion (C99 §6.8.6.4.3): if the
                # value's type doesn't match the declared return
                # type, wrap it in an implicit Cast — same shape as
                # Assignment / FunctionCall arg conversion.
                stmt.exp = _convert_to(exp, expected)
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
                return
        raise TypeError(f"unexpected statement: {stmt!r}")

    def _check_for_init(
        self, init: c99_ast.Type_for_init,
    ) -> None:
        match init:
            case c99_ast.InitDecl(var_decl=vd):
                # The for-init-decl rule (resolver) forbids storage-
                # class specifiers, so this is always plain
                # `T <name> = <exp>;` and lands as a LocalAttr.
                if not _is_object_type(vd.data_type):
                    raise TypeCheckError(
                        f"for-init {vd.name!r} declared with non-"
                        f"object type {vd.data_type!r}"
                    )
                self.symbols[vd.name] = Symbol(
                    type=vd.data_type, attrs=LocalAttr(),
                )
                if vd.init is not None:
                    # Initializer-conversion rule: type-check then
                    # wrap in an implicit Cast if needed (same
                    # shape as block-scope var decls).
                    self._check_exp(vd.init)
                    vd.init = _convert_to(vd.init, vd.data_type)
                return
            case c99_ast.InitExp(exp=exp):
                if exp is not None:
                    self._check_exp(exp)
                return
        raise TypeError(f"unexpected for_init: {init!r}")

    def _pointer_equality_common_type(
        self,
        lhs: c99_ast.Type_exp, rhs: c99_ast.Type_exp,
        tl: Type, tr: Type,
    ) -> Type:
        """Pick the common type for a `==` / `!=` whose operands
        include at least one pointer. Three legal shapes per C99
        §6.5.9 (and the c6502-specific simplification noted in the
        commit message):
          pointer == same pointer            → that pointer type
          pointer == null pointer constant   → the pointer type
                                              (the 0 is converted)
          null pointer constant == pointer   → mirror of above
        Anything else (mismatched pointer types, pointer + non-zero
        integer, pointer + FP) raises. Caller has already
        established that at least one of `tl` / `tr` is Pointer."""
        l_ptr = _is_pointer_type(tl)
        r_ptr = _is_pointer_type(tr)
        if l_ptr and r_ptr:
            if not _types_equal(tl, tr):
                raise TypeCheckError(
                    f"comparison of distinct pointer types: "
                    f"{tl!r} vs {tr!r}"
                )
            # Fresh instance so callers can attach to AST nodes
            # without aliasing — same convention as `_common_type`.
            return Pointer(referenced_type=tl.referenced_type)
        # Exactly one operand is a pointer; the other must be a
        # null pointer constant.
        if l_ptr and _is_null_pointer_constant(rhs):
            return Pointer(referenced_type=tl.referenced_type)
        if r_ptr and _is_null_pointer_constant(lhs):
            return Pointer(referenced_type=tr.referenced_type)
        raise TypeCheckError(
            f"comparison between pointer and non-null-constant "
            f"non-pointer: {tl!r} vs {tr!r}"
        )

    def _check_exp(self, exp: c99_ast.Type_exp) -> Type:
        """Type-check an expression, populating `exp.data_type` in
        place on every node visited. Returns the computed type for
        caller convenience (callers like `_check_block_var` need to
        compare it against a declared type).

        For `Binary` and `Conditional`, if operand types disagree,
        the narrower operand is wrapped in an implicit `Cast` —
        TAC sees a self-describing tree where every operand has a
        concrete `data_type` and any size-changing conversion is an
        explicit Cast node. The mutation happens on the parent
        `Binary` / `Conditional` node's child fields (`left` /
        `right` / `true_clause` / `false_clause`).
        """
        match exp:
            case c99_ast.Constant(const=c):
                # Each const variant carries its own implied type;
                # the parser's C99 §6.4.4.1 / §6.4.4.2 dispatch
                # already chose the variant based on suffix, base,
                # and value, so no re-checking here.
                if isinstance(c, c99_ast.ConstInt):
                    t = Int()
                elif isinstance(c, c99_ast.ConstLong):
                    t = Long()
                elif isinstance(c, c99_ast.ConstUInt):
                    t = UInt()
                elif isinstance(c, c99_ast.ConstULong):
                    t = ULong()
                elif isinstance(c, c99_ast.ConstFloat):
                    t = Float()
                elif isinstance(c, c99_ast.ConstDouble):
                    t = Double()
                else:
                    raise TypeError(f"unexpected const: {c!r}")
                exp.data_type = t
                return t
            case c99_ast.Cast(target_type=target, exp=inner):
                if not _is_object_type(target):
                    raise TypeCheckError(
                        f"cast target type must be an object type "
                        f"(Int / Long / UInt / ULong / Float / "
                        f"Double), got {target!r}"
                    )
                inner_type = self._check_exp(inner)
                if not _is_object_type(inner_type):
                    raise TypeCheckError(
                        f"cannot cast non-object type {inner_type!r} "
                        f"to {target!r}"
                    )
                exp.data_type = target
                return target
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
                exp.data_type = sym.type
                return sym.type
            case c99_ast.Unary(op=op, exp=inner):
                t = self._check_exp(inner)
                if not _is_object_type(t):
                    raise TypeCheckError(
                        f"unary operator on non-object type {t!r}"
                    )
                # `!x` always yields an int (C99 §6.5.3.3.5: "The
                # result has type int"). `-x` and `~x` preserve type.
                if isinstance(op, c99_ast.LogicalNot):
                    result = Int()
                else:
                    result = t
                exp.data_type = result
                return result
            case c99_ast.Binary(op=op, left=lhs, right=rhs):
                tl = self._check_exp(lhs)
                tr = self._check_exp(rhs)
                if not _is_object_type(tl) or not _is_object_type(tr):
                    raise TypeCheckError(
                        f"binary operator on non-object types: "
                        f"{tl!r}, {tr!r}"
                    )
                # Pointer equality (C99 §6.5.9.2) takes its own path
                # — `_common_type` would crash on Pointer (it calls
                # `type(a)()` for matching types, which fails for
                # Pointer's required referenced_type field), and the
                # legality rules differ from arithmetic: matching
                # pointer type is OK, pointer + null pointer
                # constant is OK, anything else is rejected. Other
                # binary ops on pointers (arithmetic, ordering)
                # aren't yet supported and fall through to the
                # arithmetic path below, which raises.
                if (
                    isinstance(op, (c99_ast.Equal, c99_ast.NotEqual))
                    and (_is_pointer_type(tl) or _is_pointer_type(tr))
                ):
                    common = self._pointer_equality_common_type(
                        lhs, rhs, tl, tr,
                    )
                    exp.left = _convert_to(lhs, common)
                    exp.right = _convert_to(rhs, common)
                    exp.data_type = Int()
                    return Int()
                # Usual arithmetic conversions (C99 §6.3.1.8): if
                # operand types differ, promote the narrower one to
                # the common type by wrapping it in an implicit
                # Cast. Both operands now have type `common`, so the
                # underlying op is well-defined at one width.
                common = _common_type(tl, tr)
                exp.left = _convert_to(lhs, common)
                exp.right = _convert_to(rhs, common)
                # Result type: arithmetic / bitwise / shift ops yield
                # the common type; comparison and logical-and/or
                # always yield int regardless of operand type
                # (§6.5.3.3.5 / §6.5.8.6 / §6.5.13.3 / §6.5.14.3).
                if isinstance(op, (
                    c99_ast.Equal, c99_ast.NotEqual,
                    c99_ast.LessThan, c99_ast.GreaterThan,
                    c99_ast.LessOrEqual, c99_ast.GreaterOrEqual,
                    c99_ast.LogicalAnd, c99_ast.LogicalOr,
                )):
                    result = Int()
                else:
                    result = common
                exp.data_type = result
                return result
            case c99_ast.Assignment(lval=lv, rval=rv):
                tl = self._check_exp(lv)
                tr = self._check_exp(rv)
                # Assignment conversion (C99 §6.5.16.1): the value of
                # the right operand is converted to the type of the
                # assignment expression. Implemented by wrapping the
                # rval in an implicit Cast when its type doesn't
                # already match the lval's. This covers compound
                # assignments too — `int_x += long_y` parses as
                # `int_x = int_x + long_y`, the Binary promotes both
                # operands to Long and yields Long, then this branch
                # narrows the Long result back to Int via an
                # implicit (int) cast — same semantics as the
                # explicit `int_x = (int)((long)int_x + long_y)`.
                # Note we don't re-check `_is_object_type(tl)` here:
                # identifier_resolution already enforces that the
                # lval is a Var, and Var lookups in `_check_exp`
                # raise "function used as variable" if the type
                # isn't an object type, so `tl` is always Int or
                # Long by the time we land here.
                exp.rval = _convert_to(rv, tl)
                exp.data_type = tl
                return tl
            case c99_ast.Postfix(operand=op):
                t = self._check_exp(op)
                if not _is_object_type(t):
                    raise TypeCheckError(
                        f"postfix operator on non-object type {t!r}"
                    )
                exp.data_type = t
                return t
            case c99_ast.Conditional(
                condition=cond,
                true_clause=t_clause,
                false_clause=f_clause,
            ):
                self._check_exp(cond)
                tt = self._check_exp(t_clause)
                tf = self._check_exp(f_clause)
                if not _is_object_type(tt) or not _is_object_type(tf):
                    raise TypeCheckError(
                        f"conditional branches must be object types, "
                        f"got {tt!r}, {tf!r}"
                    )
                # C99 §6.5.15.5: usual arithmetic conversions on the
                # two branches; result has the common type.
                common = _common_type(tt, tf)
                exp.true_clause = _convert_to(t_clause, common)
                exp.false_clause = _convert_to(f_clause, common)
                exp.data_type = common
                return common
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
                # Argument conversion (C99 §6.5.2.2.7): each argument
                # is converted, as if by assignment, to the type of
                # the corresponding parameter. Mutate `args` in place
                # so the post-conversion arg list is what the back end
                # sees — same shape as Assignment / Binary / Conditional
                # promotion.
                for i, (arg, expected) in enumerate(
                    zip(args, sym.type.params),
                ):
                    self._check_exp(arg)
                    args[i] = _convert_to(arg, expected)
                exp.data_type = sym.type.ret
                return sym.type.ret
            case c99_ast.Dereference(exp=inner):
                # `*e` — operand must have pointer type, result is
                # the pointee (C99 §6.5.3.2.4). The lvalue-ness of
                # the result is structural (Assignment / AddressOf /
                # Postfix accept Dereference); not encoded in the type.
                t_inner = self._check_exp(inner)
                if not _is_pointer_type(t_inner):
                    raise TypeCheckError(
                        f"unary '*' requires a pointer operand, got "
                        f"{type(t_inner).__name__}"
                    )
                pointee = t_inner.referenced_type
                exp.data_type = pointee
                return pointee
            case c99_ast.AddressOf(exp=inner):
                # `&e` — result is `Pointer(operand_type)`
                # (C99 §6.5.3.2.3). Lvalue check on `e` lives in
                # identifier_resolution. The operand is allowed to
                # have any object type (the type checker has already
                # rejected function-typed Vars in the inner
                # `_check_exp` call via the "function used as
                # variable" check).
                t_inner = self._check_exp(inner)
                result = Pointer(referenced_type=t_inner)
                exp.data_type = result
                return result
        raise TypeError(f"unexpected exp: {exp!r}")


def check_program(
    prog: c99_ast.Type_program,
) -> tuple[c99_ast.Type_program, SymbolTable]:
    """Type-check a c99 program. Returns the (unchanged) AST plus
    the populated SymbolTable. Raises `TypeCheckError` on any type
    error encountered."""
    return TypeChecker().check_program(prog)
