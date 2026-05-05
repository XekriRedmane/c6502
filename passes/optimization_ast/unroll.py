"""Loop unrolling for `#pragma c6502 loop unroll(enable)`.

Runs before identifier_resolution under `--optimize --unroll`.
Replaces every for-loop carrying `unroll_annotation == "unroll"`
with a Compound of N back-to-back copies of the body, each in
its own scope, with the induction variable substituted by the
corresponding integer constant in each clone.

Recognizer (canonical shape only — anything else raises
UnrollError, since the user explicitly asked to unroll):
  init: `T i = <integer-constant>;` with T in
        {int, unsigned int, long, unsigned long,
         long long, unsigned long long}
  cond: `i <op> <integer-constant>` with op in {<, <=, >, >=}
  post: `i++`, `i--`, `++i`, `--i`, `i += K`, or `i -= K`
        (K an integer constant > 0)
  body: no break/continue/goto/labeled-statement; no
        modification (assignment / inc / dec / address-of) of
        the induction variable; no inner declaration shadowing
        the induction variable.

Iteration count is capped at MAX_ITERATIONS.

Why AST level rather than TAC: identifier_resolution and
loop_labeling run AFTER this pass, so each cloned body's locals
get fresh `@N.<name>` names per iteration without us having to
re-implement that machinery. The substituted induction variable
literal flows through the type checker / TAC / asm passes
exactly like any user-written constant — no extra
constant-folding hop required.
"""
from __future__ import annotations

import copy

import c99_ast


class UnrollError(Exception):
    """A `#pragma c6502 loop unroll(enable)`-annotated loop did not
    match the canonical shape the unroller can handle."""


MAX_ITERATIONS = 256


# Induction-variable types we accept, mapped to the const variant
# we emit when substituting the iv with a literal value. The Char
# mappings follow the type checker's convention: c6502 makes plain
# `char` unsigned, so Char and UChar both route to ConstUChar; only
# `signed char` produces ConstChar.
_IV_TYPE_TO_CONST: dict[type, type] = {
    c99_ast.Int: c99_ast.ConstInt,
    c99_ast.UInt: c99_ast.ConstUInt,
    c99_ast.Long: c99_ast.ConstLong,
    c99_ast.ULong: c99_ast.ConstULong,
    c99_ast.LongLong: c99_ast.ConstLongLong,
    c99_ast.ULongLong: c99_ast.ConstULongLong,
    c99_ast.Char: c99_ast.ConstUChar,
    c99_ast.SChar: c99_ast.ConstChar,
    c99_ast.UChar: c99_ast.ConstUChar,
}


def unroll_program(prog: c99_ast.Type_program) -> c99_ast.Type_program:
    """Walk every function definition; unroll every annotated for-
    loop in their bodies. Top-level declarations and forward
    function declarations pass through unchanged."""
    new_decls: list[c99_ast.Type_declaration] = []
    for d in prog.declaration:
        if (
            isinstance(d, c99_ast.FunctionDecl)
            and d.function_decl.body is not None
        ):
            new_decls.append(c99_ast.FunctionDecl(
                function_decl=_unroll_function(d.function_decl),
            ))
        else:
            new_decls.append(d)
    return c99_ast.Program(declaration=new_decls)


def _unroll_function(
    fd: c99_ast.Type_function_decl,
) -> c99_ast.Type_function_decl:
    return c99_ast.Type_function_decl(
        name=fd.name,
        params=list(fd.params),
        body=c99_ast.Block(block_item=[
            _unroll_block_item(bi) for bi in fd.body.block_item
        ]),
        data_type=fd.data_type,
        storage_class=fd.storage_class,
        abi_annotation=fd.abi_annotation,
    )


def _unroll_block_item(bi: c99_ast.Type_block_item) -> c99_ast.Type_block_item:
    if isinstance(bi, c99_ast.S):
        return c99_ast.S(statement=_unroll_statement(bi.statement))
    return bi


