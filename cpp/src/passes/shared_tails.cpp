// Shares the code two functions of a unit end the same way with.
//
// A `JMP` can't reach a label inside another function, since dasm and
// binutils/assembler.h scope a `.` label to its function, and a label
// of any other kind starts an item the linker may move. So the shared
// instructions become a function of their own, `F@tail<n>`, internal
// to the unit, and each function that ended with them ends with `JMP
// F@tail<n>` instead.
//
// Conditions
//
// A segment is a run of instructions that ends with a `JMP` or an
// `RTS`, doesn't start with a label, and whose branches and jumps to
// `.` labels all go to labels inside it. No instruction outside a
// segment may go to a label inside it. Two segments match when they're
// the same instructions with their labels renamed consistently. Neither
// function may have a soft-stack frame (a `FunctionPrologue`), whose
// `Ret` depends on the function's own frame, and a segment may not hold
// a `JumpTable`, whose table is named for its function.
//
// The instructions are the same, so they do the same thing in either
// function: they name only registers, globals, and other functions'
// slots, since a function's own slots and labels are spelled with its
// name. A `(zp),Y` pointer is the exception, since the operand carries
// the pointer's address, and `asm_emit` names it by the slot the
// function it's in keeps there. Two functions' slots can share those
// bytes in a unit and be placed apart by the linker, so a segment may
// not read or write through a pointer in a slot's bytes
// (`asm_aliasing.through_slot_pointer`). The new function has no
// parameters or locals, and the link metadata's call graph, built from
// the TAC, already has every call it makes as a call of the functions
// it came from.
//
// A segment of `s` bytes shared by `n` functions takes `s + 3n` bytes
// in place of `n * s`, and the linker saves one of the `JMP`s. The pass
// shares a segment when that's smaller before the link too, when `s +
// 3n < n * s`. It looks for the most bytes saved first, adds every
// other function that ends with the same segment, and repeats.
#include "passes/shared_tails.h"

#include <cstddef>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "passes/asm_aliasing.h"
#include "sim/assembler.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::asm_ast;
using ::c6502::asdl::as;
using ::c6502::asdl::isa;

using Instructions = std::vector<ir::Type_instruction>;
using Shape = std::string;
using Form = std::vector<std::string>;

bool is_end(const ir::Type_instruction& ins) {
  return isa<ir::Jump, ir::Return>(ins);
}

// Returns `text` with the character `tag` in front. GCC 12 reports a
// false -Wrestrict for `"x" + std::string`, which this avoids.
std::string tagged(char tag, const std::string& text) {
  std::string out(1, tag);
  out += text;
  return out;
}

Shape shape(const ir::Type_instruction& ins) {
  if (isa<ir::Label>(ins)) {
    return "L";
  }
  if (const auto* br = as<ir::Branch>(ins)) {
    return tagged('B', std::to_string(br->cond.index()));
  }
  if (const auto* jump = as<ir::Jump>(ins); jump && jump->target.starts_with(".")) {
    return "J";
  }
  std::string out = tagged('I', asdl::dump(ins));
  out.pop_back();
  return out;
}

class FnInfo {
 public:
  FnInfo(const ir::Function& fn, const NameSet* zp_names,
         const std::set<asdl::Int>& slot_addrs)
      : fn_(&fn), zp_names_(zp_names) {
    const Instructions& instrs = fn.instructions;
    eligible = true;
    for (const auto& ins : instrs) {
      if (isa<ir::FunctionPrologue>(ins)) {
        eligible = false;
      }
    }
    shapes.reserve(instrs.size());
    blocked.reserve(instrs.size());
    for (std::size_t k = 0; k < instrs.size(); ++k) {
      const auto& ins = instrs[k];
      shapes.push_back(shape(ins));
      blocked.push_back(isa<ir::JumpTable>(ins) ||
                        through_slot_pointer(ins, slot_addrs));
      if (is_end(ins)) {
        ends.push_back(k);
        ends_by_shape[shapes.back()].push_back(k);
      }
      if (const auto* br = as<ir::Branch>(ins)) {
        refs_[br->target].push_back(k);
      } else if (const auto* jump = as<ir::Jump>(ins)) {
        refs_[jump->target].push_back(k);
      } else if (const auto* table = as<ir::JumpTable>(ins)) {
        for (const auto& x : table->targets) {
          refs_[x].push_back(k);
        }
      }
    }
  }

  void rebind(const ir::Function& fn) { fn_ = &fn; }
  const ir::Function& fn() const { return *fn_; }

