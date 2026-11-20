// Writes the `@zp-link-meta` block that the optimized output carries
// after its EQU lines, and that the unoptimized text `compile.py -c`
// assembles carries too. The assembler keeps the block, as text, in the
// object's `.c6502.link` section, and the linker reads it to place the
// `__zpabi_*` and `__local_*` slots, the zero-page objects, and the
// items of every unit for the whole program (passes/zp_link_metadata.py
// describes each record and its format). The block is comment lines,
// so dasm assembles the text as if it weren't there.
#include "passes/zp_link_metadata.h"

#include <set>

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

}  // namespace c6502::passes