def _unroll_statement(stmt: c99_ast.Type_statement) -> c99_ast.Type_statement:
    """Rewrite annotated for-loops; recurse through every other
    statement form so nested annotated loops are also unrolled."""
    match stmt:
        case c99_ast.ForStmt(unroll_annotation="unroll"):
            return _unroll_for(stmt)
        case c99_ast.ForStmt():
            return c99_ast.ForStmt(
                init=stmt.init,
                condition=stmt.condition,
                post_clause=stmt.post_clause,
                body=_unroll_statement(stmt.body),
                label=stmt.label,
                unroll_annotation=stmt.unroll_annotation,
            )
        case c99_ast.IfStmt(condition=c, then_clause=t, else_clause=e):
            return c99_ast.IfStmt(
                condition=c,
                then_clause=_unroll_statement(t),
                else_clause=_unroll_statement(e) if e is not None else None,
            )
        case c99_ast.Compound(block=block):
            return c99_ast.Compound(block=c99_ast.Block(
                block_item=[_unroll_block_item(bi) for bi in block.block_item],
            ))
        case c99_ast.WhileStmt(condition=c, body=b, label=lbl):
            return c99_ast.WhileStmt(
                condition=c,
                body=_unroll_statement(b),
                label=lbl,
            )
        case c99_ast.DoWhileStmt(body=b, condition=c, label=lbl):
            return c99_ast.DoWhileStmt(
                body=_unroll_statement(b),
                condition=c,
                label=lbl,
            )
        case c99_ast.SwitchStmt():
            return c99_ast.SwitchStmt(
                control=stmt.control,
                body=_unroll_statement(stmt.body),
                label=stmt.label,
                cases=stmt.cases,
                default_label=stmt.default_label,
                promoted_type=stmt.promoted_type,
            )
        case c99_ast.CaseStmt(value=v, body=b, label=lbl):
            return c99_ast.CaseStmt(
                value=v, body=_unroll_statement(b), label=lbl,
            )
        case c99_ast.DefaultStmt(body=b, label=lbl):
            return c99_ast.DefaultStmt(
                body=_unroll_statement(b), label=lbl,
            )
        case c99_ast.LabeledStmt(label=l, statement=s):
            return c99_ast.LabeledStmt(
                label=l, statement=_unroll_statement(s),
            )
        case _:
            return stmt


def _unroll_for(fs: c99_ast.ForStmt) -> c99_ast.Type_statement:
    iv_name, iv_type, iv_init = _validate_init(fs.init)
    op, bound = _validate_condition(fs.condition, iv_name)
    step = _validate_post(fs.post_clause, iv_name)
    _validate_body(fs.body, iv_name)
    values = _compute_iterations(iv_init, op, bound, step)

    const_cls = _IV_TYPE_TO_CONST[type(iv_type)]
    iter_stmts: list[c99_ast.Type_statement] = []
    for v in values:
        clone = _substitute(copy.deepcopy(fs.body), iv_name, const_cls, v)
        # Recurse so inner annotated loops in the clone unroll too.
        clone = _unroll_statement(clone)
        if isinstance(clone, c99_ast.Compound):
            iter_stmts.append(clone)
        else:
            # Single-statement bodies (`for (...) x++;`) get wrapped
            # in a Compound so each iteration owns its own scope —
            # required if the body declares any locals.
            iter_stmts.append(c99_ast.Compound(block=c99_ast.Block(
                block_item=[c99_ast.S(statement=clone)],
            )))

    return c99_ast.Compound(block=c99_ast.Block(
        block_item=[c99_ast.S(statement=s) for s in iter_stmts],
    ))


def _validate_init(
    init: c99_ast.Type_for_init,
) -> tuple[str, c99_ast.Type_data_type, int]:
    if not isinstance(init, c99_ast.InitDecl):
        raise UnrollError(
            "unroll: for-init must declare the induction variable "
            "(`for (T i = <const>; ...; ...)`)",
        )
    vd = init.var_decl
    if vd.storage_class is not None:
        raise UnrollError(
            "unroll: induction variable cannot have a storage class",
        )
    if type(vd.data_type) not in _IV_TYPE_TO_CONST:
        raise UnrollError(
            f"unroll: induction variable type {type(vd.data_type).__name__} "
            "not supported (use int / unsigned int / long / unsigned long / "
            "long long / unsigned long long)",
        )
    init_val = _const_int_value(vd.init)
    if init_val is None:
        raise UnrollError(
            "unroll: induction variable must be initialized to an "
            "integer constant",
        )
    return vd.name, vd.data_type, init_val


