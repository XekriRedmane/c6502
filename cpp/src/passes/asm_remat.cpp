// A narrow rematerialization peephole.
//
// A common shape after the asm SSA round trip and the byte-granular
// register allocator stages a value through one of the function's
// zero-page slots, because A is overwritten between the instruction
// that makes the value and the one that uses it:
//
//     LDA   <recomputable_src>
//     STA   __local_<fn>__<stage>          ; the stage slot's def
//     ... (A overwritten by other staging: LDA #c; STA other_arg) ...
//     LDA   __local_<fn>__<stage>          ; a use of the stage slot
//     STA   <consumer_dst>
//
// When `<recomputable_src>` costs little to compute again at the use
// (it has no inputs, or only registers and memory that nothing between
// changes), the round trip through the slot gains nothing. The use can
// load `<recomputable_src>` itself, and the `STA <local>` is then dead,
// with no reader left. `byte_dce`, `apply_redundant_store_elimination`,
// and `apply_asm_dead_store` drop it in the next sweep of the peephole
// fixed point, which then drops the first `LDA <recomputable_src>`,
// left with nothing to feed.
//
// What it costs against the staging, for a recompute of one
// instruction:
//
//   - `Imm(v)`, `ImmLabelLow`, `ImmLabelHigh`: a 2-byte `LDA #imm` in
//     place of a 2-byte load from zero page, the same.
//   - `Data(name, off)`: a load of 2 or 3 bytes (zero page or absolute)
//     in place of the 2-byte load from zero page, the same or a byte
//     more.
//   - `IndexedData(name, off, X|Y)`: a 3-byte `abs,X` / `abs,Y` load in
//     place of the 2-byte load from zero page, a byte more at the use.
//
// The `STA <stage>` that goes (2 bytes and 3 cycles), and the first
// `LDA <recomputable_src>` when nothing else reads A after it, pay for
// the extra byte only when the store does go. A read that costs a byte
// more is rewritten only when every read of the slot in the function
// is, and when there are at most `kMaxDearerReads` (2) of them
// (`costs_more`, `store_dies`). Rewriting such a read while another
// read kept the store cost 1 byte and 1 cycle each time, as in Lode
// Runner's `fill_hole` and `enable_next_level_ladders` until 19 Nov
// 2026.
//
// Eligibility
//
// A def `Mov(<src>, Data(__local_<fn>__*))` is a candidate when `<src>`
// can be recomputed and nothing between the def and a use makes the
// recompute give another value.
//
//   - `Imm`, `ImmLabelLow`, `ImmLabelHigh` always can, since nothing at
//     run time changes them.
//   - `Data(name, off)` can when `name` is the destination of no write
//     in the function, there's no `Call` between the def and the use
//     (a call may write through pointers this pass doesn't follow), and
//     no store between them goes through a pointer or to a raw address,
//     unless `name` is a static with internal linkage whose address the
//     program never forms (`collect_private_statics`). No pointer can
//     point into such a static. Without the last check,
//     `t = g; *p = 7; return t;` called with `p == &g` returned 7, the
//     value stored through `p`, where it had to return the value `g`
//     held when it was read.
//   - `IndexedData(name, off, reg)` can under the same rules for
//     `name`, when the index register (X or Y) isn't written between
//     the def and the use either. A call overwrites X and Y under the
//     zp_abi convention, so the rule against a call between covers the
//     write of the register a call makes.
//
// A source of another shape (`Frame`, `Stack`, `Indirect*`) isn't
// handled. Indirect addressing depends on a zero-page pointer pair the
// code may change, and deriving an address relative to FP again would
// race with the pushes in between.
//
// The walk
//
// Within a basic block, the walk stops at any boundary of the block and
// at any write of the stage slot, which ends the def. It matches the
// def `Mov(<recomp>, Data(__local_<fn>__*))` and looks forward for the
// reads `Mov(Data(<same_local>), <any_dst>)`. Each eligible read takes
// `<recomp>` as its source directly. The dead `STA` and the first `LDA`,
// left with nothing to feed, go in a later sweep of the fixed point, to
// the dead-store and dead-code peepholes.
#include "passes/asm_remat.h"

#include <set>
#include <string>
#include <utility>
#include <vector>

#include "passes/asm_aliasing.h"
#include "passes/asm_liveness.h"
#include "passes/asm_operands.h"
#include "passes/decimal_arith.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::asm_ast;
using ::c6502::asdl::as;
using ::c6502::asdl::isa;

