SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__0	EQU	$80
__local_test_subtract_multi_dimensional__0	EQU	$80
__local_test_subtract_multi_dimensional__1	EQU	$81
__local_test_add_complex_expressions__0	EQU	$82
__local_test_add_constant_to_pointer__0	EQU	$82
__local_test_add_different_index_types__0	EQU	$82
__local_test_add_multi_dimensional__0	EQU	$82
__local_test_add_negative_index__0	EQU	$82
__local_test_add_pointer_to_int__0	EQU	$82
__local_test_add_to_subarray_pointer__0	EQU	$82
__local_test_subtract_complex_expressions__0	EQU	$82
__local_test_subtract_different_index_types__0	EQU	$82
__local_test_subtract_from_pointer__0	EQU	$82
__local_test_subtract_multi_dimensional__2	EQU	$82
__local_test_add_complex_expressions__1	EQU	$83
__local_test_add_constant_to_pointer__1	EQU	$83
__local_test_add_different_index_types__1	EQU	$83
__local_test_add_multi_dimensional__1	EQU	$83
__local_test_add_negative_index__1	EQU	$83
__local_test_add_to_subarray_pointer__1	EQU	$83
__local_test_subtract_complex_expressions__1	EQU	$83
__local_test_subtract_different_index_types__1	EQU	$83
__local_test_subtract_from_pointer__1	EQU	$83
__local_test_subtract_multi_dimensional__3	EQU	$83
__local_test_subtract_negative_index__1	EQU	$83
__local_test_add_complex_expressions__2	EQU	$84
__local_test_add_different_index_types__2	EQU	$84
__local_test_add_multi_dimensional__2	EQU	$84
__local_test_add_pointer_to_int__2	EQU	$84
__local_test_add_to_subarray_pointer__2	EQU	$84
__local_test_subtract_complex_expressions__2	EQU	$84
__local_test_subtract_different_index_types__2	EQU	$84
__local_test_subtract_from_pointer__2	EQU	$84
__local_test_subtract_multi_dimensional__4	EQU	$84
__local_test_subtract_negative_index__2	EQU	$84
__local_test_add_complex_expressions__3	EQU	$85
__local_test_add_constant_to_pointer__3	EQU	$85
__local_test_add_different_index_types__3	EQU	$85
__local_test_add_multi_dimensional__3	EQU	$85
__local_test_add_pointer_to_int__3	EQU	$85
__local_test_add_to_subarray_pointer__3	EQU	$85
__local_test_subtract_complex_expressions__3	EQU	$85
__local_test_subtract_different_index_types__3	EQU	$85
__local_test_subtract_from_pointer__3	EQU	$85
__local_test_subtract_multi_dimensional__5	EQU	$85
__local_test_subtract_negative_index__3	EQU	$85
__local_test_add_constant_to_pointer__4	EQU	$86
__local_test_add_different_index_types__4	EQU	$86
__local_test_add_multi_dimensional__4	EQU	$86
__local_test_add_negative_index__4	EQU	$86
__local_test_subtract_complex_expressions__4	EQU	$86
__local_test_subtract_different_index_types__4	EQU	$86
__local_test_subtract_from_pointer__4	EQU	$86
__local_test_subtract_multi_dimensional__6	EQU	$86
__local_test_subtract_negative_index__arr	EQU	$86
__local_test_add_constant_to_pointer__long_arr	EQU	$87
__local_test_add_different_index_types__5	EQU	$87
__local_test_add_multi_dimensional__5	EQU	$87
__local_test_add_negative_index__unsigned_arr	EQU	$87
__local_test_add_pointer_to_int__5	EQU	$87
__local_test_subtract_different_index_types__5	EQU	$87
__local_test_subtract_from_pointer__5	EQU	$87
__local_test_add_different_index_types__6	EQU	$88
__local_test_add_pointer_to_int__int_arr	EQU	$88
__local_test_add_to_subarray_pointer__nested_arr	EQU	$88
__local_test_subtract_different_index_types__6	EQU	$88
__local_test_subtract_multi_dimensional__8	EQU	$88
__local_test_add_different_index_types__7	EQU	$89
__local_test_subtract_different_index_types__7	EQU	$89
__local_test_subtract_from_pointer__7	EQU	$89
__local_test_subtract_multi_dimensional__nested_arr	EQU	$89
__local_test_add_different_index_types__8	EQU	$8A
__local_test_add_multi_dimensional__nested_arr	EQU	$8A
__local_test_subtract_different_index_types__8	EQU	$8A
__local_test_subtract_from_pointer__long_arr	EQU	$8A
__local_test_add_different_index_types__9	EQU	$8B
__local_test_subtract_different_index_types__9	EQU	$8B
__local_test_subtract_different_index_types__double_arr	EQU	$8C
__local_test_add_different_index_types__11	EQU	$8D
__local_test_add_different_index_types__double_arr	EQU	$8E

