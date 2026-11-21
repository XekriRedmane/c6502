// The Python pp_numbers functions' results on spellings that reach
// every branch of the module, recorded by a script from Python's
// results, since the corpus spells few constants with separators,
// binary digits, floating forms, or `wb` suffixes.
#include <string>

#include "doctest.h"
#include "pp_numbers.h"

namespace P = c6502::pp_numbers;

namespace {

// Returns the message of the NumberError `f` throws, or empty.
template <class F>
std::string error_of(F f) {
  try {
    f();
  } catch (const P::NumberError& e) {
    return e.what();
  }
  return "";
}

}  // namespace

TEST_CASE("parse_integer gives Python's constants and messages") {
  {
    const auto c = P::parse_integer("0");
    CHECK(c.bits == 0);
    CHECK(c6502::asdl::int_to_string(c.value) == "0");
    CHECK(c.base == 8);
    CHECK(c.is_unsigned == false);
    CHECK(c.length == "");
  }
  {
    const auto c = P::parse_integer("7");
    CHECK(c.bits == 3);
    CHECK(c6502::asdl::int_to_string(c.value) == "7");
    CHECK(c.base == 10);
    CHECK(c.is_unsigned == false);
    CHECK(c.length == "");
  }
  CHECK(error_of([] { P::parse_integer("08"); }) ==
        "invalid digit '8' in octal constant '08'");
  {
    const auto c = P::parse_integer("0'17");
    CHECK(c.bits == 4);
    CHECK(c6502::asdl::int_to_string(c.value) == "15");
    CHECK(c.base == 8);
    CHECK(c.is_unsigned == false);
    CHECK(c.length == "");
  }
  {
    const auto c = P::parse_integer("017");
    CHECK(c.bits == 4);
    CHECK(c6502::asdl::int_to_string(c.value) == "15");
    CHECK(c.base == 8);
    CHECK(c.is_unsigned == false);
    CHECK(c.length == "");
  }
  {
    const auto c = P::parse_integer("1'000'000");
    CHECK(c.bits == 20);
    CHECK(c6502::asdl::int_to_string(c.value) == "1000000");
    CHECK(c.base == 10);
    CHECK(c.is_unsigned == false);
    CHECK(c.length == "");
  }
  {
    const auto c = P::parse_integer("0xFF'FF");
    CHECK(c.bits == 16);
    CHECK(c6502::asdl::int_to_string(c.value) == "65535");
    CHECK(c.base == 16);
    CHECK(c.is_unsigned == false);
    CHECK(c.length == "");
  }
  {
    const auto c = P::parse_integer("0XAb");
    CHECK(c.bits == 8);
    CHECK(c6502::asdl::int_to_string(c.value) == "171");
    CHECK(c.base == 16);
    CHECK(c.is_unsigned == false);
    CHECK(c.length == "");
  }
  {
    const auto c = P::parse_integer("0b1010");
    CHECK(c.bits == 4);
    CHECK(c6502::asdl::int_to_string(c.value) == "10");
    CHECK(c.base == 2);
    CHECK(c.is_unsigned == false);
    CHECK(c.length == "");
  }
  {
    const auto c = P::parse_integer("0B1");
    CHECK(c.bits == 1);
    CHECK(c6502::asdl::int_to_string(c.value) == "1");
    CHECK(c.base == 2);
    CHECK(c.is_unsigned == false);
    CHECK(c.length == "");
  }
  CHECK(error_of([] { P::parse_integer("0b"); }) ==
        "no digits after the prefix 0b in '0b'");
  CHECK(error_of([] { P::parse_integer("0x"); }) ==
        "no digits after the prefix 0x in '0x'");
  CHECK(error_of([] { P::parse_integer("0b2"); }) ==
        "invalid digit '2' in binary constant '0b2'");
  CHECK(error_of([] { P::parse_integer("0b1'2"); }) ==
        "invalid digit '2' in binary constant \"0b1'2\"");
  CHECK(error_of([] { P::parse_integer("0'8"); }) ==
        "invalid digit '8' in octal constant \"0'8\"");
  CHECK(error_of([] { P::parse_integer("1'"); }) ==
        "a digit separator has to be between two digits, and the one in \"1'\" ends the number");
  CHECK(error_of([] { P::parse_integer("1''0"); }) ==
        "a digit separator has to be between two digits, and the one in \"1''0\" is doubled");
  CHECK(error_of([] { P::parse_integer("0x'FF"); }) ==
        "a digit separator has to be between two digits, and the one in \"0x'FF\" comes right after the prefix 0x");
  CHECK(error_of([] { P::parse_integer("0b'1"); }) ==
        "a digit separator has to be between two digits, and the one in \"0b'1\" comes right after the prefix 0b");
  CHECK(error_of([] { P::parse_integer("1'u"); }) ==
        "a digit separator has to be between two digits, and the one in \"1'u\" comes right before the suffix");
  {
    const auto c = P::parse_integer("123u");
    CHECK(c.bits == 7);
    CHECK(c6502::asdl::int_to_string(c.value) == "123");
    CHECK(c.base == 10);
    CHECK(c.is_unsigned == true);
    CHECK(c.length == "");
  }
  {
    const auto c = P::parse_integer("123U");
    CHECK(c.bits == 7);
    CHECK(c6502::asdl::int_to_string(c.value) == "123");
    CHECK(c.base == 10);
    CHECK(c.is_unsigned == true);
    CHECK(c.length == "");
  }
  {
    const auto c = P::parse_integer("123ul");
    CHECK(c.bits == 7);
    CHECK(c6502::asdl::int_to_string(c.value) == "123");
    CHECK(c.base == 10);
    CHECK(c.is_unsigned == true);
    CHECK(c.length == "l");
  }
  {
    const auto c = P::parse_integer("123uLL");
    CHECK(c.bits == 7);
    CHECK(c6502::asdl::int_to_string(c.value) == "123");
    CHECK(c.base == 10);
    CHECK(c.is_unsigned == true);
    CHECK(c.length == "ll");
  }
  {
    const auto c = P::parse_integer("123lu");
    CHECK(c.bits == 7);
    CHECK(c6502::asdl::int_to_string(c.value) == "123");
    CHECK(c.base == 10);
    CHECK(c.is_unsigned == true);
    CHECK(c.length == "l");
  }
  {
    const auto c = P::parse_integer("123LLU");
    CHECK(c.bits == 7);
    CHECK(c6502::asdl::int_to_string(c.value) == "123");
    CHECK(c.base == 10);
    CHECK(c.is_unsigned == true);
    CHECK(c.length == "ll");
  }
  CHECK(error_of([] { P::parse_integer("123lL"); }) ==
        "invalid suffix 'lL' on integer constant '123lL'");
  {
    const auto c = P::parse_integer("123wb");
    CHECK(c.bits == 7);
    CHECK(c6502::asdl::int_to_string(c.value) == "123");
    CHECK(c.base == 10);
    CHECK(c.is_unsigned == false);
    CHECK(c.length == "wb");
  }
  {
    const auto c = P::parse_integer("123uwb");
    CHECK(c.bits == 7);
    CHECK(c6502::asdl::int_to_string(c.value) == "123");
    CHECK(c.base == 10);
    CHECK(c.is_unsigned == true);
    CHECK(c.length == "wb");
  }
  {
    const auto c = P::parse_integer("123WBU");
    CHECK(c.bits == 7);
    CHECK(c6502::asdl::int_to_string(c.value) == "123");
    CHECK(c.base == 10);
    CHECK(c.is_unsigned == true);
    CHECK(c.length == "wb");
  }
  CHECK(error_of([] { P::parse_integer("123wB"); }) ==
        "invalid suffix 'wB' on integer constant '123wB'");
  CHECK(error_of([] { P::parse_integer("123z"); }) ==
        "invalid suffix 'z' on integer constant '123z'");
  CHECK(error_of([] { P::parse_integer("0b1.0"); }) ==
        "a binary constant is an integer, so '0b1.0' can't have a period");
  CHECK(error_of([] { P::parse_integer("0b1."); }) ==
        "a binary constant is an integer, so '0b1.' can't have a period");
  CHECK(error_of([] { P::parse_integer("0x1g"); }) ==
        "invalid suffix 'g' on integer constant '0x1g'");
  {
    const auto c = P::parse_integer("12ll");
    CHECK(c.bits == 4);
    CHECK(c6502::asdl::int_to_string(c.value) == "12");
    CHECK(c.base == 10);
    CHECK(c.is_unsigned == false);
    CHECK(c.length == "ll");
  }
  {
    const auto c = P::parse_integer("12LL");
    CHECK(c.bits == 4);
    CHECK(c6502::asdl::int_to_string(c.value) == "12");
    CHECK(c.base == 10);
    CHECK(c.is_unsigned == false);
    CHECK(c.length == "ll");
  }
  {
    const auto c = P::parse_integer("12L");
    CHECK(c.bits == 4);
    CHECK(c6502::asdl::int_to_string(c.value) == "12");
    CHECK(c.base == 10);
    CHECK(c.is_unsigned == false);
    CHECK(c.length == "l");
  }
  {
    const auto c = P::parse_integer("12l");
    CHECK(c.bits == 4);
    CHECK(c6502::asdl::int_to_string(c.value) == "12");
    CHECK(c.base == 10);
    CHECK(c.is_unsigned == false);
    CHECK(c.length == "l");
  }
  {
    const auto c = P::parse_integer("12Ul");
    CHECK(c.bits == 4);
    CHECK(c6502::asdl::int_to_string(c.value) == "12");
    CHECK(c.base == 10);
    CHECK(c.is_unsigned == true);
    CHECK(c.length == "l");
  }
  {
    const auto c = P::parse_integer("12uL");
    CHECK(c.bits == 4);
    CHECK(c6502::asdl::int_to_string(c.value) == "12");
    CHECK(c.base == 10);
    CHECK(c.is_unsigned == true);
    CHECK(c.length == "l");
  }
  {
    const auto c = P::parse_integer("12llu");
    CHECK(c.bits == 4);
    CHECK(c6502::asdl::int_to_string(c.value) == "12");
    CHECK(c.base == 10);
    CHECK(c.is_unsigned == true);
    CHECK(c.length == "ll");
  }
  {
    const auto c = P::parse_integer("0xFFFFFFFFFFFFFFFF");
    CHECK(c.bits == 64);
    CHECK(c6502::asdl::int_to_string(c.value) == "18446744073709551615");
    CHECK(c.base == 16);
    CHECK(c.is_unsigned == false);
    CHECK(c.length == "");
  }
  {
    const auto c = P::parse_integer("18446744073709551616");
    CHECK(c.bits == 65);
    CHECK(c6502::asdl::int_to_string(c.value) == "18446744073709551616");
    CHECK(c.base == 10);
    CHECK(c.is_unsigned == false);
    CHECK(c.length == "");
  }
  {
    const auto c = P::parse_integer("99999999999999999999999999999999999999999");
    CHECK(c.bits == 137);
    CHECK(!c.fits());
    CHECK(c.base == 10);
    CHECK(c.is_unsigned == false);
    CHECK(c.length == "");
  }
  {
    const auto c = P::parse_integer("170141183460469231731687303715884105727");
    CHECK(c.bits == 127);
    CHECK(c6502::asdl::int_to_string(c.value) == "170141183460469231731687303715884105727");
    CHECK(c.base == 10);
    CHECK(c.is_unsigned == false);
    CHECK(c.length == "");
  }
  {
    const auto c = P::parse_integer("170141183460469231731687303715884105728");
    CHECK(c.bits == 128);
    CHECK(!c.fits());
    CHECK(c.base == 10);
    CHECK(c.is_unsigned == false);
    CHECK(c.length == "");
  }
  CHECK(error_of([] { P::parse_integer("0'"); }) ==
        "a digit separator has to be between two digits, and the one in \"0'\" ends the number");
  CHECK(error_of([] { P::parse_integer("0x1'"); }) ==
        "a digit separator has to be between two digits, and the one in \"0x1'\" ends the number");
  CHECK(error_of([] { P::parse_integer("1'_"); }) ==
        "a digit separator has to be between two digits, and the one in \"1'_\" comes right before the suffix");
}

