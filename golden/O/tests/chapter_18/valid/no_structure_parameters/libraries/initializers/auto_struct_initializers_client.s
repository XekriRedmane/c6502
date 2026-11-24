SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_calloc__nmemb_0	EQU	$80
__zpabi_malloc__size_0	EQU	$80
__zpabi_strcmp__s2_0	EQU	$80
__zpabi_validate_partial_initialization__expected_msg_0	EQU	$80
__zpabi_validate_two_structs__ptr2_0	EQU	$80
__zpabi_calloc__nmemb_1	EQU	$81
__zpabi_malloc__size_1	EQU	$81
__zpabi_strcmp__s2_1	EQU	$81
__zpabi_validate_partial_initialization__expected_msg_1	EQU	$81
__zpabi_validate_two_structs__ptr2_1	EQU	$81
__zpabi_calloc__nmemb_2	EQU	$82
__zpabi_malloc__size_2	EQU	$82
__zpabi_calloc__nmemb_3	EQU	$83
__zpabi_malloc__size_3	EQU	$83
__zpabi_calloc__size_0	EQU	$84
__zpabi_calloc__size_1	EQU	$85
__zpabi_calloc__size_2	EQU	$86
__zpabi_calloc__size_3	EQU	$87
__local_main__0	EQU	$88
__local_test_partial_initialization__0	EQU	$8A
__local_test_partial_initialization__1	EQU	$8B
__local_test_full_initialization__2	EQU	$8C
__local_test_partial_initialization__4	EQU	$8E
__local_test_single_exp_initializer__s1	EQU	$8E
__local_test_partial_initialization__partial	EQU	$8F
__local_test_full_initialization__full	EQU	$92
__local_test_implicit_type_conversions__converted	EQU	$92
__local_test_single_exp_initializer__d	EQU	$9F
__local_test_single_exp_initializer__s2	EQU	$A7