; @zp-link-meta-begin
; def get_elem1_ptr params=__zpabi_get_elem1_ptr__arr_0,__zpabi_get_elem1_ptr__arr_1 locals=__local_get_elem1_ptr__arr indirect=false in_cycle=false param_regs=A,X
; def get_elem2_ptr params=__zpabi_get_elem2_ptr__arr_0,__zpabi_get_elem2_ptr__arr_1 locals=__local_get_elem2_ptr__arr indirect=false in_cycle=false param_regs=A,X
; def main params= locals=__local_main__0,__local_main__1 indirect=false in_cycle=false
; def return_one params= locals= indirect=false in_cycle=false
; def test_add_complex_expressions params= locals=__local_test_add_complex_expressions__0,__local_test_add_complex_expressions__1,__local_test_add_complex_expressions__2,__local_test_add_complex_expressions__3,__local_test_add_complex_expressions__4,__local_test_add_complex_expressions__5,__local_test_add_complex_expressions__i,__local_test_add_complex_expressions__7 indirect=false in_cycle=false
; def test_add_constant_to_pointer params= locals=__local_test_add_constant_to_pointer__0,__local_test_add_constant_to_pointer__1,__local_test_add_constant_to_pointer__2,__local_test_add_constant_to_pointer__3,__local_test_add_constant_to_pointer__4,__local_test_add_constant_to_pointer__long_arr,__local_test_add_constant_to_pointer__6,__local_test_add_constant_to_pointer__7,__local_test_add_constant_to_pointer__8,__local_test_add_constant_to_pointer__9,__local_test_add_constant_to_pointer__10,__local_test_add_constant_to_pointer__11,__local_test_add_constant_to_pointer__12,__local_test_add_constant_to_pointer__13,__local_test_add_constant_to_pointer__14,__local_test_add_constant_to_pointer__15,__local_test_add_constant_to_pointer__16,__local_test_add_constant_to_pointer__17,__local_test_add_constant_to_pointer__18,__local_test_add_constant_to_pointer__19,__local_test_add_constant_to_pointer__20,__local_test_add_constant_to_pointer__21,__local_test_add_constant_to_pointer__22,__local_test_add_constant_to_pointer__23,__local_test_add_constant_to_pointer__24,__local_test_add_constant_to_pointer__25,__local_test_add_constant_to_pointer__26,__local_test_add_constant_to_pointer__27,__local_test_add_constant_to_pointer__28,__local_test_add_constant_to_pointer__29,__local_test_add_constant_to_pointer__30,__local_test_add_constant_to_pointer__31,__local_test_add_constant_to_pointer__32,__local_test_add_constant_to_pointer__33,__local_test_add_constant_to_pointer__34,__local_test_add_constant_to_pointer__35,__local_test_add_constant_to_pointer__36,__local_test_add_constant_to_pointer__37,__local_test_add_constant_to_pointer__38,__local_test_add_constant_to_pointer__39,__local_test_add_constant_to_pointer__40,__local_test_add_constant_to_pointer__41,__local_test_add_constant_to_pointer__42,__local_test_add_constant_to_pointer__43,__local_test_add_constant_to_pointer__44,__local_test_add_constant_to_pointer__45,__local_test_add_constant_to_pointer__46,__local_test_add_constant_to_pointer__47,__local_test_add_constant_to_pointer__48,__local_test_add_constant_to_pointer__49,__local_test_add_constant_to_pointer__50,__local_test_add_constant_to_pointer__51,__local_test_add_constant_to_pointer__52 indirect=false in_cycle=false
; def test_add_different_index_types params= locals=__local_test_add_different_index_types__0,__local_test_add_different_index_types__1,__local_test_add_different_index_types__2,__local_test_add_different_index_types__3,__local_test_add_different_index_types__4,__local_test_add_different_index_types__5,__local_test_add_different_index_types__6,__local_test_add_different_index_types__7,__local_test_add_different_index_types__8,__local_test_add_different_index_types__9,__local_test_add_different_index_types__10,__local_test_add_different_index_types__11,__local_test_add_different_index_types__double_arr,__local_test_add_different_index_types__13,__local_test_add_different_index_types__14,__local_test_add_different_index_types__15,__local_test_add_different_index_types__16,__local_test_add_different_index_types__17,__local_test_add_different_index_types__18,__local_test_add_different_index_types__19,__local_test_add_different_index_types__20,__local_test_add_different_index_types__21,__local_test_add_different_index_types__22,__local_test_add_different_index_types__23,__local_test_add_different_index_types__24,__local_test_add_different_index_types__25,__local_test_add_different_index_types__26,__local_test_add_different_index_types__27,__local_test_add_different_index_types__28,__local_test_add_different_index_types__29,__local_test_add_different_index_types__30,__local_test_add_different_index_types__31,__local_test_add_different_index_types__32,__local_test_add_different_index_types__33,__local_test_add_different_index_types__34,__local_test_add_different_index_types__35,__local_test_add_different_index_types__36,__local_test_add_different_index_types__37,__local_test_add_different_index_types__38,__local_test_add_different_index_types__39,__local_test_add_different_index_types__40,__local_test_add_different_index_types__41,__local_test_add_different_index_types__42,__local_test_add_different_index_types__43,__local_test_add_different_index_types__44,__local_test_add_different_index_types__45,__local_test_add_different_index_types__46,__local_test_add_different_index_types__47,__local_test_add_different_index_types__48,__local_test_add_different_index_types__49,__local_test_add_different_index_types__50,__local_test_add_different_index_types__51,__local_test_add_different_index_types__52,__local_test_add_different_index_types__53,__local_test_add_different_index_types__54,__local_test_add_different_index_types__55,__local_test_add_different_index_types__56,__local_test_add_different_index_types__57,__local_test_add_different_index_types__58,__local_test_add_different_index_types__59,__local_test_add_different_index_types__60,__local_test_add_different_index_types__61,__local_test_add_different_index_types__62,__local_test_add_different_index_types__63,__local_test_add_different_index_types__64,__local_test_add_different_index_types__65,__local_test_add_different_index_types__66,__local_test_add_different_index_types__67,__local_test_add_different_index_types__68,__local_test_add_different_index_types__69,__local_test_add_different_index_types__70,__local_test_add_different_index_types__71,__local_test_add_different_index_types__72,__local_test_add_different_index_types__73,__local_test_add_different_index_types__74,__local_test_add_different_index_types__75,__local_test_add_different_index_types__76,__local_test_add_different_index_types__77,__local_test_add_different_index_types__78,__local_test_add_different_index_types__79,__local_test_add_different_index_types__80,__local_test_add_different_index_types__81,__local_test_add_different_index_types__82,__local_test_add_different_index_types__83,__local_test_add_different_index_types__84,__local_test_add_different_index_types__85,__local_test_add_different_index_types__86,__local_test_add_different_index_types__87,__local_test_add_different_index_types__88,__local_test_add_different_index_types__89,__local_test_add_different_index_types__90,__local_test_add_different_index_types__91,__local_test_add_different_index_types__92,__local_test_add_different_index_types__93,__local_test_add_different_index_types__94,__local_test_add_different_index_types__95,__local_test_add_different_index_types__96,__local_test_add_different_index_types__97,__local_test_add_different_index_types__98,__local_test_add_different_index_types__99 indirect=false in_cycle=false
; def test_add_multi_dimensional params= locals=__local_test_add_multi_dimensional__0,__local_test_add_multi_dimensional__1,__local_test_add_multi_dimensional__2,__local_test_add_multi_dimensional__3,__local_test_add_multi_dimensional__4,__local_test_add_multi_dimensional__5,__local_test_add_multi_dimensional__6,__local_test_add_multi_dimensional__7,__local_test_add_multi_dimensional__nested_arr,__local_test_add_multi_dimensional__9,__local_test_add_multi_dimensional__10,__local_test_add_multi_dimensional__11,__local_test_add_multi_dimensional__12,__local_test_add_multi_dimensional__13,__local_test_add_multi_dimensional__14,__local_test_add_multi_dimensional__15,__local_test_add_multi_dimensional__16,__local_test_add_multi_dimensional__17,__local_test_add_multi_dimensional__18,__local_test_add_multi_dimensional__19,__local_test_add_multi_dimensional__20,__local_test_add_multi_dimensional__21,__local_test_add_multi_dimensional__22,__local_test_add_multi_dimensional__23,__local_test_add_multi_dimensional__24,__local_test_add_multi_dimensional__25 indirect=false in_cycle=false
; def test_add_negative_index params= locals=__local_test_add_negative_index__0,__local_test_add_negative_index__1,__local_test_add_negative_index__2,__local_test_add_negative_index__3,__local_test_add_negative_index__4,__local_test_add_negative_index__unsigned_arr,__local_test_add_negative_index__6,__local_test_add_negative_index__7,__local_test_add_negative_index__8,__local_test_add_negative_index__9,__local_test_add_negative_index__10,__local_test_add_negative_index__11,__local_test_add_negative_index__12,__local_test_add_negative_index__13,__local_test_add_negative_index__14,__local_test_add_negative_index__15,__local_test_add_negative_index__16,__local_test_add_negative_index__17,__local_test_add_negative_index__18,__local_test_add_negative_index__19,__local_test_add_negative_index__20,__local_test_add_negative_index__21,__local_test_add_negative_index__22,__local_test_add_negative_index__23,__local_test_add_negative_index__24,__local_test_add_negative_index__25,__local_test_add_negative_index__26,__local_test_add_negative_index__27,__local_test_add_negative_index__28 indirect=false in_cycle=false
; def test_add_pointer_to_int params= locals=__local_test_add_pointer_to_int__0,__local_test_add_pointer_to_int__1,__local_test_add_pointer_to_int__2,__local_test_add_pointer_to_int__3,__local_test_add_pointer_to_int__4,__local_test_add_pointer_to_int__5,__local_test_add_pointer_to_int__int_arr,__local_test_add_pointer_to_int__7,__local_test_add_pointer_to_int__8,__local_test_add_pointer_to_int__9,__local_test_add_pointer_to_int__10,__local_test_add_pointer_to_int__11,__local_test_add_pointer_to_int__12,__local_test_add_pointer_to_int__13,__local_test_add_pointer_to_int__14,__local_test_add_pointer_to_int__15 indirect=false in_cycle=false
; def test_add_to_subarray_pointer params= locals=__local_test_add_to_subarray_pointer__0,__local_test_add_to_subarray_pointer__1,__local_test_add_to_subarray_pointer__2,__local_test_add_to_subarray_pointer__3,__local_test_add_to_subarray_pointer__4,__local_test_add_to_subarray_pointer__5,__local_test_add_to_subarray_pointer__nested_arr,__local_test_add_to_subarray_pointer__7,__local_test_add_to_subarray_pointer__8,__local_test_add_to_subarray_pointer__9,__local_test_add_to_subarray_pointer__10,__local_test_add_to_subarray_pointer__11,__local_test_add_to_subarray_pointer__12,__local_test_add_to_subarray_pointer__13,__local_test_add_to_subarray_pointer__14,__local_test_add_to_subarray_pointer__15,__local_test_add_to_subarray_pointer__16,__local_test_add_to_subarray_pointer__17,__local_test_add_to_subarray_pointer__18,__local_test_add_to_subarray_pointer__19,__local_test_add_to_subarray_pointer__20,__local_test_add_to_subarray_pointer__21,__local_test_add_to_subarray_pointer__22,__local_test_add_to_subarray_pointer__23 indirect=false in_cycle=false
; def test_subtract_complex_expressions params= locals=__local_test_subtract_complex_expressions__0,__local_test_subtract_complex_expressions__1,__local_test_subtract_complex_expressions__2,__local_test_subtract_complex_expressions__3,__local_test_subtract_complex_expressions__4,__local_test_subtract_complex_expressions__5 indirect=false in_cycle=false
; def test_subtract_different_index_types params= locals=__local_test_subtract_different_index_types__0,__local_test_subtract_different_index_types__1,__local_test_subtract_different_index_types__2,__local_test_subtract_different_index_types__3,__local_test_subtract_different_index_types__4,__local_test_subtract_different_index_types__5,__local_test_subtract_different_index_types__6,__local_test_subtract_different_index_types__7,__local_test_subtract_different_index_types__8,__local_test_subtract_different_index_types__9,__local_test_subtract_different_index_types__double_arr,__local_test_subtract_different_index_types__11,__local_test_subtract_different_index_types__12,__local_test_subtract_different_index_types__13,__local_test_subtract_different_index_types__14,__local_test_subtract_different_index_types__15,__local_test_subtract_different_index_types__16,__local_test_subtract_different_index_types__17,__local_test_subtract_different_index_types__18,__local_test_subtract_different_index_types__19,__local_test_subtract_different_index_types__20,__local_test_subtract_different_index_types__21,__local_test_subtract_different_index_types__22,__local_test_subtract_different_index_types__23,__local_test_subtract_different_index_types__24,__local_test_subtract_different_index_types__25,__local_test_subtract_different_index_types__26,__local_test_subtract_different_index_types__27,__local_test_subtract_different_index_types__28,__local_test_subtract_different_index_types__29,__local_test_subtract_different_index_types__30,__local_test_subtract_different_index_types__31,__local_test_subtract_different_index_types__32,__local_test_subtract_different_index_types__33,__local_test_subtract_different_index_types__34,__local_test_subtract_different_index_types__35,__local_test_subtract_different_index_types__36,__local_test_subtract_different_index_types__37,__local_test_subtract_different_index_types__38,__local_test_subtract_different_index_types__39,__local_test_subtract_different_index_types__40,__local_test_subtract_different_index_types__41,__local_test_subtract_different_index_types__42,__local_test_subtract_different_index_types__43,__local_test_subtract_different_index_types__44,__local_test_subtract_different_index_types__45,__local_test_subtract_different_index_types__46,__local_test_subtract_different_index_types__47,__local_test_subtract_different_index_types__48,__local_test_subtract_different_index_types__49,__local_test_subtract_different_index_types__50,__local_test_subtract_different_index_types__51,__local_test_subtract_different_index_types__52,__local_test_subtract_different_index_types__53,__local_test_subtract_different_index_types__54,__local_test_subtract_different_index_types__55,__local_test_subtract_different_index_types__56,__local_test_subtract_different_index_types__57,__local_test_subtract_different_index_types__58,__local_test_subtract_different_index_types__59,__local_test_subtract_different_index_types__60,__local_test_subtract_different_index_types__61,__local_test_subtract_different_index_types__62,__local_test_subtract_different_index_types__63,__local_test_subtract_different_index_types__64,__local_test_subtract_different_index_types__65,__local_test_subtract_different_index_types__66,__local_test_subtract_different_index_types__67,__local_test_subtract_different_index_types__68,__local_test_subtract_different_index_types__69,__local_test_subtract_different_index_types__70,__local_test_subtract_different_index_types__71,__local_test_subtract_different_index_types__72,__local_test_subtract_different_index_types__73,__local_test_subtract_different_index_types__74,__local_test_subtract_different_index_types__75,__local_test_subtract_different_index_types__76,__local_test_subtract_different_index_types__77,__local_test_subtract_different_index_types__78,__local_test_subtract_different_index_types__79,__local_test_subtract_different_index_types__80,__local_test_subtract_different_index_types__81,__local_test_subtract_different_index_types__82,__local_test_subtract_different_index_types__83,__local_test_subtract_different_index_types__84,__local_test_subtract_different_index_types__85,__local_test_subtract_different_index_types__86,__local_test_subtract_different_index_types__87,__local_test_subtract_different_index_types__88,__local_test_subtract_different_index_types__89,__local_test_subtract_different_index_types__90,__local_test_subtract_different_index_types__91,__local_test_subtract_different_index_types__92,__local_test_subtract_different_index_types__93,__local_test_subtract_different_index_types__94,__local_test_subtract_different_index_types__95,__local_test_subtract_different_index_types__96,__local_test_subtract_different_index_types__97 indirect=false in_cycle=false
; def test_subtract_from_pointer params= locals=__local_test_subtract_from_pointer__0,__local_test_subtract_from_pointer__1,__local_test_subtract_from_pointer__2,__local_test_subtract_from_pointer__3,__local_test_subtract_from_pointer__4,__local_test_subtract_from_pointer__5,__local_test_subtract_from_pointer__6,__local_test_subtract_from_pointer__7,__local_test_subtract_from_pointer__long_arr,__local_test_subtract_from_pointer__9,__local_test_subtract_from_pointer__10,__local_test_subtract_from_pointer__11,__local_test_subtract_from_pointer__12,__local_test_subtract_from_pointer__13,__local_test_subtract_from_pointer__14,__local_test_subtract_from_pointer__15,__local_test_subtract_from_pointer__16,__local_test_subtract_from_pointer__17,__local_test_subtract_from_pointer__18,__local_test_subtract_from_pointer__19,__local_test_subtract_from_pointer__20,__local_test_subtract_from_pointer__21,__local_test_subtract_from_pointer__22,__local_test_subtract_from_pointer__23,__local_test_subtract_from_pointer__24,__local_test_subtract_from_pointer__25,__local_test_subtract_from_pointer__26,__local_test_subtract_from_pointer__27 indirect=false in_cycle=false
; def test_subtract_multi_dimensional params= locals=__local_test_subtract_multi_dimensional__0,__local_test_subtract_multi_dimensional__1,__local_test_subtract_multi_dimensional__2,__local_test_subtract_multi_dimensional__3,__local_test_subtract_multi_dimensional__4,__local_test_subtract_multi_dimensional__5,__local_test_subtract_multi_dimensional__6,__local_test_subtract_multi_dimensional__7,__local_test_subtract_multi_dimensional__8,__local_test_subtract_multi_dimensional__nested_arr,__local_test_subtract_multi_dimensional__10,__local_test_subtract_multi_dimensional__11,__local_test_subtract_multi_dimensional__12,__local_test_subtract_multi_dimensional__13,__local_test_subtract_multi_dimensional__14,__local_test_subtract_multi_dimensional__15,__local_test_subtract_multi_dimensional__16,__local_test_subtract_multi_dimensional__17,__local_test_subtract_multi_dimensional__18,__local_test_subtract_multi_dimensional__19,__local_test_subtract_multi_dimensional__20,__local_test_subtract_multi_dimensional__21,__local_test_subtract_multi_dimensional__22,__local_test_subtract_multi_dimensional__23,__local_test_subtract_multi_dimensional__24,__local_test_subtract_multi_dimensional__25,__local_test_subtract_multi_dimensional__26 indirect=false in_cycle=false
; def test_subtract_negative_index params= locals=__local_test_subtract_negative_index__0,__local_test_subtract_negative_index__1,__local_test_subtract_negative_index__2,__local_test_subtract_negative_index__3,__local_test_subtract_negative_index__arr,__local_test_subtract_negative_index__5,__local_test_subtract_negative_index__6,__local_test_subtract_negative_index__7,__local_test_subtract_negative_index__8,__local_test_subtract_negative_index__9,__local_test_subtract_negative_index__10,__local_test_subtract_negative_index__11,__local_test_subtract_negative_index__12,__local_test_subtract_negative_index__13 indirect=false in_cycle=false
; call main -> test_add_complex_expressions
; call main -> test_add_constant_to_pointer
; call main -> test_add_different_index_types
; call main -> test_add_multi_dimensional
; call main -> test_add_negative_index
; call main -> test_add_pointer_to_int
; call main -> test_add_to_subarray_pointer
; call main -> test_subtract_complex_expressions
; call main -> test_subtract_different_index_types
; call main -> test_subtract_from_pointer
; call main -> test_subtract_negative_index
; call test_add_complex_expressions -> get_elem1_ptr
; call test_add_complex_expressions -> get_elem2_ptr
; call test_add_complex_expressions -> return_one
; call test_subtract_complex_expressions -> get_elem1_ptr
; call test_subtract_complex_expressions -> get_elem2_ptr
; item test_add_constant_to_pointer section=.text local=false zero=false
; item test_add_negative_index section=.text local=false zero=false
; item test_add_pointer_to_int section=.text local=false zero=false
; item test_add_pointer_to_int@tail0 section=.text local=true zero=false
; item test_add_different_index_types section=.text local=false zero=false
; item test_add_different_index_types@run0 section=.text local=true zero=false
; item test_add_different_index_types@tail1 section=.text local=true zero=false
; item test_add_complex_expressions section=.text local=false zero=false
; item return_one section=.text local=false zero=false
; item get_elem1_ptr section=.text local=false zero=false
; item get_elem2_ptr section=.text local=false zero=false
; item test_add_multi_dimensional section=.text local=false zero=false
; item test_add_to_subarray_pointer section=.text local=false zero=false
; item test_subtract_from_pointer section=.text local=false zero=false
; item test_subtract_negative_index section=.text local=false zero=false
; item test_subtract_different_index_types section=.text local=false zero=false
; item test_subtract_complex_expressions section=.text local=false zero=false
; item test_subtract_multi_dimensional section=.text local=false zero=false
; item main section=.text local=false zero=false
; item @13.flag section=.bss local=true zero=true
; item @18.arr section=.data local=true zero=false
; item @22.index section=.data local=true zero=false
; item @25.index section=.data local=true zero=false
; item @31.index section=.data local=true zero=false
; item @41.flag section=.data local=true zero=false
; item @42.four section=.data local=true zero=false
; item @43.arr section=.data local=true zero=false
; item @45.index section=.data local=true zero=false
; runtime hargs_size=32 hargs_need=11
; @zp-link-meta-end

