#include "fuzz/stmts.h"

#include <algorithm>
#include <map>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "fuzz/bit_precise_types.h"
#include "py_random.h"

namespace c6502::fuzz::stmts {

namespace {

TypeTable standard_types() {
  return {
      {"schar", {"signed char", 8, true}},          {"uchar", {"unsigned char", 8, false}},
      {"int", {"int", 16, true}},                   {"uint", {"unsigned int", 16, false}},
      {"long", {"long", 32, true}},                 {"ulong", {"unsigned long", 32, false}},
      {"llong", {"long long", 64, true}},           {"ullong", {"unsigned long long", 64, false}},
  };
}

const std::vector<std::pair<std::string, int>> kTypeWeights = {
    {"schar", 6}, {"uchar", 8}, {"int", 6},   {"uint", 8},
    {"long", 3},  {"ulong", 3}, {"llong", 1}, {"ullong", 1},
};
constexpr int kBitPreciseWeight = 3;
constexpr int kStmtsPerFnUnderFrameLimit = 12;
constexpr std::size_t kMixesPerFinUnderFrameLimit = 4;
constexpr long kStepLimit = 200'000;
constexpr int kZeroPageBytes = 40;
constexpr int kAttempts = 200;

const RankTable kRank = {{"int", 1}, {"uint", 1}, {"long", 2},
                         {"ulong", 2}, {"llong", 3}, {"ullong", 3}};
const UnsignedOf kUnsignedOf = {{"int", "uint"}, {"long", "ulong"}, {"llong", "ullong"}};

const Int kInteresting[] = {0,      1,      2,       3,          0x7F,
                            0x80,   0xFF,   0x100,   0x7FFF,     0x8000,
                            0xFFFF, 0x10000, Int(0x7FFFFFFF), Int(0x80000000LL)};

struct Expr;
using ExprPtr = std::shared_ptr<const Expr>;

struct Lvalue {
  enum class Kind { kVar, kIndex, kMember, kDeref };
  Kind kind = Kind::kVar;
  std::string name;
  std::string member;
  ExprPtr index;
};

// The operators that modify an object, in fuzz/stmts.py's `_OPS` order.
enum class OpKind { kPreInc, kPreDec, kPostInc, kPostDec, kAdd, kSub, kMul, kAnd, kOr, kXor, kShl, kShr };
constexpr OpKind kOpKinds[] = {OpKind::kPreInc, OpKind::kPreDec, OpKind::kPostInc, OpKind::kPostDec,
                               OpKind::kAdd,    OpKind::kSub,    OpKind::kMul,     OpKind::kAnd,
                               OpKind::kOr,     OpKind::kXor,    OpKind::kShl,     OpKind::kShr};

bool is_incdec(OpKind k) { return k <= OpKind::kPostDec; }

enum class ExprKind { kConst, kRead, kCast, kUnary, kBinary, kShift, kCompare, kLogic, kCond, kOp, kCall, kAddress };

struct Expr {
  ExprKind kind = ExprKind::kConst;
  Int value = 0;
  std::string value_type;
  Lvalue read_from;
  std::string cast_to;
  std::string op;
  int shift_count = 0;
  OpKind op_kind = OpKind::kPreInc;
  Lvalue target;
  int right_operand = 0;
  std::string callee;
  std::string address_of;
  std::vector<ExprPtr> operands;
};

ExprPtr make(Expr e) { return std::make_shared<const Expr>(std::move(e)); }

ExprPtr constant(Int v, std::string t) {
  Expr e;
  e.kind = ExprKind::kConst;
  e.value = v;
  e.value_type = std::move(t);
  return make(std::move(e));
}

ExprPtr read(Lvalue lv) {
  Expr e;
  e.kind = ExprKind::kRead;
  e.read_from = std::move(lv);
  return make(std::move(e));
}

ExprPtr with_operands(ExprKind kind, std::string op, std::vector<ExprPtr> operands) {
  Expr e;
  e.kind = kind;
  e.op = std::move(op);
  e.operands = std::move(operands);
  return make(std::move(e));
}

Lvalue var(std::string name) { return Lvalue{Lvalue::Kind::kVar, std::move(name), "", nullptr}; }

struct Stmt;
using Block = std::vector<Stmt>;

struct SwitchCase {
  int value = 0;
  Block body;
  bool breaks = false;
};

enum class StmtKind { kAssign, kExpr, kMix, kIf, kFor, kWhile, kDo, kSwitch, kBreak, kContinue, kReturn };

struct Stmt {
  StmtKind kind = StmtKind::kExpr;
  Lvalue lv;
  ExprPtr value;
  ExprPtr cond;
  Block then;
  Block els;
  std::string counter;
  int count = 0;
  Block body;
  ExprPtr selector;
  std::vector<SwitchCase> cases;
  std::optional<Block> default_body;
};

enum class ObjKind { kLocal, kParam, kStatic, kGlobal, kZeroPage, kArray, kStruct, kGlobalPointer, kPointerParam };

struct Member {
  std::string name;
  std::string type;
  Int init = 0;
};

struct Obj {
  std::string name;
  std::string type;
  ObjKind kind = ObjKind::kLocal;
  Int init = 0;
  std::vector<Int> elements;
  std::string pointee;
  bool vol = false;
  std::vector<Member> members;
};

struct Fn {
  std::string name;
  std::string ret;
  std::vector<Obj> params;
  std::vector<Obj> locals;
  std::vector<Obj> statics;
  std::vector<std::string> counters;
  Block body;
};

struct Prog {
  std::vector<Obj> globals;
  std::vector<Fn> fns;
};

std::vector<Lvalue> global_reads(const Obj& g) {
  switch (g.kind) {
    case ObjKind::kGlobal:
    case ObjKind::kZeroPage:
      return {var(g.name)};
    case ObjKind::kArray: {
      std::vector<Lvalue> out;
      for (int i = 0; i < 4; ++i) out.push_back({Lvalue::Kind::kIndex, g.name, "", constant(i, "int")});
      return out;
    }
    case ObjKind::kStruct: {
      std::vector<Lvalue> out;
      for (const Member& m : g.members) out.push_back({Lvalue::Kind::kMember, g.name, m.name, nullptr});
      return out;
    }
    default:
      return {};
  }
}

// Returns the objects that main mixes into the checksum after its body,
// in order.
std::vector<Lvalue> final_reads(const Prog& p) {
  std::vector<Lvalue> reads;
  for (const Obj& g : p.globals) {
    for (Lvalue& l : global_reads(g)) reads.push_back(std::move(l));
  }
  return reads;
}

class Types {
 public:
  explicit Types(TypeTable& table) : table_(table) {}

