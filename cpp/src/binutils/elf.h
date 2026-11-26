// ELF32 object and executable files for the 6502, as c6502 writes them.
// `compile.py -c` writes a relocatable object (`ET_REL`), and linking
// writes an executable (`ET_EXEC`). `read` and `write` convert between the bytes and an `ElfFile`, which nm, objdump,
// the linker, and the simulator all work on.
//
// The machine number and the relocation numbers are those llvm-mos
// gives the 6502 (`EM_MOS` and `R_MOS_*` in its `ELF.h` and
// `ELFRelocs/MOS.def`), and `e_flags` is `EF_MOS_ARCH_6502`. The objects
// carry c6502's link metadata, though, which no other linker reads, so
// they aren't meant to be linked by llvm-mos's `ld.lld`.
//
// Relocations are `SHT_RELA`, each with its addend in the entry. The
// value a relocation computes is S + A, where S is the symbol's address
// and A the addend. `R_MOS_PCREL_8` computes S + A - P, where P is the
// address of the byte it writes, and the assembler sets A to -1, so the
// byte is the branch displacement from the next instruction.
#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace c6502::binutils::elf {

constexpr std::uint8_t ELFCLASS32 = 1;
constexpr std::uint8_t ELFDATA2LSB = 1;
constexpr std::uint32_t EV_CURRENT = 1;
constexpr std::uint16_t ET_REL = 1;
constexpr std::uint16_t ET_EXEC = 2;
constexpr std::uint16_t EM_MOS = 6502;
constexpr std::uint32_t EF_MOS_ARCH_6502 = 0x00000001;

constexpr std::uint32_t SHT_NULL = 0;
constexpr std::uint32_t SHT_PROGBITS = 1;
constexpr std::uint32_t SHT_SYMTAB = 2;
constexpr std::uint32_t SHT_STRTAB = 3;
constexpr std::uint32_t SHT_RELA = 4;
constexpr std::uint32_t SHT_NOBITS = 8;

constexpr std::uint32_t SHF_WRITE = 0x1;
constexpr std::uint32_t SHF_ALLOC = 0x2;
constexpr std::uint32_t SHF_EXECINSTR = 0x4;
constexpr std::uint32_t SHF_INFO_LINK = 0x40;

constexpr std::uint16_t SHN_UNDEF = 0;
constexpr std::uint16_t SHN_ABS = 0xFFF1;

constexpr std::uint8_t STB_LOCAL = 0;
constexpr std::uint8_t STB_GLOBAL = 1;
constexpr std::uint8_t STT_NOTYPE = 0;
constexpr std::uint8_t STT_OBJECT = 1;
constexpr std::uint8_t STT_FUNC = 2;
constexpr std::uint8_t STT_SECTION = 3;
constexpr std::uint8_t STT_FILE = 4;

// The `st_other` bit llvm-mos sets on an external symbol in zero page.
// c6502 sets it on every undefined symbol the unit reaches with
// zero-page addressing, so a reader can see which ones the linker has
// to put below $100.
constexpr std::uint8_t STO_MOS_ZEROPAGE = 0x20;

constexpr std::uint32_t PT_LOAD = 1;
constexpr std::uint32_t PF_X = 0x1;
constexpr std::uint32_t PF_W = 0x2;
constexpr std::uint32_t PF_R = 0x4;

constexpr std::uint8_t R_MOS_NONE = 0;
constexpr std::uint8_t R_MOS_IMM8 = 1;
constexpr std::uint8_t R_MOS_ADDR8 = 2;
constexpr std::uint8_t R_MOS_ADDR16 = 3;
constexpr std::uint8_t R_MOS_ADDR16_LO = 4;
constexpr std::uint8_t R_MOS_ADDR16_HI = 5;
constexpr std::uint8_t R_MOS_PCREL_8 = 6;

// Returns the name of relocation type `type` (`RELOC_NAMES`), or null
// for a type that has none.
const char* reloc_name(std::uint8_t type);

// Returns the number of bytes relocation type `type` writes
// (`RELOC_SIZES`), or none for `R_MOS_NONE` and unknown types.
std::optional<int> reloc_size(std::uint8_t type);

