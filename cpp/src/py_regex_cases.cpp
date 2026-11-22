#include "py_regex_cases.h"

#include <optional>

#include "json.h"
#include "py_regex.h"
#include "pyrepr.h"

namespace c6502::py_regex {

namespace {

std::u32string code_points(std::string_view utf8) {
  std::u32string out;
  for (const auto c : pyrepr::code_points(utf8)) out += static_cast<char32_t>(c);
  return out;
}

}  // namespace

RecordCheck check_record(std::string_view record) {
  const json::Value c = json::parse(record);
  const std::string pattern = c.get("pattern")->string;
  const json::Value* error = c.get("error");
  const bool python_raised = error != nullptr && error->kind != json::Value::Kind::kNull;
  const std::string want_error =
      python_raised ? error->items[0].string + ": " + error->items[1].string : std::string();
  std::optional<Regex> regex;
  try {
    regex.emplace(pattern);
  } catch (const Error& e) {
    const std::string got = e.type + ": " + e.message;
    if (got == want_error) return {};
    return {RecordCheck::Outcome::kDiffer,
            "raised " + got + (python_raised ? " where Python raised " + want_error
                                             : " where Python compiled it")};
  } catch (const Unsupported& e) {
    return {RecordCheck::Outcome::kUnsupported, e.message};
  }
  if (python_raised) {
    return {RecordCheck::Outcome::kDiffer, "compiled where Python raised " + want_error};
  }
  const json::Value* subjects = c.get("subjects");
  const json::Value* ends = c.get("ends");
  for (std::size_t i = 0; i < subjects->items.size(); ++i) {
    const auto m = regex->match(code_points(subjects->items[i].string), 0);
    const long long got = m ? static_cast<long long>(*m) : -1;
    const long long want = static_cast<long long>(ends->items[i].integer);
    if (got != want) {
      return {RecordCheck::Outcome::kDiffer,
              "on " + pyrepr::string(subjects->items[i].string) + " the match ends at " +
                  std::to_string(got) + " where Python's ends at " + std::to_string(want)};
    }
  }
  return {};
}

}  // namespace c6502::py_regex
