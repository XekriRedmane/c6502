#include "fuzz/calls.h"

#include <algorithm>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "fuzz/bit_precise_types.h"
#include "py_random.h"

namespace c6502::fuzz::calls {

namespace {

const std::vector<std::string> kTypes = {"signed char", "unsigned char", "int",
                                         "unsigned int", "long",          "unsigned long"};
const std::map<std::string, int> kSize = {{"signed char", 1}, {"unsigned char", 1},
                                          {"int", 2},         {"unsigned int", 2},
                                          {"long", 4},        {"unsigned long", 4}};

constexpr const char* kPointer = "int *";

constexpr const char* kRec =
    "int rec(int d, int x) { if (d <= 0) return x & 255; "
    "return rec(d - 1, (x * 3 + d) & 1023) + 1; }";
constexpr const char* kRecSaved =
    "int rec(int d, int x) {\n"
    "    unsigned char a = (unsigned char)(x * 7);\n"
    "    long b = (long)x * 3;\n"
    "    if (d <= 0) return x & 255;\n"
    "    [[c6502::save_hardware_stack]] int r = rec(d - 1, (x * 3 + d) & 1023);\n"
    "    return (r + a + (int)(b & 15)) & 1023;\n"
    "}";

struct Param {
  std::string type;
  std::string name;
};

struct Signature {
  std::string ret;
  std::vector<Param> params;
};

std::string join(const std::vector<std::string>& items, const std::string& sep) {
  std::string out;
  for (std::size_t i = 0; i < items.size(); ++i) {
    if (i != 0) out += sep;
    out += items[i];
  }
  return out;
}

bool has_pointer(const std::vector<Param>& params) {
  return std::any_of(params.begin(), params.end(), [](const Param& p) { return p.type == kPointer; });
}

class Generator {
 public:
  Generator(py_random::Random& rng, std::vector<std::string> types)
      : rng_(rng), types_(std::move(types)) {}

  std::string expr(const std::vector<std::string>& vars, int depth) {
    if (depth == 0 || rng_.random() < 0.3) {
      if (!vars.empty() && rng_.random() < 0.8) return rng_.choice(vars);
      static const std::vector<std::string> kConstants = {"0",   "1",   "2",    "3",    "7",
                                                          "100", "255", "1000", "32767"};
      return rng_.choice(kConstants);
    }
    const double k = rng_.random();
    const std::string a = expr(vars, depth - 1);
    if (k < 0.1) return "((" + rng_.choice(types_) + ")(" + a + "))";
    if (k < 0.17) {
      static const std::vector<std::string> kUnary = {"-", "~", "!"};
      return "(" + rng_.choice(kUnary) + a + ")";
    }
    const std::string b = expr(vars, depth - 1);
    static const std::vector<std::string> kBinary = {"+",  "-",  "*", "/", "%",  "&",  "|",
                                                     "^",  "<<", ">>", "<", ">", "<=", ">=",
                                                     "==", "!=", "&&", "||"};
    const std::string& op = rng_.choice(kBinary);
    if (op == "/" || op == "%") return "((" + a + ") " + op + " ((" + b + ") | 1))";
    if (op == "<<" || op == ">>") return "((" + a + ") " + op + " ((" + b + ") & 7))";
    return "((" + a + ") " + op + " (" + b + "))";
  }

  const std::string& type() { return rng_.choice(types_); }

