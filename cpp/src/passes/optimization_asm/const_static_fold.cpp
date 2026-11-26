// Folds the const-qualified scalar statics of internal linkage into
// immediates.
//
// A `static T const x = <constant>;` (or, more usefully in c6502,
// `static T *const p = <constant>;`) at file scope can't change in a
// program of one unit, since `static` keeps the name from escaping and
// `const` rejects writes through the type system. When its initializer
// is one scalar constant and nothing in the program takes its address,
// every reference to its bytes can be replaced by the immediate byte at
// compile time, which turns `LDA name` (3 bytes) into `LDA #imm` (2
// bytes) and frees the storage of `name` itself.
//
// The pass takes as a candidate each `StaticVariable` whose
//
// - `is_global` is false (internal linkage, a `static` at file scope or
//   any `static` in a block),
// - type in the symbol table is `Const(...)` at the top (a `const int
//   *p` is `Pointer(Const(Int))`, which qualifies what the pointer
//   points to, and the pointer's own bytes aren't folded),
// - and `init` is one `CharInit`, `IntInit`, `LongInit`,
//   `LongLongInit`, `FloatInit`, `DoubleInit`, or `AddressInit` (one
//   foldable scalar, which leaves out arrays).
//
// It then rules out a candidate
//
// - whose address appears in another static's initializer
//   (`AddressInit(name=candidate, ...)`),
// - that is the source of a `LoadAddress` (`&candidate`),
// - that an instruction writes, which the type checker rejects for a
//   const lvalue,
// - or that appears as an `IndexedData(name=candidate, ...)` operand.
//
// The bytes of the candidates left can be folded. In every function,
// each `Pseudo(name=candidate, offset=k)` and `Data(name=candidate,
// offset=k)` that is read becomes the byte of the initializer at k. A
// `Data` read is a load through the object's address that the TAC
// optimizer made a direct read, as `*pk` after `pk = &K`. Definitions
// and the operands of a `LoadAddress` aren't rewritten, since the rules
// above keep a candidate out of both. Last, the pass drops the
// `StaticVariable`s that nothing names any more.
//
// It runs first in `optimize_program`, before the SSA round trip of
// each function, so the forward copy propagation and the dead code
// elimination of the round trip clean up the `Mov(Imm, Pseudo)`
// staging the fold leaves behind.
#include "passes/optimization_asm/const_static_fold.h"

#include <map>
#include <optional>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "passes/decimal_arith.h"
#include "passes/optimization_asm/all_operands.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::asm_ast;
namespace c99 = ::c6502::c99_ast;
using ::c6502::asdl::as;
using ::c6502::asdl::isa;
using Bytes = std::vector<ir::Type_operand>;
using Candidates = std::map<std::string, Bytes, std::less<>>;
using Names = std::set<std::string, std::less<>>;

// Returns whether `t` is const-qualified and not volatile. The outermost
// wrapper may be `Const(...)` or `Volatile(Const(...))` (the parser
// writes `const volatile T` as `Const(Volatile(T))`, and the other
// order is accepted too), but a volatile layer anywhere among the
// qualifiers rules out the fold, since by C99 6.7.3p6 each volatile
// access is a side effect, which reading an immediate would erase. The
// walk stops below the qualifiers, since `Pointer(Const(Int))` points
// to a const int and isn't itself const.
bool is_const_qualified(const c99::Type_data_type& t) {
  bool has_const = false;
  const c99::Type_data_type* cur = &t;
  while (true) {
    if (isa<c99::Volatile>(*cur)) {
      return false;
    }
    const auto* c = as<c99::Const>(*cur);
    if (c == nullptr) {
      return has_const;
    }
    has_const = true;
    cur = &*c->referenced_type;
  }
}

// Returns the bytes, low byte first, of the `n` low bytes of `v`.
Bytes int_bytes(const asdl::Int& v, int n) {
  Bytes out;
  for (int k = 0; k < n; ++k) {
    out.emplace_back(ir::Imm{.value = (v >> (8 * k)) & 0xFF});
  }
  return out;
}

