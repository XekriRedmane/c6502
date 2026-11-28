#include "fuzz/exprs.h"

#include <string>
#include <utility>
#include <vector>

#include "fuzz/bit_precise_types.h"
#include "py_random.h"

namespace c6502::fuzz::exprs {

namespace {

// Raised when the expression being generated would be undefined.
struct UndefinedBehavior {};

const std::vector<std::string> kStandardKeys = {"schar", "uchar", "int", "uint", "long", "ulong"};

TypeTable standard_types() {
  return {
      {"schar", {"signed char", 8, true}}, {"uchar", {"unsigned char", 8, false}},
      {"int", {"int", 16, true}},          {"uint", {"unsigned int", 16, false}},
      {"long", {"long", 32, true}},        {"ulong", {"unsigned long", 32, false}},
  };
}

const RankTable kRank = {{"int", 1}, {"uint", 1}, {"long", 2}, {"ulong", 2}};
const UnsignedOf kUnsignedOf = {{"int", "uint"}, {"long", "ulong"}};

// The constants a leaf draws from, before the random bits of its type.
const Int kLeafConstants[] = {0,     1,     2,      7,          127,       128,
                              255,   256,   0x7FFF, 0x8000,     0xFFFF,    0x12345,
                              Int(0x7FFFFFFF), Int(0x80000000LL)};

struct Var {
  std::string name;
  std::string type;
  Int value;
};

struct Expr {
  std::string text;
  Int value;
  std::string type;
};

class Generator {
 public:
  Generator(py_random::Random& rng, Int seed) : rng_(rng), types_(standard_types()) {
    keys_ = kStandardKeys;
    for (std::string& k : choose_bit_precise(types_, seed, 3)) keys_.push_back(std::move(k));
  }

  GeneratedProgram program(int n_exprs, int depth) {
    for (int i = 0; i < 6; ++i) {
      const std::string t = pick_key();
      const int w = type(t).width;
      const Int random_bits = rng_.getrandbits(w);
      const Int low_bits = rng_.getrandbits(4);
      const Int choices[] = {random_bits, low_bits, Int(1) << (w - 1), (Int(1) << w) - 1, 1, 0};
      vars_.push_back({"v" + std::to_string(i), t, wrap(choices[rng_.choice_index(6)], type(t))});
    }
    std::vector<Expr> exprs;
    while (static_cast<int>(exprs.size()) < n_exprs) {
      try {
        exprs.push_back(gen(depth));
      } catch (const UndefinedBehavior&) {
      }
    }
    const bool use_params = rng_.random() < 0.5;
    std::string out;
    if (use_params) {
      std::string params;
      for (const Var& v : vars_) {
        if (!params.empty()) params += ", ";
        params += type(v.type).spelling + " " + v.name;
      }
      out += "int check(" + params + ") {\n";
    } else {
      for (const Var& v : vars_) {
        out += type(v.type).spelling + " " + v.name + " = " + lit(v.value, v.type) + ";\n";
      }
      out += "int check(void) {\n";
    }
    out += "    int fail = 0;\n";
    for (std::size_t i = 0; i < exprs.size(); ++i) {
      const Expr& e = exprs[i];
      out += "    { " + type(e.type).spelling + " r = " + e.text + "; if (r != " +
             lit(e.value, e.type) + ") fail |= " + std::to_string(1 << i) + "; }\n";
    }
    out += "    return fail;\n}\n";
    if (use_params) {
      std::string args;
      for (const Var& v : vars_) {
        if (!args.empty()) args += ", ";
        args += lit(v.value, v.type);
      }
      out += "int main(void) { return check(" + args + "); }\n";
    } else {
      out += "int main(void) { return check(); }\n";
    }
    return {out, 0};
  }

 private:
  const IntType& type(const std::string& t) const { return types_.find(t)->second; }

  const std::string& pick_key() { return keys_[rng_.choice_index(keys_.size())]; }

  bool in_range(Int v, const std::string& t) const { return wrap(v, type(t)) == v; }

  // C99 §6.3.1.1.2's integer promotion. With a 16-bit int, both char
  // types promote to int, and a bit-precise type stays as it is.
  static std::string promote(const std::string& t) {
    return t == "schar" || t == "uchar" ? "int" : t;
  }

  // C99 §6.3.1.8's usual arithmetic conversions on two integer types.
  std::string common(const std::string& a0, const std::string& b0) {
    const std::string a = promote(a0);
    const std::string b = promote(b0);
    if (a == b) return a;
    if (is_bit_precise(a) || is_bit_precise(b)) {
      return common_bit_precise(types_, kRank, kUnsignedOf, a, b);
    }
    const bool sa = type(a).is_signed;
    const bool sb = type(b).is_signed;
    if (sa == sb) return kRank.at(a) >= kRank.at(b) ? a : b;
    const std::string& u = sa ? b : a;
    const std::string& s = sa ? a : b;
    if (kRank.at(u) >= kRank.at(s)) return u;
    if (type(s).width > type(u).width) return s;
    return kUnsignedOf.at(s);
  }

