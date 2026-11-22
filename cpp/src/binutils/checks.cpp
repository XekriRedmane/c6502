#include "binutils/checks.h"

#include <algorithm>
#include <cstdio>
#include <exception>
#include <filesystem>
#include <map>
#include <set>
#include <string>
#include <string_view>

#include "argparse.h"
#include "binutils/elf.h"
#include "binutils/nm.h"
#include "capture.h"
#include "errors.h"
#include "json.h"
#include "passes/stage_registry_sharing.h"
#include "passes/zp_link_metadata.h"
#include "pyrepr.h"

namespace c6502::binutils {

namespace {

namespace fs = std::filesystem;

// The kinds of record, in the order the summary lists them.
constexpr std::string_view kKinds[] = {"assemble",    "parse_metadata", "elf_read", "parse_script",
                                       "link",        "disassemble",    "nm",       "objdump"};

// Returns the error `e` compared with the recorded one, or "" when the
// two have the same Python type and message.
std::string compare_error(const CompileError& e, const capture::Call& call,
                          const json::Value& output) {
  if (!call.output_is_error) return std::string("C++ raised ") + e.python_type() + ": " + e.what();
  if (e.python_type() != output.get("type")->string || e.what() != output.get("message")->string) {
    return std::string("C++ raised ") + e.python_type() + ": " + e.what() + "\nPython raised " +
           output.get("type")->string + ": " + output.get("message")->string;
  }
  return "";
}

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

  std::size_t files_checked() const { return round_trips_.size(); }
  std::size_t files_differ() const { return bad_files_; }
  // The `parse_metadata` records whose text `format_metadata` writes
  // again from what C++ parsed.
  std::size_t metadata_rewritten() const { return metadata_rewritten_; }

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

  std::string blob(const std::string& hash) {
    return capture::read_file(root_ / "blobs" / hash);
  }

  // Returns what differs when the object or executable `hash` is read
  // and written again in C++, or the empty string. Each file is checked
  // once.
  std::string round_trip(const std::string& hash) {
    if (auto it = round_trips_.find(hash); it != round_trips_.end()) return it->second;
    const std::string data = blob(hash);
    std::string problem;
    try {
      if (elf::write(elf::read(data, hash)) != data) {
        problem = "file " + hash + " read and written again differs";
      }
    } catch (const CompileError& e) {
      problem = "file " + hash + ": " + e.what();
    }
    if (!problem.empty()) ++bad_files_;
    round_trips_.emplace(hash, problem);
    return problem;
  }

  std::string run(const capture::Call& call, std::string_view kind, Count& count) {
    const json::Value args = json::parse(call.args_json);
    const json::Value input = json::parse(call.input);
    const json::Value output = json::parse(call.output);
    if (kind == "elf_read") {
      ++count.ran;
      const std::string data = blob(input.get("data")->string);
      std::string got;
      try {
        got = elf::write(elf::read(data, args.get("name")->string));
      } catch (const CompileError& e) {
        if (std::string problem = compare_error(e, call, output); !problem.empty()) return problem;
        ++count.matched;
        return "";
      }
      if (call.output_is_error) return "C++ read a file Python rejected";
      if (got != blob(output.get("rewritten")->string)) return "the rewrite differs";
      ++count.matched;
      return "";
    }
    if (kind == "parse_metadata") {
      ++count.ran;
      passes::LinkMetadata meta;
      try {
        meta = passes::parse_metadata(input.string);
      } catch (const CompileError& e) {
        if (std::string problem = compare_error(e, call, output); !problem.empty()) return problem;
        ++count.matched;
        return "";
      }
      if (call.output_is_error) return "C++ read a block Python rejected";
      const std::string got = passes::stage_sharing::link_metadata_json(meta);
      if (got != json::dump(*output.get("meta"))) {
        return "the metadata differs\nC++:    " + got + "\nPython: " + json::dump(*output.get("meta"));
      }
      const std::vector<std::string> lines = passes::format_metadata(meta);
      const json::Value& formatted = *output.get("formatted");
      bool same = lines.size() == formatted.items.size();
      for (std::size_t i = 0; same && i < lines.size(); ++i) same = lines[i] == formatted.items[i].string;
      if (!same) return "format_metadata's lines differ";
      // The assembler and the linker parse the block alone, which
      // `format_metadata` wrote, so it writes the same text again.
      std::string text;
      for (const auto& line : lines) text += line + "\n";
      if (text == input.string) ++metadata_rewritten_;
      ++count.matched;
      return "";
    }
    if (kind == "nm") {
      ++count.ran;
      std::map<std::string, std::string> files;
      for (const auto& pair : input.get("files")->items) {
        files.emplace(pair.items.at(0).string, pair.items.at(1).string);
      }
      std::vector<std::string> argv{"nm.py"};
      for (const auto& a : args.get("argv")->items) argv.push_back(a.string);
      const nm::ReadFile read_file = [&](const std::string& path) {
        const auto it = files.find(path);
        if (it == files.end()) {
          throw nm::FileError{"[Errno 2] No such file or directory: " +
                              pyrepr::string(argparse::path_str(path))};
        }
        return blob(it->second);
      };
      std::string out;
      std::string err;
      const int status = nm::main(argv, read_file,
                                  static_cast<int>(args.get("columns")->integer), out, err);
      if (out != output.get("stdout")->string) {
        return "stdout differs\nC++:\n" + out + "Python:\n" + output.get("stdout")->string;
      }
      if (err != output.get("stderr")->string) {
        return "stderr differs\nC++:\n" + err + "Python:\n" + output.get("stderr")->string;
      }
      if (status != output.get("status")->integer) {
        return "the exit status differs: C++ " + std::to_string(status);
      }
      ++count.matched;
      return "";
    }
    // The files each kind holds that c6502 wrote, which must make the
    // trip through the C++ reader and writer.
    std::vector<std::string> files;
    if (kind == "assemble" && !call.output_is_error) {
      files.push_back(output.get("object")->string);
    } else if (kind == "link") {
      for (const auto& pair : input.get("objects")->items) files.push_back(pair.items.at(1).string);
      if (!call.output_is_error) files.push_back(output.get("executable")->string);
    } else if (kind == "disassemble") {
      files.push_back(input.get("elf")->string);
    }
    for (const auto& hash : files) {
      if (std::string problem = round_trip(hash); !problem.empty()) return problem;
    }
    return "";
  }

  fs::path root_;
  bool verbose_;
  std::map<std::string, std::string> round_trips_;
  std::size_t bad_files_ = 0;
  std::size_t metadata_rewritten_ = 0;
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
  std::size_t files = 0;
  std::size_t bad_files = 0;
  std::size_t metadata_rewritten = 0;
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
    files += checker.files_checked();
    bad_files += checker.files_differ();
    metadata_rewritten += checker.metadata_rewritten();
  }
  std::size_t differ = 0;
  for (std::string_view kind : kKinds) {
    const auto it = counts.find(kind);
    if (it == counts.end()) continue;
    const Count& c = it->second;
    std::printf("%s: %zu records, %zu made the trip, %zu run in C++, %zu match, %zu differ\n",
                std::string(kind).c_str(), c.records, c.trips, c.ran, c.matched, c.differ);
    differ += c.differ;
    if (kind == "parse_metadata") {
      std::printf("parse_metadata: %zu texts written again by format_metadata as they were\n",
                  metadata_rewritten);
    }
  }
  std::printf("objects and executables read and written again: %zu, %zu differ\n", files,
              bad_files);
  return differ == 0 ? 0 : 1;
}

}  // namespace c6502::binutils
