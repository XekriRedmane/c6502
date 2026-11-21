// The lowering of statements, calls, expressions, and pointer
// arithmetic, part A2 of c99_to_tac (src/c99_to_tac_translator.h).

#include <string>
#include <utility>

#include "builtin_functions.h"
#include "c99_to_tac_translator.h"
#include "errors.h"

namespace c6502::c99_to_tac {

namespace {

using EK = c99::Kind_exp;

const char kSaveThroughPointer[] =
    "`[[c6502::save_hardware_stack]]` can't apply to a call through a function pointer";

// Calls `f` on each operand of `e` that is a field of its own (a list
// of arguments or items isn't), in the order of the fields, for the
// expressions whose operands are all evaluated before the operator.
template <class E, class F>
void for_each_direct_operand(E& e, F f) {
  switch (c99::kind(e)) {
    case EK::Cast:
      f(std::get<c99::Cast>(e).exp);
      return;
    case EK::Unary:
      f(std::get<c99::Unary>(e).exp);
      return;
    case EK::Binary: {
      auto& b = std::get<c99::Binary>(e);
      f(b.left);
      f(b.right);
      return;
    }
    case EK::Assignment: {
      auto& a = std::get<c99::Assignment>(e);
      f(a.lval);
      f(a.rval);
      return;
    }
    case EK::CompoundAssignment: {
      auto& a = std::get<c99::CompoundAssignment>(e);
      f(a.lval);
      f(a.rval);
      return;
    }
    case EK::Postfix:
      f(std::get<c99::Postfix>(e).operand);
      return;
    case EK::Prefix:
      f(std::get<c99::Prefix>(e).operand);
      return;
    case EK::IndirectCall:
      f(std::get<c99::IndirectCall>(e).callee);
      return;
    case EK::Dereference:
      f(std::get<c99::Dereference>(e).exp);
      return;
    case EK::AddressOf:
      f(std::get<c99::AddressOf>(e).exp);
      return;
    case EK::Subscript: {
      auto& s = std::get<c99::Subscript>(e);
      f(s.array);
      f(s.index);
      return;
    }
    case EK::Designation:
      f(std::get<c99::Designation>(e).init);
      return;
    case EK::Dot:
      f(std::get<c99::Dot>(e).operand);
      return;
    case EK::Arrow:
      f(std::get<c99::Arrow>(e).operand);
      return;
    case EK::CompoundLiteral:
      f(std::get<c99::CompoundLiteral>(e).init);
      return;
    default:
      return;
  }
}

}  // namespace

void Translator::translate_statement(const c99::Type_statement& stmt, Instrs& instrs) {
  using SK = c99::Kind_statement;
  switch (c99::kind(stmt)) {
    case SK::Return: {
      // `return;` is `Ret(None)`, and the asm epilogue skips putting a
      // value in A and X. In a function that returns a struct, the value
      // is stored through the hidden `.sret.<name>` parameter, the
      // caller's return slot, and the function returns no value.
      const auto& r = std::get<c99::Return>(stmt);
      if (!r.exp) {
        instrs.push_back(tac::Ret{});
      } else if (sret_param) {
        const Val val = translate_value(*r.exp, instrs);
        instrs.push_back(tac::Store{.src = val, .dst_ptr = tac::Var{.name = *sret_param}});
        instrs.push_back(tac::Ret{});
      } else {
        std::optional<Val> val = translate_exp(*r.exp, instrs);
        instrs.push_back(tac::Ret{.val = std::move(val)});
      }
      return;
    }
    case SK::Expression:
      // The value goes unused, and the temporary that holds it is dead.
      translate_exp(std::get<c99::Expression>(stmt).exp, instrs);
      return;
    case SK::IfStmt: {
      // `if (c) t` is `JumpIfFalse(c, end); t; Label(end)`, and with an
      // else-branch the then-branch ends with a `Jump(end)` past it.
      const auto& s = std::get<c99::IfStmt>(stmt);
      const Val cond_val = translate_value(s.condition, instrs);
      const std::string end_label = make_label("if_end");
      if (!s.else_clause) {
        instrs.push_back(tac::JumpIfFalse{.condition = cond_val, .target = end_label});
        translate_statement(*s.then_clause, instrs);
        instrs.push_back(tac::Label{.name = end_label});
      } else {
        const std::string else_label = make_label("if_else");
        instrs.push_back(tac::JumpIfFalse{.condition = cond_val, .target = else_label});
        translate_statement(*s.then_clause, instrs);
        instrs.push_back(tac::Jump{.target = end_label});
        instrs.push_back(tac::Label{.name = else_label});
        translate_statement(**s.else_clause, instrs);
        instrs.push_back(tac::Label{.name = end_label});
      }
      return;
    }
    case SK::Compound:
      // TAC is flat, and identifier resolution has already made every
      // name unique, so a compound statement is its items in order.
      translate_block(*std::get<c99::Compound>(stmt).block, instrs);
      return;
    case SK::Goto:
      instrs.push_back(tac::Jump{.target = std::get<c99::Goto>(stmt).label});
      return;
    case SK::LabeledStmt: {
      const auto& s = std::get<c99::LabeledStmt>(stmt);
      instrs.push_back(tac::Label{.name = s.label});
      translate_statement(*s.statement, instrs);
      return;
    }
    case SK::BreakStmt:
      instrs.push_back(
          tac::Jump{.target = break_label(loop_targets(std::get<c99::BreakStmt>(stmt).label))});
      return;
    case SK::ContinueStmt:
      instrs.push_back(tac::Jump{
          .target = continue_label(loop_targets(std::get<c99::ContinueStmt>(stmt).label))});
      return;
    case SK::WhileStmt: {
      // The continue target is at the top, where the condition is
      // tested again, and the break target after the loop.
      const auto& s = std::get<c99::WhileStmt>(stmt);
      const std::string cont = continue_label(s.label);
      const std::string brk = break_label(s.label);
      instrs.push_back(tac::Label{.name = cont});
      const Val cond_val = translate_value(s.condition, instrs);
      instrs.push_back(tac::JumpIfFalse{.condition = cond_val, .target = brk});
      translate_statement(*s.body, instrs);
      instrs.push_back(tac::Jump{.target = cont});
      instrs.push_back(tac::Label{.name = brk});
      return;
    }
    case SK::DoWhileStmt: {
      // The continue target sits between the body and the test, so
      // `continue` runs the test again.
      const auto& s = std::get<c99::DoWhileStmt>(stmt);
      const std::string start = start_label(s.label);
      const std::string cont = continue_label(s.label);
      const std::string brk = break_label(s.label);
      instrs.push_back(tac::Label{.name = start});
      translate_statement(*s.body, instrs);
      instrs.push_back(tac::Label{.name = cont});
      const Val cond_val = translate_value(s.condition, instrs);
      instrs.push_back(tac::JumpIfTrue{.condition = cond_val, .target = start});
      instrs.push_back(tac::Label{.name = brk});
      return;
    }
    case SK::ForStmt: {
      // The init runs once, then the test, the body, and the post step,
      // with the continue target before the post step, so `continue`
      // still runs it. A missing condition is always true, so its test
      // and its `JumpIfFalse` are left out.
      const auto& s = std::get<c99::ForStmt>(stmt);
      std::string label = s.label;
      if (s.loop_attribute == "keep") {
        // `[[c6502::keep]]`: the labels carry `kKeepLoopMark`, which
        // dead-loop elimination reads.
        keep_loops.insert(label);
        label += kKeepLoopMark;
      }
      const std::string start = start_label(label);
      const std::string cont = continue_label(label);
      const std::string brk = break_label(label);
      translate_for_init(s.init, instrs);
      instrs.push_back(tac::Label{.name = start});
      if (s.condition) {
        const Val cond_val = translate_value(*s.condition, instrs);
        instrs.push_back(tac::JumpIfFalse{.condition = cond_val, .target = brk});
      }
      translate_statement(*s.body, instrs);
      instrs.push_back(tac::Label{.name = cont});
      if (s.post_clause) {
        translate_exp(*s.post_clause, instrs);
      }
      instrs.push_back(tac::Jump{.target = start});
      instrs.push_back(tac::Label{.name = brk});
      return;
    }
    case SK::SwitchStmt: {
      // The control is evaluated once, then compared with each case's
      // constant, each followed by a jump to the case's label, and then
      // a jump to the default, or past the body when there's none. The
      // body plants the case and default labels where they appear in
      // the source, so cases fall through unless a `break` jumps to the
      // break label (C99 §6.8.4.2p4). Loop labeling gave a `break` in
      // the body the switch's base label, so the `_break` suffix
      // matches. The type checker made each case's value a constant of
      // the promoted control type, so the comparisons are at one width.
      const auto& s = std::get<c99::SwitchStmt>(stmt);
      const std::string brk = break_label(s.label);
      const Val t_val = translate_value(s.control, instrs);
      for (const c99::Type_switch_case& c : s.cases) {
        const Val case_val = translate_value(c.value, instrs);
        tac::Var eq_temp = temp_var(Type{c99::Int{}});
        instrs.push_back(tac::Binary{.op = tac::Equal{}, .src1 = t_val, .src2 = case_val, .dst = eq_temp});
        instrs.push_back(tac::JumpIfTrue{.condition = eq_temp, .target = c.label});
      }
      instrs.push_back(tac::Jump{.target = s.default_label ? *s.default_label : brk});
      translate_statement(*s.body, instrs);
      instrs.push_back(tac::Label{.name = brk});
      return;
    }
    case SK::CaseStmt: {
      // The dispatch at the switch has consumed the case's value, so only
      // its label goes here.
      const auto& s = std::get<c99::CaseStmt>(stmt);
      instrs.push_back(tac::Label{.name = s.label});
      translate_statement(*s.body, instrs);
      return;
    }
    case SK::DefaultStmt: {
      const auto& s = std::get<c99::DefaultStmt>(stmt);
      instrs.push_back(tac::Label{.name = s.label});
      translate_statement(*s.body, instrs);
      return;
    }
    case SK::Null:
      return;
  }
}

// A declaration in a for-init lowers as one at block scope does (C99
// §6.8.5 allows several sharing one specifier run), and an expression
// runs for its side effects.
void Translator::translate_for_init(const c99::Type_for_init& init, Instrs& instrs) {
  if (const auto* d = std::get_if<c99::InitDecl>(&init)) {
    for (const c99::Type_var_decl& vd : d->var_decls) {
      if (vd.init) {
        translate_var_init(vd, instrs);
      }
    }
    return;
  }
  if (const auto& e = std::get<c99::InitExp>(init).exp) {
    translate_exp(*e, instrs);
  }
}

// Evaluates a compound literal (C99 §6.5.2.5) and returns the `Var`
// naming its object, which is then an lvalue like any other. Inside a
// function the object is automatic, and each evaluation initializes it
// as a declaration with the same initializer would. At file scope, or
// with `static` (C23 §6.5.3.6), it's static, and the symbol-table sweep
// lays down its bytes.
c99::Var Translator::materialize_compound_literal(const c99::CompoundLiteral& cl,
                                                  Instrs& instrs) {
  const passes::Symbol* sym = symbols.find(cl.name);
  if (sym == nullptr) {
    throw InternalError("a compound literal missing from the symbol table: " + cl.name);
  }
  if (!std::holds_alternative<passes::StaticAttr>(sym->attrs)) {
    const c99::Type_declaration decl = c99::VarDecl{.var_decl = c99::Type_var_decl{
                                                        .name = cl.name,
                                                        .init = *cl.init,
                                                        .data_type = cl.target_type,
                                                        .storage_class = cl.storage_class,
                                                        .is_constexpr = cl.is_constexpr,
                                                    }};
    translate_declaration(decl, instrs);
  }
  return c99::Var{.name = cl.name, .data_type = cl.data_type};
}

// Lowers a call to the function or function pointer `name`, or through
// the evaluated function pointer `ptr`. The arguments are evaluated in
// source order, so their temporaries get the lower numbers. A function
// that returns a struct or union returns it through a slot the caller
// allocates, whose address goes as the hidden first argument, and the
// call's value is the slot, a struct-typed variable that the consumers
// (an assignment's Copy, `.`, `->`) treat as any addressable struct.
std::optional<Val> Translator::translate_call(bool hw, const std::optional<Type>& data_type,
                                              const std::string* name, std::optional<Val> ptr,
                                              const std::vector<Exp>& args, Instrs& instrs) {
  bool noreturn = false;
  if (name != nullptr) {
    if (const passes::Symbol* sym = symbols.find(*name)) {
      if (!ptr && std::holds_alternative<c99::Pointer>(strip_quals(sym->type))) {
        ptr = tac::Var{.name = *name};
      }
      // A call to a `[[noreturn]]` function, by any of its declarations.
      // The call graph leaves it out when it passes no arguments
      // (passes/call_edges.py).
      if (!ptr) {
        if (const auto* f = std::get_if<passes::FunAttr>(&sym->attrs)) {
          noreturn = f->noreturn;
        }
      }
    }
  }
  if (hw && ptr) {
    throw CompileError("RecursionSaveError", kSaveThroughPointer);
  }
  std::vector<Val> arg_vals;
  for (const Exp& a : args) {
    arg_vals.push_back(translate_value(a, instrs));
  }
  // A `void` callee gives no value, so the call has no destination and
  // its asm lowering skips the capture of the return value.
  const bool is_void = data_type && std::holds_alternative<c99::Void>(*data_type);
  std::optional<Type> ret_t;
  if (data_type) {
    ret_t = unqualified(*data_type);
  }
  if (!ptr && returns_carry_struct(*name)) {
    // A `return_bool_in_carry` struct comes back in A (and X) and the
    // carry flag, with no hidden pointer, so the call writes the slot
    // itself. Its address can't be taken, so the call is always direct.
    tac::Var slot = temp_var(ret_t);
    instrs.push_back(tac::FunctionCall{
        .name = *name, .args = std::move(arg_vals), .dst = slot, .hw_stack_save = hw, .noreturn = noreturn});
    return slot;
  }
  if (ret_t && asdl::isa<c99::Structure, c99::Union>(*ret_t)) {
    tac::Var slot = temp_var(*ret_t);
    tac::Var addr = temp_var(Type{c99::Pointer{.referenced_type = *ret_t}});
    instrs.push_back(tac::GetAddress{.operand = slot, .dst = addr});
    arg_vals.insert(arg_vals.begin(), addr);
    if (ptr) {
      instrs.push_back(tac::IndirectCall{.ptr = *ptr, .args = std::move(arg_vals)});
    } else {
      instrs.push_back(tac::FunctionCall{
          .name = *name, .args = std::move(arg_vals), .hw_stack_save = hw, .noreturn = noreturn});
    }
    return slot;
  }
  std::optional<Val> dst;
  if (!is_void) {
    dst = temp_var(data_type);
  }
  if (ptr) {
    // `tac_to_asm` stages the pointer into DPTR before it calls the icall
    // trampoline.
    instrs.push_back(tac::IndirectCall{.ptr = *ptr, .args = std::move(arg_vals), .dst = dst});
  } else {
    instrs.push_back(tac::FunctionCall{
        .name = *name, .args = std::move(arg_vals), .dst = dst, .hw_stack_save = hw, .noreturn = noreturn});
  }
  return dst;
}

std::optional<Val> Translator::translate_exp(const Exp& exp_in, Instrs& instrs) {
  std::optional<Exp> holder;
  const Exp* exp = &exp_in;
  if (const auto* cl = std::get_if<c99::CompoundLiteral>(exp)) {
    holder = materialize_compound_literal(*cl, instrs);
    exp = &*holder;
  }
  // Each compound literal that is a direct operand of `exp` is replaced
  // by the `Var` it evaluates to, so the lvalue paths below, which look
  // for a `Var`, see one. An operator evaluates all its operands, so
  // doing it first is fine, except for the operands of `&&`, `||`, `?:`,
  // and `,`, which run only in their turn, and of `sizeof`, which never
  // runs. Those reach `translate_exp` one at a time, and the
  // `CompoundLiteral` case above evaluates each.
  const bool skip = asdl::isa<c99::Conditional, c99::Comma, c99::SizeOfExp>(*exp) ||
                    (std::holds_alternative<c99::Binary>(*exp) &&
                     asdl::isa<c99::LogicalAnd, c99::LogicalOr>(std::get<c99::Binary>(*exp).op));
  if (!skip) {
    bool any = false;
    for_each_direct_operand(*exp, [&](const asdl::Box<Exp>& operand) {
      any = any || std::holds_alternative<c99::CompoundLiteral>(*operand);
    });
    if (any) {
      Exp copy = *exp;
      for_each_direct_operand(copy, [&](asdl::Box<Exp>& operand) {
        if (const auto* cl = std::get_if<c99::CompoundLiteral>(&*operand)) {
          c99::Var v = materialize_compound_literal(*cl, instrs);
          *operand = std::move(v);
        }
      });
      holder = std::move(copy);
      exp = &*holder;
    }
  }

  switch (c99::kind(*exp)) {
    case EK::Constant:
      return tac::Constant{.const_ = to_tac_const(std::get<c99::Constant>(*exp).const_)};
    case EK::Cast: {
      const auto& c = std::get<c99::Cast>(*exp);
      // `(void)e` evaluates `e` for its side effects, and the value is
      // discarded.
      if (std::holds_alternative<c99::Void>(c.target_type)) {
        translate_exp(*c.exp, instrs);
        return std::nullopt;
      }
      std::optional<Val> inner_val = translate_exp(*c.exp, instrs);
      const std::optional<Type>& source = data_type_of(*c.exp);
      // A missing source type (a synthetic expression that bypassed the
      // type checker) leaves the value as it is.
      if (!source || *source == c.target_type) {
        return inner_val;
      }
      if (std::holds_alternative<c99::Bool>(strip_quals(c.target_type)) && is_zero_or_one(*c.exp)) {
        return emit_zero_or_one_to_bool(need(inner_val), *source, instrs);
      }
      return emit_cast(need(inner_val), *source, c.target_type, instrs);
    }
    case EK::Unary: {
      const auto& u = std::get<c99::Unary>(*exp);
      if (std::holds_alternative<c99::Plus>(u.op)) {
        // The type checker has already promoted the operand, which is
        // all `+` does.
        return translate_exp(*u.exp, instrs);
      }
      const Val src = translate_value(*u.exp, instrs);
      tac::Var dst = temp_var(u.data_type);
      const tac::Type_unary_operator tac_op = translate_unop(u.op);
      instrs.push_back(tac::Unary{.op = tac_op, .src = src, .dst = dst});
      return restore_padding(dst, u.data_type, nullptr, &tac_op, instrs);
    }
    case EK::Binary: {
      const auto& b = std::get<c99::Binary>(*exp);
      if (std::holds_alternative<c99::LogicalAnd>(b.op)) {
        return translate_short_circuit(*b.left, *b.right, instrs, false);
      }
      if (std::holds_alternative<c99::LogicalOr>(b.op)) {
        return translate_short_circuit(*b.left, *b.right, instrs, true);
      }
      // The left operand is translated first, so its temporaries get the
      // lower numbers.
      const Val src1 = translate_value(*b.left, instrs);
      const Val src2 = translate_value(*b.right, instrs);
      // Pointer arithmetic (C99 §6.5.6) scales the integer operand by
      // the size of what the pointer points to, and divides `ptr - ptr`
      // back to a count of elements.
      const std::optional<Type>& lt = data_type_of(*b.left);
      const std::optional<Type>& rt = data_type_of(*b.right);
      const bool l_ptr = lt && std::holds_alternative<c99::Pointer>(unqualified(*lt));
      const bool r_ptr = rt && std::holds_alternative<c99::Pointer>(unqualified(*rt));
      if (asdl::isa<c99::Add, c99::Subtract>(b.op) && (l_ptr || r_ptr)) {
        return translate_pointer_arithmetic(b.op, src1, src2, need_type(lt), need_type(rt),
                                            need_type(b.data_type), instrs);
      }
      // The Binary's type is the result type after the usual arithmetic
      // conversions, or `int` for a comparison.
      tac::Var dst = temp_var(b.data_type);
      const tac::Type_binary_operator tac_op = translate_binop(b.op);
      instrs.push_back(tac::Binary{.op = tac_op, .src1 = src1, .src2 = src2, .dst = dst});
      return restore_padding(dst, b.data_type, &tac_op, nullptr, instrs);
    }
    case EK::Var: {
      // A volatile scalar is read with a volatile load (see
      // `volatile_scalar`).
      const auto& v = std::get<c99::Var>(*exp);
      if (volatile_scalar(*exp)) {
        return read_volatile_var(v.name, instrs);
      }
      return tac::Var{.name = v.name};
    }
    case EK::SizeOfExp: {
      // `sizeof e` is a constant, and its operand isn't evaluated (C99
      // §6.5.3.4p2), so no instructions are emitted for it. The type
      // checker stamped the operand with its type before any decay.
      const auto& inner = *std::get<c99::SizeOfExp>(*exp).exp;
      const auto& t = data_type_of(inner);
      if (!t) {
        throw InternalError("type_checker should have stamped data_type on sizeof's inner expression");
      }
      return tac::Constant{.const_ = tac::ConstUInt{.value = sizeof_c99(*t, types)}};
    }
    case EK::SizeOfType:
      return tac::Constant{
          .const_ = tac::ConstUInt{.value = sizeof_c99(std::get<c99::SizeOfType>(*exp).target_type, types)}};
    case EK::Subscript: {
      // `a[i]` is `*(a + i)` (C99 §6.5.2.1p2). An element of a static
      // array of at most 256 bytes is an `IndexedLoad`, which
      // `tac_to_asm` lowers to absolute,Y addressing on the array's
      // label (`try_indexed_load_subscript`).
      const auto& s = std::get<c99::Subscript>(*exp);
      if (auto fast = try_indexed_load_subscript(*s.array, *s.index, s.data_type, instrs)) {
        return fast;
      }
      const Val addr = translate_subscript_address(*s.array, *s.index, instrs);
      tac::Var dst = temp_var(s.data_type);
      instrs.push_back(tac::Load{.src_ptr = addr, .dst = dst, .is_volatile = is_volatile_quals(s.data_type)});
      return dst;
    }
    case EK::Dot: {
      const auto& d = std::get<c99::Dot>(*exp);
      const Val addr = translate_dot_address(*d.operand, d.member, instrs);
      tac::Var dst = temp_var(d.data_type);
      instrs.push_back(tac::Load{.src_ptr = addr, .dst = dst, .is_volatile = is_volatile_quals(d.data_type)});
      return dst;
    }
    case EK::Arrow: {
      const auto& a = std::get<c99::Arrow>(*exp);
      const Val addr = translate_arrow_address(*a.operand, a.member, instrs);
      tac::Var dst = temp_var(a.data_type);
      instrs.push_back(tac::Load{.src_ptr = addr, .dst = dst, .is_volatile = is_volatile_quals(a.data_type)});
      return dst;
    }
    case EK::Assignment: {
      // Identifier resolution accepts five lvalues: a variable, `*p`,
      // `a[i]`, `s.m`, and `p->m`.
      const auto& a = std::get<c99::Assignment>(*exp);
      const Exp& lval = *a.lval;
      const Val rval_val = translate_value(*a.rval, instrs);
      if (volatile_scalar(lval)) {
        // The assignment's value is the value stored (C99 §6.5.16p3), so
        // `b = v = 5` doesn't read the volatile `v` back.
        write_volatile_var(std::get<c99::Var>(lval).name, rval_val, instrs);
        return rval_val;
      }
      if (const auto* v = std::get_if<c99::Var>(&lval)) {
        // The value is the variable, so `b = a = 5` copies `a` into `b`.
        tac::Var dst{.name = v->name};
        instrs.push_back(tac::Copy{.src = rval_val, .dst = dst});
        return dst;
      }
      // The volatility of the destination is on the lvalue's type: `*p`
      // has the pointee's type, with any `Volatile` of `p`'s
      // `Pointer(Volatile(T))`.
      const bool is_vol = is_volatile_quals(data_type_of(lval));
      switch (c99::kind(lval)) {
        case EK::Dereference: {
          // `*((uint8_t *)&obj + k)` is `((uint8_t *)&obj)[k]` and stores
          // as that subscript does.
          const Exp& ptr_exp = *std::get<c99::Dereference>(lval).exp;
          if (const auto sub = deref_as_subscript(ptr_exp);
              sub && try_indexed_store_subscript(sub->first, sub->second, rval_val, is_vol, instrs)) {
            return rval_val;
          }
          const Val ptr_val = translate_value(ptr_exp, instrs);
          instrs.push_back(tac::Store{.src = rval_val, .dst_ptr = ptr_val, .is_volatile = is_vol});
          return rval_val;
        }
        case EK::Subscript: {
          const auto& s = std::get<c99::Subscript>(lval);
          if (try_indexed_store_subscript(*s.array, *s.index, rval_val, is_vol, instrs)) {
            return rval_val;
          }
          const Val addr = translate_subscript_address(*s.array, *s.index, instrs);
          instrs.push_back(tac::Store{.src = rval_val, .dst_ptr = addr, .is_volatile = is_vol});
          return rval_val;
        }
        case EK::Dot: {
          const auto& d = std::get<c99::Dot>(lval);
          const Val addr = translate_dot_address(*d.operand, d.member, instrs);
          instrs.push_back(tac::Store{.src = rval_val, .dst_ptr = addr, .is_volatile = is_vol});
          return rval_val;
        }
        case EK::Arrow: {
          const auto& d = std::get<c99::Arrow>(lval);
          const Val addr = translate_arrow_address(*d.operand, d.member, instrs);
          instrs.push_back(tac::Store{.src = rval_val, .dst_ptr = addr, .is_volatile = is_vol});
          return rval_val;
        }
        default:
          throw InternalError(
              "assignment lval must be Var, Dereference, Subscript, Dot, or Arrow "
              "(identifier_resolution should have enforced this)");
      }
    }
    case EK::CompoundAssignment: {
      const auto& a = std::get<c99::CompoundAssignment>(*exp);
      return translate_compound_assign(a.op, *a.lval, *a.rval, a.intermediate_type, instrs);
    }
    case EK::Conditional: {
      // `c ? t : f` copies either value into one temporary, so the result
      // is a single Var. When both values are `void` (C99 §6.5.15p5),
      // there's no value and no copies.
      const auto& c = std::get<c99::Conditional>(*exp);
      const Val cond_val = translate_value(*c.condition, instrs);
      const std::string else_label = make_label("cond_else");
      const std::string end_label = make_label("cond_end");
      const bool is_void = c.data_type && std::holds_alternative<c99::Void>(*c.data_type);
      std::optional<tac::Var> dst;
      if (!is_void) {
        dst = temp_var(c.data_type);
      }
      instrs.push_back(tac::JumpIfFalse{.condition = cond_val, .target = else_label});
      const std::optional<Val> t_val = translate_exp(*c.true_clause, instrs);
      if (dst) {
        instrs.push_back(tac::Copy{.src = need(t_val), .dst = *dst});
      }
      instrs.push_back(tac::Jump{.target = end_label});
      instrs.push_back(tac::Label{.name = else_label});
      const std::optional<Val> f_val = translate_exp(*c.false_clause, instrs);
      if (dst) {
        instrs.push_back(tac::Copy{.src = need(f_val), .dst = *dst});
      }
      instrs.push_back(tac::Label{.name = end_label});
      if (dst) {
        return Val{*dst};
      }
      return std::nullopt;
    }
    case EK::Comma: {
      // C99 §6.5.17: the left operand's value is discarded, and the
      // right one's is the expression's.
      const auto& c = std::get<c99::Comma>(*exp);
      translate_exp(*c.left, instrs);
      return translate_exp(*c.right, instrs);
    }
    case EK::FunctionCall: {
      const auto& f = std::get<c99::FunctionCall>(*exp);
      if (builtin_functions::is_builtin(f.name)) {
        return translate_builtin(f, instrs);
      }
      return translate_call(f.hw_stack_save, f.data_type, &f.name, std::nullopt, f.args, instrs);
    }
    case EK::IndirectCall: {
      // The type checker left a `Var` when the callee comes down to a
      // name (`(*fp)(x)`), which calls it as `fp(x)` does. Otherwise the
      // callee is a pointer to a function, evaluated before the arguments
      // (C99 §6.5.2.2p10 leaves the order unspecified).
      const auto& c = std::get<c99::IndirectCall>(*exp);
      if (const auto* v = std::get_if<c99::Var>(&*c.callee)) {
        return translate_call(c.hw_stack_save, c.data_type, &v->name, std::nullopt, c.args, instrs);
      }
      if (c.hw_stack_save) {
        throw CompileError("RecursionSaveError", kSaveThroughPointer);
      }
      Val ptr = translate_value(*c.callee, instrs);
      return translate_call(c.hw_stack_save, c.data_type, nullptr, std::move(ptr), c.args, instrs);
    }
    case EK::Postfix: {
      // `a++` gives the old value.
      const auto& p = std::get<c99::Postfix>(*exp);
      return translate_incdec(p.op, *p.operand, instrs, true);
    }
    case EK::Prefix: {
      // `++a` gives the new value.
      const auto& p = std::get<c99::Prefix>(*exp);
      return translate_incdec(p.op, *p.operand, instrs, false);
    }
    case EK::Dereference: {
      // `*p` loads through the pointer into a temporary of the pointee's
      // type, which says whether the pointee is volatile (`p` of type
      // `Pointer(Volatile(T))` gives `*p` the type `Volatile(T)`). A
      // store through a pointer is the Assignment case's, which doesn't
      // translate its lvalue. `*((uint8_t *)&obj + k)` is
      // `((uint8_t *)&obj)[k]` (C99 §6.5.2.1p2) and reads as that
      // subscript does.
      const auto& d = std::get<c99::Dereference>(*exp);
      if (const auto sub = deref_as_subscript(*d.exp)) {
        if (auto fast = try_indexed_load_subscript(sub->first, sub->second, d.data_type, instrs)) {
          return fast;
        }
      }
      const Val ptr_val = translate_value(*d.exp, instrs);
      tac::Var dst = temp_var(d.data_type);
      instrs.push_back(tac::Load{.src_ptr = ptr_val, .dst = dst, .is_volatile = is_volatile_quals(d.data_type)});
      return dst;
    }
    case EK::AddressOf: {
      const auto& a = std::get<c99::AddressOf>(*exp);
      const Exp& inner = *a.exp;
      switch (c99::kind(inner)) {
        case EK::Var: {
          // `&x` is a `GetAddress` of the variable, for a local, a
          // parameter, or a static alike, since the asm `LoadAddress`
          // dispatches on the storage class.
          tac::Var dst = temp_var(a.data_type);
          instrs.push_back(
              tac::GetAddress{.operand = tac::Var{.name = std::get<c99::Var>(inner).name}, .dst = dst});
          return dst;
        }
        case EK::Dereference:
          // `&*e` is `e` (C99 §6.5.3.2p3), and neither operator is
          // evaluated.
          return translate_exp(*std::get<c99::Dereference>(inner).exp, instrs);
        case EK::Subscript: {
          // `&a[i]` is `a + i` (C99 §6.5.3.2p3). This shape comes from the
          // type checker's decay of a subscript of a multi-dimensional
          // array, which is an array itself.
          const auto& s = std::get<c99::Subscript>(inner);
          return translate_subscript_address(*s.array, *s.index, instrs);
        }
        case EK::Dot: {
          const auto& d = std::get<c99::Dot>(inner);
          return translate_dot_address(*d.operand, d.member, instrs);
        }
        case EK::Arrow: {
          const auto& d = std::get<c99::Arrow>(inner);
          return translate_arrow_address(*d.operand, d.member, instrs);
        }
        default:
          throw InternalError(
              "address-of operand must be Var, Dereference, Subscript, Dot, or Arrow "
              "(identifier_resolution / array decay should have enforced this)");
      }
    }
    default:
      throw InternalError("unexpected exp");
  }
}

// Lowers pointer arithmetic to plain TAC operations, by the operand
// types the type checker stamped. `ptr ± int` multiplies the integer by
// the size of what the pointer points to, unless that's 1, and adds or
// subtracts two 2-byte values. `ptr - ptr` subtracts the two pointers
// and divides the difference by that size, unless it's 1, which gives
// the count of elements as an `int`, the 2-byte signed `ptrdiff_t`. The
// type checker has already widened an integer operand to `int`, so every
// value here is 2 bytes wide.
Val Translator::translate_pointer_arithmetic(const c99::Type_binary_operator& op, const Val& src1,
                                             const Val& src2, const Type& lt_in, const Type& rt_in,
                                             const Type& result_type_in, Instrs& instrs) {
  const Type lt = strip_quals(lt_in);
  const Type rt = strip_quals(rt_in);
  const Type result_type = strip_quals(result_type_in);
  const bool l_ptr = std::holds_alternative<c99::Pointer>(lt);
  const bool r_ptr = std::holds_alternative<c99::Pointer>(rt);
  const auto pointee_size = [&](const Type& t) {
    return sizeof_c99(*std::get<c99::Pointer>(unqualified(t)).referenced_type, types);
  };
  if (l_ptr && r_ptr) {
    // `ptr + ptr` was rejected by the type checker, so this is a
    // subtraction.
    if (!std::holds_alternative<c99::Subtract>(op)) {
      throw InternalError("pointer + pointer");
    }
    const asdl::Int size = pointee_size(lt);
    tac::Var diff = temp_var(Type{c99::Int{}});
    instrs.push_back(tac::Binary{.op = tac::Subtract{}, .src1 = src1, .src2 = src2, .dst = diff});
    if (size == 1) {
      return diff;
    }
    tac::Var quot = temp_var(Type{c99::Int{}});
    instrs.push_back(tac::Binary{
        .op = tac::Divide{}, .src1 = diff, .src2 = tac_const_val(c99::Int{}, size), .dst = quot});
    return quot;
  }
  // The type checker rejected `int - ptr`, so the pointer is on the left
  // of a subtraction. An addition commutes, and the pointer goes on the
  // left of it too.
  const Val& ptr_val = l_ptr ? src1 : src2;
  Val int_val = l_ptr ? src2 : src1;
  const asdl::Int size = pointee_size(l_ptr ? lt : rt);
  if (size != 1) {
    tac::Var scaled = temp_var(Type{c99::Int{}});
    instrs.push_back(tac::Binary{
        .op = tac::Multiply{}, .src1 = int_val, .src2 = tac_const_val(c99::Int{}, size), .dst = scaled});
    int_val = scaled;
  }
  tac::Var dst = temp_var(result_type);
  instrs.push_back(tac::Binary{.op = translate_binop(op), .src1 = ptr_val, .src2 = int_val, .dst = dst});
  return dst;
}

}  // namespace c6502::c99_to_tac