test_add_constant_to_pointer:
   SUBROUTINE

   LDA   #$1C
   STA   __local_test_add_constant_to_pointer__4
.clear@0:
   DEC   __local_test_add_constant_to_pointer__4
   LDA   __local_test_add_constant_to_pointer__4
   CLC
   ADC   #<__local_test_add_constant_to_pointer__long_arr
   STA   __local_test_add_constant_to_pointer__0
   LDA   #$00
   ADC   #>__local_test_add_constant_to_pointer__long_arr
   STA   __local_test_add_constant_to_pointer__1
   LDA   #$00
   LDY   #$0C
   STA   (__local_test_add_constant_to_pointer__0),Y
   LDA   __local_test_add_constant_to_pointer__4
   BNE   .clear@0
   LDA   #$00
   INY
   STY   __local_test_add_constant_to_pointer__3
   LDY   #$00
   CMP   #$00
   BNE   .thread@0
   TYA
   BNE   .thread@0
   LDA   __local_test_add_constant_to_pointer__3
   CMP   #$0D
   BEQ   .cmp_true@0
.thread@0:
   TYA
   JMP   .cmp_end@1
.cmp_true@0:
   LDA   #$01
.cmp_end@1:
   LDX   #$00
   RTS

test_add_negative_index:
   SUBROUTINE

   LDA   #$00
   LDY   #$02
   STY   __local_test_add_negative_index__unsigned_arr+4
   STA   __local_test_add_negative_index__unsigned_arr+5
   LDA   #$0E
   STA   __local_test_add_negative_index__4