// A recompute that costs a byte more than the read it replaces pays for
// itself only through the store into the slot, which goes once nothing
// reads the slot. That store is 2 bytes, so at most this many reads may
// take the dearer form.
constexpr std::size_t kMaxDearerReads = 2;

using InstrVec = std::vector<ir::Type_instruction>;
using AddrSet = std::set<long long>;

bool is_reg_a(const ir::Type_operand& op) {
  const auto* r = as<ir::Reg>(op);
  return r != nullptr && isa<ir::A>(r->reg);
}

// Returns the operand `instr` writes in place, for the read-modify-write
// instructions (`Inc`, `Dec`, and the shifts and rotates), or null.
const ir::Type_operand* rmw_dst(const ir::Type_instruction& instr) {
  using K = ir::Kind_instruction;
  switch (ir::kind(instr)) {
    case K::Inc:
      return &std::get<ir::Inc>(instr).dst;
    case K::Dec:
      return &std::get<ir::Dec>(instr).dst;
    case K::ArithmeticShiftLeft:
      return &std::get<ir::ArithmeticShiftLeft>(instr).dst;
    case K::LogicalShiftRight:
      return &std::get<ir::LogicalShiftRight>(instr).dst;
    case K::RotateLeft:
      return &std::get<ir::RotateLeft>(instr).dst;
    case K::RotateRight:
      return &std::get<ir::RotateRight>(instr).dst;
    default:
      return nullptr;
  }
}

// Returns every operand `instr` reads. A read-modify-write instruction
// (`Inc`, `Dec`, a shift) reads and writes one operand, which counts as
// read here so that the scan for referenced slots stays conservative.
// An ALU instruction's destination counts as read too. A `Mov` reads
// its source and only writes its destination.
std::vector<ir::Type_operand> read_operands(const ir::Type_instruction& instr) {
  std::vector<ir::Type_operand> out;
  switch (ir::kind(instr)) {
    case ir::Kind_instruction::DecimalArith:
      return decimal_arith::reads(std::get<ir::DecimalArith>(instr));
    case ir::Kind_instruction::Mov:
      out.push_back(std::get<ir::Mov>(instr).src);
      break;
    case ir::Kind_instruction::Add: {
      const auto& i = std::get<ir::Add>(instr);
      out = {i.src, i.dst};
      break;
    }
    case ir::Kind_instruction::Sub: {
      const auto& i = std::get<ir::Sub>(instr);
      out = {i.src, i.dst};
      break;
    }
    case ir::Kind_instruction::And: {
      const auto& i = std::get<ir::And>(instr);
      out = {i.src, i.dst};
      break;
    }
    case ir::Kind_instruction::Or: {
      const auto& i = std::get<ir::Or>(instr);
      out = {i.src, i.dst};
      break;
    }
    case ir::Kind_instruction::Xor: {
      const auto& i = std::get<ir::Xor>(instr);
      out = {i.src1, i.src2};
      break;
    }
    case ir::Kind_instruction::Push:
      out.push_back(std::get<ir::Push>(instr).src);
      break;
    case ir::Kind_instruction::Compare: {
      const auto& i = std::get<ir::Compare>(instr);
      out = {i.left, i.right};
      break;
    }
    case ir::Kind_instruction::BitTest:
      out.push_back(std::get<ir::BitTest>(instr).src);
      break;
    case ir::Kind_instruction::LoadAddress:
      out.push_back(std::get<ir::LoadAddress>(instr).src);
      break;
    default:
      if (const ir::Type_operand* dst = rmw_dst(instr)) {
        out.push_back(*dst);
      }
      break;
  }
  return out;
}

// Returns every operand `instr` writes, for the set of names written in
// the function. The reads aren't given.
std::vector<ir::Type_operand> dsts_of(const ir::Type_instruction& instr) {
  switch (ir::kind(instr)) {
    case ir::Kind_instruction::DecimalArith:
      return decimal_arith::writes(std::get<ir::DecimalArith>(instr));
    case ir::Kind_instruction::Mov:
      return {std::get<ir::Mov>(instr).dst};
    case ir::Kind_instruction::Add:
      return {std::get<ir::Add>(instr).dst};
    case ir::Kind_instruction::Sub:
      return {std::get<ir::Sub>(instr).dst};
    case ir::Kind_instruction::And:
      return {std::get<ir::And>(instr).dst};
    case ir::Kind_instruction::Or:
      return {std::get<ir::Or>(instr).dst};
    case ir::Kind_instruction::Xor:
      return {std::get<ir::Xor>(instr).dst};
    case ir::Kind_instruction::Pop:
      return {std::get<ir::Pop>(instr).dst};
    case ir::Kind_instruction::LoadAddress:
      return {std::get<ir::LoadAddress>(instr).dst};
    default:
      if (const ir::Type_operand* dst = rmw_dst(instr)) {
        return {*dst};
      }
      return {};
  }
}

