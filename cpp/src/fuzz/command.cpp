#include "fuzz/command.h"

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <exception>
#include <filesystem>
#include <map>
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#else
#include <unistd.h>
#endif

#include "argparse.h"
#include "compile_command.h"
#include "diagnostics.h"
#include "errors.h"
#include "fuzz/bit_precise_types.h"
#include "fuzz/calls.h"
#include "fuzz/exprs.h"
#include "fuzz/reduce.h"
#include "fuzz/runner.h"
#include "fuzz/stmts.h"
#include "fuzz/yarpgen.h"
#include "passes/ir_check.h"
#include "processes.h"
#include "py_int.h"
#include "tac_sim_modes.h"

namespace c6502::fuzz {

namespace {

namespace fs = std::filesystem;

constexpr const char* kUsage =
    "usage: c6502cpp fuzz GENERATOR [--start N] [--count N] [--out DIR] [-j N] [--verbose]\n"
    "       c6502cpp fuzz programs GENERATOR [--start N] [--count N] --out DIR\n"
    "       c6502cpp fuzz run PATH...\n"
    "       c6502cpp fuzz reduce [--tac-opt] PATH\n"
    "       c6502cpp fuzz tac-passes PATH...\n"
    "GENERATOR is exprs, calls, stmts, or yarpgen.\n";

enum class Generator { kExprs, kCalls, kStmts, kYarpgen };

std::optional<Generator> generator_named(const std::string& name) {
  if (name == "exprs") return Generator::kExprs;
  if (name == "calls") return Generator::kCalls;
  if (name == "stmts") return Generator::kStmts;
  if (name == "yarpgen") return Generator::kYarpgen;
  return std::nullopt;
}

void out(const std::string& text) {
  std::fputs(text.c_str(), stdout);
  std::fflush(stdout);
}

void err(const std::string& text) {
  std::fputs(text.c_str(), stderr);
  std::fflush(stderr);
}

struct Setup {
  explicit Setup(const std::string& self)
      : self(processes::self_path(self)),
        include_dir(compile_command::find_include_dir(self)),
        root(fs::path(compile_command::utf8_path(include_dir)).parent_path()),
        runner(include_dir) {}
  fs::path self;
  std::string include_dir;
  fs::path root;
  Runner runner;
};

GeneratedProgram make_program(Generator g, Int seed, const Setup& setup) {
  switch (g) {
    case Generator::kExprs: return exprs::make_program(seed);
    case Generator::kCalls: return calls::make_program(seed);
    case Generator::kStmts: return stmts::make_program(seed);
    case Generator::kYarpgen: return yarpgen::make_program(seed, yarpgen::binary(setup.root));
  }
  throw std::logic_error("no such generator");
}

struct SeedOutcome {
  std::string source;
  Results results;
  std::vector<Mode> bad;
};

SeedOutcome check_seed(Generator g, Int seed, const Setup& setup) {
  GeneratedProgram p = make_program(g, seed, setup);
  SeedOutcome o;
  o.results = setup.runner.run_all(p.source);
  o.bad = disagreements(o.results, p.expected);
  o.source = std::move(p.source);
  return o;
}

void put_section(std::string& rec, const std::string& key, const std::string& body) {
  rec += key + " " + std::to_string(body.size()) + "\n" + body;
}

std::string record_of(const SeedOutcome& o) {
  std::string rec;
  put_section(rec, "source", o.source);
  for (Mode m : kModes) {
    const Result& r = o.results[m];
    if (is_error(r)) {
      put_section(rec, std::string(mode_name(m)) + " text", std::get<std::string>(r));
    } else {
      put_section(rec, std::string(mode_name(m)) + " int", std::to_string(std::get<std::int64_t>(r)));
    }
  }
  std::string bad;
  for (Mode m : o.bad) bad += std::string(bad.empty() ? "" : " ") + mode_name(m);
  put_section(rec, "bad", bad);
  return rec;
}

SeedOutcome outcome_of(const std::string& rec) {
  std::map<std::string, std::string> sections;
  for (std::size_t pos = 0; pos < rec.size();) {
    const std::size_t nl = rec.find('\n', pos);
    if (nl == std::string::npos) throw std::runtime_error("a worker's record is cut short");
    const std::string head = rec.substr(pos, nl - pos);
    const std::size_t sp = head.rfind(' ');
    const std::size_t n = std::stoul(head.substr(sp + 1));
    sections[head.substr(0, sp)] = rec.substr(nl + 1, n);
    pos = nl + 1 + n;
  }
  SeedOutcome o;
  o.source = sections.at("source");
  for (Mode m : kModes) {
    const std::string name = mode_name(m);
    if (auto it = sections.find(name + " text"); it != sections.end()) {
      o.results[m] = it->second;
    } else {
      o.results[m] = static_cast<std::int64_t>(std::stoll(sections.at(name + " int")));
    }
  }
  const std::string& bad = sections.at("bad");
  for (Mode m : kModes) {
    const std::string name = mode_name(m);
    for (std::size_t at = 0; at < bad.size();) {
      std::size_t end = bad.find(' ', at);
      if (end == std::string::npos) end = bad.size();
      if (bad.substr(at, end - at) == name) o.bad.push_back(m);
      at = end + 1;
    }
  }
  return o;
}

bool is_int(const std::string& text) {
  try {
    py_int::parse(text, 10, "");
    return true;
  } catch (const CompileError&) {
    return false;
  }
}

std::int64_t int_value(const argparse::Namespace& ns, const std::string& dest, std::int64_t fallback) {
  const std::string* v = ns.value(dest);
  return v == nullptr ? fallback : static_cast<std::int64_t>(py_int::parse(*v, 10, dest));
}

argparse::Namespace parse(const argparse::Parser& ap, const std::vector<std::string>& args) {
  return ap.parse_args(args, argparse::terminal_columns());
}

long process_id() {
#ifdef _WIN32
  return static_cast<long>(GetCurrentProcessId());
#else
  return static_cast<long>(getpid());
#endif
}

bool report(const std::string& prog, std::int64_t seed, const SeedOutcome& o, const fs::path& dir,
            bool verbose, int& rejected) {
  const std::string head = "seed " + std::to_string(seed) + ": ";
  const bool only_limits =
      !o.bad.empty() && std::find(o.bad.begin(), o.bad.end(), Mode::kTac) == o.bad.end() &&
      std::all_of(o.bad.begin(), o.bad.end(), [&](Mode m) { return is_backend_limit(o.results[m]); });
  if (only_limits) {
    ++rejected;
    out(head + "rejected " + format_result(o.results) + "\n");
    return false;
  }
  if (o.bad.empty()) {
    if (verbose) out(head + "ok " + format_result(o.results) + "\n");
    return false;
  }
  const std::string path = compile_command::path_text(dir / (prog + std::to_string(seed) + ".c"));
  if (compile_command::write_bytes(path, o.source) != 0) {
    throw std::runtime_error("can't write " + path);
  }
  out(head + "WRONG " + format_result(o.results) + " -> " + argparse::path_str(path) + "\n");
  return true;
}

int fuzz_main(Generator g, const std::string& prog, const std::vector<std::string>& args,
              const Setup& setup) {
  argparse::Parser ap("c6502cpp fuzz " + prog, "");
  ap.add_store({"--start"}, "start", "first seed", "", "int", is_int);
  ap.add_store({"--count"}, "count", "number of seeds", "", "int", is_int);
  ap.add_store({"--out"}, "out", "directory for failing programs (default fuzz_out/<name>)");
  ap.add_store({"-j", "--jobs"}, "jobs", "processes to run seeds in (default 1)", "", "int", is_int);
  ap.add_store_true({"--verbose"}, "also write a line for each program that passed");
  const argparse::Namespace ns = parse(ap, args);
  const std::int64_t start = int_value(ns, "start", 0);
  const std::int64_t count = int_value(ns, "count", 100);
  std::int64_t jobs = int_value(ns, "jobs", 1);
  if (jobs > 0 && static_cast<std::uint64_t>(jobs) > processes::max_children()) {
    jobs = static_cast<std::int64_t>(processes::max_children());
  }
  const fs::path dir = ns.value("out") ? compile_command::utf8_path(*ns.value("out"))
                                       : fs::path("fuzz_out") / prog;
  const bool verbose = ns.flag("verbose");
  if (g == Generator::kYarpgen && !fs::exists(yarpgen::binary(setup.root))) {
    err(argparse::path_str(compile_command::path_text(yarpgen::binary(setup.root))) +
        " not found; run bash fuzz/build_yarpgen.sh\n");
    return 1;
  }
  fs::create_directories(dir);
  int wrong = 0;
  int rejected = 0;
  if (jobs <= 1) {
    for (std::int64_t seed = start; seed < start + count; ++seed) {
      wrong += report(prog, seed, check_seed(g, seed, setup), dir, verbose, rejected) ? 1 : 0;
    }
  } else {
    const fs::path records = fs::temp_directory_path() / ("c6502-fuzz-" + std::to_string(process_id()));
    fs::create_directories(records);
    auto record_path = [&](std::int64_t seed) {
      return compile_command::path_text(records / ("seed" + std::to_string(seed) + ".txt"));
    };
    processes::Children children;
    std::map<std::size_t, std::int64_t> seed_of;
    std::map<std::int64_t, int> done;
    std::int64_t next = start;
    std::int64_t reported = start;
    int failed = 0;
    while (reported < start + count) {
      while (next < start + count && static_cast<std::int64_t>(children.running()) < jobs) {
        const std::size_t id = children.start({compile_command::path_text(setup.self), "fuzz", "worker",
                                               prog, std::to_string(next), record_path(next)});
        seed_of[id] = next++;
      }
      const processes::Children::Exit e = children.wait_any();
      done[seed_of.at(e.id)] = e.status;
      for (auto it = done.find(reported); it != done.end(); it = done.find(reported)) {
        if (it->second != 0) {
          err("seed " + std::to_string(reported) + ": the worker failed with status " +
              std::to_string(it->second) + "\n");
          ++failed;
        } else {
          const SeedOutcome o = outcome_of(compile_command::read_text(record_path(reported)));
          wrong += report(prog, reported, o, dir, verbose, rejected) ? 1 : 0;
        }
        done.erase(it);
        ++reported;
      }
    }
    std::error_code ec;
    fs::remove_all(records, ec);
    if (failed != 0) {
      out(std::to_string(wrong) + " wrong, " + std::to_string(rejected) + " rejected, of " +
          std::to_string(count) + " programs, and " + std::to_string(failed) + " workers failed\n");
      return 1;
    }
  }
  out(std::to_string(wrong) + " wrong, " + std::to_string(rejected) + " rejected, of " +
      std::to_string(count) + " programs\n");
  return wrong != 0 ? 1 : 0;
}

int worker(const std::vector<std::string>& args, const Setup& setup) {
  const std::optional<Generator> g = args.size() == 3 ? generator_named(args[0]) : std::nullopt;
  if (!g) {
    err(kUsage);
    return 2;
  }
  const SeedOutcome o = check_seed(*g, py_int::parse(args[1], 10, "seed"), setup);
  return compile_command::write_bytes(args[2], record_of(o)) == 0 ? 0 : 1;
}

int programs(const std::vector<std::string>& args, const Setup& setup) {
  const std::optional<Generator> g = args.empty() ? std::nullopt : generator_named(args[0]);
  if (!g) {
    err(kUsage);
    return 2;
  }
  argparse::Parser ap("c6502cpp fuzz programs " + args[0], "");
  ap.add_store({"--start"}, "start", "first seed", "", "int", is_int);
  ap.add_store({"--count"}, "count", "number of seeds", "", "int", is_int);
  ap.add_store({"--out"}, "out", "directory for the programs");
  const argparse::Namespace ns = parse(ap, std::vector<std::string>(args.begin() + 1, args.end()));
  if (ns.value("out") == nullptr) {
    err("c6502cpp fuzz programs: --out is required\n");
    return 2;
  }
  const fs::path dir = compile_command::utf8_path(*ns.value("out"));
  fs::create_directories(dir);
  const std::int64_t start = int_value(ns, "start", 0);
  const std::int64_t count = int_value(ns, "count", 100);
  for (std::int64_t seed = start; seed < start + count; ++seed) {
    const GeneratedProgram p = make_program(*g, seed, setup);
    const std::string path = compile_command::path_text(dir / (args[0] + std::to_string(seed) + ".c"));
    if (compile_command::write_bytes(path, p.source) != 0) throw std::runtime_error("can't write " + path);
    out(std::to_string(seed) + " " + (p.expected ? std::to_string(*p.expected) : "None") + "\n");
  }
  return 0;
}

std::vector<fs::path> files_of(const std::vector<std::string>& paths) {
  std::vector<fs::path> files;
  for (const std::string& p : paths) {
    const fs::path path = compile_command::utf8_path(p);
    if (!fs::is_directory(path)) {
      files.push_back(path);
      continue;
    }
    std::vector<fs::path> found;
    for (const auto& entry : fs::directory_iterator(path)) {
      if (entry.path().extension() == ".c") found.push_back(entry.path());
    }
    std::sort(found.begin(), found.end(), [](const fs::path& a, const fs::path& b) {
      return compile_command::path_text(a) < compile_command::path_text(b);
    });
    files.insert(files.end(), found.begin(), found.end());
  }
  return files;
}

int run_files(const std::vector<std::string>& args, const Setup& setup) {
  argparse::Parser ap("c6502cpp fuzz run",
                      "Run C files through the TAC interpreter and both asm pipelines.");
  ap.add_positional("paths");
  const argparse::Namespace ns = parse(ap, args);
  const std::vector<fs::path> files = files_of(*ns.list("paths"));
  int bad = 0;
  for (const fs::path& f : files) {
    const std::string src = compile_command::read_text(compile_command::path_text(f));
    const std::optional<std::int64_t> expected = expected_of(src);
    const Results res = setup.runner.run_all(src);
    const bool wrong = !disagreements(res, expected).empty();
    bad += wrong ? 1 : 0;
    std::string name = compile_command::path_text(f.filename());
    if (name.size() < 32) name.resize(32, ' ');
    out(std::string(wrong ? "FAIL" : "ok  ") + " " + name + " exp=" +
        (expected ? std::to_string(*expected) : "None") + " " + format_result(res) + "\n");
  }
  out(std::to_string(bad) + " failing of " + std::to_string(files.size()) + "\n");
  return bad != 0 ? 1 : 0;
}

std::string tac_results_text(const std::optional<TacResults>& r) {
  if (!r) return "None";
  auto value = [](const std::optional<asdl::Int>& v) { return v ? decimal(*v) : std::string("None"); };
  return "(" + value(r->plain) + ", " + value(r->optimized) + ")";
}

int reduce_file(const std::vector<std::string>& args, const Setup& setup) {
  argparse::Parser ap("c6502cpp fuzz reduce", "");
  ap.add_positional("path");
  ap.add_store_true({"--tac-opt"}, "reduce a TAC-optimizer failure (plain vs optimized TAC)");
  const argparse::Namespace ns = parse(ap, args);
  const std::vector<std::string>& paths = *ns.list("path");
  if (paths.size() != 1) {
    err("c6502cpp fuzz reduce: give one path\n");
    return 2;
  }
  const fs::path path = compile_command::utf8_path(paths[0]);
  const std::string src = compile_command::read_text(paths[0]);
  const bool tac_opt = ns.flag("tac_opt");
  std::string reduced;
  try {
    const Predicate still_fails =
        make_predicate(setup.runner, src, tac_opt ? Failure::kTacOptimizer : Failure::kPaths);
    if (!still_fails(src)) {
      err("the input doesn't fail\n");
      return 1;
    }
    reduced = reduce(src, still_fails);
  } catch (const InternalError& e) {
    err("c6502cpp fuzz reduce: internal error while checking a candidate: " + std::string(e.what()) +
        "\n");
    return 3;
  }
  fs::path dst = path;
  dst.replace_extension(".min.c");
  if (compile_command::write_bytes(compile_command::path_text(dst), reduced) != 0) {
    throw std::runtime_error("can't write " + compile_command::path_text(dst));
  }
  out(reduced + "\n");
  if (tac_opt) {
    out("tac plain/opt: " + tac_results_text(tac_results(setup.runner, reduced)) + "\n");
  } else {
    out(format_result(setup.runner.run_all(reduced)) + "\n");
  }
  out("-> " + argparse::path_str(compile_command::path_text(dst)) + "\n");
  return 0;
}

std::string observed_text(const tac_sim::Outcome& o) {
  if (o.error_type) return "ERR " + *o.error_type + ": " + o.error_message;
  return o.value ? decimal(*o.value) : "None";
}

bool env_is_one(const char* name) {
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4996)
#endif
  const char* v = std::getenv(name);
#ifdef _MSC_VER
#pragma warning(pop)
#endif
  return v != nullptr && std::string(v) == "1";
}

int tac_passes(const std::vector<std::string>& args, const Setup& setup) {
  if (args.empty()) {
    err(kUsage);
    return 2;
  }
  const passes::ir_check::Enabled checks(env_is_one("C6502_CHECK_IR"));
  passes::FrontEndOptions front;
  front.include_dir = setup.include_dir;
  for (const std::string& path : args) {
    std::string pp_err;
    std::vector<diagnostics::CompileWarning> warnings;
    tac_sim_modes::Translated t;
    {
      diagnostics::Collecting block(warnings);
      t = tac_sim_modes::translate(compile_command::read_text(path), front, pp_err);
    }
    err(pp_err);
    const std::optional<tac_sim_modes::ChangingPass> found = tac_sim_modes::first_changing_pass(t);
    if (!found) {
      out(path + ": no TAC pass changes main()'s result\n");
    } else {
      out(path + ": after " + found->pass + " in " + found->function + ", main() returns " +
          observed_text(found->got) + " instead of " + observed_text(found->expected) + "\n");
    }
  }
  return 0;
}

}  // namespace

int command(const std::vector<std::string>& args, const std::string& self) {
  if (args.empty()) {
    err(kUsage);
    return 2;
  }
  try {
    const Setup setup(self);
    const std::vector<std::string> rest(args.begin() + 1, args.end());
    if (args[0] == "worker") return worker(rest, setup);
    if (args[0] == "programs") return programs(rest, setup);
    if (args[0] == "run") return run_files(rest, setup);
    if (args[0] == "reduce") return reduce_file(rest, setup);
    if (args[0] == "tac-passes") return tac_passes(rest, setup);
    if (const std::optional<Generator> g = generator_named(args[0])) {
      return fuzz_main(*g, args[0], rest, setup);
    }
  } catch (const argparse::Exit& e) {
    out(e.out);
    err(e.err);
    return e.status;
  } catch (const compile_command::Raised& e) {
    err("c6502cpp fuzz: " + e.type + ": " + e.message + "\n");
    return 1;
  } catch (const CompileError& e) {
    err("c6502cpp fuzz: " + e.python_type() + ": " + e.what() + "\n");
    return 1;
  }
  err(kUsage);
  return 2;
}

}  // namespace c6502::fuzz