// Returns the bytes, low byte first, of a `static_init` list that holds
// one foldable scalar, or nothing for any other list. A `CharInit` is 1
// byte, an `IntInit` 2, a `LongInit` and a `FloatInit` (the bits of an
// IEEE 754 single) 4, and a `LongLongInit` and a `DoubleInit` 8. An
// `AddressInit(name, k)` gives the halves of the address, `#<(name+k)`
// and `#>(name+k)`, so that `static const uint8_t *const S_X = MSG +
// 39;` reads as the address where it read S_X twice. Arrays, structs,
// `StringInit`, and `ZeroInit` aren't folded.
std::optional<Bytes> scalar_init_bytes(const std::vector<ir::Type_static_init>& init) {
  if (init.size() != 1) {
    return std::nullopt;
  }
  const ir::Type_static_init& item = init[0];
  using K = ir::Kind_static_init;
  switch (ir::kind(item)) {
    case K::AddressInit: {
      const auto& a = std::get<ir::AddressInit>(item);
      return Bytes{ir::ImmLabelLow{.name = a.name, .offset = a.offset},
                   ir::ImmLabelHigh{.name = a.name, .offset = a.offset}};
    }
    case K::CharInit:
      return int_bytes(std::get<ir::CharInit>(item).value, 1);
    case K::IntInit:
      return int_bytes(std::get<ir::IntInit>(item).value, 2);
    case K::LongInit:
      return int_bytes(std::get<ir::LongInit>(item).value, 4);
    case K::LongLongInit:
      return int_bytes(std::get<ir::LongLongInit>(item).value, 8);
    case K::FloatInit:
      return int_bytes(std::get<ir::FloatInit>(item).bits, 4);
    case K::DoubleInit:
      return int_bytes(std::get<ir::DoubleInit>(item).bits, 8);
    default:
      return std::nullopt;
  }
}

// Returns the candidates, by name, with their bytes. A candidate is each
// `StaticVariable` whose type is const-qualified, whose linkage is
// internal, and whose initializer is one foldable scalar.
Candidates collect_candidates(const ir::Program& prog, const SymbolTable& symbols) {
  Candidates out;
  for (const auto& tl : prog.top_level) {
    const auto* sv = as<ir::StaticVariable>(tl);
    if (sv == nullptr || sv->is_global) {
      continue;
    }
    const Symbol* sym = symbols.find(sv->name);
    if (sym == nullptr || !is_const_qualified(sym->type)) {
      continue;
    }
    if (auto bytes = scalar_init_bytes(sv->init)) {
      out.insert_or_assign(sv->name, std::move(*bytes));
    }
  }
  return out;
}

// Returns the operand `instr` writes, the destination of a write and not
// the read side of a read-modify-write, or null for an instruction that
// writes no operand. A `DecimalArith`'s writes are handled by its
// caller.
const ir::Type_operand* write_destination(const ir::Type_instruction& instr) {
  using K = ir::Kind_instruction;
  switch (ir::kind(instr)) {
    case K::Mov:
      return &std::get<ir::Mov>(instr).dst;
    case K::Add:
      return &std::get<ir::Add>(instr).dst;
    case K::Sub:
      return &std::get<ir::Sub>(instr).dst;
    case K::And:
      return &std::get<ir::And>(instr).dst;
    case K::Or:
      return &std::get<ir::Or>(instr).dst;
    case K::Xor:
      return &std::get<ir::Xor>(instr).dst;
    case K::Inc:
      return &std::get<ir::Inc>(instr).dst;
    case K::Dec:
      return &std::get<ir::Dec>(instr).dst;
    case K::ArithmeticShiftLeft:
      return &std::get<ir::ArithmeticShiftLeft>(instr).dst;
    case K::LogicalShiftRight:
      return &std::get<ir::LogicalShiftRight>(instr).dst;
    case K::RotateLeft:
      return &std::get<ir::RotateLeft>(instr).dst;
    case K::RotateRight:
      return &std::get<ir::RotateRight>(instr).dst;
    case K::Pop:
      return &std::get<ir::Pop>(instr).dst;
    default:
      return nullptr;
  }
}

// Returns every operand `instr` reads, leaving out the destinations,
// which `write_destination` gives.
std::vector<ir::Type_operand> operand_uses(const ir::Type_instruction& instr) {
  using K = ir::Kind_instruction;
  switch (ir::kind(instr)) {
    case K::DecimalArith:
      return decimal_arith::reads(std::get<ir::DecimalArith>(instr));
    case K::Mov:
      return {std::get<ir::Mov>(instr).src};
    case K::Add:
      return {std::get<ir::Add>(instr).src};
    case K::Sub:
      return {std::get<ir::Sub>(instr).src};
    case K::And:
      return {std::get<ir::And>(instr).src};
    case K::Or:
      return {std::get<ir::Or>(instr).src};
    case K::Xor: {
      const auto& x = std::get<ir::Xor>(instr);
      return {x.src1, x.src2};
    }
    case K::Push:
      return {std::get<ir::Push>(instr).src};
    case K::Compare: {
      const auto& c = std::get<ir::Compare>(instr);
      return {c.left, c.right};
    }
    default:
      return {};
  }
}

