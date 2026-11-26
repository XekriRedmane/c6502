/*
Laying a linked program out: where each input section goes, by a
linker script (`compile.py -T script.ld`, whose language link_script.cpp
describes) or by the default script, which puts `.text`, `.rodata`,
`.data`, and `.bss` one after another from the origin.

Each unit's items (its functions and objects, the `item` lines of its
link metadata, passes/sections.h) are its input sections, one each in
the object. `lay_out` walks the script's statements in order, keeping
each memory region's next free address, and places the items each
output section takes, unit by unit in the order the units were given,
and in each unit in the order it lists them. Since every section's
size is known, each address is a number, computed here.

Every output section `.name` defines `__name_start` and `__name_end`
(its run addresses), `__name_load` and `__name_load_end` (where its
bytes are in memory before the startup code copies them, which are the
run addresses unless the section has `AT>` or `AT(...)`), and
`__name_size`. Dots and other characters that can't be in a symbol
become underscores (`.text.boot` gives `__text_boot_start`). `ADDR`,
`LOADADDR`, and `SIZEOF` in the script give those values. `. = expr`
inside a section skips to `expr`, with zero bytes in a section that's
loaded.

Inside a section, a function that ends with a `JMP` to another of its
functions goes right before that one, and the `JMP` is dropped
(`fall_through`), as hand-written 6502 code falls from one routine
into the next. A section marked `(NOREORDER)` keeps its order, and
only a function already in front of its target loses the `JMP`.
Nothing moves across an assignment in the section, since one marks an
address.

A section that runs past the end of its region, or whose load bytes
run past the end of its load region, is an error, and so is one whose
run or load bytes lie outside the 6502's 64K of address space
($0000-$FFFF, `kAddressSpace`). An empty section may start at $10000,
the end of the address space, as the sections after one that fills
memory to the top do.

`startup_lines` is code for `__zp_init`, which the boot code calls
before `main`. It copies each `AT>` section from its load address to
its run address and clears each `NOLOAD` section, using `DPTR` and the
first four bytes of `HARGS`, which hold nothing yet then.

The script's file patterns match an input's name after its last `/` or
`\` with `fnmatch::fnmatchcase` (src/fnmatch.h), and so do its section
patterns. The evaluator computes in 128 bits, throwing `InternalError`
for a value of more than 125 bits, and for an address or a symbol's
value past 64 bits, which the layout keeps them in.
*/
#include "passes/link_layout.h"

#include <map>
#include <set>

#include "asm_emit.h"
#include "errors.h"
#include "fnmatch.h"
#include "lark_lexer.h"
#include "pyrepr.h"

