// The pass follows, within a basic block, for each byte `D` at a stable
// address, the byte `S` whose value was last stored into `D`. A pair
// `Mov(S, A); Mov(A, D)` is dropped when `D` is known to hold a copy of
// `S` and nothing since has changed either one. When A is read after the
// pair, only the store goes and the load stays.
//
// Aliasing
//
// The bytes followed are those at a stable address, which depends on no
// register or pointer.
//
//   * `ZP(addr, off)`, the zero-page byte at `addr + off`.
//   * `Data(name, off)` with `name` one of SSP, FP, HARGS, and DPTR, the
//     runtime's symbols, which resolve to known zero-page bytes
//     (`runtime_zp_addrs`).
//   * `Data(name, off)` with `name` a user static, whose link-time
//     address isn't known here. It is followed by `(name, off)` and
//     taken to overlap no other static and no zero-page byte.
//
// These aren't followed.
//
//   * `Frame(off)`, `Stack(off)`, `Indirect(off)`, and `IndirectY()`,
//     which go through a zero-page pointer (FP, SSP, or DPTR). The byte
//     they reach depends on the pointer's value at run time, which the
//     pass doesn't model.
//   * `IndexedData(name, off, X|Y)`, which writes the byte at
//     `name + off + index_reg`, anywhere in a range of 256 bytes.
//
// Invalidation
//
// For each instruction other than a matched pair, the pass finds the
// bytes it may write and drops every entry whose key or value may alias
// one of them. A write it can't classify, such as `STA (DPTR),Y`,
// clears the whole map.
//
// A write through `IndexedData` with a known base writes the range from
// the base to the base plus 255, and a followed byte with a known
// address is in it when `base <= addr <= base + 255`. A user static
// `(name, off)`, whose address isn't known, may be in any such range.
//
// The flags
//
// Dropping the pair drops the `LDA`'s setting of N and Z, so a later
// branch would read what the previous flag-setter left. The pass keeps
// the pair when a branch reads N/Z before another instruction sets
// them.
#include "passes/redundant_store.h"

#include <compare>
#include <map>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "passes/asm_aliasing.h"
#include "passes/asm_liveness.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::asm_ast;
using ::c6502::asdl::as;
using ::c6502::asdl::isa;

struct AddrId {
  enum Kind { kByte, kStatic };
  Kind kind = kByte;
  long long addr = 0;
  std::string name;
  long long offset = 0;

  auto operator<=>(const AddrId&) const = default;
};

struct WriteId {
  bool unknown = false;
  bool is_range = false;
  std::optional<long long> base;
  AddrId id;
};

bool is_reg_a(const ir::Type_operand& op) {
  const auto* r = as<ir::Reg>(op);
  return r != nullptr && isa<ir::A>(r->reg);
}

bool is_stable_mem(const ir::Type_operand& op) { return isa<ir::ZP, ir::Data>(op); }

class Pass {
 public:
  explicit Pass(const AsmContext& ctx) : runtime_(runtime_zp_addrs(ctx)) {}

  void rewrite_function(ir::Function& fn) const;

 private:
  std::optional<AddrId> addr_id(const ir::Type_operand& op) const {
    if (const auto* z = as<ir::ZP>(op)) {
      return AddrId{.kind = AddrId::kByte,
                    .addr = static_cast<long long>(z->address + z->offset)};
    }
    if (const auto* d = as<ir::Data>(op)) {
      const auto it = runtime_.find(d->name);
      if (it != runtime_.end()) {
        return AddrId{.kind = AddrId::kByte,
                      .addr = static_cast<long long>(it->second + d->offset)};
      }
      return AddrId{.kind = AddrId::kStatic,
                    .name = d->name,
                    .offset = static_cast<long long>(d->offset)};
    }
    return std::nullopt;
  }

  std::optional<long long> indexed_base(const ir::IndexedData& op) const {
    if (op.name.empty()) {
      return static_cast<long long>(op.offset);
    }
    const auto it = runtime_.find(op.name);
    if (it != runtime_.end()) {
      return static_cast<long long>(it->second + op.offset);
    }
    return std::nullopt;
  }

  WriteId write_of(const ir::Type_operand& dst) const {
    if (is_stable_mem(dst)) {
      return WriteId{.id = *addr_id(dst)};
    }
    if (const auto* idx = as<ir::IndexedData>(dst)) {
      return WriteId{.is_range = true, .base = indexed_base(*idx)};
    }
    return WriteId{.unknown = true};
  }

