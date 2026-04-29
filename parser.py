"""Parser for C99 — builds c99_ast nodes from the Lark parse tree.

Adding a grammar rule:
  1. Add/modify the rule in c99.lark.
  2. If the rule has alternatives that map to different AST constructors,
     give each alternative a name with `-> name`:
         statement: RETURN exp SEMICOLON            -> return_stmt
                  | IF LPAREN exp RPAREN statement  -> if_stmt
  3. Add a Transformer method in `_ASTBuilder` with the same name as the
     rule (or the alternative). The method receives the rule's items —
     subtrees already converted to AST nodes, terminals as Lark tokens —
     and returns the AST node for that rule.

@v_args(inline=True) spreads the rule's items into named parameters so
each method's signature mirrors the rule body. Unused items (the
punctuator terminals) are conventionally prefixed with `_`.
"""

from __future__ import annotations

from pathlib import Path

from lark import Lark, Token, Transformer
from lark.exceptions import VisitError
from lark.visitors import v_args

import c99_ast


_GRAMMAR_PATH = Path(__file__).parent / "c99.lark"
_LARK = Lark.open(
    str(_GRAMMAR_PATH),
    parser="lalr",
    lexer="basic",
    # `start` is the real translation-unit entry point; `lex_only`
    # keeps every terminal reachable so the lexer-only tests don't
    # see "unused terminal" trimming. `declarator` and `type_name`
    # are exposed so the §6.7.5 / §6.7.6 grammar can be unit-tested
    # in isolation, since nothing reachable from `start` exercises
    # the named-declarator side yet (var_decl / function_decl still
    # take a bare IDENTIFIER, not a full declarator).
    start=["start", "lex_only", "declarator", "type_name"],
)


class ParserError(Exception):
    """Raised for declaration-specifier constraint violations that the
    LALR grammar accepts but C99 §6.7.2 / §6.7.1 forbids — multiple
    type specifiers, multiple storage-class specifiers, or a missing
    type specifier (C99 dropped the C89 implicit-int rule)."""


# Compound-assignment operator tokens → AST binary-operator class. The
# parser desugars `lval OP= rval` into `lval = lval OP rval`, so each
# compound operator just needs to name the binary op it expands to.
_COMPOUND_ASSIGN_OPS = {
    "PLUS_ASSIGN":    c99_ast.Add,
    "MINUS_ASSIGN":   c99_ast.Subtract,
    "STAR_ASSIGN":    c99_ast.Multiply,
    "SLASH_ASSIGN":   c99_ast.Divide,
    "PERCENT_ASSIGN": c99_ast.Modulo,
    "AMP_ASSIGN":     c99_ast.BitwiseAnd,
    "PIPE_ASSIGN":    c99_ast.BitwiseOr,
    "CARET_ASSIGN":   c99_ast.BitwiseXor,
    "LSHIFT_ASSIGN":  c99_ast.LeftShift,
    "RSHIFT_ASSIGN":  c99_ast.RightShift,
}


# Storage-class specifier token type → AST node class. Used by
# `_split_specifiers` to map the parsed token to the AST node that
# rides on `Type_var_decl.storage_class` / `Type_function_decl.storage_class`.
_STORAGE_CLASSES = {
    "STATIC": c99_ast.Static,
    "EXTERN": c99_ast.Extern,
}

# Token types of every leaf the `specifier` rule can produce. After the
# `type_specifier` and `specifier` transformer methods unwrap their
# child trees, every entry on a specifier list is one of these tokens.
_SPECIFIER_TOKEN_TYPES = ("INT", "LONG", "SIGNED", "UNSIGNED",
                           "FLOAT", "DOUBLE",
                           "STATIC", "EXTERN")
_TYPE_SPECIFIER_TOKEN_TYPES = ("INT", "LONG", "SIGNED", "UNSIGNED",
                                "FLOAT", "DOUBLE")


def _resolve_data_type(type_specs):
    """Map a list of type-specifier tokens to a c99_ast object type.
    Valid combinations follow C99 §6.7.2 (subset c6502 models —
    no `char`, `short`, `_Bool`, or `long long`):
      * `int`, `signed`, `signed int`         → Int
      * `unsigned`, `unsigned int`            → UInt
      * `long`, `long int`,
        `signed long`, `signed long int`      → Long
      * `unsigned long`, `unsigned long int`  → ULong
      * `float`                               → Float
      * `double`                              → Double
      * `long double`                         — rejected (no long
                                                 double type)
      * `long long` (any combination)         — rejected (no long
                                                 long type)
      * `signed unsigned` / `unsigned signed` — rejected
      * `float`/`double` mixed with any of `int` / `long` /
        `signed` / `unsigned`                 — rejected
      * duplicate `int` / `signed` / `unsigned` / `float` /
        `double`                              — rejected
      * empty list                            — rejected
    """
    int_count = sum(1 for t in type_specs if t.type == "INT")
    long_count = sum(1 for t in type_specs if t.type == "LONG")
    signed_count = sum(1 for t in type_specs if t.type == "SIGNED")
    unsigned_count = sum(1 for t in type_specs if t.type == "UNSIGNED")
    float_count = sum(1 for t in type_specs if t.type == "FLOAT")
    double_count = sum(1 for t in type_specs if t.type == "DOUBLE")
    is_fp = float_count > 0 or double_count > 0
    is_integer = (
        int_count > 0 or signed_count > 0 or unsigned_count > 0
    )
    if is_fp and is_integer:
        raise ParserError(
            "floating type cannot combine with 'int' / 'signed' / "
            "'unsigned'"
        )
    if float_count > 1:
        raise ParserError("'float' specified more than once")
    if double_count > 1:
        raise ParserError("'double' specified more than once")
    if float_count == 1 and double_count == 1:
        raise ParserError(
            "'float' and 'double' cannot both appear in a declaration"
        )
    if double_count == 1 and long_count >= 1:
        raise ParserError(
            "'long double' is not supported (only 'float' and "
            "'double' are modeled today)"
        )
    if float_count == 1 and long_count >= 1:
        raise ParserError(
            "'long float' is not a valid type"
        )
    if signed_count > 0 and unsigned_count > 0:
        raise ParserError(
            "'signed' and 'unsigned' cannot both appear in a "
            "declaration"
        )
    if signed_count > 1:
        raise ParserError("'signed' specified more than once")
    if unsigned_count > 1:
        raise ParserError("'unsigned' specified more than once")
    if not is_fp and (int_count == 0 and long_count == 0
            and signed_count == 0 and unsigned_count == 0):
        raise ParserError("missing type specifier")
    if int_count > 1:
        raise ParserError(
            "multiple type specifiers in a declaration "
            "(at most one 'int' is permitted)"
        )
    if long_count > 1:
        raise ParserError(
            "'long long' is not supported (only 'int' and 'long' are "
            "modeled today)"
        )
    if float_count == 1:
        return c99_ast.Float()
    if double_count == 1:
        return c99_ast.Double()
    is_unsigned = unsigned_count == 1
    is_long = long_count == 1
    if is_long and is_unsigned:
        return c99_ast.ULong()
    if is_long:
        return c99_ast.Long()
    if is_unsigned:
        return c99_ast.UInt()
    return c99_ast.Int()


