// Generated from tac.asdl by `asdl.py --cpp`. Do not edit.
#include "tac_ast.h"

#include <utility>

#include "pyrepr.h"

namespace c6502::tac_ast {

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

void repr(std::string& out, const ConstChar& v) {
  out += "ConstChar(";
  out += "value=";
  out += ::c6502::asdl::int_to_string(v.value);
  out += ")";
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

void repr(std::string& out, const ConstUChar& v) {
  out += "ConstUChar(";
  out += "value=";
  out += ::c6502::asdl::int_to_string(v.value);
  out += ")";
}

ConstUChar read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<ConstUChar>) {
  r.expect_name("ConstUChar");
  return read_body_ConstUChar(r);
}

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

void repr(std::string& out, const ConstInt& v) {
  out += "ConstInt(";
  out += "value=";
  out += ::c6502::asdl::int_to_string(v.value);
  out += ")";
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

void repr(std::string& out, const ConstLong& v) {
  out += "ConstLong(";
  out += "value=";
  out += ::c6502::asdl::int_to_string(v.value);
  out += ")";
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

void repr(std::string& out, const ConstLongLong& v) {
  out += "ConstLongLong(";
  out += "value=";
  out += ::c6502::asdl::int_to_string(v.value);
  out += ")";
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

void repr(std::string& out, const ConstUInt& v) {
  out += "ConstUInt(";
  out += "value=";
  out += ::c6502::asdl::int_to_string(v.value);
  out += ")";
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

void repr(std::string& out, const ConstULong& v) {
  out += "ConstULong(";
  out += "value=";
  out += ::c6502::asdl::int_to_string(v.value);
  out += ")";
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

void repr(std::string& out, const ConstULongLong& v) {
  out += "ConstULongLong(";
  out += "value=";
  out += ::c6502::asdl::int_to_string(v.value);
  out += ")";
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

void repr(std::string& out, const ConstFloat& v) {
  out += "ConstFloat(";
  out += "bits=";
  out += ::c6502::asdl::int_to_string(v.bits);
  out += ")";
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

void repr(std::string& out, const ConstDouble& v) {
  out += "ConstDouble(";
  out += "bits=";
  out += ::c6502::asdl::int_to_string(v.bits);
  out += ")";
}

ConstDouble read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<ConstDouble>) {
  r.expect_name("ConstDouble");
  return read_body_ConstDouble(r);
}

void write(::c6502::asdl::Writer& w, const ConstInt24& v) {
  w.write_name("ConstInt24");
  w.open();
  w.write_int(v.value);
  w.close();
}

static ConstInt24 read_body_ConstInt24(::c6502::asdl::Reader& r) {
  r.open();
  ::c6502::asdl::Int f0 = r.read_int();
  r.close();
  return ConstInt24{std::move(f0)};
}

void repr(std::string& out, const ConstInt24& v) {
  out += "ConstInt24(";
  out += "value=";
  out += ::c6502::asdl::int_to_string(v.value);
  out += ")";
}

ConstInt24 read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<ConstInt24>) {
  r.expect_name("ConstInt24");
  return read_body_ConstInt24(r);
}

void write(::c6502::asdl::Writer& w, const ConstUInt24& v) {
  w.write_name("ConstUInt24");
  w.open();
  w.write_int(v.value);
  w.close();
}

static ConstUInt24 read_body_ConstUInt24(::c6502::asdl::Reader& r) {
  r.open();
  ::c6502::asdl::Int f0 = r.read_int();
  r.close();
  return ConstUInt24{std::move(f0)};
}

void repr(std::string& out, const ConstUInt24& v) {
  out += "ConstUInt24(";
  out += "value=";
  out += ::c6502::asdl::int_to_string(v.value);
  out += ")";
}

ConstUInt24 read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<ConstUInt24>) {
  r.expect_name("ConstUInt24");
  return read_body_ConstUInt24(r);
}

void write(::c6502::asdl::Writer& w, const ConstInt40& v) {
  w.write_name("ConstInt40");
  w.open();
  w.write_int(v.value);
  w.close();
}

static ConstInt40 read_body_ConstInt40(::c6502::asdl::Reader& r) {
  r.open();
  ::c6502::asdl::Int f0 = r.read_int();
  r.close();
  return ConstInt40{std::move(f0)};
}

void repr(std::string& out, const ConstInt40& v) {
  out += "ConstInt40(";
  out += "value=";
  out += ::c6502::asdl::int_to_string(v.value);
  out += ")";
}

ConstInt40 read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<ConstInt40>) {
  r.expect_name("ConstInt40");
  return read_body_ConstInt40(r);
}

void write(::c6502::asdl::Writer& w, const ConstUInt40& v) {
  w.write_name("ConstUInt40");
  w.open();
  w.write_int(v.value);
  w.close();
}

static ConstUInt40 read_body_ConstUInt40(::c6502::asdl::Reader& r) {
  r.open();
  ::c6502::asdl::Int f0 = r.read_int();
  r.close();
  return ConstUInt40{std::move(f0)};
}

void repr(std::string& out, const ConstUInt40& v) {
  out += "ConstUInt40(";
  out += "value=";
  out += ::c6502::asdl::int_to_string(v.value);
  out += ")";
}

ConstUInt40 read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<ConstUInt40>) {
  r.expect_name("ConstUInt40");
  return read_body_ConstUInt40(r);
}

void write(::c6502::asdl::Writer& w, const ConstInt48& v) {
  w.write_name("ConstInt48");
  w.open();
  w.write_int(v.value);
  w.close();
}

static ConstInt48 read_body_ConstInt48(::c6502::asdl::Reader& r) {
  r.open();
  ::c6502::asdl::Int f0 = r.read_int();
  r.close();
  return ConstInt48{std::move(f0)};
}

void repr(std::string& out, const ConstInt48& v) {
  out += "ConstInt48(";
  out += "value=";
  out += ::c6502::asdl::int_to_string(v.value);
  out += ")";
}

ConstInt48 read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<ConstInt48>) {
  r.expect_name("ConstInt48");
  return read_body_ConstInt48(r);
}

void write(::c6502::asdl::Writer& w, const ConstUInt48& v) {
  w.write_name("ConstUInt48");
  w.open();
  w.write_int(v.value);
  w.close();
}

static ConstUInt48 read_body_ConstUInt48(::c6502::asdl::Reader& r) {
  r.open();
  ::c6502::asdl::Int f0 = r.read_int();
  r.close();
  return ConstUInt48{std::move(f0)};
}

void repr(std::string& out, const ConstUInt48& v) {
  out += "ConstUInt48(";
  out += "value=";
  out += ::c6502::asdl::int_to_string(v.value);
  out += ")";
}

ConstUInt48 read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<ConstUInt48>) {
  r.expect_name("ConstUInt48");
  return read_body_ConstUInt48(r);
}

void write(::c6502::asdl::Writer& w, const ConstInt56& v) {
  w.write_name("ConstInt56");
  w.open();
  w.write_int(v.value);
  w.close();
}

static ConstInt56 read_body_ConstInt56(::c6502::asdl::Reader& r) {
  r.open();
  ::c6502::asdl::Int f0 = r.read_int();
  r.close();
  return ConstInt56{std::move(f0)};
}

void repr(std::string& out, const ConstInt56& v) {
  out += "ConstInt56(";
  out += "value=";
  out += ::c6502::asdl::int_to_string(v.value);
  out += ")";
}

ConstInt56 read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<ConstInt56>) {
  r.expect_name("ConstInt56");
  return read_body_ConstInt56(r);
}

void write(::c6502::asdl::Writer& w, const ConstUInt56& v) {
  w.write_name("ConstUInt56");
  w.open();
  w.write_int(v.value);
  w.close();
}

static ConstUInt56 read_body_ConstUInt56(::c6502::asdl::Reader& r) {
  r.open();
  ::c6502::asdl::Int f0 = r.read_int();
  r.close();
  return ConstUInt56{std::move(f0)};
}

void repr(std::string& out, const ConstUInt56& v) {
  out += "ConstUInt56(";
  out += "value=";
  out += ::c6502::asdl::int_to_string(v.value);
  out += ")";
}

