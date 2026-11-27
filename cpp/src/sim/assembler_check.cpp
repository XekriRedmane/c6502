// `c6502cpp check-sim-assembler`, the comparison of `assemble` with
// sim/assembler.py on the records of C:/w/g4asm/capture_sim_asm.py,
// which assembles the chapter corpus and the examples in both
// pipelines as sim/harness.py's `build_sim` does, and the runtime
// helpers for each layout as `sim.runtime.build_runtime` does. It is
// temporary, and group R of step 9 removes it with `cpu-run`, when
// sim/assembler.py goes.
#include <cstdio>
#include <fstream>
#include <map>
#include <string>
#include <string_view>
#include <vector>

#include "asdl/runtime.h"
#include "asm_ast.h"
#include "errors.h"
#include "json.h"
#include "sim/assembler.h"
#include "sim/cpu_run.h"

namespace c6502::sim {

namespace {

std::int64_t to_i64(asdl::Int v) { return static_cast<std::int64_t>(static_cast<long long>(v)); }

const json::Value& member(const json::Value& v, std::string_view key) {
  const json::Value* m = v.get(key);
  if (m == nullptr) {
    throw std::runtime_error("record has no " + std::string(key));
  }
  return *m;
}

int hex_digit(char c) {
  return c <= '9' ? c - '0' : (c | 0x20) - 'a' + 10;
}

// Python's image, from the record's nonzero runs.
std::vector<std::uint8_t> recorded_image(const json::Value& image) {
  std::vector<std::uint8_t> out(static_cast<std::size_t>(to_i64(member(image, "length").integer)),
                                0);
  for (const json::Value& run : member(image, "runs").items) {
    std::size_t at = static_cast<std::size_t>(to_i64(run.items.at(0).integer));
    const std::string& hex = run.items.at(1).string;
    for (std::size_t i = 0; i + 1 < hex.size(); i += 2, ++at) {
      out.at(at) = static_cast<std::uint8_t>(hex_digit(hex[i]) * 16 + hex_digit(hex[i + 1]));
    }
  }
  return out;
}

struct Counts {
  std::size_t records = 0;
  std::size_t compile_errors = 0;
  std::size_t images_equal = 0;
  std::size_t symbols_equal = 0;
  std::size_t errors_equal = 0;
  std::size_t past_64k = 0;
  std::size_t differences = 0;
};

// Returns the first difference between the C++ result and the record's
// `result`, or an empty string.
std::string compare(const json::Value& result, const asm_ast::Program& program,
                    std::int64_t origin, const std::map<std::string, std::int64_t>& extra,
                    Counts& counts) {
  std::string error_class;
  std::string error_message;
  AssembledProgram got;
  try {
    got = assemble(program, origin, extra);
  } catch (const AssemblerError& e) {
    error_class = "AssemblerError";
    error_message = e.what();
  } catch (const InternalError& e) {
    // "TypeError: ..." and the like, where Python raises that class.
    const std::string what = e.what();
    const std::size_t colon = what.find(": ");
    error_class = colon == std::string::npos ? "InternalError" : what.substr(0, colon);
    error_message = colon == std::string::npos ? what : what.substr(colon + 2);
  }
  if (const json::Value* error = result.get("error")) {
    const std::string& message = member(result, "message").string;
    if (error_class.empty()) {
      return "Python raised " + error->string + ": " + message + "; C++ assembled it";
    }
    if (error_class != error->string || error_message != message) {
      return "Python raised " + error->string + ": " + message + "; C++ raised " + error_class +
             ": " + error_message;
    }
    ++counts.errors_equal;
    return "";
  }
  const json::Value& image = member(result, "image");
  // Python's image grows past 64K for a program whose bytes run past
  // $FFFF, which `assemble` rejects.
  if (error_class == "AssemblerError" && error_message.ends_with(", past $FFFF") &&
      to_i64(member(image, "length").integer) > 0x10000) {
    ++counts.past_64k;
    return "";
  }
  if (!error_class.empty()) {
    return "C++ raised " + error_class + ": " + error_message + "; Python assembled it";
  }
  std::string diff;
  const std::string_view bytes(reinterpret_cast<const char*>(got.image.data()), got.image.size());
  if (to_i64(member(image, "length").integer) == static_cast<std::int64_t>(got.image.size()) &&
      cpu_run::crc32(bytes) == static_cast<std::uint32_t>(to_i64(member(image, "crc").integer))) {
    ++counts.images_equal;
  } else {
    const std::vector<std::uint8_t> want = recorded_image(image);
    std::size_t i = 0;
    while (i < want.size() && i < got.image.size() && want[i] == got.image[i]) ++i;
    char buf[160];
    if (i < want.size() && i < got.image.size()) {
      std::snprintf(buf, sizeof buf, "image differs at $%04zX: Python $%02X, C++ $%02X", i,
                    want[i], got.image[i]);
    } else {
      std::snprintf(buf, sizeof buf, "image length: Python %zu, C++ %zu", want.size(),
                    got.image.size());
    }
    diff = buf;
  }
  std::map<std::string, std::int64_t> want_symbols;
  for (const json::Value& pair : member(result, "symbols").items) {
    want_symbols[pair.items.at(0).string] = to_i64(pair.items.at(1).integer);
  }
  if (want_symbols == got.symbols) {
    ++counts.symbols_equal;
  } else if (diff.empty()) {
    auto w = want_symbols.begin();
    auto g = got.symbols.begin();
    while (w != want_symbols.end() && g != got.symbols.end() && *w == *g) {
      ++w;
      ++g;
    }
    if (w == want_symbols.end() || (g != got.symbols.end() && g->first < w->first)) {
      diff = "symbol " + g->first + " = " + std::to_string(g->second) + " only in C++";
    } else if (g == got.symbols.end() || w->first < g->first) {
      diff = "symbol " + w->first + " = " + std::to_string(w->second) + " only in Python";
    } else {
      diff = "symbol " + w->first + ": Python " + std::to_string(w->second) + ", C++ " +
             std::to_string(g->second);
    }
  }
  if (diff.empty() && (to_i64(member(result, "code_end").integer) != got.code_end ||
                       to_i64(member(result, "origin").integer) != got.origin)) {
    diff = "code_end: Python " + std::to_string(to_i64(member(result, "code_end").integer)) +
           ", C++ " + std::to_string(got.code_end);
  }
  return diff;
}

}  // namespace

int check_sim_assembler(const std::vector<std::string>& args) {
  Counts counts;
  std::map<std::string, bool> programs;
  for (const std::string& path : args) {
    std::ifstream in(path, std::ios::binary);
    if (!in) {
      std::fprintf(stderr, "c6502cpp: can't read %s\n", path.c_str());
      return 2;
    }
    std::string line;
    while (std::getline(in, line)) {
      if (line.empty()) continue;
      const json::Value rec = json::parse(line);
      const std::string& kind = member(rec, "kind").string;
      const std::string where = member(rec, "file").string +
                                (member(rec, "optimize").boolean ? " -O" : "") +
                                (kind == "helpers" ? " (helpers)" : "");
      programs[member(rec, "file").string] = true;
      if (kind == "compile_error") {
        ++counts.compile_errors;
        continue;
      }
      ++counts.records;
      std::map<std::string, std::int64_t> extra;
      for (const json::Value& pair : member(rec, "extra").items) {
        extra[pair.items.at(0).string] = to_i64(pair.items.at(1).integer);
      }
      std::string diff;
      try {
        const auto program = asdl::load<asm_ast::Program>(member(rec, "dump").string);
        diff = compare(member(rec, "result"), program, to_i64(member(rec, "origin").integer),
                       extra, counts);
      } catch (const std::exception& e) {
        diff = std::string("exception: ") + e.what();
      }
      if (!diff.empty()) {
        ++counts.differences;
        std::printf("DIFFERS %s: %s\n", where.c_str(), diff.c_str());
      }
    }
  }
  std::printf(
      "sim assembler: %zu programs, %zu records (%zu compile errors skipped), %zu images equal, "
      "%zu symbol tables equal, %zu errors equal, %zu past $FFFF rejected where Python's image "
      "grew, %zu differ\n",
      programs.size(), counts.records, counts.compile_errors, counts.images_equal,
      counts.symbols_equal, counts.errors_equal, counts.past_64k, counts.differences);
  return counts.differences == 0 ? 0 : 1;
}

}  // namespace c6502::sim
