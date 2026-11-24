// Generated from asm2.asdl by `asdl.py --cpp`. Do not edit.
#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <variant>
#include <vector>

#include "asdl/runtime.h"

namespace c6502::asm2_ast {

// reg
struct A;
struct X;
struct Y;
using Type_reg = std::variant<A, X, Y>;
struct A {
  bool operator==(const A& other) const;
};
struct X {
  bool operator==(const X& other) const;
};
struct Y {
  bool operator==(const Y& other) const;
};

// operand
struct Imm;
struct Reg;
struct Stack;
struct Frame;
struct Data;
struct Indirect;
struct IndirectY;
struct IndirectZp;
struct IndirectZpY;
struct ZP;
struct ImmLabelLow;
struct ImmLabelHigh;
struct IndexedData;
using Type_operand = std::variant<Imm, Reg, Stack, Frame, Data, Indirect, IndirectY, IndirectZp, IndirectZpY, ZP, ImmLabelLow, ImmLabelHigh, IndexedData>;
struct Imm {
  ::c6502::asdl::Int value = 0;
  bool operator==(const Imm& other) const;
};
struct Reg {
  Type_reg reg;
  bool operator==(const Reg& other) const;
};
struct Stack {
  ::c6502::asdl::Int offset = 0;
  bool operator==(const Stack& other) const;
};
struct Frame {
  ::c6502::asdl::Int offset = 0;
  bool operator==(const Frame& other) const;
};
struct Data {
  std::string name;
  ::c6502::asdl::Int offset = 0;
  bool operator==(const Data& other) const;
};
struct Indirect {
  ::c6502::asdl::Int offset = 0;
  bool operator==(const Indirect& other) const;
};
struct IndirectY {
  bool operator==(const IndirectY& other) const;
};
struct IndirectZp {
  ::c6502::asdl::Int address = 0;
  ::c6502::asdl::Int offset = 0;
  bool operator==(const IndirectZp& other) const;
};
struct IndirectZpY {
  ::c6502::asdl::Int address = 0;
  bool operator==(const IndirectZpY& other) const;
};
struct ZP {
  ::c6502::asdl::Int address = 0;
  ::c6502::asdl::Int offset = 0;
  bool operator==(const ZP& other) const;
};
struct ImmLabelLow {
  std::string name;
  ::c6502::asdl::Int offset = 0;
  bool operator==(const ImmLabelLow& other) const;
};
struct ImmLabelHigh {
  std::string name;
  ::c6502::asdl::Int offset = 0;
  bool operator==(const ImmLabelHigh& other) const;
};
struct IndexedData {
  std::string name;
  ::c6502::asdl::Int offset = 0;
  Type_reg index;
  bool operator==(const IndexedData& other) const;
};

// condition
struct CC;
struct CS;
struct EQ;
struct MI;
struct NE;
struct PL;
struct VC;
struct VS;
using Type_condition = std::variant<CC, CS, EQ, MI, NE, PL, VC, VS>;
struct CC {
  bool operator==(const CC& other) const;
};
struct CS {
  bool operator==(const CS& other) const;
};
struct EQ {
  bool operator==(const EQ& other) const;
};
struct MI {
  bool operator==(const MI& other) const;
};
struct NE {
  bool operator==(const NE& other) const;
};
struct PL {
  bool operator==(const PL& other) const;
};
struct VC {
  bool operator==(const VC& other) const;
};
struct VS {
  bool operator==(const VS& other) const;
};

// instruction
struct Mov;
struct Return;
struct Add;
struct Sub;
struct Call;
struct ClearCarry;
struct SetCarry;
struct Inc;
struct Dec;
struct Push;
struct Pop;
struct SetDecimal;
struct ClearDecimal;
struct PushStatus;
struct PullStatus;
struct Xor;
struct And;
struct Or;
struct ArithmeticShiftLeft;
struct LogicalShiftRight;
struct RotateLeft;
struct RotateRight;
struct Label;
struct Jump;
struct Branch;
struct JumpTable;
struct Compare;
struct BitTest;
struct LoadAddress;
struct Comment;
struct Blank;
using Type_instruction = std::variant<Mov, Return, Add, Sub, Call, ClearCarry, SetCarry, Inc, Dec, Push, Pop, SetDecimal, ClearDecimal, PushStatus, PullStatus, Xor, And, Or, ArithmeticShiftLeft, LogicalShiftRight, RotateLeft, RotateRight, Label, Jump, Branch, JumpTable, Compare, BitTest, LoadAddress, Comment, Blank>;
struct Mov {
  Type_operand src;
  Type_operand dst;
  bool operator==(const Mov& other) const;
};
struct Return {
  bool operator==(const Return& other) const;
};
struct Add {
  Type_operand src;
  Type_operand dst;
  bool operator==(const Add& other) const;
};
struct Sub {
  Type_operand src;
  Type_operand dst;
  bool operator==(const Sub& other) const;
};
struct Call {
  std::string name;
  bool operator==(const Call& other) const;
};
struct ClearCarry {
  bool operator==(const ClearCarry& other) const;
};
struct SetCarry {
  bool operator==(const SetCarry& other) const;
};
struct Inc {
  Type_operand dst;
  bool operator==(const Inc& other) const;
};
struct Dec {
  Type_operand dst;
  bool operator==(const Dec& other) const;
};
struct Push {
  Type_operand src;
  bool operator==(const Push& other) const;
};
struct Pop {
  Type_operand dst;
  bool operator==(const Pop& other) const;
};
struct SetDecimal {
  bool operator==(const SetDecimal& other) const;
};
struct ClearDecimal {
  bool operator==(const ClearDecimal& other) const;
};
struct PushStatus {
  bool operator==(const PushStatus& other) const;
};
struct PullStatus {
  bool operator==(const PullStatus& other) const;
};
struct Xor {
  Type_operand src1;
  Type_operand src2;
  Type_operand dst;
  bool operator==(const Xor& other) const;
};
struct And {
  Type_operand src;
  Type_operand dst;
  bool operator==(const And& other) const;
};
struct Or {
  Type_operand src;
  Type_operand dst;
  bool operator==(const Or& other) const;
};
struct ArithmeticShiftLeft {
  Type_operand dst;
  bool operator==(const ArithmeticShiftLeft& other) const;
};
struct LogicalShiftRight {
  Type_operand dst;
  bool operator==(const LogicalShiftRight& other) const;
};
struct RotateLeft {
  Type_operand dst;
  bool operator==(const RotateLeft& other) const;
};
struct RotateRight {
  Type_operand dst;
  bool operator==(const RotateRight& other) const;
};
struct Label {
  std::string name;
  bool operator==(const Label& other) const;
};
struct Jump {
  std::string target;
  bool operator==(const Jump& other) const;
};
struct Branch {
  Type_condition cond;
  std::string target;
  bool operator==(const Branch& other) const;
};
struct JumpTable {
  std::string table;
  std::string index;
  ::c6502::asdl::Int base = 0;
  std::vector<std::string> targets;
  bool operator==(const JumpTable& other) const;
};
struct Compare {
  Type_operand left;
  Type_operand right;
  bool operator==(const Compare& other) const;
};
struct BitTest {
  Type_operand src;
  bool operator==(const BitTest& other) const;
};
struct LoadAddress {
  Type_operand src;
  Type_operand dst;
  bool operator==(const LoadAddress& other) const;
};
struct Comment {
  std::string text;
  bool operator==(const Comment& other) const;
};
struct Blank {
  bool operator==(const Blank& other) const;
};

// static_init
struct CharInit;
struct IntInit;
struct LongInit;
struct LongLongInit;
struct FloatInit;
struct DoubleInit;
struct AddressInit;
struct AddressByteInit;
struct StringInit;
struct ZeroInit;
using Type_static_init = std::variant<CharInit, IntInit, LongInit, LongLongInit, FloatInit, DoubleInit, AddressInit, AddressByteInit, StringInit, ZeroInit>;
struct CharInit {
  ::c6502::asdl::Int value = 0;
  bool operator==(const CharInit& other) const;
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
enum class Kind_static_init : std::uint8_t {
  CharInit,
  IntInit,
  LongInit,
  LongLongInit,
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
enum class Kind_instruction : std::uint8_t {
  Mov,
  Return,
  Add,
  Sub,
  Call,
  ClearCarry,
  SetCarry,
  Inc,
  Dec,
  Push,
  Pop,
  SetDecimal,
  ClearDecimal,
  PushStatus,
  PullStatus,
  Xor,
  And,
  Or,
  ArithmeticShiftLeft,
  LogicalShiftRight,
  RotateLeft,
  RotateRight,
  Label,
  Jump,
  Branch,
  JumpTable,
  Compare,
  BitTest,
  LoadAddress,
  Comment,
  Blank,
};
inline Kind_instruction kind(const Type_instruction& v) noexcept {
  return static_cast<Kind_instruction>(v.index());
}
enum class Kind_operand : std::uint8_t {
  Imm,
  Reg,
  Stack,
  Frame,
  Data,
  Indirect,
  IndirectY,
  IndirectZp,
  IndirectZpY,
  ZP,
  ImmLabelLow,
  ImmLabelHigh,
  IndexedData,
};
inline Kind_operand kind(const Type_operand& v) noexcept {
  return static_cast<Kind_operand>(v.index());
}
enum class Kind_reg : std::uint8_t {
  A,
  X,
  Y,
};
inline Kind_reg kind(const Type_reg& v) noexcept {
  return static_cast<Kind_reg>(v.index());
}
enum class Kind_condition : std::uint8_t {
  CC,
  CS,
  EQ,
  MI,
  NE,
  PL,
  VC,
  VS,
};
inline Kind_condition kind(const Type_condition& v) noexcept {
  return static_cast<Kind_condition>(v.index());
}

inline bool A::operator==(const A&) const {
  return true;
}
inline bool X::operator==(const X&) const {
  return true;
}
inline bool Y::operator==(const Y&) const {
  return true;
}
inline bool Imm::operator==(const Imm& other) const {
  return value == other.value;
}
inline bool Reg::operator==(const Reg& other) const {
  return reg == other.reg;
}
inline bool Stack::operator==(const Stack& other) const {
  return offset == other.offset;
}
inline bool Frame::operator==(const Frame& other) const {
  return offset == other.offset;
}
inline bool Data::operator==(const Data& other) const {
  return name == other.name
      && offset == other.offset;
}
inline bool Indirect::operator==(const Indirect& other) const {
  return offset == other.offset;
}
inline bool IndirectY::operator==(const IndirectY&) const {
  return true;
}
inline bool IndirectZp::operator==(const IndirectZp& other) const {
  return address == other.address
      && offset == other.offset;
}
inline bool IndirectZpY::operator==(const IndirectZpY& other) const {
  return address == other.address;
}
inline bool ZP::operator==(const ZP& other) const {
  return address == other.address
      && offset == other.offset;
}
inline bool ImmLabelLow::operator==(const ImmLabelLow& other) const {
  return name == other.name
      && offset == other.offset;
}
inline bool ImmLabelHigh::operator==(const ImmLabelHigh& other) const {
  return name == other.name
      && offset == other.offset;
}
inline bool IndexedData::operator==(const IndexedData& other) const {
  return name == other.name
      && offset == other.offset
      && index == other.index;
}
inline bool CC::operator==(const CC&) const {
  return true;
}
inline bool CS::operator==(const CS&) const {
  return true;
}
inline bool EQ::operator==(const EQ&) const {
  return true;
}
inline bool MI::operator==(const MI&) const {
  return true;
}
inline bool NE::operator==(const NE&) const {
  return true;
}
inline bool PL::operator==(const PL&) const {
  return true;
}
inline bool VC::operator==(const VC&) const {
  return true;
}
inline bool VS::operator==(const VS&) const {
  return true;
}
inline bool Mov::operator==(const Mov& other) const {
  return src == other.src
      && dst == other.dst;
}
inline bool Return::operator==(const Return&) const {
  return true;
}
inline bool Add::operator==(const Add& other) const {
  return src == other.src
      && dst == other.dst;
}
inline bool Sub::operator==(const Sub& other) const {
  return src == other.src
      && dst == other.dst;
}
inline bool Call::operator==(const Call& other) const {
  return name == other.name;
}
inline bool ClearCarry::operator==(const ClearCarry&) const {
  return true;
}
inline bool SetCarry::operator==(const SetCarry&) const {
  return true;
}
inline bool Inc::operator==(const Inc& other) const {
  return dst == other.dst;
}
inline bool Dec::operator==(const Dec& other) const {
  return dst == other.dst;
}
inline bool Push::operator==(const Push& other) const {
  return src == other.src;
}
inline bool Pop::operator==(const Pop& other) const {
  return dst == other.dst;
}
inline bool SetDecimal::operator==(const SetDecimal&) const {
  return true;
}
inline bool ClearDecimal::operator==(const ClearDecimal&) const {
  return true;
}
inline bool PushStatus::operator==(const PushStatus&) const {
  return true;
}
inline bool PullStatus::operator==(const PullStatus&) const {
  return true;
}
inline bool Xor::operator==(const Xor& other) const {
  return src1 == other.src1
      && src2 == other.src2
      && dst == other.dst;
}
inline bool And::operator==(const And& other) const {
  return src == other.src
      && dst == other.dst;
}
inline bool Or::operator==(const Or& other) const {
  return src == other.src
      && dst == other.dst;
}
inline bool ArithmeticShiftLeft::operator==(const ArithmeticShiftLeft& other) const {
  return dst == other.dst;
}
inline bool LogicalShiftRight::operator==(const LogicalShiftRight& other) const {
  return dst == other.dst;
}
inline bool RotateLeft::operator==(const RotateLeft& other) const {
  return dst == other.dst;
}
inline bool RotateRight::operator==(const RotateRight& other) const {
  return dst == other.dst;
}
inline bool Label::operator==(const Label& other) const {
  return name == other.name;
}
inline bool Jump::operator==(const Jump& other) const {
  return target == other.target;
}
inline bool Branch::operator==(const Branch& other) const {
  return cond == other.cond
      && target == other.target;
}
inline bool JumpTable::operator==(const JumpTable& other) const {
  return table == other.table
      && index == other.index
      && base == other.base
      && targets == other.targets;
}
inline bool Compare::operator==(const Compare& other) const {
  return left == other.left
      && right == other.right;
}
inline bool BitTest::operator==(const BitTest& other) const {
  return src == other.src;
}
inline bool LoadAddress::operator==(const LoadAddress& other) const {
  return src == other.src
      && dst == other.dst;
}
inline bool Comment::operator==(const Comment& other) const {
  return text == other.text;
}
inline bool Blank::operator==(const Blank&) const {
  return true;
}
inline bool CharInit::operator==(const CharInit& other) const {
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
      && init == other.init;
}
inline bool Program::operator==(const Program& other) const {
  return top_level == other.top_level;
}

// The dump format's writers and readers (ir_dump.py describes
// the format). `asdl::dump` and `asdl::load` call them.
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
void write(::c6502::asdl::Writer& w, const Type_static_init& v);
Type_static_init read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Type_static_init>);
void write(::c6502::asdl::Writer& w, const CharInit& v);
CharInit read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<CharInit>);
void write(::c6502::asdl::Writer& w, const IntInit& v);
IntInit read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<IntInit>);
void write(::c6502::asdl::Writer& w, const LongInit& v);
LongInit read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<LongInit>);
void write(::c6502::asdl::Writer& w, const LongLongInit& v);
LongLongInit read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<LongLongInit>);
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
void write(::c6502::asdl::Writer& w, const Type_instruction& v);
Type_instruction read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Type_instruction>);
void write(::c6502::asdl::Writer& w, const Mov& v);
Mov read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Mov>);
void write(::c6502::asdl::Writer& w, const Return& v);
Return read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Return>);
void write(::c6502::asdl::Writer& w, const Add& v);
Add read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Add>);
void write(::c6502::asdl::Writer& w, const Sub& v);
Sub read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Sub>);
void write(::c6502::asdl::Writer& w, const Call& v);
Call read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Call>);
void write(::c6502::asdl::Writer& w, const ClearCarry& v);
ClearCarry read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<ClearCarry>);
void write(::c6502::asdl::Writer& w, const SetCarry& v);
SetCarry read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<SetCarry>);
void write(::c6502::asdl::Writer& w, const Inc& v);
Inc read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Inc>);
void write(::c6502::asdl::Writer& w, const Dec& v);
Dec read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Dec>);
void write(::c6502::asdl::Writer& w, const Push& v);
Push read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Push>);
void write(::c6502::asdl::Writer& w, const Pop& v);
Pop read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Pop>);
void write(::c6502::asdl::Writer& w, const SetDecimal& v);
SetDecimal read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<SetDecimal>);
void write(::c6502::asdl::Writer& w, const ClearDecimal& v);
ClearDecimal read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<ClearDecimal>);
void write(::c6502::asdl::Writer& w, const PushStatus& v);
PushStatus read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<PushStatus>);
void write(::c6502::asdl::Writer& w, const PullStatus& v);
PullStatus read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<PullStatus>);
void write(::c6502::asdl::Writer& w, const Xor& v);
Xor read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Xor>);
void write(::c6502::asdl::Writer& w, const And& v);
And read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<And>);
void write(::c6502::asdl::Writer& w, const Or& v);
Or read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Or>);
void write(::c6502::asdl::Writer& w, const ArithmeticShiftLeft& v);
ArithmeticShiftLeft read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<ArithmeticShiftLeft>);
void write(::c6502::asdl::Writer& w, const LogicalShiftRight& v);
LogicalShiftRight read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<LogicalShiftRight>);
void write(::c6502::asdl::Writer& w, const RotateLeft& v);
RotateLeft read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<RotateLeft>);
void write(::c6502::asdl::Writer& w, const RotateRight& v);
RotateRight read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<RotateRight>);
void write(::c6502::asdl::Writer& w, const Label& v);
Label read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Label>);
void write(::c6502::asdl::Writer& w, const Jump& v);
Jump read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Jump>);
void write(::c6502::asdl::Writer& w, const Branch& v);
Branch read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Branch>);
void write(::c6502::asdl::Writer& w, const JumpTable& v);
JumpTable read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<JumpTable>);
void write(::c6502::asdl::Writer& w, const Compare& v);
Compare read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Compare>);
void write(::c6502::asdl::Writer& w, const BitTest& v);
BitTest read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<BitTest>);
void write(::c6502::asdl::Writer& w, const LoadAddress& v);
LoadAddress read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<LoadAddress>);
void write(::c6502::asdl::Writer& w, const Comment& v);
Comment read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Comment>);
void write(::c6502::asdl::Writer& w, const Blank& v);
Blank read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Blank>);
void write(::c6502::asdl::Writer& w, const Type_operand& v);
Type_operand read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Type_operand>);
void write(::c6502::asdl::Writer& w, const Imm& v);
Imm read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Imm>);
void write(::c6502::asdl::Writer& w, const Reg& v);
Reg read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Reg>);
void write(::c6502::asdl::Writer& w, const Stack& v);
Stack read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Stack>);
void write(::c6502::asdl::Writer& w, const Frame& v);
Frame read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Frame>);
void write(::c6502::asdl::Writer& w, const Data& v);
Data read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Data>);
void write(::c6502::asdl::Writer& w, const Indirect& v);
Indirect read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Indirect>);
void write(::c6502::asdl::Writer& w, const IndirectY& v);
IndirectY read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<IndirectY>);
void write(::c6502::asdl::Writer& w, const IndirectZp& v);
IndirectZp read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<IndirectZp>);
void write(::c6502::asdl::Writer& w, const IndirectZpY& v);
IndirectZpY read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<IndirectZpY>);
void write(::c6502::asdl::Writer& w, const ZP& v);
ZP read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<ZP>);
void write(::c6502::asdl::Writer& w, const ImmLabelLow& v);
ImmLabelLow read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<ImmLabelLow>);
void write(::c6502::asdl::Writer& w, const ImmLabelHigh& v);
ImmLabelHigh read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<ImmLabelHigh>);
void write(::c6502::asdl::Writer& w, const IndexedData& v);
IndexedData read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<IndexedData>);
void write(::c6502::asdl::Writer& w, const Type_reg& v);
Type_reg read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Type_reg>);
void write(::c6502::asdl::Writer& w, const A& v);
A read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<A>);
void write(::c6502::asdl::Writer& w, const X& v);
X read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<X>);
void write(::c6502::asdl::Writer& w, const Y& v);
Y read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Y>);
void write(::c6502::asdl::Writer& w, const Type_condition& v);
Type_condition read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Type_condition>);
void write(::c6502::asdl::Writer& w, const CC& v);
CC read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<CC>);
void write(::c6502::asdl::Writer& w, const CS& v);
CS read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<CS>);
void write(::c6502::asdl::Writer& w, const EQ& v);
EQ read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<EQ>);
void write(::c6502::asdl::Writer& w, const MI& v);
MI read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<MI>);
void write(::c6502::asdl::Writer& w, const NE& v);
NE read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<NE>);
void write(::c6502::asdl::Writer& w, const PL& v);
PL read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<PL>);
void write(::c6502::asdl::Writer& w, const VC& v);
VC read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<VC>);
void write(::c6502::asdl::Writer& w, const VS& v);
VS read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<VS>);

