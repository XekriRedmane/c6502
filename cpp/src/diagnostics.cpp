#include "diagnostics.h"

namespace c6502::diagnostics {

namespace {

// The vector the innermost open `Collecting` block appends to, or null
// outside every such block.
thread_local std::vector<CompileWarning>* sink = nullptr;

}  // namespace

void warn(const std::string& where, const std::string& message) {
  if (sink != nullptr) {
    sink->push_back(CompileWarning{where, message});
  }
}

Collecting::Collecting(std::vector<CompileWarning>& found) : previous_(sink) { sink = &found; }

Collecting::~Collecting() { sink = previous_; }

}  // namespace c6502::diagnostics
