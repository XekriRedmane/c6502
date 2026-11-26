# binutils/CLAUDE.md

c6502's object-file tools are written in C++, in cpp/src/binutils/
(cpp/README.md, "The object-file tools"): the ELF reader and writer
(`elf.*`), the opcode table (`opcodes.*`), the assembler that
`compile.py -c` runs (`assembler.*`), the generic half of linking
(`ld.*`), the disassembler behind `objdump.py -d` (`disasm.*`), and nm
and objdump themselves (`nm.*`, `objdump.*`). The linker's zero-page
allocation and the layout by a script are in cpp/src/passes/
(`linker.*`, `link_script.*`, `link_layout.*`). nm.py and objdump.py at
the root are shims over `c6502cpp nm` and `c6502cpp objdump`.

This directory keeps the two Python modules that the simulator reads
until step 9 of the port (docs/cpp_port.md).

- `elf.py` — `ElfFile`, `Section`, `Symbol`, `Reloc`, `Segment`, and
  `read` / `write` for ELF32, little-endian, which sim/linked.py,
  sim/lode_runner.py, sim/ali_baba.py, and the tests read executables
  with. The constants are llvm-mos's: `EM_MOS` (6502),
  `EF_MOS_ARCH_6502`, `STO_MOS_ZEROPAGE`, and the `R_MOS_*` relocation
  numbers from its `ELFRelocs/MOS.def`. Relocations are `SHT_RELA`.
  `write` puts local symbols first, as ELF requires, and builds
  `.symtab`, `.strtab`, `.shstrtab`, and a `.rela<name>` section for
  each section with relocations; those aren't in `ElfFile.sections`.
  `ElfError` is the error of a file it can't read, which errors.py
  re-exports.
- `opcodes.py` — the 151 documented NMOS opcodes, by mnemonic and
  addressing mode (`OPCODES`) and back (`DECODE`), with each mode's
  size and the zero-page form of each absolute mode. sim/lode_runner.py
  and sim/ali_baba.py decode with `DECODE`.

A change to either module has to be made in cpp/src/binutils/ too,
since the C++ tools write the files these read.

The rest of this file describes the formats and the rules that the C++
tools follow.

## The object format

| Section | What it holds |
|---|---|
| one per item, named for its section (`.text`, `.rodata`, `.data`, `.bss`, or a `[[c6502::section]]` name) | a function's or object's bytes; `.bss` ones are `SHT_NOBITS` |
| `.rela<name>` | that item's relocations |
| `.c6502.link` | the link metadata block, as text (cpp/src/passes/zp_link_metadata.h) |
| `.c6502.asm` | the assembly text the object came from |
| `.symtab`, `.strtab`, `.shstrtab` | as usual |

Several sections share a name, as GNU as gives them with `unique,N`.
The linker matches a script's patterns against the section name in the
item's metadata line, which is the same name.

Symbols:

- An item's label is `STT_FUNC` or `STT_OBJECT`, global unless the
  metadata marks it internal (a `static` or a string literal).
- A `.` label is a local `STT_NOTYPE` symbol. dasm scopes these to a
  `SUBROUTINE`, and every function starts one, so two functions of an
  object can both have a `.loop@3`.
- `$d` and `$c` are ARM-style mapping symbols marking where data (a
  jump table) starts and code starts again inside a function.
- An `EQU` with a fixed address (`address(addr)`, a ROM routine) is a
  global `SHN_ABS` symbol. Every object that names it carries it, and
  the link fails when two give it different values.
- The names the linker places are undefined in the object, whatever
  the unit's `EQU`s say: SSP, FP, HARGS, DPTR, every `__zpabi_*` and
  `__local_*` slot, and the unit's zero-page objects (the `zp` lines
  of its metadata). One the unit reaches with zero-page addressing
  has `STO_MOS_ZEROPAGE`.
- An internal name that another object also uses is renamed `name.k`
  for the `k`th object of the link (`rename_locals` in
  cpp/src/passes/linker_metadata.h), in its
  symbols and its metadata. A renamed function's slots, which are
  undefined symbols spelled after the function, are renamed with it,
  and so are the call edges and shares that name it
  (`rename_meta`), so two objects' `static` functions of one
  name get slots of their own.
- An item whose metadata line has `merge=true` (a half of a table of
  row addresses, `A@lo` or `A@hi`) is kept once among the items so
  marked whose sections have the same contents
  (`merge_identical` in cpp/src/passes/linker_objects.h, after `rename_locals` and
  `ld::relax`). The contents are the bytes and the relocations, with
  each relocation's symbol taken as its name when it's undefined or
  global and as its object and name when it's local. The first
  object's copy stays, and its symbol becomes global. Each later copy's
  section goes, and its symbol becomes an undefined reference with the
  kept one's name, so its object's relocations and their addends
  resolve to the kept copy.

## Relocations

| Type | Field | Value | Written for |
|---|---|---|---|
| `R_MOS_IMM8` | 1 byte | S + A, which has to fit a byte | `#name`, `DC.B name` |
| `R_MOS_ADDR8` | 1 byte | S + A, below $100 | zero-page, `zp,X`, `(zp),Y` operands |
| `R_MOS_ADDR16` | 2 bytes | S + A | absolute operands, `JMP`, `JSR`, `DC.W` |
| `R_MOS_ADDR16_LO` | 1 byte | (S + A) & $FF | `#<name`, `#<(name+k)`, `DC.B <name` |
| `R_MOS_ADDR16_HI` | 1 byte | (S + A) >> 8 | `#>name`, `DC.B >name` |
| `R_MOS_PCREL_8` | 1 byte | S + A - P, with A = -1 | a branch to another item |

The assembler resolves a branch to a label in the same item itself.
Every relocated field holds 0 in the object.

## Addressing modes across units

The assembler picks zero-page addressing for an operand whose value it
knows to be below $100: a number, an `EQU`, or a runtime name. A label
or an undefined name gets absolute addressing. The linker then fixes
the mode to the program's addresses, in two directions:

- `ld::relax` shortens an absolute instruction whose operand the linker
  put in zero page, such as a global another unit's `-O` promoted. It
  deletes the operand's high byte, re-aims the branches in the
  section, which only get shorter, and moves the relocations, labels,
  and mapping symbols after it.
- A unit that reached a name with zero-page addressing that the
  program puts above $FF is assembled again from `.c6502.asm` with the
  program's addresses (`reassemble` in cpp/src/passes/linker_objects.h).

With both, an executable has the bytes dasm would assemble from the
units' text with the program's addresses, which is what the linker
produced when it wrote dasm text. That was checked on the three Zork
examples, function by function, before their checked-in listings were
replaced.

## The disassembly

`objdump.py -d` on an object writes the unit back as dasm text, with
each relocation's symbol and addend in the operand it fills. On an
executable, `-D` writes `PROCESSOR`, an `EQU` for each absolute
symbol, and each output section as `SEG` and `ORG` (`RORG` when it
runs elsewhere), naming each address operand after the symbol there
or the object it's inside (`src+1`). An absolute instruction with an
operand below $100 gets dasm's `.w` extension. The examples check in
this listing of their linked programs (`tests/link_helpers.listing`).
`tests/test_binutils.py` checks with dasm, where it's installed, that
the listing assembles into the executable's bytes.
