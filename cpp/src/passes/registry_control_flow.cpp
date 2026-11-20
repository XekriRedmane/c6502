#include "passes/registry_control_flow.h"

#include <cstdio>
#include <optional>
#include <set>
#include <stdexcept>
#include <utility>
#include <vector>

#include "passes/branch_invert.h"
#include "passes/capture_args.h"
#include "passes/branch_thread.h"
#include "passes/branch_through_jump.h"
#include "passes/branch_to_next_drop.h"
#include "passes/branch_value_thread.h"
#include "passes/copy_through_index.h"
#include "passes/cross_jump.h"
#include "passes/dead_label_drop.h"
#include "passes/jump_table.h"
#include "passes/long_branches.h"
#include "passes/outline_runs.h"
#include "passes/ret_slot_coalescing.h"
#include "passes/shared_tails.h"
#include "passes/short_jumps.h"
#include "passes/slot_coalescing.h"
#include "passes/tail_call.h"
#include "passes/tail_merge.h"
#include "passes/unreachable_drop.h"

namespace c6502::passes::registry {

namespace {

using json::Value;

// Returns the keyword argument `name`, or null when it is missing or
// None.
const Value* kwarg(const Value& kwargs, std::string_view name) {
  const Value* v = kwargs.get(name);
  return v == nullptr || v->kind == Value::Kind::kNull ? nullptr : v;
}

// Returns the positional argument `i`.
const Value& arg(const Value& args, std::size_t i) {
  if (i >= args.items.size()) {
    throw std::runtime_error("a positional argument is missing");
  }
  return args.items[i];
}

SymbolAddrs symbol_addrs(const Value& v) { return capture_args::addrs_of(v); }

NameSet name_set(const Value& v) { return capture_args::names_of(v); }

std::set<asdl::Int> int_set(const Value& v) { return capture_args::ints_of(v); }

std::string program_json(const asm_ast::Program& prog) {
  std::string out = R"({"asdl": "asm_ast", "dump": )";
  out += json_quote(asdl::dump(prog));
  out += '}';
  return out;
}

}  // namespace

std::string json_quote(std::string_view s) {
  std::string out = "\"";
  const auto escape = [&](unsigned cp) {
    char buf[8];
    std::snprintf(buf, sizeof buf, "\\u%04x", cp);
    out += buf;
  };
  for (std::size_t i = 0; i < s.size();) {
    const auto c = static_cast<unsigned char>(s[i]);
    unsigned cp = c;
    std::size_t len = 1;
    if (c >= 0xF0 && i + 3 < s.size()) {
      cp = ((c & 0x07u) << 18) | ((static_cast<unsigned char>(s[i + 1]) & 0x3Fu) << 12) |
           ((static_cast<unsigned char>(s[i + 2]) & 0x3Fu) << 6) |
           (static_cast<unsigned char>(s[i + 3]) & 0x3Fu);
      len = 4;
    } else if (c >= 0xE0 && i + 2 < s.size()) {
      cp = ((c & 0x0Fu) << 12) | ((static_cast<unsigned char>(s[i + 1]) & 0x3Fu) << 6) |
           (static_cast<unsigned char>(s[i + 2]) & 0x3Fu);
      len = 3;
    } else if (c >= 0xC0 && i + 1 < s.size()) {
      cp = ((c & 0x1Fu) << 6) | (static_cast<unsigned char>(s[i + 1]) & 0x3Fu);
      len = 2;
    }
    i += len;
    switch (cp) {
      case '"':
        out += "\\\"";
        continue;
      case '\\':
        out += "\\\\";
        continue;
      case '\n':
        out += "\\n";
        continue;
      case '\r':
        out += "\\r";
        continue;
      case '\t':
        out += "\\t";
        continue;
      case '\b':
        out += "\\b";
        continue;
      case '\f':
        out += "\\f";
        continue;
      default:
        break;
    }
    if (cp >= 0x20 && cp < 0x7F) {
      out += static_cast<char>(cp);
    } else if (cp > 0xFFFF) {
      // Python writes a character outside the BMP as a surrogate pair.
      const unsigned v = cp - 0x10000;
      escape(0xD800 | (v >> 10));
      escape(0xDC00 | (v & 0x3FF));
    } else {
      escape(cp);
    }
  }
  out += '"';
  return out;
}

std::string run_dead_label_drop(asm_ast::Program input, const AsmContext&,
                                const json::Value&, const json::Value&) {
  return asdl::dump(apply_dead_label_drop(std::move(input)));
}

std::string run_unreachable_drop(asm_ast::Program input, const AsmContext&,
                                 const json::Value&, const json::Value&) {
  return asdl::dump(apply_unreachable_drop(std::move(input)));
}

std::string run_branch_invert(asm_ast::Program input, const AsmContext&,
                              const json::Value&, const json::Value&) {
  return asdl::dump(apply_branch_invert(std::move(input)));
}

std::string run_branch_to_next_drop(asm_ast::Program input, const AsmContext&,
                                    const json::Value&, const json::Value&) {
  return asdl::dump(apply_branch_to_next_drop(std::move(input)));
}

std::string run_branch_through_jump(asm_ast::Program input, const AsmContext&,
                                    const json::Value&, const json::Value&) {
  return asdl::dump(apply_branch_through_jump(std::move(input)));
}

std::string run_branch_thread(asm_ast::Program input, const AsmContext&,
                              const json::Value&, const json::Value&) {
  return asdl::dump(apply_branch_thread(std::move(input)));
}

std::string run_branch_value_thread(asm_ast::Program input, const AsmContext& ctx,
                                    const json::Value&, const json::Value&) {
  return asdl::dump(apply_branch_value_thread(std::move(input), ctx));
}

std::string run_tail_call(asm_ast::Program input, const AsmContext&,
                          const json::Value&, const json::Value&) {
  return asdl::dump(apply_tail_call(std::move(input)));
}

std::string run_tail_merge(asm_ast::Program input, const AsmContext&,
                           const json::Value&, const json::Value&) {
  return asdl::dump(apply_tail_merge(std::move(input)));
}

std::string run_cross_jump(asm_ast::Program input, const AsmContext&,
                           const json::Value&, const json::Value&) {
  return asdl::dump(apply_cross_jump(std::move(input)));
}

std::string run_slot_coalescing(asm_ast::Program input, const AsmContext&,
                                const json::Value& args, const json::Value&) {
  const AbiLayouts abi = capture_args::abi_layouts(arg(args, 0));
  const std::vector<SlotShare> shares = capture_args::slot_shares(arg(args, 1));
  const SymbolAddrs slot_symbols = symbol_addrs(arg(args, 2));
  auto [prog, written] =
      apply_slot_coalescing(std::move(input), abi, shares, slot_symbols);
  std::string out = R"({"tuple": [)";
  out += program_json(prog);
  out += R"(, {"dict": [)";
  for (std::size_t i = 0; i < written.size(); ++i) {
    if (i != 0) {
      out += ", ";
    }
    out += '[';
    out += json_quote(written[i].first);
    out += ", [";
    for (std::size_t j = 0; j < written[i].second.size(); ++j) {
      if (j != 0) {
        out += ", ";
      }
      out += json_quote(written[i].second[j]);
    }
    out += "]]";
  }
  out += "]}]}\n";
  return out;
}

std::string run_ret_slot_coalescing(asm_ast::Program input, const AsmContext&,
                                    const json::Value&, const json::Value& kwargs) {
  const Value* slots = kwarg(kwargs, "slot_symbols");
  const Value* zp = kwarg(kwargs, "zp_addrs");
  const Value* eligible = kwarg(kwargs, "eligible");
  return asdl::dump(apply_ret_slot_coalescing(
      std::move(input), slots ? symbol_addrs(*slots) : SymbolAddrs{},
      zp ? symbol_addrs(*zp) : SymbolAddrs{},
      eligible ? name_set(*eligible) : NameSet{}));
}

std::string run_jump_tables(asm_ast::Program input, const AsmContext&,
                            const json::Value&, const json::Value&) {
  return asdl::dump(apply_jump_tables(std::move(input)));
}

std::string run_copy_through_index(asm_ast::Program input, const AsmContext& ctx,
                                   const json::Value&, const json::Value& kwargs) {
  std::optional<SymbolAddrs> slots;
  if (const Value* v = kwarg(kwargs, "zp_slot_symbols")) {
    slots = symbol_addrs(*v);
  }
  return asdl::dump(
      apply_copy_through_index(std::move(input), ctx, slots ? &*slots : nullptr));
}

std::string run_long_branches(asm_ast::Program input, const AsmContext&,
                              const json::Value&, const json::Value&) {
  return asdl::dump(expand_long_branches(std::move(input)));
}

std::string run_short_jumps(asm_ast::Program input, const AsmContext&,
                            const json::Value&, const json::Value&) {
  return asdl::dump(apply_short_jumps(std::move(input)));
}

namespace {

// Decodes the keyword arguments that `apply_shared_tails` and
// `apply_outline_runs` share. The symbol table, their positional
// argument, isn't read: the C++ passes report the functions they make
// in place of adding them to it.
struct SharingArgs {
  std::optional<NameSet> zp_names;
  std::set<asdl::Int> slot_addrs;
};

SharingArgs sharing_args(const Value& kwargs) {
  SharingArgs out;
  if (const Value* v = kwarg(kwargs, "zp_names")) {
    out.zp_names = name_set(*v);
  }
  if (const Value* v = kwarg(kwargs, "slot_addrs")) {
    out.slot_addrs = int_set(*v);
  }
  return out;
}

}  // namespace

std::string run_shared_tails(asm_ast::Program input, const AsmContext&,
                             const json::Value&, const json::Value& kwargs) {
  const SharingArgs a = sharing_args(kwargs);
  return asdl::dump(apply_shared_tails(std::move(input),
                                       a.zp_names ? &*a.zp_names : nullptr,
                                       a.slot_addrs));
}

std::string run_outline_runs(asm_ast::Program input, const AsmContext&,
                             const json::Value&, const json::Value& kwargs) {
  const SharingArgs a = sharing_args(kwargs);
  return asdl::dump(apply_outline_runs(std::move(input),
                                       a.zp_names ? &*a.zp_names : nullptr,
                                       a.slot_addrs));
}

}  // namespace c6502::passes::registry
