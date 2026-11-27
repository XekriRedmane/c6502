// Generated from asm.asdl by asdlgen (cpp/tools/asdlgen.h). Do not edit.
#include "asm_ast.h"

#include <utility>

#include "pcpp_text.h"
#include "pyrepr.h"

namespace c6502::asm_ast {

// Starts a line of pretty.py's text at the indentation level
// `depth`, two spaces to a level.
[[maybe_unused]] static void newline(std::string& out, int depth) {
  out += '\n';
  out.append(static_cast<std::size_t>(2 * depth), ' ');
}

void write(::c6502::asdl::Writer& w, const A&) {
  w.write_name("A");
}

static A read_body_A(::c6502::asdl::Reader& r) {
  (void)r;
  return A{};
}

void repr(std::string& out, const A&) {
  out += "A(";
  out += ")";
}

void pretty(std::string& out, const A&, int) {
  out += "A()";
}

A read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<A>) {
  r.expect_name("A");
  return read_body_A(r);
}

void write(::c6502::asdl::Writer& w, const X&) {
  w.write_name("X");
}

static X read_body_X(::c6502::asdl::Reader& r) {
  (void)r;
  return X{};
}

void repr(std::string& out, const X&) {
  out += "X(";
  out += ")";
}

void pretty(std::string& out, const X&, int) {
  out += "X()";
}

X read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<X>) {
  r.expect_name("X");
  return read_body_X(r);
}

void write(::c6502::asdl::Writer& w, const Y&) {
  w.write_name("Y");
}

static Y read_body_Y(::c6502::asdl::Reader& r) {
  (void)r;
  return Y{};
}

void repr(std::string& out, const Y&) {
  out += "Y(";
  out += ")";
}

void pretty(std::string& out, const Y&, int) {
  out += "Y()";
}

Y read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Y>) {
  r.expect_name("Y");
  return read_body_Y(r);
}

void write(::c6502::asdl::Writer& w, const Type_reg& v) {
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

void repr(std::string& out, const Type_reg& v) {
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
  }
}

void pretty(std::string& out, const Type_reg& v, int depth) {
  switch (v.index()) {
    case 0:
      pretty(out, *std::get_if<0>(&v), depth);
      return;
    case 1:
      pretty(out, *std::get_if<1>(&v), depth);
      return;
    case 2:
      pretty(out, *std::get_if<2>(&v), depth);
      return;
  }
}

Type_reg read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Type_reg>) {
  const std::string_view name = r.read_name();
  switch (name.size()) {
    case 1:
      if (name == "A") {
        return read_body_A(r);
      }
      if (name == "X") {
        return read_body_X(r);
      }
      if (name == "Y") {
        return read_body_Y(r);
      }
      break;
  }
  r.unknown_constructor(name, "reg");
}

void write(::c6502::asdl::Writer& w, const Imm& v) {
  w.write_name("Imm");
  w.open();
  w.write_int(v.value);
  w.close();
}

static Imm read_body_Imm(::c6502::asdl::Reader& r) {
  r.open();
  ::c6502::asdl::Int f0 = r.read_int();
  r.close();
  return Imm{std::move(f0)};
}

void repr(std::string& out, const Imm& v) {
  out += "Imm(";
  out += "value=";
  out += ::c6502::asdl::int_to_string(v.value);
  out += ")";
}

void pretty(std::string& out, const Imm& v, int depth) {
  out += "Imm(";
  newline(out, depth + 1);
  out += "value=";
  out += ::c6502::asdl::int_to_string(v.value);
  out += ',';
  newline(out, depth);
  out += ')';
}

Imm read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Imm>) {
  r.expect_name("Imm");
  return read_body_Imm(r);
}

void write(::c6502::asdl::Writer& w, const Reg& v) {
  w.write_name("Reg");
  w.open();
  write(w, v.reg);
  w.close();
}

static Reg read_body_Reg(::c6502::asdl::Reader& r) {
  r.open();
  Type_reg f0 = read(r, ::c6502::asdl::Tag<Type_reg>{});
  r.close();
  return Reg{std::move(f0)};
}

void repr(std::string& out, const Reg& v) {
  out += "Reg(";
  out += "reg=";
  repr(out, v.reg);
  out += ")";
}

void pretty(std::string& out, const Reg& v, int depth) {
  out += "Reg(";
  newline(out, depth + 1);
  out += "reg=";
  pretty(out, v.reg, depth + 1);
  out += ',';
  newline(out, depth);
  out += ')';
}

Reg read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Reg>) {
  r.expect_name("Reg");
  return read_body_Reg(r);
}

void write(::c6502::asdl::Writer& w, const Pseudo& v) {
  w.write_name("Pseudo");
  w.open();
  w.write_string(v.name);
  w.space();
  w.write_int(v.offset);
  w.close();
}

static Pseudo read_body_Pseudo(::c6502::asdl::Reader& r) {
  r.open();
  std::string f0 = r.read_string();
  ::c6502::asdl::Int f1 = r.read_int();
  r.close();
  return Pseudo{std::move(f0), std::move(f1)};
}

void repr(std::string& out, const Pseudo& v) {
  out += "Pseudo(";
  out += "name=";
  out += ::c6502::pyrepr::string(v.name);
  out += ", offset=";
  out += ::c6502::asdl::int_to_string(v.offset);
  out += ")";
}

void pretty(std::string& out, const Pseudo& v, int depth) {
  out += "Pseudo(";
  newline(out, depth + 1);
  out += "name=";
  out += ::c6502::pcpp::py_repr(v.name);
  out += ',';
  newline(out, depth + 1);
  out += "offset=";
  out += ::c6502::asdl::int_to_string(v.offset);
  out += ',';
  newline(out, depth);
  out += ')';
}

Pseudo read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Pseudo>) {
  r.expect_name("Pseudo");
  return read_body_Pseudo(r);
}

void write(::c6502::asdl::Writer& w, const Stack& v) {
  w.write_name("Stack");
  w.open();
  w.write_int(v.offset);
  w.close();
}

static Stack read_body_Stack(::c6502::asdl::Reader& r) {
  r.open();
  ::c6502::asdl::Int f0 = r.read_int();
  r.close();
  return Stack{std::move(f0)};
}

void repr(std::string& out, const Stack& v) {
  out += "Stack(";
  out += "offset=";
  out += ::c6502::asdl::int_to_string(v.offset);
  out += ")";
}

void pretty(std::string& out, const Stack& v, int depth) {
  out += "Stack(";
  newline(out, depth + 1);
  out += "offset=";
  out += ::c6502::asdl::int_to_string(v.offset);
  out += ',';
  newline(out, depth);
  out += ')';
}

Stack read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Stack>) {
  r.expect_name("Stack");
  return read_body_Stack(r);
}

void write(::c6502::asdl::Writer& w, const Frame& v) {
  w.write_name("Frame");
  w.open();
  w.write_int(v.offset);
  w.close();
}

static Frame read_body_Frame(::c6502::asdl::Reader& r) {
  r.open();
  ::c6502::asdl::Int f0 = r.read_int();
  r.close();
  return Frame{std::move(f0)};
}

void repr(std::string& out, const Frame& v) {
  out += "Frame(";
  out += "offset=";
  out += ::c6502::asdl::int_to_string(v.offset);
  out += ")";
}

void pretty(std::string& out, const Frame& v, int depth) {
  out += "Frame(";
  newline(out, depth + 1);
  out += "offset=";
  out += ::c6502::asdl::int_to_string(v.offset);
  out += ',';
  newline(out, depth);
  out += ')';
}

Frame read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Frame>) {
  r.expect_name("Frame");
  return read_body_Frame(r);
}

void write(::c6502::asdl::Writer& w, const Data& v) {
  w.write_name("Data");
  w.open();
  w.write_string(v.name);
  w.space();
  w.write_int(v.offset);
  w.close();
}

static Data read_body_Data(::c6502::asdl::Reader& r) {
  r.open();
  std::string f0 = r.read_string();
  ::c6502::asdl::Int f1 = r.read_int();
  r.close();
  return Data{std::move(f0), std::move(f1)};
}

void repr(std::string& out, const Data& v) {
  out += "Data(";
  out += "name=";
  out += ::c6502::pyrepr::string(v.name);
  out += ", offset=";
  out += ::c6502::asdl::int_to_string(v.offset);
  out += ")";
}

void pretty(std::string& out, const Data& v, int depth) {
  out += "Data(";
  newline(out, depth + 1);
  out += "name=";
  out += ::c6502::pcpp::py_repr(v.name);
  out += ',';
  newline(out, depth + 1);
  out += "offset=";
  out += ::c6502::asdl::int_to_string(v.offset);
  out += ',';
  newline(out, depth);
  out += ')';
}

Data read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Data>) {
  r.expect_name("Data");
  return read_body_Data(r);
}

void write(::c6502::asdl::Writer& w, const Indirect& v) {
  w.write_name("Indirect");
  w.open();
  w.write_int(v.offset);
  w.close();
}

static Indirect read_body_Indirect(::c6502::asdl::Reader& r) {
  r.open();
  ::c6502::asdl::Int f0 = r.read_int();
  r.close();
  return Indirect{std::move(f0)};
}

void repr(std::string& out, const Indirect& v) {
  out += "Indirect(";
  out += "offset=";
  out += ::c6502::asdl::int_to_string(v.offset);
  out += ")";
}

void pretty(std::string& out, const Indirect& v, int depth) {
  out += "Indirect(";
  newline(out, depth + 1);
  out += "offset=";
  out += ::c6502::asdl::int_to_string(v.offset);
  out += ',';
  newline(out, depth);
  out += ')';
}

Indirect read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Indirect>) {
  r.expect_name("Indirect");
  return read_body_Indirect(r);
}

void write(::c6502::asdl::Writer& w, const IndirectY&) {
  w.write_name("IndirectY");
}

static IndirectY read_body_IndirectY(::c6502::asdl::Reader& r) {
  (void)r;
  return IndirectY{};
}

void repr(std::string& out, const IndirectY&) {
  out += "IndirectY(";
  out += ")";
}

void pretty(std::string& out, const IndirectY&, int) {
  out += "IndirectY()";
}

IndirectY read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<IndirectY>) {
  r.expect_name("IndirectY");
  return read_body_IndirectY(r);
}

void write(::c6502::asdl::Writer& w, const IndirectZp& v) {
  w.write_name("IndirectZp");
  w.open();
  w.write_int(v.address);
  w.space();
  w.write_int(v.offset);
  w.close();
}

static IndirectZp read_body_IndirectZp(::c6502::asdl::Reader& r) {
  r.open();
  ::c6502::asdl::Int f0 = r.read_int();
  ::c6502::asdl::Int f1 = r.read_int();
  r.close();
  return IndirectZp{std::move(f0), std::move(f1)};
}

