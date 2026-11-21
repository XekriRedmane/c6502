/* TAC pass that recognizes the indirect-indexed access `(zp),Y`.

For `ptr[i]`, where `ptr` is a pointer local (or a zp_abi parameter)
and `i` is a 1-byte integer, c99_to_tac lowers to

    ZeroExtend(i,            %ext)        # i uchar to 2 bytes
    Binary(Add, ptr, %ext,   %addr)       # 16-bit pointer arithmetic
    Load(%addr,              dst)         # read 1 byte

The 6502's indirect-indexed addressing mode `(zp),Y` does the same
`ptr + Y` arithmetic for free, with `Y = i`. So when `ptr` is (or ends
up at) a zero-page address, this collapses to

    LDY i ; LDA (ptr_zp),Y ; STA dst        (about 3 instructions and 5 bytes)

against about 10 instructions and 21 bytes for the 16-bit Add, the
setup of DPTR, and an indirect-Y read with Y=0. Even when ptr can't
live in zero page, lowering through DPTR with `Y=i` (in place of `Y=0`
and a sum computed first) lets the asm forward copy propagation hoist
the loop-invariant setup of DPTR out of an unrolled body, which the
other shape prevents because DPTR then holds the per-iteration index.

This pass finds the three instructions and rewrites them to one
`IndirectIndexedLoad(ptr, index, dst)` (or the Store's mirror).

A constant offset `Binary(Add, ptr, k)` with `0 <= k <= 255` feeding
the Load or Store is recognized too (`const_offset_chain`), with the
index a `ConstUChar(k)`. That is the shape of `p[10]` and of a byte
member read through a pointer, and it lowers to `LDY #k; LDA
(DPTR),Y`. Its address may be read by several 1-byte Loads and Stores
(`all_byte_accesses`), as in `p[1]++`, and each is rewritten. With a
constant offset the value may be wider than a byte, as a 2-byte member
`p->gold` is, when its last byte is at most 255 from `ptr`
(`access_width`). It lowers as `LDY #k; LDA (DPTR),Y`, then `LDY #k+1`
(which the asm peepholes make `INY`), and so on, in place of an Add
that computes `ptr + k` into a temporary and stages it in DPTR. The
recognizer doesn't need to know whether ptr will end up in zero page,
which tac_to_asm and the asm register allocation decide.

# Eligibility

The fusion fires when these all hold.

  * The Load's `src_ptr` (or the Store's `dst_ptr`) is a temporary
    `%addr` read once and defined by an `Add` of a Var `ptr` and
    another temporary `%ext`.
  * `ptr` is a Var. A Constant is the case of `recognize_indexed_load`
    and `recognize_indexed_store`.
  * `%ext` is defined by `ZeroExtend(index_var, %ext)`.
  * `index_var`'s c99 type is 1 byte (`char`, `signed char`, or
    `unsigned char`). The high byte of the zero-extended index is zero,
    so setting `Y = index_var` is the same as adding `index_var` to the
    pointer's low byte and propagating the carry, which is what `(zp),Y`
    does.
  * The Load's `dst` (or the Store's `src`) is a 1-byte value, or, for a
    constant index `k`, a value of `w` bytes with `k + w - 1 <= 255`,
    so that Y reaches every byte.

The c99 type of `ptr` isn't checked. The type checker has already
verified that the Binary is pointer arithmetic, since its result feeds
a Load or a Store, and the only operands such an Add takes are a
pointer and an integer. The Multiply that pointer arithmetic emits for
`sizeof(pointee) > 1` would break the shape, since the Add would read
the Multiply's result in place of the ZeroExtend's, so the scale of 1
follows from the structural match.

# Soundness

The checks on the readers mean no other reader sees the intermediate
values, so removing the temporaries doesn't change what the program
computes. The high byte of `ZeroExtend(uchar, %ext)` is zero, so `ptr +
ZeroExtend(i)` equals `ptr + i` as a 16-bit sum, which is `*(ptr),Y`
with `Y = i` on the 6502, since that adds Y to the low byte and carries
into the high byte. The constraint on the width of the access matches
the single LDA or STA the lowering emits.

# Identity

The Python pass finds the access's index in the instructions by
identity, which gives the first position of an object that sits at two.
This port takes the index at which the walk calls the rewrite, which is
the same for every access that isn't shared with an earlier position. */
#include "passes/optimization/recognize_indirect_indexed.h"