ConstUInt56 read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<ConstUInt56>) {
  r.expect_name("ConstUInt56");
  return read_body_ConstUInt56(r);
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

void repr(std::string& out, const Type_const& v) {
  switch (v.index()) {
    case 0:
      repr(out, *std::get_if<0>(&v));
      return;
    case 1:
      repr(out, *std::get_if<1>(&v));
      return;
    case 2:
      repr(out, *std::get_if<2>(&v));
      return;
    case 3:
      repr(out, *std::get_if<3>(&v));
      return;
    case 4:
      repr(out, *std::get_if<4>(&v));
      return;
    case 5:
      repr(out, *std::get_if<5>(&v));
      return;
    case 6:
      repr(out, *std::get_if<6>(&v));
      return;
    case 7:
      repr(out, *std::get_if<7>(&v));
      return;
    case 8:
      repr(out, *std::get_if<8>(&v));
      return;
    case 9:
      repr(out, *std::get_if<9>(&v));
      return;
    case 10:
      repr(out, *std::get_if<10>(&v));
      return;
    case 11:
      repr(out, *std::get_if<11>(&v));
      return;
    case 12:
      repr(out, *std::get_if<12>(&v));
      return;
    case 13:
      repr(out, *std::get_if<13>(&v));
      return;
    case 14:
      repr(out, *std::get_if<14>(&v));
      return;
    case 15:
      repr(out, *std::get_if<15>(&v));
      return;
    case 16:
      repr(out, *std::get_if<16>(&v));
      return;
    case 17:
      repr(out, *std::get_if<17>(&v));
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
      if (name == "ConstChar") {
        return read_body_ConstChar(r);
      }
      if (name == "ConstLong") {
        return read_body_ConstLong(r);
      }
      if (name == "ConstUInt") {
        return read_body_ConstUInt(r);
      }
      break;
    case 10:
      if (name == "ConstUChar") {
        return read_body_ConstUChar(r);
      }
      if (name == "ConstULong") {
        return read_body_ConstULong(r);
      }
      if (name == "ConstFloat") {
        return read_body_ConstFloat(r);
      }
      if (name == "ConstInt24") {
        return read_body_ConstInt24(r);
      }
      if (name == "ConstInt40") {
        return read_body_ConstInt40(r);
      }
      if (name == "ConstInt48") {
        return read_body_ConstInt48(r);
      }
      if (name == "ConstInt56") {
        return read_body_ConstInt56(r);
      }
      break;
    case 11:
      if (name == "ConstDouble") {
        return read_body_ConstDouble(r);
      }
      if (name == "ConstUInt24") {
        return read_body_ConstUInt24(r);
      }
      if (name == "ConstUInt40") {
        return read_body_ConstUInt40(r);
      }
      if (name == "ConstUInt48") {
        return read_body_ConstUInt48(r);
      }
      if (name == "ConstUInt56") {
        return read_body_ConstUInt56(r);
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

void write(::c6502::asdl::Writer& w, const Constant& v) {
  w.write_name("Constant");
  w.open();
  write(w, v.const_);
  w.close();
}

static Constant read_body_Constant(::c6502::asdl::Reader& r) {
  r.open();
  Type_const f0 = read(r, ::c6502::asdl::Tag<Type_const>{});
  r.close();
  return Constant{std::move(f0)};
}

void repr(std::string& out, const Constant& v) {
  out += "Constant(";
  out += "const=";
  repr(out, v.const_);
  out += ")";
}

Constant read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Constant>) {
  r.expect_name("Constant");
  return read_body_Constant(r);
}

void write(::c6502::asdl::Writer& w, const Var& v) {
  w.write_name("Var");
  w.open();
  w.write_string(v.name);
  w.close();
}

static Var read_body_Var(::c6502::asdl::Reader& r) {
  r.open();
  std::string f0 = r.read_string();
  r.close();
  return Var{std::move(f0)};
}

void repr(std::string& out, const Var& v) {
  out += "Var(";
  out += "name=";
  out += ::c6502::pyrepr::string(v.name);
  out += ")";
}

Var read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Var>) {
  r.expect_name("Var");
  return read_body_Var(r);
}

void write(::c6502::asdl::Writer& w, const Type_val& v) {
  switch (v.index()) {
    case 0:
      write(w, *std::get_if<0>(&v));
      return;
    case 1:
      write(w, *std::get_if<1>(&v));
      return;
  }
}

void repr(std::string& out, const Type_val& v) {
  switch (v.index()) {
    case 0:
      repr(out, *std::get_if<0>(&v));
      return;
    case 1:
      repr(out, *std::get_if<1>(&v));
      return;
  }
}

Type_val read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Type_val>) {
  const std::string_view name = r.read_name();
  switch (name.size()) {
    case 3:
      if (name == "Var") {
        return read_body_Var(r);
      }
      break;
    case 8:
      if (name == "Constant") {
        return read_body_Constant(r);
      }
      break;
  }
  r.unknown_constructor(name, "val");
}

void write(::c6502::asdl::Writer& w, const Complement&) {
  w.write_name("Complement");
}

static Complement read_body_Complement(::c6502::asdl::Reader& r) {
  (void)r;
  return Complement{};
}

void repr(std::string& out, const Complement&) {
  out += "Complement(";
  out += ")";
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

void repr(std::string& out, const Negate&) {
  out += "Negate(";
  out += ")";
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

void repr(std::string& out, const LogicalNot&) {
  out += "LogicalNot(";
  out += ")";
}

LogicalNot read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<LogicalNot>) {
  r.expect_name("LogicalNot");
  return read_body_LogicalNot(r);
}

void write(::c6502::asdl::Writer& w, const BinToBcd&) {
  w.write_name("BinToBcd");
}

static BinToBcd read_body_BinToBcd(::c6502::asdl::Reader& r) {
  (void)r;
  return BinToBcd{};
}

void repr(std::string& out, const BinToBcd&) {
  out += "BinToBcd(";
  out += ")";
}

BinToBcd read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<BinToBcd>) {
  r.expect_name("BinToBcd");
  return read_body_BinToBcd(r);
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

void repr(std::string& out, const Type_unary_operator& v) {
  switch (v.index()) {
    case 0:
      repr(out, *std::get_if<0>(&v));
      return;
    case 1:
      repr(out, *std::get_if<1>(&v));
      return;
    case 2:
      repr(out, *std::get_if<2>(&v));
      return;
    case 3:
      repr(out, *std::get_if<3>(&v));
      return;
  }
}

Type_unary_operator read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Type_unary_operator>) {
  const std::string_view name = r.read_name();
  switch (name.size()) {
    case 6:
      if (name == "Negate") {
        return read_body_Negate(r);
      }
      break;
    case 8:
      if (name == "BinToBcd") {
        return read_body_BinToBcd(r);
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

void repr(std::string& out, const Add&) {
  out += "Add(";
  out += ")";
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

void repr(std::string& out, const Subtract&) {
  out += "Subtract(";
  out += ")";
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

void repr(std::string& out, const Multiply&) {
  out += "Multiply(";
  out += ")";
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

void repr(std::string& out, const Divide&) {
  out += "Divide(";
  out += ")";
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

void repr(std::string& out, const Modulo&) {
  out += "Modulo(";
  out += ")";
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

void repr(std::string& out, const BitwiseAnd&) {
  out += "BitwiseAnd(";
  out += ")";
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

void repr(std::string& out, const BitwiseOr&) {
  out += "BitwiseOr(";
  out += ")";
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

void repr(std::string& out, const BitwiseXor&) {
  out += "BitwiseXor(";
  out += ")";
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

void repr(std::string& out, const LeftShift&) {
  out += "LeftShift(";
  out += ")";
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

void repr(std::string& out, const RightShift&) {
  out += "RightShift(";
  out += ")";
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

void repr(std::string& out, const Equal&) {
  out += "Equal(";
  out += ")";
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

void repr(std::string& out, const NotEqual&) {
  out += "NotEqual(";
  out += ")";
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

void repr(std::string& out, const LessThan&) {
  out += "LessThan(";
  out += ")";
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

void repr(std::string& out, const GreaterThan&) {
  out += "GreaterThan(";
  out += ")";
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

void repr(std::string& out, const LessOrEqual&) {
  out += "LessOrEqual(";
  out += ")";
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

void repr(std::string& out, const GreaterOrEqual&) {
  out += "GreaterOrEqual(";
  out += ")";
}

GreaterOrEqual read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<GreaterOrEqual>) {
  r.expect_name("GreaterOrEqual");
  return read_body_GreaterOrEqual(r);
}

void write(::c6502::asdl::Writer& w, const BcdAdd&) {
  w.write_name("BcdAdd");
}

static BcdAdd read_body_BcdAdd(::c6502::asdl::Reader& r) {
  (void)r;
  return BcdAdd{};
}

void repr(std::string& out, const BcdAdd&) {
  out += "BcdAdd(";
  out += ")";
}

BcdAdd read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<BcdAdd>) {
  r.expect_name("BcdAdd");
  return read_body_BcdAdd(r);
}

void write(::c6502::asdl::Writer& w, const BcdSubtract&) {
  w.write_name("BcdSubtract");
}

static BcdSubtract read_body_BcdSubtract(::c6502::asdl::Reader& r) {
  (void)r;
  return BcdSubtract{};
}

void repr(std::string& out, const BcdSubtract&) {
  out += "BcdSubtract(";
  out += ")";
}

BcdSubtract read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<BcdSubtract>) {
  r.expect_name("BcdSubtract");
  return read_body_BcdSubtract(r);
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

void repr(std::string& out, const Type_binary_operator& v) {
  switch (v.index()) {
    case 0:
      repr(out, *std::get_if<0>(&v));
      return;
    case 1:
      repr(out, *std::get_if<1>(&v));
      return;
    case 2:
      repr(out, *std::get_if<2>(&v));
      return;
    case 3:
      repr(out, *std::get_if<3>(&v));
      return;
    case 4:
      repr(out, *std::get_if<4>(&v));
      return;
    case 5:
      repr(out, *std::get_if<5>(&v));
      return;
    case 6:
      repr(out, *std::get_if<6>(&v));
      return;
    case 7:
      repr(out, *std::get_if<7>(&v));
      return;
    case 8:
      repr(out, *std::get_if<8>(&v));
      return;
    case 9:
      repr(out, *std::get_if<9>(&v));
      return;
    case 10:
      repr(out, *std::get_if<10>(&v));
      return;
    case 11:
      repr(out, *std::get_if<11>(&v));
      return;
    case 12:
      repr(out, *std::get_if<12>(&v));
      return;
    case 13:
      repr(out, *std::get_if<13>(&v));
      return;
    case 14:
      repr(out, *std::get_if<14>(&v));
      return;
    case 15:
      repr(out, *std::get_if<15>(&v));
      return;
    case 16:
      repr(out, *std::get_if<16>(&v));
      return;
    case 17:
      repr(out, *std::get_if<17>(&v));
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
      if (name == "BcdAdd") {
        return read_body_BcdAdd(r);
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
      break;
    case 11:
      if (name == "GreaterThan") {
        return read_body_GreaterThan(r);
      }
      if (name == "LessOrEqual") {
        return read_body_LessOrEqual(r);
      }
      if (name == "BcdSubtract") {
        return read_body_BcdSubtract(r);
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

void write(::c6502::asdl::Writer& w, const PhiArg& v) {
  w.write_name("PhiArg");
  w.open();
  w.write_string(v.pred_label);
  w.space();
  write(w, v.source);
  w.close();
}

static PhiArg read_body_PhiArg(::c6502::asdl::Reader& r) {
  r.open();
  std::string f0 = r.read_string();
  Type_val f1 = read(r, ::c6502::asdl::Tag<Type_val>{});
  r.close();
  return PhiArg{std::move(f0), std::move(f1)};
}

void repr(std::string& out, const PhiArg& v) {
  out += "PhiArg(";
  out += "pred_label=";
  out += ::c6502::pyrepr::string(v.pred_label);
  out += ", source=";
  repr(out, v.source);
  out += ")";
}

PhiArg read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<PhiArg>) {
  r.expect_name("PhiArg");
  return read_body_PhiArg(r);
}

void write(::c6502::asdl::Writer& w, const Type_phi_arg& v) {
  switch (v.index()) {
    case 0:
      write(w, *std::get_if<0>(&v));
      return;
  }
}

void repr(std::string& out, const Type_phi_arg& v) {
  switch (v.index()) {
    case 0:
      repr(out, *std::get_if<0>(&v));
      return;
  }
}

Type_phi_arg read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Type_phi_arg>) {
  const std::string_view name = r.read_name();
  switch (name.size()) {
    case 6:
      if (name == "PhiArg") {
        return read_body_PhiArg(r);
      }
      break;
  }
  r.unknown_constructor(name, "phi_arg");
}

void write(::c6502::asdl::Writer& w, const Ret& v) {
  w.write_name("Ret");
  w.open();
  if (v.val) {
    write(w, (*v.val));
  } else {
    w.write_none();
  }
  w.close();
}

static Ret read_body_Ret(::c6502::asdl::Reader& r) {
  r.open();
  std::optional<Type_val> f0;
  if (!r.read_none()) {
    f0 = read(r, ::c6502::asdl::Tag<Type_val>{});
  }
  r.close();
  return Ret{std::move(f0)};
}

void repr(std::string& out, const Ret& v) {
  out += "Ret(";
  out += "val=";
  if (v.val) {
    repr(out, (*v.val));
  } else {
    out += "None";
  }
  out += ")";
}

Ret read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Ret>) {
  r.expect_name("Ret");
  return read_body_Ret(r);
}

void write(::c6502::asdl::Writer& w, const SignExtend& v) {
  w.write_name("SignExtend");
  w.open();
  write(w, v.src);
  w.space();
  write(w, v.dst);
  w.close();
}

static SignExtend read_body_SignExtend(::c6502::asdl::Reader& r) {
  r.open();
  Type_val f0 = read(r, ::c6502::asdl::Tag<Type_val>{});
  Type_val f1 = read(r, ::c6502::asdl::Tag<Type_val>{});
  r.close();
  return SignExtend{std::move(f0), std::move(f1)};
}

void repr(std::string& out, const SignExtend& v) {
  out += "SignExtend(";
  out += "src=";
  repr(out, v.src);
  out += ", dst=";
  repr(out, v.dst);
  out += ")";
}

SignExtend read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<SignExtend>) {
  r.expect_name("SignExtend");
  return read_body_SignExtend(r);
}

void write(::c6502::asdl::Writer& w, const ZeroExtend& v) {
  w.write_name("ZeroExtend");
  w.open();
  write(w, v.src);
  w.space();
  write(w, v.dst);
  w.close();
}

static ZeroExtend read_body_ZeroExtend(::c6502::asdl::Reader& r) {
  r.open();
  Type_val f0 = read(r, ::c6502::asdl::Tag<Type_val>{});
  Type_val f1 = read(r, ::c6502::asdl::Tag<Type_val>{});
  r.close();
  return ZeroExtend{std::move(f0), std::move(f1)};
}

void repr(std::string& out, const ZeroExtend& v) {
  out += "ZeroExtend(";
  out += "src=";
  repr(out, v.src);
  out += ", dst=";
  repr(out, v.dst);
  out += ")";
}

ZeroExtend read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<ZeroExtend>) {
  r.expect_name("ZeroExtend");
  return read_body_ZeroExtend(r);
}

void write(::c6502::asdl::Writer& w, const Truncate& v) {
  w.write_name("Truncate");
  w.open();
  write(w, v.src);
  w.space();
  write(w, v.dst);
  w.close();
}

static Truncate read_body_Truncate(::c6502::asdl::Reader& r) {
  r.open();
  Type_val f0 = read(r, ::c6502::asdl::Tag<Type_val>{});
  Type_val f1 = read(r, ::c6502::asdl::Tag<Type_val>{});
  r.close();
  return Truncate{std::move(f0), std::move(f1)};
}

void repr(std::string& out, const Truncate& v) {
  out += "Truncate(";
  out += "src=";
  repr(out, v.src);
  out += ", dst=";
  repr(out, v.dst);
  out += ")";
}

Truncate read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Truncate>) {
  r.expect_name("Truncate");
  return read_body_Truncate(r);
}

void write(::c6502::asdl::Writer& w, const IntToFloat& v) {
  w.write_name("IntToFloat");
  w.open();
  write(w, v.src);
  w.space();
  write(w, v.dst);
  w.close();
}

static IntToFloat read_body_IntToFloat(::c6502::asdl::Reader& r) {
  r.open();
  Type_val f0 = read(r, ::c6502::asdl::Tag<Type_val>{});
  Type_val f1 = read(r, ::c6502::asdl::Tag<Type_val>{});
  r.close();
  return IntToFloat{std::move(f0), std::move(f1)};
}

void repr(std::string& out, const IntToFloat& v) {
  out += "IntToFloat(";
  out += "src=";
  repr(out, v.src);
  out += ", dst=";
  repr(out, v.dst);
  out += ")";
}

IntToFloat read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<IntToFloat>) {
  r.expect_name("IntToFloat");
  return read_body_IntToFloat(r);
}

void write(::c6502::asdl::Writer& w, const IntToDouble& v) {
  w.write_name("IntToDouble");
  w.open();
  write(w, v.src);
  w.space();
  write(w, v.dst);
  w.close();
}

static IntToDouble read_body_IntToDouble(::c6502::asdl::Reader& r) {
  r.open();
  Type_val f0 = read(r, ::c6502::asdl::Tag<Type_val>{});
  Type_val f1 = read(r, ::c6502::asdl::Tag<Type_val>{});
  r.close();
  return IntToDouble{std::move(f0), std::move(f1)};
}

void repr(std::string& out, const IntToDouble& v) {
  out += "IntToDouble(";
  out += "src=";
  repr(out, v.src);
  out += ", dst=";
  repr(out, v.dst);
  out += ")";
}

IntToDouble read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<IntToDouble>) {
  r.expect_name("IntToDouble");
  return read_body_IntToDouble(r);
}

void write(::c6502::asdl::Writer& w, const FloatToInt& v) {
  w.write_name("FloatToInt");
  w.open();
  write(w, v.src);
  w.space();
  write(w, v.dst);
  w.close();
}

static FloatToInt read_body_FloatToInt(::c6502::asdl::Reader& r) {
  r.open();
  Type_val f0 = read(r, ::c6502::asdl::Tag<Type_val>{});
  Type_val f1 = read(r, ::c6502::asdl::Tag<Type_val>{});
  r.close();
  return FloatToInt{std::move(f0), std::move(f1)};
}

void repr(std::string& out, const FloatToInt& v) {
  out += "FloatToInt(";
  out += "src=";
  repr(out, v.src);
  out += ", dst=";
  repr(out, v.dst);
  out += ")";
}

FloatToInt read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<FloatToInt>) {
  r.expect_name("FloatToInt");
  return read_body_FloatToInt(r);
}

void write(::c6502::asdl::Writer& w, const DoubleToInt& v) {
  w.write_name("DoubleToInt");
  w.open();
  write(w, v.src);
  w.space();
  write(w, v.dst);
  w.close();
}

static DoubleToInt read_body_DoubleToInt(::c6502::asdl::Reader& r) {
  r.open();
  Type_val f0 = read(r, ::c6502::asdl::Tag<Type_val>{});
  Type_val f1 = read(r, ::c6502::asdl::Tag<Type_val>{});
  r.close();
  return DoubleToInt{std::move(f0), std::move(f1)};
}

void repr(std::string& out, const DoubleToInt& v) {
  out += "DoubleToInt(";
  out += "src=";
  repr(out, v.src);
  out += ", dst=";
  repr(out, v.dst);
  out += ")";
}

DoubleToInt read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<DoubleToInt>) {
  r.expect_name("DoubleToInt");
  return read_body_DoubleToInt(r);
}

void write(::c6502::asdl::Writer& w, const FloatToDouble& v) {
  w.write_name("FloatToDouble");
  w.open();
  write(w, v.src);
  w.space();
  write(w, v.dst);
  w.close();
}

static FloatToDouble read_body_FloatToDouble(::c6502::asdl::Reader& r) {
  r.open();
  Type_val f0 = read(r, ::c6502::asdl::Tag<Type_val>{});
  Type_val f1 = read(r, ::c6502::asdl::Tag<Type_val>{});
  r.close();
  return FloatToDouble{std::move(f0), std::move(f1)};
}

void repr(std::string& out, const FloatToDouble& v) {
  out += "FloatToDouble(";
  out += "src=";
  repr(out, v.src);
  out += ", dst=";
  repr(out, v.dst);
  out += ")";
}

FloatToDouble read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<FloatToDouble>) {
  r.expect_name("FloatToDouble");
  return read_body_FloatToDouble(r);
}

void write(::c6502::asdl::Writer& w, const DoubleToFloat& v) {
  w.write_name("DoubleToFloat");
  w.open();
  write(w, v.src);
  w.space();
  write(w, v.dst);
  w.close();
}

static DoubleToFloat read_body_DoubleToFloat(::c6502::asdl::Reader& r) {
  r.open();
  Type_val f0 = read(r, ::c6502::asdl::Tag<Type_val>{});
  Type_val f1 = read(r, ::c6502::asdl::Tag<Type_val>{});
  r.close();
  return DoubleToFloat{std::move(f0), std::move(f1)};
}

void repr(std::string& out, const DoubleToFloat& v) {
  out += "DoubleToFloat(";
  out += "src=";
  repr(out, v.src);
  out += ", dst=";
  repr(out, v.dst);
  out += ")";
}

DoubleToFloat read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<DoubleToFloat>) {
  r.expect_name("DoubleToFloat");
  return read_body_DoubleToFloat(r);
}

void write(::c6502::asdl::Writer& w, const GetAddress& v) {
  w.write_name("GetAddress");
  w.open();
  write(w, v.operand);
  w.space();
  write(w, v.dst);
  w.close();
}

static GetAddress read_body_GetAddress(::c6502::asdl::Reader& r) {
  r.open();
  Type_val f0 = read(r, ::c6502::asdl::Tag<Type_val>{});
  Type_val f1 = read(r, ::c6502::asdl::Tag<Type_val>{});
  r.close();
  return GetAddress{std::move(f0), std::move(f1)};
}

void repr(std::string& out, const GetAddress& v) {
  out += "GetAddress(";
  out += "operand=";
  repr(out, v.operand);
  out += ", dst=";
  repr(out, v.dst);
  out += ")";
}

GetAddress read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<GetAddress>) {
  r.expect_name("GetAddress");
  return read_body_GetAddress(r);
}

void write(::c6502::asdl::Writer& w, const Load& v) {
  w.write_name("Load");
  w.open();
  write(w, v.src_ptr);
  w.space();
  write(w, v.dst);
  w.space();
  w.write_bool(v.is_volatile);
  w.close();
}

static Load read_body_Load(::c6502::asdl::Reader& r) {
  r.open();
  Type_val f0 = read(r, ::c6502::asdl::Tag<Type_val>{});
  Type_val f1 = read(r, ::c6502::asdl::Tag<Type_val>{});
  bool f2 = r.read_bool();
  r.close();
  return Load{std::move(f0), std::move(f1), std::move(f2)};
}

void repr(std::string& out, const Load& v) {
  out += "Load(";
  out += "src_ptr=";
  repr(out, v.src_ptr);
  out += ", dst=";
  repr(out, v.dst);
  out += ", is_volatile=";
  out += v.is_volatile ? "True" : "False";
  out += ")";
}

Load read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Load>) {
  r.expect_name("Load");
  return read_body_Load(r);
}

void write(::c6502::asdl::Writer& w, const Store& v) {
  w.write_name("Store");
  w.open();
  write(w, v.src);
  w.space();
  write(w, v.dst_ptr);
  w.space();
  w.write_bool(v.is_volatile);
  w.close();
}

static Store read_body_Store(::c6502::asdl::Reader& r) {
  r.open();
  Type_val f0 = read(r, ::c6502::asdl::Tag<Type_val>{});
  Type_val f1 = read(r, ::c6502::asdl::Tag<Type_val>{});
  bool f2 = r.read_bool();
  r.close();
  return Store{std::move(f0), std::move(f1), std::move(f2)};
}

void repr(std::string& out, const Store& v) {
  out += "Store(";
  out += "src=";
  repr(out, v.src);
  out += ", dst_ptr=";
  repr(out, v.dst_ptr);
  out += ", is_volatile=";
  out += v.is_volatile ? "True" : "False";
  out += ")";
}

Store read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Store>) {
  r.expect_name("Store");
  return read_body_Store(r);
}

void write(::c6502::asdl::Writer& w, const IndexedLoad& v) {
  w.write_name("IndexedLoad");
  w.open();
  w.write_string(v.name);
  w.space();
  write(w, v.index);
  w.space();
  write(w, v.dst);
  w.space();
  w.write_bool(v.is_volatile);
  w.space();
  w.write_int(v.offset);
  w.space();
  w.write_int(v.plane);
  w.close();
}

static IndexedLoad read_body_IndexedLoad(::c6502::asdl::Reader& r) {
  r.open();
  std::string f0 = r.read_string();
  Type_val f1 = read(r, ::c6502::asdl::Tag<Type_val>{});
  Type_val f2 = read(r, ::c6502::asdl::Tag<Type_val>{});
  bool f3 = r.read_bool();
  ::c6502::asdl::Int f4 = r.read_int();
  ::c6502::asdl::Int f5 = r.read_int();
  r.close();
  return IndexedLoad{std::move(f0), std::move(f1), std::move(f2), std::move(f3), std::move(f4), std::move(f5)};
}

void repr(std::string& out, const IndexedLoad& v) {
  out += "IndexedLoad(";
  out += "name=";
  out += ::c6502::pyrepr::string(v.name);
  out += ", index=";
  repr(out, v.index);
  out += ", dst=";
  repr(out, v.dst);
  out += ", is_volatile=";
  out += v.is_volatile ? "True" : "False";
  out += ", offset=";
  out += ::c6502::asdl::int_to_string(v.offset);
  out += ", plane=";
  out += ::c6502::asdl::int_to_string(v.plane);
  out += ")";
}

IndexedLoad read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<IndexedLoad>) {
  r.expect_name("IndexedLoad");
  return read_body_IndexedLoad(r);
}

void write(::c6502::asdl::Writer& w, const IndexedStore& v) {
  w.write_name("IndexedStore");
  w.open();
  w.write_int(v.address);
  w.space();
  write(w, v.index);
  w.space();
  write(w, v.src);
  w.space();
  w.write_bool(v.is_volatile);
  w.close();
}

static IndexedStore read_body_IndexedStore(::c6502::asdl::Reader& r) {
  r.open();
  ::c6502::asdl::Int f0 = r.read_int();
  Type_val f1 = read(r, ::c6502::asdl::Tag<Type_val>{});
  Type_val f2 = read(r, ::c6502::asdl::Tag<Type_val>{});
  bool f3 = r.read_bool();
  r.close();
  return IndexedStore{std::move(f0), std::move(f1), std::move(f2), std::move(f3)};
}

void repr(std::string& out, const IndexedStore& v) {
  out += "IndexedStore(";
  out += "address=";
  out += ::c6502::asdl::int_to_string(v.address);
  out += ", index=";
  repr(out, v.index);
  out += ", src=";
  repr(out, v.src);
  out += ", is_volatile=";
  out += v.is_volatile ? "True" : "False";
  out += ")";
}

IndexedStore read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<IndexedStore>) {
  r.expect_name("IndexedStore");
  return read_body_IndexedStore(r);
}

void write(::c6502::asdl::Writer& w, const IndexedSymbolStore& v) {
  w.write_name("IndexedSymbolStore");
  w.open();
  w.write_string(v.name);
  w.space();
  write(w, v.index);
  w.space();
  write(w, v.src);
  w.space();
  w.write_bool(v.is_volatile);
  w.space();
  w.write_int(v.offset);
  w.close();
}

static IndexedSymbolStore read_body_IndexedSymbolStore(::c6502::asdl::Reader& r) {
  r.open();
  std::string f0 = r.read_string();
  Type_val f1 = read(r, ::c6502::asdl::Tag<Type_val>{});
  Type_val f2 = read(r, ::c6502::asdl::Tag<Type_val>{});
  bool f3 = r.read_bool();
  ::c6502::asdl::Int f4 = r.read_int();
  r.close();
  return IndexedSymbolStore{std::move(f0), std::move(f1), std::move(f2), std::move(f3), std::move(f4)};
}

void repr(std::string& out, const IndexedSymbolStore& v) {
  out += "IndexedSymbolStore(";
  out += "name=";
  out += ::c6502::pyrepr::string(v.name);
  out += ", index=";
  repr(out, v.index);
  out += ", src=";
  repr(out, v.src);
  out += ", is_volatile=";
  out += v.is_volatile ? "True" : "False";
  out += ", offset=";
  out += ::c6502::asdl::int_to_string(v.offset);
  out += ")";
}

IndexedSymbolStore read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<IndexedSymbolStore>) {
  r.expect_name("IndexedSymbolStore");
  return read_body_IndexedSymbolStore(r);
}

void write(::c6502::asdl::Writer& w, const IndexedConstLoad& v) {
  w.write_name("IndexedConstLoad");
  w.open();
  w.write_int(v.address);
  w.space();
  write(w, v.index);
  w.space();
  write(w, v.dst);
  w.space();
  w.write_bool(v.is_volatile);
  w.close();
}

static IndexedConstLoad read_body_IndexedConstLoad(::c6502::asdl::Reader& r) {
  r.open();
  ::c6502::asdl::Int f0 = r.read_int();
  Type_val f1 = read(r, ::c6502::asdl::Tag<Type_val>{});
  Type_val f2 = read(r, ::c6502::asdl::Tag<Type_val>{});
  bool f3 = r.read_bool();
  r.close();
  return IndexedConstLoad{std::move(f0), std::move(f1), std::move(f2), std::move(f3)};
}

void repr(std::string& out, const IndexedConstLoad& v) {
  out += "IndexedConstLoad(";
  out += "address=";
  out += ::c6502::asdl::int_to_string(v.address);
  out += ", index=";
  repr(out, v.index);
  out += ", dst=";
  repr(out, v.dst);
  out += ", is_volatile=";
  out += v.is_volatile ? "True" : "False";
  out += ")";
}

IndexedConstLoad read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<IndexedConstLoad>) {
  r.expect_name("IndexedConstLoad");
  return read_body_IndexedConstLoad(r);
}

void write(::c6502::asdl::Writer& w, const IndirectIndexedLoad& v) {
  w.write_name("IndirectIndexedLoad");
  w.open();
  write(w, v.ptr);
  w.space();
  write(w, v.index);
  w.space();
  write(w, v.dst);
  w.space();
  w.write_bool(v.is_volatile);
  w.close();
}

static IndirectIndexedLoad read_body_IndirectIndexedLoad(::c6502::asdl::Reader& r) {
  r.open();
  Type_val f0 = read(r, ::c6502::asdl::Tag<Type_val>{});
  Type_val f1 = read(r, ::c6502::asdl::Tag<Type_val>{});
  Type_val f2 = read(r, ::c6502::asdl::Tag<Type_val>{});
  bool f3 = r.read_bool();
  r.close();
  return IndirectIndexedLoad{std::move(f0), std::move(f1), std::move(f2), std::move(f3)};
}

void repr(std::string& out, const IndirectIndexedLoad& v) {
  out += "IndirectIndexedLoad(";
  out += "ptr=";
  repr(out, v.ptr);
  out += ", index=";
  repr(out, v.index);
  out += ", dst=";
  repr(out, v.dst);
  out += ", is_volatile=";
  out += v.is_volatile ? "True" : "False";
  out += ")";
}

IndirectIndexedLoad read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<IndirectIndexedLoad>) {
  r.expect_name("IndirectIndexedLoad");
  return read_body_IndirectIndexedLoad(r);
}

void write(::c6502::asdl::Writer& w, const IndirectIndexedStore& v) {
  w.write_name("IndirectIndexedStore");
  w.open();
  write(w, v.ptr);
  w.space();
  write(w, v.index);
  w.space();
  write(w, v.src);
  w.space();
  w.write_bool(v.is_volatile);
  w.close();
}

static IndirectIndexedStore read_body_IndirectIndexedStore(::c6502::asdl::Reader& r) {
  r.open();
  Type_val f0 = read(r, ::c6502::asdl::Tag<Type_val>{});
  Type_val f1 = read(r, ::c6502::asdl::Tag<Type_val>{});
  Type_val f2 = read(r, ::c6502::asdl::Tag<Type_val>{});
  bool f3 = r.read_bool();
  r.close();
  return IndirectIndexedStore{std::move(f0), std::move(f1), std::move(f2), std::move(f3)};
}

void repr(std::string& out, const IndirectIndexedStore& v) {
  out += "IndirectIndexedStore(";
  out += "ptr=";
  repr(out, v.ptr);
  out += ", index=";
  repr(out, v.index);
  out += ", src=";
  repr(out, v.src);
  out += ", is_volatile=";
  out += v.is_volatile ? "True" : "False";
  out += ")";
}

IndirectIndexedStore read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<IndirectIndexedStore>) {
  r.expect_name("IndirectIndexedStore");
  return read_body_IndirectIndexedStore(r);
}