  const IntType& at(const std::string& t) const { return table_.find(t)->second; }
  Int wrap(Int v, const std::string& t) const { return fuzz::wrap(v, at(t)); }

  // A bit-precise type isn't promoted (C23 §6.3.1.1p2).
  static std::string promote(const std::string& t) {
    return t == "schar" || t == "uchar" ? "int" : t;
  }

  // C99 §6.3.1.8's usual arithmetic conversions, and C23's for the
  // bit-precise types.
  std::string common(const std::string& a0, const std::string& b0) {
    const std::string a = promote(a0);
    const std::string b = promote(b0);
    if (a == b) return a;
    if (is_bit_precise(a) || is_bit_precise(b)) {
      return common_bit_precise(table_, kRank, kUnsignedOf, a, b);
    }
    const bool sa = at(a).is_signed;
    const bool sb = at(b).is_signed;
    if (sa == sb) return kRank.at(a) >= kRank.at(b) ? a : b;
    const std::string& u = sa ? b : a;
    const std::string& s = sa ? a : b;
    if (kRank.at(u) >= kRank.at(s)) return u;
    if (at(s).width > at(u).width) return s;
    return kUnsignedOf.at(s);
  }

  std::string lit(Int v, const std::string& t) const {
    const IntType& it = at(t);
    return "((" + it.spelling + ")" + decimal(v & ((Int(1) << it.width) - 1)) + "ULL)";
  }

  bool has(const std::string& t) const { return table_.count(t) != 0; }

 private:
  TypeTable& table_;
};

const char* op_format(OpKind k) {
  switch (k) {
    case OpKind::kPreInc: return "++{x}";
    case OpKind::kPreDec: return "--{x}";
    case OpKind::kPostInc: return "{x}++";
    case OpKind::kPostDec: return "{x}--";
    case OpKind::kAdd: return "{x} += {k}";
    case OpKind::kSub: return "{x} -= {k}";
    case OpKind::kMul: return "{x} *= {k}";
    case OpKind::kAnd: return "{x} &= {k}";
    case OpKind::kOr: return "{x} |= {k}";
    case OpKind::kXor: return "{x} ^= {k}";
    case OpKind::kShl: return "{x} <<= {k}";
    case OpKind::kShr: return "{x} >>= {k}";
  }
  return "";
}

class Renderer {
 public:
  explicit Renderer(const Types& types) : types_(types) {}

  std::string lv(const Lvalue& l) const {
    switch (l.kind) {
      case Lvalue::Kind::kVar: return l.name;
      case Lvalue::Kind::kIndex: return l.name + "[" + ex(*l.index) + "]";
      case Lvalue::Kind::kMember: return l.name + "." + l.member;
      case Lvalue::Kind::kDeref: return "(*" + l.name + ")";
    }
    return "";
  }

  std::string ex(const Expr& e) const {
    switch (e.kind) {
      case ExprKind::kConst: return types_.lit(e.value, e.value_type);
      case ExprKind::kRead: return lv(e.read_from);
      case ExprKind::kCast: return "((" + types_.at(e.cast_to).spelling + ")(" + ex(*e.operands[0]) + "))";
      case ExprKind::kUnary: return "(" + e.op + ex(*e.operands[0]) + ")";
      case ExprKind::kBinary:
      case ExprKind::kCompare:
      case ExprKind::kLogic:
        return "(" + ex(*e.operands[0]) + " " + e.op + " " + ex(*e.operands[1]) + ")";
      case ExprKind::kShift:
        return "(" + ex(*e.operands[0]) + " " + e.op + " " + std::to_string(e.shift_count) + ")";
      case ExprKind::kCond:
        return "(" + ex(*e.operands[0]) + " ? " + ex(*e.operands[1]) + " : " + ex(*e.operands[2]) + ")";
      case ExprKind::kOp: {
        std::string text = op_format(e.op_kind);
        const std::string x = lv(e.target);
        const std::string k = std::to_string(e.right_operand);
        if (const auto at = text.find("{x}"); at != std::string::npos) text.replace(at, 3, x);
        if (const auto at = text.find("{k}"); at != std::string::npos) text.replace(at, 3, k);
        return "(" + text + ")";
      }
      case ExprKind::kCall: {
        std::string args;
        for (std::size_t i = 0; i < e.operands.size(); ++i) {
          if (i != 0) args += ", ";
          args += ex(*e.operands[i]);
        }
        return e.callee + "(" + args + ")";
      }
      case ExprKind::kAddress: return "&" + e.address_of;
    }
    return "";
  }

  void stmts(const Block& body, const std::string& ind, std::vector<std::string>& out) const {
    for (const Stmt& s : body) stmt(s, ind, out);
  }

  void stmt(const Stmt& s, const std::string& ind, std::vector<std::string>& out) const {
    const std::string nxt = ind + "    ";
    switch (s.kind) {
      case StmtKind::kAssign:
        out.push_back(ind + lv(s.lv) + " = " + ex(*s.value) + ";");
        return;
      case StmtKind::kExpr:
        out.push_back(ind + ex(*s.value) + ";");
        return;
      case StmtKind::kMix:
        out.push_back(ind + "mix(" + ex(*s.value) + ");");
        return;
      case StmtKind::kIf:
        out.push_back(ind + "if (" + ex(*s.cond) + ") {");
        stmts(s.then, nxt, out);
        if (!s.els.empty()) {
          out.push_back(ind + "} else {");
          stmts(s.els, nxt, out);
        }
        out.push_back(ind + "}");
        return;
      case StmtKind::kFor: {
        const std::string& c = s.counter;
        out.push_back(ind + "for (unsigned char " + c + " = 0; " + c + " < " + std::to_string(s.count) +
                      "; " + c + "++) {");
        stmts(s.body, nxt, out);
        out.push_back(ind + "}");
        return;
      }
      case StmtKind::kWhile:
        out.push_back(ind + s.counter + " = 0;");
        out.push_back(ind + "while (" + s.counter + "++ < " + std::to_string(s.count) + ") {");
        stmts(s.body, nxt, out);
        out.push_back(ind + "}");
        return;
      case StmtKind::kDo:
        out.push_back(ind + s.counter + " = 0;");
        out.push_back(ind + "do {");
        stmts(s.body, nxt, out);
        out.push_back(ind + "} while (++" + s.counter + " < " + std::to_string(s.count) + ");");
        return;
      case StmtKind::kSwitch:
        out.push_back(ind + "switch (" + ex(*s.selector) + ") {");
        for (const SwitchCase& c : s.cases) {
          out.push_back(ind + "case " + std::to_string(c.value) + ":");
          if (c.body.empty()) {
            if (!c.breaks) out.push_back(nxt + ";");
          } else {
            stmts(c.body, nxt, out);
          }
          if (c.breaks) out.push_back(nxt + "break;");
        }
        if (s.default_body) {
          out.push_back(ind + "default:");
          if (s.default_body->empty()) {
            out.push_back(nxt + ";");
          } else {
            stmts(*s.default_body, nxt, out);
          }
        }
        out.push_back(ind + "}");
        return;
      case StmtKind::kBreak:
        out.push_back(ind + "break;");
        return;
      case StmtKind::kContinue:
        out.push_back(ind + "continue;");
        return;
      case StmtKind::kReturn:
        out.push_back(ind + "return " + ex(*s.value) + ";");
        return;
    }
  }