#include <set>
#include <tuple>

#include "bit_precise.h"
#include "passes/optimization/extend_add_chain.h"
#include "passes/optimization/narrow_widened_arith.h"
#include "passes/optimization/split_tables.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::tac_ast;
namespace c99 = ::c6502::c99_ast;
using asdl::Int;
using framework::DefUseEnv;
using framework::DefUseRewrite;
using framework::Instrs;
using framework::PassContext;

// The access the pass matched: a `Load(src_ptr=Var, dst=Var)` or a
// `Store(src, dst_ptr=Var)`, its address, and its index.
struct Access {
  const ir::Load* load = nullptr;
  const ir::Store* store = nullptr;
  const ir::Var* addr = nullptr;
  std::size_t at = 0;
};

bool is_var(const ir::Type_val& v, const ir::Var& var) {
  const auto* p = std::get_if<ir::Var>(&v);
  return p != nullptr && p->name == var.name;
}

// Returns whether `v` is a Var of a 1-byte type. Every Load's
// destination is a Var.
bool is_1_byte_dst(const ir::Type_val& v, const SymbolTable* symbols) {
  const auto* var = std::get_if<ir::Var>(&v);
  return var != nullptr && is_1_byte_var(*var, symbols);
}

// Returns the integer `value` of the constant `v`, or none when `v`
// isn't a constant or is a floating one.
std::optional<Int> int_constant(const ir::Type_val& v) {
  const auto* c = std::get_if<ir::Constant>(&v);
  if (c == nullptr) {
    return std::nullopt;
  }
  return bit_precise::raw_value(c->const_);
}

// Returns the width in bytes of the scalar value `v`, a Var by its c99
// type or a Constant by its class, or none.
std::optional<int> access_width(const ir::Type_val& v, const SymbolTable* symbols) {
  if (const auto* c = std::get_if<ir::Constant>(&v)) {
    using CK = ir::Kind_const;
    switch (ir::kind(c->const_)) {
      case CK::ConstChar:
      case CK::ConstUChar:
        return 1;
      case CK::ConstInt:
      case CK::ConstUInt:
        return 2;
      case CK::ConstLong:
      case CK::ConstULong:
      case CK::ConstFloat:
        return 4;
      case CK::ConstLongLong:
      case CK::ConstULongLong:
      case CK::ConstDouble:
        return 8;
      default:
        return std::nullopt;
    }
  }
  const auto& var = std::get<ir::Var>(v);
  if (symbols == nullptr) {
    return std::nullopt;
  }
  const Symbol* sym = symbols->find(var.name);
  if (sym == nullptr) {
    return std::nullopt;
  }
  const c99::Type_data_type* t = &sym->type;
  for (;;) {
    if (const auto* c = std::get_if<c99::Const>(t)) {
      t = &*c->referenced_type;
    } else if (const auto* vol = std::get_if<c99::Volatile>(t)) {
      t = &*vol->referenced_type;
    } else {
      break;
    }
  }
  return split_tables::width(*t);
}

// Returns whether `v` is wider than a byte and `index` is a constant
// offset that reaches all of its bytes in Y, as a C member access at a
// constant offset through a pointer does.
bool fits_constant_offset(const ir::Type_val& index, const ir::Type_val& v,
                          const SymbolTable* symbols) {
  const auto k = int_constant(index);
  if (!k) {
    return false;
  }
  const auto w = access_width(v, symbols);
  return w && *w > 1 && *k + *w - 1 <= 0xFF;
}