.clear@1:
   DEC   __local_test_add_negative_index__4
   LDA   __local_test_add_negative_index__4
   CLC
   ADC   #<__local_test_add_negative_index__unsigned_arr
   STA   __local_test_add_negative_index__0
   LDA   #$00
   ADC   #>__local_test_add_negative_index__unsigned_arr
   STA   __local_test_add_negative_index__1
   LDA   #$00
   LDY   #$06
   STA   (__local_test_add_negative_index__0),Y
   LDA   __local_test_add_negative_index__4
   BNE   .clear@1
   LDY   #$00
   LDA   __local_test_add_negative_index__unsigned_arr+4
   STA   __local_test_add_negative_index__1
   LDA   __local_test_add_negative_index__unsigned_arr+5
   BNE   .thread@1
   LDA   __local_test_add_negative_index__1
   CMP   #$02
   BEQ   .cmp_true@3
.thread@1:
   TYA
   JMP   .cmp_end@4
.cmp_true@3:
   LDA   #$01
.cmp_end@4:
   LDX   #$00
   RTS

test_add_pointer_to_int:
   SUBROUTINE

   LDY   #$00
   LDA   #>__local_test_add_pointer_to_int__int_arr
   STA   __local_test_add_pointer_to_int__5
   LDA   #<__local_test_add_pointer_to_int__int_arr
   CLC
   ADC   #$04
   STA   __local_test_add_pointer_to_int__3
   BCC   .carry_inc@0
   INC   __local_test_add_pointer_to_int__5
.carry_inc@0:
   LDA   #>__local_test_add_pointer_to_int__int_arr
   STA   __local_test_add_pointer_to_int__2
   LDA   #<__local_test_add_pointer_to_int__int_arr
   CLC
   ADC   #$04
   STA   __local_test_add_pointer_to_int__0
   BCC   .carry_inc@1
   INC   __local_test_add_pointer_to_int__2
.carry_inc@1:
   LDA   __local_test_add_pointer_to_int__5
   CMP   __local_test_add_pointer_to_int__2
   BNE   .and_false@2
   LDA   __local_test_add_pointer_to_int__3
   CMP   __local_test_add_pointer_to_int__0
   BNE   .and_false@2
   TYA
   BNE   .and_false@2
   LDA   #$63
   CMP   #$63
   BNE   .and_false@2
   JMP   test_add_pointer_to_int@tail0
.and_false@2:
   TYA
   LDX   #$00
   RTS

