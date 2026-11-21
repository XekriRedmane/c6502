// The codecs of the values of the register allocation (src/tagged_json.h
// describes the codecs), and the runners of its captured calls, which
// src/passes/stage_registry.cpp registers: `compute_liveness`,
// `build_interference`, `coalesce_moves`, `hwreg_eligibility`, and
// `color_graph`.
#pragma once

#include <string>
#include <tuple>
#include <vector>

#include "capture.h"
#include "json.h"
#include "ordered_map.h"
#include "passes/asm_context.h"
#include "passes/optimization/interference.h"
#include "passes/optimization_asm/coalescing.h"
#include "passes/optimization_asm/hwreg_eligibility.h"
#include "passes/optimization_asm/liveness.h"
#include "passes/stage_registry.h"

namespace c6502::passes {

// A `Liveness` as the harness records it (`_liveness_fields` in
// tools/capture_passes.py): each block as its id, its number of
// instructions, and its edges, in the order of the CFG's blocks, the
// order of the real blocks, and the sets of each block by id.
struct LivenessRecord {
  struct Block {
    asdl::Int id = 0;
    asdl::Int instructions = 0;
    std::vector<asdl::Int> predecessors;
    std::vector<asdl::Int> successors;
    bool operator==(const Block&) const = default;
  };
  std::vector<Block> blocks;
  std::vector<asdl::Int> block_order;
  OrderedMap<asdl::Int, NameSet> live_in;
  OrderedMap<asdl::Int, NameSet> live_out;
  bool operator==(const LivenessRecord&) const = default;
};

// Returns the record of `liveness`.
LivenessRecord liveness_record(const Liveness& liveness);

}  // namespace c6502::passes

namespace c6502::tagged {

#define C6502_TAGGED_CODEC(Name, Type)                    \
  struct Name {                                           \
    using type = Type;                                    \
    static type decode(const json::Value& v);             \
    static void encode(std::string& out, const type& x); \
  }

C6502_TAGGED_CODEC(LivenessC, passes::LivenessRecord);
C6502_TAGGED_CODEC(InterferenceNodeC, passes::InterferenceNode);
C6502_TAGGED_CODEC(InterferenceGraphC, passes::InterferenceGraph);
C6502_TAGGED_CODEC(CoalesceResultC, passes::CoalesceResult);
C6502_TAGGED_CODEC(HwRegEligibilityC, passes::HwRegEligibility);

#undef C6502_TAGGED_CODEC

}  // namespace c6502::tagged

namespace c6502::passes::regalloc_stage {

StageOutput run_compute_liveness(const capture::Call& call, const AsmContext& ctx,
                                 const json::Value& args, const json::Value& kwargs);
StageOutput run_build_interference(const capture::Call& call, const AsmContext& ctx,
                                   const json::Value& args, const json::Value& kwargs);
StageOutput run_coalesce_moves(const capture::Call& call, const AsmContext& ctx,
                               const json::Value& args, const json::Value& kwargs);
StageOutput run_hwreg_eligibility(const capture::Call& call, const AsmContext& ctx,
                                  const json::Value& args, const json::Value& kwargs);
StageOutput run_color_graph(const capture::Call& call, const AsmContext& ctx,
                            const json::Value& args, const json::Value& kwargs);

}  // namespace c6502::passes::regalloc_stage
