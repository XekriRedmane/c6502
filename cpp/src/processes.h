// Child processes, for the commands that spread their work over
// processes of their own (check-golden and the fuzzers) and for the
// fuzzer of YARPGen's programs, which runs YARPGen.
#pragma once

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <map>
#include <string>
#include <vector>

namespace c6502::processes {

// Returns the path of the running executable, or `fallback` where the
// platform doesn't say.
std::filesystem::path self_path(const std::string& fallback);

// Whether a child writes to the parent's stdout and stderr or to the
// null device.
enum class Output { kInherit, kDiscard };

// Runs the command line `argv` (the program first, found by its path)
// and returns its exit status. Throws `std::runtime_error` when it can't
// be started.
int run(const std::vector<std::string>& argv, Output output = Output::kInherit);

// Starts each command line of `commands` and waits for all of them,
// returning their exit statuses.
std::vector<int> run_all(const std::vector<std::vector<std::string>>& commands);

// A set of running children, each named by the number `start` returned.
class Children {
 public:
  Children() = default;
  Children(const Children&) = delete;
  Children& operator=(const Children&) = delete;
  ~Children();

  // Starts `argv` and returns its number. Throws `std::runtime_error`
  // when it can't be started.
  std::size_t start(const std::vector<std::string>& argv);

  struct Exit {
    std::size_t id = 0;
    int status = 0;
  };
  // Waits for any of the running children to end, and returns its
  // number and exit status. There must be one running.
  Exit wait_any();

  std::size_t running() const { return running_.size(); }

 private:
  // The process's handle on Windows, and its ID elsewhere, by number.
  std::map<std::size_t, std::intptr_t> running_;
  std::size_t next_ = 0;
};

}  // namespace c6502::processes
