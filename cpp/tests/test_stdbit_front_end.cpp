// tests/test_stdbit.py's TestTypes::test_result_types, ported by hand to
// the C++ front end. Its recorded calls would have held the type checker's
// tables for 140 programs that include <stdbit.h>, about 18 MB, so the case
// runs the same 140 calls without the preprocessor, by the builtins' own
// names, which the header's macros give the `stdc_` names (the Python
// tests of the header, recorded in test_stdbit_recorded.cpp, check those).
#include <cstddef>
#include <string>
#include <string_view>

#include "asdl/runtime.h"
#include "doctest.h"
#include "passes/front_end.h"

namespace {

// Returns the dump of the first call of `name` in the dump `text`, from
// its constructor to its closing parenthesis.
std::string call_of(const std::string& text, const std::string& name) {
  const std::size_t start = text.find("FunctionCall(\"" + name + "\"");
  REQUIRE(start != std::string::npos);
  int depth = 0;
  bool quoted = false;
  for (std::size_t i = start; i < text.size(); ++i) {
    const char c = text[i];
    if (quoted) {
      if (c == '\\') ++i;
      else if (c == '"') quoted = false;
    } else if (c == '"') {
      quoted = true;
    } else if (c == '(') {
      ++depth;
    } else if (c == ')' && --depth == 0) {
      return text.substr(start, i + 1 - start);
    }
  }
  FAIL("unbalanced dump");
  return "";
}

struct ValueType {
  const char* suffix;
  const char* name;
  const char* dump;  // the type's constructor in the dump
};

constexpr ValueType kTypes[] = {
    {"uc", "unsigned char", "UChar"},
    {"us", "unsigned short", "UShort"},
    {"ui", "unsigned int", "UInt"},
    {"ul", "unsigned long", "ULong"},
    {"ull", "unsigned long long", "ULongLong"},
};

// builtin_functions.py's STDBIT_OPERATIONS.
constexpr const char* kOperations[] = {
    "leading_zeros", "leading_ones", "trailing_zeros", "trailing_ones",
    "first_leading_zero", "first_leading_one", "first_trailing_zero",
    "first_trailing_one", "count_zeros", "count_ones", "has_single_bit",
    "bit_width", "bit_floor", "bit_ceil",
};

}  // namespace

TEST_CASE("stdbit: test_stdbit.py::TestTypes::test_result_types") {
  // `bit_floor` and `bit_ceil` give the value's type, `has_single_bit` a
  // `bool`, and the rest an `unsigned int`, in both forms.
  for (const ValueType& t : kTypes) {
    for (const char* op : kOperations) {
      const std::string_view o = op;
      const bool same = o == "bit_floor" || o == "bit_ceil";
      const std::string result = same ? t.name : o == "has_single_bit" ? "bool" : "unsigned int";
      const std::string want = same ? t.dump : o == "has_single_bit" ? "Bool" : "UInt";
      for (const std::string& fn : {std::string("__c6502_stdc_") + op,
                                    std::string("__c6502_stdc_") + op + "_" + t.suffix}) {
        const std::string src = std::string(t.name) + " g;\nvoid f(void) { " + result +
                                " r = " + fn + "(g); }\n";
        CAPTURE(src);
        c6502::passes::FrontEndOptions options;
        options.preprocess = false;
        std::string err;
        const auto r = c6502::passes::front_end(src, options, err);
        const std::string call = call_of(c6502::asdl::dump(r.prog), fn);
        CAPTURE(call);
        const std::string tail = "] " + want + " false)";
        CHECK(call.size() > tail.size());
        CHECK(call.compare(call.size() - tail.size(), tail.size(), tail) == 0);
      }
    }
  }
}
