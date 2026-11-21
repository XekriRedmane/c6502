/* Looks up the address of a row of a 2-D array in a table of row
addresses.

`A[i][j]` on an array of N rows of R bytes, more than 255 bytes in all,
computes `&A + i * R` with 16-bit arithmetic. When R isn't a power of
two, that's two 16-bit shifts and an add or subtract (or a call of
`mul16`), and then a 16-bit add of the array's address, about 40 bytes
for each access. 6502 programmers look the row up in a table instead,
with its addresses' low bytes in one half and their high bytes in the
other:

    LDX i; LDA rows,X; STA p; LDA rows+N+1,X; STA p+1

and then read `(p),Y`. This pass does the same. For each `&A + i * R`,
with `i` an unsigned byte, plus or minus a constant `k`, it makes the
pointer `IndexedLoad(A@rows, i, p, offset=k, plane=N+1)`, which reads
byte j of the row's address at `A@rows + k + i + j * (N + 1)`.

The table holds N+1 addresses, `A`, `A+R`, ..., `A+N*R`, so a pointer
one past the last row works too. C99 §6.5.6.8 makes any other index
undefined, so the table needs no more. For an array with a fixed
address (`[[c6502::address(addr)]]`) the bytes are numbers. For any
other array they're link-time address bytes, `DC.B <(A+R*r)` and `DC.B
>(A+R*r)`.

The pass runs once, over the whole program, on the TAC from
`c99_to_tac`, before the TAC optimizer, whose strength reduction would
otherwise turn the multiply into shifts. It doesn't run in the
unoptimized pipeline.

The shape it looks for, with every temporary defined once and read once
in the function:

    GetAddress(A, a)
    ZeroExtend(i, z)
    Binary(+ or -, z, k, x)        ; any number of these, or none
    Binary(*, x, R, m)
    Binary(+, a, m, p)

`A` is a static-storage array that isn't in zero page, `R` is the size
of its elements and not a power of two, `i` is an unsigned byte that
isn't volatile, and the constants add up to a `k` from -255 to N.
`c99_to_tac` writes this for `A[i]` when A spans more than 255 bytes,
whatever A's element type is, so `A[i][j]`, `&A[i]`, `&A[i][j]`, `A[i]`
as a row pointer, and an array of structs all qualify. The pointer is
computed where the `ZeroExtend` read `i`.

A table costs 2(N+1) bytes in each unit that makes it, and each access
saves what the multiply and the add would have cost (`saving`). The
pass builds a table for an array only when the unit's accesses to it
save more than the table costs.

The table's two halves, the low bytes and the high bytes, are objects
of their own, `A@lo` and `A@hi`, and `A@rows` exists only in the TAC,
where it holds both halves one after the other so that constant folding
and `tac_sim` read it as any table split into byte planes. Its symbol
names the two objects (`StaticAttr.planes`), and `tac_to_asm` emits
them in its place and reads byte j of a lookup from the j-th of them,
at `A@lo+k,X` and `A@hi+k,X`. Arrays with the same number of rows share
a half when its bytes are the same, as the low halves of two arrays 256
bytes apart are, so such a half is one object, named after the first
array that has it. Each half is marked `mergeable`, since nothing but
these lookups reads it and nothing takes its address, so the linker
keeps one copy of the halves that several units make with the same
bytes (passes/linker.py). */
#include "passes/optimization/row_address_tables.h"

#include <algorithm>
#include <cstddef>
#include <exception>
#include <map>
#include <optional>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "bit_precise.h"
#include "c99_to_tac.h"
#include "passes/optimization/split_tables.h"
#include "passes/optimization/strength_reduction.h"
#include "passes/optimization/var_visit.h"
#include "passes/shift_cost.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::tac_ast;
namespace c99 = ::c6502::c99_ast;
using Instrs = std::vector<ir::Type_instruction>;

