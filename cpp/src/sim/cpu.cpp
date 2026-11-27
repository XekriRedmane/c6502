// Each method follows the py65 method of the same purpose in
// py65/devices/mpu6502.py, read by read, write by write, since the
// read and write callbacks see them in that order.
#include "sim/cpu.h"

namespace c6502::sim {

// py65's `cycletime` and `extracycles`, which cpp/tests/test_cpu.cpp
// compares with the tables tools/py65_cpu_vectors.py froze.
const std::uint8_t kCycleTime[256] = {
    7, 6, 0, 0, 0, 3, 5, 0, 3, 2, 2, 0, 0, 4, 6, 0,  // $00
    2, 5, 0, 0, 0, 4, 6, 0, 2, 4, 0, 0, 0, 4, 7, 0,  // $10
    6, 6, 0, 0, 3, 3, 5, 0, 4, 2, 2, 0, 4, 4, 6, 0,  // $20
    2, 5, 0, 0, 0, 4, 6, 0, 2, 4, 0, 0, 0, 4, 7, 0,  // $30
    6, 6, 0, 0, 0, 3, 5, 0, 3, 2, 2, 0, 3, 4, 6, 0,  // $40
    2, 5, 0, 0, 0, 4, 6, 0, 2, 4, 0, 0, 0, 4, 7, 0,  // $50
    6, 6, 0, 0, 0, 3, 5, 0, 4, 2, 2, 0, 5, 4, 6, 0,  // $60
    2, 5, 0, 0, 0, 4, 6, 0, 2, 4, 0, 0, 0, 4, 7, 0,  // $70
    0, 6, 0, 0, 3, 3, 3, 0, 2, 0, 2, 0, 4, 4, 4, 0,  // $80
    2, 6, 0, 0, 4, 4, 4, 0, 2, 5, 2, 0, 0, 5, 0, 0,  // $90
    2, 6, 2, 0, 3, 3, 3, 0, 2, 2, 2, 0, 4, 4, 4, 0,  // $A0
    2, 5, 0, 0, 4, 4, 4, 0, 2, 4, 2, 0, 4, 4, 4, 0,  // $B0
    2, 6, 0, 0, 3, 3, 5, 0, 2, 2, 2, 0, 4, 4, 3, 0,  // $C0
    2, 5, 0, 0, 0, 4, 6, 0, 2, 4, 0, 0, 0, 4, 7, 0,  // $D0
    2, 6, 0, 0, 3, 3, 5, 0, 2, 2, 2, 0, 4, 4, 6, 0,  // $E0
    2, 5, 0, 0, 0, 4, 6, 0, 2, 4, 0, 0, 0, 4, 7, 0,  // $F0
};

const std::uint8_t kExtraCycles[256] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // $00
    2, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0,  // $10
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // $20
    2, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0,  // $30
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // $40
    2, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0,  // $50
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // $60
    2, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0,  // $70
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // $80
    2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // $90
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // $A0
    2, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 0,  // $B0
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // $C0
    2, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0,  // $D0
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // $E0
    2, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0,  // $F0
};

void Cpu::reset(std::uint16_t start) {
  pc = start;
  sp = 0xFF;
  a = 0;
  x = 0;
  y = 0;
  p = kBreak | kUnused;
  cycles = 0;
}

void Cpu::set_io(std::uint16_t first, std::uint16_t last, std::function<std::uint8_t(std::uint16_t)> read,
                 std::function<void(std::uint16_t, std::uint8_t)> write) {
  io_first_ = first;
  io_span_ = static_cast<std::uint16_t>(last - first);
  io_read_ = std::move(read);
  io_write_ = std::move(write);
}

void Cpu::set_trap(std::uint16_t addr, Callback trap) {
  traps_[addr] = std::move(trap);
  trap_set_[addr] = true;
}

void Cpu::clear_trap(std::uint16_t addr) {
  traps_.erase(addr);
  trap_set_[addr] = false;
}

