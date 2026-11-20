// Redundant memory-to-memory store elimination.
//
// After loop unrolling, one loop-invariant copy from memory to memory
// can appear many times in a basic block. The usual case among c6502's
// lowerings is the staging of DPTR:
//
//     LDA  ptr_lo            ; (M0)
//     STA  DPTR              ; (M1)
//     LDA  ptr_hi            ; (M2)
//     STA  DPTR+1            ; (M3)
//     ...an indirect-Y access through (DPTR),Y...
//     LDA  ptr_lo            ; redundant, DPTR still holds ptr_lo
//     STA  DPTR              ; redundant
//     LDA  ptr_hi            ; redundant
//     STA  DPTR+1            ; redundant
//     ...the next access...
//
// When nothing writes `ptr_lo`, `ptr_hi`, `DPTR`, or `DPTR+1` between
// the two stagings, the second four instructions do nothing.
// `redundant_load` can't find this, because it follows what A holds and
// the `LDA (DPTR),Y` between them overwrites A. The fact needed is that
// `memory[DPTR]` equals `memory[ptr_lo]`, which stays true while A
// changes.
//
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
// A push (`PHA`) writes page 1 ($0100-$01FF), which overlaps neither
// zero page nor the data segment c6502 uses ($0800 and up), so it
// changes nothing followed here.
//
// The flags
//
// Dropping the pair drops the `LDA`'s setting of N and Z, so a later
// branch would read what the previous flag-setter left. The pass keeps
// the pair when a branch reads N/Z before another instruction sets
// them, as `redundant_load` does. The staging of DPTR is seldom near a
// branch: it is at the top of a hot loop's body, and the branches are
// at its bottom.
//
// Where it runs
//
// Inside `compile._peephole_fixedpoint`, after `redundant_load`. The two
// complement each other: `redundant_load` finds a load of a byte a
// register already holds, and this pass a copy of a byte into one that
// already holds it.
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

// The identity of a byte at a stable address: a byte whose address is
// known (`kByte`, `addr`), or a user static's byte (`kStatic`, `name`
// and `offset`), whose link-time address isn't known here.
struct AddrId {
  enum Kind { kByte, kStatic };
  Kind kind = kByte;
  long long addr = 0;
  std::string name;
  long long offset = 0;

  auto operator<=>(const AddrId&) const = default;
};

// What an instruction may write: a byte (`id`), the 256 bytes of an
// indexed write from `base` (`is_range`, with no `base` when it isn't
// known), or, when `unknown` is set, anything.
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

// Returns whether `op` is a memory operand whose address depends on no
// register or pointer: `ZP` and `Data`.
bool is_stable_mem(const ir::Type_operand& op) { return isa<ir::ZP, ir::Data>(op); }

class Pass {
 public:
  explicit Pass(const AsmContext& ctx) : runtime_(runtime_zp_addrs(ctx)) {}

  void rewrite_function(ir::Function& fn) const;

 private:
  // Returns the identity of a stable-address operand: a byte with a
  // known address for a `ZP` or a `Data` of a runtime symbol, and a
  // user static's byte for any other `Data`.
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

  // Returns the numeric base of an `IndexedData` operand when it is
  // known. An empty name holds a raw 16-bit base in `offset`, and a
  // runtime symbol resolves through the layout. A user static's
  // link-time address isn't known here.
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

  // Returns what a write of the memory operand `dst` may write.
  WriteId write_of(const ir::Type_operand& dst) const {
    if (is_stable_mem(dst)) {
      return WriteId{.id = *addr_id(dst)};
    }
    if (const auto* idx = as<ir::IndexedData>(dst)) {
      return WriteId{.is_range = true, .base = indexed_base(*idx)};
    }
    // Frame, Stack, Indirect, IndirectY: an indirect-Y write through a
    // zero-page pointer, whose address is known only at run time.
    return WriteId{.unknown = true};
  }

  // Returns the bytes `instr` may write, with an unknown write to clear
  // the whole map. An instruction that writes no memory gives none.
  std::vector<WriteId> memory_writes(const ir::Type_instruction& instr) const {
    if (const auto* mov = as<ir::Mov>(instr)) {
      if (isa<ir::Reg>(mov->dst)) {
        return {};
      }
      return {write_of(mov->dst)};
    }
    if (isa<ir::Add, ir::Sub, ir::And, ir::Or, ir::Xor, ir::ClearCarry,
            ir::SetCarry, ir::Compare>(instr)) {
      // The result goes to A or the flags.
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
      // INX, INY, DEX, DEY, and the shifts of A write no memory.
      if (isa<ir::Reg>(*dst)) {
        return {};
      }
      if (is_stable_mem(*dst)) {
        return {WriteId{.id = *addr_id(*dst)}};
      }
      return {WriteId{.unknown = true}};
    }
    if (isa<ir::Push, ir::Pop>(instr)) {
      // PHA writes the hardware stack ($0100-$01FF), which overlaps
      // neither zero page nor the data segment ($0800 and up by
      // c6502's convention), and PLA writes only A.
      return {};
    }
    // Any other kind of instruction may write anything.
    return {WriteId{.unknown = true}};
  }

  SymbolAddrs runtime_;
};

// Returns whether a write described by `w` may change the byte `id`.
// Anything that can't be proved apart may alias.
bool aliases(const AddrId& id, const WriteId& w) {
  if (w.is_range) {
    if (!w.base) {
      return true;
    }
    if (id.kind == AddrId::kByte) {
      return *w.base <= id.addr && id.addr < *w.base + 256;
    }
    // A user static's byte, whose link-time address may be anywhere.
    return true;
  }
  if (w.id.kind == AddrId::kByte && id.kind == AddrId::kByte) {
    return w.id.addr == id.addr;
  }
  if (w.id.kind == AddrId::kStatic && id.kind == AddrId::kStatic) {
    return w.id.name == id.name && w.id.offset == id.offset;
  }
  // One byte has a known address and the other is a user static. User
  // statics live in the data segment ($0800 and up by c6502's
  // convention), apart from zero page and from the runtime's symbols.
  return false;
}

// Returns the source and destination of `a; b` when it is
// `Mov(src, A); Mov(A, dst)` with both operands at stable addresses and
// neither move volatile. A volatile move can't go, since its read or
// write is observed.
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

// Drops the entries whose value is `id`, a byte about to be
// overwritten by a store, since what they copied is gone.
void drop_entries_using(Known& known, const AddrId& id) {
  for (auto it = known.begin(); it != known.end();) {
    if (it->second == id) {
      it = known.erase(it);
    } else {
      ++it;
    }
  }
}

// Walks the function's instructions, follows the copies from memory to
// memory within each basic block, and drops each `LDA`/`STA` pair that
// makes a copy already known.
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
          // The store does nothing. The load goes too unless A is read
          // after it: the pair can start a computation that uses the
          // loaded value, as in `LDA v3; STA tmp; SEC; SBC x` where
          // tmp already holds v3.
          if (!a_dead_at(instrs, labels, i + 2)) {
            out.push_back(instr);
          }
          changed = true;
          i += 2;
          continue;
        }
      }
      // The store makes a new copy. The entries whose value was the
      // destination's old content go first, since the store
      // overwrites it.
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
    // Any other instruction: a block boundary clears the map, and each
    // write drops the entries whose key or value it may change.
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