  // Returns a C expression of type `t` with value `v`. A negative value
  // is written as a cast of the unsigned bit pattern, which is a plain
  // integer constant expression.
  std::string lit(Int v, const std::string& t) const {
    const IntType& it = type(t);
    if (is_bit_precise(t)) return bit_precise_literal(types_, v, t);
    if (t == "schar" || t == "uchar") return "((" + it.spelling + ")" + decimal(v & 0xFF) + ")";
    if (v < 0) {
      return "((" + it.spelling + ")" + decimal(v & ((Int(1) << it.width) - 1)) + "UL)";
    }
    const char* suffix = t == "int" ? "" : t == "uint" ? "u" : t == "long" ? "L" : "UL";
    return "((" + it.spelling + ")" + decimal(v) + suffix + ")";
  }

  // Returns a random expression of the types `keys_`, and throws
  // `UndefinedBehavior` when it would be undefined.
  Expr gen(int depth) {
    if (depth == 0 || rng_.random() < 0.25) {
      if (rng_.random() < 0.7) {
        const Var& v = vars_[rng_.choice_index(vars_.size())];
        return {v.name, v.value, v.type};
      }
      const std::string t = pick_key();
      const Int random_bits = rng_.getrandbits(type(t).width);
      const std::size_t i = rng_.choice_index(std::size(kLeafConstants) + 1);
      const Int v = wrap(i < std::size(kLeafConstants) ? kLeafConstants[i] : random_bits, type(t));
      return {lit(v, t), v, t};
    }
    const double kind = rng_.random();
    if (kind < 0.12) {
      const Expr a = gen(depth - 1);
      const std::string t = pick_key();
      return {"((" + type(t).spelling + ")(" + a.text + "))", wrap(a.value, type(t)), t};
    }
    if (kind < 0.2) {
      const Expr a = gen(depth - 1);
      static const std::vector<std::string> kUnary = {"-", "~", "!"};
      const std::string& op = rng_.choice(kUnary);
      if (op == "!") return {"(!" + a.text + ")", Int(a.value == 0 ? 1 : 0), "int"};
      const std::string pt = promote(a.type);
      const Int r = op == "-" ? -a.value : ~a.value;
      if (type(pt).is_signed && !in_range(r, pt)) throw UndefinedBehavior{};
      return {"(" + op + a.text + ")", wrap(r, type(pt)), pt};
    }
    const Expr a = gen(depth - 1);
    const Expr b = gen(depth - 1);
    static const std::vector<std::string> kBinary = {"+",  "-",  "*", "/", "%",  "&",  "|",
                                                     "^",  "<<", ">>", "<", ">", "<=", ">=",
                                                     "==", "!=", "&&", "||"};
    const std::string& op = rng_.choice(kBinary);
    const std::string text = "(" + a.text + " " + op + " " + b.text + ")";
    if (op == "&&") return {text, Int(a.value != 0 && b.value != 0 ? 1 : 0), "int"};
    if (op == "||") return {text, Int(a.value != 0 || b.value != 0 ? 1 : 0), "int"};
    if (op == "<<" || op == ">>") {
      // C99 §6.5.7: each operand promotes on its own, and the result has
      // the promoted left operand's type.
      const std::string pt = promote(a.type);
      if (b.value < 0 || b.value >= type(pt).width) throw UndefinedBehavior{};
      const int k = static_cast<int>(b.value);
      if (op == ">>") return {text, a.value >> k, pt};
      if (a.value < 0) throw UndefinedBehavior{};
      const Int r = a.value << k;
      if (type(pt).is_signed && !in_range(r, pt)) throw UndefinedBehavior{};
      return {text, wrap(r, type(pt)), pt};
    }
    const std::string ct = common(a.type, b.type);
    const Int x = wrap(a.value, type(ct));
    const Int y = wrap(b.value, type(ct));
    if (op == "<") return {text, Int(x < y ? 1 : 0), "int"};
    if (op == ">") return {text, Int(x > y ? 1 : 0), "int"};
    if (op == "<=") return {text, Int(x <= y ? 1 : 0), "int"};
    if (op == ">=") return {text, Int(x >= y ? 1 : 0), "int"};
    if (op == "==") return {text, Int(x == y ? 1 : 0), "int"};
    if (op == "!=") return {text, Int(x != y ? 1 : 0), "int"};
    Int r;
    if (op == "/" || op == "%") {
      if (y == 0) throw UndefinedBehavior{};
      // C99 §6.5.5.6: the quotient truncates toward zero.
      Int q = (x < 0 ? -x : x) / (y < 0 ? -y : y);
      if ((x < 0) != (y < 0)) q = -q;
      r = op == "/" ? q : x - q * y;
    } else if (op == "+") {
      r = x + y;
    } else if (op == "-") {
      r = x - y;
    } else if (op == "*") {
      r = wrapping_mul(x, y);
    } else if (op == "&") {
      r = x & y;
    } else if (op == "|") {
      r = x | y;
    } else {
      r = x ^ y;
    }
    if (type(ct).is_signed && !in_range(r, ct)) throw UndefinedBehavior{};
    return {text, wrap(r, type(ct)), ct};
  }

  py_random::Random& rng_;
  TypeTable types_;
  std::vector<std::string> keys_;
  std::vector<Var> vars_;
};

}  // namespace

GeneratedProgram make_program(Int seed, int n_exprs, int depth) {
  py_random::Random rng(seed);
  Generator g(rng, seed);
  return g.program(n_exprs, depth);
}

}  // namespace c6502::fuzz::exprs
