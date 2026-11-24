// Redundant load elimination.
//
// A tracker of the 6502's registers. It follows which operands each of
// A, X, and Y currently holds a copy of. When the next instruction is
// `LDA M` (or `LDX M` / `LDY M`) and the register already mirrors `M`,
// the load does nothing and goes.
//
// The gain is largest after loop unrolling. A fill unrolled 105 times
// emits 105 copies of `LDA value; LDX col; STA $XXXX,X`, though `value`
// and `col` don't change across them. With this pass that is one
// `LDA value; LDX col` and 105 `STA $XXXX,X`. Without unrolling, the
// pass still finds the loads that the fan-out of bytes and the moves of
// SSA destruction leave behind.
//
// Aliasing
//
// Dropping `LDA M` needs `M` unchanged since the load that put it in A.
// The hard part of the tracking is which writes between the loads may
// change which followed operands. That comes from
// `asm_aliasing.may_alias`, with the names no pointer reaches
// (`unreachable_by_pointer`) installed as its pointer proof for the
// whole pass. In short:
//
//   * `Imm` never aliases memory, being a constant in the instruction.
//   * `ZP(addr, off)` is the byte at `(addr + off) & 0xFF`. It aliases
//     another ZP byte at the same address and never a `Data` or an
//     `IndexedData`, which `replace_pseudoregisters` gives addresses of
//     $0100 and up (it emits ZP for the pseudos it puts in zero page
//     and Data for the statics of the data segment).
//   * `Data(name, off)` is a link-time address. It aliases only
//     `Data(name, off)` with the same name and offset, and no ZP byte.
//   * `IndexedData(name, off)` is `name+off,X` (or `,Y`) at link time.
//     It aliases any operand in the window from `name+off` to
//     `name+off+0xFF` that can't be proved apart, and no ZP byte.
//
// That is enough for the unrolled case (a ZP byte followed through
// writes to `IndexedData` survives them) and conservative elsewhere.
//
// The flags, and what Z reflects
//
// Dropping `LDA M` skips setting Z to "(M == 0)". A later
// `Branch(EQ|NE)` then reads what the instruction before left in Z,
// which may not be what `LDA M` would have set. Either of two
// conditions makes the drop safe.
//
//   1. The flags are dead: no reachable instruction reads N/Z before
//      another overwrites them (`flags_dead_at`, a forward walk over
//      the CFG).
//   2. The flags are already right: an earlier instruction set Z to
//      "(M == 0)" and nothing since has changed it. `z_reflects` holds
//      the operands whose zeroness Z reflects.
//
// `z_reflects` holds several operands, since several bytes can be zero
// together: after `LDA M; STA N; STA P`, Z reflects M, N, and P, as
// the lists of A, X, and Y do. The updates follow every instruction
// that sets the flags (LDA, ADC, SBC, AND, ORA, EOR, INC, DEC, the
// shifts, CMP, BIT, and the memory-to-memory moves).
//
// Availability over the CFG
//
// The tracker runs as a forward analysis of what must be available,
// over the function's CFG. At the start of each block, the state is the
// intersection of every predecessor's state at its end. An equivalence
// (A holds M, say) reaches a successor's start when every path from
// ENTRY to it leaves the equivalence intact.
//
// The shape this finds that a tracker within one block misses is the
// join of a diamond:
//
//     LDX __zpabi_..._slot       ; the entry sets X to slot
//     ...
//     BCC .merge                  ; both predecessors carry X = slot,
//     ...                         ; and neither path changes X or
//     BCS .merge                  ; slot
// .merge:
//     LDX __zpabi_..._slot       ; redundant on both paths in
//
// A tracker within one block forgets X at `.merge`, which has several
// predecessors. The intersection at the join keeps it whenever every
// path in agreed on the value.
//
// The lattice, for each register and for `z_reflects`, is a list of
// operands, empty when nothing is known. A block's transfer adds
// equivalences (a load, a store that makes a new mirror) and removes
// them (a write that may alias, by the rules above). A join at a label
// with several predecessors intersects the lists, by `operands_equal`,
// and anything missing on any path goes.
//
// `Call`, `FunctionPrologue`, `AllocateStack`, and `LoadAddress` clear
// every register in the transfer. They expand into several instructions
// in `asm_to_asm2`, and following their insides would mean modeling the
// expansions here. The one exception is a call to a routine declared
// `preserves_x` / `preserves_y`, after which X (Y) keeps an immediate it
// held, so `LDX #k; JSR f; LDX #k` loses its second load. A memory
// equivalence still goes, since the callee may write the memory.
//
// What Python computed once
//
// The Python pass kept, within one call, the atom of each operand (one
// object for every operand equal to it under `operands_equal`), the
// facts of each instruction with each state it had been applied to and
// the state it gave, and `may_alias` of each pair of atoms, since the
// dataflow visits a block again whenever its in-state changes and the
// first phase starts the analysis over each round. Here an atom is an
// index into a table of the operands met in the call, which makes
// equality an integer comparison, and the results of `may_alias` are
// kept for each pair of atoms. An operand of a kind that `operands_equal`
// doesn't compare (`IndirectZp`, `IndirectZpY`, `IndirectY`, the label
// immediates, a register) equals no operand, itself included, so its
// atom is marked unkeyed, and every test of membership or equality
// treats it as matching nothing, as Python's identity tests did. The
// update of a state by an instruction reads nothing but the two, so the
// states don't need Python's cache to give the same results.
//
// Where it runs
//
// After `replace_pseudoregisters`, so the operands are concrete, and
// after `apply_direct_index_load`, so the pass sees `LDX zp` and not
// `LDA zp; TAX`. Before `expand_long_branches`. The pass only deletes
// and adds no branches.
#include "passes/redundant_load.h"

#include <algorithm>
#include <deque>
#include <functional>
#include <optional>
#include <set>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "passes/asm_aliasing.h"
#include "passes/asm_dead_store.h"
#include "passes/asm_liveness.h"

namespace c6502::passes {

namespace ir = ::c6502::asm_ast;
using ::c6502::asdl::as;
using ::c6502::asdl::isa;

Cfg build_cfg_tolerant(const ir::Function& fn) {
  std::set<std::string> labels;
  for (const auto& instr : fn.instructions) {
    if (const auto* label = as<ir::Label>(instr)) {
      labels.insert(label->name);
    }
  }
  std::vector<std::string> unresolved;
  std::set<std::string> seen;
  for (const auto& instr : fn.instructions) {
    const std::string* target = nullptr;
    if (const auto* branch = as<ir::Branch>(instr)) {
      target = &branch->target;
    } else if (const auto* jump = as<ir::Jump>(instr)) {
      target = &jump->target;
    }
    if (target != nullptr && labels.count(*target) == 0 &&
        seen.insert(*target).second) {
      unresolved.push_back(*target);
    }
  }
  std::vector<ir::Type_instruction> extra;
  for (const auto& target : unresolved) {
    extra.push_back(ir::Label{.name = target});
    extra.push_back(ir::Return{.save_a = false});
  }
  return build_cfg(fn.instructions, std::move(extra));
}

namespace {

using K = ir::Kind_instruction;
using KO = ir::Kind_operand;

// How many times `rewrite_function` runs its first phase again after a
// rewrite, at most.
constexpr int kReorderRounds = 64;

// The register class of `reg`: 0 for A, 1 for X, 2 for Y.
int reg_class(const ir::Type_reg& reg) { return static_cast<int>(reg.index()); }
constexpr int kA = 0;
constexpr int kX = 1;
constexpr int kY = 2;

bool is_reg_a(const ir::Type_operand& op) {
  const auto* r = as<ir::Reg>(op);
  return r != nullptr && isa<ir::A>(r->reg);
}

// Returns whether `operands_equal` compares operands of `op`'s kind,
// which it does by the kind and every field: `Imm`, `ZP`, `Data`,
// `IndexedData` (`tab,X` and `tab,Y` name different bytes unless X ==
// Y, which the tracker doesn't know, and taking them for equal dropped
// the `LDA tab,Y` of `j = tab[i]; ...; g = tab[j];`), `Frame`, `Stack`,
// and `Indirect`. An operand of another kind equals no operand.
bool is_keyed(const ir::Type_operand& op) {
  return isa<ir::Imm, ir::ZP, ir::Data, ir::IndexedData, ir::Frame, ir::Stack,
             ir::Indirect>(op);
}

// Returns whether `operands_equal` holds between `a` and `b`.
bool operands_equal(const ir::Type_operand& a, const ir::Type_operand& b) {
  return is_keyed(a) && a == b;
}

std::size_t hash_int(const asdl::Int& v) {
  return std::hash<long long>{}(static_cast<long long>(v));
}

struct OperandHash {
  std::size_t operator()(const ir::Type_operand& op) const {
    std::size_t h = op.index() * 0x9E3779B97F4A7C15ULL;
    const auto mix = [&](std::size_t v) { h ^= v + 0x9E3779B97F4A7C15ULL + (h << 6) + (h >> 2); };
    switch (ir::kind(op)) {
      case KO::Imm:
        mix(hash_int(std::get<ir::Imm>(op).value));
        break;
      case KO::ZP: {
        const auto& z = std::get<ir::ZP>(op);
        mix(hash_int(z.address));
        mix(hash_int(z.offset));
        break;
      }
      case KO::Data: {
        const auto& d = std::get<ir::Data>(op);
        mix(std::hash<std::string>{}(d.name));
        mix(hash_int(d.offset));
        break;
      }
      case KO::IndexedData: {
        const auto& x = std::get<ir::IndexedData>(op);
        mix(std::hash<std::string>{}(x.name));
        mix(hash_int(x.offset));
        mix(x.index.index());
        break;
      }
      case KO::Frame:
        mix(hash_int(std::get<ir::Frame>(op).offset));
        break;
      case KO::Stack:
        mix(hash_int(std::get<ir::Stack>(op).offset));
        break;
      case KO::Indirect:
        mix(hash_int(std::get<ir::Indirect>(op).offset));
        break;
      case KO::IndirectZp: {
        const auto& p = std::get<ir::IndirectZp>(op);
        mix(hash_int(p.address));
        mix(hash_int(p.offset));
        break;
      }
      case KO::IndirectZpY:
        mix(hash_int(std::get<ir::IndirectZpY>(op).address));
        break;
      case KO::ImmLabelLow: {
        const auto& l = std::get<ir::ImmLabelLow>(op);
        mix(std::hash<std::string>{}(l.name));
        mix(hash_int(l.offset));
        break;
      }
      case KO::ImmLabelHigh: {
        const auto& l = std::get<ir::ImmLabelHigh>(op);
        mix(std::hash<std::string>{}(l.name));
        mix(hash_int(l.offset));
        break;
      }
      case KO::Reg:
        mix(std::get<ir::Reg>(op).reg.index());
        break;
      case KO::Pseudo: {
        const auto& p = std::get<ir::Pseudo>(op);
        mix(std::hash<std::string>{}(p.name));
        mix(hash_int(p.offset));
        break;
      }
      case KO::IndirectY:
        break;
    }
    return h;
  }
};

// One operand as the state tracks it, with the facts about it that the
// updates read.
//
// `dep` is the class of the register whose value the operand's address
// depends on: R's for `IndexedData(_, _, index=R)`, since the address is
// `name + offset + R`, and Y for `IndirectY` / `IndirectZpY`, which
// index with whatever Y holds. It is -1 for the rest.
struct Atom {
  ir::Type_operand op;
  bool keyed = false;
  int dep = -1;
  // `ZP`, `Data`, `Stack`, `Frame`, or `Indirect`: a byte whose address
  // depends on no index register, which may join a register's list or
  // `z_reflects` after a write.
  bool stable = false;
  // A stable byte or an `IndexedData` element, which may be in a memory
  // group.
  bool groupable = false;
  bool immediate = false;
  bool data_or_zp = false;
};

using List = std::vector<int>;

bool contains(const List& l, int a) { return std::find(l.begin(), l.end(), a) != l.end(); }

// The state of the registers at a point. Each list holds the atoms of
// the operands the register is known to mirror, empty when it is
// unknown. A register can mirror several operands at once: after
// `LDA M` A holds M, and after `STA N` A holds N as well, since the
// store wrote A's value to N and left A alone. Either is grounds for
// dropping a later `LDA M` or `LDA N`. The lists keep source order,
// which decides which memory group a load takes (`load_list`) and the
// order of a join's lists.
//
// `z` holds the operands whose current values Z reflects the zeroness
// of, whatever A, X, and Y hold. It mostly follows A (after `LDA M`
// both list M), but `INC P` keeps A and sets Z from P's new value, so
// then `z` lists P alone.
struct RegState {
  List a;
  List x;
  List y;
  List z;
  // Whether the last instruction that set the flags set N/Z from A's
  // current value (a load or computation into A, TAX / TAY). An empty
  // `z` alone doesn't say that: the flags may come from an `LDY #off`
  // setup, a compare, or an `LDX`.
  bool z_tracks_a = false;
  // Groups of two or more memory operands known to hold one value that
  // no register holds any more. A register's list becomes such a group
  // when the register is loaded with something else (`set_reg`), and a
  // load of one of the group's operands makes the register mirror them
  // all again (`load_list`). After `LDA t,X; STA p; LDA u,X; STA p+1;
  // LDA (p),Y; ...; LDA t,X; STA p`, the last store writes the value
  // `p` holds, and goes.
  std::vector<List> mem;