// Returns whether `instr` ends the linear walk. Control flow into or out
// of the block means the pass can't tell which instructions ran between
// the def and the use.
bool is_block_boundary(const ir::Type_instruction& instr) {
  return isa<ir::Label, ir::Jump, ir::Branch, ir::Call, ir::Ret, ir::Return,
             ir::FunctionPrologue, ir::AllocateStack, ir::LoadAddress,
             ir::DecimalArith>(instr);
}

// Returns whether `src` is of a shape that may be recomputed. Whether a
// particular one may (its memory unchanged, its register stable) is
// `can_remat`'s, where the range from the def to the use is known.
bool is_recomputable_shape(const ir::Type_operand& src) {
  return isa<ir::Imm, ir::ImmLabelLow, ir::ImmLabelHigh, ir::Data, ir::IndexedData>(src);
}

bool matches_local(const ir::Type_operand& op, const std::string& name) {
  const auto* d = as<ir::Data>(op);
  return d != nullptr && d->name == name && d->offset == 0;
}

// Returns whether `instr` writes the slot `name`.
bool writes_local(const ir::Type_instruction& instr, const std::string& name) {
  for (const auto& dst : dsts_of(instr)) {
    if (matches_local(dst, name)) {
      return true;
    }
  }
  return false;
}

// Returns whether `instr` writes A by anything other than a `Mov`, which
// the caller handles: an ALU operation, shift, or `Pop` into A, or a
// `DecimalArith`.
bool writes_a(const ir::Type_instruction& instr) {
  using K = ir::Kind_instruction;
  switch (ir::kind(instr)) {
    case K::DecimalArith:
      return true;
    case K::Add:
      return is_reg_a(std::get<ir::Add>(instr).dst);
    case K::Sub:
      return is_reg_a(std::get<ir::Sub>(instr).dst);
    case K::And:
      return is_reg_a(std::get<ir::And>(instr).dst);
    case K::Or:
      return is_reg_a(std::get<ir::Or>(instr).dst);
    case K::Xor:
      return is_reg_a(std::get<ir::Xor>(instr).dst);
    case K::Pop:
      return is_reg_a(std::get<ir::Pop>(instr).dst);
    default: {
      const ir::Type_operand* dst = rmw_dst(instr);
      // An `INC A` or `DEC A` isn't a write of A that this counts.
      return dst != nullptr && !isa<ir::Inc, ir::Dec>(instr) && is_reg_a(*dst);
    }
  }
}

// Returns whether `instr` writes the register `reg`: a load into it
// (`LDX`), an `INX` / `DEX`, or a pull into it. An operand lowered with
// an `LDY #off` setup writes Y. Calls write X and Y too, but the walk
// stops at a call before this is asked.
bool writes_reg(const ir::Type_instruction& instr, const ir::Type_reg& reg) {
  if (isa<ir::Y>(reg) && sets_up_y(instr)) {
    return true;
  }
  const ir::Type_operand* dst = nullptr;
  if (const auto* mov = as<ir::Mov>(instr)) {
    dst = &mov->dst;
  } else if (const auto* inc = as<ir::Inc>(instr)) {
    dst = &inc->dst;
  } else if (const auto* dec = as<ir::Dec>(instr)) {
    dst = &dec->dst;
  } else if (const auto* pop = as<ir::Pop>(instr)) {
    dst = &pop->dst;
  }
  if (dst == nullptr) {
    return false;
  }
  const auto* r = as<ir::Reg>(*dst);
  return r != nullptr && r->reg.index() == reg.index();
}