// Returns whether `v`'s SSA definitions end in a Constant, through
// Copies of any depth, so that the pass leaves the access to the
// IndexedLoad and IndexedStore recognizers when the pointer is only a
// Constant in a Var. Only an SSA-renamed name has one definition. The
// index of definitions gives a global's last store, and after `P = 0`
// on one path, `P->m` elsewhere had been left to those recognizers,
// which took it as an address computed at run time.
bool resolves_to_constant(const ir::Var& v, const DefUseEnv& env, const PassContext& ctx) {
  std::set<std::string, std::less<>> seen;
  const ir::Var* cur = &v;
  for (;;) {
    if (seen.contains(cur->name) || !ctx.is_ssa(cur->name)) {
      return false;
    }
    seen.insert(cur->name);
    const auto it = env.def_idx.find(cur->name);
    if (it == env.def_idx.end()) {
      return false;
    }
    const auto& d = env.instructions[it->second];
    const auto* copy = d ? std::get_if<ir::Copy>(&*d) : nullptr;
    if (copy == nullptr) {
      return false;
    }
    if (std::holds_alternative<ir::Constant>(copy->src)) {
      return true;
    }
    cur = &std::get<ir::Var>(copy->src);
  }
}

// Returns the number of times `instr` reads `name`.
std::size_t reads_of(const ir::Type_instruction& instr, std::string_view name) {
  std::size_t n = 0;
  for (const auto u : uses_in(instr)) {
    if (u == name) {
      ++n;
    }
  }
  return n;
}

// Returns the index of the one reader of `instr`'s destination when
// `instr` is `Binary(Add, addr, k)` (in either order) with an integer
// constant `k` outside 0 to 255, its destination is SSA-renamed, and
// that reader is a 1-byte `Load` from the destination or `Store` to it.
// Returns -1 when that access has been rewritten already in this run
// and nothing reads the destination, and none otherwise.
std::optional<std::ptrdiff_t> offset_access(const ir::Type_instruction& instr,
                                            const ir::Var& addr, const DefUseEnv& env,
                                            const PassContext& ctx, const UseCounts& uses) {
  const auto* binary = std::get_if<ir::Binary>(&instr);
  if (binary == nullptr || !std::holds_alternative<ir::Add>(binary->op)) {
    return std::nullopt;
  }
  const ir::Type_val& other = is_var(binary->src1, addr) ? binary->src2 : binary->src1;
  const auto value = int_constant(other);
  if (!value || (0 <= *value && *value <= 0xFF)) {
    return std::nullopt;
  }
  const auto* dst = std::get_if<ir::Var>(&binary->dst);
  if (dst == nullptr || !ctx.is_ssa(dst->name) || use_count(uses, dst->name) != 1) {
    return std::nullopt;
  }
  for (std::size_t i = 0; i < env.instructions.size(); ++i) {
    const auto& entry = env.instructions[i];
    if (!entry || reads_of(*entry, dst->name) == 0) {
      continue;
    }
    if (const auto* load = std::get_if<ir::Load>(&*entry); load && is_var(load->src_ptr, *dst)) {
      if (is_1_byte_dst(load->dst, ctx.symbols)) {
        return static_cast<std::ptrdiff_t>(i);
      }
      return std::nullopt;
    }
    if (const auto* store = std::get_if<ir::Store>(&*entry);
        store && is_var(store->dst_ptr, *dst)) {
      if (!is_var(store->src, *dst) && is_1_byte_val(store->src, ctx.symbols)) {
        return static_cast<std::ptrdiff_t>(i);
      }
      return std::nullopt;
    }
    return std::nullopt;
  }
  // The access has been rewritten already, so nothing reads the Add.
  return -1;
}

