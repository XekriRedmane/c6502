#include "binutils/assemble_command.h"

#include <cstdint>
#include <stdexcept>
#include <string_view>

#include "binutils/assembler.h"
#include "binutils/elf.h"
#include "errors.h"
#include "json.h"
#include "stage_checks.h"

namespace c6502::binutils {

namespace {

// Reads the lines and sections of a request in order.
class Request {
 public:
  explicit Request(const std::string& text) : text_(text) {}

  std::string_view next() {
    const std::size_t end = text_.find('\n', pos_);
    if (end == std::string::npos) throw std::runtime_error("the assemble request ends early");
    std::string_view line(text_.data() + pos_, end - pos_);
    pos_ = end + 1;
    return line;
  }

  std::string_view value(std::string_view key) {
    const std::string_view line = next();
    if (line.substr(0, key.size()) != key || line.size() <= key.size() || line[key.size()] != ' ') {
      throw std::runtime_error("the assemble request has no " + std::string(key) + " line");
    }
    return line.substr(key.size() + 1);
  }

  std::string_view section(std::string_view key) {
    const std::string length(value(key));
    std::size_t n = 0;
    try {
      n = static_cast<std::size_t>(std::stoull(length));
    } catch (const std::exception&) {
      throw std::runtime_error("the assemble request's " + std::string(key) +
                               " section has no length");
    }
    if (pos_ + n > text_.size()) {
      throw std::runtime_error("the assemble request's " + std::string(key) +
                               " section ends early");
    }
    std::string_view body(text_.data() + pos_, n);
    pos_ += n;
    return body;
  }

 private:
  const std::string& text_;
  std::size_t pos_ = 0;
};

}  // namespace

std::string assemble_response(const std::string& text) {
  Request request(text);
  if (request.next() != "c6502-assemble 1") {
    throw std::runtime_error("the assemble request doesn't start with c6502-assemble 1");
  }
  const json::Value args = json::parse(request.value("args"));
  const std::string body(request.section("text"));
  const json::Value* filename = args.get("filename");
  const json::Value* keep = args.get("keep_source");
  const json::Value* links = args.get("link_symbols");
  if (filename == nullptr || filename->kind != json::Value::Kind::kString || keep == nullptr ||
      keep->kind != json::Value::Kind::kBool || links == nullptr) {
    throw std::runtime_error("the assemble request's args need filename, keep_source, and "
                             "link_symbols");
  }
  assembler::LinkSymbols symbols;
  if (links->kind == json::Value::Kind::kArray) {
    for (const auto& pair : links->items) {
      if (pair.items.size() != 2) throw std::runtime_error("a link symbol isn't a pair");
      const json::Value& address = pair.items[1];
      symbols.emplace_back(pair.items[0].string,
                           address.kind == json::Value::Kind::kNull
                               ? std::nullopt
                               : std::optional<asdl::Int>(address.integer));
    }
  }
  std::string data;
  try {
    const elf::ElfFile obj = assembler::assemble(
        body, filename->string,
        links->kind == json::Value::Kind::kArray ? &symbols : nullptr, keep->boolean);
    check_write_fields(obj);
    data = elf::write(obj);
  } catch (const CompileError& e) {
    return "error " + stage_checks::error_text(e.python_type(), e.what()) + "\n";
  }
  return "object " + std::to_string(data.size()) + "\n" + data;
}

// Python packs each section's relocations first, then the symbols
// (`write` masks a value to 32 bits, but not a size), then the section
// headers' sizes, so the checks come in that order.
void check_write_fields(const elf::ElfFile& obj) {
  const auto out_of_range = [] { throw CompileError("struct.error", "argument out of range"); };
  for (const auto& s : obj.sections) {
    for (const auto& r : s->relocs) {
      if (r.offset < 0 || r.offset > UINT32_MAX) out_of_range();
      if (r.addend < INT32_MIN || r.addend > INT32_MAX) out_of_range();
    }
  }
  for (const bool locals : {true, false}) {
    for (const auto& sym : obj.symbols) {
      if (sym->is_global() == locals) continue;
      if (sym->size < 0 || sym->size > UINT32_MAX) out_of_range();
    }
  }
  for (const auto& s : obj.sections) {
    if (s->size() > UINT32_MAX) out_of_range();
  }
}

}  // namespace c6502::binutils