def _split_specifiers(specs):
    """Validate a `specifier+` token list and split it into
    `(data_type, storage_class)`.

    The grammar rule `specifier: type_specifier | STATIC | EXTERN`
    accepts any interleaving of type and storage-class specifiers in a
    declaration. C99 §6.7.1.2 / §6.7.2 are stricter:
      * exactly one type (composed from `INT` and `LONG` per
        `_resolve_data_type`)
      * at most one storage-class specifier

    Returns `(data_type, storage_class)` where data_type is a
    c99_ast.Int() or .Long() (or .FunType for the future) and
    storage_class is a c99_ast.Static() / .Extern() / None.
    """
    type_specs = []
    storage = None
    for spec in specs:
        if spec.type in _TYPE_SPECIFIER_TOKEN_TYPES:
            type_specs.append(spec)
        else:
            cls = _STORAGE_CLASSES[spec.type]
            if storage is not None:
                raise ParserError(
                    "at most one storage-class specifier permitted "
                    "in a declaration"
                )
            storage = cls()
    return _resolve_data_type(type_specs), storage


def _consume_specifiers(items, start):
    """Pull the leading specifier tokens off `items`. Returns
    `(specs, idx)` where `idx` is the position of the first non-
    specifier item (the IDENTIFIER for both var_decl and function_decl,
    by grammar)."""
    specs = []
    i = start
    while (
        i < len(items)
        and hasattr(items[i], "type")
        and items[i].type in _SPECIFIER_TOKEN_TYPES
    ):
        specs.append(items[i])
        i += 1
    return specs, i


# Integer constant typing per C99 §6.4.4.1 paragraph 5: "the type of
# an integer constant is the first of the corresponding list in which
# its value can be represented." c6502 models four integer types —
# int / long / unsigned int / unsigned long — corresponding 1:1 with
# the four c99_ast `const` variants. There is no `long long`, so any
# literal whose only fitting type would be `long long` (or
# `unsigned long long`) is rejected.
#
# Type ranges (literals are non-negative — unary minus comes from an
# operator, applied later):
#     int            0..127     ConstInt
#     long           0..32767   ConstLong
#     unsigned int   0..255     ConstUInt
#     unsigned long  0..65535   ConstULong
_INT_MAX = 127
_LONG_MAX = 32767
_UINT_MAX = 255
_ULONG_MAX = 65535

# Per-(token-kind, base) candidate-type list. Each entry is a tuple of
# (max_value, c99_ast Const class) — pick the first whose max accepts
# the literal's value. Bases: "decimal" for plain decimal literals,
# "hex_oct" for hex (0x...) and octal (0...) literals. The C99 table
# distinguishes the two for the unsuffixed and L-only suffix cases;
# the U-only and U+L suffix cases share one list across bases.
_INT = c99_ast.ConstInt
_LONG = c99_ast.ConstLong
_UINT = c99_ast.ConstUInt
_ULONG = c99_ast.ConstULong

_CANDIDATES = {
    ("INTEGER_CONSTANT", "decimal"): [(_INT_MAX, _INT), (_LONG_MAX, _LONG)],
    ("INTEGER_CONSTANT", "hex_oct"): [
        (_INT_MAX, _INT), (_UINT_MAX, _UINT),
        (_LONG_MAX, _LONG), (_ULONG_MAX, _ULONG),
    ],
    ("LONG_INTEGER",    "decimal"): [(_LONG_MAX, _LONG)],
    ("LONG_INTEGER",    "hex_oct"): [(_LONG_MAX, _LONG), (_ULONG_MAX, _ULONG)],
    ("UINT_INTEGER",    "decimal"): [(_UINT_MAX, _UINT), (_ULONG_MAX, _ULONG)],
    ("UINT_INTEGER",    "hex_oct"): [(_UINT_MAX, _UINT), (_ULONG_MAX, _ULONG)],
    ("ULONG_INTEGER",   "decimal"): [(_ULONG_MAX, _ULONG)],
    ("ULONG_INTEGER",   "hex_oct"): [(_ULONG_MAX, _ULONG)],
}


def _parse_integer_token(text):
    """Strip the C99 integer-constant suffix off `text` and return
    `(value, base, has_ll)`:
      * `value` — the non-negative integer (literals are always
        non-negative; negation is a separate unary-minus operator).
      * `base` — `"hex_oct"` for hex (`0x...`) and octal (`0...`)
        literals, `"decimal"` otherwise. The two bases share suffix
        rules but differ in their unsuffixed / L-suffixed type lists
        (C99 §6.4.4.1 table).
      * `has_ll` — True if the suffix contains `LL` or `ll`. c6502
        doesn't model `long long`, so callers reject these.
    """
    i = len(text)
    while i > 0 and text[i - 1] in "uUlL":
        i -= 1
    digits, suffix = text[:i], text[i:]
    has_ll = "LL" in suffix or "ll" in suffix
    if digits.startswith(("0x", "0X")):
        base = "hex_oct"
    elif len(digits) > 1 and digits.startswith("0"):
        base = "hex_oct"
    else:
        base = "decimal"
    return int(digits, 0), base, has_ll


def _const_for_token(token):
    """Map a Lark constant token to a c99_ast `Type_const` node.
    Integer literals follow C99 §6.4.4.1 paragraph 5 (first variant
    in the per-(suffix, base) candidate list whose range fits the
    value); floating literals follow C99 §6.4.4.2 (suffix uniquely
    determines the type — no value-fitting rule)."""
    text = str(token)
    if token.type in ("DOUBLE_CONSTANT", "FLOAT_CONSTANT"):
        # Strip a trailing `f`/`F` before parsing — Python's
        # `float()` doesn't recognise C suffixes.
        body = text[:-1] if token.type == "FLOAT_CONSTANT" else text
        if body.startswith(("0x", "0X")):
            # Hex floats (`0x1.0p3`) lex but Python's `float()`
            # can't parse them; rejected here until we wire up a
            # manual conversion.
            raise ParserError(
                f"hex floating literal {text!r} is not supported"
            )
        cls = (
            c99_ast.ConstDouble if token.type == "DOUBLE_CONSTANT"
            else c99_ast.ConstFloat
        )
        return cls(float=float(body))
    if token.type == "LONG_DOUBLE_CONSTANT":
        raise ParserError(
            f"`long double` is not supported (literal {text!r})"
        )
    value, base, has_ll = _parse_integer_token(text)
    if has_ll:
        raise ParserError(
            f"`long long` is not supported (literal {text!r})"
        )
    for max_value, cls in _CANDIDATES[(token.type, base)]:
        if value <= max_value:
            return cls(int=value)
    raise ParserError(
        f"integer constant {text!r} doesn't fit any supported type "
        f"(c6502 has no `long long` / `unsigned long long`)"
    )


def _make_int_const(value):
    """Factory for synthetic non-negative literals (e.g. the `1`
    minted by prefix `++a` desugaring). Picks the smallest
    candidate from the unsuffixed-decimal type list — same rule the
    parser applies to `1` written in source."""
    for max_value, cls in _CANDIDATES[("INTEGER_CONSTANT", "decimal")]:
        if value <= max_value:
            return cls(int=value)
    raise ParserError(
        f"synthetic integer constant {value} out of range "
        f"(c6502 has no `long long`)"
    )


