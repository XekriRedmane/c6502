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
from passes.constant_expression import (
    ConstantExpressionError,
    evaluate_integer_constant_expression,
)
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
LongLong = c99_ast.LongLong
UInt = c99_ast.UInt
ULong = c99_ast.ULong
ULongLong = c99_ast.ULongLong
Char = c99_ast.Char
SChar = c99_ast.SChar
UChar = c99_ast.UChar
Float = c99_ast.Float
Double = c99_ast.Double
Void = c99_ast.Void
FunType = c99_ast.FunType
Pointer = c99_ast.Pointer
Array = c99_ast.Array


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
class AddressInit:
    """Static pointer initializer of the form `&name+offset` —
    the address of another static-storage object (or a function).
    The address is link-time known, so codegen lays the bytes
    down via the assembler's symbol-resolution (`DC.W name+off`)
    rather than as a literal numeric value. C99 §6.6.7 paragraph 9
    permits this shape inside a constant expression."""
    name: str
    offset: int = 0


@dataclass(frozen=True)
class Initial(InitialValue):
    """Object declared with an initializer. The value carries the
    constant lifted out of the initializer expression — an `int`
    for the six integer types, a `float` (Python double-precision)
    for `Float` / `Double`, or an `AddressInit` for a Pointer-typed
    static initialized with `&otherstatic`. The variable's declared
    type tells codegen which `StaticVariable` width to emit; the
    same numeric value can mean different things in different
    widths, and Float vs. Double also pick different IEEE 754 byte
    sequences.

    For array-typed statics, `value` is a `tuple` whose length
    matches the declared array size, with one element per array
    slot — int / float / AddressInit for scalar element types,
    or a nested tuple for nested arrays. Missing trailing entries
    in the source initializer have already been padded with
    typed-zero by the type-check (so codegen sees a complete
    tuple of size `arr.size`)."""
    value: int | float | AddressInit | tuple


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
    symbols: SymbolTable | None = None,
) -> int | float | AddressInit:
    """Static-storage initializers must be compile-time constant
    expressions (C99 §6.7.8.4). After `_check_exp` and the
    initializer-conversion rule have run, the AST shape is one of:
      * a `Constant(...)` — drills to its int / float value.
      * a `Cast` (possibly nested) wrapping any of these — produced
        by `_convert_to` for narrowing/widening initializers, or
        explicitly written by the user.
      * an `AddressOf(Var(name))` — taking the address of another
        static-storage object (or a function). C99 §6.6.7 paragraph
        9 makes this a valid constant expression; we capture it as
        an `AddressInit` and let codegen emit `DC.W name` so the
        assembler resolves the symbol at link time.

    Integer- and float-shaped values pass through to `int` / `float`
    Python values; the address-of shape returns an `AddressInit`.
    The Cast target's type tells codegen the storage width when
    laying out the StaticVariable; the raw value passes through,
    and the declared-type-driven conversion (e.g. int constant
    initializer for a Float static) happens in c99_to_tac when it
    builds the typed `*Init` node.

    `symbols` is consulted when an AddressOf shape is detected, to
    confirm the operand is a static-storage object or a function
    (taking the address of a local would be a runtime expression,
    not a constant expression).
    """
    match exp:
        case c99_ast.Constant(const=c):
            if isinstance(c, (c99_ast.ConstFloat, c99_ast.ConstDouble)):
                return c.float
            return c.int
        case c99_ast.Cast(exp=inner):
            return _const_init_value(inner, name, symbols)
        case c99_ast.AddressOf(exp=inner):
            # Only `&name` (a bare Var operand) is a constant
            # expression — `&*p` and other forms aren't (they
            # depend on runtime values).
            if not isinstance(inner, c99_ast.Var):
                raise TypeCheckError(
                    f"initializer for static-storage object {name!r} "
                    f"takes the address of a non-Var expression "
                    f"{inner!r}"
                )
            target = inner.name
            if symbols is not None:
                sym = symbols.get(target)
                if sym is None:
                    raise TypeCheckError(
                        f"initializer for static-storage object "
                        f"{name!r} references undeclared identifier "
                        f"{target!r}"
                    )
                if not isinstance(sym.attrs, (StaticAttr, FunAttr)):
                    raise TypeCheckError(
                        f"initializer for static-storage object "
                        f"{name!r} takes the address of {target!r}, "
                        f"which doesn't have static storage duration"
                    )
            return AddressInit(name=target, offset=0)
    raise TypeCheckError(
        f"initializer for static-storage object {name!r} is not a "
        f"constant expression"
    )


def _zero_aggregate(t: "Type"):
    """Build a default-zero value tree for static-storage object of
    type `t`. Scalars use the matching Python zero (`0` for integer
    / pointer, `0.0` for FP); arrays produce a tuple of size `N`,
    each element zeroed recursively. Used both for tentative
    definitions (no init) and for trailing entries missing from a
    partial array initializer."""
    if isinstance(t, Array):
        return tuple(_zero_aggregate(t.element_type) for _ in range(t.size))
    if isinstance(t, (Float, Double)):
        return 0.0
    return 0


def _is_char_element(t: "Type") -> bool:
    """True iff `t` is one of the three char element types (Char /
    SChar / UChar). Used to decide whether a string-literal
    initializer is admissible for an array's element type per
    C99 §6.7.8.14."""
    return isinstance(t, (Char, SChar, UChar))


def _string_to_value_tuple(s: str, n: int) -> tuple:
    """Convert a string-literal body (one Python code point per
    byte, 0..255) into the value tuple of length `n` that the
    static-storage initializer machinery expects: ints for the
    string's bytes, then a null terminator if there's room, then
    typed-zero pad to `n`. Caller has already ensured `len(s) <=
    n`. When `len(s) == n` the null terminator is elided per
    §6.7.8.14 footnote 138 ("If the array is of size N+1 ... the
    array elements following the null character are unspecified."
    — when there's no extra room, the null is omitted)."""
    out = [ord(c) & 0xFF for c in s]
    while len(out) < n:
        out.append(0)
    return tuple(out)


def _const_init_aggregate(
    init: c99_ast.Type_exp,
    var_type: "Type",
    name: str,
    symbols: "SymbolTable | None" = None,
):
    """Build a value tree (scalar or nested tuple) for an array
    static initializer. Scalar leaves run through `_const_init_value`
    so all the existing constant-expression rules apply (Cast
    drilling, AddressOf static-storage); array nodes recurse into
    their `InitList.items`, padding any missing trailing entries
    with the element type's typed-zero per C99 §6.7.8.21."""
    if isinstance(var_type, Array):
        # String literal initializing a char-array (at any nesting
        # depth) — `_string_to_value_tuple` lays the bytes down
        # with zero-pad to the array's size.
        if (
            isinstance(init, c99_ast.String)
            and _is_char_element(var_type.element_type)
        ):
            return _string_to_value_tuple(init.str, var_type.size)
        if not isinstance(init, c99_ast.InitList):
            raise TypeCheckError(
                f"static array {name!r} requires a brace-enclosed "
                f"initializer (`{{...}}`)"
            )
        elem_type = var_type.element_type
        items: list = []
        for i in range(var_type.size):
            if i < len(init.items):
                items.append(_const_init_aggregate(
                    init.items[i], elem_type, name, symbols,
                ))
            else:
                items.append(_zero_aggregate(elem_type))
        return tuple(items)
    return _const_init_value(init, name, symbols)


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
    """True iff `t` is a scalar value type that can appear as an
    operand without further decay — per C99 §6.2.5, the arithmetic
    types and pointers. Arrays are object types per the standard
    too, but at every operand site they decay to a pointer first
    (`_decay_if_array` runs before this check), so we exclude Array
    here to keep the post-decay invariant catchable: any leftover
    Array operand is a missed decay site, not legal C. FunType is
    also excluded — a function isn't an object."""
    return isinstance(
        t, (Int, Long, LongLong, UInt, ULong, ULongLong,
            Char, SChar, UChar,
            Float, Double, Pointer),
    )


def _is_complete_object_type(t: Type) -> bool:
    """Like `_is_object_type` but includes Array. Used at variable-
    declaration sites where Array IS a legal type for the named
    object (the array decays only when its name appears in an
    expression, not when it's being declared)."""
    return _is_object_type(t) or isinstance(t, Array)


def _is_integer_type(t: Type) -> bool:
    return isinstance(
        t, (Int, Long, LongLong, UInt, ULong, ULongLong,
            Char, SChar, UChar),
    )


def _is_floating_type(t: Type) -> bool:
    return isinstance(t, (Float, Double))


def _is_pointer_type(t: Type) -> bool:
    return isinstance(t, Pointer)


def _is_array_type(t: Type) -> bool:
    return isinstance(t, Array)


