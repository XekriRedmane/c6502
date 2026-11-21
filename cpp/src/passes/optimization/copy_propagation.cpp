/* TAC copy propagation in SSA form.

In SSA form every Var is defined exactly once, so a `Copy(src, dst)`
states the equation `dst = src` everywhere `dst` is read, with nothing
that can kill it. Copy propagation rewrites every read of `dst` to
`src`. The Copy is then a dead store, since its destination has no
reads left, and dead-store elimination removes it in the next sweep.

The algorithm:
  1. Walk every instruction. For each `Copy(src, dst)` whose `dst` is a
     `Var`, record `copy_src[dst] = src`. A `Phi` isn't a copy, since it
     merges several values, so Phi destinations stay out of the map.
  2. Resolve chains. For each entry, follow `src` while it is a Var
     that is itself in the map, until a base value is reached (a
     Constant, or a Var that no Copy defines). SSA admits no cycles,
     since each Var has one definition and `x <- y; y <- x` can't be
     written.
  3. Walk every instruction again, and replace each Var operand that
     is read and is in the resolved map with its base value. A Phi's
     sources are rewritten the same way.

The pass propagates only Copies, which are the unambiguous statements
that one value is exactly another in SSA. `Binary(Add, x, 0, y)` isn't
propagated, though the fold would be correct, since that is constant
folding's work.

The pass is sound only in SSA form. Outside it, a Copy's destination
can be assigned again later, and `dst = src` no longer holds after
that. The driver runs copy propagation only between SSA construction
and destruction. */
#include "passes/optimization/copy_propagation.h"

#include <map>
#include <optional>
#include <set>
#include <string>
#include <utility>

#include "bit_precise.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::tac_ast;
namespace c99 = ::c6502::c99_ast;
using framework::PassContext;
using K = ir::Kind_instruction;
using CK = ir::Kind_const;

using CopySources = std::map<std::string, ir::Type_val, std::less<>>;

// Returns true or false for a Var of an unsigned or signed integer (or
// pointer) type, and none when the type is unknown or isn't an integer.
// It follows `tac_to_asm._is_unsigned_val`.
std::optional<bool> unsigned_of(const ir::Type_val& val, const SymbolTable* symbols) {
  const auto* var = std::get_if<ir::Var>(&val);
  if (symbols == nullptr || var == nullptr) {
    return std::nullopt;
  }
  const Symbol* sym = symbols->find(var->name);
  if (sym == nullptr) {
    return std::nullopt;
  }
  const c99::Type_data_type* t = &sym->type;
  for (;;) {
    if (const auto* c = std::get_if<c99::Const>(t)) {
      t = &*c->referenced_type;
    } else if (const auto* v = std::get_if<c99::Volatile>(t)) {
      t = &*v->referenced_type;
    } else {
      break;
    }
  }
  if (const auto* b = std::get_if<c99::BitInt>(t)) {
    return !b->is_signed;
  }
  using DK = c99::Kind_data_type;
  switch (c99::kind(*t)) {
    case DK::UInt:
    case DK::ULong:
    case DK::ULongLong:
    case DK::Char:
    case DK::UChar:
    case DK::Pointer:
      return true;
    case DK::Int:
    case DK::Long:
    case DK::LongLong:
    case DK::SChar:
      return false;
    default:
      return std::nullopt;
  }
}

// Returns the width in bits of an integer constant class of 1, 2, 4, or
// 8 bytes and whether it is signed, or none for any other class.
std::optional<std::pair<int, bool>> const_bits(CK k) {
  switch (k) {
    case CK::ConstChar:
      return std::pair{8, true};
    case CK::ConstUChar:
      return std::pair{8, false};
    case CK::ConstInt:
      return std::pair{16, true};
    case CK::ConstUInt:
      return std::pair{16, false};
    case CK::ConstLong:
      return std::pair{32, true};
    case CK::ConstULong:
      return std::pair{32, false};
    case CK::ConstLongLong:
      return std::pair{64, true};
    case CK::ConstULongLong:
      return std::pair{64, false};
    default:
      return std::nullopt;
  }
}

// Returns the constant of the class of `bits` bits with the signedness
// `is_signed` holding `value`.
ir::Type_const make_class(int bits, bool is_signed, asdl::Int value) {
  switch (bits) {
    case 8:
      return is_signed ? ir::Type_const{ir::ConstChar{value}} : ir::Type_const{ir::ConstUChar{value}};
    case 16:
      return is_signed ? ir::Type_const{ir::ConstInt{value}} : ir::Type_const{ir::ConstUInt{value}};
    case 32:
      return is_signed ? ir::Type_const{ir::ConstLong{value}} : ir::Type_const{ir::ConstULong{value}};
    default:
      return is_signed ? ir::Type_const{ir::ConstLongLong{value}}
                       : ir::Type_const{ir::ConstULongLong{value}};
  }
}

