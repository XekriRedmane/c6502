#include "sim/harness.h"

#include <cstdio>
#include <stdexcept>

#include "binutils/elf.h"
#include "sim/assembler.h"

namespace c6502::sim {

namespace {

std::optional<std::int64_t> lookup(const std::map<std::string, std::int64_t>& symbols, std::string_view name) {
  const auto it = symbols.find(std::string(name));
  if (it == symbols.end()) return std::nullopt;
  return it->second;
}

passes::RuntimeLayout layout_of(const std::map<std::string, std::int64_t>& symbols) {
  passes::RuntimeLayout layout;
  layout.ssp = lookup(symbols, "SSP").value_or(static_cast<std::int64_t>(layout.ssp));
  layout.fp = lookup(symbols, "FP").value_or(static_cast<std::int64_t>(layout.fp));
  layout.hargs = lookup(symbols, "HARGS").value_or(static_cast<std::int64_t>(layout.hargs));
  layout.dptr = lookup(symbols, "DPTR").value_or(static_cast<std::int64_t>(layout.dptr));
  return layout;
}

void start(Simulation& sim, std::vector<std::uint8_t> image) {
  sim.cpu.memory = std::move(image);
  sim.cpu.reset(sim.runtime.boot_addr);
}

std::string hex4(std::int64_t v) {
  char buf[16];
  std::snprintf(buf, sizeof buf, "%04llX", static_cast<long long>(v));
  return buf;
}

}  // namespace

std::uint64_t SimResult::word(std::int64_t addr, int size) const {
  std::uint64_t v = 0;
  for (int i = 0; i < size; ++i) v |= std::uint64_t{memory.at(static_cast<std::size_t>(addr + i))} << (8 * i);
  return v;
}

std::uint64_t SimResult::return_long() const { return word(hargs + 8, 4); }
std::uint64_t SimResult::return_longlong() const { return word(hargs + 16, 8); }

bool Simulation::step() {
  const auto it = runtime.hooks.find(cpu.pc);
  if (it != runtime.hooks.end()) {
    it->second(cpu.memory);
    cpu.rts();
    return true;
  }
  if (cpu.memory[cpu.pc] == 0x00) return false;
  cpu.step();
  return true;
}

SimResult Simulation::run(std::uint64_t max_cycles) {
  for (std::uint32_t addr = 0; addr < 0x10000; ++addr) {
    if (cpu.has_trap(static_cast<std::uint16_t>(addr))) cpu.clear_trap(static_cast<std::uint16_t>(addr));
  }
  for (const auto& [addr, hook] : runtime.hooks) {
    cpu.set_trap(addr, [hook = hook](Cpu& c) {
      hook(c.memory);
      return Cpu::Action::kRts;
    });
  }
  Cpu::Limits limits;
  limits.max_cycles = max_cycles;
  const Cpu::RunResult ran = cpu.run(limits);
  SimResult out;
  out.a = cpu.a;
  out.x = cpu.x;
  out.y = cpu.y;
  out.pc = cpu.pc;
  out.sp = cpu.sp;
  out.cycles = cpu.cycles;
  out.memory = cpu.memory;
  out.timed_out = ran.stop == Cpu::Stop::kCycles;
  out.hargs = runtime.hargs;
  return out;
}

std::int64_t Simulation::symbol(std::string_view name) const {
  const auto it = symbols.find(std::string(name));
  if (it == symbols.end()) throw std::out_of_range("the program has no symbol " + std::string(name));
  return it->second;
}

Simulation build_sim(const asm_ast::Program& program, const passes::OrderedSymbolAddrs& symbols,
                     std::int64_t origin) {
  std::map<std::string, std::int64_t> slot_symbols;
  for (const auto& [name, addr] : symbols) slot_symbols[name] = static_cast<std::int64_t>(addr);
  Simulation sim;
  sim.runtime = build_runtime(layout_of(slot_symbols));
  std::map<std::string, std::int64_t> extra = sim.runtime.symbols;
  for (const auto& [name, addr] : slot_symbols) extra[name] = addr;
  AssembledProgram assembled = assemble(program, origin, extra);
  const auto main = assembled.symbols.find("main");
  if (main == assembled.symbols.end()) throw std::invalid_argument("program has no `main` function");
  const std::optional<std::int64_t> init = lookup(assembled.symbols, "__zp_init");
  install_runtime(assembled.image, sim.runtime, main->second, init);
  start(sim, std::move(assembled.image));
  sim.symbols = std::move(assembled.symbols);
  sim.origin = assembled.origin;
  sim.code_end = assembled.code_end;
  for (const asm_ast::Type_top_level& tl : program.top_level) {
    if (const auto* fn = std::get_if<asm_ast::Function>(&tl)) sim.function_names.insert(fn->name);
  }
  return sim;
}

Simulation build_linked_sim(std::string_view exe_bytes, std::int64_t origin) {
  const binutils::elf::ElfFile exe = binutils::elf::read(exe_bytes, "executable");
  if (exe.type != binutils::elf::ET_EXEC) throw std::runtime_error("build_linked_sim needs a linked executable");
  Simulation sim;
  std::set<std::string> own;
  for (const binutils::elf::SymbolPtr& s : exe.symbols) {
    if (!s->defined() || s->type == binutils::elf::STT_FILE) continue;
    if (s->is_global() || sim.symbols.count(s->name) == 0) sim.symbols[s->name] = s->value;
    if (!s->is_absolute()) own.insert(s->name);
  }
  sim.runtime = build_runtime(layout_of(sim.symbols));
  for (const auto& [name, addr] : sim.runtime.symbols) {
    if (own.count(name) != 0) continue;
    const auto it = sim.symbols.find(name);
    if (it != sim.symbols.end() && it->second != addr) {
      throw std::runtime_error("the executable has `" + name + "` at $" + hex4(it->second) +
                               ", and the runtime has it at $" + hex4(addr));
    }
    sim.symbols.emplace(name, addr);
  }
  std::vector<std::uint8_t> image(0x10000);
  std::int64_t end = origin;
  for (const binutils::elf::Segment& seg : exe.segments) {
    if (seg.paddr < 0 || seg.paddr + static_cast<std::int64_t>(seg.data.size()) > 0x10000) {
      throw std::runtime_error("the executable has a segment outside $0000-$FFFF");
    }
    std::copy(seg.data.begin(), seg.data.end(), image.begin() + seg.paddr);
    end = std::max(end, seg.paddr + static_cast<std::int64_t>(seg.data.size()));
  }
  const auto main = sim.symbols.find("main");
  if (main == sim.symbols.end()) throw std::runtime_error("the executable has no `main`");
  install_runtime(image, sim.runtime, main->second, lookup(sim.symbols, "__zp_init"));
  start(sim, std::move(image));
  sim.origin = origin;
  sim.code_end = end;
  return sim;
}

std::map<std::string, std::int64_t> runtime_defsyms() { return build_runtime().symbols; }

std::vector<std::string> defsym_args() {
  std::vector<std::string> out;
  for (const auto& [name, addr] : runtime_defsyms()) out.push_back("--defsym=" + name + "=0x" + hex4(addr));
  return out;
}

}  // namespace c6502::sim
