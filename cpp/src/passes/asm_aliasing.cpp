// Aliasing rules
//
// Two operands may alias unless they can be proved to name different
// bytes. The rules are conservative, and when in doubt the answer is
// true. These are the refinements over a model where anything indirect
// may alias anything.
//
//   * Frame / Stack and ZP never alias. FP and SSP point into the soft
//     stack, a region of main RAM well above zero page ($0800 and up by
//     c6502's convention), and offsets of 0 to 255 from them never reach
//     [0, 0xFF].
//   * The indirect forms and a ZP byte in the register allocator's pool
//     never alias. The pool ([Pool.start, 0xFF], $80 to $FF by default)
//     holds the compiler's scratch for locals whose address is never
//     taken. A local whose address is taken goes in the frame, so no
//     construct in the source can make a pointer into the pool. Literal
//     casts such as `*(uint8_t*)0x84` are out of scope.
//   * ZP and Data / IndexedData never alias. The asm IR keeps the two
//     apart: `Data(symbol, off)` names a byte through a symbol, which
//     the linker resolves, and `ZP(addr, off)` gives the address. Only
//     the register allocator makes ZP operands, for slots in the pool,
//     and the runtime's symbols are named as Data, never as ZP.
//   * Frame and Frame alias when their offsets are equal, since
//     different offsets from FP are different bytes of one frame, and
//     so do Stack and Stack.
//   * Frame and Stack don't alias. FP and SSP point to different frames
//     (SSP to the top of the arguments being passed, FP to the bottom of
//     the frame), and even where their ranges meet, the offsets used
//     from each are different bytes.
//   * An indirect form and a `Data` name of the installed pointer proof
//     don't alias. The proof is empty unless a pass installs one from
//     `unreachable_by_pointer`.
//   * Indirect and Indirect alias when their offsets are equal (both
//     through DPTR).
//   * IndirectZp and IndirectZp alias when they have the same base and
//     the same offset. Through different bases they may alias, since two
//     pointer variables can hold one address (`p = q`, or both set to
//     one buffer), and nothing here knows the pointers' values.
//   * Indirect and IndirectY alias, since both reach bytes through DPTR
//     and Y could equal `off`.
//   * Indirect / IndirectY and Frame / Stack alias, since the program
//     may pass `&local`, which points DPTR into the frame.
//   * Anything else may alias.
#include "passes/asm_aliasing.h"

#include <algorithm>

#include "passes/asm_operands.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::asm_ast;
using ::c6502::asdl::as;
using ::c6502::asdl::isa;

// The names of the runtime's zero-page symbols, which match
// `sim.assembler.DEFAULT_ZP_SYMBOLS`.
bool is_runtime_zp_name(std::string_view name) {
  return name == "SSP" || name == "FP" || name == "HARGS" || name == "DPTR";
}

bool starts_with(std::string_view s, std::string_view prefix) {
  return s.substr(0, prefix.size()) == prefix;
}

bool is_imm(const ir::Type_operand& op) {
  return isa<ir::Imm, ir::ImmLabelLow, ir::ImmLabelHigh>(op);
}

bool is_indirect(const ir::Type_operand& op) {
  return isa<ir::Indirect, ir::IndirectY, ir::IndirectZp, ir::IndirectZpY>(op);
}

bool zp_data_alias(const ir::ZP& zp, const ir::Data& data, const AsmContext& ctx,
                   const SymbolAddrs* zp_slot_symbols) {
  if (const asdl::Int* user = ctx.user_zp_address(data.name)) {
    return zp.address + zp.offset == *user + data.offset;
  }
  if (!is_zp_symbol(data.name)) {
    return false;
  }
  const asdl::Int* base = ctx.layout.addr(data.name);
  if (base == nullptr && zp_slot_symbols != nullptr) {
    const auto it = zp_slot_symbols->find(data.name);
    if (it != zp_slot_symbols->end()) {
      base = &it->second;
    }
  }
  if (base != nullptr) {
    return zp.address + zp.offset == *base + data.offset;
  }
  return true;
}

}  // namespace