// The bytes a lookup saves (`saving`), and the most arrays of a group
// whose every set `best_set` tries.
constexpr int kTwoTermSaving = 40;
constexpr int kHelperSaving = 10;
constexpr std::size_t kExhaustive = 10;

// One row address to rewrite in `function`: the instruction at `at` (the
// `ZeroExtend`) becomes the lookup of row `index + k` of `array` into
// `dst`, and the instructions at `drop` go.
struct Access {
  std::string function;
  std::string array;
  std::size_t at = 0;
  ir::Type_val index;
  asdl::Int k = 0;
  ir::Type_val dst;
  std::vector<std::size_t> drop;
};

using Cells = std::vector<ValueTree>;

// Returns the symbol of a table of `const` bytes holding `values`, with
// `attrs` for its other attributes.
Symbol byte_table(Cells values, StaticAttr attrs) {
  const auto size = static_cast<asdl::Int>(values.size());
  ValueTree tuple;
  tuple.kind = ValueTree::Kind::kTuple;
  tuple.items = std::move(values);
  attrs.initial_value = Initial{.value = std::move(tuple)};
  attrs.is_global = false;
  return Symbol{.type = c99::Array{.element_type = c99::Const{.referenced_type = c99::UChar{}},
                                   .size = size},
                .attrs = std::move(attrs)};
}

// The number of rows and the size of a row of the static array `name`
// when a table of its row addresses can be made.
struct Shape {
  asdl::Int rows = 0;
  asdl::Int row = 0;
};

std::optional<Shape> array_shape(const std::string& name, const SymbolTable& symbols,
                                 const TypeTable* types) {
  const Symbol* sym = symbols.find(name);
  if (sym == nullptr) {
    return std::nullopt;
  }
  const auto* attrs = std::get_if<StaticAttr>(&sym->attrs);
  if (attrs == nullptr || attrs->zeropage) {
    return std::nullopt;
  }
  const c99::Type_data_type* t = &sym->type;
  while (true) {
    if (const auto* c = std::get_if<c99::Const>(t)) {
      t = &*c->referenced_type;
    } else if (const auto* v = std::get_if<c99::Volatile>(t)) {
      t = &*v->referenced_type;
    } else {
      break;
    }
  }
  const auto* arr = std::get_if<c99::Array>(t);
  if (arr == nullptr || arr->size < 1) {
    return std::nullopt;
  }
  asdl::Int row = 0;
  try {
    row = c99_to_tac::sizeof_c99(*arr->element_type, types);
  } catch (const std::exception&) {
    // Python's `_sizeof` raises for a type without a size, which the
    // pass takes as an array it can't make a table for.
    return std::nullopt;
  }
  if (row < 3 || (row & (row - 1)) == 0) {
    return std::nullopt;
  }
  return Shape{.rows = arr->size, .row = row};
}

const c99::Type_data_type& strip_const(const c99::Type_data_type& type) {
  const c99::Type_data_type* t = &type;
  while (const auto* c = std::get_if<c99::Const>(t)) {
    t = &*c->referenced_type;
  }
  return *t;
}

bool is_unsigned_byte(const ir::Type_val& v, const SymbolTable& symbols) {
  const auto* var = std::get_if<ir::Var>(&v);
  if (var == nullptr) {
    return false;
  }
  const Symbol* sym = symbols.find(var->name);
  if (sym == nullptr) {
    return false;
  }
  const c99::Type_data_type& t = strip_const(sym->type);
  return std::holds_alternative<c99::Char>(t) || std::holds_alternative<c99::UChar>(t) ||
         std::holds_alternative<c99::Bool>(t);
}

bool is_volatile(const ir::Type_val& v, const SymbolTable& symbols) {
  return std::holds_alternative<c99::Volatile>(
      strip_const(symbols.at(std::get<ir::Var>(v).name).type));
}

