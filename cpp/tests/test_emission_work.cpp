// The calls that the unit tests of tests/test_prune_unused_slots.py, tests/test_zp_link_metadata.py, and tests/test_runtime_layout.py make of
// `prune_unused_locals`, `format_metadata`, and `hargs_need`, each with what the test gives the function
// and what the Python function returned or raised, which the C++
// port must return or throw too. They were recorded by wrapping the
// Python functions while their unit tests ran.
#include "doctest.h"
#include "call_case.h"

TEST_CASE("prune_unused_locals: TestPruneUnusedLocals.test_address_halves_keep_local (call 1)") {
  c6502::testing::check_call(
      "prune_unused_locals",
      "{\"args\": [{\"dict\": [[\"__local_main__a\", 134]]}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Mov(ImmLabelLow(\"__local_main__a\" 0) Reg(A) false)\n"
      "    Mov(ImmLabelHigh(\"__local_main__a\" 0) Reg(A) false)\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n",
      "{\"dict\": [[\"__local_main__a\", 134]]}\n");
}

TEST_CASE("prune_unused_locals: TestPruneUnusedLocals.test_unreferenced_local_dropped (call 1)") {
  c6502::testing::check_call(
      "prune_unused_locals",
      "{\"args\": [{\"dict\": [[\"__local_main__0\", 128]]}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n",
      "{\"dict\": []}\n");
}

TEST_CASE("format_metadata: TestRoundTrip.test_format_then_parse_matches (call 1)") {
  c6502::testing::check_call(
      "format_metadata",
      "{\"args\": [], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      true,
      "{\"object\": \"passes.zp_link_metadata.LinkMetadata\", \"fields\": {\"defs\": [{\"object\": \"passes.zp_link_metadata.FunctionMeta\", \"fields\": {\"name\": \"caller\", \"params\": [\"__zpabi_caller__x_0\", \"__zpabi_caller__x_1\"], \"locals\": [\"__local_caller__sprite_x\", \"__local_caller__sprite_y\", \"__local_caller__0\"], \"indirect\": false, \"in_cycle\": false, \"param_regs\": [], \"pinned\": [], \"changes\": [], \"carry\": false}}, {\"object\": \"passes.zp_link_metadata.FunctionMeta\", \"fields\": {\"name\": \"other\", \"params\": [], \"locals\": [], \"indirect\": true, \"in_cycle\": false, \"param_regs\": [], \"pinned\": [], \"changes\": [], \"carry\": false}}], \"externs\": [{\"object\": \"passes.zp_link_metadata.ExternMeta\", \"fields\": {\"name\": \"helper\", \"params\": [\"__zpabi_helper__a_0\", \"__zpabi_helper__a_1\", \"__zpabi_helper__b_0\", \"__zpabi_helper__b_1\"], \"param_regs\": [], \"pinned\": [], \"carry\": false}}], \"calls\": [{\"tuple\": [\"caller\", \"helper\"]}, {\"tuple\": [\"other\", \"caller\"]}], \"saved_calls\": [], \"zp_objects\": [], \"hargs_size\": null, \"hargs_need\": null, \"items\": [], \"shares\": []}}",
      "[\"; @zp-link-meta-begin\", \"; def caller params=__zpabi_caller__x_0,__zpabi_caller__x_1 locals=__local_caller__sprite_x,__local_caller__sprite_y,__local_caller__0 indirect=false in_cycle=false\", \"; def other params= locals= indirect=true in_cycle=false\", \"; ext helper params=__zpabi_helper__a_0,__zpabi_helper__a_1,__zpabi_helper__b_0,__zpabi_helper__b_1\", \"; call caller -> helper\", \"; call other -> caller\", \"; @zp-link-meta-end\"]\n");
}