void write(::c6502::asdl::Writer& w, const Unary& v) {
  w.write_name("Unary");
  w.open();
  write(w, v.op);
  w.space();
  write(w, v.src);
  w.space();
  write(w, v.dst);
  w.close();
}

static Unary read_body_Unary(::c6502::asdl::Reader& r) {
  r.open();
  Type_unary_operator f0 = read(r, ::c6502::asdl::Tag<Type_unary_operator>{});
  Type_val f1 = read(r, ::c6502::asdl::Tag<Type_val>{});
  Type_val f2 = read(r, ::c6502::asdl::Tag<Type_val>{});
  r.close();
  return Unary{std::move(f0), std::move(f1), std::move(f2)};
}

void repr(std::string& out, const Unary& v) {
  out += "Unary(";
  out += "op=";
  repr(out, v.op);
  out += ", src=";
  repr(out, v.src);
  out += ", dst=";
  repr(out, v.dst);
  out += ")";
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
  write(w, v.src1);
  w.space();
  write(w, v.src2);
  w.space();
  write(w, v.dst);
  w.close();
}

static Binary read_body_Binary(::c6502::asdl::Reader& r) {
  r.open();
  Type_binary_operator f0 = read(r, ::c6502::asdl::Tag<Type_binary_operator>{});
  Type_val f1 = read(r, ::c6502::asdl::Tag<Type_val>{});
  Type_val f2 = read(r, ::c6502::asdl::Tag<Type_val>{});
  Type_val f3 = read(r, ::c6502::asdl::Tag<Type_val>{});
  r.close();
  return Binary{std::move(f0), std::move(f1), std::move(f2), std::move(f3)};
}

