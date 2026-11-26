// `choose_layout`, which places the runtime's zero-page block. The
// layout itself is `RuntimeLayout` in passes/asm_context.h, and
// `hargs_need` and `check_hargs`, which read the finished asm program,
// are in passes/zp_globals.h.
#pragma once

#include <functional>
#include <set>

#include "asdl/runtime.h"
#include "passes/asm_context.h"

namespace c6502::passes {

// Returns the number of HARGS bytes a program needs. `choose_layout`
// calls it only when the runtime's block has to move.
using HargsNeed = std::function<asdl::Int()>;

// Returns the layout for a program whose fixed zero-page objects occupy
// `reserved`: the classic layout when none of its bytes is reserved,
// and otherwise HARGS, SSP, FP, and DPTR in the lowest free runs below
// $80, in that order, with HARGS sized by `hargs_need`. Throws
// `CompileError("RuntimeLayoutError", ...)` when one of them doesn't
// fit.
RuntimeLayout choose_layout(const std::set<asdl::Int>& reserved, const HargsNeed& hargs_need);

}  // namespace c6502::passes
