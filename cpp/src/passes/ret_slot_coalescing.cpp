// Keeps a local copy of a function's zero-page result in the result's
// own bytes.
//
// The pass renames two of `G`'s own locals `L0` and `L1` to `F@ret`'s
// bytes when all of these hold.
//
//   * `G` calls `F`, directly or through other functions of the unit,
//     so that `G` is already a function whose call may write `F@ret`.
//     `memory_value_propagation` and every other pass that keeps a fact
//     about `F@ret` across a call drops it at a call of `G` (`G`'s call
//     summary includes `F`'s writes), so the writes of `F@ret` that the
//     rename adds to `G` change nothing that a caller of `G` holds.
//   * `G` is in the private-pool model, isn't recursive, and makes no
//     `save_hardware_stack` call, so the linker rejects any program in
//     which one of `G`'s calls leads back to `G` (see the external
//     calls below).
//   * `L0` and `L1` are copied from `F@ret` and `F@ret+1` somewhere in
//     `G` (`LDA F@ret; STA L0`, through any register, or a
//     memory-to-memory `Mov`). Neither is reached by a pointer
//     (`asm_aliasing.unreachable_by_pointer`), each is named only at
//     offset 0 and only by instructions whose reads and writes the pass
//     models (`slot_coalescing::excluded`), and neither is named by a
//     `ZP` operand. A `(zp),Y` access through the pair names it by
//     `L0`'s address, which the rename moves to `F@ret`'s, and then `L1`
//     must be the byte after `L0`.
//   * Neither byte of the pair interferes with its byte of `F@ret`: no
//     instruction writes one while the other is live after it, except a
//     copy of one into the other. `F@ret` is live where `G` reads it,
//     explicitly or as a `(zp),Y` base, and dead at a return or a tail
//     call, since a caller of `G` treats a call of `G` as a write of
//     `F@ret`. A call writes `F@ret` when it's a call of `F` or of a
//     function of the unit that calls `F`, and may write it when it
//     leaves the unit (a function the unit doesn't define, or one of
//     the unit's that calls one). From outside, a function of the unit
//     is reached only through its entry points, the global functions
//     and the ones whose address is taken. An entry point that calls
//     `F` and that leads to `G` would make a cycle through `G`'s call,
//     which the linker rejects, so a call that leaves the unit can
//     write `F@ret` only when an entry point calls `F` without leading
//     to `G`. Calls don't read `F@ret` or `G`'s locals.
#include "passes/ret_slot_coalescing.h"

#include <array>
#include <concepts>
#include <cstddef>
#include <map>
#include <optional>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "passes/asm_aliasing.h"
#include "passes/asm_liveness.h"
#include "passes/operand_rewrite.h"
#include "passes/slot_coalescing.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::asm_ast;
using ::c6502::asdl::as;
using ::c6502::asdl::isa;

using Instructions = std::vector<ir::Type_instruction>;

bool is_rmw(const ir::Type_instruction& ins) {
  return isa<ir::Inc, ir::Dec, ir::ArithmeticShiftLeft, ir::LogicalShiftRight,
             ir::RotateLeft, ir::RotateRight>(ins);
}

bool is_alu(const ir::Type_instruction& ins) {
  return isa<ir::Add, ir::Sub, ir::And, ir::Or, ir::Xor>(ins);
}

const ir::Type_operand* dst_of(const ir::Type_instruction& ins) {
  return std::visit(
      [](const auto& x) -> const ir::Type_operand* {
        if constexpr (requires {
                        { x.dst } -> std::same_as<const ir::Type_operand&>;
                      }) {
          return &x.dst;
        } else {
          return nullptr;
        }
      },
      ins);
}

constexpr std::array<const char*, 4> kCells = {"$L0", "$L1", "$R0", "$R1"};
constexpr int kL0 = 0;
constexpr int kL1 = 1;
constexpr int kR0 = 2;
constexpr int kR1 = 3;
constexpr int partner(int cell) { return cell ^ 2; }