def _validate_condition(
    cond: c99_ast.Type_exp | None, iv_name: str,
) -> tuple[c99_ast.Type_binary_operator, int]:
    if cond is None:
        raise UnrollError("unroll: for-loop condition is required")
    if not isinstance(cond, c99_ast.Binary):
        raise UnrollError(
            "unroll: condition must be `<iv> <op> <const>` with op in "
            "{<, <=, >, >=}",
        )
    if not isinstance(cond.op, (
        c99_ast.LessThan, c99_ast.LessOrEqual,
        c99_ast.GreaterThan, c99_ast.GreaterOrEqual,
    )):
        raise UnrollError(
            f"unroll: comparison op {type(cond.op).__name__} not supported "
            "(use <, <=, >, or >=)",
        )
    if not _is_var(cond.left, iv_name):
        raise UnrollError(
            "unroll: condition's left operand must be the induction "
            f"variable `{iv_name}`",
        )
    bound = _const_int_value(cond.right)
    if bound is None:
        raise UnrollError(
            "unroll: condition's right operand must be an integer constant",
        )
    return cond.op, bound


def _validate_post(post: c99_ast.Type_exp | None, iv_name: str) -> int:
    if post is None:
        raise UnrollError("unroll: for-loop post-clause is required")
    match post:
        case c99_ast.Postfix(op=c99_ast.Increment(), operand=op_) \
             if _is_var(op_, iv_name):
            return 1
        case c99_ast.Postfix(op=c99_ast.Decrement(), operand=op_) \
             if _is_var(op_, iv_name):
            return -1
        case c99_ast.Prefix(op=c99_ast.Increment(), operand=op_) \
             if _is_var(op_, iv_name):
            return 1
        case c99_ast.Prefix(op=c99_ast.Decrement(), operand=op_) \
             if _is_var(op_, iv_name):
            return -1
        case c99_ast.CompoundAssignment(op=op, lval=lval, rval=rval) \
             if _is_var(lval, iv_name):
            k = _const_int_value(rval)
            if k is None or k <= 0:
                raise UnrollError(
                    "unroll: post-clause step must be a positive integer "
                    "constant",
                )
            if isinstance(op, c99_ast.Add):
                return k
            if isinstance(op, c99_ast.Subtract):
                return -k
            raise UnrollError(
                f"unroll: post-clause op {type(op).__name__} not supported "
                "(use +=, -=, ++, or --)",
            )
    raise UnrollError(
        "unroll: post-clause must be `i++`, `i--`, `++i`, `--i`, "
        "`i += K`, or `i -= K`",
    )


def _validate_body(body: c99_ast.Type_statement, iv_name: str) -> None:
    """Reject body shapes the unroller can't faithfully clone."""
    for node in _walk(body):
        match node:
            case c99_ast.BreakStmt():
                raise UnrollError(
                    "unroll: `break` inside an unrolled loop body is not "
                    "supported",
                )
            case c99_ast.ContinueStmt():
                raise UnrollError(
                    "unroll: `continue` inside an unrolled loop body is "
                    "not supported",
                )
            case c99_ast.Goto():
                raise UnrollError(
                    "unroll: `goto` inside an unrolled loop body is not "
                    "supported",
                )
            case c99_ast.LabeledStmt():
                raise UnrollError(
                    "unroll: labeled statements inside an unrolled loop "
                    "body are not supported",
                )
            case c99_ast.AddressOf(exp=inner) if _is_var(inner, iv_name):
                raise UnrollError(
                    f"unroll: address of induction variable `{iv_name}` "
                    "is taken in body",
                )
            case c99_ast.Assignment(lval=lval) if _is_var(lval, iv_name):
                raise UnrollError(
                    f"unroll: induction variable `{iv_name}` is "
                    "reassigned in body",
                )
            case c99_ast.CompoundAssignment(lval=lval) \
                 if _is_var(lval, iv_name):
                raise UnrollError(
                    f"unroll: induction variable `{iv_name}` is modified "
                    "in body",
                )
            case c99_ast.Prefix(operand=op_) if _is_var(op_, iv_name):
                raise UnrollError(
                    f"unroll: induction variable `{iv_name}` is modified "
                    "in body",
                )
            case c99_ast.Postfix(operand=op_) if _is_var(op_, iv_name):
                raise UnrollError(
                    f"unroll: induction variable `{iv_name}` is modified "
                    "in body",
                )
            case c99_ast.VarDecl(var_decl=inner_vd) \
                 if inner_vd.name == iv_name:
                raise UnrollError(
                    f"unroll: induction variable `{iv_name}` is shadowed "
                    "by an inner declaration",
                )
            case c99_ast.InitDecl(var_decl=inner_vd) \
                 if inner_vd.name == iv_name:
                # Inner for-loop init declaring the same name.
                raise UnrollError(
                    f"unroll: induction variable `{iv_name}` is shadowed "
                    "by an inner for-loop init",
                )


