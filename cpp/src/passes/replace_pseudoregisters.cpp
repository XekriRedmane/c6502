
#include "passes/replace_pseudoregisters.h"

#include <set>
#include <stdexcept>
#include <variant>

#include "errors.h"
#include "passes/decimal_arith.h"
#include "pyrepr.h"

namespace c6502::passes {

namespace c99 = ::c6502::c99_ast;
namespace ir = ::c6502::asm_ast;

asdl::Int sizeof_type(const c99::Type_data_type& t, const TypeTable* types) {
  using K = c99::Kind_data_type;
  switch (c99::kind(t)) {
    case K::Const:
      return sizeof_type(*std::get<c99::Const>(t).referenced_type, types);
    case K::Volatile:
      return sizeof_type(*std::get<c99::Volatile>(t).referenced_type, types);
    case K::Char:
    case K::SChar:
    case K::UChar:
      return 1;
    case K::Int:
    case K::UInt:
    case K::Pointer:
      return 2;
    case K::Long:
    case K::ULong:
    case K::Float:
      return 4;
    case K::LongLong:
    case K::ULongLong:
    case K::Double:
      return 8;
    case K::BitInt:
      return asdl::floor_div(std::get<c99::BitInt>(t).width + 7, 8);
    case K::Array: {
      const auto& a = std::get<c99::Array>(t);
      return sizeof_type(*a.element_type, types) * a.size;
    }
    case K::Structure:
    case K::Union: {
      if (types == nullptr) {
        return 1;
      }
      const std::string& tag = c99::kind(t) == K::Structure ? std::get<c99::Structure>(t).tag
                                                            : std::get<c99::Union>(t).tag;
      const StructLayout* layout = types->find(tag);
      if (layout == nullptr || !layout->complete) {
        return 1;
      }
      return layout->size;
    }
    default:
      return 1;
  }
}

asdl::Int size_of_name(std::string_view name, const SymbolTable* symbols, const TypeTable* types) {
  if (symbols == nullptr) {
    return 1;
  }
  const Symbol* sym = symbols->find(name);
  if (sym == nullptr) {
    return 1;
  }
  return sizeof_type(sym->type, types);
}

namespace {

using NameMap = std::map<std::string, asdl::Int, std::less<>>;

template <class Fn>
void operands_in(const ir::Type_instruction& instr, Fn&& fn) {
  using K = ir::Kind_instruction;
  switch (ir::kind(instr)) {
    case K::DecimalArith:
      for (const auto& op : decimal_arith::operands(std::get<ir::DecimalArith>(instr))) {
        fn(op);
      }
      return;
    case K::Mov: {
      const auto& i = std::get<ir::Mov>(instr);
      fn(i.src);
      fn(i.dst);
      return;
    }
    case K::Add: {
      const auto& i = std::get<ir::Add>(instr);
      fn(i.src);
      fn(i.dst);
      return;
    }
    case K::Sub: {
      const auto& i = std::get<ir::Sub>(instr);
      fn(i.src);
      fn(i.dst);
      return;
    }
    case K::And: {
      const auto& i = std::get<ir::And>(instr);
      fn(i.src);
      fn(i.dst);
      return;
    }
    case K::Or: {
      const auto& i = std::get<ir::Or>(instr);
      fn(i.src);
      fn(i.dst);
      return;
    }
    case K::Xor: {
      const auto& i = std::get<ir::Xor>(instr);
      fn(i.src1);
      fn(i.src2);
      fn(i.dst);
      return;
    }
    case K::Inc:
      fn(std::get<ir::Inc>(instr).dst);
      return;
    case K::Dec:
      fn(std::get<ir::Dec>(instr).dst);
      return;
    case K::ArithmeticShiftLeft:
      fn(std::get<ir::ArithmeticShiftLeft>(instr).dst);
      return;
    case K::LogicalShiftRight:
      fn(std::get<ir::LogicalShiftRight>(instr).dst);
      return;
    case K::RotateLeft:
      fn(std::get<ir::RotateLeft>(instr).dst);
      return;
    case K::RotateRight:
      fn(std::get<ir::RotateRight>(instr).dst);
      return;
    case K::Push:
      fn(std::get<ir::Push>(instr).src);
      return;
    case K::Pop:
      fn(std::get<ir::Pop>(instr).dst);
      return;
    case K::Compare: {
      const auto& i = std::get<ir::Compare>(instr);
      fn(i.left);
      fn(i.right);
      return;
    }
    case K::LoadAddress:
      fn(std::get<ir::LoadAddress>(instr).src);
      return;
    case K::Phi:
      throw InternalError("replace_pseudoregisters: Phi node leaked past SSA destruction");
    default:
      return;
  }
}

class Replacer {
 public:
  Replacer(const std::vector<std::string>& params, const NameSet& statics,
           const SymbolTable* symbols, const TypeTable* types, const Coloring* coloring,
           const ParamLayout* param_layout, std::set<asdl::Int> private_pool_addrs,
           const OrderedMap<std::string, asdl::Int>* address_taken_zp,
           const OrderedMap<std::string, std::string>* address_taken_symbols)
      : params_(params),
        param_set_(params.begin(), params.end()),
        statics_(statics),
        symbols_(symbols),
        types_(types),
        coloring_(coloring),
        zp_layout_(param_layout != nullptr ? std::get_if<ZpLayout>(param_layout) : nullptr),
        private_pool_addrs_(std::move(private_pool_addrs)),
        address_taken_zp_(address_taken_zp),
        address_taken_symbols_(address_taken_symbols) {
    callee_saved_addrs_ = compute_callee_saved_addrs();
    local_total_ = static_cast<asdl::Int>(callee_saved_addrs_.size());
  }