std::optional<asdl::Int> const_of(const ir::Type_val& v) {
  const auto* c = std::get_if<ir::Constant>(&v);
  return c == nullptr ? std::nullopt : bit_precise::raw_value(c->const_);
}

// The definitions and uses of one function's names, for `only_def`.
struct Facts {
  const Instrs& instrs;
  std::map<std::string, std::vector<std::size_t>, std::less<>> defs;
  OrderedMap<std::string, std::size_t> uses;

  explicit Facts(const Instrs& in) : instrs(in), uses(count_uses(in)) {
    for (std::size_t k = 0; k < in.size(); ++k) {
      for (const auto d : defs_in(in[k])) {
        defs[std::string(d)].push_back(k);
      }
    }
  }

  std::size_t def_count(const std::string& name) const {
    const auto it = defs.find(name);
    return it == defs.end() ? 0 : it->second.size();
  }

  // Returns the instruction defining the temporary `v`, when it has one
  // definition and one reader.
  std::optional<std::size_t> only_def(const ir::Type_val& v) const {
    const auto* var = std::get_if<ir::Var>(&v);
    if (var == nullptr) {
      return std::nullopt;
    }
    const std::size_t* n = uses.find(var->name);
    if (n == nullptr || *n != 1) {
      return std::nullopt;
    }
    const auto it = defs.find(var->name);
    if (it == defs.end() || it->second.size() != 1) {
      return std::nullopt;
    }
    return it->second.front();
  }
};

std::optional<Access> match(const ir::Function& fn, const Facts& f, std::size_t at,
                            const ir::Binary& add, const SymbolTable& symbols,
                            const TypeTable* types) {
  const std::pair<const ir::Type_val*, const ir::Type_val*> orders[] = {
      {&add.src1, &add.src2}, {&add.src2, &add.src1}};
  for (const auto& [a_val, m_val] : orders) {
    const auto a = f.only_def(*a_val);
    if (!a) {
      continue;
    }
    const auto* get = std::get_if<ir::GetAddress>(&f.instrs[*a]);
    if (get == nullptr) {
      continue;
    }
    const auto* array = std::get_if<ir::Var>(&get->operand);
    if (array == nullptr) {
      continue;
    }
    const auto shape = array_shape(array->name, symbols, types);
    if (!shape) {
      return std::nullopt;
    }
    const auto m = f.only_def(*m_val);
    if (!m) {
      return std::nullopt;
    }
    const auto* mul = std::get_if<ir::Binary>(&f.instrs[*m]);
    if (mul == nullptr || !std::holds_alternative<ir::Multiply>(mul->op)) {
      return std::nullopt;
    }
    const ir::Type_val* x_val = nullptr;
    if (const_of(mul->src2) == shape->row) {
      x_val = &mul->src1;
    } else if (const_of(mul->src1) == shape->row) {
      x_val = &mul->src2;
    } else {
      return std::nullopt;
    }
    std::vector<std::size_t> drop = {*a, *m, at};
    asdl::Int k = 0;
    std::size_t x = 0;
    const ir::ZeroExtend* ext = nullptr;
    while (true) {
      const auto xd = f.only_def(*x_val);
      if (!xd) {
        return std::nullopt;
      }
      x = *xd;
      const auto& step = f.instrs[x];
      if ((ext = std::get_if<ir::ZeroExtend>(&step)) != nullptr) {
        break;
      }
      const auto* b = std::get_if<ir::Binary>(&step);
      if (b == nullptr) {
        return std::nullopt;
      }
      const bool is_add = std::holds_alternative<ir::Add>(b->op);
      if (is_add && const_of(b->src2)) {
        k += *const_of(b->src2);
        x_val = &b->src1;
      } else if (is_add && const_of(b->src1)) {
        k += *const_of(b->src1);
        x_val = &b->src2;
      } else if (std::holds_alternative<ir::Subtract>(b->op) && const_of(b->src2)) {
        k -= *const_of(b->src2);
        x_val = &b->src1;
      } else {
        return std::nullopt;
      }
      drop.push_back(x);
    }
    const ir::Type_val& i = ext->src;
    if (!is_unsigned_byte(i, symbols) || is_volatile(i, symbols)) {
      return std::nullopt;
    }
    if (k < -255 || k > shape->rows) {
      return std::nullopt;
    }
    return Access{.function = fn.name,
                  .array = array->name,
                  .at = x,
                  .index = i,
                  .k = k,
                  .dst = add.dst,
                  .drop = std::move(drop)};
  }
  return std::nullopt;
}