def _compute_iterations(
    init_val: int,
    op: c99_ast.Type_binary_operator,
    bound: int,
    step: int,
) -> list[int]:
    """Simulate the loop, returning the induction-variable values
    one per iteration. The simulator is the only termination
    guard: any loop that hasn't finished after MAX_ITERATIONS
    steps is rejected with a cap-exceeded error. We don't attempt
    to detect non-terminating loops up front (the halting problem
    is undecidable in general), so a `for (int i = 0; i < 4;
    i--)` is rejected via the cap rather than via a pre-flight
    direction check."""
    cond_holds = _make_cond_predicate(op)
    values: list[int] = []
    i = init_val
    while cond_holds(i, bound):
        if len(values) >= MAX_ITERATIONS:
            raise UnrollError(
                f"unroll: iteration count exceeds cap of {MAX_ITERATIONS} "
                "(loop may not terminate, or simply runs too long to unroll)",
            )
        values.append(i)
        i += step
    return values


def _make_cond_predicate(op: c99_ast.Type_binary_operator):
    if isinstance(op, c99_ast.LessThan):
        return lambda i, b: i < b
    if isinstance(op, c99_ast.LessOrEqual):
        return lambda i, b: i <= b
    if isinstance(op, c99_ast.GreaterThan):
        return lambda i, b: i > b
    if isinstance(op, c99_ast.GreaterOrEqual):
        return lambda i, b: i >= b
    raise AssertionError(f"unreachable: {op!r}")


def _substitute(node, iv_name: str, const_cls: type, value: int):
    """Walk the subtree rooted at `node`, replacing every
    `Var(iv_name)` with `Constant(const_cls(value))`. Returns
    the (possibly-replaced) root. Mutates internal dataclass
    nodes in place; caller has already deepcopy'd the tree."""
    if isinstance(node, c99_ast.Var) and node.name == iv_name:
        return c99_ast.Constant(
            const=const_cls(value=value), data_type=node.data_type,
        )
    if hasattr(node, "__dataclass_fields__"):
        for fname in node.__dataclass_fields__:
            child = getattr(node, fname)
            new_child = _substitute(child, iv_name, const_cls, value)
            if new_child is not child:
                setattr(node, fname, new_child)
        return node
    if isinstance(node, list):
        return [_substitute(c, iv_name, const_cls, value) for c in node]
    return node


def _walk(node):
    """Pre-order traversal of every dataclass node in the subtree."""
    if hasattr(node, "__dataclass_fields__"):
        yield node
        for fname in node.__dataclass_fields__:
            yield from _walk(getattr(node, fname))
    elif isinstance(node, list):
        for item in node:
            yield from _walk(item)


def _is_var(exp, name: str) -> bool:
    return isinstance(exp, c99_ast.Var) and exp.name == name


def _const_int_value(exp) -> int | None:
    """Return the int value if `exp` is an integer Constant
    (transparently unwrapping any leading Casts), else None."""
    if exp is None:
        return None
    while isinstance(exp, c99_ast.Cast):
        exp = exp.exp
    if isinstance(exp, c99_ast.Constant) and isinstance(
        exp.const,
        (c99_ast.ConstInt, c99_ast.ConstLong, c99_ast.ConstLongLong,
         c99_ast.ConstUInt, c99_ast.ConstULong, c99_ast.ConstULongLong),
    ):
        return exp.const.value
    return None
