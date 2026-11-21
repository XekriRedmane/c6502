// The port of passes/designators.py, designated initializers (C99
// §6.7.8, C23 §6.7.11).
//
// The parser leaves an item such as `[2] = v`, `.x = v`, or `.a[2].b =
// v` in a braced list as a c99 `Designation`, with an array designator's
// index already folded to an integer. `normalize_initializer` rewrites a
// list with designations into the positional form the rest of the
// compiler handles. That form has one item per subobject, in order, with
// the empty initializer `{}` for each subobject that nothing
// initializes, which default-initializes it (zero, or a null pointer). A
// union's list holds one item, and `InitList.union_member` names the
// member it initializes.
//
//     struct p { int x; int y; };
//     struct s { int n; struct p pt[2]; };
//     struct s v = {.pt[1].y = 7, .n = 3};
//
// becomes
//
//     struct s v = {3, { {}, {{}, 7} }};
//
// string_lifting calls this first, because it is the first pass that
// dispatches each item of a list by the type it initializes, so no pass
// after it sees a `Designation`. The type checker calls it too, for the
// lists the parser checks while folding a `constexpr` object.
//
// The rules come from §6.7.11p17-22.
//
// - A designation starts at the list's own object. After it, a
//   positional item initializes the subobject after the one the first
//   designator names. When the designation has more than one
//   designator, the item after it would continue inside a subaggregate,
//   which takes brace elision. c6502 doesn't elide braces, so that is an
//   error that says to brace the subaggregate.
// - A later initializer of a subobject overrides an earlier one. When
//   the later one is a designation that reaches inside a subaggregate,
//   it overrides only the part it names, and the rest of the earlier
//   initializer stays. An overridden initializer is never evaluated. C
//   leaves it unspecified whether its side effects happen (§6.7.11p19),
//   and c6502 rejects an overridden initializer that has any, which is
//   an assignment, an increment or decrement, or a call. A read of a
//   volatile object in one is dropped without an error, since the types
//   of names aren't known here.
// - A designation that initializes one member of a union replaces any
//   earlier initializer of another member.
// - An array of unknown size takes its size from the largest index
//   initialized.
#include "passes/designators.h"

#include <memory>

#include "errors.h"
#include "passes/type_checker.h"

namespace c6502::passes::designators {

namespace {

namespace ir = ::c6502::c99_ast;
using Type = ir::Type_data_type;
using Exp = ir::Type_exp;
using Int = asdl::Int;
using type_checking::c_type;
using type_checking::strip_quals;

[[noreturn]] void error(const std::string& message) {
  throw type_checking::TypeCheckError(message);
}

bool is_aggregate(const Type& t) {
  return asdl::isa<ir::Array, ir::Structure, ir::Union>(strip_quals(t));
}

const std::string& tag_of(const Type& t) {
  if (const auto* s = std::get_if<ir::Structure>(&t)) {
    return s->tag;
  }
  return std::get<ir::Union>(t).tag;
}

std::string plural(Int n) { return n != Int(1) ? "s" : ""; }

// Returns the designators as the program writes them, such as `.a[2].b`.
std::string spell(const std::vector<ir::Type_designator>& designators, std::size_t count) {
  std::string out;
  for (std::size_t i = 0; i < count && i < designators.size(); ++i) {
    const auto& d = designators[i];
    if (const auto* index = std::get_if<ir::IndexDesignator>(&d)) {
      out += "[" + asdl::int_to_string(index->index) + "]";
    } else {
      out += "." + std::get<ir::MemberDesignator>(d).member;
    }
  }
  return out;
}

std::string spell(const std::vector<ir::Type_designator>& designators) {
  return spell(designators, designators.size());
}

struct Partial;

// The initializer of one element or member: an expression (a braced
// list among them) or a `Partial`.
struct Slot {
  Int key = 0;
  std::optional<Exp> exp;
  std::unique_ptr<Partial> partial;
};

// An aggregate whose initializer is being put together. `slots` maps the
// index of an element or member to its initializer, in the order of
// Python's dict, which keeps a key's place when it is assigned again.
// For a union, `slots` has at most one entry, the member initialized.
struct Partial {
  Type t;
  std::string path;
  std::vector<Slot> slots;