// Returns the most recent instruction before `i` whose destination is
// `Reg(A)`, or -1 when a block boundary comes first or A's value comes
// from an instruction that isn't a clean producer.
//
// A `Mov(_, Reg(A))`, a load into A, is the one shape of producer. Any
// other write of A (`Add`, `Sub`, `And`, `Or`, `Xor`, a shift of A,
// `Pop(Reg(A))`) stops the search, since A's value at `i` isn't then the
// result of one load that can be repeated.
long find_a_producer(const InstrVec& instrs, std::size_t i) {
  for (long k = static_cast<long>(i) - 1; k >= 0; --k) {
    const ir::Type_instruction& instr = instrs[static_cast<std::size_t>(k)];
    if (is_block_boundary(instr)) {
      return -1;
    }
    if (const auto* mov = as<ir::Mov>(instr)) {
      if (is_reg_a(mov->dst)) {
        return k;
      }
      const auto* src_reg = as<ir::Reg>(mov->src);
      if (src_reg != nullptr || isa<ir::Reg>(mov->dst)) {
        // `TXA` / `TYA` in front of a soft-stack or indirect store
        // writes A.
        if (src_reg != nullptr && !isa<ir::A>(src_reg->reg) &&
            !isa<ir::Data, ir::ZP>(mov->dst)) {
          return -1;
        }
        continue;
      }
      if (mov->src == mov->dst) {
        continue;  // a move of a byte to itself emits nothing
      }
      // A store of a constant or a memory byte, `Mov(#k, M)` or
      // `Mov(M, N)`, emits as `LDA src; STA dst`, so A ends up holding
      // `src`. Taking it for an earlier load made `Mov(#$80, l19); STA t`
      // rematerialize `t` as the `#$FF` loaded before it (fuzz.stmts
      // seed 25347).
      return k;
    }
    if (writes_a(instr)) {
      return -1;
    }
  }
  return -1;
}

// Returns the recomputable source of a staging def at `i`, and the index
// from which `can_remat` checks the range, or nothing when `instrs[i]`
// isn't a staging def. There are two shapes.
//
//   1. `Mov(<recomputable>, Data(__local_<fn>__*))`, one memory-to-memory
//      atom. The recompute is its source, and the range starts at `i`.
//   2. `Mov(Reg(A), Data(__local_<fn>__*))`, the second of two atoms,
//      whose producer is the `Mov(<recomputable>, Reg(A))` before it
//      (`find_a_producer`). The two-atom form appears once the asm SSA
//      round trip has split a memory-to-memory move into the
//      `LDA src; STA dst` it emits as. The recompute is the producer's
//      source, and the range starts at the producer, so that it covers
//      any write of X or Y or call that would change the recompute.
std::optional<std::pair<ir::Type_operand, std::size_t>> classify_stage_def(
    const InstrVec& instrs, std::size_t i) {
  const auto* defn = as<ir::Mov>(instrs[i]);
  if (defn == nullptr || defn->is_volatile) {
    return std::nullopt;
  }
  const auto* dst = as<ir::Data>(defn->dst);
  if (dst == nullptr || !dst->name.starts_with("__local_") || dst->offset != 0) {
    return std::nullopt;
  }
  if (is_recomputable_shape(defn->src)) {
    return std::make_pair(defn->src, i);
  }
  if (is_reg_a(defn->src)) {
    const long prod_idx = find_a_producer(instrs, i);
    if (prod_idx < 0) {
      return std::nullopt;
    }
    const auto* producer = as<ir::Mov>(instrs[static_cast<std::size_t>(prod_idx)]);
    if (producer == nullptr || producer->is_volatile ||
        !is_recomputable_shape(producer->src)) {
      return std::nullopt;
    }
    return std::make_pair(producer->src, static_cast<std::size_t>(prod_idx));
  }
  return std::nullopt;
}

// Returns the name of the `Data` or `IndexedData` operand `op`.
const std::string& operand_name(const ir::Type_operand& op) {
  if (const auto* d = as<ir::Data>(op)) {
    return d->name;
  }
  return std::get<ir::IndexedData>(op).name;
}

class Pass {
 public:
  Pass(const ir::Program& prog, const AsmContext& ctx, const SymbolAddrs* zp_slot_symbols)
      : ctx_(ctx),
        slots_(zp_slot_symbols != nullptr ? *zp_slot_symbols : SymbolAddrs{}),
        runtime_(runtime_zp_addrs(ctx)),
        private_(collect_private_statics(prog)) {}

  void rewrite_function(ir::Function& fn) const;

 private:
  static NameSet collect_private_statics(const ir::Program& prog);

  void rewrite_uses(InstrVec& out, const NameSet& writable) const;
  std::vector<std::size_t> eligible_uses(const InstrVec& out, std::size_t i,
                                         const ir::Type_operand& recomp,
                                         std::size_t range_start,
                                         const std::string& local_name,
                                         const NameSet& writable) const;
  bool can_remat(const ir::Type_operand& recomp, const InstrVec& instrs,
                 std::size_t def_idx, std::size_t use_idx,
                 const NameSet& writable) const;
  bool store_may_reach(const ir::Type_operand& dst,
                       const ir::Type_operand& recomp) const;
  bool is_zero_page_name(const std::string& name) const;
  bool costs_more(const ir::Type_operand& recomp, const std::string& local_name) const;
  bool store_dies(const InstrVec& instrs, const std::vector<std::size_t>& uses,
                  const std::string& local_name) const;
  AddrSet collect_referenced_addrs(const InstrVec& instrs,
                                   const std::vector<bool>* skip) const;
  InstrVec drop_dead_stage_dsts(InstrVec instrs) const;

