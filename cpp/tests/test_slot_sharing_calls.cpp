// The calls that the tests of tests/test_slot_sharing.py made of the asm passes,
// code generation's stage calls, and emission, each with what the
// Python function returned or raised, which the C++ port must
// return or raise too. tools/record_test_calls.py recorded them
// while the Python tests ran, before they were removed. A call
// that another C++ case already checks isn't repeated here.
#include "doctest.h"
#include "record_case.h"

TEST_CASE("format_metadata: test_slot_sharing.py::TestMetadata::test_share_record_round_trips (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass format_metadata\n"
      "source tests/test_slot_sharing.py::TestMetadata::test_share_record_round_trips\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "{\"object\": \"passes.zp_link_metadata.LinkMetadata\", \"fields\": {\"defs\": [], \"externs\": [], \"calls\": [], \"saved_calls\": [], \"zp_objects\": [], \"hargs_size\": null, \"hargs_need\": null, \"items\": [], \"shares\": [{\"object\": \"passes.slot_sharing.SlotShare\", \"fields\": {\"members\": {\"tuple\": [{\"tuple\": [\"f\", {\"tuple\": [\"__zpabi_f__p_0\", \"__zpabi_f__p_1\"]}]}, {\"tuple\": [\"g\", {\"tuple\": [\"__zpabi_g__q_0\", \"__zpabi_g__q_1\"]}]}]}, \"holders\": {\"tuple\": [\"main\"]}, \"live\": {\"tuple\": [{\"tuple\": [\"main\", {\"tuple\": [\"f\", \"h\"]}]}]}, \"forwards\": {\"tuple\": [{\"tuple\": [\"f\", \"g\"]}, {\"tuple\": [\"main\", \"f\"]}]}, \"writers\": {\"tuple\": [\"main\"]}}}]}}\n"
      "output-json\n"
      "[\"; @zp-link-meta-begin\", \"; share member=f:__zpabi_f__p_0,__zpabi_f__p_1 member=g:__zpabi_g__q_0,__zpabi_g__q_1 holder=main live=main:f,h forward=f>g forward=main>f writer=main\", \"; @zp-link-meta-end\"]\n");
}
