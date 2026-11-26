// Splits each `LoadAddress` of a `Data` source into two moves of
// immediates.
#pragma once

#include <string>

#include "asdl/runtime.h"
#include "asm_ast.h"
#include "ordered_map.h"

namespace c6502::passes {

// Each function's pool bytes, by function, with the slot symbol the
// reads of each byte use.
using ByteNamesByFunction = OrderedMap<std::string, OrderedMap<asdl::Int, std::string>>;

// Returns `prog` with each `LoadAddress(src=Data(label, off), dst)` whose
// `dst` is memory made `Mov(#<(label+off), dst); Mov(#>(label+off),
// dst_hi)`. `slot_symbols` maps each zero-page slot symbol to its
// address, and `byte_names[fn]` maps each byte of `fn`'s private pool to
// the slot symbol its reads use. The high byte goes to the byte's own
// symbol when the byte after `dst` starts one (`__local_f__1` after
// `__local_f__0`), and to `dst` plus one otherwise. Either map may be
// null.
asm_ast::Program lower_data_load_address(asm_ast::Program prog,
                                         const OrderedMap<std::string, asdl::Int>* slot_symbols,
                                         const ByteNamesByFunction* byte_names);

}  // namespace c6502::passes