std::vector<Access> find_accesses(const ir::Function& fn, const SymbolTable& symbols,
                                  const TypeTable* types) {
  const Facts f(fn.instructions);
  std::vector<Access> out;
  for (std::size_t at = 0; at < fn.instructions.size(); ++at) {
    const auto* add = std::get_if<ir::Binary>(&fn.instructions[at]);
    if (add == nullptr || !std::holds_alternative<ir::Add>(add->op)) {
      continue;
    }
    const auto* dst = std::get_if<ir::Var>(&add->dst);
    if (dst == nullptr || f.def_count(dst->name) != 1) {
      continue;
    }
    if (auto acc = match(fn, f, at, *add, symbols, types)) {
      out.push_back(std::move(*acc));
    }
  }
  return out;
}

// Returns the bytes one lookup saves over `i * row` plus the array's
// address. These were measured on `M[r][c]`, `M[r][c] = v`, and `M[r]`
// for a 16-row `M` with row sizes from 20 to 104. When `row` is a sum or
// difference of two powers of two whose shifts are written inline,
// strength reduction makes the multiply two 16-bit shifts and an add or
// subtract, and the lookup saved 37 to 46 bytes. Otherwise the multiply
// calls `mul16`, and the code that sets up the call is one that
// `outline_runs` shares between the accesses, so a lookup saved from 1
// byte (with the code shared by three accesses) to about 20 (with one
// access).
int saving(asdl::Int row) {
  if (const auto terms = strength_reduction::two_power_terms(row)) {
    std::vector<int> shifts;
    if (terms->b == 0) {
      shifts = {terms->a};
    } else {
      shifts = {terms->b, terms->a - terms->b};
    }
    if (std::all_of(shifts.begin(), shifts.end(), [](int s) { return inline_shift_ok(2, s); })) {
      return kTwoTermSaving;
    }
  }
  return kHelperSaving;
}

// A table of `planes` (the low and then the high bytes of an array's row
// addresses), and the objects that hold its planes.
struct Table {
  std::string name;
  std::vector<Cells> planes;
  std::vector<std::string> objects;
};

// The tables to make, one for each array, the objects that hold their
// halves (`halves`, by name), and for each array its table and the
// number of row addresses in it.
struct Layout {
  std::vector<Table> tables;
  OrderedMap<std::string, Cells> halves;
  OrderedMap<std::string, std::pair<std::string, asdl::Int>> arrays;
};

ValueTree byte_cell(asdl::Int v) {
  ValueTree t;
  t.kind = ValueTree::Kind::kInt;
  t.integer = v;
  return t;
}

ValueTree address_byte_cell(const std::string& name, asdl::Int offset, bool high) {
  ValueTree t;
  t.kind = ValueTree::Kind::kTacInit;
  t.tac_init = ir::AddressByteInit{.name = name, .offset = offset, .high = high};
  return t;
}

// Returns the low and high bytes of the N+1 row addresses of `name`.
std::pair<Cells, Cells> row_bytes(const std::string& name, asdl::Int rows, asdl::Int row,
                                  const SymbolTable& symbols) {
  const auto& address = std::get<StaticAttr>(symbols.at(name).attrs).address;
  Cells lo;
  Cells hi;
  for (asdl::Int r = 0; r <= rows; ++r) {
    if (address) {
      const asdl::Int v = (*address + r * row) & 0xFFFF;
      lo.push_back(byte_cell(v & 0xFF));
      hi.push_back(byte_cell(v >> 8));
    } else {
      lo.push_back(address_byte_cell(name, r * row, false));
      hi.push_back(address_byte_cell(name, r * row, true));
    }
  }
  return {std::move(lo), std::move(hi)};
}

