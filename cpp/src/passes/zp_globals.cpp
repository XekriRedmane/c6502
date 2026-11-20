// The zero-page objects (`[[c6502::zeropage]]`) at the end of the back
// end.
//
// Zero page isn't part of the program image that is loaded, so a
// zero-page object's initial value (its initializer, or zero, as C gives
// static storage) is stored when the program runs. `place_zp_globals`
// drops each object's `StaticVariable` and adds a function `__zp_init`
// that stores the initial bytes of every zero-page object the unit
// defines, which the boot code calls before `main` (the simulator's boot
// stub does, in sim/runtime.py). An `extern` declaration with a fixed
// address and no definition in the unit names a byte that something
// else owns, another unit or hand-written code, so it is reserved and
// addressed but not initialized.
//
// Under `-O` each unit lists its zero-page objects in the link metadata
// (`link_records`): the size, the fixed address if it has one, whether
// the unit defines it, and its initial bytes. The linker keeps the fixed
// addresses, places the rest for the whole program, and replaces each
// unit's `__zp_init` with one that stores every object's initial bytes.
//
// `hargs_need` and `check_hargs` are the parts of
// passes/runtime_layout.py that read the finished program. A helper
// reads its inputs from HARGS, writes its outputs there, and may use
// more of it as scratch (`mul32` works in HARGS+12..19), so a program
// needs the larger of its own highest HARGS offset and the footprint of
// every helper it calls.
#include "passes/zp_globals.h"

#include <algorithm>
#include <utility>

#include "errors.h"
#include "passes/asm_operands.h"
#include "pyrepr.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::asm_ast;

// Appends the little-endian bytes of the `n`-byte value `value` as
// immediates.
void number(asdl::Int value, int n, std::vector<ir::Type_operand>& out) {
  value &= (asdl::Int(1) << (8 * n)) - 1;
  for (int k = 0; k < n; ++k) {
    out.emplace_back(ir::Imm{.value = (value >> (8 * k)) & 0xFF});
  }
}

// Returns the source operand of each initial byte of `init`: an `Imm`,
// or for an address the `ImmLabelLow` and `ImmLabelHigh` of the label.
std::vector<ir::Type_operand> init_operands(const std::vector<ir::Type_static_init>& init) {
  std::vector<ir::Type_operand> out;
  for (const auto& item : init) {
    switch (ir::kind(item)) {
      case ir::Kind_static_init::CharInit:
        number(std::get<ir::CharInit>(item).value, 1, out);
        break;
      case ir::Kind_static_init::IntInit:
        number(std::get<ir::IntInit>(item).value, 2, out);
        break;
      case ir::Kind_static_init::LongInit:
        number(std::get<ir::LongInit>(item).value, 4, out);
        break;
      case ir::Kind_static_init::FloatInit:
        number(std::get<ir::FloatInit>(item).bits, 4, out);
        break;
      case ir::Kind_static_init::LongLongInit:
        number(std::get<ir::LongLongInit>(item).value, 8, out);
        break;
      case ir::Kind_static_init::DoubleInit:
        number(std::get<ir::DoubleInit>(item).bits, 8, out);
        break;
      case ir::Kind_static_init::AddressInit: {
        const auto& a = std::get<ir::AddressInit>(item);
        out.emplace_back(ir::ImmLabelLow{.name = a.name, .offset = a.offset});
        out.emplace_back(ir::ImmLabelHigh{.name = a.name, .offset = a.offset});
        break;
      }
      case ir::Kind_static_init::AddressByteInit: {
        const auto& a = std::get<ir::AddressByteInit>(item);
        if (a.high) {
          out.emplace_back(ir::ImmLabelHigh{.name = a.name, .offset = a.offset});
        } else {
          out.emplace_back(ir::ImmLabelLow{.name = a.name, .offset = a.offset});
        }
        break;
      }
      case ir::Kind_static_init::StringInit: {
        const auto& s = std::get<ir::StringInit>(item);
        const std::vector<std::uint32_t> chars = pyrepr::code_points(s.str);
        for (const std::uint32_t c : chars) {
          out.emplace_back(ir::Imm{.value = static_cast<int>(c & 0xFF)});
        }
        for (asdl::Int k = static_cast<long long>(chars.size()); k < s.bytes; k += 1) {
          out.emplace_back(ir::Imm{.value = 0});
        }
        break;
      }
      case ir::Kind_static_init::ZeroInit:
        for (asdl::Int k = 0; k < std::get<ir::ZeroInit>(item).bytes; k += 1) {
          out.emplace_back(ir::Imm{.value = 0});
        }
        break;
    }
  }
  return out;
}

// Returns the HARGS offsets that `instrs` reads or writes, as the
// highest one plus one, or 0 for none.
asdl::Int hargs_extent(const std::vector<ir::Type_instruction>& instrs) {
  asdl::Int extent = 0;
  auto note = [&](const ir::Type_operand& op) {
    if (const auto* d = asdl::as<ir::Data>(op); d != nullptr && d->name == "HARGS") {
      extent = std::max(extent, d->offset + 1);
    } else if (const auto* x = asdl::as<ir::IndexedData>(op);
               x != nullptr && x->name == "HARGS") {
      extent = std::max(extent, x->offset + 1);
    }
  };
  for (const auto& instr : instrs) {
    for_each_operand<false>(instr, note);
    // `DecimalArith` keeps its operands in lists.
    if (const auto* d = asdl::as<ir::DecimalArith>(instr)) {
      for (const auto* list : {&d->left, &d->right, &d->dst}) {
        for (const auto& op : *list) {
          note(op);
        }
      }
    }
  }
  return extent;
}

}  // namespace

