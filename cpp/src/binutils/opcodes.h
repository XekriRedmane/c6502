// The 151 documented opcodes of the NMOS 6502, for the assembler and
// the disassembler (binutils/opcodes.py).
//
// `opcode` maps a mnemonic and an addressing mode to the opcode byte,
// and `decode` maps each opcode byte back. The modes are:
//
//     imp   implied              CLC
//     acc   accumulator          ASL       (dasm also takes `ASL A`)
//     imm   immediate            LDA #$01
//     zp    zero page            LDA $80
//     zpx   zero page,X          LDA $80,X
//     zpy   zero page,Y          LDX $80,Y
//     abs   absolute             LDA $1234
//     absx  absolute,X           LDA $1234,X
//     absy  absolute,Y           LDA $1234,Y
//     ind   indirect             JMP ($1234)
//     indx  (zero page,X)        LDA ($80,X)
//     indy  (zero page),Y        LDA ($80),Y
//     rel   relative             BNE label
//
// The table is from the MOS MCS6500 programming manual's instruction
// summary. The undocumented NMOS opcodes, and the 65C02's, aren't in
// it. The simulator's assembler (sim/assembler.py, and its
// `instruction_size` in src/sim/assembler.h) has tables of its own for
// the subset it assembles.
#pragma once

#include <cstdint>
#include <optional>
#include <span>
#include <string_view>

namespace c6502::binutils::opcodes {

enum class Mode : std::uint8_t {
  kImp,
  kAcc,
  kImm,
  kZp,
  kZpx,
  kZpy,
  kAbs,
  kAbsx,
  kAbsy,
  kInd,
  kIndx,
  kIndy,
  kRel,
};

// Returns the mode's name in the Python table (`"absx"`).
std::string_view mode_name(Mode mode);

// Returns the number of bytes an instruction in `mode` takes, opcode
// included (`SIZES`).
int size(Mode mode);

// One row of the table: a mnemonic, a mode, and its opcode.
struct Entry {
  std::string_view mnemonic;
  Mode mode;
  std::uint8_t opcode;
};

// Returns every row, in the order of the Python table (`OPCODES`).
std::span<const Entry> table();

// Returns the mnemonic and mode of opcode byte `op`, or null for a byte
// that isn't a documented opcode (`DECODE`).
const Entry* decode(std::uint8_t op);

// Returns the opcode of `mnemonic` in `mode`, or none when it has no
// such mode.
std::optional<std::uint8_t> opcode(std::string_view mnemonic, Mode mode);

// Returns true when `mnemonic` is in the table (`MNEMONICS`).
bool is_mnemonic(std::string_view mnemonic);

// Returns the zero-page form of an absolute mode (`ZP_FORM`), or none
// for the other modes.
std::optional<Mode> zp_form(Mode mode);

}  // namespace c6502::binutils::opcodes
