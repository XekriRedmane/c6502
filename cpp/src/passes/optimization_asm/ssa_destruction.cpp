// Asm-level SSA destruction.
//
// Lowers every `Phi` to `Mov`s on the edges into its block. After this
// pass no `Phi` is left, and the function is ordinary asm, ready for
// `replace_pseudoregisters`, which has no `Phi` handling.
//
// For each `Phi` in a block M,
//
//     Phi(dst, [(pred_label_1, src_1), ..., (pred_label_n, src_n)])
//
// one `Mov(src_k, dst)` goes on the edge from pred_k to M.
//
// Splitting critical edges. Asm SSA differs from TAC SSA here: a
// predecessor can end with a `Branch`, which reads the flags, and a
// `Mov` between the branch and the instruction that set its flags
// would change them (LDA sets N and Z). So every edge from a block that
// ends in a `Branch` to a block with `Phi`s gets a new block on it, and
// the moves go in the new block. An edge from a block that ends in a
// `Jump` takes the moves before the `Jump`, which reads no flag, so it
// isn't split.
//
// The order of a parallel copy. Several `Phi`s in one block give
// several moves on each edge, which `order_parallel_copies`
// (parallel_copies.cpp) orders as the TAC `from_ssa` does, breaking a
// cycle with a new temporary `.<funcname>@asm_cycle_tmp@<N>`.
#include "passes/optimization_asm/ssa_destruction.h"

#include <algorithm>
#include <cstddef>
#include <map>
#include <optional>
#include <set>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

#include "ordered_map.h"
#include "passes/optimization_asm/owned_cfg.h"
#include "passes/optimization_asm/parallel_copies.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::asm_ast;
using ::c6502::asdl::as;
using ::c6502::asdl::isa;

bool is_terminator(const ir::Type_instruction& instr) {
  return isa<ir::Jump, ir::Branch, ir::Return, ir::Ret>(instr);
}

// Returns the labels of the blocks that hold one `Phi` or more.
std::set<std::string, std::less<>> phi_bearing_labels(
    const std::vector<ir::Type_instruction>& instrs) {
  std::set<std::string, std::less<>> out;
  const std::string* current = nullptr;
  for (const auto& instr : instrs) {
    if (const auto* l = as<ir::Label>(instr)) {
      current = &l->name;
    } else if (isa<ir::Phi>(instr) && current != nullptr) {
      out.insert(*current);
    }
  }
  return out;
}

// Returns, for each index i, the name of the last `Label` at or before
// i, or nothing for an instruction before every label.
std::vector<std::optional<std::string>> block_leading_label_index(
    const std::vector<ir::Type_instruction>& instrs) {
  std::vector<std::optional<std::string>> out(instrs.size());
  std::optional<std::string> current;
  for (std::size_t i = 0; i < instrs.size(); ++i) {
    if (const auto* l = as<ir::Label>(instrs[i])) {
      current = l->name;
    }
    out[i] = current;
  }
  return out;
}

std::string mint_split_label(const std::string& fn_name, int n) {
  return "." + fn_name + "@asm_ssa_split@" + std::to_string(n);
}

// In every `Phi` of the block whose leading label is `merge_label`,
// renames the first argument from `old_pred_label` to `new_pred_label`.
// A `Phi` has one argument per edge, so it has two from P only when P
// ends in a `Branch` to the label right after it and both of P's edges
// go to the merge. `to_ssa` adds the taken edge's argument first, and
// the taken edge is split first, so each split takes one of the two.
void rewrite_phi_pred_label(std::vector<ir::Type_instruction>& instrs,
                            const std::string& merge_label, const std::string& old_pred_label,
                            const std::string& new_pred_label) {
  bool in_target_block = false;
  for (auto& instr : instrs) {
    if (const auto* l = as<ir::Label>(instr)) {
      in_target_block = l->name == merge_label;
      continue;
    }
    if (!in_target_block) {
      continue;
    }
    auto* phi = as<ir::Phi>(instr);
    if (phi == nullptr) {
      // The block's `Phi`s are behind it.
      in_target_block = false;
      continue;
    }
    for (auto& a : phi->args) {
      auto& arg = std::get<ir::AsmPhiArg>(a);
      if (arg.pred_label == old_pred_label) {
        arg.pred_label = new_pred_label;
        break;
      }
    }
  }
}