  Slot* find(Int key) {
    for (Slot& s : slots) {
      if (s.key == key) {
        return &s;
      }
    }
    return nullptr;
  }
};

bool has_side_effects(const Exp& node);

bool has_side_effects(const Partial& node) {
  for (const Slot& s : node.slots) {
    if (s.partial ? has_side_effects(*s.partial) : has_side_effects(*s.exp)) {
      return true;
    }
  }
  return false;
}

// Returns whether evaluating the initializer `node` assigns, increments,
// decrements, or calls a function. The operand of `sizeof` isn't
// evaluated, so it doesn't count.
bool has_side_effects(const Exp& node) {
  using E = ir::Kind_exp;
  switch (ir::kind(node)) {
    case E::Assignment:
    case E::CompoundAssignment:
    case E::Prefix:
    case E::Postfix:
    case E::FunctionCall:
    case E::IndirectCall:
      return true;
    case E::SizeOfExp:
    case E::SizeOfType:
    case E::Constant:
    case E::String:
    case E::Var:
      return false;
    case E::Cast:
      return has_side_effects(*std::get<ir::Cast>(node).exp);
    case E::Unary:
      return has_side_effects(*std::get<ir::Unary>(node).exp);
    case E::Binary: {
      const auto& b = std::get<ir::Binary>(node);
      return has_side_effects(*b.left) || has_side_effects(*b.right);
    }
    case E::Conditional: {
      const auto& c = std::get<ir::Conditional>(node);
      return has_side_effects(*c.condition) || has_side_effects(*c.true_clause) ||
             has_side_effects(*c.false_clause);
    }
    case E::Dereference:
      return has_side_effects(*std::get<ir::Dereference>(node).exp);
    case E::AddressOf:
      return has_side_effects(*std::get<ir::AddressOf>(node).exp);
    case E::Subscript: {
      const auto& s = std::get<ir::Subscript>(node);
      return has_side_effects(*s.array) || has_side_effects(*s.index);
    }
    case E::InitList:
      for (const Exp& item : std::get<ir::InitList>(node).items) {
        if (has_side_effects(item)) {
          return true;
        }
      }
      return false;
    case E::Designation:
      return has_side_effects(*std::get<ir::Designation>(node).init);
    case E::Dot:
      return has_side_effects(*std::get<ir::Dot>(node).operand);
    case E::Arrow:
      return has_side_effects(*std::get<ir::Arrow>(node).operand);
    case E::Comma: {
      const auto& c = std::get<ir::Comma>(node);
      return has_side_effects(*c.left) || has_side_effects(*c.right);
    }
    case E::CompoundLiteral:
      return has_side_effects(*std::get<ir::CompoundLiteral>(node).init);
  }
  return false;
}

class Normalizer {
 public:
  explicit Normalizer(const LayoutOf& layout_of) : layout_of_(layout_of) {}

  // Returns the positional form of the list `init`, which initializes
  // the object `path` of type `t`.
  Exp list(const ir::InitList& init, const Type& t, const std::string& path) {
    const Type& t_unq = strip_quals(t);
    if (!is_aggregate(t_unq)) {
      for (const Exp& item : init.items) {
        if (const auto* d = std::get_if<ir::Designation>(&item)) {
          error("`" + path + "` is a scalar of type `" + c_type(t) +
                "`, so its initializer can't have the designator `" + spell(d->designators) +
                "`, which names an element of an array or a member of a struct or union (C23 "
                "§6.7.11p6-7)");
        }
      }
      // A scalar's braces hold one expression. A list nested in them is
      // left for the type checker to reject.
      return init;
    }
    Partial partial{.t = t_unq, .path = path, .slots = {}};
    fill(partial, init);
    return emit(partial);
  }

 private:
  // Applies the items of the list `init` to `partial`, in order.
  void fill(Partial& partial, const ir::InitList& init) {
    Int pos = 0;
    const std::vector<ir::Type_designator>* chain = nullptr;
    for (const Exp& item : init.items) {
      if (const auto* d = std::get_if<ir::Designation>(&item)) {
        pos = place(partial, d->designators, 0, *d->init) + Int(1);
        chain = d->designators.size() > 1 ? &d->designators : nullptr;
        continue;
      }
      if (chain != nullptr) {
        const std::string inner = spell(*chain, 1);
        error("in the initializer of `" + partial.path + "`, the item after `" + spell(*chain) +
              " = ...` would initialize the subobject after `" + partial.path + spell(*chain) +
              "` inside `" + partial.path + inner +
              "`, which takes brace elision, and c6502 doesn't elide braces. Put the "
              "initializers of `" +
              partial.path + inner + "` in braces of their own, as in `" + inner + " = {...}`");
      }
      if (asdl::isa<ir::Union>(partial.t) && pos > Int(0)) {
        error("too many initializers for union `" + partial.path +
              "`: a union's list initializes one member, and an item without a designator "
              "can't follow another item (C23 §6.7.11p17)");
      }
      check_in_range(partial, pos);
      set(partial, pos, item);
      pos += Int(1);
    }
  }