# ---------------------------------------------------------------------------
# Declarator walking
# ---------------------------------------------------------------------------
#
# C99 declarators (§6.7.5) wrap a base type with a chain of modifiers
# — pointers, array bounds, function signatures — and name an
# identifier in the middle. The `_apply_declarator` helper walks a
# raw `declarator` parse Tree from the OUTER form inward, threading
# the base type and accumulating wrappers per the §6.7.5 precedence
# rule: postfix array / function suffixes bind tighter than a prefix
# `*`, so `int *foo(int)` is "function returning int*" not "pointer
# to function returning int".
#
# We don't add Lark transformer methods for `declarator` /
# `direct_declarator` / `pointer` — the walk needs the structural
# info (recursive nesting, suffix vs. prefix) and a single helper
# that owns the algorithm is clearer than splitting it across
# transformer methods.

def _is_token(x, ttype: str) -> bool:
    return hasattr(x, "type") and x.type == ttype


def _apply_declarator(decl_tree, base_type):
    """Walk a `declarator: pointer? direct_declarator` parse tree.

    Returns `(name, composed_type, outer_param_names)`:
      name              — identifier the declarator names.
      composed_type     — full c99_ast type for that name.
      outer_param_names — list[str | None] of parameter names from
                          the OUTERMOST direct_declarator's function
                          suffix (or None if the declarator has no
                          such suffix). Caller checks composed_type
                          to decide whether these are real function
                          params: `composed_type is FunType` →
                          they're the function's params; anything
                          else (e.g. `Pointer(FunType)` for a
                          function-pointer variable) → noise that
                          belongs to the pointee, not the variable
                          itself.
    """
    children = decl_tree.children
    if len(children) == 2:
        # `pointer direct_declarator` — pointer wraps the base type
        # before the inner direct_declarator threads its suffixes
        # through. Per §6.7.5 the pointer modifier applies AFTER
        # the postfix suffixes (the `*` is "outer" lexically but
        # "inner" in the type composition).
        pointer_tree, direct_tree = children
        base_type = _wrap_pointers(pointer_tree, base_type)
    else:
        # `direct_declarator` — pointer? matched empty.
        (direct_tree,) = children
    return _apply_direct_declarator(direct_tree, base_type)


def _wrap_pointers(pointer_tree, base_type):
    """`pointer: STAR type_qualifier* pointer?`. Walks the optional
    nested `pointer` chain, wrapping `base_type` once per `*`. Type
    qualifiers (`const` / `restrict` / `volatile`) accepted by the
    grammar are dropped — c6502 doesn't model qualifier semantics."""
    depth = 0
    cur = pointer_tree
    while True:
        depth += 1
        inner = None
        for c in cur.children:
            if hasattr(c, "data") and c.data == "pointer":
                inner = c
                break
        if inner is None:
            break
        cur = inner
    for _ in range(depth):
        base_type = c99_ast.Pointer(referenced_type=base_type)
    return base_type


def _apply_direct_declarator(dd_tree, base_type):
    """Walk a `direct_declarator` Tree. Returns
    `(name, composed_type, outer_param_names)` — same shape as
    `_apply_declarator`. The outer-vs-inner distinction follows
    Lark's parse tree: the OUTERMOST direct_declarator is the FIRST
    one we recurse into (its suffix has already been parsed at
    parse time), and inner direct_declarators are deeper recursions."""
    children = dd_tree.children
    # 1) IDENTIFIER alone — base case.
    if len(children) == 1 and _is_token(children[0], "IDENTIFIER"):
        return str(children[0]), base_type, None
    # 2) `LPAREN declarator RPAREN` — recurse into the inner
    #    declarator with the same base_type.
    if (
        len(children) == 3
        and _is_token(children[0], "LPAREN")
        and _is_token(children[2], "RPAREN")
        and hasattr(children[1], "data")
        and children[1].data == "declarator"
    ):
        return _apply_declarator(children[1], base_type)
    inner_dd = children[0]
    # 3) Array suffix — `direct_declarator LBRACKET array_suffix RBRACKET`.
    #    Walked outer-first: the outermost suffix in source order is
    #    the rightmost `[N]`, and `int a[3][4]` ends up as
    #    Array(Array(Int, 4), 3) (a is array of 3 arrays of 4 ints).
    #    The grammar accepts four `array_suffix` shapes (plain /
    #    static / qualifiers-then-static / unspecified-VLA), but
    #    c6502 only supports the plain form with a constant integer
    #    size — no `[]`, no `static`, no qualifiers, no VLA `*`.
    if (
        len(children) == 4
        and _is_token(children[1], "LBRACKET")
        and _is_token(children[3], "RBRACKET")
    ):
        suffix = children[2]
        if not hasattr(suffix, "data") or suffix.data != "array_size_plain":
            raise NotImplementedError(
                "only plain array sizes are supported "
                f"(got {getattr(suffix, 'data', suffix)!r})"
            )
        if len(suffix.children) == 0:
            raise NotImplementedError(
                "array of unspecified size (`[]`) is not supported"
            )
        if len(suffix.children) > 1:
            raise NotImplementedError(
                "type-qualified array sizes aren't supported"
            )
        size_exp = suffix.children[0]
        if not isinstance(size_exp, c99_ast.Constant):
            raise NotImplementedError(
                "array size must be an integer constant literal"
            )
        c = size_exp.const
        if not isinstance(c, (
            c99_ast.ConstInt, c99_ast.ConstLong,
            c99_ast.ConstUInt, c99_ast.ConstULong,
        )):
            raise NotImplementedError(
                "array size must be an integer constant"
            )
        if c.int <= 0:
            raise ParserError("array size must be positive")
        # Wrap base_type in an Array; recurse with the wrapped type
        # so any further suffixes (deeper in inner_dd) compose on top.
        new_base = c99_ast.Array(element_type=base_type, size=c.int)
        return _apply_direct_declarator(inner_dd, new_base)
    # 4) Function suffix — `inner_dd LPAREN body? RPAREN` where body
    #    is a parameter_type_list, VOID, an identifier_list, or
    #    empty (K&R `f()`).
    middle = children[2:-1]  # drop LPAREN at [1] and RPAREN at [-1]
    if not middle:
        # `f()` — K&R-style empty list. Treat as no params.
        param_pairs = []
    elif len(middle) == 1 and _is_token(middle[0], "VOID"):
        # `(void)` — explicit empty params.
        param_pairs = []
    elif len(middle) == 1 and isinstance(middle[0], list):
        # parameter_type_list transformer returns a list of
        # (name_or_None, type) tuples — by the time this helper
        # runs, Lark has already transformed the subtree.
        param_pairs = middle[0]
    elif len(middle) == 1 and hasattr(middle[0], "data"):
        if middle[0].data == "identifier_list":
            raise NotImplementedError(
                "K&R-style identifier-list functions aren't supported"
            )
        raise AssertionError(
            f"unexpected direct_declarator function-form child: {middle[0]!r}"
        )
    else:
        raise AssertionError(
            f"unexpected direct_declarator function-form children: {children!r}"
        )
    # C99 §6.7.5.3.1: a function declarator shall not specify a
    # return type that is a function type or an array type. Catch
    # this at composition time so e.g. `int foo(void)(void)` and
    # `int foo(void)[3]` are rejected at parse rather than producing
    # an unrepresentable FunType(ret=FunType/Array).
    _check_function_return_type(base_type)
    param_types = [t for (_n, t) in param_pairs]
    new_base = c99_ast.FunType(params=param_types, ret=base_type)
    name, composed, _inner_outer_params = _apply_direct_declarator(
        inner_dd, new_base,
    )
    # Our suffix is the OUTERMOST one for this declarator — propagate
    # our own param names up. (Inner outer-params, if any, are from
    # a deeper recursion and represent inner function-types — they
    # belong to the type, not to this declarator's name.)
    param_names = [n for (n, _t) in param_pairs]
    return name, composed, param_names