 private:
  const Types& types_;
};

std::string vol_prefix(const Obj& o) { return o.vol ? "volatile " : ""; }

std::string render(const Prog& p, std::int64_t expected, const Types& types) {
  const Renderer r(types);
  std::vector<std::string> lines = {
      "// expect: " + std::to_string(expected), "unsigned int acc;", "void mix(unsigned long v) {",
      "    acc = (unsigned int)((acc << 1) | (acc >> 15)) ^ (unsigned int)v ^ (unsigned int)(v >> 16);",
      "}"};
  for (const Obj& g : p.globals) {
    std::string c = types.has(g.type) ? types.at(g.type).spelling : "";
    if (g.vol) c = "volatile " + c;
    switch (g.kind) {
      case ObjKind::kGlobal:
        lines.push_back(c + " " + g.name + " = " + types.lit(g.init, g.type) + ";");
        break;
      case ObjKind::kZeroPage:
        lines.push_back(c + " " + g.name + " [[c6502::zeropage]] = " + types.lit(g.init, g.type) + ";");
        break;
      case ObjKind::kArray: {
        std::string vals;
        for (std::size_t i = 0; i < g.elements.size(); ++i) {
          if (i != 0) vals += ", ";
          vals += types.lit(g.elements[i], g.type);
        }
        lines.push_back(c + " " + g.name + "[4] = {" + vals + "};");
        break;
      }
      case ObjKind::kStruct: {
        std::string fields;
        std::string vals;
        for (std::size_t i = 0; i < g.members.size(); ++i) {
          if (i != 0) {
            fields += " ";
            vals += ", ";
          }
          fields += types.at(g.members[i].type).spelling + " " + g.members[i].name + ";";
          vals += types.lit(g.members[i].init, g.members[i].type);
        }
        lines.push_back("struct S_" + g.name + " { unsigned char pad; " + fields + " };");
        lines.push_back("struct S_" + g.name + " " + g.name + " = {0, " + vals + "};");
        break;
      }
      case ObjKind::kGlobalPointer:
        lines.push_back(c + " *" + g.name + " = &" + g.pointee + ";");
        break;
      default:
        break;
    }
  }
  const std::vector<Lvalue> reads = final_reads(p);
  std::size_t fins = 0;
  for (std::size_t i = 0; i < reads.size(); i += kMixesPerFinUnderFrameLimit, ++fins) {
    lines.push_back("void fin" + std::to_string(fins) + "(void) {");
    for (std::size_t j = i; j < std::min(reads.size(), i + kMixesPerFinUnderFrameLimit); ++j) {
      lines.push_back("    mix(" + r.lv(reads[j]) + ");");
    }
    lines.push_back("}");
  }
  for (const Fn& f : p.fns) {
    if (f.name == "main") {
      lines.push_back("int main(void) {");
    } else {
      std::string params;
      for (const Obj& o : f.params) {
        if (!params.empty()) params += ", ";
        params += types.at(o.type).spelling + (o.kind == ObjKind::kPointerParam ? " *" : " ") + o.name;
      }
      if (params.empty()) params = "void";
      lines.push_back(types.at(f.ret).spelling + " " + f.name + "(" + params + ") {");
    }
    for (const Obj& o : f.statics) {
      lines.push_back("    static " + vol_prefix(o) + types.at(o.type).spelling + " " + o.name + " = " +
                      types.lit(o.init, o.type) + ";");
    }
    for (const Obj& o : f.locals) {
      lines.push_back("    " + vol_prefix(o) + types.at(o.type).spelling + " " + o.name + " = " +
                      types.lit(o.init, o.type) + ";");
    }
    for (const std::string& c : f.counters) lines.push_back("    unsigned char " + c + ";");
    r.stmts(f.body, "    ", lines);
    if (f.name == "main") {
      for (std::size_t k = 0; k < fins; ++k) lines.push_back("    fin" + std::to_string(k) + "();");
      lines.push_back("    return (int)(acc & 0x7FFF);");
    }
    lines.push_back("}");
  }
  std::string out;
  for (const std::string& l : lines) out += l + "\n";
  return out;
}

struct UndefinedBehavior {};
struct PastStepLimit {};
struct Returned {
  Int value;
};
struct Broke {};
struct Continued {};

struct Value {
  Int v;
  std::string type;
};

class Interp {
 public:
  Interp(const Prog& p, Types& types) : types_(types) {
    for (const Fn& f : p.fns) fns_[f.name] = &f;
    for (const Obj& g : p.globals) {
      switch (g.kind) {
        case ObjKind::kGlobal:
        case ObjKind::kZeroPage:
          set_object(global_key(g.name), g.init, g.type);
          break;
        case ObjKind::kArray:
          for (std::size_t i = 0; i < g.elements.size(); ++i) {
            set_object(element_key(g.name, static_cast<int>(i)), g.elements[i], g.type);
          }
          break;
        case ObjKind::kStruct:
          for (const Member& m : g.members) set_object(member_key(g.name, m.name), m.init, m.type);
          break;
        default:
          break;
      }
    }
    for (const Fn& f : p.fns) {
      for (const Obj& o : f.statics) set_object(static_key(o.name), o.init, o.type);
    }
    for (const Obj& g : p.globals) {
      if (g.kind == ObjKind::kGlobalPointer) gptr_[g.name] = global_key(g.pointee);
    }
  }