// Returns whether every read of `addr` is the pointer of a `Load` into
// a 1-byte Var or of a `Store` of a 1-byte value, and `ptr` still holds
// at each one the value the Add read. With the constant `index` the Add
// adds, a wider value whose bytes Y reaches counts too
// (`fits_constant_offset`), as in `p->gold += 5`. Then every reader
// becomes an indexed access, and nothing reads the Add. A `ptr` that
// isn't SSA-renamed, such as a global, must be unchanged from the Add
// to each access (`var_unchanged`). With `offsets`, a reader may also be
// `addr + k` for a constant `k` outside 0 to 255 whose one reader is
// such an access, which `offset_from_extend_chain` rewrites.
bool all_byte_accesses(const ir::Var& addr, const ir::Type_val& ptr, const DefUseEnv& env,
                       const PassContext& ctx, const UseCounts& uses, bool offsets,
                       const ir::Type_val* index) {
  const auto add_it = env.def_idx.find(addr.name);
  if (add_it == env.def_idx.end()) {
    return false;
  }
  const std::size_t add_idx = add_it->second;
  const auto* ptr_var = std::get_if<ir::Var>(&ptr);
  std::size_t seen = 0;
  for (std::size_t i = 0; i < env.instructions.size(); ++i) {
    const auto& entry = env.instructions[i];
    if (!entry) {
      continue;
    }
    const std::size_t reads = reads_of(*entry, addr.name);
    if (reads == 0) {
      continue;
    }
    bool ok = false;
    const auto* load = std::get_if<ir::Load>(&*entry);
    const auto* store = std::get_if<ir::Store>(&*entry);
    if (load != nullptr && is_var(load->src_ptr, addr)) {
      ok = is_1_byte_dst(load->dst, ctx.symbols) ||
           (index != nullptr && fits_constant_offset(*index, load->dst, ctx.symbols));
    } else if (store != nullptr && is_var(store->dst_ptr, addr)) {
      ok = !is_var(store->src, addr) &&
           (is_1_byte_val(store->src, ctx.symbols) ||
            (index != nullptr && fits_constant_offset(*index, store->src, ctx.symbols)));
    } else if (offsets) {
      const auto at = offset_access(*entry, addr, env, ctx, uses);
      ok = at.has_value();
      if (ok && *at >= 0 && ptr_var != nullptr &&
          !var_unchanged(*ptr_var, add_idx, static_cast<std::size_t>(*at), env, ctx)) {
        return false;
      }
    }
    if (!ok || reads != 1) {
      return false;
    }
    if (ptr_var != nullptr && !var_unchanged(*ptr_var, add_idx, i, env, ctx)) {
      return false;
    }
    ++seen;
  }
  // `env.instructions` holds the rewrites made so far in this run, so an
  // access already rewritten no longer reads `addr`.
  return seen > 0;
}

// Returns the `ExtendAddChain` of `addr = ptr + ZeroExtend(i)` when
// `addr` has several readers and each is a 1-byte `Load` from it or
// `Store` to it (`all_byte_accesses`), as in `p[i] &= m`, which reads
// and writes `p + i`. Each reader becomes `(p),Y` with Y = `i`, so `i`
// must be SSA-renamed to hold the Add's value at each one.
std::optional<ExtendAddChain> shared_extend_add_chain(const ir::Var& addr, const DefUseEnv& env,
                                                      const PassContext& ctx,
                                                      const UseCounts& uses) {
  if (use_count(uses, addr.name) < 2) {
    return std::nullopt;
  }
  auto chain = recognize_extend_add_chain(addr, env, ctx, uses, /*single_use=*/false);
  if (!chain || !ctx.is_ssa(chain->idx_var.name)) {
    return std::nullopt;
  }
  if (!all_byte_accesses(addr, chain->base, env, ctx, uses, /*offsets=*/true, nullptr)) {
    return std::nullopt;
  }
  return chain;
}

// The pointer, the index, and the instructions to put before the access
// that a rewrite reads through.
struct Moved {
  ir::Type_val ptr;
  ir::Type_val index;
  Instrs pre;
};

