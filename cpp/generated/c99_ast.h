// Generated from c99.asdl by asdlgen (cpp/tools/asdlgen.h). Do not edit.
#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <variant>
#include <vector>

#include "asdl/runtime.h"

namespace c6502::c99_ast {

// const
struct ConstInt;
struct ConstLong;
struct ConstLongLong;
struct ConstUInt;
struct ConstULong;
struct ConstULongLong;
struct ConstFloat;
struct ConstDouble;
struct ConstChar;
struct ConstUChar;
struct ConstBool;
struct ConstNullptr;
struct ConstBitInt;
using Type_const = std::variant<ConstInt, ConstLong, ConstLongLong, ConstUInt, ConstULong, ConstULongLong, ConstFloat, ConstDouble, ConstChar, ConstUChar, ConstBool, ConstNullptr, ConstBitInt>;
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
struct ConstChar {
  ::c6502::asdl::Int value = 0;
  bool operator==(const ConstChar& other) const;
};
struct ConstUChar {
  ::c6502::asdl::Int value = 0;
  bool operator==(const ConstUChar& other) const;
};
struct ConstBool {
  ::c6502::asdl::Int value = 0;
  bool operator==(const ConstBool& other) const;
};
struct ConstNullptr {
  bool operator==(const ConstNullptr& other) const;
};
struct ConstBitInt {
  ::c6502::asdl::Int value = 0;
  ::c6502::asdl::Int width = 0;
  bool is_signed = false;
  bool operator==(const ConstBitInt& other) const;
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
struct Char;
struct SChar;
struct UChar;
struct Short;
struct UShort;
struct Bool;
struct NullptrT;
struct BitInt;
struct Void;
struct FunType;
struct Pointer;
struct Array;
struct Structure;
struct Union;
struct Const;
struct Volatile;
using Type_data_type = std::variant<Int, Long, LongLong, UInt, ULong, ULongLong, Float, Double, Char, SChar, UChar, Short, UShort, Bool, NullptrT, BitInt, Void, FunType, Pointer, Array, Structure, Union, Const, Volatile>;
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
struct Char {
  bool operator==(const Char& other) const;
};
struct SChar {
  bool operator==(const SChar& other) const;
};
struct UChar {
  bool operator==(const UChar& other) const;
};
struct Short {
  bool operator==(const Short& other) const;
};
struct UShort {
  bool operator==(const UShort& other) const;
};
struct Bool {
  bool operator==(const Bool& other) const;
};
struct NullptrT {
  bool operator==(const NullptrT& other) const;
};
struct BitInt {
  ::c6502::asdl::Int width = 0;
  bool is_signed = false;
  bool operator==(const BitInt& other) const;
};
struct Void {
  bool operator==(const Void& other) const;
};
struct FunType {
  std::vector<Type_data_type> params;
  ::c6502::asdl::Box<Type_data_type> ret;
  bool operator==(const FunType& other) const;
};
struct Pointer {
  ::c6502::asdl::Box<Type_data_type> referenced_type;
  bool operator==(const Pointer& other) const;
};
struct Array {
  ::c6502::asdl::Box<Type_data_type> element_type;
  ::c6502::asdl::Int size = 0;
  bool operator==(const Array& other) const;
};
struct Structure {
  std::string tag;
  bool operator==(const Structure& other) const;
};
struct Union {
  std::string tag;
  bool operator==(const Union& other) const;
};
struct Const {
  ::c6502::asdl::Box<Type_data_type> referenced_type;
  bool operator==(const Const& other) const;
};
struct Volatile {
  ::c6502::asdl::Box<Type_data_type> referenced_type;
  bool operator==(const Volatile& other) const;
};

// unary_operator
struct Complement;
struct Negate;
struct LogicalNot;
struct Plus;
using Type_unary_operator = std::variant<Complement, Negate, LogicalNot, Plus>;
struct Complement {
  bool operator==(const Complement& other) const;
};
struct Negate {
  bool operator==(const Negate& other) const;
};
struct LogicalNot {
  bool operator==(const LogicalNot& other) const;
};
struct Plus {
  bool operator==(const Plus& other) const;
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
struct LogicalAnd;
struct LogicalOr;
using Type_binary_operator = std::variant<Add, Subtract, Multiply, Divide, Modulo, BitwiseAnd, BitwiseOr, BitwiseXor, LeftShift, RightShift, Equal, NotEqual, LessThan, GreaterThan, LessOrEqual, GreaterOrEqual, LogicalAnd, LogicalOr>;
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
struct LogicalAnd {
  bool operator==(const LogicalAnd& other) const;
};
struct LogicalOr {
  bool operator==(const LogicalOr& other) const;
};

// incdec_op
struct Increment;
struct Decrement;
using Type_incdec_op = std::variant<Increment, Decrement>;
struct Increment {
  bool operator==(const Increment& other) const;
};
struct Decrement {
  bool operator==(const Decrement& other) const;
};

// designator
struct IndexDesignator;
struct MemberDesignator;
using Type_designator = std::variant<IndexDesignator, MemberDesignator>;
struct IndexDesignator {
  ::c6502::asdl::Int index = 0;
  bool operator==(const IndexDesignator& other) const;
};
struct MemberDesignator {
  std::string member;
  bool operator==(const MemberDesignator& other) const;
};

// storage_class
struct Static;
struct Extern;
struct Register;
using Type_storage_class = std::variant<Static, Extern, Register>;
struct Static {
  bool operator==(const Static& other) const;
};
struct Extern {
  bool operator==(const Extern& other) const;
};
struct Register {
  bool operator==(const Register& other) const;
};

// exp
struct Constant;
struct String;
struct Var;
struct Cast;
struct Unary;
struct Binary;
struct Assignment;
struct CompoundAssignment;
struct Postfix;
struct Prefix;
struct Conditional;
struct FunctionCall;
struct IndirectCall;
struct Dereference;
struct AddressOf;
struct Subscript;
struct InitList;
struct Designation;
struct SizeOfExp;
struct SizeOfType;
struct Dot;
struct Arrow;
struct Comma;
struct CompoundLiteral;
using Type_exp = std::variant<Constant, String, Var, Cast, Unary, Binary, Assignment, CompoundAssignment, Postfix, Prefix, Conditional, FunctionCall, IndirectCall, Dereference, AddressOf, Subscript, InitList, Designation, SizeOfExp, SizeOfType, Dot, Arrow, Comma, CompoundLiteral>;
struct Constant {
  Type_const const_;
  std::optional<Type_data_type> data_type;
  bool operator==(const Constant& other) const;
};
struct String {
  std::string str;
  bool operator==(const String& other) const;
};
struct Var {
  std::string name;
  std::optional<Type_data_type> data_type;
  bool operator==(const Var& other) const;
};
struct Cast {
  Type_data_type target_type;
  ::c6502::asdl::Box<Type_exp> exp;
  std::optional<Type_data_type> data_type;
  bool operator==(const Cast& other) const;
};
struct Unary {
  Type_unary_operator op;
  ::c6502::asdl::Box<Type_exp> exp;
  std::optional<Type_data_type> data_type;
  bool operator==(const Unary& other) const;
};
struct Binary {
  Type_binary_operator op;
  ::c6502::asdl::Box<Type_exp> left;
  ::c6502::asdl::Box<Type_exp> right;
  std::optional<Type_data_type> data_type;
  bool operator==(const Binary& other) const;
};
struct Assignment {
  ::c6502::asdl::Box<Type_exp> lval;
  ::c6502::asdl::Box<Type_exp> rval;
  std::optional<Type_data_type> data_type;
  bool operator==(const Assignment& other) const;
};
struct CompoundAssignment {
  Type_binary_operator op;
  ::c6502::asdl::Box<Type_exp> lval;
  ::c6502::asdl::Box<Type_exp> rval;
  std::optional<Type_data_type> intermediate_type;
  std::optional<Type_data_type> data_type;
  bool operator==(const CompoundAssignment& other) const;
};
struct Postfix {
  Type_incdec_op op;
  ::c6502::asdl::Box<Type_exp> operand;
  std::optional<Type_data_type> data_type;
  bool operator==(const Postfix& other) const;
};
struct Prefix {
  Type_incdec_op op;
  ::c6502::asdl::Box<Type_exp> operand;
  std::optional<Type_data_type> data_type;
  bool operator==(const Prefix& other) const;
};
struct Conditional {
  ::c6502::asdl::Box<Type_exp> condition;
  ::c6502::asdl::Box<Type_exp> true_clause;
  ::c6502::asdl::Box<Type_exp> false_clause;
  std::optional<Type_data_type> data_type;
  bool operator==(const Conditional& other) const;
};
struct FunctionCall {
  std::string name;
  std::vector<Type_exp> args;
  std::optional<Type_data_type> data_type;
  bool hw_stack_save = false;
  bool operator==(const FunctionCall& other) const;
};
struct IndirectCall {
  ::c6502::asdl::Box<Type_exp> callee;
  std::vector<Type_exp> args;
  std::optional<Type_data_type> data_type;
  bool hw_stack_save = false;
  bool operator==(const IndirectCall& other) const;
};
struct Dereference {
  ::c6502::asdl::Box<Type_exp> exp;
  std::optional<Type_data_type> data_type;
  bool operator==(const Dereference& other) const;
};
struct AddressOf {
  ::c6502::asdl::Box<Type_exp> exp;
  std::optional<Type_data_type> data_type;
  bool operator==(const AddressOf& other) const;
};
struct Subscript {
  ::c6502::asdl::Box<Type_exp> array;
  ::c6502::asdl::Box<Type_exp> index;
  std::optional<Type_data_type> data_type;
  bool operator==(const Subscript& other) const;
};
struct InitList {
  std::vector<Type_exp> items;
  std::optional<Type_data_type> data_type;
  std::optional<std::string> union_member;
  bool operator==(const InitList& other) const;
};
struct Designation {
  std::vector<Type_designator> designators;
  ::c6502::asdl::Box<Type_exp> init;
  bool operator==(const Designation& other) const;
};
struct SizeOfExp {
  ::c6502::asdl::Box<Type_exp> exp;
  std::optional<Type_data_type> data_type;
  bool operator==(const SizeOfExp& other) const;
};
struct SizeOfType {
  Type_data_type target_type;
  std::optional<Type_data_type> data_type;
  bool operator==(const SizeOfType& other) const;
};
struct Dot {
  ::c6502::asdl::Box<Type_exp> operand;
  std::string member;
  std::optional<Type_data_type> data_type;
  bool operator==(const Dot& other) const;
};
struct Arrow {
  ::c6502::asdl::Box<Type_exp> operand;
  std::string member;
  std::optional<Type_data_type> data_type;
  bool operator==(const Arrow& other) const;
};
struct Comma {
  ::c6502::asdl::Box<Type_exp> left;
  ::c6502::asdl::Box<Type_exp> right;
  std::optional<Type_data_type> data_type;
  bool operator==(const Comma& other) const;
};
struct CompoundLiteral {
  std::string name;
  Type_data_type target_type;
  ::c6502::asdl::Box<Type_exp> init;
  std::optional<Type_data_type> data_type;
  std::optional<Type_storage_class> storage_class;
  bool is_constexpr = false;
  bool operator==(const CompoundLiteral& other) const;
};

// placement
struct ZeroPage;
struct Absolute;
using Type_placement = std::variant<ZeroPage, Absolute>;
struct ZeroPage {
  std::optional<::c6502::asdl::Int> address;
  bool operator==(const ZeroPage& other) const;
};
struct Absolute {
  ::c6502::asdl::Int address = 0;
  bool operator==(const Absolute& other) const;
};

// var_decl
struct Type_var_decl;
struct Type_var_decl {
  std::string name;
  std::optional<Type_exp> init;
  Type_data_type data_type;
  std::optional<Type_storage_class> storage_class;
  std::optional<std::string> abi_annotation;
  std::optional<Type_placement> placement;
  std::optional<std::string> section;
  bool is_constexpr = false;
  bool operator==(const Type_var_decl& other) const;
};

// for_init
struct InitDecl;
struct InitExp;
using Type_for_init = std::variant<InitDecl, InitExp>;
struct InitDecl {
  std::vector<Type_var_decl> var_decls;
  bool operator==(const InitDecl& other) const;
};
struct InitExp {
  std::optional<Type_exp> exp;
  bool operator==(const InitExp& other) const;
};

// switch_case
struct Type_switch_case;
struct Type_switch_case {
  Type_exp value;
  std::string label;
  bool operator==(const Type_switch_case& other) const;
};

// member_decl
struct Type_member_decl;
struct Type_member_decl {
  std::string name;
  Type_data_type data_type;
  bool operator==(const Type_member_decl& other) const;
};

// struct_decl
struct Type_struct_decl;
struct Type_struct_decl {
  std::string tag;
  bool is_union = false;
  std::vector<Type_member_decl> members;
  bool operator==(const Type_struct_decl& other) const;
};

// block, block_item, statement, declaration, function_decl
struct Block;
struct S;
struct D;
struct Return;
struct Expression;
struct IfStmt;
struct Compound;
struct BreakStmt;
struct ContinueStmt;
struct WhileStmt;
struct DoWhileStmt;
struct ForStmt;
struct SwitchStmt;
struct CaseStmt;
struct DefaultStmt;
struct Goto;
struct LabeledStmt;
struct Null;
struct FunctionDecl;
struct VarDecl;
struct StructDecl;
struct Type_function_decl;
using Type_block = std::variant<Block>;
using Type_block_item = std::variant<S, D>;
using Type_statement = std::variant<Return, Expression, IfStmt, Compound, BreakStmt, ContinueStmt, WhileStmt, DoWhileStmt, ForStmt, SwitchStmt, CaseStmt, DefaultStmt, Goto, LabeledStmt, Null>;
using Type_declaration = std::variant<FunctionDecl, VarDecl, StructDecl>;
struct Block {
  std::vector<Type_block_item> block_item;
  bool operator==(const Block& other) const;
};
struct S {
  ::c6502::asdl::Box<Type_statement> statement;
  bool operator==(const S& other) const;
};
struct D {
  ::c6502::asdl::Box<Type_declaration> declaration;
  bool operator==(const D& other) const;
};
struct Return {
  std::optional<Type_exp> exp;
  bool operator==(const Return& other) const;
};
struct Expression {
  Type_exp exp;
  bool operator==(const Expression& other) const;
};
struct IfStmt {
  Type_exp condition;
  ::c6502::asdl::Box<Type_statement> then_clause;
  std::optional<::c6502::asdl::Box<Type_statement>> else_clause;
  bool operator==(const IfStmt& other) const;
};
struct Compound {
  ::c6502::asdl::Box<Type_block> block;
  bool operator==(const Compound& other) const;
};
struct BreakStmt {
  std::string label;
  bool operator==(const BreakStmt& other) const;
};
struct ContinueStmt {
  std::string label;
  bool operator==(const ContinueStmt& other) const;
};
struct WhileStmt {
  Type_exp condition;
  ::c6502::asdl::Box<Type_statement> body;
  std::string label;
  bool operator==(const WhileStmt& other) const;
};
struct DoWhileStmt {
  ::c6502::asdl::Box<Type_statement> body;
  Type_exp condition;
  std::string label;
  bool operator==(const DoWhileStmt& other) const;
};
struct ForStmt {
  Type_for_init init;
  std::optional<Type_exp> condition;
  std::optional<Type_exp> post_clause;
  ::c6502::asdl::Box<Type_statement> body;
  std::string label;
  std::optional<std::string> loop_attribute;
  bool operator==(const ForStmt& other) const;
};
struct SwitchStmt {
  Type_exp control;
  ::c6502::asdl::Box<Type_statement> body;
  std::string label;
  std::vector<Type_switch_case> cases;
  std::optional<std::string> default_label;
  std::optional<Type_data_type> promoted_type;
  bool operator==(const SwitchStmt& other) const;
};
struct CaseStmt {
  Type_exp value;
  ::c6502::asdl::Box<Type_statement> body;
  std::string label;
  bool operator==(const CaseStmt& other) const;
};
struct DefaultStmt {
  ::c6502::asdl::Box<Type_statement> body;
  std::string label;
  bool operator==(const DefaultStmt& other) const;
};
struct Goto {
  std::string label;
  bool operator==(const Goto& other) const;
};
struct LabeledStmt {
  std::string label;
  ::c6502::asdl::Box<Type_statement> statement;
  bool operator==(const LabeledStmt& other) const;
};
struct Null {
  bool operator==(const Null& other) const;
};
struct FunctionDecl {
  ::c6502::asdl::Box<Type_function_decl> function_decl;
  bool operator==(const FunctionDecl& other) const;
};
struct VarDecl {
  Type_var_decl var_decl;
  bool operator==(const VarDecl& other) const;
};
struct StructDecl {
  Type_struct_decl struct_decl;
  bool operator==(const StructDecl& other) const;
};
struct Type_function_decl {
  std::string name;
  std::vector<std::string> params;
  std::optional<::c6502::asdl::Box<Type_block>> body;
  Type_data_type data_type;
  std::optional<Type_storage_class> storage_class;
  std::optional<std::string> abi_annotation;
  std::vector<std::string> param_registers;
  bool carry_return = false;
  std::optional<::c6502::asdl::Int> address;
  std::vector<std::string> preserves;
  std::optional<std::string> section;
  bool noreturn = false;
  bool operator==(const Type_function_decl& other) const;
};

// program
struct Program;
using Type_program = std::variant<Program>;
struct Program {
  std::vector<Type_declaration> declaration;
  bool operator==(const Program& other) const;
};

// function_definition
struct Function;
using Type_function_definition = std::variant<Function>;
struct Function {
  std::string name;
  std::vector<std::string> params;
  Type_block body;
  bool operator==(const Function& other) const;
};

// The constructor of a sum type's value, numbered as the
// variant's alternatives are, for a switch.
enum class Kind_program : std::uint8_t {
  Program,
};
inline Kind_program kind(const Type_program& v) noexcept {
  return static_cast<Kind_program>(v.index());
}
enum class Kind_function_definition : std::uint8_t {
  Function,
};
inline Kind_function_definition kind(const Type_function_definition& v) noexcept {
  return static_cast<Kind_function_definition>(v.index());
}
enum class Kind_block : std::uint8_t {
  Block,
};
inline Kind_block kind(const Type_block& v) noexcept {
  return static_cast<Kind_block>(v.index());
}
enum class Kind_block_item : std::uint8_t {
  S,
  D,
};
inline Kind_block_item kind(const Type_block_item& v) noexcept {
  return static_cast<Kind_block_item>(v.index());
}
enum class Kind_statement : std::uint8_t {
  Return,
  Expression,
  IfStmt,
  Compound,
  BreakStmt,
  ContinueStmt,
  WhileStmt,
  DoWhileStmt,
  ForStmt,
  SwitchStmt,
  CaseStmt,
  DefaultStmt,
  Goto,
  LabeledStmt,
  Null,
};
inline Kind_statement kind(const Type_statement& v) noexcept {
  return static_cast<Kind_statement>(v.index());
}
enum class Kind_declaration : std::uint8_t {
  FunctionDecl,
  VarDecl,
  StructDecl,
};
inline Kind_declaration kind(const Type_declaration& v) noexcept {
  return static_cast<Kind_declaration>(v.index());
}
enum class Kind_storage_class : std::uint8_t {
  Static,
  Extern,
  Register,
};
inline Kind_storage_class kind(const Type_storage_class& v) noexcept {
  return static_cast<Kind_storage_class>(v.index());
}
enum class Kind_placement : std::uint8_t {
  ZeroPage,
  Absolute,
};
inline Kind_placement kind(const Type_placement& v) noexcept {
  return static_cast<Kind_placement>(v.index());
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
  Char,
  SChar,
  UChar,
  Short,
  UShort,
  Bool,
  NullptrT,
  BitInt,
  Void,
  FunType,
  Pointer,
  Array,
  Structure,
  Union,
  Const,
  Volatile,
};
inline Kind_data_type kind(const Type_data_type& v) noexcept {
  return static_cast<Kind_data_type>(v.index());
}
enum class Kind_for_init : std::uint8_t {
  InitDecl,
  InitExp,
};
inline Kind_for_init kind(const Type_for_init& v) noexcept {
  return static_cast<Kind_for_init>(v.index());
}
enum class Kind_exp : std::uint8_t {
  Constant,
  String,
  Var,
  Cast,
  Unary,
  Binary,
  Assignment,
  CompoundAssignment,
  Postfix,
  Prefix,
  Conditional,
  FunctionCall,
  IndirectCall,
  Dereference,
  AddressOf,
  Subscript,
  InitList,
  Designation,
  SizeOfExp,
  SizeOfType,
  Dot,
  Arrow,
  Comma,
  CompoundLiteral,
};
inline Kind_exp kind(const Type_exp& v) noexcept {
  return static_cast<Kind_exp>(v.index());
}
enum class Kind_designator : std::uint8_t {
  IndexDesignator,
  MemberDesignator,
};
inline Kind_designator kind(const Type_designator& v) noexcept {
  return static_cast<Kind_designator>(v.index());
}
enum class Kind_unary_operator : std::uint8_t {
  Complement,
  Negate,
  LogicalNot,
  Plus,
};
inline Kind_unary_operator kind(const Type_unary_operator& v) noexcept {
  return static_cast<Kind_unary_operator>(v.index());
}
enum class Kind_incdec_op : std::uint8_t {
  Increment,
  Decrement,
};
inline Kind_incdec_op kind(const Type_incdec_op& v) noexcept {
  return static_cast<Kind_incdec_op>(v.index());
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
  LogicalAnd,
  LogicalOr,
};
inline Kind_binary_operator kind(const Type_binary_operator& v) noexcept {
  return static_cast<Kind_binary_operator>(v.index());
}
enum class Kind_const : std::uint8_t {
  ConstInt,
  ConstLong,
  ConstLongLong,
  ConstUInt,
  ConstULong,
  ConstULongLong,
  ConstFloat,
  ConstDouble,
  ConstChar,
  ConstUChar,
  ConstBool,
  ConstNullptr,
  ConstBitInt,
};
inline Kind_const kind(const Type_const& v) noexcept {
  return static_cast<Kind_const>(v.index());
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
inline bool ConstChar::operator==(const ConstChar& other) const {
  return value == other.value;
}
inline bool ConstUChar::operator==(const ConstUChar& other) const {
  return value == other.value;
}
inline bool ConstBool::operator==(const ConstBool& other) const {
  return value == other.value;
}
inline bool ConstNullptr::operator==(const ConstNullptr&) const {
  return true;
}
inline bool ConstBitInt::operator==(const ConstBitInt& other) const {
  return value == other.value
      && width == other.width
      && is_signed == other.is_signed;
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
inline bool Char::operator==(const Char&) const {
  return true;
}
inline bool SChar::operator==(const SChar&) const {
  return true;
}
inline bool UChar::operator==(const UChar&) const {
  return true;
}
inline bool Short::operator==(const Short&) const {
  return true;
}
inline bool UShort::operator==(const UShort&) const {
  return true;
}
inline bool Bool::operator==(const Bool&) const {
  return true;
}
inline bool NullptrT::operator==(const NullptrT&) const {
  return true;
}
inline bool BitInt::operator==(const BitInt& other) const {
  return width == other.width
      && is_signed == other.is_signed;
}
inline bool Void::operator==(const Void&) const {
  return true;
}
inline bool FunType::operator==(const FunType& other) const {
  return params == other.params
      && ret == other.ret;
}
inline bool Pointer::operator==(const Pointer& other) const {
  return referenced_type == other.referenced_type;
}
inline bool Array::operator==(const Array& other) const {
  return element_type == other.element_type
      && size == other.size;
}
inline bool Structure::operator==(const Structure& other) const {
  return tag == other.tag;
}
inline bool Union::operator==(const Union& other) const {
  return tag == other.tag;
}
inline bool Const::operator==(const Const& other) const {
  return referenced_type == other.referenced_type;
}
inline bool Volatile::operator==(const Volatile& other) const {
  return referenced_type == other.referenced_type;
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
inline bool Plus::operator==(const Plus&) const {
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
inline bool LogicalAnd::operator==(const LogicalAnd&) const {
  return true;
}
inline bool LogicalOr::operator==(const LogicalOr&) const {
  return true;
}
inline bool Increment::operator==(const Increment&) const {
  return true;
}
inline bool Decrement::operator==(const Decrement&) const {
  return true;
}
inline bool IndexDesignator::operator==(const IndexDesignator& other) const {
  return index == other.index;
}
inline bool MemberDesignator::operator==(const MemberDesignator& other) const {
  return member == other.member;
}
inline bool Static::operator==(const Static&) const {
  return true;
}
inline bool Extern::operator==(const Extern&) const {
  return true;
}
inline bool Register::operator==(const Register&) const {
  return true;
}
inline bool Constant::operator==(const Constant& other) const {
  return const_ == other.const_
      && data_type == other.data_type;
}
inline bool String::operator==(const String& other) const {
  return str == other.str;
}
inline bool Var::operator==(const Var& other) const {
  return name == other.name
      && data_type == other.data_type;
}
inline bool Cast::operator==(const Cast& other) const {
  return target_type == other.target_type
      && exp == other.exp
      && data_type == other.data_type;
}
inline bool Unary::operator==(const Unary& other) const {
  return op == other.op
      && exp == other.exp
      && data_type == other.data_type;
}
inline bool Binary::operator==(const Binary& other) const {
  return op == other.op
      && left == other.left
      && right == other.right
      && data_type == other.data_type;
}
inline bool Assignment::operator==(const Assignment& other) const {
  return lval == other.lval
      && rval == other.rval
      && data_type == other.data_type;
}
inline bool CompoundAssignment::operator==(const CompoundAssignment& other) const {
  return op == other.op
      && lval == other.lval
      && rval == other.rval
      && intermediate_type == other.intermediate_type
      && data_type == other.data_type;
}
inline bool Postfix::operator==(const Postfix& other) const {
  return op == other.op
      && operand == other.operand
      && data_type == other.data_type;
}
inline bool Prefix::operator==(const Prefix& other) const {
  return op == other.op
      && operand == other.operand
      && data_type == other.data_type;
}
inline bool Conditional::operator==(const Conditional& other) const {
  return condition == other.condition
      && true_clause == other.true_clause
      && false_clause == other.false_clause
      && data_type == other.data_type;
}
inline bool FunctionCall::operator==(const FunctionCall& other) const {
  return name == other.name
      && args == other.args
      && data_type == other.data_type
      && hw_stack_save == other.hw_stack_save;
}
inline bool IndirectCall::operator==(const IndirectCall& other) const {
  return callee == other.callee
      && args == other.args
      && data_type == other.data_type
      && hw_stack_save == other.hw_stack_save;
}
inline bool Dereference::operator==(const Dereference& other) const {
  return exp == other.exp
      && data_type == other.data_type;
}
inline bool AddressOf::operator==(const AddressOf& other) const {
  return exp == other.exp
      && data_type == other.data_type;
}
inline bool Subscript::operator==(const Subscript& other) const {
  return array == other.array
      && index == other.index
      && data_type == other.data_type;
}
inline bool InitList::operator==(const InitList& other) const {
  return items == other.items
      && data_type == other.data_type
      && union_member == other.union_member;
}
inline bool Designation::operator==(const Designation& other) const {
  return designators == other.designators
      && init == other.init;
}
inline bool SizeOfExp::operator==(const SizeOfExp& other) const {
  return exp == other.exp
      && data_type == other.data_type;
}
inline bool SizeOfType::operator==(const SizeOfType& other) const {
  return target_type == other.target_type
      && data_type == other.data_type;
}
inline bool Dot::operator==(const Dot& other) const {
  return operand == other.operand
      && member == other.member
      && data_type == other.data_type;
}
inline bool Arrow::operator==(const Arrow& other) const {
  return operand == other.operand
      && member == other.member
      && data_type == other.data_type;
}
inline bool Comma::operator==(const Comma& other) const {
  return left == other.left
      && right == other.right
      && data_type == other.data_type;
}
inline bool CompoundLiteral::operator==(const CompoundLiteral& other) const {
  return name == other.name
      && target_type == other.target_type
      && init == other.init
      && data_type == other.data_type
      && storage_class == other.storage_class
      && is_constexpr == other.is_constexpr;
}
inline bool ZeroPage::operator==(const ZeroPage& other) const {
  return address == other.address;
}
inline bool Absolute::operator==(const Absolute& other) const {
  return address == other.address;
}
inline bool Type_var_decl::operator==(const Type_var_decl& other) const {
  return name == other.name
      && init == other.init
      && data_type == other.data_type
      && storage_class == other.storage_class
      && abi_annotation == other.abi_annotation
      && placement == other.placement
      && section == other.section
      && is_constexpr == other.is_constexpr;
}
inline bool InitDecl::operator==(const InitDecl& other) const {
  return var_decls == other.var_decls;
}
inline bool InitExp::operator==(const InitExp& other) const {
  return exp == other.exp;
}
inline bool Type_switch_case::operator==(const Type_switch_case& other) const {
  return value == other.value
      && label == other.label;
}
inline bool Type_member_decl::operator==(const Type_member_decl& other) const {
  return name == other.name
      && data_type == other.data_type;
}
inline bool Type_struct_decl::operator==(const Type_struct_decl& other) const {
  return tag == other.tag
      && is_union == other.is_union
      && members == other.members;
}
inline bool Block::operator==(const Block& other) const {
  return block_item == other.block_item;
}
inline bool S::operator==(const S& other) const {
  return statement == other.statement;
}
inline bool D::operator==(const D& other) const {
  return declaration == other.declaration;
}
inline bool Return::operator==(const Return& other) const {
  return exp == other.exp;
}
inline bool Expression::operator==(const Expression& other) const {
  return exp == other.exp;
}
inline bool IfStmt::operator==(const IfStmt& other) const {
  return condition == other.condition
      && then_clause == other.then_clause
      && else_clause == other.else_clause;
}
inline bool Compound::operator==(const Compound& other) const {
  return block == other.block;
}
inline bool BreakStmt::operator==(const BreakStmt& other) const {
  return label == other.label;
}
inline bool ContinueStmt::operator==(const ContinueStmt& other) const {
  return label == other.label;
}
inline bool WhileStmt::operator==(const WhileStmt& other) const {
  return condition == other.condition
      && body == other.body
      && label == other.label;
}
inline bool DoWhileStmt::operator==(const DoWhileStmt& other) const {
  return body == other.body
      && condition == other.condition
      && label == other.label;
}
inline bool ForStmt::operator==(const ForStmt& other) const {
  return init == other.init
      && condition == other.condition
      && post_clause == other.post_clause
      && body == other.body
      && label == other.label
      && loop_attribute == other.loop_attribute;
}
inline bool SwitchStmt::operator==(const SwitchStmt& other) const {
  return control == other.control
      && body == other.body
      && label == other.label
      && cases == other.cases
      && default_label == other.default_label
      && promoted_type == other.promoted_type;
}
inline bool CaseStmt::operator==(const CaseStmt& other) const {
  return value == other.value
      && body == other.body
      && label == other.label;
}
inline bool DefaultStmt::operator==(const DefaultStmt& other) const {
  return body == other.body
      && label == other.label;
}
inline bool Goto::operator==(const Goto& other) const {
  return label == other.label;
}
inline bool LabeledStmt::operator==(const LabeledStmt& other) const {
  return label == other.label
      && statement == other.statement;
}
inline bool Null::operator==(const Null&) const {
  return true;
}
inline bool FunctionDecl::operator==(const FunctionDecl& other) const {
  return function_decl == other.function_decl;
}
inline bool VarDecl::operator==(const VarDecl& other) const {
  return var_decl == other.var_decl;
}
inline bool StructDecl::operator==(const StructDecl& other) const {
  return struct_decl == other.struct_decl;
}
inline bool Type_function_decl::operator==(const Type_function_decl& other) const {
  return name == other.name
      && params == other.params
      && body == other.body
      && data_type == other.data_type
      && storage_class == other.storage_class
      && abi_annotation == other.abi_annotation
      && param_registers == other.param_registers
      && carry_return == other.carry_return
      && address == other.address
      && preserves == other.preserves
      && section == other.section
      && noreturn == other.noreturn;
}
inline bool Program::operator==(const Program& other) const {
  return declaration == other.declaration;
}
inline bool Function::operator==(const Function& other) const {
  return name == other.name
      && params == other.params
      && body == other.body;
}

// The dump format's writers and readers (ir_dump.py describes
// the format). `asdl::dump` and `asdl::load` call them.
void write(::c6502::asdl::Writer& w, const Type_program& v);
Type_program read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Type_program>);
void write(::c6502::asdl::Writer& w, const Program& v);
Program read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Program>);
void write(::c6502::asdl::Writer& w, const Type_function_definition& v);
Type_function_definition read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Type_function_definition>);
void write(::c6502::asdl::Writer& w, const Function& v);
Function read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Function>);
void write(::c6502::asdl::Writer& w, const Type_block& v);
Type_block read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Type_block>);
void write(::c6502::asdl::Writer& w, const Block& v);
Block read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Block>);
void write(::c6502::asdl::Writer& w, const Type_block_item& v);
Type_block_item read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Type_block_item>);
void write(::c6502::asdl::Writer& w, const S& v);
S read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<S>);
void write(::c6502::asdl::Writer& w, const D& v);
D read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<D>);
void write(::c6502::asdl::Writer& w, const Type_statement& v);
Type_statement read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Type_statement>);
void write(::c6502::asdl::Writer& w, const Return& v);
Return read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Return>);
void write(::c6502::asdl::Writer& w, const Expression& v);
Expression read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Expression>);
void write(::c6502::asdl::Writer& w, const IfStmt& v);
IfStmt read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<IfStmt>);
void write(::c6502::asdl::Writer& w, const Compound& v);
Compound read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Compound>);
void write(::c6502::asdl::Writer& w, const BreakStmt& v);
BreakStmt read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<BreakStmt>);
void write(::c6502::asdl::Writer& w, const ContinueStmt& v);
ContinueStmt read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<ContinueStmt>);
void write(::c6502::asdl::Writer& w, const WhileStmt& v);
WhileStmt read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<WhileStmt>);
void write(::c6502::asdl::Writer& w, const DoWhileStmt& v);
DoWhileStmt read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<DoWhileStmt>);
void write(::c6502::asdl::Writer& w, const ForStmt& v);
ForStmt read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<ForStmt>);
void write(::c6502::asdl::Writer& w, const SwitchStmt& v);
SwitchStmt read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<SwitchStmt>);
void write(::c6502::asdl::Writer& w, const CaseStmt& v);
CaseStmt read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<CaseStmt>);
void write(::c6502::asdl::Writer& w, const DefaultStmt& v);
DefaultStmt read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<DefaultStmt>);
void write(::c6502::asdl::Writer& w, const Goto& v);
Goto read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Goto>);
void write(::c6502::asdl::Writer& w, const LabeledStmt& v);
LabeledStmt read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<LabeledStmt>);
void write(::c6502::asdl::Writer& w, const Null& v);
Null read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Null>);
void write(::c6502::asdl::Writer& w, const Type_switch_case& v);
Type_switch_case read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Type_switch_case>);
void write(::c6502::asdl::Writer& w, const Type_declaration& v);
Type_declaration read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Type_declaration>);
void write(::c6502::asdl::Writer& w, const FunctionDecl& v);
FunctionDecl read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<FunctionDecl>);
void write(::c6502::asdl::Writer& w, const VarDecl& v);
VarDecl read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<VarDecl>);
void write(::c6502::asdl::Writer& w, const StructDecl& v);
StructDecl read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<StructDecl>);
void write(::c6502::asdl::Writer& w, const Type_var_decl& v);
Type_var_decl read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Type_var_decl>);
void write(::c6502::asdl::Writer& w, const Type_function_decl& v);
Type_function_decl read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Type_function_decl>);
void write(::c6502::asdl::Writer& w, const Type_struct_decl& v);
Type_struct_decl read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Type_struct_decl>);
void write(::c6502::asdl::Writer& w, const Type_member_decl& v);
Type_member_decl read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Type_member_decl>);
void write(::c6502::asdl::Writer& w, const Type_storage_class& v);
Type_storage_class read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Type_storage_class>);
void write(::c6502::asdl::Writer& w, const Static& v);
Static read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Static>);
void write(::c6502::asdl::Writer& w, const Extern& v);
Extern read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Extern>);
void write(::c6502::asdl::Writer& w, const Register& v);
Register read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Register>);
void write(::c6502::asdl::Writer& w, const Type_placement& v);
Type_placement read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Type_placement>);
void write(::c6502::asdl::Writer& w, const ZeroPage& v);
ZeroPage read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<ZeroPage>);
void write(::c6502::asdl::Writer& w, const Absolute& v);
Absolute read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Absolute>);
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
void write(::c6502::asdl::Writer& w, const Char& v);
Char read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Char>);
void write(::c6502::asdl::Writer& w, const SChar& v);
SChar read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<SChar>);
void write(::c6502::asdl::Writer& w, const UChar& v);
UChar read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<UChar>);
void write(::c6502::asdl::Writer& w, const Short& v);
Short read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Short>);
void write(::c6502::asdl::Writer& w, const UShort& v);
UShort read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<UShort>);
void write(::c6502::asdl::Writer& w, const Bool& v);
Bool read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Bool>);
void write(::c6502::asdl::Writer& w, const NullptrT& v);
NullptrT read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<NullptrT>);
void write(::c6502::asdl::Writer& w, const BitInt& v);
BitInt read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<BitInt>);
void write(::c6502::asdl::Writer& w, const Void& v);
Void read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Void>);
void write(::c6502::asdl::Writer& w, const FunType& v);
FunType read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<FunType>);
void write(::c6502::asdl::Writer& w, const Pointer& v);
Pointer read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Pointer>);
void write(::c6502::asdl::Writer& w, const Array& v);
Array read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Array>);
void write(::c6502::asdl::Writer& w, const Structure& v);
Structure read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Structure>);
void write(::c6502::asdl::Writer& w, const Union& v);
Union read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Union>);
void write(::c6502::asdl::Writer& w, const Const& v);
Const read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Const>);
void write(::c6502::asdl::Writer& w, const Volatile& v);
Volatile read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Volatile>);
void write(::c6502::asdl::Writer& w, const Type_for_init& v);
Type_for_init read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Type_for_init>);
void write(::c6502::asdl::Writer& w, const InitDecl& v);
InitDecl read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<InitDecl>);
void write(::c6502::asdl::Writer& w, const InitExp& v);
InitExp read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<InitExp>);
void write(::c6502::asdl::Writer& w, const Type_exp& v);
Type_exp read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Type_exp>);
void write(::c6502::asdl::Writer& w, const Constant& v);
Constant read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Constant>);
void write(::c6502::asdl::Writer& w, const String& v);
String read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<String>);
void write(::c6502::asdl::Writer& w, const Var& v);
Var read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Var>);
void write(::c6502::asdl::Writer& w, const Cast& v);
Cast read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Cast>);
void write(::c6502::asdl::Writer& w, const Unary& v);
Unary read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Unary>);
void write(::c6502::asdl::Writer& w, const Binary& v);
Binary read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Binary>);
void write(::c6502::asdl::Writer& w, const Assignment& v);
Assignment read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Assignment>);
void write(::c6502::asdl::Writer& w, const CompoundAssignment& v);
CompoundAssignment read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<CompoundAssignment>);
void write(::c6502::asdl::Writer& w, const Postfix& v);
Postfix read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Postfix>);
void write(::c6502::asdl::Writer& w, const Prefix& v);
Prefix read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Prefix>);
void write(::c6502::asdl::Writer& w, const Conditional& v);
Conditional read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Conditional>);
void write(::c6502::asdl::Writer& w, const FunctionCall& v);
FunctionCall read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<FunctionCall>);
void write(::c6502::asdl::Writer& w, const IndirectCall& v);
IndirectCall read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<IndirectCall>);
void write(::c6502::asdl::Writer& w, const Dereference& v);
Dereference read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Dereference>);
void write(::c6502::asdl::Writer& w, const AddressOf& v);
AddressOf read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<AddressOf>);
void write(::c6502::asdl::Writer& w, const Subscript& v);
Subscript read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Subscript>);
void write(::c6502::asdl::Writer& w, const InitList& v);
InitList read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<InitList>);
void write(::c6502::asdl::Writer& w, const Designation& v);
Designation read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Designation>);
void write(::c6502::asdl::Writer& w, const SizeOfExp& v);
SizeOfExp read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<SizeOfExp>);
void write(::c6502::asdl::Writer& w, const SizeOfType& v);
SizeOfType read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<SizeOfType>);
void write(::c6502::asdl::Writer& w, const Dot& v);
Dot read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Dot>);
void write(::c6502::asdl::Writer& w, const Arrow& v);
Arrow read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Arrow>);
void write(::c6502::asdl::Writer& w, const Comma& v);
Comma read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Comma>);
void write(::c6502::asdl::Writer& w, const CompoundLiteral& v);
CompoundLiteral read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<CompoundLiteral>);
void write(::c6502::asdl::Writer& w, const Type_designator& v);
Type_designator read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Type_designator>);
void write(::c6502::asdl::Writer& w, const IndexDesignator& v);
IndexDesignator read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<IndexDesignator>);
void write(::c6502::asdl::Writer& w, const MemberDesignator& v);
MemberDesignator read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<MemberDesignator>);
void write(::c6502::asdl::Writer& w, const Type_unary_operator& v);
Type_unary_operator read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Type_unary_operator>);
void write(::c6502::asdl::Writer& w, const Complement& v);
Complement read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Complement>);
void write(::c6502::asdl::Writer& w, const Negate& v);
Negate read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Negate>);
void write(::c6502::asdl::Writer& w, const LogicalNot& v);
LogicalNot read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<LogicalNot>);
void write(::c6502::asdl::Writer& w, const Plus& v);
Plus read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Plus>);
void write(::c6502::asdl::Writer& w, const Type_incdec_op& v);
Type_incdec_op read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Type_incdec_op>);
void write(::c6502::asdl::Writer& w, const Increment& v);
Increment read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Increment>);
void write(::c6502::asdl::Writer& w, const Decrement& v);
Decrement read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Decrement>);
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
void write(::c6502::asdl::Writer& w, const LogicalAnd& v);
LogicalAnd read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<LogicalAnd>);
void write(::c6502::asdl::Writer& w, const LogicalOr& v);
LogicalOr read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<LogicalOr>);
void write(::c6502::asdl::Writer& w, const Type_const& v);
Type_const read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Type_const>);
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
void write(::c6502::asdl::Writer& w, const ConstChar& v);
ConstChar read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<ConstChar>);
void write(::c6502::asdl::Writer& w, const ConstUChar& v);
ConstUChar read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<ConstUChar>);
void write(::c6502::asdl::Writer& w, const ConstBool& v);
ConstBool read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<ConstBool>);
void write(::c6502::asdl::Writer& w, const ConstNullptr& v);
ConstNullptr read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<ConstNullptr>);
void write(::c6502::asdl::Writer& w, const ConstBitInt& v);
ConstBitInt read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<ConstBitInt>);

