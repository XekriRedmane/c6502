#include "sim/game_run.h"

#include <cstdio>
#include <fstream>
#include <map>
#include <optional>
#include <ostream>
#include <sstream>
#include <stdexcept>
#include <string_view>

#include "binutils/elf.h"
#include "binutils/opcodes.h"
#include "sim/cpu_run.h"
#include "sim/games.h"

namespace c6502::sim::game_run {

namespace {

using games::Bytes;

struct Options {
  std::string game;
  std::string script;
  std::optional<std::string> exe;
  std::optional<std::size_t> events;
  std::string ld = "examples/ali_baba/ali_baba.ld";
  std::optional<std::size_t> steps_after;
  std::size_t steps = 0;
  std::optional<std::string> output;
};

struct UsageError : std::runtime_error {
  using std::runtime_error::runtime_error;
};

std::size_t number(const std::string& text, const std::string& option) {
  std::size_t used = 0;
  unsigned long long n = 0;
  try {
    n = std::stoull(text, &used);
  } catch (const std::exception&) {
    used = 0;
  }
  if (used != text.size() || text.empty()) throw UsageError(option + " takes a number, not " + text);
  return static_cast<std::size_t>(n);
}

Options parse(const std::vector<std::string>& args) {
  Options o;
  std::vector<std::string> positional;
  for (std::size_t i = 0; i < args.size(); ++i) {
    const std::string& a = args[i];
    const auto value = [&]() -> const std::string& {
      if (i + 1 >= args.size()) throw UsageError(a + " needs a value");
      return args[++i];
    };
    if (a == "--exe") {
      o.exe = value();
    } else if (a == "--events") {
      o.events = number(value(), a);
    } else if (a == "--ld") {
      o.ld = value();
    } else if (a == "--steps-after") {
      o.steps_after = number(value(), a);
    } else if (a == "--steps") {
      o.steps = number(value(), a);
    } else if (a == "-o") {
      o.output = value();
    } else if (!a.empty() && a[0] == '-') {
      throw UsageError("unknown option " + a);
    } else {
      positional.push_back(a);
    }
  }
  if (positional.size() != 2) throw UsageError("game-run takes a game and a script");
  o.game = positional[0];
  o.script = positional[1];
  return o;
}

std::string required_env(const char* name) {
  const std::optional<std::string> value = games::env(name);
  if (!value) throw std::runtime_error(std::string("set ") + name);
  return *value;
}

std::uint32_t crc(const Bytes& bytes) {
  return cpu_run::crc32(std::string_view(reinterpret_cast<const char*>(bytes.data()), bytes.size()));
}

std::uint32_t state(const Cpu& cpu) { return cpu_run::digest(cpu, {}); }

std::string hex_bytes(const Bytes& bytes) {
  std::string out;
  char buf[4];
  for (const std::uint8_t b : bytes) {
    std::snprintf(buf, sizeof buf, "%02X", b);
    out += buf;
  }
  return out;
}

// The digests of the events and the steps a run records, which every
// machine gives through its `on_event` and `after_step`.
struct Recorder {
  std::map<std::size_t, std::uint32_t> digests;
  std::optional<std::size_t> steps_after;
  std::size_t steps = 0;
  std::vector<std::string> step_lines;

  void event(std::size_t index, const Cpu& cpu) { digests[index] = state(cpu); }
  // The undefined opcodes executed, which py65 runs as a skip of one
  // byte, by opcode.
  std::map<int, std::uint64_t> undefined;

