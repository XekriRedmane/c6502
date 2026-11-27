#pragma once

#include <cstdint>
#include <sstream>
#include <string>
#include <type_traits>
#include <vector>

#include "doctest.h"
#include "sim_case.h"

namespace c6502::suite {

using Bytes = std::vector<std::uint8_t>;

template <typename... Parts>
std::string cat(const Parts&... parts) {
  std::ostringstream out;
  out << std::boolalpha;
  auto put = [&out](const auto& part) {
    if constexpr (std::is_same_v<std::decay_t<decltype(part)>, std::uint8_t>) {
      out << static_cast<int>(part);
    } else {
      out << part;
    }
  };
  (put(parts), ...);
  return out.str();
}

inline std::string replace_all(std::string text, const std::string& from, const std::string& to) {
  std::string out;
  std::size_t at = 0;
  for (std::size_t hit; (hit = text.find(from, at)) != std::string::npos; at = hit + from.size()) {
    out.append(text, at, hit - at);
    out += to;
  }
  out.append(text, at, std::string::npos);
  return out;
}

inline std::string second_field(const std::string& text, const std::string& sep) {
  const std::size_t at = text.find(sep);
  REQUIRE_MESSAGE(at != std::string::npos, "no " << sep << " in the text");
  const std::size_t from = at + sep.size();
  const std::size_t stop = text.find(sep, from);
  return text.substr(from, stop == std::string::npos ? std::string::npos : stop - from);
}

inline std::string between(const std::string& text, const std::string& start, const std::string& end) {
  const std::string piece = second_field(text, start);
  return piece.substr(0, piece.find(end));
}

inline std::string format_d(const std::string& text, const std::vector<std::int64_t>& values) {
  std::string out;
  std::size_t at = 0;
  for (std::int64_t v : values) {
    const std::size_t hit = text.find("%d", at);
    REQUIRE(hit != std::string::npos);
    out.append(text, at, hit - at);
    out += std::to_string(v);
    at = hit + 2;
  }
  REQUIRE(text.find("%d", at) == std::string::npos);
  out.append(text, at, std::string::npos);
  return out;
}

inline void poke(Bytes& memory, std::int64_t addr, const Bytes& bytes) {
  for (std::size_t k = 0; k < bytes.size(); ++k) memory[static_cast<std::size_t>(addr) + k] = bytes[k];
}

inline Bytes slice(const Bytes& memory, std::int64_t lo, std::int64_t hi) {
  return {memory.begin() + lo, memory.begin() + hi};
}

struct LoggedRun {
  sim::SimResult result;
  Bytes log;
};

inline LoggedRun run_logged(const std::string& source, bool optimize, const std::string& log_symbol, std::int64_t size,
                            std::uint64_t max_cycles = 2'000'000) {
  sim::Simulation s = build_sim(source, optimize);
  LoggedRun out{s.run(max_cycles), {}};
  CHECK_FALSE_MESSAGE(out.result.timed_out, "the run timed out with optimize=" << optimize);
  const std::int64_t at = s.symbol(log_symbol);
  out.log = slice(out.result.memory, at, at + size);
  return out;
}

inline Bytes hargs_head(const sim::SimResult& result) { return slice(result.memory, result.hargs, result.hargs + 2); }

inline Bytes flatten(const std::vector<Bytes>& rows) {
  Bytes out;
  for (const Bytes& row : rows) out.insert(out.end(), row.begin(), row.end());
  return out;
}

}  // namespace c6502::suite