using Halves = std::map<std::string, std::pair<Cells, Cells>, std::less<>>;
using Gains = std::map<std::string, asdl::Int, std::less<>>;

// Returns the bytes the lookups of `names` save, less their table.
asdl::Int net(const std::vector<std::string>& names, const Halves& halves, const Gains& gain,
              asdl::Int length) {
  std::vector<const Cells*> distinct;
  asdl::Int total = 0;
  for (const auto& n : names) {
    const auto& [lo, hi] = halves.at(n);
    for (const Cells* h : {&lo, &hi}) {
      const bool seen = std::any_of(distinct.begin(), distinct.end(),
                                    [&](const Cells* d) { return *d == *h; });
      if (!seen) {
        distinct.push_back(h);
      }
    }
    total += gain.at(n);
  }
  return total - length * static_cast<asdl::Int>(distinct.size());
}

// Returns the arrays of one group to make lookups for, in a fixed order,
// or an empty list when no set saves bytes.
std::vector<std::string> best_set(const std::vector<std::string>& names, const Halves& halves,
                                  const Gains& gain, asdl::Int length) {
  if (names.size() <= kExhaustive) {
    std::vector<std::string> best;
    asdl::Int best_net = 0;
    for (std::size_t mask = 1; mask < (std::size_t(1) << names.size()); ++mask) {
      std::vector<std::string> pick;
      for (std::size_t b = 0; b < names.size(); ++b) {
        if ((mask >> b) & 1) {
          pick.push_back(names[b]);
        }
      }
      const asdl::Int value = net(pick, halves, gain, length);
      if (value > best_net) {
        best = std::move(pick);
        best_net = value;
      }
    }
    return best;
  }
  std::vector<std::string> order = names;
  std::sort(order.begin(), order.end(), [&](const std::string& x, const std::string& y) {
    const asdl::Int gx = gain.at(x);
    const asdl::Int gy = gain.at(y);
    return gx != gy ? gx > gy : x < y;
  });
  std::vector<std::string> chosen;
  for (const auto& n : order) {
    std::vector<std::string> with = chosen;
    with.push_back(n);
    if (net(with, halves, gain, length) > net(chosen, halves, gain, length)) {
      chosen = std::move(with);
    }
  }
  std::sort(chosen.begin(), chosen.end());
  return chosen;
}

// Picks the arrays whose lookups save more than the bytes their tables
// add, and lays out the tables. Each chosen array gets a table `A@rows`
// of its low and high halves, and each distinct half is one object,
// `A@lo` or `A@hi` after the first array that has it. Arrays with the
// same number of rows can share halves, so the arrays of such a group
// are chosen together, trying every set of them when there are at most
// `kExhaustive` and adding them one at a time, the most accessed first,
// otherwise.
Layout choose(const std::map<std::string, std::vector<Access>, std::less<>>& accesses,
              const SymbolTable& symbols, const TypeTable* types) {
  Layout layout;
  std::map<asdl::Int, std::vector<std::string>> groups;
  Halves halves;
  Gains gain;
  // `accesses` iterates in the order of its names, as Python's
  // `sorted(accesses)` does.
  for (const auto& [name, list] : accesses) {
    const Shape shape = *array_shape(name, symbols, types);
    halves[name] = row_bytes(name, shape.rows, shape.row, symbols);
    gain[name] = static_cast<asdl::Int>(list.size()) * saving(shape.row);
    groups[shape.rows + 1].push_back(name);
  }
  for (const auto& [length, names] : groups) {
    std::vector<std::pair<const Cells*, std::string>> made;
    // Returns the object holding `half`, made as `name` if no array of
    // the group has it yet.
    const auto half_object = [&](const Cells& half, const std::string& name) -> std::string {
      for (const auto& [bytes, existing] : made) {
        if (*bytes == half) {
          return existing;
        }
      }
      made.emplace_back(&half, name);
      layout.halves.insert_or_assign(name, half);
      return name;
    };
    for (const auto& n : best_set(names, halves, gain, length)) {
      const auto& [lo, hi] = halves.at(n);
      Table table;
      table.name = n + "@rows";
      table.planes = {lo, hi};
      std::string lo_object = half_object(lo, n + "@lo");
      std::string hi_object = half_object(hi, n + "@hi");
      table.objects = {std::move(lo_object), std::move(hi_object)};
      layout.arrays.insert_or_assign(n, std::make_pair(table.name, length));
      layout.tables.push_back(std::move(table));
    }
  }
  return layout;
}