// Returns `(t, i, [Binary(Add, p, k, t)])` when the access reads or
// writes through `q + k`, its one reader, with `k` an integer constant
// that doesn't fit in a byte and `q = p + ZeroExtend(i)`, whose other
// readers, if any, all become `(p),Y` accesses
// (`shared_extend_add_chain`). The access is then `(t),Y` with Y = `i`
// and `t = p + k`, a pointer plus a constant, and `q` itself goes once
// its other readers are rewritten. A `k` of 0 to 255 is left to
// `const_offset_chain`, which reads `(q),Y` with Y = `k`, since several
// such offsets share `q` where each would need a pointer of its own
// here. `background[x]` with `background = line + 0x2000`, which CSE
// had turned into `(line + x) + 0x2000` to share `line + x` with
// `line[x]`, reads page 2 through its own pointer. `p` must hold at the
// access the value `q`'s Add read. The temporary `t` is registered even
// when the rewrite later declines, as the Python pass registers it.
std::optional<Moved> offset_from_extend_chain(const Access& m, const DefUseEnv& env,
                                              const PassContext& ctx, const UseCounts& uses) {
  if (ctx.symbols == nullptr || ctx.ssa_dsts == nullptr) {
    return std::nullopt;
  }
  const ir::Var& addr = *m.addr;
  if (!ctx.is_ssa(addr.name) || use_count(uses, addr.name) != 1) {
    return std::nullopt;
  }
  const ir::Type_instruction* def = env.def_of(addr);
  const auto* outer = def != nullptr ? std::get_if<ir::Binary>(def) : nullptr;
  if (outer == nullptr || !std::holds_alternative<ir::Add>(outer->op)) {
    return std::nullopt;
  }
  const std::pair<const ir::Type_val*, const ir::Type_val*> orders[] = {
      {&outer->src1, &outer->src2}, {&outer->src2, &outer->src1}};
  for (const auto& [qv, kv] : orders) {
    const auto* q = std::get_if<ir::Var>(qv);
    if (q == nullptr || !std::holds_alternative<ir::Constant>(*kv) || !ctx.is_ssa(q->name)) {
      continue;
    }
    const auto value = int_constant(*kv);
    if (!value) {
      continue;
    }
    if (0 <= *value && *value <= 0xFF) {
      // `(q),Y` with Y = k reads it without a second pointer.
      continue;
    }
    std::optional<ExtendAddChain> chain;
    if (use_count(uses, q->name) >= 2) {
      chain = shared_extend_add_chain(*q, env, ctx, uses);
    } else {
      chain = recognize_extend_add_chain(*q, env, ctx, uses);
      if (chain && !ctx.is_ssa(chain->idx_var.name)) {
        chain.reset();
      }
    }
    if (!chain) {
      continue;
    }
    const auto* base = std::get_if<ir::Var>(&chain->base);
    if (base == nullptr || resolves_to_constant(*base, env, ctx)) {
      continue;
    }
    const auto q_it = env.def_idx.find(q->name);
    if (q_it == env.def_idx.end() || !var_unchanged(*base, q_it->second, m.at, env, ctx)) {
      continue;
    }
    const std::string name = addr.name + ".base";
    narrow_widened_arith::register_temp(ctx, base->name, name);
    const ir::Var t{.name = name};
    Instrs pre;
    pre.push_back(ir::Binary{.op = ir::Add{}, .src1 = *base, .src2 = *kv, .dst = t});
    return Moved{.ptr = t, .index = chain->idx_var, .pre = std::move(pre)};
  }
  return std::nullopt;
}