def _is_void(t: Type) -> bool:
    """True iff `t` is the `void` type itself (NOT `void *`)."""
    return isinstance(t, Void)


def _is_void_pointer(t: Type) -> bool:
    """True iff `t` is `void *`."""
    return isinstance(t, Pointer) and isinstance(t.referenced_type, Void)


def _sizeof(t: Type) -> int:
    """Bytes occupied by a value of type `t` in c6502's storage
    model. Recursive for Array — `int[3][4]` is 12 bytes,
    `char[10]` is 10 bytes. Mirrors the helper of the same name in
    `c99_to_tac` (kept local so type_checking doesn't import the
    backend); the two stay in sync because they both encode the same
    storage-model rules. Used by the `sizeof` operator's
    type-checker. Constraint violations (Void, FunType) raise
    TypeCheckError — `sizeof` of an incomplete or function type is
    illegal per C99 §6.5.3.4.1, and any caller that lands here with
    one of those types has a bug."""
    if isinstance(t, (Int, UInt, Char, SChar, UChar)):
        return 1
    if isinstance(t, (Long, ULong, Pointer)):
        return 2
    if isinstance(t, (LongLong, ULongLong, Float)):
        return 4
    if isinstance(t, Double):
        return 8
    if isinstance(t, Array):
        return _sizeof(t.element_type) * t.size
    raise TypeCheckError(
        f"cannot take sizeof an incomplete or function type: {t!r}"
    )


def _check_well_formed_type(t: Type, *, where: str) -> None:
    """Walk `t` and raise TypeCheckError if any nested Array has an
    incomplete element type (Void or another array of incomplete
    element type), per C99 §6.7.5.2.1: "The element type shall not
    be an incomplete or function type."

    Used at every site where a declared or named type is materialized:
    var declarations, function parameter types, cast targets. The walk
    descends through Pointer (so `void (*)[3]` is rejected by virtue
    of the inner `void[3]`) and through Array (multi-dim). FunType is
    a leaf — its params / ret are validated independently when the
    function itself is checked, not recursively here.

    `where` is a short label used in the error message so the user
    knows whether the rejected shape is, say, a parameter or a cast
    target."""
    if isinstance(t, Array):
        if isinstance(t.element_type, Void):
            raise TypeCheckError(
                f"{where}: array element type cannot be void (C99 "
                f"§6.7.5.2.1 — array element must be a complete "
                f"object type)"
            )
        if isinstance(t.element_type, FunType):
            raise TypeCheckError(
                f"{where}: array element type cannot be a function "
                f"type (C99 §6.7.5.2.1)"
            )
        _check_well_formed_type(t.element_type, where=where)
        return
    if isinstance(t, Pointer):
        _check_well_formed_type(t.referenced_type, where=where)
        return
    # Int / Long / ... / Char / Float / Double / Void / FunType — all
    # leaves; no nested constraint to check at this level. (FunType's
    # params and ret get their own well-formedness checks at function-
    # decl time via `_check_function_decl`.)


def _decay_if_array(exp: c99_ast.Type_exp) -> c99_ast.Type_exp:
    """Implement C99 §6.3.2.1.3 array-to-pointer decay. If `exp.data_type`
    is `Array(elem, N)`, wrap `exp` in an implicit `AddressOf` stamped
    with `Pointer(elem)` and return the wrapper. Otherwise return `exp`
    unchanged. The wrapper type is narrower than the strict C99
    "pointer to array of N" — it's `Pointer(elem)`, the type the
    pipeline cares about (what `*(arr + i)` operates on). Each call
    site that consumes an expression — Binary operands, Conditional
    branches, Cast inner, Assignment rval, FunctionCall args, Return
    value, var initializer, Subscript array operand — is responsible
    for decaying its inputs before further type-checking; the
    `_is_object_type` predicate excludes Array, so any missed decay
    site catches as a non-object-type error rather than silently
    producing nonsense."""
    if isinstance(exp.data_type, Array):
        elem = exp.data_type.element_type
        wrapped = c99_ast.AddressOf(
            exp=exp, data_type=Pointer(referenced_type=elem),
        )
        return wrapped
    return exp


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
        (c99_ast.ConstInt, c99_ast.ConstLong, c99_ast.ConstLongLong,
         c99_ast.ConstUInt, c99_ast.ConstULong, c99_ast.ConstULongLong,
         c99_ast.ConstChar, c99_ast.ConstUChar),
    ):
        return c.int == 0
    return False


def _is_arithmetic_type(t: Type) -> bool:
    """True iff `t` is integer or floating — the types that
    participate in C99 §6.3.1.8 usual arithmetic conversions.
    Excludes Pointer, even though Pointer is an object type."""
    return _is_integer_type(t) or _is_floating_type(t)


# Width and signedness predicates for the integer types. Width is
# the byte-size in c6502's storage model (1 / 2 / 4); rank, used
# for the C99 §6.3.1.1 promotion / common-type rules, places
# Char/SChar/UChar BELOW Int by C99 §6.3.1.1.1 paragraph 3 — even
# though they share a width with Int/UInt, char-typed operands
# always integer-promote to int (or unsigned int) before
# arithmetic. So char rank 0; Int/UInt rank 1; Long/ULong rank 2;
# LongLong/ULongLong rank 3.
def _int_width(t: Type) -> int:
    if isinstance(t, (Int, UInt, Char, SChar, UChar)):
        return 1
    if isinstance(t, (Long, ULong)):
        return 2
    if isinstance(t, (LongLong, ULongLong)):
        return 4
    raise TypeError(f"_int_width: not an integer object type: {t!r}")


def _is_signed(t: Type) -> bool:
    # Plain `char` is signed in c6502 (-128..127), matching `signed
    # char`. Per C99 §6.2.5.15 the choice is implementation-defined.
    return isinstance(t, (Int, Long, LongLong, Char, SChar))


def _promote_integer(t: Type) -> Type:
    """C99 §6.3.1.1.2 integer promotion. Char/SChar/UChar promote
    to int (or unsigned int when int can't represent every value
    of the source type). For c6502:
      * SChar / Char (-128..127)  → Int (-128..127): exact range
        match, promotes to Int.
      * UChar (0..255)            → Int (-128..127) can't cover
        the full UChar range, so promotes to UInt (0..255).
    Every other integer type already has rank ≥ Int, so promotion
    is a no-op for them. Floating types pass through unchanged.

    The promotion is conventionally applied at every operand
    position of an arithmetic / bitwise / comparison / shift
    operator, plus the operand of unary `+`, `-`, `~`. The result
    of `_convert_to(exp, _promote_integer(exp.data_type))` either
    returns `exp` unchanged (already promoted) or wraps it in a
    same-width Cast — the Cast lowering elides same-width casts
    in c99_to_tac, so the runtime cost is zero."""
    if isinstance(t, (Char, SChar)):
        return Int()
    if isinstance(t, UChar):
        return UInt()
    return t


def _coerce_int_to_type(value: int, t: Type) -> int:
    """Reduce `value` mod 2**(8*width) and re-interpret with `t`'s
    signedness. Same byte-level rule as the runtime cast lowering in
    tac_to_asm — Truncate / SignExtend / ZeroExtend all collapse to
    width-modular arithmetic for compile-time-known values. Used by
    the switch type-checker to canonicalize case values to the
    promoted control type."""
    width_bits = 8 * _int_width(t)
    mask = (1 << width_bits) - 1
    raw = value & mask
    if _is_signed(t) and raw & (1 << (width_bits - 1)):
        raw -= 1 << width_bits
    return raw


def _const_for_value(value: int, t: Type) -> c99_ast.Type_const:
    """Build a `Type_const` integer variant matching `t`. Inputs come
    from `_coerce_int_to_type`; for unsigned types the value is non-
    negative, for signed types it may be negative. The const variants
    store non-negative bit patterns for downstream `_byte_at` shift-
    and-mask consumers, so signed negatives wrap to their two's-
    complement bit pattern at the matching width."""
    if isinstance(t, (Int, UInt)):
        bits = value & 0xFF
        return c99_ast.ConstInt(int=bits) if isinstance(t, Int) else c99_ast.ConstUInt(int=bits)
    if isinstance(t, (Char, SChar, UChar)):
        # ConstChar / ConstUChar exist in the AST but the parser
        # routes char literals through ConstInt per the user's
        # choice. Reaching this branch means a char-typed switch
        # case got coerced to its declared char width — return a
        # typed ConstChar / ConstUChar so the AST stays self-
        # describing if anything inspects it.
        bits = value & 0xFF
        if isinstance(t, UChar):
            return c99_ast.ConstUChar(int=bits)
        return c99_ast.ConstChar(int=bits)
    if isinstance(t, (Long, ULong)):
        bits = value & 0xFFFF
        return c99_ast.ConstLong(int=bits) if isinstance(t, Long) else c99_ast.ConstULong(int=bits)
    if isinstance(t, (LongLong, ULongLong)):
        bits = value & 0xFFFFFFFF
        return (
            c99_ast.ConstLongLong(int=bits) if isinstance(t, LongLong)
            else c99_ast.ConstULongLong(int=bits)
        )
    raise TypeError(f"_const_for_value: not an integer type: {t!r}")


