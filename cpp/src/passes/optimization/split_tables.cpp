#include "passes/optimization/split_tables.h"

#include <cstdint>
#include <string>

#include "errors.h"

namespace c6502::passes::split_tables {

namespace tac = ::c6502::tac_ast;

namespace {

// Appends the UTF-8 of the code point `cp`, which is below 256. A
// Python `str` that holds bytes holds them as the code points 0 to 255,
// and its C++ string holds their UTF-8.
void append_code_point(std::string& out, std::uint32_t cp) {
  if (cp < 0x80) {
    out += static_cast<char>(cp);
  } else {
    out += static_cast<char>(0xC0 | (cp >> 6));
    out += static_cast<char>(0x80 | (cp & 0x3F));
  }
}

}  // namespace

std::vector<tac::Type_static_init> plane_init(const std::vector<std::vector<ValueTree>>& planes) {
  std::vector<tac::Type_static_init> init;
  std::string run;
  asdl::Int run_bytes = 0;
  const auto flush = [&] {
    if (run_bytes != 0) {
      init.push_back(tac::StringInit{.str = run, .bytes = run_bytes});
      run.clear();
      run_bytes = 0;
    }
  };
  for (const auto& plane : planes) {
    for (const auto& c : plane) {
      if (c.kind == ValueTree::Kind::kInt) {
        if (c.integer < 0 || c.integer > 0xFF) {
          throw InternalError("plane_init: a cell that isn't a byte");
        }
        append_code_point(run, static_cast<std::uint32_t>(c.integer));
        ++run_bytes;
        continue;
      }
      flush();
      if (c.kind != ValueTree::Kind::kTacInit || !c.tac_init) {
        throw InternalError("plane_init: a cell that is neither a byte nor an initializer");
      }
      init.push_back(*c.tac_init);
    }
  }
  flush();
  return init;
}

}  // namespace c6502::passes::split_tables
