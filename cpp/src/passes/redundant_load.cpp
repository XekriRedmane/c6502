// Besides dropping each load of an operand that its register already
// mirrors, the pass makes these rewrites.
//
//   * The second of `TAX; TXA` (`TYA; TAY`, ...) goes, and so does a
//     transfer that repeats the one before a run of conditional
//     branches ending at it.
//   * `LDA M`, with A holding M and a branch reading the flags it sets,
//     becomes `ORA #$00`.
//   * `LDA M` becomes `TXA` / `TYA` when X or Y holds M, and `LDX M` /
//     `LDY M` becomes `TAX` / `TAY` when A holds M.
//   * A store of a register into a byte that already holds its value
//     goes, and `Mov(M, D)` with A holding M becomes the `STA D` alone.
//   * `LDA #k; STA M` with A holding a byte of memory becomes `LDY #k;
//     STY M` (`LDX #k; STX M`) when Y (X) is dead.
//   * `LDA X; STA D1...; LDA M; STA D2...` with A holding M becomes
//     `LDA M; STA D2...; LDA X; STA D1...`, whose `LDA M` then goes.
//   * `LDX M1; LDA M2` (`LDY M1`) with X (Y) holding M2 becomes `TXA;
//     LDX M1` (`TYA; LDY M1`).
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

constexpr int kReorderRounds = 64;

int reg_class(const ir::Type_reg& reg) { return static_cast<int>(reg.index()); }
constexpr int kA = 0;
constexpr int kX = 1;
constexpr int kY = 2;

bool is_reg_a(const ir::Type_operand& op) {
  const auto* r = as<ir::Reg>(op);
  return r != nullptr && isa<ir::A>(r->reg);
}

bool is_keyed(const ir::Type_operand& op) {
  return isa<ir::Imm, ir::ZP, ir::Data, ir::IndexedData, ir::Frame, ir::Stack,
             ir::Indirect>(op);
}

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

struct Atom {
  ir::Type_operand op;
  bool keyed = false;
  int dep = -1;
  bool stable = false;
  bool groupable = false;
  bool immediate = false;
  bool data_or_zp = false;
};

using List = std::vector<int>;

bool contains(const List& l, int a) { return std::find(l.begin(), l.end(), a) != l.end(); }

struct RegState {
  List a;
  List x;
  List y;
  List z;
  bool z_tracks_a = false;
  std::vector<List> mem;

  void reset() { *this = RegState{}; }
};

struct Facts {
  int z_effect = 0;
  bool setup_y = false;
  bool dst_setup_y = false;
  int src = -1;
  int dst = -1;
};

bool is_y_setup(const ir::Type_operand& op) {
  return isa<ir::Frame, ir::Stack, ir::Indirect, ir::IndirectZp>(op);
}

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