TEST_CASE("check_floating gives Python's bodies, suffixes, and messages") {
  CHECK(P::is_floating("1.0") == true);
  CHECK(P::check_floating("1.0") ==
        std::pair<std::string, std::string>{"1.0", ""});
  CHECK(P::is_floating("1.") == true);
  CHECK(P::check_floating("1.") ==
        std::pair<std::string, std::string>{"1.", ""});
  CHECK(P::is_floating(".5") == true);
  CHECK(P::check_floating(".5") ==
        std::pair<std::string, std::string>{".5", ""});
  CHECK(P::is_floating("1e5") == true);
  CHECK(P::check_floating("1e5") ==
        std::pair<std::string, std::string>{"1e5", ""});
  CHECK(P::is_floating("1e+5") == true);
  CHECK(P::check_floating("1e+5") ==
        std::pair<std::string, std::string>{"1e+5", ""});
  CHECK(P::is_floating("1E-5") == true);
  CHECK(P::check_floating("1E-5") ==
        std::pair<std::string, std::string>{"1E-5", ""});
  CHECK(P::is_floating("1.5f") == true);
  CHECK(P::check_floating("1.5f") ==
        std::pair<std::string, std::string>{"1.5", "f"});
  CHECK(P::is_floating("1.5L") == true);
  CHECK(P::check_floating("1.5L") ==
        std::pair<std::string, std::string>{"1.5", "L"});
  CHECK(P::is_floating("1.5F") == true);
  CHECK(P::check_floating("1.5F") ==
        std::pair<std::string, std::string>{"1.5", "F"});
  CHECK(P::is_floating("1.5df") == true);
  CHECK(error_of([] { P::check_floating("1.5df"); }) ==
        "the suffix 'df' of '1.5df' gives a decimal floating type, which c6502 doesn't have");
  CHECK(P::is_floating("1.5DD") == true);
  CHECK(error_of([] { P::check_floating("1.5DD"); }) ==
        "the suffix 'DD' of '1.5DD' gives a decimal floating type, which c6502 doesn't have");
  CHECK(P::is_floating("1.5x") == true);
  CHECK(error_of([] { P::check_floating("1.5x"); }) ==
        "invalid suffix 'x' on floating constant '1.5x'");
  CHECK(P::is_floating("0x1p3") == true);
  CHECK(P::check_floating("0x1p3") ==
        std::pair<std::string, std::string>{"0x1p3", ""});
  CHECK(P::is_floating("0x1.8p-2") == true);
  CHECK(P::check_floating("0x1.8p-2") ==
        std::pair<std::string, std::string>{"0x1.8p-2", ""});
  CHECK(P::is_floating("0x1.8") == true);
  CHECK(error_of([] { P::check_floating("0x1.8"); }) ==
        "hexadecimal floating constant '0x1.8' has no binary exponent (a p and a power of 2)");
  CHECK(P::is_floating("0x.p1") == true);
  CHECK(error_of([] { P::check_floating("0x.p1"); }) ==
        "floating constant '0x.p1' has no digits before or after the period");
  CHECK(P::is_floating("1'.5") == true);
  CHECK(error_of([] { P::check_floating("1'.5"); }) ==
        "a digit separator has to be between two digits, and the one in \"1'.5\" touches the period");
  CHECK(P::is_floating("1.'5") == true);
  CHECK(error_of([] { P::check_floating("1.'5"); }) ==
        "a digit separator has to be between two digits, and the one in \"1.'5\" touches the period");
  CHECK(P::is_floating("1'e5") == true);
  CHECK(error_of([] { P::check_floating("1'e5"); }) ==
        "a digit separator has to be between two digits, and the one in \"1'e5\" comes right before the exponent");
  CHECK(P::is_floating("1e'5") == true);
  CHECK(error_of([] { P::check_floating("1e'5"); }) ==
        "a digit separator has to be between two digits, and the one in \"1e'5\" comes right after the exponent's letter or sign");
  CHECK(P::is_floating("1e+'5") == true);
  CHECK(error_of([] { P::check_floating("1e+'5"); }) ==
        "a digit separator has to be between two digits, and the one in \"1e+'5\" comes right after the exponent's letter or sign");
  CHECK(P::is_floating("1e") == true);
  CHECK(error_of([] { P::check_floating("1e"); }) ==
        "the exponent of '1e' has no digits");
  CHECK(P::is_floating("1.5e") == true);
  CHECK(error_of([] { P::check_floating("1.5e"); }) ==
        "the exponent of '1.5e' has no digits");
  CHECK(P::is_floating("0x1p") == true);
  CHECK(error_of([] { P::check_floating("0x1p"); }) ==
        "the exponent of '0x1p' has no digits");
  CHECK(P::is_floating("0x'1p3") == true);
  CHECK(error_of([] { P::check_floating("0x'1p3"); }) ==
        "a digit separator has to be between two digits, and the one in \"0x'1p3\" comes right after the prefix 0x");
  CHECK(P::is_floating("1'000.5e1'0") == true);
  CHECK(P::check_floating("1'000.5e1'0") ==
        std::pair<std::string, std::string>{"1000.5e10", ""});
  CHECK(P::is_floating("0x1.ep3") == true);
  CHECK(P::check_floating("0x1.ep3") ==
        std::pair<std::string, std::string>{"0x1.ep3", ""});
  CHECK(P::is_floating("0xe'1p3") == true);
  CHECK(P::check_floating("0xe'1p3") ==
        std::pair<std::string, std::string>{"0xe1p3", ""});
  CHECK(P::is_floating("0x1p'3") == true);
  CHECK(error_of([] { P::check_floating("0x1p'3"); }) ==
        "a digit separator has to be between two digits, and the one in \"0x1p'3\" comes right after the exponent's letter or sign");
  CHECK(P::is_floating("..5") == true);
  CHECK(error_of([] { P::check_floating("..5"); }) ==
        "floating constant '..5' has no digits before or after the period");
  CHECK(P::is_floating("1.e5f") == true);
  CHECK(P::check_floating("1.e5f") ==
        std::pair<std::string, std::string>{"1.e5", "f"});
  CHECK(P::is_floating("0x1.8p3L") == true);
  CHECK(P::check_floating("0x1.8p3L") ==
        std::pair<std::string, std::string>{"0x1.8p3", "L"});
  CHECK(P::is_floating("1e5ll") == true);
  CHECK(error_of([] { P::check_floating("1e5ll"); }) ==
        "invalid suffix 'll' on floating constant '1e5ll'");
  CHECK(P::is_floating("0x1e'p3") == true);
  CHECK(error_of([] { P::check_floating("0x1e'p3"); }) ==
        "a digit separator has to be between two digits, and the one in \"0x1e'p3\" comes right before the exponent");
}

