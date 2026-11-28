// Generated from tac.asdl by asdlgen (cpp/tools/asdlgen.h). Do not edit.
#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <variant>
#include <vector>

#include "asdl/runtime.h"

namespace c6502::tac_ast {

// const
struct ConstChar;
struct ConstUChar;
struct ConstInt;
struct ConstLong;
struct ConstLongLong;
struct ConstUInt;
struct ConstULong;
struct ConstULongLong;
struct ConstFloat;
struct ConstDouble;
struct ConstInt24;
struct ConstUInt24;
struct ConstInt40;
struct ConstUInt40;
struct ConstInt48;
struct ConstUInt48;
struct ConstInt56;
struct ConstUInt56;
using Type_const = std::variant<ConstChar, ConstUChar, ConstInt, ConstLong, ConstLongLong, ConstUInt, ConstULong, ConstULongLong, ConstFloat, ConstDouble, ConstInt24, ConstUInt24, ConstInt40, ConstUInt40, ConstInt48, ConstUInt48, ConstInt56, ConstUInt56>;
struct ConstChar {
  ::c6502::asdl::Int value = 0;
  bool operator==(const ConstChar& other) const;
};
struct ConstUChar {
  ::c6502::asdl::Int value = 0;
  bool operator==(const ConstUChar& other) const;
};
struct ConstInt {
  ::c6502::asdl::Int value = 0;
  bool operator==(const ConstInt& other) const;
};
struct ConstLong {
  ::c6502::asdl::Int value = 0;
  bool operator==(const ConstLong& other) const;
};
struct ConstLongLong {
  ::c6502::asdl::Int value = 0;
  bool operator==(const ConstLongLong& other) const;
};
struct ConstUInt {
  ::c6502::asdl::Int value = 0;
  bool operator==(const ConstUInt& other) const;
};
struct ConstULong {
  ::c6502::asdl::Int value = 0;
  bool operator==(const ConstULong& other) const;
};
struct ConstULongLong {
  ::c6502::asdl::Int value = 0;
  bool operator==(const ConstULongLong& other) const;
};
struct ConstFloat {
  ::c6502::asdl::Int bits = 0;
  bool operator==(const ConstFloat& other) const;
};
struct ConstDouble {
  ::c6502::asdl::Int bits = 0;
  bool operator==(const ConstDouble& other) const;
};
struct ConstInt24 {
  ::c6502::asdl::Int value = 0;
  bool operator==(const ConstInt24& other) const;
};
struct ConstUInt24 {
  ::c6502::asdl::Int value = 0;
  bool operator==(const ConstUInt24& other) const;
};
struct ConstInt40 {
  ::c6502::asdl::Int value = 0;
  bool operator==(const ConstInt40& other) const;
};
struct ConstUInt40 {
  ::c6502::asdl::Int value = 0;
  bool operator==(const ConstUInt40& other) const;
};
struct ConstInt48 {
  ::c6502::asdl::Int value = 0;
  bool operator==(const ConstInt48& other) const;
};
struct ConstUInt48 {
  ::c6502::asdl::Int value = 0;
  bool operator==(const ConstUInt48& other) const;
};
struct ConstInt56 {
  ::c6502::asdl::Int value = 0;
  bool operator==(const ConstInt56& other) const;
};
struct ConstUInt56 {
  ::c6502::asdl::Int value = 0;
  bool operator==(const ConstUInt56& other) const;
};

// val
struct Constant;
struct Var;
using Type_val = std::variant<Constant, Var>;
struct Constant {
  Type_const const_;
  bool operator==(const Constant& other) const;
};
struct Var {
  std::string name;
  bool operator==(const Var& other) const;
};

// unary_operator
struct Complement;
struct Negate;
struct LogicalNot;
struct BinToBcd;
using Type_unary_operator = std::variant<Complement, Negate, LogicalNot, BinToBcd>;
struct Complement {
  bool operator==(const Complement& other) const;
};
struct Negate {
  bool operator==(const Negate& other) const;
};
struct LogicalNot {
  bool operator==(const LogicalNot& other) const;
};
struct BinToBcd {
  bool operator==(const BinToBcd& other) const;
};

// binary_operator
struct Add;
struct Subtract;
struct Multiply;
struct Divide;
struct Modulo;
struct BitwiseAnd;
struct BitwiseOr;
struct BitwiseXor;
struct LeftShift;
struct RightShift;
struct Equal;
struct NotEqual;
struct LessThan;
struct GreaterThan;
struct LessOrEqual;
struct GreaterOrEqual;
struct BcdAdd;
struct BcdSubtract;
using Type_binary_operator = std::variant<Add, Subtract, Multiply, Divide, Modulo, BitwiseAnd, BitwiseOr, BitwiseXor, LeftShift, RightShift, Equal, NotEqual, LessThan, GreaterThan, LessOrEqual, GreaterOrEqual, BcdAdd, BcdSubtract>;
struct Add {
  bool operator==(const Add& other) const;
};
struct Subtract {
  bool operator==(const Subtract& other) const;
};
struct Multiply {
  bool operator==(const Multiply& other) const;
};
struct Divide {
  bool operator==(const Divide& other) const;
};
struct Modulo {
  bool operator==(const Modulo& other) const;
};
struct BitwiseAnd {
  bool operator==(const BitwiseAnd& other) const;
};
struct BitwiseOr {
  bool operator==(const BitwiseOr& other) const;
};
struct BitwiseXor {
  bool operator==(const BitwiseXor& other) const;
};
struct LeftShift {
  bool operator==(const LeftShift& other) const;
};
struct RightShift {
  bool operator==(const RightShift& other) const;
};
struct Equal {
  bool operator==(const Equal& other) const;
};
struct NotEqual {
  bool operator==(const NotEqual& other) const;
};
struct LessThan {
  bool operator==(const LessThan& other) const;
};
struct GreaterThan {
  bool operator==(const GreaterThan& other) const;
};
struct LessOrEqual {
  bool operator==(const LessOrEqual& other) const;
};
struct GreaterOrEqual {
  bool operator==(const GreaterOrEqual& other) const;
};
struct BcdAdd {
  bool operator==(const BcdAdd& other) const;
};
struct BcdSubtract {
  bool operator==(const BcdSubtract& other) const;
};

// phi_arg
struct PhiArg;
using Type_phi_arg = std::variant<PhiArg>;
struct PhiArg {
  std::string pred_label;
  Type_val source;
  bool operator==(const PhiArg& other) const;
};

// instruction
struct Ret;
struct SignExtend;
struct ZeroExtend;
struct Truncate;
struct IntToFloat;
struct IntToDouble;
struct FloatToInt;
struct DoubleToInt;
struct FloatToDouble;
struct DoubleToFloat;
struct GetAddress;
struct Load;
struct Store;
struct IndexedLoad;
struct IndexedStore;
struct IndexedSymbolStore;
struct IndexedConstLoad;
struct IndirectIndexedLoad;
struct IndirectIndexedStore;
struct Unary;
struct Binary;
struct Copy;
struct Jump;
struct JumpIfTrue;
struct JumpIfFalse;
struct JumpIfCmp;
struct JumpIfMasked;
struct Label;
struct FunctionCall;
struct IndirectCall;
struct Phi;
using Type_instruction = std::variant<Ret, SignExtend, ZeroExtend, Truncate, IntToFloat, IntToDouble, FloatToInt, DoubleToInt, FloatToDouble, DoubleToFloat, GetAddress, Load, Store, IndexedLoad, IndexedStore, IndexedSymbolStore, IndexedConstLoad, IndirectIndexedLoad, IndirectIndexedStore, Unary, Binary, Copy, Jump, JumpIfTrue, JumpIfFalse, JumpIfCmp, JumpIfMasked, Label, FunctionCall, IndirectCall, Phi>;
struct Ret {
  std::optional<Type_val> val;
  bool operator==(const Ret& other) const;
};
struct SignExtend {
  Type_val src;
  Type_val dst;
  bool operator==(const SignExtend& other) const;
};
struct ZeroExtend {
  Type_val src;
  Type_val dst;
  bool operator==(const ZeroExtend& other) const;
};
struct Truncate {
  Type_val src;
  Type_val dst;
  bool operator==(const Truncate& other) const;
};
struct IntToFloat {
  Type_val src;
  Type_val dst;
  bool operator==(const IntToFloat& other) const;
};
struct IntToDouble {
  Type_val src;
  Type_val dst;
  bool operator==(const IntToDouble& other) const;
};
struct FloatToInt {
  Type_val src;
  Type_val dst;
  bool operator==(const FloatToInt& other) const;
};
struct DoubleToInt {
  Type_val src;
  Type_val dst;
  bool operator==(const DoubleToInt& other) const;
};
struct FloatToDouble {
  Type_val src;
  Type_val dst;
  bool operator==(const FloatToDouble& other) const;
};
struct DoubleToFloat {
  Type_val src;
  Type_val dst;
  bool operator==(const DoubleToFloat& other) const;
};
struct GetAddress {
  Type_val operand;
  Type_val dst;
  bool operator==(const GetAddress& other) const;
};
struct Load {
  Type_val src_ptr;
  Type_val dst;
  bool is_volatile = false;
  bool operator==(const Load& other) const;
};
struct Store {
  Type_val src;
  Type_val dst_ptr;
  bool is_volatile = false;
  bool operator==(const Store& other) const;
};
struct IndexedLoad {
  std::string name;
  Type_val index;
  Type_val dst;
  bool is_volatile = false;
  ::c6502::asdl::Int offset = 0;
  ::c6502::asdl::Int plane = 0;
  bool operator==(const IndexedLoad& other) const;
};
struct IndexedStore {
  ::c6502::asdl::Int address = 0;
  Type_val index;
  Type_val src;
  bool is_volatile = false;
  bool operator==(const IndexedStore& other) const;
};
struct IndexedSymbolStore {
  std::string name;
  Type_val index;
  Type_val src;
  bool is_volatile = false;
  ::c6502::asdl::Int offset = 0;
  bool operator==(const IndexedSymbolStore& other) const;
};
struct IndexedConstLoad {
  ::c6502::asdl::Int address = 0;
  Type_val index;
  Type_val dst;
  bool is_volatile = false;
  bool operator==(const IndexedConstLoad& other) const;
};
struct IndirectIndexedLoad {
  Type_val ptr;
  Type_val index;
  Type_val dst;
  bool is_volatile = false;
  bool operator==(const IndirectIndexedLoad& other) const;
};
struct IndirectIndexedStore {
  Type_val ptr;
  Type_val index;
  Type_val src;
  bool is_volatile = false;
  bool operator==(const IndirectIndexedStore& other) const;
};
struct Unary {
  Type_unary_operator op;
  Type_val src;
  Type_val dst;
  bool operator==(const Unary& other) const;
};
struct Binary {
  Type_binary_operator op;
  Type_val src1;
  Type_val src2;
  Type_val dst;
  bool operator==(const Binary& other) const;
};
struct Copy {
  Type_val src;
  Type_val dst;
  bool operator==(const Copy& other) const;
};
struct Jump {
  std::string target;
  bool operator==(const Jump& other) const;
};
struct JumpIfTrue {
  Type_val condition;
  std::string target;
  bool operator==(const JumpIfTrue& other) const;
};
struct JumpIfFalse {
  Type_val condition;
  std::string target;
  bool operator==(const JumpIfFalse& other) const;
};
struct JumpIfCmp {
  Type_binary_operator op;
  Type_val src1;
  Type_val src2;
  std::string target;
  bool operator==(const JumpIfCmp& other) const;
};
struct JumpIfMasked {
  Type_val val;
  ::c6502::asdl::Int mask = 0;
  bool jump_when_nonzero = false;
  std::string target;
  bool operator==(const JumpIfMasked& other) const;
};
struct Label {
  std::string name;
  bool operator==(const Label& other) const;
};
struct FunctionCall {
  std::string name;
  std::vector<Type_val> args;
  std::optional<Type_val> dst;
  bool hw_stack_save = false;
  bool noreturn = false;
  bool operator==(const FunctionCall& other) const;
};
struct IndirectCall {
  Type_val ptr;
  std::vector<Type_val> args;
  std::optional<Type_val> dst;
  bool operator==(const IndirectCall& other) const;
};
struct Phi {
  Type_val dst;
  std::vector<Type_phi_arg> args;
  bool operator==(const Phi& other) const;
};

// data_type
struct Int;
struct Long;
struct LongLong;
struct UInt;
struct ULong;
struct ULongLong;
struct Float;
struct Double;
struct Void;
struct Pointer;
struct FunType;
using Type_data_type = std::variant<Int, Long, LongLong, UInt, ULong, ULongLong, Float, Double, Void, Pointer, FunType>;
struct Int {
  bool operator==(const Int& other) const;
};
struct Long {
  bool operator==(const Long& other) const;
};
struct LongLong {
  bool operator==(const LongLong& other) const;
};
struct UInt {
  bool operator==(const UInt& other) const;
};
struct ULong {
  bool operator==(const ULong& other) const;
};
struct ULongLong {
  bool operator==(const ULongLong& other) const;
};
struct Float {
  bool operator==(const Float& other) const;
};
struct Double {
  bool operator==(const Double& other) const;
};
struct Void {
  bool operator==(const Void& other) const;
};
struct Pointer {
  bool operator==(const Pointer& other) const;
};
struct FunType {
  std::vector<Type_data_type> params;
  ::c6502::asdl::Box<Type_data_type> ret;
  bool operator==(const FunType& other) const;
};

// static_init
struct CharInit;
struct UCharInit;
struct IntInit;
struct LongInit;
struct LongLongInit;
struct UIntInit;
struct ULongInit;
struct ULongLongInit;
struct FloatInit;
struct DoubleInit;
struct AddressInit;
struct AddressByteInit;
struct StringInit;
struct ZeroInit;
using Type_static_init = std::variant<CharInit, UCharInit, IntInit, LongInit, LongLongInit, UIntInit, ULongInit, ULongLongInit, FloatInit, DoubleInit, AddressInit, AddressByteInit, StringInit, ZeroInit>;
struct CharInit {
  ::c6502::asdl::Int value = 0;
  bool operator==(const CharInit& other) const;
};
struct UCharInit {
  ::c6502::asdl::Int value = 0;
  bool operator==(const UCharInit& other) const;
};
struct IntInit {
  ::c6502::asdl::Int value = 0;
  bool operator==(const IntInit& other) const;
};
struct LongInit {
  ::c6502::asdl::Int value = 0;
  bool operator==(const LongInit& other) const;
};
struct LongLongInit {
  ::c6502::asdl::Int value = 0;
  bool operator==(const LongLongInit& other) const;
};
struct UIntInit {
  ::c6502::asdl::Int value = 0;
  bool operator==(const UIntInit& other) const;
};
struct ULongInit {
  ::c6502::asdl::Int value = 0;
  bool operator==(const ULongInit& other) const;
};
struct ULongLongInit {
  ::c6502::asdl::Int value = 0;
  bool operator==(const ULongLongInit& other) const;
};
struct FloatInit {
  ::c6502::asdl::Int bits = 0;
  bool operator==(const FloatInit& other) const;
};
struct DoubleInit {
  ::c6502::asdl::Int bits = 0;
  bool operator==(const DoubleInit& other) const;
};
struct AddressInit {
  std::string name;
  ::c6502::asdl::Int offset = 0;
  bool operator==(const AddressInit& other) const;
};
struct AddressByteInit {
  std::string name;
  ::c6502::asdl::Int offset = 0;
  bool high = false;
  bool operator==(const AddressByteInit& other) const;
};
struct StringInit {
  std::string str;
  ::c6502::asdl::Int bytes = 0;
  bool operator==(const StringInit& other) const;
};
struct ZeroInit {
  ::c6502::asdl::Int bytes = 0;
  bool operator==(const ZeroInit& other) const;
};

// top_level
struct Function;
struct StaticVariable;
using Type_top_level = std::variant<Function, StaticVariable>;
struct Function {
  std::string name;
  bool is_global = false;
  std::vector<std::string> params;
  std::vector<Type_instruction> instructions;
  bool operator==(const Function& other) const;
};
struct StaticVariable {
  std::string name;
  bool is_global = false;
  Type_data_type data_type;
  std::vector<Type_static_init> init;
  bool operator==(const StaticVariable& other) const;
};

// program
struct Program;
using Type_program = std::variant<Program>;
struct Program {
  std::vector<Type_top_level> top_level;
  bool operator==(const Program& other) const;
};

// The constructor of a sum type's value, numbered as the
// variant's alternatives are, for a switch.
enum class Kind_program : std::uint8_t {
  Program,
};
inline Kind_program kind(const Type_program& v) noexcept {
  return static_cast<Kind_program>(v.index());
}
enum class Kind_top_level : std::uint8_t {
  Function,
  StaticVariable,
};
inline Kind_top_level kind(const Type_top_level& v) noexcept {
  return static_cast<Kind_top_level>(v.index());
}
enum class Kind_instruction : std::uint8_t {
  Ret,
  SignExtend,
  ZeroExtend,
  Truncate,
  IntToFloat,
  IntToDouble,
  FloatToInt,
  DoubleToInt,
  FloatToDouble,
  DoubleToFloat,
  GetAddress,
  Load,
  Store,
  IndexedLoad,
  IndexedStore,
  IndexedSymbolStore,
  IndexedConstLoad,
  IndirectIndexedLoad,
  IndirectIndexedStore,
  Unary,
  Binary,
  Copy,
  Jump,
  JumpIfTrue,
  JumpIfFalse,
  JumpIfCmp,
  JumpIfMasked,
  Label,
  FunctionCall,
  IndirectCall,
  Phi,
};
inline Kind_instruction kind(const Type_instruction& v) noexcept {
  return static_cast<Kind_instruction>(v.index());
}
enum class Kind_phi_arg : std::uint8_t {
  PhiArg,
};
inline Kind_phi_arg kind(const Type_phi_arg& v) noexcept {
  return static_cast<Kind_phi_arg>(v.index());
}
enum class Kind_val : std::uint8_t {
  Constant,
  Var,
};
inline Kind_val kind(const Type_val& v) noexcept {
  return static_cast<Kind_val>(v.index());
}
enum class Kind_unary_operator : std::uint8_t {
  Complement,
  Negate,
  LogicalNot,
  BinToBcd,
};
inline Kind_unary_operator kind(const Type_unary_operator& v) noexcept {
  return static_cast<Kind_unary_operator>(v.index());
}
enum class Kind_binary_operator : std::uint8_t {
  Add,
  Subtract,
  Multiply,
  Divide,
  Modulo,
  BitwiseAnd,
  BitwiseOr,
  BitwiseXor,
  LeftShift,
  RightShift,
  Equal,
  NotEqual,
  LessThan,
  GreaterThan,
  LessOrEqual,
  GreaterOrEqual,
  BcdAdd,
  BcdSubtract,
};
inline Kind_binary_operator kind(const Type_binary_operator& v) noexcept {
  return static_cast<Kind_binary_operator>(v.index());
}
enum class Kind_data_type : std::uint8_t {
  Int,
  Long,
  LongLong,
  UInt,
  ULong,
  ULongLong,
  Float,
  Double,
  Void,
  Pointer,
  FunType,
};
inline Kind_data_type kind(const Type_data_type& v) noexcept {
  return static_cast<Kind_data_type>(v.index());
}
enum class Kind_const : std::uint8_t {
  ConstChar,
  ConstUChar,
  ConstInt,
  ConstLong,
  ConstLongLong,
  ConstUInt,
  ConstULong,
  ConstULongLong,
  ConstFloat,
  ConstDouble,
  ConstInt24,
  ConstUInt24,
  ConstInt40,
  ConstUInt40,
  ConstInt48,
  ConstUInt48,
  ConstInt56,
  ConstUInt56,
};
inline Kind_const kind(const Type_const& v) noexcept {
  return static_cast<Kind_const>(v.index());
}
enum class Kind_static_init : std::uint8_t {
  CharInit,
  UCharInit,
  IntInit,
  LongInit,
  LongLongInit,
  UIntInit,
  ULongInit,
  ULongLongInit,
  FloatInit,
  DoubleInit,
  AddressInit,
  AddressByteInit,
  StringInit,
  ZeroInit,
};
inline Kind_static_init kind(const Type_static_init& v) noexcept {
  return static_cast<Kind_static_init>(v.index());
}

inline bool ConstChar::operator==(const ConstChar& other) const {
  return value == other.value;
}
inline bool ConstUChar::operator==(const ConstUChar& other) const {
  return value == other.value;
}
inline bool ConstInt::operator==(const ConstInt& other) const {
  return value == other.value;
}
inline bool ConstLong::operator==(const ConstLong& other) const {
  return value == other.value;
}
inline bool ConstLongLong::operator==(const ConstLongLong& other) const {
  return value == other.value;
}
inline bool ConstUInt::operator==(const ConstUInt& other) const {
  return value == other.value;
}
inline bool ConstULong::operator==(const ConstULong& other) const {
  return value == other.value;
}
inline bool ConstULongLong::operator==(const ConstULongLong& other) const {
  return value == other.value;
}
inline bool ConstFloat::operator==(const ConstFloat& other) const {
  return bits == other.bits;
}
inline bool ConstDouble::operator==(const ConstDouble& other) const {
  return bits == other.bits;
}
inline bool ConstInt24::operator==(const ConstInt24& other) const {
  return value == other.value;
}
inline bool ConstUInt24::operator==(const ConstUInt24& other) const {
  return value == other.value;
}
inline bool ConstInt40::operator==(const ConstInt40& other) const {
  return value == other.value;
}
inline bool ConstUInt40::operator==(const ConstUInt40& other) const {
  return value == other.value;
}
inline bool ConstInt48::operator==(const ConstInt48& other) const {
  return value == other.value;
}
inline bool ConstUInt48::operator==(const ConstUInt48& other) const {
  return value == other.value;
}
inline bool ConstInt56::operator==(const ConstInt56& other) const {
  return value == other.value;
}
inline bool ConstUInt56::operator==(const ConstUInt56& other) const {
  return value == other.value;
}
inline bool Constant::operator==(const Constant& other) const {
  return const_ == other.const_;
}
inline bool Var::operator==(const Var& other) const {
  return name == other.name;
}
inline bool Complement::operator==(const Complement&) const {
  return true;
}
inline bool Negate::operator==(const Negate&) const {
  return true;
}
inline bool LogicalNot::operator==(const LogicalNot&) const {
  return true;
}
inline bool BinToBcd::operator==(const BinToBcd&) const {
  return true;
}
inline bool Add::operator==(const Add&) const {
  return true;
}
inline bool Subtract::operator==(const Subtract&) const {
  return true;
}
inline bool Multiply::operator==(const Multiply&) const {
  return true;
}
inline bool Divide::operator==(const Divide&) const {
  return true;
}
inline bool Modulo::operator==(const Modulo&) const {
  return true;
}
inline bool BitwiseAnd::operator==(const BitwiseAnd&) const {
  return true;
}
inline bool BitwiseOr::operator==(const BitwiseOr&) const {
  return true;
}
inline bool BitwiseXor::operator==(const BitwiseXor&) const {
  return true;
}
inline bool LeftShift::operator==(const LeftShift&) const {
  return true;
}
inline bool RightShift::operator==(const RightShift&) const {
  return true;
}
inline bool Equal::operator==(const Equal&) const {
  return true;
}
inline bool NotEqual::operator==(const NotEqual&) const {
  return true;
}
inline bool LessThan::operator==(const LessThan&) const {
  return true;
}
inline bool GreaterThan::operator==(const GreaterThan&) const {
  return true;
}
inline bool LessOrEqual::operator==(const LessOrEqual&) const {
  return true;
}
inline bool GreaterOrEqual::operator==(const GreaterOrEqual&) const {
  return true;
}
inline bool BcdAdd::operator==(const BcdAdd&) const {
  return true;
}
inline bool BcdSubtract::operator==(const BcdSubtract&) const {
  return true;
}
inline bool PhiArg::operator==(const PhiArg& other) const {
  return pred_label == other.pred_label
      && source == other.source;
}
inline bool Ret::operator==(const Ret& other) const {
  return val == other.val;
}
inline bool SignExtend::operator==(const SignExtend& other) const {
  return src == other.src
      && dst == other.dst;
}
inline bool ZeroExtend::operator==(const ZeroExtend& other) const {
  return src == other.src
      && dst == other.dst;
}
inline bool Truncate::operator==(const Truncate& other) const {
  return src == other.src
      && dst == other.dst;
}
inline bool IntToFloat::operator==(const IntToFloat& other) const {
  return src == other.src
      && dst == other.dst;
}
inline bool IntToDouble::operator==(const IntToDouble& other) const {
  return src == other.src
      && dst == other.dst;
}
inline bool FloatToInt::operator==(const FloatToInt& other) const {
  return src == other.src
      && dst == other.dst;
}
inline bool DoubleToInt::operator==(const DoubleToInt& other) const {
  return src == other.src
      && dst == other.dst;
}
inline bool FloatToDouble::operator==(const FloatToDouble& other) const {
  return src == other.src
      && dst == other.dst;
}
inline bool DoubleToFloat::operator==(const DoubleToFloat& other) const {
  return src == other.src
      && dst == other.dst;
}
inline bool GetAddress::operator==(const GetAddress& other) const {
  return operand == other.operand
      && dst == other.dst;
}
inline bool Load::operator==(const Load& other) const {
  return src_ptr == other.src_ptr
      && dst == other.dst
      && is_volatile == other.is_volatile;
}
inline bool Store::operator==(const Store& other) const {
  return src == other.src
      && dst_ptr == other.dst_ptr
      && is_volatile == other.is_volatile;
}
inline bool IndexedLoad::operator==(const IndexedLoad& other) const {
  return name == other.name
      && index == other.index
      && dst == other.dst
      && is_volatile == other.is_volatile
      && offset == other.offset
      && plane == other.plane;
}
inline bool IndexedStore::operator==(const IndexedStore& other) const {
  return address == other.address
      && index == other.index
      && src == other.src
      && is_volatile == other.is_volatile;
}
inline bool IndexedSymbolStore::operator==(const IndexedSymbolStore& other) const {
  return name == other.name
      && index == other.index
      && src == other.src
      && is_volatile == other.is_volatile
      && offset == other.offset;
}
inline bool IndexedConstLoad::operator==(const IndexedConstLoad& other) const {
  return address == other.address
      && index == other.index
      && dst == other.dst
      && is_volatile == other.is_volatile;
}
inline bool IndirectIndexedLoad::operator==(const IndirectIndexedLoad& other) const {
  return ptr == other.ptr
      && index == other.index
      && dst == other.dst
      && is_volatile == other.is_volatile;
}
inline bool IndirectIndexedStore::operator==(const IndirectIndexedStore& other) const {
  return ptr == other.ptr
      && index == other.index
      && src == other.src
      && is_volatile == other.is_volatile;
}
inline bool Unary::operator==(const Unary& other) const {
  return op == other.op
      && src == other.src
      && dst == other.dst;
}
inline bool Binary::operator==(const Binary& other) const {
  return op == other.op
      && src1 == other.src1
      && src2 == other.src2
      && dst == other.dst;
}
inline bool Copy::operator==(const Copy& other) const {
  return src == other.src
      && dst == other.dst;
}
inline bool Jump::operator==(const Jump& other) const {
  return target == other.target;
}
inline bool JumpIfTrue::operator==(const JumpIfTrue& other) const {
  return condition == other.condition
      && target == other.target;
}
inline bool JumpIfFalse::operator==(const JumpIfFalse& other) const {
  return condition == other.condition
      && target == other.target;
}
inline bool JumpIfCmp::operator==(const JumpIfCmp& other) const {
  return op == other.op
      && src1 == other.src1
      && src2 == other.src2
      && target == other.target;
}
inline bool JumpIfMasked::operator==(const JumpIfMasked& other) const {
  return val == other.val
      && mask == other.mask
      && jump_when_nonzero == other.jump_when_nonzero
      && target == other.target;
}
inline bool Label::operator==(const Label& other) const {
  return name == other.name;
}
inline bool FunctionCall::operator==(const FunctionCall& other) const {
  return name == other.name
      && args == other.args
      && dst == other.dst
      && hw_stack_save == other.hw_stack_save
      && noreturn == other.noreturn;
}
inline bool IndirectCall::operator==(const IndirectCall& other) const {
  return ptr == other.ptr
      && args == other.args
      && dst == other.dst;
}
inline bool Phi::operator==(const Phi& other) const {
  return dst == other.dst
      && args == other.args;
}
inline bool Int::operator==(const Int&) const {
  return true;
}
inline bool Long::operator==(const Long&) const {
  return true;
}
inline bool LongLong::operator==(const LongLong&) const {
  return true;
}
inline bool UInt::operator==(const UInt&) const {
  return true;
}
inline bool ULong::operator==(const ULong&) const {
  return true;
}
inline bool ULongLong::operator==(const ULongLong&) const {
  return true;
}
inline bool Float::operator==(const Float&) const {
  return true;
}
inline bool Double::operator==(const Double&) const {
  return true;
}
inline bool Void::operator==(const Void&) const {
  return true;
}
inline bool Pointer::operator==(const Pointer&) const {
  return true;
}
inline bool FunType::operator==(const FunType& other) const {
  return params == other.params
      && ret == other.ret;
}
inline bool CharInit::operator==(const CharInit& other) const {
  return value == other.value;
}
inline bool UCharInit::operator==(const UCharInit& other) const {
  return value == other.value;
}
inline bool IntInit::operator==(const IntInit& other) const {
  return value == other.value;
}
inline bool LongInit::operator==(const LongInit& other) const {
  return value == other.value;
}
inline bool LongLongInit::operator==(const LongLongInit& other) const {
  return value == other.value;
}
inline bool UIntInit::operator==(const UIntInit& other) const {
  return value == other.value;
}
inline bool ULongInit::operator==(const ULongInit& other) const {
  return value == other.value;
}
inline bool ULongLongInit::operator==(const ULongLongInit& other) const {
  return value == other.value;
}
inline bool FloatInit::operator==(const FloatInit& other) const {
  return bits == other.bits;
}
inline bool DoubleInit::operator==(const DoubleInit& other) const {
  return bits == other.bits;
}
inline bool AddressInit::operator==(const AddressInit& other) const {
  return name == other.name
      && offset == other.offset;
}
inline bool AddressByteInit::operator==(const AddressByteInit& other) const {
  return name == other.name
      && offset == other.offset
      && high == other.high;
}
inline bool StringInit::operator==(const StringInit& other) const {
  return str == other.str
      && bytes == other.bytes;
}
inline bool ZeroInit::operator==(const ZeroInit& other) const {
  return bytes == other.bytes;
}
inline bool Function::operator==(const Function& other) const {
  return name == other.name
      && is_global == other.is_global
      && params == other.params
      && instructions == other.instructions;
}
inline bool StaticVariable::operator==(const StaticVariable& other) const {
  return name == other.name
      && is_global == other.is_global
      && data_type == other.data_type
      && init == other.init;
}
inline bool Program::operator==(const Program& other) const {
  return top_level == other.top_level;
}

// The dump format's writers and readers (cpp/README.md, "The dump
// format"). `asdl::dump` and `asdl::load` call them.
void write(::c6502::asdl::Writer& w, const Type_program& v);
Type_program read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Type_program>);
void write(::c6502::asdl::Writer& w, const Program& v);
Program read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Program>);
void write(::c6502::asdl::Writer& w, const Type_top_level& v);
Type_top_level read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Type_top_level>);
void write(::c6502::asdl::Writer& w, const Function& v);
Function read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Function>);
void write(::c6502::asdl::Writer& w, const StaticVariable& v);
StaticVariable read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<StaticVariable>);
void write(::c6502::asdl::Writer& w, const Type_instruction& v);
Type_instruction read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Type_instruction>);
void write(::c6502::asdl::Writer& w, const Ret& v);
Ret read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Ret>);
void write(::c6502::asdl::Writer& w, const SignExtend& v);
SignExtend read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<SignExtend>);
void write(::c6502::asdl::Writer& w, const ZeroExtend& v);
ZeroExtend read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<ZeroExtend>);
void write(::c6502::asdl::Writer& w, const Truncate& v);
Truncate read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Truncate>);
void write(::c6502::asdl::Writer& w, const IntToFloat& v);
IntToFloat read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<IntToFloat>);
void write(::c6502::asdl::Writer& w, const IntToDouble& v);
IntToDouble read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<IntToDouble>);
void write(::c6502::asdl::Writer& w, const FloatToInt& v);
FloatToInt read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<FloatToInt>);
void write(::c6502::asdl::Writer& w, const DoubleToInt& v);
DoubleToInt read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<DoubleToInt>);
void write(::c6502::asdl::Writer& w, const FloatToDouble& v);
FloatToDouble read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<FloatToDouble>);
void write(::c6502::asdl::Writer& w, const DoubleToFloat& v);
DoubleToFloat read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<DoubleToFloat>);
void write(::c6502::asdl::Writer& w, const GetAddress& v);
GetAddress read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<GetAddress>);
void write(::c6502::asdl::Writer& w, const Load& v);
Load read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Load>);
void write(::c6502::asdl::Writer& w, const Store& v);
Store read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Store>);
void write(::c6502::asdl::Writer& w, const IndexedLoad& v);
IndexedLoad read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<IndexedLoad>);
void write(::c6502::asdl::Writer& w, const IndexedStore& v);
IndexedStore read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<IndexedStore>);
void write(::c6502::asdl::Writer& w, const IndexedSymbolStore& v);
IndexedSymbolStore read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<IndexedSymbolStore>);
void write(::c6502::asdl::Writer& w, const IndexedConstLoad& v);
IndexedConstLoad read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<IndexedConstLoad>);
void write(::c6502::asdl::Writer& w, const IndirectIndexedLoad& v);
IndirectIndexedLoad read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<IndirectIndexedLoad>);
void write(::c6502::asdl::Writer& w, const IndirectIndexedStore& v);
IndirectIndexedStore read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<IndirectIndexedStore>);
void write(::c6502::asdl::Writer& w, const Unary& v);
Unary read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Unary>);
void write(::c6502::asdl::Writer& w, const Binary& v);
Binary read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Binary>);
void write(::c6502::asdl::Writer& w, const Copy& v);
Copy read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Copy>);
void write(::c6502::asdl::Writer& w, const Jump& v);
Jump read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Jump>);
void write(::c6502::asdl::Writer& w, const JumpIfTrue& v);
JumpIfTrue read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<JumpIfTrue>);
void write(::c6502::asdl::Writer& w, const JumpIfFalse& v);
JumpIfFalse read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<JumpIfFalse>);
void write(::c6502::asdl::Writer& w, const JumpIfCmp& v);
JumpIfCmp read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<JumpIfCmp>);
void write(::c6502::asdl::Writer& w, const JumpIfMasked& v);
JumpIfMasked read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<JumpIfMasked>);
void write(::c6502::asdl::Writer& w, const Label& v);
Label read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Label>);
void write(::c6502::asdl::Writer& w, const FunctionCall& v);
FunctionCall read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<FunctionCall>);
void write(::c6502::asdl::Writer& w, const IndirectCall& v);
IndirectCall read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<IndirectCall>);
void write(::c6502::asdl::Writer& w, const Phi& v);
Phi read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Phi>);
void write(::c6502::asdl::Writer& w, const Type_phi_arg& v);
Type_phi_arg read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Type_phi_arg>);
void write(::c6502::asdl::Writer& w, const PhiArg& v);
PhiArg read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<PhiArg>);
void write(::c6502::asdl::Writer& w, const Type_val& v);
Type_val read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Type_val>);
void write(::c6502::asdl::Writer& w, const Constant& v);
Constant read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Constant>);
void write(::c6502::asdl::Writer& w, const Var& v);
Var read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Var>);
void write(::c6502::asdl::Writer& w, const Type_unary_operator& v);
Type_unary_operator read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Type_unary_operator>);
void write(::c6502::asdl::Writer& w, const Complement& v);
Complement read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Complement>);
void write(::c6502::asdl::Writer& w, const Negate& v);
Negate read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Negate>);
void write(::c6502::asdl::Writer& w, const LogicalNot& v);
LogicalNot read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<LogicalNot>);
void write(::c6502::asdl::Writer& w, const BinToBcd& v);
BinToBcd read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<BinToBcd>);
void write(::c6502::asdl::Writer& w, const Type_binary_operator& v);
Type_binary_operator read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Type_binary_operator>);
void write(::c6502::asdl::Writer& w, const Add& v);
Add read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Add>);
void write(::c6502::asdl::Writer& w, const Subtract& v);
Subtract read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Subtract>);
void write(::c6502::asdl::Writer& w, const Multiply& v);
Multiply read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Multiply>);
void write(::c6502::asdl::Writer& w, const Divide& v);
Divide read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Divide>);
void write(::c6502::asdl::Writer& w, const Modulo& v);
Modulo read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Modulo>);
void write(::c6502::asdl::Writer& w, const BitwiseAnd& v);
BitwiseAnd read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<BitwiseAnd>);
void write(::c6502::asdl::Writer& w, const BitwiseOr& v);
BitwiseOr read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<BitwiseOr>);
void write(::c6502::asdl::Writer& w, const BitwiseXor& v);
BitwiseXor read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<BitwiseXor>);
void write(::c6502::asdl::Writer& w, const LeftShift& v);
LeftShift read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<LeftShift>);
void write(::c6502::asdl::Writer& w, const RightShift& v);
RightShift read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<RightShift>);
void write(::c6502::asdl::Writer& w, const Equal& v);
Equal read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Equal>);
void write(::c6502::asdl::Writer& w, const NotEqual& v);
NotEqual read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<NotEqual>);
void write(::c6502::asdl::Writer& w, const LessThan& v);
LessThan read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<LessThan>);
void write(::c6502::asdl::Writer& w, const GreaterThan& v);
GreaterThan read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<GreaterThan>);
void write(::c6502::asdl::Writer& w, const LessOrEqual& v);
LessOrEqual read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<LessOrEqual>);
void write(::c6502::asdl::Writer& w, const GreaterOrEqual& v);
GreaterOrEqual read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<GreaterOrEqual>);
void write(::c6502::asdl::Writer& w, const BcdAdd& v);
BcdAdd read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<BcdAdd>);
void write(::c6502::asdl::Writer& w, const BcdSubtract& v);
BcdSubtract read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<BcdSubtract>);
void write(::c6502::asdl::Writer& w, const Type_data_type& v);
Type_data_type read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Type_data_type>);
void write(::c6502::asdl::Writer& w, const Int& v);
Int read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Int>);
void write(::c6502::asdl::Writer& w, const Long& v);
Long read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Long>);
void write(::c6502::asdl::Writer& w, const LongLong& v);
LongLong read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<LongLong>);
void write(::c6502::asdl::Writer& w, const UInt& v);
UInt read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<UInt>);
void write(::c6502::asdl::Writer& w, const ULong& v);
ULong read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<ULong>);
void write(::c6502::asdl::Writer& w, const ULongLong& v);
ULongLong read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<ULongLong>);
void write(::c6502::asdl::Writer& w, const Float& v);
Float read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Float>);
void write(::c6502::asdl::Writer& w, const Double& v);
Double read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Double>);
void write(::c6502::asdl::Writer& w, const Void& v);
Void read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Void>);
void write(::c6502::asdl::Writer& w, const Pointer& v);
Pointer read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Pointer>);
void write(::c6502::asdl::Writer& w, const FunType& v);
FunType read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<FunType>);
void write(::c6502::asdl::Writer& w, const Type_const& v);
Type_const read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Type_const>);
void write(::c6502::asdl::Writer& w, const ConstChar& v);
ConstChar read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<ConstChar>);
void write(::c6502::asdl::Writer& w, const ConstUChar& v);
ConstUChar read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<ConstUChar>);
void write(::c6502::asdl::Writer& w, const ConstInt& v);
ConstInt read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<ConstInt>);
void write(::c6502::asdl::Writer& w, const ConstLong& v);
ConstLong read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<ConstLong>);
void write(::c6502::asdl::Writer& w, const ConstLongLong& v);
ConstLongLong read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<ConstLongLong>);
void write(::c6502::asdl::Writer& w, const ConstUInt& v);
ConstUInt read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<ConstUInt>);
void write(::c6502::asdl::Writer& w, const ConstULong& v);
ConstULong read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<ConstULong>);
void write(::c6502::asdl::Writer& w, const ConstULongLong& v);
ConstULongLong read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<ConstULongLong>);
void write(::c6502::asdl::Writer& w, const ConstFloat& v);
ConstFloat read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<ConstFloat>);
void write(::c6502::asdl::Writer& w, const ConstDouble& v);
ConstDouble read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<ConstDouble>);
void write(::c6502::asdl::Writer& w, const ConstInt24& v);
ConstInt24 read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<ConstInt24>);
void write(::c6502::asdl::Writer& w, const ConstUInt24& v);
ConstUInt24 read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<ConstUInt24>);
void write(::c6502::asdl::Writer& w, const ConstInt40& v);
ConstInt40 read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<ConstInt40>);
void write(::c6502::asdl::Writer& w, const ConstUInt40& v);
ConstUInt40 read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<ConstUInt40>);
void write(::c6502::asdl::Writer& w, const ConstInt48& v);
ConstInt48 read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<ConstInt48>);
void write(::c6502::asdl::Writer& w, const ConstUInt48& v);
ConstUInt48 read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<ConstUInt48>);
void write(::c6502::asdl::Writer& w, const ConstInt56& v);
ConstInt56 read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<ConstInt56>);
void write(::c6502::asdl::Writer& w, const ConstUInt56& v);
ConstUInt56 read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<ConstUInt56>);
void write(::c6502::asdl::Writer& w, const Type_static_init& v);
Type_static_init read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Type_static_init>);
void write(::c6502::asdl::Writer& w, const CharInit& v);
CharInit read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<CharInit>);
void write(::c6502::asdl::Writer& w, const UCharInit& v);
UCharInit read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<UCharInit>);
void write(::c6502::asdl::Writer& w, const IntInit& v);
IntInit read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<IntInit>);
void write(::c6502::asdl::Writer& w, const LongInit& v);
LongInit read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<LongInit>);
void write(::c6502::asdl::Writer& w, const LongLongInit& v);
LongLongInit read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<LongLongInit>);
void write(::c6502::asdl::Writer& w, const UIntInit& v);
UIntInit read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<UIntInit>);
void write(::c6502::asdl::Writer& w, const ULongInit& v);
ULongInit read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<ULongInit>);
void write(::c6502::asdl::Writer& w, const ULongLongInit& v);
ULongLongInit read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<ULongLongInit>);
void write(::c6502::asdl::Writer& w, const FloatInit& v);
FloatInit read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<FloatInit>);
void write(::c6502::asdl::Writer& w, const DoubleInit& v);
DoubleInit read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<DoubleInit>);
void write(::c6502::asdl::Writer& w, const AddressInit& v);
AddressInit read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<AddressInit>);
void write(::c6502::asdl::Writer& w, const AddressByteInit& v);
AddressByteInit read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<AddressByteInit>);
void write(::c6502::asdl::Writer& w, const StringInit& v);
StringInit read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<StringInit>);
void write(::c6502::asdl::Writer& w, const ZeroInit& v);
ZeroInit read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<ZeroInit>);

