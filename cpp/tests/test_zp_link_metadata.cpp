// The Python unit tests of `parse_metadata` (passes/zp_link_metadata.py),
// ported by hand, one case per test. Each round trip formats the
// test's metadata, built from its tagged JSON, parses the text back,
// and checks that the result's JSON is Python's, which is the JSON of
// the metadata the test started from. The formatted lines are
// checked by the recorded calls of `format_metadata` in
// test_zp_link_metadata_calls.cpp and test_slot_sharing_calls.cpp.
#include <string>
#include <vector>

#include "doctest.h"
#include "errors.h"
#include "json.h"
#include "passes/emission_args.h"
#include "passes/stage_registry_sharing.h"
#include "passes/zp_link_metadata.h"

namespace {

using c6502::passes::format_metadata;
using c6502::passes::parse_metadata;

// Formats the metadata whose tagged JSON is `meta_json`, checks that
// the lines joined with LF are `text`, and checks that parsing them
// gives back the same JSON.
void check_round_trip(const std::string& meta_json, const std::string& text) {
  const auto meta = c6502::passes::emission_args::link_metadata(c6502::json::parse(meta_json));
  CHECK(c6502::passes::stage_sharing::link_metadata_json(meta) == meta_json);
  std::string joined;
  for (const auto& line : format_metadata(meta)) {
    if (!joined.empty()) joined += '\n';
    joined += line;
  }
  CHECK(joined == text);
  CHECK(c6502::passes::stage_sharing::link_metadata_json(parse_metadata(joined)) == meta_json);
}

// Returns "<type>: <message>" of the error parsing `text` gives, or "".
std::string error_of(const std::string& text) {
  try {
    parse_metadata(text);
  } catch (const c6502::CompileError& e) {
    return e.python_type() + ": " + e.what();
  }
  return "";
}

}  // namespace

TEST_CASE("zp_link_metadata: test_zp_link_metadata.py::TestRoundTrip::test_format_then_parse_matches") {
  check_round_trip(
      "{\"object\": \"passes.zp_link_metadata.LinkMetadata\", \"fields\": {\"defs\": [{\"object\": \"passes.zp_link_metadata.FunctionMeta\", \"fields\": {\"name\": \"caller\", \"params\": [\"__zpabi_caller__x_0\", \"__zpabi_caller__x_1\"], \"locals\": [\"__local_caller__sprite_x\", \"__local_caller__sprite_y\", \"__local_caller__0\"], \"indirect\": false, \"in_cycle\": false, \"param_regs\": [], \"pinned\": [], \"changes\": [], \"carry\": false}}, {\"object\": \"passes.zp_link_metadata.FunctionMeta\", \"fields\": {\"name\": \"other\", \"params\": [], \"locals\": [], \"indirect\": true, \"in_cycle\": false, \"param_regs\": [], \"pinned\": [], \"changes\": [], \"carry\": false}}], \"externs\": [{\"object\": \"passes.zp_link_metadata.ExternMeta\", \"fields\": {\"name\": \"helper\", \"params\": [\"__zpabi_helper__a_0\", \"__zpabi_helper__a_1\", \"__zpabi_helper__b_0\", \"__zpabi_helper__b_1\"], \"param_regs\": [], \"pinned\": [], \"carry\": false}}], \"calls\": [{\"tuple\": [\"caller\", \"helper\"]}, {\"tuple\": [\"other\", \"caller\"]}], \"saved_calls\": [], \"zp_objects\": [], \"hargs_size\": null, \"hargs_need\": null, \"items\": [], \"shares\": []}}",
      "; @zp-link-meta-begin\n"
      "; def caller params=__zpabi_caller__x_0,__zpabi_caller__x_1 locals=__local_caller__sprite_x,__local_caller__sprite_y,__local_caller__0 indirect=false in_cycle=false\n"
      "; def other params= locals= indirect=true in_cycle=false\n"
      "; ext helper params=__zpabi_helper__a_0,__zpabi_helper__a_1,__zpabi_helper__b_0,__zpabi_helper__b_1\n"
      "; call caller -> helper\n"
      "; call other -> caller\n"
      "; @zp-link-meta-end");
}