// Returns `c` expressed in the class of the destination's signedness,
// or `c` itself when the signedness already matches or is unknown.
ir::Constant retag_constant(const ir::Constant& c, std::optional<bool> dst_unsigned) {
  const auto bits = const_bits(ir::kind(c.const_));
  if (!dst_unsigned || !bits) {
    return c;
  }
  const auto [width, is_signed] = *bits;
  const asdl::Int mask = (asdl::Int(1) << width) - 1;
  const asdl::Int raw = *bit_precise::raw_value(c.const_);
  if (*dst_unsigned && is_signed) {
    return ir::Constant{.const_ = make_class(width, false, raw & mask)};
  }
  if (!*dst_unsigned && !is_signed) {
    asdl::Int v = raw & mask;
    if ((v >> (width - 1)) != 0) {
      v -= asdl::Int(1) << width;
    }
    return ir::Constant{.const_ = make_class(width, true, v)};
  }
  return c;
}

// Builds the map `copy_src[dst] = src`. A `Copy(src, dst)` contributes
// only when
//
//   * `dst` is a Var among the SSA names, which are the only names with
//     one definition. A write to a static or an address-taken local can
//     be seen elsewhere.
//   * `src` is a Constant or a Var among the SSA names. A parameter's
//     initial SSA value keeps its original spelling (`@0.p`), which
//     `to_ssa` adds to the SSA names when the parameter is promotable,
//     so the one test covers it too. A read of any other name (a static,
//     an address-taken local) gives its value at one point of the
//     program, and the memory can be written between that read and the
//     next, so propagating it would be wrong.
//
// A Copy whose source and destination differ in signedness is the
// lowering of a cast between types of one width, such as `(unsigned)s`.
// Later passes take the signedness of `/`, `%`, `>>`, and the ordering
// compares from the operand's type, so putting `s` in place of the
// unsigned destination would turn a logical shift into an arithmetic
// one. Such a Copy contributes only when its source is a Constant, which
// is then given the class of the destination's signedness. Without a
// symbol table (synthetic unit tests) the signedness is unknown, and
// every Copy counts as one of a single signedness.
CopySources collect_copy_sources(const ir::Function& fn, const NameSet& ssa,
                                 const SymbolTable* symbols) {
  CopySources out;
  for (const auto& instr : fn.instructions) {
    const auto* copy = std::get_if<ir::Copy>(&instr);
    if (copy == nullptr) {
      continue;
    }
    const auto* dst = std::get_if<ir::Var>(&copy->dst);
    if (dst == nullptr || !ssa.contains(dst->name)) {
      continue;
    }
    const std::optional<bool> dst_unsigned = unsigned_of(copy->dst, symbols);
    if (const auto* c = std::get_if<ir::Constant>(&copy->src)) {
      out.insert_or_assign(dst->name, ir::Type_val{retag_constant(*c, dst_unsigned)});
      continue;
    }
    if (const auto* v = std::get_if<ir::Var>(&copy->src); v != nullptr && ssa.contains(v->name)) {
      if (unsigned_of(copy->src, symbols) != dst_unsigned) {
        continue;
      }
      out.insert_or_assign(dst->name, copy->src);
    }
    // Otherwise the source is a Var that isn't an SSA name (a static or
    // an address-taken local), whose value can change between reads.
  }
  return out;
}

// Follows each entry of `copy_src` through any chain of further Copies
// to its base value, a Constant or a Var that no Copy defines.
CopySources resolve_chains(const CopySources& copy_src) {
  CopySources resolved;
  for (const auto& [dst, first] : copy_src) {
    std::set<std::string, std::less<>> seen;
    const ir::Type_val* cur = &first;
    for (;;) {
      const auto* v = std::get_if<ir::Var>(cur);
      if (v == nullptr) {
        break;
      }
      const auto it = copy_src.find(v->name);
      if (it == copy_src.end()) {
        break;
      }
      if (seen.contains(v->name)) {
        // SSA admits no cycles, but a malformed input stops at the
        // cycle in place of spinning.
        break;
      }
      seen.insert(v->name);
      cur = &it->second;
    }
    resolved.insert_or_assign(dst, *cur);
  }
  return resolved;
}