test_add_pointer_to_int@tail0:
   SUBROUTINE

   LDA   #$01
   LDX   #$00
   RTS

test_add_different_index_types:
   SUBROUTINE

   LDY   #$28
   LDA   #$00
.clear@4:
   DEY
   STA   __local_test_add_different_index_types__double_arr,Y
   BNE   .clear@4
   STA   __local_test_add_different_index_types__double_arr+40
   LDY   #$00
   STY   __local_test_add_different_index_types__double_arr+41
   STY   __local_test_add_different_index_types__double_arr+42
   STY   __local_test_add_different_index_types__double_arr+43
   STY   __local_test_add_different_index_types__double_arr+44
   STA   __local_test_add_different_index_types__double_arr+45
   LDA   #$18
   STA   __local_test_add_different_index_types__double_arr+46
   LDA   #$40
   STA   __local_test_add_different_index_types__double_arr+47
   LDA   #$28
   STA   __local_test_add_different_index_types__4
.clear@5:
   DEC   __local_test_add_different_index_types__4
   LDA   __local_test_add_different_index_types__4
   CLC
   ADC   #<__local_test_add_different_index_types__double_arr
   STA   __local_test_add_different_index_types__0
   LDA   #$00
   ADC   #>__local_test_add_different_index_types__double_arr
   STA   __local_test_add_different_index_types__1
   LDA   #$00
   LDY   #$30
   STA   (__local_test_add_different_index_types__0),Y
   LDA   __local_test_add_different_index_types__4
   BNE   .clear@5
   LDA   #>__local_test_add_different_index_types__double_arr
   STA   __local_test_add_different_index_types__11
   JSR   test_add_different_index_types@run0
   STA   __local_test_add_different_index_types__9
   BCC   .carry_inc@2
   INC   __local_test_add_different_index_types__11
.carry_inc@2:
   LDA   #>__local_test_add_different_index_types__double_arr
   STA   __local_test_add_different_index_types__8
   JSR   test_add_different_index_types@run0
   STA   __local_test_add_different_index_types__6
   BCC   .carry_inc@3
   INC   __local_test_add_different_index_types__8
.carry_inc@3:
   LDA   #>__local_test_add_different_index_types__double_arr
   STA   __local_test_add_different_index_types__5
   JSR   test_add_different_index_types@run0
   STA   __local_test_add_different_index_types__3
   BCC   .carry_inc@4
   INC   __local_test_add_different_index_types__5
.carry_inc@4:
   LDA   #>__local_test_add_different_index_types__double_arr
   STA   __local_test_add_different_index_types__2
   JSR   test_add_different_index_types@run0
   STA   __local_test_add_different_index_types__0
   BCC   .carry_inc@5
   INC   __local_test_add_different_index_types__2
.carry_inc@5:
   LDA   __local_test_add_different_index_types__11
   CMP   __local_test_add_different_index_types__8
   BEQ   .lb_skip@0
   JMP   .and_false@6
.lb_skip@0:
   LDA   __local_test_add_different_index_types__9
   CMP   __local_test_add_different_index_types__6
   BNE   .and_false@6
   LDA   __local_test_add_different_index_types__11
   CMP   __local_test_add_different_index_types__5
   BNE   .and_false@6
   LDA   __local_test_add_different_index_types__9
   CMP   __local_test_add_different_index_types__3
   BNE   .and_false@6
   LDA   __local_test_add_different_index_types__11
   CMP   __local_test_add_different_index_types__2
   BNE   .and_false@6
   LDA   __local_test_add_different_index_types__9
   CMP   __local_test_add_different_index_types__0
   BNE   .and_false@6
   LDA   __local_test_add_different_index_types__double_arr+40
   STA   __local_test_add_different_index_types__7
   LDA   __local_test_add_different_index_types__double_arr+41
   STA   __local_test_add_different_index_types__6
   LDA   __local_test_add_different_index_types__double_arr+42
   STA   __local_test_add_different_index_types__5
   LDA   __local_test_add_different_index_types__double_arr+43
   STA   __local_test_add_different_index_types__4
   LDA   __local_test_add_different_index_types__double_arr+44
   STA   __local_test_add_different_index_types__3
   LDA   __local_test_add_different_index_types__double_arr+45
   STA   __local_test_add_different_index_types__2
   LDA   __local_test_add_different_index_types__double_arr+46
   STA   __local_test_add_different_index_types__1
   LDA   __local_test_add_different_index_types__double_arr+47
   CMP   #$40
   BNE   .and_false@6
   LDA   __local_test_add_different_index_types__1
   CMP   #$18
   BNE   .and_false@6
   LDA   __local_test_add_different_index_types__2
   BNE   .and_false@6
   LDA   __local_test_add_different_index_types__3
   BNE   .and_false@6
   LDA   __local_test_add_different_index_types__4
   BNE   .and_false@6
   LDA   __local_test_add_different_index_types__5
   BNE   .and_false@6
   LDA   __local_test_add_different_index_types__6
   BNE   .and_false@6
   LDA   __local_test_add_different_index_types__7
   BNE   .and_false@6
   JMP   test_add_pointer_to_int@tail0
.and_false@6:
   JMP   test_add_different_index_types@tail1

test_add_different_index_types@run0:
   SUBROUTINE

   LDA   #<__local_test_add_different_index_types__double_arr
   CLC
   ADC   #$28
   RTS

test_add_different_index_types@tail1:
   SUBROUTINE

   LDA   #$00
   LDX   #$00
   RTS

test_add_complex_expressions:
   SUBROUTINE

   JSR   return_one
   CLC
   ADC   #$FE
   STA   __local_test_add_complex_expressions__0
   TXA
   ADC   #$FF
   STA   __local_test_add_complex_expressions__1
   LDA   @13.flag
   ORA   @13.flag+1
   BEQ   .cond_else@12
   LDX   #>@18.arr
   LDA   #<@18.arr
   JSR   get_elem1_ptr
   JMP   .cond_end@13
.cond_else@12:
   LDX   #>@18.arr
   LDA   #<@18.arr
   JSR   get_elem2_ptr
.cond_end@13:
   ASL   __local_test_add_complex_expressions__0
   ROL   __local_test_add_complex_expressions__1
   CLC
   ADC   __local_test_add_complex_expressions__0
   STA   __local_test_add_complex_expressions__2
   TXA
   ADC   __local_test_add_complex_expressions__1
   STA   __local_test_add_complex_expressions__3
   CMP   #>(@18.arr+2)
   BNE   .and_false@14
   LDA   __local_test_add_complex_expressions__2
   CMP   #<(@18.arr+2)
   BNE   .and_false@14
   LDY   #$00
   LDA   (__local_test_add_complex_expressions__2),Y
   STA   __local_test_add_complex_expressions__1
   INY
   LDA   (__local_test_add_complex_expressions__2),Y
   BNE   .and_false@14
   LDA   __local_test_add_complex_expressions__1
   CMP   #$02
   BNE   .and_false@14
   JMP   test_add_pointer_to_int@tail0
.and_false@14:
   JMP   test_add_different_index_types@tail1

return_one:
   SUBROUTINE

   LDX   #$00
   LDA   #$01
   RTS

get_elem1_ptr:
   SUBROUTINE

   CLC
   ADC   #$02
   BCC   .carry_inc@6
   INX
.carry_inc@6:
   RTS

get_elem2_ptr:
   SUBROUTINE

   CLC
   ADC   #$04
   BCC   .carry_inc@7
   INX
.carry_inc@7:
   RTS