  const AsmContext& ctx_;
  SymbolAddrs slots_;
  SymbolAddrs runtime_;
  NameSet private_;
};

// Returns the names of the statics with internal linkage defined in
// `prog` whose address nothing in `prog` forms. An address is formed by
// an `ImmLabelLow` / `ImmLabelHigh` operand, by a `LoadAddress` from the
// static, or by an `AddressInit` in some static's initializer. No
// pointer can point into such a static, so a store through a pointer
// leaves it unchanged.
NameSet Pass::collect_private_statics(const ir::Program& prog) {
  NameSet statics;
  NameSet taken;
  for (const auto& tl : prog.top_level) {
    if (const auto* sv = as<ir::StaticVariable>(tl)) {
      if (!sv->is_global) {
        statics.insert(sv->name);
      }
      for (const auto& init : sv->init) {
        if (const auto* a = as<ir::AddressInit>(init)) {
          taken.insert(a->name);
        } else if (const auto* b = as<ir::AddressByteInit>(init)) {
          taken.insert(b->name);
        }
      }
    } else if (const auto* fn = as<ir::Function>(tl)) {
      for (const auto& instr : fn->instructions) {
        if (const auto* la = as<ir::LoadAddress>(instr)) {
          if (const auto* d = as<ir::Data>(la->src)) {
            taken.insert(d->name);
          }
        }
        // Every operand-valued field, read or written, including those
        // in a list. A `Phi`'s arguments aren't operands themselves.
        const auto visit = [&](const ir::Type_operand& op) {
          if (const auto* lo = as<ir::ImmLabelLow>(op)) {
            taken.insert(lo->name);
          } else if (const auto* hi = as<ir::ImmLabelHigh>(op)) {
            taken.insert(hi->name);
          }
        };
        if (const auto* phi = as<ir::Phi>(instr)) {
          visit(phi->dst);
        } else {
          for_each_operand<true>(instr, visit);
        }
      }
    }
  }
  NameSet out;
  for (const auto& name : statics) {
    if (taken.find(name) == taken.end()) {
      out.insert(name);
    }
  }
  return out;
}

// Returns whether `name` is a byte in zero page, which an `LDA` reads in
// 2 bytes: a slot the unit placed there, one of the runtime's bytes, or
// a zero-page object.
bool Pass::is_zero_page_name(const std::string& name) const {
  auto it = slots_.find(name);
  if (it != slots_.end()) {
    return it->second < 0x100;
  }
  it = runtime_.find(name);
  if (it != runtime_.end()) {
    return it->second < 0x100;
  }
  return ctx_.is_user_zp_global(name);
}

// Returns whether reading `recomp` takes a byte more than reading the
// slot `local_name`. An absolute `Data` byte and an `abs,X` / `abs,Y`
// element do (3 bytes and a cycle more) when the slot is in zero page
// (2 bytes). An immediate or a zero-page byte doesn't.
bool Pass::costs_more(const ir::Type_operand& recomp, const std::string& local_name) const {
  if (!isa<ir::Data, ir::IndexedData>(recomp)) {
    return false;
  }
  if (is_zero_page_name(operand_name(recomp))) {
    return false;
  }
  return is_zero_page_name(local_name);
}

// Adds to `referenced` the zero-page bytes a read of `op` names.
void add_referenced_addrs(const ir::Type_operand& op, const SymbolAddrs& slots,
                          AddrSet& referenced) {
  if (isa<ir::Data, ir::IndexedData>(op)) {
    const auto it = slots.find(operand_name(op));
    if (it != slots.end()) {
      const asdl::Int offset = isa<ir::Data>(op) ? std::get<ir::Data>(op).offset
                                                 : std::get<ir::IndexedData>(op).offset;
      referenced.insert(static_cast<long long>(it->second + offset));
    }
  } else if (const auto* z = as<ir::ZP>(op)) {
    referenced.insert(static_cast<long long>(z->address + z->offset));
  } else if (const auto* p = as<ir::IndirectZp>(op)) {
    // The pointer pair at addr and addr+1. IndirectZp's offset is the Y
    // that indexes from it, which isn't part of the pair's address.
    referenced.insert(static_cast<long long>(p->address));
    referenced.insert(static_cast<long long>(p->address + 1));
  } else if (const auto* q = as<ir::IndirectZpY>(op)) {
    referenced.insert(static_cast<long long>(q->address));
    referenced.insert(static_cast<long long>(q->address + 1));
  }
}