void repr(std::string& out, const IndirectZp& v) {
  out += "IndirectZp(";
  out += "address=";
  out += ::c6502::asdl::int_to_string(v.address);
  out += ", offset=";
  out += ::c6502::asdl::int_to_string(v.offset);
  out += ")";
}

void pretty(std::string& out, const IndirectZp& v, int depth) {
  out += "IndirectZp(";
  newline(out, depth + 1);
  out += "address=";
  out += ::c6502::asdl::int_to_string(v.address);
  out += ',';
  newline(out, depth + 1);
  out += "offset=";
  out += ::c6502::asdl::int_to_string(v.offset);
  out += ',';
  newline(out, depth);
  out += ')';
}

IndirectZp read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<IndirectZp>) {
  r.expect_name("IndirectZp");
  return read_body_IndirectZp(r);
}

void write(::c6502::asdl::Writer& w, const IndirectZpY& v) {
  w.write_name("IndirectZpY");
  w.open();
  w.write_int(v.address);
  w.close();
}

static IndirectZpY read_body_IndirectZpY(::c6502::asdl::Reader& r) {
  r.open();
  ::c6502::asdl::Int f0 = r.read_int();
  r.close();
  return IndirectZpY{std::move(f0)};
}

void repr(std::string& out, const IndirectZpY& v) {
  out += "IndirectZpY(";
  out += "address=";
  out += ::c6502::asdl::int_to_string(v.address);
  out += ")";
}

void pretty(std::string& out, const IndirectZpY& v, int depth) {
  out += "IndirectZpY(";
  newline(out, depth + 1);
  out += "address=";
  out += ::c6502::asdl::int_to_string(v.address);
  out += ',';
  newline(out, depth);
  out += ')';
}

IndirectZpY read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<IndirectZpY>) {
  r.expect_name("IndirectZpY");
  return read_body_IndirectZpY(r);
}

void write(::c6502::asdl::Writer& w, const ZP& v) {
  w.write_name("ZP");
  w.open();
  w.write_int(v.address);
  w.space();
  w.write_int(v.offset);
  w.close();
}

static ZP read_body_ZP(::c6502::asdl::Reader& r) {
  r.open();
  ::c6502::asdl::Int f0 = r.read_int();
  ::c6502::asdl::Int f1 = r.read_int();
  r.close();
  return ZP{std::move(f0), std::move(f1)};
}

void repr(std::string& out, const ZP& v) {
  out += "ZP(";
  out += "address=";
  out += ::c6502::asdl::int_to_string(v.address);
  out += ", offset=";
  out += ::c6502::asdl::int_to_string(v.offset);
  out += ")";
}

void pretty(std::string& out, const ZP& v, int depth) {
  out += "ZP(";
  newline(out, depth + 1);
  out += "address=";
  out += ::c6502::asdl::int_to_string(v.address);
  out += ',';
  newline(out, depth + 1);
  out += "offset=";
  out += ::c6502::asdl::int_to_string(v.offset);
  out += ',';
  newline(out, depth);
  out += ')';
}

ZP read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<ZP>) {
  r.expect_name("ZP");
  return read_body_ZP(r);
}

void write(::c6502::asdl::Writer& w, const ImmLabelLow& v) {
  w.write_name("ImmLabelLow");
  w.open();
  w.write_string(v.name);
  w.space();
  w.write_int(v.offset);
  w.close();
}

static ImmLabelLow read_body_ImmLabelLow(::c6502::asdl::Reader& r) {
  r.open();
  std::string f0 = r.read_string();
  ::c6502::asdl::Int f1 = r.read_int();
  r.close();
  return ImmLabelLow{std::move(f0), std::move(f1)};
}

void repr(std::string& out, const ImmLabelLow& v) {
  out += "ImmLabelLow(";
  out += "name=";
  out += ::c6502::pyrepr::string(v.name);
  out += ", offset=";
  out += ::c6502::asdl::int_to_string(v.offset);
  out += ")";
}

void pretty(std::string& out, const ImmLabelLow& v, int depth) {
  out += "ImmLabelLow(";
  newline(out, depth + 1);
  out += "name=";
  out += ::c6502::pcpp::py_repr(v.name);
  out += ',';
  newline(out, depth + 1);
  out += "offset=";
  out += ::c6502::asdl::int_to_string(v.offset);
  out += ',';
  newline(out, depth);
  out += ')';
}

ImmLabelLow read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<ImmLabelLow>) {
  r.expect_name("ImmLabelLow");
  return read_body_ImmLabelLow(r);
}

void write(::c6502::asdl::Writer& w, const ImmLabelHigh& v) {
  w.write_name("ImmLabelHigh");
  w.open();
  w.write_string(v.name);
  w.space();
  w.write_int(v.offset);
  w.close();
}

static ImmLabelHigh read_body_ImmLabelHigh(::c6502::asdl::Reader& r) {
  r.open();
  std::string f0 = r.read_string();
  ::c6502::asdl::Int f1 = r.read_int();
  r.close();
  return ImmLabelHigh{std::move(f0), std::move(f1)};
}

void repr(std::string& out, const ImmLabelHigh& v) {
  out += "ImmLabelHigh(";
  out += "name=";
  out += ::c6502::pyrepr::string(v.name);
  out += ", offset=";
  out += ::c6502::asdl::int_to_string(v.offset);
  out += ")";
}

void pretty(std::string& out, const ImmLabelHigh& v, int depth) {
  out += "ImmLabelHigh(";
  newline(out, depth + 1);
  out += "name=";
  out += ::c6502::pcpp::py_repr(v.name);
  out += ',';
  newline(out, depth + 1);
  out += "offset=";
  out += ::c6502::asdl::int_to_string(v.offset);
  out += ',';
  newline(out, depth);
  out += ')';
}

ImmLabelHigh read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<ImmLabelHigh>) {
  r.expect_name("ImmLabelHigh");
  return read_body_ImmLabelHigh(r);
}

void write(::c6502::asdl::Writer& w, const IndexedData& v) {
  w.write_name("IndexedData");
  w.open();
  w.write_string(v.name);
  w.space();
  w.write_int(v.offset);
  w.space();
  write(w, v.index);
  w.close();
}

static IndexedData read_body_IndexedData(::c6502::asdl::Reader& r) {
  r.open();
  std::string f0 = r.read_string();
  ::c6502::asdl::Int f1 = r.read_int();
  Type_reg f2 = read(r, ::c6502::asdl::Tag<Type_reg>{});
  r.close();
  return IndexedData{std::move(f0), std::move(f1), std::move(f2)};
}

void repr(std::string& out, const IndexedData& v) {
  out += "IndexedData(";
  out += "name=";
  out += ::c6502::pyrepr::string(v.name);
  out += ", offset=";
  out += ::c6502::asdl::int_to_string(v.offset);
  out += ", index=";
  repr(out, v.index);
  out += ")";
}

void pretty(std::string& out, const IndexedData& v, int depth) {
  out += "IndexedData(";
  newline(out, depth + 1);
  out += "name=";
  out += ::c6502::pcpp::py_repr(v.name);
  out += ',';
  newline(out, depth + 1);
  out += "offset=";
  out += ::c6502::asdl::int_to_string(v.offset);
  out += ',';
  newline(out, depth + 1);
  out += "index=";
  pretty(out, v.index, depth + 1);
  out += ',';
  newline(out, depth);
  out += ')';
}

IndexedData read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<IndexedData>) {
  r.expect_name("IndexedData");
  return read_body_IndexedData(r);
}

void write(::c6502::asdl::Writer& w, const Type_operand& v) {
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

void repr(std::string& out, const Type_operand& v) {
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

void pretty(std::string& out, const Type_operand& v, int depth) {
  switch (v.index()) {
    case 0:
      pretty(out, *std::get_if<0>(&v), depth);
      return;
    case 1:
      pretty(out, *std::get_if<1>(&v), depth);
      return;
    case 2:
      pretty(out, *std::get_if<2>(&v), depth);
      return;
    case 3:
      pretty(out, *std::get_if<3>(&v), depth);
      return;
    case 4:
      pretty(out, *std::get_if<4>(&v), depth);
      return;
    case 5:
      pretty(out, *std::get_if<5>(&v), depth);
      return;
    case 6:
      pretty(out, *std::get_if<6>(&v), depth);
      return;
    case 7:
      pretty(out, *std::get_if<7>(&v), depth);
      return;
    case 8:
      pretty(out, *std::get_if<8>(&v), depth);
      return;
    case 9:
      pretty(out, *std::get_if<9>(&v), depth);
      return;
    case 10:
      pretty(out, *std::get_if<10>(&v), depth);
      return;
    case 11:
      pretty(out, *std::get_if<11>(&v), depth);
      return;
    case 12:
      pretty(out, *std::get_if<12>(&v), depth);
      return;
    case 13:
      pretty(out, *std::get_if<13>(&v), depth);
      return;
  }
}

Type_operand read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Type_operand>) {
  const std::string_view name = r.read_name();
  switch (name.size()) {
    case 2:
      if (name == "ZP") {
        return read_body_ZP(r);
      }
      break;
    case 3:
      if (name == "Imm") {
        return read_body_Imm(r);
      }
      if (name == "Reg") {
        return read_body_Reg(r);
      }
      break;
    case 4:
      if (name == "Data") {
        return read_body_Data(r);
      }
      break;
    case 5:
      if (name == "Stack") {
        return read_body_Stack(r);
      }
      if (name == "Frame") {
        return read_body_Frame(r);
      }
      break;
    case 6:
      if (name == "Pseudo") {
        return read_body_Pseudo(r);
      }
      break;
    case 8:
      if (name == "Indirect") {
        return read_body_Indirect(r);
      }
      break;
    case 9:
      if (name == "IndirectY") {
        return read_body_IndirectY(r);
      }
      break;
    case 10:
      if (name == "IndirectZp") {
        return read_body_IndirectZp(r);
      }
      break;
    case 11:
      if (name == "IndirectZpY") {
        return read_body_IndirectZpY(r);
      }
      if (name == "ImmLabelLow") {
        return read_body_ImmLabelLow(r);
      }
      if (name == "IndexedData") {
        return read_body_IndexedData(r);
      }
      break;
    case 12:
      if (name == "ImmLabelHigh") {
        return read_body_ImmLabelHigh(r);
      }
      break;
  }
  r.unknown_constructor(name, "operand");
}

void write(::c6502::asdl::Writer& w, const CC&) {
  w.write_name("CC");
}

static CC read_body_CC(::c6502::asdl::Reader& r) {
  (void)r;
  return CC{};
}

void repr(std::string& out, const CC&) {
  out += "CC(";
  out += ")";
}

void pretty(std::string& out, const CC&, int) {
  out += "CC()";
}

CC read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<CC>) {
  r.expect_name("CC");
  return read_body_CC(r);
}