TEST_CASE("check takes each spelling as Python's does") {
  CHECK(error_of([] { P::check("0"); }) == "");
  CHECK(error_of([] { P::check("7"); }) == "");
  CHECK(error_of([] { P::check("08"); }) == "invalid digit '8' in octal constant '08'");
  CHECK(error_of([] { P::check("0'17"); }) == "");
  CHECK(error_of([] { P::check("017"); }) == "");
  CHECK(error_of([] { P::check("1'000'000"); }) == "");
  CHECK(error_of([] { P::check("0xFF'FF"); }) == "");
  CHECK(error_of([] { P::check("0XAb"); }) == "");
  CHECK(error_of([] { P::check("0b1010"); }) == "");
  CHECK(error_of([] { P::check("0B1"); }) == "");
  CHECK(error_of([] { P::check("0b"); }) == "no digits after the prefix 0b in '0b'");
  CHECK(error_of([] { P::check("0x"); }) == "no digits after the prefix 0x in '0x'");
  CHECK(error_of([] { P::check("0b2"); }) == "invalid digit '2' in binary constant '0b2'");
  CHECK(error_of([] { P::check("0b1'2"); }) == "invalid digit '2' in binary constant \"0b1'2\"");
  CHECK(error_of([] { P::check("0'8"); }) == "invalid digit '8' in octal constant \"0'8\"");
  CHECK(error_of([] { P::check("1'"); }) == "a digit separator has to be between two digits, and the one in \"1'\" ends the number");
  CHECK(error_of([] { P::check("1''0"); }) == "a digit separator has to be between two digits, and the one in \"1''0\" is doubled");
  CHECK(error_of([] { P::check("0x'FF"); }) == "a digit separator has to be between two digits, and the one in \"0x'FF\" comes right after the prefix 0x");
  CHECK(error_of([] { P::check("0b'1"); }) == "a digit separator has to be between two digits, and the one in \"0b'1\" comes right after the prefix 0b");
  CHECK(error_of([] { P::check("1'u"); }) == "a digit separator has to be between two digits, and the one in \"1'u\" comes right before the suffix");
  CHECK(error_of([] { P::check("123u"); }) == "");
  CHECK(error_of([] { P::check("123U"); }) == "");
  CHECK(error_of([] { P::check("123ul"); }) == "");
  CHECK(error_of([] { P::check("123uLL"); }) == "");
  CHECK(error_of([] { P::check("123lu"); }) == "");
  CHECK(error_of([] { P::check("123LLU"); }) == "");
  CHECK(error_of([] { P::check("123lL"); }) == "invalid suffix 'lL' on integer constant '123lL'");
  CHECK(error_of([] { P::check("123wb"); }) == "");
  CHECK(error_of([] { P::check("123uwb"); }) == "");
  CHECK(error_of([] { P::check("123WBU"); }) == "");
  CHECK(error_of([] { P::check("123wB"); }) == "invalid suffix 'wB' on integer constant '123wB'");
  CHECK(error_of([] { P::check("123z"); }) == "invalid suffix 'z' on integer constant '123z'");
  CHECK(error_of([] { P::check("0b1.0"); }) == "a binary constant is an integer, so '0b1.0' can't have a period");
  CHECK(error_of([] { P::check("0b1."); }) == "a binary constant is an integer, so '0b1.' can't have a period");
  CHECK(error_of([] { P::check("0x1g"); }) == "invalid suffix 'g' on integer constant '0x1g'");
  CHECK(error_of([] { P::check("12ll"); }) == "");
  CHECK(error_of([] { P::check("12LL"); }) == "");
  CHECK(error_of([] { P::check("12L"); }) == "");
  CHECK(error_of([] { P::check("12l"); }) == "");
  CHECK(error_of([] { P::check("12Ul"); }) == "");
  CHECK(error_of([] { P::check("12uL"); }) == "");
  CHECK(error_of([] { P::check("12llu"); }) == "");
  CHECK(error_of([] { P::check("0xFFFFFFFFFFFFFFFF"); }) == "");
  CHECK(error_of([] { P::check("18446744073709551616"); }) == "");
  CHECK(error_of([] { P::check("99999999999999999999999999999999999999999"); }) == "");
  CHECK(error_of([] { P::check("170141183460469231731687303715884105727"); }) == "");
  CHECK(error_of([] { P::check("170141183460469231731687303715884105728"); }) == "");
  CHECK(error_of([] { P::check("0'"); }) == "a digit separator has to be between two digits, and the one in \"0'\" ends the number");
  CHECK(error_of([] { P::check("0x1'"); }) == "a digit separator has to be between two digits, and the one in \"0x1'\" ends the number");
  CHECK(error_of([] { P::check("1'_"); }) == "a digit separator has to be between two digits, and the one in \"1'_\" comes right before the suffix");
  CHECK(error_of([] { P::check("1.0"); }) == "");
  CHECK(error_of([] { P::check("1."); }) == "");
  CHECK(error_of([] { P::check(".5"); }) == "");
  CHECK(error_of([] { P::check("1e5"); }) == "");
  CHECK(error_of([] { P::check("1e+5"); }) == "");
  CHECK(error_of([] { P::check("1E-5"); }) == "");
  CHECK(error_of([] { P::check("1.5f"); }) == "");
  CHECK(error_of([] { P::check("1.5L"); }) == "");
  CHECK(error_of([] { P::check("1.5F"); }) == "");
  CHECK(error_of([] { P::check("1.5df"); }) == "the suffix 'df' of '1.5df' gives a decimal floating type, which c6502 doesn't have");
  CHECK(error_of([] { P::check("1.5DD"); }) == "the suffix 'DD' of '1.5DD' gives a decimal floating type, which c6502 doesn't have");
  CHECK(error_of([] { P::check("1.5x"); }) == "invalid suffix 'x' on floating constant '1.5x'");
  CHECK(error_of([] { P::check("0x1p3"); }) == "");
  CHECK(error_of([] { P::check("0x1.8p-2"); }) == "");
  CHECK(error_of([] { P::check("0x1.8"); }) == "hexadecimal floating constant '0x1.8' has no binary exponent (a p and a power of 2)");
  CHECK(error_of([] { P::check("0x.p1"); }) == "floating constant '0x.p1' has no digits before or after the period");
  CHECK(error_of([] { P::check("1'.5"); }) == "a digit separator has to be between two digits, and the one in \"1'.5\" touches the period");
  CHECK(error_of([] { P::check("1.'5"); }) == "a digit separator has to be between two digits, and the one in \"1.'5\" touches the period");
  CHECK(error_of([] { P::check("1'e5"); }) == "a digit separator has to be between two digits, and the one in \"1'e5\" comes right before the exponent");
  CHECK(error_of([] { P::check("1e'5"); }) == "a digit separator has to be between two digits, and the one in \"1e'5\" comes right after the exponent's letter or sign");
  CHECK(error_of([] { P::check("1e+'5"); }) == "a digit separator has to be between two digits, and the one in \"1e+'5\" comes right after the exponent's letter or sign");
  CHECK(error_of([] { P::check("1e"); }) == "the exponent of '1e' has no digits");
  CHECK(error_of([] { P::check("1.5e"); }) == "the exponent of '1.5e' has no digits");
  CHECK(error_of([] { P::check("0x1p"); }) == "the exponent of '0x1p' has no digits");
  CHECK(error_of([] { P::check("0x'1p3"); }) == "a digit separator has to be between two digits, and the one in \"0x'1p3\" comes right after the prefix 0x");
  CHECK(error_of([] { P::check("1'000.5e1'0"); }) == "");
  CHECK(error_of([] { P::check("0x1.ep3"); }) == "");
  CHECK(error_of([] { P::check("0xe'1p3"); }) == "");
  CHECK(error_of([] { P::check("0x1p'3"); }) == "a digit separator has to be between two digits, and the one in \"0x1p'3\" comes right after the exponent's letter or sign");
  CHECK(error_of([] { P::check("..5"); }) == "floating constant '..5' has no digits before or after the period");
  CHECK(error_of([] { P::check("1.e5f"); }) == "");
  CHECK(error_of([] { P::check("0x1.8p3L"); }) == "");
  CHECK(error_of([] { P::check("1e5ll"); }) == "invalid suffix 'll' on floating constant '1e5ll'");
  CHECK(error_of([] { P::check("0x1e'p3"); }) == "a digit separator has to be between two digits, and the one in \"0x1e'p3\" comes right before the exponent");
}

