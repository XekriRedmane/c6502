#include "passes/asm_context.h"

#include <utility>

namespace c6502::passes {

const asdl::Int* RuntimeLayout::addr(std::string_view name) const {
  if (name == "SSP") {
    return &ssp;
  }
  if (name == "FP") {
    return &fp;
  }
  if (name == "HARGS") {
    return &hargs;
  }
  if (name == "DPTR") {
    return &dptr;
  }
  return nullptr;
}

bool AsmContext::is_user_zp_global(std::string_view name) const {
  return user_zp->addrs.find(name) != user_zp->addrs.end();
}

const asdl::Int* AsmContext::user_zp_address(std::string_view name) const {
  const auto it = user_zp->addrs.find(name);
  return it == user_zp->addrs.end() ? nullptr : &it->second;
}

bool AsmContext::is_user_zp_byte(asdl::Int addr) const {
  return user_zp->bytes.count(addr) != 0;
}

bool AsmContext::in_pointer_proof(std::string_view name) const {
  return pointer_proof != nullptr &&
         pointer_proof->find(name) != pointer_proof->end();
}

AsmContext AsmContext::with_pointer_proof(NameSet names) const {
  AsmContext out = *this;
  out.pointer_proof = std::make_shared<const NameSet>(std::move(names));
  return out;
}

}  // namespace c6502::passes
