// The port of tac_sim.py, the TAC interpreter, which runs a TAC program
// with its symbol table and type table.
//
// Each TAC instruction maps to an operation that mirrors the bit-level
// semantics of the 6502 lowering. A scalar value lives in its frame's
// environment as an unsigned integer of its type's width, the bytes the
// soft stack would hold. Signedness is applied again only where it
// matters: `Divide`, `Modulo`, the ordering comparisons, `RightShift`,
// and `SignExtend`. Address-taken locals, aggregates, and objects with
// static storage live in a flat memory of bytes and are read and
// written through their addresses.
//
// The integer types, pointers, `float`, and `double` run, with integer
// arithmetic, bitwise operations, comparisons, and shifts, and with the
// floating arithmetic, comparisons, `Negate`, and `LogicalNot` of
// src/fp_arith.h. So do the control flow (a floating condition is true
// when it compares unequal to 0), `Copy`, the calls, `Ret`, the casts,
// `GetAddress`, `Load`, `Store`, the layout of each `StaticVariable`,
// struct and union arguments by value, and the `return_bool_in_carry`
// structs a `Ret` returns.
//
// The instructions that only the TAC optimizer makes run too, so
// optimized TAC runs, and so does the SSA form between the optimizer's
// passes: `JumpIfCmp` (unsigned when either operand is, as `tac_to_asm`
// lowers it), `JumpIfMasked`, `IndexedStore` and `IndexedConstLoad` (a
// numeric base address), `IndirectIndexedLoad` and
// `IndirectIndexedStore` (a pointer plus a 1-byte index), and `Phi`. A
// run of Phis at the start of a block takes, all at once, the argument
// for the block that control came from, which is the block of the last
// label executed, since SSA gives every predecessor of a Phi's block a
// label.
//
// Python's interpreter recurses once for each call of the program, and
// a call deeper than about 330 levels raises Python's RecursionError.
// The C++ interpreter keeps its frames in a list of its own, so a
// recursion of up to `kMaxCallDepth` levels runs, and a deeper one
// raises `SimError("RecursionError", ...)`.
#pragma once

#include <cstdint>

#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "asdl/runtime.h"
#include "passes/type_checking.h"
#include "tac_ast.h"

namespace c6502::tac_sim {

// The most frames a run holds at once.
inline constexpr std::size_t kMaxCallDepth = 100000;

// An exception that Python's interpreter raises while it runs a program,
// with the name of its Python class (`TacSimError` for the interpreter's
// own, and `KeyError`, `ValueError`, `OverflowError`, `AttributeError`,
// `UnicodeEncodeError`, and `RecursionError` for the others) and its
// `str()`. A `KeyError`'s message is the key's `repr`, as Python's
// `str()` of one gives it.
class SimError : public std::runtime_error {
 public:
  SimError(std::string python_type, const std::string& message)
      : std::runtime_error(message), python_type_(std::move(python_type)) {}

  const std::string& python_type() const noexcept { return python_type_; }

 private:
  std::string python_type_;
};

// A sparse memory of bytes, addressed by integers. A byte never written
// reads as 0, which is close enough to the zeroed BSS for the
// interpreter, which doesn't check for reads of uninitialized memory.
class Memory {
 public:
  explicit Memory(std::int64_t base = 0x1000) : next_addr_(base) {}

  // Returns the address of `size` new bytes. Nothing is ever freed.
  std::int64_t allocate(std::int64_t size);

  // Writes the `width` low bytes of `value`, low byte first. The bytes
  // past the eighth are 0.
  void store(std::int64_t addr, std::uint64_t value, std::int64_t width);
  // Writes the `width` low bytes of the Python integer `value`, whose
  // bytes past its own width are its sign's, as Python's `>>` gives them.
  void store_int(std::int64_t addr, asdl::Int value, std::int64_t width);
  // Returns the `width` bytes at `addr` as an integer, low byte first,
  // for `width` from 0 to 8.
  std::uint64_t load(std::int64_t addr, std::int64_t width) const;
  // Returns the `width` bytes at `addr` as an integer, for `width` from
  // 0 to 15. Throws `InternalError` for a wider one.
  asdl::Int load_int(std::int64_t addr, std::int64_t width) const;

  std::uint8_t byte(std::int64_t addr) const;
  void set_byte(std::int64_t addr, std::uint8_t b);

  std::int64_t next_addr() const { return next_addr_; }