void write(::c6502::asdl::Writer& w, const CS&) {
  w.write_name("CS");
}

static CS read_body_CS(::c6502::asdl::Reader& r) {
  (void)r;
  return CS{};
}

void repr(std::string& out, const CS&) {
  out += "CS(";
  out += ")";
}

void pretty(std::string& out, const CS&, int) {
  out += "CS()";
}

CS read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<CS>) {
  r.expect_name("CS");
  return read_body_CS(r);
}

void write(::c6502::asdl::Writer& w, const EQ&) {
  w.write_name("EQ");
}

static EQ read_body_EQ(::c6502::asdl::Reader& r) {
  (void)r;
  return EQ{};
}

void repr(std::string& out, const EQ&) {
  out += "EQ(";
  out += ")";
}

void pretty(std::string& out, const EQ&, int) {
  out += "EQ()";
}

EQ read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<EQ>) {
  r.expect_name("EQ");
  return read_body_EQ(r);
}

void write(::c6502::asdl::Writer& w, const MI&) {
  w.write_name("MI");
}

static MI read_body_MI(::c6502::asdl::Reader& r) {
  (void)r;
  return MI{};
}

void repr(std::string& out, const MI&) {
  out += "MI(";
  out += ")";
}

void pretty(std::string& out, const MI&, int) {
  out += "MI()";
}

MI read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<MI>) {
  r.expect_name("MI");
  return read_body_MI(r);
}

void write(::c6502::asdl::Writer& w, const NE&) {
  w.write_name("NE");
}

static NE read_body_NE(::c6502::asdl::Reader& r) {
  (void)r;
  return NE{};
}

void repr(std::string& out, const NE&) {
  out += "NE(";
  out += ")";
}

void pretty(std::string& out, const NE&, int) {
  out += "NE()";
}

NE read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<NE>) {
  r.expect_name("NE");
  return read_body_NE(r);
}

void write(::c6502::asdl::Writer& w, const PL&) {
  w.write_name("PL");
}

static PL read_body_PL(::c6502::asdl::Reader& r) {
  (void)r;
  return PL{};
}

void repr(std::string& out, const PL&) {
  out += "PL(";
  out += ")";
}

void pretty(std::string& out, const PL&, int) {
  out += "PL()";
}

PL read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<PL>) {
  r.expect_name("PL");
  return read_body_PL(r);
}

void write(::c6502::asdl::Writer& w, const VC&) {
  w.write_name("VC");
}

static VC read_body_VC(::c6502::asdl::Reader& r) {
  (void)r;
  return VC{};
}

void repr(std::string& out, const VC&) {
  out += "VC(";
  out += ")";
}

void pretty(std::string& out, const VC&, int) {
  out += "VC()";
}

VC read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<VC>) {
  r.expect_name("VC");
  return read_body_VC(r);
}

void write(::c6502::asdl::Writer& w, const VS&) {
  w.write_name("VS");
}

static VS read_body_VS(::c6502::asdl::Reader& r) {
  (void)r;
  return VS{};
}

void repr(std::string& out, const VS&) {
  out += "VS(";
  out += ")";
}

void pretty(std::string& out, const VS&, int) {
  out += "VS()";
}

VS read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<VS>) {
  r.expect_name("VS");
  return read_body_VS(r);
}

void write(::c6502::asdl::Writer& w, const Type_condition& v) {
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
  }
}

void repr(std::string& out, const Type_condition& v) {
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
  }
}

void pretty(std::string& out, const Type_condition& v, int depth) {
  switch (v.index()) {
    case 0:
      pretty(out, *std::get_if<0>(&v), depth);
      return;
    case 1:
      pretty(out, *std::get_if<1>(&v), depth);
      return;
    case 2:
      pretty(out, *std::get_if<2>(&v), depth);
      return;
    case 3:
      pretty(out, *std::get_if<3>(&v), depth);
      return;
    case 4:
      pretty(out, *std::get_if<4>(&v), depth);
      return;
    case 5:
      pretty(out, *std::get_if<5>(&v), depth);
      return;
    case 6:
      pretty(out, *std::get_if<6>(&v), depth);
      return;
    case 7:
      pretty(out, *std::get_if<7>(&v), depth);
      return;
  }
}

Type_condition read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Type_condition>) {
  const std::string_view name = r.read_name();
  switch (name.size()) {
    case 2:
      if (name == "CC") {
        return read_body_CC(r);
      }
      if (name == "CS") {
        return read_body_CS(r);
      }
      if (name == "EQ") {
        return read_body_EQ(r);
      }
      if (name == "MI") {
        return read_body_MI(r);
      }
      if (name == "NE") {
        return read_body_NE(r);
      }
      if (name == "PL") {
        return read_body_PL(r);
      }
      if (name == "VC") {
        return read_body_VC(r);
      }
      if (name == "VS") {
        return read_body_VS(r);
      }
      break;
  }
  r.unknown_constructor(name, "condition");
}

void write(::c6502::asdl::Writer& w, const AsmPhiArg& v) {
  w.write_name("AsmPhiArg");
  w.open();
  w.write_string(v.pred_label);
  w.space();
  write(w, v.source);
  w.close();
}

static AsmPhiArg read_body_AsmPhiArg(::c6502::asdl::Reader& r) {
  r.open();
  std::string f0 = r.read_string();
  Type_operand f1 = read(r, ::c6502::asdl::Tag<Type_operand>{});
  r.close();
  return AsmPhiArg{std::move(f0), std::move(f1)};
}

void repr(std::string& out, const AsmPhiArg& v) {
  out += "AsmPhiArg(";
  out += "pred_label=";
  out += ::c6502::pyrepr::string(v.pred_label);
  out += ", source=";
  repr(out, v.source);
  out += ")";
}

void pretty(std::string& out, const AsmPhiArg& v, int depth) {
  out += "AsmPhiArg(";
  newline(out, depth + 1);
  out += "pred_label=";
  out += ::c6502::pcpp::py_repr(v.pred_label);
  out += ',';
  newline(out, depth + 1);
  out += "source=";
  pretty(out, v.source, depth + 1);
  out += ',';
  newline(out, depth);
  out += ')';
}

AsmPhiArg read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<AsmPhiArg>) {
  r.expect_name("AsmPhiArg");
  return read_body_AsmPhiArg(r);
}

void write(::c6502::asdl::Writer& w, const Type_asm_phi_arg& v) {
  switch (v.index()) {
    case 0:
      write(w, *std::get_if<0>(&v));
      return;
  }
}

void repr(std::string& out, const Type_asm_phi_arg& v) {
  switch (v.index()) {
    case 0:
      repr(out, *std::get_if<0>(&v));
      return;
  }
}

void pretty(std::string& out, const Type_asm_phi_arg& v, int depth) {
  switch (v.index()) {
    case 0:
      pretty(out, *std::get_if<0>(&v), depth);
      return;
  }
}

Type_asm_phi_arg read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Type_asm_phi_arg>) {
  const std::string_view name = r.read_name();
  switch (name.size()) {
    case 9:
      if (name == "AsmPhiArg") {
        return read_body_AsmPhiArg(r);
      }
      break;
  }
  r.unknown_constructor(name, "asm_phi_arg");
}

void write(::c6502::asdl::Writer& w, const Mov& v) {
  w.write_name("Mov");
  w.open();
  write(w, v.src);
  w.space();
  write(w, v.dst);
  w.space();
  w.write_bool(v.is_volatile);
  w.close();
}

static Mov read_body_Mov(::c6502::asdl::Reader& r) {
  r.open();
  Type_operand f0 = read(r, ::c6502::asdl::Tag<Type_operand>{});
  Type_operand f1 = read(r, ::c6502::asdl::Tag<Type_operand>{});
  bool f2 = r.read_bool();
  r.close();
  return Mov{std::move(f0), std::move(f1), std::move(f2)};
}

void repr(std::string& out, const Mov& v) {
  out += "Mov(";
  out += "src=";
  repr(out, v.src);
  out += ", dst=";
  repr(out, v.dst);
  out += ", is_volatile=";
  out += v.is_volatile ? "True" : "False";
  out += ")";
}

void pretty(std::string& out, const Mov& v, int depth) {
  out += "Mov(";
  newline(out, depth + 1);
  out += "src=";
  pretty(out, v.src, depth + 1);
  out += ',';
  newline(out, depth + 1);
  out += "dst=";
  pretty(out, v.dst, depth + 1);
  out += ',';
  newline(out, depth + 1);
  out += "is_volatile=";
  out += v.is_volatile ? "True" : "False";
  out += ',';
  newline(out, depth);
  out += ')';
}

Mov read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Mov>) {
  r.expect_name("Mov");
  return read_body_Mov(r);
}

void write(::c6502::asdl::Writer& w, const Ret& v) {
  w.write_name("Ret");
  w.open();
  w.write_int(v.arg_bytes);
  w.space();
  w.write_int(v.local_bytes);
  w.space();
  w.write_bool(v.save_a);
  w.space();
  w.write_bool(v.save_x);
  w.space();
  w.begin_seq(false);
  for (const auto& e : v.callee_saved_addrs) {
    w.seq_item(false);
    w.write_int(e);
  }
  w.end_seq(false, v.callee_saved_addrs.empty());
  w.space();
  w.write_bool(v.save_c);
  w.close();
}

static Ret read_body_Ret(::c6502::asdl::Reader& r) {
  r.open();
  ::c6502::asdl::Int f0 = r.read_int();
  ::c6502::asdl::Int f1 = r.read_int();
  bool f2 = r.read_bool();
  bool f3 = r.read_bool();
  std::vector<::c6502::asdl::Int> f4;
  r.begin_seq();
  while (!r.end_seq()) {
    f4.push_back(r.read_int());
  }
  bool f5 = r.read_bool();
  r.close();
  return Ret{std::move(f0), std::move(f1), std::move(f2), std::move(f3), std::move(f4), std::move(f5)};
}

void repr(std::string& out, const Ret& v) {
  out += "Ret(";
  out += "arg_bytes=";
  out += ::c6502::asdl::int_to_string(v.arg_bytes);
  out += ", local_bytes=";
  out += ::c6502::asdl::int_to_string(v.local_bytes);
  out += ", save_a=";
  out += v.save_a ? "True" : "False";
  out += ", save_x=";
  out += v.save_x ? "True" : "False";
  out += ", callee_saved_addrs=";
  out += "[";
  {
    bool first = true;
    for (const auto& e : v.callee_saved_addrs) {
      if (!first) out += ", ";
      first = false;
      out += ::c6502::asdl::int_to_string(e);
    }
  }
  out += "]";
  out += ", save_c=";
  out += v.save_c ? "True" : "False";
  out += ")";
}

