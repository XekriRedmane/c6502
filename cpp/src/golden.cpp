#include "golden.h"

#include <algorithm>
#include <cerrno>
#include <chrono>
#include <cstdio>
#include <cstring>
#include <exception>
#include <filesystem>
#include <map>
#include <optional>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#else
#include <unistd.h>
#endif

#include "capture.h"
#include "compile_command.h"
#include "diagnostics.h"
#include "errors.h"
#include "json.h"
#include "passes/front_end.h"
#include "passes/ir_check.h"
#include "pcpp_text.h"
#include "processes.h"
#include "pyrepr.h"
#include "sha256.h"
#include "stage_checks.h"
#include "tac_sim_modes.h"

namespace c6502::golden {

namespace {

namespace fs = std::filesystem;

// The directories at the root whose files a command names by a path
// relative to the root (golden.h).
constexpr std::string_view kRootDirs[] = {"examples/", "tests/", "golden/"};

constexpr std::string_view kFormat = "c6502-golden 1";

using compile_command::path_text;
using compile_command::read_text;
using compile_command::utf8_path;
using compile_command::write_bytes;

// --- The manifests ---

struct Job {
  std::string label;
  // The job's lines from `job` to `end`.
  std::vector<std::string> lines;
};

struct Manifest {
  std::string name;
  // The lines before the first job.
  std::vector<std::string> header;
  std::map<std::string, std::vector<std::string>, std::less<>> defines;
  std::set<std::string, std::less<>> texts;
  std::vector<Job> jobs;
};

std::vector<std::string> split_lines(const std::string& text) {
  std::vector<std::string> lines;
  std::size_t pos = 0;
  while (pos < text.size()) {
    std::size_t end = text.find('\n', pos);
    if (end == std::string::npos) end = text.size();
    lines.push_back(text.substr(pos, end - pos));
    pos = end + 1;
  }
  return lines;
}

Manifest read_manifest(const fs::path& path) {
  Manifest m;
  m.name = path_text(path.stem());
  const std::vector<std::string> lines = split_lines(capture::read_file(path));
  if (lines.empty() || lines[0] != kFormat) {
    throw std::runtime_error(path_text(path) + " isn't a manifest of golden files");
  }
  std::size_t i = 0;
  for (; i < lines.size() && lines[i].rfind("job ", 0) != 0; ++i) {
    const std::string& l = lines[i];
    m.header.push_back(l);
    if (l.rfind("define ", 0) == 0) {
      const std::size_t space = l.find(' ', 7);
      std::vector<std::string> list;
      for (const json::Value& v : json::parse(l.substr(space + 1)).items) list.push_back(v.string);
      m.defines[l.substr(7, space - 7)] = std::move(list);
    } else if (l.rfind("text ", 0) == 0) {
      m.texts.insert(l.substr(5));
    }
  }
  while (i < lines.size()) {
    Job job;
    job.label = lines[i].substr(4);
    for (; i < lines.size(); ++i) {
      job.lines.push_back(lines[i]);
      if (lines[i] == "end") break;
    }
    if (i == lines.size()) throw std::runtime_error("a job without its end: " + job.label);
    ++i;
    m.jobs.push_back(std::move(job));
  }
  return m;
}

// The first word of a manifest's line and the rest of it.
std::pair<std::string, std::string> split_word(const std::string& line) {
  const std::size_t space = line.find(' ');
  if (space == std::string::npos) return {line, ""};
  return {line.substr(0, space), line.substr(space + 1)};
}

// The spellings of the root a message can hold, with the separator after
// them, longest first.
struct Root {
  fs::path path;
  std::string posix;
  std::vector<std::string> forms;
};

Root make_root(const fs::path& p) {
  Root r;
  r.path = fs::absolute(p).lexically_normal();
  std::string posix = path_text(r.path);
  while (posix.size() > 1 && posix.back() == '/') posix.pop_back();
  r.posix = posix;
  r.forms.push_back(posix + "/");
#ifdef _WIN32
  std::string native = posix;
  std::replace(native.begin(), native.end(), '/', '\\');
  r.forms.push_back(native + "\\");
#endif
  std::sort(r.forms.begin(), r.forms.end(),
            [](const std::string& a, const std::string& b) { return a.size() > b.size(); });
  return r;
}

std::string normalize(std::string text, const Root& root) {
  for (const std::string& form : root.forms) {
    std::size_t pos = 0;
    while ((pos = text.find(form, pos)) != std::string::npos) text.erase(pos, form.size());
  }
  return text;
}

// The directory of c6502's own headers, with the platform's
// separators.
std::string compile_include_dir(const Root& root) {
  return (root.path / "include").make_preferred().string();
}

std::string normalized_json(json::Value v, const Root& root) {
  struct Walk {
    const Root& root;
    void operator()(json::Value& x) const {
      if (x.kind == json::Value::Kind::kString) x.string = normalize(x.string, root);
      for (json::Value& item : x.items) (*this)(item);
    }
  };
  Walk{root}(v);
  return json::dump(v);
}

std::vector<std::string> tacsim_lines(const std::vector<std::string>& argv,
                                      const std::string& include_dir, const Root& root) {
  const std::string source = read_text(argv.at(0));
  passes::FrontEndOptions front;
  front.pcpp_args.assign(argv.begin() + 1, argv.end());
  front.filename = argv[0];
  front.include_dir = include_dir;
  const passes::ir_check::Enabled checks(false);
  // Each of the two checks starts from a translation of its own.
  const auto translate = [&] {
    std::string err;
    std::vector<diagnostics::CompileWarning> warnings;
    diagnostics::Collecting block(warnings);
    return tac_sim_modes::translate(source, front, err);
  };
  const auto line = [&](const std::string& key, const std::string& json_text) {
    return key + " " + normalized_json(json::parse(json_text), root);
  };
  std::vector<std::string> lines;
  std::optional<tac_sim_modes::Translated> t;
  try {
    t = translate();
  } catch (const CompileError& e) {
    return {line("front", stage_checks::error_text(e.python_type(), e.what()))};
  }
  const tac_sim_modes::PlainAndOptimized r = tac_sim_modes::run_plain_and_optimized(std::move(*t));
  lines.push_back(line("result", tac_sim_modes::outcome_json(r.plain)));
  if (r.optimized) lines.push_back(line("optimized", tac_sim_modes::outcome_json(*r.optimized)));
  tac_sim_modes::Translated again = translate();
  lines.push_back(line("found", tac_sim_modes::changing_pass_json(
                                    tac_sim_modes::first_changing_pass(again))));
  return lines;
}

bool names_root_file(std::string_view arg) {
  for (std::string_view d : kRootDirs) {
    if (arg.substr(0, d.size()) == d) return true;
  }
  return false;
}

// The path under golden/ of the text output `name` of the program
// `label` in the manifest `manifest` (golden.h).
std::string text_path(const std::string& manifest, std::string label, const std::string& name) {
  if (label.size() > 2 && label.compare(label.size() - 2, 2, ".c") == 0) {
    label.resize(label.size() - 2);
  }
  if (label.rfind("golden/", 0) == 0) label = label.substr(7);
  const std::size_t dot = name.rfind('.');
  const std::string suffix = dot != std::string::npos && dot > 0 ? name.substr(dot) : "";
  return manifest + "/" + label + suffix;
}

// The result of one job: its lines as the C++ compiler gives them, the
// number of commands it ran, and the first difference from the
// manifest, if any.
struct JobResult {
  std::vector<std::string> lines;
  std::size_t commands = 0;
  std::string difference;
};

std::string clip(const std::string& s, std::size_t n) {
  return s.size() <= n ? s : s.substr(0, n) + "...";
}

// Returns the 1-based number of the first line where `a` and `b`
// differ.
std::size_t first_line_difference(const std::string& a, const std::string& b) {
  std::size_t line = 1;
  for (std::size_t i = 0; i < a.size() && i < b.size(); ++i) {
    if (a[i] != b[i]) return line;
    if (a[i] == '\n') ++line;
  }
  return line;
}

// Takes the steps of `job` in the empty directory `work` and returns
// its lines. With `update`, writes each text output whose bytes
// changed under golden/.
JobResult run_job(const Manifest& m, const Job& job, const Root& root, const fs::path& work,
                  bool update) {
  JobResult r;
  compile_command::Options options;
  options.include_dir = compile_include_dir(root);
  // The terminal is 80 columns wide until a job's `columns` step
  // changes it.
  options.columns = 80;
  const fs::path golden = root.path / "golden";
  fs::current_path(work);
  for (const std::string& line : job.lines) {
    const auto [word, rest] = split_word(line);
    if (word == "status" || word == "stdout" || word == "stderr" || word == "raised" ||
        word == "file" || word == "end" || word == "front" || word == "result" ||
        word == "optimized" || word == "found") {
      continue;
    }
    r.lines.push_back(line);
    if (word == "job") continue;
    if (word == "mkdir") {
      fs::create_directories(work / utf8_path(rest));
    } else if (word == "cd") {
      fs::current_path(work / utf8_path(rest));
    } else if (word == "write") {
      const auto [path, spec] = split_word(rest);
      const auto [how, what] = split_word(spec);
      const std::string data =
          how == "text" ? json::parse(what).string : capture::read_file(golden / "blobs" / what);
      if (write_bytes(path_text(work / utf8_path(path)), data) != 0) {
        throw std::runtime_error("can't write " + path);
      }
    } else if (word == "columns") {
      // The terminal's width, which argparse wraps the usage and help
      // to, as the job's `COLUMNS` gives it.
      options.columns = std::stoi(rest);
    } else if (word == "tacsim") {
      ++r.commands;
      std::vector<std::string> argv;
      for (const json::Value& v : json::parse(rest).items) {
        argv.push_back(names_root_file(v.string) ? root.posix + "/" + v.string : v.string);
      }
      for (std::string& l : tacsim_lines(argv, compile_include_dir(root), root)) {
        r.lines.push_back(std::move(l));
      }
    } else if (word == "run") {
      ++r.commands;
      std::vector<std::string> argv;
      for (const json::Value& v : json::parse(rest).items) {
        if (const auto it = m.defines.find(v.string); it != m.defines.end()) {
          argv.insert(argv.end(), it->second.begin(), it->second.end());
        } else if (names_root_file(v.string)) {
          argv.push_back(root.posix + "/" + v.string);
        } else {
          argv.push_back(v.string);
        }
      }
      const compile_command::Outcome o = compile_command::run(argv, options);
      r.lines.push_back("status " + (o.status ? std::to_string(*o.status) : std::string("none")));
      const std::string out = normalize(o.out(), root);
      const std::string err = normalize(o.err(), root);
      if (!out.empty()) r.lines.push_back("stdout " + json::quote(out));
      if (!err.empty()) r.lines.push_back("stderr " + json::quote(err));
      if (o.raised) {
        // `type(e).__name__`, the name after a module's, so
        // "struct.error" is "error".
        std::string type = o.raised->type;
        if (const std::size_t dot = type.rfind('.'); dot != std::string::npos) {
          type = type.substr(dot + 1);
        }
        r.lines.push_back("raised [" + json::quote(type) + ", " +
                          json::quote(normalize(o.raised->message, root)) + "]");
      }
    } else {
      throw std::runtime_error("a manifest line that isn't a step: " + line);
    }
  }
  // The files the job left, sorted by path.
  std::vector<std::pair<std::string, fs::path>> files;
  for (const auto& entry : fs::recursive_directory_iterator(work)) {
    if (entry.is_regular_file()) {
      files.emplace_back(path_text(entry.path().lexically_relative(work)), entry.path());
    }
  }
  std::sort(files.begin(), files.end());
  std::string text_problem;
  for (const auto& [rel, path] : files) {
    const std::string data = capture::read_file(path);
    const bool program = job.label.rfind("game ", 0) != 0 && job.label.rfind("errors ", 0) != 0 &&
                         job.label.rfind("cli ", 0) != 0;
    if (program && m.texts.count(rel) != 0) {
      const std::string where = text_path(m.name, job.label, rel);
      r.lines.push_back("file " + rel + " text " + where);
      const fs::path kept = golden / utf8_path(where);
      std::string before;
      std::error_code ec;
      const bool exists = fs::is_regular_file(kept, ec);
      if (exists) before = capture::read_file(kept);
      if (!exists || before != data) {
        if (text_problem.empty()) {
          text_problem = !exists ? "golden/" + where + " is missing"
                                 : "the text of " + rel + " differs from golden/" + where +
                                       " from line " +
                                       std::to_string(first_line_difference(before, data));
        }
        if (update) {
          fs::create_directories(kept.parent_path());
          write_bytes(path_text(kept), data);
        }
      }
    } else {
      r.lines.push_back("file " + rel + " sha256 " + sha256_hex(data) + " " +
                        std::to_string(data.size()));
    }
  }
  r.lines.push_back("end");
  // The first line that differs, with the command it follows.
  std::string command;
  const std::size_t n = std::max(r.lines.size(), job.lines.size());
  for (std::size_t i = 0; i < n; ++i) {
    const std::string got = i < r.lines.size() ? r.lines[i] : "(nothing)";
    const std::string want = i < job.lines.size() ? job.lines[i] : "(nothing)";
    if (want.rfind("run ", 0) == 0) command = want.substr(4);
    if (got != want) {
      r.difference = (command.empty() ? std::string() : "after " + clip(command, 200) + ", ") +
                     "the golden file has\n    " + clip(want, 600) + "\n  and C++ gives\n    " +
                     clip(got, 600);
      return r;
    }
  }
  r.difference = text_problem;
  return r;
}

// --- Workers ---

struct Options {
  fs::path root;
  int jobs = 1;
  std::set<std::string, std::less<>> manifests;
  std::string label;
  bool update = false;
  bool verbose = false;
  // For a worker, its share of the jobs (those whose index modulo
  // `shares` is `share`) and the file it writes its results to.
  int share = -1;
  int shares = 1;
  fs::path results;
};

// Returns the manifests under golden/ that `opts` selects, sorted by
// name.
std::vector<Manifest> read_manifests(const Options& opts) {
  std::vector<fs::path> paths;
  for (const auto& entry : fs::directory_iterator(opts.root / "golden")) {
    if (entry.is_regular_file() && entry.path().extension() == ".txt") {
      const std::string name = path_text(entry.path().stem());
      if (opts.manifests.empty() || opts.manifests.count(name) != 0) paths.push_back(entry.path());
    }
  }
  std::sort(paths.begin(), paths.end());
  std::vector<Manifest> out;
  for (const fs::path& p : paths) out.push_back(read_manifest(p));
  return out;
}

// Runs a worker's share of the jobs and writes, for each, a record of
// its index, its count of commands, the difference, and its new lines.
int worker(const Options& opts) {
  const Root root = make_root(opts.root);
  const std::vector<Manifest> manifests = read_manifests(opts);
  const fs::path base = fs::temp_directory_path() /
                        ("c6502-golden-" + std::to_string(
#ifdef _WIN32
                                               GetCurrentProcessId()
#else
                                               getpid()
#endif
                                                   ));
  const fs::path here = fs::current_path();
  std::string out;
  std::size_t index = 0;
  for (const Manifest& m : manifests) {
    for (const Job& job : m.jobs) {
      const std::size_t mine = index++;
      if (static_cast<int>(mine % static_cast<std::size_t>(opts.shares)) != opts.share) continue;
      if (!opts.label.empty() && job.label != opts.label) continue;
      std::error_code ec;
      fs::remove_all(base, ec);
      fs::create_directories(base);
      JobResult r;
      try {
        r = run_job(m, job, root, base, opts.update);
      } catch (const std::exception& e) {
        r.difference = std::string("the check failed: ") + e.what();
      }
      fs::current_path(here);
      fs::remove_all(base, ec);
      std::string block;
      for (const std::string& l : r.lines) block += l + "\n";
      out += "result " + std::to_string(mine) + " " + std::to_string(r.commands) + " " +
             std::to_string(r.difference.size()) + " " + std::to_string(block.size()) + "\n" +
             r.difference + block;
    }
  }
  if (write_bytes(path_text(opts.results), out) != 0) {
    std::fprintf(stderr, "c6502cpp: can't write %s\n", path_text(opts.results).c_str());
    return 2;
  }
  return 0;
}

struct WorkerResult {
  std::size_t commands = 0;
  std::string difference;
  std::string block;
};

std::map<std::size_t, WorkerResult> read_results(const fs::path& path) {
  std::map<std::size_t, WorkerResult> out;
  const std::string data = capture::read_file(path);
  std::size_t pos = 0;
  while (pos < data.size()) {
    const std::size_t end = data.find('\n', pos);
    std::size_t index = 0, commands = 0, dlen = 0, blen = 0;
    std::istringstream head(data.substr(pos, end - pos));
    std::string word;
    if (!(head >> word >> index >> commands >> dlen >> blen) || word != "result") {
      throw std::runtime_error("a worker's results can't be read");
    }
    pos = end + 1;
    WorkerResult r{commands, data.substr(pos, dlen), data.substr(pos + dlen, blen)};
    pos += dlen + blen;
    out[index] = std::move(r);
  }
  return out;
}

// Returns the first directory from the current one upward that holds
// golden/.
fs::path find_root() {
  for (fs::path p = fs::current_path();; p = p.parent_path()) {
    if (fs::is_directory(p / "golden")) return p;
    if (p == p.parent_path()) throw std::runtime_error("no golden/ here or above; give --root");
  }
}

}  // namespace

int check_golden(const std::vector<std::string>& args, const std::string& self) {
  Options opts;
  bool root_given = false;
  for (std::size_t i = 0; i < args.size(); ++i) {
    const std::string& a = args[i];
    const auto next = [&]() -> std::string {
      if (i + 1 >= args.size()) throw std::runtime_error(a + " takes a value");
      return args[++i];
    };
    if (a == "--root") {
      opts.root = utf8_path(next());
      root_given = true;
    } else if (a == "-j") {
      opts.jobs = std::max(1, std::stoi(next()));
    } else if (a == "--manifest") {
      std::string list = next();
      for (std::size_t p = 0; p <= list.size();) {
        const std::size_t comma = std::min(list.find(',', p), list.size());
        if (comma > p) opts.manifests.insert(list.substr(p, comma - p));
        p = comma + 1;
      }
    } else if (a == "--label") {
      opts.label = next();
    } else if (a == "--update") {
      opts.update = true;
    } else if (a == "--verbose") {
      opts.verbose = true;
    } else if (a == "--worker") {
      opts.share = std::stoi(next());
      opts.shares = std::stoi(next());
      opts.results = utf8_path(next());
    } else {
      std::fprintf(stderr, "c6502cpp: check-golden doesn't take %s\n", a.c_str());
      return 2;
    }
  }
  if (!root_given) opts.root = find_root();
  opts.root = fs::absolute(opts.root);
  if (opts.share >= 0) return worker(opts);

  const auto start = std::chrono::steady_clock::now();
  const std::vector<Manifest> manifests = read_manifests(opts);
  const std::string exe = path_text(processes::self_path(self));
  const fs::path results_dir =
      fs::temp_directory_path() / ("c6502-golden-results-" + std::to_string(
#ifdef _WIN32
                                                                 GetCurrentProcessId()
#else
                                                                 getpid()
#endif
                                                                 ));
  fs::create_directories(results_dir);
  std::vector<std::vector<std::string>> commands;
  for (int k = 0; k < opts.jobs; ++k) {
    std::vector<std::string> cmd = {exe, "check-golden", "--root", path_text(opts.root),
                                    "--worker", std::to_string(k), std::to_string(opts.jobs),
                                    path_text(results_dir / (std::to_string(k) + ".txt"))};
    if (!opts.manifests.empty()) {
      std::string list;
      for (const std::string& n : opts.manifests) list += (list.empty() ? "" : ",") + n;
      cmd.push_back("--manifest");
      cmd.push_back(list);
    }
    if (!opts.label.empty()) {
      cmd.push_back("--label");
      cmd.push_back(opts.label);
    }
    if (opts.update) cmd.push_back("--update");
    commands.push_back(std::move(cmd));
  }
  const std::vector<int> statuses = processes::run_all(commands);
  std::map<std::size_t, WorkerResult> results;
  for (int k = 0; k < opts.jobs; ++k) {
    if (statuses[k] != 0) {
      std::fprintf(stderr, "c6502cpp: a worker of check-golden failed\n");
      return 2;
    }
    results.merge(read_results(results_dir / (std::to_string(k) + ".txt")));
  }
  std::error_code ec;
  fs::remove_all(results_dir, ec);

  std::size_t index = 0;
  std::size_t total_jobs = 0;
  std::size_t total_commands = 0;
  std::size_t total_differ = 0;
  for (const Manifest& m : manifests) {
    std::size_t jobs = 0, commands_run = 0, differ = 0;
    std::string text;
    for (const std::string& l : m.header) text += l + "\n";
    std::set<std::string> kept;
    for (const Job& job : m.jobs) {
      const auto it = results.find(index++);
      if (it == results.end()) {
        for (const std::string& l : job.lines) text += l + "\n";
        continue;
      }
      const WorkerResult& r = it->second;
      ++jobs;
      commands_run += r.commands;
      if (!r.difference.empty()) {
        ++differ;
        const std::string shown =
            opts.verbose ? r.difference : r.difference.substr(0, std::min<std::size_t>(r.difference.size(), 2000));
        std::printf("DIFFER %s: %s: %s\n", m.name.c_str(), job.label.c_str(), shown.c_str());
      }
      text += r.block;
      for (const std::string& l : split_lines(r.block)) {
        const auto [word, rest] = split_word(l);
        if (word != "file") continue;
        const std::size_t t = rest.find(" text ");
        if (t != std::string::npos) kept.insert(rest.substr(t + 6));
      }
    }
    std::printf("%s: %zu jobs of %zu commands, %zu differ\n", m.name.c_str(), jobs, commands_run,
                differ);
    total_jobs += jobs;
    total_commands += commands_run;
    total_differ += differ;
    if (opts.update && opts.label.empty() && differ != 0) {
      write_bytes(path_text(opts.root / "golden" / (m.name + ".txt")), text);
      // The texts that no job of the manifest names any longer.
      const fs::path dir = opts.root / "golden" / m.name;
      if (!m.texts.empty() && fs::is_directory(dir)) {
        std::vector<fs::path> stale;
        for (const auto& entry : fs::recursive_directory_iterator(dir)) {
          if (!entry.is_regular_file()) continue;
          const std::string rel = path_text(entry.path().lexically_relative(opts.root / "golden"));
          if (kept.count(rel) == 0) stale.push_back(entry.path());
        }
        for (const fs::path& p : stale) fs::remove(p, ec);
      }
    }
  }
  const double seconds =
      std::chrono::duration<double>(std::chrono::steady_clock::now() - start).count();
  std::printf("%zu jobs of %zu commands checked in %.0f s with %d processes, %zu differ%s\n",
              total_jobs, total_commands, seconds, opts.jobs, total_differ,
              opts.update && total_differ != 0 ? " (golden files updated)" : "");
  return total_differ == 0 ? 0 : 1;
}

}  // namespace c6502::golden
