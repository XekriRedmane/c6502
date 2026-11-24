// The calls that the tests of tests/test_zp_link_metadata.py made of the asm passes,
// code generation's stage calls, and emission, each with what the
// Python function returned or raised, which the C++ port must
// return or raise too. tools/record_test_calls.py recorded them
// while the Python tests ran, before they were removed. A call
// that another C++ case already checks isn't repeated here.
#include "doctest.h"
#include "record_case.h"

TEST_CASE("format_metadata: test_zp_link_metadata.py::TestRoundTrip::test_format_then_parse_matches (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass format_metadata\n"
      "source tests/test_zp_link_metadata.py::TestRoundTrip::test_format_then_parse_matches\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "{\"object\": \"passes.zp_link_metadata.LinkMetadata\", \"fields\": {\"defs\": [{\"object\": \"passes.zp_link_metadata.FunctionMeta\", \"fields\": {\"name\": \"caller\", \"params\": [\"__zpabi_caller__x_0\", \"__zpabi_caller__x_1\"], \"locals\": [\"__local_caller__sprite_x\", \"__local_caller__sprite_y\", \"__local_caller__0\"], \"indirect\": false, \"in_cycle\": false, \"param_regs\": [], \"pinned\": [], \"changes\": [], \"carry\": false}}, {\"object\": \"passes.zp_link_metadata.FunctionMeta\", \"fields\": {\"name\": \"other\", \"params\": [], \"locals\": [], \"indirect\": true, \"in_cycle\": false, \"param_regs\": [], \"pinned\": [], \"changes\": [], \"carry\": false}}], \"externs\": [{\"object\": \"passes.zp_link_metadata.ExternMeta\", \"fields\": {\"name\": \"helper\", \"params\": [\"__zpabi_helper__a_0\", \"__zpabi_helper__a_1\", \"__zpabi_helper__b_0\", \"__zpabi_helper__b_1\"], \"param_regs\": [], \"pinned\": [], \"carry\": false}}], \"calls\": [{\"tuple\": [\"caller\", \"helper\"]}, {\"tuple\": [\"other\", \"caller\"]}], \"saved_calls\": [], \"zp_objects\": [], \"hargs_size\": null, \"hargs_need\": null, \"items\": [], \"shares\": []}}\n"
      "output-json\n"
      "[\"; @zp-link-meta-begin\", \"; def caller params=__zpabi_caller__x_0,__zpabi_caller__x_1 locals=__local_caller__sprite_x,__local_caller__sprite_y,__local_caller__0 indirect=false in_cycle=false\", \"; def other params= locals= indirect=true in_cycle=false\", \"; ext helper params=__zpabi_helper__a_0,__zpabi_helper__a_1,__zpabi_helper__b_0,__zpabi_helper__b_1\", \"; call caller -> helper\", \"; call other -> caller\", \"; @zp-link-meta-end\"]\n");
}

TEST_CASE("format_metadata: test_zp_link_metadata.py::TestRoundTrip::test_zero_page_objects_and_runtime_round_trip (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass format_metadata\n"
      "source tests/test_zp_link_metadata.py::TestRoundTrip::test_zero_page_objects_and_runtime_round_trip\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "{\"object\": \"passes.zp_link_metadata.LinkMetadata\", \"fields\": {\"defs\": [], \"externs\": [], \"calls\": [], \"saved_calls\": [], \"zp_objects\": [{\"object\": \"passes.zp_link_metadata.ZpObjectMeta\", \"fields\": {\"name\": \"ptr\", \"size\": 2, \"at\": null, \"defined\": true, \"init\": [\"<buf\", \">(buf+2)\"], \"weight\": null, \"local\": false}}, {\"object\": \"passes.zp_link_metadata.ZpObjectMeta\", \"fields\": {\"name\": \"io\", \"size\": 1, \"at\": 144, \"defined\": false, \"init\": [], \"weight\": null, \"local\": false}}], \"hargs_size\": 12, \"hargs_need\": null, \"items\": [], \"shares\": []}}\n"
      "output-json\n"
      "[\"; @zp-link-meta-begin\", \"; zp ptr size=2 at=auto def=true init=<buf,>(buf+2)\", \"; zp io size=1 at=$90 def=false init=\", \"; runtime hargs_size=12\", \"; @zp-link-meta-end\"]\n");
}