  std::vector<WriteId> memory_writes(const ir::Type_instruction& instr) const {
    if (const auto* mov = as<ir::Mov>(instr)) {
      if (isa<ir::Reg>(mov->dst)) {
        return {};
      }
      return {write_of(mov->dst)};
    }
    if (isa<ir::Add, ir::Sub, ir::And, ir::Or, ir::Xor, ir::ClearCarry,
            ir::SetCarry, ir::Compare>(instr)) {
      return {};
    }
    const ir::Type_operand* dst = nullptr;
    if (const auto* inc = as<ir::Inc>(instr)) {
      dst = &inc->dst;
    } else if (const auto* dec = as<ir::Dec>(instr)) {
      dst = &dec->dst;
    } else if (const auto* asl = as<ir::ArithmeticShiftLeft>(instr)) {
      dst = &asl->dst;
    } else if (const auto* lsr = as<ir::LogicalShiftRight>(instr)) {
      dst = &lsr->dst;
    } else if (const auto* rol = as<ir::RotateLeft>(instr)) {
      dst = &rol->dst;
    } else if (const auto* ror = as<ir::RotateRight>(instr)) {
      dst = &ror->dst;
    }
    if (dst != nullptr) {
      if (isa<ir::Reg>(*dst)) {
        return {};
      }
      if (is_stable_mem(*dst)) {
        return {WriteId{.id = *addr_id(*dst)}};
      }
      return {WriteId{.unknown = true}};
    }
    if (isa<ir::Push, ir::Pop>(instr)) {
      return {};
    }
    return {WriteId{.unknown = true}};
  }

  SymbolAddrs runtime_;
};

bool aliases(const AddrId& id, const WriteId& w) {
  if (w.is_range) {
    if (!w.base) {
      return true;
    }
    if (id.kind == AddrId::kByte) {
      return *w.base <= id.addr && id.addr < *w.base + 256;
    }
    return true;
  }
  if (w.id.kind == AddrId::kByte && id.kind == AddrId::kByte) {
    return w.id.addr == id.addr;
  }
  if (w.id.kind == AddrId::kStatic && id.kind == AddrId::kStatic) {
    return w.id.name == id.name && w.id.offset == id.offset;
  }
  return false;
}

std::optional<std::pair<const ir::Type_operand*, const ir::Type_operand*>>
match_lda_sta(const ir::Type_instruction& a, const ir::Type_instruction* b) {
  if (b == nullptr) {
    return std::nullopt;
  }
  const auto* load = as<ir::Mov>(a);
  const auto* store = as<ir::Mov>(*b);
  if (load == nullptr || store == nullptr) {
    return std::nullopt;
  }
  if (load->is_volatile || store->is_volatile) {
    return std::nullopt;
  }
  if (!(is_reg_a(load->dst) && is_reg_a(store->src))) {
    return std::nullopt;
  }
  if (!(is_stable_mem(load->src) && is_stable_mem(store->dst))) {
    return std::nullopt;
  }
  return std::make_pair(&load->src, &store->dst);
}

using Known = std::map<AddrId, AddrId>;

void drop_entries_using(Known& known, const AddrId& id) {
  for (auto it = known.begin(); it != known.end();) {
    if (it->second == id) {
      it = known.erase(it);
    } else {
      ++it;
    }
  }
}

void Pass::rewrite_function(ir::Function& fn) const {
  const std::vector<ir::Type_instruction>& instrs = fn.instructions;
  const LabelMap labels(instrs);
  Known known;
  std::vector<ir::Type_instruction> out;
  out.reserve(instrs.size());
  bool changed = false;
  std::size_t i = 0;
  while (i < instrs.size()) {
    const ir::Type_instruction& instr = instrs[i];
    const ir::Type_instruction* next = i + 1 < instrs.size() ? &instrs[i + 1] : nullptr;
    const auto pair = match_lda_sta(instr, next);
    if (pair && flags_dead_at(instrs, labels, i + 2)) {
      const auto src_id = addr_id(*pair->first);
      const auto dst_id = addr_id(*pair->second);
      if (src_id && dst_id) {
        const auto it = known.find(*dst_id);
        if (it != known.end() && it->second == *src_id) {
          if (!a_dead_at(instrs, labels, i + 2)) {
            out.push_back(instr);
          }
          changed = true;
          i += 2;
          continue;
        }
      }
      if (dst_id) {
        drop_entries_using(known, *dst_id);
        if (src_id) {
          known[*dst_id] = *src_id;
        }
      }
      out.push_back(instr);
      out.push_back(*next);
      i += 2;
      continue;
    }
    if (isa<ir::Label, ir::Jump, ir::Branch, ir::Ret, ir::Return, ir::Call,
            ir::FunctionPrologue, ir::AllocateStack, ir::LoadAddress, ir::Phi>(
            instr)) {
      known.clear();
    } else {
      for (const WriteId& w : memory_writes(instr)) {
        if (w.unknown) {
          known.clear();
          break;
        }
        for (auto it = known.begin(); it != known.end();) {
          if (aliases(it->first, w) || aliases(it->second, w)) {
            it = known.erase(it);
          } else {
            ++it;
          }
        }
      }
    }
    out.push_back(instr);
    ++i;
  }
  if (changed) {
    fn.instructions = std::move(out);
  }
}

}  // namespace

asm_ast::Program apply_redundant_store_elimination(asm_ast::Program prog,
                                                   const AsmContext& ctx) {
  const Pass pass(ctx);
  for (auto& tl : prog.top_level) {
    if (auto* fn = as<ir::Function>(tl)) {
      pass.rewrite_function(*fn);
    }
  }
  return prog;
}

}  // namespace c6502::passes
