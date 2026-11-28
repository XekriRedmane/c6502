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
#include "binutils/assembler.h"
#include "binutils/disasm.h"
#include "binutils/elf.h"
#include "binutils/nm.h"
#include "binutils/objdump.h"
#include "capture.h"
#include "errors.h"
#include "json.h"
#include "binutils/ld.h"
#include "passes/link_checks.h"
#include "passes/link_script.h"
#include "passes/linker.h"
#include "passes/linker_checks.h"
#include "passes/linker_objects_checks.h"
#include "passes/runtime_library.h"
#include "passes/stage_registry_sharing.h"
#include "passes/zp_link_metadata.h"
#include "pyrepr.h"

namespace c6502::binutils {

namespace {

namespace fs = std::filesystem;

constexpr std::string_view kKinds[] = {
    "assemble",       "parse_metadata", "elf_read",  "parse_script",    "link",
    "disassemble",    "nm",             "objdump",   "runtime_library", "lay_out",
    "default_script", "fall_through",   "tail_jump", "startup_lines",
    "link_merge", "link_carry_conflict", "link_arrivals", "link_validate_eligibility",
    "link_synthesize", "link_rename_locals", "link_rename_meta", "link_merge_zp_objects",
    "link_place_zp_objects", "link_choose_layout", "link_runtime_bytes", "link_shares",
    "merge_shares", "check_linked_shares", "link_bind_extern_params", "link_drop_unused_locals",
    "link_init_lines", "link_data_lines", "link_regs",
    "global_definitions", "relax", "executable", "linker_runtime_library", "object_items",
    "item_symbol", "without_init", "unit", "apply_renames", "pointer_names", "reassemble",
    "output_symbols", "merge_identical", "contents"};

std::string compare_error(const CompileError& e, const capture::Call& call,
                          const json::Value& output) {
  if (!call.output_is_error) return std::string("C++ raised ") + e.python_type() + ": " + e.what();
  if (e.python_type() != output.get("type")->string || e.what() != output.get("message")->string) {
    return std::string("C++ raised ") + e.python_type() + ": " + e.what() + "\nPython raised " +
           output.get("type")->string + ": " + output.get("message")->string;
  }
  return "";
}

struct Count {
  std::size_t records = 0;
  std::size_t trips = 0;
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
  std::size_t metadata_rewritten() const { return metadata_rewritten_; }