TEST_CASE("bit_precise_width gives Python's widths and messages") {
  CHECK(P::bit_precise_width(P::parse_integer("0wb"), "0wb") == 2);
  CHECK(P::bit_precise_width(P::parse_integer("0uwb"), "0uwb") == 1);
  CHECK(P::bit_precise_width(P::parse_integer("1wb"), "1wb") == 2);
  CHECK(P::bit_precise_width(P::parse_integer("1uwb"), "1uwb") == 1);
  CHECK(P::bit_precise_width(P::parse_integer("3wb"), "3wb") == 3);
  CHECK(P::bit_precise_width(P::parse_integer("3uwb"), "3uwb") == 2);
  CHECK(P::bit_precise_width(P::parse_integer("127wb"), "127wb") == 8);
  CHECK(P::bit_precise_width(P::parse_integer("128wb"), "128wb") == 9);
  CHECK(P::bit_precise_width(P::parse_integer("255uwb"), "255uwb") == 8);
  CHECK(P::bit_precise_width(P::parse_integer("0x7FFFFFFFFFFFFFFFwb"), "0x7FFFFFFFFFFFFFFFwb") == 64);
  CHECK(error_of([] { P::bit_precise_width(P::parse_integer("0x8000000000000000wb"), "0x8000000000000000wb"); }) ==
        "integer constant '0x8000000000000000wb' needs a _BitInt(65), and BITINT_MAXWIDTH is 64 (C23 \xc2""\xa7""6.4.4.2p6)");
  CHECK(P::bit_precise_width(P::parse_integer("0xFFFFFFFFFFFFFFFFuwb"), "0xFFFFFFFFFFFFFFFFuwb") == 64);
  CHECK(error_of([] { P::bit_precise_width(P::parse_integer("0x10000000000000000uwb"), "0x10000000000000000uwb"); }) ==
        "integer constant '0x10000000000000000uwb' needs a unsigned _BitInt(65), and BITINT_MAXWIDTH is 64 (C23 \xc2""\xa7""6.4.4.2p6)");
  CHECK(error_of([] { P::bit_precise_width(P::parse_integer("99999999999999999999999999999999999999999wb"), "99999999999999999999999999999999999999999wb"); }) ==
        "integer constant '99999999999999999999999999999999999999999wb' needs a _BitInt(138), and BITINT_MAXWIDTH is 64 (C23 \xc2""\xa7""6.4.4.2p6)");
}