void Cpu::rts() {
  const std::uint8_t lo = memory[0x100 + ((sp + 1) & 0xFF)];
  const std::uint8_t hi = memory[0x100 + ((sp + 2) & 0xFF)];
  sp = static_cast<std::uint8_t>(sp + 2);
  pc = static_cast<std::uint16_t>(((hi << 8) | lo) + 1);
}

Cpu::RunResult Cpu::run(const Limits& limits) {
  std::uint64_t n = 0;
  for (;;) {
    if (cycles >= limits.max_cycles) return {Stop::kCycles, n};
    if (n >= limits.max_instructions) return {Stop::kInstructions, n};
    if (trap_set_[pc]) {
      const Action action = traps_.at(pc)(*this);
      if (action == Action::kStop) return {Stop::kTrap, n};
      if (action == Action::kRts) {
        rts();
        continue;
      }
      if (action == Action::kContinue) continue;
    }
    if (limits.stop_at_brk && memory[pc] == 0x00) return {Stop::kBrk, n};
    if (before_instruction) {
      const Action action = before_instruction(*this);
      if (action == Action::kStop) return {Stop::kCallback, n};
      if (action == Action::kContinue) continue;
    }
    step();
    ++n;
    if (after_instruction) after_instruction(*this);
  }
}

// `WordAt`, which py65 reads past the end of its memory at $FFFF.
std::uint16_t Cpu::word_at(std::uint16_t addr) {
  const std::uint8_t lo = read(addr);
  if (addr == 0xFFFF) throw CpuError("py65 reads a word at $FFFF, whose high byte is past the end of memory");
  return static_cast<std::uint16_t>(lo | (read(static_cast<std::uint16_t>(addr + 1)) << 8));
}

// `WrapAt`: the high byte is in the same page.
std::uint16_t Cpu::wrap_at(std::uint16_t addr) {
  const std::uint8_t lo = read(addr);
  const std::uint8_t hi = read(static_cast<std::uint16_t>((addr & 0xFF00) | ((addr + 1) & 0xFF)));
  return static_cast<std::uint16_t>(lo | (hi << 8));
}

void Cpu::push(std::uint8_t value) {
  write(static_cast<std::uint16_t>(0x100 + sp), value);
  sp = static_cast<std::uint8_t>(sp - 1);
}

std::uint8_t Cpu::pop() {
  sp = static_cast<std::uint8_t>(sp + 1);
  return read(static_cast<std::uint16_t>(0x100 + sp));
}

void Cpu::flags_nz(std::uint8_t value) {
  p &= static_cast<std::uint8_t>(~(kZero | kNegative));
  if (value == 0) {
    p |= kZero;
  } else {
    p |= value & kNegative;
  }
}

std::uint16_t Cpu::zero_page_x() { return (x + read(pc)) & 0xFF; }

std::uint16_t Cpu::zero_page_y() { return (y + read(pc)) & 0xFF; }

std::uint16_t Cpu::indirect_x() { return wrap_at((read(pc) + x) & 0xFF); }

std::uint16_t Cpu::indirect_y() {
  const std::uint16_t base = wrap_at(read(pc));
  const auto addr = static_cast<std::uint16_t>(base + y);
  if (addcycles_ && (base & 0xFF00) != (addr & 0xFF00)) ++excycles_;
  return addr;
}

std::uint16_t Cpu::absolute_x() {
  const std::uint16_t base = word_at(pc);
  const auto addr = static_cast<std::uint16_t>(base + x);
  if (addcycles_ && (base & 0xFF00) != (addr & 0xFF00)) ++excycles_;
  return addr;
}

std::uint16_t Cpu::absolute_y() {
  const std::uint16_t base = word_at(pc);
  const auto addr = static_cast<std::uint16_t>(base + y);
  if (addcycles_ && (base & 0xFF00) != (addr & 0xFF00)) ++excycles_;
  return addr;
}