  const std::vector<asdl::Int>& callee_saved_addrs() const { return callee_saved_addrs_; }

  void discover(const ir::Type_operand& op) {
    const auto* p = asdl::as<ir::Pseudo>(op);
    if (p == nullptr) {
      return;
    }
    const std::string& name = p->name;
    if (statics_.contains(name) || param_set_.contains(name) || is_colored(name) ||
        in_address_taken_zp(name) || local_bases_.contains(name)) {
      return;
    }
    const asdl::Int size = size_of_name(name, symbols_, types_);
    local_bases_.emplace(name, local_total_ + 1);
    local_total_ += size;
  }

  std::pair<asdl::Int, asdl::Int> finalize() {
    const asdl::Int m = local_total_;
    if (zp_layout_ != nullptr) {
      asdl::Int flat = 0;
      for (const auto& name : params_) {
        param_flat_offsets_.insert_or_assign(name, flat);
        flat += size_of_name(name, symbols_, types_);
      }
      return {0, m};
    }
    asdl::Int cursor = m + 3;
    for (const auto& name : params_) {
      param_bases_.insert_or_assign(name, cursor);
      cursor += size_of_name(name, symbols_, types_);
    }
    return {cursor - (m + 3), m};
  }

  ir::Type_operand replace(const ir::Type_operand& op) const {
    const auto* p = asdl::as<ir::Pseudo>(op);
    if (p == nullptr) {
      return op;
    }
    const std::string& name = p->name;
    if (statics_.contains(name)) {
      return ir::Data{.name = name, .offset = p->offset};
    }
    if (const asdl::Int* flat = param_flat_offsets_.find(name)) {
      const asdl::Int index = *flat + p->offset;
      if (index < 0 || index >= static_cast<asdl::Int>(zp_layout_->slot_symbols.size())) {
        throw InternalError("replace_pseudoregisters: list index out of range");
      }
      return ir::Data{.name = zp_layout_->slot_symbols[static_cast<std::size_t>(index)],
                      .offset = 0};
    }
    if (const asdl::Int* base = param_bases_.find(name)) {
      return ir::Frame{.offset = *base + p->offset};
    }
    if (is_colored(name)) {
      return ir::ZP{.address = *coloring_->assignments.find(name), .offset = p->offset};
    }
    if (in_address_taken_zp(name)) {
      if (address_taken_symbols_ != nullptr) {
        if (const std::string* slot = address_taken_symbols_->find(name)) {
          return ir::Data{.name = *slot, .offset = p->offset};
        }
      }
      return ir::ZP{.address = *address_taken_zp_->find(name), .offset = p->offset};
    }
    if (const auto it = local_bases_.find(name); it != local_bases_.end()) {
      return ir::Frame{.offset = it->second + p->offset};
    }
    throw InternalError("Pseudo(" + pyrepr::string(name) +
                        ") is neither a static, a colored local, an ordinary local, nor a "
                        "declared parameter; check tac_to_asm output");
  }

