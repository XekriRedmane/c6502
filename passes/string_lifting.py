"""String-literal lifting pass.

Walks a c99_ast Program after identifier_resolution and rewrites
every `String` literal that ISN'T directly initializing a
`Char` / `SChar` / `UChar` array variable. Each such string gets
hoisted to a fresh file-scope `static char [N+1]` declaration with
a unique generated name (`.str@N` — the leading `.` and `@` keep
it disjoint from any user identifier and from translator-minted
labels), and the original `String` node is replaced with a `Var`
referencing that new declaration.

Why this exists
---------------
String literals are static-storage objects per C99 §6.4.5.6 — they
live somewhere in memory for the whole program lifetime. Most
expression contexts use them by reference (decay to `char *`); only
a direct array-initializer `char arr[N] = "abc";` consumes the
bytes inline. By hoisting non-array-init strings here, the rest of
the pipeline (type-check, c99_to_tac, codegen) gets to treat them
just like ordinary file-scope arrays — array decay handles `char *
p = "abc"`, AddressOf-of-array handles `&"abc"`, subscript handles
`"abc"[1]`, etc., all without per-pass special cases.

What stays inline
-----------------
A `String` directly in `var_decl.init` whose `var_decl.data_type`
is `Array(Char | SChar | UChar, _)` keeps the literal in place. The
type checker validates it against the array's declared size; the
backend lays the bytes down as the array's storage.

Naming
------
`.str@N` — `N` is a per-program counter. Each occurrence of a
literal mints its own name (no deduplication today; trivial to add
if it matters for code size). Identifier resolution has already
run, so the new declarations don't go through it — we register
them as already-resolved names that the type checker can pick up
from the file-scope decl list.
"""

from __future__ import annotations

import c99_ast