// `opBCL` and `opBST` with `BranchRelAddr`. py65 doesn't mask PC until
// the end of the step, and a page compared after masking is the same.
void Cpu::branch(bool taken) {
  if (!taken) {
    ++pc;
    return;
  }
  ++excycles_;
  const std::uint8_t offset = read(pc);
  ++pc;
  const auto target =
      static_cast<std::uint16_t>((offset & kNegative) != 0 ? pc - (offset ^ 0xFF) - 1 : pc + offset);
  if ((pc & 0xFF00) != (target & 0xFF00)) ++excycles_;
  pc = target;
}

void Cpu::adc(std::uint8_t data) {
  const int carry = p & kCarry;
  if ((p & kDecimal) != 0) {
    int halfcarry = 0;
    int decimalcarry = 0;
    int adjust0 = 0;
    int adjust1 = 0;
    int nibble0 = (data & 0xF) + (a & 0xF) + carry;
    if (nibble0 > 9) {
      adjust0 = 6;
      halfcarry = 1;
    }
    int nibble1 = ((data >> 4) & 0xF) + ((a >> 4) & 0xF) + halfcarry;
    if (nibble1 > 9) {
      adjust1 = 6;
      decimalcarry = 1;
    }
    // The flags come from the ALU's output before the adjustment.
    nibble0 &= 0xF;
    nibble1 &= 0xF;
    const int aluresult = (nibble1 << 4) + nibble0;
    nibble0 = (nibble0 + adjust0) & 0xF;
    nibble1 = (nibble1 + adjust1) & 0xF;
    p &= static_cast<std::uint8_t>(~(kCarry | kOverflow | kNegative | kZero));
    if (aluresult == 0) {
      p |= kZero;
    } else {
      p |= aluresult & kNegative;
    }
    if (decimalcarry == 1) p |= kCarry;
    if ((~(a ^ data) & (a ^ aluresult) & kNegative) != 0) p |= kOverflow;
    a = static_cast<std::uint8_t>((nibble1 << 4) + nibble0);
    return;
  }
  const int result = data + a + carry;
  p &= static_cast<std::uint8_t>(~(kCarry | kOverflow | kNegative | kZero));
  if ((~(a ^ data) & (a ^ result) & kNegative) != 0) p |= kOverflow;
  if (result > 0xFF) p |= kCarry;
  const auto value = static_cast<std::uint8_t>(result);
  if (value == 0) {
    p |= kZero;
  } else {
    p |= value & kNegative;
  }
  a = value;
}

void Cpu::sbc(std::uint8_t data) {
  const int carry = p & kCarry;
  const int inverted = ~data & 0xFF;
  if ((p & kDecimal) != 0) {
    int halfcarry = 1;
    int decimalcarry = 0;
    int adjust0 = 0;
    int adjust1 = 0;
    const int nibble0 = (a & 0xF) + (inverted & 0xF) + carry;
    if (nibble0 <= 0xF) {
      halfcarry = 0;
      adjust0 = 10;
    }
    const int nibble1 = ((a >> 4) & 0xF) + ((inverted >> 4) & 0xF) + halfcarry;
    if (nibble1 <= 0xF) adjust1 = 10 << 4;
    // The flags come from the ALU's output before the adjustment.
    int aluresult = a + inverted + carry;
    if (aluresult > 0xFF) decimalcarry = 1;
    aluresult &= 0xFF;
    const int low = (aluresult + adjust0) & 0xF;
    const int high = ((aluresult + adjust1) >> 4) & 0xF;
    p &= static_cast<std::uint8_t>(~(kCarry | kZero | kNegative | kOverflow));
    if (aluresult == 0) {
      p |= kZero;
    } else {
      p |= aluresult & kNegative;
    }
    if (decimalcarry == 1) p |= kCarry;
    if (((a ^ data) & (a ^ aluresult) & kNegative) != 0) p |= kOverflow;
    a = static_cast<std::uint8_t>((high << 4) + low);
    return;
  }
  const int result = a + inverted + carry;
  p &= static_cast<std::uint8_t>(~(kCarry | kZero | kOverflow | kNegative));
  if (((a ^ data) & (a ^ result) & kNegative) != 0) p |= kOverflow;
  const auto value = static_cast<std::uint8_t>(result);
  if (value == 0) p |= kZero;
  if (result > 0xFF) p |= kCarry;
  p |= value & kNegative;
  a = value;
}