TEST_CASE("zp_link_metadata: test_zp_link_metadata.py::TestRoundTrip::test_zero_page_objects_and_runtime_round_trip") {
  check_round_trip(
      "{\"object\": \"passes.zp_link_metadata.LinkMetadata\", \"fields\": {\"defs\": [], \"externs\": [], \"calls\": [], \"saved_calls\": [], \"zp_objects\": [{\"object\": \"passes.zp_link_metadata.ZpObjectMeta\", \"fields\": {\"name\": \"ptr\", \"size\": 2, \"at\": null, \"defined\": true, \"init\": [\"<buf\", \">(buf+2)\"], \"weight\": null, \"local\": false}}, {\"object\": \"passes.zp_link_metadata.ZpObjectMeta\", \"fields\": {\"name\": \"io\", \"size\": 1, \"at\": 144, \"defined\": false, \"init\": [], \"weight\": null, \"local\": false}}], \"hargs_size\": 12, \"hargs_need\": null, \"items\": [], \"shares\": []}}",
      "; @zp-link-meta-begin\n"
      "; zp ptr size=2 at=auto def=true init=<buf,>(buf+2)\n"
      "; zp io size=1 at=$90 def=false init=\n"
      "; runtime hargs_size=12\n"
      "; @zp-link-meta-end");
}

TEST_CASE("zp_link_metadata: test_slot_sharing.py::TestMetadata::test_share_record_round_trips") {
  check_round_trip(
      "{\"object\": \"passes.zp_link_metadata.LinkMetadata\", \"fields\": {\"defs\": [], \"externs\": [], \"calls\": [], \"saved_calls\": [], \"zp_objects\": [], \"hargs_size\": null, \"hargs_need\": null, \"items\": [], \"shares\": [{\"object\": \"passes.slot_sharing.SlotShare\", \"fields\": {\"members\": {\"tuple\": [{\"tuple\": [\"f\", {\"tuple\": [\"__zpabi_f__p_0\", \"__zpabi_f__p_1\"]}]}, {\"tuple\": [\"g\", {\"tuple\": [\"__zpabi_g__q_0\", \"__zpabi_g__q_1\"]}]}]}, \"holders\": {\"tuple\": [\"main\"]}, \"live\": {\"tuple\": [{\"tuple\": [\"main\", {\"tuple\": [\"f\", \"h\"]}]}]}, \"forwards\": {\"tuple\": [{\"tuple\": [\"f\", \"g\"]}, {\"tuple\": [\"main\", \"f\"]}]}, \"writers\": {\"tuple\": [\"main\"]}}}]}}",
      "; @zp-link-meta-begin\n"
      "; share member=f:__zpabi_f__p_0,__zpabi_f__p_1 member=g:__zpabi_g__q_0,__zpabi_g__q_1 holder=main live=main:f,h forward=f>g forward=main>f writer=main\n"
      "; @zp-link-meta-end");
}

TEST_CASE("zp_link_metadata: test_zp_link_metadata.py::TestRoundTrip::test_empty_input") {
  // No metadata block gives an empty LinkMetadata.
  CHECK(c6502::passes::stage_sharing::link_metadata_json(parse_metadata("clear_page1:\n   RTS\n")) ==
        c6502::passes::stage_sharing::link_metadata_json(c6502::passes::LinkMetadata{}));
  CHECK(c6502::passes::stage_sharing::link_metadata_json(c6502::passes::LinkMetadata{}) ==
        "{\"object\": \"passes.zp_link_metadata.LinkMetadata\", \"fields\": {\"defs\": [], \"externs\": [], \"calls\": [], \"saved_calls\": [], \"zp_objects\": [], \"hargs_size\": null, \"hargs_need\": null, \"items\": [], \"shares\": []}}");
}

TEST_CASE("zp_link_metadata: test_zp_link_metadata.py::TestRoundTrip::test_unknown_record_raises") {
  CHECK(error_of("; @zp-link-meta-begin\n"
                 "; mystery something\n"
                 "; @zp-link-meta-end\n") ==
        "ValueError: unknown record kind 'mystery': 'mystery something'");
}

TEST_CASE("zp_link_metadata: test_zp_link_metadata.py::TestRoundTrip::test_unmatched_begin") {
  CHECK(error_of("; @zp-link-meta-begin\n") ==
        "ValueError: @zp-link-meta-begin without matching @zp-link-meta-end");
}

TEST_CASE("zp_link_metadata: test_zp_link_metadata.py::TestRoundTrip::test_unmatched_end") {
  CHECK(error_of("; @zp-link-meta-end\n") ==
        "ValueError: @zp-link-meta-end without preceding @zp-link-meta-begin");
}