// Python's `repr` of each value, the dataclass's: the class name
// and each field as `name=value`. `asdl::py_repr` returns it.
void repr(std::string& out, const Type_program& v);
void repr(std::string& out, const Program& v);
void repr(std::string& out, const Type_top_level& v);
void repr(std::string& out, const Function& v);
void repr(std::string& out, const StaticVariable& v);
void repr(std::string& out, const Type_static_init& v);
void repr(std::string& out, const CharInit& v);
void repr(std::string& out, const IntInit& v);
void repr(std::string& out, const LongInit& v);
void repr(std::string& out, const LongLongInit& v);
void repr(std::string& out, const FloatInit& v);
void repr(std::string& out, const DoubleInit& v);
void repr(std::string& out, const AddressInit& v);
void repr(std::string& out, const AddressByteInit& v);
void repr(std::string& out, const StringInit& v);
void repr(std::string& out, const ZeroInit& v);
void repr(std::string& out, const Type_instruction& v);
void repr(std::string& out, const Mov& v);
void repr(std::string& out, const Return& v);
void repr(std::string& out, const Add& v);
void repr(std::string& out, const Sub& v);
void repr(std::string& out, const Call& v);
void repr(std::string& out, const ClearCarry& v);
void repr(std::string& out, const SetCarry& v);
void repr(std::string& out, const Inc& v);
void repr(std::string& out, const Dec& v);
void repr(std::string& out, const Push& v);
void repr(std::string& out, const Pop& v);
void repr(std::string& out, const SetDecimal& v);
void repr(std::string& out, const ClearDecimal& v);
void repr(std::string& out, const PushStatus& v);
void repr(std::string& out, const PullStatus& v);
void repr(std::string& out, const Xor& v);
void repr(std::string& out, const And& v);
void repr(std::string& out, const Or& v);
void repr(std::string& out, const ArithmeticShiftLeft& v);
void repr(std::string& out, const LogicalShiftRight& v);
void repr(std::string& out, const RotateLeft& v);
void repr(std::string& out, const RotateRight& v);
void repr(std::string& out, const Label& v);
void repr(std::string& out, const Jump& v);
void repr(std::string& out, const Branch& v);
void repr(std::string& out, const JumpTable& v);
void repr(std::string& out, const Compare& v);
void repr(std::string& out, const BitTest& v);
void repr(std::string& out, const LoadAddress& v);
void repr(std::string& out, const Comment& v);
void repr(std::string& out, const Blank& v);
void repr(std::string& out, const Type_operand& v);
void repr(std::string& out, const Imm& v);
void repr(std::string& out, const Reg& v);
void repr(std::string& out, const Stack& v);
void repr(std::string& out, const Frame& v);
void repr(std::string& out, const Data& v);
void repr(std::string& out, const Indirect& v);
void repr(std::string& out, const IndirectY& v);
void repr(std::string& out, const IndirectZp& v);
void repr(std::string& out, const IndirectZpY& v);
void repr(std::string& out, const ZP& v);
void repr(std::string& out, const ImmLabelLow& v);
void repr(std::string& out, const ImmLabelHigh& v);
void repr(std::string& out, const IndexedData& v);
void repr(std::string& out, const Type_reg& v);
void repr(std::string& out, const A& v);
void repr(std::string& out, const X& v);
void repr(std::string& out, const Y& v);
void repr(std::string& out, const Type_condition& v);
void repr(std::string& out, const CC& v);
void repr(std::string& out, const CS& v);
void repr(std::string& out, const EQ& v);
void repr(std::string& out, const MI& v);
void repr(std::string& out, const NE& v);
void repr(std::string& out, const PL& v);
void repr(std::string& out, const VC& v);
void repr(std::string& out, const VS& v);