  // Throws when the positional item at index `k` is past the end of
  // `partial`.
  void check_in_range(const Partial& partial, Int k) {
    if (const auto* a = std::get_if<ir::Array>(&partial.t)) {
      if (a->size != Int(0) && k >= a->size) {
        error("too many initializers for array `" + partial.path + "`: the item for index " +
              asdl::int_to_string(k) + " is past the end of an array of " +
              asdl::int_to_string(a->size) + " element" + plural(a->size));
      }
      return;
    }
    const Members members = this->members(partial);
    const Int n = Int(static_cast<long long>(members.size()));
    if (k >= n) {
      error("too many initializers for struct `" + partial.path + "`: the struct has " +
            asdl::int_to_string(n) + " member" + plural(n) +
            ", and the item after the last one has no member to initialize");
    }
  }

  // Returns the `(name, type)` pairs of the struct or union `partial`.
  Members members(const Partial& partial) { return members_of(partial.t, partial.path); }

  Members members_of(const Type& t, const std::string& path) {
    auto members = layout_of_(tag_of(t));
    if (!members) {
      const std::string kw = asdl::isa<ir::Union>(t) ? "union" : "struct";
      error("`" + path + "` has the incomplete type `" + kw + " " + tag_of(t) +
            "`, so it can't be initialized");
    }
    return std::move(*members);
  }

  struct Step {
    Int index = 0;
    Type type;
    std::string path;
  };

  // Returns the index, type, and path of the subobject the designator
  // `d` names in the object `path` of type `t`.
  Step step(const Type& t, const ir::Type_designator& d, const std::string& path) {
    const Type& t_unq = strip_quals(t);
    if (const auto* index = std::get_if<ir::IndexDesignator>(&d)) {
      const std::string spelled = "[" + asdl::int_to_string(index->index) + "]";
      const auto* a = std::get_if<ir::Array>(&t_unq);
      if (a == nullptr) {
        error("the designator `" + spelled + "` names an element of an array, and `" + path +
              "` has type `" + c_type(t) + "` (C23 §6.7.11p6)");
      }
      if (a->size != Int(0) && index->index >= a->size) {
        error("the designator `" + spelled + "` is past the end of `" + path + "`, an array of " +
              asdl::int_to_string(a->size) + " element" + plural(a->size) + " (C23 §6.7.11p6)");
      }
      return {index->index, *a->element_type, path + spelled};
    }
    const std::string& member = std::get<ir::MemberDesignator>(d).member;
    if (!asdl::isa<ir::Structure, ir::Union>(t_unq)) {
      error("the designator `." + member +
            "` names a member of a struct or union, and `" + path + "` has type `" + c_type(t) +
            "` (C23 §6.7.11p7)");
    }
    const Members members = members_of(t_unq, path);
    for (std::size_t k = 0; k < members.size(); ++k) {
      if (members[k].first == member) {
        return {Int(static_cast<long long>(k)), members[k].second, path + "." + member};
      }
    }
    error("the designator `." + member + "` names no member of `" + path +
          "`, which has type `" + c_type(t) + "` (C23 §6.7.11p7)");
  }

  // Applies the designation `designators[first:] = init` to `partial`
  // and returns the index of the subobject its first designator names.
  Int place(Partial& partial, const std::vector<ir::Type_designator>& designators,
            std::size_t first, const Exp& init) {
    Step s = step(partial.t, designators[first], partial.path);
    if (designators.size() - first == 1) {
      set(partial, s.index, init);
      return s.index;
    }
    // The next designator is checked against the subobject first, so a
    // wrong one is reported before anything is overridden.
    step(s.type, designators[first + 1], s.path);
    if (asdl::isa<ir::Union>(partial.t)) {
      drop_other_members(partial, s.index);
    }
    Slot* old = partial.find(s.index);
    if (old != nullptr && old->partial) {
      // The subobject's earlier initializer is a `Partial` already, which
      // the designation changes in place.
      place(*old->partial, designators, first + 1, init);
      return s.index;
    }
    auto sub = as_partial(old != nullptr ? &*old->exp : nullptr, s.type, s.path);
    place(*sub, designators, first + 1, init);
    if (Slot* again = partial.find(s.index)) {
      again->exp.reset();
      again->partial = std::move(sub);
    } else {
      partial.slots.push_back(Slot{.key = s.index, .exp = std::nullopt, .partial = std::move(sub)});
    }
    return s.index;
  }

  // Makes `init` the initializer of subobject `k` of `partial`,
  // overriding whatever initialized it before.
  void set(Partial& partial, Int k, const Exp& init) {
    if (asdl::isa<ir::Union>(partial.t)) {
      drop_other_members(partial, k);
    }
    if (Slot* old = partial.find(k)) {
      discard(*old, sub_path(partial, k));
      old->partial.reset();
      old->exp = init;
      return;
    }
    partial.slots.push_back(Slot{.key = k, .exp = init, .partial = nullptr});
  }