void pretty(std::string& out, const Ret& v, int depth) {
  out += "Ret(";
  newline(out, depth + 1);
  out += "arg_bytes=";
  out += ::c6502::asdl::int_to_string(v.arg_bytes);
  out += ',';
  newline(out, depth + 1);
  out += "local_bytes=";
  out += ::c6502::asdl::int_to_string(v.local_bytes);
  out += ',';
  newline(out, depth + 1);
  out += "save_a=";
  out += v.save_a ? "True" : "False";
  out += ',';
  newline(out, depth + 1);
  out += "save_x=";
  out += v.save_x ? "True" : "False";
  out += ',';
  newline(out, depth + 1);
  out += "callee_saved_addrs=";
  if (v.callee_saved_addrs.empty()) {
    out += "[]";
  } else {
    out += "[";
    for (const auto& e : v.callee_saved_addrs) {
      newline(out, depth + 2);
      out += ::c6502::asdl::int_to_string(e);
      out += ',';
    }
    newline(out, depth + 1);
    out += ']';
  }
  out += ',';
  newline(out, depth + 1);
  out += "save_c=";
  out += v.save_c ? "True" : "False";
  out += ',';
  newline(out, depth);
  out += ')';
}

Ret read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Ret>) {
  r.expect_name("Ret");
  return read_body_Ret(r);
}

void write(::c6502::asdl::Writer& w, const Return& v) {
  w.write_name("Return");
  w.open();
  w.write_bool(v.save_a);
  w.space();
  w.write_bool(v.save_x);
  w.space();
  w.write_bool(v.save_c);
  w.close();
}

static Return read_body_Return(::c6502::asdl::Reader& r) {
  r.open();
  bool f0 = r.read_bool();
  bool f1 = r.read_bool();
  bool f2 = r.read_bool();
  r.close();
  return Return{std::move(f0), std::move(f1), std::move(f2)};
}

void repr(std::string& out, const Return& v) {
  out += "Return(";
  out += "save_a=";
  out += v.save_a ? "True" : "False";
  out += ", save_x=";
  out += v.save_x ? "True" : "False";
  out += ", save_c=";
  out += v.save_c ? "True" : "False";
  out += ")";
}

void pretty(std::string& out, const Return& v, int depth) {
  out += "Return(";
  newline(out, depth + 1);
  out += "save_a=";
  out += v.save_a ? "True" : "False";
  out += ',';
  newline(out, depth + 1);
  out += "save_x=";
  out += v.save_x ? "True" : "False";
  out += ',';
  newline(out, depth + 1);
  out += "save_c=";
  out += v.save_c ? "True" : "False";
  out += ',';
  newline(out, depth);
  out += ')';
}

Return read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Return>) {
  r.expect_name("Return");
  return read_body_Return(r);
}

void write(::c6502::asdl::Writer& w, const FunctionPrologue& v) {
  w.write_name("FunctionPrologue");
  w.open();
  w.write_int(v.arg_bytes);
  w.space();
  w.write_int(v.local_bytes);
  w.space();
  w.begin_seq(false);
  for (const auto& e : v.callee_saved_addrs) {
    w.seq_item(false);
    w.write_int(e);
  }
  w.end_seq(false, v.callee_saved_addrs.empty());
  w.close();
}

static FunctionPrologue read_body_FunctionPrologue(::c6502::asdl::Reader& r) {
  r.open();
  ::c6502::asdl::Int f0 = r.read_int();
  ::c6502::asdl::Int f1 = r.read_int();
  std::vector<::c6502::asdl::Int> f2;
  r.begin_seq();
  while (!r.end_seq()) {
    f2.push_back(r.read_int());
  }
  r.close();
  return FunctionPrologue{std::move(f0), std::move(f1), std::move(f2)};
}

void repr(std::string& out, const FunctionPrologue& v) {
  out += "FunctionPrologue(";
  out += "arg_bytes=";
  out += ::c6502::asdl::int_to_string(v.arg_bytes);
  out += ", local_bytes=";
  out += ::c6502::asdl::int_to_string(v.local_bytes);
  out += ", callee_saved_addrs=";
  out += "[";
  {
    bool first = true;
    for (const auto& e : v.callee_saved_addrs) {
      if (!first) out += ", ";
      first = false;
      out += ::c6502::asdl::int_to_string(e);
    }
  }
  out += "]";
  out += ")";
}

void pretty(std::string& out, const FunctionPrologue& v, int depth) {
  out += "FunctionPrologue(";
  newline(out, depth + 1);
  out += "arg_bytes=";
  out += ::c6502::asdl::int_to_string(v.arg_bytes);
  out += ',';
  newline(out, depth + 1);
  out += "local_bytes=";
  out += ::c6502::asdl::int_to_string(v.local_bytes);
  out += ',';
  newline(out, depth + 1);
  out += "callee_saved_addrs=";
  if (v.callee_saved_addrs.empty()) {
    out += "[]";
  } else {
    out += "[";
    for (const auto& e : v.callee_saved_addrs) {
      newline(out, depth + 2);
      out += ::c6502::asdl::int_to_string(e);
      out += ',';
    }
    newline(out, depth + 1);
    out += ']';
  }
  out += ',';
  newline(out, depth);
  out += ')';
}

FunctionPrologue read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<FunctionPrologue>) {
  r.expect_name("FunctionPrologue");
  return read_body_FunctionPrologue(r);
}

void write(::c6502::asdl::Writer& w, const AllocateStack& v) {
  w.write_name("AllocateStack");
  w.open();
  w.write_int(v.bytes);
  w.close();
}

static AllocateStack read_body_AllocateStack(::c6502::asdl::Reader& r) {
  r.open();
  ::c6502::asdl::Int f0 = r.read_int();
  r.close();
  return AllocateStack{std::move(f0)};
}

void repr(std::string& out, const AllocateStack& v) {
  out += "AllocateStack(";
  out += "bytes=";
  out += ::c6502::asdl::int_to_string(v.bytes);
  out += ")";
}

void pretty(std::string& out, const AllocateStack& v, int depth) {
  out += "AllocateStack(";
  newline(out, depth + 1);
  out += "bytes=";
  out += ::c6502::asdl::int_to_string(v.bytes);
  out += ',';
  newline(out, depth);
  out += ')';
}

AllocateStack read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<AllocateStack>) {
  r.expect_name("AllocateStack");
  return read_body_AllocateStack(r);
}

void write(::c6502::asdl::Writer& w, const Add& v) {
  w.write_name("Add");
  w.open();
  write(w, v.src);
  w.space();
  write(w, v.dst);
  w.close();
}

static Add read_body_Add(::c6502::asdl::Reader& r) {
  r.open();
  Type_operand f0 = read(r, ::c6502::asdl::Tag<Type_operand>{});
  Type_operand f1 = read(r, ::c6502::asdl::Tag<Type_operand>{});
  r.close();
  return Add{std::move(f0), std::move(f1)};
}

void repr(std::string& out, const Add& v) {
  out += "Add(";
  out += "src=";
  repr(out, v.src);
  out += ", dst=";
  repr(out, v.dst);
  out += ")";
}

void pretty(std::string& out, const Add& v, int depth) {
  out += "Add(";
  newline(out, depth + 1);
  out += "src=";
  pretty(out, v.src, depth + 1);
  out += ',';
  newline(out, depth + 1);
  out += "dst=";
  pretty(out, v.dst, depth + 1);
  out += ',';
  newline(out, depth);
  out += ')';
}

Add read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Add>) {
  r.expect_name("Add");
  return read_body_Add(r);
}

void write(::c6502::asdl::Writer& w, const Sub& v) {
  w.write_name("Sub");
  w.open();
  write(w, v.src);
  w.space();
  write(w, v.dst);
  w.close();
}

static Sub read_body_Sub(::c6502::asdl::Reader& r) {
  r.open();
  Type_operand f0 = read(r, ::c6502::asdl::Tag<Type_operand>{});
  Type_operand f1 = read(r, ::c6502::asdl::Tag<Type_operand>{});
  r.close();
  return Sub{std::move(f0), std::move(f1)};
}

void repr(std::string& out, const Sub& v) {
  out += "Sub(";
  out += "src=";
  repr(out, v.src);
  out += ", dst=";
  repr(out, v.dst);
  out += ")";
}

void pretty(std::string& out, const Sub& v, int depth) {
  out += "Sub(";
  newline(out, depth + 1);
  out += "src=";
  pretty(out, v.src, depth + 1);
  out += ',';
  newline(out, depth + 1);
  out += "dst=";
  pretty(out, v.dst, depth + 1);
  out += ',';
  newline(out, depth);
  out += ')';
}

Sub read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Sub>) {
  r.expect_name("Sub");
  return read_body_Sub(r);
}

void write(::c6502::asdl::Writer& w, const Call& v) {
  w.write_name("Call");
  w.open();
  w.write_string(v.name);
  w.space();
  w.begin_seq(false);
  for (const auto& e : v.reg_args) {
    w.seq_item(false);
    w.write_string(e);
  }
  w.end_seq(false, v.reg_args.empty());
  w.space();
  w.begin_seq(false);
  for (const auto& e : v.preserves) {
    w.seq_item(false);
    w.write_string(e);
  }
  w.end_seq(false, v.preserves.empty());
  w.close();
}

static Call read_body_Call(::c6502::asdl::Reader& r) {
  r.open();
  std::string f0 = r.read_string();
  std::vector<std::string> f1;
  r.begin_seq();
  while (!r.end_seq()) {
    f1.push_back(r.read_string());
  }
  std::vector<std::string> f2;
  r.begin_seq();
  while (!r.end_seq()) {
    f2.push_back(r.read_string());
  }
  r.close();
  return Call{std::move(f0), std::move(f1), std::move(f2)};
}

void repr(std::string& out, const Call& v) {
  out += "Call(";
  out += "name=";
  out += ::c6502::pyrepr::string(v.name);
  out += ", reg_args=";
  out += "[";
  {
    bool first = true;
    for (const auto& e : v.reg_args) {
      if (!first) out += ", ";
      first = false;
      out += ::c6502::pyrepr::string(e);
    }
  }
  out += "]";
  out += ", preserves=";
  out += "[";
  {
    bool first = true;
    for (const auto& e : v.preserves) {
      if (!first) out += ", ";
      first = false;
      out += ::c6502::pyrepr::string(e);
    }
  }
  out += "]";
  out += ")";
}