bool is_zp_symbol(std::string_view name) {
  return is_runtime_zp_name(name) || starts_with(name, "__zpabi_") ||
         starts_with(name, "__local_");
}

SymbolAddrs runtime_zp_addrs(const AsmContext& ctx) {
  return {{"SSP", ctx.layout.ssp},
          {"FP", ctx.layout.fp},
          {"HARGS", ctx.layout.hargs},
          {"DPTR", ctx.layout.dptr}};
}

bool may_alias(const ir::Type_operand& a, const ir::Type_operand& b,
               const AsmContext& ctx, const MayAliasOptions& options) {
  const auto* da = as<ir::Data>(a);
  const auto* db = as<ir::Data>(b);
  if (da != nullptr && db != nullptr) {
    return da->name == db->name && da->offset == db->offset;
  }
  // Imm / ImmLabelLow / ImmLabelHigh are values, which never alias a
  // memory operand. `LDA #<label` loads the low byte of the label's
  // address and reads none of the memory the label names.
  if (is_imm(a) || is_imm(b)) {
    return false;
  }
  const auto* za = as<ir::ZP>(a);
  const auto* zb = as<ir::ZP>(b);
  const auto* xa = as<ir::IndexedData>(a);
  const auto* xb = as<ir::IndexedData>(b);
  if (za != nullptr && zb != nullptr) {
    return za->address + za->offset == zb->address + zb->offset;
  }
  if (za != nullptr && db != nullptr) {
    return zp_data_alias(*za, *db, ctx, options.zp_slot_symbols);
  }
  if (zb != nullptr && da != nullptr) {
    return zp_data_alias(*zb, *da, ctx, options.zp_slot_symbols);
  }
  // ZP vs IndexedData are distinct namespaces too. An IndexedData
  // names a static, in a namespace of its own, or a raw address of
  // $0100 or above. An indexed access to a user zero-page array does
  // reach zero page.
  if (za != nullptr && xb != nullptr) {
    return ctx.is_user_zp_global(xb->name) || is_zp_symbol(xb->name);
  }
  if (zb != nullptr && xa != nullptr) {
    return ctx.is_user_zp_global(xa->name) || is_zp_symbol(xa->name);
  }
  // An indexed access to a zero-page slot's object, a local array of a
  // function with a private pool, which `memory_value_propagation`
  // spells `name,Y` when a pointer holds its address, may reach any of
  // its bytes.
  if (da != nullptr && xb != nullptr && xb->name == da->name) {
    return true;
  }
  if (db != nullptr && xa != nullptr && xa->name == db->name) {
    return true;
  }
  // A slot symbol (`__local_*`, `__zpabi_*`) against an IndexedData,
  // which names a static or a raw address of $0100 or above, can't
  // alias, by the reasoning of the ZP rule above.
  if (da != nullptr && xb != nullptr && is_zp_symbol(da->name)) {
    return false;
  }
  if (db != nullptr && xa != nullptr && is_zp_symbol(db->name)) {
    return false;
  }
  // A user zero-page object named by a `Data` operand, such as Lode
  // Runner's `GAME_COLNUM` at $85, against an indexed access to an
  // object outside zero page, follows the same reasoning as the
  // literal `ZP` operand above, since the name stands for its
  // zero-page address.
  for (const auto& [d, x] : {std::pair{da, xb}, std::pair{db, xa}}) {
    if (d != nullptr && x != nullptr && ctx.is_user_zp_global(d->name) &&
        d->name != x->name && !ctx.is_user_zp_global(x->name) &&
        !is_zp_symbol(x->name)) {
      return false;
    }
  }
  if (isa<ir::Frame, ir::Stack>(a) && zb != nullptr) {
    return false;
  }
  if (isa<ir::Frame, ir::Stack>(b) && za != nullptr) {
    return false;
  }
  if (is_indirect(a) && zb != nullptr &&
      !ctx.is_user_zp_byte(zb->address + zb->offset)) {
    const asdl::Int addr = zb->address + zb->offset;
    if (options.pool_lo <= addr && addr < options.pool_hi) {
      return false;
    }
  }
  if (is_indirect(b) && za != nullptr &&
      !ctx.is_user_zp_byte(za->address + za->offset)) {
    const asdl::Int addr = za->address + za->offset;
    if (options.pool_lo <= addr && addr < options.pool_hi) {
      return false;
    }
  }
  if (is_indirect(a) && db != nullptr && ctx.in_pointer_proof(db->name)) {
    return false;
  }
  if (is_indirect(b) && da != nullptr && ctx.in_pointer_proof(da->name)) {
    return false;
  }
  // The same holds for an indexed access to such an object, a `static`
  // array whose address the unit never takes, such as a table of row
  // addresses: no pointer reaches any of its bytes.
  if (is_indirect(a) && xb != nullptr && ctx.in_pointer_proof(xb->name)) {
    return false;
  }
  if (is_indirect(b) && xa != nullptr && ctx.in_pointer_proof(xa->name)) {
    return false;
  }
  // IndirectZp / IndirectZpY vs Data(runtime symbol). These forms go
  // through an explicit zero-page base, never through DPTR, so they
  // alias neither DPTR (the pointer source of `Indirect`) nor any other
  // runtime symbol, since user pointers don't point into the runtime's
  // bytes by c6502's convention. `Indirect` / `IndirectY` do go through
  // DPTR and have to alias `Data("DPTR", _)`, or dead-store elimination
  // would drop a live `STA DPTR` that a later `LDA (DPTR),Y` reads, so
  // this rule is only for the two IndirectZp forms.
  if (isa<ir::IndirectZp, ir::IndirectZpY>(a) && db != nullptr &&
      is_runtime_zp_name(db->name)) {
    return false;
  }
  if (isa<ir::IndirectZp, ir::IndirectZpY>(b) && da != nullptr &&
      is_runtime_zp_name(da->name)) {
    return false;
  }
  if (const auto* fa = as<ir::Frame>(a)) {
    if (const auto* fb = as<ir::Frame>(b)) {
      return fa->offset == fb->offset;
    }
  }
  if (const auto* sa = as<ir::Stack>(a)) {
    if (const auto* sb = as<ir::Stack>(b)) {
      return sa->offset == sb->offset;
    }
  }
  if (const auto* ia = as<ir::Indirect>(a)) {
    if (const auto* ib = as<ir::Indirect>(b)) {
      return ia->offset == ib->offset;
    }
  }
  if (const auto* pa = as<ir::IndirectZp>(a)) {
    if (const auto* pb = as<ir::IndirectZp>(b)) {
      if (pa->address != pb->address) {
        return true;
      }
      return pa->offset == pb->offset;
    }
  }
  if ((isa<ir::Frame>(a) && isa<ir::Stack>(b)) ||
      (isa<ir::Stack>(a) && isa<ir::Frame>(b))) {
    return false;
  }
  return true;
}