def _check_function_return_type(ret_type):
    """C99 §6.7.5.3.1: a function declarator's return type can't be
    a function type or an array type. Used by both
    `_apply_direct_declarator` and `_apply_direct_abstract_declarator`
    just before they wrap a return type in a FunType."""
    if isinstance(ret_type, c99_ast.FunType):
        raise ParserError(
            "function declarator cannot specify a function-typed "
            "return (C99 §6.7.5.3.1)"
        )
    if isinstance(ret_type, c99_ast.Array):
        raise ParserError(
            "function declarator cannot specify an array return type "
            "(C99 §6.7.5.3.1)"
        )


def _adjust_param_type(t):
    """C99 §6.7.5.3.7 array-to-pointer adjustment for parameters:
    "A declaration of a parameter as 'array of type' shall be
    adjusted to 'qualified pointer to type'." So `int foo(int a[3])`
    is exactly equivalent to `int foo(int *a)`. Only the OUTERMOST
    array suffix decays — multi-dim params like `int a[3][4]` adjust
    to `int (*a)[4]` (pointer to inner array), not `int **a`. The
    adjustment happens at parse time so the FunType the type checker
    builds, the function's symbol-table entry, and every Var
    reference to the parameter all see the adjusted (pointer) type
    uniformly.

    C99 also adjusts function parameters of function type to
    "pointer to function" (§6.7.5.3.8), but the c6502 grammar only
    accepts named parameters via the `declarator` form which can't
    produce a bare FunType for a parameter (a function-typed
    declarator always wraps the FunType in a Pointer or names a
    function being declared, not a parameter), so that adjustment
    isn't needed here."""
    if isinstance(t, c99_ast.Array):
        return c99_ast.Pointer(referenced_type=t.element_type)
    return t


def _array_size_from_suffix(suffix):
    """Decode an `array_suffix` parse tree into a positive int size.
    Used by both the named (`int a[3]`) and abstract (`int (*)[3]`)
    declarator paths."""
    if not hasattr(suffix, "data") or suffix.data != "array_size_plain":
        raise NotImplementedError(
            "only plain array sizes are supported "
            f"(got {getattr(suffix, 'data', suffix)!r})"
        )
    if len(suffix.children) == 0:
        raise NotImplementedError(
            "array of unspecified size (`[]`) is not supported"
        )
    if len(suffix.children) > 1:
        raise NotImplementedError(
            "type-qualified array sizes aren't supported"
        )
    size_exp = suffix.children[0]
    if not isinstance(size_exp, c99_ast.Constant):
        raise NotImplementedError(
            "array size must be an integer constant literal"
        )
    c = size_exp.const
    if not isinstance(c, (
        c99_ast.ConstInt, c99_ast.ConstLong,
        c99_ast.ConstUInt, c99_ast.ConstULong,
    )):
        raise NotImplementedError(
            "array size must be an integer constant"
        )
    if c.int <= 0:
        raise ParserError("array size must be positive")
    return c.int


def _apply_abstract_declarator(adecl_tree, base_type):
    """Walk an `abstract_declarator` parse tree (used for type-names
    and unnamed parameter declarations) and return the composed
    c99_ast type. The grammar is `pointer | pointer?
    direct_abstract_declarator`. Same composition rule as
    `_apply_declarator`: the pointer prefix wraps the base type FIRST
    (so it ends up as the INNERMOST wrapper), then the direct
    abstract declarator's suffixes wrap around it. So `int *[3]`
    becomes `Array(Pointer(Int), 3)` — array of 3 pointers — while
    `int (*)[3]` becomes `Pointer(Array(Int, 3))` — pointer to array.
    """
    children = adecl_tree.children
    # `pointer` alone — no inner direct_abstract_declarator.
    if (
        len(children) == 1
        and hasattr(children[0], "data")
        and children[0].data == "pointer"
    ):
        return _wrap_pointers(children[0], base_type)
    # `pointer direct_abstract_declarator` — pointer wraps base FIRST,
    # then the dad applies its suffixes around it.
    if (
        len(children) == 2
        and hasattr(children[0], "data")
        and children[0].data == "pointer"
    ):
        base_type = _wrap_pointers(children[0], base_type)
        return _apply_direct_abstract_declarator(children[1], base_type)
    # `direct_abstract_declarator` alone (no leading pointer).
    if (
        len(children) == 1
        and hasattr(children[0], "data")
        and children[0].data == "direct_abstract_declarator"
    ):
        return _apply_direct_abstract_declarator(children[0], base_type)
    raise AssertionError(
        f"unexpected abstract_declarator children: {children!r}"
    )


def _apply_direct_abstract_declarator(dad_tree, base_type):
    """Walk a `direct_abstract_declarator`, applying the OUTERMOST
    suffix in source order first (it's the rightmost child here) and
    threading the composed type through the recursion. Mirrors
    `_apply_direct_declarator` but with no IDENTIFIER base case — the
    leaves are bare suffix forms (`[N]`, `(params)`) or a parenthesised
    inner abstract_declarator."""
    children = dad_tree.children
    # Leaf 1: `LPAREN abstract_declarator RPAREN` — recurse into the
    # inner abstract_declarator with the same base_type (parens just
    # group; no suffix here).
    if (
        len(children) == 3
        and _is_token(children[0], "LPAREN")
        and _is_token(children[2], "RPAREN")
        and hasattr(children[1], "data")
        and children[1].data == "abstract_declarator"
    ):
        return _apply_abstract_declarator(children[1], base_type)
    # Leaf 2: `LBRACKET array_suffix RBRACKET` — bare array suffix.
    if (
        len(children) == 3
        and _is_token(children[0], "LBRACKET")
        and _is_token(children[2], "RBRACKET")
    ):
        size = _array_size_from_suffix(children[1])
        return c99_ast.Array(element_type=base_type, size=size)
    # Leaf 3: `LPAREN parameter_type_list? RPAREN` / `LPAREN VOID
    # RPAREN` — bare function suffix.
    if (
        len(children) >= 2
        and _is_token(children[0], "LPAREN")
        and _is_token(children[-1], "RPAREN")
    ):
        _check_function_return_type(base_type)
        param_pairs = _parse_function_suffix_middle(children[1:-1])
        param_types = [t for (_n, t) in param_pairs]
        return c99_ast.FunType(params=param_types, ret=base_type)
    # Recursive forms — first child is an inner direct_abstract_declarator,
    # followed by a postfix suffix.
    if (
        len(children) >= 3
        and hasattr(children[0], "data")
        and children[0].data == "direct_abstract_declarator"
    ):
        inner_dad = children[0]
        # Recursive array suffix: `inner LBRACKET array_suffix RBRACKET`.
        if (
            len(children) == 4
            and _is_token(children[1], "LBRACKET")
            and _is_token(children[3], "RBRACKET")
        ):
            size = _array_size_from_suffix(children[2])
            new_base = c99_ast.Array(element_type=base_type, size=size)
            return _apply_direct_abstract_declarator(inner_dad, new_base)
        # Recursive function suffix: `inner LPAREN body? RPAREN`.
        if (
            _is_token(children[1], "LPAREN")
            and _is_token(children[-1], "RPAREN")
        ):
            _check_function_return_type(base_type)
            param_pairs = _parse_function_suffix_middle(children[2:-1])
            param_types = [t for (_n, t) in param_pairs]
            new_base = c99_ast.FunType(params=param_types, ret=base_type)
            return _apply_direct_abstract_declarator(inner_dad, new_base)
    raise AssertionError(
        f"unexpected direct_abstract_declarator children: {children!r}"
    )