void pretty(std::string& out, const Call& v, int depth) {
  out += "Call(";
  newline(out, depth + 1);
  out += "name=";
  out += ::c6502::pcpp::py_repr(v.name);
  out += ',';
  newline(out, depth + 1);
  out += "reg_args=";
  if (v.reg_args.empty()) {
    out += "[]";
  } else {
    out += "[";
    for (const auto& e : v.reg_args) {
      newline(out, depth + 2);
      out += ::c6502::pcpp::py_repr(e);
      out += ',';
    }
    newline(out, depth + 1);
    out += ']';
  }
  out += ',';
  newline(out, depth + 1);
  out += "preserves=";
  if (v.preserves.empty()) {
    out += "[]";
  } else {
    out += "[";
    for (const auto& e : v.preserves) {
      newline(out, depth + 2);
      out += ::c6502::pcpp::py_repr(e);
      out += ',';
    }
    newline(out, depth + 1);
    out += ']';
  }
  out += ',';
  newline(out, depth);
  out += ')';
}

Call read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Call>) {
  r.expect_name("Call");
  return read_body_Call(r);
}

void write(::c6502::asdl::Writer& w, const ClearCarry&) {
  w.write_name("ClearCarry");
}

static ClearCarry read_body_ClearCarry(::c6502::asdl::Reader& r) {
  (void)r;
  return ClearCarry{};
}

void repr(std::string& out, const ClearCarry&) {
  out += "ClearCarry(";
  out += ")";
}

void pretty(std::string& out, const ClearCarry&, int) {
  out += "ClearCarry()";
}

ClearCarry read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<ClearCarry>) {
  r.expect_name("ClearCarry");
  return read_body_ClearCarry(r);
}

void write(::c6502::asdl::Writer& w, const SetCarry&) {
  w.write_name("SetCarry");
}

static SetCarry read_body_SetCarry(::c6502::asdl::Reader& r) {
  (void)r;
  return SetCarry{};
}

void repr(std::string& out, const SetCarry&) {
  out += "SetCarry(";
  out += ")";
}

void pretty(std::string& out, const SetCarry&, int) {
  out += "SetCarry()";
}

SetCarry read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<SetCarry>) {
  r.expect_name("SetCarry");
  return read_body_SetCarry(r);
}

void write(::c6502::asdl::Writer& w, const Inc& v) {
  w.write_name("Inc");
  w.open();
  write(w, v.dst);
  w.close();
}

static Inc read_body_Inc(::c6502::asdl::Reader& r) {
  r.open();
  Type_operand f0 = read(r, ::c6502::asdl::Tag<Type_operand>{});
  r.close();
  return Inc{std::move(f0)};
}

void repr(std::string& out, const Inc& v) {
  out += "Inc(";
  out += "dst=";
  repr(out, v.dst);
  out += ")";
}

void pretty(std::string& out, const Inc& v, int depth) {
  out += "Inc(";
  newline(out, depth + 1);
  out += "dst=";
  pretty(out, v.dst, depth + 1);
  out += ',';
  newline(out, depth);
  out += ')';
}

Inc read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Inc>) {
  r.expect_name("Inc");
  return read_body_Inc(r);
}

void write(::c6502::asdl::Writer& w, const Dec& v) {
  w.write_name("Dec");
  w.open();
  write(w, v.dst);
  w.close();
}

static Dec read_body_Dec(::c6502::asdl::Reader& r) {
  r.open();
  Type_operand f0 = read(r, ::c6502::asdl::Tag<Type_operand>{});
  r.close();
  return Dec{std::move(f0)};
}

void repr(std::string& out, const Dec& v) {
  out += "Dec(";
  out += "dst=";
  repr(out, v.dst);
  out += ")";
}

void pretty(std::string& out, const Dec& v, int depth) {
  out += "Dec(";
  newline(out, depth + 1);
  out += "dst=";
  pretty(out, v.dst, depth + 1);
  out += ',';
  newline(out, depth);
  out += ')';
}

Dec read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Dec>) {
  r.expect_name("Dec");
  return read_body_Dec(r);
}

void write(::c6502::asdl::Writer& w, const Push& v) {
  w.write_name("Push");
  w.open();
  write(w, v.src);
  w.close();
}

static Push read_body_Push(::c6502::asdl::Reader& r) {
  r.open();
  Type_operand f0 = read(r, ::c6502::asdl::Tag<Type_operand>{});
  r.close();
  return Push{std::move(f0)};
}

void repr(std::string& out, const Push& v) {
  out += "Push(";
  out += "src=";
  repr(out, v.src);
  out += ")";
}

void pretty(std::string& out, const Push& v, int depth) {
  out += "Push(";
  newline(out, depth + 1);
  out += "src=";
  pretty(out, v.src, depth + 1);
  out += ',';
  newline(out, depth);
  out += ')';
}

Push read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Push>) {
  r.expect_name("Push");
  return read_body_Push(r);
}

void write(::c6502::asdl::Writer& w, const Pop& v) {
  w.write_name("Pop");
  w.open();
  write(w, v.dst);
  w.close();
}

static Pop read_body_Pop(::c6502::asdl::Reader& r) {
  r.open();
  Type_operand f0 = read(r, ::c6502::asdl::Tag<Type_operand>{});
  r.close();
  return Pop{std::move(f0)};
}

void repr(std::string& out, const Pop& v) {
  out += "Pop(";
  out += "dst=";
  repr(out, v.dst);
  out += ")";
}

void pretty(std::string& out, const Pop& v, int depth) {
  out += "Pop(";
  newline(out, depth + 1);
  out += "dst=";
  pretty(out, v.dst, depth + 1);
  out += ',';
  newline(out, depth);
  out += ')';
}

Pop read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Pop>) {
  r.expect_name("Pop");
  return read_body_Pop(r);
}

void write(::c6502::asdl::Writer& w, const Xor& v) {
  w.write_name("Xor");
  w.open();
  write(w, v.src1);
  w.space();
  write(w, v.src2);
  w.space();
  write(w, v.dst);
  w.close();
}

static Xor read_body_Xor(::c6502::asdl::Reader& r) {
  r.open();
  Type_operand f0 = read(r, ::c6502::asdl::Tag<Type_operand>{});
  Type_operand f1 = read(r, ::c6502::asdl::Tag<Type_operand>{});
  Type_operand f2 = read(r, ::c6502::asdl::Tag<Type_operand>{});
  r.close();
  return Xor{std::move(f0), std::move(f1), std::move(f2)};
}

void repr(std::string& out, const Xor& v) {
  out += "Xor(";
  out += "src1=";
  repr(out, v.src1);
  out += ", src2=";
  repr(out, v.src2);
  out += ", dst=";
  repr(out, v.dst);
  out += ")";
}

void pretty(std::string& out, const Xor& v, int depth) {
  out += "Xor(";
  newline(out, depth + 1);
  out += "src1=";
  pretty(out, v.src1, depth + 1);
  out += ',';
  newline(out, depth + 1);
  out += "src2=";
  pretty(out, v.src2, depth + 1);
  out += ',';
  newline(out, depth + 1);
  out += "dst=";
  pretty(out, v.dst, depth + 1);
  out += ',';
  newline(out, depth);
  out += ')';
}

Xor read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Xor>) {
  r.expect_name("Xor");
  return read_body_Xor(r);
}

void write(::c6502::asdl::Writer& w, const And& v) {
  w.write_name("And");
  w.open();
  write(w, v.src);
  w.space();
  write(w, v.dst);
  w.close();
}

static And read_body_And(::c6502::asdl::Reader& r) {
  r.open();
  Type_operand f0 = read(r, ::c6502::asdl::Tag<Type_operand>{});
  Type_operand f1 = read(r, ::c6502::asdl::Tag<Type_operand>{});
  r.close();
  return And{std::move(f0), std::move(f1)};
}

void repr(std::string& out, const And& v) {
  out += "And(";
  out += "src=";
  repr(out, v.src);
  out += ", dst=";
  repr(out, v.dst);
  out += ")";
}

void pretty(std::string& out, const And& v, int depth) {
  out += "And(";
  newline(out, depth + 1);
  out += "src=";
  pretty(out, v.src, depth + 1);
  out += ',';
  newline(out, depth + 1);
  out += "dst=";
  pretty(out, v.dst, depth + 1);
  out += ',';
  newline(out, depth);
  out += ')';
}

And read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<And>) {
  r.expect_name("And");
  return read_body_And(r);
}

void write(::c6502::asdl::Writer& w, const Or& v) {
  w.write_name("Or");
  w.open();
  write(w, v.src);
  w.space();
  write(w, v.dst);
  w.close();
}

static Or read_body_Or(::c6502::asdl::Reader& r) {
  r.open();
  Type_operand f0 = read(r, ::c6502::asdl::Tag<Type_operand>{});
  Type_operand f1 = read(r, ::c6502::asdl::Tag<Type_operand>{});
  r.close();
  return Or{std::move(f0), std::move(f1)};
}

void repr(std::string& out, const Or& v) {
  out += "Or(";
  out += "src=";
  repr(out, v.src);
  out += ", dst=";
  repr(out, v.dst);
  out += ")";
}

void pretty(std::string& out, const Or& v, int depth) {
  out += "Or(";
  newline(out, depth + 1);
  out += "src=";
  pretty(out, v.src, depth + 1);
  out += ',';
  newline(out, depth + 1);
  out += "dst=";
  pretty(out, v.dst, depth + 1);
  out += ',';
  newline(out, depth);
  out += ')';
}

Or read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Or>) {
  r.expect_name("Or");
  return read_body_Or(r);
}

void write(::c6502::asdl::Writer& w, const ArithmeticShiftLeft& v) {
  w.write_name("ArithmeticShiftLeft");
  w.open();
  write(w, v.dst);
  w.close();
}

static ArithmeticShiftLeft read_body_ArithmeticShiftLeft(::c6502::asdl::Reader& r) {
  r.open();
  Type_operand f0 = read(r, ::c6502::asdl::Tag<Type_operand>{});
  r.close();
  return ArithmeticShiftLeft{std::move(f0)};
}

void repr(std::string& out, const ArithmeticShiftLeft& v) {
  out += "ArithmeticShiftLeft(";
  out += "dst=";
  repr(out, v.dst);
  out += ")";
}

void pretty(std::string& out, const ArithmeticShiftLeft& v, int depth) {
  out += "ArithmeticShiftLeft(";
  newline(out, depth + 1);
  out += "dst=";
  pretty(out, v.dst, depth + 1);
  out += ',';
  newline(out, depth);
  out += ')';
}

ArithmeticShiftLeft read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<ArithmeticShiftLeft>) {
  r.expect_name("ArithmeticShiftLeft");
  return read_body_ArithmeticShiftLeft(r);
}