; @zp-link-meta-begin
; def get_double params= locals= indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1 indirect=false in_cycle=false
; def test_full_initialization params= locals=__local_test_full_initialization__0,__local_test_full_initialization__1,__local_test_full_initialization__2,__local_test_full_initialization__3,__local_test_full_initialization__4,__local_test_full_initialization__5,__local_test_full_initialization__6,__local_test_full_initialization__7,__local_test_full_initialization__full,__local_test_full_initialization__9,__local_test_full_initialization__10,__local_test_full_initialization__11,__local_test_full_initialization__12,__local_test_full_initialization__13,__local_test_full_initialization__14,__local_test_full_initialization__15,__local_test_full_initialization__16,__local_test_full_initialization__17,__local_test_full_initialization__18,__local_test_full_initialization__19,__local_test_full_initialization__20,__local_test_full_initialization__21,__local_test_full_initialization__22,__local_test_full_initialization__23,__local_test_full_initialization__24 indirect=false in_cycle=false
; def test_implicit_type_conversions params= locals=__local_test_implicit_type_conversions__0,__local_test_implicit_type_conversions__1,__local_test_implicit_type_conversions__2,__local_test_implicit_type_conversions__3,__local_test_implicit_type_conversions__4,__local_test_implicit_type_conversions__5,__local_test_implicit_type_conversions__6,__local_test_implicit_type_conversions__7,__local_test_implicit_type_conversions__converted,__local_test_implicit_type_conversions__9,__local_test_implicit_type_conversions__10,__local_test_implicit_type_conversions__11,__local_test_implicit_type_conversions__12,__local_test_implicit_type_conversions__13,__local_test_implicit_type_conversions__14,__local_test_implicit_type_conversions__15,__local_test_implicit_type_conversions__16,__local_test_implicit_type_conversions__17,__local_test_implicit_type_conversions__18,__local_test_implicit_type_conversions__19,__local_test_implicit_type_conversions__20,__local_test_implicit_type_conversions__21,__local_test_implicit_type_conversions__22,__local_test_implicit_type_conversions__23,__local_test_implicit_type_conversions__24 indirect=false in_cycle=false
; def test_partial_initialization params= locals=__local_test_partial_initialization__0,__local_test_partial_initialization__1,__local_test_partial_initialization__2,__local_test_partial_initialization__3,__local_test_partial_initialization__4,__local_test_partial_initialization__partial,__local_test_partial_initialization__6,__local_test_partial_initialization__7,__local_test_partial_initialization__8,__local_test_partial_initialization__9,__local_test_partial_initialization__10,__local_test_partial_initialization__11,__local_test_partial_initialization__12,__local_test_partial_initialization__13,__local_test_partial_initialization__14,__local_test_partial_initialization__15,__local_test_partial_initialization__16,__local_test_partial_initialization__17,__local_test_partial_initialization__18,__local_test_partial_initialization__19,__local_test_partial_initialization__20,__local_test_partial_initialization__21 indirect=false in_cycle=false
; def test_single_exp_initializer params= locals=__local_test_single_exp_initializer__0,__local_test_single_exp_initializer__1,__local_test_single_exp_initializer__2,__local_test_single_exp_initializer__3,__local_test_single_exp_initializer__s1,__local_test_single_exp_initializer__5,__local_test_single_exp_initializer__6,__local_test_single_exp_initializer__7,__local_test_single_exp_initializer__8,__local_test_single_exp_initializer__9,__local_test_single_exp_initializer__10,__local_test_single_exp_initializer__11,__local_test_single_exp_initializer__12,__local_test_single_exp_initializer__13,__local_test_single_exp_initializer__14,__local_test_single_exp_initializer__15,__local_test_single_exp_initializer__16,__local_test_single_exp_initializer__17,__local_test_single_exp_initializer__18,__local_test_single_exp_initializer__19,__local_test_single_exp_initializer__20,__local_test_single_exp_initializer__d,__local_test_single_exp_initializer__22,__local_test_single_exp_initializer__23,__local_test_single_exp_initializer__24,__local_test_single_exp_initializer__25,__local_test_single_exp_initializer__26,__local_test_single_exp_initializer__27,__local_test_single_exp_initializer__28,__local_test_single_exp_initializer__s2,__local_test_single_exp_initializer__30,__local_test_single_exp_initializer__31,__local_test_single_exp_initializer__32,__local_test_single_exp_initializer__33,__local_test_single_exp_initializer__34,__local_test_single_exp_initializer__35,__local_test_single_exp_initializer__36,__local_test_single_exp_initializer__37,__local_test_single_exp_initializer__38,__local_test_single_exp_initializer__39,__local_test_single_exp_initializer__40,__local_test_single_exp_initializer__41,__local_test_single_exp_initializer__42,__local_test_single_exp_initializer__43,__local_test_single_exp_initializer__44,__local_test_single_exp_initializer__45 indirect=false in_cycle=false
; ext calloc params=__zpabi_calloc__nmemb_0,__zpabi_calloc__nmemb_1,__zpabi_calloc__nmemb_2,__zpabi_calloc__nmemb_3,__zpabi_calloc__size_0,__zpabi_calloc__size_1,__zpabi_calloc__size_2,__zpabi_calloc__size_3
; ext malloc params=__zpabi_malloc__size_0,__zpabi_malloc__size_1,__zpabi_malloc__size_2,__zpabi_malloc__size_3
; ext strcmp params=__zpabi_strcmp__s1_0,__zpabi_strcmp__s1_1,__zpabi_strcmp__s2_0,__zpabi_strcmp__s2_1 param_regs=A,X,,
; ext validate_converted params=__zpabi_validate_converted__ptr_0,__zpabi_validate_converted__ptr_1 param_regs=A,X
; ext validate_full_initialization params=__zpabi_validate_full_initialization__ptr_0,__zpabi_validate_full_initialization__ptr_1 param_regs=A,X
; ext validate_partial_initialization params=__zpabi_validate_partial_initialization__ptr_0,__zpabi_validate_partial_initialization__ptr_1,__zpabi_validate_partial_initialization__expected_msg_0,__zpabi_validate_partial_initialization__expected_msg_1 param_regs=A,X,,
; ext validate_two_structs params=__zpabi_validate_two_structs__ptr1_0,__zpabi_validate_two_structs__ptr1_1,__zpabi_validate_two_structs__ptr2_0,__zpabi_validate_two_structs__ptr2_1 param_regs=A,X,,
; call main -> test_full_initialization
; call main -> test_implicit_type_conversions
; call main -> test_partial_initialization
; call main -> test_single_exp_initializer
; call test_full_initialization -> get_double
; call test_full_initialization -> validate_full_initialization
; call test_implicit_type_conversions -> calloc
; call test_implicit_type_conversions -> malloc
; call test_implicit_type_conversions -> validate_converted
; call test_partial_initialization -> calloc
; call test_partial_initialization -> validate_partial_initialization
; call test_single_exp_initializer -> validate_two_structs
; item get_double section=.text local=false zero=false
; item test_full_initialization section=.text local=false zero=false
; item test_partial_initialization section=.text local=false zero=false
; item test_partial_initialization@run0 section=.text local=true zero=false
; item test_implicit_type_conversions section=.text local=false zero=false
; item test_implicit_type_conversions@run2 section=.text local=true zero=false
; item test_implicit_type_conversions@run1 section=.text local=true zero=false
; item test_single_exp_initializer section=.text local=false zero=false
; item main section=.text local=false zero=false
; item str@0 section=.rodata local=true zero=false
; item str@1 section=.rodata local=true zero=false
; item str@2 section=.rodata local=true zero=false
; item @12.msg section=.data local=true zero=false
; item @14.i section=.data local=true zero=false
; runtime hargs_size=32 hargs_need=24
; @zp-link-meta-end