  const std::optional<Form>& form(long s, long t) {
    const auto key = std::make_pair(s, t);
    auto it = forms_.find(key);
    if (it == forms_.end()) {
      it = forms_.emplace(key, compute_form(s, t)).first;
    }
    return it->second;
  }

  long size(long s, long t) {
    const auto key = std::make_pair(s, t);
    auto it = sizes_.find(key);
    if (it == sizes_.end()) {
      long total = 0;
      for (long k = s; k <= t; ++k) {
        total += sim::instruction_size(fn_->instructions[k], zp_names_);
      }
      it = sizes_.emplace(key, total).first;
    }
    return it->second;
  }

  bool eligible = true;
  std::vector<Shape> shapes;
  std::vector<bool> blocked;
  std::vector<std::size_t> ends;
  std::map<Shape, std::vector<std::size_t>> ends_by_shape;

 private:
  std::optional<Form> compute_form(long s, long t) const {
    const Instructions& instrs = fn_->instructions;
    if (s > t || s < 0 || t >= static_cast<long>(instrs.size())) {
      return std::nullopt;
    }
    if (isa<ir::Label>(instrs[s]) || !is_end(instrs[t])) {
      return std::nullopt;
    }
    std::unordered_map<std::string, long> inside;
    std::vector<std::string> inside_order;
    long n = 0;
    for (long k = s; k <= t; ++k) {
      if (const auto* label = as<ir::Label>(instrs[k])) {
        if (!inside.contains(label->name)) {
          inside_order.push_back(label->name);
        }
        inside[label->name] = n;
        n += 1;
      }
    }
    Form out;
    out.reserve(static_cast<std::size_t>(t - s + 1));
    for (long k = s; k <= t; ++k) {
      if (blocked[k]) {
        return std::nullopt;
      }
      const auto& ins = instrs[k];
      if (const auto* label = as<ir::Label>(ins)) {
        out.push_back(tagged('L', std::to_string(inside.at(label->name))));
      } else if (const auto* br = as<ir::Branch>(ins)) {
        const auto it = inside.find(br->target);
        if (it == inside.end()) {
          return std::nullopt;
        }
        out.push_back(shapes[k] + ":" + std::to_string(it->second));
      } else if (const auto* jump = as<ir::Jump>(ins);
                 jump && jump->target.starts_with(".")) {
        const auto it = inside.find(jump->target);
        if (it == inside.end()) {
          return std::nullopt;
        }
        out.push_back(tagged('J', std::to_string(it->second)));
      } else {
        out.push_back(shapes[k]);
      }
    }
    for (const auto& name : inside_order) {
      const auto it = refs_.find(name);
      if (it == refs_.end()) {
        continue;
      }
      for (const std::size_t k : it->second) {
        if (static_cast<long>(k) < s || static_cast<long>(k) > t) {
          return std::nullopt;
        }
      }
    }
    return out;
  }

  const ir::Function* fn_;
  const NameSet* zp_names_;
  std::unordered_map<std::string, std::vector<std::size_t>> refs_;
  std::map<std::pair<long, long>, std::optional<Form>> forms_;
  std::map<std::pair<long, long>, long> sizes_;
};

struct Span {
  long start = 0;
  long end = 0;
};

struct Share {
  std::string owner;
  std::map<std::size_t, Span> segments;
};

struct Common {
  long sa;
  long sb;
  Form form;
};

std::optional<Common> common(FnInfo& a, long ta, FnInfo& b, long tb) {
  long k = 0;
  while (ta - k >= 0 && tb - k >= 0 && a.shapes[ta - k] == b.shapes[tb - k]) {
    k += 1;
  }
  for (long length = k; length > 0; --length) {
    const long sa = ta - length + 1;
    const long sb = tb - length + 1;
    const auto& form = a.form(sa, ta);
    if (form && form == b.form(sb, tb)) {
      return Common{.sa = sa, .sb = sb, .form = *form};
    }
  }
  return std::nullopt;
}

using Enders = std::vector<std::pair<std::size_t, long>>;

