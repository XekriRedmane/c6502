#include "passes/linker_checks.h"

#include <optional>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "binutils/elf.h"
#include "errors.h"
#include "passes/emission_args.h"
#include "passes/linker_metadata.h"
#include "passes/slot_sharing.h"
#include "passes/stage_registry_sharing.h"
#include "pyrepr.h"
#include "tagged_json.h"

namespace c6502::passes::linker_checks {

namespace {

namespace elf = ::c6502::binutils::elf;
using asdl::Int;
using json::Value;
using Shares = tagged::ListOf<tagged::SlotShareC>;
using ZpRecords = tagged::ListOf<tagged::ZpObjectMetaC>;

const Value& field(const Value& v, std::string_view name) {
  const Value* f = v.get(name);
  if (f == nullptr) {
    throw InternalError("the record has no `" + std::string(name) + "`");
  }
  return *f;
}

std::vector<std::string> strings_of(const Value& v) {
  std::vector<std::string> out;
  for (const Value& s : v.items) {
    out.push_back(s.string);
  }
  return out;
}

std::optional<Int> optional_int(const Value& v) {
  return v.kind == Value::Kind::kNull ? std::nullopt : std::optional<Int>(v.integer);
}

std::set<Int> int_set(const Value& v) {
  std::set<Int> out;
  for (const Value& b : v.items) {
    out.insert(b.integer);
  }
  return out;
}

// --- The JSON of the results, as Python's `json.dumps` writes it ---

std::string strings_text(const std::vector<std::string>& names) {
  std::string out = "[";
  for (std::size_t i = 0; i < names.size(); ++i) {
    out += (i != 0 ? ", " : "") + json::quote(names[i]);
  }
  return out + "]";
}

std::string optional_text(const std::optional<Int>& v) {
  return v ? pyrepr::integer(*v) : "null";
}

std::string addrs_text(const OrderedMap<std::string, Int>& addrs) {
  std::string out = "[";
  bool first = true;
  for (const auto& [name, value] : addrs) {
    out += (first ? "[" : ", [") + json::quote(name) + ", " + pyrepr::integer(value) + "]";
    first = false;
  }
  return out + "]";
}

std::string metas_text(const std::vector<LinkMetadata>& metas) {
  std::string out = "[";
  for (std::size_t i = 0; i < metas.size(); ++i) {
    out += (i != 0 ? ", " : "") + stage_sharing::link_metadata_json(metas[i]);
  }
  return out + "]";
}

std::string zp_objects_text(const linker::ZpObjects& objects) {
  std::string out = "[";
  bool first = true;
  for (const auto& [name, records] : objects) {
    out += (first ? "[" : ", [") + json::quote(name) + ", " +
           tagged::encode<ZpRecords>(records) + "]";
    first = false;
  }
  return out + "]";
}

// --- Reading the inputs ---

std::vector<LinkMetadata> metas_of(const Value& v) {
  std::vector<LinkMetadata> out;
  for (const Value& m : v.items) {
    out.push_back(emission_args::link_metadata(m));
  }
  return out;
}

ExternMeta extern_of(const Value& v) {
  const Value& f = tagged::object_fields(v, "passes.zp_link_metadata.ExternMeta",
                                         {"name", "params", "param_regs", "pinned", "carry"});
  ExternMeta e;
  e.name = field(f, "name").string;
  e.params = strings_of(field(f, "params"));
  e.param_regs = strings_of(field(f, "param_regs"));
  for (const Value& p : field(f, "pinned").items) {
    e.pinned.push_back(optional_int(p));
  }
  e.carry = field(f, "carry").boolean;
  return e;
}

linker::ZpObjects zp_objects_of(const Value& v) {
  linker::ZpObjects out;
  for (const Value& pair : v.items) {
    out.insert_or_assign(pair.items.at(0).string, ZpRecords::decode(pair.items.at(1)));
  }
  return out;
}

OrderedMap<std::string, Int> addrs_of(const Value& v) {
  OrderedMap<std::string, Int> out;
  for (const Value& pair : v.items) {
    out.insert_or_assign(pair.items.at(0).string, pair.items.at(1).integer);
  }
  return out;
}

linker::Objects objects_of(const Value& v, const Blob& blob) {
  linker::Objects out;
  for (const Value& pair : v.items) {
    const std::string& name = pair.items.at(0).string;
    out.emplace_back(name, elf::read(blob(pair.items.at(1).string), name));
  }
  return out;
}

// Returns what differs between the objects and those the record names,
// compared byte for byte, or the empty string.
std::string compare_objects(const linker::Objects& objects, const Value& recorded,
                            const Blob& blob) {
  if (objects.size() != recorded.items.size()) {
    return "the number of objects differs";
  }
  for (std::size_t k = 0; k < objects.size(); ++k) {
    if (elf::write(objects[k].second) != blob(recorded.items[k].items.at(1).string)) {
      return "object " + std::to_string(k) + " (" + objects[k].first + ") differs";
    }
  }
  return "";
}

// Runs the function of a record of `kind` and returns the JSON text of
// its result, as the harness records it. `extra` is set to what differs
// in an object the function changed.
std::string run(std::string_view kind, const Value& args, const Value& input, const Value& output,
                const Blob& blob, std::string& extra) {
  if (kind == "link_merge") {
    return stage_sharing::link_metadata_json(linker::merge(metas_of(input)));
  }
  if (kind == "link_carry_conflict") {
    return json::quote(linker::carry_conflict(field(args, "name").string));
  }
  if (kind == "link_arrivals") {
    std::vector<std::optional<Int>> pinned;
    for (const Value& p : field(input, "pinned").items) {
      pinned.push_back(optional_int(p));
    }
    const linker::Arrivals got =
        linker::arrivals(static_cast<std::size_t>(field(input, "param_bytes").integer),
                         strings_of(field(input, "param_regs")), pinned);
    std::string out = "[";
    for (std::size_t i = 0; i < got.size(); ++i) {
      out += (i != 0 ? ", [" : "[") + json::quote(got[i].first) + ", " +
             optional_text(got[i].second) + "]";
    }
    return out + "]";
  }
  if (kind == "link_validate_eligibility") {
    linker::validate_eligibility(emission_args::link_metadata(input));
    return "null";
  }
  if (kind == "link_synthesize") {
    const linker::Synthesized s = linker::synthesize(emission_args::link_metadata(input));
    return "{\"prog\": " + tagged::encode<tagged::TacProgramC>(s.prog) +
           ", \"abi\": " + tagged::encode<tagged::AbiC>(s.abi) + ", \"local_bytes\": " +
           tagged::encode<tagged::DictOf<tagged::Str, tagged::IntC>>(s.local_bytes) + "}";
  }
  if (kind == "link_rename_locals") {
    linker::Objects objects = objects_of(field(input, "objects"), blob);
    std::vector<LinkMetadata> metas = metas_of(field(input, "metas"));
    const std::vector<std::string> taken_list = strings_of(field(args, "taken"));
    const NameSet taken(taken_list.begin(), taken_list.end());
    const std::vector<linker::Renames> renames = linker::rename_locals(objects, metas, taken);
    const Value* recorded = output.get("objects");
    if (recorded != nullptr && recorded->kind == Value::Kind::kArray) {
      extra = compare_objects(objects, *recorded, blob);
    }
    std::string out = "{\"renames\": [";
    for (std::size_t i = 0; i < renames.size(); ++i) {
      out += (i != 0 ? ", " : "") + [&] {
        std::string pairs = "[";
        bool first = true;
        for (const auto& [from, to] : renames[i]) {
          pairs += (first ? "[" : ", [") + json::quote(from) + ", " + json::quote(to) + "]";
          first = false;
        }
        return pairs + "]";
      }();
    }
    // The recorded objects are compared above, so the text takes them
    // from the record.
    return out + "], \"metas\": " + metas_text(metas) +
           ", \"objects\": " + (recorded != nullptr ? json::dump(*recorded) : "null") + "}";
  }
  if (kind == "link_rename_meta") {
    LinkMetadata meta = emission_args::link_metadata(field(input, "meta"));
    linker::Renames renames;
    for (const Value& pair : field(input, "renames").items) {
      renames.insert_or_assign(pair.items.at(0).string, pair.items.at(1).string);
    }
    linker::rename_meta(meta, renames);
    return stage_sharing::link_metadata_json(meta);
  }
  if (kind == "link_merge_zp_objects") {
    std::vector<LinkMetadata> metas;
    for (const Value& records : input.items) {
      LinkMetadata m;
      m.zp_objects = ZpRecords::decode(records);
      metas.push_back(std::move(m));
    }
    return zp_objects_text(linker::merge_zp_objects(metas));
  }
  if (kind == "link_place_zp_objects") {
    const std::vector<std::string> pointer_list = strings_of(field(input, "pointers"));
    const auto [addrs, demoted] = linker::place_zp_objects(
        zp_objects_of(field(input, "objects")), addrs_of(field(input, "layout")),
        int_set(field(input, "param_bytes")), NameSet(pointer_list.begin(), pointer_list.end()));
    return "[" + addrs_text(addrs) + ", " + strings_text(demoted) + "]";
  }
  if (kind == "link_choose_layout") {
    std::vector<LinkMetadata> metas;
    for (const Value& need : field(input, "hargs_needs").items) {
      LinkMetadata m;
      m.hargs_need = optional_int(need);
      metas.push_back(std::move(m));
    }
    return addrs_text(linker::choose_layout(metas, zp_objects_of(field(input, "objects")),
                                            field(args, "startup_need").integer,
                                            int_set(field(input, "param_bytes"))));
  }
  if (kind == "link_runtime_bytes") {
    std::string out = "[";
    bool first = true;
    for (Int b : linker::runtime_bytes(addrs_of(input))) {
      out += (first ? "" : ", ") + pyrepr::integer(b);
      first = false;
    }
    return out + "]";
  }
  if (kind == "link_shares") {
    return tagged::encode<Shares>(linker::link_shares(
        emission_args::link_metadata(field(input, "merged")), metas_of(field(input, "metas"))));
  }
  if (kind == "merge_shares") {
    OrderedMap<std::string, std::string> canonical;
    for (const Value& pair : field(input, "canonical").items) {
      canonical.insert_or_assign(pair.items.at(0).string, pair.items.at(1).string);
    }
    return tagged::encode<Shares>(merge_shares(Shares::decode(field(input, "shares")), canonical));
  }
  if (kind == "check_linked_shares") {
    std::vector<std::pair<std::string, std::string>> calls;
    for (const Value& c : field(input, "calls").items) {
      calls.emplace_back(c.items.at(0).string, c.items.at(1).string);
    }
    const std::vector<std::string> defined = strings_of(field(input, "defined"));
    OrderedMap<std::string, NameSet> changed;
    for (const Value& pair : field(input, "changed").items) {
      const std::vector<std::string> slots = strings_of(pair.items.at(1));
      changed.insert_or_assign(pair.items.at(0).string, NameSet(slots.begin(), slots.end()));
    }
    check_linked_shares(Shares::decode(field(input, "shares")), calls,
                        NameSet(defined.begin(), defined.end()), changed);
    return "null";
  }
  if (kind == "link_bind_extern_params") {
    std::vector<ExternMeta> externs;
    for (const Value& e : field(input, "externs").items) {
      externs.push_back(extern_of(e));
    }
    linker::SymbolAddrs symbols = addrs_of(field(input, "symbols"));
    linker::bind_extern_params(emission_args::link_metadata(field(input, "merged")), externs,
                               symbols);
    return addrs_text(symbols);
  }
  if (kind == "link_drop_unused_locals") {
    LinkMetadata merged = emission_args::link_metadata(field(input, "merged"));
    linker::drop_unused_locals(merged, objects_of(field(input, "objects"), blob));
    return stage_sharing::link_metadata_json(merged);
  }
  if (kind == "link_init_lines") {
    return strings_text(linker::init_lines(ZpRecords::decode(field(input, "objects")),
                                           strings_of(field(input, "startup"))));
  }
  if (kind == "link_data_lines") {
    return strings_text(linker::data_lines(tagged::ZpObjectMetaC::decode(input)));
  }
  if (kind == "link_regs") {
    return strings_text(
        linker::regs(strings_of(input), static_cast<std::size_t>(field(args, "count").integer)));
  }
  throw InternalError("no check for records of " + std::string(kind));
}

}  // namespace

bool handles(std::string_view kind) {
  return kind == "link_merge" || kind == "link_carry_conflict" || kind == "link_arrivals" ||
         kind == "link_validate_eligibility" || kind == "link_synthesize" ||
         kind == "link_rename_locals" || kind == "link_rename_meta" ||
         kind == "link_merge_zp_objects" || kind == "link_place_zp_objects" ||
         kind == "link_choose_layout" || kind == "link_runtime_bytes" ||
         kind == "link_shares" || kind == "merge_shares" || kind == "check_linked_shares" ||
         kind == "link_bind_extern_params" || kind == "link_drop_unused_locals" ||
         kind == "link_init_lines" || kind == "link_data_lines" || kind == "link_regs";
}

std::string check(std::string_view kind, const json::Value& args, const json::Value& input,
                  const json::Value& output, bool is_error, const Blob& blob) {
  std::string extra;
  std::string got;
  try {
    got = run(kind, args, input, output, blob, extra);
  } catch (const CompileError& e) {
    if (!is_error) {
      return std::string("C++ raised ") + e.python_type() + ": " + e.what();
    }
    if (e.python_type() != field(output, "type").string ||
        e.what() != field(output, "message").string) {
      return std::string("C++ raised ") + e.python_type() + ": " + e.what() + "\nPython raised " +
             field(output, "type").string + ": " + field(output, "message").string;
    }
    return "";
  }
  if (is_error) {
    return "C++ gave a result where Python raised " + field(output, "type").string + ": " +
           field(output, "message").string;
  }
  if (!extra.empty()) {
    return extra;
  }
  const std::string want = json::dump(output);
  if (got != want) {
    return "the result differs\nC++:    " + got + "\nPython: " + want;
  }
  return "";
}

}  // namespace c6502::passes::linker_checks