  ir::Type_instruction replace_instruction(const ir::Type_instruction& instr, asdl::Int arg_bytes,
                                           asdl::Int local_bytes) const {
    using K = ir::Kind_instruction;
    const auto r = [this](const ir::Type_operand& op) { return replace(op); };
    switch (ir::kind(instr)) {
      case K::DecimalArith:
        return decimal_arith::map_operands(std::get<ir::DecimalArith>(instr), r);
      case K::Mov: {
        const auto& i = std::get<ir::Mov>(instr);
        return ir::Mov{.src = r(i.src), .dst = r(i.dst), .is_volatile = i.is_volatile};
      }
      case K::Add: {
        const auto& i = std::get<ir::Add>(instr);
        return ir::Add{.src = r(i.src), .dst = r(i.dst)};
      }
      case K::Sub: {
        const auto& i = std::get<ir::Sub>(instr);
        return ir::Sub{.src = r(i.src), .dst = r(i.dst)};
      }
      case K::And: {
        const auto& i = std::get<ir::And>(instr);
        return ir::And{.src = r(i.src), .dst = r(i.dst)};
      }
      case K::Or: {
        const auto& i = std::get<ir::Or>(instr);
        return ir::Or{.src = r(i.src), .dst = r(i.dst)};
      }
      case K::Xor: {
        const auto& i = std::get<ir::Xor>(instr);
        return ir::Xor{.src1 = r(i.src1), .src2 = r(i.src2), .dst = r(i.dst)};
      }
      case K::Inc:
        return ir::Inc{.dst = r(std::get<ir::Inc>(instr).dst)};
      case K::Dec:
        return ir::Dec{.dst = r(std::get<ir::Dec>(instr).dst)};
      case K::ArithmeticShiftLeft:
        return ir::ArithmeticShiftLeft{.dst = r(std::get<ir::ArithmeticShiftLeft>(instr).dst)};
      case K::LogicalShiftRight:
        return ir::LogicalShiftRight{.dst = r(std::get<ir::LogicalShiftRight>(instr).dst)};
      case K::RotateLeft:
        return ir::RotateLeft{.dst = r(std::get<ir::RotateLeft>(instr).dst)};
      case K::RotateRight:
        return ir::RotateRight{.dst = r(std::get<ir::RotateRight>(instr).dst)};
      case K::Push:
        return ir::Push{.src = r(std::get<ir::Push>(instr).src)};
      case K::Pop:
        return ir::Pop{.dst = r(std::get<ir::Pop>(instr).dst)};
      case K::Compare: {
        const auto& i = std::get<ir::Compare>(instr);
        return ir::Compare{.left = r(i.left), .right = r(i.right)};
      }
      case K::Ret: {
        const auto& i = std::get<ir::Ret>(instr);
        return ir::Ret{.arg_bytes = arg_bytes,
                       .local_bytes = local_bytes,
                       .save_a = i.save_a,
                       .save_x = i.save_x,
                       .callee_saved_addrs = callee_saved_addrs_,
                       .save_c = i.save_c};
      }
      case K::LoadAddress: {
        const auto& i = std::get<ir::LoadAddress>(instr);
        return ir::LoadAddress{.src = r(i.src), .dst = r(i.dst)};
      }
      default:
        return instr;
    }
  }

 private:
  std::vector<asdl::Int> compute_callee_saved_addrs() const {
    if (coloring_ == nullptr || coloring_->assignments.empty()) {
      return {};
    }
    const Range callee_range = coloring_->pool.callee_saved();
    std::set<asdl::Int> used;
    for (const auto& [name, base] : coloring_->assignments) {
      const asdl::Int width = size_of_name(name, symbols_, types_);
      for (asdl::Int k = 0; k < width; ++k) {
        const asdl::Int byte_addr = base + k;
        if (callee_range.contains(byte_addr) && !private_pool_addrs_.contains(byte_addr)) {
          used.insert(byte_addr);
        }
      }
    }
    return {used.begin(), used.end()};
  }

  bool is_colored(std::string_view name) const {
    return coloring_ != nullptr && coloring_->assignments.contains(name);
  }