// The section that holds the link metadata.
inline constexpr std::string_view LINK_SECTION = ".c6502.link";
// The section that holds the assembly text an object was assembled
// from, which the linker assembles again when the program's addresses
// contradict the unit's.
inline constexpr std::string_view SOURCE_SECTION = ".c6502.asm";

struct Symbol;

// A relocation: the `type`'s bytes at `offset` in its section get the
// value computed from `symbol` and `addend`.
struct Reloc {
  std::int64_t offset = 0;
  std::uint8_t type = R_MOS_NONE;
  std::shared_ptr<Symbol> symbol;
  std::int64_t addend = 0;
};

// A section with contents: code or data (`SHT_PROGBITS`), space with
// none (`SHT_NOBITS`, where `nobits_size` gives its length), or the link
// metadata. `addr` is its run address in an executable, and 0 in an
// object. Symbol and relocation tables aren't `Section`s; they are built
// from `ElfFile::symbols` and each section's `relocs`.
struct Section {
  std::string name;
  std::uint32_t type = SHT_PROGBITS;
  std::uint32_t flags = SHF_ALLOC;
  std::string data;
  std::int64_t nobits_size = 0;
  std::int64_t addr = 0;
  std::int64_t align = 1;
  std::vector<Reloc> relocs;

  std::int64_t size() const {
    return type == SHT_NOBITS ? nobits_size : static_cast<std::int64_t>(data.size());
  }
};

using SectionPtr = std::shared_ptr<Section>;

// A symbol. `section` is the `Section` it's defined in, or null with
// `shndx` `SHN_ABS` for an absolute value or `SHN_UNDEF` for one another
// file defines. `value` is its offset in the section in an object, and
// its address in an executable.
struct Symbol {
  std::string name;
  std::int64_t value = 0;
  std::int64_t size = 0;
  std::uint8_t bind = STB_LOCAL;
  std::uint8_t type = STT_NOTYPE;
  SectionPtr section;
  std::uint16_t shndx = SHN_UNDEF;
  std::uint8_t other = 0;

  bool defined() const { return section != nullptr || shndx != SHN_UNDEF; }
  bool is_global() const { return bind != STB_LOCAL; }
  bool is_absolute() const { return section == nullptr && shndx == SHN_ABS; }
};

using SymbolPtr = std::shared_ptr<Symbol>;

// A loadable segment of an executable: `memsz` bytes run at `vaddr`,
// loaded at `paddr` (different from `vaddr` for a section the startup
// code copies), with the first `data.size()` from `data`.
struct Segment {
  std::int64_t vaddr = 0;
  std::int64_t paddr = 0;
  std::string data;
  std::int64_t memsz = 0;
  std::uint32_t flags = PF_R;
};

// An object (`ET_REL`) or executable (`ET_EXEC`). `symbols` excludes
// the null symbol at index 0.
struct ElfFile {
  std::uint16_t type = ET_REL;
  std::vector<SectionPtr> sections;
  std::vector<SymbolPtr> symbols;
  std::vector<Segment> segments;
  std::int64_t entry = 0;
  std::uint32_t flags = EF_MOS_ARCH_6502;

  // Returns the first section named `name`, or null.
  SectionPtr section(std::string_view name) const;
  // Returns the link metadata's text, or none when there's none.
  std::optional<std::string> link_text() const;
  // Returns the assembly text the object was assembled from, or none.
  std::optional<std::string> source_text() const;
  // Returns the global symbol `name`, else the first local one, or null.
  SymbolPtr symbol(std::string_view name) const;
};

// Returns the bytes of `elf`. Local symbols are written first, as ELF
// requires, and otherwise in the order of `elf.symbols`. Throws
// `CompileError("ElfError", ...)` for a relocation that names a symbol
// the symbol table doesn't hold.
std::string write(const ElfFile& elf);

// Returns the `ElfFile` in `data`. `name` is for error messages. Throws
// `CompileError("ElfError", ...)` when `data` isn't a little-endian
// ELF32 file for the 6502, or when it's truncated or damaged.
ElfFile read(std::string_view data, std::string_view name = "file");

}  // namespace c6502::binutils::elf
