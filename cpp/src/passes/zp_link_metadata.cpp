#include "passes/zp_link_metadata.h"

#include <algorithm>
#include <map>
#include <set>

#include "errors.h"
#include "passes/call_edges.h"
#include "passes/recursion_saves.h"
#include "pcpp_text.h"
#include "py_int.h"
#include "pyrepr.h"

namespace c6502::passes {

namespace {

std::string join(const std::vector<std::string>& items) {
  std::string out;
  for (std::size_t i = 0; i < items.size(); ++i) {
    if (i != 0) {
      out += ',';
    }
    out += items[i];
  }
  return out;
}

bool any_set(const std::vector<std::string>& items) {
  for (const auto& s : items) {
    if (!s.empty()) {
      return true;
    }
  }
  return false;
}

bool any_pinned(const std::vector<std::optional<asdl::Int>>& pinned) {
  for (const auto& a : pinned) {
    if (a) {
      return true;
    }
  }
  return false;
}

std::string format_pinned(const std::vector<std::optional<asdl::Int>>& pinned) {
  std::string out;
  for (std::size_t i = 0; i < pinned.size(); ++i) {
    if (i != 0) {
      out += ',';
    }
    if (pinned[i]) {
      out += '$';
      out += pyrepr::hex(*pinned[i], 2);
    }
  }
  return out;
}

const char* boolean(bool b) { return b ? "true" : "false"; }

}  // namespace

std::vector<std::string> format_metadata(const LinkMetadata& meta) {
  std::vector<std::string> lines{"; @zp-link-meta-begin"};
  for (const auto& d : meta.defs) {
    std::string line = "; def " + d.name + " params=" + join(d.params) +
                       " locals=" + join(d.locals) + " indirect=" + boolean(d.indirect) +
                       " in_cycle=" + boolean(d.in_cycle);
    if (!d.param_regs.empty() && any_set(d.param_regs)) {
      line += " param_regs=" + join(d.param_regs);
    }
    if (any_pinned(d.pinned)) {
      line += " pinned=" + format_pinned(d.pinned);
    }
    if (!d.changes.empty()) {
      line += " changes=" + join(d.changes);
    }
    if (d.carry) {
      line += " carry=true";
    }
    lines.push_back(std::move(line));
  }
  for (const auto& e : meta.externs) {
    std::string line = "; ext " + e.name + " params=" + join(e.params);
    if (!e.param_regs.empty() && any_set(e.param_regs)) {
      line += " param_regs=" + join(e.param_regs);
    }
    if (any_pinned(e.pinned)) {
      line += " pinned=" + format_pinned(e.pinned);
    }
    if (e.carry) {
      line += " carry=true";
    }
    lines.push_back(std::move(line));
  }
  const std::set<std::pair<std::string, std::string>> saved(meta.saved_calls.begin(),
                                                             meta.saved_calls.end());
  for (const auto& call : meta.calls) {
    lines.push_back("; call " + call.first + " -> " + call.second +
                    (saved.contains(call) ? " saved" : ""));
  }
  for (const auto& share : meta.shares) {
    std::vector<std::string> tokens;
    for (const auto& [f, syms] : share.members) {
      tokens.push_back("member=" + f + ":" + join(syms));
    }
    for (const auto& h : share.holders) {
      tokens.push_back("holder=" + h);
    }
    for (const auto& [f, callees] : share.live) {
      tokens.push_back("live=" + f + ":" + join(callees));
    }
    for (const auto& [a, b] : share.forwards) {
      tokens.push_back("forward=" + a + ">" + b);
    }
    for (const auto& w : share.writers) {
      tokens.push_back("writer=" + w);
    }
    std::string line = "; share ";
    for (std::size_t i = 0; i < tokens.size(); ++i) {
      if (i != 0) {
        line += ' ';
      }
      line += tokens[i];
    }
    lines.push_back(std::move(line));
  }
  for (const auto& it : meta.items) {
    lines.push_back("; item " + it.name + " section=" + it.section + " local=" +
                    boolean(it.local) + " zero=" + boolean(it.zero) +
                    (it.merge ? " merge=true" : ""));
  }
  for (const auto& z : meta.zp_objects) {
    std::string at = "auto";
    if (z.at) {
      at = "$";
      at += pyrepr::hex(*z.at, 2);
    }
    lines.push_back("; zp " + z.name + " size=" + pyrepr::integer(z.size) + " at=" + at +
                    " def=" + boolean(z.defined) + " init=" + join(z.init) +
                    (z.weight ? " weight=" + pyrepr::integer(*z.weight) : "") +
                    (z.local ? " local=true" : ""));
  }
  if (meta.hargs_size) {
    lines.push_back("; runtime hargs_size=" + pyrepr::integer(*meta.hargs_size) +
                    (meta.hargs_need ? " hargs_need=" + pyrepr::integer(*meta.hargs_need) : ""));
  }
  lines.emplace_back("; @zp-link-meta-end");
  return lines;
}


LinkMetadata build_metadata(const tac_ast::Program& prog, const Abi& abi,
                            const OrderedMap<std::string, std::vector<asdl::Int>>& local_pools,
                            const OrderedMap<std::string, std::vector<std::string>>* slot_names_by_fn,
                            const OrderedMap<std::string, std::vector<std::string>>* changes,
                            const NameSet* carry) {
  namespace tac = ::c6502::tac_ast;
  CallGraph callgraph;
  NameSet indirect;
  std::map<std::pair<std::string, std::string>, bool> saved;
  for (const auto& tl : prog.top_level) {
    if (const auto* fn = std::get_if<tac::Function>(&tl)) {
      callgraph[fn->name];
    }
  }
  for (const auto& tl : prog.top_level) {
    const auto* fn = std::get_if<tac::Function>(&tl);
    if (fn == nullptr) {
      continue;
    }
    for (const auto& instr : fn->instructions) {
      if (asdl::isa<tac::IndirectCall>(instr)) {
        indirect.insert(fn->name);
      } else if (const auto* call = asdl::as<tac::FunctionCall>(instr)) {
        if (abandons_caller(instr)) {
          continue;
        }
        callgraph[fn->name].insert(call->name);
        const auto [it, fresh] = saved.try_emplace({fn->name, call->name}, true);
        it->second = it->second && call->hw_stack_save;
      }
    }
  }
  const Cycles in_cycle = cycles(callgraph);

  LinkMetadata out;
  const auto regs_of = [](const ZpLayout& layout) {
    std::vector<std::string> regs;
    for (const auto& r : layout.param_registers) {
      regs.push_back(r.value_or(""));
    }
    return regs;
  };
  for (const auto& [name, callees] : callgraph) {
    FunctionMeta meta;
    meta.name = name;
    const std::vector<std::string>* names =
        slot_names_by_fn == nullptr ? nullptr : slot_names_by_fn->find(name);
    if (names != nullptr) {
      meta.locals = *names;
    } else if (const auto* pool = local_pools.find(name)) {
      for (std::size_t k = 0; k < pool->size(); ++k) {
        meta.locals.push_back("__local_" + name + "__" + std::to_string(k));
      }
    }
    const ParamLayout* layout = abi.find(name);
    if (const ZpLayout* zp = layout == nullptr ? nullptr : std::get_if<ZpLayout>(layout)) {
      meta.params = zp->slot_symbols;
      meta.param_regs = regs_of(*zp);
      meta.pinned = zp->pinned;
    }
    meta.indirect = indirect.contains(name);
    meta.in_cycle = in_cycle.contains(name);
    if (changes != nullptr) {
      if (const auto* c = changes->find(name)) {
        meta.changes = *c;
      }
    }
    meta.carry = carry != nullptr && carry->contains(name);
    out.defs.push_back(std::move(meta));
  }
  std::vector<std::string> declared;
  for (const auto& [name, layout] : abi) {
    if (!callgraph.contains(name) && std::holds_alternative<ZpLayout>(layout)) {
      declared.push_back(name);
    }
  }
  std::sort(declared.begin(), declared.end());
  for (const auto& name : declared) {
    const auto& zp = std::get<ZpLayout>(abi.at(name));
    out.externs.push_back(ExternMeta{.name = name,
                                     .params = zp.slot_symbols,
                                     .param_regs = regs_of(zp),
                                     .pinned = zp.pinned,
                                     .carry = carry != nullptr && carry->contains(name)});
  }
  for (const auto& [caller, callees] : callgraph) {
    for (const auto& callee : callees) {
      out.calls.emplace_back(caller, callee);
      const auto it = saved.find({caller, callee});
      if (it != saved.end() && it->second) {
        out.saved_calls.emplace_back(caller, callee);
      }
    }
  }
  return out;
}

namespace {

constexpr std::string_view kBegin = "; @zp-link-meta-begin";
constexpr std::string_view kEnd = "; @zp-link-meta-end";

constexpr std::string_view kWhere = "the link metadata";

[[noreturn]] void value_error(const std::string& message) {
  throw CompileError("ValueError", message);
}

class Fields {
 public:
  Fields(const std::vector<std::string>& parts, std::size_t first) {
    for (std::size_t i = first; i < parts.size(); ++i) {
      auto [k, v] = split_kv(parts[i]);
      values_[k] = v;
    }
  }