// Python's `repr` of each value, the dataclass's: the class name
// and each field as `name=value`. `asdl::py_repr` returns it.
void repr(std::string& out, const Type_program& v);
void repr(std::string& out, const Program& v);
void repr(std::string& out, const Type_top_level& v);
void repr(std::string& out, const Function& v);
void repr(std::string& out, const StaticVariable& v);
void repr(std::string& out, const Type_instruction& v);
void repr(std::string& out, const Ret& v);
void repr(std::string& out, const SignExtend& v);
void repr(std::string& out, const ZeroExtend& v);
void repr(std::string& out, const Truncate& v);
void repr(std::string& out, const IntToFloat& v);
void repr(std::string& out, const IntToDouble& v);
void repr(std::string& out, const FloatToInt& v);
void repr(std::string& out, const DoubleToInt& v);
void repr(std::string& out, const FloatToDouble& v);
void repr(std::string& out, const DoubleToFloat& v);
void repr(std::string& out, const GetAddress& v);
void repr(std::string& out, const Load& v);
void repr(std::string& out, const Store& v);
void repr(std::string& out, const IndexedLoad& v);
void repr(std::string& out, const IndexedStore& v);
void repr(std::string& out, const IndexedSymbolStore& v);
void repr(std::string& out, const IndexedConstLoad& v);
void repr(std::string& out, const IndirectIndexedLoad& v);
void repr(std::string& out, const IndirectIndexedStore& v);
void repr(std::string& out, const Unary& v);
void repr(std::string& out, const Binary& v);
void repr(std::string& out, const Copy& v);
void repr(std::string& out, const Jump& v);
void repr(std::string& out, const JumpIfTrue& v);
void repr(std::string& out, const JumpIfFalse& v);
void repr(std::string& out, const JumpIfCmp& v);
void repr(std::string& out, const JumpIfMasked& v);
void repr(std::string& out, const Label& v);
void repr(std::string& out, const FunctionCall& v);
void repr(std::string& out, const IndirectCall& v);
void repr(std::string& out, const Phi& v);
void repr(std::string& out, const Type_phi_arg& v);
void repr(std::string& out, const PhiArg& v);
void repr(std::string& out, const Type_val& v);
void repr(std::string& out, const Constant& v);
void repr(std::string& out, const Var& v);
void repr(std::string& out, const Type_unary_operator& v);
void repr(std::string& out, const Complement& v);
void repr(std::string& out, const Negate& v);
void repr(std::string& out, const LogicalNot& v);
void repr(std::string& out, const BinToBcd& v);
void repr(std::string& out, const Type_binary_operator& v);
void repr(std::string& out, const Add& v);
void repr(std::string& out, const Subtract& v);
void repr(std::string& out, const Multiply& v);
void repr(std::string& out, const Divide& v);
void repr(std::string& out, const Modulo& v);
void repr(std::string& out, const BitwiseAnd& v);
void repr(std::string& out, const BitwiseOr& v);
void repr(std::string& out, const BitwiseXor& v);
void repr(std::string& out, const LeftShift& v);
void repr(std::string& out, const RightShift& v);
void repr(std::string& out, const Equal& v);
void repr(std::string& out, const NotEqual& v);
void repr(std::string& out, const LessThan& v);
void repr(std::string& out, const GreaterThan& v);
void repr(std::string& out, const LessOrEqual& v);
void repr(std::string& out, const GreaterOrEqual& v);
void repr(std::string& out, const BcdAdd& v);
void repr(std::string& out, const BcdSubtract& v);
void repr(std::string& out, const Type_data_type& v);
void repr(std::string& out, const Int& v);
void repr(std::string& out, const Long& v);
void repr(std::string& out, const LongLong& v);
void repr(std::string& out, const UInt& v);
void repr(std::string& out, const ULong& v);
void repr(std::string& out, const ULongLong& v);
void repr(std::string& out, const Float& v);
void repr(std::string& out, const Double& v);
void repr(std::string& out, const Void& v);
void repr(std::string& out, const Pointer& v);
void repr(std::string& out, const FunType& v);
void repr(std::string& out, const Type_const& v);
void repr(std::string& out, const ConstChar& v);
void repr(std::string& out, const ConstUChar& v);
void repr(std::string& out, const ConstInt& v);
void repr(std::string& out, const ConstLong& v);
void repr(std::string& out, const ConstLongLong& v);
void repr(std::string& out, const ConstUInt& v);
void repr(std::string& out, const ConstULong& v);
void repr(std::string& out, const ConstULongLong& v);
void repr(std::string& out, const ConstFloat& v);
void repr(std::string& out, const ConstDouble& v);
void repr(std::string& out, const ConstInt24& v);
void repr(std::string& out, const ConstUInt24& v);
void repr(std::string& out, const ConstInt40& v);
void repr(std::string& out, const ConstUInt40& v);
void repr(std::string& out, const ConstInt48& v);
void repr(std::string& out, const ConstUInt48& v);
void repr(std::string& out, const ConstInt56& v);
void repr(std::string& out, const ConstUInt56& v);
void repr(std::string& out, const Type_static_init& v);
void repr(std::string& out, const CharInit& v);
void repr(std::string& out, const UCharInit& v);
void repr(std::string& out, const IntInit& v);
void repr(std::string& out, const LongInit& v);
void repr(std::string& out, const LongLongInit& v);
void repr(std::string& out, const UIntInit& v);
void repr(std::string& out, const ULongInit& v);
void repr(std::string& out, const ULongLongInit& v);
void repr(std::string& out, const FloatInit& v);
void repr(std::string& out, const DoubleInit& v);
void repr(std::string& out, const AddressInit& v);
void repr(std::string& out, const AddressByteInit& v);
void repr(std::string& out, const StringInit& v);
void repr(std::string& out, const ZeroInit& v);

