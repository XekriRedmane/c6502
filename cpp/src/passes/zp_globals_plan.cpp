#include <algorithm>
#include <cstdint>
#include <map>
#include <optional>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "errors.h"
#include "passes/optimization/var_visit.h"
#include "passes/replace_pseudoregisters.h"
#include "passes/zp_globals.h"
#include "pyrepr.h"

namespace c6502::passes {

namespace {

namespace c99 = ::c6502::c99_ast;
namespace tac = ::c6502::tac_ast;
using asdl::Int;

constexpr Int kAutoHi = 0x80;

constexpr Int kMinWeightPerByte = 3;

CompileError zp_global_error(const std::string& message) {
  return CompileError("ZpGlobalError", message);
}

std::string hex2(Int v) { return "$" + pyrepr::hex(v, 2); }
std::string hex4(Int v) { return "$" + pyrepr::hex(v, 4); }

OrderedMap<std::string, Int> fixed_elsewhere(const SymbolTable& symbols, const TypeTable* types) {
  std::map<Int, std::string> used;
  OrderedMap<std::string, Int> out;
  for (const auto& [name, sym] : symbols) {
    const auto* attrs = std::get_if<StaticAttr>(&sym.attrs);
    if (attrs == nullptr || !attrs->address || *attrs->address < 0x100) {
      continue;
    }
    const Int addr = *attrs->address;
    const Int size = sizeof_type(sym.type, types);
    if (addr + size > 0x10000) {
      throw zp_global_error("object " + pyrepr::string(name) + " (" + pyrepr::integer(size) +
                            " bytes at " + hex4(addr) + ") runs past $FFFF");
    }
    for (Int b = addr; b < addr + size; ++b) {
      if (const auto it = used.find(b); it != used.end()) {
        throw zp_global_error("object " + pyrepr::string(name) + " at " + hex4(addr) +
                              " overlaps " + pyrepr::string(it->second) + " at byte " +
                              hex4(b));
      }
      used.emplace(b, name);
    }
    out.insert_or_assign(name, addr);
  }
  return out;
}

std::optional<Int> first_free(const std::set<Int>& taken, Int size) {
  for (Int addr = 0; addr < kAutoHi - size + 1; ++addr) {
    bool free = true;
    for (Int b = addr; b < addr + size; ++b) {
      if (taken.contains(b)) {
        free = false;
        break;
      }
    }
    if (free) {
      return addr;
    }
  }
  return std::nullopt;
}

double as_double(Int v) { return static_cast<double>(static_cast<std::int64_t>(v)); }

struct Candidate {
  std::string name;
  Int weight = 0;
  Int size = 0;
};

std::vector<Candidate> promotion_order(const SymbolTable& symbols, const TypeTable* types,
                                       const OrderedMap<std::string, Int>* weights) {
  std::vector<Candidate> out;
  if (weights == nullptr || weights->empty()) {
    return out;
  }
  for (const auto& [name, sym] : symbols) {
    const auto* attrs = std::get_if<StaticAttr>(&sym.attrs);
    if (attrs == nullptr || attrs->zeropage || attrs->address) {
      continue;
    }
    if (std::holds_alternative<NoInitializer>(attrs->initial_value)) {
      continue;
    }
    const c99::Type_data_type* t = &sym.type;
    while (const auto* v = asdl::as<c99::Volatile>(*t)) {
      t = &*v->referenced_type;
    }
    if (asdl::isa<c99::Const, c99::Array, c99::Structure, c99::Union, c99::FunType>(*t)) {
      continue;
    }
    const Int size = sizeof_type(sym.type, types);
    const Int* w = weights->find(name);
    const Int weight = w == nullptr ? 0 : *w;
    if (size == 0 || weight < kMinWeightPerByte * size) {
      continue;
    }
    out.push_back(Candidate{name, weight, size});
  }
  std::sort(out.begin(), out.end(), [](const Candidate& a, const Candidate& b) {
    const double ka = -(as_double(a.weight) / as_double(a.size));
    const double kb = -(as_double(b.weight) / as_double(b.size));
    if (ka != kb) {
      return ka < kb;
    }
    return a.name < b.name;
  });
  return out;
}

void names_in(const tac::Type_instruction& instr, std::vector<std::string_view>& found) {
  const auto add_vals = [&] {
    for (const tac::Type_val* v : vals_in(instr)) {
      if (const auto* var = std::get_if<tac::Var>(v)) {
        found.push_back(var->name);
      }
    }
  };
  switch (tac::kind(instr)) {
    case tac::Kind_instruction::IndexedLoad:
      found.push_back(std::get<tac::IndexedLoad>(instr).name);
      add_vals();
      return;
    case tac::Kind_instruction::IndexedSymbolStore:
      found.push_back(std::get<tac::IndexedSymbolStore>(instr).name);
      add_vals();
      return;
    case tac::Kind_instruction::FunctionCall:
      found.push_back(std::get<tac::FunctionCall>(instr).name);
      add_vals();
      return;
    case tac::Kind_instruction::Phi: {
      const auto& phi = std::get<tac::Phi>(instr);
      if (const auto* var = std::get_if<tac::Var>(&phi.dst)) {
        found.push_back(var->name);
      }
      for (const auto& arg : phi.args) {
        const auto& a = std::get<tac::PhiArg>(arg);
        found.push_back(a.pred_label);
        if (const auto* var = std::get_if<tac::Var>(&a.source)) {
          found.push_back(var->name);
        }
      }
      return;
    }
    default:
      add_vals();
      return;
  }
}

const std::string* jump_target(const tac::Type_instruction& instr) {
  switch (tac::kind(instr)) {
    case tac::Kind_instruction::Jump:
      return &std::get<tac::Jump>(instr).target;
    case tac::Kind_instruction::JumpIfTrue:
      return &std::get<tac::JumpIfTrue>(instr).target;
    case tac::Kind_instruction::JumpIfFalse:
      return &std::get<tac::JumpIfFalse>(instr).target;
    case tac::Kind_instruction::JumpIfCmp:
      return &std::get<tac::JumpIfCmp>(instr).target;
    case tac::Kind_instruction::JumpIfMasked:
      return &std::get<tac::JumpIfMasked>(instr).target;
    default:
      return nullptr;
  }
}

}  // namespace

ZpGlobals allocate_zp_globals(const SymbolTable& symbols, const TypeTable* types,
                              const HargsNeed& hargs_need,
                              const OrderedMap<std::string, Int>* weights) {
  struct Fixed {
    std::string name;
    Int addr = 0;
    Int size = 0;
  };
  std::vector<Fixed> fixed;
  std::vector<std::pair<std::string, Int>> autos;
  NameSet owned;
  OrderedMap<std::string, Int> sizes;
  OrderedMap<std::string, Int> external;
  OrderedMap<std::string, Int> elsewhere = fixed_elsewhere(symbols, types);
  std::set<Int> param_bytes;
  for (const auto& [name, sym] : symbols) {
    if (const auto* f = std::get_if<FunAttr>(&sym.attrs)) {
      if (f->address) {
        elsewhere.insert_or_assign(name, *f->address);
      }
      if (!f->param_zeropage.empty()) {
        const auto& params = std::get<c99::FunType>(sym.type).params;
        const std::size_t n = std::min(f->param_zeropage.size(), params.size());
        for (std::size_t k = 0; k < n; ++k) {
          if (const auto& addr = f->param_zeropage[k]) {
            const Int size = sizeof_type(params[k], types);
            for (Int b = *addr; b < *addr + size; ++b) {
              param_bytes.insert(b);
            }
          }
        }
      }
      continue;
    }
    const auto* attrs = std::get_if<StaticAttr>(&sym.attrs);
    if (attrs == nullptr || !attrs->zeropage) {
      continue;
    }
    const Int size = sizeof_type(sym.type, types);
    const bool defined = !std::holds_alternative<NoInitializer>(attrs->initial_value);
    if (defined) {
      owned.insert(name);
    }
    if (!attrs->zp_address) {
      if (!defined) {
        external.insert_or_assign(name, size);
        continue;
      }
      autos.emplace_back(name, size);
    } else {
      fixed.push_back(Fixed{name, *attrs->zp_address, size});
    }
    sizes.insert_or_assign(name, size);
  }

  std::map<Int, std::string> used;
  OrderedMap<std::string, Int> addrs;
  const auto claim = [&](const std::string& name, Int addr, Int size) {
    for (Int b = addr; b < addr + size; ++b) {
      if (const auto it = used.find(b); it != used.end()) {
        throw zp_global_error("zero-page object " + pyrepr::string(name) + " at " + hex2(addr) +
                              " overlaps " + pyrepr::string(it->second) + " at byte " +
                              hex2(b));
      }
    }
    for (Int b = addr; b < addr + size; ++b) {
      used.emplace(b, name);
    }
    addrs.insert_or_assign(name, addr);
  };

  for (const auto& f : fixed) {
    if (f.addr + f.size > 0x100) {
      throw zp_global_error("zero-page object " + pyrepr::string(f.name) + " (" +
                            pyrepr::integer(f.size) + " bytes at " + hex2(f.addr) +
                            ") runs past $FF");
    }
    claim(f.name, f.addr, f.size);
  }
  std::set<Int> fixed_bytes = param_bytes;
  for (const auto& [b, name] : used) {
    fixed_bytes.insert(b);
  }
  const RuntimeLayout layout =
      choose_layout(fixed_bytes, hargs_need ? hargs_need : HargsNeed([] {
        return RuntimeLayout{}.hargs_size;
      }));
  std::set<Int> taken = fixed_bytes;
  for (const Int b : layout.bytes()) {
    taken.insert(b);
  }
  for (const auto& [name, size] : autos) {
    const std::optional<Int> addr = first_free(taken, size);
    if (!addr) {
      throw zp_global_error("no room for zero-page object " + pyrepr::string(name) + " (" +
                            pyrepr::integer(size) + " bytes) below " + hex2(kAutoHi) +
                            ", after the runtime's block and the fixed zero-page objects");
    }
    claim(name, *addr, size);
    for (Int b = *addr; b < *addr + size; ++b) {
      taken.insert(b);
    }
  }
  OrderedMap<std::string, Int> promoted;
  for (const auto& c : promotion_order(symbols, types, weights)) {
    const std::optional<Int> addr = first_free(taken, c.size);
    if (!addr) {
      continue;
    }
    claim(c.name, *addr, c.size);
    for (Int b = *addr; b < *addr + c.size; ++b) {
      taken.insert(b);
    }
    owned.insert(c.name);
    sizes.insert_or_assign(c.name, c.size);
    promoted.insert_or_assign(c.name, c.weight);
  }
  ZpGlobals out;
  out.addrs = std::move(addrs);
  out.reserved = param_bytes;
  for (const auto& [b, name] : used) {
    out.reserved.insert(b);
  }
  out.owned = std::move(owned);
  out.fixed = std::move(elsewhere);
  out.layout = layout;
  out.sizes = std::move(sizes);
  for (const auto& f : fixed) {
    out.pinned.insert(f.name);
  }
  out.external = std::move(external);
  out.promoted = std::move(promoted);
  out.param_bytes = std::move(param_bytes);
  return out;
}

OrderedMap<std::string, Int> access_weights(const tac::Program& prog) {
  OrderedMap<std::string, Int> out;
  for (const auto& tl : prog.top_level) {
    const auto* fn = std::get_if<tac::Function>(&tl);
    if (fn == nullptr) {
      continue;
    }
    const auto& instrs = fn->instructions;
    std::map<std::string, std::size_t, std::less<>> label_at;
    for (std::size_t k = 0; k < instrs.size(); ++k) {
      if (const auto* label = asdl::as<tac::Label>(instrs[k])) {
        label_at[label->name] = k;
      }
    }
    std::vector<Int> depth(instrs.size() + 1, 0);
    for (std::size_t k = 0; k < instrs.size(); ++k) {
      const std::string* target = jump_target(instrs[k]);
      if (target == nullptr) {
        continue;
      }
      const auto it = label_at.find(*target);
      if (it != label_at.end() && it->second < k) {
        depth[it->second] += 1;
        depth[k + 1] -= 1;
      }
    }
    Int level = 0;
    std::vector<std::string_view> found;
    for (std::size_t k = 0; k < instrs.size(); ++k) {
      level += depth[k];
      if (asdl::isa<tac::Label, tac::GetAddress>(instrs[k])) {
        continue;
      }
      found.clear();
      names_in(instrs[k], found);
      const Int weight = level >= 3 ? 512 : level == 2 ? 64 : level == 1 ? 8 : 1;
      for (const auto name : found) {
        const std::string key(name);
        Int* w = out.find(key);
        if (w == nullptr) {
          out.insert_or_assign(key, weight);
        } else {
          *w += weight;
        }
      }
    }
  }
  return out;
}

ZpGlobals plan_zero_page(const tac::Program& prog, const SymbolTable& symbols,
                         const TypeTable* types, bool promote,
                         const HargsNeed& unoptimized_hargs_need) {
  if (promote) {
    const OrderedMap<std::string, Int> weights = access_weights(prog);
    return allocate_zp_globals(symbols, types, unoptimized_hargs_need, &weights);
  }
  return allocate_zp_globals(symbols, types, unoptimized_hargs_need, nullptr);
}

}  // namespace c6502::passes