// Returns `(ptr, Constant(k))` when `addr` is an SSA name defined by
// `Binary(Add, ptr, k)` (in either order) with an integer constant `0
// <= k <= 255`. The access then lowers as `LDY #k; LDA (DPTR),Y`,
// because `(zp),Y` adds Y to the pointer with a carry into the high
// byte, which is the 16-bit sum the Add computed. The index comes back
// as a `ConstUChar`, so that it meets the indexed instructions' contract
// of a 1-byte index. `addr` may have several readers when each one is a
// 1-byte `Load` from it or `Store` to it (`all_byte_accesses`), since
// each is rewritten the same way. `p[1]++` reads and writes `p + 1`.
std::optional<std::pair<ir::Type_val, ir::Type_val>> const_offset_chain(const ir::Var& addr,
                                                                        const DefUseEnv& env,
                                                                        const PassContext& ctx,
                                                                        const UseCounts& uses) {
  if (ctx.symbols == nullptr || ctx.ssa_dsts == nullptr || !ctx.is_ssa(addr.name)) {
    return std::nullopt;
  }
  const ir::Type_instruction* def = env.def_of(addr);
  const auto* binary = def != nullptr ? std::get_if<ir::Binary>(def) : nullptr;
  if (binary == nullptr || !std::holds_alternative<ir::Add>(binary->op)) {
    return std::nullopt;
  }
  const std::pair<const ir::Type_val*, const ir::Type_val*> orders[] = {
      {&binary->src1, &binary->src2}, {&binary->src2, &binary->src1}};
  for (const auto& [ptr, k] : orders) {
    if (!std::holds_alternative<ir::Constant>(*k) || std::holds_alternative<ir::Constant>(*ptr)) {
      continue;
    }
    const auto value = int_constant(*k);
    if (!value || !(0 <= *value && *value <= 0xFF)) {
      continue;
    }
    const ir::Type_val index = ir::Constant{.const_ = ir::ConstUChar{.value = *value}};
    if (use_count(uses, addr.name) != 1 &&
        !all_byte_accesses(addr, *ptr, env, ctx, uses, /*offsets=*/false, &index)) {
      return std::nullopt;
    }
    return std::pair{*ptr, index};
  }
  return std::nullopt;
}

// Returns the largest value the 1-byte `v` can hold, from its
// definition: a constant, a left shift by a constant (whose low bits are
// 0), a logical right shift or an AND with a constant, or a copy of one
// of those. 255 for anything else.
Int byte_max(const ir::Type_val& v, const DefUseEnv& env, int depth = 0) {
  if (const auto* c = std::get_if<ir::Constant>(&v)) {
    const auto value = bit_precise::raw_value(c->const_);
    return value ? asdl::floor_mod(*value, 256) : Int(0xFF);
  }
  const auto* var = std::get_if<ir::Var>(&v);
  if (var == nullptr || depth > 8) {
    return 0xFF;
  }
  const ir::Type_instruction* d = env.def_of(*var);
  if (d == nullptr) {
    return 0xFF;
  }
  if (const auto* copy = std::get_if<ir::Copy>(d)) {
    return byte_max(copy->src, env, depth + 1);
  }
  const auto* binary = std::get_if<ir::Binary>(d);
  if (binary == nullptr || !std::holds_alternative<ir::Constant>(binary->src2)) {
    return 0xFF;
  }
  const auto k = int_constant(binary->src2);
  const bool is_and = std::holds_alternative<ir::BitwiseAnd>(binary->op);
  if (!k || !(0 <= *k && *k < 8)) {
    if (is_and && k) {
      return asdl::floor_mod(*k, 256);
    }
    return 0xFF;
  }
  const int shift = static_cast<int>(*k);
  if (std::holds_alternative<ir::LeftShift>(binary->op)) {
    return (0xFF << shift) & 0xFF;
  }
  if (std::holds_alternative<ir::RightShift>(binary->op)) {
    return 0xFF >> shift;
  }
  if (is_and) {
    return *k;
  }
  return 0xFF;
}