namespace c6502::passes::link_layout {

namespace {

namespace ls = ::c6502::passes::link_script;
using ls::Expr;

[[noreturn]] void layout_error(const std::string& message) {
  throw CompileError("LayoutError", message);
}

const asdl::Int kLimit = asdl::Int(1) << 125;

[[noreturn]] void too_wide() {
  throw InternalError("a linker script's value takes more than 125 bits");
}

asdl::Int bounded(asdl::Int v) {
  if (v > kLimit || v < -kLimit) too_wide();
  return v;
}

asdl::Int magnitude(asdl::Int v) { return v < 0 ? -v : v; }

std::int64_t to_i64(asdl::Int v) {
  if (v > asdl::Int(INT64_MAX) || v < asdl::Int(INT64_MIN)) {
    throw InternalError("a linker script's address or symbol takes more than 64 bits");
  }
  return static_cast<std::int64_t>(v);
}

// Returns `e` as a linker script spells it, with each binary operation
// in parentheses.
std::string expr_text(const Expr& e) {
  switch (e.kind) {
    case Expr::Kind::Num:
      return pyrepr::integer(e.value);
    case Expr::Kind::Sym:
      return e.name;
    case Expr::Kind::Dot:
      return ".";
    case Expr::Kind::Unary:
      return e.op + expr_text(e.args[0]);
    case Expr::Kind::Binary:
      return "(" + expr_text(e.args[0]) + " " + e.op + " " + expr_text(e.args[1]) + ")";
    case Expr::Kind::Call: {
      std::string out = e.fn + "(";
      for (std::size_t i = 0; i < e.args.size(); ++i) {
        if (i > 0) out += ", ";
        out += expr_text(e.args[i]);
      }
      return out + ")";
    }
  }
  return "?";
}

asdl::Int align(asdl::Int base, asdl::Int n) {
  if (n <= 1) return base;
  return bounded(base + n - 1) & ~(n - 1);
}

asdl::Int binary_op(const std::string& op, asdl::Int a, asdl::Int b) {
  if (op == "+") return bounded(a + b);
  if (op == "-") return bounded(a - b);
  if (op == "*") {
    if (a != 0 && magnitude(b) > kLimit / magnitude(a)) too_wide();
    return a * b;
  }
  if (op == "/") {
    if (b == 0) layout_error("division by zero in the linker script");
    return asdl::floor_div(a, b);
  }
  if (op == "&") return a & b;
  if (op == "|") return a | b;
  if (op == "^") return a ^ b;
  if ((op == "<<" || op == ">>") && b < 0) {
    layout_error("a shift by a negative count in the linker script");
  }
  if (op == "<<") {
    if (a == 0) return 0;
    if (b > 125 || magnitude(a) > (kLimit >> static_cast<int>(b))) too_wide();
    return a * (asdl::Int(1) << static_cast<int>(b));
  }
  if (op == ">>") {
    if (b > 126) return a < 0 ? -1 : 0;
    return a >> static_cast<int>(b);
  }
  layout_error("unknown operator " + op + " in the linker script");
}

class Evaluator {
 public:
  Evaluator(const ls::Script& script, const OrderedMap<std::string, std::int64_t>& symbols,
            const Lookup& lookup)
      : script_(script), symbols_(symbols), lookup_(lookup) {}

  asdl::Int operator()(const Expr& e, std::optional<asdl::Int> dot) const {
    switch (e.kind) {
      case Expr::Kind::Num:
        return e.value;
      case Expr::Kind::Sym: {
        if (const std::int64_t* v = symbols_.find(e.name)) return *v;
        const std::optional<std::int64_t> v = lookup_ ? lookup_(e.name) : std::nullopt;
        if (!v) layout_error("the script uses `" + e.name + "` before it has a value");
        return *v;
      }
      case Expr::Kind::Dot:
        if (!dot) layout_error("`.` isn't defined here");
        return *dot;
      case Expr::Kind::Unary: {
        const asdl::Int v = (*this)(e.args[0], dot);
        return e.op == "-" ? -v : ~v;
      }
      case Expr::Kind::Binary: {
        const asdl::Int a = (*this)(e.args[0], dot);
        const asdl::Int b = (*this)(e.args[1], dot);
        return binary_op(e.op, a, b);
      }
      case Expr::Kind::Call:
        return call(e, dot);
    }
    layout_error("can't use `" + expr_text(e) + "` in a linker script expression");
  }

 private:
  asdl::Int call(const Expr& e, std::optional<asdl::Int> dot) const {
    if (e.fn == "ALIGN") {
      asdl::Int base;
      const Expr* n;
      if (e.args.size() == 1) {
        if (!dot) layout_error("`ALIGN(n)` needs `.`");
        base = *dot;
        n = &e.args[0];
      } else {
        base = (*this)(e.args[0], dot);
        n = &e.args[1];
      }
      if (n->kind != Expr::Kind::Num || (n->value & (n->value - 1)) != 0) {
        layout_error("ALIGN takes a power of two written as a number");
      }
      return align(base, n->value);
    }
    const bool one_name = e.args.size() == 1 && e.args[0].kind == Expr::Kind::Sym;
    if (one_name && (e.fn == "ORIGIN" || e.fn == "LENGTH")) {
      const std::string& name = e.args[0].name;
      const ls::Region* region = script_.regions.find(name);
      if (region == nullptr) layout_error(e.fn + "(" + name + ") names no region");
      return e.fn == "ORIGIN" ? region->origin : region->length;
    }
    if (one_name && (e.fn == "ADDR" || e.fn == "LOADADDR" || e.fn == "SIZEOF")) {
      const std::string& name = e.args[0].name;
      const std::string key = e.fn == "ADDR" ? "start" : e.fn == "LOADADDR" ? "load" : "size";
      const std::string sym = "__" + section_symbol(name) + "_" + key;
      const std::int64_t* v = symbols_.find(sym);
      if (v == nullptr) {
        layout_error(e.fn + "(" + name + ") comes before section " + name + " is laid out");
      }
      return *v;
    }
    layout_error("can't use `" + expr_text(e) + "` in a linker script expression");
  }