  // Discards the initializer of each member of the union `partial` other
  // than member `k`, since a union holds one.
  void drop_other_members(Partial& partial, Int k) {
    std::vector<Int> others;
    for (const Slot& s : partial.slots) {
      if (s.key != k) {
        others.push_back(s.key);
      }
    }
    for (const Int j : others) {
      for (auto it = partial.slots.begin(); it != partial.slots.end(); ++it) {
        if (it->key == j) {
          Slot popped = std::move(*it);
          partial.slots.erase(it);
          discard(popped, sub_path(partial, j));
          break;
        }
      }
    }
  }

  std::string sub_path(const Partial& partial, Int k) {
    if (asdl::isa<ir::Array>(partial.t)) {
      return partial.path + "[" + asdl::int_to_string(k) + "]";
    }
    const Members members = this->members(partial);
    return partial.path + "." + members.at(static_cast<std::size_t>(k)).first;
  }

  // Returns the subobject's earlier initializer `old` as a `Partial`
  // that a later designation can override part of.
  std::unique_ptr<Partial> as_partial(const Exp* old, const Type& t, const std::string& path) {
    auto partial = std::make_unique<Partial>(Partial{.t = strip_quals(t), .path = path, .slots = {}});
    if (old == nullptr) {
      return partial;
    }
    if (const auto* list = std::get_if<ir::InitList>(old)) {
      fill(*partial, *list);
      return partial;
    }
    const std::string what =
        asdl::isa<ir::String>(*old) ? "a string literal" : "an expression";
    error("a designator overrides part of `" + path + "`, which " + what +
          " initializes, and c6502 can only override part of a subobject that a braced list "
          "initializes. Initialize `" +
          path + "` with a braced list instead");
  }

  // Drops the overridden initializer `old` of `path`, which must have no
  // side effects.
  void discard(const Slot& old, const std::string& path) {
    const bool effects = old.partial ? has_side_effects(*old.partial) : has_side_effects(*old.exp);
    if (effects) {
      error("a later designator overrides the initializer of `" + path +
            "`, which has side effects. C leaves it unspecified whether they happen (C23 "
            "§6.7.11p19), and c6502 doesn't evaluate an overridden initializer, so it rejects "
            "one with side effects");
    }
  }

  // Returns the positional InitList for `partial`.
  Exp emit(Partial& partial) {
    const Type& t = partial.t;
    if (const auto* a = std::get_if<ir::Array>(&t)) {
      const Type elem = *a->element_type;
      Int n = 0;
      for (const Slot& s : partial.slots) {
        if (s.key + Int(1) > n) {
          n = s.key + Int(1);
        }
      }
      ir::InitList out;
      for (Int k = 0; k < n; k += Int(1)) {
        out.items.push_back(emit_item(partial.find(k), elem, sub_path(partial, k)));
      }
      return out;
    }
    const Members members = this->members(partial);
    if (asdl::isa<ir::Union>(t)) {
      if (partial.slots.size() != 1) {
        throw InternalError("ValueError: a union's initializer has no member or several");
      }
      Slot& slot = partial.slots.front();
      const auto& [name, m_type] = members.at(static_cast<std::size_t>(slot.key));
      ir::InitList out;
      out.items.push_back(emit_item(&slot, m_type, partial.path + "." + name));
      out.union_member = name;
      return out;
    }
    Int n = 0;
    for (const Slot& s : partial.slots) {
      if (s.key + Int(1) > n) {
        n = s.key + Int(1);
      }
    }
    ir::InitList out;
    for (Int k = 0; k < n; k += Int(1)) {
      out.items.push_back(emit_item(partial.find(k), members.at(static_cast<std::size_t>(k)).second,
                                    sub_path(partial, k)));
    }
    return out;
  }

  Exp emit_item(Slot* item, const Type& t, const std::string& path) {
    if (item == nullptr) {
      return ir::InitList{};
    }
    if (item->partial) {
      return emit(*item->partial);
    }
    if (const auto* list = std::get_if<ir::InitList>(&*item->exp)) {
      if (has_designation(*item->exp)) {
        return this->list(*list, t, path);
      }
    }
    return *item->exp;
  }

  const LayoutOf& layout_of_;
};

}  // namespace

bool has_designation(const Exp& init) {
  if (asdl::isa<ir::Designation>(init)) {
    return true;
  }
  if (const auto* list = std::get_if<ir::InitList>(&init)) {
    for (const Exp& item : list->items) {
      if (has_designation(item)) {
        return true;
      }
    }
  }
  return false;
}

Exp normalize_initializer(Exp init, const Type& t, const LayoutOf& layout_of,
                          const std::optional<std::string>& what) {
  const auto* list = std::get_if<ir::InitList>(&init);
  if (list == nullptr || !has_designation(init)) {
    return init;
  }
  const std::string path = what ? *what : "(" + c_type(t) + "){...}";
  return Normalizer(layout_of).list(*list, t, path);
}

}  // namespace c6502::passes::designators
