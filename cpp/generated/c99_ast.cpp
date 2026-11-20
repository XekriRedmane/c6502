// Generated from c99.asdl by `asdl.py --cpp`. Do not edit.
#include "c99_ast.h"

#include <utility>

namespace c6502::c99_ast {

void write(::c6502::asdl::Writer& w, const ConstInt& v) {
  w.write_name("ConstInt");
  w.open();
  w.write_int(v.value);
  w.close();
}

static ConstInt read_body_ConstInt(::c6502::asdl::Reader& r) {
  r.open();
  ::c6502::asdl::Int f0 = r.read_int();
  r.close();
  return ConstInt{std::move(f0)};
}

ConstInt read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<ConstInt>) {
  r.expect_name("ConstInt");
  return read_body_ConstInt(r);
}

void write(::c6502::asdl::Writer& w, const ConstLong& v) {
  w.write_name("ConstLong");
  w.open();
  w.write_int(v.value);
  w.close();
}

static ConstLong read_body_ConstLong(::c6502::asdl::Reader& r) {
  r.open();
  ::c6502::asdl::Int f0 = r.read_int();
  r.close();
  return ConstLong{std::move(f0)};
}

ConstLong read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<ConstLong>) {
  r.expect_name("ConstLong");
  return read_body_ConstLong(r);
}

void write(::c6502::asdl::Writer& w, const ConstLongLong& v) {
  w.write_name("ConstLongLong");
  w.open();
  w.write_int(v.value);
  w.close();
}

static ConstLongLong read_body_ConstLongLong(::c6502::asdl::Reader& r) {
  r.open();
  ::c6502::asdl::Int f0 = r.read_int();
  r.close();
  return ConstLongLong{std::move(f0)};
}

ConstLongLong read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<ConstLongLong>) {
  r.expect_name("ConstLongLong");
  return read_body_ConstLongLong(r);
}

void write(::c6502::asdl::Writer& w, const ConstUInt& v) {
  w.write_name("ConstUInt");
  w.open();
  w.write_int(v.value);
  w.close();
}

static ConstUInt read_body_ConstUInt(::c6502::asdl::Reader& r) {
  r.open();
  ::c6502::asdl::Int f0 = r.read_int();
  r.close();
  return ConstUInt{std::move(f0)};
}

ConstUInt read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<ConstUInt>) {
  r.expect_name("ConstUInt");
  return read_body_ConstUInt(r);
}

void write(::c6502::asdl::Writer& w, const ConstULong& v) {
  w.write_name("ConstULong");
  w.open();
  w.write_int(v.value);
  w.close();
}

static ConstULong read_body_ConstULong(::c6502::asdl::Reader& r) {
  r.open();
  ::c6502::asdl::Int f0 = r.read_int();
  r.close();
  return ConstULong{std::move(f0)};
}

ConstULong read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<ConstULong>) {
  r.expect_name("ConstULong");
  return read_body_ConstULong(r);
}

void write(::c6502::asdl::Writer& w, const ConstULongLong& v) {
  w.write_name("ConstULongLong");
  w.open();
  w.write_int(v.value);
  w.close();
}

static ConstULongLong read_body_ConstULongLong(::c6502::asdl::Reader& r) {
  r.open();
  ::c6502::asdl::Int f0 = r.read_int();
  r.close();
  return ConstULongLong{std::move(f0)};
}

ConstULongLong read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<ConstULongLong>) {
  r.expect_name("ConstULongLong");
  return read_body_ConstULongLong(r);
}

void write(::c6502::asdl::Writer& w, const ConstFloat& v) {
  w.write_name("ConstFloat");
  w.open();
  w.write_int(v.bits);
  w.close();
}

static ConstFloat read_body_ConstFloat(::c6502::asdl::Reader& r) {
  r.open();
  ::c6502::asdl::Int f0 = r.read_int();
  r.close();
  return ConstFloat{std::move(f0)};
}

ConstFloat read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<ConstFloat>) {
  r.expect_name("ConstFloat");
  return read_body_ConstFloat(r);
}

void write(::c6502::asdl::Writer& w, const ConstDouble& v) {
  w.write_name("ConstDouble");
  w.open();
  w.write_int(v.bits);
  w.close();
}

static ConstDouble read_body_ConstDouble(::c6502::asdl::Reader& r) {
  r.open();
  ::c6502::asdl::Int f0 = r.read_int();
  r.close();
  return ConstDouble{std::move(f0)};
}

ConstDouble read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<ConstDouble>) {
  r.expect_name("ConstDouble");
  return read_body_ConstDouble(r);
}

void write(::c6502::asdl::Writer& w, const ConstChar& v) {
  w.write_name("ConstChar");
  w.open();
  w.write_int(v.value);
  w.close();
}

static ConstChar read_body_ConstChar(::c6502::asdl::Reader& r) {
  r.open();
  ::c6502::asdl::Int f0 = r.read_int();
  r.close();
  return ConstChar{std::move(f0)};
}

ConstChar read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<ConstChar>) {
  r.expect_name("ConstChar");
  return read_body_ConstChar(r);
}

void write(::c6502::asdl::Writer& w, const ConstUChar& v) {
  w.write_name("ConstUChar");
  w.open();
  w.write_int(v.value);
  w.close();
}

static ConstUChar read_body_ConstUChar(::c6502::asdl::Reader& r) {
  r.open();
  ::c6502::asdl::Int f0 = r.read_int();
  r.close();
  return ConstUChar{std::move(f0)};
}

ConstUChar read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<ConstUChar>) {
  r.expect_name("ConstUChar");
  return read_body_ConstUChar(r);
}

void write(::c6502::asdl::Writer& w, const ConstBool& v) {
  w.write_name("ConstBool");
  w.open();
  w.write_int(v.value);
  w.close();
}

static ConstBool read_body_ConstBool(::c6502::asdl::Reader& r) {
  r.open();
  ::c6502::asdl::Int f0 = r.read_int();
  r.close();
  return ConstBool{std::move(f0)};
}

ConstBool read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<ConstBool>) {
  r.expect_name("ConstBool");
  return read_body_ConstBool(r);
}

void write(::c6502::asdl::Writer& w, const ConstNullptr&) {
  w.write_name("ConstNullptr");
}

static ConstNullptr read_body_ConstNullptr(::c6502::asdl::Reader& r) {
  (void)r;
  return ConstNullptr{};
}

ConstNullptr read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<ConstNullptr>) {
  r.expect_name("ConstNullptr");
  return read_body_ConstNullptr(r);
}

void write(::c6502::asdl::Writer& w, const ConstBitInt& v) {
  w.write_name("ConstBitInt");
  w.open();
  w.write_int(v.value);
  w.space();
  w.write_int(v.width);
  w.space();
  w.write_bool(v.is_signed);
  w.close();
}

static ConstBitInt read_body_ConstBitInt(::c6502::asdl::Reader& r) {
  r.open();
  ::c6502::asdl::Int f0 = r.read_int();
  ::c6502::asdl::Int f1 = r.read_int();
  bool f2 = r.read_bool();
  r.close();
  return ConstBitInt{std::move(f0), std::move(f1), std::move(f2)};
}

ConstBitInt read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<ConstBitInt>) {
  r.expect_name("ConstBitInt");
  return read_body_ConstBitInt(r);
}

void write(::c6502::asdl::Writer& w, const Type_const& v) {
  switch (v.index()) {
    case 0:
      write(w, *std::get_if<0>(&v));
      return;
    case 1:
      write(w, *std::get_if<1>(&v));
      return;
    case 2:
      write(w, *std::get_if<2>(&v));
      return;
    case 3:
      write(w, *std::get_if<3>(&v));
      return;
    case 4:
      write(w, *std::get_if<4>(&v));
      return;
    case 5:
      write(w, *std::get_if<5>(&v));
      return;
    case 6:
      write(w, *std::get_if<6>(&v));
      return;
    case 7:
      write(w, *std::get_if<7>(&v));
      return;
    case 8:
      write(w, *std::get_if<8>(&v));
      return;
    case 9:
      write(w, *std::get_if<9>(&v));
      return;
    case 10:
      write(w, *std::get_if<10>(&v));
      return;
    case 11:
      write(w, *std::get_if<11>(&v));
      return;
    case 12:
      write(w, *std::get_if<12>(&v));
      return;
  }
}

Type_const read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Type_const>) {
  const std::string_view name = r.read_name();
  switch (name.size()) {
    case 8:
      if (name == "ConstInt") {
        return read_body_ConstInt(r);
      }
      break;
    case 9:
      if (name == "ConstLong") {
        return read_body_ConstLong(r);
      }
      if (name == "ConstUInt") {
        return read_body_ConstUInt(r);
      }
      if (name == "ConstChar") {
        return read_body_ConstChar(r);
      }
      if (name == "ConstBool") {
        return read_body_ConstBool(r);
      }
      break;
    case 10:
      if (name == "ConstULong") {
        return read_body_ConstULong(r);
      }
      if (name == "ConstFloat") {
        return read_body_ConstFloat(r);
      }
      if (name == "ConstUChar") {
        return read_body_ConstUChar(r);
      }
      break;
    case 11:
      if (name == "ConstDouble") {
        return read_body_ConstDouble(r);
      }
      if (name == "ConstBitInt") {
        return read_body_ConstBitInt(r);
      }
      break;
    case 12:
      if (name == "ConstNullptr") {
        return read_body_ConstNullptr(r);
      }
      break;
    case 13:
      if (name == "ConstLongLong") {
        return read_body_ConstLongLong(r);
      }
      break;
    case 14:
      if (name == "ConstULongLong") {
        return read_body_ConstULongLong(r);
      }
      break;
  }
  r.unknown_constructor(name, "const");
}

void write(::c6502::asdl::Writer& w, const Int&) {
  w.write_name("Int");
}

static Int read_body_Int(::c6502::asdl::Reader& r) {
  (void)r;
  return Int{};
}

Int read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Int>) {
  r.expect_name("Int");
  return read_body_Int(r);
}

void write(::c6502::asdl::Writer& w, const Long&) {
  w.write_name("Long");
}

static Long read_body_Long(::c6502::asdl::Reader& r) {
  (void)r;
  return Long{};
}

Long read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Long>) {
  r.expect_name("Long");
  return read_body_Long(r);
}

void write(::c6502::asdl::Writer& w, const LongLong&) {
  w.write_name("LongLong");
}

static LongLong read_body_LongLong(::c6502::asdl::Reader& r) {
  (void)r;
  return LongLong{};
}

LongLong read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<LongLong>) {
  r.expect_name("LongLong");
  return read_body_LongLong(r);
}

void write(::c6502::asdl::Writer& w, const UInt&) {
  w.write_name("UInt");
}

static UInt read_body_UInt(::c6502::asdl::Reader& r) {
  (void)r;
  return UInt{};
}

UInt read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<UInt>) {
  r.expect_name("UInt");
  return read_body_UInt(r);
}

void write(::c6502::asdl::Writer& w, const ULong&) {
  w.write_name("ULong");
}

static ULong read_body_ULong(::c6502::asdl::Reader& r) {
  (void)r;
  return ULong{};
}

ULong read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<ULong>) {
  r.expect_name("ULong");
  return read_body_ULong(r);
}

void write(::c6502::asdl::Writer& w, const ULongLong&) {
  w.write_name("ULongLong");
}

static ULongLong read_body_ULongLong(::c6502::asdl::Reader& r) {
  (void)r;
  return ULongLong{};
}

ULongLong read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<ULongLong>) {
  r.expect_name("ULongLong");
  return read_body_ULongLong(r);
}

void write(::c6502::asdl::Writer& w, const Float&) {
  w.write_name("Float");
}

static Float read_body_Float(::c6502::asdl::Reader& r) {
  (void)r;
  return Float{};
}

Float read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Float>) {
  r.expect_name("Float");
  return read_body_Float(r);
}

void write(::c6502::asdl::Writer& w, const Double&) {
  w.write_name("Double");
}

static Double read_body_Double(::c6502::asdl::Reader& r) {
  (void)r;
  return Double{};
}

Double read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Double>) {
  r.expect_name("Double");
  return read_body_Double(r);
}

void write(::c6502::asdl::Writer& w, const Char&) {
  w.write_name("Char");
}

static Char read_body_Char(::c6502::asdl::Reader& r) {
  (void)r;
  return Char{};
}

Char read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Char>) {
  r.expect_name("Char");
  return read_body_Char(r);
}

void write(::c6502::asdl::Writer& w, const SChar&) {
  w.write_name("SChar");
}

static SChar read_body_SChar(::c6502::asdl::Reader& r) {
  (void)r;
  return SChar{};
}

SChar read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<SChar>) {
  r.expect_name("SChar");
  return read_body_SChar(r);
}

void write(::c6502::asdl::Writer& w, const UChar&) {
  w.write_name("UChar");
}

static UChar read_body_UChar(::c6502::asdl::Reader& r) {
  (void)r;
  return UChar{};
}

UChar read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<UChar>) {
  r.expect_name("UChar");
  return read_body_UChar(r);
}

void write(::c6502::asdl::Writer& w, const Short&) {
  w.write_name("Short");
}

