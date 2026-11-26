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
  }
  return out;
}

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
        break;
      }
      seen.insert(v->name);
      cur = &it->second;
    }
    resolved.insert_or_assign(dst, *cur);
  }
  return resolved;
}

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