  void step(std::size_t events_so_far, const Cpu& cpu, std::uint8_t op) {
    if (binutils::opcodes::decode(op) == nullptr) ++undefined[op];
    if (!steps_after || events_so_far <= *steps_after || step_lines.size() >= steps) return;
    std::ostringstream line;
    line << "step " << cpu.pc << ' ' << int{cpu.a} << ' ' << int{cpu.x} << ' ' << int{cpu.y} << ' ' << int{cpu.sp}
         << ' ' << int{cpu.p} << ' ' << cpu.cycles;
    step_lines.push_back(line.str());
  }
  std::string digest(std::size_t index) const {
    const auto it = digests.find(index);
    return it == digests.end() ? "-" : std::to_string(it->second);
  }
  void write_steps(std::ostream& out) const {
    for (const std::string& line : step_lines) out << line << '\n';
    out << "undefined";
    for (const auto& [op, n] : undefined) out << ' ' << op << ':' << n;
    out << '\n';
  }
};

binutils::elf::ElfFile read_exe(const std::string& path) {
  const Bytes bytes = games::read_bytes(path);
  return binutils::elf::read(std::string_view(reinterpret_cast<const char*>(bytes.data()), bytes.size()), path);
}

void run_zork(const Options& o, std::ostream& out) {
  const Bytes dsk = games::read_bytes(required_env("ZORK_DSK"));
  const games::ZorkSession& session = games::zork_session(o.script);
  std::unique_ptr<games::ZorkRun> run;
  if (o.exe) {
    const Bytes exe = games::read_bytes(*o.exe);
    run = games::zork_program(std::string_view(reinterpret_cast<const char*>(exe.data()), exe.size()), dsk, session);
  } else {
    run = games::zork_original(dsk, session);
  }
  Recorder rec{{}, o.steps_after, o.steps, {}};
  apple2::Apple2& a = run->machine;
  a.on_call = [&](const Cpu& cpu) { rec.event(a.calls.size() - 1, cpu); };
  a.after_step = [&](const Cpu& cpu, std::uint8_t op) { rec.step(a.calls.size(), cpu, op); };
  const apple2::Stop stop = run->run();
  for (std::size_t i = 0; i < a.calls.size(); ++i) {
    out << "call " << i << ' ';
    if (const auto* m = std::get_if<apple2::MonitorCall>(&a.calls[i])) {
      out << apple2::routine_name(m->routine) << ' ' << (m->a ? std::to_string(*m->a) : "-") << ' ' << int{m->ch}
          << ' ' << int{m->cv} << ' ' << int{m->invflg};
    } else {
      const auto& r = std::get<apple2::RwtsCall>(a.calls[i]);
      out << "RWTS " << int{r.slot} << ' ' << int{r.drive} << ' ' << int{r.track} << ' ' << int{r.sector} << ' '
          << int{r.command};
    }
    out << " state " << rec.digest(i) << '\n';
  }
  for (const apple2::SectorWrite& w : a.written) {
    out << "write " << int{w.slot} << ' ' << int{w.drive} << ' ' << int{w.track} << ' ' << int{w.sector} << '\n';
  }
  out << "printer " << a.printer.size() << ' ' << crc(a.printer) << '\n';
  const std::string text = a.text();
  out << "text " << cpu_run::crc32(text) << '\n';
  for (const auto& [key, disk] : a.disks) {
    out << "disk " << int{key.first} << ' ' << int{key.second} << ' ' << crc(disk) << '\n';
  }
  rec.write_steps(out);
  out << "end state " << state(run->cpu) << " memory " << crc(run->cpu.memory) << " instructions "
      << a.instructions << " why " << apple2::stop_name(stop) << '\n';
}

std::string lode_runner_end(lode_runner::End end) {
  switch (end) {
    case lode_runner::End::kCycles: return "cycles";
    case lode_runner::End::kStop: return "stop";
    case lode_runner::End::kBrk: return "brk";
    case lode_runner::End::kReturned: return "returned";
  }
  return "?";
}

void run_lode_runner(const Options& o, std::ostream& out) {
  const games::LodeRunnerFiles files = games::LodeRunnerFiles::read(required_env("LODE_RUNNER_DIR"));
  const games::LodeRunnerScript& script = games::lode_runner_script(o.script);
  const Bytes disk = games::lode_runner_disk(files.disk, script.disk);
  const auto original = [&]() { return lode_runner::original_machine(files.image, disk, script.keys); };
  std::optional<lode_runner::Machine> machine;
  std::function<bool(const lode_runner::Machine&)> stop;
  std::uint64_t max_cycles = script.max_cycles;
  if (o.exe) {
    std::size_t n = 0;
    if (o.events) {
      n = *o.events;
    } else {
      lode_runner::Machine first = original();
      first.run(script.max_cycles, games::mode_change(script.mode_before, script.mode_after));
      n = first.events.size();
    }
    machine.emplace(lode_runner::program_machine(files.image, read_exe(*o.exe), disk, script.keys));
    max_cycles = machine->cpu.cycles + 2 * script.max_cycles;
    stop = [n](const lode_runner::Machine& m) { return m.events.size() >= n; };
  } else {
    machine.emplace(original());
    stop = games::mode_change(script.mode_before, script.mode_after);
  }
  Recorder rec{{}, o.steps_after, o.steps, {}};
  lode_runner::Machine& m = *machine;
  m.on_event = [&](const Cpu& cpu) { rec.event(m.events.size() - 1, cpu); };
  m.after_step = [&](const Cpu& cpu, std::uint8_t op) { rec.step(m.events.size(), cpu, op); };
  const lode_runner::End end = m.run(max_cycles, stop);
  for (std::size_t i = 0; i < m.events.size(); ++i) {
    const lode_runner::Event& e = m.events.events[i];
    out << "event " << i << ' ' << e.code << ' ' << e.page1 << ' ' << e.page2 << ' ' << e.pc << ' ' << e.cycles
        << " state " << rec.digest(i);
    if (const auto r = m.events.rwts.find(i); r != m.events.rwts.end()) {
      out << " rwts " << int{r->second.command} << ' ' << int{r->second.track} << ' ' << int{r->second.sector} << ' '
          << crc(r->second.buffer);
    }
    out << '\n';
  }
  rec.write_steps(out);
  out << "end state " << state(m.cpu) << " memory " << crc(m.cpu.memory) << " disk " << crc(m.disk) << " kbd "
      << m.kbd_reads << ' ' << int{m.kbd_latch} << " display " << m.text << ' ' << m.mixed << ' ' << m.page2 << ' '
      << m.hires << " instructions " << m.instructions << " why " << lode_runner_end(end) << '\n';
}

void run_ali_baba(const Options& o, std::ostream& out) {
  games::AliBabaFiles files(required_env("ALI_BABA_DIR"));
  const games::AliBabaScript& script = games::ali_baba_script(o.script);
  std::unique_ptr<ali_baba::Machine> machine;
  std::function<bool(const ali_baba::Machine&)> stop;
  std::uint64_t max_cycles = script.max_cycles;
  if (o.exe) {
    std::size_t n = o.events ? *o.events : files.run_original(script)->events.size();
    machine = games::ali_baba_program(files, read_exe(*o.exe), games::read_text(o.ld), script);
    max_cycles = machine->cpu.cycles + 2 * script.max_cycles;
    stop = [n](const ali_baba::Machine& m) { return m.events.size() >= n; };
  } else {
    machine = files.original(script);
    const int end = script.end;
    stop = [end](const ali_baba::Machine& m) { return m.kbd_reads >= end; };
  }
  Recorder rec{{}, o.steps_after, o.steps, {}};
  ali_baba::Machine& m = *machine;
  m.on_event = [&](const Cpu& cpu) { rec.event(m.events.size() - 1, cpu); };
  m.after_step = [&](const Cpu& cpu, std::uint8_t op) { rec.step(m.events.size(), cpu, op); };
  const ali_baba::End end = m.run(max_cycles, stop);
  for (std::size_t i = 0; i < m.events.size(); ++i) {
    const ali_baba::Event& e = m.events.events[i];
    out << "event " << i << ' ' << e.code << ' ' << e.page1 << ' ' << e.arg << ' ' << e.pc << ' ' << e.cycles
        << " state " << rec.digest(i);
    if (const auto d = m.events.disk.find(i); d != m.events.disk.end()) {
      const ali_baba::DiskRecord& r = d->second;
      out << " disk " << r.entry << ' ' << int{r.carry} << ' ' << r.half_track << ' '
          << (r.iob.empty() ? "-" : hex_bytes(r.iob)) << ' ' << (r.buffer.empty() ? "-" : std::to_string(crc(r.buffer)));
    }
    out << '\n';
  }
  rec.write_steps(out);
  const disk2::DiskII& c = m.disk;
  out << "controller " << c.selected << ' ' << c.motor << ' ' << c.motor_off_at << ' ' << c.q6 << ' ' << c.q7 << ' '
      << int{c.latch} << ' ' << c.position << '\n';
  for (const auto& [name, drive] : m.media) {
    Bytes tracks;
    for (const Bytes& t : drive.tracks) tracks.insert(tracks.end(), t.begin(), t.end());
    out << "medium " << name << " dsk " << crc(drive.to_dsk()) << " tracks " << crc(tracks) << " head "
        << drive.half_track << ' ' << drive.phases << '\n';
  }
  out << "end state " << state(m.cpu) << " memory " << crc(m.cpu.memory) << " seen " << crc(m.seen) << " kbd "
      << m.kbd_reads << ' ' << int{m.kbd_latch} << " display " << m.text << ' ' << m.mixed << ' ' << m.page2 << ' '
      << m.hires << ' ' << m.display_set << " instructions " << m.instructions << " why " << ali_baba::end_name(end)
      << '\n';
}

}  // namespace

int main(const std::vector<std::string>& args, std::ostream& out, std::ostream& err) {
  Options o;
  try {
    o = parse(args);
  } catch (const UsageError& e) {
    err << "game-run: " << e.what() << '\n';
    return 2;
  }
  std::ofstream file;
  if (o.output) {
    file.open(*o.output, std::ios::binary);
    if (!file) {
      err << "game-run: can't write " << *o.output << '\n';
      return 1;
    }
  }
  std::ostream& to = o.output ? file : out;
  try {
    if (o.game == "zork") {
      run_zork(o, to);
    } else if (o.game == "lode_runner") {
      run_lode_runner(o, to);
    } else if (o.game == "ali_baba") {
      run_ali_baba(o, to);
    } else {
      err << "game-run: no game " << o.game << " (zork, lode_runner, or ali_baba)\n";
      return 2;
    }
  } catch (const std::exception& e) {
    err << "game-run: " << e.what() << '\n';
    return 1;
  }
  return 0;
}

}  // namespace c6502::sim::game_run
