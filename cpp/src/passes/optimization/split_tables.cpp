#include "passes/optimization/split_tables.h"

#include <cstddef>
#include <cstdint>
#include <map>
#include <set>
#include <string>
#include <utility>

#include "bit_precise.h"
#include "errors.h"
#include "passes/optimization/var_visit.h"

namespace c6502::passes::split_tables {

namespace tac = ::c6502::tac_ast;

namespace {

void append_code_point(std::string& out, std::uint32_t cp) {
  if (cp < 0x80) {
    out += static_cast<char>(cp);
  } else {
    out += static_cast<char>(0xC0 | (cp >> 6));
    out += static_cast<char>(0x80 | (cp & 0x3F));
  }
}

}  // namespace

std::vector<tac::Type_static_init> plane_init(const std::vector<std::vector<ValueTree>>& planes) {
  std::vector<tac::Type_static_init> init;
  std::string run;
  asdl::Int run_bytes = 0;
  const auto flush = [&] {
    if (run_bytes != 0) {
      init.push_back(tac::StringInit{.str = run, .bytes = run_bytes});
      run.clear();
      run_bytes = 0;
    }
  };
  for (const auto& plane : planes) {
    for (const auto& c : plane) {
      if (c.kind == ValueTree::Kind::kInt) {
        if (c.integer < 0 || c.integer > 0xFF) {
          throw InternalError("plane_init: a cell that isn't a byte");
        }
        append_code_point(run, static_cast<std::uint32_t>(c.integer));
        ++run_bytes;
        continue;
      }
      flush();
      if (c.kind != ValueTree::Kind::kTacInit || !c.tac_init) {
        throw InternalError("plane_init: a cell that is neither a byte nor an initializer");
      }
      init.push_back(*c.tac_init);
    }
  }
  flush();
  return init;
}

std::optional<int> width(const c99_ast::Type_data_type& t) {
  using DK = c99_ast::Kind_data_type;
  switch (c99_ast::kind(t)) {
    case DK::Char:
    case DK::SChar:
    case DK::UChar:
    case DK::Bool:
      return 1;
    case DK::Int:
    case DK::UInt:
    case DK::Short:
    case DK::UShort:
    case DK::Pointer:
      return 2;
    case DK::Long:
    case DK::ULong:
    case DK::Float:
      return 4;
    case DK::LongLong:
    case DK::ULongLong:
    case DK::Double:
      return 8;
    case DK::BitInt:
      return static_cast<int>((std::get<c99_ast::BitInt>(t).width + 7) / 8);
    default:
      return std::nullopt;
  }
}

}  // namespace c6502::passes::split_tables