void repr(std::string& out, const Binary& v) {
  out += "Binary(";
  out += "op=";
  repr(out, v.op);
  out += ", src1=";
  repr(out, v.src1);
  out += ", src2=";
  repr(out, v.src2);
  out += ", dst=";
  repr(out, v.dst);
  out += ")";
}

Binary read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Binary>) {
  r.expect_name("Binary");
  return read_body_Binary(r);
}

void write(::c6502::asdl::Writer& w, const Copy& v) {
  w.write_name("Copy");
  w.open();
  write(w, v.src);
  w.space();
  write(w, v.dst);
  w.close();
}

static Copy read_body_Copy(::c6502::asdl::Reader& r) {
  r.open();
  Type_val f0 = read(r, ::c6502::asdl::Tag<Type_val>{});
  Type_val f1 = read(r, ::c6502::asdl::Tag<Type_val>{});
  r.close();
  return Copy{std::move(f0), std::move(f1)};
}

void repr(std::string& out, const Copy& v) {
  out += "Copy(";
  out += "src=";
  repr(out, v.src);
  out += ", dst=";
  repr(out, v.dst);
  out += ")";
}

Copy read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Copy>) {
  r.expect_name("Copy");
  return read_body_Copy(r);
}

void write(::c6502::asdl::Writer& w, const Jump& v) {
  w.write_name("Jump");
  w.open();
  w.write_string(v.target);
  w.close();
}

