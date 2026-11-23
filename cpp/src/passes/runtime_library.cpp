// The runtime library: the 6502 helpers the compiled code calls to
// multiply, divide, and shift by a variable count (`mul16`, `sdivmod16`,
// `asl8`, and the rest), which the linker adds to a program as a C
// toolchain links routines from libgcc.
//
// The helpers are the `asm_ast` functions of rtlib/helpers.py
// (`all_helper_functions`), the same code the simulator runs, which the
// C++ side reads from the copy tools/rtlib_table.py makes
// (src/rtlib/helpers.h). When a program's objects name a helper that
// none of them defines and no `--defsym` gives, the linker writes that
// helper and every helper it calls (`sdivmod16` calls `udivmod16`) as
// text with `asm_emit` and assembles it into one object, at link time.
// Each helper is a function in a section of its own, `.text`, and the
// object goes by the name `runtime.o` (`kLibraryName`), so a linker
// script places the helpers with an input description such as
// `runtime.o(.text)`, or with a `*(.text)` that catches every unit's
// code. Without a script they go in `.text` after the program's code.
//
// The helpers read and write only HARGS, which the object leaves
// undefined, so they get the program's HARGS address with the rest of
// the program's units. The object's link metadata has the HARGS bytes
// the helpers touch (`helper_footprints`), which the linker counts in
// the size of the program's block.
//
// The helpers without 6502 code (the floating-point ones, the 64-bit
// ones, and `icall`) aren't in the library. A program that calls one
// links only with `--defsym` giving its address.
#include "passes/runtime_library.h"

#include <algorithm>
#include <map>
#include <set>

#include "asm2_ast.h"
#include "asm_ast.h"
#include "asm_emit.h"
#include "passes/asm_to_asm2.h"
#include "passes/zp_globals.h"
#include "passes/zp_link_metadata.h"
#include "rtlib/helpers.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::asm_ast;

// Returns each helper's function by name.
const std::map<std::string, const ir::Function*, std::less<>>& functions() {
  static const std::map<std::string, const ir::Function*, std::less<>> by_name = [] {
    std::map<std::string, const ir::Function*, std::less<>> out;
    for (const ir::Function& fn : rtlib::all_helper_functions()) {
      out.emplace(fn.name, &fn);
    }
    return out;
  }();
  return by_name;
}

// Returns the helpers `fn` calls or jumps to.
std::set<std::string> callees(const ir::Function& fn) {
  const auto& names = functions();
  std::set<std::string> out;
  for (const auto& instr : fn.instructions) {
    if (const auto* call = asdl::as<ir::Call>(instr); call != nullptr && names.contains(call->name)) {
      out.insert(call->name);
    } else if (const auto* jump = asdl::as<ir::Jump>(instr);
               jump != nullptr && names.contains(jump->target)) {
      out.insert(jump->target);
    }
  }
  return out;
}

}  // namespace

NameSet helper_names() {
  NameSet out;
  for (const auto& [name, fn] : functions()) {
    out.insert(name);
  }
  return out;
}

std::vector<std::string> closure(const std::vector<std::string>& names) {
  const auto& fns = functions();
  std::vector<std::string> todo;
  for (const std::string& n : names) {
    if (fns.contains(n)) {
      todo.push_back(n);
    }
  }
  std::set<std::string> seen;
  while (!todo.empty()) {
    const std::string name = todo.back();
    todo.pop_back();
    if (!seen.insert(name).second) {
      continue;
    }
    for (const std::string& callee : callees(*fns.at(name))) {
      if (!seen.contains(callee)) {
        todo.push_back(callee);
      }
    }
  }
  std::vector<std::string> out;
  for (const ir::Function& fn : rtlib::all_helper_functions()) {
    if (seen.contains(fn.name)) {
      out.push_back(fn.name);
    }
  }
  return out;
}

std::vector<std::string> needed(
    const std::vector<std::pair<std::string, binutils::elf::ElfFile>>& objects,
    const NameSet& defined) {
  const auto& fns = functions();
  // Python collects these in a set, which `closure` puts in the
  // library's order, so the order here doesn't reach the result.
  std::vector<std::string> wanted;
  std::set<std::string> seen;
  for (const auto& [path, obj] : objects) {
    for (const auto& s : obj.symbols) {
      if (!s->defined() && fns.contains(s->name) && !defined.contains(s->name) &&
          seen.insert(s->name).second) {
        wanted.push_back(s->name);
      }
    }
  }
  std::vector<std::string> out;
  for (std::string& n : closure(wanted)) {
    if (!defined.contains(n)) {
      out.push_back(std::move(n));
    }
  }
  return out;
}

std::string library_text(const std::vector<std::string>& names) {
  const auto& fns = functions();
  const HelperFootprints& footprints = helper_footprints();
  asdl::Int need = 0;
  for (const std::string& n : names) {
    need = std::max(need, footprints.at(n));
  }
  LinkMetadata meta;
  for (const std::string& n : names) {
    meta.items.push_back(ItemMeta{.name = n, .section = ".text"});
  }
  meta.hargs_size = need;
  meta.hargs_need = need;
  ir::Program prog;
  for (const std::string& n : names) {
    prog.top_level.emplace_back(*fns.at(n));
  }
  // The helpers name no `(zp),Y` pointer, so the runtime's layout, which
  // names one by its address, doesn't change the text, and the classic
  // one stands for the link's, as `runtime_layout.current()` is outside
  // a compile.
  asm_emit::EmitOptions options;
  options.link_metadata_lines = format_metadata(meta);
  return asm_emit::emit_program(lower_to_asm2(prog), RuntimeLayout{}, options);
}

// Python's `library_object` is `assemble(library_text(names),
// LIBRARY_NAME)`, which the linker's `runtime_library`
// (src/passes/linker_objects.cpp) does with the C++ assembler.

}  // namespace c6502::passes
