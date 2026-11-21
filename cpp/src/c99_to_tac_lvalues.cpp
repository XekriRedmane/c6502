// The lowering of initializers, lvalues, and subscripts, part A2 of
// c99_to_tac (src/c99_to_tac_translator.h).

#include <algorithm>
#include <string>
#include <utility>

#include "c99_to_tac_translator.h"
#include "errors.h"
#include "pyrepr.h"

namespace c6502::c99_to_tac {

namespace {

using EK = c99::Kind_exp;

// The fewest zero bytes in a run that an automatic aggregate's
// initializer clears with a loop (`emit_zero_bytes`). A shorter run gets
// constant stores. These are the bytes of code that `= {}` adds to a
// function holding a struct of n bytes, measured on 17 Nov 2026 with
// each form forced.
//
//   n                       2    4    6    7    8   10   16   32   loop
//   -O, in the pool         6   12   16   18   20   24   36   68     10
//   -O, soft-stack frame    9   15   21   24   27   33   51   99     23
//   unoptimized            35   41   80  117   53   92  110  226     81
//
// The loop is smaller from 4 bytes up in the pool and from 7 bytes up in
// a frame. It's also slower. In the pool, it's `LDY #n; LDA #0; DEY; STA
// v,Y; BNE`, 10 cycles a byte, where the stores are `STA v+k` with A or
// Y zero, 3 cycles a byte. The loop was 11 bytes and 14 cycles a byte
// when this table was measured, with `LDA #0` and `TYA` in it, before
// passes/loop_constant_a.py. In a frame, the loop takes the object's
// address in each pass. From 8 bytes up the loop is the smaller form in
// both -O settings, but 8 bytes are one 8-byte store, which the
// unoptimized pipeline lowers in 53 bytes against the loop's 81. The
// loop's temporaries also take 7 more bytes of an unoptimized frame than
// the store's, and that took `test_non_constant` in
// tests/chapter_15/valid/initialization/automatic.c past the 255-byte
// reach of `(FP),Y`. So the loop starts at 9 bytes. Below that, the
// stores under -O cost at most 10 bytes more than the loop and run over
// three times as fast.
constexpr asdl::Int kClearLoopBytes = 9;

// The types `emit_zero_bytes` stores zero with, widest first: one Store
// per 8, 4, 2, or 1 bytes.
const std::pair<asdl::Int, Type>& zero_store_type(asdl::Int left) {
  static const std::pair<asdl::Int, Type> kTypes[] = {
      {8, c99::ULongLong{}},
      {4, c99::ULong{}},
      {2, c99::UInt{}},
      {1, c99::UChar{}},
  };
  for (const auto& entry : kTypes) {
    if (left >= entry.first) {
      return entry;
    }
  }
  return kTypes[3];
}

const std::string& tag_of(const Type& t) {
  if (const auto* s = std::get_if<c99::Structure>(&t)) {
    return s->tag;
  }
  return std::get<c99::Union>(t).tag;
}

// The indices of a subscript of a static array, outermost first, and
// the array's variable: `arr[i]` gives `[i]`, and `arr[i][j]`, whose
// inner array the type checker decayed, gives `[i, j]`. None for any
// other shape of `array`.
struct Subscripts {
  std::vector<const Exp*> indices;
  const c99::Var* base = nullptr;
};

std::optional<Subscripts> static_subscripts(const Exp& array, const Exp& index) {
  std::vector<const Exp*> inner_to_outer = {&index};
  const Exp* cur = &array;
  while (true) {
    const auto* a = std::get_if<c99::AddressOf>(cur);
    if (a == nullptr) {
      return std::nullopt;
    }
    const Exp& inner = *a->exp;
    if (const auto* v = std::get_if<c99::Var>(&inner)) {
      std::reverse(inner_to_outer.begin(), inner_to_outer.end());
      return Subscripts{.indices = std::move(inner_to_outer), .base = v};
    }
    const auto* s = std::get_if<c99::Subscript>(&inner);
    if (s == nullptr) {
      return std::nullopt;
    }
    inner_to_outer.push_back(&*s->index);
    cur = &*s->array;
  }
}

// The dimensions of the static array `t` and the size of its scalar
// element.
struct Dims {
  std::vector<asdl::Int> sizes;
  asdl::Int elem_size = 0;
};

Dims array_dims(const Type& t, const TypeTable* types) {
  Dims d;
  const Type* p = &t;
  while (const auto* a = std::get_if<c99::Array>(p)) {
    d.sizes.push_back(a->size);
    p = &*a->element_type;
  }
  d.elem_size = sizeof_c99(unqualified(*p), types);
  return d;
}

// The flat byte index of a subscript of a multi-dimensional array whose
// indices are all constants, outermost first, or none when one isn't a
// constant or the element lies outside the array.
std::optional<asdl::Int> flat_byte_index(const Subscripts& subs, const Dims& dims, asdl::Int total) {
  std::vector<asdl::Int> constants;
  for (const Exp* idx : subs.indices) {
    const auto v = const_int_or_none(idx);
    if (!v) {
      return std::nullopt;
    }
    constants.push_back(*v);
  }
  asdl::Int flat = 0;
  for (std::size_t level = 0; level < constants.size(); ++level) {
    asdl::Int stride = dims.elem_size;
    for (std::size_t inner = level + 1; inner < dims.sizes.size(); ++inner) {
      stride *= dims.sizes[inner];
    }
    flat += constants[level] * stride;
  }
  if (flat < 0 || flat >= total) {
    return std::nullopt;
  }
  return flat;
}

}  // namespace

// Stores the initial value of the automatic aggregate `vd`, an array,
// struct, or union initialized by a brace-enclosed list or (for a char
// array) a string literal.
//
// The address of the object is taken once. `init_leaves` lists the
// scalar stores the initializer makes, in order of offset, and the zero
// bytes it leaves: the subobjects the list doesn't initialize, which C23
// §6.7.11p20 subjects to default initialization (zero, or a null
// pointer, which is zero bytes here), the explicit zero constants, and a
// union's bytes past its member when it's default-initialized or its
// list has a designator. Adjacent zero bytes are merged into one run,
// and `emit_zero_bytes` clears each run. So `struct s v = {};`, C23's
// empty initializer, and `struct s v = {0};` both clear the object as
// one run. The initializer's expressions are evaluated in the order of
// the bytes they initialize, which C23 §6.7.11p24 leaves unspecified.
void Translator::emit_initializer(const c99::Type_var_decl& vd, Instrs& instrs) {
  std::optional<Val> base;
  std::vector<InitLeaf> leaves;
  init_leaves(vd.data_type, vd.init ? &*vd.init : nullptr, 0, leaves);
  leaves.push_back(InitLeaf{});  // the end
  const std::size_t end = leaves.size() - 1;
  asdl::Int run_start = 0;
  asdl::Int run_end = 0;
  for (std::size_t i = 0; i < leaves.size(); ++i) {
    const InitLeaf& leaf = leaves[i];
    const bool is_end = i == end;
    if (!is_end && leaf.zero && run_end > run_start && leaf.offset == run_end) {
      run_end += leaf.n;
      continue;
    }
    if (run_end > run_start) {
      emit_zero_bytes(vd.name, base, run_start, run_end - run_start, instrs);
    }
    run_start = run_end = 0;
    if (is_end) {
      break;
    }
    if (leaf.zero) {
      run_start = leaf.offset;
      run_end = leaf.offset + leaf.n;
      continue;
    }
    const Val val = leaf.constant ? *leaf.constant : translate_value(*leaf.exp, instrs);
    const Val b = base_address(vd.name, base, instrs);
    const Val dst_ptr = add_offset(b, leaf.offset, *leaf.type, instrs);
    instrs.push_back(tac::Store{.src = val, .dst_ptr = dst_ptr, .is_volatile = is_volatile_quals(*leaf.type)});
  }
}

// Returns the object's address, taken just before the first store
// through it. A clear loop takes its own.
Val Translator::base_address(const std::string& name, std::optional<Val>& base, Instrs& instrs) {
  if (!base) {
    tac::Var b = temp_var(Type{c99::Pointer{.referenced_type = c99::UChar{}}});
    instrs.push_back(tac::GetAddress{.operand = tac::Var{.name = name}, .dst = b});
    base = b;
  }
  return *base;
}

// Appends to `out` what initializing an object of type `t` at byte
// `offset` with `init` stores, in order of offset. `init` is null for a
// subobject the enclosing list doesn't reach, and an empty InitList is
// C23's empty initializer. Both mean default initialization (C23
// §6.7.11p11).
//
// A volatile scalar is always a store of its own, so that its access
// stays one `Store` marked volatile. Structs have no padding in c6502,
// since nothing is aligned, but a union is as big as its biggest member.
// A default-initialized union is zero in all its bytes, since
// §6.7.11p11 zeroes its padding. A union initialized by a list without a
// designator leaves the bytes past its first member alone, since their
// value is unspecified then. A union whose list has a designator
// (`InitList.union_member`) gets zero in the bytes past the member it
// initializes, as a static one does.
void Translator::init_leaves(const Type& t, const Exp* init, asdl::Int offset,
                             std::vector<InitLeaf>& out) {
  const Type& t_unq = unqualified(t);
  const auto* list = init != nullptr ? std::get_if<c99::InitList>(init) : nullptr;
  static const std::vector<Exp> kNoItems;
  const std::vector<Exp>& items = list != nullptr ? list->items : kNoItems;
  if (const auto* a = std::get_if<c99::Array>(&t_unq)) {
    const Type& elem = *a->element_type;
    if (const auto* s = init != nullptr ? std::get_if<c99::String>(init) : nullptr;
        s != nullptr && is_char_element(elem)) {
      const std::vector<std::uint32_t> cps = pyrepr::code_points(s->str);
      for (asdl::Int i = 0; i < a->size; ++i) {
        const auto k = static_cast<std::size_t>(i);
        const asdl::Int byte = k < cps.size() ? asdl::Int(cps[k] & 0xFF) : asdl::Int(0);
        if (byte == 0 && !is_volatile_quals(elem)) {
          out.push_back(InitLeaf{.zero = true, .offset = offset + i, .n = 1});
        } else {
          out.push_back(InitLeaf{.offset = offset + i, .constant = tac_const_val(elem, byte), .type = elem});
        }
      }
      return;
    }
    const asdl::Int elem_size = sizeof_c99(elem, types);
    for (asdl::Int i = 0; i < a->size; ++i) {
      const auto k = static_cast<std::size_t>(i);
      init_leaves(elem, k < items.size() ? &items[k] : nullptr, offset + i * elem_size, out);
    }
    return;
  }
  if (asdl::isa<c99::Structure, c99::Union>(t_unq)) {
    if (types == nullptr) {
      throw InternalError("struct init lowering requires TypeTable");
    }
    const passes::StructLayout* layout = types->find(tag_of(t_unq));
    if (layout == nullptr || !layout->complete) {
      throw InternalError("incomplete struct/union " + tag_of(t_unq));
    }
    const bool is_union = std::holds_alternative<c99::Union>(t_unq);
    std::vector<const passes::MemberInfo*> members;
    for (const passes::MemberInfo& m : layout->members) {
      members.push_back(&m);
      if (is_union) {
        break;
      }
    }
    const std::optional<std::string> chosen =
        is_union && list != nullptr ? list->union_member : std::nullopt;
    if (chosen) {
      members.clear();
      for (const passes::MemberInfo& m : layout->members) {
        if (m.name == *chosen) {
          members.push_back(&m);
        }
      }
    }
    for (std::size_t i = 0; i < members.size(); ++i) {
      init_leaves(members[i]->type, i < items.size() ? &items[i] : nullptr,
                  offset + members[i]->byte_offset, out);
    }
    if (is_union && (items.empty() || chosen)) {
      const asdl::Int first = members.empty() ? 0 : sizeof_c99(members[0]->type, types);
      if (layout->size > first) {
        out.push_back(InitLeaf{.zero = true, .offset = offset + first, .n = layout->size - first});
      }
    }
    return;
  }
  if (init == nullptr || is_zero_constant(*init)) {
    if (is_volatile_quals(t)) {
      out.push_back(InitLeaf{.offset = offset, .constant = tac_const_val(t, 0), .type = t});
    } else {
      out.push_back(InitLeaf{.zero = true, .offset = offset, .n = sizeof_c99(t, types)});
    }
    return;
  }
  out.push_back(InitLeaf{.offset = offset, .exp = init, .type = t});
}

// Stores zero in the `n` bytes at `offset` in the object `name`. `base`
// holds the object's address, taken once for the whole initializer. A
// run of at least `kClearLoopBytes` bytes gets a loop for each 256
// bytes, which counts a byte index down to zero.
//
//       i = 256 or what's left    ; unsigned char, 0 for 256
//   .clear@N:
//       i = i - 1
//       Store(0, &name + offset + i)
//       if (i != 0) goto .clear@N
//
// A shorter run gets one constant store per 8, 4, 2, or 1 bytes, widest
// first.
//
// Under -O, with `v` in a function's pool, the loop is `LDY #n; LDA #0;
// DEY; STA v,Y; BNE`, where passes/loop_constant_a.py has moved the `LDA
// #0` out of the loop and dropped the `TYA` that tested the index. The
// loop takes the object's address again in each pass, as `((unsigned
// char *)&v)[i] = 0` in C would. `memory_value_propagation` then sees
// the address's halves in the pointer's bytes right before the store, so
// the store is `v,Y` and the `GetAddress` goes. With the address taken
// once before the loop, the store through the pointer would kill that
// fact on the way around the loop, and the store would stay `(p),Y`.
// Storing before the decrement would put the store at `v-1`, an offset
// that `name+65535` stands for in the asm IR.
void Translator::emit_zero_bytes(const std::string& name, std::optional<Val>& base,
                                 asdl::Int offset, asdl::Int n, Instrs& instrs) {
  const Type uchar = c99::UChar{};
  const Type uchar_ptr = c99::Pointer{.referenced_type = c99::UChar{}};
  while (n >= kClearLoopBytes) {
    const asdl::Int chunk = std::min(n, asdl::Int(256));
    tac::Var i = temp_var(uchar);
    instrs.push_back(tac::Copy{.src = tac_const_val(uchar, chunk & 0xFF), .dst = i});
    const std::string top = make_label("clear");
    instrs.push_back(tac::Label{.name = top});
    instrs.push_back(tac::Binary{.op = tac::Subtract{}, .src1 = i, .src2 = tac_const_val(uchar, 1), .dst = i});
    tac::Var wide = temp_var(Type{c99::UInt{}});
    instrs.push_back(tac::ZeroExtend{.src = i, .dst = wide});
    tac::Var start = temp_var(uchar_ptr);
    instrs.push_back(tac::GetAddress{.operand = tac::Var{.name = name}, .dst = start});
    const Val p = add_offset(start, offset, uchar, instrs);
    tac::Var addr = temp_var(uchar_ptr);
    instrs.push_back(tac::Binary{.op = tac::Add{}, .src1 = p, .src2 = wide, .dst = addr});
    instrs.push_back(tac::Store{.src = tac_const_val(uchar, 0), .dst_ptr = addr, .is_volatile = false});
    instrs.push_back(tac::JumpIfTrue{.condition = i, .target = top});
    offset += chunk;
    n -= chunk;
  }
  const asdl::Int end = offset + n;
  while (offset < end) {
    const auto& [width, zt] = zero_store_type(end - offset);
    const Val zero = tac_const_val(zt, 0);
    const Val b = base_address(name, base, instrs);
    const Val dst_ptr = add_offset(b, offset, zt, instrs);
    instrs.push_back(tac::Store{.src = zero, .dst_ptr = dst_ptr, .is_volatile = false});
    offset += width;
  }
}

// Returns the byte offset of `member` in `struct_type`'s layout, which
// the type checker has checked.
asdl::Int Translator::member_offset(const Type& struct_type, std::string_view member) const {
  const Type& t = unqualified(struct_type);
  const passes::StructLayout* layout = types != nullptr ? types->find(tag_of(t)) : nullptr;
  if (layout != nullptr) {
    for (const passes::MemberInfo& m : layout->members) {
      if (m.name == member) {
        return m.byte_offset;
      }
    }
  }
  throw InternalError("struct/union " + pyrepr::string(tag_of(t)) + " has no member " +
                      pyrepr::string(member) + " (type checker should have caught this)");
}

// Returns the declared type of the first member named `member`.
const Type& Translator::member_type(const Type& struct_type, std::string_view member) const {
  const Type& t = unqualified(struct_type);
  const passes::StructLayout* layout = types != nullptr ? types->find(tag_of(t)) : nullptr;
  if (layout != nullptr) {
    for (const passes::MemberInfo& m : layout->members) {
      if (m.name == member) {
        return m.type;
      }
    }
  }
  throw InternalError("struct/union has no member " + std::string(member));
}

// Returns `base + offset` as a pointer-typed temporary, or `base` itself
// for an offset of 0.
Val Translator::add_offset(const Val& base, asdl::Int offset, const Type& elem_type, Instrs& instrs) {
  if (offset == 0) {
    return base;
  }
  tac::Var addr = temp_var(Type{c99::Pointer{.referenced_type = elem_type}});
  instrs.push_back(tac::Binary{
      .op = tac::Add{}, .src1 = base, .src2 = tac_const_val(c99::Int{}, offset), .dst = addr});
  return addr;
}

// Computes the byte address of an lvalue, for the parent struct of a
// `.` and for `&` of a member. Besides the addressable forms (a
// variable, `*p`, `a[i]`, `s.m`, `p->m`) it takes a struct or union
// rvalue such as `f().x` or `(c ? a : b).x`, whose translation gives a
// struct-typed temporary (the sret slot of a call, the destination of a
// `?:`), which is addressable.
Val Translator::translate_lvalue_address(const Exp& lval_in, Instrs& instrs) {
  std::optional<Exp> holder;
  const Exp* lval = &lval_in;
  if (const auto* cl = std::get_if<c99::CompoundLiteral>(lval)) {
    holder = materialize_compound_literal(*cl, instrs);
    lval = &*holder;
  }
  switch (c99::kind(*lval)) {
    case EK::Var: {
      const auto& v = std::get<c99::Var>(*lval);
      tac::Var base = temp_var(Type{c99::Pointer{.referenced_type = need_type(v.data_type)}});
      instrs.push_back(tac::GetAddress{.operand = tac::Var{.name = v.name}, .dst = base});
      return base;
    }
    case EK::Dereference:
      return translate_value(*std::get<c99::Dereference>(*lval).exp, instrs);
    case EK::Subscript: {
      const auto& s = std::get<c99::Subscript>(*lval);
      return translate_subscript_address(*s.array, *s.index, instrs);
    }
    case EK::Dot: {
      const auto& d = std::get<c99::Dot>(*lval);
      return translate_dot_address(*d.operand, d.member, instrs);
    }
    case EK::Arrow: {
      const auto& d = std::get<c99::Arrow>(*lval);
      return translate_arrow_address(*d.operand, d.member, instrs);
    }
    default:
      break;
  }
  const std::optional<Type>& t = data_type_of(*lval);
  if (t && asdl::isa<c99::Structure, c99::Union>(*t)) {
    const Val val = translate_value(*lval, instrs);
    if (!std::holds_alternative<tac::Var>(val)) {
      throw InternalError("struct rvalue translation didn't produce a Var");
    }
    tac::Var base = temp_var(Type{c99::Pointer{.referenced_type = *t}});
    instrs.push_back(tac::GetAddress{.operand = val, .dst = base});
    return base;
  }
  throw InternalError("not an addressable lvalue");
}

// Computes the byte address of `e.m`: the address of the struct or union
// lvalue `e`, plus the member's offset.
Val Translator::translate_dot_address(const Exp& operand, std::string_view member, Instrs& instrs) {
  const Type struct_type = unqualified(need_type(data_type_of(operand)));
  const Val base = translate_lvalue_address(operand, instrs);
  const asdl::Int offset = member_offset(struct_type, member);
  const Type m_type = member_type(struct_type, member);
  return add_offset(base, offset, m_type, instrs);
}

// Computes the byte address of `p->m`: the pointer's value plus the
// member's offset.
Val Translator::translate_arrow_address(const Exp& operand, std::string_view member,
                                        Instrs& instrs) {
  const Type& ptr_type = unqualified(need_type(data_type_of(operand)));
  const auto* p = std::get_if<c99::Pointer>(&ptr_type);
  if (p == nullptr) {
    throw InternalError("the operand of -> isn't a pointer");
  }
  const Type struct_type = unqualified(*p->referenced_type);
  const Val ptr_val = translate_value(operand, instrs);
  const asdl::Int offset = member_offset(struct_type, member);
  const Type m_type = member_type(struct_type, member);
  return add_offset(ptr_val, offset, m_type, instrs);
}

// Emits an `IndexedLoad` and returns its destination when `array[index]`
// qualifies for absolute,Y addressing, which is when the array is an
// object with static storage (its address is a link-time label) of at
// most 256 bytes, so that the byte index of any access within it fits
// in Y, and the subscript reaches a scalar (a partial subscript gives a
// subarray). A single subscript `arr[i]` has the type checker's decay
// `AddressOf(Var(name))` as its array, and its index may be computed at
// run time (`byte_index`). A multi-dimensional subscript qualifies only
// when every index is a constant, whose flat byte index is computed
// here; one with an index computed at run time takes the general path.
//
// A cast of a static object's address, `((uint8_t *)&score)[i]`, reads
// the object as an array of the pointed-to type (`object_view`), and
// lowers as a single subscript of an array at the object's address does.
std::optional<Val> Translator::try_indexed_load_subscript(const Exp& array, const Exp& index,
                                                          const std::optional<Type>& data_type,
                                                          Instrs& instrs) {
  if (const auto view = static_object_view(array)) {
    const auto got = byte_index(index, view->total, view->elem, instrs);
    if (!got) {
      return std::nullopt;
    }
    tac::Var dst = temp_var(data_type);
    instrs.push_back(tac::IndexedLoad{
        .name = view->name,
        .index = got->first,
        .dst = dst,
        .is_volatile = view->obj_volatile || is_volatile_quals(data_type),
        .offset = got->second,
    });
    return dst;
  }
  const auto subs = static_subscripts(array, index);
  if (!subs) {
    return std::nullopt;
  }
  const passes::Symbol* sym = symbols.find(subs->base->name);
  if (sym == nullptr || !std::holds_alternative<passes::StaticAttr>(sym->attrs) ||
      !std::holds_alternative<c99::Array>(sym->type)) {
    return std::nullopt;
  }
  const asdl::Int total = sizeof_c99(sym->type, types);
  if (total > 256) {
    return std::nullopt;
  }
  const Dims dims = array_dims(sym->type, types);
  if (subs->indices.size() != dims.sizes.size()) {
    return std::nullopt;
  }
  const std::string name = subs->base->name;
  if (dims.sizes.size() >= 2) {
    const auto flat = flat_byte_index(*subs, dims, total);
    if (!flat) {
      return std::nullopt;
    }
    tac::Var dst = temp_var(data_type);
    instrs.push_back(tac::IndexedLoad{
        .name = name,
        .index = tac::Constant{.const_ = tac::ConstUChar{.value = *flat}},
        .dst = dst,
        .is_volatile = is_volatile_quals(data_type),
    });
    return dst;
  }
  const auto got = byte_index(*subs->indices[0], total, dims.elem_size, instrs);
  if (!got) {
    return std::nullopt;
  }
  tac::Var dst = temp_var(data_type);
  instrs.push_back(tac::IndexedLoad{
      .name = name,
      .index = got->first,
      .dst = dst,
      .is_volatile = is_volatile_quals(data_type),
      .offset = got->second,
  });
  return dst;
}

// Returns the name, the element size, the object size, and whether the
// object itself is volatile when `array` is a cast of the address of an
// object with static storage (`object_view`). Every access to a
// volatile object through the cast is made volatile too.
std::optional<Translator::StaticView> Translator::static_object_view(const Exp& array) const {
  const auto view = object_view(array, types);
  if (!view) {
    return std::nullopt;
  }
  const passes::Symbol* sym = symbols.find(view->obj->name);
  if (sym == nullptr || !std::holds_alternative<passes::StaticAttr>(sym->attrs)) {
    return std::nullopt;
  }
  bool obj_volatile = false;
  const Type* t = &sym->type;
  while (true) {
    if (const auto* c = std::get_if<c99::Const>(t)) {
      t = &*c->referenced_type;
    } else if (const auto* v = std::get_if<c99::Volatile>(t)) {
      obj_volatile = true;
      t = &*v->referenced_type;
    } else if (const auto* a = std::get_if<c99::Array>(t)) {
      t = &*a->element_type;
    } else {
      break;
    }
  }
  return StaticView{
      .name = view->obj->name, .elem = view->elem, .total = view->total, .obj_volatile = obj_volatile};
}

// Returns `(byte index, offset)` for the subscript `index` into a static
// array of `elem_size`-byte elements that spans `total` bytes (at most
// 256), for an `IndexedLoad` or `IndexedSymbolStore`.
//
// A constant index is the constant byte offset of its element, so `t[2]`
// reads `t+2` with no index register. It's none when that element lies
// outside the array, which C leaves undefined, and the access then goes
// through a pointer. Any other index is evaluated, truncated to a byte,
// and multiplied by the element size, since every index C defines
// leaves the byte offset within the array. A constant added to the
// index goes in the offset (`split_index_addend`).
//
// An `extern T t[]` declared without a size has a `total` of 0, and is
// taken to span 256 bytes, which its definition must not exceed for any
// of its indexed accesses to work.
std::optional<std::pair<Val, asdl::Int>> Translator::byte_index(const Exp& index, asdl::Int total,
                                                                asdl::Int elem_size, Instrs& instrs) {
  if (total == 0) {
    total = 256;
  }
  if (const auto k = exact_const_index(index, types)) {
    if (!(0 <= *k * elem_size && *k * elem_size < total)) {
      return std::nullopt;
    }
    return std::pair<Val, asdl::Int>{tac::Constant{.const_ = tac::ConstUChar{.value = *k * elem_size}}, 0};
  }
  const Exp* idx_exp = &index;
  asdl::Int addend = 0;
  if (const auto split = split_index_addend(index, total, elem_size, types)) {
    idx_exp = split->first;
    addend = split->second;
  }
  const Val idx_val = translate_value(*idx_exp, instrs);
  const Type uchar = c99::UChar{};
  tac::Var byte = temp_var(uchar);
  instrs.push_back(tac::Truncate{.src = idx_val, .dst = byte});
  if (elem_size == 1) {
    return std::pair<Val, asdl::Int>{byte, addend};
  }
  tac::Var scaled = temp_var(uchar);
  instrs.push_back(tac::Binary{
      .op = tac::Multiply{},
      .src1 = byte,
      .src2 = tac::Constant{.const_ = tac::ConstUChar{.value = elem_size}},
      .dst = scaled,
  });
  return std::pair<Val, asdl::Int>{scaled, addend * elem_size};
}

// The store-side mirror of `try_indexed_load_subscript`: emits an
// `IndexedSymbolStore` and returns true when `array[index]` qualifies,
// or returns false and leaves the store to the general pointer
// arithmetic. The width of the store comes from the symbol-table type of
// `rval_val`, which `tac_to_asm`'s lowering of `IndexedSymbolStore`
// reads.
bool Translator::try_indexed_store_subscript(const Exp& array, const Exp& index,
                                             const Val& rval_val, bool is_volatile,
                                             Instrs& instrs) {
  if (const auto view = static_object_view(array)) {
    const auto got = byte_index(index, view->total, view->elem, instrs);
    if (!got) {
      return false;
    }
    instrs.push_back(tac::IndexedSymbolStore{
        .name = view->name,
        .index = got->first,
        .src = rval_val,
        .is_volatile = is_volatile || view->obj_volatile,
        .offset = got->second,
    });
    return true;
  }
  const auto subs = static_subscripts(array, index);
  if (!subs) {
    return false;
  }
  const passes::Symbol* sym = symbols.find(subs->base->name);
  if (sym == nullptr || !std::holds_alternative<passes::StaticAttr>(sym->attrs) ||
      !std::holds_alternative<c99::Array>(sym->type)) {
    return false;
  }
  const asdl::Int total = sizeof_c99(sym->type, types);
  if (total > 256) {
    return false;
  }
  const Dims dims = array_dims(sym->type, types);
  if (subs->indices.size() != dims.sizes.size()) {
    return false;
  }
  const std::string name = subs->base->name;
  if (dims.sizes.size() >= 2) {
    const auto flat = flat_byte_index(*subs, dims, total);
    if (!flat) {
      return false;
    }
    instrs.push_back(tac::IndexedSymbolStore{
        .name = name,
        .index = tac::Constant{.const_ = tac::ConstUChar{.value = *flat}},
        .src = rval_val,
        .is_volatile = is_volatile,
    });
    return true;
  }
  const auto got = byte_index(*subs->indices[0], total, dims.elem_size, instrs);
  if (!got) {
    return false;
  }
  instrs.push_back(tac::IndexedSymbolStore{
      .name = name,
      .index = got->first,
      .src = rval_val,
      .is_volatile = is_volatile,
      .offset = got->second,
  });
  return true;
}

// Computes the byte address of `array[index]`. The type checker has
// decayed an array operand to a pointer, so this is pointer arithmetic,
// and `translate_pointer_arithmetic` scales the index by the size of an
// element. Both the load and the store of an element go through the
// address.
//
// When `array` is the implicit decay of an array object whose elements
// end within 255 bytes of its start, every index that C defines lands on
// one of those bytes (C99 §6.5.6p8 leaves anything else undefined), so
// the offset fits a byte. The index is then truncated to a byte, scaled
// there, and zero-extended. A pointer operand isn't an array object, so
// it takes the general path. `prompts[which]` on a
// `struct prompt prompts[3]` of 16-byte records shifts `which` left 4 in
// A instead of shifting a 16-bit value. An array of one element is left
// alone, since a struct's last `[1]` member is often indexed past its
// end.
Val Translator::translate_subscript_address(const Exp& array, const Exp& index, Instrs& instrs) {
  const Val arr_val = translate_value(array, instrs);
  const Val idx_val = translate_value(index, instrs);
  const auto scale = byte_offset_scale(array, types);
  if (scale && !std::holds_alternative<tac::Constant>(idx_val)) {
    const Type uchar = c99::UChar{};
    tac::Var byte = temp_var(uchar);
    instrs.push_back(tac::Truncate{.src = idx_val, .dst = byte});
    if (*scale != 1) {
      tac::Var scaled = temp_var(uchar);
      instrs.push_back(tac::Binary{
          .op = tac::Multiply{}, .src1 = byte, .src2 = tac_const_val(uchar, *scale), .dst = scaled});
      byte = scaled;
    }
    tac::Var offset = temp_var(Type{c99::UInt{}});
    instrs.push_back(tac::ZeroExtend{.src = byte, .dst = offset});
    tac::Var dst = temp_var(strip_quals(need_type(data_type_of(array))));
    instrs.push_back(tac::Binary{.op = tac::Add{}, .src1 = arr_val, .src2 = offset, .dst = dst});
    return dst;
  }
  const Type& array_t = need_type(data_type_of(array));
  return translate_pointer_arithmetic(c99::Add{}, arr_val, idx_val, array_t,
                                      need_type(data_type_of(index)), array_t, instrs);
}

// Evaluates the pointer `ptr` of a `*ptr` that's read and written in
// place. `*((uint8_t *)&obj + k)` is the subscript `((uint8_t *)&obj)[k]`,
// whose offset `translate_subscript_address` computes in a byte. A plain
// `*(uint8_t *)&obj` has no offset to compute.
Val Translator::translate_deref_address(const Exp& ptr, Instrs& instrs) {
  if (std::holds_alternative<c99::Binary>(ptr)) {
    if (const auto sub = deref_as_subscript(ptr)) {
      return translate_subscript_address(sub->first, sub->second, instrs);
    }
  }
  return translate_value(ptr, instrs);
}

}  // namespace c6502::c99_to_tac