static Short read_body_Short(::c6502::asdl::Reader& r) {
  (void)r;
  return Short{};
}

Short read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Short>) {
  r.expect_name("Short");
  return read_body_Short(r);
}

void write(::c6502::asdl::Writer& w, const UShort&) {
  w.write_name("UShort");
}

static UShort read_body_UShort(::c6502::asdl::Reader& r) {
  (void)r;
  return UShort{};
}

UShort read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<UShort>) {
  r.expect_name("UShort");
  return read_body_UShort(r);
}

void write(::c6502::asdl::Writer& w, const Bool&) {
  w.write_name("Bool");
}

static Bool read_body_Bool(::c6502::asdl::Reader& r) {
  (void)r;
  return Bool{};
}

Bool read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Bool>) {
  r.expect_name("Bool");
  return read_body_Bool(r);
}

void write(::c6502::asdl::Writer& w, const NullptrT&) {
  w.write_name("NullptrT");
}

static NullptrT read_body_NullptrT(::c6502::asdl::Reader& r) {
  (void)r;
  return NullptrT{};
}

NullptrT read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<NullptrT>) {
  r.expect_name("NullptrT");
  return read_body_NullptrT(r);
}

void write(::c6502::asdl::Writer& w, const BitInt& v) {
  w.write_name("BitInt");
  w.open();
  w.write_int(v.width);
  w.space();
  w.write_bool(v.is_signed);
  w.close();
}

static BitInt read_body_BitInt(::c6502::asdl::Reader& r) {
  r.open();
  ::c6502::asdl::Int f0 = r.read_int();
  bool f1 = r.read_bool();
  r.close();
  return BitInt{std::move(f0), std::move(f1)};
}

BitInt read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<BitInt>) {
  r.expect_name("BitInt");
  return read_body_BitInt(r);
}

void write(::c6502::asdl::Writer& w, const Void&) {
  w.write_name("Void");
}

static Void read_body_Void(::c6502::asdl::Reader& r) {
  (void)r;
  return Void{};
}

Void read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Void>) {
  r.expect_name("Void");
  return read_body_Void(r);
}

void write(::c6502::asdl::Writer& w, const FunType& v) {
  w.write_name("FunType");
  w.open();
  w.begin_seq(false);
  for (const auto& e : v.params) {
    w.seq_item(false);
    write(w, e);
  }
  w.end_seq(false, v.params.empty());
  w.space();
  write(w, *v.ret);
  w.close();
}

static FunType read_body_FunType(::c6502::asdl::Reader& r) {
  r.open();
  std::vector<Type_data_type> f0;
  r.begin_seq();
  while (!r.end_seq()) {
    f0.push_back(read(r, ::c6502::asdl::Tag<Type_data_type>{}));
  }
  ::c6502::asdl::Box<Type_data_type> f1 = ::c6502::asdl::Box<Type_data_type>(read(r, ::c6502::asdl::Tag<Type_data_type>{}));
  r.close();
  return FunType{std::move(f0), std::move(f1)};
}

FunType read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<FunType>) {
  r.expect_name("FunType");
  return read_body_FunType(r);
}

void write(::c6502::asdl::Writer& w, const Pointer& v) {
  w.write_name("Pointer");
  w.open();
  write(w, *v.referenced_type);
  w.close();
}

static Pointer read_body_Pointer(::c6502::asdl::Reader& r) {
  r.open();
  ::c6502::asdl::Box<Type_data_type> f0 = ::c6502::asdl::Box<Type_data_type>(read(r, ::c6502::asdl::Tag<Type_data_type>{}));
  r.close();
  return Pointer{std::move(f0)};
}

Pointer read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Pointer>) {
  r.expect_name("Pointer");
  return read_body_Pointer(r);
}

void write(::c6502::asdl::Writer& w, const Array& v) {
  w.write_name("Array");
  w.open();
  write(w, *v.element_type);
  w.space();
  w.write_int(v.size);
  w.close();
}

static Array read_body_Array(::c6502::asdl::Reader& r) {
  r.open();
  ::c6502::asdl::Box<Type_data_type> f0 = ::c6502::asdl::Box<Type_data_type>(read(r, ::c6502::asdl::Tag<Type_data_type>{}));
  ::c6502::asdl::Int f1 = r.read_int();
  r.close();
  return Array{std::move(f0), std::move(f1)};
}

Array read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Array>) {
  r.expect_name("Array");
  return read_body_Array(r);
}

void write(::c6502::asdl::Writer& w, const Structure& v) {
  w.write_name("Structure");
  w.open();
  w.write_string(v.tag);
  w.close();
}

static Structure read_body_Structure(::c6502::asdl::Reader& r) {
  r.open();
  std::string f0 = r.read_string();
  r.close();
  return Structure{std::move(f0)};
}

Structure read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Structure>) {
  r.expect_name("Structure");
  return read_body_Structure(r);
}

void write(::c6502::asdl::Writer& w, const Union& v) {
  w.write_name("Union");
  w.open();
  w.write_string(v.tag);
  w.close();
}

static Union read_body_Union(::c6502::asdl::Reader& r) {
  r.open();
  std::string f0 = r.read_string();
  r.close();
  return Union{std::move(f0)};
}

Union read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Union>) {
  r.expect_name("Union");
  return read_body_Union(r);
}

void write(::c6502::asdl::Writer& w, const Const& v) {
  w.write_name("Const");
  w.open();
  write(w, *v.referenced_type);
  w.close();
}

static Const read_body_Const(::c6502::asdl::Reader& r) {
  r.open();
  ::c6502::asdl::Box<Type_data_type> f0 = ::c6502::asdl::Box<Type_data_type>(read(r, ::c6502::asdl::Tag<Type_data_type>{}));
  r.close();
  return Const{std::move(f0)};
}

Const read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Const>) {
  r.expect_name("Const");
  return read_body_Const(r);
}

void write(::c6502::asdl::Writer& w, const Volatile& v) {
  w.write_name("Volatile");
  w.open();
  write(w, *v.referenced_type);
  w.close();
}

static Volatile read_body_Volatile(::c6502::asdl::Reader& r) {
  r.open();
  ::c6502::asdl::Box<Type_data_type> f0 = ::c6502::asdl::Box<Type_data_type>(read(r, ::c6502::asdl::Tag<Type_data_type>{}));
  r.close();
  return Volatile{std::move(f0)};
}

Volatile read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Volatile>) {
  r.expect_name("Volatile");
  return read_body_Volatile(r);
}

void write(::c6502::asdl::Writer& w, const Type_data_type& v) {
  switch (v.index()) {
    case 0:
      write(w, *std::get_if<0>(&v));
      return;
    case 1:
      write(w, *std::get_if<1>(&v));
      return;
    case 2:
      write(w, *std::get_if<2>(&v));
      return;
    case 3:
      write(w, *std::get_if<3>(&v));
      return;
    case 4:
      write(w, *std::get_if<4>(&v));
      return;
    case 5:
      write(w, *std::get_if<5>(&v));
      return;
    case 6:
      write(w, *std::get_if<6>(&v));
      return;
    case 7:
      write(w, *std::get_if<7>(&v));
      return;
    case 8:
      write(w, *std::get_if<8>(&v));
      return;
    case 9:
      write(w, *std::get_if<9>(&v));
      return;
    case 10:
      write(w, *std::get_if<10>(&v));
      return;
    case 11:
      write(w, *std::get_if<11>(&v));
      return;
    case 12:
      write(w, *std::get_if<12>(&v));
      return;
    case 13:
      write(w, *std::get_if<13>(&v));
      return;
    case 14:
      write(w, *std::get_if<14>(&v));
      return;
    case 15:
      write(w, *std::get_if<15>(&v));
      return;
    case 16:
      write(w, *std::get_if<16>(&v));
      return;
    case 17:
      write(w, *std::get_if<17>(&v));
      return;
    case 18:
      write(w, *std::get_if<18>(&v));
      return;
    case 19:
      write(w, *std::get_if<19>(&v));
      return;
    case 20:
      write(w, *std::get_if<20>(&v));
      return;
    case 21:
      write(w, *std::get_if<21>(&v));
      return;
    case 22:
      write(w, *std::get_if<22>(&v));
      return;
    case 23:
      write(w, *std::get_if<23>(&v));
      return;
  }
}

Type_data_type read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Type_data_type>) {
  const std::string_view name = r.read_name();
  switch (name.size()) {
    case 3:
      if (name == "Int") {
        return read_body_Int(r);
      }
      break;
    case 4:
      if (name == "Long") {
        return read_body_Long(r);
      }
      if (name == "UInt") {
        return read_body_UInt(r);
      }
      if (name == "Char") {
        return read_body_Char(r);
      }
      if (name == "Bool") {
        return read_body_Bool(r);
      }
      if (name == "Void") {
        return read_body_Void(r);
      }
      break;
    case 5:
      if (name == "ULong") {
        return read_body_ULong(r);
      }
      if (name == "Float") {
        return read_body_Float(r);
      }
      if (name == "SChar") {
        return read_body_SChar(r);
      }
      if (name == "UChar") {
        return read_body_UChar(r);
      }
      if (name == "Short") {
        return read_body_Short(r);
      }
      if (name == "Array") {
        return read_body_Array(r);
      }
      if (name == "Union") {
        return read_body_Union(r);
      }
      if (name == "Const") {
        return read_body_Const(r);
      }
      break;
    case 6:
      if (name == "Double") {
        return read_body_Double(r);
      }
      if (name == "UShort") {
        return read_body_UShort(r);
      }
      if (name == "BitInt") {
        return read_body_BitInt(r);
      }
      break;
    case 7:
      if (name == "FunType") {
        return read_body_FunType(r);
      }
      if (name == "Pointer") {
        return read_body_Pointer(r);
      }
      break;
    case 8:
      if (name == "LongLong") {
        return read_body_LongLong(r);
      }
      if (name == "NullptrT") {
        return read_body_NullptrT(r);
      }
      if (name == "Volatile") {
        return read_body_Volatile(r);
      }
      break;
    case 9:
      if (name == "ULongLong") {
        return read_body_ULongLong(r);
      }
      if (name == "Structure") {
        return read_body_Structure(r);
      }
      break;
  }
  r.unknown_constructor(name, "data_type");
}

void write(::c6502::asdl::Writer& w, const Complement&) {
  w.write_name("Complement");
}

static Complement read_body_Complement(::c6502::asdl::Reader& r) {
  (void)r;
  return Complement{};
}

Complement read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Complement>) {
  r.expect_name("Complement");
  return read_body_Complement(r);
}

void write(::c6502::asdl::Writer& w, const Negate&) {
  w.write_name("Negate");
}

static Negate read_body_Negate(::c6502::asdl::Reader& r) {
  (void)r;
  return Negate{};
}

Negate read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Negate>) {
  r.expect_name("Negate");
  return read_body_Negate(r);
}

void write(::c6502::asdl::Writer& w, const LogicalNot&) {
  w.write_name("LogicalNot");
}

static LogicalNot read_body_LogicalNot(::c6502::asdl::Reader& r) {
  (void)r;
  return LogicalNot{};
}

LogicalNot read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<LogicalNot>) {
  r.expect_name("LogicalNot");
  return read_body_LogicalNot(r);
}

void write(::c6502::asdl::Writer& w, const Plus&) {
  w.write_name("Plus");
}

static Plus read_body_Plus(::c6502::asdl::Reader& r) {
  (void)r;
  return Plus{};
}

Plus read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Plus>) {
  r.expect_name("Plus");
  return read_body_Plus(r);
}

void write(::c6502::asdl::Writer& w, const Type_unary_operator& v) {
  switch (v.index()) {
    case 0:
      write(w, *std::get_if<0>(&v));
      return;
    case 1:
      write(w, *std::get_if<1>(&v));
      return;
    case 2:
      write(w, *std::get_if<2>(&v));
      return;
    case 3:
      write(w, *std::get_if<3>(&v));
      return;
  }
}

Type_unary_operator read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Type_unary_operator>) {
  const std::string_view name = r.read_name();
  switch (name.size()) {
    case 4:
      if (name == "Plus") {
        return read_body_Plus(r);
      }
      break;
    case 6:
      if (name == "Negate") {
        return read_body_Negate(r);
      }
      break;
    case 10:
      if (name == "Complement") {
        return read_body_Complement(r);
      }
      if (name == "LogicalNot") {
        return read_body_LogicalNot(r);
      }
      break;
  }
  r.unknown_constructor(name, "unary_operator");
}

void write(::c6502::asdl::Writer& w, const Add&) {
  w.write_name("Add");
}

static Add read_body_Add(::c6502::asdl::Reader& r) {
  (void)r;
  return Add{};
}

Add read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Add>) {
  r.expect_name("Add");
  return read_body_Add(r);
}

void write(::c6502::asdl::Writer& w, const Subtract&) {
  w.write_name("Subtract");
}

static Subtract read_body_Subtract(::c6502::asdl::Reader& r) {
  (void)r;
  return Subtract{};
}

Subtract read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Subtract>) {
  r.expect_name("Subtract");
  return read_body_Subtract(r);
}

