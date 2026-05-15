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
import fp_arith
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
Structure = c99_ast.Structure
Union = c99_ast.Union
Const = c99_ast.Const


# ---------------------------------------------------------------------------
# Struct / union layout (parallel to SymbolTable)
# ---------------------------------------------------------------------------


@dataclass(frozen=True)
class MemberInfo:
    """One row of a struct/union's layout: the member's source-level
    name, its declared type, and its byte offset from the start of the
    enclosing struct (always 0 for union members)."""
    name: str
    type: Type
    byte_offset: int


@dataclass
class StructLayout:
    """Layout for one struct or union tag. `complete=False` means the
    tag has been declared (`struct foo;`) but no body has been
    provided yet; `members` is empty and `size==0`. A subsequent
    declaration with a body upgrades the layout in place."""
    tag: str
    is_union: bool
    members: list[MemberInfo]
    size: int
    complete: bool


class TypeTable:
    """Flat program-global table mapping each struct/union tag to its
    layout. Block-scope tag visibility is tracked separately by the
    type checker (a stack of visible-tag sets) so a tag declared in
    an inner block isn't visible after exit, even though its layout
    sits in this table."""

    def __init__(self) -> None:
        self._table: dict[str, StructLayout] = {}

    def __contains__(self, tag: str) -> bool:
        return tag in self._table

    def __getitem__(self, tag: str) -> StructLayout:
        return self._table[tag]

    def __setitem__(self, tag: str, layout: StructLayout) -> None:
        self._table[tag] = layout

    def get(self, tag: str) -> StructLayout | None:
        return self._table.get(tag)

    def items(self):
        return self._table.items()

    def __len__(self) -> int:
        return len(self._table)

    def __repr__(self) -> str:
        return f"TypeTable({self._table!r})"


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


def _coerce_init_const_value(
    c: c99_ast.Type_const, target_type: "Type",
) -> int:
    """Map a c99 Constant's payload into `target_type`'s natural
    static-init form: a numeric int for integer / char target types
    (callers mask to width later), or an IEEE 754 bit pattern int
    for Float / Double targets. The constant's own type is inferred
    from its variant; the conversion goes through `fp_arith` whenever
    a Float / Double boundary is crossed."""
    src_type = _const_source_type(c)
    raw = c.bits if isinstance(
        c, (c99_ast.ConstFloat, c99_ast.ConstDouble),
    ) else c.value
    return _coerce_init_value(raw, src_type, target_type)


def _coerce_init_value(
    value: int, source_type: "Type", target_type: "Type",
) -> int:
    """Convert `value` (in `source_type`'s natural static-init form)
    into `target_type`'s natural form. Same-type conversions are a
    no-op; integer / integer pass through unbounded (callers mask to
    width); int ↔ FP routes through `fp_arith` so we never see a
    Python float."""
    if _types_equal(source_type, target_type):
        return value
    src_is_fp = isinstance(source_type, (Float, Double))
    tgt_is_fp = isinstance(target_type, (Float, Double))
    if src_is_fp and tgt_is_fp:
        # FP → FP precision change.
        if isinstance(source_type, Float):
            return fp_arith.single_bits_to_double_bits(value)
        return fp_arith.double_bits_to_single_bits(value)
    if not src_is_fp and tgt_is_fp:
        # Integer numeric value → IEEE 754 bits at target precision.
        if isinstance(target_type, Float):
            return fp_arith.int_to_single_bits(int(value))
        return fp_arith.int_to_double_bits(int(value))
    if src_is_fp and not tgt_is_fp:
        # IEEE 754 bits → integer numeric value, truncate toward
        # zero per C99 §6.3.1.4. Caller masks to the target type's
        # width as part of the static_init build.
        if isinstance(source_type, Float):
            return fp_arith.single_bits_to_int(value)
        return fp_arith.double_bits_to_int(value)
    # Integer → integer: pass through unbounded; the static_init
    # builder applies the target-width mask.
    return value


def _const_source_type(c: c99_ast.Type_const) -> "Type":
    """Map a c99 const variant to its data type, for the
    static-init coercion path."""
    if isinstance(c, c99_ast.ConstInt):
        return Int()
    if isinstance(c, c99_ast.ConstUInt):
        return UInt()
    if isinstance(c, c99_ast.ConstLong):
        return Long()
    if isinstance(c, c99_ast.ConstULong):
        return ULong()
    if isinstance(c, c99_ast.ConstLongLong):
        return LongLong()
    if isinstance(c, c99_ast.ConstULongLong):
        return ULongLong()
    if isinstance(c, c99_ast.ConstFloat):
        return Float()
    if isinstance(c, c99_ast.ConstDouble):
        return Double()
    if isinstance(c, c99_ast.ConstChar):
        return Char()
    if isinstance(c, c99_ast.ConstUChar):
        return UChar()
    raise TypeError(f"unexpected c99 const: {c!r}")


def _const_init_value(
    exp: c99_ast.Type_exp, target_type: "Type", name: str,
    symbols: SymbolTable | None = None,
) -> int | AddressInit:
    """Static-storage initializers must be compile-time constant
    expressions (C99 §6.7.8.4). After `_check_exp` and the
    initializer-conversion rule have run, the AST shape is one of:
      * a `Constant(...)` — drills to its int / bits value.
      * a `Cast` (possibly nested) wrapping any of these — produced
        by `_convert_to` for narrowing/widening initializers, or
        explicitly written by the user.
      * an `AddressOf(Var(name))` — taking the address of another
        static-storage object (or a function). C99 §6.6.7 paragraph
        9 makes this a valid constant expression; we capture it as
        an `AddressInit` and let codegen emit `DC.W name` so the
        assembler resolves the symbol at link time.

    The returned value is in `target_type`'s natural form:
      * integer types → Python int (numeric value, unbounded — the
        caller masks to the type's width when laying out the cell).
      * Float / Double → Python int holding the IEEE 754 bit pattern
        (32 or 64 bits respectively).
      * Pointer (with AddressOf operand) → AddressInit.

    Cast wrappers apply their conversion in turn, so the final value
    accounts for every cast in the chain (e.g., `(int)(float)0x100`
    routes through float, losing precision in the int→float→int
    round-trip). Integer / FP conversions go through `fp_arith` to
    avoid Python float intermediaries.

    `symbols` is consulted when an AddressOf shape is detected, to
    confirm the operand is a static-storage object or a function
    (taking the address of a local would be a runtime expression,
    not a constant expression).
    """
    match exp:
        case c99_ast.Constant(const=c):
            return _coerce_init_const_value(c, target_type)
        case c99_ast.Cast(target_type=cast_target, exp=inner):
            inner_val = _const_init_value(
                inner, cast_target, name, symbols,
            )
            return _coerce_init_value(
                inner_val, cast_target, target_type,
            )
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