// Returns the zero-page addresses read anywhere in `instrs`, by any
// operand shape, resolved to the bytes' addresses, leaving out the
// instructions `skip` marks. A stage byte can be collapsed only when its
// address isn't here, since one read of it through a pointer (or of the
// other byte of the pair, as an indirect access reads both bytes of its
// pointer) keeps it live.
//
//   * `Data(name, off)` and `IndexedData(name, off, _)` read the byte at
//     `zp_slot_symbols[name] + off` when `name` is a slot symbol, and
//     nothing in the pool otherwise.
//   * `ZP(addr, off)` reads the byte at `addr + off`.
//   * `IndirectZp(addr, off)` and `IndirectZpY(addr)` read the pointer
//     pair at `addr` and `addr + 1`. `off` is the `LDY #off` that
//     indexes from it, which `asm_dead_store`'s `ptr_source_reads` reads
//     the same way.
//   * The other shapes (`Reg`, `Imm`, `ImmLabel*`, `Frame`, `Stack`,
//     `Indirect`, `IndirectY`) name nothing in the pool.
AddrSet Pass::collect_referenced_addrs(const InstrVec& instrs,
                                       const std::vector<bool>* skip) const {
  AddrSet out;
  for (std::size_t k = 0; k < instrs.size(); ++k) {
    if (skip != nullptr && (*skip)[k]) {
      continue;
    }
    const ir::Type_instruction& instr = instrs[k];
    for (const auto& src : read_operands(instr)) {
      add_referenced_addrs(src, slots_, out);
    }
    // An indirect-Y destination, as in `STA (ptr),Y`, reads the
    // pointer's bytes to find the address, though the operand is the
    // one written, so a slot that holds the pointer isn't taken for
    // dead. This is what `asm_dead_store`'s `ptr_source_reads` gives.
    for (const auto& dst : dsts_of(instr)) {
      if (const auto* p = as<ir::IndirectZp>(dst)) {
        out.insert(static_cast<long long>(p->address));
        out.insert(static_cast<long long>(p->address + 1));
      } else if (const auto* q = as<ir::IndirectZpY>(dst)) {
        out.insert(static_cast<long long>(q->address));
        out.insert(static_cast<long long>(q->address + 1));
      } else if (isa<ir::Indirect, ir::IndirectY>(dst)) {
        // `(DPTR),Y` reads DPTR and DPTR+1, at their addresses in the
        // runtime's layout.
        const auto it = runtime_.find("DPTR");
        if (it != runtime_.end()) {
          out.insert(static_cast<long long>(it->second));
          out.insert(static_cast<long long>(it->second + 1));
        }
      }
    }
  }
  return out;
}

// Returns the names whose address some instruction of `instrs` builds,
// as an `ImmLabelLow` / `ImmLabelHigh` operand or a `LoadAddress`
// source, leaving out the instructions `skip` marks.
NameSet collect_escaped_names(const InstrVec& instrs, const std::vector<bool>* skip) {
  NameSet out;
  for (std::size_t k = 0; k < instrs.size(); ++k) {
    if (skip != nullptr && (*skip)[k]) {
      continue;
    }
    const ir::Type_instruction& instr = instrs[k];
    if (const auto* la = as<ir::LoadAddress>(instr)) {
      if (const auto* d = as<ir::Data>(la->src)) {
        out.insert(d->name);
      }
    }
    for (const auto& op : read_operands(instr)) {
      if (const auto* lo = as<ir::ImmLabelLow>(op)) {
        out.insert(lo->name);
      } else if (const auto* hi = as<ir::ImmLabelHigh>(op)) {
        out.insert(hi->name);
      }
    }
  }
  return out;
}

// Returns whether rewriting the reads at `uses` leaves the slot
// `local_name` with no read anywhere in the function, by name, as a
// pointer's byte, or through its address, so that the store into it
// goes, and whether there are few enough of them to pay for it.
bool Pass::store_dies(const InstrVec& instrs, const std::vector<std::size_t>& uses,
                      const std::string& local_name) const {
  if (uses.size() > kMaxDearerReads) {
    return false;
  }
  const auto it = slots_.find(local_name);
  if (it == slots_.end()) {
    return false;
  }
  std::vector<bool> skip(instrs.size(), false);
  for (const std::size_t u : uses) {
    skip[u] = true;
  }
  if (collect_referenced_addrs(instrs, &skip).count(static_cast<long long>(it->second)) != 0) {
    return false;
  }
  const NameSet escaped = collect_escaped_names(instrs, &skip);
  return escaped.find(local_name) == escaped.end();
}

