#include "binutils/opcodes.h"

#include <array>

namespace c6502::binutils::opcodes {

namespace {

using M = Mode;

constexpr Entry kTable[] = {
    {"ADC", M::kImm, 0x69},
    {"ADC", M::kZp, 0x65},
    {"ADC", M::kZpx, 0x75},
    {"ADC", M::kAbs, 0x6D},
    {"ADC", M::kAbsx, 0x7D},
    {"ADC", M::kAbsy, 0x79},
    {"ADC", M::kIndx, 0x61},
    {"ADC", M::kIndy, 0x71},
    {"AND", M::kImm, 0x29},
    {"AND", M::kZp, 0x25},
    {"AND", M::kZpx, 0x35},
    {"AND", M::kAbs, 0x2D},
    {"AND", M::kAbsx, 0x3D},
    {"AND", M::kAbsy, 0x39},
    {"AND", M::kIndx, 0x21},
    {"AND", M::kIndy, 0x31},
    {"ASL", M::kAcc, 0x0A},
    {"ASL", M::kZp, 0x06},
    {"ASL", M::kZpx, 0x16},
    {"ASL", M::kAbs, 0x0E},
    {"ASL", M::kAbsx, 0x1E},
    {"BCC", M::kRel, 0x90},
    {"BCS", M::kRel, 0xB0},
    {"BEQ", M::kRel, 0xF0},
    {"BIT", M::kZp, 0x24},
    {"BIT", M::kAbs, 0x2C},
    {"BMI", M::kRel, 0x30},
    {"BNE", M::kRel, 0xD0},
    {"BPL", M::kRel, 0x10},
    {"BRK", M::kImp, 0x00},
    {"BVC", M::kRel, 0x50},
    {"BVS", M::kRel, 0x70},
    {"CLC", M::kImp, 0x18},
    {"CLD", M::kImp, 0xD8},
    {"CLI", M::kImp, 0x58},
    {"CLV", M::kImp, 0xB8},
    {"CMP", M::kImm, 0xC9},
    {"CMP", M::kZp, 0xC5},
    {"CMP", M::kZpx, 0xD5},
    {"CMP", M::kAbs, 0xCD},
    {"CMP", M::kAbsx, 0xDD},
    {"CMP", M::kAbsy, 0xD9},
    {"CMP", M::kIndx, 0xC1},
    {"CMP", M::kIndy, 0xD1},
    {"CPX", M::kImm, 0xE0},
    {"CPX", M::kZp, 0xE4},
    {"CPX", M::kAbs, 0xEC},
    {"CPY", M::kImm, 0xC0},
    {"CPY", M::kZp, 0xC4},
    {"CPY", M::kAbs, 0xCC},
    {"DEC", M::kZp, 0xC6},
    {"DEC", M::kZpx, 0xD6},
    {"DEC", M::kAbs, 0xCE},
    {"DEC", M::kAbsx, 0xDE},
    {"DEX", M::kImp, 0xCA},
    {"DEY", M::kImp, 0x88},
    {"EOR", M::kImm, 0x49},
    {"EOR", M::kZp, 0x45},
    {"EOR", M::kZpx, 0x55},
    {"EOR", M::kAbs, 0x4D},
    {"EOR", M::kAbsx, 0x5D},
    {"EOR", M::kAbsy, 0x59},
    {"EOR", M::kIndx, 0x41},
    {"EOR", M::kIndy, 0x51},
    {"INC", M::kZp, 0xE6},
    {"INC", M::kZpx, 0xF6},
    {"INC", M::kAbs, 0xEE},
    {"INC", M::kAbsx, 0xFE},
    {"INX", M::kImp, 0xE8},
    {"INY", M::kImp, 0xC8},
    {"JMP", M::kAbs, 0x4C},
    {"JMP", M::kInd, 0x6C},
    {"JSR", M::kAbs, 0x20},
    {"LDA", M::kImm, 0xA9},
    {"LDA", M::kZp, 0xA5},
    {"LDA", M::kZpx, 0xB5},
    {"LDA", M::kAbs, 0xAD},
    {"LDA", M::kAbsx, 0xBD},
    {"LDA", M::kAbsy, 0xB9},
    {"LDA", M::kIndx, 0xA1},
    {"LDA", M::kIndy, 0xB1},
    {"LDX", M::kImm, 0xA2},
    {"LDX", M::kZp, 0xA6},
    {"LDX", M::kZpy, 0xB6},
    {"LDX", M::kAbs, 0xAE},
    {"LDX", M::kAbsy, 0xBE},
    {"LDY", M::kImm, 0xA0},
    {"LDY", M::kZp, 0xA4},
    {"LDY", M::kZpx, 0xB4},
    {"LDY", M::kAbs, 0xAC},
    {"LDY", M::kAbsx, 0xBC},
    {"LSR", M::kAcc, 0x4A},
    {"LSR", M::kZp, 0x46},
    {"LSR", M::kZpx, 0x56},
    {"LSR", M::kAbs, 0x4E},
    {"LSR", M::kAbsx, 0x5E},
    {"NOP", M::kImp, 0xEA},
    {"ORA", M::kImm, 0x09},
    {"ORA", M::kZp, 0x05},
    {"ORA", M::kZpx, 0x15},
    {"ORA", M::kAbs, 0x0D},
    {"ORA", M::kAbsx, 0x1D},
    {"ORA", M::kAbsy, 0x19},
    {"ORA", M::kIndx, 0x01},
    {"ORA", M::kIndy, 0x11},
    {"PHA", M::kImp, 0x48},
    {"PHP", M::kImp, 0x08},
    {"PLA", M::kImp, 0x68},
    {"PLP", M::kImp, 0x28},
    {"ROL", M::kAcc, 0x2A},
    {"ROL", M::kZp, 0x26},
    {"ROL", M::kZpx, 0x36},
    {"ROL", M::kAbs, 0x2E},
    {"ROL", M::kAbsx, 0x3E},
    {"ROR", M::kAcc, 0x6A},
    {"ROR", M::kZp, 0x66},
    {"ROR", M::kZpx, 0x76},
    {"ROR", M::kAbs, 0x6E},
    {"ROR", M::kAbsx, 0x7E},
    {"RTI", M::kImp, 0x40},
    {"RTS", M::kImp, 0x60},
    {"SBC", M::kImm, 0xE9},
    {"SBC", M::kZp, 0xE5},
    {"SBC", M::kZpx, 0xF5},
    {"SBC", M::kAbs, 0xED},
    {"SBC", M::kAbsx, 0xFD},
    {"SBC", M::kAbsy, 0xF9},
    {"SBC", M::kIndx, 0xE1},
    {"SBC", M::kIndy, 0xF1},
    {"SEC", M::kImp, 0x38},
    {"SED", M::kImp, 0xF8},
    {"SEI", M::kImp, 0x78},
    {"STA", M::kZp, 0x85},
    {"STA", M::kZpx, 0x95},
    {"STA", M::kAbs, 0x8D},
    {"STA", M::kAbsx, 0x9D},
    {"STA", M::kAbsy, 0x99},
    {"STA", M::kIndx, 0x81},
    {"STA", M::kIndy, 0x91},
    {"STX", M::kZp, 0x86},
    {"STX", M::kZpy, 0x96},
    {"STX", M::kAbs, 0x8E},
    {"STY", M::kZp, 0x84},
    {"STY", M::kZpx, 0x94},
    {"STY", M::kAbs, 0x8C},
    {"TAX", M::kImp, 0xAA},
    {"TAY", M::kImp, 0xA8},
    {"TSX", M::kImp, 0xBA},
    {"TXA", M::kImp, 0x8A},
    {"TXS", M::kImp, 0x9A},
    {"TYA", M::kImp, 0x98},
};

static_assert(std::size(kTable) == 151);

std::array<int, 256> decode_table() {
  std::array<int, 256> out;
  out.fill(-1);
  for (std::size_t i = 0; i < std::size(kTable); ++i) {
    out[kTable[i].opcode] = static_cast<int>(i);
  }
  return out;
}

}  // namespace

std::string_view mode_name(Mode mode) {
  switch (mode) {
    case Mode::kImp:
      return "imp";
    case Mode::kAcc:
      return "acc";
    case Mode::kImm:
      return "imm";
    case Mode::kZp:
      return "zp";
    case Mode::kZpx:
      return "zpx";
    case Mode::kZpy:
      return "zpy";
    case Mode::kAbs:
      return "abs";
    case Mode::kAbsx:
      return "absx";
    case Mode::kAbsy:
      return "absy";
    case Mode::kInd:
      return "ind";
    case Mode::kIndx:
      return "indx";
    case Mode::kIndy:
      return "indy";
    case Mode::kRel:
      return "rel";
  }
  return "";
}

int size(Mode mode) {
  switch (mode) {
    case Mode::kImp:
    case Mode::kAcc:
      return 1;
    case Mode::kImm:
    case Mode::kZp:
    case Mode::kZpx:
    case Mode::kZpy:
    case Mode::kIndx:
    case Mode::kIndy:
    case Mode::kRel:
      return 2;
    case Mode::kAbs:
    case Mode::kAbsx:
    case Mode::kAbsy:
    case Mode::kInd:
      return 3;
  }
  return 0;
}

std::span<const Entry> table() { return kTable; }

const Entry* decode(std::uint8_t op) {
  static const std::array<int, 256> rows = decode_table();
  const int i = rows[op];
  return i < 0 ? nullptr : &kTable[i];
}

std::optional<std::uint8_t> opcode(std::string_view mnemonic, Mode mode) {
  for (const Entry& e : kTable) {
    if (e.mnemonic == mnemonic && e.mode == mode) return e.opcode;
  }
  return std::nullopt;
}

bool is_mnemonic(std::string_view mnemonic) {
  for (const Entry& e : kTable) {
    if (e.mnemonic == mnemonic) return true;
  }
  return false;
}

std::optional<Mode> zp_form(Mode mode) {
  switch (mode) {
    case Mode::kAbs:
      return Mode::kZp;
    case Mode::kAbsx:
      return Mode::kZpx;
    case Mode::kAbsy:
      return Mode::kZpy;
    default:
      return std::nullopt;
  }
}

}  // namespace c6502::binutils::opcodes
