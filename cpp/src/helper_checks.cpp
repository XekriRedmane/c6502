#include "helper_checks.h"

#include <algorithm>
#include <functional>
#include <map>
#include <memory>

#include "capture.h"
#include "errors.h"
#include "passes/asm_aliasing.h"
#include "passes/asm_dead_store.h"
#include "passes/asm_liveness.h"
#include "passes/call_edges.h"
#include "passes/dead_a_arith.h"
#include "passes/decimal_arith.h"
#include "passes/inc_peephole.h"
#include "passes/optimization_asm/cfg.h"
#include "passes/redundant_load.h"
#include "sim/assembler.h"

namespace c6502::helper_checks {

namespace {

namespace ir = ::c6502::asm_ast;
namespace tac = ::c6502::tac_ast;
using ::c6502::asdl::as;
using ::c6502::asdl::Int;
using ::c6502::asdl::isa;
using namespace ::c6502::passes;

std::string bits(const std::vector<bool>& xs) {
  if (xs.empty()) {
    return "-";
  }
  std::string out;
  out.reserve(xs.size());
  for (const bool x : xs) {
    out += x ? '1' : '0';
  }
  return out;
}

template <class T>
std::string ints(const T& xs) {
  std::string out;
  for (const auto& x : xs) {
    if (!out.empty()) {
      out += ',';
    }
    if constexpr (std::is_same_v<std::remove_cvref_t<decltype(x)>, Int>) {
      out += asdl::int_to_string(x);
    } else {
      out += std::to_string(x);
    }
  }
  return out.empty() ? "-" : out;
}

std::string join(const std::vector<std::string>& xs) {
  std::string out;
  for (const auto& x : xs) {
    if (!out.empty()) {
      out += ' ';
    }
    out += x;
  }
  return out.empty() ? "-" : out;
}

std::string names(const NameSet& xs) {
  return join(std::vector<std::string>(xs.begin(), xs.end()));
}

std::string dump1(const ir::Type_operand& op) {
  std::string d = asdl::dump(op);
  d.pop_back();
  return d;
}

// The distinct operands of a function, as `function_operands` gives
// them, with each one's index by its dump.
struct Operands {
  std::vector<ir::Type_operand> ops;
  std::map<std::string, std::size_t> index;