std::vector<ir::Type_operand> instruction_operands(const ir::Type_instruction& ins) {
  std::vector<ir::Type_operand> out;
  for_each_operand<true>(ins, [&](const ir::Type_operand& op) { out.push_back(op); });
  return out;
}

FunctionNames function_names(Instrs instrs) {
  FunctionNames out;
  for (const auto& ins : instrs) {
    if (const auto* call = as<ir::Call>(ins)) {
      out.called.insert(call->name);
    }
    if (const auto* la = as<ir::LoadAddress>(ins)) {
      std::visit(
          [&](const auto& src) {
            if constexpr (requires { src.name; }) {
              out.taken.insert(src.name);
            }
          },
          la->src);
    }
    for_each_operand<true>(ins, [&](const ir::Type_operand& op) {
      if (const auto* lo = as<ir::ImmLabelLow>(op)) {
        out.taken.insert(lo->name);
      } else if (const auto* hi = as<ir::ImmLabelHigh>(op)) {
        out.taken.insert(hi->name);
      } else if (const auto* d = as<ir::Data>(op)) {
        out.seen.insert(d->name);
      }
    });
  }
  return out;
}

NameSet unreachable_by_pointer(const ir::Program& prog) {
  NameSet defined;
  for (const auto& tl : prog.top_level) {
    if (const auto* fn = as<ir::Function>(tl)) {
      defined.insert(fn->name);
    }
  }
  NameSet named = defined;
  NameSet statics;
  NameSet taken;
  NameSet seen;
  for (const auto& tl : prog.top_level) {
    if (const auto* sv = as<ir::StaticVariable>(tl)) {
      if (!sv->is_global) {
        statics.insert(sv->name);
      }
      for (const auto& init : sv->init) {
        if (const auto* ai = as<ir::AddressInit>(init)) {
          taken.insert(ai->name);
        } else if (const auto* abi = as<ir::AddressByteInit>(init)) {
          taken.insert(abi->name);
        }
      }
    } else if (const auto* fn = as<ir::Function>(tl)) {
      FunctionNames names = function_names(fn->instructions);
      named.merge(names.called);
      taken.merge(names.taken);
      seen.merge(names.seen);
    }
  }
  const auto own_slot = [&](const std::string& name) {
    const std::string* owner = nullptr;
    int owners = 0;
    for (const auto& f : named) {
      for (const char* prefix : {"__local_", "__zpabi_"}) {
        if (starts_with(name, std::string(prefix) + f + "__")) {
          ++owners;
          owner = &f;
        }
      }
    }
    return owners == 1 && defined.count(*owner) != 0;
  };
  NameSet out;
  for (const auto& name : statics) {
    if (taken.count(name) == 0) {
      out.insert(name);
    }
  }
  for (const auto& name : seen) {
    if (taken.count(name) == 0 && own_slot(name)) {
      out.insert(name);
    }
  }
  return out;
}

