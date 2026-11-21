#include "tac_to_asm_stage.h"

#include <optional>
#include <stdexcept>
#include <string>

#include "tac_to_asm.h"
#include "tagged_json.h"

namespace c6502::tac_to_asm {

namespace {

using namespace ::c6502::tagged;

const json::Value* kwarg(const json::Value& kwargs, std::string_view name) {
  const json::Value* v = kwargs.get(std::string(name));
  return v != nullptr && v->kind != json::Value::Kind::kNull ? v : nullptr;
}

const json::Value* positional(const json::Value& args, std::size_t i) {
  if (i >= args.items.size() || args.items[i].kind == json::Value::Kind::kNull) {
    return nullptr;
  }
  return &args.items[i];
}

// Decodes the keyword argument `name` with codec `C`, or gives none when
// the call passed None or left it out.
template <class C>
std::optional<typename C::type> decode_kwarg(const json::Value& kwargs, std::string_view name) {
  if (const json::Value* v = kwarg(kwargs, name)) {
    return C::decode(*v);
  }
  return std::nullopt;
}

template <class T>
const T* ptr(const std::optional<T>& v) {
  return v ? &*v : nullptr;
}

}  // namespace

passes::StageOutput run_tac_to_asm(const capture::Call& call, const passes::AsmContext& ctx,
                                   const json::Value& args, const json::Value& kwargs) {
  const auto prog = asdl::load<tac_ast::Program>(call.input);
  std::optional<passes::SymbolTable> symbols;
  std::string before;
  if (const json::Value* v = positional(args, 0)) {
    symbols = SymbolTableC::decode(*v);
    before = encode<SymbolTableC>(*symbols);
  }
  std::optional<passes::TypeTable> types;
  if (const json::Value* v = positional(args, 1)) {
    types = TypeTableC::decode(*v);
  }
  const auto bare_exit = decode_kwarg<BoolC>(kwargs, "bare_exit");
  const auto abi = decode_kwarg<AbiC>(kwargs, "abi");
  const auto recursion = decode_kwarg<DictOf<Str, NameSetC>>(kwargs, "recursion_saves");
  const auto bindings = decode_kwarg<BindingsC>(kwargs, "bindings");
  const auto pinned = decode_kwarg<DictOf<Str, NameSetC>>(kwargs, "pinned_copies");
  const auto aliases = decode_kwarg<DictOf<Str, Str>>(kwargs, "slot_aliases");
  const Options options{.bare_exit = bare_exit.value_or(false),
                        .abi = ptr(abi),
                        .recursion_saves = ptr(recursion),
                        .bindings = ptr(bindings),
                        .pinned_copies = ptr(pinned),
                        .slot_aliases = ptr(aliases)};
  const asm_ast::Program out =
      translate_program(prog, symbols ? &*symbols : nullptr, ptr(types), ctx, options);
  std::string mutated;
  if (symbols) {
    const std::string after = encode<SymbolTableC>(*symbols);
    if (after != before) {
      mutated = "{\"args\": {\"0\": " + after + "}, \"kwargs\": {}}";
    }
  }
  return {asdl::dump(out), mutated};
}

}  // namespace c6502::tac_to_asm
