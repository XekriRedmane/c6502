#include "fuzz/runner.h"

#include <algorithm>
#include <cctype>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <utility>
#include <vector>

#include "diagnostics.h"
#include "errors.h"
#include "passes/ir_check.h"
#include "sim/assembler.h"
#include "sim/cpu.h"
#include "sim/runtime.h"
#include "sim/source_sim.h"
#include "tac_sim_modes.h"

namespace c6502::fuzz {

namespace {

constexpr std::size_t kMessageChars = 120;

constexpr const char* kBackendLimits[] = {"local_bytes", "frame offset"};

std::string first_chars(const std::string& s, std::size_t n) {
  std::size_t i = 0;
  for (std::size_t chars = 0; i < s.size() && chars < n; ++chars) {
    const auto c = static_cast<unsigned char>(s[i]);
    i += c < 0x80 ? 1 : c < 0xE0 ? 2 : c < 0xF0 ? 3 : 4;
  }
  return s.substr(0, std::min(i, s.size()));
}

std::string strip(const std::string& s) {
  const char* ws = " \t\n\r\v\f";
  const std::size_t b = s.find_first_not_of(ws);
  if (b == std::string::npos) return "";
  return s.substr(b, s.find_last_not_of(ws) - b + 1);
}

std::string unqualified_class_name(const std::string& python_type) {
  const std::size_t dot = python_type.rfind('.');
  return dot == std::string::npos ? python_type : python_type.substr(dot + 1);
}

Result error_result(const std::string& python_type, const std::string& message) {
  return "ERR " + unqualified_class_name(python_type) + ": " + first_chars(message, kMessageChars);
}

Result internal_error_result(const InternalError& e) {
  std::string m = strip(std::string("internal error: ") + e.what());
  m.erase(0, std::string("internal error: ").size());
  return error_result("AssertionError", m);
}

Result backend_failure_result(const std::string& stderr_line) {
  return error_result("RuntimeError", "c6502cpp backend failed: " + strip(stderr_line));
}

passes::FrontEndOptions front_options(const std::string& include_dir) {
  passes::FrontEndOptions o;
  o.include_dir = include_dir;
  return o;
}

}  // namespace

const char* mode_name(Mode m) {
  switch (m) {
    case Mode::kTac: return "tac";
    case Mode::kUnopt: return "unopt";
    case Mode::kOpt: return "opt";
  }
  return "";
}

std::int64_t s16(std::int64_t v) {
  v &= 0xFFFF;
  return v & 0x8000 ? v - 0x10000 : v;
}

Runner::Runner(std::string include_dir) : include_dir_(std::move(include_dir)) {}

Result Runner::run_tac(const std::string& src) const {
  const passes::ir_check::Enabled checks(true);
  std::string err;
  std::vector<diagnostics::CompileWarning> warnings;
  tac_sim::Outcome o;
  try {
    tac_sim_modes::Translated t;
    {
      diagnostics::Collecting block(warnings);
      t = tac_sim_modes::translate(src, front_options(include_dir_), err);
    }
    std::cerr << err;
    err.clear();
    try {
      o = tac_sim::run_main(t.tac, t.symbols, &t.types);
    } catch (const CompileError& e) {
      return backend_failure_result(std::string("error: ") + e.what());
    }
  } catch (const CompileError& e) {
    std::cerr << err;
    return error_result(e.python_type(), e.what());
  } catch (const InternalError& e) {
    std::cerr << err;
    return internal_error_result(e);
  } catch (const std::exception& e) {
    std::cerr << err;
    return backend_failure_result(std::string("c6502cpp: ") + e.what());
  }
  if (o.error_type) return error_result(*o.error_type, o.error_message);
  if (!o.value) return std::string("ERR non-int result None");
  return s16(static_cast<std::int64_t>(*o.value & 0xFFFF));
}

Result Runner::run_asm(const std::string& src, bool optimize) const {
  sim::SourceOptions options;
  options.include_dir = include_dir_;
  options.optimize = optimize;
  options.check_ir = true;
  std::string err;
  sim::SimResult result;
  try {
    sim::Simulation s = sim::build_source_sim(src, options, err);
    std::cerr << err;
    err.clear();
    result = s.run(kMaxCycles);
  } catch (const CompileError& e) {
    std::cerr << err;
    return error_result(e.python_type(), e.what());
  } catch (const InternalError& e) {
    std::cerr << err;
    return internal_error_result(e);
  } catch (const sim::AssemblerError& e) {
    return error_result("AssemblerError", e.what());
  } catch (const sim::HookError& e) {
    return error_result(e.python_type(), e.what());
  } catch (const sim::CpuError&) {
    return error_result("IndexError", "bytearray index out of range");
  } catch (const std::invalid_argument& e) {
    return error_result("ValueError", e.what());
  } catch (const std::exception& e) {
    std::cerr << err;
    return backend_failure_result(std::string("c6502cpp: ") + e.what());
  }
  if (result.timed_out) return std::string("TIMEOUT");
  return result.return_int_signed();
}

Results Runner::run_all(const std::string& src) const {
  Results r;
  r[Mode::kTac] = run_tac(src);
  r[Mode::kUnopt] = run_asm(src, false);
  r[Mode::kOpt] = run_asm(src, true);
  return r;
}

bool is_error(const Result& r) { return std::holds_alternative<std::string>(r); }

bool is_backend_limit(const Result& r) {
  const std::string* text = std::get_if<std::string>(&r);
  if (text == nullptr) return false;
  for (const char* limit : kBackendLimits) {
    if (text->find(limit) != std::string::npos) return true;
  }
  return false;
}

std::vector<Mode> disagreements(const Results& res, std::optional<std::int64_t> expected) {
  std::vector<Mode> out;
  if (expected) {
    for (Mode m : kModes) {
      if (res[m] != Result(*expected)) out.push_back(m);
    }
    return out;
  }
  const Result& ref = res[Mode::kTac];
  if (is_error(ref)) return {std::begin(kModes), std::end(kModes)};
  for (Mode m : {Mode::kUnopt, Mode::kOpt}) {
    if (res[m] != ref) out.push_back(m);
  }
  return out;
}

std::string format_result(const Results& res) {
  std::string out;
  for (Mode m : kModes) {
    if (!out.empty()) out += ' ';
    out += std::string(mode_name(m)) + "=";
    const Result& r = res[m];
    out += is_error(r) ? std::get<std::string>(r) : std::to_string(std::get<std::int64_t>(r));
  }
  return out;
}

std::optional<std::int64_t> expected_of(const std::string& src) {
  auto space = [](char c) { return c == ' ' || (c >= '\t' && c <= '\r'); };
  for (std::size_t at = src.find("//"); at != std::string::npos; at = src.find("//", at + 1)) {
    std::size_t i = at + 2;
    while (i < src.size() && space(src[i])) ++i;
    if (src.compare(i, 7, "expect:") != 0) continue;
    i += 7;
    while (i < src.size() && space(src[i])) ++i;
    const bool negative = i < src.size() && src[i] == '-';
    if (negative) ++i;
    if (i >= src.size() || !std::isdigit(static_cast<unsigned char>(src[i]))) continue;
    std::int64_t v = 0;
    for (; i < src.size() && std::isdigit(static_cast<unsigned char>(src[i])); ++i) {
      v = (v * 10 + (src[i] - '0')) & 0xFFFF;
    }
    return s16(negative ? -v : v);
  }
  return std::nullopt;
}

}  // namespace c6502::fuzz