// pretty.py's text of each value at the indentation level `depth`:
// the class name, a `name=value,` line for each field one level
// deeper, lists one item to a line, and Python's `repr` of each leaf.
// `asdl::py_pretty` returns it.
void pretty(std::string& out, const Type_program& v, int depth);
void pretty(std::string& out, const Program& v, int depth);
void pretty(std::string& out, const Type_top_level& v, int depth);
void pretty(std::string& out, const Function& v, int depth);
void pretty(std::string& out, const StaticVariable& v, int depth);
void pretty(std::string& out, const Type_instruction& v, int depth);
void pretty(std::string& out, const Ret& v, int depth);
void pretty(std::string& out, const SignExtend& v, int depth);
void pretty(std::string& out, const ZeroExtend& v, int depth);
void pretty(std::string& out, const Truncate& v, int depth);
void pretty(std::string& out, const IntToFloat& v, int depth);
void pretty(std::string& out, const IntToDouble& v, int depth);
void pretty(std::string& out, const FloatToInt& v, int depth);
void pretty(std::string& out, const DoubleToInt& v, int depth);
void pretty(std::string& out, const FloatToDouble& v, int depth);
void pretty(std::string& out, const DoubleToFloat& v, int depth);
void pretty(std::string& out, const GetAddress& v, int depth);
void pretty(std::string& out, const Load& v, int depth);
void pretty(std::string& out, const Store& v, int depth);
void pretty(std::string& out, const IndexedLoad& v, int depth);
void pretty(std::string& out, const IndexedStore& v, int depth);
void pretty(std::string& out, const IndexedSymbolStore& v, int depth);
void pretty(std::string& out, const IndexedConstLoad& v, int depth);
void pretty(std::string& out, const IndirectIndexedLoad& v, int depth);
void pretty(std::string& out, const IndirectIndexedStore& v, int depth);
void pretty(std::string& out, const Unary& v, int depth);
void pretty(std::string& out, const Binary& v, int depth);
void pretty(std::string& out, const Copy& v, int depth);
void pretty(std::string& out, const Jump& v, int depth);
void pretty(std::string& out, const JumpIfTrue& v, int depth);
void pretty(std::string& out, const JumpIfFalse& v, int depth);
void pretty(std::string& out, const JumpIfCmp& v, int depth);
void pretty(std::string& out, const JumpIfMasked& v, int depth);
void pretty(std::string& out, const Label& v, int depth);
void pretty(std::string& out, const FunctionCall& v, int depth);
void pretty(std::string& out, const IndirectCall& v, int depth);
void pretty(std::string& out, const Phi& v, int depth);
void pretty(std::string& out, const Type_phi_arg& v, int depth);
void pretty(std::string& out, const PhiArg& v, int depth);
void pretty(std::string& out, const Type_val& v, int depth);
void pretty(std::string& out, const Constant& v, int depth);
void pretty(std::string& out, const Var& v, int depth);
void pretty(std::string& out, const Type_unary_operator& v, int depth);
void pretty(std::string& out, const Complement& v, int depth);
void pretty(std::string& out, const Negate& v, int depth);
void pretty(std::string& out, const LogicalNot& v, int depth);
void pretty(std::string& out, const BinToBcd& v, int depth);
void pretty(std::string& out, const Type_binary_operator& v, int depth);
void pretty(std::string& out, const Add& v, int depth);
void pretty(std::string& out, const Subtract& v, int depth);
void pretty(std::string& out, const Multiply& v, int depth);
void pretty(std::string& out, const Divide& v, int depth);
void pretty(std::string& out, const Modulo& v, int depth);
void pretty(std::string& out, const BitwiseAnd& v, int depth);
void pretty(std::string& out, const BitwiseOr& v, int depth);
void pretty(std::string& out, const BitwiseXor& v, int depth);
void pretty(std::string& out, const LeftShift& v, int depth);
void pretty(std::string& out, const RightShift& v, int depth);
void pretty(std::string& out, const Equal& v, int depth);
void pretty(std::string& out, const NotEqual& v, int depth);
void pretty(std::string& out, const LessThan& v, int depth);
void pretty(std::string& out, const GreaterThan& v, int depth);
void pretty(std::string& out, const LessOrEqual& v, int depth);
void pretty(std::string& out, const GreaterOrEqual& v, int depth);
void pretty(std::string& out, const BcdAdd& v, int depth);
void pretty(std::string& out, const BcdSubtract& v, int depth);
void pretty(std::string& out, const Type_data_type& v, int depth);
void pretty(std::string& out, const Int& v, int depth);
void pretty(std::string& out, const Long& v, int depth);
void pretty(std::string& out, const LongLong& v, int depth);
void pretty(std::string& out, const UInt& v, int depth);
void pretty(std::string& out, const ULong& v, int depth);
void pretty(std::string& out, const ULongLong& v, int depth);
void pretty(std::string& out, const Float& v, int depth);
void pretty(std::string& out, const Double& v, int depth);
void pretty(std::string& out, const Void& v, int depth);
void pretty(std::string& out, const Pointer& v, int depth);
void pretty(std::string& out, const FunType& v, int depth);
void pretty(std::string& out, const Type_const& v, int depth);
void pretty(std::string& out, const ConstChar& v, int depth);
void pretty(std::string& out, const ConstUChar& v, int depth);
void pretty(std::string& out, const ConstInt& v, int depth);
void pretty(std::string& out, const ConstLong& v, int depth);
void pretty(std::string& out, const ConstLongLong& v, int depth);
void pretty(std::string& out, const ConstUInt& v, int depth);
void pretty(std::string& out, const ConstULong& v, int depth);
void pretty(std::string& out, const ConstULongLong& v, int depth);
void pretty(std::string& out, const ConstFloat& v, int depth);
void pretty(std::string& out, const ConstDouble& v, int depth);
void pretty(std::string& out, const ConstInt24& v, int depth);
void pretty(std::string& out, const ConstUInt24& v, int depth);
void pretty(std::string& out, const ConstInt40& v, int depth);
void pretty(std::string& out, const ConstUInt40& v, int depth);
void pretty(std::string& out, const ConstInt48& v, int depth);
void pretty(std::string& out, const ConstUInt48& v, int depth);
void pretty(std::string& out, const ConstInt56& v, int depth);
void pretty(std::string& out, const ConstUInt56& v, int depth);
void pretty(std::string& out, const Type_static_init& v, int depth);
void pretty(std::string& out, const CharInit& v, int depth);
void pretty(std::string& out, const UCharInit& v, int depth);
void pretty(std::string& out, const IntInit& v, int depth);
void pretty(std::string& out, const LongInit& v, int depth);
void pretty(std::string& out, const LongLongInit& v, int depth);
void pretty(std::string& out, const UIntInit& v, int depth);
void pretty(std::string& out, const ULongInit& v, int depth);
void pretty(std::string& out, const ULongLongInit& v, int depth);
void pretty(std::string& out, const FloatInit& v, int depth);
void pretty(std::string& out, const DoubleInit& v, int depth);
void pretty(std::string& out, const AddressInit& v, int depth);
void pretty(std::string& out, const AddressByteInit& v, int depth);
void pretty(std::string& out, const StringInit& v, int depth);
void pretty(std::string& out, const ZeroInit& v, int depth);

}  // namespace c6502::tac_ast
