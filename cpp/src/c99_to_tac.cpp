// c99_to_tac.py: the translation of a type-checked c99_ast program
// into three-address code (tac_ast).
//
// Every C99 expression becomes a TAC `val`, either a Constant or a Var
// holding the result of an earlier instruction. Compound expressions
// are flattened: nested operators put their intermediate results in
// fresh temporaries and emit the TAC instruction that computes each.
//
// A TAC program is `Program(top_level*)`, where a `top_level` is a
// `Function` or a `StaticVariable`. The functions come from walking the
// c99 declarations in source order, and the static variables from the
// symbol table after that walk. No TAC is emitted for a file-scope
// variable or a block-scope `extern` or `static` one where it is
// declared, since those objects have static storage duration, and their
// `StaticVariable` entries come from the symbol table, which holds each
// object's initial value after the merging of its redeclarations.
//
// The Translator owns the counter of the temporaries (`%0`, `%1`, ...)
// and a separate counter of the labels (`.and_false@0`, `.if_end@1`,
// ...). Both run over the whole program, so the order of the lowering
// is part of its output. The instruction list of the function being
// translated is passed down explicitly, so there is no implicit
// current function on the Translator.
//
// The lowering of each construct:
//
// - A function definition becomes a TAC `Function` with the
//   instructions of its body. When the body doesn't end in a `Ret`, a
//   `Ret` of 0 of the return type is appended (C99 §5.1.2.2.3 gives
//   that for `main`, and c6502 applies it to every function so that
//   every TAC function ends in a `Ret`), or `Ret(None)` for a `void`
//   function or one that returns a struct.
// - A declaration `T x = e;` evaluates `e` and copies it into `x`, the
//   same TAC as the assignment `x = e`, and a bare `T x;` emits nothing.
//   An aggregate's initializer list is stored into its bytes
//   (`emit_initializer`).
// - `return e;` is `Ret(e)`, and in a function that returns a struct it
//   stores `e` through the hidden `.sret.<name>` parameter, the address
//   of the caller's return slot, and then is `Ret(None)`.
// - `if` evaluates the condition and jumps around the then-branch with
//   `JumpIfFalse` to `.if_end@N`, or to `.if_else@N` with a `Jump` past
//   the else-branch.
// - `goto` and a labeled statement keep the label that label resolution
//   made, `.<function>@<label>`, a dasm local label. The `@` keeps those
//   apart from the labels the translator makes, whose part after `@` is
//   a run of digits.
// - A loop's labels derive from the base label `.loop@N` that loop
//   labeling gave it: `_start`, `_continue`, and `_break`, with
//   `kKeepLoopMark` before them for a loop marked `[[c6502::keep]]`.
//   `while` tests at its continue label, `do`-`while` tests between the
//   body and the break label with its continue label before the test,
//   and `for` runs its init once, tests after its start label (a missing
//   condition is always true and is left out), and runs its post clause
//   after its continue label, so `continue` still runs it.
// - `switch` evaluates the control once, compares it with each case's
//   constant (the type checker converted each to the promoted control
//   type) and jumps to the case's label, and then jumps to the default
//   or past the body. The body plants the labels of its cases where they
//   appear, so fall-through is implicit.
// - A constant is the TAC constant of the same type. A variable is
//   `Var(name)` with the name identifier resolution made, `@N.orig`,
//   which shares a namespace with the temporaries `%N`, though neither
//   can be a C identifier. A volatile scalar is read with a volatile
//   `IndexedLoad(name, 0)` and written with a volatile
//   `IndexedSymbolStore`, which every TAC pass keeps.
// - Unary and binary operators evaluate their operands left to right,
//   so the left operand's temporaries get the lower numbers, and emit
//   one TAC instruction into a temporary of the expression's type.
//   Unary `+` is its operand, which the type checker promoted.
// - `a = b` copies `b` into a variable and stores it through the address
//   of any other lvalue, and returns the value, so `b = a = 5` composes.
// - `++`, `--`, and compound assignment compute the lvalue's address
//   once, so `++arr[--i]` and `arr[i++] += 1` have their side effects
//   once, load the value, apply the operation at the type the type
//   checker gave, and store the result. A pointer operand's step is
//   scaled by the size of what it points to (C99 §6.5.6.8).
// - A cast between integers of the same width and different signedness
//   copies the value into a temporary of the target type (a constant is
//   retagged), since the later passes take the signedness of `>>`, `/`,
//   `%`, an ordering, and a widening from the operand's type. A wider
//   target is a `SignExtend` or a `ZeroExtend` by the source's
//   signedness, a narrower one a `Truncate`, and a floating source or
//   target an `IntToFloat`, `FloatToInt`, `FloatToDouble`, or the other
//   conversions, which fold for a constant operand. A cast to `bool` is
//   `x != 0`, or the low byte of a value that is already 0 or 1.
// - A call evaluates its arguments in source order and emits a
//   `FunctionCall`, or an `IndirectCall` through a pointer, into a
//   temporary of the result type. A call that returns a struct passes
//   the address of a temporary slot as a hidden first argument. The
//   builtins lower to TAC operators (`translate_builtin`).
// - `a[i]` is the address `a + i * sizeof *a` and a `Load` through it,
//   or an `IndexedLoad` of a static array of at most 256 bytes. The type
//   checker made each decay of an array an `AddressOf`, so `arr[i]` and
//   `ptr[i]` have the same shape.
// - `e.m` and `p->m` add the member's offset to the struct's address and
//   load through it.
// - A compound literal declares its object where evaluation reaches it
//   and is then that object's `Var`.
// - `c ? t : f` copies either value into one temporary between the
//   labels `.cond_else@N` and `.cond_end@N`, and `,` evaluates its left
//   operand for its side effects and gives its right one.
// - `&&` and `||` are control flow, with `.and_false@N` and `.and_end@N`
//   or `.or_true@N` and `.or_end@N`, and give an `int` 0 or 1.
// - `sizeof` is a constant, and its operand isn't evaluated.
//
// The passes after this one rely on these properties of the TAC.
//
// - Every temporary is in the symbol table, as a `LocalAttr` whose type
//   is the type of the expression it holds, without qualifiers, so that
//   `tac_to_asm` and `replace_pseudoregisters` can size it.
// - Parameter names keep the form `@N.orig` that identifier resolution
//   gave them.
// - A TAC integer constant's class gives its width and signedness, and
//   `tac_const_for` builds the constant of a type.
// - Pointer arithmetic is scaled in one place,
//   `translate_pointer_arithmetic`: `ptr ± n` multiplies `n` by the size
//   of what `ptr` points to, and `ptr - ptr` divides the difference of
//   the addresses by it.
//
// The translation changes the program and the tables it is given in
// place, as Python's does. `short` and `nullptr_t` are erased before it
// and `bool` and the bit-precise types after it, so that no pass after
// this one sees them, and compile.py hands the changed program to
// `select_abi`.

#include "c99_to_tac.h"

#include <functional>
#include <string>
#include <utility>

#include "bit_precise.h"
#include "builtin_functions.h"
#include "c99_to_tac_translator.h"
#include "errors.h"
#include "fp_arith.h"
#include "passes/recursion_saves.h"
#include "pyrepr.h"

namespace c6502::c99_to_tac {

namespace {

using TK = c99::Kind_data_type;
using EK = c99::Kind_exp;

// --- Rewriting the types of the program and the tables ---

// Rewrites `t` in place, at any depth: where `leaf` replaces a type, the
// replacement stands, and everywhere else the walk goes on into the
// types inside it.
void rewrite_type(Type& t, const std::function<bool(Type&)>& leaf) {
  if (leaf(t)) {
    return;
  }
  switch (c99::kind(t)) {
    case TK::FunType: {
      auto& f = std::get<c99::FunType>(t);
      for (Type& p : f.params) {
        rewrite_type(p, leaf);
      }
      rewrite_type(*f.ret, leaf);
      return;
    }
    case TK::Pointer:
      rewrite_type(*std::get<c99::Pointer>(t).referenced_type, leaf);
      return;
    case TK::Array:
      rewrite_type(*std::get<c99::Array>(t).element_type, leaf);
      return;
    case TK::Const:
      rewrite_type(*std::get<c99::Const>(t).referenced_type, leaf);
      return;
    case TK::Volatile:
      rewrite_type(*std::get<c99::Volatile>(t).referenced_type, leaf);
      return;
    default:
      return;
  }
}

// Replaces `short` with `int` and `unsigned short` with `unsigned int`.
bool short_leaf(Type& t) {
  if (c99::kind(t) == TK::Short) {
    t = c99::Int{};
    return true;
  }
  if (c99::kind(t) == TK::UShort) {
    t = c99::UInt{};
    return true;
  }
  return false;
}

// Replaces `nullptr_t` with `void *`.
bool nullptr_leaf(Type& t) {
  if (c99::kind(t) == TK::NullptrT) {
    t = c99::Pointer{.referenced_type = c99::Void{}};
    return true;
  }
  return false;
}

// Replaces `bool` with `unsigned char`.
bool bool_leaf(Type& t) {
  if (c99::kind(t) == TK::Bool) {
    t = c99::UChar{};
    return true;
  }
  return false;
}

// Replaces a bit-precise type with the integer type of its storage.
bool bit_precise_leaf(Type& t) {
  if (const auto* b = std::get_if<c99::BitInt>(&t)) {
    t = bit_precise::storage_type(*b);
    return true;
  }
  return false;
}

// Returns `(void *)0`, type-checked, which is what `nullptr` becomes.
Exp null_void_pointer() {
  const Type void_p = c99::Pointer{.referenced_type = c99::Void{}};
  return c99::Cast{
      .target_type = void_p,
      .exp = c99::Constant{.const_ = c99::ConstInt{.value = 0}, .data_type = c99::Int{}},
      .data_type = void_p,
  };
}

// Walks every type of a c99 program in place, as Python's
// `_short_to_int` and `_nullptr_to_void_pointer` walk every field of
// every node. With `nullptr_exps`, each `nullptr` constant is replaced
// by `(void *)0`, and the walk doesn't go into the replacement.
struct ProgramWalk {
  std::function<bool(Type&)> leaf;
  bool nullptr_exps = false;

  void type(Type& t) const { rewrite_type(t, leaf); }
  void type(std::optional<Type>& t) const {
    if (t) {
      type(*t);
    }
  }

  void exp(Exp& e) const {
    if (nullptr_exps) {
      if (const auto* c = std::get_if<c99::Constant>(&e)) {
        if (std::holds_alternative<c99::ConstNullptr>(c->const_)) {
          e = null_void_pointer();
          return;
        }
      }
    }
    switch (c99::kind(e)) {
      case EK::Constant:
        type(std::get<c99::Constant>(e).data_type);
        return;
      case EK::String:
        return;
      case EK::Var:
        type(std::get<c99::Var>(e).data_type);
        return;
      case EK::Cast: {
        auto& x = std::get<c99::Cast>(e);
        type(x.target_type);
        exp(*x.exp);
        type(x.data_type);
        return;
      }
      case EK::Unary: {
        auto& x = std::get<c99::Unary>(e);
        exp(*x.exp);
        type(x.data_type);
        return;
      }
      case EK::Binary: {
        auto& x = std::get<c99::Binary>(e);
        exp(*x.left);
        exp(*x.right);
        type(x.data_type);
        return;
      }
      case EK::Assignment: {
        auto& x = std::get<c99::Assignment>(e);
        exp(*x.lval);
        exp(*x.rval);
        type(x.data_type);
        return;
      }
      case EK::CompoundAssignment: {
        auto& x = std::get<c99::CompoundAssignment>(e);
        exp(*x.lval);
        exp(*x.rval);
        type(x.intermediate_type);
        type(x.data_type);
        return;
      }
      case EK::Postfix: {
        auto& x = std::get<c99::Postfix>(e);
        exp(*x.operand);
        type(x.data_type);
        return;
      }
      case EK::Prefix: {
        auto& x = std::get<c99::Prefix>(e);
        exp(*x.operand);
        type(x.data_type);
        return;
      }
      case EK::Conditional: {
        auto& x = std::get<c99::Conditional>(e);
        exp(*x.condition);
        exp(*x.true_clause);
        exp(*x.false_clause);
        type(x.data_type);
        return;
      }
      case EK::FunctionCall: {
        auto& x = std::get<c99::FunctionCall>(e);
        for (Exp& a : x.args) {
          exp(a);
        }
        type(x.data_type);
        return;
      }
      case EK::IndirectCall: {
        auto& x = std::get<c99::IndirectCall>(e);
        exp(*x.callee);
        for (Exp& a : x.args) {
          exp(a);
        }
        type(x.data_type);
        return;
      }
      case EK::Dereference: {
        auto& x = std::get<c99::Dereference>(e);
        exp(*x.exp);
        type(x.data_type);
        return;
      }
      case EK::AddressOf: {
        auto& x = std::get<c99::AddressOf>(e);
        exp(*x.exp);
        type(x.data_type);
        return;
      }
      case EK::Subscript: {
        auto& x = std::get<c99::Subscript>(e);
        exp(*x.array);
        exp(*x.index);
        type(x.data_type);
        return;
      }
      case EK::InitList: {
        auto& x = std::get<c99::InitList>(e);
        for (Exp& a : x.items) {
          exp(a);
        }
        type(x.data_type);
        return;
      }
      case EK::Designation:
        exp(*std::get<c99::Designation>(e).init);
        return;
      case EK::SizeOfExp: {
        auto& x = std::get<c99::SizeOfExp>(e);
        exp(*x.exp);
        type(x.data_type);
        return;
      }
      case EK::SizeOfType: {
        auto& x = std::get<c99::SizeOfType>(e);
        type(x.target_type);
        type(x.data_type);
        return;
      }
      case EK::Dot: {
        auto& x = std::get<c99::Dot>(e);
        exp(*x.operand);
        type(x.data_type);
        return;
      }
      case EK::Arrow: {
        auto& x = std::get<c99::Arrow>(e);
        exp(*x.operand);
        type(x.data_type);
        return;
      }
      case EK::Comma: {
        auto& x = std::get<c99::Comma>(e);
        exp(*x.left);
        exp(*x.right);
        type(x.data_type);
        return;
      }
      case EK::CompoundLiteral: {
        auto& x = std::get<c99::CompoundLiteral>(e);
        type(x.target_type);
        exp(*x.init);
        type(x.data_type);
        return;
      }
    }
  }