// Returns whether a write to `dst` may change a byte that the `Data` or
// `IndexedData` operand `recomp` reads.
//
// A store through a pointer (`(DPTR),Y` or `(zp),Y`) and an indexed
// store to a raw address (`IndexedData` with an empty name) can reach
// any byte whose address the program can form. They can't reach a
// static among the private ones, since no instruction or initializer in
// the program builds that static's address, and the static has internal
// linkage, so no other translation unit builds it either. A store to a
// named static indexed by X or Y stays inside that static's bytes in a
// valid program, and the set of names written already counts it. Other
// memory destinations are compared with `may_alias`, which matches a
// numeric `ZP` byte against a zero-page global of the same address.
bool Pass::store_may_reach(const ir::Type_operand& dst,
                           const ir::Type_operand& recomp) const {
  const auto* idx = as<ir::IndexedData>(dst);
  if (isa<ir::Indirect, ir::IndirectY, ir::IndirectZp, ir::IndirectZpY>(dst) ||
      (idx != nullptr && idx->name.empty())) {
    const std::string& name = operand_name(recomp);
    return name.empty() || private_.find(name) == private_.end();
  }
  if (isa<ir::Data, ir::IndexedData>(dst)) {
    return false;
  }
  if (isa<ir::Reg, ir::Frame, ir::Stack>(dst)) {
    return false;
  }
  return may_alias(dst, recomp, ctx_);
}

// Returns whether recomputing `recomp` at `use_idx`, for the def whose
// range starts at `def_idx`, gives the byte the def gave.
//
// An immediate always does. A `Data` or `IndexedData` operand does when
// its name is written nowhere in the function, no `Call` comes between
// (a call may write the named storage through routines this pass
// doesn't model), and no store between may reach the named storage
// through a pointer or a raw address (`store_may_reach`). An
// `IndexedData` needs its index register (X or Y) unwritten between as
// well.
bool Pass::can_remat(const ir::Type_operand& recomp, const InstrVec& instrs,
                     std::size_t def_idx, std::size_t use_idx,
                     const NameSet& writable) const {
  if (isa<ir::Imm, ir::ImmLabelLow, ir::ImmLabelHigh>(recomp)) {
    return true;
  }
  if (!isa<ir::Data, ir::IndexedData>(recomp)) {
    return false;
  }
  if (writable.find(operand_name(recomp)) != writable.end()) {
    return false;
  }
  for (std::size_t k = def_idx + 1; k < use_idx; ++k) {
    for (const auto& dst : dsts_of(instrs[k])) {
      if (store_may_reach(dst, recomp)) {
        return false;
      }
    }
  }
  for (std::size_t k = def_idx + 1; k < use_idx; ++k) {
    if (isa<ir::Call>(instrs[k])) {
      return false;
    }
  }
  const auto* indexed = as<ir::IndexedData>(recomp);
  if (indexed == nullptr) {
    return true;
  }
  if (!isa<ir::X, ir::Y>(indexed->index)) {
    return false;
  }
  for (std::size_t k = def_idx + 1; k < use_idx; ++k) {
    if (writes_reg(instrs[k], indexed->index)) {
      return false;
    }
  }
  return true;
}

// Returns the indexes of the reads of `local_name` after the staging def
// at `i`, in its block, that can read `recomp` instead. Several reads
// may each take the same recomputed source.
std::vector<std::size_t> Pass::eligible_uses(const InstrVec& out, std::size_t i,
                                             const ir::Type_operand& recomp,
                                             std::size_t range_start,
                                             const std::string& local_name,
                                             const NameSet& writable) const {
  std::vector<std::size_t> uses;
  const auto* indexed = as<ir::IndexedData>(recomp);
  for (std::size_t j = i + 1; j < out.size(); ++j) {
    const ir::Type_instruction& cur = out[j];
    if (is_block_boundary(cur) || writes_local(cur, local_name)) {
      break;
    }
    const auto* mov = as<ir::Mov>(cur);
    if (mov == nullptr || mov->is_volatile || !matches_local(mov->src, local_name)) {
      continue;
    }
    // There's no `LDX abs,X` or `LDY abs,Y`, so an indexed read can't
    // load its own index register.
    if (indexed != nullptr) {
      const auto* r = as<ir::Reg>(mov->dst);
      if (r != nullptr && r->reg.index() == indexed->index.index()) {
        continue;
      }
    }
    if (!can_remat(recomp, out, range_start, j, writable)) {
      break;
    }
    uses.push_back(j);
  }
  return uses;
}

