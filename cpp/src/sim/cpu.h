// The simulator's 6502, which executes each instruction as py65 1.2.0's
// `MPU` (py65/devices/mpu6502.py) does: the same registers, flags,
// memory written, and cycles, for all 256 opcodes. That includes
// py65's departures from the NMOS 6502. In decimal mode, ADC and SBC
// set Z, N, and V from the binary result before the adjustment, `DEC
// abs` takes 3 cycles, and each of the 105 opcodes py65 doesn't define
// skips one operand byte in 0 cycles and changes nothing else. Nothing
// raises an interrupt. cpp/tests/frozen/cpu holds py65's own results,
// which cpp/tests/test_cpu.cpp checks the CPU against.
#pragma once

#include <bitset>
#include <cstdint>
#include <functional>
#include <stdexcept>
#include <unordered_map>
#include <utility>
#include <vector>

namespace c6502::sim {

// A step that py65 can't take. py65 raises `IndexError` when it reads a
// word at $FFFF, since the word's high byte would be at $10000.
class CpuError : public std::runtime_error {
 public:
  using std::runtime_error::runtime_error;
};

// The base cycles of each opcode, and the flag that adds one cycle for
// a page crossed by an abs,X, abs,Y, or (zp),Y read (py65's
// `cycletime` and `extracycles`). The branches' flag is 2, and a taken
// branch adds one cycle, and one more when it lands on another page
// than the instruction after it, whatever the flag.
extern const std::uint8_t kCycleTime[256];
extern const std::uint8_t kExtraCycles[256];

class Cpu {
 public:
  static constexpr std::uint8_t kCarry = 0x01;
  static constexpr std::uint8_t kZero = 0x02;
  static constexpr std::uint8_t kInterrupt = 0x04;
  static constexpr std::uint8_t kDecimal = 0x08;
  static constexpr std::uint8_t kBreak = 0x10;
  static constexpr std::uint8_t kUnused = 0x20;
  static constexpr std::uint8_t kOverflow = 0x40;
  static constexpr std::uint8_t kNegative = 0x80;

  // What a trap or `before_instruction` gives `run` to do next.
  enum class Action {
    kRts,       // return as RTS would, in 0 cycles (`rts`)
    kExecute,   // execute the instruction at PC
    kContinue,  // start the loop again at PC, which the callback has set
    kStop,      // end the run
  };
  using Callback = std::function<Action(Cpu&)>;

  // Why `run` returned.
  enum class Stop { kTrap, kBrk, kCycles, kInstructions, kCallback };
  struct Limits {
    std::uint64_t max_cycles = UINT64_MAX;
    std::uint64_t max_instructions = UINT64_MAX;
    // Stop before an opcode of $00, as the harness's step loop does.
    bool stop_at_brk = true;
  };
  struct RunResult {
    Stop stop;
    std::uint64_t instructions;
  };

  std::uint16_t pc = 0;
  std::uint8_t a = 0;
  std::uint8_t x = 0;
  std::uint8_t y = 0;
  std::uint8_t sp = 0xFF;
  std::uint8_t p = kBreak | kUnused;
  std::uint64_t cycles = 0;
  std::vector<std::uint8_t> memory = std::vector<std::uint8_t>(0x10000);

  // When set, every write appends its address and value, in the order
  // py65 makes them: a push's byte, each byte of JSR's and BRK's return
  // address high byte first, and a read-modify-write's one store.
  std::vector<std::pair<std::uint16_t, std::uint8_t>>* write_log = nullptr;

  // Called in `run` before each instruction, after the trap table and
  // the stop at BRK. `kRts` counts as `kExecute`.
  Callback before_instruction;
  // Called in `run` after each instruction.
  std::function<void(Cpu&)> after_instruction;

  // Sets the registers as py65's `reset` does with `start_pc` at `start`,
  // leaving memory as it is: SP $FF, A, X, and Y 0, P $30 (B and bit 5),
  // and no cycles.
  void reset(std::uint16_t start = 0);

  // Executes the instruction at PC and adds its cycles. Every byte the
  // instruction reads, the opcode included, goes through the read
  // callback when its address is in the callbacks' range, and every
  // byte it writes goes to the write callback in that range before it
  // is stored. A read-modify-write reads its byte before it writes it.
  // Throws `CpuError` where py65 raises, after the reads and writes
  // that py65 makes before it.
  void step();

  // Calls `read` for each read of an address from `first` to `last` and
  // returns its value, in place of memory's, and calls `write` with each
  // byte written there, which is then stored in memory too. Either may
  // be empty.
  void set_io(std::uint16_t first, std::uint16_t last, std::function<std::uint8_t(std::uint16_t)> read,
              std::function<void(std::uint16_t, std::uint8_t)> write);

  // Makes `run` call `trap` when PC reaches `addr`, before the fetch. A
  // trap must not remove its own entry.
  void set_trap(std::uint16_t addr, Callback trap);
  void clear_trap(std::uint16_t addr);
  bool has_trap(std::uint16_t addr) const { return trap_set_[addr]; }

  // Steps until a limit, a trap or callback that gives `kStop`, or (with
  // `stop_at_brk`) an opcode of $00 at PC. Before each instruction it
  // checks the cycles against `max_cycles` and the instructions this run
  // has executed against `max_instructions`, then calls the trap at PC,
  // then stops at BRK, then calls `before_instruction`, and steps. This
  // is the order of sim/harness.py's `run`.
  RunResult run(const Limits& limits);

  // Returns as RTS would, without the cycles: pops the return address and
  // sets PC to the byte after it, as the harness does after a hook.
  void rts();

 private:
  std::uint8_t read(std::uint16_t addr) {
    if (io_read_ && static_cast<std::uint16_t>(addr - io_first_) <= io_span_) return io_read_(addr);
    return memory[addr];
  }
  void write(std::uint16_t addr, std::uint8_t value) {
    if (write_log != nullptr) write_log->emplace_back(addr, value);
    if (io_write_ && static_cast<std::uint16_t>(addr - io_first_) <= io_span_) io_write_(addr, value);
    memory[addr] = value;
  }
  std::uint16_t word_at(std::uint16_t addr);
  std::uint16_t wrap_at(std::uint16_t addr);
  void push(std::uint8_t value);
  std::uint8_t pop();
  void flags_nz(std::uint8_t value);

  std::uint16_t zero_page_x();
  std::uint16_t zero_page_y();
  std::uint16_t indirect_x();
  std::uint16_t indirect_y();
  std::uint16_t absolute_x();
  std::uint16_t absolute_y();
  void branch(bool taken);

  void adc(std::uint8_t data);
  void sbc(std::uint8_t data);
  void compare(std::uint16_t addr, std::uint8_t reg);
  std::uint8_t asl(std::uint8_t v);
  std::uint8_t lsr(std::uint8_t v);
  std::uint8_t rol(std::uint8_t v);
  std::uint8_t ror(std::uint8_t v);
  std::uint8_t inc(std::uint8_t v);
  std::uint8_t dec(std::uint8_t v);
  void execute(std::uint8_t op);

  // py65's `excycles` and `addcycles`, for the instruction executing.
  int excycles_ = 0;
  bool addcycles_ = false;

  std::uint16_t io_first_ = 0;
  std::uint16_t io_span_ = 0;
  std::function<std::uint8_t(std::uint16_t)> io_read_;
  std::function<void(std::uint16_t, std::uint8_t)> io_write_;

  std::bitset<0x10000> trap_set_;
  std::unordered_map<std::uint16_t, Callback> traps_;
};

}  // namespace c6502::sim
