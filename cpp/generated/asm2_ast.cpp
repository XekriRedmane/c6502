// Generated from asm2.asdl by `asdl.py --cpp`. Do not edit.
#include "asm2_ast.h"

#include <utility>

namespace c6502::asm2_ast {

void write(::c6502::asdl::Writer& w, const A&) {
  w.write_name("A");
}

static A read_body_A(::c6502::asdl::Reader& r) {
  (void)r;
  return A{};
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

Reg read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Reg>) {
  r.expect_name("Reg");
  return read_body_Reg(r);
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

void write(::c6502::asdl::Writer& w, const Mov& v) {
  w.write_name("Mov");
  w.open();
  write(w, v.src);
  w.space();
  write(w, v.dst);
  w.close();
}

static Mov read_body_Mov(::c6502::asdl::Reader& r) {
  r.open();
  Type_operand f0 = read(r, ::c6502::asdl::Tag<Type_operand>{});
  Type_operand f1 = read(r, ::c6502::asdl::Tag<Type_operand>{});
  r.close();
  return Mov{std::move(f0), std::move(f1)};
}

Mov read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Mov>) {
  r.expect_name("Mov");
  return read_body_Mov(r);
}

void write(::c6502::asdl::Writer& w, const Return&) {
  w.write_name("Return");
}

static Return read_body_Return(::c6502::asdl::Reader& r) {
  (void)r;
  return Return{};
}

Return read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Return>) {
  r.expect_name("Return");
  return read_body_Return(r);
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

Sub read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Sub>) {
  r.expect_name("Sub");
  return read_body_Sub(r);
}

void write(::c6502::asdl::Writer& w, const Call& v) {
  w.write_name("Call");
  w.open();
  w.write_string(v.name);
  w.close();
}

static Call read_body_Call(::c6502::asdl::Reader& r) {
  r.open();
  std::string f0 = r.read_string();
  r.close();
  return Call{std::move(f0)};
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

Pop read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Pop>) {
  r.expect_name("Pop");
  return read_body_Pop(r);
}

void write(::c6502::asdl::Writer& w, const SetDecimal&) {
  w.write_name("SetDecimal");
}

static SetDecimal read_body_SetDecimal(::c6502::asdl::Reader& r) {
  (void)r;
  return SetDecimal{};
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

ClearDecimal read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<ClearDecimal>) {
  r.expect_name("ClearDecimal");
  return read_body_ClearDecimal(r);
}

void write(::c6502::asdl::Writer& w, const PushStatus&) {
  w.write_name("PushStatus");
}

static PushStatus read_body_PushStatus(::c6502::asdl::Reader& r) {
  (void)r;
  return PushStatus{};
}

PushStatus read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<PushStatus>) {
  r.expect_name("PushStatus");
  return read_body_PushStatus(r);
}

void write(::c6502::asdl::Writer& w, const PullStatus&) {
  w.write_name("PullStatus");
}

static PullStatus read_body_PullStatus(::c6502::asdl::Reader& r) {
  (void)r;
  return PullStatus{};
}

PullStatus read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<PullStatus>) {
  r.expect_name("PullStatus");
  return read_body_PullStatus(r);
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

Label read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Label>) {
  r.expect_name("Label");
  return read_body_Label(r);
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

JumpTable read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<JumpTable>) {
  r.expect_name("JumpTable");
  return read_body_JumpTable(r);
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

LoadAddress read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<LoadAddress>) {
  r.expect_name("LoadAddress");
  return read_body_LoadAddress(r);
}

void write(::c6502::asdl::Writer& w, const Comment& v) {
  w.write_name("Comment");
  w.open();
  w.write_string(v.text);
  w.close();
}

static Comment read_body_Comment(::c6502::asdl::Reader& r) {
  r.open();
  std::string f0 = r.read_string();
  r.close();
  return Comment{std::move(f0)};
}

Comment read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Comment>) {
  r.expect_name("Comment");
  return read_body_Comment(r);
}

void write(::c6502::asdl::Writer& w, const Blank&) {
  w.write_name("Blank");
}

static Blank read_body_Blank(::c6502::asdl::Reader& r) {
  (void)r;
  return Blank{};
}

Blank read(::c6502::asdl::Reader& r, ::c6502::asdl::Tag<Blank>) {
  r.expect_name("Blank");
  return read_body_Blank(r);
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
      if (name == "Blank") {
        return read_body_Blank(r);
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
      if (name == "Comment") {
        return read_body_Comment(r);
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
      if (name == "SetDecimal") {
        return read_body_SetDecimal(r);
      }
      if (name == "PushStatus") {
        return read_body_PushStatus(r);
      }
      if (name == "PullStatus") {
        return read_body_PullStatus(r);
      }
      if (name == "RotateLeft") {
        return read_body_RotateLeft(r);
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
      if (name == "ClearDecimal") {
        return read_body_ClearDecimal(r);
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

}  // namespace c6502::asm2_ast