static Jump read_body_Jump(::c6502::asdl::Reader& r) {
  r.open();
  std::string f0 = r.read_string();
  r.close();
  return Jump{std::move(f0)};
}

void repr(std::string& out, const Jump& v) {
  out += "Jump(";
  out += "target=";
  out += ::c6502::pyrepr::string(v.target);
  out += ")";
}

Jump read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Jump>) {
  r.expect_name("Jump");
  return read_body_Jump(r);
}

void write(::c6502::asdl::Writer& w, const JumpIfTrue& v) {
  w.write_name("JumpIfTrue");
  w.open();
  write(w, v.condition);
  w.space();
  w.write_string(v.target);
  w.close();
}

static JumpIfTrue read_body_JumpIfTrue(::c6502::asdl::Reader& r) {
  r.open();
  Type_val f0 = read(r, ::c6502::asdl::Tag<Type_val>{});
  std::string f1 = r.read_string();
  r.close();
  return JumpIfTrue{std::move(f0), std::move(f1)};
}

void repr(std::string& out, const JumpIfTrue& v) {
  out += "JumpIfTrue(";
  out += "condition=";
  repr(out, v.condition);
  out += ", target=";
  out += ::c6502::pyrepr::string(v.target);
  out += ")";
}

JumpIfTrue read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<JumpIfTrue>) {
  r.expect_name("JumpIfTrue");
  return read_body_JumpIfTrue(r);
}

void write(::c6502::asdl::Writer& w, const JumpIfFalse& v) {
  w.write_name("JumpIfFalse");
  w.open();
  write(w, v.condition);
  w.space();
  w.write_string(v.target);
  w.close();
}

static JumpIfFalse read_body_JumpIfFalse(::c6502::asdl::Reader& r) {
  r.open();
  Type_val f0 = read(r, ::c6502::asdl::Tag<Type_val>{});
  std::string f1 = r.read_string();
  r.close();
  return JumpIfFalse{std::move(f0), std::move(f1)};
}

void repr(std::string& out, const JumpIfFalse& v) {
  out += "JumpIfFalse(";
  out += "condition=";
  repr(out, v.condition);
  out += ", target=";
  out += ::c6502::pyrepr::string(v.target);
  out += ")";
}

JumpIfFalse read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<JumpIfFalse>) {
  r.expect_name("JumpIfFalse");
  return read_body_JumpIfFalse(r);
}

void write(::c6502::asdl::Writer& w, const JumpIfCmp& v) {
  w.write_name("JumpIfCmp");
  w.open();
  write(w, v.op);
  w.space();
  write(w, v.src1);
  w.space();
  write(w, v.src2);
  w.space();
  w.write_string(v.target);
  w.close();
}

static JumpIfCmp read_body_JumpIfCmp(::c6502::asdl::Reader& r) {
  r.open();
  Type_binary_operator f0 = read(r, ::c6502::asdl::Tag<Type_binary_operator>{});
  Type_val f1 = read(r, ::c6502::asdl::Tag<Type_val>{});
  Type_val f2 = read(r, ::c6502::asdl::Tag<Type_val>{});
  std::string f3 = r.read_string();
  r.close();
  return JumpIfCmp{std::move(f0), std::move(f1), std::move(f2), std::move(f3)};
}

void repr(std::string& out, const JumpIfCmp& v) {
  out += "JumpIfCmp(";
  out += "op=";
  repr(out, v.op);
  out += ", src1=";
  repr(out, v.src1);
  out += ", src2=";
  repr(out, v.src2);
  out += ", target=";
  out += ::c6502::pyrepr::string(v.target);
  out += ")";
}

JumpIfCmp read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<JumpIfCmp>) {
  r.expect_name("JumpIfCmp");
  return read_body_JumpIfCmp(r);
}

void write(::c6502::asdl::Writer& w, const JumpIfMasked& v) {
  w.write_name("JumpIfMasked");
  w.open();
  write(w, v.val);
  w.space();
  w.write_int(v.mask);
  w.space();
  w.write_bool(v.jump_when_nonzero);
  w.space();
  w.write_string(v.target);
  w.close();
}

static JumpIfMasked read_body_JumpIfMasked(::c6502::asdl::Reader& r) {
  r.open();
  Type_val f0 = read(r, ::c6502::asdl::Tag<Type_val>{});
  ::c6502::asdl::Int f1 = r.read_int();
  bool f2 = r.read_bool();
  std::string f3 = r.read_string();
  r.close();
  return JumpIfMasked{std::move(f0), std::move(f1), std::move(f2), std::move(f3)};
}

void repr(std::string& out, const JumpIfMasked& v) {
  out += "JumpIfMasked(";
  out += "val=";
  repr(out, v.val);
  out += ", mask=";
  out += ::c6502::asdl::int_to_string(v.mask);
  out += ", jump_when_nonzero=";
  out += v.jump_when_nonzero ? "True" : "False";
  out += ", target=";
  out += ::c6502::pyrepr::string(v.target);
  out += ")";
}

JumpIfMasked read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<JumpIfMasked>) {
  r.expect_name("JumpIfMasked");
  return read_body_JumpIfMasked(r);
}

void write(::c6502::asdl::Writer& w, const Label& v) {
  w.write_name("Label");
  w.open();
  w.write_string(v.name);
  w.close();
}

static Label read_body_Label(::c6502::asdl::Reader& r) {
  r.open();
  std::string f0 = r.read_string();
  r.close();
  return Label{std::move(f0)};
}

void repr(std::string& out, const Label& v) {
  out += "Label(";
  out += "name=";
  out += ::c6502::pyrepr::string(v.name);
  out += ")";
}

Label read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Label>) {
  r.expect_name("Label");
  return read_body_Label(r);
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
  if (v.dst) {
    write(w, (*v.dst));
  } else {
    w.write_none();
  }
  w.space();
  w.write_bool(v.hw_stack_save);
  w.space();
  w.write_bool(v.noreturn);
  w.close();
}

static FunctionCall read_body_FunctionCall(::c6502::asdl::Reader& r) {
  r.open();
  std::string f0 = r.read_string();
  std::vector<Type_val> f1;
  r.begin_seq();
  while (!r.end_seq()) {
    f1.push_back(read(r, ::c6502::asdl::Tag<Type_val>{}));
  }
  std::optional<Type_val> f2;
  if (!r.read_none()) {
    f2 = read(r, ::c6502::asdl::Tag<Type_val>{});
  }
  bool f3 = r.read_bool();
  bool f4 = r.read_bool();
  r.close();
  return FunctionCall{std::move(f0), std::move(f1), std::move(f2), std::move(f3), std::move(f4)};
}

void repr(std::string& out, const FunctionCall& v) {
  out += "FunctionCall(";
  out += "name=";
  out += ::c6502::pyrepr::string(v.name);
  out += ", args=";
  out += "[";
  {
    bool first = true;
    for (const auto& e : v.args) {
      if (!first) out += ", ";
      first = false;
      repr(out, e);
    }
  }
  out += "]";
  out += ", dst=";
  if (v.dst) {
    repr(out, (*v.dst));
  } else {
    out += "None";
  }
  out += ", hw_stack_save=";
  out += v.hw_stack_save ? "True" : "False";
  out += ", noreturn=";
  out += v.noreturn ? "True" : "False";
  out += ")";
}

FunctionCall read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<FunctionCall>) {
  r.expect_name("FunctionCall");
  return read_body_FunctionCall(r);
}

