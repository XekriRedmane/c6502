#include "passes/optimization/var_visit.h"

#include <variant>

namespace c6502::passes {

namespace ir = ::c6502::tac_ast;
using K = ir::Kind_instruction;

namespace {

// Calls `f` on the source and destination of each one-source,
// one-destination instruction, the conversions, `Unary`, and `Copy`,
// and returns whether `instr` is one.
template <class F>
bool with_src_dst(const ir::Type_instruction& instr, F f) {
  switch (ir::kind(instr)) {
#define C6502_SRC_DST(Name)                       \
  case K::Name: {                                 \
    const auto& i = std::get<ir::Name>(instr);    \
    f(i.src, i.dst);                              \
    return true;                                  \
  }
    C6502_SRC_DST(SignExtend)
    C6502_SRC_DST(ZeroExtend)
    C6502_SRC_DST(Truncate)
    C6502_SRC_DST(IntToFloat)
    C6502_SRC_DST(IntToDouble)
    C6502_SRC_DST(FloatToInt)
    C6502_SRC_DST(DoubleToInt)
    C6502_SRC_DST(FloatToDouble)
    C6502_SRC_DST(DoubleToFloat)
    C6502_SRC_DST(Unary)
    C6502_SRC_DST(Copy)
#undef C6502_SRC_DST
    default:
      return false;
  }
}

void add_var(std::vector<std::string_view>& out, const ir::Type_val& v) {
  if (const auto* var = std::get_if<ir::Var>(&v)) {
    out.push_back(var->name);
  }
}

}  // namespace

std::vector<const ir::Type_val*> vals_in(const ir::Type_instruction& instr) {
  std::vector<const ir::Type_val*> out;
  if (with_src_dst(instr, [&](const ir::Type_val& s, const ir::Type_val& d) {
        out.push_back(&s);
        out.push_back(&d);
      })) {
    return out;
  }
  switch (ir::kind(instr)) {
    case K::Ret: {
      const auto& i = std::get<ir::Ret>(instr);
      if (i.val) {
        out.push_back(&*i.val);
      }
      break;
    }
    case K::GetAddress: {
      const auto& i = std::get<ir::GetAddress>(instr);
      out = {&i.operand, &i.dst};
      break;
    }
    case K::Load: {
      const auto& i = std::get<ir::Load>(instr);
      out = {&i.src_ptr, &i.dst};
      break;
    }
    case K::Store: {
      const auto& i = std::get<ir::Store>(instr);
      out = {&i.src, &i.dst_ptr};
      break;
    }
    case K::IndexedLoad: {
      const auto& i = std::get<ir::IndexedLoad>(instr);
      out = {&i.index, &i.dst};
      break;
    }
    case K::IndexedStore: {
      const auto& i = std::get<ir::IndexedStore>(instr);
      out = {&i.index, &i.src};
      break;
    }
    case K::IndexedSymbolStore: {
      const auto& i = std::get<ir::IndexedSymbolStore>(instr);
      out = {&i.index, &i.src};
      break;
    }
    case K::IndexedConstLoad: {
      const auto& i = std::get<ir::IndexedConstLoad>(instr);
      out = {&i.index, &i.dst};
      break;
    }
    case K::IndirectIndexedLoad: {
      const auto& i = std::get<ir::IndirectIndexedLoad>(instr);
      out = {&i.ptr, &i.index, &i.dst};
      break;
    }
    case K::IndirectIndexedStore: {
      const auto& i = std::get<ir::IndirectIndexedStore>(instr);
      out = {&i.ptr, &i.index, &i.src};
      break;
    }
    case K::Binary: {
      const auto& i = std::get<ir::Binary>(instr);
      out = {&i.src1, &i.src2, &i.dst};
      break;
    }
    case K::JumpIfTrue:
      out = {&std::get<ir::JumpIfTrue>(instr).condition};
      break;
    case K::JumpIfFalse:
      out = {&std::get<ir::JumpIfFalse>(instr).condition};
      break;
    case K::JumpIfCmp: {
      const auto& i = std::get<ir::JumpIfCmp>(instr);
      out = {&i.src1, &i.src2};
      break;
    }
    case K::JumpIfMasked:
      out = {&std::get<ir::JumpIfMasked>(instr).val};
      break;
    case K::FunctionCall: {
      const auto& i = std::get<ir::FunctionCall>(instr);
      for (const auto& a : i.args) {
        out.push_back(&a);
      }
      if (i.dst) {
        out.push_back(&*i.dst);
      }
      break;
    }
    case K::IndirectCall: {
      const auto& i = std::get<ir::IndirectCall>(instr);
      out.push_back(&i.ptr);
      for (const auto& a : i.args) {
        out.push_back(&a);
      }
      if (i.dst) {
        out.push_back(&*i.dst);
      }
      break;
    }
    case K::Phi: {
      const auto& i = std::get<ir::Phi>(instr);
      out.push_back(&i.dst);
      for (const auto& a : i.args) {
        out.push_back(&std::get<ir::PhiArg>(a).source);
      }
      break;
    }
    default:
      break;
  }
  return out;
}

std::vector<std::string_view> defs_in(const ir::Type_instruction& instr) {
  std::vector<std::string_view> out;
  if (with_src_dst(instr, [&](const ir::Type_val&, const ir::Type_val& d) { add_var(out, d); })) {
    return out;
  }
  switch (ir::kind(instr)) {
    case K::Binary:
      add_var(out, std::get<ir::Binary>(instr).dst);
      break;
    case K::GetAddress:
      add_var(out, std::get<ir::GetAddress>(instr).dst);
      break;
    case K::Load:
      add_var(out, std::get<ir::Load>(instr).dst);
      break;
    case K::IndexedLoad:
      add_var(out, std::get<ir::IndexedLoad>(instr).dst);
      break;
    case K::IndexedConstLoad:
      add_var(out, std::get<ir::IndexedConstLoad>(instr).dst);
      break;
    case K::Phi:
      add_var(out, std::get<ir::Phi>(instr).dst);
      break;
    case K::FunctionCall: {
      const auto& d = std::get<ir::FunctionCall>(instr).dst;
      if (d) {
        add_var(out, *d);
      }
      break;
    }
    case K::IndirectCall: {
      const auto& d = std::get<ir::IndirectCall>(instr).dst;
      if (d) {
        add_var(out, *d);
      }
      break;
    }
    default:
      break;
  }
  return out;
}

std::vector<std::string_view> uses_in(const ir::Type_instruction& instr) {
  std::vector<std::string_view> out;
  if (with_src_dst(instr, [&](const ir::Type_val& s, const ir::Type_val&) { add_var(out, s); })) {
    return out;
  }
  switch (ir::kind(instr)) {
    case K::Ret: {
      const auto& v = std::get<ir::Ret>(instr).val;
      if (v) {
        add_var(out, *v);
      }
      break;
    }
    case K::Binary: {
      const auto& i = std::get<ir::Binary>(instr);
      add_var(out, i.src1);
      add_var(out, i.src2);
      break;
    }
    case K::Load:
      add_var(out, std::get<ir::Load>(instr).src_ptr);
      break;
    case K::Store: {
      const auto& i = std::get<ir::Store>(instr);
      add_var(out, i.src);
      add_var(out, i.dst_ptr);
      break;
    }
    case K::IndexedLoad: {
      const auto& i = std::get<ir::IndexedLoad>(instr);
      out.push_back(i.name);
      add_var(out, i.index);
      break;
    }
    case K::IndexedStore: {
      const auto& i = std::get<ir::IndexedStore>(instr);
      add_var(out, i.index);
      add_var(out, i.src);
      break;
    }
    case K::IndexedSymbolStore: {
      const auto& i = std::get<ir::IndexedSymbolStore>(instr);
      out.push_back(i.name);
      add_var(out, i.index);
      add_var(out, i.src);
      break;
    }
    case K::IndexedConstLoad:
      add_var(out, std::get<ir::IndexedConstLoad>(instr).index);
      break;
    case K::IndirectIndexedLoad: {
      const auto& i = std::get<ir::IndirectIndexedLoad>(instr);
      add_var(out, i.ptr);
      add_var(out, i.index);
      break;
    }
    case K::IndirectIndexedStore: {
      const auto& i = std::get<ir::IndirectIndexedStore>(instr);
      add_var(out, i.ptr);
      add_var(out, i.index);
      add_var(out, i.src);
      break;
    }
    case K::JumpIfTrue:
      add_var(out, std::get<ir::JumpIfTrue>(instr).condition);
      break;
    case K::JumpIfFalse:
      add_var(out, std::get<ir::JumpIfFalse>(instr).condition);
      break;
    case K::JumpIfCmp: {
      const auto& i = std::get<ir::JumpIfCmp>(instr);
      add_var(out, i.src1);
      add_var(out, i.src2);
      break;
    }
    case K::JumpIfMasked:
      add_var(out, std::get<ir::JumpIfMasked>(instr).val);
      break;
    case K::FunctionCall:
      for (const auto& a : std::get<ir::FunctionCall>(instr).args) {
        add_var(out, a);
      }
      break;
    case K::IndirectCall: {
      const auto& i = std::get<ir::IndirectCall>(instr);
      add_var(out, i.ptr);
      for (const auto& a : i.args) {
        add_var(out, a);
      }
      break;
    }
    case K::Phi:
      for (const auto& a : std::get<ir::Phi>(instr).args) {
        add_var(out, std::get<ir::PhiArg>(a).source);
      }
      break;
    default:
      break;
  }
  return out;
}

OrderedMap<std::string, std::size_t> count_uses(const std::vector<ir::Type_instruction>& instrs) {
  OrderedMap<std::string, std::size_t> counts;
  for (const auto& instr : instrs) {
    for (const auto name : uses_in(instr)) {
      if (std::size_t* n = counts.find(name)) {
        ++*n;
      } else {
        counts.insert_or_assign(std::string(name), 1);
      }
    }
  }
  return counts;
}

}  // namespace c6502::passes
