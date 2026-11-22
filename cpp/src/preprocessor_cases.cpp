#include "preprocessor_cases.h"

#include <map>
#include <memory>
#include <optional>
#include <stdexcept>
#include <vector>

#include "diagnostics.h"
#include "errors.h"
#include "json.h"
#include "pcpp_text.h"
#include "preprocessor.h"

namespace c6502::preprocessor {

namespace {

// Returns the bytes of a string whose code points are 0 to 255.
std::string latin1_bytes(std::string_view utf8) {
  std::string out;
  for (std::size_t i = 0; i < utf8.size();) {
    std::size_t len = 0;
    out += static_cast<char>(pcpp::decode_at(utf8, i, len));
    i += len;
  }
  return out;
}

std::optional<std::string> string_or_null(const json::Value& c, const char* key) {
  const json::Value* v = c.get(key);
  if (v == nullptr || v->kind == json::Value::Kind::kNull) return std::nullopt;
  return v->string;
}

}  // namespace

std::string check_recorded_call(std::string_view record,
                                const std::map<std::string, std::string>* shared) {
  const json::Value c = json::parse(record);
  std::vector<std::string> argv;
  if (const json::Value* a = c.get("argv"); a != nullptr) {
    for (const auto& item : a->items) argv.push_back(item.string);
  }
  auto files = std::make_shared<std::map<std::string, std::string>>();
  if (const json::Value* f = c.get("files"); f != nullptr) {
    for (std::size_t i = 0; i < f->keys.size(); ++i) {
      const std::string& v = f->items[i].string;
      if (shared != nullptr && !v.empty() && v[0] == '@') {
        const auto it = shared->find(v.substr(1));
        if (it == shared->end()) throw std::runtime_error("no shared file " + v);
        (*files)[f->keys[i]] = latin1_bytes(it->second);
      } else {
        (*files)[f->keys[i]] = latin1_bytes(v);
      }
    }
  }
  const std::string include_dir = *string_or_null(c, "include_dir");
  pcpp::Host host;
  host.paths.flavor = c.get("nt")->boolean ? pcpp::PathFlavor::Nt : pcpp::PathFlavor::Posix;
  host.paths.cwd = *string_or_null(c, "cwd");
  host.encoding = host.paths.flavor == pcpp::PathFlavor::Nt ? "cp1252" : "utf-8";
  host.read_file = [files](const std::string& p) -> std::optional<std::string> {
    const auto it = files->find(p);
    if (it == files->end()) return std::nullopt;
    return it->second;
  };
  const char sep = host.paths.sep();
  host.isdir = [files, include_dir, sep](const std::string& p) {
    if (p == include_dir) return true;
    for (const auto& [k, v] : *files) {
      if (k.size() > p.size() && k.compare(0, p.size(), p) == 0 && k[p.size()] == sep) {
        return true;
      }
    }
    return false;
  };
  host.localtime = [] {
    std::tm tm{};
    tm.tm_year = 126;
    tm.tm_mon = 10;
    tm.tm_mday = 21;
    return tm;
  };
  auto debug_log = std::make_shared<std::optional<std::string>>();
  host.write_debug_log = [debug_log](const std::string& bytes) { *debug_log = bytes; };
  std::vector<diagnostics::CompileWarning> found;
  std::string got_text;
  std::optional<std::pair<std::string, std::string>> got_error;
  std::string got_stderr;
  {
    diagnostics::Collecting block(found);
    try {
      got_text = preprocess(*string_or_null(c, "source"), argv, string_or_null(c, "filename"),
                            host, include_dir, &got_stderr)
                     .text;
    } catch (const CompileError& e) {
      got_error = std::make_pair(e.python_type(), std::string(e.what()));
    }
  }
  if (const json::Value* e = c.get("error"); e != nullptr && e->kind != json::Value::Kind::kNull) {
    const std::string want = e->items[0].string + ": " + e->items[1].string;
    if (!got_error) return "returned text where Python raised " + want;
    const std::string got = got_error->first + ": " + got_error->second;
    if (got != want) return "raised " + got + " where Python raised " + want;
  } else {
    if (got_error) {
      return "raised " + got_error->first + ": " + got_error->second + " where Python returned";
    }
    if (got_text != *string_or_null(c, "output")) {
      return "the text differs:\n" + got_text + "\n--- Python's:\n" + *string_or_null(c, "output");
    }
  }
  std::vector<diagnostics::CompileWarning> want;
  for (const auto& w : c.get("warnings")->items) {
    want.push_back({w.items[0].string, w.items[1].string});
  }
  if (found != want) return "the warnings differ";
  if (got_stderr != *string_or_null(c, "stderr")) {
    return "stderr differs:\n" + got_stderr + "--- Python's:\n" + *string_or_null(c, "stderr");
  }
  if (c.get("debug_log") != nullptr) {
    const auto want_log = string_or_null(c, "debug_log");
    const std::optional<std::string> want_bytes =
        want_log ? std::optional<std::string>(latin1_bytes(*want_log)) : std::nullopt;
    if (*debug_log != want_bytes) {
      return "pcpp_debug.log differs:\n" + debug_log->value_or("(not written)") +
             "--- Python's:\n" + want_bytes.value_or("(not written)");
    }
  }
  return "";
}

}  // namespace c6502::preprocessor