test_add_multi_dimensional:
   SUBROUTINE

   LDA   #$01
   STA   __local_test_add_multi_dimensional__nested_arr
   LDA   #$00
   STA   __local_test_add_multi_dimensional__nested_arr+1
   LDY   #$02
   STY   __local_test_add_multi_dimensional__nested_arr+2
   STA   __local_test_add_multi_dimensional__nested_arr+3
   INY
   STY   __local_test_add_multi_dimensional__nested_arr+4
   STA   __local_test_add_multi_dimensional__nested_arr+5
   INY
   STY   __local_test_add_multi_dimensional__nested_arr+6
   STA   __local_test_add_multi_dimensional__nested_arr+7
   INY
   STY   __local_test_add_multi_dimensional__nested_arr+8
   STA   __local_test_add_multi_dimensional__nested_arr+9
   INY
   STY   __local_test_add_multi_dimensional__nested_arr+10
   STA   __local_test_add_multi_dimensional__nested_arr+11
   INY
   STY   __local_test_add_multi_dimensional__nested_arr+12
   STA   __local_test_add_multi_dimensional__nested_arr+13
   INY
   STY   __local_test_add_multi_dimensional__nested_arr+14
   STA   __local_test_add_multi_dimensional__nested_arr+15
   INY
   STY   __local_test_add_multi_dimensional__nested_arr+16
   STA   __local_test_add_multi_dimensional__nested_arr+17
   LDA   @22.index
   ASL
   STA   __local_test_add_multi_dimensional__4
   LDA   @22.index+1
   ROL
   STA   __local_test_add_multi_dimensional__5
   LDA   __local_test_add_multi_dimensional__4
   ASL
   STA   __local_test_add_multi_dimensional__2
   LDA   __local_test_add_multi_dimensional__5
   ROL
   STA   __local_test_add_multi_dimensional__3
   LDA   __local_test_add_multi_dimensional__2
   CLC
   ADC   __local_test_add_multi_dimensional__4
   STA   __local_test_add_multi_dimensional__1
   LDA   __local_test_add_multi_dimensional__3
   ADC   __local_test_add_multi_dimensional__5
   STA   __local_test_add_multi_dimensional__0
   LDA   #<__local_test_add_multi_dimensional__nested_arr
   CLC
   ADC   __local_test_add_multi_dimensional__1
   STA   __local_test_add_multi_dimensional__2
   LDA   #>__local_test_add_multi_dimensional__nested_arr
   ADC   __local_test_add_multi_dimensional__0
   STA   __local_test_add_multi_dimensional__3
   LDY   #$00
   LDA   (__local_test_add_multi_dimensional__2),Y
   STA   __local_test_add_multi_dimensional__1
   INY
   LDA   (__local_test_add_multi_dimensional__2),Y
   BNE   .thread@2
   LDA   __local_test_add_multi_dimensional__1
   CMP   #$07
   BEQ   .cmp_true@6
.thread@2:
   LDA   #$00
   BEQ   .cmp_end@7
.cmp_true@6:
   LDA   #$01
.cmp_end@7:
   LDX   #$00
   RTS

test_add_to_subarray_pointer:
   SUBROUTINE

   LDA   #$01
   STA   __local_test_add_to_subarray_pointer__nested_arr
   LDA   #$00
   STA   __local_test_add_to_subarray_pointer__nested_arr+1
   LDY   #$02
   STY   __local_test_add_to_subarray_pointer__nested_arr+2
   STA   __local_test_add_to_subarray_pointer__nested_arr+3
   INY
   STY   __local_test_add_to_subarray_pointer__nested_arr+4
   STA   __local_test_add_to_subarray_pointer__nested_arr+5
   INY
   STY   __local_test_add_to_subarray_pointer__nested_arr+6
   STA   __local_test_add_to_subarray_pointer__nested_arr+7
   INY
   STY   __local_test_add_to_subarray_pointer__nested_arr+8
   STA   __local_test_add_to_subarray_pointer__nested_arr+9
   INY
   STY   __local_test_add_to_subarray_pointer__nested_arr+10
   STA   __local_test_add_to_subarray_pointer__nested_arr+11
   INY
   STY   __local_test_add_to_subarray_pointer__nested_arr+12
   STA   __local_test_add_to_subarray_pointer__nested_arr+13
   INY
   STY   __local_test_add_to_subarray_pointer__nested_arr+14
   STA   __local_test_add_to_subarray_pointer__nested_arr+15
   INY
   STY   __local_test_add_to_subarray_pointer__nested_arr+16
   STA   __local_test_add_to_subarray_pointer__nested_arr+17
   LDA   @25.index
   ASL
   STA   __local_test_add_to_subarray_pointer__0
   LDA   @25.index+1
   ROL
   STA   __local_test_add_to_subarray_pointer__1
   LDA   __local_test_add_to_subarray_pointer__0
   CLC
   ADC   #<__local_test_add_to_subarray_pointer__nested_arr
   STA   __local_test_add_to_subarray_pointer__2
   LDA   __local_test_add_to_subarray_pointer__1
   ADC   #>__local_test_add_to_subarray_pointer__nested_arr
   STA   __local_test_add_to_subarray_pointer__3
   LDY   #$06
   LDA   (__local_test_add_to_subarray_pointer__2),Y
   STA   __local_test_add_to_subarray_pointer__1
   INY
   LDA   (__local_test_add_to_subarray_pointer__2),Y
   BNE   .thread@3
   LDA   __local_test_add_to_subarray_pointer__1
   CMP   #$06
   BEQ   .cmp_true@9
.thread@3:
   LDA   #$00
   BEQ   .cmp_end@10
.cmp_true@9:
   LDA   #$01
.cmp_end@10:
   LDX   #$00
   RTS

test_subtract_from_pointer:
   SUBROUTINE

   LDA   #$0A
   STA   __local_test_subtract_from_pointer__long_arr
   LDA   #$00
   STA   __local_test_subtract_from_pointer__long_arr+1
   LDY   #$00
   STY   __local_test_subtract_from_pointer__long_arr+2
   STA   __local_test_subtract_from_pointer__long_arr+3
   LDY   #$09
   STY   __local_test_subtract_from_pointer__long_arr+4
   LDY   #$00
   STY   __local_test_subtract_from_pointer__long_arr+5
   STY   __local_test_subtract_from_pointer__long_arr+6
   STA   __local_test_subtract_from_pointer__long_arr+7
   LDY   #$08
   STY   __local_test_subtract_from_pointer__long_arr+8
   LDY   #$00
   STY   __local_test_subtract_from_pointer__long_arr+9
   STY   __local_test_subtract_from_pointer__long_arr+10
   STA   __local_test_subtract_from_pointer__long_arr+11
   LDY   #$07
   STY   __local_test_subtract_from_pointer__long_arr+12
   LDY   #$00
   STY   __local_test_subtract_from_pointer__long_arr+13
   STY   __local_test_subtract_from_pointer__long_arr+14
   STA   __local_test_subtract_from_pointer__long_arr+15
   LDY   #$06
   STY   __local_test_subtract_from_pointer__long_arr+16
   LDY   #$00
   STY   __local_test_subtract_from_pointer__long_arr+17
   STY   __local_test_subtract_from_pointer__long_arr+18
   STA   __local_test_subtract_from_pointer__long_arr+19
   LDA   #>__local_test_subtract_from_pointer__long_arr
   STA   __local_test_subtract_from_pointer__7
   LDA   #<__local_test_subtract_from_pointer__long_arr
   CLC
   ADC   #$14
   STA   __local_test_subtract_from_pointer__2
   BCC   .carry_inc@8
   INC   __local_test_subtract_from_pointer__7
