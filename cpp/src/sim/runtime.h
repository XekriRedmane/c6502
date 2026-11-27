// The simulator's runtime, ported from sim/runtime.py: the boot stub at
// $0600, the runtime helpers with 6502 code assembled at $E000 with
// `icall` after them, and the hooks of the helpers with no 6502 code at
// trap addresses after that, which the harness runs in place of the
// instructions there.
//
//     $0000-$0025  SSP, FP, HARGS, and DPTR in the classic layout
//     $0100-$01FF  the hardware stack
//     $0600        the boot stub
//     $0800-       the program
//     $E000-       the helpers, `icall`, and the trap addresses
//     $FFFC-$FFFD  the reset vector, pointing at the boot stub
//
// SSP starts at $7FFF and grows down.
#pragma once

#include <cstdint>
#include <functional>
#include <map>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

#include "passes/asm_context.h"

namespace c6502::sim {

inline constexpr std::uint16_t kBootAddr = 0x0600;
inline constexpr std::uint16_t kSspInit = 0x7FFF;
inline constexpr std::uint16_t kResetVector = 0xFFFC;
inline constexpr std::uint16_t kHelperOrigin = 0xE000;
inline constexpr std::uint16_t kTrapStride = 0x10;

// The failure of a hook, which ends the run, by the name of the Python
// exception the hook raised and its message.
class HookError : public std::runtime_error {
 public:
  HookError(std::string python_type, const std::string& message)
      : std::runtime_error(message), python_type_(std::move(python_type)) {}
  const std::string& python_type() const noexcept { return python_type_; }

 private:
  std::string python_type_;
};

// A hook reads its operands from the simulator's 64K memory and writes
// its results there.
using Hook = std::function<void(std::vector<std::uint8_t>& memory)>;

struct Runtime {
  // The address of every helper, `icall` included.
  std::map<std::string, std::int64_t> symbols;
  // The hook at each trap address.
  std::map<std::uint16_t, Hook> hooks;
  // The bytes of the helpers and `icall`, laid down at `helper_origin`.
  std::vector<std::uint8_t> helper_image;
  std::uint16_t helper_origin = kHelperOrigin;
  std::uint16_t boot_addr = kBootAddr;
  std::uint16_t ssp_init = kSspInit;
  // The program's SSP and HARGS.
  std::uint16_t ssp = 0x00;
  std::uint16_t hargs = 0x04;
};

// The helpers with no 6502 code, in the order of sim/runtime.py's
// `_HELPERS`, which gives each the next trap address.
const std::vector<std::string>& hooked_helpers();

// Returns the hook of the helper `name` for a program whose HARGS is at
// `hargs`. Throws `std::out_of_range` for a name with no hook.
Hook make_hook(std::string_view name, std::uint16_t hargs);

// Returns the runtime for `layout`: the helpers assembled at $E000 with
// the layout's addresses, `icall` (`JMP (DPTR)`) after them, and a trap
// address for each of `hooked_helpers`, 16 bytes apart from the next
// multiple of 16. The addresses are the same for every layout.
Runtime build_runtime(const passes::RuntimeLayout& layout = {});

// Returns the boot stub, `LDA #$FF; STA SSP; LDA #$7F; STA SSP+1;
// [JSR __zp_init;] JSR main; BRK`. Throws `std::invalid_argument` for an
// address past $FFFF.
std::vector<std::uint8_t> boot_stub(std::int64_t main_addr, std::optional<std::int64_t> init_addr,
                                    std::uint16_t ssp);

// Writes the boot stub, the reset vector, and the helpers into `image`.
void install_runtime(std::vector<std::uint8_t>& image, const Runtime& runtime, std::int64_t main_addr,
                     std::optional<std::int64_t> init_addr = std::nullopt);

}  // namespace c6502::sim