void write(::c6502::asdl::Writer& w, const Multiply&) {
  w.write_name("Multiply");
}

static Multiply read_body_Multiply(::c6502::asdl::Reader& r) {
  (void)r;
  return Multiply{};
}

Multiply read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Multiply>) {
  r.expect_name("Multiply");
  return read_body_Multiply(r);
}

void write(::c6502::asdl::Writer& w, const Divide&) {
  w.write_name("Divide");
}

static Divide read_body_Divide(::c6502::asdl::Reader& r) {
  (void)r;
  return Divide{};
}

Divide read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Divide>) {
  r.expect_name("Divide");
  return read_body_Divide(r);
}

void write(::c6502::asdl::Writer& w, const Modulo&) {
  w.write_name("Modulo");
}

static Modulo read_body_Modulo(::c6502::asdl::Reader& r) {
  (void)r;
  return Modulo{};
}

Modulo read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Modulo>) {
  r.expect_name("Modulo");
  return read_body_Modulo(r);
}

void write(::c6502::asdl::Writer& w, const BitwiseAnd&) {
  w.write_name("BitwiseAnd");
}

static BitwiseAnd read_body_BitwiseAnd(::c6502::asdl::Reader& r) {
  (void)r;
  return BitwiseAnd{};
}

BitwiseAnd read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<BitwiseAnd>) {
  r.expect_name("BitwiseAnd");
  return read_body_BitwiseAnd(r);
}

void write(::c6502::asdl::Writer& w, const BitwiseOr&) {
  w.write_name("BitwiseOr");
}

static BitwiseOr read_body_BitwiseOr(::c6502::asdl::Reader& r) {
  (void)r;
  return BitwiseOr{};
}

BitwiseOr read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<BitwiseOr>) {
  r.expect_name("BitwiseOr");
  return read_body_BitwiseOr(r);
}

void write(::c6502::asdl::Writer& w, const BitwiseXor&) {
  w.write_name("BitwiseXor");
}

static BitwiseXor read_body_BitwiseXor(::c6502::asdl::Reader& r) {
  (void)r;
  return BitwiseXor{};
}

BitwiseXor read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<BitwiseXor>) {
  r.expect_name("BitwiseXor");
  return read_body_BitwiseXor(r);
}

void write(::c6502::asdl::Writer& w, const LeftShift&) {
  w.write_name("LeftShift");
}

static LeftShift read_body_LeftShift(::c6502::asdl::Reader& r) {
  (void)r;
  return LeftShift{};
}

LeftShift read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<LeftShift>) {
  r.expect_name("LeftShift");
  return read_body_LeftShift(r);
}

void write(::c6502::asdl::Writer& w, const RightShift&) {
  w.write_name("RightShift");
}

static RightShift read_body_RightShift(::c6502::asdl::Reader& r) {
  (void)r;
  return RightShift{};
}

RightShift read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<RightShift>) {
  r.expect_name("RightShift");
  return read_body_RightShift(r);
}

void write(::c6502::asdl::Writer& w, const Equal&) {
  w.write_name("Equal");
}

static Equal read_body_Equal(::c6502::asdl::Reader& r) {
  (void)r;
  return Equal{};
}

Equal read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Equal>) {
  r.expect_name("Equal");
  return read_body_Equal(r);
}

void write(::c6502::asdl::Writer& w, const NotEqual&) {
  w.write_name("NotEqual");
}

static NotEqual read_body_NotEqual(::c6502::asdl::Reader& r) {
  (void)r;
  return NotEqual{};
}

NotEqual read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<NotEqual>) {
  r.expect_name("NotEqual");
  return read_body_NotEqual(r);
}

void write(::c6502::asdl::Writer& w, const LessThan&) {
  w.write_name("LessThan");
}

static LessThan read_body_LessThan(::c6502::asdl::Reader& r) {
  (void)r;
  return LessThan{};
}

LessThan read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<LessThan>) {
  r.expect_name("LessThan");
  return read_body_LessThan(r);
}

void write(::c6502::asdl::Writer& w, const GreaterThan&) {
  w.write_name("GreaterThan");
}

static GreaterThan read_body_GreaterThan(::c6502::asdl::Reader& r) {
  (void)r;
  return GreaterThan{};
}

GreaterThan read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<GreaterThan>) {
  r.expect_name("GreaterThan");
  return read_body_GreaterThan(r);
}

void write(::c6502::asdl::Writer& w, const LessOrEqual&) {
  w.write_name("LessOrEqual");
}

static LessOrEqual read_body_LessOrEqual(::c6502::asdl::Reader& r) {
  (void)r;
  return LessOrEqual{};
}

LessOrEqual read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<LessOrEqual>) {
  r.expect_name("LessOrEqual");
  return read_body_LessOrEqual(r);
}

void write(::c6502::asdl::Writer& w, const GreaterOrEqual&) {
  w.write_name("GreaterOrEqual");
}

static GreaterOrEqual read_body_GreaterOrEqual(::c6502::asdl::Reader& r) {
  (void)r;
  return GreaterOrEqual{};
}

GreaterOrEqual read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<GreaterOrEqual>) {
  r.expect_name("GreaterOrEqual");
  return read_body_GreaterOrEqual(r);
}

void write(::c6502::asdl::Writer& w, const LogicalAnd&) {
  w.write_name("LogicalAnd");
}

static LogicalAnd read_body_LogicalAnd(::c6502::asdl::Reader& r) {
  (void)r;
  return LogicalAnd{};
}

LogicalAnd read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<LogicalAnd>) {
  r.expect_name("LogicalAnd");
  return read_body_LogicalAnd(r);
}

void write(::c6502::asdl::Writer& w, const LogicalOr&) {
  w.write_name("LogicalOr");
}

static LogicalOr read_body_LogicalOr(::c6502::asdl::Reader& r) {
  (void)r;
  return LogicalOr{};
}

LogicalOr read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<LogicalOr>) {
  r.expect_name("LogicalOr");
  return read_body_LogicalOr(r);
}

void write(::c6502::asdl::Writer& w, const Type_binary_operator& v) {
  switch (v.index()) {
    case 0:
      write(w, *std::get_if<0>(&v));
      return;
    case 1:
      write(w, *std::get_if<1>(&v));
      return;
    case 2:
      write(w, *std::get_if<2>(&v));
      return;
    case 3:
      write(w, *std::get_if<3>(&v));
      return;
    case 4:
      write(w, *std::get_if<4>(&v));
      return;
    case 5:
      write(w, *std::get_if<5>(&v));
      return;
    case 6:
      write(w, *std::get_if<6>(&v));
      return;
    case 7:
      write(w, *std::get_if<7>(&v));
      return;
    case 8:
      write(w, *std::get_if<8>(&v));
      return;
    case 9:
      write(w, *std::get_if<9>(&v));
      return;
    case 10:
      write(w, *std::get_if<10>(&v));
      return;
    case 11:
      write(w, *std::get_if<11>(&v));
      return;
    case 12:
      write(w, *std::get_if<12>(&v));
      return;
    case 13:
      write(w, *std::get_if<13>(&v));
      return;
    case 14:
      write(w, *std::get_if<14>(&v));
      return;
    case 15:
      write(w, *std::get_if<15>(&v));
      return;
    case 16:
      write(w, *std::get_if<16>(&v));
      return;
    case 17:
      write(w, *std::get_if<17>(&v));
      return;
  }
}

Type_binary_operator read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Type_binary_operator>) {
  const std::string_view name = r.read_name();
  switch (name.size()) {
    case 3:
      if (name == "Add") {
        return read_body_Add(r);
      }
      break;
    case 5:
      if (name == "Equal") {
        return read_body_Equal(r);
      }
      break;
    case 6:
      if (name == "Divide") {
        return read_body_Divide(r);
      }
      if (name == "Modulo") {
        return read_body_Modulo(r);
      }
      break;
    case 8:
      if (name == "Subtract") {
        return read_body_Subtract(r);
      }
      if (name == "Multiply") {
        return read_body_Multiply(r);
      }
      if (name == "NotEqual") {
        return read_body_NotEqual(r);
      }
      if (name == "LessThan") {
        return read_body_LessThan(r);
      }
      break;
    case 9:
      if (name == "BitwiseOr") {
        return read_body_BitwiseOr(r);
      }
      if (name == "LeftShift") {
        return read_body_LeftShift(r);
      }
      if (name == "LogicalOr") {
        return read_body_LogicalOr(r);
      }
      break;
    case 10:
      if (name == "BitwiseAnd") {
        return read_body_BitwiseAnd(r);
      }
      if (name == "BitwiseXor") {
        return read_body_BitwiseXor(r);
      }
      if (name == "RightShift") {
        return read_body_RightShift(r);
      }
      if (name == "LogicalAnd") {
        return read_body_LogicalAnd(r);
      }
      break;
    case 11:
      if (name == "GreaterThan") {
        return read_body_GreaterThan(r);
      }
      if (name == "LessOrEqual") {
        return read_body_LessOrEqual(r);
      }
      break;
    case 14:
      if (name == "GreaterOrEqual") {
        return read_body_GreaterOrEqual(r);
      }
      break;
  }
  r.unknown_constructor(name, "binary_operator");
}

void write(::c6502::asdl::Writer& w, const Increment&) {
  w.write_name("Increment");
}

static Increment read_body_Increment(::c6502::asdl::Reader& r) {
  (void)r;
  return Increment{};
}

Increment read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Increment>) {
  r.expect_name("Increment");
  return read_body_Increment(r);
}

void write(::c6502::asdl::Writer& w, const Decrement&) {
  w.write_name("Decrement");
}

static Decrement read_body_Decrement(::c6502::asdl::Reader& r) {
  (void)r;
  return Decrement{};
}

Decrement read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Decrement>) {
  r.expect_name("Decrement");
  return read_body_Decrement(r);
}

void write(::c6502::asdl::Writer& w, const Type_incdec_op& v) {
  switch (v.index()) {
    case 0:
      write(w, *std::get_if<0>(&v));
      return;
    case 1:
      write(w, *std::get_if<1>(&v));
      return;
  }
}

Type_incdec_op read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Type_incdec_op>) {
  const std::string_view name = r.read_name();
  switch (name.size()) {
    case 9:
      if (name == "Increment") {
        return read_body_Increment(r);
      }
      if (name == "Decrement") {
        return read_body_Decrement(r);
      }
      break;
  }
  r.unknown_constructor(name, "incdec_op");
}

void write(::c6502::asdl::Writer& w, const IndexDesignator& v) {
  w.write_name("IndexDesignator");
  w.open();
  w.write_int(v.index);
  w.close();
}

static IndexDesignator read_body_IndexDesignator(::c6502::asdl::Reader& r) {
  r.open();
  ::c6502::asdl::Int f0 = r.read_int();
  r.close();
  return IndexDesignator{std::move(f0)};
}

IndexDesignator read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<IndexDesignator>) {
  r.expect_name("IndexDesignator");
  return read_body_IndexDesignator(r);
}

void write(::c6502::asdl::Writer& w, const MemberDesignator& v) {
  w.write_name("MemberDesignator");
  w.open();
  w.write_string(v.member);
  w.close();
}

static MemberDesignator read_body_MemberDesignator(::c6502::asdl::Reader& r) {
  r.open();
  std::string f0 = r.read_string();
  r.close();
  return MemberDesignator{std::move(f0)};
}

MemberDesignator read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<MemberDesignator>) {
  r.expect_name("MemberDesignator");
  return read_body_MemberDesignator(r);
}

void write(::c6502::asdl::Writer& w, const Type_designator& v) {
  switch (v.index()) {
    case 0:
      write(w, *std::get_if<0>(&v));
      return;
    case 1:
      write(w, *std::get_if<1>(&v));
      return;
  }
}

Type_designator read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Type_designator>) {
  const std::string_view name = r.read_name();
  switch (name.size()) {
    case 15:
      if (name == "IndexDesignator") {
        return read_body_IndexDesignator(r);
      }
      break;
    case 16:
      if (name == "MemberDesignator") {
        return read_body_MemberDesignator(r);
      }
      break;
  }
  r.unknown_constructor(name, "designator");
}

void write(::c6502::asdl::Writer& w, const Static&) {
  w.write_name("Static");
}

static Static read_body_Static(::c6502::asdl::Reader& r) {
  (void)r;
  return Static{};
}

Static read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Static>) {
  r.expect_name("Static");
  return read_body_Static(r);
}

void write(::c6502::asdl::Writer& w, const Extern&) {
  w.write_name("Extern");
}

static Extern read_body_Extern(::c6502::asdl::Reader& r) {
  (void)r;
  return Extern{};
}

Extern read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Extern>) {
  r.expect_name("Extern");
  return read_body_Extern(r);
}

void write(::c6502::asdl::Writer& w, const Register&) {
  w.write_name("Register");
}

static Register read_body_Register(::c6502::asdl::Reader& r) {
  (void)r;
  return Register{};
}

Register read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Register>) {
  r.expect_name("Register");
  return read_body_Register(r);
}