TEST_CASE("pp_number_at finds Python's pp-numbers") {
  CHECK(P::pp_number_at("123abc+", 0) == "123abc");
  CHECK(P::pp_number_at("123abc+", 1) == "23abc");
  CHECK(P::pp_number_at("123abc+", 2) == "3abc");
  CHECK(P::pp_number_at("123abc+", 3) == "");
  CHECK(P::pp_number_at("123abc+", 4) == "");
  CHECK(P::pp_number_at("123abc+", 5) == "");
  CHECK(P::pp_number_at("123abc+", 6) == "");
  CHECK(P::pp_number_at("1e+5x", 0) == "1e+5x");
  CHECK(P::pp_number_at("1e+5x", 1) == "");
  CHECK(P::pp_number_at("1e+5x", 2) == "");
  CHECK(P::pp_number_at("1e+5x", 3) == "5x");
  CHECK(P::pp_number_at("1e+5x", 4) == "");
  CHECK(P::pp_number_at("1'000", 0) == "1'000");
  CHECK(P::pp_number_at("1'000", 1) == "");
  CHECK(P::pp_number_at("1'000", 2) == "000");
  CHECK(P::pp_number_at("1'000", 3) == "00");
  CHECK(P::pp_number_at("1'000", 4) == "0");
  CHECK(P::pp_number_at("1'", 0) == "1");
  CHECK(P::pp_number_at("1'", 1) == "");
  CHECK(P::pp_number_at(".5;", 0) == ".5");
  CHECK(P::pp_number_at(".5;", 1) == "5");
  CHECK(P::pp_number_at(".5;", 2) == "");
  CHECK(P::pp_number_at("a1", 0) == "");
  CHECK(P::pp_number_at("a1", 1) == "1");
  CHECK(P::pp_number_at(".a", 0) == "");
  CHECK(P::pp_number_at(".a", 1) == "");
  CHECK(P::pp_number_at("0x1p-3;", 0) == "0x1p-3");
  CHECK(P::pp_number_at("0x1p-3;", 1) == "");
  CHECK(P::pp_number_at("0x1p-3;", 2) == "1p-3");
  CHECK(P::pp_number_at("0x1p-3;", 3) == "");
  CHECK(P::pp_number_at("0x1p-3;", 4) == "");
  CHECK(P::pp_number_at("0x1p-3;", 5) == "3");
  CHECK(P::pp_number_at("0x1p-3;", 6) == "");
  CHECK(P::pp_number_at("1..2", 0) == "1..2");
  CHECK(P::pp_number_at("1..2", 1) == "");
  CHECK(P::pp_number_at("1..2", 2) == ".2");
  CHECK(P::pp_number_at("1..2", 3) == "2");
  CHECK(P::pp_number_at("1'''", 0) == "1");
  CHECK(P::pp_number_at("1'''", 1) == "");
  CHECK(P::pp_number_at("1'''", 2) == "");
  CHECK(P::pp_number_at("1'''", 3) == "");
  CHECK(P::pp_number_at("1'a'b", 0) == "1'a'b");
  CHECK(P::pp_number_at("1'a'b", 1) == "");
  CHECK(P::pp_number_at("1'a'b", 2) == "");
  CHECK(P::pp_number_at("1'a'b", 3) == "");
  CHECK(P::pp_number_at("1'a'b", 4) == "");
  CHECK(P::pp_number_at("9_z.q", 0) == "9_z.q");
  CHECK(P::pp_number_at("9_z.q", 1) == "");
  CHECK(P::pp_number_at("9_z.q", 2) == "");
  CHECK(P::pp_number_at("9_z.q", 3) == "");
  CHECK(P::pp_number_at("9_z.q", 4) == "");
  CHECK(P::pp_number_at("1e", 0) == "1e");
  CHECK(P::pp_number_at("1e", 1) == "");
  CHECK(P::pp_number_at("1e-", 0) == "1e-");
  CHECK(P::pp_number_at("1e-", 1) == "");
  CHECK(P::pp_number_at("1e-", 2) == "");
  CHECK(P::pp_number_at("1p+", 0) == "1p+");
  CHECK(P::pp_number_at("1p+", 1) == "");
  CHECK(P::pp_number_at("1p+", 2) == "");
  CHECK(P::pp_number_at("0 1", 0) == "0");
  CHECK(P::pp_number_at("0 1", 1) == "");
  CHECK(P::pp_number_at("0 1", 2) == "1");
  CHECK(P::pp_number_at("x 12", 0) == "");
  CHECK(P::pp_number_at("x 12", 1) == "");
  CHECK(P::pp_number_at("x 12", 2) == "12");
  CHECK(P::pp_number_at("x 12", 3) == "2");
  CHECK(P::pp_number_at("1'\xc3""\xa9""", 0) == "1");
  CHECK(P::pp_number_at("1'\xc3""\xa9""", 1) == "");
  CHECK(P::pp_number_at("7\xc3""\xa9""", 0) == "7");
}