// Returns `(base, t, [Binary(Add, b, k, t)])` when `ptr`, the pointer an
// access reads at the constant offset `k`, is `base + ZeroExtend(b)` for
// a 1-byte `b` whose largest value plus `k` fits in a byte, and `base`
// still holds at the access the value the Add read. Then `ptr + k` is
// `base + (b + k)` with no carry out of the byte sum, and the access is
// `(base),Y` with Y = `b + k`. In Zork's abbreviation lookup
// `Z_ABBREV_TABLE[i + 1]` with `i = z << 1`, which is even, became `INY`
// after the read of `Z_ABBREV_TABLE[i]`, where it had added `i` to the
// table's address in a zero-page pair. The temporary `t` is registered
// even when the rewrite later declines, as the Python pass registers it.
std::optional<Moved> byte_index_sum(const ir::Type_val& ptr_val, const ir::Type_val& k,
                                    const Access& m, const DefUseEnv& env, const PassContext& ctx,
                                    const UseCounts& uses) {
  if (ctx.symbols == nullptr || ctx.ssa_dsts == nullptr) {
    return std::nullopt;
  }
  const auto* ptr = std::get_if<ir::Var>(&ptr_val);
  if (ptr == nullptr || !ctx.is_ssa(ptr->name)) {
    return std::nullopt;
  }
  // Only when the access is the sum's one reader, so that its 16-bit add
  // goes. `entry += stride; ... entry[0]` in the loop of
  // `match_dictionary_word` still reads the new `entry`, and reading
  // through the old one at Y = stride kept both alive.
  const Int kv = std::get<ir::ConstUChar>(std::get<ir::Constant>(k).const_).value;
  if (kv == 0 || use_count(uses, ptr->name) != 1) {
    return std::nullopt;
  }
  const ir::Type_instruction* def = env.def_of(*ptr);
  const auto* add = def != nullptr ? std::get_if<ir::Binary>(def) : nullptr;
  if (add == nullptr || !std::holds_alternative<ir::Add>(add->op)) {
    return std::nullopt;
  }
  const std::pair<const ir::Type_val*, const ir::Type_val*> orders[] = {
      {&add->src1, &add->src2}, {&add->src2, &add->src1}};
  for (const auto& [bv, ev] : orders) {
    const auto* base = std::get_if<ir::Var>(bv);
    const auto* ext = std::get_if<ir::Var>(ev);
    if (base == nullptr || ext == nullptr || !ctx.is_ssa(ext->name)) {
      continue;
    }
    const ir::Type_instruction* zd = env.def_of(*ext);
    const auto* z = zd != nullptr ? std::get_if<ir::ZeroExtend>(zd) : nullptr;
    const auto* b = z != nullptr ? std::get_if<ir::Var>(&z->src) : nullptr;
    if (b == nullptr || !ctx.is_ssa(b->name) || !is_1_byte_var(*b, ctx.symbols)) {
      continue;
    }
    if (byte_max(*b, env) + kv > 0xFF) {
      continue;
    }
    const auto add_it = env.def_idx.find(ptr->name);
    if (add_it == env.def_idx.end() || !var_unchanged(*base, add_it->second, m.at, env, ctx)) {
      continue;
    }
    const std::string name = ptr->name + ".y" + asdl::int_to_string(kv);
    narrow_widened_arith::register_temp(ctx, b->name, name);
    const ir::Var t{.name = name};
    Instrs pre;
    pre.push_back(ir::Binary{.op = ir::Add{}, .src1 = *b, .src2 = k, .dst = t});
    return Moved{.ptr = *base, .index = t, .pre = std::move(pre)};
  }
  return std::nullopt;
}

// Returns the `Rewrite` of the matched access as an indexed one through
// `ptr` at `index`, after the instructions `pre`, dropping the
// definitions of `drop`, or none.
std::optional<DefUseRewrite> finish(const Access& m, const PassContext& ctx,
                                    const ir::Type_val& ptr, const ir::Type_val& index,
                                    std::vector<std::string> drop, Instrs pre,
                                    const DefUseEnv& env) {
  // (zp),Y takes a pointer Var as its base. A Constant base is the
  // absolute,X case of recognize_indexed_store and recognize_indexed_load.
  const auto* ptr_var = std::get_if<ir::Var>(&ptr);
  if (ptr_var == nullptr) {
    return std::nullopt;
  }
  // Those recognizers take the access when the pointer resolves to a
  // Constant.
  if (resolves_to_constant(*ptr_var, env, ctx)) {
    return std::nullopt;
  }
  ir::Type_instruction replacement;
  if (m.load != nullptr) {
    if (!(is_1_byte_dst(m.load->dst, ctx.symbols) ||
          fits_constant_offset(index, m.load->dst, ctx.symbols))) {
      return std::nullopt;
    }
    replacement = ir::IndirectIndexedLoad{
        .ptr = *ptr_var, .index = index, .dst = m.load->dst, .is_volatile = m.load->is_volatile};
  } else {
    if (!(is_1_byte_val(m.store->src, ctx.symbols) ||
          fits_constant_offset(index, m.store->src, ctx.symbols))) {
      return std::nullopt;
    }
    replacement = ir::IndirectIndexedStore{
        .ptr = *ptr_var, .index = index, .src = m.store->src, .is_volatile = m.store->is_volatile};
  }
  // The intermediate definitions (the Add, and the ZeroExtend when there
  // is one) go with the rewrite, so no later pass of dead-store
  // elimination is needed.
  if (pre.empty()) {
    return DefUseRewrite::dropping(std::move(replacement), std::move(drop));
  }
  DefUseRewrite r;
  r.replacement = std::move(pre);
  r.replacement.push_back(std::move(replacement));
  r.is_list = true;
  r.is_rewrite = true;
  r.drop_defs = std::move(drop);
  return r;
}

