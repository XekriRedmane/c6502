#pragma once

#include <cstdint>
#include <map>
#include <optional>
#include <string>
#include <vector>

#include "binutils/elf.h"
#include "sim/cpu.h"

namespace c6502::sim::game {

// The bits of `SwitchAccess::access`. A read-modify-write sets both.
inline constexpr std::uint8_t kReadAccess = 1;
inline constexpr std::uint8_t kWriteAccess = 2;

struct SwitchAccess {
  std::uint16_t address;
  std::uint8_t access;
  // The register a store writes, X for STX, Y for STY, and A for every
  // other instruction.
  std::uint8_t value;
};

// Returns the soft switch that the instruction at PC reads or writes,
// or none. Only the absolute, absolute indexed, and (zero page)
// indirect modes can reach one, and JMP and JSR don't access their
// operand.
std::optional<SwitchAccess> soft_switch_access(const Cpu& cpu);

// The CRC-32 of the bytes from `start` up to `end`, computed again only
// when they differ from the copy kept with the last one.
class PageCrc {
 public:
  PageCrc(std::uint16_t start, std::uint16_t end) : start_(start), end_(end) {}
  std::uint32_t operator()(const std::vector<std::uint8_t>& memory);

 private:
  std::uint16_t start_;
  std::uint16_t end_;
  std::vector<std::uint8_t> copy_;
  std::uint32_t crc_ = 0;
};

// Returns the defined symbols of the linked executable, other than
// those of type STT_FILE, a global one where a name is both.
std::map<std::string, std::int64_t> program_symbols(const binutils::elf::ElfFile& exe);

// Returns the address and name of each function of the executable.
std::map<std::int64_t, std::string> function_names(const binutils::elf::ElfFile& exe);

// Returns the routine of `routines` (address to name) that `pc` is in,
// as "NAME+offset", or "$XXXX" when no routine starts at or below it.
std::string routine_at(const std::map<std::int64_t, std::string>& routines, std::int64_t pc);

// Pushes `back` as a JSR pushes its return address, high byte first, so
// that the subroutine's RTS goes to `back` + 1.
void push_return(Cpu& cpu, std::uint16_t back);

// These tokenize the reverse-engineering projects' main.asm and
// main.sym, which hold no control character other than tab, CR, and LF.
// The whitespace is space, tab, CR, and LF, and `split_lines` splits at
// LF and drops a CR before it.
bool is_space(char c);
std::vector<std::string> split_words(std::string_view text);
std::string strip(std::string_view text);
std::vector<std::string> split_lines(std::string_view text);

}  // namespace c6502::sim::game
