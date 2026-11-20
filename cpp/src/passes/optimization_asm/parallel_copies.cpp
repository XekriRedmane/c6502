// The ordering of a parallel copy.
//
// The `Phi`s at the start of a block stand for moves that all happen at
// once on each edge into it, and the arguments of a call that go in the
// callee's zero-page slots are such a set of moves too. Written one
// after another in their own order, a move can read a byte that an
// earlier move of the set has just written, the classic "lost copy".
// As the TAC `from_ssa` does, the moves are sorted so that a move whose
// destination no other pending move reads goes first. When every
// pending move's destination is read by another, the moves form a
// cycle (two bytes swapped, `a, b = b, a`), which a new temporary
// breaks: the first pending move's destination is copied into it, and
// the moves that read that destination read the temporary instead.
//
// The temporary is a Pseudo named `.<fn_name>@asm_cycle_tmp@<N>`.
// `replace_pseudoregisters_bare_exit` gives it a frame byte of its
// own, since it isn't in the coloring and the symbol table doesn't
// have it, so `size_of_name` gives it 1 byte. One byte is right,
// because asm SSA versions a byte at a time and every move of a cycle
// is of one byte.
#include "passes/optimization_asm/parallel_copies.h"

#include <cstddef>
#include <optional>

namespace c6502::passes {

namespace {

namespace ir = ::c6502::asm_ast;
using ::c6502::asdl::as;

// The storage an operand names. Two operands with equal keys name the
// same location. Comparing by location rather than by SSA name finds
// the cycles that `apply_coloring` makes when it puts two distinct SSA
// names at one zero-page byte.
struct StorageKey {
  ir::Kind_operand kind;
  std::string name;
  asdl::Int value = 0;
  bool operator==(const StorageKey&) const = default;
};

// Returns the key of `op`, or nothing for an operand no other operand
// can alias (an immediate, a label's half, or a register-indexed or
// indirect form), which is never the source of a cycle.
std::optional<StorageKey> storage_key(const ir::Type_operand& op) {
  switch (ir::kind(op)) {
    case ir::Kind_operand::Pseudo: {
      const auto& p = std::get<ir::Pseudo>(op);
      return StorageKey{ir::Kind_operand::Pseudo, p.name, p.offset};
    }
    case ir::Kind_operand::ZP: {
      const auto& z = std::get<ir::ZP>(op);
      return StorageKey{ir::Kind_operand::ZP, "", z.address + z.offset};
    }
    case ir::Kind_operand::Reg:
      return StorageKey{ir::Kind_operand::Reg, "",
                        static_cast<int>(ir::kind(std::get<ir::Reg>(op).reg))};
    case ir::Kind_operand::Stack:
      return StorageKey{ir::Kind_operand::Stack, "", std::get<ir::Stack>(op).offset};
    case ir::Kind_operand::Frame:
      return StorageKey{ir::Kind_operand::Frame, "", std::get<ir::Frame>(op).offset};
    case ir::Kind_operand::Data: {
      const auto& d = std::get<ir::Data>(op);
      return StorageKey{ir::Kind_operand::Data, d.name, d.offset};
    }
    case ir::Kind_operand::Indirect:
      return StorageKey{ir::Kind_operand::Indirect, "", std::get<ir::Indirect>(op).offset};
    default:
      return std::nullopt;
  }
}

}  // namespace

std::vector<ir::Mov> order_parallel_copies(std::vector<ir::Mov> movs, const std::string& fn_name,
                                           int& cycle_counter) {
  if (movs.size() <= 1) {
    return movs;
  }
  std::vector<ir::Mov> pending = std::move(movs);
  std::vector<ir::Mov> out;
  out.reserve(pending.size());
  while (!pending.empty()) {
    std::optional<std::size_t> ready;
    for (std::size_t i = 0; i < pending.size() && !ready; ++i) {
      const auto d_key = storage_key(pending[i].dst);
      if (!d_key) {
        // Nothing reads a destination that nothing aliases.
        ready = i;
        break;
      }
      bool blocks_other = false;
      for (std::size_t j = 0; j < pending.size() && !blocks_other; ++j) {
        blocks_other = j != i && storage_key(pending[j].src) == d_key;
      }
      if (!blocks_other) {
        ready = i;
      }
    }
    if (ready) {
      out.push_back(std::move(pending[*ready]));
      pending.erase(pending.begin() + static_cast<std::ptrdiff_t>(*ready));
      continue;
    }
    // The pending moves form a cycle. The first one's destination is
    // saved in a new temporary, and each other pending move that reads
    // that destination reads the temporary.
    const ir::Mov& chosen = pending[0];
    const auto d_key = storage_key(chosen.dst);
    if (!d_key) {
      for (auto& m : pending) {
        out.push_back(std::move(m));
      }
      break;
    }
    ++cycle_counter;
    const ir::Pseudo tmp{.name = "." + fn_name + "@asm_cycle_tmp@" + std::to_string(cycle_counter),
                         .offset = 0};
    out.push_back(ir::Mov{.src = chosen.dst, .dst = tmp});
    for (std::size_t j = 1; j < pending.size(); ++j) {
      if (storage_key(pending[j].src) == d_key) {
        pending[j] = ir::Mov{.src = tmp, .dst = pending[j].dst};
      }
    }
  }
  return out;
}

}  // namespace c6502::passes
