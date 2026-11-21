#include "c99_to_tac_stage.h"

#include <optional>
#include <string>

#include "c99_to_tac.h"
#include "tagged_json.h"

namespace c6502::c99_to_tac {

namespace {

using namespace ::c6502::tagged;

const json::Value* positional(const json::Value& args, std::size_t i) {
  if (i >= args.items.size() || args.items[i].kind == json::Value::Kind::kNull) {
    return nullptr;
  }
  return &args.items[i];
}

}  // namespace

passes::StageOutput run_c99_to_tac(const capture::Call& call, const passes::AsmContext&,
                                   const json::Value& args, const json::Value&) {
  auto prog = asdl::load<c99_ast::Program>(call.input);
  const std::string input_before = asdl::dump(prog);
  passes::SymbolTable symbols;
  if (const json::Value* v = positional(args, 0)) {
    symbols = SymbolTableC::decode(*v);
  }
  const std::string symbols_before = encode<SymbolTableC>(symbols);
  std::optional<passes::TypeTable> types;
  std::string types_before;
  if (const json::Value* v = positional(args, 1)) {
    types = TypeTableC::decode(*v);
    types_before = encode<TypeTableC>(*types);
  }
  const tac_ast::Program out = translate_program(prog, symbols, types ? &*types : nullptr);
  // The harness lists the changed arguments by position, and the
  // program under `input` before them when the call changed it.
  std::string changed;
  const std::string symbols_after = encode<SymbolTableC>(symbols);
  if (symbols_after != symbols_before) {
    changed += "\"0\": " + symbols_after;
  }
  if (types) {
    const std::string types_after = encode<TypeTableC>(*types);
    if (types_after != types_before) {
      if (!changed.empty()) {
        changed += ", ";
      }
      changed += "\"1\": " + types_after;
    }
  }
  std::string mutated;
  const bool input_changed = asdl::dump(prog) != input_before;
  if (input_changed || !changed.empty()) {
    mutated += '{';
    if (input_changed) {
      mutated += "\"input\": ";
      mutated += encode<C99ProgramC>(prog);
      mutated += ", ";
    }
    mutated += "\"args\": {";
    mutated += changed;
    mutated += "}, \"kwargs\": {}}";
  }
  return {asdl::dump(out), mutated};
}

}  // namespace c6502::c99_to_tac