  std::int64_t acc() const { return acc_; }

  void mix(Int v) {
    const Int u = v & 0xFFFFFFFF;
    const std::int64_t rot = ((acc_ << 1) | (acc_ >> 15)) & 0xFFFF;
    acc_ = rot ^ static_cast<std::int64_t>(u & 0xFFFF) ^ static_cast<std::int64_t>((u >> 16) & 0xFFFF);
  }

  void run_program(const Prog& p) {
    call("main", {});
    frames_.emplace_back();
    for (const Lvalue& l : final_reads(p)) mix(mem_.at(loc(l)));
  }

 private:
  using Frame = std::unordered_map<std::string, std::string>;

  static std::string global_key(const std::string& n) { return "g " + n; }
  static std::string element_key(const std::string& a, int i) { return "a " + a + " " + std::to_string(i); }
  static std::string member_key(const std::string& s, const std::string& m) { return "s " + s + " " + m; }
  static std::string static_key(const std::string& n) { return "st " + n; }
  static std::string local_key(std::size_t depth, const std::string& n) {
    return "l " + std::to_string(depth) + " " + n;
  }

  void set_object(const std::string& key, Int v, const std::string& type) {
    mem_[key] = v;
    obj_types_[key] = type;
  }

  void count_step() {
    if (++steps_ > kStepLimit) throw PastStepLimit{};
  }

  std::string loc(const Lvalue& l) {
    Frame& frame = frames_.back();
    switch (l.kind) {
      case Lvalue::Kind::kVar: {
        if (auto it = frame.find(l.name); it != frame.end()) return it->second;
        if (mem_.count(global_key(l.name)) != 0) return global_key(l.name);
        return static_key(l.name);
      }
      case Lvalue::Kind::kIndex: {
        const Int i = ev(*l.index).v;
        if (!(0 <= i && i < 4)) throw UndefinedBehavior{};
        return element_key(l.name, static_cast<int>(i));
      }
      case Lvalue::Kind::kMember:
        return member_key(l.name, l.member);
      case Lvalue::Kind::kDeref:
        if (auto it = frame.find(l.name); it != frame.end()) return pointees_.at(it->second);
        return gptr_.at(l.name);
    }
    return "";
  }

  void store(const std::string& key, Int v) { mem_[key] = types_.wrap(v, obj_types_.at(key)); }

  Int check(Int v, const std::string& t) const {
    const Int w = types_.wrap(v, t);
    if (types_.at(t).is_signed && w != v) throw UndefinedBehavior{};
    return w;
  }