// Python's `repr` of each value, the dataclass's: the class name
// and each field as `name=value`. `asdl::py_repr` returns it.
void repr(std::string& out, const Type_program& v);
void repr(std::string& out, const Program& v);
void repr(std::string& out, const Type_function_definition& v);
void repr(std::string& out, const Function& v);
void repr(std::string& out, const Type_block& v);
void repr(std::string& out, const Block& v);
void repr(std::string& out, const Type_block_item& v);
void repr(std::string& out, const S& v);
void repr(std::string& out, const D& v);
void repr(std::string& out, const Type_statement& v);
void repr(std::string& out, const Return& v);
void repr(std::string& out, const Expression& v);
void repr(std::string& out, const IfStmt& v);
void repr(std::string& out, const Compound& v);
void repr(std::string& out, const BreakStmt& v);
void repr(std::string& out, const ContinueStmt& v);
void repr(std::string& out, const WhileStmt& v);
void repr(std::string& out, const DoWhileStmt& v);
void repr(std::string& out, const ForStmt& v);
void repr(std::string& out, const SwitchStmt& v);
void repr(std::string& out, const CaseStmt& v);
void repr(std::string& out, const DefaultStmt& v);
void repr(std::string& out, const Goto& v);
void repr(std::string& out, const LabeledStmt& v);
void repr(std::string& out, const Null& v);
void repr(std::string& out, const Type_switch_case& v);
void repr(std::string& out, const Type_declaration& v);
void repr(std::string& out, const FunctionDecl& v);
void repr(std::string& out, const VarDecl& v);
void repr(std::string& out, const StructDecl& v);
void repr(std::string& out, const Type_var_decl& v);
void repr(std::string& out, const Type_function_decl& v);
void repr(std::string& out, const Type_struct_decl& v);
void repr(std::string& out, const Type_member_decl& v);
void repr(std::string& out, const Type_storage_class& v);
void repr(std::string& out, const Static& v);
void repr(std::string& out, const Extern& v);
void repr(std::string& out, const Register& v);
void repr(std::string& out, const Type_placement& v);
void repr(std::string& out, const ZeroPage& v);
void repr(std::string& out, const Absolute& v);
void repr(std::string& out, const Type_data_type& v);
void repr(std::string& out, const Int& v);
void repr(std::string& out, const Long& v);
void repr(std::string& out, const LongLong& v);
void repr(std::string& out, const UInt& v);
void repr(std::string& out, const ULong& v);
void repr(std::string& out, const ULongLong& v);
void repr(std::string& out, const Float& v);
void repr(std::string& out, const Double& v);
void repr(std::string& out, const Char& v);
void repr(std::string& out, const SChar& v);
void repr(std::string& out, const UChar& v);
void repr(std::string& out, const Short& v);
void repr(std::string& out, const UShort& v);
void repr(std::string& out, const Bool& v);
void repr(std::string& out, const NullptrT& v);
void repr(std::string& out, const BitInt& v);
void repr(std::string& out, const Void& v);
void repr(std::string& out, const FunType& v);
void repr(std::string& out, const Pointer& v);
void repr(std::string& out, const Array& v);
void repr(std::string& out, const Structure& v);
void repr(std::string& out, const Union& v);
void repr(std::string& out, const Const& v);
void repr(std::string& out, const Volatile& v);
void repr(std::string& out, const Type_for_init& v);
void repr(std::string& out, const InitDecl& v);
void repr(std::string& out, const InitExp& v);
void repr(std::string& out, const Type_exp& v);
void repr(std::string& out, const Constant& v);
void repr(std::string& out, const String& v);
void repr(std::string& out, const Var& v);
void repr(std::string& out, const Cast& v);
void repr(std::string& out, const Unary& v);
void repr(std::string& out, const Binary& v);
void repr(std::string& out, const Assignment& v);
void repr(std::string& out, const CompoundAssignment& v);
void repr(std::string& out, const Postfix& v);
void repr(std::string& out, const Prefix& v);
void repr(std::string& out, const Conditional& v);
void repr(std::string& out, const FunctionCall& v);
void repr(std::string& out, const IndirectCall& v);
void repr(std::string& out, const Dereference& v);
void repr(std::string& out, const AddressOf& v);
void repr(std::string& out, const Subscript& v);
void repr(std::string& out, const InitList& v);
void repr(std::string& out, const Designation& v);
void repr(std::string& out, const SizeOfExp& v);
void repr(std::string& out, const SizeOfType& v);
void repr(std::string& out, const Dot& v);
void repr(std::string& out, const Arrow& v);
void repr(std::string& out, const Comma& v);
void repr(std::string& out, const CompoundLiteral& v);
void repr(std::string& out, const Type_designator& v);
void repr(std::string& out, const IndexDesignator& v);
void repr(std::string& out, const MemberDesignator& v);
void repr(std::string& out, const Type_unary_operator& v);
void repr(std::string& out, const Complement& v);
void repr(std::string& out, const Negate& v);
void repr(std::string& out, const LogicalNot& v);
void repr(std::string& out, const Plus& v);
void repr(std::string& out, const Type_incdec_op& v);
void repr(std::string& out, const Increment& v);
void repr(std::string& out, const Decrement& v);
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
void repr(std::string& out, const LogicalAnd& v);
void repr(std::string& out, const LogicalOr& v);
void repr(std::string& out, const Type_const& v);
void repr(std::string& out, const ConstInt& v);
void repr(std::string& out, const ConstLong& v);
void repr(std::string& out, const ConstLongLong& v);
void repr(std::string& out, const ConstUInt& v);
void repr(std::string& out, const ConstULong& v);
void repr(std::string& out, const ConstULongLong& v);
void repr(std::string& out, const ConstFloat& v);
void repr(std::string& out, const ConstDouble& v);
void repr(std::string& out, const ConstChar& v);
void repr(std::string& out, const ConstUChar& v);
void repr(std::string& out, const ConstBool& v);
void repr(std::string& out, const ConstNullptr& v);
void repr(std::string& out, const ConstBitInt& v);