get_double:
   SUBROUTINE

   LDA   #$00
   STA   HARGS+16
   LDY   #$00
   STY   HARGS+17
   STA   HARGS+18
   LDA   #$A2
   STA   HARGS+19
   LDA   #$94
   STA   HARGS+20
   LDA   #$1A
   STA   HARGS+21
   LDA   #$7D
   STA   HARGS+22
   LDA   #$42
   STA   HARGS+23
   RTS

test_full_initialization:
   SUBROUTINE

   LDA   #<str@0
   STA   __local_test_full_initialization__full
   LDA   #>str@0
   STA   __local_test_full_initialization__full+1
   LDA   #$73
   STA   __local_test_full_initialization__full+2
   LDA   #$75
   STA   __local_test_full_initialization__full+3
   LDA   #$70
   STA   __local_test_full_initialization__full+4
   LDA   #<__local_test_full_initialization__full
   STA   __local_test_full_initialization__full+5
   LDA   #>__local_test_full_initialization__full
   STA   __local_test_full_initialization__full+6
   JSR   get_double
   LDA   HARGS+23
   STA   __local_test_full_initialization__full+14
   LDA   HARGS+16
   STA   __local_test_full_initialization__full+7
   LDA   HARGS+17
   STA   __local_test_full_initialization__full+8
   LDA   HARGS+18
   STA   __local_test_full_initialization__full+9
   LDA   HARGS+19
   STA   __local_test_full_initialization__full+10
   LDA   HARGS+20
   STA   __local_test_full_initialization__full+11
   LDA   HARGS+21
   STA   __local_test_full_initialization__full+12
   LDA   HARGS+22
   STA   __local_test_full_initialization__full+13
   LDA   #>__local_test_full_initialization__full
   STA   __local_test_full_initialization__2
   LDA   #<__local_test_full_initialization__full
   CLC
   ADC   #$07
   BCC   .carry_inc@0
   INC   __local_test_full_initialization__2
.carry_inc@0:
   STA   __local_test_full_initialization__full+15
   LDA   __local_test_full_initialization__2
   STA   __local_test_full_initialization__full+16
   LDX   #>__local_test_full_initialization__full
   LDA   #<__local_test_full_initialization__full
   JMP   validate_full_initialization

test_partial_initialization:
   SUBROUTINE

   LDA   @12.msg
   STA   __local_test_partial_initialization__partial
   LDA   @12.msg+1
   STA   __local_test_partial_initialization__partial+1
   LDA   #$61
   STA   __local_test_partial_initialization__partial+2
   LDA   #$62
   STA   __local_test_partial_initialization__partial+3
   LDA   #$00
   STA   __local_test_partial_initialization__partial+4
   LDA   #$01
   STA   __zpabi_calloc__nmemb_0
   LDA   #$11
   JSR   test_partial_initialization@run0
   STA   __local_test_partial_initialization__partial+5
   STX   __local_test_partial_initialization__partial+6
   LDA   #$0A
   STA   __local_test_partial_initialization__4