void Cpu::compare(std::uint16_t addr, std::uint8_t reg) {
  const std::uint8_t value = read(addr);
  p &= static_cast<std::uint8_t>(~(kCarry | kZero | kNegative));
  if (reg == value) {
    p |= kCarry | kZero;
  } else if (reg > value) {
    p |= kCarry;
  }
  p |= (reg - value) & kNegative;
}

std::uint8_t Cpu::asl(std::uint8_t v) {
  p &= static_cast<std::uint8_t>(~(kCarry | kNegative | kZero));
  if ((v & kNegative) != 0) p |= kCarry;
  v = static_cast<std::uint8_t>(v << 1);
  if (v != 0) {
    p |= v & kNegative;
  } else {
    p |= kZero;
  }
  return v;
}

std::uint8_t Cpu::lsr(std::uint8_t v) {
  p &= static_cast<std::uint8_t>(~(kCarry | kNegative | kZero));
  p |= v & 1;
  v = static_cast<std::uint8_t>(v >> 1);
  if (v == 0) p |= kZero;
  return v;
}

std::uint8_t Cpu::rol(std::uint8_t v) {
  int t = v;
  if ((p & kCarry) != 0) {
    if ((t & kNegative) == 0) p &= static_cast<std::uint8_t>(~kCarry);
    t = (t << 1) | 1;
  } else {
    if ((t & kNegative) != 0) p |= kCarry;
    t <<= 1;
  }
  const auto out = static_cast<std::uint8_t>(t);
  flags_nz(out);
  return out;
}

std::uint8_t Cpu::ror(std::uint8_t v) {
  int t = v;
  if ((p & kCarry) != 0) {
    if ((t & 1) == 0) p &= static_cast<std::uint8_t>(~kCarry);
    t = (t >> 1) | kNegative;
  } else {
    if ((t & 1) != 0) p |= kCarry;
    t >>= 1;
  }
  const auto out = static_cast<std::uint8_t>(t);
  flags_nz(out);
  return out;
}

std::uint8_t Cpu::inc(std::uint8_t v) {
  v = static_cast<std::uint8_t>(v + 1);
  flags_nz(v);
  return v;
}

std::uint8_t Cpu::dec(std::uint8_t v) {
  v = static_cast<std::uint8_t>(v - 1);
  flags_nz(v);
  return v;
}

void Cpu::step() {
  const std::uint8_t op = read(pc);
  ++pc;
  excycles_ = 0;
  addcycles_ = kExtraCycles[op] != 0;
  execute(op);
  cycles += kCycleTime[op] + static_cast<std::uint64_t>(excycles_);
}