int z_from_a(const ir::Type_instruction& instr) {
  switch (ir::kind(instr)) {
    case K::Mov: {
      const auto& m = std::get<ir::Mov>(instr);
      if (is_reg_a(m.dst)) {
        return 1;
      }
      if (isa<ir::Reg>(m.dst)) {
        return is_reg_a(m.src) ? 1 : 0;
      }
      if (!isa<ir::Reg>(m.src)) {
        return 1;
      }
      return -1;
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

bool is_passive(const ir::Type_instruction& instr) {
  return isa<ir::Label, ir::Jump, ir::Branch, ir::Ret, ir::Return>(instr);
}

bool named_object(const ir::Type_operand& op) {
  const auto* d = as<ir::Data>(op);
  return d != nullptr && !d->name.starts_with("__local_") &&
         !d->name.starts_with("__zpabi_") && d->name != "SSP" && d->name != "FP" &&
         d->name != "HARGS" && d->name != "DPTR";
}

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

bool is_a_load(const ir::Type_instruction& ins) {
  const auto* m = as<ir::Mov>(ins);
  return m != nullptr && !m->is_volatile && is_reg_a(m->dst) && !isa<ir::Reg>(m->src);
}

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

  RegState join_states(const std::vector<const RegState*>& states) const;
  std::vector<List> join_memory(const RegState& result,
                                const std::vector<const RegState*>& states) const;
  States solve(const Cfg& cfg);

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

bool Pass::has(const List& atoms, const ir::Type_operand& op) {
  if (!is_keyed(op)) {
    return false;
  }
  return contains(atoms, atom(op));
}

bool Pass::intersects(const List& l1, const List& l2) const {
  for (const int a : l1) {
    if (keyed(a) && contains(l2, a)) {
      return true;
    }
  }
  return false;
}

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
      state.reset();
      break;
    case K::Call: {
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
      const auto& p = std::get<ir::Pop>(instr);
      if (const auto* r = as<ir::Reg>(p.dst)) {
        set_reg(state, reg_class(r->reg), {});
      }
      state.z.clear();
      break;
    }
    case K::Compare:
    case K::BitTest:
      // CMP and BIT set Z from a relation between A and the operand.
      state.z.clear();
      break;
    case K::Add:
    case K::Sub:
    case K::And:
    case K::Or:
    case K::Xor: {
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
      invalidate_aliasing(state, f.dst);
      state.z = {f.dst};
      break;
    }
    case K::Inc:
    case K::Dec: {
      if (f.dst < 0) {
        set_reg(state, reg_class(std::get<ir::Reg>(*step_or_shift_dst(instr)).reg), {});
        state.z.clear();
        break;
      }
      invalidate_aliasing(state, f.dst);
      state.z = {f.dst};
      break;
    }
    default:
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

void Pass::update_for_mov(const ir::Mov& mov, RegState& state, int s, int d) {
  if (mov.is_volatile) {
    if (d < 0) {
      const int dst_rc = reg_class(std::get<ir::Reg>(mov.dst).reg);
      set_reg(state, dst_rc, {});
      if (s >= 0) {
        state.z = {s};
      } else if (reg_class(std::get<ir::Reg>(mov.src).reg) == kA && dst_rc != kA) {
        state.z = state.a;
      } else {
        state.z.clear();
      }
      return;
    }
    if (s < 0) {
      invalidate_aliasing(state, d);
      invalidate_z_aliasing(state, d);
      return;
    }
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
    List new_value = reg_list(state, reg_class(std::get<ir::Reg>(mov.src).reg));
    set_reg(state, reg_class(std::get<ir::Reg>(mov.dst).reg), new_value);
    state.z = std::move(new_value);
    return;
  }
  if (d < 0) {
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
  if (s == d && keyed(s)) {
    return;
  }
  invalidate_aliasing(state, d);
  invalidate_z_aliasing(state, d);
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
    return intersects(reg_list(state, reg_class(src->reg)), reg_list(state, reg_class(dst->reg)));
  }
  return has(reg_list(state, reg_class(dst->reg)), m->src);
}

std::optional<std::pair<std::vector<ir::Type_instruction>, std::size_t>>
Pass::store_const_via_index(Instrs instrs, const LabelMap& labels, std::size_t i,
                            const RegState& state) {
  const auto* first = as<ir::Mov>(instrs[i]);
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
  if (i > 0) {
    if (const auto* prev = as<ir::Mov>(instrs[i - 1]);
        prev != nullptr && is_reg_a(prev->src) && operands_equal(prev->dst, first->src)) {
      return false;
    }
  }
  return flags_dead_at(instrs, labels, i + 2);
}

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

std::optional<ir::Mov> Pass::redirect_lda_to_transfer(const ir::Type_instruction& instr,
                                                      const RegState& state) {
  const auto* m = as<ir::Mov>(instr);
  if (m == nullptr || m->is_volatile || !is_reg_a(m->dst) || isa<ir::Reg>(m->src)) {
    return std::nullopt;
  }
  if (has(state.a, m->src)) {
    return std::nullopt;
  }
  if (has(state.x, m->src)) {
    return ir::Mov{.src = ir::Reg{.reg = ir::X{}}, .dst = m->dst, .is_volatile = false};
  }
  if (has(state.y, m->src)) {
    return ir::Mov{.src = ir::Reg{.reg = ir::Y{}}, .dst = m->dst, .is_volatile = false};
  }
  return std::nullopt;
}

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

bool Pass::stores_held_value(const ir::Type_instruction& instr, const RegState& state) {
  const auto* m = as<ir::Mov>(instr);
  if (!(m != nullptr && !m->is_volatile && isa<ir::Reg>(m->src) &&
        isa<ir::Data, ir::ZP>(m->dst))) {
    return false;
  }
  return has(reg_list(state, reg_class(std::get<ir::Reg>(m->src).reg)), m->dst);
}

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
      out.push_back(instr);
      update_state(instr, state);
      ++i;
      continue;
    }
    if (auto via = store_const_via_index(instrs, labels, i, state)) {
      for (auto& ins : via->first) {
        update_state(ins, state);
        out.push_back(std::move(ins));
      }
      changed = true;
      i += via->second;
      continue;
    }
    if (const auto swap = swap_pairs_ok(instrs, labels, i, state)) {
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
    if (is_passive(instr)) {
      out.push_back(instr);
      continue;
    }
    const auto* mov = as<ir::Mov>(instr);
    if (mov == nullptr) {
      out.push_back(instr);
      update_state(instr, state);
      continue;
    }
    if (reverses_transfer(instrs, i) || repeats_transfer(instrs, i)) {
      continue;
    }
    const bool redundant = is_redundant_load(instr, state);
    if (redundant && flags_redundant_at(instrs, labels, i, state)) {
      continue;
    }
    if (redundant && is_reg_a(mov->dst) && !isa<ir::Reg>(mov->src)) {
      out.push_back(ir::Or{.src = ir::Imm{.value = 0}, .dst = mov->dst});
      state.z = state.a;
      continue;
    }
    if (stores_held_value(instr, state)) {
      continue;
    }
    if (auto redirected = redirect_lda_to_transfer(instr, state)) {
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

void Pass::rewrite_function(ir::Function& fn) {
  if (fn.instructions.empty()) {
    return;
  }
  for (int reorder = kReorderRounds;; --reorder) {
    const Cfg cfg = build_cfg_tolerant(fn);
    const States in_state = solve(cfg);
    if (reorder > 0) {
      if (auto reordered = reorder_with_in_states(fn, cfg, in_state)) {
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