.clear@0:
   DEC   __local_test_partial_initialization__4
   LDA   __local_test_partial_initialization__4
   CLC
   ADC   #<__local_test_partial_initialization__partial
   STA   __local_test_partial_initialization__0
   LDA   #$00
   ADC   #>__local_test_partial_initialization__partial
   STA   __local_test_partial_initialization__1
   LDA   #$00
   LDY   #$07
   STA   (__local_test_partial_initialization__0),Y
   LDA   __local_test_partial_initialization__4
   BNE   .clear@0
   LDA   @12.msg
   STA   __zpabi_validate_partial_initialization__expected_msg_0
   LDA   @12.msg+1
   STA   __zpabi_validate_partial_initialization__expected_msg_1
   LDX   #>__local_test_partial_initialization__partial
   LDA   #<__local_test_partial_initialization__partial
   JMP   validate_partial_initialization

test_partial_initialization@run0:
   SUBROUTINE

   STA   __zpabi_calloc__size_0
   LDA   #$00
   STA   __zpabi_calloc__nmemb_1
   STA   __zpabi_calloc__nmemb_2
   STA   __zpabi_calloc__nmemb_3
   STA   __zpabi_calloc__size_1
   STA   __zpabi_calloc__size_2
   STA   __zpabi_calloc__size_3
   JMP   calloc

test_implicit_type_conversions:
   SUBROUTINE

   LDA   #$05
   STA   __zpabi_malloc__size_0
   LDA   #$00
   STA   __zpabi_malloc__size_1
   STA   __zpabi_malloc__size_2
   STA   __zpabi_malloc__size_3
   JSR   malloc
   STA   __local_test_implicit_type_conversions__converted
   STX   __local_test_implicit_type_conversions__converted+1
   JSR   test_implicit_type_conversions@run2
   LDA   #$02
   JSR   test_implicit_type_conversions@run1
   STA   __local_test_implicit_type_conversions__converted+2
   JSR   test_implicit_type_conversions@run2
   LDA   #$03
   JSR   test_implicit_type_conversions@run1
   STA   __local_test_implicit_type_conversions__converted+3
   LDA   @14.i
   ASL
   ASL
   STA   __local_test_implicit_type_conversions__converted+4
   LDA   #$00
   STA   __local_test_implicit_type_conversions__converted+5
   STA   __local_test_implicit_type_conversions__converted+6
   LDA   @14.i
   SEC
   SBC   #$01
   STA   HARGS
   LDA   @14.i+1
   SBC   #$00
   STA   HARGS+1
   JSR   i2d
   LDA   HARGS+2
   STA   __local_test_implicit_type_conversions__converted+7
   LDA   HARGS+3
   STA   __local_test_implicit_type_conversions__converted+8
   LDA   HARGS+4
   STA   __local_test_implicit_type_conversions__converted+9
   LDA   HARGS+5
   STA   __local_test_implicit_type_conversions__converted+10
   LDA   HARGS+6
   STA   __local_test_implicit_type_conversions__converted+11
   LDA   HARGS+7
   STA   __local_test_implicit_type_conversions__converted+12
   LDA   HARGS+8
   STA   __local_test_implicit_type_conversions__converted+13
   LDA   HARGS+9
   STA   __local_test_implicit_type_conversions__converted+14
   LDA   #$01
   STA   __zpabi_calloc__nmemb_0
   LDA   #$08
   JSR   test_partial_initialization@run0
   STA   __local_test_implicit_type_conversions__converted+15
   STX   __local_test_implicit_type_conversions__converted+16
   LDX   #>__local_test_implicit_type_conversions__converted
   LDA   #<__local_test_implicit_type_conversions__converted
   JMP   validate_converted

test_implicit_type_conversions@run2:
   SUBROUTINE

   LDA   @14.i
   STA   HARGS
   LDA   @14.i+1
   STA   HARGS+1
   RTS

test_implicit_type_conversions@run1:
   SUBROUTINE

   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   JSR   sdivmod16
   LDA   HARGS+4
   RTS