int cell_of(const ir::Type_operand& op) {
  const auto* d = as<ir::Data>(op);
  if (d == nullptr) {
    return -1;
  }
  for (int i = 0; i < 4; ++i) {
    if (d->name == kCells[static_cast<std::size_t>(i)]) {
      return i;
    }
  }
  return -1;
}

class CallGraph {
 public:
  CallGraph(const ir::Program& prog,
            const std::map<std::string, const ir::Function*>& fns) {
    std::map<std::string, std::set<std::string>> callees;
    for (const auto& [name, fn] : fns) {
      defined_.insert(name);
      std::unordered_set<std::string> labels;
      for (const auto& i : fn->instructions) {
        if (const auto* label = as<ir::Label>(i)) {
          labels.insert(label->name);
        }
      }
      auto& out = callees[name];
      for (const auto& i : fn->instructions) {
        if (const auto* call = as<ir::Call>(i)) {
          out.insert(call->name);
        } else if (const auto* jump = as<ir::Jump>(i);
                   jump != nullptr && !labels.contains(jump->target)) {
          out.insert(jump->target);
        }
      }
    }
    for (const auto& [name, fn] : fns) {
      std::set<std::string> seen;
      std::vector<std::string> stack(callees[name].begin(), callees[name].end());
      while (!stack.empty()) {
        std::string g = std::move(stack.back());
        stack.pop_back();
        if (seen.contains(g)) {
          continue;
        }
        seen.insert(g);
        if (const auto it = callees.find(g); it != callees.end()) {
          stack.insert(stack.end(), it->second.begin(), it->second.end());
        }
      }
      reach_[name] = std::move(seen);
    }
    std::set<std::string> taken;
    for (const auto& tl : prog.top_level) {
      if (const auto* sv = as<ir::StaticVariable>(tl)) {
        for (const auto& init : sv->init) {
          if (const auto* a = as<ir::AddressInit>(init)) {
            taken.insert(a->name);
          } else if (const auto* b = as<ir::AddressByteInit>(init)) {
            taken.insert(b->name);
          }
        }
      } else if (const auto* fn = as<ir::Function>(tl)) {
        for (const auto& ins : fn->instructions) {
          for (const auto& op : instruction_operands(ins)) {
            if (const auto* lo = as<ir::ImmLabelLow>(op)) {
              taken.insert(lo->name);
            } else if (const auto* hi = as<ir::ImmLabelHigh>(op)) {
              taken.insert(hi->name);
            }
          }
          if (const auto* la = as<ir::LoadAddress>(ins)) {
            std::visit(
                [&](const auto& src) {
                  if constexpr (requires { src.name; }) {
                    taken.insert(src.name);
                  }
                },
                la->src);
          }
        }
      }
    }
    for (const auto& [name, fn] : fns) {
      if (fn->is_global || taken.contains(name)) {
        entries_.push_back(name);
      }
    }
  }

  const std::set<std::string>* reach(const std::string& name) const {
    const auto it = reach_.find(name);
    return it == reach_.end() ? nullptr : &it->second;
  }

  bool leaves_unit(const std::string& name) const {
    if (!defined_.contains(name)) {
      return true;
    }
    for (const auto& g : reach_.at(name)) {
      if (!defined_.contains(g)) {
        return true;
      }
    }
    return false;
  }

  bool may_write_ret(const std::string& callee, const std::string& f,
                     const std::string& g) const {
    const auto* r = reach(callee);
    if (callee == f || (r != nullptr && r->contains(f))) {
      return true;
    }
    if (!leaves_unit(callee)) {
      return false;
    }
    for (const auto& e : entries_) {
      const auto& re = reach_.at(e);
      if (e == f || re.contains(f)) {
        if (e != g && !re.contains(g)) {
          return true;
        }
      }
    }
    return false;
  }

 private:
  std::set<std::string> defined_;
  std::map<std::string, std::set<std::string>> reach_;
  std::vector<std::string> entries_;
};

