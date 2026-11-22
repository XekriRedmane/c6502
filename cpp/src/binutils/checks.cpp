#include "binutils/checks.h"

#include <algorithm>
#include <cstdio>
#include <exception>
#include <filesystem>
#include <map>
#include <set>
#include <string>
#include <string_view>

#include "capture.h"
#include "json.h"

namespace c6502::binutils {

namespace {

namespace fs = std::filesystem;

// The kinds of record, in the order the summary lists them.
constexpr std::string_view kKinds[] = {"assemble", "elf_read",    "parse_script", "link",
                                       "disassemble", "nm",       "objdump"};

// The counts of one kind's records.
struct Count {
  std::size_t records = 0;
  // Records whose JSON made the trip to C++ and back.
  std::size_t trips = 0;
  // Records a ported function ran on, and those it gave the recorded
  // result for.
  std::size_t ran = 0;
  std::size_t matched = 0;
  std::size_t differ = 0;
};

class Checker {
 public:
  Checker(fs::path root, bool verbose) : root_(std::move(root)), verbose_(verbose) {}

  void check(const fs::path& path, std::string_view kind, Count& count) {
    const std::string text = capture::read_file(path);
    ++count.records;
    std::string problem;
    try {
      const capture::Call call = capture::parse(text);
      problem = trip(call);
      if (problem.empty()) {
        ++count.trips;
        problem = run(call, kind, count);
      }
    } catch (const std::exception& e) {
      problem = std::string("exception: ") + e.what();
    }
    if (!problem.empty()) {
      ++count.differ;
      const std::string shown = verbose_ ? problem : problem.substr(0, problem.find('\n'));
      std::printf("DIFFER %s: %s\n", path.string().c_str(), shown.c_str());
    }
  }

 private:
  // Returns what differs when the record's JSON is read and written
  // again, or the empty string.
  static std::string trip(const capture::Call& call) {
    const auto again = [](const std::string& text, bool newline) {
      const std::string body = newline ? text.substr(0, text.size() - 1) : text;
      return json::dump(json::parse(body)) + (newline ? "\n" : "");
    };
    if (again(call.args_json, false) != call.args_json) return "the args line";
    if (!call.input_is_json) return "the input isn't JSON";
    if (again(call.input, true) != call.input) return "the input";
    if (!call.output_is_json && !call.output_is_error) return "the output isn't JSON";
    if (again(call.output, true) != call.output) return "the output";
    return "";
  }

  // Returns what differs when a ported function runs on the record. No
  // function is ported yet.
  std::string run(const capture::Call&, std::string_view, Count&) { return ""; }

  fs::path root_;
  bool verbose_;
};

}  // namespace

int check_binutils(const std::vector<std::string>& args) {
  bool verbose = false;
  std::vector<fs::path> roots;
  for (const auto& a : args) {
    if (a == "--verbose") {
      verbose = true;
    } else {
      roots.emplace_back(a);
    }
  }
  std::map<std::string, Count, std::less<>> counts;
  for (const auto& root : roots) {
    Checker checker(root, verbose);
    for (std::string_view kind : kKinds) {
      const fs::path dir = root / std::string(kind);
      if (!fs::is_directory(dir)) continue;
      std::vector<fs::path> paths;
      for (const auto& entry : fs::directory_iterator(dir)) {
        if (entry.path().extension() == ".txt") paths.push_back(entry.path());
      }
      std::sort(paths.begin(), paths.end());
      Count& count = counts[std::string(kind)];
      for (const auto& path : paths) checker.check(path, kind, count);
    }
  }
  std::size_t differ = 0;
  for (std::string_view kind : kKinds) {
    const auto it = counts.find(kind);
    if (it == counts.end()) continue;
    const Count& c = it->second;
    std::printf("%s: %zu records, %zu made the trip, %zu run in C++, %zu match, %zu differ\n",
                std::string(kind).c_str(), c.records, c.trips, c.ran, c.matched, c.differ);
    differ += c.differ;
  }
  return differ == 0 ? 0 : 1;
}

}  // namespace c6502::binutils