void write(::c6502::asdl::Writer& w, const IndirectCall& v) {
  w.write_name("IndirectCall");
  w.open();
  write(w, v.ptr);
  w.space();
  w.begin_seq(false);
  for (const auto& e : v.args) {
    w.seq_item(false);
    write(w, e);
  }
  w.end_seq(false, v.args.empty());
  w.space();
  if (v.dst) {
    write(w, (*v.dst));
  } else {
    w.write_none();
  }
  w.close();
}

static IndirectCall read_body_IndirectCall(::c6502::asdl::Reader& r) {
  r.open();
  Type_val f0 = read(r, ::c6502::asdl::Tag<Type_val>{});
  std::vector<Type_val> f1;
  r.begin_seq();
  while (!r.end_seq()) {
    f1.push_back(read(r, ::c6502::asdl::Tag<Type_val>{}));
  }
  std::optional<Type_val> f2;
  if (!r.read_none()) {
    f2 = read(r, ::c6502::asdl::Tag<Type_val>{});
  }
  r.close();
  return IndirectCall{std::move(f0), std::move(f1), std::move(f2)};
}

void repr(std::string& out, const IndirectCall& v) {
  out += "IndirectCall(";
  out += "ptr=";
  repr(out, v.ptr);
  out += ", args=";
  out += "[";
  {
    bool first = true;
    for (const auto& e : v.args) {
      if (!first) out += ", ";
      first = false;
      repr(out, e);
    }
  }
  out += "]";
  out += ", dst=";
  if (v.dst) {
    repr(out, (*v.dst));
  } else {
    out += "None";
  }
  out += ")";
}

IndirectCall read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<IndirectCall>) {
  r.expect_name("IndirectCall");
  return read_body_IndirectCall(r);
}

void write(::c6502::asdl::Writer& w, const Phi& v) {
  w.write_name("Phi");
  w.open();
  write(w, v.dst);
  w.space();
  w.begin_seq(false);
  for (const auto& e : v.args) {
    w.seq_item(false);
    write(w, e);
  }
  w.end_seq(false, v.args.empty());
  w.close();
}

static Phi read_body_Phi(::c6502::asdl::Reader& r) {
  r.open();
  Type_val f0 = read(r, ::c6502::asdl::Tag<Type_val>{});
  std::vector<Type_phi_arg> f1;
  r.begin_seq();
  while (!r.end_seq()) {
    f1.push_back(read(r, ::c6502::asdl::Tag<Type_phi_arg>{}));
  }
  r.close();
  return Phi{std::move(f0), std::move(f1)};
}

void repr(std::string& out, const Phi& v) {
  out += "Phi(";
  out += "dst=";
  repr(out, v.dst);
  out += ", args=";
  out += "[";
  {
    bool first = true;
    for (const auto& e : v.args) {
      if (!first) out += ", ";
      first = false;
      repr(out, e);
    }
  }
  out += "]";
  out += ")";
}

Phi read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Phi>) {
  r.expect_name("Phi");
  return read_body_Phi(r);
}

void write(::c6502::asdl::Writer& w, const Type_instruction& v) {
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
    case 24:
      write(w, *std::get_if<24>(&v));
      return;
    case 25:
      write(w, *std::get_if<25>(&v));
      return;
    case 26:
      write(w, *std::get_if<26>(&v));
      return;
    case 27:
      write(w, *std::get_if<27>(&v));
      return;
    case 28:
      write(w, *std::get_if<28>(&v));
      return;
    case 29:
      write(w, *std::get_if<29>(&v));
      return;
    case 30:
      write(w, *std::get_if<30>(&v));
      return;
  }
}

void repr(std::string& out, const Type_instruction& v) {
  switch (v.index()) {
    case 0:
      repr(out, *std::get_if<0>(&v));
      return;
    case 1:
      repr(out, *std::get_if<1>(&v));
      return;
    case 2:
      repr(out, *std::get_if<2>(&v));
      return;
    case 3:
      repr(out, *std::get_if<3>(&v));
      return;
    case 4:
      repr(out, *std::get_if<4>(&v));
      return;
    case 5:
      repr(out, *std::get_if<5>(&v));
      return;
    case 6:
      repr(out, *std::get_if<6>(&v));
      return;
    case 7:
      repr(out, *std::get_if<7>(&v));
      return;
    case 8:
      repr(out, *std::get_if<8>(&v));
      return;
    case 9:
      repr(out, *std::get_if<9>(&v));
      return;
    case 10:
      repr(out, *std::get_if<10>(&v));
      return;
    case 11:
      repr(out, *std::get_if<11>(&v));
      return;
    case 12:
      repr(out, *std::get_if<12>(&v));
      return;
    case 13:
      repr(out, *std::get_if<13>(&v));
      return;
    case 14:
      repr(out, *std::get_if<14>(&v));
      return;
    case 15:
      repr(out, *std::get_if<15>(&v));
      return;
    case 16:
      repr(out, *std::get_if<16>(&v));
      return;
    case 17:
      repr(out, *std::get_if<17>(&v));
      return;
    case 18:
      repr(out, *std::get_if<18>(&v));
      return;
    case 19:
      repr(out, *std::get_if<19>(&v));
      return;
    case 20:
      repr(out, *std::get_if<20>(&v));
      return;
    case 21:
      repr(out, *std::get_if<21>(&v));
      return;
    case 22:
      repr(out, *std::get_if<22>(&v));
      return;
    case 23:
      repr(out, *std::get_if<23>(&v));
      return;
    case 24:
      repr(out, *std::get_if<24>(&v));
      return;
    case 25:
      repr(out, *std::get_if<25>(&v));
      return;
    case 26:
      repr(out, *std::get_if<26>(&v));
      return;
    case 27:
      repr(out, *std::get_if<27>(&v));
      return;
    case 28:
      repr(out, *std::get_if<28>(&v));
      return;
    case 29:
      repr(out, *std::get_if<29>(&v));
      return;
    case 30:
      repr(out, *std::get_if<30>(&v));
      return;
  }
}

Type_instruction read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Type_instruction>) {
  const std::string_view name = r.read_name();
  switch (name.size()) {
    case 3:
      if (name == "Ret") {
        return read_body_Ret(r);
      }
      if (name == "Phi") {
        return read_body_Phi(r);
      }
      break;
    case 4:
      if (name == "Load") {
        return read_body_Load(r);
      }
      if (name == "Copy") {
        return read_body_Copy(r);
      }
      if (name == "Jump") {
        return read_body_Jump(r);
      }
      break;
    case 5:
      if (name == "Store") {
        return read_body_Store(r);
      }
      if (name == "Unary") {
        return read_body_Unary(r);
      }
      if (name == "Label") {
        return read_body_Label(r);
      }
      break;
    case 6:
      if (name == "Binary") {
        return read_body_Binary(r);
      }
      break;
    case 8:
      if (name == "Truncate") {
        return read_body_Truncate(r);
      }
      break;
    case 9:
      if (name == "JumpIfCmp") {
        return read_body_JumpIfCmp(r);
      }
      break;
    case 10:
      if (name == "SignExtend") {
        return read_body_SignExtend(r);
      }
      if (name == "ZeroExtend") {
        return read_body_ZeroExtend(r);
      }
      if (name == "IntToFloat") {
        return read_body_IntToFloat(r);
      }
      if (name == "FloatToInt") {
        return read_body_FloatToInt(r);
      }
      if (name == "GetAddress") {
        return read_body_GetAddress(r);
      }
      if (name == "JumpIfTrue") {
        return read_body_JumpIfTrue(r);
      }
      break;
    case 11:
      if (name == "IntToDouble") {
        return read_body_IntToDouble(r);
      }
      if (name == "DoubleToInt") {
        return read_body_DoubleToInt(r);
      }
      if (name == "IndexedLoad") {
        return read_body_IndexedLoad(r);
      }
      if (name == "JumpIfFalse") {
        return read_body_JumpIfFalse(r);
      }
      break;
    case 12:
      if (name == "IndexedStore") {
        return read_body_IndexedStore(r);
      }
      if (name == "JumpIfMasked") {
        return read_body_JumpIfMasked(r);
      }
      if (name == "FunctionCall") {
        return read_body_FunctionCall(r);
      }
      if (name == "IndirectCall") {
        return read_body_IndirectCall(r);
      }
      break;
    case 13:
      if (name == "FloatToDouble") {
        return read_body_FloatToDouble(r);
      }
      if (name == "DoubleToFloat") {
        return read_body_DoubleToFloat(r);
      }
      break;
    case 16:
      if (name == "IndexedConstLoad") {
        return read_body_IndexedConstLoad(r);
      }
      break;
    case 18:
      if (name == "IndexedSymbolStore") {
        return read_body_IndexedSymbolStore(r);
      }
      break;
    case 19:
      if (name == "IndirectIndexedLoad") {
        return read_body_IndirectIndexedLoad(r);
      }
      break;
    case 20:
      if (name == "IndirectIndexedStore") {
        return read_body_IndirectIndexedStore(r);
      }
      break;
  }
  r.unknown_constructor(name, "instruction");
}

void write(::c6502::asdl::Writer& w, const Int&) {
  w.write_name("Int");
}

static Int read_body_Int(::c6502::asdl::Reader& r) {
  (void)r;
  return Int{};
}