namespace c6502::passes {

namespace {

namespace ir = ::c6502::tac_ast;
namespace c99 = ::c6502::c99_ast;
using Instrs = std::vector<ir::Type_instruction>;

struct Table {
  int width = 0;
  int count = 0;
  std::vector<ValueTree> cells;
};

using Tables = OrderedMap<std::string, Table>;

ValueTree byte_cell(asdl::Int v) {
  ValueTree t;
  t.kind = ValueTree::Kind::kInt;
  t.integer = v;
  return t;
}

ValueTree init_cell(ir::Type_static_init init) {
  ValueTree t;
  t.kind = ValueTree::Kind::kTacInit;
  t.tac_init = std::move(init);
  return t;
}

std::optional<std::vector<ValueTree>> init_cells(const std::vector<ir::Type_static_init>& init) {
  std::vector<ValueTree> out;
  const auto num = [&](asdl::Int v, int n) {
    v &= (asdl::Int(1) << (8 * n)) - 1;
    for (int k = 0; k < n; ++k) {
      out.push_back(byte_cell((v >> (8 * k)) & 0xFF));
    }
  };
  using K = ir::Kind_static_init;
  for (const auto& item : init) {
    switch (ir::kind(item)) {
      case K::CharInit:
        num(std::get<ir::CharInit>(item).value, 1);
        break;
      case K::UCharInit:
        num(std::get<ir::UCharInit>(item).value, 1);
        break;
      case K::IntInit:
        num(std::get<ir::IntInit>(item).value, 2);
        break;
      case K::UIntInit:
        num(std::get<ir::UIntInit>(item).value, 2);
        break;
      case K::LongInit:
        num(std::get<ir::LongInit>(item).value, 4);
        break;
      case K::ULongInit:
        num(std::get<ir::ULongInit>(item).value, 4);
        break;
      case K::LongLongInit:
        num(std::get<ir::LongLongInit>(item).value, 8);
        break;
      case K::ULongLongInit:
        num(std::get<ir::ULongLongInit>(item).value, 8);
        break;
      case K::FloatInit:
        num(std::get<ir::FloatInit>(item).bits, 4);
        break;
      case K::DoubleInit:
        num(std::get<ir::DoubleInit>(item).bits, 8);
        break;
      case K::AddressInit: {
        const auto& a = std::get<ir::AddressInit>(item);
        out.push_back(init_cell(ir::AddressByteInit{.name = a.name, .offset = a.offset, .high = false}));
        out.push_back(init_cell(ir::AddressByteInit{.name = a.name, .offset = a.offset, .high = true}));
        break;
      }
      case K::ZeroInit: {
        const asdl::Int n = std::get<ir::ZeroInit>(item).bytes;
        for (asdl::Int k = 0; k < n; ++k) {
          out.push_back(byte_cell(0));
        }
        break;
      }
      default:
        return std::nullopt;
    }
  }
  return out;
}

Tables candidates(const ir::Program& prog, const SymbolTable& symbols) {
  Tables out;
  for (const auto& tl : prog.top_level) {
    const auto* sv = std::get_if<ir::StaticVariable>(&tl);
    if (sv == nullptr || sv->is_global) {
      continue;
    }
    const Symbol* sym = symbols.find(sv->name);
    if (sym == nullptr) {
      continue;
    }
    const auto* attrs = std::get_if<StaticAttr>(&sym->attrs);
    if (attrs == nullptr || attrs->zeropage || attrs->address) {
      continue;
    }
    const auto* arr = std::get_if<c99::Array>(&sym->type);
    if (arr == nullptr || arr->size < 2 || arr->size > 256) {
      continue;
    }
    const auto* c = std::get_if<c99::Const>(&*arr->element_type);
    if (c == nullptr) {
      continue;
    }
    const c99::Type_data_type& elem = *c->referenced_type;
    if (std::holds_alternative<c99::Volatile>(elem)) {
      continue;
    }
    const auto w = split_tables::width(elem);
    if (!w || *w < 2) {
      continue;
    }
    auto cells = init_cells(sv->init);
    if (!cells || static_cast<asdl::Int>(cells->size()) != *w * arr->size) {
      continue;
    }
    out.insert_or_assign(
        sv->name, Table{.width = *w, .count = static_cast<int>(arr->size), .cells = std::move(*cells)});
  }
  return out;
}

struct Read {
  std::string function;
  std::string table;
  std::size_t at = 0;
  ir::Type_val index;
  ir::Type_val dst;
  asdl::Int element = 0;
  std::vector<std::string> chain;
};

using Plans = OrderedMap<std::string, std::optional<std::vector<Read>>>;

std::vector<int> block_starts(const Instrs& instrs) {
  std::vector<int> out;
  out.reserve(instrs.size());
  int block = 0;
  for (std::size_t k = 0; k < instrs.size(); ++k) {
    const auto& ins = instrs[k];
    if (std::holds_alternative<ir::Label>(ins) && k != 0) {
      ++block;
    }
    out.push_back(block);
    if (std::holds_alternative<ir::Jump>(ins) || std::holds_alternative<ir::JumpIfTrue>(ins) ||
        std::holds_alternative<ir::JumpIfFalse>(ins) || std::holds_alternative<ir::JumpIfCmp>(ins) ||
        std::holds_alternative<ir::JumpIfMasked>(ins) || std::holds_alternative<ir::Ret>(ins)) {
      ++block;
    }
  }
  return out;
}

bool is_byte(const ir::Type_val& v, const SymbolTable& symbols, bool unsigned_) {
  const auto* var = std::get_if<ir::Var>(&v);
  if (var == nullptr) {
    return false;
  }
  const Symbol* sym = symbols.find(var->name);
  if (sym == nullptr) {
    return false;
  }
  const c99::Type_data_type* t = &sym->type;
  if (std::holds_alternative<c99::Volatile>(*t)) {
    return false;
  }
  if (const auto* c = std::get_if<c99::Const>(t);
      c != nullptr && std::holds_alternative<c99::Volatile>(*c->referenced_type)) {
    return false;
  }
  while (const auto* c = std::get_if<c99::Const>(t)) {
    t = &*c->referenced_type;
  }
  if (std::holds_alternative<c99::Char>(*t) || std::holds_alternative<c99::UChar>(*t) ||
      std::holds_alternative<c99::Bool>(*t)) {
    return true;
  }
  return !unsigned_ && std::holds_alternative<c99::SChar>(*t);
}

std::optional<asdl::Int> const_of(const ir::Type_val& v) {
  const auto* c = std::get_if<ir::Constant>(&v);
  return c == nullptr ? std::nullopt : bit_precise::raw_value(c->const_);
}

std::optional<ir::Type_val> scaled_by(const ir::Type_instruction& ins, int width) {
  const auto* b = std::get_if<ir::Binary>(&ins);
  if (b == nullptr) {
    return std::nullopt;
  }
  int shift = -1;
  for (int w = width; w != 0; w >>= 1) {
    ++shift;
  }
  if (std::holds_alternative<ir::LeftShift>(b->op) && const_of(b->src2) == asdl::Int(shift)) {
    return b->src1;
  }
  if (std::holds_alternative<ir::Multiply>(b->op)) {
    if (const_of(b->src2) == asdl::Int(width)) {
      return b->src1;
    }
    if (const_of(b->src1) == asdl::Int(width)) {
      return b->src2;
    }
  }
  return std::nullopt;
}

bool unchanged(const Instrs& instrs, std::size_t start, std::size_t end, const std::string& var,
               const SymbolTable& symbols) {
  const Symbol* sym = symbols.find(var);
  bool automatic = sym != nullptr && !std::holds_alternative<StaticAttr>(sym->attrs);
  if (automatic) {
    for (const auto& ins : instrs) {
      const auto* g = std::get_if<ir::GetAddress>(&ins);
      if (g == nullptr) {
        continue;
      }
      const auto* o = std::get_if<ir::Var>(&g->operand);
      if (o != nullptr && o->name == var) {
        automatic = false;
        break;
      }
    }
  }
  for (std::size_t k = start + 1; k < end && k < instrs.size(); ++k) {
    const auto& ins = instrs[k];
    for (const auto d : defs_in(ins)) {
      if (d == var) {
        return false;
      }
    }
    if (automatic) {
      continue;
    }
    if (std::holds_alternative<ir::FunctionCall>(ins) ||
        std::holds_alternative<ir::IndirectCall>(ins) || std::holds_alternative<ir::Store>(ins) ||
        std::holds_alternative<ir::IndexedStore>(ins) ||
        std::holds_alternative<ir::IndexedSymbolStore>(ins) ||
        std::holds_alternative<ir::IndirectIndexedStore>(ins)) {
      return false;
    }
  }
  return true;
}

std::optional<int> dst_width(const ir::Type_val& v, const SymbolTable& symbols) {
  const auto* var = std::get_if<ir::Var>(&v);
  if (var == nullptr) {
    return std::nullopt;
  }
  const Symbol* sym = symbols.find(var->name);
  if (sym == nullptr) {
    return std::nullopt;
  }
  const c99::Type_data_type* t = &sym->type;
  while (const auto* c = std::get_if<c99::Const>(t)) {
    t = &*c->referenced_type;
  }
  return split_tables::width(*t);
}

struct FunctionFacts {
  const Instrs& instrs;
  std::map<std::string, std::vector<std::size_t>, std::less<>> defs;
  std::vector<int> blocks;
  OrderedMap<std::string, std::size_t> uses;