// pretty.py's text of each value at the indentation level `depth`:
// the class name, a `name=value,` line for each field one level
// deeper, lists one item to a line, and Python's `repr` of each leaf.
// `asdl::py_pretty` returns it.
void pretty(std::string& out, const Type_program& v, int depth);
void pretty(std::string& out, const Program& v, int depth);
void pretty(std::string& out, const Type_top_level& v, int depth);
void pretty(std::string& out, const Function& v, int depth);
void pretty(std::string& out, const StaticVariable& v, int depth);
void pretty(std::string& out, const Type_static_init& v, int depth);
void pretty(std::string& out, const CharInit& v, int depth);
void pretty(std::string& out, const IntInit& v, int depth);
void pretty(std::string& out, const LongInit& v, int depth);
void pretty(std::string& out, const LongLongInit& v, int depth);
void pretty(std::string& out, const FloatInit& v, int depth);
void pretty(std::string& out, const DoubleInit& v, int depth);
void pretty(std::string& out, const AddressInit& v, int depth);
void pretty(std::string& out, const AddressByteInit& v, int depth);
void pretty(std::string& out, const StringInit& v, int depth);
void pretty(std::string& out, const ZeroInit& v, int depth);
void pretty(std::string& out, const Type_instruction& v, int depth);
void pretty(std::string& out, const Mov& v, int depth);
void pretty(std::string& out, const Return& v, int depth);
void pretty(std::string& out, const Add& v, int depth);
void pretty(std::string& out, const Sub& v, int depth);
void pretty(std::string& out, const Call& v, int depth);
void pretty(std::string& out, const ClearCarry& v, int depth);
void pretty(std::string& out, const SetCarry& v, int depth);
void pretty(std::string& out, const Inc& v, int depth);
void pretty(std::string& out, const Dec& v, int depth);
void pretty(std::string& out, const Push& v, int depth);
void pretty(std::string& out, const Pop& v, int depth);
void pretty(std::string& out, const SetDecimal& v, int depth);
void pretty(std::string& out, const ClearDecimal& v, int depth);
void pretty(std::string& out, const PushStatus& v, int depth);
void pretty(std::string& out, const PullStatus& v, int depth);
void pretty(std::string& out, const Xor& v, int depth);
void pretty(std::string& out, const And& v, int depth);
void pretty(std::string& out, const Or& v, int depth);
void pretty(std::string& out, const ArithmeticShiftLeft& v, int depth);
void pretty(std::string& out, const LogicalShiftRight& v, int depth);
void pretty(std::string& out, const RotateLeft& v, int depth);
void pretty(std::string& out, const RotateRight& v, int depth);
void pretty(std::string& out, const Label& v, int depth);
void pretty(std::string& out, const Jump& v, int depth);
void pretty(std::string& out, const Branch& v, int depth);
void pretty(std::string& out, const JumpTable& v, int depth);
void pretty(std::string& out, const Compare& v, int depth);
void pretty(std::string& out, const BitTest& v, int depth);
void pretty(std::string& out, const LoadAddress& v, int depth);
void pretty(std::string& out, const Comment& v, int depth);
void pretty(std::string& out, const Blank& v, int depth);
void pretty(std::string& out, const Type_operand& v, int depth);
void pretty(std::string& out, const Imm& v, int depth);
void pretty(std::string& out, const Reg& v, int depth);
void pretty(std::string& out, const Stack& v, int depth);
void pretty(std::string& out, const Frame& v, int depth);
void pretty(std::string& out, const Data& v, int depth);
void pretty(std::string& out, const Indirect& v, int depth);
void pretty(std::string& out, const IndirectY& v, int depth);
void pretty(std::string& out, const IndirectZp& v, int depth);
void pretty(std::string& out, const IndirectZpY& v, int depth);
void pretty(std::string& out, const ZP& v, int depth);
void pretty(std::string& out, const ImmLabelLow& v, int depth);
void pretty(std::string& out, const ImmLabelHigh& v, int depth);
void pretty(std::string& out, const IndexedData& v, int depth);
void pretty(std::string& out, const Type_reg& v, int depth);
void pretty(std::string& out, const A& v, int depth);
void pretty(std::string& out, const X& v, int depth);
void pretty(std::string& out, const Y& v, int depth);
void pretty(std::string& out, const Type_condition& v, int depth);
void pretty(std::string& out, const CC& v, int depth);
void pretty(std::string& out, const CS& v, int depth);
void pretty(std::string& out, const EQ& v, int depth);
void pretty(std::string& out, const MI& v, int depth);
void pretty(std::string& out, const NE& v, int depth);
void pretty(std::string& out, const PL& v, int depth);
void pretty(std::string& out, const VC& v, int depth);
void pretty(std::string& out, const VS& v, int depth);

}  // namespace c6502::asm2_ast