void repr(std::string& out, const Int&) {
  out += "Int(";
  out += ")";
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

void repr(std::string& out, const Long&) {
  out += "Long(";
  out += ")";
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

void repr(std::string& out, const LongLong&) {
  out += "LongLong(";
  out += ")";
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

void repr(std::string& out, const UInt&) {
  out += "UInt(";
  out += ")";
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

void repr(std::string& out, const ULong&) {
  out += "ULong(";
  out += ")";
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

void repr(std::string& out, const ULongLong&) {
  out += "ULongLong(";
  out += ")";
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

void repr(std::string& out, const Float&) {
  out += "Float(";
  out += ")";
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

void repr(std::string& out, const Double&) {
  out += "Double(";
  out += ")";
}

Double read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Double>) {
  r.expect_name("Double");
  return read_body_Double(r);
}

void write(::c6502::asdl::Writer& w, const Void&) {
  w.write_name("Void");
}

static Void read_body_Void(::c6502::asdl::Reader& r) {
  (void)r;
  return Void{};
}

void repr(std::string& out, const Void&) {
  out += "Void(";
  out += ")";
}

Void read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Void>) {
  r.expect_name("Void");
  return read_body_Void(r);
}

void write(::c6502::asdl::Writer& w, const Pointer&) {
  w.write_name("Pointer");
}

static Pointer read_body_Pointer(::c6502::asdl::Reader& r) {
  (void)r;
  return Pointer{};
}

void repr(std::string& out, const Pointer&) {
  out += "Pointer(";
  out += ")";
}

Pointer read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Pointer>) {
  r.expect_name("Pointer");
  return read_body_Pointer(r);
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

void repr(std::string& out, const FunType& v) {
  out += "FunType(";
  out += "params=";
  out += "[";
  {
    bool first = true;
    for (const auto& e : v.params) {
      if (!first) out += ", ";
      first = false;
      repr(out, e);
    }
  }
  out += "]";
  out += ", ret=";
  repr(out, *v.ret);
  out += ")";
}

FunType read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<FunType>) {
  r.expect_name("FunType");
  return read_body_FunType(r);
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
  }
}

void repr(std::string& out, const Type_data_type& v) {
  switch (v.index()) {
    case 0:
      repr(out, *std::get_if<0>(&v));
      return;
    case 1:
      repr(out, *std::get_if<1>(&v));
      return;
    case 2:
      repr(out, *std::get_if<2>(&v));
      return;
    case 3:
      repr(out, *std::get_if<3>(&v));
      return;
    case 4:
      repr(out, *std::get_if<4>(&v));
      return;
    case 5:
      repr(out, *std::get_if<5>(&v));
      return;
    case 6:
      repr(out, *std::get_if<6>(&v));
      return;
    case 7:
      repr(out, *std::get_if<7>(&v));
      return;
    case 8:
      repr(out, *std::get_if<8>(&v));
      return;
    case 9:
      repr(out, *std::get_if<9>(&v));
      return;
    case 10:
      repr(out, *std::get_if<10>(&v));
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
      break;
    case 6:
      if (name == "Double") {
        return read_body_Double(r);
      }
      break;
    case 7:
      if (name == "Pointer") {
        return read_body_Pointer(r);
      }
      if (name == "FunType") {
        return read_body_FunType(r);
      }
      break;
    case 8:
      if (name == "LongLong") {
        return read_body_LongLong(r);
      }
      break;
    case 9:
      if (name == "ULongLong") {
        return read_body_ULongLong(r);
      }
      break;
  }
  r.unknown_constructor(name, "data_type");
}

void write(::c6502::asdl::Writer& w, const CharInit& v) {
  w.write_name("CharInit");
  w.open();
  w.write_int(v.value);
  w.close();
}

static CharInit read_body_CharInit(::c6502::asdl::Reader& r) {
  r.open();
  ::c6502::asdl::Int f0 = r.read_int();
  r.close();
  return CharInit{std::move(f0)};
}

void repr(std::string& out, const CharInit& v) {
  out += "CharInit(";
  out += "value=";
  out += ::c6502::asdl::int_to_string(v.value);
  out += ")";
}

CharInit read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<CharInit>) {
  r.expect_name("CharInit");
  return read_body_CharInit(r);
}

void write(::c6502::asdl::Writer& w, const UCharInit& v) {
  w.write_name("UCharInit");
  w.open();
  w.write_int(v.value);
  w.close();
}

static UCharInit read_body_UCharInit(::c6502::asdl::Reader& r) {
  r.open();
  ::c6502::asdl::Int f0 = r.read_int();
  r.close();
  return UCharInit{std::move(f0)};
}

void repr(std::string& out, const UCharInit& v) {
  out += "UCharInit(";
  out += "value=";
  out += ::c6502::asdl::int_to_string(v.value);
  out += ")";
}

UCharInit read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<UCharInit>) {
  r.expect_name("UCharInit");
  return read_body_UCharInit(r);
}

void write(::c6502::asdl::Writer& w, const IntInit& v) {
  w.write_name("IntInit");
  w.open();
  w.write_int(v.value);
  w.close();
}

static IntInit read_body_IntInit(::c6502::asdl::Reader& r) {
  r.open();
  ::c6502::asdl::Int f0 = r.read_int();
  r.close();
  return IntInit{std::move(f0)};
}

void repr(std::string& out, const IntInit& v) {
  out += "IntInit(";
  out += "value=";
  out += ::c6502::asdl::int_to_string(v.value);
  out += ")";
}

IntInit read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<IntInit>) {
  r.expect_name("IntInit");
  return read_body_IntInit(r);
}

void write(::c6502::asdl::Writer& w, const LongInit& v) {
  w.write_name("LongInit");
  w.open();
  w.write_int(v.value);
  w.close();
}

static LongInit read_body_LongInit(::c6502::asdl::Reader& r) {
  r.open();
  ::c6502::asdl::Int f0 = r.read_int();
  r.close();
  return LongInit{std::move(f0)};
}

void repr(std::string& out, const LongInit& v) {
  out += "LongInit(";
  out += "value=";
  out += ::c6502::asdl::int_to_string(v.value);
  out += ")";
}

LongInit read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<LongInit>) {
  r.expect_name("LongInit");
  return read_body_LongInit(r);
}

void write(::c6502::asdl::Writer& w, const LongLongInit& v) {
  w.write_name("LongLongInit");
  w.open();
  w.write_int(v.value);
  w.close();
}

static LongLongInit read_body_LongLongInit(::c6502::asdl::Reader& r) {
  r.open();
  ::c6502::asdl::Int f0 = r.read_int();
  r.close();
  return LongLongInit{std::move(f0)};
}

void repr(std::string& out, const LongLongInit& v) {
  out += "LongLongInit(";
  out += "value=";
  out += ::c6502::asdl::int_to_string(v.value);
  out += ")";
}

LongLongInit read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<LongLongInit>) {
  r.expect_name("LongLongInit");
  return read_body_LongLongInit(r);
}

void write(::c6502::asdl::Writer& w, const UIntInit& v) {
  w.write_name("UIntInit");
  w.open();
  w.write_int(v.value);
  w.close();
}

static UIntInit read_body_UIntInit(::c6502::asdl::Reader& r) {
  r.open();
  ::c6502::asdl::Int f0 = r.read_int();
  r.close();
  return UIntInit{std::move(f0)};
}

void repr(std::string& out, const UIntInit& v) {
  out += "UIntInit(";
  out += "value=";
  out += ::c6502::asdl::int_to_string(v.value);
  out += ")";
}

UIntInit read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<UIntInit>) {
  r.expect_name("UIntInit");
  return read_body_UIntInit(r);
}

void write(::c6502::asdl::Writer& w, const ULongInit& v) {
  w.write_name("ULongInit");
  w.open();
  w.write_int(v.value);
  w.close();
}

static ULongInit read_body_ULongInit(::c6502::asdl::Reader& r) {
  r.open();
  ::c6502::asdl::Int f0 = r.read_int();
  r.close();
  return ULongInit{std::move(f0)};
}

void repr(std::string& out, const ULongInit& v) {
  out += "ULongInit(";
  out += "value=";
  out += ::c6502::asdl::int_to_string(v.value);
  out += ")";
}

ULongInit read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<ULongInit>) {
  r.expect_name("ULongInit");
  return read_body_ULongInit(r);
}

void write(::c6502::asdl::Writer& w, const ULongLongInit& v) {
  w.write_name("ULongLongInit");
  w.open();
  w.write_int(v.value);
  w.close();
}

static ULongLongInit read_body_ULongLongInit(::c6502::asdl::Reader& r) {
  r.open();
  ::c6502::asdl::Int f0 = r.read_int();
  r.close();
  return ULongLongInit{std::move(f0)};
}

void repr(std::string& out, const ULongLongInit& v) {
  out += "ULongLongInit(";
  out += "value=";
  out += ::c6502::asdl::int_to_string(v.value);
  out += ")";
}

ULongLongInit read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<ULongLongInit>) {
  r.expect_name("ULongLongInit");
  return read_body_ULongLongInit(r);
}

void write(::c6502::asdl::Writer& w, const FloatInit& v) {
  w.write_name("FloatInit");
  w.open();
  w.write_int(v.bits);
  w.close();
}

static FloatInit read_body_FloatInit(::c6502::asdl::Reader& r) {
  r.open();
  ::c6502::asdl::Int f0 = r.read_int();
  r.close();
  return FloatInit{std::move(f0)};
}

void repr(std::string& out, const FloatInit& v) {
  out += "FloatInit(";
  out += "bits=";
  out += ::c6502::asdl::int_to_string(v.bits);
  out += ")";
}

FloatInit read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<FloatInit>) {
  r.expect_name("FloatInit");
  return read_body_FloatInit(r);
}

void write(::c6502::asdl::Writer& w, const DoubleInit& v) {
  w.write_name("DoubleInit");
  w.open();
  w.write_int(v.bits);
  w.close();
}

static DoubleInit read_body_DoubleInit(::c6502::asdl::Reader& r) {
  r.open();
  ::c6502::asdl::Int f0 = r.read_int();
  r.close();
  return DoubleInit{std::move(f0)};
}

void repr(std::string& out, const DoubleInit& v) {
  out += "DoubleInit(";
  out += "bits=";
  out += ::c6502::asdl::int_to_string(v.bits);
  out += ")";
}

DoubleInit read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<DoubleInit>) {
  r.expect_name("DoubleInit");
  return read_body_DoubleInit(r);
}

void write(::c6502::asdl::Writer& w, const AddressInit& v) {
  w.write_name("AddressInit");
  w.open();
  w.write_string(v.name);
  w.space();
  w.write_int(v.offset);
  w.close();
}

static AddressInit read_body_AddressInit(::c6502::asdl::Reader& r) {
  r.open();
  std::string f0 = r.read_string();
  ::c6502::asdl::Int f1 = r.read_int();
  r.close();
  return AddressInit{std::move(f0), std::move(f1)};
}

void repr(std::string& out, const AddressInit& v) {
  out += "AddressInit(";
  out += "name=";
  out += ::c6502::pyrepr::string(v.name);
  out += ", offset=";
  out += ::c6502::asdl::int_to_string(v.offset);
  out += ")";
}

AddressInit read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<AddressInit>) {
  r.expect_name("AddressInit");
  return read_body_AddressInit(r);
}

void write(::c6502::asdl::Writer& w, const AddressByteInit& v) {
  w.write_name("AddressByteInit");
  w.open();
  w.write_string(v.name);
  w.space();
  w.write_int(v.offset);
  w.space();
  w.write_bool(v.high);
  w.close();
}

static AddressByteInit read_body_AddressByteInit(::c6502::asdl::Reader& r) {
  r.open();
  std::string f0 = r.read_string();
  ::c6502::asdl::Int f1 = r.read_int();
  bool f2 = r.read_bool();
  r.close();
  return AddressByteInit{std::move(f0), std::move(f1), std::move(f2)};
}