  Int arith(const std::string& op, Int x, Int y, const std::string& ct) const {
    Int r;
    if (op == "/" || op == "%") {
      if (y == 0) throw UndefinedBehavior{};
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
    return check(r, ct);
  }

  Int shift(bool left, Int v, int k, const std::string& pt) const {
    if (!(0 <= k && k < types_.at(pt).width)) throw UndefinedBehavior{};
    if (!left) return v >> k;
    if (v < 0) throw UndefinedBehavior{};
    return check(v << k, pt);
  }

  Value ev(const Expr& e) {
    count_step();
    switch (e.kind) {
      case ExprKind::kConst:
        return {e.value, e.value_type};
      case ExprKind::kRead: {
        const std::string k = loc(e.read_from);
        return {mem_.at(k), obj_types_.at(k)};
      }
      case ExprKind::kCast:
        return {types_.wrap(ev(*e.operands[0]).v, e.cast_to), e.cast_to};
      case ExprKind::kUnary: {
        const Value a = ev(*e.operands[0]);
        if (e.op == "!") return {Int(a.v == 0 ? 1 : 0), "int"};
        const std::string pt = Types::promote(a.type);
        const Int v = types_.wrap(a.v, pt);
        return {e.op == "-" ? check(-v, pt) : types_.wrap(~v, pt), pt};
      }
      case ExprKind::kBinary: {
        const Value a = ev(*e.operands[0]);
        const Value b = ev(*e.operands[1]);
        const std::string ct = types_.common(a.type, b.type);
        return {arith(e.op, types_.wrap(a.v, ct), types_.wrap(b.v, ct), ct), ct};
      }
      case ExprKind::kShift: {
        const Value a = ev(*e.operands[0]);
        const std::string pt = Types::promote(a.type);
        return {shift(e.op == "<<", types_.wrap(a.v, pt), e.shift_count, pt), pt};
      }
      case ExprKind::kCompare: {
        const Value a = ev(*e.operands[0]);
        const Value b = ev(*e.operands[1]);
        const std::string ct = types_.common(a.type, b.type);
        const Int x = types_.wrap(a.v, ct);
        const Int y = types_.wrap(b.v, ct);
        bool r;
        if (e.op == "<") {
          r = x < y;
        } else if (e.op == ">") {
          r = x > y;
        } else if (e.op == "<=") {
          r = x <= y;
        } else if (e.op == ">=") {
          r = x >= y;
        } else if (e.op == "==") {
          r = x == y;
        } else {
          r = x != y;
        }
        return {Int(r ? 1 : 0), "int"};
      }
      case ExprKind::kLogic: {
        const Value a = ev(*e.operands[0]);
        if (e.op == "&&") {
          if (a.v == 0) return {0, "int"};
          return {Int(ev(*e.operands[1]).v != 0 ? 1 : 0), "int"};
        }
        if (a.v != 0) return {1, "int"};
        return {Int(ev(*e.operands[1]).v != 0 ? 1 : 0), "int"};
      }
      case ExprKind::kCond: {
        const Value c = ev(*e.operands[0]);
        const std::string at = type_of(*e.operands[1]);
        const std::string bt = type_of(*e.operands[2]);
        const std::string ct = types_.common(at, bt);
        const Value v = ev(*e.operands[c.v != 0 ? 1 : 2]);
        return {types_.wrap(v.v, ct), ct};
      }
      case ExprKind::kOp:
        return op(e.op_kind, e.target, e.right_operand);
      case ExprKind::kCall:
        return call(e.callee, e.operands);
      case ExprKind::kAddress:
        break;
    }
    throw std::logic_error("stmts: an address outside a call's arguments");
  }

  std::string type_of(const Expr& e) {
    const long saved = steps_;
    std::string t = ev(e).type;
    steps_ = saved;
    return t;
  }

  Value op(OpKind kind, const Lvalue& l, int k) {
    const std::string key = loc(l);
    const std::string t = obj_types_.at(key);
    const Int v = mem_.at(key);
    const std::string pt = Types::promote(t);
    if (is_incdec(kind)) {
      const bool inc = kind == OpKind::kPreInc || kind == OpKind::kPostInc;
      const Int changed = types_.wrap(check(types_.wrap(v, pt) + (inc ? 1 : -1), pt), t);
      mem_[key] = changed;
      const bool pre = kind == OpKind::kPreInc || kind == OpKind::kPreDec;
      return {pre ? changed : v, t};
    }
    Int r;
    if (kind == OpKind::kShl || kind == OpKind::kShr) {
      r = shift(kind == OpKind::kShl, types_.wrap(v, pt), k, pt);
    } else {
      const std::string ct = types_.common(t, "int");
      const char* sym = kind == OpKind::kAdd   ? "+"
                        : kind == OpKind::kSub ? "-"
                        : kind == OpKind::kMul ? "*"
                        : kind == OpKind::kAnd ? "&"
                        : kind == OpKind::kOr  ? "|"
                                               : "^";
      r = arith(sym, types_.wrap(v, ct), types_.wrap(k, ct), ct);
    }
    const Int changed = types_.wrap(r, t);
    mem_[key] = changed;
    return {changed, t};
  }

  Value call(const std::string& name, const std::vector<ExprPtr>& args) {
    const Fn& f = *fns_.at(name);
    struct Arg {
      bool pointer;
      Int value;
      std::string pointee;
    };
    std::vector<Arg> vals;
    for (std::size_t i = 0; i < f.params.size() && i < args.size(); ++i) {
      const Obj& p = f.params[i];
      if (p.kind == ObjKind::kPointerParam) {
        vals.push_back({true, 0, loc(var(args[i]->address_of))});
      } else {
        vals.push_back({false, types_.wrap(ev(*args[i]).v, p.type), ""});
      }
    }
    const std::size_t depth = frames_.size();
    Frame frame;
    for (std::size_t i = 0; i < vals.size(); ++i) {
      const std::string key = local_key(depth, f.params[i].name);
      frame[f.params[i].name] = key;
      if (vals[i].pointer) {
        pointees_[key] = vals[i].pointee;
      } else {
        set_object(key, vals[i].value, f.params[i].type);
      }
    }
    for (const Obj& o : f.locals) {
      const std::string key = local_key(depth, o.name);
      frame[o.name] = key;
      set_object(key, o.init, o.type);
    }
    for (const std::string& c : f.counters) {
      const std::string key = local_key(depth, c);
      frame[c] = key;
      set_object(key, 0, "uchar");
    }
    frames_.push_back(std::move(frame));
    Int result = 0;
    try {
      run(f.body);
    } catch (const Returned& r) {
      result = r.value;
    } catch (...) {
      frames_.pop_back();
      throw;
    }
    frames_.pop_back();
    return {types_.wrap(result, f.ret), f.ret};
  }

  void run(const Block& body) {
    for (const Stmt& s : body) exec(s);
  }

  bool run_iteration(const Block& body) {
    try {
      run(body);
    } catch (const Continued&) {
    } catch (const Broke&) {
      return false;
    }
    return true;
  }

  void exec(const Stmt& s) {
    count_step();
    switch (s.kind) {
      case StmtKind::kAssign: {
        const Int v = ev(*s.value).v;
        store(loc(s.lv), v);
        return;
      }
      case StmtKind::kExpr:
        ev(*s.value);
        return;
      case StmtKind::kMix:
        mix(ev(*s.value).v);
        return;
      case StmtKind::kIf:
        run(ev(*s.cond).v != 0 ? s.then : s.els);
        return;
      case StmtKind::kFor: {
        const std::string key = local_key(frames_.size() - 1, s.counter);
        frames_.back()[s.counter] = key;
        set_object(key, 0, "uchar");
        while (mem_.at(key) < s.count) {
          if (!run_iteration(s.body)) break;
          mem_[key] = types_.wrap(mem_.at(key) + 1, "uchar");
        }
        return;
      }
      case StmtKind::kWhile: {
        const std::string key = frames_.back().at(s.counter);
        mem_[key] = 0;
        while (true) {
          const Int old = mem_.at(key);
          mem_[key] = types_.wrap(old + 1, "uchar");
          if (!(old < s.count)) break;
          if (!run_iteration(s.body)) break;
        }
        return;
      }
      case StmtKind::kDo: {
        const std::string key = frames_.back().at(s.counter);
        mem_[key] = 0;
        while (true) {
          if (!run_iteration(s.body)) break;
          mem_[key] = types_.wrap(mem_.at(key) + 1, "uchar");
          if (!(mem_.at(key) < s.count)) break;
        }
        return;
      }
      case StmtKind::kSwitch: {
        const Int v = ev(*s.selector).v;
        std::optional<std::size_t> start;
        for (std::size_t i = 0; i < s.cases.size(); ++i) {
          if (s.cases[i].value == v) {
            start = i;
            break;
          }
        }
        if (!start && s.default_body) start = s.cases.size();
        if (!start) return;
        try {
          for (std::size_t i = *start; i < s.cases.size(); ++i) {
            run(s.cases[i].body);
            if (s.cases[i].breaks) return;
          }
          if (s.default_body) run(*s.default_body);
        } catch (const Broke&) {
        }
        return;
      }
      case StmtKind::kBreak:
        throw Broke{};
      case StmtKind::kContinue:
        throw Continued{};
      case StmtKind::kReturn:
        throw Returned{ev(*s.value).v};
    }
  }

  Types& types_;
  std::map<std::string, const Fn*> fns_;
  std::unordered_map<std::string, Int> mem_;
  std::unordered_map<std::string, std::string> obj_types_;
  std::unordered_map<std::string, std::string> pointees_;
  std::map<std::string, std::string> gptr_;
  std::vector<Frame> frames_;
  std::int64_t acc_ = 0;
  long steps_ = 0;
};

struct Writable {
  Lvalue lv;
  std::string type;
  std::string key;
};

struct WithOp {
  ExprPtr e;
  std::optional<std::string> written;
};

struct SideOp {
  ExprPtr e;
  std::string key;
};

struct Context {
  int depth = 0;
  int loops = 0;
  bool breakable = false;
  bool in_loop = false;
};

class Generator {
 public:
  Generator(py_random::Random& rng, py_random::Random& vrng, const Types& types,
            const std::vector<std::string>& extra_types)
      : rng_(rng), vrng_(vrng), types_(types), weights_(kTypeWeights) {
    for (const std::string& t : extra_types) {
      if (std::none_of(weights_.begin(), weights_.end(), [&](const auto& w) { return w.first == t; })) {
        weights_.push_back({t, kBitPreciseWeight});
      }
    }
    for (const auto& w : weights_) weight_values_.push_back(w.second);
  }