  explicit FunctionFacts(const Instrs& in)
      : instrs(in), blocks(block_starts(in)), uses(count_uses(in)) {
    for (std::size_t k = 0; k < in.size(); ++k) {
      for (const auto d : defs_in(in[k])) {
        defs[std::string(d)].push_back(k);
      }
    }
  }

  std::size_t use_count(const std::string& name) const {
    const std::size_t* n = uses.find(name);
    return n == nullptr ? 0 : *n;
  }

  std::optional<std::size_t> single_def(const ir::Type_val& v) const {
    const auto* var = std::get_if<ir::Var>(&v);
    if (var == nullptr) {
      return std::nullopt;
    }
    const auto it = defs.find(var->name);
    if (it == defs.end() || it->second.size() != 1) {
      return std::nullopt;
    }
    return it->second.front();
  }
};

const std::string& var_name(const ir::Type_val& v) { return std::get<ir::Var>(v).name; }

std::optional<Read> indexed_read(const ir::Function& fn, const FunctionFacts& f, std::size_t k,
                                 const ir::IndexedLoad& ins, const Table& table,
                                 const SymbolTable& symbols) {
  const int width = table.width;
  if (ins.is_volatile || ins.plane != 0 || ins.offset % width != 0) {
    return std::nullopt;
  }
  if (dst_width(ins.dst, symbols) != width) {
    return std::nullopt;
  }
  const asdl::Int element = ins.offset / width;
  if (const auto c = const_of(ins.index)) {
    if (*c % width != 0) {
      return std::nullopt;
    }
    return Read{.function = fn.name,
                .table = ins.name,
                .at = k,
                .index = ir::Constant{.const_ = ir::ConstUChar{.value = 0}},
                .dst = ins.dst,
                .element = element + *c / width,
                .chain = {}};
  }
  const auto s = f.single_def(ins.index);
  if (!s || *s > k || f.blocks[*s] != f.blocks[k]) {
    return std::nullopt;
  }
  const auto i = scaled_by(f.instrs[*s], width);
  if (!i || !is_byte(*i, symbols, false)) {
    return std::nullopt;
  }
  if (!unchanged(f.instrs, *s, k, var_name(*i), symbols)) {
    return std::nullopt;
  }
  return Read{.function = fn.name,
              .table = ins.name,
              .at = k,
              .index = *i,
              .dst = ins.dst,
              .element = element,
              .chain = {var_name(ins.index)}};
}

std::optional<Read> load_read(const ir::Function& fn, const FunctionFacts& f, std::size_t k,
                              const ir::Load& ins, const Tables& tables,
                              const SymbolTable& symbols) {
  if (ins.is_volatile || !std::holds_alternative<ir::Var>(ins.src_ptr)) {
    return std::nullopt;
  }
  if (f.use_count(var_name(ins.src_ptr)) != 1) {
    return std::nullopt;
  }
  const auto p = f.single_def(ins.src_ptr);
  if (!p || *p > k || f.blocks[*p] != f.blocks[k]) {
    return std::nullopt;
  }
  const auto* add = std::get_if<ir::Binary>(&f.instrs[*p]);
  if (add == nullptr || !std::holds_alternative<ir::Add>(add->op)) {
    return std::nullopt;
  }
  const std::pair<const ir::Type_val*, const ir::Type_val*> orders[] = {
      {&add->src1, &add->src2}, {&add->src2, &add->src1}};
  for (const auto& [a_val, s_val] : orders) {
    const auto a = f.single_def(*a_val);
    if (!a || f.blocks[*a] != f.blocks[k]) {
      continue;
    }
    const auto* get = std::get_if<ir::GetAddress>(&f.instrs[*a]);
    if (get == nullptr) {
      continue;
    }
    const auto* operand = std::get_if<ir::Var>(&get->operand);
    if (operand == nullptr || !tables.contains(operand->name)) {
      continue;
    }
    const std::string& name = operand->name;
    const int width = tables.at(name).width;
    if (dst_width(ins.dst, symbols) != width) {
      return std::nullopt;
    }
    const auto s = f.single_def(*s_val);
    if (!s || f.blocks[*s] != f.blocks[k]) {
      return std::nullopt;
    }
    const auto z_val = scaled_by(f.instrs[*s], width);
    const auto z = z_val ? f.single_def(*z_val) : std::nullopt;
    if (!z || f.blocks[*z] != f.blocks[k]) {
      return std::nullopt;
    }
    const auto* ext = std::get_if<ir::ZeroExtend>(&f.instrs[*z]);
    if (ext == nullptr) {
      return std::nullopt;
    }
    if (!is_byte(ext->src, symbols, true)) {
      return std::nullopt;
    }
    if (!unchanged(f.instrs, *z, k, var_name(ext->src), symbols)) {
      return std::nullopt;
    }
    return Read{.function = fn.name,
                .table = name,
                .at = k,
                .index = ext->src,
                .dst = ins.dst,
                .element = 0,
                .chain = {var_name(*a_val), var_name(ins.src_ptr), var_name(*s_val),
                          var_name(*z_val)}};
  }
  return std::nullopt;
}

void find_reads(const ir::Function& fn, const Tables& tables, const SymbolTable& symbols,
                Plans& plans) {
  const Instrs& instrs = fn.instructions;
  const FunctionFacts f(instrs);
  const auto disqualify = [&](const std::string& name) { plans.find(name)->reset(); };
  const auto add = [&](Read read) {
    auto& plan = *plans.find(read.table);
    if (plan) {
      plan->push_back(std::move(read));
    }
  };
  std::map<std::string, std::size_t, std::less<>> covered;
  for (std::size_t k = 0; k < instrs.size(); ++k) {
    if (const auto* ins = std::get_if<ir::IndexedLoad>(&instrs[k])) {
      const Table* table = tables.find(ins->name);
      if (table == nullptr) {
        continue;
      }
      auto read = indexed_read(fn, f, k, *ins, *table, symbols);
      if (!read) {
        disqualify(ins->name);
      } else {
        add(std::move(*read));
      }
      continue;
    }
    if (const auto* ins = std::get_if<ir::Load>(&instrs[k])) {
      auto read = load_read(fn, f, k, *ins, tables, symbols);
      if (read) {
        ++covered[read->chain.front()];
        add(std::move(*read));
      }
    }
  }
  for (std::size_t k = 0; k < instrs.size(); ++k) {
    const auto& ins = instrs[k];
    if (const auto* g = std::get_if<ir::GetAddress>(&ins)) {
      const auto* operand = std::get_if<ir::Var>(&g->operand);
      if (operand != nullptr && tables.contains(operand->name)) {
        const auto* d = std::get_if<ir::Var>(&g->dst);
        bool ok = false;
        if (d != nullptr && f.single_def(g->dst) == k) {
          const auto it = covered.find(d->name);
          const std::size_t n = it == covered.end() ? 0 : it->second;
          ok = n == f.use_count(d->name) && f.use_count(d->name) > 0;
        }
        if (!ok) {
          disqualify(operand->name);
        }
        continue;
      }
    }
    if (std::holds_alternative<ir::IndexedLoad>(ins)) {
      continue;
    }
    for (const auto u : uses_in(ins)) {
      if (tables.contains(std::string(u))) {
        disqualify(std::string(u));
      }
    }
    for (const auto d : defs_in(ins)) {
      if (tables.contains(std::string(d))) {
        disqualify(std::string(d));
      }
    }
    if (const auto* st = std::get_if<ir::IndexedSymbolStore>(&ins);
        st != nullptr && tables.contains(st->name)) {
      disqualify(st->name);
    }
  }
}

ir::Function rewrite_function(ir::Function fn, const std::vector<const Read*>& reads,
                              const Tables& tables) {
  Instrs instrs = std::move(fn.instructions);
  std::set<std::string, std::less<>> chain_names;
  for (const Read* r : reads) {
    const int count = tables.at(r->table).count;
    ir::Type_val index = r->index;
    asdl::Int offset = r->element;
    if (std::holds_alternative<ir::Constant>(index)) {
      index = ir::Constant{.const_ = ir::ConstUChar{.value = offset}};
      offset = 0;
    }
    instrs[r->at] = ir::IndexedLoad{.name = r->table,
                                    .index = std::move(index),
                                    .dst = r->dst,
                                    .is_volatile = false,
                                    .offset = offset,
                                    .plane = count};
    chain_names.insert(r->chain.begin(), r->chain.end());
  }
  while (true) {
    const auto uses = count_uses(instrs);
    Instrs keep;
    keep.reserve(instrs.size());
    for (auto& ins : instrs) {
      bool dead = false;
      for (const auto d : defs_in(ins)) {
        if (!chain_names.contains(d)) {
          continue;
        }
        const std::size_t* n = uses.find(std::string(d));
        if (n == nullptr || *n == 0) {
          dead = true;
          break;
        }
      }
      if (!dead) {
        keep.push_back(std::move(ins));
      }
    }
    const bool same = keep.size() == instrs.size();
    instrs = std::move(keep);
    if (same) {
      break;
    }
  }
  fn.instructions = std::move(instrs);
  return fn;
}

}  // namespace

ir::Program apply_split_tables(ir::Program prog, SymbolTable* symbols) {
  if (symbols == nullptr) {
    return prog;
  }
  const Tables tables = candidates(prog, *symbols);
  if (tables.empty()) {
    return prog;
  }
  Plans plans;
  for (const auto& [name, _] : tables) {
    plans.insert_or_assign(name, std::vector<Read>{});
  }
  for (const auto& tl : prog.top_level) {
    if (const auto* fn = std::get_if<ir::Function>(&tl)) {
      find_reads(*fn, tables, *symbols, plans);
    } else if (const auto* sv = std::get_if<ir::StaticVariable>(&tl)) {
      for (const auto& item : sv->init) {
        const std::string* name = nullptr;
        if (const auto* a = std::get_if<ir::AddressInit>(&item)) {
          name = &a->name;
        } else if (const auto* b = std::get_if<ir::AddressByteInit>(&item)) {
          name = &b->name;
        }
        if (name != nullptr) {
          if (auto* plan = plans.find(*name)) {
            plan->reset();
          }
        }
      }
    }
  }
  NameSet chosen;
  for (const auto& [name, reads] : plans) {
    if (reads && !reads->empty()) {
      chosen.insert(name);
    }
  }
  if (chosen.empty()) {
    return prog;
  }
  for (auto& tl : prog.top_level) {
    if (auto* fn = std::get_if<ir::Function>(&tl)) {
      std::vector<const Read*> reads;
      for (const auto& name : chosen) {
        for (const auto& r : **plans.find(name)) {
          if (r.function == fn->name) {
            reads.push_back(&r);
          }
        }
      }
      if (!reads.empty()) {
        *fn = rewrite_function(std::move(*fn), reads, tables);
      }
    } else if (auto* sv = std::get_if<ir::StaticVariable>(&tl); sv != nullptr && chosen.contains(sv->name)) {
      const Table& table = tables.at(sv->name);
      std::vector<std::vector<ValueTree>> planes(static_cast<std::size_t>(table.width));
      for (int k = 0; k < table.width; ++k) {
        for (int e = 0; e < table.count; ++e) {
          planes[static_cast<std::size_t>(k)].push_back(
              table.cells[static_cast<std::size_t>(e * table.width + k)]);
        }
      }
      sv->init = split_tables::plane_init(planes);
      sv->is_global = false;
      ValueTree values;
      values.kind = ValueTree::Kind::kTuple;
      for (auto& plane : planes) {
        for (auto& c : plane) {
          values.items.push_back(std::move(c));
        }
      }
      const Symbol& old = symbols->at(sv->name);
      StaticAttr attrs;
      attrs.initial_value = Initial{.value = values};
      attrs.is_global = false;
      attrs.section = std::get<StaticAttr>(old.attrs).section;
      Symbol sym{.type = c99::Array{.element_type = c99::Const{.referenced_type = c99::UChar{}},
                                    .size = static_cast<asdl::Int>(values.items.size())},
                 .attrs = std::move(attrs)};
      symbols->insert_or_assign(sv->name, std::move(sym));
    }
  }
  return prog;
}

}  // namespace c6502::passes