class StringLifter:
    """Per-program lifter — owns the `.str@N` counter so each string
    literal in the program gets a globally unique name. Walks the
    Program top-down once, accumulating new file-scope var_decls in
    `_lifted` and rewriting String nodes in-place where the
    walking parent allows it."""

    def __init__(self) -> None:
        self._counter = 0
        self._lifted: list[c99_ast.Type_declaration] = []

    def lift_program(
        self, prog: c99_ast.Type_program,
    ) -> c99_ast.Type_program:
        out: list[c99_ast.Type_declaration] = []
        for decl in prog.declaration:
            out.append(self._lift_declaration(decl))
        # Lifted strings prepend the original declarations so any
        # static-init reference (`static char *p = "abc";` →
        # `&Var(.str@0)`) finds its target already declared earlier
        # in the file; the type checker's file-scope walk processes
        # declarations in order.
        return c99_ast.Program(declaration=self._lifted + out)

    # ------------------------------------------------------------------
    # Declarations
    # ------------------------------------------------------------------

    def _lift_declaration(
        self, decl: c99_ast.Type_declaration,
    ) -> c99_ast.Type_declaration:
        if isinstance(decl, c99_ast.VarDecl):
            return c99_ast.VarDecl(var_decl=self._lift_var_decl(decl.var_decl))
        if isinstance(decl, c99_ast.FunctionDecl):
            return c99_ast.FunctionDecl(
                function_decl=self._lift_function_decl(decl.function_decl),
            )
        raise TypeError(f"unexpected declaration: {decl!r}")

    def _lift_var_decl(self, vd):
        """Lift any String *inside* the init, except the direct-
        char-array-init case. Pass other fields through unchanged."""
        new_init = self._lift_init(vd.init, vd.data_type)
        if new_init is vd.init:
            return vd
        return c99_ast.Type_var_decl(
            name=vd.name, init=new_init,
            data_type=vd.data_type, storage_class=vd.storage_class,
        )

    def _lift_init(self, init, declared_type):
        """`init` is the raw exp from `var_decl.init` (or None).
        Threads `declared_type` through any nested `InitList` so
        that a String at any depth whose corresponding sub-array
        slot has a char-element type stays inline (`signed char
        a[3][4] = {{...}, "efgh", "ijk"};` — the inner Strings
        target `signed char[4]` sub-arrays, so they're direct
        char-array initializers and stay inline). Anywhere a
        String shows up that ISN'T at a char-array slot, fall
        through to `_lift_exp` to lift it to a file-scope
        static."""
        if init is None:
            return None
        # Direct char-array init — keep the String inline. The
        # init may be the bare `String(...)` or a Cast wrapping
        # one (the parser doesn't wrap String in casts today, but
        # be tolerant).
        if (
            isinstance(init, c99_ast.String)
            and self._is_char_array(declared_type)
        ):
            return init
        if (
            isinstance(init, c99_ast.InitList)
            and isinstance(declared_type, c99_ast.Array)
        ):
            # Walk each item against the corresponding sub-array
            # element type. The InitList itself has a fixed
            # shape (one item per array slot); nested InitLists
            # at sub-array positions recurse with the sub-array's
            # element type, so a String at the deepest char-
            # element-array slot is recognised as a direct
            # char-array initializer.
            elem_type = declared_type.element_type
            new_items = [
                self._lift_init(it, elem_type) for it in init.items
            ]
            return c99_ast.InitList(
                items=new_items, data_type=init.data_type,
            )
        return self._lift_exp(init)

    def _lift_function_decl(self, fd):
        if fd.body is None:
            return fd
        return c99_ast.Type_function_decl(
            name=fd.name, params=fd.params,
            body=self._lift_block(fd.body),
            data_type=fd.data_type, storage_class=fd.storage_class,
        )

    # ------------------------------------------------------------------
    # Blocks / statements
    # ------------------------------------------------------------------

    def _lift_block(self, block):
        return c99_ast.Block(block_item=[
            self._lift_block_item(it) for it in block.block_item
        ])

    def _lift_block_item(self, item):
        if isinstance(item, c99_ast.S):
            return c99_ast.S(statement=self._lift_statement(item.statement))
        if isinstance(item, c99_ast.D):
            return c99_ast.D(
                declaration=self._lift_declaration(item.declaration),
            )
        raise TypeError(f"unexpected block item: {item!r}")

    def _lift_statement(self, stmt):
        match stmt:
            case c99_ast.Return(exp=e):
                return c99_ast.Return(
                    exp=self._lift_exp(e) if e is not None else None,
                )
            case c99_ast.Expression(exp=e):
                return c99_ast.Expression(exp=self._lift_exp(e))
            case c99_ast.IfStmt(
                condition=c, then_clause=t, else_clause=el,
            ):
                return c99_ast.IfStmt(
                    condition=self._lift_exp(c),
                    then_clause=self._lift_statement(t),
                    else_clause=(
                        self._lift_statement(el) if el is not None else None
                    ),
                )
            case c99_ast.Compound(block=b):
                return c99_ast.Compound(block=self._lift_block(b))
            case c99_ast.WhileStmt(condition=c, body=b, label=lbl):
                return c99_ast.WhileStmt(
                    condition=self._lift_exp(c),
                    body=self._lift_statement(b), label=lbl,
                )
            case c99_ast.DoWhileStmt(body=b, condition=c, label=lbl):
                return c99_ast.DoWhileStmt(
                    body=self._lift_statement(b),
                    condition=self._lift_exp(c), label=lbl,
                )
            case c99_ast.ForStmt(
                init=fi, condition=c, post_clause=p, body=b, label=lbl,
            ):
                return c99_ast.ForStmt(
                    init=self._lift_for_init(fi),
                    condition=self._lift_exp(c) if c is not None else None,
                    post_clause=(
                        self._lift_exp(p) if p is not None else None
                    ),
                    body=self._lift_statement(b), label=lbl,
                )
            case c99_ast.SwitchStmt(
                control=c, body=b, label=lbl, cases=cs,
                default_label=dl, promoted_type=pt,
            ):
                return c99_ast.SwitchStmt(
                    control=self._lift_exp(c),
                    body=self._lift_statement(b),
                    label=lbl, cases=cs, default_label=dl,
                    promoted_type=pt,
                )
            case c99_ast.CaseStmt(value=v, body=b, label=lbl):
                # `value` is a constant expression — strings
                # aren't legal there per §6.6, but we walk it for
                # uniformity. The type checker rejects any
                # surviving String.
                return c99_ast.CaseStmt(
                    value=self._lift_exp(v),
                    body=self._lift_statement(b), label=lbl,
                )
            case c99_ast.DefaultStmt(body=b, label=lbl):
                return c99_ast.DefaultStmt(
                    body=self._lift_statement(b), label=lbl,
                )
            case c99_ast.LabeledStmt(label=lbl, statement=s):
                return c99_ast.LabeledStmt(
                    label=lbl, statement=self._lift_statement(s),
                )
            case (
                c99_ast.Goto()
                | c99_ast.BreakStmt()
                | c99_ast.ContinueStmt()
                | c99_ast.Null()
            ):
                return stmt
        raise TypeError(f"unexpected statement: {stmt!r}")

    def _lift_for_init(self, fi):
        if isinstance(fi, c99_ast.InitDecl):
            return c99_ast.InitDecl(
                var_decl=self._lift_var_decl(fi.var_decl),
            )
        if isinstance(fi, c99_ast.InitExp):
            return c99_ast.InitExp(
                exp=self._lift_exp(fi.exp) if fi.exp is not None else None,
            )
        raise TypeError(f"unexpected for-init: {fi!r}")

    # ------------------------------------------------------------------
    # Expressions
    # ------------------------------------------------------------------

    def _lift_exp(self, exp):
        """Walk an expression tree, rewriting every String node to a
        Var(.str@N) reference and recording the new file-scope
        var_decl. Other expression nodes pass through, recursing into
        their child expressions to catch nested Strings."""
        if exp is None:
            return None
        match exp:
            case c99_ast.String(str=s):
                return self._mint_static_for_string(s)
            case c99_ast.Constant() | c99_ast.Var():
                return exp
            case c99_ast.Cast(target_type=t, exp=inner, data_type=dt):
                return c99_ast.Cast(
                    target_type=t, exp=self._lift_exp(inner), data_type=dt,
                )
            case c99_ast.Unary(op=op, exp=inner, data_type=dt):
                return c99_ast.Unary(
                    op=op, exp=self._lift_exp(inner), data_type=dt,
                )
            case c99_ast.Binary(
                op=op, left=l, right=r, data_type=dt,
            ):
                return c99_ast.Binary(
                    op=op,
                    left=self._lift_exp(l), right=self._lift_exp(r),
                    data_type=dt,
                )
            case c99_ast.Assignment(lval=lv, rval=rv, data_type=dt):
                return c99_ast.Assignment(
                    lval=self._lift_exp(lv), rval=self._lift_exp(rv),
                    data_type=dt,
                )
            case c99_ast.Postfix(op=op, operand=o, data_type=dt):
                return c99_ast.Postfix(
                    op=op, operand=self._lift_exp(o), data_type=dt,
                )
            case c99_ast.Prefix(op=op, operand=o, data_type=dt):
                return c99_ast.Prefix(
                    op=op, operand=self._lift_exp(o), data_type=dt,
                )
            case c99_ast.Conditional(
                condition=c, true_clause=t, false_clause=f, data_type=dt,
            ):
                return c99_ast.Conditional(
                    condition=self._lift_exp(c),
                    true_clause=self._lift_exp(t),
                    false_clause=self._lift_exp(f),
                    data_type=dt,
                )
            case c99_ast.FunctionCall(name=n, args=args, data_type=dt):
                return c99_ast.FunctionCall(
                    name=n, args=[self._lift_exp(a) for a in args],
                    data_type=dt,
                )
            case c99_ast.Dereference(exp=inner, data_type=dt):
                return c99_ast.Dereference(
                    exp=self._lift_exp(inner), data_type=dt,
                )
            case c99_ast.AddressOf(exp=inner, data_type=dt):
                return c99_ast.AddressOf(
                    exp=self._lift_exp(inner), data_type=dt,
                )
            case c99_ast.Subscript(array=a, index=i, data_type=dt):
                return c99_ast.Subscript(
                    array=self._lift_exp(a), index=self._lift_exp(i),
                    data_type=dt,
                )
            case c99_ast.InitList(items=items, data_type=dt):
                return c99_ast.InitList(
                    items=[self._lift_exp(it) for it in items],
                    data_type=dt,
                )
        raise TypeError(f"unexpected exp: {exp!r}")

    # ------------------------------------------------------------------
    # Static minting
    # ------------------------------------------------------------------

    def _mint_static_for_string(self, s: str) -> c99_ast.Var:
        """Mint a fresh file-scope `static char[N+1]` declaration
        whose initializer is the literal `s`, and return a Var
        node naming that declaration. The declaration is appended
        to `self._lifted` and prepended to the program at the end
        of the walk."""
        name = f".str@{self._counter}"
        self._counter += 1
        # The hoisted declaration: `static char .str@N[len(s)+1] = "s";`.
        # `Char` (the conventional plain-char element type) — codegen
        # treats SChar / Char as identical 1-byte signed types, and the
        # element type doesn't otherwise matter since the only reads
        # are byte loads from a Char-pointer that decays from this
        # array.
        decl = c99_ast.VarDecl(var_decl=c99_ast.Type_var_decl(
            name=name,
            init=c99_ast.String(str=s),
            data_type=c99_ast.Array(
                element_type=c99_ast.Char(), size=len(s) + 1,
            ),
            storage_class=c99_ast.Static(),
        ))
        self._lifted.append(decl)
        return c99_ast.Var(name=name)

    # ------------------------------------------------------------------
    # Predicates
    # ------------------------------------------------------------------

    @staticmethod
    def _is_char_array(t) -> bool:
        if not isinstance(t, c99_ast.Array):
            return False
        return isinstance(
            t.element_type, (c99_ast.Char, c99_ast.SChar, c99_ast.UChar),
        )


def lift_program(prog: c99_ast.Type_program) -> c99_ast.Type_program:
    """Module-level entry point — fresh `StringLifter` per call so
    `.str@N` numbering restarts at 0 for each program. The result
    has every non-direct-array-init String replaced with a Var, and
    the new file-scope statics prepended to the program's
    declaration list."""
    return StringLifter().lift_program(prog)