  void reset() { *this = RegState{}; }
};

// The facts of an instruction that its update reads.
struct Facts {
  // `z_from_a`: 1 when the instruction leaves N/Z set from A's value,
  // 0 when it sets them from something else, and -1 when it leaves them
  // alone.
  int z_effect = 0;
  // Whether an operand is lowered with an `LDY #off` setup, and whether
  // the destination of a `Mov` or `Pop` is.
  bool setup_y = false;
  bool dst_setup_y = false;
  // The atoms of a `Mov`'s memory operands, and of the memory operand a
  // shift, `INC`, or `DEC` writes, or -1.
  int src = -1;
  int dst = -1;
};

bool is_y_setup(const ir::Type_operand& op) {
  return isa<ir::Frame, ir::Stack, ir::Indirect, ir::IndirectZp>(op);
}

// Returns every operand of `instr`, read or written.
std::vector<const ir::Type_operand*> operands_of(const ir::Type_instruction& instr) {
  switch (ir::kind(instr)) {
    case K::Mov: {
      const auto& i = std::get<ir::Mov>(instr);
      return {&i.src, &i.dst};
    }
    case K::LoadAddress: {
      const auto& i = std::get<ir::LoadAddress>(instr);
      return {&i.src, &i.dst};
    }
    case K::Add: {
      const auto& i = std::get<ir::Add>(instr);
      return {&i.src, &i.dst};
    }
    case K::Sub: {
      const auto& i = std::get<ir::Sub>(instr);
      return {&i.src, &i.dst};
    }
    case K::And: {
      const auto& i = std::get<ir::And>(instr);
      return {&i.src, &i.dst};
    }
    case K::Or: {
      const auto& i = std::get<ir::Or>(instr);
      return {&i.src, &i.dst};
    }
    case K::Xor: {
      const auto& i = std::get<ir::Xor>(instr);
      return {&i.src1, &i.src2, &i.dst};
    }
    case K::Compare: {
      const auto& i = std::get<ir::Compare>(instr);
      return {&i.left, &i.right};
    }
    case K::Inc:
      return {&std::get<ir::Inc>(instr).dst};
    case K::Dec:
      return {&std::get<ir::Dec>(instr).dst};
    case K::ArithmeticShiftLeft:
      return {&std::get<ir::ArithmeticShiftLeft>(instr).dst};
    case K::LogicalShiftRight:
      return {&std::get<ir::LogicalShiftRight>(instr).dst};
    case K::RotateLeft:
      return {&std::get<ir::RotateLeft>(instr).dst};
    case K::RotateRight:
      return {&std::get<ir::RotateRight>(instr).dst};
    case K::Pop:
      return {&std::get<ir::Pop>(instr).dst};
    case K::Push:
      return {&std::get<ir::Push>(instr).src};
    case K::BitTest:
      return {&std::get<ir::BitTest>(instr).src};
    default:
      return {};
  }
}

// Returns the destination of a shift, `Inc`, or `Dec`, or null.
const ir::Type_operand* step_or_shift_dst(const ir::Type_instruction& instr) {
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

// Returns the destination of an ALU instruction (`Add`, `Sub`, `And`,
// `Or`, `Xor`), or null.
const ir::Type_operand* alu_dst(const ir::Type_instruction& instr) {
  switch (ir::kind(instr)) {
    case K::Add:
      return &std::get<ir::Add>(instr).dst;
    case K::Sub:
      return &std::get<ir::Sub>(instr).dst;
    case K::And:
      return &std::get<ir::And>(instr).dst;
    case K::Or:
      return &std::get<ir::Or>(instr).dst;
    case K::Xor:
      return &std::get<ir::Xor>(instr).dst;
    default:
      return nullptr;
  }
}

// Returns whether `instr` leaves N/Z set from A's value (1), sets them
// from something else (0), or leaves them alone (-1). The `LDY` of an
// addressing-mode setup is the caller's.
int z_from_a(const ir::Type_instruction& instr) {
  switch (ir::kind(instr)) {
    case K::Mov: {
      const auto& m = std::get<ir::Mov>(instr);
      if (is_reg_a(m.dst)) {
        return 1;  // LDA / TXA / TYA
      }
      if (isa<ir::Reg>(m.dst)) {
        return is_reg_a(m.src) ? 1 : 0;  // TAX / TAY set Z from A; LDX doesn't
      }
      if (!isa<ir::Reg>(m.src)) {
        return 1;  // memory to memory: LDA src; STA dst
      }
      return -1;  // STA / STX / STY
    }
    case K::Add:
    case K::Sub:
    case K::And:
    case K::Or:
    case K::Xor:
      return is_reg_a(*alu_dst(instr)) ? 1 : 0;
    case K::ArithmeticShiftLeft:
    case K::LogicalShiftRight:
    case K::RotateLeft:
    case K::RotateRight:
      return is_reg_a(*step_or_shift_dst(instr)) ? 1 : 0;
    case K::Pop:
      return is_reg_a(std::get<ir::Pop>(instr).dst) ? 1 : 0;
    case K::Push:
    case K::SetCarry:
    case K::ClearCarry:
    case K::Branch:
    case K::Label:
      return -1;
    default:
      return 0;
  }
}

// Returns whether `instr` is one the transfer of a block passes over: a
// leading `Label` (a marker) or a terminator (`Jump`, `Branch`, `Ret`,
// `Return`), which change no register in the model, since the CFG
// carries the state into the successors.
bool is_passive(const ir::Type_instruction& instr) {
  return isa<ir::Label, ir::Jump, ir::Branch, ir::Ret, ir::Return>(instr);
}

// Returns whether `op` is a byte of an object the program names: a
// `Data` operand that isn't one of the compiler's zero-page slots
// (`__local_*`, `__zpabi_*`) or the runtime's bytes.
bool named_object(const ir::Type_operand& op) {
  const auto* d = as<ir::Data>(op);
  return d != nullptr && !d->name.starts_with("__local_") &&
         !d->name.starts_with("__zpabi_") && d->name != "SSP" && d->name != "FP" &&
         d->name != "HARGS" && d->name != "DPTR";
}

// Returns the bytes that hold the address `op` is reached through: a
// zero-page pointer's two bytes, DPTR's, FP's, or SSP's.
std::vector<ir::Type_operand> pointer_bytes(const ir::Type_operand& op) {
  asdl::Int base = 0;
  if (const auto* p = as<ir::IndirectZp>(op)) {
    base = p->address;
  } else if (const auto* q = as<ir::IndirectZpY>(op)) {
    base = q->address;
  } else {
    std::string name;
    if (isa<ir::Indirect, ir::IndirectY>(op)) {
      name = "DPTR";
    } else if (isa<ir::Frame>(op)) {
      name = "FP";
    } else if (isa<ir::Stack>(op)) {
      name = "SSP";
    } else {
      return {};
    }
    return {ir::Data{.name = name, .offset = 0}, ir::Data{.name = name, .offset = 1}};
  }
  return {ir::ZP{.address = base, .offset = 0}, ir::ZP{.address = base, .offset = 1}};
}

// Returns whether `ins` is a non-volatile `LDA` of memory or a constant.
bool is_a_load(const ir::Type_instruction& ins) {
  const auto* m = as<ir::Mov>(ins);
  return m != nullptr && !m->is_volatile && is_reg_a(m->dst) && !isa<ir::Reg>(m->src);
}

// Counts the non-volatile stores of A into memory that come in a row
// from `instrs[k]`.
std::size_t stores_of_a_after(Instrs instrs, std::size_t k) {
  std::size_t n = 0;
  while (k + n < instrs.size()) {
    const auto* m = as<ir::Mov>(instrs[k + n]);
    if (!(m != nullptr && !m->is_volatile && is_reg_a(m->src) && !isa<ir::Reg>(m->dst))) {
      break;
    }
    ++n;
  }
  return n;
}

}  // namespace

bool same_index_apart(const ir::Type_operand& a, const ir::Type_operand& b, Instrs window) {
  const auto* x = as<ir::IndexedData>(a);
  const auto* y = as<ir::IndexedData>(b);
  if (x == nullptr || y == nullptr || x->name != y->name || !(x->index == y->index) ||
      x->offset == y->offset) {
    return false;
  }
  asdl::Int diff = x->offset - y->offset;
  if (diff < 0) {
    diff = -diff;
  }
  if (!(diff < 256)) {
    return false;
  }
  if (isa<ir::Y>(x->index)) {
    return std::none_of(window.begin(), window.end(),
                        [](const ir::Type_instruction& ins) { return sets_up_y(ins); });
  }
  return isa<ir::X>(x->index);
}

namespace {

// Returns whether `instrs[i]` moves a register back into the register it
// was just moved from (`TAX; TXA`, `TYA; TAY`, ...).
bool reverses_transfer(Instrs instrs, std::size_t i) {
  if (i == 0) {
    return false;
  }
  const auto* prev = as<ir::Mov>(instrs[i - 1]);
  const auto* ins = as<ir::Mov>(instrs[i]);
  return prev != nullptr && ins != nullptr && isa<ir::Reg>(prev->src) &&
         isa<ir::Reg>(prev->dst) && !(prev->src == prev->dst) && ins->src == prev->dst &&
         ins->dst == prev->src;
}

// Returns whether `instrs[i]` is a transfer between registers that the
// instruction before a run of conditional branches ending at `i` already
// made, as in `TXA; BEQ two; TXA; BPL three`. A branch changes no
// register and no flag, and with no label in the run the second transfer
// is reached only from the first, so both registers and N and Z already
// hold what it would put there. A volatile transfer, which reads or
// writes a `register volatile` local, stays.
bool repeats_transfer(Instrs instrs, std::size_t i) {
  const auto* ins = as<ir::Mov>(instrs[i]);
  if (!(ins != nullptr && !ins->is_volatile && isa<ir::Reg>(ins->src) &&
        isa<ir::Reg>(ins->dst) && !(ins->src == ins->dst))) {
    return false;
  }
  long k = static_cast<long>(i) - 1;
  while (k >= 0 && isa<ir::Branch>(instrs[static_cast<std::size_t>(k)])) {
    --k;
  }
  return k < static_cast<long>(i) - 1 && k >= 0 &&
         instrs[static_cast<std::size_t>(k)] == instrs[i];
}

class Pass {
 public:
  Pass(const AsmContext& ctx, const SymbolAddrs* zp_symbol_addrs)
      : ctx_(ctx), zp_addrs_(zp_symbol_addrs != nullptr ? *zp_symbol_addrs : SymbolAddrs{}) {}