def _common_type(a: Type, b: Type) -> Type:
    """Usual arithmetic conversions per C99 §6.3.1.8 paragraph 1.

    Floating types dominate per §6.3.1.8.1:
      * either operand `Double` → result `Double`
      * else either operand `Float` → result `Float`
      * else both operands integer → integer rules (below)

    Integer rules, with ranks 1 (Int/UInt), 2 (Long/ULong), and 3
    (LongLong/ULongLong):
      * matching types               → that type
      * both signed (or both unsigned) → the higher-rank type wins
      * mixed; unsigned has rank ≥ signed → unsigned wins
      * mixed; signed has higher rank and can represent all of the
        unsigned type's range                → signed wins
        (Long covers UInt's 0..255; LongLong covers UInt's 0..255 and
        ULong's 0..65535)
      * otherwise → unsigned counterpart of the signed type
        (only applies when widths are equal but signedness differs at
        rank 3 — handled by the rank-≥ rule above; kept for forward
        compatibility.)

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
    # Signed has the higher rank. C99 §6.3.1.8 asks whether it can
    # represent every value of the unsigned type. With c6502's three
    # widths and unsigned rank strictly less than signed, the signed
    # type's range always covers the unsigned type's range:
    #   Long (-32768..32767)         covers UInt (0..255).
    #   LongLong (-2^31..2^31-1)     covers UInt (0..255) and
    #                                  ULong (0..65535).
    return type(signed)()


def _convert_to(exp: c99_ast.Type_exp, target: Type) -> c99_ast.Type_exp:
    """If `exp.data_type` already equals `target`, return `exp` as-is.
    Otherwise wrap it in an implicit `Cast(target, exp)` and tag the
    Cast with `target` as its data_type. The wrapper is what TAC /
    codegen will see, so every operand reaching the back end has a
    self-describing type and any size-changing conversion is an
    explicit Cast node.

    For implicit conversions to pointer C99 §6.5.16.1.1 allows:
      * a compatible pointer type (matching pointee);
      * a `void *` source converted to any object pointer (and the
        mirror — any object pointer converted to `void *`), per
        §6.3.2.3.1: "A pointer to void may be converted to or from
        a pointer to any object type [...] without an explicit cast.";
      * a null pointer constant — an integer constant expression with
        value 0 (§6.3.2.3.3).
    Anything else (non-null integer, mismatched non-void pointer type,
    FP, ...) is rejected here so it doesn't silently lower to nonsense
    bytes. The mirror rule applies for arithmetic targets: a pointer
    source is rejected (only an explicit cast can perform pointer→
    integer or pointer→FP conversion). Explicit `(T)x` casts go
    through the Cast type-check handler and aren't gated by this
    function."""
    if exp.data_type is not None and _types_equal(exp.data_type, target):
        return exp
    # A void source (e.g. the result of a void-returning function call
    # or a `(void)e` cast) has no value — it can't be implicitly
    # converted to anything except its own type. The target=Void case
    # is handled by an explicit `(void)e` cast (which goes through the
    # Cast type-check, not `_convert_to`), so reaching here with target
    # void either means a misuse or a missed call site; flag it.
    if exp.data_type is not None and _is_void(exp.data_type):
        raise TypeCheckError(
            f"void expression cannot be converted to {target!r}; void "
            f"has no value"
        )
    if _is_void(target):
        raise TypeCheckError(
            f"cannot implicitly convert {exp.data_type!r} to void; "
            f"use an explicit `(void)expr` cast to discard a value"
        )
    if isinstance(target, Pointer) and exp.data_type is not None:
        src = exp.data_type
        if _is_integer_type(src):
            if not _is_null_pointer_constant(exp):
                raise TypeCheckError(
                    f"cannot implicitly convert non-null integer to "
                    f"pointer type {target!r}; only the null pointer "
                    f"constant (an integer constant expression with "
                    f"value 0) is assignable to a pointer per C99 "
                    f"§6.3.2.3.3"
                )
        elif isinstance(src, Pointer):
            # Both pointers but the equality short-circuit above
            # didn't fire, so the pointee types differ. C99 §6.3.2.3.1
            # gives `void *` a free conversion to and from any object
            # pointer; treat it as a no-op cast (same byte width, same
            # representation).
            if _is_void_pointer(src) or _is_void_pointer(target):
                pass  # fall through to the Cast wrapper below
            else:
                raise TypeCheckError(
                    f"cannot implicitly convert {src!r} to {target!r}; "
                    f"pointer-to-pointer assignment requires matching "
                    f"pointee types per C99 §6.5.16.1.1"
                )
        else:
            # Float / Double / anything else — only integer null
            # pointer constants and matching pointers are assignable.
            raise TypeCheckError(
                f"cannot implicitly convert {src!r} to pointer type "
                f"{target!r}; only an integer null pointer constant or "
                f"a matching pointer type is assignable per C99 "
                f"§6.5.16.1.1"
            )
    if (_is_arithmetic_type(target)
            and exp.data_type is not None
            and isinstance(exp.data_type, Pointer)):
        # Pointer → integer / FP needs an explicit cast (§6.5.16.1.1
        # only allows assigning a pointer to a `_Bool` lval, which
        # c6502 doesn't model).
        raise TypeCheckError(
            f"cannot implicitly convert pointer type {exp.data_type!r} "
            f"to arithmetic type {target!r}; use an explicit cast"
        )
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
        if not _is_complete_object_type(vd.data_type):
            raise TypeCheckError(
                f"file-scope object {vd.name!r} declared with non-"
                f"object type {vd.data_type!r}"
            )
        _check_well_formed_type(
            vd.data_type, where=f"file-scope object {vd.name!r}",
        )
        is_extern = isinstance(vd.storage_class, c99_ast.Extern)
        if vd.init is None:
            initial: InitialValue = (
                NoInitializer() if is_extern else Tentative()
            )
        elif isinstance(vd.data_type, Array):
            # File-scope `int a[3] = {1, 2, 3};` — same shape as the
            # block-scope `static` array path: validate the init list
            # and lift its constant values into a typed-zero-padded
            # value tuple.
            #
            # Char-array initialization with a string literal
            # (`char arr[N] = "abc";`) is the §6.7.8.14 special
            # case — see `_check_string_array_init`.
            if (
                isinstance(vd.init, c99_ast.String)
                and _is_char_element(vd.data_type.element_type)
            ):
                self._check_string_array_init(
                    vd.init, vd.data_type, vd.name,
                )
                initial = Initial(_string_to_value_tuple(
                    vd.init.str, vd.data_type.size,
                ))
            else:
                if not isinstance(vd.init, c99_ast.InitList):
                    raise TypeCheckError(
                        f"file-scope array {vd.name!r} requires a "
                        f"brace-enclosed initializer (`{{...}}`)"
                    )
                self._check_array_init_list(
                    vd.init, vd.data_type, vd.name,
                )
                initial = Initial(_const_init_aggregate(
                    vd.init, vd.data_type, vd.name, self.symbols,
                ))
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
            # Array-to-pointer decay — `static char *p = arr;`
            # (where `arr` is char[]) and the same shape after
            # string lifting (`static char *p = .str@0;`) both
            # need the rval to decay before the pointer-target
            # conversion.
            vd.init = _decay_if_array(vd.init)
            vd.init = _convert_to(vd.init, vd.data_type)
            initial = Initial(
                _const_init_value(vd.init, vd.name, self.symbols),
            )
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
        # Per C99 §6.7.5.3.4, a parameter shall not have void type
        # (the `f(void)` form for empty params is handled by the
        # parser via a dedicated `LPAREN VOID RPAREN` rule, so any
        # Void surviving in `ftype.params` here is a real
        # parameter declaration like `int f(void x)`).
        for p_type in ftype.params:
            if _is_void(p_type):
                raise TypeCheckError(
                    f"parameter of function {fd.name!r} cannot have "
                    f"void type (C99 §6.7.5.3.4)"
                )
            # Reject `void foo[3]` / `void (*foo)[3]` parameters too —
            # the array element type must be complete (§6.7.5.2.1).
            # The parameter-array adjustment (§6.7.5.3.7) only
            # rewrites the OUTERMOST array suffix to a pointer, so
            # `void foo[3]` becomes `void *foo` (which is fine — caught
            # by the separate void-pointer is-allowed check above)
            # but `void (*foo)[3]` keeps the inner Array(Void, 3)
            # intact and needs the recursive well-formed walk.
            _check_well_formed_type(
                p_type, where=f"parameter of function {fd.name!r}",
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
        if not _is_complete_object_type(vd.data_type):
            raise TypeCheckError(
                f"object {vd.name!r} declared with non-object type "
                f"{vd.data_type!r}"
            )
        _check_well_formed_type(
            vd.data_type, where=f"object {vd.name!r}",
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
                # `static int x;` / `static int a[N];` — zero-
                # initialize per C99 §6.7.8.10. For arrays, that
                # means a tuple of typed zeros sized to the array.
                initial: InitialValue = Initial(_zero_aggregate(vd.data_type))
            elif isinstance(vd.data_type, Array):
                # `static int a[3] = {1, 2, 3};` — brace-enclosed
                # initializer required (a bare scalar is illegal).
                # `_check_array_init_list` validates the count and
                # converts each item to the element type;
                # `_const_init_aggregate` then walks the same tree
                # to extract a Python value tuple for the symbol
                # table, padding missing trailing entries.
                #
                # Char-array string-literal init (`static char a[N]
                # = "abc";`) takes its own §6.7.8.14 path.
                if (
                    isinstance(vd.init, c99_ast.String)
                    and _is_char_element(vd.data_type.element_type)
                ):
                    self._check_string_array_init(
                        vd.init, vd.data_type, vd.name,
                    )
                    initial = Initial(_string_to_value_tuple(
                        vd.init.str, vd.data_type.size,
                    ))
                else:
                    if not isinstance(vd.init, c99_ast.InitList):
                        raise TypeCheckError(
                            f"static array {vd.name!r} requires a "
                            f"brace-enclosed initializer (`{{...}}`)"
                        )
                    self._check_array_init_list(
                        vd.init, vd.data_type, vd.name,
                    )
                    initial = Initial(_const_init_aggregate(
                        vd.init, vd.data_type, vd.name, self.symbols,
                    ))
            else:
                # Same flow as the file-scope-static path: type-
                # check, apply the conversion rule (so a literal of
                # the wrong variant gets wrapped in an implicit
                # Cast), then drill through Casts to the underlying
                # integer value.
                self._check_exp(vd.init)
                vd.init = _convert_to(vd.init, vd.data_type)
                initial = Initial(
                    _const_init_value(vd.init, vd.name, self.symbols),
                )
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
            if isinstance(vd.data_type, Array):
                # Arrays must use a brace-enclosed initializer list
                # (`int a[3] = {1, 2, 3};`); a bare scalar is illegal.
                # The char-array + string-literal special case
                # (`char a[N] = "abc";`) takes its own §6.7.8.14
                # path — c99_to_tac then emits per-byte stores.
                if (
                    isinstance(vd.init, c99_ast.String)
                    and _is_char_element(vd.data_type.element_type)
                ):
                    self._check_string_array_init(
                        vd.init, vd.data_type, vd.name,
                    )
                    return
                if not isinstance(vd.init, c99_ast.InitList):
                    raise TypeCheckError(
                        f"array {vd.name!r} requires a brace-enclosed "
                        f"initializer (`{{...}}`)"
                    )
                self._check_array_init_list(
                    vd.init, vd.data_type, vd.name,
                )
                return
            # Scalar var with InitList init — `int x = {1,2,3};` is
            # rejected (C99 also allows `int x = {5};` but that's a
            # corner we don't need yet).
            if isinstance(vd.init, c99_ast.InitList):
                raise TypeCheckError(
                    f"scalar variable {vd.name!r} cannot have a "
                    f"brace-enclosed initializer"
                )
            self._check_exp(vd.init)
            vd.init = _decay_if_array(vd.init)
            vd.init = _convert_to(vd.init, vd.data_type)

    def _check_string_array_init(
        self,
        init: c99_ast.String,
        arr_type: Array,
        name: str,
    ) -> None:
        """Validate `char arr[N] = "abc";` per C99 §6.7.8.14.
        The literal body has length `len(init.str)` (no terminator);
        the standard allows up to `len(s) + 1` bytes (with the null)
        when N >= len+1, and exactly len bytes (no null) when
        N == len. N < len is a constraint violation. After
        validation the type checker stamps the String with its
        Array(Char, len+1) data_type so any inspecting consumer
        sees a self-describing tree."""
        slen = len(init.str)
        if slen > arr_type.size:
            raise TypeCheckError(
                f"string literal initializer for array {name!r} has "
                f"{slen} bytes but the declared array size is "
                f"{arr_type.size} (C99 §6.7.8.14 requires the "
                f"literal length to be ≤ the array size, with the "
                f"null terminator omitted when the array has no "
                f"room for it)"
            )
        init.data_type = Array(element_type=Char(), size=slen + 1)

    def _check_array_init_list(
        self,
        init: c99_ast.InitList,
        arr_type: Array,
        var_name: str,
    ) -> None:
        """Type-check a brace-enclosed initializer for an array.
        Validates the item count (≤ array size; shorter lists are
        zero-padded by `c99_to_tac` at lowering time) and converts
        each item to the element type via `_convert_to` (so a
        narrowing or widening literal gets an implicit Cast). Mutates
        `init.items` in place with the converted forms; stamps
        `init.data_type` with the array type so downstream passes
        recognise the shape.

        For multi-dim arrays (`int a[2][3] = {{1,2,3},{4,5,6}};`),
        each top-level item must itself be an `InitList` matching
        the inner array type — the recursion handles arbitrary
        nesting. C99 also allows the flat form
        (`{1,2,3,4,5,6}`) by §6.7.8.20's "subaggregate" rule, but
        that's a parsing-time pre-grouping pass we don't have, so
        only the fully-nested form is accepted.
        """
        if len(init.items) > arr_type.size:
            raise TypeCheckError(
                f"too many initializers for array {var_name!r}: "
                f"{len(init.items)} given, array has {arr_type.size} "
                f"element{'s' if arr_type.size != 1 else ''}"
            )
        elem_type = arr_type.element_type
        for i, item in enumerate(init.items):
            if isinstance(elem_type, Array):
                # Each item must itself be a nested InitList — or
                # a String when the sub-array's element type is a
                # char type (C99 §6.7.8.14: a char-array can also
                # be initialized by a string literal at any
                # nesting level). The flat form
                # (`{1,2,3,4,5,6}` for `int[2][3]`) isn't
                # supported.
                if (
                    isinstance(item, c99_ast.String)
                    and _is_char_element(elem_type.element_type)
                ):
                    self._check_string_array_init(
                        item, elem_type, f"{var_name}[{i}]",
                    )
                    # Replace the String with a typed-zero-padded
                    # value tuple in init.items so the static-
                    # storage path (`_const_init_aggregate`) can
                    # walk it uniformly.
                    init.items[i] = item
                    continue
                if not isinstance(item, c99_ast.InitList):
                    raise TypeCheckError(
                        f"expected nested initializer (`{{...}}`) "
                        f"at index {i} of {var_name!r}; element "
                        f"type is {elem_type!r}"
                    )
                self._check_array_init_list(item, elem_type, var_name)
            else:
                if isinstance(item, c99_ast.InitList):
                    raise TypeCheckError(
                        f"unexpected nested initializer at index "
                        f"{i} of {var_name!r} (element type is "
                        f"{elem_type!r}, not an array)"
                    )
                self._check_exp(item)
                # Same conversion rule as scalar init / Assignment
                # rval — array-decay first (so e.g. `&other` shapes
                # work), then _convert_to wraps in an implicit Cast
                # on type mismatch.
                item = _decay_if_array(item)
                init.items[i] = _convert_to(item, elem_type)
        init.data_type = arr_type

    # ------------------------------------------------------------------
    # Statements / expressions
    # ------------------------------------------------------------------

    def _check_statement(
        self, stmt: c99_ast.Type_statement,
    ) -> None:
        match stmt:
            case c99_ast.Return(exp=exp):
                expected = self._return_type
                # `expected is None` only happens if `Return` shows
                # up outside any function body, which the parser
                # doesn't allow; defensive check just in case.
                if expected is None:
                    raise TypeCheckError(
                        "return statement outside of any function"
                    )
                # Bare `return;` (no expression). Legal only inside a
                # void-returning function (C99 §6.8.6.4.1).
                if exp is None:
                    if not _is_void(expected):
                        raise TypeCheckError(
                            f"`return;` without a value in a function "
                            f"returning {expected!r}; bare `return;` "
                            f"is only legal in a void-returning "
                            f"function (C99 §6.8.6.4.1)"
                        )
                    return
                # `return e;` with an expression. Illegal in a void-
                # returning function — §6.8.6.4.1: "A return statement
                # with an expression shall not appear in a function
                # whose return type is void."
                if _is_void(expected):
                    raise TypeCheckError(
                        f"`return <expr>;` in a void-returning function; "
                        f"void functions must use bare `return;` (C99 "
                        f"§6.8.6.4.1)"
                    )
                self._check_exp(exp)
                # Return-value conversion (C99 §6.8.6.4.3): if the
                # value's type doesn't match the declared return
                # type, wrap it in an implicit Cast — same shape as
                # Assignment / FunctionCall arg conversion. Array
                # decay applies here too — `int *foo() { return arr;
                # }` is the standard idiom.
                exp = _decay_if_array(exp)
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
            case c99_ast.SwitchStmt():
                self._check_switch(stmt)
                return
            case c99_ast.CaseStmt(body=inner) | c99_ast.DefaultStmt(body=inner):
                # The case / default value's typing is handled by the
                # owning switch's `_check_switch` (which runs through
                # `evaluate_integer_constant_expression`); here we just
                # descend into the inner statement so any nested
                # statements get checked.
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

    def _check_switch(self, stmt: c99_ast.SwitchStmt) -> None:
        # C99 §6.8.4.2.1: "The controlling expression of a switch
        # statement shall have integer type." For c6502 the integer
        # types are Int/UInt/Long/ULong/LongLong/ULongLong; reject
        # Float, Double, and Pointer.
        self._check_exp(stmt.control)
        ctrl_type = stmt.control.data_type
        if ctrl_type is None or not _is_integer_type(ctrl_type):
            raise TypeCheckError(
                f"switch controlling expression must have integer type "
                f"(got {ctrl_type!r}); C99 §6.8.4.2.1"
            )
        # Integer promotion (§6.3.1.1). For c6502's six integer
        # types every type is already at promotion rank ≥ Int, so
        # promotion is a no-op — the promoted type IS the control
        # type. Stash it on the SwitchStmt so c99_to_tac can match
        # case-constant variants to the dispatch type without
        # recomputing.
        stmt.promoted_type = ctrl_type
        # Validate every case value: it must be an integer constant
        # expression (§6.6.6 + §6.8.4.2.3); after conversion to the
        # promoted control type, no two cases shall have the same
        # value (§6.8.4.2.3).
        seen_values: dict[int, str] = {}
        for case in stmt.cases:
            # Type-check the case value first so any nested SizeOfExp
            # inside has its inner expression's data_type stamped — the
            # constant evaluator reads `inner.data_type` to fold
            # `sizeof e`. For a bare integer literal or a Cast around
            # one, this is just a no-op type stamp; for sizeof it's
            # what makes folding possible. We mirror the post-eval
            # canonicalization (case.value → Constant of promoted
            # type) below, so any rewriting _check_exp does to
            # case.value (e.g. wrapping in implicit Cast) gets
            # overwritten anyway.
            self._check_exp(case.value)
            try:
                value, _value_type = (
                    evaluate_integer_constant_expression(case.value)
                )
            except ConstantExpressionError as e:
                raise TypeCheckError(
                    f"case label is not an integer constant expression: "
                    f"{e}"
                ) from e
            # Convert to the promoted type via the same width-modular
            # rule the runtime cast lowering would apply.
            converted = _coerce_int_to_type(value, ctrl_type)
            if converted in seen_values:
                raise TypeCheckError(
                    f"duplicate case value {converted} in switch "
                    f"statement (C99 §6.8.4.2.3)"
                )
            seen_values[converted] = case.label
            # Replace the case's value expression with a single
            # canonicalized integer Constant of the promoted type, so
            # c99_to_tac can dispatch off a uniform shape.
            case.value = c99_ast.Constant(
                const=_const_for_value(converted, ctrl_type),
                data_type=ctrl_type,
            )
        # Now type-check the body — case / default nodes inside it
        # will descend via the case branch above.
        self._check_statement(stmt.body)

    def _check_for_init(
        self, init: c99_ast.Type_for_init,
    ) -> None:
        match init:
            case c99_ast.InitDecl(var_decl=vd):
                # The for-init-decl rule (resolver) forbids storage-
                # class specifiers, so this is always plain
                # `T <name> = <exp>;` and lands as a LocalAttr.
                if not _is_complete_object_type(vd.data_type):
                    raise TypeCheckError(
                        f"for-init {vd.name!r} declared with non-"
                        f"object type {vd.data_type!r}"
                    )
                _check_well_formed_type(
                    vd.data_type, where=f"for-init {vd.name!r}",
                )
                self.symbols[vd.name] = Symbol(
                    type=vd.data_type, attrs=LocalAttr(),
                )
                if vd.init is not None:
                    if isinstance(vd.data_type, Array):
                        if not isinstance(vd.init, c99_ast.InitList):
                            raise TypeCheckError(
                                f"array {vd.name!r} requires a "
                                f"brace-enclosed initializer "
                                f"(`{{...}}`)"
                            )
                        self._check_array_init_list(
                            vd.init, vd.data_type, vd.name,
                        )
                        return
                    if isinstance(vd.init, c99_ast.InitList):
                        raise TypeCheckError(
                            f"scalar variable {vd.name!r} cannot "
                            f"have a brace-enclosed initializer"
                        )
                    # Initializer-conversion rule: type-check then
                    # wrap in an implicit Cast if needed (same
                    # shape as block-scope var decls).
                    self._check_exp(vd.init)
                    vd.init = _decay_if_array(vd.init)
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
        include at least one pointer. Legal shapes per C99 §6.5.9.2:
          pointer == same pointer            → that pointer type
          void * == any object pointer       → void *
                                              (§6.5.9.2 — "one
                                              operand is a pointer
                                              to an object type and
                                              the other is a pointer
                                              to a qualified or
                                              unqualified version of
                                              void")
          pointer == null pointer constant   → the pointer type
                                              (the 0 is converted)
          null pointer constant == pointer   → mirror of above
        Anything else (mismatched non-void pointer types, pointer +
        non-zero integer, pointer + FP) raises. Caller has already
        established that at least one of `tl` / `tr` is Pointer."""
        l_ptr = _is_pointer_type(tl)
        r_ptr = _is_pointer_type(tr)
        if l_ptr and r_ptr:
            # `void *` matches any object pointer here, with the
            # common type being `void *`.
            if _is_void_pointer(tl) or _is_void_pointer(tr):
                return Pointer(referenced_type=Void())
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
                elif isinstance(c, c99_ast.ConstLongLong):
                    t = LongLong()
                elif isinstance(c, c99_ast.ConstUInt):
                    t = UInt()
                elif isinstance(c, c99_ast.ConstULong):
                    t = ULong()
                elif isinstance(c, c99_ast.ConstULongLong):
                    t = ULongLong()
                elif isinstance(c, c99_ast.ConstChar):
                    t = SChar()
                elif isinstance(c, c99_ast.ConstUChar):
                    t = UChar()
                elif isinstance(c, c99_ast.ConstFloat):
                    t = Float()
                elif isinstance(c, c99_ast.ConstDouble):
                    t = Double()
                else:
                    raise TypeError(f"unexpected const: {c!r}")
                exp.data_type = t
                return t
            case c99_ast.String(str=s):
                # A string literal has type `char[N+1]` per C99
                # §6.4.5.6: the array has one element per source byte
                # plus a trailing null terminator, with element type
                # `char`. Most String nodes have already been hoisted
                # to a file-scope static by `passes.string_lifting`
                # before we land here — the surviving Strings are the
                # ones that DIRECTLY initialize a char[] var_decl
                # (`char arr[10] = "abc";`), and the type checker
                # validates them in `_check_init` rather than as
                # operand exps. The Array type we stamp here lets
                # any leftover bare String go through `_decay_if_array`
                # cleanly if a future caller ever forgets the lift.
                t = Array(element_type=Char(), size=len(s) + 1)
                exp.data_type = t
                return t
            case c99_ast.Cast(target_type=target, exp=inner):
                # Cast targets per C99 §6.5.4.2: scalar (object) types
                # OR `void` (the discard form `(void)e`). Array and
                # function types are rejected at the parser, so by
                # here `target` is one of the legal type-name shapes;
                # we additionally accept Void here.
                if not (_is_object_type(target) or _is_void(target)):
                    raise TypeCheckError(
                        f"cast target type must be an object type "
                        f"(Int / Long / LongLong / UInt / ULong / "
                        f"ULongLong / Float / Double / Pointer) or "
                        f"void, got {target!r}"
                    )
                # Reject malformed cast targets like `(void(*)[3])e`
                # — the array element type must be complete even
                # inside a pointer wrapping.
                _check_well_formed_type(target, where="cast target")
                self._check_exp(inner)
                # Decay an array operand before further type-checking
                # — `(int *)arr` is legal and the cast operates on
                # the decayed pointer.
                exp.exp = _decay_if_array(inner)
                inner = exp.exp
                inner_type = inner.data_type
                # `(void)e` accepts any type for `e` (including a
                # void-typed expression like another void function
                # call); the result is just discarded. Skip the
                # operand-shape and pointer/FP-cross checks.
                if _is_void(target):
                    exp.data_type = target
                    return target
                if not _is_object_type(inner_type):
                    raise TypeCheckError(
                        f"cannot cast non-object type {inner_type!r} "
                        f"to {target!r}"
                    )
                # C99 §6.3.2.3 only defines pointer ↔ integer and
                # pointer ↔ pointer conversions. Pointer ↔ floating
                # has no meaning — an address isn't an arithmetic
                # value — so reject those mixes here.
                src_fp = _is_floating_type(inner_type)
                tgt_fp = _is_floating_type(target)
                src_ptr = _is_pointer_type(inner_type)
                tgt_ptr = _is_pointer_type(target)
                if (src_fp and tgt_ptr) or (src_ptr and tgt_fp):
                    raise TypeCheckError(
                        f"cannot cast between pointer and "
                        f"floating-point type: {inner_type!r} → "
                        f"{target!r}"
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
                self._check_exp(inner)
                # Array-to-pointer decay (C99 §6.3.2.1.3): `!arr`,
                # `!"abc"` etc. coerce the array to a pointer first
                # so the resulting Pointer-typed operand satisfies
                # the scalar-operand requirement of `!` (and so
                # `&arr` / `*arr` callers don't reach this branch
                # — they have their own dedicated AST nodes).
                exp.exp = _decay_if_array(exp.exp)
                t = exp.exp.data_type
                if not _is_object_type(t):
                    raise TypeCheckError(
                        f"unary operator on non-object type {t!r}"
                    )
                # Per-operator operand-type constraints:
                #   `-` requires arithmetic (rejects Pointer per
                #     C99 §6.5.3.3.1).
                #   `~` requires integer (rejects Pointer per the
                #     same paragraph, and rejects Float / Double
                #     per §6.5.3.3.4: "The operand of the unary ~
                #     operator shall have integer type.").
                #   `!` requires scalar (integer / floating /
                #     pointer); always yields int per §6.5.3.3.5.
                #     `!p` is legal — it's defined as `p != 0`,
                #     the pointer's null-ness check.
                if (
                    isinstance(op, (c99_ast.Negate, c99_ast.Complement))
                    and _is_pointer_type(t)
                ):
                    op_name = "-" if isinstance(op, c99_ast.Negate) else "~"
                    raise TypeCheckError(
                        f"unary '{op_name}' is not defined on pointer "
                        f"type {t!r}"
                    )
                if (
                    isinstance(op, c99_ast.Complement)
                    and _is_floating_type(t)
                ):
                    raise TypeCheckError(
                        f"unary '~' requires integer operand, got "
                        f"{type(t).__name__}"
                    )
                # Integer promotion (C99 §6.3.1.1.2 + §6.5.3.3.4):
                # the operand of `-` / `~` / `+` is integer-promoted.
                # `!` doesn't promote — its operand is just compared
                # to zero, and the result is always int.
                if isinstance(op, (c99_ast.Negate, c99_ast.Complement)):
                    promoted = _promote_integer(t)
                    if not _types_equal(promoted, t):
                        exp.exp = _convert_to(exp.exp, promoted)
                        t = promoted
                # `!x` always yields an int. `-x` and `~x` preserve
                # type (after promotion).
                if isinstance(op, c99_ast.LogicalNot):
                    result = Int()
                else:
                    result = t
                exp.data_type = result
                return result
            case c99_ast.Binary(op=op, left=lhs, right=rhs):
                self._check_exp(lhs)
                self._check_exp(rhs)
                # Array-to-pointer decay (C99 §6.3.2.1.3): both
                # operands run through `_decay_if_array` before any
                # type-driven dispatch, so a bare array name behaves
                # exactly like a pointer to its first element.
                exp.left = _decay_if_array(lhs)
                exp.right = _decay_if_array(rhs)
                lhs, rhs = exp.left, exp.right
                tl, tr = lhs.data_type, rhs.data_type
                if not _is_object_type(tl) or not _is_object_type(tr):
                    raise TypeCheckError(
                        f"binary operator on non-object types: "
                        f"{tl!r}, {tr!r}"
                    )
                # `&&` / `||` test each operand for non-zero
                # independently (C99 §6.5.13 / §6.5.14) — no usual-
                # arithmetic-conversions promotion. Pointers are
                # legal: a non-null pointer is "true", a null
                # pointer is "false". By short-circuiting before
                # `_common_type`, we sidestep its crash on Pointer
                # operands (it calls `type(a)()` for matching
                # types, which fails for Pointer's required
                # referenced_type field).
                if isinstance(op, (c99_ast.LogicalAnd, c99_ast.LogicalOr)):
                    # Both operands have to be scalar — the
                    # `_is_object_type` check above is sufficient
                    # (we've excluded FunType, the only non-scalar
                    # object type in the c6502 vocabulary).
                    exp.data_type = Int()
                    return Int()
                # Pointer equality (C99 §6.5.9.2) takes its own path
                # — `_common_type` would crash on Pointer for the
                # same reason as above, and the legality rules
                # differ from arithmetic: matching pointer type is
                # OK, pointer + null pointer constant is OK,
                # anything else is rejected.
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
                # Pointer ordering (C99 §6.5.8.2) — stricter than
                # equality: both operands must be pointers to
                # compatible object types. Null pointer constants
                # aren't allowed on the relational ops the way they
                # are on `==` / `!=`, and mixing pointer with integer
                # / floating is a constraint violation. Result is
                # always Int per §6.5.8.6.
                if (
                    isinstance(op, (
                        c99_ast.LessThan, c99_ast.GreaterThan,
                        c99_ast.LessOrEqual, c99_ast.GreaterOrEqual,
                    ))
                    and (_is_pointer_type(tl) or _is_pointer_type(tr))
                ):
                    op_name = {
                        c99_ast.LessThan: "<",
                        c99_ast.GreaterThan: ">",
                        c99_ast.LessOrEqual: "<=",
                        c99_ast.GreaterOrEqual: ">=",
                    }[type(op)]
                    if not (_is_pointer_type(tl) and _is_pointer_type(tr)):
                        raise TypeCheckError(
                            f"binary '{op_name}' between pointer and "
                            f"non-pointer operand: {tl!r} {op_name} {tr!r}"
                        )
                    if not _types_equal(tl, tr):
                        raise TypeCheckError(
                            f"comparison of distinct pointer types: "
                            f"{tl!r} {op_name} {tr!r}"
                        )
                    exp.data_type = Int()
                    return Int()
                # Multiplicative operators (`*`, `/`, `%`) — C99
                # §6.5.5 requires arithmetic operands for `*` and
                # `/` and integer operands for `%`. None of those
                # categories include Pointer, so pointer operands
                # are flat-out rejected here. (Without this the
                # failure cascades into `_common_type`'s `type(a)()`
                # crash on Pointer; better to flag it cleanly.)
                if (
                    isinstance(op, (
                        c99_ast.Multiply, c99_ast.Divide, c99_ast.Modulo,
                    ))
                    and (_is_pointer_type(tl) or _is_pointer_type(tr))
                ):
                    op_name = {
                        c99_ast.Multiply: "*",
                        c99_ast.Divide: "/",
                        c99_ast.Modulo: "%",
                    }[type(op)]
                    raise TypeCheckError(
                        f"binary '{op_name}' is not defined on pointer "
                        f"operands ({tl!r} {op_name} {tr!r})"
                    )
                # Pointer arithmetic (C99 §6.5.6) — additive operators
                # only. Like the equality and multiplicative paths
                # above, this short-circuits before `_common_type`
                # (which can't construct a Pointer without a
                # referenced_type). The four legal forms:
                #   ptr + int / int + ptr  → ptr (offset by N elements)
                #   ptr - int              → ptr (offset by -N elements)
                #   ptr - ptr (same type)  → Long (element count;
                #                            c6502's stand-in for the
                #                            standard's ptrdiff_t)
                # Anything else (ptr + ptr, int - ptr, ptr ± FP,
                # mismatched ptr - ptr, pointer-to-function
                # arithmetic) is a constraint violation.
                if (
                    isinstance(op, (c99_ast.Add, c99_ast.Subtract))
                    and (_is_pointer_type(tl) or _is_pointer_type(tr))
                ):
                    op_name = "+" if isinstance(op, c99_ast.Add) else "-"
                    # Reject pointer-to-function and pointer-to-void:
                    # §6.5.6.2 requires "pointer to a complete object
                    # type" for the additive ops, and sizeof(void) /
                    # sizeof(function) is undefined.
                    for t in (tl, tr):
                        if (
                            isinstance(t, Pointer)
                            and isinstance(t.referenced_type, FunType)
                        ):
                            raise TypeCheckError(
                                f"binary '{op_name}' is not defined on "
                                f"pointer-to-function operands "
                                f"({tl!r} {op_name} {tr!r})"
                            )
                        if isinstance(t, Pointer) and isinstance(
                            t.referenced_type, Void,
                        ):
                            raise TypeCheckError(
                                f"binary '{op_name}' is not defined on "
                                f"void pointer operands "
                                f"({tl!r} {op_name} {tr!r})"
                            )
                    if _is_floating_type(tl) or _is_floating_type(tr):
                        raise TypeCheckError(
                            f"binary '{op_name}' is not defined on a "
                            f"pointer and a floating-point operand "
                            f"({tl!r} {op_name} {tr!r})"
                        )
                    if _is_pointer_type(tl) and _is_pointer_type(tr):
                        # ptr + ptr is illegal; ptr - ptr is legal iff
                        # the pointer types match.
                        if isinstance(op, c99_ast.Add):
                            raise TypeCheckError(
                                f"binary '+' is not defined on two "
                                f"pointer operands ({tl!r} + {tr!r})"
                            )
                        if not _types_equal(tl, tr):
                            raise TypeCheckError(
                                f"subtraction of distinct pointer "
                                f"types: {tl!r} - {tr!r}"
                            )
                        # Operand types stay as-is — both pointers are
                        # 2 bytes at the byte level, so the underlying
                        # subtract is a normal 2-byte op. The result
                        # is a byte-difference that c99_to_tac will
                        # divide by sizeof(pointee) to yield the
                        # element count. Result type is Long
                        # (c6502's ptrdiff_t).
                        exp.data_type = Long()
                        return Long()
                    # Exactly one operand is a pointer; the other must
                    # be integer. ptr + int / int + ptr / ptr - int are
                    # legal; int - ptr is not.
                    int_is_left = _is_integer_type(tl)
                    if int_is_left and isinstance(op, c99_ast.Subtract):
                        raise TypeCheckError(
                            f"binary '-' between integer and pointer "
                            f"is not defined ({tl!r} - {tr!r})"
                        )
                    # Widen the integer operand to Long so the
                    # underlying byte-level add lines up with the
                    # pointer's 2-byte width. c99_to_tac scales this
                    # widened value by sizeof(pointee) before the add.
                    if int_is_left:
                        exp.left = _convert_to(lhs, Long())
                        ptr_type = tr
                    else:
                        exp.right = _convert_to(rhs, Long())
                        ptr_type = tl
                    # Result is the pointer type — fresh instance per
                    # the same convention as `_common_type`.
                    exp.data_type = Pointer(
                        referenced_type=ptr_type.referenced_type,
                    )
                    return exp.data_type
                # Bitwise / shift / modulo (C99 §6.5.5.2 / §6.5.7.2 /
                # §6.5.10.2 / §6.5.11.2 / §6.5.12.2): operands must
                # have integer type. None of the four c6502 integer
                # types are FP, so rejecting Float / Double on either
                # side covers the constraint cleanly. Pointer
                # operands fall through to `_common_type` which is
                # its own crash today; the chapter_14 pointer-bitwise
                # tests rely on that path's crash for rejection.
                if (
                    isinstance(op, (
                        c99_ast.Modulo,
                        c99_ast.BitwiseAnd,
                        c99_ast.BitwiseOr,
                        c99_ast.BitwiseXor,
                        c99_ast.LeftShift,
                        c99_ast.RightShift,
                    ))
                    and (
                        _is_floating_type(tl) or _is_floating_type(tr)
                    )
                ):
                    op_name = {
                        c99_ast.Modulo: "%",
                        c99_ast.BitwiseAnd: "&",
                        c99_ast.BitwiseOr: "|",
                        c99_ast.BitwiseXor: "^",
                        c99_ast.LeftShift: "<<",
                        c99_ast.RightShift: ">>",
                    }[type(op)]
                    raise TypeCheckError(
                        f"binary '{op_name}' requires integer operands "
                        f"({tl!r} {op_name} {tr!r})"
                    )
                # Integer promotion (C99 §6.3.1.1.2): char-typed
                # operands promote to int (or unsigned int) before
                # the usual arithmetic conversions run. For c6502
                # this is a same-width Cast (SChar/Char → Int,
                # UChar → UInt), elided to a no-op by c99_to_tac's
                # cast lowering.
                exp.left = _convert_to(lhs, _promote_integer(tl))
                exp.right = _convert_to(rhs, _promote_integer(tr))
                lhs, rhs = exp.left, exp.right
                tl, tr = lhs.data_type, rhs.data_type
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
                # Arrays aren't assignable as a whole (C99 §6.5.16.1
                # constraint: lval must be a "modifiable lvalue", and
                # an array isn't one). Subscript / Dereference lvals
                # have their element type by the time we land here,
                # so this only catches an outright `arr_name = ...`.
                if isinstance(tl, Array):
                    raise TypeCheckError(
                        f"cannot assign to an array (use subscript): "
                        f"{lv!r}"
                    )
                # rval array-to-pointer decay (`int *p = arr;` is
                # the most common case). After decay, the rval is
                # Pointer-typed and `_convert_to` either no-ops
                # (when target matches) or wraps in a Pointer→Pointer
                # Cast for cross-pointer-type assignments.
                rv = _decay_if_array(rv)
                exp.rval = rv
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
            case c99_ast.Postfix(operand=op) | c99_ast.Prefix(operand=op):
                t = self._check_exp(op)
                if not _is_object_type(t):
                    raise TypeCheckError(
                        f"increment/decrement operator on non-object "
                        f"type {t!r}"
                    )
                # `++p` / `--p` / `p++` / `p--` is defined as `p ± 1`
                # element, which requires a complete pointee — exactly
                # the §6.5.6.2 constraint Binary `+`/`-` enforces. Void
                # has no size, so reject (C99 §6.5.2.4.1 / §6.5.3.1.1
                # require an "object type" operand for ++/--).
                if isinstance(t, Pointer) and isinstance(
                    t.referenced_type, Void,
                ):
                    raise TypeCheckError(
                        f"increment/decrement on void pointer is not "
                        f"defined (sizeof(void) is undefined)"
                    )
                exp.data_type = t
                return t
            case c99_ast.Conditional(
                condition=cond,
                true_clause=t_clause,
                false_clause=f_clause,
            ):
                self._check_exp(cond)
                self._check_exp(t_clause)
                self._check_exp(f_clause)
                # Decay each branch independently — `cond ? arr : ptr`
                # both branches end up as Pointer.
                exp.true_clause = _decay_if_array(t_clause)
                exp.false_clause = _decay_if_array(f_clause)
                t_clause, f_clause = exp.true_clause, exp.false_clause
                tt, tf = t_clause.data_type, f_clause.data_type
                # Per C99 §6.5.15.5: "If both the second and third
                # operands have void type, the result has void type."
                # No conversion is applied — both branches stay as
                # they are; the conditional just sequences the side
                # effects.
                if _is_void(tt) and _is_void(tf):
                    exp.data_type = Void()
                    return Void()
                if _is_void(tt) or _is_void(tf):
                    raise TypeCheckError(
                        f"conditional branches must both be void or "
                        f"both be value-producing; got {tt!r} vs "
                        f"{tf!r}"
                    )
                if not _is_object_type(tt) or not _is_object_type(tf):
                    raise TypeCheckError(
                        f"conditional branches must be object types, "
                        f"got {tt!r}, {tf!r}"
                    )
                # C99 §6.5.15.6: pointer cases first (since
                # `_common_type` only knows about arithmetic types).
                # Both pointers — must be the same type, OR one of them
                # is `void *` (the null-pointer-constant rule from
                # §6.5.15.6 also applies). One pointer plus a null
                # pointer constant — the pointer wins. Anything else
                # falls through to the usual arithmetic conversions.
                tt_ptr = isinstance(tt, Pointer)
                tf_ptr = isinstance(tf, Pointer)
                if tt_ptr or tf_ptr:
                    if tt_ptr and tf_ptr:
                        # `void *` and any object pointer compose to
                        # `void *` (C99 §6.5.15.6).
                        if _is_void_pointer(tt) or _is_void_pointer(tf):
                            common = Pointer(referenced_type=Void())
                        elif not _types_equal(tt, tf):
                            raise TypeCheckError(
                                f"conditional branches have distinct "
                                f"pointer types: {tt!r} vs {tf!r}"
                            )
                        else:
                            common = Pointer(referenced_type=tt.referenced_type)
                    elif tt_ptr and _is_null_pointer_constant(f_clause):
                        common = Pointer(referenced_type=tt.referenced_type)
                    elif tf_ptr and _is_null_pointer_constant(t_clause):
                        common = Pointer(referenced_type=tf.referenced_type)
                    else:
                        raise TypeCheckError(
                            f"conditional branches must both be "
                            f"arithmetic or both pointer (or pointer "
                            f"+ null pointer constant); got {tt!r} "
                            f"vs {tf!r}"
                        )
                else:
                    # Integer promotion (C99 §6.3.1.1.2) before the
                    # usual arithmetic conversions, same as in Binary.
                    exp.true_clause = _convert_to(
                        t_clause, _promote_integer(tt),
                    )
                    exp.false_clause = _convert_to(
                        f_clause, _promote_integer(tf),
                    )
                    t_clause = exp.true_clause
                    f_clause = exp.false_clause
                    tt, tf = t_clause.data_type, f_clause.data_type
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
                # Two callee shapes:
                #   FunType                — direct call (`foo()`).
                #   Pointer(FunType)       — indirect call through a
                #     function pointer (`fp()`). C99 §6.5.2.2 says
                #     the callee must have type pointer-to-function;
                #     §6.3.2.1.4 lets a function name auto-decay to
                #     a pointer, so both shapes converge on the
                #     same `FunType` for arg / return checking.
                # Anything else (Int / Long / etc.) is an error.
                fn_type: FunType
                if isinstance(sym.type, FunType):
                    fn_type = sym.type
                elif (
                    isinstance(sym.type, Pointer)
                    and isinstance(sym.type.referenced_type, FunType)
                ):
                    fn_type = sym.type.referenced_type
                else:
                    raise TypeCheckError(
                        f"variable {name!r} called as a function"
                    )
                if len(args) != len(fn_type.params):
                    plural = "s" if len(args) != 1 else ""
                    raise TypeCheckError(
                        f"function {name!r} called with {len(args)} "
                        f"argument{plural}, expected "
                        f"{len(fn_type.params)}"
                    )
                # Argument conversion (C99 §6.5.2.2.7): each argument
                # is converted, as if by assignment, to the type of
                # the corresponding parameter. Mutate `args` in place
                # so the post-conversion arg list is what the back end
                # sees — same shape as Assignment / Binary / Conditional
                # promotion.
                for i, (arg, expected) in enumerate(
                    zip(args, fn_type.params),
                ):
                    self._check_exp(arg)
                    arg = _decay_if_array(arg)
                    args[i] = _convert_to(arg, expected)
                exp.data_type = fn_type.ret
                return fn_type.ret
            case c99_ast.Dereference(exp=inner):
                # `*e` — operand must have pointer type, result is
                # the pointee (C99 §6.5.3.2.4). The lvalue-ness of
                # the result is structural (Assignment / AddressOf /
                # Postfix accept Dereference); not encoded in the type.
                self._check_exp(inner)
                # `*arr` for an array decays arr first — `*(elem *)arr`
                # is the address of the first element dereferenced,
                # i.e. arr[0]. Same shape as `*(arr + 0)`.
                exp.exp = _decay_if_array(inner)
                inner = exp.exp
                t_inner = inner.data_type
                if not _is_pointer_type(t_inner):
                    raise TypeCheckError(
                        f"unary '*' requires a pointer operand, got "
                        f"{type(t_inner).__name__}"
                    )
                pointee = t_inner.referenced_type
                exp.data_type = pointee
                return pointee
            case c99_ast.InitList():
                # InitList only makes sense as the init slot of a
                # var_decl whose data_type is Array — `_check_block_var`
                # / `_check_for_init` consume it directly there. Hitting
                # this case from inside a regular expression context
                # means the user wrote `{1, 2}` somewhere it doesn't
                # belong (a return value, an arg, an assignment rval,
                # etc.).
                raise TypeCheckError(
                    "brace-enclosed initializer (`{...}`) is only "
                    "valid as a variable initializer for an array"
                )
            case c99_ast.SizeOfExp(exp=inner):
                # Type-check the inner expression to populate its
                # data_types — the type checker doesn't actually
                # evaluate anything, just validates and stamps
                # types, so this preserves the C99 §6.5.3.4.2
                # "operand is not evaluated" rule (the actual
                # not-evaluated guarantee is enforced in c99_to_tac
                # by NOT translating the inner). Crucially we do
                # NOT decay arrays at the top level — sizeof of an
                # array yields the array size, not the pointer
                # size (§6.3.2.1.3 explicitly excludes the operand
                # of sizeof from array decay). _check_exp leaves
                # the outer node's data_type un-decayed (decay is
                # only applied as expressions become operands of
                # other operators), so reading inner.data_type
                # directly gives the un-decayed type.
                self._check_exp(inner)
                inner_t = inner.data_type
                if inner_t is None:
                    raise TypeCheckError(
                        f"sizeof: inner expression has no type"
                    )
                if _is_void(inner_t):
                    raise TypeCheckError(
                        "sizeof of void expression is illegal "
                        "(C99 §6.5.3.4.1)"
                    )
                if isinstance(inner_t, FunType):
                    raise TypeCheckError(
                        "sizeof of a function type is illegal "
                        "(C99 §6.5.3.4.1)"
                    )
                # Result type is `unsigned long` (c6502's size_t).
                exp.data_type = ULong()
                return ULong()
            case c99_ast.SizeOfType(target_type=t):
                if _is_void(t):
                    raise TypeCheckError(
                        "sizeof(void) is illegal — void is an "
                        "incomplete type (C99 §6.5.3.4.1)"
                    )
                if isinstance(t, FunType):
                    raise TypeCheckError(
                        "sizeof of a function type is illegal "
                        "(C99 §6.5.3.4.1)"
                    )
                # Reject `sizeof (void[3])` etc. — array element
                # types must be complete.
                _check_well_formed_type(t, where="sizeof type")
                exp.data_type = ULong()
                return ULong()
            case c99_ast.Subscript(array=arr, index=idx):
                # `E1[E2]` per C99 §6.5.2.1.2 is defined as
                # `*((E1)+(E2))`. Because the underlying `+` is
                # commutative for pointer/integer pairs, the two
                # operands are symmetric: one must be pointer (or
                # array, after decay) and the other must be integer,
                # but either side can be either. So `arr[3]` and
                # `3[arr]` are equivalent and both valid C.
                #
                # Canonicalize back to (pointer, integer) order so
                # downstream passes (c99_to_tac.translate_pointer_
                # arithmetic, lvalue stores, AddressOf) see one
                # uniform shape.
                self._check_exp(arr)
                self._check_exp(idx)
                arr = _decay_if_array(arr)
                idx = _decay_if_array(idx)
                ta, ti = arr.data_type, idx.data_type
                if _is_pointer_type(ta) and _is_integer_type(ti):
                    ptr_exp, int_exp = arr, idx
                elif _is_pointer_type(ti) and _is_integer_type(ta):
                    # Reverse subscript `int[ptr]` — swap so
                    # `exp.array` ends up holding the pointer.
                    ptr_exp, int_exp = idx, arr
                else:
                    raise TypeCheckError(
                        f"subscript needs a pointer/array operand and "
                        f"an integer operand; got {ta!r} and {ti!r}"
                    )
                if isinstance(ptr_exp.data_type.referenced_type, FunType):
                    raise TypeCheckError(
                        "subscript of pointer-to-function is not "
                        "supported"
                    )
                # Widen the index to Long so c99_to_tac can use a
                # uniform 2-byte add to compute the byte address.
                exp.array = ptr_exp
                exp.index = _convert_to(int_exp, Long())
                exp.data_type = ptr_exp.data_type.referenced_type
                return exp.data_type
            case c99_ast.AddressOf(exp=inner):
                # `&e` — result is `Pointer(operand_type)` per C99
                # §6.5.3.2.3. Lvalue check on `e` lives in
                # identifier_resolution.
                #
                # Function names need a special case: the regular
                # `Var` lookup rejects function-typed names with
                # "function used as a variable" (since they aren't
                # legal in most expression contexts), but `&foo`
                # is exactly the place where they ARE legal —
                # taking the address of a function yields a function
                # pointer. Detect that shape directly so the inner
                # `_check_exp` doesn't trip the guard.
                if isinstance(inner, c99_ast.Var):
                    sym = self.symbols.get(inner.name)
                    if sym is not None and isinstance(sym.type, FunType):
                        inner.data_type = sym.type
                        result = Pointer(referenced_type=sym.type)
                        exp.data_type = result
                        return result
                t_inner = self._check_exp(inner)
                # `&arr` for an array — type per C99 §6.5.3.2.3 is
                # `Pointer(Array(elem, N))`. `_to_tac_data_type`
                # collapses Pointer onto Long, and `_pointee_size`
                # / `_sizeof` recurse into Array, so the downstream
                # pipeline handles this fine. Synthesised AddressOf
                # wrappers from `_decay_if_array` bypass `_check_exp`
                # (we set their data_type directly) so they don't
                # double-up.
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