def _zero_aggregate(t: "Type", types: "TypeTable | None" = None):
    """Build a default-zero value tree for static-storage object of
    type `t`. Scalars use the matching Python zero (`0` for integer
    / pointer, `0.0` for FP); arrays produce a tuple of size `N`,
    each element zeroed recursively; structs / unions produce a
    tuple keyed positionally by member-declaration order, each
    member zeroed recursively. Used both for tentative definitions
    (no init) and for trailing entries missing from a partial array
    initializer."""
    if isinstance(t, Array):
        return tuple(_zero_aggregate(t.element_type, types) for _ in range(t.size))
    if isinstance(t, (Structure, Union)):
        # Look up layout. For struct: zero each member; the value
        # tuple has one entry per member in declaration order. For
        # union: zero only the first member (unions get all-zero
        # bytes regardless; we use the first-member shape so the
        # static-init flattening machinery has a uniform tree to
        # walk).
        if types is None:
            return ()
        layout = types.get(t.tag)
        if layout is None or not layout.complete:
            return ()
        if isinstance(t, Union):
            if not layout.members:
                return ()
            return (_zero_aggregate(layout.members[0].type, types),)
        return tuple(
            _zero_aggregate(m.type, types) for m in layout.members
        )
    if isinstance(t, (Float, Double)):
        # IEEE 754 +0.0 has all-zero bits, so the same `0` works as
        # the bit-pattern representation for both Float and Double.
        return 0
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
    types: "TypeTable | None" = None,
):
    """Build a value tree (scalar or nested tuple) for an array /
    struct / union static initializer. Scalar leaves run through
    `_const_init_value` so all the existing constant-expression
    rules apply (Cast drilling, AddressOf static-storage); array /
    struct nodes recurse into their `InitList.items`, padding any
    missing trailing entries with the element / member type's
    typed-zero per C99 §6.7.8.21."""
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
                    init.items[i], elem_type, name, symbols, types,
                ))
            else:
                items.append(_zero_aggregate(elem_type, types))
        return tuple(items)
    if isinstance(var_type, (Structure, Union)):
        if not isinstance(init, c99_ast.InitList):
            raise TypeCheckError(
                f"static struct/union {name!r} requires a brace-"
                f"enclosed initializer (`{{...}}`)"
            )
        if types is None:
            raise TypeCheckError(
                f"static struct/union {name!r} initializer requires "
                f"a TypeTable"
            )
        layout = types.get(var_type.tag)
        if layout is None or not layout.complete:
            raise TypeCheckError(
                f"static struct/union {name!r} has incomplete type"
            )
        if isinstance(var_type, Union):
            # Per C99 §6.7.8.16, only the first named member of a
            # union is initialized when the initializer list isn't
            # a designated one. We accept at most one item.
            members = layout.members[:1]
        else:
            members = layout.members
        items: list = []
        for i, m in enumerate(members):
            if i < len(init.items):
                items.append(_const_init_aggregate(
                    init.items[i], m.type, name, symbols, types,
                ))
            else:
                items.append(_zero_aggregate(m.type, types))
        return tuple(items)
    return _const_init_value(init, var_type, name, symbols)


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
    also excluded — a function isn't an object. Structure / Union
    are object types but they don't reach scalar operand sites
    (the type checker rejects struct operands on arithmetic ops,
    bare struct rvalues on most contexts), so they're excluded
    here too — consumers that legitimately accept structs check
    explicitly. `Const(...)` is transparent — `const int` is still
    an object type."""
    t = _strip_const(t)
    return isinstance(
        t, (Int, Long, LongLong, UInt, ULong, ULongLong,
            Char, SChar, UChar,
            Float, Double, Pointer),
    )


def _is_complete_object_type(t: Type) -> bool:
    """Like `_is_object_type` but includes Array, Structure, and
    Union. Used at variable-declaration sites where these IS a legal
    type for the named object (the array / struct decays only when
    its name appears in certain expression contexts, not when it's
    being declared). `Const` is transparent."""
    t = _strip_const(t)
    return _is_object_type(t) or isinstance(
        t, (Array, Structure, Union),
    )


def _is_struct_or_union(t: Type) -> bool:
    return isinstance(_strip_const(t), (Structure, Union))


def _is_integer_type(t: Type) -> bool:
    return isinstance(
        _strip_const(t),
        (Int, Long, LongLong, UInt, ULong, ULongLong,
         Char, SChar, UChar),
    )


def _is_floating_type(t: Type) -> bool:
    return isinstance(_strip_const(t), (Float, Double))


def _is_pointer_type(t: Type) -> bool:
    return isinstance(_strip_const(t), Pointer)


def _is_array_type(t: Type) -> bool:
    return isinstance(_strip_const(t), Array)


def _is_void(t: Type) -> bool:
    """True iff `t` is the `void` type itself (NOT `void *`)."""
    return isinstance(_strip_const(t), Void)


def _is_void_pointer(t: Type) -> bool:
    """True iff `t` is `void *`."""
    t = _strip_const(t)
    return isinstance(t, Pointer) and isinstance(
        _strip_const(t.referenced_type), Void,
    )


def _strip_const(t: Type) -> Type:
    """Peel ONE top-level `Const` wrapper. `Const(Int)` → `Int`;
    `Const(Pointer(Const(Int)))` → `Pointer(Const(Int))` (only the
    outermost Const is removed — the pointee's Const stays).
    Idempotent for non-`Const` types."""
    if isinstance(t, c99_ast.Const):
        return t.referenced_type
    return t


def _strip_const_recursive(t: Type) -> Type:
    """Strip every `Const` wrapper anywhere in `t`. `Pointer(Const(Int))`
    becomes `Pointer(Int)`, `Const(Pointer(Const(Int)))` becomes
    `Pointer(Int)`, `Array(Const(Int), N)` becomes `Array(Int, N)`,
    etc. Used at the boundary into TAC where const-correctness is no
    longer relevant."""
    if isinstance(t, c99_ast.Const):
        return _strip_const_recursive(t.referenced_type)
    if isinstance(t, Pointer):
        return Pointer(referenced_type=_strip_const_recursive(t.referenced_type))
    if isinstance(t, Array):
        return Array(
            element_type=_strip_const_recursive(t.element_type),
            size=t.size,
        )
    if isinstance(t, FunType):
        return FunType(
            params=[_strip_const_recursive(p) for p in t.params],
            ret=_strip_const_recursive(t.ret),
        )
    return t


def _is_const_qualified(t: Type) -> bool:
    """True iff `t` has a top-level `Const` wrapper. Used at
    modification sites to reject `Assignment` / `CompoundAssignment` /
    `Prefix` / `Postfix` operations on a const-qualified lvalue."""
    return isinstance(t, c99_ast.Const)


def _propagate_const(member_t: Type, container_t: Type) -> Type:
    """Combine a member's declared type with the container's
    qualifier per C99 §6.5.2.3.3: a Const-qualified container
    accessed via `.` propagates Const to the member result. For
    `->`, pass the pointee in `container_t`. Idempotent — if
    `member_t` is already `Const(...)`, no extra wrapping."""
    if not _is_const_qualified(container_t):
        return member_t
    if isinstance(member_t, c99_ast.Const):
        return member_t
    return c99_ast.Const(referenced_type=member_t)


def _sizeof(t: Type, types: "TypeTable | None" = None) -> int:
    """Bytes occupied by a value of type `t` in c6502's storage
    model. Recursive for Array — `int[3][4]` is 24 bytes (Int = 2),
    `char[10]` is 10 bytes. For Structure / Union, looks up the
    tag's layout in `types` and reads its `.size` (raising if the
    layout is incomplete or `types` is None — sizeof of an
    incomplete type is illegal per C99 §6.5.3.4.1).

    `Const` is transparent — `sizeof(const int)` is `sizeof(int)`.

    Mirrors the helper of the same name in `c99_to_tac` (kept local
    so type_checking doesn't import the backend); the two stay in
    sync because they both encode the same storage-model rules.
    Used by the `sizeof` operator's type-checker. Constraint
    violations (Void, FunType) raise TypeCheckError."""
    t = _strip_const(t)
    if isinstance(t, (Char, SChar, UChar)):
        return 1
    if isinstance(t, (Int, UInt, Pointer)):
        return 2
    if isinstance(t, (Long, ULong, Float)):
        return 4
    if isinstance(t, (LongLong, ULongLong, Double)):
        return 8
    if isinstance(t, Array):
        return _sizeof(t.element_type, types) * t.size
    if isinstance(t, (Structure, Union)):
        if types is None:
            raise TypeCheckError(
                f"sizeof of struct/union type {t!r} requires a "
                f"TypeTable (none provided)"
            )
        layout = types.get(t.tag)
        if layout is None or not layout.complete:
            raise TypeCheckError(
                f"sizeof of incomplete struct/union type "
                f"'{'union' if isinstance(t, Union) else 'struct'} "
                f"{t.tag}' is illegal (C99 §6.5.3.4.1)"
            )
        return layout.size
    raise TypeCheckError(
        f"cannot take sizeof an incomplete or function type: {t!r}"
    )


def _check_well_formed_type(t: Type, *, where: str, types: "TypeTable | None" = None, require_complete: bool = False, tag_visible=None, auto_introduce=None) -> None:
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
    # Strip a top-level Const wrapper — the qualifier doesn't change
    # well-formedness; the underlying type is what we validate.
    t = _strip_const(t)
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
        # Array element must be a complete type — for struct
        # elements that means the tag's body must be visible.
        _check_well_formed_type(
            t.element_type, where=where, types=types,
            require_complete=True, tag_visible=tag_visible,
            auto_introduce=auto_introduce,
        )
        return
    if isinstance(t, Pointer):
        # Pointer's pointee may be incomplete (`struct foo *p;` is
        # legal even if `struct foo` only forward-declared) — pass
        # `require_complete=False` regardless of the caller's flag.
        _check_well_formed_type(
            t.referenced_type, where=where, types=types,
            require_complete=False, tag_visible=tag_visible,
            auto_introduce=auto_introduce,
        )
        return
    if isinstance(t, (Structure, Union)):
        if types is None:
            return  # caller didn't request layout validation
        # Strip the resolver-minted `@<N>.` prefix when rendering
        # error messages so the user sees their source spelling.
        display_tag = t.tag
        if display_tag.startswith("@") and "." in display_tag:
            display_tag = display_tag.split(".", 1)[1]
        layout = types.get(t.tag)
        # Tag visibility (per the live tag-scope stack). Three cases:
        # (1) tag never declared anywhere → auto-introduce as a
        #     forward declaration if `auto_introduce` is provided
        #     (which it is whenever a TypeChecker is on the stack);
        #     this implements C99's "appearance of `struct foo`
        #     in any declaration introduces the tag with incomplete
        #     type" rule. Required complete still fails.
        # (2) tag declared somewhere but not visible (popped scope,
        #     e.g. for-loop body's tag used outside) → reject as
        #     "not in scope". This is what makes `for_loop_scope.c`
        #     reject correctly.
        # (3) tag visible → proceed normally.
        if tag_visible is not None and not tag_visible(t.tag):
            if layout is None:
                # Auto-introduce as a forward declaration in the
                # current tag scope.
                if auto_introduce is not None:
                    auto_introduce(t)
                    layout = types.get(t.tag)
                else:
                    kw = "union" if isinstance(t, Union) else "struct"
                    raise TypeCheckError(
                        f"{where}: undeclared type '{kw} {display_tag}'"
                    )
            else:
                kw = "union" if isinstance(t, Union) else "struct"
                raise TypeCheckError(
                    f"{where}: '{kw} {display_tag}' is not in scope"
                )
        if layout is None:
            # Caller didn't pass tag_visible (so we couldn't tell if
            # the tag is in scope) but the tag isn't in the
            # TypeTable either — it was never declared.
            kw = "union" if isinstance(t, Union) else "struct"
            raise TypeCheckError(
                f"{where}: undeclared type '{kw} {display_tag}'"
            )
        # Tag-kind disagreement (`struct foo` referenced where the
        # in-scope `foo` was declared `union foo` or vice versa).
        if layout.is_union != isinstance(t, Union):
            kw = "union" if isinstance(t, Union) else "struct"
            pkw = "union" if layout.is_union else "struct"
            raise TypeCheckError(
                f"{where}: tag {display_tag!r} declared as '{pkw}' "
                f"but used as '{kw}'"
            )
        if require_complete and not layout.complete:
            kw = "union" if isinstance(t, Union) else "struct"
            raise TypeCheckError(
                f"{where}: incomplete type '{kw} {display_tag}' "
                f"(C99 §6.7.2.1.8 — type must be complete)"
            )
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
        return c.value == 0
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
    t = _strip_const(t)
    if isinstance(t, (Char, SChar, UChar)):
        return 1
    if isinstance(t, (Int, UInt)):
        return 2
    if isinstance(t, (Long, ULong)):
        return 4
    if isinstance(t, (LongLong, ULongLong)):
        return 8
    raise TypeError(f"_int_width: not an integer object type: {t!r}")


def _is_signed(t: Type) -> bool:
    # Plain `char` is unsigned in c6502 (0..255), matching `unsigned
    # char`. Per C99 §6.2.5.15 the choice is implementation-defined.
    return isinstance(_strip_const(t), (Int, Long, LongLong, SChar))


def _promote_integer(t: Type) -> Type:
    """C99 §6.3.1.1.2 integer promotion. Char/SChar/UChar promote
    to int (or unsigned int when int can't represent every value
    of the source type). For c6502 with Int = 16 bits:
      * SChar (-128..127)         → Int (-32768..32767): Int's range
        covers the full source range, promotes to Int via SignExtend.
      * Char / UChar (0..255)     → Int (-32768..32767): Int's range
        also covers 0..255, so plain `char` (which c6502 treats as
        unsigned) and `unsigned char` both promote to Int via
        ZeroExtend (NOT UInt — this is the key difference from
        c6502's earlier narrow-Int model where UChar's 0..255
        wouldn't fit in an 8-bit Int).
    Every other integer type already has rank ≥ Int, so promotion
    is a no-op for them. Floating types pass through unchanged.

    The promotion is conventionally applied at every operand
    position of an arithmetic / bitwise / comparison / shift
    operator, plus the operand of unary `+`, `-`, `~`. The result
    of `_convert_to(exp, _promote_integer(exp.data_type))` wraps
    `exp` in a SignExtend / ZeroExtend Cast (1B → 2B); c99_to_tac
    lowers those to byte-level inline sequences.

    Strips a top-level `Const` — qualifiers don't survive integer
    promotion (the promoted value is an rvalue; rvalues aren't
    qualified per C99 §6.3.2.1.2)."""
    t = _strip_const(t)
    if isinstance(t, (Char, SChar, UChar)):
        return Int()
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
        return c99_ast.ConstInt(value=bits) if isinstance(t, Int) else c99_ast.ConstUInt(value=bits)
    if isinstance(t, (Char, SChar, UChar)):
        # ConstChar / ConstUChar exist in the AST but the parser
        # routes char literals through ConstInt per the user's
        # choice. Reaching this branch means a char-typed switch
        # case got coerced to its declared char width — return a
        # typed ConstChar / ConstUChar so the AST stays self-
        # describing if anything inspects it. Plain `char` is
        # unsigned in c6502, so it routes to ConstUChar alongside
        # `unsigned char`; only `signed char` produces ConstChar.
        bits = value & 0xFF
        if isinstance(t, SChar):
            return c99_ast.ConstChar(value=bits)
        return c99_ast.ConstUChar(value=bits)
    if isinstance(t, (Long, ULong)):
        bits = value & 0xFFFF
        return c99_ast.ConstLong(value=bits) if isinstance(t, Long) else c99_ast.ConstULong(value=bits)
    if isinstance(t, (LongLong, ULongLong)):
        bits = value & 0xFFFFFFFF
        return (
            c99_ast.ConstLongLong(value=bits) if isinstance(t, LongLong)
            else c99_ast.ConstULongLong(value=bits)
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
    to AST nodes without aliasing. Strips `Const` from inputs —
    common-type computation operates on rvalue types, which aren't
    qualifier-bearing per C99 §6.3.2.1.2."""
    a = _strip_const(a)
    b = _strip_const(b)
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
    function.

    `Const` qualifiers on source/target are stripped before the
    compatibility checks — assignment conversion operates on
    unqualified types per C99 §6.5.16.1. The lvalue modification
    check fires at the Assignment site and is independent of this
    conversion logic."""
    if exp.data_type is not None and _types_equal(
        _strip_const_recursive(exp.data_type),
        _strip_const_recursive(target),
    ):
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
    """Walks one program, populating `self.symbols` and `self.types`.
    The same instance is used for the whole program so both tables
    accumulate across all top-level declarations.

    Tag visibility is per-block: `_tag_scopes` is a stack of sets,
    pushed on every new block-scope (Compound bodies, for-headers,
    function bodies) and popped on exit. The bottom of the stack is
    the file-scope tag set. A tag must be in some live scope-set for
    a `Structure(tag)` reference to be valid; the layout itself
    lives flat in `self.types` so block-scope shadowing isn't
    supported (it would need per-scope unique tag names; deferred
    until a use-case demands it)."""

    def __init__(self) -> None:
        self.symbols = SymbolTable()
        self.types = TypeTable()
        # Type the enclosing function should return — set by
        # `_check_function_decl` while walking a body, restored on
        # exit. Used by `_check_statement` to type-check `return`s.
        self._return_type: Type | None = None
        # Tag visibility stack — outermost (file scope) at index 0.
        self._tag_scopes: list[set[str]] = [set()]

    def _push_tag_scope(self) -> None:
        self._tag_scopes.append(set())

    def _pop_tag_scope(self) -> None:
        self._tag_scopes.pop()

    def _tag_visible(self, tag: str) -> bool:
        return any(tag in s for s in self._tag_scopes)

    def _record_tag_visible(self, tag: str) -> None:
        self._tag_scopes[-1].add(tag)

    def _auto_introduce_tag(self, t) -> None:
        """Add a forward declaration for `t.tag` to the TypeTable
        and the current tag scope. Used when a Structure / Union
        reference appears (typically through a Pointer) without
        a prior declaration — C99's "appearance of `struct foo`
        in any declaration introduces it" rule. Only called when
        the tag is genuinely unseen; redundant calls would
        silently overwrite the existing layout."""
        is_union = isinstance(t, c99_ast.Union)
        self.types[t.tag] = StructLayout(
            tag=t.tag, is_union=is_union,
            members=[], size=0, complete=False,
        )
        self._record_tag_visible(t.tag)

    def _require_scalar_controlling(
        self, exp: c99_ast.Type_exp, where: str,
    ) -> None:
        """The controlling expression of an `if` / `while` / `do` /
        `for` / `?:` shall have scalar type (C99 §6.8.4.1.1 /
        §6.8.5.2 / §6.5.15.2). Scalar = arithmetic + pointer; struct
        / union / void are rejected. Arrays decay to pointers in
        operand context (§6.3.2.1.3) and are accepted here."""
        t = exp.data_type
        if isinstance(t, Array):
            return
        if not _is_object_type(t):
            kind = type(t).__name__
            tag = ""
            if isinstance(t, (Structure, Union)):
                kw = "union" if isinstance(t, Union) else "struct"
                tag = f" '{kw} {t.tag}'"
            raise TypeCheckError(
                f"{where}: controlling expression must have scalar "
                f"type, got {kind}{tag} (C99 §6.8.4.1)"
            )

    def _require_complete_value(
        self, exp: c99_ast.Type_exp, where: str,
    ) -> None:
        """If `exp.data_type` is an incomplete struct/union type,
        reject — used at sites that materialize the value of an
        expression (assignment lval/rval, cast operand, function-
        call return, expression statement, for-init expression).
        C99 §6.7.2.1.8 forbids reading or writing the value of an
        incomplete-typed object: its size and layout are unknown
        until the type is completed. Sites that take the address
        of an expression (`&v`, `sizeof v` once v has incomplete
        type — though sizeof has its own check) DON'T materialize
        the value, so they bypass this helper."""
        t = exp.data_type
        if isinstance(t, (Structure, Union)):
            layout = self.types.get(t.tag)
            if layout is None or not layout.complete:
                kw = "union" if isinstance(t, Union) else "struct"
                raise TypeCheckError(
                    f"{where}: incomplete type '{kw} {t.tag}' "
                    f"(C99 §6.7.2.1.8)"
                )

    def check_program(
        self, prog: c99_ast.Type_program,
    ) -> tuple[c99_ast.Type_program, SymbolTable, TypeTable]:
        match prog:
            case c99_ast.Program(declaration=decls):
                for d in decls:
                    self._check_file_scope_declaration(d)
                return prog, self.symbols, self.types
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
            case c99_ast.StructDecl(struct_decl=sd):
                self._check_struct_decl(sd)
            case _:
                raise TypeError(f"unexpected declaration: {decl!r}")

    def _check_struct_decl(
        self, sd: c99_ast.Type_struct_decl,
    ) -> None:
        """Validate a struct/union declaration and (if it has a body)
        compute its layout. The tag becomes visible in the current
        scope. Re-declarations of the same tag in the same scope are
        legal: a forward decl + a body completes the layout; two
        bodies for the same tag in the same scope is a redefinition
        error."""
        tag = sd.tag
        is_union = sd.is_union
        prior = self.types.get(tag)
        # Visibility: tag enters the current tag scope (whether or
        # not it was already in some outer scope — a same-spelling
        # tag declared in the inner scope shadows the outer one for
        # visibility purposes; the layout in the flat TypeTable is
        # shared, which is fine for the corpus we target).
        self._record_tag_visible(tag)
        if not sd.members:
            # Forward declaration. Register an incomplete layout if
            # nothing's been registered yet; otherwise leave the
            # existing entry alone.
            if prior is None:
                self.types[tag] = StructLayout(
                    tag=tag, is_union=is_union,
                    members=[], size=0, complete=False,
                )
            elif prior.is_union != is_union:
                kw = "union" if is_union else "struct"
                pkw = "union" if prior.is_union else "struct"
                raise TypeCheckError(
                    f"redeclaration of '{kw} {tag}' as a {pkw}"
                )
            return
        # Definition with a body.
        if prior is not None:
            if prior.is_union != is_union:
                kw = "union" if is_union else "struct"
                pkw = "union" if prior.is_union else "struct"
                raise TypeCheckError(
                    f"redeclaration of '{pkw} {tag}' as a {kw}"
                )
            if prior.complete:
                kw = "union" if is_union else "struct"
                raise TypeCheckError(
                    f"redefinition of '{kw} {tag}'"
                )
        # Register a forward-declared layout BEFORE computing the
        # final one, so self-referential members (`struct linked_list
        # { struct linked_list *next; };`) can resolve their pointee
        # type through the in-progress declaration. The pointer's
        # well-formedness check passes `require_complete=False`, so
        # the incomplete entry is enough.
        if prior is None:
            self.types[tag] = StructLayout(
                tag=tag, is_union=is_union,
                members=[], size=0, complete=False,
            )
        # Compute the layout. Each member's type must be a complete
        # object type; in particular, recursive struct types are
        # rejected unless the recursion is via a Pointer.
        layout = self._compute_layout(sd)
        self.types[tag] = layout

    def _compute_layout(
        self, sd: c99_ast.Type_struct_decl,
    ) -> StructLayout:
        members: list[MemberInfo] = []
        seen: set[str] = set()
        offset = 0
        for m in sd.members:
            if m.name in seen:
                raise TypeCheckError(
                    f"duplicate member {m.name!r} in struct/union "
                    f"{sd.tag!r}"
                )
            seen.add(m.name)
            mtype = m.data_type
            # Member can't be void, can't be a function type.
            if isinstance(mtype, Void):
                raise TypeCheckError(
                    f"member {m.name!r} of struct/union {sd.tag!r} "
                    f"cannot have void type"
                )
            if isinstance(mtype, FunType):
                raise TypeCheckError(
                    f"member {m.name!r} of struct/union {sd.tag!r} "
                    f"cannot have function type"
                )
            # Reject self-recursion via non-pointer (struct foo {
            # struct foo inner; }) — through a Pointer is fine.
            if self._contains_self_struct(mtype, sd.tag):
                raise TypeCheckError(
                    f"member {m.name!r} of struct/union {sd.tag!r} "
                    f"recursively contains the same struct/union "
                    f"type (use a pointer instead)"
                )
            _check_well_formed_type(
                mtype,
                where=f"member {m.name!r} of struct/union {sd.tag!r}",
                types=self.types,
                require_complete=True,
                tag_visible=self._tag_visible,
                auto_introduce=self._auto_introduce_tag,
            )
            msize = _sizeof(mtype, self.types)
            mem = MemberInfo(
                name=m.name, type=mtype, byte_offset=offset,
            )
            members.append(mem)
            if sd.is_union:
                # Union members all live at offset 0; total size is
                # the maximum of member sizes (so we keep `offset`
                # at 0 for the next member but track size as max).
                if msize > offset:
                    pass  # tracked via final pass below
            else:
                offset += msize
        if sd.is_union:
            total = max(
                (_sizeof(m.data_type, self.types) for m in sd.members),
                default=0,
            )
            # Re-stamp every member offset to 0 (defensive — the
            # construction loop already does this).
            members = [
                MemberInfo(name=m.name, type=m.type, byte_offset=0)
                for m in members
            ]
        else:
            total = offset
        return StructLayout(
            tag=sd.tag, is_union=sd.is_union,
            members=members, size=total, complete=True,
        )

    def _contains_self_struct(self, t: Type, tag: str) -> bool:
        """True iff `t` contains a Structure/Union(tag) reference
        without going through a Pointer. Walks Array element types
        recursively; stops at Pointer."""
        if isinstance(t, (Structure, Union)) and t.tag == tag:
            return True
        if isinstance(t, Array):
            return self._contains_self_struct(t.element_type, tag)
        return False

    def _lookup_member(
        self, t: Type, member: str, where: str,
    ) -> MemberInfo:
        """Resolve a member name against a struct/union type. Raises
        TypeCheckError on incomplete-type access or missing member.
        Strips a top-level `Const` — `(const struct S).m` looks up
        `m` on the underlying `struct S`. (Const propagation to the
        member's result type is the caller's job.)"""
        t = _strip_const(t)
        if not isinstance(t, (Structure, Union)):
            raise TypeCheckError(
                f"{where}: operand has non-struct/union type {t!r}"
            )
        if not self._tag_visible(t.tag):
            kw = "union" if isinstance(t, Union) else "struct"
            raise TypeCheckError(
                f"{where}: '{kw} {t.tag}' is not in scope"
            )
        layout = self.types.get(t.tag)
        if layout is None or not layout.complete:
            kw = "union" if isinstance(t, Union) else "struct"
            raise TypeCheckError(
                f"{where}: incomplete type '{kw} {t.tag}'"
            )
        for m in layout.members:
            if m.name == member:
                return m
        kw = "union" if isinstance(t, Union) else "struct"
        raise TypeCheckError(
            f"{where}: '{kw} {t.tag}' has no member named "
            f"{member!r}"
        )

    def _check_file_scope_var_well_formed(
        self, vd: c99_ast.Type_var_decl,
    ) -> None:
        require_complete = isinstance(
            vd.data_type, (Structure, Union, Array),
        )
        _check_well_formed_type(
            vd.data_type, where=f"file-scope object {vd.name!r}",
            types=self.types, require_complete=require_complete,
            tag_visible=self._tag_visible,
                auto_introduce=self._auto_introduce_tag,
        )

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
        is_extern_decl = isinstance(vd.storage_class, c99_ast.Extern)
        # `extern` references to struct types may be incomplete (the
        # definition can live in another TU). Otherwise the type
        # must be complete.
        require_complete = (
            isinstance(vd.data_type, (Structure, Union, Array))
            and not is_extern_decl
        )
        _check_well_formed_type(
            vd.data_type, where=f"file-scope object {vd.name!r}",
            types=self.types, require_complete=require_complete,
            tag_visible=self._tag_visible,
                auto_introduce=self._auto_introduce_tag,
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
                    vd.init, vd.data_type, vd.name,
                    self.symbols, self.types,
                ))
        elif isinstance(vd.data_type, (Structure, Union)):
            # File-scope `struct s x = {1,2,3};` — type-check the
            # init list against the layout and lift to a value tree.
            if not isinstance(vd.init, c99_ast.InitList):
                raise TypeCheckError(
                    f"file-scope struct/union {vd.name!r} requires "
                    f"a brace-enclosed initializer (`{{...}}`)"
                )
            self._check_struct_init_list(
                vd.init, vd.data_type, vd.name,
            )
            initial = Initial(_const_init_aggregate(
                vd.init, vd.data_type, vd.name,
                self.symbols, self.types,
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
                _const_init_value(
                    vd.init, vd.data_type, vd.name, self.symbols,
                ),
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
            # Parameters may have struct/union types — the tag must
            # be declared. For a function *definition* the type must
            # also be complete (the body needs to know its size to
            # access it); a forward declaration can name an
            # incomplete struct/union as long as the type is
            # completed before any caller tries to use it (C99
            # §6.7.2.1.8). Array element types must always be
            # complete; the outermost array suffix is rewritten to a
            # pointer by the parameter adjustment, so a surviving
            # Array here is an inner array (`int (*a)[3]`).
            require_complete = isinstance(p_type, Array) or (
                defined and isinstance(p_type, (Structure, Union))
            )
            _check_well_formed_type(
                p_type, where=f"parameter of function {fd.name!r}",
                types=self.types, require_complete=require_complete,
                tag_visible=self._tag_visible,
                auto_introduce=self._auto_introduce_tag,
            )
        # The return type also has to be well-formed. For function
        # *definitions* a struct/union return must be complete
        # (the body may compute the bytes, but it can't write a
        # zero-sized object); a forward declaration can name an
        # incomplete struct in its return type as long as no
        # caller tries to use the result before the type is
        # completed.
        ret_t = ftype.ret
        require_ret_complete = (
            defined
            and isinstance(ret_t, (Structure, Union))
        )
        _check_well_formed_type(
            ret_t, where=f"return type of function {fd.name!r}",
            types=self.types,
            require_complete=require_ret_complete,
            tag_visible=self._tag_visible,
            auto_introduce=self._auto_introduce_tag,
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
            # Push a tag scope for the function body. (The
            # function-prototype scope is its own thing per
            # §6.2.1.4, but c6502 doesn't permit struct/union
            # definitions in parameter declarations, so the only
            # tags reaching here are the ones already in the
            # surrounding scope.)
            self._push_tag_scope()
            try:
                self._check_block(fd.body)
            finally:
                self._pop_tag_scope()
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
            case c99_ast.StructDecl(struct_decl=sd):
                self._check_struct_decl(sd)
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
        # For struct/union object types the tag must be a complete
        # type at the point of declaration. (Pointers to struct
        # may point to incomplete types — the well-formed-type walk
        # passes `require_complete=False` through Pointer.)
        require_complete = isinstance(
            vd.data_type, (Structure, Union, Array),
        )
        _check_well_formed_type(
            vd.data_type, where=f"object {vd.name!r}",
            types=self.types, require_complete=require_complete,
            tag_visible=self._tag_visible,
                auto_introduce=self._auto_introduce_tag,
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
                initial: InitialValue = Initial(_zero_aggregate(vd.data_type, self.types))
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
                        vd.init, vd.data_type, vd.name,
                        self.symbols, self.types,
                    ))
            elif isinstance(vd.data_type, (Structure, Union)):
                # `static struct s x = {1,2};`.
                if not isinstance(vd.init, c99_ast.InitList):
                    raise TypeCheckError(
                        f"static struct/union {vd.name!r} requires "
                        f"a brace-enclosed initializer (`{{...}}`)"
                    )
                self._check_struct_init_list(
                    vd.init, vd.data_type, vd.name,
                )
                initial = Initial(_const_init_aggregate(
                    vd.init, vd.data_type, vd.name,
                    self.symbols, self.types,
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
                    _const_init_value(
                        vd.init, vd.data_type, vd.name, self.symbols,
                    ),
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
            # `Const(Array(...))` / `Const(Structure(...))` shouldn't
            # normally arise (C99 §6.7.3.8 says qualifiers on an
            # array specify its elements; on structs they ride at
            # the variable level). Strip a top-level Const here so
            # the per-shape branches dispatch on the underlying
            # aggregate type uniformly.
            data_type_uq = _strip_const(vd.data_type)
            if isinstance(data_type_uq, Array):
                # Arrays must use a brace-enclosed initializer list
                # (`int a[3] = {1, 2, 3};`); a bare scalar is illegal.
                # The char-array + string-literal special case
                # (`char a[N] = "abc";`) takes its own §6.7.8.14
                # path — c99_to_tac then emits per-byte stores.
                if (
                    isinstance(vd.init, c99_ast.String)
                    and _is_char_element(data_type_uq.element_type)
                ):
                    self._check_string_array_init(
                        vd.init, data_type_uq, vd.name,
                    )
                    return
                if not isinstance(vd.init, c99_ast.InitList):
                    raise TypeCheckError(
                        f"array {vd.name!r} requires a brace-enclosed "
                        f"initializer (`{{...}}`)"
                    )
                self._check_array_init_list(
                    vd.init, data_type_uq, vd.name,
                )
                return
            if isinstance(data_type_uq, (Structure, Union)):
                # Two valid initializer forms for struct/union:
                #   `struct s x = {1, 2};`  → InitList per-member
                #   `struct s x = other;`   → struct copy (other has
                #                             matching struct type)
                if isinstance(vd.init, c99_ast.InitList):
                    self._check_struct_init_list(
                        vd.init, data_type_uq, vd.name,
                    )
                    return
                # Non-InitList — must be a struct-typed expression
                # of the matching tag.
                self._check_exp(vd.init)
                init_t = vd.init.data_type
                if not _types_equal(
                    _strip_const(init_t), data_type_uq,
                ):
                    raise TypeCheckError(
                        f"struct/union {vd.name!r}: initializer has "
                        f"type {init_t!r}, expected {vd.data_type!r}"
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

    def _check_struct_init_list(
        self,
        init: c99_ast.InitList,
        struct_type: "Type",
        var_name: str,
    ) -> None:
        """Type-check a brace-enclosed initializer for a struct or
        union. Validates the item count (≤ member count) and converts
        each item to the matching member's type via `_convert_to`.
        Mutates `init.items` in place with the converted forms."""
        layout = self.types.get(struct_type.tag)
        if layout is None or not layout.complete:
            kw = "union" if isinstance(struct_type, Union) else "struct"
            raise TypeCheckError(
                f"struct/union {var_name!r}: incomplete type "
                f"'{kw} {struct_type.tag}'"
            )
        if isinstance(struct_type, Union):
            # Per C99 §6.7.8.16, a non-designated initializer for a
            # union initializes the *first named member*. At most
            # one item permitted.
            members = layout.members[:1]
            if len(init.items) > 1:
                raise TypeCheckError(
                    f"too many initializers for union {var_name!r}: "
                    f"{len(init.items)} given, only one (the first "
                    f"named member) is permitted without designators"
                )
        else:
            members = layout.members
            if len(init.items) > len(members):
                raise TypeCheckError(
                    f"too many initializers for struct {var_name!r}: "
                    f"{len(init.items)} given, struct has "
                    f"{len(members)} member"
                    f"{'s' if len(members) != 1 else ''}"
                )
        for i, item in enumerate(init.items):
            m = members[i]
            elem_type = m.type
            if isinstance(elem_type, Array):
                # Char-array member initialized by string literal.
                if (
                    isinstance(item, c99_ast.String)
                    and _is_char_element(elem_type.element_type)
                ):
                    self._check_string_array_init(
                        item, elem_type, f"{var_name}.{m.name}",
                    )
                    continue
                if not isinstance(item, c99_ast.InitList):
                    raise TypeCheckError(
                        f"expected nested initializer (`{{...}}`) "
                        f"for member {m.name!r} of {var_name!r}; "
                        f"member type is {elem_type!r}"
                    )
                self._check_array_init_list(item, elem_type, var_name)
            elif isinstance(elem_type, (Structure, Union)):
                if not isinstance(item, c99_ast.InitList):
                    raise TypeCheckError(
                        f"expected nested initializer (`{{...}}`) "
                        f"for member {m.name!r} of {var_name!r}; "
                        f"member type is {elem_type!r}"
                    )
                self._check_struct_init_list(
                    item, elem_type, f"{var_name}.{m.name}",
                )
            else:
                if isinstance(item, c99_ast.InitList):
                    raise TypeCheckError(
                        f"unexpected nested initializer for member "
                        f"{m.name!r} of {var_name!r} (member type is "
                        f"{elem_type!r}, not aggregate)"
                    )
                self._check_exp(item)
                item = _decay_if_array(item)
                init.items[i] = _convert_to(item, elem_type)
        init.data_type = struct_type

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
            elif isinstance(elem_type, (Structure, Union)):
                # Array of structs / unions: each item is a nested
                # InitList for the element struct.
                if not isinstance(item, c99_ast.InitList):
                    raise TypeCheckError(
                        f"expected nested initializer (`{{...}}`) "
                        f"at index {i} of {var_name!r}; element "
                        f"type is {elem_type!r}"
                    )
                self._check_struct_init_list(
                    item, elem_type, f"{var_name}[{i}]",
                )
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
                # }` is the standard idiom. Struct/union returns
                # bypass `_convert_to` (no Cast for struct types);
                # the value's type must match exactly.
                exp = _decay_if_array(exp)
                if isinstance(expected, (Structure, Union)):
                    if not _types_equal(exp.data_type, expected):
                        raise TypeCheckError(
                            f"return value type {exp.data_type!r} "
                            f"doesn't match declared return type "
                            f"{expected!r}"
                        )
                    stmt.exp = exp
                else:
                    stmt.exp = _convert_to(exp, expected)
                return
            case c99_ast.Expression(exp=exp):
                self._check_exp(exp)
                # `e;` evaluates `e` and discards the result —
                # incomplete struct/union values can't be
                # materialized.
                self._require_complete_value(
                    exp, "expression statement",
                )
                return
            case c99_ast.IfStmt(
                condition=cond, then_clause=then_, else_clause=else_,
            ):
                self._check_exp(cond)
                self._require_scalar_controlling(cond, "`if` condition")
                self._check_statement(then_)
                if else_ is not None:
                    self._check_statement(else_)
                return
            case c99_ast.Compound(block=block):
                # New tag-visibility scope for the compound block.
                self._push_tag_scope()
                try:
                    self._check_block(block)
                finally:
                    self._pop_tag_scope()
                return
            case c99_ast.WhileStmt(condition=cond, body=body):
                self._check_exp(cond)
                self._require_scalar_controlling(
                    cond, "`while` condition",
                )
                self._check_statement(body)
                return
            case c99_ast.DoWhileStmt(body=body, condition=cond):
                self._check_statement(body)
                self._check_exp(cond)
                self._require_scalar_controlling(
                    cond, "`do`-`while` condition",
                )
                return
            case c99_ast.ForStmt(
                init=init, condition=cond, post_clause=post,
                body=body,
            ):
                # The for-header opens its own block-scope per C99
                # §6.8.5.3 (so a struct declared in init / cond /
                # post isn't visible outside the loop). The body
                # opens its own nested scope on top via Compound.
                self._push_tag_scope()
                try:
                    self._check_for_init(init)
                    if cond is not None:
                        self._check_exp(cond)
                        self._require_scalar_controlling(
                            cond, "`for` condition",
                        )
                    if post is not None:
                        self._check_exp(post)
                    self._check_statement(body)
                finally:
                    self._pop_tag_scope()
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
        # Integer promotion (§6.3.1.1). Char-typed control
        # operands (Char/SChar/UChar — all 1-byte integer types in
        # c6502) promote to Int / UInt; the rank-≥-Int integer
        # types pass through unchanged. We rewrap stmt.control in
        # the implicit promotion Cast so the dispatch in
        # c99_to_tac reads from the promoted-type val (matters for
        # char switches: case constants would otherwise coerce
        # modulo 256 and a 33554632-valued case would fold to -56,
        # spuriously matching a `char c = -56` control).
        promoted = _promote_integer(ctrl_type)
        if promoted != ctrl_type:
            stmt.control = _convert_to(stmt.control, promoted)
        # Stash on the SwitchStmt so c99_to_tac can match case-
        # constant variants to the dispatch type without recomputing.
        stmt.promoted_type = promoted
        ctrl_type = promoted
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
            case c99_ast.InitDecl(var_decls=vds):
                # The for-init-decl rule (resolver) forbids storage-
                # class specifiers on every declarator, so each is
                # plain `T <name> = <exp>;` and lands as a LocalAttr.
                for vd in vds:
                    if not _is_complete_object_type(vd.data_type):
                        raise TypeCheckError(
                            f"for-init {vd.name!r} declared with non-"
                            f"object type {vd.data_type!r}"
                        )
                    _check_well_formed_type(
                        vd.data_type, where=f"for-init {vd.name!r}",
                        types=self.types,
                        require_complete=isinstance(
                            vd.data_type, (Structure, Union, Array),
                        ),
                        tag_visible=self._tag_visible,
                        auto_introduce=self._auto_introduce_tag,
                    )
                    self.symbols[vd.name] = Symbol(
                        type=vd.data_type, attrs=LocalAttr(),
                    )
                    if vd.init is None:
                        continue
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
                        continue
                    if isinstance(vd.data_type, (Structure, Union)):
                        # Same two forms accepted by block-scope var
                        # decls: brace-enclosed compound initializer
                        # or a struct-typed copy.
                        if isinstance(vd.init, c99_ast.InitList):
                            self._check_struct_init_list(
                                vd.init, vd.data_type, vd.name,
                            )
                            continue
                        self._check_exp(vd.init)
                        init_t = vd.init.data_type
                        if not _types_equal(init_t, vd.data_type):
                            raise TypeCheckError(
                                f"struct/union {vd.name!r}: "
                                f"initializer has type {init_t!r}, "
                                f"expected {vd.data_type!r}"
                            )
                        continue
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
                    # Same as Expression statement — the value is
                    # evaluated and discarded, so an incomplete
                    # struct/union expression isn't usable here.
                    self._require_complete_value(
                        exp, "for-init expression",
                    )
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
        # Strip top-level Const so subsequent `_types_equal` and
        # `.referenced_type` accesses see the bare pointer shape.
        # Const-qualification of the pointer itself (`int * const`)
        # doesn't change equality semantics.
        tl_p = _strip_const(tl)
        tr_p = _strip_const(tr)
        if l_ptr and r_ptr:
            # `void *` matches any object pointer here, with the
            # common type being `void *`.
            if _is_void_pointer(tl) or _is_void_pointer(tr):
                return Pointer(referenced_type=Void())
            if not _types_equal(tl_p, tr_p):
                raise TypeCheckError(
                    f"comparison of distinct pointer types: "
                    f"{tl!r} vs {tr!r}"
                )
            # Fresh instance so callers can attach to AST nodes
            # without aliasing — same convention as `_common_type`.
            return Pointer(referenced_type=tl_p.referenced_type)
        # Exactly one operand is a pointer; the other must be a
        # null pointer constant.
        if l_ptr and _is_null_pointer_constant(rhs):
            return Pointer(referenced_type=tl_p.referenced_type)
        if r_ptr and _is_null_pointer_constant(lhs):
            return Pointer(referenced_type=tr_p.referenced_type)
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
                _check_well_formed_type(
                    target, where="cast target",
                    types=self.types,
                    tag_visible=self._tag_visible,
                auto_introduce=self._auto_introduce_tag,
                )
                self._check_exp(inner)
                # Decay an array operand before further type-checking
                # — `(int *)arr` is legal and the cast operates on
                # the decayed pointer.
                exp.exp = _decay_if_array(inner)
                inner = exp.exp
                inner_type = inner.data_type
                # The operand of a cast is evaluated even for the
                # `(void)e` discard form (C99 §6.3.2.2.1), so its
                # value must be representable — which rules out
                # incomplete struct/union types.
                self._require_complete_value(inner, "cast operand")
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
                #   ptr - ptr (same type)  → Int (element count;
                #                            c6502's stand-in for the
                #                            standard's ptrdiff_t —
                #                            Int is 2 bytes signed,
                #                            matches pointer width)
                # Anything else (ptr + ptr, int - ptr, ptr ± FP,
                # mismatched ptr - ptr, pointer-to-function
                # arithmetic) is a constraint violation.
                if (
                    isinstance(op, (c99_ast.Add, c99_ast.Subtract))
                    and (_is_pointer_type(tl) or _is_pointer_type(tr))
                ):
                    op_name = "+" if isinstance(op, c99_ast.Add) else "-"
                    # Strip top-level Const from each pointer operand
                    # so subsequent `isinstance(t, Pointer)` checks
                    # and `.referenced_type` field accesses work
                    # uniformly. The const-on-the-pointer-itself
                    # doesn't affect pointer arithmetic legality.
                    tl_p = _strip_const(tl)
                    tr_p = _strip_const(tr)
                    # Reject pointer-to-function and pointer-to-void:
                    # §6.5.6.2 requires "pointer to a complete object
                    # type" for the additive ops, and sizeof(void) /
                    # sizeof(function) is undefined.
                    for t in (tl_p, tr_p):
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
                        # the pointer types match. Compare on the
                        # un-Const-qualified pointer types — `int *`
                        # and `int * const` are compatible for
                        # subtraction.
                        if isinstance(op, c99_ast.Add):
                            raise TypeCheckError(
                                f"binary '+' is not defined on two "
                                f"pointer operands ({tl!r} + {tr!r})"
                            )
                        if not _types_equal(tl_p, tr_p):
                            raise TypeCheckError(
                                f"subtraction of distinct pointer "
                                f"types: {tl!r} - {tr!r}"
                            )
                        # Operand types stay as-is — both pointers are
                        # 2 bytes at the byte level, so the underlying
                        # subtract is a normal 2-byte op. The result
                        # is a byte-difference that c99_to_tac will
                        # divide by sizeof(pointee) to yield the
                        # element count. Result type is Int (c6502's
                        # ptrdiff_t — 2 bytes signed, matches the
                        # 16-bit address width).
                        exp.data_type = Int()
                        return Int()
                    # Exactly one operand is a pointer; the other must
                    # be integer. ptr + int / int + ptr / ptr - int are
                    # legal; int - ptr is not.
                    int_is_left = _is_integer_type(tl)
                    if int_is_left and isinstance(op, c99_ast.Subtract):
                        raise TypeCheckError(
                            f"binary '-' between integer and pointer "
                            f"is not defined ({tl!r} - {tr!r})"
                        )
                    # Widen the integer operand to Int (a 2-byte
                    # type) so the underlying byte-level add lines up
                    # with the pointer's 2-byte width. c99_to_tac
                    # scales this widened value by sizeof(pointee)
                    # before the add. Pre-refactor this widened to
                    # Long; after the C99 width refresh Long is 4
                    # bytes — wider than a pointer — so we widen to
                    # Int (which is now exactly 2 bytes).
                    if int_is_left:
                        exp.left = _convert_to(lhs, Int())
                        ptr_type = tr_p
                    else:
                        exp.right = _convert_to(rhs, Int())
                        ptr_type = tl_p
                    # Result is the pointer type (un-const-qualified
                    # at the top level — the rvalue isn't qualified
                    # per §6.3.2.1.2). Pointee const, if any, rides
                    # through.
                    exp.data_type = Pointer(
                        referenced_type=ptr_type.referenced_type,
                    )
                    return exp.data_type
                # Bitwise / shift / modulo (C99 §6.5.5.2 / §6.5.7.2 /
                # §6.5.10.2 / §6.5.11.2 / §6.5.12.2): operands must
                # have integer type. Reject Float / Double directly.
                # Pointer operands for non-shift bitwise / modulo
                # ops fall through to `_common_type` which crashes
                # on Pointer; the chapter_14 pointer-bitwise tests
                # rely on that path's crash for rejection. Shifts
                # are special — they skip `_common_type` (per the
                # branch below), so Pointer-typed shift operands
                # need an explicit rejection here.
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
                        or (
                            isinstance(op, (
                                c99_ast.LeftShift, c99_ast.RightShift,
                            ))
                            and (
                                isinstance(tl, c99_ast.Pointer)
                                or isinstance(tr, c99_ast.Pointer)
                            )
                        )
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
                # Shifts (C99 §6.5.7.3): the integer promotions are
                # performed on each operand independently (already
                # done above) and the result type is the type of the
                # promoted LEFT operand — NOT the common type. Skip
                # the usual arithmetic conversions; the right
                # operand keeps its independently-promoted type.
                # Tac_to_asm passes only the right operand's low
                # byte to the asl/asr/lsr helpers (a shift count of
                # ≥ width-bits is UB), so the right's wider-than-
                # left width — say `ulong << longlong` — doesn't
                # matter at codegen.
                if isinstance(op, (c99_ast.LeftShift, c99_ast.RightShift)):
                    exp.data_type = tl
                    return tl
                # Usual arithmetic conversions (C99 §6.3.1.8): if
                # operand types differ, promote the narrower one to
                # the common type by wrapping it in an implicit
                # Cast. Both operands now have type `common`, so the
                # underlying op is well-defined at one width.
                common = _common_type(tl, tr)
                exp.left = _convert_to(lhs, common)
                exp.right = _convert_to(rhs, common)
                # Result type: arithmetic / bitwise ops yield the
                # common type; comparison and logical-and/or always
                # yield int regardless of operand type (§6.5.3.3.5 /
                # §6.5.8.6 / §6.5.13.3 / §6.5.14.3).
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
            case c99_ast.CompoundAssignment(op=op, lval=lv, rval=rv):
                # Compound assignment `lval OP= rval`: type-check
                # both sides, then apply the binop type rule
                # (integer promotion, plus the usual arithmetic
                # conversions for non-shifts; shifts skip the
                # arithmetic conversions per §6.5.7.3) to find the
                # *intermediate* type at which the binop happens.
                # The const-qualification check on the lval fires
                # below, after `_check_exp(lv)` stamps lv.data_type.
                # The rval gets cast to that intermediate type so
                # c99_to_tac can read it directly; the lval keeps
                # its own type, and c99_to_tac casts the loaded
                # value to intermediate before the binop and casts
                # the binop result back to the lval's type before
                # the store. The expression's data_type is the
                # lval's type — the result of `lval OP= rval` is
                # the new value of lval, with lval's type.
                tl = self._check_exp(lv)
                tr = self._check_exp(rv)
                if _is_const_qualified(tl):
                    raise TypeCheckError(
                        f"cannot modify const-qualified lvalue with "
                        f"compound assignment: {lv!r} has type {tl!r}"
                    )
                self._require_complete_value(lv, "compound assignment lval")
                self._require_complete_value(rv, "compound assignment rval")
                if isinstance(tl, Array):
                    raise TypeCheckError(
                        f"cannot use compound assignment on an array: "
                        f"{lv!r}"
                    )
                if isinstance(tl, (Structure, Union)):
                    raise TypeCheckError(
                        f"compound assignment is not defined for "
                        f"struct/union types ({tl!r})"
                    )
                # rval array decay (lval can't be an array — caught
                # above). rval might be `arr` decaying to pointer.
                rv = _decay_if_array(rv)
                exp.rval = rv
                tr = rv.data_type
                # Type constraints by op kind (mirror the Binary
                # rule in this same `_check_exp`):
                #   - Modulo / bitwise / shift require integer
                #     operands; reject Float / Double.
                #   - Shifts additionally reject Pointer (the
                #     non-shift bitwise / modulo paths route
                #     through `_common_type` which crashes on
                #     Pointer; shifts skip `_common_type`, so
                #     they need an explicit Pointer reject here).
                if isinstance(op, (
                    c99_ast.Modulo, c99_ast.BitwiseAnd, c99_ast.BitwiseOr,
                    c99_ast.BitwiseXor, c99_ast.LeftShift, c99_ast.RightShift,
                )):
                    if _is_floating_type(tl) or _is_floating_type(tr):
                        raise TypeCheckError(
                            f"compound `{op!r}` requires integer "
                            f"operands ({tl!r}, {tr!r})"
                        )
                    if isinstance(op, (c99_ast.LeftShift, c99_ast.RightShift)):
                        if isinstance(tl, c99_ast.Pointer) or isinstance(
                            tr, c99_ast.Pointer,
                        ):
                            raise TypeCheckError(
                                f"compound shift requires integer "
                                f"operands ({tl!r}, {tr!r})"
                            )
                # Pointer arithmetic: `ptr += int` / `ptr -= int`
                # (C99 §6.5.16.2 — same as `ptr + int` / `ptr - int`).
                # Widen the integer rval to Int (the 2-byte type
                # matching pointer width); lval stays pointer. No
                # intermediate type — c99_to_tac dispatches on the
                # pointer-arith path before consulting it.
                if (
                    isinstance(op, (c99_ast.Add, c99_ast.Subtract))
                    and isinstance(tl, c99_ast.Pointer)
                ):
                    if not _is_integer_type(tr):
                        raise TypeCheckError(
                            f"pointer compound +/- requires integer "
                            f"rhs ({tl!r}, {tr!r})"
                        )
                    exp.rval = _convert_to(rv, c99_ast.Int())
                    exp.intermediate_type = tl
                    exp.data_type = tl
                    return tl
                # Integer promotion on each operand independently.
                tl_p = _promote_integer(tl)
                tr_p = _promote_integer(tr)
                # Shifts: intermediate type = promoted left.
                # The right keeps its independently-promoted type;
                # c99_to_tac's helper-call shift path passes only
                # the right's low byte to the asl/asr/lsr helpers.
                if isinstance(op, (c99_ast.LeftShift, c99_ast.RightShift)):
                    if tr != tr_p:
                        exp.rval = _convert_to(rv, tr_p)
                    exp.intermediate_type = tl_p
                    exp.data_type = tl
                    return tl
                # Other arithmetic / bitwise: usual arithmetic
                # conversions to the common type. The rval is cast
                # to the common type so c99_to_tac reads it at that
                # width directly.
                common = _common_type(tl_p, tr_p)
                exp.rval = _convert_to(rv, common)
                # Result type is the lval's type — the binop result
                # gets converted back to lval's type for the
                # storage write (handled in c99_to_tac via a cast
                # at lowering time).
                exp.intermediate_type = common
                exp.data_type = tl
                return tl
            case c99_ast.Assignment(lval=lv, rval=rv):
                tl = self._check_exp(lv)
                tr = self._check_exp(rv)
                # Reject assignment to a const-qualified lvalue
                # (C99 §6.5.16.1 constraint: "An assignment operator
                # shall have a modifiable lvalue as its left
                # operand"; §6.3.2.1.1 defines a modifiable lvalue as
                # one that does NOT have a const-qualified type).
                # Pointer-assignment qualifier compatibility (e.g.
                # `int *q = (const int *)p;` discarding const at the
                # pointee level) is NOT checked here — c6502 follows
                # gcc's `-Wno-discarded-qualifiers` behavior on that
                # front; the user can use an explicit cast if they
                # mean to discard.
                if _is_const_qualified(tl):
                    raise TypeCheckError(
                        f"cannot assign to const-qualified lvalue: "
                        f"{lv!r} has type {tl!r}"
                    )
                # Both sides materialize the struct's bytes (the lval
                # is overwritten, the rval read), so neither may
                # have incomplete struct/union type.
                self._require_complete_value(lv, "assignment lval")
                self._require_complete_value(rv, "assignment rval")
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
                # Struct / union assignment: legal if the two types
                # have the same tag and is_union flag. Bypass
                # `_convert_to` (which doesn't know about struct
                # types) and the `_decay_if_array` rval path.
                if isinstance(tl, (Structure, Union)):
                    if not _types_equal(tl, tr):
                        raise TypeCheckError(
                            f"struct/union assignment between "
                            f"distinct types: {tl!r} = {tr!r}"
                        )
                    exp.data_type = tl
                    return tl
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
                # already match the lval's. Compound assignments
                # have their own CompoundAssignment AST node and
                # don't reach this branch — they're handled above
                # with their own intermediate-type tracking.
                exp.rval = _convert_to(rv, tl)
                exp.data_type = tl
                return tl
            case c99_ast.Postfix(operand=op) | c99_ast.Prefix(operand=op):
                t = self._check_exp(op)
                # `++` / `--` modify the operand — reject const-
                # qualified lvalues (C99 §6.5.2.4.1 / §6.5.3.1.1
                # require a "modifiable lvalue" operand; §6.3.2.1.1
                # excludes const-qualified types from "modifiable").
                if _is_const_qualified(t):
                    raise TypeCheckError(
                        f"cannot use ++/-- on const-qualified lvalue: "
                        f"{op!r} has type {t!r}"
                    )
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
                self._require_scalar_controlling(
                    cond, "`?:` condition",
                )
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
                # Struct / union: both branches must have matching
                # struct/union type (C99 §6.5.15.6's "both operands
                # have compatible structure or union types"). The
                # type must also be complete — branches of an
                # incomplete struct have no value (you can't copy
                # zero bytes meaningfully). No conversion — the
                # result IS that struct type.
                if _is_struct_or_union(tt) or _is_struct_or_union(tf):
                    if not (_is_struct_or_union(tt)
                            and _is_struct_or_union(tf)
                            and _types_equal(tt, tf)):
                        raise TypeCheckError(
                            f"conditional branches must have matching "
                            f"struct/union type; got {tt!r} vs {tf!r}"
                        )
                    layout = self.types.get(tt.tag)
                    if layout is None or not layout.complete:
                        kw = "union" if isinstance(tt, Union) else "struct"
                        raise TypeCheckError(
                            f"conditional branches have incomplete "
                            f"type '{kw} {tt.tag}'"
                        )
                    exp.data_type = tt
                    return tt
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
            case c99_ast.Comma(left=left, right=right):
                # C99 §6.5.17: evaluate `left` for side effects, then
                # `right`; the result has `right`'s type and value.
                # Type-check both sides independently (no conversion).
                # `left` follows the same rule as an expression
                # statement — its value is discarded, so an incomplete
                # struct/union expression isn't usable here.
                self._check_exp(left)
                self._require_complete_value(
                    left, "left operand of comma operator",
                )
                tr = self._check_exp(right)
                exp.right = _decay_if_array(right)
                exp.data_type = exp.right.data_type
                return exp.data_type
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
                # The function's return type must be complete (or
                # void) at the call site (C99 §6.5.2.2.1: the called
                # function must return "void or a complete object
                # type"). A forward declaration with an incomplete
                # struct/union return is allowed, but actually
                # calling it requires the type to be completed first.
                if isinstance(fn_type.ret, (Structure, Union)):
                    layout = self.types.get(fn_type.ret.tag)
                    if layout is None or not layout.complete:
                        kw = (
                            "union" if isinstance(fn_type.ret, Union)
                            else "struct"
                        )
                        raise TypeCheckError(
                            f"call to {name!r}: incomplete return "
                            f"type '{kw} {fn_type.ret.tag}' "
                            f"(C99 §6.5.2.2.1)"
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
                    # Struct / union args bypass `_convert_to` (which
                    # doesn't model struct conversion). The arg's
                    # type must match the parameter's struct/union
                    # type exactly, and the type must be complete at
                    # the call site (C99 §6.7.2.1.8 — the function
                    # may be forward-declared with an incomplete
                    # type, but its caller can't materialize the
                    # value to pass).
                    if isinstance(expected, (Structure, Union)):
                        kw = "union" if isinstance(expected, Union) else "struct"
                        layout = self.types.get(expected.tag)
                        if layout is None or not layout.complete:
                            raise TypeCheckError(
                                f"argument {i} of {name!r}: cannot pass "
                                f"incomplete type '{kw} {expected.tag}'"
                            )
                        if not _types_equal(arg.data_type, expected):
                            raise TypeCheckError(
                                f"argument {i} of {name!r}: type "
                                f"{arg.data_type!r} doesn't match "
                                f"parameter type {expected!r}"
                            )
                        args[i] = arg
                    else:
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
                # `int * const p; *p` reads the int through p — strip
                # the const-on-the-pointer-itself before unwrapping
                # to the pointee. (Const-on-the-pointee is preserved
                # in the result, making `*p` a const lvalue when p
                # is `const int *`.)
                t_inner_uq = _strip_const(t_inner)
                pointee = t_inner_uq.referenced_type
                # Pointer to incomplete struct/union: dereferencing
                # the value isn't well-defined (no size, no member
                # layout, no addressable storage). The `&*p ≡ p`
                # identity (C99 §6.5.3.2.3) is the one shape where
                # an incomplete pointee is OK; the AddressOf arm
                # below sidesteps this check by translating
                # `&Dereference(p)` directly without recursing into
                # the inner Dereference's type-check.
                if isinstance(pointee, (Structure, Union)):
                    layout = self.types.get(pointee.tag)
                    if layout is None or not layout.complete:
                        kw = "union" if isinstance(pointee, Union) else "struct"
                        display = pointee.tag
                        if display.startswith("@") and "." in display:
                            display = display.split(".", 1)[1]
                        raise TypeCheckError(
                            f"dereference of pointer to incomplete "
                            f"type '{kw} {display}' is not permitted"
                        )
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
                # types must be complete. For struct/union targets
                # the tag must also be complete and in scope.
                _check_well_formed_type(
                    t, where="sizeof type",
                    types=self.types,
                    require_complete=isinstance(t, (Structure, Union, Array)),
                    tag_visible=self._tag_visible,
                auto_introduce=self._auto_introduce_tag,
                )
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
                # `int * const p; p[i]` — the pointer itself is
                # const-qualified, but that doesn't make the element
                # const. Strip the outer Const to get to the
                # `Pointer(...)` shape; the pointee's own Const (if
                # any) rides through into the result.
                ptr_type_uq = _strip_const(ptr_exp.data_type)
                if isinstance(ptr_type_uq.referenced_type, FunType):
                    raise TypeCheckError(
                        "subscript of pointer-to-function is not "
                        "supported"
                    )
                # Widen the index to Int (the 2-byte type matching
                # pointer width) so c99_to_tac can use a uniform
                # 2-byte add to compute the byte address.
                exp.array = ptr_exp
                exp.index = _convert_to(int_exp, Int())
                exp.data_type = ptr_type_uq.referenced_type
                return exp.data_type
            case c99_ast.Dot(operand=operand, member=member):
                # `e.m` per C99 §6.5.2.3.1: operand must have struct
                # or union type; result type is the member's type.
                # §6.5.2.3.3: "If the first expression has qualified
                # type, the result has the so-qualified version of
                # the type of the designated member." So a Const-
                # qualified operand makes the result const-qualified.
                self._check_exp(operand)
                t_op = operand.data_type
                m = self._lookup_member(
                    t_op, member, where=f"member access '.{member}'",
                )
                result_t = _propagate_const(m.type, t_op)
                exp.data_type = result_t
                return result_t
            case c99_ast.Arrow(operand=operand, member=member):
                # `p->m` per C99 §6.5.2.3.2: operand must have
                # pointer-to-struct/union type. Equivalent to
                # `(*p).m`, so const-qualification propagates from
                # the POINTEE (not the pointer) to the member result
                # — `const struct S *p; p->m` makes `p->m` const,
                # but `struct S * const p; p->m` does NOT.
                self._check_exp(operand)
                exp.operand = _decay_if_array(operand)
                operand = exp.operand
                t_op = operand.data_type
                # `int * const p; p->m` — strip the const-on-the-
                # pointer-itself before unwrapping to the pointee.
                t_op_uq = _strip_const(t_op)
                if not isinstance(t_op_uq, Pointer):
                    raise TypeCheckError(
                        f"member access '->{member}': operand has "
                        f"non-pointer type {t_op!r}"
                    )
                pointee = t_op_uq.referenced_type
                m = self._lookup_member(
                    pointee, member,
                    where=f"member access '->{member}'",
                )
                result_t = _propagate_const(m.type, pointee)
                exp.data_type = result_t
                return result_t
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
                # `&*p ≡ p` per C99 §6.5.3.2.3 — type-check the
                # inner pointer expression directly, bypassing the
                # Dereference's incomplete-pointee guard. Without
                # this, `&*p` on a pointer-to-incomplete-struct
                # would wrongly fail. We still stamp the
                # Dereference's data_type so any downstream walker
                # that inspects it sees a self-describing tree.
                if isinstance(inner, c99_ast.Dereference):
                    ptr_type = self._check_exp(inner.exp)
                    if isinstance(ptr_type, Pointer):
                        inner.data_type = ptr_type.referenced_type
                        exp.data_type = ptr_type
                        return ptr_type
                    # Non-pointer operand of `*` — let the regular
                    # Dereference path raise its standard error.
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
) -> tuple[c99_ast.Type_program, SymbolTable, TypeTable]:
    """Type-check a c99 program. Returns the (unchanged) AST plus
    the populated SymbolTable and TypeTable. Raises `TypeCheckError`
    on any type error encountered."""
    return TypeChecker().check_program(prog)
