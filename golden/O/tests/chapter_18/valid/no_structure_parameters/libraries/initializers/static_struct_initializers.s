SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_strcmp__s2_0	EQU	$80
__zpabi_strcmp__s2_1	EQU	$81
__local_test_partial_inner_init__0	EQU	$82
__local_test_partially_initialized__0	EQU	$82

; @zp-link-meta-begin
; def test_implicit_conversion params= locals=__local_test_implicit_conversion__0,__local_test_implicit_conversion__1,__local_test_implicit_conversion__2,__local_test_implicit_conversion__3,__local_test_implicit_conversion__4,__local_test_implicit_conversion__5,__local_test_implicit_conversion__6,__local_test_implicit_conversion__7 indirect=false in_cycle=false
; def test_partial_inner_init params= locals=__local_test_partial_inner_init__0,__local_test_partial_inner_init__1,__local_test_partial_inner_init__2,__local_test_partial_inner_init__3,__local_test_partial_inner_init__4,__local_test_partial_inner_init__5,__local_test_partial_inner_init__6,__local_test_partial_inner_init__7 indirect=false in_cycle=false
; def test_partially_initialized params= locals=__local_test_partially_initialized__0,__local_test_partially_initialized__1,__local_test_partially_initialized__2,__local_test_partially_initialized__3,__local_test_partially_initialized__4,__local_test_partially_initialized__5,__local_test_partially_initialized__6,__local_test_partially_initialized__7 indirect=false in_cycle=false
; def test_uninitialized params= locals=__local_test_uninitialized__0,__local_test_uninitialized__1,__local_test_uninitialized__2,__local_test_uninitialized__3,__local_test_uninitialized__4,__local_test_uninitialized__5,__local_test_uninitialized__6,__local_test_uninitialized__7 indirect=false in_cycle=false
; ext strcmp params=__zpabi_strcmp__s1_0,__zpabi_strcmp__s1_1,__zpabi_strcmp__s2_0,__zpabi_strcmp__s2_1 param_regs=A,X,,
; call test_partial_inner_init -> strcmp
; call test_partially_initialized -> strcmp
; item test_uninitialized section=.text local=false zero=false
; item test_uninitialized@tail0 section=.text local=true zero=false
; item test_partially_initialized section=.text local=false zero=false
; item test_partial_inner_init section=.text local=false zero=false
; item test_implicit_conversion section=.text local=false zero=false
; item str@0 section=.rodata local=true zero=false
; item str@1 section=.rodata local=true zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

test_uninitialized:
   SUBROUTINE

   LDA   uninitialized
   ORA   uninitialized+1
   ORA   uninitialized+2
   ORA   uninitialized+3
   ORA   uninitialized+4
   ORA   uninitialized+5
   ORA   uninitialized+6
   ORA   uninitialized+7
   BNE   .test_uninitialized@jthread@4
   LDA   uninitialized+8
   ORA   uninitialized+9
   BNE   .test_uninitialized@jthread@4
   LDA   uninitialized+10
   BNE   .test_uninitialized@jthread@4
   LDA   uninitialized+11
   BNE   .test_uninitialized@jthread@4
   LDA   uninitialized+12
   BNE   .test_uninitialized@jthread@4
   LDA   uninitialized+13
   ORA   uninitialized+14
   BEQ   .if_end@10
.test_uninitialized@jthread@4:
   LDX   #$00
   TXA
   RTS
.if_end@10:
   JMP   test_uninitialized@tail0

test_uninitialized@tail0:
   SUBROUTINE

   LDX   #$00
   LDA   #$01
   RTS

test_partially_initialized:
   SUBROUTINE

   LDA   partial+7
   CMP   #$3F
   BNE   .test_partially_initialized@jthread@3
   LDA   partial+6
   CMP   #$F0
   BNE   .test_partially_initialized@jthread@3
   LDA   partial+5
   BNE   .test_partially_initialized@jthread@3
   LDA   partial+4
   BNE   .test_partially_initialized@jthread@3
   LDA   partial+3
   BNE   .test_partially_initialized@jthread@3
   LDA   partial+2
   BNE   .test_partially_initialized@jthread@3
   LDA   partial+1
   BNE   .test_partially_initialized@jthread@3
   LDA   partial
   BNE   .test_partially_initialized@jthread@3
   LDX   partial+9
   LDA   #<str@0
   STA   __zpabi_strcmp__s2_0
   LDA   #>str@0
   STA   __zpabi_strcmp__s2_1
   LDA   partial+8
   JSR   strcmp
   STX   __local_test_partially_initialized__0
   ORA   __local_test_partially_initialized__0
   BNE   .test_partially_initialized@jthread@3
   LDA   partial+10
   BNE   .test_partially_initialized@jthread@3
   LDA   partial+11
   BNE   .test_partially_initialized@jthread@3
   LDA   partial+12
   BNE   .test_partially_initialized@jthread@3
   LDA   partial+13
   ORA   partial+14
   BEQ   .if_end@20
