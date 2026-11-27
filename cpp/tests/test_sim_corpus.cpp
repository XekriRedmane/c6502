// The simulator's runs of the chapter corpus, from test_sim_asm.py,
// test_sim_asm_optimized.py, and test_sim_differential.py: each program
// of `expected_returns` runs in one pipeline or both, and its `int`
// return must equal the traced value, or the two pipelines' returns
// must equal each other with the optimized run no slower. The runs are
// split into shards, each a case that ctest runs by itself
// (cpp/CMakeLists.txt), so that `ctest -j` spreads them.
#include <algorithm>
#include <cstdio>
#include <exception>
#include <string>
#include <vector>

#include "compile_case.h"
#include "doctest.h"
#include "sim_case.h"
#include "sim_chapters.h"

namespace c6502::suite {
namespace {

constexpr std::uint64_t kMaxCycles = 5'000'000;
constexpr int kShards = 4;

std::vector<std::pair<std::string_view, std::int64_t>> int_range_cases() {
  std::vector<std::pair<std::string_view, std::int64_t>> out;
  for (const auto& [path, value] : expected_returns()) {
    if (skipped().count(path) != 0) continue;
    if (value < -32768 || value > 32767) continue;
    out.emplace_back(path, value);
  }
  return out;
}

std::string state(const sim::SimResult& r) {
  char buf[96];
  std::snprintf(buf, sizeof buf, "A:X=%02x%02x A=$%02X X=$%02X Y=$%02X cycles=%llu%s", r.a, r.x, r.a, r.x, r.y,
                static_cast<unsigned long long>(r.cycles), r.timed_out ? " TIMED_OUT" : "");
  return buf;
}

void check_returns(const std::vector<std::pair<std::string_view, std::int64_t>>& cases, Pipeline pipeline, int shard,
                   int shards) {
  REQUIRE(!cases.empty());
  int skips = 0, ran = 0;
  for (std::size_t i = static_cast<std::size_t>(shard); i < cases.size(); i += static_cast<std::size_t>(shards)) {
    const auto& [path, expected] = cases[i];
    if (sim_skips().count(path) != 0) {
      ++skips;
      continue;
    }
    INFO(path);
    ++ran;
    try {
      const sim::SimResult r =
          build_sim(read_repo_file("tests/" + std::string(path)), pipeline).run(kMaxCycles);
      CHECK_MESSAGE(!r.timed_out, "the simulator timed out after " << r.cycles << " cycles");
      CHECK_MESSAGE(r.return_int_signed() == expected, state(r));
    } catch (const std::exception& e) {
      FAIL_CHECK(e.what());
    }
  }
  MESSAGE(ran << " programs run, " << skips << " skipped by sim_skips");
}

void check_differential(int shard) {
  std::vector<std::string_view> cases;
  for (const auto& [path, value] : expected_returns()) cases.push_back(path);
  std::sort(cases.begin(), cases.end());
  REQUIRE(!cases.empty());
  int skips = 0, ran = 0;
  for (std::size_t i = static_cast<std::size_t>(shard); i < cases.size(); i += kShards) {
    const std::string_view path = cases[i];
    if (skipped().count(path) != 0 || sim_skips().count(path) != 0 || opt_diverges().count(path) != 0) {
      ++skips;
      continue;
    }
    INFO(path);
    ++ran;
    const std::string source = read_repo_file("tests/" + std::string(path));
    sim::SimResult unopt, opt;
    try {
      unopt = build_sim(source, Pipeline::kUnoptimized).run(kMaxCycles);
    } catch (const std::exception& e) {
      FAIL_CHECK("unopt compile/sim error: " << e.what());
      continue;
    }
    try {
      opt = build_sim(source, Pipeline::kOptimized).run(kMaxCycles);
    } catch (const std::exception& e) {
      FAIL_CHECK("opt compile/sim error: " << e.what());
      continue;
    }
    if (unopt.timed_out || opt.timed_out) {
      FAIL_CHECK("timed out: unopt " << state(unopt) << ", opt " << state(opt));
      continue;
    }
    CHECK_MESSAGE(opt.return_int() == unopt.return_int(),
                  "opt vs unopt return-value divergence: unopt " << state(unopt) << ", opt " << state(opt));
    CHECK_MESSAGE(opt.cycles <= unopt.cycles,
                  "the optimizer regressed cycles (" << unopt.cycles << " -> " << opt.cycles << ")");
  }
  MESSAGE(ran << " programs run, " << skips << " skipped");
}

}  // namespace

TEST_CASE("shard: sim asm 0") { check_returns(int_range_cases(), Pipeline::kUnoptimized, 0, kShards); }
TEST_CASE("shard: sim asm 1") { check_returns(int_range_cases(), Pipeline::kUnoptimized, 1, kShards); }
TEST_CASE("shard: sim asm 2") { check_returns(int_range_cases(), Pipeline::kUnoptimized, 2, kShards); }
TEST_CASE("shard: sim asm 3") { check_returns(int_range_cases(), Pipeline::kUnoptimized, 3, kShards); }

TEST_CASE("sim asm optimized: the programs of chapters 1 to 12 return their traced values under -O") {
  std::vector<std::pair<std::string_view, std::int64_t>> cases;
  for (const auto& c : int_range_cases()) {
    for (int n = 1; n <= 12; ++n) {
      if (c.first.substr(0, c.first.find('/') + 1) == "chapter_" + std::to_string(n) + "/") {
        cases.push_back(c);
        break;
      }
    }
  }
  check_returns(cases, Pipeline::kOptimized, 0, 1);
}

TEST_CASE("shard: sim differential 0") { check_differential(0); }
TEST_CASE("shard: sim differential 1") { check_differential(1); }
TEST_CASE("shard: sim differential 2") { check_differential(2); }
TEST_CASE("shard: sim differential 3") { check_differential(3); }

}  // namespace c6502::suite