void repr(std::string& out, const AddressByteInit& v) {
  out += "AddressByteInit(";
  out += "name=";
  out += ::c6502::pyrepr::string(v.name);
  out += ", offset=";
  out += ::c6502::asdl::int_to_string(v.offset);
  out += ", high=";
  out += v.high ? "True" : "False";
  out += ")";
}

AddressByteInit read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<AddressByteInit>) {
  r.expect_name("AddressByteInit");
  return read_body_AddressByteInit(r);
}

void write(::c6502::asdl::Writer& w, const StringInit& v) {
  w.write_name("StringInit");
  w.open();
  w.write_string(v.str);
  w.space();
  w.write_int(v.bytes);
  w.close();
}

static StringInit read_body_StringInit(::c6502::asdl::Reader& r) {
  r.open();
  std::string f0 = r.read_string();
  ::c6502::asdl::Int f1 = r.read_int();
  r.close();
  return StringInit{std::move(f0), std::move(f1)};
}

void repr(std::string& out, const StringInit& v) {
  out += "StringInit(";
  out += "str=";
  out += ::c6502::pyrepr::string(v.str);
  out += ", bytes=";
  out += ::c6502::asdl::int_to_string(v.bytes);
  out += ")";
}

StringInit read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<StringInit>) {
  r.expect_name("StringInit");
  return read_body_StringInit(r);
}

void write(::c6502::asdl::Writer& w, const ZeroInit& v) {
  w.write_name("ZeroInit");
  w.open();
  w.write_int(v.bytes);
  w.close();
}

static ZeroInit read_body_ZeroInit(::c6502::asdl::Reader& r) {
  r.open();
  ::c6502::asdl::Int f0 = r.read_int();
  r.close();
  return ZeroInit{std::move(f0)};
}

void repr(std::string& out, const ZeroInit& v) {
  out += "ZeroInit(";
  out += "bytes=";
  out += ::c6502::asdl::int_to_string(v.bytes);
  out += ")";
}

ZeroInit read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<ZeroInit>) {
  r.expect_name("ZeroInit");
  return read_body_ZeroInit(r);
}

void write(::c6502::asdl::Writer& w, const Type_static_init& v) {
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
  }
}

void repr(std::string& out, const Type_static_init& v) {
  switch (v.index()) {
    case 0:
      repr(out, *std::get_if<0>(&v));
      return;
    case 1:
      repr(out, *std::get_if<1>(&v));
      return;
    case 2:
      repr(out, *std::get_if<2>(&v));
      return;
    case 3:
      repr(out, *std::get_if<3>(&v));
      return;
    case 4:
      repr(out, *std::get_if<4>(&v));
      return;
    case 5:
      repr(out, *std::get_if<5>(&v));
      return;
    case 6:
      repr(out, *std::get_if<6>(&v));
      return;
    case 7:
      repr(out, *std::get_if<7>(&v));
      return;
    case 8:
      repr(out, *std::get_if<8>(&v));
      return;
    case 9:
      repr(out, *std::get_if<9>(&v));
      return;
    case 10:
      repr(out, *std::get_if<10>(&v));
      return;
    case 11:
      repr(out, *std::get_if<11>(&v));
      return;
    case 12:
      repr(out, *std::get_if<12>(&v));
      return;
    case 13:
      repr(out, *std::get_if<13>(&v));
      return;
  }
}

Type_static_init read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Type_static_init>) {
  const std::string_view name = r.read_name();
  switch (name.size()) {
    case 7:
      if (name == "IntInit") {
        return read_body_IntInit(r);
      }
      break;
    case 8:
      if (name == "CharInit") {
        return read_body_CharInit(r);
      }
      if (name == "LongInit") {
        return read_body_LongInit(r);
      }
      if (name == "UIntInit") {
        return read_body_UIntInit(r);
      }
      if (name == "ZeroInit") {
        return read_body_ZeroInit(r);
      }
      break;
    case 9:
      if (name == "UCharInit") {
        return read_body_UCharInit(r);
      }
      if (name == "ULongInit") {
        return read_body_ULongInit(r);
      }
      if (name == "FloatInit") {
        return read_body_FloatInit(r);
      }
      break;
    case 10:
      if (name == "DoubleInit") {
        return read_body_DoubleInit(r);
      }
      if (name == "StringInit") {
        return read_body_StringInit(r);
      }
      break;
    case 11:
      if (name == "AddressInit") {
        return read_body_AddressInit(r);
      }
      break;
    case 12:
      if (name == "LongLongInit") {
        return read_body_LongLongInit(r);
      }
      break;
    case 13:
      if (name == "ULongLongInit") {
        return read_body_ULongLongInit(r);
      }
      break;
    case 15:
      if (name == "AddressByteInit") {
        return read_body_AddressByteInit(r);
      }
      break;
  }
  r.unknown_constructor(name, "static_init");
}

void write(::c6502::asdl::Writer& w, const Function& v) {
  w.write_name("Function");
  w.open();
  w.write_string(v.name);
  w.space();
  w.write_bool(v.is_global);
  w.space();
  w.begin_seq(false);
  for (const auto& e : v.params) {
    w.seq_item(false);
    w.write_string(e);
  }
  w.end_seq(false, v.params.empty());
  w.space();
  w.begin_seq(true);
  for (const auto& e : v.instructions) {
    w.seq_item(true);
    write(w, e);
  }
  w.end_seq(true, v.instructions.empty());
  w.close();
}

static Function read_body_Function(::c6502::asdl::Reader& r) {
  r.open();
  std::string f0 = r.read_string();
  bool f1 = r.read_bool();
  std::vector<std::string> f2;
  r.begin_seq();
  while (!r.end_seq()) {
    f2.push_back(r.read_string());
  }
  std::vector<Type_instruction> f3;
  r.begin_seq();
  while (!r.end_seq()) {
    f3.push_back(read(r, ::c6502::asdl::Tag<Type_instruction>{}));
  }
  r.close();
  return Function{std::move(f0), std::move(f1), std::move(f2), std::move(f3)};
}

void repr(std::string& out, const Function& v) {
  out += "Function(";
  out += "name=";
  out += ::c6502::pyrepr::string(v.name);
  out += ", is_global=";
  out += v.is_global ? "True" : "False";
  out += ", params=";
  out += "[";
  {
    bool first = true;
    for (const auto& e : v.params) {
      if (!first) out += ", ";
      first = false;
      out += ::c6502::pyrepr::string(e);
    }
  }
  out += "]";
  out += ", instructions=";
  out += "[";
  {
    bool first = true;
    for (const auto& e : v.instructions) {
      if (!first) out += ", ";
      first = false;
      repr(out, e);
    }
  }
  out += "]";
  out += ")";
}

Function read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Function>) {
  r.expect_name("Function");
  return read_body_Function(r);
}

void write(::c6502::asdl::Writer& w, const StaticVariable& v) {
  w.write_name("StaticVariable");
  w.open();
  w.write_string(v.name);
  w.space();
  w.write_bool(v.is_global);
  w.space();
  write(w, v.data_type);
  w.space();
  w.begin_seq(false);
  for (const auto& e : v.init) {
    w.seq_item(false);
    write(w, e);
  }
  w.end_seq(false, v.init.empty());
  w.close();
}

static StaticVariable read_body_StaticVariable(::c6502::asdl::Reader& r) {
  r.open();
  std::string f0 = r.read_string();
  bool f1 = r.read_bool();
  Type_data_type f2 = read(r, ::c6502::asdl::Tag<Type_data_type>{});
  std::vector<Type_static_init> f3;
  r.begin_seq();
  while (!r.end_seq()) {
    f3.push_back(read(r, ::c6502::asdl::Tag<Type_static_init>{}));
  }
  r.close();
  return StaticVariable{std::move(f0), std::move(f1), std::move(f2), std::move(f3)};
}

void repr(std::string& out, const StaticVariable& v) {
  out += "StaticVariable(";
  out += "name=";
  out += ::c6502::pyrepr::string(v.name);
  out += ", is_global=";
  out += v.is_global ? "True" : "False";
  out += ", data_type=";
  repr(out, v.data_type);
  out += ", init=";
  out += "[";
  {
    bool first = true;
    for (const auto& e : v.init) {
      if (!first) out += ", ";
      first = false;
      repr(out, e);
    }
  }
  out += "]";
  out += ")";
}

StaticVariable read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<StaticVariable>) {
  r.expect_name("StaticVariable");
  return read_body_StaticVariable(r);
}

void write(::c6502::asdl::Writer& w, const Type_top_level& v) {
  switch (v.index()) {
    case 0:
      write(w, *std::get_if<0>(&v));
      return;
    case 1:
      write(w, *std::get_if<1>(&v));
      return;
  }
}

void repr(std::string& out, const Type_top_level& v) {
  switch (v.index()) {
    case 0:
      repr(out, *std::get_if<0>(&v));
      return;
    case 1:
      repr(out, *std::get_if<1>(&v));
      return;
  }
}

Type_top_level read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Type_top_level>) {
  const std::string_view name = r.read_name();
  switch (name.size()) {
    case 8:
      if (name == "Function") {
        return read_body_Function(r);
      }
      break;
    case 14:
      if (name == "StaticVariable") {
        return read_body_StaticVariable(r);
      }
      break;
  }
  r.unknown_constructor(name, "top_level");
}

void write(::c6502::asdl::Writer& w, const Program& v) {
  w.write_name("Program");
  w.open();
  w.begin_seq(true);
  for (const auto& e : v.top_level) {
    w.seq_item(true);
    write(w, e);
  }
  w.end_seq(true, v.top_level.empty());
  w.close();
}

static Program read_body_Program(::c6502::asdl::Reader& r) {
  r.open();
  std::vector<Type_top_level> f0;
  r.begin_seq();
  while (!r.end_seq()) {
    f0.push_back(read(r, ::c6502::asdl::Tag<Type_top_level>{}));
  }
  r.close();
  return Program{std::move(f0)};
}

void repr(std::string& out, const Program& v) {
  out += "Program(";
  out += "top_level=";
  out += "[";
  {
    bool first = true;
    for (const auto& e : v.top_level) {
      if (!first) out += ", ";
      first = false;
      repr(out, e);
    }
  }
  out += "]";
  out += ")";
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

void repr(std::string& out, const Type_program& v) {
  switch (v.index()) {
    case 0:
      repr(out, *std::get_if<0>(&v));
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

}  // namespace c6502::tac_ast