// Replaces every Var that `instr` reads with its resolved base value.
// The definitions stay, since their names are the SSA values propagated
// from. The instructions this leaves alone are those Python's `_rewrite`
// has no case for: a `GetAddress`, whose operand names a storage cell,
// the loads and stores at a fixed address or through a pointer and an
// index, and the jumps and labels without operands.
void rewrite(ir::Type_instruction& instr, const CopySources& resolved) {
  const auto sub = [&](ir::Type_val& v) {
    if (const auto* var = std::get_if<ir::Var>(&v)) {
      const auto it = resolved.find(var->name);
      if (it != resolved.end()) {
        v = it->second;
      }
    }
  };
  switch (ir::kind(instr)) {
    case K::Ret: {
      auto& r = std::get<ir::Ret>(instr);
      if (r.val) {
        sub(*r.val);
      }
      return;
    }
    case K::SignExtend:
      sub(std::get<ir::SignExtend>(instr).src);
      return;
    case K::ZeroExtend:
      sub(std::get<ir::ZeroExtend>(instr).src);
      return;
    case K::Truncate:
      sub(std::get<ir::Truncate>(instr).src);
      return;
    case K::IntToFloat:
      sub(std::get<ir::IntToFloat>(instr).src);
      return;
    case K::IntToDouble:
      sub(std::get<ir::IntToDouble>(instr).src);
      return;
    case K::FloatToInt:
      sub(std::get<ir::FloatToInt>(instr).src);
      return;
    case K::DoubleToInt:
      sub(std::get<ir::DoubleToInt>(instr).src);
      return;
    case K::FloatToDouble:
      sub(std::get<ir::FloatToDouble>(instr).src);
      return;
    case K::DoubleToFloat:
      sub(std::get<ir::DoubleToFloat>(instr).src);
      return;
    case K::Unary:
      sub(std::get<ir::Unary>(instr).src);
      return;
    case K::Binary: {
      auto& b = std::get<ir::Binary>(instr);
      sub(b.src1);
      sub(b.src2);
      return;
    }
    case K::Copy:
      sub(std::get<ir::Copy>(instr).src);
      return;
    case K::Load:
      sub(std::get<ir::Load>(instr).src_ptr);
      return;
    case K::Store: {
      auto& s = std::get<ir::Store>(instr);
      sub(s.src);
      sub(s.dst_ptr);
      return;
    }
    case K::IndexedLoad:
      sub(std::get<ir::IndexedLoad>(instr).index);
      return;
    case K::IndexedStore: {
      auto& s = std::get<ir::IndexedStore>(instr);
      sub(s.index);
      sub(s.src);
      return;
    }
    case K::IndexedSymbolStore: {
      auto& s = std::get<ir::IndexedSymbolStore>(instr);
      sub(s.index);
      sub(s.src);
      return;
    }
    case K::JumpIfTrue:
      sub(std::get<ir::JumpIfTrue>(instr).condition);
      return;
    case K::JumpIfFalse:
      sub(std::get<ir::JumpIfFalse>(instr).condition);
      return;
    case K::JumpIfCmp: {
      auto& j = std::get<ir::JumpIfCmp>(instr);
      sub(j.src1);
      sub(j.src2);
      return;
    }
    case K::JumpIfMasked:
      sub(std::get<ir::JumpIfMasked>(instr).val);
      return;
    case K::FunctionCall:
      for (auto& a : std::get<ir::FunctionCall>(instr).args) {
        sub(a);
      }
      return;
    case K::IndirectCall: {
      auto& c = std::get<ir::IndirectCall>(instr);
      sub(c.ptr);
      for (auto& a : c.args) {
        sub(a);
      }
      return;
    }
    case K::Phi:
      for (auto& a : std::get<ir::Phi>(instr).args) {
        sub(std::get<ir::PhiArg>(a).source);
      }
      return;
    default:
      return;
  }
}

}  // namespace

// The SSA names are the names `to_ssa` made, which are the names whose
// only definition is in this function and isn't repeated. A Copy is a
// candidate only when its destination is one of them, since a static,
// an address-taken local, or any other name that wasn't promoted can be
// written by a call, a `Store`, or another assignment, and propagating
// its Copy's source to its reads would read a stale value. Outside SSA
// form there is no safe way to tell which Copies are SSA ones, so the
// pass does nothing.
tac_ast::Function copy_propagate(tac_ast::Function fn, const PassContext& ctx) {
  if (ctx.ssa_dsts == nullptr) {
    return fn;
  }
  const CopySources copy_src = collect_copy_sources(fn, *ctx.ssa_dsts, ctx.symbols);
  if (copy_src.empty()) {
    return fn;
  }
  const CopySources resolved = resolve_chains(copy_src);
  for (auto& instr : fn.instructions) {
    rewrite(instr, resolved);
  }
  return fn;
}

}  // namespace c6502::passes