// Puts a new block on every edge from a block that ends in a `Branch`
// to a block with `Phi`s, so that the moves of the destruction land
// where they can't change the branch's flags. There are two cases.
//   - `Branch(cond, M_label)` to M, the branch's taken target. A new
//     label `L_split` is made, the branch goes to it, and `Label(L_split);
//     Jump(M_label)` goes at the end of the function. The moves go
//     between the label and the jump later.
//   - `Branch(cond, _)` followed in source order by `Label(M)`, so that
//     M is the branch's fall-through. `Label(L_split)` goes between the
//     branch and `Label(M)`, and the branch falls through into the new
//     block, which falls through to M, with no new jump. The moves go
//     between `L_split` and `Label(M)`.
// In both cases each `Phi` argument of M that named the predecessor P
// is renamed to name `L_split`, so that the destruction finds the new
// block for it.
ir::Function split_critical_edges_for_phi_merges(const ir::Function& fn) {
  std::vector<ir::Type_instruction> instrs = fn.instructions;
  const auto phi_merges = phi_bearing_labels(instrs);
  if (phi_merges.empty()) {
    return fn;
  }
  int split_counter = 0;
  // The fall-through inserts change the order of the instructions, so
  // they are applied after the taken edges, from the last to the first.
  // Each is (insert position, new label, label of the merge).
  std::vector<std::tuple<std::size_t, std::string, std::string>> fall_through_inserts;
  // Each is (new label, label of the merge, label of the predecessor).
  std::vector<std::tuple<std::string, std::string, std::string>> taken_appends;
  const auto pred_label_at = block_leading_label_index(instrs);

  for (std::size_t i = 0; i < instrs.size(); ++i) {
    const auto* branch = as<ir::Branch>(instrs[i]);
    if (branch == nullptr) {
      continue;
    }
    // The predecessor is the block that holds the branch, named by the
    // last label at or before it. After SSA construction every block
    // has one.
    const auto& p_label = pred_label_at[i];
    if (!p_label) {
      continue;
    }
    if (phi_merges.contains(branch->target)) {
      std::string split_label = mint_split_label(fn.name, split_counter++);
      taken_appends.emplace_back(split_label, branch->target, *p_label);
      instrs[i] = ir::Branch{.cond = branch->cond, .target = split_label};
    }
    const std::size_t next_idx = i + 1;
    if (next_idx < instrs.size()) {
      if (const auto* l = as<ir::Label>(instrs[next_idx]); l != nullptr && phi_merges.contains(l->name)) {
        fall_through_inserts.emplace_back(next_idx, mint_split_label(fn.name, split_counter++),
                                          l->name);
      }
    }
  }

  std::vector<ir::Type_instruction> appended;
  for (const auto& [split_label, target_label, old_pred_label] : taken_appends) {
    appended.push_back(ir::Label{.name = split_label});
    appended.push_back(ir::Jump{.target = target_label});
    rewrite_phi_pred_label(instrs, target_label, old_pred_label, split_label);
  }
  // The inserts go in from the last position to the first, so that an
  // insert doesn't move the positions of those still to go. The
  // positions are distinct, since each follows its own branch.
  std::stable_sort(fall_through_inserts.begin(), fall_through_inserts.end(),
                   [](const auto& a, const auto& b) { return std::get<0>(a) > std::get<0>(b); });
  for (const auto& [insert_pos, split_label, target_label] : fall_through_inserts) {
    // The merge's predecessor by the fall-through is the block that
    // holds the branch at `insert_pos - 1`.
    const auto& old_pred_label = pred_label_at[insert_pos - 1];
    instrs.insert(instrs.begin() + static_cast<std::ptrdiff_t>(insert_pos),
                  ir::Label{.name = split_label});
    if (old_pred_label) {
      rewrite_phi_pred_label(instrs, target_label, *old_pred_label, split_label);
    }
  }
  instrs.insert(instrs.end(), appended.begin(), appended.end());
  return ir::Function{.name = fn.name,
                      .is_global = fn.is_global,
                      .params = fn.params,
                      .instructions = std::move(instrs)};
}

