// CPython's `random.Random` in C++ (src/py_random.h) against CPython
// 3.11's own results, which tools/py_random_vectors.py froze in
// cpp/tests/frozen/py_random.txt.
#include <cstdint>
#include <cstring>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

#include "compile_case.h"
#include "doctest.h"
#include "json.h"
#include "py_int.h"
#include "py_random.h"

namespace {

using c6502::asdl::Int;
using c6502::py_random::Random;

Int number(const std::string& text) { return c6502::py_int::parse(text, 10, "py_random.txt"); }

std::vector<std::size_t> indices(std::istringstream& in) {
  std::vector<std::size_t> out;
  std::string word;
  while (in >> word) {
    if (word != "-") out.push_back(static_cast<std::size_t>(std::stoull(word)));
  }
  return out;
}

std::string hex_bits(double d) {
  std::uint64_t bits = 0;
  std::memcpy(&bits, &d, sizeof bits);
  char buf[17];
  std::snprintf(buf, sizeof buf, "%016llx", static_cast<unsigned long long>(bits));
  return buf;
}

}  // namespace

TEST_CASE("py_random: every call matches CPython 3.11 on every frozen seed") {
  const std::string text = c6502::suite::read_repo_file("cpp/tests/frozen/py_random.txt");
  std::istringstream lines(text);
  std::optional<Random> rng;
  std::string line;
  int seeds = 0;
  int calls = 0;
  while (std::getline(lines, line)) {
    INFO(line);
    std::istringstream in(line);
    std::string op;
    in >> op;
    if (op == "seed") {
      std::string kind;
      in >> kind;
      std::string rest;
      std::getline(in, rest);
      rest = rest.substr(1);
      if (kind == "int") {
        rng.emplace(number(rest));
      } else {
        rng.emplace(c6502::json::parse(rest).string);
      }
      ++seeds;
      continue;
    }
    REQUIRE(rng.has_value());
    ++calls;
    std::string a, b, c;
    if (op == "genrand") {
      in >> a;
      CHECK(Int(rng->genrand_uint32()) == number(a));
    } else if (op == "random") {
      in >> a;
      CHECK(hex_bits(rng->random()) == a);
    } else if (op == "getrandbits") {
      in >> a >> b;
      CHECK(rng->getrandbits(std::stoi(a)) == number(b));
    } else if (op == "randbelow") {
      in >> a >> b;
      CHECK(rng->randbelow(number(a)) == number(b));
    } else if (op == "randrange") {
      in >> a >> b;
      CHECK(rng->randrange(number(a)) == number(b));
    } else if (op == "randrange2") {
      in >> a >> b >> c;
      CHECK(rng->randrange(number(a), number(b)) == number(c));
    } else if (op == "randint") {
      in >> a >> b >> c;
      CHECK(rng->randint(number(a), number(b)) == number(c));
    } else if (op == "choice") {
      in >> a >> b;
      std::vector<int> seq(std::stoul(a));
      for (std::size_t i = 0; i < seq.size(); ++i) seq[i] = static_cast<int>(i);
      CHECK(rng->choice(seq) == std::stoi(b));
    } else if (op == "shuffle") {
      in >> a;
      std::vector<std::size_t> seq(std::stoul(a));
      for (std::size_t i = 0; i < seq.size(); ++i) seq[i] = i;
      rng->shuffle(seq);
      CHECK(seq == indices(in));
    } else if (op == "choices") {
      in >> a >> b;
      std::vector<double> weights;
      std::istringstream ws(b);
      std::string w;
      while (std::getline(ws, w, ',')) weights.push_back(std::stod(w));
      CHECK(rng->choices_indices(weights, std::stoul(a)) == indices(in));
    } else if (op == "choicesn") {
      in >> a >> b;
      CHECK(rng->choices_indices(std::stoul(a), std::stoul(b)) == indices(in));
    } else if (op == "sample") {
      in >> a >> b;
      CHECK(rng->sample_indices(std::stoul(a), std::stoul(b)) == indices(in));
    } else {
      FAIL("unknown call " << op);
    }
  }
  CHECK(seeds == 28);
  CHECK(calls > 2000);
}

TEST_CASE("py_random: the errors of empty ranges and choices") {
  Random rng(1);
  CHECK_THROWS(rng.randbelow(0));
  CHECK_THROWS(rng.randrange(0));
  CHECK_THROWS(rng.randrange(3, 3));
  CHECK_THROWS(rng.choice(std::vector<int>{}));
  CHECK_THROWS(rng.choices_indices(std::vector<double>{0, 0}, 1));
  CHECK_THROWS(rng.sample_indices(2, 3));
  CHECK_THROWS(rng.getrandbits(128));
}