.carry_inc@8:
   LDA   @31.index
   ASL
   STA   __local_test_subtract_from_pointer__0
   LDA   @31.index+1
   ROL
   ASL   __local_test_subtract_from_pointer__0
   ROL
   STA   __local_test_subtract_from_pointer__1
   LDA   __local_test_subtract_from_pointer__2
   SEC
   SBC   __local_test_subtract_from_pointer__0
   STA   __local_test_subtract_from_pointer__4
   LDA   __local_test_subtract_from_pointer__7
   SBC   __local_test_subtract_from_pointer__1
   STA   __local_test_subtract_from_pointer__5
   LDY   #$00
   LDA   (__local_test_subtract_from_pointer__4),Y
   STA   __local_test_subtract_from_pointer__3
   INY
   LDA   (__local_test_subtract_from_pointer__4),Y
   STA   __local_test_subtract_from_pointer__2
   INY
   LDA   (__local_test_subtract_from_pointer__4),Y
   STA   __local_test_subtract_from_pointer__1
   INY
   LDA   (__local_test_subtract_from_pointer__4),Y
   BNE   .thread@4
   LDA   __local_test_subtract_from_pointer__1
   BNE   .thread@4
   LDA   __local_test_subtract_from_pointer__2
   BNE   .thread@4
   LDA   __local_test_subtract_from_pointer__3
   CMP   #$08
   BEQ   .cmp_true@12
.thread@4:
   LDA   #$00
   BEQ   .cmp_end@13
.cmp_true@12:
   LDA   #$01
.cmp_end@13:
   LDX   #$00
   RTS

test_subtract_negative_index:
   SUBROUTINE

   LDA   #$64
   STA   __local_test_subtract_negative_index__arr
   LDA   #$00
   STA   __local_test_subtract_negative_index__arr+1
   LDY   #$65
   STY   __local_test_subtract_negative_index__arr+2
   STA   __local_test_subtract_negative_index__arr+3
   INY
   STY   __local_test_subtract_negative_index__arr+4
   STA   __local_test_subtract_negative_index__arr+5
   INY
   STY   __local_test_subtract_negative_index__arr+6
   STA   __local_test_subtract_negative_index__arr+7
   INY
   STY   __local_test_subtract_negative_index__arr+8
   STA   __local_test_subtract_negative_index__arr+9
   LDA   #<__local_test_subtract_negative_index__arr
   SEC
   SBC   #$FA
   STA   __local_test_subtract_negative_index__2
   LDA   #>__local_test_subtract_negative_index__arr
   SBC   #$FF
   STA   __local_test_subtract_negative_index__3
   LDY   #$00
   LDA   (__local_test_subtract_negative_index__2),Y
   STA   __local_test_subtract_negative_index__1
   INY
   LDA   (__local_test_subtract_negative_index__2),Y
   BNE   .thread@5
   LDA   __local_test_subtract_negative_index__1
   CMP   #$67
   BEQ   .cmp_true@15
.thread@5:
   LDA   #$00
   BEQ   .cmp_end@16
.cmp_true@15:
   LDA   #$01
.cmp_end@16:
   LDX   #$00
   RTS

test_subtract_different_index_types:
   SUBROUTINE

   LDY   #$30
   LDA   #$00
.clear@16:
   DEY
   STA   __local_test_subtract_different_index_types__double_arr,Y
   BNE   .clear@16
   STA   __local_test_subtract_different_index_types__double_arr+48
   LDY   #$00
   STY   __local_test_subtract_different_index_types__double_arr+49
   STY   __local_test_subtract_different_index_types__double_arr+50
   STY   __local_test_subtract_different_index_types__double_arr+51
   STY   __local_test_subtract_different_index_types__double_arr+52
   STA   __local_test_subtract_different_index_types__double_arr+53
   LDA   #$18
   STA   __local_test_subtract_different_index_types__double_arr+54
   LDA   #$40
   STA   __local_test_subtract_different_index_types__double_arr+55
   LDA   #$20
   STA   __local_test_subtract_different_index_types__4
.clear@17:
   DEC   __local_test_subtract_different_index_types__4
   LDA   __local_test_subtract_different_index_types__4
   CLC
   ADC   #<__local_test_subtract_different_index_types__double_arr
   STA   __local_test_subtract_different_index_types__0
   LDA   #$00
   ADC   #>__local_test_subtract_different_index_types__double_arr
   STA   __local_test_subtract_different_index_types__1
   LDA   #$00
   LDY   #$38
   STA   (__local_test_subtract_different_index_types__0),Y
   LDA   __local_test_subtract_different_index_types__4
   BNE   .clear@17
   LDA   #>__local_test_subtract_different_index_types__double_arr
   STA   __local_test_subtract_different_index_types__9
   LDA   #<__local_test_subtract_different_index_types__double_arr
   CLC
   ADC   #$58
   BCC   .carry_inc@9
   INC   __local_test_subtract_different_index_types__9
.carry_inc@9:
   SEC
   SBC   #$28
   STA   __local_test_subtract_different_index_types__8
   BCS   .carry_inc@10
   DEC   __local_test_subtract_different_index_types__9
.carry_inc@10:
   LDA   __local_test_subtract_different_index_types__9
   CMP   __local_test_subtract_different_index_types__9
   BEQ   .lb_skip@2
   JMP   .and_false@18
.lb_skip@2:
   LDA   __local_test_subtract_different_index_types__8
   CMP   __local_test_subtract_different_index_types__8
   BEQ   .lb_skip@1
   JMP   .and_false@18
.lb_skip@1:
   LDA   __local_test_subtract_different_index_types__9
   CMP   __local_test_subtract_different_index_types__9
   BNE   .and_false@18
   LDA   __local_test_subtract_different_index_types__8
   CMP   __local_test_subtract_different_index_types__8
   BNE   .and_false@18
   LDA   __local_test_subtract_different_index_types__9
   CMP   __local_test_subtract_different_index_types__9
   BNE   .and_false@18
   LDA   __local_test_subtract_different_index_types__8
   CMP   __local_test_subtract_different_index_types__8
   BNE   .and_false@18
   LDY   #$00
   LDA   (__local_test_subtract_different_index_types__8),Y
   STA   __local_test_subtract_different_index_types__7
   INY
   LDA   (__local_test_subtract_different_index_types__8),Y
   STA   __local_test_subtract_different_index_types__6
   INY
   LDA   (__local_test_subtract_different_index_types__8),Y
   STA   __local_test_subtract_different_index_types__5
   INY
   LDA   (__local_test_subtract_different_index_types__8),Y
   STA   __local_test_subtract_different_index_types__4
   INY
   LDA   (__local_test_subtract_different_index_types__8),Y
   STA   __local_test_subtract_different_index_types__3
   INY
   LDA   (__local_test_subtract_different_index_types__8),Y
   STA   __local_test_subtract_different_index_types__2
   INY
   LDA   (__local_test_subtract_different_index_types__8),Y
   STA   __local_test_subtract_different_index_types__1
   INY
   LDA   (__local_test_subtract_different_index_types__8),Y
   CMP   #$40
   BNE   .and_false@18
   LDA   __local_test_subtract_different_index_types__1
   CMP   #$18
   BNE   .and_false@18
   LDA   __local_test_subtract_different_index_types__2
   BNE   .and_false@18
   LDA   __local_test_subtract_different_index_types__3
   BNE   .and_false@18
   LDA   __local_test_subtract_different_index_types__4
   BNE   .and_false@18
   LDA   __local_test_subtract_different_index_types__5
   BNE   .and_false@18
   LDA   __local_test_subtract_different_index_types__6
   BNE   .and_false@18
   LDA   __local_test_subtract_different_index_types__7
   BNE   .and_false@18
   JMP   test_add_pointer_to_int@tail0
.and_false@18:
   JMP   test_add_different_index_types@tail1

