"""Translate a c99_ast tree into a tac_ast tree (three-address code).

Every C99 expression becomes a tac_ast `val` (either a Constant or a Var
holding the result of an earlier instruction). Compound expressions get
flattened: nested operators materialize their intermediate results into
fresh Var-typed temporaries and emit the corresponding TAC instruction.

State:
  - Translator owns the temporary-name counter (`%0`, `%1`, ...) and a
    separate label counter (`and_false@0`, `and_end@0`, ...) for the
    short-circuit lowerings.
  - The per-function instruction list is passed down explicitly as an
    argument so there's no implicit "current function" on the instance.

Mapping:
  C99 Program(fn)             -> TAC Program(translate_function(fn))
  C99 Function(name, body)    -> TAC Function(name, <instrs built from
                                 each block_item in order>); if the
                                 body doesn't already end in a Ret,
                                 append `Ret(Constant(0))` (C99
                                 §5.1.2.2.3 for main; we apply it
                                 generally so every function
                                 terminates).
  C99 S(stmt)                 -> dispatches to translate_statement
  C99 D(decl)                 -> dispatches to translate_declaration
  C99 Declaration(name, init) -> if init is None, emit nothing; else
                                 evaluate init then
                                 Copy(init_val, Var(name)) — same TAC
                                 as the assignment `name = init`. TAC
                                 has no separate notion of a declared-
                                 but-uninitialized variable; the var
                                 name appears the first time it's used.
  C99 Return(exp)             -> emit Ret(translate_exp(exp))
  C99 Expression(exp)         -> translate_exp(exp) for side effects;
                                 the returned val is discarded.
  C99 IfStmt(cond, then,      -> evaluate cond, JumpIfFalse around
        else_clause)             the then-branch (skip directly to
                                 if_end@N when there's no else;
                                 jump-around an else-branch with a
                                 Jump+Label pair when there is). All
                                 labels come from the shared label
                                 counter (`if_end@N`, `if_else@N`).
  C99 Goto(label)             -> tac Jump(label). The label name is
                                 the unique `.<funcname>@<label>`
                                 minted by label_resolution — a
                                 dasm-style local label, scoped to
                                 the SUBROUTINE the asm emits. The
                                 `@` separator (illegal in C
                                 identifiers) keeps it disjoint
                                 from translator-minted labels
                                 (`.<prefix>_<N>`).
  C99 LabeledStmt(label, stmt) -> emit tac Label(label), then lower
                                 the inner statement. Label name is
                                 already unique (see Goto).
  C99 BreakStmt(label)        -> tac Jump(<label>_break). The
                                 incoming `label` is the base name
                                 (`.loop@<N>`) attached by
                                 loop_labeling; we derive the per-
                                 loop sub-targets by suffix.
  C99 ContinueStmt(label)     -> tac Jump(<label>_continue).
  C99 WhileStmt(cond, body,   -> Label(<continue>); <eval cond -> v>;
                label)           JumpIfFalse(v, <break>); <lower body>;
                                 Jump(<continue>); Label(<break>). The
                                 continue target is at the top of
                                 the loop (re-tests the condition);
                                 the break target sits after.
  C99 DoWhileStmt(body, cond, -> Label(<start>); <lower body>;
                  label)         Label(<continue>); <eval cond -> v>;
                                 JumpIfTrue(v, <start>); Label(<break>).
                                 The continue target sits between the
                                 body and the test, so `continue` re-
                                 runs the condition.
  C99 ForStmt(init, cond,     -> <init insns>; Label(<start>);
              post, body,        <eval cond -> v>;  -- omitted if cond
              label)             JumpIfFalse(v, <break>); -- is None
                                 <lower body>; Label(<continue>);
                                 <post insns>; -- omitted if post is None
                                 Jump(<start>); Label(<break>). The
                                 init runs once, then a test-body-
                                 post cycle. `continue` jumps to the
                                 post step (so it still runs); a
                                 missing condition is treated as
                                 unconditionally true so the test
                                 and its JumpIfFalse drop out.
  C99 InitDecl(decl)          -> same as a top-level Declaration
                                 (Copy of the initializer into the
                                 var; nothing for a bare `int x;`).
  C99 InitExp(exp)            -> evaluate `exp` for its side effects;
                                 result is discarded. Empty
                                 `InitExp(None)` lowers to nothing.
  C99 Compound(block)         -> lower each block item in order;
                                 no extra TAC structure (TAC is
                                 flat — block boundaries don't
                                 survive into the IR).
  C99 Null                    -> emit nothing
  C99 Constant(v)             -> TAC Constant(v)
  C99 Unary(op, inner)        -> emit Unary(op', translate(inner), Var(t))
                                 and return Var(t), where t is a fresh temp
  C99 Binary(op, left, right) -> emit Binary(op', translate(left),
                                 translate(right), Var(t))
                                 and return Var(t); left is translated
                                 before right so any temps it needs are
                                 numbered first.
  C99 Var(name)               -> TAC Var(name) — passthrough. The name
                                 is the unique `@N.orig` minted by
                                 identifier_resolution; it shares a
                                 namespace with TAC temps `%n` but
                                 can't collide because `@` and `%` are
                                 both illegal in C identifiers.
  C99 Assignment(Var(v), rval) -> emit translate(rval) -> rval_val,
                                 then Copy(rval_val, Var(v)); return
                                 Var(v) so chained assignments
                                 (`b = a = 5`) compose correctly. lval
                                 must be a Var (identifier_resolution
                                 enforces this; we double-check at
                                 runtime).
  C99 Postfix(op, Var(v))     -> emit Copy(Var(v), %old) to capture
                                 the operand's value before mutation,
                                 then Binary(Add/Subtract, Var(v),
                                 Constant(1), %new) to compute the
                                 updated value, then Copy(%new,
                                 Var(v)) to store it back. Returns
                                 Var(%old) so callers see the *old*
                                 value (postfix semantics) — distinct
                                 from prefix `++a`/`--a`, which the
                                 parser desugars to `a = a ± 1` and
                                 returns the *new* value via the
                                 Assignment branch.
  C99 Negate / Complement /   -> TAC Negate / Complement / LogicalNot
    LogicalNot
  C99 Add / Subtract /        -> TAC Add / Subtract / Multiply / Divide
    Multiply / Divide /          / Modulo / BitwiseAnd / BitwiseOr /
    Modulo / BitwiseAnd /        BitwiseXor / LeftShift / RightShift /
    BitwiseOr / BitwiseXor /     Equal / NotEqual / LessThan /
    LeftShift / RightShift /     GreaterThan / LessOrEqual /
    Equal / NotEqual /           GreaterOrEqual
    LessThan / GreaterThan /
    LessOrEqual / GreaterOrEqual

  C99 Conditional(cond, t, f) -> like an if/else that also produces a
                                 value: evaluate cond, JumpIfFalse to
                                 cond_else@N, evaluate t and Copy into
                                 a fresh dst temp, Jump(cond_end@N),
                                 Label(cond_else@N), evaluate f and
                                 Copy into the same dst, Label(
                                 cond_end@N). Returns dst. Labels come
                                 from the shared label counter
                                 (`cond_else@N`/`cond_end@N`), so each
                                 ternary gets globally unique numbers.

Short-circuit lowerings (no corresponding TAC binary op — the control
flow *is* the semantics):
  C99 Binary(LogicalAnd, L, R):
      <eval L -> src1>
      JumpIfFalse(src1, and_false@N)
      <eval R -> src2>
      JumpIfFalse(src2, and_false@N)
      Copy(Constant(1), result)
      Jump(and_end@N)
      Label(and_false@N)
      Copy(Constant(0), result)
      Label(and_end@N)
  C99 Binary(LogicalOr, L, R): symmetric, with JumpIfTrue / or_true@N /
      or_end@N and the 0/1 constants swapped. Each use of && or || gets
      a fresh N so nested short-circuits don't collide.
"""