void write(::c6502::asdl::Writer& w, const Type_storage_class& v) {
  switch (v.index()) {
    case 0:
      write(w, *std::get_if<0>(&v));
      return;
    case 1:
      write(w, *std::get_if<1>(&v));
      return;
    case 2:
      write(w, *std::get_if<2>(&v));
      return;
  }
}

Type_storage_class read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Type_storage_class>) {
  const std::string_view name = r.read_name();
  switch (name.size()) {
    case 6:
      if (name == "Static") {
        return read_body_Static(r);
      }
      if (name == "Extern") {
        return read_body_Extern(r);
      }
      break;
    case 8:
      if (name == "Register") {
        return read_body_Register(r);
      }
      break;
  }
  r.unknown_constructor(name, "storage_class");
}

void write(::c6502::asdl::Writer& w, const Constant& v) {
  w.write_name("Constant");
  w.open();
  write(w, v.const_);
  w.space();
  if (v.data_type) {
    write(w, (*v.data_type));
  } else {
    w.write_none();
  }
  w.close();
}

static Constant read_body_Constant(::c6502::asdl::Reader& r) {
  r.open();
  Type_const f0 = read(r, ::c6502::asdl::Tag<Type_const>{});
  std::optional<Type_data_type> f1;
  if (!r.read_none()) {
    f1 = read(r, ::c6502::asdl::Tag<Type_data_type>{});
  }
  r.close();
  return Constant{std::move(f0), std::move(f1)};
}

Constant read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Constant>) {
  r.expect_name("Constant");
  return read_body_Constant(r);
}

void write(::c6502::asdl::Writer& w, const String& v) {
  w.write_name("String");
  w.open();
  w.write_string(v.str);
  w.close();
}

static String read_body_String(::c6502::asdl::Reader& r) {
  r.open();
  std::string f0 = r.read_string();
  r.close();
  return String{std::move(f0)};
}

String read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<String>) {
  r.expect_name("String");
  return read_body_String(r);
}

void write(::c6502::asdl::Writer& w, const Var& v) {
  w.write_name("Var");
  w.open();
  w.write_string(v.name);
  w.space();
  if (v.data_type) {
    write(w, (*v.data_type));
  } else {
    w.write_none();
  }
  w.close();
}

static Var read_body_Var(::c6502::asdl::Reader& r) {
  r.open();
  std::string f0 = r.read_string();
  std::optional<Type_data_type> f1;
  if (!r.read_none()) {
    f1 = read(r, ::c6502::asdl::Tag<Type_data_type>{});
  }
  r.close();
  return Var{std::move(f0), std::move(f1)};
}

Var read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Var>) {
  r.expect_name("Var");
  return read_body_Var(r);
}

void write(::c6502::asdl::Writer& w, const Cast& v) {
  w.write_name("Cast");
  w.open();
  write(w, v.target_type);
  w.space();
  write(w, *v.exp);
  w.space();
  if (v.data_type) {
    write(w, (*v.data_type));
  } else {
    w.write_none();
  }
  w.close();
}

static Cast read_body_Cast(::c6502::asdl::Reader& r) {
  r.open();
  Type_data_type f0 = read(r, ::c6502::asdl::Tag<Type_data_type>{});
  ::c6502::asdl::Box<Type_exp> f1 = ::c6502::asdl::Box<Type_exp>(read(r, ::c6502::asdl::Tag<Type_exp>{}));
  std::optional<Type_data_type> f2;
  if (!r.read_none()) {
    f2 = read(r, ::c6502::asdl::Tag<Type_data_type>{});
  }
  r.close();
  return Cast{std::move(f0), std::move(f1), std::move(f2)};
}

Cast read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Cast>) {
  r.expect_name("Cast");
  return read_body_Cast(r);
}

void write(::c6502::asdl::Writer& w, const Unary& v) {
  w.write_name("Unary");
  w.open();
  write(w, v.op);
  w.space();
  write(w, *v.exp);
  w.space();
  if (v.data_type) {
    write(w, (*v.data_type));
  } else {
    w.write_none();
  }
  w.close();
}

static Unary read_body_Unary(::c6502::asdl::Reader& r) {
  r.open();
  Type_unary_operator f0 = read(r, ::c6502::asdl::Tag<Type_unary_operator>{});
  ::c6502::asdl::Box<Type_exp> f1 = ::c6502::asdl::Box<Type_exp>(read(r, ::c6502::asdl::Tag<Type_exp>{}));
  std::optional<Type_data_type> f2;
  if (!r.read_none()) {
    f2 = read(r, ::c6502::asdl::Tag<Type_data_type>{});
  }
  r.close();
  return Unary{std::move(f0), std::move(f1), std::move(f2)};
}

Unary read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Unary>) {
  r.expect_name("Unary");
  return read_body_Unary(r);
}

void write(::c6502::asdl::Writer& w, const Binary& v) {
  w.write_name("Binary");
  w.open();
  write(w, v.op);
  w.space();
  write(w, *v.left);
  w.space();
  write(w, *v.right);
  w.space();
  if (v.data_type) {
    write(w, (*v.data_type));
  } else {
    w.write_none();
  }
  w.close();
}

static Binary read_body_Binary(::c6502::asdl::Reader& r) {
  r.open();
  Type_binary_operator f0 = read(r, ::c6502::asdl::Tag<Type_binary_operator>{});
  ::c6502::asdl::Box<Type_exp> f1 = ::c6502::asdl::Box<Type_exp>(read(r, ::c6502::asdl::Tag<Type_exp>{}));
  ::c6502::asdl::Box<Type_exp> f2 = ::c6502::asdl::Box<Type_exp>(read(r, ::c6502::asdl::Tag<Type_exp>{}));
  std::optional<Type_data_type> f3;
  if (!r.read_none()) {
    f3 = read(r, ::c6502::asdl::Tag<Type_data_type>{});
  }
  r.close();
  return Binary{std::move(f0), std::move(f1), std::move(f2), std::move(f3)};
}

Binary read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Binary>) {
  r.expect_name("Binary");
  return read_body_Binary(r);
}

void write(::c6502::asdl::Writer& w, const Assignment& v) {
  w.write_name("Assignment");
  w.open();
  write(w, *v.lval);
  w.space();
  write(w, *v.rval);
  w.space();
  if (v.data_type) {
    write(w, (*v.data_type));
  } else {
    w.write_none();
  }
  w.close();
}

static Assignment read_body_Assignment(::c6502::asdl::Reader& r) {
  r.open();
  ::c6502::asdl::Box<Type_exp> f0 = ::c6502::asdl::Box<Type_exp>(read(r, ::c6502::asdl::Tag<Type_exp>{}));
  ::c6502::asdl::Box<Type_exp> f1 = ::c6502::asdl::Box<Type_exp>(read(r, ::c6502::asdl::Tag<Type_exp>{}));
  std::optional<Type_data_type> f2;
  if (!r.read_none()) {
    f2 = read(r, ::c6502::asdl::Tag<Type_data_type>{});
  }
  r.close();
  return Assignment{std::move(f0), std::move(f1), std::move(f2)};
}

Assignment read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Assignment>) {
  r.expect_name("Assignment");
  return read_body_Assignment(r);
}

void write(::c6502::asdl::Writer& w, const CompoundAssignment& v) {
  w.write_name("CompoundAssignment");
  w.open();
  write(w, v.op);
  w.space();
  write(w, *v.lval);
  w.space();
  write(w, *v.rval);
  w.space();
  if (v.intermediate_type) {
    write(w, (*v.intermediate_type));
  } else {
    w.write_none();
  }
  w.space();
  if (v.data_type) {
    write(w, (*v.data_type));
  } else {
    w.write_none();
  }
  w.close();
}

static CompoundAssignment read_body_CompoundAssignment(::c6502::asdl::Reader& r) {
  r.open();
  Type_binary_operator f0 = read(r, ::c6502::asdl::Tag<Type_binary_operator>{});
  ::c6502::asdl::Box<Type_exp> f1 = ::c6502::asdl::Box<Type_exp>(read(r, ::c6502::asdl::Tag<Type_exp>{}));
  ::c6502::asdl::Box<Type_exp> f2 = ::c6502::asdl::Box<Type_exp>(read(r, ::c6502::asdl::Tag<Type_exp>{}));
  std::optional<Type_data_type> f3;
  if (!r.read_none()) {
    f3 = read(r, ::c6502::asdl::Tag<Type_data_type>{});
  }
  std::optional<Type_data_type> f4;
  if (!r.read_none()) {
    f4 = read(r, ::c6502::asdl::Tag<Type_data_type>{});
  }
  r.close();
  return CompoundAssignment{std::move(f0), std::move(f1), std::move(f2), std::move(f3), std::move(f4)};
}

CompoundAssignment read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<CompoundAssignment>) {
  r.expect_name("CompoundAssignment");
  return read_body_CompoundAssignment(r);
}

void write(::c6502::asdl::Writer& w, const Postfix& v) {
  w.write_name("Postfix");
  w.open();
  write(w, v.op);
  w.space();
  write(w, *v.operand);
  w.space();
  if (v.data_type) {
    write(w, (*v.data_type));
  } else {
    w.write_none();
  }
  w.close();
}

static Postfix read_body_Postfix(::c6502::asdl::Reader& r) {
  r.open();
  Type_incdec_op f0 = read(r, ::c6502::asdl::Tag<Type_incdec_op>{});
  ::c6502::asdl::Box<Type_exp> f1 = ::c6502::asdl::Box<Type_exp>(read(r, ::c6502::asdl::Tag<Type_exp>{}));
  std::optional<Type_data_type> f2;
  if (!r.read_none()) {
    f2 = read(r, ::c6502::asdl::Tag<Type_data_type>{});
  }
  r.close();
  return Postfix{std::move(f0), std::move(f1), std::move(f2)};
}

Postfix read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Postfix>) {
  r.expect_name("Postfix");
  return read_body_Postfix(r);
}

void write(::c6502::asdl::Writer& w, const Prefix& v) {
  w.write_name("Prefix");
  w.open();
  write(w, v.op);
  w.space();
  write(w, *v.operand);
  w.space();
  if (v.data_type) {
    write(w, (*v.data_type));
  } else {
    w.write_none();
  }
  w.close();
}

static Prefix read_body_Prefix(::c6502::asdl::Reader& r) {
  r.open();
  Type_incdec_op f0 = read(r, ::c6502::asdl::Tag<Type_incdec_op>{});
  ::c6502::asdl::Box<Type_exp> f1 = ::c6502::asdl::Box<Type_exp>(read(r, ::c6502::asdl::Tag<Type_exp>{}));
  std::optional<Type_data_type> f2;
  if (!r.read_none()) {
    f2 = read(r, ::c6502::asdl::Tag<Type_data_type>{});
  }
  r.close();
  return Prefix{std::move(f0), std::move(f1), std::move(f2)};
}

Prefix read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Prefix>) {
  r.expect_name("Prefix");
  return read_body_Prefix(r);
}

void write(::c6502::asdl::Writer& w, const Conditional& v) {
  w.write_name("Conditional");
  w.open();
  write(w, *v.condition);
  w.space();
  write(w, *v.true_clause);
  w.space();
  write(w, *v.false_clause);
  w.space();
  if (v.data_type) {
    write(w, (*v.data_type));
  } else {
    w.write_none();
  }
  w.close();
}

static Conditional read_body_Conditional(::c6502::asdl::Reader& r) {
  r.open();
  ::c6502::asdl::Box<Type_exp> f0 = ::c6502::asdl::Box<Type_exp>(read(r, ::c6502::asdl::Tag<Type_exp>{}));
  ::c6502::asdl::Box<Type_exp> f1 = ::c6502::asdl::Box<Type_exp>(read(r, ::c6502::asdl::Tag<Type_exp>{}));
  ::c6502::asdl::Box<Type_exp> f2 = ::c6502::asdl::Box<Type_exp>(read(r, ::c6502::asdl::Tag<Type_exp>{}));
  std::optional<Type_data_type> f3;
  if (!r.read_none()) {
    f3 = read(r, ::c6502::asdl::Tag<Type_data_type>{});
  }
  r.close();
  return Conditional{std::move(f0), std::move(f1), std::move(f2), std::move(f3)};
}

Conditional read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Conditional>) {
  r.expect_name("Conditional");
  return read_body_Conditional(r);
}

void write(::c6502::asdl::Writer& w, const FunctionCall& v) {
  w.write_name("FunctionCall");
  w.open();
  w.write_string(v.name);
  w.space();
  w.begin_seq(false);
  for (const auto& e : v.args) {
    w.seq_item(false);
    write(w, e);
  }
  w.end_seq(false, v.args.empty());
  w.space();
  if (v.data_type) {
    write(w, (*v.data_type));
  } else {
    w.write_none();
  }
  w.space();
  w.write_bool(v.hw_stack_save);
  w.close();
}

static FunctionCall read_body_FunctionCall(::c6502::asdl::Reader& r) {
  r.open();
  std::string f0 = r.read_string();
  std::vector<Type_exp> f1;
  r.begin_seq();
  while (!r.end_seq()) {
    f1.push_back(read(r, ::c6502::asdl::Tag<Type_exp>{}));
  }
  std::optional<Type_data_type> f2;
  if (!r.read_none()) {
    f2 = read(r, ::c6502::asdl::Tag<Type_data_type>{});
  }
  bool f3 = r.read_bool();
  r.close();
  return FunctionCall{std::move(f0), std::move(f1), std::move(f2), std::move(f3)};
}