  static std::pair<std::string, std::string> split_kv(const std::string& token) {
    const std::size_t eq = token.find('=');
    if (eq == std::string::npos) {
      value_error("expected key=value, got " + pcpp::py_repr(token));
    }
    return {token.substr(0, eq), token.substr(eq + 1)};
  }

  const std::string& at(const std::string& key) const {
    const auto it = values_.find(key);
    if (it == values_.end()) throw CompileError("KeyError", pcpp::py_repr(key));
    return it->second;
  }

  std::string get(const std::string& key) const {
    const auto it = values_.find(key);
    return it == values_.end() ? std::string() : it->second;
  }
  bool has(const std::string& key) const { return values_.contains(key); }
  bool is_true(const std::string& key) const { return has(key) && get(key) == "true"; }

 private:
  std::map<std::string, std::string> values_;
};

std::vector<std::string> split_csv(const std::string& value) {
  std::vector<std::string> out;
  if (value.empty()) return out;
  std::size_t start = 0;
  while (true) {
    const std::size_t comma = value.find(',', start);
    if (comma == std::string::npos) {
      out.push_back(value.substr(start));
      return out;
    }
    out.push_back(value.substr(start, comma - start));
    start = comma + 1;
  }
}

std::string lstrip_dollars(const std::string& value) {
  return value.substr(std::min(value.find_first_not_of('$'), value.size()));
}

std::vector<std::optional<asdl::Int>> parse_pinned(const std::string& value) {
  std::vector<std::optional<asdl::Int>> out;
  if (value.empty()) return out;
  for (const auto& v : split_csv(value)) {
    if (v.empty()) {
      out.emplace_back(std::nullopt);
    } else {
      out.emplace_back(py_int::parse(lstrip_dollars(v), 16, kWhere));
    }
  }
  return out;
}

std::pair<std::string, std::string> partition(const std::string& value, char sep) {
  const std::size_t at = value.find(sep);
  if (at == std::string::npos) return {value, ""};
  return {value.substr(0, at), value.substr(at + 1)};
}

void parse_record(const std::string& record, LinkMetadata& out) {
  const std::vector<std::string> parts = pcpp::split_whitespace(record);
  if (parts.empty()) return;
  const std::string& kind = parts[0];
  const std::string shown = pcpp::py_repr(record);
  if (kind == "def") {
    if (parts.size() < 6) value_error("malformed def record: " + shown);
    const Fields kvs(parts, 2);
    FunctionMeta d;
    d.name = parts[1];
    d.params = split_csv(kvs.at("params"));
    d.locals = split_csv(kvs.at("locals"));
    d.indirect = kvs.at("indirect") == "true";
    d.in_cycle = kvs.at("in_cycle") == "true";
    d.param_regs = split_csv(kvs.get("param_regs"));
    d.pinned = parse_pinned(kvs.get("pinned"));
    d.changes = split_csv(kvs.get("changes"));
    d.carry = kvs.is_true("carry");
    out.defs.push_back(std::move(d));
  } else if (kind == "ext") {
    if (parts.size() < 3) value_error("malformed ext record: " + shown);
    const Fields kvs(parts, 2);
    ExternMeta e;
    e.name = parts[1];
    e.params = split_csv(kvs.at("params"));
    e.param_regs = split_csv(kvs.get("param_regs"));
    e.pinned = parse_pinned(kvs.get("pinned"));
    e.carry = kvs.is_true("carry");
    out.externs.push_back(std::move(e));
  } else if (kind == "share") {
    SlotShare share;
    for (std::size_t i = 1; i < parts.size(); ++i) {
      const auto [key, value] = Fields::split_kv(parts[i]);
      if (key == "member") {
        auto [fn, syms] = partition(value, ':');
        share.members.emplace_back(fn, split_csv(syms));
      } else if (key == "holder") {
        share.holders.push_back(value);
      } else if (key == "live") {
        auto [fn, callees] = partition(value, ':');
        share.live.emplace_back(fn, split_csv(callees));
      } else if (key == "forward") {
        share.forwards.push_back(partition(value, '>'));
      } else if (key == "writer") {
        share.writers.push_back(value);
      } else {
        value_error("malformed share record: " + shown);
      }
    }
    if (share.members.empty()) value_error("malformed share record: " + shown);
    out.shares.push_back(std::move(share));
  } else if (kind == "zp") {
    if (parts.size() < 3) value_error("malformed zp record: " + shown);
    const Fields kvs(parts, 2);
    const std::string& at = kvs.at("at");
    ZpObjectMeta z;
    z.name = parts[1];
    z.size = py_int::parse(kvs.at("size"), 10, kWhere);
    if (at != "auto") z.at = py_int::parse(lstrip_dollars(at), 16, kWhere);
    z.defined = kvs.at("def") == "true";
    z.init = split_csv(kvs.get("init"));
    if (kvs.has("weight")) z.weight = py_int::parse(kvs.get("weight"), 10, kWhere);
    z.local = kvs.is_true("local");
    out.zp_objects.push_back(std::move(z));
  } else if (kind == "item") {
    if (parts.size() < 3) value_error("malformed item record: " + shown);
    const Fields kvs(parts, 2);
    out.items.push_back(ItemMeta{.name = parts[1],
                                 .section = kvs.at("section"),
                                 .local = kvs.is_true("local"),
                                 .zero = kvs.is_true("zero"),
                                 .merge = kvs.is_true("merge")});
  } else if (kind == "runtime") {
    const Fields kvs(parts, 1);
    out.hargs_size = py_int::parse(kvs.at("hargs_size"), 10, kWhere);
    if (kvs.has("hargs_need")) out.hargs_need = py_int::parse(kvs.get("hargs_need"), 10, kWhere);
  } else if (kind == "call") {
    if ((parts.size() != 4 && parts.size() != 5) || parts[2] != "->" ||
        (parts.size() == 5 && parts[4] != "saved")) {
      value_error("malformed call record: " + shown);
    }
    out.calls.emplace_back(parts[1], parts[3]);
    if (parts.size() == 5) out.saved_calls.emplace_back(parts[1], parts[3]);
  } else {
    value_error("unknown record kind " + pcpp::py_repr(kind) + ": " + shown);
  }
}

}  // namespace

LinkMetadata parse_metadata(std::string_view asm_text) {
  LinkMetadata out;
  bool in_block = false;
  for (const auto& raw : pcpp::splitlines(asm_text)) {
    const std::string line = pcpp::strip(raw);
    if (line == kBegin) {
      if (in_block) {
        value_error("nested @zp-link-meta-begin without preceding @zp-link-meta-end");
      }
      in_block = true;
      continue;
    }
    if (line == kEnd) {
      if (!in_block) value_error("@zp-link-meta-end without preceding @zp-link-meta-begin");
      in_block = false;
      continue;
    }
    if (!in_block) continue;
    if (line.rfind("; ", 0) != 0) {
      value_error("unexpected line inside zp-link-meta block: " + pcpp::py_repr(line));
    }
    parse_record(pcpp::strip(std::string_view(line).substr(2)), out);
  }
  if (in_block) value_error("@zp-link-meta-begin without matching @zp-link-meta-end");
  return out;
}

}  // namespace c6502::passes