  void add(const ir::Type_operand& op) {
    std::string d = dump1(op);
    if (index.emplace(std::move(d), ops.size()).second) {
      ops.push_back(op);
    }
  }
  std::size_t of(const ir::Type_operand& op) const { return index.at(dump1(op)); }
};

Operands function_operands(Instrs instrs) {
  Operands out;
  for (const auto& ins : instrs) {
    for (const auto& op : instruction_operands(ins)) {
      out.add(op);
    }
  }
  for (const auto& ins : instrs) {
    for (const auto& op : asm_dead_store::read_operands(ins)) {
      out.add(op);
    }
  }
  const std::vector<ir::Type_operand> first(out.ops);
  for (const auto& op : first) {
    for (const auto& p : asm_dead_store::ptr_source_reads(op)) {
      out.add(p);
    }
  }
  return out;
}

const std::string* name_of(const ir::Type_operand& op) {
  return std::visit(
      [](const auto& o) -> const std::string* {
        if constexpr (requires { o.name; }) {
          return &o.name;
        } else {
          return nullptr;
        }
      },
      op);
}

std::string cfg_text(const std::function<Cfg()>& build) {
  Cfg cfg;
  try {
    cfg = build();
  } catch (const InternalError&) {
    return "E";
  }
  std::string blocks;
  for (const auto& b : cfg.blocks) {
    if (!blocks.empty()) {
      blocks += ';';
    }
    blocks += std::to_string(b.id) + ":" + std::to_string(b.begin) + ":" +
              std::to_string(b.end) + ":" + ints(b.predecessors) + ":" +
              ints(b.successors);
  }
  const std::vector<int> rpo = reverse_postorder(cfg);
  const Dominators doms = immediate_dominators(cfg);
  const auto children = dominator_tree_children(doms);
  const auto df = dominance_frontiers(cfg);
  std::string idom_text;
  std::string children_text;
  std::string df_text;
  for (const int b : doms.order) {
    if (!idom_text.empty()) {
      idom_text += ',';
      children_text += ';';
      df_text += ';';
    }
    idom_text += std::to_string(b) + ">" + std::to_string(doms.idom[b]);
    children_text += std::to_string(b) + ":" + ints(children[b]);
    df_text += std::to_string(b) + ":" + ints(df[b]);
  }
  return ints(cfg.block_order) + "|" + blocks + "|" + ints(rpo) + "|" + idom_text +
         "|" + children_text + "|" + df_text;
}

// The `Data` and `ZP` operands of `instrs[i]` and `instrs[i + 1]`, each
// once, in order (`mem_dead_targets`).
std::vector<ir::Type_operand> mem_dead_targets(Instrs instrs, std::size_t i) {
  std::vector<ir::Type_operand> out;
  std::set<std::string> seen;
  for (std::size_t k = i; k < instrs.size() && k < i + 2; ++k) {
    for (const auto& op : instruction_operands(instrs[k])) {
      if (isa<ir::Data, ir::ZP>(op) && seen.insert(dump1(op)).second) {
        out.push_back(op);
      }
    }
  }
  return out;
}

std::string pairs(const SymbolAddrs& m) {
  std::vector<std::string> out;
  for (const auto& [k, v] : m) {
    out.push_back(k + "=" + asdl::int_to_string(v));
  }
  return join(out);
}

std::string sizes(Instrs instrs, const NameSet* zp_names) {
  std::vector<std::string> out;
  for (const auto& ins : instrs) {
    try {
      out.push_back(std::to_string(sim::instruction_size(ins, zp_names)));
    } catch (const InternalError&) {
      out.push_back("E");
    }
  }
  return join(out);
}

}  // namespace

HelperContext parse_context(const json::Value& v) {
  HelperContext out;
  out.ctx = capture::context_from_json(v);
  if (const json::Value* slots = v.get("zp_slot_symbols");
      slots != nullptr && slots->kind != json::Value::Kind::kNull) {
    SymbolAddrs m;
    for (const auto& pair : slots->items) {
      m[pair.items[0].string] = pair.items[1].integer;
    }
    out.zp_slot_symbols = std::move(m);
  }
  if (const json::Value* zp_names = v.get("zp_names");
      zp_names != nullptr && zp_names->kind != json::Value::Kind::kNull) {
    NameSet s;
    for (const auto& n : zp_names->items) {
      s.insert(n.string);
    }
    out.zp_names = std::move(s);
  }
  for (const auto& a : v.get("slot_addrs")->items) {
    out.slot_addrs.insert(a.integer);
  }
  for (const auto& n : v.get("pointer_proof")->items) {
    out.pointer_proof.insert(n.string);
  }
  return out;
}

Values function_values(const ir::Function& fn, const HelperContext& hctx) {
  const Instrs instrs = fn.instructions;
  const std::size_t n = instrs.size();
  const AsmContext& ctx = hctx.ctx;
  const AsmContext proof_ctx = ctx.with_pointer_proof(hctx.pointer_proof);
  const SymbolAddrs* slots = hctx.zp_slot_symbols ? &*hctx.zp_slot_symbols : nullptr;
  const LabelMap labels(instrs);
  Values out;

  // asm_liveness.
  using Walk = bool (*)(Instrs, const LabelMap&, std::size_t);
  const std::pair<const char*, Walk> walks[] = {
      {"a_dead_at", a_dead_at},         {"x_dead_at", x_dead_at},
      {"y_dead_at", y_dead_at},         {"flags_dead_at", flags_dead_at},
      {"all_flags_dead_at", all_flags_dead_at}, {"c_dead_at", c_dead_at},
      {"v_dead_at", v_dead_at},
  };
  for (const auto& [name, walk] : walks) {
    std::vector<bool> xs;
    for (std::size_t i = 0; i <= n; ++i) {
      xs.push_back(walk(instrs, labels, i));
    }
    out.emplace_back(name, bits(xs));
  }
  using Pred = bool (*)(const ir::Type_instruction&);
  const std::pair<const char*, Pred> preds[] = {
      {"reads_a", reads_a},       {"kills_a", kills_a},     {"reads_x", reads_x},
      {"kills_x", kills_x},       {"reads_y", reads_y},     {"kills_y", kills_y},
      {"sets_up_y", sets_up_y},   {"sets_flags", sets_flags},
      {"returns_carry", returns_carry},
  };
  const auto per_instr = [&](const auto& f) {
    std::vector<bool> xs;
    for (const auto& ins : instrs) {
      xs.push_back(f(ins));
    }
    return bits(xs);
  };
  for (const auto& [name, pred] : preds) {
    out.emplace_back(name, per_instr(pred));
  }
  out.emplace_back("call_preserves_x", per_instr([](const ir::Type_instruction& i) {
                     return call_preserves(i, "X");
                   }));
  out.emplace_back("call_preserves_y", per_instr([](const ir::Type_instruction& i) {
                     return call_preserves(i, "Y");
                   }));
  {
    std::vector<std::string> entries;
    for (const auto& [name, idx] : labels.entries()) {
      entries.push_back(name + "=" + std::to_string(idx));
    }
    out.emplace_back("label_map", join(entries));
  }

  // The operands.
  const Operands o = function_operands(instrs);
  const auto& ops = o.ops;
  out.emplace_back("operands", std::to_string(ops.size()));
  {
    std::vector<std::string> a;
    std::vector<std::string> r;
    std::vector<std::string> w;
    for (const auto& ins : instrs) {
      std::vector<std::size_t> xs;
      for (const auto& op : instruction_operands(ins)) {
        xs.push_back(o.of(op));
      }
      a.push_back(ints(xs));
      xs.clear();
      for (const auto& op : asm_dead_store::read_operands(ins)) {
        xs.push_back(o.of(op));
      }
      r.push_back(ints(xs));
      const auto wo = asm_dead_store::write_operand(ins);
      w.push_back(wo ? std::to_string(o.of(*wo)) : "-");
    }
    out.emplace_back("instruction_operands", join(a));
    out.emplace_back("read_operands", join(r));
    out.emplace_back("write_operand", join(w));
    std::vector<std::string> p;
    for (const auto& op : ops) {
      std::vector<std::size_t> xs;
      for (const auto& q : asm_dead_store::ptr_source_reads(op)) {
        xs.push_back(o.of(q));
      }
      p.push_back(ints(xs));
    }
    out.emplace_back("ptr_source_reads", join(p));
  }

  // asm_aliasing.
  {
    std::string zs;
    std::string ug;
    std::string ub;
    for (const auto& op : ops) {
      const std::string* name = name_of(op);
      zs += name == nullptr ? '-' : is_zp_symbol(*name) ? '1' : '0';
      ug += name == nullptr ? '-' : ctx.is_user_zp_global(*name) ? '1' : '0';
      const auto* zp = as<ir::ZP>(op);
      ub += zp == nullptr ? '-' : ctx.is_user_zp_byte(zp->address + zp->offset) ? '1' : '0';
    }
    out.emplace_back("is_zp_symbol", zs.empty() ? "-" : zs);
    out.emplace_back("is_user_zp_global", ug.empty() ? "-" : ug);
    out.emplace_back("is_user_zp_byte", ub.empty() ? "-" : ub);
  }
  const auto matrix = [&](const auto& f) {
    std::vector<bool> xs;
    xs.reserve(ops.size() * ops.size());
    for (const auto& a : ops) {
      for (const auto& b : ops) {
        xs.push_back(f(a, b));
      }
    }
    return bits(xs);
  };
  const MayAliasOptions with_slots{.zp_slot_symbols = slots};
  out.emplace_back("may_alias", matrix([&](const auto& a, const auto& b) {
                     return may_alias(a, b, ctx);
                   }));
  out.emplace_back("may_alias_slots", matrix([&](const auto& a, const auto& b) {
                     return may_alias(a, b, ctx, with_slots);
                   }));
  out.emplace_back("may_alias_proof", matrix([&](const auto& a, const auto& b) {
                     return may_alias(a, b, proof_ctx, with_slots);
                   }));
  const std::set<Int> pba = pointer_byte_addresses(instrs);
  out.emplace_back("pointer_byte_addresses", ints(pba));
  {
    std::vector<bool> xs;
    for (const auto& op : ops) {
      xs.push_back(is_pointer_byte(op, pba, slots));
    }
    out.emplace_back("is_pointer_byte", bits(xs));
  }
  out.emplace_back("through_slot_pointer", per_instr([&](const ir::Type_instruction& i) {
                     return through_slot_pointer(i, hctx.slot_addrs);
                   }));
  {
    const FunctionNames fnames = function_names(instrs);
    out.emplace_back("function_names_called", names(fnames.called));
    out.emplace_back("function_names_taken", names(fnames.taken));
    out.emplace_back("function_names_seen", names(fnames.seen));
  }
  out.emplace_back("runtime_zp_addrs", pairs(runtime_zp_addrs(ctx)));

  // decimal_arith.
  {
    std::vector<std::string> xs;
    for (std::size_t i = 0; i < n; ++i) {
      const auto* d = as<ir::DecimalArith>(instrs[i]);
      if (d == nullptr) {
        continue;
      }
      const auto idx = [&](const std::vector<ir::Type_operand>& v) {
        std::vector<std::size_t> r;
        for (const auto& op : v) {
          r.push_back(o.of(op));
        }
        return ints(r);
      };
      xs.push_back(std::to_string(i) + ":" + idx(decimal_arith::reads(*d)) + ":" +
                   idx(decimal_arith::writes(*d)) + ":" +
                   idx(decimal_arith::operands(*d)) + ":" +
                   (decimal_arith::writes_y(*d) ? "1" : "0"));
    }
    out.emplace_back("decimal_arith", join(xs));
  }

  // asm_dead_store.
  const NameSet taken = asm_dead_store::collect_address_taken_names(instrs);
  const std::string prefix = "__local_" + fn.name + "__";
  out.emplace_back("collect_address_taken_names", names(taken));
  {
    std::vector<std::string> xs;
    for (std::size_t i = 0; i < n; ++i) {
      xs.push_back(ints(asm_dead_store::successors(instrs, i, labels)));
    }
    out.emplace_back("successors", join(xs));
  }
  out.emplace_back("is_opaque", per_instr(asm_dead_store::is_opaque));
  std::vector<ir::Type_operand> targets;
  for (const auto& op : ops) {
    if (isa<ir::Data, ir::ZP>(op)) {
      targets.push_back(op);
    }
  }
  const auto by_target = [&](const auto& f) {
    std::vector<bool> xs;
    for (const auto& ins : instrs) {
      for (const auto& t : targets) {
        xs.push_back(f(ins, t));
      }
    }
    return bits(xs);
  };
  out.emplace_back("reads", by_target([&](const auto& ins, const auto& t) {
                     return asm_dead_store::reads(ins, t, ctx, slots);
                   }));
  out.emplace_back("reads_private", by_target([&](const auto& ins, const auto& t) {
                     return asm_dead_store::reads(ins, t, ctx, slots, true);
                   }));
  out.emplace_back("reads_proof", by_target([&](const auto& ins, const auto& t) {
                     return asm_dead_store::reads(ins, t, proof_ctx, slots);
                   }));
  out.emplace_back("writes_same", by_target([&](const auto& ins, const auto& t) {
                     return asm_dead_store::writes_same(ins, t);
                   }));
  {
    std::vector<bool> cct;
    std::vector<bool> exit;
    std::vector<bool> tail;
    for (const auto& t : targets) {
      cct.push_back(asm_dead_store::call_cannot_touch(t, prefix, taken));
      exit.push_back(asm_dead_store::is_dead_at_exit(t, slots));
      tail.push_back(asm_dead_store::is_dead_at_tail_call(t, &taken));
    }
    out.emplace_back("call_cannot_touch", bits(cct));
    out.emplace_back("is_dead_at_exit", bits(exit));
    out.emplace_back("is_dead_at_tail_call", bits(tail));
  }
  for (const auto* c : {&ctx, &proof_ctx}) {
    std::vector<std::string> rows;
    for (std::size_t i = 0; i < n; ++i) {
      std::vector<bool> xs;
      for (const auto& t : mem_dead_targets(instrs, i)) {
        xs.push_back(
            asm_dead_store::mem_dead_after(instrs, labels, i, t, *c, fn.name, slots));
      }
      rows.push_back(bits(xs));
    }
    out.emplace_back(c == &ctx ? "mem_dead_after" : "mem_dead_after_proof",
                     join(rows));
  }

  // inc_peephole and dead_a_arith.
  {
    std::vector<bool> xi;
    std::vector<bool> el;
    for (const auto& op : ops) {
      xi.push_back(is_x_indexed(op));
      el.push_back(inc_peephole::is_inc_eligible_operand(op));
    }
    out.emplace_back("is_x_indexed", bits(xi));
    out.emplace_back("is_inc_eligible_operand", bits(el));
  }
  out.emplace_back("operands_equal", matrix([](const auto& a, const auto& b) {
                     return inc_peephole::operands_equal(a, b);
                   }));
  {
    std::vector<bool> xs;
    for (std::size_t i = 0; i < n; ++i) {
      xs.push_back(dead_a_arith::flags_reflect_src_reg(instrs, i));
    }
    out.emplace_back("flags_reflect_src_reg", bits(xs));
  }
  const std::pair<const char*, ir::Type_reg> regs[] = {
      {"writes_reg_setting_n_z_a", ir::A{}},
      {"writes_reg_setting_n_z_x", ir::X{}},
      {"writes_reg_setting_n_z_y", ir::Y{}},
  };
  for (const auto& entry : regs) {
    // Clang 14 doesn't let a lambda capture a structured binding.
    const ir::Type_reg& reg = entry.second;
    out.emplace_back(entry.first, per_instr([&](const ir::Type_instruction& i) {
                       return dead_a_arith::writes_reg_setting_n_z(i, reg);
                     }));
  }

  // cfg and redundant_load's tolerant builder.
  out.emplace_back("build_cfg", cfg_text([&] { return build_cfg(fn); }));
  out.emplace_back("build_cfg_tolerant", cfg_text([&] { return build_cfg_tolerant(fn); }));

  // sim.assembler.
  out.emplace_back("instruction_size", sizes(instrs, nullptr));
  if (hctx.zp_names) {
    out.emplace_back("instruction_size_zp", sizes(instrs, &*hctx.zp_names));
  }
  return out;
}

Values program_values(const ir::Program& prog) {
  return {{"unreachable_by_pointer", names(unreachable_by_pointer(prog))}};
}

Values tac_values(const tac::Function& fn) {
  std::vector<bool> ab;
  for (const auto& i : fn.instructions) {
    ab.push_back(abandons_caller(i));
  }
  const auto calls = graph_calls(fn);
  std::vector<std::size_t> idx;
  std::size_t c = 0;
  for (std::size_t k = 0; k < fn.instructions.size() && c < calls.size(); ++k) {
    if (as<tac::FunctionCall>(fn.instructions[k]) == calls[c]) {
      idx.push_back(k);
      ++c;
    }
  }
  return {{"abandons_caller", bits(ab)}, {"graph_calls", ints(idx)}};
}

std::size_t value_count(const std::string& name, const std::string& value) {
  // The helpers whose values are lists, one value per field. A CFG is
  // one value, and so is the count of operands.
  static const std::set<std::string> kLists = {
      "instruction_operands", "read_operands", "write_operand",
      "ptr_source_reads", "successors", "instruction_size",
      "instruction_size_zp", "label_map", "function_names_called",
      "function_names_taken", "function_names_seen",
      "collect_address_taken_names", "runtime_zp_addrs",
      "pointer_byte_addresses", "decimal_arith", "graph_calls",
      "unreachable_by_pointer",
  };
  if (name == "operands" || name == "build_cfg" || name == "build_cfg_tolerant") {
    return 1;
  }
  if (kLists.count(name) != 0) {
    return static_cast<std::size_t>(std::count(value.begin(), value.end(), ' ')) + 1;
  }
  return static_cast<std::size_t>(std::count(value.begin(), value.end(), '0') +
                                  std::count(value.begin(), value.end(), '1'));
}

}  // namespace c6502::helper_checks