  Prog program() {
    py_random::Random& r = rng_;
    std::vector<Obj> globals;
    const int n_globals = static_cast<int>(r.randint(2, 4));
    for (int i = 0; i < n_globals; ++i) {
      const std::string t = typ();
      Obj o{fresh("g"), t, ObjKind::kGlobal};
      o.init = value(t);
      globals.push_back(std::move(o));
    }
    const int n_zp = static_cast<int>(r.randint(0, 3));
    for (int i = 0; i < n_zp; ++i) {
      const std::string t = typ();
      const int size = (types_.at(t).width + 7) / 8;
      if (zp_bytes_ + size > kZeroPageBytes) break;
      zp_bytes_ += size;
      Obj o{fresh("z"), t, ObjKind::kZeroPage};
      o.init = value(t);
      globals.push_back(std::move(o));
    }
    {
      const std::string t = typ();
      Obj o{fresh("ga"), t, ObjKind::kArray};
      for (int i = 0; i < 4; ++i) o.elements.push_back(value(t));
      globals.push_back(std::move(o));
    }
    {
      std::vector<Member> members;
      for (int i = 0; i < 2; ++i) members.push_back({"m" + std::to_string(i), typ()});
      Obj o{fresh("gs"), "", ObjKind::kStruct};
      for (Member& m : members) m.init = value(m.type);
      o.members = std::move(members);
      globals.push_back(std::move(o));
    }
    for (Obj& g : globals) {
      if (g.kind == ObjKind::kGlobal || g.kind == ObjKind::kZeroPage) g.vol = vrng_.random() < 0.25;
    }
    std::vector<std::size_t> scalars;
    for (std::size_t i = 0; i < globals.size(); ++i) {
      if (globals[i].kind == ObjKind::kGlobal || globals[i].kind == ObjKind::kZeroPage) scalars.push_back(i);
    }
    const auto k = std::min(scalars.size(), static_cast<std::size_t>(r.randint(0, 2)));
    for (std::size_t pick : r.sample_indices(scalars.size(), k)) {
      Obj& target = globals[scalars[pick]];
      target.vol = false;
      Obj gp{fresh("gp"), target.type, ObjKind::kGlobalPointer};
      gp.pointee = target.name;
      globals.push_back(std::move(gp));
    }
    globals_ = globals;
    std::vector<Fn> fns;
    const int n_fns = static_cast<int>(r.randint(1, 3));
    for (int i = 0; i < n_fns; ++i) {
      Fn f = function(fns, false);
      fns.push_back(std::move(f));
    }
    Fn m = function(fns, true);
    fns.push_back(std::move(m));
    return {std::move(globals), std::move(fns)};
  }

 private:
  std::string fresh(const std::string& prefix) { return prefix + std::to_string(++n_); }

  std::string typ() { return weights_[rng_.choices_indices(weight_values_, 1)[0]].first; }

  Int value(const std::string& t) {
    const Int random_bits = rng_.getrandbits(types_.at(t).width);
    const Int low_bits = rng_.getrandbits(4);
    const std::size_t i = rng_.choice_index(std::size(kInteresting) + 2);
    const Int v = i < std::size(kInteresting) ? kInteresting[i]
                  : i == std::size(kInteresting) ? random_bits
                                                 : low_bits;
    return types_.wrap(v, t);
  }

  Fn function(const std::vector<Fn>& callees, bool is_main) {
    py_random::Random& r = rng_;
    Fn fn;
    fn.name = is_main ? "main" : fresh("f");
    if (!is_main) {
      const int n_params = static_cast<int>(r.randint(0, 3));
      for (int i = 0; i < n_params; ++i) {
        const std::string t = typ();
        fn.params.push_back(Obj{fresh("p"), t, ObjKind::kParam});
      }
      if (r.random() < 0.5) {
        const std::string name = fresh("pp");
        fn.params.push_back(Obj{name, typ(), ObjKind::kPointerParam});
      }
    }
    const int n_locals = static_cast<int>(r.randint(1, 3));
    for (int i = 0; i < n_locals; ++i) {
      const std::string t = typ();
      Obj o{fresh("l"), t, ObjKind::kLocal};
      o.init = value(t);
      fn.locals.push_back(std::move(o));
    }
    for (const Fn& c : callees) {
      for (const Obj& p : c.params) {
        if (p.kind != ObjKind::kPointerParam) continue;
        const bool have = std::any_of(fn.locals.begin(), fn.locals.end(), [&](const Obj& o) {
          return o.type == p.type && o.kind == ObjKind::kLocal;
        });
        if (!have) {
          Obj o{fresh("l"), p.type, ObjKind::kLocal};
          o.init = value(p.type);
          fn.locals.push_back(std::move(o));
        }
      }
    }
    if (!is_main && r.random() < 0.6) {
      const std::string t = typ();
      Obj o{fresh("s"), t, ObjKind::kStatic};
      o.init = value(t);
      fn.statics.push_back(std::move(o));
    }
    std::vector<std::string> by_ptr;
    for (const Fn& c : callees) {
      for (const Obj& p : c.params) {
        if (p.kind == ObjKind::kPointerParam) by_ptr.push_back(p.type);
      }
    }
    auto pick_volatile = [&](Obj& o) {
      if (std::find(by_ptr.begin(), by_ptr.end(), o.type) == by_ptr.end()) o.vol = vrng_.random() < 0.25;
    };
    for (Obj& o : fn.locals) pick_volatile(o);
    for (Obj& o : fn.statics) pick_volatile(o);
    fn.ret = is_main ? "int" : typ();
    fn_ = &fn;
    callees_ = &callees;
    counters_in_scope_.clear();
    budget_ = kStmtsPerFnUnderFrameLimit;
    const int n = static_cast<int>(r.randint(3, 6));
    fn.body = block(n, Context{});
    if (!is_main) {
      Stmt ret;
      ret.kind = StmtKind::kReturn;
      ret.value = expr(1);
      fn.body.push_back(std::move(ret));
    }
    fn_ = nullptr;
    return fn;
  }