// Sends each move of a non-register, non-immediate source into X or Y
// through A, so that the result can be assembled after
// `replace_pseudoregisters`. A `Phi` whose destination the coloring put
// in X or Y can have arguments whose source is still a Pseudo (a
// parameter, an address-taken local, a static, which stay symbolic
// until `replace_pseudoregisters`). The 6502 has no `LDX (zp),Y` or
// `LDY (zp),Y`, so a direct move from a frame, stack, or indirect byte
// into X or Y can't be assembled. Which Pseudos become zero-page or
// `Data` bytes, which could be, isn't known here, so every source but
// an immediate or a register goes through A.
std::vector<ir::Mov> safe_dst_xy_split(std::vector<ir::Mov> movs) {
  if (movs.empty()) {
    return movs;
  }
  std::vector<ir::Mov> out;
  out.reserve(movs.size());
  const ir::Reg reg_a{.reg = ir::A{}};
  for (auto& m : movs) {
    const auto* dst = as<ir::Reg>(m.dst);
    if (dst != nullptr && isa<ir::X, ir::Y>(dst->reg) && !isa<ir::Imm, ir::Reg>(m.src)) {
      out.push_back(ir::Mov{.src = m.src, .dst = reg_a});
      out.push_back(ir::Mov{.src = reg_a, .dst = m.dst});
    } else {
      out.push_back(std::move(m));
    }
  }
  return out;
}

}  // namespace

ir::Function from_ssa(const ir::Function& input) {
  const ir::Function fn = split_critical_edges_for_phi_merges(input);
  OwnedCfg cfg = build_owned_cfg(fn);
  std::unordered_map<std::string, int> label_to_block;
  for (std::size_t id = 0; id < cfg.code.size(); ++id) {
    const auto& code = cfg.code[id];
    if (!code.empty()) {
      if (const auto* l = as<ir::Label>(code[0])) {
        label_to_block[l->name] = static_cast<int>(id);
      }
    }
  }
  int cycle_counter = 0;

  for (const int id : cfg.graph.block_order) {
    std::vector<ir::Phi> phis;
    for (const auto& instr : cfg.code[id]) {
      if (const auto* phi = as<ir::Phi>(instr)) {
        phis.push_back(*phi);
      }
    }
    if (phis.empty()) {
      continue;
    }
    OrderedMap<std::string, std::vector<ir::Mov>> per_pred;
    for (const auto& phi : phis) {
      for (const auto& a : phi.args) {
        const auto& arg = std::get<ir::AsmPhiArg>(a);
        std::vector<ir::Mov>* movs = per_pred.find(arg.pred_label);
        if (movs == nullptr) {
          movs = &per_pred.insert_or_assign(arg.pred_label, {});
        }
        movs->push_back(ir::Mov{.src = arg.source, .dst = phi.dst});
      }
    }
    for (auto& [pred_label, movs] : per_pred) {
      const auto it = label_to_block.find(pred_label);
      if (it == label_to_block.end()) {
        // The predecessor was dropped, so the argument is dead.
        continue;
      }
      std::vector<ir::Mov> ordered =
          safe_dst_xy_split(order_parallel_copies(std::move(movs), fn.name, cycle_counter));
      auto& pred_code = cfg.code[it->second];
      std::size_t insert_pos = pred_code.size();
      if (!pred_code.empty() && is_terminator(pred_code.back())) {
        // The predecessors that weren't split end with a `Jump`,
        // `Return`, or `Ret`, since those that end in a `Branch` now go
        // through a split block. The moves go before a `Jump`, which
        // reads no flag. A `Return` or `Ret` would mean a `Phi` at EXIT,
        // which can't be.
        --insert_pos;
      }
      pred_code.insert(pred_code.begin() + static_cast<std::ptrdiff_t>(insert_pos),
                       ordered.begin(), ordered.end());
    }
    auto& code = cfg.code[id];
    std::erase_if(code, [](const ir::Type_instruction& i) { return isa<ir::Phi>(i); });
  }
  return owned_cfg_to_function(fn, cfg);
}

}  // namespace c6502::passes