  void var_decl(c99::Type_var_decl& vd) const {
    if (vd.init) {
      exp(*vd.init);
    }
    type(vd.data_type);
  }

  void block(c99::Type_block& b) const {
    for (c99::Type_block_item& item : std::get<c99::Block>(b).block_item) {
      if (auto* s = std::get_if<c99::S>(&item)) {
        statement(*s->statement);
      } else {
        declaration(*std::get<c99::D>(item).declaration);
      }
    }
  }

  void statement(c99::Type_statement& s) const {
    using SK = c99::Kind_statement;
    switch (c99::kind(s)) {
      case SK::Return: {
        auto& x = std::get<c99::Return>(s);
        if (x.exp) {
          exp(*x.exp);
        }
        return;
      }
      case SK::Expression:
        exp(std::get<c99::Expression>(s).exp);
        return;
      case SK::IfStmt: {
        auto& x = std::get<c99::IfStmt>(s);
        exp(x.condition);
        statement(*x.then_clause);
        if (x.else_clause) {
          statement(**x.else_clause);
        }
        return;
      }
      case SK::Compound:
        block(*std::get<c99::Compound>(s).block);
        return;
      case SK::BreakStmt:
      case SK::ContinueStmt:
      case SK::Goto:
      case SK::Null:
        return;
      case SK::WhileStmt: {
        auto& x = std::get<c99::WhileStmt>(s);
        exp(x.condition);
        statement(*x.body);
        return;
      }
      case SK::DoWhileStmt: {
        auto& x = std::get<c99::DoWhileStmt>(s);
        statement(*x.body);
        exp(x.condition);
        return;
      }
      case SK::ForStmt: {
        auto& x = std::get<c99::ForStmt>(s);
        if (auto* d = std::get_if<c99::InitDecl>(&x.init)) {
          for (c99::Type_var_decl& vd : d->var_decls) {
            var_decl(vd);
          }
        } else if (auto& ie = std::get<c99::InitExp>(x.init); ie.exp) {
          exp(*ie.exp);
        }
        if (x.condition) {
          exp(*x.condition);
        }
        if (x.post_clause) {
          exp(*x.post_clause);
        }
        statement(*x.body);
        return;
      }
      case SK::SwitchStmt: {
        auto& x = std::get<c99::SwitchStmt>(s);
        exp(x.control);
        statement(*x.body);
        for (c99::Type_switch_case& c : x.cases) {
          exp(c.value);
        }
        type(x.promoted_type);
        return;
      }
      case SK::CaseStmt: {
        auto& x = std::get<c99::CaseStmt>(s);
        exp(x.value);
        statement(*x.body);
        return;
      }
      case SK::DefaultStmt:
        statement(*std::get<c99::DefaultStmt>(s).body);
        return;
      case SK::LabeledStmt:
        statement(*std::get<c99::LabeledStmt>(s).statement);
        return;
    }
  }

  void declaration(c99::Type_declaration& d) const {
    using DK = c99::Kind_declaration;
    switch (c99::kind(d)) {
      case DK::FunctionDecl: {
        auto& fd = *std::get<c99::FunctionDecl>(d).function_decl;
        if (fd.body) {
          block(**fd.body);
        }
        type(fd.data_type);
        return;
      }
      case DK::VarDecl:
        var_decl(std::get<c99::VarDecl>(d).var_decl);
        return;
      case DK::StructDecl:
        for (c99::Type_member_decl& m : std::get<c99::StructDecl>(d).struct_decl.members) {
          type(m.data_type);
        }
        return;
    }
  }