TEST_CASE("format_metadata: TestRoundTrip.test_zero_page_objects_and_runtime_round_trip (call 1)") {
  c6502::testing::check_call(
      "format_metadata",
      "{\"args\": [], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      true,
      "{\"object\": \"passes.zp_link_metadata.LinkMetadata\", \"fields\": {\"defs\": [], \"externs\": [], \"calls\": [], \"saved_calls\": [], \"zp_objects\": [{\"object\": \"passes.zp_link_metadata.ZpObjectMeta\", \"fields\": {\"name\": \"ptr\", \"size\": 2, \"at\": null, \"defined\": true, \"init\": [\"<buf\", \">(buf+2)\"], \"weight\": null, \"local\": false}}, {\"object\": \"passes.zp_link_metadata.ZpObjectMeta\", \"fields\": {\"name\": \"io\", \"size\": 1, \"at\": 144, \"defined\": false, \"init\": [], \"weight\": null, \"local\": false}}], \"hargs_size\": 12, \"hargs_need\": null, \"items\": [], \"shares\": []}}",
      "[\"; @zp-link-meta-begin\", \"; zp ptr size=2 at=auto def=true init=<buf,>(buf+2)\", \"; zp io size=1 at=$90 def=false init=\", \"; runtime hargs_size=12\", \"; @zp-link-meta-end\"]\n");
}

TEST_CASE("hargs_need: TestHargsSize.test_need_counts_offsets_and_calls (call 1)") {
  c6502::testing::check_call(
      "hargs_need",
      "{\"args\": [], \"kwargs\": {\"footprints\": {\"dict\": [[\"mul8\", 5], [\"mul16\", 10], [\"mul32\", 20], [\"udivmod8\", 4], [\"sdivmod8\", 6], [\"udivmod16\", 9], [\"sdivmod16\", 11], [\"udivmod32\", 20], [\"sdivmod32\", 22], [\"asl8\", 3], [\"asr8\", 3], [\"lsr8\", 3], [\"asl16\", 5], [\"asr16\", 5], [\"lsr16\", 5], [\"asl32\", 9], [\"asr32\", 9], [\"lsr32\", 9], [\"bin16_to_bcd\", 7]]}}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"f\" true [] [])\n"
      "])\n",
      "0\n");
}

TEST_CASE("hargs_need: TestHargsSize.test_need_counts_offsets_and_calls (call 2)") {
  c6502::testing::check_call(
      "hargs_need",
      "{\"args\": [], \"kwargs\": {\"footprints\": {\"dict\": [[\"mul8\", 5], [\"mul16\", 10], [\"mul32\", 20], [\"udivmod8\", 4], [\"sdivmod8\", 6], [\"udivmod16\", 9], [\"sdivmod16\", 11], [\"udivmod32\", 20], [\"sdivmod32\", 22], [\"asl8\", 3], [\"asr8\", 3], [\"lsr8\", 3], [\"asl16\", 5], [\"asr16\", 5], [\"lsr16\", 5], [\"asl32\", 9], [\"asr32\", 9], [\"lsr32\", 9], [\"bin16_to_bcd\", 7]]}}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Reg(A) Data(\"HARGS\" 9) false)\n"
      "  ])\n"
      "])\n",
      "10\n");
}

TEST_CASE("hargs_need: TestHargsSize.test_need_counts_offsets_and_calls (call 3)") {
  c6502::testing::check_call(
      "hargs_need",
      "{\"args\": [], \"kwargs\": {\"footprints\": {\"dict\": [[\"mul8\", 5], [\"mul16\", 10], [\"mul32\", 20], [\"udivmod8\", 4], [\"sdivmod8\", 6], [\"udivmod16\", 9], [\"sdivmod16\", 11], [\"udivmod32\", 20], [\"sdivmod32\", 22], [\"asl8\", 3], [\"asr8\", 3], [\"lsr8\", 3], [\"asl16\", 5], [\"asr16\", 5], [\"lsr16\", 5], [\"asl32\", 9], [\"asr32\", 9], [\"lsr32\", 9], [\"bin16_to_bcd\", 7]]}}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Call(\"mul32\" [] [])\n"
      "  ])\n"
      "])\n",
      "20\n");
}
