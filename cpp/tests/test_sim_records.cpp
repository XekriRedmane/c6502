// The comparison of the C++ harness with the Python one, for group G4 of
// step 9 of the port. With C6502_SIM_RECORDS naming a directory of the
// records that sim/harness.py wrote for each `Simulation.run` of the
// Python suite (one JSON object a line, in `*.jsonl` files), this case
// builds each program again in C++, compares the image and the symbols,
// runs it from the same state, and compares the registers, the cycles,
// the memory, and the hook's error. Without the variable it does
// nothing. It goes with the Python harness, in group R.
#include <algorithm>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "doctest.h"
#include "json.h"
#include "sim/cpu_run.h"
#include "sim_case.h"

namespace c6502::suite {
namespace {

std::int64_t num(const json::Value& v, std::string_view key) {
  const json::Value* m = v.get(key);
  if (m == nullptr) throw std::runtime_error("the record has no " + std::string(key));
  return static_cast<std::int64_t>(m->integer);
}

std::string from_hex(const std::string& hex) {
  std::string out;
  for (std::size_t i = 0; i + 1 < hex.size(); i += 2) {
    out.push_back(static_cast<char>(std::stoi(hex.substr(i, 2), nullptr, 16)));
  }
  return out;
}

std::uint32_t crc_of(const std::vector<std::uint8_t>& memory) {
  return sim::cpu_run::crc32(std::string_view(reinterpret_cast<const char*>(memory.data()), memory.size()));
}

std::string regs(const sim::Cpu& c) {
  std::ostringstream s;
  s << "pc=" << c.pc << " a=" << int(c.a) << " x=" << int(c.x) << " y=" << int(c.y) << " sp=" << int(c.sp)
    << " p=" << int(c.p) << " cycles=" << c.cycles;
  return s.str();
}

std::string regs(const json::Value& r) {
  std::ostringstream s;
  s << "pc=" << num(r, "pc") << " a=" << num(r, "a") << " x=" << num(r, "x") << " y=" << num(r, "y")
    << " sp=" << num(r, "sp") << " p=" << num(r, "p") << " cycles=" << num(r, "cycles");
  return s.str();
}

struct Tally {
  int records = 0, other = 0, built = 0, build_errors = 0, images = 0, symbols = 0, runs = 0;
  int errors_equal = 0, differ = 0;
  std::map<std::string, int> hooks_called;
};

// Returns "" when the record's run matches, or what differs.
std::string compare(const json::Value& rec, Tally& t) {
  const json::Value& build = *rec.get("build");
  const std::string kind = build.get("kind")->string;
  if (kind == "other") {
    ++t.other;
    return "";
  }
  sim::Simulation s;
  try {
    if (kind == "source") {
      SimOptions o;
      o.optimize = build.get("optimize")->boolean;
      o.inline_calls = build.get("inline")->boolean;
      o.check_ir = build.get("check_ir")->boolean;
      o.origin = num(build, "origin");
      s = build_sim(build.get("source")->string, o);
    } else {
      s = sim::build_linked_sim(from_hex(build.get("exe")->string), num(build, "origin"));
    }
  } catch (const std::exception& e) {
    ++t.build_errors;
    return std::string("build failed: ") + e.what();
  }
  ++t.built;
  std::string why;
  if (crc_of(s.cpu.memory) == static_cast<std::uint32_t>(num(rec, "build_crc"))) {
    ++t.images;
  } else {
    why += "image differs; ";
  }
  std::vector<std::pair<std::string, std::int64_t>> want;
  for (const json::Value& p : rec.get("symbols")->items) {
    want.emplace_back(p.items[0].string, static_cast<std::int64_t>(p.items[1].integer));
  }
  const std::vector<std::pair<std::string, std::int64_t>> got(s.symbols.begin(), s.symbols.end());
  if (want == got) {
    ++t.symbols;
  } else {
    why += "symbols differ; ";
  }
  // The hooks the Python run had, by name.
  std::map<std::int64_t, std::string> names;
  for (const auto& [name, addr] : s.runtime.symbols) names[addr] = name;
  std::vector<std::string> hooks;
  for (const json::Value& h : rec.get("hooks")->items) hooks.push_back(h.string);
  for (auto it = s.runtime.hooks.begin(); it != s.runtime.hooks.end();) {
    if (std::find(hooks.begin(), hooks.end(), names[it->first]) == hooks.end()) {
      it = s.runtime.hooks.erase(it);
    } else {
      ++it;
    }
  }
  if (s.runtime.hooks.size() != hooks.size()) why += "hooks differ; ";
  for (auto& [addr, hook] : s.runtime.hooks) {
    hook = [inner = hook, name = names[addr], &t](std::vector<std::uint8_t>& m) {
      ++t.hooks_called[name];
      inner(m);
    };
  }
  for (const json::Value& p : rec.get("patch")->items) {
    s.cpu.memory[static_cast<std::size_t>(p.items[0].integer)] = static_cast<std::uint8_t>(p.items[1].integer);
  }
  const json::Value& start = *rec.get("start");
  s.cpu.pc = static_cast<std::uint16_t>(num(start, "pc"));
  s.cpu.a = static_cast<std::uint8_t>(num(start, "a"));
  s.cpu.x = static_cast<std::uint8_t>(num(start, "x"));
  s.cpu.y = static_cast<std::uint8_t>(num(start, "y"));
  s.cpu.sp = static_cast<std::uint8_t>(num(start, "sp"));
  s.cpu.p = static_cast<std::uint8_t>(num(start, "p"));
  s.cpu.cycles = static_cast<std::uint64_t>(num(start, "cycles"));
  const std::vector<std::uint8_t> start_memory = s.cpu.memory;
  std::string error_type, error_message;
  bool timed_out = false;
  try {
    timed_out = s.run(static_cast<std::uint64_t>(num(rec, "max_cycles"))).timed_out;
  } catch (const sim::HookError& e) {
    error_type = e.python_type();
    error_message = e.what();
  } catch (const sim::CpuError& e) {
    error_type = "IndexError";
    error_message = e.what();
  }
  const json::Value* err = rec.get("error");
  if (err != nullptr) {
    if (error_type != err->get("type")->string ||
        (error_type != "IndexError" && error_message != err->get("message")->string)) {
      why += "error differs: C++ " + error_type + ": " + error_message + ", Python " + err->get("type")->string +
             ": " + err->get("message")->string + "; ";
    } else {
      ++t.errors_equal;
    }
  } else if (!error_type.empty()) {
    why += "C++ raised " + error_type + ": " + error_message + "; ";
  } else if (timed_out != rec.get("timed_out")->boolean) {
    why += "timed_out differs; ";
  }
  const std::string want_regs = regs(*rec.get("end"));
  if (regs(s.cpu) != want_regs) why += "registers: C++ " + regs(s.cpu) + ", Python " + want_regs + "; ";
  if (crc_of(s.cpu.memory) != static_cast<std::uint32_t>(num(rec, "crc"))) {
    std::vector<std::uint8_t> want_memory = start_memory;
    for (const json::Value& p : rec.get("diff")->items) {
      want_memory[static_cast<std::size_t>(p.items[0].integer)] = static_cast<std::uint8_t>(p.items[1].integer);
    }
    int shown = 0;
    std::ostringstream d;
    for (std::size_t a = 0; a < want_memory.size() && shown < 8; ++a) {
      if (want_memory[a] != s.cpu.memory[a]) {
        d << " $" << std::hex << a << ": C++ " << int(s.cpu.memory[a]) << " Python " << int(want_memory[a])
          << std::dec;
        ++shown;
      }
    }
    why += "memory differs:" + d.str() + "; ";
  }
  if (why.empty()) ++t.runs;
  return why;
}

}  // namespace

TEST_CASE("sim records: the C++ harness runs each recorded program as the Python one did") {
  const char* dir = std::getenv("C6502_SIM_RECORDS");
  if (dir == nullptr) {
    MESSAGE("C6502_SIM_RECORDS isn't set");
    return;
  }
  Tally t;
  std::ostringstream report;
  std::vector<std::filesystem::path> files;
  for (const auto& entry : std::filesystem::directory_iterator(dir)) {
    if (entry.path().extension() == ".jsonl") files.push_back(entry.path());
  }
  std::sort(files.begin(), files.end());
  for (const std::filesystem::path& file : files) {
    std::ifstream in(file, std::ios::binary);
    std::string line;
    while (std::getline(in, line)) {
      if (line.empty()) continue;
      ++t.records;
      const json::Value rec = json::parse(line);
      const std::string why = compare(rec, t);
      if (!why.empty()) {
        ++t.differ;
        if (t.differ <= 200) report << rec.get("test")->string << ": " << why << "\n";
      }
    }
  }
  std::ostringstream counts;
  counts << "records " << t.records << ", not built by build_sim or build_linked_sim " << t.other << ", built "
         << t.built << ", build errors " << t.build_errors << ", images equal " << t.images << ", symbols equal "
         << t.symbols << ", runs equal " << t.runs << " (errors equal " << t.errors_equal << "), differ "
         << t.differ << "\nhook calls:";
  for (const auto& [name, n] : t.hooks_called) counts << " " << name << "=" << n;
  const char* out = std::getenv("C6502_SIM_RECORDS_REPORT");
  if (out != nullptr) {
    std::ofstream f(out, std::ios::binary);
    f << counts.str() << "\n" << report.str();
  }
  MESSAGE(counts.str());
  CHECK(t.differ == 0);
}

}  // namespace c6502::suite
