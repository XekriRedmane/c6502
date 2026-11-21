// How many low bytes of each local can be nonzero.
//
// `tac_to_asm` lowers a shift of a byte widened to 16 bits as a shift of
// all 16 bits, and calls `asl16` for one of more than 4 places, though
// the high byte is zero. `64 * (c & 7)` in Zork's
// `please_insert_save_diskette` is `ZeroExtend(c) & 7`, then `<< 6`.
// `significant_bytes` works out, for each local of a function, how many
// of its low bytes can be nonzero, so the lowering can shift just those
// (`tac_to_asm`'s `inline_left_shift_byte`).
//
// A local's count is the largest over its definitions:
//
// - `ZeroExtend(x)`, `Copy(x)`: x's count.
// - `Truncate(x)`: x's count, at most the local's size.
// - `x & y`: the smaller of the two counts.
// - `x | y`, `x ^ y`: the larger.
// - `x >> k` with a constant `k`, when x is unsigned or its top byte is
//   known to be zero: x's count less the bytes `k` shifts out.
// - A `Phi`: the largest over its arguments.
// - Anything else: every byte.
//
// A constant counts the bytes its value needs, a negative one every byte
// of its type.
//
// `low_byte_only` works out the converse, which bytes of a value are
// read: the locals defined once whose every read is a `Truncate` to one
// byte. `(uint8_t)(zchars >> 5)` in Zork's `get_next_zchar` is
// `t = zchars >> 5; Truncate(t)`, so `tac_to_asm` computes only `t`'s
// low byte (`inline_right_shift_to_byte`) where it called `lsr16`. A
// parameter, a static, a local whose address is taken (a store through a
// pointer could write it), and anything not defined in the function
// counts every byte. The counts start at 0 and only grow, so the loop
// that recomputes them ends.
#include "passes/significant_bytes.h"

#include <algorithm>
#include <set>
#include <utility>
#include <vector>

#include "asdl/runtime.h"
#include "bit_precise.h"
#include "passes/optimization/var_visit.h"

