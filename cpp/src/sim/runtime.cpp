#include "sim/runtime.h"

#include <optional>

#include "asdl/runtime.h"
#include "fp_arith.h"
#include "rtlib/helpers.h"
#include "sim/assembler.h"

namespace c6502::sim {

namespace {

using Memory = std::vector<std::uint8_t>;
using asdl::Int;

std::uint64_t read_int(const Memory& mem, unsigned addr, int nbytes) {
  std::uint64_t v = 0;
  for (int i = 0; i < nbytes; ++i) v |= std::uint64_t{mem[(addr + i) & 0xFFFF]} << (8 * i);
  return v;
}

void write_int(Memory& mem, unsigned addr, std::uint64_t v, int nbytes) {
  for (int i = 0; i < nbytes; ++i) mem[(addr + i) & 0xFFFF] = static_cast<std::uint8_t>(v >> (8 * i));
}

std::uint64_t mask(int nbytes) { return nbytes >= 8 ? ~std::uint64_t{0} : (std::uint64_t{1} << (8 * nbytes)) - 1; }

std::int64_t sign_extend(std::uint64_t v, int nbytes) {
  if (nbytes >= 8) return static_cast<std::int64_t>(v);
  const std::uint64_t sign = std::uint64_t{1} << (8 * nbytes - 1);
  return static_cast<std::int64_t>((v ^ sign) - sign);
}

// --- The integer hooks ---

Hook mul_hook(std::uint16_t hargs, int n) {
  return [hargs, n](Memory& mem) {
    const std::uint64_t a = read_int(mem, hargs, n);
    const std::uint64_t b = read_int(mem, hargs + n, n);
    write_int(mem, hargs + 2 * n, a * b, n);
  };
}

Hook udivmod_hook(std::uint16_t hargs, int n) {
  return [hargs, n](Memory& mem) {
    const std::uint64_t a = read_int(mem, hargs, n);
    const std::uint64_t b = read_int(mem, hargs + n, n);
    if (b == 0) throw HookError("RuntimeError", "udivmod" + std::to_string(8 * n) + ": division by zero");
    write_int(mem, hargs + 2 * n, a / b, n);
    write_int(mem, hargs + 3 * n, a % b, n);
  };
}

// The quotient truncates toward zero and the remainder takes the
// dividend's sign (C99 §6.5.5.6), modulo 2^(8n), so the most negative
// value divided by -1 gives itself and a remainder of 0.
Hook sdivmod_hook(std::uint16_t hargs, int n) {
  return [hargs, n](Memory& mem) {
    const std::uint64_t a_raw = read_int(mem, hargs, n);
    const std::uint64_t b_raw = read_int(mem, hargs + n, n);
    if (b_raw == 0) throw HookError("RuntimeError", "sdivmod" + std::to_string(8 * n) + ": division by zero");
    const std::int64_t a = sign_extend(a_raw, n), b = sign_extend(b_raw, n);
    const std::uint64_t ua = a < 0 ? 0 - static_cast<std::uint64_t>(a) : static_cast<std::uint64_t>(a);
    const std::uint64_t ub = b < 0 ? 0 - static_cast<std::uint64_t>(b) : static_cast<std::uint64_t>(b);
    std::uint64_t q = ua / ub;
    if ((a < 0) != (b < 0)) q = 0 - q;
    const std::uint64_t r = static_cast<std::uint64_t>(a) - q * static_cast<std::uint64_t>(b);
    write_int(mem, hargs + 2 * n, q, n);
    write_int(mem, hargs + 3 * n, r, n);
  };
}

enum class Shift { kLeft, kLogical, kArithmetic };

// The count is one byte after the value, and the result follows it. A
// count of the width or more gives 0, or the sign in every bit for an
// arithmetic shift.
Hook shift_hook(std::uint16_t hargs, int n, Shift kind) {
  return [hargs, n, kind](Memory& mem) {
    const std::uint64_t v = read_int(mem, hargs, n);
    unsigned c = mem[(hargs + n) & 0xFFFF];
    const unsigned bits = 8 * static_cast<unsigned>(n);
    std::uint64_t r = 0;
    switch (kind) {
      case Shift::kLeft: r = c >= bits ? 0 : (v << c) & mask(n); break;
      case Shift::kLogical: r = c >= bits ? 0 : v >> c; break;
      case Shift::kArithmetic:
        if (c >= bits) c = bits - 1;
        r = static_cast<std::uint64_t>(sign_extend(v, n) >> c) & mask(n);
        break;
    }
    write_int(mem, hargs + n + 1, r, n);
  };
}

// --- The floating hooks ---
//
// They compute with fp_arith (src/fp_arith.h), as the TAC interpreter
// and constant folding do, so the three give the same results (decision
// 1 of step 9 of the port). A float or a double is its bit pattern,
// low byte first.

Int bits_at(const Memory& mem, unsigned addr, int size) { return Int(read_int(mem, addr, size)); }

void write_bits(Memory& mem, unsigned addr, Int bits, int size) {
  write_int(mem, addr, static_cast<std::uint64_t>(bits), size);
}

// The result goes to HARGS+8 for floats and HARGS+16 for doubles.
Hook arith_hook(std::uint16_t hargs, int size, char op) {
  return [hargs, size, op](Memory& mem) {
    const Int a = bits_at(mem, hargs, size);
    const Int b = bits_at(mem, hargs + size, size);
    Int r = 0;
    switch (op) {
      case '+': r = size == 4 ? fp_arith::single_add(a, b) : fp_arith::double_add(a, b); break;
      case '-': r = size == 4 ? fp_arith::single_sub(a, b) : fp_arith::double_sub(a, b); break;
      case '*': r = size == 4 ? fp_arith::single_mul(a, b) : fp_arith::double_mul(a, b); break;
      default: r = size == 4 ? fp_arith::single_div(a, b) : fp_arith::double_div(a, b); break;
    }
    write_bits(mem, hargs + 2 * size, r, size);
  };
}

// A NaN is unordered, so both comparisons give 0 for it.
Hook compare_hook(std::uint16_t hargs, int size, bool lt) {
  return [hargs, size, lt](Memory& mem) {
    const Int a = bits_at(mem, hargs, size);
    const Int b = bits_at(mem, hargs + size, size);
    const fp_arith::Order o = size == 4 ? fp_arith::single_compare(a, b) : fp_arith::double_compare(a, b);
    const bool r = o == fp_arith::Order::kLt || (!lt && o == fp_arith::Order::kEq);
    mem[(hargs + 2 * size) & 0xFFFF] = r ? 1 : 0;
  };
}

// The integer at HARGS+0 as a float or double right after it.
Hook int_to_fp_hook(std::uint16_t hargs, int int_size, bool is_signed, int fp_size) {
  return [hargs, int_size, is_signed, fp_size](Memory& mem) {
    const std::uint64_t raw = read_int(mem, hargs, int_size);
    const Int v = is_signed ? Int(sign_extend(raw, int_size)) : Int(raw);
    write_bits(mem, hargs + int_size, fp_size == 4 ? fp_arith::int_to_single_bits(v) : fp_arith::int_to_double_bits(v),
               fp_size);
  };
}

// The float or double at HARGS+0 truncated toward zero and reduced
// modulo 2^N right after it. A NaN or an infinity has no integer value,
// and the hook fails with the error the TAC interpreter raises for it.
Hook fp_to_int_hook(std::uint16_t hargs, int fp_size, int int_size) {
  return [hargs, fp_size, int_size](Memory& mem) {
    const Int bits = bits_at(mem, hargs, fp_size);
    const std::optional<fp_arith::TruncatedInt> n =
        fp_size == 4 ? fp_arith::single_bits_to_int(bits) : fp_arith::double_bits_to_int(bits);
    if (!n) {
      const bool nan = fp_size == 4 ? (bits & 0x7FFFFF) != 0 : (bits & 0xFFFFFFFFFFFFFull) != 0;
      if (nan) throw HookError("ValueError", "cannot convert float NaN to integer");
      throw HookError("OverflowError", "cannot convert float infinity to integer");
    }
    write_int(mem, hargs + fp_size, static_cast<std::uint64_t>(n->wrapped(8 * int_size)), int_size);
  };
}

Hook f2d_hook(std::uint16_t hargs) {
  return [hargs](Memory& mem) {
    write_bits(mem, hargs + 4, fp_arith::single_bits_to_double_bits(bits_at(mem, hargs, 4)), 8);
  };
}

Hook d2f_hook(std::uint16_t hargs) {
  return [hargs](Memory& mem) {
    write_bits(mem, hargs + 8, fp_arith::double_bits_to_single_bits(bits_at(mem, hargs, 8)), 4);
  };
}

struct Conversion {
  const char* name;
  int int_size;
  bool is_signed;
};

constexpr Conversion kConversions[] = {
    {"i", 2, true}, {"u", 2, false}, {"l", 4, true}, {"ul", 4, false}, {"ll", 8, true}, {"ull", 8, false},
};

}  // namespace

const std::vector<std::string>& hooked_helpers() {
  static const std::vector<std::string> names = [] {
    std::vector<std::string> out{"mul64", "udivmod64", "sdivmod64", "asl64", "asr64", "lsr64"};
    for (const char* fp : {"f", "d"}) {
      for (const Conversion& c : kConversions) out.push_back(std::string(c.name) + "2" + fp);
    }
    for (const char* fp : {"f", "d"}) {
      for (const Conversion& c : kConversions) out.push_back(std::string(fp) + "2" + c.name);
    }
    for (const char* name : {"f2d", "d2f", "fadd", "fsub", "fmul", "fdiv", "dadd", "dsub", "dmul", "ddiv", "flt",
                             "fle", "dlt", "dle"}) {
      out.emplace_back(name);
    }
    return out;
  }();
  return names;
}

Hook make_hook(std::string_view name, std::uint16_t hargs) {
  if (name == "mul64") return mul_hook(hargs, 8);
  if (name == "udivmod64") return udivmod_hook(hargs, 8);
  if (name == "sdivmod64") return sdivmod_hook(hargs, 8);
  if (name == "asl64") return shift_hook(hargs, 8, Shift::kLeft);
  if (name == "asr64") return shift_hook(hargs, 8, Shift::kArithmetic);
  if (name == "lsr64") return shift_hook(hargs, 8, Shift::kLogical);
  if (name == "f2d") return f2d_hook(hargs);
  if (name == "d2f") return d2f_hook(hargs);
  for (const Conversion& c : kConversions) {
    if (name == std::string(c.name) + "2f") return int_to_fp_hook(hargs, c.int_size, c.is_signed, 4);
    if (name == std::string(c.name) + "2d") return int_to_fp_hook(hargs, c.int_size, c.is_signed, 8);
    if (name == std::string("f2") + c.name) return fp_to_int_hook(hargs, 4, c.int_size);
    if (name == std::string("d2") + c.name) return fp_to_int_hook(hargs, 8, c.int_size);
  }
  if (name.size() == 4 && (name[0] == 'f' || name[0] == 'd')) {
    const int size = name[0] == 'f' ? 4 : 8;
    const std::string_view op = name.substr(1);
    if (op == "add") return arith_hook(hargs, size, '+');
    if (op == "sub") return arith_hook(hargs, size, '-');
    if (op == "mul") return arith_hook(hargs, size, '*');
    if (op == "div") return arith_hook(hargs, size, '/');
  }
  if (name == "flt" || name == "dlt") return compare_hook(hargs, name[0] == 'f' ? 4 : 8, true);
  if (name == "fle" || name == "dle") return compare_hook(hargs, name[0] == 'f' ? 4 : 8, false);
  throw std::out_of_range("no hook for the helper " + std::string(name));
}

Runtime build_runtime(const passes::RuntimeLayout& layout) {
  Runtime rt;
  rt.ssp = static_cast<std::uint16_t>(layout.ssp);
  rt.hargs = static_cast<std::uint16_t>(layout.hargs);
  asm_ast::Program helpers;
  for (const asm_ast::Function& fn : rtlib::all_helper_functions()) helpers.top_level.emplace_back(fn);
  const std::map<std::string, std::int64_t> zp{{"SSP", static_cast<std::int64_t>(layout.ssp)},
                                                {"FP", static_cast<std::int64_t>(layout.fp)},
                                                {"HARGS", static_cast<std::int64_t>(layout.hargs)},
                                                {"DPTR", static_cast<std::int64_t>(layout.dptr)}};
  const AssembledProgram assembled = assemble(helpers, kHelperOrigin, zp);
  for (const asm_ast::Function& fn : rtlib::all_helper_functions()) rt.symbols[fn.name] = assembled.symbols.at(fn.name);
  rt.helper_image.assign(assembled.image.begin() + kHelperOrigin, assembled.image.begin() + assembled.code_end);
  std::int64_t cursor = assembled.code_end;
  // `icall`: the caller puts the target in DPTR and JSRs here, and the
  // target's RTS returns to the caller.
  rt.symbols["icall"] = cursor;
  rt.helper_image.insert(rt.helper_image.end(), {0x6C, static_cast<std::uint8_t>(layout.dptr), 0x00});
  cursor = (cursor + 3 + 0xF) & ~std::int64_t{0xF};
  for (const std::string& name : hooked_helpers()) {
    rt.symbols[name] = cursor;
    rt.hooks[static_cast<std::uint16_t>(cursor)] = make_hook(name, rt.hargs);
    cursor += kTrapStride;
  }
  return rt;
}

std::vector<std::uint8_t> boot_stub(std::int64_t main_addr, std::optional<std::int64_t> init_addr,
                                    std::uint16_t ssp) {
  std::vector<std::int64_t> calls;
  if (init_addr) calls.push_back(*init_addr);
  calls.push_back(main_addr);
  for (const std::int64_t addr : calls) {
    if (addr < 0 || addr > 0xFFFF) throw std::invalid_argument("call address " + std::to_string(addr) + " out of range");
  }
  std::vector<std::uint8_t> out{0xA9, kSspInit & 0xFF, 0x85, static_cast<std::uint8_t>(ssp),
                                0xA9, kSspInit >> 8,   0x85, static_cast<std::uint8_t>(ssp + 1)};
  for (const std::int64_t addr : calls) {
    out.insert(out.end(), {0x20, static_cast<std::uint8_t>(addr), static_cast<std::uint8_t>(addr >> 8)});
  }
  out.push_back(0x00);
  return out;
}

void install_runtime(std::vector<std::uint8_t>& image, const Runtime& runtime, std::int64_t main_addr,
                     std::optional<std::int64_t> init_addr) {
  const std::vector<std::uint8_t> boot = boot_stub(main_addr, init_addr, runtime.ssp);
  std::copy(boot.begin(), boot.end(), image.begin() + runtime.boot_addr);
  image[kResetVector] = static_cast<std::uint8_t>(runtime.boot_addr);
  image[kResetVector + 1] = static_cast<std::uint8_t>(runtime.boot_addr >> 8);
  std::copy(runtime.helper_image.begin(), runtime.helper_image.end(), image.begin() + runtime.helper_origin);
}

}  // namespace c6502::sim