bool names_candidate(const ir::Type_operand& op, const Candidates& candidates,
                     std::string* name_out) {
  const std::string* name = nullptr;
  if (const auto* p = as<ir::Pseudo>(op)) {
    name = &p->name;
  }
  if (name == nullptr || !candidates.contains(*name)) {
    return false;
  }
  *name_out = *name;
  return true;
}

// Adds to `disqualified` each candidate that `instr` uses where it can't
// be folded. A read (the source of a `Mov` or an `Add`, a `Compare`) can
// be folded, and anything else rules the candidate out.
void scan_instr(const ir::Type_instruction& instr, const Candidates& candidates,
                Names& disqualified) {
  std::string name;
  // A `LoadAddress` whose source names a candidate takes its address.
  // That is the case of a frame source, since `tac_to_asm` lowers a
  // `LoadAddress` of a static to label halves, which the walk over the
  // operands below finds. A `LoadAddress` writes a temporary of 2
  // bytes, which is never a static.
  if (const auto* la = as<ir::LoadAddress>(instr)) {
    if (names_candidate(la->src, candidates, &name)) {
      disqualified.insert(name);
    }
    return;
  }
  if (const auto* d = as<ir::DecimalArith>(instr)) {
    for (const auto& op : decimal_arith::writes(*d)) {
      if (names_candidate(op, candidates, &name)) {
        disqualified.insert(name);
      }
    }
  }
  if (const ir::Type_operand* dst = write_destination(instr)) {
    if (names_candidate(*dst, candidates, &name)) {
      disqualified.insert(name);
    }
  }
  // An `IndexedData` on a candidate indexes through it, which makes
  // sense only for an array and can't be folded. An `ImmLabelLow` or
  // `ImmLabelHigh` on one is `&candidate` after `tac_to_asm` lowered the
  // `LoadAddress` of a static, whose address is resolved at link time, so
  // the storage has to stay.
  for (const auto& op : operand_uses(instr)) {
    const std::string* n = nullptr;
    if (const auto* ix = as<ir::IndexedData>(op)) {
      n = &ix->name;
    } else if (const auto* lo = as<ir::ImmLabelLow>(op)) {
      n = &lo->name;
    } else if (const auto* hi = as<ir::ImmLabelHigh>(op)) {
      n = &hi->name;
    }
    if (n != nullptr && candidates.contains(*n)) {
      disqualified.insert(*n);
    }
  }
}

// Returns the candidates whose address is taken, that something writes,
// or that something indexes through.
Names scan_disqualifying(const ir::Program& prog, const Candidates& candidates) {
  Names disqualified;
  for (const auto& tl : prog.top_level) {
    if (const auto* sv = as<ir::StaticVariable>(tl)) {
      for (const auto& item : sv->init) {
        const std::string* n = nullptr;
        if (const auto* a = as<ir::AddressInit>(item)) {
          n = &a->name;
        } else if (const auto* b = as<ir::AddressByteInit>(item)) {
          n = &b->name;
        }
        if (n != nullptr && candidates.contains(*n)) {
          disqualified.insert(*n);
        }
      }
    } else {
      for (const auto& instr : std::get<ir::Function>(tl).instructions) {
        scan_instr(instr, candidates, disqualified);
      }
    }
  }
  return disqualified;
}

// Returns `op` with a read of a surviving candidate replaced by its
// byte. An integer byte is an immediate wherever it goes. A label's half
// is an immediate only where `asm_emit` writes one (`LDA`, `ADC`, `SBC`,
// `CMP`), which `labels` says, and elsewhere the read stays, and so does
// the storage it reads.
//
// A `Data` read is one the TAC optimizer made of a load through the
// static's address, as `*p` after `p = &K` becomes `IndexedLoad(K, 0)`,
// which `tac_to_asm` lowers to `Data(K, 0)`.
ir::Type_operand sub(const ir::Type_operand& op, const Candidates& surviving, bool labels) {
  const std::string* name = nullptr;
  asdl::Int offset = 0;
  if (const auto* p = as<ir::Pseudo>(op)) {
    name = &p->name;
    offset = p->offset;
  } else if (const auto* d = as<ir::Data>(op)) {
    name = &d->name;
    offset = d->offset;
  }
  if (name == nullptr) {
    return op;
  }
  const auto it = surviving.find(*name);
  if (it == surviving.end() || offset < 0 || offset >= static_cast<asdl::Int>(it->second.size())) {
    return op;
  }
  const ir::Type_operand& byte = it->second[static_cast<std::size_t>(offset)];
  if (isa<ir::Imm>(byte)) {
    return byte;
  }
  return labels ? byte : op;
}