bool through_slot_pointer(const ir::Type_instruction& ins,
                          const std::set<asdl::Int>& slot_addrs) {
  bool found = false;
  for_each_operand<true>(ins, [&](const ir::Type_operand& op) {
    asdl::Int address = 0;
    if (const auto* p = as<ir::IndirectZp>(op)) {
      address = p->address;
    } else if (const auto* q = as<ir::IndirectZpY>(op)) {
      address = q->address;
    } else {
      return;
    }
    if (slot_addrs.count(address) != 0 || slot_addrs.count(address + 1) != 0) {
      found = true;
    }
  });
  return found;
}

std::set<asdl::Int> pointer_byte_addresses(Instrs instrs) {
  std::set<asdl::Int> out;
  for (const auto& ins : instrs) {
    for_each_operand<true>(ins, [&](const ir::Type_operand& op) {
      if (const auto* p = as<ir::IndirectZp>(op)) {
        out.insert(p->address);
        out.insert(p->address + 1);
      } else if (const auto* q = as<ir::IndirectZpY>(op)) {
        out.insert(q->address);
        out.insert(q->address + 1);
      }
    });
  }
  return out;
}

bool is_pointer_byte(const ir::Type_operand& op,
                     const std::set<asdl::Int>& pointer_addrs,
                     const SymbolAddrs* zp_symbol_addrs) {
  if (pointer_addrs.empty()) {
    return false;
  }
  if (const auto* zp = as<ir::ZP>(op)) {
    return pointer_addrs.count(zp->address + zp->offset) != 0;
  }
  if (const auto* d = as<ir::Data>(op)) {
    if (zp_symbol_addrs != nullptr) {
      const auto it = zp_symbol_addrs->find(d->name);
      if (it != zp_symbol_addrs->end()) {
        return pointer_addrs.count(it->second + d->offset) != 0;
      }
    }
    return is_zp_symbol(d->name);
  }
  return false;
}

}  // namespace c6502::passes