  void rewrite_function(ir::Function& fn);

 private:
  using States = std::vector<std::optional<RegState>>;

  // Atoms.
  int atom(const ir::Type_operand& op);
  const Atom& at(int id) const { return atoms_[static_cast<std::size_t>(id)]; }
  bool keyed(int id) const { return at(id).keyed; }
  bool has(const List& atoms, const ir::Type_operand& op);
  bool intersects(const List& l1, const List& l2) const;
  List intersect(const List& l1, const List& l2) const;
  bool list_equal(const List& l1, const List& l2) const;
  bool state_equal(const RegState& s1, const RegState& s2) const;
  List without_dependent(const List& atoms, int rc) const;
  std::vector<List> drop_dependent_groups(const std::vector<List>& mem, int rc) const;
  bool alias(int a, int d);
  List without_aliasing(const List& atoms, int d);

  // The update.
  Facts facts(const ir::Type_instruction& instr);
  void update_state(const ir::Type_instruction& instr, RegState& state);
  void update_for_mov(const ir::Mov& mov, RegState& state, int s, int d);
  List& reg_list(RegState& state, int rc) const;
  const List& reg_list(const RegState& state, int rc) const {
    return rc == kA ? state.a : rc == kX ? state.x : state.y;
  }
  void set_reg(RegState& state, int rc, List value) const;
  List load_list(RegState& state, int src) const;
  void invalidate_aliasing(RegState& state, int d);
  void invalidate_z_aliasing(RegState& state, int d);

  // The dataflow.
  RegState join_states(const std::vector<const RegState*>& states) const;
  std::vector<List> join_memory(const RegState& result,
                                const std::vector<const RegState*>& states) const;
  States solve(const Cfg& cfg);

  // The rewrites.
  bool flags_redundant_at(Instrs instrs, const LabelMap& labels, std::size_t i,
                          const RegState& state);
  bool is_redundant_load(const ir::Type_instruction& instr, const RegState& state);
  std::optional<std::pair<std::vector<ir::Type_instruction>, std::size_t>>
  store_const_via_index(Instrs instrs, const LabelMap& labels, std::size_t i,
                        const RegState& state);
  bool reloads_a(Instrs instrs, std::size_t start, const List& held,
                 const ir::Type_operand& stored);
  std::optional<std::pair<std::size_t, std::size_t>> swap_pairs_ok(
      Instrs instrs, const LabelMap& labels, std::size_t i, const RegState& state);
  bool swap_loads_ok(Instrs instrs, const LabelMap& labels, std::size_t i,
                     const RegState& state);
  std::optional<ir::Mov> store_from_a(const ir::Type_instruction& instr, const RegState& state);
  std::optional<ir::Mov> redirect_lda_to_transfer(const ir::Type_instruction& instr,
                                                  const RegState& state);
  std::optional<ir::Mov> redirect_index_load_to_transfer(const ir::Type_instruction& instr,
                                                         const RegState& state,
                                                         const ir::Type_instruction* prev);
  bool stores_held_value(const ir::Type_instruction& instr, const RegState& state);
  std::optional<std::vector<ir::Type_instruction>> reorder_with_in_states(
      const ir::Function& fn, const Cfg& cfg, const States& in_state);
  std::vector<ir::Type_instruction> rewrite_with_in_states(const ir::Function& fn,
                                                           const Cfg& cfg,
                                                           const States& in_state);

  bool may_alias_zp(const ir::Type_operand& a, const ir::Type_operand& b) const {
    return may_alias(a, b, ctx_, MayAliasOptions{.zp_slot_symbols = &zp_addrs_});
  }