FunctionCall read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<FunctionCall>) {
  r.expect_name("FunctionCall");
  return read_body_FunctionCall(r);
}

void write(::c6502::asdl::Writer& w, const IndirectCall& v) {
  w.write_name("IndirectCall");
  w.open();
  write(w, *v.callee);
  w.space();
  w.begin_seq(false);
  for (const auto& e : v.args) {
    w.seq_item(false);
    write(w, e);
  }
  w.end_seq(false, v.args.empty());
  w.space();
  if (v.data_type) {
    write(w, (*v.data_type));
  } else {
    w.write_none();
  }
  w.space();
  w.write_bool(v.hw_stack_save);
  w.close();
}

static IndirectCall read_body_IndirectCall(::c6502::asdl::Reader& r) {
  r.open();
  ::c6502::asdl::Box<Type_exp> f0 = ::c6502::asdl::Box<Type_exp>(read(r, ::c6502::asdl::Tag<Type_exp>{}));
  std::vector<Type_exp> f1;
  r.begin_seq();
  while (!r.end_seq()) {
    f1.push_back(read(r, ::c6502::asdl::Tag<Type_exp>{}));
  }
  std::optional<Type_data_type> f2;
  if (!r.read_none()) {
    f2 = read(r, ::c6502::asdl::Tag<Type_data_type>{});
  }
  bool f3 = r.read_bool();
  r.close();
  return IndirectCall{std::move(f0), std::move(f1), std::move(f2), std::move(f3)};
}

IndirectCall read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<IndirectCall>) {
  r.expect_name("IndirectCall");
  return read_body_IndirectCall(r);
}

void write(::c6502::asdl::Writer& w, const Dereference& v) {
  w.write_name("Dereference");
  w.open();
  write(w, *v.exp);
  w.space();
  if (v.data_type) {
    write(w, (*v.data_type));
  } else {
    w.write_none();
  }
  w.close();
}

static Dereference read_body_Dereference(::c6502::asdl::Reader& r) {
  r.open();
  ::c6502::asdl::Box<Type_exp> f0 = ::c6502::asdl::Box<Type_exp>(read(r, ::c6502::asdl::Tag<Type_exp>{}));
  std::optional<Type_data_type> f1;
  if (!r.read_none()) {
    f1 = read(r, ::c6502::asdl::Tag<Type_data_type>{});
  }
  r.close();
  return Dereference{std::move(f0), std::move(f1)};
}

Dereference read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Dereference>) {
  r.expect_name("Dereference");
  return read_body_Dereference(r);
}

void write(::c6502::asdl::Writer& w, const AddressOf& v) {
  w.write_name("AddressOf");
  w.open();
  write(w, *v.exp);
  w.space();
  if (v.data_type) {
    write(w, (*v.data_type));
  } else {
    w.write_none();
  }
  w.close();
}

static AddressOf read_body_AddressOf(::c6502::asdl::Reader& r) {
  r.open();
  ::c6502::asdl::Box<Type_exp> f0 = ::c6502::asdl::Box<Type_exp>(read(r, ::c6502::asdl::Tag<Type_exp>{}));
  std::optional<Type_data_type> f1;
  if (!r.read_none()) {
    f1 = read(r, ::c6502::asdl::Tag<Type_data_type>{});
  }
  r.close();
  return AddressOf{std::move(f0), std::move(f1)};
}

AddressOf read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<AddressOf>) {
  r.expect_name("AddressOf");
  return read_body_AddressOf(r);
}

void write(::c6502::asdl::Writer& w, const Subscript& v) {
  w.write_name("Subscript");
  w.open();
  write(w, *v.array);
  w.space();
  write(w, *v.index);
  w.space();
  if (v.data_type) {
    write(w, (*v.data_type));
  } else {
    w.write_none();
  }
  w.close();
}

static Subscript read_body_Subscript(::c6502::asdl::Reader& r) {
  r.open();
  ::c6502::asdl::Box<Type_exp> f0 = ::c6502::asdl::Box<Type_exp>(read(r, ::c6502::asdl::Tag<Type_exp>{}));
  ::c6502::asdl::Box<Type_exp> f1 = ::c6502::asdl::Box<Type_exp>(read(r, ::c6502::asdl::Tag<Type_exp>{}));
  std::optional<Type_data_type> f2;
  if (!r.read_none()) {
    f2 = read(r, ::c6502::asdl::Tag<Type_data_type>{});
  }
  r.close();
  return Subscript{std::move(f0), std::move(f1), std::move(f2)};
}

Subscript read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Subscript>) {
  r.expect_name("Subscript");
  return read_body_Subscript(r);
}

void write(::c6502::asdl::Writer& w, const InitList& v) {
  w.write_name("InitList");
  w.open();
  w.begin_seq(false);
  for (const auto& e : v.items) {
    w.seq_item(false);
    write(w, e);
  }
  w.end_seq(false, v.items.empty());
  w.space();
  if (v.data_type) {
    write(w, (*v.data_type));
  } else {
    w.write_none();
  }
  w.space();
  if (v.union_member) {
    w.write_string((*v.union_member));
  } else {
    w.write_none();
  }
  w.close();
}

static InitList read_body_InitList(::c6502::asdl::Reader& r) {
  r.open();
  std::vector<Type_exp> f0;
  r.begin_seq();
  while (!r.end_seq()) {
    f0.push_back(read(r, ::c6502::asdl::Tag<Type_exp>{}));
  }
  std::optional<Type_data_type> f1;
  if (!r.read_none()) {
    f1 = read(r, ::c6502::asdl::Tag<Type_data_type>{});
  }
  std::optional<std::string> f2;
  if (!r.read_none()) {
    f2 = r.read_string();
  }
  r.close();
  return InitList{std::move(f0), std::move(f1), std::move(f2)};
}

InitList read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<InitList>) {
  r.expect_name("InitList");
  return read_body_InitList(r);
}

void write(::c6502::asdl::Writer& w, const Designation& v) {
  w.write_name("Designation");
  w.open();
  w.begin_seq(false);
  for (const auto& e : v.designators) {
    w.seq_item(false);
    write(w, e);
  }
  w.end_seq(false, v.designators.empty());
  w.space();
  write(w, *v.init);
  w.close();
}

static Designation read_body_Designation(::c6502::asdl::Reader& r) {
  r.open();
  std::vector<Type_designator> f0;
  r.begin_seq();
  while (!r.end_seq()) {
    f0.push_back(read(r, ::c6502::asdl::Tag<Type_designator>{}));
  }
  ::c6502::asdl::Box<Type_exp> f1 = ::c6502::asdl::Box<Type_exp>(read(r, ::c6502::asdl::Tag<Type_exp>{}));
  r.close();
  return Designation{std::move(f0), std::move(f1)};
}

Designation read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Designation>) {
  r.expect_name("Designation");
  return read_body_Designation(r);
}

void write(::c6502::asdl::Writer& w, const SizeOfExp& v) {
  w.write_name("SizeOfExp");
  w.open();
  write(w, *v.exp);
  w.space();
  if (v.data_type) {
    write(w, (*v.data_type));
  } else {
    w.write_none();
  }
  w.close();
}

static SizeOfExp read_body_SizeOfExp(::c6502::asdl::Reader& r) {
  r.open();
  ::c6502::asdl::Box<Type_exp> f0 = ::c6502::asdl::Box<Type_exp>(read(r, ::c6502::asdl::Tag<Type_exp>{}));
  std::optional<Type_data_type> f1;
  if (!r.read_none()) {
    f1 = read(r, ::c6502::asdl::Tag<Type_data_type>{});
  }
  r.close();
  return SizeOfExp{std::move(f0), std::move(f1)};
}

SizeOfExp read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<SizeOfExp>) {
  r.expect_name("SizeOfExp");
  return read_body_SizeOfExp(r);
}

void write(::c6502::asdl::Writer& w, const SizeOfType& v) {
  w.write_name("SizeOfType");
  w.open();
  write(w, v.target_type);
  w.space();
  if (v.data_type) {
    write(w, (*v.data_type));
  } else {
    w.write_none();
  }
  w.close();
}

static SizeOfType read_body_SizeOfType(::c6502::asdl::Reader& r) {
  r.open();
  Type_data_type f0 = read(r, ::c6502::asdl::Tag<Type_data_type>{});
  std::optional<Type_data_type> f1;
  if (!r.read_none()) {
    f1 = read(r, ::c6502::asdl::Tag<Type_data_type>{});
  }
  r.close();
  return SizeOfType{std::move(f0), std::move(f1)};
}

SizeOfType read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<SizeOfType>) {
  r.expect_name("SizeOfType");
  return read_body_SizeOfType(r);
}

void write(::c6502::asdl::Writer& w, const Dot& v) {
  w.write_name("Dot");
  w.open();
  write(w, *v.operand);
  w.space();
  w.write_string(v.member);
  w.space();
  if (v.data_type) {
    write(w, (*v.data_type));
  } else {
    w.write_none();
  }
  w.close();
}

static Dot read_body_Dot(::c6502::asdl::Reader& r) {
  r.open();
  ::c6502::asdl::Box<Type_exp> f0 = ::c6502::asdl::Box<Type_exp>(read(r, ::c6502::asdl::Tag<Type_exp>{}));
  std::string f1 = r.read_string();
  std::optional<Type_data_type> f2;
  if (!r.read_none()) {
    f2 = read(r, ::c6502::asdl::Tag<Type_data_type>{});
  }
  r.close();
  return Dot{std::move(f0), std::move(f1), std::move(f2)};
}

Dot read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Dot>) {
  r.expect_name("Dot");
  return read_body_Dot(r);
}

void write(::c6502::asdl::Writer& w, const Arrow& v) {
  w.write_name("Arrow");
  w.open();
  write(w, *v.operand);
  w.space();
  w.write_string(v.member);
  w.space();
  if (v.data_type) {
    write(w, (*v.data_type));
  } else {
    w.write_none();
  }
  w.close();
}

static Arrow read_body_Arrow(::c6502::asdl::Reader& r) {
  r.open();
  ::c6502::asdl::Box<Type_exp> f0 = ::c6502::asdl::Box<Type_exp>(read(r, ::c6502::asdl::Tag<Type_exp>{}));
  std::string f1 = r.read_string();
  std::optional<Type_data_type> f2;
  if (!r.read_none()) {
    f2 = read(r, ::c6502::asdl::Tag<Type_data_type>{});
  }
  r.close();
  return Arrow{std::move(f0), std::move(f1), std::move(f2)};
}

Arrow read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Arrow>) {
  r.expect_name("Arrow");
  return read_body_Arrow(r);
}

void write(::c6502::asdl::Writer& w, const Comma& v) {
  w.write_name("Comma");
  w.open();
  write(w, *v.left);
  w.space();
  write(w, *v.right);
  w.space();
  if (v.data_type) {
    write(w, (*v.data_type));
  } else {
    w.write_none();
  }
  w.close();
}

static Comma read_body_Comma(::c6502::asdl::Reader& r) {
  r.open();
  ::c6502::asdl::Box<Type_exp> f0 = ::c6502::asdl::Box<Type_exp>(read(r, ::c6502::asdl::Tag<Type_exp>{}));
  ::c6502::asdl::Box<Type_exp> f1 = ::c6502::asdl::Box<Type_exp>(read(r, ::c6502::asdl::Tag<Type_exp>{}));
  std::optional<Type_data_type> f2;
  if (!r.read_none()) {
    f2 = read(r, ::c6502::asdl::Tag<Type_data_type>{});
  }
  r.close();
  return Comma{std::move(f0), std::move(f1), std::move(f2)};
}

Comma read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Comma>) {
  r.expect_name("Comma");
  return read_body_Comma(r);
}

void write(::c6502::asdl::Writer& w, const CompoundLiteral& v) {
  w.write_name("CompoundLiteral");
  w.open();
  w.write_string(v.name);
  w.space();
  write(w, v.target_type);
  w.space();
  write(w, *v.init);
  w.space();
  if (v.data_type) {
    write(w, (*v.data_type));
  } else {
    w.write_none();
  }
  w.space();
  if (v.storage_class) {
    write(w, (*v.storage_class));
  } else {
    w.write_none();
  }
  w.space();
  w.write_bool(v.is_constexpr);
  w.close();
}

static CompoundLiteral read_body_CompoundLiteral(::c6502::asdl::Reader& r) {
  r.open();
  std::string f0 = r.read_string();
  Type_data_type f1 = read(r, ::c6502::asdl::Tag<Type_data_type>{});
  ::c6502::asdl::Box<Type_exp> f2 = ::c6502::asdl::Box<Type_exp>(read(r, ::c6502::asdl::Tag<Type_exp>{}));
  std::optional<Type_data_type> f3;
  if (!r.read_none()) {
    f3 = read(r, ::c6502::asdl::Tag<Type_data_type>{});
  }
  std::optional<Type_storage_class> f4;
  if (!r.read_none()) {
    f4 = read(r, ::c6502::asdl::Tag<Type_storage_class>{});
  }
  bool f5 = r.read_bool();
  r.close();
  return CompoundLiteral{std::move(f0), std::move(f1), std::move(f2), std::move(f3), std::move(f4), std::move(f5)};
}