void write(::c6502::asdl::Writer& w, const LogicalShiftRight& v) {
  w.write_name("LogicalShiftRight");
  w.open();
  write(w, v.dst);
  w.close();
}

static LogicalShiftRight read_body_LogicalShiftRight(::c6502::asdl::Reader& r) {
  r.open();
  Type_operand f0 = read(r, ::c6502::asdl::Tag<Type_operand>{});
  r.close();
  return LogicalShiftRight{std::move(f0)};
}

void repr(std::string& out, const LogicalShiftRight& v) {
  out += "LogicalShiftRight(";
  out += "dst=";
  repr(out, v.dst);
  out += ")";
}

void pretty(std::string& out, const LogicalShiftRight& v, int depth) {
  out += "LogicalShiftRight(";
  newline(out, depth + 1);
  out += "dst=";
  pretty(out, v.dst, depth + 1);
  out += ',';
  newline(out, depth);
  out += ')';
}

LogicalShiftRight read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<LogicalShiftRight>) {
  r.expect_name("LogicalShiftRight");
  return read_body_LogicalShiftRight(r);
}

void write(::c6502::asdl::Writer& w, const RotateLeft& v) {
  w.write_name("RotateLeft");
  w.open();
  write(w, v.dst);
  w.close();
}

static RotateLeft read_body_RotateLeft(::c6502::asdl::Reader& r) {
  r.open();
  Type_operand f0 = read(r, ::c6502::asdl::Tag<Type_operand>{});
  r.close();
  return RotateLeft{std::move(f0)};
}

void repr(std::string& out, const RotateLeft& v) {
  out += "RotateLeft(";
  out += "dst=";
  repr(out, v.dst);
  out += ")";
}

void pretty(std::string& out, const RotateLeft& v, int depth) {
  out += "RotateLeft(";
  newline(out, depth + 1);
  out += "dst=";
  pretty(out, v.dst, depth + 1);
  out += ',';
  newline(out, depth);
  out += ')';
}

RotateLeft read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<RotateLeft>) {
  r.expect_name("RotateLeft");
  return read_body_RotateLeft(r);
}

void write(::c6502::asdl::Writer& w, const RotateRight& v) {
  w.write_name("RotateRight");
  w.open();
  write(w, v.dst);
  w.close();
}

static RotateRight read_body_RotateRight(::c6502::asdl::Reader& r) {
  r.open();
  Type_operand f0 = read(r, ::c6502::asdl::Tag<Type_operand>{});
  r.close();
  return RotateRight{std::move(f0)};
}

void repr(std::string& out, const RotateRight& v) {
  out += "RotateRight(";
  out += "dst=";
  repr(out, v.dst);
  out += ")";
}

void pretty(std::string& out, const RotateRight& v, int depth) {
  out += "RotateRight(";
  newline(out, depth + 1);
  out += "dst=";
  pretty(out, v.dst, depth + 1);
  out += ',';
  newline(out, depth);
  out += ')';
}

RotateRight read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<RotateRight>) {
  r.expect_name("RotateRight");
  return read_body_RotateRight(r);
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

void pretty(std::string& out, const Label& v, int depth) {
  out += "Label(";
  newline(out, depth + 1);
  out += "name=";
  out += ::c6502::pcpp::py_repr(v.name);
  out += ',';
  newline(out, depth);
  out += ')';
}

Label read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Label>) {
  r.expect_name("Label");
  return read_body_Label(r);
}

void write(::c6502::asdl::Writer& w, const Jump& v) {
  w.write_name("Jump");
  w.open();
  w.write_string(v.target);
  w.space();
  w.begin_seq(false);
  for (const auto& e : v.reg_args) {
    w.seq_item(false);
    w.write_string(e);
  }
  w.end_seq(false, v.reg_args.empty());
  w.close();
}

static Jump read_body_Jump(::c6502::asdl::Reader& r) {
  r.open();
  std::string f0 = r.read_string();
  std::vector<std::string> f1;
  r.begin_seq();
  while (!r.end_seq()) {
    f1.push_back(r.read_string());
  }
  r.close();
  return Jump{std::move(f0), std::move(f1)};
}

void repr(std::string& out, const Jump& v) {
  out += "Jump(";
  out += "target=";
  out += ::c6502::pyrepr::string(v.target);
  out += ", reg_args=";
  out += "[";
  {
    bool first = true;
    for (const auto& e : v.reg_args) {
      if (!first) out += ", ";
      first = false;
      out += ::c6502::pyrepr::string(e);
    }
  }
  out += "]";
  out += ")";
}

void pretty(std::string& out, const Jump& v, int depth) {
  out += "Jump(";
  newline(out, depth + 1);
  out += "target=";
  out += ::c6502::pcpp::py_repr(v.target);
  out += ',';
  newline(out, depth + 1);
  out += "reg_args=";
  if (v.reg_args.empty()) {
    out += "[]";
  } else {
    out += "[";
    for (const auto& e : v.reg_args) {
      newline(out, depth + 2);
      out += ::c6502::pcpp::py_repr(e);
      out += ',';
    }
    newline(out, depth + 1);
    out += ']';
  }
  out += ',';
  newline(out, depth);
  out += ')';
}

Jump read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Jump>) {
  r.expect_name("Jump");
  return read_body_Jump(r);
}

void write(::c6502::asdl::Writer& w, const Branch& v) {
  w.write_name("Branch");
  w.open();
  write(w, v.cond);
  w.space();
  w.write_string(v.target);
  w.close();
}

static Branch read_body_Branch(::c6502::asdl::Reader& r) {
  r.open();
  Type_condition f0 = read(r, ::c6502::asdl::Tag<Type_condition>{});
  std::string f1 = r.read_string();
  r.close();
  return Branch{std::move(f0), std::move(f1)};
}

void repr(std::string& out, const Branch& v) {
  out += "Branch(";
  out += "cond=";
  repr(out, v.cond);
  out += ", target=";
  out += ::c6502::pyrepr::string(v.target);
  out += ")";
}

void pretty(std::string& out, const Branch& v, int depth) {
  out += "Branch(";
  newline(out, depth + 1);
  out += "cond=";
  pretty(out, v.cond, depth + 1);
  out += ',';
  newline(out, depth + 1);
  out += "target=";
  out += ::c6502::pcpp::py_repr(v.target);
  out += ',';
  newline(out, depth);
  out += ')';
}

Branch read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Branch>) {
  r.expect_name("Branch");
  return read_body_Branch(r);
}

void write(::c6502::asdl::Writer& w, const JumpTable& v) {
  w.write_name("JumpTable");
  w.open();
  w.write_string(v.table);
  w.space();
  w.write_string(v.index);
  w.space();
  w.write_int(v.base);
  w.space();
  w.begin_seq(false);
  for (const auto& e : v.targets) {
    w.seq_item(false);
    w.write_string(e);
  }
  w.end_seq(false, v.targets.empty());
  w.close();
}

static JumpTable read_body_JumpTable(::c6502::asdl::Reader& r) {
  r.open();
  std::string f0 = r.read_string();
  std::string f1 = r.read_string();
  ::c6502::asdl::Int f2 = r.read_int();
  std::vector<std::string> f3;
  r.begin_seq();
  while (!r.end_seq()) {
    f3.push_back(r.read_string());
  }
  r.close();
  return JumpTable{std::move(f0), std::move(f1), std::move(f2), std::move(f3)};
}

void repr(std::string& out, const JumpTable& v) {
  out += "JumpTable(";
  out += "table=";
  out += ::c6502::pyrepr::string(v.table);
  out += ", index=";
  out += ::c6502::pyrepr::string(v.index);
  out += ", base=";
  out += ::c6502::asdl::int_to_string(v.base);
  out += ", targets=";
  out += "[";
  {
    bool first = true;
    for (const auto& e : v.targets) {
      if (!first) out += ", ";
      first = false;
      out += ::c6502::pyrepr::string(e);
    }
  }
  out += "]";
  out += ")";
}

void pretty(std::string& out, const JumpTable& v, int depth) {
  out += "JumpTable(";
  newline(out, depth + 1);
  out += "table=";
  out += ::c6502::pcpp::py_repr(v.table);
  out += ',';
  newline(out, depth + 1);
  out += "index=";
  out += ::c6502::pcpp::py_repr(v.index);
  out += ',';
  newline(out, depth + 1);
  out += "base=";
  out += ::c6502::asdl::int_to_string(v.base);
  out += ',';
  newline(out, depth + 1);
  out += "targets=";
  if (v.targets.empty()) {
    out += "[]";
  } else {
    out += "[";
    for (const auto& e : v.targets) {
      newline(out, depth + 2);
      out += ::c6502::pcpp::py_repr(e);
      out += ',';
    }
    newline(out, depth + 1);
    out += ']';
  }
  out += ',';
  newline(out, depth);
  out += ')';
}

JumpTable read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<JumpTable>) {
  r.expect_name("JumpTable");
  return read_body_JumpTable(r);
}

void write(::c6502::asdl::Writer& w, const DecimalArith& v) {
  w.write_name("DecimalArith");
  w.open();
  w.write_bool(v.subtract);
  w.space();
  w.begin_seq(false);
  for (const auto& e : v.left) {
    w.seq_item(false);
    write(w, e);
  }
  w.end_seq(false, v.left.empty());
  w.space();
  w.begin_seq(false);
  for (const auto& e : v.right) {
    w.seq_item(false);
    write(w, e);
  }
  w.end_seq(false, v.right.empty());
  w.space();
  w.begin_seq(false);
  for (const auto& e : v.dst) {
    w.seq_item(false);
    write(w, e);
  }
  w.end_seq(false, v.dst.empty());
  w.close();
}

static DecimalArith read_body_DecimalArith(::c6502::asdl::Reader& r) {
  r.open();
  bool f0 = r.read_bool();
  std::vector<Type_operand> f1;
  r.begin_seq();
  while (!r.end_seq()) {
    f1.push_back(read(r, ::c6502::asdl::Tag<Type_operand>{}));
  }
  std::vector<Type_operand> f2;
  r.begin_seq();
  while (!r.end_seq()) {
    f2.push_back(read(r, ::c6502::asdl::Tag<Type_operand>{}));
  }
  std::vector<Type_operand> f3;
  r.begin_seq();
  while (!r.end_seq()) {
    f3.push_back(read(r, ::c6502::asdl::Tag<Type_operand>{}));
  }
  r.close();
  return DecimalArith{std::move(f0), std::move(f1), std::move(f2), std::move(f3)};
}