 private:
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
    if (passes::link_checks::handles(kind)) {
      ++count.ran;
      std::string problem =
          passes::link_checks::check(kind, args, input, output, call.output_is_error);
      if (problem.empty()) ++count.matched;
      return problem;
    }
    if (passes::linker_checks::handles(kind)) {
      ++count.ran;
      std::string problem = passes::linker_checks::check(
          kind, args, input, output, call.output_is_error,
          [this](const std::string& hash) { return blob(hash); });
      if (problem.empty()) ++count.matched;
      return problem;
    }
    if (passes::linker_objects_checks::handles(kind)) {
      ++count.ran;
      std::string problem = passes::linker_objects_checks::check(
          kind, args, input, output, call.output_is_error,
          [this](const std::string& hash) { return blob(hash); });
      if (problem.empty()) ++count.matched;
      return problem;
    }
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
    if (kind == "assemble") {
      ++count.ran;
      if (!call.output_is_error) {
        if (std::string problem = round_trip(output.get("object")->string); !problem.empty()) {
          return problem;
        }
      }
      const json::Value* links = args.get("link_symbols");
      assembler::LinkSymbols symbols;
      if (links->kind == json::Value::Kind::kArray) {
        for (const auto& pair : links->items) {
          const json::Value& address = pair.items.at(1);
          symbols.emplace_back(pair.items.at(0).string,
                               address.kind == json::Value::Kind::kNull
                                   ? std::nullopt
                                   : std::optional<asdl::Int>(address.integer));
        }
      }
      std::string got;
      try {
        got = elf::write(assembler::assemble(
            input.string, args.get("filename")->string,
            links->kind == json::Value::Kind::kArray ? &symbols : nullptr,
            args.get("keep_source")->boolean));
      } catch (const CompileError& e) {
        if (std::string problem = compare_error(e, call, output); !problem.empty()) return problem;
        ++count.matched;
        return "";
      }
      if (call.output_is_error) return "C++ assembled a text Python rejected";
      if (got != blob(output.get("object")->string)) return "the object differs";
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
      std::string text;
      for (const auto& line : lines) text += line + "\n";
      if (text == input.string) ++metadata_rewritten_;
      ++count.matched;
      return "";
    }
    if (kind == "disassemble") {
      if (std::string problem = round_trip(input.get("elf")->string); !problem.empty()) {
        return problem;
      }
      ++count.ran;
      const elf::ElfFile file = elf::read(blob(input.get("elf")->string), "file");
      std::set<std::string> only;
      const json::Value* listed = args.get("only");
      if (listed->kind == json::Value::Kind::kArray) {
        for (const auto& name : listed->items) only.insert(name.string);
      }
      const disasm::Options options{args.get("raw")->boolean, args.get("addresses")->boolean};
      const std::vector<std::string> got =
          disasm::disassemble(file, args.get("all_sections")->boolean,
                              listed->kind == json::Value::Kind::kArray ? &only : nullptr, options);
      std::vector<std::string> want;
      for (const auto& l : output.items) want.push_back(l.string);
      if (got != want) {
        std::size_t k = 0;
        while (k < got.size() && k < want.size() && got[k] == want[k]) ++k;
        return "the listing differs at line " + std::to_string(k + 1) + "\nC++:    " +
               (k < got.size() ? got[k] : "(end)") + "\nPython: " +
               (k < want.size() ? want[k] : "(end)");
      }
      ++count.matched;
      return "";
    }
    if (kind == "nm" || kind == "objdump") {
      ++count.ran;
      std::map<std::string, std::string> files;
      for (const auto& pair : input.get("files")->items) {
        files.emplace(pair.items.at(0).string, pair.items.at(1).string);
      }
      std::vector<std::string> argv{std::string(kind) + ".py"};
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
      const auto tool = kind == "nm" ? nm::main : objdump::main;
      const int status =
          tool(argv, read_file, static_cast<int>(args.get("columns")->integer), out, err);
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
    if (kind == "runtime_library") {
      ++count.ran;
      std::vector<std::pair<std::string, elf::ElfFile>> objects;
      for (const auto& pair : input.get("objects")->items) {
        const std::string& hash = pair.items.at(1).string;
        if (std::string problem = round_trip(hash); !problem.empty()) return problem;
        objects.emplace_back(pair.items.at(0).string,
                             elf::read(blob(hash), pair.items.at(0).string));
      }
      passes::NameSet defined;
      for (const auto& name : args.get("defined")->items) defined.insert(name.string);
      const std::vector<std::string> names = passes::needed(objects, defined);
      std::vector<std::string> expected;
      for (const auto& name : output.get("names")->items) expected.push_back(name.string);
      if (names != expected) {
        std::string got;
        for (const auto& n : names) got += " " + n;
        return "the helpers differ: C++ has" + got;
      }
      const json::Value* text = output.get("text");
      if (text->kind == json::Value::Kind::kString) {
        const std::string got = passes::library_text(names);
        if (got != text->string) {
          return "the text differs\nC++:\n" + got + "Python:\n" + text->string;
        }
      }
      ++count.matched;
      return "";
    }
    std::vector<std::string> files;
    if (kind == "link") {
      for (const auto& pair : input.get("objects")->items) files.push_back(pair.items.at(1).string);
      if (!call.output_is_error) files.push_back(output.get("executable")->string);
    }
    for (const auto& hash : files) {
      if (std::string problem = round_trip(hash); !problem.empty()) return problem;
    }
    if (kind == "link") return run_link(call, args, input, output, count);
    return "";
  }

  std::string run_link(const capture::Call& call, const json::Value& args,
                       const json::Value& input, const json::Value& output, Count& count) {
    ++count.ran;
    ld::Objects objects;
    for (const auto& pair : input.get("objects")->items) {
      const std::string& name = pair.items.at(0).string;
      objects.emplace_back(name, elf::read(blob(pair.items.at(1).string), name));
    }
    std::optional<passes::link_script::Script> script;
    if (const json::Value* text = input.get("script"); text->kind == json::Value::Kind::kString) {
      script = passes::link_script::parse_script(text->string);
    }
    ld::Names defsyms;
    for (const auto& pair : args.get("defsyms")->items) {
      defsyms.insert_or_assign(pair.items.at(0).string,
                               static_cast<std::int64_t>(pair.items.at(1).integer));
    }
    std::string got;
    try {
      got = elf::write(passes::linker::link_objects(
          std::move(objects), script, static_cast<std::int64_t>(args.get("origin")->integer),
          defsyms));
    } catch (const CompileError& e) {
      std::string problem = compare_error(e, call, output);
      if (problem.empty()) ++count.matched;
      return problem;
    }
    if (call.output_is_error) {
      return "C++ linked, and Python raised " + output.get("type")->string + ": " +
             output.get("message")->string;
    }
    if (got != blob(output.get("executable")->string)) return "the executable differs";
    ++count.matched;
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