  void program(c99::Program& prog) const {
    for (c99::Type_declaration& d : prog.declaration) {
      declaration(d);
    }
  }
};

// Rewrites every symbol's type and every member type of the layouts.
void rewrite_tables(SymbolTable& symbols, TypeTable* types, const std::function<bool(Type&)>& leaf) {
  for (auto& [name, sym] : symbols) {
    rewrite_type(sym.type, leaf);
  }
  if (types != nullptr) {
    for (auto& [tag, layout] : *types) {
      for (passes::MemberInfo& m : layout.members) {
        rewrite_type(m.type, leaf);
      }
    }
  }
}

// Rewrites the types of the top-level function and variable
// declarations, which `abi_selection` reads.
void rewrite_top_level(c99::Program& prog, const std::function<bool(Type&)>& leaf) {
  for (c99::Type_declaration& d : prog.declaration) {
    if (auto* f = std::get_if<c99::FunctionDecl>(&d)) {
      rewrite_type(f->function_decl->data_type, leaf);
    } else if (auto* v = std::get_if<c99::VarDecl>(&d)) {
      rewrite_type(v->var_decl.data_type, leaf);
    }
  }
}

// Replaces `short` with `int` and `unsigned short` with `unsigned int`
// in the program, the symbol table, and the struct and union layouts,
// before translating. With a 16-bit `int` they're the same width, and
// the type checker has already applied the promotions, in which `short`
// goes to `int` and `unsigned short` to `unsigned int`, so no pass after
// the type checker needs a `short` of its own.
void erase_short(c99::Program& prog, SymbolTable& symbols, TypeTable* types) {
  ProgramWalk{.leaf = short_leaf}.program(prog);
  rewrite_tables(symbols, types, short_leaf);
}

// Replaces `nullptr_t` with `void *` and `nullptr` with `(void *)0` in
// the program, the symbol table, and the struct and union layouts,
// before translating. The type checker has already checked every use
// of `nullptr_t` and written each of its conversions as a Cast. A
// `nullptr_t` value has a null `void *`'s representation (C23
// §7.21.2p3), so as a `void *` each of those Casts converts a null
// pointer to a null pointer or to `false`. `nullptr` is then the
// constant 0 from here on, which the optimizer folds like any other.
void erase_nullptr(c99::Program& prog, SymbolTable& symbols, TypeTable* types) {
  ProgramWalk{.leaf = nullptr_leaf, .nullptr_exps = true}.program(prog);
  rewrite_tables(symbols, types, nullptr_leaf);
}

// Replaces `bool` with `unsigned char` everywhere the passes after this
// one read c99 types: the symbol table, the struct and union layouts,
// and the types of the top-level declarations (which `abi_selection`
// reads). By now every conversion to `bool` has become a `!= 0` in the
// TAC, and what's left of the type is a byte holding 0 or 1, so the
// back end needs no `bool` of its own. The expression nodes keep their
// `bool`, so translating the program again still lowers its
// conversions. A function returning `bool` is marked `returns_bool`,
// and an object of type `bool` `was_bool`, since the byte holds 0 or 1,
// which `!` can flip with an `^ 1` (passes/optimization/lnot_jump_fold.py).
void erase_bool(c99::Program& prog, SymbolTable& symbols, TypeTable* types) {
  for (auto& [name, sym] : symbols) {
    if (auto* f = std::get_if<passes::FunAttr>(&sym.attrs)) {
      if (const auto* ft = std::get_if<c99::FunType>(&sym.type);
          ft != nullptr && c99::kind(unqualified(*ft->ret)) == TK::Bool) {
        f->returns_bool = true;
      }
    }
    if (c99::kind(unqualified(sym.type)) == TK::Bool) {
      if (auto* l = std::get_if<passes::LocalAttr>(&sym.attrs)) {
        l->was_bool = true;
      } else if (auto* s = std::get_if<passes::StaticAttr>(&sym.attrs)) {
        s->was_bool = true;
      }
    }
    rewrite_type(sym.type, bool_leaf);
  }
  if (types != nullptr) {
    for (auto& [tag, layout] : *types) {
      for (passes::MemberInfo& m : layout.members) {
        rewrite_type(m.type, bool_leaf);
      }
    }
  }
  rewrite_top_level(prog, bool_leaf);
}

// Replaces each bit-precise type with the integer type of its storage
// everywhere the passes after this one read c99 types: the symbol
// table, the struct and union layouts, and the types of the top-level
// declarations. By now the TAC has brought the padding bits back to the
// value's sign or zero extension after every operation that can change
// them, so the back end reads only the byte count and the signedness:
// `_BitInt(12)` is an `int` from here on, and `unsigned _BitInt(20)` a
// 3-byte `BitInt(24)`. The expression nodes keep their bit-precise
// types, so translating the program again still restores the padding.
void erase_bit_precise(c99::Program& prog, SymbolTable& symbols, TypeTable* types) {
  rewrite_tables(symbols, types, bit_precise_leaf);
  rewrite_top_level(prog, bit_precise_leaf);
}

// --- Static initializers ---

// Returns Python's `int(v)` of a value tree's number: the integer, or a
// float truncated toward zero. Throws `InternalError` for anything else,
// and for an infinity or a NaN, where Python raises.
asdl::Int py_int(const passes::ValueTree& v) {
  using VK = passes::ValueTree::Kind;
  if (v.kind == VK::kInt) {
    return v.integer;
  }
  if (v.kind == VK::kFloat) {
    if (const auto t = fp_arith::double_bits_to_int(asdl::Int(v.float_bits))) {
      if (const auto i = t->exact()) {
        return *i;
      }
    }
    throw InternalError("int() of a float that has no integer value");
  }
  throw InternalError("int() of a value tree that isn't a number");
}

passes::ValueTree int_tree(asdl::Int v) {
  return passes::ValueTree{.kind = passes::ValueTree::Kind::kInt, .integer = v};
}

passes::ValueTree tuple_tree(std::vector<passes::ValueTree> items) {
  return passes::ValueTree{.kind = passes::ValueTree::Kind::kTuple, .items = std::move(items)};
}

// Returns the width in bits of the declared integer type of a static's
// initializer, by which the value is reduced (Python's `_TRUNC_BITS`).
int trunc_bits(const Type& t) {
  switch (c99::kind(t)) {
    case TK::Char:
    case TK::SChar:
    case TK::UChar:
    case TK::Bool:
      return 8;
    case TK::Int:
    case TK::UInt:
      return 16;
    case TK::Long:
    case TK::ULong:
      return 32;
    case TK::LongLong:
    case TK::ULongLong:
      return 64;
    default:
      throw InternalError("no static truncation width for this type");
  }
}

// Reduces `value` to fit the declared integer type's width as a
// nonnegative bit pattern, the form `IntInit`, `LongInit`, and
// `LongLongInit` carry to asm_emit's `dc.b`, `dc.w`, and `dc.l`. A
// negative value wraps to its two's-complement pattern at that width
// (C99 §6.3.1.3).
asdl::Int truncate_int_for_static(const Type& t, const passes::ValueTree& value) {
  return py_int(value) & ((asdl::Int(1) << trunc_bits(t)) - 1);
}

// Builds the TAC `static_init` holding `value`, of the variant that
// matches the declared type. The integer side of the `static_init` sum
// keeps signedness beside width, and the floating side keeps Float and
// Double apart because their bit patterns differ. A floating static's
// value is already its IEEE 754 bit pattern, since the type checker's
// `_const_init_value` converts its initializer. An `AddressInit` value
// (`&otherstatic`) initializes only a pointer, which the type checker
// has checked, so another declared type is a bug.
tac::Type_static_init tac_static_init_for(const Type& t, const passes::ValueTree& value) {
  if (value.kind == passes::ValueTree::Kind::kAddress) {
    if (!std::holds_alternative<c99::Pointer>(t)) {
      throw InternalError(
          "AddressInit value can only initialize a pointer-typed static; got another "
          "declared type");
    }
    return tac::AddressInit{.name = value.address.name, .offset = value.address.offset};
  }
  switch (c99::kind(t)) {
    case TK::SChar:
      return tac::CharInit{.value = truncate_int_for_static(t, value)};
    case TK::Char:
    case TK::UChar:
    case TK::Bool:
      // Plain `char` is unsigned in c6502, the same bytes as `unsigned
      // char`. The type checker already gave a `bool` 0 or 1.
      return tac::UCharInit{.value = truncate_int_for_static(t, value)};
    case TK::Int:
      return tac::IntInit{.value = truncate_int_for_static(t, value)};
    case TK::UInt:
      return tac::UIntInit{.value = truncate_int_for_static(t, value)};
    case TK::Long:
      return tac::LongInit{.value = truncate_int_for_static(t, value)};
    case TK::ULong:
      return tac::ULongInit{.value = truncate_int_for_static(t, value)};
    case TK::LongLong:
      return tac::LongLongInit{.value = truncate_int_for_static(t, value)};
    case TK::ULongLong:
      return tac::ULongLongInit{.value = truncate_int_for_static(t, value)};
    case TK::Float:
      return tac::FloatInit{.bits = py_int(value)};
    case TK::Double:
      return tac::DoubleInit{.bits = py_int(value)};
    case TK::Pointer:
      // A pointer is 2 bytes, as `unsigned int` is, and a null pointer
      // is 0x0000.
      return tac::UIntInit{.value = truncate_int_for_static(c99::UInt{}, value)};
    default:
      throw InternalError("static-storage object can't have a non-object type");
  }
}

// Returns the zero value tree of a tentative definition's type: 0 for a
// scalar (the bits of +0.0 are all zero too), a tuple of zeros for an
// array, a tuple of each member's zero for a struct, and of the first
// member's for a union. The type checker gives `static T x;` the same
// shape, so `flat_static_init` reads both alike.
passes::ValueTree zero_init_value(const Type& t_in, const TypeTable* types) {
  const Type& t = unqualified(t_in);
  if (const auto* a = std::get_if<c99::Array>(&t)) {
    std::vector<passes::ValueTree> items;
    const passes::ValueTree elem = zero_init_value(*a->element_type, types);
    items.assign(static_cast<std::size_t>(a->size), elem);
    return tuple_tree(std::move(items));
  }
  const bool is_struct = std::holds_alternative<c99::Structure>(t);
  const bool is_union = std::holds_alternative<c99::Union>(t);
  if (is_struct || is_union) {
    if (types == nullptr) {
      return tuple_tree({});
    }
    const std::string& tag =
        is_struct ? std::get<c99::Structure>(t).tag : std::get<c99::Union>(t).tag;
    const passes::StructLayout* layout = types->find(tag);
    if (layout == nullptr || !layout->complete) {
      return tuple_tree({});
    }
    if (is_union) {
      if (layout->members.empty()) {
        return tuple_tree({});
      }
      return tuple_tree({zero_init_value(layout->members[0].type, types)});
    }
    std::vector<passes::ValueTree> items;
    for (const passes::MemberInfo& m : layout->members) {
      items.push_back(zero_init_value(m.type, types));
    }
    return tuple_tree(std::move(items));
  }
  return int_tree(0);
}

// Returns the bytes `it` lays down in memory, which pads a union whose
// first member is smaller than the union.
asdl::Int static_init_byte_count(const tac::Type_static_init& it) {
  using SK = tac::Kind_static_init;
  switch (tac::kind(it)) {
    case SK::CharInit:
    case SK::UCharInit:
      return 1;
    case SK::IntInit:
    case SK::UIntInit:
      return 2;
    case SK::LongInit:
    case SK::ULongInit:
      return 4;
    case SK::LongLongInit:
    case SK::ULongLongInit:
      return 8;
    case SK::FloatInit:
      return 4;
    case SK::DoubleInit:
      return 8;
    case SK::AddressInit:
      return 2;
    case SK::ZeroInit:
      return std::get<tac::ZeroInit>(it).bytes;
    case SK::StringInit:
      return std::get<tac::StringInit>(it).bytes;
    default:
      throw InternalError("unknown static_init");
  }
}

// Returns the byte count of `item` when its bytes are all zero, or
// none. An integer zero and a floating +0.0 qualify. An `AddressInit`
// never does, since `&name` is resolved at link time to an address that
// may or may not be zero.
std::optional<asdl::Int> zero_byte_count(const tac::Type_static_init& item) {
  using SK = tac::Kind_static_init;
  switch (tac::kind(item)) {
    case SK::CharInit:
      return std::get<tac::CharInit>(item).value == 0 ? std::optional<asdl::Int>(1) : std::nullopt;
    case SK::UCharInit:
      return std::get<tac::UCharInit>(item).value == 0 ? std::optional<asdl::Int>(1)
                                                       : std::nullopt;
    case SK::IntInit:
      return std::get<tac::IntInit>(item).value == 0 ? std::optional<asdl::Int>(2) : std::nullopt;
    case SK::UIntInit:
      return std::get<tac::UIntInit>(item).value == 0 ? std::optional<asdl::Int>(2) : std::nullopt;
    case SK::LongInit:
      return std::get<tac::LongInit>(item).value == 0 ? std::optional<asdl::Int>(4) : std::nullopt;
    case SK::ULongInit:
      return std::get<tac::ULongInit>(item).value == 0 ? std::optional<asdl::Int>(4)
                                                       : std::nullopt;
    case SK::LongLongInit:
      return std::get<tac::LongLongInit>(item).value == 0 ? std::optional<asdl::Int>(8)
                                                          : std::nullopt;
    case SK::ULongLongInit:
      return std::get<tac::ULongLongInit>(item).value == 0 ? std::optional<asdl::Int>(8)
                                                           : std::nullopt;
    case SK::FloatInit:
      return std::get<tac::FloatInit>(item).bits == 0 ? std::optional<asdl::Int>(4) : std::nullopt;
    case SK::DoubleInit:
      return std::get<tac::DoubleInit>(item).bits == 0 ? std::optional<asdl::Int>(8)
                                                       : std::nullopt;
    case SK::ZeroInit:
      return std::get<tac::ZeroInit>(item).bytes;
    default:
      return std::nullopt;
  }
}

// Merges each run of zero items into one `ZeroInit(N)`, N the sum of
// the run's sizes.
std::vector<tac::Type_static_init> coalesce_zero_inits(std::vector<tac::Type_static_init> items) {
  std::vector<tac::Type_static_init> out;
  asdl::Int pending = 0;
  for (tac::Type_static_init& item : items) {
    if (const auto b = zero_byte_count(item)) {
      pending += *b;
      continue;
    }
    if (pending > 0) {
      out.push_back(tac::ZeroInit{.bytes = pending});
      pending = 0;
    }
    out.push_back(std::move(item));
  }
  if (pending > 0) {
    out.push_back(tac::ZeroInit{.bytes = pending});
  }
  return out;
}

// Returns the UTF-8 of the code point `cp`, which is below 256, as the
// C++ string of a Python `str` holds it.
void append_code_point(std::string& out, unsigned cp) {
  if (cp < 0x80) {
    out += static_cast<char>(cp);
  } else {
    out += static_cast<char>(0xC0 | (cp >> 6));
    out += static_cast<char>(0x80 | (cp & 0x3F));
  }
}

void flat_static_init_raw(const Type& t_in, const passes::ValueTree& value, const TypeTable* types,
                          std::vector<tac::Type_static_init>& out) {
  using VK = passes::ValueTree::Kind;
  const Type& t = unqualified(t_in);
  if (const auto* a = std::get_if<c99::Array>(&t)) {
    if (value.kind != VK::kTuple || asdl::Int(value.items.size()) != a->size) {
      throw InternalError("array static init shape mismatch");
    }
    // An array of characters, at any depth of nesting, is one
    // `StringInit(str, bytes=N)`, the same bytes as an init per element
    // and more compact in the listing, or a `ZeroInit(N)` when every
    // byte is zero, which the asm writes as one `ds.b N`.
    if (is_char_element(*a->element_type)) {
      std::string s;
      bool all_zero = true;
      for (const passes::ValueTree& b : value.items) {
        const auto byte = static_cast<unsigned>(py_int(b) & 0xFF);
        all_zero = all_zero && byte == 0;
        append_code_point(s, byte);
      }
      if (all_zero) {
        out.push_back(tac::ZeroInit{.bytes = a->size});
      } else {
        out.push_back(tac::StringInit{.str = std::move(s), .bytes = a->size});
      }
      return;
    }
    for (const passes::ValueTree& elem : value.items) {
      flat_static_init_raw(*a->element_type, elem, types, out);
    }
    return;
  }
  const bool is_struct = std::holds_alternative<c99::Structure>(t);
  const bool is_union = std::holds_alternative<c99::Union>(t);
  if (is_struct || is_union) {
    if (types == nullptr) {
      out.push_back(tac::ZeroInit{.bytes = 0});
      return;
    }
    const std::string& tag =
        is_struct ? std::get<c99::Structure>(t).tag : std::get<c99::Union>(t).tag;
    const passes::StructLayout* layout = types->find(tag);
    if (layout == nullptr || !layout->complete) {
      out.push_back(tac::ZeroInit{.bytes = 0});
      return;
    }
    std::vector<const passes::MemberInfo*> members;
    for (const passes::MemberInfo& m : layout->members) {
      members.push_back(&m);
      if (is_union) {
        break;
      }
    }
    const passes::ValueTree* v = &value;
    passes::ValueTree wrapped;
    if (value.kind == VK::kUnion) {
      // A designator initialized a member other than the first.
      members = {&layout->members.at(static_cast<std::size_t>(value.member))};
      wrapped = tuple_tree({value.items.at(0)});
      v = &wrapped;
    }
    const std::size_t start = out.size();
    for (std::size_t i = 0; i < members.size(); ++i) {
      if (v->kind == VK::kTuple && i < v->items.size()) {
        flat_static_init_raw(members[i]->type, v->items[i], types, out);
      } else {
        flat_static_init_raw(members[i]->type, zero_init_value(members[i]->type, types), types,
                             out);
      }
    }
    // A union's storage is padded out to its full size when its first
    // member is smaller, as in `union u { char a; int b; } x = {1};`.
    if (is_union && layout->size > 0) {
      asdl::Int laid_down = 0;
      for (std::size_t i = start; i < out.size(); ++i) {
        laid_down += static_init_byte_count(out[i]);
      }
      if (laid_down < layout->size) {
        out.push_back(tac::ZeroInit{.bytes = layout->size - laid_down});
      }
    }
    return;
  }
  if (const auto* b = std::get_if<c99::BitInt>(&t)) {
    // The value at N bits, as an integer of the storage width. A
    // storage of 3, 5, 6, or 7 bytes has no init of its own, so it's
    // laid down a byte at a time, low byte first.
    const asdl::Int v = bit_precise::wrap(py_int(value), static_cast<int>(b->width), b->is_signed);
    const Type storage = bit_precise::storage_type(*b);
    const asdl::Int n = bit_precise::storage_bytes(b->width);
    if (!std::holds_alternative<c99::BitInt>(storage)) {
      out.push_back(tac_static_init_for(storage, int_tree(v)));
      return;
    }
    for (asdl::Int k = 0; k < n; ++k) {
      out.push_back(tac::UCharInit{.value = (v >> static_cast<int>(8 * k)) & 0xFF});
    }
    return;
  }
  out.push_back(tac_static_init_for(t, value));
}

// Lays out a static's value tree as a flat list of TAC `static_init`
// items in the order of its bytes: one item for a scalar, and the
// elements in row-major order for an array. Then each run of zero items
// is merged into one `ZeroInit(N)`, so the zero padding of a missing
// initializer (C99 §6.7.8.21) and a static with no initializer
// (§6.7.8.10) lay down one `DS.B N`.
std::vector<tac::Type_static_init> flat_static_init(const Type& t, const passes::ValueTree& value,
                                                    const TypeTable* types) {
  std::vector<tac::Type_static_init> out;
  flat_static_init_raw(t, value, types, out);
  return coalesce_zero_inits(std::move(out));
}

// Translates a c99 type to its TAC counterpart. A pointer is its own
// TAC type of 2 bytes. The character types and `bool` collapse onto
// `Int` or `UInt` by their signedness, and the bit-precise types too,
// since the symbol table gives the width. An array, a struct, or a
// union is `Pointer`, since TAC operates on such values through their
// address and the c99 symbol table sizes the storage. `Const` and
// `Volatile` are stripped: the type checker has applied `const`, and
// `volatile` is on each access's `is_volatile`.
tac::Type_data_type to_tac_data_type(const Type& t_in) {
  const Type& t = unqualified(t_in);
  switch (c99::kind(t)) {
    case TK::Int:
      return tac::Int{};
    case TK::Long:
      return tac::Long{};
    case TK::LongLong:
      return tac::LongLong{};
    case TK::UInt:
      return tac::UInt{};
    case TK::ULong:
      return tac::ULong{};
    case TK::ULongLong:
      return tac::ULongLong{};
    case TK::SChar:
      return tac::Int{};
    case TK::Char:
    case TK::UChar:
    case TK::Bool:
      return tac::UInt{};
    case TK::BitInt:
      return std::get<c99::BitInt>(t).is_signed ? tac::Type_data_type{tac::Int{}}
                                                : tac::Type_data_type{tac::UInt{}};
    case TK::Float:
      return tac::Float{};
    case TK::Double:
      return tac::Double{};
    case TK::Void:
      return tac::Void{};
    case TK::Pointer:
    case TK::Array:
    case TK::Structure:
    case TK::Union:
      return tac::Pointer{};
    case TK::FunType: {
      const auto& f = std::get<c99::FunType>(t);
      tac::FunType out;
      for (const Type& p : f.params) {
        out.params.push_back(to_tac_data_type(p));
      }
      out.ret = to_tac_data_type(*f.ret);
      return out;
    }
    default:
      throw InternalError("unexpected c99 data_type");
  }
}

// The constant of `width` bytes for the result of a packed BCD builtin.
tac::Type_const bcd_const(int width, asdl::Int value) {
  switch (width) {
    case 1:
      return tac::ConstUChar{.value = value};
    case 2:
      return tac::ConstUInt{.value = value};
    case 4:
      return tac::ConstULong{.value = value};
    default:
      return tac::ConstULongLong{.value = value};
  }
}

// Returns the raw `value` of the integer constant `v`, which Python
// reads as `v.const.value`.
asdl::Int const_raw_value(const Val& v) {
  const auto raw = bit_precise::raw_value(std::get<tac::Constant>(v).const_);
  if (!raw) {
    throw InternalError("the value of a floating constant");
  }
  return *raw;
}

}  // namespace

// --- Queries on c99 types and expressions ---

const std::optional<Type>& data_type_of(const Exp& exp) {
  static const std::optional<Type> kNone;
  switch (c99::kind(exp)) {
    case EK::Constant:
      return std::get<c99::Constant>(exp).data_type;
    case EK::Var:
      return std::get<c99::Var>(exp).data_type;
    case EK::Cast:
      return std::get<c99::Cast>(exp).data_type;
    case EK::Unary:
      return std::get<c99::Unary>(exp).data_type;
    case EK::Binary:
      return std::get<c99::Binary>(exp).data_type;
    case EK::Assignment:
      return std::get<c99::Assignment>(exp).data_type;
    case EK::CompoundAssignment:
      return std::get<c99::CompoundAssignment>(exp).data_type;
    case EK::Postfix:
      return std::get<c99::Postfix>(exp).data_type;
    case EK::Prefix:
      return std::get<c99::Prefix>(exp).data_type;
    case EK::Conditional:
      return std::get<c99::Conditional>(exp).data_type;
    case EK::FunctionCall:
      return std::get<c99::FunctionCall>(exp).data_type;
    case EK::IndirectCall:
      return std::get<c99::IndirectCall>(exp).data_type;
    case EK::Dereference:
      return std::get<c99::Dereference>(exp).data_type;
    case EK::AddressOf:
      return std::get<c99::AddressOf>(exp).data_type;
    case EK::Subscript:
      return std::get<c99::Subscript>(exp).data_type;
    case EK::InitList:
      return std::get<c99::InitList>(exp).data_type;
    case EK::SizeOfExp:
      return std::get<c99::SizeOfExp>(exp).data_type;
    case EK::SizeOfType:
      return std::get<c99::SizeOfType>(exp).data_type;
    case EK::Dot:
      return std::get<c99::Dot>(exp).data_type;
    case EK::Arrow:
      return std::get<c99::Arrow>(exp).data_type;
    case EK::Comma:
      return std::get<c99::Comma>(exp).data_type;
    case EK::CompoundLiteral:
      return std::get<c99::CompoundLiteral>(exp).data_type;
    case EK::String:
    case EK::Designation:
      return kNone;
  }
  return kNone;
}

const Type& unqualified(const Type& t) {
  const Type* p = &t;
  while (true) {
    if (const auto* c = std::get_if<c99::Const>(p)) {
      p = &*c->referenced_type;
    } else if (const auto* v = std::get_if<c99::Volatile>(p)) {
      p = &*v->referenced_type;
    } else {
      return *p;
    }
  }
}

// Strips every `Const` and `Volatile` anywhere in `t`, so that
// `Pointer(Const(Int))` becomes `Pointer(Int)` and `Array(Const(Int),
// N)` becomes `Array(Int, N)`. This is the boundary between the type
// checker and the TAC: the type checker has checked `const`, and
// `volatile` is captured on each access's `is_volatile`, so the storage
// layout, the byte sizes, and the initial values treat `T`, `const T`,
// and `volatile T` alike.
Type strip_quals(const Type& t) {
  switch (c99::kind(t)) {
    case TK::Const:
      return strip_quals(*std::get<c99::Const>(t).referenced_type);
    case TK::Volatile:
      return strip_quals(*std::get<c99::Volatile>(t).referenced_type);
    case TK::Pointer:
      return c99::Pointer{.referenced_type = strip_quals(*std::get<c99::Pointer>(t).referenced_type)};
    case TK::Array: {
      const auto& a = std::get<c99::Array>(t);
      return c99::Array{.element_type = strip_quals(*a.element_type), .size = a.size};
    }
    case TK::FunType: {
      const auto& f = std::get<c99::FunType>(t);
      c99::FunType out;
      for (const Type& p : f.params) {
        out.params.push_back(strip_quals(p));
      }
      out.ret = strip_quals(*f.ret);
      return out;
    }
    default:
      return t;
  }
}

// Every Load, Store, and indexed access sets `is_volatile` from this.
// A missing type, on a synthetic expression that bypassed the type
// checker, is not volatile, which lets the passes treat the access
// normally.
bool is_volatile_quals(const Type& t) {
  const Type* p = &t;
  while (const auto* c = std::get_if<c99::Const>(p)) {
    p = &*c->referenced_type;
  }
  return std::holds_alternative<c99::Volatile>(*p);
}

bool is_volatile_quals(const std::optional<Type>& t) { return t && is_volatile_quals(*t); }

bool is_integer_type(const Type& t) {
  switch (c99::kind(unqualified(t))) {
    case TK::Char:
    case TK::SChar:
    case TK::UChar:
    case TK::Bool:
    case TK::Int:
    case TK::UInt:
    case TK::Long:
    case TK::ULong:
    case TK::LongLong:
    case TK::ULongLong:
    case TK::BitInt:
      return true;
    default:
      return false;
  }
}

// Matches `tac_to_asm`'s `_is_unsigned_val` rule.
bool is_unsigned_integer(const Type& t_in) {
  const Type& t = unqualified(t_in);
  switch (c99::kind(t)) {
    case TK::BitInt:
      return !std::get<c99::BitInt>(t).is_signed;
    case TK::Char:
    case TK::UChar:
    case TK::Bool:
    case TK::UInt:
    case TK::ULong:
    case TK::ULongLong:
    case TK::Pointer:
      return true;
    default:
      return false;
  }
}

std::optional<std::pair<asdl::Int, asdl::Int>> int_range(const Type& t_in) {
  const Type& t = unqualified(t_in);
  if (const auto* b = std::get_if<c99::BitInt>(&t)) {
    return bit_precise::value_range(static_cast<int>(b->width), b->is_signed);
  }
  if (std::holds_alternative<c99::Bool>(t)) {
    return std::pair<asdl::Int, asdl::Int>{0, 1};
  }
  if (std::holds_alternative<c99::Pointer>(t) || is_integer_type(t)) {
    return bit_precise::value_range(8 * byte_width_of(t), !is_unsigned_integer(t));
  }
  return std::nullopt;
}

asdl::Int reinterpret_int(asdl::Int value, int width, bool is_signed) {
  const int bits = 8 * width;
  value &= (asdl::Int(1) << bits) - 1;
  if (is_signed && (value >> (bits - 1)) != 0) {
    value -= asdl::Int(1) << bits;
  }
  return value;
}

bool is_char_element(const Type& t) {
  return asdl::isa<c99::Char, c99::SChar, c99::UChar>(t);
}

// Returns the integer value of a constant integer expression under any
// number of casts, which the multi-dimensional `IndexedLoad` recognizer
// uses to find subscripts that are all constants.
std::optional<asdl::Int> const_int_or_none(const Exp* exp) {
  if (exp == nullptr) {
    return std::nullopt;
  }
  while (const auto* c = std::get_if<c99::Cast>(exp)) {
    exp = &*c->exp;
  }
  if (const auto* k = std::get_if<c99::Constant>(exp)) {
    using CK = c99::Kind_const;
    switch (c99::kind(k->const_)) {
      case CK::ConstInt:
        return std::get<c99::ConstInt>(k->const_).value;
      case CK::ConstLong:
        return std::get<c99::ConstLong>(k->const_).value;
      case CK::ConstLongLong:
        return std::get<c99::ConstLongLong>(k->const_).value;
      case CK::ConstUInt:
        return std::get<c99::ConstUInt>(k->const_).value;
      case CK::ConstULong:
        return std::get<c99::ConstULong>(k->const_).value;
      case CK::ConstULongLong:
        return std::get<c99::ConstULongLong>(k->const_).value;
      case CK::ConstChar:
        return std::get<c99::ConstChar>(k->const_).value;
      case CK::ConstUChar:
        return std::get<c99::ConstUChar>(k->const_).value;
      case CK::ConstBool:
        return std::get<c99::ConstBool>(k->const_).value;
      default:
        return std::nullopt;
    }
  }
  return std::nullopt;
}

// An automatic aggregate's initializer clears such a member along with
// the bytes around it (`init_leaves`): an integer or bit-precise 0, a
// floating +0.0, `nullptr`, or a cast of one of these, since a cast of 0
// gives 0, +0.0, or a null pointer.
bool is_zero_constant(const Exp& exp_in) {
  const Exp* exp = &exp_in;
  while (const auto* c = std::get_if<c99::Cast>(exp)) {
    exp = &*c->exp;
  }
  const auto* k = std::get_if<c99::Constant>(exp);
  if (k == nullptr) {
    return false;
  }
  using CK = c99::Kind_const;
  switch (c99::kind(k->const_)) {
    case CK::ConstNullptr:
      return true;
    case CK::ConstFloat:
      return std::get<c99::ConstFloat>(k->const_).bits == 0;
    case CK::ConstDouble:
      return std::get<c99::ConstDouble>(k->const_).bits == 0;
    case CK::ConstBitInt:
      return std::get<c99::ConstBitInt>(k->const_).value == 0;
    default:
      return const_int_or_none(exp) == asdl::Int(0);
  }
}

// A conversion to `bool` of a value that can only be 0 or 1 needs no
// `!= 0`: a `bool`, a comparison, `&&`, `||`, `!`, the constants 0 and
// 1, an integer cast of such a value, `&` with such an operand, `|` and
// `^` of two of them, and `?:` and `,` that yield one.
bool is_zero_or_one(const Exp& exp) {
  if (const auto& t = data_type_of(exp); t && std::holds_alternative<c99::Bool>(strip_quals(*t))) {
    return true;
  }
  switch (c99::kind(exp)) {
    case EK::Binary: {
      const auto& b = std::get<c99::Binary>(exp);
      using BK = c99::Kind_binary_operator;
      switch (c99::kind(b.op)) {
        case BK::Equal:
        case BK::NotEqual:
        case BK::LessThan:
        case BK::GreaterThan:
        case BK::LessOrEqual:
        case BK::GreaterOrEqual:
        case BK::LogicalAnd:
        case BK::LogicalOr:
          return true;
        case BK::BitwiseAnd:
          return is_zero_or_one(*b.left) || is_zero_or_one(*b.right);
        case BK::BitwiseOr:
        case BK::BitwiseXor:
          return is_zero_or_one(*b.left) && is_zero_or_one(*b.right);
        default:
          return false;
      }
    }
    case EK::Unary:
      return std::holds_alternative<c99::LogicalNot>(std::get<c99::Unary>(exp).op);
    case EK::Constant: {
      const auto& c = std::get<c99::Constant>(exp).const_;
      if (asdl::isa<c99::ConstFloat, c99::ConstDouble>(c)) {
        return false;
      }
      std::optional<asdl::Int> v;
      if (const auto* bi = std::get_if<c99::ConstBitInt>(&c)) {
        v = bi->value;
      } else if (std::holds_alternative<c99::ConstNullptr>(c)) {
        // `ConstNullptr` has no value; nullptr was erased before the
        // translation reads it.
        return false;
      } else {
        v = const_int_or_none(&exp);
      }
      return v == asdl::Int(0) || v == asdl::Int(1);
    }
    case EK::Cast: {
      const auto& c = std::get<c99::Cast>(exp);
      return is_integer_type(c.target_type) && is_zero_or_one(*c.exp);
    }
    case EK::Conditional: {
      const auto& c = std::get<c99::Conditional>(exp);
      return is_zero_or_one(*c.true_clause) && is_zero_or_one(*c.false_clause);
    }
    case EK::Comma:
      return is_zero_or_one(*std::get<c99::Comma>(exp).right);
    default:
      return false;
  }
}

namespace {

// Returns whether evaluating `node` calls a function, or, with
// `writes`, also assigns, increments, or decrements. The operand of
// `sizeof` isn't evaluated, so it doesn't count.
bool calls_or_writes(const Exp& node, bool writes);

bool any_calls_or_writes(const std::vector<Exp>& nodes, bool writes) {
  for (const Exp& n : nodes) {
    if (calls_or_writes(n, writes)) {
      return true;
    }
  }
  return false;
}

bool calls_or_writes(const Exp& node, bool writes) {
  switch (c99::kind(node)) {
    case EK::FunctionCall:
    case EK::IndirectCall:
      return true;
    case EK::Assignment:
    case EK::CompoundAssignment:
    case EK::Prefix:
    case EK::Postfix:
      if (writes) {
        return true;
      }
      break;
    case EK::SizeOfExp:
    case EK::SizeOfType:
      return false;
    default:
      break;
  }
  switch (c99::kind(node)) {
    case EK::Constant:
    case EK::String:
    case EK::Var:
    case EK::SizeOfExp:
    case EK::SizeOfType:
    case EK::FunctionCall:
    case EK::IndirectCall:
      return false;
    case EK::Cast:
      return calls_or_writes(*std::get<c99::Cast>(node).exp, writes);
    case EK::Unary:
      return calls_or_writes(*std::get<c99::Unary>(node).exp, writes);
    case EK::Binary: {
      const auto& x = std::get<c99::Binary>(node);
      return calls_or_writes(*x.left, writes) || calls_or_writes(*x.right, writes);
    }
    case EK::Assignment: {
      const auto& x = std::get<c99::Assignment>(node);
      return calls_or_writes(*x.lval, writes) || calls_or_writes(*x.rval, writes);
    }
    case EK::CompoundAssignment: {
      const auto& x = std::get<c99::CompoundAssignment>(node);
      return calls_or_writes(*x.lval, writes) || calls_or_writes(*x.rval, writes);
    }
    case EK::Postfix:
      return calls_or_writes(*std::get<c99::Postfix>(node).operand, writes);
    case EK::Prefix:
      return calls_or_writes(*std::get<c99::Prefix>(node).operand, writes);
    case EK::Conditional: {
      const auto& x = std::get<c99::Conditional>(node);
      return calls_or_writes(*x.condition, writes) || calls_or_writes(*x.true_clause, writes) ||
             calls_or_writes(*x.false_clause, writes);
    }
    case EK::Dereference:
      return calls_or_writes(*std::get<c99::Dereference>(node).exp, writes);
    case EK::AddressOf:
      return calls_or_writes(*std::get<c99::AddressOf>(node).exp, writes);
    case EK::Subscript: {
      const auto& x = std::get<c99::Subscript>(node);
      return calls_or_writes(*x.array, writes) || calls_or_writes(*x.index, writes);
    }
    case EK::InitList:
      return any_calls_or_writes(std::get<c99::InitList>(node).items, writes);
    case EK::Designation:
      return calls_or_writes(*std::get<c99::Designation>(node).init, writes);
    case EK::Dot:
      return calls_or_writes(*std::get<c99::Dot>(node).operand, writes);
    case EK::Arrow:
      return calls_or_writes(*std::get<c99::Arrow>(node).operand, writes);
    case EK::Comma: {
      const auto& x = std::get<c99::Comma>(node);
      return calls_or_writes(*x.left, writes) || calls_or_writes(*x.right, writes);
    }
    case EK::CompoundLiteral:
      return calls_or_writes(*std::get<c99::CompoundLiteral>(node).init, writes);
  }
  return false;
}

}  // namespace

// `lval OP= rval` evaluates `rval` first when `rval` calls a function
// and `lval` neither calls nor writes anything and isn't volatile. C
// leaves the evaluations of the two operands unsequenced (C23
// §6.5.17.1), and a call indeterminately sequenced with them
// (§6.5.3.3), so the lvalue may be read after the call. Its address and
// value then needn't be kept across the call, and `E->level |= h(x)` is
// `JSR h; LDY #k; ORA (E),Y; STA (E),Y`.
bool calls_first(const Exp& lval, const Exp& rval) {
  if (is_volatile_quals(data_type_of(lval))) {
    return false;
  }
  return calls_or_writes(rval, false) && !calls_or_writes(lval, true);
}

// `_const_int_or_none` skips the casts, which is only right for casts
// that keep the value, so this one converts at each cast (C99 §6.3.1.3),
// and `(signed char)200` is -56. A `sizeof` of a struct needs `types`.
std::optional<asdl::Int> exact_const_index(const Exp& exp, const TypeTable* types) {
  if (std::holds_alternative<c99::Constant>(exp)) {
    return const_int_or_none(&exp);
  }
  if (const auto* s = std::get_if<c99::SizeOfType>(&exp)) {
    return sizeof_c99(s->target_type, types);
  }
  if (const auto* s = std::get_if<c99::SizeOfExp>(&exp)) {
    if (const auto& t = data_type_of(*s->exp)) {
      return sizeof_c99(*t, types);
    }
    return std::nullopt;
  }
  const auto* c = std::get_if<c99::Cast>(&exp);
  if (c == nullptr || !is_integer_type(c->target_type)) {
    return std::nullopt;
  }
  const auto v = exact_const_index(*c->exp, types);
  if (!v) {
    return std::nullopt;
  }
  const Type target = strip_quals(c->target_type);
  if (std::holds_alternative<c99::Bool>(target)) {
    return asdl::Int(*v != 0 ? 1 : 0);
  }
  return reinterpret_int(*v, static_cast<int>(sizeof_c99(target, nullptr)),
                         !is_unsigned_integer(target));
}

// The sum `e + k` is at most 510, so the add can't wrap, and an index C
// defines leaves `e * elem_size` within a byte. The access then reads
// `arr + k * elem_size` indexed by `e * elem_size`, and
// `LOCAL_ZVARS[x + 1]` is `LDA LOCAL_ZVARS+1,X` where it was `x + 1`
// computed in A. A byte add written as `(uint8_t)(x + 1)` wraps at 255,
// and the narrowing cast leaves it alone. `k` may be a `sizeof`, as in
// `((uint8_t *)t)[i + sizeof *t]`.
std::optional<std::pair<const Exp*, asdl::Int>> split_index_addend(const Exp& index_in,
                                                                   asdl::Int total,
                                                                   asdl::Int elem_size,
                                                                   const TypeTable* types) {
  const Exp* index = &index_in;
  while (true) {
    const auto* c = std::get_if<c99::Cast>(index);
    if (c == nullptr || !is_integer_type(strip_quals(c->target_type)) ||
        sizeof_c99(c->target_type, nullptr) < 2) {
      break;
    }
    index = &*c->exp;
  }
  const auto* b = std::get_if<c99::Binary>(index);
  if (b == nullptr || !std::holds_alternative<c99::Add>(b->op) || !b->data_type ||
      sizeof_c99(*b->data_type, nullptr) < 2) {
    return std::nullopt;
  }
  const std::pair<const Exp*, const Exp*> orders[2] = {{&*b->left, &*b->right},
                                                       {&*b->right, &*b->left}};
  for (auto [e, k] : orders) {
    const auto value = exact_const_index(*k, types);
    if (!value || !(0 <= *value * elem_size && *value * elem_size < total)) {
      continue;
    }
    // `i + sizeof(T)` widens `i` to `int` and then to `size_t`. Each
    // cast keeps a byte's value, so the inner one will do.
    while (true) {
      const auto* c = std::get_if<c99::Cast>(e);
      if (c == nullptr || !is_integer_type(c->target_type) ||
          sizeof_c99(c->target_type, nullptr) < 2) {
        break;
      }
      const auto* inner = std::get_if<c99::Cast>(&*c->exp);
      if (inner == nullptr || !is_integer_type(inner->target_type) ||
          sizeof_c99(inner->target_type, nullptr) < 2) {
        break;
      }
      e = &*c->exp;
    }
    const auto* c = std::get_if<c99::Cast>(e);
    if (c == nullptr || sizeof_c99(c->target_type, nullptr) < 2) {
      continue;
    }
    const auto& inner_t = data_type_of(*c->exp);
    if (!inner_t || !asdl::isa<c99::UChar, c99::Char, c99::Bool>(strip_quals(*inner_t))) {
      continue;
    }
    return std::pair<const Exp*, asdl::Int>{e, *value};
  }
  return std::nullopt;
}

// Reading or writing an object's bytes through `(T *)&obj`, with T a
// character type, is the access C99 §6.5.7 allows for every object, so
// `((uint8_t *)&score)[i]` is the way to read byte `i` of a `uint32_t`.
// The cast may go through more pointer casts, as in `(T *)(void *)&obj`.
const c99::Var* cast_address_object(const Exp& ptr) {
  const auto* c = std::get_if<c99::Cast>(&ptr);
  if (c == nullptr || !std::holds_alternative<c99::Pointer>(unqualified(c->target_type))) {
    return nullptr;
  }
  const Exp* inner = &*c->exp;
  while (true) {
    const auto* ic = std::get_if<c99::Cast>(inner);
    if (ic == nullptr || !std::holds_alternative<c99::Pointer>(unqualified(ic->target_type))) {
      break;
    }
    const auto& t = data_type_of(*ic->exp);
    if (!t || !std::holds_alternative<c99::Pointer>(unqualified(*t))) {
      break;
    }
    inner = &*ic->exp;
  }
  if (const auto* a = std::get_if<c99::AddressOf>(inner)) {
    return std::get_if<c99::Var>(&*a->exp);
  }
  return nullptr;
}

// Subscripting such a pointer reads the object as an array of the
// pointed-to integer or pointer type, when the object's size is known
// and at most 256 bytes.
std::optional<ObjectView> object_view(const Exp& ptr, const TypeTable* types) {
  const c99::Var* obj = cast_address_object(ptr);
  if (obj == nullptr || !obj->data_type) {
    return std::nullopt;
  }
  const auto& cast = std::get<c99::Cast>(ptr);
  const Type& elem_t =
      unqualified(*std::get<c99::Pointer>(unqualified(cast.target_type)).referenced_type);
  switch (c99::kind(elem_t)) {
    case TK::Char:
    case TK::SChar:
    case TK::UChar:
    case TK::Bool:
    case TK::Int:
    case TK::UInt:
    case TK::Long:
    case TK::ULong:
    case TK::LongLong:
    case TK::ULongLong:
    case TK::Pointer:
      break;
    default:
      return std::nullopt;
  }
  const Type& obj_t = unqualified(*obj->data_type);
  if (std::holds_alternative<c99::FunType>(obj_t)) {
    return std::nullopt;
  }
  if (const auto* a = std::get_if<c99::Array>(&obj_t); a != nullptr && a->size == 0) {
    return std::nullopt;
  }
  const asdl::Int total = sizeof_c99(obj_t, types);
  const asdl::Int elem = sizeof_c99(elem_t, types);
  if (!(1 <= elem && elem <= total && total <= 256)) {
    return std::nullopt;
  }
  return ObjectView{.obj = obj, .elem = elem, .total = total};
}

// `*ptr` is `p[k]` when `ptr` is `p + k`, `k + p`, or `p` (which is
// `p[0]`), with `p` a cast of an object's address (C99 §6.5.2.1p2).
// Otherwise an ordinary dereference is left as it is.
std::optional<std::pair<Exp, Exp>> deref_as_subscript(const Exp& ptr) {
  if (cast_address_object(ptr) != nullptr) {
    return std::pair<Exp, Exp>{
        ptr, c99::Constant{.const_ = c99::ConstInt{.value = 0}, .data_type = c99::Int{}}};
  }
  const auto* b = std::get_if<c99::Binary>(&ptr);
  if (b == nullptr || !std::holds_alternative<c99::Add>(b->op)) {
    return std::nullopt;
  }
  const std::pair<const Exp*, const Exp*> orders[2] = {{&*b->left, &*b->right},
                                                       {&*b->right, &*b->left}};
  for (auto [p, k] : orders) {
    const auto& kt = data_type_of(*k);
    if (cast_address_object(*p) != nullptr && kt && is_integer_type(*kt)) {
      return std::pair<Exp, Exp>{*p, *k};
    }
  }
  return std::nullopt;
}

// The element size of `array` when it's the implicit decay of an array
// object of at least two elements that spans at most 255 bytes, so that
// the byte offset of any index C defines fits in a byte (see
// `translate_subscript_address`). `&arr` written out is a pointer to the
// whole array, which isn't a decay, and a pointer operand isn't an
// array object. A cast of an object's address, `((uint8_t *)&score)[i]`,
// reads the object as an array (`object_view`), and its offset is scaled
// in a byte when the object spans at most 255 bytes, so that the address
// one past its end fits too.
std::optional<asdl::Int> byte_offset_scale(const Exp& array, const TypeTable* types) {
  if (const auto view = object_view(array, types)) {
    return view->total <= 255 ? std::optional<asdl::Int>(view->elem) : std::nullopt;
  }
  const auto* a = std::get_if<c99::AddressOf>(&array);
  if (a == nullptr) {
    return std::nullopt;
  }
  const auto& inner_t = data_type_of(*a->exp);
  if (!inner_t || !a->data_type) {
    return std::nullopt;
  }
  const Type arr_t = strip_quals(*inner_t);
  const Type ptr_t = strip_quals(*a->data_type);
  const auto* arr = std::get_if<c99::Array>(&arr_t);
  const auto* ptr = std::get_if<c99::Pointer>(&ptr_t);
  if (arr == nullptr || ptr == nullptr || !(*ptr->referenced_type == *arr->element_type)) {
    return std::nullopt;
  }
  if (arr->size < 2) {
    return std::nullopt;
  }
  const asdl::Int elem = sizeof_c99(*arr->element_type, types);
  if (elem < 1 || arr->size * elem > 255) {
    return std::nullopt;
  }
  return elem;
}

// --- TAC values ---

int byte_width_of(const Type& t_in) {
  const Type& t = unqualified(t_in);
  switch (c99::kind(t)) {
    case TK::Char:
    case TK::SChar:
    case TK::UChar:
    case TK::Bool:
      return 1;
    case TK::Int:
    case TK::UInt:
      return 2;
    case TK::Long:
    case TK::ULong:
      return 4;
    case TK::LongLong:
    case TK::ULongLong:
      return 8;
    case TK::BitInt:
      return static_cast<int>(bit_precise::storage_bytes(std::get<c99::BitInt>(t).width));
    case TK::Float:
      return 4;
    case TK::Double:
      return 8;
    case TK::Pointer:
      return 2;
    default:
      throw InternalError("_byte_width_of: not an object type");
  }
}

asdl::Int sizeof_c99(const Type& t_in, const TypeTable* types) {
  const Type& t = unqualified(t_in);
  switch (c99::kind(t)) {
    case TK::Char:
    case TK::SChar:
    case TK::UChar:
    case TK::Bool:
      return 1;
    case TK::Int:
    case TK::UInt:
    case TK::Pointer:
      return 2;
    case TK::Long:
    case TK::ULong:
    case TK::Float:
      return 4;
    case TK::LongLong:
    case TK::ULongLong:
    case TK::Double:
      return 8;
    case TK::BitInt:
      return bit_precise::storage_bytes(std::get<c99::BitInt>(t).width);
    case TK::Array: {
      const auto& a = std::get<c99::Array>(t);
      return sizeof_c99(*a.element_type, types) * a.size;
    }
    case TK::Structure:
    case TK::Union: {
      const std::string& tag = std::holds_alternative<c99::Structure>(t)
                                   ? std::get<c99::Structure>(t).tag
                                   : std::get<c99::Union>(t).tag;
      if (types == nullptr) {
        throw InternalError("cannot size struct/union without TypeTable: " + tag);
      }
      const passes::StructLayout* layout = types->find(tag);
      if (layout == nullptr || !layout->complete) {
        throw InternalError("cannot size incomplete struct/union " + tag);
      }
      return layout->size;
    }
    default:
      throw InternalError("cannot size a type that has no size");
  }
}

tac::Type_const tac_const_for(const Type& t_in, asdl::Int value) {
  const Type& t = unqualified(t_in);
  switch (c99::kind(t)) {
    case TK::BitInt: {
      // The value modulo 2^N, held as an integer of the storage width,
      // which is how the padding bits are kept (bit_precise.py).
      const auto& b = std::get<c99::BitInt>(t);
      return bit_precise::make_const(static_cast<int>(bit_precise::storage_bytes(b.width)),
                                     b.is_signed,
                                     bit_precise::wrap(value, static_cast<int>(b.width), b.is_signed));
    }
    case TK::SChar:
      return tac::ConstChar{.value = value};
    case TK::Char:
    case TK::UChar:
    case TK::Bool:
      return tac::ConstUChar{.value = value};
    case TK::Int:
      return tac::ConstInt{.value = value};
    case TK::UInt:
    case TK::Pointer:
      return tac::ConstUInt{.value = value};
    case TK::Long:
      return tac::ConstLong{.value = value};
    case TK::ULong:
      return tac::ConstULong{.value = value};
    case TK::LongLong:
      return tac::ConstLongLong{.value = value};
    case TK::ULongLong:
      return tac::ConstULongLong{.value = value};
    case TK::Float:
      return tac::ConstFloat{.bits = fp_arith::int_to_single_bits(value)};
    case TK::Double:
      return tac::ConstDouble{.bits = fp_arith::int_to_double_bits(value)};
    default:
      throw InternalError("cannot build a TAC const for a non-object type");
  }
}

Val tac_const_val(const Type& t, asdl::Int value) {
  return tac::Constant{.const_ = tac_const_for(t, value)};
}

Val tac_storage_const(const c99::BitInt& t, asdl::Int bits) {
  return tac::Constant{.const_ = bit_precise::make_const(
                           static_cast<int>(bit_precise::storage_bytes(t.width)), t.is_signed, bits)};
}

// TAC carries the full integer kind, width and signedness, on each
// constant, so the map is one to one. `true` and `false` are bytes
// holding 1 and 0, since `bool` is `unsigned char` after `erase_bool`.
tac::Type_const to_tac_const(const c99::Type_const& c) {
  using CK = c99::Kind_const;
  switch (c99::kind(c)) {
    case CK::ConstInt:
      return tac::ConstInt{.value = std::get<c99::ConstInt>(c).value};
    case CK::ConstLong:
      return tac::ConstLong{.value = std::get<c99::ConstLong>(c).value};
    case CK::ConstLongLong:
      return tac::ConstLongLong{.value = std::get<c99::ConstLongLong>(c).value};
    case CK::ConstUInt:
      return tac::ConstUInt{.value = std::get<c99::ConstUInt>(c).value};
    case CK::ConstULong:
      return tac::ConstULong{.value = std::get<c99::ConstULong>(c).value};
    case CK::ConstULongLong:
      return tac::ConstULongLong{.value = std::get<c99::ConstULongLong>(c).value};
    case CK::ConstChar:
      return tac::ConstChar{.value = std::get<c99::ConstChar>(c).value};
    case CK::ConstUChar:
      return tac::ConstUChar{.value = std::get<c99::ConstUChar>(c).value};
    case CK::ConstBool:
      return tac::ConstUChar{.value = std::get<c99::ConstBool>(c).value};
    case CK::ConstFloat:
      return tac::ConstFloat{.bits = std::get<c99::ConstFloat>(c).bits};
    case CK::ConstDouble:
      return tac::ConstDouble{.bits = std::get<c99::ConstDouble>(c).bits};
    case CK::ConstBitInt: {
      const auto& b = std::get<c99::ConstBitInt>(c);
      return tac_const_for(c99::BitInt{.width = b.width, .is_signed = b.is_signed}, b.value);
    }
    case CK::ConstNullptr:
      break;
  }
  throw InternalError("unexpected c99 const");
}

namespace {

// Returns the floating value with IEEE 754 `bits` (single when
// `single`) converted to the integer or pointer type `target`, truncated
// toward zero (C99 §6.3.1.4). A value out of `target`'s range makes the
// conversion undefined, and the result is then the truncated value
// modulo 2^N in `target`'s range, so the TAC constant holds a value of
// its type. `(unsigned long)200e300` in a branch that never runs once
// gave a `ConstULong` of 1000 bits. An infinity or a NaN, which has no
// integer part, gives 0.
asdl::Int fp_to_integer(const Type& target, bool single, asdl::Int bits) {
  const auto rng = int_range(target);
  if (!rng || std::holds_alternative<c99::Bool>(strip_quals(target))) {
    // Python keeps the whole value here. No conversion that reaches
    // this has a `bool` target, since `emit_cast` lowers those first.
    const auto t = single ? fp_arith::single_bits_to_int(bits) : fp_arith::double_bits_to_int(bits);
    if (!t) {
      return 0;
    }
    if (const auto v = t->exact()) {
      return *v;
    }
    throw InternalError("the integer value of a float is too large to hold");
  }
  const auto [lo, hi] = *rng;
  const asdl::Int modulus = hi - lo + 1;
  int width = 0;
  while ((asdl::Int(1) << width) < modulus) {
    ++width;
  }
  const auto t = single ? fp_arith::single_bits_to_int(bits) : fp_arith::double_bits_to_int(bits);
  if (!t) {
    return 0;
  }
  return asdl::floor_mod(t->wrapped(width) - lo, modulus) + lo;
}

}  // namespace

// The operand's signedness is in its TAC class, so an unsigned constant
// is masked back to its nonnegative bit pattern before the conversion,
// and a signed one passes through. A floating source converts from its
// bit pattern through `fp_arith`, which keeps Python's floats out of
// it, and truncates toward zero as C99 §6.3.1.4 requires.
tac::Type_const fold_fp_cast_constant(const Type& target, const tac::Type_const& c) {
  using CK = tac::Kind_const;
  switch (tac::kind(c)) {
    case CK::ConstFloat: {
      const asdl::Int bits = std::get<tac::ConstFloat>(c).bits;
      if (std::holds_alternative<c99::Float>(target)) {
        return tac::ConstFloat{.bits = bits};
      }
      if (std::holds_alternative<c99::Double>(target)) {
        return tac::ConstDouble{.bits = fp_arith::single_bits_to_double_bits(bits)};
      }
      return tac_const_for(target, fp_to_integer(target, true, bits));
    }
    case CK::ConstDouble: {
      const asdl::Int bits = std::get<tac::ConstDouble>(c).bits;
      if (std::holds_alternative<c99::Double>(target)) {
        return tac::ConstDouble{.bits = bits};
      }
      if (std::holds_alternative<c99::Float>(target)) {
        return tac::ConstFloat{.bits = fp_arith::double_bits_to_single_bits(bits)};
      }
      return tac_const_for(target, fp_to_integer(target, false, bits));
    }
    case CK::ConstInt:
      return tac_const_for(target, std::get<tac::ConstInt>(c).value);
    case CK::ConstUInt:
      return tac_const_for(target, std::get<tac::ConstUInt>(c).value & 0xFF);
    case CK::ConstLong:
      return tac_const_for(target, std::get<tac::ConstLong>(c).value);
    case CK::ConstULong:
      return tac_const_for(target, std::get<tac::ConstULong>(c).value & 0xFFFF);
    case CK::ConstLongLong:
      return tac_const_for(target, std::get<tac::ConstLongLong>(c).value);
    case CK::ConstULongLong:
      return tac_const_for(target, std::get<tac::ConstULongLong>(c).value & 0xFFFFFFFF);
    default:
      // A bit-precise source, whose storage is one of the character
      // classes or the odd ones.
      return tac_const_for(target, bit_precise::const_value(c));
  }
}

const Val& need(const std::optional<Val>& v) {
  if (!v) {
    throw InternalError("a void value where the translation needs one");
  }
  return *v;
}

const Type& need_type(const std::optional<Type>& type) {
  if (!type) {
    throw InternalError("an expression without the type checker's data_type");
  }
  return *type;
}

// The loop labeling pass stamps each loop with a base label like
// `.loop@3`, and the lowering needs three targets for it (start,
// continue, and break), so the base gets a suffix. The base contains
// `@`, which no C identifier can, so neither it nor its suffixed forms
// can be a label the user wrote, and they differ from the translator's
// other labels (`.if_end@<N>`, `.cond_else@<N>`) in their prefix.
std::string start_label(std::string_view loop_label) {
  return std::string(loop_label) + "_start";
}
std::string continue_label(std::string_view loop_label) {
  return std::string(loop_label) + "_continue";
}
std::string break_label(std::string_view loop_label) {
  return std::string(loop_label) + "_break";
}

// --- The Translator's shared queries ---

// A `break` or `continue` carries the loop's label without
// `kKeepLoopMark`, which a `keep` loop's labels have.
std::string Translator::loop_targets(std::string_view label) const {
  if (keep_loops.contains(label)) {
    return std::string(label) + std::string(kKeepLoopMark);
  }
  return std::string(label);
}

// Makes a fresh temporary `%N` and registers it in the symbol table as
// an automatic object whose type is the type of the expression it
// holds, without qualifiers, since a temporary holds an rvalue (C99
// §6.3.2.1p2) and the frame sizing and `tac_to_asm`'s widths read the
// table and don't model qualifiers. A missing type, which only unit
// tests of the bare counter give, registers as `int`.
std::string Translator::make_temporary_variable_name(const Type* t) {
  std::string name(1, '%');
  name += std::to_string(temp_counter);
  ++temp_counter;
  symbols.insert_or_assign(
      name, passes::Symbol{.type = t == nullptr ? Type{c99::Int{}} : strip_quals(*t),
                           .attrs = passes::LocalAttr{}});
  return name;
}

// The leading `.` makes the label a dasm local label, scoped to the
// enclosing SUBROUTINE, so labels in different functions don't collide.
// The `@` (illegal in a C identifier) keeps it apart from a user's goto
// label, which is `.<function>@<orig>` with a C identifier after the
// `@`, where here a run of digits follows it.
std::string Translator::make_label(std::string_view prefix) {
  std::string name(1, '.');
  name += prefix;
  name += '@';
  name += std::to_string(label_counter);
  ++label_counter;
  return name;
}

// Returns whether `fn_name` is a `return_bool_in_carry` function that
// returns a struct, whose value member comes back in A (and X) and whose
// flag comes back in the carry, with no hidden pointer.
bool Translator::returns_carry_struct(std::string_view fn_name) const {
  const passes::Symbol* sym = symbols.find(fn_name);
  if (sym == nullptr) {
    return false;
  }
  const auto* f = std::get_if<passes::FunAttr>(&sym->attrs);
  if (f == nullptr || !f->carry_return) {
    return false;
  }
  const auto* ft = std::get_if<c99::FunType>(&sym->type);
  return ft != nullptr && asdl::isa<c99::Structure, c99::Union>(strip_quals(*ft->ret));
}

// A temporary `%N`, or any name missing from the symbol table, has no
// c99 qualifier.
bool Translator::is_volatile_var(std::string_view name) const {
  const passes::Symbol* sym = symbols.find(name);
  return sym != nullptr && is_volatile_quals(sym->type);
}

// Returns whether `exp` names a volatile scalar object. A plain `Copy`
// from or to a `Var` carries no volatile bit, so the TAC optimizer
// would treat the object as unchanging: it would drop `KBD;`, read `KBD`
// once for `while (!(KBD & 0x80)) ;`, and merge two reads. Every access
// to such an object goes through `read_volatile_var` and
// `write_volatile_var` instead, which emit the volatile `IndexedLoad`
// and `IndexedSymbolStore` that every pass preserves. Aggregates aren't
// read or written whole through these paths, and a `register` object is
// pinned to Y, so both keep the plain `Var` form.
bool Translator::volatile_scalar(const Exp& exp) const {
  const auto* v = std::get_if<c99::Var>(&exp);
  if (v == nullptr) {
    return false;
  }
  const passes::Symbol* sym = symbols.find(v->name);
  if (sym == nullptr || !is_volatile_quals(sym->type)) {
    return false;
  }
  if (asdl::isa<c99::Array, c99::Structure, c99::Union>(strip_quals(sym->type))) {
    return false;
  }
  if (std::holds_alternative<passes::StaticAttr>(sym->attrs)) {
    return true;
  }
  const auto* l = std::get_if<passes::LocalAttr>(&sym->attrs);
  return l != nullptr && !l->register_;
}

// Reads the volatile scalar `name` into a fresh temporary with a
// volatile `IndexedLoad` at index 0, which `tac_to_asm` lowers to a
// volatile `LDA` of the object's own storage: `LDA name` for an object
// with static storage, and the local's slot for an automatic one.
tac::Var Translator::read_volatile_var(const std::string& name, Instrs& instrs) {
  const Type t = symbols.at(name).type;
  tac::Var dst = temp_var(t);
  instrs.push_back(tac::IndexedLoad{
      .name = name,
      .index = tac::Constant{.const_ = tac::ConstUChar{.value = 0}},
      .dst = dst,
      .is_volatile = true,
  });
  return dst;
}

// Writes `src` to the volatile scalar `name`, the mirror of
// `read_volatile_var`.
void Translator::write_volatile_var(const std::string& name, const Val& src, Instrs& instrs) {
  instrs.push_back(tac::IndexedSymbolStore{
      .name = name,
      .index = tac::Constant{.const_ = tac::ConstUChar{.value = 0}},
      .src = src,
      .is_volatile = true,
  });
}

// Lowers `T x = e;` at block scope for an initializer that isn't a list.
void Translator::init_scalar(const c99::Type_var_decl& vd, Instrs& instrs) {
  const Val init_val = translate_value(*vd.init, instrs);
  if (volatile_scalar(c99::Var{.name = vd.name})) {
    write_volatile_var(vd.name, init_val, instrs);
    return;
  }
  instrs.push_back(tac::Copy{.src = init_val, .dst = tac::Var{.name = vd.name}});
}

// --- The driver ---

// Two passes build the program's list of top-level items. The first
// walks the declarations in source order, and each function definition
// becomes a TAC `Function`. The second iterates the symbol table and
// emits a `StaticVariable` for each object with static storage whose
// initial value is concrete. The two can't be folded together, because
// the symbol table holds each object's initial value after the merging
// of its redeclarations, and its `is_global`, neither of which is known
// at any one declaration.
tac::Program Translator::translate_program(c99::Program& prog) {
  erase_short(prog, symbols, types);
  erase_nullptr(prog, symbols, types);
  tac::Program program;
  for (const c99::Type_declaration& d : prog.declaration) {
    if (auto fn = translate_top_level_declaration(d)) {
      program.top_level.push_back(std::move(*fn));
    }
  }
  for (tac::Type_top_level& sv : emit_static_variables()) {
    program.top_level.push_back(std::move(sv));
  }
  erase_bool(prog, symbols, types);
  erase_bit_precise(prog, symbols, types);
  passes::check_marked_calls(program);
  return program;
}

// A function definition becomes a TAC `Function`, and everything else
// at file scope (a forward declaration, a variable, a struct) emits
// nothing here: the symbol-table pass emits the variables, and a struct
// declaration is type information the type checker has consumed.
std::optional<tac::Type_top_level> Translator::translate_top_level_declaration(
    const c99::Type_declaration& decl) {
  if (const auto* f = std::get_if<c99::FunctionDecl>(&decl)) {
    if (!f->function_decl->body) {
      return std::nullopt;
    }
    return translate_function_decl(*f->function_decl);
  }
  return std::nullopt;
}

// Translates a free-standing `Function(name, params, body)`, the shape
// the unit tests build, as a function of `int` parameters returning
// `int`.
tac::Function Translator::translate_function(const c99::Function& fn) {
  c99::FunType ftype;
  ftype.params.assign(fn.params.size(), c99::Int{});
  ftype.ret = c99::Int{};
  const c99::Type_function_decl fd{
      .name = fn.name,
      .params = fn.params,
      .body = asdl::Box<c99::Type_block>(fn.body),
      .data_type = ftype,
  };
  return translate_function_decl(fd);
}

tac::Function Translator::translate_function_decl(const c99::Type_function_decl& fd) {
  if (!fd.body) {
    throw InternalError("_translate_function of a declaration without a body");
  }
  const Type ret_type = [&]() -> Type {
    if (const auto* ft = std::get_if<c99::FunType>(&fd.data_type)) {
      return unqualified(*ft->ret);
    }
    return c99::Int{};
  }();
  // A function that returns a struct or union gets a hidden first
  // parameter holding the address of the caller's return slot. The
  // body's `return e;` stores `e` through it and returns no value, and
  // the caller passes the slot's address as the first argument.
  std::optional<std::string> sret;
  if (asdl::isa<c99::Structure, c99::Union>(ret_type) && !returns_carry_struct(fd.name)) {
    sret = std::string(".sret.");
    *sret += fd.name;
    symbols.insert_or_assign(*sret, passes::Symbol{.type = c99::Pointer{.referenced_type = ret_type},
                                                   .attrs = passes::LocalAttr{}});
  }
  std::optional<std::string> prior = std::move(sret_param);
  sret_param = sret;
  Instrs instrs;
  translate_block(**fd.body, instrs);
  sret_param = std::move(prior);
  // A body that doesn't end in a Return falls off the end with an
  // implicit `return 0`, of the declared return type's class, so that
  // every TAC function ends in a `Ret`. A `void` function returns no
  // value, which is legal to fall off (C99 §6.9.1p12), and so does one
  // that returns a struct, whose last `return` wrote through the sret
  // pointer.
  if (instrs.empty() || !std::holds_alternative<tac::Ret>(instrs.back())) {
    if (std::holds_alternative<c99::Void>(ret_type) || sret) {
      instrs.push_back(tac::Ret{});
    } else {
      instrs.push_back(tac::Ret{.val = tac_const_val(ret_type, 0)});
    }
  }
  // Function names aren't renamed by identifier resolution, since
  // linkage needs the source spelling, so the symbol table is keyed by
  // `fd.name`. A function missing from the table, which only the unit
  // tests give, is global, as one without `static` is.
  bool is_global = true;
  if (const passes::Symbol* sym = symbols.find(fd.name)) {
    if (const auto* f = std::get_if<passes::FunAttr>(&sym->attrs)) {
      is_global = f->is_global;
    }
  }
  std::vector<std::string> params;
  if (sret) {
    params.push_back(*sret);
  }
  params.insert(params.end(), fd.params.begin(), fd.params.end());
  return tac::Function{
      .name = fd.name,
      .is_global = is_global,
      .params = std::move(params),
      .instructions = std::move(instrs),
  };
}

// Walks the symbol table in insertion order, which is source order for
// the file-scope declarations, and emits a `StaticVariable` for every
// object with static storage and a concrete initial value, laid out as
// a flat list of typed items in the order of its bytes. A
// `NoInitializer` entry refers to a definition elsewhere, which emits
// its own. A tentative definition that no initializer upgraded is zero
// at the end of the translation unit (C99 §6.9.2.2).
std::vector<tac::Type_top_level> Translator::emit_static_variables() {
  std::vector<tac::Type_top_level> out;
  for (const auto& [name, sym] : symbols) {
    const auto* s = std::get_if<passes::StaticAttr>(&sym.attrs);
    if (s == nullptr) {
      continue;
    }
    const Type t_unq = strip_quals(sym.type);
    passes::ValueTree init_value;
    if (const auto* i = std::get_if<passes::Initial>(&s->initial_value)) {
      init_value = i->value;
    } else if (std::holds_alternative<passes::Tentative>(s->initial_value)) {
      init_value = zero_init_value(t_unq, types);
    } else {
      continue;
    }
    out.push_back(tac::StaticVariable{
        .name = name,
        .is_global = s->is_global,
        .data_type = to_tac_data_type(sym.type),
        .init = flat_static_init(t_unq, init_value, types),
    });
  }
  return out;
}

void Translator::translate_block(const c99::Type_block& block, Instrs& instrs) {
  for (const c99::Type_block_item& item : std::get<c99::Block>(block).block_item) {
    translate_block_item(item, instrs);
  }
}

void Translator::translate_block_item(const c99::Type_block_item& item, Instrs& instrs) {
  if (const auto* s = std::get_if<c99::S>(&item)) {
    translate_statement(*s->statement, instrs);
  } else {
    translate_declaration(*std::get<c99::D>(item).declaration, instrs);
  }
}

// TAC has no "declare" instruction, since an automatic variable is
// introduced by its first appearance, so a bare `int x;` lowers to
// nothing and `int x = e;` lowers as the assignment `x = e` does.
// Block-scope `static` and `extern` objects have static storage
// duration and appear in the program's `StaticVariable` list, so they
// run no code where they're declared. A `register` local has a storage
// class too, but it's automatic and its initializer runs. A function
// declaration and a struct declaration have no run-time effect.
void Translator::translate_declaration(const c99::Type_declaration& decl, Instrs& instrs) {
  const auto* v = std::get_if<c99::VarDecl>(&decl);
  if (v == nullptr) {
    return;
  }
  const c99::Type_var_decl& vd = v->var_decl;
  if (vd.storage_class && asdl::isa<c99::Static, c99::Extern>(*vd.storage_class)) {
    return;
  }
  if (vd.init) {
    translate_var_init(vd, instrs);
  }
}

// Lowers the initializer of the automatic `vd`, by the variable's
// declared type for a list: an array or an aggregate gets its bytes
// stored, `char arr[N] = "abc";` lays down each byte and the zero
// padding, and a plain initializer, a struct copy among them, is a
// `Copy`, which the lowering widens to the destination's size.
void Translator::translate_var_init(const c99::Type_var_decl& vd, Instrs& instrs) {
  if (std::holds_alternative<c99::InitList>(*vd.init)) {
    if (std::holds_alternative<c99::Array>(vd.data_type) ||
        asdl::isa<c99::Structure, c99::Union>(unqualified(vd.data_type))) {
      emit_initializer(vd, instrs);
      return;
    }
    throw InternalError("InitList for non-aggregate type");
  }
  if (std::holds_alternative<c99::String>(*vd.init)) {
    if (!std::holds_alternative<c99::Array>(vd.data_type)) {
      throw InternalError("a string initializer for a variable that isn't an array");
    }
    emit_initializer(vd, instrs);
    return;
  }
  init_scalar(vd, instrs);
}

// Lowers a call of a builtin (builtin_functions.py). The packed BCD
// builtins become `Binary(BcdAdd)` or `Binary(BcdSubtract)`. The type
// checker converted both operands to the result type, and when both are
// constants the result is folded here, in both pipelines. A <stdbit.h>
// builtin goes to `translate_stdbit`. `__c6502_bin_to_bcd` becomes
// `Unary(BinToBcd)` of the `unsigned int` the type checker converted its
// argument to, or the folded constant.
Val Translator::translate_builtin(const c99::FunctionCall& exp, Instrs& instrs) {
  const std::string& name = exp.name;
  if (const auto* stdbit = builtin_functions::find_stdbit_builtin(name)) {
    return translate_stdbit(exp, stdbit->operation, instrs);
  }
  if (name == builtin_functions::kBinToBcd) {
    const Val v = translate_value(exp.args.at(0), instrs);
    if (std::holds_alternative<tac::Constant>(v)) {
      return tac::Constant{
          .const_ = tac::ConstULong{.value = builtin_functions::bin_to_bcd(const_raw_value(v))}};
    }
    tac::Var dst = temp_var(Type{c99::ULong{}});
    instrs.push_back(tac::Unary{.op = tac::BinToBcd{}, .src = v, .dst = dst});
    return dst;
  }
  const Val a = translate_value(exp.args.at(0), instrs);
  const Val b = translate_value(exp.args.at(1), instrs);
  const bool add = name == builtin_functions::kBcdAdd;
  const Type t = strip_quals(need_type(exp.data_type));
  if (std::holds_alternative<tac::Constant>(a) && std::holds_alternative<tac::Constant>(b)) {
    int width = 0;
    switch (c99::kind(t)) {
      case TK::UChar:
        width = 1;
        break;
      case TK::UInt:
      case TK::UShort:
        width = 2;
        break;
      case TK::ULong:
        width = 4;
        break;
      case TK::ULongLong:
        width = 8;
        break;
      default:
        throw InternalError("a packed BCD builtin of an unexpected type");
    }
    const asdl::Int value = add ? builtin_functions::bcd_add(const_raw_value(a), const_raw_value(b), width)
                                : builtin_functions::bcd_sub(const_raw_value(a), const_raw_value(b), width);
    return tac::Constant{.const_ = bcd_const(width, value)};
  }
  tac::Var dst = temp_var(t);
  instrs.push_back(tac::Binary{
      .op = add ? tac::Type_binary_operator{tac::BcdAdd{}}
                : tac::Type_binary_operator{tac::BcdSubtract{}},
      .src1 = a,
      .src2 = b,
      .dst = dst,
  });
  return dst;
}

// Lowers a <stdbit.h> builtin (C23 §7.18, docs/stdbit.md). The type
// checker converted the argument to the value type, and a constant
// argument is folded here, in both pipelines, with
// `builtin_functions::stdbit`.
//
// Otherwise the call becomes a loop on a copy `t` of the value (or of
// its complement, for the operations that look for 1 bits where the
// others look for 0 bits). Each loop shifts `t` a bit at a time until
// it's zero, which is the 6502's `LSR` or `ASL` chain and an `ORA` of
// the bytes, and counts the shifts in a byte. That keeps the code small
// for every width, at the cost of up to N passes for an N-bit value.
// These are the results in terms of the width N and the number of
// shifts s.
//
// - Shifting right, s is the bit width (`bit_width`), and N - s is the
//   number of leading zeros.
// - Shifting left, s is N minus the number of trailing zeros, or 0 for
//   a zero value, so N - s is the number of trailing zeros.
// - A `first_...` operation is 0 for a zero `t` and N + 1 - s
//   otherwise.
// - `count_ones` and `count_zeros` add up the bits shifted out to the
//   right.
// - `bit_floor` and `bit_ceil` shift a result left once for each right
//   shift of `t`, starting from 1, which takes `bit_ceil` past the top
//   bit to 0 when its power of 2 doesn't fit (§7.18.16p2).
// - `has_single_bit` of one or two bytes is `v != 0 && (v & (v - 1)) ==
//   0`, and of four or eight it shifts a nonzero value right past its
//   lowest 1 and tests what's left for zero.
Val Translator::translate_stdbit(const c99::FunctionCall& exp, std::string_view operation,
                                 Instrs& instrs) {
  const Type value_t = strip_quals(need_type(data_type_of(exp.args.at(0))));
  const Type result_t = strip_quals(need_type(exp.data_type));
  const int width = byte_width_of(value_t);
  const int bits = 8 * width;
  const Val v = translate_value(exp.args.at(0), instrs);
  if (std::holds_alternative<tac::Constant>(v)) {
    return tac_const_val(result_t, builtin_functions::stdbit(operation, const_raw_value(v), bits));
  }
  const Val one = tac_const_val(c99::Int{}, 1);
  const Type uchar = c99::UChar{};
  const auto temp = [&](const Type& t) { return temp_var(t); };
  const auto shift = [&](tac::Type_binary_operator op, const tac::Var& var) {
    instrs.push_back(tac::Binary{.op = std::move(op), .src1 = var, .src2 = one, .dst = var});
  };
  const auto step = [&](const tac::Var& counter, int delta) {
    instrs.push_back(tac::Binary{
        .op = delta > 0 ? tac::Type_binary_operator{tac::Add{}}
                        : tac::Type_binary_operator{tac::Subtract{}},
        .src1 = counter,
        .src2 = tac_const_val(uchar, 1),
        .dst = counter,
    });
  };
  const auto label = [&](std::string_view prefix) {
    std::string full("stdbit_");
    full += prefix;
    return make_label(full);
  };

  const std::string end = label("end");
  if (operation == "has_single_bit" && width > 2) {
    // Shift the value right until its low bit is 1, and then once more.
    // The value had a single 1 bit if and only if nothing is left. For 4
    // and 8 bytes that takes less code than the v & (v - 1) below, and
    // for 1 and 2 more.
    const tac::Var r = temp(result_t);
    const tac::Var t = temp(value_t);
    const tac::Var low = temp(uchar);
    const std::string body = label("body");
    const std::string test = label("test");
    instrs.push_back(tac::Copy{.src = tac_const_val(result_t, 0), .dst = r});
    instrs.push_back(tac::Copy{.src = v, .dst = t});
    instrs.push_back(tac::JumpIfFalse{.condition = t, .target = end});
    instrs.push_back(tac::Jump{.target = test});
    instrs.push_back(tac::Label{.name = body});
    shift(tac::RightShift{}, t);
    instrs.push_back(tac::Label{.name = test});
    instrs.push_back(tac::Truncate{.src = t, .dst = low});
    instrs.push_back(tac::Binary{
        .op = tac::BitwiseAnd{}, .src1 = low, .src2 = tac_const_val(uchar, 1), .dst = low});
    instrs.push_back(tac::JumpIfFalse{.condition = low, .target = body});
    shift(tac::RightShift{}, t);
    instrs.push_back(tac::JumpIfTrue{.condition = t, .target = end});
    instrs.push_back(tac::Copy{.src = tac_const_val(result_t, 1), .dst = r});
    instrs.push_back(tac::Label{.name = end});
    return r;
  }
  if (operation == "has_single_bit") {
    const tac::Var r = temp(result_t);
    const tac::Var d = temp(value_t);
    instrs.push_back(tac::Copy{.src = tac_const_val(result_t, 0), .dst = r});
    instrs.push_back(tac::JumpIfFalse{.condition = v, .target = end});
    instrs.push_back(tac::Binary{
        .op = tac::Subtract{}, .src1 = v, .src2 = tac_const_val(value_t, 1), .dst = d});
    instrs.push_back(tac::Binary{.op = tac::BitwiseAnd{}, .src1 = v, .src2 = d, .dst = d});
    instrs.push_back(tac::JumpIfTrue{.condition = d, .target = end});
    instrs.push_back(tac::Copy{.src = tac_const_val(result_t, 1), .dst = r});
    instrs.push_back(tac::Label{.name = end});
    return r;
  }
  const tac::Var t = temp(value_t);
  const std::string body = label("body");
  const std::string test = label("test");
  if (operation == "bit_floor" || operation == "bit_ceil") {
    // Both loops start from r = 1. `bit_floor` shifts v >> 1 and clears
    // r after the loop for v == 0, and `bit_ceil` shifts v - 1, or 0 for
    // v == 0, whose result is 1. The loop's exit then joins no other
    // value of r, which would put a copy of r in the loop.
    const tac::Var r = temp(value_t);
    instrs.push_back(tac::Copy{.src = tac_const_val(value_t, 1), .dst = r});
    if (operation == "bit_floor") {
      instrs.push_back(tac::Binary{.op = tac::RightShift{}, .src1 = v, .src2 = one, .dst = t});
    } else {
      instrs.push_back(tac::Copy{.src = v, .dst = t});
      instrs.push_back(tac::JumpIfFalse{.condition = t, .target = test});
      instrs.push_back(tac::Binary{
          .op = tac::Subtract{}, .src1 = t, .src2 = tac_const_val(value_t, 1), .dst = t});
    }
    instrs.push_back(tac::Jump{.target = test});
    instrs.push_back(tac::Label{.name = body});
    shift(tac::RightShift{}, t);
    shift(tac::LeftShift{}, r);
    instrs.push_back(tac::Label{.name = test});
    instrs.push_back(tac::JumpIfTrue{.condition = t, .target = body});
    if (operation == "bit_floor") {
      instrs.push_back(tac::JumpIfTrue{.condition = v, .target = end});
      instrs.push_back(tac::Copy{.src = tac_const_val(value_t, 0), .dst = r});
      instrs.push_back(tac::Label{.name = end});
    }
    return r;
  }
  const bool ones = operation == "leading_ones" || operation == "trailing_ones" ||
                    operation == "first_leading_zero" || operation == "first_trailing_zero";
  if (ones) {
    instrs.push_back(tac::Unary{.op = tac::Complement{}, .src = v, .dst = t});
  } else {
    instrs.push_back(tac::Copy{.src = v, .dst = t});
  }
  const tac::Var n = temp(uchar);
  const auto set_n = [&](int value) {
    instrs.push_back(tac::Copy{.src = tac_const_val(uchar, value), .dst = n});
  };
  if (operation == "count_ones" || operation == "count_zeros") {
    const bool counting_ones = operation == "count_ones";
    set_n(counting_ones ? 0 : bits);
    const std::string skip = label("skip");
    tac::Var low = t;
    if (width > 1) {
      low = temp(uchar);
    }
    const tac::Var bit = temp(uchar);
    instrs.push_back(tac::Jump{.target = test});
    instrs.push_back(tac::Label{.name = body});
    if (width > 1) {
      instrs.push_back(tac::Truncate{.src = t, .dst = low});
    }
    instrs.push_back(tac::Binary{
        .op = tac::BitwiseAnd{}, .src1 = low, .src2 = tac_const_val(uchar, 1), .dst = bit});
    instrs.push_back(tac::JumpIfFalse{.condition = bit, .target = skip});
    step(n, counting_ones ? 1 : -1);
    instrs.push_back(tac::Label{.name = skip});
    shift(tac::RightShift{}, t);
  } else {
    const bool right = operation == "leading_zeros" || operation == "leading_ones" ||
                       operation == "bit_width" || operation == "first_leading_zero" ||
                       operation == "first_leading_one";
    const tac::Type_binary_operator direction =
        right ? tac::Type_binary_operator{tac::RightShift{}}
              : tac::Type_binary_operator{tac::LeftShift{}};
    if (operation.starts_with("first_")) {
      set_n(0);
      instrs.push_back(tac::JumpIfFalse{.condition = t, .target = end});
      set_n(bits + 1);
      instrs.push_back(tac::Label{.name = body});
      shift(direction, t);
      step(n, -1);
      instrs.push_back(tac::JumpIfTrue{.condition = t, .target = body});
      instrs.push_back(tac::Label{.name = end});
      return stdbit_count_result(n, result_t, instrs);
    }
    const bool counting_up = operation == "bit_width";
    set_n(counting_up ? 0 : bits);
    instrs.push_back(tac::Jump{.target = test});
    instrs.push_back(tac::Label{.name = body});
    shift(direction, t);
    step(n, counting_up ? 1 : -1);
  }
  instrs.push_back(tac::Label{.name = test});
  instrs.push_back(tac::JumpIfTrue{.condition = t, .target = body});
  return stdbit_count_result(n, result_t, instrs);
}

// Returns the count `n`, a byte, zero-extended to `result_t`, the
// `unsigned int` a <stdbit.h> count is.
Val Translator::stdbit_count_result(const tac::Var& n, const Type& result_t, Instrs& instrs) {
  tac::Var r = temp_var(result_t);
  instrs.push_back(tac::ZeroExtend{.src = n, .dst = r});
  return r;
}

tac::Program translate_program(c99::Program& prog, SymbolTable& symbols, TypeTable* types) {
  return Translator(symbols, types).translate_program(prog);
}

}  // namespace c6502::c99_to_tac
