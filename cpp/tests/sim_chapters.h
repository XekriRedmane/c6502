#pragma once

#include <cstdint>
#include <map>
#include <string_view>
#include <utility>
#include <vector>

namespace c6502::suite {

// What `main` of each chapter file returns, traced by hand, by the
// file's path under tests/, in the table's order.
const std::vector<std::pair<std::string_view, std::int64_t>>& expected_returns();

// The chapter files the runs don't attempt, with the reason.
const std::map<std::string_view, std::string_view>& skipped();

// The files of `expected_returns` that the simulator can't run, with
// the category of the reason.
const std::map<std::string_view, std::string_view>& sim_skips();

// The files whose optimized and unoptimized runs are known to differ.
const std::map<std::string_view, std::string_view>& opt_diverges();

}  // namespace c6502::suite