CompoundLiteral read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<CompoundLiteral>) {
  r.expect_name("CompoundLiteral");
  return read_body_CompoundLiteral(r);
}

void write(::c6502::asdl::Writer& w, const Type_exp& v) {
  switch (v.index()) {
    case 0:
      write(w, *std::get_if<0>(&v));
      return;
    case 1:
      write(w, *std::get_if<1>(&v));
      return;
    case 2:
      write(w, *std::get_if<2>(&v));
      return;
    case 3:
      write(w, *std::get_if<3>(&v));
      return;
    case 4:
      write(w, *std::get_if<4>(&v));
      return;
    case 5:
      write(w, *std::get_if<5>(&v));
      return;
    case 6:
      write(w, *std::get_if<6>(&v));
      return;
    case 7:
      write(w, *std::get_if<7>(&v));
      return;
    case 8:
      write(w, *std::get_if<8>(&v));
      return;
    case 9:
      write(w, *std::get_if<9>(&v));
      return;
    case 10:
      write(w, *std::get_if<10>(&v));
      return;
    case 11:
      write(w, *std::get_if<11>(&v));
      return;
    case 12:
      write(w, *std::get_if<12>(&v));
      return;
    case 13:
      write(w, *std::get_if<13>(&v));
      return;
    case 14:
      write(w, *std::get_if<14>(&v));
      return;
    case 15:
      write(w, *std::get_if<15>(&v));
      return;
    case 16:
      write(w, *std::get_if<16>(&v));
      return;
    case 17:
      write(w, *std::get_if<17>(&v));
      return;
    case 18:
      write(w, *std::get_if<18>(&v));
      return;
    case 19:
      write(w, *std::get_if<19>(&v));
      return;
    case 20:
      write(w, *std::get_if<20>(&v));
      return;
    case 21:
      write(w, *std::get_if<21>(&v));
      return;
    case 22:
      write(w, *std::get_if<22>(&v));
      return;
    case 23:
      write(w, *std::get_if<23>(&v));
      return;
  }
}

Type_exp read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Type_exp>) {
  const std::string_view name = r.read_name();
  switch (name.size()) {
    case 3:
      if (name == "Var") {
        return read_body_Var(r);
      }
      if (name == "Dot") {
        return read_body_Dot(r);
      }
      break;
    case 4:
      if (name == "Cast") {
        return read_body_Cast(r);
      }
      break;
    case 5:
      if (name == "Unary") {
        return read_body_Unary(r);
      }
      if (name == "Arrow") {
        return read_body_Arrow(r);
      }
      if (name == "Comma") {
        return read_body_Comma(r);
      }
      break;
    case 6:
      if (name == "String") {
        return read_body_String(r);
      }
      if (name == "Binary") {
        return read_body_Binary(r);
      }
      if (name == "Prefix") {
        return read_body_Prefix(r);
      }
      break;
    case 7:
      if (name == "Postfix") {
        return read_body_Postfix(r);
      }
      break;
    case 8:
      if (name == "Constant") {
        return read_body_Constant(r);
      }
      if (name == "InitList") {
        return read_body_InitList(r);
      }
      break;
    case 9:
      if (name == "AddressOf") {
        return read_body_AddressOf(r);
      }
      if (name == "Subscript") {
        return read_body_Subscript(r);
      }
      if (name == "SizeOfExp") {
        return read_body_SizeOfExp(r);
      }
      break;
    case 10:
      if (name == "Assignment") {
        return read_body_Assignment(r);
      }
      if (name == "SizeOfType") {
        return read_body_SizeOfType(r);
      }
      break;
    case 11:
      if (name == "Conditional") {
        return read_body_Conditional(r);
      }
      if (name == "Dereference") {
        return read_body_Dereference(r);
      }
      if (name == "Designation") {
        return read_body_Designation(r);
      }
      break;
    case 12:
      if (name == "FunctionCall") {
        return read_body_FunctionCall(r);
      }
      if (name == "IndirectCall") {
        return read_body_IndirectCall(r);
      }
      break;
    case 15:
      if (name == "CompoundLiteral") {
        return read_body_CompoundLiteral(r);
      }
      break;
    case 18:
      if (name == "CompoundAssignment") {
        return read_body_CompoundAssignment(r);
      }
      break;
  }
  r.unknown_constructor(name, "exp");
}

void write(::c6502::asdl::Writer& w, const ZeroPage& v) {
  w.write_name("ZeroPage");
  w.open();
  if (v.address) {
    w.write_int((*v.address));
  } else {
    w.write_none();
  }
  w.close();
}

static ZeroPage read_body_ZeroPage(::c6502::asdl::Reader& r) {
  r.open();
  std::optional<::c6502::asdl::Int> f0;
  if (!r.read_none()) {
    f0 = r.read_int();
  }
  r.close();
  return ZeroPage{std::move(f0)};
}

ZeroPage read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<ZeroPage>) {
  r.expect_name("ZeroPage");
  return read_body_ZeroPage(r);
}

void write(::c6502::asdl::Writer& w, const Absolute& v) {
  w.write_name("Absolute");
  w.open();
  w.write_int(v.address);
  w.close();
}

static Absolute read_body_Absolute(::c6502::asdl::Reader& r) {
  r.open();
  ::c6502::asdl::Int f0 = r.read_int();
  r.close();
  return Absolute{std::move(f0)};
}

Absolute read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Absolute>) {
  r.expect_name("Absolute");
  return read_body_Absolute(r);
}

void write(::c6502::asdl::Writer& w, const Type_placement& v) {
  switch (v.index()) {
    case 0:
      write(w, *std::get_if<0>(&v));
      return;
    case 1:
      write(w, *std::get_if<1>(&v));
      return;
  }
}

Type_placement read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Type_placement>) {
  const std::string_view name = r.read_name();
  switch (name.size()) {
    case 8:
      if (name == "ZeroPage") {
        return read_body_ZeroPage(r);
      }
      if (name == "Absolute") {
        return read_body_Absolute(r);
      }
      break;
  }
  r.unknown_constructor(name, "placement");
}

void write(::c6502::asdl::Writer& w, const Type_var_decl& v) {
  w.write_name("var_decl");
  w.open();
  w.write_string(v.name);
  w.space();
  if (v.init) {
    write(w, (*v.init));
  } else {
    w.write_none();
  }
  w.space();
  write(w, v.data_type);
  w.space();
  if (v.storage_class) {
    write(w, (*v.storage_class));
  } else {
    w.write_none();
  }
  w.space();
  if (v.abi_annotation) {
    w.write_string((*v.abi_annotation));
  } else {
    w.write_none();
  }
  w.space();
  if (v.placement) {
    write(w, (*v.placement));
  } else {
    w.write_none();
  }
  w.space();
  if (v.section) {
    w.write_string((*v.section));
  } else {
    w.write_none();
  }
  w.space();
  w.write_bool(v.is_constexpr);
  w.close();
}

static Type_var_decl read_body_Type_var_decl(::c6502::asdl::Reader& r) {
  r.open();
  std::string f0 = r.read_string();
  std::optional<Type_exp> f1;
  if (!r.read_none()) {
    f1 = read(r, ::c6502::asdl::Tag<Type_exp>{});
  }
  Type_data_type f2 = read(r, ::c6502::asdl::Tag<Type_data_type>{});
  std::optional<Type_storage_class> f3;
  if (!r.read_none()) {
    f3 = read(r, ::c6502::asdl::Tag<Type_storage_class>{});
  }
  std::optional<std::string> f4;
  if (!r.read_none()) {
    f4 = r.read_string();
  }
  std::optional<Type_placement> f5;
  if (!r.read_none()) {
    f5 = read(r, ::c6502::asdl::Tag<Type_placement>{});
  }
  std::optional<std::string> f6;
  if (!r.read_none()) {
    f6 = r.read_string();
  }
  bool f7 = r.read_bool();
  r.close();
  return Type_var_decl{std::move(f0), std::move(f1), std::move(f2), std::move(f3), std::move(f4), std::move(f5), std::move(f6), std::move(f7)};
}

Type_var_decl read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Type_var_decl>) {
  r.expect_name("var_decl");
  return read_body_Type_var_decl(r);
}

void write(::c6502::asdl::Writer& w, const InitDecl& v) {
  w.write_name("InitDecl");
  w.open();
  w.begin_seq(false);
  for (const auto& e : v.var_decls) {
    w.seq_item(false);
    write(w, e);
  }
  w.end_seq(false, v.var_decls.empty());
  w.close();
}

static InitDecl read_body_InitDecl(::c6502::asdl::Reader& r) {
  r.open();
  std::vector<Type_var_decl> f0;
  r.begin_seq();
  while (!r.end_seq()) {
    f0.push_back(read(r, ::c6502::asdl::Tag<Type_var_decl>{}));
  }
  r.close();
  return InitDecl{std::move(f0)};
}

InitDecl read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<InitDecl>) {
  r.expect_name("InitDecl");
  return read_body_InitDecl(r);
}

void write(::c6502::asdl::Writer& w, const InitExp& v) {
  w.write_name("InitExp");
  w.open();
  if (v.exp) {
    write(w, (*v.exp));
  } else {
    w.write_none();
  }
  w.close();
}

static InitExp read_body_InitExp(::c6502::asdl::Reader& r) {
  r.open();
  std::optional<Type_exp> f0;
  if (!r.read_none()) {
    f0 = read(r, ::c6502::asdl::Tag<Type_exp>{});
  }
  r.close();
  return InitExp{std::move(f0)};
}

InitExp read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<InitExp>) {
  r.expect_name("InitExp");
  return read_body_InitExp(r);
}

void write(::c6502::asdl::Writer& w, const Type_for_init& v) {
  switch (v.index()) {
    case 0:
      write(w, *std::get_if<0>(&v));
      return;
    case 1:
      write(w, *std::get_if<1>(&v));
      return;
  }
}

Type_for_init read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Type_for_init>) {
  const std::string_view name = r.read_name();
  switch (name.size()) {
    case 7:
      if (name == "InitExp") {
        return read_body_InitExp(r);
      }
      break;
    case 8:
      if (name == "InitDecl") {
        return read_body_InitDecl(r);
      }
      break;
  }
  r.unknown_constructor(name, "for_init");
}

void write(::c6502::asdl::Writer& w, const Type_switch_case& v) {
  w.write_name("switch_case");
  w.open();
  write(w, v.value);
  w.space();
  w.write_string(v.label);
  w.close();
}

static Type_switch_case read_body_Type_switch_case(::c6502::asdl::Reader& r) {
  r.open();
  Type_exp f0 = read(r, ::c6502::asdl::Tag<Type_exp>{});
  std::string f1 = r.read_string();
  r.close();
  return Type_switch_case{std::move(f0), std::move(f1)};
}

Type_switch_case read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Type_switch_case>) {
  r.expect_name("switch_case");
  return read_body_Type_switch_case(r);
}

void write(::c6502::asdl::Writer& w, const Type_member_decl& v) {
  w.write_name("member_decl");
  w.open();
  w.write_string(v.name);
  w.space();
  write(w, v.data_type);
  w.close();
}

static Type_member_decl read_body_Type_member_decl(::c6502::asdl::Reader& r) {
  r.open();
  std::string f0 = r.read_string();
  Type_data_type f1 = read(r, ::c6502::asdl::Tag<Type_data_type>{});
  r.close();
  return Type_member_decl{std::move(f0), std::move(f1)};
}

Type_member_decl read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Type_member_decl>) {
  r.expect_name("member_decl");
  return read_body_Type_member_decl(r);
}

void write(::c6502::asdl::Writer& w, const Type_struct_decl& v) {
  w.write_name("struct_decl");
  w.open();
  w.write_string(v.tag);
  w.space();
  w.write_bool(v.is_union);
  w.space();
  w.begin_seq(false);
  for (const auto& e : v.members) {
    w.seq_item(false);
    write(w, e);
  }
  w.end_seq(false, v.members.empty());
  w.close();
}

static Type_struct_decl read_body_Type_struct_decl(::c6502::asdl::Reader& r) {
  r.open();
  std::string f0 = r.read_string();
  bool f1 = r.read_bool();
  std::vector<Type_member_decl> f2;
  r.begin_seq();
  while (!r.end_seq()) {
    f2.push_back(read(r, ::c6502::asdl::Tag<Type_member_decl>{}));
  }
  r.close();
  return Type_struct_decl{std::move(f0), std::move(f1), std::move(f2)};
}

Type_struct_decl read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Type_struct_decl>) {
  r.expect_name("struct_decl");
  return read_body_Type_struct_decl(r);
}

void write(::c6502::asdl::Writer& w, const Block& v) {
  w.write_name("Block");
  w.open();
  w.begin_seq(true);
  for (const auto& e : v.block_item) {
    w.seq_item(true);
    write(w, e);
  }
  w.end_seq(true, v.block_item.empty());
  w.close();
}