 private:
  py_random::Random& rng_;
  std::vector<std::string> types_;
};

std::vector<std::string> locals_of(const std::vector<std::string>& vars) {
  std::vector<std::string> out;
  for (const std::string& v : vars) {
    if (v.rfind('l', 0) == 0) out.push_back(v);
  }
  return out;
}

// Returns the functions that abi_selection leaves on the soft stack
// under -O, where function i calls the functions `calls[i]`, and `rec`
// when `calls_rec[i]`. The last function is called through a pointer
// when `use_ptr_fn`, and `rec` has a private pool when `rec_has_pool`.
std::set<int> soft_stack_functions(const std::vector<std::vector<int>>& calls,
                                   const std::vector<bool>& calls_rec, bool use_ptr_fn,
                                   bool rec_has_pool) {
  const int n = static_cast<int>(calls.size());
  std::set<int> soft;
  auto calls_to = [&](int i, int j) {
    return std::find(calls[i].begin(), calls[i].end(), j) != calls[i].end();
  };
  if (use_ptr_fn) {
    soft.insert(n - 1);
    for (int i = 0; i < n; ++i) {
      if (calls_to(i, n - 1)) soft.insert(i);
    }
  }
  if (!rec_has_pool) {
    for (int i = 0; i < n; ++i) {
      if (calls_rec[i]) soft.insert(i);
    }
  }
  for (int i = n - 1; i >= 0; --i) {
    for (int j : calls[i]) {
      if (soft.count(j) != 0) {
        soft.insert(i);
        break;
      }
    }
  }
  return soft;
}

}  // namespace

GeneratedProgram make_program(Int seed) {
  py_random::Random rng(seed);
  // The carry-flag choices come from a stream of their own, so the rest
  // of each seed's program is what it was before they existed.
  py_random::Random crng(seed * 7919 + 1);
  // So does the choice of recursive function.
  const bool saved_rec = py_random::Random(seed * 104729 + 3).random() < 0.5;
  std::vector<std::string> types = kTypes;
  std::map<std::string, int> size = kSize;
  TypeTable bp_types;
  for (const std::string& key : choose_bit_precise(bp_types, seed, 2)) {
    const IntType& t = bp_types.at(key);
    types.push_back(t.spelling);
    size[t.spelling] = (t.width + 7) / 8;
  }
  Generator gen(rng, types);
  const int n = static_cast<int>(rng.randint(3, 6));
  std::vector<std::string> out;
  std::vector<std::string> globals;
  const int n_globals = static_cast<int>(rng.randint(1, 3));
  for (int i = 0; i < n_globals; ++i) {
    const std::string g = "g" + std::to_string(i);
    globals.push_back(g);
    const std::string t = gen.type();
    out.push_back(t + " " + g + " = " + decimal(rng.randint(0, 200)) + ";");
  }
  const bool use_rec = rng.random() < 0.4;
  const bool use_ptr_fn = rng.random() < 0.4;

  std::vector<Signature> sigs(n);
  for (int i = 0; i < n; ++i) {
    sigs[i].ret = gen.type();
    const int n_params = static_cast<int>(rng.randint(0, 4));
    for (int k = 0; k < n_params; ++k) sigs[i].params.push_back({gen.type(), "p" + std::to_string(k)});
    if (rng.random() < 0.25) sigs[i].params.push_back({kPointer, "pp"});
  }
  std::vector<std::vector<int>> calls(n);
  for (int i = 0; i < n; ++i) {
    std::vector<int> callees;
    for (int j = i + 1; j < n; ++j) callees.push_back(j);
    rng.shuffle(callees);
    const auto keep = static_cast<std::size_t>(rng.randint(0, 2));
    callees.resize(std::min(keep, callees.size()));
    calls[i] = callees;
  }
  std::vector<bool> calls_rec(n);
  for (int i = 0; i < n; ++i) calls_rec[i] = use_rec && rng.random() < 0.5;
  std::set<int> carry;
  for (int i = 0; i < n; ++i) {
    if (crng.random() < 0.3 && !(use_ptr_fn && i == n - 1)) carry.insert(i);
  }
  for (int i : carry) sigs[i].ret = "bool";
  py_random::Random arng(seed * 15485863 + 5);
  std::set<int> marked;
  for (int i : carry) {
    if (arng.random() < 0.5) marked.insert(i);
  }

  const std::set<int> soft = soft_stack_functions(calls, calls_rec, use_ptr_fn, saved_rec);
  auto reg_ok = [&](int i) { return soft.count(i) == 0; };

  std::vector<std::string> lines;
  if (use_rec) lines.push_back(saved_rec ? kRecSaved : kRec);
  for (int i = n - 1; i >= 0; --i) {
    const std::vector<Param>& params = sigs[i].params;
    std::vector<std::string> decl;
    int reg_bytes = 0;
    for (const Param& p : params) {
      std::string q;
      if (reg_ok(i) && p.type != kPointer && reg_bytes + size.at(p.type) <= 2 &&
          rng.random() < 0.4) {
        q = "register ";
        reg_bytes += size.at(p.type);
      }
      decl.push_back(q + p.type + " " + p.name);
    }
    std::vector<std::string> body;
    std::vector<std::string> vars;
    for (const Param& p : params) {
      if (p.type != kPointer) vars.push_back(p.name);
    }
    vars.insert(vars.end(), globals.begin(), globals.end());
    const int n_locals = static_cast<int>(rng.randint(1, 4));
    for (int k = 0; k < n_locals; ++k) {
      const std::string t = gen.type();
      body.push_back("    " + t + " l" + std::to_string(k) + " = " + gen.expr(vars, 2) + ";");
      vars.push_back("l" + std::to_string(k));
    }
    if (reg_ok(i) && rng.random() < 0.3) {
      body.push_back("    register unsigned char ry = (unsigned char)(l0);");
      vars.push_back("ry");
    }
    if (has_pointer(params)) {
      body.push_back("    *pp += (int)(" + gen.expr(vars, 1) + ");");
      vars.push_back("(*pp)");
    }
    for (int j : calls[i]) {
      const std::string fj = "f" + std::to_string(j);
      const std::vector<Param>& cparams = sigs[j].params;
      const bool needs_box = has_pointer(cparams);
      const std::string box = "box" + std::to_string(j);
      if (needs_box) body.push_back("    int " + box + " = (int)(l0);");
      std::vector<std::string> args;
      for (const Param& p : cparams) args.push_back(p.type == kPointer ? "&" + box : gen.expr(vars, 1));
      std::string call = fj + "(" + join(args, ", ") + ")";
      if (use_ptr_fn && j == n - 1) {
        std::vector<std::string> ptypes;
        for (const Param& p : cparams) ptypes.push_back(p.type);
        body.push_back("    " + sigs[j].ret + " (*fp)(" + join(ptypes, ", ") + ") = " + fj + ";");
        call = "fp(" + join(args, ", ") + ")";
      }
      const std::string dst = rng.choice(locals_of(vars));
      if (carry.count(j) != 0 && crng.random() < 0.5) {
        body.push_back("    if (" + call + ") " + dst + " += 3; else " + dst + " ^= 5;");
      } else if (rng.random() < 0.3) {
        body.push_back("    for (int it = 0; it < 3; it++) " + dst + " += (" + call + ") + it;");
      } else {
        body.push_back("    " + dst + " ^= " + call + ";");
      }
      if (needs_box) body.push_back("    " + dst + " += " + box + ";");
      if (!globals.empty() && rng.random() < 0.5) {
        body.push_back("    " + rng.choice(globals) + " += " + dst + ";");
      }
    }
    if (calls_rec[i]) {
      const std::string dst = rng.choice(locals_of(vars));
      body.push_back("    " + dst + " += rec(3, (int)(" + gen.expr(vars, 1) + "));");
    }
    body.push_back("    return " + gen.expr(vars, 2) + ";");
    const std::string attr = marked.count(i) != 0 ? "[[c6502::return_bool_in_carry]] " : "";
    const std::string params_text = decl.empty() ? "void" : join(decl, ", ");
    lines.push_back(attr + sigs[i].ret + " f" + std::to_string(i) + "(" + params_text + ") {");
    lines.insert(lines.end(), body.begin(), body.end());
    lines.push_back("}");
  }

  std::vector<std::string> pre;
  std::vector<std::string> margs;
  for (const Param& p : sigs[0].params) {
    if (p.type == kPointer) {
      pre.push_back("    int mbox = 5;");
      margs.push_back("&mbox");
    } else {
      margs.push_back(decimal(rng.randint(0, 300)));
    }
  }
  std::vector<std::string> mix;
  for (std::size_t k = 0; k < globals.size(); ++k) mix.push_back("(long)g" + std::to_string(k));
  std::vector<std::string> all = out;
  all.insert(all.end(), lines.begin(), lines.end());
  all.push_back("int main(void) {");
  all.insert(all.end(), pre.begin(), pre.end());
  all.push_back("    long r = (long)f0(" + join(margs, ", ") + ");");
  all.push_back("    r ^= " + join(mix, " ^ ") + ";");
  all.push_back("    return (int)(r ^ (r >> 16));");
  all.push_back("}");
  return {join(all, "\n") + "\n", std::nullopt};
}

}  // namespace c6502::fuzz::calls