void repr(std::string& out, const DecimalArith& v) {
  out += "DecimalArith(";
  out += "subtract=";
  out += v.subtract ? "True" : "False";
  out += ", left=";
  out += "[";
  {
    bool first = true;
    for (const auto& e : v.left) {
      if (!first) out += ", ";
      first = false;
      repr(out, e);
    }
  }
  out += "]";
  out += ", right=";
  out += "[";
  {
    bool first = true;
    for (const auto& e : v.right) {
      if (!first) out += ", ";
      first = false;
      repr(out, e);
    }
  }
  out += "]";
  out += ", dst=";
  out += "[";
  {
    bool first = true;
    for (const auto& e : v.dst) {
      if (!first) out += ", ";
      first = false;
      repr(out, e);
    }
  }
  out += "]";
  out += ")";
}

void pretty(std::string& out, const DecimalArith& v, int depth) {
  out += "DecimalArith(";
  newline(out, depth + 1);
  out += "subtract=";
  out += v.subtract ? "True" : "False";
  out += ',';
  newline(out, depth + 1);
  out += "left=";
  if (v.left.empty()) {
    out += "[]";
  } else {
    out += "[";
    for (const auto& e : v.left) {
      newline(out, depth + 2);
      pretty(out, e, depth + 2);
      out += ',';
    }
    newline(out, depth + 1);
    out += ']';
  }
  out += ',';
  newline(out, depth + 1);
  out += "right=";
  if (v.right.empty()) {
    out += "[]";
  } else {
    out += "[";
    for (const auto& e : v.right) {
      newline(out, depth + 2);
      pretty(out, e, depth + 2);
      out += ',';
    }
    newline(out, depth + 1);
    out += ']';
  }
  out += ',';
  newline(out, depth + 1);
  out += "dst=";
  if (v.dst.empty()) {
    out += "[]";
  } else {
    out += "[";
    for (const auto& e : v.dst) {
      newline(out, depth + 2);
      pretty(out, e, depth + 2);
      out += ',';
    }
    newline(out, depth + 1);
    out += ']';
  }
  out += ',';
  newline(out, depth);
  out += ')';
}

DecimalArith read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<DecimalArith>) {
  r.expect_name("DecimalArith");
  return read_body_DecimalArith(r);
}

void write(::c6502::asdl::Writer& w, const SetDecimal&) {
  w.write_name("SetDecimal");
}

static SetDecimal read_body_SetDecimal(::c6502::asdl::Reader& r) {
  (void)r;
  return SetDecimal{};
}

void repr(std::string& out, const SetDecimal&) {
  out += "SetDecimal(";
  out += ")";
}

void pretty(std::string& out, const SetDecimal&, int) {
  out += "SetDecimal()";
}

SetDecimal read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<SetDecimal>) {
  r.expect_name("SetDecimal");
  return read_body_SetDecimal(r);
}

void write(::c6502::asdl::Writer& w, const ClearDecimal&) {
  w.write_name("ClearDecimal");
}

static ClearDecimal read_body_ClearDecimal(::c6502::asdl::Reader& r) {
  (void)r;
  return ClearDecimal{};
}

void repr(std::string& out, const ClearDecimal&) {
  out += "ClearDecimal(";
  out += ")";
}

void pretty(std::string& out, const ClearDecimal&, int) {
  out += "ClearDecimal()";
}

ClearDecimal read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<ClearDecimal>) {
  r.expect_name("ClearDecimal");
  return read_body_ClearDecimal(r);
}

void write(::c6502::asdl::Writer& w, const Compare& v) {
  w.write_name("Compare");
  w.open();
  write(w, v.left);
  w.space();
  write(w, v.right);
  w.close();
}

static Compare read_body_Compare(::c6502::asdl::Reader& r) {
  r.open();
  Type_operand f0 = read(r, ::c6502::asdl::Tag<Type_operand>{});
  Type_operand f1 = read(r, ::c6502::asdl::Tag<Type_operand>{});
  r.close();
  return Compare{std::move(f0), std::move(f1)};
}

void repr(std::string& out, const Compare& v) {
  out += "Compare(";
  out += "left=";
  repr(out, v.left);
  out += ", right=";
  repr(out, v.right);
  out += ")";
}

void pretty(std::string& out, const Compare& v, int depth) {
  out += "Compare(";
  newline(out, depth + 1);
  out += "left=";
  pretty(out, v.left, depth + 1);
  out += ',';
  newline(out, depth + 1);
  out += "right=";
  pretty(out, v.right, depth + 1);
  out += ',';
  newline(out, depth);
  out += ')';
}

Compare read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Compare>) {
  r.expect_name("Compare");
  return read_body_Compare(r);
}

void write(::c6502::asdl::Writer& w, const BitTest& v) {
  w.write_name("BitTest");
  w.open();
  write(w, v.src);
  w.close();
}

static BitTest read_body_BitTest(::c6502::asdl::Reader& r) {
  r.open();
  Type_operand f0 = read(r, ::c6502::asdl::Tag<Type_operand>{});
  r.close();
  return BitTest{std::move(f0)};
}

void repr(std::string& out, const BitTest& v) {
  out += "BitTest(";
  out += "src=";
  repr(out, v.src);
  out += ")";
}

void pretty(std::string& out, const BitTest& v, int depth) {
  out += "BitTest(";
  newline(out, depth + 1);
  out += "src=";
  pretty(out, v.src, depth + 1);
  out += ',';
  newline(out, depth);
  out += ')';
}

BitTest read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<BitTest>) {
  r.expect_name("BitTest");
  return read_body_BitTest(r);
}

void write(::c6502::asdl::Writer& w, const LoadAddress& v) {
  w.write_name("LoadAddress");
  w.open();
  write(w, v.src);
  w.space();
  write(w, v.dst);
  w.close();
}

static LoadAddress read_body_LoadAddress(::c6502::asdl::Reader& r) {
  r.open();
  Type_operand f0 = read(r, ::c6502::asdl::Tag<Type_operand>{});
  Type_operand f1 = read(r, ::c6502::asdl::Tag<Type_operand>{});
  r.close();
  return LoadAddress{std::move(f0), std::move(f1)};
}

void repr(std::string& out, const LoadAddress& v) {
  out += "LoadAddress(";
  out += "src=";
  repr(out, v.src);
  out += ", dst=";
  repr(out, v.dst);
  out += ")";
}

void pretty(std::string& out, const LoadAddress& v, int depth) {
  out += "LoadAddress(";
  newline(out, depth + 1);
  out += "src=";
  pretty(out, v.src, depth + 1);
  out += ',';
  newline(out, depth + 1);
  out += "dst=";
  pretty(out, v.dst, depth + 1);
  out += ',';
  newline(out, depth);
  out += ')';
}

