// The runners of the asm optimizer's stage calls. Each decodes the
// recorded arguments with the codecs of src/tagged_json.h, where an
// argument Python gives as None, or leaves out, is null or absent, and
// runs the C++ function.
#include "passes/optimization_asm/optimizer_stage.h"

#include <optional>
#include <set>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "asm_ast.h"
#include "passes/function_local_sizing.h"
#include "passes/optimization_asm/apply_coloring.h"
#include "passes/optimization_asm/const_static_fold.h"
#include "passes/optimization_asm/dead_static.h"
#include "tagged_json.h"

namespace c6502::passes::optimizer_stage {

namespace {

namespace ir = ::c6502::asm_ast;
using namespace ::c6502::tagged;
using json::Value;

using FunctionC = Asdl<ir::Type_top_level, kAsmAst>;

ir::Function load_function(const capture::Call& call) {
  auto tl = asdl::load<ir::Type_top_level>(call.input);
  auto* fn = std::get_if<ir::Function>(&tl);
  if (fn == nullptr) {
    throw std::runtime_error("the call's input isn't a Function");
  }
  return std::move(*fn);
}

std::string dump_function(ir::Function fn) {
  return asdl::dump(ir::Type_top_level{std::move(fn)});
}

const Value& positional(const Value& args, std::size_t i) {
  if (i >= args.items.size()) {
    throw std::runtime_error("the call has too few positional arguments");
  }
  return args.items[i];
}

bool is_null(const Value* v) { return v == nullptr || v->kind == Value::Kind::kNull; }

// Returns the codec's value of `v`, or nothing when `v` is absent or
// None.
template <class C>
std::optional<typename C::type> maybe(const Value* v) {
  if (is_null(v)) {
    return std::nullopt;
  }
  return C::decode(*v);
}

template <class T>
const T* ptr(const std::optional<T>& v) {
  return v ? &*v : nullptr;
}

}  // namespace

StageOutput run_compute_local_bytes(const capture::Call& call, const AsmContext&, const Value&,
                                    const Value&) {
  const auto prog = asdl::load<ir::Program>(call.input);
  return {encode<DictOf<Str, IntC>>(compute_local_bytes(prog)), ""};
}

StageOutput run_compute_address_taken_bytes(const capture::Call& call, const AsmContext&,
                                            const Value& args, const Value& kwargs) {
  const auto prog = asdl::load<ir::Program>(call.input);
  const Value* sym_arg = args.items.size() > 0 ? &args.items[0] : kwargs.get("symbols");
  const Value* types_arg = args.items.size() > 1 ? &args.items[1] : kwargs.get("types");
  const auto symbols = maybe<SymbolTableC>(sym_arg);
  const auto types = maybe<TypeTableC>(types_arg);
  return {encode<DictOf<Str, IntC>>(compute_address_taken_bytes(prog, ptr(symbols), ptr(types))),
          ""};
}

StageOutput run_apply_coloring(const capture::Call& call, const AsmContext&, const Value& args,
                               const Value& kwargs) {
  auto fn = load_function(call);
  const Coloring coloring = ColoringC::decode(positional(args, 0));
  const Value* pool_arg = args.items.size() > 1 ? &args.items[1] : kwargs.get("local_pool");
  const auto pool = maybe<ListOf<IntC>>(pool_arg);
  return {dump_function(apply_coloring(std::move(fn), coloring, ptr(pool))), ""};
}

StageOutput run_fold_const_statics(const capture::Call& call, const AsmContext&, const Value&,
                                   const Value& kwargs) {
  auto prog = asdl::load<ir::Program>(call.input);
  const auto symbols = maybe<SymbolTableC>(kwargs.get("symbols"));
  return {asdl::dump(fold_const_statics(std::move(prog), ptr(symbols))), ""};
}

StageOutput run_dead_static_elimination(const capture::Call& call, const AsmContext&, const Value&,
                                        const Value&) {
  auto prog = asdl::load<ir::Program>(call.input);
  return {asdl::dump(apply_dead_static_elimination(std::move(prog))), ""};
}

}  // namespace c6502::passes::optimizer_stage