  const AsmContext& ctx_;
  // The zero-page address of each slot symbol and zero-page object, so
  // that `swap_pairs_ok` can tell whether a store writes a pointer's
  // byte. Empty when the caller has no map, and then a store to any
  // slot may write the pointer of an `(zp),Y` access.
  SymbolAddrs zp_addrs_;
  std::vector<Atom> atoms_;
  std::unordered_map<ir::Type_operand, int, OperandHash> keyed_index_;
  std::unordered_map<ir::Type_operand, int, OperandHash> unkeyed_index_;
  std::unordered_map<unsigned long long, bool> alias_;
};

int Pass::atom(const ir::Type_operand& op) {
  const bool k = is_keyed(op);
  auto& index = k ? keyed_index_ : unkeyed_index_;
  const auto it = index.find(op);
  if (it != index.end()) {
    return it->second;
  }
  Atom a{.op = op, .keyed = k};
  switch (ir::kind(op)) {
    case KO::IndexedData:
      a.dep = reg_class(std::get<ir::IndexedData>(op).index);
      break;
    case KO::IndirectY:
    case KO::IndirectZpY:
      a.dep = kY;
      break;
    default:
      break;
  }
  a.stable = isa<ir::ZP, ir::Data, ir::Stack, ir::Frame, ir::Indirect>(op);
  a.groupable = a.stable || isa<ir::IndexedData>(op);
  a.immediate = isa<ir::Imm, ir::ImmLabelLow, ir::ImmLabelHigh>(op);
  a.data_or_zp = isa<ir::Data, ir::ZP>(op);
  const int id = static_cast<int>(atoms_.size());
  atoms_.push_back(std::move(a));
  index.emplace(op, id);
  return id;
}

// Returns whether some operand of `atoms` equals `op`.
bool Pass::has(const List& atoms, const ir::Type_operand& op) {
  if (!is_keyed(op)) {
    return false;
  }
  return contains(atoms, atom(op));
}

// Returns whether some operand of `l1` equals some operand of `l2`.
bool Pass::intersects(const List& l1, const List& l2) const {
  for (const int a : l1) {
    if (keyed(a) && contains(l2, a)) {
      return true;
    }
  }
  return false;
}

// Returns the operands of `l1` that equal one of `l2`, in `l1`'s order,
// so the dataflow is deterministic. An operand that equals nothing goes.
List Pass::intersect(const List& l1, const List& l2) const {
  List out;
  if (l1.empty() || l2.empty()) {
    return out;
  }
  for (const int a : l1) {
    if (keyed(a) && contains(l2, a)) {
      out.push_back(a);
    }
  }
  return out;
}

// Returns whether the lists hold the same operands, in any order, as
// multisets. An operand that equals nothing makes the lists unequal,
// even to themselves.
bool Pass::list_equal(const List& l1, const List& l2) const {
  if (l1.size() != l2.size()) {
    return false;
  }
  if (l1.empty()) {
    return true;
  }
  for (const int a : l1) {
    if (!keyed(a)) {
      return false;
    }
  }
  for (const int a : l2) {
    if (!keyed(a)) {
      return false;
    }
  }
  if (l1.size() == 1) {
    return l1[0] == l2[0];
  }
  List s1 = l1;
  List s2 = l2;
  std::sort(s1.begin(), s1.end());
  std::sort(s2.begin(), s2.end());
  return s1 == s2;
}

bool Pass::state_equal(const RegState& s1, const RegState& s2) const {
  if (!(s1.z_tracks_a == s2.z_tracks_a && list_equal(s1.a, s2.a) && list_equal(s1.x, s2.x) &&
        list_equal(s1.y, s2.y) && list_equal(s1.z, s2.z) && s1.mem.size() == s2.mem.size())) {
    return false;
  }
  for (const auto& g1 : s1.mem) {
    if (std::none_of(s2.mem.begin(), s2.mem.end(),
                     [&](const List& g2) { return list_equal(g1, g2); })) {
      return false;
    }
  }
  return true;
}

// Returns `atoms` less those whose address depends on the register of
// class `rc`.
List Pass::without_dependent(const List& atoms, int rc) const {
  List out;
  out.reserve(atoms.size());
  for (const int a : atoms) {
    if (at(a).dep != rc) {
      out.push_back(a);
    }
  }
  return out;
}

// Returns `mem` less the operands whose address depends on the register
// of class `rc`, and less the groups left with fewer than two.
std::vector<List> Pass::drop_dependent_groups(const std::vector<List>& mem, int rc) const {
  std::vector<List> out;
  for (const auto& g : mem) {
    List g2 = without_dependent(g, rc);
    if (g2.size() >= 2) {
      out.push_back(std::move(g2));
    }
  }
  return out;
}

// Returns `may_alias` of the operands of two atoms, which reads only
// their fields and the pointer proof installed for the pass, so the
// result for a pair is kept.
bool Pass::alias(int a, int d) {
  const unsigned long long key =
      (static_cast<unsigned long long>(static_cast<unsigned>(a)) << 32) |
      static_cast<unsigned>(d);
  const auto it = alias_.find(key);
  if (it != alias_.end()) {
    return it->second;
  }
  const bool r = may_alias(at(a).op, at(d).op, ctx_);
  alias_.emplace(key, r);
  return r;
}

// Returns `atoms` less those that might alias `d`.
List Pass::without_aliasing(const List& atoms, int d) {
  List keep;
  keep.reserve(atoms.size());
  for (const int a : atoms) {
    if (!alias(a, d)) {
      keep.push_back(a);
    }
  }
  return keep;
}

Facts Pass::facts(const ir::Type_instruction& instr) {
  Facts f;
  for (const auto* op : operands_of(instr)) {
    if (is_y_setup(*op)) {
      f.setup_y = true;
    }
  }
  if (f.setup_y) {
    if (const auto* m = as<ir::Mov>(instr)) {
      f.dst_setup_y = is_y_setup(m->dst);
    } else if (const auto* p = as<ir::Pop>(instr)) {
      f.dst_setup_y = is_y_setup(p->dst);
    }
  }
  if (const auto* m = as<ir::Mov>(instr)) {
    if (!isa<ir::Reg>(m->src)) {
      f.src = atom(m->src);
    }
    if (!isa<ir::Reg>(m->dst)) {
      f.dst = atom(m->dst);
    }
  } else if (const ir::Type_operand* dst = step_or_shift_dst(instr)) {
    // An `INC A` takes the memory path of the step, as it always has.
    const auto* r = as<ir::Reg>(*dst);
    if (!(r != nullptr && isa<ir::X, ir::Y>(r->reg))) {
      f.dst = atom(*dst);
    }
  }
  f.z_effect = z_from_a(instr);
  return f;
}

List& Pass::reg_list(RegState& state, int rc) const {
  if (rc == kA) {
    return state.a;
  }
  if (rc == kX) {
    return state.x;
  }
  return state.y;
}

// Sets what the register of class `rc` mirrors. The memory operands of
// its old list still hold one value, so two or more of them become a
// memory group, less any whose address depends on the register itself.
void Pass::set_reg(RegState& state, int rc, List value) const {
  const List& cur = reg_list(state, rc);
  if (cur.size() >= 2) {
    List old;
    for (const int a : cur) {
      if (at(a).groupable && at(a).dep != rc) {
        old.push_back(a);
      }
    }
    if (old.size() >= 2 &&
        std::none_of(state.mem.begin(), state.mem.end(),
                     [&](const List& g) { return list_equal(old, g); })) {
      state.mem.push_back(std::move(old));
    }
  }
  if (rc == kA) {
    state.a = std::move(value);
    return;
  }
  // An operand indexed by the register now names another byte, in the
  // memory groups, in the other registers' lists, and in the value
  // given, as `TAX` after `LDA t,X` gives.
  if (!state.mem.empty()) {
    state.mem = drop_dependent_groups(state.mem, rc);
  }
  value = without_dependent(value, rc);
  state.a = without_dependent(state.a, rc);
  if (rc == kX) {
    state.y = without_dependent(state.y, rc);
    state.x = std::move(value);
  } else {
    state.x = without_dependent(state.x, rc);
    state.y = std::move(value);
  }
}

// Returns what a register mirrors after a load of `src`: the memory
// group that holds `src`, which leaves the groups, or `src` alone.
List Pass::load_list(RegState& state, int src) const {
  if (keyed(src)) {
    for (std::size_t i = 0; i < state.mem.size(); ++i) {
      if (contains(state.mem[i], src)) {
        List g = std::move(state.mem[i]);
        state.mem.erase(state.mem.begin() + static_cast<long>(i));
        return g;
      }
    }
  }
  return {src};
}

// Drops, from every register's list and every memory group, the
// operands that might alias `d`, the atom of a write's destination.
void Pass::invalidate_aliasing(RegState& state, int d) {
  if (!state.a.empty()) {
    state.a = without_aliasing(state.a, d);
  }
  if (!state.x.empty()) {
    state.x = without_aliasing(state.x, d);
  }
  if (!state.y.empty()) {
    state.y = without_aliasing(state.y, d);
  }
  if (!state.mem.empty()) {
    std::vector<List> out;
    for (const auto& g : state.mem) {
      List g2 = without_aliasing(g, d);
      if (g2.size() >= 2) {
        out.push_back(std::move(g2));
      }
    }
    state.mem = std::move(out);
  }
}

void Pass::invalidate_z_aliasing(RegState& state, int d) {
  if (!state.z.empty()) {
    state.z = without_aliasing(state.z, d);
  }
}

// Applies `instr`'s effect to the registers' lists and `z`, with the
// side effects of its addressing-mode setup.
//
// `Frame`, `Stack`, `Indirect`, and `IndirectZp` operands are emitted
// with an `LDY #off` in front of the access, which overwrites Y and sets
// N/Z from the offset. When the access is the instruction's last opcode
// and doesn't set N/Z itself (a store, as in `LDY #off; STA (FP),Y`),
// the flags afterwards reflect the offset, so `z` is cleared.
void Pass::update_state(const ir::Type_instruction& instr, RegState& state) {
  const Facts f = facts(instr);
  switch (ir::kind(instr)) {
    case K::Mov:
      update_for_mov(std::get<ir::Mov>(instr), state, f.src, f.dst);
      break;
    case K::Label:
    case K::Jump:
    case K::Ret:
    case K::Return:
    case K::FunctionPrologue:
    case K::AllocateStack:
    case K::LoadAddress:
    case K::DecimalArith:
      // A label is a join, and a jump or return ends the block.
      // `FunctionPrologue`, `AllocateStack`, `LoadAddress`, and
      // `DecimalArith` expand into several instructions in
      // `asm_to_asm2`, whose effects the tracker doesn't follow.
      state.reset();
      break;
    case K::Call: {
      // A JSR may change any register, memory, or flag. An X or Y the
      // callee preserves keeps its value, which still equals an
      // immediate it mirrored, but memory may have changed.
      List x;
      List y;
      if (call_preserves(instr, "X")) {
        for (const int a : state.x) {
          if (at(a).immediate) {
            x.push_back(a);
          }
        }
      }
      if (call_preserves(instr, "Y")) {
        for (const int a : state.y) {
          if (at(a).immediate) {
            y.push_back(a);
          }
        }
      }
      state.reset();
      state.x = std::move(x);
      state.y = std::move(y);
      break;
    }
    case K::Pop: {
      // PLA: the register gets an unknown value, and N/Z are set from
      // it.
      const auto& p = std::get<ir::Pop>(instr);
      if (const auto* r = as<ir::Reg>(p.dst)) {
        set_reg(state, reg_class(r->reg), {});
      }
      state.z.clear();
      break;
    }
    case K::Compare:
    case K::BitTest:
      // CMP and BIT set Z to a relation between A and the operand, not
      // to the operand's zeroness.
      state.z.clear();
      break;
    case K::Add:
    case K::Sub:
    case K::And:
    case K::Or:
    case K::Xor: {
      // ADC, SBC, AND, ORA, and EOR write A in this IR. What A holds is
      // lost, and Z reflects its new value, which isn't followed.
      if (const auto* r = as<ir::Reg>(*alu_dst(instr))) {
        set_reg(state, reg_class(r->reg), {});
      }
      state.z.clear();
      break;
    }
    case K::ArithmeticShiftLeft:
    case K::LogicalShiftRight:
    case K::RotateLeft:
    case K::RotateRight: {
      if (const auto* r = as<ir::Reg>(*step_or_shift_dst(instr))) {
        set_reg(state, reg_class(r->reg), {});
        state.z.clear();
        break;
      }
      // A shift of a memory byte sets N/Z from the byte's new value, so
      // Z reflects that byte and nothing it reflected before.
      // `LDA t; STA t2; LSR M; LDA t2; BEQ` must keep the second load,
      // whose Z the `BEQ` reads.
      invalidate_aliasing(state, f.dst);
      state.z = {f.dst};
      break;
    }
    case K::Inc:
    case K::Dec: {
      if (f.dst < 0) {
        // INX, DEX, INY, DEY: the register's own list goes, and so does
        // any other list's `IndexedData(_, _, index=reg)` operand, whose
        // address depended on the old value (`set_reg` drops both).
        set_reg(state, reg_class(std::get<ir::Reg>(*step_or_shift_dst(instr)).reg), {});
        state.z.clear();
        break;
      }
      // INC / DEC of a memory operand: Z is (the new value == 0).
      invalidate_aliasing(state, f.dst);
      state.z = {f.dst};
      break;
    }
    default:
      // A branch, a push, SEC, and CLC leave the registers and the flags
      // alone.
      break;
  }
  if (f.z_effect >= 0) {
    state.z_tracks_a = f.z_effect == 1;
  }
  if (f.setup_y) {
    set_reg(state, kY, {});
    state.z = without_dependent(state.z, kY);
    if (f.dst_setup_y) {
      state.z.clear();
      state.z_tracks_a = false;
    }
  }
}

// Applies a `Mov`'s effect to the lists and `z`. A `Mov` has four
// shapes: a transfer between registers, a load, a store, and a move from
// memory to memory (emitted as `LDA src; STA dst`). `s` and `d` are the
// atoms of the source and destination, -1 for a register.
void Pass::update_for_mov(const ir::Mov& mov, RegState& state, int s, int d) {
  if (mov.is_volatile) {
    if (d < 0) {
      // A volatile load: the register's list is cleared, so no later
      // load of M goes. `z` gets M, since Z's value is (M as read == 0)
      // for the branch right after, and a later `LDA M` wouldn't go
      // anyway, since it is volatile.
      const int dst_rc = reg_class(std::get<ir::Reg>(mov.dst).reg);
      set_reg(state, dst_rc, {});
      if (s >= 0) {
        state.z = {s};
      } else if (reg_class(std::get<ir::Reg>(mov.src).reg) == kA && dst_rc != kA) {
        // A volatile `TAX` / `TAY` writes the register a
        // `register volatile` local lives in. A keeps its value, and
        // N/Z are set from it.
        state.z = state.a;
      } else {
        state.z.clear();
      }
      return;
    }
    if (s < 0) {
      // A volatile store. M doesn't join the source register's list,
      // since a later `LDA M` couldn't go anyway.
      invalidate_aliasing(state, d);
      invalidate_z_aliasing(state, d);
      return;
    }
    // A volatile move from memory to memory, emitted as `LDA src; STA
    // dst` (and dropped when it moves a byte to itself, as below). The
    // load overwrites A, so A holds src's value and none of those it
    // held. The flag often comes from dst's being volatile while src is
    // a stable byte that isn't (`Copy(temp, volatile_y)` in sfx_tone),
    // so A keeps src for a later `LDA src`. A volatile `LDA src` doesn't
    // go anyway, and the same holds for dst: a load of dst that isn't
    // volatile exists only when dst isn't the volatile side.
    if (s == d && keyed(s)) {
      return;
    }
    invalidate_aliasing(state, d);
    invalidate_z_aliasing(state, d);
    List known;
    for (const int a : {s, d}) {
      if (at(a).stable || isa<ir::Imm>(at(a).op)) {
        known.push_back(a);
      }
    }
    state.a = known;
    state.z = std::move(known);
    return;
  }
  if (s < 0 && d < 0) {
    // TAX / TAY / TXA / TYA: dst mirrors what src mirrors, and Z reflects
    // the byte moved, the source register's value.
    List new_value = reg_list(state, reg_class(std::get<ir::Reg>(mov.src).reg));
    set_reg(state, reg_class(std::get<ir::Reg>(mov.dst).reg), new_value);
    state.z = std::move(new_value);
    return;
  }
  if (d < 0) {
    // A load: the register mirrors the source operand, and the memory
    // group that holds it (`load_list`). What it held before is gone,
    // and Z reflects (src == 0).
    //
    // A load into X or Y also drops, through `set_reg`, what the other
    // registers and the memory groups held that depends on the register
    // changed. An `IndexedData(_, _, index=X)` in A's list is stale once
    // X holds another value, and without this `LDA arr,X; LDX #N;
    // LDA arr,X` lost its second load. The list the load gives the
    // register, and `z`, lose such operands too.
    const int rc = reg_class(std::get<ir::Reg>(mov.dst).reg);
    set_reg(state, rc, {});
    List loaded = load_list(state, s);
    if (rc == kA) {
      state.a = std::move(loaded);
      state.z = {s};
    } else {
      loaded = without_dependent(loaded, rc);
      if (rc == kX) {
        state.x = std::move(loaded);
      } else {
        state.y = std::move(loaded);
      }
      if (at(s).dep != rc) {
        state.z = {s};
      } else {
        state.z.clear();
      }
    }
    return;
  }
  if (s < 0) {
    // A store. STA, STX, and STY don't touch N/Z. dst joins the source
    // register's list, so a later `LDA dst` can go. dst joins `z` only
    // when Z already followed the register's value: when `z` shares an
    // operand with the register's list, or when both are empty and the
    // last instruction that set the flags set Z from A (Z reflects A's
    // unknown value, and dst now holds it).
    //
    // A store that changes dst also ends Z's reflection of dst and of
    // every byte that may alias it. After `INC M; STA M` with A not a
    // copy of M, Z still holds the INC's test of the old M + 1, so an
    // `LDA M` before a `BEQ` must stay. A store of a register that
    // already mirrors dst writes the value dst holds, and leaves Z's
    // reflection of it alone.
    const int rc = reg_class(std::get<ir::Reg>(mov.src).reg);
    if (!(keyed(d) && contains(reg_list(state, rc), d))) {
      invalidate_z_aliasing(state, d);
    }
    invalidate_aliasing(state, d);
    if (at(d).stable) {
      List cur = reg_list(state, rc);
      const bool cur_was_empty = cur.empty();
      if (!contains(cur, d)) {
        cur.push_back(d);
        reg_list(state, rc) = cur;
      }
      // Whether Z reflects an operand of the list other than dst. An
      // operand equal to dst that Z reflects would mean Z reflects dst
      // already, and nothing is added either way.
      bool shared = false;
      for (const int c : cur) {
        if (c != d && keyed(c) && contains(state.z, c)) {
          shared = true;
          break;
        }
      }
      if (shared || (state.z.empty() && cur_was_empty && state.z_tracks_a)) {
        if (!contains(state.z, d)) {
          state.z.push_back(d);
        }
      }
    }
    return;
  }
  // A move from memory to memory, emitted as `LDA src; STA dst`. The
  // load sets Z to (src == 0), and dst now holds that value too. src
  // doesn't join A's list or `z`, since it may be an indexed operand
  // whose value later code can change, and following dst covers the
  // usual `LDA M; STA N; LDA N`.
  //
  // `Mov(M, M)` does nothing. `asm_emit` drops both the load and the
  // store, so A isn't loaded. SSA destruction leaves one when a Phi's
  // source and destination get the same byte.
  if (s == d && keyed(s)) {
    return;
  }
  invalidate_aliasing(state, d);
  invalidate_z_aliasing(state, d);
  // The load sets N/Z from the moved value, so Z reflects dst and
  // nothing older. A constant source still equals A too, whatever the
  // store wrote.
  List known;
  if (at(d).stable) {
    known.push_back(d);
  }
  if (at(s).immediate) {
    known.push_back(s);
  }
  state.a = known;
  state.z = std::move(known);
}

// Returns the memory groups that hold on every path in: each group of
// at least two operands that one group of every state contains, less the
// operands a register of the joined state still mirrors, which stay in
// that register's list.
std::vector<List> Pass::join_memory(const RegState& result,
                                    const std::vector<const RegState*>& states) const {
  const auto memory_groups = [](const RegState& s) {
    std::vector<List> g = s.mem;
    g.push_back(s.a);
    g.push_back(s.x);
    g.push_back(s.y);
    return g;
  };
  std::vector<List> groups = memory_groups(*states[0]);
  for (std::size_t k = 1; k < states.size(); ++k) {
    std::vector<List> merged;
    std::vector<List> others;
    for (auto& g2 : memory_groups(*states[k])) {
      if (!g2.empty()) {
        others.push_back(std::move(g2));
      }
    }
    for (const auto& g1 : groups) {
      if (g1.empty()) {
        continue;
      }
      for (const auto& g2 : others) {
        List inter = intersect(g1, g2);
        if (inter.size() >= 2) {
          merged.push_back(std::move(inter));
        }
      }
    }
    groups = std::move(merged);
  }
  List in_regs;
  for (const List* l : {&result.a, &result.x, &result.y}) {
    for (const int a : *l) {
      if (keyed(a)) {
        in_regs.push_back(a);
      }
    }
  }
  std::vector<List> out;
  for (const auto& g : groups) {
    List g2;
    for (const int a : g) {
      if (!contains(in_regs, a)) {
        g2.push_back(a);
      }
    }
    if (g2.size() >= 2 &&
        std::none_of(out.begin(), out.end(), [&](const List& o) { return list_equal(g2, o); })) {
      out.push_back(std::move(g2));
    }
  }
  return out;
}

// Returns the intersection of the states. `z` is dropped at a block with
// more than one predecessor. A register's list is made by the load
// itself (`LDA M` makes A hold M), so a later drop that rests on every
// path agreeing doesn't disturb the instruction that made it. An entry
// of `z` records that some earlier instruction set Z, possibly a
// different one on each path. Dropping a load because Z is already
// (M == 0) relies on one such instruction per path surviving, but the
// later dead-store and dead-A passes don't see that dependence across
// blocks and may delete a store or load that alone kept Z set on one
// path. So `z` crosses only an edge that is the one way in.
RegState Pass::join_states(const std::vector<const RegState*>& states) const {
  RegState result = *states[0];
  for (std::size_t k = 1; k < states.size(); ++k) {
    result.a = intersect(result.a, states[k]->a);
    result.x = intersect(result.x, states[k]->x);
    result.y = intersect(result.y, states[k]->y);
  }
  if (states.size() > 1) {
    result.z.clear();
    result.z_tracks_a = false;
    result.mem = join_memory(result, states);
  }
  return result;
}

// Returns the in-state of each block, by a forward worklist of what must
// be available. An in-state is nothing while no predecessor's out-state
// is known, and the join skips such predecessors, so a loop's back edge
// doesn't empty the state before it is computed.
Pass::States Pass::solve(const Cfg& cfg) {
  const std::size_t n = cfg.blocks.size();
  States in_state(n);
  States out_state(n);
  in_state[kEntryId] = RegState{};
  out_state[kEntryId] = RegState{};
  std::deque<int> worklist(cfg.block_order.begin(), cfg.block_order.end());
  std::vector<bool> in_worklist(n, false);
  for (const int b : worklist) {
    in_worklist[static_cast<std::size_t>(b)] = true;
  }
  const std::size_t max_iters = 1000 * (n + 1);
  std::size_t iters = 0;
  while (!worklist.empty() && iters < max_iters) {
    ++iters;
    const int bid = worklist.front();
    worklist.pop_front();
    const auto b = static_cast<std::size_t>(bid);
    in_worklist[b] = false;
    if (bid == kEntryId || bid == kExitId) {
      continue;
    }
    const BasicBlock& block = cfg.blocks[b];
    std::vector<const RegState*> preds;
    for (const int p : block.predecessors) {
      if (const auto& o = out_state[static_cast<std::size_t>(p)]) {
        preds.push_back(&*o);
      }
    }
    if (preds.empty()) {
      continue;
    }
    RegState new_in = join_states(preds);
    if (in_state[b] && state_equal(new_in, *in_state[b])) {
      continue;
    }
    in_state[b] = new_in;
    RegState state = std::move(new_in);
    for (std::size_t p = block.begin; p < block.end; ++p) {
      const ir::Type_instruction& instr = cfg.instruction(p);
      if (!is_passive(instr)) {
        update_state(instr, state);
      }
    }
    if (!out_state[b] || !state_equal(state, *out_state[b])) {
      out_state[b] = std::move(state);
      for (const int s : block.successors) {
        if (s == kExitId) {
          continue;
        }
        if (!in_worklist[static_cast<std::size_t>(s)]) {
          worklist.push_back(s);
          in_worklist[static_cast<std::size_t>(s)] = true;
        }
      }
    }
  }
  return in_state;
}

// Returns whether the N/Z effect of `instrs[i]`, a load the pass is
// about to drop, may be skipped: the flags are dead after it, or `z`
// already covers its source M, so Z is already (M == 0). For a transfer
// between registers, Z covers the source register when `z` shares an
// operand with its list.
bool Pass::flags_redundant_at(Instrs instrs, const LabelMap& labels, std::size_t i,
                              const RegState& state) {
  const auto* m = as<ir::Mov>(instrs[i]);
  if (m == nullptr) {
    return flags_dead_at(instrs, labels, i + 1);
  }
  if (const auto* r = as<ir::Reg>(m->src)) {
    if (intersects(state.z, reg_list(state, reg_class(r->reg)))) {
      return true;
    }
    return flags_dead_at(instrs, labels, i + 1);
  }
  if (has(state.z, m->src)) {
    return true;
  }
  return flags_dead_at(instrs, labels, i + 1);
}

bool Pass::is_redundant_load(const ir::Type_instruction& instr, const RegState& state) {
  const auto* m = as<ir::Mov>(instr);
  // A volatile load has to read the byte every time (C99 §6.7.3.6).
  if (m == nullptr || m->is_volatile) {
    return false;
  }
  const auto* dst = as<ir::Reg>(m->dst);
  if (dst == nullptr) {
    return false;
  }
  if (const auto* src = as<ir::Reg>(m->src)) {
    // A transfer between registers (TXA, TYA, TAX, TAY) does nothing
    // when the two lists share an operand: both registers hold the same
    // value. The caller checks the flags separately.
    return intersects(reg_list(state, reg_class(src->reg)), reg_list(state, reg_class(dst->reg)));
  }
  return has(reg_list(state, reg_class(dst->reg)), m->src);
}

// When `instrs[i:i+2]` is `LDA #k; STA M` (or `instrs[i]` the one atom
// `Mov(#k, M)`), A holds a byte of memory (its list has a `Data` or `ZP`
// byte, say), nothing reads the `k` in A afterwards, and Y (or else X)
// is dead, returns `LDY #k; STY M` (`LDX #k; STX M`), which leaves A
// alone, and how many instructions it replaces.
//
// `get_alphabet` in examples/zork/zstring.c returns the alphabet it
// loaded after `SHIFT_ALPHABET = 0xFF`, which then needs no copy in a
// local: `LDY #$FF; STY SHIFT_ALPHABET`, as in Zork's code. The two
// forms are the same size and speed, so the rewrite costs nothing when
// no reload goes.
std::optional<std::pair<std::vector<ir::Type_instruction>, std::size_t>>
Pass::store_const_via_index(Instrs instrs, const LabelMap& labels, std::size_t i,
                            const RegState& state) {
  const auto* first = as<ir::Mov>(instrs[i]);
  // Both forms start with a `Mov` of a constant, and both need A to hold
  // a byte of memory.
  if (first == nullptr || !isa<ir::Imm, ir::ImmLabelLow, ir::ImmLabelHigh>(first->src)) {
    return std::nullopt;
  }
  if (std::none_of(state.a.begin(), state.a.end(), [&](int a) { return at(a).data_or_zp; })) {
    return std::nullopt;
  }
  const ir::Type_operand a = ir::Reg{.reg = ir::A{}};
  ir::Mov load;
  ir::Mov store;
  std::size_t after = 0;
  if (isa<ir::Data, ir::ZP>(first->dst)) {
    // The one-atom form, `Mov(#k, M)`, which emits as the pair.
    load = ir::Mov{.src = first->src, .dst = a, .is_volatile = false};
    store = ir::Mov{.src = a, .dst = first->dst, .is_volatile = first->is_volatile};
    after = i + 1;
  } else if (i + 2 <= instrs.size()) {
    const auto* l = as<ir::Mov>(instrs[i]);
    const auto* s = as<ir::Mov>(instrs[i + 1]);
    if (l == nullptr || s == nullptr) {
      return std::nullopt;
    }
    load = *l;
    store = *s;
    after = i + 2;
  } else {
    return std::nullopt;
  }
  if (!(load.dst == a && !load.is_volatile &&
        isa<ir::Imm, ir::ImmLabelLow, ir::ImmLabelHigh>(load.src))) {
    return std::nullopt;
  }
  if (!(store.src == a && isa<ir::Data, ir::ZP>(store.dst))) {
    return std::nullopt;
  }
  // `reloads_a` usually looks at one or two instructions, so it goes
  // before the liveness walk.
  if (!reloads_a(instrs, after, state.a, store.dst)) {
    return std::nullopt;
  }
  if (!a_dead_at(instrs, labels, after)) {
    return std::nullopt;
  }
  for (const int rc : {kY, kX}) {
    const bool dead = rc == kY ? y_dead_at(instrs, labels, i) : x_dead_at(instrs, labels, i);
    if (dead) {
      const ir::Type_operand reg =
          rc == kY ? ir::Reg{.reg = ir::Y{}} : ir::Reg{.reg = ir::X{}};
      std::vector<ir::Type_instruction> out = {
          ir::Mov{.src = load.src, .dst = reg, .is_volatile = false},
          ir::Mov{.src = reg, .dst = store.dst, .is_volatile = store.is_volatile},
      };
      return std::make_pair(std::move(out), after - i);
    }
  }
  return std::nullopt;
}

// Returns whether the next instruction from `start` along the
// fall-through path that reads or writes A loads it from one of `held`,
// the operands A mirrors, unchanged since (and not `stored`). Branches
// and labels are passed over, so this guesses at a reload the second
// phase can drop. `store_const_via_index` is sound either way.
bool Pass::reloads_a(Instrs instrs, std::size_t start, const List& held,
                     const ir::Type_operand& stored) {
  std::vector<ir::Type_operand> written = {stored};
  for (std::size_t k = start; k < instrs.size(); ++k) {
    const ir::Type_instruction& ins = instrs[k];
    if (isa<ir::Label, ir::Branch>(ins)) {
      continue;
    }
    const auto* m = as<ir::Mov>(ins);
    if (m != nullptr && is_reg_a(m->dst)) {
      if (m->is_volatile || !has(held, m->src)) {
        return false;
      }
      return std::none_of(written.begin(), written.end(), [&](const ir::Type_operand& w) {
        return may_alias(w, m->src, ctx_);
      });
    }
    if (const ir::Type_operand* dst = step_or_shift_dst(ins);
        dst != nullptr && isa<ir::Inc, ir::Dec>(ins) && !isa<ir::Reg>(*dst)) {
      written.push_back(*dst);
      continue;
    }
    if (m != nullptr) {
      if (const auto* r = as<ir::Reg>(m->src); r != nullptr && !isa<ir::A>(r->reg)) {
        written.push_back(m->dst);
        continue;
      }
    }
    return false;
  }
  return false;
}

// Returns the numbers of stores `(n1, n2)` when `instrs[i:]` starts
// `LDA X; STA D1 (n1 of them); LDA M; STA D2 (n2 of them)` with A
// already mirroring M, and the two groups can trade places:
// `STA D2...; LDA X; STA D1...` does the same.
//
// No store of one group may change what the other group reads or
// writes (each `D1` against `M` and each `D2`, each `D2` against `X`),
// and A and N/Z, which end up holding X instead of M, must be dead
// after.
//
// That's the join of `floor_enemy_draw`'s dispatch: every case leaves
// the high byte of a pointer in A, and the join stores the low byte to
// a parameter first. Lode Runner's `enable_next_level_ladders` stores
// the row into a local and into `GAME_ROWNUM` and then copies the
// column, which A still holds, from its local into `GAME_COLNUM`.
std::optional<std::pair<std::size_t, std::size_t>> Pass::swap_pairs_ok(
    Instrs instrs, const LabelMap& labels, std::size_t i, const RegState& state) {
  if (i >= instrs.size() || !is_a_load(instrs[i])) {
    return std::nullopt;
  }
  const std::size_t n1 = stores_of_a_after(instrs, i + 1);
  const std::size_t j = i + 1 + n1;
  if (n1 == 0 || j >= instrs.size() || !is_a_load(instrs[j])) {
    return std::nullopt;
  }
  const std::size_t n2 = stores_of_a_after(instrs, j + 1);
  if (n2 == 0) {
    return std::nullopt;
  }
  const auto& load_x = std::get<ir::Mov>(instrs[i]);
  const auto& load_m = std::get<ir::Mov>(instrs[j]);
  std::vector<const ir::Type_operand*> d1s;
  std::vector<const ir::Type_operand*> d2s;
  for (std::size_t k = i + 1; k < j; ++k) {
    d1s.push_back(&std::get<ir::Mov>(instrs[k]).dst);
  }
  for (std::size_t k = j + 1; k < j + 1 + n2; ++k) {
    d2s.push_back(&std::get<ir::Mov>(instrs[k]).dst);
  }
  const Instrs window = instrs.subspan(i, j + 1 + n2 - i);
  if (!has(state.a, load_m.src)) {
    return std::nullopt;
  }
  if (has(state.a, load_x.src)) {
    return std::nullopt;
  }
  const ir::Type_operand& x = load_x.src;
  const ir::Type_operand& m = load_m.src;
  for (const auto* d1 : d1s) {
    if (may_alias(*d1, m, ctx_)) {
      return std::nullopt;
    }
    for (const auto* d2 : d2s) {
      if (may_alias(*d1, *d2, ctx_) && !same_index_apart(*d1, *d2, window)) {
        return std::nullopt;
      }
    }
  }
  for (const auto* d2 : d2s) {
    if (may_alias(*d2, x, ctx_)) {
      return std::nullopt;
    }
  }
  // Nor may a store of the first group write the pointer that a store of
  // the second group's address is read through, or a store of the
  // second group the one the first load's is: `LDA t,X; STA p+1; LDA #7;
  // STA (p),Y` with A holding 7 had stored through the old `p`.
  for (const auto* d1 : d1s) {
    for (const auto* d2 : d2s) {
      for (const auto& b : pointer_bytes(*d2)) {
        if (may_alias_zp(*d1, b)) {
          return std::nullopt;
        }
      }
    }
  }
  for (const auto* d2 : d2s) {
    for (const auto& b : pointer_bytes(x)) {
      if (may_alias_zp(*d2, b)) {
        return std::nullopt;
      }
    }
  }
  for (const auto* d1 : d1s) {
    // The first group's stores move after the second load, whose address
    // may be read through a pointer the stores write.
    for (const auto& b : pointer_bytes(m)) {
      if (may_alias_zp(*d1, b)) {
        return std::nullopt;
      }
    }
  }
  const std::size_t end = j + 1 + n2;
  if (!(a_dead_at(instrs, labels, end) && flags_dead_at(instrs, labels, end))) {
    return std::nullopt;
  }
  return std::make_pair(n1, n2);
}

// Returns whether `instrs[i:i+2]` is `LDX M1; LDA M2` (or `LDY M1`) with
// X (Y) already mirroring M2 and A not, so that `LDA M2; LDX M1` does
// the same and its `LDA M2` becomes `TXA` (`TYA`) in the second phase.
// Swapping a function's two register arguments, `g(b, a)` in `f(a, b)`,
// is `STA a; STX b; LDX a; LDA b` and becomes `STA a; STX b; TXA; LDX
// a`, and the store of `b` then goes when nothing reads it.
//
// A load writes only its register and N/Z, so after either order A holds
// M2 and X holds M1. The loads mustn't be volatile, since the order of
// volatile reads is the program's, and neither operand may be lowered
// with an `LDY #off` setup, which writes Y too. M2 may not be indexed by
// the register M1 goes into, which would read M2 at another address
// once the load of M1 comes after it. N/Z, which end up set from M1
// instead of M2, must be dead after.
bool Pass::swap_loads_ok(Instrs instrs, const LabelMap& labels, std::size_t i,
                         const RegState& state) {
  if (i + 2 > instrs.size()) {
    return false;
  }
  const auto* first = as<ir::Mov>(instrs[i]);
  const auto* second = as<ir::Mov>(instrs[i + 1]);
  if (first == nullptr || second == nullptr) {
    return false;
  }
  const auto* fr = as<ir::Reg>(first->dst);
  const auto* sr = as<ir::Reg>(second->dst);
  if (fr == nullptr || sr == nullptr || !isa<ir::A>(sr->reg) || !isa<ir::X, ir::Y>(fr->reg)) {
    return false;
  }
  for (const auto* ins : {first, second}) {
    if (ins->is_volatile || isa<ir::Reg>(ins->src) || sets_up_y(ir::Type_instruction{*ins})) {
      return false;
    }
  }
  const int rc = reg_class(fr->reg);
  const List& mirror = rc == kX ? state.x : state.y;
  if (const auto* idx = as<ir::IndexedData>(second->src);
      idx != nullptr && reg_class(idx->index) == rc) {
    return false;
  }
  if (!has(mirror, second->src)) {
    return false;
  }
  if (has(state.a, second->src)) {
    return false;
  }
  // `transfer_pm1_store` makes `LDA M1; SEC; SBC #k; STA M1; LDX M1` a
  // `DEX` of M1, as in Zork's `return_value`, so the load right after
  // `STA M1` stays.
  if (i > 0) {
    if (const auto* prev = as<ir::Mov>(instrs[i - 1]);
        prev != nullptr && is_reg_a(prev->src) && operands_equal(prev->dst, first->src)) {
      return false;
    }
  }
  return flags_dead_at(instrs, labels, i + 2);
}

// When `instr` is a store through A, `Mov(M, D)` with M memory or an
// immediate (emitted as `LDA M; STA D`), and A already mirrors M,
// returns `Mov(Reg(A), D)`, the `STA D` alone. The caller checks that
// the `LDA`'s N/Z effect isn't needed. That's the second store of
// `LDA #$FF; STA S; LDA #$FF; STA S+1`. A volatile `Mov` qualifies only
// with an immediate source, since the flag doesn't say which side is
// volatile.
std::optional<ir::Mov> Pass::store_from_a(const ir::Type_instruction& instr,
                                          const RegState& state) {
  const auto* m = as<ir::Mov>(instr);
  if (m == nullptr || isa<ir::Reg>(m->src) || isa<ir::Reg>(m->dst)) {
    return std::nullopt;
  }
  if (operands_equal(m->src, m->dst)) {
    return std::nullopt;
  }
  if (m->is_volatile && !isa<ir::Imm, ir::ImmLabelLow, ir::ImmLabelHigh>(m->src)) {
    return std::nullopt;
  }
  if (!has(state.a, m->src)) {
    return std::nullopt;
  }
  return ir::Mov{.src = ir::Reg{.reg = ir::A{}}, .dst = m->dst, .is_volatile = m->is_volatile};
}

// When `instr` is `LDA M` (a load of memory into A) and X or Y already
// mirrors M, returns `Mov(Reg(X|Y), Reg(A))`, a `TXA` or `TYA`. It sets
// A and N/Z as the load does, since X == M, so no separate check of the
// flags is needed. A volatile load has to read the byte (C99 §6.7.3.6),
// and a transfer between registers is `is_redundant_load`'s. It saves
// 1-2 bytes and 2-3 cycles, and A still mirrors M, so a later `STA M`
// can become `STX` / `STY` in `via_a_store_fold`.
std::optional<ir::Mov> Pass::redirect_lda_to_transfer(const ir::Type_instruction& instr,
                                                      const RegState& state) {
  const auto* m = as<ir::Mov>(instr);
  if (m == nullptr || m->is_volatile || !is_reg_a(m->dst) || isa<ir::Reg>(m->src)) {
    return std::nullopt;
  }
  if (has(state.a, m->src)) {
    return std::nullopt;
  }
  // X first, then Y. Both transfers are the same length.
  if (has(state.x, m->src)) {
    return ir::Mov{.src = ir::Reg{.reg = ir::X{}}, .dst = m->dst, .is_volatile = false};
  }
  if (has(state.y, m->src)) {
    return ir::Mov{.src = ir::Reg{.reg = ir::Y{}}, .dst = m->dst, .is_volatile = false};
  }
  return std::nullopt;
}

// When `instr` is `LDX M` or `LDY M` (a load from memory or an immediate
// that isn't volatile) and A already mirrors M, returns `TAX` or `TAY`,
// which sets the register and N/Z to the same value in one byte and two
// cycles.
//
// A parameter arriving in A that a loop counts down in Y is `STA d;
// LDY d`, which becomes `STA d; TAY`, and the store goes once nothing
// reads `d` again.
//
// The caller rewrites a load of a byte the compiler allocated only when
// nothing reads M after it (`mem_dead_after`), so that the store of M
// goes too. A load of such a byte that's still read, such as a loop
// counter's `LDX i` at the top of a loop that tests `LDA i; CMP #n`,
// stays, since `loop_counter_to_x` and the other register promotions
// after the first peephole fixed point look for the counter's `LDX`. A
// load of a named object (`named_object`), which those promotions leave
// alone, is rewritten either way: `A1 = A2 = C1; B1 = B2 = C2;
// pos(C1, C2)` passes C2 with `TAX` after the stores.
//
// The load is left alone right after `LDA M`, where `direct_index_load`
// makes `LDA M; TAX` the `LDX M` it was when A is dead after it, so the
// two passes don't undo each other.
//
// A volatile load qualifies only right after a store of A to M that
// isn't volatile. Every access of a volatile byte carries the flag, so M
// isn't volatile, and the flag comes from the register, which a
// `register volatile` local lives in (`wait`'s `y`). `TAY` writes that
// register once, as `LDY M` did, and keeps the flag.
std::optional<ir::Mov> Pass::redirect_index_load_to_transfer(
    const ir::Type_instruction& instr, const RegState& state, const ir::Type_instruction* prev) {
  const auto* m = as<ir::Mov>(instr);
  if (m == nullptr) {
    return std::nullopt;
  }
  const auto* r = as<ir::Reg>(m->dst);
  if (r == nullptr || !isa<ir::X, ir::Y>(r->reg)) {
    return std::nullopt;
  }
  if (isa<ir::Reg>(m->src) || sets_up_y(instr)) {
    return std::nullopt;
  }
  const auto* pm = prev != nullptr ? as<ir::Mov>(*prev) : nullptr;
  if (m->is_volatile && !(pm != nullptr && !pm->is_volatile && is_reg_a(pm->src) &&
                          operands_equal(pm->dst, m->src))) {
    return std::nullopt;
  }
  if (!has(state.a, m->src)) {
    return std::nullopt;
  }
  if (pm != nullptr && is_reg_a(pm->dst) && operands_equal(pm->src, m->src)) {
    return std::nullopt;
  }
  return ir::Mov{.src = ir::Reg{.reg = ir::A{}}, .dst = m->dst, .is_volatile = m->is_volatile};
}

// Returns whether `instr` is a store of A, X, or Y that isn't volatile
// into a `Data` / `ZP` byte the register is known to mirror, so the
// byte already holds the value: a store right after a load of the same
// byte, or one that a memory group made redundant.
bool Pass::stores_held_value(const ir::Type_instruction& instr, const RegState& state) {
  const auto* m = as<ir::Mov>(instr);
  if (!(m != nullptr && !m->is_volatile && isa<ir::Reg>(m->src) &&
        isa<ir::Data, ir::ZP>(m->dst))) {
    return false;
  }
  return has(reg_list(state, reg_class(std::get<ir::Reg>(m->src).reg)), m->dst);
}

// Returns, for each instruction of the function, the block it is in.
std::vector<int> blocks_of(const Cfg& cfg, std::size_t n) {
  std::vector<int> out(n, -1);
  std::size_t pos = 0;
  for (const int bid : cfg.block_order) {
    const BasicBlock& block = cfg.blocks[static_cast<std::size_t>(bid)];
    for (std::size_t p = block.begin; p < block.end; ++p, ++pos) {
      if (pos < n) {
        out[pos] = bid;
      }
    }
  }
  return out;
}

// The first phase: walks as `rewrite_with_in_states` does, but only to
// swap load-store pairs (`swap_pairs_ok`), keeping the `LDA M` for the
// second phase to drop, to swap two loads (`swap_loads_ok`), to turn a
// store through A of a value A holds into the `STA` alone
// (`store_from_a`), and to store a constant through Y or X when A holds
// a value worth keeping (`store_const_via_index`). It stops at the end
// of the block where it first rewrites something, since the in-state of
// a later block may no longer hold. Returns nothing when nothing
// applies.
std::optional<std::vector<ir::Type_instruction>> Pass::reorder_with_in_states(
    const ir::Function& fn, const Cfg& cfg, const States& in_state) {
  const Instrs instrs = fn.instructions;
  const LabelMap labels(instrs);
  const std::vector<int> block_of = blocks_of(cfg, instrs.size());
  RegState state;
  int last_block = -2;
  std::vector<ir::Type_instruction> out;
  bool changed = false;
  std::size_t i = 0;
  while (i < instrs.size()) {
    const ir::Type_instruction& instr = instrs[i];
    const int cur_block = block_of[i];
    if (cur_block != last_block && changed) {
      // The in-state of this block may no longer hold after the rewrite.
      out.insert(out.end(), instrs.begin() + static_cast<long>(i), instrs.end());
      break;
    }
    if (cur_block != last_block) {
      const auto& bin = cur_block >= 0 ? in_state[static_cast<std::size_t>(cur_block)]
                                       : std::optional<RegState>{};
      state = bin ? *bin : RegState{};
      last_block = cur_block;
    }
    if (is_passive(instr)) {
      out.push_back(instr);
      ++i;
      continue;
    }
    if (!isa<ir::Mov>(instr)) {
      // Each rewrite below starts at a `Mov`.
      out.push_back(instr);
      update_state(instr, state);
      ++i;
      continue;
    }
    if (auto via = store_const_via_index(instrs, labels, i, state)) {
      // `LDA #k; STA M` with A holding a value from memory: `LDY #k;
      // STY M` keeps it, for the second phase to drop its reload.
      for (auto& ins : via->first) {
        update_state(ins, state);
        out.push_back(std::move(ins));
      }
      changed = true;
      i += via->second;
      continue;
    }
    if (const auto swap = swap_pairs_ok(instrs, labels, i, state)) {
      // `LDA X; STA D1; LDA M; STA D2` with A already holding M:
      // `LDA M; STA D2; LDA X; STA D1`, whose `LDA M` the second phase
      // drops. Each load may have several stores.
      const auto [n1, n2] = *swap;
      std::vector<std::size_t> order;
      for (std::size_t k = i + 1 + n1; k < i + 2 + n1 + n2; ++k) {
        order.push_back(k);
      }
      for (std::size_t k = i; k < i + 1 + n1; ++k) {
        order.push_back(k);
      }
      for (const std::size_t k : order) {
        out.push_back(instrs[k]);
        update_state(instrs[k], state);
      }
      changed = true;
      i += 2 + n1 + n2;
      continue;
    }
    if (swap_loads_ok(instrs, labels, i, state)) {
      // `LDX M1; LDA M2` with X already holding M2: `LDA M2; LDX M1`,
      // whose `LDA M2` the second phase makes `TXA`.
      for (const std::size_t k : {i + 1, i}) {
        out.push_back(instrs[k]);
        update_state(instrs[k], state);
      }
      changed = true;
      i += 2;
      continue;
    }
    if (auto stored = store_from_a(instr, state);
        stored && flags_redundant_at(instrs, labels, i, state)) {
      // `Mov(M, D)` emits as `LDA M; STA D`, and with A already holding
      // M, the `STA D` alone does it.
      update_state(*stored, state);
      out.push_back(std::move(*stored));
      changed = true;
      ++i;
      continue;
    }
    out.push_back(instr);
    update_state(instr, state);
    ++i;
  }
  if (!changed) {
    return std::nullopt;
  }
  return out;
}

// The final walk: visits each instruction in source order, with the
// block's in-state at the start of each block, and drops the loads the
// tracker finds redundant. A block no path reaches starts from an empty
// state.
std::vector<ir::Type_instruction> Pass::rewrite_with_in_states(const ir::Function& fn,
                                                               const Cfg& cfg,
                                                               const States& in_state) {
  const Instrs instrs = fn.instructions;
  const LabelMap labels(instrs);
  const std::vector<int> block_of = blocks_of(cfg, instrs.size());
  RegState state;
  int last_block = -2;
  std::vector<ir::Type_instruction> out;
  out.reserve(instrs.size());
  for (std::size_t i = 0; i < instrs.size(); ++i) {
    const ir::Type_instruction& instr = instrs[i];
    const int cur_block = block_of[i];
    if (cur_block != last_block) {
      const auto& bin = cur_block >= 0 ? in_state[static_cast<std::size_t>(cur_block)]
                                       : std::optional<RegState>{};
      state = bin ? *bin : RegState{};
      last_block = cur_block;
    }
    // A label marks a block, and a terminator changes no state in the
    // model. Both stay as they are.
    if (is_passive(instr)) {
      out.push_back(instr);
      continue;
    }
    const auto* mov = as<ir::Mov>(instr);
    if (mov == nullptr) {
      // Every rewrite below is of a `Mov`.
      out.push_back(instr);
      update_state(instr, state);
      continue;
    }
    if (reverses_transfer(instrs, i) || repeats_transfer(instrs, i)) {
      // `TAX; TXA`: after the first, both registers and N/Z hold the
      // value already, as in `twice(v) { return add(v, v); }` with `v`
      // and both of add's arguments in registers. And `TXA; BEQ l;
      // TXA`, where only branches come between.
      continue;
    }
    const bool redundant = is_redundant_load(instr, state);
    if (redundant && flags_redundant_at(instrs, labels, i, state)) {
      // A, X, or Y already mirrors the load's source, and either Z
      // already reflects it or nothing reads Z before it is set again.
      continue;
    }
    if (redundant && is_reg_a(mov->dst) && !isa<ir::Reg>(mov->src)) {
      // A holds M, but a branch reads the flags `LDA M` sets.
      // `ORA #$00` sets them from A, which equals M, and is never
      // slower (`print_zstring`'s `STA c; BCC .l; RTS; .l: LDA c; BNE`
      // becomes `... .l: ORA #$00; BNE`). A is unchanged, so every
      // operand it mirrors is still there, and Z now reflects them all.
      out.push_back(ir::Or{.src = ir::Imm{.value = 0}, .dst = mov->dst});
      state.z = state.a;
      continue;
    }
    if (stores_held_value(instr, state)) {
      // The byte already holds the register's value: a group of memory
      // operands that held one value became the register's list when it
      // loaded one of them. A store sets no flag, and the state stays.
      continue;
    }
    if (auto redirected = redirect_lda_to_transfer(instr, state)) {
      // `LDA M` becomes `TXA` / `TYA` when X or Y already mirrors M.
      update_state(*redirected, state);
      out.push_back(std::move(*redirected));
      continue;
    }
    if (auto redirected =
            redirect_index_load_to_transfer(instr, state, out.empty() ? nullptr : &out.back());
        redirected && (named_object(mov->src) ||
                       asm_dead_store::mem_dead_after(instrs, labels, i, mov->src, ctx_,
                                                      fn.name, &zp_addrs_))) {
      update_state(*redirected, state);
      out.push_back(std::move(*redirected));
      continue;
    }
    out.push_back(instr);
    update_state(instr, state);
  }
  return out;
}

// Runs the dataflow of what must be available and rewrites with its
// in-states. The rewrites that change what a register or Z holds at the
// end of a block (`swap_pairs_ok`, `swap_loads_ok`, `store_from_a`,
// `store_const_via_index`) would make the in-state of a later block
// wrong, so they are a phase of their own, which stops at the end of
// the block where it first rewrites something. Then the analysis starts
// again on the rewritten code, at most `kReorderRounds` times more. A
// swap at the join before `for (i = 0; ...)` once left A holding
// another value where the next block's `i = 0` had become `STA i`.
void Pass::rewrite_function(ir::Function& fn) {
  if (fn.instructions.empty()) {
    return;
  }
  for (int reorder = kReorderRounds;; --reorder) {
    const Cfg cfg = build_cfg_tolerant(fn);
    const States in_state = solve(cfg);
    if (reorder > 0) {
      if (auto reordered = reorder_with_in_states(fn, cfg, in_state)) {
        // The phase stops at the end of the block it rewrote, so it runs
        // again on the rest with states that see the rewrite.
        fn.instructions = std::move(*reordered);
        continue;
      }
    }
    fn.instructions = rewrite_with_in_states(fn, cfg, in_state);
    return;
  }
}

}  // namespace

asm_ast::Program apply_redundant_load_elimination(asm_ast::Program prog,
                                                  const AsmContext& ctx,
                                                  const SymbolAddrs* zp_symbol_addrs) {
  const AsmContext inner = ctx.with_pointer_proof(unreachable_by_pointer(prog));
  Pass pass(inner, zp_symbol_addrs);
  for (auto& tl : prog.top_level) {
    if (auto* fn = as<ir::Function>(tl)) {
      pass.rewrite_function(*fn);
    }
  }
  return prog;
}

}  // namespace c6502::passes