void Cpu::execute(std::uint8_t op) {
  // The address of the operand of each mode, which leaves PC at the
  // operand. The instruction then moves PC past it.
  const auto zp = [this] { return static_cast<std::uint16_t>(read(pc)); };
  const auto absolute = [this] { return word_at(pc); };
  // A read-modify-write of memory.
  const auto rmw = [this](std::uint16_t addr, std::uint8_t (Cpu::*f)(std::uint8_t)) {
    write(addr, (this->*f)(read(addr)));
  };
  const auto load = [this](std::uint8_t& reg, std::uint16_t addr) {
    reg = read(addr);
    flags_nz(reg);
  };
  const auto ora = [this](std::uint16_t addr) {
    a |= read(addr);
    flags_nz(a);
  };
  const auto and_ = [this](std::uint16_t addr) {
    a &= read(addr);
    flags_nz(a);
  };
  const auto eor = [this](std::uint16_t addr) {
    a ^= read(addr);
    flags_nz(a);
  };
  const auto bit = [this](std::uint16_t addr) {
    const std::uint8_t v = read(addr);
    p &= static_cast<std::uint8_t>(~(kZero | kNegative | kOverflow));
    if ((a & v) == 0) p |= kZero;
    p |= v & (kNegative | kOverflow);
  };
  switch (op) {
    case 0x00: {  // BRK
      const auto ret = static_cast<std::uint16_t>(pc + 1);
      push(static_cast<std::uint8_t>(ret >> 8));
      push(static_cast<std::uint8_t>(ret));
      p |= kBreak;
      push(static_cast<std::uint8_t>(p | kBreak | kUnused));
      p |= kInterrupt;
      pc = word_at(0xFFFE);
      break;
    }
    case 0x01: ora(indirect_x()); ++pc; break;
    case 0x05: ora(zp()); ++pc; break;
    case 0x06: rmw(zp(), &Cpu::asl); ++pc; break;
    case 0x08: push(static_cast<std::uint8_t>(p | kBreak | kUnused)); break;  // PHP
    case 0x09: ora(pc); ++pc; break;
    case 0x0A: a = asl(a); break;
    case 0x0D: ora(absolute()); pc += 2; break;
    case 0x0E: rmw(absolute(), &Cpu::asl); pc += 2; break;
    case 0x10: branch((p & kNegative) == 0); break;  // BPL
    case 0x11: ora(indirect_y()); ++pc; break;
    case 0x15: ora(zero_page_x()); ++pc; break;
    case 0x16: rmw(zero_page_x(), &Cpu::asl); ++pc; break;
    case 0x18: p &= static_cast<std::uint8_t>(~kCarry); break;  // CLC
    case 0x19: ora(absolute_y()); pc += 2; break;
    case 0x1D: ora(absolute_x()); pc += 2; break;
    case 0x1E: rmw(absolute_x(), &Cpu::asl); pc += 2; break;
    case 0x20: {  // JSR, which pushes before it reads its target
      const auto ret = static_cast<std::uint16_t>(pc + 1);
      push(static_cast<std::uint8_t>(ret >> 8));
      push(static_cast<std::uint8_t>(ret));
      pc = word_at(pc);
      break;
    }
    case 0x21: and_(indirect_x()); ++pc; break;
    case 0x24: bit(zp()); ++pc; break;
    case 0x25: and_(zp()); ++pc; break;
    case 0x26: rmw(zp(), &Cpu::rol); ++pc; break;
    case 0x28: p = static_cast<std::uint8_t>(pop() | kBreak | kUnused); break;  // PLP
    case 0x29: and_(pc); ++pc; break;
    case 0x2A: a = rol(a); break;
    case 0x2C: bit(absolute()); pc += 2; break;
    case 0x2D: and_(absolute()); pc += 2; break;
    case 0x2E: rmw(absolute(), &Cpu::rol); pc += 2; break;
    case 0x30: branch((p & kNegative) != 0); break;  // BMI
    case 0x31: and_(indirect_y()); ++pc; break;
    case 0x35: and_(zero_page_x()); ++pc; break;
    case 0x36: rmw(zero_page_x(), &Cpu::rol); ++pc; break;
    case 0x38: p |= kCarry; break;  // SEC
    case 0x39: and_(absolute_y()); pc += 2; break;
    case 0x3D: and_(absolute_x()); pc += 2; break;
    case 0x3E: rmw(absolute_x(), &Cpu::rol); pc += 2; break;
    case 0x40: {  // RTI
      p = static_cast<std::uint8_t>(pop() | kBreak | kUnused);
      const std::uint8_t lo = pop();
      pc = static_cast<std::uint16_t>(lo | (pop() << 8));
      break;
    }
    case 0x41: eor(indirect_x()); ++pc; break;
    case 0x45: eor(zp()); ++pc; break;
    case 0x46: rmw(zp(), &Cpu::lsr); ++pc; break;
    case 0x48: push(a); break;  // PHA
    case 0x49: eor(pc); ++pc; break;
    case 0x4A: a = lsr(a); break;
    case 0x4C: pc = absolute(); break;  // JMP abs
    case 0x4D: eor(absolute()); pc += 2; break;
    case 0x4E: rmw(absolute(), &Cpu::lsr); pc += 2; break;
    case 0x50: branch((p & kOverflow) == 0); break;  // BVC
    case 0x51: eor(indirect_y()); ++pc; break;
    case 0x55: eor(zero_page_x()); ++pc; break;
    case 0x56: rmw(zero_page_x(), &Cpu::lsr); ++pc; break;
    case 0x58: p &= static_cast<std::uint8_t>(~kInterrupt); break;  // CLI
    case 0x59: eor(absolute_y()); pc += 2; break;
    case 0x5D: eor(absolute_x()); pc += 2; break;
    case 0x5E: rmw(absolute_x(), &Cpu::lsr); pc += 2; break;
    case 0x60: {  // RTS
      const std::uint8_t lo = pop();
      pc = static_cast<std::uint16_t>((lo | (pop() << 8)) + 1);
      break;
    }
    case 0x61: adc(read(indirect_x())); ++pc; break;
    case 0x65: adc(read(zp())); ++pc; break;
    case 0x66: rmw(zp(), &Cpu::ror); ++pc; break;
    case 0x68: a = pop(); flags_nz(a); break;  // PLA
    case 0x69: adc(read(pc)); ++pc; break;
    case 0x6A: a = ror(a); break;
    case 0x6C: pc = wrap_at(absolute()); break;  // JMP ind
    case 0x6D: adc(read(absolute())); pc += 2; break;
    case 0x6E: rmw(absolute(), &Cpu::ror); pc += 2; break;
    case 0x70: branch((p & kOverflow) != 0); break;  // BVS
    case 0x71: adc(read(indirect_y())); ++pc; break;
    case 0x75: adc(read(zero_page_x())); ++pc; break;
    case 0x76: rmw(zero_page_x(), &Cpu::ror); ++pc; break;
    case 0x78: p |= kInterrupt; break;  // SEI
    case 0x79: adc(read(absolute_y())); pc += 2; break;
    case 0x7D: adc(read(absolute_x())); pc += 2; break;
    case 0x7E: rmw(absolute_x(), &Cpu::ror); pc += 2; break;
    case 0x81: write(indirect_x(), a); ++pc; break;
    case 0x84: write(zp(), y); ++pc; break;
    case 0x85: write(zp(), a); ++pc; break;
    case 0x86: write(zp(), x); ++pc; break;
    case 0x88: --y; flags_nz(y); break;  // DEY
    case 0x8A: a = x; flags_nz(a); break;  // TXA
    case 0x8C: write(absolute(), y); pc += 2; break;
    case 0x8D: write(absolute(), a); pc += 2; break;
    case 0x8E: write(absolute(), x); pc += 2; break;
    case 0x90: branch((p & kCarry) == 0); break;  // BCC
    case 0x91: write(indirect_y(), a); ++pc; break;
    case 0x94: write(zero_page_x(), y); ++pc; break;
    case 0x95: write(zero_page_x(), a); ++pc; break;
    case 0x96: write(zero_page_y(), x); ++pc; break;
    case 0x98: a = y; flags_nz(a); break;  // TYA
    case 0x99: write(absolute_y(), a); pc += 2; break;
    case 0x9A: sp = x; break;  // TXS
    case 0x9D: write(absolute_x(), a); pc += 2; break;
    case 0xA0: load(y, pc); ++pc; break;
    case 0xA1: load(a, indirect_x()); ++pc; break;
    case 0xA2: load(x, pc); ++pc; break;
    case 0xA4: load(y, zp()); ++pc; break;
    case 0xA5: load(a, zp()); ++pc; break;
    case 0xA6: load(x, zp()); ++pc; break;
    case 0xA8: y = a; flags_nz(y); break;  // TAY
    case 0xA9: load(a, pc); ++pc; break;
    case 0xAA: x = a; flags_nz(x); break;  // TAX
    case 0xAC: load(y, absolute()); pc += 2; break;
    case 0xAD: load(a, absolute()); pc += 2; break;
    case 0xAE: load(x, absolute()); pc += 2; break;
    case 0xB0: branch((p & kCarry) != 0); break;  // BCS
    case 0xB1: load(a, indirect_y()); ++pc; break;
    case 0xB4: load(y, zero_page_x()); ++pc; break;
    case 0xB5: load(a, zero_page_x()); ++pc; break;
    case 0xB6: load(x, zero_page_y()); ++pc; break;
    case 0xB8: p &= static_cast<std::uint8_t>(~kOverflow); break;  // CLV
    case 0xB9: load(a, absolute_y()); pc += 2; break;
    case 0xBA: x = sp; flags_nz(x); break;  // TSX
    case 0xBC: load(y, absolute_x()); pc += 2; break;
    case 0xBD: load(a, absolute_x()); pc += 2; break;
    case 0xBE: load(x, absolute_y()); pc += 2; break;
    case 0xC0: compare(pc, y); ++pc; break;
    case 0xC1: compare(indirect_x(), a); ++pc; break;
    case 0xC4: compare(zp(), y); ++pc; break;
    case 0xC5: compare(zp(), a); ++pc; break;
    case 0xC6: rmw(zp(), &Cpu::dec); ++pc; break;
    case 0xC8: ++y; flags_nz(y); break;  // INY
    case 0xC9: compare(pc, a); ++pc; break;
    case 0xCA: --x; flags_nz(x); break;  // DEX
    case 0xCC: compare(absolute(), y); pc += 2; break;
    case 0xCD: compare(absolute(), a); pc += 2; break;
    case 0xCE: rmw(absolute(), &Cpu::dec); pc += 2; break;
    case 0xD0: branch((p & kZero) == 0); break;  // BNE
    case 0xD1: compare(indirect_y(), a); ++pc; break;
    case 0xD5: compare(zero_page_x(), a); ++pc; break;
    case 0xD6: rmw(zero_page_x(), &Cpu::dec); ++pc; break;
    case 0xD8: p &= static_cast<std::uint8_t>(~kDecimal); break;  // CLD
    case 0xD9: compare(absolute_y(), a); pc += 2; break;
    case 0xDD: compare(absolute_x(), a); pc += 2; break;
    case 0xDE: rmw(absolute_x(), &Cpu::dec); pc += 2; break;
    case 0xE0: compare(pc, x); ++pc; break;
    case 0xE1: sbc(read(indirect_x())); ++pc; break;
    case 0xE4: compare(zp(), x); ++pc; break;
    case 0xE5: sbc(read(zp())); ++pc; break;
    case 0xE6: rmw(zp(), &Cpu::inc); ++pc; break;
    case 0xE8: ++x; flags_nz(x); break;  // INX
    case 0xE9: sbc(read(pc)); ++pc; break;
    case 0xEA: break;  // NOP
    case 0xEC: compare(absolute(), x); pc += 2; break;
    case 0xED: sbc(read(absolute())); pc += 2; break;
    case 0xEE: rmw(absolute(), &Cpu::inc); pc += 2; break;
    case 0xF0: branch((p & kZero) != 0); break;  // BEQ
    case 0xF1: sbc(read(indirect_y())); ++pc; break;
    case 0xF5: sbc(read(zero_page_x())); ++pc; break;
    case 0xF6: rmw(zero_page_x(), &Cpu::inc); ++pc; break;
    case 0xF8: p |= kDecimal; break;  // SED
    case 0xF9: sbc(read(absolute_y())); pc += 2; break;
    case 0xFD: sbc(read(absolute_x())); pc += 2; break;
    case 0xFE: rmw(absolute_x(), &Cpu::inc); pc += 2; break;
    default:
      // `inst_not_implemented`.
      ++pc;
      break;
  }
}

}  // namespace c6502::sim