from __future__ import annotations

import c99_ast
import tac_ast


# Per-loop sub-label derivation. The loop_labeling pass stamps each
# loop with a base label like `.loop@3`; the TAC lowering needs three
# distinct targets for that loop (start, continue target, break
# target), so we suffix the base. The base contains `@` (illegal in
# any C identifier), so neither it nor its suffixed forms can collide
# with a user-mangled label. They're also disjoint from every other
# translator-minted label (`.if_end@<N>`, `.cond_else@<N>`, …): those
# differ in prefix, and they end at the digit run after `@` rather
# than in a `_start`/`_continue`/`_break` suffix.
def _start_label(loop_label: str) -> str:
    return f"{loop_label}_start"


def _continue_label(loop_label: str) -> str:
    return f"{loop_label}_continue"


def _break_label(loop_label: str) -> str:
    return f"{loop_label}_break"


class Translator:
    def __init__(self) -> None:
        self._temp_counter = 0
        self._label_counter = 0

    def make_temporary_variable_name(self) -> str:
        name = f"%{self._temp_counter}"
        self._temp_counter += 1
        return name

    def make_label(self, prefix: str) -> str:
        # Leading `.` makes this a dasm-style local label — scoped to
        # the enclosing SUBROUTINE, so labels in different functions
        # don't collide in the global asm namespace. The `@`
        # separator (illegal in any C identifier) means a translator-
        # minted label can never be confused with anything the user
        # could write: user goto labels are mangled to
        # `.<funcname>@<orig>` where the part after `@` is a C
        # identifier; here the part after `@` is digits.
        name = f".{prefix}@{self._label_counter}"
        self._label_counter += 1
        return name

    def translate_program(self, prog: c99_ast.Type_program) -> tac_ast.Type_program:
        # tac.asdl still has a single function_definition slot — multi-
        # function TAC is on the same TODO list as `FunctionCall`
        # lowering, since you can't usefully have one without the
        # other. For now the c99 program must contain exactly one
        # function definition (`int main(void) { ... }`); when call
        # lowering lands, both AST and this dispatcher will widen.
        match prog:
            case c99_ast.Program(function_definition=fns):
                if len(fns) != 1:
                    raise NotImplementedError(
                        "c99_to_tac currently supports a single function "
                        f"definition; got {len(fns)}",
                    )
                return tac_ast.Program(
                    function_definition=self.translate_function(fns[0]),
                )
        raise TypeError(f"unexpected program: {prog!r}")

    def translate_function(
        self, fn: c99_ast.Type_function_definition,
    ) -> tac_ast.Type_function_definition:
        match fn:
            case c99_ast.Function(name=name, body=body):
                instrs: list[tac_ast.Type_instruction] = []
                self.translate_block(body, instrs)
                # If the body didn't end in a Return, fall off the end
                # with an implicit `return 0`. C99 §5.1.2.2.3 specifies
                # this for `main`; we apply it generally so every TAC
                # function is guaranteed to terminate with a Ret. If a
                # Ret is already there, skip — adding a second would
                # be unreachable dead code.
                if not instrs or not isinstance(instrs[-1], tac_ast.Ret):
                    instrs.append(tac_ast.Ret(val=tac_ast.Constant(value=0)))
                return tac_ast.Function(name=name, instructions=instrs)
        raise TypeError(f"unexpected function: {fn!r}")

    def translate_block(
        self,
        block: c99_ast.Type_block,
        instrs: list[tac_ast.Type_instruction],
    ) -> None:
        match block:
            case c99_ast.Block(block_item=items):
                for item in items:
                    self.translate_block_item(item, instrs)
                return
        raise TypeError(f"unexpected block: {block!r}")

    def translate_block_item(
        self,
        item: c99_ast.Type_block_item,
        instrs: list[tac_ast.Type_instruction],
    ) -> None:
        match item:
            case c99_ast.S(statement=stmt):
                self.translate_statement(stmt, instrs)
                return
            case c99_ast.D(declaration=decl):
                self.translate_declaration(decl, instrs)
                return
        raise TypeError(f"unexpected block item: {item!r}")

    def translate_declaration(
        self,
        decl: c99_ast.Type_declaration,
        instrs: list[tac_ast.Type_instruction],
    ) -> None:
        # TAC has no "declare" instruction — variables are introduced
        # by their first appearance. So a bare `int x;` lowers to
        # nothing, and `int x = e;` lowers exactly like the assignment
        # `x = e`: evaluate the initializer, then Copy into the var.
        # A FunctionDecl is purely a name-binding artifact (consumed
        # by identifier_resolution to validate calls); it has no
        # runtime effect, so it lowers to nothing.
        match decl:
            case c99_ast.VarDecl(var_decl=vd):
                if vd.init is not None:
                    init_val = self.translate_exp(vd.init, instrs)
                    instrs.append(tac_ast.Copy(
                        src=init_val, dst=tac_ast.Var(name=vd.name),
                    ))
                return
            case c99_ast.FunctionDecl():
                return
        raise TypeError(f"unexpected declaration: {decl!r}")

    def translate_statement(
        self,
        stmt: c99_ast.Type_statement,
        instrs: list[tac_ast.Type_instruction],
    ) -> None:
        match stmt:
            case c99_ast.Return(exp=exp):
                instrs.append(tac_ast.Ret(val=self.translate_exp(exp, instrs)))
                return
            case c99_ast.Expression(exp=exp):
                # Translate for side effects (assignments today; calls
                # later). Whatever val the expression returns goes
                # unused — the result-temp it points at is just dead.
                self.translate_exp(exp, instrs)
                return
            case c99_ast.IfStmt(
                condition=cond, then_clause=then_stmt, else_clause=else_stmt,
            ):
                # `if (cond) then` lowers to:
                #   <eval cond -> cond_val>
                #   JumpIfFalse(cond_val, end_N)
                #   <lower then>
                #   Label(end_N)
                # With an else-branch, an extra Jump and Label split
                # the two arms:
                #   <eval cond -> cond_val>
                #   JumpIfFalse(cond_val, else_N)
                #   <lower then>
                #   Jump(end_N)
                #   Label(else_N)
                #   <lower else>
                #   Label(end_N)
                # Labels share the same counter the short-circuit
                # lowerings use, so each `if` gets globally unique
                # `if_else@N`/`if_end@N` numbers.
                cond_val = self.translate_exp(cond, instrs)
                end_label = self.make_label("if_end")
                if else_stmt is None:
                    instrs.append(tac_ast.JumpIfFalse(
                        condition=cond_val, target=end_label,
                    ))
                    self.translate_statement(then_stmt, instrs)
                    instrs.append(tac_ast.Label(name=end_label))
                else:
                    else_label = self.make_label("if_else")
                    instrs.append(tac_ast.JumpIfFalse(
                        condition=cond_val, target=else_label,
                    ))
                    self.translate_statement(then_stmt, instrs)
                    instrs.append(tac_ast.Jump(target=end_label))
                    instrs.append(tac_ast.Label(name=else_label))
                    self.translate_statement(else_stmt, instrs)
                    instrs.append(tac_ast.Label(name=end_label))
                return
            case c99_ast.Compound(block=block):
                # `{ ... }` — TAC is flat, so a compound statement
                # is just its block items lowered in order. Scope is
                # already gone by this point (identifier_resolution
                # rewrote every name to its globally-unique form), so
                # there's nothing left for `{ ... }` to mean at the
                # IR level. The grammar doesn't yet have a
                # `compound_stmt` rule, so this only fires when an
                # AST is built directly; the lowering is the same
                # either way.
                self.translate_block(block, instrs)
                return
            case c99_ast.Goto(label=label):
                # `goto label;` lowers to an unconditional Jump. The
                # target name is the unique `.<funcname>@<label>`
                # minted by label_resolution — a dasm local label
                # (leading dot scopes it to the enclosing SUBROUTINE).
                # The `@` separator (illegal in a C identifier) keeps
                # these disjoint from translator-minted labels like
                # `.if_end@N` — they share the @-marker convention,
                # but the part after `@` is a C identifier here vs.
                # a digit run there.
                instrs.append(tac_ast.Jump(target=label))
                return
            case c99_ast.LabeledStmt(label=label, statement=inner):
                # `label: stmt` lowers to a TAC Label followed by the
                # inner statement's own lowering. The label name is
                # already the unique `.<funcname>@<label>` from
                # label_resolution.
                instrs.append(tac_ast.Label(name=label))
                self.translate_statement(inner, instrs)
                return
            case c99_ast.BreakStmt(label=label):
                # `break;` lowers to an unconditional jump to the
                # break-target label of the enclosing loop. The loop
                # label is the base name (e.g. `.loop@3`) minted by
                # the loop_labeling pass; we derive the per-loop
                # break/continue/start targets from it by suffix.
                instrs.append(tac_ast.Jump(target=_break_label(label)))
                return
            case c99_ast.ContinueStmt(label=label):
                instrs.append(tac_ast.Jump(target=_continue_label(label)))
                return
            case c99_ast.WhileStmt(condition=cond, body=body, label=label):
                # while: test-then-body, with the continue target at
                # the top of the loop (re-tests the condition) and the
                # break target after the loop.
                #   Label(<continue>)
                #   <eval cond -> v>
                #   JumpIfFalse(v, <break>)
                #   <lower body>
                #   Jump(<continue>)
                #   Label(<break>)
                cont = _continue_label(label)
                brk = _break_label(label)
                instrs.append(tac_ast.Label(name=cont))
                cond_val = self.translate_exp(cond, instrs)
                instrs.append(tac_ast.JumpIfFalse(
                    condition=cond_val, target=brk,
                ))
                self.translate_statement(body, instrs)
                instrs.append(tac_ast.Jump(target=cont))
                instrs.append(tac_ast.Label(name=brk))
                return
            case c99_ast.DoWhileStmt(body=body, condition=cond, label=label):
                # do-while: body-then-test. The continue target sits
                # *between* the body and the condition test (so
                # `continue` re-runs the test), and the break target
                # sits after everything.
                #   Label(<start>)
                #   <lower body>
                #   Label(<continue>)
                #   <eval cond -> v>
                #   JumpIfTrue(v, <start>)
                #   Label(<break>)
                start = _start_label(label)
                cont = _continue_label(label)
                brk = _break_label(label)
                instrs.append(tac_ast.Label(name=start))
                self.translate_statement(body, instrs)
                instrs.append(tac_ast.Label(name=cont))
                cond_val = self.translate_exp(cond, instrs)
                instrs.append(tac_ast.JumpIfTrue(
                    condition=cond_val, target=start,
                ))
                instrs.append(tac_ast.Label(name=brk))
                return
            case c99_ast.ForStmt(
                init=init, condition=cond, post_clause=post,
                body=body, label=label,
            ):
                # for: init, then test-body-post, with the continue
                # target between the body and the post-iteration step
                # (so `continue` skips the rest of the body but still
                # runs the post step), and the break target after the
                # loop. A missing condition is treated as
                # unconditionally true — we just skip the
                # JumpIfFalse, since there's nothing to test.
                #   <init insns>
                #   Label(<start>)
                #   <eval cond -> v>          (omitted if cond is None)
                #   JumpIfFalse(v, <break>)   (omitted if cond is None)
                #   <lower body>
                #   Label(<continue>)
                #   <post insns>              (omitted if post is None)
                #   Jump(<start>)
                #   Label(<break>)
                start = _start_label(label)
                cont = _continue_label(label)
                brk = _break_label(label)
                self.translate_for_init(init, instrs)
                instrs.append(tac_ast.Label(name=start))
                if cond is not None:
                    cond_val = self.translate_exp(cond, instrs)
                    instrs.append(tac_ast.JumpIfFalse(
                        condition=cond_val, target=brk,
                    ))
                self.translate_statement(body, instrs)
                instrs.append(tac_ast.Label(name=cont))
                if post is not None:
                    # Post-clause is an expression evaluated for its
                    # side effects (the result value is discarded).
                    self.translate_exp(post, instrs)
                instrs.append(tac_ast.Jump(target=start))
                instrs.append(tac_ast.Label(name=brk))
                return
            case c99_ast.Null():
                # No-op statement. Nothing to emit.
                return
        raise TypeError(f"unexpected statement: {stmt!r}")

    def translate_for_init(
        self,
        init: c99_ast.Type_for_init,
        instrs: list[tac_ast.Type_instruction],
    ) -> None:
        # For-init runs once before the loop body. A declaration
        # lowers exactly like a top-level declaration (Copy of init
        # value into the var, or nothing for a bare `int x;`); an
        # expression-init runs the expression for side effects with
        # the result thrown away. An empty `for (;;)` lowers to no
        # init instructions.
        match init:
            case c99_ast.InitDecl(var_decl=vd):
                # for-init is restricted to variable declarations
                # (C99 §6.8.5), so we lower the var_decl directly
                # rather than going through the wider declaration
                # dispatcher.
                if vd.init is not None:
                    init_val = self.translate_exp(vd.init, instrs)
                    instrs.append(tac_ast.Copy(
                        src=init_val, dst=tac_ast.Var(name=vd.name),
                    ))
                return
            case c99_ast.InitExp(exp=exp):
                if exp is not None:
                    self.translate_exp(exp, instrs)
                return
        raise TypeError(f"unexpected for_init: {init!r}")

    def translate_exp(
        self,
        exp: c99_ast.Type_exp,
        instrs: list[tac_ast.Type_instruction],
    ) -> tac_ast.Type_val:
        match exp:
            case c99_ast.Constant(value=v):
                return tac_ast.Constant(value=v)
            case c99_ast.Unary(op=op, exp=inner):
                src = self.translate_exp(inner, instrs)
                dst = tac_ast.Var(name=self.make_temporary_variable_name())
                instrs.append(tac_ast.Unary(
                    op=self.translate_unop(op),
                    src=src,
                    dst=dst,
                ))
                return dst
            case c99_ast.Binary(op=c99_ast.LogicalAnd(), left=left, right=right):
                return self.translate_short_circuit(
                    left, right, instrs,
                    short_circuit_on_true=False,
                )
            case c99_ast.Binary(op=c99_ast.LogicalOr(), left=left, right=right):
                return self.translate_short_circuit(
                    left, right, instrs,
                    short_circuit_on_true=True,
                )
            case c99_ast.Binary(op=op, left=left, right=right):
                # Translate left first so its temps get the lower
                # numbers — matches a left-to-right evaluation order
                # readers will expect.
                src1 = self.translate_exp(left, instrs)
                src2 = self.translate_exp(right, instrs)
                dst = tac_ast.Var(name=self.make_temporary_variable_name())
                instrs.append(tac_ast.Binary(
                    op=self.translate_binop(op),
                    src1=src1,
                    src2=src2,
                    dst=dst,
                ))
                return dst
            case c99_ast.Var(name=name):
                # Resolved name from identifier_resolution (e.g. `@0.x`)
                # passes straight through into TAC's Var namespace —
                # `@` and TAC's `%` are both illegal in C identifiers,
                # so user vars and translator temps can't collide.
                return tac_ast.Var(name=name)
            case c99_ast.Assignment(lval=lval, rval=rval):
                # identifier_resolution already enforces lval-is-Var;
                # the runtime check here is belt-and-braces in case a
                # later refactor lets a non-Var slip through.
                if not isinstance(lval, c99_ast.Var):
                    raise TypeError(
                        f"assignment lval must be Var (variable_"
                        f"resolution should have enforced this); "
                        f"got {lval!r}"
                    )
                rval_val = self.translate_exp(rval, instrs)
                dst = tac_ast.Var(name=lval.name)
                instrs.append(tac_ast.Copy(src=rval_val, dst=dst))
                # Return the lval so chained assignments compose:
                # `b = a = 5` -> inner returns Var(@0.a), outer copies
                # that into @1.b and returns Var(@1.b).
                return dst
            case c99_ast.Conditional(
                condition=cond,
                true_clause=true_clause,
                false_clause=false_clause,
            ):
                # `cond ? t : f` lowers like an if/else that also
                # produces a value: both arms Copy into a shared dst
                # temp so the result is a single Var the caller can
                # thread into later instructions. Labels come from the
                # same counter as `if`/short-circuit, so numbering stays
                # globally unique.
                #   <eval cond -> cond_val>
                #   JumpIfFalse(cond_val, cond_else@N)
                #   <eval true -> t_val>
                #   Copy(t_val, dst)
                #   Jump(cond_end@N)
                #   Label(cond_else@N)
                #   <eval false -> f_val>
                #   Copy(f_val, dst)
                #   Label(cond_end@N)
                cond_val = self.translate_exp(cond, instrs)
                else_label = self.make_label("cond_else")
                end_label = self.make_label("cond_end")
                dst = tac_ast.Var(name=self.make_temporary_variable_name())
                instrs.append(tac_ast.JumpIfFalse(
                    condition=cond_val, target=else_label,
                ))
                t_val = self.translate_exp(true_clause, instrs)
                instrs.append(tac_ast.Copy(src=t_val, dst=dst))
                instrs.append(tac_ast.Jump(target=end_label))
                instrs.append(tac_ast.Label(name=else_label))
                f_val = self.translate_exp(false_clause, instrs)
                instrs.append(tac_ast.Copy(src=f_val, dst=dst))
                instrs.append(tac_ast.Label(name=end_label))
                return dst
            case c99_ast.FunctionCall():
                # TODO: lower function calls. Needs both a TAC
                # representation for calls (not in tac.asdl yet) and
                # a calling convention that hands args through the
                # soft stack and reads the return value back. Until
                # then, parsing+resolution accept calls but the IR
                # has no way to express them.
                raise NotImplementedError(
                    "FunctionCall lowering is not yet implemented",
                )
            case c99_ast.Postfix(op=op, operand=operand):
                # `a++` (resp. `a--`) returns the *old* value of `a`
                # while incrementing (decrementing) it. Capture the
                # old value into a temp first; only then update `a`.
                # Returning the temp means later uses of the result
                # see the old value even after `a` has been mutated.
                #
                # Same defense-in-depth lvalue check as Assignment:
                # identifier_resolution should have already rejected
                # non-Var operands.
                if not isinstance(operand, c99_ast.Var):
                    raise TypeError(
                        f"postfix operand must be Var (variable_"
                        f"resolution should have enforced this); "
                        f"got {operand!r}"
                    )
                var = tac_ast.Var(name=operand.name)
                old = tac_ast.Var(name=self.make_temporary_variable_name())
                instrs.append(tac_ast.Copy(src=var, dst=old))
                new = tac_ast.Var(name=self.make_temporary_variable_name())
                instrs.append(tac_ast.Binary(
                    op=self.translate_incdec(op),
                    src1=var,
                    src2=tac_ast.Constant(value=1),
                    dst=new,
                ))
                instrs.append(tac_ast.Copy(src=new, dst=var))
                return old
        raise TypeError(f"unexpected exp: {exp!r}")

    def translate_short_circuit(
        self,
        left: c99_ast.Type_exp,
        right: c99_ast.Type_exp,
        instrs: list[tac_ast.Type_instruction],
        short_circuit_on_true: bool,
    ) -> tac_ast.Type_val:
        # && short-circuits to 0 on the first false operand; || to 1
        # on the first true operand. Otherwise the two lowerings are
        # mirror images, so we parametrize:
        #   - which conditional-jump opcode short-circuits the chain
        #   - which constant the short-circuit branch writes (the
        #     short-circuit outcome), vs. the fallthrough branch (the
        #     opposite outcome)
        if short_circuit_on_true:
            branch_prefix, end_prefix = "or_true", "or_end"
            short_circuit_jump = tac_ast.JumpIfTrue
            short_circuit_value, fallthrough_value = 1, 0
        else:
            branch_prefix, end_prefix = "and_false", "and_end"
            short_circuit_jump = tac_ast.JumpIfFalse
            short_circuit_value, fallthrough_value = 0, 1
        branch_label = self.make_label(branch_prefix)
        end_label = self.make_label(end_prefix)
        dst = tac_ast.Var(name=self.make_temporary_variable_name())

        src1 = self.translate_exp(left, instrs)
        instrs.append(short_circuit_jump(condition=src1, target=branch_label))
        src2 = self.translate_exp(right, instrs)
        instrs.append(short_circuit_jump(condition=src2, target=branch_label))
        instrs.append(tac_ast.Copy(
            src=tac_ast.Constant(value=fallthrough_value), dst=dst,
        ))
        instrs.append(tac_ast.Jump(target=end_label))
        instrs.append(tac_ast.Label(name=branch_label))
        instrs.append(tac_ast.Copy(
            src=tac_ast.Constant(value=short_circuit_value), dst=dst,
        ))
        instrs.append(tac_ast.Label(name=end_label))
        return dst

    def translate_unop(
        self, op: c99_ast.Type_unary_operator,
    ) -> tac_ast.Type_unary_operator:
        match op:
            case c99_ast.Complement():
                return tac_ast.Complement()
            case c99_ast.Negate():
                return tac_ast.Negate()
            case c99_ast.LogicalNot():
                return tac_ast.LogicalNot()
        raise TypeError(f"unexpected unop: {op!r}")

    def translate_incdec(
        self, op: c99_ast.Type_incdec_op,
    ) -> tac_ast.Type_binary_operator:
        # Postfix ++/-- lower to a Binary(Add/Subtract, operand, 1).
        match op:
            case c99_ast.Increment():
                return tac_ast.Add()
            case c99_ast.Decrement():
                return tac_ast.Subtract()
        raise TypeError(f"unexpected incdec op: {op!r}")

    def translate_binop(
        self, op: c99_ast.Type_binary_operator,
    ) -> tac_ast.Type_binary_operator:
        match op:
            case c99_ast.Add():
                return tac_ast.Add()
            case c99_ast.Subtract():
                return tac_ast.Subtract()
            case c99_ast.Multiply():
                return tac_ast.Multiply()
            case c99_ast.Divide():
                return tac_ast.Divide()
            case c99_ast.Modulo():
                return tac_ast.Modulo()
            case c99_ast.BitwiseAnd():
                return tac_ast.BitwiseAnd()
            case c99_ast.BitwiseOr():
                return tac_ast.BitwiseOr()
            case c99_ast.BitwiseXor():
                return tac_ast.BitwiseXor()
            case c99_ast.LeftShift():
                return tac_ast.LeftShift()
            case c99_ast.RightShift():
                return tac_ast.RightShift()
            case c99_ast.Equal():
                return tac_ast.Equal()
            case c99_ast.NotEqual():
                return tac_ast.NotEqual()
            case c99_ast.LessThan():
                return tac_ast.LessThan()
            case c99_ast.GreaterThan():
                return tac_ast.GreaterThan()
            case c99_ast.LessOrEqual():
                return tac_ast.LessOrEqual()
            case c99_ast.GreaterOrEqual():
                return tac_ast.GreaterOrEqual()
        raise TypeError(f"unexpected binop: {op!r}")


def translate_program(prog: c99_ast.Type_program) -> tac_ast.Type_program:
    """Convenience wrapper: builds a fresh Translator per call (so the
    temporary counter starts at 0 every time)."""
    return Translator().translate_program(prog)
