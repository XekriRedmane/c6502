#include "passes/preprocess_stage.h"

#include <stdexcept>
#include <string>
#include <vector>

#include "diagnostics.h"
#include "errors.h"
#include "preprocessor.h"
#include "stage_checks.h"
#include "tagged_json.h"

namespace c6502::passes::preprocess_stage {

StageOutput run(const capture::Call& call, const AsmContext&, const json::Value& args,
                const json::Value&) {
  using namespace ::c6502::tagged;
  if (args.items.size() != 2) {
    throw std::runtime_error("preprocess takes the arguments and the file name");
  }
  const std::string source = Str::decode(json::parse(call.input));
  const auto argv = ListOf<Str>::decode(args.items[0]);
  const auto filename = Optional<Str>::decode(args.items[1]);
  const pcpp::Host host = pcpp::native_host();
  const std::string include_dir = host.paths.join(host.paths.cwd, "include");
  StageOutput out;
  std::vector<diagnostics::CompileWarning> found;
  std::string err;
  {
    diagnostics::Collecting block(found);
    try {
      const auto result = preprocessor::preprocess(source, argv, filename, host, include_dir, &err);
      out.output = encode<Str>(result.text);
    } catch (const CompileError& e) {
      out.output = stage_checks::error_text(e.python_type(), e.what()) + "\n";
    }
  }
  if (!found.empty()) {
    std::vector<std::vector<std::string>> pairs;
    for (const auto& w : found) pairs.push_back({w.where, w.message});
    out.warnings = encode<ListOf<ListOf<Str>>>(pairs);
  }
  if (!err.empty()) out.stderr_json = encode<Str>(err);
  return out;
}

}  // namespace c6502::passes::preprocess_stage