namespace c6502::passes {

namespace tac = ::c6502::tac_ast;

namespace {

// Returns the number of bits of the non-negative `v`, Python's
// `int.bit_length`.
int bit_length(asdl::Int v) {
  int n = 0;
  while (v != 0) {
    v >>= 1;
    ++n;
  }
  return n;
}

// Returns the names whose address `fn` takes with `GetAddress`.
NameSet addressed_names(const tac::Function& fn) {
  NameSet out;
  for (const auto& ins : fn.instructions) {
    if (const auto* get = asdl::as<tac::GetAddress>(ins)) {
      if (const auto* var = asdl::as<tac::Var>(get->operand)) {
        out.insert(var->name);
      }
    }
  }
  return out;
}

bool is_local(const SymbolTable* symbols, std::string_view name) {
  if (symbols == nullptr) {
    return false;
  }
  const Symbol* sym = symbols->find(name);
  return sym != nullptr && std::holds_alternative<LocalAttr>(sym->attrs);
}

}  // namespace

std::map<std::string, int, std::less<>> significant_bytes(const tac::Function& fn,
                                                          const SymbolTable* symbols,
                                                          const ValSize& size_of,
                                                          const ValTest& is_unsigned) {
  const NameSet addressed = addressed_names(fn);
  const NameSet params(fn.params.begin(), fn.params.end());
  // Each defined name's defining instructions, in the order of the
  // first definitions.
  OrderedMap<std::string, std::vector<const tac::Type_instruction*>> defs;
  for (const auto& ins : fn.instructions) {
    for (const auto d : defs_in(ins)) {
      defs[std::string(d)].push_back(&ins);
    }
  }
  OrderedMap<std::string, int> tracked;
  for (const auto& [name, _] : defs) {
    if (!is_local(symbols, name) || addressed.contains(name) || params.contains(name)) {
      continue;
    }
    tracked.insert_or_assign(name, 0);
  }

  const auto count = [&](const tac::Type_val& val) -> int {
    const int size = size_of(val);
    if (const auto* c = asdl::as<tac::Constant>(val)) {
      const auto v = bit_precise::raw_value(c->const_);
      if (!v || *v < 0) {
        return size;
      }
      return std::min(size, (bit_length(*v) + 7) / 8);
    }
    if (const auto* var = asdl::as<tac::Var>(val)) {
      if (const int* n = tracked.find(var->name)) {
        return *n;
      }
    }
    return size;
  };

  const auto of = [&](const tac::Type_instruction& ins, int size) -> int {
    switch (tac::kind(ins)) {
      case tac::Kind_instruction::ZeroExtend:
        return count(std::get<tac::ZeroExtend>(ins).src);
      case tac::Kind_instruction::Copy:
        return count(std::get<tac::Copy>(ins).src);
      case tac::Kind_instruction::Truncate:
        return count(std::get<tac::Truncate>(ins).src);
      case tac::Kind_instruction::Binary: {
        const auto& b = std::get<tac::Binary>(ins);
        if (asdl::isa<tac::BitwiseAnd>(b.op)) {
          return std::min(count(b.src1), count(b.src2));
        }
        if (asdl::isa<tac::BitwiseOr, tac::BitwiseXor>(b.op)) {
          return std::max(count(b.src1), count(b.src2));
        }
        if (asdl::isa<tac::RightShift>(b.op)) {
          const auto* k = asdl::as<tac::Constant>(b.src2);
          if (k == nullptr) {
            return size;
          }
          const int n = count(b.src1);
          const auto shift = bit_precise::raw_value(k->const_);
          if (!shift || *shift < 0) {
            return size;
          }
          if (n == size_of(b.src1) && !is_unsigned(b.src1)) {
            return size;
          }
          const asdl::Int bytes = asdl::floor_div(asdl::Int(8 * n) - *shift + 7, 8);
          return bytes < 0 ? 0 : static_cast<int>(bytes);
        }
        return size;
      }
      case tac::Kind_instruction::Phi: {
        const auto& phi = std::get<tac::Phi>(ins);
        if (phi.args.empty()) {
          return size;
        }
        int best = 0;
        bool first = true;
        for (const auto& a : phi.args) {
          const int c = count(std::get<tac::PhiArg>(a).source);
          best = first ? c : std::max(best, c);
          first = false;
        }
        return best;
      }
      default:
        return size;
    }
  };

  bool changed = true;
  while (changed) {
    changed = false;
    for (auto& [name, current] : tracked) {
      int n = 0;
      for (const tac::Type_instruction* ins : defs.at(name)) {
        const std::string dst(defs_in(*ins)[0]);
        const int size = size_of(tac::Var{dst});
        n = std::max(n, std::min(size, of(*ins, size)));
      }
      if (n > current) {
        current = n;
        changed = true;
      }
    }
  }
  std::map<std::string, int, std::less<>> out;
  for (const auto& [name, n] : tracked) {
    if (n < size_of(tac::Var{name})) {
      out.emplace(name, n);
    }
  }
  return out;
}

NameSet low_byte_only(const tac::Function& fn, const SymbolTable* symbols,
                      const ValSize& size_of) {
  OrderedMap<std::string, int> defs;
  for (const auto& ins : fn.instructions) {
    for (const auto d : defs_in(ins)) {
      defs[std::string(d)] += 1;
    }
  }
  const NameSet params(fn.params.begin(), fn.params.end());
  const NameSet addressed = addressed_names(fn);
  NameSet ok;
  for (const auto& [name, n] : defs) {
    if (n == 1 && !params.contains(name) && !addressed.contains(name) &&
        is_local(symbols, name)) {
      ok.insert(name);
    }
  }
  NameSet read;
  for (const auto& ins : fn.instructions) {
    for (const auto v : uses_in(ins)) {
      const auto it = ok.find(v);
      if (it == ok.end()) {
        continue;
      }
      read.emplace(v);
      bool truncates = false;
      if (const auto* t = asdl::as<tac::Truncate>(ins)) {
        const auto* src = asdl::as<tac::Var>(t->src);
        truncates = src != nullptr && src->name == v && size_of(t->dst) == 1;
      }
      if (!truncates) {
        ok.erase(it);
      }
    }
  }
  NameSet out;
  for (const auto& name : ok) {
    if (read.contains(name)) {
      out.insert(name);
    }
  }
  return out;
}

}  // namespace c6502::passes