// Returns `instr` with each read of a surviving candidate replaced by
// its byte. The definitions are left alone.
ir::Type_instruction rewrite_instr(const ir::Type_instruction& instr, const Candidates& surviving) {
  using K = ir::Kind_instruction;
  auto s = [&](const ir::Type_operand& op, bool labels = true) {
    return sub(op, surviving, labels);
  };
  switch (ir::kind(instr)) {
    case K::DecimalArith:
      return decimal_arith::map_reads(std::get<ir::DecimalArith>(instr),
                                      [&](const ir::Type_operand& op) { return s(op, false); });
    case K::Mov: {
      const auto& m = std::get<ir::Mov>(instr);
      return ir::Mov{.src = s(m.src), .dst = m.dst, .is_volatile = m.is_volatile};
    }
    case K::Add: {
      const auto& m = std::get<ir::Add>(instr);
      return ir::Add{.src = s(m.src), .dst = m.dst};
    }
    case K::Sub: {
      const auto& m = std::get<ir::Sub>(instr);
      return ir::Sub{.src = s(m.src), .dst = m.dst};
    }
    case K::And: {
      const auto& m = std::get<ir::And>(instr);
      return ir::And{.src = s(m.src, false), .dst = m.dst};
    }
    case K::Or: {
      const auto& m = std::get<ir::Or>(instr);
      return ir::Or{.src = s(m.src, false), .dst = m.dst};
    }
    case K::Xor: {
      const auto& x = std::get<ir::Xor>(instr);
      return ir::Xor{.src1 = s(x.src1, false), .src2 = s(x.src2, false), .dst = x.dst};
    }
    case K::Push:
      return ir::Push{.src = s(std::get<ir::Push>(instr).src, false)};
    case K::Compare: {
      const auto& c = std::get<ir::Compare>(instr);
      return ir::Compare{.left = s(c.left), .right = s(c.right)};
    }
    default:
      return instr;
  }
}

// Adds the name of each operand of `instr` that names an object
// (`Pseudo`, `Data`, `IndexedData`, and a label's halves) to `out`.
// Those are the operands `all_operands_in` gives, with a `Phi`'s
// argument sources, and a `BitTest`'s source.
void names_in(const ir::Type_instruction& instr, Names& out) {
  auto add = [&](const ir::Type_operand& op) {
    using K = ir::Kind_operand;
    switch (ir::kind(op)) {
      case K::Pseudo:
        out.insert(std::get<ir::Pseudo>(op).name);
        break;
      case K::Data:
        out.insert(std::get<ir::Data>(op).name);
        break;
      case K::IndexedData:
        out.insert(std::get<ir::IndexedData>(op).name);
        break;
      case K::ImmLabelLow:
        out.insert(std::get<ir::ImmLabelLow>(op).name);
        break;
      case K::ImmLabelHigh:
        out.insert(std::get<ir::ImmLabelHigh>(op).name);
        break;
      default:
        break;
    }
  };
  if (const auto* bit = as<ir::BitTest>(instr)) {
    add(bit->src);
    return;
  }
  for (const ir::Type_operand* op : optimizer::all_operands_in(instr)) {
    add(*op);
  }
}

// Returns `prog` with the reads of the surviving candidates rewritten in
// every function, and without the surviving statics. A static that
// something still names after the rewrite, in an instruction
// `rewrite_instr` doesn't rewrite, keeps its storage.
ir::Program rewrite_program(ir::Program prog, const Candidates& surviving) {
  Names named;
  for (auto& tl : prog.top_level) {
    if (auto* fn = as<ir::Function>(tl)) {
      for (auto& instr : fn->instructions) {
        instr = rewrite_instr(instr, surviving);
        names_in(instr, named);
      }
    }
  }
  std::vector<ir::Type_top_level> top;
  top.reserve(prog.top_level.size());
  for (auto& tl : prog.top_level) {
    if (const auto* sv = as<ir::StaticVariable>(tl)) {
      if (surviving.contains(sv->name) && !named.contains(sv->name)) {
        continue;
      }
    }
    top.push_back(std::move(tl));
  }
  prog.top_level = std::move(top);
  return prog;
}

}  // namespace

asm_ast::Program fold_const_statics(asm_ast::Program prog, const SymbolTable* symbols) {
  if (symbols == nullptr) {
    return prog;
  }
  const Candidates candidates = collect_candidates(prog, *symbols);
  if (candidates.empty()) {
    return prog;
  }
  const Names disqualified = scan_disqualifying(prog, candidates);
  Candidates surviving;
  for (const auto& [name, bytes] : candidates) {
    if (!disqualified.contains(name)) {
      surviving.insert_or_assign(name, bytes);
    }
  }
  if (surviving.empty()) {
    return prog;
  }
  return rewrite_program(std::move(prog), surviving);
}

}  // namespace c6502::passes
