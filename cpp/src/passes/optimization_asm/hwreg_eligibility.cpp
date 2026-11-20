// The eligibility of the asm SSA pseudos for coloring to X or Y.
//
// The asm allocator colors most pseudos to zero-page bytes, but some
// may live in the 6502's X or Y index register across their whole live
// range, which lies within a block. The main gain is the `LDX` / `LDY`
// setup it saves before each `IndexedData(name, offset, index=R)` access
// whose index is the pinned pseudo. tac_to_asm always writes
//
//     Mov(P, Reg(A)); Mov(Reg(A), Reg(X)); IndexedData(... index=X)
//
// (LDA / TAX / LDA name,X), and with P pinned to X the setup collapses
// to nothing, since X already holds P's value.
//
// # Eligibility
//
// A pseudo P is eligible when every definition and use of it in the
// function is one of the supported instructions. The definitions:
//
//   * `Mov(<Imm | Reg(A) | ZP | Data | Pseudo>, P)`, which is LDX / LDY.
//     The 6502 has LDX / LDY with zero page, absolute, and immediate
//     operands and no indirect-Y form, so a Stack, Frame, or Indirect
//     source disqualifies P, and so does an address-taken pseudo, which
//     the allocator never colors and which can end up in a Frame slot.
//   * `Inc(P)`, which is INX / INY.
//   * `Dec(P)`, which is DEX / DEY.
//   * `Phi(dst=P)`, a Phi merging the values from the predecessors. SSA
//     destruction turns its copies into `Mov(<src>, P)` at the end of
//     each predecessor, where `<src>` is another pseudo (likely pinned
//     too after coalescing) or one of the shapes LDX / LDY take.
//
// The uses:
//
//   * `Mov(P, <Reg(A) | ZP | Data | Pseudo>)`, which is TXA / TYA / STX
//     / STY (zero page or absolute only, where a Stack, Frame, Indirect,
//     or address-taken pseudo destination would need A and disqualifies
//     P).
//   * `Mov(P, <other Pseudo>)`, kept for completeness, which the emitter
//     writes through A between X and Y. After coalescing it mostly goes.
//   * `Inc(P)` / `Dec(P)`, already counted as definitions.
//   * `Compare(P, <Imm | ZP | Data>)`, which is CPX / CPY.
//   * The index of an `IndexedData`, which the absolute,X or absolute,Y
//     addressing mode reads without an explicit move of the pseudo. What
//     reads P there is the setup `Mov(P, Reg(A)); Mov(Reg(A),
//     Reg(X|Y))` that tac_to_asm writes before each `IndexedData`
//     access, which the scan finds, so the `IndexedData` counts as a use
//     of P.
//
// The hard constraints are one byte (offset 0, since a value of several
// bytes can't ride in X or Y), no `Call` in the live range, since the
// helpers' ABI clobbers A, X, and Y (the allocator reads the
// `lives_across_call` that the interference builder sets), and width 1
// in the interference graph, a guard against an unusual name of several
// bytes slipping through.
//
// If any definition or use of P doesn't match these rules, P isn't
// eligible. The scan visits each instruction once.
//
// # Hints
//
// Some eligible pseudos have a strong preference for one register,
// which the shape of the IR gives.
//
//   * An X hint: P is in a chain `Mov(P, Reg(A)); Mov(Reg(A), Reg(X))`,
//     the usual setup of an `IndexedData` index. With P colored to X the
//     chain becomes `Mov(Reg(X), Reg(A)); Mov(Reg(A), Reg(X))`, which
//     apply_coloring drops as redundant, and P costs nothing in the
//     `IndexedData` uses after it.
//   * A Y hint: the same with Reg(Y). It is less common in what
//     tac_to_asm writes today, since every `IndexedData` uses X, but it
//     appears after apply_coloring rewrites `IndexedData.index` when one
//     pseudo is pinned to X and another wants the same role.
//
// A pseudo with both hints would need the setup for X and for Y in one
// function, which is rare, and the coloring then takes whichever
// register is still free.
//
//   * A carry hint (`hints_carry`): P and Q in `Mov(P, Reg(A));
//     Add|Sub(Imm(0), Reg(A)); Mov(Reg(A), Q)`, the upper byte of an add
//     or subtract of a byte to a value of several bytes. In X it's `TXA;
//     SBC #0; TAX`, which carry_inc turns into `BCS +; DEX` (or, when
//     the carry is read after, `BCS +; CPX #1; DEX`), where a zero-page
//     byte is `LDA M; SBC #0; STA M`. The allocator tries these after
//     the index hints, so they never take a register an index wanted,
//     and pins P and Q to one register or to none (`carry_pairs`): with
//     only Q in X, P would have to be loaded into X first, and the
//     rewrite would save nothing. P must be a Phi, a value a loop
//     carries round, since that's where keeping it in a register pays.
//     In straight-line code the byte P comes from is usually in memory
//     already, where carry_inc updates it in place, and moving it into X
//     or Y costs a transfer each way. The high byte of
//     `do_rwts_on_sector`'s 16-bit `sector` in examples/zork/disk.c
//     lives in X this way, as in Zork's own code.
//
// # Output
//
// `scan_function` returns a `HwRegEligibility` with the sets of names
// that pass the rules for each register, which the coloring checks
// (a name that isn't eligible can't be pinned, whatever its hints), and
// the hint sets, which may overlap (rarely). The `lives_across_call` of
// the interference graph's node is checked separately by the allocator,
// so this scan reads only the shapes of the instructions.
#include "passes/optimization_asm/hwreg_eligibility.h"

