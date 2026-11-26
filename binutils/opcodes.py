"""The 151 documented opcodes of the NMOS 6502, which the simulator's
stand-ins of the Apple II decode with (sim/lode_runner.py and
sim/ali_baba.py). The assembler and the disassembler use the C++ copy,
cpp/src/binutils/opcodes.h.

`OPCODES` maps a mnemonic and an addressing mode to the opcode byte,
and `DECODE` maps each opcode byte back. The modes are:

    imp   implied              CLC
    acc   accumulator          ASL       (dasm also takes `ASL A`)
    imm   immediate            LDA #$01
    zp    zero page            LDA $80
    zpx   zero page,X          LDA $80,X
    zpy   zero page,Y          LDX $80,Y
    abs   absolute             LDA $1234
    absx  absolute,X           LDA $1234,X
    absy  absolute,Y           LDA $1234,Y
    ind   indirect             JMP ($1234)
    indx  (zero page,X)        LDA ($80,X)
    indy  (zero page),Y        LDA ($80),Y
    rel   relative             BNE label

The table is from the MOS MCS6500 programming manual's instruction
summary. The undocumented NMOS opcodes, and the 65C02's, aren't in it.
"""
from __future__ import annotations

# Bytes each mode takes, opcode included.
SIZES = {
    "imp": 1, "acc": 1, "imm": 2, "zp": 2, "zpx": 2, "zpy": 2,
    "abs": 3, "absx": 3, "absy": 3, "ind": 3, "indx": 2, "indy": 2,
    "rel": 2,
}

_TABLE = {
    "ADC": {"imm": 0x69, "zp": 0x65, "zpx": 0x75, "abs": 0x6D,
            "absx": 0x7D, "absy": 0x79, "indx": 0x61, "indy": 0x71},
    "AND": {"imm": 0x29, "zp": 0x25, "zpx": 0x35, "abs": 0x2D,
            "absx": 0x3D, "absy": 0x39, "indx": 0x21, "indy": 0x31},
    "ASL": {"acc": 0x0A, "zp": 0x06, "zpx": 0x16, "abs": 0x0E,
            "absx": 0x1E},
    "BCC": {"rel": 0x90}, "BCS": {"rel": 0xB0}, "BEQ": {"rel": 0xF0},
    "BIT": {"zp": 0x24, "abs": 0x2C},
    "BMI": {"rel": 0x30}, "BNE": {"rel": 0xD0}, "BPL": {"rel": 0x10},
    "BRK": {"imp": 0x00},
    "BVC": {"rel": 0x50}, "BVS": {"rel": 0x70},
    "CLC": {"imp": 0x18}, "CLD": {"imp": 0xD8}, "CLI": {"imp": 0x58},
    "CLV": {"imp": 0xB8},
    "CMP": {"imm": 0xC9, "zp": 0xC5, "zpx": 0xD5, "abs": 0xCD,
            "absx": 0xDD, "absy": 0xD9, "indx": 0xC1, "indy": 0xD1},
    "CPX": {"imm": 0xE0, "zp": 0xE4, "abs": 0xEC},
    "CPY": {"imm": 0xC0, "zp": 0xC4, "abs": 0xCC},
    "DEC": {"zp": 0xC6, "zpx": 0xD6, "abs": 0xCE, "absx": 0xDE},
    "DEX": {"imp": 0xCA}, "DEY": {"imp": 0x88},
    "EOR": {"imm": 0x49, "zp": 0x45, "zpx": 0x55, "abs": 0x4D,
            "absx": 0x5D, "absy": 0x59, "indx": 0x41, "indy": 0x51},
    "INC": {"zp": 0xE6, "zpx": 0xF6, "abs": 0xEE, "absx": 0xFE},
    "INX": {"imp": 0xE8}, "INY": {"imp": 0xC8},
    "JMP": {"abs": 0x4C, "ind": 0x6C},
    "JSR": {"abs": 0x20},
    "LDA": {"imm": 0xA9, "zp": 0xA5, "zpx": 0xB5, "abs": 0xAD,
            "absx": 0xBD, "absy": 0xB9, "indx": 0xA1, "indy": 0xB1},
    "LDX": {"imm": 0xA2, "zp": 0xA6, "zpy": 0xB6, "abs": 0xAE,
            "absy": 0xBE},
    "LDY": {"imm": 0xA0, "zp": 0xA4, "zpx": 0xB4, "abs": 0xAC,
            "absx": 0xBC},
    "LSR": {"acc": 0x4A, "zp": 0x46, "zpx": 0x56, "abs": 0x4E,
            "absx": 0x5E},
    "NOP": {"imp": 0xEA},
    "ORA": {"imm": 0x09, "zp": 0x05, "zpx": 0x15, "abs": 0x0D,
            "absx": 0x1D, "absy": 0x19, "indx": 0x01, "indy": 0x11},
    "PHA": {"imp": 0x48}, "PHP": {"imp": 0x08},
    "PLA": {"imp": 0x68}, "PLP": {"imp": 0x28},
    "ROL": {"acc": 0x2A, "zp": 0x26, "zpx": 0x36, "abs": 0x2E,
            "absx": 0x3E},
    "ROR": {"acc": 0x6A, "zp": 0x66, "zpx": 0x76, "abs": 0x6E,
            "absx": 0x7E},
    "RTI": {"imp": 0x40}, "RTS": {"imp": 0x60},
    "SBC": {"imm": 0xE9, "zp": 0xE5, "zpx": 0xF5, "abs": 0xED,
            "absx": 0xFD, "absy": 0xF9, "indx": 0xE1, "indy": 0xF1},
    "SEC": {"imp": 0x38}, "SED": {"imp": 0xF8}, "SEI": {"imp": 0x78},
    "STA": {"zp": 0x85, "zpx": 0x95, "abs": 0x8D, "absx": 0x9D,
            "absy": 0x99, "indx": 0x81, "indy": 0x91},
    "STX": {"zp": 0x86, "zpy": 0x96, "abs": 0x8E},
    "STY": {"zp": 0x84, "zpx": 0x94, "abs": 0x8C},
    "TAX": {"imp": 0xAA}, "TAY": {"imp": 0xA8}, "TSX": {"imp": 0xBA},
    "TXA": {"imp": 0x8A}, "TXS": {"imp": 0x9A}, "TYA": {"imp": 0x98},
}

OPCODES: dict[tuple[str, str], int] = {
    (mnemonic, mode): op
    for mnemonic, modes in _TABLE.items() for mode, op in modes.items()
}

MNEMONICS = frozenset(_TABLE)

DECODE: dict[int, tuple[str, str]] = {op: key for key, op in OPCODES.items()}

assert len(OPCODES) == 151 and len(DECODE) == 151

# The zero-page form of each absolute mode, where the instruction has
# one.
ZP_FORM = {"abs": "zp", "absx": "zpx", "absy": "zpy"}


def modes(mnemonic: str) -> dict[str, int]:
    """The modes `mnemonic` has, with their opcodes."""
    return _TABLE[mnemonic]
