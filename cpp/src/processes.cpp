#include "processes.h"

#include <iterator>
#include <limits>
#include <stdexcept>
#include <type_traits>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#else
#include <fcntl.h>
#include <spawn.h>
#include <sys/wait.h>
#include <unistd.h>
extern char** environ;
#endif

#include "compile_command.h"

namespace c6502::processes {

#ifdef _WIN32
static_assert(std::is_same_v<Process, HANDLE>);
#endif

namespace {

#ifdef _WIN32
// Quotes `arg` by the rules CommandLineToArgvW splits a command line by.
std::wstring quote_arg(const std::wstring& arg) {
  if (!arg.empty() && arg.find_first_of(L" \t\"") == std::wstring::npos) return arg;
  std::wstring out = L"\"";
  std::size_t backslashes = 0;
  for (wchar_t c : arg) {
    if (c == L'\\') {
      ++backslashes;
      continue;
    }
    if (c == L'"') {
      out.append(backslashes * 2 + 1, L'\\');
    } else {
      out.append(backslashes, L'\\');
    }
    backslashes = 0;
    out += c;
  }
  out.append(backslashes * 2, L'\\');
  return out + L"\"";
}

HANDLE spawn(const std::vector<std::string>& argv, Output output) {
  std::wstring line;
  for (const std::string& a : argv) {
    if (!line.empty()) line += L' ';
    line += quote_arg(compile_command::utf8_path(a).wstring());
  }
  STARTUPINFOW si{};
  si.cb = sizeof si;
  HANDLE null_device = INVALID_HANDLE_VALUE;
  if (output == Output::kDiscard) {
    SECURITY_ATTRIBUTES sa{sizeof sa, nullptr, TRUE};
    null_device = CreateFileW(L"NUL", GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, &sa,
                              OPEN_EXISTING, 0, nullptr);
    si.dwFlags = STARTF_USESTDHANDLES;
    si.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
    si.hStdOutput = null_device;
    si.hStdError = null_device;
  }
  PROCESS_INFORMATION pi{};
  const BOOL ok =
      CreateProcessW(nullptr, line.data(), nullptr, nullptr, TRUE, 0, nullptr, nullptr, &si, &pi);
  if (null_device != INVALID_HANDLE_VALUE) CloseHandle(null_device);
  if (!ok) throw std::runtime_error("can't start " + argv.front());
  CloseHandle(pi.hThread);
  return pi.hProcess;
}

int wait_for(HANDLE process) {
  WaitForSingleObject(process, INFINITE);
  DWORD code = 1;
  GetExitCodeProcess(process, &code);
  CloseHandle(process);
  return static_cast<int>(code);
}
#else
pid_t spawn(const std::vector<std::string>& argv, Output output) {
  std::vector<char*> args;
  for (const std::string& a : argv) args.push_back(const_cast<char*>(a.c_str()));
  args.push_back(nullptr);
  posix_spawn_file_actions_t actions;
  posix_spawn_file_actions_init(&actions);
  if (output == Output::kDiscard) {
    posix_spawn_file_actions_addopen(&actions, 1, "/dev/null", O_WRONLY, 0);
    posix_spawn_file_actions_adddup2(&actions, 1, 2);
  }
  pid_t pid = 0;
  const int error = posix_spawn(&pid, args[0], &actions, nullptr, args.data(), environ);
  posix_spawn_file_actions_destroy(&actions);
  if (error != 0) throw std::runtime_error("can't start " + argv.front());
  return pid;
}

int status_of(int status) { return WIFEXITED(status) ? WEXITSTATUS(status) : 1; }

int wait_for(pid_t pid) {
  int status = 0;
  waitpid(pid, &status, 0);
  return status_of(status);
}
#endif

}  // namespace

std::filesystem::path self_path(const std::string& fallback) {
#ifdef _WIN32
  wchar_t buf[MAX_PATH * 4];
  const DWORD n = GetModuleFileNameW(nullptr, buf, static_cast<DWORD>(std::size(buf)));
  if (n > 0 && n < std::size(buf)) return std::filesystem::path(std::wstring(buf, n));
#else
  char buf[4096];
  const ssize_t n = readlink("/proc/self/exe", buf, sizeof buf - 1);
  if (n > 0) return std::filesystem::path(std::string(buf, static_cast<std::size_t>(n)));
#endif
  return compile_command::utf8_path(fallback);
}

int run(const std::vector<std::string>& argv, Output output) {
  return wait_for(spawn(argv, output));
}

std::vector<int> run_all(const std::vector<std::vector<std::string>>& commands) {
  std::vector<Process> started;
  for (const auto& argv : commands) started.push_back(spawn(argv, Output::kInherit));
  std::vector<int> statuses;
  for (auto p : started) statuses.push_back(wait_for(p));
  return statuses;
}

std::size_t max_children() {
#ifdef _WIN32
  return MAXIMUM_WAIT_OBJECTS;
#else
  return std::numeric_limits<std::size_t>::max();
#endif
}

Children::~Children() {
  for (const auto& [id, p] : running_) wait_for(p);
}

std::size_t Children::start(const std::vector<std::string>& argv) {
  if (running_.size() >= max_children()) throw std::runtime_error("too many child processes");
  running_[next_] = spawn(argv, Output::kInherit);
  return next_++;
}

Children::Exit Children::wait_any() {
  if (running_.empty()) throw std::logic_error("wait_any with no child running");
#ifdef _WIN32
  std::vector<HANDLE> handles;
  std::vector<std::size_t> ids;
  for (const auto& [id, p] : running_) {
    handles.push_back(p);
    ids.push_back(id);
  }
  const DWORD r = WaitForMultipleObjects(static_cast<DWORD>(handles.size()), handles.data(), FALSE,
                                         INFINITE);
  if (r >= WAIT_OBJECT_0 + handles.size()) throw std::runtime_error("can't wait for a child process");
  const std::size_t i = r - WAIT_OBJECT_0;
  running_.erase(ids[i]);
  return {ids[i], wait_for(handles[i])};
#else
  int status = 0;
  while (true) {
    const pid_t pid = waitpid(-1, &status, 0);
    if (pid < 0) throw std::runtime_error("can't wait for a child process");
    for (auto it = running_.begin(); it != running_.end(); ++it) {
      if (it->second == pid) {
        const std::size_t id = it->first;
        running_.erase(it);
        return {id, status_of(status)};
      }
    }
  }
#endif
}

}  // namespace c6502::processes