std::optional<Share> best_share(const std::vector<ir::Type_top_level>& top,
                                const NameSet* zp_names,
                                const std::set<asdl::Int>& slot_addrs,
                                std::vector<std::unique_ptr<FnInfo>>& infos) {
  std::vector<std::pair<std::size_t, FnInfo*>> fns;
  for (std::size_t i = 0; i < top.size(); ++i) {
    const auto* fn = as<ir::Function>(top[i]);
    if (fn == nullptr) {
      continue;
    }
    if (!infos[i]) {
      infos[i] = std::make_unique<FnInfo>(*fn, zp_names, slot_addrs);
    } else {
      infos[i]->rebind(*fn);
    }
    if (infos[i]->eligible) {
      fns.emplace_back(i, infos[i].get());
    }
  }
  std::map<long, std::map<Form, Enders>> by_length;
  const auto ends_with = [&](long length, const Form& form) -> const Enders& {
    auto found = by_length.find(length);
    if (found == by_length.end()) {
      auto& index = by_length[length];
      for (const auto& [fc, c] : fns) {
        for (const std::size_t tc_u : c->ends) {
          const long tc = static_cast<long>(tc_u);
          const auto& f = c->form(tc - length + 1, tc);
          if (!f) {
            continue;
          }
          Enders& enders = index[*f];
          bool seen = false;
          for (const auto& [fx, tx] : enders) {
            seen = seen || fx == fc;
          }
          if (!seen) {
            enders.emplace_back(fc, tc);
          }
        }
      }
      found = by_length.find(length);
    }
    static const Enders kNone;
    const auto it = found->second.find(form);
    return it == found->second.end() ? kNone : it->second;
  };
  std::optional<Share> best;
  long best_gain = 0;
  for (std::size_t ai = 0; ai < fns.size(); ++ai) {
    const auto [fa, a] = fns[ai];
    for (std::size_t bi = ai + 1; bi < fns.size(); ++bi) {
      const auto [fb, b] = fns[bi];
      for (const std::size_t ta_u : a->ends) {
        const auto shaped = b->ends_by_shape.find(a->shapes[ta_u]);
        if (shaped == b->ends_by_shape.end()) {
          continue;
        }
        for (const std::size_t tb_u : shaped->second) {
          const long ta = static_cast<long>(ta_u);
          const long tb = static_cast<long>(tb_u);
          const auto seg = common(*a, ta, *b, tb);
          if (!seg) {
            continue;
          }
          const long size = a->size(seg->sa, ta);
          const long length = ta - seg->sa + 1;
          const Enders& others = ends_with(length, seg->form);
          long n = 2 + static_cast<long>(others.size());
          for (const auto& [fc, tc] : others) {
            if (fc == fa || fc == fb) {
              n -= 1;
            }
          }
          const long gain = n * size - (size + 3 * n);
          if (gain > best_gain) {
            best_gain = gain;
            Share share{.owner = a->fn().name};
            share.segments[fa] = Span{seg->sa, ta};
            share.segments[fb] = Span{seg->sb, tb};
            for (const auto& [fc, tc] : others) {
              if (!share.segments.contains(fc)) {
                share.segments[fc] = Span{tc - length + 1, tc};
              }
            }
            best = std::move(share);
          }
        }
      }
    }
  }
  return best;
}

}  // namespace

asm_ast::Program apply_shared_tails(asm_ast::Program prog,
                                    const NameSet* zp_names,
                                    const std::set<asdl::Int>& slot_addrs,
                                    std::vector<NewFunction>* new_functions) {
  std::vector<ir::Type_top_level>& top = prog.top_level;
  std::vector<std::unique_ptr<FnInfo>> infos(top.size());
  long count = 0;
  while (true) {
    auto best = best_share(top, zp_names, slot_addrs, infos);
    if (!best) {
      return prog;
    }
    const std::string name = best->owner + "@tail" + std::to_string(count);
    count += 1;
    std::optional<Instructions> body;
    for (const auto& [fi, span] : best->segments) {
      auto& fn = std::get<ir::Function>(top[fi]);
      auto& instrs = fn.instructions;
      if (!body) {
        body.emplace(instrs.begin() + span.start, instrs.begin() + span.end + 1);
      }
      Instructions out(instrs.begin(), instrs.begin() + span.start);
      out.push_back(ir::Jump{.target = name, .reg_args = {"A", "X", "Y"}});
      out.insert(out.end(), std::make_move_iterator(instrs.begin() + span.end + 1),
                 std::make_move_iterator(instrs.end()));
      instrs = std::move(out);
      infos[fi].reset();
    }
    if (new_functions != nullptr) {
      new_functions->push_back(NewFunction{.name = name, .owner = best->owner});
    }
    std::size_t at = 0;
    for (std::size_t i = 0; i < top.size(); ++i) {
      const auto* fn = as<ir::Function>(top[i]);
      if (fn != nullptr && fn->name == best->owner) {
        at = i;
        break;
      }
    }
    top.insert(top.begin() + static_cast<long>(at) + 1,
               ir::Function{.name = name, .is_global = false,
                            .instructions = std::move(*body)});
    infos.insert(infos.begin() + static_cast<long>(at) + 1, nullptr);
  }
}

}  // namespace c6502::passes