  bool in_address_taken_zp(std::string_view name) const {
    return address_taken_zp_ != nullptr && address_taken_zp_->contains(name);
  }

  const std::vector<std::string>& params_;
  std::set<std::string, std::less<>> param_set_;
  const NameSet& statics_;
  const SymbolTable* symbols_;
  const TypeTable* types_;
  const Coloring* coloring_;
  const ZpLayout* zp_layout_;
  std::set<asdl::Int> private_pool_addrs_;
  const OrderedMap<std::string, asdl::Int>* address_taken_zp_;
  const OrderedMap<std::string, std::string>* address_taken_symbols_;
  std::vector<asdl::Int> callee_saved_addrs_;
  NameMap local_bases_;
  asdl::Int local_total_ = 0;
  OrderedMap<std::string, asdl::Int> param_flat_offsets_;
  OrderedMap<std::string, asdl::Int> param_bases_;
};

bool is_shift(const ir::Type_instruction& instr) {
  return asdl::isa<ir::ArithmeticShiftLeft, ir::LogicalShiftRight, ir::RotateLeft,
                   ir::RotateRight>(instr);
}

const ir::Type_operand& shift_dst(const ir::Type_instruction& instr) {
  using K = ir::Kind_instruction;
  switch (ir::kind(instr)) {
    case K::ArithmeticShiftLeft:
      return std::get<ir::ArithmeticShiftLeft>(instr).dst;
    case K::LogicalShiftRight:
      return std::get<ir::LogicalShiftRight>(instr).dst;
    case K::RotateLeft:
      return std::get<ir::RotateLeft>(instr).dst;
    default:
      return std::get<ir::RotateRight>(instr).dst;
  }
}

ir::Type_instruction shift_of_a(const ir::Type_instruction& instr) {
  const ir::Type_operand a = ir::Reg{.reg = ir::A{}};
  using K = ir::Kind_instruction;
  switch (ir::kind(instr)) {
    case K::ArithmeticShiftLeft:
      return ir::ArithmeticShiftLeft{.dst = a};
    case K::LogicalShiftRight:
      return ir::LogicalShiftRight{.dst = a};
    case K::RotateLeft:
      return ir::RotateLeft{.dst = a};
    default:
      return ir::RotateRight{.dst = a};
  }
}

bool is_soft_stack(const ir::Type_operand& op) { return asdl::isa<ir::Frame, ir::Stack>(op); }

// The 6502 has no `(zp),Y` form of ASL, LSR, ROL, or ROR, so `asm_emit`
// would write this sequence anyway, and spelling it out here lets every
// later pass see that A is overwritten.
std::vector<ir::Type_instruction> expand_soft_stack_shifts(std::vector<ir::Type_instruction> instrs) {
  std::vector<ir::Type_instruction> out;
  out.reserve(instrs.size());
  const ir::Type_operand reg_a = ir::Reg{.reg = ir::A{}};
  for (auto& instr : instrs) {
    if (is_shift(instr) && is_soft_stack(shift_dst(instr))) {
      const ir::Type_operand byte = shift_dst(instr);
      out.push_back(ir::Mov{.src = byte, .dst = reg_a});
      out.push_back(shift_of_a(instr));
      out.push_back(ir::Mov{.src = reg_a, .dst = byte});
      continue;
    }
    if (const auto* mov = asdl::as<ir::Mov>(instr)) {
      const auto* src = asdl::as<ir::Reg>(mov->src);
      if (src != nullptr && asdl::isa<ir::X, ir::Y>(src->reg) && is_soft_stack(mov->dst)) {
        // There's no `STX (zp),Y`, so `asm_emit` writes this as `TXA;
        // STA (FP),Y`, and A is overwritten too.
        out.push_back(ir::Mov{.src = mov->src, .dst = reg_a});
        out.push_back(ir::Mov{.src = reg_a, .dst = mov->dst, .is_volatile = mov->is_volatile});
        continue;
      }
    }
    out.push_back(std::move(instr));
  }
  return out;
}

struct FunctionTables {
  const ParamLayout* param_layout = nullptr;
  std::set<asdl::Int> private_pool_addrs;
  const OrderedMap<std::string, asdl::Int>* address_taken_zp = nullptr;
  const OrderedMap<std::string, std::string>* address_taken_symbols = nullptr;
};

FrameDims replace_function(ir::Function& fn, const NameSet& statics, const SymbolTable* symbols,
                           const TypeTable* types, const Coloring* coloring, bool bare_exit,
                           FunctionTables tables) {
  Replacer r(fn.params, statics, symbols, types, coloring, tables.param_layout,
             std::move(tables.private_pool_addrs), tables.address_taken_zp,
             tables.address_taken_symbols);
  for (const auto& instr : fn.instructions) {
    operands_in(instr, [&](const ir::Type_operand& op) { r.discover(op); });
  }
  const auto [arg_bytes, local_bytes] = r.finalize();
  FrameDims dims{.arg_bytes = arg_bytes,
                 .local_bytes = local_bytes,
                 .callee_saved_addrs = r.callee_saved_addrs()};
  std::vector<ir::Type_instruction> replaced;
  replaced.reserve(fn.instructions.size() + 1);
  if (!bare_exit) {
    replaced.push_back(ir::FunctionPrologue{.arg_bytes = arg_bytes,
                                            .local_bytes = local_bytes,
                                            .callee_saved_addrs = r.callee_saved_addrs()});
  }
  std::vector<ir::Type_instruction> body;
  body.reserve(fn.instructions.size());
  for (const auto& instr : fn.instructions) {
    body.push_back(r.replace_instruction(instr, arg_bytes, local_bytes));
  }
  for (auto& instr : expand_soft_stack_shifts(std::move(body))) {
    replaced.push_back(std::move(instr));
  }
  fn.instructions = std::move(replaced);
  return dims;
}

NameSet statics_of(const ir::Program& prog, const NameSet& extra_statics) {
  NameSet statics = extra_statics;
  for (const auto& tl : prog.top_level) {
    if (const auto* fn = asdl::as<ir::Function>(tl)) {
      statics.insert(fn->name);
    } else if (const auto* sv = asdl::as<ir::StaticVariable>(tl)) {
      statics.insert(sv->name);
    }
  }
  return statics;
}

}  // namespace

ir::Program replace_program(ir::Program prog, const NameSet& extra_statics,
                            const SymbolTable* symbols, const TypeTable* types,
                            const ColoringsByFunction* colorings) {
  const NameSet statics = statics_of(prog, extra_statics);
  for (auto& tl : prog.top_level) {
    auto* fn = asdl::as<ir::Function>(tl);
    if (fn == nullptr) {
      continue;
    }
    const Coloring* coloring = colorings != nullptr ? colorings->find(fn->name) : nullptr;
    replace_function(*fn, statics, symbols, types, coloring, false, {});
  }
  return prog;
}

std::pair<ir::Program, FrameDimsByFunction> replace_program_bare_exit(
    ir::Program prog, const NameSet& extra_statics, const SymbolTable* symbols,
    const TypeTable* types, const ColoringsByFunction* colorings, const BareExitTables& tables) {
  const NameSet statics = statics_of(prog, extra_statics);
  FrameDimsByFunction dims_by_fn;
  for (auto& tl : prog.top_level) {
    auto* fn = asdl::as<ir::Function>(tl);
    if (fn == nullptr) {
      continue;
    }
    const Coloring* coloring = colorings != nullptr ? colorings->find(fn->name) : nullptr;
    FunctionTables fn_tables;
    if (tables.param_layouts != nullptr) {
      fn_tables.param_layout = tables.param_layouts->find(fn->name);
    }
    if (tables.local_pools != nullptr) {
      if (const auto* pool = tables.local_pools->find(fn->name)) {
        fn_tables.private_pool_addrs.insert(pool->begin(), pool->end());
      }
    }
    if (tables.address_taken_assignments != nullptr) {
      fn_tables.address_taken_zp = tables.address_taken_assignments->find(fn->name);
    }
    if (tables.address_taken_symbols != nullptr) {
      fn_tables.address_taken_symbols = tables.address_taken_symbols->find(fn->name);
    }
    FrameDims dims =
        replace_function(*fn, statics, symbols, types, coloring, true, std::move(fn_tables));
    dims_by_fn.insert_or_assign(fn->name, std::move(dims));
  }
  return {std::move(prog), std::move(dims_by_fn)};
}

}  // namespace c6502::passes