.test_partially_initialized@jthread@3:
   LDX   #$00
   TXA
   RTS
.if_end@20:
   JMP   test_uninitialized@tail0

test_partial_inner_init:
   SUBROUTINE

   LDA   partial_with_array+7
   CMP   #$40
   BNE   .test_partial_inner_init@jthread@3
   LDA   partial_with_array+6
   CMP   #$08
   BNE   .test_partial_inner_init@jthread@3
   LDA   partial_with_array+5
   BNE   .test_partial_inner_init@jthread@3
   LDA   partial_with_array+4
   BNE   .test_partial_inner_init@jthread@3
   LDA   partial_with_array+3
   BNE   .test_partial_inner_init@jthread@3
   LDA   partial_with_array+2
   BNE   .test_partial_inner_init@jthread@3
   LDA   partial_with_array+1
   BNE   .test_partial_inner_init@jthread@3
   LDA   partial_with_array
   BNE   .test_partial_inner_init@jthread@3
   LDX   partial_with_array+9
   LDA   #<str@1
   STA   __zpabi_strcmp__s2_0
   LDA   #>str@1
   STA   __zpabi_strcmp__s2_1
   LDA   partial_with_array+8
   JSR   strcmp
   STX   __local_test_partial_inner_init__0
   ORA   __local_test_partial_inner_init__0
   BNE   .test_partial_inner_init@jthread@3
   LDA   partial_with_array+10
   CMP   #$01
   BNE   .test_partial_inner_init@jthread@3
   LDA   partial_with_array+14
   BNE   .test_partial_inner_init@jthread@3
   LDA   partial_with_array+13
   CMP   #$02
   BNE   .test_partial_inner_init@jthread@3
   LDA   partial_with_array+11
   BNE   .test_partial_inner_init@jthread@3
   LDA   partial_with_array+12
   BEQ   .if_end@30
.test_partial_inner_init@jthread@3:
   LDX   #$00
   TXA
   RTS
.if_end@30:
   JMP   test_uninitialized@tail0

test_implicit_conversion:
   SUBROUTINE

   LDA   converted+7
   CMP   #$43
   BNE   .test_implicit_conversion@jthread@4
   LDA   converted+6
   CMP   #$B0
   BNE   .test_implicit_conversion@jthread@4
   LDA   converted+5
   BNE   .test_implicit_conversion@jthread@4
   LDA   converted+4
   BNE   .test_implicit_conversion@jthread@4
   LDA   converted+3
   BNE   .test_implicit_conversion@jthread@4
   LDA   converted+2
   BNE   .test_implicit_conversion@jthread@4
   LDA   converted+1
   BNE   .test_implicit_conversion@jthread@4
   LDA   converted
   BNE   .test_implicit_conversion@jthread@4
   LDA   converted+8
   ORA   converted+9
   BNE   .test_implicit_conversion@jthread@4
   LDA   converted+10
   CMP   #$61
   BNE   .test_implicit_conversion@jthread@4
   LDA   converted+11
   CMP   #$62
   BNE   .test_implicit_conversion@jthread@4
   LDA   converted+12
   CMP   #$63
   BNE   .test_implicit_conversion@jthread@4
   LDA   converted+14
   BNE   .test_implicit_conversion@jthread@4
   LDA   converted+13
   CMP   #$05
   BEQ   .if_end@41
.test_implicit_conversion@jthread@4:
   LDX   #$00
   TXA
   RTS
.if_end@41:
   JMP   test_uninitialized@tail0

str@0:
   DC.B  $48, $65, $6C, $6C, $6F, $00

str@1:
   DC.B  $21, $00