  std::vector<Writable> lvalues() {
    std::vector<Writable> out;
    const Fn& f = *fn_;
    for (const Obj& o : f.locals) out.push_back({var(o.name), o.type, o.name});
    for (const Obj& o : f.statics) out.push_back({var(o.name), o.type, o.name});
    for (const Obj& o : f.params) {
      if (o.kind == ObjKind::kParam) {
        out.push_back({var(o.name), o.type, o.name});
      } else {
        out.push_back({Lvalue{Lvalue::Kind::kDeref, o.name, "", nullptr}, o.type, "*" + o.name});
      }
    }
    for (const Obj& g : globals_) {
      switch (g.kind) {
        case ObjKind::kGlobal:
        case ObjKind::kZeroPage:
          out.push_back({var(g.name), g.type, g.name});
          break;
        case ObjKind::kArray:
          out.push_back({Lvalue{Lvalue::Kind::kIndex, g.name, "", index()}, g.type, g.name});
          break;
        case ObjKind::kStruct:
          for (const Member& m : g.members) {
            out.push_back({Lvalue{Lvalue::Kind::kMember, g.name, m.name, nullptr}, m.type,
                           g.name + "." + m.name});
          }
          break;
        case ObjKind::kGlobalPointer:
          out.push_back({Lvalue{Lvalue::Kind::kDeref, g.name, "", nullptr}, g.type, g.pointee});
          break;
        default:
          break;
      }
    }
    return out;
  }

  std::vector<Writable> lvalues_except(const std::optional<std::string>& avoid) {
    std::vector<Writable> out;
    for (Writable& w : lvalues()) {
      if (!avoid || w.key != *avoid) out.push_back(std::move(w));
    }
    return out;
  }

  ExprPtr index() {
    if (!counters_in_scope_.empty() && rng_.random() < 0.6) {
      const std::string& c = rng_.choice(counters_in_scope_);
      return with_operands(ExprKind::kBinary, "&", {read(var(c)), constant(3, "int")});
    }
    return constant(rng_.randint(0, 3), "int");
  }

  ExprPtr expr(int depth, const std::optional<std::string>& avoid = std::nullopt) {
    py_random::Random& r = rng_;
    if (depth == 0 || r.random() < 0.3) {
      const std::vector<Writable> choices = lvalues_except(avoid);
      if (!counters_in_scope_.empty() && r.random() < 0.2) {
        return read(var(r.choice(counters_in_scope_)));
      }
      if (!choices.empty() && r.random() < 0.75) return read(r.choice(choices).lv);
      const std::string t = typ();
      return constant(value(t), t);
    }
    const double k = r.random();
    if (k < 0.1) {
      ExprPtr a = expr(depth - 1, avoid);
      Expr e;
      e.kind = ExprKind::kCast;
      e.cast_to = typ();
      e.operands = {std::move(a)};
      return make(std::move(e));
    }
    if (k < 0.18) {
      ExprPtr a = expr(depth - 1, avoid);
      static const std::vector<std::string> kUnary = {"-", "~", "!"};
      return with_operands(ExprKind::kUnary, r.choice(kUnary), {std::move(a)});
    }
    if (k < 0.28) {
      ExprPtr a = expr(depth - 1, avoid);
      static const std::vector<std::string> kShifts = {"<<", ">>"};
      Expr e;
      e.kind = ExprKind::kShift;
      e.op = r.choice(kShifts);
      e.shift_count = static_cast<int>(r.randint(0, 7));
      e.operands = {std::move(a)};
      return make(std::move(e));
    }
    if (k < 0.4) {
      ExprPtr a = expr(depth - 1, avoid);
      ExprPtr b = expr(depth - 1, avoid);
      static const std::vector<std::string> kCompares = {"<", ">", "<=", ">=", "==", "!="};
      return with_operands(ExprKind::kCompare, r.choice(kCompares), {std::move(a), std::move(b)});
    }
    if (k < 0.45) {
      ExprPtr a = expr(depth - 1, avoid);
      ExprPtr b = expr(depth - 1, avoid);
      static const std::vector<std::string> kLogic = {"&&", "||"};
      return with_operands(ExprKind::kLogic, r.choice(kLogic), {std::move(a), std::move(b)});
    }
    if (k < 0.5) {
      ExprPtr c = expr(depth - 1, avoid);
      ExprPtr a = expr(depth - 1, avoid);
      ExprPtr b = expr(depth - 1, avoid);
      return with_operands(ExprKind::kCond, "", {std::move(c), std::move(a), std::move(b)});
    }
    ExprPtr a = expr(depth - 1, avoid);
    ExprPtr b = expr(depth - 1, avoid);
    static const std::vector<std::string> kBinary = {"+", "-", "*", "&", "|", "^",
                                                     "+", "-", "&", "/", "%"};
    const std::string op = r.choice(kBinary);
    if (op == "/" || op == "%") b = constant(r.randint(1, 7), "int");
    return with_operands(ExprKind::kBinary, op, {std::move(a), std::move(b)});
  }

  std::optional<SideOp> side_op() {
    const std::vector<Writable> choices = lvalues();
    if (choices.empty()) return std::nullopt;
    const Writable& w = rng_.choice(choices);
    Expr e;
    e.kind = ExprKind::kOp;
    e.target = w.lv;
    e.op_kind = kOpKinds[rng_.choice_index(std::size(kOpKinds))];
    if (e.op_kind == OpKind::kShl || e.op_kind == OpKind::kShr) {
      e.right_operand = static_cast<int>(rng_.randint(0, 7));
    } else {
      static const std::vector<int> kOperands = {1, 3, 5, 0x5A, 0x81, 0xFF};
      e.right_operand = rng_.choice(kOperands);
    }
    return SideOp{make(std::move(e)), w.key};
  }

  WithOp with_op(int depth) {
    py_random::Random& r = rng_;
    if (r.random() < 0.5) return {expr(depth), std::nullopt};
    std::optional<SideOp> so = side_op();
    if (!so) return {expr(depth), std::nullopt};
    if (r.random() < 0.5) return {so->e, so->key};
    ExprPtr rest = expr(depth - 1, so->key);
    static const std::vector<std::string> kOps = {"+", "-", "^", "&", "|"};
    const std::string bop = r.choice(kOps);
    if (r.random() < 0.5) return {with_operands(ExprKind::kBinary, bop, {so->e, rest}), so->key};
    return {with_operands(ExprKind::kBinary, bop, {rest, so->e}), so->key};
  }