void Pass::rewrite_uses(InstrVec& out, const NameSet& writable) const {
  // A rewrite changes only reads after the def, so the walk sees each
  // def as the earlier rewrites left it.
  for (std::size_t i = 0; i < out.size(); ++i) {
    const auto stage = classify_stage_def(out, i);
    if (!stage) {
      continue;
    }
    const auto& [recomp, range_start] = *stage;
    const std::string local_name = std::get<ir::Data>(std::get<ir::Mov>(out[i]).dst).name;
    const std::vector<std::size_t> uses =
        eligible_uses(out, i, recomp, range_start, local_name, writable);
    if (uses.empty()) {
      continue;
    }
    if (costs_more(recomp, local_name) && !store_dies(out, uses, local_name)) {
      continue;
    }
    for (const std::size_t j : uses) {
      auto& cur = std::get<ir::Mov>(out[j]);
      out[j] = ir::Mov{.src = recomp, .dst = cur.dst, .is_volatile = cur.is_volatile};
    }
  }
}

// Turns every `Mov(<src>, Data(__local_<fn>__<...>))` whose stage byte
// nothing in the function reads any more into `Mov(<src>, Reg(A))`. The
// memory destination is what kept the atom past `apply_asm_dead_store`,
// which matches only stores from a register. The atom is then a pure
// `LDA <src>`, and when A is dead after it, as it usually is after a
// rematerialization since the next instruction is usually another
// `LDA #c` of the staging, `apply_dead_a_arith_elimination` or
// `apply_redundant_load_elimination` drops it in a later sweep.
//
// A read through a pointer counts: `IndirectZp(addr, off)` and
// `IndirectZpY(addr)` read the two bytes of a zero-page pair. A
// `__local_*` slot that is one byte of a pointer has to stay even when
// no source operand names it, so the names are resolved through
// `zp_slot_symbols` and compared by address. A slot whose address the
// function builds (`LDA #<slot` / `LDA #>slot`, or a `LoadAddress`) can
// be read through that pointer, which names no byte of it. A struct
// local read with `c.v` is the usual case, and every byte of such a
// slot stays.
InstrVec Pass::drop_dead_stage_dsts(InstrVec instrs) const {
  const AddrSet referenced = collect_referenced_addrs(instrs, nullptr);
  const NameSet escaped = collect_escaped_names(instrs, nullptr);
  InstrVec out;
  out.reserve(instrs.size());
  for (auto& instr : instrs) {
    if (const auto* mov = as<ir::Mov>(instr); mov != nullptr && !mov->is_volatile) {
      const auto* d = as<ir::Data>(mov->dst);
      if (d != nullptr && d->name.starts_with("__local_") && d->offset == 0) {
        const auto it = slots_.find(d->name);
        if (it != slots_.end() &&
            referenced.count(static_cast<long long>(it->second)) == 0 &&
            escaped.find(d->name) == escaped.end()) {
          if (isa<ir::Reg>(mov->src)) {
            // From A, the def was the second half of an `LDA <src>;
            // STA local` pair whose first half is still there, and A
            // already holds the value, so the store just goes. From X
            // or Y (`STX local` / `STY local`), the dead store changes
            // no flag and no A, so it goes too: a `TXA` / `TYA` in its
            // place would overwrite A and set N/Z, which is wrong when
            // A holds a live value, such as the low byte of a 2-byte
            // register return staged in A while its high byte is
            // stored from X to a slot.
            continue;
          }
          out.push_back(ir::Mov{.src = mov->src, .dst = ir::Reg{.reg = ir::A{}},
                                .is_volatile = false});
          continue;
        }
      }
    }
    out.push_back(std::move(instr));
  }
  return out;
}

void Pass::rewrite_function(ir::Function& fn) const {
  NameSet writable;
  for (const auto& instr : fn.instructions) {
    for (const auto& dst : dsts_of(instr)) {
      if (isa<ir::Data, ir::IndexedData>(dst)) {
        writable.insert(operand_name(dst));
      }
    }
  }
  InstrVec out = fn.instructions;
  rewrite_uses(out, writable);
  fn.instructions = drop_dead_stage_dsts(std::move(out));
}

}  // namespace

asm_ast::Program apply_remat(asm_ast::Program prog, const AsmContext& ctx,
                             const SymbolAddrs* zp_slot_symbols) {
  const Pass pass(prog, ctx, zp_slot_symbols);
  for (auto& tl : prog.top_level) {
    if (auto* fn = as<ir::Function>(tl)) {
      pass.rewrite_function(*fn);
    }
  }
  return prog;
}

}  // namespace c6502::passes