 private:
  std::int64_t next_addr_;
  // The bytes below 16 MiB, which hold every address that `allocate`
  // gives out in practice, and those of any other address.
  std::vector<std::uint8_t> low_;
  std::unordered_map<std::int64_t, std::uint8_t> high_;
};

// What `_type_info` gives for a scalar type: whether it is signed, its
// width in bytes, and whether it is floating. A pointer is unsigned, and
// plain `char` is unsigned in c6502 (C99 §6.2.5.15 leaves it to the
// implementation), so only `signed char` is a signed character type.
// For a floating type the signedness is never read, and the width
// chooses single or double precision.
struct TypeInfo {
  bool is_signed = false;
  int width = 0;
  bool is_fp = false;
};

// Runs a TAC program. The program and the tables must outlive the
// simulator.
//
//     Simulator sim(tac, symbols, &types);
//     sim.call("main", {});
//
// After a call, a static variable's value is `read_static(name)`, and
// `memory()` gives the bytes.
class Simulator {
 public:
  // Lays out the program's memory. The `StaticVariable`s get their
  // addresses first, in the program's order, then each function gets 2
  // bytes whose address stands for it, so that `&f` and a call through a
  // pointer work without code, and then the static initializers are
  // laid down, since an `AddressInit` may name a function. `types` may
  // be null, as Python's may be None, for a program without structs or
  // unions.
  Simulator(const tac_ast::Program& program, const passes::SymbolTable& symbols,
            const passes::TypeTable* types);
  ~Simulator();
  Simulator(const Simulator&) = delete;
  Simulator& operator=(const Simulator&) = delete;

  // Calls the function `name` with the arguments `args`, a struct or
  // union argument being the address of its bytes, and returns its
  // result, or none when it returns no value. An integer result is
  // signed when the function's return type is, a floating one is its
  // IEEE 754 bit pattern, and a `return_bool_in_carry` struct's flag,
  // its last byte, comes back as 0 or 1. Throws `SimError`.
  std::optional<asdl::Int> call(const std::string& name, const std::vector<asdl::Int>& args);

  // Returns the value of the scalar static variable `name`, signed when
  // its type is, or the bit pattern of a floating one.
  asdl::Int read_static(const std::string& name);

  const Memory& memory() const { return memory_; }
  Memory& memory() { return memory_; }

  // Returns the address that `name`, a static variable or a function,
  // was given, or none.
  std::optional<std::int64_t> address_of_name(const std::string& name) const;

 private:
  struct FunctionInfo;
  struct Frame;
  struct SymInfo;
  struct Read {
    std::uint64_t value = 0;
    TypeInfo ti;
  };
  // What a `Ret` gives the call: a value of `width` bytes, or none.
  struct Return {
    std::optional<asdl::Int> value;
    std::int64_t width = 0;
  };

  void lay_down_static(const tac_ast::StaticVariable& sv);
  void push_frame(const std::string& name, const std::vector<asdl::Int>& args);
  std::optional<asdl::Int> finish_call(const std::string& name, const Return& ret);
  // Runs one instruction of the innermost frame. Returns the frame's
  // `Ret`, or none, and pushes a frame for a call.
  std::optional<Return> step(Frame& frame, const tac_ast::Type_instruction& ins);
  void run_phis(Frame& frame);
  void jump(Frame& frame, const std::string& target);
  void capture_return(Frame& frame, const tac_ast::Type_val& dst, const std::optional<asdl::Int>& rv);
  std::vector<asdl::Int> call_args(Frame& frame, const std::vector<tac_ast::Type_val>& args);

  const SymInfo& sym_info(const std::string& name) const;
  const passes::Symbol& symbol(const std::string& name) const;
  bool is_aggregate_var(const tac_ast::Type_val& v) const;
  bool is_local_aggregate(const std::string& name) const;
  bool is_local_access(const std::string& name, const tac_ast::Type_val& index) const;
  std::int64_t address_of(Frame& frame, const tac_ast::Type_val& v);
  std::int64_t address_of_var(Frame& frame, const std::string& name);
  std::optional<std::int64_t> static_address(const std::string& name);
  std::int64_t static_slot(const std::string& name);
  bool is_unset(const Frame& frame, const tac_ast::Type_val& v) const;
  static void unset(Frame& frame, const tac_ast::Type_val& v);
  Read read(Frame& frame, const tac_ast::Type_val& v);
  void write(Frame& frame, const tac_ast::Type_val& v, std::uint64_t raw);
  void write_var(Frame& frame, const std::string& name, std::uint64_t raw);
  int dst_width(const tac_ast::Type_val& v) const;
  std::int64_t size_of(const c99_ast::Type_data_type& t) const;
  void memcpy(std::int64_t dst, std::int64_t src, std::int64_t size);

  const passes::SymbolTable& symbols_;
  const passes::TypeTable* types_;
  Memory memory_;
  std::unordered_map<std::string, std::unique_ptr<FunctionInfo>> functions_;
  std::unordered_map<std::string, std::int64_t> static_addr_;
  std::unordered_map<std::string, std::int64_t> function_addr_;
  std::unordered_map<std::int64_t, std::string> addr_to_function_;
  mutable std::unordered_map<std::string, std::unique_ptr<SymInfo>> sym_cache_;
  std::vector<std::unique_ptr<Frame>> frames_;
};

// What a run of `main` gives: its result, or the Python class and
// message of the exception it raised.
struct Outcome {
  std::optional<asdl::Int> value;
  std::optional<std::string> error_type;
  std::string error_message;

  bool operator==(const Outcome&) const = default;
};

// Runs `main` of `program` with no arguments, as
// `Simulator(program, symbols, types).call("main", [])` does, and
// returns its result or the `SimError` it raised.
Outcome run_main(const tac_ast::Program& program, const passes::SymbolTable& symbols,
                 const passes::TypeTable* types);

}  // namespace c6502::tac_sim