#include <utility>

#include "passes/decimal_arith.h"

namespace c6502::passes {

namespace ir = ::c6502::asm_ast;

NameSet HwRegEligibility::eligible() const {
  NameSet out = eligible_x;
  out.insert(eligible_y.begin(), eligible_y.end());
  return out;
}

namespace hwreg_eligibility {

namespace {

bool is_digit(char c) { return c >= '0' && c <= '9'; }

// Returns the length of the run of digits that ends at `end` in `s`.
std::size_t digits_before(std::string_view s, std::size_t end) {
  std::size_t i = end;
  while (i > 0 && is_digit(s[i - 1])) {
    --i;
  }
  return end - i;
}

}  // namespace

std::string ssa_base(std::string_view name) {
  // `^(.*?)\.b\d+\.v\d+$`: the suffix that ends the name, since no
  // earlier position can match a pattern anchored at the end with only
  // digits after its last `.v`.
  {
    std::size_t end = name.size();
    const std::size_t v_digits = digits_before(name, end);
    if (v_digits > 0 && end - v_digits >= 2 && name.substr(end - v_digits - 2, 2) == ".v") {
      end -= v_digits + 2;
      const std::size_t b_digits = digits_before(name, end);
      if (b_digits > 0 && end - b_digits >= 2 && name.substr(end - b_digits - 2, 2) == ".b") {
        name = name.substr(0, end - b_digits - 2);
      }
    }
  }
  // `^(.*?)\.\d+$`: one TAC suffix.
  const std::size_t digits = digits_before(name, name.size());
  if (digits > 0 && name.size() - digits >= 1 && name[name.size() - digits - 1] == '.') {
    name = name.substr(0, name.size() - digits - 1);
  }
  return std::string(name);
}

}  // namespace hwreg_eligibility

namespace {

// The position a pseudo holds in an instruction, which decides whether
// it can be pinned. `kOther` is any position that disqualifies it (the
// binary operations, `Push`, `Pop`, `LoadAddress`, and the rest).
enum class Role { kMovSrc, kMovDst, kIncDec, kCmpLeft, kCmpRight, kPhiDst, kPhiArg, kOther };

bool is_a(const ir::Type_operand& op) {
  const auto* r = asdl::as<ir::Reg>(op);
  return r != nullptr && asdl::isa<ir::A>(r->reg);
}

// Calls `fn(role, operand)` for every operand of `instr`.
template <class Fn>
void operand_roles(const ir::Type_instruction& instr, Fn&& fn) {
  using K = ir::Kind_instruction;
  switch (ir::kind(instr)) {
    case K::DecimalArith:
      // ADC and SBC can't read X or Y, so no operand of a BCD chain goes
      // in a register.
      for (const auto& op : decimal_arith::operands(std::get<ir::DecimalArith>(instr))) {
        fn(Role::kOther, op);
      }
      break;
    case K::Mov: {
      const auto& m = std::get<ir::Mov>(instr);
      fn(Role::kMovSrc, m.src);
      fn(Role::kMovDst, m.dst);
      break;
    }
    case K::Inc:
      fn(Role::kIncDec, std::get<ir::Inc>(instr).dst);
      break;
    case K::Dec:
      fn(Role::kIncDec, std::get<ir::Dec>(instr).dst);
      break;
    case K::Compare: {
      const auto& c = std::get<ir::Compare>(instr);
      fn(Role::kCmpLeft, c.left);
      fn(Role::kCmpRight, c.right);
      break;
    }
    case K::Phi: {
      const auto& p = std::get<ir::Phi>(instr);
      fn(Role::kPhiDst, p.dst);
      for (const auto& a : p.args) {
        fn(Role::kPhiArg, std::get<ir::AsmPhiArg>(a).source);
      }
      break;
    }
    case K::Add: {
      const auto& a = std::get<ir::Add>(instr);
      fn(Role::kOther, a.src);
      fn(Role::kOther, a.dst);
      break;
    }
    case K::Sub: {
      const auto& a = std::get<ir::Sub>(instr);
      fn(Role::kOther, a.src);
      fn(Role::kOther, a.dst);
      break;
    }
    case K::And: {
      const auto& a = std::get<ir::And>(instr);
      fn(Role::kOther, a.src);
      fn(Role::kOther, a.dst);
      break;
    }
    case K::Or: {
      const auto& a = std::get<ir::Or>(instr);
      fn(Role::kOther, a.src);
      fn(Role::kOther, a.dst);
      break;
    }
    case K::Xor: {
      const auto& x = std::get<ir::Xor>(instr);
      fn(Role::kOther, x.src1);
      fn(Role::kOther, x.src2);
      fn(Role::kOther, x.dst);
      break;
    }
    case K::ArithmeticShiftLeft:
      fn(Role::kOther, std::get<ir::ArithmeticShiftLeft>(instr).dst);
      break;
    case K::LogicalShiftRight:
      fn(Role::kOther, std::get<ir::LogicalShiftRight>(instr).dst);
      break;
    case K::RotateLeft:
      fn(Role::kOther, std::get<ir::RotateLeft>(instr).dst);
      break;
    case K::RotateRight:
      fn(Role::kOther, std::get<ir::RotateRight>(instr).dst);
      break;
    case K::Push:
      fn(Role::kOther, std::get<ir::Push>(instr).src);
      break;
    case K::Pop:
      fn(Role::kOther, std::get<ir::Pop>(instr).dst);
      break;
    case K::LoadAddress: {
      const auto& l = std::get<ir::LoadAddress>(instr);
      fn(Role::kOther, l.src);
      fn(Role::kOther, l.dst);
      break;
    }
    default:
      break;
  }
}

using XY = std::pair<bool, bool>;

// Returns whether `peer`, the other side of a `Mov` whose pseudo side
// would be X or Y, is something `asm_emit` can write, for X and for Y.
// `role` says which side the pseudo is: the source (`kMovSrc`, and the
// peer is the destination) or the destination (`kMovDst`). That matters
// only for an `IndexedData`: a load through `LDX abs,Y` or `LDY abs,X`
// is valid, while the 6502 has no store `STX abs,X|Y` or `STY abs,X|Y`,
// so no `Mov(Reg(X|Y), IndexedData)` can be written.
XY peer_is_hwreg_friendly(const ir::Type_operand& peer, Role role) {
  using K = ir::Kind_operand;
  switch (ir::kind(peer)) {
    case K::Imm:
      return {true, true};
    // A, X, and Y are all fine, since TXA / TYA / TAX / TAY exist and the
    // emitter drops a move of a register to itself.
    case K::Reg:
      return {true, true};
    // LDX / LDY and STX / STY with zero page or absolute.
    case K::Data:
    case K::ZP:
      return {true, true};
    // After coalescing the peer may or may not be colored to a
    // register, and apply_coloring resolves both sides before emission.
    case K::Pseudo:
      return {true, true};
    // Indirect-Y addressing, which LDX / LDY / STX / STY lack. There's
    // no move from one of these into X or Y at all, and the store is
    // written as `TXA` / `TYA` and an `LDY #off; STA (zp),Y`, which
    // overwrites A and Y where the IR doesn't say so, so later passes
    // that track A would go wrong.
    case K::Stack:
    case K::Frame:
    case K::Indirect:
      return {false, false};
    case K::IndexedData: {
      if (role == Role::kMovDst) {
        // The pseudo is the load's target and the peer its source,
        // which is valid only when the load's register differs from the
        // index register (`LDX abs,Y`, `LDY abs,X`).
        const auto& idx = std::get<ir::IndexedData>(peer).index;
        return {asdl::isa<ir::Y>(idx), asdl::isa<ir::X>(idx)};
      }
      // A store, which goes from A only.
      return {false, false};
    }
    // `LDA #<name`, an immediate, only as a source, which the emitter
    // writes into A only today. It counts as fine for now.
    case K::ImmLabelLow:
    case K::ImmLabelHigh:
      return {true, true};
    default:
      return {false, false};
  }
}

// Returns whether `peer` is a right operand of a `Compare` that CPX /
// CPY can take. They have only immediate, zero-page, and absolute
// addressing, with no indirect-Y. A pseudo right operand could become a
// register or zero page after coloring, but checking that again after
// the coloring isn't worth it, so it disqualifies.
bool cmp_peer_is_hwreg_friendly(const ir::Type_operand& peer) {
  return asdl::isa<ir::Imm, ir::Data, ir::ZP>(peer);
}

// Returns whether a pseudo in `role` of `instr` can be written when it
// is colored to X and to Y. Most roles give one answer for both, but a
// `Mov` with an `IndexedData` peer is asymmetric: `LDX abs,Y` and `LDY
// abs,X` exist, and `LDX abs,X` and `LDY abs,Y` don't, so a pseudo that
// is the destination of a load from an `IndexedData` indexed by X may go
// in Y and not in X.
XY role_ok(Role role, const ir::Type_instruction& instr, const NameSet& address_taken) {
  if (role == Role::kOther) {
    return {false, false};
  }
  if (role == Role::kIncDec) {
    return {true, true};
  }
  if (role == Role::kPhiDst || role == Role::kPhiArg) {
    // apply_coloring substitutes a Phi's arguments after SSA destruction
    // turns each into a `Mov` on the edge from its predecessor, and that
    // `Mov` is checked there as an ordinary pair of source and
    // destination, so a Phi is accepted here.
    return {true, true};
  }
  if (const auto* mov = asdl::as<ir::Mov>(instr)) {
    const ir::Type_operand& peer = role == Role::kMovSrc ? mov->dst : mov->src;
    // An address-taken pseudo counts as the Frame slot it can end up in.
    if (const auto* p = asdl::as<ir::Pseudo>(peer); p != nullptr && address_taken.contains(p->name)) {
      return peer_is_hwreg_friendly(ir::Frame{.offset = 0}, role);
    }
    return peer_is_hwreg_friendly(peer, role);
  }
  if (role == Role::kCmpLeft) {
    // `Compare(Reg(X|Y), <Imm | ZP | Data>)` is CPX / CPY. A Frame,
    // Stack, or Indirect right operand isn't, since CPX / CPY have no
    // indirect-Y.
    const bool ok = cmp_peer_is_hwreg_friendly(std::get<ir::Compare>(instr).right);
    return {ok, ok};
  }
  // A pseudo on the right of a `Compare` is unusual, and CPX / CPY have
  // no form with X or Y on the right unless the left is A, so it
  // disqualifies.
  return {false, false};
}

NameSet intersect(const NameSet& a, const NameSet& b) {
  NameSet out;
  for (const auto& n : a) {
    if (b.contains(n)) {
      out.insert(n);
    }
  }
  return out;
}

}  // namespace

HwRegEligibility scan_function(const ir::Function& fn,
                               const OrderedMap<std::string, std::string>* register_pins,
                               const OrderedMap<std::string, std::string>* register_hints) {
  NameSet seen;
  NameSet disqualified_x;
  NameSet disqualified_y;
  HwRegEligibility out;

  const auto& instrs = fn.instructions;
  NameSet phi_dsts;
  // An address-taken name (`LoadAddress.src`) never gets a color from
  // the allocator. replace_pseudoregisters gives it a Frame slot, or a
  // zero-page byte from the function's private pool when one fits, so
  // as the peer of a `Mov` it counts as a Frame operand.
  NameSet address_taken;
  for (const auto& ins : instrs) {
    if (const auto* phi = asdl::as<ir::Phi>(ins)) {
      if (const auto* d = asdl::as<ir::Pseudo>(phi->dst)) {
        phi_dsts.insert(d->name);
      }
    } else if (const auto* la = asdl::as<ir::LoadAddress>(ins)) {
      if (const auto* s = asdl::as<ir::Pseudo>(la->src)) {
        address_taken.insert(s->name);
      }
    }
  }

  for (std::size_t i = 0; i < instrs.size(); ++i) {
    const auto& instr = instrs[i];
    operand_roles(instr, [&](Role role, const ir::Type_operand& op) {
      const auto* p = asdl::as<ir::Pseudo>(op);
      if (p == nullptr) {
        return;
      }
      seen.insert(p->name);
      if (asdl::Int* n = out.use_count.find(p->name)) {
        *n += 1;
      } else {
        out.use_count.insert_or_assign(p->name, 1);
      }
      if (p->offset != 0) {
        disqualified_x.insert(p->name);
        disqualified_y.insert(p->name);
        return;
      }
      const auto [x_ok, y_ok] = role_ok(role, instr, address_taken);
      if (!x_ok) {
        disqualified_x.insert(p->name);
      }
      if (!y_ok) {
        disqualified_y.insert(p->name);
      }
    });
    // The upper byte of an add or subtract of a byte:
    //   instrs[i-1] = Mov(P, Reg(A))
    //   instrs[i  ] = Add|Sub(Imm(0), Reg(A))
    //   instrs[i+1] = Mov(Reg(A), Q)
    if (i > 0 && i + 1 < instrs.size()) {
      const ir::Type_operand* src = nullptr;
      const ir::Type_operand* dst = nullptr;
      if (const auto* add = asdl::as<ir::Add>(instr)) {
        src = &add->src;
        dst = &add->dst;
      } else if (const auto* sub = asdl::as<ir::Sub>(instr)) {
        src = &sub->src;
        dst = &sub->dst;
      }
      const auto* imm = src != nullptr ? asdl::as<ir::Imm>(*src) : nullptr;
      if (imm != nullptr && imm->value == 0 && is_a(*dst)) {
        const auto* prev = asdl::as<ir::Mov>(instrs[i - 1]);
        const auto* nxt = asdl::as<ir::Mov>(instrs[i + 1]);
        const auto* p = prev != nullptr ? asdl::as<ir::Pseudo>(prev->src) : nullptr;
        const auto* q = nxt != nullptr ? asdl::as<ir::Pseudo>(nxt->dst) : nullptr;
        if (p != nullptr && is_a(prev->dst) && p->offset == 0 && phi_dsts.contains(p->name) &&
            q != nullptr && is_a(nxt->src) && q->offset == 0) {
          out.hints_carry.insert(p->name);
          out.hints_carry.insert(q->name);
          out.carry_pairs.emplace_back(p->name, q->name);
        }
      }
    }
    // The chain of index setup at this position:
    //   instrs[i-1] = Mov(P, Reg(A))
    //   instrs[i  ] = Mov(Reg(A), Reg(X|Y))
    // gives P the hint of that register. The chain is two adjacent
    // instructions, and a longer one with other code between doesn't
    // count, since anything between might clobber A or the index
    // register.
    if (i > 0) {
      const auto* mov = asdl::as<ir::Mov>(instr);
      const auto* prev = asdl::as<ir::Mov>(instrs[i - 1]);
      if (mov != nullptr && prev != nullptr && is_a(prev->dst)) {
        const auto* p = asdl::as<ir::Pseudo>(prev->src);
        const auto* dst = asdl::as<ir::Reg>(mov->dst);
        if (p != nullptr && p->offset == 0 && is_a(mov->src) && dst != nullptr) {
          if (asdl::isa<ir::X>(dst->reg)) {
            out.hints_x.insert(p->name);
          } else if (asdl::isa<ir::Y>(dst->reg)) {
            out.hints_y.insert(p->name);
          }
        }
      }
    }
  }

  for (const auto& n : seen) {
    if (!disqualified_x.contains(n)) {
      out.eligible_x.insert(n);
    }
    if (!disqualified_y.contains(n)) {
      out.eligible_y.insert(n);
    }
  }
  // The hints are kept to names eligible for either register. A name
  // with an X hint that is only eligible for Y still says something
  // useful, since the allocator's fallback tries the other register,
  // and a name eligible for neither can't be pinned at all.
  const NameSet eligible_any = out.eligible();
  out.hints_x = intersect(out.hints_x, eligible_any);
  out.hints_y = intersect(out.hints_y, eligible_any);
  out.hints_carry = intersect(out.hints_carry, eligible_any);
  // Each base name (such as `@2.y`) may have many SSA versions in
  // `seen`, and each goes in the required (hard) or hint (soft) set of
  // its base's register.
  for (const auto& name : seen) {
    const std::string base = hwreg_eligibility::ssa_base(name);
    if (register_pins != nullptr && !register_pins->empty()) {
      if (const std::string* target = register_pins->find(base)) {
        if (*target == "X") {
          out.required_x.insert(name);
        } else if (*target == "Y") {
          out.required_y.insert(name);
        }
      }
    }
    if (register_hints != nullptr && !register_hints->empty()) {
      if (const std::string* target = register_hints->find(base)) {
        if (*target == "X") {
          out.hints_x.insert(name);
        } else if (*target == "Y") {
          out.hints_y.insert(name);
        }
      }
    }
  }
  // The restriction again, so a hint added here for a name eligible for
  // neither register drops out, as the hints from the chains do.
  out.hints_x = intersect(out.hints_x, eligible_any);
  out.hints_y = intersect(out.hints_y, eligible_any);
  return out;
}

}  // namespace c6502::passes