// pretty.py's text of each value at the indentation level `depth`:
// the class name, a `name=value,` line for each field one level
// deeper, lists one item to a line, and Python's `repr` of each leaf.
// `asdl::py_pretty` returns it.
void pretty(std::string& out, const Type_program& v, int depth);
void pretty(std::string& out, const Program& v, int depth);
void pretty(std::string& out, const Type_function_definition& v, int depth);
void pretty(std::string& out, const Function& v, int depth);
void pretty(std::string& out, const Type_block& v, int depth);
void pretty(std::string& out, const Block& v, int depth);
void pretty(std::string& out, const Type_block_item& v, int depth);
void pretty(std::string& out, const S& v, int depth);
void pretty(std::string& out, const D& v, int depth);
void pretty(std::string& out, const Type_statement& v, int depth);
void pretty(std::string& out, const Return& v, int depth);
void pretty(std::string& out, const Expression& v, int depth);
void pretty(std::string& out, const IfStmt& v, int depth);
void pretty(std::string& out, const Compound& v, int depth);
void pretty(std::string& out, const BreakStmt& v, int depth);
void pretty(std::string& out, const ContinueStmt& v, int depth);
void pretty(std::string& out, const WhileStmt& v, int depth);
void pretty(std::string& out, const DoWhileStmt& v, int depth);
void pretty(std::string& out, const ForStmt& v, int depth);
void pretty(std::string& out, const SwitchStmt& v, int depth);
void pretty(std::string& out, const CaseStmt& v, int depth);
void pretty(std::string& out, const DefaultStmt& v, int depth);
void pretty(std::string& out, const Goto& v, int depth);
void pretty(std::string& out, const LabeledStmt& v, int depth);
void pretty(std::string& out, const Null& v, int depth);
void pretty(std::string& out, const Type_switch_case& v, int depth);
void pretty(std::string& out, const Type_declaration& v, int depth);
void pretty(std::string& out, const FunctionDecl& v, int depth);
void pretty(std::string& out, const VarDecl& v, int depth);
void pretty(std::string& out, const StructDecl& v, int depth);
void pretty(std::string& out, const Type_var_decl& v, int depth);
void pretty(std::string& out, const Type_function_decl& v, int depth);
void pretty(std::string& out, const Type_struct_decl& v, int depth);
void pretty(std::string& out, const Type_member_decl& v, int depth);
void pretty(std::string& out, const Type_storage_class& v, int depth);
void pretty(std::string& out, const Static& v, int depth);
void pretty(std::string& out, const Extern& v, int depth);
void pretty(std::string& out, const Register& v, int depth);
void pretty(std::string& out, const Type_placement& v, int depth);
void pretty(std::string& out, const ZeroPage& v, int depth);
void pretty(std::string& out, const Absolute& v, int depth);
void pretty(std::string& out, const Type_data_type& v, int depth);
void pretty(std::string& out, const Int& v, int depth);
void pretty(std::string& out, const Long& v, int depth);
void pretty(std::string& out, const LongLong& v, int depth);
void pretty(std::string& out, const UInt& v, int depth);
void pretty(std::string& out, const ULong& v, int depth);
void pretty(std::string& out, const ULongLong& v, int depth);
void pretty(std::string& out, const Float& v, int depth);
void pretty(std::string& out, const Double& v, int depth);
void pretty(std::string& out, const Char& v, int depth);
void pretty(std::string& out, const SChar& v, int depth);
void pretty(std::string& out, const UChar& v, int depth);
void pretty(std::string& out, const Short& v, int depth);
void pretty(std::string& out, const UShort& v, int depth);
void pretty(std::string& out, const Bool& v, int depth);
void pretty(std::string& out, const NullptrT& v, int depth);
void pretty(std::string& out, const BitInt& v, int depth);
void pretty(std::string& out, const Void& v, int depth);
void pretty(std::string& out, const FunType& v, int depth);
void pretty(std::string& out, const Pointer& v, int depth);
void pretty(std::string& out, const Array& v, int depth);
void pretty(std::string& out, const Structure& v, int depth);
void pretty(std::string& out, const Union& v, int depth);
void pretty(std::string& out, const Const& v, int depth);
void pretty(std::string& out, const Volatile& v, int depth);
void pretty(std::string& out, const Type_for_init& v, int depth);
void pretty(std::string& out, const InitDecl& v, int depth);
void pretty(std::string& out, const InitExp& v, int depth);
void pretty(std::string& out, const Type_exp& v, int depth);
void pretty(std::string& out, const Constant& v, int depth);
void pretty(std::string& out, const String& v, int depth);
void pretty(std::string& out, const Var& v, int depth);
void pretty(std::string& out, const Cast& v, int depth);
void pretty(std::string& out, const Unary& v, int depth);
void pretty(std::string& out, const Binary& v, int depth);
void pretty(std::string& out, const Assignment& v, int depth);
void pretty(std::string& out, const CompoundAssignment& v, int depth);
void pretty(std::string& out, const Postfix& v, int depth);
void pretty(std::string& out, const Prefix& v, int depth);
void pretty(std::string& out, const Conditional& v, int depth);
void pretty(std::string& out, const FunctionCall& v, int depth);
void pretty(std::string& out, const IndirectCall& v, int depth);
void pretty(std::string& out, const Dereference& v, int depth);
void pretty(std::string& out, const AddressOf& v, int depth);
void pretty(std::string& out, const Subscript& v, int depth);
void pretty(std::string& out, const InitList& v, int depth);
void pretty(std::string& out, const Designation& v, int depth);
void pretty(std::string& out, const SizeOfExp& v, int depth);
void pretty(std::string& out, const SizeOfType& v, int depth);
void pretty(std::string& out, const Dot& v, int depth);
void pretty(std::string& out, const Arrow& v, int depth);
void pretty(std::string& out, const Comma& v, int depth);
void pretty(std::string& out, const CompoundLiteral& v, int depth);
void pretty(std::string& out, const Type_designator& v, int depth);
void pretty(std::string& out, const IndexDesignator& v, int depth);
void pretty(std::string& out, const MemberDesignator& v, int depth);
void pretty(std::string& out, const Type_unary_operator& v, int depth);
void pretty(std::string& out, const Complement& v, int depth);
void pretty(std::string& out, const Negate& v, int depth);
void pretty(std::string& out, const LogicalNot& v, int depth);
void pretty(std::string& out, const Plus& v, int depth);
void pretty(std::string& out, const Type_incdec_op& v, int depth);
void pretty(std::string& out, const Increment& v, int depth);
void pretty(std::string& out, const Decrement& v, int depth);
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
void pretty(std::string& out, const LogicalAnd& v, int depth);
void pretty(std::string& out, const LogicalOr& v, int depth);
void pretty(std::string& out, const Type_const& v, int depth);
void pretty(std::string& out, const ConstInt& v, int depth);
void pretty(std::string& out, const ConstLong& v, int depth);
void pretty(std::string& out, const ConstLongLong& v, int depth);
void pretty(std::string& out, const ConstUInt& v, int depth);
void pretty(std::string& out, const ConstULong& v, int depth);
void pretty(std::string& out, const ConstULongLong& v, int depth);
void pretty(std::string& out, const ConstFloat& v, int depth);
void pretty(std::string& out, const ConstDouble& v, int depth);
void pretty(std::string& out, const ConstChar& v, int depth);
void pretty(std::string& out, const ConstUChar& v, int depth);
void pretty(std::string& out, const ConstBool& v, int depth);
void pretty(std::string& out, const ConstNullptr& v, int depth);
void pretty(std::string& out, const ConstBitInt& v, int depth);

}  // namespace c6502::c99_ast