// The rewrite of one access, which walks back through the SSA
// definitions to the ZeroExtend and the Add, or to the Add of a
// constant. The use and its producers need not be adjacent.
std::optional<DefUseRewrite> rewrite_access(const Access& m, const DefUseEnv& env,
                                            const PassContext& ctx, const UseCounts& uses) {
  const ir::Var& addr = *m.addr;
  const auto chain = recognize_extend_add_chain(addr, env, ctx, uses);
  if (chain) {
    return finish(m, ctx, chain->base, chain->idx_var, chain->drop_defs(), {}, env);
  }
  if (const auto shared = shared_extend_add_chain(addr, env, ctx, uses)) {
    // The Add and the extend stay for the readers not rewritten yet.
    // Once every one is, nothing reads them, and the asm byte DCE drops
    // them.
    return finish(m, ctx, shared->base, shared->idx_var, {}, {}, env);
  }
  if (auto moved = offset_from_extend_chain(m, env, ctx, uses)) {
    return finish(m, ctx, moved->ptr, moved->index, {}, std::move(moved->pre), env);
  }
  const auto const_chain = const_offset_chain(addr, env, ctx, uses);
  if (!const_chain) {
    return std::nullopt;
  }
  const auto& [ptr, index] = *const_chain;
  // With several readers, the Add stays for the ones not rewritten yet,
  // which find their chain through it. Once all of them are, nothing
  // reads it, and the asm byte DCE drops it.
  std::vector<std::string> drop;
  if (use_count(uses, addr.name) == 1) {
    drop.push_back(addr.name);
  }
  if (auto folded = byte_index_sum(ptr, index, m, env, ctx, uses)) {
    return finish(m, ctx, folded->ptr, folded->index, std::move(drop), std::move(folded->pre),
                  env);
  }
  return finish(m, ctx, ptr, index, std::move(drop), {}, env);
}

}  // namespace

ir::Function recognize_indirect_indexed(ir::Function fn, const PassContext& ctx) {
  const UseCounts uses = count_uses(fn.instructions);
  // The walk calls the rewrite once for each instruction in order, so
  // the count of calls is the instruction's index.
  std::size_t next = 0;
  return framework::run_def_use(
      std::move(fn), ctx, [&](const ir::Type_instruction& instr, const DefUseEnv& env) {
        Access m;
        m.at = next++;
        if (const auto* load = std::get_if<ir::Load>(&instr)) {
          m.addr = std::get_if<ir::Var>(&load->src_ptr);
          if (m.addr == nullptr || !std::holds_alternative<ir::Var>(load->dst)) {
            return std::optional<DefUseRewrite>();
          }
          m.load = load;
        } else if (const auto* store = std::get_if<ir::Store>(&instr)) {
          m.addr = std::get_if<ir::Var>(&store->dst_ptr);
          if (m.addr == nullptr) {
            return std::optional<DefUseRewrite>();
          }
          m.store = store;
        } else {
          return std::optional<DefUseRewrite>();
        }
        return rewrite_access(m, env, ctx, uses);
      });
}

}  // namespace c6502::passes