ir::Function rewrite(ir::Function fn, const std::vector<const Access*>& accesses,
                     const Layout& layout) {
  std::set<std::size_t> drop;
  for (const Access* acc : accesses) {
    const auto& [table, plane] = layout.arrays.at(acc->array);
    fn.instructions[acc->at] = ir::IndexedLoad{.name = table,
                                               .index = acc->index,
                                               .dst = acc->dst,
                                               .is_volatile = false,
                                               .offset = acc->k,
                                               .plane = plane};
    drop.insert(acc->drop.begin(), acc->drop.end());
  }
  Instrs out;
  out.reserve(fn.instructions.size());
  for (std::size_t k = 0; k < fn.instructions.size(); ++k) {
    if (!drop.contains(k)) {
      out.push_back(std::move(fn.instructions[k]));
    }
  }
  fn.instructions = std::move(out);
  return fn;
}

}  // namespace

ir::Program apply_row_address_tables(ir::Program prog, SymbolTable* symbols,
                                     const TypeTable* types) {
  if (symbols == nullptr) {
    return prog;
  }
  std::map<std::string, std::vector<Access>, std::less<>> accesses;
  for (const auto& tl : prog.top_level) {
    if (const auto* fn = std::get_if<ir::Function>(&tl)) {
      for (auto& acc : find_accesses(*fn, *symbols, types)) {
        accesses[acc.array].push_back(std::move(acc));
      }
    }
  }
  if (accesses.empty()) {
    return prog;
  }
  const Layout layout = choose(accesses, *symbols, types);
  for (auto& tl : prog.top_level) {
    auto* fn = std::get_if<ir::Function>(&tl);
    if (fn == nullptr) {
      continue;
    }
    std::vector<const Access*> mine;
    for (const auto& [name, _] : layout.arrays) {
      for (const auto& a : accesses.at(name)) {
        if (a.function == fn->name) {
          mine.push_back(&a);
        }
      }
    }
    if (!mine.empty()) {
      *fn = rewrite(std::move(*fn), mine, layout);
    }
  }
  for (const auto& [name, half] : layout.halves) {
    StaticAttr attrs;
    attrs.mergeable = true;
    symbols->insert_or_assign(name, byte_table(half, std::move(attrs)));
  }
  for (const auto& table : layout.tables) {
    prog.top_level.push_back(ir::StaticVariable{.name = table.name,
                                                .is_global = false,
                                                .data_type = ir::Pointer{},
                                                .init = split_tables::plane_init(table.planes)});
    Cells values;
    for (const auto& plane : table.planes) {
      values.insert(values.end(), plane.begin(), plane.end());
    }
    StaticAttr attrs;
    attrs.planes = table.objects;
    symbols->insert_or_assign(table.name, byte_table(std::move(values), std::move(attrs)));
  }
  return prog;
}

}  // namespace c6502::passes