static Block read_body_Block(::c6502::asdl::Reader& r) {
  r.open();
  std::vector<Type_block_item> f0;
  r.begin_seq();
  while (!r.end_seq()) {
    f0.push_back(read(r, ::c6502::asdl::Tag<Type_block_item>{}));
  }
  r.close();
  return Block{std::move(f0)};
}

Block read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Block>) {
  r.expect_name("Block");
  return read_body_Block(r);
}

void write(::c6502::asdl::Writer& w, const Type_block& v) {
  switch (v.index()) {
    case 0:
      write(w, *std::get_if<0>(&v));
      return;
  }
}

Type_block read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Type_block>) {
  const std::string_view name = r.read_name();
  switch (name.size()) {
    case 5:
      if (name == "Block") {
        return read_body_Block(r);
      }
      break;
  }
  r.unknown_constructor(name, "block");
}

void write(::c6502::asdl::Writer& w, const S& v) {
  w.write_name("S");
  w.open();
  write(w, *v.statement);
  w.close();
}

static S read_body_S(::c6502::asdl::Reader& r) {
  r.open();
  ::c6502::asdl::Box<Type_statement> f0 = ::c6502::asdl::Box<Type_statement>(read(r, ::c6502::asdl::Tag<Type_statement>{}));
  r.close();
  return S{std::move(f0)};
}

S read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<S>) {
  r.expect_name("S");
  return read_body_S(r);
}

void write(::c6502::asdl::Writer& w, const D& v) {
  w.write_name("D");
  w.open();
  write(w, *v.declaration);
  w.close();
}

static D read_body_D(::c6502::asdl::Reader& r) {
  r.open();
  ::c6502::asdl::Box<Type_declaration> f0 = ::c6502::asdl::Box<Type_declaration>(read(r, ::c6502::asdl::Tag<Type_declaration>{}));
  r.close();
  return D{std::move(f0)};
}

D read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<D>) {
  r.expect_name("D");
  return read_body_D(r);
}

void write(::c6502::asdl::Writer& w, const Type_block_item& v) {
  switch (v.index()) {
    case 0:
      write(w, *std::get_if<0>(&v));
      return;
    case 1:
      write(w, *std::get_if<1>(&v));
      return;
  }
}

Type_block_item read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Type_block_item>) {
  const std::string_view name = r.read_name();
  switch (name.size()) {
    case 1:
      if (name == "S") {
        return read_body_S(r);
      }
      if (name == "D") {
        return read_body_D(r);
      }
      break;
  }
  r.unknown_constructor(name, "block_item");
}

void write(::c6502::asdl::Writer& w, const Return& v) {
  w.write_name("Return");
  w.open();
  if (v.exp) {
    write(w, (*v.exp));
  } else {
    w.write_none();
  }
  w.close();
}

static Return read_body_Return(::c6502::asdl::Reader& r) {
  r.open();
  std::optional<Type_exp> f0;
  if (!r.read_none()) {
    f0 = read(r, ::c6502::asdl::Tag<Type_exp>{});
  }
  r.close();
  return Return{std::move(f0)};
}

Return read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Return>) {
  r.expect_name("Return");
  return read_body_Return(r);
}

void write(::c6502::asdl::Writer& w, const Expression& v) {
  w.write_name("Expression");
  w.open();
  write(w, v.exp);
  w.close();
}

static Expression read_body_Expression(::c6502::asdl::Reader& r) {
  r.open();
  Type_exp f0 = read(r, ::c6502::asdl::Tag<Type_exp>{});
  r.close();
  return Expression{std::move(f0)};
}

Expression read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Expression>) {
  r.expect_name("Expression");
  return read_body_Expression(r);
}

void write(::c6502::asdl::Writer& w, const IfStmt& v) {
  w.write_name("IfStmt");
  w.open();
  write(w, v.condition);
  w.space();
  write(w, *v.then_clause);
  w.space();
  if (v.else_clause) {
    write(w, *(*v.else_clause));
  } else {
    w.write_none();
  }
  w.close();
}

static IfStmt read_body_IfStmt(::c6502::asdl::Reader& r) {
  r.open();
  Type_exp f0 = read(r, ::c6502::asdl::Tag<Type_exp>{});
  ::c6502::asdl::Box<Type_statement> f1 = ::c6502::asdl::Box<Type_statement>(read(r, ::c6502::asdl::Tag<Type_statement>{}));
  std::optional<::c6502::asdl::Box<Type_statement>> f2;
  if (!r.read_none()) {
    f2 = ::c6502::asdl::Box<Type_statement>(read(r, ::c6502::asdl::Tag<Type_statement>{}));
  }
  r.close();
  return IfStmt{std::move(f0), std::move(f1), std::move(f2)};
}

IfStmt read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<IfStmt>) {
  r.expect_name("IfStmt");
  return read_body_IfStmt(r);
}

void write(::c6502::asdl::Writer& w, const Compound& v) {
  w.write_name("Compound");
  w.open();
  write(w, *v.block);
  w.close();
}

static Compound read_body_Compound(::c6502::asdl::Reader& r) {
  r.open();
  ::c6502::asdl::Box<Type_block> f0 = ::c6502::asdl::Box<Type_block>(read(r, ::c6502::asdl::Tag<Type_block>{}));
  r.close();
  return Compound{std::move(f0)};
}

Compound read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Compound>) {
  r.expect_name("Compound");
  return read_body_Compound(r);
}

void write(::c6502::asdl::Writer& w, const BreakStmt& v) {
  w.write_name("BreakStmt");
  w.open();
  w.write_string(v.label);
  w.close();
}

static BreakStmt read_body_BreakStmt(::c6502::asdl::Reader& r) {
  r.open();
  std::string f0 = r.read_string();
  r.close();
  return BreakStmt{std::move(f0)};
}

BreakStmt read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<BreakStmt>) {
  r.expect_name("BreakStmt");
  return read_body_BreakStmt(r);
}

void write(::c6502::asdl::Writer& w, const ContinueStmt& v) {
  w.write_name("ContinueStmt");
  w.open();
  w.write_string(v.label);
  w.close();
}

static ContinueStmt read_body_ContinueStmt(::c6502::asdl::Reader& r) {
  r.open();
  std::string f0 = r.read_string();
  r.close();
  return ContinueStmt{std::move(f0)};
}

ContinueStmt read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<ContinueStmt>) {
  r.expect_name("ContinueStmt");
  return read_body_ContinueStmt(r);
}

void write(::c6502::asdl::Writer& w, const WhileStmt& v) {
  w.write_name("WhileStmt");
  w.open();
  write(w, v.condition);
  w.space();
  write(w, *v.body);
  w.space();
  w.write_string(v.label);
  w.close();
}

static WhileStmt read_body_WhileStmt(::c6502::asdl::Reader& r) {
  r.open();
  Type_exp f0 = read(r, ::c6502::asdl::Tag<Type_exp>{});
  ::c6502::asdl::Box<Type_statement> f1 = ::c6502::asdl::Box<Type_statement>(read(r, ::c6502::asdl::Tag<Type_statement>{}));
  std::string f2 = r.read_string();
  r.close();
  return WhileStmt{std::move(f0), std::move(f1), std::move(f2)};
}

WhileStmt read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<WhileStmt>) {
  r.expect_name("WhileStmt");
  return read_body_WhileStmt(r);
}

void write(::c6502::asdl::Writer& w, const DoWhileStmt& v) {
  w.write_name("DoWhileStmt");
  w.open();
  write(w, *v.body);
  w.space();
  write(w, v.condition);
  w.space();
  w.write_string(v.label);
  w.close();
}

static DoWhileStmt read_body_DoWhileStmt(::c6502::asdl::Reader& r) {
  r.open();
  ::c6502::asdl::Box<Type_statement> f0 = ::c6502::asdl::Box<Type_statement>(read(r, ::c6502::asdl::Tag<Type_statement>{}));
  Type_exp f1 = read(r, ::c6502::asdl::Tag<Type_exp>{});
  std::string f2 = r.read_string();
  r.close();
  return DoWhileStmt{std::move(f0), std::move(f1), std::move(f2)};
}

DoWhileStmt read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<DoWhileStmt>) {
  r.expect_name("DoWhileStmt");
  return read_body_DoWhileStmt(r);
}

void write(::c6502::asdl::Writer& w, const ForStmt& v) {
  w.write_name("ForStmt");
  w.open();
  write(w, v.init);
  w.space();
  if (v.condition) {
    write(w, (*v.condition));
  } else {
    w.write_none();
  }
  w.space();
  if (v.post_clause) {
    write(w, (*v.post_clause));
  } else {
    w.write_none();
  }
  w.space();
  write(w, *v.body);
  w.space();
  w.write_string(v.label);
  w.space();
  if (v.loop_attribute) {
    w.write_string((*v.loop_attribute));
  } else {
    w.write_none();
  }
  w.close();
}

static ForStmt read_body_ForStmt(::c6502::asdl::Reader& r) {
  r.open();
  Type_for_init f0 = read(r, ::c6502::asdl::Tag<Type_for_init>{});
  std::optional<Type_exp> f1;
  if (!r.read_none()) {
    f1 = read(r, ::c6502::asdl::Tag<Type_exp>{});
  }
  std::optional<Type_exp> f2;
  if (!r.read_none()) {
    f2 = read(r, ::c6502::asdl::Tag<Type_exp>{});
  }
  ::c6502::asdl::Box<Type_statement> f3 = ::c6502::asdl::Box<Type_statement>(read(r, ::c6502::asdl::Tag<Type_statement>{}));
  std::string f4 = r.read_string();
  std::optional<std::string> f5;
  if (!r.read_none()) {
    f5 = r.read_string();
  }
  r.close();
  return ForStmt{std::move(f0), std::move(f1), std::move(f2), std::move(f3), std::move(f4), std::move(f5)};
}

ForStmt read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<ForStmt>) {
  r.expect_name("ForStmt");
  return read_body_ForStmt(r);
}

void write(::c6502::asdl::Writer& w, const SwitchStmt& v) {
  w.write_name("SwitchStmt");
  w.open();
  write(w, v.control);
  w.space();
  write(w, *v.body);
  w.space();
  w.write_string(v.label);
  w.space();
  w.begin_seq(false);
  for (const auto& e : v.cases) {
    w.seq_item(false);
    write(w, e);
  }
  w.end_seq(false, v.cases.empty());
  w.space();
  if (v.default_label) {
    w.write_string((*v.default_label));
  } else {
    w.write_none();
  }
  w.space();
  if (v.promoted_type) {
    write(w, (*v.promoted_type));
  } else {
    w.write_none();
  }
  w.close();
}

static SwitchStmt read_body_SwitchStmt(::c6502::asdl::Reader& r) {
  r.open();
  Type_exp f0 = read(r, ::c6502::asdl::Tag<Type_exp>{});
  ::c6502::asdl::Box<Type_statement> f1 = ::c6502::asdl::Box<Type_statement>(read(r, ::c6502::asdl::Tag<Type_statement>{}));
  std::string f2 = r.read_string();
  std::vector<Type_switch_case> f3;
  r.begin_seq();
  while (!r.end_seq()) {
    f3.push_back(read(r, ::c6502::asdl::Tag<Type_switch_case>{}));
  }
  std::optional<std::string> f4;
  if (!r.read_none()) {
    f4 = r.read_string();
  }
  std::optional<Type_data_type> f5;
  if (!r.read_none()) {
    f5 = read(r, ::c6502::asdl::Tag<Type_data_type>{});
  }
  r.close();
  return SwitchStmt{std::move(f0), std::move(f1), std::move(f2), std::move(f3), std::move(f4), std::move(f5)};
}

SwitchStmt read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<SwitchStmt>) {
  r.expect_name("SwitchStmt");
  return read_body_SwitchStmt(r);
}

void write(::c6502::asdl::Writer& w, const CaseStmt& v) {
  w.write_name("CaseStmt");
  w.open();
  write(w, v.value);
  w.space();
  write(w, *v.body);
  w.space();
  w.write_string(v.label);
  w.close();
}

static CaseStmt read_body_CaseStmt(::c6502::asdl::Reader& r) {
  r.open();
  Type_exp f0 = read(r, ::c6502::asdl::Tag<Type_exp>{});
  ::c6502::asdl::Box<Type_statement> f1 = ::c6502::asdl::Box<Type_statement>(read(r, ::c6502::asdl::Tag<Type_statement>{}));
  std::string f2 = r.read_string();
  r.close();
  return CaseStmt{std::move(f0), std::move(f1), std::move(f2)};
}

CaseStmt read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<CaseStmt>) {
  r.expect_name("CaseStmt");
  return read_body_CaseStmt(r);
}

void write(::c6502::asdl::Writer& w, const DefaultStmt& v) {
  w.write_name("DefaultStmt");
  w.open();
  write(w, *v.body);
  w.space();
  w.write_string(v.label);
  w.close();
}