std::array<std::set<std::string>, 2> copies_of(const Instructions& instrs,
                                               const std::string& ret,
                                               const std::string& prefix) {
  std::array<std::set<std::string>, 2> out;
  for (std::size_t k = 0; k < instrs.size(); ++k) {
    const auto* mov = as<ir::Mov>(instrs[k]);
    if (mov == nullptr || mov->is_volatile) {
      continue;
    }
    const ir::Type_operand* src = &mov->src;
    const auto* reg = as<ir::Reg>(*src);
    if (reg != nullptr && isa<ir::A>(reg->reg) && k > 0) {
      const auto* prev = as<ir::Mov>(instrs[k - 1]);
      if (prev != nullptr && prev->dst == *src) {
        src = &prev->src;
      }
    }
    const auto* s = as<ir::Data>(*src);
    const auto* d = as<ir::Data>(mov->dst);
    if (s != nullptr && s->name == ret && (s->offset == 0 || s->offset == 1) &&
        d != nullptr && d->offset == 0 && d->name.starts_with(prefix)) {
      out[static_cast<std::size_t>(s->offset)].insert(d->name);
    }
  }
  return out;
}

struct Cells {
  std::array<std::pair<std::string, asdl::Int>, 4> names;
  std::map<asdl::Int, int> at;

  int named(const std::string& name, asdl::Int offset) const {
    for (int i = 0; i < 4; ++i) {
      const auto& n = names[static_cast<std::size_t>(i)];
      if (n.first == name && n.second == offset) {
        return i;
      }
    }
    return -1;
  }
  bool names_name(const std::string& name) const {
    for (const auto& n : names) {
      if (n.first == name) {
        return true;
      }
    }
    return false;
  }
  int cell_at(asdl::Int addr) const {
    const auto it = at.find(addr);
    return it == at.end() ? -1 : it->second;
  }
};

std::optional<ir::Type_instruction> canonical(const ir::Type_instruction& ins,
                                              const Cells& cells) {
  bool touched = false;
  for (const auto& op : instruction_operands(ins)) {
    if (const auto* zp = as<ir::ZP>(op);
        zp != nullptr && cells.at.contains(zp->address + zp->offset)) {
      return std::nullopt;
    }
    if (const auto* x = as<ir::IndexedData>(op);
        x != nullptr && (cells.named(x->name, 0) >= 0 || cells.named(x->name, 1) >= 0)) {
      return std::nullopt;
    }
    std::optional<asdl::Int> base;
    if (const auto* zi = as<ir::IndirectZp>(op)) {
      base = zi->address;
    } else if (const auto* zy = as<ir::IndirectZpY>(op)) {
      base = zy->address;
    }
    if (base) {
      const int lo = cells.cell_at(*base);
      const int hi = cells.cell_at(*base + 1);
      if (lo < 0 && hi < 0) {
        continue;
      }
      if (!((lo == kL0 && hi == kL1) || (lo == kR0 && hi == kR1))) {
        return std::nullopt;
      }
      touched = true;
    }
    if (const auto* d = as<ir::Data>(op)) {
      if (cells.named(d->name, d->offset) >= 0) {
        touched = true;
      } else if (cells.names_name(d->name)) {
        return std::nullopt;
      }
    }
  }
  if (!touched) {
    return ins;
  }
  if (!(isa<ir::Mov, ir::Pop, ir::Compare, ir::BitTest, ir::Push>(ins) ||
        is_rmw(ins) || is_alu(ins))) {
    return std::nullopt;
  }
  if (const auto* mov = as<ir::Mov>(ins); mov != nullptr && mov->is_volatile) {
    return std::nullopt;
  }
  ir::Type_instruction out = ins;
  for_each_operand_mut(out, [&](ir::Type_operand& op) {
    if (const auto* d = as<ir::Data>(op)) {
      const int cell = cells.named(d->name, d->offset);
      if (cell >= 0) {
        op = ir::Data{.name = kCells[static_cast<std::size_t>(cell)], .offset = 0};
      }
    }
  });
  return out;
}

struct CellRw {
  unsigned reads = 0;
  unsigned writes = 0;
};

unsigned base_cells(const ir::Type_operand& op, const Cells& cells) {
  std::optional<asdl::Int> base;
  if (const auto* zi = as<ir::IndirectZp>(op)) {
    base = zi->address;
  } else if (const auto* zy = as<ir::IndirectZpY>(op)) {
    base = zy->address;
  }
  unsigned out = 0;
  if (base) {
    for (const asdl::Int addr : {*base, *base + 1}) {
      if (const int c = cells.cell_at(addr); c >= 0) {
        out |= 1u << c;
      }
    }
  }
  return out;
}