  int op_depth() {
    static const std::vector<int> kDepths = {1, 1, 2};
    return rng_.choice(kDepths);
  }

  ExprPtr call() {
    if (callees_->empty()) return nullptr;
    const Fn& f = rng_.choice(*callees_);
    Expr e;
    e.kind = ExprKind::kCall;
    e.callee = f.name;
    for (const Obj& p : f.params) {
      if (p.kind == ObjKind::kPointerParam) {
        std::vector<std::string> locals;
        for (const Obj& o : fn_->locals) {
          if (o.type == p.type) locals.push_back(o.name);
        }
        Expr addr;
        addr.kind = ExprKind::kAddress;
        addr.address_of = rng_.choice(locals);
        e.operands.push_back(make(std::move(addr)));
      } else {
        e.operands.push_back(expr(1));
      }
    }
    return make(std::move(e));
  }

  Block block(int n, const Context& ctx) {
    Block out;
    while (static_cast<int>(out.size()) < n && budget_ > 0) {
      --budget_;
      out.push_back(statement(ctx));
    }
    return out;
  }

  Stmt statement(const Context& ctx) {
    py_random::Random& r = rng_;
    const double k = r.random();
    const bool nested = ctx.depth < 3;
    Stmt s;
    const Context inner{ctx.depth + 1, ctx.loops, ctx.breakable, ctx.in_loop};
    if (nested && k < 0.12) {
      s.kind = StmtKind::kIf;
      s.cond = with_op(op_depth()).e;
      s.then = block(static_cast<int>(r.randint(1, 3)), inner);
      if (r.random() < 0.5) s.els = block(static_cast<int>(r.randint(1, 2)), inner);
      return s;
    }
    if (nested && ctx.loops < 2 && k < 0.24) {
      s.count = static_cast<int>(r.randint(1, 4));
      static const std::vector<StmtKind> kLoops = {StmtKind::kFor, StmtKind::kWhile, StmtKind::kDo};
      s.kind = r.choice(kLoops);
      s.counter = fresh(s.kind == StmtKind::kFor ? "i" : "w");
      if (s.kind != StmtKind::kFor) fn_->counters.push_back(s.counter);
      counters_in_scope_.push_back(s.counter);
      s.body = block(static_cast<int>(r.randint(1, 4)), Context{ctx.depth + 1, ctx.loops + 1, true, true});
      counters_in_scope_.erase(std::find(counters_in_scope_.begin(), counters_in_scope_.end(), s.counter));
      return s;
    }
    if (nested && k < 0.32) {
      s.kind = StmtKind::kSwitch;
      ExprPtr e = with_op(op_depth()).e;
      Expr cast;
      cast.kind = ExprKind::kCast;
      cast.cast_to = "int";
      cast.operands = {std::move(e)};
      s.selector = with_operands(ExprKind::kBinary, "&", {make(std::move(cast)), constant(7, "int")});
      const auto n_values = static_cast<std::size_t>(r.randint(1, 4));
      std::vector<std::size_t> values = r.sample_indices(8, n_values);
      std::sort(values.begin(), values.end());
      const Context in_switch{ctx.depth + 1, ctx.loops, true, ctx.in_loop};
      for (std::size_t v : values) {
        SwitchCase c;
        c.value = static_cast<int>(v);
        c.body = block(static_cast<int>(r.randint(0, 2)), in_switch);
        c.breaks = r.random() < 0.6;
        s.cases.push_back(std::move(c));
      }
      if (r.random() < 0.6) s.default_body = block(static_cast<int>(r.randint(1, 2)), in_switch);
      return s;
    }
    if (ctx.breakable && k < 0.36) {
      s.kind = StmtKind::kBreak;
      return s;
    }
    if (ctx.in_loop && k < 0.39) {
      s.kind = StmtKind::kContinue;
      return s;
    }
    if (fn_->name != "main" && ctx.depth > 0 && k < 0.43) {
      s.kind = StmtKind::kReturn;
      s.value = with_op(op_depth()).e;
      return s;
    }
    if (k < 0.5) {
      if (ExprPtr c = call()) {
        const std::string local = r.choice(fn_->locals).name;
        static const std::vector<StmtKind> kUses = {StmtKind::kExpr, StmtKind::kMix, StmtKind::kAssign};
        s.kind = r.choice(kUses);
        s.value = std::move(c);
        if (s.kind == StmtKind::kAssign) s.lv = var(local);
        return s;
      }
    }
    if (k < 0.62) {
      s.kind = StmtKind::kMix;
      s.value = with_op(op_depth()).e;
      return s;
    }
    if (k < 0.78) {
      if (std::optional<SideOp> so = side_op()) {
        s.kind = StmtKind::kExpr;
        s.value = so->e;
        return s;
      }
    }
    WithOp w = with_op(op_depth());
    const std::vector<Writable> targets = lvalues_except(w.written);
    s.kind = StmtKind::kAssign;
    s.lv = r.choice(targets).lv;
    s.value = std::move(w.e);
    return s;
  }

  py_random::Random& rng_;
  // Picks which objects are `volatile`. It's separate from `rng_` so
  // that adding the qualifier leaves each seed's program otherwise the
  // same.
  py_random::Random& vrng_;
  const Types& types_;
  std::vector<std::pair<std::string, int>> weights_;
  std::vector<double> weight_values_;
  int n_ = 0;
  int zp_bytes_ = 0;
  std::vector<Obj> globals_;
  Fn* fn_ = nullptr;
  const std::vector<Fn>* callees_ = nullptr;
  std::vector<std::string> counters_in_scope_;
  int budget_ = 0;
};

}  // namespace

GeneratedProgram make_program(Int seed) {
  TypeTable table = standard_types();
  const std::vector<std::string> extra = choose_bit_precise(table, seed, 2);
  Types types(table);
  for (int attempt = 0; attempt < kAttempts; ++attempt) {
    py_random::Random rng(seed * 1000 + attempt);
    py_random::Random vrng(seed * 1000 + attempt + 500);
    const Prog p = Generator(rng, vrng, types, extra).program();
    Interp interp(p, types);
    try {
      interp.run_program(p);
    } catch (const UndefinedBehavior&) {
      continue;
    } catch (const PastStepLimit&) {
      continue;
    }
    const std::int64_t expected = interp.acc() & 0x7FFF;
    return {render(p, expected, types), expected};
  }
  throw std::runtime_error("seed " + decimal(seed) + ": no program that ends without undefined behavior");
}

}  // namespace c6502::fuzz::stmts