test_subtract_complex_expressions:
   SUBROUTINE

   LDA   @41.flag
   ORA   @41.flag+1
   BEQ   .cond_else@24
   LDX   #>@43.arr
   LDA   #<@43.arr
   JSR   get_elem1_ptr
   STA   __local_test_subtract_complex_expressions__2
   STX   __local_test_subtract_complex_expressions__4
   JMP   .cond_end@25
.cond_else@24:
   LDX   #>@43.arr
   LDA   #<@43.arr
   JSR   get_elem2_ptr
   STA   __local_test_subtract_complex_expressions__2
   STX   __local_test_subtract_complex_expressions__4
.cond_end@25:
   LDA   @42.four
   STA   HARGS
   LDA   @42.four+1
   STA   HARGS+1
   LDA   #$FE
   STA   HARGS+2
   LDA   #$FF
   STA   HARGS+3
   JSR   sdivmod16
   LDA   HARGS+4
   STA   __local_test_subtract_complex_expressions__0
   LDA   HARGS+5
   ASL   __local_test_subtract_complex_expressions__0
   ROL
   STA   __local_test_subtract_complex_expressions__1
   LDA   __local_test_subtract_complex_expressions__2
   SEC
   SBC   __local_test_subtract_complex_expressions__0
   STA   __local_test_subtract_complex_expressions__2
   LDA   __local_test_subtract_complex_expressions__4
   SBC   __local_test_subtract_complex_expressions__1
   STA   __local_test_subtract_complex_expressions__3
   LDY   #$00
   LDA   (__local_test_subtract_complex_expressions__2),Y
   STA   __local_test_subtract_complex_expressions__1
   INY
   LDA   (__local_test_subtract_complex_expressions__2),Y
   BNE   .thread@6
   LDA   __local_test_subtract_complex_expressions__1
   CMP   #$04
   BEQ   .cmp_true@18
.thread@6:
   LDA   #$00
   BEQ   .cmp_end@19
.cmp_true@18:
   LDA   #$01
.cmp_end@19:
   LDX   #$00
   RTS

test_subtract_multi_dimensional:
   SUBROUTINE

   LDA   #$01
   STA   __local_test_subtract_multi_dimensional__nested_arr
   LDA   #$00
   STA   __local_test_subtract_multi_dimensional__nested_arr+1
   LDY   #$02
   STY   __local_test_subtract_multi_dimensional__nested_arr+2
   STA   __local_test_subtract_multi_dimensional__nested_arr+3
   INY
   STY   __local_test_subtract_multi_dimensional__nested_arr+4
   STA   __local_test_subtract_multi_dimensional__nested_arr+5
   INY
   STY   __local_test_subtract_multi_dimensional__nested_arr+6
   STA   __local_test_subtract_multi_dimensional__nested_arr+7
   INY
   STY   __local_test_subtract_multi_dimensional__nested_arr+8
   STA   __local_test_subtract_multi_dimensional__nested_arr+9
   INY
   STY   __local_test_subtract_multi_dimensional__nested_arr+10
   STA   __local_test_subtract_multi_dimensional__nested_arr+11
   INY
   STY   __local_test_subtract_multi_dimensional__nested_arr+12
   STA   __local_test_subtract_multi_dimensional__nested_arr+13
   INY
   STY   __local_test_subtract_multi_dimensional__nested_arr+14
   STA   __local_test_subtract_multi_dimensional__nested_arr+15
   INY
   STY   __local_test_subtract_multi_dimensional__nested_arr+16
   STA   __local_test_subtract_multi_dimensional__nested_arr+17
   LDA   #>__local_test_subtract_multi_dimensional__nested_arr
   STA   __local_test_subtract_multi_dimensional__8
   LDA   #<__local_test_subtract_multi_dimensional__nested_arr
   CLC
   ADC   #$0C
   STA   __local_test_subtract_multi_dimensional__6
   BCC   .carry_inc@11
   INC   __local_test_subtract_multi_dimensional__8
.carry_inc@11:
   LDA   @45.index
   ASL
   STA   __local_test_subtract_multi_dimensional__4
   LDA   @45.index+1
   ROL
   STA   __local_test_subtract_multi_dimensional__5
   LDA   __local_test_subtract_multi_dimensional__4
   ASL
   STA   __local_test_subtract_multi_dimensional__2
   LDA   __local_test_subtract_multi_dimensional__5
   ROL
   STA   __local_test_subtract_multi_dimensional__3
   LDA   __local_test_subtract_multi_dimensional__2
   CLC
   ADC   __local_test_subtract_multi_dimensional__4
   STA   __local_test_subtract_multi_dimensional__1
   LDA   __local_test_subtract_multi_dimensional__3
   ADC   __local_test_subtract_multi_dimensional__5
   STA   __local_test_subtract_multi_dimensional__0
   LDA   __local_test_subtract_multi_dimensional__6
   SEC
   SBC   __local_test_subtract_multi_dimensional__1
   STA   __local_test_subtract_multi_dimensional__2
   LDA   __local_test_subtract_multi_dimensional__8
   SBC   __local_test_subtract_multi_dimensional__0
   STA   __local_test_subtract_multi_dimensional__3
   LDY   #$00
   LDA   (__local_test_subtract_multi_dimensional__2),Y
   STA   __local_test_subtract_multi_dimensional__1
   INY
   LDA   (__local_test_subtract_multi_dimensional__2),Y
   BNE   .thread@7
   LDA   __local_test_subtract_multi_dimensional__1
   CMP   #$04
   BEQ   .cmp_true@21
.thread@7:
   LDA   #$00
   BEQ   .cmp_end@22
.cmp_true@21:
   LDA   #$01
.cmp_end@22:
   LDX   #$00
   RTS

main:
   SUBROUTINE

   JSR   test_add_constant_to_pointer
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@26
   LDX   #$00
   LDA   #$01
   RTS
.if_end@26:
   JSR   test_add_negative_index
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@27
   LDX   #$00
   LDA   #$02
   RTS
.if_end@27:
   JSR   test_add_pointer_to_int
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@28
   LDX   #$00
   LDA   #$03
   RTS
.if_end@28:
   JSR   test_add_different_index_types
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@29
   LDX   #$00
   LDA   #$04
   RTS
.if_end@29:
   JSR   test_add_complex_expressions
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@30
   LDX   #$00
   LDA   #$05
   RTS
.if_end@30:
   JSR   test_add_multi_dimensional
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@31
   LDX   #$00
   LDA   #$06
   RTS
.if_end@31:
   JSR   test_add_to_subarray_pointer
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@32
   LDX   #$00
   LDA   #$07
   RTS
.if_end@32:
   JSR   test_subtract_from_pointer
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@33
   LDX   #$00
   LDA   #$08
   RTS
.if_end@33:
   JSR   test_subtract_negative_index
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@34
   LDX   #$00
   LDA   #$09
   RTS
.if_end@34:
   JSR   test_subtract_different_index_types
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@35
   LDX   #$00
   LDA   #$0A
   RTS
.if_end@35:
   JSR   test_subtract_complex_expressions
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@36
   LDX   #$00
   LDA   #$0B
   RTS
.if_end@36:
   LDX   #$00
   TXA
   RTS

@13.flag:
   DS.B  2

@18.arr:
   DC.W  $0001
   DC.W  $0002
   DC.W  $0003
   DC.W  $0004

@22.index:
   DC.W  $0002

@25.index:
   DC.W  $0002

@31.index:
   DC.W  $0003

@41.flag:
   DC.W  $0001

@42.four:
   DC.W  $0004

@43.arr:
   DC.W  $0001
   DC.W  $0002
   DC.W  $0003
   DC.W  $0004

@45.index:
   DC.W  $0001