CellRw cell_reads_writes(const ir::Type_instruction& ins, const Cells& cells,
                         const std::string& f, const std::string& g,
                         const CallGraph& graph) {
  CellRw rw;
  if (const auto* call = as<ir::Call>(ins)) {
    if (graph.may_write_ret(call->name, f, g)) {
      rw.writes = (1u << kR0) | (1u << kR1);
    }
    return rw;
  }
  const auto ops = instruction_operands(ins);
  for (const auto& op : ops) {
    rw.reads |= base_cells(op, cells);
  }
  const auto bit = [](const ir::Type_operand& op) -> unsigned {
    const int c = cell_of(op);
    return c < 0 ? 0u : 1u << c;
  };
  if (const auto* mov = as<ir::Mov>(ins)) {
    rw.reads |= bit(mov->src);
    rw.writes |= bit(mov->dst);
  } else if (const auto* pop = as<ir::Pop>(ins)) {
    rw.writes |= bit(pop->dst);
  } else if (is_rmw(ins)) {
    rw.reads |= bit(*dst_of(ins));
    rw.writes |= bit(*dst_of(ins));
  } else {
    for (const auto& op : ops) {
      rw.reads |= bit(op);
    }
    if (is_alu(ins)) {
      rw.writes |= bit(*dst_of(ins));
    }
  }
  return rw;
}

std::vector<unsigned> live_out(const Instructions& instrs,
                               const std::vector<CellRw>& rw) {
  const LabelMap label_at(instrs);
  std::vector<std::vector<std::size_t>> succ(instrs.size());
  for (std::size_t k = 0; k < instrs.size(); ++k) {
    succ[k] = slot_coalescing::successors(instrs, k, label_at);
  }
  std::vector<unsigned> live_in(instrs.size(), 0);
  std::vector<unsigned> live_out(instrs.size(), 0);
  bool changed = true;
  while (changed) {
    changed = false;
    for (std::size_t k = instrs.size(); k-- > 0;) {
      unsigned out = 0;
      for (const std::size_t s : succ[k]) {
        out |= live_in[s];
      }
      const unsigned new_in = rw[k].reads | (out & ~rw[k].writes);
      if (out != live_out[k] || new_in != live_in[k]) {
        live_out[k] = out;
        live_in[k] = new_in;
        changed = true;
      }
    }
  }
  return live_out;
}