test_single_exp_initializer:
   SUBROUTINE

   LDA   #$9A
   STA   __local_test_single_exp_initializer__d
   LDA   #$99
   STA   __local_test_single_exp_initializer__d+1
   STA   __local_test_single_exp_initializer__d+2
   STA   __local_test_single_exp_initializer__d+3
   STA   __local_test_single_exp_initializer__d+4
   LDA   #$D9
   STA   __local_test_single_exp_initializer__d+5
   LDA   #$5E
   STA   __local_test_single_exp_initializer__d+6
   LDA   #$40
   STA   __local_test_single_exp_initializer__d+7
   LDA   #<str@2
   STA   __local_test_single_exp_initializer__s1
   LDA   #>str@2
   STA   __local_test_single_exp_initializer__s1+1
   LDA   #$78
   STA   __local_test_single_exp_initializer__s1+2
   LDA   #$79
   STA   __local_test_single_exp_initializer__s1+3
   LDA   #<__local_test_single_exp_initializer__s1
   STA   __local_test_single_exp_initializer__s1+5
   LDA   #>__local_test_single_exp_initializer__s1
   STA   __local_test_single_exp_initializer__s1+6
   LDA   #$00
   STA   __local_test_single_exp_initializer__s1+4
   STA   __local_test_single_exp_initializer__s1+7
   LDY   #$00
   STY   __local_test_single_exp_initializer__s1+8
   STY   __local_test_single_exp_initializer__s1+9
   STY   __local_test_single_exp_initializer__s1+10
   STA   __local_test_single_exp_initializer__s1+11
   LDA   #$C0
   STA   __local_test_single_exp_initializer__s1+12
   LDA   #$62
   STA   __local_test_single_exp_initializer__s1+13
   LDA   #$40
   STA   __local_test_single_exp_initializer__s1+14
   LDA   #<__local_test_single_exp_initializer__d
   STA   __local_test_single_exp_initializer__s1+15
   LDA   #>__local_test_single_exp_initializer__d
   STA   __local_test_single_exp_initializer__s1+16
   LDA   #<str@2
   STA   __local_test_single_exp_initializer__s2
   LDA   #>str@2
   STA   __local_test_single_exp_initializer__s2+1
   LDA   #$78
   STA   __local_test_single_exp_initializer__s2+2
   LDA   #$79
   STA   __local_test_single_exp_initializer__s2+3
   STY   __local_test_single_exp_initializer__s2+4
   LDA   #<__local_test_single_exp_initializer__s1
   STA   __local_test_single_exp_initializer__s2+5
   LDA   #>__local_test_single_exp_initializer__s1
   STA   __local_test_single_exp_initializer__s2+6
   TYA
   STA   __local_test_single_exp_initializer__s2+7
   STY   __local_test_single_exp_initializer__s2+8
   STY   __local_test_single_exp_initializer__s2+9
   STY   __local_test_single_exp_initializer__s2+10
   STA   __local_test_single_exp_initializer__s2+11
   LDA   #$C0
   STA   __local_test_single_exp_initializer__s2+12
   LDA   #$62
   STA   __local_test_single_exp_initializer__s2+13
   LDA   #$40
   STA   __local_test_single_exp_initializer__s2+14
   LDA   #<__local_test_single_exp_initializer__d
   STA   __local_test_single_exp_initializer__s2+15
   LDA   #>__local_test_single_exp_initializer__d
   STA   __local_test_single_exp_initializer__s2+16
   LDA   #<__local_test_single_exp_initializer__s2
   STA   __zpabi_validate_two_structs__ptr2_0
   LDA   #>__local_test_single_exp_initializer__s2
   STA   __zpabi_validate_two_structs__ptr2_1
   LDX   #>__local_test_single_exp_initializer__s1
   LDA   #<__local_test_single_exp_initializer__s1
   JMP   validate_two_structs

main:
   SUBROUTINE

   JSR   test_full_initialization
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@1
   LDX   #$00
   LDA   #$01
   RTS
.if_end@1:
   JSR   test_partial_initialization
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@2
   LDX   #$00
   LDA   #$02
   RTS
.if_end@2:
   JSR   test_implicit_type_conversions
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@3
   LDX   #$00
   LDA   #$03
   RTS
.if_end@3:
   JSR   test_single_exp_initializer
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@4
   LDX   #$00
   LDA   #$04
   RTS
.if_end@4:
   LDX   #$00
   TXA
   RTS

str@0:
   DC.B  $49, $27, $6D, $20, $61, $20, $73, $74, $72, $75, $63, $74, $21, $00

str@1:
   DC.B  $41, $6E, $6F, $74, $68, $65, $72, $20, $73, $74, $72, $69, $6E, $67, $20, $6C
   DC.B  $69, $74, $65, $72, $61, $6C, $00

str@2:
   DC.B  $59, $65, $74, $20, $61, $6E, $6F, $74, $68, $65, $72, $20, $73, $74, $72, $69
   DC.B  $6E, $67, $00

@12.msg:
   DC.W  str@1

@14.i:
   DC.W  $0BB8
