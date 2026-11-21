// Part C1 of tac_to_asm: calls and the ABI, and memory and pointers.
#include <algorithm>
#include <utility>

#include "errors.h"
#include "passes/optimization_asm/parallel_copies.h"
#include "passes/pointer_restage.h"
#include "tac_to_asm_translator.h"

namespace c6502::tac_to_asm {

using ::c6502::asdl::as;
using ::c6502::asdl::isa;

namespace {

void append(Instrs& out, Instrs more) {
  out.insert(out.end(), std::make_move_iterator(more.begin()), std::make_move_iterator(more.end()));
}

// Returns the registers ("X", "Y") the function `fn_name` leaves
// unchanged, from `[[c6502::preserves_x]]` and `preserves_y`, sorted.
std::vector<std::string> preserved_registers(const Translator& t, const std::string& fn_name) {
  if (t.symbols == nullptr) {
    return {};
  }
  const passes::Symbol* sym = t.symbols->find(fn_name);
  if (sym == nullptr) {
    return {};
  }
  const auto* fun = std::get_if<passes::FunAttr>(&sym->attrs);
  if (fun == nullptr) {
    return {};
  }
  return {fun->preserves.begin(), fun->preserves.end()};
}

// Returns whether `name` is an object with static storage (a file-scope
// object or a block-scope `static`) or a function. The address of
// either is a constant label, which is stored as `ImmLabelLow` and
// `ImmLabelHigh` immediates rather than with the compound `LoadAddress`.
// A function declared here but defined elsewhere has no other record in
// the asm program, so without the function case `fp = f;` stored the
// address of a soft-stack slot.
bool is_static_storage(const Translator& t, const std::string& name) {
  if (t.symbols == nullptr) {
    return false;
  }
  const passes::Symbol* sym = t.symbols->find(name);
  return sym != nullptr && (std::holds_alternative<passes::StaticAttr>(sym->attrs) ||
                            std::holds_alternative<passes::FunAttr>(sym->attrs));
}

// Returns the copy of the two address bytes of `ptr_op` into the DPTR
// pair in zero page. Each byte goes through A, the one register that
// loads from every kind of operand (Imm, Pseudo, Frame, Data, Indirect).
Instrs stage_dptr(const ir::Type_operand& ptr_op) {
  return {ir::Mov{.src = byte_at(ptr_op, 0), .dst = reg_a()},
          ir::Mov{.src = reg_a(), .dst = ir::Data{kDptr, 0}},
          ir::Mov{.src = byte_at(ptr_op, 1), .dst = reg_a()},
          ir::Mov{.src = reg_a(), .dst = ir::Data{kDptr, 1}}};
}

// Returns whether a load (or a store, `store`) of `n` bytes at `offset`
// through `ptr` copies the pointer into DPTR again before each byte
// after the first, under `-O`, so that every byte is read or written
// through the pointer in zero page (passes/pointer_restage.h has the
// conditions and the reason a store's earlier bytes can't change the
// pointer).
bool restage(const Translator& t, const tac::Type_val& ptr, const ir::Type_operand& ptr_op,
             asdl::Int offset, int n, bool store) {
  if (!(t.bare_exit && n > 1 && passes::pointer_restage::pointer_in_zero_page(ptr_op, t.ctx))) {
    return false;
  }
  if (!store) {
    return true;
  }
  const auto* var = as<tac::Var>(ptr);
  if (var == nullptr || t.symbols == nullptr) {
    return false;
  }
  const passes::Symbol* sym = t.symbols->find(var->name);
  return sym != nullptr && passes::pointer_restage::restage_store(&sym->type, offset, n, t.types);
}

// Returns the soft-stack argument writes, `AllocateStack(N)` and a Mov
// per byte into `Stack(off)`, packed from `Stack(1)` in source order,
// for a soft-stack callee or an indirect call.
Instrs softstack_arg_writes(const Translator& t, const std::vector<tac::Type_val>& args) {
  std::vector<int> sizes;
  int total = 0;
  for (const auto& a : args) {
    sizes.push_back(t.size_of(a));
    total += sizes.back();
  }
  if (total == 0) {
    return {};
  }
  Instrs out{ir::AllocateStack{.bytes = total}};
  int off = 1;
  for (std::size_t i = 0; i < args.size(); ++i) {
    const ir::Type_operand arg_op = translate_val(args[i]);
    for (int k = 0; k < sizes[i]; ++k) {
      out.push_back(ir::Mov{.src = byte_at(arg_op, k), .dst = ir::Stack{off + k}});
    }
    off += sizes[i];
  }
  return out;
}

// Returns the argument writes of a call to a zp_abi callee. A byte goes
// one of two ways.
//
// 1. A byte the callee takes in its slot (`param_registers[i]` none) is
//    `Mov(src, Data(slot_sym))`. These are ordered as a parallel copy,
//    so that swaps between zero-page bytes go through a temporary. A
//    byte already in the callee's slot (`same_slot`) isn't copied.
// 2. A byte the callee takes in a register ("A", "X", or "Y") is
//    `Mov(src, Reg)`, after all the slot writes, so the ordering of the
//    slot writes never sees a register destination. X and Y are loaded
//    before A, through A (`LDA src; TAX`), so A's load is last.
Instrs zp_arg_writes(Translator& t, const std::vector<tac::Type_val>& args,
                     const passes::ZpLayout& layout) {
  std::vector<ir::Mov> slot_movs;
  // Each register byte's register and source. The register decides the
  // order (X and Y before A), so the Movs are made after.
  std::vector<std::pair<std::string, ir::Type_operand>> reg_writes;
  std::size_t flat_idx = 0;
  for (const auto& arg : args) {
    const ir::Type_operand arg_op = translate_val(arg);
    const int sz = t.size_of(arg);
    for (int k = 0; k < sz; ++k) {
      const std::size_t i = flat_idx + static_cast<std::size_t>(k);
      const std::optional<std::string> target_reg =
          i < layout.param_registers.size() ? layout.param_registers[i] : std::nullopt;
      ir::Type_operand src = byte_at(arg_op, k);
      if (!target_reg) {
        if (i >= layout.slot_symbols.size()) {
          throw InternalError("list index out of range");
        }
        const std::string& sym = layout.slot_symbols[i];
        if (t.same_slot(src, sym)) {
          // The argument is already in the callee's bytes.
          continue;
        }
        slot_movs.push_back(ir::Mov{.src = std::move(src), .dst = ir::Data{sym, 0}});
      } else {
        reg_writes.emplace_back(*target_reg, std::move(src));
      }
    }
    flat_idx += static_cast<std::size_t>(sz);
  }
  // Each call site gets a name of its own for the cycle-break
  // temporaries the ordering may make, so the temporaries of different
  // call sites can't collide.
  ++t.zp_call_site_counter;
  int cycle_counter = 0;
  Instrs out;
  for (auto& m : passes::order_parallel_copies(
           std::move(slot_movs), "argwrite_" + std::to_string(t.zp_call_site_counter),
           cycle_counter)) {
    out.push_back(std::move(m));
  }
  // The register arguments, ordered so A's load is last. TAX and TAY
  // leave A alone, but the LDA before each transfer overwrites it.
  std::vector<std::pair<std::string, ir::Type_operand>> ordered;
  bool y_writes = false;
  std::vector<std::pair<std::string, ir::Type_operand>> a_writes;
  for (const char* r : {"X", "Y"}) {
    for (const auto& w : reg_writes) {
      if (w.first == r) {
        ordered.push_back(w);
        y_writes = y_writes || w.first == "Y";
      }
    }
  }
  for (const auto& w : reg_writes) {
    if (w.first == "A") {
      a_writes.push_back(w);
    }
  }
  // Loading A's argument last can still destroy Y's: when its source
  // ends up on the soft stack, the load is `LDY #off; LDA (FP),Y`. So
  // with a Y argument, A's argument is loaded first and kept on the
  // hardware stack until the others are in place. An immediate can't be
  // on the soft stack.
  bool park_a = false;
  if (y_writes) {
    for (const auto& w : a_writes) {
      park_a = park_a || !isa<ir::Imm>(w.second);
    }
  }
  if (park_a) {
    out.push_back(ir::Mov{.src = a_writes[0].second, .dst = reg_a()});
    out.push_back(ir::Push{.src = reg_a()});
    a_writes.clear();
  }
  for (const auto& w : a_writes) {
    ordered.push_back(w);
  }
  for (const auto& [r, src] : ordered) {
    if (r == "A") {
      out.push_back(ir::Mov{.src = src, .dst = reg_a()});
    } else {
      // Through A, then a transfer. The `direct_index_load` peephole
      // later makes `LDA src; TAX` an `LDX src` when `src` is an Imm, a
      // Data, or a ZP, so that case needs nothing here.
      out.push_back(ir::Mov{.src = src, .dst = reg_a()});
      out.push_back(ir::Mov{.src = reg_a(), .dst = r == "X" ? reg_x() : reg_y()});
    }
  }
  if (park_a) {
    out.push_back(ir::Pop{.dst = reg_a()});
  }
  return out;
}

// Returns the capture of a return value of `dst_size` bytes that comes
// back in registers or in the HARGS slot of its width.
Instrs capture_return(const Translator& t, const ir::Type_operand& dst_op, int dst_size,
                      int reg_bytes) {
  if (reg_bytes != 0) {
    return t.capture_reg_return(dst_op, reg_bytes);
  }
  // A wider return is read back a byte at a time through A from the
  // HARGS slot of its width: 2 bytes at +0..1, 4 at +8..11, and 8 at
  // +16..23.
  Instrs out;
  const int in_off = ret_hargs_offset(dst_size);
  for (int k = 0; k < dst_size; ++k) {
    out.push_back(ir::Mov{.src = hargs(in_off + k), .dst = reg_a()});
    out.push_back(ir::Mov{.src = reg_a(), .dst = byte_at(dst_op, k)});
  }
  return out;
}

}  // namespace

// The caller's side of the calling convention, by the callee's
// `ParamLayout`. With the soft stack (the default), each argument takes
// its size in consecutive stack bytes, packed in source order from
// `Stack(1)`, and the callee's epilogue moves SSP back, so the call site
// has no cleanup. With `ZpLayout`, each argument byte is written to the
// callee's slot or register, with no `AllocateStack`. A source in a
// caller's slot may share its byte with another argument's destination,
// which the ordering of the parallel copy handles.
//
// The capture of the return value is the same for both, and is the
// other side of `translate_ret`: a 1-byte value from A, an `int`, an
// `unsigned int`, or a pointer from A (low) and X (high), a 4-byte value
// from HARGS+8..11, and an 8-byte one from HARGS+16..23.
Instrs Translator::translate_function_call(const tac::FunctionCall& call) {
  const std::string& name = call.name;
  const passes::ParamLayout* layout = abi.find(name);
  const auto* zp = layout != nullptr ? std::get_if<passes::ZpLayout>(layout) : nullptr;
  Instrs emitted;
  std::vector<std::string> reg_args;
  if (zp != nullptr) {
    append(emitted, zp_arg_writes(*this, call.args, *zp));
    // The registers the callee reads as parameters, without repeats, in
    // the order of the parameters. They reach the asm liveness through
    // the Call, so that the `LDA #imm` of an argument in A isn't dropped
    // by `dead_a_arith`, which would otherwise take the Call to kill A
    // without reading it.
    for (const auto& r : zp->param_registers) {
      if (r && !r->empty() && std::find(reg_args.begin(), reg_args.end(), *r) == reg_args.end()) {
        reg_args.push_back(*r);
      }
    }
  } else {
    append(emitted, softstack_arg_writes(*this, call.args));
  }
  emitted.push_back(
      ir::Call{.name = name, .reg_args = reg_args, .preserves = preserved_registers(*this, name)});
  // A call with no destination is a call of a void callee, and nothing
  // is captured.
  if (!call.dst) {
    return emitted;
  }
  const ir::Type_operand dst_op = translate_val(*call.dst);
  if (returns_carry_struct(name)) {
    // A struct of a value and a `bool`: the value is stored from A (and
    // X), which leaves C alone, and then C is captured into the flag, the
    // struct's last byte.
    const int n = size_of(*call.dst) - 1;
    append(emitted, capture_reg_return(dst_op, n));
    emitted.push_back(ir::Mov{.src = ir::Imm{0}, .dst = reg_a()});
    emitted.push_back(ir::RotateLeft{.dst = reg_a()});
    emitted.push_back(ir::Mov{.src = reg_a(), .dst = byte_at(dst_op, n)});
  } else if (returns_carry(name)) {
    // The result is the carry flag. `LDA #0; ROL A` turns it into 0 or
    // 1, and `carry_branch_fold` turns the capture and a `BEQ` or `BNE`
    // on it into a `BCC` or `BCS`.
    emitted.push_back(ir::Mov{.src = ir::Imm{0}, .dst = reg_a()});
    emitted.push_back(ir::RotateLeft{.dst = reg_a()});
    emitted.push_back(ir::Mov{.src = reg_a(), .dst = byte_at(dst_op, 0)});
    const int size = size_of(*call.dst);
    for (int k = 1; k < size; ++k) {
      emitted.push_back(ir::Mov{.src = ir::Imm{0}, .dst = byte_at(dst_op, k)});
    }
  } else {
    const int dst_size = size_of(*call.dst);
    // The convention is keyed by the callee's declared return type, as
    // the callee's `translate_ret` decides where to put the value.
    const int reg_bytes = reg_return_bytes(fn_return_type(name), dst_size);
    append(emitted, capture_return(*this, dst_op, dst_size, reg_bytes));
  }
  return emitted;
}

// A call through a function pointer. The arguments go on the soft stack
// as for a direct call, and the call site copies the pointer's two bytes
// into DPTR and calls the runtime's `icall` trampoline (`icall: JMP
// (DPTR)`). The `JSR` pushes the return address as usual, the indirect
// `JMP` goes to the function, and its `RTS` returns past the `JSR
// icall`. DPTR is caller-saved like HARGS, so the callee may change it
// freely, and the staging keeps it intact up to the `JSR icall`. The
// capture of the return value is the direct path's.
Instrs Translator::translate_indirect_call(const tac::IndirectCall& call) {
  Instrs emitted = softstack_arg_writes(*this, call.args);
  append(emitted, stage_dptr(translate_val(call.ptr)));
  emitted.push_back(ir::Call{.name = kIcall});
  // The indirect callee has no name here, so the register convention is
  // keyed by the type of the result's temporary, which is the call's
  // return type, the type the callee's `translate_ret` sees.
  if (call.dst) {
    const ir::Type_operand dst_op = translate_val(*call.dst);
    const int dst_size = size_of(*call.dst);
    const int reg_bytes = reg_return_bytes(type_of_val(*call.dst), dst_size);
    append(emitted, capture_return(*this, dst_op, dst_size, reg_bytes));
  }
  return emitted;
}

// `&x`. For an object with static storage the address is a constant
// label, so the lowering is two Movs of `ImmLabelLow` and `ImmLabelHigh`
// into the bytes of `dst`. The asm SSA optimizer's copy propagation and
// dead-code elimination then see through the temporary `dst` to where
// the address goes (the Mov into a zp_abi pointer argument's slot, for
// one). For an automatic object the address is `FP + offset`, the
// compound `LoadAddress` that the emitter writes as `CLC; LDA; ADC`.
Instrs Translator::translate_get_address(const tac::GetAddress& instr) {
  const auto* var = as<tac::Var>(instr.operand);
  if (var == nullptr) {
    throw InternalError("GetAddress operand must be a Var (an lvalue name); got " +
                        asdl::dump(instr.operand));
  }
  const ir::Type_operand dst_op = translate_val(instr.dst);
  if (is_static_storage(*this, var->name)) {
    return {ir::Mov{.src = ir::ImmLabelLow{var->name, 0}, .dst = byte_at(dst_op, 0)},
            ir::Mov{.src = ir::ImmLabelHigh{var->name, 0}, .dst = byte_at(dst_op, 1)}};
  }
  return {ir::LoadAddress{.src = ir::Pseudo{var->name, 0}, .dst = dst_op}};
}

// `*p` read. The two bytes of `p` go into DPTR in zero page, and then
// the width of `dst` is read through `(DPTR),Y` into `dst`. The reads
// through DPTR carry the volatile bit of a volatile Load, and the
// staging Movs don't.
Instrs Translator::translate_load(const tac::Load& instr) {
  const ir::Type_operand ptr_op = translate_val(instr.src_ptr);
  const ir::Type_operand dst_op = translate_val(instr.dst);
  const int n = size_of(instr.dst);
  Instrs reads = indirect_read(dst_op, n, instr.is_volatile);
  if (restage(*this, instr.src_ptr, ptr_op, 0, n, false)) {
    Instrs out;
    for (std::size_t k = 0; k < reads.size(); ++k) {
      if (k % 2 == 0) {
        append(out, stage_dptr(ptr_op));
      }
      out.push_back(std::move(reads[k]));
    }
    return out;
  }
  Instrs out = stage_dptr(ptr_op);
  append(out, std::move(reads));
  return out;
}

// `*p = v`. The two bytes of `p` go into DPTR, and then the bytes of
// `src` are written through `(DPTR),Y`. The volatile bit goes on the
// writes through DPTR only.
Instrs Translator::translate_store(const tac::Store& instr) {
  const ir::Type_operand ptr_op = translate_val(instr.dst_ptr);
  const ir::Type_operand src_op = translate_val(instr.src);
  const int n = size_of(instr.src);
  if (!instr.is_volatile && restage(*this, instr.dst_ptr, ptr_op, 0, n, true)) {
    Instrs writes = indirect_write(src_op, n);
    Instrs out;
    for (std::size_t k = 0; k < writes.size(); ++k) {
      if (k % 2 == 0) {
        append(out, stage_dptr(ptr_op));
      }
      out.push_back(std::move(writes[k]));
    }
    return out;
  }
  Instrs out = stage_dptr(ptr_op);
  append(out, indirect_write(src_op, n, instr.is_volatile));
  return out;
}

// A load from a static array at an index: the index goes into X, and
// the width of `dst` is read from `name+0..N-1,X` (absolute,X on the
// label) into `dst`.
//
// X is used because Frame, Stack, and Indirect operands use Y for their
// `(PTR),Y` accesses, and a destination in the frame would put `LDY
// #off; STA (FP),Y` between the reads. Nothing else uses X at this
// layer, so `LDA arr+k,X` reads interleave with stores to the frame. The
// index goes into X through A (`LDA; TAX`), since it may be in the frame
// and LDX has no indirect-Y mode.
//
// A constant index needs no X. The bytes are read from `name+k+0..N-1`
// with absolute addressing. A volatile read goes through A as a volatile
// `LDA` and an ordinary `STA`, so the store into `dst` can be dropped
// when nothing reads it, as in `while (!(KBD & 0x80)) ;`. One Mov from
// memory to memory would carry the volatile bit to the store as well.
//
// `offset`, the node's constant offset, is added to every byte's
// address: `LDA name+offset+k,X`. A table split into byte planes
// (`plane` nonzero) has byte k at `name+offset+k*plane,X`. A table of
// row addresses has its planes in objects of their own, and byte k is at
// `plane_k+offset,X`.
Instrs Translator::translate_indexed_load(const tac::IndexedLoad& instr) {
  const ir::Type_operand index_op = translate_val(instr.index);
  const ir::Type_operand dst_op = translate_val(instr.dst);
  const int n = size_of(instr.dst);
  const asdl::Int step = instr.plane != 0 ? instr.plane : 1;
  const std::vector<std::string>* planes = nullptr;
  if (symbols != nullptr) {
    if (const passes::Symbol* sym = symbols->find(instr.name)) {
      if (const auto* st = std::get_if<passes::StaticAttr>(&sym->attrs)) {
        if (st->planes && !st->planes->empty()) {
          planes = &*st->planes;
        }
      }
    }
  }
  // The object and offset of byte k of the element at `base`.
  const auto byte = [&](int k, asdl::Int base) -> std::pair<std::string, asdl::Int> {
    if (planes != nullptr) {
      return {planes->at(static_cast<std::size_t>(k)), base};
    }
    return {instr.name, base + k * step};
  };
  if (const auto* imm = as<ir::Imm>(index_op)) {
    const asdl::Int base = (imm->value & 0xFF) + instr.offset;
    Instrs out;
    for (int k = 0; k < n; ++k) {
      const auto [obj, off] = byte(k, base);
      if (instr.is_volatile) {
        out.push_back(ir::Mov{.src = named_byte(obj, off), .dst = reg_a(), .is_volatile = true});
        out.push_back(ir::Mov{.src = reg_a(), .dst = byte_at(dst_op, k)});
      } else {
        out.push_back(ir::Mov{.src = named_byte(obj, off), .dst = byte_at(dst_op, k)});
      }
    }
    return out;
  }
  Instrs out{ir::Mov{.src = index_op, .dst = reg_a()}, ir::Mov{.src = reg_a(), .dst = reg_x()}};
  for (int k = 0; k < n; ++k) {
    const auto [obj, off] = byte(k, instr.offset);
    out.push_back(ir::Mov{.src = ir::IndexedData{.name = obj, .offset = off, .index = ir::X{}},
                          .dst = byte_at(dst_op, k),
                          .is_volatile = instr.is_volatile});
  }
  return out;
}

// A store to a numeric base address at an index: the index goes into X,
// `src` into A, and then `STA $<address>,X`. An `IndexedData` with an
// empty name is written as the address in hex, with no symbol for the
// linker. The recognizer (`recognize_indexed_store`) has checked that
// the index is a 1-byte variable and `src` a 1-byte variable or
// constant, so the lowering is one STA.
Instrs Translator::translate_indexed_store(const tac::IndexedStore& instr) {
  const ir::Type_operand index_op = translate_val(instr.index);
  const ir::Type_operand src_op = translate_val(instr.src);
  return {ir::Mov{.src = index_op, .dst = reg_a()}, ir::Mov{.src = reg_a(), .dst = reg_x()},
          ir::Mov{.src = src_op, .dst = reg_a()},
          ir::Mov{.src = reg_a(),
                  .dst = ir::IndexedData{.name = "", .offset = instr.address, .index = ir::X{}},
                  .is_volatile = instr.is_volatile}};
}

// A store to a static array at an index: the index goes into X, and the
// bytes of `src` are written to `name+0..N-1,X`, the mirror of
// `translate_indexed_load`, with X for the same reason. The
// `direct_index_load` peephole makes the `LDA; TAX` pair one `LDX` when
// the index is an Imm, a Data, or a ZP. A constant index needs no X,
// and the bytes are written to `name+k+0..N-1` with absolute addressing.
Instrs Translator::translate_indexed_symbol_store(const tac::IndexedSymbolStore& instr) {
  const ir::Type_operand index_op = translate_val(instr.index);
  const ir::Type_operand src_op = translate_val(instr.src);
  const int n = size_of(instr.src);
  Instrs out;
  if (const auto* imm = as<ir::Imm>(index_op)) {
    const asdl::Int base = (imm->value & 0xFF) + instr.offset;
    for (int k = 0; k < n; ++k) {
      out.push_back(ir::Mov{.src = byte_at(src_op, k), .dst = reg_a()});
      out.push_back(ir::Mov{
          .src = reg_a(), .dst = named_byte(instr.name, base + k), .is_volatile = instr.is_volatile});
    }
    return out;
  }
  out.push_back(ir::Mov{.src = index_op, .dst = reg_a()});
  out.push_back(ir::Mov{.src = reg_a(), .dst = reg_x()});
  for (int k = 0; k < n; ++k) {
    out.push_back(ir::Mov{.src = byte_at(src_op, k), .dst = reg_a()});
    out.push_back(
        ir::Mov{.src = reg_a(),
                .dst = ir::IndexedData{.name = instr.name, .offset = instr.offset + k, .index = ir::X{}},
                .is_volatile = instr.is_volatile});
  }
  return out;
}

// A load from a numeric base address at an index, the mirror of
// `translate_indexed_store`: the index goes into X, then `LDA
// $<address>,X`, then A into `dst`. The recognizer checked that every
// operand is 1 byte.
Instrs Translator::translate_indexed_const_load(const tac::IndexedConstLoad& instr) {
  const ir::Type_operand index_op = translate_val(instr.index);
  const ir::Type_operand dst_op = translate_val(instr.dst);
  return {ir::Mov{.src = index_op, .dst = reg_a()}, ir::Mov{.src = reg_a(), .dst = reg_x()},
          ir::Mov{.src = ir::IndexedData{.name = "", .offset = instr.address, .index = ir::X{}},
                  .dst = reg_a(),
                  .is_volatile = instr.is_volatile},
          ir::Mov{.src = reg_a(), .dst = dst_op}};
}

// A `(zp),Y` load through DPTR, with Y holding the index:
//
//     <ptr into DPTR>
//     Mov(idx, A)        ; LDA idx   (may change Y for an index in the frame)
//     Mov(A, Reg(Y))     ; TAY       (Y = idx)
//     Mov(IndirectY, A)  ; LDA (DPTR),Y
//     Mov(A, dst)        ; STA dst
//
// These are eight instructions, where the `ZeroExtend`, `Add`, and
// `Load` it replaces take about ten, and the gain grows in an unrolled
// body, where the staging of DPTR is the same each time and the asm
// passes drop the repeats. The only instruction between `TAY` and `LDA
// (DPTR),Y` is the read through `IndirectY` itself, which emits no
// `LDY`, so Y still holds the index.
//
// The recognizer checked that `dst` is 1 byte, or that the index is a
// constant `k` that reaches each of its bytes in Y. Such a wider value
// is read a byte at a time, `LDY #k+b; LDA (DPTR),Y`, as a plain Load is.
Instrs Translator::translate_indirect_indexed_load(const tac::IndirectIndexedLoad& instr) {
  const ir::Type_operand ptr_op = translate_val(instr.ptr);
  const ir::Type_operand idx_op = byte_at(translate_val(instr.index), 0);
  const ir::Type_operand dst_op = translate_val(instr.dst);
  const int n = size_of(instr.dst);
  if (const auto* imm = as<ir::Imm>(idx_op); n > 1 && imm != nullptr) {
    const bool again = restage(*this, instr.ptr, ptr_op, imm->value, n, false);
    Instrs out;
    for (int b = 0; b < n; ++b) {
      if (b == 0 || again) {
        append(out, stage_dptr(ptr_op));
      }
      out.push_back(
          ir::Mov{.src = ir::Indirect{imm->value + b}, .dst = reg_a(), .is_volatile = instr.is_volatile});
      out.push_back(ir::Mov{.src = reg_a(), .dst = byte_at(dst_op, b)});
    }
    return out;
  }
  Instrs out = stage_dptr(ptr_op);
  append(out, load_y_index(idx_op));
  out.push_back(ir::Mov{.src = ir::IndirectY{}, .dst = reg_a(), .is_volatile = instr.is_volatile});
  out.push_back(ir::Mov{.src = reg_a(), .dst = dst_op});
  return out;
}

// A `(zp),Y` store through DPTR. Two values have to reach the STA, `src`
// in A and the index in Y, and loading either from the frame changes Y
// (its `LDY #off`), so `src` is kept on the hardware stack across the
// index's load:
//
//     <ptr into DPTR>
//     Mov(src, A)         ; LDA src   (Y may change)
//     Push                ; PHA       (src on the hardware stack)
//     Mov(idx, A)         ; LDA idx   (may change Y, which is set next)
//     Mov(A, Reg(Y))      ; TAY       (Y = idx)
//     Pop                 ; PLA       (A = src)
//     Mov(A, IndirectY)   ; STA (DPTR),Y
//
// The PHA and PLA cost 2 bytes and 7 cycles against a sequence for
// operands in zero page only, and they keep the store correct when
// either operand is in the frame. A peephole could drop them when both
// operands turn out to be in zero page after register allocation, and
// none does yet.
Instrs Translator::translate_indirect_indexed_store(const tac::IndirectIndexedStore& instr) {
  const ir::Type_operand ptr_op = translate_val(instr.ptr);
  const ir::Type_operand idx_op = byte_at(translate_val(instr.index), 0);
  const int n = size_of(instr.src);
  const auto* imm = as<ir::Imm>(idx_op);
  if (n > 1 && imm != nullptr) {
    // A value wider than a byte at a constant offset, which the
    // recognizer allows when Y reaches all its bytes.
    const ir::Type_operand whole = translate_val(instr.src);
    const bool again =
        !instr.is_volatile && restage(*this, instr.ptr, ptr_op, imm->value, n, true);
    Instrs out;
    for (int b = 0; b < n; ++b) {
      if (b == 0 || again) {
        append(out, stage_dptr(ptr_op));
      }
      out.push_back(ir::Mov{.src = byte_at(whole, b), .dst = reg_a()});
      out.push_back(
          ir::Mov{.src = reg_a(), .dst = ir::Indirect{imm->value + b}, .is_volatile = instr.is_volatile});
    }
    return out;
  }
  // `byte_at` takes the low byte of a constant, so a negative `signed
  // char` constant becomes its 2's complement byte.
  const ir::Type_operand src_op = byte_at(translate_val(instr.src), 0);
  Instrs out = stage_dptr(ptr_op);
  if (imm != nullptr) {
    // `LDY #k` leaves A alone, so no save is needed.
    out.push_back(ir::Mov{.src = src_op, .dst = reg_a()});
    out.push_back(ir::Mov{.src = idx_op, .dst = reg_y()});
    out.push_back(ir::Mov{.src = reg_a(), .dst = ir::IndirectY{}, .is_volatile = instr.is_volatile});
    return out;
  }
  out.push_back(ir::Mov{.src = src_op, .dst = reg_a()});
  out.push_back(ir::Push{.src = reg_a()});
  out.push_back(ir::Mov{.src = idx_op, .dst = reg_a()});
  out.push_back(ir::Mov{.src = reg_a(), .dst = reg_y()});
  out.push_back(ir::Pop{.dst = reg_a()});
  out.push_back(ir::Mov{.src = reg_a(), .dst = ir::IndirectY{}, .is_volatile = instr.is_volatile});
  return out;
}

}  // namespace c6502::tac_to_asm