static DefaultStmt read_body_DefaultStmt(::c6502::asdl::Reader& r) {
  r.open();
  ::c6502::asdl::Box<Type_statement> f0 = ::c6502::asdl::Box<Type_statement>(read(r, ::c6502::asdl::Tag<Type_statement>{}));
  std::string f1 = r.read_string();
  r.close();
  return DefaultStmt{std::move(f0), std::move(f1)};
}

DefaultStmt read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<DefaultStmt>) {
  r.expect_name("DefaultStmt");
  return read_body_DefaultStmt(r);
}

void write(::c6502::asdl::Writer& w, const Goto& v) {
  w.write_name("Goto");
  w.open();
  w.write_string(v.label);
  w.close();
}

static Goto read_body_Goto(::c6502::asdl::Reader& r) {
  r.open();
  std::string f0 = r.read_string();
  r.close();
  return Goto{std::move(f0)};
}

Goto read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Goto>) {
  r.expect_name("Goto");
  return read_body_Goto(r);
}

void write(::c6502::asdl::Writer& w, const LabeledStmt& v) {
  w.write_name("LabeledStmt");
  w.open();
  w.write_string(v.label);
  w.space();
  write(w, *v.statement);
  w.close();
}

static LabeledStmt read_body_LabeledStmt(::c6502::asdl::Reader& r) {
  r.open();
  std::string f0 = r.read_string();
  ::c6502::asdl::Box<Type_statement> f1 = ::c6502::asdl::Box<Type_statement>(read(r, ::c6502::asdl::Tag<Type_statement>{}));
  r.close();
  return LabeledStmt{std::move(f0), std::move(f1)};
}

LabeledStmt read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<LabeledStmt>) {
  r.expect_name("LabeledStmt");
  return read_body_LabeledStmt(r);
}

void write(::c6502::asdl::Writer& w, const Null&) {
  w.write_name("Null");
}

static Null read_body_Null(::c6502::asdl::Reader& r) {
  (void)r;
  return Null{};
}

Null read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Null>) {
  r.expect_name("Null");
  return read_body_Null(r);
}

void write(::c6502::asdl::Writer& w, const Type_statement& v) {
  switch (v.index()) {
    case 0:
      write(w, *std::get_if<0>(&v));
      return;
    case 1:
      write(w, *std::get_if<1>(&v));
      return;
    case 2:
      write(w, *std::get_if<2>(&v));
      return;
    case 3:
      write(w, *std::get_if<3>(&v));
      return;
    case 4:
      write(w, *std::get_if<4>(&v));
      return;
    case 5:
      write(w, *std::get_if<5>(&v));
      return;
    case 6:
      write(w, *std::get_if<6>(&v));
      return;
    case 7:
      write(w, *std::get_if<7>(&v));
      return;
    case 8:
      write(w, *std::get_if<8>(&v));
      return;
    case 9:
      write(w, *std::get_if<9>(&v));
      return;
    case 10:
      write(w, *std::get_if<10>(&v));
      return;
    case 11:
      write(w, *std::get_if<11>(&v));
      return;
    case 12:
      write(w, *std::get_if<12>(&v));
      return;
    case 13:
      write(w, *std::get_if<13>(&v));
      return;
    case 14:
      write(w, *std::get_if<14>(&v));
      return;
  }
}

Type_statement read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Type_statement>) {
  const std::string_view name = r.read_name();
  switch (name.size()) {
    case 4:
      if (name == "Goto") {
        return read_body_Goto(r);
      }
      if (name == "Null") {
        return read_body_Null(r);
      }
      break;
    case 6:
      if (name == "Return") {
        return read_body_Return(r);
      }
      if (name == "IfStmt") {
        return read_body_IfStmt(r);
      }
      break;
    case 7:
      if (name == "ForStmt") {
        return read_body_ForStmt(r);
      }
      break;
    case 8:
      if (name == "Compound") {
        return read_body_Compound(r);
      }
      if (name == "CaseStmt") {
        return read_body_CaseStmt(r);
      }
      break;
    case 9:
      if (name == "BreakStmt") {
        return read_body_BreakStmt(r);
      }
      if (name == "WhileStmt") {
        return read_body_WhileStmt(r);
      }
      break;
    case 10:
      if (name == "Expression") {
        return read_body_Expression(r);
      }
      if (name == "SwitchStmt") {
        return read_body_SwitchStmt(r);
      }
      break;
    case 11:
      if (name == "DoWhileStmt") {
        return read_body_DoWhileStmt(r);
      }
      if (name == "DefaultStmt") {
        return read_body_DefaultStmt(r);
      }
      if (name == "LabeledStmt") {
        return read_body_LabeledStmt(r);
      }
      break;
    case 12:
      if (name == "ContinueStmt") {
        return read_body_ContinueStmt(r);
      }
      break;
  }
  r.unknown_constructor(name, "statement");
}

void write(::c6502::asdl::Writer& w, const FunctionDecl& v) {
  w.write_name("FunctionDecl");
  w.open();
  write(w, *v.function_decl);
  w.close();
}

static FunctionDecl read_body_FunctionDecl(::c6502::asdl::Reader& r) {
  r.open();
  ::c6502::asdl::Box<Type_function_decl> f0 = ::c6502::asdl::Box<Type_function_decl>(read(r, ::c6502::asdl::Tag<Type_function_decl>{}));
  r.close();
  return FunctionDecl{std::move(f0)};
}

FunctionDecl read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<FunctionDecl>) {
  r.expect_name("FunctionDecl");
  return read_body_FunctionDecl(r);
}

void write(::c6502::asdl::Writer& w, const VarDecl& v) {
  w.write_name("VarDecl");
  w.open();
  write(w, v.var_decl);
  w.close();
}

static VarDecl read_body_VarDecl(::c6502::asdl::Reader& r) {
  r.open();
  Type_var_decl f0 = read(r, ::c6502::asdl::Tag<Type_var_decl>{});
  r.close();
  return VarDecl{std::move(f0)};
}

VarDecl read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<VarDecl>) {
  r.expect_name("VarDecl");
  return read_body_VarDecl(r);
}

void write(::c6502::asdl::Writer& w, const StructDecl& v) {
  w.write_name("StructDecl");
  w.open();
  write(w, v.struct_decl);
  w.close();
}

static StructDecl read_body_StructDecl(::c6502::asdl::Reader& r) {
  r.open();
  Type_struct_decl f0 = read(r, ::c6502::asdl::Tag<Type_struct_decl>{});
  r.close();
  return StructDecl{std::move(f0)};
}

StructDecl read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<StructDecl>) {
  r.expect_name("StructDecl");
  return read_body_StructDecl(r);
}

void write(::c6502::asdl::Writer& w, const Type_declaration& v) {
  switch (v.index()) {
    case 0:
      write(w, *std::get_if<0>(&v));
      return;
    case 1:
      write(w, *std::get_if<1>(&v));
      return;
    case 2:
      write(w, *std::get_if<2>(&v));
      return;
  }
}

Type_declaration read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Type_declaration>) {
  const std::string_view name = r.read_name();
  switch (name.size()) {
    case 7:
      if (name == "VarDecl") {
        return read_body_VarDecl(r);
      }
      break;
    case 10:
      if (name == "StructDecl") {
        return read_body_StructDecl(r);
      }
      break;
    case 12:
      if (name == "FunctionDecl") {
        return read_body_FunctionDecl(r);
      }
      break;
  }
  r.unknown_constructor(name, "declaration");
}

void write(::c6502::asdl::Writer& w, const Type_function_decl& v) {
  w.write_name("function_decl");
  w.open();
  w.write_string(v.name);
  w.space();
  w.begin_seq(false);
  for (const auto& e : v.params) {
    w.seq_item(false);
    w.write_string(e);
  }
  w.end_seq(false, v.params.empty());
  w.space();
  if (v.body) {
    write(w, *(*v.body));
  } else {
    w.write_none();
  }
  w.space();
  write(w, v.data_type);
  w.space();
  if (v.storage_class) {
    write(w, (*v.storage_class));
  } else {
    w.write_none();
  }
  w.space();
  if (v.abi_annotation) {
    w.write_string((*v.abi_annotation));
  } else {
    w.write_none();
  }
  w.space();
  w.begin_seq(false);
  for (const auto& e : v.param_registers) {
    w.seq_item(false);
    w.write_string(e);
  }
  w.end_seq(false, v.param_registers.empty());
  w.space();
  w.write_bool(v.carry_return);
  w.space();
  if (v.address) {
    w.write_int((*v.address));
  } else {
    w.write_none();
  }
  w.space();
  w.begin_seq(false);
  for (const auto& e : v.preserves) {
    w.seq_item(false);
    w.write_string(e);
  }
  w.end_seq(false, v.preserves.empty());
  w.space();
  if (v.section) {
    w.write_string((*v.section));
  } else {
    w.write_none();
  }
  w.space();
  w.write_bool(v.noreturn);
  w.close();
}

static Type_function_decl read_body_Type_function_decl(::c6502::asdl::Reader& r) {
  r.open();
  std::string f0 = r.read_string();
  std::vector<std::string> f1;
  r.begin_seq();
  while (!r.end_seq()) {
    f1.push_back(r.read_string());
  }
  std::optional<::c6502::asdl::Box<Type_block>> f2;
  if (!r.read_none()) {
    f2 = ::c6502::asdl::Box<Type_block>(read(r, ::c6502::asdl::Tag<Type_block>{}));
  }
  Type_data_type f3 = read(r, ::c6502::asdl::Tag<Type_data_type>{});
  std::optional<Type_storage_class> f4;
  if (!r.read_none()) {
    f4 = read(r, ::c6502::asdl::Tag<Type_storage_class>{});
  }
  std::optional<std::string> f5;
  if (!r.read_none()) {
    f5 = r.read_string();
  }
  std::vector<std::string> f6;
  r.begin_seq();
  while (!r.end_seq()) {
    f6.push_back(r.read_string());
  }
  bool f7 = r.read_bool();
  std::optional<::c6502::asdl::Int> f8;
  if (!r.read_none()) {
    f8 = r.read_int();
  }
  std::vector<std::string> f9;
  r.begin_seq();
  while (!r.end_seq()) {
    f9.push_back(r.read_string());
  }
  std::optional<std::string> f10;
  if (!r.read_none()) {
    f10 = r.read_string();
  }
  bool f11 = r.read_bool();
  r.close();
  return Type_function_decl{std::move(f0), std::move(f1), std::move(f2), std::move(f3), std::move(f4), std::move(f5), std::move(f6), std::move(f7), std::move(f8), std::move(f9), std::move(f10), std::move(f11)};
}

Type_function_decl read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Type_function_decl>) {
  r.expect_name("function_decl");
  return read_body_Type_function_decl(r);
}

void write(::c6502::asdl::Writer& w, const Program& v) {
  w.write_name("Program");
  w.open();
  w.begin_seq(true);
  for (const auto& e : v.declaration) {
    w.seq_item(true);
    write(w, e);
  }
  w.end_seq(true, v.declaration.empty());
  w.close();
}

static Program read_body_Program(::c6502::asdl::Reader& r) {
  r.open();
  std::vector<Type_declaration> f0;
  r.begin_seq();
  while (!r.end_seq()) {
    f0.push_back(read(r, ::c6502::asdl::Tag<Type_declaration>{}));
  }
  r.close();
  return Program{std::move(f0)};
}

Program read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Program>) {
  r.expect_name("Program");
  return read_body_Program(r);
}

void write(::c6502::asdl::Writer& w, const Type_program& v) {
  switch (v.index()) {
    case 0:
      write(w, *std::get_if<0>(&v));
      return;
  }
}

Type_program read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Type_program>) {
  const std::string_view name = r.read_name();
  switch (name.size()) {
    case 7:
      if (name == "Program") {
        return read_body_Program(r);
      }
      break;
  }
  r.unknown_constructor(name, "program");
}

void write(::c6502::asdl::Writer& w, const Function& v) {
  w.write_name("Function");
  w.open();
  w.write_string(v.name);
  w.space();
  w.begin_seq(false);
  for (const auto& e : v.params) {
    w.seq_item(false);
    w.write_string(e);
  }
  w.end_seq(false, v.params.empty());
  w.space();
  write(w, v.body);
  w.close();
}

static Function read_body_Function(::c6502::asdl::Reader& r) {
  r.open();
  std::string f0 = r.read_string();
  std::vector<std::string> f1;
  r.begin_seq();
  while (!r.end_seq()) {
    f1.push_back(r.read_string());
  }
  Type_block f2 = read(r, ::c6502::asdl::Tag<Type_block>{});
  r.close();
  return Function{std::move(f0), std::move(f1), std::move(f2)};
}

Function read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Function>) {
  r.expect_name("Function");
  return read_body_Function(r);
}

void write(::c6502::asdl::Writer& w, const Type_function_definition& v) {
  switch (v.index()) {
    case 0:
      write(w, *std::get_if<0>(&v));
      return;
  }
}

Type_function_definition read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Type_function_definition>) {
  const std::string_view name = r.read_name();
  switch (name.size()) {
    case 8:
      if (name == "Function") {
        return read_body_Function(r);
      }
      break;
  }
  r.unknown_constructor(name, "function_definition");
}

}  // namespace c6502::c99_ast