std::optional<Instructions> try_pair(const ir::Function& fn, const std::string& l0,
                                     const std::string& l1, const std::string& ret,
                                     const std::string& f, const CallGraph& graph,
                                     const NameSet& unreachable,
                                     const SymbolAddrs& slot_symbols,
                                     asdl::Int ret_addr) {
  const Instructions& instrs = fn.instructions;
  if (!(unreachable.contains(l0) && unreachable.contains(l1))) {
    return std::nullopt;
  }
  const auto i0 = slot_symbols.find(l0);
  const auto i1 = slot_symbols.find(l1);
  if (i0 == slot_symbols.end() || i1 == slot_symbols.end()) {
    return std::nullopt;
  }
  if (!slot_coalescing::excluded(instrs, NameSet{l0, l1}).empty()) {
    return std::nullopt;
  }
  const asdl::Int a0 = i0->second;
  const asdl::Int a1 = i1->second;
  Cells cells;
  cells.names = {std::make_pair(l0, asdl::Int(0)), std::make_pair(l1, asdl::Int(0)),
                 std::make_pair(ret, asdl::Int(0)), std::make_pair(ret, asdl::Int(1))};
  cells.at[a0] = kL0;
  cells.at[a1] = kL1;
  cells.at[ret_addr] = kR0;
  cells.at[ret_addr + 1] = kR1;
  Instructions canon;
  canon.reserve(instrs.size());
  for (const auto& ins : instrs) {
    auto c = canonical(ins, cells);
    if (!c) {
      return std::nullopt;
    }
    canon.push_back(std::move(*c));
  }
  std::vector<CellRw> rw;
  rw.reserve(canon.size());
  for (const auto& c : canon) {
    rw.push_back(cell_reads_writes(c, cells, f, fn.name, graph));
  }
  const auto live = live_out(canon, rw);
  const auto copies =
      slot_coalescing::copy_sources(canon, NameSet{"$L0", "$L1", "$R0", "$R1"});
  for (std::size_t k = 0; k < rw.size(); ++k) {
    for (int w = 0; w < 4; ++w) {
      if (!(rw[k].writes & (1u << w))) {
        continue;
      }
      const int other = partner(w);
      if (!(live[k] & (1u << other))) {
        continue;
      }
      const auto it = copies.find(k);
      if (it == copies.end() || it->second != kCells[static_cast<std::size_t>(other)]) {
        return std::nullopt;
      }
    }
  }
  Instructions out = instrs;
  for (auto& ins : out) {
    for_each_operand_mut(ins, [&](ir::Type_operand& op) {
      if (auto* d = as<ir::Data>(op)) {
        if (d->offset == 0 && d->name == l0) {
          op = ir::Data{.name = ret, .offset = 0};
        } else if (d->offset == 0 && d->name == l1) {
          op = ir::Data{.name = ret, .offset = 1};
        }
      } else if (auto* p = as<ir::IndirectZp>(op); p && p->address == a0) {
        p->address = ret_addr;
      } else if (auto* q = as<ir::IndirectZpY>(op); q && q->address == a0) {
        q->address = ret_addr;
      }
    });
  }
  return out;
}

bool coalesce_one(ir::Function& fn, const std::string& ret, const std::string& f,
                  const CallGraph& graph, const NameSet& unreachable,
                  const SymbolAddrs& slot_symbols, asdl::Int ret_addr) {
  const auto [lows, highs] =
      copies_of(fn.instructions, ret, "__local_" + fn.name + "__");
  for (const auto& l0 : lows) {
    for (const auto& l1 : highs) {
      if (l0 == l1) {
        continue;
      }
      auto renamed =
          try_pair(fn, l0, l1, ret, f, graph, unreachable, slot_symbols, ret_addr);
      if (renamed) {
        fn.instructions = std::move(*renamed);
        return true;
      }
    }
  }
  return false;
}

}  // namespace

asm_ast::Program apply_ret_slot_coalescing(asm_ast::Program prog,
                                           const SymbolAddrs& slot_symbols,
                                           const SymbolAddrs& zp_addrs,
                                           const NameSet& eligible) {
  std::map<std::string, const ir::Function*> fns;
  for (const auto& tl : prog.top_level) {
    if (const auto* fn = as<ir::Function>(tl)) {
      fns[fn->name] = fn;
    }
  }
  std::map<std::string, std::string> rets;
  for (const auto& tl : prog.top_level) {
    const auto* sv = as<ir::StaticVariable>(tl);
    if (sv == nullptr || !sv->name.ends_with("@ret")) {
      continue;
    }
    const std::string f = sv->name.substr(0, sv->name.size() - 4);
    const auto addr = zp_addrs.find(sv->name);
    if (fns.contains(f) && addr != zp_addrs.end() && addr->second < 0xFF) {
      rets[sv->name] = f;
    }
  }
  if (rets.empty()) {
    return prog;
  }
  const CallGraph graph(prog, fns);
  const NameSet unreachable = unreachable_by_pointer(prog);
  for (auto& tl : prog.top_level) {
    auto* fn = as<ir::Function>(tl);
    if (fn == nullptr || !eligible.contains(fn->name)) {
      continue;
    }
    for (const auto& [ret, f] : rets) {
      const auto* reach = graph.reach(fn->name);
      if (fn->name != f && reach != nullptr && reach->contains(f)) {
        for (int round = 0; round < 8; ++round) {
          if (!coalesce_one(*fn, ret, f, graph, unreachable, slot_symbols,
                            zp_addrs.at(ret))) {
            break;
          }
        }
      }
    }
  }
  return prog;
}

}  // namespace c6502::passes