  const ls::Script& script_;
  const OrderedMap<std::string, std::int64_t>& symbols_;
  const Lookup& lookup_;
};

std::string file_name(const std::string& path) {
  const std::size_t at = path.find_last_of("/\\");
  return at == std::string::npos ? path : path.substr(at + 1);
}

}  // namespace

std::string address_text(asdl::Int n) {
  return n >= 0 ? "$" + pyrepr::hex(n, 4) : "-$" + pyrepr::hex(-n, 4);
}

std::optional<std::string> tail_jump(const elf::SectionPtr& sec,
                                     const std::vector<elf::SymbolPtr>& symbols) {
  const std::int64_t size = sec->size();
  if (sec->type == elf::SHT_NOBITS || size < 3 ||
      static_cast<unsigned char>(sec->data[static_cast<std::size_t>(size - 3)]) != 0x4C) {
    return std::nullopt;
  }
  const elf::Reloc* r = nullptr;
  for (const elf::Reloc& rel : sec->relocs) {
    if (rel.offset == size - 2) {
      r = &rel;
      break;
    }
  }
  if (r == nullptr || r->type != elf::R_MOS_ADDR16 || r->addend != 0) return std::nullopt;
  if (r->symbol->name.starts_with(".")) return std::nullopt;
  std::optional<std::pair<std::int64_t, std::string>> mapping;
  for (const elf::SymbolPtr& s : symbols) {
    if (s->section != sec) continue;
    if (s->value > size - 3 && s->type != elf::STT_FUNC) {
      return std::nullopt;
    }
    if ((s->name == "$c" || s->name == "$d") && s->value <= size - 3) {
      if (!mapping || s->value >= mapping->first) mapping = std::make_pair(s->value, s->name);
    }
  }
  if (mapping && mapping->second == "$d") return std::nullopt;
  return r->symbol->name;
}

// A function is the target of at most one pair, the first of the
// functions jumping to it in `entries`, and a pair that would close a
// cycle is left out. Each chain of pairs is laid down where its first
// function is, and every other entry keeps its place. Dropping the jump
// keeps any label on it, which then names the target's first
// instruction, the address the jump went to.
std::vector<std::pair<Entry, bool>> fall_through(const std::vector<Entry>& entries,
                                                 bool reorder) {
  std::map<std::string, std::size_t, std::less<>> index;
  for (std::size_t i = 0; i < entries.size(); ++i) {
    if (entries[i].function) index.insert_or_assign(entries[i].name, i);
  }
  std::map<std::size_t, std::size_t> jumps;
  for (std::size_t i = 0; i < entries.size(); ++i) {
    const Entry& e = entries[i];
    if (!e.function || !e.tail) continue;
    const auto it = index.find(*e.tail);
    if (it == index.end()) continue;
    const std::size_t j = it->second;
    if (j == i || (!reorder && j != i + 1)) continue;
    jumps.emplace(i, j);
  }
  std::map<std::size_t, std::size_t> after;
  std::map<std::size_t, std::size_t> before;
  for (const auto& [i, j] : jumps) {
    if (before.contains(j)) continue;
    std::size_t end = j;
    while (after.contains(end)) end = after.at(end);
    if (end == i) continue;
    after.emplace(i, j);
    before.emplace(j, i);
  }
  std::vector<std::pair<Entry, bool>> out;
  for (std::size_t i = 0; i < entries.size(); ++i) {
    if (before.contains(i)) continue;
    std::optional<std::size_t> n = i;
    while (n) {
      out.emplace_back(entries[*n], after.contains(*n));
      const auto it = after.find(*n);
      n = it == after.end() ? std::nullopt : std::optional<std::size_t>(it->second);
    }
  }
  return out;
}

std::string size_symbol(const std::string& symbol) { return "__" + symbol + "_size"; }

bool is_size_symbol(std::string_view name) {
  return name.starts_with("__") && name.ends_with("_size");
}

std::string section_symbol(const std::string& section) {
  std::size_t start = 0;
  while (start < section.size() && section[start] == '.') ++start;
  std::string out;
  for (const char32_t c : lark::decode_utf8(std::string_view(section).substr(start))) {
    const bool keep = (c >= U'A' && c <= U'Z') || (c >= U'a' && c <= U'z') ||
                      (c >= U'0' && c <= U'9') || c == U'_';
    out += keep ? static_cast<char>(c) : '_';
  }
  return out;
}

link_script::Script default_script(const std::vector<Unit>& units, std::int64_t origin) {
  const std::vector<std::string> std_names = {".text", ".rodata", ".data", ".bss"};
  std::vector<std::string> names = std_names;
  const auto is_std = [&](const std::string& name) {
    for (const auto& n : std_names) {
      if (name == n) return true;
    }
    return false;
  };
  for (const Unit& u : units) {
    for (const ItemMeta& it : u.items) {
      const std::string& base = it.section;
      bool standard = false;
      for (const auto& n : std_names) {
        if (base == n || base.starts_with(n + ".")) standard = true;
      }
      bool listed = false;
      for (const auto& n : names) {
        if (base == n) listed = true;
      }
      if (!standard && !listed) names.push_back(base);
    }
  }
  link_script::Script script;
  for (std::size_t k = 0; k < names.size(); ++k) {
    link_script::OutputSection sec;
    sec.name = names[k];
    if (k == 0) {
      Expr e;
      e.kind = Expr::Kind::Num;
      e.value = origin;
      sec.address = e;
    }
    link_script::Input in;
    in.patterns = is_std(names[k]) ? std::vector<std::string>{names[k], names[k] + ".*"}
                                   : std::vector<std::string>{names[k]};
    sec.contents.emplace_back(std::move(in));
    script.statements.emplace_back(std::move(sec));
  }
  return script;
}

Layout lay_out(const link_script::Script& script, const std::vector<Unit>& units,
               const Lookup& lookup) {
  Layout layout;
  auto& sym = layout.symbols;
  const Evaluator ev(script, sym, lookup);
  std::set<std::pair<int, std::string>> placed;
  OrderedMap<std::string, asdl::Int> cursor;
  for (const auto& [name, r] : script.regions) cursor.insert_or_assign(name, r.origin);
  std::optional<asdl::Int> prev_end;
  for (const link_script::Statement& stmt : script.statements) {
    if (const auto* assign = std::get_if<link_script::Assign>(&stmt)) {
      if (assign->symbol == ".") layout_error("`.` can only be set inside a section");
      const std::int64_t v = to_i64(ev(assign->expr, prev_end));
      sym.insert_or_assign(assign->symbol, v);
      continue;
    }
    const auto& sec = std::get<link_script::OutputSection>(stmt);
    const std::string s = section_symbol(sec.name);
    asdl::Int vma;
    if (sec.address) {
      vma = ev(*sec.address, prev_end);
    } else if (sec.region) {
      vma = cursor.at(*sec.region);
    } else if (prev_end) {
      vma = *prev_end;
    } else {
      layout_error("output section " + sec.name + " has no address and no region");
    }
    if (sec.align) {
      if (sec.align->kind != Expr::Kind::Num) {
        layout_error("ALIGN on " + sec.name + " has to be a number");
      }
      const asdl::Int n = sec.align->value;
      if ((n & (n - 1)) != 0) layout_error("ALIGN on " + sec.name + " takes a power of two");
      vma = align(vma, n);
    }
    const bool relocated = sec.at.has_value() || sec.load_region.has_value();
    asdl::Int lma;
    if (sec.at) {
      lma = ev(*sec.at, prev_end);
    } else if (sec.load_region) {
      lma = cursor.at(*sec.load_region);
    } else {
      lma = vma;
    }
    binutils::ld::OutputSection out;
    out.name = sec.name;
    out.addr = to_i64(vma);
    out.load_addr = to_i64(lma);
    out.noload = sec.noload;
    sym.insert_or_assign("__" + s + "_start", to_i64(vma));
    if (relocated) sym.insert_or_assign("__" + s + "_load", to_i64(lma));
    asdl::Int pc = vma;
    std::vector<Entry> run;
    std::vector<std::pair<const Unit*, elf::SectionPtr>> payloads;

    const auto flush = [&]() {
      for (const auto& [entry, dropped] : fall_through(run, sec.reorder)) {
        const auto& [unit, it_section] = payloads[entry.payload];
        const std::int64_t size = it_section->size() - (dropped ? 3 : 0);
        out.parts.push_back(binutils::ld::Placed{unit->index, it_section, to_i64(pc), size});
        if ((it_section->flags & elf::SHF_EXECINSTR) != 0) out.executable = true;
        pc = bounded(pc + size);
      }
      run.clear();
      payloads.clear();
    };

    for (const link_script::Content& c : sec.contents) {
      if (const auto* a = std::get_if<link_script::Assign>(&c)) {
        flush();
        if (a->symbol == ".") {
          const asdl::Int target = ev(a->expr, pc);
          if (target < pc) {
            layout_error("`. = ...` in " + sec.name + " moves the location back from $" +
                         pyrepr::hex(pc, 4) + " to $" + pyrepr::hex(target, 4));
          }
          pc = target;
        } else {
          sym.insert_or_assign(a->symbol, to_i64(ev(a->expr, pc)));
        }
        continue;
      }
      const auto& in = std::get<link_script::Input>(c);
      for (const Unit& unit : units) {
        if (!fnmatch::fnmatchcase(file_name(unit.path), in.file)) continue;
        for (const ItemMeta& it : unit.items) {
          if (placed.contains({unit.index, it.name})) continue;
          bool matches = false;
          for (const std::string& p : in.patterns) {
            if (fnmatch::fnmatchcase(it.section, p)) {
              matches = true;
              break;
            }
          }
          if (!matches) continue;
          if (sec.noload && !it.zero) {
            layout_error("`" + it.name + "` (section " + it.section + ", " + unit.path +
                         ") has initial bytes, so the NOLOAD output section " + sec.name +
                         " can't hold it");
          }
          placed.insert({unit.index, it.name});
          const elf::SectionPtr& it_section = unit.sections.at(it.name);
          Entry entry;
          entry.name = it.name;
          entry.function = (it_section->flags & elf::SHF_EXECINSTR) != 0;
          if (const std::string* tail = unit.tails.find(it.name)) entry.tail = *tail;
          entry.payload = payloads.size();
          payloads.emplace_back(&unit, it_section);
          run.push_back(std::move(entry));
        }
      }
    }
    flush();
    const asdl::Int size = bounded(pc - vma);
    out.size = to_i64(size);
    const asdl::Int end = pc;
    sym.insert_or_assign("__" + s + "_end", to_i64(end));
    sym.insert_or_assign(size_symbol(s), to_i64(size));
    if (relocated) {
      sym.insert_or_assign("__" + s + "_load_end", to_i64(bounded(lma + size)));
    } else {
      sym.insert_or_assign("__" + s + "_load", to_i64(vma));
      sym.insert_or_assign("__" + s + "_load_end", to_i64(end));
    }
    const std::pair<const std::optional<std::string>*, asdl::Int> checks[] = {
        {&sec.region, end}, {&sec.load_region, bounded(lma + size)}};
    for (const auto& [region, last] : checks) {
      if (!*region) continue;
      const link_script::Region& r = script.regions.at(**region);
      if (last > r.end()) {
        layout_error("section " + sec.name + " runs past the end of region " + **region +
                     " by " + pyrepr::integer(last - r.end()) + " bytes");
      }
      cursor.insert_or_assign(**region, last);
    }
    const std::pair<const char*, asdl::Int> starts[] = {{"runs at", vma}, {"loads at", lma}};
    for (const auto& [what, start] : starts) {
      if (start < 0 || start + size > kAddressSpace) {
        layout_error("section " + sec.name + " " + what + " " + address_text(start) + " with " +
                     pyrepr::integer(size) + " bytes, outside the 6502's address space " +
                     "($0000-$FFFF)");
      }
    }
    layout.outputs.push_back(std::move(out));
    prev_end = end;
  }
  std::string unplaced;
  for (const Unit& unit : units) {
    for (const ItemMeta& it : unit.items) {
      if (placed.contains({unit.index, it.name})) continue;
      if (!unplaced.empty()) unplaced += ", ";
      unplaced += "`" + it.name + "` (section " + it.section + ", " + unit.path + ")";
    }
  }
  if (!unplaced.empty()) layout_error("no output section in the script takes " + unplaced);
  return layout;
}

std::vector<std::string> startup_lines(const link_script::Script& script) {
  using asm_emit::instr_line;
  std::vector<std::string> out;
  const auto add = [&](std::initializer_list<std::string> lines) {
    out.insert(out.end(), lines.begin(), lines.end());
  };
  const auto secs = script.output_sections();
  for (std::size_t k = 0; k < secs.size(); ++k) {
    const link_script::OutputSection& sec = *secs[k];
    const bool relocated = sec.at.has_value() || sec.load_region.has_value();
    if (!(relocated || sec.noload)) continue;
    const std::string s = section_symbol(sec.name);
    const std::string size = size_symbol(s);
    const std::string n = std::to_string(k);
    if (relocated) {
      add({instr_line("LDA", "#<__" + s + "_load"), instr_line("STA", "DPTR"),
           instr_line("LDA", "#>__" + s + "_load"), instr_line("STA", "DPTR+1")});
    }
    add({instr_line("LDA", "#<__" + s + "_start"), instr_line("STA", "HARGS"),
         instr_line("LDA", "#>__" + s + "_start"), instr_line("STA", "HARGS+1"),
         instr_line("LDA", "#<" + size), instr_line("STA", "HARGS+2"),
         instr_line("LDA", "#>" + size), instr_line("STA", "HARGS+3"),
         instr_line("LDY", "#$00"), ".start" + n + ":", instr_line("LDA", "HARGS+2"),
         instr_line("ORA", "HARGS+3"), instr_line("BEQ", ".done" + n)});
    if (relocated) {
      add({instr_line("LDA", "(DPTR),Y"), instr_line("STA", "(HARGS),Y"),
           instr_line("INC", "DPTR"), instr_line("BNE", ".src" + n), instr_line("INC", "DPTR+1"),
           ".src" + n + ":"});
    } else {
      add({instr_line("LDA", "#$00"), instr_line("STA", "(HARGS),Y")});
    }
    add({instr_line("INC", "HARGS"), instr_line("BNE", ".dst" + n), instr_line("INC", "HARGS+1"),
         ".dst" + n + ":", instr_line("LDA", "HARGS+2"), instr_line("BNE", ".low" + n),
         instr_line("DEC", "HARGS+3"), ".low" + n + ":", instr_line("DEC", "HARGS+2"),
         instr_line("JMP", ".start" + n), ".done" + n + ":"});
  }
  return out;
}

}  // namespace c6502::passes::link_layout
