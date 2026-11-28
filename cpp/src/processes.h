#pragma once

#include <cstddef>
#include <filesystem>
#include <map>
#include <string>
#include <vector>

#ifndef _WIN32
#include <sys/types.h>
#endif

namespace c6502::processes {

// A started child: its HANDLE on Windows, and its process ID elsewhere.
#ifdef _WIN32
using Process = void*;
#else
using Process = pid_t;
#endif

// The most children a `Children` runs at once. On Windows it's
// MAXIMUM_WAIT_OBJECTS, the most handles WaitForMultipleObjects waits
// for, and elsewhere there is no limit.
std::size_t max_children();

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
  std::map<std::size_t, Process> running_;
  std::size_t next_ = 0;
};

}  // namespace c6502::processes