LoadAddress read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<LoadAddress>) {
  r.expect_name("LoadAddress");
  return read_body_LoadAddress(r);
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
  Type_operand f0 = read(r, ::c6502::asdl::Tag<Type_operand>{});
  std::vector<Type_asm_phi_arg> f1;
  r.begin_seq();
  while (!r.end_seq()) {
    f1.push_back(read(r, ::c6502::asdl::Tag<Type_asm_phi_arg>{}));
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

void pretty(std::string& out, const Phi& v, int depth) {
  out += "Phi(";
  newline(out, depth + 1);
  out += "dst=";
  pretty(out, v.dst, depth + 1);
  out += ',';
  newline(out, depth + 1);
  out += "args=";
  if (v.args.empty()) {
    out += "[]";
  } else {
    out += "[";
    for (const auto& e : v.args) {
      newline(out, depth + 2);
      pretty(out, e, depth + 2);
      out += ',';
    }
    newline(out, depth + 1);
    out += ']';
  }
  out += ',';
  newline(out, depth);
  out += ')';
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
    case 31:
      write(w, *std::get_if<31>(&v));
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
    case 31:
      repr(out, *std::get_if<31>(&v));
      return;
  }
}

void pretty(std::string& out, const Type_instruction& v, int depth) {
  switch (v.index()) {
    case 0:
      pretty(out, *std::get_if<0>(&v), depth);
      return;
    case 1:
      pretty(out, *std::get_if<1>(&v), depth);
      return;
    case 2:
      pretty(out, *std::get_if<2>(&v), depth);
      return;
    case 3:
      pretty(out, *std::get_if<3>(&v), depth);
      return;
    case 4:
      pretty(out, *std::get_if<4>(&v), depth);
      return;
    case 5:
      pretty(out, *std::get_if<5>(&v), depth);
      return;
    case 6:
      pretty(out, *std::get_if<6>(&v), depth);
      return;
    case 7:
      pretty(out, *std::get_if<7>(&v), depth);
      return;
    case 8:
      pretty(out, *std::get_if<8>(&v), depth);
      return;
    case 9:
      pretty(out, *std::get_if<9>(&v), depth);
      return;
    case 10:
      pretty(out, *std::get_if<10>(&v), depth);
      return;
    case 11:
      pretty(out, *std::get_if<11>(&v), depth);
      return;
    case 12:
      pretty(out, *std::get_if<12>(&v), depth);
      return;
    case 13:
      pretty(out, *std::get_if<13>(&v), depth);
      return;
    case 14:
      pretty(out, *std::get_if<14>(&v), depth);
      return;
    case 15:
      pretty(out, *std::get_if<15>(&v), depth);
      return;
    case 16:
      pretty(out, *std::get_if<16>(&v), depth);
      return;
    case 17:
      pretty(out, *std::get_if<17>(&v), depth);
      return;
    case 18:
      pretty(out, *std::get_if<18>(&v), depth);
      return;
    case 19:
      pretty(out, *std::get_if<19>(&v), depth);
      return;
    case 20:
      pretty(out, *std::get_if<20>(&v), depth);
      return;
    case 21:
      pretty(out, *std::get_if<21>(&v), depth);
      return;
    case 22:
      pretty(out, *std::get_if<22>(&v), depth);
      return;
    case 23:
      pretty(out, *std::get_if<23>(&v), depth);
      return;
    case 24:
      pretty(out, *std::get_if<24>(&v), depth);
      return;
    case 25:
      pretty(out, *std::get_if<25>(&v), depth);
      return;
    case 26:
      pretty(out, *std::get_if<26>(&v), depth);
      return;
    case 27:
      pretty(out, *std::get_if<27>(&v), depth);
      return;
    case 28:
      pretty(out, *std::get_if<28>(&v), depth);
      return;
    case 29:
      pretty(out, *std::get_if<29>(&v), depth);
      return;
    case 30:
      pretty(out, *std::get_if<30>(&v), depth);
      return;
    case 31:
      pretty(out, *std::get_if<31>(&v), depth);
      return;
  }
}

Type_instruction read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Type_instruction>) {
  const std::string_view name = r.read_name();
  switch (name.size()) {
    case 2:
      if (name == "Or") {
        return read_body_Or(r);
      }
      break;
    case 3:
      if (name == "Mov") {
        return read_body_Mov(r);
      }
      if (name == "Ret") {
        return read_body_Ret(r);
      }
      if (name == "Add") {
        return read_body_Add(r);
      }
      if (name == "Sub") {
        return read_body_Sub(r);
      }
      if (name == "Inc") {
        return read_body_Inc(r);
      }
      if (name == "Dec") {
        return read_body_Dec(r);
      }
      if (name == "Pop") {
        return read_body_Pop(r);
      }
      if (name == "Xor") {
        return read_body_Xor(r);
      }
      if (name == "And") {
        return read_body_And(r);
      }
      if (name == "Phi") {
        return read_body_Phi(r);
      }
      break;
    case 4:
      if (name == "Call") {
        return read_body_Call(r);
      }
      if (name == "Push") {
        return read_body_Push(r);
      }
      if (name == "Jump") {
        return read_body_Jump(r);
      }
      break;
    case 5:
      if (name == "Label") {
        return read_body_Label(r);
      }
      break;
    case 6:
      if (name == "Return") {
        return read_body_Return(r);
      }
      if (name == "Branch") {
        return read_body_Branch(r);
      }
      break;
    case 7:
      if (name == "Compare") {
        return read_body_Compare(r);
      }
      if (name == "BitTest") {
        return read_body_BitTest(r);
      }
      break;
    case 8:
      if (name == "SetCarry") {
        return read_body_SetCarry(r);
      }
      break;
    case 9:
      if (name == "JumpTable") {
        return read_body_JumpTable(r);
      }
      break;
    case 10:
      if (name == "ClearCarry") {
        return read_body_ClearCarry(r);
      }
      if (name == "RotateLeft") {
        return read_body_RotateLeft(r);
      }
      if (name == "SetDecimal") {
        return read_body_SetDecimal(r);
      }
      break;
    case 11:
      if (name == "RotateRight") {
        return read_body_RotateRight(r);
      }
      if (name == "LoadAddress") {
        return read_body_LoadAddress(r);
      }
      break;
    case 12:
      if (name == "DecimalArith") {
        return read_body_DecimalArith(r);
      }
      if (name == "ClearDecimal") {
        return read_body_ClearDecimal(r);
      }
      break;
    case 13:
      if (name == "AllocateStack") {
        return read_body_AllocateStack(r);
      }
      break;
    case 16:
      if (name == "FunctionPrologue") {
        return read_body_FunctionPrologue(r);
      }
      break;
    case 17:
      if (name == "LogicalShiftRight") {
        return read_body_LogicalShiftRight(r);
      }
      break;
    case 19:
      if (name == "ArithmeticShiftLeft") {
        return read_body_ArithmeticShiftLeft(r);
      }
      break;
  }
  r.unknown_constructor(name, "instruction");
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

void pretty(std::string& out, const CharInit& v, int depth) {
  out += "CharInit(";
  newline(out, depth + 1);
  out += "value=";
  out += ::c6502::asdl::int_to_string(v.value);
  out += ',';
  newline(out, depth);
  out += ')';
}

CharInit read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<CharInit>) {
  r.expect_name("CharInit");
  return read_body_CharInit(r);
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

void pretty(std::string& out, const IntInit& v, int depth) {
  out += "IntInit(";
  newline(out, depth + 1);
  out += "value=";
  out += ::c6502::asdl::int_to_string(v.value);
  out += ',';
  newline(out, depth);
  out += ')';
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

void pretty(std::string& out, const LongInit& v, int depth) {
  out += "LongInit(";
  newline(out, depth + 1);
  out += "value=";
  out += ::c6502::asdl::int_to_string(v.value);
  out += ',';
  newline(out, depth);
  out += ')';
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

void pretty(std::string& out, const LongLongInit& v, int depth) {
  out += "LongLongInit(";
  newline(out, depth + 1);
  out += "value=";
  out += ::c6502::asdl::int_to_string(v.value);
  out += ',';
  newline(out, depth);
  out += ')';
}

LongLongInit read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<LongLongInit>) {
  r.expect_name("LongLongInit");
  return read_body_LongLongInit(r);
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

void pretty(std::string& out, const FloatInit& v, int depth) {
  out += "FloatInit(";
  newline(out, depth + 1);
  out += "bits=";
  out += ::c6502::asdl::int_to_string(v.bits);
  out += ',';
  newline(out, depth);
  out += ')';
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

void pretty(std::string& out, const DoubleInit& v, int depth) {
  out += "DoubleInit(";
  newline(out, depth + 1);
  out += "bits=";
  out += ::c6502::asdl::int_to_string(v.bits);
  out += ',';
  newline(out, depth);
  out += ')';
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

void pretty(std::string& out, const AddressInit& v, int depth) {
  out += "AddressInit(";
  newline(out, depth + 1);
  out += "name=";
  out += ::c6502::pcpp::py_repr(v.name);
  out += ',';
  newline(out, depth + 1);
  out += "offset=";
  out += ::c6502::asdl::int_to_string(v.offset);
  out += ',';
  newline(out, depth);
  out += ')';
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

void pretty(std::string& out, const AddressByteInit& v, int depth) {
  out += "AddressByteInit(";
  newline(out, depth + 1);
  out += "name=";
  out += ::c6502::pcpp::py_repr(v.name);
  out += ',';
  newline(out, depth + 1);
  out += "offset=";
  out += ::c6502::asdl::int_to_string(v.offset);
  out += ',';
  newline(out, depth + 1);
  out += "high=";
  out += v.high ? "True" : "False";
  out += ',';
  newline(out, depth);
  out += ')';
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

void pretty(std::string& out, const StringInit& v, int depth) {
  out += "StringInit(";
  newline(out, depth + 1);
  out += "str=";
  out += ::c6502::pcpp::py_repr(v.str);
  out += ',';
  newline(out, depth + 1);
  out += "bytes=";
  out += ::c6502::asdl::int_to_string(v.bytes);
  out += ',';
  newline(out, depth);
  out += ')';
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

void pretty(std::string& out, const ZeroInit& v, int depth) {
  out += "ZeroInit(";
  newline(out, depth + 1);
  out += "bytes=";
  out += ::c6502::asdl::int_to_string(v.bytes);
  out += ',';
  newline(out, depth);
  out += ')';
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
  }
}

void pretty(std::string& out, const Type_static_init& v, int depth) {
  switch (v.index()) {
    case 0:
      pretty(out, *std::get_if<0>(&v), depth);
      return;
    case 1:
      pretty(out, *std::get_if<1>(&v), depth);
      return;
    case 2:
      pretty(out, *std::get_if<2>(&v), depth);
      return;
    case 3:
      pretty(out, *std::get_if<3>(&v), depth);
      return;
    case 4:
      pretty(out, *std::get_if<4>(&v), depth);
      return;
    case 5:
      pretty(out, *std::get_if<5>(&v), depth);
      return;
    case 6:
      pretty(out, *std::get_if<6>(&v), depth);
      return;
    case 7:
      pretty(out, *std::get_if<7>(&v), depth);
      return;
    case 8:
      pretty(out, *std::get_if<8>(&v), depth);
      return;
    case 9:
      pretty(out, *std::get_if<9>(&v), depth);
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
      if (name == "ZeroInit") {
        return read_body_ZeroInit(r);
      }
      break;
    case 9:
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

void pretty(std::string& out, const Function& v, int depth) {
  out += "Function(";
  newline(out, depth + 1);
  out += "name=";
  out += ::c6502::pcpp::py_repr(v.name);
  out += ',';
  newline(out, depth + 1);
  out += "is_global=";
  out += v.is_global ? "True" : "False";
  out += ',';
  newline(out, depth + 1);
  out += "params=";
  if (v.params.empty()) {
    out += "[]";
  } else {
    out += "[";
    for (const auto& e : v.params) {
      newline(out, depth + 2);
      out += ::c6502::pcpp::py_repr(e);
      out += ',';
    }
    newline(out, depth + 1);
    out += ']';
  }
  out += ',';
  newline(out, depth + 1);
  out += "instructions=";
  if (v.instructions.empty()) {
    out += "[]";
  } else {
    out += "[";
    for (const auto& e : v.instructions) {
      newline(out, depth + 2);
      pretty(out, e, depth + 2);
      out += ',';
    }
    newline(out, depth + 1);
    out += ']';
  }
  out += ',';
  newline(out, depth);
  out += ')';
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
  std::vector<Type_static_init> f2;
  r.begin_seq();
  while (!r.end_seq()) {
    f2.push_back(read(r, ::c6502::asdl::Tag<Type_static_init>{}));
  }
  r.close();
  return StaticVariable{std::move(f0), std::move(f1), std::move(f2)};
}

void repr(std::string& out, const StaticVariable& v) {
  out += "StaticVariable(";
  out += "name=";
  out += ::c6502::pyrepr::string(v.name);
  out += ", is_global=";
  out += v.is_global ? "True" : "False";
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

void pretty(std::string& out, const StaticVariable& v, int depth) {
  out += "StaticVariable(";
  newline(out, depth + 1);
  out += "name=";
  out += ::c6502::pcpp::py_repr(v.name);
  out += ',';
  newline(out, depth + 1);
  out += "is_global=";
  out += v.is_global ? "True" : "False";
  out += ',';
  newline(out, depth + 1);
  out += "init=";
  if (v.init.empty()) {
    out += "[]";
  } else {
    out += "[";
    for (const auto& e : v.init) {
      newline(out, depth + 2);
      pretty(out, e, depth + 2);
      out += ',';
    }
    newline(out, depth + 1);
    out += ']';
  }
  out += ',';
  newline(out, depth);
  out += ')';
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

void pretty(std::string& out, const Type_top_level& v, int depth) {
  switch (v.index()) {
    case 0:
      pretty(out, *std::get_if<0>(&v), depth);
      return;
    case 1:
      pretty(out, *std::get_if<1>(&v), depth);
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

void pretty(std::string& out, const Program& v, int depth) {
  out += "Program(";
  newline(out, depth + 1);
  out += "top_level=";
  if (v.top_level.empty()) {
    out += "[]";
  } else {
    out += "[";
    for (const auto& e : v.top_level) {
      newline(out, depth + 2);
      pretty(out, e, depth + 2);
      out += ',';
    }
    newline(out, depth + 1);
    out += ']';
  }
  out += ',';
  newline(out, depth);
  out += ')';
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

void pretty(std::string& out, const Type_program& v, int depth) {
  switch (v.index()) {
    case 0:
      pretty(out, *std::get_if<0>(&v), depth);
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

}  // namespace c6502::asm_ast