asdl::Int hargs_need(const asm_ast::Program& prog, const HelperFootprints& footprints) {
  asdl::Int need = 0;
  for (const auto& tl : prog.top_level) {
    const auto* fn = asdl::as<ir::Function>(tl);
    if (fn == nullptr) {
      continue;
    }
    need = std::max(need, hargs_extent(fn->instructions));
    for (const auto& instr : fn->instructions) {
      if (const auto* call = asdl::as<ir::Call>(instr)) {
        if (const auto it = footprints.find(call->name); it != footprints.end()) {
          need = std::max(need, it->second);
        }
      }
    }
  }
  return need;
}

void check_hargs(const asm_ast::Program& prog, const RuntimeLayout& layout,
                 const HelperFootprints& footprints) {
  const asdl::Int need = hargs_need(prog, footprints);
  if (need > layout.hargs_size) {
    throw CompileError("RuntimeLayoutError",
                       "internal error: the program uses " + pyrepr::integer(need) +
                           " HARGS bytes, but the runtime layout was sized for " +
                           pyrepr::integer(layout.hargs_size));
  }
}

asm_ast::Program place_zp_globals(asm_ast::Program prog, const ZpGlobals& zp,
                                  const HelperFootprints& footprints) {
  check_hargs(prog, zp.layout, footprints);
  if (zp.addrs.empty()) {
    return prog;
  }
  // The bytes each value is stored to, by the dump of the value, in the
  // order the values first appear. The first entry of each list is the
  // value itself.
  std::map<std::string, std::vector<ir::Type_operand>> stores;
  std::vector<std::string> order;
  std::vector<ir::Type_top_level> top;
  for (auto& tl : prog.top_level) {
    if (const auto* sv = asdl::as<ir::StaticVariable>(tl);
        sv != nullptr && zp.addrs.contains(sv->name)) {
      if (zp.owned.contains(sv->name)) {
        const std::vector<ir::Type_operand> srcs = init_operands(sv->init);
        for (std::size_t k = 0; k < srcs.size(); ++k) {
          std::string key = asdl::dump(srcs[k]);
          auto [it, fresh] = stores.try_emplace(key);
          if (fresh) {
            it->second.push_back(srcs[k]);
            order.push_back(key);
          }
          it->second.emplace_back(
              ir::Data{.name = sv->name, .offset = static_cast<long long>(k)});
        }
      }
      continue;
    }
    top.push_back(std::move(tl));
  }
  if (order.empty()) {
    return ir::Program{.top_level = std::move(top)};
  }
  const std::string zero = asdl::dump(ir::Type_operand(ir::Imm{.value = 0}));
  std::stable_sort(order.begin(), order.end(), [&](const std::string& a, const std::string& b) {
    return (a == zero) && (b != zero);
  });
  const ir::Type_operand a = ir::Reg{.reg = ir::A{}};
  std::vector<ir::Type_instruction> body;
  for (const auto& key : order) {
    const auto& values = stores.at(key);
    body.emplace_back(ir::Mov{.src = values.front(), .dst = a, .is_volatile = false});
    for (std::size_t i = 1; i < values.size(); ++i) {
      body.emplace_back(ir::Mov{.src = a, .dst = values[i], .is_volatile = false});
    }
  }
  body.emplace_back(ir::Return{.save_a = false, .save_x = false});
  top.emplace_back(ir::Function{
      .name = kZpInitFunction, .is_global = true, .params = {}, .instructions = std::move(body)});
  return ir::Program{.top_level = std::move(top)};
}

std::vector<ZpObjectMeta> link_records(const asm_ast::Program& prog, const ZpGlobals& zp) {
  std::map<std::string, const std::vector<ir::Type_static_init>*, std::less<>> inits;
  NameSet internal;
  for (const auto& tl : prog.top_level) {
    if (const auto* sv = asdl::as<ir::StaticVariable>(tl)) {
      inits[sv->name] = &sv->init;
      if (!sv->is_global) {
        internal.insert(sv->name);
      }
    }
  }
  auto text = [](const ir::Type_operand& op) -> std::string {
    if (const auto* i = asdl::as<ir::Imm>(op)) {
      return "$" + pyrepr::hex(i->value & 0xFF, 2);
    }
    std::string half;
    const std::string* name = nullptr;
    asdl::Int offset = 0;
    if (const auto* l = asdl::as<ir::ImmLabelLow>(op)) {
      half = "<";
      name = &l->name;
      offset = l->offset;
    } else {
      const auto& h = std::get<ir::ImmLabelHigh>(op);
      half = ">";
      name = &h.name;
      offset = h.offset;
    }
    if (offset == 0) {
      return half + *name;
    }
    return half + "(" + *name + "+" + pyrepr::integer(offset) + ")";
  };
  std::vector<std::pair<asdl::Int, std::string>> by_address;
  for (const auto& [name, addr] : zp.addrs) {
    by_address.emplace_back(addr, name);
  }
  std::sort(by_address.begin(), by_address.end());
  std::vector<ZpObjectMeta> out;
  for (const auto& [addr, name] : by_address) {
    const bool defined = zp.owned.contains(name);
    ZpObjectMeta meta{.name = name, .size = zp.sizes.at(name), .defined = defined};
    if (zp.pinned.contains(name)) {
      meta.at = addr;
    }
    if (defined) {
      if (const auto it = inits.find(name); it != inits.end()) {
        for (const auto& op : init_operands(*it->second)) {
          meta.init.push_back(text(op));
        }
      }
    }
    if (const auto it = zp.promoted.find(name); it != zp.promoted.end()) {
      meta.weight = it->second;
    }
    meta.local = internal.contains(name);
    out.push_back(std::move(meta));
  }
  for (const auto& [name, size] : zp.external) {
    out.push_back(ZpObjectMeta{.name = name, .size = size, .defined = false});
  }
  return out;
}

}  // namespace c6502::passes