def _parse_function_suffix_middle(middle):
    """Decode the middle children of a function-suffix declarator
    (between LPAREN and RPAREN) into a list of `(name_or_None, type)`
    pairs. Shared between the named and abstract walkers."""
    if not middle:
        # `f()` — K&R-style empty list. Treat as no params.
        return []
    if len(middle) == 1 and _is_token(middle[0], "VOID"):
        return []
    if len(middle) == 1 and isinstance(middle[0], list):
        # parameter_type_list transformer returns a list of
        # (name_or_None, type) tuples.
        return middle[0]
    if len(middle) == 1 and hasattr(middle[0], "data"):
        if middle[0].data == "identifier_list":
            raise NotImplementedError(
                "K&R-style identifier-list functions aren't supported"
            )
    raise AssertionError(
        f"unexpected function-suffix middle: {middle!r}"
    )


class _ASTBuilder(Transformer):
    def start(self, items):
        # `start: declaration*` — items is the list of Type_declaration
        # nodes already built by `declaration` (each wrapping a
        # function_decl or var_decl).
        return c99_ast.Program(declaration=list(items))

    @v_args(inline=True)
    def specifier(self, token):
        # `specifier: type_specifier | STATIC | EXTERN`. Either branch
        # contributes a single token (after `type_specifier` unwraps
        # its tree), so this method just passes the token through for
        # var_decl / function_decl to scan.
        return token

    @v_args(inline=True)
    def type_specifier(self, token):
        # `type_specifier: INT | LONG`. Inline the single child so the
        # caller (`specifier` or `type_name`) sees a Token directly.
        return token

    def type_name(self, items):
        # `type_name: type_specifier+ abstract_declarator?` — used
        # inside cast expressions.
        #
        # type_specifier tokens land first (the `type_specifier`
        # transformer passes INT / LONG / SIGNED / etc. tokens
        # through). If an abstract_declarator subtree is present,
        # it's the last item — `_apply_abstract_declarator` composes
        # it into the full type (pointers, arrays, function suffixes).
        # We then reject any cast target whose composed type isn't
        # a scalar: per C99 §6.5.4.2 a cast target must be qualified
        # or unqualified scalar type (or void). Array and function
        # types are aggregates / function types respectively, neither
        # scalar. Pointer-to-array and pointer-to-function are fine
        # (`int (*)[3]` / `int (*)(void)`) — the outer wrapper is a
        # Pointer, which IS scalar.
        type_specs = [it for it in items if isinstance(it, Token)]
        base = _resolve_data_type(type_specs)
        if len(type_specs) == len(items):
            # No abstract_declarator — bare type-specifier list.
            return base
        # Trailing abstract_declarator subtree.
        adecl_tree = items[-1]
        composed = _apply_abstract_declarator(adecl_tree, base)
        if isinstance(composed, c99_ast.Array):
            raise ParserError(
                "cannot cast to an array type (C99 §6.5.4.2 — cast "
                "target must be scalar or void)"
            )
        if isinstance(composed, c99_ast.FunType):
            raise ParserError(
                "cannot cast to a function type (C99 §6.5.4.2 — cast "
                "target must be scalar or void)"
            )
        return composed

    def parameter_declaration(self, items):
        # `parameter_declaration: specifier+ declarator
        #                       | specifier+ abstract_declarator?`
        # Returns `(name_or_None, data_type)`. An unnamed parameter
        # — `int foo(int *)` — gives `name=None`. An empty
        # abstract_declarator (`int foo(int)` would give the inner
        # `int` here) gives `name=None` and the bare specifier type.
        specs = []
        rest = []
        for it in items:
            if (
                hasattr(it, "type")
                and it.type in _SPECIFIER_TOKEN_TYPES
            ):
                specs.append(it)
            else:
                rest.append(it)
        base, storage = _split_specifiers(specs)
        # C99 §6.7.5.3.2: "The only storage-class specifier that shall
        # occur in a parameter declaration is register." c6502 doesn't
        # model `register` (the keyword lexes but no parser rule
        # accepts it), so any storage class reaching here is `static`
        # or `extern` and is a constraint violation.
        if storage is not None:
            raise ParserError(
                f"storage class "
                f"{type(storage).__name__.lower()!r} isn't permitted "
                f"on a function parameter (C99 §6.7.5.3.2)"
            )
        if not rest:
            return (None, base)
        decl_tree = rest[0]
        if decl_tree.data == "declarator":
            name, composed, _outer = _apply_declarator(decl_tree, base)
            return (name, _adjust_param_type(composed))
        if decl_tree.data == "abstract_declarator":
            t = _apply_abstract_declarator(decl_tree, base)
            return (None, _adjust_param_type(t))
        raise AssertionError(
            f"unexpected parameter_declaration child: {decl_tree.data}"
        )

    def parameter_or_ellipsis(self, items):
        # `parameter_or_ellipsis: parameter_declaration | ELLIPSIS`.
        # parameter_declaration has already been transformed into a
        # tuple; ELLIPSIS comes through as a Token. Reject ELLIPSIS
        # for now — c6502 has no variadic AST node.
        item = items[0]
        if hasattr(item, "type") and item.type == "ELLIPSIS":
            raise ParserError(
                "variadic functions ('...') aren't supported yet"
            )
        return item

    def parameter_type_list(self, items):
        # `parameter_type_list: parameter_declaration
        #                       (COMMA parameter_or_ellipsis)*`
        # — items alternate: tuple, COMMA, tuple, COMMA, ... .
        # Strip COMMAs and return the list of (name_or_None, type)
        # tuples. The transformer for parameter_or_ellipsis already
        # rejected ELLIPSIS, so by here every entry is a real param.
        return [it for it in items if not _is_token(it, "COMMA")]

    def block(self, items):
        # `block: LBRACE block_item* RBRACE`. Non-inline because
        # block_item* expands to a variable number of children.
        return c99_ast.Block(block_item=list(items[1:-1]))

    # Alternatives of `block_item` — wrap a statement / declaration.
    @v_args(inline=True)
    def stmt_item(self, statement):
        return c99_ast.S(statement=statement)

    @v_args(inline=True)
    def decl_item(self, declaration):
        # C99 §6.9.1: function-definitions are an external-declaration
        # form, only legal at file scope. A `block_item` may carry a
        # function *forward declaration* (FunctionDecl with body=None)
        # but never a definition (body present). The grammar allows
        # both — `block_item: declaration` and `declaration:
        # function_decl | var_decl`, where `function_decl` is the
        # body-bearing form — so reject the definition shape here.
        if (
            isinstance(declaration, c99_ast.FunctionDecl)
            and declaration.function_decl.body is not None
        ):
            raise ParserError(
                f"function definition for "
                f"{declaration.function_decl.name!r} is only legal at "
                f"file scope; nested function definitions aren't part "
                f"of standard C (C99 §6.9.1)"
            )
        return c99_ast.D(declaration=declaration)

    @v_args(inline=True)
    def declaration(self, child):
        # `declaration: function_decl | var_decl`. Each branch
        # returns its product type (`Type_function_decl` or
        # `Type_var_decl`); wrap into the matching declaration sum
        # constructor here.
        if isinstance(child, c99_ast.Type_var_decl):
            return c99_ast.VarDecl(var_decl=child)
        return c99_ast.FunctionDecl(function_decl=child)

    # `initializer` rule alternatives. `init_exp` is `assignment_exp`
    # (a single scalar initializer); the inner exp passes through
    # unchanged. `init_list` is `{ initializer (, initializer)* ,? }`
    # — strip the LBRACE / COMMA / RBRACE tokens and wrap the
    # remaining items in InitList. Items are themselves initializer
    # results (an exp or another InitList), supporting nested init
    # for future multi-dim use.
    @v_args(inline=True)
    def init_exp(self, exp):
        return exp

    def init_list(self, items):
        children = [
            it for it in items
            if not (
                hasattr(it, "type")
                and it.type in ("LBRACE", "RBRACE", "COMMA")
            )
        ]
        return c99_ast.InitList(items=children)

    def var_decl(self, items):
        # `var_decl: specifier+ declarator (ASSIGN exp)? SEMICOLON`.
        # Layout: <specs...> declarator [ASSIGN exp] SEMICOLON. The
        # specifiers give the BASE type; the declarator wraps it
        # with pointer / function modifiers and names the identifier.
        # If the resulting composed_type is FunType, this is actually
        # a forward function declaration (`int foo(int);` parses as
        # var_decl with a function-typed declarator) — rewrap as
        # Type_function_decl with body=None.
        specs, i = _consume_specifiers(items, 0)
        base_type, storage_class = _split_specifiers(specs)
        decl_tree = items[i]
        name, composed, outer_param_names = _apply_declarator(
            decl_tree, base_type,
        )
        i += 1
        init = None
        if i < len(items) and _is_token(items[i], "ASSIGN"):
            init = items[i + 1]
        if isinstance(composed, c99_ast.FunType):
            if init is not None:
                raise ParserError(
                    "function declaration cannot have an initializer"
                )
            return c99_ast.Type_function_decl(
                name=name,
                params=outer_param_names or [],
                body=None,
                data_type=composed,
                storage_class=storage_class,
            )
        return c99_ast.Type_var_decl(
            name=name,
            init=init,
            data_type=composed,
            storage_class=storage_class,
        )

    def function_decl(self, items):
        # `function_decl: specifier+ declarator block` — function
        # *definition* path (body present). Forward declarations land
        # in `var_decl` above (the grammar lets `int foo(int);` parse
        # as var_decl with a function-typed declarator, since both
        # rules share the `specifier+ declarator` prefix and the
        # trailing `(ASSIGN exp)? SEMICOLON` matches the no-init,
        # no-body case).
        #
        # The declarator must compose to a FunType — its outermost
        # direct_declarator form has to be the function-suffix shape.
        # Anything else (a plain identifier, a function-pointer
        # variable, ...) means the source isn't a function definition.
        specs, i = _consume_specifiers(items, 0)
        base_type, storage_class = _split_specifiers(specs)
        decl_tree = items[i]
        name, composed, outer_param_names = _apply_declarator(
            decl_tree, base_type,
        )
        block = items[i + 1]
        if not isinstance(composed, c99_ast.FunType):
            raise ParserError(
                f"function definition's declarator must compose to a "
                f"function type; got {type(composed).__name__}"
            )
        return c99_ast.Type_function_decl(
            name=name,
            params=outer_param_names or [],
            body=block,
            data_type=composed,
            storage_class=storage_class,
        )

    # Alternatives of `statement` — each named in c99.lark.
    @v_args(inline=True)
    def return_stmt(self, _return, exp, _semi):
        return c99_ast.Return(exp=exp)

    @v_args(inline=True)
    def expression_stmt(self, exp, _semi):
        return c99_ast.Expression(exp=exp)

    # `if (exp) stmt` (4 children) or `if (exp) stmt else stmt` (6
    # children). The else-branch is variable, so non-inline.
    def if_stmt(self, items):
        condition = items[2]
        then_clause = items[4]
        else_clause = items[6] if len(items) == 7 else None
        return c99_ast.IfStmt(
            condition=condition,
            then_clause=then_clause,
            else_clause=else_clause,
        )

    @v_args(inline=True)
    def goto_stmt(self, _goto, identifier, _semi):
        return c99_ast.Goto(label=str(identifier))

    @v_args(inline=True)
    def labeled_stmt(self, identifier, _colon, stmt):
        return c99_ast.LabeledStmt(label=str(identifier), statement=stmt)

    @v_args(inline=True)
    def compound_stmt(self, block):
        # `{ ... }` as a statement. `block` has already been built
        # into a `Block` by the `block` transformer; wrap it in a
        # `Compound` so it fits the `statement` sum.
        return c99_ast.Compound(block=block)

    @v_args(inline=True)
    def null_stmt(self, _semi):
        return c99_ast.Null()

    # Loop and jump statements. Loop labels are minted by the
    # loop_labeling pass that runs after identifier_resolution; the
    # parser leaves them as empty strings.
    @v_args(inline=True)
    def break_stmt(self, _break, _semi):
        return c99_ast.BreakStmt(label="")

    @v_args(inline=True)
    def continue_stmt(self, _continue, _semi):
        return c99_ast.ContinueStmt(label="")

    @v_args(inline=True)
    def while_stmt(self, _while, _lp, cond, _rp, body):
        return c99_ast.WhileStmt(condition=cond, body=body, label="")

    @v_args(inline=True)
    def do_stmt(self, _do, body, _while, _lp, cond, _rp, _semi):
        return c99_ast.DoWhileStmt(body=body, condition=cond, label="")

    # `for_init: var_decl | exp? SEMICOLON`. The var_decl alternative
    # already consumes its own SEMICOLON, so it arrives as the only
    # child — a `Type_var_decl` for object decls, or a
    # `Type_function_decl` if the var_decl rule's declarator composed
    # to a FunType (forward function declaration, which shares the
    # `specifier+ declarator ;` prefix with var_decl). Function
    # declarations aren't legal in for-init per C99 §6.8.5.3 ("only
    # objects having storage class auto or register"); reject those
    # explicitly so the function decl doesn't silently get dropped
    # and the loop parsed with an empty init clause. The exp-or-empty
    # alternative carries an explicit SEMICOLON token: zero or one
    # preceding exp child, then SEMICOLON.
    def for_init(self, items):
        if len(items) == 1:
            child = items[0]
            if isinstance(child, c99_ast.Type_var_decl):
                # §6.8.5.3 also bans `static` / `extern` for-init
                # objects (only auto / register are permitted; in our
                # model that's `storage_class=None`).
                if child.storage_class is not None:
                    raise ParserError(
                        f"storage class "
                        f"{type(child.storage_class).__name__.lower()!r} "
                        f"isn't permitted on a for-loop initializer "
                        f"(C99 §6.8.5.3 — only auto / register)"
                    )
                return c99_ast.InitDecl(var_decl=child)
            if isinstance(child, c99_ast.Type_function_decl):
                raise ParserError(
                    "function declarations aren't permitted in a "
                    "for-loop initializer (C99 §6.8.5.3)"
                )
            # Bare SEMICOLON — empty for-init clause.
            return c99_ast.InitExp(exp=None)
        # exp + SEMICOLON.
        return c99_ast.InitExp(exp=items[0])

    # `switch (exp) statement` — the controlling expression is a full
    # `exp` (per C99 §6.8.4.2.1 — wider than the §6.6 constraints
    # imposed on case labels). The labeling pass mints `.switch@<N>`
    # and stamps it onto `label`; the same pass walks the body to
    # collect `cases` / `default_label`. The type checker fills in
    # `promoted_type` after integer promotion of the control.
    @v_args(inline=True)
    def switch_stmt(self, _switch, _lp, control, _rp, body):
        return c99_ast.SwitchStmt(
            control=control,
            body=body,
            label="",
            cases=[],
            default_label=None,
            promoted_type=None,
        )

    # `case constant_exp : statement`. The grammar's `constant_exp`
    # non-terminal is just a one-child wrapper around `conditional_exp`
    # (C99 §6.6 — same syntax, additional semantic constraints). The
    # constness check lives in the labeling / type-checking passes,
    # not here.
    @v_args(inline=True)
    def case_stmt(self, _case, value, _colon, body):
        return c99_ast.CaseStmt(value=value, body=body, label="")

    @v_args(inline=True)
    def default_stmt(self, _default, _colon, body):
        return c99_ast.DefaultStmt(body=body, label="")

    @v_args(inline=True)
    def constant_exp(self, exp):
        # Pass-through wrapper. The grammar non-terminal exists so call
        # sites are self-documenting and share a §6.6 validator; the
        # AST shape is just the inner exp.
        return exp

    # `for (for_init exp? ; exp?) statement` — for_init contributes one
    # child that already swallowed the first SEMICOLON; the middle SEMI
    # between the condition and post_clause is in our items list. Each
    # of condition / post_clause is independently optional, so we scan
    # for the SEMICOLON to know which side each remaining child is on.
    def for_stmt(self, items):
        # items: [FOR, LPAREN, for_init, condition?, SEMICOLON, post_clause?,
        #         RPAREN, statement]
        init = items[2]
        body = items[-1]
        middle = items[3:-2]
        semi_idx = next(
            i for i, c in enumerate(middle)
            if hasattr(c, "type") and c.type == "SEMICOLON"
        )
        condition = middle[0] if semi_idx > 0 else None
        post_clause = (
            middle[semi_idx + 1] if semi_idx + 1 < len(middle) else None
        )
        return c99_ast.ForStmt(
            init=init,
            condition=condition,
            post_clause=post_clause,
            body=body,
            label="",
        )

    # Alternatives of `exp` — each named in c99.lark.
    @v_args(inline=True)
    def const(self, token):
        # `const: INTEGER_CONSTANT | LONG_INTEGER | UINT_INTEGER
        # | ULONG_INTEGER | DOUBLE_CONSTANT | FLOAT_CONSTANT
        # | LONG_DOUBLE_CONSTANT`. The lex split is by suffix presence
        # only. For integers, `_const_for_token` consults the C99
        # §6.4.4.1 type list (keyed by token-kind + base) and picks
        # the first variant whose range fits. For FP, the suffix
        # uniquely determines the type (§6.4.4.2). Long-double and
        # long-long both raise here.
        return _const_for_token(token)

    @v_args(inline=True)
    def constant(self, c):
        # `?atom: const -> constant`. The `const` handler already
        # built a ConstInt or ConstLong; wrap it in the Constant
        # expression node.
        return c99_ast.Constant(const=c)

    @v_args(inline=True)
    def identifier(self, token):
        return c99_ast.Var(name=str(token))

    @v_args(inline=True)
    def cast(self, _lp, target_type, _rp, exp):
        # `cast_exp: LPAREN type_name RPAREN cast_exp -> cast`. The
        # `type_name` transformer already resolved the type-specifier
        # run to a c99_ast Int/Long node; just plug it into the AST.
        return c99_ast.Cast(target_type=target_type, exp=exp)

    @v_args(inline=True)
    def assignment(self, lval, _assign, rval):
        return c99_ast.Assignment(lval=lval, rval=rval)

    @v_args(inline=True)
    def compound_assign(self, lval, op_token, rval):
        # `lval OP= rval` desugars at parse time to `lval = lval OP rval`.
        # The lval node is duplicated as a tree reference (Assignment.lval
        # and Binary.left point at the same Python object). That's safe
        # today because the only legal lval is a `Var`, which has no
        # side effect when re-evaluated. When richer lvalues (`*p`,
        # `a[i]`, `s.f`) land, this rewrite has to materialize the
        # address into a temp instead so the lval is evaluated once.
        op_cls = _COMPOUND_ASSIGN_OPS[op_token.type]
        return c99_ast.Assignment(
            lval=lval,
            rval=c99_ast.Binary(op=op_cls(), left=lval, right=rval),
        )

    @v_args(inline=True)
    def unary(self, op, inner):
        return c99_ast.Unary(op=op, exp=inner)

    # `*e` and `&e` build their own AST nodes (not Unary variants).
    # The leading STAR / AMP token is discarded — the AST node itself
    # encodes the operator. The `data_type` field is left unset for
    # the type checker to fill in (Dereference yields the pointee
    # type; AddressOf yields a Pointer to the operand's type).
    @v_args(inline=True)
    def dereference(self, _star, inner):
        return c99_ast.Dereference(exp=inner)

    @v_args(inline=True)
    def address_of(self, _amp, inner):
        return c99_ast.AddressOf(exp=inner)

    # Prefix `++a` / `--a` desugar to `a = a ± 1` (same shape as
    # `a += 1` / `a -= 1`). The lval node is duplicated by reference
    # — safe today because the only legal lval is a `Var`, which has
    # no side effect when re-evaluated. Future richer lvalues need a
    # rewrite that materializes the address into a temp first.
    @v_args(inline=True)
    def pre_increment(self, _op, operand):
        return self._prefix_incdec(c99_ast.Add(), operand)

    @v_args(inline=True)
    def pre_decrement(self, _op, operand):
        return self._prefix_incdec(c99_ast.Subtract(), operand)

    def _prefix_incdec(self, op, operand):
        return c99_ast.Assignment(
            lval=operand,
            rval=c99_ast.Binary(
                op=op,
                left=operand,
                right=c99_ast.Constant(const=_make_int_const(1)),
            ),
        )

    # Postfix `a++` / `a--` keep their own AST node because they have
    # to return the *old* value of the operand while also mutating
    # it. The lvalue check (operand must be a `Var`) lives in
    # identifier_resolution alongside the Assignment check.
    @v_args(inline=True)
    def post_increment(self, operand, _op):
        return c99_ast.Postfix(op=c99_ast.Increment(), operand=operand)

    @v_args(inline=True)
    def post_decrement(self, operand, _op):
        return c99_ast.Postfix(op=c99_ast.Decrement(), operand=operand)

    # Postfix subscript `e1[e2]` per C99 §6.5.2.1. Children are
    # `[postfix_exp, LBRACKET, exp, RBRACKET]` — keep the array and
    # index expressions; bracket tokens are framing.
    @v_args(inline=True)
    def subscript(self, array, _lb, index, _rb):
        return c99_ast.Subscript(array=array, index=index)

    @v_args(inline=True)
    def paren(self, _lp, inner, _rp):
        return inner

    # `IDENTIFIER LPAREN arg_list? RPAREN` — a function call. With
    # no arguments, items is [IDENT, LPAREN, RPAREN] (3); with an
    # arg_list, items is [IDENT, LPAREN, [arg, ...], RPAREN] (4).
    def function_call(self, items):
        name = str(items[0])
        args = items[2] if len(items) == 4 else []
        return c99_ast.FunctionCall(name=name, args=args)

    # `LPAREN exp RPAREN LPAREN arg_list? RPAREN` — function call
    # through a parenthesised callee. Children:
    #   [LPAREN, callee_exp, RPAREN, LPAREN, arg_list?, RPAREN]
    # — 5 if no args, 6 with an arg_list. Per C99 §6.3.2.1.4 the
    # callee auto-decays to a function pointer in any expression
    # context except the operands of `&` / `sizeof`, so several
    # surface forms boil down to "call through this name":
    #   `(fp)(args)`   — Var(fp): direct or indirect, depending on
    #                    fp's symbol-table type. Same as `fp(args)`.
    #   `(*fp)(args)`  — Dereference(Var(fp)): the dereference is
    #                    elided per the auto-decay rule. Same as
    #                    `fp(args)`.
    #   `(&foo)(args)` — AddressOf(Var(foo)): pointer to foo,
    #                    immediately called through. Same as
    #                    `foo(args)`.
    # Anything else (`(arr[i])(args)`, `(f())(args)`, ...) needs
    # the AST's name field replaced with a general callee
    # expression — separate scope; reject for now.
    def indirect_function_call(self, items):
        callee = items[1]
        args = items[4] if len(items) == 6 else []
        # Drill through the auto-decay shapes.
        if isinstance(callee, c99_ast.Var):
            name = callee.name
        elif (
            isinstance(callee, (c99_ast.Dereference, c99_ast.AddressOf))
            and isinstance(callee.exp, c99_ast.Var)
        ):
            name = callee.exp.name
        else:
            raise ParserError(
                "indirect call through complex expression not "
                "supported — callee must be a name, `*name`, or "
                "`&name`"
            )
        return c99_ast.FunctionCall(name=name, args=args)

    # `arg_list: exp (COMMA exp)*` — every other child is an exp;
    # the COMMA tokens are interleaved.
    def arg_list(self, items):
        return [items[i] for i in range(0, len(items), 2)]

    @v_args(inline=True)
    def conditional(self, condition, _q, true_clause, _c, false_clause):
        return c99_ast.Conditional(
            condition=condition,
            true_clause=true_clause,
            false_clause=false_clause,
        )

    # Binary alternatives of `exp` — tokens discarded, build a Binary node.
    @v_args(inline=True)
    def multiply(self, left, _star, right):
        return c99_ast.Binary(op=c99_ast.Multiply(), left=left, right=right)

    @v_args(inline=True)
    def divide(self, left, _slash, right):
        return c99_ast.Binary(op=c99_ast.Divide(), left=left, right=right)

    @v_args(inline=True)
    def modulo(self, left, _percent, right):
        return c99_ast.Binary(op=c99_ast.Modulo(), left=left, right=right)

    @v_args(inline=True)
    def add(self, left, _plus, right):
        return c99_ast.Binary(op=c99_ast.Add(), left=left, right=right)

    @v_args(inline=True)
    def subtract(self, left, _minus, right):
        return c99_ast.Binary(op=c99_ast.Subtract(), left=left, right=right)

    @v_args(inline=True)
    def bitwise_and(self, left, _amp, right):
        return c99_ast.Binary(op=c99_ast.BitwiseAnd(), left=left, right=right)

    @v_args(inline=True)
    def bitwise_or(self, left, _pipe, right):
        return c99_ast.Binary(op=c99_ast.BitwiseOr(), left=left, right=right)

    @v_args(inline=True)
    def bitwise_xor(self, left, _caret, right):
        return c99_ast.Binary(op=c99_ast.BitwiseXor(), left=left, right=right)

    @v_args(inline=True)
    def left_shift(self, left, _lshift, right):
        return c99_ast.Binary(op=c99_ast.LeftShift(), left=left, right=right)

    @v_args(inline=True)
    def right_shift(self, left, _rshift, right):
        return c99_ast.Binary(op=c99_ast.RightShift(), left=left, right=right)

    @v_args(inline=True)
    def equal(self, left, _eq, right):
        return c99_ast.Binary(op=c99_ast.Equal(), left=left, right=right)

    @v_args(inline=True)
    def not_equal(self, left, _ne, right):
        return c99_ast.Binary(op=c99_ast.NotEqual(), left=left, right=right)

    @v_args(inline=True)
    def less_than(self, left, _lt, right):
        return c99_ast.Binary(op=c99_ast.LessThan(), left=left, right=right)

    @v_args(inline=True)
    def greater_than(self, left, _gt, right):
        return c99_ast.Binary(op=c99_ast.GreaterThan(), left=left, right=right)

    @v_args(inline=True)
    def less_or_equal(self, left, _le, right):
        return c99_ast.Binary(op=c99_ast.LessOrEqual(), left=left, right=right)

    @v_args(inline=True)
    def greater_or_equal(self, left, _ge, right):
        return c99_ast.Binary(op=c99_ast.GreaterOrEqual(), left=left, right=right)

    @v_args(inline=True)
    def logical_and(self, left, _andand, right):
        return c99_ast.Binary(op=c99_ast.LogicalAnd(), left=left, right=right)

    @v_args(inline=True)
    def logical_or(self, left, _oror, right):
        return c99_ast.Binary(op=c99_ast.LogicalOr(), left=left, right=right)

    # Alternatives of `unop` — tokens discarded, just produce the AST op.
    @v_args(inline=True)
    def negate(self, _minus):
        return c99_ast.Negate()

    @v_args(inline=True)
    def complement(self, _tilde):
        return c99_ast.Complement()

    @v_args(inline=True)
    def logical_not(self, _bang):
        return c99_ast.LogicalNot()


_BUILDER = _ASTBuilder()


def parse(source: str) -> c99_ast.Type_program:
    tree = _LARK.parse(source, start="start")
    try:
        return _BUILDER.transform(tree)
    except VisitError as e:
        # Lark wraps any exception raised by a transformer method in
        # `VisitError`. We raise our own ParserError from
        # `_split_specifiers`, so unwrap to give the caller a clean
        # ParserError instead of forcing it to dig through `.orig_exc`.
        if isinstance(e.orig_exc, ParserError):
            raise e.orig_exc
        raise
