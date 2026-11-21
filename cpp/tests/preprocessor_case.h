// A recorded call of `preprocessor.preprocess` for the tests of the
// preprocessor, checked by `preprocessor::check_recorded_call`
// (src/preprocessor_cases.h describes the record).
#pragma once

#include <map>
#include <string>
#include <string_view>

#include "doctest.h"
#include "preprocessor_cases.h"

namespace c6502::testing {

// Checks that the C++ preprocessor gives the recorded call's result,
// warnings, and stderr, with the headers the records share in `shared`.
inline void check_preprocess_case(std::string_view record,
                                  const std::map<std::string, std::string>& shared) {
  const std::string mismatch = preprocessor::check_recorded_call(record, &shared);
  INFO(record.substr(0, 2000));
  CHECK_MESSAGE(mismatch.empty(), mismatch);
}

}  // namespace c6502::testing
